#include "_includes_.h"

void init_history(char **history, unsigned HISTSIZE)
{
    for (int i = 0; i < HISTSIZE; i++)
    {
        history[i] = (char *)malloc(LINE_LENGTH * sizeof(char));
    }
}

void free_history(char **history, unsigned HISTSIZE)
{
    for (int i = 0; i < HISTSIZE; i++)
    {
        if (history[i] != NULL)
        {
            free(history[i]);
        }
    }
}

void append_history(char** history, const char *cmd, int *history_count, unsigned HISTSIZE)
{
    int current_history = (int)*history_count;
    if (current_history != 0)
    {
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

char *get_history(char **history, int history_count, int index)
{
    if (index > history_count || index - 1 < 0 || history_count == 0)
    {
        return NULL;
    }
    return history[index - 1];
}

void export_history(char **history, int history_count)
{
    if (history[0] == NULL)
        printf("No history found");
    for (int i = 0; i < history_count; i++)
    {
        printf("[%d] %s\n", i + 1, history[i]);
    }
}

int get_index(char *cmd, int history_count)
{
    char *temp = cmd;

    if (cmd[1] == '!' && cmd[2] == '\0')
        return history_count;

    memmove(cmd, cmd + 1, strlen(cmd));

    int index = get_number(cmd);
    cmd = temp;
    return index;
}