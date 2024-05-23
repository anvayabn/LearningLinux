#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/semaphore.h>
#include <linux/delay.h>
#include <linux/spinlock.h>
#include <linux/ktime.h>

// Module metadata
MODULE_AUTHOR("Anvaya B Narappa");
MODULE_DESCRIPTION("Spink Lock test");
MODULE_LICENSE("GPL");

// spin lock
static spinlock_t lock;

// init task structs
static struct task_struct *threads[3];

struct bankacc
{
    int balance;
};

static struct bankacc b;

// thread function
// acquires the lock and updates the balance to value specified balance
static int thread_fn(void *data)
{
    ktime_t start, end;
    s64 elapsed_time;
    int thread_num = (int)(long)data;
    int bal = thread_num * 100;
    // acquire the lock
    printk(KERN_INFO "Acquiring lock\n");

    /*
     * start a timer
     * aquire lock
     * end timer
     * dsplay the amount time it took
     */
    start = ktime_get();
    spin_lock(&lock);
    end = ktime_get();
    elapsed_time = ktime_to_ns(ktime_sub(end, start));

    printk(KERN_INFO "Thread %d successfully acquired lock in time : %lld\n", thread_num, elapsed_time);

    // update balance
    b.balance = bal;

    printk(KERN_INFO "Thread %d set balance %d\n", thread_num, b.balance);

    // release the lock
    spin_unlock(&lock);

    msleep(20000);
    return 0;
}

// init function
static int __init spininit(void)
{

    printk(KERN_INFO "Initializing semaphore driver");

    // init spin lock
    spin_lock_init(&lock);

    // initialize balance
    b.balance = 0;
    // run all the threads
    for (int i = 0; i < 3; i++)
    {
        threads[i] = kthread_run(thread_fn, (void *)(long)i, "thread[%d]", i);
        if (IS_ERR(threads[i]))
        {
            printk(KERN_ALERT "Failed to create thread[%d]\n", i);
            return PTR_ERR(threads[i]);
        }
    }

    return 0;
}

// exit function
static void __exit spinexit(void)
{
    int ret;
    printk(KERN_INFO "Clean up module and exiting\n");
    // clean up
    for (int i = 0; i < 3; i++)
    {
        if (threads[i] && !IS_ERR(threads[i]))
        {
            ret = kthread_stop(threads[i]);
            if (ret != 0)
            {
                printk(KERN_ALERT "Error stopping thread[%d]: %d\n", i, ret);
            }
            else
            {
                printk(KERN_INFO "Thread[%d] stopped successfully\n", i);
            }
        }
    }
}

module_init(spininit);
module_exit(spinexit);
