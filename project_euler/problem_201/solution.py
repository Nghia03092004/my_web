"""
Problem 201: Subsets with a Unique Sum

For S = {1^2, 2^2, ..., 100^2}, find the sum of all integers that are the
sum of exactly one subset of S of size 50.

For each subset size we maintain two bitsets:
- once[j]: sums achievable in exactly one way
- many[j]: sums achievable in at least two ways
"""


def solve():
    n = 100
    k = 50
    elements = [i * i for i in range(1, n + 1)]

    prefix = [0]
    for value in elements:
        prefix.append(prefix[-1] + value)

    min_sum = prefix[k]
    max_sum = prefix[n] - prefix[n - k]
    mask = (1 << (max_sum + 1)) - 1

    once = [0] * (k + 1)
    many = [0] * (k + 1)
    once[0] = 1  # Only sum 0 is achievable with 0 elements.

    for value in elements:
        for size in range(k, 0, -1):
            generated_once = (once[size - 1] << value) & mask
            generated_many = (many[size - 1] << value) & mask

            new_many = many[size] | generated_many | (once[size] & generated_once)
            new_once = (once[size] ^ generated_once) & ~new_many

            many[size] = new_many
            once[size] = new_once & mask

    answer = 0
    bits = once[k]
    for total in range(min_sum, max_sum + 1):
        if (bits >> total) & 1:
            answer += total

    print(answer)


if __name__ == "__main__":
    solve()
