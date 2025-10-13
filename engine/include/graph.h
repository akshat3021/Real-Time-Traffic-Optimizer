#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>

typedef struct AdjListNode {
    int dest;
    int weight;
    struct AdjListNode* next;
} AdjListNode;

typedef struct AdjList {
    AdjListNode* head; 
} AdjList;

typedef struct Graph {
    int V;
    AdjList* array;
} Graph;

AdjListNode* newAdjListNode(int dest, int weight);

Graph* createGraph(int V);

void addEdge(Graph* graph, int src, int dest, int weight);

Graph* loadGraphFromFile(const char* filename, int numVertices);

void freeGraph(Graph* graph);

void printGraph(Graph* graph);

#endif // GRAPH_H