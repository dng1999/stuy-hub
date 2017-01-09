#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

char **parseCommand (char *cmd){
  char **single = (char **)calloc(10,10);

  int i = 0;
  char *p;
  while(cmd){
    p = strsep(&cmd," ");
    if (strcmp(p,"") == 0){
      p = strsep(&cmd," ");
    }

    single[i] = p;
    i++;
  }
  single[i] = NULL;
  return single;
}

char ***parseInput (char *input){
  char ***multi = (char ***)calloc(100,100);
  
  int i = 0;
  while (input){
    multi[i] = parseCommand(strsep(&input, ";"));
    i++;
  }
  multi[i] = NULL;
  return multi;
}
