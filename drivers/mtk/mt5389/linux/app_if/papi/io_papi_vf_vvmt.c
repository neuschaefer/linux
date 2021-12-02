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
 * $RCSfile: io_papi_vf_vvmt.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_papi_mixer_vmx.c
 *  PAPI kernel driver implementation: MixerVmx
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include <linux/papi_ioctl.h>
#include <linux/cb_data.h>
#include "papi_types.h"
#include "papi_vf.h"
#include "sv_const.h"
#include "vdp_drvif.h"
#include "vdp_if.h"
#include "drv_video.h"
#include "drv_scpos.h"
#include "drvcust_if.h"

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

#define COPY_TO_USER(USER_PARAMETER, KERNEL_PARAMETER, SIZE)                   \
    do                                                                         \
    {                                                                          \
        if (!access_ok(VERIFY_WRITE, (void __user *)(USER_PARAMETER), (SIZE))) \
        {                                                                      \
            printk("%s check access fail\n", __FUNCTION__);                    \
            return (TM_ERR_BAD_PARAMETER);                                     \
        }                                                                      \
        if (copy_to_user((void __user *)(USER_PARAMETER),                      \
            (KERNEL_PARAMETER), (SIZE)))                                       \
        {                                                                      \
            printk("%s copy_to_user fail\n", __FUNCTION__);                    \
            return TM_ERR_BAD_PARAMETER;                                       \
        }                                                                      \
    }                                                                          \
    while (0)

#define COPY_FROM_USER(USER_PARAMETER, KERNEL_PARAMETER, SIZE)                 \
    do                                                                         \
    {                                                                          \
        if (!access_ok(VERIFY_READ, (void __user *)(USER_PARAMETER), (SIZE))) \
        {                                                                      \
            printk("%s check access fail\n", __FUNCTION__);                    \
            return (TM_ERR_BAD_PARAMETER);                                     \
        }                                                                      \
        if (copy_from_user((KERNEL_PARAMETER),                                 \
                           (void __user *)(USER_PARAMETER), (SIZE)))           \
        {                                                                      \
            printk("%s copy_from_user fail\n", __FUNCTION__);                  \
            return TM_ERR_BAD_PARAMETER;                                       \
        }                                                                      \
    }                                                                          \
    while (0)


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static VMixColor _arMuteColor[VDP_MAX_NS] = {{0, 0, 0}, {0, 0, 0}};
static BOOL _au4AutoMute[VDP_MAX_NS] = {TRUE, TRUE};
//static BOOL _au4Mute[VDP_MAX_NS] = {TRUE, TRUE};


