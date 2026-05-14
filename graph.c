#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

Graph* create_graph(int num_nodes) {
    Graph* g = (Graph*)malloc(sizeof(Graph));
    if (!g) {
        printf("ERROR: malloc failed for Graph\n");
        exit(1);
    }
    g->num_nodes = num_nodes;
    for (int i = 0; i < num_nodes; i++)
        g->adj[i] = NULL;
    return g;
}

void graph_add_edge(Graph* g, int src, int dest) {
    if (!g) return;
    AdjNode* node = (AdjNode*)malloc(sizeof(AdjNode));
    if (!node) {
        printf("ERROR: malloc failed for AdjNode\n");
        exit(1);
    }
    node->dest  = dest;
    node->next  = g->adj[src];
    g->adj[src] = node;
}

static void dfs(Graph* g, int node, int* visited, int* result, int* idx) {
    visited[node] = 1;
    AdjNode* cur = g->adj[node];
    while (cur) {
        if (!visited[cur->dest])
            dfs(g, cur->dest, visited, result, idx);
        cur = cur->next;
    }
    result[(*idx)--] = node;
}

void graph_topo_sort(Graph* g, int* result, int* size) {
    if (!g) return;
    int visited[MAX_NODES] = {0};
    int idx = g->num_nodes - 1;
    *size   = g->num_nodes;
    for (int i = 0; i < g->num_nodes; i++) {
        if (!visited[i])
            dfs(g, i, visited, result, &idx);
    }
}

void graph_free(Graph* g) {
    if (!g) return;
    for (int i = 0; i < g->num_nodes; i++) {
        AdjNode* cur = g->adj[i];
        while (cur) {
            AdjNode* next = cur->next;
            free(cur);
            cur = next;
        }
    }
    free(g);
}