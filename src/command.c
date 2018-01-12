#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h>

#include "command.h"

void execute_command(char* command, char* options, char* arguments)
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
			
            printf("User time used: %lds\n"
				"System time used: %lds\n"
				"Number of involuntary context switch: %ld\n"
				"Number of voluntary context switch: %ld\n"
				"Page Faults: %ld\n"
				"Page Reclaims: %ld\n", resource_usage.ru_utime.tv_sec, resource_usage.ru_stime.tv_sec, resource_usage.ru_nivcsw, resource_usage.ru_nvcsw, resource_usage.ru_majflt, resource_usage.ru_minflt);
        }
    }
}


