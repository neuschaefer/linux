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
 * $Author: jifeng.fan $
 * $Date: 2012/11/06 $
 * $RCSfile: fbm_drvif.h,v $
 * $Revision: #3 $

 *
 *---------------------------------------------------------------------------*/

/** @file fbm_drvif.h
 *  API of FBM (Frame Buffer Manager)
 *  Details of file fbm_drvif.h (optional).
 */

#ifndef FBM_DRVIF_H
#define FBM_DRVIF_H


//#define FBM_INHERITANCE
//#define RACING_MODE

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "stc_drvif.h"
#include "x_lint.h"
#include "x_typedef.h"

#if defined(CC_MT5395) || defined(CC_MT5396)
#define CC_FBM_2ND_CHANNEL
#endif

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

// Video Plane Full ISR
// For DTV00007209, Tearing
//#define CC_VDP_FULL_ISR

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

// Frame Buffer Alignment
#define FBM_ALIGN_MASK(value, mask)     ((value + mask) & (~mask))

/** Brief of FBM_POOL_TYPE_T
 *  FBM Memory Pool Index
 */
typedef enum
{
    FBM_POOL_TYPE_DMX,
    FBM_POOL_TYPE_TDC,
    FBM_POOL_TYPE_TDC_2,
    FBM_POOL_TYPE_VBI,
    FBM_POOL_TYPE_VBI_2,
    FBM_POOL_TYPE_MPEG,
    FBM_POOL_TYPE_MJC_1,
    FBM_POOL_TYPE_MPEG_TV,
    FBM_POOL_TYPE_MPEG_NET,
    FBM_POOL_TYPE_SCPOS_MAIN,
    FBM_POOL_TYPE_DMX_TV,
    FBM_POOL_TYPE_DMX_NET,
    FBM_POOL_TYPE_FEEDER_NET,
    FBM_POOL_TYPE_SCPOS_PIP,
    FBM_POOL_TYPE_FEEDER,
    FBM_POOL_TYPE_FEEDER_MMP,
    FBM_POOL_TYPE_MDDI_1,
    FBM_POOL_TYPE_PQ_MM,
    FBM_POOL_TYPE_PQ_NET,
    FBM_POOL_TYPE_PQ_TV,
    FBM_POOL_TYPE_NET_SWAP,
    FBM_POOL_TYPE_TDC_DYN,
    FBM_POOL_TYPE_SCPOS_DYN_MAIN,
    FBM_POOL_TYPE_MDDI_DYN_MAIN,
    FBM_POOL_TYPE_SCPOS_DYN_SUB,
    FBM_POOL_TYPE_MDDI_DYN_SUB,
    FBM_POOL_TYPE_MDDI_PIP,
    FBM_POOL_TYPE_NR_1,
    FBM_POOL_TYPE_DSP,
    FBM_POOL_TYPE_B2R,
    FBM_POOL_TYPE_JPEG,
    FBM_POOL_TYPE_JPEG_OSD1,
    FBM_POOL_TYPE_JPEG_OSD2,
    FBM_POOL_TYPE_JPEG_OSD3,
    FBM_POOL_TYPE_JPEG_OSD4,
    FBM_POOL_TYPE_PQ_TOOL,
    FBM_POOL_TYPE_OD,
    FBM_POOL_TYPE_VENC,
    FBM_POOL_TYPE_SWDMX,
    FBM_POOL_TYPE_FACEDET,
    FBM_POOL_TYPE_MPEG_FREE,
    FBM_POOL_TYPE_DIVA,
    FBM_POOL_TYPE_FEEDER2,
    FBM_POOL_TYPE_DMX2,
    FBM_POOL_TYPE_MPEG2,
    FBM_POOL_TYPE_PVR,
    FBM_POOL_TYPE_PVR2,
    FBM_POOL_TYPE_JPG_VDP,
    FBM_POOL_TYPE_DIVX,
    FBM_POOL_TYPE_MPEG3,
    FBM_POOL_TYPE_VSS,
    FBM_POOL_TYPE_DMXPID,
    FBM_POOL_TYPE_DRM,
    FBM_POOL_TYPE_TOTAL2,
    FBM_POOL_TYPE_TOTAL,
    FBM_POOL_TYPE_NS
} FBM_POOL_TYPE_T;


#define FBM_POOL_TYPE_NR FBM_POOL_TYPE_NR_1
#define FBM_POOL_TYPE_MJC FBM_POOL_TYPE_MJC_1
#define FBM_POOL_TYPE_MDDI_MAIN FBM_POOL_TYPE_MDDI_1

#define FBM_POOL_MODE_8BIT                                              (1 << 0)
#define FBM_POOL_MODE_9BIT                                              (1 << 1)
#define FBM_POOL_MODE_10BIT                                             (1 << 2)
#define FBM_POOL_MODE_C_10BIT                                           (1 << 3)
#define FBM_POOL_MODE_420_FB                                            (1 << 4)
#define FBM_POOL_MODE_422_FB                                            (1 << 5)
#define FBM_POOL_MODE_444_FB                                            (1 << 6)
#define FBM_POOL_MODE_SD                                                (1 << 8)
#define FBM_POOL_MODE_720HD                                             (1 << 9)
#define FBM_POOL_MODE_1080HD                                            (1 << 10)
#define FBM_POOL_MODE_EPG                                               (1 << 11)
#define FBM_POOL_MODE_SHARE                                             (1 << 13)
#define FBM_POOL_MODE_TOGGLE_TRIGGER                                    (1 << 14)
#define FBM_POOL_MODE_UNSTABLE                                          (1 << 15)

#define FBM_POOL_MODE_MPEG_NO_COMP                                      (1 << 16)
#define FBM_POOL_MODE_MPEG_COMP_75                                      (1 << 17)
#define FBM_POOL_MODE_MPEG_COMP_50                                      (1 << 18)
#define FBM_POOL_MODE_MPEG_DBK                                          (1 << 19)

#define FBM_POOL_MODE_MJC_DROP                                          (1 << 16)
#define FBM_POOL_MODE_MJC_NO_DROP                                       (1 << 17)
#define FBM_POOL_MODE_MJC_NO_COMP                                       (1 << 18)
#define FBM_POOL_MODE_MJC_Y_COMP4                                       (1 << 19)
#define FBM_POOL_MODE_MJC_Y_COMP3                                       (1 << 20)

#define FBM_POOL_MODE_NR_3D_Y_C                                         (1 << 16)
#define FBM_POOL_MODE_NR_3D_Y_ONLY                                      (1 << 17)
#define FBM_POOL_MODE_NR_2D                                             (1 << 18)
#define FBM_POOL_MODE_NR_BYPASS                                         (1 << 19)

#define FBM_POOL_MODE_SCPOS_1FB                                         (1 << 16)
#define FBM_POOL_MODE_SCPOS_2FB                                         (1 << 17)
#define FBM_POOL_MODE_SCPOS_3FB                                         (1 << 18)
#define FBM_POOL_MODE_SCPOS_DISP                                        (1 << 19)
#define FBM_POOL_MODE_SCPOS_FULL                                        (1 << 20)
#define FBM_POOL_MODE_SCPOS_PSCAN_DISP                                  (1 << 21)
#define FBM_POOL_MODE_SCPOS_MJC_DISP                                    (1 << 22)
//#define FBM_POOL_MODE_SCPOS_MODE_MASK                                   (0xf << 19)
#define FBM_POOL_MODE_SCPOS_MODE_MASK                                   (0x8f << 19)
#define FBM_POOL_MODE_SCPOS_MM_BUFCHG                                   (1 << 23)
#define FBM_POOL_MODE_SCPOS_BYPASS_PSCANNR                              (1 << 24)
#define FBM_POOL_MODE_SCPOS_PROG_DISP                                   (1 << 25)
#define FBM_POOL_MODE_SCPOS_LINESYNC_DISP                               (1 << 26)

#define FBM_POOL_MODE_MDDI_BOB                                          (1 << 16)
#define FBM_POOL_MODE_MDDI_FULL                                         (1 << 17)
#define FBM_POOL_MODE_MDDI_CBOB                                         (1 << 18)
#define FBM_POOL_MODE_MDDI_MODEMSK                                      (0x7 << 16)
// used in progressive mode
#define FBM_POOL_MODE_MDDI_NR_Y_C                                       (1 << 19)
#define FBM_POOL_MODE_MDDI_NR_Y_ONLY                                    (1 << 20)
#define FBM_POOL_MODE_MDDI_NR_OFF                                       (1 << 21)
#define FBM_POOL_MODE_MDDI_NR_MODEMSK                                   (0x7 << 19)
#define FBM_POOL_MODE_MDDI_NR_10BIT                                     (1 << 22)
#define FBM_POOL_MODE_MDDI_COMP                                         (1 << 23)
#define FBM_POOL_MODE_MDDI_DISP                                         (1 << 24)
#define FBM_POOL_MODE_MDDI_P2I                                          (1 << 25)
#define FBM_POOL_MODE_MDDI_MM_BUFCHG                                    (1 << 26)

// TDC mode
#define FBM_POOL_MODE_TDC_FULL                                          (1 << 16)
#define FBM_POOL_MODE_TDC_GAME                                       (1 << 17)

