#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Edge {
    int destination;
    double weight;
    struct Edge* next;
} Edge;

typedef struct Node {
    int id;
    double latitude;
    double longitude;
    Edge* head; 
} Node;

typedef struct Graph {
    int num_nodes;
    Node* nodes; 
} Graph;

Graph* load_graph(const char* junctions_file, const char* edges_file);

void print_graph(Graph* graph);

void free_graph(Graph* graph);

#endif 