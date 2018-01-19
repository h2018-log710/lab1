#ifndef JOB_H
#define JOB_H

#include <sys/types.h>

typedef struct job
{
    pid_t pid;
    int job_id;
    char job_name[255];
    struct job* previous;
    struct job* next;
} job;

extern job* last_job;

job* create_job_entry(pid_t pid, char* job_name);
void cleanup_finished_jobs();
void list_jobs(const job* job);
int get_job_count();

#endif
