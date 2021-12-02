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
 * aif_parser.h,v
 * 1.1.2.1 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * BingsaiJin
 * $CCRevision: /main/DTV_X_HQ_int/2 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 0916b8010fb2d9ebf453b3d4efdbc5e9 $ Asa
 *
 * Description: 
 *         This file exports public API of the aif parser function
 *---------------------------------------------------------------------------*/

#ifndef _AIF_PARSER_H_
#define _AIF_PARSER_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#include "inc/u_common.h"
#include "handle/u_handle.h"
#include "mutil/minfo/minfo.h"
#include "mutil/minfo/minfo_dbg.h"
#include "mutil/mfmtrecg/u_mfmtrecg.h"

#else

#include "u_common.h"
#include "u_handle.h"
#include "../minfo.h"
#include "../minfo_dbg.h"
#include "../../mfmtrecg/u_mfmtrecg.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
----------------------------------------------------------------------------*/
#define dwBigFourCC(a,b,c,d)  ((((UINT32)a)<<24)|((b)<<16)|((c)<<8)|(d))
        
#define AIF_CK_FORM    dwBigFourCC('F', 'O', 'R', 'M')/*IFF file*/
#define AIF_CK_AIFF    dwBigFourCC('A', 'I', 'F', 'F')/*AIFF file*/
#define AIF_CK_AIFC    dwBigFourCC('A', 'I', 'F', 'C')/*AIFC file*/
#define AIF_CK_COMM    dwBigFourCC('C', 'O', 'M', 'M')/*Common chunk*/
#define AIF_CK_SSND    dwBigFourCC('S', 'S', 'N', 'D')/*Sound Data Chunk*/
#define AIF_CK_MARK    dwBigFourCC('M', 'A', 'R', 'K')/*Marker Chunk*/
#define AIF_CK_MIDI    dwBigFourCC('M', 'I', 'D', 'I')/*MIDI Data Chunk*/
#define AIF_CK_INST    dwBigFourCC('I', 'N', 'S', 'T')/*Instrument Chunk*/
#define AIF_CK_AESD    dwBigFourCC('A', 'E', 'S', 'D')/*Audio Recording Chunk*/
#define AIF_CK_APPL    dwBigFourCC('A', 'P', 'P', 'L')/*Application Specific Chunk*/
#define AIF_CK_COMT    dwBigFourCC('C', 'O', 'M', 'T')/*Comments Chunk*/
#define AIF_CK_NAME    dwBigFourCC('N', 'A', 'M', 'E')/*Name Chunk*/
#define AIF_CK_AUTH    dwBigFourCC('A', 'U', 'T', 'H')/*Author Chunk*/
#define AIF_CK_CPRT    dwBigFourCC('(', 'c', ')', ' ')/*Copyright Chunk*/
#define AIF_CK_ANNO    dwBigFourCC('A', 'N', 'N', 'O')/*Annotation Chunk*/

#define AIF_BOX_BUF_LENGTH                  1024
#define AIF_STREAM_MAX                      0xFF

#define PROP_FLAG_PN_SAVE_ENABLED           0x0001
#define PROP_FLAG_PN_PERFECT_PLAY_ENABLED   0x0002
#define PROP_FLAG_PN_LIVE_BROADCAST         0x0004

#define DATA_PKT_PN_RELIABLE_FLAG           0x0001
#define DATA_PKT_PN_KEYFRAME_FLAG           0x0002

#define NO_LOOPING                          0
#define FORWARD_LOOPING                     1
#define FORWARD_BACKWARD_LOOPING            2
typedef enum
{
    AIF_STRM_TYPE_UNKNOWN,
    AIF_STRM_TYPE_VIDEO,
    AIF_STRM_TYPE_AUDIO,
    AIF_STRM_TYPE_SUBTL
}AIF_STRM_TYPE_T;

typedef struct _AIF_FILE_HEADER_T 
{
    UINT32      ui4_ck_id;                             /* "FORM" */
    UINT32      ui4_ck_size;
    UINT32      ui4_type;                            /* "AIFF" */
}AIF_FILE_HEADER_T;

typedef struct _AIF_COMM_CK_T                        /*Common Chunk*/
{
    UINT32      ui4_ck_id;                            /*COMM*/
    UINT32      ui4_ck_size;
    UINT16      ui2_num_channels;
    UINT32      ui4_num_sample_frames;
    UINT16      ui2_sample_size;
    UINT32      ui4_sample_rate;
    UINT32      ui4_sample_rate_scale;
    UINT32      ui4_comp_type;
    UINT8*      pui1_comp_name;
    UINT16      ui2_pad_data;
}AIF_COMM_CK_T;

typedef struct _AIF_SSND_CK_T                        /*Sound Data Chunk*/
{
    UINT32      ui4_ck_id;                           /*SSND*/
    UINT32      ui4_ck_size;
    UINT32      ui4_offset;
    UINT32      ui4_block_size;
    UINT32      *pui4_waveform_data;
}AIF_SSND_CK_T;

