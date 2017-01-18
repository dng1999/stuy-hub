#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "parser.h"
#include "execr.h"

static void sighandler (int signo) {
    if (signo == SIGINT)
        exit(0);
}

int main(){
  printf("%s\n", "test");
  signal(SIGINT, sighandler);

  char input[512];
  char cwd[512];

  while (1){
    getcwd(cwd,sizeof(cwd));
    printf("%s$ ",cwd);
    fgets(input, sizeof(input), stdin);

    int i = 0;
    for (;i<strlen(input);i++){
      if (strcmp(&input[i],"\n") == 0){
	input[i] = 0;
      }
    }

    char ***cmd = parseInput(input);
    execInput(cmd);
  }
  return 0;
}
