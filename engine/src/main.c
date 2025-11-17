#include <stdio.h>
#include <stdlib.h>
#include "../include/graph.h"
#include "../include/algorithm.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_id> <destination_id>\n", argv[0]);
        return 1;
    }

    int src_id = atoi(argv[1]);
    int dest_id = atoi(argv[2]);

    // This path is now correct because your files are in the engine folder
    Graph* graph = load_graph("engine/junctions_detailed_fixed_dehradun.csv", "engine/edges_detailed_fixed_dehradun.csv");
    if (graph == NULL) {
        printf("{\"error\":\"Failed to load graph data.\",\"path\":[],\"total_weight\":-1}");
        return 1;
    }

    int src_idx = src_id - 1;
    int dest_idx = dest_id - 1;

    if (src_idx < 0 || src_idx >= graph->num_nodes || dest_idx < 0 || dest_idx >= graph->num_nodes) {
         printf("{\"error\":\"Invalid location ID (must be between 1 and %d).\",\"path\":[],\"total_weight\":-1}", graph->num_nodes);
         free_graph(graph);
         return 1;
    }

    PathResult* result = dijkstra(graph, src_idx, dest_idx);
    
    if (result != NULL && result->path != NULL && result->length > 0) {
        printf("{\"path\":[");
        for (int i = 0; i < result->length; i++) {
            int node_idx = result->path[i]; 
            printf("[%.6f,%.6f]", graph->nodes[node_idx].latitude, graph->nodes[node_idx].longitude);
            if (i < result->length - 1) printf(",");
        }
        printf("],\"total_weight\":%.2f}", result->total_weight);
    } else {
        printf("{\"error\":\"No route could be found between these locations.\",\"path\":[],\"total_weight\":-1}");
    }

    if (result) {
        if (result->path) free(result->path);
        free(result);
    }
    free_graph(graph);

    return 0;
}