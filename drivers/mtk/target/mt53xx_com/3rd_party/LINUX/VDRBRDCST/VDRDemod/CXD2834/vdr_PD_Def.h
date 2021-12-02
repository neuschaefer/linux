/***********************************************************************/
/*  Copyright (c) 2007, MediaTek Inc.                                  */
/*  All rights reserved.                                               */
/*                                                                     */
/*  Unauthorized use, practice, perform, copy, distribution,           */
/*  reproduction, or disclosure of this information in whole or in     */
/*  part is prohibited.                                                */
/***********************************************************************/
/***********************************************************************/
/*  No Warranty                                                        */
/*  Except as may be otherwise agreed to in writing, no warranties of  */
/*  any kind, whether express or implied, are given by MTK with        */
/*  respect to any MTK Deliverables or any use thereof, and MTK        */
/*  Deliverables are provided on an "AS IS" basis. MTK hereby          */
/*  expressly disclaims all such warranties, including any implied     */
/*  warranties of merchantability, non-infringement and fitness for a  */
/*  particular purpose and any warranties arising out of course of     */
/*  performance, course of dealing or usage of trade. Parties further  */
/*  acknowledge that Company may, either presently and/or in the       */
/*  future, instruct MTK to assist it in the development and the       */
/*  implementation, in accordance with Company's designs, of certain   */
/*  softwares relating to Company's product(s) (the "Services").       */
/*  Except as may be otherwise agreed to in writing, no warranties of  */
/*  any kind, whether express or implied, are given by MTK with        */
/*  respect to the Services provided, and the Services are provided on */
/*  an "AS IS" basis. Company further acknowledges that the Services   */
/*  may contain errors, that testing is important and Company is       */
/*  solely responsible for fully testing the Services and/or           */
/*  derivatives thereof before they are used, sublicensed or           */
/*  distributed. Should there be any third party action brought        */
/*  against MTK, arising out of or relating to the Services, Company   */
/*  agree to fully indemnify and hold MTK harmless. If the parties     */
/*  mutually agree to enter into or continue a business relationship   */
/*  or other arrangement, the terms and conditions set forth hereunder */
/*  shall remain effective and, unless explicitly stated otherwise,    */
/*  shall prevail in the event of a conflict in the terms in any       */
/*  agreements entered into between the parties.                       */
/***********************************************************************/
/***********************************************************************/
/*                                                                     */
/*  FILE DESCRIPTION                                                   */
/*                                                                     */
/*  $RCSfile: PD_Def.h,v $                                             */
/*                                                                     */
/***********************************************************************/

#ifndef _vdr_PD_DEF_H_INCLUDED_
#define _vdr_PD_DEF_H_INCLUDED_

/***********************************************************************/
/*              Includes                                               */
/***********************************************************************/
#include "x_os.h"
#include "x_assert.h"
#include "x_printf.h"
#include "x_stl_lib.h"
#include "x_serial.h"                      
#include "x_bim.h"
#include "x_typedef.h"
#include "x_csemaphore.h"
#include "x_driver_os.h"
#include "x_timer.h"                        
#include "x_tuner.h"                       
#include "tuner_if.h"


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

#ifndef CC_LINUX_PLATFORM
typedef unsigned char   UCHAR;
typedef unsigned char   UINT8;
typedef unsigned short  UINT16;
typedef unsigned long   UINT32;
typedef char            CHAR;
typedef signed short    INT16;
typedef signed long     INT32;
typedef UINT8           BOOL;
typedef UINT8           BYTE;
typedef UINT16          WORD;
typedef UINT32          DWORD;
typedef UINT8           Data8;
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


/***********************************************************************/
/*              Common Macro                                           */
/***********************************************************************/
#define mcSLEEP_US(x)           mcDELAY_MS((UINT32) ((x) / 1000 + 1))
#define mcDELAY_US(x)           HAL_Delay_us((UINT32) (x))
#define mcDELAY_MS(x)           x_thread_delay((UINT32) (x))
#define mcGET_SYS_TICK()        x_os_get_sys_tick()
#define mcGET_TICK_PERIOD()     x_os_drv_get_tick_period()
#define mcGET_SYS_TIME(u4Tick)  HAL_GetTime(&u4Tick)
#define mcGET_DIFF_TIME(u4Diff, u4Start, u4Cur) HAL_GetDeltaTime(&u4Diff, &u4Start, &u4Cur)
#define mcCONV_SYS_TIME(u4Tick) (u4Tick.u4Micros / 1000 + u4Tick.u4Seconds * 1000)
#define mcCONV_SYS_TIME_US(u4Tick)  (u4Tick.u4Micros + u4Tick.u4Seconds * 1000000)
#define mcALLOC_MEM(byteCount)  x_mem_alloc(byteCount)
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
#define TIME_TICK_T                 HAL_TIME_T
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
EXTERN  x_break_fct isBreak;
EXTERN  UINT8       u1DbgLevel;
#endif  // _PD_DEF_H_INCLUDED_