#define FBM_POOL_MODE_MJC_Y10_C10_422_0RDC_3FRM                         (1 << 16)
#define FBM_POOL_MODE_MJC_Y10_C10_422_1RDC_3FRM                         (1 << 17)
#define FBM_POOL_MODE_MJC_Y10_C10_420_0RDC_3FRM                         (1 << 18)
#define FBM_POOL_MODE_MJC_Y10_C10_420_1RDC_3FRM                         (1 << 19)
#define FBM_POOL_MODE_MJC_Y10_C08_422_0RDC_3FRM                         (1 << 20)
#define FBM_POOL_MODE_MJC_Y10_C08_422_1RDC_3FRM                         (1 << 21)
#define FBM_POOL_MODE_MJC_Y10_C08_420_0RDC_3FRM                         (1 << 22)
#define FBM_POOL_MODE_MJC_Y10_C08_420_1RDC_3FRM                         (1 << 23)
#define FBM_POOL_MODE_MJC_Y08_C08_422_0RDC_3FRM                         (1 << 24)
#define FBM_POOL_MODE_MJC_Y08_C08_422_1RDC_3FRM                         (1 << 25)
#define FBM_POOL_MODE_FRC_Y10_C10_444                                   (1 << 26)
#define FBM_POOL_MODE_FRC_Y08_C08_444                                   (1 << 27)
#define FBM_POOL_MODE_MJC_Y08_C08_420_0RDC_3FRM                         (1 << 28)
#define FBM_POOL_MODE_MJC_Y08_C08_420_1RDC_3FRM                         (1 << 29)
#define FBM_POOL_MODE_MJC_Y08_C08_420_0RDC_1FRM                         (1 << 30)




#define FBM_TV_MODE_TYPE_NORMAL                                         0
#define FBM_TV_MODE_TYPE_PIP                                            1
#define FBM_TV_MODE_TYPE_POP                                            2


#define FBM_POOL_INHERIT                                                1
#define FBM_POOL_ROOT                                                   0

/** Brief of FBM_POOL_T
 *  FBM Memory Pool Info
 */
typedef struct
{
    UINT32 u4Addr;
    UINT32 u4Size;
    UINT32 u4Mode;
    UINT32 u4Width;
    UINT32 u4Height;
    UINT32 u4Inherit;
} FBM_POOL_T;

typedef struct
{
    UINT8 u1AspectRatioType;
    UINT16 u2AspectHorizSize;
    UINT16 u2AspectVertSize;
} FBM_ASPECT_RATIO_T;

/** Brief of FBM_POOL_MODULE_INFO_T
 *  FBM Memory Pool Info
 */
typedef struct
{
    UINT32 u4SourceType;
    UINT32 u4Width;
    UINT32 u4Height;
    UINT32 u4Interlace;
    UINT32 u4ColorFormat444;
    UINT32 u4VgaTiming;
    UINT32 u4PipPop;
    UINT32 u4Resolution;
} FBM_POOL_MODULE_INFO_T;

/**
 *  FBM internal buf
 */
typedef struct
{
    UCHAR ucBuf[254];
    UCHAR ucFbgId;
    UCHAR ucFbId;
} FBM_INTERNAL_BUF_T;

//Pool padding pattern
#define FBM_POOL_PADDING                                                0xAA
#define FBM_POOL_PADDING_SIZE                                           0x8

// Frame Buffer Group Id
#define FBM_FBG_ID_UNKNOWN                                              0xFF

// Frame Buffer Id
#define FBM_FB_ID_UNKNOWN                                               0xFF

// Frame Buffer Group Type
#define FBM_FBG_TYPE_UNKNOWN                                            0xFF
#define FBM_FBG_TYPE_1080HD                                             0
#define FBM_FBG_TYPE_720HD                                              1
#define FBM_FBG_TYPE_PAL                                                2
#define FBM_FBG_TYPE_NTSC                                               3
#define FBM_FBG_TYPE_1080HD_DBK                                         4
#define FBM_FBG_TYPE_720HD_DBK                                          5
#define FBM_FBG_TYPE_PAL_DBK                                            6
#define FBM_FBG_TYPE_NTSC_DBK                                           7
#define FBM_FBG_TYPE_1080HD_3FB                                         8
#define FBM_FBG_TYPE_720HD_3FB                                          9
#define FBM_FBG_TYPE_PAL_3FB                                            10
#define FBM_FBG_TYPE_1080HD_2_5FB                                       11
#define FBM_FBG_TYPE_720HD_2_5FB                                        12
#define FBM_FBG_TYPE_PAL_2_5FB                                          13
#define FBM_FBG_TYPE_1080HD_1_5FB                                       14      // 3FB + Half Reference
#define FBM_FBG_TYPE_720HD_1_5FB                                        15      // 3FB + Half Reference
#define FBM_FBG_TYPE_PAL_1_5FB                                          16      // 3FB + Half Reference
#define FBM_FBG_TYPE_1080HD_RR                                          17      // RR
#define FBM_FBG_TYPE_720HD_RR                                           18      // RR
#define FBM_FBG_TYPE_PAL_RR                                             19      // RR
#define FBM_FBG_TYPE_RESIDUAL                                           20
#define FBM_FBG_TYPE_SBSHD                                              21
#define FBM_FBG_TYPE_TNBHD                                              22
//#define FBM_FBG_TYPE_MHP                                                23
//#define FBM_FBG_TYPE_OMX                                                24

// Frame Buffer Alignment
#define FBM_FMG_Y_ALIGMENT                                              (2048 - 1)
#define FBM_FMG_C_ALIGMENT                                              (1024 - 1)
#define FBM_MPV_Y_ALIGMENT                                              (512 - 1)
#define FBM_MPV_C_ALIGMENT                                              (256 - 1)

#define FBM_B2R_H_PITCH                                                 (64-1)
#define FBM_MPV_V_ALIGMENT                                              (32-1)

// Frame Buffer Line Size
#define FBM_FBG_SBSHD_H                                                3840
#define FBM_FBG_SBSHD_V                                                1088
#define FBM_FBG_TNBHD_H                                                1920
#define FBM_FBG_TNBHD_V                                                2176
#define FBM_FBG_1080HD_V                                                1088
#define FBM_FBG_1080HD_H                                                1920
#define FBM_FBG_1080HD_V                                                1088
#define FBM_FBG_720HD_H                                                 1280
#define FBM_FBG_720HD_V                                                 736
#define FBM_FBG_PAL_H                                                   768
#define FBM_FBG_PAL_V                                                   576
#define FBM_FBG_NTSC_H                                                  768
#define FBM_FBG_NTSC_V                                                  480

#define FBM_RACING_FULLB_FB_NS                                          3
#define FBM_NORMAL_FB_NS                                                4
#define FBM_DBK_FB_NS                                                   5

#define FBM_LOCK_FB_NS                                                  (2)


// H.264 Reference Information
#define FBM_FBG_H264_MVC_FHD_REFERNCE_NS                                (8)     // Reference Frame Number for 1920x1088
#define FBM_FBG_H264_MVC_REFERNCE_MAX_NS                                (32)
#ifdef CC_H264_LV5_SUPPORT
#define FBM_FBG_H264_FHD_REFERNCE_NS                                    (8)     // Reference Frame Number for 1920x1088
#else
#define FBM_FBG_H264_FHD_REFERNCE_NS                                    (4)     // Reference Frame Number for 1920x1088
#endif
#define FBM_FBG_H264_REFERNCE_MAX_NS                                    (16)

// Frame Buffer Size
#define FBM_FBG_TYPE_SBSHD_Y_SIZE                                      FBM_ALIGN_MASK((FBM_FBG_SBSHD_H * FBM_FBG_SBSHD_V), FBM_FMG_Y_ALIGMENT)
#define FBM_FBG_TYPE_SBSHD_C_SIZE                                      (FBM_FBG_TYPE_SBSHD_Y_SIZE >> 1)
#define FBM_FBG_TYPE_TNBHD_Y_SIZE                                      FBM_ALIGN_MASK((FBM_FBG_TNBHD_H * FBM_FBG_TNBHD_V), FBM_FMG_Y_ALIGMENT)
#define FBM_FBG_TYPE_TNBHD_C_SIZE                                      (FBM_FBG_TYPE_TNBHD_Y_SIZE >> 1)
#define FBM_FBG_TYPE_1080HD_Y_SIZE                                      FBM_ALIGN_MASK((FBM_FBG_1080HD_H * FBM_FBG_1080HD_V), FBM_FMG_Y_ALIGMENT)
#define FBM_FBG_TYPE_1080HD_C_SIZE                                      (FBM_FBG_TYPE_1080HD_Y_SIZE >> 1)
#define FBM_FBG_TYPE_720HD_Y_SIZE                                       FBM_ALIGN_MASK((FBM_FBG_720HD_H * FBM_FBG_720HD_V), FBM_FMG_Y_ALIGMENT)
#define FBM_FBG_TYPE_720HD_C_SIZE                                       (FBM_FBG_TYPE_720HD_Y_SIZE >> 1)
#define FBM_FBG_TYPE_PAL_Y_SIZE                                         FBM_ALIGN_MASK((FBM_FBG_PAL_H * FBM_FBG_PAL_V), FBM_FMG_Y_ALIGMENT)
#define FBM_FBG_TYPE_PAL_C_SIZE                                         (FBM_FBG_TYPE_PAL_Y_SIZE >> 1)
#define FBM_FBG_TYPE_NTSC_Y_SIZE                                        FBM_ALIGN_MASK((FBM_FBG_NTSC_H * FBM_FBG_NTSC_V), FBM_FMG_Y_ALIGMENT)
#define FBM_FBG_TYPE_NTSC_C_SIZE                                        (FBM_FBG_TYPE_NTSC_Y_SIZE >> 1)

