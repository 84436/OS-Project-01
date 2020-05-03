#include "_includes_.h"

int built_in_cd(char **args)
{
   if (args[1] == NULL || args[2] != NULL) {
       fprintf(stderr,"cd to unknown directory\n");
       //perror: return Success :<
   } else {
     if (chdir(args[1]) != 0) {
       perror("cd failed");
     }
  }
   return 1;
}

int built_in_PS1(char** args){
   char* new_shellname = malloc(LINE_LENGTH);

   // reset on args[1] empty
   if(args[1] == NULL)
   {
      if (setenv("SHELLNAME", PS1_DEFAULT, 1) != 0){ //set env var to new shell name
         perror("PS1 failed. You are extra screwed.");
      }
      return 1;
   }
   
   strcat(new_shellname, args[1]); //create new shell name with ">"
   strcat(new_shellname, "> ");

   if (setenv("SHELLNAME", new_shellname, 1) != 0){ //set env var to new shell name
      perror("PS1 failed");
   }

   free(new_shellname);    //free space
   return 1;
}

int built_in_help(char** args)
{
   static char _HELP_STRING_OVERVIEW[] =
      "Type \"help [command]\" to get command-specific help.\n"
      "All of the built-in commands will only take the first argument, if available.\n"
      "[Built-in commands]\n"
      "cd       :  Change current working directory\n"
      "PS1      :  Change shell prompt\n"
      "history  :  Reveal history (* alternative syntax available)\n"
      "histsize : Change history list size\n"
      "help     :  What you're reading now.\n"
      "exit     :  Leave banhxeOSH\n"
      "\n";
   
   static char _HELP_STRING_CD[] =
      "Syntax   : cd <directory name>\n"
      "Function : Change the current working directory.\n"
      "\n";
   
   static char _HELP_STRING_PS1[] =
      "Syntax   : PS1 [prompt]\n"
      "Function : Change the shell prompt. If no prompt is given, reset to default.\n"
      "\n";

   static char _HELP_STRING_HISTORY[] =
      "Syntax   : history\n"
      "Function : Show a list of previously run commands,\n"
      "           incrementally numbered from oldest to newest.\n"
      "Alternative syntax:\n"
      "    !! = reveal and run latest command.\n"
      "    !x = reveal and run command [x] in history list.\n"
      "\n";
   
   static char _HELP_STRING_HISTORY_SIZE[] =
      "Syntax   : histsize <size>\n"
      "Function : Resize the history list (default is 20).\n"
      "           If the given size is smallest than current,\n"
      "           the list will be truncated to <size> most recent items.\n"
      "\n";
   
   static char _HELP_STRING_SELF[] =
      "\"help\" is a collection of help messages for banhxeOSH built-in commands.\n";
   
   static char _HELP_STRING_EXIT[] =
      "Syntax   : exit\n"
      "Function : Exits banhxeOSH.\n"
      "           Currently no settings are persistent\n"
      "           (i.e. shell prompt and history list is not saved.)\n"
      "\n";
   
   static char _HELP_STRING_NOT_FOUND[] =
      "help : Specified command not found.\n"
      "If the command is not a part of banhxeOSH\'s built-ins,\n"
      "try referring to manpages, GNU info, and/or the command's own help system.\n"
      "\n";

   long unsigned arg_count = 0;
   while (args[arg_count + 1] != NULL) arg_count++;

   if (arg_count == 0)
   {
      printf("%s", _HELP_STRING_OVERVIEW);
   }
   else if (arg_count == 1)
   {
      string_lower(args[1]);
           if (strcmp(args[1], "cd") == 0)         printf("%s", _HELP_STRING_CD);
      else if (strcmp(args[1], "ps1") == 0)        printf("%s", _HELP_STRING_PS1);
      else if (strcmp(args[1], "history") == 0)    printf("%s", _HELP_STRING_HISTORY);
      else if (strcmp(args[1], "histsize") == 0)   printf("%s", _HELP_STRING_HISTORY_SIZE);
      else if (strcmp(args[1], "help") == 0)       printf("%s%s", _HELP_STRING_SELF, _HELP_STRING_OVERVIEW);
      else if (strcmp(args[1], "exit") == 0)       printf("%s", _HELP_STRING_EXIT);
      else                                         printf("%s", _HELP_STRING_NOT_FOUND);
   }
   else
   {
      printf("help : Too many arguments.\n");
   }

   return 1;
}

// tolower() a string
void string_lower(char* str)
{
   for (int i = 0; i < strlen(str); i++)
      str[i] = tolower(str[i]);
}

int get_number(char* number_string) {
   char *index_array = (char *)malloc(LINE_LENGTH - 1);
   int i = 0;
   while (number_string[i] != '\0')
   {
      if(!isdigit(number_string[i]))
         return -1;

      index_array[i] = number_string[i];
      i = i + 1;
    }

    return atoi(index_array);
}

int built_in_history(char** args, char** history, int *history_count) {
   long unsigned arg_count = 0;
   while (args[arg_count + 1] != NULL) arg_count++;

   if(arg_count == 0){
      export_history(history, *history_count);
   }
   else if(arg_count == 1){
      if(strcmp(args[1], "-c") == 0 || strcmp(args[1], "--clear") == 0) {
         *history_count = 0;
         printf("History's clear.\n");
      }
   }
   else{
      printf("history: Too many arguments.\n");
   }
   return 1;
}

int built_in_histsize(char** args, char*** history, int* history_count, unsigned int HISTSIZE){
   long unsigned arg_count = 0;
   while (args[arg_count + 1] != NULL) arg_count++;

   if(arg_count == 0){
      return 1; // do nothing
   }
   else if(arg_count == 1) {
      int new_size = get_number(args[1]);
      if(new_size == -1){
         printf("Unknown size.\n");
         return 1;
      }

      if(new_size == HISTSIZE){
         printf("HISTSIZE's unchanged.\n");
         return 1;
      }
      else if(new_size > HISTSIZE || new_size < HISTSIZE){
         int current_count = *history_count;

         char** new_history = (char**) malloc (new_size * sizeof(char*));
         init_history(new_history, new_size);

         if (setenv("HISTSIZE", args[1], 1) != 0){ 
            perror("Changing HISTSIZE failed");
         }

         if(new_size > HISTSIZE || (new_size < HISTSIZE && new_size > current_count)){
            for(int i = 0; i < current_count; i++) {
               const char* _src = (*history)[i];
               strcpy(new_history[i], _src);
            }
         }
         else if (new_size < HISTSIZE && new_size < current_count){
            for(int i = 0; i < new_size; i++) {
               const char* _src = (*history)[new_size + i - 1];
               strcpy(new_history[i], _src);
            }
            *history_count = new_size; 
         }
         else {} // ?

         char** temp = *history;
         *history = new_history;
         free_history(temp, HISTSIZE);
      }
   }
   return 1;
}
