# chardev 3: Device Files

Builds on [chardev-2-driver-registration](../chardev-2-driver-registration): the device file
is now created automatically when the module loads.

> Use class_create and device_create to create a special device file that will correspond
> to the module.

- On load, after the `cdev` is registered: `class_create("hello_kmod")`, then
  `device_create()` for the allocated device number.
- On unload, in reverse order: `device_destroy()`, `class_destroy()`, `cdev_del()`,
  `unregister_chrdev_region()`.
- The error path unwinds whatever was already set up.

Two differences from the course slides, required by kernel 6.12:

- `class_create()` takes only the name. The `THIS_MODULE` argument was removed in Linux 6.4.
- `class_create()` and `device_create()` return an error pointer on failure, not `NULL`,
  so the results are checked with `IS_ERR()` / `PTR_ERR()`.

## Build

```bash
make BR_OUTPUT=/path/to/buildroot/output
```

## Test (on the Pi)

```
# ls /dev/hello_kmod /sys/class/hello_kmod        → No such file or directory
# insmod hello_kmod.ko                            → hello_kmod: hello world, major=240 minor=0
# ls -l /dev/hello_kmod                           → crw------- 1 root root 240, 0 /dev/hello_kmod
# ls /sys/class/hello_kmod                        → hello_kmod
# cat /sys/class/hello_kmod/hello_kmod/dev        → 240:0
# rmmod hello_kmod
# ls /dev/hello_kmod /sys/class/hello_kmod        → No such file or directory
```

The Pi's `/dev` is a `devtmpfs`, so the kernel creates `/dev/hello_kmod` itself when
`device_create()` runs; no udev is needed.
