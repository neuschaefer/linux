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
 * $RCSfile: io_mtpmx.c,v $
 * $Revision: #6 $
 *---------------------------------------------------------------------------*/

/** @file io_mtpmx.c
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mtpmx.h"
#include "mtutil.h"
#include <linux/mtal_ioctl.h>
#include "pmx_drvif.h"
#include "nptv_if.h"
#include "drv_vdoclk.h"
#include "drv_display.h"
#include "drv_video.h"
#include "drv_tcon.h"
#include "drv_lvds.h"
#include "vdo_misc.h"
#include "osd_drvif.h"
#include "panel.h"
#include "x_bim.h"
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
#ifdef CC_MT5396
extern void PANEL_SetVTotal(UINT16 u2VTotal48Hz, UINT16 u2VTotal50Hz, UINT16 u2VTotal60Hz);
#endif

/** init pmx driver and hardware status
 *  @param  VOID
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 */
static MT_RESULT_T _MTPMX_Init(VOID)
{
    MTPMX_PRINT(" - void\n");
	x_os_drv_set_timestamp("_MTPMX_Init start");

    //add driver implementation code here
    PMX_Init();
	x_os_drv_set_timestamp("_MTPMX_Init end");

    return MTR_OK;
}

/** get plane z-order
 *  @param  u4PlaneId               plane id:{MTPMX_MAIN, MTPMX_PIP, MTPMX_OSD1, MTPMX_OSD2}
 *  @param  pu4PlaneOrder           plane order:{MTPMX_ORDER1/2/3/4}
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 */
static MT_RESULT_T _MTPMX_GetPlaneOrder(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UINT32  u4PlaneId;
    UINT32* pu4PlaneOrder;
    UINT32  u4PlaneOrder = 0;
    UCHAR u1PlaneOrder = 0;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u4PlaneId = (UINT32)rArg.ai4Arg[0];
    pu4PlaneOrder = (UINT32*)rArg.ai4Arg[1];

    //add driver implementation code here
    if (PMX_GetPlaneOrder(u4PlaneId, &u1PlaneOrder) != PMX_SET_OK)
    {
        return MTR_NOT_OK;
    }
    u4PlaneOrder = (UINT32)u1PlaneOrder;

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4PlaneOrder,UINT32);
    COPY_TO_USER_ARG(pu4PlaneOrder, u4PlaneOrder,UINT32);

    MTPMX_PRINT(" - u4PlaneId = 0x%x, u4PlaneOrder = %d\n",
        (unsigned int)u4PlaneId,
        (int)u4PlaneOrder);

    return MTR_OK;
}

/** set plane z-order
 *  @param  u4PlaneId               plane id:{MTPMX_MAIN, MTPMX_PIP, MTPMX_OSD1, MTPMX_OSD2}
 *  @param  u4PlaneOrder            plane order:{MTPMX_ORDER1/2/3/4}
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 */
static MT_RESULT_T _MTPMX_SetPlaneOrder(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UINT32 u4PlaneId;
    UINT32 u4PlaneOrder;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u4PlaneId = (UINT32)rArg.ai4Arg[0];
    u4PlaneOrder = (UINT32)rArg.ai4Arg[1];

    MTPMX_PRINT(" - u4PlaneId = 0x%x, u4PlaneOrder = %d\n",
        (unsigned int)u4PlaneId,
        (int)u4PlaneOrder);

    //add driver implementation code here
    if (PMX_SetPlaneOrder(u4PlaneId, u4PlaneOrder) != PMX_SET_OK)
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}


/** set plane z-order array
 *  @param  pu4PlaneOrder               plane order array addr
 *  @param  u4PlaneOrder            plane order:{MTPMX_ORDER1/2/3/4}
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 */
static MT_RESULT_T _MTPMX_SetPlaneOrderArray(unsigned long arg)
{
    MTAL_IOCTL_8ARG_T    rArg;

    USR_SPACE_ACCESS_VALIDATE_8ARG(arg);
    COPY_FROM_USER_8ARG(arg, rArg);

    //add driver implementation code here
    if(PMX_SetPlaneOrderArray(rArg.ai4Arg) != PMX_SET_OK)
    {
        return MTR_NOT_OK;
    }
    
    return MTR_OK;
}

/** Get plane z-order array
 *  @param  pu4PlaneOrder               plane order array addr
 *  @param  u4PlaneOrder            plane order:{MTPMX_ORDER1/2/3/4}
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 */
static MT_RESULT_T _MTPMX_GetPlaneOrderArray(unsigned long arg)
{
    UINT32* pu4Temp         = (UINT32*)arg;
    UINT32 pu4PlaneOrder[PMX_MAX_INPORT_NS+1];

    USR_SPACE_ACCESS_VALIDATE_8ARG(pu4Temp);

    if( PMX_SET_OK != PMX_GetPlaneOrderArray(pu4PlaneOrder) )
    {
        return MTR_NOT_OK;
    }
    
    COPY_TO_USER_8ARG(pu4Temp,pu4PlaneOrder);
    
    return MTR_OK;
}

