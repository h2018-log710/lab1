#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/resource.h>

#include "main.h"

void execute_command(const char* command, const char* options, const char* arguments)
{
    struct rusage resource_usage;
    pid_t pid = fork();
    int status = 0;
    
    if (pid == -1)
    {
        printf("Failed to fork.\n");
    }
    
    else if (pid == 0)
    {
        char* args[4];
        
        args[0] = command;
        args[1] = options;
        args[2] = arguments;
        args[3] = NULL;
        
        getrusage(RUSAGE_SELF, &resource_usage);
        
        if (execvp(args[0], args) == -1)
        {
            return;
        }
    }
    
    else
    {
        waitpid(pid, &status, 0);
        
        if (status == 0)
        {
            getrusage(RUSAGE_SELF, &resource_usage);
			
            printf("User time used: %d\n"
				"System time used: %d\n"
				"Number of involuntary context switch: %d\n"
				"Number of voluntary context switch: %d\n"
				"Page Faults: %d\n"
				"Page Reclaims: %d\n", resource_usage.ru_utime, resource_usage.ru_stime, resource_usage.ru_nivcsw, resource_usage.ru_nvcsw, resource_usage.ru_majflt, resource_usage.ru_minflt);
        }
    }
}

int main(int argc, char* argv[])
{
	while (true)
	{
		char input[255];
		printf("Log710H2018%>");
		fgets(input, 255, stdin);
		
		execute_command(argv[1], argv[2], argv[3]); 
	}
	
    return 0;
}
