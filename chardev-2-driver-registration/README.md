# chardev 2: Driver Registration

Builds on [chardev-1-device-numbers](../chardev-1-device-numbers): the module now registers
itself as a character device.

> Register the module as a character device. Use dynamic device number allocation.
> Statically allocate the cdev structure.

- A statically allocated `struct cdev` and a `file_operations` table (no entry points yet,
  only `.owner = THIS_MODULE`).
- On load: `alloc_chrdev_region()`, then `cdev_init()` + `cdev_add()`. If `cdev_add()`
  fails, the device number is released before returning.
- On unload, in reverse order: `cdev_del()`, then `unregister_chrdev_region()`.

## Build

```bash
make BR_OUTPUT=/path/to/buildroot/output
```

## Test (on the Pi)

`/proc/devices` lists the module while it is loaded, as in exercise 1. To show that the
`cdev` is registered, create a device file with the allocated major and read it:

```
# insmod hello_kmod.ko              → hello_kmod: hello world, major=240 minor=0
# grep hello /proc/devices          → 240 hello_kmod
# mknod /tmp/hello c 240 0
# cat /tmp/hello                    → read error: Invalid argument
                                      (driver found; no read entry point yet)
# rmmod hello_kmod
# grep hello /proc/devices          → (not listed)
# cat /tmp/hello                    → can't open: No such device or address
                                      (no driver registered for major 240)
```
