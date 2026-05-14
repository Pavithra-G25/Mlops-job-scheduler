#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

Queue* create_queue(void) {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    if (!q) {
        printf("ERROR: malloc failed for Queue\n");
        exit(1);
    }
    q->front = NULL;
    q->rear  = NULL;
    q->size  = 0;
    return q;
}

void enqueue(Queue* q, Job* job) {
    if (!q || !job) return;
    QNode* node = (QNode*)malloc(sizeof(QNode));
    if (!node) {
        printf("ERROR: malloc failed for QNode\n");
        exit(1);
    }
    node->job  = job;
    node->next = NULL;
    if (q->rear == NULL) {
        q->front = node;
        q->rear  = node;
    } else {
        q->rear->next = node;
        q->rear       = node;
    }
    q->size++;
}

Job* dequeue(Queue* q) {
    if (!q || !q->front) return NULL;
    QNode* node = q->front;
    Job*   job  = node->job;
    q->front = node->next;
    if (q->front == NULL) q->rear = NULL;
    free(node);
    q->size--;
    return job;
}

int queue_empty(Queue* q) {
    return (!q || q->front == NULL);
}

void queue_free(Queue* q) {
    if (!q) return;
    QNode* cur = q->front;
    while (cur) {
        QNode* next = cur->next;
        free(cur);
        cur = next;
    }
    free(q);
}