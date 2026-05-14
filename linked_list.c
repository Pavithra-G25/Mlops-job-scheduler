#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

LinkedList* create_list(void) {
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    if (!list) {
        printf("ERROR: malloc failed for LinkedList\n");
        exit(1);
    }
    list->head  = NULL;
    list->count = 0;
    return list;
}

void list_add(LinkedList* list, Job* job) {
    if (!list || !job) return;
    if (list->head == NULL) {
        list->head = job;
    } else {
        Job* current = list->head;
        while (current->next != NULL)
            current = current->next;
        current->next = job;
    }
    job->next = NULL;
    list->count++;
}

Job* list_find(LinkedList* list, int id) {
    if (!list) return NULL;
    Job* current = list->head;
    while (current) {
        if (current->id == id) return current;
        current = current->next;
    }
    return NULL;
}

void list_print(LinkedList* list) {
    if (!list || !list->head) {
        printf("  [Empty]\n");
        return;
    }
    Job* current = list->head;
    while (current) {
        print_job(current);
        current = current->next;
    }
}

void list_free(LinkedList* list) {
    if (!list) return;
    Job* current = list->head;
    while (current) {
        Job* next = current->next;
        free(current);
        current = next;
    }
    free(list);
}