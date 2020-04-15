#include "_includes_.h"

int built_in_cd(char **args)
{
   if (args[1] == NULL) {
       fprintf(stderr,"cd to unknown directory");
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
   
   strcat(new_shellname, args[1]); //create new shell name with ">"
   strcat(new_shellname, "> ");

   if (setenv("SHELLNAME", new_shellname, 1) != 0){ //set env var to new shell name
      perror("PS1 failed");
   }

   free(new_shellname);    //free space
   return 1;
}