#include <bits/stdc++.h>
using namespace std;

long long countNonSuperPerfect(long long /*limit*/) {
    // The proof shows every perfect right-angled triangle has area divisible by 84,
    // so none can fail both the divisibility-by-6 and divisibility-by-28 tests.
    return 0;
}

int main() {
    cout << countNonSuperPerfect(10000000000000000LL) << '\n';
    return 0;
}
