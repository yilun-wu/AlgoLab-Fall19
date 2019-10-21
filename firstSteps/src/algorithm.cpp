#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;
using namespace std;

void algorithm(const weighted_graph & G, const weight_map & weights) {
    std::vector<edge_desc> mst;
    boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));

    int sum = 0;
    for (auto &edge: mst) {
        sum += weights[edge];
    }

    int n = boost::num_vertices(G);
    std::vector<int> dist_map(n);

    boost::dijkstra_shortest_paths(G, 0,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

    cout << sum << ' ' << *max_element(dist_map.begin(), dist_map.end()) << '\n';
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);

    int num_test_cases; cin >> num_test_cases;
    while (num_test_cases--) {
        int n, m; cin >> n >> m;
        weighted_graph G(n);
        weight_map weights = boost::get(boost::edge_weight, G);
        edge_desc e;
        for (int i = 0; i < m; i ++) {
            int s, d, weight; cin >> s >> d >> weight;
            e = boost::add_edge(s, d, G).first; weights[e] = weight;
        }
        algorithm(G, weights);
    }
    return 0;
    // TODO
}
