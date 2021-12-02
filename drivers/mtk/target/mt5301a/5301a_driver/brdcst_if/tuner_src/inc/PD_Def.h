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
/*  $RCSfile: PD_Def.h,v $                                             */
/*                                                                     */
/***********************************************************************/

#ifndef _PD_DEF_H_INCLUDED_
#define _PD_DEF_H_INCLUDED_

/***********************************************************************/
/*              Includes                                               */
/***********************************************************************/
#include "x_os.h"
#include "x_assert.h"
#include "x_printf.h"
#include "x_stl_lib.h"
//#include "x_cli.h"
#include "x_serial.h"                       // for SerPollGetChar
#include "x_bim.h"
#include "x_typedef.h"
#include "x_csemaphore.h"
#include "x_driver_os.h"
#include "x_timer.h"                        // For HAL_GetTime
#include "x_tuner.h"                       // For SIG_POL_T & SIG_SP_T
#include "tuner_if.h"
#ifdef CC_MT5381
#include "x_hal_5381.h"                     // For RISC base addr
#endif
#include "PI_Def.h"
//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>

/***********************************************************************/
/*              Platform-Specific Conditional Compiling                */
/***********************************************************************/
#ifdef __KERNEL__
#define CC_LINUX_KERNEL
#endif
#ifdef __linux__
#define CC_LINUX_PLATFORM
#endif
#ifdef _WINDOWS
#define CC_WINDOWS_PLATFORM
#endif
#ifdef __arm
#define CC_ARM_PLATFORM
#endif

/***********************************************************************/
/*              Public Types                                           */
/***********************************************************************/
/*Jackson remove this, due to this already declare in x_typedef.h*/
#ifndef CC_LINUX_PLATFORM
typedef unsigned char   UCHAR;
typedef unsigned char   UINT8;
typedef unsigned short  UINT16;
typedef unsigned long   UINT32;
typedef char            CHAR;
//typedef signed char     INT8;
//typedef char            INT8;
typedef signed short    INT16;
typedef signed long     INT32;

typedef UINT8           BOOL;
typedef UINT8           BYTE;

typedef UINT16          WORD;
typedef UINT32          DWORD;

typedef UINT8           Data8;
//typedef UINT16          Data16;
typedef INT16           Data16;
typedef UINT32          Data32;
typedef UINT32          Data64;
typedef BOOL            Bool;
#endif

typedef UINT8           U8;
typedef UINT16          U16;
typedef UINT32          U32;
//typedef unsigned int    UINT;
typedef INT8            S8;
typedef INT16           S16;
typedef INT32           S32;


/************************ Constant *************************/
#ifndef EXTERN
#define EXTERN          extern
#endif
#ifndef STATIC
#define STATIC          static
#endif
#ifndef VOID
#define VOID            void
#endif

//#ifdef TRUE
//#undef TRUE
//#endif

//#ifdef FALSE
//#undef FALSE
//#endif

//#ifdef NULL
//#undef NULL
//#endif

//#define TRUE            ((BOOL) 1)          /* Judgment Flag */
//#define FALSE           ((BOOL) 0)
//#define NULL            (0)

/***********************************************************************/
#define FUNC_EXPORT         

/***********************************************************************/
//typedef INT32   (*x_break_fct)(VOID);
//typedef INT32   (*x_break_fct)(VOID *pArg);
//typedef INT32   (*PF_NIM_BREAK_CB_T)(VOID *pArg);
//typedef VOID *PATD_SPECIFIC_CTX;

/***********************************************************************/
/*              Conditional Compiling                                  */
/***********************************************************************/
#if defined(CC_LINUX_PLATFORM) || defined(CC_LINUX_KERNEL)
#define CC_FOR_SDAL
#endif

#define fcADD_ACQ_CHK           1

/***********************************************************************/
/*              Defines                                                */
/***********************************************************************/
#define cMULTI_MSG_MSK          4
#define cMAX_STR_LEN            40

/***********************************************************************/
/*              CPU-Specific                                           */
/***********************************************************************/
#define cRISC_WORD_WID          2
//#define cRISC_WORD_LEN          (sizeof(UINT32) / sizeof(UINT8))
#define cRISC_WORD_LEN          mcBIT(cRISC_WORD_WID)

#ifndef CC_LINUX_KERNEL
// Not on Linux platform
#define WORD_ALIGN              __align(4)
#else
#define WORD_ALIGN
#endif

/***********************************************************************/
/*              OS-Specific                                            */
/***********************************************************************/
#define cREGRESSION_TIME        1
//#define cSHORT_SLEEP_TIME       0           // Consume 5 ms
#define cSHORT_SLEEP_TIME       1           // Consume 5 ms

