# chardev 1: Device Numbers

First step in turning the `hello_kmod` kernel module into a character device driver.

> Register a dynamically allocated device number in the module's initialization function,
> and unregister it in the module's exit function.

- `alloc_chrdev_region()` on load: the kernel picks a free major number (minor 0, 1 device)
  under the name `hello_kmod`.
- `unregister_chrdev_region()` on unload: the number is released.

## Build

Cross-compiled for the Raspberry Pi 5 against a Buildroot-built kernel (6.12):

```bash
make BR_OUTPUT=/path/to/buildroot/output
```

## Test (on the Pi)

```
# grep hello /proc/devices          → (not listed)
# insmod hello_kmod.ko              → hello_kmod: hello world, major=240 minor=0
# grep hello /proc/devices          → 240 hello_kmod
# rmmod hello_kmod                  → hello_kmod: bye bye world, released major=240
# grep hello /proc/devices          → (not listed)
```
