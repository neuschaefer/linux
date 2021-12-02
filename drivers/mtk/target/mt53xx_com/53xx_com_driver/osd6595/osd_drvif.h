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
 * $RCSfile: osd_drvif.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file osd_drvif.h
 *  This header file declares public function prototypes of OSD.
 */

#ifndef OSD_DRVIF_H
#define OSD_DRVIF_H

/*lint -e717 -e572*/

/*
e717 : do ... while(0)
e572 : Excessive shift value (precision Integer shifted right by Integer)
*/

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_typedef.h"
#include "x_assert.h"
#include "x_common.h"
#include "x_os.h"
#include "x_printf.h"
#ifdef DRV_SUPPORT_EXTMJC
#include "extmjc_if.h"
#endif
LINT_EXT_HEADER_END


//#define CURSOR_SUPPORT
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
//#define CC_MT5391_FPGA
//#define CC_MTK_FPGA

#define OSD_DMA_ALIGN_SIZE             63  // 64 byte alignment

#define OSD_BASE_REG                   OSD_BASE

// skip update and reset register
#define OSD_BASE_SKIP                  2
#define OSD_BASE_REG_NUM               18
#define OSD_BASE_SKIP_START            11
#define OSD_BASE_SKIP_END              16

#define OSD_PLA1_REG                   (OSD_BASE_REG + 0x100)
#define OSD_PLA1_RGN_REG               (OSD_PLA1_REG + 0x40)
#define OSD_PLA2_REG                   (OSD_BASE_REG + 0x200)
#define OSD_PLA3_REG                   (OSD_BASE_REG + 0x300)
#define OSD_PLA_REG_NUM                7


#define OSD_SC2_REG                    (OSD_BASE_REG + 0x400)
#define OSD_SC3_REG                    (OSD_BASE_REG + 0x500)
#define OSD_SC_REG_NUM                 9
#define OSD_SC_STEP_BIT                14
#define OSD_SC_STEP_BASE               0x4000

#define OSD_LPF_PARAM_NUM              3
#define OSD_DEFAULT_LPF_C1             -7
#define OSD_DEFAULT_LPF_C2             28
#define OSD_DEFAULT_LPF_C3             88
#define OSD_DEFAULT_LPF_C4             0
#define OSD_DEFAULT_LPF_C5             0

#define OSD_CSR_REG                    (OSD_BASE_REG + 0x700)
#define OSD_CSR_DATA_REG               (OSD_BASE_REG + 0x800)
#define OSD_CSR_DATA_SIZE              0x100
#define OSD_CSR_COLOR_REG              (OSD_CSR_REG + 0x4)

#define OSD_CSR_REG_NUM                1

#define OSD_RGN_REG_NUM                8

#ifdef CC_MINI_DRIVER
    #define OSD_MAX_NUM_RGN            10
    #define OSD_MAX_NUM_RGN_LIST       10
#else
    #define OSD_MAX_NUM_RGN            120
    #define OSD_MAX_NUM_RGN_LIST       90
#endif
#define OSD_CK_REG                     IO_ADDR(0xd220)
#define OSD_CKEN_REG                   IO_ADDR(0xd2a8)
#define OSD_CK_XTAL                    0
#define OSD_CK_SYS                     1 // 162 or 216Mhz
//#define OSD_CK_TVD                     2 //tvd:180Mhz
#define OSD_CK_TVD                     1 //tvd:180Mhz
#define OSD_CK_OCLK                    6 //vopll
//4=dram clock,5=cpu clock

#define OSD_FMT_08                     (OSD_BASE_REG + 0x8)
    #define YUV_OUTPUT                 (1 << 9)

#define OSD_FIELD_TOP                  1
#define OSD_FIELD_BOT                  0
#define OSD_FRAME_MODE                 2

#define OSD_INT_ENABLE_REG             (OSD_BASE_REG + 0x30)
    #define OSD1_CFUD_IEN              (1 << 8)
    #define OSD2_CFUD_IEN              (1 << 7)
    #define OSD2_PFUD_IEN              (1 << 6)
    #define OSD2_VUTO_IEN              (1 << 5)
    #define OSD2_VDTO_IEN              (1 << 4)
    #define OSD3_CFUD_IEN              (1 << 3)
    #define OSD3_PFUD_IEN              (1 << 2)
    #define OSD3_VUTO_IEN              (1 << 1)
    #define OSD3_VDTO_IEN              (1 << 0)
#define OSD_INT_STATUS_REG             (OSD_BASE_REG + 0x34)
    #define OSD_INT                    (1 << 16)
    #define OSD1_CFUD_IID              (1 << 8)
    #define OSD2_CFUD_IID              (1 << 7)
    #define OSD2_PFUD_IID              (1 << 6)
    #define OSD2_VUTO_IID              (1 << 5)
    #define OSD2_VDTO_IID              (1 << 4)
    #define OSD3_CFUD_IID              (1 << 3)
    #define OSD3_PFUD_IID              (1 << 2)
    #define OSD3_VUTO_IID              (1 << 1)
    #define OSD3_VDTO_IID              (1 << 0)

#define OSD_CHECKSUM                   (OSD_BASE_REG + 0x50)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


/** OSD plane enum.
 */
typedef enum
{
    OSD_PLANE_1,
    OSD_PLANE_2,
    //#ifdef CC_MT5363
    OSD_PLANE_3,
    //#endif
    OSD_PLANE_MAX_NUM
} OSD_PLANE_T;

#define OSD_PLANE_NONE OSD_PLANE_MAX_NUM


/** OSD scaler enum.
 */
typedef enum
{
    OSD_SCALER_2,
    //#ifdef CC_MT5363
    OSD_SCALER_3,
    //#endif
    OSD_SCALER_MAX_NUM
    
} OSD_SCALER_T;


/** OSD function return code.
 */
typedef enum
{
    OSD_RET_OK,
    OSD_RET_INV_ARG,
    OSD_RET_OUT_OF_MEM,
    OSD_RET_OUT_OF_REGION,
    OSD_RET_OUT_OF_LIST,
    OSD_RET_UNINIT,
    OSD_RET_INV_PLANE,
    OSD_RET_INV_SCALER,
    OSD_RET_INV_REGION,
    OSD_RET_INV_BITMAP,
    OSD_RET_INV_LIST,
    OSD_RET_INV_DISPLAY_MODE,
    OSD_RET_NO_ATTACH_REGION,
    OSD_RET_REGION_COLLISION,
    OSD_RET_ERR_INTERNAL,
    OSD_RET_UNDEF_ERR
} OSD_RET_CODE_T;

/** OSD color mode.
 */
