#include <stdio.h>
#include "graph.h"

#define NUM_LOCATIONS 30

void printGraph(Graph* graph);

int main() {
    printf("Attempting to load road network...\n");

    Graph* dehradun_map = loadGraphFromFile("map.txt", NUM_LOCATIONS);

    if (dehradun_map == NULL) {
        fprintf(stderr, "Error: Failed to create the graph from file.\n");
        return 1;
    }

    
    printf("\nThe graph data structure is ready for pathfinding.\n");

    freeGraph(dehradun_map);

    return 0;
}