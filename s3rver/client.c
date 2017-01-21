#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parser.h"
#include "execr.h"
#include "networking.h"
void process( char * s );

int main( int argc, char *argv[] ) {
  system("clear");
  printf("\n");
  printf("               o                              8                 8\n");
  printf("               8                              8                 8\n");
  printf("     oPYo    oo8oo  o    o   o    o           8oPYo    o    o   8oPYo\n");
  printf("    Yb         8    8    8   8    8   ooooo   8    8   8    8   8    8\n");
  printf("      'Yb      8    8    8   8    8           8    8   8    8   8    8\n");
  printf("     YooP'     8    `YooP'   `YooP8           8    8   `YooP'   `YooP'\n");
  printf("                                  8\n");
  printf("                                  8\n");
  printf("                              8888'\n");
  printf("\n");
  printf("                  - Welcome to the improved Github shell -\n");
  printf("\n");

  char *host;

  if (argc != 2 ) {
    printf("host not specified, connecting to 127.0.0.1\n");
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

  printf("Entered shell.\n");

  char cwd[512];

  while (1) {
    getcwd(cwd,sizeof(cwd));
    printf("%s$ ",cwd);
    fgets( buffer, sizeof(buffer), stdin );
    char *p = strchr(buffer, '\n');
    *p = 0;
    //printf("%s\n", buffer);
    int x = 0;
    process( buffer );
    /*
    write( sd, buffer, sizeof(buffer) );
    read( sd, buffer, sizeof(buffer) );
    printf( "received: %s\n", buffer );
    */
  }
  // printf("test\n");
  return 0;
}

void process( char * s ) {
    char ***cmd = parseInput(s);
    execInput(cmd);
}
