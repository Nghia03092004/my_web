#include <bits/stdc++.h>

using namespace std;

struct NearestRedCentroid {
    static constexpr int INF = (int)1e9;

    int n;
    vector<vector<int>> tree;
    vector<int> sub_size;
    vector<int> removed;
    vector<int> centroid_parent;
    vector<int> best;
    vector<vector<pair<int, int>>> path_to_centroids;

    explicit NearestRedCentroid(int n)
        : n(n),
          tree(n),
          sub_size(n, 0),
          removed(n, 0),
          centroid_parent(n, -1),
          best(n, INF),
          path_to_centroids(n) {}

    void add_edge(int u, int v) {
        tree[u].push_back(v);
        tree[v].push_back(u);
    }

    void build() {
        decompose(0, -1);
    }

    void paint_red(int v) {
        for (const auto& entry : path_to_centroids[v]) {
            int centroid = entry.first;
            int dist = entry.second;
            best[centroid] = min(best[centroid], dist);
        }
    }

    int query_nearest_red(int v) const {
        int answer = INF;
        for (const auto& entry : path_to_centroids[v]) {
            int centroid = entry.first;
            int dist = entry.second;
            answer = min(answer, best[centroid] + dist);
        }
        return answer;
    }

private:
    void dfs_size(int v, int p) {
        sub_size[v] = 1;
        for (int to : tree[v]) {
            if (to == p || removed[to]) continue;
            dfs_size(to, v);
            sub_size[v] += sub_size[to];
        }
    }

    int find_centroid(int v, int p, int total) {
        for (int to : tree[v]) {
            if (to == p || removed[to]) continue;
            if (sub_size[to] > total / 2) {
                return find_centroid(to, v, total);
            }
        }
        return v;
    }

    void collect_distances(int v, int p, int dist, int centroid) {
        path_to_centroids[v].push_back({centroid, dist});
        for (int to : tree[v]) {
            if (to == p || removed[to]) continue;
            collect_distances(to, v, dist + 1, centroid);
        }
    }

    void decompose(int entry, int parent) {
        dfs_size(entry, -1);
        int centroid = find_centroid(entry, -1, sub_size[entry]);
        centroid_parent[centroid] = parent;
        collect_distances(centroid, -1, 0, centroid);

        removed[centroid] = 1;
        for (int to : tree[centroid]) {
            if (!removed[to]) {
                decompose(to, centroid);
            }
        }
    }
};

// Example usage:
// build the structure, paint node 0 red, then answer online nearest-red queries.
vector<int> process_nearest_red_queries(
    int n,
    const vector<pair<int, int>>& edges,
    const vector<pair<int, int>>& operations
) {
    NearestRedCentroid solver(n);
    for (const auto& edge : edges) solver.add_edge(edge.first, edge.second);
    solver.build();
    solver.paint_red(0);

    vector<int> answers;
    for (const auto& op : operations) {
        int type = op.first;
        int v = op.second;
        if (type == 1) {
            solver.paint_red(v);
        } else {
            answers.push_back(solver.query_nearest_red(v));
        }
    }
    return answers;
}
