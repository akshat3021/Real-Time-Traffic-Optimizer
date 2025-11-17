#include "../include/graph.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_LINE_LEN 512 // Increased buffer size for long place names

int count_lines(FILE* fp) {
    int count = 0;
    char buffer[MAX_LINE_LEN];
    if (fgets(buffer, MAX_LINE_LEN, fp) == NULL) return 0; // Skip header
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
        fprintf(stderr, "Error: Could not open data files.\n");
        if (fp_junctions) fclose(fp_junctions);
        if (fp_edges) fclose(fp_edges);
        return NULL;
    }

    int num_nodes = count_lines(fp_junctions);
    if (num_nodes == 0) {
        fprintf(stderr, "Error: Junctions file is empty.\n");
        return NULL;
    }

    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->num_nodes = num_nodes;
    graph->nodes = (Node*)malloc(num_nodes * sizeof(Node));
    
    char buffer[MAX_LINE_LEN];
    fgets(buffer, MAX_LINE_LEN, fp_junctions); // Skip header

    for (int i = 0; i < num_nodes; i++) {
        if (fgets(buffer, MAX_LINE_LEN, fp_junctions) == NULL) {
             fprintf(stderr, "Error reading junction file at line %d\n", i+2);
             free_graph(graph);
             return NULL;
        }
        
        char* token;
        token = strtok(buffer, ",");
        if (token) graph->nodes[i].id = atoi(token);
        
        token = strtok(NULL, ","); // Skip place_name
        
        token = strtok(NULL, ",");
        if (token) graph->nodes[i].latitude = atof(token);
        
        token = strtok(NULL, "\n");
        if (token) graph->nodes[i].longitude = atof(token);
        
        graph->nodes[i].head = NULL;
    }
    fclose(fp_junctions);

    fgets(buffer, MAX_LINE_LEN, fp_edges); // Skip header

    int from_id, to_id;
    double travel_time;
    
    while (fgets(buffer, MAX_LINE_LEN, fp_edges) != NULL) {
        char* token;
        token = strtok(buffer, ",");
        if (token) from_id = atoi(token);
        
        token = strtok(NULL, ",");
        if (token) to_id = atoi(token);
        
        token = strtok(NULL, ","); // Skip distance_km
        
        token = strtok(NULL, "\n");
        if (token) travel_time = atof(token);
        
        int from_idx = from_id - 1; 
        int to_idx = to_id - 1;

        if (from_idx >= 0 && from_idx < num_nodes && to_idx >= 0 && to_idx < num_nodes) {
            Edge* edge1 = (Edge*)malloc(sizeof(Edge));
            edge1->destination = to_id; 
            edge1->weight = travel_time; 
            edge1->next = graph->nodes[from_idx].head;
            graph->nodes[from_idx].head = edge1;

            Edge* edge2 = (Edge*)malloc(sizeof(Edge));
            edge2->destination = from_id; 
            edge2->weight = travel_time;
            edge2->next = graph->nodes[to_idx].head;
            graph->nodes[to_idx].head = edge2;
        }
    }
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