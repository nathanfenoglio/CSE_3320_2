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
    //unsigned long start = 0; //I don't believe that start is ever used, instead just uses this_vma.vm_start 
    int counter = 0;
    int tot_mem_size = 0;
    char *mem_storage_area_name;

    the_process = find_task_by_vpid(pid); //get pointer reference to process
    the_memory = the_process->active_mm; //the process task_struct contains a struct active_mm
    start_mem_area = the_memory->mmap; //the_memory is an mm_struct grabbed from active_mm struct which was grabbed from find_task_by_pid(pid)
    num_virt_mem_areas = the_memory->map_count; //mm_struct contains int map_count which is the # of memory regions of the process

    //just printing, don't think that this line is needed for the assignment
    printk("\nThere are %i virtual memory areas\n", num_virt_mem_areas);

    one_mem_area = *start_mem_area; //seems this line is unnecessary because one_mem_area is assigned this value in the for loop, but could change to a while loop and assign here
    //start = this_vma_.vm_start; //I don't believe that start is ever used, instead just uses this_vma.vm_start 

    while(counter < num_virt_mem_areas){
        /*
        printk("----------------------------------\n");
        printk("virtual memory area # %i", counter);
        */
        /*
        printk("Start address: 0x%lx\n", one_mem_area.vm_start); //I think perhaps this would be the only thing to print in this 3 line block of code and a \t instead of \n
        printk("End address: 0x%lx\n", one_mem_area.vm_end);
        printk("Total size one_mem_area.vm_end - one_mem_area.vm_start = %lu\n", one_mem_area.vm_end - one_mem_area.vm_start);
        */
        /*
        printk("0x%lx\t", one_mem_area.vm_start);
        printk("%lu\t", one_mem_area.vm_end - one_mem_area.vm_start);
        */

        flags = one_mem_area.vm_flags;
        /*
        printk("%c", flags&PROT_READ ? 'r' : '-'); //I guess bit masking to check specific bits of flags assigned for read, write, exec privileges
        printk("%c", flags&PROT_WRITE ? 'w' : '-');
        printk("%c--\n", flags&PROT_EXEC ? 'x' : '-');

        //trying to put some stuff together in one printk statement
        printk("0x%lx\t%lu\t%c%c%c--\t", one_mem_area.vm_start, one_mem_area.vm_end - one_mem_area.vm_start, flags&PROT_READ ? 'r' : '-', flags&PROT_WRITE ? 'w' : '-', flags&PROT_EXEC ? 'x' : '-');
        */
        /*
        printk("Access permissions:\n");
        printk("Read permission: %s\n", flags&PROT_READ ? "yes" : "no"); //I guess bit masking to check specific bits of flags assigned for read, write, exec privileges
        printk("Write permission: %s\n", flags&PROT_WRITE ? "yes" : "no");
        printk("Execute permission: %s\n", flags&PROT_EXEC ? "yes" : "no");
        */
        /*
        //vm_area_struct has member struct file *vm_file which is the file name that is mapped to. Can be NULL if temporary malloced memory I think...
        if(one_mem_area.vm_file != NULL){
            mem_storage_area_name = d_path(&one_mem_area->f_path, (char *)__get_free_page(GFP_TEMPORARY), PAGE_SIZE); //Convert a dentry into an ASCII path name
            printk("Mapped file: %s\n", mem_storage_area_name);
        }
        else{
            //so perhaps need this else to print [ anon ]
        }
        */
        if(one_mem_area.vm_file != NULL){
            //mem_storage_area_name = d_path(&one_mem_area->f_path, (char *)__get_free_page(GFP_TEMPORARY), PAGE_SIZE); //Convert a dentry into an ASCII path name
            //printk("Mapped file: %s\n", mem_storage_area_name);
            //printk("one_mem_area.vm_file != NULL\n");
            printk("0x%lx\t%lu\t%c%c%c--\t%s", one_mem_area.vm_start, one_mem_area.vm_end - one_mem_area.vm_start, flags&PROT_READ ? 'r' : '-', flags&PROT_WRITE ? 'w' : '-', flags&PROT_EXEC ? 'x' : '-', one_mem_area.vm_file->f_path.dentry->d_iname);
            
            //printk("%s", one_mem_area.vm_file->f_path.dentry->d_iname);
        }
        else{
            //so perhaps need this else to print [ anon ]
            printk("0x%lx\t%lu\t%c%c%c--\t[ anon ]", one_mem_area.vm_start, one_mem_area.vm_end - one_mem_area.vm_start, flags&PROT_READ ? 'r' : '-', flags&PROT_WRITE ? 'w' : '-', flags&PROT_EXEC ? 'x' : '-');
            //printk("[ anon ]\n");
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
