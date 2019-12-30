#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

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

struct food {
    int from, to;
    int val;

    food(int from, int to, int val): from(from), to(to), val(val) {}
};

bool compareFood(const food & a, const struct food &b) {
    if (a.from != b.from) return a.from < b.from;
    else if (a.to != b.to) return a.to < b.to;
    else return a.val > b.val;
}

void algorithm() {
    int n, m; cin >> n >> m;
    graph G(n+2);
    const int v_source = n;
    const int v_target = n+1;
    edge_adder adder(G);

    int total_flow = 0; vector<int> caps;
    for (int i = 0; i < n-1; i ++) {
        int cap; cin >> cap; total_flow += cap; caps.push_back(cap);
        adder.add_edge(v_source, i, cap, 0);
        adder.add_edge(i+1, v_target, cap, 0);
        adder.add_edge(i, i+1, cap, 128);
    }

    vector<food> foods; foods.reserve(m);
    for (int i = 0; i < m; i ++) {
        int from, to, val; cin >> from >> to >> val;
        foods.emplace_back(from, to, val);
    }
    sort(foods.begin(), foods.end(), compareFood);

    vector<food> foods_to_add;
    auto itr = foods.begin();
    while (itr != foods.end()) {
        auto itr_e = itr;
        while (itr_e->to == itr->to && itr_e->from == itr->from) itr_e ++;
        int num_foods = distance(itr, itr_e);
        int min_cap = *min_element(caps.begin() + itr->from, caps.begin() + itr->to);
        foods_to_add.insert(foods_to_add.begin(), itr, itr+min(num_foods, min_cap));
        itr = itr_e;
    }

    for (auto &food:foods_to_add) {
        adder.add_edge(food.from, food.to, 1, 128*(food.to-food.from)-food.val);
    }

    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    int cost = -boost::find_flow_cost(G) + 128*total_flow;
    cout << cost << "\n";

}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int num_test_cases; cin >> num_test_cases;

    while (num_test_cases--) {
        algorithm();
    }
}
