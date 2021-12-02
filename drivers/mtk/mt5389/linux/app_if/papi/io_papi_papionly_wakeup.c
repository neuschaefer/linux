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
 * $RCSfile: io_papi_papionly_wakeup.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_papi_src_src.c
 *  PAPI kernel driver implementation: Src selection
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_typedef.h"
#include "nptv_if.h"
#include "source_table.h"
#include "vdp_if.h"
#include "mpv_if.h"
#include "aud_if.h"
#include "aud_drvif.h"
#include "aud_dsp_cfg.h"
#include "fbm_drvif.h"
#include "x_assert.h"
#include "x_pdwnc.h"
#include "papi.h"
#include <linux/papi_ioctl.h>
#include <linux/mt53xx_cb.h>


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
/// Common error code
#define PAPI_PAPIONLY_WAKEUP_FAIL                     (-0x800)

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

/// Copy data from user space to kernel space
#define COPY_FROM_USER(dst, src, size) \
    if (!access_ok(VERIFY_READ, (void __user *)(src), (size))) \
    { \
        return PAPI_PAPIONLY_WAKEUP_FAIL  ; \
    } \
    if (copy_from_user((dst), (void __user *)(src), (size))) \
    { \
        return PAPI_PAPIONLY_WAKEUP_FAIL  ; \
    }

/// Copy data from kernel space to user space
#define COPY_TO_USER(dst, src, size) \
    if (!access_ok(VERIFY_WRITE, (void __user *)(dst), (size))) \
    { \
        return PAPI_PAPIONLY_WAKEUP_FAIL  ; \
    } \
    if (copy_to_user((void __user *)(dst), (src), (size))) \
    { \
        return PAPI_PAPIONLY_WAKEUP_FAIL  ; \
    }


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static int _i4WakeupReason = 0;
static PDWNC_SERVOAD_WAKEUP_T _arPapiWakeupServos[1];
static MTK_CUST_WAKEUP_KEY_T _arPapiWakeupKeys[MTK_CUST_RC_MAX_COUNT];
static MTK_CUST_WAKEUP_KEY_T _rPapiActualWakeupKey;

//-----------------------------------------------------------------------------
// External function prototypes
//-----------------------------------------------------------------------------

/// Put a callback event
EXTERN INT32 _CB_PutEvent(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);

//-----------------------------------------------------------------------------
// Function prototype forward declarations
//-----------------------------------------------------------------------------
INT32 _io_papi_RecordActualWakeupKey(void);


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static INT32 _ioctl_ProgramWakeupReasons(unsigned long u4Arg)
{
    // Add check for valid wakeup reason
    _i4WakeupReason = (int)u4Arg;
      
    return 0;
}

static INT32 _ioctl_ProgramWakeupKeysRC(unsigned long u4Arg)
{
    INT32 i4Ret = 0;
    int i4NumKeys = 0;
    UINT32 i;
    IOCTL_2ARG_T rArg;

    COPY_FROM_USER(&rArg, u4Arg, sizeof(IOCTL_2ARG_T));    

    i4NumKeys = (int)rArg.au4Arg[0];
    
    COPY_FROM_USER((void*)_arPapiWakeupKeys, (void*)rArg.au4Arg[1], i4NumKeys * 3 * sizeof(int));    

    for(i = 0; i < i4NumKeys; i++)
    {
        if(_arPapiWakeupKeys[i].i4Src == papi_papionly_keyntf_SourceRc5)
        {
            _arPapiWakeupKeys[i].i4Src = WAKEUP_KEY_TYPE_RC5;        
        }
        else if(_arPapiWakeupKeys[i].i4Src == papi_papionly_keyntf_SourceRc6)
        {
            _arPapiWakeupKeys[i].i4Src = WAKEUP_KEY_TYPE_RC6;        
        }
        else
        {
            return -1;
        }
    }
    
    i4Ret = IRRX_SetWakeupKeys((i4NumKeys > MTK_CUST_RC_MAX_COUNT)?MTK_CUST_RC_MAX_COUNT:i4NumKeys, _arPapiWakeupKeys);
    
    return i4Ret;
} 

