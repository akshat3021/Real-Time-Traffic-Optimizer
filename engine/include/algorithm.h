#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "graph.h"

typedef struct MinHeapNode {
    int v;
    double dist;
} MinHeapNode;

typedef struct MinHeap {
    int size;
    int capacity;
    int* pos;
    MinHeapNode** array;
} MinHeap;

typedef struct PathResult {
    int* path;
    int length;
    double total_weight;
} PathResult;

PathResult* dijkstra(Graph* graph, int src, int dest);

#endif