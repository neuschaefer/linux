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

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#include "x_printf.h"
#include "x_stl_lib.h"

#include "x_mid.h"
#include "x_hal_926.h"
#include "x_bim.h"
#include "v6_pmu.h"

LINT_EXT_HEADER_END

/*
ARMv6 event type
0xB Data cache miss. Does not include Cache Operations.
0xA Data cache access. Does not include Cache Operations. This event occurs for each
    nonsequential access to a cache line, regardless of whether or not the location is
    cachable.
0x9 Data cache access. Does not include Cache Operations. This event occurs for each
    nonsequential access to a cache line, for cachable locations.
*/
//---------------------------------------------------------------------
void CPU_ResetMonitor(void)
{
    reset_ccnt();              // Reset the CCNT (cycle counter)
    reset_pmn();               // Reset the configurable counters
    pmn_config(0, 0x09);       // cache access
    pmn_config(1, 0x0b);       // cache miss
}

void CPU_StartMonitor(void)
{
    enable_pmu();              // Enable the PMU
}

void CPU_EndMonitor(void)
{
    disable_pmu();
}

void CPU_PrintMonitor(void)
{
    UINT32 cnt[2];
    cnt[0] = read_pmn(0);
    cnt[1] = read_pmn(1);
    printf("cache(access/miss)=(%d/%d), cycle=%d\n",
        cnt[0], cnt[1], read_ccnt());
}

void CPU_ResetTick(void)
{
    IO_WRITE32MSK(BIM_BASE, REG_RW_TIMER_CTRL, 0, TMR1_CNTDWN_EN | TMR1_AUTOLD_EN);
    BIM_WRITE32(REG_RW_TIMER1_LLMT, 0xffffffff);
    BIM_WRITE32(REG_RW_TIMER1_LOW , 0xffffffff);
    IO_WRITE32MSK(BIM_BASE, REG_RW_TIMER_CTRL, TMR1_CNTDWN_EN | TMR1_AUTOLD_EN, TMR1_CNTDWN_EN | TMR1_AUTOLD_EN);
}

UINT32 CPU_GetTick(void)
{
    return ~BIM_READ32(REG_RW_TIMER1_LOW);
}

void ConfigPerformanceMonitorControlReg(UINT32 value)
{
    HalFlushInvalidateDCache();
    HalInvalidateICache();
    CPU_ResetTick();
}


UINT32 ReadARMCycleCounter(void)
{
    return CPU_GetTick();
}


BOOL IsARMCycleCounterOverflow(void)
{
  return 0;
}

#if 0
void ConfigPerformanceMonitorControlReg(UINT32 value)
{
  HalFlushInvalidateDCache();
  HalInvalidateICache();
#if 0
  __asm
  {
    MCR     p15, 0, value, c15, c12, 0
  }
#else
    IO_WRITE32MSK(BIM_BASE, REG_RW_TIMER_CTRL, 0, TMR1_CNTDWN_EN | TMR1_AUTOLD_EN);
    BIM_WRITE32(REG_RW_TIMER1_LLMT, 0xffffffff);
    BIM_WRITE32(REG_RW_TIMER1_LOW , 0xffffffff);
    IO_WRITE32MSK(BIM_BASE, REG_RW_TIMER_CTRL, TMR1_CNTDWN_EN | TMR1_AUTOLD_EN, TMR1_CNTDWN_EN | TMR1_AUTOLD_EN);
#endif
}


UINT32 ReadARMCycleCounter(void)
{
#if 0
  UINT32 r;

  __asm
  {
    MRC     p15, 0, r, c15, c12, 1
  } 

    return (r);
#else
    return ~BIM_READ32(REG_RW_TIMER1_LOW);
#endif
}


BOOL IsARMCycleCounterOverflow(void)
{
#if 0
  UINT32 r;
  UINT32 overflow_bit;

  __asm
  {
    MRC     p15, 0, r, c15, c12, 0
  } 

  overflow_bit = (r>>10) & 0x0001;
  return ((overflow_bit == 1));
#else
  return 0;
#endif
}
#endif
