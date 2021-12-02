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
 * $RCSfile: io_papi_fe_vip.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_papi_fe_vip.c
 *  PAPI kernel driver implementation: video properties
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include <linux/papi_ioctl.h>
#include <linux/mt53xx_cb.h>
#include <linux/cb_data.h>
#include "papi_fe.h"
#include "papi_vf.h"
#include "nptv_if.h"
#include "video_def.h"
#include "vdp_if.h"
#include "vdo_misc.h"
#include "source_select.h"
#include "drv_scart.h"
#include "hw_tdc.h"
#include "drv_tvd.h"
#include "sv_const.h"
#include "drvcust_if.h"
#include "api_notify.h"


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
#if 0
typedef struct {
    UINT8            module_id;
    UINT8            beo_connected;
    UINT8            vpath_assigned;
    SdVideo_Settings_t setting;
    SdBackEndInID_k        be_id;
    struct rSdalBackend *     be_mod;
    SdWindowID_k        out_id;    //main or sub
    SdBool_t            mute;
    SdBool_t            still;
    SdBool_t            injectvideo; //for sdimage
} SdalVideo_t;
#endif
//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define VERIFY_NULL(ptr)                                    \
          do {                                              \
            if (ptr == NULL) { return (TM_ERR_BAD_PARAMETER); }      \
          } while (0)

#define USR_SPACE_ACCESS_VALIDATE_1ARG(arg)                 \
    if (!access_ok(VERIFY_WRITE, (void __user *)arg,         \
                   sizeof(int)))                            \
    {                                                       \
        printk("papi_fe_vip argument error\n");     \
        return (TM_ERR_BAD_PARAMETER);                                          \
    }  

#define COPY_TO_USER(dst, src, size) \
    if (copy_to_user((void __user *)(dst), (src), (size))) \
    { \
        return (TM_ERR_BAD_PARAMETER);\
    }

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


static INT32 _papi_fe_vip_GetVideoPresent(int *retval) 
{
    UINT8 u1Status;

    VERIFY_NULL(retval);	

    u1Status = bDrvVideoSignalStatus(SV_VP_MAIN);
	
    if (u1Status == SV_VDO_NOSIGNAL)
    {
        *retval = papi_fe_vip_VideoPresenceNotDetected;
    }
    else if (u1Status == SV_VDO_NOSUPPORT)
    {
        *retval = papi_fe_vip_VideoPresenceInvalid;   
    }
    else if (u1Status == SV_VDO_STABLE)
    {
        *retval = papi_fe_vip_VideoPresenceDetected;
    }
    else
    {
        *retval = papi_fe_vip_VideoPresenceUnknown;
    }

    return TM_OK;
    
}

static INT32 _papi_fe_vip_GetSignalPresent(int *retval) 
{
    int i4VideoPresent;
 //   int i4AudioPresent;
    
    VERIFY_NULL(retval);	

    _papi_fe_vip_GetVideoPresent(&i4VideoPresent);

    // Check if audio present
    //
    // TBD
    // 
//    i4AudioPresent = 0;
    	
    if (i4VideoPresent == papi_fe_vip_VideoPresenceDetected)
    {
        *retval = papi_fe_vip_SignalPresenceDetected;        
    }
    else if (i4VideoPresent == papi_fe_vip_VideoPresenceInvalid)
    {
        *retval = papi_fe_vip_SignalPresenceInvalid;        
    }
    else if (i4VideoPresent == papi_fe_vip_VideoPresenceUnknown)
    {
        *retval = papi_fe_vip_SignalPresenceUnknown;        
    }
    else if (i4VideoPresent == papi_fe_vip_VideoPresenceNotDetected)
    {
        *retval = papi_fe_vip_SignalPresenceNotDetected;        
    }
    return TM_OK;
}

static INT32 _papi_fe_vip_GetFieldFrequency(int *retval) 
{
    VERIFY_NULL(retval);	

    *retval = (int)bDrvVideoGetRefreshRate(SV_VP_MAIN);    

    return TM_OK;
}

static INT32 _papi_fe_vip_GetNumberOfFrameLines(int *retval) 
{
    VERIFY_NULL(retval);	

    *retval = (int)wDrvVideoGetVTotal(SV_VP_MAIN);    

    return TM_OK;
}

static INT32 _papi_fe_vip_GetNumberOfVisibleLines(int *retval) 
{
    VERIFY_NULL(retval);	

    *retval = (int)wDrvVideoInputHeight(SV_VP_MAIN);    

    return TM_OK;
}

