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
 * $RCSfile: x_plane_mxr.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains video decoder driver specific
 *         definitions, which are exported.
 *---------------------------------------------------------------------------*/

#ifndef _X_PLANE_MXR_H_
#define _X_PLANE_MXR_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "res_mngr/x_rm.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* Get operations */
#define PLA_MXR_GET_TYPE_CTRL               (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 0))
#define PLA_MXR_GET_TYPE_BRIGHTNESS         (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 1))
#define PLA_MXR_GET_TYPE_CONTRAST           (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 2))
#define PLA_MXR_GET_TYPE_HUE                (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 3))
#define PLA_MXR_GET_TYPE_SATURATION         (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 4))
#define PLA_MXR_GET_TYPE_MAX_GAMMA_CURVE    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 5))
#define PLA_MXR_GET_TYPE_GAMMA              (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 6))
#define PLA_MXR_GET_TYPE_ADJ_PLA_BY_PORT    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 7))
#define PLA_MXR_GET_TYPE_ADJ_PLA_BY_ID_TYPE (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 8))
#define PLA_MXR_GET_TYPE_PLA_ORDER          (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 9))
#define PLA_MXR_GET_TYPE_CAPABILITY         (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)10))
#define PLA_MXR_GET_TYPE_RESOLUTION         (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)11))
#define PLA_MXR_GET_TYPE_OUTPUT_PORT        (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)13))
#define PLA_MXR_GET_TYPE_BG                 (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)14))
#define PLA_MXR_GET_TYPE_ASPECT_RATIO       (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)15))
#define PLA_MXR_GET_TYPE_DIGITAL_OUTPUT_FMT (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)16))
#define PLA_MXR_GET_TYPE_DITHER             (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)17))
#define PLA_MXR_GET_TYPE_DITHER_CAP         (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)18))
#define PLA_MXR_GET_TYPE_MIN_MAX            (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)19))
#define PLA_MXR_GET_TYPE_COLOR_GAIN         (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)20))
#define PLA_MXR_GET_TYPE_COLOR_GAIN_MAX     (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)21))
#define PLA_MXR_GET_TYPE_COLOR_GAIN_MIN     (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)22))
#define PLA_MXR_GET_TYPE_COLOR_OFFSET       (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)23))
#define PLA_MXR_GET_TYPE_COLOR_OFFSET_MAX   (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)24))
#define PLA_MXR_GET_TYPE_COLOR_OFFSET_MIN   (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)25))
#define PLA_MXR_GET_TYPE_BACK_LIGHT         (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)26))
#define PLA_MXR_GET_TYPE_ADP_BACK_LIGHT     (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)27))
#define PLA_MXR_GET_TYPE_TV_MODE            (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)28))
#define PLA_MXR_GET_TYPE_MJC                (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)29))
#define PLA_MXR_GET_TYPE_TEST_PATTERN       (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)30))
#define PLA_MXR_GET_TYPE_BACK_LIGHT_CTRL    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)31))
#define PLA_MXR_GET_TYPE_ADP_BACK_LIGHT_RNG (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)32))
#define PLA_MXR_GET_TYPE_BACK_LIGHT_ENERGY_SAVE (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)33))
#define PLA_MXR_GET_TYPE_FRAMERATE          (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)34))

