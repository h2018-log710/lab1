#include <stdio.h>
#include <stdlib.h>

#include "job.h"

job* last_job = NULL;

job* create_job_entry(pid_t pid)
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

    return new_job;
}

void cleanup_jobs()
{
    job* job_ptr = last_job;
    while (job_ptr != NULL)
    {
        job_ptr = job_ptr->previous;
    }
}

void list_jobs(const job* job)
{
    if (job == NULL)
    {
        printf("No background jobs.\n");
        return;
    }

    if (job->previous != NULL)
    {
        list_jobs(job->previous);
    }
    printf("[%d] %d\n", job->job_id, job->pid);
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