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
 * $RCSfile: imgrz_debug.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file imgrz_debug.c
 *  image resizer driver debug interface
 *  
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "imgrz_if.h"
#include "imgrz_drvif.h"
#include "imgrz_dif.h"
#include "imgrz_hw.h"
#include "imgrz_cmdque.h"
#include "imgrz_debug.h"



#if defined(IMGRZ_DEBUG_MODE)
//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

// statistics for debug use
static IMGRZ_DBG_INFO_T _rRzDbgInfo;

static HAL_TIME_T _rRzDbgFlushStartTime;

static IMGRZ_DBG_ENUM2TEXT_T _rRzDbgClkEnum2TextTbl[] =
{
    {(INT32)E_IMGRZ_CLK_XTAL_CK,     "xtal_clk"},
#if 1//def CC_MT5360     
    {(INT32)E_IMGRZ_CLK_SYSPLL_D2,   "syspll / 2"},
    {(INT32)E_IMGRZ_CLK_TVDPLL_D3,   "tvdpll / 3"},
    {(INT32)E_IMGRZ_CLK_TVDPLL_D5,   "tvdpll / 5"},
    {(INT32)E_IMGRZ_CLK_DTDPLL_D3,   "dtdpll / 3"},
    {(INT32)E_IMGRZ_CLK_DTDPLL_D4,   "dtdpll / 4"},
    {(INT32)E_IMGRZ_CLK_SYSPLL_D1P5, "syspll / 1"},
    {(INT32)E_IMGRZ_CLK_MEM,         "mempll"},
#else    
    {(INT32)E_IMGRZ_CLK_SYSPLL_D3,   "syspll / 3"},
    {(INT32)E_IMGRZ_CLK_DTDPLL_D3,   "dtdpll / 3"},
    {(INT32)E_IMGRZ_CLK_SYSPLL_D4,   "syspll / 4"},
    {(INT32)E_IMGRZ_CLK_SYSPLL_D6,   "syspll / 6"},
    {(INT32)E_IMGRZ_CLK_MEM_CK,      "mempll"},
    {(INT32)E_IMGRZ_CLK_TVDPLL_D2,   "tvdpll / 2"},
    {(INT32)E_IMGRZ_CLK_USBPLL_CK,   "usbpll"},
#endif
    {(INT32)E_IMGRZ_CLK_MAX,         NULL}
};


//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------


//-------------------------------------------------------------------------
/** IMGRZ_DbgInit
 *  
 */
//-------------------------------------------------------------------------
void IMGRZ_DbgInit(void)
{
    _rRzDbgInfo.i4DrvFlushCount = 0;
    _rRzDbgInfo.i4DrvIntCount   = 0;
    _rRzDbgInfo.i4MwFlushCount  = 0;
    _rRzDbgInfo.i4MwIntCount    = 0;
    _rRzDbgInfo.i4CmFlushCount  = 0;
    _rRzDbgInfo.i4CmIntCount    = 0;
}


//-------------------------------------------------------------------------
/** IMGRZ_DbgIncDrvFlushCount
 *  increase driver flush count
 */
//-------------------------------------------------------------------------
void IMGRZ_DbgIncDrvFlushCount(void)
{
    _rRzDbgInfo.i4DrvFlushCount++;
}


//-------------------------------------------------------------------------
/** IMGRZ_DbgIncDrvIntCount
 *  increase driver interrupt count
 */
//-------------------------------------------------------------------------
void IMGRZ_DbgIncDrvIntCount(void)
{
    _rRzDbgInfo.i4DrvIntCount++;
}


//-------------------------------------------------------------------------
/** IMGRZ_DbgIncMwFlushCount
 *  increase middleware flush count
 */
//-------------------------------------------------------------------------
void IMGRZ_DbgIncMwFlushCount(void)
{
    _rRzDbgInfo.i4MwFlushCount++;
}


//-------------------------------------------------------------------------
/** IMGRZ_DbgIncMwIntCount
 *  increase middleware interrupt count
 */
//-------------------------------------------------------------------------
void IMGRZ_DbgIncMwIntCount(void)
{
    _rRzDbgInfo.i4MwIntCount++;
}


//-------------------------------------------------------------------------
/** IMGRZ_DbgIncCmFlushCount
 *  increase c-model flush count
 */
