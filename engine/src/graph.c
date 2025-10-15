#include "graph.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LEN 100

// Count lines excluding the header line
int count_lines(FILE* fp) {
    int count = 0;
    char buffer[MAX_LINE_LEN];
    // Skip header
    if (fgets(buffer, MAX_LINE_LEN, fp) == NULL) {
        return 0; // empty file
    }

    while (fgets(buffer, MAX_LINE_LEN, fp) != NULL) {
        count++;
    }
    rewind(fp); // Reset file pointer to start
    return count;
}

Graph* load_graph(const char* junctions_file, const char* edges_file) {
    FILE* fp_junctions = fopen(junctions_file, "r");
    FILE* fp_edges = fopen(edges_file, "r");

    if (!fp_junctions || !fp_edges) {
        printf("Error: Could not open data files.\n");
        if (fp_junctions) fclose(fp_junctions);
        if (fp_edges) fclose(fp_edges);
        return NULL;
    }

    // Step 1: Read junctions and create nodes
    int num_nodes = count_lines(fp_junctions);
    if (num_nodes == 0) {
        printf("Error: Junctions file is empty or missing data.\n");
        fclose(fp_junctions);
        fclose(fp_edges);
        return NULL;
    }

    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (!graph) {
        printf("Error: Memory allocation failed for graph.\n");
        fclose(fp_junctions);
        fclose(fp_edges);
        return NULL;
    }
    graph->num_nodes = num_nodes;
    graph->nodes = (Node*)malloc(num_nodes * sizeof(Node));
    if (!graph->nodes) {
        printf("Error: Memory allocation failed for nodes.\n");
        free(graph);
        fclose(fp_junctions);
        fclose(fp_edges);
        return NULL;
    }

    char buffer[MAX_LINE_LEN];
    fgets(buffer, MAX_LINE_LEN, fp_junctions); // Skip header

    for (int i = 0; i < num_nodes; i++) {
        if (fscanf(fp_junctions, "%d,%lf,%lf", &graph->nodes[i].id, &graph->nodes[i].latitude, &graph->nodes[i].longitude) != 3) {
            printf("Error: Invalid format in junctions file at line %d\n", i + 2);
            free_graph(graph);
            fclose(fp_junctions);
            fclose(fp_edges);
            return NULL;
        }
        graph->nodes[i].head = NULL;
    }

    // Step 2: Read edges and build connections
    fgets(buffer, MAX_LINE_LEN, fp_edges); // Skip header

    int node1, node2;
    double weight;
    while (fscanf(fp_edges, "%d,%d,%lf", &node1, &node2, &weight) == 3) {
        // Convert IDs to zero-based indices
        int idx1 = node1 - 1;
        int idx2 = node2 - 1;

        // Check index validity
        if (idx1 < 0 || idx1 >= num_nodes || idx2 < 0 || idx2 >= num_nodes) {
            printf("Warning: Edge references invalid node ID (%d or %d), skipping.\n", node1, node2);
            continue;
        }

        // Add edge from node1 to node2
        Edge* edge1 = (Edge*)malloc(sizeof(Edge));
        if (!edge1) {
            printf("Error: Memory allocation failed for edge.\n");
            free_graph(graph);
            fclose(fp_junctions);
            fclose(fp_edges);
            return NULL;
        }
        edge1->destination = node2; // or idx2? Usually destination stores node ID
        edge1->weight = weight;
        edge1->next = graph->nodes[idx1].head;
        graph->nodes[idx1].head = edge1;

        // Add edge from node2 to node1 (bidirectional)
        Edge* edge2 = (Edge*)malloc(sizeof(Edge));
        if (!edge2) {
            printf("Error: Memory allocation failed for edge.\n");
            free_graph(graph);
            fclose(fp_junctions);
            fclose(fp_edges);
            return NULL;
        }
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

// Print graph adjacency list
void print_graph(Graph* graph) {
    if (!graph) {
        printf("Graph is NULL\n");
        return;
    }
    printf("\n--- Graph Adjacency List ---\n");
    for (int v = 0; v < graph->num_nodes; ++v) {
        Edge* edge = graph->nodes[v].head;
        printf("Junction [%d] (%.6f, %.6f):", graph->nodes[v].id, graph->nodes[v].latitude, graph->nodes[v].longitude);
        while (edge) {
            printf(" -> %d (weight: %.3f)", edge->destination, edge->weight);
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
