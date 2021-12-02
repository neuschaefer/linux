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
/***********************************************************************/
/*                                                                     */
/*  FILE DESCRIPTION                                                   */
/*                                                                     */
/*  $RCSfile: PD_Def_dbg.h,v $                                         */
/*                                                                     */
/***********************************************************************/

#ifndef _PD_DEF_DBG_H_INCLUDED_
#define _PD_DEF_DBG_H_INCLUDED_

/***********************************************************************/
/*              Includes                                               */
/***********************************************************************/
#include "eeprom_if.h"
#include "x_pinmux.h"
#include "x_stl_lib.h"
/***********************************************************************/
/*              Conditional Compiling                                  */
/***********************************************************************/
#define fcINTERNAL_TEST         0
#define fcERROR_CHECK           1
#define fcCONV_RISC_ENDIAN      0
#define fcADD_HAL_LOCK          1

/***********************************************************************/
/*              Conditional Compiling Dependent Included Files         */
/***********************************************************************/
#include "PI_Def.h"

/***********************************************************************/
//#ifdef CC_FOR_SDAL
//#define fcTUNE_NOT_START        1
//#define fcTUNE_NOT_START        0
//#else
//    #ifdef CC_MT5381
//#define fcTUNE_NOT_START        1
//    #else
//#define fcTUNE_NOT_START        0
//    #endif
//#endif
#define fcTUNE_NOT_START        1

//#if defined(CC_MT5381) && !defined(CC_FOR_SDAL)
//#define fcADD_DIAG_INFO   // always defined in tuner_src/Makefile
//#define fcADD_DIAG_INFO         1
//#define fcADD_DIAG_INFO         0
//#define fcADD_MW_DEMOD_INFO     1 // move to below (in #ifndef CC_FOR_SDAL)
//#else
//#define fcADD_DIAG_INFO         0
//#define fcADD_MW_DEMOD_INFO     0
//#endif


#ifdef CC_FOR_SDAL
//#define fcADD_DIAG_INFO         0
 #ifdef fcADD_DIAG_INFO
 #undef fcADD_DIAG_INFO
 #endif
 #define fcADD_MW_DEMOD_INFO     0
#else
 #define fcADD_MW_DEMOD_INFO     1
#endif
/***********************************************************************/
/*              Defines                                                */
/***********************************************************************/
#define cpMSG_MONI              (cMULTI_MSG_MSK + 0)
#define cpMSG_INT               (cMULTI_MSG_MSK + 1)

/***********************************************************************/
/*              CPU-Specific                                           */
/***********************************************************************/
/***********************************************************************/
/*              OS-Specific                                            */
/***********************************************************************/
/***********************************************************************/
/*              Public Types                                           */
/***********************************************************************/
/***********************************************************************/
/*              Common Macro                                           */
/***********************************************************************/
#define mcCALC_RISC_ADDR_BASE(u2RegAddr)    \
    (u4RegSetting = (u2RegAddr >> cRISC_WORD_WID))  /* Word aligned */

#define mcCALC_RISC_ADDR_CNT(u2ByteCount, u1ByteOfst)           \
{                                                               \
    u2CurByteCount = u2ByteCount;                               \
    if (u2CurByteCount > (cRISC_WORD_LEN - u1ByteOfst))         \
        u2CurByteCount = (cRISC_WORD_LEN - u1ByteOfst);         \
}

#define mcCALC_RISC_ADDR(u2RegAddr, u2ByteCount)                \
{                                                               \
    mcCALC_RISC_ADDR_BASE(u2RegAddr);                           \
    u1ByteOfst = (u2RegAddr & mcMASK(cRISC_WORD_WID));  /* Byte offset */   \
                                                                \
    mcCALC_RISC_ADDR_CNT(u2ByteCount, u1ByteOfst);              \
/*    u2ByteCount -= u2CurByteCount; */                         \
}

/***********************************************************************/
//#define mcSEMA_DBG_MSG          mcPRN_MSG_INT
#define mcSEMA_DBG_MSG(fmt...)

