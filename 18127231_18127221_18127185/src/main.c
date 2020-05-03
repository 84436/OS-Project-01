#include "_includes_.h"

int main() {
    setenv("SHELLNAME", PS1_DEFAULT, 1);        // Default shell prompt
    setenv("HISTSIZE", HISTSIZE_DEFAULT, 1);    // Default history size

    bool is_running = true;
    int wait;                                   // Should the command run in background?
    char *argv[BUFFER_LENGTH];
    char *argv2[BUFFER_LENGTH];
    unsigned op = 0;
    char user_cmd[LINE_LENGTH];

    //where the history is saved
    char* HISTSIZE_string = getenv("HISTSIZE");
    unsigned HISTSIZE = get_number(HISTSIZE_string);
    int history_count = 0;
    char** cmd_history = (char**) malloc (HISTSIZE * sizeof(char*));
    init_history(cmd_history, HISTSIZE);
    
    while (is_running) {
        char* SHELLNAME = getenv("SHELLNAME");
        
        //move in function
        HISTSIZE_string = getenv("HISTSIZE");
        HISTSIZE = get_number(HISTSIZE_string);

        // Prompt for input
        printf("%s", SHELLNAME);
        fflush(stdout);
        while (fgets(user_cmd, LINE_LENGTH, stdin) == NULL) {
            perror("Wrong input!");
            fflush(stdin);
        }

        user_cmd[strcspn(user_cmd, "\n")] = '\0';
        //Trailing endline character (\n). For windows, change it to \r\n
        if (user_cmd[0] == '\0')
            continue;

        if (strcmp(user_cmd, "exit") == 0)
        {
            is_running = false;
            continue;
        }

        if (user_cmd[0] == '!')
        {
            int index = get_index(user_cmd, history_count);
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
        append_history(cmd_history, user_cmd, &history_count, HISTSIZE);

        // Parse the command
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

        // Built-in command check
        if (strcmp(user_cmd,"cd") == 0)
            if (built_in_cd(argv))
                continue;

        if (strcmp(user_cmd, "PS1") == 0)
            if (built_in_PS1(argv))
                continue;

        if (strcmp(user_cmd, "help") == 0)
            if (built_in_help(argv))
                continue;

        if(strcmp(user_cmd, "history") == 0)
            if (built_in_history(argv, cmd_history, &history_count))
                continue;

        if(strcmp(user_cmd, "histsize") == 0)
            if (built_in_histsize(argv, &cmd_history, &history_count, HISTSIZE))
                continue;
        
        // That command was not one of the built-ins.
        pid_t pid = fork();
        switch (pid) {
            case -1:
                perror("fork() failed!");
                exit(EXIT_FAILURE);
            case 0:
                switch (op)
                {
                    case OP_FROMFILE:
                        child_fromfile(argv, argv2);
                        break;
                    case OP_TOFILE:
                    case OP_TOFILE_APP:
                        child_tofile(argv, argv2, (op == OP_TOFILE_APP));
                        break;
                    case OP_PIPE:
                        child_pipe(argv, argv2);
                        break;
                    default:
                        child(argv);
                        break;
                }
                exit(EXIT_FAILURE);
            default: {
                parent(pid,wait);
            }
                //TODO: Handle exception
        }
    }
    free_history(cmd_history, HISTSIZE);
    return 0;
}
