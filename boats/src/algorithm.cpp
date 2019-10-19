#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;


struct boat {
    int pos_ring;
    int length;
    int earliest_end;

    boat(int pos_ring, int length): pos_ring(pos_ring), length(length) {
        earliest_end = pos_ring;
    };

    bool operator< (const boat& other) const {
        if(earliest_end == other.earliest_end) {
            return pos_ring > other.pos_ring;
        }
        return earliest_end > other.earliest_end;
    }
};

typedef priority_queue<boat> boats;

int algorithm(boats & boats) {
    int picked_boats = 0;
    int free_pos = -numeric_limits<int>::min();
    while(!boats.empty()) {
        boat it = boats.top();
        boats.pop();
        if (it.earliest_end - it.length >= free_pos) {
            // great, take it
            free_pos = it.earliest_end; picked_boats ++;
        }
        else {
            if (it.pos_ring >= free_pos) {
                it.earliest_end = free_pos + it.length;
                boats.push(it);
            }
        }
    }
    return picked_boats;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int num_test_cases; cin >> num_test_cases;
    while (num_test_cases --) {
        int num_boats; cin >> num_boats;
        boats boats;
        for (int i = 0; i < num_boats; i ++) {
            int a, b; cin >> a >> b;
            boats.push(boat(b, a));
        }
        cout << algorithm(boats) << "\n";
    }
    return 0;
}
