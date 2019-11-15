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

void algorithm(int n) {
    vector<K::Point_2> rests;
    for (int i = 0; i < n; i ++) {
        K::Point_2 p;
        cin >> p; rests.push_back(p);
    }
    Triangulation t;
    t.insert(rests.begin(), rests.end());

    int m; cin >> m;
    for (int i = 0; i < m; i ++) {
        K::Point_2 p;
        cin >> p;
        Triangulation::Vertex_handle e = t.nearest_vertex(p);
        cout << (long)CGAL::to_double(CGAL::squared_distance(p, e->point())) << "\n";
    }

}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    while (true) {
        int num_existing_rest; cin >> num_existing_rest;
        if (num_existing_rest) {
            algorithm(num_existing_rest);
        }
        else return 0;
    }

}
