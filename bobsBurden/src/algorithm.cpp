#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;


#define IDX(i, j) ((i+1)*(i)/2 + j)

void algorithm() {
    int k; cin >> k;
    vector<vector<int>> w(k, vector<int>(k, -1));
    vector<int> w_linear; w_linear.reserve(k*(k+1)/2);
    weighted_graph G(k*(k+1)/2);
    weight_map weights = boost::get(boost::edge_weight, G);

    edge_desc e;
    const int v_exit1 = 0;
    const int v_exit2 = IDX(k-1, 0);
    const int v_exit3 = IDX(k-1, k-1);
    for (int i = 0; i < k; i ++) {
        for (int j = 0; j < i+1; j ++) {
            int weight; cin >> weight;
            w[i][j] = weight;
            w_linear.push_back(weight);
        }
    }
    
    for (int i = 0; i < k; i ++) {
        for (int j = 0; j < i+1; j ++) {
            // left edge
            if (j >= 1) {e = boost::add_edge(IDX(i, j), IDX(i, j-1), G).first; weights[e] = w[i][j-1];}
            // right edge
            if (j+1 <= i) {e = boost::add_edge(IDX(i, j), IDX(i, j+1), G).first; weights[e] = w[i][j+1];}
            // up-left edge
            if (i >= 1 && j-1 >= 0) {e = boost::add_edge(IDX(i, j), IDX(i-1, j-1), G).first; weights[e] = w[i-1][j-1];}
            // up-right edge
            if (i >=1 && j <= i-1) {e = boost::add_edge(IDX(i, j), IDX(i-1, j), G).first; weights[e] = w[i-1][j];}
            // bottom-left edge
            if (i+1 <= k-1) {e = boost::add_edge(IDX(i, j), IDX(i+1, j), G).first; weights[e] = w[i+1][j];}
            // bottom-right edge
            if (i+1 <= k-1) {e = boost::add_edge(IDX(i, j), IDX(i+1, j+1), G).first; weights[e] = w[i+1][j+1];}
        }
    }
    int n = boost::num_vertices(G);
    std::vector<int> dist_map_1(n); std::vector<int> dist_map_2(n); std::vector<int> dist_map_3(n);
    
    boost::dijkstra_shortest_paths(G, v_exit1,
        boost::distance_map(boost::make_iterator_property_map(dist_map_1.begin(), 
                                        boost::get(boost::vertex_index, G))));

    boost::dijkstra_shortest_paths(G, v_exit2,
        boost::distance_map(boost::make_iterator_property_map(dist_map_2.begin(), 
                                        boost::get(boost::vertex_index, G))));

    boost::dijkstra_shortest_paths(G, v_exit3,
        boost::distance_map(boost::make_iterator_property_map(dist_map_3.begin(), 
                                        boost::get(boost::vertex_index, G))));
    int min_weight = numeric_limits<int>::max();
    for (int i = 0; i < n; i ++) {
        min_weight = min(min_weight, dist_map_1[i] + dist_map_2[i] + dist_map_3[i] - 2*w_linear[i]);
    }

    cout << min_weight << "\n";

}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int num_test_cases; cin >> num_test_cases;

    while (num_test_cases --) {
        algorithm();
    }
}
