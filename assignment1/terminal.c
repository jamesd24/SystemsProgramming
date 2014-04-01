#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include "terminal.h"


#define buffer 80
int loop = 0;
char input[buffer];


int main()
{
    while(loop == 0)
    {
        fgets(input, buffer, stdin);
        
        // Check if the exit command was given, limit of 4 due to buffer messing up the compare over 4 letters
        if(strncmp(input, "exit", 4) == 0)
        {
            exit(EXIT_SUCCESS);
        }
        else
        {
            //char* arg[] = {"ls", "-ltr", NULL};
            /**char* command = "ls";
            char* arg[] = {"ls", "-ltr", NULL};
            fprintf(stdout, "command: %s\n", command);
            fprintf(stdout, "args 0: %s\n", arg[0]);
            fprintf(stdout, "args 1: %s\n", arg[1]);
            fprintf(stdout, "args 1: %s\n", arg[2]);
            execvp(arg[0], arg);*/
            
            
            int pid;
    
            pid = fork();
            
            if(pid == -1)
            {
                fputs("Forking failed", stderr);
            }
            else if (pid > 0)
            {
                waitpid(-1, NULL, 0);
            }
            else
            {
                call_system(input);
            }
                        
        }        
    }
    
    return 0;
}

// Builds the arguments for the command to be run
void call_system(char *input)
{  
    char * string = input;
    
    char ** result  = NULL;
    char * ptr   = strtok (string, " ");
    int n_spaces = 0, i;


    /* split string and append tokens to 'result' */

    while (ptr) 
    {
        result = realloc (result, sizeof (char*) * ++n_spaces);

        if (result == NULL)
            exit (-1); /* memory allocation failed */

        result[n_spaces-1] = ptr;

       ptr= strtok (NULL, " ");
    }

    /* realloc one extra element for the last NULL */

    result = realloc (result, sizeof (char*) * (n_spaces+1));
    result[n_spaces] = 0;

    /* print the resultult */

    for (i = 0; i < (n_spaces+1); ++i)
    printf ("result[%d] = %s\n", i, result[i]);

    
    execvp(result[0], result);
    
    /* free the memory allocated */
    free (result);
       
    
    
    /**char * string = input;
    char * argArr[128];
    char * command;
    char *token = strtok( string, " " );
    int numArgs = -1;
    int flag;
    
    command = token;
    argArr[++numArgs] = token;
 
    while( token != NULL )
    {
            token = strtok( NULL, " " );
            argArr[++numArgs] = token;
    }
    argArr[numArgs] = '\0';
    
    fprintf(stdout, "command: %s\n", command);
    fprintf(stdout, "args 0: %s\n", argArr[0]);
    fprintf(stdout, "args 1: %s\n", argArr[1]);
    
    flag = execvp(command, argArr);
    
    fprintf(stdout, "Return value: %d", flag);*/
    
    
    /**char *a[1];
    int n=0, nn;

    char *ds=strdup(input);

    a[n]=strtok(ds, " ");
    while(a[n] && n<4) 
        a[++n]=strtok(NULL, " ");

    for(nn=0; nn<=n; ++nn) 
        printf("%s ", a[nn]);
    putchar('\n');

    free(ds);
    
    execvp(a[0], a);*/
        
    /**char *args[64];
    char **next = args;
    char *temp = strtok(input, " ");
    while (temp != NULL)
    {
        *next++ = temp;
        //printf("%s\n", temp);
        temp = strtok(NULL, " ");
    }
    *next = NULL;

    puts("Checking:");
    for (next = args; *next != 0; next++)
        puts(*next);

    execvp(args[0], args);*/

    /**
    char *commandArgs;
    char **cmdptr = commandArgs;
    char *token = strtok(input, " ");
    int numArgs = 0;
    
    
    commandArgs = malloc(numArgs+1);
    token = strtok(input, " ");
    commandArgs[numArgs] = token;
    //strcpy(commandArgs[numArgs], token);
    token = strtok(NULL, " ");
    
    
    while (token != NULL)
    {           
        //commandArgs = malloc(numArgs+1); 
        commandArgs = realloc(commandArgs, numArgs+1);
        *cmdptr++ = token;    
        token = strtok(NULL, " ");
        numArgs++;
    }
    
    //fprintf(stdout, "command: %s\n", command);
    //fprintf(stdout, "args 0: %s\n", commandArgs[0]);
    //fprintf(stdout, "args 1: %s\n", commandArgs[1]);
    
    //fprintf(stdout, "Command: %s", command);
    execvp(commandArgs[0], commandArgs);
    //system(command);*/
    
}