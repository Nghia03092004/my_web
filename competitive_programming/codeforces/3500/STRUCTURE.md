# Codeforces 3500+ Archive Structure

This directory is the source-of-truth tree for the website's **Codeforces 3500+ Problems** section.

The public site builds these files into:

- `/competitive-programming/codeforces/3500/`
- `/competitive-programming/codeforces/3500/<problem-slug>/`
- `/competitive-programming/codeforces/3500+/` as an alias to the main landing page

The website reads this directory directly at build time. The rendered pages are presentation around these files, not a second manual source.

## Layout

```text
competitive_programming/
  codeforces/
    3500/
      STRUCTURE.md
      metadata.json
      problems/
        chiori-and-doll-picking-hard-version/
          editorial.tex
          solution.cpp
          meta.json
        replace/
          editorial.tex
          solution.cpp
          meta.json
```

## Source of truth

Every published Codeforces problem page is backed by exactly three editable files:

- `editorial.tex`
- `solution.cpp`
- `meta.json`

Use them like this:

- edit `editorial.tex` to update the reasoning, pseudocode, complexity section, or pitfalls
- edit `solution.cpp` to update the implementation
- edit `meta.json` to update the title, contest identifier, slug, official link, rating, tags, or short summary

Do not hand-maintain HTML for problem pages. The website renders these files directly.

## `editorial.tex` section contract

The current renderer expects top-level `\section{...}` blocks. Use these names:

- `\section{Problem Summary}`
- `\section{Editorial}`
- `\section{Pseudocode}` (optional)
- `\section{Complexity Analysis}`
- `\section{Notes}` (optional)

Subsections inside `Editorial` are encouraged. They show up in the page table of contents and are the right place for sections such as Motivation, Key Observations, Derivation, Algorithm, and Why It Works.

## `meta.json` contract

Each solved problem should keep this shape:

```json
{
  "title": "Replace",
  "slug": "replace",
  "contestId": 1707,
  "index": "E",
  "rating": 3500,
  "officialUrl": "https://codeforces.com/problemset/problem/1707/E",
  "tags": ["interval dynamics", "binary lifting", "range queries"],
  "status": "solved",
  "order": 2,
  "summary": "Given an interval, repeatedly replace it by the minimum and maximum values inside it and ask for the first time it becomes [1, n]."
}
```

The root-level `metadata.json` defines archive-level copy for the landing page.

## Statement policy

This archive is **not** a mirror of Codeforces statements.

For each problem page:

- write a short original summary in your own words
- link to the official Codeforces page
- avoid copying full official statement text
- avoid copying sample sections when the license position is unclear

## Adding a new 3500+ problem

1. Confirm the problem really belongs in this section: rating at least `3500`, strong enough to justify a portfolio-grade write-up, and a solution that can be explained precisely.
2. Create a folder under `competitive_programming/codeforces/3500/problems/` whose name matches the desired slug.
3. Add:
   - `editorial.tex`
   - `solution.cpp`
   - `meta.json`
4. Set `"status": "solved"` only when the explanation and code are both real and publishable.
5. Run `npm run build`.

If a candidate problem is not yet reliable enough, leave it out. This section is intentionally small.

## Build and verification

To verify the website routes:

```bash
npm run build
```

To compile a single C++ solution locally when needed:

```bash
g++ -std=c++17 -O2 -pipe -Wall -Wextra competitive_programming/codeforces/3500/problems/<slug>/solution.cpp
```

The site build does not compile the Codeforces solutions, so keep the C++ files honest when editing them.
