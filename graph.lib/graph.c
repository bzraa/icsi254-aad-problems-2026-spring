#include "graph.h"
#include <ctype.h>
#include <math.h> /* NAN, isnan */

/* ─── Internal helpers ───────────────────────────────────────────────────── */

static Edge *edge_new(int to, double w) {
  Edge *e = malloc(sizeof *e);
  if (!e) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }
  e->to = to;
  e->weight = w;
  e->next = NULL;
  return e;
}

static void adj_push(Graph *g, int u, int v, double w) {
  Edge *e = edge_new(v, w);
  e->next = g->adj[u];
  g->adj[u] = e;
}

static GraphFmt detect_fmt(const char *path) {
  const char *dot = strrchr(path, '.');
  if (!dot)
    return FMT_EDGE_LIST;
  if (strcmp(dot, ".al") == 0)
    return FMT_ADJ_LIST;
  return FMT_EDGE_LIST; /* .el, .txt, .graph, … */
}

/* Skip blank lines and # comments */
static bool next_line(FILE *fp, char *buf, int sz) {
  while (fgets(buf, sz, fp)) {
    char *p = buf;
    while (isspace((unsigned char)*p))
      p++;
    if (*p && *p != '#') {
      memmove(buf, p, strlen(p) + 1);
      return true;
    }
  }
  return false;
}

/* ─── Construction / destruction ─────────────────────────────────────────── */

Graph *graph_create(int n, bool directed, bool weighted) {
  Graph *g = malloc(sizeof *g);
  g->n = n;
  g->m = 0;
  g->directed = directed;
  g->weighted = weighted;
  g->adj = calloc(n, sizeof(Edge *));
  if (!g->adj) {
    perror("calloc");
    exit(EXIT_FAILURE);
  }
  return g;
}

void graph_free(Graph *g) {
  if (!g)
    return;
  for (int i = 0; i < g->n; i++) {
    Edge *e = g->adj[i];
    while (e) {
      Edge *tmp = e->next;
      free(e);
      e = tmp;
    }
  }
  free(g->adj);
  free(g);
}

/* ─── Edge operations ────────────────────────────────────────────────────── */

void graph_add_edge(Graph *g, int u, int v, double w) {
  if (u < 0 || u >= g->n || v < 0 || v >= g->n) {
    fprintf(stderr, "graph_add_edge: vertex out of range (%d, %d)\n", u, v);
    return;
  }
  adj_push(g, u, v, w);
  if (!g->directed)
    adj_push(g, v, u, w);
  g->m++;
}

bool graph_has_edge(const Graph *g, int u, int v) {
  for (Edge *e = g->adj[u]; e; e = e->next)
    if (e->to == v)
      return true;
  return false;
}

double graph_edge_weight(const Graph *g, int u, int v) {
  for (Edge *e = g->adj[u]; e; e = e->next)
    if (e->to == v)
      return e->weight;
  return NAN;
}

/* ─── Edge-list format  ──────────────────────────────────────────────────── */
/*
 *  Header: <n> <m> [directed] [weighted]
 *  Body:   <u> <v> [weight]          (0-indexed vertices)
 */
static Graph *read_edge_list(FILE *fp) {
  char buf[512];
  if (!next_line(fp, buf, sizeof buf)) {
    fputs("Empty file\n", stderr);
    return NULL;
  }

  int n, m, dir = 0, wt = 0;
  int parsed = sscanf(buf, "%d %d %d %d", &n, &m, &dir, &wt);
  if (parsed < 2) {
    fputs("Bad header\n", stderr);
    return NULL;
  }

  Graph *g = graph_create(n, (bool)dir, (bool)wt);

  for (int i = 0; i < m; i++) {
    if (!next_line(fp, buf, sizeof buf))
      break;
    int u, v;
    double w = 1.0;
    if (wt)
      sscanf(buf, "%d %d %lf", &u, &v, &w);
    else
      sscanf(buf, "%d %d", &u, &v);
    graph_add_edge(g, u, v, w);
  }
  return g;
}

static void write_edge_list(const Graph *g, FILE *fp) {
  fprintf(fp, "# edge list: n m directed weighted\n");
  fprintf(fp, "%d %d %d %d\n", g->n, g->m, (int)g->directed, (int)g->weighted);
  for (int u = 0; u < g->n; u++) {
    for (Edge *e = g->adj[u]; e; e = e->next) {
      if (!g->directed && e->to < u)
        continue; /* skip reverse copy */
      if (g->weighted)
        fprintf(fp, "%d %d %.6g\n", u, e->to, e->weight);
      else
        fprintf(fp, "%d %d\n", u, e->to);
    }
  }
}

