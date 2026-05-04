#include <bits/stdc++.h>

using namespace std;

struct LiChaoTree {
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

    struct Node {
        Line line{0, 0};
        bool has_line = false;
        Node* left = nullptr;
        Node* right = nullptr;
    };

    static constexpr long long INF = (1LL << 62);

    long long lo;
    long long hi;
    Node* root = nullptr;

    LiChaoTree(long long lo, long long hi) : lo(lo), hi(hi) {}

    void add_line(long long m, long long b) {
        add_line(root, lo, hi, Line{m, b});
    }

    long long query(long long x) const {
        return query_value(root, lo, hi, x);
    }

private:
    void add_line(Node*& node, long long l, long long r, Line nw) {
        if (!node) node = new Node();
        if (!node->has_line) {
            node->line = nw;
            node->has_line = true;
            return;
        }

        if (node->line.m == nw.m) {
            if (nw.b < node->line.b) node->line = nw;
            return;
        }

        long long mid = l + (r - l) / 2;
        bool left_better = nw.eval_ld(l) < node->line.eval_ld(l);
        bool mid_better = nw.eval_ld(mid) < node->line.eval_ld(mid);

        if (mid_better) swap(node->line, nw);
        if (l == r) return;

        if (left_better != mid_better) {
            add_line(node->left, l, mid, nw);
        } else {
            add_line(node->right, mid + 1, r, nw);
        }
    }

    long long query_value(Node* node, long long l, long long r, long long x) const {
        if (!node) return INF;
        long long answer = node->has_line ? node->line.eval(x) : INF;
        if (l == r) return answer;
        long long mid = l + (r - l) / 2;
        if (x <= mid) return min(answer, query_value(node->left, l, mid, x));
        return min(answer, query_value(node->right, mid + 1, r, x));
    }
};

// Example application:
// dp[0] = 0
// dp[i] = x[i]^2 + fixed_cost + min_{j < i}(dp[j] + x[j]^2 - 2*x[i]*x[j])
vector<long long> solve_quadratic_dp_arbitrary_x(const vector<long long>& x, long long fixed_cost) {
    int n = (int)x.size();
    if (n == 0) return {};

    long long min_x = *min_element(x.begin(), x.end());
    long long max_x = *max_element(x.begin(), x.end());
    LiChaoTree lichao(min_x, max_x);

    vector<long long> dp(n, 0);
    lichao.add_line(-2 * x[0], x[0] * x[0]);
    for (int i = 1; i < n; ++i) {
        long long best = lichao.query(x[i]);
        dp[i] = x[i] * x[i] + fixed_cost + best;
        lichao.add_line(-2 * x[i], dp[i] + x[i] * x[i]);
    }
    return dp;
}
