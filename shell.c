// Andrew Le
// andrewle19@csu.fullerton.edu

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 80 /* The maximum length command */


// quit function
// PARAMETERS: char poitner
// function compares arg to quit() if it is > 10 then exit Program else continue
// Return: 1 or 0 depending on run
int quit(char *str)
{
  if(strcmp(str,"exit") == 0)
  {
    printf("Quiting Program...\n");
    return 0;
  }
  return 1;
}




int main(void)
{
  FILE *fp; // file pointer
  char *args[MAX_LINE/2 + 1]; /* command line arguments */
  char *arg = malloc(MAX_LINE);
  int run = 1; /* flag to determine when to exit program */
  int argc = 0; // count of arguments

  while (run)
  {

    printf("bs>");
    fgets(arg,MAX_LINE,stdin); // get the input of user
    strtok(arg, "\n"); // tokenize the string taking out newline
    fflush(stdout); // clears input buffer

    run = quit(arg); // check if user wants to quit or not

    args[0] = strtok(arg," "); // grab the first token
    argc = 1; //increment the argument count
    // parse/tokenize the commands into args array
    while (args[argc-1] != NULL)
    {
      args[argc] = strtok(NULL," "); // tokenzie by blank spaces
      argc++; // increment. the argument count
    }


    pid_t pid;


    // check whether the process should wait for the child procss will run in background
    if (strcmp(args[argc-2],"&") == 0)
    {

      // fork the process
      pid = fork();
      // parent process
      if(pid > 0)
      {
        wait(NULL);
      }
      else
      {
        fp = fopen("history.txt","a");

        for(int i = 0; i < argc-1; i++)
        {
          fputs(args[i],fp);
          fputs(" ",fp);
        }
        fputs("\n",fp);
        fclose(fp);
        args[argc-2] = NULL;
        execvp(args[0],args);

      }
    }
    // executes the command in child but parent wont wait so it will exit
    else
    {
      // fork the process
      pid = fork();

      if(pid > 0)
      {
        exit(0);
      }
      else
      {
        execvp(args[0],args);

        exit(0);
      }
    }

    }
free(arg);
return 0;
}
