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
 * $RCSfile: u_vid_plane.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains video decoder driver specific
 *         definitions, which are exported.
 *---------------------------------------------------------------------------*/
#ifndef _U_VID_PLANE_H_
#define _U_VID_PLANE_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "inc/u_common.h"
#include "res_mngr/u_rm.h"
#include "graphic/u_gl.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* Get operations */
#define VID_PLA_GET_TYPE_CTRL              (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 0))
#define VID_PLA_GET_TYPE_MODE              (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 1))
#define VID_PLA_GET_TYPE_BG                (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 2))
#define VID_PLA_GET_TYPE_DISP_FMT          (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 3))
#define VID_PLA_GET_TYPE_DISP_REGION       (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 4))
#define VID_PLA_GET_TYPE_SRC_REGION        (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 5))
#define VID_PLA_GET_TYPE_BLENDING          (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 6))
#define VID_PLA_GET_TYPE_ENHANCE           (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 7))
#define VID_PLA_GET_TYPE_CAPABILITY        (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 8))
#define VID_PLA_GET_TYPE_QV_INP            (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 9))
#define VID_PLA_GET_TYPE_BRIGHTNESS        (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)10))
#define VID_PLA_GET_TYPE_CONTRAST          (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)11))
#define VID_PLA_GET_TYPE_HUE               (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)12))
#define VID_PLA_GET_TYPE_SATURATION        (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)13))
#define VID_PLA_GET_TYPE_CTI               (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)14))
#define VID_PLA_GET_TYPE_ETI               (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)15))
#define VID_PLA_GET_TYPE_SHARPNESS         (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)16))
#define VID_PLA_GET_TYPE_COLOR_SUPPRESS    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)17))
#define VID_PLA_GET_TYPE_OVER_SCAN_CLIPPER (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)18))
#define VID_PLA_GET_TYPE_MIN_MAX           (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)19))
#define VID_PLA_GET_TYPE_COLOR_GAIN        (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)20))
#define VID_PLA_GET_TYPE_COLOR_GAIN_MAX    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)21))
#define VID_PLA_GET_TYPE_COLOR_GAIN_MIN    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)22))
#define VID_PLA_GET_TYPE_COLOR_OFFSET      (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)23))
#define VID_PLA_GET_TYPE_COLOR_OFFSET_MAX  (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)24))
#define VID_PLA_GET_TYPE_COLOR_OFFSET_MIN  (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)25))
#define VID_PLA_GET_TYPE_NR                (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)26))
#define VID_PLA_GET_TYPE_BLACK_LVL_EXT     (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)27))
#define VID_PLA_GET_TYPE_WHITE_PEAK_LMT    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)28))
#define VID_PLA_GET_TYPE_FLESH_TONE        (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)29))
#define VID_PLA_GET_TYPE_LUMA              (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)30))
#define VID_PLA_GET_TYPE_MAX               (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)31))
#define VID_PLA_GET_TYPE_3D_NR             (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)32))
#define VID_PLA_GET_TYPE_FINAL_DISP_REGION (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)33))
#define VID_PLA_GET_TYPE_FINAL_SRC_REGION  (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)34))
#define VID_PLA_GET_TYPE_LETTER_BOX_DETECT (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)35))
#define VID_PLA_GET_TYPE_DYNAMIC_SCALING   (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)36))
#define VID_PLA_GET_TYPE_SPLIT_SCRN_DEMO   (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)37))
#define VID_PLA_GET_TYPE_H_POSITION        (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)38))
#define VID_PLA_GET_TYPE_V_POSITION        (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)39))
#define VID_PLA_GET_TYPE_H_SIZE            (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)40))
#define VID_PLA_GET_TYPE_V_SIZE            (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)41))
#define VID_PLA_GET_TYPE_FILM_MODE         (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)42))
#define VID_PLA_GET_TYPE_DISP_DIMENSION    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)43))
#define VID_PLA_GET_TYPE_FORCE_UNMUTE_ON_NO_SIG (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)44))
#define VID_PLA_GET_TYPE_PIC_MODE          (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)45))
#define VID_PLA_GET_TYPE_HDMI_MODE         (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)46))
#define VID_PLA_GET_TYPE_REGION_CAPABILITY (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)47))
#define VID_PLA_GET_TYPE_LB_DETECT_CONFIG  (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)48))
#define VID_PLA_GET_TYPE_PICTURE_INFO      (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)49))
#define VID_PLA_GET_TYPE_MODE_CAPABILITY   (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)50))
#define VID_PLA_GET_TYPE_DI_FILM_MODE      (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)51))
#define VID_PLA_GET_TYPE_DI_MA             (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)52))
#define VID_PLA_GET_TYPE_DI_EDGE_PRESERVE  (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)53))
#define VID_PLA_GET_TYPE_MJC_EFFECT        (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)54))
#define VID_PLA_GET_TYPE_MJC_MODE          (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)55))
#define VID_PLA_GET_TYPE_MJC_DEMO          (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)56))
#define VID_PLA_GET_TYPE_HDMI_WITH_XVYCC   (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)57))
#define VID_PLA_GET_TYPE_XVYCC             (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)58))
#define VID_PLA_GET_TYPE_WCG               (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)59))
#define VID_PLA_GET_TYPE_GAME_MODE         (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)60))
#define VID_PLA_GET_TYPE_RED_STRETCH       (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)61))
#define VID_PLA_GET_TYPE_GREEN_STRETCH     (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)62))
#define VID_PLA_GET_TYPE_BLUE_STRETCH      (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)63))
#define VID_PLA_GET_TYPE_MPEG_NR           (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)64))
#define VID_PLA_GET_TYPE_SUPER_FREEZE      (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)65))
#define VID_PLA_GET_TYPE_HDCP_KEY_EXIST    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)66))
#define VID_PLA_GET_TYPE_BLACK_STRETCH     (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)67))
#define VID_PLA_GET_TYPE_WHITE_STRETCH     (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)68))
#define VID_PLA_GET_TYPE_FLIP              (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)69))
#define VID_PLA_GET_TYPE_MIRROR            (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)70))
#define VID_PLA_GET_TYPE_DRAMINF           (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)71))
#define VID_PLA_GET_TYPE_LCDIM             (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)72))
#define VID_PLA_GET_TYPE_DELAY_TIME        (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)73))
#define VID_PLA_GET_TYPE_3D_MODE           (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)74))
#define VID_PLA_GET_TYPE_3D_LR_SWITCH      (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)75))
#define VID_PLA_GET_TYPE_3D_PARALLAX       (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)76))
#define VID_PLA_GET_TYPE_3D_FLD_DEPTH      (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)77))
#define VID_PLA_GET_TYPE_3D_TO_2D          (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)78))
#define VID_PLA_GET_TYPE_3D_FPR            (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)79)) 
#define VID_PLA_GET_TYPE_OVERSCAN_ENABLE   (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)80)) 
#define VID_PLA_GET_TYPE_NON_LINEAR_SCALING (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)81)) 
#define VID_PLA_GET_TYPE_3D_PANEL_TYPE     (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)82)) 
#define VID_PLA_GET_TYPE_3D_MJC_ENABLE     (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)83)) 
#define VID_PLA_GET_TYPE_3D_PROTRUDE_SCR   (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)84))
#define VID_PLA_GET_TYPE_3D_DISTANCE_TV    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)85))
#define VID_PLA_GET_TYPE_3D_OSD_DEPTH      (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)86))
#define VID_PLA_GET_TYPE_3D_NAV            (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)87))
#define VID_PLA_GET_TYPE_3D_FMT_CAPABILITY (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)88))
#define VID_PLA_GET_TYPE_3D_CTRL_CAPABILITY (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)89))
#define VID_PLA_GET_TYPE_3D_NAVI_AUTO_CHG_EN (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)90))
#define VID_PLA_GET_TYPE_3D_IMAGE_SAFETY_EN (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)91))
#define VID_PLA_GET_TYPE_3D_NAVI_TAG       (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)92))
#define VID_PLA_GET_TYPE_ASP_RATIO_ENABLE  (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)93))

