#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <tuple>
#include <stdexcept>

using namespace std;
typedef tuple<int, int, vector<int>> IIVB;

struct drink {
    int cost;
    int vol;

    drink(int cost, int vol): cost(cost), vol(vol) {};
};

bool compare_min_cost_max_variety (const IIVB & lfs, const IIVB & rhs) {
    if (get<0>(lfs) == get<0>(rhs)) return get<1>(lfs) > get<1>(rhs);
    else return get<0>(lfs) < get<0>(rhs);
}

IIVB query(int required_volume, vector<IIVB> & table, vector<drink>& drinks) {
    if (required_volume <= 0) {
        return IIVB(0, 0, vector<int>(drinks.size(), 0));
    }
    if (get<0>(table[required_volume]) >= 0) return table[required_volume];
    else {
        IIVB best_CVB = IIVB(numeric_limits<int>::max(), -1, vector<int>(drinks.size(), 0));
        for (int i = 0; i < drinks.size(); i ++) {
            IIVB ret = query(required_volume - drinks[i].vol, table, drinks);
            get<0>(ret) += drinks[i].cost;
            get<1>(ret) += get<2>(ret)[i] ? 0 : 1;
            get<2>(ret)[i] = 1;
            if (compare_min_cost_max_variety(ret, best_CVB)) {
                best_CVB = ret;
            }
        }
        table[required_volume] = best_CVB;
        return table[required_volume];
    }
}

void algorithm() {
    int n, k; cin >> n >> k;
    int required_vol = k;
    vector<drink> drinks; drinks.reserve(n);
    for (int i = 0; i < n; i ++) {
        int c, v; cin >> c >> v;
        drinks.emplace_back(c, v);
    }

    vector<IIVB> min_cost_max_variety(required_vol + 1, IIVB(-1, -1, vector<int>(n, 0)));
    IIVB ret = query(required_vol, min_cost_max_variety, drinks);
    cout << get<0>(ret) << ' ' << get<1>(ret) << '\n';
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int num_test_cases; cin >> num_test_cases;
    while (num_test_cases --) {
        algorithm();
    }
}
