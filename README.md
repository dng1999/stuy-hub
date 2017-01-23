# stuy-hub
Members: Dorothy Ng, Nancy Cao

## How To Compile: ##

1. make

## How To Use: ##
1. Run ./server in one terminal.
2. Run ./client in another terminal. ./client [IP ADDRESS] if using networking.
3. Use as normal shell. (Enter command HELP for information about what stuy-hub can do.)
4. Exit client using EXIT command.
5. make clean

## Features ##
#### SUPPORTED REDIRECTION ####
- >
- <
- |

#### SPECIAL COMMANDS: ####
- git lazy [optional commit msg]  --  git add, git commit <msg>, and git push in one command
- git log  --  formatted and informative version of log

## List of Files: ##
#### C FILES ####
- networking.c
- client.c
- server.c
- parser.c
- execr.c
- shm.c

#### Header Files ####
- networking.h
- execr.h
- parser.h
- shm.h

#### Documents ####
- DESIGN
- DEVLOG
- README.md

#### Etc. ####
- makefile
