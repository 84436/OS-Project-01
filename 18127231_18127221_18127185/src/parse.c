#include "_includes_.h"

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
