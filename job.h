#ifndef JOB_H
#define JOB_H

#define PENDING     0
#define RUNNING     1
#define COMPLETED   2
#define FAILED      3

#define TOTAL_JOBS    8
#define MAX_RETRIES   3
#define MAX_CONCURRENT 2

typedef struct Job {
    int  id;
    char name[50];
    int  priority;
    int  status;
    int  retry_count;
    int  max_retries;
    struct Job* next;
} Job;

Job*        create_job(int id, const char* name, int priority, int max_retries);
const char* status_to_string(int status);
void        print_job(Job* job);

#endif