#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>

#include "command.h"
#include "job.h"

/**
    This function execute a command

    int argc      the argument count
    char* argv[]  the argument value
    return  0 if the process was successfully created,
            -1 if the process failed to be created
*/
int execute_command(int argc, char* argv[])
{
    struct rusage resource_usage;
    struct timeval start_time, end_time;
    bool is_background = strchr(argv[argc - 1], '&') != NULL;
    printf("%d\n", is_background);
    
    getrusage(RUSAGE_CHILDREN, &resource_usage);
    gettimeofday(&start_time, NULL);
    pid_t pid = fork();
    
    if (pid == -1)
    {
        printf("Failed to fork.\n");
        return -1;
    }
    
    else if (pid == 0)
    {
        if (execvp(argv[0], argv) == -1)
        {
            printf("Failed to exec.\n");
            return -1;
        }
    }
    
    else
    {
        int status = 0;
        
        if (!is_background)
        {
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
        
		else
		{
			job* new_job = (job*) malloc(sizeof(job));
			
			new_job->pid = pid;
			new_job->next = NULL;
			
			if (last_job == NULL)
			{
				new_job->job_id = 0;
				new_job->previous = NULL;
			}
			
			else
			{
				new_job->job_id = last_job->job_id + 1;
				new_job->previous = last_job;
				
				last_job->next = new_job;
			}
			
			last_job = new_job;
		}
    }
	
	return 0;
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
    if (strcmp(argv[0], "exit") == 0)
    {
        int job_count = get_job_count();
        if (job_count != 0)
        {
            printf("There are %d background jobs running.\n", job_count);
            return 0;
        }
        else
        {
            int exit_val = (argc == 1) ? 0 : strtol(argv[1], NULL, 10);
            exit(exit_val);
        }
    }
	
    else if (strcmp(argv[0], "cd") == 0)
    {
        char* path = (argc == 1) ? "." : argv[1];
        return chdir(path);
    }

    else if (strcmp(argv[0], "aptaches") == 0)
    {
        list_jobs(last_job);
        return 0;
    }
	
    return 1;
}