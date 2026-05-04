#include <bits/stdc++.h>

using namespace std;

struct PersistentSegmentTree {
    struct Node {
        int left = 0;
        int right = 0;
        long long sum = 0;
    };

    int n;
    vector<Node> pool;

    explicit PersistentSegmentTree(int n) : n(n), pool(1) {}

    int build(const vector<long long>& a, int l, int r) {
        int node = clone(0);
        if (l == r) {
            pool[node].sum = a[l];
            return node;
        }
        int mid = (l + r) / 2;
        pool[node].left = build(a, l, mid);
        pool[node].right = build(a, mid + 1, r);
        pull(node);
        return node;
    }

    int update(int node, int l, int r, int pos, long long value) {
        int cur = clone(node);
        if (l == r) {
            pool[cur].sum = value;
            return cur;
        }
        int mid = (l + r) / 2;
        if (pos <= mid) {
            pool[cur].left = update(pool[cur].left, l, mid, pos, value);
        } else {
            pool[cur].right = update(pool[cur].right, mid + 1, r, pos, value);
        }
        pull(cur);
        return cur;
    }

    long long query(int node, int l, int r, int ql, int qr) const {
        if (qr < l || r < ql) return 0;
        if (ql <= l && r <= qr) return pool[node].sum;
        int mid = (l + r) / 2;
        return query(pool[node].left, l, mid, ql, qr) +
               query(pool[node].right, mid + 1, r, ql, qr);
    }

private:
    int clone(int from) {
        pool.push_back(pool[from]);
        return (int)pool.size() - 1;
    }

    void pull(int node) {
        pool[node].sum = pool[pool[node].left].sum + pool[pool[node].right].sum;
    }
};

// Example application: keep every version after point assignments and answer historical range sums.
vector<long long> historical_range_sum_queries(
    const vector<long long>& initial,
    const vector<pair<int, long long>>& updates,
    const vector<array<int, 3>>& queries  // {version, left, right}
) {
    int n = (int)initial.size();
    PersistentSegmentTree pst(n);
    vector<int> roots;
    roots.reserve(updates.size() + 1);
    roots.push_back(pst.build(initial, 0, n - 1));

    for (const auto& update : updates) {
        int pos = update.first;
        long long value = update.second;
        roots.push_back(pst.update(roots.back(), 0, n - 1, pos, value));
    }

    vector<long long> answer;
    answer.reserve(queries.size());
    for (const auto& query : queries) {
        int version = query[0];
        int ql = query[1];
        int qr = query[2];
        answer.push_back(pst.query(roots[version], 0, n - 1, ql, qr));
    }
    return answer;
}
