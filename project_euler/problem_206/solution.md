# Problem 206: Concealed Square

## Problem Statement

Find the unique positive integer whose square has the form $1\_2\_3\_4\_5\_6\_7\_8\_9\_0$, where each underscore represents a single digit.

## Mathematical Development

**Theorem (Divisibility by 10).** *If $n^2$ has the form $1\_2\_3\_4\_5\_6\_7\_8\_9\_0$, then $10 \mid n$.*

*Proof.* The last digit of $n^2$ is 0, so $n$ must be divisible by both 2 and 5. Hence $10 \mid n$. $\square$

**Theorem (Residue Constraint mod 100).** *Write $n = 10m$. Then $n^2$ has the prescribed form only if $m \equiv 3 \pmod{10}$ or $m \equiv 7 \pmod{10}$, that is, $n \equiv 30 \pmod{100}$ or $n \equiv 70 \pmod{100}$.*

*Proof.* Since $n^2 = 100m^2$, the pattern forces the hundreds digit of $n^2$ to be 9, so the units digit of $m^2$ must be 9. A square ends in 9 exactly when its root ends in 3 or 7. $\square$

**Lemma (Search Bounds).** *The value of $n$ satisfies*
$$
1010101010 \leq n \leq 1389199190.
$$

*Proof.* The 19-digit square $n^2$ lies in the interval $[1020304050607080900,\; 1929394959697989990]$. Taking square roots gives
$$
\left\lceil\sqrt{1020304050607080900}\right\rceil = 1010101010, \qquad
\left\lfloor\sqrt{1929394959697989990}\right\rfloor = 1389199189.
$$
Since $n$ must be divisible by 10, the upper endpoint rounds to $1389199190$. $\square$

**Theorem (Uniqueness).** *There exists exactly one $n$ in the search range satisfying the pattern constraint.*

*Proof.* The search is exhaustive over the candidates with final digits 30 or 70 inside the square-root interval. Computational verification finds exactly one solution:
$$
n = 1389019170,
\qquad
n^2 = 1929374254627488900,
$$
which matches the pattern $1\_2\_3\_4\_5\_6\_7\_8\_9\_0$. $\square$

## Editorial

The digit pattern forces strong modular restrictions before any search begins. Because the square ends in 0, the number itself must be divisible by 10. After dividing by 10, the next fixed digit forces the remaining factor to end in 3 or 7, so the full number must end in 30 or 70.

That leaves a narrow arithmetic progression inside the square-root bounds of the pattern interval. We scan those candidates and test the square digit by digit against `1_2_3_4_5_6_7_8_9_0`. The search is still exhaustive, but the modular pruning makes it small enough to finish quickly.

## Pseudocode

```text
lower = ceil(sqrt(1020304050607080900))
upper = floor(sqrt(1929394959697989990))

Move lower upward to the first number in [lower, upper]
whose last two digits are 30 or 70.

candidate = lower
While candidate <= upper:
    square = candidate * candidate
    If the digits of square in positions
       1, 3, 5, ..., 19 are 1, 2, 3, ..., 9, 0 respectively:
        return candidate

    If candidate ends in 30:
        candidate += 40
    Otherwise:
        candidate += 60
```

## Complexity Analysis

- **Time:** $O((n_{\max} - n_{\min}) / 50)$ candidate checks, each with constant-time digit testing.
- **Space:** $O(1)$.

## Answer

$$\boxed{1389019170}$$