typedef enum
{
    OSD_CM_YCBCR_CLUT2,
    OSD_CM_YCBCR_CLUT4,
    OSD_CM_YCBCR_CLUT8,
    OSD_CM_RESERVED_0,
    OSD_CM_CBYCRY422_DIRECT16,
    OSD_CM_YCBYCR422_DIRECT16,
    OSD_CM_AYCBCR8888_DIRECT32,
    OSD_CM_RESERVED_1,
    OSD_CM_RGB_CLUT2,
    OSD_CM_RGB_CLUT4,
    OSD_CM_RGB_CLUT8,
    OSD_CM_RGB565_DIRECT16,
    OSD_CM_ARGB1555_DIRECT16,
    OSD_CM_ARGB4444_DIRECT16,
    OSD_CM_ARGB8888_DIRECT32,
    OSD_CM_ALPHA_BIT
} OSD_COLOR_MODE_T;

/** OSD display mode. for FPGA emulation
 */
typedef enum
{
    OSD_DM_480I,
    OSD_DM_576I,
    OSD_DM_480P,
    OSD_DM_576P,
    OSD_DM_720P,
    OSD_DM_768P,
    OSD_DM_1080I,
    OSD_DM_1080P,
    OSD_DM_MAX_NUM
} OSD_DISPLAY_MODE_T;

/** OSD blending mode.
 */
typedef enum
{
    OSD_BM_NONE,
    OSD_BM_PIXEL,
    OSD_BM_REGION,
    OSD_BM_PLANE
} OSD_BLEND_MODE_T;

/** OSD region list cmd.
 */
typedef enum
{
    OSD_RGN_LIST_HEAD,
    OSD_RGN_LIST_TAIL,
    OSD_RGN_LIST_COUNT,
    OSD_LIST_FLAGS
} OSD_RGN_LIST_CMD_T;

/** OSD region cmd.
 */
typedef enum
{
    OSD_RGN_ALLOC,
    OSD_RGN_PREV,
    OSD_RGN_NEXT,
    OSD_RGN_FLAGS,
    OSD_RGN_POS_X,
    OSD_RGN_POS_Y,
    OSD_RGN_BMP_W,
    OSD_RGN_BMP_H,
    OSD_RGN_DISP_W,
    OSD_RGN_DISP_H,
    OSD_RGN_OUT_W,
    OSD_RGN_OUT_H,
    OSD_RGN_COLORMODE,
    OSD_RGN_ALPHA,
    OSD_RGN_BMP_ADDR,
    OSD_RGN_BMP_PITCH,
    OSD_RGN_CLIP_V,
    OSD_RGN_CLIP_H,
    OSD_RGN_V_FLIP,
    OSD_RGN_H_MIRROR,
    OSD_RGN_PAL_LOAD,
    OSD_RGN_PAL_ADDR,
    OSD_RGN_PAL_LEN,
    OSD_RGN_STEP_V,
    OSD_RGN_STEP_H,
    OSD_RGN_COLOR_KEY,
    OSD_RGN_COLOR_KEY_EN,
    OSD_RGN_MIX_SEL,
    OSD_RGN_BIG_ENDIAN,
    OSD_RGN_ALPHA_SEL,
    OSD_RGN_YR_SEL,
    OSD_RGN_UG_SEL,
    OSD_RGN_VB_SEL,
    OSD_RGN_NEXT_EN,
    OSD_RGN_NEXT_HDR_ADDR,
    OSD_RGN_FIFO_EX,
    OSD_RGN_HPX_EXT,
    OSD_RGN_ENDIAN,
    OSD_RGN_HINTP_MODE,
    OSD_RGN_VINTP_MODE,
    OSD_RGN_HSTEP_OFFSET,
    OSD_RGN_VSTEP_OFFSET,
    OSD_RGN_DEC_MODE,
    OSD_RGN_DEC_LINE,
    OSD_RGN_DEC_EN
} OSD_RGN_CMD_T;



/** OSD base register bit-field type.
 */
 //#ifdef CC_MT5363
 #if 1
 typedef struct _OSD_BASE_FIELD_T
{
    /* DWORD - 000 */
    UINT32          fgUpdate            :   1;
    UINT32          fgAlwaysUpdate      :   1;
    UINT32          fgForceCLKEn      :   1;
    UINT32          fgTveUpdate      :   1;
    UINT32          fgOSD3FifoSel      :   1;
    UINT32                              :   27;

    /* DWORD - 004 */
    //UINT32          fgResetFsm          :   1;
    UINT32          fgResetAll          :   2;
    UINT32                              :   30;

    /* DWORD - 008 */
    UINT32          fgHsEdge            :   1;
    UINT32          fgVsEdge            :   1;
    UINT32          fgFldPol            :   1;
    UINT32          fgPrgs              :   1;
    UINT32          fgExtTGen           :   1;
    UINT32          fgIntTGen           :   1;
    UINT32          fgChecksumEn        :   1;
    UINT32          fgOSD12Ex              :   1;
    UINT32          fgAutoPos           :   1;
    UINT32          fgYuvOut            :   1;
    UINT32                :   2;
    UINT32          u4Sc2ChkSumSel      :   2;
    UINT32          fgHSyncSrcSel       :   1;
    UINT32          fgVSyncSrcSel       :   1;
    UINT32          fgTGenMethod        :   1;
    UINT32          fgCMPREn        :   1;
    UINT32          fgTVEEn        :   1;
    UINT32          fgYCC709En        :   1;
    UINT32          fgAlphaBackEn        :   1;
    UINT32                              :   11;

    /* DWORD - 00C */
    UINT32          u4Ovt               :   11;
    UINT32                              :   5;
    UINT32          u4Oht               :   12;
    UINT32                              :   4;

    /* DWORD - 010 */
    UINT32          u4VsWidth           :   8;
    UINT32          u4HsWidth           :   8;
    UINT32          u4ScrnHStartOsd1    :   13;
    UINT32          u4DelayAdj          :   3;

    /* DWORD - 014 */
    UINT32          u4ScrnHStartOsd3    :   13;
    UINT32                              :   3;
    UINT32          u4ScrnHStartOsd2    :   13;
    UINT32                              :   3;

    /* DWORD - 018 */
    UINT32          u4ScrnVStartBot     :   11;
    UINT32                              :   5;
    UINT32          u4ScrnVStartTop     :   8;
    UINT32          fgAutoPosFld        :   1;
    UINT32                              :   7;

    /* DWORD - 01C */
    UINT32          u4ScrnVSize         :   11;
    UINT32                              :   5;
    UINT32          u4ScrnHSize         :   11;
    UINT32                              :   5;

    /* DWORD - 020 */
    UINT32          u4Osd1VStart        :   11;
    UINT32                              :   5;
    UINT32          u4Osd1HStart        :   11;
    UINT32                              :   5;

    /* DWORD - 024 */
    UINT32          u4Osd2VStart        :   11;
    UINT32                              :   5;
    UINT32          u4Osd2HStart        :   11;
    UINT32                              :   5;

    /* DWORD - 028 */
    UINT32          u4Osd3VStart        :   11;
    UINT32                              :   5;
    UINT32          u4Osd3HStart        :   11;
    UINT32                              :   5;

     /* DWORD - 02C */
    UINT32                 :   32;
     /* DWORD - 030 */     
    UINT32                 :   32;
     /* DWORD - 034 */          
    UINT32                 :   32;
     /* DWORD - 038 */          
    UINT32                 :   32;
     /* DWORD - 03C */          
    UINT32                 :   32;
     /* DWORD - 040 */          
    UINT32                 :   32;
     /* DWORD - 044 */          
    UINT32           u4LineShiftEn    :   1;
    UINT32           u4LineShiftINV    :   1;     
    UINT32           u4PlaneSwitchEn    :   1;     
    UINT32           u4PlaneSwitchInv    :   1;     
    UINT32          u4AutoLR  :   1;
    UINT32 		:1;
    UINT32          u4LineDup  :   1;   //3D line double
    UINT32 		:1;
    UINT32          u4Plane2ShiftValue :8;
    UINT32          u4Plane3ShiftValue :8;
    UINT32                 :   7;
    UINT32          u4LRFlag   :   1; 
    
LINT_SUPPRESS_NEXT_EXPRESSION(950)
} __attribute__ ((packed)) OSD_BASE_FIELD_T;

