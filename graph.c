#include "graph.h"
#include <string.h>

AdjListNode* newAdjListNode(int dest, int weight) {
    AdjListNode* newNode = (AdjListNode*)malloc(sizeof(AdjListNode));
    if (!newNode) {
        perror("Memory allocation failed for AdjListNode");
        exit(EXIT_FAILURE);
    }
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

Graph* createGraph(int V) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (!graph) {
        perror("Memory allocation failed for Graph");
        exit(EXIT_FAILURE);
    }
    graph->V = V;

    graph->array = (AdjList*)malloc(V * sizeof(AdjList));
    if (!graph->array) {
        perror("Memory allocation failed for adjacency list array");
        free(graph);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < V; ++i) {
        graph->array[i].head = NULL;
    }

    return graph;
}

void addEdge(Graph* graph, int src, int dest, int weight) {
    AdjListNode* newNode = newAdjListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
}

Graph* loadGraphFromFile(const char* filename, int numVertices) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    Graph* graph = createGraph(numVertices);
    char line[100];
    int src, dest, weight;

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n' || line[0] == '#') {
            continue;
        }

        if (sscanf(line, "%d %d %d", &src, &dest, &weight) == 3) {
            if (src >= numVertices || dest >= numVertices || src < 0 || dest < 0) {
                fprintf(stderr, "Warning: Vertex out of bounds in line: %s", line);
                continue;
            }
            addEdge(graph, src, dest, weight);
            addEdge(graph, dest, src, weight);
        }
    }

    fclose(file);
    printf("Graph loaded successfully from %s.\n", filename);
    return graph;
}
void printGraph(Graph* graph) {
    printf("\n--- Graph Adjacency List ---\n");
    for (int v = 0; v < graph->V; ++v) {
        AdjListNode* node = graph->array[v].head;
        printf("Location [%d]:", v);
        if (!node) {
            printf(" (No connections)\n");
        } else {
            while (node) {
                printf(" -> %d (time: %d mins)", node->dest, node->weight);
                node = node->next;
            }
            printf("\n");
        }
    }
    printf("--------------------------\n");
}

void freeGraph(Graph* graph) {
    if (!graph) return;

    for (int i = 0; i < graph->V; ++i) {
        AdjListNode* current = graph->array[i].head;
        while (current != NULL) {
            AdjListNode* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(graph->array);
    free(graph);
    printf("Graph memory freed successfully.\n");
}