#define mcSEMA_LOCK_DBG(sema)                           \
{                                                       \
    mcSEMA_DBG_MSG(" [ _L_%s/%s-%d ]", __FILE__, __FUNCTION__, __LINE__);   \
    mcSEMA_LOCK(sema);                                  \
    mcSEMA_DBG_MSG(" [ _K_%s/%s-%d ]", __FILE__, __FUNCTION__, __LINE__);   \
}

#define mcSEMA_UNLOCK_DBG(sema)                         \
{                                                       \
    mcSEMA_DBG_MSG(" [ _U_%s/%s-%d ]", __FILE__, __FUNCTION__, __LINE__);   \
    mcSEMA_UNLOCK(sema);                                \
}

#if fcADD_HAL_LOCK
#define mcSEMA_HAL_DBG_MSG      mcPRN_MSG_INT
//#define mcSEMA_HAL_DBG_MSG(fmt...)

// Avoid re-entry
#define mcSEMA_LOCK_HAL(sema)                                               \
{                                                                           \
    mcSEMA_HAL_DBG_MSG(" [ _L_%s/%s-%d ]", __FILE__, __FUNCTION__, __LINE__);    \
    mcSEMA_LOCK(sema);                                                      \
    mcSEMA_HAL_DBG_MSG(" [ _K_%s/%s-%d ]", __FILE__, __FUNCTION__, __LINE__);    \
}
#define mcSEMA_UNLOCK_HAL(sema)                                             \
{                                                                           \
    mcSEMA_HAL_DBG_MSG(" [ _U_%s/%s-%d ]", __FILE__, __FUNCTION__, __LINE__);    \
    mcSEMA_UNLOCK(sema);                                                    \
}
#else
#define mcSEMA_LOCK_HAL(sema)
#define mcSEMA_UNLOCK_HAL(sema)
#endif

/***********************************************************************/
//#define mcSTR_2_INT             StrToInt
//#define mcSTR_2_INT(_x_)        axtoi((CHAR *) (_x_))
#define mcSTR_2_INT(_x_)        StrToInt((CHAR *) (_x_))

#define mcREAD_CHK_INPUT(u1ArgOfst)                             \
{                                                               \
    u2RegAddr = mcSTR_2_INT(aszArgv[u1ArgOfst]);                \
                                                                \
    u1ByteCount = 1;                                            \
    if (i4Argc > (u1ArgOfst + 1))                               \
        u1ByteCount = mcSTR_2_INT(aszArgv[u1ArgOfst + 1]);      \
}

#define mcREAD_CHK_LIMIT(u1Max)                                 \
{                                                               \
    if ((u2RegAddr + u1ByteCount - 1) > u1Max)                  \
        u1ByteCount = u1Max - u2RegAddr + 1;                    \
}

#define mcREAD_SHOW_VALUE_FUNC(u1Func, psDemodCtx, mcChkFunc)   \
{                                                               \
    for (ii = 0; u1ByteCount; )                                 \
    {                                                           \
    UINT8   jj;                                                 \
                                                                \
        mcChkFunc(u1Func, psDemodCtx);                          \
                                                                \
        for (jj = 0; jj < u1ByteCountRd; jj ++, ii ++)          \
        {                                                       \
            if ((ii % 8) == 0)                                  \
                mcSHOW_USER_MSG(("%03X: ", u2RegAddr + ii));    \
            mcSHOW_USER_MSG((" %02X", au1Data[jj]));            \
            if ((ii % 8) == 7)                                  \
                mcSHOW_USER_MSG(("\n"));                        \
        }                                                       \
        u1ByteCount -= u1ByteCountRd;                           \
    }                                                           \
    mcSHOW_USER_MSG(("\n"));                                    \
}

#define mcREAD_MULTI_FUNC(u1Func, psDemodCtx)                   \
{                                                               \
    u1ByteCountRd = u1Func(psDemodCtx, u2RegAddr + ii, au1Data, u1ByteCount);   \
    if (u1ByteCountRd == 0)                                     \
        break;                                                  \
}