static INT32 _ioctl_ProgramWakeupLkb(unsigned long u4Arg)
{
    INT32 i4Ret;
    UINT32 u4NumButtons, u4Servo, i;
    MTK_CUST_KEYBOARD_BUTTON_T* prButtons;
    
    int i4Cmd;

    COPY_FROM_USER(&i4Cmd, u4Arg, sizeof(int));   

    // Read in keyboard definition from diversity library
    if(_MTK_CUST_GetKeyboard(&u4Servo, &u4NumButtons, &prButtons))
    {
        return -1;
    }

    _arPapiWakeupServos[0].u4Servo = u4Servo;            
    for(i = 0; i < u4NumButtons; i++)
    {
        if(prButtons[i].u4Key == i4Cmd)
        {
            _arPapiWakeupServos[0].u4Top = prButtons[i].u4Top;
            _arPapiWakeupServos[0].u4Bottom = prButtons[i].u4Btm;            
        }
    }

    i4Ret = PDWNC_SetWakeupServos(1, _arPapiWakeupServos);
    
    return i4Ret;
}


static INT32 _ioctl_GetWakeupDetailsRC(unsigned long u4Arg)
{
    IOCTL_3ARG_T rArg;
    
    _io_papi_RecordActualWakeupKey();

    rArg.au4Arg[0] = (UINT32)_rPapiActualWakeupKey.i4Src;
    rArg.au4Arg[1] = (UINT32)_rPapiActualWakeupKey.i4Sys;
    rArg.au4Arg[2] = (UINT32)_rPapiActualWakeupKey.i4Cmd;
    
    COPY_TO_USER((void*)u4Arg, &rArg, sizeof(IOCTL_3ARG_T));
    
    return 0;
}

static INT32 _ioctl_GetWakeupDetailsLkb(unsigned long u4Arg)
{
    UINT32 u4NumButtons, u4Servo;
    MTK_CUST_KEYBOARD_BUTTON_T* prButtons;
    
    // Read in keyboard definition from diversity library
    if(_MTK_CUST_GetKeyboard(&u4Servo, &u4NumButtons, &prButtons))
    {
        return -1;
    }

    // Get real wakeup details?
    COPY_TO_USER((void*)u4Arg, &prButtons[0].u4Key, sizeof(int));
    
    return 0;
}


static INT32 _ioctl_GetProgrammedWakeupReasons(unsigned long u4Arg)
{
    UINT32 u4Val;
    
    COPY_TO_USER((void*)u4Arg, &_i4WakeupReason, sizeof(int));
   
    return 0;
}


static INT32 _ioctl_GetActualWakeupReasons(unsigned long u4Arg)
{
    UINT32 u4Val;
    int i4WakeupReason = 0;
    
    u4Val = PDWNC_ReadWakeupReason();

    if(u4Val == PDWNC_WAKE_UP_REASON_AC_POWER)
    {
        i4WakeupReason = papi_papionly_wakeup_WakeupReasonBoot;
    }        
    else if(u4Val == PDWNC_WAKE_UP_REASON_RTC)
    {
        i4WakeupReason = papi_papionly_wakeup_WakeupReasonAlarm;    
    }
    else if(u4Val == PDWNC_WAKE_UP_REASON_IRRC)
    {
        i4WakeupReason = papi_papionly_wakeup_WakeupReasonRc;        
    }
    else if(u4Val == PDWNC_WAKE_UP_REASON_KEYPAD)
    {
        i4WakeupReason = papi_papionly_wakeup_WakeupReasonLkb;        
    }    
    else if(u4Val == PDWNC_WAKE_UP_REASON_HDMI)
    {
        i4WakeupReason = papi_papionly_wakeup_WakeupReasonCec;        
    }        

    COPY_TO_USER((void*)u4Arg, &i4WakeupReason, sizeof(int));
   
    return 0;
}


