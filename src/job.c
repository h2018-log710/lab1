#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "job.h"

job* last_job = NULL;

job* create_job_entry(pid_t pid, char* job_name)
{
	job* new_job = (job*) malloc(sizeof(job));

    strcpy(new_job->job_name, job_name);
			
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

    return new_job;
}

void cleanup_finished_jobs()
{
    job* job_ptr = last_job;
    while (job_ptr != NULL)
    {
        job* next_job = job_ptr->next;
        job* previous_job = job_ptr->previous;

        if (waitpid(job_ptr->pid, NULL, WNOHANG) != 0)
        {
            if (previous_job == NULL && next_job == NULL)
            {
                // No more jobs
                last_job = NULL;
            }

            if (previous_job != NULL)
            {
                previous_job->next = next_job;
            }

            if (next_job != NULL)
            {
                next_job->previous = previous_job;
            }

            if (job_ptr == last_job)
            {
                // The job we removed from list was the last_job, the previous job is the new last_job
                last_job = previous_job;
            }

            free(job_ptr);
            job_ptr = NULL;
        }
        job_ptr = previous_job;
    }
}

void list_jobs(const job* job_ptr)
{
    if (job_ptr == NULL)
    {
        printf("No background jobs.\nls");
        return;
    }

    if (job_ptr->previous != NULL)
    {
        list_jobs(job_ptr->previous);
    }
    printf("[%d] %d %s\n", job_ptr->job_id, job_ptr->pid, job_ptr->job_name);
}

int get_job_count()
{
    job* job_ptr = last_job;
    int job_count = 0;

    while (job_ptr != NULL)
    {
        job_count += 1;
        job_ptr = job_ptr->previous;
    }
    return job_count;
}