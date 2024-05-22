#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/semaphore.h>
#include <linux/delay.h>

// Module metadata
MODULE_AUTHOR("Anvaya B Narappa");
MODULE_DESCRIPTION("Sem init");
MODULE_LICENSE("GPL");

//semaphore struct 
static struct semaphore sem_st; 

//init task structs 
static struct task_struct *threads[4]; 

struct bankacc { 
    int balance;
}; 

static struct bankacc b; 

//thread function 
//acquires the lock and updates the balance to value specified balance 
static int thread_fn(void* data){ 

    int thread_num = (int)(long) data; 
    int bal = thread_num * 100; 
    //acquire the lock
    printk(KERN_INFO "Acquiring lock\n"); 

    if (down_interruptible(&sem_st)){ 
        printk(KERN_INFO "Thread %d failed to acquire lock\n", thread_num); 
        return -ERESTARTSYS; 
    }

    printk(KERN_INFO "Thread %d successfully acquired lock\n", thread_num); 

    //update balance
    b.balance = bal; 

    printk(KERN_INFO "Thread %d set balance %d\n", thread_num, b.balance); 

    up(&sem_st); 

    msleep(20000);
    return 0; 
}

// init function 
static int __init seminit(void){

    printk(KERN_INFO "Initializing semaphore driver"); 
    //intitialize semaphore
    sema_init(&sem_st, 1); 

    //initialize balance 
    b.balance = 0 ; 
    //run all the threads 
    for (int i = 0 ; i < 4 ; i++){ 
        threads[i] = kthread_run(thread_fn, (void *)(long)i, "thread[%d]", i); 
        if (IS_ERR(threads[i])){ 
            printk(KERN_ALERT "Failed to create thread[%d]\n", i); 
            return PTR_ERR(threads[i]); 
        }
    }
   
    return 0; 
}

//exit function 
static void __exit semexit(void){ 
    int ret;
    printk(KERN_INFO "Clean up module and exiting\n");
    // clean up 
    for(int i = 0 ; i < 4 ; i++){ 
        if (threads[i] && !IS_ERR(threads[i])){ 
            ret = kthread_stop(threads[i]); 
            if (ret != 0){ 
                printk(KERN_ALERT "Error stopping thread[%d]: %d\n", i, ret); 
            } else {
                printk(KERN_INFO "Thread[%d] stopped successfully\n", i);
            }
        }
    }
}



module_init(seminit);
module_exit(semexit);
