/*
 *  linux/arch/arm/mm/fault.c
 *
 *  Copyright (C) 1995  Linus Torvalds
 *  Modifications for ARM processor (c) 1995-2004 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/module.h>
#include <linux/signal.h>
#include <linux/mm.h>
#include <linux/hardirq.h>
#include <linux/init.h>
#include <linux/kprobes.h>
#include <linux/uaccess.h>
#include <linux/page-flags.h>
#include <linux/sched.h>
#include <linux/highmem.h>
#include <linux/perf_event.h>

#include <asm/exception.h>
#include <asm/pgtable.h>
#include <asm/system_misc.h>
#include <asm/system_info.h>
#include <asm/tlbflush.h>

#include "fault.h"
#include <linux/seq_file.h>


static unsigned int stack_start,stack_end;


static void show_map_vma(struct seq_file *m, struct vm_area_struct *vma,unsigned long esp)
{
	struct mm_struct *mm = vma->vm_mm;
	struct file *file = vma->vm_file;
	int flags = vma->vm_flags;
	unsigned long ino = 0;
	unsigned long long pgoff = 0;
	unsigned long start, end;
	dev_t dev = 0;
	const char *name = NULL;


	if (file) {
		struct inode *inode = file_inode(vma->vm_file);
		dev = inode->i_sb->s_dev;
		ino = inode->i_ino;
		pgoff = ((loff_t)vma->vm_pgoff) << PAGE_SHIFT;
	}
	
	start = vma->vm_start;
	if (stack_guard_page_start(vma, start))
		start += PAGE_SIZE;
	end = vma->vm_end;
	if (stack_guard_page_end(vma, end))
		end -= PAGE_SIZE;
	
	seq_setwidth(m, 25 + sizeof(void *) * 6 - 1);
	seq_printf(m, "%08lx-%08lx %c%c%c%c %08llx %02x:%02x %lu ",
			start,
			end,
			flags & VM_READ ? 'r' : '-',
			flags & VM_WRITE ? 'w' : '-',
			flags & VM_EXEC ? 'x' : '-',
			flags & VM_MAYSHARE ? 's' : 'p',
			pgoff,
			MAJOR(dev), MINOR(dev), ino);

	/*
	 * Print the dentry name for named mappings, and a
	 * special [heap] marker for the heap:
	 */
	if (file) {
		seq_pad(m, ' ');
		seq_path(m, &file->f_path, "\n");
		goto done;
	}

	if (vma->vm_ops && vma->vm_ops->name) {
		name = vma->vm_ops->name(vma);
		if (name)
			goto done;
	}

	name = arch_vma_name(vma);
	if (!name) {

		if (!mm) {
			name = "[vdso]";
			goto done;
		}

		if (vma->vm_start <= mm->brk &&
		    vma->vm_end >= mm->start_brk) {
			name = "[heap]";
			goto done;
		}
        if (vma->vm_start <= esp &&
			    vma->vm_end >= esp) {
				name = "[stack]";
				stack_start = vma->vm_start;
				stack_end = vma->vm_end;
	    }
	}

done:
	if (name) {
		seq_pad(m, ' ');
		seq_puts(m, name);
	}
	seq_putc(m, '\n');
}


#define IMMVAL(bincode)  ((((bincode)&0xf00)==0)?(bincode)&0xff:   \
	(((bincode)&0xff)<<((16-(((bincode)&0xf00)>>8))<<1)))


