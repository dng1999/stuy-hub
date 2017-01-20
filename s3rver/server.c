#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>

#include "networking.h"

void process( char * s );
void sub_server( int sd );
struct shmid_ds shminfo;

int main() {
  system("clear");
  
  char buffer[MESSAGE_BUFFER_SIZE];

  //create shared memory
  int key = ftok("makefile", 22);
  
  int shmid = shmget(key, 4, IPC_CREAT|0644);
  int *len = shmat(shmid, NULL, 0);
  shmctl(shmid, IPC_STAT, &shminfo);
  //shmctl(shmid, IPC_RMID,0);
  
  //create server
  int sd, connection;
  sd = server_setup();
  
  while (1) {
    connection = server_connect( sd );
      
    int f = fork();
    if (f == 0){
      close(sd);
      sub_server( connection );
      exit(0);
    }      
    else{
      close ( connection );
    }
  }
  return 0;
}


void sub_server( int sd ) {
  char buffer[MESSAGE_BUFFER_SIZE];
  
  int key = ftok("makefile", 22);
  
  int shmid = shmget(key, 4, IPC_CREAT|0644);
  int *len = shmat(shmid, NULL, 0);
  shmctl(shmid, IPC_STAT, &shminfo);
  //printf("attaches: %d\n",shminfo.shm_nattch);

  if (shminfo.shm_nattch > 3){
    strcpy(buffer,"rejected");
    printf("Connection rejected. Too many clients.\n");
    write( sd, buffer, sizeof(buffer));
  }
  else{
    strcpy(buffer,"accepted");
    write (sd, buffer, sizeof(buffer));
    while (read( sd, buffer, sizeof(buffer) )) {      
      printf("[SERVER %d] received: %s\n", getpid(), buffer );
      process( buffer );
      write( sd, buffer, sizeof(buffer));    
    }
  }
  
}
void process( char * s ) {

  while ( *s ) {
    /*if (s == "exit"){
      shmdt(shmid, NULL, 0);
      }*/
    *s = (*s - 'a' + 13) % 26 + 'a';
    s++;
  }
}
