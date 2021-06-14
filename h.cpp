#include <iostream>
#include <set>
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
class segment {
public:

    T x;
    T y;
    point<T> left;
    point<T> right;

    segment(point<T> a, point<T> b) {
        left = a;
        right = b;
        if (left.x > right.x) {
            std::swap(left, right);
        }
        x = right.x - left.x;
        y = right.y - left.y;
    }

    segment(T x, T y) : x(x), y(y) {
        left = {0, 0};
        right = {x, y};
    }

    segment() = default;


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
        T cross_product = operator&(segment<T>(point.x - left.x, point.y - left.y));
        return (cross_product > 0) - (cross_product < 0);
    }

    bool operator<(const segment<T> &other) const {
        if (is_vertical() && other.is_vertical()) {
            return left.y < other.left.y;
        }
        int turn = (left.x < other.left.x || other.is_vertical()) ? left_turn(other.left) : -1 * other.left_turn(left);
        return turn > 0;
    }

    bool is_vertical() const {
        return left.x == right.x;
    }
};


template<typename T>
class event {
private:
    int type;
public:
    segment<T> seg;

    explicit event(::segment<T> &segment, int type) : seg(segment), type(type) {
    }

    bool operator<(const event<T> &other) const {
        return (get_point().x < other.get_point().x) || (get_point().x == other.get_point().x
                                                         && type < other.type);
    }

    point<T> get_point() const {
        if (is_open()) {
            return seg.left;
        }
        return seg.right;
    }

    bool is_open() const {
        return type == 0;
    }


};

template<typename T>
event<T> close_event(segment<T> &segment) {
    return event<T>(segment, 1);
}

template<typename T>
event<T> open_event(segment<T> &segment) {
    return event<T>(segment, 0);
}

template<typename T>
class process_result {
public:
    bool intersect;
    segment<T> a;
    segment<T> b;

    process_result(segment<T> a, segment<T> b) : a(a), b(b) {
        intersect = true;
    }

    process_result() {
        intersect = false;
    }
};

template<typename T>
process_result<T> process_open_segment(segment<T> &segment, std::set<::segment<T>> &open_segments) {
    auto upper = open_segments.upper_bound(segment);
    if (upper != open_segments.end() && is_intersect(segment, *upper)) {
        return process_result<T>(segment, *upper);
    }
    if (upper != open_segments.begin() && is_intersect(segment, *(std::prev(upper)))) {
        return process_result<T>(segment, *(std::prev(upper)));
    }
    open_segments.insert(segment);
    return process_result<T>();
}

template<typename T>
process_result<T> process_close_segment(segment<T> &segment, std::set<::segment<T>> &open_segments) {
    open_segments.erase(segment);
    auto upper = open_segments.upper_bound(segment);
    if (upper == open_segments.end() || upper == open_segments.begin()) {
        return process_result<T>();
    }
    if (is_intersect(*upper, *std::prev(upper))) {
        return process_result<T>(*upper, *std::prev(upper));
    }
    return process_result<T>();
}

template<typename T>
bool is_intersect(segment<T> a, segment<T> b) {
    return a.left_turn(b.left) * a.left_turn(b.right) <= 0
           && b.left_turn(a.left) * b.left_turn(a.right) <= 0
           && std::max(a.left.x, b.left.x) <= std::min(a.right.x, b.right.x)
           && std::max(std::min(a.left.y, a.right.y), std::min(b.left.y, b.right.y)) <=
              std::min(std::max(a.left.y, a.right.y), std::max(b.left.y, b.right.y));
}


template<typename T>
process_result<T> find_intersection(std::vector<segment<T>> segments) {
    std::vector<event<T>> events;
    for (segment<T> segment : segments) {
        events.push_back(close_event(segment));
        events.push_back(open_event(segment));
    }
    std::sort(events.begin(), events.end());
    std::set<segment<T>> open_segments;
    for (event<T> event : events) {
        process_result<T> result = process_result<T>();
        if (event.is_open()) {
            result = process_open_segment(event.seg, open_segments);
        } else {
            result = process_close_segment(event.seg, open_segments);
        }
        if (result.intersect) {
            return result;
        }
    }
    return process_result<T>();
}

template<typename T>
bool is_same(segment<T> a, segment<T> b) {
    return a.left == b.left && a.right == b.right;
}

void solveH() {
    int n;
    std::cin >> n;
    std::vector<segment<int>> segments;
    for (int i = 0; i < n; i++) {
        int x, y;
        std::cin >> x >> y;
        point<int> p(x, y);
        std::cin >> x >> y;
        point<int> q(x, y);
        segments.emplace_back(p, q);
    }
    auto result = find_intersection<int>(segments);
    if (result.intersect) {
        std::cout << "YES\n";
        int i, j = 0;
        for (int k = 0; k < segments.size(); k++) {
            if (is_same(result.a, segments[k])) {
                i = k;
                break;
            }
        }
        for (int k = 0; k < segments.size(); k++) {
            if (is_same(result.b, segments[k]) && i != k) {
                j = k;
                break;
            }
        }
        std::cout << i + 1 << ' ' << j + 1 << '\n';
    } else {
        std::cout << "NO\n";
    }
}

int main() {
    solveH();
}
