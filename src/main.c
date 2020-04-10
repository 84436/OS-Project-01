
#include <unistd.h> 
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h> 
#include <stdlib.h>

#define LINE_LENGTH 100
#define SHELLNAME "banhxeo> " //flexible <?>
#define BUFFER_LENGTH 100

void parse_cmd(char input[], char *argv[], int *wait)

{
    memset(argv,'\0',BUFFER_LENGTH);

    //TODO: HANDLE PIPE AND BG PROCESS OR I/O
    
    *wait = 0;

    const char *delim = " ";
    unsigned i = 0;
    char *token = strtok(input, delim);
    while (token != NULL)
    {
        argv[i] = token;
        i = i + 1;
        token = strtok(NULL,delim);
    }

    argv[i] = NULL;
}
int main()
{

    bool is_running = true;
    pid_t pid;
    int wait;
    char *argv[BUFFER_LENGTH];
    int status = 0;
    char user_cmd[LINE_LENGTH];
    
    while(is_running)
    {
        printf(SHELLNAME);
        fflush(stdout);
        while(fgets(user_cmd, LINE_LENGTH, stdin) == NULL)
        {
            perror("Wrong input!");
            fflush(stdin);
        }

        user_cmd[strcspn(user_cmd,"\n")] = '\0'; //Split \n from string and end it with \0 aka trailing endline character. (Windows: \r\n).

        if (strcmp(user_cmd,"exit") == 0)
        {
           is_running = false;
            continue;
        }
       
        //TODO: tokenizer here
        parse_cmd(user_cmd,argv, &wait);

        //TODO: pipe and i/o here
        

        pid_t pid = fork();

        switch(pid)
        {
            case -1:
                perror("fork() failed!");
                exit(EXIT_FAILURE);
            case 0:
                if ( execvp(argv[0], argv) == -1)
                {
                    perror("Failed to execute cmd");
                    exit(EXIT_FAILURE);
                }
               
                exit(EXIT_FAILURE);

            default:
                {
                int status;
                switch(wait)

                {
                    case 0: {
                                waitpid(pid, &status,0);
                                break;                           
                            }
                    default:
                            waitpid(pid, &status, WUNTRACED);
                            break;

                }

              }

                //TODO: Handle exception
        }

    }

	return 0;
}

