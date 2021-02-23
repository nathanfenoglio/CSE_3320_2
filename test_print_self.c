#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>

#define __NR_print_self 334

int main(int argc, char *argv[])
{
    syscall(__NR_print_self);
    return 0;
}
