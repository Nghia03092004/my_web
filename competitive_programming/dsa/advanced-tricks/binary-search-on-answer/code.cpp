template <class F>
long long first_true(long long lo, long long hi, F pred) {
    while (lo < hi) {
        long long mid = lo + (hi - lo) / 2;
        if (pred(mid)) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}

bool can_partition_with_limit(const vector<long long>& a, int k, long long limit) {
    int parts = 1;
    long long current = 0;
    for (long long x : a) {
        if (x > limit) {
            return false;
        }
        if (current + x > limit) {
            ++parts;
            current = 0;
        }
        current += x;
    }
    return parts <= k;
}

long long minimize_max_segment_sum(const vector<long long>& a, int k) {
    long long lo = *max_element(a.begin(), a.end());
    long long hi = accumulate(a.begin(), a.end(), 0LL);
    return first_true(lo, hi, [&](long long limit) {
        return can_partition_with_limit(a, k, limit);
    });
}