//-----------------------------------------------------------------------------
// External function prototypes
//-----------------------------------------------------------------------------
extern INT32 _CB_PutEvent(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

/**  @ingroup vf_functions */
/**  @brief Indicates if the specified mute color is supported. */
/**  @param red The red component of the mute color. See datatype \ref IPlfApiVideoMute_ColorRange. */
/**  @param green The green component of the mute color. See datatype \ref IPlfApiVideoMute_ColorRange. */
/**  @param blue The blue component of the mute color. See datatype \ref IPlfApiVideoMute_ColorRange. */
/**  @param retval Is set to True if the specified mute color is supported. */
static INT32 _papi_vf_vmt_IsMuteColorSupported( int red, int green, int blue, Bool * retval )
{
    Bool u4Support;
    
    u4Support = FALSE;
    if ((red >= papi_vf_vmt_MinColor) && (red <= papi_vf_vmt_MaxColor) && 
    	(green >= papi_vf_vmt_MinColor) && (green <= papi_vf_vmt_MaxColor) && 
    	(blue >= papi_vf_vmt_MinColor) && (blue <= papi_vf_vmt_MaxColor))
    {
        u4Support = TRUE;
    }
    
    COPY_TO_USER(retval, &u4Support, sizeof(Bool));
	
    return TM_OK;
}

/**  @ingroup vf_functions */
/**  @brief Indicates if mute can be set. */
/**  @param retval Returns True if mute can be set. */
static INT32 _papi_vf_vmt_IsMuteSupported( Bool * retval )
{
    Bool u4Support;
    
    u4Support = TRUE;    
    COPY_TO_USER(retval, &u4Support, sizeof(Bool));
    
    return TM_OK;
}

/**  @ingroup vf_functions */
/**  @brief Indicates if auto mute is supported. */
/**  @param retval Is set to True when auto mute is supported. Is set to False otherwise. */
static INT32 _papi_vf_vmt_IsAutoMuteSupported( Bool * retval )
{
    Bool u4Support;
    
    u4Support = TRUE;    
    COPY_TO_USER(retval, &u4Support, sizeof(Bool));

    return TM_OK;
}

/**  @ingroup vf_functions */
/**  @brief Returns the actual mute color. The platform may internally overrule the mute color that was set. It provides the actual mute color on the video layer. */
/**  @param red The red component of the mute color. See datatype \ref IPlfApiVideoMute_ColorRange. */
/**  @param green The green component of the mute color. See datatype \ref IPlfApiVideoMute_ColorRange. */
/**  @param blue The blue component of the mute color. See datatype \ref IPlfApiVideoMute_ColorRange. */
static INT32 _papi_vf_vmt_GetActualMuteColor( int * red, int * green, int * blue )
{
    //FIXME: need drv api to get MuteColor
    //UINT32 u4MuteColor =0;
    int i4Red, i4Green, i4Blue;

    //u4MuteColor = dwScposGetBg(SV_VP_MAIN);
    //i4Red = (u4MuteColor & 0xff0000) >> 16;
    //i4Green = (u4MuteColor & 0x00ff00) >> 8;
    //i4Blue = (u4MuteColor & 0x0000ff);

    i4Red = _arMuteColor[VDP_1].redOrY;
    i4Green = _arMuteColor[VDP_1].greenOrU;
    i4Blue = _arMuteColor[VDP_1].blueOrV;
    
    COPY_TO_USER(red, &i4Red, sizeof(int));
    COPY_TO_USER(green, &i4Green, sizeof(int));
    COPY_TO_USER(blue, &i4Blue, sizeof(int));

    return TM_OK;
}

/**  @ingroup vf_functions */
/**  @brief Controls the internal mute of the platform based on signal conditions. This is auto blanking of the platform in case of bad signal conditions (no video, zapping, ...). It applies to the video layer only! */
/**  @param mode True=on */
static INT32 _papi_vf_vmt_SetAutoMute( Bool mode )
{
    
    //VDP_SetAutoMute(VDP_1, mode);
    _au4AutoMute[VDP_1] = (BOOL)mode;
    vScposMuteMaskOnOff(VDP_1, mode);

    return TM_OK;
}

/**  @ingroup vf_functions */
/**  @brief Returns if mute is set or not. */
/**  @param retval The mute value set. */
static INT32 _papi_vf_vmt_GetMute( Bool * retval )
{
    Bool u4Mute;
    UCHAR ucMode;

    if (VDP_GetMode(VDP_1, &ucMode) != VDP_SET_OK)
    {
        return TM_ERR_BAD_PARAMETER;
    }
    	
    if (ucMode == VDP_MODE_BG)
    {
        u4Mute = TRUE;
    }
    else
    {
        u4Mute = FALSE;
    }
   
    COPY_TO_USER(retval, &u4Mute, sizeof(Bool));

    return TM_OK;
}

/**  @ingroup vf_functions */
/**  @brief Returns the color used when muting the video. */
/**  @param red Set to the red component of the mute color. See datatype \ref IPlfApiVideoMute_ColorRange. */
/**  @param green Set to the green component of the mute color. See datatype \ref IPlfApiVideoMute_ColorRange. */
/**  @param blue Set to the blue component of the mute color. See datatype \ref IPlfApiVideoMute_ColorRange. */
static INT32 _papi_vf_vmt_GetMuteColor( int * red, int * green, int * blue )
{
    int i4R, i4G, i4B;

    i4R = _arMuteColor[VDP_1].redOrY;
    i4G = _arMuteColor[VDP_1].greenOrU;
    i4B = _arMuteColor[VDP_1].blueOrV;

    COPY_TO_USER(red, &i4R, sizeof(int));
    COPY_TO_USER(green, &i4G, sizeof(int));
    COPY_TO_USER(blue, &i4B, sizeof(int));

    return TM_OK;
}

/**  @ingroup vf_functions */
/**  @brief Indicates if the video output is actually muted. Either as a result of SetMute or by implementation defined behavior. It provides the actual blank status (on/off) on the video layer. */
/**  @param retval  */
static INT32 _papi_vf_vmt_GetActualMute( Bool * retval )
{
    UINT32 u4MuteOnOff;

    u4MuteOnOff = fgMainMuteOn()?(TRUE):(FALSE);
    
    COPY_TO_USER(retval, &u4MuteOnOff, sizeof(Bool));

    return TM_OK;
}

/**  @ingroup vf_functions */
/**  @brief Mutes the video layer. */
/**  @param mode Mutes the video when set to True. Unmutes otherwise. */
/**  @param retval True, if the action was completed successfully. False, if the action will be handled asynchronously. In the latter case, a notification will be given when the action is completed. */
static INT32 _papi_vf_vmt_SetMute( Bool mode, Bool * retval )
{
    Bool u4Done;
    UCHAR ucMode;
    	
    if (mode)
    {
        ucMode = VDP_MODE_BG;
    }
    else
    {
        ucMode = VDP_MODE_NORMAL;
    }
    
    if (VDP_SetMode(VDP_1, ucMode) != VDP_SET_OK)
    {
        return TM_ERR_BAD_PARAMETER;
    }

    vDrvMainMute(TRUE);
    u4Done = TRUE;
    COPY_TO_USER(retval, &u4Done, sizeof(Bool));
    	
    return TM_OK;
}

/**  @ingroup vf_functions */
/**  @brief Specifies the color to be used when muting the video. */
/**  @param red The red component of the mute color. See datatype \ref IPlfApiVideoMute_ColorRange. */
/**  @param green The green component of the mute color. See datatype \ref IPlfApiVideoMute_ColorRange. */
/**  @param blue The blue component of the mute color. See datatype \ref IPlfApiVideoMute_ColorRange. */
static INT32 _papi_vf_vmt_SetMuteColor( int red, int green, int blue )
{
    UINT32 u4BgColor;

    if ((red < papi_vf_vmt_MinColor) || (red > papi_vf_vmt_MaxColor) || 
    	(green < papi_vf_vmt_MinColor) || (green > papi_vf_vmt_MaxColor) || 
    	(blue < papi_vf_vmt_MinColor) || (blue > papi_vf_vmt_MaxColor))
    {
        return TM_ERR_BAD_PARAMETER;
    }

    _arMuteColor[VDP_1].redOrY = red;
    _arMuteColor[VDP_1].greenOrU= green;
    _arMuteColor[VDP_1].blueOrV = blue;
    u4BgColor = (red<<16) | (green<<8) | (blue);
    VDP_SetBg(VDP_1, u4BgColor);

    return TM_OK;
}

/**  @ingroup vf_functions */
/**  @brief Returns the previously set auto mute status. */
/**  @param retval The set auto mute status. */
static INT32 _papi_vf_vmt_GetAutoMute( Bool * retval )
{
    UINT32 u4AutoMute;
    
    u4AutoMute = _au4AutoMute[VDP_1];
    COPY_TO_USER(retval, &u4AutoMute, sizeof(Bool));

    return TM_OK;
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

int papi_ioctl_vf_vmt(struct inode *inode, struct file *file,
                         unsigned int cmd, unsigned long arg)
{
    IOCTL_4ARG_T rArg4;

    int i4Ret = 0;

//    _ApiLock();

    if ((_IOC_SIZE(cmd)) > sizeof(unsigned long))
    {
        COPY_FROM_USER(arg, &rArg4, _IOC_SIZE(cmd));
    }

    switch (cmd)
    {

    case PAPI_IO_VF_VMT_ISMUTECOLORSUPPORTED:
    	i4Ret = _papi_vf_vmt_IsMuteColorSupported((int)rArg4.au4Arg[0], (int)rArg4.au4Arg[1], (int)rArg4.au4Arg[2], (Bool *)rArg4.au4Arg[3]);
    	break;
    case PAPI_IO_VF_VMT_ISMUTESUPPORTED:
    	i4Ret = _papi_vf_vmt_IsMuteSupported((Bool *)arg);
    	break;
    case PAPI_IO_VF_VMT_ISAUTOMUTESUPPORTED:
    	i4Ret = _papi_vf_vmt_IsAutoMuteSupported((Bool *)arg);
    	break;
    case PAPI_IO_VF_VMT_GETACTUALMUTECOLOR:
    	i4Ret = _papi_vf_vmt_GetActualMuteColor((int *)rArg4.au4Arg[0], (int *)rArg4.au4Arg[1], (int *)rArg4.au4Arg[2]);
    	break;
    case PAPI_IO_VF_VMT_SETAUTOMUTE:
    	i4Ret = _papi_vf_vmt_SetAutoMute((Bool)arg);
    	break;
    case PAPI_IO_VF_VMT_GETMUTE:
    	i4Ret = _papi_vf_vmt_GetMute((Bool *)arg);
    	break;
    case PAPI_IO_VF_VMT_GETMUTECOLOR:
    	i4Ret = _papi_vf_vmt_GetMuteColor((int *)rArg4.au4Arg[0], (int *)rArg4.au4Arg[1], (int *)rArg4.au4Arg[2]);
    	break;
    case PAPI_IO_VF_VMT_GETACTUALMUTE:
    	i4Ret = _papi_vf_vmt_GetActualMute((Bool *)arg);
    	break;
    case PAPI_IO_VF_VMT_SETMUTE:
    	i4Ret = _papi_vf_vmt_SetMute((Bool)rArg4.au4Arg[0], (Bool *)rArg4.au4Arg[1]);
    	break;
    case PAPI_IO_VF_VMT_SETMUTECOLOR:
    	i4Ret = _papi_vf_vmt_SetMuteColor((int)rArg4.au4Arg[0], (int)rArg4.au4Arg[1], (int)rArg4.au4Arg[2]);
    	break;
    case PAPI_IO_VF_VMT_GETAUTOMUTE:
    	i4Ret = _papi_vf_vmt_GetAutoMute((Bool *)arg);
    	break;
        
    default:
        printk("papi_ioctl_vf_vmt: unknown IO command %u\n", cmd);
        i4Ret = -EFAULT;
    }

//    _ApiUnlock();

    return i4Ret;

}


// Notification function

void PAPIVFNotifyActualMuteChanged(UINT32 u4VdpId)
{
    PAPI_VF_CB_T rSrcCb;

    // debug msg
    printk("ActualMuteChanged: %u\n", (unsigned int)u4VdpId);

    rSrcCb.i4Type = PAPI_CB_VF_VMT_ONACTUALMUTECHANGED;
    _CB_PutEvent(CB_PAPI_VF_TRIGGER, sizeof (PAPI_VF_CB_T), &rSrcCb);
}

void PAPIVFNotifyActualMuteColorChanged(UINT32 u4VdpId)
{
    PAPI_VF_CB_T rSrcCb;

    // debug msg
    printk("ActualMuteColorChanged: %u\n", (unsigned int)u4VdpId);

    rSrcCb.i4Type = PAPI_CB_VF_VMT_ONACTUALMUTECOLORCHANGED;
    _CB_PutEvent(CB_PAPI_VF_TRIGGER, sizeof (PAPI_VF_CB_T), &rSrcCb);
}

void PAPIVFNotifyBlankedChanged(UINT32 u4VdpId)
{
    PAPI_VF_CB_T rSrcCb;

    // debug msg
    printk("BlankedChanged: %u\n", (unsigned int)u4VdpId);

    rSrcCb.i4Type = PAPI_CB_VF_VIO_ONBLANKEDCHANGED;
    _CB_PutEvent(CB_PAPI_VF_TRIGGER, sizeof (PAPI_VF_CB_T), &rSrcCb);
}

void PAPIVFNotifyMuteChanged(NOTIFY_MUTE_T tNotifyMute)
{
    switch (tNotifyMute.u4NotifyId)
    {
        case NOTIFICATION_MUTE_ACTUALMUTECHANGED:
              PAPIVFNotifyActualMuteChanged(tNotifyMute.u4VdpId);
        	break;
        case NOTIFICATION_MUTE_ACTUALMUTECOLORCHANGED:
              PAPIVFNotifyActualMuteColorChanged(tNotifyMute.u4VdpId);
        	break;
        case NOTIFICATION_MUTE_BLANKEDCHANGED:
              PAPIVFNotifyBlankedChanged(tNotifyMute.u4VdpId);
                break;
        default:
              printk("papi_ioctl_vf_vmt: unknown notification command %u\n", (unsigned int)tNotifyMute.u4NotifyId);
        	break;
    }
}