//Extra 2 MB row buffer for B pic deblocking (field pic need 4 MB row)
#define FBM_TWO_MB_ROW_V                                                64

#define FBM_SBSHD_Y_EXTRA_SIZE                                          FBM_ALIGN_MASK((FBM_FBG_SBSHD_H * FBM_TWO_MB_ROW_V), FBM_MPV_Y_ALIGMENT)
#define FBM_TBNHD_Y_EXTRA_SIZE                                          FBM_ALIGN_MASK((FBM_FBG_TNBHD_H * FBM_TWO_MB_ROW_V), FBM_MPV_Y_ALIGMENT)
#define FBM_1080HD_Y_EXTRA_SIZE                                         FBM_ALIGN_MASK((FBM_FBG_1080HD_H * FBM_TWO_MB_ROW_V), FBM_MPV_Y_ALIGMENT)
#define FBM_720HD_Y_EXTRA_SIZE                                          FBM_ALIGN_MASK((FBM_FBG_720HD_H * FBM_TWO_MB_ROW_V), FBM_MPV_Y_ALIGMENT)
#define FBM_PAL_Y_EXTRA_SIZE                                            FBM_ALIGN_MASK((FBM_FBG_PAL_H * FBM_TWO_MB_ROW_V), FBM_MPV_Y_ALIGMENT)
#define FBM_NTSC_Y_EXTRA_SIZE                                           FBM_ALIGN_MASK((FBM_FBG_NTSC_H * FBM_TWO_MB_ROW_V), FBM_MPV_Y_ALIGMENT)
#define FBM_1080HD_C_EXTRA_SIZE                                         (FBM_ALIGN_MASK((FBM_FBG_1080HD_H * FBM_TWO_MB_ROW_V), FBM_MPV_C_ALIGMENT) >> 1)
#define FBM_720HD_C_EXTRA_SIZE                                          (FBM_ALIGN_MASK((FBM_FBG_720HD_H * FBM_TWO_MB_ROW_V), FBM_MPV_C_ALIGMENT) >> 1)
#define FBM_PAL_C_EXTRA_SIZE                                            (FBM_ALIGN_MASK((FBM_FBG_PAL_H * FBM_TWO_MB_ROW_V), FBM_MPV_C_ALIGMENT) >> 1)
#define FBM_NTSC_C_EXTRA_SIZE                                           (FBM_ALIGN_MASK((FBM_FBG_NTSC_H * FBM_TWO_MB_ROW_V), FBM_MPV_C_ALIGMENT) >> 1)

#define FBM_1080HD_EXTRA_SIZE                                           (FBM_1080HD_Y_EXTRA_SIZE + FBM_1080HD_C_EXTRA_SIZE)
#define FBM_720HD_EXTRA_SIZE                                            (FBM_720HD_Y_EXTRA_SIZE + FBM_720HD_C_EXTRA_SIZE)
#define FBM_PAL_EXTRA_SIZE                                              (FBM_PAL_Y_EXTRA_SIZE + FBM_PAL_C_EXTRA_SIZE)
#define FBM_NTSC_EXTRA_SIZE                                             (FBM_NTSC_Y_EXTRA_SIZE + FBM_NTSC_C_EXTRA_SIZE)

//MPEG4/VC1 working buffer size
#define FBM_WORK_BUF_SIZE                                               (819200)

#define FBM_RESIZE_RPR_BUF_JUMP_SIZE                                    (1024*1024)

// Frame Buffer Group Size
//#define FBM_FBG_TYPE_1080HD_DBK_SIZE                                    (((FBM_FBG_TYPE_1080HD_Y_SIZE * (FBM_DBK_FB_NS + 1) * 3) >> 1) + FBM_1080HD_EXTRA_SIZE)
//#define FBM_FBG_TYPE_720HD_DBK_SIZE                                     (((FBM_FBG_TYPE_720HD_Y_SIZE * (FBM_DBK_FB_NS + 2) * 3) >> 1) + FBM_720HD_EXTRA_SIZE)
//#define FBM_FBG_TYPE_PAL_DBK_SIZE                                       (((FBM_FBG_TYPE_PAL_Y_SIZE * (FBM_DBK_FB_NS + 2) * 3) >> 1) + FBM_PAL_EXTRA_SIZE)
//#define FBM_FBG_TYPE_NTSC_DBK_SIZE                                      (((FBM_FBG_TYPE_NTSC_Y_SIZE * (FBM_DBK_FB_NS + 2) * 3) >> 1) + FBM_NTSC_EXTRA_SIZE)
#define FBM_FBG_TYPE_1080HD_DBK_SIZE                                    (((FBM_FBG_TYPE_1080HD_Y_SIZE * (FBM_DBK_FB_NS + 1) * 3) >> 1) + FBM_WORK_BUF_SIZE)
#define FBM_FBG_TYPE_720HD_DBK_SIZE                                     (((FBM_FBG_TYPE_720HD_Y_SIZE * (FBM_DBK_FB_NS + 2) * 3) >> 1) + FBM_WORK_BUF_SIZE)
#define FBM_FBG_TYPE_PAL_DBK_SIZE                                       (((FBM_FBG_TYPE_PAL_Y_SIZE * (FBM_DBK_FB_NS + 2) * 3) >> 1) + FBM_WORK_BUF_SIZE)
#define FBM_FBG_TYPE_NTSC_DBK_SIZE                                      (((FBM_FBG_TYPE_NTSC_Y_SIZE * (FBM_DBK_FB_NS + 2) * 3) >> 1) + FBM_WORK_BUF_SIZE)
#define FBM_FBG_TYPE_STD_PAL_DBK_SIZE                                   (((FBM_FBG_TYPE_PAL_Y_SIZE * FBM_DBK_FB_NS * 3) >> 1) + FBM_WORK_BUF_SIZE)

#define FBM_H264_REFERENCE_FRAME_SIZE (12*1024*1024)
#define FBM_H264_MV_SIZE (3*1024*1024)

#ifdef CC_H264_LV5_SUPPORT
#define FBM_FBG_TYPE_1080HD_H264_R_NS                                8
#else
#define FBM_FBG_TYPE_1080HD_H264_R_NS                                4
#endif
#define FBM_FBG_TYPE_720HD_H264_R_NS                                 9
#define FBM_FBG_TYPE_PAL_H264_R_NS                                   16




#define FBM_FBG_TYPE_1080HD_H264_B_NS                                   4
#define FBM_FBG_TYPE_1080HD_H264_B_64M_NS                               3

// Size = (R + B) * (Y Size +C Size) + (R + 1) * (Y Size/4)
#define FBM_FBG_TYPE_1080HD_H264_FB_SIZE                                (((FBM_FBG_TYPE_1080HD_Y_SIZE * (FBM_FBG_TYPE_1080HD_H264_R_NS + FBM_FBG_TYPE_1080HD_H264_B_NS))  * 3) >> 1)
#define FBM_FBG_TYPE_1080HD_H264_MV_SIZE                                ((FBM_FBG_TYPE_1080HD_Y_SIZE * (FBM_FBG_TYPE_1080HD_H264_R_NS + 1)) >> 2)
//#define FBM_FBG_TYPE_1080HD_H264_SIZE                                   (FBM_FBG_TYPE_1080HD_H264_FB_SIZE + FBM_FBG_TYPE_1080HD_H264_MV_SIZE)

#define FBM_FBG_TYPE_1080HD_H264_FB_64M_SIZE                            (((FBM_FBG_TYPE_1080HD_Y_SIZE * (FBM_FBG_TYPE_1080HD_H264_R_NS + FBM_FBG_TYPE_1080HD_H264_B_64M_NS))  * 3) >> 1)
#define FBM_FBG_TYPE_1080HD_H264_64M_SIZE                               (FBM_FBG_TYPE_1080HD_H264_FB_64M_SIZE + FBM_FBG_TYPE_1080HD_H264_MV_SIZE)


//#define FBM_FBG_TYPE_PAL_H264_R_NS                                      16
//#define FBM_FBG_TYPE_PAL_H264_B_NS                                      3
// Size = (R + B) * (Y Size +C Size) + (R + 1) * (Y Size/4)
//#define FBM_FBG_TYPE_PAL_H264_FB_SIZE                                   (((FBM_FBG_TYPE_PAL_Y_SIZE * (FBM_FBG_TYPE_PAL_H264_R_NS + FBM_FBG_TYPE_PAL_H264_B_NS)) * 3) >> 1)
//#define FBM_FBG_TYPE_PAL_H264_MV_SIZE                                   ((FBM_FBG_TYPE_PAL_Y_SIZE * (FBM_FBG_TYPE_PAL_H264_R_NS + 1)) >> 2)
//#define FBM_FBG_TYPE_PAL_H264_SIZE                                      (FBM_FBG_TYPE_PAL_H264_FB_SIZE + FBM_FBG_TYPE_PAL_H264_MV_SIZE)


