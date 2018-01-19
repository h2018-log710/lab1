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
		
		char* output = strtok(input, " ");
		char* arguments[255];
		int count = 0;
		
		while (output != NULL)
		{
			printf("Output: %s", output);
			arguments[count++] = output;
			output = strtok(input, " ");
		}
		
		execute_command(count, arguments);
	}
	
    return 0;
}
