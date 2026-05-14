#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "scheduler.h"

int main(void) {
    srand((unsigned int)time(NULL));

    printf("\n");
    printf("=============================================================\n");
    printf("   MLOps Model Job Queue and Resource Scheduler              \n");
    printf("   Production ML Pipeline Simulation - Built in C            \n");
    printf("=============================================================\n\n");

    /* Create the scheduler */
    Scheduler* s = create_scheduler();

    /*
     * Define pipeline dependencies using 0-based graph indices.
     * Job IDs are 1-based. Graph indices are 0-based.
     * Edge (A -> B) means: Job A must COMPLETE before Job B can start.
     *
     * 0 = Data Ingestion
     * 1 = Data Preprocessing
     * 2 = Data Validation
     * 3 = Model Training
     * 4 = Model Evaluation
     * 5 = Model Registration
     * 6 = Model Deployment
     * 7 = Monitoring
     */
    graph_add_edge(s->dep_graph, 0, 1);
    graph_add_edge(s->dep_graph, 1, 2);
    graph_add_edge(s->dep_graph, 2, 3);
    graph_add_edge(s->dep_graph, 3, 4);
    graph_add_edge(s->dep_graph, 4, 5);
    graph_add_edge(s->dep_graph, 5, 6);
    graph_add_edge(s->dep_graph, 6, 7);

    /* Submit all 8 ML pipeline jobs to the scheduler */
    printf("[INIT] Submitting ML pipeline jobs...\n\n");

    scheduler_add_job(s, create_job(1, "Data Ingestion",     2, MAX_RETRIES));
    scheduler_add_job(s, create_job(2, "Data Preprocessing", 2, MAX_RETRIES));
    scheduler_add_job(s, create_job(3, "Data Validation",    2, MAX_RETRIES));
    scheduler_add_job(s, create_job(4, "Model Training",     1, MAX_RETRIES));
    scheduler_add_job(s, create_job(5, "Model Evaluation",   1, MAX_RETRIES));
    scheduler_add_job(s, create_job(6, "Model Registration", 3, MAX_RETRIES));
    scheduler_add_job(s, create_job(7, "Model Deployment",   1, MAX_RETRIES));
    scheduler_add_job(s, create_job(8, "Monitoring",         3, MAX_RETRIES));

    /* Start the pipeline */
    scheduler_run(s);

    /* Print final status of all jobs */
    scheduler_report(s);

    /* Free all allocated memory */
    scheduler_free(s);

    printf("\n[DONE] All memory freed. Scheduler exited cleanly.\n\n");
    return 0;
}