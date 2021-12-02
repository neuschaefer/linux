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
 * $RCSfile: img_slideshow.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: Photo Slideshow Library Header file
 *---------------------------------------------------------------------------*/

#ifndef _IMG_SLIDESHOW_H_
#define _IMG_SLIDESHOW_H_

#include "u_common.h"
#include "x_dbg.h"
#include "x_os.h"
#include "x_handle.h"

#define SLIDESHOW_OK				0
#define SLIDESHOW_INVALID_ARG 		-1
#define SLIDESHOW_SEMA_ERROR		-2
#define SLIDESHOW_STOP				-3
#define SLIDESHOW_INITIAL_ERROR		-4
#define SLIDESHOW_MSG_ERROR			-5

#define SLIDESHOW_CLI_INV_ARG		-6
#define SLIDESHOW_CLI_OUT_OF_MEMORY -7

#define PHOTO_SLIDE_SHOW_EFFECT                         1
#define ENABLE_PHOTO_TRANS_EFFECT_BLOCKS                1
#define ENABLE_PHOTO_TRANS_EFFECT_DIAGONAL_BLOCKS       1
#define ENABLE_PHOTO_TRANS_EFFECT_WIPE                  1
#define ENABLE_PHOTO_TRANS_EFFECT_SLIDE                 1
#define ENABLE_PHOTO_TRANS_EFFECT_FADE_IN               1


