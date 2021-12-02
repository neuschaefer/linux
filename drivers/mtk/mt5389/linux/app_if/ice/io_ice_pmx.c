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
 * $Author: hongjun.chu $
 * $Date: 2012/04/28 $
 * $RCSfile: io_ice_vga.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_ice_vga.c
 *  ICE VGA driver interface.
 */

#include "mttype.h"
#include "ice.h"
#include "vdp_if.h"
#include "drv_scpos.h"
#include "panel.h"
#include "mute_if.h"
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Include files
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

//-----------------------------------------------------------------------------
// Import functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
#if 0
static MT_RESULT_T _ICE_PMX_SetPanel(unsigned long arg)
{
    UINT32 _u4PanelIndex =0 ;
    MT_RESULT_T eRet = MTR_OK;
    _u4PanelIndex = (UINT32)arg;
     printf(" IO_ice_pmx.c  _u4PanelIndex %d \n",_u4PanelIndex);
    //_VDP_SelectPanel(_u4PanelIndex);
    return eRet;
}
#endif

static MT_RESULT_T _ICE_PMX_EnablePattern(unsigned long arg)
{
	UINT32 _u4OnOff =0 ;
	MT_RESULT_T eRet = MTR_OK;
	_u4OnOff = (UINT32)arg;
	printf(" IO_ice_pmx.c  _u4OnOff %d \n",_u4OnOff);
	vDrvSetOSTGOutPattern(_u4OnOff);
	return eRet;
}

static MT_RESULT_T _ICE_PMX_ListPanel(unsigned long arg)
{
	MT_RESULT_T eRet = MTR_OK;
	printf(" List Panel \n");
    DumpPanelAttribute(3);
	return eRet;
}

static MT_RESULT_T  _ICE_PMX_VideoMute(unsigned long arg)
{
	MT_RESULT_T eRet = MTR_OK;
	ICE_IOCTL_2ARG_T rArg;
	UINT32 bPath, bOnOff;
	USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
	bPath = (UINT32)(rArg.ai4Arg[0]);
	bOnOff = (UINT32)(rArg.ai4Arg[0]);
	printf(" set Video Mute  bPath = %d, bOnOff = %d \n",bPath,bOnOff);

	if(bOnOff == 1)
	{
		_vDrvVideoSetMute(MUTE_MODULE_API_FORCE, bPath, FOREVER_MUTE, FALSE);
	}
	else
	{
		_vDrvVideoSetMute(MUTE_MODULE_API_FORCE, bPath, 0, FALSE);
	}
	return eRet;
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
int ice_ioctl_pmx(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
{
    int i4Ret = MTR_NOT_OK;
    switch (cmd)
    {
		case ICE_IO_PMX_PATTERN:
            i4Ret = _ICE_PMX_EnablePattern(arg);  
            break;
		case ICE_IO_PMX_LISTPANEL:
			i4Ret = _ICE_PMX_ListPanel(arg);  
			break;
		case ICE_IO_PMX_VIDEOMUTE:
			i4Ret = _ICE_PMX_VideoMute(arg);  
			break;
        default:
            break;
    }
    return i4Ret;
}