int count_bitset(unsigned bincode)
{
    unsigned aa;

	aa =  (bincode&0x5555)+((bincode&0xaaaa)>>1);
	aa=   (aa&0x3333)+((aa&0xcccc)>>2); 
	aa= (aa&0x0f0f)+((aa&0xf0f0)>>4);
	aa= (aa&0xff)+((aa&0xff00)>>8);
	return aa;
}
unsigned int getexec_start(unsigned int pc)
{
    struct vm_area_struct * mmap=NULL; 
	if(current==NULL || current->mm==NULL)
		return 0;
	
	mmap = current->mm->mmap;
	while(mmap)
	{
	    if((pc<=mmap->vm_end)
			&&(pc>=mmap->vm_start))

	    {
	        if(VM_EXEC&mmap->vm_flags)
				return mmap->vm_start;
			else 
				return 0xffffffff;
	    }
	    mmap = mmap->vm_next;
	}
	return 0xffffffff;
}
int backtrace(unsigned long epc,unsigned long esp,unsigned long era,void **buffer, int size)
{
	unsigned int addr;
	unsigned int exec_start;
	unsigned int ra;
	unsigned int sp;
    unsigned int bincode=0;
	unsigned int tmpval,stacksize1,stacksize2,stacksize3;    
	int depth=0;
    ra = (unsigned int)epc;
    sp = (unsigned int)esp;

	printk("epc=%08x,esp=%08x,era=%08x\n",(unsigned int)epc,(unsigned int)esp,(unsigned int)era);

	/* Repeat backward scanning */
	for (depth = 0; depth < size && ra; ++depth) 
    {
    printk("depth=%d\n",depth);
    buffer[depth] = (void*)ra;
	/* Scanning to find the size of the current stack-frame */
	stacksize1 = 0;
	stacksize2 = 0;
	stacksize3 = 0;

	exec_start = getexec_start(ra);
	if(exec_start==0xffffffff)
		break;
	
    for(addr=ra; stacksize3==0&&exec_start<=ra;addr-=4)
    {	   	  
       get_user(bincode,(unsigned int*)addr);	
	   if((bincode & 0x0ffff000)==0x024dd000) //sub	sp, sp, stack_size
	   {
	       tmpval = IMMVAL(bincode);
		   printk("find 1 bincode(%08x) at %08x,tmpval=%d\n",bincode,addr,tmpval);
		   if(tmpval>255)  //there maybe two add sp,sp,stack_size
		   {
		      if(stacksize2==0)
			  	stacksize2 = tmpval;		
			  else if(stacksize2!=tmpval)
			  {
			     printk("conflict stacksize\n");
			  }
		   }
		   else 
		   {
		       if(stacksize1==0)
			   	  stacksize1 = tmpval;
			   else if(stacksize1!=tmpval)
			   {
			       printk("conflict stacksize\n");
			   }
		   }		   
	   }

	   else if((bincode & 0x0fff4000)==0x092d4000) //push	{...., lr}
	   {
	      tmpval = count_bitset(bincode);
		  tmpval<<=2;
		  printk("find 2 bincode(%08x) at %08x,tmpval=%d\n",bincode,addr,tmpval);		  
		  if(stacksize3==0)
		  	stacksize3 = tmpval;
		  else if(stacksize3 != tmpval)
		  	printk("conflict stacksize\n");
		  break;		  
	   }
	   else if((bincode & 0xfffffff)==0x052de004)  //push	{lr}
	   {
	      tmpval = 4;
		  printk("find bincode(%08x) at %08x\n",bincode,addr);
		  if(stacksize3==0)
		  	stacksize3 = tmpval;
		  else if(stacksize3 != tmpval)
		  	printk("conflict stacksize\n");
		  break;	
	   }
	   else if((bincode & 0xfffffff)==0x012fff1e) //bx	lr
	   {
	      printk("find 3 bincode(%08x) at %08x\n",bincode,addr);
	      if(depth==0)
		  	ra=era;
	      break;
	   }
   	}
	if((stacksize3+stacksize1+stacksize2)!=0)
	{
	    sp += stacksize3+stacksize1+stacksize2;
		if(sp>=stack_end)
		{
			ra = 0;
			break;
		}
		get_user(ra,((unsigned int*)(sp-4)));		
		printk("ra=%08x,sp=%08x\n",ra,sp);
		if(ra&3)
			break;
	}
	else if(depth!=0)
		break;	
	
	}
	return depth;
}
#define NUM_STACK_ENTRIES   15
#define BUFF_LEN_512 512

