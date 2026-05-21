#include <bits/stdc++.h>
using namespace std;

int main() {
    long long R = 12017639147LL;
    long long n = (R + 3) / 2;

    vector<long long> primes;
    long long tmp = n;
    for (long long p = 2; p * p <= tmp; p++) {
        if (tmp % p == 0) {
            primes.push_back(p);
            while (tmp % p == 0) {
                tmp /= p;
            }
        }
    }
    if (tmp > 1) {
        primes.push_back(tmp);
    }

    long long answer = 0;
    for (int mask = 0; mask < (1 << static_cast<int>(primes.size())); mask++) {
        long long d = 1;
        int bits = 0;
        for (int i = 0; i < static_cast<int>(primes.size()); i++) {
            if (mask & (1 << i)) {
                d *= primes[i];
                bits++;
            }
        }

        long long mu = (bits % 2 == 0) ? 1 : -1;
        long long nd = n / d;
        long long count = 0;

        if (d % 3 != 0) {
            long long inverseMod3 = (d % 3 == 1) ? 1 : 2;
            long long residue = (2 * inverseMod3) % 3;
            long long upper = nd - 1;

            if (residue == 0) {
                if (upper >= 3) {
                    count = (upper - 3) / 3 + 1;
                }
            } else {
                if (upper >= residue) {
                    count = (upper - residue) / 3 + 1;
                }
            }
        }

        answer += mu * count;
    }

    cout << answer << '\n';
    return 0;
}
