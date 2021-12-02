#ifndef __ASM_ARM_SUSPEND_H
#define __ASM_ARM_SUSPEND_H

static inline int arch_prepare_suspend(void) { return 0; }

/* image of the saved processor state */
struct saved_context {
	/*
	 * Structure saved_context would be used to hold processor state
	 * except caller and callee registers, just before suspending.
	 */

	/* coprocessor 15 registers */
	__u32 CR;
	__u32 ACR;
	__u32 TTBR0;
	__u32 TTBR1;
	__u32 TTBCR;
	__u32 DACR;
	__u32 TLBLR;
	__u32 FCSE;
	__u32 CID;
	__u32 EFR;
};

#endif /* __ASM_ARM_SUSPEND_H */