#define mcREAD_SHOW_VALUE_MULTI(u1Func, psDemodCtx)             \
    mcREAD_SHOW_VALUE_FUNC(u1Func, psDemodCtx, mcREAD_MULTI_FUNC)

#define mcREAD_SHOW_VALUE_ONCE(u1Func, psDemodCtx, au1Data)     \
{                                                               \
    for (ii = 0; ii < u1ByteCount; ii ++)                       \
    {                                                           \
        if ((ii % 8) == 0)                                      \
            mcSHOW_USER_MSG(("%03X: ", u2RegAddr + ii));        \
        if (u1Func(psDemodCtx, u2RegAddr + ii, au1Data, 1) == 0)\
            break;                                              \
                                                                \
        mcSHOW_USER_MSG((" %04X", au1Data[0]));                 \
        if ((ii % 8) == 7)                                      \
            mcSHOW_USER_MSG(("\n"));                            \
    }                                                           \
    mcSHOW_USER_MSG(("\n"));                                    \
}

#define mcREAD_CHK_NORM(u1Max)

#define mcWRITE_CHK_INPUT(u1ArgOfst, mcChkFunc, u1Max, au1Data) \
{                                                               \
    u1ByteCount = (i4Argc - (u1ArgOfst + 1));                   \
    if (u1ByteCount > cMAX_READ_NUM)                            \
        u1ByteCount = cMAX_READ_NUM;                            \
    mcChkFunc(u1Max);                                           \
                                                                \
    for (ii = 0; ii < u1ByteCount; ii ++)                       \
        au1Data[ii] = mcSTR_2_INT(aszArgv[ii + (u1ArgOfst + 1)]);   \
}

#define mcWRITE_SHOW_VALUE(au1Data)                             \
{                                                               \
    for (ii = 0; ii < u1ByteCount; ii ++)                       \
        mcSHOW_USER_MSG((" %04X", au1Data[ii]));                \
    mcSHOW_USER_MSG(("\n"));                                    \
}

#define mcWRITE_SET_VALUE_ONE_SEQ(u1ArgOfst, u1Func, psDemodCtx, mcChkFunc, u1Max)  \
{                                                               \
    if (i4Argc > (u1ArgOfst))                                   \
        u2RegAddr = mcSTR_2_INT(aszArgv[u1ArgOfst]);            \
                                                                \
    if (i4Argc > (u1ArgOfst + 1))                               \
    {                                                           \
        mcWRITE_CHK_INPUT(u1ArgOfst, mcChkFunc, u1Max, au1Data);\
                                                                \
        u1Func(psDemodCtx, u2RegAddr, au1Data, u1ByteCount);    \
        mcWRITE_SHOW_VALUE(au1Data);                            \
    }                                                           \
}

#define mcWRITE_SET_VALUE_ONCE(u1ArgOfst, u1Func, psDemodCtx, au1Data, mcChkFunc, u1Max)    \
{                                                               \
    if (i4Argc > (u1ArgOfst))                                   \
        u2RegAddr = mcSTR_2_INT(aszArgv[u1ArgOfst]);            \
                                                                \
    if (i4Argc > (u1ArgOfst + 1))                               \
    {                                                           \
        mcWRITE_CHK_INPUT(u1ArgOfst, mcChkFunc, u1Max, au1Data);\
                                                                \
        for (ii = 0; ii < u1ByteCount; ii ++)                   \
        {                                                       \
            u1Func(psDemodCtx, u2RegAddr + ii, au1Data + ii, 1);\
        }                                                       \
        mcSHOW_USER_MSG(("Write %03X =", u2RegAddr));           \
        mcWRITE_SHOW_VALUE(au1Data);                            \
    }                                                           \
}