/** Set plane mix alpha and osd&video layout
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 */
static MT_RESULT_T _MTPMX_SetMixAlpha(unsigned long arg)
{
    MTAL_IOCTL_4ARG_T   rArg;

    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg, rArg);

    //add driver implementation code here
    if(PMX_SetMixAlpha((UCHAR)rArg.ai4Arg[0], (UINT32*)&rArg.ai4Arg[1]) != PMX_SET_OK)
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}



/** set pmx background color, HW will update color on next vsync after AP set
 *  @param  u4BgColor               background color(0xrrggbb)
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 */
static MT_RESULT_T _MTPMX_SetBg(unsigned long arg)
{
    UINT32 u4BgColor;

    u4BgColor = (UINT32)arg;

    MTPMX_PRINT(" - u4BgColor = 0x%x\n", (unsigned int)u4BgColor);

    //add driver implementation code here
    if (PMX_SetBg(u4BgColor, FALSE) != PMX_SET_OK)
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}

/** setup alpha value for video plane
 *  @param  u4PlaneId               plane id:{MTPMX_MAIN, MTPMX_PIP}
 *  @param  u1Alpha                 alpha value(0~255)
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 */
static MT_RESULT_T _MTPMX_SetAlpha(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UINT32 u4PlaneId;
    UINT8 u1Alpha;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u4PlaneId = (UINT32)rArg.ai4Arg[0];
    u1Alpha = (UINT8)rArg.ai4Arg[1];

    MTPMX_PRINT(" - u4PlaneId = 0x%x, u1Alpha = 0x%x\n",
        (unsigned int)u4PlaneId,
        (unsigned int)u1Alpha);

    //add driver implementation code here
    if (PMX_SetAlpha(u4PlaneId, u1Alpha) != PMX_SET_OK)
    {
        return MTR_NOT_OK;
    }

    return MTR_OK;
}

static MT_RESULT_T _MTPMX_SetSpreadSpectrum(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UINT32 u4Period;
    UINT32 u4Range;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u4Period = (UINT32)rArg.ai4Arg[0];
    u4Range = (UINT32)rArg.ai4Arg[1];

    MTPMX_PRINT(" - u4Period = 0x%x, u4Range = 0x%x\n",
        (unsigned int)u4Period,
        (unsigned int)u4Range);

    if (u4Range > 1000)
    {
        return MTR_NOT_OK;
    }

    DDDS_SetSpreadSpectrum(u4Period, u4Range);

    return MTR_OK;
}

/** mute all plane
 *  @param  fgMute                  mute=TRUE/unmute=FALSE
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 */
static MT_RESULT_T _MTPMX_SetAllMute(unsigned long arg)
{
    BOOL fgMute;

    fgMute = (BOOL)arg;

    MTPMX_PRINT(" - fgMute = %d\n", (unsigned int)fgMute);

    //add driver implementation code here
    //PMX_SetFastBlank(fgMute);
    vDrvSetAllMute(fgMute);

    return MTR_OK;
}

/** output frame rate control (50/60/... Hz)
 *  @param  u4FrameRate             output frame rate
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_NO_SUPPORT          Frame no support
 */
static MT_RESULT_T _MTPMX_PANEL_SetFrameRate(unsigned long arg)
{
    UINT32 u4FrameRate;

    u4FrameRate = (UINT32)arg;

    MTPMX_PRINT(" - u4FrameRate = %d\n", (int)u4FrameRate);

    vDrvLCDSetFrame((UINT16)u4FrameRate);
    return MTR_OK;
}

/** get current output frame rate
 *  @param  pu4FrameRate            current output frame rate
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 */
static MT_RESULT_T _MTPMX_PANEL_GetFrameRate(unsigned long arg)
{
    UINT32* pu4FrameRate;
    UINT32  u4FrameRate;

    pu4FrameRate = (UINT32*)arg;

    u4FrameRate = (UINT32)vDrvGetLCDFreq();   

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4FrameRate,INT32);
    COPY_TO_USER_ARG(pu4FrameRate, u4FrameRate,INT32);

    MTPMX_PRINT(" - u4FrameRate = %d\n", (int)u4FrameRate);
    return MTR_OK;
}

static MT_RESULT_T _MTPMX_PANEL_SetFilmMode(unsigned long arg)
{
    #ifdef CC_MT5392B  
    BOOL fgFilmOnOff;

    fgFilmOnOff = (BOOL)arg;

    MTPMX_PRINT(" - fgFilmOnOff = %d\n", (int)fgFilmOnOff);

    OSTG_SetFilmMode(fgFilmOnOff);
    return MTR_OK;
    #else
    return MTR_NOT_SUPPORTED;
    #endif
}