static INT32 _papi_fe_vip_GetNumberOfVisiblePixels(int *retval) 
{
    VERIFY_NULL(retval);	

    *retval = (int)wApiVideoGetInputWidth(SV_VP_MAIN);    

    return TM_OK;
}

static INT32 _papi_fe_vip_GetInterlace(int *retval) 
{
    VERIFY_NULL(retval);	

//    *retval = (int)bDrvVideoIsSrcInterlace(SV_VP_MAIN);

    if(bDrvVideoIsSrcInterlace(SV_VP_MAIN))
        *retval = papi_fe_vip_InterlaceTwoOne;
    else
    	 *retval = papi_fe_vip_InterlaceOneOne;


    return TM_OK;
}

static INT32 _papi_fe_vip_GetImageFormat(int *retval) 
{
    UINT8 u1SrcType;

    VERIFY_NULL(retval);	

    u1SrcType = bApiVideoGetSrcType(SV_VP_MAIN);

    switch(VSS_MAJOR(u1SrcType))
    {
#if SUPPORT_SCART
        case VSS_SCART:
            switch(bDrvGetScartPin8ASR(_bSrcMainNew))
            {
                case SV_WS_4W_3H:
                    *retval = papi_fe_vip_ImageFormat4by3FullFormat;
                    break;
                case SV_WS_16W_9H:
                    *retval = papi_fe_vip_ImageFormat16by9FullFormat;
                    break;
                default:
                    *retval = papi_fe_vip_ImageFormatInvalid;
                    break;
            }
            break;
#endif
#if SUPPORT_WSS
        case VSS_ATD:
        case VSS_ATV: 
            break;
#endif
        case VSS_HDMI: /* InfoFrame */
        default:
            *retval = papi_fe_vip_ImageFormatUnknown;
            break;
    }

    return TM_OK;
}

static INT32 _papi_fe_vip_IsFilmMode(Bool *retval) 
{	
    VERIFY_NULL(retval);

    *retval=VDP_GetDIFilm32Enable(VDP_1);

    return TM_OK;
}

static INT32 _papi_fe_vip_IsFromVcr(Bool *retval) 
{
    VERIFY_NULL(retval);	

    *retval = fgHwTvdTrick() || fgHwTvdHeadSwitch();

    return TM_OK;
}

