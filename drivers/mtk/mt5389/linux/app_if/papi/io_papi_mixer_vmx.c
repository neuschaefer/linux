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
 * $RCSfile: io_papi_mixer_vmx.c,v $
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
#include "papi_papionly.h"
#include "drvcust_if.h"
#include "nptv_if.h"
#include "pmx_drvif.h"
#include "vdp_if.h"


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define NumOfSupportLayers 3

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define COPY_TO_USER(USER_PARAMETER, KERNEL_PARAMETER, SIZE)                   \
    do                                                                         \
    {                                                                          \
        if (USER_PARAMETER == NULL)                                            \
        {                                                                      \
            printk("%s verify null fail\n", __FUNCTION__);                     \
            return TM_ERR_BAD_PARAMETER;                                       \
        }                                                                      \
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
        if (USER_PARAMETER == NULL)                                            \
        {                                                                      \
            printk("%s verify null fail\n", __FUNCTION__);                     \
            return TM_ERR_BAD_PARAMETER;                                       \
        }                                                                      \
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
static VMixColor _rVmxBgColor;
static VMixColor _rVmxBlankColor;
static Bool _bVmxBlanked=0;
static Bool _fgBlankOnFieldRateChangedEnabled = FALSE;
static Bool _fgBlankOnOutputResolutionChangedEnabled = FALSE;
static Bool _fgBlankOnScanTypeChangedEnabled = FALSE;
static tmDigVdec_ScanType_t rScanType = tmDigVdec_Progressive;



//-----------------------------------------------------------------------------
// External function prototypes
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static INT32 _ioctl_GetBgColor(UINT32 u4Arg)
{ 
    // Copy data to user memory
    COPY_TO_USER(u4Arg, &_rVmxBgColor, sizeof(VMixColor));
    return TM_OK;
}

static INT32 _ioctl_SetBgColor(UINT32 u4Arg)
{ 
    VMixColor rMixBgColor;
    UINT32 BgColor;

    // Copy arguments from user space
    COPY_FROM_USER(u4Arg, &rMixBgColor, sizeof(VMixColor));
    BgColor = (((UINT32)rMixBgColor.redOrY)<<16)|(((UINT32)rMixBgColor.greenOrU)<<8)|((UINT32)rMixBgColor.blueOrV);
    PMX_SetBg(BgColor, 0);
    _rVmxBgColor.redOrY=rMixBgColor.redOrY;
    _rVmxBgColor.greenOrU=rMixBgColor.greenOrU;
    _rVmxBgColor.blueOrV=rMixBgColor.blueOrV;
   
    return TM_OK;
}

static INT32 _ioctl_BlankOutput(UINT32 u4Arg)
{
     IOCTL_2ARG_T rArg;
     Bool bVmxBlank;
     VMixColor rVmxBlankColor;
     UINT32 BlankColor;

    // Copy arguments from user space
    COPY_FROM_USER(u4Arg, &rArg, sizeof(IOCTL_2ARG_T));

    // Get arguments
    bVmxBlank = (Bool)rArg.au4Arg[0];
    COPY_FROM_USER(rArg.au4Arg[1], &rVmxBlankColor, sizeof(VMixColor));

    if(bVmxBlank) //blank Vmx output
    {
        BlankColor = (((UINT32)rVmxBlankColor.redOrY)<<16)|(((UINT32)rVmxBlankColor.greenOrU)<<8)|((UINT32)rVmxBlankColor.blueOrV);
        PMX_SetBg(BlankColor, 0);
        PMX_SetAllPlaneHide(True);
        _rVmxBlankColor.redOrY=rVmxBlankColor.redOrY;
        _rVmxBlankColor.greenOrU=rVmxBlankColor.greenOrU;
        _rVmxBlankColor.blueOrV=rVmxBlankColor.blueOrV;
    }
    else //unblank Vmx output
    {
        PMX_SetAllPlaneHide(False);
    }
    _bVmxBlanked=bVmxBlank;
    
    return TM_OK;
}

static INT32 _ioctl_GetOutputBlanked(UINT32 u4Arg)
{
    IOCTL_2ARG_T rArg;
    Bool* pOutputBlank;
    VMixColor* pVmxBlankColor;

    // Copy arguments from user space
    COPY_FROM_USER(u4Arg, &rArg, sizeof(IOCTL_2ARG_T));

    // Get arguments
    pOutputBlank = (Bool*)rArg.au4Arg[0];
    pVmxBlankColor = (VMixColor*)rArg.au4Arg[1];

    // Copy data to user memory
    COPY_TO_USER(pOutputBlank, &_bVmxBlanked, sizeof(Bool));
    COPY_TO_USER(pVmxBlankColor, &_rVmxBlankColor, sizeof(VMixColor));
  
    return TM_OK;
}


static INT32 _ioctl_GetSuppNRLayers(UINT32 u4Arg)
{
    UINT32 rSuppNRLayers;
    rSuppNRLayers= NumOfSupportLayers;
    // Copy data to user memory
    COPY_TO_USER(u4Arg, &rSuppNRLayers, sizeof(UINT32));
    return TM_OK;
}


/**  @ingroup mixer_functions */
/**  @brief Returns the auto blanking settings in the platform. */
/**  @param pBlankOnOutputResolutionChangedEnabled True, if the output is blanked if the output resolution changes. */
/**  @param pBlankOnFieldRateChangedEnabled True, if the output is blanked if the field rate changes. */
/**  @param pBlankOnScanTypeChangedEnabled True, if the output is blanked if the scan type changes. */
/**  @param pBlankColor If the output is auto blanked, the blanking color that is shown. */
static INT32 _papi_mixer_vmx_GetAutoBlankEnabled(Bool * pBlankOnOutputResolutionChangedEnabled,
                                                 Bool * pBlankOnFieldRateChangedEnabled,
                                                 Bool * pBlankOnScanTypeChangedEnabled,
                                                 VMixColor * pBlankColor)
{
    COPY_TO_USER(pBlankOnOutputResolutionChangedEnabled, &_fgBlankOnOutputResolutionChangedEnabled, sizeof(Bool));
    COPY_TO_USER(pBlankOnFieldRateChangedEnabled, &_fgBlankOnFieldRateChangedEnabled, sizeof(Bool));
    COPY_TO_USER(pBlankOnScanTypeChangedEnabled, &_fgBlankOnScanTypeChangedEnabled, sizeof(Bool));
    COPY_TO_USER(pBlankColor, &_rVmxBlankColor, sizeof(VMixColor));

    return TM_OK;
}


/**  @ingroup mixer_functions */
/**  @brief Returns the output properties of the video mixer. */
/**  @param pWidth Output parameter with the number of pixels per line. */
/**  @param pHeight Output parameter with the number of lines. */
/**  @param pFieldRate Output parameter with the field rate. See datatype \ref tmIVmix_FieldRate. */
/**  @param pScanType Output parameter with the scan type. See datatype \ref tmIVmix_ScanType. */
static INT32 _papi_mixer_vmx_GetOutputProperties(Nat32 * pWidth,
                                                 Nat32 * pHeight,
                                                 int * pFieldRate,
                                                 int * pScanType)
{
    UINT32 value;
    value = PANEL_GetPanelWidth();
    COPY_TO_USER(pWidth, &value, sizeof(Nat32));
    value = PANEL_GetPanelHeight();
    COPY_TO_USER(pHeight, &value, sizeof(Nat32));
    value = vDrvGetLCDFreq();
    COPY_TO_USER(pFieldRate, &value, sizeof(int));
    switch(rScanType)
    {
        case tmDigVdec_Progressive:
            value = papi_papionly_vmix_ScanTypeProgressive;
            break;
        case tmDigVdec_Interlaced:
            value = papi_papionly_vmix_ScanTypeInterlaced;
            break;
        case tmDigVdec_UnknownScanType:
            value = papi_papionly_vmix_ScanTypeUnknown;
            break;
        default:
            value = papi_papionly_vmix_ScanTypeInvalid;
            break;
    }
    COPY_TO_USER(pScanType, &value, sizeof(int));

    return TM_OK;
}


/**  @ingroup mixer_functions */
/**  @brief Enable the auto blanking behaviour in the platform. */
/**  @param enableOnOutputResolutionChanged True, to blank the output if the output resolution changes. */
/**  @param enableOnFieldRateChanged True, to blank the output if the field rate changes. */
/**  @param enableOnScanTypeChanged True, to blank the output if the scan type changes. */
/**  @param blankColor If the output is auto blanked, the blanking color that needs to be shown. */
static INT32 _papi_mixer_vmx_EnableAutoBlank(Bool enableOnOutputResolutionChanged,
                                             Bool enableOnFieldRateChanged,
                                             Bool enableOnScanTypeChanged,
                                             VMixColor * pBlankColor)
{
    _fgBlankOnFieldRateChangedEnabled = enableOnFieldRateChanged;
    VDP_SetBlankedReason(BLANK_FRAMERATE_CHANGED, _fgBlankOnFieldRateChangedEnabled);
    _fgBlankOnOutputResolutionChangedEnabled = enableOnOutputResolutionChanged;
    VDP_SetBlankedReason(BLANK_RESOLUTION_CHANGED, _fgBlankOnOutputResolutionChangedEnabled);
    // no support
    // _fgBlankOnScanTypeChangedEnabled = enableOnScanTypeChanged;
    // no support
    // COPY_FROM_USER(pBlankColor, &_rBlankColor, sizeof(VMixColor));


    return TM_OK;
}


/**  @ingroup mixer_functions */
/**  @brief Deactivate auto blank. */
static INT32 _papi_mixer_vmx_AutoBlankUnblank(void)
{
    _fgBlankOnFieldRateChangedEnabled = FALSE;
    _fgBlankOnOutputResolutionChangedEnabled = FALSE;
    _fgBlankOnScanTypeChangedEnabled = FALSE;

    VDP_SetBlankedReason(BLANK_FRAMERATE_CHANGED|BLANK_RESOLUTION_CHANGED, 0);

    return TM_OK;
}



//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

int papi_ioctl_mixer_vmx(struct inode *inode, struct file *file,
                         unsigned int cmd, unsigned long arg)
{
    IOCTL_4ARG_T rArg4;
    int i4Ret = 0;

    if (_IOC_SIZE(cmd) > sizeof(unsigned long))
    {
        COPY_FROM_USER(arg, &rArg4, _IOC_SIZE(cmd));
    }

    switch (cmd)
    {
    case PAPI_IO_MIXER_VMX_GETBGCOLOR:
    	i4Ret = _ioctl_GetBgColor(arg);
    	break;
    case PAPI_IO_MIXER_VMX_BLANKOUTPUT:
    	i4Ret = _ioctl_BlankOutput(arg);
    	break;
    case PAPI_IO_MIXER_VMX_SETBGCOLOR:
    	i4Ret = _ioctl_SetBgColor(arg);
    	break;
    case PAPI_IO_MIXER_VMX_GETSUPPNRLAYERS:
    	i4Ret = _ioctl_GetSuppNRLayers(arg);
    	break;
    case PAPI_IO_MIXER_VMX_GETOUTPUTBLANKED:
    	i4Ret = _ioctl_GetOutputBlanked(arg);
    	break;


    case PAPI_IO_MIXER_VMX_GETAUTOBLANKENABLED:
        i4Ret =  _papi_mixer_vmx_GetAutoBlankEnabled((Bool*)rArg4.au4Arg[0],
                                                   (Bool*)rArg4.au4Arg[1],
                                                   (Bool*)rArg4.au4Arg[2],
                                                   (VMixColor*)rArg4.au4Arg[3]);
        break;

    case PAPI_IO_MIXER_VMX_ENABLEAUTOBLANK:
        i4Ret =  _papi_mixer_vmx_EnableAutoBlank((Bool)rArg4.au4Arg[0],
                                               (Bool)rArg4.au4Arg[1],
                                               (Bool)rArg4.au4Arg[2],
                                               (VMixColor*)rArg4.au4Arg[3]);
        break;

    case PAPI_IO_MIXER_VMX_AUTOBLANKUNBLANK:
        i4Ret = _papi_mixer_vmx_AutoBlankUnblank();
    	break;

    case PAPI_IO_MIXER_VMX_GETOUTPUTPROPERTIES:
        i4Ret =  _papi_mixer_vmx_GetOutputProperties((Nat32*)rArg4.au4Arg[0],
                                                   (Nat32*)rArg4.au4Arg[1],
                                                   (int*)rArg4.au4Arg[2],
                                                   (int*)rArg4.au4Arg[3]);
        break;

    default:
        printk("papi_ioctl_mixer_vmx: unknown IO command %u\n", cmd);
        return TM_ERR_BAD_PARAMETER;
    }

    return i4Ret;
}

/* notification */
void PAPIMIXERNotifyNewProperties(UINT32 u4VdpId)
{
    PAPI_MIXER_CB_T rCb;
    //VMixNewVidLayerProp *prVmxVlProp = (VMixNewVidLayerProp*)u4VdpId;

    //debug msg
    printk("PAPI_MIXER_VMX_ONNEWPROPERTIES\n");

    rCb.i4Type = PAPI_CB_MIXER_VMX_ONNEWPROPERTIES;

    // cookie
    rCb.u4Data1 = 0;
    rCb.u4Data2 = 0; /* oRes changed */
    rCb.u4Data5 = 0; /* fieldRate changed */
    rCb.u4Data7 = 0; /* scanType changed */

    rCb.u4Data3 = PANEL_GetPanelWidth();/* width */
    rCb.u4Data4 = PANEL_GetPanelHeight(); /* height */

    rCb.u4Data6 = vDrvGetLCDFreq(); /* field rate */

    /* scanType */
    switch(rScanType)
    {
        case tmDigVdec_Progressive:
            rCb.u4Data8 = papi_papionly_vmix_ScanTypeProgressive;
            break;
        case tmDigVdec_Interlaced:
            rCb.u4Data8 = papi_papionly_vmix_ScanTypeInterlaced;
            break;
        case tmDigVdec_UnknownScanType:
            rCb.u4Data8 = papi_papionly_vmix_ScanTypeUnknown;
            break;
        default:
            rCb.u4Data8 = papi_papionly_vmix_ScanTypeInvalid;
            break;
    }

    rCb.u4Data9 = NumOfSupportLayers;

    rCb.u4Data10 = 0; //prVmxVlProp->changes;
    rCb.u4Data11 = 0; //prVmxVlProp->status;
    rCb.u4Data12 = rCb.u4Data3; //prVmxVlProp->horResolution;
    rCb.u4Data13 = rCb.u4Data4; //prVmxVlProp->vertResolution;
    rCb.u4Data14 = 16; //prVmxVlProp->aspectRatioNumerator;
    rCb.u4Data15 = 9; //prVmxVlProp->aspectRatioDenominator;
    rCb.u4Data16 = 0; //prVmxVlProp->activeImageArea.ul.x;
    rCb.u4Data17 = 0; //prVmxVlProp->activeImageArea.ul.y;
    rCb.u4Data18 = rCb.u4Data3; //prVmxVlProp->activeImageArea.lr.x;
    rCb.u4Data19 = rCb.u4Data4; //prVmxVlProp->activeImageArea.lr.y;

    _CB_PutEvent(CB_PAPI_MIXER_TRIGGER, sizeof(PAPI_MIXER_CB_T), &rCb);
}

void PAPIMIXERNotify(NOTIFY_MIXER_VIDEOLAYER_T tNotify)
{
    switch(tNotify.u4NotifyId)
    {
        case NOTIFICATION_MIXER_NEWPROPERTIES:
            PAPIMIXERNotifyNewProperties(tNotify.u4VdpId);
            break;
        default:
            printk("papi_mixer_vmx: unknown notification command %u\n", tNotify.u4NotifyId);
            break;
    }
}

