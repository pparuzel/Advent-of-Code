#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_NODES 1024
#define NODE_LEN 3

// A structure to represent the graph using an adjacency matrix
typedef struct {
    bool adj[MAX_NODES][MAX_NODES];
    // Keeps track of the number of neighbors for each node
    int size[MAX_NODES];
} Graph;

void init_graph(Graph *graph);
void add_edge(Graph *graph, int u, int v);
bool find_clique(Graph *graph, int *nodes, int count, int size, int *clique);

// Utility to find the index of a node by its name
int find_node_index(char *node_name,
                   char node_names[MAX_NODES][NODE_LEN],
                   int *node_count);

int main(int argc, char **argv) {
    char buffer[256];
    Graph graph;
    char node_names[MAX_NODES][NODE_LEN];
    int node_count = 0;

    init_graph(&graph);

    FILE* f = stdin;
    if (argc > 1) {
        f = fopen(argv[1], "r");
    }
    while (fgets(buffer, sizeof(buffer), f)) {
        char a[NODE_LEN], b[NODE_LEN];
        sscanf(buffer, "%2[^-]-%2s", a, b);

        int u = find_node_index(a, node_names, &node_count);
        int v = find_node_index(b, node_names, &node_count);
        add_edge(&graph, u, v);
    }

    // Find the maximum degree (most edges)
    int most_edges = 0;
    for (int i = 0; i < node_count; i++) {
        if (graph.size[i] > most_edges) {
            most_edges = graph.size[i];
        }
    }
    printf("At most there is %d edges which means that the largest "
           "fully-connected cycle can be of size %d\n",
           most_edges, most_edges + 1);

    // Find the maximum clique
    int max_clique_size = most_edges + 1;
    int nodes[MAX_NODES];
    int clique[MAX_NODES];
    for (int i = 0; i < node_count; i++) {
        nodes[i] = i;
    }

    for (int clique_size = max_clique_size; clique_size > 0; --clique_size) {
        if (find_clique(&graph, nodes, node_count, clique_size, clique)) {
            const char* last = node_names[clique[clique_size - 1]];
            printf("Maximum clique of size %d: ['", clique_size);
            for (int i = 0; i < clique_size - 1; i++) {
                printf("%s', '", node_names[clique[i]]);
            }
            printf("%s']\n", last);
            printf("The password is\n\t");
            for (int i = 0; i < clique_size - 1; i++) {
                printf("%s,", node_names[clique[i]]);
            }
            printf("%s\n", last);
            break;
        }
        printf("No clique of size %d found\n", clique_size);
    }

    return EXIT_SUCCESS;
}

void init_graph(Graph *graph) {
    memset(graph->adj, 0, sizeof(graph->adj));
    memset(graph->size, 0, sizeof(graph->size));
}

void add_edge(Graph *graph, int u, int v) {
    if (graph->adj[u][v]) {
        return;
    }
    graph->adj[u][v] = true;
    graph->adj[v][u] = true;
    graph->size[u]++;
    graph->size[v]++;
}

int find_node_index(char *node_name, char node_names[MAX_NODES][NODE_LEN],
                    int *node_count) {
    for (int i = 0; i < *node_count; i++) {
        if (strcmp(node_names[i], node_name) == 0) {
            return i;
        }
    }
    if (*node_count >= MAX_NODES) {
        printf("error: index out of range\ntip: Increase the MAX_NODES value\n");
        exit(254);
    }
    strcpy(node_names[*node_count], node_name);
    return (*node_count)++;
}

bool find_clique(Graph *graph, int *nodes, int node_count, int size, int *clique) {
    if (size == 0) {
        return true;
    }

    for (int i = 0; i < node_count; i++) {
        int current_node = nodes[i];
        int subgraph[MAX_NODES];
        int subgraph_size = 0;

        for (int j = 0; j < node_count; j++) {
            if (graph->adj[current_node][nodes[j]]) {
                subgraph[subgraph_size++] = nodes[j];
            }
        }

        if (subgraph_size < size - 1) {
            continue;
        }

        if (find_clique(graph, subgraph, subgraph_size, size - 1, clique)) {
            clique[size - 1] = current_node;
            return true;
        }
    }

    return false;
}