typedef struct _AIF_MARK_T                            
{
    UINT16      ui2_mk_id;       
    UINT32      ui4_mk_pos;
    UINT8       aui1_mk_name[256];
    
}AIF_MARK_T;

typedef struct _AIF_MARK_CK_T                        /*Marker Chunk*/
{
    UINT32      ui4_ck_id;                           /*MARK*/
    UINT32      ui4_ck_size;
    UINT16      ui2_num_markers;
    AIF_MARK_T  *pt_markers;
}AIF_MARK_CK_T;

typedef struct _AIF_LOOP_T
{
    UINT16        ui2_play_mode;
    UINT16        ui2_begin_loop;
    UINT16        ui2_end_loop;
}AIF_LOOP_T;

typedef struct _AIF_INST_CK_T                        /*Instrument Chunk*/
{
    UINT32      ui4_ck_id;                             /*INST*/
    UINT32      ui4_ck_size;
    UINT8       ui1_base_note;
    UINT8       ui1_detune;
    UINT8       ui1_low_note;
    UINT8       ui1_high_note;
    UINT8       ui1_low_velocity;
    UINT8       ui1_high_velocity;
    UINT16      ui1_gain;
    AIF_LOOP_T  r_sustain_loop;
    AIF_LOOP_T  r_release_loop;
}AIF_INST_CK_T;

typedef struct _AIF_MIDI_CK_T                        /*MIDI Data Chunk*/
{
    UINT32      ui4_ck_id;                             /*MIDI*/
    UINT32      ui4_ck_size;
    UINT8       *pui1_midi_data;
}AIF_MIDI_CK_T;

typedef struct _AIF_AESD_CK_T                        /*Audio Recording Chunk*/
{
    UINT32      ui4_ck_id;                             /*AESD*/
    UINT32      ui4_ck_size;
    UINT8       aui1_aes_ch_status[24];
}AIF_AESD_CK_T;

typedef struct _AIF_APPL_CK_T                        /*Application Specific Chunk*/
{
    UINT32      ui4_ck_id;                             /*APPL*/
    UINT32      ui4_ck_size;
    UINT8       aui1_appl_signature[4];
    UINT8       *pui1_data;
}AIF_APPL_CK_T;

typedef struct _AIF_COMT_T
{
    UINT32        ui4_time_stamp;
    UINT16        ui2_marker;
    UINT16        ui2_count;
    UINT8         *pui1_text;
}AIF_COMT_T;

typedef struct _AIF_COMT_CK_T                        /*Comments Chunk*/
{
    UINT32      ui4_ck_id;                             /*COMT*/
    UINT32      ui4_ck_size;
    UINT16      ui2_num_comments;
    AIF_COMT_T  *pt_comments;
}AIF_COMT_CK_T;

typedef struct _AIF_TEXT_CK_T                        /*Text Chunks*/
{
    UINT32      ui4_ck_id;                            /*NAME|AUTH|ANNO|(c) */
    UINT32      ui4_ck_size;
    UINT8       *pui1_text;
}AIF_TEXT_CK_T;

typedef struct _AIF_IFF_CK_T                        /*General IFF Chunks*/
{
    UINT32      ui4_ck_id;                            /*any IFF chunks*/
    UINT32      ui4_ck_size;
}AIF_IFF_CK_T;

typedef struct _AIF_FILE_LAYOUT_T
{
    AIF_FILE_HEADER_T               r_file_head;                     
    AIF_COMM_CK_T                   r_comm_ck;
    AIF_SSND_CK_T                   r_ssnd_ck;                 
    AIF_MARK_CK_T                   r_mark_ck;               
    AIF_INST_CK_T                   r_inst_ck;  
    AIF_MIDI_CK_T                   r_midi_ck;                     
    AIF_AESD_CK_T                   r_aesd_ck;                     
    AIF_APPL_CK_T                   r_appl_ck; 
    AIF_COMT_CK_T                   r_comt_ck;     
    AIF_TEXT_CK_T                   r_name_ck; 
    AIF_TEXT_CK_T                   r_auth_ck; 
    AIF_TEXT_CK_T                   r_anno_ck; 
    AIF_TEXT_CK_T                   r_copyright_ck; 
}AIF_FILE_LAYOUT_T;

typedef struct _MINFO_AIF_FILE_INF_T
{
    /* Current parser offset, file base */
    UINT32                             ui4_curr_parser_offset;
    
    /* Generic infomation */
    MINFO_TYPE_GENERIC_T               r_generic_info;

    /* AIF file content */
    AIF_FILE_LAYOUT_T                  r_file_layout;
}MINFO_AIF_FILE_INF_T;


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
extern INT32 aif_file_minfo_parse(MINFO_OBJ_T*    pt_minfo_obj);

#endif /* _AIF_PARSER_H_ */