//-------------------------------------------------------------------------
void IMGRZ_DbgIncCmFlushCount(void)
{
    _rRzDbgInfo.i4CmFlushCount++;
}


//-------------------------------------------------------------------------
/** IMGRZ_DbgIncCmIntCount
 *  increase c-model interrupt count
 */
//-------------------------------------------------------------------------
void IMGRZ_DbgIncCmIntCount(void)
{
    _rRzDbgInfo.i4CmIntCount++;
}


//-------------------------------------------------------------------------
/** IMGRZ_DbgIncEngineCrashCount
 *  increase engine crash count
 */
//-------------------------------------------------------------------------
void IMGRZ_DbgIncEngineCrashCount(void)
{
    _rRzDbgInfo.i4EngineCrashCount++;
}


//-------------------------------------------------------------------------
/** IMGRZ_DbgDumpInfo
 *  dump debug information
 */
//-------------------------------------------------------------------------
void IMGRZ_DbgDumpInfo(void)
{
    volatile UINT32 u4RegOffset;

    CHAR szClkSrcText[D_RZ_STR_SIZE];

    LOG(1, "\n--- imgrz dbg report ---- (begin)\n");

    LOG(1, "   driver     flush     count = %d\n", _rRzDbgInfo.i4DrvFlushCount);
    LOG(1, "   driver     interrupt count = %d\n", _rRzDbgInfo.i4DrvIntCount);
    LOG(1, "   middleware flush     count = %d\n", _rRzDbgInfo.i4MwFlushCount);
    LOG(1, "   middleware interrupt count = %d\n", _rRzDbgInfo.i4MwIntCount);
    LOG(1, "   c-model    flush     count = %d\n", _rRzDbgInfo.i4CmFlushCount);
    LOG(1, "   c-model    interrupt count = %d\n", _rRzDbgInfo.i4CmIntCount);
    LOG(1, "   engine     crash     count = %d\n", _rRzDbgInfo.i4EngineCrashCount);

    switch (_rRzDbgInfo.i4DrvCurrentStatus)
    {
    case ((INT32)E_IMGRZ_IDLE):
        LOG(1, "   driver     current  status = IDLE\n"); break;
    case ((INT32)E_IMGRZ_BUSY):
        LOG(1, "   driver     current  status = BUSY\n"); break;
    case ((INT32)E_IMGRZ_SEMIIDLE):
        LOG(1, "   driver     current  status = SEMIIDLE\n"); break;
    case ((INT32)E_IMGRZ_CRASHED):
        LOG(1, "   driver     current  status = CRASHED\n"); break;
    case ((INT32)E_IMGRZ_UNKNOWN):
        LOG(1, "   driver     current  status = UNKNOWN\n"); break;
    default:        
        break;
    } // ~switch


    if (_rRzDbgInfo.i4EngineExecuteMod == (INT32)E_RZ_SW_MOD)
    {
        LOG(1, "   engine     execute     mod = Software (C-model)\n");
    }
    else
    {
        LOG(1, "   engine     execute     mod = Hardware\n");
    }

    // get engine clock source text
    IMGRZ_DbgEnum2Text(_rRzDbgInfo.i4EngineClockSrc, szClkSrcText, 
        _rRzDbgClkEnum2TextTbl, (INT32)E_IMGRZ_CLK_MAX);
    LOG(1, "   engine     clock    source = (%s)\n", szClkSrcText);

    u4RegOffset = ((RZ_REG_LAST << 2) & IMGRZ_REG_MASK);
    LOG(1, "   engine start count = %u\n", 
            (IMGRZ_READ32(u4RegOffset) & 0x0000ffff));
#if 0            
    u4RegOffset = ((RZ_REG_DRAMQ_RDPTR << 2) & IMGRZ_REG_MASK);
    LOG(1, "   engine cmdq read ptr = %u\n", 
            (IMGRZ_READ32(u4RegOffset) << 4));
            
    u4RegOffset = ((RZ_REG_DRAMQ_EXPTR << 2) & IMGRZ_REG_MASK);
    LOG(1, "   engine cmdq execute ptr = %u\n", 
            (IMGRZ_READ32(u4RegOffset) << 4));
#endif
    LOG(1, "--- imgrz dbg report ---- (end)\n\n");
}


