# Problem 202: Laserbeam

## Problem Statement

Three mirrors are arranged in the shape of an equilateral triangle, with their reflective surfaces pointing inwards. There is an infinitesimal gap at each vertex through which a laser beam may pass. A laser beam enters at vertex $C$, bounces off exactly $R = 12017639147$ reflecting surfaces, and then exits through vertex $C$. In how many ways can this be done?

## Mathematical Development

**Theorem (Unfolding Principle).** *A laser beam making $R$ reflections inside an equilateral triangle corresponds, via the standard unfolding technique, to a straight line in the triangular tessellation of the plane. The line starts at the origin and terminates at a lattice point $(a, b)$ in the oblique coordinate system, where $a + b = n$ with $n = (R + 3)/2$.*

*Proof.* Each reflection off a mirror wall is equivalent to reflecting the triangle across that wall and continuing the beam in a straight line. After $R$ reflections, the beam has crossed $R$ triangle edges. The beam enters at vertex $C$ and must exit at a vertex, crossing $R$ edges total. In the oblique coordinate system where the three families of parallel lines in the tessellation are indexed by coordinates $(a, b)$ with $a + b = n$, careful counting of crossings gives $n = (R + 3)/2$. For $R = 12017639147$, we get $n = 6008819575$. $\square$

**Theorem (Exit Vertex Classification).** *The beam exits through vertex $C$ if and only if $b \not\equiv 0 \pmod{3}$ and $a \not\equiv 0 \pmod{3}$, which (given $a + b = n$ with $n \equiv 1 \pmod{3}$) is equivalent to $b \equiv 2 \pmod{3}$.*

*Proof.* In the triangular tessellation, a lattice point $(a, b)$ corresponds to a copy of vertex $A$, $B$, or $C$ depending on the residues of $a$ and $b$ modulo 3. Since $n = a + b \equiv 1 \pmod{3}$ and we require the endpoint to be a copy of $C$, the condition reduces to $b \equiv 2 \pmod{3}$. $\square$

**Theorem (No Intermediate Vertex Condition).** *The beam passes through no intermediate vertex, and thus does not exit prematurely, if and only if $\gcd(a, b) = 1$, equivalently $\gcd(b, n) = 1$.*

*Proof.* If $d = \gcd(a, b) > 1$, then $(a/d, b/d)$ is an intermediate lattice point on the line segment from $(0,0)$ to $(a,b)$, so the beam would reach a vertex earlier. Conversely, if $\gcd(a,b) = 1$, no intermediate lattice point lies on the segment. Since $a = n - b$, we have $\gcd(a,b) = \gcd(n-b, b) = \gcd(n, b)$. $\square$

**Theorem (Counting via Mobius Inversion).** *The number of valid beam paths is*
$$
\text{Count} = \sum_{d \mid n} \mu(d) \cdot C(d),
$$
*where $C(d) = \#\{b \in [1, n-1] : d \mid b,\; b \equiv 2 \pmod{3}\}$ and $\mu$ is the Mobius function.*

*Proof.* We seek
$$
\#\{b \in [1, n-1] : \gcd(b, n) = 1,\; b \equiv 2 \pmod{3}\}.
$$
By Mobius inversion, $\sum_{d \mid \gcd(b,n)} \mu(d) = [\gcd(b,n) = 1]$. Exchanging the order of summation gives
$$
\text{Count} = \sum_{d \mid n} \mu(d) \sum_{\substack{b=1 \\ d \mid b,\; b \equiv 2 \pmod{3}}}^{n-1} 1 = \sum_{d \mid n} \mu(d) \cdot C(d).
$$
For each squarefree divisor $d$ of $n$ with $\gcd(d, 3) = 1$, writing $b = dk$ gives $k \in [1, n/d - 1]$ with $dk \equiv 2 \pmod{3}$, so the inner count is a simple arithmetic progression count. $\square$

**Lemma (Factorization).** $n = 6008819575 = 5^2 \times 11 \times 17 \times 23 \times 29 \times 41 \times 47.$ Since $3 \nmid n$, all $2^7 = 128$ squarefree divisors built from the distinct prime factors may contribute.

*Proof.* Verified by trial division. Since $5^2 \mid n$, the Mobius function vanishes on divisors containing $5^2$, so it is enough to enumerate squarefree divisors built from $\{5, 11, 17, 23, 29, 41, 47\}$. $\square$

## Editorial

The geometric part is handled by unfolding the reflections. Instead of following a bouncing beam inside one triangle, we follow a straight segment in the triangular tiling. For the given reflection count, every admissible beam corresponds to an endpoint on the line $a + b = n$, where $n = (R + 3)/2 = 6008819575$.

Two arithmetic conditions remain. The endpoint must be a copy of vertex $C$, which becomes the congruence $b \equiv 2 \pmod{3}$, and the segment must not hit an earlier lattice vertex, which becomes $\gcd(b, n) = 1$. That leaves a clean number-theoretic count: integers in one residue class modulo 3 that are also coprime to $n$. Mobius inversion removes the coprimality condition by inclusion-exclusion over the squarefree divisors of $n$.

## Pseudocode

```text
Set R = 12017639147 and n = (R + 3) / 2.
Factor n and keep its distinct prime divisors p_1, ..., p_k.

answer = 0
For each bitmask from 0 to 2^k - 1:
    Let d be the product of the primes selected by the bitmask.
    Let mu be +1 if the number of selected primes is even, else -1.

    We need multiples b = d * t with 1 <= t <= n / d - 1
    and d * t congruent to 2 modulo 3.

    Solve the congruence for t modulo 3.
    Count how many t in the interval satisfy that residue class.
    Add mu times that count to answer.

Return answer
```

## Complexity Analysis

- **Time:** $O(\sqrt{n})$ for factorization, plus $O(2^k)$ for the Mobius sum where $k = 7$ is the number of distinct prime factors.
- **Space:** $O(k)$ to store the distinct prime factors.

## Answer

$$\boxed{1209002624}$$
