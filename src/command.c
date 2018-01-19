#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>

#include "command.h"

void execute_command(char* command, char* options, char* arguments)
{
    struct rusage resource_usage;
    struct timeval start_time, end_time;

    getrusage(RUSAGE_CHILDREN, &resource_usage);
    gettimeofday(&start_time, NULL);
	pid_t pid = fork();
	
    if (pid == -1)
    {
        printf("Failed to fork.\n");
        return;
    }
    
    else if (pid == 0)
    {
        char* args[4];
        
        args[0] = command;
        args[1] = options;
        args[2] = arguments;
        args[3] = NULL;
        
        if (execvp(args[0], args) == -1)
        {
			printf("Failed to exec.\n");
            return;
        }
    }
    
    else
    {
        int status = 0;
		
        waitpid(pid, &status, 0);
		getrusage(RUSAGE_CHILDREN, &resource_usage);
		gettimeofday(&end_time, NULL);
		
		// Calculate the wall-clock time and the cpu time.
		long wall_clock_time = ((end_time.tv_sec - start_time.tv_sec) * 1000000 + end_time.tv_usec - start_time.tv_usec) / 1000;
		long cpu_time = (resource_usage.ru_utime.tv_sec + resource_usage.ru_stime.tv_sec) * 1000 + (resource_usage.ru_utime.tv_usec + resource_usage.ru_stime.tv_usec) / 1000;
		
		printf("Wall-clock time: %ldms\n", wall_clock_time);
		printf("CPU time: %ldms\n", cpu_time);
		printf("Number of involuntary context switch: %ld\n", resource_usage.ru_nivcsw);
		printf("Number of voluntary context switch: %ld\n", resource_usage.ru_nvcsw);
		printf("Page faults: %ld\n", resource_usage.ru_majflt);
		printf("Page reclaims: %ld\n", resource_usage.ru_minflt);
		
		if (status != 0)
        {
			printf("Child exited with status: %d\n", status);
        }
    }
}

/**
    This function execute a builtin, or do nothing if command is not a builtin

    int argc      the argument count
    char* argv[]  the argument value
    return  0 if command is a builtin,
            -1 if the command is a builtin, but an error occured
            1  if the command is not a builtin
*/
int execute_builtin(int argc, char* argv[])
{
    if(strcmp(argv[0], "exit") == 0)
    {
        int exit_val = (argc == 1) ? 0 : strtol(argv[1], NULL, 10);
        exit(exit_val);
    }
    else if (strcmp(argv[0], "cd") == 0)
    {
        char *path = (argc == 1) ? "." : argv[1];
        return chdir(path);
    }
    return 1;
}