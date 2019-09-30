#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>


using namespace std;

int query (int i, int j, int num_pax, int num_sov, int k, vector<vector<int>> & table, vector<int> &sov) {
    if (table[i][j] != -1) return table[i][j];
    else {
        int curr_length = j - i + 1;
        int ret;
        if ((num_sov - curr_length) % num_pax != k) {
            int turns_avail;
            if ((num_sov - curr_length) % num_pax < k) {
                turns_avail = k - (num_sov - curr_length) % num_pax;
            }
            else {
                turns_avail = num_pax - (num_sov - curr_length) % num_pax + k;
            }
            if (curr_length <= turns_avail) ret = 0;
            else ret = min(query(i+1, j, num_pax, num_sov, k, table, sov),
                        query(i, j-1, num_pax, num_sov, k, table, sov));
        }
        else {
            if (curr_length == 1) ret = sov[i];
            else ret = max(sov[i] + query(i+1, j, num_pax, num_sov, k, table, sov),
                      sov[j] + query(i, j-1, num_pax, num_sov, k, table, sov));
        }
        table[i][j] = ret;
        return ret;
    }
}

int algorithm(int num_sov, int num_pax, int k, vector<int> sov) {
    vector<vector<int>> table(num_sov, vector<int>(num_sov, -1));
    int i = 0, j = num_sov - 1; // i, j - index of first, last element of window
    
    return query(i, j, num_pax, num_sov, k, table, sov);
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int num_test_cases; cin >> num_test_cases;
    for (unsigned int i = 0; i < num_test_cases; i ++) {
        int num_sov, num_pax, k;
        cin >> num_sov; cin >> num_pax; cin >> k;
        vector<int> sov;
        sov.reserve(num_sov);
        for (unsigned int j = 0; j < num_sov; j ++) {
            int temp; cin >> temp;
            sov.push_back(temp);
        }
        cout << algorithm(num_sov, num_pax, k, sov) << endl;
    }
    return 0;
}
