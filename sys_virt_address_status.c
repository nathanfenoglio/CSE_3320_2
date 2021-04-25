#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/fs.h>
#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/mman.h>

SYSCALL_DEFINE2(virt_address_status, unsigned long long, mem, int, pid){
        struct task_struct *task = find_task_by_vpid(pid); //get pointer reference to process
        struct mm_struct *memory = task->active_mm; //the process task_struct contains a struct active_mm which is a struct mm_struct that contains information related to the process address space 
        int data = 0;
        int ref = 0;
        int dirty = 0;

        //pathway through multi-level page table 
        //global directory -> p4 directory -> upper directory -> middle directory -> table

        /*pgd_offset(mm, addr) 
        Receives as parameters the address of a memory descriptor from active_mm struct and a linear address addr received as argument to system call. 
        The macro yields the linear address of the entry in a Page Global Directory that corresponds to the address addr;
        the Page Global Directory is found through a pointer within the memory descriptor.*/ 
        pgd_t *pgd = pgd_offset(memory, mem); //page global directory
        
        //another level to the page table in between global and upper needed, added after the book was written
        p4d_t *p4d = p4d_offset(pgd, mem);
        
        /*pud_offset(pgd, addr) 
        Receives as parameters a pointer pgd to a Page Global Directory entry and a linear address addr received as argument to system call. 
        The macro yields the linear address of the entry in a Page Upper Directory that corresponds to addr. 
        In a two or three-level paging system, this macro yields pgd, the address of a Page Global Directory entry.*/
        //pud_t *pud = pud_offset(pgd, mem); // page upper directory
        pud_t *pud = pud_offset(p4d, mem); // page upper directory
        
        /*pmd_offset(pud, addr)
        Receives as parameters a pointer pud to a Page Upper Directory entry and a linear address addr received as argument to system call. 
        The macro yields the address of the entry in a Page Middle Directory that corresponds to addr. 
        In a two-level paging system, it yields pud, the address of a Page Global Directory entry.*/
        pmd_t *pmd = pmd_offset(pud, mem); // page middle directory
        
        /*pte_offset_kernel(dir, addr)
        Yields the linear address of the Page Table that corresponds to the linear address addr mapped by the Page Middle Directory dir. */
        pte_t *ptep = pte_offset_kernel(pmd, mem); // page table entry pointer
        
        pte_t pte = *ptep; // page table entry
        
        /*The pte_present macro yields the value 1 if either the Present flag or the Page Size
        flag of a Page Table entry is equal to 1, the value 0 otherwise.*/
        data = pte_present(pte);
        printk("Present flag value: %i\n", data ? 1 : 0); //1. If the data in this address is in memory or on disk
        
        //pte_young() reads the Accessed flag
        ref = pte_young(pte);
        printk("Referenced flag value: %i\n", ref ? 1 : 0); //2. If the page which this address belongs to has been referenced or not.
        
        //pte_dirty() Reads the Dirty flag
        dirty = pte_dirty(pte);
        printk("Dirty flag value: %i\n", dirty ? 1 : 0); //3. If the page which this address belongs to is dirty or not.
        
        return 1;
}
