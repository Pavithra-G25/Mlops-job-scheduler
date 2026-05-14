#ifndef GRAPH_H
#define GRAPH_H

#define MAX_NODES 10

typedef struct AdjNode {
    int             dest;
    struct AdjNode* next;
} AdjNode;

typedef struct {
    AdjNode* adj[MAX_NODES];
    int      num_nodes;
} Graph;

Graph* create_graph(int num_nodes);
void   graph_add_edge(Graph* g, int src, int dest);
void   graph_topo_sort(Graph* g, int* result, int* size);
void   graph_free(Graph* g);

#endif