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

typedef multimap<int, poster, std::less<int>> MM;


struct less_x
{
    inline bool operator() (const poster& struct1, const poster& struct2)
    {
        return (struct1.x < struct2.x);
    }
};
struct less_y
{
    inline bool operator() (const poster& struct1, const poster& struct2)
    {
        return (struct1.y < struct2.y);
    }
};


typedef vector<poster> P;

inline bool collide_x(int h, int w, int x0, int y0, int x1, int y1) {
    return abs(y1-y0) * w <= h * abs(x1-x0);
}

inline bool less_a_max(pair<int, int> a1, pair<int, int> a2) {
    return a1.second < a2.second;
}

pair<size_t, bool> query_tightest_constraint(int i, P &posters, MM &sort_x, MM &sort_y, int h, int w) {
    int a_max_lx, a_max_ux, a_max_ly, a_max_uy;
    //find lower_x, upper_x
    MM::iterator search = sort_x.find(posters[i].x);
    while (search->second.id != i) search ++;
    MM::iterator lower_x_idx, upper_x_idx = search; lower_x_idx --; upper_x_idx ++;
    while (distance(lower_x_idx, sort_x.begin()) >= 0 && !collide_x(h, w, search->second.x, search->second.y, lower_x_idx->second.x, lower_x_idx->second.y)) {
        lower_x_idx --;
    }
    if (lower_x_idx == --sort_x.begin()) a_max_lx = numeric_limits<int>::max();
    else a_max_lx = abs(search->second.x - lower_x_idx->second.x)*2/w - 1;
    while (upper_x_idx != sort_x.end() && !collide_x(h, w, search->second.x, search->second.y, upper_x_idx->second.x, upper_x_idx->second.y)) {
        upper_x_idx ++;
    }
    if (upper_x_idx == sort_x.end()) a_max_ux = numeric_limits<int>::max();
    else a_max_ux = abs(search->second.x - upper_x_idx->second.x)*2/w - 1;

    //find lower_y, upper_y
    search = sort_y.find(posters[i].y);
    MM::iterator lower_y_idx, upper_y_idx = search; lower_y_idx --; upper_y_idx ++;
    while (distance(lower_y_idx, sort_y.begin()) >= 0 && collide_x(h, w, search->second.x, search->second.y, lower_x_idx->second.x, lower_x_idx->second.y)) {
        lower_y_idx --;
    }
    if (lower_y_idx == --sort_y.begin()) a_max_ly = numeric_limits<int>::max();
    else a_max_ly = abs(search->second.y - lower_y_idx->second.y)*2/h - 1;
    while (upper_y_idx != sort_y.end() && collide_x(h, w, search->second.x, search->second.y, upper_x_idx->second.x, upper_x_idx->second.y)) {
        upper_y_idx ++;
    }
    if (upper_y_idx == sort_y.end()) a_max_uy = numeric_limits<int>::max();
    else a_max_ly = abs(search->second.y - upper_y_idx->second.y)*2/h - 1;
    vector<pair<int, int>> a_max;
    a_max.emplace_back(lower_x_idx->second.id, a_max_lx);
    a_max.emplace_back(upper_x_idx->second.id, a_max_ux);
    a_max.emplace_back(lower_y_idx->second.id, a_max_ly);
    a_max.emplace_back(upper_y_idx->second.id, a_max_uy);
    return pair<size_t, bool>(min_element(a_max.begin(), a_max.end(), less_a_max)->first, 
                            min_element(a_max.begin(), a_max.end(), less_a_max) - a_max.begin() <= 2);
}

void algorithm() {
    int n, m, h, w; cin >> n >> m >> h >> w;
    Program lp(CGAL::SMALLER, true, 1, false, 0);
    P posters;
    for (int i = 0; i < n; i ++) {
        int x, y; cin >> x >> y;
        posters.emplace_back(i, x, y, true);
    }
    for (int i = 0; i < m; i ++) {
        int x, y; cin >> x >> y;
        posters.emplace_back(n + i, x, y, false);
    }

    MM sort_x, sort_y;
    for (int i = 0; i < posters.size(); i ++) {
        sort_x.insert({posters[i].x, posters[i]});
        sort_y.insert({posters[i].y, posters[i]});
    }



    // vector<size_t> sort_x_idx(posters.size()); iota(sort_x_idx.begin(), sort_x_idx.end(), 0);
    // vector<size_t> sort_y_idx(posters.size()); iota(sort_y_idx.begin(), sort_y_idx.end(), 0);

    // sort(sort_x_idx.begin(), sort_x_idx.end(), 
    //     [&posters](size_t i1, size_t i2) {return posters[i1].x < posters[i2].x;});
    // sort(sort_y_idx.begin(), sort_y_idx.end(), 
    //     [&posters](size_t i1, size_t i2) {return posters[i1].y < posters[i2].y;});

    for (int i = 0; i < n; i ++) {
        lp.set_c(i, 1);
        //find closest contraint for each free point
        pair<size_t, bool> ret = query_tightest_constraint(i, posters, sort_x, sort_y, h, w);
        size_t idx = ret.first; bool dir = ret.second;
        if (dir) {
            lp.set_a(i, i, w/2);
            if (posters[idx].is_free) {
                lp.set_a(idx, i, w/2);
                lp.set_b(i, abs(posters[i].x - posters[idx].x));
            }
            else {
                lp.set_b(i, abs(posters[i].x - posters[idx].x) - w/2);
            }
        }
        else {
            lp.set_a(i, i, h/2);
            if (posters[idx].is_free) {
                lp.set_a(idx, i, h/2);
                lp.set_b(i, abs(posters[i].y - posters[idx].y));
            }
            else {
                lp.set_b(i, abs(posters[i].y - posters[idx].y) - h/2);
            }
        }
    }

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));
  
  
    std::cout << s; 


}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int num_test_cases; cin >> num_test_cases;
    while (num_test_cases --) {
        algorithm();
    }
}
