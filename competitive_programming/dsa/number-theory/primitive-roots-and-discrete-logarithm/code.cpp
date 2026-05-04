long long mod_pow(long long a, long long e, long long mod) {
    long long result = 1 % mod;
    while (e > 0) {
        if (e & 1) result = (__int128)result * a % mod;
        a = (__int128)a * a % mod;
        e >>= 1;
    }
    return result;
}

vector<long long> distinct_prime_factors(long long n) {
    vector<long long> factors;
    for (long long p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            factors.push_back(p);
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) factors.push_back(n);
    return factors;
}

long long primitive_root_prime(long long p) {
    vector<long long> factors = distinct_prime_factors(p - 1);
    for (long long g = 2; g < p; ++g) {
        bool ok = true;
        for (long long q : factors) {
            if (mod_pow(g, (p - 1) / q, p) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return g;
    }
    return -1;
}

long long discrete_log_prime(long long g, long long h, long long p) {
    long long n = (long long)sqrt((long double)p) + 1;
    unordered_map<long long, long long> baby;

    long long value = 1;
    for (long long j = 0; j < n; ++j) {
        if (!baby.count(value)) baby[value] = j;
        value = (__int128)value * g % p;
    }

    long long factor = mod_pow(mod_pow(g, p - 2, p), n, p);
    value = h % p;
    for (long long i = 0; i <= n; ++i) {
        auto it = baby.find(value);
        if (it != baby.end()) {
            return i * n + it->second;
        }
        value = (__int128)value * factor % p;
    }
    return -1;
}
