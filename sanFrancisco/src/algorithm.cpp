#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

struct canals {
    int u; int v; int p;
    canals(int u, int v, int p): u(u), v(v), p(p) {}
};

long query(int node, int step, vector<vector<long>> &table, vector<vector<canals>> &canals) {
    long ret;
    if (table[node][step] != -1) return table[node][step];
    else if (step == 0) ret = 0;
    else {
        int num_canals = canals[node].size();
        if (num_canals == 0) {
            if (node == 0) {
                ret = 0;
            }
            else return query(0, step, table, canals);
        }
        else {
            vector<long> intermediate_scores(num_canals);
            for (int i = 0; i < num_canals; i ++) {
                int next_node = canals[node][i].v;
                intermediate_scores[i] = canals[node][i].p + query(next_node, step - 1, table, canals);
            }
            ret = *max_element(intermediate_scores.begin(), intermediate_scores.end());
        }
    }
    table[node][step] = ret;
    return ret;
}

void algorithm() {
    int n, m; long x; int k; cin >> n >> m >> x >> k;
    vector<vector<long>> table(n, vector<long>(k+1, -1));
    vector<vector<canals>> canals(n);
    for (int i = 0; i < m; i ++) {
        int u, v, p; cin >> u >> v >> p;
        canals[u].emplace_back(u, v, p);
    }
    for (int i = 1; i <= k; i ++) {
        long max_score = query(0, i, table, canals);
        if (max_score >= x) {
            cout << i << "\n";
            return;
        }
    }
    cout << "Impossible" << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int num_test_cases; cin >> num_test_cases;

    while(num_test_cases --) {
        algorithm();
    }
}