/* Set operations */
#define PLA_MXR_SET_TYPE_CTRL                  ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 0)) | RM_SET_TYPE_ARG_NO_REF)
#define PLA_MXR_SET_TYPE_BRIGHTNESS            ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 1)) | RM_SET_TYPE_ARG_NO_REF)
#define PLA_MXR_SET_TYPE_CONTRAST              ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 2)) | RM_SET_TYPE_ARG_NO_REF)
#define PLA_MXR_SET_TYPE_HUE                   ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 3)) | RM_SET_TYPE_ARG_NO_REF)
#define PLA_MXR_SET_TYPE_SATURATION            ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 4)) | RM_SET_TYPE_ARG_NO_REF)
#define PLA_MXR_SET_TYPE_GAMMA                 ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 5)) | RM_SET_TYPE_ARG_NO_REF)
#define PLA_MXR_SET_TYPE_PLA_ORDER              (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 6))
#define PLA_MXR_SET_TYPE_RESOLUTION             (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 7))
#define PLA_MXR_SET_TYPE_OUTPUT_PORT           ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 8)) | RM_SET_TYPE_ARG_NO_REF)
#define PLA_MXR_SET_TYPE_BG                     (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 9))
#define PLA_MXR_SET_TYPE_ASPECT_RATIO          ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)10)) | RM_SET_TYPE_ARG_NO_REF)
#define PLA_MXR_SET_TYPE_DIGITAL_OUTPUT_FMT    ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)11)) | RM_SET_TYPE_ARG_NO_REF)
#define PLA_MXR_SET_TYPE_DITHER                ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)12)) | RM_SET_TYPE_ARG_NO_REF)
#define PLA_MXR_SET_TYPE_COLOR_GAIN             (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)13))
#define PLA_MXR_SET_TYPE_COLOR_OFFSET           (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)14))
#define PLA_MXR_SET_TYPE_BACK_LIGHT            ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)15)) | RM_SET_TYPE_ARG_NO_REF)
#define PLA_MXR_SET_TYPE_ADP_BACK_LIGHT        ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)16)) | RM_SET_TYPE_ARG_NO_REF)
#define PLA_MXR_SET_TYPE_TV_MODE               ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)17)) | RM_SET_TYPE_ARG_NO_REF)
#define PLA_MXR_SET_TYPE_MJC                   ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)18)) | RM_SET_TYPE_ARG_NO_REF)
#define PLA_MXR_SET_TYPE_TEST_PATTERN          ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)19)) | RM_SET_TYPE_ARG_NO_REF)
#define PLA_MXR_SET_TYPE_BACK_LIGHT_CTRL       ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)20)) | RM_SET_TYPE_ARG_NO_REF)
#define PLA_MXR_SET_TYPE_ADP_BACK_LIGHT_RNG    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)21))
#define PLA_MXR_SET_TYPE_BACK_LIGHT_ENERGY_SAVE (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)22))
#define PLA_MXR_SET_TYPE_FRAMERATE             ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)23)) | RM_SET_TYPE_ARG_NO_REF)
#define PLA_MXR_SET_TYPE_FORCE_FRAMERATE       ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)24)) | RM_SET_TYPE_ARG_NO_REF)


/* Dither Capability types */
#define PLA_MXR_DITHER_CAP_NONE       ((UINT32) 0)
#define PLA_MXR_DITHER_CAP_1_BIT      MAKE_BIT_MASK_16(0)
#define PLA_MXR_DITHER_CAP_2_BITS     MAKE_BIT_MASK_16(1)
#define PLA_MXR_DITHER_CAP_3_BITS     MAKE_BIT_MASK_16(2)
#define PLA_MXR_DITHER_CAP_4_BITS     MAKE_BIT_MASK_16(3)
#define PLA_MXR_DITHER_CAP_5_BITS     MAKE_BIT_MASK_16(4)
#define PLA_MXR_DITHER_CAP_6_BITS     MAKE_BIT_MASK_16(5)
#define PLA_MXR_DITHER_CAP_7_BITS     MAKE_BIT_MASK_16(6)
#define PLA_MXR_DITHER_CAP_8_BITS     MAKE_BIT_MASK_16(7)
#define PLA_MXR_DITHER_CAP_9_BITS     MAKE_BIT_MASK_16(8)
#define PLA_MXR_DITHER_CAP_10_BITS    MAKE_BIT_MASK_16(9)
#define PLA_MXR_DITHER_CAP_11_BITS    MAKE_BIT_MASK_16(10)
#define PLA_MXR_DITHER_CAP_12_BITS    MAKE_BIT_MASK_16(11)
#define PLA_MXR_DITHER_CAP_13_BITS    MAKE_BIT_MASK_16(12)
#define PLA_MXR_DITHER_CAP_14_BITS    MAKE_BIT_MASK_16(13)
#define PLA_MXR_DITHER_CAP_15_BITS    MAKE_BIT_MASK_16(14)
#define PLA_MXR_DITHER_CAP_16_BITS    MAKE_BIT_MASK_16(15)


