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
#include "c_model.h"
#include "../../drv_cust/drv_default.h"
#include "drvcust_if.h"
#include "x_typedef.h"
#include "x_dram.h"

LINT_EXT_HEADER_END

//lint --e{717} do ... while (0);

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

#ifdef NDEBUG
// DRAM debug mode.
//#undef NDEBUG
#endif

#ifndef NDEBUG
//#define DRAM_DEBUG
#endif /* NDEBUG */

//#define DRAM_SLT_DEBUG   /* Use for SLT and Nand boot debug DRAM loader */

#define DRAM_GATING_SCAN

//#define DRAM_DEBUG_TOOL

//#define DRAM_GATING_DQS_CALIBRATION

//#define DRAM_DQINPUTPERBITCALI

//#define DRAM_DUTY_DQS_CALIBRATION

//#define DRAM_CS_DELAY_CALIBRATION

//#define DRAM_CMD_DELAY_CALIBRATION

//#define DRAM_WRITE_DQS_CALIBRATION

//#define DRAM_CLOCK_WRITE_DQS_CALIBRATION

#define DRAM_HW_CALIBRATION_V1 /* Totally h/w calibration */

#define DRAM_HW_DQINPUTPERBITCALI 

//#define DRAM_TESTAGENT_CALIBRATION

//#define DRAM_MAIN_IC_ODT_CALIBRATION

//#define DRAM_DRAM_ODT_CALIBRATION

//#define DRAM_WDQS_CALIBRATION

//#define DRAM_WDQS_PERBITCALIBRATION

#ifdef CC_BOARD_DETECT_DRAM_TYPE   // Caution: Only used under gpio select DRAM type

#define CC_SUPPORT_DDR3
#include "ddr3.h"

// Use fixed DQS input delay or by boot up calibration at ddr_cal3.c
//#define DRAM_MANUAL_DQS_MODE
#define MANUAL_DQS              0x00000E0E  // DQS3~0 : 0, 0, 14, 14.

#else /* CC_BOARD_DETECT_DRAM_TYPE */

#if ((DEFAULT_DRAM_TYPE == DDR_II_x1) || (DEFAULT_DRAM_TYPE == DDR_II_x2) || (DEFAULT_DRAM_TYPE == DDR_II_x3) || (DEFAULT_DRAM_TYPE == DDR_II_x4))
#define CC_SUPPORT_DDR2
#include "ddr2.h"

// Use fixed DQS input delay or by boot up calibration at ddr_cal3.c
//#define DRAM_MANUAL_DQS_MODE
#define MANUAL_DQS              0x00000E0E  // DQS3~0 : 0, 0, 14, 14.

#elif ((DEFAULT_DRAM_TYPE == DDR_III_x1) || (DEFAULT_DRAM_TYPE == DDR_III_x2) || (DEFAULT_DRAM_TYPE == DDR_III_x3) || (DEFAULT_DRAM_TYPE == DDR_III_x4))
#define CC_SUPPORT_DDR3
#include "ddr3.h"

// Use fixed DQS input delay or by boot up calibration at ddr_cal3.c
//#define DRAM_MANUAL_DQS_MODE
#define MANUAL_DQS              0x00000E0E  // DQS3~0 : 0, 0, 14, 14.

#else
#error "No DRAM type is configured."
#endif

#endif /* CC_BOARD_DETECT_DRAM_TYPE */
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
typedef enum 
{
    DDR_RTT_DISABLE = 0,
    DDR_RTT_75OHM   = 1,
    DDR_RTT_150OHM  = 2,
    DDR_RTT_50OHM   = 3, 
    DDR_RTT_37OHM  = 4,
    DDR_RTT_25OHM  = 5,
    DDR_RTT_30OHM  = 6,
    DDR_RTT_21OHM  = 7    
} DDR_RTT_T;

#define DRAM_CHANGE_CLK_SETTING                    1
#define DRAM_CHANGE_DDR_BEFORE_AUTOINIT  2
#define DRAM_CHANGE_DDR_AFTER_AUTOINIT    3

#define BYTE_PER_CHIP   2
#define RECORD_DQS_REG             (IO_VIRT + 0x8100)

#define DRAM_DQS_OUTPUT_TYPE   0
#define DRAM_DQS_INPUT_TYPE   1
#define DRAM_DQS_NO_CHANGE_TYPE   2

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#if defined(CC_ROM_BOOT) || (defined(CC_MTK_PRELOADER) && defined(NDEBUG))
    #define LOG(level, fmt...)
#else /* defined(CC_MTK_PRELOADER) && defined(NDEBUG) */
    #define LOG(level, fmt...)  Printf(fmt)
#endif /* defined(CC_MTK_PRELOADER) && defined(NDEBUG) */

//#define BIN_2_GREY(u4Bin)   ((u4Bin) ^ ((u4Bin) >> 1))

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
#if !defined(__MODEL_slt__) || defined(DRAM_SLT_DEBUG)
EXTERN void CLK_QueryDramSetting(UINT8 *szString, UINT32 u4Stage);
#else
#define CLK_QueryDramSetting(x, y)
#endif

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

#ifdef CC_SUPPORT_DDR2
EXTERN void _DDR_SetDdr2BeforeAI(void);
EXTERN void _DDR_SetDdr2AfterAI(void);
#endif

#ifdef CC_SUPPORT_DDR3
EXTERN void _DDR_SetDdr3BeforeAI(void);
EXTERN void _DDR_SetDdr3AfterAI(void);
#endif

EXTERN void _DDR_SetDdrMode(UINT32 fgPostWrite);
EXTERN void _DDR_SetRTT(UINT32 u4MainChipODT);
EXTERN UINT16 _DDR_GrayToBinary(UINT16 num);
EXTERN void _DDR_DeDQSGlitch(void);
EXTERN void _DDR_SetDQS(UINT32 u4DQSth, UINT32 u4Bin);
EXTERN void _DDR_SetOutDQS(UINT32 u4DQSth, UINT32 u4Bin);
EXTERN void _DDR_SetDQ(UINT32 u4DQSth, UINT32 u4Bin, UINT32 u4DQInputDelay[]);
EXTERN void _DDR_SetOutDQ(UINT32 u4DQSth, UINT32 u4Bin);
EXTERN void _DDR_SetDQPerBit(UINT32 u4DQSth, INT8 i1DQPerBitDelay[], UINT32 u4DQInputDelay[]);
EXTERN void _DDR_ResetPhy(void);
EXTERN void _DDR_ResetPhyOnly(void);
EXTERN BOOL _DDR_CheckColumnAddress(void);
EXTERN void _DDR_CheckBankAddress(BOOL fgColAddr9bits);
EXTERN void _DDR_QueryDramScrambleDataKey(void);
EXTERN UINT32 _DDR_CheckSize(BOOL fgChBActive);
EXTERN void _DDR_ToolMain(void);
EXTERN void _DDR_GatingScan(void);
EXTERN void DDR_PerbitCalibrateOutDqs(void);


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
EXTERN void CHIP_Delay_us(UINT32 u4Micros);
EXTERN void CHIP_DisplayString(const CHAR *szString);
EXTERN void CHIP_DisplayInteger(INT32 i);
EXTERN void CHIP_DisplayChar(UINT32 u4Char);
EXTERN UINT32 CHIP_IsRemap(void);

#endif //DDR_H

