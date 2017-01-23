#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>

void main() {

  //create shared memory                                                        
  int key = ftok("makefile", 22);

  int shmid = shmget(key, 4, IPC_CREAT|0644);
  int *len = shmat(shmid, NULL, 0);
  shmctl(shmid, IPC_RMID,NULL);
}
