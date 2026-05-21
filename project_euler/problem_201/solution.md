# Problem 201: Subsets with a Unique Sum

## Problem Statement

For any set $A$ of numbers, let $\operatorname{sum}(A)$ denote the sum of the elements of $A$. Consider the set $S = \{1^2, 2^2, 3^2, \ldots, 100^2\}$. Let $U$ be the set of all integers $v$ such that exactly one 50-element subset of $S$ has sum equal to $v$. Find $\operatorname{sum}(U)$.

## Mathematical Development

**Definition 1.** Let $S = \{a_1, a_2, \ldots, a_n\}$ with $a_i = i^2$ and $n = 100$. For integers $0 \leq j \leq n$ and $s \geq 0$, define the *subset count function*
$$
c(j, s) = \bigl|\bigl\{T \subseteq S : |T| = j \text{ and } \operatorname{sum}(T) = s\bigr\}\bigr|.
$$

The problem asks for $\sum_{v : c(50, v) = 1} v$.

**Theorem 1 (Sum Bounds).** *The sum of any 50-element subset of $S$ lies in the interval $[S_{\min}, S_{\max}]$ where*
$$
S_{\min} = \sum_{k=1}^{50} k^2 = 42925, \qquad S_{\max} = \sum_{k=51}^{100} k^2 = 295425.
$$

*Proof.* The minimum is attained uniquely by $\{1^2, 2^2, \ldots, 50^2\}$ and the maximum uniquely by $\{51^2, 52^2, \ldots, 100^2\}$. Applying the identity $\sum_{k=1}^{m} k^2 = m(m+1)(2m+1)/6$ yields $S_{\min} = 50 \cdot 51 \cdot 101 / 6 = 42925$. The total $\sum_{k=1}^{100} k^2 = 100 \cdot 101 \cdot 201 / 6 = 338350$, so $S_{\max} = 338350 - 42925 = 295425$. $\square$

**Lemma 1 (Recurrence).** *The subset count function satisfies the recurrence*
$$
c_i(j, s) = c_{i-1}(j, s) + c_{i-1}(j-1, s - a_i)
$$
*for $i = 1, \ldots, n$, with the convention $c_0(0, 0) = 1$ and $c_0(j, s) = 0$ otherwise. Here $c_i(j, s)$ denotes the count restricted to subsets of $\{a_1, \ldots, a_i\}$.*

*Proof.* The $j$-element subsets of $\{a_1, \ldots, a_i\}$ summing to $s$ are partitioned into those not containing $a_i$ (counted by $c_{i-1}(j, s)$) and those containing $a_i$ (counted by $c_{i-1}(j-1, s - a_i)$). $\square$

**Theorem 2 (Three-State DP Sufficiency).** *Define the clamped count $\hat{c}_i(j, s) = \min(c_i(j, s), 2)$. Then $\hat{c}_i$ satisfies*
$$
\hat{c}_i(j, s) = \min\bigl(\hat{c}_{i-1}(j, s) + \hat{c}_{i-1}(j-1, s - a_i),\; 2\bigr)
$$
*and correctly determines whether $c_n(j, s)$ is $0$, $1$, or $\geq 2$.*

*Proof.* We show by induction on $i$ that $\hat{c}_i(j, s) = \min(c_i(j, s), 2)$ for all $j, s$.

**Base case** ($i = 0$): $\hat{c}_0(0, 0) = \min(1, 2) = 1 = c_0(0, 0)$, and all other entries are $0$.

**Inductive step**: Assume $\hat{c}_{i-1}(j, s) = \min(c_{i-1}(j, s), 2)$ for all $j, s$. Let $u = c_{i-1}(j, s)$ and $w = c_{i-1}(j-1, s-a_i)$, so $c_i(j,s) = u + w$. We must show $\min(\min(u,2) + \min(w,2), 2) = \min(u + w, 2)$.

- If $u + w \leq 1$: then $u, w \leq 1$, so $\min(u,2) = u$ and $\min(w,2) = w$, giving $\min(u + w, 2) = u + w$.
- If $u + w \geq 2$: then $\min(u,2) + \min(w,2) \geq \min(u + w, 4) \geq 2$, so the clamp yields $2$. $\square$

**Corollary.** *The set $U = \{v : \hat{c}_n(50, v) = 1\}$ is precisely the set of sums achieved by exactly one 50-element subset.*

## Editorial

The key observation is that we never need the exact number of subsets once that number reaches 2. For each subset size and each possible sum, it is enough to remember whether the sum is unreachable, achievable in exactly one way, or achievable in at least two ways. That turns the problem into a 0/1 knapsack-style dynamic program with a three-state counter instead of an unbounded integer counter.

We process the squares in increasing order and update the table in reverse order of subset size and sum so that each square is used at most once. Whenever a new transition contributes to a state, we clamp the count at 2. After all 100 squares have been processed, the sums with `dp[50][s] = 1` are exactly the values in $U$, and adding those sums gives the answer.

## Pseudocode

```text
Set squares to [1^2, 2^2, ..., 100^2].
Let K = 50.
Compute S_min = 1^2 + 2^2 + ... + 50^2.
Compute S_max = 51^2 + 52^2 + ... + 100^2.

Create a table dp[0..K][0..S_max], initialized to 0.
Interpret dp[j][s] as:
    0 if no j-element subset has sum s,
    1 if exactly one j-element subset has sum s,
    2 if at least two j-element subsets have sum s.
Set dp[0][0] = 1.

For each square a_i in increasing order:
    For subset size j from min(i, K) down to 1:
        Restrict the sum loop to the values that are feasible for j - 1 chosen
        squares among the first i - 1 terms.
        For each such previous sum t, scanned in descending order:
            If dp[j - 1][t] is nonzero:
                Update dp[j][t + a_i] to min(2, dp[j][t + a_i] + dp[j - 1][t]).

answer = 0
For s from S_min to S_max:
    If dp[K][s] = 1:
        answer += s

Return answer
```

## Complexity Analysis

- **Time:** In worst-case form the DP is $O(n \cdot K \cdot S_{\max})$, with $n = 100$, $K = 50$, and $S_{\max} = 295425$. The implementation prunes each inner loop to the sum interval that is actually feasible at that stage.
- **Space:** $O(K \cdot S_{\max})$ for the three-state table.

## Answer

$$\boxed{115039000}$$
