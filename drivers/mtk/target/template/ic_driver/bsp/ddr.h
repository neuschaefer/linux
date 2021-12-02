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
/** @file ddr.h
 *  Dram configurations and options.
 */

#ifndef DDR_H
#define DDR_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "drvcust_if.h"
#include "c_model.h"
#include "x_typedef.h"
#include "x_dram.h"

LINT_EXT_HEADER_END

//lint --e{717} do ... while (0);


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

// Debug level of DDR
#ifndef CC_DEFAULT_LOGLVL
#define CC_DEFAULT_LOGLVL           5
#endif //CC_DEFAULT_LOGLVL

#ifdef CC_MT5392B
#define ENABLE_DRAM_DATA_SCRAMBLE 1	// enable/disable scramble
#else
#define ENABLE_DRAM_DATA_SCRAMBLE 0
#endif

#define DRAM_SPREAD_SPECTRUM_RANGE	0	//0= disable; max=100, might crash if !=0

//#define CC_DRAM_DEBUG
// Dram debug mode
#ifdef CC_DRAM_DEBUG
#define CC_DRAM_STOP_DEBUG          1
#else
#define CC_DRAM_STOP_DEBUG          0
#endif

// Use manual dqs or auto-calibration
#define CC_MANUAL_DQS_MODE          0
#if CC_MANUAL_DQS_MODE
    #define MANUAL_DQS              0xF7F7F7F7
#endif //CC_MANUAL_DQS_MODE

// Use auto mode or manual mode
#define CC_USE_AUTO_MODE            1

// Use analog DLL or digital DLL
// Should put before including ddr2.h and ddr3.h
#define CC_USE_ANALOG_DLL           0
#if CC_USE_ANALOG_DLL
#define CC_USE_AUTO_MODE            0   // analog dll mode => manual mode
#endif //CC_USE_ANALOG_DLL

#if (DEFAULT_DRAM_TYPE==DDR_II_x1) || (DEFAULT_DRAM_TYPE==DDR_II_x2)
#define CC_SUPPORT_DDR2             1
#define CC_SUPPORT_DDR3             0
#include "ddr2.h"
#elif (DEFAULT_DRAM_TYPE==DDR_III_x1) || (DEFAULT_DRAM_TYPE==DDR_III_x2)
#define CC_SUPPORT_DDR2             0
#define CC_SUPPORT_DDR3             1
#include "ddr3.h"
#else
#error "Not support dram type!!"
#endif

#define CC_ONLY_SUPPORT_16BIT       0

// Record calibration result in backup register
#define CC_RECORD_DQS               1
#if CC_RECORD_DQS
#define RECORD_DQS_REG              0x20028140
#endif

// Calibration algorithm version
#define CC_CALIB_VER                3


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

// Offset to avoid non-stable boundary
#define DDR_CALIB_JUMP              5

#define DRAM_TEST_DATA1     0x5AA5A55A
#define DRAM_TEST_DATA2     0xA55A5AA5

#define MAX_PAT         (sizeof(_au4Pattern) / sizeof(UINT32))
#define PATTERN(i)      (_au4Pattern[((i) % MAX_PAT)])
#define NUM_ELE(a)      (sizeof(a) / sizeof(a[0]))

typedef enum {
    DDR_III  = 0,
    DDR_II   = 1
} DDR_PACK_TYPE_T;

typedef enum {
    DDR_RTT_DISABLE = 0,
    DDR_RTT_75OHM   = 1,
    DDR_RTT_150OHM  = 2,
    DDR_RTT_50OHM   = 3
} DDR_RTT_T;

#define DDR_PHY_CTRL_1_RST          0x10000000  //70F0H
#define DDR_CTRL_1_RST              0x02000000  //70F4H
#define DDR_DQSIEN_DMCKSEL          0x80000000


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#if defined(CC_NAND_BOOT) || defined(CC_ROM_BOOT) || (defined(CC_5391_PRELOADER) && defined(NDEBUG))
    #define LOG(level, fmt...)
#else /* defined(CC_5391_PRELOADER) && defined(NDEBUG) */
    EXTERN UINT32 _u4DDRLogLvl;
    #define LOG(level, fmt...)              \
        do                                  \
        {                                   \
            UINT32 u4_level = level;        \
            if (u4_level <= _u4DDRLogLvl)   \
            {                               \
                Printf(fmt);                \
            }                               \
        } while (0)
#endif /* defined(CC_5391_PRELOADER) && defined(NDEBUG) */

// After setting dram parameter, wait for stable
#define WAIT_DRAM_STABLE()              \
    do                                  \
    {                                   \
        UINT32 z;                       \
        for (z = 0; z < 0x400; z++) {}; \
    } while (0)

#define BIN_2_GREY(u4Bin)   ((u4Bin) ^ ((u4Bin) >> 1))


//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

#ifndef __MODEL_slt__
EXTERN void CLK_QueryDramSetting(UINT8 *szString);
#else
#define CLK_QueryDramSetting(x)
#endif
EXTERN void print_dqs(INT32 i4Line, INT32 i4Min, INT32 i4Max, INT32 i4Avg);
EXTERN void _DDR_SetDdr2BeforeAI(UINT32 u4Clk, UINT32 fgFull,
    UINT32 fg64En, UINT32 fgDQS, UINT32 fgGDDR, UINT32 u4ChipNum);
EXTERN void _DDR_SetDdr2AfterAI(UINT32 u4Clk, UINT32 fgFull, UINT32 fg64En,
    UINT32 fgDQS, UINT32 fgGDDR, UINT32 u4ChipNum);
EXTERN void _DDR_SetDdr3BeforeAI(UINT32 u4Clk, UINT32 fgFull,
    UINT32 fg64En, UINT32 fgDQS, UINT32 fgGDDR, UINT32 u4ChipNum);
EXTERN void _DDR_SetDdr3AfterAI(UINT32 u4Clk, UINT32 fgFull, UINT32 fg64En,
    UINT32 fgDQS, UINT32 fgGDDR, UINT32 u4ChipNum);
EXTERN void _DDR_SetRtt(DDR_RTT_T eRtt);
EXTERN void DDR_UseAnalogDll(void);


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

void _DDR_SetDQS(UINT32 u4DQSth, UINT32 u4Bin);
void _DDR_SetDQ(UINT32 u4DQth, UINT32 u4Bin);
void _DDR_ResetPhy(void);


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

UINT32 DDR_CalibrateDqs(void);


#endif //DDR_H

