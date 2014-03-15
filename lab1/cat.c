#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "cat.h"

extern char *optarg;
extern int optind;
int showEnd = 0;
int lineNum = 0;
int error = 0;
	
int main(int argsc, char *argsv[])
{
	int c;
	
	while ((c = getopt(argsc, argsv, "En")) != -1)
	{
		switch(c)
		{
			case 'E':
				showEnd = 1;
				break;
			case 'n':
				lineNum = 1;
				break;
			case '?':
				printf("Unknown argument: %c", c);
				break;
		}
	}
	
	if(error == 1)
	{
		fputs("Unknown argument", stdout);
	}
	
	if(argsc == 1)
	{				
		do		
		{
			c = fgetc(stdin);
			fputc(c, stdout);
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
	return 0;
}

/*
 * Checks if a file input through the command line is a file and is so prints its contents  
 */
void printfile(char* input)
{
		FILE *file;
		int c;
				
		if( (file = fopen(input, "r")) == NULL )
		{
			fputs(input, stdout);
			fputs(": No such file or directory\n", stdout);
		}
		else
		{				
			int i = 1;
			int endOfLine = 0;
			
			// Put the initial line number in there
			if(lineNum == 1)
			{
				printf("     %d  ",i);
			}

			// Need some way to print the line number before the text
			while( ((c = fgetc(file)) != EOF) )
			{
				if(c == 10)
				{
					if(endOfLine == 1)
					{
						i++;
						printf("     %d  %c", i, c);
					}
					else
					{
						endOfLine = 1;
						fputc(c, stdout);
					}
				}
				else
				{
					if(lineNum == 1 && endOfLine == 1)
					{
						i++;
						printf("     %d  %c", i, c);
						endOfLine=0;
					}
					else
					{
						fputc(c, stdout);
					}
				}
			}			
			fclose(file);			
		}
}

