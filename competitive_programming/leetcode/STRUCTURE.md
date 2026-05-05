# LeetCode Archive Structure

This directory is the source-of-truth tree for the website's **LeetCode Solutions** section.

The public site builds these files into:

- `/competitive-programming/leetcode/`
- `/competitive-programming/leetcode/<category>/`
- `/competitive-programming/leetcode/<category>/<problem-slug>/`

The site reads this directory directly at build time. Keep the archive source-first: edit the files here, then let the website render them.

## Layout

```text
competitive_programming/
  leetcode/
    STRUCTURE.md
    metadata.json
    arrays/
      two-sum/
        editorial.tex
        solution.cpp
        meta.json
    two-pointers/
    sliding-window/
    binary-search/
    stack/
    heap/
    graph/
    tree/
    dynamic-programming/
    backtracking/
    greedy/
    bitmask/
    advanced/
```

Each published problem page is backed by exactly three editable files:

- `editorial.tex`
- `solution.cpp`
- `meta.json`

## `editorial.tex` contract

LeetCode pages are pattern-first, so keep these top-level sections in this order:

- `\section{Problem Summary}`
- `\section{Recognition Pattern}`
- `\section{Key Idea}`
- `\section{Step-by-step Reasoning}`
- `\section{Complexity}`
- `\section{Common Pitfalls}`
- `\section{Variants / Follow-ups}`

Use short paragraphs and bullet lists. The goal is interview clarity, not a full formal proof.

## `solution.cpp` contract

Use clean modern C++ that matches the LeetCode function signature.

Guidelines:

- prefer readable names over short contest names
- avoid macros and template-heavy tricks
- keep helper functions small and local to the pattern
- write code that would still read well during an interview walkthrough

## `meta.json` contract

Each solved problem should keep this shape:

```json
{
  "title": "Two Sum",
  "category": "Arrays",
  "slug": "two-sum",
  "officialUrl": "https://leetcode.com/problems/two-sum/",
  "difficulty": "Easy",
  "tags": ["array", "hash map", "complement lookup"],
  "status": "solved",
  "order": 1,
  "summary": "Return the two indices whose values add up to the target."
}
```

The root-level `metadata.json` defines the archive-level copy, learning path, and supported category list.

## Statement and editorial policy

This archive is **not** a mirror of LeetCode statements or discussions.

For each problem page:

- write an original summary in your own words
- link to the official LeetCode page
- explain the recognition pattern explicitly
- avoid copying statements, discussions, editorials, or sample walkthroughs

## Adding a new problem

1. Choose the right category under `competitive_programming/leetcode/`.
2. Create a problem folder whose name matches the slug.
3. Add:
   - `editorial.tex`
   - `solution.cpp`
   - `meta.json`
4. Set `"status": "solved"` only when the explanation and code are both publishable.
5. Run `npm run build`.

If a problem is only planned, keep it out of the public archive until it has a real explanation and real code.

## Category organization

The categories are intentionally pattern-oriented:

- `arrays`, `two-pointers`, `sliding-window`, `binary-search`, `stack`, `heap`
- `graph`, `tree`, `dynamic-programming`
- `backtracking`, `greedy`, `bitmask`, `advanced`

Choose the category by the main recognition pattern, not by the data type in the input.

## Build and verification

To verify the routes and rendered pages:

```bash
npm run build
```

The site build does not compile LeetCode C++ files, so keep the implementations honest when editing them.
