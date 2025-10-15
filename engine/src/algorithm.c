#include "algorithm.h"
#include <limits.h>

// --- Priority Queue (Min-Heap) Functions ---

MinHeapNode* newMinHeapNode(int v, double dist) {
    MinHeapNode* minHeapNode = (MinHeapNode*)malloc(sizeof(MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

MinHeap* createMinHeap(int capacity) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->pos = (int*)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (MinHeapNode**)malloc(capacity * sizeof(MinHeapNode*));
    return minHeap;
}

void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b) {
    MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(MinHeap* minHeap, int idx) {
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->dist < minHeap->array[smallest]->dist)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->dist < minHeap->array[smallest]->dist)
        smallest = right;

    if (smallest != idx) {
        MinHeapNode* smallestNode = minHeap->array[smallest];
        MinHeapNode* idxNode = minHeap->array[idx];
        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

int isEmpty(MinHeap* minHeap) {
    return minHeap->size == 0;
}

MinHeapNode* extractMin(MinHeap* minHeap) {
    if (isEmpty(minHeap)) return NULL;
    MinHeapNode* root = minHeap->array[0];
    MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;
    minHeap->pos[root->v] = minHeap->size - 1;
    minHeap->pos[lastNode->v] = 0;
    --minHeap->size;
    minHeapify(minHeap, 0);
    return root;
}

void decreaseKey(MinHeap* minHeap, int v, double dist) {
    int i = minHeap->pos[v];
    minHeap->array[i]->dist = dist;
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist) {
        minHeap->pos[minHeap->array[i]->v] = (i - 1) / 2;
        minHeap->pos[minHeap->array[(i - 1) / 2]->v] = i;
        swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

int isInMinHeap(MinHeap* minHeap, int v) {
    if (minHeap->pos[v] < minHeap->size) return 1;
    return 0;
}

// --- Dijkstra's Algorithm ---

PathResult* dijkstra(Graph* graph, int src, int dest) {
    int V = graph->num_nodes;
    double dist[V];
    int parent[V];
    MinHeap* minHeap = createMinHeap(V);

    // Initialize distances and parent array
    for (int v = 0; v < V; ++v) {
        dist[v] = INT_MAX;
        parent[v] = -1;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }

    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src]);
    minHeap->size = V;

    // Dijkstra's algorithm loop
    while (!isEmpty(minHeap)) {
        MinHeapNode* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v;

        Edge* pCrawl = graph->nodes[u].head;
        while (pCrawl != NULL) {
            int v_idx = pCrawl->destination - 1;
            if (isInMinHeap(minHeap, v_idx) && dist[u] != INT_MAX && pCrawl->weight + dist[u] < dist[v_idx]) {
                dist[v_idx] = dist[u] + pCrawl->weight;
                parent[v_idx] = u;
                decreaseKey(minHeap, v_idx, dist[v_idx]);
            }
            pCrawl = pCrawl->next;
        }
    }

    // Debug: Print the distances and parent array to check if the path was found
    printf("\n--- Dijkstra's Result ---\n");
    for (int i = 0; i < V; ++i) {
        printf("Node %d: Distance = %.2f, Parent = %d\n", i + 1, dist[i], parent[i]);
    }

    // If destination is not reachable, return an empty path
    PathResult* result = (PathResult*)malloc(sizeof(PathResult));
    if (dist[dest] == INT_MAX) {
        result->path = NULL;
        result->length = 0;
        result->total_weight = -1;
        return result;
    }

    // Reconstruct the path from destination to source
    int path_len = 0;
    int temp_path[V];
    int current = dest;
    while (current != -1) {
        temp_path[path_len++] = current;
        current = parent[current];
    }

    result->path = (int*)malloc(path_len * sizeof(int));
    result->length = path_len;
    result->total_weight = dist[dest];
    for (int i = 0; i < path_len; i++) {
        result->path[i] = temp_path[path_len - 1 - i];
    }

    free(minHeap->pos);
    for (int i = 0; i < V; ++i) free(minHeap->array[i]);
    free(minHeap->array);
    free(minHeap);

    return result;
}
