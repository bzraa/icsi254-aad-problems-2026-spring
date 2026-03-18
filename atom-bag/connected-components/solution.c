#include "../../graph-structure-c/DS.h"

void dfs(Graph *g, int x, int visited[]){
	if(visited[x]==0) {
		visited[x] = 1;
	}
	Elm *i;
	for (i = g->adj[x].head; i != NULL; i = i->next) {
		if(visited[i->x]==0) dfs(g, i->x, visited);
	}
}

int gr_connected_components(Graph *g)
{
	int visited[g->n+1];
	for (int i = 1; i < g->n; i++) visited[i] = 0;
	
	int cc_number=0;
	for (int i = 1; i < g->n+1; i++)
	{
		if(visited[i]==0) {
            cc_number++;
            dfs(g, i, visited);
        }
	}
	return cc_number;
}

int main()
{
	int n, m;
	scanf("%d%d", &n, &m);
	Graph g;
	gr_read(&g, n, m, 0, 0);

	int cc_number = gr_connected_components(&g);
    printf("Component number: %d\n", cc_number);

	return 0;
}