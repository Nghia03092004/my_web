struct HopcroftKarp {
    int n_left;
    int n_right;
    vector<vector<int>> graph;
    vector<int> dist;
    vector<int> match_left;
    vector<int> match_right;

    HopcroftKarp(int n_left, int n_right)
        : n_left(n_left),
          n_right(n_right),
          graph(n_left),
          dist(n_left),
          match_left(n_left, -1),
          match_right(n_right, -1) {}

    void add_edge(int left, int right) {
        graph[left].push_back(right);
    }

    bool bfs() {
        queue<int> q;
        fill(dist.begin(), dist.end(), -1);
        for (int v = 0; v < n_left; ++v) {
            if (match_left[v] == -1) {
                dist[v] = 0;
                q.push(v);
            }
        }

        bool found = false;
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int to : graph[v]) {
                int mate = match_right[to];
                if (mate == -1) {
                    found = true;
                } else if (dist[mate] == -1) {
                    dist[mate] = dist[v] + 1;
                    q.push(mate);
                }
            }
        }
        return found;
    }

    bool dfs(int v) {
        for (int to : graph[v]) {
            int mate = match_right[to];
            if (mate == -1 || (dist[mate] == dist[v] + 1 && dfs(mate))) {
                match_left[v] = to;
                match_right[to] = v;
                return true;
            }
        }
        dist[v] = -1;
        return false;
    }

    int maximum_matching() {
        int matching = 0;
        while (bfs()) {
            for (int v = 0; v < n_left; ++v) {
                if (match_left[v] == -1 && dfs(v)) {
                    ++matching;
                }
            }
        }
        return matching;
    }
};