static MT_RESULT_T _MTPMX_PANEL_GetFilmMode(unsigned long arg)
{
    #ifdef CC_MT5392B  
    BOOL* pfgFilmOnOff;
    BOOL  fgFilmOnOff;

    pfgFilmOnOff = (BOOL*)arg;
    fgFilmOnOff = (BOOL)OSTG_GetFilmMode();   

    USR_SPACE_ACCESS_VALIDATE_ARG(pfgFilmOnOff,INT32);
    COPY_TO_USER_ARG(pfgFilmOnOff, fgFilmOnOff,INT32);

    MTPMX_PRINT(" - fgFilmOnOff = %d\n", (int)fgFilmOnOff);
    return MTR_OK;
    #else
    return MTR_NOT_SUPPORTED;
    #endif
}


static BOOL _fgPanelSelected = FALSE;
BOOL _MTPMX_IsPanelSelected(void)
{
    return _fgPanelSelected;
}

/** enable TV encoder
 *  @param  fgEnable                enable=TRUE/disable=FALSE
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 */
static MT_RESULT_T _MTPMX_PANEL_Select(unsigned long arg)
{
    UINT32 u4PanelIndex;

    u4PanelIndex = (UINT32)arg;
    MTPMX_PRINT(" - u4PanelIndex = %d\n", (unsigned int)u4PanelIndex);
    SelectPanel(u4PanelIndex);

    vDrvOutputStageInit();
    vDrvVOFreqSet(PANEL_GetPixelClk60Hz());
    vDrvSetLCDTiming();
    OSTG_OnOutputVSync();
    IGNORE_RET(OSD_UpdateTiming());

    _fgPanelSelected = TRUE;

    return MTR_OK;
}

/** enable LVDS power
 *  @param  fgEnable                enable=TRUE/disable=FALSE
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 */
static MT_RESULT_T _MTPMX_PANEL_EnableLVDSPower(unsigned long arg)
{
    BOOL fgEnable;

    fgEnable = (BOOL)arg;

    MTPMX_PRINT(" - fgEnable = %d\n", fgEnable);

    vDrvLVDSPowerOnOff(fgEnable);
    return MTR_OK;
}

/** enable backlight power
 *  @param  fgEnable                enable=TRUE/disable=FALSE
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 */
static MT_RESULT_T _MTPMX_PANEL_EnableBacklightPower(unsigned long arg)
{
    BOOL fgEnable;

    fgEnable = (BOOL)arg;

    MTPMX_PRINT(" - fgEnable = %d\n", fgEnable);

    vApiBackltONOFF(fgEnable);
    return MTR_OK;
}

/** enable LVDS signal
 *  @param  fgEnable                enable=TRUE/disable=FALSE
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 */
static MT_RESULT_T _MTPMX_PANEL_EnableLVDSSignal(unsigned long arg)
{
    BOOL fgEnable;

    fgEnable = (BOOL)arg;

    MTPMX_PRINT(" - fgEnable = %d\n", fgEnable);
    if(fgEnable == TRUE)
    {
    	vDrvLVDSConfig();
    }
    vDrvLVDSSignalOnOff( fgEnable);
    return MTR_OK;
}

/** set LVDS driving current
 *  @param  u4Current               driving current (2~15 mA)
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 */
static MT_RESULT_T _MTPMX_PANEL_SetDrivingCurrent(unsigned long arg)
{
    UINT32 u4Current;

    u4Current = (UINT32)arg;

    MTPMX_PRINT(" - u4Current = %d\n", (int)u4Current);

    vDrvLVDSSetDriving(u4Current);
    return MTR_OK;
}

/** Set panel back light dimming
 *  @param  u4Frequency           
 *  @param  u4Dimming               
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 */
static MT_RESULT_T _MTPMX_PANEL_SetBackltDimming(unsigned long arg)
{
    UINT32 u4Frequency, u4Dimming;
    MTAL_IOCTL_2ARG_T    rArg;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u4Frequency = rArg.ai4Arg[0];
    u4Dimming = rArg.ai4Arg[1];
    
    MTPMX_PRINT(" - u4Frequency = %d , u4Dimming= %d\n", (int)u4Frequency, (int)u4Dimming);

    BACKLT_SetDimming( u4Frequency, u4Dimming);
    return MTR_OK;
}


/** Panel on/off full sequence
 *  @param  fgEnable               TRUE: Pnael on, FALSE: Panel off
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 */
static MT_RESULT_T _MTPMX_PANEL_PowerSequence(unsigned long arg)
{
    BOOL fgEnable;
    fgEnable = (BOOL)arg;
	x_os_drv_set_timestamp("_MTPMX_PANEL_PowerSequence start");

    MTPMX_PRINT(" - fgEnable = %d\n", fgEnable);
    vApiPanelPowerSequence(fgEnable);
	
	x_os_drv_set_timestamp("_MTPMX_PANEL_PowerSequence end");
    return MTR_OK;
}


