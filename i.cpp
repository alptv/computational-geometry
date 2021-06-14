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

    bool operator==(const point<T> &other) const {
        return x == other.x && y == other.y;
    }
};

template<typename T>
int left_turn(point<T> s, point<T> p, point<T> q) {
    int turn = (p.x - s.x) * (q.y - s.y) - (p.y - s.y) * (q.x - s.x);
    return (turn > 0) - (turn < 0);
}

template<typename T>
bool is_in_triangle(point<T> a, point<T> left, point<T> middle, point<T> right) {
    return (left_turn(left, middle, a) * left_turn(left, right, a) <= 0)
           && (left_turn(middle, left, a) * left_turn(middle, right, a) <= 0)
           && (left_turn(right, left, a) * left_turn(right, middle, a) <= 0);
}

template<typename T>
bool has_ear(std::vector<int> &stack, std::vector<point<T>> &polygon, std::vector<int> &used) {
    int size = stack.size();
    if (size < 3) {
        return false;
    }
    int left = stack[size - 3];
    int middle = stack[size - 2];
    int right = stack[size - 1];
    if (left_turn(polygon[left], polygon[middle], polygon[right]) > 0) {
        for (int i = 0; i < polygon.size(); i++) {
            if (i != left && i != right && i != middle && !used[i] &&
                is_in_triangle(polygon[i], polygon[left], polygon[middle], polygon[right])) {
                return false;
            }
        }
        return true;
    }
    return false;
}

template<typename T>
std::vector<std::pair<int, int>> triangulate(std::vector<point<T>> &polygon) {
    std::vector<std::pair<int, int>> diagonals;
    std::vector<int> used(polygon.size(), 0);
    std::vector<int> stack;
    for (int i = 0; i < polygon.size(); i++) {
        stack.push_back(i);
        while (diagonals.size() != polygon.size() - 3 && has_ear(stack, polygon, used)) {
            int size = stack.size();
            diagonals.emplace_back(stack[size - 3], stack[size - 1]);
            std::swap(stack[size - 1], stack[size - 2]);
            used[stack[size - 1]] = 1;
            stack.pop_back();
        }
    }
    return diagonals;
}

template<typename T>
bool is_clock_wise_orientation(std::vector<point<T>> &polygon) {
    long long square = 0;
    for (int i = 0; i < polygon.size(); i++) {
        int j = (i + 1) % polygon.size();
        square += (polygon[i].x * polygon[j].y - polygon[i].y * polygon[j].x);
    }
    return square < 0;

}

void solveI() {
    int n;
    std::cin >> n;
    std::vector<point<long long>> polygon;
    for (int i = 0; i < n; i++) {
        int x, y;
        std::cin >> x >> y;
        polygon.emplace_back(x, y);
    }
    bool clock_wise = is_clock_wise_orientation(polygon);
    if (clock_wise) {
        std::reverse(polygon.begin(), polygon.end());
    }
    auto diagonals = triangulate(polygon);
    for (auto diagonal : diagonals) {
        if (clock_wise) {
            std::cout << n - diagonal.first - 1 << ' ' << n - diagonal.second - 1 << '\n';
        } else {
            std::cout << diagonal.first << ' ' << diagonal.second << '\n';
        }
    }
}

int main() {
    solveI();
}