#else

typedef struct _OSD_BASE_FIELD_T
{
    /* DWORD - 000 */
    UINT32          fgUpdate            :   1;
    UINT32          fgAlwaysUpdate      :   1;
    UINT32          fgForceCLKEn      :   1;
    UINT32                              :   29;

    /* DWORD - 004 */
    //UINT32          fgResetFsm          :   1;
    UINT32          fgResetAll          :   2;
    UINT32                              :   30;

    /* DWORD - 008 */
    UINT32          fgHsEdge            :   1;
    UINT32          fgVsEdge            :   1;
    UINT32          fgFldPol            :   1;
    UINT32          fgPrgs              :   1;
    UINT32          fgExtTGen           :   1;
    UINT32          fgIntTGen           :   1;
    UINT32          fgChecksumEn        :   1;
    UINT32          fgOSD12Ex              :   1;
    UINT32          fgAutoPos           :   1;
    UINT32          fgYuvOut            :   1;
    UINT32                :   2;
    UINT32          u4Sc2ChkSumSel      :   2;
    UINT32          fgHSyncSrcSel       :   1;
    UINT32          fgVSyncSrcSel       :   1;
    UINT32          fgTGenMethod        :   1;
    UINT32          fgCMPREn        :   1;
    UINT32          fgTVEEn        :   1;
    UINT32          fgYCC709En        :   1;
    UINT32          fgAlphaBackEn        :   1;
    UINT32                              :   11;

    /* DWORD - 00C */
    UINT32          u4Ovt               :   11;
    UINT32                              :   5;
    UINT32          u4Oht               :   12;
    UINT32                              :   4;

    /* DWORD - 010 */
    UINT32          u4VsWidth           :   8;
    UINT32          u4HsWidth           :   8;
    UINT32          u4ScrnHStartOsd1    :   13;
    UINT32          u4DelayAdj          :   3;

    /* DWORD - 014 */
    UINT32              :   16;
    UINT32          u4ScrnHStartOsd2    :   13;
    UINT32                              :   3;

    /* DWORD - 018 */
    UINT32          u4ScrnVStartBot     :   11;
    UINT32                              :   5;
    UINT32          u4ScrnVStartTop     :   8;
    UINT32          fgAutoPosFld        :   1;
    UINT32                              :   7;

    /* DWORD - 01C */
    UINT32          u4ScrnVSize         :   11;
    UINT32                              :   5;
    UINT32          u4ScrnHSize         :   11;
    UINT32                              :   5;

    /* DWORD - 020 */
    UINT32          u4Osd1VStart        :   11;
    UINT32                              :   5;
    UINT32          u4Osd1HStart        :   11;
    UINT32                              :   5;

    /* DWORD - 024 */
    UINT32          u4Osd2VStart        :   11;
    UINT32                              :   5;
    UINT32          u4Osd2HStart        :   11;
    UINT32                              :   5;

    /* DWORD - 028 */
    UINT32                 :   32;

     /* DWORD - 02C */
    //UINT32                 :   32;
    
LINT_SUPPRESS_NEXT_EXPRESSION(950)
} __attribute__ ((packed)) OSD_BASE_FIELD_T;
 #endif
 
/** OSD plane register bit-field type.
 */
typedef struct _OSD_PLA_FIELD_T
{
    /* DWORD - 000 */
    UINT32          fgEn             :   1;
    UINT32                              :   1;
    UINT32          fgFakeHdr   :   1;
    UINT32          fgPrngEn      :   1;
    UINT32                              :   1;
    UINT32                              :   1;
    UINT32                              :   1;
    UINT32          fgPrngMode :   1;
    UINT32          fgAEdgeDetEn :   1;
    UINT32          fgCLKeySel  :   1;
    UINT32                              :   3;
    UINT32          fgShutter     :   1;
    UINT32                              :   18;    

    /* DWORD - 004 */
    UINT32          u4HeaderAddr        :   25;	//5363 address 25bits
    UINT32                              :   7;

    /* DWORD - 008 */
    UINT32          u4Blending            :   8;
    UINT32          u4Fading               :   8;
    UINT32          fgHFilter                :   1;
    UINT32          fgColor_Exp_sel    :   1;
    UINT32                                        :   14;

    /* DWORD - 00c */
    UINT32          u4ContReqLmt        :   4;
    UINT32          u4FifoSize              :   4;
    UINT32          u4VacTh                 :   4;
    UINT32          u4PauseCnt            :   4;
    UINT32          u4ContReqLmt0        :   4;
    UINT32          fgBurstDis        :   1;
    UINT32                  :   1;
    UINT32                  :   1;
    UINT32                  :   1;
    UINT32          LarbReqEn        :   1;
    UINT32                  :   1;
    UINT32                  :   1;
    UINT32                  :   1;
    UINT32          LarbTim        :   4;

    /* DWORD - 010 */
    UINT32                 :   32;
    /* DWORD - 014 */     
    UINT32                 :   32;
    /* DWORD - 018 */          
    UINT32         u4RightHeaderAddr  :   25;
    UINT32                 :   7;    
    
LINT_SUPPRESS_NEXT_EXPRESSION(950)
} __attribute__ ((packed)) OSD_PLA_FIELD_T;

