/*
 *  linux/arch/arm/mm/context.c
 *
 *  Copyright (C) 2002-2003 Deep Blue Solutions Ltd, all rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/smp.h>
#include <linux/percpu.h>

#include <asm/mmu_context.h>
#if defined(CONFIG_BCM_KF_ARM_BCM963XX) && defined(CONFIG_BCM_KF_ARM_ERRATA_798181)
#include <asm/smp_plat.h>
#include <asm/thread_notify.h>
#endif
#include <asm/tlbflush.h>
#if defined(CONFIG_BCM_KF_ARM_BCM963XX) && defined(CONFIG_BCM_KF_ARM_ERRATA_798181)
#include <asm/proc-fns.h>

/*
 * On ARMv6, we have the following structure in the Context ID:
 *
 * 31                         7          0
 * +-------------------------+-----------+
 * |      process ID         |   ASID    |
 * +-------------------------+-----------+
 * |              context ID             |
 * +-------------------------------------+
 *
 * The ASID is used to tag entries in the CPU caches and TLBs.
 * The context ID is used by debuggers and trace logic, and
 * should be unique within all running processes.
 *
 * In big endian operation, the two 32 bit words are swapped if accesed by
 * non 64-bit operations.
 */
#define ASID_FIRST_VERSION	(1ULL << ASID_BITS)
#define NUM_USER_ASIDS		ASID_FIRST_VERSION

static DEFINE_RAW_SPINLOCK(cpu_asid_lock);
static atomic64_t asid_generation = ATOMIC64_INIT(ASID_FIRST_VERSION);
static DECLARE_BITMAP(asid_map, NUM_USER_ASIDS);

static DEFINE_PER_CPU(atomic64_t, active_asids);
static DEFINE_PER_CPU(u64, reserved_asids);
static cpumask_t tlb_flush_pending;

#ifdef CONFIG_ARM_ERRATA_798181
void a15_erratum_get_cpumask(int this_cpu, struct mm_struct *mm,
			     cpumask_t *mask)
{
	int cpu;
	unsigned long flags;
	u64 context_id, asid;

	raw_spin_lock_irqsave(&cpu_asid_lock, flags);
	context_id = mm->context.id.counter;
	for_each_online_cpu(cpu) {
		if (cpu == this_cpu)
			continue;
		/*
		 * We only need to send an IPI if the other CPUs are
		 * running the same ASID as the one being invalidated.
		 */
		asid = per_cpu(active_asids, cpu).counter;
		if (asid == 0)
			asid = per_cpu(reserved_asids, cpu);
		if (context_id == asid)
			cpumask_set_cpu(cpu, mask);
	}
	raw_spin_unlock_irqrestore(&cpu_asid_lock, flags);
}
#endif
#else

static DEFINE_RAW_SPINLOCK(cpu_asid_lock);
unsigned int cpu_last_asid = ASID_FIRST_VERSION;
#ifdef CONFIG_SMP
DEFINE_PER_CPU(struct mm_struct *, current_mm);
#endif
#endif

#if defined(CONFIG_BCM_KF_ARM_BCM963XX) && defined(CONFIG_BCM_KF_ARM_ERRATA_798181)
#ifdef CONFIG_ARM_LPAE
static void cpu_set_reserved_ttbr0(void)
{
	/*
	 * Set TTBR0 to swapper_pg_dir which contains only global entries. The
	 * ASID is set to 0.
	 */
	cpu_set_ttbr(0, __pa(swapper_pg_dir));
	isb();
}
#else
static void cpu_set_reserved_ttbr0(void)
{
	u32 ttb;
	/* Copy TTBR1 into TTBR0 */
	asm volatile(
	"	mrc	p15, 0, %0, c2, c0, 1		@ read TTBR1\n"
	"	mcr	p15, 0, %0, c2, c0, 0		@ set TTBR0\n"
	: "=r" (ttb));
	isb();
}
#endif
#else
#ifdef CONFIG_ARM_LPAE
#define cpu_set_asid(asid) {						\
	unsigned long ttbl, ttbh;					\
	asm volatile(							\
	"	mrrc	p15, 0, %0, %1, c2		@ read TTBR0\n"	\
	"	mov	%1, %2, lsl #(48 - 32)		@ set ASID\n"	\
	"	mcrr	p15, 0, %0, %1, c2		@ set TTBR0\n"	\
	: "=&r" (ttbl), "=&r" (ttbh)					\
	: "r" (asid & ~ASID_MASK));					\
}
#else
#define cpu_set_asid(asid) \
	asm("	mcr	p15, 0, %0, c13, c0, 1\n" : : "r" (asid))
#endif
#endif

