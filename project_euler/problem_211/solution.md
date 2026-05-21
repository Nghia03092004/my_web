# Problem 211: Divisor Square Sum

## Problem Statement

For a positive integer $n$, let $\sigma_2(n)$ denote the sum of the squares of the divisors of $n$:

$$
\sigma_2(n) = \sum_{d \mid n} d^2
$$

For example, $\sigma_2(10) = 1^2 + 2^2 + 5^2 + 10^2 = 130$.

Find the sum of all $n$, $0 < n < 64{,}000{,}000$, such that $\sigma_2(n)$ is a perfect square.

## Mathematical Development

**Definition 1.** For $k \in \mathbb{Z}_{\geq 0}$, the *divisor power sum function* is $\sigma_k(n) = \sum_{d \mid n} d^k$. The case $k = 2$ gives the sum-of-squares-of-divisors function.

**Theorem 1 (Multiplicativity of $\sigma_2$).** *The function $\sigma_2$ is multiplicative: if $\gcd(a, b) = 1$, then $\sigma_2(ab) = \sigma_2(a) \cdot \sigma_2(b)$.*

*Proof.* Let $a, b \in \mathbb{Z}_{>0}$ with $\gcd(a, b) = 1$. We claim the map
$$
\varphi \colon \{d_1 : d_1 \mid a\} \times \{d_2 : d_2 \mid b\} \to \{d : d \mid ab\},
\qquad
\varphi(d_1, d_2) = d_1 d_2
$$
is a bijection.

*Surjectivity.* Let $d \mid ab$. Write $d = \prod_p p^{e_p}$. Since $\gcd(a, b) = 1$, each prime $p$ divides at most one of $a$ or $b$. Set
$$
d_1 = \prod_{p \mid a} p^{e_p},
\qquad
d_2 = \prod_{p \mid b} p^{e_p}.
$$
Then $d_1 \mid a$, $d_2 \mid b$, and $d = d_1 d_2$.

*Injectivity.* If $d_1 d_2 = d_1' d_2'$ with $d_1, d_1' \mid a$ and $d_2, d_2' \mid b$, then $\gcd(d_1, d_2') = 1$, so $d_1 \mid d_1'$. By symmetry $d_1' \mid d_1$, hence $d_1 = d_1'$ and then $d_2 = d_2'$.

Therefore
$$
\sigma_2(ab)
= \sum_{d \mid ab} d^2
= \sum_{d_1 \mid a} \sum_{d_2 \mid b} (d_1 d_2)^2
= \left(\sum_{d_1 \mid a} d_1^2\right)\left(\sum_{d_2 \mid b} d_2^2\right)
= \sigma_2(a)\sigma_2(b). \qquad \square
$$

**Lemma 1 (Geometric sum for prime powers).** *For a prime $p$ and integer $a \geq 0$,*
$$
\sigma_2(p^a) = \sum_{j=0}^{a} p^{2j} = \frac{p^{2(a+1)} - 1}{p^2 - 1}.
$$

*Proof.* The divisors of $p^a$ are precisely $1, p, p^2, \ldots, p^a$. Hence
$$
\sigma_2(p^a) = \sum_{j=0}^{a} (p^j)^2 = \sum_{j=0}^{a} p^{2j},
$$
which is a finite geometric series with ratio $p^2$. $\square$

**Proposition 1 (Overflow bound).** *For $n < N = 6.4 \times 10^7$, unsigned 64-bit integers suffice to store $\sigma_2(n)$ in the sieve implementation.*

**Theorem 2 (Segmented factorization formula).** *Let*
$$
n = \prod_{i=1}^{r} p_i^{a_i}
$$
*be the prime factorization of $n$. If we process each prime factor $p_i$ once, multiply an accumulator by*
$$
1 + p_i^2 + p_i^4 + \cdots + p_i^{2a_i},
$$
*and remove $p_i^{a_i}$ from a working copy of $n$, then the final accumulator equals $\sigma_2(n)$. If a residual factor remains after all primes up to $\sqrt{n}$ have been removed, that residual factor is prime and contributes a final factor $1 + q^2$.*

*Proof.* The product formula
$$
\sigma_2(n) = \prod_{i=1}^{r} \sigma_2(p_i^{a_i})
= \prod_{i=1}^{r} \left(1 + p_i^2 + p_i^4 + \cdots + p_i^{2a_i}\right)
$$
follows immediately from Theorem 1 and Lemma 1.

Now remove the prime powers from a working copy of $n$. When all primes up to $\sqrt{n}$ have been processed, any remaining factor $q > 1$ cannot be composite, because a composite number has a prime divisor at most its square root. Therefore the remaining factor is either $1$ or a single prime, and multiplying by $1 + q^2$ completes the product for $\sigma_2(n)$. $\square$

**Lemma 2 (Perfect square detection).** *A nonnegative integer $s$ is a perfect square if and only if $\lfloor \sqrt{s} \rfloor^2 = s$. Integer square root can be computed exactly via Newton's method or a built-in `isqrt`.*

## Editorial

The useful structural fact is that $\sigma_2$ is multiplicative, so once the prime factorization of $n$ is known we can recover $\sigma_2(n)$ from the geometric-series factors
$$
1 + p^2 + p^4 + \cdots + p^{2a}.
$$
Factoring each integer from scratch would still be too slow, so the implementation works in blocks. Inside one block we keep the current values of the integers and an accumulator for their $\sigma_2$ product. Then, for each prime $p \le \sqrt{N}$, we walk through the multiples of $p$ inside that block, strip off the full power of $p$, and multiply the accumulator by the corresponding prime-power contribution.

After all small primes are processed, each number has at most one prime factor left. Multiplying by $1 + q^2$ finishes $\sigma_2(n)$, and then a single integer-square-root check tells us whether that value is a square. This keeps the same mathematics as the direct formula, but avoids storing a length-$64{,}000{,}000$ divisor table in Python.

## Pseudocode

```text
Set N = 64,000,000 and choose a block size B.
Generate all primes up to sqrt(N - 1).

answer = 0
For each block [L, R):
    values[i] = L + i for every index in the block
    sigma2[i] = 1 for every index in the block

    For each prime p in the prime list:
        Find the first multiple of p inside [L, R)

        For each multiple m of p inside the block:
            contribution = 1
            power = 1

            While values[m - L] is divisible by p:
                Divide values[m - L] by p
                power = power * p^2
                contribution = contribution + power

            Multiply sigma2[m - L] by contribution

    For each position i in the block:
        If values[i] > 1:
            Multiply sigma2[i] by 1 + values[i]^2

        r = integer square root of sigma2[i]
        If r * r = sigma2[i]:
            answer += L + i

Return answer
```

## Complexity Analysis

- **Time:** Roughly $O(N \log \log N)$ arithmetic over all blocks. Each prime visits its multiples, and each prime factor is removed only as many times as its exponent occurs.
- **Space:** $O(B + \pi(\sqrt{N}))$, where $B$ is the chosen block size.

## Answer

$$\boxed{1922364685}$$
