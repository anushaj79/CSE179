#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <omp.h>

#define V 100 // 

int minKey(int key[], bool mstSet[]) {
    int min = INT_MAX, min_index = -1;

    #pragma omp parallel
    {
        int local_min = INT_MAX, local_index = -1;
        #pragma omp for nowait // each thread finds local min
        for (int v = 0; v < V; v++) {
            if (mstSet[v] == false && key[v] < local_min) {
                local_min = key[v];
                local_index = v;
            }
        }
        #pragma omp critical // ensure thread-safe update to global min
        {
            if (local_min < min) {
                min = local_min;
                min_index = local_index;
            }
        }
    }
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
        if (u == -1) break; 
        mstSet[u] = true;

        #pragma omp parallel for // update key values in parallel
        for (int v = 0; v < V; v++) {
            if (graph[u][v] && mstSet[v] == false && graph[u][v] < key[v]) {
                parent[v] = u;
                key[v] = graph[u][v];
            }
        }
    }

    int total_weight = 0;
    for (int i = 1; i < V; i++) total_weight += graph[i][parent[i]];
    printf("Parallel Total MST Weight: %d\n", total_weight);
}

int main() {
    int graph[V][V];
    FILE *file = fopen("adj_matrix.txt", "r");
    if (!file) { printf("Error opening file!\n"); return 1; }

    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            fscanf(file, "%d", &graph[i][j]);
    fclose(file);

    double start = omp_get_wtime();
    primMST(graph);
    double end = omp_get_wtime();
    
    printf("Parallel Time: %f s\n", end - start);
    return 0;
}