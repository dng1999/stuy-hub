#ifndef EXEC_H
#define EXEC_H

int execRedirI(char **cmd);
int execRedirO(char **cmd);
void execPipe(char **cmd);
void execCommand(char **cmd);
void execInput(char ***input);

#endif