#if defined(CONFIG_BCM_KF_ARM_BCM963XX) && defined(CONFIG_BCM_KF_ARM_ERRATA_798181)
#ifdef CONFIG_PID_IN_CONTEXTIDR
static int contextidr_notifier(struct notifier_block *unused, unsigned long cmd,
			       void *t)
{
	u32 contextidr;
	pid_t pid;
	struct thread_info *thread = t;

	if (cmd != THREAD_NOTIFY_SWITCH)
		return NOTIFY_DONE;

	pid = task_pid_nr(thread->task) << ASID_BITS;
	asm volatile(
	"	mrc	p15, 0, %0, c13, c0, 1\n"
	"	and	%0, %0, %2\n"
	"	orr	%0, %0, %1\n"
	"	mcr	p15, 0, %0, c13, c0, 1\n"
	: "=r" (contextidr), "+r" (pid)
	: "I" (~ASID_MASK));
	isb();

	return NOTIFY_OK;
}

static struct notifier_block contextidr_notifier_block = {
	.notifier_call = contextidr_notifier,
};

static int __init contextidr_notifier_init(void)
{
	return thread_register_notifier(&contextidr_notifier_block);
}
arch_initcall(contextidr_notifier_init);
#endif

static void flush_context(unsigned int cpu)
{
	int i;
	u64 asid;

	/* Update the list of reserved ASIDs and the ASID bitmap. */
	bitmap_clear(asid_map, 0, NUM_USER_ASIDS);
	for_each_possible_cpu(i) {
		if (i == cpu) {
			asid = 0;
		} else {
			asid = atomic64_xchg(&per_cpu(active_asids, i), 0);
			/*
			 * If this CPU has already been through a
			 * rollover, but hasn't run another task in
			 * the meantime, we must preserve its reserved
			 * ASID, as this is the only trace we have of
			 * the process it is still running.
			 */
			if (asid == 0)
				asid = per_cpu(reserved_asids, i);
			__set_bit(asid & ~ASID_MASK, asid_map);
		}
		per_cpu(reserved_asids, i) = asid;
	}

	/* Queue a TLB invalidate and flush the I-cache if necessary. */
	cpumask_setall(&tlb_flush_pending);

	if (icache_is_vivt_asid_tagged())
		__flush_icache_all();
}

static int is_reserved_asid(u64 asid)
{
	int cpu;
	for_each_possible_cpu(cpu)
		if (per_cpu(reserved_asids, cpu) == asid)
			return 1;
	return 0;
}

static u64 new_context(struct mm_struct *mm, unsigned int cpu)
{
	u64 asid = atomic64_read(&mm->context.id);
	u64 generation = atomic64_read(&asid_generation);

	if (asid != 0 && is_reserved_asid(asid)) {
		/*
		 * Our current ASID was active during a rollover, we can
		 * continue to use it and this was just a false alarm.
		 */
		asid = generation | (asid & ~ASID_MASK);
	} else {
		/*
		 * Allocate a free ASID. If we can't find one, take a
		 * note of the currently active ASIDs and mark the TLBs
		 * as requiring flushes. We always count from ASID #1,
		 * as we reserve ASID #0 to switch via TTBR0 and indicate
		 * rollover events.
		 */
		asid = find_next_zero_bit(asid_map, NUM_USER_ASIDS, 1);
		if (asid == NUM_USER_ASIDS) {
			generation = atomic64_add_return(ASID_FIRST_VERSION,
							 &asid_generation);
			flush_context(cpu);
			asid = find_next_zero_bit(asid_map, NUM_USER_ASIDS, 1);
		}
		__set_bit(asid, asid_map);
		asid |= generation;
		cpumask_clear(mm_cpumask(mm));
	}

	return asid;
}

void check_and_switch_context(struct mm_struct *mm, struct task_struct *tsk)
{
	unsigned long flags;
	unsigned int cpu = smp_processor_id();
	u64 asid;

	if (unlikely(mm->context.kvm_seq != init_mm.context.kvm_seq))
		__check_kvm_seq(mm);

	/*
	 * Required during context switch to avoid speculative page table
	 * walking with the wrong TTBR.
	 */
	cpu_set_reserved_ttbr0();

	asid = atomic64_read(&mm->context.id);
	if (!((asid ^ atomic64_read(&asid_generation)) >> ASID_BITS)
	    && atomic64_xchg(&per_cpu(active_asids, cpu), asid))
		goto switch_mm_fastpath;

	raw_spin_lock_irqsave(&cpu_asid_lock, flags);
	/* Check that our ASID belongs to the current generation. */
	asid = atomic64_read(&mm->context.id);
	if ((asid ^ atomic64_read(&asid_generation)) >> ASID_BITS) {
		asid = new_context(mm, cpu);
		atomic64_set(&mm->context.id, asid);
	}

	if (cpumask_test_and_clear_cpu(cpu, &tlb_flush_pending)) {
		local_flush_bp_all();
		local_flush_tlb_all();
		/* both 3.10.15 and 3.11.4 have the following line, but
		 * 3.12.13 doesn't.  I will comment it out for now */
		//erratum_a15_798181();
	}

	atomic64_set(&per_cpu(active_asids, cpu), asid);
	cpumask_set_cpu(cpu, mm_cpumask(mm));
	raw_spin_unlock_irqrestore(&cpu_asid_lock, flags);

switch_mm_fastpath:
	cpu_switch_mm(mm->pgd, mm);
}
#else
/*
 * We fork()ed a process, and we need a new context for the child
 * to run in.  We reserve version 0 for initial tasks so we will
 * always allocate an ASID. The ASID 0 is reserved for the TTBR
 * register changing sequence.
 */