void printbacktrace(struct pt_regs *regs)
{
    static void *stackAddr[NUM_STACK_ENTRIES];
    static char sbuf[BUFF_LEN_512];
    int depth,i;
	struct vm_area_struct * mmap=NULL; 
	struct seq_file file;

	stack_start = 0;
	stack_end = 0;
	
	printk(KERN_EMERG "Call Process maps\n");
	//call process mpas  #cat /proc/pid/maps
	if(current!=NULL && current->mm!=NULL)
	{
	mmap = current->mm->mmap;
 
	while(mmap)
	{
	    memset(sbuf,0,BUFF_LEN_512);
		file.buf = sbuf;
		file.size = BUFF_LEN_512;
	    file.count = 0;        
		show_map_vma(&file,mmap,regs->ARM_sp);
		mmap = mmap->vm_next;        
		printk(KERN_EMERG "%s",file.buf);		
	}
	
#if 0	
	if((stack_start!=0) && (stack_end!=0) && (stack_start<stack_end))
	{   
	    printk(KERN_EMERG "\nCall stack dump:\n");
	    for(i=((stack_start+3)&~0x3);i<stack_end;i+=4)
	    {
	       if((i&0x1f)==0)
		   	   printk("\n%08x:%08x  ",i,*((unsigned int*)i));
		   else
		   	   printk("%08x  ",*((unsigned int*)i));
	    }
		printk(KERN_EMERG "\nCall stack dump end\n");
	}
#endif	
	
	}
	

	printk(KERN_EMERG "\nCall Process Trace\n");
	//call process trace 
    depth=backtrace(regs->ARM_pc,regs->ARM_sp,regs->ARM_lr,stackAddr,NUM_STACK_ENTRIES);
    //dump process trace from buff
	for(i=0;i<=depth;i++)
    {  
       memset(sbuf,0,sizeof(sbuf));
       if(current!=NULL && current->mm!=NULL)
       {
           mmap = current->mm->mmap;		   
		   while(mmap)
		   {
		      if(mmap->vm_start<=(unsigned int)stackAddr[i] &&
			  	 mmap->vm_end>= (unsigned int)stackAddr[i])
		      {
		         unsigned int offset;
				 char* filename;
				 struct file *vm_file = mmap->vm_file;
				 if((unsigned int)stackAddr[i]<0x10000000)
				 	offset = (unsigned int)stackAddr[i];
				 else
				 	offset = (unsigned int)stackAddr[i]-mmap->vm_start;

		         offset=sprintf(sbuf,"[%08x] in ",offset);
				 if(vm_file)
				 {
				 	 filename = d_path(&vm_file->f_path, &sbuf[offset], sizeof(sbuf)-offset);
					 if (!IS_ERR(filename))
					 	  memmove(&sbuf[offset],filename,sizeof(sbuf)-offset-(filename-&sbuf[offset]));
				 }
			     break;
		      }
			  mmap = mmap->vm_next;
		   }
       }
       
       printk(KERN_EMERG  "[%d]  [0x%p] %s\n",i,stackAddr[i],sbuf);
    }
	printk(KERN_EMERG "trace end\n");

}
#ifdef CONFIG_MMU

#ifdef CONFIG_KPROBES
static inline int notify_page_fault(struct pt_regs *regs, unsigned int fsr)
{
	int ret = 0;

	if (!user_mode(regs)) {
		/* kprobe_running() needs smp_processor_id() */
		preempt_disable();
		if (kprobe_running() && kprobe_fault_handler(regs, fsr))
			ret = 1;
		preempt_enable();
	}

	return ret;
}
#else
static inline int notify_page_fault(struct pt_regs *regs, unsigned int fsr)
{
	return 0;
}
#endif

/*
 * This is useful to dump out the page tables associated with
 * 'addr' in mm 'mm'.
 */
