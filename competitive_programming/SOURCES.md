# Competitive Programming Sources

This directory is the source-backed archive for the website's **Competitive Programming** section.

It currently combines three maintained archives plus the DSA notebook:

- `ICPC`: copied from `C:\Users\ADMIN\Downloads\ICPC`
- `IOI`: copied from `E:\IOI`
- `CSES`: maintained directly in this repository under `competitive_programming/cses`

The website reads the files in this directory directly at build time. The archive is intentionally organized so the editable source files stay obvious and stable.

## Source of truth

For every problem entry:

- the written solution source of truth is the local `solution.tex` for ICPC and IOI
- the written solution source of truth is the local `editorial.tex` for CSES
- the implementation source of truth is the local `solution.cpp`

The website pages do not replace those files with hand-maintained HTML copies.

## What was copied

### ICPC

The website copy keeps:

- yearly `contest_problems.pdf` files
- per-problem `meta.json`
- per-problem `solution.tex`
- per-problem `solution.cpp`
- per-problem `statement.txt` when available
- per-problem `statement.pdf` when available

The website copy does **not** keep temporary caches, generated binaries, helper scripts, or unrelated repository clutter from the source tree.

### IOI

The website copy keeps:

- per-problem `solution.tex`
- per-problem `solution.cpp`

When the original IOI source folder used task-specific filenames such as `message.tex` or `message.cpp`, those files were copied into this archive under the normalized names:

- `solution.tex`
- `solution.cpp`

The file contents were not rewritten.

The website copy does **not** keep local brute-force programs, stress scripts, binaries, or other ad-hoc verification artefacts from the source tree.

### CSES

The CSES archive is authored directly for this site instead of being imported from a separate local repository.

Each solved problem keeps:

- `editorial.tex`
- `solution.cpp`
- `meta.json`

The category structure mirrors the official CSES problemset categories so future batches can be added without changing the route scheme.

## How the website uses this directory

The Astro site scans `competitive_programming/` directly and builds:

- `/competitive-programming`
- `/competitive-programming/cses`
- `/competitive-programming/cses/<category>`
- `/competitive-programming/cses/<category>/<slug>`
- `/competitive-programming/icpc`
- `/competitive-programming/icpc/<year>`
- `/competitive-programming/ioi`
- `/competitive-programming/ioi/<year>`
- per-entry pages under `/competitive-programming/<track>/<year>/<slug>`

Each entry page exposes the exact copied or authored source files. ICPC pages also expose copied statement assets when they exist. CSES pages link to official statements instead of mirroring them.

Public pages are intentionally written for readers first. Workflow notes such as source-of-truth and editing rules stay in this documentation instead of taking over the main site copy.

Raw file URLs are generated from this directory through the site's `competitive-programming-assets` routes, so the repo keeps one clear local source tree instead of duplicating everything into `public/`.

## How to edit an existing solution

If you want to edit the written solution:

- edit that entry's `solution.tex`

If you want to edit the implementation:

- edit that entry's `solution.cpp`

Do not edit the website pages to change the solution content. The website pages are presentation around the source files, not the source of truth themselves.

## How to add new solutions later

### Add a new ICPC solution

1. Create or copy a problem folder under `competitive_programming/icpc/<YEAR>/`.
2. Keep at least:
   - `meta.json`
   - `solution.tex`
   - `solution.cpp`
3. Add `statement.txt` and `statement.pdf` if you want the statement assets available on the site.
4. Rebuild the site.

### Add a new IOI solution

1. Create or copy a problem folder under `competitive_programming/ioi/<YEAR>/`.
2. Keep:
   - `solution.tex`
   - `solution.cpp`
3. Rebuild the site.

### Add a new CSES solution

1. Create a problem folder under `competitive_programming/cses/<CATEGORY-SLUG>/`.
2. Keep:
   - `editorial.tex`
   - `solution.cpp`
   - `meta.json`
3. Mark the problem `"status": "solved"` only when the editorial and code are both real.
4. Rebuild the site.

## Re-importing from external source repositories

The import used for this archive is scripted:

- `tools/competitive-programming/import-sources.mjs`

Example:

```bash
node tools/competitive-programming/import-sources.mjs --icpc "C:\Users\ADMIN\Downloads\ICPC" --ioi "E:\IOI"
```

That script refreshes the managed `competitive_programming/icpc` and `competitive_programming/ioi` trees from the external source folders while keeping the website-facing structure consistent.

CSES is maintained directly in this repository, so it is not part of the import script.
