#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

struct node {
    vector<node*> children;
    //int parent;
    int cost;
    // off: minimum cost covering subtree with node turned off (not paying)
    // on: minimum cost covering subtree with node turned on (paying)
    // ex: minimum cost covering upper level tree subtree (pay or not pay)
    int subcost_off = -1, subcost_on = -1, subcost_ex = -1;

};

pair<int, int> get_cost(node & city) {
    if (city.children.empty()) {
        city.subcost_off = numeric_limits<int>::max();
        city.subcost_on = city.cost;
        city.subcost_ex = 0;
    }
    else {
        pair<int, int> child_subcost;
        vector<int> children_subcost_off;
        vector<int> children_subcost_on;
        vector<int> optimal_children_subcost;
        bool child_all_off = true;
        for (int child_id = 0 ; child_id < city.children.size(); child_id ++) {
            child_subcost = get_cost(*city.children[child_id]);
            children_subcost_off.push_back(child_subcost.first);
            children_subcost_on.push_back(child_subcost.second);
            if (child_subcost.first < child_subcost.second) {
                optimal_children_subcost.push_back(child_subcost.first);
            }
            else {
                optimal_children_subcost.push_back(child_subcost.second);
                child_all_off = false;
            }
        }
        city.subcost_ex = 0; for (auto& n : optimal_children_subcost) city.subcost_ex += n;

        city.subcost_off = city.subcost_ex;
        if (child_all_off) {
            vector<int> children_subcost_diff;
            for (int i = 0; i < children_subcost_off.size(); i ++) {
                children_subcost_diff.push_back(children_subcost_on[i] - children_subcost_off[i]);
            }
            city.subcost_off += *min_element(children_subcost_diff.begin(), children_subcost_diff.end());
        } 

        city.subcost_on = city.cost;
        for (int child_id = 0; child_id < city.children.size(); child_id ++) {
            city.subcost_on += min(min((*city.children[child_id]).subcost_on, (*city.children[child_id]).subcost_off), (*city.children[child_id]).subcost_ex);
        }
    }
    
    return pair<int, int> (city.subcost_off, city.subcost_on);
}

int algorithm (int num_cities) {
    // build tree
    vector<node> city;
    city.resize (num_cities);
    for (int i = 0 ; i < num_cities - 1; i ++ ) {
        int a, b; cin >> a >> b;
        city[a].children.push_back(&city[b]);
    }
    for (int i = 0; i < num_cities; i ++) {
        int cost; cin >> cost;
        city[i].cost = cost;
    }

    pair<int, int> cost = get_cost(city[0]);

    return min(cost.first, cost.second);

}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int num_test_cases;
    cin >> num_test_cases;
    for (int i = 0; i < num_test_cases; i ++) {
        int num_cities; cin >> num_cities;
        cout << algorithm(num_cities) << endl;
        
    }
    return 0;
}