/* Set operations */
#define VID_PLA_SET_TYPE_CTRL             ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 0)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_MODE             ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 1)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_BG                (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 2))
#define VID_PLA_SET_TYPE_DISP_FMT         ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 3)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_DISP_REGION       (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 4))
#define VID_PLA_SET_TYPE_SRC_REGION        (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 5))
#define VID_PLA_SET_TYPE_BLENDING         ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 6)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_ENHANCE           (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 7))
#define VID_PLA_SET_TYPE_QV_INP            (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 8))
#define VID_PLA_SET_TYPE_BRIGHTNESS       ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 9)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_CONTRAST         ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)10)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_HUE              ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)11)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_SATURATION       ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)12)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_CTI              ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)13)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_ETI              ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)14)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_SHARPNESS        ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)15)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_COLOR_SUPPRESS   ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)16)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_OVER_SCAN_CLIPPER (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)17))
#define VID_PLA_SET_TYPE_COLOR_GAIN        (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)18))
#define VID_PLA_SET_TYPE_COLOR_OFFSET      (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)19))
#define VID_PLA_SET_TYPE_NR               ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)20)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_BLACK_LVL_EXT    ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)21)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_WHITE_PEAK_LMT   ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)22)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_FLESH_TONE       ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)23)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_LUMA             ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)24)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_3D_NR            ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)25)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_LETTER_BOX_DETECT ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)26)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_DYNAMIC_SCALING  ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)27)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_SPLIT_SCRN_DEMO  ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)28)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_H_POSITION       ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)29)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_V_POSITION       ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)30)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_H_SIZE           ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)31)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_V_SIZE           ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)32)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_FILM_MODE        ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)33)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_DISP_DIMENSION    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)34))
#define VID_PLA_SET_TYPE_INJECT_VDP        (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)35))
#define VID_PLA_SET_TYPE_FORCE_UNMUTE_ON_NO_SIG ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)36)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_PIC_MODE         ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)37)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_HDMI_MODE        ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)38)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_NFY_FCT          ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)39)) | RM_SET_TYPE_ARG_CALLBACK)
#define VID_PLA_SET_TYPE_LB_DETECT_CONFIG  (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)40))
#define VID_PLA_SET_TYPE_DI_FILM_MODE     ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)41)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_DI_MA            ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)42)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_DI_EDGE_PRESERVE ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)43)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_MJC_EFFECT       ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)44)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_MJC_MODE         ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)45)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_MJC_DEMO         ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)46)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_XVYCC            ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)47)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_WCG              ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)48)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_GAME_MODE        ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)49)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_BLUE_STRETCH     ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)50)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_MPEG_NR          ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)51)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_SUPER_FREEZE     ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)52)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_SUPER_BLANK       (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)53))
#define VID_PLA_SET_TYPE_THUMBNAIL_ENABLE ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)54)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_BLACK_STRETCH    ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)55)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_WHITE_STRETCH    ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)56)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_ATOMIC_CMD_GRP    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)57))
#define VID_PLA_SET_TYPE_COLORSPACE       ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)58)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_SRM_MODE         ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)59)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_FLIP             ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)60)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_MIRROR           ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)61)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_FORCE_UNMUTE     ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)62)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_LCDIM            ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)63)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_3D_MODE          ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)64)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_3D_LR_SWITCH     ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)65)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_3D_PARALLAX      ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)66)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_3D_FLD_DEPTH     ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)67)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_3D_TO_2D         ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)68)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_3D_FPR           ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)69)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_PEDESTAL         ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)70)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_HDMI_RANGE       ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)71)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_STATIC_GAMMA     ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)72)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_ANGULAR_SCE      (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)73))
#define VID_PLA_SET_TYPE_DOWN_BW_MODE     ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)74)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_CHAP_INFO        (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 75)) 
#define VID_PLA_SET_TYPE_3D_PROTRUDE_SCR  ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)76)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_3D_DISTANCE_TV   ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)77)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_3D_OSD_DEPTH     ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)78)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_3D_NAV           ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)79)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_3D_NAVI_AUTO_CHG_EN ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)80)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_3D_IMAGE_SAFETY_EN ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)81)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_PLA_SET_TYPE_SUPER_RESOLUTION ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)82)) | RM_SET_TYPE_ARG_NO_REF)

