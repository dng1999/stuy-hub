# stuy-hub
Members: Dorothy Ng, Nancy Cao

## About ##
stuy-hub is a custom-made shell for Github usage. It has built-in special commands to make Github easier to use.
Only one user may access stuy-hub at a time to prevent merge conflicts from editing files.

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
- EXIT -- exits client, and optionally, closes server
- HELP -- prints info about client features

## List of Files: ##
#### C Files ####
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
