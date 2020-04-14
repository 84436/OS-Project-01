#include "_includes_.h"

// Manual parsing without strchr/strtok
void parse2(char* input, char** args1, unsigned* op, char** args2)
{
    // Clear all
    memset(args1, 0, BUFFER_LENGTH);
    memset(args2, 0, BUFFER_LENGTH);
    *op = 0;

     // Internal state vars
    bool new_token = true;
    bool args2_turn = false;
    unsigned args1_i = 0;
    unsigned args2_i = 0;

    // Parse
    unsigned ilen = strlen(input);
    for (unsigned i = 0; i < ilen; i++)
    {
        switch (input[i])
        {
            // Terminate string
            case ' ':
                new_token = true;                       input[i] = 0;
                break;
            
            // Mark the presence of |, <, > or & indicating args1 has finished
            case '|':
                new_token = true;   args2_turn = true;  input[i] = 0;
                *op = (*op == 0)    ? OP_PIPE           : OP_NOTSP;
                break;
            case '<':
                new_token = true;   args2_turn = true;  input[i] = 0;
                *op = (*op == 0)    ? OP_FROMFILE       : OP_NOTSP;
                break;
            case '>':
                new_token = true;   args2_turn = true;  input[i] = 0;
                *op = (*op == 0)    ? OP_TOFILE         : OP_NOTSP;
                break;
            case '&':
                new_token = true;   args2_turn = true;  input[i] = 0;
                *op = (*op == 0)    ? OP_BG             : OP_NOTSP;
                break;
            
            // Mark the beginning of a new token into either args1 or args2
            default:
                if (new_token)
                {
                    if (args2_turn) { args2[args2_i++] = (input+i); }
                    else            { args1[args1_i++] = (input+i); }
                    new_token = false;
                }
                break;
        }

        // Immediately stop on NOTSP
        if (*op == OP_NOTSP) break;     
    }

    // Op validity check
    switch (*op)
    {
        // |, <, >: args1 must exist
        case OP_FROMFILE:
        case OP_TOFILE:
        case OP_PIPE:
            if (args1_i == 0) { *op = OP_NOTSP; }
            break;
        
        // &: args2 must not exist
        case OP_BG:
            if (args2_i != 0) { *op = OP_NOTSP; }
            break;
    }

    // DIRTY DEBUGGING
    // for (unsigned i = 0; i < args1_i; i++)  printf("args1[%d]: %s\n", i, args1[i]);
    // for (unsigned i = 0; i < args2_i; i++)  printf("args2[%d]: %s\n", i, args2[i]);
    // printf("op: ");
    // switch (*op)
    // {
    //     case OP_FROMFILE:   printf("From file");                    break;
    //     case OP_TOFILE:     printf("To file");                      break;
    //     case OP_PIPE:       printf("Pipe");                         break;
    //     case OP_BG:         printf("Background");                   break;
    //     case OP_NOTSP:      printf("Not supported");                break;
    //     case 0:             printf("No-op");                        break;
    //     default:            printf("Something weird happened.");    break;
    // }
    // printf("\n");
}
