# Problem 209: Circular Logic

## Problem Statement

A 6-input binary truth table $\tau(a,b,c,d,e,f)$ must satisfy
$$
\tau(a,b,c,d,e,f) \wedge \tau(b,c,d,e,f, a \oplus (b \wedge c)) = 0
$$
for all inputs $(a,b,c,d,e,f) \in \{0,1\}^6$.

How many such truth tables exist?

## Mathematical Development

### The Permutation

Define
$$
\sigma(a,b,c,d,e,f) = (b,c,d,e,f, a \oplus (b \wedge c)).
$$

This is a bijection on $\{0,1\}^6$, because from the image $(b,c,d,e,f,g)$ we can recover
$$
a = g \oplus (b \wedge c).
$$

### Constraint as a Cycle Problem

The condition
$$
\tau(x) \wedge \tau(\sigma(x)) = 0
$$
means that two consecutive states along an orbit of $\sigma$ cannot both be assigned the value 1. Since $\sigma$ is a permutation, the 64 states decompose into disjoint cycles, and the constraint can be solved independently on each cycle.

### Independent Sets on a Cycle

For a cycle of length $n$, the valid assignments are exactly the independent sets of the cycle graph $C_n$. Their number is the Lucas number
$$
L_n = F_{n-1} + F_{n+1},
$$
where $F_n$ is the Fibonacci sequence.

For the fixed-point case $n = 1$, the condition becomes $\tau(x) \wedge \tau(x) = 0$, so only the assignment $\tau(x) = 0$ is allowed; this matches the convention $L_1 = 1$.

### Cycle Structure

A direct computation of the permutation on all 64 bit patterns yields the cycle lengths
$$
1,\ 2,\ 3,\ 6,\ 6,\ 46.
$$

Therefore the total number of valid truth tables is
$$
L_1 \cdot L_2 \cdot L_3 \cdot L_6^2 \cdot L_{46}.
$$

## Editorial

The Boolean condition becomes much easier once the input tuples are viewed as nodes of a directed permutation graph. Every tuple points to exactly one successor under $\sigma$, and because $\sigma$ is invertible, those successor chains are actually cycles.

On a single cycle, the rule simply says that no two adjacent positions may both receive the value 1. That is the independent-set count for a cycle graph, which is a Lucas number. So the whole problem reduces to finding the cycle decomposition of a 64-state permutation and multiplying the appropriate Lucas counts.

## Pseudocode

```text
Define sigma(x) by decoding the 6 bits of x,
shifting them left, and appending a xor (b and c).

Mark all 64 states as unvisited.
cycle_lengths = empty list

For x from 0 to 63:
    If x is already visited:
        continue

    Follow sigma starting from x until the cycle closes,
    marking states visited and counting the length.
    Append that length to cycle_lengths.

Define Lucas(n):
    If n = 1, return 1
    If n = 2, return 3
    Build upward with the recurrence L_n = L_{n-1} + L_{n-2}

answer = 1
For each length in cycle_lengths:
    answer *= Lucas(length)

Return answer
```

## Complexity Analysis

- **Time:** $O(64)$ to find the cycle decomposition, plus $O(\max \text{ cycle length})$ to build the Lucas numbers.
- **Space:** $O(64)$.

## Answer

$$\boxed{15964587728784}$$
