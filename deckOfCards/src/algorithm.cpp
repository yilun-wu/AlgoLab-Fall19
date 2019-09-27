// Deck of Cards
// Week 1

#include <algorithm>
#include <cassert>
#include <limits>
#include <deque>
#include <iostream>
#include <sstream>
#include <map>
#include <queue>
#include <set>
#include <vector>
#include <cmath>

using namespace std;

pair<int, int> algorithm (int num_cards, vector<int> array, int desired_sum) {
    int left = 0, right = 0;
    std::vector<int> sum = {array[0]};
    for (unsigned int i = 1; i < array.size(); i ++) {
        sum.push_back(sum.back() + array[i]);
    }

    int best_left = 0, best_right = 0, best_diff = numeric_limits<int>::max();
    while (right != array.size()) {
        int partial_sum = sum[right] - sum[left];
        if (abs(desired_sum - partial_sum) < best_diff) {
            best_diff = abs(desired_sum - partial_sum);
            best_left = left; best_right = right;
        }
        if ((sum[right] - sum[left]) < desired_sum) {
            right ++;
        }
        else if ((sum[right] - sum[left] > desired_sum)) {
            left ++;
        }
        else
        {
            return pair<int, int>(left, right);
        }
        
    }

    return pair<int, int> (best_left, best_right);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int test_cases;
    cin >> test_cases;
    for (unsigned int i = 0 ; i < test_cases; i ++) {
        int num_cards, desired_sum;
        vector<int> array;
        cin >> num_cards;
        cin >> desired_sum;
        for (unsigned int j = 0; j < num_cards; j ++) {
            int a; cin >> a; array.push_back(a);
        }

        pair<int, int> ret = algorithm(num_cards, array, desired_sum);
        cout << ret.first + 1 << " " << ret.second << endl;
    }
    return 0;
}
