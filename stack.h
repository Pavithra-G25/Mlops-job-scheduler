#ifndef STACK_H
#define STACK_H

#include "job.h"

typedef struct SNode {
    Job*          job;
    struct SNode* next;
} SNode;

typedef struct {
    SNode* top;
    int    size;
} Stack;

Stack* create_stack(void);
void   push(Stack* s, Job* job);
Job*   pop(Stack* s);
int    stack_empty(Stack* s);
void   stack_free(Stack* s);

#endif
