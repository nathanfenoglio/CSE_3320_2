#include <linux/kernel.h>
#include <linux/syscalls.h>

SYSCALL_DEFINED(helloworld){
	printk(KERN_EMERG "Hello World !\n");
	return 1;
}

