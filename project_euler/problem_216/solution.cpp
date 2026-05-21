#include <bits/stdc++.h>
using namespace std;

// Problem 216: Count primes in t(n) = 2n^2 - 1 for 2 <= n <= 50,000,000
// Uses a sieve approach: for each prime p, find roots of 2x^2 = 1 (mod p)
// and sieve out multiples.

const int N = 50000000;

// We store t[n] and divide out small prime factors via sieve.
// After sieving, if remaining value > 1, t(n) is prime.
// We use long long since t(N) ~ 5e15.

// For memory, we store the "remaining" factor of t(n).
// t(n) = 2n^2 - 1. We sieve primes p up to sqrt(2*N^2) ~ 1e8.

// Actually, a simpler approach: store t[n] as long long, sieve by dividing.
// But 50M long longs = 400MB, too much.

// Better approach: segmented sieve on n. For each segment of n-values,
// compute t(n), sieve small primes, check if remainder is 1 or t(n).

// Even simpler: use a boolean array. Mark n as "composite t(n)" when we find
// a prime dividing t(n). But t(n) could have all prime factors > sieve limit
// only if t(n) is prime or a product of two large primes.
// If t(n) has a factor <= sqrt(t(n)) ~ 7e7, we'll find it.
// Sieve limit: sqrt(2 * 50000000^2 - 1) ~ 70710678

// We sieve primes up to ~70710678 using standard sieve, then for each such prime p,
// find roots of 2x^2 = 1 mod p, and mark those n as having a small factor.
// But this doesn't fully work because t(n) might be a prime power or product of
// primes all > sqrt(t(n)). Actually if t(n) = a*b with a,b > sqrt(t(n)), impossible.
// So if no prime <= sqrt(t(n)) divides t(n), then t(n) is prime.
// sqrt(t(n)) varies with n. sqrt(t(N)) ~ 7.07e7.
// For smaller n, sqrt(t(n)) is smaller, so sieving up to 7.07e7 covers everything.

// Memory for boolean sieve of primes up to 7.07e7: ~70MB (bitset or vector<bool>).
// Boolean array for n: 50MB.

// Approach:
// 1) Sieve primes up to ~70710678.
// 2) For each prime p, find roots r1, r2 of 2x^2 = 1 mod p.
//    This requires p | (2x^2 - 1), so 2 must be a QR mod p, i.e., p = +/-1 mod 8.
//    (But we also need p odd and p >= 3.)
//    Actually, p=2: t(n) = 2n^2-1 is always odd, so p=2 never divides t(n).
// 3) For each root r, mark r, r+p, r+2p, ... as composite.
//    Also mark p-r, p-r+p, ... as composite.
// 4) Count n in [2, N] not marked.

// But wait: marking n as composite just because p | t(n) is wrong if p^2 | t(n)
// or if t(n) = p. We need: t(n) is NOT prime, so we should mark n only if t(n) != p.
// Actually, if p | t(n) and t(n) > p, then t(n) is composite.
// If t(n) = p, then t(n) is prime and we should NOT mark it.
// t(n) = p means 2n^2 - 1 = p, so n = sqrt((p+1)/2). This only happens for
// specific small n/p values. We can handle this edge case.

// For p | t(n) with p < t(n): t(n) is composite.
// For p | t(n) with p = t(n): t(n) is prime. Don't mark.
// t(n) = p iff 2n^2 - 1 = p, i.e., n^2 = (p+1)/2.

long long modpow(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

// Tonelli-Shanks: find x such that x^2 = a mod p
long long sqrt_mod(long long a, long long p) {
    if (a == 0) return 0;
    if (p == 2) return a & 1;

    // Check if a is QR
    if (modpow(a, (p - 1) / 2, p) != 1) return -1;

    if (p % 4 == 3) {
        return modpow(a, (p + 1) / 4, p);
    }

    // Factor p-1 = 2^s * q
    long long s = 0, q = p - 1;
    while (q % 2 == 0) { s++; q /= 2; }

    // Find non-residue
    long long z = 2;
    while (modpow(z, (p - 1) / 2, p) != p - 1) z++;

    long long M = s;
    long long c = modpow(z, q, p);
    long long t = modpow(a, q, p);
    long long R = modpow(a, (q + 1) / 2, p);

    while (true) {
        if (t == 1) return R;
        long long i = 1;
        long long tmp = t * t % p;
        while (tmp != 1) { tmp = tmp * tmp % p; i++; }
        long long b = c;
        for (long long j = 0; j < M - i - 1; j++) b = b * b % p;
        M = i;
        c = b * b % p;
        t = t * c % p;
        R = R * b % p;
    }
}

int main() {
    const int LIMIT = N;
    const int SIEVE_LIMIT = 70710679; // > sqrt(2 * N^2)

    // Step 1: Sieve primes up to SIEVE_LIMIT
    vector<bool> is_prime_sieve(SIEVE_LIMIT + 1, true);
    is_prime_sieve[0] = is_prime_sieve[1] = false;
    for (long long i = 2; i * i <= SIEVE_LIMIT; i++) {
        if (is_prime_sieve[i]) {
            for (long long j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime_sieve[j] = false;
        }
    }

    // Step 2: For each prime, sieve t(n)
    // is_composite[n] = true means t(n) is composite
    vector<bool> is_composite(LIMIT + 1, false);

    for (long long p = 3; p <= SIEVE_LIMIT; p++) {
        if (!is_prime_sieve[p]) continue;
        // Check if 2 is QR mod p: p = +/-1 mod 8
        if (p % 8 != 1 && p % 8 != 7) continue;

        // Find n such that 2n^2 = 1 mod p, i.e., n^2 = (p+1)/2 mod p
        long long half_inv = (p + 1) / 2; // This is 2^{-1} mod p
        long long r = sqrt_mod(half_inv, p);
        if (r < 0) continue;

        // Two roots: r and p - r
        // For each root, sieve n = root, root+p, root+2p, ...
        // But skip if t(n) = p (meaning n^2 = (p+1)/2 exactly)

        long long p_check = -1;
        // t(n) = p iff 2n^2-1 = p iff n^2 = (p+1)/2
        {
            long long val = (p + 1) / 2;
            long long sq = (long long)sqrt((double)val);
            while (sq * sq < val) sq++;
            while (sq * sq > val) sq--;
            if (sq * sq == val && sq >= 2 && sq <= LIMIT) p_check = sq;
        }

        for (int sign = 0; sign < 2; sign++) {
            long long root = (sign == 0) ? r : p - r;
            if (root == 0) continue;
            // start from root, step by p
            for (long long n = root; n <= LIMIT; n += p) {
                if (n >= 2 && n != p_check) {
                    is_composite[n] = true;
                }
            }
        }
    }

    // Step 3: Count primes
    int count = 0;
    for (int n = 2; n <= LIMIT; n++) {
        if (!is_composite[n]) count++;
    }

    cout << count << endl;
    return 0;
}
