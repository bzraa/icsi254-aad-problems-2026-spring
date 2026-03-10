#include<bits/stdc++.h>
#include"Graph.h"
#define ff first
#define ss second
#define pii pair<int,int> 
using namespace std;

Node::Node() : name("") {}
Node::Node(string _name) : name(_name) {}

bool Edge::operator<(const Edge& other) const {
    /*
    Энэ < операторын тодорхойлолт нь ирмэгүүдийг жингээр нь жиших боломжтой болгож байна.
    Жишээлбэл vector<Edge> myEdges; ирмэгүүдээр дүүрсэн vector байсан гэж үзвэл.
    Стандарт сангийн sort функцыг ашиглан myEdges vector - н ирмэгүүдийг жингээр нь
    буурахгүйгээр эрэмбэлэх боломжтой.

    sort(myEdges.begin(), myEdges.end()); -> myEdges ирмэгүүдийн жин нь буурахгүйгээр эрэмбэлэгдсэн.
    */
    return w < other.w;
}

bool Edge::operator==(const Edge& other) const {
    return u == other.u and v == other.v and (!is_weighted or w == other.w);
}

Edge::Edge(){}
Edge::Edge(int _u, int _v, bool _weighted) : u(_u), v(_v), is_weighted(_weighted) {}
Edge::Edge(int _u, int _v, int _w, bool _weighted) : u(_u), v(_v), w(_w), is_weighted(_weighted) {}

Graph::Graph(int _n, bool directed, bool _weighted) : n(_n), is_directed(directed), is_weighted(_weighted), m(0) {
    adj.assign(n, vector<pii>());
    nodes.assign(n, Node());
}

Graph::Graph(int _n, int _m, const vector<Edge>& _edges, bool _directed, bool _weighted) : n(_n), m(_m), is_directed(_directed), is_weighted(_weighted) {

    adj.assign(n, vector<pii>());

    nodes.assign(n, Node());

    // Adjacency list 

    if(is_weighted){
        for(auto edge : _edges){
            adj[edge.u].push_back({edge.w, edge.v}); // ff -> first, ss -> second

            if(!is_directed)
                adj[edge.v].push_back({edge.w, edge.u});
        }
    } else{
        for(auto edge : _edges){
            adj[edge.u].push_back({1, edge.v});

            if(!is_directed)
                adj[edge.v].push_back({1, edge.u});
        }
    }

    // Edge list

    assert(int32_t(_edges.size()) == m);

    edges = _edges;

}

void Graph::add_node(){
    n++;
    adj.resize(n);
    nodes.resize(n);
}

void Graph::add_edge(int _u, int _v, int _w){
    assert(_u >= 0 and _u < n and _v >= 0 and _v < n);
    assert(is_weighted);

    adj[_u].push_back({_w, _v});
    if(!is_directed) adj[_v].push_back({_w, _u});

    edges.push_back(Edge(_u, _v, _w));
    m++;
}

void Graph::add_edge(int _u, int _v){
    assert(_u >= 0 and _u < n and _v >= 0 and _v < n);
    assert(!is_weighted);

    adj[_u].push_back({1, _v});
    if(!is_directed) adj[_v].push_back({1, _u});

    edges.push_back(Edge(_u, _v, 1));
    m++;
}

void Graph::print_adjacency_list(bool weighted){
    for(int i = 0;i < n;i++){

        cout << i << " : ";

        for(auto neigh : adj[i]){
            if(weighted){
                cout << "{";
                cout << neigh.ss << ", " << neigh.ff;
                cout << "} ";
            } else{
                cout << neigh.ss << ' ';
            }
        }

        cout << endl;

    }

}

void Graph::print_edges(bool weighted){
    for(const auto &edge : edges){
        if(weighted)
            cout << edge.u << ' ' << edge.v << ' ' << edge.w << endl;
        else
            cout << edge.u << ' ' << edge.v << endl;
    }
}

void Graph::add_node_info(int id, const string& name){
    assert(id < n and id >= 0);
    nodes[id].name = name;
}

void Graph::print_node_name(){
    for(int i = 0;i < n;i++)
        cout << i << ": " << nodes[i].name << endl;
}

void Graph::print_node(int i){
    cout << i << ": " << nodes[i].name << endl;
}