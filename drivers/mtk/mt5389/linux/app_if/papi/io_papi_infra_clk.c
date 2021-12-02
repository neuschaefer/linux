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
 * $RCSfile: io_papi_infra_clk.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_papi_infra_led.c
 *  PAPI kernel driver implementation: infra_led
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_linux.h"
#include "x_typedef.h"
#include "x_assert.h"
#include "x_timer.h"
#include <linux/papi_ioctl.h>	
#include <linux/cb_data.h>
#include "papi.h"


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
/// Common error code
#define PAPI_INFRA_CLK_FAIL                     (-0x800)



//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

/// Copy data from user space to kernel space
#define COPY_FROM_USER(dst, src, size) \
    if (!access_ok(VERIFY_READ, (void __user *)(src), (size))) \
    { \
        return PAPI_INFRA_CLK_FAIL  ; \
    } \
    if (copy_from_user((dst), (void __user *)(src), (size))) \
    { \
        return PAPI_INFRA_CLK_FAIL  ; \
    }

/// Copy data from kernel space to user space
#define COPY_TO_USER(dst, src, size) \
    if (!access_ok(VERIFY_WRITE, (void __user *)(dst), (size))) \
    { \
        return PAPI_INFRA_CLK_FAIL  ; \
    } \
    if (copy_to_user((void __user *)(dst), (src), (size))) \
    { \
        return PAPI_INFRA_CLK_FAIL  ; \
    }


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static Bool _fgAlarmIsSet = False;
static Bool _fgClockIsSet = False;
static DateTime _rLocalTimeOffset = 0;
static BOOL _fgIsrInstalled = FALSE;


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static void _AlarmHandler(void)
{
    PAPI_INFRA_CLK_CB_T rCb;
    rCb.u4Type = CLK_CB_ONALARM;    
    _CB_PutEvent(CB_PAPI_INFRA_CLK_TRIGGER, sizeof(PAPI_INFRA_CLK_CB_T), (void *)&rCb);
}

static int _io_GetAlarm(unsigned long arg)
{
    UINT64 u8Utc;
    DateTime rTime;
    RTC_GetAlertTime(&u8Utc);
    rTime = (DateTime)u8Utc;
    COPY_TO_USER((void*)arg, &rTime, sizeof(int));   
    
    return 0;
}

static int _io_AlarmIsSet(unsigned long arg)
{
    COPY_TO_USER((void*)arg, &_fgAlarmIsSet, sizeof(int));   
    
    return 0;
}

static int _io_SetAlarm(unsigned long arg)
{
    IOCTL_2ARG_T rArg;
    DateTime rTime;
    UINT64 u8Utc;
    
    // Copy arguments from user space
    COPY_FROM_USER(&rTime, arg, sizeof (int));
    
    u8Utc = rTime;
    
    if(!_fgIsrInstalled)
    {
        RTC_HookAlertHandler(_AlarmHandler);
        _fgIsrInstalled = TRUE;
    }

    RTC_SetAlertTime(u8Utc);
    
    return 0;
}

static int _io_CancelAlarm(unsigned long arg)
{
    _fgAlarmIsSet = False;
    return 0;
}

static int _io_ClockIsSet(unsigned long arg)
{
    COPY_TO_USER((void*)arg, &_fgClockIsSet, sizeof(int));   
    
    return 0;
}

static int _io_SetClock(unsigned long arg)
{
    IOCTL_2ARG_T rArg;
    UINT64 u8Utc;
    UINT32 u4Utc, u4TimeOffset;
    PAPI_INFRA_CLK_CB_T rCb;
    	
    // Copy arguments from user space
    COPY_FROM_USER(&rArg, arg, sizeof (IOCTL_2ARG_T));

    RTC_GetTimeDate(&u8Utc);
    rCb.u4Org = (UINT32)u8Utc;   
    
    u4Utc = rArg.au4Arg[0];
    u4TimeOffset = rArg.au4Arg[1];        
    rCb.u4New = u4Utc + u4TimeOffset;    

    u8Utc = (UINT64)rCb.u4New;
    RTC_SetTimeDate(&u8Utc);

    if(rCb.u4Org != rCb.u4New)
    {
        rCb.u4Type = CLK_CB_ONCLOCKCHANGED;    
        _CB_PutEvent(CB_PAPI_INFRA_CLK_TRIGGER, sizeof(PAPI_INFRA_CLK_CB_T), (void *)&rCb);
    }

    return 0;
 
}