/** enable TV encoder
 *  @param  fgEnable                enable=TRUE/disable=FALSE
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 */
static MT_RESULT_T _MTPMX_TVE_Enable(unsigned long arg)
{
    BOOL fgEnable;

    fgEnable = (BOOL)arg;

    MTPMX_PRINT(" - fgEnable = %d\n", fgEnable);

    //add driver implementation code here
    return MTR_OK;
}

/** Get panel resolution
 *  @param  (MTAL_IOCTL_2ARG_T*)arg->pu4PanelWidth 
    width of panel
*  @param  (MTAL_IOCTL_2ARG_T*)arg->pu4PanelHeight
    height of panel
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 */
static MT_RESULT_T _MTPMX_PANEL_GetResolution(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    UINT32 u4PanelWidth;
    UINT32 u4PanelHeight;
    UINT32 *pu4PanelWidth;
    UINT32 *pu4PanelHeight;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    pu4PanelWidth = (UINT32*)rArg.ai4Arg[0];
    pu4PanelHeight = (UINT32*)rArg.ai4Arg[1];

    u4PanelWidth = PANEL_GetPanelWidth();
    u4PanelHeight = PANEL_GetPanelHeight();

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4PanelWidth,UINT32);
    COPY_TO_USER_ARG(pu4PanelWidth, u4PanelWidth,UINT32);

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4PanelHeight,UINT32);
    COPY_TO_USER_ARG(pu4PanelHeight, u4PanelHeight,UINT32);

    MTPMX_PRINT(" - u4PanelWidth = %d, u4PanelHeight = %d\n",
        (unsigned int)u4PanelWidth, (unsigned int)u4PanelHeight);

    return MTR_OK;
}

/** Sync lock / Sync free-run control
 *  @param  fgEnable            TRUE: lock, FALSE: free-run
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
*/
static MT_RESULT_T _MTPMX_PANEL_EnableSyncLock(unsigned long arg)
{
    BOOL fgEnable;
    fgEnable = (BOOL)arg;

    MTPMX_PRINT(" - fgEnable = %d\n", fgEnable);
    if(fgEnable != 0)
    {
        vDrvSetSyncLockMode(eSYNCLOCK_ON);
    }
    else
    {
        vDrvSetSyncLockMode(eSYNCLOCK_OFF);
    }
    return MTR_OK;
}

/** get current lock status
 *  @param  pfgLock            current lock status
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 */
static MT_RESULT_T _MTPMX_PANEL_GetLockStatus(unsigned long arg)
{
    BOOL *pfgLock = NULL;
    BOOL fgLock;
    INT32 rArg;

    USR_SPACE_ACCESS_VALIDATE_ARG(arg,INT32);
    COPY_FROM_USER_ARG(arg,rArg,INT32);

    pfgLock = (BOOL*)rArg;

    if(bDrvCheckMTKGoodStatus())
        fgLock = TRUE;
    else
        fgLock = FALSE;

    USR_SPACE_ACCESS_VALIDATE_ARG(pfgLock, BOOL);
    COPY_TO_USER_ARG(pfgLock, fgLock, BOOL);

    MTPMX_PRINT(" -fgLock = %d\n", fgLock);

    return MTR_OK;
}

static MT_RESULT_T _MTPMX_PANEL_3DMode(unsigned long arg)
{
    UINT32 u4Mode;

    u4Mode = (UINT32)arg;

    MTPMX_PRINT(" - u4Mode = %d\n", (int)u4Mode);

    #ifdef CC_SUPPORT_3DTV
    v3DLRGenSetMode(u4Mode);
    return MTR_OK;
    #else
    return MTR_NOT_SUPPORTED;    
    #endif
    
}

/**
 * Get LR info of output (LVDS stage) 
 *
 * @param pu4Info LR info
 */
static MT_RESULT_T _MTPMX_GetLRStatus(unsigned long arg)
{
    #ifdef CC_SUPPORT_3DTV
    UINT32* pu4LRInfo;
    UINT32 u4LRInfo;

    pu4LRInfo = (UINT32*)arg;
    u4LRInfo = (UINT32)u43DGetOutputLR();

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4LRInfo,INT32);
    COPY_TO_USER_ARG(pu4LRInfo, u4LRInfo,INT32);

    MTPMX_PRINT(" - Output LR = %d\n", (int)u4LRInfo);
    return MTR_OK;
    
    #else
    return MTR_NOT_SUPPORTED;    
    #endif
}



/**
 * Set LR polarity of output (LVDS stage) 
 *
 * @param u4LRPolarity LR polarity
 */
