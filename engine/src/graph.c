#include "graph.h"
#include <string.h>

#define MAX_LINE_LEN 100

int count_lines(FILE* fp) {
    int count = 0;
    char buffer[MAX_LINE_LEN];
    if (fgets(buffer, MAX_LINE_LEN, fp) == NULL) return 0;
    while (fgets(buffer, MAX_LINE_LEN, fp) != NULL) count++;
    rewind(fp);
    return count;
}

Graph* load_graph(const char* junctions_file, const char* edges_file) {
    FILE* fp_junctions = fopen(junctions_file, "r");
    FILE* fp_edges = fopen(edges_file, "r");

    if (!fp_junctions || !fp_edges) {
        fprintf(stderr, "Error: Could not open data files.\n");
        if (fp_junctions) fclose(fp_junctions);
        if (fp_edges) fclose(fp_edges);
        return NULL;
    }

    int num_nodes = count_lines(fp_junctions);
    if (num_nodes == 0) {
        fprintf(stderr, "Error: Junctions file is empty.\n");
        fclose(fp_junctions);
        fclose(fp_edges);
        return NULL;
    }

    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->num_nodes = num_nodes;
    graph->nodes = (Node*)malloc(num_nodes * sizeof(Node));

    char buffer[MAX_LINE_LEN];
    fgets(buffer, MAX_LINE_LEN, fp_junctions); // Skip header

    for (int i = 0; i < num_nodes; i++) {
        fscanf(fp_junctions, "%d,%lf,%lf", &graph->nodes[i].id, &graph->nodes[i].latitude, &graph->nodes[i].longitude);
        graph->nodes[i].head = NULL;
    }

    fgets(buffer, MAX_LINE_LEN, fp_edges); // Skip header

    int node1, node2;
    double weight;
    while (fscanf(fp_edges, "%d,%d,%lf", &node1, &node2, &weight) == 3) {
        int idx1 = node1 - 1;
        int idx2 = node2 - 1;
        if (idx1 < 0 || idx1 >= num_nodes || idx2 < 0 || idx2 >= num_nodes) continue;

        Edge* edge1 = (Edge*)malloc(sizeof(Edge));
        edge1->destination = node2;
        edge1->weight = weight;
        edge1->next = graph->nodes[idx1].head;
        graph->nodes[idx1].head = edge1;

        Edge* edge2 = (Edge*)malloc(sizeof(Edge));
        edge2->destination = node1;
        edge2->weight = weight;
        edge2->next = graph->nodes[idx2].head;
        graph->nodes[idx2].head = edge2;
    }

    fclose(fp_junctions);
    fclose(fp_edges);
    return graph;
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
}