//#define FBM_FBG_TYPE_720HD_H264_R_NS                                      7
//#define FBM_FBG_TYPE_720HD_H264_B_NS                                      3
// Size = (R + B) * (Y Size +C Size) + (R + 1) * (Y Size/4)
//#define FBM_FBG_TYPE_720HD_H264_FB_SIZE                                   (((FBM_FBG_TYPE_720HD_Y_SIZE * (FBM_FBG_TYPE_720HD_H264_R_NS + FBM_FBG_TYPE_720HD_H264_B_NS)) * 3) >> 1)
//#define FBM_FBG_TYPE_720HD_H264_MV_SIZE                                   ((FBM_FBG_TYPE_720HD_Y_SIZE * (FBM_FBG_TYPE_720HD_H264_R_NS + 1)) >> 2)
//#define FBM_FBG_TYPE_720HD_H264_SIZE                                      (FBM_FBG_TYPE_720HD_H264_FB_SIZE + FBM_FBG_TYPE_720HD_H264_MV_SIZE)

// size = (ref frame 12mb) + (decode frame+ two lock frame) + (ref frame + decoder)/4 ---> for motion vector

#if defined(DIVX_PLUS_CER) || defined(CC_NETFLIX_CE3_SUPPORT)
#define FBM_EXTRA_FOR_SEAMLESS (0x600000)
#else
#define FBM_EXTRA_FOR_SEAMLESS (0)
#endif

#define FBM_FBG_TYPE_1080HD_H264_SIZE ((((FBM_FBG_TYPE_1080HD_H264_R_NS+1+FBM_LOCK_FB_NS)*FBM_FBG_TYPE_1080HD_Y_SIZE*3)>>1) + (((FBM_FBG_TYPE_1080HD_H264_R_NS+1)*FBM_FBG_TYPE_1080HD_Y_SIZE) >>2))
#define FBM_FBG_TYPE_720HD_H264_SIZE  ((((FBM_FBG_TYPE_720HD_H264_R_NS+1+FBM_LOCK_FB_NS)*FBM_FBG_TYPE_720HD_Y_SIZE*3)>>1) + (((FBM_FBG_TYPE_720HD_H264_R_NS+1)*FBM_FBG_TYPE_720HD_Y_SIZE) >>2))
#define FBM_FBG_TYPE_PAL_H264_SIZE    ((((FBM_FBG_TYPE_PAL_H264_R_NS+1+FBM_LOCK_FB_NS)*FBM_FBG_TYPE_PAL_Y_SIZE*3)>>1) + (((FBM_FBG_TYPE_PAL_H264_R_NS+1)*FBM_FBG_TYPE_PAL_Y_SIZE) >>2))


#define FBM_FBG_TYPE_1080HD_H264_RR_FB_SIZE                             (FBM_FBG_TYPE_1080HD_H264_FB_SIZE >> 1)
#define FBM_FBG_TYPE_1080HD_H264_RR_SIZE                                   (FBM_FBG_TYPE_1080HD_H264_RR_FB_SIZE + FBM_FBG_TYPE_1080HD_H264_MV_SIZE)

#define FBM_FBG_TYPE_1080HD_H264_RR_FB_64M_SIZE                         (FBM_FBG_TYPE_1080HD_H264_FB_64M_SIZE >> 1)
#define FBM_FBG_TYPE_1080HD_H264_RR_64M_SIZE                            (FBM_FBG_TYPE_1080HD_H264_RR_FB_64M_SIZE + FBM_FBG_TYPE_1080HD_H264_MV_SIZE)

#define FBM_FBG_TYPE_PAL_H264_RR_FB_SIZE                                   (FBM_FBG_TYPE_PAL_H264_FB_SIZE >> 1)
#define FBM_FBG_TYPE_PAL_H264_RR_SIZE                                      (FBM_FBG_TYPE_PAL_H264_RR_FB_SIZE + FBM_FBG_TYPE_PAL_H264_MV_SIZE)


#define FBM_FBG_TYPE_1080HD_SIZE                                        (FBM_FBG_TYPE_1080HD_Y_SIZE * ((FBM_NORMAL_FB_NS*3)>>1))
#define FBM_FBG_TYPE_720HD_SIZE                                         (FBM_FBG_TYPE_720HD_Y_SIZE * ((FBM_NORMAL_FB_NS*3)>>1))
#define FBM_FBG_TYPE_PAL_SIZE                                           (FBM_FBG_TYPE_PAL_Y_SIZE * ((FBM_NORMAL_FB_NS*3)>>1))
#define FBM_FBG_TYPE_NTSC_SIZE                                          (FBM_FBG_TYPE_NTSC_Y_SIZE * ((FBM_NORMAL_FB_NS*3)>>1))
#define FBM_FBG_TYPE_1080HD_3FB_SIZE                                    ((FBM_FBG_TYPE_1080HD_Y_SIZE * 9) /2)
#define FBM_FBG_TYPE_720HD_3FB_SIZE                                     ((FBM_FBG_TYPE_720HD_Y_SIZE * 9) /2)
#define FBM_FBG_TYPE_PAL_3FB_SIZE                                       ((FBM_FBG_TYPE_PAL_Y_SIZE * 9) /2)
#define FBM_FBG_TYPE_PAL_2_5FB_SIZE                                     ((FBM_FBG_TYPE_PAL_Y_SIZE * 15) /4)

#define FBM_FBG_TYPE_1080HD_1_5FB_SIZE                                  (FBM_FBG_TYPE_1080HD_3FB_SIZE / 2)
#define FBM_FBG_TYPE_1080HD_2_FB_SIZE                                   (FBM_FBG_TYPE_1080HD_Y_SIZE * 3)

#define FBM_FBG_TYPE_1080HD_VOMX_ARGB_NS                                (4)
#define FBM_FBG_TYPE_1080HD_VOMX_ARGB_SIZE                              (FBM_FBG_TYPE_1080HD_Y_SIZE * 4) * (FBM_FBG_TYPE_1080HD_VOMX_ARGB_NS + 1)
#define FBM_FBG_TYPE_1080HD_VOMX_NV12_NS                                (4)
#define FBM_FBG_TYPE_1080HD_VOMX_NV12_SIZE                              (FBM_FBG_TYPE_1080HD_Y_SIZE * 3 / 2) * (FBM_FBG_TYPE_1080HD_VOMX_ARGB_NS)

// ES ID
#define FBM_VIN_ES_ID                                                4    // For Video Input Hardware, Mpeg 0~3
#define FBM_DV_ES_ID                                                 5    // For DV Hardware, Mpeg 0~3

// Frame Buffer Group Flag
#define FBM_FLAG_CLR_ALL                                             0xFFFFFFFF // Clear all flag
#define FBM_FLAG_DISP_READY                                          (1 << 0)   // Display Ready, video plane will get FB from queue
#define FBM_FLAG_RESET                                               (1 << 1)   // Reset corresponding FBG
#define FBM_FLAG_SEQ_CHG                                             (1 << 2)   // Sequence Header Change
#define FBM_FLAG_SKIP_PIC                                            (1 << 3)   // Skip one picture
#define FBM_FLAG_DIGEST_MODE                                         (1 << 4)   // Digest Mode, obsolete
#define FBM_FLAG_CLEAR_FRC_PRM                                       (1 << 5)   // After Mpv reset, ask vdp frc to reset prm
#define FBM_FLAG_DEINTERLACE                                         (1 << 6)   // Deinterlace, add one more frame buffer
#define FBM_FLAG_NOT_READY                                           (1 << 7)   // No FB status change in this mode
#define FBM_FLAG_THREE_B                                             (1 << 8)   // 3 B pics between two Ref pics in this sequence
#define FBM_FLAG_DISPLAYING                                          (1 << 9)   // Video plane displaying
#define FBM_FLAG_ES_FIFO_FULL                                        (1 << 10)  // MPEG Elementary Stream's FIFO Full
#define FBM_FLAG_FIX_REF_FB                                          (1 << 11)  // For MPEG Racing Mode, EmptyR Q and EmptyB Q
#define FBM_FLAG_RACING_HALF_B                                       (1 << 12)  // For MPEG Racing Mode, Half B FB, Small DRAM Size
#define FBM_FLAG_RACING_FULL_B                                       (1 << 13)  // For MPEG Racing Mode, Full B FB, Small DRAM Bandwidth
#define FBM_FLAG_FREEZE                                              (1 << 14)  // MPEG should still decode, if no empty fb, get fb from display Q
#define FBM_FLAG_SKIP_PIC_NO_PTS                                     (1 << 15)  // Skip one picture with PTS = 0
#define FBM_FLAG_DEBLOCKING                                          (1 << 16)  // Deblocking mode
#define FBM_FLAG_REDUCE_REF                                          (1 << 17)  // Reduce Reference mode
#define FBM_FLAG_RELEASE_FBG                                         (1 << 18)  // Release FBG
#define FBM_FLAG_SEEK_MODE                                           (1 << 19)  // MM seek mode
#define FBM_FLAG_SUPER_FREEZE                                        (1 << 20)  // middleware super freeze mode
#define FBM_FLAG_3D_SEQ_CHG                                          (1 << 21)  //3D state sequence change notify
#define FBM_FLAG_3D_ERROR_HANDLE_DISABLE                             (1 << 22)  // enable 3D error handling
#define FBM_FLAG_TRACKING_MODE_ENABLE                                (1 << 23)  //enable b2r tracking mode
#define FBM_FLAG_SOURCE_SCRAMBLED                                    (1 << 24)  // Source Scrambled state flag 
#define FBM_FLAG_AFD_CHG_ONLY                                        (1 << 25)  //AFD change only
// Picture Coding Type
#define MPEG_PIC_TYPE_I                                              1
#define MPEG_PIC_TYPE_P                                              2
#define MPEG_PIC_TYPE_B                                              3

