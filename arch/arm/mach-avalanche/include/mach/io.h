/*
 * linux/include/asm-arm/arch-shark/io.h
 *
 * by Alexander Schulz
 *
 * derived from:
 * linux/include/asm-arm/arch-ebsa110/io.h
 * Copyright (C) 1997,1998 Russell King
 */

/* Copyright 2008, Texas Instruments Incorporated
 *
 * This program has been modified from its original operation by Texas Instruments
 * to do the following:
 * Explanation of modification.
 *  avalanche io changes
 *  
 *
 * THIS MODIFIED SOFTWARE AND DOCUMENTATION ARE PROVIDED
 * "AS IS," AND TEXAS INSTRUMENTS MAKES NO REPRESENTATIONS
 * OR WARRENTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO, WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY
 * PARTICULAR PURPOSE OR THAT THE USE OF THE SOFTWARE OR
 * DOCUMENTATION WILL NOT INFRINGE ANY THIRD PARTY PATENTS,
 * COPYRIGHTS, TRADEMARKS OR OTHER RIGHTS.
 * See The GNU General Public License for more details.
 *
 * These changes are covered under version 2 of the GNU General Public License,
 * dated June 1991.
 */


#ifndef __ASM_ARM_ARCH_IO_H
#define __ASM_ARM_ARCH_IO_H
#include <mach/hardware.h>
#define IO_SPACE_LIMIT 0xffffffff


/*
 * Dynamic IO functions - let the compiler
 * optimize the expressions
 */
#define DECLARE_DYN_OUT(fnsuffix,instr)						\
static inline void __out##fnsuffix (unsigned int value, unsigned int port)	\
{										\
	__asm__ __volatile__(							\
	"str" instr "	%0, [%2, %1]	@ out" #fnsuffix			\
	: 								\
	: "r" (value), "r" (port), "r" (IO_BASE)	\
	: "cc");								\
}

#define DECLARE_DYN_IN(sz,fnsuffix,instr)					\
static inline unsigned sz __in##fnsuffix (unsigned int port)		\
{										\
	unsigned long value;						\
	__asm__ __volatile__(							\
	"ldr" instr "	%0, [%2, %1]	@ in" #fnsuffix				\
	: "=r" (value)						\
	: "r" (port), "r" (IO_BASE)		\
	: "cc");								\
	return (unsigned sz)value;						\
}

static inline unsigned int __ioaddr (unsigned int port)			\
{										\
		return (unsigned int)(IO_BASE + (port));			\
}

#define DECLARE_IO(sz,fnsuffix,instr)	\
	DECLARE_DYN_OUT(fnsuffix,instr)	\
	DECLARE_DYN_IN(sz,fnsuffix,instr)

DECLARE_IO(char,b,"b")
DECLARE_IO(short,w,"h")
DECLARE_IO(long,l,"")

#undef DECLARE_IO
#undef DECLARE_DYN_OUT
#undef DECLARE_DYN_IN

/*
 * Constant address IO functions
 *
 * These have to be macros for the 'J' constraint to work -
 * +/-4096 immediate operand.
 */
#define __outbc(value,port)							\
({										\
	__asm__ __volatile__(						\
	"strb	%0, [%1, %2]		@ outbc"			\
	: : "r" (value), "r" (IO_BASE), "r" (port));		\
})

#define __inbc(port)								\
({										\
	unsigned char result;                                                   \
	__asm__ __volatile__(						\
	"ldrb	%0, [%1, %2]		@ inbc"				\
	: "=r" (result) : "r" (IO_BASE), "r" (port));		\
	result;									\
})

#define __outwc(value,port)							\
({										\
	unsigned long v = value;						\
	__asm__ __volatile__(						\
	"strh	%0, [%1, %2]		@ outwc"			\
	: : "r" (v|v<<16), "r" (IO_BASE), "r" (port));		\
})

#define __inwc(port)								\
({										\
	unsigned short result;							\
	__asm__ __volatile__(						\
	"ldrh	%0, [%1, %2]		@ inwc"				\
	: "=r" (result) : "r" (IO_BASE), "r" (port));		\
	result & 0xffff;							\
})

#define __outlc(value,port)								\
({										\
	unsigned long v = value;						\
	__asm__ __volatile__(						\
	"str	%0, [%1, %2]		@ outlc"			\
	: : "r" (v), "r" (IO_BASE), "r" (port));			\
})

#define __inlc(port)								\
({										\
	unsigned long result;							\
	__asm__ __volatile__(						\
	"ldr	%0, [%1, %2]		@ inlc"				\
	: "=r" (result) : "r" (IO_BASE), "r" (port));		\
	result;									\
})

#define __ioaddrc(port)								\
({										\
	unsigned long addr;							\
	addr = IO_BASE + (port);					\
	addr;									\
})

#define __mem_pci(addr) (addr)

#if 0
#define inb(p)	 	(__builtin_constant_p((p)) ? __inbc(p)    : __inb(p))
#define inw(p)	 	(__builtin_constant_p((p)) ? __inwc(p)    : __inw(p))
#define inl(p)	 	(__builtin_constant_p((p)) ? __inlc(p)    : __inl(p))
#define outb(v,p)	(__builtin_constant_p((p)) ? __outbc(v,p) : __outb(v,p))
#define outw(v,p)	(__builtin_constant_p((p)) ? __outwc(v,p) : __outw(v,p))
#define outl(v,p)	(__builtin_constant_p((p)) ? __outlc(v,p) : __outl(v,p))

/*
 * Translated address IO functions
 *
 * IO address has already been translated to a virtual address
 */
#define outb_t(v,p)								\
	(*(volatile unsigned char *)(p) = (v))

#define inb_t(p)								\
	(*(volatile unsigned char *)(p))

#define outl_t(v,p)								\
	(*(volatile unsigned long *)(p) = (v))

#define inl_t(p)								\
	(*(volatile unsigned long *)(p))

#endif
#define __io(a)  ((void __iomem *)(a))
#endif
