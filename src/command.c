#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>

#include "command.h"

void execute_command(int argc, char* argv[])
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
        if (execvp(argv[0], argv) == -1)
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
		
		printf("=======================================\n");
		printf("Wall-clock time: %ldms\n", wall_clock_time);
		printf("CPU time: %ldms\n", cpu_time);
		printf("Number of involuntary context switch: %ld\n", resource_usage.ru_nivcsw);
		printf("Number of voluntary context switch: %ld\n", resource_usage.ru_nvcsw);
		printf("Page faults: %ld\n", resource_usage.ru_majflt);
		printf("Page reclaims: %ld\n", resource_usage.ru_minflt);
		printf("=======================================\n");
		
		if (status != 0)
        {
			printf("Child exited with status: %d\n", status);
        }
    }
}
