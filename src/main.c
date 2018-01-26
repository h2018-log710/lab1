#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#include "command.h"
#include "job.h"

int main(int argc, char* argv[])
{
	// Ignore SIGCHLD to avoid them becoming zombies
	signal(SIGCHLD, SIG_IGN);

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

			cleanup_finished_jobs();

			switch (execute_builtin(count, arguments))
			{
				// Not a builtin, try to execute the command.
				case 1:
					if (execute_command(count, arguments) == 0)
					{
						break; // The command was executed successfully. No need to print an error.
					}
				case -1:
					printf("%s\n", strerror(errno));
					break;
				default:
					// Nothing should be done.
					break;
			}
		}
	}
	
    return 0;
}
