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
 * $RCSfile: fbm_drvif.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file fbm_drvif.h
 *  API of FBM (Frame Buffer Manager)
 *  Details of file fbm_drvif.h (optional).
 */

#ifndef FBM_IF_H
#define FBM_IF_H


//#define FBM_INHERITANCE
//#define RACING_MODE

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "stc_drvif.h"
#include "x_lint.h"
#include "x_typedef.h"

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
#define FBM_ALIGN_MASK(value, mask)                                     ((value + mask) & (~mask))

/** Brief of FBM_POOL_TYPE_T
 *  FBM Memory Pool Index
 */
typedef enum
{
    FBM_POOL_TYPE_MPEG,
    FBM_POOL_TYPE_MDDI_MAIN,
    FBM_POOL_TYPE_SCPOS_MAIN,
    FBM_POOL_TYPE_NR,
    FBM_POOL_TYPE_TDC,
    FBM_POOL_TYPE_VBI,
    FBM_POOL_TYPE_DMX,    
    FBM_POOL_TYPE_MDDI_PIP,
    FBM_POOL_TYPE_SCPOS_PIP,
    FBM_POOL_TYPE_SCPOS_VGA,
    FBM_POOL_TYPE_DSP,
    FBM_POOL_TYPE_GFXSC,
    FBM_POOL_TYPE_B2R,
    FBM_POOL_TYPE_JPEG,
    FBM_POOL_TYPE_JPEG_OSD1,
    FBM_POOL_TYPE_JPEG_OSD2,
    FBM_POOL_TYPE_JPEG_OSD3,
    FBM_POOL_TYPE_JPEG_OSD4,
    FBM_POOL_TYPE_FEEDER,
    FBM_POOL_TYPE_RSV_01,
    FBM_POOL_TYPE_RSV_02,
    FBM_POOL_TYPE_RSV_03,
    FBM_POOL_TYPE_RSV_04,
    FBM_POOL_TYPE_TOTAL,
    FBM_POOL_TYPE_NS
} FBM_POOL_TYPE_T;

#define FBM_POOL_MODE_SHARE                                             (1 << 0)
#define FBM_POOL_MODE_8BIT                                              (1 << 1)
#define FBM_POOL_MODE_10BIT                                             (1 << 2)
#define FBM_POOL_MODE_420_FB                                            (1 << 3)
#define FBM_POOL_MODE_422_FB                                            (1 << 4)
#define FBM_POOL_MODE_444_FB                                            (1 << 5)
#define FBM_POOL_MODE_MPEG_DBK                                          (1 << 6)
#define FBM_POOL_MODE_MPEG_3FB_RACING                                   (1 << 7)
#define FBM_POOL_MODE_MPEG_2FB_RACING                                   (1 << 8)
#define FBM_POOL_MODE_1FB                                               (1 << 9)
#define FBM_POOL_MODE_2FB                                               (1 << 10)
#define FBM_POOL_MODE_3FB                                               (1 << 11)
#define FBM_POOL_MODE_4FB                                               (1 << 12)
#define FBM_POOL_MODE_Y_C                                               (1 << 13)	// For NR

#define FBM_TV_MODE_TYPE_NORMAL                                         0
#define FBM_TV_MODE_TYPE_PIP                                            1
#define FBM_TV_MODE_TYPE_POP                                            2


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
} FBM_POOL_MODULE_INFO_T;

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
#define FBM_FBG_TYPE_1080HD_2_FB                                        17      // Half Reference
#define FBM_FBG_TYPE_720HD_2_FB                                         18      // Half Reference
#define FBM_FBG_TYPE_PAL_2_FB                                           19      // Half Reference
#define FBM_FBG_TYPE_RESIDUAL                                           20

// Frame Buffer Alignment
#define FBM_FMG_Y_ALIGMENT                                              (2048 - 1)
#define FBM_FMG_C_ALIGMENT                                              (1024 - 1)
#define FBM_MPV_Y_ALIGMENT                                              (512 - 1)
#define FBM_MPV_C_ALIGMENT                                              (256 - 1)

#define FBM_B2R_H_PITCH                                                 (64-1)
#define FBM_MPV_V_ALIGMENT                                              (16-1)

