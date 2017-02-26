 /*
 * slush.C
 *   The SLU shell: a simple command-line interpreter
 *   usage: slush
 *
 *   Jessie Cisneros and Joey Gao 09/17/15
 */

#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>

/* function: handle_signal
 *    Blank function used to ignore CTRL-C
 *    inputs: none
 *    output: none
 */

void handle_signal()
{
}

/* function: check_syntax
 *    Checks for syntax errors in slush input
 *    inputs: string to check for errors
 *    output: 0 for OK, 1 for ERROR
 */

int check_syntax(char checkString[260])
{
  int syntaxError = 0;
   
  if (checkString[0] == '(') syntaxError = 1;
  if (checkString[strlen(checkString) - 2] == '(') syntaxError = 1;
  if (strstr(checkString, "((") != NULL) syntaxError = 1;
  if (strstr(checkString, "( (") != NULL) syntaxError = 1;
  
  return syntaxError;
}

/* function: fork_execute
 *    Forks, and executes commands.
 *    inputs: array of arguments, size of array of arguments
 *    output: command execution
 */

void fork_execute(char *commands[15][256], int size)
{
  int loopCounter;
  int childCounter = 0;   
  int singleCommand = (size == 1);
  
  int fdStore;
  int fdesc[2];
  
  // Looping through commands array, piping when necessary, forking, and executing the commands
  for (loopCounter = size - 1; loopCounter >= 0; loopCounter--)
  {
    int leftMostCommand = (loopCounter == 0); 
    int rightMostCommand = (loopCounter == size -1 );
    
    if (leftMostCommand != 1 && singleCommand != 1)
    {
      pipe(fdesc);
    }
    
    switch(fork())
    {
      // Child duplicates fdesc, closes its fdesc, and executes command
      case 0:
	if (singleCommand == 1)
	{
	  execvp(commands[loopCounter][0], commands[loopCounter]);
	  perror(commands[loopCounter][0]);
	  exit(0);
	  break;
	}
	if (rightMostCommand != 1)
	{
	  dup2(fdStore, 0);
	}
	if (leftMostCommand != 1)
	{
	  dup2(fdesc[1], 1);
	}
	close(fdesc[0]);
	close(fdesc[1]);
	execvp(commands[loopCounter][0], commands[loopCounter]);
	perror(commands[loopCounter][0]);
	exit(0);
      
      // Parent stores previous pipe's read end and closes its fdesc
      default:
	if (singleCommand == 1)
	{
	  childCounter++;
	  break;
	}
	if (rightMostCommand != 1)
	{
	  close(fdStore);
	}
	if (leftMostCommand != 1)
	{
	  fdStore = fdesc[0];
	  close(fdesc[1]);
	}
	if (leftMostCommand == 1)
	{
	  close(fdesc[1]);
	}
	childCounter++;
	break;
      
      case -1:
	perror("fork");
	exit(errno);
    }
  }
  
  // Waiting for children
  while (childCounter > 0)
  {
    wait(NULL);
    childCounter--;
  }
}
  
 /* function: main
 *    Initializes and runs the SLU shell, performs 'cd' logic,  
 *    inputs: none
 *    outputs: slush shell
 */

int main() 
{ 
  signal(SIGINT,handle_signal);
  
  printf("slush%% ");

  // Looping the command prompt
  char commandLine[260];
  while(fgets(commandLine, 260, stdin))
  {
    if (strlen(commandLine) - 1 > 256)
    {
      fprintf(stderr, "slush: error: input line exceeds maximum character length (256)\n");
      goto REPROMPT;
    }
    
    int syntaxError = check_syntax(commandLine);
    if (syntaxError == 1)
    {
      fprintf(stderr, "Syntax error: Invalid null command\n");
      goto REPROMPT;
    }
    
    else if (commandLine[0] != '\n')
    {
      // Filling argv with commands and arguments
      int i = 0;
      char *argv[256]; 
      argv[0] = strtok(commandLine, " \n\t");

      while(argv[i] != NULL)
      {
	i++;
	argv[i] = strtok(NULL, " \n\t");
      }
      
      // Built-in cd functionality
      if (strcmp(argv[0], "cd") == 0)
      {
	errno = 0;
	chdir(argv[1]);
	if (errno != 0)
	{
          perror(argv[0]);
	}
      }
      
      // Program execution
      else
      {
	argv[i] = "(";
	argv[i + 1] = '\0';
	
	// Creating commandsArray, a 2D char array out of argv, split by "("
	char *commandsArray[15][256];
	int argvCounter = 0;
	int commandsArray1D = 0;
	int commandsArray2D = 0;
	int parenthesisCounter = 0;
	
	while (argv[argvCounter] != '\0')
	{
	  if (parenthesisCounter == 15)
	  {
	    fprintf(stderr, "slush: error: pipeline exceeds maximum arguments (15)\n");
	    goto REPROMPT;
	  }
	  
	  else if (strcmp(argv[argvCounter], "(") == 0)
	  {
	    parenthesisCounter++;
	    commandsArray[commandsArray1D][commandsArray2D] = '\0';
	    commandsArray1D++;
	    commandsArray2D = 0;
	  }
	  else
	  {
	    commandsArray[commandsArray1D][commandsArray2D] = argv[argvCounter];
	    commandsArray2D++;
	  }
	  argvCounter++;
	}
	
	fork_execute(commandsArray, commandsArray1D);
	memset(commandsArray, 0, sizeof(commandsArray));
      }
    }
    REPROMPT: printf("slush%% ");
  }
  
  return 0;
}

