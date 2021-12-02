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
 * $RCSfile: venc_h264api.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file venc_h264api.h
 *  This header file declares exported APIs of H.264.
 */


#ifndef VENC_H264API_H
#define VENC_H264API_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "fbm_drvif.h"
#include "../venc_debug.h"
#include "venc_util_drvif.h"
#include "venc_drvif.h"
#include "venc_if.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

// Macro VENC_H264_FILL_STUFF_BYTES_BY_FW 
// is used for rate control fitting target bitrate via adding stuff bytes
// if inaccurate bitrate is allowed, mark this macro for better performance
//#define VENC_H264_FILL_STUFF_BYTES_BY_FW


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define VENC_H264_MAX_VIDEO_RESOLUTION_WIDTH (1280)
#define VENC_H264_MAX_VIDEO_RESOLUTION_HEIGHT (736)
#define VENC_H264_MAX_VIDEO_RESOLUTION (VENC_H264_MAX_VIDEO_RESOLUTION_WIDTH * VENC_H264_MAX_VIDEO_RESOLUTION_HEIGHT)
#define VENC_H264_BS_SIZE (0x200000) // for level 3.1, at least 0x1c0000
#define VENC_H264_MAX_COARSE_RES        (58880)  // 1280/4 * 736/4 


#define _fgIsValidPic(type) ((type) >= VENC_H264_PIC_TYPE_SPS && (type) <= VENC_H264_PIC_TYPE_B)  

#define _fgIsRealPic(type) \
    (VENC_H264_PIC_TYPE_IDR == (type) \
    || VENC_H264_PIC_TYPE_I == (type) \
    || VENC_H264_PIC_TYPE_P == (type) \
    || VENC_H264_PIC_TYPE_B == (type))


#define H264_BFRAME_MAX  (2)

#define H264_EMU_PARM (VENC_H264 + 0xFF)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef struct 
{
    //~~ must have the same layout with VENC_OUT_PIC_T
    //data type
    UINT32 u4Type;    //[out]
    UINT32 u4Pts;
    //data 
    UINT32 u4Addr;    // [in]
    UINT32 u4Size;    // [in]
    UINT32 u4RStart;  // [in]ring start
    UINT32 u4REnd;    // [in]ring end

    //EMU
    UINT32 u4SPSAddr;       // [out]
    UINT32 u4SPSSize;       // [out]

    UINT32 u4PPSAddr;       // [out]
    UINT32 u4PPSSize;       // [out]

    UINT32 u4FRMAddr;       // [out]
    UINT32 u4FRMSize;       // [out]
} H264_EMU_OUT_PIC_T;


typedef enum 
{
    H264_REZ_STATE_IDLE,
    H264_REZ_STATE_REZ,
    H264_REZ_STATE_MAX
} H264_REZ_STATE_T;


#define H264_MMR_EMU \
    UINT32 mmr_vui_aspect_ratio_info_present_flag;\
    UINT32 mmr_vui_aspect_ratio_idc;\
    UINT32 mmr_vui_sar_width;\
    UINT32 mmr_vui_sar_height;\
    UINT32 mmr_vui_overscan_info_present_flag;\
    UINT32 mmr_vui_overscan_appropriate_flag;\
    UINT32 mmr_vui_video_signal_type_present_flag;\
    UINT32 mmr_vui_video_format;\
    UINT32 mmr_vui_video_full_range_flag;\
    UINT32 mmr_vui_colour_description_present_flag;\
    UINT32 mmr_vui_colour_primaries;\
    UINT32 mmr_vui_transfer_characteristics;\
    UINT32 mmr_vui_matrix_coefficients;\
    UINT32 mmr_vui_chroma_location_info_present_flag;\
    UINT32 mmr_vui_chroma_sample_loc_type_top_field;\
    UINT32 mmr_vui_chroma_sample_loc_type_bottom_field;\
    UINT32 mmr_vui_timing_info_present_flag;\
    UINT32 mmr_vui_num_units_in_tick;\
    /*UINT32 mmr_vui_time_scale;*/\
    UINT32 mmr_vui_fixed_frame_rate_flag;\
    UINT32 mmr_vui_nal_hrd_parameters_present_flag;\
    UINT32 mmr_vui_nal_bit_rate_scale;\
    UINT32 mmr_vui_nal_cpb_size_scale;\
    UINT32 mmr_vui_nal_bit_rate_value_minus1;\
    UINT32 mmr_vui_nal_cpb_size_value_minus1;\
    UINT32 mmr_vui_nal_vbr_cbr_flag;\
    UINT32 mmr_vui_nal_initial_cpb_removal_delay_length_minus1;\
    UINT32 mmr_vui_nal_cpb_removal_delay_length_minus1;\
    UINT32 mmr_vui_nal_dpb_output_delay_length_minus1;\
    UINT32 mmr_vui_nal_time_offset_length;\
    UINT32 mmr_vui_vcl_hrd_parameters_present_flag;\
    UINT32 mmr_vui_vcl_bit_rate_scale;\
    UINT32 mmr_vui_vcl_cpb_size_scale;\
    UINT32 mmr_vui_vcl_bit_rate_value_minus1;\
    UINT32 mmr_vui_vcl_cpb_size_value_minus1;\
    UINT32 mmr_vui_vcl_vbr_cbr_flag;\
    UINT32 mmr_vui_vcl_initial_cpb_removal_delay_length_minus1;\
    UINT32 mmr_vui_vcl_cpb_removal_delay_length_minus1;\
    UINT32 mmr_vui_vcl_dpb_output_delay_length_minus1;\
    UINT32 mmr_vui_vcl_time_offset_length;\
    UINT32 mmr_vui_low_delay_hrd_flag;\
    UINT32 mmr_vui_pic_struct_present_flag;\
    UINT32 mmr_vui_bitstream_restriction_flag;\
    UINT32 mmr_vui_motion_vectors_over_pic_boundaries_flag;\
    UINT32 mmr_vui_max_bytes_per_pic_denom;\
    UINT32 mmr_vui_max_bits_per_mb_denom;\
    UINT32 mmr_vui_log2_max_mv_length_vertical;\
    UINT32 mmr_vui_log2_max_mv_length_horizontal;\
    UINT32 mmr_vui_num_reorder_frames;\
    UINT32 mmr_vui_max_dec_frame_buffering;