/* Notify conditions */
typedef enum
{
    VID_PLA_COND_ERROR = -1,
    VID_PLA_COND_EOS,
    VID_PLA_COND_SYNC_POINT,     /* current, useless, to compliant older driver code */
    VID_PLA_COND_RANGE_AB_DONE,
    VID_PLA_COND_DISP_MODE,      /* Scaler display mode support or not support */
    VID_PLA_COND_DISP_FRAME,     /* notify MW/AP the first frame has displayed */
    VID_PLA_COND_CHAP_DONE     
}   VID_PLA_COND_T;

/* VID_PLA_SET_TYPE_NFY_FCT settings *****************************************/
typedef VOID (*x_vid_pla_nfy_fct) (
    VOID*               pv_nfy_tag,
    VID_PLA_COND_T      e_nfy_cond,
    UINT32              ui4_data_1,
    UINT32              ui4_data_2
    );

typedef struct _VID_PLA_NFY_INFO_T
{
    VOID*              pv_tag;
    x_vid_pla_nfy_fct  pf_vid_pla_nfy;
}   VID_PLA_NFY_INFO_T;

/* VID_PLA_GET_TYPE_CTRL/VID_PLA_SET_TYPE_CTRL Control settings. *************/
typedef enum
{
    VID_PLA_CTRL_RESET = 0,
    VID_PLA_CTRL_DISABLE,
    VID_PLA_CTRL_ENABLE
}   VID_PLA_CTRL_T;


/* VID_PLA_GET_TYPE_MODE/VID_PLA_SET_TYPE_MODE Plane modes *******************/
/* VID_PLA_GET_TYPE_MODE_CAPABILITY ******************************************/
typedef enum
{
    VID_PLA_NORMAL = 0,
    VID_PLA_BLANK,
    VID_PLA_QUAD_VIDEO,
    VID_PLA_DEINT,
    VID_PLA_FREEZE
}   VID_PLA_MODE_T;

/* VID_PLA_SET_TYPE_SRM_MODE *************************************************/
typedef enum
{
    VID_PLA_SRM_MODE_OFF = 0,
    VID_PLA_SRM_MODE_THUMBNAIL,
    VID_PLA_SRM_MODE_VIDEO
}   VID_PLA_SRM_MODE_T;

/* VID_PLA_GET_TYPE_DISP_FMT/VID_PLA_SET_TYPE_DISP_FMT display format ********/
typedef enum
{
    VID_PLA_DISP_FMT_UNKNOWN = 0,
    VID_PLA_DISP_FMT_NORMAL,
    VID_PLA_DISP_FMT_LETTERBOX,
    VID_PLA_DISP_FMT_PAN_SCAN,
    VID_PLA_DISP_FMT_USER_DEFINED,
    VID_PLA_DISP_FMT_NON_LINEAR_ZOOM,
    VID_PLA_DISP_FMT_DOT_BY_DOT,
    VID_PLA_DISP_FMT_CUSTOM_0,
    VID_PLA_DISP_FMT_CUSTOM_1,
    VID_PLA_DISP_FMT_CUSTOM_2,
    VID_PLA_DISP_FMT_CUSTOM_3,
    VID_PLA_DISP_FMT_CUSTOM_4,
    VID_PLA_DISP_FMT_CUSTOM_5,
    VID_PLA_DISP_FMT_CUSTOM_6,
    VID_PLA_DISP_FMT_CUSTOM_7,
    VID_PLA_DISP_FMT_NLZ_CUSTOM_0, /* NONE LINEAR ZOOM */
    VID_PLA_DISP_FMT_NLZ_CUSTOM_1, /* NONE LINEAR ZOOM */
    VID_PLA_DISP_FMT_NLZ_CUSTOM_2, /* NONE LINEAR ZOOM */
    VID_PLA_DISP_FMT_NLZ_CUSTOM_3, /* NONE LINEAR ZOOM */

    VID_PLA_DISP_FMT_MAX           /* add max for enum count */
}   VID_PLA_DISP_FMT_T;

/* VID_PLA_GET_TYPE_BLENDING/VID_PLA_SET_TYPE_BLENDING ***********************/
typedef UINT8    VID_PLA_BLENDING_T;

/* VID_PLA_GET_TYPE_CAPABILITY Capability info *******************************/
#define VID_PLA_CAP_NONE                 ((UINT32) 0)
#define VID_PLA_CAP_MODE_NORMAL          MAKE_BIT_MASK_32( 0 )
#define VID_PLA_CAP_MODE_BLANK           MAKE_BIT_MASK_32( 1 )
#define VID_PLA_CAP_MODE_QUAD_VIDEO      MAKE_BIT_MASK_32( 2 )
#define VID_PLA_CAP_MODE_DEINT           MAKE_BIT_MASK_32( 3 )
#define VID_PLA_CAP_BG                   MAKE_BIT_MASK_32( 4 )
#define VID_PLA_CAP_DISP_FMT             MAKE_BIT_MASK_32( 5 )
#define VID_PLA_CAP_DISP_REGION          MAKE_BIT_MASK_32( 6 )
#define VID_PLA_CAP_SRC_REGION           MAKE_BIT_MASK_32( 7 )
#define VID_PLA_CAP_BLENDING             MAKE_BIT_MASK_32( 8 )
#define VID_PLA_CAP_ENHANCE              MAKE_BIT_MASK_32( 9 )
#define VID_PLA_CAP_BRIGHTNESS           MAKE_BIT_MASK_32( 10 )
#define VID_PLA_CAP_CONTRAST             MAKE_BIT_MASK_32( 11 )
#define VID_PLA_CAP_HUE                  MAKE_BIT_MASK_32( 12 )
#define VID_PLA_CAP_SATURATION           MAKE_BIT_MASK_32( 13 )
#define VID_PLA_CAP_CTI                  MAKE_BIT_MASK_32( 14 )
#define VID_PLA_CAP_ETI                  MAKE_BIT_MASK_32( 15 )
#define VID_PLA_CAP_SHARPNESS            MAKE_BIT_MASK_32( 16 )
#define VID_PLA_CAP_COLOR_SUPPRESS       MAKE_BIT_MASK_32( 17 )
#define VID_PLA_CAP_OVER_SCAN_CLIPPER    MAKE_BIT_MASK_32( 18 )
#define VID_PLA_CAP_COLOR_GAIN           MAKE_BIT_MASK_32( 19 )
#define VID_PLA_CAP_COLOR_OFFSET         MAKE_BIT_MASK_32( 20 )
#define VID_PLA_CAP_NR                   MAKE_BIT_MASK_32( 21 )
#define VID_PLA_CAP_BLACK_LVL_EXT        MAKE_BIT_MASK_32( 22 )
#define VID_PLA_CAP_WHITE_PEAK_LMT       MAKE_BIT_MASK_32( 23 )
#define VID_PLA_CAP_FLESH_TONE           MAKE_BIT_MASK_32( 24 )
#define VID_PLA_CAP_LUMA                 MAKE_BIT_MASK_32( 25 )
#define VID_PLA_CAP_H_POS                MAKE_BIT_MASK_32( 26 )
#define VID_PLA_CAP_V_POS                MAKE_BIT_MASK_32( 27 )
#define VID_PLA_CAP_H_SIZE               MAKE_BIT_MASK_32( 28 )
#define VID_PLA_CAP_V_SIZE               MAKE_BIT_MASK_32( 29 )
#define VID_PLA_CAP_FILM_MODE            MAKE_BIT_MASK_32( 30 )
#define VID_PLA_CAP_FORCE_UNMUTE_ON_NO_SIG MAKE_BIT_MASK_32( 31 )
#define VID_PLA_CAP_PIC_MODE             MAKE_BIT_MASK_32( 32 )

