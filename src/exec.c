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

void child_tofile(char **argv, char **dir) {
    int fd; //file description
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
