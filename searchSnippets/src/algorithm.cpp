// Search Snippets
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

int algorithm (vector<vector<int>> pos_words, int num_words) {
    int shortest_length = numeric_limits<int>::max();
    int left = 0, right = 0;
    map<int, int> head = {};
    for (unsigned int i = 0; i < num_words; i ++) {
        head.insert({pos_words[i][0], i});
    }
    
    while (true) {
        left = head.begin()->first;
        right = (--head.end())->first;
        if ((right - left + 1) < shortest_length) shortest_length = right - left + 1;
        
        pos_words[head.begin()->second].erase(pos_words[head.begin()->second].begin());
        if (pos_words[head.begin()->second].size() > 0) {
            head.insert({pos_words[head.begin()->second][0], head.begin()->second});
            head.erase(head.begin());
        }
        else {
            return shortest_length;
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    cin.tie(0);
    int num_test_cases;
    cin >> num_test_cases;

    for (unsigned int i = 0; i < num_test_cases; i ++) {
        std::vector<std::vector<int>> pos_words;
        int num_words;
        cin >> num_words;
        std::vector<int> num_occurances;
        num_occurances.reserve(num_words);
        int occur;
        for (unsigned int j = 0; j < num_words; j ++) {
            cin >> occur;
            num_occurances.push_back(occur);
        }
        for (unsigned int j = 0; j < num_words; j ++) {
            std::vector<int> pos_word;
            pos_word.reserve(num_occurances[j]);
            int pos;
            for (unsigned int k = 0; k < num_occurances[j]; k ++) {
                cin >> pos; pos_word.push_back(pos);
            }
            pos_words.push_back(pos_word);
        }
        cout << algorithm(pos_words, num_words) << endl;
    }

    return 0;
}
