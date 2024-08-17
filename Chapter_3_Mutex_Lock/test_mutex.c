#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

static struct mutex my_mutex;
static char *buffer = NULL;
static size_t buffer_size = 1024; // Size of allocated buffer

static int test_function(void)
{
    int ret = 0;

    // Allocate memory using kmalloc
    buffer = kmalloc(buffer_size, GFP_KERNEL);
    if (!buffer) {
        pr_info("Failed to allocate memory\n");
        return -ENOMEM;
    }

    // Lock mutex
    mutex_lock(&my_mutex);

    pr_info("Buffer allocated at: %px\n", buffer);

    // Unlock mutex
    mutex_unlock(&my_mutex);

    // Free allocated memory
    kfree(buffer);

    return ret;
}

static int __init mutex_module_init(void)
{
    pr_info("Initializing my module\n");

    // Initialize mutex
    mutex_init(&my_mutex);

    // Call example function
    test_function();

    return 0;
}

static void __exit mutex_module_exit(void)
{
    pr_info("Exiting my module\n");

    // Clean up mutex
    mutex_destroy(&my_mutex);
}

module_init(mutex_module_init);
module_exit(mutex_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Suyog B <suyogburadkar@gmail.com>");
MODULE_DESCRIPTION("Kernel module demonstrates mutex locking");
