struct MobiusSieve {
    vector<int> primes;
    vector<int> mu;
    vector<bool> is_composite;

    explicit MobiusSieve(int n) : mu(n + 1, 0), is_composite(n + 1, false) {
        mu[1] = 1;
        for (int i = 2; i <= n; ++i) {
            if (!is_composite[i]) {
                primes.push_back(i);
                mu[i] = -1;
            }
            for (int p : primes) {
                if (1LL * i * p > n) break;
                is_composite[i * p] = true;
                if (i % p == 0) {
                    mu[i * p] = 0;
                    break;
                }
                mu[i * p] = -mu[i];
            }
        }
    }
};

long long count_coprime_pairs(int n, int m) {
    int limit = min(n, m);
    MobiusSieve sieve(limit);
    long long answer = 0;
    for (int d = 1; d <= limit; ++d) {
        answer += 1LL * sieve.mu[d] * (n / d) * (m / d);
    }
    return answer;
}
