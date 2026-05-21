# Problem 214: Totient Chains

## Problem Statement

Let $\phi$ denote Euler's totient function. The **totient chain** of $n$ is
$$
n \to \phi(n) \to \phi(\phi(n)) \to \cdots \to 1.
$$

The **length** of the chain is the number of elements, including both $n$ and $1$. For example, the chain of 12 is
$$
12 \to 4 \to 2 \to 1,
$$
which has length 4.

Find the sum of all primes $p < 4 \times 10^7$ such that the totient chain of $p$ has length exactly 25.

## Mathematical Development

**Theorem 1 (Euler's product formula).** *For $n = p_1^{a_1} \cdots p_k^{a_k}$ with distinct primes $p_i$,*
$$
\phi(n) = n \prod_{i=1}^{k} \left(1 - \frac{1}{p_i}\right).
$$

*Proof.* By inclusion-exclusion on $\{1, \ldots, n\}$, the count of integers coprime to $n$ is
$$
\phi(n) = n - \sum_{p_i \mid n} \frac{n}{p_i} + \sum_{p_i < p_j} \frac{n}{p_i p_j} - \cdots
= n \prod_{i=1}^{k}\left(1 - \frac{1}{p_i}\right). \qquad \square
$$

**Lemma 1 (Strict decrease).** *For all $n \geq 2$, $\phi(n) < n$.*

*Proof.* If $n \geq 2$, then $n$ has at least one prime factor $p$, so
$$
\phi(n) = n \prod_{q \mid n} \left(1 - \frac{1}{q}\right) \leq n\left(1 - \frac{1}{p}\right) < n.
$$
$\square$

**Theorem 2 (Chain recurrence).** *If $\ell(n)$ denotes the totient-chain length, then $\ell(1) = 1$ and*
$$
\ell(n) = 1 + \ell(\phi(n))
$$
*for all $n \geq 2$.*

*Proof.* The chain of $n$ is $n$ followed by the chain of $\phi(n)$. Since $\phi(n) < n$ by Lemma 1, repeated iteration must eventually reach 1. $\square$

**Lemma 2 (Totient sieve correctness).** *Initialize $\phi[n] = n$ for all $n$. For each prime $p$, updating every multiple $m$ by*
$$
\phi[m] \leftarrow \phi[m] \cdot \frac{p-1}{p}
$$
*produces the correct totient values for all $m$.*

*Proof.* Each prime divisor $p$ of $m$ contributes exactly one factor $(1 - 1/p)$ in Euler's product formula, and the sieve applies those factors once per prime divisor. $\square$

**Lemma 3 (Prime detection).** *For $n \geq 2$, we have $\phi(n) = n - 1$ if and only if $n$ is prime.*

*Proof.* If $n$ is prime, then every positive integer below it is coprime to $n$, so $\phi(n) = n - 1$. Conversely, a composite $n$ has a nontrivial prime factor $p$, so at least the multiples of $p$ below $n$ are excluded, forcing $\phi(n) < n - 1$. $\square$

## Editorial

The recursion for the chain length is the important structural fact: once $\phi(n)$ is known, the chain length of $n$ is just one more than the chain length of a smaller number. That means a sieve order is perfect for this problem, because when we reach $n$, both $\phi(n)$ and $\ell(\phi(n))$ are already available.

So we combine three tasks in one pass. First compute the totient values with the standard sieve. Then, in increasing order, use
$$
\ell(n) = 1 + \ell(\phi(n))
$$
to fill the chain-length array. Whenever $n$ is prime, identified by $\phi(n) = n-1$, check whether the chain length is 25 and add it if it is.

## Pseudocode

```text
Set LIMIT = 40,000,000.
Initialize phi[n] = n for 0 <= n < LIMIT.
Initialize chain[1] = 1 and chain[n] = 0 otherwise.
answer = 0

For n from 2 to LIMIT - 1:
    If phi[n] = n:
        For each multiple m of n:
            phi[m] -= phi[m] / n

    chain[n] = chain[phi[n]] + 1

    If phi[n] = n - 1 and chain[n] = 25:
        answer += n

Return answer
```

## Complexity Analysis

- **Time:** $O(N \log \log N)$ for the totient sieve, plus $O(N)$ for the chain updates and prime filtering.
- **Space:** $O(N)$ for the totient table and the chain-length table.

## Answer

$$\boxed{1677366278943}$$
