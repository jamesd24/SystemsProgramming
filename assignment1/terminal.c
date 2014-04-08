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
int length;

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
                      
            /** Find what the command was */
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
                        if(background == 0)
                            waitpid(-1, NULL, 0);
                        break;
                }
            }
        }        
    }
    
    return 0;
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
    
    char ** result  = NULL;
    char * ptr   = strtok (string, " ");
    char* command;
    int args = 1;
    int catToken = 0;
    char* tempToken;
    int i = 1;
    
    // While more strings, increase the side of result and add the string
    while (ptr) 
    {
        result = realloc (result, sizeof (char*) * args);

        // If null then alloc failed
        if (result == NULL)
            return 1;

        // 34 is the ascii for "
        if(ptr[0] == 34)
        {            
            catToken = 1;  
            length = strlen(ptr);
            
            tempToken = malloc(sizeof(char) * length);
            // Removes the " from the start of the string
            for(i = 1; i < length; i++)
            {
                tempToken[i-1] = ptr[i];
            }
            
            length = strlen(tempToken);
            
            // If " is the last character as well then this is simply a full command
            if(tempToken[length-1] == 34)
            {
                tempToken[length-1] = '\0';
                
                if(args == 1)
                {
                    command  = tempToken;
                    result[args-1] = basename(ptr);
                }
                else
                {
                    result[args-1] = ptr;
                }
            }
        }
        else if(catToken == 1)
        {
            length = strlen(ptr);
            // If the last character is " then this is the end of the string
            if(ptr[length-1] == 34)
            {
                ptr[length-1] = '\0';
                strcat(tempToken, " ");
                strcat(tempToken, ptr);
                                
                if(args == 1)
                {
                    command  = tempToken;
                    result[args-1] = basename(ptr);
                }
                else
                {
                    result[args-1] = ptr;
                }
                
                catToken = 0;
            }
            else
            {
                strcat(tempToken, " ");
                strcat(tempToken, ptr);          
            }                      
        }
        else
        {
            if(args == 1)
            {
                command = ptr;
                result[args-1] = basename(ptr);
            }
            else
            {
                result[args-1] = ptr;
            }
            
            args++;
        }             

       ptr = strtok (NULL, " ");
    }

    // Add on extra space for the null value
    result = realloc (result, sizeof (char*) * (args+1));
    result[args] = '\0';
    
    execvp(command, result);
        
    // Free up memory
    free(result);
    free(string);
    free(ptr);
    
    return 0;
}