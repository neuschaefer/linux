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
/*
;************************************************************************
;*
;*  Arm vector table entry files
;*
;************************************************************************
*/
#include "arm_cpu.h"

    .global  INT_Vectors
    .global  RemapJumpToDram

INT_Vectors:
#ifndef CC_MTK_PRELOADER
    LDR     pc, INT_Reset_Addr
#else
    B       INT_Reset
#endif
    LDR     pc, Undef_Instr_Addr
    LDR     pc, SWI_Addr
    LDR     pc, Prefetch_Abort_Addr
    LDR     pc, Data_Abort_Addr
    LDR     pc, Reserved_Addr
    LDR     pc, IRQ_Handler_Addr
    LDR     pc, FIQ_Handler_Addr

INT_Reset_Addr:
    .word INT_Reset
Undef_Instr_Addr:
    .word INT_Undef_Inst
SWI_Addr:
    .word INT_Software
Prefetch_Abort_Addr:
    .word INT_Prefetch_Abort
Data_Abort_Addr:
    .word INT_Data_Abort
Reserved_Addr:
    .word INT_Reserved
IRQ_Handler_Addr:
#ifndef CC_MTK_PRELOADER
    .word CIrqHandler
#else
    .word INT_IRQ
#endif    
FIQ_Handler_Addr:
#ifndef CC_MTK_PRELOADER
    .word CFiqHandler
#else
    .word INT_FIQ
#endif    

/*
;************************************************************************
;* Function: RemapJumpToDram
;* Description:
;*      Before calling this function, system must be run on norflash.
;*  And finished to copy or extract application to the beginning of dram.
;*  This function will change the address of norflash & dram and jump to
;*  address 0 then (INT_Reset of application at dram).
;*      This function will be called at loader. While remapping dram
;*  address, it will run on dram of application. And then jump to Reset
;*  vector of application to start application program.
;*      Because this function won't return to caller. All registers
;*  content are don't care.
;************************************************************************
*/

RemapJumpToDram:

    @ Invalidate ICache
    MOV     r0, #0
    MCR     p15, 0, r0, c7, c5, 0

    @ Disable ICache
    MRC     p15, 0, r0, c1, c0, 0
    BIC     r0, r0, #CR_ICACHE
    MCR     p15, 0, r0, c1, c0, 0
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP

    @ Set remap register
    LDR     r0, =REG_RW_REMAP
    LDR     r1, =BIT_REMAP
    STR     r1, [r0, #0]            @ Trigger remapping

    @ Run nop instruction for pipeline, arm11 has 8 level pipeline!
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP

@ LOOP
@    B       LOOP

    @ Jump to address 0x00000000 of dram.
@    MOV     r0, #0
@    BX      r0
    B       INT_Reset
