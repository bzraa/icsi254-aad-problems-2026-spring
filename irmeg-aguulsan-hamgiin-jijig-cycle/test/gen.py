#!/usr/bin/env python3
"""
Test generator for "smallest cycle containing edge e".

Input format (edge-list):
  n m 0 0
  u1 v1
  ...
  eu ev        <- query edge (must exist in the graph)

Usage:
  python gen.py <seed> [type]

  type (optional):
    random   – random connected undirected graph  (default)
    bridge   – query edge is a bridge  → answer: No cycle
    triangle – tiny triangle, answer is 3
    cycle_k  – single k-cycle (k read from seed lower bits)
    large    – larger stress-test graph
"""

import sys
import random
from collections import deque


# ── helpers ──────────────────────────────────────────────────────────────────

def spanning_tree_edges(n, rng):
    """Return a random spanning tree on vertices 0..n-1."""
    nodes = list(range(n))
    rng.shuffle(nodes)
    edges = []
    in_tree = {nodes[0]}
    for v in nodes[1:]:
        u = rng.choice(list(in_tree))
        edges.append((u, v))
        in_tree.add(v)
    return edges


def bfs_shortest_path_skip(adj, n, src, dst, skip_u, skip_v):
    """BFS from src to dst in undirected graph, skipping edge (skip_u,skip_v)."""
    dist = [-1] * n
    prev = [-1] * n
    dist[src] = 0
    q = deque([src])
    while q:
        u = q.popleft()
        for v in adj[u]:
            if (u == skip_u and v == skip_v) or (u == skip_v and v == skip_u):
                continue
            if dist[v] != -1:
                continue
            dist[v] = dist[u] + 1
            prev[v] = u
            if v == dst:
                # reconstruct path
                path = []
                cur = dst
                while cur != -1:
                    path.append(cur)
                    cur = prev[cur]
                path.reverse()
                return dist[dst] + 1, path  # cycle_len = hops + 1
            q.append(v)
    return None, None  # no cycle


def build_adj(n, edges):
    adj = [[] for _ in range(n)]
    for u, v in edges:
        adj[u].append(v)
        adj[v].append(u)
    return adj


def emit(n, edges, eu, ev):
    m = len(edges)
    print(f"{n} {m} 0 0")
    for u, v in edges:
        print(f"{u} {v}")
    print(f"{eu} {ev}")


# ── generators ───────────────────────────────────────────────────────────────

def gen_random(rng, n=None, extra=None):
    """Random connected graph; query edge chosen randomly."""
    if n is None:
        n = rng.randint(5, 20)
    if extra is None:
        extra = rng.randint(1, max(1, n // 2))

    edges = spanning_tree_edges(n, rng)
    existing = set(map(frozenset, edges))

    for _ in range(extra * 3):          # try to add `extra` non-duplicate edges
        if len(edges) >= n * (n - 1) // 2:
            break
        u = rng.randint(0, n - 1)
        v = rng.randint(0, n - 1)
        if u != v and frozenset({u, v}) not in existing:
            edges.append((u, v))
            existing.add(frozenset({u, v}))

    eu, ev = rng.choice(edges)
    emit(n, edges, eu, ev)


def gen_bridge(rng):
    """Query edge is a bridge (tree edge only) → answer must be 'No cycle'."""
    n = rng.randint(4, 15)
    edges = spanning_tree_edges(n, rng)   # pure tree = all edges are bridges
    eu, ev = rng.choice(edges)
    emit(n, edges, eu, ev)


def gen_triangle(rng):  # noqa: ARG001
    """Simple triangle: answer is 3."""
    edges = [(0, 1), (1, 2), (0, 2)]
    emit(3, edges, 0, 1)


def gen_cycle_k(rng, k=None):
    """Single k-cycle: answer is k."""
    if k is None:
        k = rng.randint(4, 10)
    edges = [(i, (i + 1) % k) for i in range(k)]
    emit(k, edges, 0, 1)


def gen_large(rng):
    """Larger graph for stress testing (n~50, extra edges)."""
    gen_random(rng, n=rng.randint(30, 50), extra=rng.randint(10, 25))


def gen_guaranteed_cycle(rng):
    """Graph where query edge is guaranteed to be part of at least one cycle."""
    n = rng.randint(5, 20)
    edges = spanning_tree_edges(n, rng)
    existing = set(map(frozenset, edges))

    # Add several extra edges to create cycles
    extra = rng.randint(2, max(2, n // 2))
    for _ in range(extra * 5):
        if len(edges) >= n * (n - 1) // 2:
            break
        u = rng.randint(0, n - 1)
        v = rng.randint(0, n - 1)
        if u != v and frozenset({u, v}) not in existing:
            edges.append((u, v))
            existing.add(frozenset({u, v}))

    # Pick only non-bridge edges as query edge
    adj = build_adj(n, edges)
    non_bridges = []
    for u, v in edges:
        cycle_len, _ = bfs_shortest_path_skip(adj, n, v, u, u, v)
        if cycle_len is not None:
            non_bridges.append((u, v))

    if non_bridges:
        eu, ev = rng.choice(non_bridges)
    else:
        eu, ev = rng.choice(edges)

    emit(n, edges, eu, ev)


# ── main ─────────────────────────────────────────────────────────────────────

TYPES = {
    "random":           gen_random,
    "bridge":           gen_bridge,
    "triangle":         gen_triangle,
    "cycle_k":          gen_cycle_k,
    "large":            gen_large,
    "guaranteed_cycle": gen_guaranteed_cycle,
}

if __name__ == "__main__":
    seed = int(sys.argv[1]) if len(sys.argv) > 1 else 42
    kind = sys.argv[2] if len(sys.argv) > 2 else "random"

    if kind not in TYPES:
        print(f"Unknown type '{kind}'. Available: {', '.join(TYPES)}", file=sys.stderr)
        sys.exit(1)

    rng = random.Random(seed)
    TYPES[kind](rng)