void show_pte(struct mm_struct *mm, unsigned long addr)
{
	pgd_t *pgd;

	if (!mm)
		mm = &init_mm;

	pr_alert("pgd = %p\n", mm->pgd);
	pgd = pgd_offset(mm, addr);
	pr_alert("[%08lx] *pgd=%08llx",
			addr, (long long)pgd_val(*pgd));

	do {
		pud_t *pud;
		pmd_t *pmd;
		pte_t *pte;

		if (pgd_none(*pgd))
			break;

		if (pgd_bad(*pgd)) {
			pr_cont("(bad)");
			break;
		}

		pud = pud_offset(pgd, addr);
		if (PTRS_PER_PUD != 1)
			pr_cont(", *pud=%08llx", (long long)pud_val(*pud));

		if (pud_none(*pud))
			break;

		if (pud_bad(*pud)) {
			pr_cont("(bad)");
			break;
		}

		pmd = pmd_offset(pud, addr);
		if (PTRS_PER_PMD != 1)
			pr_cont(", *pmd=%08llx", (long long)pmd_val(*pmd));

		if (pmd_none(*pmd))
			break;

		if (pmd_bad(*pmd)) {
			pr_cont("(bad)");
			break;
		}

		/* We must not map this if we have highmem enabled */
		if (PageHighMem(pfn_to_page(pmd_val(*pmd) >> PAGE_SHIFT)))
			break;

		pte = pte_offset_map(pmd, addr);
		pr_cont(", *pte=%08llx", (long long)pte_val(*pte));
#ifndef CONFIG_ARM_LPAE
		pr_cont(", *ppte=%08llx",
		       (long long)pte_val(pte[PTE_HWTABLE_PTRS]));
#endif
		pte_unmap(pte);
	} while(0);

	pr_cont("\n");
}
#else					/* CONFIG_MMU */
void show_pte(struct mm_struct *mm, unsigned long addr)
{ }
#endif					/* CONFIG_MMU */

/*
 * Oops.  The kernel tried to access some page that wasn't present.
 */
static void
__do_kernel_fault(struct mm_struct *mm, unsigned long addr, unsigned int fsr,
		  struct pt_regs *regs)
{
	/*
	 * Are we prepared to handle this kernel fault?
	 */
	if (fixup_exception(regs))
		return;

	/*
	 * No handler, we'll have to terminate things with extreme prejudice.
	 */
	bust_spinlocks(1);
	pr_alert("Unable to handle kernel %s at virtual address %08lx\n",
		 (addr < PAGE_SIZE) ? "NULL pointer dereference" :
		 "paging request", addr);

	show_pte(mm, addr);
	die("Oops", regs, fsr);
	bust_spinlocks(0);
	do_exit(SIGKILL);
}

/*
 * Something tried to access memory that isn't in our memory map..
 * User mode accesses just cause a SIGSEGV
 */
static void
__do_user_fault(struct task_struct *tsk, unsigned long addr,
		unsigned int fsr, unsigned int sig, int code,
		struct pt_regs *regs)
{
	struct siginfo si;

#if defined(CONFIG_BCM_KF_SPECTRE_PATCH) && defined(CONFIG_BCM_SPECTRE_PATCH_ENABLE)
	if (addr > TASK_SIZE)
		harden_branch_predictor();
#endif

#ifdef CONFIG_DEBUG_USER
	if (((user_debug & UDBG_SEGV) && (sig == SIGSEGV)) ||
	    ((user_debug & UDBG_BUS)  && (sig == SIGBUS))) {
		printk(KERN_DEBUG "%s: unhandled page fault (%d) at 0x%08lx, code 0x%03x\n",
		       tsk->comm, sig, addr, fsr);
		show_pte(tsk->mm, addr);
		show_regs(regs);
		printbacktrace(regs);
	}
#endif

	tsk->thread.address = addr;
	tsk->thread.error_code = fsr;
	tsk->thread.trap_no = 14;
	si.si_signo = sig;
	si.si_errno = 0;
	si.si_code = code;
	si.si_addr = (void __user *)addr;
	force_sig_info(sig, &si, tsk);
}

void do_bad_area(unsigned long addr, unsigned int fsr, struct pt_regs *regs)
{
	struct task_struct *tsk = current;
	struct mm_struct *mm = tsk->active_mm;

	/*
	 * If we are in kernel mode at this point, we
	 * have no context to handle this fault with.
	 */
	if (user_mode(regs))
		__do_user_fault(tsk, addr, fsr, SIGSEGV, SEGV_MAPERR, regs);
	else
		__do_kernel_fault(mm, addr, fsr, regs);
}

#ifdef CONFIG_MMU
#define VM_FAULT_BADMAP		0x010000
#define VM_FAULT_BADACCESS	0x020000

/*
 * Check that the permissions on the VMA allow for the fault which occurred.
 * If we encountered a write fault, we must have write permission, otherwise
 * we allow any permission.
 */
static inline bool access_error(unsigned int fsr, struct vm_area_struct *vma)
{
	unsigned int mask = VM_READ | VM_WRITE | VM_EXEC;

	if (fsr & FSR_WRITE)
		mask = VM_WRITE;
	if (fsr & FSR_LNX_PF)
		mask = VM_EXEC;

	return vma->vm_flags & mask ? false : true;
}

