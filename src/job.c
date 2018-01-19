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
