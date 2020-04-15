// Universal includes and macros

#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>

#define LINE_LENGTH 100
#define SHELLNAME "banhxeo> " //flexible <?>
#define BUFFER_LENGTH 100
#define HISTSIZE 20 //size of history array

#define OP_NOTSP    0x15 // NAK; not supported 
#define OP_BG       0x26 // &
#define OP_FROMFILE 0x3C // <
#define OP_TOFILE   0x3E // >
#define OP_PIPE     0x7C // |

// Parser
void parse_cmd(char input[], char *argv[], int *wait);

void parse2(char *input, char **args1, unsigned *op, char **args2);

// Execution
void child(char *argv[]);

void child_fromfile(char **argv, char **dir);

void child_tofile(char **argv, char **dir);

void parent(pid_t child_pid, int wait);

// History
void init_history(char *history[LINE_LENGTH]);

void free_history(char *history[LINE_LENGTH]);

void append_history(char *history[LINE_LENGTH], const char *cmd, int *history_count);

char *get_history(char *history[LINE_LENGTH], int history_count, int index);

void export_history(char *history[LINE_LENGTH], int history_count);

int getIndex(char cmd[], int history_count);


//Built-in
int built_in_cd(char **args);