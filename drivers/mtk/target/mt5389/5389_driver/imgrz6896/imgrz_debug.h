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
 * $RCSfile: imgrz_debug.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file imgrz_debug.h
 *  Brief of file imgrz_debug.h.
 *  Details of file imgrz_debug.h (optional).
 */

#ifndef IMGRZ_DEBUG_H
#define IMGRZ_DEBUG_H


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#define DEFINE_IS_LOG   IMGRZ_IsLog
#include "x_debug.h"


//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

#if defined(IMGRZ_DEBUG_MODE)

// debug information
typedef struct _IMGRZ_DBG_INFO_T
{
    INT32 i4DrvFlushCount;      // driver     flush     count
    INT32 i4DrvIntCount;        // driver     interrupt count
    INT32 i4MwFlushCount;       // middleware flush     count
    INT32 i4MwIntCount;         // middleware interrupt count
    INT32 i4CmFlushCount;       // c-model    flush     count
    INT32 i4CmIntCount;         // c-model    interrupt count
    INT32 i4EngineCrashCount;   // engine     crash     count
    INT32 i4DrvCurrentStatus;   // driver     current   status
    INT32 i4EngineExecuteMod;   // engine     execute   mod
    INT32 i4EngineClockSrc;     // engine     clock     source
} IMGRZ_DBG_INFO_T;

typedef struct _IMGRZ_DBG_ENUM2TEXT_T
{
    INT32 i4Value;
    CHAR *szText;
} IMGRZ_DBG_ENUM2TEXT_T;

#endif // #if defined(IMGRZ_DEBUG_MODE)


//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

#if defined(IMGRZ_DEBUG_MODE)

    #define IMGRZ_DBG_INC_DRV_FLUSH_COUNT(u4Which)       IMGRZ_DbgIncDrvFlushCount(u4Which)
    #define IMGRZ_DBG_INC_DRV_INT_COUNT(u4Which)         IMGRZ_DbgIncDrvIntCount(u4Which)
    #define IMGRZ_DBG_INC_MW_FLUSH_COUNT(u4Which)        IMGRZ_DbgIncMwFlushCount(u4Which)
    #define IMGRZ_DBG_INC_MW_INT_COUNT(u4Which)          IMGRZ_DbgIncMwIntCount(u4Which)
    #define IMGRZ_DBG_INC_CM_FLUSH_COUNT(u4Which)        IMGRZ_DbgIncCmFlushCount(u4Which)
    #define IMGRZ_DBG_INC_CM_INT_COUNT(u4Which)          IMGRZ_DbgIncCmIntCount(u4Which)
    #define IMGRZ_DBG_INC_ENG_CRASH_COUNT(u4Which)       IMGRZ_DbgIncEngineCrashCount(u4Which)
    #define IMGRZ_DBG_RECOVER_ENGINE(u4Which)            IMGRZ_DbgRecoverEngine(u4Which)
    #define IMGRZ_DBG_REC_FLUSH_TIME(u4Which)            IMGRZ_DbgRecFlushTime(u4Which)
    #define IMGRZ_DBG_REC_ENG_EXE_MOD(u4Which, mod)      IMGRZ_DbgRecEngExecuteMod(u4Which, mod)
    #define IMGRZ_DBG_REC_CUR_STATUS(u4Which, status)    IMGRZ_DbgRecCurrentStatus(u4Which, status)
    #define IMGRZ_DBG_REC_ENG_CLK_SRC(u4Which, clk)      IMGRZ_DbgRecEngClkSrc(u4Which, clk)
    #define IMGRZ_DBG_CHK_ENG_IDLE(u4Which)              IMGRZ_DbgCheckEngIdle(u4Which)    
#else       // normal mode
#define IMGRZ_DBG_INC_DRV_FLUSH_COUNT(u4Which)     
#define IMGRZ_DBG_INC_DRV_INT_COUNT(u4Which)       
#define IMGRZ_DBG_INC_MW_FLUSH_COUNT(u4Which)      
#define IMGRZ_DBG_INC_MW_INT_COUNT(u4Which)        
#define IMGRZ_DBG_INC_CM_FLUSH_COUNT(u4Which)      
#define IMGRZ_DBG_INC_CM_INT_COUNT(u4Which)        
#define IMGRZ_DBG_INC_ENG_CRASH_COUNT(u4Which)     
#define IMGRZ_DBG_RECOVER_ENGINE(u4Which)          
#define IMGRZ_DBG_REC_FLUSH_TIME(u4Which)          
#define IMGRZ_DBG_REC_ENG_EXE_MOD(u4Which, mod)    
#define IMGRZ_DBG_REC_CUR_STATUS(u4Which, status)  
#define IMGRZ_DBG_REC_ENG_CLK_SRC(u4Which, clk)    
#define IMGRZ_DBG_CHK_ENG_IDLE(u4Which)            
#endif // #if defined(IMGRZ_DEBUG_MODE)


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

#if defined(IMGRZ_DEBUG_MODE)
extern void IMGRZ_DbgInit(UINT32 u4Which);

extern void IMGRZ_DbgIncDrvFlushCount(UINT32 u4Which);

extern void IMGRZ_DbgIncDrvIntCount(UINT32 u4Which);

extern void IMGRZ_DbgIncMwFlushCount(UINT32 u4Which);

extern void IMGRZ_DbgIncMwIntCount(UINT32 u4Which);

extern void IMGRZ_DbgIncCmFlushCount(UINT32 u4Which);

extern void IMGRZ_DbgIncCmIntCount(UINT32 u4Which);

extern void IMGRZ_DbgIncEngineCrashCount(UINT32 u4Which);

extern void IMGRZ_DbgDumpInfo(UINT32 u4Which);

extern void IMGRZ_DbgRecCurrentStatus(UINT32 u4Which, INT32 i4Status);

extern void IMGRZ_DbgRecoverEngine(UINT32 u4Which);

extern void IMGRZ_DbgRecFlushTime(UINT32 u4Which);

extern void IMGRZ_DbgShowBusyTime(UINT32 u4Which);

extern void IMGRZ_DbgRecEngExecuteMod(UINT32 u4Which, INT32 i4Mod);

extern void IMGRZ_DbgEnum2Text(INT32 i4Enum, CHAR *szText,
    IMGRZ_DBG_ENUM2TEXT_T *prEnum2Text, INT32 i4Stop);

extern void IMGRZ_DbgRecEngClkSrc(UINT32 u4Which, INT32 i4ClkSrc);

extern void IMGRZ_DbgCheckEngIdle(UINT32 u4Which);
#endif // #if defined(IMGRZ_DEBUG_MODE)



#endif // IMGRZ_DEBUG_H