/* DISP_DIMENSION setting **********/
typedef struct _VID_PLA_DISP_DIMENSION_T
{
    UINT32      ui4_width;
    UINT32      ui4_height;
}   VID_PLA_DISP_DIMENSION_T;

/* DISP_DIMENSION setting **********/
typedef struct _VID_PLA_INJECT_VDP_T
{
    VOID            *pv_src;
    UINT32          ui4_src_x;
    UINT32          ui4_src_y;
    UINT32          ui4_src_pitch;
    UINT32          ui4_dst_x;
    UINT32          ui4_dst_y;
    UINT32          ui4_width;
    UINT32          ui4_height;
    GL_COLORMODE_T  e_src_cm;
}   VID_PLA_INJECT_VDP_T;

/* VID_PLA_GET_TYPE_BG/VID_PLA_SET_TYPE_BG background color setting **********/
typedef struct _VID_PLA_BG_COLOR_T
{
    UINT8       ui1_r;
    UINT8       ui1_g;
    UINT8       ui1_b;
}   VID_PLA_BG_COLOR_T;

/* VID_PLA_GET_TYPE_DISP_REGION/VID_PLA_SET_TYPE_DISP_REGION *****************/
/* VID_PLA_GET_TYPE_SRC_REGION/VID_PLA_SET_TYPE_SRC_REGION *******************/
typedef struct _VID_PLA_VID_REGION_T
{
    BOOL        b_set_to_full_scr;
    UINT32      ui4_x;
    UINT32      ui4_y;
    UINT32      ui4_width;
    UINT32      ui4_height;
}   VID_PLA_VID_REGION_T;

#ifdef DIVX_PLUS_CER
typedef struct _VID_PLA_VID_CHAP_T 
{
    BOOL        b_seek;
    BOOL        b_first;
    UINT64      ui8_start_time;
    UINT64      ui8_end_time;
}   VID_PLA_VID_CHAP_T;
#endif

/* VID_PLA_GET_TYPE_REGION_CAPABILITY ****************************************/
typedef struct _VID_PLA_VID_REGION_CAPABILITY_T
{
    UINT32      e_get_type;         /* [IN] get type (VID_PLA_GET_TYPE_DISP_REGION/VID_PLA_GET_TYPE_SRC_REGION) */
    BOOL        b_is_enable;        /* [OUT] is function enable? */
    UINT32      ui4_x_min;
    UINT32      ui4_x_max;
    UINT32      ui4_y_min;
    UINT32      ui4_y_max;
    UINT32      ui4_width_min;
    UINT32      ui4_width_max;
    UINT32      ui4_height_min;
    UINT32      ui4_height_max;
}   VID_PLA_VID_REGION_CAPABILITY_T;

/* VID_PLA_GET_ASPECT_RATIO_TYPE ******************************************************/
typedef struct VID_PLA_GET_ASPECT_RATIO_TYPE_T
{
    BOOL        b_e_get_info;              /* get info*/
    UINT16      ui2_asp_ratio_type;        /* asp_ratio_type */
} VID_PLA_GET_ASPECT_RATIO_TYPE_T;

/* VID_PLA_GET_TYPE_ENHANCE/VID_PLA_SET_TYPE_ENHANCE *************************/
/* video plane enhance mode setting */
typedef enum
{
    VID_PLA_ENHANCE_DISABLE = 0,
    VID_PLA_ENHANCE,
    VID_PLA_BLUR
}   VID_PLA_ENHANCE_MODE_T;

typedef struct _VID_PLA_ENHANCE_T
{
    VID_PLA_ENHANCE_MODE_T    e_mode;
    UINT8                     ui1_level;
}   VID_PLA_ENHANCE_T;

/* VID_PLA_GET_TYPE_QV_INP/VID_PLA_SET_TYPE_QV_INP ***************************/
/* The input port definition of quad video */
typedef enum
{
    VID_PLA_QV_INP_NULL = 0,
    VID_PLA_QV_INP_1,           /* port 0 */
    VID_PLA_QV_INP_2,           /* port 1 */
    VID_PLA_QV_INP_3,           /* port 2 */
    VID_PLA_QV_INP_4            /* port 3 */
}   VID_PLA_QV_INP_T;

/* Quad-video info */
typedef struct _VID_PLA_QV_INFO_T
{
    VID_PLA_QV_INP_T        e_tl;
    VID_PLA_QV_INP_T        e_tr;
    VID_PLA_QV_INP_T        e_br;
    VID_PLA_QV_INP_T        e_bl;
}   VID_PLA_QV_INFO_T;