/** OSD scaler register bit-field type.
 */
typedef struct _OSD_SC_FIELD_T
{
    /* DWORD - 000 */
    UINT32                              :   2;
    UINT32          fgVuscEn            :   1;
    UINT32                    :   1;
    UINT32          fgHuscEn            :   1;
    UINT32                     :   1;
    UINT32                     :   1;
    UINT32          fgScEn              :   1;
    UINT32          fgScAEdgeEn    :   1;
    UINT32                              :   23;

    /* DWORD - 004 */
    UINT32          u4SrcVSize          :   11;
    UINT32                              :   5;
    UINT32          u4SrcHSize          :   11;
    UINT32                              :   5;

    /* DWORD - 008 */
    UINT32          u4DstVSize          :   11;
    UINT32                              :   5;
    UINT32          u4DstHSize          :   11;
    UINT32                              :   5;

    /* DWORD - 00C */
    UINT32          u4VscHSize          :   11;
    UINT32                              :   21;

    /* DWORD - 010 */
    UINT32                  :   32;

    /* DWORD - 014 */
    UINT32          u4HuscStep          :   14;
    UINT32                              :   2;
    UINT32          u4HuscOfst          :   14;
    UINT32                              :   2;

    /* DWORD - 018 */
    UINT32          u4VscOfstBot        :   14;
    UINT32                              :   2;
    UINT32          u4VscOfstTop        :   14;
    UINT32                              :   2;

    /* DWORD - 01C */
    UINT32          u4VscStep           :   14;
    UINT32                              :   18;
LINT_SUPPRESS_NEXT_EXPRESSION(950)
} __attribute__ ((packed)) OSD_SC_FIELD_T;

/** OSD region register bit-field type.
 */
typedef struct _OSD_RGN_FIELD_T
{
    /* DWORD - 000 */

    UINT32          u4NextOsdAddr       :   25;
    UINT32                              :   1;
    UINT32          fgFifoEx            :   1;
    UINT32          fgNextOsdEn         :   1;
    UINT32          u4ColorMode         :   4;

    /* DWORD - 004 */
    UINT32          u4DataAddr          :   24;
    UINT32          u4MixWeight         :   8;

    /* DWORD - 008 */
    UINT32          u4HClip             :   12;
    UINT32          u4VClip             :   11;
    UINT32          fgLineSize9         :   1;
    UINT32          u4VbSel             :   2;
    UINT32          u4UgSel             :   2;
    UINT32          u4YrSel             :   2;
    UINT32          u4AlphaSel          :   2;

    /* DWORD - 00C */
    UINT32          u4PaletteAddr       :   24;
    UINT32          fgDecMode          :   2;
    UINT32          u4PaletteLen        :   2;
    UINT32          fgNewPalette        :   1;
    UINT32          fgDecLine           :   1;
    UINT32          fgHMirror           :   1;
    UINT32          fgVFlip             :   1;

    /* DWORD - 010 */
    UINT32          u4Ihw               :   12;
    UINT32          u4Ivw               :   11;
    UINT32          u4LineSize          :   9;

    /* DWORD - 014 */
    UINT32          u4HStep             :   16;
    UINT32          u4VStep             :   16;

    /* DWORD - 018 */
    UINT32          u4Ovw               :   11;
    UINT32          fgDecEn                    :   1;
    UINT32          u4Ovs               :   11;
    UINT32          fgColorKeyEn        :   1;
    UINT32          u4ColorKey          :   8;

    /* DWORD - 01C */
    UINT32          u4Ohw               :   11;
    UINT32                              :   1;
    UINT32          u4Ohs               :   11;
    //UINT32                              :   4;
    UINT32                              :   2;
    UINT32          data_adr28         :   1;
     UINT32         pal_adr28          :   1;
    UINT32          fgAcsFrame          :   1;
    UINT32          fgAcsAuto           :   1;
    UINT32          fgAcsTop            :   1;
    UINT32          u4MixSel            :   2;
LINT_SUPPRESS_NEXT_EXPRESSION(950)
} __attribute__ ((packed)) OSD_RGN_FIELD_T;


/** OSD cursor register bit-field type.
 */
#ifdef CURSOR_SUPPORT
typedef struct _OSD_CSR_FIELD_T
{
    /* DWORD - 000 */
    UINT32          u4VStart            :   11;
    UINT32                              :   1;
    UINT32          u4HStart            :   11;
    UINT32                              :   1;

    UINT32          fgV2x               :   1;
    UINT32          fgH2x               :   1;
    UINT32          fgEn                :   1;
    UINT32                              :   1;
    UINT32          fgH4Bit             :   1;
    UINT32          fgV4Bit             :   1;
    UINT32                              :   2;
LINT_SUPPRESS_NEXT_EXPRESSION(950)
} __attribute__ ((packed)) OSD_CSR_FIELD_T;
#endif

/** OSD base register union type.
 */
/*lint -save -e960 */
typedef union _OSD_BASE_UNION_T
{
    UINT32             au4Reg[OSD_BASE_REG_NUM];
    OSD_BASE_FIELD_T   rField;
} OSD_BASE_UNION_T;
/*lint -restore */

/** OSD plane register union type.
 */
/*lint -save -e960 */
typedef union _OSD_PLA_UNION_T
{
    UINT32             au4Reg[OSD_PLA_REG_NUM];
    OSD_PLA_FIELD_T    rField;
} OSD_PLA_UNION_T;
/*lint -restore */
/* 3D related register */

/*lint -restore */

/** OSD plane scaler union type.
 */
/*lint -save -e960 */
typedef union _OSD_SC_UNION_T
{
    UINT32             au4Reg[OSD_SC_REG_NUM];
    OSD_SC_FIELD_T     rField;
} OSD_SC_UNION_T;
/*lint -restore */

/** OSD region register union type.
 */
/*lint -save -e960 */
typedef union _OSD_RGN_UNION_T
{
    UINT32             au4Reg[OSD_RGN_REG_NUM];
    OSD_RGN_FIELD_T    rField;
} OSD_RGN_UNION_T;
/*lint -restore */

/** OSD cursor register union type.
 */
/*lint -save -e960 */
#ifdef CURSOR_SUPPORT
typedef union _OSD_CSR_UNION_T
{
    UINT32            au4Reg[OSD_CSR_REG_NUM];
    OSD_CSR_FIELD_T   rField;
} OSD_CSR_UNION_T;
#endif
/*lint -restore */

// this struct is uesd to create palette
typedef struct _OSD_ARGB_T
{
    UINT8 u1Alpha;
    UINT8 u1Red;
    UINT8 u1Green;
    UINT8 u1Blue;
} OSD_ARGB_T;


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define PARAM(IDX) (UINT32)StrToInt(szArgv[IDX+1])

