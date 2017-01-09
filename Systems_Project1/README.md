# C Shell
by Dorothy Ng

###Features:
	Can execute commands
	Parses multiple commands in a line where the commands are separated by ;

###Attempted:
	Tried to get redirection to work
	Tried to get pipes to work
	
###Bugs:
	Putting multiple ;'s consecutively will cause a segmentation fault
	Same for >,< and |
	> redirection might work on certain computers (I was not able to test < or |)
	Sometimes > works but writes a different file than specified
	Single spaces only

###Files & Function Headers:
parser.h
	/*========char **parseCommand (char *cmd);
	Takes in a command line and parses it to separate the command from its 
	arguments.

	Returns an array containing the command as the first element and the 
	arguments in the following elements.
	========*/

	/*========char ***parseInput (char *input);
	Takes in a string of multiple commands that are separated by ; and parses it
	to separate each command from the other.

	Returns an array of command lines which are an array of a command and its
	arguments.
	========*/

execr.h
	/*========void execRedirI(char **cmd);
	Takes in an array of strings and separates it at the element containing <.
	Executes the command specified before < using the arguments given by the file
	specified after <.

	Returns nothing.
	========*/

	/*========void execRedirO(char **cmd);
	Takes in an array of strings and separates it at the element containing >.
	Executes the command specified before > and writes its output to the file
	specified after >.

	Returns nothing.
	========*/

	/*========void execPipe(char **cmd);
	Takes in an array of strings and separates it at the element containing |.
	Executes the command specified before | and uses its output as the input for
	the command specified after |.

	Returns nothing.
	========*/

	/*========void execCommand(char **cmd);
	Takes in an array of strings containing the command and its arguments.
	Executes the given command.

	Returns nothing.
	========*/

	/*========void execInput(char ***input);
	Takes in an array of commands which are in the form of an array of strings 
	containing the command and its arguments.
	Increments through the array to execute every command.


	Returns nothing.
	========*/