/* VID_PLA_GET_TYPE_MIN_MAX **************************************************/
typedef struct _VID_PLA_MIN_MAX_INFO_T
{
    UINT32      e_get_type;         /* get type */
    UINT32      ui4_min_value;      /* minimum */
    UINT32      ui4_max_value;      /* maximum */
} VID_PLA_MIN_MAX_INFO_T;

#define VID_PLA_MIN_MAX_INFO_WIDTH_MASK           ((UINT32)0xffff)
#define VID_PLA_MIN_MAX_INFO_HEIGHT_MASK          ((UINT32)0xffff0000)

/*
#define VID_PLA_MIN_MAX_INFO_SET_WIDTH( ui4_value, width )                    \
    ( (ui4_value) = (((UINT32)(ui4_value) & 0xFFFF0000) |                     \
                     ((UINT32)(width) & VID_PLA_MIN_MAX_INFO_WIDTH_MASK)) )

#define VID_PLA_MIN_MAX_INFO_SET_HEIGHT( ui4_value, height )                  \
    ( (ui4_value) = (((UINT32)(ui4_value) & 0xFFFF) |                         \
                     ((UINT32)(height<<16) & VID_PLA_MIN_MAX_INFO_HEIGHT_MASK)) )
*/

/* VID_PLA_GET_TYPE_COLOR_GAIN/VID_PLA_SET_TYPE_COLOR_GAIN *******************/
/* VID_PLA_GET_TYPE_COLOR_GAIN_MIN/VID_PLA_GET_TYPE_COLOR_GAIN_MAX ***********/
/* VID_PLA_GET_TYPE_COLOR_OFFSET/VID_PLA_SET_TYPE_COLOR_OFFSET ***************/
/* VID_PLA_GET_TYPE_COLOR_OFFSET_MIN/VID_PLA_GET_TYPE_COLOR_OFFSET_MAX *******/ 
typedef struct _VID_PLA_COLOR_GAIN_T
{
    UINT16      ui2_r_gain;
    UINT16      ui2_g_gain;
    UINT16      ui2_b_gain;
}   VID_PLA_COLOR_GAIN_T;

/* VID_PLA_GET_TYPE_OVER_SCAN_CLIPPER/VID_PLA_SET_TYPE_OVER_SCAN_CLIPPER *****/
typedef struct _VID_PLA_OVER_SCAN_CLIPPER_T
{
    UINT32      ui4_top;
    UINT32      ui4_bottom;
    UINT32      ui4_left;
    UINT32      ui4_right;
}   VID_PLA_OVER_SCAN_CLIPPER_T;

/* VID_PLA_GET_TYPE_MAX ******************************************************/
typedef struct _VID_PLA_MAX_INFO_T
{
    UINT32      e_get_type;         /* get type */
    UINT32      ui4_max_value;      /* maximum */
} VID_PLA_MAX_INFO_T;

/* VID_PLA_GET_TYPE_SPLIT_SCRN_DEMO/VID_PLA_SET_TYPE_SPLIT_SCRN_DEMO *********/
typedef enum
{
    VID_PLA_SPLIT_SCRN_DEMO_OFF = 0,
    VID_PLA_SPLIT_SCRN_DEMO_LEFT,
    VID_PLA_SPLIT_SCRN_DEMO_RIGHT
}   VID_PLA_SPLIT_SCRN_DEMO_T;

/* VID_PLA_GET_TYPE_PIC_MODE/VID_PLA_SET_TYPE_PIC_MODE ***********************/
typedef enum
{
    VID_PLA_PIC_MODE_DEFAULT = 0,
    VID_PLA_PIC_MODE_CUSTOM_1,
    VID_PLA_PIC_MODE_CUSTOM_2,
    VID_PLA_PIC_MODE_CUSTOM_3,
    VID_PLA_PIC_MODE_CUSTOM_4,
    VID_PLA_PIC_MODE_CUSTOM_5,
    VID_PLA_PIC_MODE_CUSTOM_6,
    VID_PLA_PIC_MODE_CUSTOM_7,
    VID_PLA_PIC_MODE_CUSTOM_8,
    VID_PLA_PIC_MODE_CUSTOM_9,
    VID_PLA_PIC_MODE_MAX
}   VID_PLA_PIC_MODE_T;

/* VID_PLA_GET_TYPE_HDMI_MODE/VID_PLA_SET_TYPE_HDMI_MODE *********************/
typedef enum
{
    VID_PLA_HDMI_MODE_UNKNOWN = 0,
    VID_PLA_HDMI_MODE_AUTO,
    VID_PLA_HDMI_MODE_GRAPHIC,
    VID_PLA_HDMI_MODE_VIDEO
}   VID_PLA_HDMI_MODE_T;

/* VID_PLA_GET_TYPE_LB_DETECT_CONFIG/VID_PLA_SET_TYPE_LB_DETECT_CONFIG *******/
typedef VOID (*x_vid_pla_lb_detect_nfy_fct) (
    VOID*                   pv_nfy_tag,
    /* original picture size */
    UINT32                  ui4_src_width,
    UINT32                  ui4_src_height,
    /* active (no black bar) region information */
    UINT32                  ui4_active_width,
    UINT32                  ui4_active_height,
    UINT32                  ui4_active_x,
    UINT32                  ui4_active_y );

typedef struct _VID_PLA_LB_DETECT_NFY_INFO_T
{
    x_vid_pla_lb_detect_nfy_fct    pf_lb_detect_nfy;
    VOID*                          pv_nfy_tag;
} VID_PLA_LB_DETECT_NFY_INFO_T;

typedef struct _VID_PLA_LB_DETECT_CONFIG_T
{
    UINT32                          ui4_time_threshold;
    UINT32                          ui4_bounds_top;
    UINT32                          ui4_bounds_bottom;
    UINT32                          ui4_bounds_left;
    UINT32                          ui4_bounds_right;
    /* enhancement for manual mode */
    BOOL                            b_manual_mode;          /* false means auto mode */
    VID_PLA_LB_DETECT_NFY_INFO_T    t_lb_detect_nfy_info;   /* callback function information (SET only) */
} VID_PLA_LB_DETECT_CONFIG_T;

