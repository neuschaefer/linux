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
 * $RCSfile: vdo_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file vdo_if.h
 *  NPTV Video interface, including input source select ,and timing information
 */

#ifndef _VDO_IF_H_
#define _VDO_IF_H_

#include "x_typedef.h"
#include "sv_const.h"


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------



///-----------------------------------------------------------------------------
/// Public functions
///-----------------------------------------------------------------------------

/**
 * Trigger the action to select video input source.
 *
 * @param bPath SV_VP_MAIN / SV_VP_PIP
 * @param bSrc Select the video input source. SV_VS_TUNER1 / SV_VS_CVBS1
 * / SV_VS_CVBS2 / SV_VS_CVBS3 / SV_VS_S1 / SV_VS_S2 / SV_VS_S3
 * / SV_VS_YPbPr1 / SV_VS_YPbPr2 / SV_VS_SCART1 / SV_VS_SCART2
 * / SV_VS_VGA / SV_VS_DVI / SV_VS_HDMI
 * @return The exit status of this api.
 * @retval SV_FAIL
 * @retval SV_NO_CHANGE
 * @retval SV_SUCCESS
 */
EXTERN UINT8 bApiVideoSetVideoSrc(UINT8 bPath, UINT8 bSrc);

/**
 * Set the video sources of main and sub.
 *
 * @param bMainSrc Main video input source. SV_VS_TUNER1 / SV_VS_CVBS1
 * / SV_VS_CVBS2 / SV_VS_CVBS3 / SV_VS_S1 / SV_VS_S2 / SV_VS_S3
 * / SV_VS_YPbPr1 / SV_VS_YPbPr2 / SV_VS_SCART1 / SV_VS_SCART2
 * / SV_VS_VGA / SV_VS_DVI / SV_VS_HDMI
 * @param bSubSrc Sub video input source. SV_VS_TUNER1 / SV_VS_CVBS1
 * / SV_VS_CVBS2 / SV_VS_CVBS3 / SV_VS_S1 / SV_VS_S2 / SV_VS_S3
 * / SV_VS_YPbPr1 / SV_VS_YPbPr2 / SV_VS_SCART1 / SV_VS_SCART2
 * / SV_VS_VGA / SV_VS_DVI / SV_VS_HDMI
 * @return The exit status of this api.
 * @retval SV_FAIL
 * @retval SV_SUCCESS
 */
EXTERN UINT8 bApiVideoMainSubSrc(UINT8 bMainSrc, UINT8 bSubSrc);

/**
 * Select TV color system
 *
 * @param bColSys Select the color system. SV_CS_AUTO / SV_CS_PAL / SV_CS_PAL_M
 * / SV_CS_PAL_N / SV_CS_NTSC358 / SV_CS_NTSC443 / SV_CS_SECAM
 * @return NONE
 */
EXTERN UINT8 fgApiVideoColorSystem(UINT8 bColSys);

/**
 * Get Current TV color system
 *
 * @return Current Color System. SV_CS_AUTO / SV_CS_PAL / SV_CS_PAL_M
 * / SV_CS_PAL_N / SV_CS_NTSC358 / SV_CS_NTSC443 / SV_CS_SECAM
 */
EXTERN UINT8 bApiVideoGetColorSystem(void);

EXTERN UINT8 bApiVideoGetInputTimingID(UINT8 bPath);

/**
 * @brief bApiVideoGetVideoTiming(UINT8 bPath)
 * Get input video timing
 * @param  bPath :main/sub path (SV_VP_MAIN / SV_VP_PIP)
 * @retval   input timing (index)
 */
EXTERN UINT8 bApiVideoGetVideoTiming(UINT8 bPath);


EXTERN UINT8 bApiVideoGetInputPolarity(UINT8 bPath);

/**
 * @brief wApiVideoGetInputWidth(UINT8 bPath)
 * Get input video resolution width (in pixel)
 * @param  bPath :main/sub path (SV_VP_MAIN / SV_VP_PIP)
 * @retval  width (in pixel)
 */
EXTERN UINT16 wApiVideoGetInputWidth(UINT8 bPath);

