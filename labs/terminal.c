#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>


#define buffer 80
int loop = 0;
char input[buffer];

int main()
{
    while(loop == 0)
    {
        fgets(input, buffer, stdin);
        
        if(strncmp(input, "exit", 4) == 0)
        {
            exit(EXIT_SUCCESS);
        }
        else
        {
            fputs(input, stdout);
        }
        
        //fprintf(stdout, "%d", strncmp(input, exitString, 4));
    }
    
    return 0;
}