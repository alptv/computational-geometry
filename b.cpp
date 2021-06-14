#include <iostream>
#include <cmath>
#include <iomanip>

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
};

//pq and pa
template<typename T>
T cross_product(point<T> &a, point<T> &p, point<T> &q) {
    return (q.x - p.x) * (a.y - p.y) - (q.y - p.y) * (a.x - p.x);
}

template<typename T>
int left_turn(point<T> &a, point<T> &p, point<T> &q) {
    T sgn_square = cross_product(a, p, q);
    return (sgn_square > 0) - (sgn_square < 0);
}

//pq and pa
template<typename T>
T scalar_product(point<T> &a, point<T> &p, point<T> &q) {
    return (q.x - p.x) * (a.x - p.x) + (q.y - p.y) * (a.y - p.y);
}

template<typename T>
long double squared_distance(point<T> &a, point<T> &b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

template<typename T>
long double distance(point<T> &a, point<T> &b) {
    return sqrt(squared_distance(a, b));
}

//correct
template<typename T>
long double distance_to_segment(point<T> &a, point<T> &p, point<T> &q) {
    return std::abs(cross_product(a, p, q)) / distance(p, q);
}

//correct
template<typename T>
long double distance_to_ray(point<T> &a, point<T> &p, point<T> &q) {
    T projection = scalar_product(a, p, q);
    if (scalar_product(a, p, q) < 0) {
        return distance(a, p);
    }
    return distance_to_segment(a, p, q);
}

//correct
template<typename T>
long double distance_to_segment(point<T> &a, point<T> &p, point<T> &q) {
    T projection = scalar_product(a, p, q);
    if (projection < 0) {
        return distance(a, p);
    }
    if (projection > squared_distance(p, q)) {
        return distance(a, q);
    }
    return distance_to_segment(a, p, q);
}


enum location {
    left,
    mid,
    right,
    parallel,
    one_line

};

template<typename T>
location choose_location(T t, T denominator) {
    if (t * denominator < 0) {
        return left;
    }
    t = std::abs(t);
    denominator = std::abs(denominator);
    if (t > denominator) {
        return right;
    }
    return mid;
}

template<typename T>
std::pair<location, location> get_locations(point<T> &a, point<T> &b, point<T> &p, point<T> &q) {
    T vx = q.x - p.x;
    T vy = q.y - p.y;
    T wx = b.x - a.x;
    T wy = b.y - a.y;
    T denominator = vx * wy - vy * wx;
    if (denominator == 0) {
        if ((a.x - p.x) * wy == (a.y - p.y) * wx) {
            return {one_line, one_line};
        }
        return {parallel, parallel};
    }
    T t = (a.x - p.x) * wy - (a.y - p.y) * wx;
    T k = (a.x - p.x) * vy - (a.y - p.y) * vx;
    std::pair<location, location> loc;
    loc.first = choose_location(k, denominator);
    loc.second = choose_location(t, denominator);
    return loc;
}


template<typename T>
bool is_intersect_segments(point<T> &a, point<T> &b, point<T> &p, point<T> &q) {
    auto loc = get_locations(a, b, p, q);
    return (loc.first == mid || loc.first == one_line)
           && ((loc.second == mid || loc.second == one_line))
           && std::max(std::min(a.x, b.x), std::min(p.x, q.x)) <= std::min(std::max(a.x, b.x), std::max(p.x, q.x))
           && std::max(std::min(a.y, b.y), std::min(p.y, q.y)) <= std::min(std::max(a.y, b.y), std::max(p.y, q.y));
}

template<typename T>
long double segments_distance(point<T> &a, point<T> &b, point<T> &p, point<T> &q) {
    if (is_intersect_segments(a, b, p, q)) {
        return 0;
    }
    return std::min(std::min(distance_to_segment(a, p, q), distance_to_segment(b, p, q)),
                    std::min(distance_to_segment(p, a, b), distance_to_segment(q, a, b)));
}

template<typename T>
bool is_intersect_segment_ray(point<T> &a, point<T> &b, point<T> &p, point<T> &q) {
    auto loc = get_locations(a, b, p, q);
    T x_d = q.x - p.x;
    T y_d = q.y - p.y;
    T ax_d = a.x - p.x;
    T ay_d = a.y - p.y;
    T bx_d = b.x - p.x;
    T by_d = b.y - p.y;
    return (loc.first == mid && (loc.second == mid || loc.second == right)) ||
           (loc.first == one_line && loc.second == one_line &&
            ((ax_d * x_d >= 0 && ay_d * y_d >= 0) || (bx_d * x_d >= 0 && by_d * y_d >= 0)));

}

template<typename T>
long double segment_ray_distance(point<T> &a, point<T> &b, point<T> &p, point<T> &q) {
    if (is_intersect_segment_ray(a, b, p, q)) {
        return 0;
    }
    return std::min(distance_to_segment(p, a, b), std::min(distance_to_ray(a, p, q), distance_to_ray(b, p, q)));
}

template<typename T>
bool is_intersect_segment_line(point<T> &a, point<T> &b, point<T> &p, point<T> &q) {
    auto loc = get_locations(a, b, p, q);
    return loc.first == mid;
}

template<typename T>
long double segment_line_distance(point<T> &a, point<T> &b, point<T> &p, point<T> &q) {
    if (is_intersect_segment_line(a, b, p, q)) {
        return 0;
    }
    return std::min(distance_to_segment(a, p, q), distance_to_segment(b, p, q));
}

template<typename T>
bool is_intersect_ray_ray(point<T> &a, point<T> &b, point<T> &p, point<T> &q) {
    auto loc = get_locations(a, b, p, q);
    T x_dab = b.x - a.x;
    T y_dab = b.y - a.y;
    T x_dpq = q.x - p.x;
    T y_dpq = q.y - p.y;

    T ax_d = a.x - p.x;
    T ay_d = a.y - p.y;
    T bx_d = b.x - p.x;
    T by_d = b.y - p.y;
    return ((loc.first == mid || loc.first == right) && (loc.second == mid || loc.second == right))
           || (loc.first == one_line && loc.second == one_line &&
               ((x_dab * x_dpq >= 0 && y_dab * y_dpq >= 0) ||
                ((ax_d * x_dpq >= 0 && ay_d * y_dpq >= 0) || (bx_d * x_dpq >= 0 && by_d * y_dpq >= 0))));
}

template<typename T>
long double ray_ray_distance(point<T> &a, point<T> &b, point<T> &p, point<T> &q) {
    if (is_intersect_ray_ray(a, b, p, q)) {
        return 0;
    }
    return std::min(distance_to_ray(a, p, q), distance_to_ray(p, a, b));
}


template<typename T>
bool is_intersect_ray_line(point<T> &a, point<T> &b, point<T> &p, point<T> &q) {
    auto loc = get_locations(a, b, p, q);
    return loc.first == mid || loc.first == right || loc.first == one_line;
}

template<typename T>
long double ray_line_distance(point<T> &a, point<T> &b, point<T> &p, point<T> &q) {
    if (is_intersect_ray_line(a, b, p, q)) {
        return 0;
    }
    return distance_to_segment(a, p, q);
}

template<typename T>
long double line_line_distance(point<T> &a, point<T> &b, point<T> &p, point<T> &q) {
    auto loc = get_locations(a, b, p, q);
    if (loc.first != parallel) {
        return 0;
    }
    return distance_to_segment(a, p, q);
}


int main() {
    point<long long> a;
    point<long long> b;
    point<long long> c;
    point<long long> d;
    std::cin >> a >> b >> c >> d;
    int precision = 13;
    std::cout << std::fixed << std::setprecision(precision) << distance(a, c) << '\n';
    std::cout << std::fixed << std::setprecision(precision) << distance_to_segment(a, c, d) << '\n';
    std::cout << std::fixed << std::setprecision(precision) << distance_to_ray(a, c, d) << '\n';
    std::cout << std::fixed << std::setprecision(precision) << distance_to_segment(a, c, d) << '\n';

    std::cout << std::fixed << std::setprecision(precision) << distance_to_segment(c, a, b) << '\n';
    std::cout << std::fixed << std::setprecision(precision) << segments_distance(a, b, c, d) << '\n';
    std::cout << std::fixed << std::setprecision(precision) << segment_ray_distance(a, b, c, d) << '\n';
    std::cout << std::fixed << std::setprecision(precision) << segment_line_distance(a, b, c, d) << '\n';

    std::cout << std::fixed << std::setprecision(precision) << distance_to_ray(c, a, b) << '\n';
    std::cout << std::fixed << std::setprecision(precision) << segment_ray_distance(c, d, a, b) << '\n';
    std::cout << std::fixed << std::setprecision(precision) << ray_ray_distance(a, b, c, d) << '\n';
    std::cout << std::fixed << std::setprecision(precision) << ray_line_distance(a, b, c, d) << '\n';

    std::cout << std::fixed << std::setprecision(precision) << distance_to_segment(c, a, b) << '\n';
    std::cout << std::fixed << std::setprecision(precision) << segment_line_distance(c, d, a, b) << '\n';
    std::cout << std::fixed << std::setprecision(precision) << ray_line_distance(c, d, a, b) << '\n';
    std::cout << std::fixed << std::setprecision(precision) << line_line_distance(a, b, c, d) << '\n';

}




