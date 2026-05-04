#include <bits/stdc++.h>

using namespace std;

struct DigitDpNoAdjacent {
    string digits;
    long long memo[20][11];
    bool seen[20][11];

    long long dfs(int pos, int prev, bool tight) {
        if (pos == (int)digits.size()) return 1;
        if (!tight && seen[pos][prev]) return memo[pos][prev];

        int limit = tight ? digits[pos] - '0' : 9;
        long long answer = 0;
        for (int digit = 0; digit <= limit; ++digit) {
            int next_prev = prev;
            if (prev == 10 && digit == 0) {
                next_prev = 10;  // still only leading zeros
            } else {
                if (digit == prev) continue;
                next_prev = digit;
            }
            answer += dfs(pos + 1, next_prev, tight && digit == limit);
        }

        if (!tight) {
            seen[pos][prev] = true;
            memo[pos][prev] = answer;
        }
        return answer;
    }

    long long count_up_to(long long limit) {
        if (limit < 0) return 0;
        digits = to_string(limit);
        memset(seen, 0, sizeof(seen));
        return dfs(0, 10, true);
    }

    long long count_in_range(long long left, long long right) {
        return count_up_to(right) - count_up_to(left - 1);
    }
};
