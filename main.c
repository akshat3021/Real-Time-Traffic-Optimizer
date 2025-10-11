#include <stdio.h>
#include "graph.h"

// Define the total number of locations (vertices) in your map
#define NUM_LOCATIONS 30

// A utility function to print the graph (add its declaration to graph.h)
void printGraph(Graph* graph);

int main() {
    printf("Attempting to load road network...\n");

    // Load the graph from map.txt using your function from Module 1
    Graph* dehradun_map = loadGraphFromFile("map.txt", NUM_LOCATIONS);

    if (dehradun_map == NULL) {
        fprintf(stderr, "Error: Failed to create the graph from file.\n");
        return 1; // Exit with an error code
    }

    // --- At this point, the graph is ready for Module 2 & 3 ---

    // Optional: Print the adjacency list to verify it's loaded correctly.
    // printGraph(dehradun_map);
    
    printf("\nThe graph data structure is ready for pathfinding.\n");

    // Clean up memory before exiting
    freeGraph(dehradun_map);

    return 0;
}