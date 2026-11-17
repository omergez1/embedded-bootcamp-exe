# embedded-bootcamp-exe

Exercises from a Linux user-space programming course: processes, signals,
POSIX threads, synchronization and IPC in C.

Each exercise is a standalone C file.

## Exercises

| File | Topic |
|---|---|
| `job-queue2.c` | Thread job queue protected by a mutex, with jobs allocated using `malloc` |

## Build and run

```bash
gcc -Wall -Wextra job-queue2.c -o job-queue2 -lpthread
./job-queue2
```
