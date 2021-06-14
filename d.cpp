#include <iostream>
#include <vector>
#include <algorithm>

template<typename T>
class point {
public:
    T x;
    T y;

    point() = default;

    point(T x, T y) : x(x), y(y) {}

    bool operator==(const point &other) {
        return x == other.x && y == other.y;
    }
};

template<typename T>
class segment {
private:
    point<T> left;
    point<T> right;
    T x;
    T y;

public:
    segment(point<T> a, point<T> b) {
        left = a;
        right = b;
        x = right.x - left.x;
        y = right.y - left.y;
    }

    segment(T x, T y) : x(x), y(y) {}

    T operator*(const segment<T> &other) const {
        return x * other.x + y * other.y;
    }

    segment<T> operator+(const segment<T> &other) const {
        return segment<T>(x + other.x, y + other.y);
    }

    T operator&(const segment<T> &other) const {
        return x * other.y - y * other.x;
    }

    int left_turn(point<T> point) const {
        T cross_product = operator&(segment<T>(left, point));
        return (cross_product > 0) - (cross_product < 0);
    }

    bool is_in_box(point<T> point) const {
        return left.x <= point.x && point.x <= right.x;
        //&& std::min(left.y, right.y) <= p.y && p.y <= std::max(left.y, right.y);
    }
};


template<typename T>
void set_left_point_to_head(std::vector<::point<T>> &polygon_vertices) {
    int left_index = 0;
    for (int i = 0; i < polygon_vertices.size(); i++) {
        if (polygon_vertices[i].x < polygon_vertices[left_index].x) {
            left_index = i;
        }
    }
    std::reverse(polygon_vertices.begin(), polygon_vertices.begin() + left_index);
    std::reverse(polygon_vertices.begin() + left_index, polygon_vertices.end());
    std::reverse(polygon_vertices.begin(), polygon_vertices.end());
}

template<typename T>
bool is_inside_convex_polygon(point<T> point, std::vector<::point<T>> &polygon_vertices) {
    int l = 0;
    int r = polygon_vertices.size();
    ::point<T> left_point = polygon_vertices[0];
    while (r - l > 1) {
        int m = (r + l) / 2;
        segment<T> diagonal(left_point, polygon_vertices[m]);
        if (diagonal.left_turn(point) <= 0) {
            l = m;
        } else {
            r = m;
        }
    }
    if (l == 0) {
        return false;
    }
    if (r == polygon_vertices.size()) {
        segment<T> diagonal(left_point, polygon_vertices[l]);
        return diagonal.is_in_box(point) && diagonal.left_turn(point) == 0;
    }
    segment<T> border = segment<T>(polygon_vertices[l], polygon_vertices[l + 1]);
    return border.left_turn(point) <= 0;
}


void solveD() {
    int n, m, k;
    std::cin >> n >> m >> k;
    std::vector<point<long long>> polygon_vertices(n);
    for (point<long long> &point : polygon_vertices) {
        std::cin >> point.x >> point.y;
    }
    std::reverse(polygon_vertices.begin(), polygon_vertices.end());
    set_left_point_to_head(polygon_vertices);
    int inside_count = 0;
    for (int i = 0; i < m; i++) {
        point<long long> p{};
        std::cin >> p.x >> p.y;
        inside_count += is_inside_convex_polygon(p, polygon_vertices);
    }
    if (inside_count >= k) {
        std::cout << "YES\n";
    } else {
        std::cout << "NO\n";
    }
}