/* PLA_MXR_GET_TYPE_CTRL/PLA_MXR_SET_TYPE_CTRL data info *********************/
typedef enum
{
    PLA_MXR_CTRL_RESET = 0,
    PLA_MXR_CTRL_DISABLE,
    PLA_MXR_CTRL_ENABLE
}   PLA_MXR_CTRL_T;


/* Availiabe z-orders ********************************************************/
#define PLA_MXR_Z_ORDER_UNKNOWN  ((UINT32) 0xFFFFFFFF)
#define PLA_MXR_Z_ORDER_0        ((UINT32) 0)
#define PLA_MXR_Z_ORDER_1        ((UINT32) 1)
#define PLA_MXR_Z_ORDER_2        ((UINT32) 2)
#define PLA_MXR_Z_ORDER_3        ((UINT32) 3)
#define PLA_MXR_Z_ORDER_4        ((UINT32) 4)
#define PLA_MXR_Z_ORDER_5        ((UINT32) 5)
#define PLA_MXR_Z_ORDER_6        ((UINT32) 6)
#define PLA_MXR_Z_ORDER_7        ((UINT32) 7)
#define PLA_MXR_Z_ORDER_8        ((UINT32) 8)
#define PLA_MXR_Z_ORDER_9        ((UINT32) 9)
#define PLA_MXR_Z_ORDER_10       ((UINT32)10)
#define PLA_MXR_Z_ORDER_11       ((UINT32)11)
#define PLA_MXR_Z_ORDER_12       ((UINT32)12)
#define PLA_MXR_Z_ORDER_13       ((UINT32)13)
#define PLA_MXR_Z_ORDER_14       ((UINT32)14)
#define PLA_MXR_Z_ORDER_15       ((UINT32)15)
#define PLA_MXR_Z_ORDER_16       ((UINT32)16)
#define PLA_MXR_Z_ORDER_17       ((UINT32)17)
#define PLA_MXR_Z_ORDER_18       ((UINT32)18)
#define PLA_MXR_Z_ORDER_19       ((UINT32)19)
#define PLA_MXR_Z_ORDER_20       ((UINT32)20)
#define PLA_MXR_Z_ORDER_21       ((UINT32)21)
#define PLA_MXR_Z_ORDER_22       ((UINT32)22)
#define PLA_MXR_Z_ORDER_23       ((UINT32)23)
#define PLA_MXR_Z_ORDER_24       ((UINT32)24)
#define PLA_MXR_Z_ORDER_25       ((UINT32)25)
#define PLA_MXR_Z_ORDER_26       ((UINT32)26)
#define PLA_MXR_Z_ORDER_27       ((UINT32)27)
#define PLA_MXR_Z_ORDER_28       ((UINT32)28)
#define PLA_MXR_Z_ORDER_29       ((UINT32)29)
#define PLA_MXR_Z_ORDER_30       ((UINT32)30)
#define PLA_MXR_Z_ORDER_31       ((UINT32)31)

