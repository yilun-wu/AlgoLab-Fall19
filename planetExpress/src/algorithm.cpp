#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/reverse_graph.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      graph;
typedef boost::property_map<graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor          vertex_desc;
using namespace std;

void algorithm() {
    int n, m, k, T; cin >> n >> m >> k >> T;
    vector<int> tele_nodes; // pair of node ID/SCC group
    for (int i = 0; i < T; i ++) {int node; cin >> node; tele_nodes.push_back(node);}
    graph G(n);
    weight_map weights = boost::get(boost::edge_weight, G);
    for (int i = 0; i < m; i ++) {
        int s, d, w; cin >> s >> d >> w;
        edge_desc e;
        e = boost::add_edge(s, d, G).first; weights[e] = w;
    }

    // add tele edges
    vector<int> scc_map(n);
    int nscc = boost::strong_components(G,
      boost::make_iterator_property_map(scc_map.begin(), boost::get(boost::vertex_index, G)));
    vector<vector<int>> scc_group(nscc);
    for (int i = 0; i < T; i ++) {
        scc_group[scc_map[tele_nodes[i]]].push_back(tele_nodes[i]);
    }

    for (int i = 0; i < nscc; i ++) {
        int price = scc_group[i].size() - 1;
        for (int ns = 0; ns < scc_group[i].size(); ns ++) {
            edge_desc e = boost::add_edge(n + i, scc_group[i][ns], G).first; weights[e] = price;
            e = boost::add_edge(scc_group[i][ns], n + i, G).first; weights[e] = 0;
        }
    }

    // find minimum path from last node
    std::vector<int> dist_map(n + nscc);
    graph R;
    boost::copy_graph(boost::make_reverse_graph(G), R);
    boost::dijkstra_shortest_paths(R, n-1,
       boost::distance_map(boost::make_iterator_property_map( dist_map.begin(), boost::get(boost::vertex_index, G))));

    int min_dist = *min_element(dist_map.begin(), dist_map.begin() + k);
    if (min_dist <= 1000000) cout << min_dist << '\n';
    else cout << "no" << '\n';    

}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int num_test_cases; cin >> num_test_cases;
    while (num_test_cases --) {
        algorithm();
    }
    return 0;
}
