from math import isqrt


def primes_up_to(limit):
    sieve = bytearray(b"\x01") * (limit + 1)
    sieve[0:2] = b"\x00\x00"
    for p in range(2, isqrt(limit) + 1):
        if sieve[p]:
            sieve[p * p : limit + 1 : p] = b"\x00" * (((limit - p * p) // p) + 1)
    return [p for p in range(2, limit + 1) if sieve[p]]


def solve():
    limit = 64_000_000
    block_size = 1_000_000
    primes = primes_up_to(isqrt(limit - 1))

    answer = 0

    for block_start in range(1, limit, block_size):
        block_end = min(limit, block_start + block_size)
        size = block_end - block_start

        values = [block_start + i for i in range(size)]
        sigma2 = [1] * size

        for p in primes:
            p2 = p * p
            first = ((block_start + p - 1) // p) * p

            for multiple in range(first, block_end, p):
                idx = multiple - block_start
                term = 1
                power = 1

                while values[idx] % p == 0:
                    values[idx] //= p
                    power *= p2
                    term += power

                sigma2[idx] *= term

        for offset, remaining in enumerate(values):
            if remaining > 1:
                sigma2[offset] *= 1 + remaining * remaining

            total = sigma2[offset]
            root = isqrt(total)
            if root * root == total:
                answer += block_start + offset

    print(answer)


if __name__ == "__main__":
    solve()
