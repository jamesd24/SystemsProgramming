#include <stdio.h>
#include <stdlib.h>

int main(int argsc, char *argsv[])
{
	int c;
	
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
		int i;
		FILE *file;

		for(i = 1; i < argsc; i++)
		{
			if( (file = fopen(argsv[i], "r")) == NULL )
			{
				fputs(argsv[i], stdout);
				fputs(": No such file or directory\n", stdout);
			}
			else
			{				
				do		
				{
					c = fgetc(file);
					fputc(c, stdout);	
				} while(c != EOF);
			}
		}
	}
	return 0;
}
