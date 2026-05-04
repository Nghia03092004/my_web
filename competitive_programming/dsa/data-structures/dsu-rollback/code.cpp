#include <bits/stdc++.h>

using namespace std;

struct DsuRollback {
    struct Change {
        int child;
        int parent_before;
        int root;
        int size_before;
    };

    vector<int> parent;
    vector<int> size;
    vector<Change> history;
    int components;

    explicit DsuRollback(int n) : parent(n), size(n, 1), components(n) {
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int v) const {
        while (parent[v] != v) v = parent[v];
        return v;
    }

    int snapshot() const {
        return (int)history.size();
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (size[a] < size[b]) swap(a, b);

        history.push_back({b, parent[b], a, size[a]});
        parent[b] = a;
        size[a] += size[b];
        --components;
        return true;
    }

    void rollback(int snap) {
        while ((int)history.size() > snap) {
            Change ch = history.back();
            history.pop_back();
            parent[ch.child] = ch.parent_before;
            size[ch.root] = ch.size_before;
            ++components;
        }
    }

    bool same(int a, int b) const {
        return find(a) == find(b);
    }
};

struct ConnectivityQuery {
    int u;
    int v;
    int id;
};

struct OfflineDynamicConnectivity {
    int q;
    vector<vector<pair<int, int>>> edges_on_segment;
    vector<vector<ConnectivityQuery>> queries_at_time;

    explicit OfflineDynamicConnectivity(int q)
        : q(q), edges_on_segment(4 * q), queries_at_time(q) {}

    void add_edge_interval(int node, int l, int r, int ql, int qr, pair<int, int> edge) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            edges_on_segment[node].push_back(edge);
            return;
        }
        int mid = (l + r) / 2;
        add_edge_interval(node * 2, l, mid, ql, qr, edge);
        add_edge_interval(node * 2 + 1, mid + 1, r, ql, qr, edge);
    }

    void add_edge_interval(int left_time, int right_time, int u, int v) {
        add_edge_interval(1, 0, q - 1, left_time, right_time, {u, v});
    }

    void add_query(int time, int u, int v, int id) {
        queries_at_time[time].push_back({u, v, id});
    }

    void dfs(int node, int l, int r, DsuRollback& dsu, vector<int>& answer) {
        int snap = dsu.snapshot();
        for (const auto& edge : edges_on_segment[node]) dsu.unite(edge.first, edge.second);

        if (l == r) {
            for (const auto& query : queries_at_time[l]) {
                answer[query.id] = dsu.same(query.u, query.v);
            }
        } else {
            int mid = (l + r) / 2;
            dfs(node * 2, l, mid, dsu, answer);
            dfs(node * 2 + 1, mid + 1, r, dsu, answer);
        }

        dsu.rollback(snap);
    }
};
