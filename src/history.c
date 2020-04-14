#include "_includes_.h"

void init_history(char *history[LINE_LENGTH])
{
    for (int i = 0; i < HISTSIZE; i++)
    {
        history[i] = (char *)malloc(LINE_LENGTH);
    }
}

void append_history(char *history[LINE_LENGTH], const char *cmd, int *history_count)
{
    int current_history = (int)*history_count;

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

char *get_history(char *history[LINE_LENGTH], int history_count, const char x)
{
    int index = x - '0';

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
