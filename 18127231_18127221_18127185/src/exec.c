#include "_includes_.h"

void child(char *argv[]) {
    if (execvp(argv[0], argv) == -1) {
        perror("Failed to execute cmd");
        exit(EXIT_FAILURE);
    }
}

void child_fromfile(char **argv, char **dir) {
    int fd; //file description
    fd = open(dir[0], O_RDONLY); //O_RDONLY: Open for reading only
    if (fd == -1) {
        perror("Redirect to input file failed");
        exit(EXIT_FAILURE);
    }
    dup2(fd, STDIN_FILENO); //STDIN_FILENO: File number of stdin; 0.
    if (close(fd) == -1) {
        perror("Closing input file failed");
        exit(EXIT_FAILURE);
    }
    child(argv);
}

void child_tofile(char **argv, char **dir, bool is_append) {
    int fd; //file description

    // Append only if ">>" AND file exists
    if (is_append && access(dir[0], F_OK) != -1)
        /* Quoted from open(2):
         * "The argument flags MUST include one of the following access modes:
         * O_RDONLY, O_WRONLY, or O_RDWR. These request opening the file
         * read-only, write-only, or read/write, respectively."
         */
        fd = open(dir[0], O_WRONLY | O_APPEND);
    else
        fd = creat(dir[0], S_IRWXU); //S_IRWXU: Godmod(7) : read - write - execute
    
    if (fd == -1) {
        perror("Redirect to output file failed");
        exit(EXIT_FAILURE);
    }
    dup2(fd, STDOUT_FILENO); //STDOUT_FILENO: File number of stdout; 1.
    if (close(fd) == -1) {
        perror("Closing output file failed");
        exit(EXIT_FAILURE);
    }
    child(argv);
}

void child_pipe(char **argv_in, char **argv_out) {
    int fd[2]; //file description of in: fd[0] and out: fd[1]

    if (pipe(fd) == -1) //fd[0] is set up for reading, fd[1] is set up for writing
    {
        perror("Failed to pipe cmd");
        exit(EXIT_FAILURE);
    }
    //first argv's fork
    if (fork() == 0) {
        dup2(fd[1], STDOUT_FILENO);

        if (close(fd[0]) == -1 || close(fd[1]) == -1) {
            perror("Unexpected error when make a pipe I/O");
        }
        child(argv_in);
        exit(EXIT_FAILURE); //prevent access to second argv's fork
    }
    //second argv's fork
    if (fork() == 0) {
        dup2(fd[0], STDIN_FILENO);
        if (close(fd[1]) == -1 || close(fd[0]) == -1) {
            perror("Unexpected error when make a pipe I/O");
        }
        child(argv_out);
        exit(EXIT_FAILURE);
    }

    close(fd[0]);
    close(fd[1]);
    //wait for child 1
    wait(0);
    //wait for child 2
    wait(0);
}

void parent(pid_t child_pid, int wait) {
    static int count_bg_process = 0;
    int status;
    switch (wait) {
        // parent and child run concurrently, normal case.
        case 0: {
            waitpid(child_pid, &status, 0);
            break;
        }
        // parrent wait for child - when child terminated, parent rev and rerun.
        // case '&' | run in background.
        default:
            count_bg_process += 1;
            printf("[%d] %d\n",count_bg_process,child_pid);
            waitpid(child_pid, &status, WUNTRACED);
            if (WIFEXITED(status))
            //This macro returns a nonzero value if the child process terminated normally with exit or _exit.
            {
                count_bg_process -=1;
                printf("[%d] is finished and exited with status %d\n",child_pid,status);
            }
            break;
    }
}
