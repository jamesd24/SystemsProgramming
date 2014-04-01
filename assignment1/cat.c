#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "cat.h"

extern char *optarg;
extern int optind;
int showEnd = 0;
int showLineNum = 0; 
//None blank line numbers only
int nb_line = 0;
int squeeze = 0;
// Print the none visible characters
int extra_char = 0; 
int tabs = 0; 
int error = 0;

int lineNum = 0;
char* lineEnd = "";
	
int main(int argsc, char *argsv[])
{
	int c;
	
        
	while ((c = getopt(argsc, argsv, "Enbsvte")) != -1)
	{
            switch(c)
            {
                case 'E':
                    showEnd = 1;
                    break;
                case 'n':
                    showLineNum = 1;
                    break;
                case 'b':
                    nb_line = 1;
                    break;
                case 's':
                    squeeze = 1;
                    break;
                case 'v':
                    extra_char = 1;
                    break;
                case 't':
                    tabs = 1;
                    break;
                case 'e':
                    showEnd = 1;
                    extra_char = 1;
                    break;
                case '?':
                    error = 1;
                    break;
            }
	}
	
	// If there was an error in the setup then do not run
	if(error != 1)
	{
            if(argsc == 1)
            {				
                do		
                {
                        c = fgetc(stdin);
                        printString(fputc(c, stdout));
                } while(c != EOF);
            }
            else
            {		

                if (optind < argsc)
                {
                        for(; optind < argsc; optind++)
                        {			
                                printfile(argsv[optind]);			
                        }
                }
            }
	}	
	return 0;
}

/*
 * Checks if a file input through the command line is a file and is so prints its contents  
 */

//TODO can probably be reduced in lines if I get time to have a look at all the logic in play there, a decent amount of duplicate code in play
void printfile(char* input)
{
    
        FILE* file;
        char c;
        int squeezed = 0;
        
        if(showEnd == 1)
        {
                lineEnd = "$";
        }
                        
        if( (file = fopen(input, "r")) == NULL )
        {
                fputs(input, stderr);
                fputs(": No such file or directory\n", stderr);
        }
        else
        {
                      
            // Used to check if the last character was EOL
            // Set to 1 initially to print the first line number
            int endOfLine = 1;            
            
            while( ((c = fgetc(file)) != EOF) )
            {               
                    if(c == 10)
                    {       //If the previous character was also an end of line        
                            if(endOfLine == 1)
                            {   //If we are squeezing out extra blank lines  
                                if(squeeze == 1)
                                {   // Checks what the number of squeezed lines is currently
                                    if(squeezed < 1)
                                    {
                                        if(nb_line == 1)
                                        {
                                            printString(fprintf(stdout, "%s%c", lineEnd, c));
                                        }
                                        else if(showLineNum == 1)
                                        {
                                            printString(fprintf(stdout, "\t%d  %s%c", ++lineNum, lineEnd, c));
                                        }
                                        else
                                        {
                                            printString(fprintf(stdout, "%s%c", lineEnd, c));
                                        }
                                        squeezed++;
                                    }
                                 }
                                 else
                                 {
                                     if(nb_line == 1)
                                     {
                                         printString(fprintf(stdout, "%s%c", lineEnd, c));
                                     }
                                     else if(showLineNum == 1)
                                     {
                                         printString(fprintf(stdout, "\t%d  %s%c", ++lineNum, lineEnd, c));
                                     }
                                     else
                                     {
                                         printString(fprintf(stdout, "%s%c", lineEnd, c));
                                     }
                                 }
                            }
                            //If the last line wasn't also an end of line
                            else
                            {                
                                endOfLine = 1;
                                printString(fprintf(stdout, "%s%c", lineEnd, c));
                            }
                    }
                    else
                    {
                        squeezed = 0;
                        //If we are showing line numbers and the last line was an end of line then put a new line number in
                        if( (showLineNum == 1 || nb_line == 1) && endOfLine == 1)
                        {
                                printString(fprintf(stdout, "\t%d  %c", ++lineNum, c));                                
                        }
                        else
                        {
                            // If we are printing hidden characters or tabs then check the type of character
                            if(extra_char == 1 || tabs == 1)
                            {
                                switch(c)
                                {
                                    // If it is a tab, check if we are printing tabs
                                    case 9:
                                        if(tabs == 1)
                                            printString(fputs("^I", stdout));
                                        else
                                            printString(fputc(c, stdout));
                                        break;
                                    case 24:
                                            printString(fputs("^X", stdout));
                                        break;
                                    case 4:
                                            printString(fputs("^D", stdout));
                                        break;
                                    default:
                                        printString(fputc(c, stdout));
                                        break;
                                }
                            }
                            else                                   
                            {
                                printString(fputc(c, stdout));
                            }                        
                                
                        }
                        endOfLine=0;
                    }
            }
            fclose(file);
        }
}

// Takes the return value from an output function and check if it was an error
void printString(int output)
{
    if( output == 0 && (ferror(stdout) == 0) )
    {
        exit(EXIT_FAILURE);
    }
}

// Unused code that might be useful later on
/**
char* stringBuilder(const char* message, const char append)
{
    size_t message_len = strlen(message);
    char *result = (char*) malloc(message_len+2);
    strcat(result, message);
    
    result[message_len] = append;
    result[message_len+1] = '\0';
        
    return result;
}

void printString(char* output)
{
    printString(fprintf(stdout, "%s%s", output, lineEnd);
}

*/