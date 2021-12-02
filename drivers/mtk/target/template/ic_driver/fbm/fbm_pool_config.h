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
 * $RCSfile: fbm_pool_config.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file fbm_pool_if.h
 *  API of FBM (Frame Buffer Manager)
 *  Details of file fbm_pool_if.h (optional).
 */

#ifndef FBM_POOL_IF_H
#define FBM_POOL_IF_H

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_typedef.h"
#include "fbm_drvif.h"

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

// -------- MDDI  --------

// MDDI Flag Size Macro
#define FBM_MDDI_FLAG_SIZE(width, height)                               ((((((width + 255) / 256) * height) * 2) * 16) * 3)

// MDDI Size Macro
#define FBM_MDDI_8BIT_420_SIZE(width, height)                           ((((width * height) * 3) >> 1) * 2)
#define FBM_MDDI_8BIT_SIZE(width, height)                               (((width * height) * 2) * 2)
#define FBM_MDDI_10BIT_PLUS_SIZE(width, height)                         (((((((width * 2) + 127) / 128) * height) * 2) * 16) * 2)

// MDDI Pool Size Macro
#define FBM_MDDI_420_POOL_SIZE(align, width, height)                   FBM_ALIGN_MASK((FBM_MDDI_8BIT_420_SIZE(width, height) + FBM_MDDI_FLAG_SIZE(width, height)), align)
#define FBM_MDDI_420_10BIT_POOL_SIZE(align, width, height)             FBM_ALIGN_MASK(((FBM_MDDI_8BIT_420_SIZE(width, height) + FBM_MDDI_10BIT_PLUS_SIZE(width, height)) + FBM_MDDI_FLAG_SIZE(width, height)), align)
#define FBM_MDDI_POOL_SIZE(align, width, height)                       FBM_ALIGN_MASK((FBM_MDDI_8BIT_SIZE(width, height) + FBM_MDDI_FLAG_SIZE(width, height)), align)
#define FBM_MDDI_10BIT_POOL_SIZE(align, width, height)                 FBM_ALIGN_MASK(((FBM_MDDI_8BIT_SIZE(width, height) + FBM_MDDI_10BIT_PLUS_SIZE(width, height)) + FBM_MDDI_FLAG_SIZE(width, height)), align)

// MDDI Alignment
#define FBM_MDDI_ADDR_ALIGMENT                                          (0x2000 - 1)       // 8K
#define FBM_MDDI_SIZE_ALIGMENT                                          0

// MDDI Pool Size
#define FBM_MDDI_420_SD_POOL_SIZE                                       FBM_MDDI_420_POOL_SIZE(FBM_MDDI_SIZE_ALIGMENT, 720, 576)
#define FBM_MDDI_SD_POOL_SIZE                                           FBM_MDDI_POOL_SIZE(FBM_MDDI_SIZE_ALIGMENT, 720, 576)
#define FBM_MDDI_SD_10BIT_POOL_SIZE                                     FBM_MDDI_10BIT_POOL_SIZE(FBM_MDDI_SIZE_ALIGMENT, 720, 576)
#define FBM_MDDI_420_1440PANEL_POOL_SIZE                                FBM_MDDI_420_POOL_SIZE(FBM_MDDI_SIZE_ALIGMENT, 1440, 1080)
#define FBM_MDDI_1440PANEL_POOL_SIZE                                    FBM_MDDI_POOL_SIZE(FBM_MDDI_SIZE_ALIGMENT, 1440, 1080)
#define FBM_MDDI_1440PANEL_10BIT_POOL_SIZE                              FBM_MDDI_10BIT_POOL_SIZE(FBM_MDDI_SIZE_ALIGMENT, 1440, 1080)
#define FBM_MDDI_420_FHDPANEL_POOL_SIZE                                 FBM_MDDI_420_POOL_SIZE(FBM_MDDI_SIZE_ALIGMENT, 1920, 1080)
#define FBM_MDDI_420_FHDPANEL_10BIT_POOL_SIZE                           FBM_MDDI_420_10BIT_POOL_SIZE(FBM_MDDI_SIZE_ALIGMENT, 1920, 1080)
#define FBM_MDDI_FHDPANEL_POOL_SIZE                                     FBM_MDDI_POOL_SIZE(FBM_MDDI_SIZE_ALIGMENT, 1920, 1080)
#define FBM_MDDI_FHDPANEL_10BIT_POOL_SIZE                               FBM_MDDI_10BIT_POOL_SIZE(FBM_MDDI_SIZE_ALIGMENT, 1920, 1080)