/***********************************************************************/
#ifndef CC_LINUX_KERNEL
// Not on Linux platform
#define cFMT_UL                 "u"
#define cFMT_IL                 "d"
#define cFMT_XL                 "X"
#else
#define cFMT_UL                 "lu"
#define cFMT_IL                 "ld"
#define cFMT_XL                 "lX"
#endif

/***********************************************************************/

#if 0 //remove to tuner_if.h
/*********** Signal structure *************************************/
typedef struct
{
    INT32       Frequency;                  /* carrier frequency (in Hz)   */
    UINT32      SymbolRate;                 /* Symbol Rate (in Baud)       */
    INT32       QAMSize;                    /* QAM size (16/32/64/128/256) */
} SIGNAL;

typedef struct
{
    INT32       i4FrequencyK;               /* carrier frequency (in Hz)   */
    UINT32      u4SymbolRateK;              /* Symbol Rate (in Baud)       */
    INT32       i4QamSize;                  /* QAM size (16/32/64/128/256) */
} SIGNAL_T;
#endif 

/***********************************************************************/
/*              Common Macro                                           */
/***********************************************************************/
// in us
#define mcSLEEP_US(x)           mcDELAY_MS((UINT32) ((x) / 1000 + 1))
#define mcDELAY_US(x)           HAL_Delay_us((UINT32) (x))
// in ms
//#ifndef CC_LINUX_KERNEL
#define mcDELAY_MS(x)           x_thread_delay((UINT32) (x))
//#define mcDELAY_MS(x)           HAL_Delay_us(((x)*(1000)))  //when verifying 5391 platform, x_thread_delay will case system abort
//#else
//#define mcDELAY_MS(x)           msleep((UINT32) (x))
//#endif
#define mcGET_SYS_TICK()        x_os_get_sys_tick()
#define mcGET_TICK_PERIOD()     x_os_drv_get_tick_period()
//#define mcGET_SYS_TIME(u4Tick)  { u4Tick = mcGET_SYS_TICK(); }
#define mcGET_SYS_TIME(u4Tick)  HAL_GetTime(&u4Tick)
//#define mcGET_DIFF_TIME(u4Diff, u4Start, u4Cur) { u4Diff = (u4Cur - u4Start); }
#define mcGET_DIFF_TIME(u4Diff, u4Start, u4Cur) HAL_GetDeltaTime(&u4Diff, &u4Start, &u4Cur)
//#define mcCONV_SYS_TIME(u4Tick) (u4Tick * mcGET_TICK_PERIOD())
#define mcCONV_SYS_TIME(u4Tick) (u4Tick.u4Micros / 1000 + u4Tick.u4Seconds * 1000)
#define mcCONV_SYS_TIME_US(u4Tick)  (u4Tick.u4Micros + u4Tick.u4Seconds * 1000000)
//#define mcALLOC_MEM(byteCount)  malloc(byteCount)
#define mcALLOC_MEM(byteCount)  x_mem_alloc(byteCount)
//#define mcALLOC_MEM(byteCount)  x_mem_calloc(1, byteCount)
//#define mcFREE_MEM(pBuffer)     free(pBuffer)
#define mcFREE_MEM(pBuffer)     x_mem_free(pBuffer)
#define mcMEMSET                x_memset
#define mcMEMCPY                x_memcpy
#define mcPRINTF                Printf
#define mcSCANF
#define mcKBHIT                 fgKbHit
#define mcGETCHAR_SER           SerPollGetChar
#define mcSTR_CMP               x_strcmp
#define mcSTR_LEN               x_strlen

#ifndef CC_LINUX_KERNEL
// linux platfrom driver should not call MW API cli_get_char
#define mcGETCHAR_CLI           cli_get_char
#endif
#define mcGETCHAR               GetChar
#define mcGETSTR                SerGetS
#define mcGETINT(u2Value)                   \
{                                           \
CHAR szTmp0[cMAX_STR_LEN];                  \
                                            \
    mcGETSTR(szTmp0);                       \
    u2Value = StrToInt(szTmp0);             \
}

#define mcGETHEX(fValue)                    \
{                                           \
CHAR szTmp0[cMAX_STR_LEN];                  \
                                            \
    mcGETSTR(szTmp0);                       \
    fValue = StrToHex(szTmp0);              \
}