#define OSD_DMA_ALIGN(X) (((X) + OSD_DMA_ALIGN_SIZE) & (~OSD_DMA_ALIGN_SIZE))
#define OSD_PALETTE_SIZE 1024

#define OSD_CHECK_NULL(X) \
    do \
    { \
        if ((X) == NULL) \
        { \
            return -(INT32)OSD_RET_INV_ARG; \
        } \
    } while (0)

#define OSD_VERIFY_PLANE(X) \
    do \
    { \
        if ((UINT32)(X) >= (UINT32)OSD_PLANE_MAX_NUM) \
        { \
            return -(INT32)OSD_RET_INV_PLANE; \
        } \
    } while (0)

#define OSD_VERIFY_SCALER(X) \
    do \
    { \
        if ((UINT32)(X) >= (UINT32)OSD_SCALER_MAX_NUM) \
        { \
            return -(INT32)OSD_RET_INV_SCALER; \
        } \
    } while (0)

#define OSD_VERIFY_REGION(X) \
    do \
    { \
        if ((UINT32)(X) >= (UINT32)OSD_MAX_NUM_RGN) \
        { \
            return -(INT32)OSD_RET_INV_REGION; \
        } \
    } while (0)

#define OSD_VERIFY_RGNLIST(X) \
    do \
    { \
        if ((UINT32)(X) >= (UINT32)OSD_MAX_NUM_RGN_LIST) \
        { \
            return -(INT32)OSD_RET_INV_LIST; \
        } \
    } while (0)

#define OSD_VERIFY_DISPLAY_MODE(X) \
    do \
    { \
        if ((UINT32)(X) >= (UINT32)OSD_DM_MAX_NUM) \
        { \
            return -(INT32)OSD_RET_INV_DISPLAY_MODE; \
        } \
    } while (0)

#define OSD_PLANE_TO_SCALER(X) ((X)-1)
#define OSD_SCALER_TO_PLANE(X) ((X)+1)

#define IGNORE_RET(X) \
    do \
    { \
        INT32 i4Ignore; \
        i4Ignore = (INT32)(X); \
        UNUSED(i4Ignore); \
    } while (0)

#define OSD_GET_LPF_SIGN_NUM(NUM, SIGN, DST) \
    do { \
        if ((NUM) > (SIGN)) \
        { \
            (DST) = (SIGN) - (NUM); \
        } \
        else \
        { \
            (DST) = (NUM); \
        } \
    } while (0)

#define OSD_SET_LPF_SIGN_NUM(NUM, SIGN, SRC) \
    do { \
        if (SRC < 0) \
        { \
            (NUM) = (UINT16)((SIGN) - SRC); \
        } \
        else \
        { \
            (NUM) = (UINT16)SRC; \
        } \
        if ((NUM) >= ((SIGN) << 1)) \
        { \
            return -(INT32)OSD_RET_INV_ARG; \
        } \
    } while (0)

#define OSD_BYTE_PER_PIXEL(CM, BPP) \
    do \
    { \
        switch ((CM)) \
        { \
        case OSD_CM_CBYCRY422_DIRECT16: \
        case OSD_CM_YCBYCR422_DIRECT16: \
        case OSD_CM_RGB565_DIRECT16: \
        case OSD_CM_ARGB1555_DIRECT16: \
        case OSD_CM_ARGB4444_DIRECT16: \
            (BPP) = 2; \
            break; \
        case OSD_CM_AYCBCR8888_DIRECT32: \
        case OSD_CM_ARGB8888_DIRECT32: \
            (BPP) = 4; \
            break; \
        default: \
            (BPP) = 1; \
        } \
    } while (0)

#define OSD_GET_PITCH_SIZE(CM, WIDTH, SIZE) \
    do \
    { \
        switch ((CM)) \
        { \
        case OSD_CM_CBYCRY422_DIRECT16: \
        case OSD_CM_YCBYCR422_DIRECT16: \
        case OSD_CM_RGB565_DIRECT16: \
        case OSD_CM_ARGB1555_DIRECT16: \
        case OSD_CM_ARGB4444_DIRECT16: \
            (SIZE) = (WIDTH) * 2; \
            break; \
        case OSD_CM_AYCBCR8888_DIRECT32: \
        case OSD_CM_ARGB8888_DIRECT32: \
            (SIZE) = (WIDTH) * 4; \
            break; \
        case OSD_CM_YCBCR_CLUT8: \
        case OSD_CM_RGB_CLUT8: \
            (SIZE) = (WIDTH); \
            break; \
        case OSD_CM_YCBCR_CLUT4: \
        case OSD_CM_RGB_CLUT4: \
            (SIZE) = (WIDTH) >> 1; \
            break; \
        case OSD_CM_YCBCR_CLUT2: \
        case OSD_CM_RGB_CLUT2: \
            (SIZE) = (WIDTH) >> 2; \
            break; \
        default: \
            ASSERT(0); \
        } \
    } while (0)


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

