#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
using namespace std;

void parse_solution(int problem, Solution* s) {
    if (s->is_optimal()) {
        //Solution::Variable_value_iterator opt = s->variable_values_begin();
        CGAL::Quotient<ET> res = s->objective_value();

        double opt_val = CGAL::to_double(res);
        if (problem == 1) {

            cout << -ceil(opt_val) << "\n";
        }
        else if (problem ==2) {
            cout << ceil(opt_val) << "\n";
        }
    }
    else if (s->is_infeasible()) cout << "no\n";
    else if (s->is_unbounded()) cout << "unbounded\n";

    //cout << *s; 
}

void algorithm_1() {
    int a, b; cin >> a >> b;
    Program lp(CGAL::SMALLER, true, 0, false, 0);

    const int X = 0; const int Y = 1;
    lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0, 4);
    lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_b(1, a*b);
    lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1);

    lp.set_c(Y, -b); lp.set_c(X, a); lp.set_c0(0);

    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));

    parse_solution(1, &s);
}

void algorithm_2() {
    int a, b; cin >> a >> b;
    Program lp(CGAL::SMALLER, false, 0, true, 0);

    const int X = 0; const int Y = 1; const int Z = 2;
    lp.set_a(X, 0, -1); lp.set_a(Y, 0, -1); lp.set_b(0, 4);
    lp.set_a(X, 1, -4); lp.set_a(Y, 1, -2); lp.set_a(Z, 1, -1); lp.set_b(1, a*b);
    lp.set_a(X, 2, 1); lp.set_a(Y, 2, -1); lp.set_b(2, 1);

    lp.set_c(Y, b); lp.set_c(X, a); lp.set_c(Z, 1); lp.set_c0(0);

    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));

    parse_solution(2, &s);

}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);

    while (true) {
        int problem; cin >> problem;
        if (problem == 1) algorithm_1();
        else if (problem == 2) algorithm_2();
        else break;
    }
    return 0;
}