/* VID_PLA_GET_TYPE_PICTURE_INFO *********************************************/
typedef enum
{
    VID_PLA_PIC_INFO_TYPE_UNKNOWN= 0,
    VID_PLA_PIC_INFO_TYPE_GOP,
    VID_PLA_PIC_INFO_TYPE_PICTURE
}   VID_PLA_PIC_INFO_TYPE_T;

typedef struct _VID_PLA_TIME_CODE_INFO_T
{
    UINT8                     ui1_hours;    /* 0 - 23 */
    UINT8                     ui1_minutes;  /* 0 - 59 */
    UINT8                     ui1_seconds;  /* 0 - 59 */
    UINT8                     ui1_pictures; /* 0 - 59 */
    INT32                     i4_total_dur_adj;
} VID_PLA_TIME_CODE_INFO_T;

typedef struct _VID_PLA_PICTURE_INFO_T
{
    VID_PLA_PIC_INFO_TYPE_T   e_type;
    UINT64                    ui8_offset;
    union {
        VID_PLA_TIME_CODE_INFO_T  t_time_code;
        UINT64                    ui8_pts;
    } u;
}   VID_PLA_PICTURE_INFO_T;

/* VID_PLA_GET_TYPE_DI_FILM_MODE/VID_PLA_SET_TYPE_DI_FILM_MODE ***************/
typedef enum
{
    VID_PLA_DI_FILM_MODE_OFF = 0,
    VID_PLA_DI_FILM_MODE_SLOW_PICTURE,
    VID_PLA_DI_FILM_MODE_ACTION_PICTURE
}   VID_PLA_DI_FILM_MODE_T;

/* VID_PLA_GET_TYPE_DI_MA/VID_PLA_SET_TYPE_DI_MA *****************************/
typedef enum
{
    VID_PLA_DI_MA_SLOW_PICTURE = 0,
    VID_PLA_DI_MA_ACTION_PICTURE
}   VID_PLA_DI_MA_T;

/* VID_PLA_GET_TYPE_DI_EDGE_PRESERVE/VID_PLA_SET_TYPE_DI_EDGE_PRESERVE *******/
typedef enum
{
    VID_PLA_DI_EDGE_PRESERVE_WEAK = 0,
    VID_PLA_DI_EDGE_PRESERVE_STRONG
}   VID_PLA_DI_EDGE_PRESERVE_T;

/* VID_PLA_GET_TYPE_MJC_EFFECT/VID_PLA_SET_TYPE_MJC_EFFECT *******************/
typedef enum
{
    VID_PLA_MJC_EFFECT_OFF = 0,
    VID_PLA_MJC_EFFECT_LOW,
    VID_PLA_MJC_EFFECT_MIDDLE,
    VID_PLA_MJC_EFFECT_HIGH 
}   VID_PLA_MJC_EFFECT_T;

/* VID_PLA_GET_TYPE_MJC_MODE/VID_PLA_SET_TYPE_MJC_MODE ***********************/
typedef enum
{
    VID_PLA_MJC_MODE_0 = 0, /* NORMAL */
    VID_PLA_MJC_MODE_1,     /* FALLBACK */
    VID_PLA_MJC_MODE_2      /* NO_FILM */
}   VID_PLA_MJC_MODE_T;

/* VID_PLA_GET_TYPE_MJC_DEMO/VID_PLA_SET_TYPE_MJC_DEMO ***********************/
typedef enum
{
    VID_PLA_MJC_DEMO_OFF = 0,
    VID_PLA_MJC_DEMO_RIGHT,
    VID_PLA_MJC_DEMO_LEFT
}   VID_PLA_MJC_DEMO_T;

/* VID_PLA_GET_TYPE_XVYCC/VID_PLA_SET_TYPE_XVYCC *****************************/
typedef enum
{
    VID_PLA_XVYCC_OFF = 0,
    VID_PLA_XVYCC_ON
}   VID_PLA_XVYCC_T;

/* VID_PLA_GET_TYPE_WCG/VID_PLA_SET_TYPE_WCG *********************************/
typedef enum
{
    VID_PLA_WCG_OFF = 0,
    VID_PLA_WCG_ON
}   VID_PLA_WCG_T;

/* VID_PLA_GET_TYPE_GAME_MODE/VID_PLA_SET_TYPE_GAME_MODE *********************/
typedef enum
{
    VID_PLA_GAME_MODE_OFF = 0,
    VID_PLA_GAME_MODE_ON
}   VID_PLA_GAME_MODE_T;

/* VID_PLA_GET_TYPE_BLUE_STRETCH/VID_PLA_SET_TYPE_BLUE_STRETCH ***************/
typedef enum
{
    VID_PLA_BLUE_STRETCH_OFF = 0,
    VID_PLA_BLUE_STRETCH_BLUE,
    VID_PLA_BLUE_STRETCH_RED,
    VID_PLA_BLUE_STRETCH_GREEN
}   VID_PLA_BLUE_STRETCH_T;

/* VID_PLA_GET_TYPE_MPEG_NR/VID_PLA_SET_TYPE_MPEG_NR *************************/
typedef enum
{
    VID_PLA_MPEG_NR_OFF = 0,
    VID_PLA_MPEG_NR_LOW,
    VID_PLA_MPEG_NR_MIDDLE,
    VID_PLA_MPEG_NR_HIGH 
}   VID_PLA_MPEG_NR_T;

/* VID_PLA_GET_TYPE_SUPER_FREEZE **********************************************/
typedef enum
{
    VID_PLA_SUPER_FREEZE_MODE_DISABLE = 0,
    VID_PLA_SUPER_FREEZE_MODE_ENABLE,
    VID_PLA_SUPER_FREEZE_MODE_FREEZING
}   VID_PLA_SUPER_FREEZE_MODE_T;


