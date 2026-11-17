# Embedded Linux exercises

One repo for all the small exercises from the Linux user-space programming course
(LOGTEL, 2026 Q4). Each exercise is a standalone C file in the repo root.

## Course materials (in ~/Downloads, not in the repo)
- `User-Space-Linux-Programming-Labs-2026Q4.pdf` - the official lab exercises (6 slides).
- `User-Space-Linux-Programming-2026Q4.pdf` - lecture slides. Example files are only
  named there (embedded in the original pptx), not shown.
- Base/template `.c` files are downloaded to ~/Downloads and copied into the repo.
  Commit the untouched template first, then the solution, so `git diff` shows the work.
- Some tasks come from the instructor verbally and are not in any file
  (e.g. randomizing thread work time in no-join.c). The user will describe them.

## Exercises
| File | Source | Status |
|---|---|---|
| job-queue2.c | Labs: "Fill in job-queue2.c" | done |
| no-join.c | Lecture p.157 example + instructor: randomize per-thread work time | done |
| job-queue3.c | Labs: condition variables (complete it, then replace semaphore with condvars) | not started, file not downloaded yet |

## Build
    gcc -Wall -Wextra --std=c99 <file>.c -o <file> -lpthread
Put `-lpthread` after the source file. Compiled binaries are not committed:
add each new binary name to `.gitignore`.

## Git
- Work on `dev`, merge into `main` when an exercise is done.
- Commit and push only when the user asks.