static MT_RESULT_T _MTPMX_SetLRPolarity(unsigned long arg)
{
    #ifdef CC_SUPPORT_3DTV
    UINT32 u4LRPolarity;

    u4LRPolarity = (UINT32)arg;

    MTPMX_PRINT(" - u4Polarity = %d\n", (int)u4LRPolarity);

    v3DSetOutputLRPolarity(u4LRPolarity);
    return MTR_OK;    
    #else
    return MTR_NOT_SUPPORTED;    
    #endif
}

/** output frame rate control (50/60/... Hz)
 *  @param  u4FrameRate             output frame rate
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_NO_SUPPORT          Frame no support
 */
static MT_RESULT_T _MTPMX_PANEL_SetFrameTrack(unsigned long arg)
{
    UINT32 u4OnOff;

    u4OnOff = (UINT32)arg;

    MTPMX_PRINT(" - u4FrameTrackOnOff = %d\n", (int)u4OnOff);

#ifndef CC_MTK_LOADER
    #ifdef SUPPORT_SW_FRAME_CHECK
        vSetSWFrameTrackOnOff(u4OnOff);
    #endif
#endif
    return MTR_OK;
}


/**  Set panel color depth
 * @param u1BitRes: 0: 8 bits/1: 10 bit
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_NO_SUPPORT          Frame no support
 */
static MT_RESULT_T _MTPMX_PANEL_SetColorDepth(unsigned long arg)
{
#ifdef CC_MT5396
    UINT32 u1BitRes;

    u1BitRes = (UINT32)arg;
    PANEL_SetColorDepth(u1BitRes);
    MTPMX_PRINT(" - u1BitRes = %d\n", (int)u1BitRes);
#endif
    return MTR_OK;
}

/** Set lvds format
 * @param u1Format: 0:ns/1: jeida
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_NO_SUPPORT          Frame no support
 */
static MT_RESULT_T _MTPMX_PANEL_SetLvdsSelect(unsigned long arg)
{
#ifdef CC_MT5396
    UINT32 u1Format;

    u1Format = (UINT32)arg;
    PANEL_SetLvdsSelect(u1Format);
    MTPMX_PRINT(" - u1Format = %d\n", (int)u1Format);
#endif
    return MTR_OK;
}

/** Set lvds mode
 * @param u1Mode: 0:single/1: dual
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_NO_SUPPORT          Frame no support
 */
static MT_RESULT_T _MTPMX_PANEL_SetLvdsPixel(unsigned long arg)
{
#ifdef CC_MT5396
    UINT32 u1Mode;

    u1Mode = (UINT32)arg;
    PANEL_SetLvdsPixel(u1Mode);
    MTPMX_PRINT(" - u1Mode = %d\n", (int)u1Mode);
#endif
    return MTR_OK;
}

/** Set panel resolution
 *  @param  pu4Width                 panel width (pixel)
 *  @param  pu4Height                panel height (line)
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_NO_SUPPORT          Frame no support
 */
static MT_RESULT_T _MTPMX_PANEL_SetResolution(unsigned long arg)
{
#ifdef CC_MT5396
    MTAL_IOCTL_2ARG_T    rArg;
    UINT32 u4Width;
    UINT32 u4Height;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u4Width = (UINT32)rArg.ai4Arg[0];
    u4Height = (UINT32)rArg.ai4Arg[1];
    PANEL_SetResolution(u4Width, u4Height);
    MTPMX_PRINT(" - u4Width = %d, u4Height = %d\n",  (int)u4Width, (int)u4Height);
#endif
    return MTR_OK;
}

/** Set horizontal period
 * @param u2Htotal Htotal
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_NO_SUPPORT          Frame no support
 */
static MT_RESULT_T _MTPMX_PANEL_SetHoriPeriod(unsigned long arg)
{
#ifdef CC_MT5396
    UINT32 u2Htotal;

    u2Htotal = (UINT32)arg;
    PANEL_SetHTotalTyp(u2Htotal);
    MTPMX_PRINT(" - u2Htotal = %d\n", (int)u2Htotal);
#endif
    return MTR_OK;
}


/** Set panel vertical period
 *  @param  u2Vtotal48Hz                vtotal 48hz
 *  @param  u2Vtotal50Hz                vtotal 50hz
 *  @param  u2Vtotal50Hz                vtotal 60hz
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_NO_SUPPORT          Frame no support
 */
