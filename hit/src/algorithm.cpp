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
    long int x1, y1, x2, y2;
    cin >> x1 >> y1 >> x2 >> y2;
    K::Ray_2 ray (P(x1, y1), P(x2, y2));

    bool hit = false;
    for (unsigned int i = 0; i < n; i ++) {
        cin >> x1 >> y1 >> x2 >> y2;
        K::Segment_2 seg(P(x1, y1), P(x2, y2));
        if (!hit && CGAL::do_intersect(ray, seg)) {
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
