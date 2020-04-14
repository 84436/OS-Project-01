
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define LINE_LENGTH 100
#define SHELLNAME "banhxeo> " //flexible <?>
#define BUFFER_LENGTH 100
#define HISTSIZE 10 //size of history array

void init_history(char *history[LINE_LENGTH])
{
    for (int i = 0; i < HISTSIZE; i++)
    {
        history[i] = (char *)malloc(LINE_LENGTH);
    }
}

void append_history(char *history[LINE_LENGTH], const char *cmd, int *history_count)
{
    int current_history = (int)*history_count;

    if (current_history == 0)
    {
        strcpy(history[0], cmd);
        current_history = current_history + 1;
    }
    else if (current_history < HISTSIZE)
    {
        strcpy(history[current_history], cmd);
        current_history = current_history + 1;
    }
    else
    {
        for (int i = 1; i < HISTSIZE; i++)
        {
            strcpy(history[i - 1], history[i]);
        }

        strcpy(history[HISTSIZE - 1], cmd);
    }

    *history_count = current_history;
}

char *get_history(char *history[LINE_LENGTH], int history_count, const char x)
{
    int index = x - '0';

    if (index > history_count || index - 1 < 0 || history_count == 0)
    {
        return NULL;
    }
    return history[index - 1];
}

void export_history(char *history[LINE_LENGTH], int history_count)
{
    if (history[0] == NULL)
        printf("No history found");
    for (int i = 0; i < history_count; i++)
    {
        printf("[%d] %s\n", i + 1, history[i]);
    }
}

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

    //where the history is saved
    int history_count = 0;
    char *cmd_history[LINE_LENGTH];
    init_history(cmd_history);

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

        if (user_cmd[0] == '!' && (user_cmd[1] == '!' || user_cmd[1] != '\0'))
        {
            if (user_cmd[1] == '!')
                user_cmd[1] = '1';

            char *last_cmd = get_history(cmd_history, history_count, user_cmd[1]);
            if (last_cmd != NULL){
                strcpy(user_cmd, last_cmd);
                printf("%s%s\n",SHELLNAME, user_cmd);
                fflush(stdout);
            }
            else {
                printf("No command available\n");
                fflush(stdout);
                continue;
            }
        }

        // append new cmd to history
        append_history(cmd_history, user_cmd, &history_count);

        if (strcmp(user_cmd, "history") == 0)
        {
            export_history(cmd_history, history_count);
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

    //Free history space?

    return 0;
}

