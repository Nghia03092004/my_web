#include <bits/stdc++.h>

using namespace std;

struct Line {
    long long m;
    long long b;

    long long eval(long long x) const {
        return m * x + b;
    }

    long double eval_ld(long long x) const {
        return (long double)m * x + b;
    }
};

struct MonotoneConvexHullTrick {
    deque<Line> hull;

    // Assumes minimum queries, slopes inserted in monotone order.
    static bool is_bad(const Line& a, const Line& b, const Line& c) {
#if defined(__SIZEOF_INT128__)
        return (__int128)(b.b - a.b) * (a.m - c.m) >= (__int128)(c.b - a.b) * (a.m - b.m);
#else
        long double left = (long double)(b.b - a.b) * (a.m - c.m);
        long double right = (long double)(c.b - a.b) * (a.m - b.m);
        return left >= right;
#endif
    }

    void add_line(long long m, long long b) {
        Line line{m, b};
        while (!hull.empty() && hull.back().m == line.m) {
            if (hull.back().b <= line.b) return;
            hull.pop_back();
        }
        while (hull.size() >= 2 && is_bad(hull[hull.size() - 2], hull.back(), line)) {
            hull.pop_back();
        }
        hull.push_back(line);
    }

    long long query(long long x) {
        while (hull.size() >= 2 && hull[0].eval_ld(x) >= hull[1].eval_ld(x)) {
            hull.pop_front();
        }
        return hull.front().eval(x);
    }
};

// Example application:
// x must be sorted increasingly.
vector<long long> solve_quadratic_dp_sorted(const vector<long long>& x, long long fixed_cost) {
    int n = (int)x.size();
    if (n == 0) return {};

    vector<long long> dp(n, 0);
    MonotoneConvexHullTrick cht;
    cht.add_line(-2 * x[0], x[0] * x[0]);

    for (int i = 1; i < n; ++i) {
        dp[i] = x[i] * x[i] + fixed_cost + cht.query(x[i]);
        cht.add_line(-2 * x[i], dp[i] + x[i] * x[i]);
    }
    return dp;
}
