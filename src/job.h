#ifndef JOB_H
#define JOB_H

typedef struct job
{
    pid_t pid;
    int job_id;
    struct job* previous;
    struct job* next;
} job;

extern job* last_job;

job* create_job_entry(pid_t pid);
void cleanup_jobs();
void list_jobs(const job* job);
int get_job_count();

#endif