static INT32 _papi_fe_vip_GetVideoCoding(int *retval) 
{
    UINT8 u1SrcType;
    UINT8 u1VideoCoding;
    UINT8 u1Timing;

    VERIFY_NULL(retval);	

    u1SrcType = bApiVideoGetSrcType(SV_VP_MAIN);

    switch(VSS_MAJOR(u1SrcType))
    {
        case VSS_ATV:
        case VSS_ATD:
        case VSS_CVBS:
            *retval = papi_fe_vip_VideoCodingCvbs;
        break;
        case VSS_SV:
            *retval = papi_fe_vip_VideoCodingYc;
        break;
        case VSS_DTV:
        case VSS_DTD:
            u1Timing = bDrvVideoGetTiming(SV_VP_MAIN);
            if(Get_VGAMODE_ICLK(u1Timing) >= 650)
            {
        	*retval = papi_fe_vip_VideoCodingYPbPr3FH;
            }
            else if(u1Timing == MODE_525I_OVERSAMPLE || u1Timing == MODE_625I_OVERSAMPLE || u1Timing == MODE_525I || u1Timing == MODE_625I || u1Timing == MODE_240P)
            {
        	*retval = papi_fe_vip_VideoCodingYPbPr;
            }
            else
            {
        	*retval = papi_fe_vip_VideoCodingYPbPr2FH;
            }
        break;
        case VSS_VGA:
        case VSS_VGACOMP:
            if(Get_VGAMODE_ICLK(bDrvVideoGetTiming(SV_VP_MAIN)) >= 650)
            {
                *retval = papi_fe_vip_VideoCodingRgb3FH;
            }
            else
            {
                *retval = papi_fe_vip_VideoCodingRgb2FH;
            }
        break;
        break;
        case VSS_YPBPR:
            u1Timing = bDrvVideoGetTiming(SV_VP_MAIN);
            if(Get_VGAMODE_ICLK(u1Timing) >= 650)
            {
                *retval = papi_fe_vip_VideoCodingYPbPr3FH;
            }
            else if(u1Timing == MODE_525I_OVERSAMPLE || u1Timing == MODE_625I_OVERSAMPLE || u1Timing == MODE_525I || u1Timing == MODE_625I || u1Timing == MODE_240P)
            {
                *retval = papi_fe_vip_VideoCodingYPbPr;
            }
            else
            {
                *retval = papi_fe_vip_VideoCodingYPbPr2FH;
            }
        break;
        case VSS_HDMI:
            u1VideoCoding = bHDMIColorFormat(0/*Auto detect */);
            u1Timing = bDrvVideoGetTiming(SV_VP_MAIN);
            switch(u1VideoCoding)
            {
                case SV_COLOR_HDMI_RGB:
                    if(Get_VGAMODE_ICLK(u1Timing) >= 650)
                    {
                	*retval = papi_fe_vip_VideoCodingRgb3FH;
                    }
                    else if(u1Timing == MODE_525I_OVERSAMPLE || u1Timing == MODE_625I_OVERSAMPLE || u1Timing == MODE_525I || u1Timing == MODE_625I || u1Timing == MODE_240P)
                    {
                	*retval = papi_fe_vip_VideoCodingRgb;
                    }
                    else
                    {
                	*retval = papi_fe_vip_VideoCodingRgb2FH;
                    }
                break;
                case SV_COLOR_HDMI_422_601:
                case SV_COLOR_HDMI_444_601:
                case SV_COLOR_HDMI_XVYCC_601:
                    if(Get_VGAMODE_ICLK(u1Timing) >= 650)
                    {
                	*retval = papi_fe_vip_VideoCodingYuv3FH;
                    }
                    else if(u1Timing == MODE_525I_OVERSAMPLE || u1Timing == MODE_625I_OVERSAMPLE || u1Timing == MODE_525I || u1Timing == MODE_625I || u1Timing == MODE_240P)
                    {
                	*retval = papi_fe_vip_VideoCodingYuv;
                    }
                    else
                    {
                	*retval = papi_fe_vip_VideoCodingYuv2FH;
                    }
                break;
                case SV_COLOR_HDMI_422_709:
                case SV_COLOR_HDMI_444_709:
                case SV_COLOR_HDMI_XVYCC_709:
                    if(Get_VGAMODE_ICLK(u1Timing) >= 650)
                    {
                	*retval = papi_fe_vip_VideoCodingYPbPr3FH;
                    }
                    else if(u1Timing == MODE_525I_OVERSAMPLE || u1Timing == MODE_625I_OVERSAMPLE || u1Timing == MODE_525I || u1Timing == MODE_625I || u1Timing == MODE_240P)
                    {
                	*retval = papi_fe_vip_VideoCodingYPbPr;
                    }
                    else
                    {
                	*retval = papi_fe_vip_VideoCodingYPbPr2FH;
                    }
                break;
                default:
                	*retval = papi_fe_vip_VideoCodingUnknown;
                break;
            }
            
      	 break;
        case VSS_SCART:
            u1VideoCoding = bDrvGetScartInputMode();
            switch(u1VideoCoding)
            {
                case SCART_COLOR_SV:
                	*retval = papi_fe_vip_VideoCodingYc;
             	  break;
             	  case SCART_COLOR_RGB:
             	  	*retval = papi_fe_vip_VideoCodingRgb;
      	         break;
                case SCART_COLOR_CVBS:
                default:
                	*retval = papi_fe_vip_VideoCodingCvbs;
                break;
      	         
            }
        break;

        default:
            *retval = papi_fe_vip_VideoCodingUnknown;
        break;
    }

    return TM_OK;
}

static INT32 _papi_fe_vip_GetMacroVisionType(int *retval) 
{
    UINT8 u1SrcType;

    VERIFY_NULL(retval);	

    u1SrcType = bApiVideoGetSrcType(SV_VP_MAIN);
    
    switch(VSS_MAJOR(u1SrcType))
    {
        case VSS_CVBS:
        case VSS_SV:
        case VSS_SCART:
            if(fgHwTvdIsCSTRIPE())
            {
                *retval = papi_fe_vip_MacroVisionColorstripeType2;
            }
            else if(fgHwTvdIsPSync())
            {
                *retval = papi_fe_vip_MacroVisionAgc;
            }
            else
            {
                *retval = papi_fe_vip_MacroVisionUnknown;
            }
        break;
        default:
            *retval = papi_fe_vip_MacroVisionInvalid;
        break;
    }

    return TM_OK;
}