void __init_new_context(struct task_struct *tsk, struct mm_struct *mm)
{
	mm->context.id = 0;
	raw_spin_lock_init(&mm->context.id_lock);
}

static void flush_context(void)
{
	/* set the reserved ASID before flushing the TLB */
	cpu_set_asid(0);
	isb();
	local_flush_tlb_all();
	if (icache_is_vivt_asid_tagged()) {
		__flush_icache_all();
		dsb();
	}
}

#ifdef CONFIG_SMP

static void set_mm_context(struct mm_struct *mm, unsigned int asid)
{
	unsigned long flags;

	/*
	 * Locking needed for multi-threaded applications where the
	 * same mm->context.id could be set from different CPUs during
	 * the broadcast. This function is also called via IPI so the
	 * mm->context.id_lock has to be IRQ-safe.
	 */
	raw_spin_lock_irqsave(&mm->context.id_lock, flags);
	if (likely((mm->context.id ^ cpu_last_asid) >> ASID_BITS)) {
		/*
		 * Old version of ASID found. Set the new one and
		 * reset mm_cpumask(mm).
		 */
		mm->context.id = asid;
		cpumask_clear(mm_cpumask(mm));
	}
	raw_spin_unlock_irqrestore(&mm->context.id_lock, flags);

	/*
	 * Set the mm_cpumask(mm) bit for the current CPU.
	 */
	cpumask_set_cpu(smp_processor_id(), mm_cpumask(mm));
}

/*
 * Reset the ASID on the current CPU. This function call is broadcast
 * from the CPU handling the ASID rollover and holding cpu_asid_lock.
 */
static void reset_context(void *info)
{
	unsigned int asid;
	unsigned int cpu = smp_processor_id();
	struct mm_struct *mm = per_cpu(current_mm, cpu);

	/*
	 * Check if a current_mm was set on this CPU as it might still
	 * be in the early booting stages and using the reserved ASID.
	 */
	if (!mm)
		return;

	smp_rmb();
	asid = cpu_last_asid + cpu + 1;

	flush_context();
	set_mm_context(mm, asid);

	/* set the new ASID */
	cpu_set_asid(mm->context.id);
	isb();
}

#else

static inline void set_mm_context(struct mm_struct *mm, unsigned int asid)
{
	mm->context.id = asid;
	cpumask_copy(mm_cpumask(mm), cpumask_of(smp_processor_id()));
}

#endif

void __new_context(struct mm_struct *mm)
{
	unsigned int asid;

	raw_spin_lock(&cpu_asid_lock);
#ifdef CONFIG_SMP
	/*
	 * Check the ASID again, in case the change was broadcast from
	 * another CPU before we acquired the lock.
	 */
	if (unlikely(((mm->context.id ^ cpu_last_asid) >> ASID_BITS) == 0)) {
		cpumask_set_cpu(smp_processor_id(), mm_cpumask(mm));
		raw_spin_unlock(&cpu_asid_lock);
		return;
	}
#endif
	/*
	 * At this point, it is guaranteed that the current mm (with
	 * an old ASID) isn't active on any other CPU since the ASIDs
	 * are changed simultaneously via IPI.
	 */
	asid = ++cpu_last_asid;
	if (asid == 0)
		asid = cpu_last_asid = ASID_FIRST_VERSION;

	/*
	 * If we've used up all our ASIDs, we need
	 * to start a new version and flush the TLB.
	 */
	if (unlikely((asid & ~ASID_MASK) == 0)) {
		asid = cpu_last_asid + smp_processor_id() + 1;
		flush_context();
#ifdef CONFIG_SMP
		smp_wmb();
		smp_call_function(reset_context, NULL, 1);
#endif
		cpu_last_asid += NR_CPUS;
	}

	set_mm_context(mm, asid);
	raw_spin_unlock(&cpu_asid_lock);
}
#endif