/* ─── Adjacency-list format ──────────────────────────────────────────────── */
/*
 *  Header: <n> [directed] [weighted]
 *  Body:   <u>  <v1>[,w1]  <v2>[,w2]  ...
 */
static Graph *read_adj_list(FILE *fp) {
  char buf[4096];
  if (!next_line(fp, buf, sizeof buf)) {
    fputs("Empty file\n", stderr);
    return NULL;
  }

  int n, dir = 0, wt = 0;
  sscanf(buf, "%d %d %d", &n, &dir, &wt);

  Graph *g = graph_create(n, (bool)dir, (bool)wt);

  for (int i = 0; i < n; i++) {
    if (!next_line(fp, buf, sizeof buf))
      break;
    char *tok = strtok(buf, " \t\n");
    if (!tok)
      continue;
    int u = atoi(tok);
    while ((tok = strtok(NULL, " \t\n"))) {
      int v;
      double w = 1.0;
      char *comma = strchr(tok, ',');
      if (comma) {
        *comma = '\0';
        v = atoi(tok);
        w = atof(comma + 1);
      } else
        v = atoi(tok);
      graph_add_edge(g, u, v, w);
    }
  }
  return g;
}

static void write_adj_list(const Graph *g, FILE *fp) {
  fprintf(fp, "# adjacency list: n directed weighted\n");
  /* Mark directed=1 always so the reader adds edges exactly once */
  fprintf(fp, "%d 1 %d\n", g->n, (int)g->weighted);
  for (int u = 0; u < g->n; u++) {
    fprintf(fp, "%d", u);
    for (Edge *e = g->adj[u]; e; e = e->next) {
      if (!g->directed && e->to < u)
        continue; /* skip reverse copy */
      if (g->weighted)
        fprintf(fp, " %d,%.6g", e->to, e->weight);
      else
        fprintf(fp, " %d", e->to);
    }
    fputc('\n', fp);
  }
}

/* ─── Public I/O ─────────────────────────────────────────────────────────── */

Graph *graph_read_stream(FILE *fp, GraphFmt fmt) {
  if (fmt == FMT_ADJ_LIST)
    return read_adj_list(fp);
  return read_edge_list(fp);
}

Graph *graph_read_file(const char *path, GraphFmt fmt) {
  if (fmt == FMT_AUTO)
    fmt = detect_fmt(path);
  FILE *fp = fopen(path, "r");
  if (!fp) {
    perror(path);
    return NULL;
  }
  Graph *g = graph_read_stream(fp, fmt);
  fclose(fp);
  return g;
}

void graph_write_stream(const Graph *g, FILE *fp, GraphFmt fmt) {
  if (fmt == FMT_ADJ_LIST)
    write_adj_list(g, fp);
  else
    write_edge_list(g, fp);
}

void graph_write_file(const Graph *g, const char *path, GraphFmt fmt) {
  if (fmt == FMT_AUTO)
    fmt = detect_fmt(path);
  FILE *fp = fopen(path, "w");
  if (!fp) {
    perror(path);
    return;
  }
  graph_write_stream(g, fp, fmt);
  fclose(fp);
}

/* ─── Utility ────────────────────────────────────────────────────────────── */

void graph_print(const Graph *g) {
  printf("Graph: n=%d  m=%d  %s  %s\n", g->n, g->m,
         g->directed ? "directed" : "undirected",
         g->weighted ? "weighted" : "unweighted");
  for (int u = 0; u < g->n; u++) {
    printf("  %d ->", u);
    for (Edge *e = g->adj[u]; e; e = e->next) {
      if (g->weighted)
        printf("  %d(%.4g)", e->to, e->weight);
      else
        printf("  %d", e->to);
    }
    putchar('\n');
  }
}

int graph_degree(const Graph *g, int v) {
  int d = 0;
  for (Edge *e = g->adj[v]; e; e = e->next)
    d++;
  return d;
}

int graph_in_degree(const Graph *g, int v) {
  if (!g->directed)
    return graph_degree(g, v);
  int d = 0;
  for (int u = 0; u < g->n; u++)
    for (Edge *e = g->adj[u]; e; e = e->next)
      if (e->to == v)
        d++;
  return d;
}
