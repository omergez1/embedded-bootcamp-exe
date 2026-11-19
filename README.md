# embedded-bootcamp-exe

Exercises from an Embedded Linux course, written in C. Each exercise is in its own folder
with a README describing the task, how to build it, and how to test it.

## Exercises

### User-space programming

| Folder | Topic |
|---|---|
| [job-queue2](job-queue2) | Job queue processed by worker threads, protected by a mutex |

### Character device driver (Raspberry Pi 5)

These evolve one kernel module, `hello_kmod`, step by step into a character device driver.
Each folder contains the complete module at that step.

| Folder | Topic |
|---|---|
| [chardev-1-device-numbers](chardev-1-device-numbers) | Allocate and release a dynamic device number |
| [chardev-2-driver-registration](chardev-2-driver-registration) | Register a character device (`cdev`) |
| [chardev-3-device-files](chardev-3-device-files) | Create `/dev` and `/sys/class` entries automatically |

## Build

User-space exercises build with gcc. Threaded ones need `-lpthread`:

```bash
gcc -Wall -Wextra --std=c99 <exercise>.c -o <exercise> -lpthread
```

Kernel module exercises are cross-compiled for the Raspberry Pi 5 against a
Buildroot-built kernel:

```bash
make BR_OUTPUT=/path/to/buildroot/output
```