static INT32 _papi_fe_vip_GetVideoProtection(int *retval) 
{
    UINT8 u1SrcType;

    VERIFY_NULL(retval);	

    u1SrcType = bApiVideoGetSrcType(SV_VP_MAIN);

    *retval = papi_fe_vip_VideoProtectionUnknown; /* assume no protection */
    switch(VSS_MAJOR(u1SrcType))
    {
        case VSS_CVBS:
        case VSS_SV:
            if(fgHwTvdIsMacroVision())
            {
                *retval = papi_fe_vip_VideoProtectionMacroVision;
            }
            /* TODO: check if CopyProtected (CGMS) */
        break;

        default:
            *retval = papi_fe_vip_VideoProtectionUnknown;
        break;
    }

    return TM_OK;
}

static INT32 _papi_fe_vip_IsPcFormat(UINT32 pixels, UINT32 lines, UINT32 scantype, Bool *ret)
{
    Bool fgIsPcFormat = FALSE;
    UINT8 bVgaTimingN = DVI_SEARCH_START;

    while(bVgaTimingN <= DVI_SEARCH_END)
    {
       if(pixels == Get_VGAMODE_IPH_WID(bVgaTimingN) &&
          lines == Get_VGAMODE_IPV_LEN(bVgaTimingN))
       {
           fgIsPcFormat = TRUE;
           break;
       } 
       bVgaTimingN++;
    }
    
    COPY_TO_USER(ret, &fgIsPcFormat, sizeof(Bool));

    return TM_OK;
}

int papi_ioctl_fe_vip(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg)
{
    IOCTL_4ARG_T rArg4;
    int i4Ret = 0;
    int i4ReturnValue = 0;

//    _ApiLock();
    if ((_IOC_SIZE(cmd)) > sizeof(unsigned long))
    {
        COPY_FROM_USER(arg, &rArg4, _IOC_SIZE(cmd));
    }
    else 
    {
        USR_SPACE_ACCESS_VALIDATE_1ARG(arg);	
    }

    switch (cmd)
    {
    case PAPI_IO_FE_VIP_GETVIDEOPRESENT:
        _papi_fe_vip_GetVideoPresent(&i4ReturnValue);
        break;
    case PAPI_IO_FE_VIP_GETIMAGEFORMAT:
        _papi_fe_vip_GetImageFormat(&i4ReturnValue);
        break;
    case PAPI_IO_FE_VIP_GETMACROVISIONTYPE:
        _papi_fe_vip_GetMacroVisionType(&i4ReturnValue);
        break;
    case PAPI_IO_FE_VIP_GETFIELDFREQUENCY:
        _papi_fe_vip_GetFieldFrequency(&i4ReturnValue);
        break;
    case PAPI_IO_FE_VIP_ISFILMMODE:
        _papi_fe_vip_IsFilmMode((Bool *)(&i4ReturnValue));
        break;
    case PAPI_IO_FE_VIP_GETNUMBEROFVISIBLEPIXELS:
        _papi_fe_vip_GetNumberOfVisiblePixels(&i4ReturnValue);
        break;
    case PAPI_IO_FE_VIP_ISFROMVCR:    
        _papi_fe_vip_IsFromVcr((Bool *)(&i4ReturnValue));
        break;
    case PAPI_IO_FE_VIP_GETSIGNALPRESENT:
        _papi_fe_vip_GetSignalPresent(&i4ReturnValue);
        break;
    case PAPI_IO_FE_VIP_GETINTERLACE:
        _papi_fe_vip_GetInterlace(&i4ReturnValue);
        break;
    case PAPI_IO_FE_VIP_GETNUMBEROFVISIBLELINES:
        _papi_fe_vip_GetNumberOfVisibleLines(&i4ReturnValue);
        break;
    case PAPI_IO_FE_VIP_GETVIDEOCODING:
        _papi_fe_vip_GetVideoCoding(&i4ReturnValue);
        break;
    case PAPI_IO_FE_VIP_GETNUMBEROFFRAMELINES:
        _papi_fe_vip_GetNumberOfFrameLines(&i4ReturnValue);
        break;
    case PAPI_IO_FE_VIP_GETVIDEOPROTECTION:    	
        _papi_fe_vip_GetVideoProtection(&i4ReturnValue);
        break;
    case PAPI_IO_FE_VIP_ISPCFORMAT:
        _papi_fe_vip_IsPcFormat(rArg4.au4Arg[0], rArg4.au4Arg[1], rArg4.au4Arg[2], rArg4.au4Arg[3]);
        break;
        
    default:
        printk("papi_ioctl_fe_vip: unknown IO command %u\n", cmd);
        i4Ret = -EFAULT;
    }

    if (i4Ret == 0)
    {
        COPY_TO_USER(arg, &i4ReturnValue, sizeof(int));
    }

//    _ApiUnlock();

    return i4Ret;
}


