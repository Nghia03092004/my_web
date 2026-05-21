#include <bits/stdc++.h>
using namespace std;

int main() {
    // O = (0,0), C = (r/4, r/4), B = lattice point with |x|+|y| <= r
    // Count B where triangle OBC is obtuse.
    //
    // Three disjoint obtuse regions:
    // Region O (obtuse at O): x+y < 0, count = r^2
    // Region C (obtuse at C): x+y > r/2, count = r^2/2
    // Region B (obtuse at B): (x-r/8)^2+(y-r/8)^2 < r^2/32, x != y
    //   circle_count - collinear_count, where collinear = r/4 - 1
    //
    // N(r) = r^2 + r^2/2 + circle_count - (r/4 - 1)
    //       = 3r^2/2 - r/4 + 1 + circle_count

    long long r = 1000000000LL;
    long long s = r / 8; // 125000000, center of circle for region B

    // Region O + Region C
    long long region_OC = r * r + r * r / 2; // 3r^2/2

    // Region B: circle centered at (s, s), radius^2 = R2 = r^2/32 = 2*s^2
    // Count lattice points (x,y) with (x-s)^2 + (y-s)^2 < R2
    // Equivalently, (u,v) = (x-s, y-s), count u^2+v^2 < R2 = 2*s^2

    long long R2 = 2LL * s * s; // = 2 * 125000000^2 = 31250000000000000

    // For each u from -max_u to max_u, count v with v^2 < R2 - u^2
    // max_u: u^2 < R2, |u| <= isqrt(R2 - 1)

    // isqrt function
    auto isqrt = [](long long n) -> long long {
        if (n < 0) return -1;
        long long q = (long long)sqrt((double)n);
        while (q * q > n) q--;
        while ((q + 1) * (q + 1) <= n) q++;
        return q;
    };

    long long max_u = isqrt(R2 - 1);

    long long circle_count = 0;
    // Use symmetry: u and -u give same count, and swap u,v gives same count
    // circle_count = sum_{u=-max_u}^{max_u} (2*max_v(u) + 1)
    // By symmetry in u: = (2*max_v(0)+1) + 2*sum_{u=1}^{max_u} (2*max_v(u)+1)

    {
        // u = 0
        long long rem = R2;
        long long q = isqrt(rem);
        long long mv = (q * q == rem) ? q - 1 : q;
        circle_count += 2 * mv + 1;
    }

    for (long long u = 1; u <= max_u; u++) {
        long long rem = R2 - u * u;
        if (rem <= 0) break;
        long long q = isqrt(rem);
        long long mv = (q * q == rem) ? q - 1 : q;
        circle_count += 2 * (2 * mv + 1); // factor 2 for +u and -u
    }

    // Collinear points in circle: u = v, 2u^2 < R2 = 2s^2, so u^2 < s^2, |u| <= s-1
    // Count = 2*(s-1) + 1 = 2s - 1 = r/4 - 1
    long long collinear = r / 4 - 1;
    long long region_B = circle_count - collinear;
    long long answer = region_OC + region_B;

    cout << answer << endl;

    return 0;
}
