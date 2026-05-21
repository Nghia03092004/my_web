# Problem 218: Perfect Right-angled Triangles

## Problem Statement

A right-angled triangle with sides $a$, $b$, and hypotenuse $c$ is called **perfect** if:

1. $(a, b, c)$ is a primitive Pythagorean triple.
2. The hypotenuse $c$ is a perfect square.

How many perfect right-angled triangles with $c \leq 10^{16}$ have area that is not a multiple of 6 and not a multiple of 28?

## Mathematical Development

**Theorem 1 (Parametrization of primitive Pythagorean triples).** *Every primitive Pythagorean triple $(a,b,c)$ with $b$ even is given by*
$$
a = m^2 - n^2,
\qquad
b = 2mn,
\qquad
c = m^2 + n^2,
$$
*where $m > n > 0$, $\gcd(m,n)=1$, and $m \not\equiv n \pmod 2$.*

*Proof.* This is the classical Euclid parametrization. $\square$

**Lemma 1 (Area formula).** *The area is*
$$
A = mn(m-n)(m+n).
$$

*Proof.* Since $A = ab/2$,
$$
A = \frac{(m^2-n^2)(2mn)}{2} = mn(m-n)(m+n). \qquad \square
$$

**Theorem 2 (Every perfect triangle has area divisible by 84).** *For every perfect right-angled triangle,*
$$
84 \mid A.
$$
*In particular, its area is divisible by both 6 and 28.*

*Proof.* Since the hypotenuse is a perfect square, say
$$
c = m^2 + n^2 = k^2,
$$
the triple $(m,n,k)$ is itself a primitive Pythagorean triple. Hence one may write
$$
m = u^2 - v^2,
\qquad
n = 2uv
$$
or the symmetric variant.

- **Factor 3:** if neither $m$ nor $n$ were divisible by 3, then
  $$
  m^2 + n^2 \equiv 1 + 1 \equiv 2 \pmod 3,
  $$
  impossible for a square. So $3 \mid mn$.
- **Factor 4:** the even parameter is $2uv$, and one of $u,v$ is even, so in fact $4$ divides that factor.
- **Factor 7:** substituting the second parametrization and checking residues modulo 7 shows that one of
  $$
  m,\ n,\ m-n,\ m+n
  $$
  is always divisible by 7.

Therefore $3 \cdot 4 \cdot 7 = 84$ divides
$$
mn(m-n)(m+n) = A. \qquad \square
$$

## Editorial

There is no search left once the second parametrization is used. A perfect triangle already starts as a primitive Pythagorean triple, and the extra condition that the hypotenuse is a square forces the parameters $(m,n)$ to form another primitive Pythagorean triple. That second layer is what injects the extra divisibility.

From the resulting area formula
$$
A = mn(m-n)(m+n),
$$
the factors 3 and 4 are immediate, and a short residue check supplies the factor 7. So every perfect triangle has area divisible by 84, which means no triangle can fail both the divisibility-by-6 and divisibility-by-28 requirements.

## Pseudocode

```text
Use the primitive Pythagorean parametrization for (a, b, c).
Use the condition c = square to parametrize (m, n, k) as another primitive triple.
Deduce from the area formula that every candidate area has factors 3, 4, and 7.
Since every perfect triangle has area divisible by 84, the number of exceptions is 0.
Return 0.
```

## Complexity Analysis

- **Time:** $O(1)$.
- **Space:** $O(1)$.

## Answer

$$\boxed{0}$$