#define ROUTINE
//#define TIME_TICK_T                 UINT32
#define TIME_TICK_T                 HAL_TIME_T
//#define TIME_DIFF_T                 UINT32
#define TIME_DIFF_T                 HAL_TIME_T
#define TIME_CONV_T                 UINT32
#define THREAD_T                    HANDLE_T
#define mcTHREAD_CREATE             x_thread_create
#define mcTHREAD_DESTROY            x_thread_exit
#define mcTHREAD_SUSPEND            x_thread_suspend
#define mcTHREAD_RESUME             x_thread_resume
#define mcTHREAD_TIMER              x_timer_start
#define mcCHK_STATE()
#define PRC_T                       static

#define TIMER_T                     HANDLE_T
#define mcTIMER_CREATE(hndl)        x_timer_create(hndl)
#define mcTIMER_START(hndl, u4Prd, u4Flg, vCBFunc, pvArg)   x_timer_start(hndl, u4Prd, u4Flg, vCBFunc, pvArg)
#define mcTIMER_STOP(hndl)          x_timer_stop(hndl)
#define mcTIMER_DESTROY             x_timer_delete

#define SEMA_T                      HANDLE_T
#define mcSEMA_CREATE(sema, init)   x_sema_create(sema, X_SEMA_TYPE_BINARY, init)
#define mcSEMA_DESTROY              x_sema_delete
// Non-blocking
#define mcSEMA_TRYLOCK(sema)        x_sema_lock(sema, X_SEMA_OPTION_NOWAIT)
#define mcSEMA_LOCK(sema)           x_sema_lock(sema, X_SEMA_OPTION_WAIT)
#define mcSEMA_LOCK_TIMEOUT         x_sema_lock_timeout
#define mcSEMA_UNLOCK               x_sema_unlock

#define MUTEX_T                     HANDLE_T
#define mcMUTEX_CREATE(sema, init)  x_sema_create(sema, X_SEMA_TYPE_MUTEX, init)
#define mcMUTEX_DESTROY             x_sema_delete
// Non-blocking
#define mcMUTEX_TRYLOCK(sema)       x_sema_lock(sema, X_SEMA_OPTION_NOWAIT)
#define mcMUTEX_LOCK(sema)          x_sema_lock(sema, X_SEMA_OPTION_WAIT)
#define mcMUTEX_UNLOCK              x_sema_unlock

#define mcCRIT_START                x_crit_start
#define mcCRIT_END                  x_crit_end

/***********************************************************************/
#define DelayBreak(id, x)                   \
{                                           \
    mcDELAY_MS(x);                          \
    if (isBreak(id))                        \
    /*    return (YES);     */              \
    /*    return;           */              \
        break;                              \
}

/***********************************************************************/
#define mcRISC_Reg(RBas, ROfs)      (*((volatile UINT32 *) ((UINT8 *) (RBas) + (ROfs))))
#define mcRISC_Reg16(RBas, ROfs)    (*((volatile UINT16 *) ((UINT8 *) (RBas) + (ROfs))))
#define mcRISC_Reg8(RBas, ROfs)     (*((volatile UINT8 *) (RBas) + (ROfs)))

#define mcRISC_RegAry(RBas, ROfs, RIdx)     mcRISC_Reg(RBas, (ROfs) + ((RIdx) * 4))
#define mcRISC_RegAry16(RBas, ROfs, RIdx)   mcRISC_Reg16(RBas, (ROfs) + ((RIdx) * 2))
#define mcRISC_RegAry8(RBas, ROfs, RIdx)    mcRISC_Reg8(RBas, (ROfs) + (RIdx))

//#define mcVERIFY_ASSERT         VERIFY
#define mcVERIFY_ASSERT         ASSERT

/* For Lint */
#define mcLINT_NULL_PTR(_x_)    /*@-null@*/ _x_ /*@=null@*/
#define mcLINT_IGNORE_RET(_x_)  \
    if (!_x_)   \
    {           \
        ;       \
    }

#define mcLINT_UNUSED(_x_)      (void) _x_

#if 0
#define MAX(X, Y)               (((X) >= (Y)) ? (X) : (Y))
#define MIN(X, Y)               (((X) <= (Y)) ? (X) : (Y))
#endif

/***********************************************************************/
/*              Conditional Compiling Dependent Macro                  */
/***********************************************************************/
#define mcCHECK_DBG_LVL(Lvl)            (mcCHK_MASK(u1DbgLevel, mcMASK(cMULTI_MSG_MSK)) >= Lvl)

#define mcCHECK_HW_MSG                  mcCHECK_DBG_LVL(1)
#define mcCHECK_DBG_MSG1                mcCHECK_DBG_LVL(1)
#define mcCHECK_DBG_MSG                 mcCHECK_DBG_LVL(2)
#define mcCHECK_DBG_MSG2                mcCHECK_DBG_LVL(3)
#define mcCHECK_DBG_MSG3                mcCHECK_DBG_LVL(4)
//#define mcCHECK_USER_MSG              mcCHECK_DBG_LVL(0)


