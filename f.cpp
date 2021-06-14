#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

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

void print_square(long long square) {
    std::cout << square / 2;
    if (square % 2 == 1) {
        std::cout << ".5";
    }
}

void solveF() {
    int n;
    std::cin >> n;
    std::vector<point<long long>> points;
    for (int i = 0; i < n; i++) {
        int x, y;
        std::cin >> x >> y;
        points.emplace_back(x, y);
    }
    std::vector<point<long long>> hull = convex_hull(points);
    long long square = 0;
    for (int i = 0; i < hull.size(); i++) {
        point<long long> a = hull[i];
        point<long long> b = hull[(i + 1) % hull.size()];
        square += (a.x * b.y - a.y * b.x);
    }
    std::reverse(hull.begin(), hull.end());
    std::cout << hull.size() << '\n';
    for (point<long long> point : hull) {
        std::cout << point.x << ' ' << point.y << '\n';
    }
    print_square(square);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    solveF();
}
