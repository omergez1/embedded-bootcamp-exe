# job-queue2

A job queue processed by a pool of worker threads, protected by a mutex.

- The main thread fills a linked-list queue with 10 jobs, then starts 3 worker threads.
- Each worker locks the queue mutex, takes the next job, unlocks, and processes the job
  (prints which thread handled it and sleeps 100 ms).
- A worker exits when it finds the queue empty; `main` joins all workers.

The queue is filled before the threads start, because a worker that finds the queue
empty exits immediately.

## Build and run

```bash
gcc -Wall -Wextra --std=c99 job-queue2.c -o job-queue2 -lpthread
./job-queue2
```