/* VID_PLA_GET_TYPE_LCDIM / VID_PLA_SET_TYPE_LCDIM ****************************/
typedef enum
{
  VID_PLA_LOCAL_DIMMING_MODE_OFF = 0,
  VID_PLA_LOCAL_DIMMING_MODE_LOW,
  VID_PLA_LOCAL_DIMMING_MODE_MID,
  VID_PLA_LOCAL_DIMMING_MODE_HIGH
} VID_PLA_LOCAL_DIMMING_MODE_T;


/* VID_PLA_SET_TYPE_SUPER_FREEZE **********************************************/
typedef enum
{
    VID_PLA_SUPER_FREEZE_TYPE_MEMO = 0,
    VID_PLA_SUPER_FREEZE_TYPE_FORCE    /* for super freeze */
}   VID_PLA_SUPER_FREEZE_TYPE_T;

typedef struct _VID_PLA_SUPER_FREEZE_T
{
    VID_PLA_SUPER_FREEZE_TYPE_T e_type;
    BOOL                        b_enable;
}   VID_PLA_SUPER_FREEZE_T;


/* VID_PLA_SET_TYPE_SUPER_BLANK ***********************************************/
typedef struct _VID_PLA_SUPER_BLANK_T
{
    BOOL                 b_enable;
    VID_PLA_BG_COLOR_T   t_bg_color;
    VID_PLA_VID_REGION_T t_disp_region;
}   VID_PLA_SUPER_BLANK_T;

/* VID_PLA_SET_TYPE_BLACK_STRETCH *********************************************/
typedef enum
{
    VID_PLA_BLACK_STRETCH_OFF = 0,
    VID_PLA_BLACK_STRETCH_LOW,
    VID_PLA_BLACK_STRETCH_MID,
    VID_PLA_BLACK_STRETCH_HIGH
}   VID_PLA_BLACK_STRETCH_T;

/* VID_PLA_SET_TYPE_WHITE_STRETCH *********************************************/
typedef enum
{
    VID_PLA_WHITE_STRETCH_OFF = 0,
    VID_PLA_WHITE_STRETCH_LOW,
    VID_PLA_WHITE_STRETCH_MID,
    VID_PLA_WHITE_STRETCH_HIGH
}   VID_PLA_WHITE_STRETCH_T;

/* VID_PLA_SET_TYPE_ATOMIC_CMD_GRP ********************************************/
#define VID_PLA_ATOMIC_CMD_CODE_END ((DRV_SET_TYPE_T) 0)
typedef struct _VID_PLA_ATOMIC_CMD_T
{
    DRV_SET_TYPE_T       e_drv_set_type;
    union
    {
        VOID* pv_data;
    } u;
}   VID_PLA_ATOMIC_CMD_T;

/* VID_PLA_SET_TYPE_COLORSPACE ************************************************/
typedef enum
{
    VID_PLA_COLORSPACE_AUTO = 0,
    VID_PLA_COLORSPACE_FORCE_RGB,
    VID_PLA_COLORSPACE_FORCE_YCBCR
}   VID_PLA_COLORSPACE_T;

/* VID_PLA_SET_TYPE_FORCE_UNMUTE************************************************/
typedef enum
{
    VID_PLA_FORCE_UNMUTE_TYPE_ON_NO_SIG = 0,
    VID_PLA_FORCE_UNMUTE_TYPE_ATV_CHG_CH
}   VID_PLA_FORCE_UNMUTE_TYPE_T;

typedef struct _VID_PLA_FORCE_UNMUTE_T
{
    VID_PLA_FORCE_UNMUTE_TYPE_T     e_force_unmute_type;
    BOOL                            b_enable;
}   VID_PLA_FORCE_UNMUTE_T;

#define PHOTO_COLOR_MODE_START  10
typedef enum _CAP_COLOR_FORMAT_T
{
    /*For video*/
    COLOR_FORMAT_8BIT_RGB422 = 0,
    COLOR_FORMAT_8BIT_RGB444,
    COLOR_FORMAT_8BIT_YCbCr422,
    COLOR_FORMAT_8BIT_YCbCr444,
    COLOR_FORMAT_10BIT_RGB422,
    COLOR_FORMAT_10BIT_RGB444,
    COLOR_FORMAT_10BIT_YCbCr422,
    COLOR_FORMAT_10BIT_YCbCr444,
    
    /*For photo*/
    COLOR_FORMAT_AYCbCr_CLUT2  = PHOTO_COLOR_MODE_START,
    COLOR_FORMAT_AYCbCr_CLUT4,
    COLOR_FORMAT_AYCbCr_CLUT8,
    COLOR_FORMAT_CbYCrY_16,
    COLOR_FORMAT_YCbYCr_16,
    COLOR_FORMAT_AYCbCr_D8888,
    COLOR_FORMAT_ARGB_CLUT2,
    COLOR_FORMAT_ARGB_CLUT4,
    COLOR_FORMAT_ARGB_CLUT8,
    COLOR_FORMAT_RGB_D565,
    COLOR_FORMAT_ARGB_D1555,
    COLOR_FORMAT_ARGB_D4444,
    COLOR_FORMAT_ARGB_D8888,
    COLOR_FORMAT_YUV_420_BLK,
    COLOR_FORMAT_YUV_420_RS,
    COLOR_FORMAT_YUV_422_BLK,
    COLOR_FORMAT_YUV_422_RS,
    COLOR_FORMAT_YUV_444_BLK,
    COLOR_FORMAT_YUV_444_RS
}CAP_COLOR_FORMAT_T;

typedef enum _CAP_OUTPUT_PIC_FMT_T
{
    CAP_OUTPUT_PIC_FORMAT_JPEG,
    CAP_OUTPUT_PIC_FORMAT_MPEG
}CAP_OUTPUT_PIC_FMT_T;

