#include "DS.h"

void gr_init_graph(Graph *g, int n)
{
	int i;
	g->adj = (List *) malloc(sizeof(List) * (n + 1));
	g->n = n;
	for (i = 0; i <= n; i++) {
		g->adj[i].head = g->adj[i].tail = NULL;
		g->adj[i].len = 0;
	}
}

void gr_add_edge(Graph *g, int x, int y, float weight)
{
	l_push_back(&g->adj[x], y, weight);
}

void gr_add_edge_undirected(Graph *g, int x, int y, float weight)
{
	l_push_back(&g->adj[x], y, weight);
	l_push_back(&g->adj[y], x, weight);
}

void gr_read(Graph *g, int n, int m, int isDirected, int hasWeight){
	int i, x, y, w;
	gr_init_graph(g, n);
	if(hasWeight){
		for (i = 0; i < m; i++) {
			scanf("%d%d%f", &x, &y, &w);
			if(isDirected) gr_add_edge(g, x, y, w);
			else gr_add_edge_undirected(g, x, y, w);
		}
	}
	else {
		for (i = 0; i < m; i++) {
			scanf("%d%d", &x, &y);
			if(isDirected) gr_add_edge(g, x, y, 1);
			else gr_add_edge_undirected(g, x, y, 1);
		}
	}
}