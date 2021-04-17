#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/sched.h>

SYSCALL_DEFINE0(print_stuff){
	struct task_struct *p;
	p = current;

	printk(KERN_INFO "process id (pid): %i\n", p->pid);
	printk(KERN_INFO "state: %ld\n", p->state);
	printk(KERN_INFO "program name: %s\n", p->comm);
	printk(KERN_INFO "start time: %llu\n", p->real_start_time);
	printk(KERN_INFO "virtual runtime: %llu\n", p->se.vruntime);
	printk(KERN_INFO "priority: %d\n", p->prio);
    	/*
	printk(KERN_INFO "static prio: %d\n", p->static_prio);
    	printk(KERN_INFO "parent_exec_id: %llu\n", p->parent_exec_id);
    	printk(KERN_INFO "self_exec_id: %llu\n", p->self_exec_id);
    	printk(KERN_INFO "policy: %u\n", p->policy);
	*/

	//print parent processes up until init
	struct task_struct *q;
	for(q = p; q != &init_task; q = q->parent){
		printk("program name: %s\n", q->comm);
	}

	return 1;
}

