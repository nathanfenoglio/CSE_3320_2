#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/fs.h>
#include <asm/page.h>
#include <asm/mman.h>


SYSCALL_DEFINE1(virt_address_stats, int, pid){
    struct task_struct *the_process;
    struct mm_struct *the_memory;
    struct vm_area_struct *start_mem_area;
    struct vm_area_struct one_mem_area;

    int num_virt_mem_areas;
    unsigned long flags = 0;
    int counter = 0;
    int tot_mem_size = 0;

    the_process = find_task_by_vpid(pid); //get pointer reference to process
    the_memory = the_process->active_mm; //the process task_struct contains a struct active_mm which is a struct mm_struct that contains information related to the process address space
    start_mem_area = the_memory->mmap; //the_memory is an mm_struct grabbed from active_mm struct which was grabbed from find_task_by_pid(pid)
    num_virt_mem_areas = the_memory->map_count; //mm_struct contains int map_count which is the # of memory regions of the process

    one_mem_area = *start_mem_area; //initialize one_mem_area to start of the virtual memory linked list to use to traverse the entire list in the while loop
    while(counter < num_virt_mem_areas){
        flags = one_mem_area.vm_flags; //get permission flags from mmap vm_area_struct for currently considered memory area

        //vm_file will contain the name of file of the virtual memory address 
        if(one_mem_area.vm_file != NULL){
            //constructing string to include all of the information in same order as pmap would if entered as a terminal command
            printk("0x%lx\t%lu\t%c%c%c--\t%s", one_mem_area.vm_start, one_mem_area.vm_end - one_mem_area.vm_start, flags&PROT_READ ? 'r' : '-', flags&PROT_WRITE ? 'w' : '-', flags&PROT_EXEC ? 'x' : '-', one_mem_area.vm_file->f_path.dentry->d_iname);            
        }
        else{ //if vm_file is NULL, then file is heap malloced memory to be referred to as [ anon ]
            printk("0x%lx\t%lu\t%c%c%c--\t[ anon ]", one_mem_area.vm_start, one_mem_area.vm_end - one_mem_area.vm_start, flags&PROT_READ ? 'r' : '-', flags&PROT_WRITE ? 'w' : '-', flags&PROT_EXEC ? 'x' : '-');
        }

        //increment tot_mem_size by size of this one_mem_area
        tot_mem_size = tot_mem_size + (one_mem_area.vm_end - one_mem_area.vm_start);

        //check if the last of the linked list
        if(counter != num_virt_mem_areas - 1){
            one_mem_area = *(one_mem_area.vm_next); //move down list vm_next is a pointer in vm_area_struct to the next memory area in the linked list 
        }

        counter++; //increment counter for checking if all virtual memory areas of the process have been dealt with
    }

    printk("\ntotal\t%i\n", tot_mem_size);

    return 1;

}
