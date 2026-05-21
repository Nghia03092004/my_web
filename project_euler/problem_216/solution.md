# Problem 216: Investigating the Primality of $2n^2 - 1$

## Problem Statement

Consider the sequence
$$
t(n) = 2n^2 - 1
$$
for $n \geq 1$:
$$
1, 7, 17, 31, 49, 71, 97, 127, 161, 199, \ldots
$$

How many values of $t(n)$ are prime for $2 \leq n \leq 50{,}000{,}000$?

## Mathematical Development

**Theorem 1 (Quadratic residue condition).** *If an odd prime $p$ divides $t(n) = 2n^2 - 1$, then*
$$
p \equiv \pm 1 \pmod{8}.
$$

*Proof.* From $p \mid 2n^2 - 1$ we get
$$
2n^2 \equiv 1 \pmod{p},
$$
so $2^{-1}$ is a quadratic residue modulo $p$. Equivalently,
$$
\left(\frac{2}{p}\right) = 1.
$$
By the second supplement to quadratic reciprocity,
$$
\left(\frac{2}{p}\right) = (-1)^{(p^2 - 1)/8},
$$
which equals 1 exactly for $p \equiv \pm 1 \pmod{8}$. $\square$

**Theorem 2 (Root periodicity).** *If $p \mid t(n_0)$, then*
$$
p \mid t(n_0 + kp)
$$
*for every integer $k$. Moreover, when the congruence*
$$
2x^2 \equiv 1 \pmod{p}
$$
*has a solution, its two roots are $r$ and $p-r$.*

*Proof.* Expanding gives
$$
t(n_0 + kp) = 2(n_0 + kp)^2 - 1 \equiv 2n_0^2 - 1 \equiv 0 \pmod{p}.
$$
If $r^2 \equiv 2^{-1} \pmod{p}$, then $(p-r)^2 \equiv r^2 \pmod{p}$ as well. Since a quadratic congruence over $\mathbb{F}_p$ has at most two roots, these are the only solutions. $\square$

**Lemma 1 (Tonelli-Shanks).** *For an odd prime $p$ and a quadratic residue $a \pmod p$, the Tonelli-Shanks algorithm finds a square root of $a$ modulo $p$ in polylogarithmic time.*

**Theorem 3 (Boolean residue-class sieve).** *Maintain an array `is_composite[n]`, initially false for $2 \leq n \leq N$. For each prime*
$$
p \leq \sqrt{2N^2}
$$
*with $p \equiv \pm 1 \pmod{8}$: solve*
$$
2x^2 \equiv 1 \pmod{p},
$$
*obtaining roots $r$ and $p-r$, and mark every $n \equiv r \pmod p$ or $n \equiv -r \pmod p$ as composite, except for the unique case $t(n) = p$. After all such primes are processed, `is_composite[n]` is false exactly when $t(n)$ is prime.*

*Proof.* If $n$ is marked for some prime $p$, then $p \mid t(n)$. The skipped exceptional case is precisely $t(n)=p$, which is prime; every other marked value is composite.

Conversely, if $t(n)$ is composite, it has some prime divisor
$$
q \leq \sqrt{t(n)} \leq \sqrt{2N^2}.
$$
By Theorem 1, $q \equiv \pm 1 \pmod{8}$, so the congruence $2x^2 \equiv 1 \pmod q$ has the relevant roots, and by Theorem 2 the value of $n$ lies in one of the marked residue classes modulo $q$. Therefore every composite $t(n)$ is marked. $\square$

## Editorial

The sieve works on the parameter $n$, not on the values $2n^2-1$ themselves. For a prime $p$ to divide $2n^2-1$, the congruence
$$
2n^2 \equiv 1 \pmod p
$$
must be solvable, so only primes with $p \equiv \pm 1 \pmod 8$ matter. Once such a prime is fixed, its solutions form two residue classes modulo $p$, and every $n$ in those classes produces a composite value unless $2n^2-1$ happens to equal $p$ itself.

That turns the problem into a quadratic-residue sieve. Generate the relevant primes, use Tonelli-Shanks to find the two roots of the congruence, mark the corresponding arithmetic progressions in $n$, and count the values left unmarked.

## Pseudocode

```text
Set LIMIT = 50,000,000.
Sieve all primes up to floor(sqrt(2 * LIMIT^2)).
Create a boolean array is_composite[0..LIMIT], initially false.

For each odd prime p from the sieve:
    If p is not congruent to 1 or 7 modulo 8:
        continue

    Solve 2 * r^2 ≡ 1 (mod p) using Tonelli-Shanks.
    Let the two roots be r and p - r.

    Check whether 2 * n^2 - 1 = p has an integer solution n.
    If it does, remember that exceptional n so it is not marked.

    For each root root in {r, p - r}:
        Mark every n ≡ root (mod p) with 2 <= n <= LIMIT
        as composite, except the exceptional n if it exists.

Count the integers n from 2 to LIMIT with is_composite[n] = false.
Return that count.
```

## Complexity Analysis

- **Time:** The prime sieve costs $O(M \log \log M)$ with $M = \lfloor \sqrt{2}N \rfloor$. The marking phase costs
  $$
  O\!\left(\sum_{p \equiv \pm 1 \bmod 8} \frac{N}{p}\right)
  = O(N \log \log N).
  $$
- **Space:** $O(M)$ for the prime sieve and $O(N)$ for the composite-marker array.

## Answer

$$\boxed{5437849}$$
