#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

using namespace std;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Ray_2 Ray;
typedef K::Line_2 L;
typedef K::Segment_2 Seg;
typedef K::Direction_2 Dir;

typedef long int LI;

struct BoolRay {
    int id;
    bool failed = false;
    Ray ray;
    long y0;
    Seg pos_slope_seg;

    BoolRay(int id, LI y0, LI x1, LI y1): id(id), ray(Ray(P(0, y0), P(x1, y1))), y0(y0), pos_slope_seg(P(0,0), P(x1, abs(y0-y1))) {};
};

enum result{
    NI, // not intersecting
    FF, // first ray fail
    SF // second ray fail
};

result check_cross (BoolRay r1, BoolRay r2) {
    if (CGAL::do_intersect(r1.ray, r2.ray)) {
        CGAL::Comparison_result ret = CGAL::compare_slope<K>(r1.pos_slope_seg, r2.pos_slope_seg);
        if ( ret == CGAL::Comparison_result::SMALLER) return result::SF;
        else if (ret == CGAL::Comparison_result::LARGER) return result::FF;
        else {
            if (r1.y0 > r2.y0) return result::FF;
            else return result::SF;
        }
    }
    else return result::NI;
}

bool compare_y0(BoolRay r1, BoolRay r2) {
    return r1.y0 > r2.y0;
}

void algorithm () {
    int num_bikes; cin >> num_bikes;
    vector<BoolRay> trails; trails.reserve(num_bikes);
    for (int i = 0; i < num_bikes; i ++) {
        LI y0, x1, y1; cin >> y0 >> x1 >> y1;
        trails.push_back(BoolRay(i, y0, x1, y1));
    }

    //sort by y0
    sort(trails.begin(), trails.end(), compare_y0);

    for (auto it = trails.begin() + 1; it != trails.end(); it++) {
        auto comp_it = it - 1;
        while (comp_it >= trails.begin()) {
            if (!comp_it->failed) {
                result sub_ret = check_cross(*it, *comp_it);
                if (sub_ret == result::NI) break;
                else if (sub_ret == result::FF) {it->failed = true; break;}
                else comp_it->failed = true;
            }
            comp_it -- ;
        }
    }
    
    vector<int> passed_trails;
    for (int i = 0; i < num_bikes; i ++) {
        if (!trails[i].failed) passed_trails.push_back(trails[i].id);
    }
    sort(passed_trails.begin(), passed_trails.end());
    for (auto&n: passed_trails) cout << n << " ";
    cout << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int num_test_cases; cin >> num_test_cases;

    while (num_test_cases -- ) {
        algorithm();
    }

    return 0;
}
