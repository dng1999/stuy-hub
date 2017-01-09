#include <unistd.h>
#include <stdio.h>

int main(){
  printf("Exiting...\n");
  execlp("exit","exit",NULL);
  return 0;
}
