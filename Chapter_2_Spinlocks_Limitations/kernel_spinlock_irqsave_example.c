#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/spinlock.h>

struct dummy_struct {
    spinlock_t lock;
    unsigned int foo;
};

static struct dummy_struct *ds;

static struct dummy_struct *demo_alloc_init_function(void) {
    struct dummy_struct *ds;
    ds = kmalloc(sizeof(struct dummy_struct), GFP_KERNEL);
    if (!ds)
        return NULL;
    spin_lock_init(&ds->lock);
    return ds;
}

static int __init my_module_init(void)
{
    unsigned long flags;

    pr_info("Initializing Spinlock Example Module\n");
    ds = demo_alloc_init_function();
    if (!ds)
    {
        pr_alert("Allocation failed\n");
        return -ENOMEM;
    }

    // Lock the spinlock and save the interrupt state
    spin_lock_irqsave(&ds->lock, flags);
    pr_info("Spinlock acquired and interrupts disabled\n");

    // Simulate a critical section
    ds->foo = 42;
    pr_info("Critical section executed, foo = %u\n", ds->foo);

    // Unlock the spinlock and restore the interrupt state
    spin_unlock_irqrestore(&ds->lock, flags);
    pr_info("Spinlock released and interrupts restored\n");

    return 0;
}

static void __exit my_module_exit(void)
{
    pr_info("Exiting Spinlock Example Module\n");
    // Free the allocated memory
    if (ds)
    {
        kfree(ds);
        pr_info("Memory freed\n");
    }
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Suyog B <suyogburadkar@gmail.com>");
MODULE_DESCRIPTION("An example kernel module using spin_lock_irqsave and spin_unlock_irqrestore");
MODULE_VERSION("1.0");
