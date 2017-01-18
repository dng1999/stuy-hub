#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "networking.h"

void process( char * s );
void run_server( int sd );

int main() {

  int sd, connection;

  sd = server_setup();
    
  while (1) {
    connection = server_connect( sd );
    
    int f = fork();
    close(sd);
    run_server( connection );
    
    exit(0);
    //close( connection );
  }    
  return 0;
}


void run_server( int sd ) {

  char buffer[MESSAGE_BUFFER_SIZE];
  while (read( sd, buffer, sizeof(buffer) )) {

    printf("[SERVER %d] received: %s\n", getpid(), buffer );
    process( buffer );
    write( sd, buffer, sizeof(buffer));    
  }
  
}
void process( char * s ) {

  while ( *s ) {
    *s = (*s - 'a' + 13) % 26 + 'a';
    s++;
  }
}
