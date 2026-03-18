#ifndef GRAPH_STRUCTURE_H
#define GRAPH_STRUCTURE_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define WHITE 0
#define GRAY 1
#define BLACK 2

struct Elm {
        int x;
        float weight;
        struct Elm *next;
};
typedef struct Elm Elm;

struct List {
        Elm *head;
        Elm *tail;
        int len;
};
typedef struct List List;

void l_push_back(List *, int, float);
void l_push_front(List *, int, float);
void l_insert(List *, int, float, int);
void l_pop_front(List *);
void l_pop_back(List *);
void l_erase(List *, int);
void l_print(List *);
Elm *l_search(List *, int);

struct Stack {
        Elm *top;
        int len;
};
typedef struct Stack Stack;

void s_push(Stack *, int );
void s_pop(Stack *);
void s_print(Stack *);

struct Queue {
        Elm *head;
        Elm *tail;
        int len;
};
typedef struct Queue Queue;

void q_push(Queue *, int);
void q_pop(Queue *);
void q_print(Queue *);

struct Graph {
	int n;     // Оройн тоо
	int m;     // Ирмэгүүдийн тоо
	List *adj;
};
typedef struct Graph Graph;

void gr_init_graph(Graph *, int);
void gr_add_edge(Graph *, int, int, float);
void gr_add_edge_undirected(Graph *, int, int, float);
void gr_read(Graph *, int, int, int, int);
void gr_bfs(Graph *, int, int[], int[]);
void gr_print_path(Graph *, int, int, int[]);
void gr_dfs(Graph *, int[], int[], int[]);
void gr_dfs_visit(Graph *, int, int[], int[], int[], int[], int *);

#endif