#if PHOTO_SLIDE_SHOW_EFFECT
/**
 * Slide show effect enum
*/
typedef enum
{
    IMG_SLIDE_SHOW_EFFECT_NORMAL,//0
#if (ENABLE_PHOTO_TRANS_EFFECT_BLOCKS)
    IMG_SLIDE_SHOW_EFFECT_HORZ_BLOCKS_TOPLEFT, //01
    IMG_SLIDE_SHOW_EFFECT_HORZ_BLOCKS_TOPRIGHT,
    IMG_SLIDE_SHOW_EFFECT_HORZ_BLOCKS_BOTTOMLEFT,
    IMG_SLIDE_SHOW_EFFECT_HORZ_BLOCKS_BOTTOMRIGHT,
    IMG_SLIDE_SHOW_EFFECT_VERT_BLOCKS_TOPLEFT,
    IMG_SLIDE_SHOW_EFFECT_VERT_BLOCKS_TOPRIGHT,
    IMG_SLIDE_SHOW_EFFECT_VERT_BLOCKS_BOTTOMLEFT,
    IMG_SLIDE_SHOW_EFFECT_VERT_BLOCKS_BOTTOMRIGHT,
    IMG_SLIDE_SHOW_EFFECT_HORZ_ZIGZAG_BLOCKS_TOPLEFT,
    IMG_SLIDE_SHOW_EFFECT_HORZ_ZIGZAG_BLOCKS_TOPRIGHT,//10
    IMG_SLIDE_SHOW_EFFECT_HORZ_ZIGZAG_BLOCKS_BOTTOMLEFT,
    IMG_SLIDE_SHOW_EFFECT_HORZ_ZIGZAG_BLOCKS_BOTTOMRIGHT,
    IMG_SLIDE_SHOW_EFFECT_VERT_ZIGZAG_BLOCKS_TOPLEFT,
    IMG_SLIDE_SHOW_EFFECT_VERT_ZIGZAG_BLOCKS_TOPRIGHT,
    IMG_SLIDE_SHOW_EFFECT_VERT_ZIGZAG_BLOCKS_BOTTOMLEFT,
    IMG_SLIDE_SHOW_EFFECT_VERT_ZIGZAG_BLOCKS_BOTTOMRIGHT,
    IMG_SLIDE_SHOW_EFFECT_BLOCKS_RANDOM,//17
#endif
#if (ENABLE_PHOTO_TRANS_EFFECT_DIAGONAL_BLOCKS)
    IMG_SLIDE_SHOW_EFFECT_DIAGONAL_BLOCKS_TOPLEFT,
    IMG_SLIDE_SHOW_EFFECT_DIAGONAL_BLOCKS_TOPRIGHT,
    IMG_SLIDE_SHOW_EFFECT_DIAGONAL_BLOCKS_BOTTOMLEFT, //20
    IMG_SLIDE_SHOW_EFFECT_DIAGONAL_BLOCKS_BOTTOMRIGHT,
#endif
#if (ENABLE_PHOTO_TRANS_EFFECT_WIPE)
    IMG_SLIDE_SHOW_EFFECT_BARWIPE_LEFT2RIGHT,//22
    IMG_SLIDE_SHOW_EFFECT_BARWIPE_RIGHT2LEFT,//23
    IMG_SLIDE_SHOW_EFFECT_BARWIPE_TOP2BOTTOM,//24
    IMG_SLIDE_SHOW_EFFECT_BARWIPE_BOTTOM2TOP,//25
    IMG_SLIDE_SHOW_EFFECT_BOXWIPE_TOPLEFT,//26
    IMG_SLIDE_SHOW_EFFECT_BOXWIPE_TOPRIGHT,//27
    IMG_SLIDE_SHOW_EFFECT_BOXWIPE_BOTTOMLEFT,//28
    IMG_SLIDE_SHOW_EFFECT_BOXWIPE_BOTTOMRIGHT,//29
    IMG_SLIDE_SHOW_EFFECT_BARNDOORWIPE_HORIZONTAL_OPEN,//30
    IMG_SLIDE_SHOW_EFFECT_BARNDOORWIPE_HORIZONTAL_CLOSE,//31
    IMG_SLIDE_SHOW_EFFECT_BARNDOORWIPE_VERTICAL_OPEN,//32
    IMG_SLIDE_SHOW_EFFECT_BARNDOORWIPE_VERTICAL_CLOSE,//33
    IMG_SLIDE_SHOW_EFFECT_BOXWIPE_TOPCENTER,//34
    IMG_SLIDE_SHOW_EFFECT_BOXWIPE_RIGHTCENTER,//35
    IMG_SLIDE_SHOW_EFFECT_BOXWIPE_BOTTOMCENTER,//36
    IMG_SLIDE_SHOW_EFFECT_BOXWIPE_LEFTCENTER,//37
    IMG_SLIDE_SHOW_EFFECT_IRISWIPE_CENTER,//38
    IMG_SLIDE_SHOW_EFFECT_IRISWIPE_OUTER,//39
    IMG_SLIDE_SHOW_EFFECT_4BARWIPE_HORIZONTAL_FROM_TOP,//40
    IMG_SLIDE_SHOW_EFFECT_4BARWIPE_HORIZONTAL_FROM_BOTTOM,
    IMG_SLIDE_SHOW_EFFECT_4BARWIPE_VERTICAL_FROM_LEFT,
    IMG_SLIDE_SHOW_EFFECT_4BARWIPE_VERTICAL_FROM_RIGHT,//43
#endif
#if (ENABLE_PHOTO_TRANS_EFFECT_SLIDE)
    IMG_SLIDE_SHOW_EFFECT_BARSLIDE_LEFT2RIGHT,//44
    IMG_SLIDE_SHOW_EFFECT_BARSLIDE_RIGHT2LEFT,
    IMG_SLIDE_SHOW_EFFECT_BARSLIDE_TOP2BOTTOM,
    IMG_SLIDE_SHOW_EFFECT_BARSLIDE_BOTTOM2TOP,
    IMG_SLIDE_SHOW_EFFECT_BOXSLIDE_TOPLEFT,
    IMG_SLIDE_SHOW_EFFECT_BOXSLIDE_TOPRIGHT,
    IMG_SLIDE_SHOW_EFFECT_BOXSLIDE_BOTTOMLEFT,//50
    IMG_SLIDE_SHOW_EFFECT_BOXSLIDE_BOTTOMRIGHT,
    IMG_SLIDE_SHOW_EFFECT_BARNDOORSLIDE_HORIZONTAL_OPEN,
    IMG_SLIDE_SHOW_EFFECT_BARNDOORSLIDE_HORIZONTAL_CLOSE,
    IMG_SLIDE_SHOW_EFFECT_BARNDOORSLIDE_VERTICAL_OPEN,
    IMG_SLIDE_SHOW_EFFECT_BARNDOORSLIDE_VERTICAL_CLOSE,
    IMG_SLIDE_SHOW_EFFECT_4BARSLIDE_HORIZONTAL_FROM_TOP,
    IMG_SLIDE_SHOW_EFFECT_4BARSLIDE_HORIZONTAL_FROM_BOTTOM,
    IMG_SLIDE_SHOW_EFFECT_4BARSLIDE_VERTICAL_FROM_LEFT,
    IMG_SLIDE_SHOW_EFFECT_4BARSLIDE_VERTICAL_FROM_RIGHT,//59
#endif
#if (ENABLE_PHOTO_TRANS_EFFECT_FADE_IN)
    IMG_SLIDE_SHOW_EFFECT_FADE_IN,
#endif
    SLIDESHOW_MAX
} IMG_SLIDE_SHOW_EFFECT_T;
#endif //#if PHOTO_SLIDE_SHOW_EFFECT