/** osd global initialization function
 *  @param  void
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_Init(void);

EXTERN INT32 OSD_Reset(void);

EXTERN INT32 OSD_UpdateTiming(void);
EXTERN void _OSD_AlwaysUpdateReg(BOOL fgEnable);

EXTERN void OSD_WaitVsync(void);
EXTERN void OSD_UnlockVsyncSema(void);

/** set whether osd1/osd2 is top plane
 *  @param  u4TopPlane OSD_PLANE_1/OSD_PLANE_2/OSD_PLANE_NONE(video plane is on top)
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_BASE_SetTopPlane(UINT32 u4TopPlane);

EXTERN INT32 OSD_BASE_SetDisplayMode(UINT32 u4DisplayMode);

/** set osd plane position
 *  @param  u4Plane plane id
 *  @param  u4X x position
 *  @param  u4Y y position
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_BASE_SetOsdPosition(UINT32 u4Plane, UINT32 u4X, UINT32 u4Y);

/** get osd plane position
 *  @param  u4Plane plane id
 *  @param  pu4X x position pointer
 *  @param  pu4Y y position pointer
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_BASE_GetOsdPosition(UINT32 u4Plane, UINT32* pu4X,
                                     UINT32* pu4Y);
                                     
EXTERN INT32 OSD_BASE_SetClock(UINT32 u4Clock);
EXTERN INT32 OSD_BASE_SetYuvOutput(BOOL fgYuvOutput);
EXTERN void OSD_BASE_Dump(void);

EXTERN INT32 OSD_BASE_SetLineShift(UINT32 u4Enable, UINT32 u4Shift );


EXTERN INT32 OSD_SetTVEOut(BOOL fgEnable);
EXTERN INT32 OSD_SetPowerDwn(BOOL fgEnable);
EXTERN INT32 OSD_ShowLogo(void);

EXTERN INT32 OSD_PLA_Init(UINT32 u4Plane);
EXTERN INT32 OSD_PLA_Reset(UINT32 u4Plane);

/** enable/disable osd plane, it will also disable scaler when disable plane
 *  @param  u4Plane plane id
 *  @param  fgEnable enable=TRUE/disable=FALSE
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_PLA_Enable(UINT32 u4Plane, BOOL fgEnable);

/** attach a specific list to osd plane
 *  @param  u4Plane plane id
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_PLA_FlipTo(UINT32 u4Plane, UINT32 u4RgnList);

EXTERN INT32 OSD_PLA_FlipToNone(UINT32 u4Plane);
EXTERN INT32 OSD_PLA_Reflip(UINT32 u4RgnList);

/** get osd plane alpha level
 *  @param  u4Plane plane id
 *  @param  pu1BlendLevel aplha level pointer
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_PLA_GetBlendLevel(UINT32 u4Plane, UINT8* pu1BlendLevel);

/** set osd plane alpha level
 *  @param  u4Plane plane id
 *  @param  u1BlendLevel aplha level(0~255)
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_PLA_SetBlendLevel(UINT32 u4Plane, UINT8 u1BlendLevel);

/** get osd plane fading value
 *  @param  u4Plane plane id
 *  @param  pu1Fading fading value pointer
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_PLA_GetFading(UINT32 u4Plane, UINT8* pu1Fading);

/** set osd plane fading value
 *  @param  u4Plane plane id
 *  @param  u1Fading fading value
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_PLA_SetFading(UINT32 u4Plane, UINT8 u1Fading);

EXTERN INT32 OSD_PLA_SetHFilter(UINT32 u4Plane, BOOL fgEnable);
EXTERN INT32 OSD_PLA_SetVFilter(UINT32 u4Plane, BOOL fgEnable);
EXTERN INT32 OSD_PLA_SetFifo(UINT32 u4Plane, BOOL fgFastReq, UINT8 u1ExVacThr,
                             UINT8 u1VacThr, UINT8 u1FullThr);
EXTERN INT32 OSD_PLA_SetGlobeEnable(UINT32 u4Plane, BOOL fgGlobeEnable);
EXTERN void OSD_PLA_Mute(void);
EXTERN void OSD_PLA_Unmute(void);
EXTERN void OSD_PLA_SetWorkaround(UINT32 u4Plane, BOOL fgEnable);

/** dump osd debug info
 *  @param  u4Plane osd plane
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_PLA_Dump(UINT32 u4Plane);

/** get the first region id of osd plane
 *  @param  u4Plane osd plane
 *  @retval region id, -1 is no region
 */
EXTERN INT32 OSD_PLA_GetFirstRegion(UINT32 u4Plane);

EXTERN INT32 OSD_PLA_GetFirstRegionList(UINT32 u4Plane);
EXTERN INT32 OSD_PLA_GetEnable(UINT32 u4Plane, BOOL* pfgEnable);

/** get the first region address of osd plane
 *  @param  u4Plane osd plane
 *  @retval region address, 0 is no region
 */
EXTERN UINT32 OSD_PLA_GetFirstRegionAddress(UINT32 u4Plane);

EXTERN void OSD_PLA_SetCompressMode(UINT32 u4Plane, UINT32 u4DecEnable, UINT32 u4AlphaMode);
EXTERN void OSD_PLA_GetCompressMode(UINT32 u4Plane, UINT32 *pu4DecEnable, UINT32 *pu4AlphaMode);
//#ifdef CC_MT5363
//EXTERN INT32 OSD_PLA_Swap(BOOL fgSwap);
//#endif

#if 0
EXTERN INT32 OSD_PLA_Set3DAddress(UINT32 u4Plane, UINT32 u4L_Address, UINT32 u4R_Address);
EXTERN INT32 OSD_Vsync_3DUpdateLR(UINT32 u4L_R);
#endif
EXTERN INT32 OSD_Set3DMode(UINT8 u1Mode);

EXTERN INT32 OSD_RGN_LIST_Set(UINT32 u4RgnList, INT32 i4Cmd, UINT32 u4Value);
EXTERN INT32 OSD_RGN_LIST_Get(UINT32 u4RgnList, INT32 i4Cmd, UINT32 * pu4Value);

/** create an osd list
 *  @param pu4RgnList list id pointer
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_RGN_LIST_Create(UINT32 * pu4RgnList);

/** delete an osd list
 *  @param  u4RgnList list id
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_RGN_LIST_Delete(UINT32 u4RgnList);

/** get first region id of list
 *  @param  u4RgnList list id
 *  @param  pi4HeadRegion region id pointer
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_RGN_LIST_GetHead(UINT32 u4RgnList, INT32 * phHeadRegion);

EXTERN INT32 OSD_RGN_LIST_GetNext(UINT32 u4RgnList, INT32 hRgnCurr,
                                  INT32 *phRgnNext);
EXTERN INT32 OSD_RGN_LIST_DetachAll(UINT32 u4RgnList);
EXTERN INT32 OSD_RGN_LIST_GetCount(UINT32 u4RgnList, INT32 * pi4Count);


EXTERN INT32 OSD_RGN_LIST_Init(void);
EXTERN INT32 OSD_RGN_Init(void);

/** get osd region attribute
 *  @param  u4Region region id
 *  @param  i4Cmd attribute name, refer to OSD_RGN_CMD_T
 *  @param  pu4Value attribute value pointer
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_RGN_Get(UINT32 u4Region, INT32 i4Cmd, UINT32 * pu4Value);

EXTERN INT32 OSD_RGN_Set(UINT32 u4Region, INT32 i4Cmd, UINT32 u4Value);

/** create an osd region
 *  @param  pu4Region region id pointer
 *  @param  u4BmpWidth source bitmap width
 *  @param  u4BmpHeight source bitmap height
 *  @param  pvBitmap source bitmap address, MUST align with 16bytes
 *  @param  eColorMode, region color mode, refer to OSD_COLOR_MODE_T
 *  @param  u4BmpPitch, bytes per line, MUST align with 16bytes
 *  @param  u4DispX, region x position
 *  @param  u4DispY, region y position
 *  @param  u4DispW, output width
 *  @param  u4DispH, output height

 *  @retval OSD_RET_OK success
 *  @retval OSD_RET_INV_ARG invalid argument
 */
EXTERN INT32 OSD_RGN_Create(UINT32 * pu4Region, UINT32 u4BmpWidth,
                            UINT32 u4BmpHeight, void * pvBitmap,
                            UINT32 eColorMode, UINT32 u4BmpPitch,
                            UINT32 u4DispX, UINT32 u4DispY, UINT32 u4DispW,
                            UINT32 u4DispH);
                            
