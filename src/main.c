#include "_includes_.h"

int main() {
    bool is_running = true;
    int wait;
    //wait = 0 normal command execute | wait = 1 command execute in background.
    char *argv[BUFFER_LENGTH];
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

        if (user_cmd[0] == '!')
        {
            int index = getIndex(user_cmd, history_count);
            char *last_cmd = get_history(cmd_history, history_count, index);
            
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
