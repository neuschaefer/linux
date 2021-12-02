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
 * $RCSfile: mt53xx_vdp.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file vdp-mt53xx.h
 *  Define VDP interface.
 */

#ifndef VDP_MT53XX_H
#define VDP_MT53XX_H


#include <x_typedef.h>
#ifdef __KERNEL__
#include "vdp_if.h"
#else
#define VDP_MAX_REGION_WIDTH 10000
#define VDP_MAX_REGION_HEIGHT 10000

// Video plane argument
typedef enum
{
    VDP_ARG_BLENDING = 0,           ///< blending
    VDP_ARG_BRIGHTNESS,             ///< brightness
    VDP_ARG_CONTRAST,               ///< contrast
    VDP_ARG_HUE,                    ///< hue
    VDP_ARG_SATURATION,             ///< saturation
    VDP_ARG_CTI,                    		// < CTI
    VDP_ARG_LTI,                    			// < LTI    
    VDP_ARG_SHARPNESS_ON_OFF,       ///< sharpness switch
    VDP_ARG_SHARPNESS,              ///< sharpness
    VDP_ARG_NR,                     ///< 2D noise reduction
    VDP_ARG_COLOR_SUPPRESS,         ///< obsolete
    VDP_ARG_DEINT_FILM,             ///< cinema mode
    VDP_ARG_DEINT_DEINT,            ///< normal mode
    VDP_ARG_DEINT_EDGE,             ///< edge mode
    VDP_ARG_ADAPTIVE_LUMA,          ///< adaptive luma
    VDP_ARG_SCE,                    ///< second color enhance
    VDP_ARG_R_GAIN,                 ///< R-gain
    VDP_ARG_G_GAIN,                 ///< G-gain
    VDP_ARG_B_GAIN,                 ///< B-gain
    //mtk01140 20081016 range redirection for basic control
    VDP_ARG_R_OFFSET_BEFORE_GAIN,   ///< R-offset1
    VDP_ARG_G_OFFSET_BEFORE_GAIN,   ///< G-offset1
    VDP_ARG_B_OFFSET_BEFORE_GAIN,   ///< B-offset1
    VDP_ARG_R_OFFSET,               ///< R-offset
    VDP_ARG_G_OFFSET,               ///< G-offset
    VDP_ARG_B_OFFSET,               ///< B-offset
    VDP_ARG_GAMMA,                  ///< Gamma
    VDP_ARG_WHITE_PEAK_LMT,         ///< white peak limit
    VDP_ARG_BACK_LIGHT_LVL,         ///< back light level
    VDP_ARG_ADAPTIVE_BACK_LIGHT,    ///< adaptive back light
    VDP_ARG_3D_NR,                  ///< 3D noise reduction
    VDP_ARG_SOFT_BWS,               ///< SoftBWS
    VDP_ARG_DEMO,                   ///< Demo mode
    VDP_ARG_CDS,
    // New UI
    VDP_ARG_DI_FILM_MODE,
    VDP_ARG_DI_MA,
    VDP_ARG_DI_EDGE_PRESERVE,
    VDP_ARG_MJC_MODE,       
    VDP_ARG_MJC_EFFECT,
    VDP_ARG_MJC_DEMO,
    VDP_ARG_XVYCC,
    VDP_ARG_WCG,
    VDP_ARG_GAME_MODE,
    VDP_ARG_BLUE_STRETCH,
    VDP_ARG_MPEG_NR,
    VDP_ARG_NS                      ///< total color items
} VDP_ARG_TYPE;

typedef struct
{
    UINT32 u4X;
    UINT32 u4Y;
    UINT32 u4Width;
    UINT32 u4Height;
} VDP_REGION_T;
#endif

typedef struct
{
    UINT32 u4Top;
    UINT32 u4Bottom;
    UINT32 u4Left;
    UINT32 u4Right;
} VDP_OVERSCAN_REGION_T;

#define VDP_IOCTL_ENABLE            0
#define VDP_IOCTL_SET_MODE          1
#define VDP_IOCTL_SET_OUTREGION     2
#define VDP_IOCTL_SET_SRCREGION     3
#define VDP_IOCTL_SET_INPUT         4
#define VDP_IOCTL_SET_MASTER        5
#define VDP_IOCTL_INIT              6


typedef struct
{
    UCHAR           ucVdpId;

    UCHAR           ucMode;

	UCHAR           ucOutFullRegion;
	VDP_REGION_T    rOutRegion;

	UCHAR           ucSrcFullRegion;
	VDP_REGION_T    rSrcRegion;

	UCHAR           ucEsId;
	UCHAR           ucPort;
} VDP_IOCTL_T;


#endif  // VDP_MT53XX_H

