#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Ray_2 L;
typedef K::Segment_2 S;

using namespace std;

bool algorithm(int n) {
    CGAL::set_ascii_mode(cin);
    K::Ray_2 ray;
    cin >> ray;

    bool hit = false;
    for (unsigned int i = 0; i < n; i ++) {
        K::Segment_2 seg;
        cin >> seg;
        if (CGAL::do_intersect(ray, seg)) {
            hit = true;
        }
    }
    return hit;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int n; cin >> n;
    while (n) {
        if (algorithm(n)) {
            cout << "yes\n";
        }
        else cout << "no\n";

        cin >> n;
    }
    return 0;
}
