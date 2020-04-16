#include "_includes_.h"

int main() {
    setenv("SHELLNAME", PS1_DEFAULT, 1); //startup shell name

    bool is_running = true;
    int wait;
    //wait = 0 normal command execute | wait = 1 command execute in background.
    char *argv[BUFFER_LENGTH];
    char *argv2[BUFFER_LENGTH]; // parse2()
    unsigned op = 0;            // parse2()
    char user_cmd[LINE_LENGTH];

    //where the history is saved
    int history_count = 0;
    char *cmd_history[LINE_LENGTH];
    init_history(cmd_history);

    while (is_running) {
        char* SHELLNAME = getenv("SHELLNAME");
        printf("%s", SHELLNAME);
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
        // parse_cmd(user_cmd, argv, &wait);
        parse2(user_cmd, argv, &op, argv2);
        wait = 0;
        switch (op)
        {
            case OP_BG:
                wait = 1; // Compatiability
                break;
            case OP_FROMFILE:
                break;
            case OP_TOFILE:
                break;
            case OP_PIPE:
                break;
            case OP_NOTSP:
                printf("op: Invalid or not supported syntax.\n");
                break;
            default:
                break;
        }

        if (strcmp(user_cmd,"cd") == 0)
        {
            if (built_in_cd(argv))
                continue;
        }

        if (strcmp(user_cmd, "PS1") == 0){
            if (built_in_PS1(argv))
                continue;
        }

        if (strcmp(user_cmd, "help") == 0)
        {
            if (built_in_help(argv))
                continue;
        }

        pid_t pid = fork();

        switch (pid) {
            case -1:
                perror("fork() failed!");
                exit(EXIT_FAILURE);
            case 0:
                // parse2() : temporary block
                if (!(op == OP_FROMFILE || op == OP_TOFILE || op == OP_PIPE || op == OP_NOTSP))
                    child(argv);
                else if (op == OP_FROMFILE)
                    child_fromfile(argv, argv2);
                else if (op == OP_TOFILE)
                    child_tofile(argv, argv2);
                else if (op == OP_PIPE)
                    child_pipe(argv, argv2);
                exit(EXIT_FAILURE);
            default: {
                parent(pid,wait);
            }

                //TODO: Handle exception
        }

    }

    free_history(cmd_history);

    return 0;
}