/* bit mask of adjustable z-order ********************************************/
#define PLA_MXR_Z_ORDER_FLAG_UNKNOWN ((UINT32) 0)
#define PLA_MXR_Z_ORDER_FLAG_0   MAKE_BIT_MASK_32(PLA_MXR_Z_ORDER_0)
#define PLA_MXR_Z_ORDER_FLAG_1   MAKE_BIT_MASK_32(PLA_MXR_Z_ORDER_1)
#define PLA_MXR_Z_ORDER_FLAG_2   MAKE_BIT_MASK_32(PLA_MXR_Z_ORDER_2)
#define PLA_MXR_Z_ORDER_FLAG_3   MAKE_BIT_MASK_32(PLA_MXR_Z_ORDER_3)
#define PLA_MXR_Z_ORDER_FLAG_4   MAKE_BIT_MASK_32(PLA_MXR_Z_ORDER_4)
#define PLA_MXR_Z_ORDER_FLAG_5   MAKE_BIT_MASK_32(PLA_MXR_Z_ORDER_5)
#define PLA_MXR_Z_ORDER_FLAG_6   MAKE_BIT_MASK_32(PLA_MXR_Z_ORDER_6)
#define PLA_MXR_Z_ORDER_FLAG_7   MAKE_BIT_MASK_32(PLA_MXR_Z_ORDER_7)
#define PLA_MXR_Z_ORDER_FLAG_8   MAKE_BIT_MASK_32(PLA_MXR_Z_ORDER_8)
#define PLA_MXR_Z_ORDER_FLAG_9   MAKE_BIT_MASK_32(PLA_MXR_Z_ORDER_9)
#define PLA_MXR_Z_ORDER_FLAG_10  MAKE_BIT_MASK_32(PLA_MXR_Z_ORDER_10)
#define PLA_MXR_Z_ORDER_FLAG_11  MAKE_BIT_MASK_32(PLA_MXR_Z_ORDER_11)
#define PLA_MXR_Z_ORDER_FLAG_12  MAKE_BIT_MASK_32(PLA_MXR_Z_ORDER_12)
#define PLA_MXR_Z_ORDER_FLAG_13  MAKE_BIT_MASK_32(PLA_MXR_Z_ORDER_13)
#define PLA_MXR_Z_ORDER_FLAG_14  MAKE_BIT_MASK_32(PLA_MXR_Z_ORDER_14)
#define PLA_MXR_Z_ORDER_FLAG_15  MAKE_BIT_MASK_32(PLA_MXR_Z_ORDER_15)
#define PLA_MXR_Z_ORDER_FLAG_16  MAKE_BIT_MASK_32(PLA_MXR_Z_ORDER_16)
#define PLA_MXR_Z_ORDER_FLAG_17  MAKE_BIT_MASK_32(PLA_MXR_Z_ORDER_17)
#define PLA_MXR_Z_ORDER_FLAG_18  MAKE_BIT_MASK_32(PLA_MXR_Z_ORDER_18)
#define PLA_MXR_Z_ORDER_FLAG_19  MAKE_BIT_MASK_32(PLA_MXR_Z_ORDER_19)
#define PLA_MXR_Z_ORDER_FLAG_20  MAKE_BIT_MASK_32(PLA_MXR_Z_ORDER_20)
#define PLA_MXR_Z_ORDER_FLAG_21  MAKE_BIT_MASK_32(PLA_MXR_Z_ORDER_21)
#define PLA_MXR_Z_ORDER_FLAG_22  MAKE_BIT_MASK_32(PLA_MXR_Z_ORDER_22)
#define PLA_MXR_Z_ORDER_FLAG_23  MAKE_BIT_MASK_32(PLA_MXR_Z_ORDER_23)
#define PLA_MXR_Z_ORDER_FLAG_24  MAKE_BIT_MASK_32(PLA_MXR_Z_ORDER_24)
#define PLA_MXR_Z_ORDER_FLAG_25  MAKE_BIT_MASK_32(PLA_MXR_Z_ORDER_25)
#define PLA_MXR_Z_ORDER_FLAG_26  MAKE_BIT_MASK_32(PLA_MXR_Z_ORDER_26)
#define PLA_MXR_Z_ORDER_FLAG_27  MAKE_BIT_MASK_32(PLA_MXR_Z_ORDER_27)
#define PLA_MXR_Z_ORDER_FLAG_28  MAKE_BIT_MASK_32(PLA_MXR_Z_ORDER_28)
#define PLA_MXR_Z_ORDER_FLAG_29  MAKE_BIT_MASK_32(PLA_MXR_Z_ORDER_29)
#define PLA_MXR_Z_ORDER_FLAG_30  MAKE_BIT_MASK_32(PLA_MXR_Z_ORDER_30)
#define PLA_MXR_Z_ORDER_FLAG_31  MAKE_BIT_MASK_32(PLA_MXR_Z_ORDER_31)