typedef struct _H246_MMR_PARAM
{
    // MMR variables start
    UINT32 mmr_period_i_frm_10; 
    UINT32 mmr_period_idr_frm_10; 
    UINT32 mmr_num_of_b_frm_2; 
    UINT32 mmr_bitstream_format_1;
    UINT32 mmr_h264_profile_8;
    UINT32 mmr_h264_level_6;
    UINT32 mmr_dram_page_size_div256_7;
    UINT32 mmr_gen_b_rec_frm_1;
    UINT32 mmr_use_copy_b_1;
    UINT32 mmr_use_mbaff_1;
    UINT32 mmr_use_cabac_1;
    UINT32 mmr_quant_i_frm_6;
    UINT32 mmr_quant_p_frm_6;
    UINT32 mmr_quant_b_frm_6;
    UINT32 mmr_en_p_interskip_1;
    UINT32 mmr_en_p_inter16x16_1;
    UINT32 mmr_en_p_inter16x08_1;
    UINT32 mmr_en_p_inter08x16_1;
    UINT32 mmr_en_p_inter08x08_1;
    UINT32 mmr_en_b_interdirect_1;
    UINT32 mmr_en_b_inter16x16_1;
    UINT32 mmr_en_b_inter16x08_1;
    UINT32 mmr_en_b_inter08x16_1;
    UINT32 mmr_en_b_inter08x08_1;
    UINT32 mmr_p_sr_h_div16_3;
    UINT32 mmr_p_sr_v_div16_2;
    UINT32 mmr_b_sr_h_div16_2;
    UINT32 mmr_b_sr_v_div16_2;
    UINT32 mmr_use_coarse_search_1;
    UINT32 mmr_p_coarse_sr_h_div8_3;
    UINT32 mmr_p_coarse_sr_v_div8_2;
    UINT32 mmr_b_coarse_sr_h_div8_2;
    UINT32 mmr_b_coarse_sr_v_div8_2;
    UINT32 mmr_use_rc_1;
    UINT32 mmr_rc_use_cbr_1;
    UINT32 mmr_rc_use_init_qp_1;
    UINT32 mmr_rc_target_bit_rate_14;
    UINT32 mmr_fps_8;
    UINT32 mmr_p_frm_q_limiter_8;
    UINT32 mmr_b_frm_q_limiter_8;

    UINT32 mmr_source_scan_type_2;
    UINT32 mmr_pitch_width_in_pels;

    UINT32 mmr_image_crop_right_4;
    UINT32 mmr_image_crop_bottom_5;
    UINT32 mmr_image_width_div16_7;
    UINT32 mmr_image_height_div16_6;
    UINT32 mmr_num_mb_in_frm_12;
    UINT32 mmr_coarse_image_crop_bottom_3;
    UINT32 mmr_coarse_image_width_div8_6;
    UINT32 mmr_coarse_image_height_div8_5;
    UINT32 mmr_num_blk_in_coarse_frm_10;
    UINT32 mmr_ini_bitstream_dram_byte_div8_26;
    UINT32 mmr_max_bitstream_byte_size_29;
    UINT32 mmr_max_pic_bitstream_byte_cnt_22;
    //    UINT32 mmr_frm_buf_y_dram_byte_addr_div8_26 [8];
    //    UINT32 mmr_frm_buf_uv_dram_byte_addr_div8_26 [8];
    UINT32 mmr_upper_mb_info_dram_byte_addr_div8_26;
    UINT32 mmr_col_info_dram_byte_addr_div8_26;
    //    UINT32 mmr_coarse_frm_buf_y_dram_byte_addr_div8_26 [7];
    UINT32 mmr_coarse_multiplier_7;

    UINT32 mmr_crc_check_enable_1;
    UINT32 mmr_enable_vui_support_1;
    UINT32 mmr_vui_time_scale;
    
    UINT32 mmr_rc_adaptive_fps;
    UINT32 mmr_rc_adaptive_tbr;
    UINT32 use_stuff_byte;
#ifdef VENC_EMU
    H264_MMR_EMU
    UINT32 dwNumOfIandPFrames;
    UINT32 mmr_end;
#endif
    // MMR variables end

    //coded picture type and display type begin
    BOOL write_pic_timing;
    UINT8 vui_pic_struct_present_flag;
    UINT8 coded_picture_type;
    UINT8 picture_display_type;
    //coded picture type and display type end
}H264_MMR_PARAM_T;

