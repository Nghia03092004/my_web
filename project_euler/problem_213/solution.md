# Problem 213: Flea Circus

## Problem Statement

A $30 \times 30$ grid starts with exactly one flea on each square. Each round, every flea independently jumps to a uniformly random adjacent square. Corner fleas have 2 choices, edge fleas have 3, and interior fleas have 4.

After 50 rounds, what is the expected number of empty squares?

## Mathematical Development

**Definition 1.** Let $G = (V, E)$ be the grid graph with
$$
V = \{(r, c) : 0 \leq r, c < 30\},
$$
and edges between horizontally or vertically adjacent squares. Let $N = |V| = 900$.

**Definition 2.** The *transition matrix* $T \in \mathbb{R}^{N \times N}$ of the random walk on $G$ is defined by
$$
T_{ij} =
\begin{cases}
1/\deg(i) & \text{if } j \sim i, \\
0 & \text{otherwise},
\end{cases}
$$
where $\deg(i) \in \{2,3,4\}$ is the degree of square $i$.

**Lemma 1 (Row stochasticity).** *The matrix $T$ is row-stochastic: $\sum_j T_{ij} = 1$ for all $i$.*

*Proof.* For a fixed square $i$, exactly $\deg(i)$ entries in row $i$ are nonzero, each equal to $1/\deg(i)$. Their sum is therefore 1. $\square$

**Theorem 1 (Chapman-Kolmogorov).** *The entry $(T^k)_{ij}$ equals the probability that a random walk starting at square $i$ is at square $j$ after exactly $k$ steps.*

*Proof.* The case $k = 1$ is the definition of $T$. The inductive step is the usual matrix-product form of the law of total probability. $\square$

**Theorem 2 (Expected empty squares).** *Let $\mathbf{1}_i$ be the indicator that square $i$ is empty after 50 rounds. Then*
$$
E\!\left[\sum_{i=1}^{N} \mathbf{1}_i\right]
= \sum_{i=1}^{N} \prod_{j=1}^{N} \bigl(1 - (T^{50})_{ji}\bigr).
$$

*Proof.* By linearity of expectation,
$$
E\!\left[\sum_i \mathbf{1}_i\right] = \sum_i P(\text{square } i \text{ is empty}).
$$
Square $i$ is empty exactly when none of the 900 fleas lands there. Since the fleas move independently,
$$
P(\text{square } i \text{ is empty})
= \prod_{j=1}^{N} P(\text{flea } j \text{ is not at } i)
= \prod_{j=1}^{N} \bigl(1 - (T^{50})_{ji}\bigr),
$$
using Theorem 1 for the individual landing probabilities. $\square$

**Remark.** When multiplying many factors of the form $1-p$, a logarithmic accumulation
$$
\sum_j \log(1-p_j)
$$
can be numerically more stable than repeated floating-point multiplication.

## Editorial

The empty-square expectation separates cleanly by square. For one fixed target square, each flea independently misses it with some probability, so the probability that the square ends up empty is a product over all starting squares. Summing those 900 emptiness probabilities gives the final expectation.

The real work is therefore to compute the 50-step distribution of a flea starting from each square. That can be done either by matrix powers or, more practically, by propagating one probability vector for 50 rounds from each starting square. After each source distribution is computed, multiply every board position by the probability that this particular flea is absent there. At the end, those products are exactly the emptiness probabilities.

## Pseudocode

```text
Label the 900 squares and precompute the legal neighbors of each one.
Initialize absent_probability[i] = 1 for every square i.

For each starting square s:
    Set a probability vector current with current[s] = 1.

    Repeat 50 times:
        Create a zero vector next.
        For each square u:
            Split current[u] equally among the neighbors of u
            and add those contributions into next.
        Replace current by next.

    For each target square i:
        absent_probability[i] *= 1 - current[i]

answer = sum of absent_probability[i] over all squares i
Return answer
```

## Complexity Analysis

- **Time:** Propagating one flea distribution for 50 rounds costs $O(50N)$ state updates, so handling all 900 starting squares costs $O(N^2 R)$ with $N = 900$ and $R = 50$.
- **Space:** $O(N)$ for the current and next probability vectors, plus $O(N)$ for the accumulated absence probabilities.

## Answer

$$\boxed{330.721154}$$
