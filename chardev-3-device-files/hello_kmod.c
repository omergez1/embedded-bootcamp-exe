// SPDX-License-Identifier: GPL-2.0
/*
 * hello_kmod - a trivial kernel module, evolving into a character device driver.
 *
 * Device Numbers: allocate a device number dynamically on load and release it
 * on unload. The name and major number appear in /proc/devices.
 *
 * Driver Registration: register a statically allocated cdev for that device
 * number, so the kernel routes file operations on it to this driver.
 *
 * Device Files: create a device class and a device, so /sys/class/hello_kmod
 * and /dev/hello_kmod appear on load and disappear on unload.
 */
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

#define HELLO_KMOD_NAME		"hello_kmod"
#define HELLO_KMOD_COUNT	1

static dev_t hello_kmod_devno;
static struct cdev hello_kmod_cdev;
static struct class *hello_kmod_class;

/* No entry points yet: they come with the read/write and ioctl exercises. */
static const struct file_operations hello_kmod_fops = {
	.owner = THIS_MODULE,
};

static int __init hello_kmod_init(void)
{
	struct device *dev;
	int ret;

	ret = alloc_chrdev_region(&hello_kmod_devno, 0, HELLO_KMOD_COUNT,
				  HELLO_KMOD_NAME);
	if (ret < 0) {
		pr_err("hello_kmod: alloc_chrdev_region failed: %d\n", ret);
		return ret;
	}

	cdev_init(&hello_kmod_cdev, &hello_kmod_fops);
	ret = cdev_add(&hello_kmod_cdev, hello_kmod_devno, HELLO_KMOD_COUNT);
	if (ret < 0) {
		pr_err("hello_kmod: cdev_add failed: %d\n", ret);
		goto err_unregister;
	}

	/* Kernel >= 6.4: class_create() takes only the name (no THIS_MODULE). */
	hello_kmod_class = class_create(HELLO_KMOD_NAME);
	if (IS_ERR(hello_kmod_class)) {
		ret = PTR_ERR(hello_kmod_class);
		pr_err("hello_kmod: class_create failed: %d\n", ret);
		goto err_cdev_del;
	}

	dev = device_create(hello_kmod_class, NULL, hello_kmod_devno, NULL,
			    HELLO_KMOD_NAME);
	if (IS_ERR(dev)) {
		ret = PTR_ERR(dev);
		pr_err("hello_kmod: device_create failed: %d\n", ret);
		goto err_class_destroy;
	}

	pr_info("hello_kmod: hello world, major=%d minor=%d\n",
		MAJOR(hello_kmod_devno), MINOR(hello_kmod_devno));
	return 0;

err_class_destroy:
	class_destroy(hello_kmod_class);
err_cdev_del:
	cdev_del(&hello_kmod_cdev);
err_unregister:
	unregister_chrdev_region(hello_kmod_devno, HELLO_KMOD_COUNT);
	return ret;
}

static void __exit hello_kmod_exit(void)
{
	device_destroy(hello_kmod_class, hello_kmod_devno);
	class_destroy(hello_kmod_class);
	cdev_del(&hello_kmod_cdev);
	unregister_chrdev_region(hello_kmod_devno, HELLO_KMOD_COUNT);
	pr_info("hello_kmod: bye bye world, released major=%d\n",
		MAJOR(hello_kmod_devno));
}

module_init(hello_kmod_init);
module_exit(hello_kmod_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Trivial hello world kernel module");
MODULE_AUTHOR("Omer Gez");
