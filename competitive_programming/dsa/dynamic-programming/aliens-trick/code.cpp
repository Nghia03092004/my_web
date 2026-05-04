struct State {
    long long value;
    int count;
};

State better(State a, State b) {
    if (a.value != b.value) return (a.value > b.value ? a : b);
    return (a.count > b.count ? a : b);
}

State add(State base, long long delta_value, int delta_count) {
    return {base.value + delta_value, base.count + delta_count};
}

State solve_with_penalty(const vector<long long>& a, long long penalty) {
    const long long NEG = -(1LL << 60);
    State best_total{0, 0};
    State best_end{NEG, -(int)1e9};

    for (long long x : a) {
        best_end = better(
            add(best_end, x, 0),
            add(best_total, x - penalty, 1)
        );
        best_total = better(best_total, best_end);
    }
    return best_total;
}

long long max_sum_of_at_most_k_disjoint_subarrays(const vector<long long>& a, int k) {
    long long lo = -(1LL << 40), hi = (1LL << 40);
    while (lo < hi) {
        long long mid = lo + (hi - lo + 1) / 2;
        if (solve_with_penalty(a, mid).count >= k) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    State result = solve_with_penalty(a, lo);
    return result.value + lo * k;
}
