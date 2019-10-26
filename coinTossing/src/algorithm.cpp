#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

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

using namespace std;

enum result {AW = 1, BW = 2, UNKNOWN = 0};
typedef pair<pair<int, int>, result> game;

void algorithm() {
    int n, m; cin >> n >> m;
    vector<game> R(m);
    for (int i = 0; i < m; i ++) {
        int a, b, c; cin >> a >> b >> c;
        R[i].first = pair<int, int>(a, b); R[i].second = (result)c;
    }
    vector<int> SB(n);
    for (int i = 0; i < n; i ++) {
        int s; cin >> s;
        SB[i] = s;
    }

    // remove deterministic results from scoreboard
    vector<pair<int, int>> unknown_rounds;
    for (auto &r:R) {
        if (r.second == result::AW) {
            SB[r.first.first] --;
        }
        else if (r.second == result::BW) {
            SB[r.first.second] --;
        }
        else {
            unknown_rounds.emplace_back(r.first.first, r.first.second);
        }
    }

    //build graph
    int num_ur = unknown_rounds.size();
    graph G(num_ur + n); edge_adder adder(G);
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_target = boost::add_vertex(G);
    for (int i = 0; i < num_ur; i ++) {
        adder.add_edge(v_source, i, 1);
        adder.add_edge(i, num_ur + unknown_rounds[i].first, 1);
        adder.add_edge(i, num_ur + unknown_rounds[i].second, 1);
    }

    if (accumulate(SB.begin(), SB.end(), 0) != num_ur) {cout << "no" << "\n"; return;}
    for (int i = 0; i < n; i ++) {
        if (SB[i] > 0) adder.add_edge(num_ur + i, v_target, SB[i]);
        else if (SB[i] < 0) {cout << "no" << "\n"; return;}
    }

    int flow = boost::push_relabel_max_flow(G, v_source, v_target);
    if (flow == num_ur) cout << "yes" << "\n";
    else cout << "no" << "\n";


}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int num_test_cases; cin >> num_test_cases;

    while (num_test_cases--) {
        algorithm();
    }
}