typedef struct 
{
    UINT32 u4CosAddr;
}H246_PIC_PRAM;



typedef struct _H246_CTRL
{
    //Coarse 
    H264_REZ_STATE_T eRezFrmState;
    VENC_STATE_T     eRezCmdState;
    HANDLE_T    hRezThread;
    HANDLE_T    hRezCmdQ;
    HANDLE_T    hRezFrmQ;
    UINT16      u2SrcWidth;
    UINT16      u2SrcHeidht;
    UINT16      u2DesWidth;
    UINT16      u2DesHeidht;
    UINT16      u2DesPitch;
    BOOL        fgCoarse;
    VENC_REZ_OBJ_T *ptRezObj;
    VENC_BUFMGR_BASE_T *ptCoarseBuffMgr;

    //encode flow 
    UINT32      u4FrameNo;
    BOOL        fg1stHeader;
    BOOL        fgSkipHeaderOnce;
    BOOL        fgKeyframe;
    UINT8       u1RepeatHeader;
    BOOL        fgWriteSEI;
    BOOL        fgForceIDR;

    VENC_H264_PIC_TYPE eType;
    UINT8       u1BFrameNo;
    UINT8       u1BFrmIdx;    

    VENC_IN_PIC_T  uaIn[H264_BFRAME_MAX];
    VENC_OUT_PIC_T uaOut[H264_BFRAME_MAX];
    H246_PIC_PRAM  uaPram[H264_BFRAME_MAX];

    UINT8 u1BSeqIdx;
    UINT8 u1RefSeqIdx;

    UINT32 u4FrefCSAddr;
    UINT32 u4BrefCSAddr;

    UINT16 u2IDRFrmPeriod;
    UINT16 u2IDRFrmIdx;

    UINT16 u2IFrmPeriod;
    UINT16 u2IFrmIdx;

    UINT32 u4RCUAddr;
    UINT32 u4RCUCodeSize;

    //working buffer
    UINT32 u4CFrmBuffAddr[7];
    UINT32 u4YFrmBuffAddr[2];
    UINT32 u4UVFrmBuffAddr[2];

    UINT32 u4InitWp;
    UINT32 u4BsSa;
    UINT32 u4BsSz;
    UINT32 u4BsEa;
    UINT32 u4NaluBufSa;
    UINT32 u4NaluBufSz;
    UINT32 u4NaluBufEa;
    UINT32 u4NaluBufWp;
    UINT32 u4RomCodeAddr;
    UINT32 u4UpperInfoMBAddr;
    UINT32 u4ColInfoAddr;
    UINT32 u4CSInfoAddr;
    UINT32 u4NextCpSa;
    UINT32 u4DumpInfoAddr;

    BOOL   fgEMUDirectMode;
   
    //debug info
    UINT32 u4SPScrc;
    UINT32 u4PPScrc;
    UINT32 u4FRMcrc;
} H264_CTRL_T;


//-----------------------------------------------------------------------------
// Prototype  of inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Prototype  of public functions
//-----------------------------------------------------------------------------
VENC_OBJECT_T *H264Open(void);
VOID _H264DumpBst(UINT8 *pu1Buf, UINT32 u4BufSz);



#endif // VDEC_H264API_H


