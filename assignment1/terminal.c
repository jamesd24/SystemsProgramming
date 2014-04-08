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


int main()
{
    while(loop == 0)
    {
        // Gets the current working directory for the application and prints it out
        char currDir[256];
        getcwd(currDir, 255);
        fprintf(stdout, "%s\n", currDir);
        // prints a couple of >> to prompt input and then gets the user intput
        fputs(">> ", stdout);
        fgets(input, buffer, stdin);
        
        // Check if the exit command was given, limit of 4 due to buffer messing up the compare over 4 letters
        if(strncmp(input, "exit", 4) == 0)
        {
            loop = 1;
        }
        else
        {
            int pid, background = 0;  
            int length = strlen(input); 
            
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
                      
            /** Find what the command was */
            char* checkString = malloc ((sizeof (char*) * strlen(input)) +1 );;
            
            strcpy(checkString, input);
            char* token = strtok(checkString, " ");
            
            // If we're being told to change directory then we don't need to run in a new process
            if(strncmp(token, "cd", 2) == 0)
            {
                token = strtok (NULL, " ");                
                if(token != NULL)
                    if(token[strlen(token)-1] == '\n')                    
                        token[strlen(token)-1] = '\0';
                    
                chdir(token);
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
                        if(background == 0)
                            waitpid(-1, NULL, 0);
                        break;
                }
            }
        }        
    }
    
    return 0;
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
    
    char ** result  = NULL;
    char * ptr   = strtok (string, " ");
    char* command;
    int args = 0;
    
    // While more strings, increase the side of result and add the string
    while (ptr) 
    {
        result = realloc (result, sizeof (char*) * ++args);

        // If null then alloc failed
        if (result == NULL)
            return 1;

        if(args == 1)
        {
            command = ptr;
            result[args-1] = basename(ptr);
        }
        else
        {
            result[args-1] = ptr;
        }
            

       ptr = strtok (NULL, " ");
    }

    // Add on extra space for the null value
    result = realloc (result, sizeof (char*) * (args+1));
    result[args] = 0;
    
    execvp(command, result);
    
    
    // Free up memory
    free(result);
    free(string);
    free(ptr);
    
    return 0;
}