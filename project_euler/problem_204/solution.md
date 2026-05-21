# Problem 204: Generalised Hamming Numbers

## Problem Statement

A Hamming number is a positive integer which has no prime factor larger than 5. We define a **type-$t$ generalised Hamming number** as a positive integer which has no prime factor larger than $t$. How many type-100 generalised Hamming numbers (that is, 100-smooth numbers) do not exceed $10^9$?

## Mathematical Development

**Theorem (Fundamental Theorem of Arithmetic, applied to smooth numbers).** *Every 100-smooth number $n \geq 1$ can be written uniquely as*
$$
n = p_1^{a_1} p_2^{a_2} \cdots p_{25}^{a_{25}},
$$
*where $a_i \geq 0$ and $p_1 < p_2 < \cdots < p_{25}$ are the 25 primes not exceeding 100. Thus the 100-smooth numbers up to $N$ are in bijection with the exponent vectors $(a_1,\ldots,a_{25})$ satisfying $\prod p_i^{a_i} \leq N$.*

*Proof.* This is exactly the Fundamental Theorem of Arithmetic restricted to the prime set $\{2,3,5,\ldots,97\}$. $\square$

**Theorem (Recursive Counting).** *Define $\Psi(N, i)$ as the number of positive integers $\leq N$ whose prime factors all lie in $\{p_i, p_{i+1}, \ldots, p_{25}\}$. Then*
$$
\Psi(N, i) = \sum_{a=0}^{\lfloor \log_{p_i} N \rfloor} \Psi\!\left(\left\lfloor \frac{N}{p_i^a} \right\rfloor, i+1\right),
$$
*with base case $\Psi(N, 26) = 1$ for all $N \geq 1$. The desired answer is $\Psi(10^9, 1)$.*

*Proof.* Partition the valid numbers by the exponent of $p_i$. For a fixed exponent $a$, the remaining factor must be at most $N / p_i^a$ and may only use later primes. Summing over all admissible $a$ gives the recurrence. $\square$

**Lemma (Exponent Bounds).** *For each prime $p_i$, the maximum exponent satisfying $p_i^a \leq 10^9$ is $a_{\max}(p_i) = \lfloor \log_{p_i} 10^9 \rfloor$. In particular, $a_{\max}(2) = 29$, $a_{\max}(3) = 18$, $a_{\max}(5) = 12$, $a_{\max}(7) = 10$, and $a_{\max}(97) = 4$.*

*Proof.* Direct comparison of powers with $10^9$. $\square$

## Editorial

Every type-100 Hamming number is assembled from the 25 primes at most 100, so the problem is really about choosing exponent vectors rather than testing integers one by one. Once the prime list is fixed, a depth-first search can decide the exponent of the current prime, shrink the remaining budget accordingly, and recurse to the next prime.

This visits only valid smooth numbers. Large primes have very small exponent ranges, so the recursion tree thins out quickly, and every leaf corresponds to one admissible product. That is much more efficient than scanning all numbers up to $10^9$ and factoring them.

## Pseudocode

```text
List all primes p_1, p_2, ..., p_25 not exceeding 100.

Define Count(index, current_product):
    If index = 25:
        return 1

    p = p_index
    total = 0
    power = 1

    While current_product * power <= 10^9:
        total += Count(index + 1, current_product * power)
        power *= p

    return total

Return Count(0, 1)
```

## Complexity Analysis

- **Time:** Proportional to the number of recursive states visited, which is on the order of the number of 100-smooth numbers up to $10^9$.
- **Space:** $O(25)$ for the recursion stack.

## Answer

$$\boxed{2944730}$$
