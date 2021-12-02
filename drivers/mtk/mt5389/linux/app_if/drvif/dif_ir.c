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
 * $RCSfile: dif_ir.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dif_jpg.c
 *  JPG driver interface implementation.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "dif_debug.h"
#include "dif_ir.h"
#include "x_linux.h"
#include "x_drv_if.h"
#include "x_timer.h"
#include <linux/mt53xx_ir.h>

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static BOOL _fgPollMode = TRUE;
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _IrOnInit
 *  Handler of test command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _IrOnInit(unsigned long arg)
{
    IR_IOCTL_INIT_T rInit;
    DRV_IR_PARAM_T rParam;
    INT32 i4Ret = 0;
    
    if (copy_from_user(&rInit, (void __user *)arg, sizeof (rInit)))
    {
        LOG(3, "IR_IOCTL: copy_to_user() failed\n");
        return FALSE;
    }
    
    if(rInit.ucPollMode == 1)
    {        
        i4Ret = IRRX_InitMtkIr(rInit.u4Type);
        _fgPollMode = TRUE;
    }
    else
    {
        rParam.rIrType = rInit.u4Type;
        rParam.rRepeatFreq = DRV_IR_REPEAT_FREQ_NONE;
        i4Ret = DRV_IR_SetParameter(&(rInit));
        _fgPollMode = FALSE;        
    }
    
    if(i4Ret)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}


//-----------------------------------------------------------------------------
/** _JpgOnDecode
 *  
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _IrOnPoll(unsigned long arg)
{
    IR_IOCTL_POLL_T rPoll;
    HAL_TIME_T rTime;
    if(IRRX_PollMtkIr(&(rPoll.u4Key)) == 0)
    {
        HAL_GetTime(&rTime);
        rPoll.u4Seconds = rTime.u4Seconds;
        rPoll.u4Micros = rTime.u4Micros;
        if (copy_to_user((void __user *)arg, &rPoll, sizeof (rPoll)))
        {
            LOG(3, "IR_IOCTL: copy_to_user() failed\n");
            return FALSE;
        }
        return TRUE;
    }
}

//-----------------------------------------------------------------------------
/** _JpgOnDecode
 *  
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _IrOnStart(unsigned long arg)
{
    if(DRV_IR_Start() != DRV_E_OK)
    {
        return FALSE;
    }
    return TRUE;
}

//-----------------------------------------------------------------------------
/** _JpgOnMMAP
 *  
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------

UINT32 _IrOnStop(void)
{
    if(DRV_IR_Stop() != DRV_E_OK)
    {
        return FALSE;
    }
    return TRUE;
}


