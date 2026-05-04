#include <bits/stdc++.h>

using namespace std;

struct SumDistancesRerooting {
    int n;
    vector<vector<int>> tree;
    vector<int> sub_size;
    vector<long long> down;
    vector<long long> answer;

    explicit SumDistancesRerooting(int n)
        : n(n), tree(n), sub_size(n, 1), down(n, 0), answer(n, 0) {}

    void add_edge(int u, int v) {
        tree[u].push_back(v);
        tree[v].push_back(u);
    }

    void dfs_down(int v, int parent = -1) {
        for (int to : tree[v]) {
            if (to == parent) continue;
            dfs_down(to, v);
            sub_size[v] += sub_size[to];
            down[v] += down[to] + sub_size[to];
        }
    }

    void dfs_up(int v, int parent = -1) {
        for (int to : tree[v]) {
            if (to == parent) continue;
            answer[to] = answer[v] - sub_size[to] + (n - sub_size[to]);
            dfs_up(to, v);
        }
    }

    vector<long long> solve(int root = 0) {
        dfs_down(root);
        answer[root] = down[root];
        dfs_up(root);
        return answer;
    }
};

// Example application: compute sum of distances from every node.
vector<long long> sum_of_distances_on_tree(int n, const vector<pair<int, int>>& edges) {
    SumDistancesRerooting solver(n);
    for (const auto& edge : edges) solver.add_edge(edge.first, edge.second);
    return solver.solve(0);
}
