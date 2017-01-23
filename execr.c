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

// >
int execRedirO(char **cmd){ // cmd > file
  char *out = cmd[2];
  //printf("in: %s\n", in);
  //printf("out: %s\n", out);
  int newOut = dup(STDOUT_FILENO);
  //printf("%d\n", newOut);
  int out_fd = open(out, O_CREAT | O_TRUNC | O_WRONLY , 0666);
  //printf("out_fd: %d\n", out_fd);
  dup2(out_fd, STDOUT_FILENO);
  char *command[50];
  command[0] = cmd[0];
  command[1] = NULL;
  execvp(cmd[0], command);
  close(out_fd);
  return newOut;
}

// <
// REDIRECTION HERE
int execRedir1(char **cmd){ // input:"command < file"
  char *in = cmd[2];
  //printf("in: %s\n", in);
  //printf("out: %s\n", out);
  int newIn = dup(STDIN_FILENO);
  //printf("dupIn: %d\n", newIn);
  int in_fd = open(in, O_RDONLY , 0644);
  //printf("in_fd: %d\n", in_fd);
  dup2(in_fd, STDIN_FILENO);
  //printf("newIN: %d\n", res);
  char *command[50];
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
  int f = fork();
  if (f==0){
    //printf("running command1\n");
    command1[ci] = ">";
    command1[ci+1] = "tmpy";
    command1[ci+2] = NULL;
    execRedirO(command1);
    //printf("execRedirO(command1);\n");
  }
  else{
    int status;
    wait(&status);
    //printf("running command2\n");
    command2[fi] = "<";
    command2[fi+1] = "tmpy";
    command2[fi+2] = NULL;
    int f2 = fork();
    if (f2 == 0){
      execRedir1(command2);
      //printf("execRedirI(command2);\n");
    }
    else {
      int status;
      wait(&status);
      execlp("rm","rm","tmpy",NULL);
      //printf("execlp(rm,rm,tmpy,NULL);\n");
    }
  }
}

//code below works

void execCommand(char **cmd){
  if (strcmp(cmd[0],"exit") == 0){
    //printf("%s\n", "exit #1");
    exit(0);
  }
  else if (strcmp(cmd[0],"cd") == 0){
    chdir(cmd[1]);
  }
  else {
    int f = fork();
    //printf("forked\n");
    char *special = "null";
    if (f==0){
      //printf("child\n");
      int i = 0;
      // detect special characters
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
      // git add -> git commit -> git push in one go!
      if ((strcmp(cmd[0],"git") == 0) && (strcmp(cmd[1],"lazy") == 0)) {
        system("git add .");
        if (cmd[2] == NULL) { // commit message exists
          system("git commit -m \"lazy commit\"");
        }
        else { // commit message doesn't exist
          char commit[100] = "git commit -m \"";
          strcat(commit, cmd[2]);
          strcat(commit, "\"");
          system(commit);
        }
        system("git push");
      }

      // pretty git log
      else if ((strcmp(cmd[0],"git") == 0) && (strcmp(cmd[1],"log") == 0)) {
        system("git log --pretty=format:\"%h%x09%an%x09%ad%x09%s\"");
        exit(0);
      }

      else if (strcmp(cmd[0],"HELP") == 0) {

        printf("\nABOUT STUY-HUB:\n");
        printf("-----------------\n");
        printf("stuy-hub is a custom-made shell for Github usage.\n");
        printf("Only one user may run stuy-hub at a time to prevent merge conflicts.\n");

        printf("\nSUPPORTED REDIRECTION:\n");
        printf("-----------------\n");
        printf(">\n");
        printf("<\n");
        printf("||\n\n");

        printf("SPECIAL COMMANDS:\n");
        printf("-----------------\n");
        printf("git lazy <optional commit msg>  --  git add, git commit <msg>, and git push in one command\n");
        printf("git log  --  formatted and informative version of log\n\n");
      }

      // redirection
      else if (strcmp(special,">") == 0){
        //printf("> detected\n");
        int newOut = execRedirO(cmd);
        dup2(newOut, STDOUT_FILENO);
      }
      else if (strcmp(special,"<") == 0){
        int newIn = execRedir1(cmd);
        dup2(newIn, STDIN_FILENO);
      }
      else if (strcmp(special,"|") == 0){
        //printf("run execPipe\n");
        execPipe(cmd);
      }
      else{
        //printf("no special symbols\n");
        execvp(cmd[0],cmd);
      }
      //resetFileTable(currentSTD_IN, currentSTD_OUT);
    }
    else {
      //rintf("parent\n");
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