// Picture Structure
#define MPEG_PIC_TOP_FIELD                                           1
#define MPEG_PIC_BOTTOM_FIELD                                        2
#define MPEG_PIC_FRAME                                               3
#define MPEG_PIC_FIELD                                               4


// Frame Rate Code
#define MPEG_FRAME_RATE_24_                                          1    // minus
#define MPEG_FRAME_RATE_24                                           2
#define MPEG_FRAME_RATE_25                                           3
#define MPEG_FRAME_RATE_30_                                          4    // minus
#define MPEG_FRAME_RATE_30                                           5
#define MPEG_FRAME_RATE_50                                           6
#define MPEG_FRAME_RATE_60_                                          7    // minus
#define MPEG_FRAME_RATE_60                                           8
#define MPEG_FRAME_RATE_UNKNOWN                                      0xFF

// Aspect Ratio
#define MPEG_ASPECT_RATIO_1_1                                        1    // SAR, 1:1
#define MPEG_ASPECT_RATIO_4_3                                        2    // DAR, 4:3
#define MPEG_ASPECT_RATIO_16_9                                       3    // DAR, 16:9
#define MPEG_ASPECT_RATIO_221_1                                      4    // DAR, 2.21:1

#define MPEG_ASPECT_RATIO_10_11         5 // SAR, 10:11
#define MPEG_ASPECT_RATIO_40_33         6 // SAR, 40:33
#define MPEG_ASPECT_RATIO_16_11         7 // SAR, 16:11
#define MPEG_ASPECT_RATIO_12_11         8 // SAR, 12:11
#define MPEG_ASPECT_RATIO_3_2           9 // SAR 3:2
#define MPEG_ASPECT_RATIO_24_11       10
#define MPEG_ASPECT_RATIO_20_11       11
#define MPEG_ASPECT_RATIO_32_11       12
#define MPEG_ASPECT_RATIO_80_33       13
#define MPEG_ASPECT_RATIO_18_11       14
#define MPEG_ASPECT_RATIO_15_11       15
#define MPEG_ASPECT_RATIO_64_33       16
#define MPEG_ASPECT_RATIO_160_99      17
#define MPEG_ASPECT_RATIO_TRANSMIT    18



// Colour Primary
#define MPEG_COLOR_PRIMARY_709                                       1

// Frame Rate
#define FBM_FRAME_RATE_UNKNOWN                                       0xFF
#define FBM_FRAME_RATE_24                                            24
#define FBM_FRAME_RATE_25                                            25
#define FBM_FRAME_RATE_30                                            30
#define FBM_FRAME_RATE_50                                            50
#define FBM_FRAME_RATE_60                                            60

// Frame Buffer Color Mode
#define FBM_CM_UNKNOWN                                               0xFF
#define FBM_CM_420                                                   0
#define FBM_CM_422                                                   1
#define FBM_CM_444                                                   2
#define FBM_CM_422C                                                  8
#define FBM_CM_YUV24                                                 12
#define FBM_CM_RGB24                                                 13
#define FBM_CM_YUV32                                                 14
#define FBM_CM_RGB32                                                 15
#define FBM_CM_DEFAULT                                               FBM_CM_420

// Frame Buffer Status
#define FBM_FB_STATUS_UNKNOWN                                        0xFF
#define FBM_FB_STATUS_EMPTY                                          0
#define FBM_FB_STATUS_DECODE                                         1
#define FBM_FB_STATUS_READY                                          2
#define FBM_FB_STATUS_DISPLAYQ                                       3
#define FBM_FB_STATUS_LOCK                                           4
#define FBM_FB_STATUS_DIGEST                                         5
#define FBM_FB_STATUS_FIELD_READY                                    6

// Number of Pan Scan Vector
#define FBM_PANSCAN_VECTOR_NS                                        3

//Active format
#define ACT_FMT_DEFAULT                                              0x8

//MT539x Video decoder format
#define FBM_VDEC_UNKNOWN                                             0xFF
#define FBM_VDEC_H264                                                0x1
#define FBM_VDEC_MPEG2                                               0x2
#define FBM_VDEC_MPEG4                                               0x3
#define FBM_VDEC_VC1                                                 0x4
#define FBM_VDEC_JPEG                                                0x5
#define FBM_VDEC_RV                                                  0x6
#define FBM_VDEC_AVS                                                 0x7
#define FBM_VDEC_RAW                                                 0x8
#define FBM_VDEC_VP6                                                 0x9
#define FBM_VDEC_VP8                                                 0xA

//FBM flag for MM
#define FBM_MM_EOS_FLAG                                              (1 << 0)
#define FBM_MM_SYNC_POINT_FLAG                                       (1 << 1)
#define FBM_MM_NOT_DISPLAY_FLAG                                      (1 << 2)
#define FBM_MM_PSEUDO_EOS_FLAG                                       (1 << 3)
#define FBM_MM_3D_CHG_FLAG                                           (1 << 4)
#define FBM_MM_BACKWARD_FLAG                                         (1 << 5) // only valid when EOS flag is on

// FBM Picture Header ucTrueZeroPTS
#define FBM_USE_ZERO_PTS_IF_ZERO    0x1
#define FBM_USE_LAST_PTS_IF_ZERO    0x0

// FBG Play mode
#define FBM_FBG_DTV_MODE        0
#define FBM_FBG_MM_MODE         1
#define FBM_FBG_NOTDISPLAY_MODE 2
#define FBM_FBG_MODE_NS         3

// Application Mode
#define FBM_FBG_APP_NORMAL          (0)
#define FBM_FBG_APP_MHP             (1)
#define FBM_FBG_APP_OMX             (2)
#define FBM_FBG_APP_SKYPE           (3)
#define FBM_FBG_APP_VP6_ALPHA       (4)
#define FBM_FBG_APP_Frame_Pack_3D   (5)
#define FBM_FBG_APP_OMX_DISP        (6)
#define FBM_FBG_APP_MTIMAGE         (7)


// FBG AV sync mode
#define FBM_FBG_AVSYNC_DISABLE        0
#define FBM_FBG_AVSYNC_STC_INVALID    1
#define FBM_FBG_AVSYNC_WAIT_1ST       2
#define FBM_FBG_AVSYNC_RESERVE        3
#define FBM_FBG_AVSYNC_NORMAL         4
#define FBM_FBG_AVSYNC_REPEAT         5
#define FBM_FBG_AVSYNC_DROP           6
#define FBM_FBG_AVSYNC_PTS_JITTER     7
#define FBM_FBG_AVSYNC_NO_PTS         8
#define FBM_FBG_AVSYNC_UNKNOWN        9


/* FBM global flag, not related to FBG index */
#define FBM_FLAG_FB_NO_TIMEOUT        (UINT8)(1 << 0)  // FB wait not timeout
#define FBM_FLAG_FB_DECODE_ONLY       (UINT8)(1 << 1)  //

// FBM resize small picture for scalar h/w limitation
// Scalar only accept width bigger thant 180 pixels
// but actually, it can low to 64 pixel.
// it only fail with 32x32 or lower resolution
#define CC_RESIZE_SMALL_IMAGE

#if defined(CC_MT5395)
#define FBM_IMG_RESIZE_LIMITATION (192)
#elif defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
#define FBM_IMG_RESIZE_LIMITATION (112)
#else
#define FBM_IMG_RESIZE_LIMITATION (192)
#endif
#define FBM_IMG_RESIZE_LIMITATION_MAX (1920)
#define FBM_IMG_RESIZE_NETFLIX_WIDTH (1280)
#define FBM_IMG_RESIZE_NETFLIX_HEIGHT (720)

//B2R 3D ERROR Detectation
#define B2R_3D_ERROR_COUNT (15)
#define B2R_3D_ERROR_RECOVER_COUNT (15)

//B2R 3D Type
#define B2R_3D_NONE (0)
#define B2R_3D_FRAME_SEQUENTIAL (1)
#define B2R_3D_SIDE_BY_SIDE_REAL_D (2)
#define B2R_3D_SIDE_BY_SIDE (3)
#define B2R_3D_TOP_N_BOTTOM (4)

#define B2R_3D_MODE_ALWAYS_L_VIEW (1)
#define B2R_3D_MODE_AUTO_TOGGLE (2)
#define B2R_3D_MODE_MANUAL_SET (3)

//B2R Trick Mode Type
#define B2R_3D_TRICK_MODE_L_VIEW (0)
#define B2R_3D_TRICK_MODE_R_VIEW (1)
#define B2R_3D_TRICK_MODE_3D_VIEW (2)


