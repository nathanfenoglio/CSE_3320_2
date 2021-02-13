#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/sched.h>

SYSCALL_DEFINE1(print_other, pid_t, pid_to_find){
	struct task_struct *p;
	int found = 0;//to specify if the pid_to_find is found
	
	//start pointer at init_task, follow down list through next_task pointers until looped back	     //around to init_task so covered every task
	for(p = &init_task; next_task(p) != &init_task; p = next_task(p)){
		
		if(p->pid == pid_to_find){
			found = 1; 
			//if process id is found, no need to continue, break to save current pointer
			break;
		}
	}

	if(found){
		printk(KERN_INFO "process id (pid): %i\n", p->pid);
		printk(KERN_INFO "state: %ld\n", p->state);		
		printk(KERN_INFO "program name: %s\n", p->comm);
		printk(KERN_INFO "start time: %llu\n", p->real_start_time);
		printk(KERN_INFO "virtual runtime: %llu\n", p->se.vruntime);
	}
	else{
		printk("process id specified was not found\n");
	}

	return 0;

}

