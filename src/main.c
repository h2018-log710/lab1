#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "command.h"

int main(int argc, char* argv[])
{
	while (true)
	{
		char input[255];
		printf("Log710H2018%%>");
		fgets(input, 255, stdin);
		
		if (input[0] != '\n')
		{
			int count = 0;
			char* arguments[255];
			char* output = strtok(input, " ");
			
			while (output != NULL)
			{
				arguments[count++] = output;
				output = strtok(NULL, " ");
			}
			
			arguments[count - 1] = strtok(arguments[count - 1], "\n");
			arguments[count] = NULL; // The last argument should be null when using execvp.
			
			execute_command(count, arguments);
		}
	}
	
    return 0;
}
