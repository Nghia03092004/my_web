#include <bits/stdc++.h>

using namespace std;

struct TwoSat {
    int n;
    vector<vector<int>> graph;
    vector<vector<int>> reverse_graph;
    vector<int> comp;
    vector<int> order;
    vector<int> assignment;

    explicit TwoSat(int n)
        : n(n),
          graph(2 * n),
          reverse_graph(2 * n),
          comp(2 * n, -1),
          assignment(n, 0) {}

    int literal(int variable, bool is_true) const {
        return 2 * variable + (is_true ? 0 : 1);
    }

    int neg(int lit) const {
        return lit ^ 1;
    }

    void add_implication(int from, int to) {
        graph[from].push_back(to);
        reverse_graph[to].push_back(from);
    }

    void add_or(int x, bool x_true, int y, bool y_true) {
        int a = literal(x, x_true);
        int b = literal(y, y_true);
        add_implication(neg(a), b);
        add_implication(neg(b), a);
    }

    void force_true(int x, bool value) {
        int lit = literal(x, value);
        add_implication(neg(lit), lit);
    }

    void dfs1(int v, vector<int>& seen) {
        seen[v] = 1;
        for (int to : graph[v]) {
            if (!seen[to]) dfs1(to, seen);
        }
        order.push_back(v);
    }

    void dfs2(int v, int id) {
        comp[v] = id;
        for (int to : reverse_graph[v]) {
            if (comp[to] == -1) dfs2(to, id);
        }
    }

    bool satisfiable() {
        vector<int> seen(2 * n, 0);
        fill(comp.begin(), comp.end(), -1);
        order.clear();

        for (int v = 0; v < 2 * n; ++v) {
            if (!seen[v]) dfs1(v, seen);
        }
        reverse(order.begin(), order.end());

        int id = 0;
        for (int v : order) {
            if (comp[v] == -1) dfs2(v, id++);
        }

        for (int x = 0; x < n; ++x) {
            int t = literal(x, true);
            int f = literal(x, false);
            if (comp[t] == comp[f]) return false;
            assignment[x] = comp[t] > comp[f];
        }
        return true;
    }
};

// Example application:
// each clause is (x is x_true) OR (y is y_true).
vector<int> solve_clause_system(
    int variables,
    const vector<tuple<int, bool, int, bool>>& clauses,
    const vector<pair<int, bool>>& forced_literals = {}
) {
    TwoSat solver(variables);
    for (const auto& clause : clauses) {
        int x = get<0>(clause);
        bool x_true = get<1>(clause);
        int y = get<2>(clause);
        bool y_true = get<3>(clause);
        solver.add_or(x, x_true, y, y_true);
    }
    for (const auto& literal : forced_literals) {
        solver.force_true(literal.first, literal.second);
    }
    if (!solver.satisfiable()) return {};
    return solver.assignment;
}
