"""
Problem 202: Laserbeam

A laser beam enters vertex C of an equilateral triangle with mirrored sides,
reflects exactly 12017639147 times off the internal surfaces, and exits
through vertex C. Count the number of distinct beam paths.

Approach:
- Unfold reflections into a triangular tessellation.
- n = (R + 3) / 2 = 6008819575 is the lattice parameter.
- Count b in [1, n-1] with b congruent to 2 modulo 3 and gcd(b, n) = 1.
- Use Mobius inversion over squarefree divisors of n.

n = 6008819575 = 5^2 * 11 * 17 * 23 * 29 * 41 * 47
"""


def solve():
    r = 12017639147
    n = (r + 3) // 2

    primes = []
    tmp = n
    p = 2
    while p * p <= tmp:
        if tmp % p == 0:
            primes.append(p)
            while tmp % p == 0:
                tmp //= p
        p += 1
    if tmp > 1:
        primes.append(tmp)

    answer = 0
    for mask in range(1 << len(primes)):
        d = 1
        bits = 0
        for i, prime in enumerate(primes):
            if mask & (1 << i):
                d *= prime
                bits += 1

        mu = 1 if bits % 2 == 0 else -1
        nd = n // d

        if d % 3 == 0:
            count = 0
        else:
            inverse_mod_3 = 1 if d % 3 == 1 else 2
            residue = (2 * inverse_mod_3) % 3
            upper = nd - 1
            if residue == 0:
                count = (upper - 3) // 3 + 1 if upper >= 3 else 0
            else:
                count = (upper - residue) // 3 + 1 if upper >= residue else 0

        answer += mu * count

    print(answer)


if __name__ == "__main__":
    solve()
