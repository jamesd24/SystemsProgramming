#include <stdio.h>
#include <stdlib.h>

int main(int argsc, char *argsv[])
{
	if(argsc == 1)
	{
		int c;
		while(1)
		{
			while( (c = fgetc(stdin)) != 10)
			{
				fputc(c, stdout);	
			}
			fputc(10, stdout);
		}
	}
	else
	{
		int i;
		char fcont;
		FILE *file;

		for(i = 1; i < argsc; i++)
		{
			if( (file = fopen(argsv[i], "r")) == NULL )
			{
				fputs(argsv[i], stdout);
				fputs(" ", stdout);
			}
			else
			{
				while( (fcont = fgetc(file)) != EOF)
				{
					fputc(fcont, stdout);
				}
			}
		}
		printf("\n");
	}
	return 0;
}
