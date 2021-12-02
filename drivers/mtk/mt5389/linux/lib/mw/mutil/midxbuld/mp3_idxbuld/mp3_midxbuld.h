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
 * $RCSfile: mp3_midxbuld.h,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/9 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 907020e011d0dbd7ca156869d249b742 $ Asa
 *
 * Description:
 *         This file exports all media index builder API and constant for
 *         other module using.
 *---------------------------------------------------------------------------*/

#ifndef _MP3_MIDXBULD_H_
#define _MP3_MIDXBULD_H_


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#include "inc/u_common.h"
#include "handle/u_handle.h"
#include "mutil/midxbuld/midxbuld.h"

#else

#include "u_common.h"
#include "u_handle.h"
#include "../midxbuld.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

#define IS_INPUT_OK(i4_ret)                        \
{                                                  \
    INT32    _i4_ret = (i4_ret);                  \
    if (INPUTR_OK != _i4_ret && INPUTR_EOF != _i4_ret) \
    {                                              \
        return MIDXBULDR_INTERNAL_ERR;                 \
    }                                              \
}

typedef enum
{
    MP3_IDXBULD_LAYER_I,
    MP3_IDXBULD_LAYER_II,
    MP3_IDXBULD_LAYER_III,
    MP3_IDXBULD_LAYER_NONE
} MP3_IDXBULD_LAYER_T;

typedef enum
{
    MP3_IDXBULD_VERSION_1,
    MP3_IDXBULD_VERSION_2,
    MP3_IDXBULD_VERSION_NONE,
    MP3_IDXBULD_VERSION_2_5
}MP3_IDXBULD_VERSION_T;

/* This structure holds the obj of mp3 idxbuild info. */
typedef struct _MP3_IDXBULD_OBJ_T
{
    UINT8       ui1_version;
    UINT8       ui1_layer;
    UINT32      ui4_sample_rate;
    UINT32                       ui4_frame_slots;
    UINT8                        ui1_slot_bytes;
    UINT64                       ui8_frame_duration;

    MIDXBULD_KEY_TBL_EXT_T       t_tbl_ext;
    UINT64      ui8_mp3_length_in_byte;
} MP3_IDXBULD_OBJ_T;

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/* This function open a handler for a file */
extern INT32 mp3_midxbuld_fopen (
                        CHAR*                  ps_path_name,
                        MIDXBULD_KEY_INFO_T*   pt_key_info,
    VOID*                      pv_obj
);

/* This function open a handler for a memory buffer */
extern INT32 mp3_midxbuld_mopen (
                        VOID*                  pv_mem_data,
                        SIZE_T                 z_mem_leng,
                        MIDXBULD_KEY_INFO_T*   pt_key_info,
    VOID*                      pv_obj
);

/* This function close a handler */
extern INT32 mp3_midxbuld_close (
    VOID*                      pv_obj
);

/* This function start indexing */
extern INT32 mp3_midxbuld_start_indexing (
                        VOID*                pv_obj,
    UINT32*                     pui4_range_id
);

/* This API retrieves the information of media idx builder handle. It will return MINFO_NOT_READY, before parse media complete. */
extern INT32 mp3_midxbuld_get_info (
                        VOID*                       pv_obj,
                        MIDXBULD_GET_INFO_TYPE_T    e_info_type,
                        VOID*                       pv_buf,
    SIZE_T*                     pz_buf_leng
);

/* This API retrieves the information of media idx builder handle. It will return MINFO_NOT_READY, before parse media complete. */
extern INT32 mp3_midxbuld_set_info
(
                        VOID*                        pv_obj,
                        MIDXBULD_SET_INFO_TYPE_T     e_info_type,
                        VOID*                        pv_buf,
    SIZE_T*                     pz_buf_leng
);

#ifdef __cplusplus
}
#endif

#endif /* _MP3_MIDXBULD_H_ */

