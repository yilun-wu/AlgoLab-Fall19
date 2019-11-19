#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef boost::disjoint_sets_with_storage<> Uf;

using namespace std;

struct mission {
    K::Point_2 start;
    K::Point_2 end;
    Triangulation::Vertex_handle e_start;
    Triangulation::Vertex_handle e_end;
    long s_pow;
    long e_pow;
    bool safe;

    mission(K::Point_2 start, K::Point_2 end, Triangulation & t): start(start), end(end) {
        e_start = t.nearest_vertex(start);
        e_end = t.nearest_vertex(end);

        s_pow = CGAL::squared_distance(start, e_start->point()) * 4;
        e_pow = CGAL::squared_distance(end, e_end->point()) * 4;
    }
};

struct Edge {
    Triangulation::Vertex_handle v1;
    Triangulation::Vertex_handle v2;

    long sq_dist;

    Edge(Triangulation::Vertex_handle v1, Triangulation::Vertex_handle v2): v1(v1), v2(v2) {
        sq_dist = (long)CGAL::to_double(CGAL::squared_distance(v1->point(), v2->point()));
    }

};

inline bool operator<(const Edge& e, const Edge& f) {
    return e.sq_dist < f.sq_dist;
}

long find_min_max_edge(int n, Triangulation &t, vector<Edge> & edges, vector<mission>& missions, bool take_all) {
    Uf net(n);
    long max_edge_sq = 0; 
    auto it = edges.begin();

    for(int i = 0; i < missions.size(); i ++) {
        auto m = missions[i];
        if (!m.safe && !take_all) {
            continue;
        }

        // Add edges which has not yet been added till we connect jammers
        // corresponding to start and end of the mission.
        while (it != edges.end() && (net.find_set(m.e_start->info()) !=
                                     net.find_set(m.e_end->info()))) {
            max_edge_sq = max(max_edge_sq, it->sq_dist);
            net.union_set(it->v1->info(), it->v2->info());
            it++;
        }
    }
    return max_edge_sq;
}

void algorithm() {
    int n, M; cin >> n >> M; long p; cin >> p;
    vector<pair<K::Point_2, int>> jammers;
    for (int i = 0; i < n; i ++) {
        K::Point_2 p; cin >> p; jammers.push_back(make_pair(p, i));
    }
    Triangulation t;
    t.insert(jammers.begin(), jammers.end());

    // sort edges
    vector<Edge> edges;
    for (auto it = t.finite_edges_begin(); it!= t.finite_edges_end(); it ++) {
        auto vertex_1 = it->first->vertex((it->second + 1) % 3);
        auto vertex_2 = it->first->vertex((it->second + 2) % 3);
        edges.emplace_back(vertex_1, vertex_2);
    }
    sort(edges.begin(), edges.end());

    Uf safe_net(n);

    // insert edges up to p
    for (auto e:edges) {
        if (e.sq_dist <= p) safe_net.union_set(e.v1->info(), e.v2->info());
    }

    long min_p_se_all = 0; long min_p_se_safe = 0;
    vector<mission> missions;
    for (int i = 0; i < M; i ++) {
        K::Point_2 start, end; cin >> start >> end;
        mission m(start, end, t);
        
        if (m.s_pow <= p && m.e_pow <= p && safe_net.find_set(m.e_start->info()) == safe_net.find_set(m.e_end->info())) {
            cout << "y";
            m.safe = true;
            min_p_se_safe = max(min_p_se_safe, m.s_pow); min_p_se_safe = max(min_p_se_safe, m.e_pow);
        }
        else {
            cout << "n";
            m.safe = false;
        }
        missions.push_back(m);
        min_p_se_all = max(min_p_se_all, m.s_pow); min_p_se_all = max(min_p_se_all, m.e_pow);
    }
    cout << "\n";

    long min_p_edges_safe = find_min_max_edge(n, t, edges, missions, false);
    long min_p_edges_all = find_min_max_edge(n, t, edges, missions, true);

    cout << max(min_p_se_all, min_p_edges_all) << "\n";
    cout << max(min_p_se_safe, min_p_edges_safe) << "\n";


}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int num_test_cases; cin >> num_test_cases;

    while (num_test_cases--) {
        algorithm();
    }
}
