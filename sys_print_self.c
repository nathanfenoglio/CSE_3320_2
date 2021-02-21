#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/sched.h>

SYSCALL_DEFINE0(print_self)
{
    struct task_struct *p;
    p = current;

    printk(KERN_INFO "process id (pid): %i\n", p->pid);
    printk(KERN_INFO "state: %ld\n", p->state);
    printk(KERN_INFO "program name: %s\n", p->comm);
    printk(KERN_INFO "start time: %llu\n", p->real_start_time);
    printk(KERM_INFO "virtual runtime: %llu\n", p->se.vruntime);

    struct task_struct *q;
    for (q = p; q != &init_task; q = q->parent)
    {
        printk("program name: %s\n", q->comm);
    }
    return 1;
}
