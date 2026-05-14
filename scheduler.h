#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "job.h"
#include "linked_list.h"
#include "queue.h"
#include "stack.h"
#include "hashmap.h"
#include "graph.h"

typedef struct {
    LinkedList* job_store;
    Queue*      job_queue;
    Stack*      retry_stack;
    HashMap*    status_map;
    Graph*      dep_graph;
} Scheduler;

Scheduler* create_scheduler(void);
void       scheduler_add_job(Scheduler* s, Job* job);
void       scheduler_run(Scheduler* s);
void       scheduler_report(Scheduler* s);
void       scheduler_free(Scheduler* s);

#endif