# CSES Archive Structure

This directory is the source-of-truth tree for the website's **CSES Problem Set** section.

The public site builds these files into:

- `/competitive-programming/cses/`
- `/competitive-programming/cses/<category>/`
- `/competitive-programming/cses/<category>/<problem-slug>/`

The site reads this directory directly at build time. The archive pages are presentation around these files, not a second manual source.

## Layout

```text
competitive_programming/
  cses/
    STRUCTURE.md
    metadata.json
    introductory-problems/
      weird-algorithm/
        editorial.tex
        solution.cpp
        meta.json
      missing-number/
        editorial.tex
        solution.cpp
        meta.json
    sorting-and-searching/
    dynamic-programming/
    graph-algorithms/
    range-queries/
    tree-algorithms/
    mathematics/
    string-algorithms/
    geometry/
    advanced-techniques/
    additional-problems/
```

## Source of truth

Every published CSES problem page is backed by exactly three editable files:

- `editorial.tex`
- `solution.cpp`
- `meta.json`

Use them like this:

- edit `editorial.tex` to update the explanation, pseudocode, complexity section, or notes
- edit `solution.cpp` to update the implementation
- edit `meta.json` to update the title, slug, category, official link, tags, level, or short summary

Do not hand-maintain HTML for problem pages. The website renders these files directly.

## `editorial.tex` section contract

The current renderer expects top-level `\section{...}` blocks. Use these names:

- `\section{Problem Summary}`
- `\section{Editorial}`
- `\section{Pseudocode}` (optional)
- `\section{Complexity Analysis}`
- `\section{Notes}` (optional)

Subsections inside `Editorial` are fine and will appear in the page table of contents.

## `meta.json` contract

Each solved problem should keep this shape:

```json
{
  "title": "Weird Algorithm",
  "category": "Introductory Problems",
  "slug": "weird-algorithm",
  "officialUrl": "https://cses.fi/problemset/task/1068",
  "level": "basic",
  "tags": ["simulation", "math"],
  "status": "solved",
  "order": 1,
  "summary": "Generate the Collatz sequence starting from n until it reaches 1."
}
```

The root-level `metadata.json` defines the archive-level metadata and the supported category list.

## Statement policy

This archive is **not** a mirror of CSES statements.

For each problem page:

- write a short original summary in your own words
- link to the official CSES statement
- avoid copying full official statement text
- avoid copying official sample sections when the license position is unclear

## Adding a new CSES problem

1. Choose the official category folder under `competitive_programming/cses/`.
2. Create a problem folder whose name matches the desired slug.
3. Add:
   - `editorial.tex`
   - `solution.cpp`
   - `meta.json`
4. Set `"status": "solved"` only when the editorial and code are both real and publishable.
5. Run `npm run build`.

If a problem is merely planned, prefer keeping that plan in `metadata.json` or in local notes instead of publishing an empty solved page.

## Build and verification

To verify the website routes:

```bash
npm run build
```

To compile a single C++ solution locally when needed:

```bash
g++ -std=c++17 -O2 -pipe -Wall -Wextra competitive_programming/cses/<category>/<slug>/solution.cpp
```

The site itself does not compile the competitive-programming solutions during `npm run build`, so keep the C++ files honest when editing them.
