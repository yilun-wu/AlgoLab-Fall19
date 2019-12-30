#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <iomanip>
#include <stdexcept>

using namespace std;

double query(vector<vector<double>> & table, vector<double> & p, int n, int m, int target, int max_n) {
    if (m > target) return 1.0;
    else if (table[n][m] >= 0) return table[n][m];
    else if (n == max_n) {double prob = (m >= target ? 1.0 : 0.0); table[n][m] = prob; return prob;}
    else {
        double prob;
        if (m >= target) prob = 1.0;
        else if (m == 0) prob = 0.0;
        else {
            vector<double> prob_candidates;

            for (int bet = 0; bet <= m; bet ++) {
                prob = max(prob, p[n] * query(table, p, n+1, m+bet, target, max_n) +
                                    (1-p[n]) * query(table, p, n+1, m-bet, target, max_n));
            }
        }
        table[n][m] = prob;
        return prob;
    }
}

void algorithm() {
    int n, k, m; cin >> n >> k >> m;
    vector<double> p;
    for (int i = 0; i < n; i ++) {
        double prob; cin >> prob;
        p.push_back(prob);
    }
    vector<vector<double>> table(n+1, vector<double>(m+1, -1));

    double max_prob = query(table, p, 0, k, m, n);
    cout << max_prob << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(5);
    int num_test_cases; cin >> num_test_cases;

    while (num_test_cases--) {
        algorithm();
    }
}