static int _io_GetLocalTime(unsigned long arg)
{
    UINT64 u8Utc;
    DateTime rTime;
    RTC_GetTimeDate(&u8Utc);
    rTime = (DateTime)u8Utc + _rLocalTimeOffset;
    COPY_TO_USER((void*)arg, &rTime, sizeof(int));   
    
    return 0;
}

static int _io_GetLocalTimeOffset(unsigned long arg)
{
    COPY_TO_USER((void*)arg, &_rLocalTimeOffset, sizeof(int));   
    
    return 0;
}

static int _io_RunDuringStandby(unsigned long arg)
{
    // Always run during standby
    return 0;
}

static int _io_SecondsSinceWakeup(unsigned long arg)
{
    HAL_TIME_T rTime;

    // Read timer
    HAL_GetTime(&rTime);
    
    COPY_TO_USER((void*)arg, &(rTime.u4Seconds), sizeof(UINT32));   
    
    return  0;
}

static int _io_GetUniversalTime(unsigned long arg)
{
    UINT64 u8Utc;
    DateTime rTime;
    RTC_GetTimeDate(&u8Utc);
    rTime = (DateTime)u8Utc;
    COPY_TO_USER((void*)arg, &rTime, sizeof(int));   
    
    return 0;
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

int papi_ioctl_infra_clk(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg)
{
    int i4Ret = 0;

    switch (cmd)
    {
        case PAPI_IO_INFRA_CLK_GETALARM:
            i4Ret = _io_GetAlarm(arg);        	
            break;
        case PAPI_IO_INFRA_CLK_ALARMISSET:
            i4Ret = _io_AlarmIsSet(arg);         
            break;            
        case PAPI_IO_INFRA_CLK_SETALARM:
            i4Ret = _io_SetAlarm(arg);            	        	
            break;            
        case PAPI_IO_INFRA_CLK_CANCELALARM:
            i4Ret = _io_CancelAlarm(arg);            	        	
            break;            
        case PAPI_IO_INFRA_CLK_CLOCKISSET:
            i4Ret = _io_ClockIsSet(arg);            	        	        	
            break;            
        case PAPI_IO_INFRA_CLK_SETCLOCK:
            i4Ret = _io_SetClock(arg);            	        	        	        	
            break;            
        case PAPI_IO_INFRA_CLK_GETLOCALTIME:
            i4Ret = _io_GetLocalTime(arg);            	        	        	        	
            break;                    	
        case PAPI_IO_INFRA_CLK_GETLOCALTIMEOFFSET:
            i4Ret = _io_GetLocalTimeOffset(arg);            	        	        	        	
            break;                    	        	
        case PAPI_IO_INFRA_CLK_RUNDURINGSTANDBY:
            i4Ret = _io_RunDuringStandby(arg);            	        	        	        	
            break;                    	        	        	
        case PAPI_IO_INFRA_CLK_SECONDSSINCEWAKEUP:
            i4Ret = _io_SecondsSinceWakeup(arg);            	        	        	        	
            break;         	
        case PAPI_IO_INFRA_CLK_GETUNIVERSALTIME: 
            i4Ret = _io_GetUniversalTime(arg);            	        	        	        	
            break;         	
        default:    	
        printk("papi_ioctl_infra_clk: unknown IO command %u\n", cmd);
        i4Ret = -EFAULT;
    }

    return i4Ret;
}

