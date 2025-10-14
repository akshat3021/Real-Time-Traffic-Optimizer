#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "graph.h"

// Structure for a node in the priority queue (min-heap)
typedef struct MinHeapNode {
    int v;      // Vertex number (index)
    double dist; // Distance from the source
} MinHeapNode;

// Structure for the priority queue itself
typedef struct MinHeap {
    int size;      // Number of nodes currently in the heap
    int capacity;  // Maximum capacity of the heap
    int* pos;      // To help with decreasing key values
    MinHeapNode** array;
} MinHeap;

// Structure to hold the final calculated path
typedef struct PathResult {
    int* path;
    int length;
    double total_weight;
} PathResult;

// Function declaration for Dijkstra's algorithm
// This is the main function that algorithm.c will provide.
PathResult* dijkstra(Graph* graph, int src, int dest);

#endif //ALGORITHM_H