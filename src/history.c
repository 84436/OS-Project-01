#include "_includes_.h"

void init_history(char *history[LINE_LENGTH]) {
    for (int i = 0; i < HISTSIZE; i++) {
        history[i] = (char *) malloc(LINE_LENGTH);
    }
}


void free_history(char **history) {
    for (int i = 0; i < HISTSIZE; i++) {
        if (history[i] != NULL) {
            free(history[i]);
        }
    }
}

void append_history(char *history[LINE_LENGTH], const char *cmd, int *history_count) {
    int current_history = (int) *history_count;
    if (current_history != 0) {
        if (strcmp(history[current_history - 1], "history") == 0 && strcmp(history[current_history - 1], cmd) == 0)
            return;
    }

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

char *get_history(char *history[LINE_LENGTH], int history_count, int index)
{
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

int getIndex(char cmd[], int history_count)
{
    if (cmd[1] == '!' && cmd[2] == '\0')
        return history_count;
    
    char *index_array = (char *)malloc(LINE_LENGTH - 1);
    int i = 1;
    while (cmd[i] != '\0')
    {
        if(!isdigit(cmd[i]))
            return -1;

        index_array[i - 1] = cmd[i];
        i = i + 1;
    }

    return atoi(index_array);
}