#define mcWRITE_SET_VALUE_MULTI(u1ArgOfst, u1Func, psDemodCtx, mcChkFunc, u1Max)    \
{                                                               \
    if (i4Argc > (u1ArgOfst))                                   \
        u2RegAddr = mcSTR_2_INT(aszArgv[u1ArgOfst]);            \
                                                                \
    if (i4Argc > (u1ArgOfst + 1))                               \
    {                                                           \
        mcWRITE_CHK_INPUT(u1ArgOfst, mcChkFunc, u1Max, au1Data);\
                                                                \
        if (u1ByteCount > cMAX_READ_NUM)                        \
    	    u1ByteCount = cMAX_READ_NUM;                        \
                                                                \
        for (ii = 0; u1ByteCount && (ii <= cMAX_READ_NUM); )                             \
        {                                                       \
            u1ByteCountRd = u1Func(psDemodCtx, u2RegAddr + ii, au1Data + ii, u1ByteCount);  \
            if (u1ByteCountRd == 0)                             \
                break;                                          \
            u1ByteCount -= u1ByteCountRd;                       \
            ii += u1ByteCountRd;                                \
        }                                                       \
                                                                \
        u1ByteCount = ii;                                       \
        mcSHOW_USER_MSG(("Write %03X =", u2RegAddr));           \
        mcWRITE_SHOW_VALUE(au1Data);                            \
    }                                                           \
}

#define mcWRITE_SET_VALUE_LIMIT(u1ArgOfst, u1Func, psDemodCtx, u1Max)   \
    mcWRITE_SET_VALUE_MULTI(u1ArgOfst, u1Func, psDemodCtx, mcREAD_CHK_LIMIT, u1Max)

#define mcWRITE_SET_VALUE_NORM(u1ArgOfst, u1Func, psDemodCtx)   \
    mcWRITE_SET_VALUE_MULTI(u1ArgOfst, u1Func, psDemodCtx, mcREAD_CHK_NORM, 0)

#define mcESTIMATE_TIME(_x_)                                                \
{                                                                           \
    mcGET_SYS_TIME(u4TickCurMeasure);                                       \
    mcGET_DIFF_TIME(u4TickDiffCnt0, u4TickStartMeasure, u4TickCurMeasure);  \
    mcGET_DIFF_TIME(u4TickDiffCnt1, u4PrevStartTime, u4TickCurMeasure);     \
    mcSHOW_DRVTIME_MSG(_x_);                                                \
    mcSHOW_DRVTIME_MSG(("(%4"cFMT_UL"ms +%3"cFMT_UL")\n", mcCONV_SYS_TIME(u4TickDiffCnt0),  \
                        mcCONV_SYS_TIME(u4TickDiffCnt1)));                  \
    mcGET_SYS_TIME(u4PrevStartTime);                                        \
}

/***********************************************************************/
/*              For Monitor Thread                                     */
/***********************************************************************/
/* #if fcENABLE_MONITOR == 2 */
    #if 0
STATIC BOOL GetMonitorState(FE_MONI_TASK_T *psMoniTask)
{
BOOL            fgMonitorState;
CRIT_STATE_T    hCritState;

    hCritState = mcCRIT_START();

    fgMonitorState = psMoniTask->fgMonitorActive;

    mcCRIT_END(hCritState);

    return (fgMonitorState);
}

STATIC VOID SetMonitorState(FE_MONI_TASK_T *psMoniTask, BOOL fgMonitorActive)
{
CRIT_STATE_T hCritState;

    hCritState = mcCRIT_START();

    psMoniTask->fgMonitorActive = fgMonitorActive;

    mcCRIT_END(hCritState);
}
    #endif
/* #endif */


/***********************************************************************/
/*              Conditional Compiling Dependent Macro                  */
/***********************************************************************/
#define mcCHECK_MULTI_MSG(Msk)            (mcTEST_BIT(u1DbgLevel, Msk))

#define mcDBG_LVL_PRN(Lvl, fmt...)      \
{                                       \
    if (mcCHECK_DBG_LVL(Lvl))           \
    {                                   \
        mcPRINTF(fmt);                  \
    }                                   \
}

