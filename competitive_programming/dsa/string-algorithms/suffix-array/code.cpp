#include <bits/stdc++.h>

using namespace std;

pair<vector<int>, vector<int>> build_suffix_array(const string& original) {
    string s = original + '$';
    int n = (int)s.size();

    vector<int> sa(n), cls(n);
    {
        vector<pair<char, int>> initial(n);
        for (int i = 0; i < n; ++i) initial[i] = {s[i], i};
        sort(initial.begin(), initial.end());
        for (int i = 0; i < n; ++i) sa[i] = initial[i].second;

        cls[sa[0]] = 0;
        for (int i = 1; i < n; ++i) {
            cls[sa[i]] = cls[sa[i - 1]] + (initial[i].first != initial[i - 1].first);
        }
    }

    for (int k = 0; (1 << k) < n; ++k) {
        for (int i = 0; i < n; ++i) {
            sa[i] = (sa[i] - (1 << k) + n) % n;
        }

        vector<int> cnt(n, 0), next_sa(n);
        for (int x : cls) ++cnt[x];
        for (int i = 1; i < n; ++i) cnt[i] += cnt[i - 1];
        for (int i = n - 1; i >= 0; --i) {
            next_sa[--cnt[cls[sa[i]]]] = sa[i];
        }
        sa = next_sa;

        vector<int> next_cls(n);
        next_cls[sa[0]] = 0;
        for (int i = 1; i < n; ++i) {
            pair<int, int> cur = {cls[sa[i]], cls[(sa[i] + (1 << k)) % n]};
            pair<int, int> prev = {cls[sa[i - 1]], cls[(sa[i - 1] + (1 << k)) % n]};
            next_cls[sa[i]] = next_cls[sa[i - 1]] + (cur != prev);
        }
        cls = next_cls;
    }

    sa.erase(sa.begin());  // remove suffix starting at '$'
    int m = (int)sa.size();
    vector<int> rank(m), lcp(max(0, m - 1), 0);
    for (int i = 0; i < m; ++i) rank[sa[i]] = i;

    for (int i = 0, common = 0; i < m; ++i) {
        if (rank[i] == m - 1) {
            common = 0;
            continue;
        }
        int j = sa[rank[i] + 1];
        while (i + common < m && j + common < m && original[i + common] == original[j + common]) {
            ++common;
        }
        lcp[rank[i]] = common;
        if (common) --common;
    }
    return {sa, lcp};
}

// Example application: number of distinct substrings.
long long count_distinct_substrings_with_sa(const string& s) {
    pair<vector<int>, vector<int>> result = build_suffix_array(s);
    const vector<int>& sa = result.first;
    const vector<int>& lcp = result.second;
    long long total = 0;
    for (int pos : sa) total += (int)s.size() - pos;
    for (int x : lcp) total -= x;
    return total;
}
