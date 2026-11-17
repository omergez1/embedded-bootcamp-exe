# embedded-bootcamp-exe

A collection of small exercises from an Embedded Linux course, written in C.

Exercises cover topics from across the course, such as processes, signals,
threads, synchronization and inter-process communication. Each one is a
standalone source file, and new exercises are added as the course goes on.

## Build

Most exercises build with a single gcc command. Threaded ones need `-lpthread`:

```bash
gcc -Wall -Wextra <exercise>.c -o <exercise> -lpthread
./<exercise>
```