static int __kprobes
__do_page_fault(struct mm_struct *mm, unsigned long addr, unsigned int fsr,
		unsigned int flags, struct task_struct *tsk)
{
	struct vm_area_struct *vma;
	int fault;

	vma = find_vma(mm, addr);
	fault = VM_FAULT_BADMAP;
	if (unlikely(!vma))
		goto out;
	if (unlikely(vma->vm_start > addr))
		goto check_stack;

	/*
	 * Ok, we have a good vm_area for this
	 * memory access, so we can handle it.
	 */
good_area:
	if (access_error(fsr, vma)) {
		fault = VM_FAULT_BADACCESS;
		goto out;
	}

	return handle_mm_fault(mm, vma, addr & PAGE_MASK, flags);

check_stack:
	/* Don't allow expansion below FIRST_USER_ADDRESS */
	if (vma->vm_flags & VM_GROWSDOWN &&
	    addr >= FIRST_USER_ADDRESS && !expand_stack(vma, addr))
		goto good_area;
out:
	return fault;
}

static int __kprobes
do_page_fault(unsigned long addr, unsigned int fsr, struct pt_regs *regs)
{
	struct task_struct *tsk;
	struct mm_struct *mm;
	int fault, sig, code;
	unsigned int flags = FAULT_FLAG_ALLOW_RETRY | FAULT_FLAG_KILLABLE;

	if (notify_page_fault(regs, fsr))
		return 0;

	tsk = current;
	mm  = tsk->mm;

	/* Enable interrupts if they were enabled in the parent context. */
	if (interrupts_enabled(regs))
		local_irq_enable();

	/*
	 * If we're in an interrupt or have no user
	 * context, we must not take the fault..
	 */
	if (in_atomic() || !mm)
		goto no_context;

	if (user_mode(regs))
		flags |= FAULT_FLAG_USER;
	if (fsr & FSR_WRITE)
		flags |= FAULT_FLAG_WRITE;

	/*
	 * As per x86, we may deadlock here.  However, since the kernel only
	 * validly references user space from well defined areas of the code,
	 * we can bug out early if this is from code which shouldn't.
	 */
	if (!down_read_trylock(&mm->mmap_sem)) {
		if (!user_mode(regs) && !search_exception_tables(regs->ARM_pc))
			goto no_context;
retry:
		down_read(&mm->mmap_sem);
	} else {
		/*
		 * The above down_read_trylock() might have succeeded in
		 * which case, we'll have missed the might_sleep() from
		 * down_read()
		 */
		might_sleep();
#ifdef CONFIG_DEBUG_VM
		if (!user_mode(regs) &&
		    !search_exception_tables(regs->ARM_pc))
			goto no_context;
#endif
	}

	fault = __do_page_fault(mm, addr, fsr, flags, tsk);

	/* If we need to retry but a fatal signal is pending, handle the
	 * signal first. We do not need to release the mmap_sem because
	 * it would already be released in __lock_page_or_retry in
	 * mm/filemap.c. */
	if ((fault & VM_FAULT_RETRY) && fatal_signal_pending(current)) {
		if (!user_mode(regs))
			goto no_context;
		return 0;
	}

	/*
	 * Major/minor page fault accounting is only done on the
	 * initial attempt. If we go through a retry, it is extremely
	 * likely that the page will be found in page cache at that point.
	 */

	perf_sw_event(PERF_COUNT_SW_PAGE_FAULTS, 1, regs, addr);
	if (!(fault & VM_FAULT_ERROR) && flags & FAULT_FLAG_ALLOW_RETRY) {
		if (fault & VM_FAULT_MAJOR) {
			tsk->maj_flt++;
			perf_sw_event(PERF_COUNT_SW_PAGE_FAULTS_MAJ, 1,
					regs, addr);
		} else {
			tsk->min_flt++;
			perf_sw_event(PERF_COUNT_SW_PAGE_FAULTS_MIN, 1,
					regs, addr);
		}
		if (fault & VM_FAULT_RETRY) {
			/* Clear FAULT_FLAG_ALLOW_RETRY to avoid any risk
			* of starvation. */
			flags &= ~FAULT_FLAG_ALLOW_RETRY;
			flags |= FAULT_FLAG_TRIED;
			goto retry;
		}
	}

	up_read(&mm->mmap_sem);

	/*
	 * Handle the "normal" case first - VM_FAULT_MAJOR / VM_FAULT_MINOR
	 */
	if (likely(!(fault & (VM_FAULT_ERROR | VM_FAULT_BADMAP | VM_FAULT_BADACCESS))))
		return 0;

	/*
	 * If we are in kernel mode at this point, we
	 * have no context to handle this fault with.
	 */
	if (!user_mode(regs))
		goto no_context;

	if (fault & VM_FAULT_OOM) {
		/*
		 * We ran out of memory, call the OOM killer, and return to
		 * userspace (which will retry the fault, or kill us if we
		 * got oom-killed)
		 */
		pagefault_out_of_memory();
		return 0;
	}

	if (fault & VM_FAULT_SIGBUS) {
		/*
		 * We had some memory, but were unable to
		 * successfully fix up this page fault.
		 */
		sig = SIGBUS;
		code = BUS_ADRERR;
	} else {
		/*
		 * Something tried to access memory that
		 * isn't in our memory map..
		 */
		sig = SIGSEGV;
		code = fault == VM_FAULT_BADACCESS ?
			SEGV_ACCERR : SEGV_MAPERR;
	}

	__do_user_fault(tsk, addr, fsr, sig, code, regs);
	return 0;

no_context:
	__do_kernel_fault(mm, addr, fsr, regs);
	return 0;
}
#else					/* CONFIG_MMU */
static int
do_page_fault(unsigned long addr, unsigned int fsr, struct pt_regs *regs)
{
	return 0;
}
#endif					/* CONFIG_MMU */

