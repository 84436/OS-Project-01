// Universal includes and macros

#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#define LINE_LENGTH 100
#define SHELLNAME "banhxeo> " //flexible <?>
#define BUFFER_LENGTH 100
#define HISTSIZE 20 //size of history array

// Parser
void parse_cmd(char input[], char *argv[], int *wait);

// Execution
void child(char *argv[]);
void parent(pid_t child_pid,int wait);

// History
void init_history(char *history[LINE_LENGTH]);
void append_history(char *history[LINE_LENGTH], const char *cmd, int *history_count);
char *get_history(char *history[LINE_LENGTH], int history_count, int index);
void export_history(char *history[LINE_LENGTH], int history_count);
int getIndex(char cmd[], int history_count);


//Built-in
int built_in_cd(char **args);