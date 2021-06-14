#include <iostream>
#include <vector>

template<typename T>
class point {
public:
    T x;
    T y;

    point() = default;

    point(T x, T y) : x(x), y(y) {}
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
        if (left.x > right.x) {
            std::swap(left, right);
        }
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

    bool is_between_vertical_semi_interval(point<T> point) const {
        return left.x <= point.x && point.x < right.x;
    }
};

template<typename T>
std::vector<segment<T>> make_sides(std::vector<point<T>> &polygon_vertices) {
    std::vector<segment<T>> polygon_sides = {};
    for (int i = 0; i < polygon_vertices.size(); i++) {
        int j = (i + 1) % polygon_vertices.size();
        polygon_sides.push_back(segment<T>(polygon_vertices[i], polygon_vertices[j]));
    }
    return polygon_sides;
}

template<typename T>
std::vector<int> compute_left_turns(point<T> point, std::vector<segment<T>> &polygon_sides) {
    std::vector<int> left_turns;
    left_turns.reserve(polygon_sides.size());
    for (const segment<T> &side : polygon_sides) {
        left_turns.push_back(side.left_turn(point));
    }
    return left_turns;
}

template<typename T>
std::vector<int> compute_between_vertical_lines(point<T> point, std::vector<segment<T>> &polygon_sides) {
    std::vector<int> between_vertical_lines;
    between_vertical_lines.reserve(polygon_sides.size());
    for (const segment<T> &side : polygon_sides) {
        between_vertical_lines.push_back(side.is_between_vertical_semi_interval(point));
    }
    return between_vertical_lines;
}

bool is_on_side(std::vector<int> &between_vertical_lines, std::vector<int> &left_turns) {
    for (int i = 0; i < left_turns.size(); i++) {
        if (between_vertical_lines[i] && left_turns[i] == 0) {
            return true;
        }
    }
    return false;
}

int compute_sides_intersection_count(std::vector<int> &between_vertical_lines, std::vector<int> &left_turns) {
    int intersection_count = 0;
    for (int i = 0; i < left_turns.size(); i++) {
        if (between_vertical_lines[i] && left_turns[i] < 0) {
            intersection_count++;
        }
    }
    return intersection_count;
}

template<typename T>
bool is_in_polygon(point<T> point, std::vector<::point<T>> &polygon_vertices) {
    std::vector<segment<T>> polygon_sides = make_sides(polygon_vertices);
    std::vector<int> between_vertical_lines = compute_between_vertical_lines(point, polygon_sides);
    std::vector<int> left_turns = compute_left_turns(point, polygon_sides);
    if (is_on_side(between_vertical_lines, left_turns)) {
        return true;
    }
    int intersection_count = compute_sides_intersection_count(between_vertical_lines, left_turns);
    return intersection_count % 2;
}

void solveE() {
    int polygon_vertices_count;
    std::cin >> polygon_vertices_count;
    int x, y;
    std::cin >> x >> y;
    point<int> point(x, y);
    std::vector<::point<int>> polygon_vertices(polygon_vertices_count);
    for (int i = 0; i < polygon_vertices_count; i++) {
        std::cin >> x >> y;
        polygon_vertices[i] = ::point<int>(x, y);
    }
    if (is_in_polygon(point, polygon_vertices)) {
        std::cout << "YES";
    } else {
        std::cout << "NO";
    }
}
