template <int MOD>
int mod_pow_int(int a, long long e) {
    long long result = 1;
    long long base = a;
    while (e > 0) {
        if (e & 1) result = result * base % MOD;
        base = base * base % MOD;
        e >>= 1;
    }
    return (int)result;
}

template <int MOD>
int lagrange_interpolate(const vector<int>& y, long long x) {
    int n = (int)y.size() - 1;
    if (0 <= x && x <= n) return y[(int)x];

    vector<int> fact(n + 2, 1), inv_fact(n + 2, 1);
    for (int i = 1; i <= n + 1; ++i) fact[i] = (long long)fact[i - 1] * i % MOD;
    inv_fact[n + 1] = mod_pow_int<MOD>(fact[n + 1], MOD - 2);
    for (int i = n; i >= 0; --i) inv_fact[i] = (long long)inv_fact[i + 1] * (i + 1) % MOD;

    vector<int> pref(n + 2, 1), suff(n + 2, 1);
    for (int i = 0; i <= n; ++i) pref[i + 1] = (long long)pref[i] * ((x - i) % MOD + MOD) % MOD;
    for (int i = n; i >= 0; --i) suff[i] = (long long)suff[i + 1] * ((x - i) % MOD + MOD) % MOD;

    long long answer = 0;
    for (int i = 0; i <= n; ++i) {
        long long numerator = (long long)pref[i] * suff[i + 1] % MOD;
        long long denominator = (long long)inv_fact[i] * inv_fact[n - i] % MOD;
        if ((n - i) & 1) denominator = (MOD - denominator) % MOD;
        answer = (answer + 1LL * y[i] * numerator % MOD * denominator) % MOD;
    }
    return (int)answer;
}
