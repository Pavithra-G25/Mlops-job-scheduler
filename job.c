#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "job.h"

Job* create_job(int id, const char* name, int priority, int max_retries) {
    Job* job = (Job*)malloc(sizeof(Job));
    if (!job) {
        printf("ERROR: malloc failed for Job %d\n", id);
        exit(1);
    }
    job->id          = id;
    strncpy(job->name, name, 49);
    job->name[49]    = '\0';
    job->priority    = priority;
    job->status      = PENDING;
    job->retry_count = 0;
    job->max_retries = max_retries;
    job->next        = NULL;
    return job;
}

const char* status_to_string(int status) {
    switch (status) {
        case PENDING:   return "PENDING";
        case RUNNING:   return "RUNNING";
        case COMPLETED: return "COMPLETED";
        case FAILED:    return "FAILED";
        default:        return "UNKNOWN";
    }
}

void print_job(Job* job) {
    if (!job) return;
    printf("  [%d] %-25s | Priority: %d | Status: %-10s | Retries: %d/%d\n",
        job->id, job->name, job->priority,
        status_to_string(job->status),
        job->retry_count, job->max_retries);
}