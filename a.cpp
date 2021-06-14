#include <bits/stdc++.h>
#include <gmpxx.h>


class mpf_point {
public:
    mpf_class x;
    mpf_class y;

    mpf_point(double x, double y) {
        this->x = mpf_class(x);
        this->x.set_prec(5000);
        this->y = mpf_class(y);
        this->y.set_prec(5000);
    }


};

int left_turn(mpf_point &s, mpf_point &p, mpf_point &q) {
    mpf_class turn = (p.x - s.x) * (q.y - s.y) - (p.y - s.y) * (q.x - s.x);
    return sgn(turn);
}

bool intersect_projections(mpf_class &p, mpf_class &q, mpf_class &a, mpf_class &b) {
    return std::max(std::min(p, q), std::min(a, b)) <= std::min(std::max(p, q), std::max(a, b));
}

bool intersect(mpf_point &p, mpf_point &q, mpf_point &a, mpf_point &b) {
    return (left_turn(p, q, a) * left_turn(p, q, b) <= 0)
           && (left_turn(a, b, p) * left_turn(a, b, q) <= 0)
           && intersect_projections(p.x, q.x, a.x, b.x)
           && intersect_projections(p.y, q.y, a.y, b.y);
}


void solveA() {
    int test_num;
    std::cin >> test_num;
    std::vector<double> input = genTest(test_num);
    std::string ans;

    for (uint32_t i = 0; i < input.size(); i += 8) {
        mpf_point p(input[i], input[i + 1]);
        mpf_point q(input[i + 2], input[i + 3]);
        mpf_point a(input[i + 4], input[i + 5]);
        mpf_point b(input[i + 6], input[i + 7]);
        if (intersect(p, q, a, b)) {
            ans.push_back('Y');
        } else {
            ans.push_back('N');
        }
    }
    std::cout << ans << '\n';
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    solveA();
}


