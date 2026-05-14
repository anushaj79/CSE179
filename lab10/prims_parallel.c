#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <omp.h>

#define V 100 

void primMST(int graph[V][V]) {
    int parent[V], d[V];
    bool mstSet[V];

    for (int i = 0; i < V; i++) {
        d[i] = INT_MAX;
        mstSet[i] = false;
    }

    d[0] = 0; 
    parent[0] = -1;

    for (int count = 0; count < V - 1; count++) {
        int u = -1;
        int min = INT_MAX;

        // finding globally closest node using reduction 
        #pragma omp parallel
        {
            int local_min = INT_MAX;
            int local_u = -1;

            #pragma omp for nowait
            for (int v = 0; v < V; v++) {
                if (!mstSet[v] && d[v] < local_min) {
                    local_min = d[v];
                    local_u = v;
                }
            }

            // global reduction to find the min d[u] atomically
            #pragma omp critical
            {
                if (local_min < min) {
                    min = local_min;
                    u = local_u;
                }
            }
        }

        if (u == -1) break;
        mstSet[u] = true; // add u to VT

        // each process updates its part of d vector locally 
        #pragma omp parallel for
        for (int v = 0; v < V; v++) {
            // d[v] := min{d[v], w(u,v)}
            if (graph[u][v] && !mstSet[v] && graph[u][v] < d[v]) {
                parent[v] = u;
                d[v] = graph[u][v];
            }
        }
    }

    // results for report verification
    int total_weight = 0;
    for (int i = 1; i < V; i++) total_weight += graph[i][parent[i]];
    printf("Parallel Total MST Weight: %d\n", total_weight);
}

int main() {
    int graph[V][V];
    FILE *file = fopen("adj_matrix.txt", "r");
    if (!file) return 1;

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