//B2R force 2d type
#define B2R_FORCE_2D_L_VIEW (0)
#define B2R_FORCE_2D_R_VIEW (1)

//B2R option define
//#define B2R_SD_TEST
#define CC_NETFLIX_SUPPORT
//#define CC_B2R_ENABLE_SEAMLESS_THREAD
//#define CC_B2R_ENALBE_OUTPUT_FRC
//#define CC_B2R_DISABLE_PTS_IIR
//#define CC_B2R_FORCE_UPDATE_STC

//#define CC_B2R_3D_MVC_INTERLACE_SUPPORT
#define CC_B2R_DISABLE_ONE_SECOND_FRC
//#define CC_B2R_GFX_COPY_FIELD
//#define FBM_VIRTUAL_MPEG
//#define CC_B2R_3D_SUPPROT
//#define CC_B2R_3D_ERROR_SUPPORT
//#define VDP_AVS_ADJUST_STC
#define VERIFICATION_REALD  (FALSE)
//#define CC_B2R_ENABLE_CHG_FRAME_MSG

#ifndef __MODEL_slt__
//#ifndef CC_B2R_SKYPE_SUPPORT
#define CC_B2R_WAIT_NPTV_STABLE
//#endif
#endif
#ifdef FBM_VIRTUAL_MPEG
#define B2R_AUTO_TEST
#endif

// Frame Buffer Pool Configuration
#ifdef CC_2HD_SYSTEM
// Maximum frame buffer group number
#define FBG_MAX_NS					10

#elif defined (CC_1HD_1SD_SYSTEM)
// Maximum frame buffer group number
#define FBG_MAX_NS					7

#elif defined (CC_1SD_SYSTEM)
// Maximum frame buffer group number
#define FBG_MAX_NS					3

#else
// Maximum frame buffer group number
#define FBG_MAX_NS					6
#endif

// Decoder puts this ascii into put buffer
#define FBG_INTERNAL_VOMX_BUFFER_ID    "FBG_INTERNAL_VOMX_BUFFER"
#define FBG_INTERNAL_VOMX_BUFFER_SIZE  (20)

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

#ifdef DIVX_PLUS_CER
#ifdef   CC_B2R_CHAPTER_CHG
typedef struct _VDP_CHAPTER_T
{
    UINT64      u8StartPts;
    UINT64      u8EndPts;
    BOOL        fgIsPending;
    BOOL        fgIsSetEndPts;
    BOOL        fgPending;
    BOOL        fgFirstSetChapter;
} VDP_CHAPTER_T;
#endif
#endif

//3D infromation update structure
typedef struct
{
    UINT8 u1B2R3DType;
    UINT8 u1B2RForce2DType;
    BOOL fgB2R3DEnable;
    BOOL fgB2RForce2D;
    UINT32 u4B2R3DWidthOffset;
    UINT32 u4B2R3DHeightOffset;
} FBM_3D_INFO_T;

/** Brief of FBM_SEQ_HDR_T
 *  Sequence & GOP Header Structure
 */
typedef struct
{
    BOOL fgLoadIntraMatrix;                         // load_intra_quantizer_matrix;
    BOOL fgLoadNonIntraMatrix;                      // load_non_intra_quantizer_matrix;
    BOOL fgProgressiveSeq;                          // progressive_sequence
    BOOL fgClosedGop;
    BOOL fgBrokenLink;
    BOOL fgYRatio34;
    BOOL fgCRatio34;
    BOOL fgRRYEn;
    BOOL fgRRCEn;
    BOOL fgNoOverScan;
    BOOL fgColorDescrip;
    BOOL fgNoPts;
    BOOL fgResizeSmallPic;
    BOOL fgRasterOrder;
    BOOL fgB2R3DEnable;
    BOOL fgB2RForce2D;
    BOOL fg422Mode;
    BOOL fgRealDEnhance;                            // RealD enhancement
    BOOL fgPixelAspectRatio;
    BOOL fgSwDecoder;

    UCHAR ucAspRatInf;                              // aspect_ratio_information
    UCHAR ucFrmRatCod;                              // frame_rate_code
    UCHAR ucCrmaFmt;                                // chroma_format;
    UCHAR ucColourPrimaries;
    UCHAR ucTransCharacter;
    UCHAR ucMatrixCoeff;
    UCHAR ucActFmt;                                 //active_format;
    UCHAR ucHours;
    UCHAR ucMinutes;
    UCHAR ucSeconds;
    UCHAR ucPictures;
    UINT8 u1B2R3DType;
    UINT8 u1B2RForce2DType;
    UINT8 u1B2R3DTrickModeType;

    UINT16 u2LineSize;                              // horizontal line size, due to block color mode
    UINT16 u2HSize;                                 // horizontal size value to dispaly
    UINT16 u2VSize;                                 // vertical size value to display
    UINT16 u2DHS;                                 // display_horizontal_size in Sequence display extension
    UINT16 u2DVS;                                 // display_vertical_size in Sequence display extension
    UINT16 u2OrgHSize;                              // original horizontal size value in Sequence Header
    UINT16 u2OrgVSize;                              // original vertical size value in Sequence Header

    UINT32 u4BitRatVal;                             // bit_rate_value;
    UINT32 u4LastPTS;                               //record the last PTS
    UINT32 u4FrmCropTop;
    UINT32 u4FrmCropBtm;
    UINT32 u4AspectRatioWidth;
    UINT32 u4AspectRatioHeight;
    UINT32 u4ResizeWidth;
    UINT32 u4ResizeHeight;
    UINT32 u4YAddrOffset;
    UINT32 u4CAddrOffset;
    UINT32 u4B2R3DWidthOffset;
    UINT32 u4B2R3DHeightOffset;
    //cropping info
    BOOL fgCropping;
    UINT32 u4CropX;
    UINT32 u4CropY;
    UINT32 u4CropWidth;
    UINT32 u4CropHeight;
} FBM_SEQ_HDR_T;

// This structure must be same with VDEC_PIC_EXTRA_INFO_T
/** Brief of FBM_PIC_EXTRA_HDR_T
 *  Picture Header Structure
 */
typedef struct
{
    BOOL fgValid;

    UINT64 u8PTSI;                                  //  I frame pts
    UINT64 u8AudPTS;
    UINT64 u8AudOffset;
} FBM_PIC_EXTRA_INFO_T;

/** Brief of FBM_PIC_HDR_T
 *  Picture Header Structure
 */
typedef struct
{
    UCHAR ucPicCdTp;
    UCHAR ucFullPelFordVec;
    UCHAR ucFordFCode;
    UCHAR ucFullPelBackVec;
    UCHAR ucBackFCode;
    UCHAR ucIntraDcPre;
    UCHAR ucPicStruct;
    UCHAR ucTrueZeroPTS; /* If set to 1, Not use last PTS if current is Zero */

    BOOL fgTopFldFirst;
    BOOL fgFrmPredFrmDct;
    BOOL fgConcMotionVec;
    BOOL fgQScaleType;
    BOOL fgIntraVlcFmt;
    BOOL fgAltScan;
    BOOL fgRepFirstFld;
    BOOL fgProgressiveFrm;

    UCHAR ucfcode[2][2];

    INT32 i4TemporalRef;
    UINT16 u2DecodingOrder;

    UINT16 au2HOffset[FBM_PANSCAN_VECTOR_NS];
    UINT16 au2VOffset[FBM_PANSCAN_VECTOR_NS];

    UINT32 u4PTS;                                   //  90 KHz PTS value, 33/32 bit ?
    UINT32 u4QpAvg;

    UINT32 u4TotlaTimeOffset;
    UINT32 u4TickNum;

    UINT32 u4MmFlag;

    UINT32 u4Timestap;
    UINT64 u8Offset;
    UINT64 u8OffsetDisp;
    UINT64 u8OffsetI;
    //for image resizer (netflix and small resolution)
    BOOL fgSeamlessDisp;
    UINT32 u4PicWidth;
    UINT32 u4PicWidthPitch;
    UINT32 u4PicHeight;
    UINT32 u4AddrOffset;
    FBM_PIC_EXTRA_INFO_T rExtra;
    //for tracking mode
    BOOL fgTrackingMode;
    BOOL fgDummyFrame;
    BOOL fgTrickTopField;
    BOOL fgProgressive2Interlace;
} FBM_PIC_HDR_T;


/** Brief of FBM_PIC_NTF_INFO_T
 *  Picture Notify Infomation Structure
 */
typedef struct
{
    UCHAR   ucFbgId;
    UCHAR   ucFbId;
    BOOL    fgEos;
} FBM_PIC_NTF_INFO_T;

// FBM indication function
typedef VOID (*FBM_FBG_CHG_IND_FUNC)(UCHAR ucFbgId, UCHAR ucEsId);
typedef VOID (*FBM_PICSTRUCT_CHG_IND_FUNC)(UCHAR ucEsId, UCHAR ucPicStruct);
typedef VOID (*FBM_FB_READY_IND_FUNC)(UCHAR ucFbgId);
//typedef VOID (*FBM_GFX_MEMSET_FUNC)(UINT8 *pu1Addr, UINT32 u4Size, UINT32 u4Color);
typedef VOID (*FBM_GFX_MEMSET_FUNC)(UINT8 *pu1Addr, UINT32 u4Width, UINT32 u4Height, UINT32 u4Color);
typedef VOID (*FBM_CB_POOL_CHANGE_FUNC)(FBM_POOL_TYPE_T ePool);
typedef VOID (*FBM_FB_READY_IND_FUNC_EX)(UINT32 u4Tag, FBM_PIC_NTF_INFO_T *prPicNfy);

