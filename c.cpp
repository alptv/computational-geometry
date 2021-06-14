#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
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
T squared_distance(point<T> a, point<T> b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

template<typename T>
class min_distance_result {
public:
    point<T> a;
    point<T> b;
    T squared_distance;
    long double distance;

    min_distance_result(point<T> a, point<T> b) : a(a), b(b) {
        squared_distance = ::squared_distance(a, b);
        distance = sqrt(static_cast<long double>(squared_distance));
    }

};

template<typename T>
min_distance_result<T> merge(min_distance_result<T> a, min_distance_result<T> b) {
    if (a.squared_distance < b.squared_distance) {
        return a;
    }
    return b;
}

template<typename T>
min_distance_result<T> min_distance_trivial(std::vector<point<T>> &dots, int l, int r) {
    assert(dots.size() >= 2);
    min_distance_result<T> result(dots[0], dots[1]);
    for (int i = l; i < r; i++) {
        for (int j = i + 1; j < r; j++) {
            result = merge(result, min_distance_result<T>(dots[i], dots[j]));
        }
    }
    return result;
}

template<typename T>
int is_close(point<T> a, T split_x, T min_squared_distance) {
    return (a.x - split_x) * (a.x - split_x) < min_squared_distance;
}

template<typename T>
min_distance_result<T> min_distance_between_dots_recursively(std::vector<point<T>> &dots, int l, int r) {
    if (r - l <= 3) {
        return min_distance_trivial(dots, l, r);
    }
    int m = (l + r) / 2;
    T split_x = dots[m].x;
    min_distance_result<T> min_result = merge(min_distance_between_dots_recursively(dots, l, m),
                                              min_distance_between_dots_recursively(dots, m, r));
    T min_squared_distance = min_result.squared_distance;

    std::sort(dots.begin() + l, dots.begin() + r, [&](const point<T> &a, const point<T> &b) -> bool {
        int close_a = is_close(a, split_x, min_squared_distance);
        int close_b = is_close(b, split_x, min_squared_distance);
        return (close_a > close_b) || (close_a == close_b && a.y < b.y);
    });
    int last = l;
    while (last < r && is_close(dots[last], split_x, min_squared_distance)) {
        last++;
    }
    for (int i = l; i < last; i++) {
        for (int j = i + 1;
             j < last && (dots[j].y - dots[i].y) * (dots[j].y - dots[i].y) < min_squared_distance; j++) {
            min_distance_result<T> current_result = min_distance_result<T>(dots[i], dots[j]);
            min_result = merge(min_result, current_result);
        }
    }
    return min_result;
}

template<typename T>
min_distance_result<T> min_distance_between_dots(std::vector<point<T>> &dots) {
    std::sort(dots.begin(), dots.end(), [](const point<T> &a, const point<T> &b) -> bool {
        return a.x < b.x;
    });
    return min_distance_between_dots_recursively(dots, 0, dots.size());
}


void solveC() {
    int n;
    std::cin >> n;
    std::vector<point<long long>> dots(n);
    for (int i = 0; i < n; i++) {
        long long x, y;
        std::cin >> x >> y;
        dots[i] = point<long long>(x, y);
    }
    min_distance_result<long long> result = min_distance_between_dots<long long>(dots);
    std::cout << result.a.x << ' ' << result.a.y << '\n';
    std::cout << result.b.x << ' ' << result.b.y << '\n';
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    solveC();
}