/* notification */
void PAPIFENotifyVideoPresentChanged(UINT32 u4VdpId)
{
    PAPI_FE_VIP_CB_T rSrcCb;

    // debug msg
    printk("VideoPresentChanged: %u\n", (unsigned int)u4VdpId);

    rSrcCb.i4Type = PAPI_CB_FE_VIP_ONVIDEOPRESENTCHANGED;
    _CB_PutEvent(CB_PAPI_FE_VIP_TRIGGER, sizeof (PAPI_FE_VIP_CB_T), &rSrcCb);
}

void PAPIFENotifySignalPresentChanged(UINT32 u4VdpId)
{
    PAPI_FE_VIP_CB_T rSrcCb;

    // debug msg
    printk("SignalPresentChanged: %u\n", (unsigned int) u4VdpId);

    rSrcCb.i4Type = PAPI_CB_FE_VIP_ONSIGNALPRESENTCHANGED;
    _CB_PutEvent(CB_PAPI_FE_VIP_TRIGGER, sizeof (PAPI_FE_VIP_CB_T), &rSrcCb);
}

void PAPIFENotifyFieldFrequencyChanged(UINT32 u4VdpId)
{
    PAPI_FE_VIP_CB_T rSrcCb;
    PAPI_VF_CB_T rVfCb;

    // debug msg
    printk("FieldFrequencyChanged: %u\n", (unsigned int)u4VdpId);

    rSrcCb.i4Type = PAPI_CB_FE_VIP_ONFIELDFREQUENCYCHANGED;
    _CB_PutEvent(CB_PAPI_FE_VIP_TRIGGER, sizeof (PAPI_FE_VIP_CB_T), &rSrcCb);

    rVfCb.i4Type = PAPI_CB_VF_VIO_ONFIELDFREQUENCYCHANGED;
    _CB_PutEvent(CB_PAPI_VF_TRIGGER, sizeof (PAPI_VF_CB_T), &rVfCb);
}

void PAPIFENotifyNumberOfVisibleLinesChanged(UINT32 u4VdpId)
{
    PAPI_FE_VIP_CB_T rSrcCb;

    // debug msg
    printk("NumberOfVisibleLinesChanged: %u\n", (unsigned int)u4VdpId);

    rSrcCb.i4Type = PAPI_CB_FE_VIP_ONNUMBEROFVISIBLELINESCHANGED;
    _CB_PutEvent(CB_PAPI_FE_VIP_TRIGGER, sizeof (PAPI_FE_VIP_CB_T), &rSrcCb);
}

void PAPIFENotifyNumberOfVisiblePixelsChanged(UINT32 u4VdpId)
{
    PAPI_FE_VIP_CB_T rSrcCb;

    // debug msg
    printk("NumberOfVisiblePixels: %u\n", (unsigned int)u4VdpId);

    rSrcCb.i4Type = PAPI_CB_FE_VIP_ONNUMBEROFVISIBLEPIXELSCHANGED;
    _CB_PutEvent(CB_PAPI_FE_VIP_TRIGGER, sizeof (PAPI_FE_VIP_CB_T), &rSrcCb);
}

void PAPIFENotifyNumberOfFrameLinesChanged(UINT32 u4VdpId)
{
    PAPI_FE_VIP_CB_T rSrcCb;
    PAPI_VF_CB_T rVfCb;

    // debug msg
    printk("NumberOfFrameLinesChanged: %u\n", (unsigned int)u4VdpId);

    rSrcCb.i4Type = PAPI_CB_FE_VIP_ONNUMBEROFFRAMELINESCHANGED;
    _CB_PutEvent(CB_PAPI_FE_VIP_TRIGGER, sizeof (PAPI_FE_VIP_CB_T), &rSrcCb);

    rVfCb.i4Type = PAPI_CB_VF_VIO_ONNUMBEROFLINESCHANGED;
    _CB_PutEvent(CB_PAPI_VF_TRIGGER, sizeof (PAPI_VF_CB_T), &rVfCb);
}

