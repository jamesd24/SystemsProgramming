#include <stdio.h>
#include <stdlib.h>

int main(int argsc, char *argsv[])
{
	int i;
	char fcont;
	FILE *file;

	for(i = 1; i < argsc; i++)
	{
		file = fopen(argsv[i], "r");
		if(file == NULL)
		{
			printf("%s\n", argsv[i]);			
		}
		else
		{			
			while((fcont = fgetc(file)) != EOF)
			{				
				printf("%c", fcont);								
			}
			printf("\n");			
		}
	}	
	return 0;
}
