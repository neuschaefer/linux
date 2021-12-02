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
 *
 *---------------------------------------------------------------------------*/

/** @file nptv_if.h
 *  NPTV video path control, include  input source select ,
 *  timing information, and video quality control
 */

#ifndef _NPTV_IF_H_
#define _NPTV_IF_H_

#include "drv_hdmi.h"
#include "util.h"
#include "drv_vga.h"
#include "drv_tvd.h"
#include "api_notify.h"
#include "vdo_if.h"
#include "nptv_vdec_if.h"
#include "ostg_if.h"

//-----------------------------------------------------------------------------
//function Declare
//-----------------------------------------------------------------------------


#if 0 // 5387


/**
 * Set display region information of the path video window.
 *
 * @param bPath SV_VP_MAIN / SV_VP_PIP
 * @param wResX The region start horizontal position of the path.
 * @param wY The region start vertical position of the path.
 * @param wTotWidth The total width in pixel of the path.
 * @param bHeight The total height in pixel of the path.
 * @return The exit status of this api.
 */
EXTERN UINT8 bApiVideoSetPathDispInfo(UINT8 bPath, UINT16 wRegSX, UINT16 wRegSY,
                               UINT16 wTotWidth, UINT16 wTotHeight);

/**
 * set scaler to pixel based or percentage based.
 *
 * @param u4OnOff = 0 refer to scaler API change to percentage based. (Default)
 * @param u4OnOff = 1 refer to scaler API change to pixel based.
 * @return void
 */
EXTERN void vApiSetScalerPixelBased(UINT32 u4OnOff);

/**
 * Set display position and size of video window.
 *
 * @param bPath SV_VP_MAIN / SV_VP_PIP
 * @param wX horizontal position
 * @param wY vertical position
 * @param wWidth width in pixel
 * @param wHeight height in pixel
 * @ruturn The exit status of this api.
 * @retval SV_SUCCESS
 * @retval SV_FAIL
 */
EXTERN UINT8 bApiVideoSetDispPosSize(UINT8 bPath, UINT16 wX, UINT16 wY,
                              UINT16 wWidth, UINT16 wHeight);


/**
 * @brief bApiVideoSetSrcPosSize

 * Set position and size of the source video window

 * @param  bPath: SV_VP_MAIN/SV_VP_PIP
 *         wXOff: the horizontal position of the source video window
 *         wYOff: the vertical position of the source video window
 *         wWidth: the width of the source video window
 *         wHeight: the height of the source video window
 * @retval SV_SUCCESS/SV_FAIL
 */
EXTERN UINT8 bApiVideoSetSrcOverScan(UINT8 bPath, UINT16 wTop, UINT16 wBottom,
                              UINT16 wLeft, UINT16 wRight);

/**
 * Set position and size of source video window.
 *
 * @param bPath SV_VP_MAIN / SV_VP_PIP
 * @wX horizontal position
 * @wY vertical position
 * @bWidth width in pixel
 * @bHeight height in pixel
 * @return The exit status of this api.
 * @retval SV_SUCCESS
 * @retval SV_FAIL
 */
EXTERN UINT8 bApiVideoSetSrcPosSize(UINT8 bPath, UINT16 wX, UINT16 wY, UINT16 wWidth,
                             UINT16 wHeight);

/**
 * Enable/Disable the feature of blanking the area outside video windows.
 *
 * @bPath SV_VP_MAIN / SV_VP_PIP
 * @bOnOff SV_ON - After position or size is changed, the old picture will remain still.
 * SV_OFF - After position or size is changed, the old picture will be cleared.
 * @return NONE
 */
EXTERN void vApiVideoSetDispMask(UINT8 bPath, UINT8 bOnOff);

/**
 * Make video frozen.
 *
 * @param bPath SV_VP_MAIN / SV_VP_PIP
 * @param bOnOff SV_ON / SV_OFF
 * @return NONE
 */
EXTERN void vApiVideoFreeze(UINT8 bPath, UINT8 bOnOff);
#endif

//EXTERN void vApiVideoForceMute(UINT8 bPath, UINT8 bOnOff);


// removed. Call vDrvVideoSetBg() instead
//EXTERN void vApiVideoSetBg(UINT8 bPath, UINT32 u4Bg);

/**
 * @brief vApiVideoSetNonLinearUpScaler
 * Set the flag "_fgNonLinearUP" for Panoramic Mode(Non-linear upscaling mode)
 * @param  bOnOff: SV_ON/SV_OFF
 * @retval void
 */
//EXTERN void vApiVideoSetNonLinearUpScaler(UINT8 bOnOff);


// removed!
// call vApiLVDSSetDriving() instead.
//EXTERN void vDrvLVDSSetDriving(UINT8 bDriving);

// removed!
// call vApiLCDSetFrameRate() instead.
//EXTERN void vDrvLCDSetFrame(UINT16 wFrameRate);

// removed!
// call bApiLCDGetFrameRate() instead.
//EXTERN UINT8 vDrvGetLCDFreq(void);

// Removed. Call wApiVideoGetInputWidth() instead
//EXTERN UINT16 wDrvVideoInputWidth(UINT8 bPath);


// Removed. Call wApiVideoGetInputHeight() instead
//EXTERN UINT16 wDrvVideoInputHeight(UINT8 bPath);

// Removed. Call bApiVideoGetRefreshRate() instead.
//EXTERN UINT8 bDrvVideoGetRefreshRate(UINT8 bPath);

// Removed. Call bApiVideoIsSrcInterlace() instead
//EXTERN UINT8 bDrvVideoIsSrcInterlace(UINT8 bPath);

// Remove. Call bApiVgaSigStatus() intstead
//EXTERN UINT8 bVgaSigStatus(void);

 // Removed! Use fgApiVideoVgaAuto() instead.
//EXTERN void vDrvVgaAutoStart(void);

// Remove. Call vApiVgaAutoStop() instead.
//EXTERN void vDrvVgaAutoStop(void);


//EXTERN UINT16 wDrvVideoGetPorch(UINT8 bPath, UINT8 bPorchType);
//EXTERN UINT16 wDrvVideoSetPorch(UINT8 bPath, UINT8 bPorchType, UINT16 wValue);


/**
 * @brief vHDMIHPDAdjust(UINT32 value)
 * Time period of HPD low which is used to reset player.
 * @param value The low period of HPD in millisecond.
 * @retval VOID
 */
//EXTERN void vHDMIHPDAdjust(UINT32 value);

/**
 * @brief vHDMIHPDAdjust(UINT32 value)
 * Time period of HPD low which is used to reset player.
 * @param frame The type of infoFram.
 	HDMI_InfoFrame_AVI/ 	HDMI_InfoFrame_SPD/	HDMI_InfoFrame_AUDIO/ HDMI_InfoFrame_MPEG/
	HDMI_InfoFrame_UNREC/ HDMI_InfoFrame_GAMUT/ HDMI_InfoFrame_ACP

 * @retval Pointer of HDMI_INFOFRAME_DESCRIPTION which conatains FrameSize and data.
 */
//EXTERN HDMI_INFOFRAME_DESCRIPTION *API_HDMI_Get_InfoFrame(UINT8 frame);



#endif /* _NPTV_IF_H_ */
