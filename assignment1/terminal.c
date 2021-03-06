#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "terminal.h"
#include <libgen.h>

// size of input buffer
#define buffer 80
int loop = 0;
char input[buffer];
int pid, background = 0; 
int length, i;
char* cmdTok;
char delim = ';';
char ** cmdArray = NULL;
int numCmd;

int main()
{
    while(loop == 0)
    {
        // Gets the current working directory for the application and prints it out
        char currDir[256];
        getcwd(currDir, 255);
        fprintf(stdout, "%s\n", currDir);
        
        fgets(input, buffer, stdin);
        
        // Get a list of all the commands to be run
        cmdTok = strtok(input, &delim);
        numCmd = 1;
        while(cmdTok)
        {            
           cmdArray = realloc (cmdArray, sizeof (char*) * numCmd);
           if(cmdTok[strlen(cmdTok)-1] == '\n')
                cmdTok[strlen(cmdTok)-1] = '\0';
           cmdArray[numCmd-1] = cmdTok;
           numCmd++;            
           cmdTok = strtok(NULL, &delim);
        }
        // Run all the commands (This didn't work for me in the one loop for some reason)
        for(i = 0; i < numCmd-1; i++)
        {
            parseInput(cmdArray[i]);
        }
    }
    
    return 0;
}

// Decides what the application will do with the input it is given
void parseInput(char* input)
{
    char* string = malloc(sizeof(char) * strlen(input));
    strcpy(string, input);
    char pipeDelim = '|';
    char* tok = strtok(string, &pipeDelim);
    char* tok2 = strtok(NULL, &pipeDelim);
    
    // If token 2 isn't null then we have a pipe command
    if(tok2 != NULL)
    {
        if(createPipe(tok, tok2) != 0)
        {
            fprintf(stderr, "Pipe failed between: %s and %s\n", tok, tok2);
        }
    }
    else
    {
        // Check if the exit command was given, limit of 4 due to buffer messing up the compare over 4 letters
        if(strncmp(input, "exit", 4) == 0)
        {
            loop = 1;
        }
        else
        {             
            length = strlen(input);
            // Check if the process should be run in the background and if so remove the ampersand and the new line and set background to 1
            if(input[length-2] == '&')
            {
                background = 1;
                input[length-2] ='\0';
                input[length-1] ='\0';
            }
            else
            {
                background = 0;
            }            
                    
            // Find what the command was
            char* checkString = malloc ((sizeof (char*) * strlen(input)) +1 );;
            
            strcpy(checkString, input);
            char* token = strtok(checkString, " ");
            
            // If we're being told to change directory then we don't need to run in a new process
            if(strncmp(token, "cd", 2) == 0)
            {
                token = strtok (NULL, " ");                
                if(changeDir(token) != 0)
                {
                    fprintf(stderr, "Could not change directory to: %s", token);
                }
            }
            else
            {
                // Create a child process
                pid = fork();
                switch(pid)
                {
                    // Forking returned an error
                    case -1:
                        fprintf(stderr, "Fork failed");
                        break;
                    // Child process
                    case 0:
                        if(call_system(input) == 1)
                            fprintf(stderr, "System Call failed");
                        break;
                    // Parent process checks if the child is being run in the background or not
                    default:
                        if(background == 1)
                            waitpid(-1, NULL, 0);
                        else
                            signal(SIGCHLD, SIG_IGN);
                        break;
                }
            }
        }
    }
    
}

// Creates a pipe between the two processes that it is given
int createPipe(char* cmd1, char* cmd2)
{
    int pipedes[2];
    pid_t pid1, pid2;
    
        if (pipe(pipedes) == -1)
        {
                fprintf(stderr, "Cannot open pipe\n");
                exit(EXIT_FAILURE);
        }

        if ((pid1 = fork()) == -1)
        {
                fprintf(stderr, "Cannot create child\n");
                exit(EXIT_FAILURE);
        }
        // Child process for writing part of the pipe
        else if (pid1 == 0)
        {
                close(pipedes[0]);
                if (dup2(pipedes[1], STDOUT_FILENO) < 0)
                {
                        perror("Cannot redirect stdout to pipe");
                        exit(EXIT_FAILURE);
                }
                close(pipedes[1]);
                fprintf(stdout, "CALL SYSTEM WITH: %s", cmd1);
                call_system(cmd1);
                exit(EXIT_FAILURE);
        }

        if ((pid2 = fork()) == -1)
        {
                fprintf(stderr, "Cannot create child\n");
                exit(EXIT_FAILURE);
        }
        // Child process for reading part of the pipe
        else if (pid2 == 0)
        {
                close(pipedes[1]);
                if (dup2(pipedes[0], STDIN_FILENO) < 0)
                {
                        perror("Cannot redirect stdin from pipe");
                        exit(EXIT_FAILURE);
                }
                close(pipedes[0]);
                call_system(cmd2);
        }
        
        // Close the pipes as the childered are handling it
        close(pipedes[0]);
        close(pipedes[1]);
        
        // Wait got child processes to finish
        pid_t pid;
        int status;
        while ((pid = wait(&status)) >= 0)
        {
                if (WEXITSTATUS(status) != 0)
                        fprintf(stderr, "Child %d exited with code %d\n", pid, WEXITSTATUS(status));
        }

        return EXIT_SUCCESS;
}

