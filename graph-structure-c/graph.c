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
	int i, x, y;
	float w;
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


void gr_bfs(Graph *g, int s, int pi[], int distance[])
{
	int color[g->n+1];
	for (int i = 1; i <= g->n; i++) {
		color[i]=WHITE;
		pi[i]=-1;
		distance[i] = INT_MAX;
	}
	Queue *queue = (Queue *)malloc(sizeof(Queue));
	queue->head = queue->tail = NULL;
	queue->len = 0;
	q_push(queue, s);
	color[s]=GRAY;
	distance[s] = 0;
	while (queue->len>0)
	{
		int u = queue->head->x;
		q_pop(queue);
		Elm *i;
		for (i = g->adj[u].head; i != NULL; i = i->next) {
			if (color[i->x]==WHITE)
			{
				q_push(queue, i->x);
				color[i->x]=GRAY;
				pi[i->x]=u;
				distance[i->x]=distance[u] + 1;
			}
		}
		color[u] = BLACK;
	}
	free(queue);
}

void gr_print_path(Graph *g, int s, int v, int pi[]){
	if(s == v){
		printf("%d ", s);
	} else if(pi[v] == -1){
		printf("no path from %d to %d exists", s, v);
	} else{
		gr_print_path(g, s, pi[v], pi);
		printf("%d ", v);
	}
	printf("\n");
}

void gr_dfs(Graph *g, int distance[], int finish[], int pi[]){
	int color[g->n+1];
	for (int i = 1; i < g->n+1; i++)
	{
		color[i] = WHITE;
		pi[i] = -1;
	}
	int time = 0;
	for (int i = 1; i < g->n+1; i++)
	{
		if(color[i] == WHITE) gr_dfs_visit(g, i, color, distance, finish, pi, &time);
	}
}

void gr_dfs_visit(Graph *g, int x, int color[], int distance[], int finish[], int pi[], int *time){
	(*time)++;
	distance[x] = *time;
	color[x] = GRAY;
	Elm *curr = g->adj[x].head;
    while (curr) {
        if (color[curr->x] == WHITE){
			pi[curr->x] = x;
			gr_dfs_visit(g, curr->x, color, distance, finish, pi, time);
		}
        curr = curr->next;
    }
	color[x] = BLACK;
	(*time)++;
	finish[x] = *time;
}