#define mcMULTI_MSG_PRN(Msk, fmt...)    \
{                                       \
    if (mcCHECK_MULTI_MSG(Msk))         \
        mcPRINTF(fmt);                  \
}

#define mcPRN_HW_MSG(fmt...)            mcDBG_LVL_PRN(1, fmt)
#define mcPRN_DBG_MSG1(fmt...)          mcDBG_LVL_PRN(1, fmt)
#define mcPRN_DBG_MSG(fmt...)           mcDBG_LVL_PRN(2, fmt)
#define mcPRN_DBG_MSG2(fmt...)          mcDBG_LVL_PRN(3, fmt)
//#define mcPRN_USER_MSG(fmt...)          mcDBG_LVL_PRN(1, fmt)
#define mcPRN_USER_MSG(fmt...)          mcPRINTF(fmt)

#define mcCHECK_MSG_MONI                mcCHECK_MULTI_MSG(cpMSG_MONI)
#define mcCHECK_MSG_INT                 mcCHECK_MULTI_MSG(cpMSG_INT)

#define mcPRN_MSG_MONI(fmt...)          mcMULTI_MSG_PRN(cpMSG_MONI, fmt)
#define mcPRN_MSG_INT(fmt...)           mcMULTI_MSG_PRN(cpMSG_INT, fmt)

/***********************************************************************/
#if fcCONV_RISC_ENDIAN
#define mcRISC_BYTE_MASK(u2CurByteCount, u1ByteOfst, cpRISC_BYTE3_MASK) \
    mcMASKS(u2CurByteCount, (cpRISC_BYTE3_MASK + 1) - u2CurByteCount - u1ByteOfst)

#define mcRISC_ENDIAN_READ(pu1Buffer, u1ByteOfst, u2CurByteCount)   \
{                                                                   \
    u4RegValue <<= (u1ByteOfst * 8);                                \
    for (ii = 0; ii < u2CurByteCount; ii ++)                        \
        (pu1Buffer)[ii] = mcLOW_BYTE(u4RegValue >> ((3 - ii) * 8)); \
}

#define mcRISC_ENDIAN_WRITE(pu1Buffer, u1ByteOfst, u2CurByteCount)  \
{                                                                   \
/*    u4RegValue = pu1Buffer[0]; */                                 \
/*    u4RegValue = *((UINT32 *) pu1Buffer) >> (u1ByteOfst * 8); */  \
    u4RegValue = 0;                                                 \
    for (ii = 0; ii < u2CurByteCount; ii ++)                        \
        u4RegValue |= ((pu1Buffer)[ii] << ((3 - ii) * 8));          \
    /*  ((UINT8 *) (&u4RegValue))[3 - u1ByteOfst - ii] = (pu1Buffer)[ii]; */    \
    u4RegValue >>= (u1ByteOfst * 8);                                \
}

#else

#define mcRISC_BYTE_MASK(u2CurByteCount, u1ByteOfst, cpRISC_BYTE0_MASK) \
    mcMASKS(u2CurByteCount, (cpRISC_BYTE0_MASK) + u1ByteOfst)

#define mcRISC_ENDIAN_READ(pu1Buffer, u1ByteOfst, u2CurByteCount)   \
{                                                                   \
    u4RegValue >>= (u1ByteOfst * 8);                                \
    for (ii = 0; ii < u2CurByteCount; ii ++)                        \
        (pu1Buffer)[ii] = mcLOW_BYTE(u4RegValue >> (ii * 8));       \
    /*  (pu1Buffer)[ii] = ((UINT8 *) (&u4RegValue))[ii]; */         \
}

#define mcRISC_ENDIAN_WRITE(pu1Buffer, u1ByteOfst, u2CurByteCount)  \
{                                                                   \
    u4RegValue = 0;                                                 \
    for (ii = 0; ii < u2CurByteCount; ii ++)                        \
        u4RegValue |= ((pu1Buffer)[ii] << (ii * 8));                \
    u4RegValue <<= (u1ByteOfst * 8);                                \
}
#endif

#endif  // _PD_DEF_DBG_H_INCLUDED_
