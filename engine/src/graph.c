#include "graph.h"
#include <string.h>

#define MAX_LINE_LEN 100

int count_lines(FILE* fp) {
    int count = 0;
    char buffer[MAX_LINE_LEN];
    fgets(buffer, MAX_LINE_LEN, fp); 
    while (fgets(buffer, MAX_LINE_LEN, fp) != NULL) {
        count++;
    }
    rewind(fp); 
    return count;
}

Graph* load_graph(const char* junctions_file, const char* edges_file) {
    FILE* fp_junctions = fopen(junctions_file, "r");
    FILE* fp_edges = fopen(edges_file, "r");

    if (!fp_junctions || !fp_edges) {
        printf("Error: Could not open data files.\n");
        return NULL;
    }

    // --- Step 1: Read junctions and create nodes ---
    int num_nodes = count_lines(fp_junctions);
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->num_nodes = num_nodes;
    graph->nodes = (Node*)malloc(num_nodes * sizeof(Node));
    
    char buffer[MAX_LINE_LEN];
    fgets(buffer, MAX_LINE_LEN, fp_junctions); // Skip header

    for (int i = 0; i < num_nodes; i++) {
        fscanf(fp_junctions, "%d,%lf,%lf", &graph->nodes[i].id, &graph->nodes[i].latitude, &graph->nodes[i].longitude);
        graph->nodes[i].head = NULL;
    }

    // --- Step 2: Read edges and build connections ---
    fgets(buffer, MAX_LINE_LEN, fp_edges); // Skip header

    int node1, node2;
    double weight;
    while (fscanf(fp_edges, "%d,%d,%lf", &node1, &node2, &weight) == 3) {
        // Find the source and destination nodes in our array (assuming IDs are 1-based)
        int idx1 = node1 - 1;
        int idx2 = node2 - 1;

        // Add edge from node1 to node2 (like Mohit's code)
        Edge* edge1 = (Edge*)malloc(sizeof(Edge));
        edge1->destination = node2;
        edge1->weight = weight;
        edge1->next = graph->nodes[idx1].head;
        graph->nodes[idx1].head = edge1;

        // Add edge from node2 to node1 to make it bidirectional
        Edge* edge2 = (Edge*)malloc(sizeof(Edge));
        edge2->destination = node1;
        edge2->weight = weight;
        edge2->next = graph->nodes[idx2].head;
        graph->nodes[idx2].head = edge2;
    }

    fclose(fp_junctions);
    fclose(fp_edges);
    printf("Graph loaded successfully with %d junctions.\n", num_nodes);
    return graph;
}

// Mohit's printGraph function, slightly adapted
void print_graph(Graph* graph) {
    printf("\n--- Graph Adjacency List ---\n");
    for (int v = 0; v < graph->num_nodes; ++v) {
        Edge* edge = graph->nodes[v].head;
        printf("Junction [%d] (%.4f, %.4f):", graph->nodes[v].id, graph->nodes[v].latitude, graph->nodes[v].longitude);
        while (edge) {
            printf(" -> %d (time: %.1f)", edge->destination, edge->weight);
            edge = edge->next;
        }
        printf("\n");
    }
    printf("--------------------------\n");
}

void free_graph(Graph* graph) {
    if (!graph) return;
    for (int i = 0; i < graph->num_nodes; ++i) {
        Edge* current = graph->nodes[i].head;
        while (current != NULL) {
            Edge* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(graph->nodes);
    free(graph);
    printf("Graph memory freed successfully.\n");
}