#define FBM_MDDI_1440PANEL_WA_POOL_SIZE                                 FBM_MDDI_POOL_SIZE(FBM_MDDI_SIZE_ALIGMENT, 1440, 1082)
#define FBM_MDDI_1440PANEL_WA_10BIT_POOL_SIZE                           FBM_MDDI_10BIT_POOL_SIZE(FBM_MDDI_SIZE_ALIGMENT, 1440, 1082)
#define FBM_MDDI_FHDPANEL_WA_POOL_SIZE                                  FBM_MDDI_POOL_SIZE(FBM_MDDI_SIZE_ALIGMENT, 1920, 1082)
#define FBM_MDDI_FHDPANEL_WA_10BIT_POOL_SIZE                            FBM_MDDI_10BIT_POOL_SIZE(FBM_MDDI_SIZE_ALIGMENT, 1920, 1082)

// -------- SCPOS --------

// SCPOS Total Size Macro
// Work Around by Cosh Hsu, 20070423
#define FBM_SCPOS_SIZE(width, height)                                   ((width + 8) * (height + 2) * 2 * 3 )
#define FBM_SCPOS_10BIT_SIZE(width, height)                             (((((width + 5) / 6) + 1) * 16) * (height + 2) * 3)    // Ceil( WIDTH /6 ) * 16 * HEIGHT
#define FBM_SCPOS_2FB_SIZE(width, height)                               ((width + 8) * (height + 2) * 2 * 2 )
#define FBM_SCPOS_444_3FB_SIZE(width, height)                           ((width + 8) * (height + 2) * 3 * 3 )

// SCPOS Pool Size Macro
#define FBM_SCPOS_POOL_SIZE(align, width, height)                       FBM_ALIGN_MASK(FBM_SCPOS_SIZE(width, height), align)
#define FBM_SCPOS_10BIT_POOL_SIZE(align, width, height)                 FBM_ALIGN_MASK(FBM_SCPOS_10BIT_SIZE(width, height), align)
#define FBM_SCPOS_2FB_POOL_SIZE(align, width, height)                   FBM_ALIGN_MASK(FBM_SCPOS_2FB_SIZE(width, height), align)
#define FBM_SCPOS_444_3FB_POOL_SIZE(align, width, height)               FBM_ALIGN_MASK(FBM_SCPOS_444_3FB_SIZE(width, height), align)

// SCPOS Alignment
#define FBM_SCPOS_ADDR_ALIGMENT                                         (0x2000 - 1)       // 8K
#define FBM_SCPOS_SIZE_ALIGMENT                                         0

// SCPOS Pool Size
#define FBM_SCPOS_SD_2FB_POOL_SIZE                                      FBM_SCPOS_2FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 720, 576)
#define FBM_SCPOS_SD_POOL_SIZE                                          FBM_SCPOS_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 720, 576)
#define FBM_SCPOS_SD_10BIT_POOL_SIZE                                    FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 720, 576)
#define FBM_SCPOS_SD_444_3FB_POOL_SIZE                                  FBM_SCPOS_444_3FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 720, 576)
#define FBM_SCPOS_1440PANEL_2FB_POOL_SIZE                               FBM_SCPOS_2FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1440, 900)
#define FBM_SCPOS_1440PANEL_POOL_SIZE                                   FBM_SCPOS_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1440, 900)
#define FBM_SCPOS_1440PANEL_10BIT_POOL_SIZE                             FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1440, 900)
#define FBM_SCPOS_1440PANEL_444_3FB_POOL_SIZE                           FBM_SCPOS_444_3FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1440, 900)
#define FBM_SCPOS_FHDPANEL_2FB_POOL_SIZE                                FBM_SCPOS_2FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1920, 1080)
#define FBM_SCPOS_FHDPANEL_POOL_SIZE                                    FBM_SCPOS_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1920, 1080)
#define FBM_SCPOS_FHDPANEL_10BIT_POOL_SIZE                              FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1920, 1080)
#define FBM_SCPOS_FHDPANEL_444_3FB_POOL_SIZE                            FBM_SCPOS_444_3FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1920, 1080)

