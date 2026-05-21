# Problem 203: Squarefree Binomial Coefficients

## Problem Statement

Find the sum of all distinct squarefree numbers in the first 51 rows (rows 0 through 50) of Pascal's triangle.

## Mathematical Development

**Theorem (Kummer, 1852).** *For a prime $p$ and non-negative integers $m, n$, the $p$-adic valuation $v_p\binom{m+n}{m}$ equals the number of carries when adding $m$ and $n$ in base $p$.*

*Proof.* By Legendre's formula,
$$
v_p(k!) = \sum_{i=1}^{\infty} \left\lfloor \frac{k}{p^i} \right\rfloor.
$$
Therefore
$$
v_p\binom{m+n}{m}
= v_p((m+n)!) - v_p(m!) - v_p(n!)
= \sum_{i=1}^{\infty}\left(\left\lfloor\frac{m+n}{p^i}\right\rfloor - \left\lfloor\frac{m}{p^i}\right\rfloor - \left\lfloor\frac{n}{p^i}\right\rfloor\right).
$$
Each summand equals 0 or 1, and it equals 1 precisely when there is a carry out of position $i-1$ in the base-$p$ addition of $m$ and $n$. $\square$

**Lemma (Bounded Valuation for Large Primes).** *For $n \leq 50$ and any prime $p \geq 11$, every binomial coefficient $\binom{n}{k}$ satisfies $v_p\binom{n}{k} \leq 1$.*

*Proof.* Since $p \geq 11$ and $n \leq 50 < p^2 = 121$, both $k$ and $n-k$ have at most two digits in base $p$. Adding two such numbers produces at most one carry. By Kummer's theorem, $v_p\binom{n}{k} \leq 1$. $\square$

**Theorem (Squarefreeness Criterion).** *A binomial coefficient $\binom{n}{k}$ with $n \leq 50$ is squarefree if and only if $\binom{n}{k}$ is not divisible by any of $4$, $9$, $25$, or $49$.*

*Proof.* A positive integer is squarefree if and only if $v_p \leq 1$ for every prime $p$. By the lemma, this automatically holds for all primes $p \geq 11$. Thus squarefreeness can fail only for $p \in \{2,3,5,7\}$, and that is equivalent to divisibility by $p^2 \in \{4,9,25,49\}$. $\square$

## Editorial

The heavy number theory reduces the checking step to something very small. Kummer's theorem shows that in rows $0$ through $50$, any prime at least 11 can appear with exponent at most 1 in a binomial coefficient, so repeated prime factors can only come from $2$, $3$, $5$, or $7$.

That means we can generate every distinct binomial coefficient in the first 51 rows of Pascal's triangle, store them in a set, and test squarefreeness with only four divisibility checks: by $4$, $9$, $25$, and $49$. The values that survive are exactly the squarefree ones, and summing them finishes the problem.

## Pseudocode

```text
Set N = 50.
Create an empty set values.
Start with row = [1].
Insert 1 into values.

For n from 1 to N:
    Build the next Pascal row from the previous one:
        begin with 1,
        fill each interior entry by adding the two entries above it,
        end with 1.
    Insert every entry of the new row into values.

answer = 0
For each v in values:
    If v is not divisible by 4, 9, 25, or 49:
        answer += v

Return answer
```

## Complexity Analysis

- **Time:** $O(N^2)$ to generate the first 51 rows of Pascal's triangle, plus constant work per distinct value for the four square divisibility checks.
- **Space:** $O(|S|)$ for the set of distinct binomial coefficients.

## Answer

$$\boxed{34029210557338}$$