static MT_RESULT_T _MTPMX_PANEL_SetVertPeriod(unsigned long arg)
{
#ifdef CC_MT5396
    MTAL_IOCTL_3ARG_T    rArg;
    UINT32 u2Vtotal48Hz;
    UINT32 u2Vtotal50Hz;
    UINT32 u2Vtotal60Hz;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u2Vtotal48Hz = (UINT32)rArg.ai4Arg[0];
    u2Vtotal50Hz = (UINT32)rArg.ai4Arg[1];
    u2Vtotal60Hz = (UINT32)rArg.ai4Arg[2];

    PANEL_SetVTotal(u2Vtotal48Hz, u2Vtotal50Hz, u2Vtotal60Hz);
    MTPMX_PRINT(" - u2Vtotal48Hz = %d, u2Vtotal50Hz = %d, u2Vtotal60Hz = %d\n",  (int)u2Vtotal48Hz, (int)u2Vtotal50Hz, (int)u2Vtotal60Hz);
#endif
    return MTR_OK;
}


/** Set panel vertical sync width
 *  @param  u2Vsync48Hz                vtotal 48hz
 *  @param  u2Vsync50Hz                vtotal 50hz
 *  @param  u2Vsync60Hz                vtotal 60hz
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_NO_SUPPORT          Frame no support
 */
static MT_RESULT_T _MTPMX_PANEL_SetVertSyncWidth(unsigned long arg)
{
#ifdef CC_MT5396
    MTAL_IOCTL_3ARG_T    rArg;
    UINT32 u2Vsync48Hz;
    UINT32 u2Vsync50Hz;
    UINT32 u2Vsync60Hz;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u2Vsync48Hz = (UINT32)rArg.ai4Arg[0];
    u2Vsync50Hz = (UINT32)rArg.ai4Arg[1];
    u2Vsync60Hz = (UINT32)rArg.ai4Arg[2];

    PANEL_SetVertSyncWidth(u2Vsync48Hz, u2Vsync50Hz, u2Vsync60Hz);
    MTPMX_PRINT(" - u2Vsync48Hz = %d, u2Vsync50Hz = %d, u2Vsync60Hz = %d\n",  (int)u2Vsync48Hz, (int)u2Vsync50Hz, (int)u2Vsync60Hz);
#endif
    return MTR_OK;
}


/** Set panel vertical back porch
 *  @param  u2VbackPorch48Hz                vtotal 48hz
 *  @param  u2VbackPorch50Hz                vtotal 50hz
 *  @param  u2VbackPorch60Hz                vtotal 60hz
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *  @retval MTR_NO_SUPPORT          Frame no support
 */
static MT_RESULT_T _MTPMX_PANEL_SetVertBackPorch(unsigned long arg)
{
#ifdef CC_MT5396
    MTAL_IOCTL_3ARG_T    rArg;
    UINT32 u2VbackPorch48Hz;
    UINT32 u2VbackPorch50Hz;
    UINT32 u2VbackPorch60Hz;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u2VbackPorch48Hz = (UINT32)rArg.ai4Arg[0];
    u2VbackPorch50Hz = (UINT32)rArg.ai4Arg[1];
    u2VbackPorch60Hz = (UINT32)rArg.ai4Arg[2];

    PANEL_SetVertBackPorch(u2VbackPorch48Hz, u2VbackPorch50Hz, u2VbackPorch60Hz);
    MTPMX_PRINT(" - u2VbackPorch48Hz = %d, u2VbackPorch50Hz = %d, u2VbackPorch60Hz = %d\n",  (int)u2VbackPorch48Hz, (int)u2VbackPorch50Hz, (int)u2VbackPorch60Hz);
#endif
    return MTR_OK;
}


/** get current panel index
 *  @param  pu4FrameRate            current output frame rate
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 */
static MT_RESULT_T _MTPMX_PANEL_GetCurrentIndex(unsigned long arg)
{
#ifdef CC_MT5396
    UINT32* pu4Index;
    UINT32  u4Index;

    pu4Index = (UINT32*)arg;
    u4Index = GetCurrentPanelIndex();

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4Index,INT32);
    COPY_TO_USER_ARG(pu4Index, u4Index,INT32);

    MTPMX_PRINT(" - u4Index = %d\n", (int)u4Index);
#endif
    return MTR_OK;
}

#if 1 //add the function for backlight
static MT_RESULT_T _MTPMX_PANEL_GetBlacklightPixelPercentage(unsigned long arg)
{
    UINT32* pui4_value;
    UINT32 ui4_value;
    pui4_value = (UINT32*)arg;
    ui4_value=u4DrvGetBlackPixelPercentage();
    USR_SPACE_ACCESS_VALIDATE_ARG(pui4_value,INT32);
    COPY_TO_USER_ARG(pui4_value, ui4_value,INT32);
    MTPMX_PRINT("GetBlacklightPixelPercentage:*pu4_value=%d,ui4_value=%d",*pui4_value,ui4_value);
    return MTR_OK;
}

