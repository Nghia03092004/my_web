# Problem 219: Skew-cost Coding

## Problem Statement

Build a prefix-free binary code for
$$
N = 10^9
$$
symbols. A `0` bit costs 1 and a `1` bit costs 4. The **cost** of a codeword is the sum of the costs of its bits, and the **total cost** is the sum over all codewords in the code.

Find the minimum total cost.

## Mathematical Development

**Definition.** A prefix-free code corresponds to a binary tree whose leaves are the codewords. The cost of a leaf is the weighted path length from the root, where moving left adds 1 and moving right adds 4.

**Theorem 1 (Greedy optimality).** *An optimal code is obtained by repeatedly splitting a leaf of minimum current cost.*

*Proof.* Suppose some optimal tree splits a leaf of cost $c_2$ while leaving another leaf of smaller cost $c_1 < c_2$ unsplit. Swapping those choices decreases the total cost by
$$
\bigl[(c_1+1)+(c_1+4)-c_1\bigr]

- \bigl[(c_2+1)+(c_2+4)-c_2\bigr]
= c_1 - c_2 < 0,
$$
contradicting optimality. Therefore the next split must always come from a minimum-cost leaf. $\square$

**Lemma 1 (Cost accounting).** *Splitting a leaf of cost $c$ removes that leaf and creates two leaves of costs $c+1$ and $c+4$. The net increase in total cost is*
$$
(c+1) + (c+4) - c = c + 5.
$$
*The number of leaves increases by 1.*

**Theorem 2 (Bulk processing of equal-cost leaves).** *If the current minimum cost is $c$ and there are $k$ leaves of that cost, then processing them all together is equivalent to $k$ consecutive greedy splits. The histogram update is:*

- *remove $k$ leaves of cost $c$*
- *add $k$ leaves of cost $c+1$*
- *add $k$ leaves of cost $c+4$*

*Proof.* All $k$ leaves are tied for minimum cost, so the greedy rule may split them in any order. Each split has the same local effect, and the $k$ effects add linearly. $\square$

## Editorial

The tree interpretation makes the greedy step completely local. Splitting a leaf of cost $c$ replaces it by two children of costs $c+1$ and $c+4$, so the total cost rises by $c+5$. The exchange argument shows that delaying a cheaper leaf in favor of a more expensive one can only make the final total worse.

That means we never need the full tree structure. A histogram of how many leaves exist at each cost is enough. Repeatedly take the smallest cost bucket, split as many leaves from that bucket as are still needed, update the counts of the $c+1$ and $c+4$ buckets, and continue until the tree has exactly $10^9$ leaves.

## Pseudocode

```text
Create a sorted map frequency with one entry:
    cost 0 appears once.

leaf_count = 1
total_cost = 0

While leaf_count < N:
    Let c be the smallest cost present in the map.
    Let available be the number of leaves of cost c.

    need = N - leaf_count
    splits = min(available, need)

    total_cost += splits * (c + 5)
    leaf_count += splits

    Add splits leaves to cost c + 1.
    Add splits leaves to cost c + 4.

    If not all leaves of cost c were split:
        put the unused remainder back into cost c.

Return total_cost
```

## Complexity Analysis

- **Time:** $O(M \log M)$ where $M$ is the number of distinct cost buckets that ever become active in the histogram.
- **Space:** $O(M)$ for the histogram.

## Answer

$$\boxed{64564225042}$$
