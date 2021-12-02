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
#include "arm_cpu.h"
/*
;*************************************************************************
;*   External vector entry functions
;*************************************************************************
*/
    .global INT_Reset
    .global INT_Undef_Inst
    .global INT_Software
    .global INT_Prefetch_Abort
    .global INT_Data_Abort
    .global INT_Reserved
    .global INT_IRQ
    .global INT_FIQ
    .global MsgCMain
    .global ARM_SendEvent

@ Debug messages while booting

MsgCMain:
#ifndef CC_MTK_PRELOADER
    .asciz     "\n\rStart Lmain\n\r"
#else
    .asciz     "\n\rStart Pmain\n\r"
#endif
    .align

MsgHalting:
    .asciz     "\n\rHlt\n\r"
    .align

/*
;*************************************************************************
;*  Exception handlers
;*************************************************************************
*/
INT_Undef_Inst:
    B       INT_Exception

INT_Software:
    B       INT_Exception

INT_Prefetch_Abort:
    B       INT_Exception

INT_Data_Abort:
    B       INT_Exception

INT_Reserved:
    B       INT_Exception

INT_IRQ:
    B       INT_Exception

INT_FIQ:
    B       INT_Exception

INT_Exception:
    MRC     p15, 0, r1, c0, c0, 5
    LDR     r0, =0x80000001 @ Check if dual core
    CMP     r0, r1
    BNE     INT_Exception
    LDR     r1, =0xfb00bf04
    BLX     r1

/*
;*************************************************************************
;*  Reset handler
;*************************************************************************
*/
INT_Reset:

    @ Set to supervisor mode and disable interrupts
    MOV     r1, #(MODE_SVC | BIT_I | BIT_F)
    MSR     CPSR_c, r1

    @ Disable caches and MMU
    MRC     p15, 0, r0, c1, c0, 0       @ Read CP15 control register
    BIC     r0, r0, #CR_ICACHE
    BIC     r0, r0, #CR_DCACHE
    BIC     r0, r0, #CR_MMU
    ORR     r0, r0, #CR_BPRDCT
    MCR     p15, 0, r0, c1, c0, 0       @ Write back control register

@LOOP:
    @B       LOOP

    @ let core1 sleep
    MRC     p15, 0, r1, c0, c0, 5
    LDR     r0, =0x80000001 @ Check if dual core
    CMP     r0, r1
    BNE     CORE_0

CORE_1:
    LDR     r0, =0xf00170c0             @ Enable DMX SRAM
    LDR     r1, [r0]
    LDR     r2, =0x4000000
    ORR     r1, r1, r2
    STR     r1, [r0]
    ISB
    DSB
    LDR     r0, =SRAM_CORE1_BASE        @ write WFE to the lask 256bytes of DMX SRAM
    LDR     r1, =0xe320f002             @ WFE instruction
    STR     r1, [r0]
    ADD     r0,r0,#4
    LDR     r1, =0xf57ff06f             @ ISB
    STR     r1, [r0]
    @ADD     r0,r0,#4
    @LDR     r1, =0xf57ff04f             @ DSB
    @STR     r1, [r0]
    @ADD     r0,r0,#4
    @LDR     r1, =0xf57ff05f             @ DMB
    @STR     r1, [r0]
    LDR     r0, =SRAM_CORE1_BASE        @ write WFE to the lask 256bytes of DMX SRAM
    DSB                                 @ to assure WFE is in SRAM
    BX      r0

CORE_0:
    @ disable L2C
    LDR     r0, =0xf2000100
    LDR     r1, =0x0
    STR     r1, [r0]

    @ Enable I-Cache
    BL      ARM_EnableICache

VFP_ENABLE:
    MRC     p15, 0, r0, c1, c0, 2       @ Read CP15 control register
    ORR     r0, r0, #CR_CP1011_PRI_ONLY   @ Enable CP10 ACCESS
    MCR     p15, 0, r0, c1, c0, 2       @ Write back control register
    MOV     r0, #0
    MCR     p15, 0, r0, c7, c5, 4       @ISB
    MOV     r0,#0x40000000              @ VFP enable bit
    FMXR    FPEXC,r0


