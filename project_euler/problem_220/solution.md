# Problem 220: Heighway Dragon

## Problem Statement

The Heighway Dragon curve is defined by the Lindenmayer system:

- **Axiom:** $F_a$
- **Rules:** $F_a \to F_a R F_b$, $F_b \to F_a L F_b$

Here $F_a$ and $F_b$ mean "draw forward", $R$ means "turn right 90 degrees", and $L$ means "turn left 90 degrees".

Starting at the origin facing up, find the position after $10^{12}$ drawing steps of the order-50 dragon curve.

## Mathematical Development

### String Expansion

Let $F_a^{(n)}$ and $F_b^{(n)}$ denote the level-$n$ expansions of the two symbols. Their recursive structure is
$$
F_a^{(n)} = F_a^{(n-1)} \; R \; F_b^{(n-1)},
\qquad
F_b^{(n)} = F_a^{(n-1)} \; L \; F_b^{(n-1)}.
$$

Each level doubles the number of forward moves, so both expansions contain exactly
$$
2^n
$$
draw steps.

### Precomputed Displacements

For each level $n$, each symbol $s \in \{a,b\}$, and each starting direction $d \in \{0,1,2,3\}$ corresponding to north, east, south, west, define:

- $\Delta x(n,s,d)$ and $\Delta y(n,s,d)$: the net displacement after executing the whole expansion
- $\operatorname{rot}(n,s)$: the net change of direction modulo 4

At level 0, both symbols are just one forward step, so
$$
\operatorname{rot}(0,a) = \operatorname{rot}(0,b) = 0
$$
and the displacement is one unit in the current direction.

From the recursive rules,
$$
\operatorname{rot}(n,a) = \operatorname{rot}(n-1,a) + 1 + \operatorname{rot}(n-1,b) \pmod 4,
$$
$$
\operatorname{rot}(n,b) = \operatorname{rot}(n-1,a) - 1 + \operatorname{rot}(n-1,b) \pmod 4.
$$

The displacement recurrences follow by concatenating the displacement of the first half with the appropriately rotated displacement of the second half.

### Recursive Query

To find the position after only the first $k$ drawing steps of $F_s^{(n)}$, observe that the first half always consists of $F_a^{(n-1)}$ and contains exactly
$$
2^{n-1}
$$
steps.

- If $k \leq 2^{n-1}$, recurse into the first half.
- If $k > 2^{n-1}$, take the full displacement of the first half, apply the intervening turn ($R$ for $a$, $L$ for $b$), and recurse into the second half $F_b^{(n-1)}$ with the remaining step count.

Because each recursive call drops the level by 1, the query depth is at most 50.

## Editorial

The full dragon string is astronomically large, but we never need to build it. The recursive production rules already tell us how a level-$n$ block is assembled from two level-$(n-1)$ blocks and one turn, so the useful information is just the net effect of a block: where it ends and which way it is facing.

Once those net effects are precomputed, answering the $10^{12}$-step query becomes a binary decomposition problem. Each level splits into a first half of size $2^{n-1}$ and a second half of the same size, so we only need to decide which half contains the requested step count. The recursion walks down at most 50 levels and accumulates the displacement along the branches that are taken in full.

## Pseudocode

```text
For each direction, record the unit step vectors.

Initialize level-0 data:
    each symbol moves one step in the current direction
    and produces zero net rotation.

For n from 1 to 50:
    Compute rot(n, a) and rot(n, b) from the two recursive formulas.
    For each starting direction d:
        Build the displacement of F_a^(n) by:
            taking the full displacement of F_a^(n-1),
            turning right,
            then adding the displacement of F_b^(n-1) in the new direction.
        Build the displacement of F_b^(n) similarly, but turn left.

Define Query(n, symbol, direction, steps):
    If steps = 0:
        return (0, 0)
    If n = 0:
        return one unit step in the current direction

    half = 2^(n - 1)
    If steps <= half:
        return Query(n - 1, a, direction, steps)

    first = full displacement of F_a^(n-1) in this direction
    direction_after_first = direction rotated by rot(n - 1, a)
    direction_for_second = apply R or L depending on the symbol

    second = Query(n - 1, b, direction_for_second, steps - half)
    return first + second

Return Query(50, a, north, 10^12)
```

## Complexity Analysis

- **Time:** $O(50 \cdot 4)$ for the precomputation tables and $O(50)$ for the recursive query.
- **Space:** $O(50 \cdot 4)$ for the displacement tables.

## Answer

$$\boxed{139776,963904}$$
