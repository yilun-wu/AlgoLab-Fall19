#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <tuple>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef std::pair<K::Point_2,Index> IPoint;
typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;
using namespace std;

int n; long r;

std::ostream& operator<<(std::ostream& o, const Edge& e) {
  return o << std::get<0>(e) << " " << std::get<1>(e) << " " << std::get<2>(e);
}

int binarySearch(int lb, int ub, vector<IPoint> & planets) {
    if (ub - lb == 1) return lb;
    else {
        int mid = floor((ub + lb)/2);
        // enemy takes mid steps, left with n-mid planets
        Delaunay t;
        t.insert(planets.begin()+mid, planets.end());

        EdgeV edges;
        int m = n-mid;
        edges.reserve(3*m);
        for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
            Index i1 = e->first->vertex((e->second+1)%3)->info();
            Index i2 = e->first->vertex((e->second+2)%3)->info();
            // ensure smaller index comes first
            if (i1 > i2) std::swap(i1, i2);
            edges.emplace_back(i1, i2, t.segment(e).squared_length());
        }
        std::sort(edges.begin(), edges.end(),
                [](const Edge& e1, const Edge& e2) -> bool {
                return std::get<2>(e1) < std::get<2>(e2);
                    });
        
        boost::disjoint_sets_with_storage<> uf(m);
        Index n_components = m;
        // ... and process edges in order of increasing length
        for (EdgeV::const_iterator e = edges.begin(); e != edges.end() && std::get<2>(*e) <= r*r; ++e) {
            // determine components of endpoints
            Index c1 = uf.find_set(std::get<0>(*e)-mid);
            Index c2 = uf.find_set(std::get<1>(*e)-mid);
            if (c1 != c2) {
                // this edge connects two different components => part of the emst
                uf.link(c1, c2);
                //cout << *e << "\n";
                if (--n_components == 1) break;
            }
        }
        vector<int> cardinality_set(m, 0);
        for (int i = mid; i < n; i++) {
            cardinality_set[uf.find_set(i-mid)] ++;
        }
        int max_cardinality = *max_element(cardinality_set.begin(), cardinality_set.end());
        if (max_cardinality >= mid) lb = mid;
        else ub = mid;
        return binarySearch(lb, ub, planets);
    }
}

void algorithm() {
    cin >> n >> r;
    vector<IPoint> planets;
    for (int i = 0; i < n; i ++) {
        int x, y; cin >> x >> y;
        planets.emplace_back(K::Point_2(x, y), i);
    }
    int lb = 1, ub = ceil(n/2)+1;
    cout << binarySearch(lb, ub, planets) << "\n";

}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int num_test_cases; cin >> num_test_cases;
    while (num_test_cases--){
        algorithm();
    }
}
