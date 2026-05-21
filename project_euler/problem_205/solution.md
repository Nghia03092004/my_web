# Problem 205: Dice Game

## Problem Statement

Peter has nine four-sided (pyramidal) dice, each with faces numbered 1 to 4. Colin has six six-sided (cubic) dice, each with faces numbered 1 to 6. Peter and Colin roll their dice and compare totals: the highest total wins. What is the probability that Pyramidal Pete beats Cubic Colin? Give the answer rounded to seven decimal places.

## Mathematical Development

**Theorem (Convolution of Discrete Uniform Distributions).** *Let $X_1, X_2, \ldots, X_n$ be independent random variables, each uniformly distributed on $\{1, 2, \ldots, d\}$. The probability mass function of $S_n = \sum_{i=1}^{n} X_i$ is the $n$-fold convolution of the individual distributions. Equivalently, the number of ways to realize total $s$ is the coefficient of $x^s$ in $(x + x^2 + \cdots + x^d)^n$.*

*Proof.* For independent discrete random variables, the probability mass function of the sum is obtained by convolution. The generating-function statement is the coefficient form of the same recurrence. $\square$

**Theorem (Winning Probability).** *Let $P$ be Peter's total and $C$ be Colin's total. Then*
$$
\Pr(P > C) = \frac{1}{4^9 \cdot 6^6} \sum_{p=9}^{36} f_P(p) \cdot F_C(p-1),
$$
*where $f_P(p)$ is the number of Peter outcomes with total $p$, and $F_C(t) = \sum_{c=6}^{t} f_C(c)$ is Colin's cumulative frequency table.*

*Proof.* Since $P$ and $C$ are independent,
$$
\Pr(P > C) = \sum_p \Pr(P = p)\Pr(C < p).
$$
Writing these probabilities in terms of outcome counts yields the stated formula. $\square$

**Lemma (Range Constraints).** *Peter's sum lies in $[9, 36]$ and Colin's sum lies in $[6, 36]$.*

*Proof.* The minimum of $n$ dice is $n$, and the maximum is $nd$. $\square$

## Editorial

The clean approach is to compute the exact sum distribution for each player. Repeated convolution gives the number of ways Peter can make each total from 9 to 36 and the number of ways Colin can make each total from 6 to 36.

After that, the probability calculation is just bookkeeping. For a fixed Peter total $p$, every Colin total below $p$ is a win, so a cumulative table for Colin turns the double sum into a single pass over Peter's distribution. Dividing the final win count by $4^9 6^6$ gives the exact probability, which is then rounded to seven decimals.

## Pseudocode

```text
BuildDistribution(number_of_dice, faces):
    Start with freq[0] = 1.
    Repeat once per die:
        Create an empty table next.
        For every current sum s with frequency count:
            For face from 1 to faces:
                Add count to next[s + face].
        Replace freq by next.
    Return freq

Let peter = BuildDistribution(9, 4).
Let colin = BuildDistribution(6, 6).

Build a cumulative table smaller_colin where smaller_colin[p]
is the number of Colin outcomes with total strictly less than p.

winning_count = 0
For each Peter total p:
    winning_count += peter[p] * smaller_colin[p]

Return winning_count / (4^9 * 6^6), rounded to seven decimals
```

## Complexity Analysis

- **Time:** $O(n_P d_P s_{\max} + n_C d_C s_{\max} + s_{\max})$, where $s_{\max} = 36$.
- **Space:** $O(s_{\max})$ for the two distribution tables and Colin's cumulative table.

## Answer

$$\boxed{0.5731441}$$