// Frame Buffer Line Size
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

// Frame Buffer Size
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


// Frame Buffer Group Size
#define FBM_FBG_TYPE_1080HD_DBK_SIZE                                    (((FBM_FBG_TYPE_1080HD_Y_SIZE * (FBM_DBK_FB_NS + 1) * 3) >> 1) + FBM_1080HD_EXTRA_SIZE) 
#define FBM_FBG_TYPE_720HD_DBK_SIZE                                     (((FBM_FBG_TYPE_720HD_Y_SIZE * (FBM_DBK_FB_NS + 2) * 3) >> 1) + FBM_720HD_EXTRA_SIZE) 
#define FBM_FBG_TYPE_PAL_DBK_SIZE                                       (((FBM_FBG_TYPE_PAL_Y_SIZE * (FBM_DBK_FB_NS + 2) * 3) >> 1) + FBM_PAL_EXTRA_SIZE) 
#define FBM_FBG_TYPE_NTSC_DBK_SIZE                                      (((FBM_FBG_TYPE_NTSC_Y_SIZE * (FBM_DBK_FB_NS + 2) * 3) >> 1) + FBM_NTSC_EXTRA_SIZE) 
#define FBM_FBG_TYPE_STD_PAL_DBK_SIZE                                   (((FBM_FBG_TYPE_PAL_Y_SIZE * FBM_DBK_FB_NS * 3) >> 1) + FBM_PAL_EXTRA_SIZE) 
#define FBM_FBG_TYPE_1080HD_H264_SIZE                                    ((FBM_FBG_TYPE_1080HD_Y_SIZE * 42) >> 2) //(4+2)*7/4
#define FBM_FBG_TYPE_720HD_H264_SIZE                                    ((FBM_FBG_TYPE_720HD_Y_SIZE * 77) >> 2) //(9+2)*7/4
#define FBM_FBG_TYPE_PAL_H264_SIZE                                      ((FBM_FBG_TYPE_PAL_Y_SIZE * 126) >> 2) //(16+2)*7/4


#define FBM_FBG_TYPE_1080HD_SIZE                                        (FBM_FBG_TYPE_1080HD_Y_SIZE * 6)
#define FBM_FBG_TYPE_720HD_SIZE                                         (FBM_FBG_TYPE_720HD_Y_SIZE * 6)
#define FBM_FBG_TYPE_PAL_SIZE                                           (FBM_FBG_TYPE_PAL_Y_SIZE * 6)
#define FBM_FBG_TYPE_NTSC_SIZE                                          (FBM_FBG_TYPE_NTSC_Y_SIZE * 6)
#define FBM_FBG_TYPE_1080HD_3FB_SIZE                                    ((FBM_FBG_TYPE_1080HD_Y_SIZE * 9) /2)
#define FBM_FBG_TYPE_720HD_3FB_SIZE                                     ((FBM_FBG_TYPE_720HD_Y_SIZE * 9) /2)
#define FBM_FBG_TYPE_PAL_3FB_SIZE                                       ((FBM_FBG_TYPE_PAL_Y_SIZE * 9) /2)
#define FBM_FBG_TYPE_PAL_2_5FB_SIZE                                     ((FBM_FBG_TYPE_PAL_Y_SIZE * 15) /4)

#define FBM_FBG_TYPE_1080HD_1_5FB_SIZE                                  (FBM_FBG_TYPE_1080HD_3FB_SIZE / 2)
#define FBM_FBG_TYPE_1080HD_2_FB_SIZE                                   (FBM_FBG_TYPE_1080HD_Y_SIZE * 3)

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

// Aspect Ratio
#define MPEG_ASPECT_RATIO_1_1                                        1    // SAR, 1:1
#define MPEG_ASPECT_RATIO_4_3                                        2    // DAR, 4:3
#define MPEG_ASPECT_RATIO_16_9                                       3    // DAR, 16:9
#define MPEG_ASPECT_RATIO_221_1                                      4    // DAR, 2.21:1

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


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

/** Brief of FBM_SEQ_HDR_T
 *  Sequence Header Structure 
 */