/** Brief of FBM_CB_FUNC_ENUM_T
 */
typedef enum
{
    FBM_CB_FUNC_FBG_CHG_IND,
    FBM_CB_FUNC_PICSTRUCT_CHG_IND,
    FBM_CB_FUNC_FB_READY_IND,
    FBM_CB_FUNC_GFX_MEMSET,
    FBM_CB_FUNC_POOL_CHANGE,
    FBM_CB_FUNC_FB_READY_EX_IND,
    FBM_CB_FUNC_NS
} FBM_CB_FUNC_ENUM_T;

/** Brief of FBM_CB_FUNC_T
 *  FBM Call Back Function Structure
 */
typedef struct
{
    UINT32 au4CbFunc[FBM_CB_FUNC_NS];
    UINT32 au4CbFuncCRC[FBM_CB_FUNC_NS];
    UINT32 aau4CbFunc[FBG_MAX_NS][FBM_CB_FUNC_NS];
    UINT32 aau4CbFuncCRC[FBG_MAX_NS][FBM_CB_FUNC_NS];
    UINT32 aau4CbFuncTag[FBG_MAX_NS][FBM_CB_FUNC_NS];
} FBM_CB_FUNC_T;

/** Brief of FBM_CREATE_FBG_PAR_T
 *  Create group parameter
 */
typedef struct
{
    UINT8 u1AppMode;
    UINT8 u1VdecId;
} FBM_CREATE_FBG_PAR_T;

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

#define FBM_CHECK_CB_FUNC_VERIFY(func, crc)         ((UINT32)func == ~((UINT32)crc))


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

EXTERN VOID FBM_Init(VOID);

EXTERN UCHAR FBM_GetFbgNs(VOID);

EXTERN UCHAR FBM_SelectGroupType(UINT32 u4Width, UINT32 u4Height);

EXTERN UCHAR FBM_CreateGroup(UCHAR ucFbgType, UINT32 u4VDecFmt, UINT32 u4HSize, UINT32 u4VSize);

EXTERN UCHAR FBM_CreateGroupExt(UCHAR ucFbgType, UINT32 u4VDecFmt,
                                UINT32 u4HSize, UINT32 u4VSize, FBM_CREATE_FBG_PAR_T *prPar);

EXTERN VOID FBM_ReleaseGroup(UCHAR ucFbgId);

EXTERN VOID FBM_ResetGroup(UCHAR ucFbgId);

EXTERN VOID FBM_InvalidateRefFrameBuffer(UCHAR ucFbgId);

EXTERN VOID FBM_WaitUnlockFrameBuffer(UCHAR ucFbgId, UINT32 u4Timeout);

EXTERN VOID FBM_ConfigColorMode(UCHAR ucFbgId, UCHAR ucFbgCm,
                                UINT32 u4HSize, UINT32 u4VSize, FBM_CREATE_FBG_PAR_T *prPar);

EXTERN VOID FBM_SetFrameBufferColor(UCHAR ucFbgId, UCHAR ucFbId, UINT32 u4Color);

EXTERN VOID FBM_SetFrameBufferZebraColor(UCHAR ucFbgId, UCHAR ucFbId);

EXTERN VOID FBM_SetFrameBufferFlag(UCHAR ucFbgId, UINT32 u4Flag);

EXTERN UCHAR FBM_ChkFrameBufferFlag(UCHAR ucFbgId, UINT32 u4Flag);

EXTERN UCHAR FBM_ChkFrameBufferFlagIsr(UCHAR ucFbgId, UINT32 u4Flag);

EXTERN VOID FBM_ClrFrameBufferFlag(UCHAR ucFbgId, UINT32 u4Flag);

EXTERN UCHAR FBM_ChkFrameBufferPicFlag(UCHAR ucFbgId, UCHAR ucFbId, UINT32 u4Flag);

EXTERN VOID FBM_SetFrameBufferPicFlag(UCHAR ucFbgId, UCHAR ucFbId, UINT32 u4Flag);

EXTERN VOID FBM_ClrFrameBufferPicFlag(UCHAR ucFbgId, UCHAR ucFbId, UINT32 u4Flag);

EXTERN UCHAR FBM_GetFrameBufferNs(UCHAR ucFbgId);

EXTERN VOID FBM_GetFrameBufferSize(UCHAR ucFbgId, UINT32 *pu4Width, UINT32 *pu4Height);

EXTERN UCHAR FBM_GetFrameBufferStatus(UCHAR ucFbgId, UCHAR ucFbId);

EXTERN UCHAR FBM_CheckFrameBufferStatus(UCHAR ucFbgId, UCHAR ucFbId, UCHAR ucFbStatus);

EXTERN VOID FBM_SetFrameBufferStatus(UCHAR ucFbgId, UCHAR ucFbId, UCHAR ucFbStatus);

EXTERN VOID FBM_FlushLockFrameBuffer(UCHAR ucFbgId);

EXTERN UCHAR FBM_GetFrameBufferFromDispQ(UCHAR ucFbgId);

EXTERN UCHAR FBM_PreLookFrameBufferFromDispQ(UCHAR ucFbgId);

EXTERN UCHAR FBM_CheckFrameBufferDispQ(UCHAR ucFbgId);

EXTERN UINT32 FBM_GetNextDispQPts(UCHAR ucFbgId);
EXTERN UCHAR  FBM_CheckFbg(UCHAR ucFbgId);

EXTERN UCHAR FBM_CalFrmRate(UCHAR ucFbgId);

EXTERN VOID FBM_GetFrameBufferAtIsr(UCHAR ucFbgId, UINT32* pu4AddrY, UINT32* pu4AddrC);

EXTERN FBM_PIC_HDR_T *FBM_GetFrameBufferPicHdr(UCHAR ucFbgId, UCHAR ucFbId);

EXTERN FBM_SEQ_HDR_T *FBM_GetFrameBufferSeqHdr(UCHAR ucFbgId);

EXTERN VOID FBM_GetFrameBufferAddr(UCHAR ucFbgId, UCHAR ucFbId, UINT32 *pu4AddrY, UINT32 *pu4AddrC);

EXTERN VOID FBM_GetFrameBufferAddrMv(UCHAR ucFbgId, UCHAR ucFbId, UINT32 *pu4AddrMv);

EXTERN void FBM_GetWorkingBuffer(UCHAR ucFbgId, UINT32 *pu4Addr, UINT32 *pu4Size);

EXTERN void FBM_GetExtraFrameBuffer(UCHAR ucFbgId, UINT32* pu4YAddr, UINT32* pu4CAddr);

EXTERN VOID FBM_QueryStatus(VOID);

EXTERN void FBM_UpdateReferenceList(UCHAR ucFbgId, UCHAR ucFbId, BOOL fgReference);

EXTERN UCHAR FBM_GetDecFrameBuffer(UCHAR ucFbgId);

EXTERN VOID FBM_GetRefFrameBuffer(UCHAR ucFbgId, UCHAR* ucFRefFbId, UCHAR* ucBRefFbId);

EXTERN UCHAR FBM_GetEmptyRefFrameBuffer(UCHAR ucFbgId, UINT32 u4Delay);

EXTERN UCHAR FBM_GetEmptyBFrameBuffer(UCHAR ucFbgId, UINT32 u4Delay);

EXTERN UCHAR FBM_GetEmptyFrameBuffer(UCHAR ucFbgId, UINT32 u4Delay);

EXTERN UCHAR FBM_CanGetEmptyRefFrameBuffer(UCHAR ucFbgId);

EXTERN UCHAR FBM_CanGetEmptyBFrameBuffer(UCHAR ucFbgId);

EXTERN VOID FBM_SetFrameBufferPlayBackNs(UCHAR ucFbgId, UCHAR ucFbId, UCHAR ucPlayBackNs, UCHAR ucChangeFieldNs);

EXTERN UCHAR FBM_GetFrameBufferPlayBackNs(UCHAR ucFbgId, UCHAR ucFbId);

EXTERN UCHAR FBM_GetFrameBufferChangeFieldNs(UCHAR ucFbgId, UCHAR ucFbId);

EXTERN UCHAR FBM_ChkFrameBufferLastPlayBack(UCHAR ucFbgId, UCHAR ucFbId);

// This function is called back by mpeg decode to indicate that
// Frame buffer group ID is change. This function will by pass this notify
// to video plane drive
EXTERN VOID FBM_FbgChgNotify(UCHAR ucFbgId, UCHAR ucEsId);

EXTERN VOID FBM_PicStructChgNotify(UCHAR ucEsId, UCHAR ucPicStruct);

EXTERN VOID FBM_RegCbFunc(FBM_CB_FUNC_ENUM_T eFuncType, UINT32 u4FuncPtr);
EXTERN VOID FBM_RegFbgCbFunc(UCHAR ucFbgId, FBM_CB_FUNC_ENUM_T eFuncType, UINT32 u4FuncPtr, UINT32 u4Tag);

