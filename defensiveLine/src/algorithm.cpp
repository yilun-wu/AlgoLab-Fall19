#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

void algorithm() {
    int n, m, k; cin >> n >> m >> k;
    vector<int> S(n+1, 0);
    for (int i = 1; i <= n; i++) {
        int d; cin >> d;
        S[i] = S[i-1] + d;
    }
    int i1 = 0; int i2 = 1;
    vector<vector<int>> match(n);
    while (i1 <= n-1 && i2 <= n) {
        int sum = S[i2] - S[i1];
        if (sum < k) i2++;
        else if (sum > k) i1++;
        else {
            /*if ((i2-i1) > 1)*/ match[i1].push_back(i2); // length = i2 - i1 with next segment starts at i2
            i1++;
        }
    }

    vector<vector<int>> table(n+1, vector<int>(m+1, -1));
    for (int i = 0; i <= n; i ++) table[i][0] = 0;
    //for (int i = 0; i < m+1; i ++) table[n][i] = -1;
    for (int i = n-2; i >= 0; i --) {
        for (int j = 1; j <= m; j ++) {
            int s_max = -1;
            for (int s = 0; s < match[i].size(); s ++) {
                int length = match[i][s] - i;
                if (i+length <= n && table[i+length][j-1] >= 0) {
                    s_max = max(s_max, length + table[i+length][j-1]);
                }
            }
            table[i][j] = max(s_max, table[i+1][j]);
        }
    }
    int num_def = table[0][m];
    if (num_def == -1) cout << "fail" << "\n";
    else cout << num_def << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int num_test_cases; cin >> num_test_cases;
    while (num_test_cases--) {
        algorithm();
    }
}
