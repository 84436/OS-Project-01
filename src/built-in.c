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