/*
 * First Level Translation Fault Handler
 *
 * We enter here because the first level page table doesn't contain
 * a valid entry for the address.
 *
 * If the address is in kernel space (>= TASK_SIZE), then we are
 * probably faulting in the vmalloc() area.
 *
 * If the init_task's first level page tables contains the relevant
 * entry, we copy the it to this task.  If not, we send the process
 * a signal, fixup the exception, or oops the kernel.
 *
 * NOTE! We MUST NOT take any locks for this case. We may be in an
 * interrupt or a critical region, and should only copy the information
 * from the master page table, nothing more.
 */
#ifdef CONFIG_MMU
static int __kprobes
do_translation_fault(unsigned long addr, unsigned int fsr,
		     struct pt_regs *regs)
{
	unsigned int index;
	pgd_t *pgd, *pgd_k;
	pud_t *pud, *pud_k;
	pmd_t *pmd, *pmd_k;

	if (addr < TASK_SIZE)
		return do_page_fault(addr, fsr, regs);

	if (user_mode(regs))
		goto bad_area;

	index = pgd_index(addr);

	pgd = cpu_get_pgd() + index;
	pgd_k = init_mm.pgd + index;

	if (pgd_none(*pgd_k))
		goto bad_area;
	if (!pgd_present(*pgd))
		set_pgd(pgd, *pgd_k);

	pud = pud_offset(pgd, addr);
	pud_k = pud_offset(pgd_k, addr);

	if (pud_none(*pud_k))
		goto bad_area;
	if (!pud_present(*pud))
		set_pud(pud, *pud_k);

	pmd = pmd_offset(pud, addr);
	pmd_k = pmd_offset(pud_k, addr);

#ifdef CONFIG_ARM_LPAE
	/*
	 * Only one hardware entry per PMD with LPAE.
	 */
	index = 0;
#else
	/*
	 * On ARM one Linux PGD entry contains two hardware entries (see page
	 * tables layout in pgtable.h). We normally guarantee that we always
	 * fill both L1 entries. But create_mapping() doesn't follow the rule.
	 * It can create inidividual L1 entries, so here we have to call
	 * pmd_none() check for the entry really corresponded to address, not
	 * for the first of pair.
	 */
	index = (addr >> SECTION_SHIFT) & 1;
#endif
	if (pmd_none(pmd_k[index]))
		goto bad_area;

