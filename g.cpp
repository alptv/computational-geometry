#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <cassert>


template<typename T>
class point {
public:
    T x;
    T y;

    point() = default;

    point(T x, T y) : x(x), y(y) {}

    friend std::istream &operator>>(std::istream &is, point<T> &p) {
        is >> p.x >> p.y;
        return is;
    }

    friend point<T> operator+(const point<T> &a, const point<T> &b) {
        return {a.x + b.x, a.y + b.y};
    }

    friend point<T> operator-(const point<T> &a, const point<T> &b) {
        return {a.x - b.x, a.y - b.y};
    }
};

template<typename T>
T min(T x, T y) {
    if (x < y) {
        return x;
    }
    return y;
}

template<typename T>
T sgn(T x) {
    return (x > 0) - (x < 0);
}

template<typename T>
T cross_product(point<T> p, point<T> q, point<T> a, point<T> b) {
    T turn = (q.x - p.x) * (b.y - a.y) - (b.x - a.x) * (q.y - p.y);
    return turn;
}

template<typename T>
T scalar_product(point<T> &a, point<T> &p, point<T> &q) {
    return (q.x - p.x) * (a.x - p.x) + (q.y - p.y) * (a.y - p.y);
}

template<typename T>
T squared_distance(point<T> &a, point<T> &b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

template<typename T>
long double distance(point<T> &a, point<T> &b) {
    return std::sqrt(squared_distance(a, b));
}

template<typename T>
long double distance_to_line(point<T> &a, point<T> &p, point<T> &q) {
    return std::abs(cross_product(p, a, p, q)) / distance(p, q);
}

template<typename T>
long double distance_to_segment(point<T> &a, point<T> &p, point<T> &q) {
    T projection = scalar_product(a, p, q);
    if (projection < 0) {
        return distance(a, p);
    }
    if (projection > squared_distance(p, q)) {
        return distance(a, q);
    }
    return distance_to_line(a, p, q);
}
// distances end


template<typename T>
void set_lowest_point_to_head(std::vector<point<T>> &points) {
    int position = 0;
    for (int i = 1; i < points.size(); i++) {
        if (points[i].x < points[position].x || (points[i].x == points[position].x && points[i].y < points[position].y)) {
            position = i;
        }
    }
    std::reverse(points.begin() + position, points.end());
    std::reverse(points.begin(), points.begin() + position);
    std::reverse(points.begin(), points.end());
}

template<typename T>
long double polygons_distance(std::vector<point<T>> p, std::vector<point<T>> q) {
    point<T> start = {0, 0};
    set_lowest_point_to_head(p);
    for (point<T> &vertex: q) {
        vertex.x *= -1;
        vertex.y *= -1;
    }
    set_lowest_point_to_head(q);
    p.push_back(p[0]);
    q.push_back(q[0]);
    point<T> cur = p[0] + q[0];
    long double distance = std::sqrt(cur.x * cur.x + cur.y * cur.y);
    int left = 1;
    int right = 1;
    while (left < p.size() && right < q.size()) {
        int turn = sgn(cross_product(p[left - 1], p[left], q[right - 1], q[right]));
        point<T> prev = cur;
        if (turn >= 0) {
            cur = cur + p[left] - p[left - 1];
            left++;
        } else {
            cur = cur + q[right] - q[right - 1];
            right++;
        }
        distance = min(distance, distance_to_segment(start, cur, prev));
    }
    while (left < p.size()) {
        point<T> prev = cur;
        cur = cur + p[left] - p[left - 1];
        distance = min(distance, distance_to_segment(start, cur, prev));
        left++;
    }
    while (right < q.size()) {
        point<T> prev = cur;
        cur = cur + q[right] - q[right - 1];
        distance = min(distance, distance_to_segment(start, cur, prev));
        right++;
    }
    return distance;
}

template<typename T>
std::vector<point<T>> read_polygon() {
    int n;
    std::cin >> n;
    assert(n > 2);
    std::vector<point<T>> polygon(n);
    for (int i = 0; i < n; i++) {
        std::cin >> polygon[i];
    }
    std::reverse(polygon.begin(), polygon.end());
    return polygon;
}

void solveG() {
    auto p = read_polygon<long long>();
    auto q = read_polygon<long long>();
    std::cout << std::fixed << std::setprecision(30) << polygons_distance(p, q);
}

int main() {
    solveG();
}
