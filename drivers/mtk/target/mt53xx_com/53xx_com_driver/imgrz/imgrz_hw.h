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
/*-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: imgrz_hw.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file imgrz_hw.h
 *  image resizer hardware interface
 *  init, get register base, isr, vpll calibration, reset,
 *  get status, get que length, get idle, get yline, get op count,
 *  get hardware version, set power down mode
 */

#ifndef IMGRZ_HW_H
#define IMGRZ_HW_H


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

//#define IMGRZ_ADDR                IO_ADDR(GRAPH_BASE)
#define IMGRZ_ADDR                  (IMGRZ_BASE)

#define IMGRZ_ENG_RST_READY         ((UINT32)0x00000000)
#define IMGRZ_CMDQ_RST_READY        ((UINT32)0x00000000)

// must use IO_READ32 or IO_WRITE32 to control HW registers
//#define RZREG_RZSTART               (0x00)
//#define RZREG_RZDONE                (0xFC)

//#define RZREG_DRAMQ_CFG             (0x1C4)
//#define RZREG_DRAMQ_STAD            (0x1C8)

//#define RZREG_DRAMQ_LEN             (0x1CC)

#define IMGRZ_REG_MASK              (0xFFF)


//------------------------------------------------
// HW registers' shift
//   [RZSTART] - 0x20000
#define RZREG_SHT_REGRST             8
#define RZREG_SHT_INT_MASK           7
#define RZREG_SHT_SW_RSTB            2
#define RZREG_SHT_CKEN               1
#define RZREG_SHT_START              0
//   [RZDONE] - 0x200FC
#define RZREG_SHT_IDLE               4
//   [DRAMQ_CFG] - 0x201C4
#define RZREG_SHT_SW_CMDQ_RST_B     30
#define RZREG_SHT_STOP              16
#define RZREG_SHT_DRAMQ_SIZE         8
#define RZREG_SHT_CMDQ_THRS          4
#define RZREG_SHT_DRAMQ_MODE         1
#define RZREG_SHT_EN_DRAMQ           0
//   [DRAMQ_STAD] - 0x201C8
#define RZREG_SHT_DRAMQ_BSAD         0


//------------------------------------------------
// HW registers' mask
//   [RZSTART] - 0x20000
#define RZREG_MSK_INT_MASK      ((UINT32)0x1 << RZREG_SHT_INT_MASK)
#define RZREG_MSK_SW_RSTB       ((UINT32)0x3 << RZREG_SHT_SW_RSTB)
#define RZREG_MSK_CKEN          ((UINT32)0x1 << RZREG_SHT_CKEN)
#define RZREG_MSK_START         ((UINT32)0x1 << RZREG_SHT_START)
#define RZREG_MSK_REGRST        ((UINT32)0x3 << RZREG_SHT_REGRST)
//   [RZDONE] - 0x200FC
#define RZREG_MSK_IDLE          ((UINT32)0x1 << RZREG_SHT_IDLE)
//   [DRAMQ_CFG] - 0x201C4
#define RZREG_MSK_SW_CMDQ_RST_B ((UINT32)0x3 << RZREG_SHT_SW_CMDQ_RST_B)
#define RZREG_MSK_STOP          ((UINT32)0x1 << RZREG_SHT_STOP)
#define RZREG_MSK_DRAMQ_SIZE    ((UINT32)0x7 << RZREG_SHT_DRAMQ_SIZE)
#define RZREG_MSK_CMDQ_THRS     ((UINT32)0x3 << RZREG_SHT_CMDQ_THRS)
#define RZREG_MSK_DRAMQ_MODE    ((UINT32)0x1 << RZREG_SHT_DRAMQ_MODE)
#define RZREG_MSK_EN_DRAMQ      ((UINT32)0x1 << RZREG_SHT_EN_DRAMQ)
//   [DRAMQ_STAD] - 0x201C8
#define RZREG_MSK_DRAMQ_BSAD    ((UINT32)0xFFFFFFF << RZREG_SHT_DRAMQ_BSAD)

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

/// Read 32 bits data from GFX HW registers.
#define IMGRZ_READ32(offset)           IO_READ32(IMGRZ_ADDR, (offset))

/// Write 32 bits data into GFX HW registers.
#define IMGRZ_WRITE32(offset, value)   IO_WRITE32(IMGRZ_ADDR, (offset), (value))


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

// 5 hw engine state
enum EIMGRZ_CORE_STATUS_T
{
    E_IMGRZ_IDLE = 0,
    E_IMGRZ_BUSY,
    E_IMGRZ_SEMIIDLE,
    E_IMGRZ_CRASHED,
    E_IMGRZ_UNKNOWN
};

// engine clock source
enum EIMGRZ_ENGINE_CLOCK_T
{
#if 1//def CC_MT5360 
    E_IMGRZ_CLK_XTAL_CK = 0,
    E_IMGRZ_CLK_SYSPLL_D2,    // system pll / 2
    E_IMGRZ_CLK_TVDPLL_D3,    // tv-dec pll / 3
    E_IMGRZ_CLK_TVDPLL_D5,    // tv-dec pll / 5
    E_IMGRZ_CLK_DTDPLL_D3,    // dtd    pll / 3   (200 MHz)
    E_IMGRZ_CLK_DTDPLL_D4,    // dtd    pll / 4
    E_IMGRZ_CLK_SYSPLL_D1P5,  //sys      pll / 1 (216 MHz)
    E_IMGRZ_CLK_MEM,          // mem    pll
#else
    E_IMGRZ_CLK_XTAL_CK = 0,
    E_IMGRZ_CLK_SYSPLL_D3,     // 216MHz
    E_IMGRZ_CLK_DTDPLL_D3,     // 200/144 MHz
    E_IMGRZ_CLK_SYSPLL_D4,     // 162MHz
    E_IMGRZ_CLK_SYSPLL_D6,     // 108MHz
    E_IMGRZ_CLK_MEM_CK,        // UNUSED
    E_IMGRZ_CLK_TVDPLL_D2,     // 270MHz
    E_IMGRZ_CLK_USBPLL_CK,     //240MHz
#endif
    E_IMGRZ_CLK_MAX
};


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

extern void IMGRZ_HwInit(void);

extern void IMGRZ_HwGetRegBase(UINT32 **ppu4RegBase);

extern void IMGRZ_HwISR(void);

extern void IMGRZ_HwReset(void);

extern INT32 IMGRZ_HwGetIdle(void);

extern void IMGRZ_HwAction(void);

extern void IMGRZ_HwSetRiscMode(void);

extern void IMGRZ_HwSetCmdQMode(void);

extern void IMGRZ_HwSetEngClkSrc(INT32 i4ClkSrc);

#ifdef IMGRZ_POLLING
extern INT32 IMGRZ_HwPolling(void);
#endif

extern void IMGRZ_HwBreak(void);

extern void IMGRZ_HwResume(void);

#endif // IMGRZ_HW_H

extern void IMGRZ_HwPowerDown(void);


