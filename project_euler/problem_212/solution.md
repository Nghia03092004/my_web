# Problem 212: Combined Volume of Cuboids

## Problem Statement

An axis-aligned cuboid with parameters $\{(x_0, y_0, z_0), (\delta x, \delta y, \delta z)\}$ consists of all points $(X, Y, Z)$ with
$$
x_0 \leq X \leq x_0 + \delta x,
\qquad
y_0 \leq Y \leq y_0 + \delta y,
\qquad
z_0 \leq Z \leq z_0 + \delta z.
$$

A lagged Fibonacci-like sequence $S_1, S_2, \ldots, S_{300000}$ is defined by:

- For $1 \leq k \leq 55$:
  $$
  S_k = (100003 - 200003k + 300007k^3) \bmod 1{,}000{,}000
  $$
- For $k > 55$:
  $$
  S_k = (S_{k-24} + S_{k-55}) \bmod 1{,}000{,}000
  $$

There are $50{,}000$ cuboids $C_1, \ldots, C_{50000}$. For cuboid $C_n$:

$$
x_0 = S_{6n-5} \bmod 10000,
\quad
y_0 = S_{6n-4} \bmod 10000,
\quad
z_0 = S_{6n-3} \bmod 10000
$$
$$
\delta x = 1 + (S_{6n-2} \bmod 399),
\quad
\delta y = 1 + (S_{6n-1} \bmod 399),
\quad
\delta z = 1 + (S_{6n} \bmod 399)
$$

Find the combined volume of all $50{,}000$ cuboids.

## Mathematical Development

**Definition 1.** Let $\mathcal{C} = \{C_1, \ldots, C_n\}$ be a collection of axis-aligned cuboids in $\mathbb{R}^3$. The *combined volume* is
$$
\operatorname{vol}\!\bigl(\bigcup_{i=1}^{n} C_i\bigr),
$$
where $\operatorname{vol}$ denotes 3-dimensional Lebesgue measure.

**Theorem 1 (Cavalieri's principle for cuboid unions).** *Let $z_1 < z_2 < \cdots < z_M$ be the distinct $z$-coordinates among all faces $\{z_0^{(i)}, z_0^{(i)} + \delta z^{(i)}\}_{i=1}^{n}$. Then*
$$
\operatorname{vol}\!\left(\bigcup_{i=1}^{n} C_i\right) = \sum_{j=1}^{M-1} (z_{j+1} - z_j) \cdot A_j
$$
*where $A_j$ is the 2-dimensional measure of the union of the $xy$-projections of all cuboids active in the slab $[z_j, z_{j+1})$.*

*Proof.* By Cavalieri's principle,
$$
\operatorname{vol}(U) = \int_{-\infty}^{\infty} \operatorname{Area}(U \cap \{z = t\})\,dt.
$$
For $t \in (z_j, z_{j+1})$, no cuboid face lies strictly between $z_j$ and $z_{j+1}$, so the active set is constant there. Hence the cross-sectional area is constant on that slab, equal to $A_j$, and integrating gives the stated sum. $\square$

**Theorem 2 (2D area via $x$-sweep).** *For a fixed $z$-slab, let $\mathcal{R}$ be the set of active $xy$-rectangles. Let $x_1 < x_2 < \cdots < x_K$ be the distinct $x$-coordinates among all left and right edges of rectangles in $\mathcal{R}$. Then*
$$
A_j = \sum_{\ell=1}^{K-1} (x_{\ell+1} - x_\ell) \cdot L_\ell
$$
*where $L_\ell$ is the total length of the union of the $y$-intervals from rectangles in $\mathcal{R}$ that cover the strip $[x_\ell, x_{\ell+1})$.*

*Proof.* Apply Cavalieri's principle again in dimension 2. Between consecutive $x$-boundaries the active rectangle set is fixed, so the union length in the $y$-direction is constant on that strip. $\square$

**Lemma 1 (Interval union via greedy merge).** *Given intervals $[a_1, b_1], \ldots, [a_m, b_m]$ sorted by left endpoint, their union length can be computed in linear time by greedily merging overlaps.*

*Proof.* Because the intervals are sorted by left endpoint, each new interval either starts a new disjoint component or extends the current merged component. Summing the lengths of the resulting maximal merged components gives the union length. $\square$

**Lemma 2 (Integer layer reduction).** *In this problem all face coordinates are integers. Therefore the volume is also the sum over the unit slabs $[z, z+1)$ of the union area of the cuboids active at height $z$.*

*Proof.* Every cuboid is of the form
$$
[x_1, x_2] \times [y_1, y_2] \times [z_1, z_2]
$$
with integer endpoints, so its contribution is constant on each unit slab $[z, z+1)$ with $z_1 \leq z < z_2$. Summing those constant cross-sectional areas over all integer $z$ reproduces the volume. $\square$

## Editorial

The naive decomposition by distinct $z$-faces is mathematically correct but still too slow if every slab rescans all 50,000 cuboids and then recomputes the 2D union with nested brute force. The crucial simplification here is that every coordinate is an integer below 10,400, so the volume can be accumulated one unit $z$-layer at a time.

For each integer layer $[z, z+1)$ we maintain the active cuboids by start and end events. Their projections onto the $xy$-plane are just rectangles, so the remaining task is a standard rectangle-union problem: sweep in $x$, and keep a segment tree over the $y$-axis storing the total covered $y$-length. That gives the union area of the active rectangles for the current layer, which contributes directly to the total volume because the slab thickness is 1.

## Pseudocode

```text
Generate the lagged sequence and build all cuboids as half-open boxes
[x1, x2) x [y1, y2) x [z1, z2).

For each cuboid:
    append its index to starts[z1]
    append its index to ends[z2]

active = empty set
volume = 0

For z from 0 up to the largest top face minus 1:
    Remove from active every cuboid whose z2 equals z.
    Add to active every cuboid whose z1 equals z.

    If active is empty:
        continue

    Build x-events from the active cuboids:
        (x1, +1, y1, y2) and (x2, -1, y1, y2)
    Sort the x-events by x.

    Clear the segment tree over the y-axis.
    area = 0
    prev_x = first event position

    Sweep through the x-events in order:
        area += covered_y_length * (current_x - prev_x)
        apply every event at current_x to the segment tree
        prev_x = current_x

    volume += area

Return volume
```

## Complexity Analysis

Let $A_z$ be the number of cuboids active in the unit slab $[z, z+1)$. Then:

- **Time:** building the event buckets is $O(n)$. For each $z$-layer, the 2D sweep costs $O(A_z \log A_z + A_z \log 10400)$. The total is
  $$
  O\!\left(\sum_z A_z \log A_z\right),
  $$
  and here $\sum_z A_z = \sum_i \delta z_i \leq 50{,}000 \cdot 399$.
- **Space:** $O(n)$ for the cuboids, event buckets, active set, and sweep events, plus $O(10400)$ for the segment tree.

## Answer

$$\boxed{328968937309}$$
