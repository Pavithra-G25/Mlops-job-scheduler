#ifndef QUEUE_H
#define QUEUE_H

#include "job.h"

typedef struct QNode {
    Job*          job;
    struct QNode* next;
} QNode;

typedef struct {
    QNode* front;
    QNode* rear;
    int    size;
} Queue;

Queue* create_queue(void);
void   enqueue(Queue* q, Job* job);
Job*   dequeue(Queue* q);
int    queue_empty(Queue* q);
void   queue_free(Queue* q);

#endif