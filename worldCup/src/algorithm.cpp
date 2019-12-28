#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <cmath>
#include <stdexcept>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;


using namespace std;

struct warehouse {
    int id;
    int x, y;
    K::Point_2 p;
    int supply;
    int a_per;
    warehouse(int id, int x, int y, int supply, int a_per):
        id(id), p(x, y), supply(supply), a_per(a_per){}
};

struct stadium {
    int id;
    K::Point_2 p;
    int demand;
    int a_lim;
    stadium(int id, int x, int y, int demand, int a_lim):
        id(id), p(x, y), demand(demand), a_lim(a_lim){}
};

struct contour {
    K::Point_2 p;
    int r;
    contour(int x, int y, int r): p(x, y), r(r) {}
};


void algorithm() {
    int n, m, c; cin >> n >> m >> c;
    vector<warehouse> W; W.reserve(n);
    for (int i = 0; i < n; i ++) {
        int x, y, S, per; cin >> x >> y >> S >> per;
        W.emplace_back(i, x, y, S, per);
    }
    vector<stadium> S; S.reserve(m);
    for (int i = 0; i < m; i ++) {
        int x, y, D, lim; cin >> x >> y >> D >> lim;
        S.emplace_back(i, x, y, D, lim);
    }
    vector<vector<pair<int, int>>> profit(n, vector<pair<int, int>>(m, make_pair(0, 0)));
    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < m; j ++) {
            int p; cin >> p;
            profit[i][j].first = p;
        }
    }
    vector<contour> C; C.reserve(c);
    for (int i = 0; i < c; i ++) {
        int x, y, r; cin >> x >> y >> r;
        C.emplace_back(x, y, r);
    }

    vector<vector<bool>> IO_W(n, vector<bool>(c, false)); // IN true, OUT false
    vector<vector<bool>> IO_S(m, vector<bool>(c, false));
    Triangulation t;
    for (auto &w:W) t.insert(w.p);
    for (auto &s:S) t.insert(s.p);

    for (int k = 0; k < c; k ++) {
        Triangulation::Vertex_handle e = t.nearest_vertex(C[k].p);
        if (CGAL::squared_distance(e->point(), C[k].p) >= C[k].r * C[k].r) {
            continue;
        }
        else {
            for (int i = 0; i < n; i ++) {
                if (CGAL::squared_distance(W[i].p, C[k].p) <= C[k].r * C[k].r) {
                    IO_W[i][k] = true;
                }
            }
            for (int i = 0; i < m; i ++) {
                if (CGAL::squared_distance(S[i].p, C[k].p) <= C[k].r * C[k].r) {
                    IO_S[i][k] = true;
                }
            }
            for (int i = 0; i < n; i ++) {
                for (int j = 0; j < m; j ++) {
                    profit[i][j].second += IO_W[i][k] ^ IO_S[j][k];
                }
            }
        }
    }


    //LP 
    Program lp (CGAL::SMALLER, true, 0, false, 0);
    // variables n*m litres to be transported
    int c_base = 0;
    // constraint: Supply - sum(1*x) < di
    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < m; j ++) {
            lp.set_a(i*m+j, i, 1);
        }
        lp.set_b(i, W[i].supply);
    }
    c_base += n;

    // constraint: Demand Equality
    for (int j = 0; j < m; j ++) {
        for (int i = 0; i < n; i ++) {
            lp.set_a(i*m+j, c_base+j, 1);
        }
        lp.set_b(c_base + j, S[j].demand);
        lp.set_r(c_base + j, CGAL::EQUAL);
    }
    c_base += m;
    
    // constraint: Alcohol Upper Limit
    for (int j = 0; j < m; j ++) {
        for (int i = 0; i < n; i ++) {
            lp.set_a(i*m+j, c_base+j, W[i].a_per);
        }
        lp.set_b(c_base + j, 100*S[j].a_lim);
    }

    // objective
    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < m; j ++) {
            lp.set_c(i*m+j, - profit[i][j].first*100 + profit[i][j].second);
        }
    }

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_nonnegative_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));
    
    // output solution
    if (s.is_infeasible()) cout << "RIOT!\n";
    else {
        CGAL::Quotient<ET> res = -s.objective_value()/100.0;
        int sol = (int)floor(CGAL::to_double(res));
        std::cout << sol << "\n"; 
    }

}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int num_test_cases; cin >> num_test_cases;

    while (num_test_cases --) {
        algorithm();
    }
}