typedef struct
{
    UCHAR ucAspRatInf;                              // aspect_ratio_information
    UCHAR ucFrmRatCod;                              // frame_rate_code
    UCHAR ucCrmaFmt;                                // chroma_format;
    UCHAR ucColourPrimaries;                        // BT.709 or BT.601
    UCHAR ucActFmt;                                 //active_format;    

    BOOL fgLoadIntraMatrix;                         // load_intra_quantizer_matrix;
    BOOL fgLoadNonIntraMatrix;                      // load_non_intra_quantizer_matrix;
    BOOL fgProgressiveSeq;                          // progressive_sequence
    BOOL fgClosedGop;
    BOOL fgBrokenLink;
    BOOL fgRatio34;    

    UINT16 u2LineSize;                              // horizontal line size, due to block color mode
    UINT16 u2HSize;                                 // horizontal size value to dispaly
    UINT16 u2VSize;                                 // vertical size value to display
    UINT16 u2OrgHSize;                              // original horizontal size value in Sequence Header
    UINT16 u2OrgVSize;                              // original vertical size value in Sequence Header

    UINT32 u4BitRatVal;                             // bit_rate_value;
    UINT32 u4LastPTS;                               //record the last PTS
} FBM_SEQ_HDR_T;

/** Brief of FBM_PIC_HDR_T
 *  Picture & GOP Header Structure 
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

    BOOL fgTopFldFirst;
    BOOL fgFrmPredFrmDct;
    BOOL fgConcMotionVec; 
    BOOL fgQScaleType; 
    BOOL fgIntraVlcFmt; 
    BOOL fgAltScan;
    BOOL fgRepFirstFld;
    BOOL fgProgressiveFrm;

    UCHAR ucfcode[2][2];

    UINT16 u2TemporalRef;

    UINT16 au2HOffset[FBM_PANSCAN_VECTOR_NS];
    UINT16 au2VOffset[FBM_PANSCAN_VECTOR_NS];

    UINT32 u4PTS;                                   //  90 KHz PTS value, 33/32 bit ? 
    UINT32 u4QpAvg;
} FBM_PIC_HDR_T;

// FBM indication function
typedef void (*FBM_FBG_CHG_IND_FUNC)(UCHAR ucFbgId, UCHAR ucEsId);
typedef void (*FBM_PICSTRUCT_CHG_IND_FUNC)(UCHAR ucEsId, UCHAR ucPicStruct);
typedef void (*FBM_FB_READY_IND_FUNC)(UCHAR ucFbgId);
//typedef void (*FBM_GFX_MEMSET_FUNC)(UINT8 *pu1Addr, UINT32 u4Size, UINT32 u4Color);
typedef void (*FBM_GFX_MEMSET_FUNC)(UINT8 *pu1Addr, UINT32 u4Width, UINT32 u4Height, UINT32 u4Color);
typedef void (*FBM_CB_POOL_CHANGE_FUNC)(FBM_POOL_TYPE_T ePool);

/** Brief of FBM_CB_FUNC_ENUM_T
 */
typedef enum
{
    FBM_CB_FUNC_FBG_CHG_IND,
    FBM_CB_FUNC_PICSTRUCT_CHG_IND,
    FBM_CB_FUNC_FB_READY_IND,
    FBM_CB_FUNC_GFX_MEMSET,
    FBM_CB_FUNC_POOL_CHANGE,
    FBM_CB_FUNC_NS
} FBM_CB_FUNC_ENUM_T;

/** Brief of FBM_CB_FUNC_T
 *  FBM Call Back Function Structure
 */
typedef struct
{
    UINT32 au4CbFunc[FBM_CB_FUNC_NS];
    UINT32 au4CbFuncCRC[FBM_CB_FUNC_NS];
} FBM_CB_FUNC_T;

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

EXTERN void FBM_Init(void);

EXTERN UCHAR FBM_GetFbgNs(void);

EXTERN UCHAR FBM_SelectGroupType(UINT32 u4Width, UINT32 u4Height);

EXTERN UCHAR FBM_CreateGroup(UCHAR ucFbgType, UINT32 u4VDecFmt, UINT32 u4HSize, UINT32 u4VSize);

EXTERN void FBM_ReleaseGroup(UCHAR ucFbgId);