/* PLA_MXR_GET_TYPE_ADJ_PLA_BY_PORT ******************************************/
typedef struct _PLA_MXR_ADJ_PLA_BY_PORT_INFO_T
{
    UINT8  ui1_port;       /* input */
    UINT32 ui4_adj_zorder; /* output, bit mask of adjustable z-order */
} PLA_MXR_ADJ_PLA_BY_PORT_INFO_T;


/* PLA_MXR_GET_TYPE_ADJ_PLA_BY_ID_TYPE ***************************************/
typedef struct _PLA_MXR_ADJ_PLA_BY_ID_TYPE_INFO_T
{
    DRV_TYPE_T e_plane_type;   /* input */
    UINT16     ui2_comp_id;    /* input */
    UINT32     ui4_adj_zorder; /* output, bit mask of adjustable z-order */
} PLA_MXR_ADJ_PLA_BY_ID_TYPE_INFO_T;


/* PLA_MXR_GET_TYPE_CAPABILITY data info *************************************/
/* plane mixer capabilities */
#define PLA_MXR_CAP_BRIGHTNESS            MAKE_BIT_MASK_32(0)
#define PLA_MXR_CAP_CONTRAST              MAKE_BIT_MASK_32(1)
#define PLA_MXR_CAP_HUE                   MAKE_BIT_MASK_32(2)
#define PLA_MXR_CAP_SATURATION            MAKE_BIT_MASK_32(3)
#define PLA_MXR_CAP_GAMMA                 MAKE_BIT_MASK_32(4)
#define PLA_MXR_CAP_ADJ_PLA               MAKE_BIT_MASK_32(5)
#define PLA_MXR_CAP_PLA_ORDER             MAKE_BIT_MASK_32(6)
#define PLA_MXR_CAP_RESOLUTION            MAKE_BIT_MASK_32(7)
#define PLA_MXR_CAP_DIGITAL_OUT           MAKE_BIT_MASK_32(8)
#define PLA_MXR_CAP_BG                    MAKE_BIT_MASK_32(9)
#define PLA_MXR_CAP_ASPECT_RATIO          MAKE_BIT_MASK_32(10)
#define PLA_MXR_CAP_DIGITAL_OUTPUT_FMT    MAKE_BIT_MASK_32(11)
#define PLA_MXR_CAP_DITHER                MAKE_BIT_MASK_32(12)
#define PLA_MXR_CAP_COLOR_GAIN            MAKE_BIT_MASK_32(13)
#define PLA_MXR_CAP_COLOR_OFFSET          MAKE_BIT_MASK_32(14)
#define PLA_MXR_CAP_BACK_LIGHT            MAKE_BIT_MASK_32(15)
#define PLA_MXR_CAP_ADP_BACK_LIGHT        MAKE_BIT_MASK_32(16)
#define PLA_MXR_CAP_BACK_LIGHT_CTRL       MAKE_BIT_MASK_32(17)



/* plane mixer capability info */
typedef UINT32     PLA_MXR_CAPABILITY_INFO_T;

/* PLA_MXR_GET_TYPE_RESOLUTION/PLA_MXR_SET_TYPE_RESOLUTION data info *********/
typedef struct _PLA_MXR_RESOLUTION_INFO_T
{
    UINT16               ui2_width;
    UINT16               ui2_height;
    BOOL                 b_is_progressive;
    UINT16               ui2_curr_v_clk;
    UINT16               ui2_v_clk_max;/* Ignored when used in set function */
    UINT16               ui2_v_clk_min;/* Ignored when used in set function */
    UINT16               ui2_panel_width;
    UINT16               ui2_panel_height;
} PLA_MXR_RESOLUTION_INFO_T;

/* PLA_MXR_GET_TYPE_OUTPUT_PORT/PLA_MXR_SET_TYPE_OUTPUT_PORT *****************/
/* bit-mask of output port */
#define PLA_MXR_OUTPUT_FLAG_DIGITAL     MAKE_BIT_MASK_32( 0 )
#define PLA_MXR_OUTPUT_FLAG_LVDS        MAKE_BIT_MASK_32( 1 )
#define PLA_MXR_OUTPUT_FLAG_CRT         MAKE_BIT_MASK_32( 2 )


