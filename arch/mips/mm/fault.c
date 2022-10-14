/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1995 - 2000 by Ralf Baechle
 */
#include <linux/context_tracking.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/ptrace.h>
#include <linux/ratelimit.h>
#include <linux/mman.h>
#include <linux/mm.h>
#include <linux/smp.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/perf_event.h>

#include <asm/branch.h>
#include <asm/mmu_context.h>
#include <asm/uaccess.h>
#include <asm/ptrace.h>
#include <asm/highmem.h>		/* For VMALLOC_END */
#include <linux/kdebug.h>

int show_unhandled_signals = 1;

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
		show_map_vma(&file,mmap,regs->regs[29]);
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
	depth=backtrace(regs->cp0_epc,regs->regs[29],regs->regs[31],stackAddr,NUM_STACK_ENTRIES);
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

/*
 * This routine handles page faults.  It determines the address,
 * and the problem, and then passes it off to one of the appropriate
 * routines.
 */
static void __kprobes __do_page_fault(struct pt_regs *regs, unsigned long write,
	unsigned long address)
{
	struct vm_area_struct * vma = NULL;
	struct task_struct *tsk = current;
	struct mm_struct *mm = tsk->mm;
	const int field = sizeof(unsigned long) * 2;
	siginfo_t info;
	int fault;
	unsigned int flags = FAULT_FLAG_ALLOW_RETRY | FAULT_FLAG_KILLABLE;

	static DEFINE_RATELIMIT_STATE(ratelimit_state, 5 * HZ, 10);

#if 0
	printk("Cpu%d[%s:%d:%0*lx:%ld:%0*lx]\n", raw_smp_processor_id(),
	       current->comm, current->pid, field, address, write,
	       field, regs->cp0_epc);
#endif

#ifdef CONFIG_KPROBES
	/*
	 * This is to notify the fault handler of the kprobes.	The
	 * exception code is redundant as it is also carried in REGS,
	 * but we pass it anyhow.
	 */
	if (notify_die(DIE_PAGE_FAULT, "page fault", regs, -1,
		       (regs->cp0_cause >> 2) & 0x1f, SIGSEGV) == NOTIFY_STOP)
		return;
#endif

	info.si_code = SEGV_MAPERR;

	/*
	 * We fault-in kernel-space virtual memory on-demand. The
	 * 'reference' page table is init_mm.pgd.
	 *
	 * NOTE! We MUST NOT take any locks for this case. We may
	 * be in an interrupt or a critical region, and should
	 * only copy the information from the master page table,
	 * nothing more.
	 */
#ifdef CONFIG_64BIT
# define VMALLOC_FAULT_TARGET no_context
#else
# define VMALLOC_FAULT_TARGET vmalloc_fault
#endif

	if (unlikely(address >= VMALLOC_START && address <= VMALLOC_END))
		goto VMALLOC_FAULT_TARGET;
#ifdef MODULE_START
	if (unlikely(address >= MODULE_START && address < MODULE_END))
		goto VMALLOC_FAULT_TARGET;
#endif

	/*
	 * If we're in an interrupt or have no user
	 * context, we must not take the fault..
	 */
	if (in_atomic() || !mm)
		goto bad_area_nosemaphore;

	if (user_mode(regs))
		flags |= FAULT_FLAG_USER;
retry:
	down_read(&mm->mmap_sem);
	vma = find_vma(mm, address);
	if (!vma)
		goto bad_area;
	if (vma->vm_start <= address)
		goto good_area;
	if (!(vma->vm_flags & VM_GROWSDOWN))
		goto bad_area;
	if (expand_stack(vma, address))
		goto bad_area;
/*
 * Ok, we have a good vm_area for this memory access, so
 * we can handle it..
 */
good_area:
	info.si_code = SEGV_ACCERR;

	if (write) {
		if (!(vma->vm_flags & VM_WRITE))
			goto bad_area;
		flags |= FAULT_FLAG_WRITE;
	} else {
		if (cpu_has_rixi) {
			if (address == regs->cp0_epc && !(vma->vm_flags & VM_EXEC)) {
#if 0
				pr_notice("Cpu%d[%s:%d:%0*lx:%ld:%0*lx] XI violation\n",
					  raw_smp_processor_id(),
					  current->comm, current->pid,
					  field, address, write,
					  field, regs->cp0_epc);
#endif
				goto bad_area;
			}
			if (!(vma->vm_flags & VM_READ)) {
#if 0
				pr_notice("Cpu%d[%s:%d:%0*lx:%ld:%0*lx] RI violation\n",
					  raw_smp_processor_id(),
					  current->comm, current->pid,
					  field, address, write,
					  field, regs->cp0_epc);
#endif
				goto bad_area;
			}
		} else {
			if (!(vma->vm_flags & (VM_READ | VM_WRITE | VM_EXEC)))
				goto bad_area;
		}
	}

	/*
	 * If for any reason at all we couldn't handle the fault,
	 * make sure we exit gracefully rather than endlessly redo
	 * the fault.
	 */
	fault = handle_mm_fault(mm, vma, address, flags);

	if ((fault & VM_FAULT_RETRY) && fatal_signal_pending(current))
		return;

	perf_sw_event(PERF_COUNT_SW_PAGE_FAULTS, 1, regs, address);
	if (unlikely(fault & VM_FAULT_ERROR)) {
		if (fault & VM_FAULT_OOM)
			goto out_of_memory;
		else if (fault & VM_FAULT_SIGSEGV)
			goto bad_area;
		else if (fault & VM_FAULT_SIGBUS)
			goto do_sigbus;
		BUG();
	}
	if (flags & FAULT_FLAG_ALLOW_RETRY) {
		if (fault & VM_FAULT_MAJOR) {
			perf_sw_event(PERF_COUNT_SW_PAGE_FAULTS_MAJ, 1,
						  regs, address);
			tsk->maj_flt++;
		} else {
			perf_sw_event(PERF_COUNT_SW_PAGE_FAULTS_MIN, 1,
						  regs, address);
			tsk->min_flt++;
		}
		if (fault & VM_FAULT_RETRY) {
			flags &= ~FAULT_FLAG_ALLOW_RETRY;
			flags |= FAULT_FLAG_TRIED;

			/*
			 * No need to up_read(&mm->mmap_sem) as we would
			 * have already released it in __lock_page_or_retry
			 * in mm/filemap.c.
			 */

			goto retry;
		}
	}

	up_read(&mm->mmap_sem);
	return;

/*
 * Something tried to access memory that isn't in our memory map..
 * Fix it, but check if it's kernel or user first..
 */
bad_area:
	up_read(&mm->mmap_sem);

bad_area_nosemaphore:
	/* User mode accesses just cause a SIGSEGV */
	if (user_mode(regs)) {
		tsk->thread.cp0_badvaddr = address;
		tsk->thread.error_code = write;
		if (show_unhandled_signals &&
		    unhandled_signal(tsk, SIGSEGV) &&
		    __ratelimit(&ratelimit_state)) {
			pr_info("\ndo_page_fault(): sending SIGSEGV to %s for invalid %s %0*lx",
				tsk->comm,
				write ? "write access to" : "read access from",
				field, address);
			pr_info("epc = %0*lx in", field,
				(unsigned long) regs->cp0_epc);
			print_vma_addr(" ", regs->cp0_epc);
			pr_info("ra  = %0*lx in", field,
				(unsigned long) regs->regs[31]);
			print_vma_addr(" ", regs->regs[31]);
			pr_info("\n");
		}
        printbacktrace(regs);		
		info.si_signo = SIGSEGV;
		info.si_errno = 0;
		/* info.si_code has been set above */
		info.si_addr = (void __user *) address;
		force_sig_info(SIGSEGV, &info, tsk);
		return;
	}

no_context:
	/* Are we prepared to handle this kernel fault?	 */
	if (fixup_exception(regs)) {
		current->thread.cp0_baduaddr = address;
		return;
	}

	/*
	 * Oops. The kernel tried to access some bad page. We'll have to
	 * terminate things with extreme prejudice.
	 */
	bust_spinlocks(1);

	printk(KERN_EMERG "CPU %d Unable to handle kernel paging request at "
	       "virtual address %0*lx, epc == %0*lx, ra == %0*lx\n",
	       raw_smp_processor_id(), field, address, field, regs->cp0_epc,
	       field,  regs->regs[31]);
	die("Oops", regs);

out_of_memory:
	/*
	 * We ran out of memory, call the OOM killer, and return the userspace
	 * (which will retry the fault, or kill us if we got oom-killed).
	 */
	up_read(&mm->mmap_sem);
	if (!user_mode(regs))
		goto no_context;
	pagefault_out_of_memory();
	return;

do_sigbus:
	up_read(&mm->mmap_sem);

	/* Kernel mode? Handle exceptions or die */
	if (!user_mode(regs))
		goto no_context;
	else
	/*
	 * Send a sigbus, regardless of whether we were in kernel
	 * or user mode.
	 */
#if 1
		printk(KERN_EMERG  "do_page_fault() #3: sending SIGBUS to %s for "
		       "invalid %s\n%0*lx (epc == %0*lx, ra == %0*lx)\n",
		       tsk->comm,
		       write ? "write access to" : "read access from",
		       field, address,
		       field, (unsigned long) regs->cp0_epc,
		       field, (unsigned long) regs->regs[31]);
#endif
    printbacktrace(regs);
	tsk->thread.cp0_badvaddr = address;
	info.si_signo = SIGBUS;
	info.si_errno = 0;
	info.si_code = BUS_ADRERR;
	info.si_addr = (void __user *) address;
	force_sig_info(SIGBUS, &info, tsk);

	return;
#ifndef CONFIG_64BIT
vmalloc_fault:
	{
		/*
		 * Synchronize this task's top level page-table
		 * with the 'reference' page table.
		 *
		 * Do _not_ use "tsk" here. We might be inside
		 * an interrupt in the middle of a task switch..
		 */
		int offset = __pgd_offset(address);
		pgd_t *pgd, *pgd_k;
		pud_t *pud, *pud_k;
		pmd_t *pmd, *pmd_k;
		pte_t *pte_k;

		pgd = (pgd_t *) pgd_current[raw_smp_processor_id()] + offset;
		pgd_k = init_mm.pgd + offset;

		if (!pgd_present(*pgd_k))
			goto no_context;
		set_pgd(pgd, *pgd_k);

		pud = pud_offset(pgd, address);
		pud_k = pud_offset(pgd_k, address);
		if (!pud_present(*pud_k))
			goto no_context;

		pmd = pmd_offset(pud, address);
		pmd_k = pmd_offset(pud_k, address);
		if (!pmd_present(*pmd_k))
			goto no_context;
		set_pmd(pmd, *pmd_k);

		pte_k = pte_offset_kernel(pmd_k, address);
		if (!pte_present(*pte_k))
			goto no_context;
		return;
	}
#endif
}

asmlinkage void __kprobes do_page_fault(struct pt_regs *regs,
	unsigned long write, unsigned long address)
{
	enum ctx_state prev_state;

	prev_state = exception_enter();
	__do_page_fault(regs, write, address);
	exception_exit(prev_state);
}
