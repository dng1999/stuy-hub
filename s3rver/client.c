#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "networking.h"

int main( int argc, char *argv[] ) {

  char *host;
  if (argc != 2 ) {
    printf("host not specified, conneting to 127.0.0.1\n");
    host = "127.0.0.1";
  }
  else
    host = argv[1];
  
  char buffer[MESSAGE_BUFFER_SIZE];
  
  
  int sd;
  
  sd = client_connect( host , getpid());

  read( sd, buffer, sizeof(buffer) );
  if (strcmp(buffer, "rejected") == 0){
    strcpy(buffer,"Connection rejected. Another user is accessing the server.");
    printf("%s\n",buffer);
    write (sd, buffer, sizeof(buffer));
    exit(0);
  } 
  while (1) {
    printf("enter message: ");
    fgets( buffer, sizeof(buffer), stdin );
    char *p = strchr(buffer, '\n');
    *p = 0;
    
    write( sd, buffer, sizeof(buffer) );
    read( sd, buffer, sizeof(buffer) );
    printf( "received: %s\n", buffer );
  }
  return 0;
}
