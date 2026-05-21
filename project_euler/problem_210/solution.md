# Problem 210: Obtuse Angled Triangles

## Problem Statement

Consider the set $S(r)$ of points $(x, y)$ with integer coordinates satisfying $|x| + |y| \leq r$. Let $O = (0, 0)$ and $C = (r/4, r/4)$. Let $N(r)$ be the number of points $B$ in $S(r)$ such that triangle $OBC$ has an obtuse angle, so its largest angle satisfies $90^\circ < \alpha < 180^\circ$.

Given that $N(4) = 24$ and $N(8) = 100$, find $N(10^9)$.

## Mathematical Development

### Classifying Obtuse Angles

With $O = (0, 0)$ and $C = (r/4, r/4)$, we determine which vertex is obtuse by dot products.

**Obtuse at $O$:**
$$
\vec{OB} \cdot \vec{OC} < 0
\iff x \cdot \frac{r}{4} + y \cdot \frac{r}{4} < 0
\iff x + y < 0.
$$

**Obtuse at $C$:**
$$
\vec{CO} \cdot \vec{CB} < 0
\iff \left(-\frac{r}{4}\right)\left(x - \frac{r}{4}\right) + \left(-\frac{r}{4}\right)\left(y - \frac{r}{4}\right) < 0
\iff x + y > \frac{r}{2}.
$$

**Obtuse at $B$:**
$$
\vec{BO} \cdot \vec{BC} < 0
\iff (-x)\left(\frac{r}{4} - x\right) + (-y)\left(\frac{r}{4} - y\right) < 0
\iff x^2 + y^2 < \frac{r}{4}(x + y).
$$

Completing the square gives
$$
\left(x - \frac{r}{8}\right)^2 + \left(y - \frac{r}{8}\right)^2 < \frac{r^2}{32},
$$
so the $B$-obtuse region is an open disk centered at $(r/8, r/8)$.

### These Regions Are Disjoint

- Region $O$ requires $x + y < 0$.
- Region $C$ requires $x + y > r/2$.
- Region $B$ implies $0 < x + y < r/2$.

Hence the three obtuse cases are pairwise disjoint.

### Degenerate Cases

The points $O$, $B$, and $C$ are collinear exactly when $B$ lies on the line $y = x$. These points must be excluded because they do not form a genuine triangle.

### Region O Count

Inside the diamond $|x| + |y| \leq r$, the condition $x + y < 0$ cuts off exactly half of the non-boundary points, and removing the collinear points on $y = x$ leaves
$$
|\text{Region } O| = r^2.
$$

### Region C Count

A parallel counting argument on the strips $x + y = s$ for $s > r/2$ gives
$$
|\text{Region } C| = \frac{r^2}{2}.
$$

### Region B Count

Write
$$
u = x - \frac{r}{8}, \qquad v = y - \frac{r}{8}.
$$
Because $r = 10^9$ is divisible by 8, this is an integer translation, and the disk becomes
$$
u^2 + v^2 < \frac{r^2}{32} = 2\left(\frac{r}{8}\right)^2.
$$

Thus Region $B$ contributes the number of lattice points in that open disk, minus the collinear points where $u = v$. The collinear condition gives
$$
2u^2 < \frac{r^2}{32}
\iff |u| < \frac{r}{8},
$$
so the number of excluded points is
$$
\frac{r}{4} - 1.
$$

### Final Formula

If `circle_count` denotes the number of lattice points satisfying
$$
u^2 + v^2 < \frac{r^2}{32},
$$
then
$$
N(r) = r^2 + \frac{r^2}{2} + \text{circle\_count} - \left(\frac{r}{4} - 1\right)
= \frac{3r^2}{2} - \frac{r}{4} + 1 + \text{circle\_count}.
$$

## Editorial

The clean split is by the vertex at which the obtuse angle occurs. Dot products turn the conditions at $O$ and $C$ into simple linear inequalities inside the diamond $|x| + |y| \leq r$, and the condition at $B$ turns into an open disk after completing the square.

Because those three regions are disjoint, the total count is the sum of their contributions. The first two are closed-form counts, while the third becomes a lattice-point count in a translated circle. After subtracting the collinear points on $y = x$, the remaining computation is a single integer sweep over the horizontal offsets of that disk.

## Pseudocode

```text
Set r = 10^9.
region_OC = r^2 + r^2 / 2

s = r / 8
R2 = 2 * s^2
circle_count = 0

For each integer u from -floor(sqrt(R2 - 1)) to floor(sqrt(R2 - 1)):
    rem = R2 - u^2
    max_v = largest integer v with v^2 < rem
    Add 2 * max_v + 1 to circle_count

collinear = r / 4 - 1
region_B = circle_count - collinear

Return region_OC + region_B
```

## Complexity Analysis

- **Time:** $O(r/8)$ for the disk lattice-point sweep.
- **Space:** $O(1)$.

## Answer

$$\boxed{1598174770174689458}$$
