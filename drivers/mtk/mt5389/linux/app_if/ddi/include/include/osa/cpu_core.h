/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
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
extern	unsigned int	_start[];
extern	unsigned int	_data_start[];
#define __START			(unsigned int)_start
#define __DSTART		(unsigned int)_data_start

#define CPU_REG_CNT		20

typedef unsigned long	cpu_reg_t;
typedef	cpu_reg_t		cpu_regs_t[CPU_REG_CNT];
typedef struct _regs_set_
{
	unsigned long	pc;
	cpu_regs_t		regs;
} CPU_REGS_T;

#ifdef	__GNUC__
#define get_lr(_x) 	{ __asm__ ("movl %%esp, (%0)" : /* no output */ : "r" (_x)); }
#define get_ra(_x) 	{ __asm__ ("movl %%esp, (%0)" : /* no output */ : "r" (_x)); }
#define get_sp(_x) 	{ __asm__ ("movl %%esp, (%0)" : /* no output */ : "r" (_x)); }

#define str_lr(_x) 	{ *(UINT32*)_x = (UINT32)x86_getra();                        }
#define str_ra(_x) 	{ *(UINT32*)_x = (UINT32)x86_getra();                        }
#define str_sp(_x) 	{ __asm__ ("movl %%ebp, (%0)" : /* no output */ : "r" (_x)); }
#define str_pc(_x)	{ *(UINT32*)_x = (UINT32)x86_getpc();                        }

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

extern UINT32* x86_getpc(void);
extern UINT32* x86_getra(void);
#endif

#endif

#ifdef __MIPS__

extern	unsigned int	__start[];
extern	unsigned int	__data_start[];
#define __START			(unsigned int)__start
#define __DSTART		(unsigned int)__data_start

#define CPU_REG_CNT		32

typedef unsigned long	cpu_reg_t;
typedef	cpu_reg_t		cpu_regs_t[CPU_REG_CNT];
typedef struct _regs_set_
{
	unsigned long	pc;
	cpu_regs_t		regs;
} CPU_REGS_T;

#ifdef	__GNUC__
#define get_lr(_x) 	{ __asm__ ("LW $31, 0(%0)" : /* no output */ :"r" (_x)); }
#define get_ra(_x) 	{ __asm__ ("LW $31, 0(%0)" : /* no output */ :"r" (_x)); }
#define get_sp(_x) 	{ __asm__ ("LW $29, 0(%0)" : /* no output */ :"r" (_x)); }

#define str_lr(_x) 	{ __asm__ ("SW $31, 0(%0)" : /* no output */ :"r" (_x)); }
#define str_ra(_x) 	{ __asm__ ("SW $31, 0(%0)" : /* no output */ :"r" (_x)); }
#define str_sp(_x) 	{ __asm__ ("SW $29, 0(%0)" : /* no output */ :"r" (_x)); }
#define str_pc(_x)	{ *_x = mips_getpc();                                    }

#define _REG_PC		 0
#define _REG_RA		31
#define _REG_SP		29

extern UINT32*	mips_getpc(void);
#endif

#endif

#ifdef __ARM__
extern	unsigned int	_start[];
/* [L8] raxis.lim (2010/04/02) - move _data_start[] to data_start[] */
extern	unsigned int	data_start[];
#define __START			(unsigned int)_start
#define __DSTART		(unsigned int)data_start

#define CPU_REG_CNT		16

typedef unsigned long	cpu_reg_t;
typedef	cpu_reg_t		cpu_regs_t[CPU_REG_CNT];
typedef struct _regs_set_
{
	unsigned long	pc;
	cpu_regs_t		regs;
} CPU_REGS_T;

#ifdef	__GNUC__
#define get_lr(_x) 	{ __asm__ ("MOV %0, LR"   : /* no output */ :"r" (_x)); }
#define get_ra(_x) 	{ __asm__ ("MOV %0, LR"   : /* no output */ :"r" (_x)); }
#define get_sp(_x) 	{ __asm__ ("MOV %0, SP"   : /* no output */ :"r" (_x)); }
#define get_pc(_x) 	{ __asm__ ("MOV %0, PC"   : /* no output */ :"r" (_x)); }

/* [L8] seokjoo.lee (2010/05/23) GCC 4.x.x complains uninitialized->added clobberlist:"memory" */
#define str_lr(_x) 	{ __asm__ __volatile__("STR LR, [%0]" : /* no output */ :"r" (_x):"memory"); }
#define str_ra(_x) 	{ __asm__ __volatile__("STR LR, [%0]" : /* no output */ :"r" (_x):"memory"); }
#define str_sp(_x) 	{ __asm__ __volatile__("STR SP, [%0]" : /* no output */ :"r" (_x):"memory"); }
#define str_pc(_x) 	{ __asm__ __volatile__("STR PC, [%0]" : /* no output */ :"r" (_x):"memory"); }

#define _REG_PC		15
#define _REG_RA		14
#define _REG_SP		13

#endif

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
