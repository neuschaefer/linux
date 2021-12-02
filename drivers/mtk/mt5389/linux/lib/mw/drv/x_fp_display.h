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
 * $RCSfile: x_fp_display.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This header file contains FP display driver specific definitions,
 *         which are exported.
 *---------------------------------------------------------------------------*/

#ifndef _X_FP_DISPLAY_H_
#define _X_FP_DISPLAY_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "res_mngr/x_rm.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* Get operations */
#define FPD_GET_TYPE_CAPABILITY    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 0))
#define FPD_GET_TYPE_LEVEL         (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 1))
#define FPD_GET_TYPE_MODE          (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 2))
#define FPD_GET_TYPE_COLOR         (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 3))
#define FPD_GET_TYPE_NUM_OF_LINES  (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 4))
#define FPD_GET_TYPE_LINE_LEN      (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 5))
#define FPD_GET_TYPE_CONTENT       (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 6))

/* Set operations */
#define FPD_SET_TYPE_LEVEL    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 1))
#define FPD_SET_TYPE_MODE     (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 2))
#define FPD_SET_TYPE_COLOR    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 3))
#define FPD_SET_TYPE_CONTENT  (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 6))
#define FPD_SET_TYPE_CLEAR    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 7))

/* Flags used during registration */
#define FPD_FLAG_ALPHA_NUMERIC  ((UINT32) 0x00010000)


/* Color and sink types */
typedef enum
{
    FPD_COLOR_SINGLE = 0,
    FPD_COLOR_RED,
    FPD_COLOR_GREEN,
    FPD_COLOR_YELLOW,
    FPD_COLOR_BLUE,
    FPD_COLOR_AMBER,
    FPD_COLOR_WHITE
}   FPD_COLOR_T;

/* Brightness level types */
typedef enum
{
    FPD_LEVEL_0 = 0,
    FPD_LEVEL_10,
    FPD_LEVEL_20,
    FPD_LEVEL_30,
    FPD_LEVEL_40,
    FPD_LEVEL_50,
    FPD_LEVEL_60,
    FPD_LEVEL_70,
    FPD_LEVEL_80,
    FPD_LEVEL_90,
    FPD_LEVEL_100
}   FPD_LEVEL_T;

/* Mode types */
typedef enum
{
    FPD_MODE_STEADY = 0,
    FPD_MODE_BLINK,
    FPD_MODE_CIRCLE_RIGHT,
    FPD_MODE_CIRCLE_LEFT
}   FPD_MODE_T;

/* Speed types */
typedef enum
{
    FPD_SPEED_SLOW = 0,
    FPD_SPEED_MEDIUM,
    FPD_SPEED_FAST
}   FPD_SPEED_T;

/* Capability color */
#define FPD_CAP_COLOR_SINGLE  (((UINT32) 1) << FPD_COLOR_SINGLE)
#define FPD_CAP_COLOR_RED     (((UINT32) 1) << FPD_COLOR_RED)
#define FPD_CAP_COLOR_GREEN   (((UINT32) 1) << FPD_COLOR_GREEN)
#define FPD_CAP_COLOR_YELLOW  (((UINT32) 1) << FPD_COLOR_YELLOW)
#define FPD_CAP_COLOR_BLUE    (((UINT32) 1) << FPD_COLOR_BLUW)
#define FPD_CAP_COLOR_AMBER   (((UINT32) 1) << FPD_COLOR_AMBER)
#define FPD_CAP_COLOR_WHITE   (((UINT32) 1) << FPD_COLOR_WHITE)

/* Capability level */
#define FPD_CAP_LEVEL_0    (((UINT32) 1) << FPD_LEVEL_0)
#define FPD_CAP_LEVEL_10   (((UINT32) 1) << FPD_LEVEL_10)
#define FPD_CAP_LEVEL_20   (((UINT32) 1) << FPD_LEVEL_20)
#define FPD_CAP_LEVEL_30   (((UINT32) 1) << FPD_LEVEL_30)
#define FPD_CAP_LEVEL_40   (((UINT32) 1) << FPD_LEVEL_40)
#define FPD_CAP_LEVEL_50   (((UINT32) 1) << FPD_LEVEL_50)
#define FPD_CAP_LEVEL_60   (((UINT32) 1) << FPD_LEVEL_60)
#define FPD_CAP_LEVEL_70   (((UINT32) 1) << FPD_LEVEL_70)
#define FPD_CAP_LEVEL_80   (((UINT32) 1) << FPD_LEVEL_80)
#define FPD_CAP_LEVEL_90   (((UINT32) 1) << FPD_LEVEL_90)
#define FPD_CAP_LEVEL_100  (((UINT32) 1) << FPD_LEVEL_100)

/* Capability speed */
#define FPD_CAP_SPEED_SLOW    (((UINT32) 1) << FPD_STATE_SLOW)
#define FPD_CAP_SPEED_MEDIUM  (((UINT32) 1) << FPD_STATE_MEDIUM)
#define FPD_CAP_SPEED_FAST    (((UINT32) 1) << FPD_STATE_FAST)

/* Capability info */
typedef struct _FPD_CAPABILITY_INFO_T
{
    UINT32  ui4_level;
    UINT32  ui4_color;
    UINT32  ui4_blink_speed;
    UINT32  ui4_circle_right_speed;
    UINT32  ui4_circle_left_speed;
}   FPD_CAPABILITY_INFO_T;

/* Content clear info */
typedef struct _FPD_CLEAR_INFO_T
{
    UINT16  ui2_line_num;

    SIZE_T  z_offset;

    SIZE_T  z_len;
}   FPD_CLEAR_INFO_T;

/* Content display info. Used to show ASCII or UNICODE */
/* represented as sequences of UTF-8 code values.      */
typedef struct _FPD_CONTENT_CHAR_INFO_T
{
    UINT16  ui2_line_num;

    SIZE_T  z_offset;

    CHAR*  ps_content;
}   FPD_CONTENT_CHAR_INFO_T;

/* Mode info */
typedef struct _FPD_MODE_INFO_T
{
    FPD_MODE_T   e_mode;
    FPD_SPEED_T  e_speed;
}   FPD_MODE_INFO_T;


#endif /* _X_FP_DISPLAY_H_ */
