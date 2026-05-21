# Problem 208: Robot Walks

## Problem Statement

A robot moves on a plane, taking steps that are each an arc of one fifth of a circle. At each step, the robot can turn either left or right. After 70 steps, how many distinct closed paths return the robot to its starting position with its original orientation?

## Mathematical Development

### Heading States

Each step changes the heading by $\pm 72^\circ$, so the heading can be represented by an element of $\mathbb{Z}/5\mathbb{Z}$:

- left turn: $d \mapsto d + 1 \pmod{5}$
- right turn: $d \mapsto d - 1 \pmod{5}$

The walk starts at heading $0$ and must end there as well.

### Equal Use of the Five Headings

Let $v_d$ be the number of steps taken while the robot is in heading $d$. Writing the arc displacements in complex form and summing them shows that the total displacement is a fixed nonzero scalar multiple of
$$
v_0 + v_1 \omega + v_2 \omega^2 + v_3 \omega^3 + v_4 \omega^4,
$$
where $\omega = e^{2\pi i / 5}$.

The only rational linear relation among $1, \omega, \omega^2, \omega^3, \omega^4$ is
$$
1 + \omega + \omega^2 + \omega^3 + \omega^4 = 0,
$$
so zero displacement forces
$$
v_0 = v_1 = v_2 = v_3 = v_4.
$$

Since the walk has 70 steps in total, each heading must therefore be used exactly
$$
70 / 5 = 14
$$
times.

### Dynamic Programming State

Once the geometric condition has been reduced to equal heading counts, the remaining task is purely combinatorial. We use memoized dynamic programming on states
$$
(d, c_0, c_1, c_2, c_3, c_4),
$$
where $d$ is the current heading and $c_i$ is the number of remaining times heading $i$ may still be used.

If all $c_i$ are zero, the walk is valid exactly when the current heading is back to $0$.

### Transitions

From a state with $c_d > 0$, we consume one use of heading $d$ and branch in the two possible directions:

- turn left and move to heading $(d+1) \bmod 5$
- turn right and move to heading $(d-1) \bmod 5$

Every closed walk corresponds to exactly one path through this state graph, and every path that exhausts the counts and returns to heading 0 is a valid closed walk.

## Editorial

The geometry looks awkward until it is compressed into a counting constraint. A closed walk must use the five heading classes equally often, so for 70 steps each heading is used exactly 14 times. After that reduction, the shape of the path no longer needs separate coordinate tracking.

What remains is a finite state search. At any moment we only need to know the current heading and how many uses of each heading are still available. From that state there are at most two legal continuations, corresponding to the next left or right turn, so memoization counts the closed walks without revisiting the same subproblem.

## Pseudocode

```text
Define Solve(heading, c0, c1, c2, c3, c4):
    remaining = c0 + c1 + c2 + c3 + c4
    If remaining = 0:
        return 1 if heading = 0, otherwise 0

    If the state was computed before:
        return the cached value

    Let counts be [c0, c1, c2, c3, c4].
    If counts[heading] = 0:
        return 0

    Decrease counts[heading] by 1.

    total =
        Solve((heading + 1) mod 5, updated counts) +
        Solve((heading - 1) mod 5, updated counts)

    Cache total for the state and return it.

Return Solve(0, 14, 14, 14, 14, 14)
```

## Complexity Analysis

- **Time:** $O(\text{number of reachable memoized states})$, with at most two transitions per state.
- **Space:** $O(\text{number of reachable memoized states})$ for the cache.

## Answer

$$\boxed{331951449665644800}$$
