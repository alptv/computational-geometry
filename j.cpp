#include <iostream>
#include <cassert>
#include <vector>
#include <map>
#include <set>
#include <iomanip>
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

    bool operator<(const point<T> &other) const {
        return x < other.x || (x == other.x && y < other.y);
    }
};


template<typename T>
class line {

public:
    point<T> s;
    point<T> e;
    T a;
    T b;
    T c;

    line() = default;

    line(point<T> s, point<T> e) : s(s), e(e) {
        a = s.y - e.y;
        b = e.x - s.x;
        c = -a * s.x - b * s.y;
    }

    bool is_parallel(line &l) {
        return a * l.b == b * l.a;
    }

    bool operator==(line<T> &other) {
        return is_parallel(other) && (c * other.a == other.c * a) && (c * other.b == other.c * b);
    }

    point<long double> intersection(line &l) {
        return {(long double)(l.c * b - c * l.b) / (long double) (a * l.b - l.a * b), (long double) (c * l.a - l.c * a) / (long double)(l.b * a - b * l.a)};
    }

    bool contains(point<long double> p) {
        return (std::abs(a * p.x + b * p.y + c) < 1e-10);
    }
};

class edge {
public:
    edge *twin;
    edge *next;
    edge *prev;
    int s_index;
    int e_index;
    bool has_face;
    int index;

    edge(int s_index, int e_index, int index) : s_index(s_index), e_index(e_index), index(index) {
        twin = nullptr;
        next = nullptr;
        prev = nullptr;
        has_face = false;
    }

};

class vertex {
public:
    point<long double> p;
    std::vector<edge *> edges;
};

struct DCEL {
    std::vector<vertex> vertices;
    std::vector<edge *> edges;
};

DCEL lines_to_DCEL(std::vector<line<int>> &lines) {
    std::set<point<long double>> intersections;
    for (int i = 0; i < lines.size(); i++) {
        for (int j = i + 1; j < lines.size(); j++) {
            if (lines[i] == lines[j] || lines[i].is_parallel(lines[j])) {
                continue;
            }
            intersections.insert(lines[i].intersection(lines[j]));
        }
    }
    std::vector<vertex> vertices;
    vertices.reserve(intersections.size());
    for (auto p : intersections) {
        vertices.push_back({p, {}});
    }
    std::vector<edge *> edges;
    for (auto l : lines) {
        std::vector<std::pair<point<long double>, int>> points_on_line;
        for (int i = 0; i < vertices.size(); i++) {
            if (l.contains(vertices[i].p)) {
                points_on_line.emplace_back(vertices[i].p, i);
            }
        }
        std::sort(points_on_line.begin(), points_on_line.end());
        for (int i = 0; i + 1 < points_on_line.size(); i++) {
            edge *e1 = new edge(points_on_line[i].second,
                                points_on_line[i + 1].second, (int) edges.size());
            edge *e2 = new edge(points_on_line[i + 1].second,
                                points_on_line[i].second, (int) edges.size() + 1);
            e1->twin = e2;
            e2->twin = e1;
            edges.push_back(e1);
            edges.push_back(e2);
        }
    }
    return {vertices, edges};
}

template<typename T>
int sgn(T x) {
    return (x > 0) - (x < 0);
}

std::vector<long double> compute_faces_square(DCEL dcel) {
    auto vertices = dcel.vertices;
    auto edges = dcel.edges;
    for (edge *e : edges) {
        vertices[e->e_index].edges.push_back(e);
    }
    for (auto v : vertices) {
        int x = 1;
        std::sort(v.edges.begin(), v.edges.end(), [&](edge *a, edge *b) {
            long double ay = vertices[a->s_index].p.y;
            long double by = vertices[b->s_index].p.y;
            int a_above = sgn(ay - v.p.y);
            int b_above = sgn(by - v.p.y);
            long double ax = vertices[a->s_index].p.x;
            long double bx = vertices[b->s_index].p.x;
            if (a_above >= 0 && b_above == -1) {
                return true;
            }
            if (a_above == -1 && b_above >= 0) {
                return false;
            }
            if (a_above == 0 && b_above == 0) {
                int a_left = sgn(ax - v.p.x);
                int b_left = sgn(bx - v.p.x);
                return a_left < b_left;
            }
            int turn = sgn((ax - v.p.x) * (by - v.p.y) - (bx - v.p.x) * (ay - v.p.y));
            return turn < 0;
        });
        for (int i = 0; i + 1 < v.edges.size(); i++) {
            v.edges[i]->next = v.edges[i + 1]->twin;
            v.edges[i + 1]->twin->prev = v.edges[i];
        }
        if (v.edges.size() >= 2) {
            v.edges[v.edges.size() - 1]->next = v.edges[0]->twin;
            v.edges[0]->twin->prev = v.edges[v.edges.size() - 1];
        }
    }
    std::vector<long double> squares;
    for (int i = 0; i < edges.size(); i++) {
        if (edges[i]->has_face) {
            continue;
        }
        int cur = i;
        long double square = 0;
        bool face = false;
        while (true) {
            edges[cur]->has_face = true;
            point<long double> s = vertices[edges[cur]->s_index].p;
            point<long double> e = vertices[edges[cur]->e_index].p;
            square += s.x * e.y - s.y * e.x;
            if (edges[i]->next == nullptr) {
                break;
            }
            int next = edges[cur]->next->index;
            if (next == i) {
                face = true;
                break;
            }
            cur = next;
        }
        if (face && square > 0) {
            squares.push_back(square / 2);
        }
    }
    return squares;

}

void solveJ() {
    int x1, y1, x2, y2;
    int n;
    std::cin >> n;

    std::vector<line<int>> lines;
    for (int i = 0; i < n; i++) {
        std::cin >> x1 >> y1 >> x2 >> y2;
        line<int> line({x1, y1}, {x2, y2});
        lines.push_back(line);
    }
    auto dcel = lines_to_DCEL(lines);
    auto squares = compute_faces_square(dcel);
    std::sort(squares.begin(), squares.end());
    long double min_s = 1e-8;
    std::vector<long double> result;
    for (long double &s : squares) {
        if (s >= min_s) {
            result.push_back(s);
        }
    }
    std::cout << result.size() << '\n';
    for (long double s : result) {
        std::cout << std::fixed << std::setprecision(9) << s << '\n';
    }
}

int main() {
    solveJ();
}