#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <iterator>

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
using namespace std;

struct poster {
    int id;
    int x; int y;
    bool is_free;
    poster(int id, int x, int y, bool is_free): id(id), x(x), y(y), is_free(is_free) {}
};

typedef multimap<int, poster, std::less<int>> MM_I;
typedef multimap<int, poster, std::greater<int>> MM_D;

typedef vector<poster> P;

inline bool collide_x(int h, int w, int x0, int y0, int x1, int y1) {
    return abs(y1-y0) * (long)w <= (long) h * abs(x1-x0);
}


void algorithm() {
    int n, m, h, w; cin >> n >> m >> h >> w;
    Program lp(CGAL::SMALLER, true, 1, false, 0);
    P free, fixed;
    for (int i = 0; i < n; i ++) {
        int x, y; cin >> x >> y;
        free.emplace_back(i, x, y, true);
    }
    for (int i = 0; i < m; i ++) {
        int x, y; cin >> x >> y;
        fixed.emplace_back(n + i, x, y, false);
    }

    MM_I sort_x_i, sort_y_i;
    MM_D sort_x_d, sort_y_d;
    for (int i = 0; i < fixed.size(); i ++) {
        sort_x_i.insert({fixed[i].x, fixed[i]});
        sort_y_i.insert({fixed[i].y, fixed[i]});
        sort_x_d.insert({fixed[i].x, fixed[i]});
        sort_y_d.insert({fixed[i].y, fixed[i]});
    }

    // add pair-wise constraint for free posters
    int con = 0;
    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < i; j ++) {
            if (collide_x(h, w, free[i].x, free[i].y, free[j].x, free[j].y)) {
                lp.set_a(i, con, w);
                lp.set_a(j, con, w);
                lp.set_b(con, 2*abs(free[i].x - free[j].x));
                con++;
            }
            else {
                lp.set_a(i, con, h);
                lp.set_a(j, con, h);
                lp.set_b(con, 2*abs(free[i].y - free[j].y));
                con++;
            }
        }
    }

    for (int i = 0; i < n; i ++) {
        int x0 = free[i].x; int y0 = free[i].y;
        MM_I::iterator x_plus_itr = sort_x_i.upper_bound(x0);
        MM_D::iterator x_minus_itr = sort_x_d.upper_bound(x0);
        MM_I::iterator y_plus_itr = sort_y_i.upper_bound(y0);
        MM_D::iterator y_minus_itr = sort_y_d.upper_bound(y0);
        while (x_plus_itr != sort_x_i.end()) {
            if (collide_x(h, w, x0, y0, x_plus_itr->second.x, x_plus_itr->second.y)) {
                // set up a constraint
                lp.set_a(i, con, w);
                lp.set_b(con, 2*abs(x_plus_itr->second.x - x0) - w);
                con++;
                break;
            }
            else x_plus_itr ++;
        }
        while (x_minus_itr != sort_x_d.end()) {
            if (collide_x(h, w, x0, y0, x_minus_itr->second.x, x_minus_itr->second.y)) {
                // set up a constraint
                lp.set_a(i, con, w);
                lp.set_b(con, 2*abs(x_minus_itr->second.x - x0) - w);
                con++;
                break;
            }
            else x_minus_itr ++;
        }
        while (y_plus_itr != sort_y_i.end()) {
            if (!collide_x(h, w, x0, y0, y_plus_itr->second.x, y_plus_itr->second.y)) {
                // set up a constraint
                lp.set_a(i, con, h);
                lp.set_b(con, 2*abs(y_plus_itr->second.y - y0) - h);
                con++;
                break;
            }
            else y_plus_itr ++;
        }
        while (y_minus_itr != sort_y_d.end()) {
            if (!collide_x(h, w, x0, y0, y_minus_itr->second.x, y_minus_itr->second.y)) {
                // set up a constraint
                lp.set_a(i, con, h);
                lp.set_b(con, 2*abs(y_minus_itr->second.y - y0) - h);
                con++;
                break;
            }
            else y_minus_itr ++;
        }

    }


    for (int i = 0; i < n; i ++) {
        lp.set_c(i, -1);
    }

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));
    assert(s.is_optimal());
    unsigned int opt_value = round(-CGAL::to_double(s.objective_value()) * (h+w) * 2);
  
    std::cout << opt_value  << "\n"; 


}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int num_test_cases; cin >> num_test_cases;
    while (num_test_cases --) {
        algorithm();
    }
}
