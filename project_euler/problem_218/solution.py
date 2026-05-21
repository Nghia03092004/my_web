"""
Problem 218: Perfect Right-angled Triangles

Every perfect right-angled triangle has area divisible by 84.
Therefore no perfect triangle can fail both the divisibility-by-6
and divisibility-by-28 tests.
"""


def count_non_super_perfect(limit):
    # The proof is independent of the hypotenuse bound.
    return 0


def solve():
    print(count_non_super_perfect(10**16))


if __name__ == "__main__":
    solve()
