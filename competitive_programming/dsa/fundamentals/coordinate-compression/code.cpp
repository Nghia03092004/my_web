#include <bits/stdc++.h>

using namespace std;

template <class T>
struct CoordinateCompression {
    vector<T> values;

    void add_value(const T& x) {
        values.push_back(x);
    }

    void build() {
        sort(values.begin(), values.end());
        values.erase(unique(values.begin(), values.end()), values.end());
    }

    int index(const T& x) const {
        return (int)(lower_bound(values.begin(), values.end(), x) - values.begin());
    }

    T value_at(int idx) const {
        return values[idx];
    }

    int size() const {
        return (int)values.size();
    }
};

struct Fenwick {
    int n;
    vector<long long> bit;

    explicit Fenwick(int n) : n(n), bit(n + 1, 0) {}

    void add(int idx, long long delta) {
        for (; idx <= n; idx += idx & -idx) bit[idx] += delta;
    }

    long long prefix_sum(int idx) const {
        long long result = 0;
        for (; idx > 0; idx -= idx & -idx) result += bit[idx];
        return result;
    }
};

template <class T>
vector<int> compress_vector(const vector<T>& a, vector<T>* sorted_unique = nullptr) {
    CoordinateCompression<T> cc;
    for (const T& x : a) cc.add_value(x);
    cc.build();

    vector<int> compressed;
    compressed.reserve(a.size());
    for (const T& x : a) compressed.push_back(cc.index(x));
    if (sorted_unique) *sorted_unique = cc.values;
    return compressed;
}

// Example application: count inversions in an array with large or negative values.
long long count_inversions_with_compression(const vector<long long>& a) {
    vector<int> compressed = compress_vector(a);
    int max_rank = 0;
    for (int x : compressed) max_rank = max(max_rank, x);

    Fenwick fw(max_rank + 1);
    long long inversions = 0;
    for (int i = 0; i < (int)compressed.size(); ++i) {
        int rank = compressed[i] + 1;  // Fenwick is 1-indexed.
        long long not_greater = fw.prefix_sum(rank);
        inversions += i - not_greater;
        fw.add(rank, 1);
    }
    return inversions;
}
