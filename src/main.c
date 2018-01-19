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
		
		int count = 0;
		char* arguments[255];
		char* output = strtok(input, " ");
		
		while (output != NULL)
		{
			arguments[count++] = output;
			output = strtok(NULL, " ");
		}
		
		execute_command(count, arguments);
	}
	
    return 0;
}