//-------------------------------------------------------------------------
/** IMGRZ_DbgRecCurrentStatus
 *  record driver code current status
 */
//-------------------------------------------------------------------------
void IMGRZ_DbgRecCurrentStatus(INT32 i4Status)
{
    _rRzDbgInfo.i4DrvCurrentStatus = i4Status;
}


//-------------------------------------------------------------------------
/** IMGRZ_DbgRecoverEngine
 *  Recover engine when engine crashed
 */
//-------------------------------------------------------------------------
void IMGRZ_DbgRecoverEngine(void)
{
    IMGRZ_DbgIncEngineCrashCount();
    IMGRZ_DbgRecCurrentStatus((INT32)E_IMGRZ_CRASHED);

    IMGRZ_DbgShowBusyTime();

#if !defined(IMGRZ_RISC_MODE) // cmdque mode
    IMGRZ_HwISR();
#endif

    IMGRZ_Init();

    // unlock cmdque resource
    IMGRZ_UnlockCmdque();
}


//-------------------------------------------------------------------------
/** IMGRZ_DbgRecFlushTime
 *  record flush start time
 */
//-------------------------------------------------------------------------
void IMGRZ_DbgRecFlushTime(void)
{
    HAL_GetTime(&_rRzDbgFlushStartTime);
}


//-------------------------------------------------------------------------
/** IMGRZ_DbgShowBusyTime
 *  show busy working time
 */
//-------------------------------------------------------------------------
void IMGRZ_DbgShowBusyTime(void)
{
    HAL_TIME_T rCurrentTime;
    HAL_TIME_T rDiffTime;
    
    HAL_GetTime(&rCurrentTime);
    HAL_GetDeltaTime(&rDiffTime, &_rRzDbgFlushStartTime, &rCurrentTime);

    LOG(1, "IMGRZ engine busy working time = %u sec\n", rDiffTime.u4Seconds);
    LOG(1, "-- flush   time = %u sec\n", _rRzDbgFlushStartTime.u4Seconds);
    LOG(1, "-- current time = %u sec\n", rCurrentTime.u4Seconds);
}


//-------------------------------------------------------------------------
/** IMGRZ_DbgRecEngExecuteMod
 *  record engine mode, HW-engine or SW-cmodel
 */
//-------------------------------------------------------------------------
void IMGRZ_DbgRecEngExecuteMod(INT32 i4Mod)
{
    _rRzDbgInfo.i4EngineExecuteMod = i4Mod;
}


//-------------------------------------------------------------------------
/** IMGRZ_DbgEnum2Text
 *  translate enum into text
 */
//-------------------------------------------------------------------------
/*lint -save -e613 -e818 -e830 -e831  */
void IMGRZ_DbgEnum2Text(INT32 i4Enum, CHAR *szText,
    IMGRZ_DBG_ENUM2TEXT_T *prEnum2Text, INT32 i4Stop)
{
    IMGRZ_ASSERT(prEnum2Text != NULL);
    IMGRZ_ASSERT(szText != NULL);

    while (1)
    {
        if (prEnum2Text->i4Value == i4Stop)
        {
            LOG(0, "Error: Enum can NOT be translated into Text\n");
            IMGRZ_ASSERT(prEnum2Text->i4Value != i4Stop);
        }
        
        if (i4Enum == prEnum2Text->i4Value)
        {
            break;
        }
        else
        {
            prEnum2Text++;
        }
    }

    IMGRZ_UNUSED_RET(x_strcpy(szText, prEnum2Text->szText));
}
/*lint -restore */


//-------------------------------------------------------------------------
/** IMGRZ_DbgRecEngClkSrc
 *  record engine clock source
 */
//-------------------------------------------------------------------------
void IMGRZ_DbgRecEngClkSrc(INT32 i4ClkSrc)
{
    _rRzDbgInfo.i4EngineClockSrc = i4ClkSrc;
}


//-------------------------------------------------------------------------
/** IMGRZ_DbgCheckEngIdle
 *  check engine idle
 */
//-------------------------------------------------------------------------
void IMGRZ_DbgCheckEngIdle(void)
{
    VERIFY(IMGRZ_HwGetIdle());
    VERIFY(_rRzDbgInfo.i4DrvFlushCount == _rRzDbgInfo.i4DrvIntCount);
}
#endif // #if defined(IMGRZ_DEBUG_MODE)



