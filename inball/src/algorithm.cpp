#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
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

void algorithm(int n, int d) {
    const int r = 0;
    Program lp (CGAL::SMALLER, false, 0, false, 0);
    lp.set_l(r, true, 0);
    lp.set_c(r, -1); // min -r
    for (int i = 0 ; i < n; i ++) {
        int sum = 0;
        for (int j = 0; j < d; j ++) {
            int aij; cin >> aij;
            sum += aij * aij;
            lp.set_a(j+1, i, aij);
        }
        int ai_norm = sqrt(sum);
        lp.set_a(r, i, ai_norm);
        int bi; cin >> bi;
        lp.set_b(i, bi);
    }
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));

    if (s.is_infeasible()) cout << "none\n";
    else if (s.is_unbounded()) cout << "inf\n";
    else if (s.is_optimal()) {
        unsigned int opt_value = floor(-CGAL::to_double(s.objective_value()));
        cout << opt_value << "\n";
    }
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    while (true) {
        int n, d; cin >> n;
        if (n == 0) break;
        else cin >> d;
        algorithm(n, d);

    }
}