static MT_RESULT_T _MTPMX_PANEL_GetBlacklightAPLPercentage(unsigned long arg)
{
    UINT32* pui4_value;
    UINT32 ui4_value;
    pui4_value = (UINT32*)arg;
    ui4_value=u4DrvGetAPLPercentage();
    USR_SPACE_ACCESS_VALIDATE_ARG(pui4_value,INT32);
    COPY_TO_USER_ARG(pui4_value, ui4_value,INT32);
    MTPMX_PRINT("GetBlacklightPixelPercentage:*pu4_value=%d,ui4_value=%d",*pui4_value,ui4_value);
    return MTR_OK;
}

static MT_RESULT_T _MTPMX_PANEL_GetBlacklightWhitePixelPercentage(unsigned long arg)
{
    UINT32* pui4_value;
    UINT32 ui4_value;
    pui4_value = (UINT32*)arg;
    ui4_value=u4DrvGetWhitePixelPercentage();
    USR_SPACE_ACCESS_VALIDATE_ARG(pui4_value,INT32);
    COPY_TO_USER_ARG(pui4_value, ui4_value,INT32);
    MTPMX_PRINT("GetBlacklightPixelPercentage:*pu4_value=%d,ui4_value=%d",*pui4_value,ui4_value);
    return MTR_OK;
}



static MT_RESULT_T  _MTPMX_PANEL_SetBlacklightBright (unsigned long arg)
{
    UINT32 ui4_bvalue=0;
    ui4_bvalue = (UINT32)arg;
    //vApiSetPanelBright(ui4_bvalue);
    vApiSetPanelBrightTemp(ui4_bvalue);
    MTPMX_PRINT("_MTPMX_PANEL_SetBlacklightBright %d.\r\n",ui4_bvalue);
    return MTR_OK;
}

static MT_RESULT_T _MTPMX_PANEL_CalPwmFianlOutput(unsigned long arg)
{
    UINT32* pui4_value;
    UINT32 ui4_value;
    pui4_value = (UINT32*)arg;
    ui4_value= CalPWMFinalOutput();
    USR_SPACE_ACCESS_VALIDATE_ARG(pui4_value,INT32);
    COPY_TO_USER_ARG(pui4_value, ui4_value,INT32);
    MTPMX_PRINT("GetBlacklightPixelPercentage:*pu4_value=%d,ui4_value=%d",*pui4_value,ui4_value);
    return MTR_OK;
}

