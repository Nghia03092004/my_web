#include <bits/stdc++.h>

using namespace std;

vector<long long> subset_sums(const vector<long long>& a) {
    int n = (int)a.size();
    vector<long long> sums;
    sums.reserve(1 << n);
    for (int mask = 0; mask < (1 << n); ++mask) {
        long long total = 0;
        for (int bit = 0; bit < n; ++bit) {
            if ((mask >> bit) & 1) total += a[bit];
        }
        sums.push_back(total);
    }
    return sums;
}

// Example application: maximum subset sum not exceeding limit.
long long max_subset_sum_at_most(const vector<long long>& a, long long limit) {
    int n = (int)a.size();
    int mid = n / 2;
    vector<long long> left(a.begin(), a.begin() + mid);
    vector<long long> right(a.begin() + mid, a.end());

    vector<long long> left_sums = subset_sums(left);
    vector<long long> right_sums = subset_sums(right);
    sort(right_sums.begin(), right_sums.end());

    long long answer = 0;
    for (long long x : left_sums) {
        if (x > limit) continue;
        auto it = upper_bound(right_sums.begin(), right_sums.end(), limit - x);
        if (it != right_sums.begin()) {
            --it;
            answer = max(answer, x + *it);
        }
    }
    return answer;
}

// Another common application: exact target via hashing instead of sorting + upper_bound.
bool subset_sum_exact_target(const vector<long long>& a, long long target) {
    int n = (int)a.size();
    int mid = n / 2;
    vector<long long> left(a.begin(), a.begin() + mid);
    vector<long long> right(a.begin() + mid, a.end());

    vector<long long> left_sums = subset_sums(left);
    unordered_set<long long> wanted;
    wanted.reserve(left_sums.size() * 2);
    for (long long x : left_sums) wanted.insert(target - x);

    for (long long y : subset_sums(right)) {
        if (wanted.count(y)) return true;
    }
    return false;
}
