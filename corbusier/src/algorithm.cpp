#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

int n, I, k;

int query_dp(int i, int e, vector<vector<int>> &table, vector<int> & nums) {
    if (table[e][i] != -1) return table[e][i];
    else {
        if (e == 0) { // only 1 element
            if (nums[e] % k == i) table[e][i] = 1;
            else table[e][i] = 0;
        }
        else {
            int mod = nums[e] % k;
            int remainder_effective = (i - mod >= 0) ? i - mod : k + i - mod;
            table[e][i] = query_dp(i, e-1, table, nums) || query_dp(remainder_effective, e-1, table, nums);
        }
        return table[e][i];
    }
}

void algorithm() {
    cin >> n >> I >> k;
    vector<vector<int>> table(n, vector<int>(k, -1));

    vector<int> nums; nums.reserve(n);
    for (int i = 0; i < n; i ++) {
        int num; cin >> num;
        nums.push_back(num);
    }

    if (query_dp(I, n-1, table, nums) == 1) cout << "yes" << "\n";
    else cout << "no" << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int num_test_cases; cin >> num_test_cases;

    while (num_test_cases--) {
        algorithm();
    }
}
