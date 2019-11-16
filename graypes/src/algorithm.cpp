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
    vector<K::Point_2> graypes;
    for (int i = 0; i < n; i ++) {
        K::Point_2 p;
        cin >> p; graypes.push_back(p);
    }
    Triangulation t;
    t.insert(graypes.begin(), graypes.end());

    long min_distance = numeric_limits<long>::max(); 
    for (Triangulation::Edge_iterator eit = t.edges_begin(); eit != t.edges_end(); eit ++) {
        auto v1 = eit->first->vertex((eit->second + 1) % 3);
        auto v2 = eit->first->vertex((eit->second + 2) % 3);
        long dist = (long)CGAL::to_double(CGAL::squared_distance(v1->point(), v2->point()));

        if (dist < min_distance) min_distance = dist;
    }

    cout << ceil(sqrt(min_distance) / 2 * 100) << "\n";

}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    while (true) {
        int n; cin >> n;
        if (n) {
            algorithm(n);
        }
        else return 0;
    }

}
