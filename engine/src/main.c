#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "algorithm.h"

// Placeholder for traffic simulation (you can extend this later)
void simulate_traffic(Graph* graph) {
    // For now, no traffic updates, just a placeholder
    printf("Note: Traffic simulation not yet implemented.\n");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_id> <destination_id>\n", argv[0]);
        return 1;
    }

    int src_id = atoi(argv[1]);
    int dest_id = atoi(argv[2]);

    if (src_id <= 0 || dest_id <= 0) {
        fprintf(stderr, "Source and destination IDs must be positive integers.\n");
        return 1;
    }

    // Load graph (Module 1)
    Graph* graph = load_graph("junctions.csv", "edges.csv");
    if (graph == NULL) {
        fprintf(stderr, "Error loading graph data.\n");
        return 1;
    }

    printf("Graph loaded successfully with %d junctions.\n", graph->num_nodes);

    // Print the graph adjacency for debugging
    print_graph(graph);

    // Simulate traffic (update edge weights if needed)
    simulate_traffic(graph);

    // Find shortest path (Module 2)
    PathResult* result = dijkstra(graph, src_id - 1, dest_id - 1);

    // Debug: Check if the path result is valid
    if (result == NULL) {
        fprintf(stderr, "Error: Dijkstra algorithm failed.\n");
        free_graph(graph);
        return 1;
    }

    // Debug: Print the path length and total weight
    printf("Path length: %d\n", result->length);
    printf("Total weight: %.2f\n", result->total_weight);

    // Print path as JSON array of coordinates or show message if no path found
    if (result->path != NULL && result->length > 0) {
        printf("[");
        for (int i = 0; i < result->length; i++) {
            int node_idx = result->path[i];
            printf("[%.6f,%.6f]", graph->nodes[node_idx].latitude, graph->nodes[node_idx].longitude);
            if (i < result->length - 1) printf(",");
        }
        printf("]\n");
    } else {
        printf("[]\n");
    }

    // Free resources
    if (result->path) free(result->path);
    free(result);
    free_graph(graph);

    return 0;
}
