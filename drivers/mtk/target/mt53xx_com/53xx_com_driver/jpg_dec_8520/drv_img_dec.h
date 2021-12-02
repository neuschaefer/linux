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
 * $RCSfile: drv_img_dec.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $ 2007/08/27
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: Vincent Hsu $
 * $MD5HEX:  $
 *
 * Description: 
 *         This file exports all set & get types commands and related structures 
 *         for the image decoder driver.
 *---------------------------------------------------------------------------*/

#ifndef _DRV_IMG_DEC_H_
#define _DRV_IMG_DEC_H_


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* get types */
#define IMG_DRV_GET_TYPE_PROGRESS   0x1000
    
/* set types */
#define IMG_DRV_SET_TYPE_FRM_START  0x2001
#define IMG_DRV_SET_TYPE_DECODE     0x2002
#define IMG_DRV_SET_TYPE_STOP       0x2003
#define IMG_DRV_SET_TYPE_BUF_FILLED 0x2004
#define IMG_DRV_SET_TYPE_FRM_END    0x2005

/* Driver error codes */
#define IMG_DRV_DEC_HW_ERROR        (-1)
#define IMG_DRV_DEC_IO_ERROR        (-2)
#define IMG_DRV_DEC_NOT_SUPPORT     (-3)

/* MW buffer-filling return codes */
#define IMG_DRV_MW_FILL_OK          ( 0)
#define IMG_DRV_MW_FILL_EOF         (-1)
#define IMG_DRV_MW_FILL_ERROR       (-2)

#define RMR_IMG_DRV_GET_FAILED      -1
#define RMR_IMG_DRV_SET_FAILED      -2
#define RMR_IMG_DRV_OK              0

/* notification states */
typedef enum
{
    IMG_DRV_NFY_FILL_BUF,
    IMG_DRV_NFY_FINISHED,
    IMG_DRV_NFY_ERROR,
    IMG_DRV_NFY_STOP_DONE,
    IMG_DRV_NFY_ALREADY_STOPPED
} IMG_DRV_NFY_STATE_T;

/* callback function */
typedef VOID (*IMG_DRV_NFY_FCT_T)
(
    UINT32              ui4_img_id,         /* the image id which causes this notification */
    VOID                *pv_tag,            /* tag passed to the callback function */
    VOID                *pv_data,           /* data passed with this notification */
    IMG_DRV_NFY_STATE_T e_state             /* notification state */
);

/* rotation information */
typedef enum
{
    IMG_DRV_ROTATE_NONE,                    /* no rotation */
    IMG_DRV_ROTATE_CW_90,                   /* clockwise  90 degrees  */
    IMG_DRV_ROTATE_CW_180,                  /* clockwise 180 degrees  */
    IMG_DRV_ROTATE_CW_270,                  /* clockwise 270 degrees  */
    IMG_DRV_ROTATE_NONE_WITH_FLIP,          /* no rotation, with flip */
    IMG_DRV_ROTATE_CW_90_WITH_FLIP,         /* clockwise  90 degrees, with flip */
    IMG_DRV_ROTATE_CW_180_WITH_FLIP,        /* clockwise 180 degrees, with flip */
    IMG_DRV_ROTATE_CW_270_WITH_FLIP         /* clockwise 270 degrees, with flip */
} IMG_DRV_ROTATE_T;

/* data passed with IMG_SET_TYPE_FRM_START */
typedef struct
{
    UINT32              ui4_img_id;         /* the image id decoded */
    VOID                *pv_img_buf;        /* image data */
    UINT32              ui4_img_size;       /* image size */
    VOID                *pv_aux_cache;      /* auxiliary cache data */
    IMG_DRV_NFY_FCT_T   pf_func;            /* callback function */
    VOID                *pv_tag;            /* tag passed to the callback function */
} IMG_DRV_FRM_START_T;

/* data passed with IMG_NFY_FILL_BUF */
typedef struct
{
    UINT32              ui4_trsn_id;        /* transaction id */
    VOID                *pv_start_add;      /* starting address */
    UINT32              ui4_required_len;   /* required length */
    BOOL                b_reset_pos;        /* position-resetting flag */
    UINT32              ui4_position;       /* the position to be resettd */
} IMG_DRV_FILL_BUF_T;

/* data passed with IMG_SET_TYPE_BUF_FILLED */
typedef struct
{
    UINT32          ui4_trsn_id;        /* transaction id */
    INT32           i4_ret;             /* MW buffer-filling return codes */
    UINT32          ui4_filled_len;     /* filled length */
} IMG_DRV_BUF_FILLED_T;

/* enumerations */
typedef enum _GFX_COLORMODE_T
{
    GFX_COLORMODE_AYCbCr_CLUT2  = 0,
    GFX_COLORMODE_AYCbCr_CLUT4  = 1,
    GFX_COLORMODE_AYCbCr_CLUT8  = 2,
    GFX_COLORMODE_CbYCrY_16     = 3,
    GFX_COLORMODE_YCbYCr_16     = 4,
    GFX_COLORMODE_AYCbCr_D8888  = 5,
    GFX_COLORMODE_ARGB_CLUT2    = 6,
    GFX_COLORMODE_ARGB_CLUT4    = 7,
    GFX_COLORMODE_ARGB_CLUT8    = 8,
    GFX_COLORMODE_RGB_D565      = 9,
    GFX_COLORMODE_ARGB_D1555    = 10,
    GFX_COLORMODE_ARGB_D4444    = 11,
    GFX_COLORMODE_ARGB_D8888    = 12
} GFX_DRV_COLORMODE_T;

/* data passed with IMG_SET_TYPE_DECODE */
typedef struct
{
    UINT32              ui4_img_id;         /* the image id decoded */
    UINT32              ui4_frame_index;    /* frame index */

    UINT32              ui4_src_x;          /* x offset in the source image in pixels */
    UINT32              ui4_src_y;          /* y offset in the source image in pixels */
    UINT32              ui4_src_width;      /* width to be decoded in pixels */
    UINT32              ui4_src_height;     /* height to be decoded in pixels */

    VOID                *pv_dst;            /* destination starting address */
    UINT32              ui4_dst_x;          /* x offset in the destination in pixels */
    UINT32              ui4_dst_y;          /* y offset in the destination in pixels */
    UINT32              ui4_dst_width;      /* expected output width in pixels */
    UINT32              ui4_dst_height;     /* expected output height in pixels */
    UINT32              ui4_dst_pitch;      /* pitch of the destination image */
    GFX_DRV_COLORMODE_T e_dst_cm;           /* destination color mode */
    
    IMG_DRV_ROTATE_T    e_rotate;           /* rotation option */
} IMG_DRV_DECODE_T;

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

#endif /* _DRV_IMG_DEC_H_ */

