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
 * $RCSfile: mp4_common.h,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/2 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 0916b8010fb2d9ebf453b3d4efdbc5e9 $ Asa
 *
 * Description: 
 *         This file exports public API of the mp4 common function
 *---------------------------------------------------------------------------*/

#ifndef _MP4_COMMON_H_
#define _MP4_COMMON_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#include "inc/u_common.h"
#include "handle/u_handle.h"

#else

#include "u_common.h"
#include "u_handle.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define MP4_OK                        ((INT32)    0)

typedef enum
{
    MP4_PARSER_STATE_IDLE,
    MP4_PARSER_STATE_CHECK_TRACK,    
    MP4_PARSER_STATE_LOAD_TRACK_INF,
    MP4_PARSER_STATE_LOAD_VIDEO_TRACK_INF,
    MP4_PARSER_STATE_LOAD_SOUND_TRACK_INF,
    MP4_PARSER_STATE_LOAD_SUBTITLE_TRACK_INF    
}MP4_PARSER_STATE_ENUM_T;

typedef enum 
{
    MP4_BOX_ID_UUID,
    MP4_BOX_ID_FTYP,
    MP4_BOX_ID_MOOV,
    MP4_BOX_ID_MVHD,
    MP4_BOX_ID_TRAK,
    MP4_BOX_ID_TKHD,
    MP4_BOX_ID_MDIA,
    MP4_BOX_ID_MDHD,
    MP4_BOX_ID_HDLR,
    MP4_BOX_ID_MINF,
    MP4_BOX_ID_STBL,
    MP4_BOX_ID_STSD,
    MP4_BOX_ID_ESDS,
    MP4_BOX_ID_STTS,
    MP4_BOX_ID_STSC,
    MP4_BOX_ID_STSZ,
    MP4_BOX_ID_STCO,
    MP4_BOX_ID_STSS,
    MP4_BOX_ID_MDAT,
    MP4_BOX_ID_IODS,
    MP4_BOX_ID_TREF,
    MP4_BOX_ID_NMHD,
    MP4_BOX_ID_DINF,
    MP4_BOX_ID_DREF,
    MP4_BOX_ID_VMHD,
    MP4_BOX_ID_SMHD,
    MP4_BOX_ID_HMHD,
    MP4_BOX_ID_UDTA,
    MP4_BOX_ID_CTTS,
    MP4_BOX_ID_ODSM,
    MP4_BOX_ID_EDTS,
    MP4_BOX_ID_ELST,
    MP4_BOX_ID_CO64,
    MP4_BOX_ID_GMHD,/*mov only*/
    MP4_BOX_ID_LOAD,/*mov only*/
    MP4_BOX_ID_IMAP,/*mov only*/
    MP4_BOX_ID_CODE,/*mov only*/
    MP4_BOX_ID_CMOV,
    MP4_BOX_ID_MVEX,
    MP4_BOX_ID_MAX
}MP4_BOX_ID_ENUM_T;


typedef struct _MP4_BOX_ID_T
{
    CHAR sz_four_cc[5];
    MP4_BOX_ID_ENUM_T  e_box_id;
}MP4_BOX_ID_T;

typedef struct _MP4_BOX_T
{
    MP4_BOX_ID_ENUM_T  e_box_id;
    UINT64              ui8_offset;
    UINT32 ui4_size; 
    struct _MP4_BOX_T* pt_parent_box;
    struct _MP4_BOX_T* pt_next_box;
    struct _MP4_BOX_T* pt_first_sub_box;
}MP4_BOX_T;    

/* used to store cmov information*/
typedef struct _MP4_BOX_CMOV_INFO_T
{
    BOOL    b_is_cmov_box_exist;

    /* raw cmov box info*/
    UINT64  ui8_cmov_box_pos;
    UINT32  ui4_cmov_box_size;
    UINT8*  pui1_raw_cmov_data;
    
    /* decompressed cmov box info*/
    UINT32  ui4_cmov_box_decomp_size;
    UINT8*  pui1_decomp_cmov_data;
} MP4_BOX_CMOV_INFO_T;

typedef struct _PIFF_BOX_INET_INFO_T
{
    UINT64      ui8_moof_start_pos; 
    UINT32      ui4_moof_cnt;
    UINT32*     pui4_moof_pos;
    UINT8*      pui1_moof_buff;
    BOOL        b_is_piff_file;
    BOOL        b_is_inet_data;
    BOOL        b_key_build;   
    BOOL        b_is_end_file;
    BOOL        b_is_mfra_key_exist;
    BOOL        b_is_bivl_file;
    UINT64      ui8_mfra_box_start_pos; 
    UINT32      ui4_mfra_box_size; 
} PIFF_BOX_INET_INFO_T;

/* marlin track information */
typedef struct 
{ 
    UINT8*  pui1_ct_id;              /* octopus content id           ---8id box content         */
    UINT32  ui4_ct_id_length;        /* length of octopus content id ---8id box content length-1*/
    UINT8   ui1_ipmp_id;             /* ipmp descriptor id           ---the byte before 0xA551  */
    UINT32  ui4_satr_algo_type;      /* algorithm type               ---HMAC size==32 ???       */
    UINT8*  pui1_satr_ctm;           /* content metadata,i.e satr_box---satr box content        */
    UINT32  ui4_satr_ctm_length;     /* length of content metadata   ---satr box size-8         */
    UINT8*  pui1_satr_hmac_val;      /* hmac value, i.e satr_hmac, length is 32 bytes */
} MINFO_MP4_MARLIN_TRACK_INFO_T;
/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

extern MP4_BOX_ID_T _r_mp4_box_id_map[MP4_BOX_ID_MAX];

extern BOOL _mp4_box_is_need_check_sub(MP4_BOX_ID_ENUM_T e_id);
extern BOOL _mp4_offset_is_in_this_box(UINT64  ui8_offset,  MP4_BOX_T* pt_box);
extern UINT32 _mp4_swap_4bytes(UINT32 ui4_in);
extern INT32 _mp4_get_par_box_by_offset(MP4_BOX_T*    pt_head_box, 
                                        UINT64        ui8_Offset, 
                                        MP4_BOX_T**   ppt_parent_box);
extern INT32 _mp4_get_last_sub_box(MP4_BOX_T*  pt_par_box, 
                                   MP4_BOX_T** ppt_last_sub_box);
extern INT32 _mp4_get_box_by_id(MP4_BOX_T*            pt_head_box, 
                                MP4_BOX_ID_ENUM_T     e_id,
                                MP4_BOX_T**           ppt_e_id_box);

#endif /* _MP4_COMMON_H_ */
