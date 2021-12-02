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
 * $RCSfile: x_img_dec.h,v $
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

#ifndef _X_IMG_DEC_H_
#define _X_IMG_DEC_H_


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "x_gfx.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* get types */
#define IMG_GET_TYPE_PROGRESS                       \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 1))

#define IMG_GET_TYPE_LIMITATION                     \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 2))
    
/* set types */
#define IMG_SET_TYPE_CONFIG							\
	(RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 1))

#define IMG_SET_TYPE_FRM_START                      \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 2))
    
#define IMG_SET_TYPE_DECODE                         \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 3))
    
#define IMG_SET_TYPE_STOP                           \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 4))
    
#define IMG_SET_TYPE_BUF_FILLED                     \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 5))
    
#define IMG_SET_TYPE_FRM_END                        \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 6))

#define IMG_SET_TYPE_DIRECT_DECODE                  \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 7))

/* Driver error codes */
#define IMG_DRV_HW_ERROR        (-1)
#define IMG_DRV_IO_ERROR        (-2)
#define IMG_DRV_NOT_SUPPORT     (-3)
#define IMG_DRV_BUSY            (-4)

/* MW buffer-filling return codes */
#define IMG_MW_FILL_OK          ( 0)
#define IMG_MW_FILL_EOF         (-1)
#define IMG_MW_FILL_ERROR       (-2)

/* ID for limitation qeury */
#define IMG_LIMIT_PROGRESSIVE       ((UINT32) 0)
#define IMG_LIMIT_RESOLUTION        ((UINT32) 1)
#define IMG_LIMIT_PGSV_RESOLUTION   ((UINT32) 2)
#define IMG_LIMIT_FULL_MODE         ((UINT32) 3)

/* notification states */
typedef enum
{
    IMG_NFY_FILL_BUF,
    IMG_NFY_FINISHED,
    IMG_NFY_ERROR,
    IMG_NFY_STOP_DONE,
    IMG_NFY_ALREADY_STOPPED,
    IMG_NFY_DRV_STOPED
} IMG_NFY_STATE_T;

/* callback function */
typedef VOID (*IMG_NFY_FCT_T)
(
    UINT32          ui4_img_id,         /* the image id which causes this notification */
    VOID            *pv_tag,            /* tag passed to the callback function */
    VOID            *pv_data,           /* data passed with this notification */
    IMG_NFY_STATE_T e_state);           /* notification state */

/* rotation information */
typedef enum
{
#if 0
    IMG_ROTATE_NONE             = 0,    /* no rotation */
    IMG_ROTATE_CW_90            = 1,    /* clockwise  90 degrees  */
    IMG_ROTATE_CW_180           = 2,    /* clockwise 180 degrees  */
    IMG_ROTATE_CW_270           = 3,    /* clockwise 270 degrees  */
    IMG_ROTATE_NONE_WITH_FLIP   = 4,    /* no rotation, with flip */
    IMG_ROTATE_CW_90_WITH_FLIP  = 5,    /* clockwise  90 degrees, with flip */
    IMG_ROTATE_CW_180_WITH_FLIP = 6,    /* clockwise 180 degrees, with flip */
    IMG_ROTATE_CW_270_WITH_FLIP = 7     /* clockwise 270 degrees, with flip */
#endif
	IMG_ROTATE_NONE			 		= 0,
	IMG_ROTATE_NONE_WITH_FLIP	 	= 1,
	IMG_ROTATE_CW_180			 	= 2,
	IMG_ROTATE_CW_180_WITH_FLIP 	= 3,
	IMG_ROTATE_CW_270_WITH_FLIP 	= 4,
	IMG_ROTATE_CW_90			 	= 5,
	IMG_ROTATE_CW_90_WITH_FLIP  	= 6,
	IMG_ROTATE_CW_270			 	= 7
} IMG_ROTATE_T;

/* data passed with IMG_SET_TYPE_FRM_START */
typedef struct
{
    UINT32          ui4_img_id;         /* the image id decoded */
    VOID            *pv_img_buf;        /* image data */
    UINT32          ui4_img_size;       /* image size */
    VOID            *pv_aux_cache;      /* auxiliary cache data */
    IMG_NFY_FCT_T   pf_func;            /* callback function */
    VOID            *pv_tag;            /* tag passed to the callback function */
} IMG_FRM_START_T;

/* data passed with IMG_NFY_FILL_BUF */
typedef struct
{
    UINT32          ui4_trsn_id;        /* transaction id */
    VOID            *pv_start_add;      /* starting address */
    UINT32          ui4_required_len;   /* required length */
    BOOL            b_reset_pos;        /* position-resetting flag */
    UINT32          ui4_position;       /* the position to be resettd */
} IMG_FILL_BUF_T;

