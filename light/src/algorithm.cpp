#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef std::size_t Index;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index, K> Vb;
typedef CGAL::Triangulation_data_structure_2<Vb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

using namespace std;

void algorithm() {
    int m, n; cin >> m >> n;
    vector<pair<K::Point_2, int>> M; M.reserve(m);
    for (int i = 0; i < m; i ++) {
        K::Point_2 p; cin >> p; int r; cin >> r;
        M.push_back(make_pair(p, r));
    }
    int h; cin >> h;
    vector<pair<K::Point_2, Index>> N; N.reserve(n);
    for (int i = 0; i < n; i ++) {
        K::Point_2 p; cin >> p;
        N.emplace_back(p, i);
    }

    Triangulation t;
    t.insert(N.begin(), N.end());

    vector<int> hit_by(m, -1);
    bool non_survive = true;

    for (int i = 0; i < m; i ++) {
        Triangulation::Vertex_handle e = t.nearest_vertex(M[i].first);
        if (CGAL::squared_distance(e->point(), M[i].first) >= pow(h+M[i].second, 2)){
            cout << i << " "; non_survive = false;
        }
        else {
            hit_by[i] = e->info();
        }
    }
    
    if (non_survive) {
        // TODO: Implement Binary Search to find out min_lamps to kill all
    }
    cout << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int num_test_cases; cin >> num_test_cases;

    while (num_test_cases--) {
        algorithm();
    }
}