void PAPIFENotifyInterlaceChanged(UINT32 u4VdpId)
{
    PAPI_FE_VIP_CB_T rSrcCb;
    PAPI_VF_CB_T rVfCb;

    // debug msg
    printk("InterlaceChanged: %u\n", (unsigned int)u4VdpId);

    rSrcCb.i4Type = PAPI_CB_FE_VIP_ONINTERLACEDCHANGED;
    _CB_PutEvent(CB_PAPI_FE_VIP_TRIGGER, sizeof (PAPI_FE_VIP_CB_T), &rSrcCb);

    rVfCb.i4Type = PAPI_CB_VF_VIO_ONINTERLACEDCHANGED;
    _CB_PutEvent(CB_PAPI_VF_TRIGGER, sizeof (PAPI_VF_CB_T), &rVfCb);
}

void PAPIFENotifyImageFormatChanged(UINT32 u4VdpId)
{
    PAPI_FE_VIP_CB_T rSrcCb;

    // debug msg
    printk("ImageFormatChanged: %ud\n", (unsigned int)u4VdpId);

    rSrcCb.i4Type = PAPI_CB_FE_VIP_ONIMAGEFORMATCHANGED;
    _CB_PutEvent(CB_PAPI_FE_VIP_TRIGGER, sizeof (PAPI_FE_VIP_CB_T), &rSrcCb);
}

void PAPIFENotifyFilmModeChanged(UINT32 u4VdpId)
{
    PAPI_FE_VIP_CB_T rSrcCb;

    // debug msg
    printk("FilmModeChanged: %u\n", (unsigned int)u4VdpId);

    rSrcCb.i4Type = PAPI_CB_FE_VIP_ONFILMMODECHANGED;
    _CB_PutEvent(CB_PAPI_FE_VIP_TRIGGER, sizeof (PAPI_FE_VIP_CB_T), &rSrcCb);
}

void PAPIFENotifyFromVcrChanged(UINT32 u4VdpId)
{
    PAPI_FE_VIP_CB_T rSrcCb;

    // debug msg
    printk("FromVcrChanged: %u\n", (unsigned int)u4VdpId);

    rSrcCb.i4Type = PAPI_CB_FE_VIP_ONFROMVCRCHANGED;
    _CB_PutEvent(CB_PAPI_FE_VIP_TRIGGER, sizeof (PAPI_FE_VIP_CB_T), &rSrcCb);
}

void PAPIFENotifyVideoCodingChanged(UINT32 u4VdpId)
{
    PAPI_FE_VIP_CB_T rSrcCb;

    // debug msg
    printk("VideoCodingChanged: %u\n", (unsigned int)u4VdpId);

    rSrcCb.i4Type = PAPI_CB_FE_VIP_ONVIDEOCODINGCHANGED;
    _CB_PutEvent(CB_PAPI_FE_VIP_TRIGGER, sizeof (PAPI_FE_VIP_CB_T), &rSrcCb);
}

void PAPIFENotifyVideoProtectionChanged(UINT32 u4VdpId)
{
    PAPI_FE_VIP_CB_T rSrcCb;

    // debug msg
    printk("VideoProtectionChanged: %u\n", (unsigned int)u4VdpId);

    rSrcCb.i4Type = PAPI_CB_FE_VIP_ONVIDEOPROTECTIONCHANGED;
    _CB_PutEvent(CB_PAPI_FE_VIP_TRIGGER, sizeof (PAPI_FE_VIP_CB_T), &rSrcCb);
}