typedef struct _CAP_INPUT_T
{
    /*-----------------Common Info------------------------*/
    UINT32              ui4_src_height;         /* Dram frame Height     */
    UINT32              ui4_src_width;          /* Dram frame Width      */
    UINT32              ui4_src_pitch;          /* Dram frame Picth      */
    
    GL_RECT_T           t_rect;                 /* Capture rectangle     */
    UCHAR*              puac_dst_buffer;        /* Dest buffer           */
    UINT32              ui4_max_len;            /* Max encoding length   */
    UINT32              ui4_quality;            /* Output picture quality*/
    
    /*-----------------SD Infor------------------------*/
    UCHAR*              puac_src_buffer;        /* Dram frame start addr */
    UINT32              ui4_buffer_length;      /* Dram frame size       */
    CAP_COLOR_FORMAT_T  e_color_format;         /* Input color format    */
    
    /*-----------------HD Infor------------------------*/
    UINT8               ui1_data_src;           /* 1:B2R, 2:DI, 3:Scaler */
    UINT8               ui1_block_mode;         /* 0:MSB(H:128bits V:16 lines) 1:MSB(H:8*128bits V:8 lines) 
                                                   LSB always(H:4*128bits V:32 lines)*/
    UINT8               ui1_out_mode;           /* 0:Interlace 1:Progressive 2:RGB444*/
    UINT8               ui1_color_mode;         /*0:422 1:420  2:RGB444*/
    UINT8               ui1_bit_res;            /* 8bit(MSB)/10bit(MSB&LSB)*/
    UINT32              ui4_y_start_msb_add;    /* Y MSB Start Address    */
    UINT32              ui4_y_start_lsb_add;    /* Y LSB Start Address    */
    UINT32              ui4_u_start_msb_add;    /* C MSB Start Address    */
    UINT32              ui4_u_start_lsb_add;    /* C LSB Start Address    */
    UINT32              ui4_v_start_msb_add;
    UINT32              ui4_v_start_lsb_add;
    UINT8               ui1_AF_Bottom;
    UINT8               ui1_AF_YUVIndex;

    UINT8               ui1_y_frame_num;
    UINT8               ui1_c_frame_num;

    UINT8               ui1_DramFlipMirror; // 1 : means pscan write dram flip/mirror  ; 0 : means scaler read flip mirror
	UINT8               ui1_CfgFlipMirror;  // 0 : means normal ; 1 : means mirror ; 2 : means flip ; 3 : means flip & mirror

	UINT8				ui1_video_path;//0 main or  1 sub
	UINT8				ui1_pixel_byte;

	BOOL				b_dram_snake_map; // 1 snake, 0 non-snake
} CAP_INPUT_T;

/* VID_PLA_GET_TYPE_3D_NAV / VID_PLA_SET_TYPE_3D_NAV  *************************/
typedef enum
{
  VID_PLA_3D_NAV_OFF = 0,
  VID_PLA_3D_NAV_ON
} VID_PLA_3D_NAV_T;

/* VID_PLA_GET_TYPE_3D_MODE / VID_PLA_SET_TYPE_3D_MODE ***********************/
typedef enum
{
  VID_PLA_3D_MODE_OFF = 0,
  VID_PLA_3D_MODE_AUTO,
  VID_PLA_3D_MODE_2D_TO_3D,
  VID_PLA_3D_MODE_FRM_SEQ,
  VID_PLA_3D_MODE_SIDE_SIDE,
  VID_PLA_3D_MODE_TOP_AND_BTM,
  VID_PLA_3D_MODE_REALD,
  VID_PLA_3D_MODE_SENSIO,
  VID_PLA_3D_MODE_LINE_INTERLEAVE,
  VID_PLA_3D_MODE_DOT_ALT,
  VID_PLA_3D_MODE_CHK_BOARD
} VID_PLA_3D_MODE_T;

/* VID_PLA_GET_TYPE_3D_LR_SWITCH / VID_PLA_SET_TYPE_3D_LR_SWITCH *************/
typedef enum
{
  VID_PLA_3D_LR_SWITCH_OFF = 0,
  VID_PLA_3D_LR_SWITCH_ON
} VID_PLA_3D_LR_SWITCH_T;

/* VID_PLA_GET_TYPE_3D_TO_2D / VID_PLA_SET_TYPE_3D_TO_2D **********************/
typedef enum
{
  VID_PLA_3D_TO_2D_OFF = 0,
  VID_PLA_3D_TO_2D_LEFT,
  VID_PLA_3D_TO_2D_RIGHT
} VID_PLA_3D_TO_2D_T;

/* VID_PLA_GET_TYPE_3D_FPR / VID_PLA_SET_TYPE_3D_FPR *************************/
typedef enum
{
  VID_PLA_3D_FPR_NORMAL = 0,
  VID_PLA_3D_FPR_FPR1,
  VID_PLA_3D_FPR_FPR2
} VID_PLA_3D_FPR_T;

/* VID_PLA_SET_HDMI_RANGE ************************ *************************/
typedef enum
{
  VID_PLA_HDMI_RANGE_AUTO = 0,
  VID_PLA_HDMI_RANGE_LIMIT,
  VID_PLA_HDMI_RANGE_FULL
} VID_PLA_HDMI_RANGE_T;

/* VID_PLA_3D_PANEL_TYPE **************************************************/
typedef enum
{
  VID_PLA_2D_PANEL = 0,
  VID_PLA_3D_PR_PANEL,
  VID_PLA_3D_SG_PANEL
} VID_PLA_3D_PANEL_TYPE_T;

/* angular sce setting */
typedef struct _VID_PLA_ANGULAR_SCE_T
{
    UINT8   ui1_color;
    UINT8   ui1_ysh;
    UINT8   ui1_value;    
} VID_PLA_ANGULAR_SCE_T;

/* 3D nav auto change enable */
typedef enum
{
    VID_PLA_3D_NAV_AUTO_CHG_MANUAL = 0,
    VID_PLA_3D_NAV_AUTO_CHG_SEMI_AUTO,
    VID_PLA_3D_NAV_AUTO_CHG_AUTO
} VID_PLA_3D_NAV_AUTO_CHG_T;

/* 3D image safety enable */
typedef enum
{
    VID_PLA_3D_IMAGE_SAFETY_OFF = 0,
    VID_PLA_3D_IMAGE_SAFETY_LOW,
    VID_PLA_3D_IMAGE_SAFETY_MID,
    VID_PLA_3D_IMAGE_SAFETY_HIGH
} VID_PLA_3D_IMAGE_SAFETY_T;

#endif /* _U_VID_PLANE_H_ */

