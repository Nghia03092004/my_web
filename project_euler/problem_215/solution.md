# Problem 215: Crack-free Walls

## Problem Statement

Consider building a wall that is 32 units wide and 10 units tall using bricks of width 2 and width 3, all of height 1. A wall is **crack-free** if the gaps between horizontally adjacent bricks never line up in consecutive layers.

How many crack-free walls of dimensions $32 \times 10$ can be built?

## Mathematical Development

**Definition.** A *row tiling* of width $w$ is an ordered sequence of bricks of width 2 or 3 whose widths sum to $w$. The *crack set* $C(r)$ of a row tiling $r$ is the set of interior partial sums, namely the joint positions between consecutive bricks, excluding 0 and $w$.

**Theorem 1 (Consecutive-layer characterization).** *A wall with rows $r_1, \ldots, r_h$ is crack-free if and only if*
$$
C(r_i) \cap C(r_{i+1}) = \emptyset
\qquad \text{for every } 1 \leq i < h.
$$

*Proof.* This is exactly the condition stated in the problem: no crack position may line up in two consecutive layers. A position $x$ lines up between rows $r_i$ and $r_{i+1}$ precisely when $x \in C(r_i) \cap C(r_{i+1})$. $\square$

**Lemma 1 (Row tiling count).** *The number of row tilings of width 32 with bricks of width 2 and 3 equals*
$$
\sum \binom{a+b}{a}
$$
*over all $(a, b) \in \mathbb{Z}_{\geq 0}^2$ satisfying $2a + 3b = 32$. The valid pairs are*
$$
(16,0),\ (13,2),\ (10,4),\ (7,6),\ (4,8),\ (1,10).
$$

*Proof.* A tiling with $a$ bricks of width 2 and $b$ bricks of width 3 is an ordering of $a+b$ objects with $a$ identical 2-bricks and $b$ identical 3-bricks. The number of such orderings is $\binom{a+b}{a}$. The width equation gives the listed pairs. $\square$

**Theorem 2 (Dynamic programming on the compatibility graph).** *Let $R$ be the set of all row tilings. Define $f(r, k)$ as the number of crack-free walls of height $k$ whose top row is $r$. Then*

- *Base case:* $f(r, 1) = 1$ for all $r \in R$.
- *Recurrence:*
  $$
  f(r, k) = \sum_{\substack{r' \in R \\ C(r) \cap C(r') = \emptyset}} f(r', k-1).
  $$
- *Answer:*
  $$
  \sum_{r \in R} f(r, 10).
  $$

*Proof.* A valid wall of height $k$ ending in row $r$ is obtained by taking a valid wall of height $k-1$ ending in some compatible row $r'$ and placing $r$ above it. Every valid wall arises uniquely in this way. $\square$

## Editorial

The geometry disappears once each row is represented by its crack positions. A row is compatible with another row exactly when their crack sets are disjoint, so the wall problem becomes a path-counting problem in a finite compatibility graph whose vertices are the possible row tilings.

That graph is small enough to build explicitly. After generating all width-32 rows, precompute which pairs are compatible. Then dynamic programming over the wall height counts how many ways there are to end with each possible top row. Summing those counts after 10 layers gives the total number of crack-free walls.

## Pseudocode

```text
Generate every row tiling of width 32.
For each row, store the set or bitmask of its crack positions.

Build the compatibility list:
    row i is adjacent to row j
    if their crack sets are disjoint.

Set dp[i] = 1 for every row i, representing walls of height 1.

Repeat 9 times:
    Create a new array next filled with 0.
    For each row i:
        For each compatible predecessor j of row i:
            next[i] += dp[j]
    Replace dp by next.

Return the sum of all entries of dp.
```

## Complexity Analysis

Let $m$ be the number of width-32 row tilings.

- **Time:** $O(m^2)$ to build compatibility, then $O(10m^2)$ for the height DP.
- **Space:** $O(m^2)$ for the compatibility graph and $O(m)$ for the DP arrays.

## Answer

$$\boxed{806844323190414}$$
