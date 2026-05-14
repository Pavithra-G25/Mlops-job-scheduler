#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "job.h"

typedef struct {
    Job* head;
    int  count;
} LinkedList;

LinkedList* create_list(void);
void        list_add(LinkedList* list, Job* job);
Job*        list_find(LinkedList* list, int id);
void        list_print(LinkedList* list);
void        list_free(LinkedList* list);

#endif