#include <stdio.h>
#include <stdlib.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>

#define __NR_print_other 335

int main(int argc, char **argv)
{
    char *pid_to_find = argv[1];
    int num = atoi(pid_to_find);
    printf("process id to find: %d\n", num);
    int ret = syscall(__NR_print_other, num);
    printf("syscall return value: %d\n", ret);
    return 0;
}
