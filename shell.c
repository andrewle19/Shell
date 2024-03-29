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

// Count Line function
// Parameters: file pointer
// Function will count the number of lines in the Program
// Returns the count of them
int countLines(FILE *fp)
{
  fp = fopen("history.txt","r");
  int count = 0;
  char ch;

  // if the file doesnt exist return count as 0
  if (fp == NULL)
  {
    fclose(fp);
    return count;
  }
  else
  {
    // loop till end of file
    while(!feof(fp))
    {
      ch = fgetc(fp);
      // checks if their is a newline if true increment count
      if(ch == '\n')
      {
        count++;
      }
    }
    fclose(fp);
    return count;
  }
}

// Prints the last 10 Commands
// PARAMETERS: FIle pointer, the number of lines counted

void showHistory(FILE *fp, int lineCount)
{
  fp = fopen("history.txt","r");
  char *line = malloc(MAX_LINE);
  char *lines;

  if(fp == NULL)
  {
    printf("No Commands in history\n");
  }
  else
  {
    if(lineCount > 10)
    {
        for(int i = 0; i < lineCount; i++)
        {
          fgets(line,MAX_LINE,fp);
          strtok(line,"\n");
          if(i > lineCount-11)
          {
            printf("%i. %s\n",i+1,line);
          }
        }
    }
    else
    {
      for(int i = 0; i < lineCount; i++)
      {
        fgets(line,MAX_LINE,fp);
        strtok(line,"\n");
        printf("%i. %s\n",i+1,line);
      }
    }
  }
  free(line);
  fclose(fp);
}

// Writes an argument to the history file
// PARAMETERS: File pointer, char pointer array, argument count
void writeFile(FILE *fp, char *args[], int argc)
{
  fp = fopen("history.txt","a");
  for(int i = 0; i < argc; i++)
  {
    fputs(args[i],fp);
    fputs(" ",fp);
  }
  fputs("\n",fp);
  fclose(fp);
}
int main(void)
{
  FILE *fp; // file pointer
  char *args[MAX_LINE/2 + 1]; /* command line arguments */
  char *arg = malloc(MAX_LINE);
  int run = 1; /* flag to determine when to exit program */
  int argc = 0; // count of arguments

  int count = countLines(fp); // checks how many commands are in history



  while (run)
  {

    printf("bs>");
    fgets(arg,MAX_LINE,stdin); // get the input of user
    fflush(stdout); // clears input buffer
    strtok(arg, "\n"); // tokenize the string taking out newline
    run = quit(arg); // check if user wants to quit or not

    args[0] = strtok(arg," "); // grab the first token
    argc = 1; //set the argument count

    // parse/tokenize the commands into args array
    while (args[argc-1] != NULL)
    {
      args[argc] = strtok(NULL," "); // tokenzie by blank spaces
      argc++; // increment. the argument count
    }

  char firstintial = *args[0]; // get the first character of string
  pid_t pid; // the process id


  // if the first character is ! means the user wants to acess history
  if (firstintial == '!')
    {
      // if the user wants the most recent command
      if(strcmp(args[0],"!!") == 0)
      {
        // open file
        fp = fopen("history.txt","r");
        for(int i = 0; i < count; i++)
        {
          fgets(arg,MAX_LINE,fp);
        }
        strtok(arg, "\n"); // tokenize the string taking out newline
        printf("%s\n",arg); // prints the most recent command
        args[0] = strtok(arg," "); // grab the first token
        argc = 1; //set the argument count

        // parse/tokenize the commands into args array
        while (args[argc-1] != NULL)
        {
          args[argc] = strtok(NULL," "); // tokenzie by blank spaces
          argc++; // increment. the argument count
        }
        fclose(fp);
      }
      // else the user wants a specefic command
      else
      {

        args[0] = strtok(args[0],"!"); // get string after !
        int commandNum = atoi(args[0]); // convert string to an int

        // check if the command is out of bounds of history
        if(commandNum > count || commandNum < 0)
        {
          printf("%ith command is not in history\n",commandNum);
        }
        // go to specefic command
        else
        {
          // used to buffer file
          char *traverse = malloc(MAX_LINE);
          fp = fopen("history.txt","r");

          // loop through commands
          for(int i = 0; i < count; i++)
          {
            // if i+1 on command line take in the command
            if(commandNum == i+1)
            {
              fgets(arg,MAX_LINE,fp);
            }
            // else put in traverse
            else
            {
              fgets(traverse,MAX_LINE,fp);
            }
          }
          strtok(arg, "\n"); // tokenize the string taking out newline


          printf("%s\n",arg);

          args[0] = strtok(arg," "); // grab the first token
          argc = 1; //set the argument count

          // parse/tokenize the commands into args array
          while (args[argc-1] != NULL)
          {
            args[argc] = strtok(NULL," "); // tokenzie by blank spaces
            argc++; // increment. the argument count
          }
          fclose(fp);
          free(traverse);
        }
      }
    }



  // check whether the process should wait for the child procss will run in background
  if (strcmp(args[argc-2],"&") == 0)
  {
      // fork the process
    pid = fork();
      // parent process
    if(pid > 0)
    {
      count += 1; // increments count
      writeFile(fp,args,argc-1); //writes the argument to file
      wait(NULL);
    }
    else
    {
      // NULL &
      args[argc-2] = NULL;
      
      execvp(args[0],args);

    }
  }
  // if the user wants the history of commands
  else if(strcmp(args[0],"history") == 0)
  {
    showHistory(fp,count);
  }
  // executes the command in child but parent wont wait so it will exit
  else
  {
    // fork the process
    pid = fork();

    if(pid > 0)
    {
      if(strcmp(args[0],"\n") != 0)
      {
        count++; // increment by  count
        writeFile(fp,args,argc-1); //writes the argument to file
      }

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
