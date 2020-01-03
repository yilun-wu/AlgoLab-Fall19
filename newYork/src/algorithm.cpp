#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <list>
#include <set>
#include <stdexcept>
#include <cassert>

using namespace std;

struct node {
    int id;
    int temp;
    int node_start = -1;
    int parent = -1;
    vector<int> children;
    bool feasible = false;
    bool visit = false;
    node(int id, int temp): id(id), temp(temp) {};
};


void algorithm() {
    int n, m, k; cin >> n >> m >> k;
    vector<node> cpts;
    for (int i = 0; i < n; i ++) {
        int temp; cin >> temp;
        cpts.emplace_back(i, temp);
    }
    for (int i = 0; i < n-1; i ++) {
        int from, to; cin >> from >> to;
        cpts[from].children.push_back(to);
        cpts[to].parent = from;
    }
    
    for (auto &pt:cpts) {
        if (pt.children.empty()) {
            // a leaf node continue
            multiset<int> window;
            node* tail = &pt; node* head = &cpts[pt.parent];
            window.insert(tail->temp); window.insert(head->temp);
            while (true) {
                if (tail->visit) break;
                int diff = *window.rbegin() - *window.begin();
                int length = window.size();
                if (diff > k) {
                    // remove tail from window
                    tail->visit = true;
                    window.erase(window.find(tail->temp));
                    tail = &cpts[tail->parent];
                }
                else if (length < m) {
                    if (head->parent != -1) {
                        head = &cpts[head->parent];
                        window.insert(head->temp);
                    }
                    else break;
                }
                else {
                    assert(length == m);
                    head->feasible = true;
                    tail->visit = true;
                    window.erase(window.find(tail->temp));
                    tail = &cpts[tail->parent];
                }
            }
        }
    }

    bool abort = true;
    for (auto &pt:cpts) {
        if (pt.feasible) {cout << pt.id << " "; abort = false;}
    }
    if (abort) cout << "Abort mission";
    cout << "\n";

}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int num_test_cases; cin >> num_test_cases;

    while (num_test_cases --) {
        algorithm();
    }
}
