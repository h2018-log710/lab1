#include <stdio.h>
#include <stdbool.h>

#include "command.h"

int main(int argc, char* argv[])
{
	while (true)
	{
		char input[255];
		printf("Log710H2018%%>");
		fgets(input, 255, stdin);
		
		execute_command(argv[1], argv[2], argv[3]); 
	}
	
    return 0;
}