static MT_RESULT_T _MTPMX_PANEL_GetDynamicalBackelightValue(unsigned long arg)
{
    MTAL_IOCTL_3ARG_T rArg;
    UINT32 u4typevalue;
    UINT32 u4picmode;
    UINT32 u4bkvalue;
    UINT32 *pu4bkvalue;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u4picmode = (UINT32)rArg.ai4Arg[0];
    u4typevalue = (UINT32)rArg.ai4Arg[1];
    pu4bkvalue = (UINT32*)rArg.ai4Arg[2];

    printf("u4picmode=%d,u4typevalue=%d.\r\n",u4picmode,u4typevalue);

    u4bkvalue = vDrvGetBackLightGlobalValue(u4picmode,u4typevalue);

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4bkvalue,UINT32);
    COPY_TO_USER_ARG( pu4bkvalue, u4bkvalue,UINT32);
    MTPMX_PRINT("====_MTPMX_PANEL_GetDynamicalBackelightValue, *pu4bkvalue=%d.\r\n",*pu4bkvalue);
    return MTR_OK;
}
#endif
int mtal_ioctl_mtpmx(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
{
    int i4Ret = MTR_OK;

    switch (cmd)
    {
        case MTAL_IO_PMX_INIT:
            return _MTPMX_Init();
        case MTAL_IO_PMX_GETPLANEORDER:
            return _MTPMX_GetPlaneOrder(arg);
        case MTAL_IO_PMX_SETPLANEORDER:
            return _MTPMX_SetPlaneOrder(arg);
        case MTAL_IO_PMX_SETPLANEORDERARRAY:
            return _MTPMX_SetPlaneOrderArray(arg);     
		case MTAL_IO_PMX_GETPLANEORDERARRAY:
            return _MTPMX_GetPlaneOrderArray(arg);  
		case MTAL_IO_PMX_MIX_ALPHA:	
			return _MTPMX_SetMixAlpha(arg);
        case MTAL_IO_PMX_SETBG:
            return _MTPMX_SetBg(arg);
        case MTAL_IO_PMX_SETALPHA:
            return _MTPMX_SetAlpha(arg);
        case MTAL_IO_PMX_SETALLMUTE:
            return _MTPMX_SetAllMute(arg);
        case MTAL_IO_PMX_PANEL_SELECT:
        	return _MTPMX_PANEL_Select(arg);
        case MTAL_IO_PMX_PANEL_SETFRAMERATE:
            return _MTPMX_PANEL_SetFrameRate(arg);
        case MTAL_IO_PMX_GETFRAMERATE:
            return _MTPMX_PANEL_GetFrameRate(arg);
        case MTAL_IO_PMX_PANEL_SETFILMMODE:
            return _MTPMX_PANEL_SetFilmMode(arg);
        case MTAL_IO_PMX_PANEL_GETFILMMODE:
            return _MTPMX_PANEL_GetFilmMode(arg);
        case MTAL_IO_PMX_ENABLELVDSPOWER:
            return _MTPMX_PANEL_EnableLVDSPower(arg);
        case MTAL_IO_PMX_ENABLEBACKLIGHTPOWER:
            return _MTPMX_PANEL_EnableBacklightPower(arg);
        case MTAL_IO_PMX_ENABLELVDSSIGNAL:
            return _MTPMX_PANEL_EnableLVDSSignal(arg);
        case MTAL_IO_PMX_SETDRIVINGCURRENT:
            return _MTPMX_PANEL_SetDrivingCurrent(arg);
        case MTAL_IO_PMX_TVE_ENABLE:
            return _MTPMX_TVE_Enable(arg);
        case MTAL_IO_PMX_SETBACKLTDIMMING:
            return _MTPMX_PANEL_SetBackltDimming(arg);
        case MTAL_IO_PMX_POWERSEQUENCE:            
            return _MTPMX_PANEL_PowerSequence(arg);        
        case MTAL_IO_PMX_SETSPREADSPECTRUM:
            return _MTPMX_SetSpreadSpectrum(arg);	
        case MTAL_IO_PMX_PANEL_GETRESOLUTION:
            return _MTPMX_PANEL_GetResolution(arg);
        case MTAL_IO_PMX_PANEL_SYNCLOCK:
            return _MTPMX_PANEL_EnableSyncLock(arg);
        case MTAL_IO_PMX_PANEL_GETLOCKSTATUS:
            return _MTPMX_PANEL_GetLockStatus(arg);
        case MTAL_IO_PMX_3D_MODE:
            return _MTPMX_PANEL_3DMode(arg);
        case MTAL_IO_PMX_3D_GET_LR_INFO:
            return _MTPMX_GetLRStatus(arg);
        case MTAL_IO_PMX_3D_SET_LR_POLARITY:
            return _MTPMX_SetLRPolarity(arg);
        case MTAL_IO_PMX_PANEL_SETFRAMETRACK:
            return _MTPMX_PANEL_SetFrameTrack(arg);
        case MTAL_IO_PMX_PANEL_SETCOLORDEPTH:
            return _MTPMX_PANEL_SetColorDepth(arg);
        case MTAL_IO_PMX_PANEL_SETLVDSSELECT:
            return _MTPMX_PANEL_SetLvdsSelect(arg);
        case MTAL_IO_PMX_PANEL_SETLVDSPIXEL:
            return _MTPMX_PANEL_SetLvdsPixel(arg);
        case MTAL_IO_PMX_PANEL_SETRESOLUTION:
            return _MTPMX_PANEL_SetResolution(arg);
        case MTAL_IO_PMX_PANEL_SETHORIPERIOD:
            return _MTPMX_PANEL_SetHoriPeriod(arg);
        case MTAL_IO_PMX_PANEL_SETVERTPERIOD:
            return _MTPMX_PANEL_SetVertPeriod(arg);
        case MTAL_IO_PMX_PANEL_SETVERTSYNCWIDTH:
            return _MTPMX_PANEL_SetVertSyncWidth(arg);
        case MTAL_IO_PMX_PANEL_SETVERTBACKPROCH:
            return _MTPMX_PANEL_SetVertBackPorch(arg);
        case MTAL_IO_PMX_PANEL_GETPANELINDEX:
            return _MTPMX_PANEL_GetCurrentIndex(arg);
        #if 1 //add some io_ctl function
        case MTAL_IO_PMX_GETBACKLIGHTPIXEL:
            {
              return _MTPMX_PANEL_GetBlacklightPixelPercentage(arg);
            }
        case  MTAL_IO_PMX_SETBACKLIGHTBRIGHT:
            {
              return _MTPMX_PANEL_SetBlacklightBright(arg);
            }
        case MTAL_IO_PMX_GETPWMOUTPUT:
            {
               return  _MTPMX_PANEL_CalPwmFianlOutput(arg);
            }
        case MTAL_IO_PMX_GETDYNAMICALBKVALUE:
           {
              return _MTPMX_PANEL_GetDynamicalBackelightValue(arg);
           }
        case MTAL_IO_PMX_GETAPLPIXEL:
           {
             return _MTPMX_PANEL_GetBlacklightAPLPercentage(arg);
           }
        case MTAL_IO_PMX_GETBACKLIGHTWHITEPIXEL:
           {
            return _MTPMX_PANEL_GetBlacklightWhitePixelPercentage(arg);
           }
        #endif
        default:
            return MTR_NOT_SUPPORTED;
    }
    return i4Ret;
}