void PAPIFEVIPNotifyVideoChanged(NOTIFY_VIDEO_T tNotifyVideo)
{
    switch (tNotifyVideo.u4NotifyId)
    {
        case NOTIFICATION_VIDEO_SIGNALCHANGED:
              /* do nothing, since the signal status is by polling */
                break;
        case NOTIFICATION_VIDEO_VIDEOPRESENTCHANGED:
              PAPIFENotifyVideoPresentChanged(tNotifyVideo.u4VdpId);
        	break;
        case NOTIFICATION_VIDEO_SIGNALPRESENTCHANGED:
              PAPIFENotifySignalPresentChanged(tNotifyVideo.u4VdpId);
        	break;
        case NOTIFICATION_VIDEO_FIELDFREQUENCYCHANGED:
              PAPIFENotifyFieldFrequencyChanged(tNotifyVideo.u4VdpId);
        	break;
        case NOTIFICATION_VIDEO_NUMBEROFVISIBLELINESCHANGED:
              PAPIFENotifyNumberOfVisibleLinesChanged(tNotifyVideo.u4VdpId);
        	break;
        case NOTIFICATION_VIDEO_NUMBEROFVISIBLEPIXELSCHANGED:
              PAPIFENotifyNumberOfVisiblePixelsChanged(tNotifyVideo.u4VdpId);
        	break;
        case NOTIFICATION_VIDEO_NUMBEROFFRAMELINESCHANGED:
              PAPIFENotifyNumberOfFrameLinesChanged(tNotifyVideo.u4VdpId);
        	break;
        case NOTIFICATION_VIDEO_INTERLACEDCHANGED:
              PAPIFENotifyInterlaceChanged(tNotifyVideo.u4VdpId);
        	break;
        case NOTIFICATION_VIDEO_IMAGEFORMATCHANGED:
              PAPIFENotifyImageFormatChanged(tNotifyVideo.u4VdpId);
        	break;
        case NOTIFICATION_VIDEO_FILMMODECHANGED:
              PAPIFENotifyFilmModeChanged(tNotifyVideo.u4VdpId);
        	break;
        case NOTIFICATION_VIDEO_FROMVCRCHANGED:
              PAPIFENotifyFromVcrChanged(tNotifyVideo.u4VdpId);
        	break;
        case NOTIFICATION_VIDEO_VIDEOCODINGCHANGED:
              PAPIFENotifyVideoCodingChanged(tNotifyVideo.u4VdpId);
        	break;
        case NOTIFICATION_VIDEO_VIDEOPROTECTIONCHANGED:
              PAPIFENotifyVideoProtectionChanged(tNotifyVideo.u4VdpId);
        	break;
        default:
              printk("papi_ioctl_fe_vip: unknown notification command %u\n", (unsigned int)tNotifyVideo.u4NotifyId);
        	break;
    }

}


static UINT8 u1FromVcr = 0, u1FilmMode = 0, u1VideoCoding = 0, u1VideoProtection= 0, u1ImageFormat = 0;
static VMixNewVidLayerProp rVmxVlProp = {0}; /* changes initial value is zero */
static UINT8 u1SignalPresent = 0;

INT32 papi_af_smt_Auto_Mute_Output(UINT32 u4Arg);

void PAPIFEVIPNotify()
{
    static UINT8 /*u1SignalPresent = 0,*/ u1VideoPresent = 0, u1Interlaced = 0;
    static UINT8 u1FieldFreq = 0;
    static UINT16 u2NoOfVisibleLines = 0, u2NoOfVisiblePixels = 0, u2NoOfFrameLines = 0;
    int i4tmp;

    if(u1SignalPresent != (i4tmp = bDrvVideoSignalStatus(SV_VP_MAIN)))
    {
        vApiNotifySignalPresentChanged(i4tmp);
        u1SignalPresent = i4tmp;
        if(u1SignalPresent == SV_VDO_STABLE)
        {
            /* temporarily un-mute audio here */
            papi_af_smt_Auto_Mute_Output(0);
        }
        else /* unknown | nosignal */
        {
            /* let's temporarily mute audio here */
            papi_af_smt_Auto_Mute_Output(1);
            /* signal changed */
            vApiNotifyVideoPresentChanged(0);
        }
    }
    

    if(u1SignalPresent == SV_VDO_NOSIGNAL || u1SignalPresent == SV_VDO_UNKNOWN)
    {
        u1VideoPresent = 0;
        u1FieldFreq = 0;
        u2NoOfVisibleLines = 0;
        u2NoOfVisiblePixels = 0;
        u2NoOfFrameLines = 0;

        u1VideoProtection = 0;
        u1FilmMode = 0;
        u1FromVcr = 0;

        return;
    }
    
    if(u1VideoPresent != (i4tmp = bDrvVideoGetTiming(SV_VP_MAIN)))
    {
        vApiNotifyVideoPresentChanged(i4tmp);
        u1VideoPresent = i4tmp;
    }

    if(u1FieldFreq != (i4tmp=bDrvVideoGetRefreshRate(SV_VP_MAIN)))
    {
        vApiNotifyFieldFrequencyChanged(i4tmp);
        u1FieldFreq = i4tmp;
    }

    if(u1Interlaced != (i4tmp=bDrvVideoIsSrcInterlace(SV_VP_MAIN)))
    {
        vApiNotifyInterlacedChanged(i4tmp);
        u1Interlaced = i4tmp;
    }


    if(u2NoOfVisiblePixels != (i4tmp=wApiVideoGetInputWidth(SV_VP_MAIN)))
    {
        vApiNotifyNumberOfVisiblePixelsChanged(i4tmp);
        u2NoOfVisiblePixels = i4tmp;
        rVmxVlProp.changes |= VMix_ResolutionChanged;
    }

    if(u2NoOfVisibleLines != (i4tmp=wDrvVideoInputHeight(SV_VP_MAIN)))
    {
        vApiNotifyNumberOfVisibleLinesChanged(i4tmp);
        u2NoOfVisibleLines = i4tmp;
        rVmxVlProp.changes |= VMix_ResolutionChanged;
    }

    if(u2NoOfFrameLines != (i4tmp=wDrvVideoGetVTotal(SV_VP_MAIN)))
    {
        vApiNotifyNumberOfFrameLinesChanged(i4tmp);
        u2NoOfFrameLines = i4tmp;
    }


    if(rVmxVlProp.changes != 0)
    {
        rVmxVlProp.status = VMix_ValStatKnown;

        rVmxVlProp.horResolution = u2NoOfVisiblePixels;
        rVmxVlProp.vertResolution = u2NoOfVisibleLines;
    }
}


