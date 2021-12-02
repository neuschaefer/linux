/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *---------------------------------------------------------------------------
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
static IMGRZ_DBG_INFO_T rRzDbgInfo[IMGRZ_NUM];
#define _rRzDbgInfo rRzDbgInfo[u4Which]

static HAL_TIME_T rRzDbgFlushStartTime[IMGRZ_NUM];
#define _rRzDbgFlushStartTime rRzDbgFlushStartTime[u4Which]

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
void IMGRZ_DbgInit(UINT32 u4Which)
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
void IMGRZ_DbgIncDrvFlushCount(UINT32 u4Which)
{
    _rRzDbgInfo.i4DrvFlushCount++;
}


//-------------------------------------------------------------------------
/** IMGRZ_DbgIncDrvIntCount
 *  increase driver interrupt count
 */
//-------------------------------------------------------------------------
void IMGRZ_DbgIncDrvIntCount(UINT32 u4Which)
{
    _rRzDbgInfo.i4DrvIntCount++;
}


//-------------------------------------------------------------------------
/** IMGRZ_DbgIncMwFlushCount
 *  increase middleware flush count
 */
//-------------------------------------------------------------------------
void IMGRZ_DbgIncMwFlushCount(UINT32 u4Which)
{
    _rRzDbgInfo.i4MwFlushCount++;
}


//-------------------------------------------------------------------------
/** IMGRZ_DbgIncMwIntCount
 *  increase middleware interrupt count
 */
//-------------------------------------------------------------------------
void IMGRZ_DbgIncMwIntCount(UINT32 u4Which)
{
    _rRzDbgInfo.i4MwIntCount++;
}


//-------------------------------------------------------------------------
/** IMGRZ_DbgIncCmFlushCount
 *  increase c-model flush count
 */
//-------------------------------------------------------------------------
void IMGRZ_DbgIncCmFlushCount(UINT32 u4Which)
{
    _rRzDbgInfo.i4CmFlushCount++;
}


//-------------------------------------------------------------------------
/** IMGRZ_DbgIncCmIntCount
 *  increase c-model interrupt count
 */
//-------------------------------------------------------------------------
void IMGRZ_DbgIncCmIntCount(UINT32 u4Which)
{
    _rRzDbgInfo.i4CmIntCount++;
}


//-------------------------------------------------------------------------
/** IMGRZ_DbgIncEngineCrashCount
 *  increase engine crash count
 */
//-------------------------------------------------------------------------
void IMGRZ_DbgIncEngineCrashCount(UINT32 u4Which)
{
    _rRzDbgInfo.i4EngineCrashCount++;
}


//-------------------------------------------------------------------------
/** IMGRZ_DbgDumpInfo
 *  dump debug information
 */
//-------------------------------------------------------------------------
void IMGRZ_DbgDumpInfo(UINT32 u4Which)
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
            (IMGRZ_READ32(u4Which, u4RegOffset) & 0x0000ffff));
#if 0            
    u4RegOffset = ((RZ_REG_DRAMQ_RDPTR << 2) & IMGRZ_REG_MASK);
    LOG(1, "   engine cmdq read ptr = %u\n", 
            (IMGRZ_READ32(u4Which, u4RegOffset) << 4));
            
    u4RegOffset = ((RZ_REG_DRAMQ_EXPTR << 2) & IMGRZ_REG_MASK);
    LOG(1, "   engine cmdq execute ptr = %u\n", 
            (IMGRZ_READ32(u4Which, u4RegOffset) << 4));
#endif
    LOG(1, "--- imgrz dbg report ---- (end)\n\n");
}


//-------------------------------------------------------------------------
/** IMGRZ_DbgRecCurrentStatus
 *  record driver code current status
 */
//-------------------------------------------------------------------------
void IMGRZ_DbgRecCurrentStatus(UINT32 u4Which, INT32 i4Status)
{
    _rRzDbgInfo.i4DrvCurrentStatus = i4Status;
}


//-------------------------------------------------------------------------
/** IMGRZ_DbgRecoverEngine
 *  Recover engine when engine crashed
 */
//-------------------------------------------------------------------------
void IMGRZ_DbgRecoverEngine(UINT32 u4Which)
{
    IMGRZ_DbgIncEngineCrashCount(u4Which);
    IMGRZ_DbgRecCurrentStatus(u4Which, (INT32)E_IMGRZ_CRASHED);

    IMGRZ_DbgShowBusyTime(u4Which);

#if !defined(IMGRZ_RISC_MODE) // cmdque mode
    IMGRZ_HwISR(u4Which);
#endif

    _IMGRZ_Init(u4Which);

    // unlock cmdque resource
    _IMGRZ_UnlockCmdque(u4Which);
}


//-------------------------------------------------------------------------
/** IMGRZ_DbgRecFlushTime
 *  record flush start time
 */
//-------------------------------------------------------------------------
void IMGRZ_DbgRecFlushTime(UINT32 u4Which)
{
    HAL_GetTime(&_rRzDbgFlushStartTime);
}


//-------------------------------------------------------------------------
/** IMGRZ_DbgShowBusyTime
 *  show busy working time
 */
//-------------------------------------------------------------------------
void IMGRZ_DbgShowBusyTime(UINT32 u4Which)
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
void IMGRZ_DbgRecEngExecuteMod(UINT32 u4Which, INT32 i4Mod)
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
void IMGRZ_DbgRecEngClkSrc(UINT32 u4Which, INT32 i4ClkSrc)
{
    _rRzDbgInfo.i4EngineClockSrc = i4ClkSrc;
}


//-------------------------------------------------------------------------
/** IMGRZ_DbgCheckEngIdle
 *  check engine idle
 */
//-------------------------------------------------------------------------
void IMGRZ_DbgCheckEngIdle(UINT32 u4Which)
{
    VERIFY(IMGRZ_HwGetIdle(u4Which));
    VERIFY(_rRzDbgInfo.i4DrvFlushCount == _rRzDbgInfo.i4DrvIntCount);
}
#endif // #if defined(IMGRZ_DEBUG_MODE)



