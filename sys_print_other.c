#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/sched.h>

SYSCALL_DEFINE1(print_other, pid_t, pid_to_find)
{
    struct task_struct *p;
    int found = 0;

    for (p = &init_task; next_task(p) != &init_task; p = next_task(p))
    {
        if (p->pid == pid_to_find)
        {
            found = 1;
            break;
        }
    }

    if (found)
    {
        printk(KERN_INFO "process id (pid): %i\n", p->pid);
        printk(KERN_INFO "state: %ld\n", p->state);
        printk(KERN_INFO "program name: %s\n", p->comm);
        printk(KERN_INFO "start time: %llu\n", p->real_start_time);
        printk(KERN_INFO "virtual runtime: %llu\n", p->se.vruntime);
    }
    else
    {
        printk("process id specified was not found\n");
    }
    return 0;
}
