#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../graph.lib/graph.h"

static int bfs(const Graph *g, int src, int dst,
               int skip_u, int skip_v, int *prev) {
    int *dist = malloc(g->n * sizeof(int));
    int *queue = malloc(g->n * sizeof(int));

    for (int i = 0; i < g->n; i++) {
        dist[i] = -1;
        prev[i] = -1;
    }

    dist[src] = 0;
    int head = 0, tail = 0;
    queue[tail++] = src;

    while (head < tail) {
        int u = queue[head++];
        for (Edge *e = g->adj[u]; e; e = e->next) {
            int v = e->to;
            if ((u == skip_u && v == skip_v) ||
                (!g->directed && u == skip_v && v == skip_u))
                continue;
            if (dist[v] != -1)
                continue;
            dist[v] = dist[u] + 1;
            prev[v] = u;
            if (v == dst) {
                int d = dist[v];
                free(dist);
                free(queue);
                return d;
            }
            queue[tail++] = v;
        }
    }

    int d = dist[dst];
    free(dist);
    free(queue);
    return d;
}

int main(void) {
    Graph *g = graph_read_stream(stdin, FMT_EDGE_LIST);
    if (!g) {
        fprintf(stderr, "Failed to read graph\n");
        return 1;
    }

    int eu, ev;
    if (scanf("%d %d", &eu, &ev) != 2) {
        fprintf(stderr, "Failed to read query edge\n");
        graph_free(g);
        return 1;
    }

    if (!graph_has_edge(g, eu, ev)) {
        fprintf(stderr, "Edge (%d, %d) not in graph\n", eu, ev);
        graph_free(g);
        return 1;
    }

    int *prev = malloc(g->n * sizeof(int));

    int hops = bfs(g, ev, eu, eu, ev, prev);

    if (hops == -1) {
        printf("No cycle\n");
    } else {
        int cycle_len = hops + 1;
        printf("%d\n", cycle_len);

        int *path = malloc((hops + 1) * sizeof(int));
        int idx = 0;
        for (int v = eu; v != -1; v = prev[v])
            path[idx++] = v;

        for (int i = idx - 1; i >= 0; i--)
            printf("%d ", path[i]);

        free(path);
    }

    free(prev);
    graph_free(g);
    return 0;
}
