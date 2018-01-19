#include <stdio.h>
#include <stdlib.h>

#include "job.h"

job* last_job = NULL;

void list_jobs(const job* job) {
    if(job->previous != NULL)
    {
        list_jobs(job->previous);
    }
    printf("[%d] %d\n", job->job_id, job->pid);
}

int get_job_count()
{
    job* job_ptr = last_job;
    int job_count = 0;

    while(job_ptr != NULL)
    {
        job_count += 1;
        job_ptr = job_ptr->previous;
    }
    return job_count;
} 