INT32 _io_papi_EnterPowerDown(void)
{
    int wakeupreason = _i4WakeupReason;
    UINT32 u4PowerDownEn = 0;
    if(wakeupreason | papi_papionly_wakeup_WakeupReasonAlarm)
    {
        u4PowerDownEn |= WAK_RTC;
    }
    if(wakeupreason | papi_papionly_wakeup_WakeupReasonRc)
    {
        u4PowerDownEn |= WAK_IRRX;    
    }    
    if(wakeupreason | papi_papionly_wakeup_WakeupReasonLkb)
    {
        u4PowerDownEn |= SRVAD_WAKEN(5);        
    }    
    if(wakeupreason | papi_papionly_wakeup_WakeupReasonCec)
    {
        u4PowerDownEn |= WAK_CEC;            
    }    
    if(wakeupreason | papi_papionly_wakeup_WakeupReasonPin)
    {
        // Not support
    }    
    if(wakeupreason | papi_papionly_wakeup_WakeupReasonBoot)
    {
        // Not support
    }   
    PDWNC_EnterPowerDown(u4PowerDownEn, PDWNC_POWER_DOWN_IRRC);
    
    return 0;
}

INT32 _io_papi_RecordActualWakeupKey(void)
{
    static BOOL fgHasRead = FALSE; 
    if(!fgHasRead)
    {
        UINT32 u4Type, u4Addr, u4Cmd;       
        IRRX_GetActualWakeupKey(&u4Type, &u4Addr, &u4Cmd);	
        if(u4Cmd == WAKEUP_KEY_TYPE_RC5)
        {
            _rPapiActualWakeupKey.i4Src = papi_papionly_keyntf_SourceRc5;
        }
        else
        {
            _rPapiActualWakeupKey.i4Src = papi_papionly_keyntf_SourceRc6;        
        }
        _rPapiActualWakeupKey.i4Sys = (int)u4Addr;
        _rPapiActualWakeupKey.i4Cmd = (int)u4Cmd;        
        fgHasRead = TRUE;
    }
    return 0;
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

int papi_ioctl_papionly_wakeup(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg)
{
    int i4Ret = 0;

    switch (cmd)
    {
    case PAPI_IO_PAPIONLY_WAKEUP_PROGRAMWAKEUPREASONS:
        i4Ret = _ioctl_ProgramWakeupReasons(arg);
        break;
    case PAPI_IO_PAPIONLY_WAKEUP_PROGRAMWAKEUPKEYSRC:
        i4Ret = _ioctl_ProgramWakeupKeysRC(arg);
        break;
    case PAPI_IO_PAPIONLY_WAKEUP_PROGRAMWAKEUPLKB:
        i4Ret = _ioctl_ProgramWakeupLkb(arg);
        break;
    case PAPI_IO_PAPIONLY_WAKEUP_GETWAKEUPDETAILSRC:
        i4Ret = _ioctl_GetWakeupDetailsRC(arg);
        break;         
    case PAPI_IO_PAPIONLY_WAKEUP_GETWAKEUPDETAILSLKB:
        i4Ret = _ioctl_GetWakeupDetailsLkb(arg);
        break;             
    case PAPI_IO_PAPIONLY_WAKEUP_GETPROGRAMMEDWAKEUPREASONS:
        i4Ret = _ioctl_GetProgrammedWakeupReasons(arg);
        break;                 	
    case PAPI_IO_PAPIONLY_WAKEUP_GETACTUALWAKEUPREASONS:        
        i4Ret = _ioctl_GetActualWakeupReasons(arg);
        break;                 	
    default:
        printk("papi_ioctl_papiony_wakeup: unknown IO command %u\n", cmd);
        i4Ret = -EFAULT;
    }

    return i4Ret;
}


