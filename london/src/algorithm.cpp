#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

#define k_nl 26

using namespace std;

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// Custom edge adder class, highly recommended
class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

inline int ascii(char c) {return int(c) - 65;}

void algorithm() {
    int h, w; cin >> h >> w;
    string str; cin >> str;
    vector<int> input(k_nl, 0);
    for(auto & c:str) {
        input[ascii(c)] ++;
    }
    vector<vector<int>> letters_f (h, vector<int>(w));
    vector<vector<int>> letters_b (h, vector<int>(w));
    for (int i = 0; i < h; i ++) {
        for (int j = 0; j < w; j ++) {
            char c; cin >> c;
            letters_f[i][j] = ascii(c);
        }
    }
    for (int i = 0; i < h; i ++) {
        for (int j = 0; j < w; j ++) {
            char c; cin >> c;
            letters_b[i][j] = ascii(c);
        }
    }

    // establish pairs
    graph G(k_nl);
    edge_adder adder(G);
    vector<int> pairs(k_nl, 0);

    for (int i = 0; i < h; i ++ ) {
        for (int j = 0; j < w; j ++) {
            int a = letters_f[i][j];
            int b = letters_b[i][w - j - 1];
            adder.add_edge(a, b, 1);
            adder.add_edge(b, a, 1);
            pairs[a] ++; pairs[b] ++;
        }
    }

    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);
    
    for (int i = 0; i < input.size(); i ++) {
        //if (input[i]) {
            adder.add_edge(v_source, i, input[i]);
        //}
    }

    for (int i = 0; i < k_nl; i++) {
        int sink_capacity = pairs[i] - input[i];
        assert(sink_capacity >= 0);
        if (sink_capacity > 0) {
            adder.add_edge(i, v_sink, sink_capacity);
        }
    }

    boost::push_relabel_max_flow(G, v_source, v_sink);

    // Retrieve the capacity map and reverse capacity map
    const auto c_map = boost::get(boost::edge_capacity, G);
    const auto rc_map = boost::get(boost::edge_residual_capacity, G);
    
    for (int i = 0 ; i < input.size(); i ++) {
        edge_desc edge = boost::edge(v_source, i, G).first;
        const long flow_through_edge = c_map[edge] - rc_map[edge];
        if (flow_through_edge < input[i]) {
            cout << "No" << '\n'; return;
        }
    }

    cout << "Yes" << '\n';
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int num_test_cases; cin >> num_test_cases;
    
    while (num_test_cases --) {
        algorithm();
    }
}
