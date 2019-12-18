#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator
using namespace std;

class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
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
  }
};

struct task {
    int start, end;
    int t_start, t_end;
    int profit;
    task(int start, int end, int t_start, int t_end, int profit):
        start(start), end(end), t_start(t_start), t_end(t_end), profit(profit) {}
};

void algorithm() {
    int N, S; cin >> N >> S;
    const int v_source = 0;
    graph G(S+1); edge_adder adder(G);
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);

    int total_cars = 0;
    for (int i = 1; i <= S; i ++) {
        int init_cars; cin >> init_cars; total_cars += init_cars;
        adder.add_edge(v_source, i, init_cars, 0);
    }

    vector<task> tasks; tasks.reserve(N);
    int max_rate = 0; int end_time = 0;
    for (int i = 0; i < N; i ++) {
        int s_start, s_end, t_start, t_end, profit; 
        cin >> s_start >> s_end >> t_start >> t_end >> profit;
        tasks.emplace_back(s_start-1, s_end-1, t_start, t_end, profit);
        max_rate = max(max_rate, (int)ceil(double(profit)/(t_end-t_start)));
        end_time = max(end_time, t_end);
    }
    
    int node_idx = S; // include the source node and all t=0 node
    // keep a reference on the nodes established
    vector<map<int, int>> nodes(S);
    for (auto task:tasks) {
        int node_start, node_end;
        int duration = task.t_end - task.t_start;
        auto itr_s = nodes[task.start].find(task.t_start);
        auto itr_e = nodes[task.end].find(task.t_end);
        if (itr_s == nodes[task.start].end()) {
            node_start = ++node_idx;
            nodes[task.start].insert({task.t_start, node_start});
        }
        else node_start = itr_s->second;
        if (itr_e == nodes[task.end].end()) {
            node_end = ++node_idx;
            nodes[task.end].insert({task.t_end, node_end});
        }
        else node_end = itr_e->second;
        adder.add_edge(node_start, node_end, 1, duration * max_rate-task.profit);
    }
    //add edge along station axis until t_end
    const int v_target = ++node_idx;
    int inf = numeric_limits<int>::max();
    for (int i = 0; i < S; i ++) {
        if (!nodes[i].empty()) {
            adder.add_edge(i+1, nodes[i].begin()->second, inf, max_rate*nodes[i].begin()->first);
            auto last_n = --nodes[i].end();
            adder.add_edge(last_n->second, v_target, inf, max_rate*(end_time - last_n->first));
        }
        else {
            adder.add_edge(i+1, v_target, inf, max_rate * end_time);
        }
        for (auto itr = nodes[i].begin(); itr != --nodes[i].end(); itr++) {
            auto n_1 = itr; auto n_2 = next(n_1, 1);
            adder.add_edge(n_1->second, n_2->second, inf, max_rate*(n_2->first - n_1->first));
        }
    }

    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    int cost = boost::find_flow_cost(G);
    int profit = max_rate * end_time * total_cars - cost;
    cout << profit << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int num_test_cases; cin >> num_test_cases;
    while (num_test_cases--) {
        algorithm();
    }
}
