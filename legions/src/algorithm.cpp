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


using namespace std;

void algorithm() {
    int Xs, Ys; cin >> Xs >> Ys;
    int N; cin >> N;
    const int X = 0; const int Y = 1; const int t = 2;
    Program lp (CGAL::SMALLER, false, 0, false, 0);

    for (int i = 0; i < N; i ++) {
        long a, b, c, v; cin >> a >> b >> c >> v;
        // distance >= 0
        bool pos = (a*Xs + b*Ys + c) >= 0;
        long denom = sqrt(a*a + b*b);
        lp.set_a(t, i, denom*v);
        if (pos) {
            lp.set_a(X, i, -a); lp.set_a(Y, i, -b);
            lp.set_b(i, c);
        }
        else {
            lp.set_a(X, i, a); lp.set_a(Y, i, b);
            lp.set_b(i, -c);
        }
    }
    lp.set_l(t, true, 0);
    lp.set_c(t, -1);

    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));
    CGAL::Quotient<ET> res = s.objective_value();
    int sol = (int)floor(-CGAL::to_double(res));
  
    std::cout << sol << "\n"; 
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int num_test_cases; cin >> num_test_cases;
    while (num_test_cases --) {
        algorithm();
    }
}