/* PLA_MXR_GET_TYPE_BG/PLA_MXR_SET_TYPE_BG ***********************************/
typedef struct _PLA_MXR_BG_COLOR_T
{
    UINT8       ui1_r;
    UINT8       ui1_g;
    UINT8       ui1_b;
}   PLA_MXR_BG_COLOR_T;


/* PLA_MXR_GET_TYPE_ASPECT_RATIO/PLA_MXR_SET_TYPE_ASPECT_RATIO data info *****/
typedef enum
{
    PLA_MXR_ASPECT_RATIO_UNKNOWN = 0,
    PLA_MXR_ASPECT_RATIO_4_3,
    PLA_MXR_ASPECT_RATIO_16_9,
    PLA_MXR_ASPECT_RATIO_2_21_1
}   PLA_MXR_ASPECT_RATIO_T;

/* PLA_MXR_GET_TYPE_DIGITAL_OUTPUT_FMT/PLA_MXR_SET_TYPE_DIGITAL_OUTPUT_FMT ***/
typedef enum
{
    PLA_MXR_DIGITAL_OUTPUT_FMT_YBR,
    PLA_MXR_DIGITAL_OUTPUT_FMT_RGB
}   PLA_MXR_DIGITAL_OUTPUT_FMT_T;

/* PLA_MXR_GET_TYPE_MIN_MAX **************************************************/
typedef struct _PLA_MXR_MIN_MAX_INFO_T
{
    UINT32  e_get_type;
    UINT32  ui4_min_value;
    UINT32  ui4_max_value;
} PLA_MXR_MIN_MAX_INFO_T;


/* PLA_MXR_GET_TYPE_COLOR_GAIN_MAX/PLA_MXR_GET_TYPE_COLOR_GAIN_MIN ***********/
/* PLA_MXR_GET_TYPE_COLOR_GAIN/PLA_MXR_SET_TYPE_COLOR_GAIN *******************/
typedef struct _PLA_MXR_COLOR_GAIN_T
{
    BOOL        b_auto;
    UINT16      ui2_r_gain;
    UINT16      ui2_g_gain;
    UINT16      ui2_b_gain;
} PLA_MXR_COLOR_GAIN_T;

/* PLA_MXR_GET_TYPE_TV_MODE/PLA_MXR_SET_TYPE_TV_MODE *************************/
typedef enum
{
    PLA_MXR_TV_MODE_NORMAL = 0,
    PLA_MXR_TV_MODE_PIP,
    PLA_MXR_TV_MODE_POP
}   PLA_MXR_TV_MODE_T;

/* PLA_MXR_GET_TYPE_MJC/PLA_MXR_SET_TYPE_MJC *********************************/
typedef enum
{
    PLA_MXR_MJC_OFF = 0,
    PLA_MXR_MJC_ON,
    PLA_MXR_MJC_DNM
}   PLA_MXR_MJC_T;

/* PLA_MXR_GET_TYPE_TEST_PATTERN/PLA_MXR_SET_TYPE_TEST_PATTERN ***************/
typedef enum
{
    PLA_MXR_TEST_PATTERN_DISABLE = 0,
    PLA_MXR_TEST_PATTERN_1,
    PLA_MXR_TEST_PATTERN_2,
    PLA_MXR_TEST_PATTERN_3,
    PLA_MXR_TEST_PATTERN_4,
    PLA_MXR_TEST_PATTERN_5,
    PLA_MXR_TEST_PATTERN_6,
    PLA_MXR_TEST_PATTERN_7,
    PLA_MXR_TEST_PATTERN_8
}   PLA_MXR_TEST_PATTERN_T;

/* PLA_MXR_GET_TYPE_ADP_BACK_LIGHT_RNG/PLA_MXR_SET_TYPE_ADP_BACK_LIGHT_RNG ***/
typedef struct _PLA_MXR_RANGE_INFO_T
{
    UINT32  ui4_min_value;
    UINT32  ui4_max_value;
} PLA_MXR_RANGE_INFO_T;


#endif /* _X_PLANE_MXR_H_ */
