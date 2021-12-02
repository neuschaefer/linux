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
 * $Date  $
 * $RCSfile: util.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#ifndef _UTIL_H_
#define _UTIL_H_

#include "typedef.h"
#include "x_util.h"
#include "debug.h"
#include "x_timer.h"

extern void vUtInitVar(void) ;


#define DELAY_MACRO 1
#if DELAY_MACRO
#include "x_os.h"

/* some benifit, 1.  value can >255 , 2. *2 or *10  *1000 can be calculate in compiler time */
#define vUtDelay2us(value)	HAL_Delay_us(2*(value))
#define vUtDelay1ms(value)	(((value)<=10)?HAL_Delay_us(1000*(value)):x_thread_delay(value))
#define vUtDelay10ms(value)	x_thread_delay(10*(value))
#define vUtDelay1s(value)		x_thread_delay(1000*(value))

#else
extern void vUtDelay2us(UINT8 bValue) ;
extern void vUtDelay1ms(UINT8 bValue) ;
extern void vUtDelay10ms(UINT8 bValue) ;
extern void vUtDelay1s(UINT8 bValue) ;
#endif

enum
{
    SV_OUT_VSYNC_TIME=0,
    SV_MLOOP_TIME0,    
    SV_MLOOP_TIME1,    
    SV_TEST_M,
    SV_MAX_SYS_MODULE
};


#if  CC_MEASURE_SYS_TIME
#define TIME_MODULE_STR_LEN                  21

extern HAL_TIME_T rStartMeasureTime[SV_MAX_SYS_MODULE];
extern HAL_TIME_T rEndMeasureTime[SV_MAX_SYS_MODULE];
extern UINT8 _bSysMeasueTimeEn;
extern CHAR _aszTimeModuleStr[SV_MAX_SYS_MODULE][TIME_MODULE_STR_LEN];



#define SYS_MEASURE_TIME_START(module) \
{\
    if(_bSysMeasueTimeEn && (1 << module))\
    {\
    HAL_GetTime(&(rStartMeasureTime[module]));\
    }\
}

#define SYS_MEASURE_TIME_END(module) \
{\
     if(_bSysMeasueTimeEn && (1 << module))\
     {\
    HAL_GetTime(&(rEndMeasureTime[module]));\
    }\
}

#define SYS_PRINT_TIME(module) \
{\
    HAL_TIME_T  rDeltaTime;\
    if(_bSysMeasueTimeEn & (1 << module))\
    {\
        HAL_GetDeltaTime(&rDeltaTime, &rStartMeasureTime[module], &rEndMeasureTime[module]);\
        Printf("[%s] %d.%d s\n", _aszTimeModuleStr[module], rDeltaTime.u4Seconds,  (rDeltaTime.u4Micros / 1000));\
    }\
}

#else
#define SYS_MEASURE_TIME_START(module)
#define SYS_MEASURE_TIME_END(module)
#define SYS_PRINT_TIME(module)
#endif


#define PRINT_TIMESTAMP_MSG_N(Title, String)                          \
{                                                                   \
    HAL_TIME_T _rTime;                                              \
    HAL_GetTime(&_rTime);                                           \
    Printf(Title);                                                  \
    Printf(" (%06d:%03d) ", _rTime.u4Seconds, _rTime.u4Micros/1000);\
    Printf(String);                                                 \
}


#endif
