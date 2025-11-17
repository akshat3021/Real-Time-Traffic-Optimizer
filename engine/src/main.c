#include <stdio.h>
#include <stdlib.h>
#include "../include/graph.h"
#include "../include/algorithm.h"

// Placeholder for traffic simulation (Module 3's other responsibility)
void simulate_traffic(Graph* graph) {
    // In the future, this function can randomly increase the weight of some edges
    printf("Note: Traffic simulation not yet implemented.\n");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <source_id> <destination_id>\n", argv[0]);
        return 1;
    }

    int src_id = atoi(argv[1]);
    int dest_id = atoi(argv[2]);

    // This path is now correct because your files are in the engine folder
    Graph* graph = load_graph("engine/junctions_detailed_fixed_dehradun.csv", "engine/edges_detailed_fixed_dehradun.csv");
    if (graph == NULL) {
        return 1; // Error loading graph
    }

    // --- (Optional) Print graph for debugging ---
    // print_graph(graph);

    if (src_id < 0 || src_id >= graph->num_nodes || dest_id < 0 || dest_id >= graph->num_nodes) {
         printf("{\"error\":\"Invalid location ID (must be between 1 and %d).\",\"path\":[],\"total_weight\":-1}", graph->num_nodes);
         free_graph(graph);
         return 1;
    }

    // --- Step 3: Find shortest path (Module 2) ---
    // Note: We use ID - 1 because arrays are 0-indexed
    PathResult* result = dijkstra(graph, src_id - 1, dest_id - 1);

    // --- Step 4: Format and print the output for the website ---
    if (result != NULL && result->path != NULL) {
        printf("["); // Start of JSON array
        for (int i = 0; i < result->length; i++) {
            int node_index = result->path[i];
            printf("[%lf,%lf]", graph->nodes[node_index].latitude, graph->nodes[node_index].longitude);
            if (i < result->length - 1) {
                printf(","); // Comma between coordinate pairs
            }
        }
        printf("]"); // End of JSON array
    } else {
        printf("[]"); // Print empty array if no path is found
    }

    // --- Step 5: Clean up memory ---
    if (result != NULL) {
        free(result->path);
        free(result);
    }
    free_graph(graph);

    return 0;
}
