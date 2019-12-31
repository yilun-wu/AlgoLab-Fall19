#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

int query(int s1, int s2, vector<vector<int>> & table, vector<int> &A, vector<int> &B) {
    if (table[s1][s2] >= 0) return table[s1][s2];
    //else if (s1 == 0 && s2 == 0) {table[s1][s2] = A[0]*B[0]; return table[s1][s2];}
    else if (s1 == 0) {
        int sum = accumulate(B.begin(), B.begin() + (s2+1), 0);
        table[s1][s2] = sum; return sum;
    }
    else if (s2 == 0) {
        int sum = accumulate(A.begin(), A.begin() + (s1+1), 0);
        table[s1][s2] = sum; return sum;
    }
    else {
        table[s1][s2] = A[s1] * B[s2] + min(query(s1-1, s2-1, table, A, B), 
                        min(query(s1-1, s2, table, A, B), query(s1, s2-1, table, A, B)));
        return table[s1][s2];
    }
}

void algorithm() {
    int n; cin >> n;
    vector<int> A; A.reserve(n);
    vector<int> B; B.reserve(n);
    vector<int> A_sum(n, 0);
    vector<int> B_sum(n, 0);
    for (int i = 0; i < n; i ++) {
        int a; cin >> a; --a; A.push_back(a);
        if (i==0) A_sum[0] = a;
        else A_sum[i] = A_sum[i-1] + a;
    }
    for (int i = 0; i < n; i ++) {
        int b; cin >> b; b--; B.push_back(b);
        if (i==0) B_sum[0] = b;
        else B_sum[i] = B_sum[i-1] + b;
    }
    vector<vector<int>> table(n, vector<int>(n, -1));
    for (int i = 0; i < n; i ++) {
        table[0][i] = A[0] * B_sum[i];
        table[i][0] = B[0] * A_sum[i];
    }
    for (int i = 1; i < n; i ++) {
        for (int j = 1; j < n; j ++) {
            table[i][j] = A[i]*B[j] + min(table[i-1][j-1], min(table[i][j-1], table[i-1][j]));
        }
    }
    int min_cost = table[n-1][n-1];
    cout << min_cost << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int num_test_cases; cin >> num_test_cases;

    while (num_test_cases --) {
        algorithm();
    }
}