void PAPIFEVIPNotify_ModeNotChange()
{
    static UINT8 u1VideoProtection;
    int i4tmp;

    if(u1SignalPresent == SV_VDO_NOSIGNAL || u1SignalPresent == SV_VDO_UNKNOWN)
    {
        return;
    }

    if( bGetVideoDecType(SV_VP_MAIN) == SV_VD_TVD3D &&
        u1FromVcr != (i4tmp=(fgHwTvdTrick()||fgHwTvdHeadSwitch())))
    {
        vApiNotifyFromVcrChanged(i4tmp);
        u1FromVcr = i4tmp;
    }

    _papi_fe_vip_IsFilmMode(&i4tmp);
    if(u1FilmMode != i4tmp) 
    {
        vApiNotifyFilmModeChanged(i4tmp);
        u1FilmMode = i4tmp; 
    }

    _papi_fe_vip_GetVideoCoding(&i4tmp);
    if(u1VideoCoding != i4tmp)
    {
        vApiNotifyVideoCodingChanged(i4tmp);
        u1VideoCoding = i4tmp;
    }

    _papi_fe_vip_GetVideoProtection(&i4tmp);
    if(u1VideoProtection != i4tmp)
    {
        vApiNotifyVideoProtectionChanged(i4tmp);
        u1VideoProtection = i4tmp;
    }

    _papi_fe_vip_GetImageFormat(&i4tmp);
    if(u1ImageFormat != i4tmp)
    {
        vApiNotifyImageFormatChanged(i4tmp);
        u1ImageFormat = i4tmp;

        rVmxVlProp.changes |= VMix_AspectRatioChanged;
        switch(i4tmp)
        {
            case papi_fe_vip_ImageFormat4by3FullFormat:
                rVmxVlProp.aspectRatioNumerator = 4;
                rVmxVlProp.aspectRatioDenominator = 3;
                break;
            case papi_fe_vip_ImageFormat14by9Top:
            case papi_fe_vip_ImageFormat14by9Center:
                rVmxVlProp.aspectRatioNumerator = 14;
                rVmxVlProp.aspectRatioDenominator = 9;
                break;
            case papi_fe_vip_ImageFormat16by9Top:
            case papi_fe_vip_ImageFormat16by9Center:
            case papi_fe_vip_ImageFormat16by9FullFormat:
                rVmxVlProp.aspectRatioNumerator = 16;
                rVmxVlProp.aspectRatioDenominator = 9;
                break;
            default:
                /* unknown */
                rVmxVlProp.aspectRatioNumerator = 0;
                rVmxVlProp.aspectRatioDenominator = 1;
                break;
        }

    }

    if(rVmxVlProp.changes != 0)
    {
        rVmxVlProp.changes |= VMix_ActiveImageAreaChanged;

        rVmxVlProp.activeImageArea.ul.x = 0;
        rVmxVlProp.activeImageArea.ul.y = 0;

        rVmxVlProp.activeImageArea.lr.x = rVmxVlProp.horResolution;
        rVmxVlProp.activeImageArea.lr.y = rVmxVlProp.vertResolution;

        vApiNotifyMixerNewProperties(&rVmxVlProp);
        rVmxVlProp.changes = 0; 
    }
}

