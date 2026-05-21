#include <bits/stdc++.h>
using namespace std;

int main() {
    const int N = 100;
    const int K = 50;

    vector<int> squares(N);
    for (int i = 0; i < N; i++) {
        squares[i] = (i + 1) * (i + 1);
    }

    vector<int> prefix(N + 1, 0);
    for (int i = 0; i < N; i++) {
        prefix[i + 1] = prefix[i] + squares[i];
    }

    int minSum = prefix[K];
    int maxSum = prefix[N] - prefix[N - K];

    // dp[j][s] is 0, 1, or 2 depending on whether there are
    // zero, one, or at least two j-element subsets with sum s.
    vector<vector<uint8_t>> dp(K + 1, vector<uint8_t>(maxSum + 1, 0));
    dp[0][0] = 1;

    for (int i = 1; i <= N; i++) {
        int value = squares[i - 1];
        int previousTotal = prefix[i - 1];

        for (int size = min(i, K); size >= 1; size--) {
            int minPrev = prefix[size - 1];
            int maxPrev = previousTotal - prefix[i - size];
            auto& curr = dp[size];
            const auto& prev = dp[size - 1];

            for (int prevSum = maxPrev; prevSum >= minPrev; prevSum--) {
                if (prev[prevSum]) {
                    int newSum = prevSum + value;
                    int total = curr[newSum] + prev[prevSum];
                    curr[newSum] = static_cast<uint8_t>(min(total, 2));
                }
            }
        }
    }

    long long answer = 0;
    for (int sum = minSum; sum <= maxSum; sum++) {
        if (dp[K][sum] == 1) {
            answer += sum;
        }
    }

    cout << answer << '\n';
    return 0;
}