/* data passed with IMG_SET_TYPE_BUF_FILLED */
typedef struct
{
    UINT32          ui4_trsn_id;        /* transaction id */
    INT32           i4_ret;             /* MW buffer-filling return codes */
    UINT32          ui4_filled_len;     /* filled length */
} IMG_BUF_FILLED_T;

/* data passed with IMG_SET_TYPE_DECODE */
typedef struct
{	  
    VOID            *pv_img_buf;        /* image data */
    VOID            *pv_img_buf2;       /* image data2 */
    UINT32          ui4_img_size;       /* image size */
    VOID            *pv_type_data;      /* type data */

    UINT32          ui4_src_x;          /* x offset in the source image in pixels */
    UINT32          ui4_src_y;          /* y offset in the source image in pixels */
    UINT32          ui4_src_width;      /* width to be decoded in pixels */
    UINT32          ui4_src_height;     /* height to be decoded in pixels */

    VOID            *pv_dst;            /* destination starting address */
    VOID            *pv_dst2;           /* additional dst starting address */
    UINT32          ui4_dst_x;          /* x offset in the destination in pixels */
    UINT32          ui4_dst_y;          /* y offset in the destination in pixels */
    UINT32          ui4_dst_width;      /* expected output width in pixels */
    UINT32          ui4_dst_height;     /* expected output height in pixels */
    UINT32          ui4_dst_pitch;      /* pitch of the destination image */
    UINT32          ui4_dst2_pitch;     /* pitch of the additional destination image */
    GFX_COLORMODE_T e_dst_cm;           /* destination color mode */
    
    IMG_ROTATE_T    e_rotate;           /* rotation option */
} IMG_DIRECT_DECODE_T;

typedef struct
{
    UINT32          ui4_img_id;         /* the image id decoded */
    VOID            *pv_type_data;      /* frame index */

    UINT32          ui4_src_x;          /* x offset in the source image in pixels */
    UINT32          ui4_src_y;          /* y offset in the source image in pixels */
    UINT32          ui4_src_width;      /* width to be decoded in pixels */
    UINT32          ui4_src_height;     /* height to be decoded in pixels */

    VOID            *pv_dst;            /* destination starting address */
    VOID            *pv_dst2;           /* additional dst starting address */
    UINT32          ui4_dst_x;          /* x offset in the destination in pixels */
    UINT32          ui4_dst_y;          /* y offset in the destination in pixels */
    UINT32          ui4_dst_width;      /* expected output width in pixels */
    UINT32          ui4_dst_height;     /* expected output height in pixels */
    UINT32          ui4_dst_pitch;      /* pitch of the destination image */
    UINT32          ui4_dst2_pitch;     /* pitch of the additional destination image */
    GFX_COLORMODE_T e_dst_cm;           /* destination color mode */
    
    IMG_ROTATE_T    e_rotate;           /* rotation option */
    BOOL b_compressed;
	UINT32			ui4_CbCrOffset;
} IMG_DECODE_T;

/* passed with IMG_GET_TYPE_LIMITATION */
typedef struct
{
    /* INPUT */
    UINT32          ui4_type;

    /* OUTPUT */
    union {
        /* used for IMG_LIMIT_PROGRESSIVE */
        BOOL        b_progressive_support;
        
        /* used for IMG_LIMIT_RESOLUTION */
        struct {
            UINT32  ui4_max_width;
            UINT32  ui4_min_width;
            UINT32  ui4_max_height;
            UINT32  ui4_min_height;
        } t_resolution;

        /* used for IMG_LIMIT_PGSV_RESOLUTION */
        struct {
            UINT32  ui4_max_area;
            UINT32  ui4_min_width;
            UINT32  ui4_min_height;
        } t_pgsv_resolution;

        /* used for IMG_LIMIT_FULL_MODE */
        BOOL        b_full_mode_support;
    } u;
} IMG_LIMITATION_T;

typedef enum
{
    IMAGE_TYPE_JPEG,
    IMAGE_TYPE_PNG,
    IMAGE_TYPE_WEBP,
    IMAGE_TYPE_MAX
}ENUM_IMG_TYPE;

//component flag
#define		JPG_COMP_FLAG_JPG_SUPPORT	0x4
#define		JPG_COMP_FLAG_PNG_SUPPORT	0x8
#define  	JPG_COMP_FLAG_WEBP_SUPPORT	0x10

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* _X_IMG_DEC_H_ */

