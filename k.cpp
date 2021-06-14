#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cmath>

template<typename T>
class point {
public:
    T x;
    T y;

    point() = default;

    point(T x, T y) : x(x), y(y) {}
};

template<typename T>
int left_turn(point<T> &s, point<T> from, point<T> to) {
    T cross_product = (from.x - s.x) * (to.y - s.y) - (to.x - s.x) * (from.y - s.y);
    return (cross_product > 0) - (cross_product < 0);
}

template<typename T>
void set_lowest_point_to_head(std::vector<point<T>> &points) {
    for (int i = 1; i < points.size(); i++) {
        if (points[i].y < points[0].y || (points[i].y == points[0].y && points[i].x > points[0].x)) {
            std::swap(points[i], points[0]);
        }
    }
}

template<typename T>
std::vector<point<T>> convex_hull(std::vector<point<T>> &points) {
    set_lowest_point_to_head(points);
    std::sort(points.begin() + 1, points.end(), [&](const point<T> &a, const point<T> &b) -> bool {
        point<T> start = points[0];
        int turn = left_turn(start, a, b);
        T len_a = (a.x - start.x) * (a.x - start.x) + (a.y - start.y) * (a.y - start.y);
        T len_b = (b.x - start.x) * (b.x - start.x) + (b.y - start.y) * (b.y - start.y);
        //vertical lines
        return turn > 0 || (turn == 0 && len_a < len_b);
    });
    std::vector<point<T>> hull;
    for (int i = 0; i < points.size(); i++) {
        while (hull.size() >= 2 && left_turn(hull.back(), points[i], hull[hull.size() - 2]) <= 0) {
            hull.pop_back();
        }
        hull.push_back(points[i]);
    }
    return hull;
}

template<typename T>
T det(T x1, T y1, T x2, T y2) {
    return std::abs(x1 * y2 - x2 * y1);
}

template<typename T>
T squared_len(T x1, T y1, T x2, T y2) {
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

template<typename T>
T squared_len(T x, T y) {
    return x * x + y * y;
}

template<typename T>
T max(T a, T b) {
    if (a < b) {
        return b;
    }
    return a;
}

template<typename T>
int sgn(T x) {
    return (x > 0) - (x < 0);
}

template<typename T>
T diameter(std::vector<point<T>> &points) {
    auto hull = convex_hull(points);
    int l = 0;
    for (int i = 0; i < hull.size(); i++) {
        if (hull[i].x < hull[l].x) {
            l = i;
        }
    }
    int r = 0;
    for (int i = 0; i < hull.size(); i++) {
        if (hull[i].x > hull[r].x) {
            r = i;
        }
    }
    int r_s = r;
    int l_s = l;
    T x = 0;
    T y = -1;
    T max_squared_distance = 0;
    while (true) {
        max_squared_distance = max(max_squared_distance,
                                   squared_len(hull[l].x, hull[l].y, hull[r].x, hull[r].y));
        int lnext = (l + 1) % hull.size();
        int rnext = (r + 1) % hull.size();
        T lx = (hull[lnext].x - hull[l].x);
        T ly = (hull[lnext].y - hull[l].y);
        T rx = (hull[rnext].x - hull[r].x);
        T ry = (hull[rnext].y - hull[r].y);
        long double ldet = det(x, y, lx, ly);
        long double rdet = det(-x, -y, rx, ry);
        ldet *= ldet;
        ldet /= (long double) squared_len(lx, ly);
        rdet *= rdet;
        rdet /= (long double) squared_len(rx, ry);
        long long lscal = sgn(x * lx + y * ly);
        long long rscal = sgn(-x * rx - y * ry);
        if ((lscal * rscal > 0 && ldet < rdet) || (lscal * rscal <= 0 && lscal >= rscal)) {
            if (lnext == l_s) {
                break;
            }
            x = lx;
            y = ly;
            l = lnext;
        } else {
            if (rnext == r_s) {
                break;
            }
            x = -rx;
            y = -ry;
            r = rnext;
        }
    }
    return max_squared_distance;
}

void solveK() {
    int n;
    std::cin >> n;
    std::vector<point<long long>> points;
    for (int i = 0; i < n; i++) {
        int x, y;
        std::cin >> x >> y;
        points.emplace_back(x, y);
    }
    long long max_dist = 0;
    if (n <= 2) {
        for (point<long long> p : points) {
            for (point<long long> q : points) {
                max_dist = max<long long>(max_dist, squared_len<long long>(p.x, p.y, q.x, q.y));
            }
        }
    } else {
        max_dist = diameter(points);
    }
    std::cout << std::fixed << std::setprecision(50) << std::sqrt((long double) max_dist) << '\n';
}

int main() {
    solveK();
}