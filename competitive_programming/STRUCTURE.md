# Competitive Programming Structure

This directory is organized by archive family first. ICPC and IOI stay year-based; CSES stays category-based.

## Layout

```text
competitive_programming/
  SOURCES.md
  STRUCTURE.md
  cses/
    metadata.json
    STRUCTURE.md
    introductory-problems/
      weird-algorithm/
        meta.json
        editorial.tex
        solution.cpp
  icpc/
    2024/
      contest_problems.pdf
      A-billboards/
        meta.json
        solution.tex
        solution.cpp
        statement.txt
        statement.pdf
  ioi/
    2024/
      message/
        solution.tex
        solution.cpp
```

## Design rules

- `icpc/`, `ioi/`, `dsa/`, and `cses/` are separate top-level archive branches.
- ICPC and IOI keep year-based contest history.
- CSES keeps the official problemset categories directly under `cses/`.
- Each problem gets its own directory.
- The problem directory is the place to edit and maintain that entry.
- `solution.tex` stays the written-source canonical file for ICPC and IOI.
- `editorial.tex` is the written-source canonical file for CSES.
- `solution.cpp` is the implementation-source canonical file for all problem archives.

## Why this shape

This structure is intended to stay:

- easy to browse on GitHub
- easy to turn into public pages that browse by track, year, then problem
- easy to extend with more yearly folders or more category-based archives later
- easy for the website build to scan without hidden conventions

## Track-specific notes

### ICPC

ICPC problem folders preserve the existing contest-style metadata and statement assets when they are useful:

- `meta.json`
- `statement.txt`
- `statement.pdf`

Year folders may also keep `contest_problems.pdf`.

### IOI

IOI problem folders are intentionally minimal:

- `solution.tex`
- `solution.cpp`

The folder name identifies the task, while the normalized filenames keep the per-entry contract consistent across the archive.

### CSES

CSES uses the official category names as stable folder groups:

- `introductory-problems/`
- `sorting-and-searching/`
- `dynamic-programming/`
- `graph-algorithms/`
- `range-queries/`
- `tree-algorithms/`
- `mathematics/`
- `string-algorithms/`
- `geometry/`
- `advanced-techniques/`
- `additional-problems/`

Each solved CSES problem keeps:

- `editorial.tex`
- `solution.cpp`
- `meta.json`

Archive-level metadata and maintenance notes live in:

- `cses/metadata.json`
- `cses/STRUCTURE.md`
