#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include "execr.h"

int currentSTD_IN = 0;
int currentSTD_OUT = 1;


int execRedirO(char **cmd){ // cmd > file
  char *out = cmd[2];
  //printf("in: %s\n", in);
  //printf("out: %s\n", out);
  int newOut = dup(STDOUT_FILENO);
  //printf("%d\n", newOut);
  int out_fd = open(out, O_CREAT | O_TRUNC | O_WRONLY , 0644);
  //printf("out_fd: %d\n", out_fd);
  dup2(out_fd, STDOUT_FILENO);
  char **command;
  command[0] = cmd[0];
  command[1] = NULL;
  execvp(cmd[0], command);
  close(out_fd);
  return newOut;
}

// REDIRECTION HERE
int execRedirI(char **cmd){ // input:"command < file"
  char *in = cmd[2];
  //printf("in: %s\n", in);
  //printf("out: %s\n", out);
  int newIn = dup(STDIN_FILENO);
  //printf("%d\n", newIn);
  int in_fd = open(in, O_RDONLY , 0644);
  //printf("out_fd: %d\n", out_fd);
  int res = dup2(in_fd, STDIN_FILENO);
  //printf("%d\n", res);
  char **command;
  command[0] = cmd[0];
  command[1] = NULL;
  execvp(cmd[0], command);
  close(in_fd);
  return newIn;
}

void execPipe(char **cmd){
  //printf("running execPipe\n");
  char *command1[50];
  char *command2[50];

  int ci = 0;
  int fi = 0;
  int i = 0;
  int signPassed = 0;
  while(cmd[i]){
    //printf("running parser loop\n");
    if (strcmp(cmd[i],"|") == 0){
      //printf("ran by pipe\n");
      signPassed = 1;
    }
    else if (!signPassed){
      //printf("running by command1\n");
      command1[ci] = cmd[i];
      ci++;
    }
    else {
      //printf("running by command2\n");
      command2[fi] = cmd[i];
      fi++;
    }
    i++;
  }
  //printf("running execution\n");
  //printf("running command1\n");
  command1[ci] = ">";
  command1[ci+1] = ".tmpy";
  command1[ci+2] = NULL;
  //execRedirO(command1);
  printf("execRedirO(command1);\n");

  //printf("running command2\n");
  command2[fi] = "<";
  command2[fi+1] = ".tmpy";
  command2[fi+2] = NULL;
  //execRedirI(command2);
  printf("execRedirI(command2);\n");

  //execlp("rm","rm",".tmpy",NULL);
  printf("execlp(rm,rm,.tmpy,NULL);\n");
}

//code below works

void execCommand(char **cmd){
  //printf("testing \n");
  if (strcmp(cmd[0],"exit") == 0){
    exit(0);
  }
  else if (strcmp(cmd[0],"cd") == 0){
    chdir(cmd[1]);
  }
  else {
    int f = fork();
    char *special = "null";
    if (f==0){
      //printf("forked\n");
      int i = 0;
      while (cmd[i]){
        if (strcmp(cmd[i],">") == 0){
          //printf("found >\n");
          special = ">";
        }
        else if (strcmp(cmd[i],"<") == 0){
          //printf("found <\n");
          special = "<";
        }
        else if (strcmp(cmd[i],"|") == 0){
          //printf("found pipe\n");
          special = "|";
        }
        i++;
      }

      if (strcmp(special,">") == 0){
        //printf("> detected\n");
        int newOut = execRedirO(cmd);
        dup2(newOut, STDOUT_FILENO);
      }
      else if (strcmp(special,"<") == 0){
        int newIn = execRedirI(cmd);
        dup2(newIn, STDIN_FILENO);
      }
      else if (strcmp(special,"|") == 0){
        printf("run execPipe\n");
        execPipe(cmd);
      }
      else{
        //printf("no special symbols\n");
        execvp(cmd[0],cmd);
      }
      //resetFileTable(currentSTD_IN, currentSTD_OUT);
    }
    else {
      int status;
      wait(&status);
    }
  }
}

void execInput(char ***input){
  int i = 0;
  while (input[i]){
    execCommand(input[i]);
    i++;
  }
}
