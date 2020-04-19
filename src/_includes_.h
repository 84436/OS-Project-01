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

#define LINE_LENGTH         100
#define BUFFER_LENGTH       100
#define HISTSIZE_DEFAULT    "20"     // size of history array
#define PS1_DEFAULT         "banhxeo> "

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

void child_pipe(char **argv_in, char **argv_out);

// History
void init_history(char **history, unsigned HISTSIZE);

void free_history(char **history, unsigned HISTSIZE);

void append_history(char **history, const char *cmd, int *history_count, unsigned HISTSIZE);

char *get_history(char **history, int history_count, int index);

void export_history(char **history, int history_count);

int get_index(char cmd[], int history_count);

//Built-in
int built_in_cd(char **args);

int built_in_PS1(char** args);

int built_in_help(char** args);

int built_in_history(char** args, char** history, int* history_count);

int built_in_histsize(char** args, char*** history, int* history_count, unsigned int HISTSIZE);

//Where to put?
int get_number(char* number_string); //build-in.c