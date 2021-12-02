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
 * $RCSfile: x_jpg_hw.h,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/1 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 59e343be09047b7cf9dc29a005ce42fe $ Asa
 *
 * Description: 
 *         This file exports all image library API and constant for 
 *         other module using.
 *---------------------------------------------------------------------------*/

#ifndef _X_JPG_HW_H_
#define _X_JPG_HW_H_


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "x_gfx.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* This structure enumerates all event types for the callback function (*x_img_nfy_fct). */
#ifdef __cplusplus
extern "C" {
#endif

#define DCTSIZE2 64        /* DCTSIZE squared; # of elements in a block */
#define MAX_DOUBLE_BUFFER_NUM           ((UINT32)  2)
/*-----------------------------------------------------------------------------
 * Get types
 *---------------------------------------------------------------------------*/
#define JPG_HW_DEC_GET_TYPE_COLOR_BUF \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 0))
#define JPG_HW_DEC_GET_TYPE_FILL_BUF \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 1))

    
/*-----------------------------------------------------------------------------
 * Set types
 *---------------------------------------------------------------------------*/
#define JPG_HW_DEC_SET_TYPE_SETUP \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 0))
#define JPG_HW_DEC_SET_TYPE_START \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 1))
#define JPG_HW_DEC_SET_TYPE_END   \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 2))

/* NEW jpg drv cmd */
#define JPG_HW_SET_TYPE_INIT      \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 3))
    
#define JPG_HW_SET_TYPE_DECODE    \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 4))
    
#define JPG_HW_SET_TYPE_STOP      \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 5))
    
#define JPG_HW_SET_TYPE_END       \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 6))

/* notification state */
#define JPG_HW_NFY_FILL_BUF         (1)     /* driver needs more data */
#define JPG_HW_NFY_PROGRESS         (2)     /* driver notifies the progress (%) */
#define JPG_HW_NFY_FINISHED         (3)     /* decoding process finished */
#define JPG_HW_NFY_ERROR            (4)     /* decoding error */
#define JPG_HW_NFY_STOP_DONE        (5)     /* stop command notification */
#define JPG_HW_NFY_ALREADY_STOPPED  (6)     /* stop command notification */

/* retrn value of the stop notification */
typedef enum
{
    JPG_HW_STOP_OK,                     /* the stop command is done */
    JPG_HW_ALREADY_STOPPED              /* the decoding process is already stopped */
} JPG_HW_STOP_STATE_T;

typedef VOID (*JPG_HW_NFY_FCT_T)
(
    UINT32      ui4_img_id,         /* the image id which causes this notification */
    VOID        *pv_tag,            /* tag passed to the callback function */
    VOID        *pv_data,           /* data passed with this notification */
    UINT32      ui4_state);         /* notification state */
/*
 * while ui4_state := JPG_HW_NFY_FILL_BUF,
 * then  pv_data   := (FILL_RING_BUF_T *) 
 *
 * while ui4_state := JPG_HW_NFY_PROGRESS,
 * then  pv_data   := UINT32
 */

typedef enum
{
    JPG_HW_ROTATE_NONE,                 /* no rotation */
    JPG_HW_ROTATE_CW_90,                /* clockwise  90 degrees  */
    JPG_HW_ROTATE_CW_180,               /* clockwise 180 degrees  */
    JPG_HW_ROTATE_CW_270,               /* clockwise 270 degrees  */
    JPG_HW_ROTATE_NONE_WITH_FLIP,       /* no rotation, with flip */
    JPG_HW_ROTATE_CW_90_WITH_FLIP,      /* clockwise  90 degrees, with flip */
    JPG_HW_ROTATE_CW_180_WITH_FLIP,     /* clockwise 180 degrees, with flip */
    JPG_HW_ROTATE_CW_270_WITH_FLIP      /* clockwise 270 degrees, with flip */
} JPG_HW_ROTATE_T;

typedef enum
{
    JPG_FILL_RING_BUF_EVENT = 0
} JPG_HW_NFY_EVENT_TYPE_T;

typedef struct _JPG_HW_DECODED_CLR_BUF_INFO_T
{    
    VOID*          pv_internal;
    SIZE_T         z_MCU_row_num;
    VOID*          pv_y_clr_buf;
    SIZE_T         z_y_clr_buf_len;
    SIZE_T         z_y_pitch;
    SIZE_T         z_y_width;
    SIZE_T         z_y_height;
    VOID*          pv_cb_clr_buf;
    SIZE_T         z_cb_clr_buf_len;
    SIZE_T         z_cb_pitch;
    SIZE_T         z_cb_width;
    SIZE_T         z_cb_height;
    VOID*          pv_cr_clr_buf;
    SIZE_T         z_cr_clr_buf_len;
    SIZE_T         z_cr_pitch;
    SIZE_T         z_cr_width;
    SIZE_T         z_cr_height;
}JPG_HW_DECODED_CLR_BUF_INFO_T;

typedef struct _START_DEC_T
{
    JPG_HW_DECODED_CLR_BUF_INFO_T*      pt_clr_buf;
    INT32                               i4_ret;
} START_DEC_T;

typedef struct _ENTRY_BUF_T
{
    BOOL            b_is_valid;         /* indicates if this entry is a valid one */
    VOID*           pv_start_add;       /* starting address of the buffer */
    UINT32          ui4_required_len;   /* required length to be filled this time */
    UINT32          ui4_filled_len;     /* [OUT] the data filled this time */
} ENTRY_BUF_T;

typedef struct _FILL_RING_BUF_T
{
    ENTRY_BUF_T     t_entry_1;          /* entry 1 for the buffer */
    ENTRY_BUF_T     t_entry_2;          /* entry 2 for the buffer */
    BOOL            b_file_end;         /* [OUT] indicate if EOF is reached */
} FILL_RING_BUF_T;


/* Huffman coding tables. */
typedef struct _HUFF_TBL_T
{
  /* These two fields directly represent the contents of a JPEG DHT marker */
  UINT8 aui1_bits[17];        /* bits[k] = # of symbols with codes of */
  /* length k bits; bits[0] is unused */
  UINT8 aui1_huffval[256];        /* The symbols, in order of incr code length */
  /* This field is used only during compression.  It's initialized FALSE when
   * the table is created, and set TRUE when it's been output to the file.
   * You could suppress output of a table by setting this to TRUE.
   */
} HUFF_TBL_T;

/* Basic info about one component (color channel). */
typedef struct _COMPONENT_INFO_T
{
  /* These values are fixed over the whole image. */
  /* For compression, they must be supplied by parameter setup; */
  /* for decompression, they are read from the SOF marker. */
  INT16 i2_component_id;        /* identifier for this component (0..255) */
  INT16 i2_component_index;    /* its index in SOF or cinfo->comp_info[] */
  INT16 i2_h_samp_factor;        /* horizontal sampling factor (1..4) */
  INT16 i2_v_samp_factor;        /* vertical sampling factor (1..4) */
  INT16 i2_quant_tbl_no;        /* quantization table selector (0..3) */
  /* These values may vary between scans. */
  /* For compression, they must be supplied by parameter setup; */
  /* for decompression, they are read from the SOS marker. */
  /* The decompressor output side may not use these variables. */
  INT16 i2_dc_tbl_no;        /* DC entropy table selector (0..3) */
  INT16 i2_ac_tbl_no;        /* AC entropy table selector (0..3) */

  /* Remaining fields should be treated as private by applications. */

  /* These values are computed during compression or decompression startup: */
  /* Component's size in DCT blocks.
   * Any dummy blocks added to complete an MCU are not counted; therefore
   * these values do not depend on whether a scan is interleaved or not.
   */
  UINT16 ui2_width_in_blocks;
  UINT16 ui2_height_in_blocks;

  /* This flag is used only for decompression.  In cases where some of the
   * components will be ignored (eg grayscale output from YCbCr image),
   * we can skip most computations for the unused components.
   */
  BOOL b_component_needed;    /* do we need the value of this component? */

  /* These values are computed before starting a scan of the component. */
  /* The decompressor output side may not use these variables. */
  INT16 i2_MCU_width;        /* number of blocks per MCU, horizontally */
  INT16 i2_MCU_height;        /* number of blocks per MCU, vertically */
  INT16 i2_MCU_blocks;        /* MCU_width * MCU_height */

  INT16 i2_last_row_height;    /* # of non-dummy blocks down in last MCU */

  /* Saved quantization table for component; NULL if none yet saved.
   * See jdinput.c comments about the need for this information.
   * This field is currently used only for decompression.
   */
  UINT16 aui2_quant_table[DCTSIZE2];
  BOOL b_quant_table_saved;    /* We save quantization table yet ? */
  /* Private per-component storage for DCT or IDCT subsystem. */
  INT16 ai2_dct_table[DCTSIZE2];
} COMPONENT_INFO_T;

typedef struct _RING_BUFFER
{
    HANDLE_T    h_buf_lock;
    UCHAR*        puc_buf;
    UINT32          ui4_head;
    UINT32          ui4_tail;
    UINT32          ui4_valid_length;
    UINT32          ui4_buf_length;
} RING_BUFFER;

typedef struct _JPG_HW_DEC_SETUP_INFO_T
{   
    HANDLE_T                                h_img;
    HUFF_TBL_T*                             pt_dc_huff_tbl_array;
    HUFF_TBL_T*                             pt_ac_huff_tbl_array;
    RING_BUFFER*                            pt_prefetch_ring_buf;
    UINT16**                                ppui2_quantval;
    UINT16                                  ui2_restart_interval;
    UINT16                                  ui2_num_components;
    COMPONENT_INFO_T*                       pt_comp_info_array; 
    UINT8                                   ui1_comps_in_scan;
    UINT8                                   ui1_Ss;
    UINT8                                   ui1_Se;
    UINT8                                   ui1_Ah;
    UINT8                                   ui1_Al;
} JPG_HW_DEC_SETUP_INFO_T;

/* data passed with JPG_HW_SET_TYPE_INIT */
typedef struct
{
    JPG_HW_NFY_FCT_T    pf_func;        /* callback function */
    VOID                *pv_tag;        /* tag passed to the callback function */
} JPG_HW_INIT_T;

/* data passed with JPG_HW_SET_TYPE_DECODE */
typedef struct
{
    UINT32          ui4_img_id;         /* the image id decoded */

    INT32           i4_src_x;           /* x offset in the source image in pixels */
    INT32           i4_src_y;           /* y offset in the source image in pixels */
    UINT32          ui4_src_width;      /* width to be decoded in pixels */
    UINT32          ui4_src_height;     /* height to be decoded in pixels */

    VOID            *pv_dst;            /* destination starting address */
    INT32           i4_dst_x;           /* x offset in the destination in pixels */
    INT32           i4_dst_y;           /* y offset in the destination in pixels */
    UINT32          ui4_dst_width;      /* expected output width in pixels */
    UINT32          ui4_dst_height;     /* expected output height in pixels */
    UINT32          ui4_dst_pitch;      /* pitch of the destination image */
    GFX_COLORMODE_T e_dst_cm;           /* destination color mode */
    
    JPG_HW_ROTATE_T e_rotate;           /* rotation option */
} JPG_HW_DECODE_T;

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* _X_JPG_HW_H_ */

