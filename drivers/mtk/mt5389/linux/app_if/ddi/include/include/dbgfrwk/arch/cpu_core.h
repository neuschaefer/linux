#ifndef _CPU_CORE_H_
#define _CPU_CORE_H_

#ifdef __cplusplus
extern "C" {
#endif


/*---------------------------------------------------------------------*/
/* Some macro function to access byte, word, long words                */
/*---------------------------------------------------------------------*/
#define    outb(x,y)     (*((volatile unsigned char  *)(x))  = (y))
#define    outw(x,y)     (*((volatile unsigned short *)(x))  = (y))
#define    outl(x,y)     (*((volatile unsigned long  *)(x))  = (y))
#define    outl_OR(x,y)  (*((volatile unsigned long  *)(x)) |= (y))
#define    outl_AND(x,y) (*((volatile unsigned long  *)(x)) &= (y))

#define    inb(x)        (*((volatile unsigned char  *)(x)))
#define    inw(x)        (*((volatile unsigned short *)(x)))
#define    inl(x)        (*((volatile unsigned long  *)(x)))

/*---------------------------------------------------------------------*/
/* Some macro function to read some registers                          */
/*---------------------------------------------------------------------*/
#ifdef __X86__
#define __START			(unsigned int)_start
#define __DSTART		(unsigned int)_data_start

#define CPU_REG_CNT		20

typedef unsigned long	cpu_reg_t;
typedef	 cpu_reg_t		cpu_regs_t[CPU_REG_CNT];
typedef struct _regs_set_
{
	unsigned long	pc;
	cpu_regs_t		regs;
} cpu_regs_set_t;

#ifdef	__GNUC__
#define get_lr(_x) 	{ __asm__ ("movl %%esp, (%0)" : /* no output */ : "r" (_x)); }
#define get_ra(_x) 	{ __asm__ ("movl %%esp, (%0)" : /* no output */ : "r" (_x)); }
#define get_sp(_x) 	{ __asm__ ("movl %%esp, (%0)" : /* no output */ : "r" (_x)); }

#define str_lr(_x) 	{ *(unsigned int*)_x = (unsigned int)x86_getra();                        }
#define str_ra(_x) 	{ *(unsigned int*)_x = (unsigned int)x86_getra();                        }
#define str_sp(_x) 	{ __asm__ ("movl %%ebp, (%0)" : /* no output */ : "r" (_x)); }
#define str_pc(_x)	{ *(unsigned int*)_x = (unsigned int)x86_getpc();                        }

#define _REG_GS		 0
#define _REG_FS		 1
#define _REG_ES		 2
#define _REG_DS		 3
#define _REG_EDI	 4
#define _REG_ESI	 5
#define _REG_EBP	 6
#define _REG_ESP	 7
#define _REG_EBX	 8
#define _REG_EDX	 9
#define _REG_ECX	10
#define _REG_EAX	11
#define _REG_TRAP	12
#define _REG_ERR	13
#define _REG_EIP	14
#define _REG_CS		15
#define _REG_EFL	16
#define _REG_UESP	17
#define _REG_SS		18
#define _REG_NONE	19

#define _REG_PC		(_REG_EIP)
#define _REG_RA		(_REG_NONE)
#define _REG_SP		(_REG_EBP)

#endif

#endif

#ifdef __MIPS__

#define __START			(unsigned int)__start
#define __DSTART		(unsigned int)__data_start

#define CPU_REG_CNT		32

typedef unsigned long	cpu_reg_t;
typedef	cpu_reg_t		cpu_regs_t[CPU_REG_CNT];
typedef struct _regs_set_
{
	unsigned long	pc;
	cpu_regs_t		regs;
} cpu_regs_set_t;

#ifdef	__GNUC__
#define get_lr(_x) 	{ __asm__ ("LW $31, 0(%0)" : /* no output */ :"r" (_x)); }
#define get_ra(_x) 	{ __asm__ ("LW $31, 0(%0)" : /* no output */ :"r" (_x)); }
#define get_sp(_x) 	{ __asm__ ("LW $29, 0(%0)" : /* no output */ :"r" (_x)); }

#define str_lr(_x) 	{ __asm__ ("SW $31, 0(%0)" : /* no output */ :"r" (_x)); }
#define str_ra(_x) 	{ __asm__ ("SW $31, 0(%0)" : /* no output */ :"r" (_x)); }
#define str_sp(_x) 	{ __asm__ ("SW $29, 0(%0)" : /* no output */ :"r" (_x)); }
#define str_pc(_x)	{ *_x = (unsigned int)mips_getpc();                                    }

#define _REG_PC		 0
#define _REG_RA		31
#define _REG_SP		29

#endif

#endif

#ifdef __ARM__
#define __START			(unsigned int)_start
#define __DSTART		(unsigned int)data_start

#define CPU_REG_CNT		16

typedef unsigned long	cpu_reg_t;
typedef	 cpu_reg_t		cpu_regs_t[CPU_REG_CNT];
typedef struct _regs_set_
{
	unsigned long	pc;
	cpu_regs_t		regs;
} cpu_regs_set_t;

#ifdef	__GNUC__
#define get_lr(_x) 	{ __asm__ ("MOV %0, LR"   : /* no output */ :"r" (_x)); }
#define get_ra(_x) 	{ __asm__ ("MOV %0, LR"   : /* no output */ :"r" (_x)); }
#define get_sp(_x) 	{ __asm__ ("MOV %0, SP"   : /* no output */ :"r" (_x)); }
#define get_pc(_x) 	{ __asm__ ("MOV %0, PC"   : /* no output */ :"r" (_x)); }

/* [L8] seokjoo.lee (2010/05/23) GCC 4.x.x complains uninitialized->added clobberlist:"memory" */
#define str_lr(_x)  { __asm__ __volatile__("STR LR, [%0]" : /* no output */ :"r" (_x):"memory"); }
#define str_ra(_x)  { __asm__ __volatile__("STR LR, [%0]" : /* no output */ :"r" (_x):"memory"); }
#define str_sp(_x)  { __asm__ __volatile__("STR SP, [%0]" : /* no output */ :"r" (_x):"memory"); }
#define str_pc(_x)  { __asm__ __volatile__("STR PC, [%0]" : /* no output */ :"r" (_x):"memory"); }


#define _REG_PC		15
#define _REG_RA		14
#define _REG_SP		13

#endif

#define _SCODE		__START
#define _ECODE		__DSTART

#define	IS_VALID_PC(x) ( ((unsigned int)_SCODE <= (unsigned int)(x))		&&	\
						 ((unsigned int)_ECODE >= (unsigned int)(x))		&&	\
						 (0       == (0x3 & (unsigned int)(x)))		)

#endif

#ifdef _EMUL_WIN
//#define CPU_REG_CNT				16

#define get_lr(_x) 	{}
#define get_ra(_x) 	{}
#define get_sp(_x) 	{}
#define get_pc(_x) 	{}

#define str_lr(_x) 	{}
#define str_ra(_x) 	{}
#define str_sp(_x) 	{}
#define str_pc(_x) 	{}

#define _REG_PC		15
#define _REG_RA		14
#define _REG_SP		13


#endif

#ifdef __cplusplus
}
#endif

#endif /* _CPU_CORE_H_ */