// -------- TDC --------

#define FBM_TDC_ADDR_ALIGMENT                                           (32 - 1)
#define FBM_TDC_SIZE_ALIGMENT                                           (32 - 1)
#define FBM_TDC_SIZE                                                    0x2B4C10 // [10-bit] // 0x251ca0 [9-bit]
#define FBM_TDC_8BIT_SIZE                                               0x251ca0  
#define FBM_TDC_POOL_SIZE                                               FBM_ALIGN_MASK((FBM_TDC_SIZE), FBM_TDC_SIZE_ALIGMENT)
#define FBM_TDC_8BIT_POOL_SIZE                                          FBM_ALIGN_MASK((FBM_TDC_8BIT_SIZE), FBM_TDC_SIZE_ALIGMENT)

// -------- NR --------

#define FBM_NR_ADDR_ALIGMENT                                            (32 - 1)
#define FBM_NR_SIZE_ALIGMENT                                            (32 - 1)
#define FBM_NR_MODE                                                     0
#define FBM_NR_SIZE                                                     0x2A6840              // 2.78M
#define FBM_NR_8BIT_SIZE                                                0x28A3D7              // 2.54M
#define FBM_NR_Y_C_SIZE                                                 (FBM_NR_SIZE * 2)
#define FBM_NR_8BIT_Y_C_SIZE                                            (FBM_NR_8BIT_SIZE * 2)
#define FBM_NR_POOL_SIZE                                                FBM_ALIGN_MASK((FBM_NR_SIZE), FBM_NR_SIZE_ALIGMENT)
#define FBM_NR_8BIT_POOL_SIZE                                           FBM_ALIGN_MASK((FBM_NR_8BIT_SIZE), FBM_NR_SIZE_ALIGMENT)
#define FBM_NR_Y_C_POOL_SIZE                                            FBM_ALIGN_MASK((FBM_NR_Y_C_SIZE), FBM_NR_SIZE_ALIGMENT)
#define FBM_NR_8BIT_Y_C_POOL_SIZE                                       FBM_ALIGN_MASK((FBM_NR_8BIT_Y_C_SIZE), FBM_NR_SIZE_ALIGMENT)

//NR (SD Only)
#define FBM_NR_SD_8BIT_SIZE                                             (800 * 1024)           // 800KB
#define FBM_NR_SD_8BIT_Y_C_SIZE                                         (0xF0000)              // 960KB
#define FBM_NR_SD_8BIT_POOL_SIZE                                        FBM_ALIGN_MASK((FBM_NR_SD_8BIT_SIZE), FBM_NR_SIZE_ALIGMENT)
#define FBM_NR_SD_8BIT_Y_C_POOL_SIZE                                    FBM_ALIGN_MASK((FBM_NR_SD_8BIT_Y_C_SIZE), FBM_NR_SIZE_ALIGMENT)

// -------- VBI --------

#define FBM_VBI_ADDR_ALIGMENT                                           (16 - 1)
#define FBM_VBI_SIZE_ALIGMENT                                           0
#define FBM_VBI_SIZE                                                    (0x17700 + 0xC00 + 0x10)
#define FBM_VBI_POOL_SIZE                                               FBM_ALIGN_MASK((FBM_VBI_SIZE), FBM_VBI_SIZE_ALIGMENT)

// -------- MPEG  --------

#define FBM_MPEG_ADDR_ALIGMENT                                          FBM_FMG_Y_ALIGMENT
#define FBM_MPEG_POOL_PAD                                               (1920 * 32)
#define FBM_MPEG_720P_POOL_PAD                                          (1280 * 32)
#define FBM_MPEG_SD_POOL_PAD                                            (720 * 32)

// -------- DSP  --------

#define FBM_DSP_ADDR_ALIGMENT                                           (256 - 1)
#define FBM_DSP_SIZE_ALIGMENT                                           0
#ifdef __MODEL_slt__
    #define FBM_DSP_SIZE                                                0x180000
