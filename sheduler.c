#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "scheduler.h"

static void timestamp(char* buf, int sz) {
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    strftime(buf, sz, "%H:%M:%S", t);
}

static void log_msg(int id, const char* name, const char* msg) {
    char ts[12];
    timestamp(ts, sizeof(ts));
    printf("[%s] Job[%d] %-25s  %s\n", ts, id, name, msg);
}

/*
 * check_deps checks if all predecessor jobs of job_id are COMPLETED.
 * Returns  1 : all dependencies completed, job can run
 * Returns  0 : some dependency not yet done
 * Returns -1 : a dependency permanently failed, skip this job
 *
 * Graph edges go from src to dest meaning src must complete before dest.
 * To find predecessors of job_id, scan all adjacency lists for edges
 * pointing to job_id's index.
 */
static int check_deps(Scheduler* s, int job_id) {
    int idx = job_id - 1;
    for (int i = 0; i < TOTAL_JOBS; i++) {
        AdjNode* cur = s->dep_graph->adj[i];
        while (cur) {
            if (cur->dest == idx) {
                int st = map_get(s->status_map, i + 1);
                if (st == FAILED)    return -1;
                if (st != COMPLETED) return  0;
            }
            cur = cur->next;
        }
    }
    return 1;
}

/* 30 percent chance of failure to simulate real job failures */
static int simulate_execution(void) {
    return (rand() % 10) >= 3 ? 1 : 0;
}

Scheduler* create_scheduler(void) {
    Scheduler* s = (Scheduler*)malloc(sizeof(Scheduler));
    if (!s) {
        printf("ERROR: malloc failed for Scheduler\n");
        exit(1);
    }
    s->job_store   = create_list();
    s->job_queue   = create_queue();
    s->retry_stack = create_stack();
    s->status_map  = create_hashmap();
    s->dep_graph   = create_graph(TOTAL_JOBS);
    return s;
}

void scheduler_add_job(Scheduler* s, Job* job) {
    if (!s || !job) return;
    list_add(s->job_store, job);
    map_set(s->status_map, job->id, PENDING);
    log_msg(job->id, job->name, "SUBMITTED to scheduler");
}

void scheduler_run(Scheduler* s) {
    if (!s) return;

    printf("\n");
    printf("=============================================================\n");
    printf("     MLOps Production Pipeline - Scheduler Starting          \n");
    printf("=============================================================\n");
    printf("  Max Concurrent Jobs  : %d\n", MAX_CONCURRENT);
    printf("  Max Retries Per Job  : %d\n", MAX_RETRIES);
    printf("  Total Pipeline Jobs  : %d\n", TOTAL_JOBS);
    printf("=============================================================\n\n");

    /* Step 1: Resolve execution order using topological sort on dependency graph */
    int order[MAX_NODES];
    int order_size = 0;
    graph_topo_sort(s->dep_graph, order, &order_size);

    printf("[GRAPH] Topological sort resolved execution order:\n");
    for (int i = 0; i < order_size; i++) {
        Job* j = list_find(s->job_store, order[i] + 1);
        if (j) printf("  Step %d -> Job[%d] %s\n", i + 1, j->id, j->name);
    }
    printf("\n");

    /* Step 2: Enqueue all jobs in dependency-safe order */
    for (int i = 0; i < order_size; i++) {
        Job* j = list_find(s->job_store, order[i] + 1);
        if (j) enqueue(s->job_queue, j);
    }

    printf("[QUEUE] All jobs enqueued. Execution begins now.\n\n");

    /* Step 3: Main execution loop with resource limiting and retry logic */
    int concurrent_count = 0;

    while (!queue_empty(s->job_queue)) {

        Job* job = dequeue(s->job_queue);
        if (!job) continue;

        /* Check if any dependency has permanently failed */
        int dep_result = check_deps(s, job->id);
        if (dep_result == -1) {
            job->status = FAILED;
            map_set(s->status_map, job->id, FAILED);
            log_msg(job->id, job->name,
                "SKIPPED - A required dependency permanently failed");
            continue;
        }

        /* Show resource limit batching */
        concurrent_count++;
        if (concurrent_count > MAX_CONCURRENT) {
            printf("\n  [RESOURCE MANAGER] Slot limit %d reached."
                   " Starting next batch:\n\n", MAX_CONCURRENT);
            concurrent_count = 1;
        }

        /* Mark job as RUNNING and update hash map */
        job->status = RUNNING;
        map_set(s->status_map, job->id, RUNNING);
        log_msg(job->id, job->name, "RUNNING");

        /* Execute with retry using the stack */
        int finished = 0;
        while (!finished) {
            int success = simulate_execution();

            if (success) {
                job->status = COMPLETED;
                map_set(s->status_map, job->id, COMPLETED);
                log_msg(job->id, job->name, "COMPLETED - Success");
                finished = 1;

            } else {
                job->retry_count++;

                if (job->retry_count <= job->max_retries) {
                    /* Push failed job onto retry stack */
                    job->status = FAILED;
                    map_set(s->status_map, job->id, FAILED);
                    char msg[80];
                    snprintf(msg, sizeof(msg),
                        "FAILED - Pushed to retry stack (attempt %d of %d)",
                        job->retry_count, job->max_retries);
                    log_msg(job->id, job->name, msg);
                    push(s->retry_stack, job);

                    /* Pop from retry stack and retry immediately */
                    Job* retry = pop(s->retry_stack);
                    if (retry) {
                        retry->status = RUNNING;
                        map_set(s->status_map, retry->id, RUNNING);
                        snprintf(msg, sizeof(msg),
                            "RETRYING now - attempt %d of %d",
                            retry->retry_count, retry->max_retries);
                        log_msg(retry->id, retry->name, msg);
                    }

                } else {
                    /* Retries exhausted - permanent failure */
                    job->status = FAILED;
                    map_set(s->status_map, job->id, FAILED);
                    log_msg(job->id, job->name,
                        "PERMANENTLY FAILED - max retries exhausted");
                    finished = 1;
                }
            }
        }
    }

    printf("\n=============================================================\n");
    printf("              Pipeline Execution Complete                     \n");
    printf("=============================================================\n");
}

void scheduler_report(Scheduler* s) {
    if (!s) return;
    printf("\n[FINAL STATUS REPORT - All Pipeline Jobs]\n");
    printf("-------------------------------------------------------------\n");
    for (int id = 1; id <= TOTAL_JOBS; id++) {
        Job* j = list_find(s->job_store, id);
        if (j) {
            j->status = map_get(s->status_map, id);
            print_job(j);
        }
    }
    printf("-------------------------------------------------------------\n");
}

void scheduler_free(Scheduler* s) {
    if (!s) return;
    list_free(s->job_store);
    queue_free(s->job_queue);
    stack_free(s->retry_stack);
    map_free(s->status_map);
    graph_free(s->dep_graph);
    free(s);
}