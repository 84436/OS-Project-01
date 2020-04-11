
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define LINE_LENGTH 100
#define SHELLNAME "banhxeo> " //flexible <?>
#define BUFFER_LENGTH 100




void child(char *argv[])
{
    if (execvp(argv[0], argv) == -1) {
        perror("Failed to execute cmd");
        exit(EXIT_FAILURE);
    }
}

void parent(pid_t child_pid,int wait)
{
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
            waitpid(child_pid, &status, WUNTRACED);
            //For debug:
            if (WIFEXITED(status) == 0)
            //This macro returns a nonzero value if the child process terminated normally with exit or _exit.
            {
                printf("$ Error");
            }

            else
            {
                printf("Child process %d exit with status %d.\n",child_pid,status);
            }
            break;
    }
}

void parse_cmd(char input[], char *argv[], int *wait) {
    memset(argv, '\0', BUFFER_LENGTH);

    //Check for trailing \& - set wait to 1 if execute in background
    if ((input[strlen(input) - 1]) == '&') {
        *wait = 1;
        input[strlen(input) - 1] = '\0';
    } else { *wait = 0; }




    //Tokenization to make cmd -> arguments for execvp(cmd, argv);
    //argv : |cmd|suffix|...
    //eg   : |ps |aux   |
    const char *delim = " ";
    unsigned i = 0;
    char *token = strtok(input, delim);
    while (token != NULL) {
        argv[i] = token;
        i = i + 1;
        token = strtok(NULL, delim);
    }

    argv[i] = NULL;
}

int main() {
    bool is_running = true;
    pid_t pid;
    int wait;
    //wait = 0 normal command execute | wait = 1 command execute in background.
    char *argv[BUFFER_LENGTH];
    int status = 0;
    char user_cmd[LINE_LENGTH];

    while (is_running) {
        printf(SHELLNAME);
        fflush(stdout);
        while (fgets(user_cmd, LINE_LENGTH, stdin) == NULL) {
            perror("Wrong input!");
            fflush(stdin);
        }

        user_cmd[strcspn(user_cmd, "\n")] = '\0';
        //Trailing endline character (\n). For windows, change it to \r\n

        if (strcmp(user_cmd, "exit") == 0) {
            is_running = false;
            continue;
        }

        //TODO: tokenizer here
        parse_cmd(user_cmd, argv, &wait);

        //TODO: pipe and i/o here


        pid_t pid = fork();

        switch (pid) {
            case -1:
                perror("fork() failed!");
                exit(EXIT_FAILURE);
            case 0:
                child(argv);
                exit(EXIT_FAILURE);
            default: {
                parent(pid,wait);
            }

                //TODO: Handle exception
        }

    }

    return 0;
}

