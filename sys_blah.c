#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/sched.h>

SYSCALL_DEFINE0(blah){
    printk(KERN_EMERG "\nblah\n");

    return 1;
}
