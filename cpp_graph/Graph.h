#ifndef CPP_GRAPH_H
#define CPP_GRAPH_H

#include <string>
#include <vector>
#include <utility>

using pii = std::pair<int,int>;

struct Node {
    /* оройн нэмэлт мэдээлэл оруулж болно */
    std::string name;

    Node();
    Node(std::string _name);
};

struct Edge {
    /* ирмэгийн нэмэлт мэдээлэл оруулж болно */
    int u, v;
    int w;

    bool is_weighted;

    bool operator<(const Edge& other) const;
    bool operator==(const Edge& other) const;
    Edge();
    Edge(int _u, int _v, bool _weighted = false);
    Edge(int _u, int _v, int _w, bool _weighted = true);
};

struct Graph {

    int n;
    int m;
    bool is_directed;
    bool is_weighted;

    std::vector<std::vector<pii> > adj; // pii -> pair<int,int> 

    std::vector<Node> nodes;
    std::vector<Edge> edges;

    Graph(int _n, bool directed = false, bool _weighted = false);

    Graph(int _n, int _m, const std::vector<Edge>& _edges, bool _directed = false, bool _weighted = false);

    void add_node();

    void add_edge(int _u, int _v, int _w);

    void add_edge(int _u, int _v);

    void print_adjacency_list(bool weighted = false);

    void print_edges(bool weighted = false);

    void add_node_info(int id, const std::string& name);

    void print_node_name();

    void print_node(int i);

};

#endif