#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/sched.h>

SYSCALL_DEFINE0(helloworld){
	printk(KERN_EMERG "Hello World !\n");
	
	return 1;
}

//from the point of view of I already had this file and it was working fine 
//but it is on my local computer only and I would actually like to get it into another person's git repository on my own separate branch so that both can look at

