// Even pairs
// Week 1
// Aleksander Matusiak

#include <algorithm>
#include <cassert>
#include <climits>
#include <deque>
#include <iostream>
#include <sstream>
#include <map>
#include <queue>
#include <set>
#include <vector>

#define REP(i, n) for (int i = 0; i < (n); ++i)
#define ST first
#define ND second
#define PB push_back
#define MP make_pair
#define deb(a) a

using namespace std;

typedef pair<int, int> PII;
typedef vector<PII> VPII;
typedef vector<int> VI;
typedef long long int LL;

void do_test() {
    int n;
    cin >> n;

    int sum = 0, res = 0;
    // Counters for number of odd and even prefix sums.
    int odds = 0, evens = 1;
    REP(i, n) {
        int a;
        cin >> a;
        sum += a;
        // We are relying on the fact that sum of each interval is equal to
        // the difference between appropriate prefix sums.
        if (sum % 2) {
            res += odds;
            odds++;
        } else {
            res += evens;
            evens++;
        }
    }

    cout << res << "\n";
}


pair<int, int> algorithm (int num_cards, vector<int> array, int desired_sum) {
    cout << 12 << endl;
    return pair<int, int> (0,0);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int test_cases;
    cin >> test_cases;
    for (unsigned int i = 0 ; i < test_cases; i ++) {
        int num_cards, desired_sum;
        vector<int> array = {1};
        cin >> num_cards;
        cin >> desired_sum;
        string line;
        getline(cin, line);
        istringstream iss(line);
        int a;
        while (iss >> a) {
            array.push_back(a);
        }
        algorithm(num_cards, array, desired_sum);
    }
    return 0;
}
