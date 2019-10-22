#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

struct node {
    int id;
    int time;
    bool deactivated = false;
    node* parents[2];

    bool operator < (const node& rhs) {
        return time < rhs.time;
    } 
};

int get_deactivate_time(node& bomb) {
    int required_time;
    if (bomb.deactivated) required_time = 0;
    else {
        if (bomb.parents[0]) {
            required_time = 1 + get_deactivate_time(*bomb.parents[0]) + get_deactivate_time(*bomb.parents[1]);
        }
        else {
            // no parents
            required_time = 1;
        }
        bomb.deactivated = true;
    }
    return required_time;
}

void algorithm() {
    int num_bombs; cin >> num_bombs;
    vector<node> bombs(num_bombs);
    for (int i = 0; i < num_bombs; i ++) {
        bombs[i].id = i;
        int time; cin >> time;
        bombs[i].time = time;
    }
    
    // link parents
    for (int i = 0 ; i < (num_bombs - 1)/2; i ++) {
        bombs[i].parents[0] = &bombs[2*i + 1];
        bombs[i].parents[1] = &bombs[2*i + 2];
    }
    for (int i = (num_bombs - 1)/2; i < num_bombs; i ++) {
        bombs[i].parents[0] = nullptr;
        bombs[i].parents[1] = nullptr;
    }

    vector<size_t> sorted_idx(bombs.size());
    iota(sorted_idx.begin(), sorted_idx.end(), 0);

    // sort indexes based on comparing values in time
    sort(sorted_idx.begin(), sorted_idx.end(),
       [&bombs](size_t i1, size_t i2) {return bombs[i1].time < bombs[i2].time;});

    int time_elapsed = 0; bool fail = false;
    for (auto &idx:sorted_idx) {
        time_elapsed += get_deactivate_time(bombs[idx]);
        if (time_elapsed > bombs[idx].time) {
            fail = true; break;
        }
    }
    if (fail) cout << "no" << "\n";
    else cout << "yes" << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int num_test_cases; cin >> num_test_cases;

    while(num_test_cases--) {
        algorithm();
    }
}
