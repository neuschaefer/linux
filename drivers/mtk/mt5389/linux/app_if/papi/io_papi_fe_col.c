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
 * $RCSfile: io_papi_fe_col.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_papi_fe_col.c
 *  PAPI kernel driver implementation: Color Decode
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include <linux/papi_ioctl.h>
#include <linux/mt53xx_cb.h>
#include "papi_fe.h"
#include "drv_tvd.h"
#include "drv_tdc.h"
#include "sv_const.h"
#include "nptv_if.h"
#include "video_def.h"
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define TM_FAIL                     (-0x800)

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
        return TM_FAIL ; \
    } \
    if (copy_from_user((dst), (void __user *)(src), (size))) \
    { \
        return TM_FAIL ; \
    }

/// Copy data from kernel space to user space
#define COPY_TO_USER(dst, src, size) \
    if (!access_ok(VERIFY_WRITE, (void __user *)(dst), (size))) \
    { \
        return TM_FAIL ; \
    } \
    if (copy_to_user((void __user *)(dst), (src), (size))) \
    { \
        return TM_FAIL ; \
    }

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// External function prototypes
//-----------------------------------------------------------------------------
EXTERN INT32 _CB_PutEvent(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
/**
 * @brief  color system of TVDecoder
 *
 * return the Tvdmode of TVDecoder
 *
 * @return bColSys bColSys specifies the desired color system, it can be SV_CS_PAL_N,
 * SV_CS_PAL, SV_CS_PAL_M, SV_CS_NTSC358, SV_CS_SECAM, SV_CS_PAL_60, SV_CS_NTSC443, and
 * SV_CS_AUTO (for auto mode detection).
 */

int ColorandFreq_to_Tvdmode(int ColorSystem, int Freq)
{
    int i4Ret = 0;

    switch(ColorSystem)
    {
        case papi_fe_col_Pal:
            if(Freq == papi_fe_col_Freq4pt433619)
            {
                i4Ret = SV_CS_PAL;
            }
            else if(Freq == papi_fe_col_Freq3pt575611)
            {
                i4Ret = SV_CS_PAL_M;
            }
            else if(Freq == papi_fe_col_Freq3pt582056)
            {
                i4Ret = SV_CS_PAL_N;
            }
            else
            {
                i4Ret = SV_CS_NONE;
            }            
        break;
        case papi_fe_col_Secam:
            if(Freq == papi_fe_col_Freq4pt433619)
            {
                i4Ret = SV_CS_SECAM;
            }
            else
            {
                i4Ret = SV_CS_NONE;
            }
        break;
        case papi_fe_col_Ntsc:
            if(Freq == papi_fe_col_Freq3pt579545)
            {
                i4Ret = SV_CS_NTSC358;
            }
            else if(Freq == papi_fe_col_Freq4pt433619)
            {
                i4Ret = SV_CS_NTSC443;
            }
            else
            {
                i4Ret = SV_CS_NONE;
            }
        break;
        default:
            i4Ret = SV_CS_NONE;
        break;
    }
    //printk("ColorandFreq_to_Tvdmode = %d\n", i4Ret);
    return i4Ret;    
}

int Tvdmode_to_Freq(int Tvdmode)
{
    int i4Ret = 0;

    switch(Tvdmode)
    {
        case SV_CS_PAL:
        case SV_CS_SECAM:
        case SV_CS_NTSC443:
            i4Ret = papi_fe_col_Freq4pt433619;
        break;
        case SV_CS_PAL_M:
            i4Ret = papi_fe_col_Freq3pt575611;
        break;
        case SV_CS_PAL_N:
            i4Ret = papi_fe_col_Freq3pt582056;
        break;
        case SV_CS_NTSC358:
            i4Ret = papi_fe_col_Freq3pt579545;
        break;
        default:
            i4Ret = papi_fe_col_FreqUnknown;
        break;
    }

    return i4Ret;    
}

int Tvdmode_to_Color(int Tvdmode)
{
    int i4Ret = 0;

    switch(Tvdmode)
    {
        case SV_CS_PAL:
        case SV_CS_PAL_M:
        case SV_CS_PAL_N:
            i4Ret = papi_fe_col_Pal;
        break;
        case SV_CS_SECAM:
            i4Ret = papi_fe_col_Secam;
        break;
        case SV_CS_NTSC358:
        case SV_CS_NTSC443:
            i4Ret = papi_fe_col_Ntsc;
        break;
        default:
            i4Ret = papi_fe_col_UnknownColorSystem;
        break;
    }

    return i4Ret;    
}

    UINT8 SuggestEnable = 0;

static Bool _papi_fe_col_Suggested(int i4Ret)
{
    switch(i4Ret)
    {
        case SV_CS_PAL_N:
        case SV_CS_PAL:
        case SV_CS_PAL_M:
        case SV_CS_NTSC358:
        case SV_CS_SECAM:
        case SV_CS_PAL_60:
        case SV_CS_NTSC443:
            bTvdCtrl(TCTL_DFTMODE,TC_ENABLE,(UINT32)i4Ret);
            SuggestEnable = 1;
            return TRUE;
        default:
            SuggestEnable = 0;
            return FALSE;
    }
}

void papi_NotifyColorSystem(void)
{
    PAPI_FE_COL_CB_T rFeColCb;

    rFeColCb.i4Type = PAPI_CB_FE_COL_ONCOLORSYSTEMCHANGED; // 
    _CB_PutEvent(CB_PAPI_FE_COL_TRIGGER, sizeof (PAPI_FE_COL_CB_T),
        &rFeColCb);

}

void papi_NotifyCombFilter(void)
{
    PAPI_FE_COL_CB_T rFeColCb;

    rFeColCb.i4Type = PAPI_CB_FE_COL_ONCOMBFILTERACTIVECHANGED; // 
    _CB_PutEvent(CB_PAPI_FE_COL_TRIGGER, sizeof (PAPI_FE_COL_CB_T),
        &rFeColCb);

}

static UINT8 u1ColorSystem = 0xff, u1CombFilter = 0;
void PAPIFECOLNotify(void)
{
    static UINT8 u1SignalPresent = 0;
    UINT8 u1tmp;

    u1SignalPresent = bDrvVideoSignalStatus(SV_VP_MAIN);
    
    if(u1SignalPresent == SV_VDO_NOSIGNAL || u1SignalPresent == SV_VDO_UNKNOWN)
    {
        u1ColorSystem = 0xff;
        return;
    }
    
   if(u1ColorSystem != (u1tmp = bHwTvdMode()))
    {
        vApiNotifyColorSystemChanged();
        u1ColorSystem = u1tmp;
    }
}

void PAPIFECOLNotify_ModeNotChange(void)
{
    UINT8 u1tmp;

    if(u1CombFilter != (u1tmp = _bTDCEnable))
    {
        vApiNotifyCombFilterActiveChanged();
        u1CombFilter = u1tmp;
    }
}

static INT32 _papi_fe_col_ColorSystemSupportedEx (unsigned long u4Arg) 
{
    IOCTL_3ARG_T rArg;
    int i4Ret = 0;
    int colorsystem;
    int carrierfrequency;

    COPY_FROM_USER(&rArg, (void*)u4Arg, sizeof(IOCTL_3ARG_T));
    
    colorsystem = rArg.au4Arg[0];
    carrierfrequency = rArg.au4Arg[1];
    i4Ret = ColorandFreq_to_Tvdmode(colorsystem, carrierfrequency);
    if(i4Ret != SV_CS_NONE) 
    {
        rArg.au4Arg[2] = TRUE;
    }
    else
    {
        rArg.au4Arg[2] = FALSE;
    }
    COPY_TO_USER((void*)u4Arg, &rArg, sizeof(IOCTL_3ARG_T));

    return TM_OK;
}

    int Forcedcolorsystem = 0;
    int Forcedcarrierfreq = 0;
static INT32 _papi_fe_col_ForceColorSystemEx (unsigned long u4Arg)//int colorsystem, int carrierfrequency 
{
    IOCTL_2ARG_T rArg;
    int i4Ret = 0;

    COPY_FROM_USER(&rArg, (void*)u4Arg, sizeof(IOCTL_2ARG_T));

    Forcedcolorsystem = rArg.au4Arg[0];
    Forcedcarrierfreq = rArg.au4Arg[1];

    i4Ret = ColorandFreq_to_Tvdmode(Forcedcolorsystem, Forcedcarrierfreq);
    //printk("ForceColorSystemEx = %d\n",i4Ret);
    if(i4Ret != SV_CS_NONE) 
    {
        if(fgApiVideoColorSystem((Nat8)i4Ret))
        {
            //printk("_papi_fe_col_ForceColorSystemEx TM_OK\n");
            return TM_OK;
        }
        else
        {
            printk("Not TVD Source\n");
            return TM_FAIL;
        }
    }
    else
    {
        printk("Unknown ColorSystem and CarrierFrequency\n");
        return TM_FAIL;
    }
}

static INT32 _papi_fe_col_GetForcedColorSystem(unsigned long u4Arg)
{
    int rArg;

    rArg = Forcedcolorsystem;
    COPY_TO_USER((void*)u4Arg, &rArg, sizeof(int));

    return TM_OK;
}

static INT32 _papi_fe_col_GetForcedCarrierFrequency (unsigned long u4Arg)
{
    int rArg;

    rArg = Forcedcarrierfreq;
    COPY_TO_USER((void*)u4Arg, &rArg, sizeof(int));

    return TM_OK;
}

    int Suggestedcolorsystem = 0;
    int Suggestedfreq = 0;
static INT32 _papi_fe_col_SuggestColorSystemEx (unsigned long u4Arg) 
{
    IOCTL_2ARG_T rArg;
    int i4Ret = 0;
    
    COPY_FROM_USER(&rArg, (void*)u4Arg, sizeof(IOCTL_2ARG_T));

    Suggestedcolorsystem = rArg.au4Arg[0];
    Suggestedfreq = rArg.au4Arg[1];

    i4Ret = ColorandFreq_to_Tvdmode(Suggestedcolorsystem, Suggestedfreq);
    if(_papi_fe_col_Suggested(i4Ret))
    {
         return TM_OK;
    }
    else
    {
        printk("Input parameter fail, return to Auto\n");
         return TM_FAIL;
    }
    
    return TM_OK;
}

static INT32 _papi_fe_col_GetSuggestedColorSystem (unsigned long u4Arg) 
{
    int rArg;
    
    rArg = Suggestedcolorsystem;
    COPY_TO_USER((void*)u4Arg, &rArg, sizeof(int));

    return TM_OK;
}

static INT32 _papi_fe_col_GetSuggestedCarrierFrequency (unsigned long u4Arg)
{
    int rArg;
    
    rArg = Suggestedfreq;
    COPY_TO_USER((void*)u4Arg, &rArg, sizeof(int));

    return TM_OK;
}

INT32 _papi_fe_col_GetColorSystem (int *retval) 
{
    *retval = Tvdmode_to_Color(bApiVideoGetColorSystem());

    return TM_OK;
}

static INT32 _papi_fe_col_GetColorSystem2 (unsigned long u4Arg)
{
    int rArg;

    rArg = Tvdmode_to_Color(bApiVideoGetColorSystem());
    COPY_TO_USER((void*)u4Arg, &rArg, sizeof(int));

    return TM_OK;
}

static INT32 _papi_fe_col_GetCarrierFrequency (unsigned long u4Arg)
{
    int rArg;
    
    rArg = Tvdmode_to_Freq(bApiVideoGetColorSystem());
    COPY_TO_USER((void*)u4Arg, &rArg, sizeof(int));

    return TM_OK;
}

static INT32 _papi_fe_col_CombFilterSupported (unsigned long u4Arg) 
{
    Bool rArg;

    rArg = (Bool)(fgDrvTDCEnableChk() && (( _rMChannel.bDecType==SV_VD_TVD3D)||( _rPChannel.bDecType==SV_VD_TVD3D)));
    COPY_TO_USER((void*)u4Arg, &rArg, sizeof(Bool));

    return TM_OK;
}

static INT32 _papi_fe_col_EnableCombFilter (unsigned long u4Arg) 
{
    Bool rArg;
    Nat8 mode;
    
    COPY_FROM_USER(&rArg, (void*)u4Arg, sizeof(Bool));
    mode = (Nat8)rArg;
    vDrvTDCOnOff(mode);

    return TM_OK;
}

static INT32 _papi_fe_col_CombFilterEnabled (unsigned long u4Arg) 
{
    Bool rArg;

    rArg = (Bool)_bTDCEnable;    
    COPY_TO_USER((void*)u4Arg, &rArg, sizeof(Bool));

    return TM_OK;
}

static INT32 _papi_fe_col_CombFilterActive (unsigned long u4Arg) 
{
    Bool rArg;
    
    rArg = (Bool)_bTDCEnable;    
    COPY_TO_USER((void*)u4Arg, &rArg, sizeof(Bool));

    return TM_OK;
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

int papi_ioctl_fe_col(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg)
{
    int i4Ret = 0;
    //_ApiLock();

    switch (cmd)
    {      
    case PAPI_IO_FE_COL_COLORSYSTEMSUPPORTEDEXT:
        i4Ret = _papi_fe_col_ColorSystemSupportedEx(arg);
        break;
    case PAPI_IO_FE_COL_FORCECOLORSYSTEMEX:
        i4Ret = _papi_fe_col_ForceColorSystemEx(arg);
        break;
    case PAPI_IO_FE_COL_GETFORCECOLORSYSTEM:
        i4Ret = _papi_fe_col_GetForcedColorSystem(arg);
        break;
    case PAPI_IO_FE_COL_GETFORCECARRIERFREQUENCY:
        i4Ret = _papi_fe_col_GetForcedCarrierFrequency(arg);
        break;
    case PAPI_IO_FE_COL_SUGGESTCOLORSYSTEMEX:
        i4Ret = _papi_fe_col_SuggestColorSystemEx(arg);
        break;
    case PAPI_IO_FE_COL_GETSUGGESTEDCOLORSYSTEM:
        i4Ret = _papi_fe_col_GetSuggestedColorSystem(arg);
        break;
    case PAPI_IO_FE_COL_GETSUGGESTEDCARRIERFREQUENCY:  
        i4Ret = _papi_fe_col_GetSuggestedCarrierFrequency(arg);
        break;
    case PAPI_IO_FE_COL_GETCOLORSYSTEM:
        i4Ret = _papi_fe_col_GetColorSystem2(arg);
        break;
    case PAPI_IO_FE_COL_GETCARRIERFREQUENCY:
        i4Ret = _papi_fe_col_GetCarrierFrequency(arg);
        break;
    case PAPI_IO_FE_COL_COMBFILTERSUPPORTED:
        i4Ret = _papi_fe_col_CombFilterSupported(arg);
        break;
    case PAPI_IO_FE_COL_ENABLECOMBFILTER:
        i4Ret = _papi_fe_col_EnableCombFilter (arg);
        break;
    case PAPI_IO_FE_COL_COMBFILTERENABLED:
        i4Ret = _papi_fe_col_CombFilterEnabled(arg);
        break;
    case PAPI_IO_FE_COL_COMBFILTERACTIVE:
        i4Ret = _papi_fe_col_CombFilterActive(arg);
        break;
        
    default:
        printk("papi_ioctl_fe_col: unknown IO command %u\n", cmd);
        i4Ret = -EFAULT;
    }
    
    //_ApiUnlock();

    return i4Ret;
}
#if 0
INT32 _papi_fe_col_GetSuggestedColorSystem_i (void) 
{
    return Suggestedcolorsystem;
}
#endif

