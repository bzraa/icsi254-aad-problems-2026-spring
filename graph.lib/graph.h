#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ─── Types ──────────────────────────────────────────────────────────────── */

typedef struct Edge {
  int to;
  double weight;
  struct Edge *next;
} Edge;

typedef struct {
  int n; /* number of vertices            */
  int m; /* number of edges               */
  bool directed;
  bool weighted;
  Edge **adj; /* adj[v] -> linked list of edges */
} Graph;

/* ─── File format tags ───────────────────────────────────────────────────── */
/*
 * EDGE LIST  (.el / no header)
 *   First line:  <n> <m> [directed] [weighted]
 *   Then m lines: <u> <v> [weight]
 *
 * ADJACENCY LIST  (.al)
 *   First line:  <n> [directed] [weighted]
 *   Then n lines: <u> <v1>[,w1] <v2>[,w2] ...
 *
 * Auto-detected from file extension or explicit enum.
 */
typedef enum { FMT_AUTO, FMT_EDGE_LIST, FMT_ADJ_LIST } GraphFmt;

/* ─── API ────────────────────────────────────────────────────────────────── */

/* Construction / destruction */
Graph *graph_create(int n, bool directed, bool weighted);
void graph_free(Graph *g);

/* Edge manipulation */
void graph_add_edge(Graph *g, int u, int v, double w);
bool graph_has_edge(const Graph *g, int u, int v);
double graph_edge_weight(const Graph *g, int u, int v); /* NaN if absent */

/* I/O */
Graph *graph_read_file(const char *path, GraphFmt fmt);
Graph *graph_read_stream(FILE *fp, GraphFmt fmt);
void graph_write_file(const Graph *g, const char *path, GraphFmt fmt);
void graph_write_stream(const Graph *g, FILE *fp, GraphFmt fmt);

/* Utility */
void graph_print(const Graph *g);
int graph_degree(const Graph *g, int v); /* out-degree for directed */
int graph_in_degree(const Graph *g, int v);

#endif /* GRAPH_H */