EXTERN void FBM_ResetGroup(UCHAR ucFbgId);

EXTERN void FBM_InvalidateRefFrameBuffer(UCHAR ucFbgId);

EXTERN void FBM_WaitUnlockFrameBuffer(UCHAR ucFbgId, UINT32 u4Timeout);

EXTERN void FBM_ConfigColorMode(UCHAR ucFbgId, UCHAR ucFbgCm, UINT32 u4HSize, UINT32 u4VSize);

EXTERN void FBM_SetFrameBufferColor(UCHAR ucFbgId, UCHAR ucFbId, UINT32 u4Color);

EXTERN void FBM_SetFrameBufferZebraColor(UCHAR ucFbgId, UCHAR ucFbId);

EXTERN void FBM_SetFrameBufferFlag(UCHAR ucFbgId, UINT32 u4Flag);

EXTERN UCHAR FBM_ChkFrameBufferFlag(UCHAR ucFbgId, UINT32 u4Flag);

EXTERN UCHAR FBM_ChkFrameBufferFlagIsr(UCHAR ucFbgId, UINT32 u4Flag);

EXTERN void FBM_ClrFrameBufferFlag(UCHAR ucFbgId, UINT32 u4Flag);

EXTERN UCHAR FBM_GetFrameBufferNs(UCHAR ucFbgId);

EXTERN void FBM_GetFrameBufferSize(UCHAR ucFbgId, UINT32 *pu4Width, UINT32 *pu4Height);

EXTERN UCHAR FBM_GetFrameBufferStatus(UCHAR ucFbgId, UCHAR ucFbId);

EXTERN UCHAR FBM_CheckFrameBufferStatus(UCHAR ucFbgId, UCHAR ucFbId, UCHAR ucFbStatus);

EXTERN void FBM_SetFrameBufferStatus(UCHAR ucFbgId, UCHAR ucFbId, UCHAR ucFbStatus);

EXTERN void FBM_FlushLockFrameBuffer(UCHAR ucFbgId);

EXTERN UCHAR FBM_GetFrameBufferFromDispQ(UCHAR ucFbgId);

EXTERN UCHAR FBM_CheckFrameBufferDispQ(UCHAR ucFbgId);

EXTERN UINT32 FBM_GetNextDispQPts(UCHAR ucFbgId);

EXTERN void FBM_GetFrameBufferAtIsr(UCHAR ucFbgId, UINT32* pu4AddrY, UINT32* pu4AddrC);

EXTERN FBM_PIC_HDR_T *FBM_GetFrameBufferPicHdr(UCHAR ucFbgId, UCHAR ucFbId);

EXTERN FBM_SEQ_HDR_T *FBM_GetFrameBufferSeqHdr(UCHAR ucFbgId);

EXTERN void FBM_GetFrameBufferAddr(UCHAR ucFbgId, UCHAR ucFbId, UINT32 *pu4AddrY, UINT32 *pu4AddrC);

EXTERN void FBM_GetFrameBufferAddrMv(UCHAR ucFbgId, UCHAR ucFbId, UINT32 *pu4AddrMv);

EXTERN void FBM_QueryStatus(void);

EXTERN UCHAR FBM_GetDecFrameBuffer(UCHAR ucFbgId);

EXTERN void FBM_GetRefFrameBuffer(UCHAR ucFbgId, UCHAR* ucFRefFbId, UCHAR* ucBRefFbId);

EXTERN UCHAR FBM_GetEmptyRefFrameBuffer(UCHAR ucFbgId, UINT32 u4Delay);

EXTERN UCHAR FBM_GetEmptyBFrameBuffer(UCHAR ucFbgId, UINT32 u4Delay);

EXTERN UCHAR FBM_GetEmptyFrameBuffer(UCHAR ucFbgId, UINT32 u4Delay);

EXTERN UCHAR FBM_CanGetEmptyRefFrameBuffer(UCHAR ucFbgId);

EXTERN UCHAR FBM_CanGetEmptyBFrameBuffer(UCHAR ucFbgId);

EXTERN void FBM_SetFrameBufferPlayBackNs(UCHAR ucFbgId, UCHAR ucFbId, UCHAR ucPlayBackNs, UCHAR ucChangeFieldNs);