// Small function used to change directories
int changeDir(char* input)
{
    char* string = malloc(sizeof(char) * strlen(input));
    strcpy(string, input);
    
    if(string != NULL)
        if(string[strlen(string)-1] == '\n')                    
            string[strlen(string)-1] = '\0';
    
    return chdir(string);
}

// Builds the arguments for the command to be run
int call_system(char *input)
{  
    char * string = malloc((sizeof(char) * strlen(input)) +1);
    strcpy(string, input);
    
    // Remove new line from the end of the input line
    int length = strlen(string);   
    if(string[length-1] == '\n')
        string[length-1] ='\0';
    
    char ** argArray  = NULL;
    char * ptr   = strtok (string, " ");
    char* command;
    int args = 1;
    int catToken = 0;
    char* tempToken = NULL;
    int i = 1;
    char* escapeCheck = NULL; 
    char* result;
    int resultPos = 0;
    
    
    // While more strings, increase the side of result and add the string
    while (ptr) 
    {
        argArray = realloc (argArray, sizeof (char*) * args);

        // If null then alloc failed
        if (argArray == NULL)
            return 1;
            
       // If the first character is a " remove it
       if(ptr[0] == 34)
       {         
           fprintf(stdout, "REMOVING OPENING QUOTE\n");
            catToken = 1;
            length = strlen(ptr);

            tempToken = malloc(sizeof(char) * length);
            // Removes the " from the start of the string
            for(i = 1; i < length; i++)
            {
                tempToken[i-1] = ptr[i];
            }
            // Check the ends for quotes, making sure not to remove them if they are escaped
            if( tempToken[strlen(tempToken)-1] == 34 && tempToken[strlen(tempToken)-2] != 93 )
            {
                fprintf(stdout, "REMOVING CLOSING QUOTE\n");
                tempToken[strlen(tempToken)-1] = '\0';
                catToken = 0;
            }
       }       
       // If we are concatenating a command then check if it ends with an unescaped " and remove it then add the ptr to the command
       else if(catToken == 1)
       {
            // Add the new token onto the old one
            if(ptr[strlen(ptr)-1] == 34 && ptr[strlen(ptr)-2] != 93)
            {
                ptr[strlen(ptr)-1] = '\0';
                catToken = 0;
            }
            strcat(tempToken, " ");
            strcat(tempToken, ptr);
       }             
       // If we have a tempToken then that is the string we are going to check for escape characters, if not then check the raw ptr token
       if(tempToken != NULL)
       {
           escapeCheck = malloc(sizeof(char) * strlen(tempToken));
           strcpy(escapeCheck, tempToken);
           tempToken = NULL;
       }
       else
       {
           escapeCheck = malloc(sizeof(char) * strlen(ptr));
           strcpy(escapeCheck, ptr);
       }
       
       // Create a string to hold the result of eliminated escape characters
       result = malloc(sizeof(char) * strlen(escapeCheck));
       resultPos = 0;
        
       if(result == NULL)
           fputs("Error allocation memory", stderr);
       
       // Remove any \ that are escaping a "
       for(i = 0; i < strlen(escapeCheck); i++)
       {        
           if( !(escapeCheck[i] == 92 && escapeCheck[i+1] == 34) )
           {
               result[resultPos] = escapeCheck[i];
               resultPos++;
           }
       }       
       
       // Finally put the completed command into the correct location
       if(catToken == 0)
       {
            if(args == 1)
            {
                command  = result;
                argArray[args-1] = basename(result);
            }
            else
            {
                argArray[args-1] = result;
            }
            args++;
       }
       
       ptr = strtok (NULL, " ");     
    }

    // Add on extra space for the null value
    argArray = realloc (argArray, sizeof (char*) * (args+1));
    argArray[args] = '\0';    
        
    execvp(command, argArray);
        
    // Free up memory
    free(result);
    free(string);
    free(ptr);
    
    return 0;
}