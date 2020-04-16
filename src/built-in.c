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
      "banhxeOSH\n"
      "Type \"help [command]\" to get command-specific help.\n"
      "All of the built-in commands will only take the first argument, if available.\n"
      "\n"
      "[Built-in commands]\n"
      "cd      :  Change current working directory\n"
      "PS1     :  Change shell prompt\n"
      "history :  Reveal history\n"
      "help    :  What you're reading now.\n"
      "\n"
      "[Dotfiles] (Work in progress):\n"
      "~/.banhxeorc : Startup script / Configuration file\n"
      "ARM ASM ASMR.\n"
      "\n";
   
   static char _HELP_STRING_CD[] =
      "banhxeOSH | cd\n"
      "Syntax: cd <directory name>\n"
      "\n";
   
   static char _HELP_STRING_PS1[] =
      "banhxeOSH | PS1\n"
      "Syntax: PS1 [prompt]\n"
      "Change the shell prompt.\n"
      "If no prompt is given, reset to default.\n"
      "\n";

   static char _HELP_STRING_HISTORY[] =
      "banhxeOSH | history\n"
      "Syntax: history\n"
      "Show a list of previously run commands.\n"
      "\n";
   
   static char _HELP_STRING_SELF[] =
      "banhxeOSH | What are you doing?\n"
      "You are reading help.\n"
      "\n";
   
   long unsigned arg_count = 0;
   while (args[arg_count + 1] != NULL) arg_count++;

   if (arg_count == 0)
   {
      printf("%s", _HELP_STRING_OVERVIEW);
   }
   else if (arg_count == 1)
   {
           if (strcmp(args[1], "cd") == 0)      printf("%s", _HELP_STRING_CD);
      else if (strcmp(args[1], "ps1") == 0)     printf("%s", _HELP_STRING_PS1);
      else if (strcmp(args[1], "PS1") == 0)     printf("%s", _HELP_STRING_PS1);
      else if (strcmp(args[1], "history") == 0) printf("%s", _HELP_STRING_HISTORY);
      else if (strcmp(args[1], "help") == 0)    printf("%s", _HELP_STRING_SELF);
   }
   else
   {
      printf("help : Too many arguments.\n");
   }

   return 1;
}
