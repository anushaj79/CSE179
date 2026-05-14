#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>

#define V 100

int minKey(int key[], bool mstSet[]) {
    int min = INT_MAX, min_index;
    for (int v = 0; v < V; v++)
        if (mstSet[v] == false && key[v] < min)
            min = key[v], min_index = v;
    return min_index;
}

void primMST(int graph[V][V]) {
    int parent[V], key[V];
    bool mstSet[V];

    for (int i = 0; i < V; i++) {
        key[i] = INT_MAX;
        mstSet[i] = false;
    }

    key[0] = 0;
    parent[0] = -1;

    for (int count = 0; count < V - 1; count++) {
        int u = minKey(key, mstSet);
        mstSet[u] = true;

        for (int v = 0; v < V; v++)
            if (graph[u][v] && mstSet[v] == false && graph[u][v] < key[v])
                parent[v] = u, key[v] = graph[u][v];
    }

    int total_weight = 0;
    for (int i = 1; i < V; i++) total_weight += graph[i][parent[i]];
    printf("Serial Total MST Weight: %d\n", total_weight);
}

int main() {
    int graph[V][V];
    FILE *file = fopen("adj_matrix.txt", "r"); 
    if (!file) return 1;

    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            fscanf(file, "%d", &graph[i][j]);
    fclose(file);

    clock_t start = clock();
    primMST(graph);
    clock_t end = clock();
    
    printf("Serial Time: %f s\n", (double)(end - start) / CLOCKS_PER_SEC);
    return 0;
}