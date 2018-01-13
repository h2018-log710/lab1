#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>

#include "command.h"

long get_interval(struct timeval *start, struct timeval *end) {
    return ((end->tv_sec - start->tv_sec) * 1000000 + end->tv_usec - start->tv_usec) / 1000;
}

void execute_command(char* command, char* options, char* arguments)
{
    struct rusage resource_usage;
    struct timeval start_time, end_time;

    getrusage(RUSAGE_CHILDREN, &resource_usage);
    gettimeofday(&start_time, NULL);
    printf("%ld", start_time.tv_usec);

    pid_t pid = fork();
    if (pid == -1)
    {
        printf("Failed to fork.\n");
        return; //TODO error code
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
            return; //TODO error code
        }
    }
    
    else
    {
        int status = 0;
        waitpid(pid, &status, 0);
        getrusage(RUSAGE_CHILDREN, &resource_usage);
        gettimeofday(&end_time, NULL);
        printf("s: %ld e:%ld", start_time.tv_usec, end_time.tv_usec);

        // Calculate the wall-clock time and the cpu time
        long total_time = get_interval(&start_time, &end_time);
        long cpu_time = (resource_usage.ru_utime.tv_sec + resource_usage.ru_stime.tv_sec) * 1000 + (resource_usage.ru_utime.tv_usec + resource_usage.ru_stime.tv_usec) / 1000 ;
        
        if (status == 0)
        {
            printf("Wall-clock time: %ldms\n", total_time);
			printf("CPU time used: %ldms\n", cpu_time);
			printf("Number of involuntary context switch: %ld\n", resource_usage.ru_nivcsw);
			printf("Number of voluntary context switch: %ld\n", resource_usage.ru_nvcsw);
			printf("Page Faults: %ld\n", resource_usage.ru_majflt);
			printf("Page Reclaims: %ld\n", resource_usage.ru_minflt);
        }
    }
}