typedef enum 
{
	SLIDESHOW_NFY_COMPLETE_ONE_FRAME,
	SLIDESHOW_NFY_COMPLETE_ONE_PIC,
	SLIDESHOW_NFY_STOP,
	SLIDESHOW_NFY_PAUSE,
	SLIDESHOW_NFY_RESUME
}SLIDESHOW_NFY_EVENT_TYPE_T;

typedef enum
{
    SLIDESHOW_MSG_START,
    SLIDESHOW_MSG_EXIT
}SLIDESHOW_MSG_TYPE_T;

typedef struct 
{
    SLIDESHOW_MSG_TYPE_T	e_type;
	IMG_SLIDE_SHOW_EFFECT_T 	effect; 
	UCHAR						*src_Y;
	UCHAR						*src_C; //422 YYYY+CbCrCbCr
	UCHAR						*cur_Y;
	UCHAR						*cur_C;
	UCHAR						*dst_Y;
	UCHAR						*dst_C;
	UINT32						ui4_Y_buf_size;
	UINT32						ui4_C_buf_size;
	UINT32						ui4_width;
	UINT32						ui4_height;
	UINT32						ui4_total_time;
}SLIDESHOW_MSG_T;

typedef VOID (*img_slidshow_nfy_fct)	(SLIDESHOW_NFY_EVENT_TYPE_T	e_event	);

INT32	x_img_init_slideshow(img_slidshow_nfy_fct pf_nfy);

INT32	x_img_async_slideshow(
	IMG_SLIDE_SHOW_EFFECT_T 	effect, 
	UCHAR						*src_Y,
	UCHAR						*src_C, //422 YYYY+CbCrCbCr
	UCHAR						*cur_Y,
	UCHAR						*cur_C,
	UCHAR						*dst_Y,
	UCHAR						*dst_C,
	UINT32						ui4_Y_buf_size,
	UINT32						ui4_C_buf_size,
	UINT32						ui4_width,
	UINT32						ui4_height,
	UINT32						ui4_total_time);

INT32	x_img_sync_slideshow(
	IMG_SLIDE_SHOW_EFFECT_T 	effect, 
	UCHAR						*src_Y,
	UCHAR						*src_C, //422 YYYY+CbCrCbCr
	UCHAR						*cur_Y,
	UCHAR						*cur_C,
	UCHAR						*dst_Y,
	UCHAR						*dst_C,
	UINT32						ui4_Y_buf_size,
	UINT32						ui4_C_buf_size,
	UINT32						ui4_width,
	UINT32						ui4_height,
	UINT32						ui4_total_time);

INT32	x_img_stop_slideshow(VOID);

#endif

