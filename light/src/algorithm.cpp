#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

using namespace std;

int binarySearch(int lb, int ub, int h, vector<K::Point_2> &N, vector<pair<K::Point_2, int>> &M) {
    if (ub - lb == 1) return ub;
    else {
        int mid = floor((ub + lb)/2);
        // include first mid lamps
        Triangulation t;
        t.insert(N.begin(), N.begin()+mid);
        bool non_survive = true;
        for (auto &m:M) {
            Triangulation::Vertex_handle e = t.nearest_vertex(m.first);
            if (CGAL::squared_distance(e->point(), m.first) >= pow(h+m.second, 2)) non_survive = false;
        }
        if (non_survive) return binarySearch(lb, mid, h, N, M);
        else return binarySearch(mid, ub, h, N, M);
    }
}

void algorithm() {
    int m, n; cin >> m >> n;
    vector<pair<K::Point_2, int>> M; M.reserve(m);
    for (int i = 0; i < m; i ++) {
        K::Point_2 p; cin >> p; int r; cin >> r;
        M.push_back(make_pair(p, r));
    }
    int h; cin >> h;
    vector<K::Point_2> N; N.reserve(n);
    for (int i = 0; i < n; i ++) {
        K::Point_2 p; cin >> p;
        N.push_back(p);
    }

    Triangulation t;
    t.insert(N.begin(), N.end());

    bool non_survive = true;

    for (int i = 0; i < m; i ++) {
        Triangulation::Vertex_handle e = t.nearest_vertex(M[i].first);
        if (CGAL::squared_distance(e->point(), M[i].first) >= pow(h+M[i].second, 2)){
            cout << i << " "; non_survive = false;
        }
    }
    
    if (non_survive) {
        // TODO: Implement Binary Search to find out min_lamps to kill all
        int min_lamps = binarySearch(0, n, h, N, M);
        Triangulation t;
        t.insert(N.begin(), N.begin() + min_lamps - 1);
        for (int i = 0; i < m; i ++) {
            Triangulation::Vertex_handle e = t.nearest_vertex(M[i].first);
            if (CGAL::squared_distance(e->point(), M[i].first) >= pow(h+M[i].second, 2)){
                cout << i << " ";
            }
        }
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
