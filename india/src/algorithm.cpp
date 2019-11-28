#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

using namespace std;

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  const edge_desc add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
    return e;
  }
};

void algorithm() {
    int c, g, b, k, a; cin >> c >> g >> b >> k >> a;

    const int v_source = c;
    const int v_sink = c + 1;

    graph G(c + 2);
    edge_adder adder(G);
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);

    for (int i = 0; i < g; i ++) {
        int x, y, d, e; cin >> x >> y >> d >> e;
        adder.add_edge(x, y, e, d);
    }

    adder.add_edge(v_source, k, numeric_limits<long>::max(), 0);
    auto sink_e = adder.add_edge(a, v_sink, numeric_limits<long>::max(), 0);

    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
    long cost = boost::find_flow_cost(G);
    int s_flow = 0;
    out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
        s_flow += c_map[*e] - rc_map[*e];     
    }
    if (cost <= b) {
        cout << s_flow << "\n"; return;
    }

    // begin binary search to find max flow under budget
    int flow_up = s_flow, flow_low = 0;

    while (flow_up - flow_low > 1) {
        int flow_con = (flow_up + flow_low) / 2;
        c_map[sink_e] = flow_con;
        boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
        long cost = boost::find_flow_cost(G);
        if (cost > b) flow_up = flow_con;
        else if (cost <= b) flow_low = flow_con;
    }

    c_map[sink_e] = flow_low;
    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
    s_flow = 0;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
        s_flow += c_map[*e] - rc_map[*e];     
    }
    cout << s_flow << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int num_test_cases; cin >> num_test_cases;

    while(num_test_cases --) {
        algorithm();
    }
}
