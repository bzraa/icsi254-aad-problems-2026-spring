#include<bits/stdc++.h>
#define pii pair<int,int>
#define ff first
#define ss second
using namespace std;

struct Node {
    /* оройн нэмэлт мэдээлэл оруулж болно */
    string name;

    Node() : name("") {}
    Node(string _name) : name(_name) {}
};

struct Edge {
    /* ирмэгийн нэмэлт мэдээлэл оруулж болно */
    int u, v;
    int w;

    bool operator<(const Edge& other) const {
        return w < other.w; 
        /*
        Энэ < операторын тодорхойлолт нь ирмэгүүдийг жингээр нь жиших боломжтой болгож байна.
        Жишээлбэл vector<Edge> myEdges; ирмэгүүдээр дүүрсэн vector байсан гэж үзвэл.
        Стандарт сангийн sort функцыг ашиглан myEdges vector - н ирмэгүүдийг жингээр нь
        буурахгүйгээр эрэмбэлэх боломжтой.

        sort(myEdges.begin(), myEdges.end()); -> myEdges ирмэгүүдийн жин нь буурахгүйгээр эрэмбэлэгдсэн.
        */
    }

    Edge(int _u, int _v, int _w) : u(_u), v(_v), w(_w) {}
};

struct Graph {

    int n;
    int m;
    bool is_directed;
    bool is_weighted;

    vector<vector<pii> > adj; // pii -> pair<int,int> 

    vector<Node> nodes;
    vector<Edge> edges;

    Graph(int _n, bool directed = false, bool _weighted = false) : n(_n), is_directed(directed), is_weighted(_weighted) {

        adj.assign(_n, vector<pii>());
        nodes.assign(n, Node(""));

    }

    Graph(int _n, int _m, const vector<Edge>& _edges, bool directed = false, bool _weighted = false) : n(_n), m(_m), is_directed(directed), is_weighted(_weighted) {

        adj.assign(_n, vector<pii>());
        nodes.assign(n, Node(""));

        // Adjacency list 

        if(is_weighted){
            for(auto edge : _edges){
                adj[edge.u].push_back({edge.w, edge.v}); // ff -> first, ss -> second

                if(!directed)
                    adj[edge.v].push_back({edge.w, edge.u});
            }
        } else{
            for(auto edge : _edges){
                adj[edge.u].push_back({1, edge.v});

                if(!directed)
                    adj[edge.v].push_back({1, edge.u});
            }
        }

        // Edge list

        edges = _edges;

    }

    void add_node(){
        n++;
        adj.resize(n);
        nodes.resize(n);
    }

    void add_edge(int _u, int _v, int _w){
        assert(_u >= 0 and _u < n and _v >= 0 and _v < n);
        assert(is_weighted);

        adj[_u].push_back({_w, _v});
        if(!is_directed) adj[_v].push_back({_w, _v});

        edges.push_back(Edge(_u, _v, _w));

    }

    void add_edge(int _u, int _v){
        assert(_u >= 0 and _u < n and _v >= 0 and _v < n);
        assert(!is_weighted);

        adj[_u].push_back({1, _v});
        if(!is_directed) adj[_v].push_back({1, _v});

        edges.push_back(Edge(_u, _v, 1));

    }

    void print_adjacency_list(bool weighted = false){

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

    void print_edges(bool weighted = false){
        for(auto edge : edges){
            if(weighted)
                cout << edge.u << ' ' << edge.v << ' ' << edge.w << endl;
            else
                cout << edge.u << ' ' << edge.v << endl;
        }
    }

    void add_node_info(int id, const string name){
        assert(id < n and id >= 0);
        nodes[id].name = name;
    }

    void print_node_name(){
        for(int i = 0;i < n;i++)
            cout << i << ": " << nodes[i].name << endl;
    }

    void print_node(int i){
        cout << i << ": " << nodes[i].name << endl;
    }

};

int32_t main(){

    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    // in файлаас уншиж авсан жишээ хэрэглээ

    int n, m;

    cin >> n >> m;

    vector<Edge> edges;

    for(int i = 0;i < m;i++){

        int u, v, w; cin >> u >> v >> w;
        edges.push_back(Edge(u, v, w));

    }

    Graph myGraph = Graph(n, m, edges, false, true);

    myGraph.print_adjacency_list(true);
    myGraph.print_edges(true);

    myGraph.add_edge(1, 3, 100);

    myGraph.print_edges(true);

    myGraph.add_node_info(0, "I am the first node!");

    myGraph.print_node_name();

}