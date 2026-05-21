# Problem 217: Balanced Numbers

## Problem Statement

A positive integer with $k$ digits is **balanced** if the sum of the first $\lfloor k/2 \rfloor$ digits equals the sum of the last $\lfloor k/2 \rfloor$ digits. For odd $k$, the middle digit is ignored.

All 1-digit numbers are trivially balanced.

Find the sum of all balanced numbers less than $10^{47}$, modulo
$$
3^{15} = 14348907.
$$

## Mathematical Development

**Definition.** For a $k$-digit number, let $h = \lfloor k/2 \rfloor$.

- If $k = 2h$, write
  $$
  N = L \cdot 10^h + R,
  $$
  where $L$ is an $h$-digit number and $0 \leq R < 10^h$.
- If $k = 2h+1$, write
  $$
  N = L \cdot 10^{h+1} + m \cdot 10^h + R,
  $$
  where $L$ is an $h$-digit number, $m \in \{0,\ldots,9\}$, and $0 \leq R < 10^h$.

Balanced means $\operatorname{digitsum}(L) = \operatorname{digitsum}(R)$.

**Theorem 1 (Decomposition of the balanced-number sum).** *For $h$-digit strings allowing leading zeros, define*

- *$C(h, s)$ = number of strings with digit sum $s$*
- *$\Sigma(h, s)$ = sum of the numeric values of those strings*

*For genuine left halves with no leading zero, define*
$$
C_L(h, s) = C(h, s) - C(h-1, s),
\qquad
\Sigma_L(h, s) = \Sigma(h, s) - \Sigma(h-1, s).
$$

*Then for even length $2h$,*
$$
\text{Sum}_{2h}
= \sum_{s=0}^{9h}
\Bigl[
\Sigma_L(h,s)\,10^h\,C(h,s)
+
C_L(h,s)\,\Sigma(h,s)
\Bigr].
$$

*For odd length $2h+1$,*
$$
\text{Sum}_{2h+1}
= \sum_{s=0}^{9h}
\Bigl[
10\,\Sigma_L(h,s)\,10^{h+1}\,C(h,s)
+
45\,10^h\,C_L(h,s)\,C(h,s)
+
10\,C_L(h,s)\,\Sigma(h,s)
\Bigr].
$$

*Proof.* For even length, each balanced number is formed by pairing a valid left half and a right half with the same digit sum. Summing the contribution of the left block and the right block separately gives the stated formula.

For odd length, there are 10 choices for the middle digit. The left and right contributions are therefore multiplied by 10, while the middle position contributes
$$
0 + 1 + \cdots + 9 = 45
$$
times the corresponding place value. $\square$

**Lemma 1 (DP for digit-sum statistics).** *The tables $C(h,s)$ and $\Sigma(h,s)$ satisfy*
$$
C(h, s) = \sum_{d=0}^{9} C(h-1, s-d),
$$
$$
\Sigma(h, s) = \sum_{d=0}^{9} \bigl[10\Sigma(h-1, s-d) + d\,C(h-1, s-d)\bigr],
$$
*with base values $C(0,0)=1$ and $\Sigma(0,0)=0$.*

*Proof.* Append a final digit $d$ to a shorter string of digit sum $s-d$. The count recurrence is immediate, and the value recurrence follows from the decimal-value update
$$
v \mapsto 10v + d.
$$
$\square$

## Editorial

The balance condition couples the two halves only through their digit sum. That means the useful state is not the half-string itself, but just two aggregated quantities for each pair `(length, digit sum)`: how many half-strings realize that sum, and what the total of their numeric values is.

Once those tables are built, assembling balanced numbers is straightforward. For each possible total length and each feasible digit sum, pair a left half with a right half of the same sum. Even and odd lengths differ only in how the middle digit is handled, so the whole problem reduces to one reusable DP table plus a clean final accumulation.

## Pseudocode

```text
Set MOD = 3^15 and MAX_HALF = 23.
Create tables count[h][s] and total_value[h][s].

Initialize count[0][0] = 1 and total_value[0][0] = 0.
Precompute powers of 10 modulo MOD.

For half-length h from 0 to MAX_HALF - 1:
    For each reachable digit sum s:
        For digit d from 0 to 9:
            count[h + 1][s + d] += count[h][s]
            total_value[h + 1][s + d] += total_value[h][s] + d * 10^h * count[h][s]
            Reduce everything modulo MOD.

answer = sum of the 1-digit numbers = 45.

For full length k from 2 to 47:
    h = floor(k / 2)
    For each digit sum s:
        left_count  = count[h][s] - count[h - 1][s]
        left_total  = total_value[h][s] - total_value[h - 1][s]
        right_count = count[h][s]
        right_total = total_value[h][s]

        If k is even:
            add the contribution of left halves shifted by 10^h
            plus the contribution of right halves.
        Otherwise:
            also include the 10 choices for the middle digit
            and their total contribution 45 * 10^h.

Return answer modulo MOD.
```

## Complexity Analysis

- **Time:** Building the half-string tables costs $O(H S \cdot 10)$ with $H = 23$ and $S = 9H = 207$. The final accumulation over all lengths costs $O(47S)$.
- **Space:** $O(HS)$ for the count and value tables.

## Answer

$$\boxed{6273134}$$
