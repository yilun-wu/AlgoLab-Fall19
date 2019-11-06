#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

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

using namespace std;

void algorithm() {
    int Z, J; cin >> Z >> J;
    graph G(Z + J);
    edge_adder adder(G);
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_target = boost::add_vertex(G);
    int total_reward = 0;
    for (int i = 0; i < Z; i ++) {
        int zone_price; cin >> zone_price;
        adder.add_edge(J + i, v_target, zone_price);
    }
    for (int i = 0 ; i < J; i ++) {
        int reward; cin >> reward;
        adder.add_edge(v_source, i, reward);
        total_reward += reward;
    }
    for (int i = 0; i < J; i ++) {
        int num_zones; cin >> num_zones;
        for (int j = 0; j < num_zones; j ++) {
            int zone; cin >> zone;
            adder.add_edge(i, J + zone, numeric_limits<int>::max());
        }
    }
    long flow = boost::push_relabel_max_flow(G, v_source, v_target);
    cout << total_reward - flow << endl;


}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int num_test_cases; cin >> num_test_cases;

    while (num_test_cases --) {
        algorithm();
    }

    return 0;
}
