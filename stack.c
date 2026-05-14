#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

Stack* create_stack(void) {
    Stack* s = (Stack*)malloc(sizeof(Stack));
    if (!s) {
        printf("ERROR: malloc failed for Stack\n");
        exit(1);
    }
    s->top  = NULL;
    s->size = 0;
    return s;
}

void push(Stack* s, Job* job) {
    if (!s || !job) return;
    SNode* node = (SNode*)malloc(sizeof(SNode));
    if (!node) {
        printf("ERROR: malloc failed for SNode\n");
        exit(1);
    }
    node->job  = job;
    node->next = s->top;
    s->top     = node;
    s->size++;
}

Job* pop(Stack* s) {
    if (!s || !s->top) return NULL;
    SNode* node = s->top;
    Job*   job  = node->job;
    s->top = node->next;
    free(node);
    s->size--;
    return job;
}

int stack_empty(Stack* s) {
    return (!s || s->top == NULL);
}

void stack_free(Stack* s) {
    if (!s) return;
    SNode* cur = s->top;
    while (cur) {
        SNode* next = cur->next;
        free(cur);
        cur = next;
    }
    free(s);
}