/**
 * @brief wApiVideoGetInputHeight(UINT8 bPath)
 * Get input video resolution hight (in pixel)
 * @param  bPath :main/sub path (SV_VP_MAIN / SV_VP_PIP)
 * @retval  hight (in pixel)
 */
EXTERN UINT16 wApiVideoGetInputHeight(UINT8 bPath);

/**
 * @brief bApiVideoGetRefreshRate(UINT8 bPath)
 * Get input video refreh rate(in Hz)
 * @param  bPath :main/sub path (SV_VP_MAIN / SV_VP_PIP)
 * @retval  refreh rate (in Hz)
 */
EXTERN UINT8 bApiVideoGetRefreshRate(UINT8 bPath);

/**
 * Check if input video is interlace mode
 *
 *  @param  bPath path ID
 *  @retval TRUE for interlace input and FALSE for progressive input
 */
EXTERN UINT8 bApiVideoIsSrcInterlace(UINT8 bPath);

EXTERN void vApiVideoSetFixColorSpaceMode(UINT8 bPath, SV_FIXCOLORSPACE_MODE bMode);
EXTERN SV_FIXCOLORSPACE_MODE bApiVideoGetFixColorSpaceMode(UINT8 bPath);

EXTERN void vApiHdmiColorModeChg(UINT8 bPath);

/**
 * @brief fgApiVideoIsVgaTiming(UINT8 bPath)
 * check if current input source is VGA timing
 * @param  bPath :main/sub path (SV_VP_MAIN / SV_VP_PIP)
 * @retval   TRUE/FALSE
 */
EXTERN UINT8 fgApiVideoIsVgaTiming(UINT8 bPath);

// REMOVE ME!
EXTERN UINT8 fgApiVideoIsVgaMode(UINT8 bPath, UINT8 bChkFlg);

// REMOVE ME!
EXTERN UINT8 fgApiVideoIsVgaModeEx(UINT8 bPath, UINT8 bChkFlg);

/**
 * @brief fgApiVideoIsSourceCVBS(UINT8 bPath)
 * check if current input source is CVBS
 * @param  bPath :main/sub path (SV_VP_MAIN / SV_VP_PIP)
 * @retval   TRUE/FALSE
 */
EXTERN BOOL fgApiVideoIsSourceCVBS(UINT8 bPath);

/**
 * @brief Get main/sub path video source type
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 */
EXTERN UINT8 bApiVideoGetSrcType(UINT8 bPath);

/**
 * @brief Set main/sub path video to tuner scanning mode
 * @param u4VdpId SV_VP_MAIN/SV_VP_PIP
 * @param u4Mode  scan mode on/off
 */
EXTERN void vApiVideoSetScanMode(UINT32 u4VdpId, UINT32 u4Mode);

/**
 * @brief Get tuner scanning mode status
 * @param u4VdpId SV_VP_MAIN/SV_VP_PIP
 * @retval  Scan mode on/off
 */
EXTERN UINT32 u4ApiVideoGetScanMode(UINT32 u4VdpId);

/**
 * @brief Set if show snow screen when ATV no signal
 * @param u4VdpId SV_VP_MAIN/SV_VP_PIP
 * @param u4Mode  scan mode on/off
 */
EXTERN void vApiVideoSetSnowScreen(UINT32 u4OnOff);

/**
 * @brief Get Snow screen current setting
 * @param N/A
 * @retval  Snow screen on/off
 */
EXTERN UINT32 u4ApiVideoGetSnowScreen(void);

/**
 * @brief Video init 
 * @param N/A
 * @retval  N/A
 */
EXTERN void vApiVideoInit(void);

#ifdef CC_AUD_QUERY_VDO_INFO
typedef struct
{
    UINT16 u2Width;
    UINT16 u2Height;
    UINT8 u1Timing;
    UINT8 u1Interlace;
    UINT8 u1RefreshRate;
    UINT8 u1PanelDelay;
} VDO_INPUT_INFO;
EXTERN void DrvVideoGetInputTimingInfo(UINT8 bPath, VDO_INPUT_INFO* info);
#endif

#endif // _VDO_IF_H_
 