	copy_pmd(pmd, pmd_k);
	return 0;

bad_area:
	do_bad_area(addr, fsr, regs);
	return 0;
}
#else					/* CONFIG_MMU */
static int
do_translation_fault(unsigned long addr, unsigned int fsr,
		     struct pt_regs *regs)
{
	return 0;
}
#endif					/* CONFIG_MMU */

/*
 * Some section permission faults need to be handled gracefully.
 * They can happen due to a __{get,put}_user during an oops.
 */
#ifndef CONFIG_ARM_LPAE
static int
do_sect_fault(unsigned long addr, unsigned int fsr, struct pt_regs *regs)
{
	do_bad_area(addr, fsr, regs);
	return 0;
}
#endif /* CONFIG_ARM_LPAE */

/*
 * This abort handler always returns "fault".
 */
static int
do_bad(unsigned long addr, unsigned int fsr, struct pt_regs *regs)
{
	return 1;
}

struct fsr_info {
	int	(*fn)(unsigned long addr, unsigned int fsr, struct pt_regs *regs);
	int	sig;
	int	code;
	const char *name;
};

/* FSR definition */
#ifdef CONFIG_ARM_LPAE
#include "fsr-3level.c"
#else
#include "fsr-2level.c"
#endif

void __init
hook_fault_code(int nr, int (*fn)(unsigned long, unsigned int, struct pt_regs *),
		int sig, int code, const char *name)
{
	if (nr < 0 || nr >= ARRAY_SIZE(fsr_info))
		BUG();

	fsr_info[nr].fn   = fn;
	fsr_info[nr].sig  = sig;
	fsr_info[nr].code = code;
	fsr_info[nr].name = name;
}

/*
 * Dispatch a data abort to the relevant handler.
 */
asmlinkage void __exception
do_DataAbort(unsigned long addr, unsigned int fsr, struct pt_regs *regs)
{
	const struct fsr_info *inf = fsr_info + fsr_fs(fsr);
	struct siginfo info;

	if (!inf->fn(addr, fsr & ~FSR_LNX_PF, regs))
		return;

	pr_alert("Unhandled fault: %s (0x%03x) at 0x%08lx\n",
		inf->name, fsr, addr);
	show_pte(current->mm, addr);

	info.si_signo = inf->sig;
	info.si_errno = 0;
	info.si_code  = inf->code;
	info.si_addr  = (void __user *)addr;
	arm_notify_die("", regs, &info, fsr, 0);
}

void __init
hook_ifault_code(int nr, int (*fn)(unsigned long, unsigned int, struct pt_regs *),
		 int sig, int code, const char *name)
{
	if (nr < 0 || nr >= ARRAY_SIZE(ifsr_info))
		BUG();

	ifsr_info[nr].fn   = fn;
	ifsr_info[nr].sig  = sig;
	ifsr_info[nr].code = code;
	ifsr_info[nr].name = name;
}

asmlinkage void __exception
do_PrefetchAbort(unsigned long addr, unsigned int ifsr, struct pt_regs *regs)
{
	const struct fsr_info *inf = ifsr_info + fsr_fs(ifsr);
	struct siginfo info;

	if (!inf->fn(addr, ifsr | FSR_LNX_PF, regs))
		return;

	pr_alert("Unhandled prefetch abort: %s (0x%03x) at 0x%08lx\n",
		inf->name, ifsr, addr);

	info.si_signo = inf->sig;
	info.si_errno = 0;
	info.si_code  = inf->code;
	info.si_addr  = (void __user *)addr;
	arm_notify_die("", regs, &info, ifsr, 0);
}

#ifndef CONFIG_ARM_LPAE
static int __init exceptions_init(void)
{
	if (cpu_architecture() >= CPU_ARCH_ARMv6) {
		hook_fault_code(4, do_translation_fault, SIGSEGV, SEGV_MAPERR,
				"I-cache maintenance fault");
	}

	if (cpu_architecture() >= CPU_ARCH_ARMv7) {
		/*
		 * TODO: Access flag faults introduced in ARMv6K.
		 * Runtime check for 'K' extension is needed
		 */
		hook_fault_code(3, do_bad, SIGSEGV, SEGV_MAPERR,
				"section access flag fault");
		hook_fault_code(6, do_bad, SIGSEGV, SEGV_MAPERR,
				"section access flag fault");
	}

	return 0;
}

arch_initcall(exceptions_init);
#endif
