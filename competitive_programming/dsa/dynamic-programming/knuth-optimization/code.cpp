long long minimum_adjacent_merge_cost(const vector<long long>& a) {
    int n = (int)a.size();
    vector<long long> pref(n + 1, 0);
    for (int i = 0; i < n; ++i) pref[i + 1] = pref[i] + a[i];

    auto range_sum = [&](int l, int r) {
        return pref[r + 1] - pref[l];
    };

    const long long INF = (1LL << 62);
    vector<vector<long long>> dp(n, vector<long long>(n, 0));
    vector<vector<int>> opt(n, vector<int>(n, 0));
    for (int i = 0; i < n; ++i) opt[i][i] = i;

    for (int len = 2; len <= n; ++len) {
        for (int l = 0; l + len - 1 < n; ++l) {
            int r = l + len - 1;
            dp[l][r] = INF;
            int start = opt[l][r - 1];
            int finish = opt[l + 1][r];
            for (int k = start; k <= finish; ++k) {
                long long candidate = dp[l][k] + dp[k + 1][r] + range_sum(l, r);
                if (candidate < dp[l][r]) {
                    dp[l][r] = candidate;
                    opt[l][r] = k;
                }
            }
        }
    }
    return dp[0][n - 1];
}