#ifndef PFXMSG_SUPPORT		
#define PFXMSG_SUPPORT 1	 /* may override from makefile */
#endif
#ifndef PFXMSG_OFF_BY_DEFAULT	
#define PFXMSG_OFF_BY_DEFAULT 0 /* may override from makefile */
#endif

#define MSG_SHOW_TOP_PREFIX 1  //"[CID]"

#if MSG_SHOW_TOP_PREFIX
#define TOP_PREFIX_MSG	"[CID]"
#else
#define TOP_PREFIX_MSG	/*empty*/
#endif


#define STR1(x)	#x
#define STR(x)	STR1(x)


#ifndef PFXMSG
//#error please add  DEFINES += -DPFXMSG=XXXX in Makefile or define in .c or .h file
#define BANNERMSG  TOP_PREFIX_MSG "[" __FILE__ "]"
#else
#define BANNERMSG  TOP_PREFIX_MSG "[" STR(PFXMSG) "]"
#endif

#if PFXMSG_SUPPORT
EXTERN UCHAR u1DbgPfxMsgEnable;

#define PFXMSG_PRINT()  mcPRINTF(BANNERMSG)  /*for user manual print*/
#define PFXMSG_DEF() 	u1DbgPfxMsgEnable=(PFXMSG_OFF_BY_DEFAULT?FALSE:TRUE)
#define PFXMSG_OFF() u1DbgPfxMsgEnable=FALSE
#define PFXMSG_ON() 	u1DbgPfxMsgEnable=TRUE  /* for future cli */

#define PrintPrefixMsg(x) if(u1DbgPfxMsgEnable) mcPRINTF(x)
#else
#define PrintPrefixMsg(x)
#endif



#define mcDBG_LVL_MSG(Lvl, _x_ )        \
{                                       \
    if (mcCHECK_DBG_LVL(Lvl))           \
    { \
        PrintPrefixMsg(BANNERMSG);      \
	mcPRINTF _x_;                   \
    }	\
}

#define mcDBG_NBIT_MSG(nBit, _x_)    \
{                                                            \
       if (u1DbgLevel & (1<<nBit))           \
     	mcPRINTF _x_;                                \
}

#define mcSHOW_POP_MSG                  mcSHOW_HW_MSG
#define mcSHOW_HW_MSG(_x_)              mcDBG_LVL_MSG(1, _x_)
#define mcSHOW_DBG_MSG1(_x_)            mcDBG_LVL_MSG(1, _x_)
#define mcSHOW_DBG_MSG(_x_)             mcDBG_LVL_MSG(2, _x_)
#define mcSHOW_DBG_MSG2(_x_)            mcDBG_LVL_MSG(3, _x_)
#define mcSHOW_DBG_MSG3(_x_)            mcDBG_LVL_MSG(3, _x_)
#define mcSHOW_DBG_MSG4(_x_)            mcDBG_LVL_MSG(4, _x_)
//#define mcSHOW_USER_MSG(_x_)            mcDBG_LVL_MSG(1, _x_)
#define mcSHOW_USER_MSG(_x_)            mcPRINTF _x_

#define mcSHOW_DRVERR_MSG(_x_)          mcDBG_LVL_MSG(1, _x_)
#define mcSHOW_OSAI_MSG(_x_)            mcDBG_LVL_MSG(2, _x_)
#define mcSHOW_OSAIOK_MSG(_x_)          mcDBG_LVL_MSG(4, _x_)
#define mcSHOW_DRVAPI_MSG(_x_)          mcDBG_LVL_MSG(2, _x_)
#define mcSHOW_DRVAPIOK_MSG(_x_)        mcDBG_LVL_MSG(4, _x_)
#define mcSHOW_DRVLOCK_MSG(_x_)         mcDBG_LVL_MSG(4, _x_)
#define mcSHOW_API_MSG(_x_)                 mcDBG_NBIT_MSG(4, _x_) // Enable bit 4                 

/***********************************************************************/
/*              External declarations                                  */
/***********************************************************************/
//EXTERN  x_break_fct isBreak;
EXTERN  x_break_fct isBreak;
EXTERN  UINT8       u1DbgLevel;

BOOL fgKbHit(CHAR *cValue);
#ifndef CC_LINUX_KERNEL
// linux platfrom driver should not call MW API cli_get_char
CHAR cli_get_char(VOID);
#endif
CHAR GetChar(VOID);

#endif  // _PD_DEF_H_INCLUDED_