EXTERN UCHAR FBM_GetFrameBufferPlayBackNs(UCHAR ucFbgId, UCHAR ucFbId);

EXTERN UCHAR FBM_GetFrameBufferChangeFieldNs(UCHAR ucFbgId, UCHAR ucFbId);

EXTERN UCHAR FBM_ChkFrameBufferLastPlayBack(UCHAR ucFbgId, UCHAR ucFbId);

// This function is called back by mpeg decode to indicate that
// Frame buffer group ID is change. This function will by pass this notify
// to video plane drive
EXTERN void FBM_FbgChgNotify(UCHAR ucFbgId, UCHAR ucEsId);

EXTERN void FBM_PicStructChgNotify(UCHAR ucEsId, UCHAR ucPicStruct);

EXTERN void FBM_RegCbFunc(FBM_CB_FUNC_ENUM_T eFuncType, UINT32 u4FuncPtr);

EXTERN FBM_CB_FUNC_T* FBM_GetCbFuncTbl(void);

// This function is implemented for mpeg error concealment, it will return FbId in
// display-q state or lock state
EXTERN UCHAR FBM_GetReadyFrameBuffer(UCHAR ucFbgId);

EXTERN void FBM_SetSyncStc(UCHAR ucFbgId, UCHAR ucMode, UCHAR ucStcId);

EXTERN void FBM_GetSyncStc(UCHAR ucFbgId, UCHAR* pucMode, UCHAR* pucStcId);

EXTERN FBM_POOL_T *FBM_GetPoolInfo(UCHAR ucPoolType);

EXTERN FBM_POOL_T* FBM_GetPoolInfoAuto(UCHAR ucPoolType, const FBM_POOL_MODULE_INFO_T* pInfo);

EXTERN FBM_POOL_T* FBM_GetPoolMpeg(void);

EXTERN UCHAR FBM_GetPoolCurrent(UCHAR ucPoolType);

EXTERN void FBM_CheckPadding(void);

EXTERN void FBM_ReleaseDispQ(UCHAR ucFbgId);

#if 0
EXTERN UCHAR FBM_GetDbkSmallBFrameBuffer(UCHAR ucFbgId);

EXTERN void FBM_SetFrameBufferFtrCnt(UCHAR ucFbgId, UCHAR ucFbId, UINT32 u4DbkOfStCnt, UINT32 u4Dbk4pCnt);

EXTERN void FBM_GetFrameBufferFtrCnt(UCHAR ucFbgId, UCHAR ucFbId, UINT32 *pu4DbkOfStCnt, UINT32 *pu4Dbk4pCnt);
#endif

EXTERN UCHAR FBM_GetDbkEmptyRefFrameBuffer(UCHAR ucFbgId, UINT32 u4Delay);

EXTERN INT32 FBM_ResetFbChangeField(UCHAR ucFbgId);

EXTERN INT32 FBM_WaitFbChangeField(UCHAR ucFbgId, UINT32 u4Timeout);

EXTERN void FBM_FbChangeField(UCHAR ucFbgId);

EXTERN void FBM_SetFbNextFieldPlayBackNs(UCHAR ucFbgId, UCHAR ucFbId, UCHAR ucPlayBackNs);

EXTERN UCHAR FBM_GetFbNextFieldPlayBackNs(UCHAR ucFbgId, UCHAR ucFbId);

#ifdef FBM_VIRTUAL_DISPLAY
EXTERN void FBM_VirtualDispStart(UINT32 u4Delay);

EXTERN void FBM_VirtualDispStop(void);
#endif

#ifdef FBM_VIRTUAL_MPEG
EXTERN void FBM_VirtualMpegStart(UINT32 u4Delay);

EXTERN void FBM_VirtualMpegStop(void);
#endif

#ifdef VDP_RACE_LOG
EXTERN void VDP_FrcRacingLog(CHAR cLog);
#endif

#ifdef FBM_FB_LOG
EXTERN void FBM_FlushFbLog(UINT32 u4FbIdx, UINT32 u4FbStatus);
#endif

#endif    // FBM_IF_H