/** delete an osd region
 *  @param  u4Region region id
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_RGN_Delete(UINT32 u4Region);

EXTERN INT32 OSD_RGN_AttachTail(UINT32 u4Region, UINT32 u4RgnList);

/** attach region to list
 *  @param  u4Region region id
 *  @param  u4RgnList list id
 *  @retval OSD_RET_OK success
 *  @retval OSD_RET_INV_LIST invalid list
 */
EXTERN INT32 OSD_RGN_Insert(UINT32 u4Region, UINT32 u4RgnList);

/** dettach region from list
 *  @param  u4Region region id
 *  @param  u4RgnList list id
 *  @retval OSD_RET_OK success
 *  @retval OSD_RET_INV_REGION invalid region
 */
EXTERN INT32 OSD_RGN_Detach(UINT32 u4Region, UINT32 u4RgnList);

EXTERN INT32 OSD_RGN_SetDisplayWidth(UINT32 u4Region, UINT32 u4Width);
EXTERN INT32 OSD_RGN_SetDisplayHeight(UINT32 u4Region, UINT32 u4Height);
EXTERN INT32 OSD_RGN_SetBigEndian(UINT32 u4Region, BOOL fgBE);
EXTERN INT32 OSD_RGN_Dump(UINT32 u4Region);

//add for setting/getting the region list used by linux frame buffer
EXTERN INT32 OSD_Set_FrameBuffer_RGN_LIST(UINT32 u4RgnList);
EXTERN INT32 OSD_Get_FrameBuffer_RGN_LIST(UINT32 * pu4Value);

//add for getting the region resolution used by linux frame buffer
EXTERN INT32 OSD_Get_FrameBuffer_Resolution(UINT32* pu4Width,UINT32* pu4Height);

#ifdef CC_FLIP_MIRROR_SUPPORT
EXTERN INT32 OSD_RGN_GetOriginalY(UINT32 u4Region, UINT32 * pu4Value);
EXTERN INT32 OSD_RGN_GetOriginalX(UINT32 u4Region, UINT32 * pu4Value);
#endif


/** cinfugure osd scaler
 *  @param  u4Scaler scaler id(only OSD_SCALER_2)
 *  @param  fgEnable enable=TRUE/disable=FALSE
 *  @param  u4SrcWidth osd input width
 *  @param  u4SrcHeight osd input height
 *  @param  u4DstWidth osd output width
 *  @param  u4DstHeight osd output height
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_SC_Scale(UINT32 u4Scaler, UINT32 u4Enable,
                          UINT32 u4SrcWidth, UINT32 u4SrcHeight,
                          UINT32 u4DstWidth, UINT32 u4DstHeight);
                          
#ifdef CC_DEBUG
EXTERN INT32 OSD_SC_HDown(UINT32 u4Scaler, UINT32 u4SrcWidth, UINT32 u4Step);
EXTERN INT32 OSD_SC_HUp(UINT32 u4Scaler, UINT32 u4SrcWidth, UINT32 u4Step);
EXTERN INT32 OSD_SC_VDown(UINT32 u4Scaler, UINT32 u4SrcHeight, UINT32 u4Step);
EXTERN INT32 OSD_SC_VUp(UINT32 u4Scaler, UINT32 u4SrcHeight, UINT32 u4Step);
#endif

/** get osd scaler infomation
 *  @param  u4Scaler scaler id(only OSD_SCALER_2)
 *  @param  pu4Enable enable pointer
 *  @param  pu4SrcW osd input width pointer
 *  @param  pu4SrcH osd input height pointer
 *  @param  pu4DstW osd output width pointer
 *  @param  pu4DstH osd output height pointer
 *  @param  pu4Is16Bpp don't care
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_SC_GetScalerInfo(UINT32 u4Scaler, UINT32* pu4Enable,
                                  UINT32* pu4SrcW, UINT32* pu4SrcH,
                                  UINT32* pu4DstW, UINT32* pu4DstH,
                                  UINT32* pu4Is16Bpp);
                                  
EXTERN INT32 OSD_SC_SetLpfInfo(UINT32 u4Scaler, UINT32 u4Enable, INT16 i2C1,
                               INT16 i2C2, INT16 i2C3, INT16 i2C4, INT16 i2C5);
                               
/** enable low pass filter
 *  @param  u4Scaler scaler id(only OSD_SCALER_2)
 *  @param  fgEnable enable=TRUE/disable=FALSE
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_SC_SetLpf(UINT32 u4Scaler, UINT32 u4Enable);

EXTERN INT32 OSD_SC_GetLpfInfo(UINT32 u4Scaler, UINT32* pu4Enable, INT16* pi2C1,
                               INT16* pi2C2, INT16* pi2C3,
                               INT16* pi2C4, INT16* pi2C5);
EXTERN void OSD_SC_UpdateDstSize(UINT32 u4Scaler);
EXTERN INT32 OSD_SC_CheckCapability(UINT32 u4SrcW, UINT32 u4SrcH,
                                    UINT32 u4DstH);
EXTERN INT32 OSD_SC_SetSramConfiguration(UINT32 u4Mode);
EXTERN INT32 OSD_SC_GetSramConfiguration(void);
EXTERN INT32 OSD_SC_Dump(UINT32 u4Scaler);
//#ifdef CC_MT5363
//EXTERN INT32 OSD_SC_Swap(BOOL fgSwap);
//#endif
EXTERN INT32 OSD_SC_SetRegion(UINT32 u4Scaler, INT32 i4Region);



EXTERN void OSD_DrawVLine(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                          UINT32 u4ColorMode);
EXTERN void OSD_DrawHLine(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                          UINT32 u4ColorMode);
EXTERN void OSD_DrawBorder(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                           UINT32 u4ColorMode);
EXTERN void OSD_DrawColorbar(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                             UINT32 u4ColorMode);
EXTERN void OSD_DrawSlt(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                        UINT32 u4ColorMode);
EXTERN void OSD_DrawRamp(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                         UINT32 u4ColorMode);
EXTERN void OSD_DrawMute(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                         UINT32 u4ColorMode, UINT32 u4MuteColor);
EXTERN void OSD_CreatePaleTable(OSD_ARGB_T *prPaleTable);
EXTERN void OSD_DrawColor(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                          UINT32 u4ColorMode, UINT32 u4Color);

EXTERN void OSD_DrawPattern0(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                             UINT32 u4ColorMode,UINT32 u4BgColor);
EXTERN void OSD_DrawPattern1(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                 UINT32 u4ColorMode, UINT32 u4Pattern );
EXTERN void OSD_DrawPattern2(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                 UINT32 u4ColorMode, UINT32 u4Pattern);
EXTERN void OSD_DrawPattern3(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                      UINT32 u4ColorMode, UINT32 u4Pattern,UINT32 u4BgColor);
EXTERN void OSD_DrawPattern4(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                      UINT32 u4ColorMode, UINT32 u4Pattern );
EXTERN void OSD_DrawPattern5(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                      UINT32 u4ColorMode);
EXTERN void OSD_DrawPattern6(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                      UINT32 u4ColorMode);
EXTERN void OSD_DrawPattern7(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                      UINT32 u4ColorMode);
EXTERN void OSD_DrawPattern8(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                      UINT32 u4ColorMode, UINT32 uPattern);
EXTERN void OSD_DrawPattern9(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                      UINT32 u4ColorMode);
EXTERN void OSD_DrawPattern10(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                      UINT32 u4ColorMode);

EXTERN void OSD_DrawPattern11(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                      UINT32 u4ColorMode);
EXTERN void OSD_DrawPattern12(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                      UINT32 u4ColorMode);
EXTERN void OSD_DrawPattern13(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                      UINT32 u4ColorMode);
EXTERN void OSD_DrawPattern14(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                      UINT32 u4ColorMode, UINT32 u4Pattern,UINT32 u4BgColor);
EXTERN void OSD_DrawPattern15(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                      UINT32 u4ColorMode);


EXTERN void OSD_DrawPatternL92(
    UINT8* 				pu1Canvas,
    UINT32 				u4Width,
    UINT32 				u4Height,
    UINT32 				u4ColorMode,
    UINT8 				ui1_grid_width,
    UINT8 				ui1_grid_heigh,
    UINT32 				u4BgColor);

EXTERN void OSD_DrawPatternGomi(
    UINT8* 				pu1Canvas,
    UINT32 				u4Width,
    UINT32 				u4Height,
    UINT32 				u4ColorMode,
    UINT32 				u4BgColor,
    UINT8               ui1_mode);
EXTERN void OSD_DrawPatternCrosstalk(
    UINT8* 				pu1Canvas,
    UINT32 				u4Width,
    UINT32 				u4Height,
    UINT32 				u4ColorMode,
    UINT32 				u4BgColor);
EXTERN void OSD_DrawPatternFlicker(
    UINT8* 				pu1Canvas,
    UINT32 				u4Width,
    UINT32 				u4Height,
    UINT32 				u4ColorMode,
    UINT32 				u4BgColor);

/** OSD operation mode.
 */
