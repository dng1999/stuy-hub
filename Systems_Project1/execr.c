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

void resetFileTable(in, out) {
  if (in != 0) {
    int in_fd = open("STDIN", O_RDONLY);
    dup2(in_fd, 0);
    close(in_fd);
  }
  if (out != 1){
    int out_fd = open("STDOUT", O_RDONLY);
    dup2(out_fd, 1);
    close(out_fd);
  }
}

void execRedirO(char **cmd){ // cmd > file
  char *in = cmd[0];
  char *out = cmd[2];

  int in_fd = open(in , O_RDONLY);
  dup2(in_fd , STDIN_FILENO);
  close(in_fd);

  int out_fd = open(out, O_WRONLY|O_CREAT, 0666);
  dup2(out_fd , STDOUT_FILENO);
  close(out_fd);

  execvp(cmd[0], cmd);
}

// REDIRECTION HERE
void execRedirI(char **cmd){ // input:"command < file"
  char **command;
  char *file;

  int ci = 0;
  int i = 0;
  int signPassed = 0;
  while(cmd[i]){
    if (!signPassed){
      command[ci] = cmd[i];
      ci++;
    }
    else if (strcmp(cmd[i],"<") == 0){
      signPassed = 1;
    }
    else {
      file = cmd[i];
    }
    i++;
  }
  int filed = open(file, O_RDONLY, 0644);
  dup2(filed,STDIN_FILENO);
  close(filed);
  execvp(command[0],command);
}

void execPipe(char **cmd){
  char **command1;
  char **command2;
  char tempCmd[30];

  int ci = 0;
  int fi = 0;
  int i = 0;
  int signPassed = 0;
  while(cmd[i]){
    if (!signPassed){
      command1[ci] = cmd[i];
      ci++;
    }
    else if (strcmp(cmd[i],"|") == 0){
      signPassed = 1;
    }
    else {
      command2[fi] = cmd[i];
      fi++;
    }
    i++;
  }
  i = 2;
  while(i){
    int f = fork();
    if (f){
      wait(0);
    }
    else {
      if (i){
	strcpy(tempCmd,command1);
	strcat(tempCmd," > .tmp");
	printf("%s",tempCmd);
      }
      else {
	strcpy(tempCmd,command1);
        strcat(tempCmd," < .tmp");
	printf("%s",tempCmd);
      }
      exit(0);
    }
    execlp("rm","rm",".tmp",NULL);
  }
}

//code below works

void execCommand(char **cmd){
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
      int i = 0;
      while (cmd[i]){
	      if (strcmp(cmd[i],">") == 0){
	          special = ">";
        }
	      else if (strcmp(cmd[i],"<") == 0){
	         special = "<";
	      }
	      else if (strcmp(cmd[i],"|") == 0){
	         special = "|";
	      }
	      i++;
     }

     if (strcmp(special,">") == 0){
	      execRedirO(cmd);
     }
     else if (strcmp(special,"<") == 0){
	      execRedirI(cmd);
     }
     else if (strcmp(special,"|") == 0){
	      execPipe(cmd);
     }
     else{
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
