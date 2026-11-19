// SPDX-License-Identifier: GPL-2.0
/*
 * hello_kmod - a trivial kernel module, evolving into a character device driver.
 *
 * Device Numbers: allocate a device number dynamically on load and release it
 * on unload. The name and major number appear in /proc/devices.
 */
#include <linux/module.h>
#include <linux/fs.h>

#define HELLO_KMOD_NAME		"hello_kmod"
#define HELLO_KMOD_COUNT	1

static dev_t hello_kmod_devno;

static int __init hello_kmod_init(void)
{
	int ret;

	ret = alloc_chrdev_region(&hello_kmod_devno, 0, HELLO_KMOD_COUNT,
				  HELLO_KMOD_NAME);
	if (ret < 0) {
		pr_err("hello_kmod: alloc_chrdev_region failed: %d\n", ret);
		return ret;
	}

	pr_info("hello_kmod: hello world, major=%d minor=%d\n",
		MAJOR(hello_kmod_devno), MINOR(hello_kmod_devno));
	return 0;
}

static void __exit hello_kmod_exit(void)
{
	unregister_chrdev_region(hello_kmod_devno, HELLO_KMOD_COUNT);
	pr_info("hello_kmod: bye bye world, released major=%d\n",
		MAJOR(hello_kmod_devno));
}

module_init(hello_kmod_init);
module_exit(hello_kmod_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Trivial hello world kernel module");
MODULE_AUTHOR("Omer Gez");