typedef enum
{
    OSD_OP_NORMAL,
    OSD_OP_DMA,
    OSD_OP_GFX
} OSD_OP_MODE_T;

EXTERN void OSD_DMA_Init(void);

EXTERN void OSD_DMA_SetMode(OSD_OP_MODE_T eMode);

EXTERN void OSD_DMA_SetTarget(UINT32 *au4TargetAddr);

EXTERN INT32 OSD_DMA_TransferRegion(UINT32 u4Plane);

EXTERN INT32 OSD_DMA_WriteRegister(UINT32 u4Reg, UINT32 u4Value, BOOL fg8bitData, UINT32 u4Parity);

EXTERN INT32 OSD_DMA_Copy(UINT32 u4DstAddress, UINT32 u4SrcAddress, UINT32 u4Size);

EXTERN void OSD_DMA_OnOutputVsync(void);

EXTERN void OSD_DMA_SetUpdatePeriod(UINT32 u4DmaUpdatePeriod);

EXTERN void OSD_DMA_Dump(void);

EXTERN OSD_OP_MODE_T OSD_DMA_GetMode(void);

EXTERN void OSD_DMA_EnableI2C(BOOL fgEnable);

EXTERN void OSD_DMA_AfterPaletteUpdate(UINT32 u4PaletteId, UINT32 u4PaletteAddress);

//Add for Set DRAM parameters    	
EXTERN void OSD_PLA_SetContReqLmt(UINT32 u4Plane, UINT32 u4Value);
EXTERN void OSD_PLA_SetFifoSize(UINT32 u4Plane, UINT32 u4Value);
EXTERN void OSD_PLA_SetVacTh(UINT32 u4Plane, UINT32 u4Value);
EXTERN void OSD_PLA_SetPauseCnt(UINT32 u4Plane, UINT32 u4Value);
EXTERN void OSD_PLA_SetContReqLmt0(UINT32 u4Plane, UINT32 u4Value);

EXTERN INT32 OSD_DMA_Set8281(UINT32 u4addr, UINT32 u4value);

EXTERN BOOL OSD_GetUnderflowStatus(void);

EXTERN void OSD_ClearUnderflowStatus(void);

//For OSD scroll
EXTERN INT32 OSD_Vsync_UpdateScroll(void);
EXTERN INT32 OSD_RGN_SetScrollInfo(UINT32 u4Rgn, UINT32 u4BaseAddress, UINT32 u4Pitch, UINT32 u4Step);
EXTERN INT32 OSD_SetScrollMode(UINT8 u1Mode);

#ifdef SYS_3D_SUPPORT
typedef enum
{
    OSD_3D_SBSHALF,
    OSD_3D_TPANDBT,
    OSD_3D_NONE
} OSD_3D_MODE_T;

#ifndef CC_MTK_LOADER
extern INT32 OSD_PLA_Set3DFlip(UINT32 u4Plane, OSD_3D_MODE_T e3DMode, UINT32 u4Shift, 
						UINT32 u4BmpWidth, UINT32 u4BmpHeight,UINT32 u4BmpPitch, 
						UINT32 u4BmpAddr, UINT32 eColorMode);

EXTERN INT32 OSD_PLA_Set3DBlit(UINT32 u4Plane, OSD_3D_MODE_T e3DMode, UINT32 u4Shift, 
						UINT32 u4BmpWidth, UINT32 u4BmpHeight, UINT32 u4BmpPitch,
						UINT32 u4DstAddr, UINT32 eColorMode, UINT32 u4SrcAddr);

EXTERN INT32 OSD_PLA_Redraw3D(OSD_3D_MODE_T e3DMode);
#endif

EXTERN void OSD_SC_ForceSize(BOOL fgForceSize);

#endif

#ifndef CC_MTK_LOADER
#if defined(CC_CONFIG_OSD_SBS)
INT32 OSD_PLA_Set3DDFB(UINT32 u4Plane, UINT32 e3DMode, UINT32 u4Shift, 
						UINT32 u4BmpWidth, UINT32 u4BmpHeight, UINT32 u4BmpPitch,
						UINT32 u4DstAddr, UINT32 eColorMode, UINT32 u4SrcAddr);
#endif
#endif
#endif //OSD_DRVIF_H
