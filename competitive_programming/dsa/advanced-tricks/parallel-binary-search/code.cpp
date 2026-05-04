struct Fenwick {
    int n;
    vector<long long> bit;

    explicit Fenwick(int n) : n(n), bit(n + 1, 0) {}

    void add(int idx, long long delta) {
        for (; idx <= n; idx += idx & -idx) bit[idx] += delta;
    }

    long long sum_prefix(int idx) const {
        long long result = 0;
        for (; idx > 0; idx -= idx & -idx) result += bit[idx];
        return result;
    }

    long long range_sum(int l, int r) const {
        return sum_prefix(r) - sum_prefix(l - 1);
    }
};

struct PointUpdate {
    int pos;
    long long delta;
};

struct ThresholdQuery {
    int l;
    int r;
    long long need;
    int id;
};

void solve_parallel_bs(
    int left_update,
    int right_update,
    vector<ThresholdQuery> queries,
    const vector<PointUpdate>& updates,
    Fenwick& fw,
    vector<int>& answer
) {
    if (queries.empty()) return;
    if (left_update == right_update) {
        for (const auto& query : queries) {
            answer[query.id] = left_update;
        }
        return;
    }

    int mid = (left_update + right_update) / 2;
    for (int i = left_update; i <= mid; ++i) {
        fw.add(updates[i].pos, updates[i].delta);
    }

    vector<ThresholdQuery> go_left;
    vector<ThresholdQuery> go_right;
    for (auto query : queries) {
        long long gained = fw.range_sum(query.l, query.r);
        if (gained >= query.need) {
            go_left.push_back(query);
        } else {
            query.need -= gained;
            go_right.push_back(query);
        }
    }

    for (int i = left_update; i <= mid; ++i) {
        fw.add(updates[i].pos, -updates[i].delta);
    }

    solve_parallel_bs(left_update, mid, go_left, updates, fw, answer);
    solve_parallel_bs(mid + 1, right_update, go_right, updates, fw, answer);
}

// updates is 1-indexed. Add a dummy no-op update at index m + 1 to represent "never".
vector<int> first_time_range_sum_reaches_target(
    int n,
    const vector<PointUpdate>& updates,
    const vector<ThresholdQuery>& queries
) {
    Fenwick fw(n);
    vector<int> answer(queries.size(), (int)updates.size() - 1);
    solve_parallel_bs(1, (int)updates.size() - 1, queries, updates, fw, answer);
    return answer;
}
