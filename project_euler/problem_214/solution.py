from array import array


def solve():
    limit = 40_000_000

    phi = array("I", range(limit))
    chain = bytearray(limit)
    chain[1] = 1

    answer = 0

    for n in range(2, limit):
        if phi[n] == n:
            for multiple in range(n, limit, n):
                phi[multiple] -= phi[multiple] // n

        chain[n] = chain[phi[n]] + 1
        if phi[n] == n - 1 and chain[n] == 25:
            answer += n

    print(answer)


if __name__ == "__main__":
    solve()
