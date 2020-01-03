#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;
typedef boost::graph_traits<weighted_graph>::edge_iterator		        edge_it;
typedef boost::graph_traits<weighted_graph>::out_edge_iterator		    out_edge_it;

using namespace std;

void algorithm() {
    int n, idx_t; cin >> n >> idx_t;
    weighted_graph G(n);
    weight_map weights = boost::get(boost::edge_weight, G);

    edge_desc e;
    
    vector<vector<int>> C(n, vector<int>(n, -1));
    for (int i = 0; i < n-1; i ++) {
        for (int j = i+1; j < n; j ++) {
            int c; cin >> c;
            C[i][j] = c; C[j][i] = c;
            e = boost::add_edge(i, j, G).first; weights[e] = c;
        }
    }

    vector<edge_desc> mst;
    boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
    vector<vertex_desc> prim_predmap(n);
    boost::prim_minimum_spanning_tree(G, boost::make_iterator_property_map(prim_predmap.begin(),
			boost::get(boost::vertex_index, G)), boost::root_vertex(idx_t-1));

    weighted_graph MST(n);
    weight_map MST_w = boost::get(boost::edge_weight, MST);
    long min_cost = 0;
    for (int i = 0; i < n ; i ++) {
        if (i != prim_predmap[i]) {
            e = boost::add_edge(i, prim_predmap[i], MST).first; MST_w[e] = C[i][prim_predmap[i]];
            min_cost += MST_w[e];
        }
    }
    
    vector<vector<int>> max_edge(n, vector<int>(n, -1));
    for (int u = 0; u < n; u++) {
		boost::queue<int> Q;
		Q.push(u);
		vector<bool> vis(n,false);
		vis[u] = true;
		while(!Q.empty()) {
			const int x = Q.front();
			Q.pop();
			out_edge_it ebeg, eend;
			for (tie(ebeg, eend) = out_edges(x, MST); ebeg != eend; ++ebeg) {
				const int v = target(*ebeg, MST);
				if(vis[v]) continue;
				max_edge[u][v] = max(max_edge[u][x],MST_w[*ebeg]);
				max_edge[v][u] = max_edge[u][v];
				vis[x] = true;
				Q.push(v);
			}
		}
	}

    int min_diff = numeric_limits<int>::max();
    for (int i = 0; i < n-1; i ++) {
        for (int j = i+1; j < n; j ++) {
            if (j == prim_predmap[i] || i == prim_predmap[j]) continue;
            min_diff = min(min_diff, C[i][j] - max_edge[i][j]);
        }
    }

    cout << min_cost+min_diff << "\n";
    
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int num_test_cases; cin >> num_test_cases;

    while (num_test_cases--) {
        algorithm();
    }
}