NoRemapPPort:
    BL      mt53xx_SRAMSetup

    @ Query SRAM stack pointer address, and pass to ARM_StackITCM by r2.
    BL      CHIP_GetStack
    MOV     r2, r0

    @ Enable ITCM and Setup Stack to ITCM.
    BL      ARM_StackITCM

    @ Call CHIP_BootupInit
    BL      CHIP_RS232Init
    MOV     r1, #0
    CMP     r0, r1
    BEQ     BootMsg
    BX      r1

BootMsg:
    @ Show 'B' to note that console uart is ok.
    MOV     r0, #0x42
    BL      CHIP_DisplayChar

    @ Jump to C Code
    BL      BSP_Start

mt53xx_SRAMSetup:
    @ enable UART
    LDR     r0, =0xf000c020
    LDR     r1, =0x0
    STR     r1, [r0]
    @ Enable 16K OSD + 6K DMX SRAM
    LDR     r0, =0xf00170c0             @ Enable 6K DMX SRAM
    LDR     r1, [r0]
    LDR     r2, =0x4000000
    ORR     r1, r1, r2
    STR     r1, [r0]
    LDR     r0, =0xf0021008             @ Enable 16K OSD SRAM
    LDR     r1, [r0]
    LDR     r2, =0x00200000
    ORR     r1, r1, r2
    STR     r1, [r0]
    BX      lr

HaltSystem:
    @ Done, halt system
    LDR     r0, =MsgHalting
    BL      CHIP_DisplayString

SystemDone:
    B       SystemDone


ARM_EnableICache:
    MOV     r0, #0
    MCR     p15, 0, r0, c7, c5, 0       @ Invalidate I-Cache
    MRC     p15, 0, r0, c1, c0, 0
    ORR     r0, r0, #CR_ICACHE
    MCR     p15, 0, r0, c1, c0, 0
    B       InstNop

ARM_InvalidateICache:
    MOV     r0, #0
    MCR     p15, 0, r0, c7, c5, 0       @ Invalidate I-Cache

InstNop:
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    BX      lr

/*
;*************************************************************************
; Enable ITCM and Setup Stack to ITCM.
; Input: r2 is the SRAM stack pointer
;*************************************************************************
*/
ARM_StackITCM:
    MOV     r1, r2                      @ move R2 to R1, the new ITCM address.
    @ Here, R1 is the TCM start address. Setup All mode Stacks.
    MRS     r0, CPSR
    BIC     r0, r0, #MODE_MASK
    ORR     r0, r0, #MODE_SVC
    MSR     CPSR_cxsf, r0
#ifdef CC_MTK_PRELOADER
    ADD     r2, r1, #0x1000             @ SVC stack is TCM+4KB.
#else
    ADD     r2, r1, #0x4000             @ SVC stack is TCM+16KB for loader usb upgrade
#endif
    MOV     sp, r2
    BIC     r0, r0, #MODE_MASK
    ORR     r0, r0, #MODE_FIQ
    MSR     CPSR_cxsf, r0
    ADD     r2, r1, #0x400              @ SVC stack is TCM+1KB.
    MOV     sp, r2
    BIC     r0, r0, #MODE_MASK
    ORR     r0, r0, #MODE_IRQ
    MSR     CPSR_cxsf, r0
    ADD     r2, r1, #0x400              @ SVC stack is TCM+1KB.
    MOV     sp, r2
    BIC     r0, r0, #MODE_MASK
    ORR     r0, r0, #MODE_ABT
    MSR     CPSR_cxsf, r0
    ADD     r2, r1, #0x800              @ SVC stack is TCM+2KB.
    MOV     sp, r2
    BIC     r0, r0, #MODE_MASK
    ORR     r0, r0, #MODE_UDF
    MSR     CPSR_cxsf, r0
    ADD     r2, r1, #0x800              @ SVC stack is TCM+2KB.
    MOV     sp, r2

    BIC     r0, r0, #MODE_MASK
    ORR     r0, r0, #MODE_SVC
    MSR     CPSR_cxsf, r0

    BX      lr

ARM_SendEvent:
    DSB
    ISB
    SEV
    BX      lr
