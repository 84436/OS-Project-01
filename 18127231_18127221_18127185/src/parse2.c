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
    unsigned op_tofrom_potential = 0;
    /* "Potential" variable for >, <
     * 0 = not yet
     * 1 = there is potential
     * 2 = nope.
     * When 2 is set, any further >/< will result in invalidation.
     */

    // Parse
    unsigned ilen = strlen(input);
    for (unsigned i = 0; i < ilen; i++)
    {
        switch (input[i])
        {
            // Terminate string
            case ' ':
                input[i] = 0; new_token = true;
                if (op_tofrom_potential == 1) op_tofrom_potential = 2;  // >, <: potential lost
                break;
            
            // Mark the presence of |, <, > or & indicating args1 has finished
            case '|':
                input[i] = 0; new_token = true; args2_turn = true;  
                *op = (*op == 0) ? OP_PIPE : OP_NOTSP;
                break;
            
            case '<':
                input[i] = 0; new_token = true; args2_turn = true;  
                switch (op_tofrom_potential)
                {
                    case 0:     op_tofrom_potential = 1;
                                *op = (*op == 0) ? OP_FROMFILE : OP_NOTSP;
                                break;

                    case 1:     if (*op == OP_FROMFILE)
                                    { op_tofrom_potential = 2; *op = OP_FROMFILE_APP; }
                                else
                                    *op = OP_NOTSP;
                                break;

                    case 2:     *op = OP_NOTSP;
                                break;
                    
                    default:    break;
                }
                break;
            
            case '>':
                input[i] = 0; new_token = true; args2_turn = true;  
                switch (op_tofrom_potential)
                {
                    case 0:     op_tofrom_potential = 1;
                                *op = (*op == 0) ? OP_TOFILE : OP_NOTSP;
                                break;

                    case 1:     if (*op == OP_TOFILE)
                                    { op_tofrom_potential = 2; *op = OP_TOFILE_APP; }
                                else
                                    *op = OP_NOTSP;
                                break;

                    case 2:     *op = OP_NOTSP;
                                break;

                    default:    break;
                }
                break;
            
            case '&':
                input[i] = 0; new_token = true; args2_turn = true;  
                *op = (*op == 0) ? OP_BG : OP_NOTSP;
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
        // |, <, >, <<, >>: args1 AND args2 must exist
        case OP_FROMFILE:
        case OP_TOFILE:
        case OP_FROMFILE_APP:
        case OP_TOFILE_APP:
        case OP_PIPE:
            if (args1_i == 0 || args2_i == 0) { *op = OP_NOTSP; }
            break;
        
        // &: args1 must exist AND args2 must not exist
        case OP_BG:
            if (args1_i == 0 || args2_i != 0) { *op = OP_NOTSP; }
            break;
    }

    // DIRTY DEBUGGING
    // for (unsigned i = 0; i < args1_i; i++)  printf("args1[%d]: %s\n", i, args1[i]);
    // for (unsigned i = 0; i < args2_i; i++)  printf("args2[%d]: %s\n", i, args2[i]);
    // printf("op: ");
    // switch (*op)
    // {
    //     case OP_FROMFILE:       printf("From file");                    break;
    //     case OP_TOFILE:         printf("To file");                      break;
    //     case OP_FROMFILE_APP:   printf("From file (append)");           break;
    //     case OP_TOFILE_APP:     printf("To file (append)");             break;
    //     case OP_PIPE:           printf("Pipe");                         break;
    //     case OP_BG:             printf("Background");                   break;
    //     case OP_NOTSP:          printf("Not supported");                break;
    //     case 0:                 printf("No-op");                        break;
    //     default:                printf("Something weird happened.");    break;
    // }
    // printf("\n");
}
