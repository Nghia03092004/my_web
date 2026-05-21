"""
Problem 209: Circular Logic

Count 6-input truth tables tau such that
tau(a,b,c,d,e,f) AND tau(b,c,d,e,f, a XOR (b AND c)) = 0
for all inputs.

This is equivalent to counting independent sets on the cycle graph
formed by the permutation sigma on {0,1}^6.
The answer is the product of Lucas numbers over all cycle lengths.
"""


def solve():
    def sigma(x):
        a = (x >> 5) & 1
        b = (x >> 4) & 1
        c = (x >> 3) & 1
        d = (x >> 2) & 1
        e = (x >> 1) & 1
        f = x & 1
        new_f = a ^ (b & c)
        return (b << 5) | (c << 4) | (d << 3) | (e << 2) | (f << 1) | new_f

    visited = [False] * 64
    cycle_lengths = []

    for start in range(64):
        if visited[start]:
            continue
        length = 0
        cur = start
        while not visited[cur]:
            visited[cur] = True
            cur = sigma(cur)
            length += 1
        cycle_lengths.append(length)

    def lucas(n):
        if n == 1:
            return 1
        if n == 2:
            return 3
        a, b = 1, 3
        for _ in range(3, n + 1):
            a, b = b, a + b
        return b

    answer = 1
    for length in cycle_lengths:
        answer *= lucas(length)

    return answer


answer = solve()
assert answer == 15964587728784
print(answer)
