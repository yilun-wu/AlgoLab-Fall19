#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;
// BGL include
#include <boost/graph/adjacency_list.hpp>

// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

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

void algorithm() {
    int n, m, s, d; cin >> n >> m >> s >> d;

    graph G(2*n);
    edge_adder adder(G);
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_target = boost::add_vertex(G);

    for (int i = 0; i < m; i ++) {
        int from, to; cin >> from >> to;
        adder.add_edge(2*from+1, 2*to, m);
    }

    for (int i = 0; i < s; i ++) {
        int start; cin >> start;
        adder.add_edge(v_source, 2*start, m);
    }

    for (int i = 0; i < d; i ++) {
        int end; cin >> end;
        adder.add_edge(2*end+1, v_target, m);
    }

    for (int i = 0; i < n; i ++) {
        adder.add_edge(2*i, 2*i+1, 1);
    }

    cout << boost::push_relabel_max_flow(G, v_source, v_target) << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int num_test_cases; cin >> num_test_cases;
    
    while (num_test_cases--) {
        algorithm();
    }
}