EXTERN FBM_CB_FUNC_T* FBM_GetCbFuncTbl(VOID);

// This function is implemented for mpeg error concealment, it will return FbId in
// display-q state or lock state
EXTERN UCHAR FBM_GetReadyFrameBuffer(UCHAR ucFbgId);

EXTERN VOID FBM_SetSyncStc(UCHAR ucFbgId, UCHAR ucMode, UCHAR ucStcId);

EXTERN VOID FBM_GetSyncStc(UCHAR ucFbgId, UCHAR* pucMode, UCHAR* pucStcId);

EXTERN void FBM_SetPlayMode(UCHAR ucFbgId, UCHAR ucPlayMode);
EXTERN void FBM_GetPlayMode(UCHAR ucFbgId, UCHAR* pucPlayMode);

EXTERN UINT8 FBM_GetDecoderSrcId(UCHAR ucFbgId);

EXTERN UINT8 FBM_GetFbgAppMode(UCHAR ucFbgId);


EXTERN void FBM_SetAVSyncStatus(UCHAR ucFbgId, UINT32 u4AVSyncStatus);

EXTERN FBM_POOL_T *FBM_GetPoolInfo(UCHAR ucPoolType);

EXTERN FBM_POOL_T* FBM_GetPoolInfoAuto(UCHAR ucPoolType, const FBM_POOL_MODULE_INFO_T* pInfo);

EXTERN FBM_POOL_T* FBM_GetPoolMpeg(VOID);

EXTERN UCHAR FBM_GetPoolCurrent(UCHAR ucPoolType);

EXTERN VOID FBM_CheckPadding(VOID);

EXTERN VOID FBM_ReleaseDispQ(UCHAR ucFbgId);

EXTERN void FBM_GetEmptyQCount(UCHAR ucFbgId, UCHAR* pucCount);

#if 1
EXTERN UCHAR FBM_GetDbkSmallBFrameBuffer(UCHAR ucFbgId);
#endif

#if 1
EXTERN void FBM_SetFrameBufferFtrCnt(UCHAR ucFbgId, UCHAR ucFbId, UINT32 u4DbkOfStCnt, UINT32 u4Dbk4pCnt);

EXTERN VOID FBM_GetFrameBufferFtrCnt(UCHAR ucFbgId, UCHAR ucFbId, UINT32 *pu4DbkOfStCnt, UINT32 *pu4Dbk4pCnt);
#endif

EXTERN UCHAR FBM_GetDbkEmptyRefFrameBuffer(UCHAR ucFbgId, UINT32 u4Delay);

EXTERN INT32 FBM_ResetFbChangeField(UCHAR ucFbgId);

EXTERN INT32 FBM_WaitFbChangeField(UCHAR ucFbgId, UINT32 u4Timeout);

EXTERN VOID FBM_FbChangeField(UCHAR ucFbgId);

EXTERN VOID FBM_SetFbNextFieldPlayBackNs(UCHAR ucFbgId, UCHAR ucFbId, UCHAR ucPlayBackNs);

EXTERN UCHAR FBM_GetFbNextFieldPlayBackNs(UCHAR ucFbgId, UCHAR ucFbId);

EXTERN void FBM_SetFrameBufferGlobalFlag(UCHAR ucFbgId, UINT8 ucFlag);
EXTERN UCHAR FBM_ChkFrameBufferGlobalFlagIsr(UCHAR ucFbgId, UINT8 ucFlag);
EXTERN void FBM_ClrFrameBufferGlobalFlag(UCHAR ucFbgId, UINT8 ucFlag);

EXTERN UINT32 FBM_GetVDecMemSize(UCHAR ucFbgId);
EXTERN UINT32 FBM_GetRunningVdecMemSize(UCHAR ucFbgId);

EXTERN void FBM_SetColorMode(UINT8 u1ColorMode);
EXTERN void FBM_SetDecoderFB(UINT8 u1DecoderType,UINT8 u1NumOfFB);

#ifdef FBM_VIRTUAL_DISPLAY
EXTERN VOID FBM_VirtualDispStart(UINT32 u4Delay, UINT32 u4VirtualDispPath);

EXTERN VOID FBM_VirtualDispStop(VOID);
#endif

#ifdef FBM_VIRTUAL_MPEG
EXTERN VOID FBM_VirtualInit(UINT32 u4FolderNs,UINT32 u4FrmNs);
EXTERN VOID FBM_VirtualMpegStart(UINT32 u4Delay);
EXTERN VOID FBM_VirtualMpegStop(VOID);
#endif
#ifdef B2R_AUTO_TEST
EXTERN INT32 T32_HostExec(INT32 i4TargetFlag, const CHAR *szFormat, ...);
EXTERN UINT32 FBM_VirtualGetFrmCrc(UINT32 u4Idx);
EXTERN UINT32 FBM_VirtualGetPlayIdx(void);
EXTERN UINT32 FBM_VirtualIsRestart(void);
EXTERN void FBM_VirtualSetPlayIdx(UINT32 u4Idx);
EXTERN UINT32 FBM_VirtualIsActive(void);
EXTERN void FBM_VirtualMpegReStart(void);
#endif

#ifdef VDP_RACE_LOG
EXTERN VOID VDP_FrcRacingLog(CHAR cLog);
#endif

EXTERN void VDP_VsyncMutexLock(void);
EXTERN void VDP_VsyncMutexUnlock(void);

#ifdef CC_B2R_SKYPE_SUPPORT
EXTERN UCHAR VDP_GetPendingFB(UCHAR ucVdpId);
EXTERN void VDP_CleanPendingFB(UCHAR ucVdpId);
#else
EXTERN UCHAR VDP_GetPendingFB(void);
EXTERN void VDP_CleanPendingFB(void);
#endif

#ifdef FBM_FB_LOG
EXTERN VOID FBM_FlushFbLog(UINT32 u4FbIdx, UINT32 u4FbStatus);
#endif

#ifdef FBM_CS_LOG
EXTERN VOID FBM_FlushFbCsLog(void);
#endif

EXTERN void FBM_VdecMemProtect(UINT8 u1Mode, BOOL fgDBKDisable);

EXTERN CHAR* FBM_GetPoolName(UCHAR ucPoolType);
EXTERN UINT32 FBM_GetPoolSize(UCHAR ucPoolType);

/* FBM calculated-once size table - begin */
typedef enum
{
    FBM_TYPE_MAIN_PIP_MODE_1080I,
    FBM_TYPE_MAIN_PIP_MODE_576I,
    FBM_TYPE_MAIN_PIP_MODE_1080P_422,
    FBM_TYPE_MAIN_PIP_MODE_1080P_444,
    FBM_TYPE_SUB_PIP_MODE_1080I,
    FBM_TYPE_SUB_PIP_MODE_576I,
    FBM_TYPE_SUB_PIP_MODE_1080P_422,
    FBM_TYPE_SUB_PIP_MODE_1080P_444,
    FBM_TYPE_MAIN_POP_MODE_1080I,
    FBM_TYPE_MAIN_POP_MODE_576I,
    FBM_TYPE_MAIN_POP_MODE_1080P_422,
    FBM_TYPE_MAIN_POP_MODE_1080P_444,
    FBM_TYPE_SUB_POP_MODE_1080I,
    FBM_TYPE_SUB_POP_MODE_576I,
    FBM_TYPE_SUB_POP_MODE_1080P_422,
    FBM_TYPE_SUB_POP_MODE_1080P_444,
    FBM_TYPE_MM_1080I_MODE,
    FBM_TYPE_MM_576I_MODE,
    FBM_TYPE_MM_1080P_MODE,
    FBM_TYPE_ENV_MAX
} FBM_POOL_ENV_TYPE_T;

typedef enum
{
    FBM_MAIN_PSCAN,
    FBM_MAIN_SCALER,
    FBM_SUB_PSCAN,
    FBM_SUB_SCALER,
    FBM_TDC,
    FBM_VIDEO_AGENT_MAX
} FBM_NPTV_VIDEO_AGNET_T;

EXTERN FBM_POOL_T _FBM_NPTV_VIDEO_CONF[FBM_TYPE_ENV_MAX][FBM_VIDEO_AGENT_MAX];
/* FBM calculated-once size table - end */

EXTERN UINT32 SRMFBM_GetConf(void);
#if defined(CC_DYNAMIC_FBMSRM_CONF)
EXTERN void SRMFBM_SetConf(UINT32 conf);
#endif

#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
typedef enum {
    FLIP_NONE =0,
    FLIP_BY_SCALER = 0x1,
    FLIP_BY_PSCAN  = 0x2,
    FLIP_BY_B2R    = 0x4,
    FLIP_BY_MJC    = 0x8,
    FLIP_BY_3D     = 0x10
} FLIP_MODULE_T;
UINT32 u4QueryFlipModule(UINT32 u4VdpId);
#endif

#if defined(CC_MT5395) || defined(CC_MT5396)
UINT8 FBM_IsAddrInChB(UINT32 u4Addr);
#endif

EXTERN void FBM_SetPscanTestMode(BOOL fgIsTestMode);
EXTERN void SRM_SetCurrentFBMMode(UINT32 u4FBMMode);

#endif    // FBM_DRVIF_H

