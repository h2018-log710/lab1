#include <stdio.h>
#include <stdlib.h>

void list_job(const job* job) {
    if(job->previous != NULL)
    {
        recurse_job_list(job->previous)
    }
    printf("[%d] %ld", job->job_id, job->pid);
}