#else
    #ifdef CC_DVBT
		#ifndef CC_MT5391_AUD_SUPPORT
	        #define FBM_DSP_SIZE                                             0x8A000
		#else
			#define FBM_DSP_SIZE											 0x9FC00
		#endif
    #else
		#ifndef	CC_MT5391_AUD_SUPPORT
        	#define FBM_DSP_SIZE                                             0x80400
		#else
			#define FBM_DSP_SIZE											 0x96000
		#endif		
	#endif
#endif
#define FBM_DSP_POOL_SIZE                                               FBM_ALIGN_MASK((FBM_DSP_SIZE), FBM_DSP_SIZE_ALIGMENT)

// -------- DMX  --------

#define FBM_DMX_ADDR_ALIGMENT                                           (64 - 1)
#define FBM_DMX_SIZE_ALIGMENT                                           (64 - 1)
#define FBM_DMX_SIZE                                                    0x300000
#define FBM_DMX_5381_FHD_SIZE                                           0x280000
#define FBM_DMX_5371_SIZE                                               0x180000
#define FBM_DMX_5380_16MB_SIZE                                          0x100000
#define FBM_DMX_DVBT_SIZE                                               0xC0000
#define FBM_DMX_POOL_SIZE                                               FBM_ALIGN_MASK((FBM_DMX_SIZE), FBM_DMX_SIZE_ALIGMENT)

// -------- GFX  --------
#define FBM_GFXSC_ADDR_ALIGMENT                                         (16 - 1)
#define FBM_GFXSC_SIZE_ALIGMENT                                         0
#define FBM_GFXSC_SIZE                                                  0x180000

// -------- B2R  --------
#define FBM_B2R_ADDR_ALIGMENT                                           (64 - 1)
#define FBM_B2R_SIZE_ALIGMENT                                           0
#define FBM_B2R_POOL_SIZE(width, height, ns, bpp)                       FBM_ALIGN_MASK(((((width * height) * bpp) + (FBM_B2R_ADDR_ALIGMENT + 1)) * ns), FBM_B2R_SIZE_ALIGMENT)
#define FBM_B2R_420_POOL_SIZE(width, height, ns)                        FBM_ALIGN_MASK((((((width * height) * 3) / 2) + (FBM_B2R_ADDR_ALIGMENT + 1)) * ns), FBM_B2R_SIZE_ALIGMENT)

// -------- JPEG  --------
#ifdef __KERNEL__ 
#define FBM_JPEG_ADDR_ALIGMENT                                          (PAGE_SIZE - 1)
#define FBM_JPEG_SIZE_ALIGMENT                                          (PAGE_SIZE - 1)
#else
#define FBM_JPEG_ADDR_ALIGMENT                                          (256 - 1)
#define FBM_JPEG_SIZE_ALIGMENT                                          0
#endif
#define FBM_JPEG_SIZE                                                   0x200000

// -------- OSD  --------
#ifdef __KERNEL__ 
#define FBM_OSD_ADDR_ALIGMENT                                          (PAGE_SIZE - 1)
#define FBM_OSD_SIZE_ALIGMENT                                          (PAGE_SIZE - 1)
#else
#define FBM_OSD_ADDR_ALIGMENT                                          (16 - 1)
#define FBM_OSD_SIZE_ALIGMENT                                          0
#endif

#define FBM_OSD_POOL_SIZE(width, height, ns, bpp)                      FBM_ALIGN_MASK(((((width * height) * bpp) + (FBM_OSD_ADDR_ALIGMENT + 1)) * ns), FBM_OSD_SIZE_ALIGMENT)

// -------- Feeder  --------

#ifdef __KERNEL__ 
#define FBM_FEEDER_ADDR_ALIGMENT                                       (PAGE_SIZE - 1)
#define FBM_FEEDER_SIZE_ALIGMENT                                       (PAGE_SIZE - 1)
#else
#define FBM_FEEDER_ADDR_ALIGMENT                                       (16 - 1)
#define FBM_FEEDER_SIZE_ALIGMENT                                       0
#endif

#define FBM_FEEDER_SIZE                                                0x100000


#endif    // FBM_POOL_IF_H

