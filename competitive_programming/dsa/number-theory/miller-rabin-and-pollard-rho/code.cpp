#include <bits/stdc++.h>

using namespace std;

using u64 = uint64_t;

u64 gcd_u64(u64 a, u64 b) {
    while (b != 0) {
        u64 t = a % b;
        a = b;
        b = t;
    }
    return a;
}

u64 mod_mul(u64 a, u64 b, u64 mod) {
    u64 result = 0;
    while (b > 0) {
        if (b & 1) {
            if (result >= mod - a) result -= mod - a;
            else result += a;
        }
        b >>= 1;
        if (b == 0) break;
        if (a >= mod - a) a -= mod - a;
        else a += a;
    }
    return result;
}

u64 mod_pow(u64 a, u64 e, u64 mod) {
    u64 result = 1;
    while (e > 0) {
        if (e & 1) result = mod_mul(result, a, mod);
        a = mod_mul(a, a, mod);
        e >>= 1;
    }
    return result;
}

bool is_prime(u64 n) {
    if (n < 2) return false;
    for (u64 p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
        if (n % p == 0) return n == p;
    }

    u64 d = n - 1, s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    auto witness = [&](u64 a) {
        u64 x = mod_pow(a % n, d, n);
        if (x == 1 || x == n - 1) return false;
        for (u64 r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) return false;
        }
        return true;
    };

    for (u64 a : {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL}) {
        if (a % n != 0 && witness(a)) return false;
    }
    return true;
}

u64 pollard_rho(u64 n) {
    if ((n & 1) == 0) return 2;
    static mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

    while (true) {
        u64 c = uniform_int_distribution<u64>(1, n - 1)(rng);
        u64 x = uniform_int_distribution<u64>(0, n - 1)(rng);
        u64 y = x;
        u64 d = 1;

        auto f = [&](u64 v) {
            return (mod_mul(v, v, n) + c) % n;
        };

        while (d == 1) {
            x = f(x);
            y = f(f(y));
            u64 diff = x > y ? x - y : y - x;
            d = gcd_u64(diff, n);
        }
        if (d != n) return d;
    }
}

void factor_rec(u64 n, vector<u64>& factors) {
    if (n == 1) return;
    if (is_prime(n)) {
        factors.push_back(n);
        return;
    }
    u64 d = pollard_rho(n);
    factor_rec(d, factors);
    factor_rec(n / d, factors);
}

vector<u64> factorize_u64(u64 n) {
    vector<u64> factors;
    factor_rec(n, factors);
    sort(factors.begin(), factors.end());
    return factors;
}

// Example application: largest prime factor of a 64-bit integer.
u64 largest_prime_factor(u64 n) {
    vector<u64> factors = factorize_u64(n);
    return factors.empty() ? 0 : factors.back();
}
