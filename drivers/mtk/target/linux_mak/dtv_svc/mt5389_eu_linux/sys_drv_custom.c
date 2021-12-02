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
 * $RCSfile: sys_drv_custom.c,v $
 * $Revision: #3 $
 * $Date: 2012/05/31 $
 * $Author: jifeng.fan $
 *
 * Description:
 *         This file contains application initialization.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_sys_name.h"
#include "d_drv_cust.h"
#include "x_start_common.h"
#include "x_os.h"
#include "util/ssma.h"

VOID* g_pv_drv_config; /* MHF Linux - jzuo */

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define MAKE_AUD_DEC_DRV_ID(id)       { DRVT_AUD_DEC,        id }
#define MAKE_TVD_DRV_ID(id)           { DRVT_TV_DEC,         id }

#define MAKE_AVC_COMP_VID_DRV_ID(id)  { DRVT_AVC_COMP_VIDEO, id }
#define MAKE_AVC_S_VIDEO_DRV_ID(id)   { DRVT_AVC_S_VIDEO,    id }
#define MAKE_AVC_Y_PB_PR_DRV_ID(id)   { DRVT_AVC_Y_PB_PR,    id }
#define MAKE_AVC_VGA_DRV_ID(id)       { DRVT_AVC_VGA,        id }
#define MAKE_AVC_SCART_DRV_ID(id)     { DRVT_AVC_SCART,      id }
#define MAKE_AVC_DVI_DRV_ID(id)       { DRVT_AVC_DVI,        id }
#define MAKE_AVC_HDMI_DRV_ID(id)      { DRVT_AVC_HDMI,       id }
#define MAKE_AVC_AUDIO_INP_DRV_ID(id) { DRVT_AVC_AUDIO_INP,  id }
#define MAKE_AVC_SPDIF_DRV_ID(id)     { DRVT_AVC_SPDIF,      id }
#define MAKE_AVC_COMBI_DRV_ID(id)     { DRVT_AVC_COMBI,      id }
#define MAKE_AVC_SCART_OUT_DRV_ID(id) { DRVT_AVC_SCART_OUT,  id }

#define MAKE_TUNER_SAT_DIG_DRV_ID(id) { DRVT_TUNER_SAT_DIG,  id }
#define MAKE_TUNER_CAB_DIG_DRV_ID(id) { DRVT_TUNER_CAB_DIG,  id }
#define MAKE_TUNER_TER_DIG_DRV_ID(id) { DRVT_TUNER_TER_DIG,  id }
#define MAKE_TUNER_CAB_DIG_OOB_RX_DRV_ID(id) { DRVT_TUNER_CAB_DIG_OOB_RX, id }
#define MAKE_TUNER_SAT_ANA_DRV_ID(id) { DRVT_TUNER_SAT_ANA,  id }
#define MAKE_TUNER_CAB_ANA_DRV_ID(id) { DRVT_TUNER_CAB_ANA,  id }
#define MAKE_TUNER_TER_ANA_DRV_ID(id) { DRVT_TUNER_TER_ANA,  id }

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
extern VOID asp_ratio_normal_ex_fct     (const DRV_CUSTOM_RESOLUTION_INFO_T*      pt_src_res,
                                      const DRV_CUSTOM_RESOLUTION_INFO_T*      pt_dst_res,
                                      const DRV_CUSTOM_VID_SRC_INFO_T*         pt_vid_src_info,
                                      VID_PLA_VID_REGION_T*                    pt_src_reg,
                                      VID_PLA_VID_REGION_T*                    pt_dst_reg);

extern VOID asp_ratio_letterbox_ex_fct  (const DRV_CUSTOM_RESOLUTION_INFO_T*      pt_src_res,
                                      const DRV_CUSTOM_RESOLUTION_INFO_T*      pt_dst_res,
                                      const DRV_CUSTOM_VID_SRC_INFO_T*         pt_vid_src_info,
                                      VID_PLA_VID_REGION_T*                    pt_src_reg,
                                      VID_PLA_VID_REGION_T*                    pt_dst_reg);

extern VOID asp_ratio_pan_scan_ex_fct   (const DRV_CUSTOM_RESOLUTION_INFO_T*      pt_src_res,
                                      const DRV_CUSTOM_RESOLUTION_INFO_T*      pt_dst_res,
                                      const DRV_CUSTOM_VID_SRC_INFO_T*         pt_vid_src_info,
                                      VID_PLA_VID_REGION_T*                    pt_src_reg,
                                      VID_PLA_VID_REGION_T*                    pt_dst_reg);

extern VOID asp_ratio_panorama_ex_fct   (const DRV_CUSTOM_RESOLUTION_INFO_T*      pt_src_res,
                                      const DRV_CUSTOM_RESOLUTION_INFO_T*      pt_dst_res,
                                      const DRV_CUSTOM_VID_SRC_INFO_T*         pt_vid_src_info,
                                      VID_PLA_VID_REGION_T*                    pt_src_reg,
                                      VID_PLA_VID_REGION_T*                    pt_dst_reg);

extern VOID asp_ratio_dot_by_dot_ex_fct (const DRV_CUSTOM_RESOLUTION_INFO_T*      pt_src_res,
                                      const DRV_CUSTOM_RESOLUTION_INFO_T*      pt_dst_res,
                                      const DRV_CUSTOM_VID_SRC_INFO_T*         pt_vid_src_info,
                                      VID_PLA_VID_REGION_T*                    pt_src_reg,
                                      VID_PLA_VID_REGION_T*                    pt_dst_reg);

extern VOID asp_ratio_auto_ex_fct       (const DRV_CUSTOM_RESOLUTION_INFO_T*      pt_src_res,
                                      const DRV_CUSTOM_RESOLUTION_INFO_T*      pt_dst_res,
                                      const DRV_CUSTOM_VID_SRC_INFO_T*         pt_vid_src_info,
                                      VID_PLA_VID_REGION_T*                    pt_src_reg,
                                      VID_PLA_VID_REGION_T*                    pt_dst_reg);

extern VOID asp_ratio_no_video_fct   (const DRV_CUSTOM_RESOLUTION_INFO_T*      pt_src_res,
                                      const DRV_CUSTOM_RESOLUTION_INFO_T*      pt_dst_res,
                                      const DRV_CUSTOM_VID_SRC_INFO_T*         pt_vid_src_info,
                                      VID_PLA_VID_REGION_T*                    pt_src_reg,
                                      VID_PLA_VID_REGION_T*                    pt_dst_reg);
/*-----------------------------------------------------------------------------
                    data declaraions
 ----------------------------------------------------------------------------*/

static DRV_ID_T at_aud_dec_drv_ids[] =
{
    MAKE_AUD_DEC_DRV_ID(0),

    /* DO NOT REMOVE THIS LINE */
    DRV_ID_END
};

static DRV_ID_T at_tvd_drv_ids[] =
{
    MAKE_TVD_DRV_ID(0),
    MAKE_TVD_DRV_ID(1),
#ifdef MW_TV_AV_BYPASS_SUPPORT
    MAKE_TVD_DRV_ID(2),
    MAKE_TVD_DRV_ID(3),
#endif

    /* DO NOT REMOVE THIS LINE */
    DRV_ID_END
};

static DRV_ID_T at_tvd_n_aud_dec_drv_ids[] =
{
    MAKE_TVD_DRV_ID(0),
    MAKE_TVD_DRV_ID(1),
#ifdef MW_TV_AV_BYPASS_SUPPORT
    MAKE_TVD_DRV_ID(2),
    MAKE_TVD_DRV_ID(3),
#endif    
    MAKE_AUD_DEC_DRV_ID(0),

    /* DO NOT REMOVE THIS LINE */
    DRV_ID_END
};

static DRV_ID_T at_scart_sink_src_drv_ids[] =
{
    MAKE_AVC_COMP_VID_DRV_ID(0),
    MAKE_AVC_COMP_VID_DRV_ID(1),
    MAKE_AVC_COMP_VID_DRV_ID(2),
    MAKE_AVC_S_VIDEO_DRV_ID(0),
    MAKE_AVC_S_VIDEO_DRV_ID(1),
    MAKE_AVC_S_VIDEO_DRV_ID(2),
    MAKE_TUNER_TER_ANA_DRV_ID(0),
    MAKE_TUNER_CAB_ANA_DRV_ID(0),
    MAKE_AVC_SCART_DRV_ID(AVC_SCART_TYPE_SOURCE + 0),
    MAKE_AVC_SCART_DRV_ID(AVC_SCART_TYPE_SOURCE + 1),

    /* DO NOT REMOVE THIS LINE */
    DRV_ID_END
};

/*-----------------------------------------------------------------------------
 * AVC Source Customization
 *---------------------------------------------------------------------------*/
static DRV_REG_T at_avc_comps_reg[] =
{
    /* The max size of s_name is 15 BYTES!!!*/
    /*s_name, ui4_hw_addr,      t_drv_id,                                                  pt_connection_ids */
    { "avc_comp_vid_0",   0, MAKE_AVC_COMP_VID_DRV_ID(0),                               at_tvd_drv_ids },
    { "avc_comp_vid_1",   1, MAKE_AVC_COMP_VID_DRV_ID(1),                               at_tvd_drv_ids },
    { "avc_comp_vid_2",   2, MAKE_AVC_COMP_VID_DRV_ID(2),                               at_tvd_drv_ids },
    { "avc_s_vid_0",      0, MAKE_AVC_S_VIDEO_DRV_ID(0),                                at_tvd_drv_ids },
    { "avc_s_vid_1",      1, MAKE_AVC_S_VIDEO_DRV_ID(1),                                at_tvd_drv_ids },
    { "avc_s_vid_2",      2, MAKE_AVC_S_VIDEO_DRV_ID(2),                                at_tvd_drv_ids },
    { "avc_y_pb_pr_0",    0, MAKE_AVC_Y_PB_PR_DRV_ID(0),                                at_tvd_drv_ids },
    { "avc_y_pb_pr_1",    1, MAKE_AVC_Y_PB_PR_DRV_ID(1),                                at_tvd_drv_ids },
    { "avc_hdmi_0",       0, MAKE_AVC_HDMI_DRV_ID(0),                                   at_tvd_n_aud_dec_drv_ids },
    { "avc_hdmi_1",       1, MAKE_AVC_HDMI_DRV_ID(1),                                   at_tvd_n_aud_dec_drv_ids },
    { "avc_hdmi_2",       2, MAKE_AVC_HDMI_DRV_ID(2),                                   at_tvd_n_aud_dec_drv_ids },
    { "avc_hdmi_3",       3, MAKE_AVC_HDMI_DRV_ID(3),                                   at_tvd_n_aud_dec_drv_ids },
    { "avc_vag_0",        0, MAKE_AVC_VGA_DRV_ID(0),                                    at_tvd_drv_ids },
    { "avc_dvi_0",        0, MAKE_AVC_DVI_DRV_ID(0),                                    at_tvd_drv_ids },
    { "avc_sct_inp_0",   0, MAKE_AVC_SCART_DRV_ID(AVC_SCART_TYPE_SOURCE + 0),          at_tvd_n_aud_dec_drv_ids },
    { "avc_sct_io_0",    0, MAKE_AVC_SCART_DRV_ID(AVC_SCART_TYPE_SOURCE_SINK + 0),     at_tvd_n_aud_dec_drv_ids },
    { "avc_sct_pin8_0",  0, MAKE_AVC_SCART_DRV_ID(AVC_SCART_TYPE_MONITOR + 0),         at_tvd_n_aud_dec_drv_ids },
    { "avc_sct_inp_1",   1, MAKE_AVC_SCART_DRV_ID(AVC_SCART_TYPE_SOURCE + 1),          at_tvd_n_aud_dec_drv_ids },
    { "avc_sct_io_1",    1, MAKE_AVC_SCART_DRV_ID(AVC_SCART_TYPE_SOURCE_SINK + 1),     at_tvd_n_aud_dec_drv_ids },    
    { "avc_sct_pin8_1",  1, MAKE_AVC_SCART_DRV_ID(AVC_SCART_TYPE_MONITOR + 1),         at_tvd_n_aud_dec_drv_ids },                    
    { "avc_sct_otp_0",   0, MAKE_AVC_SCART_DRV_ID(AVC_SCART_TYPE_SINK + 0),            at_scart_sink_src_drv_ids },
    { "avc_sct_otp_1",   1, MAKE_AVC_SCART_DRV_ID(AVC_SCART_TYPE_SINK + 1),            at_scart_sink_src_drv_ids },
    { "avc_aud_inp_0",    0, MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_COMP_VID_0), at_aud_dec_drv_ids },
    { "avc_aud_inp_1",    0, MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_COMP_VID_1), at_aud_dec_drv_ids },
    { "avc_aud_inp_2",    0, MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_COMP_VID_2), at_aud_dec_drv_ids },
    { "avc_aud_inp_3",    0, MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_COMP_VID_3), at_aud_dec_drv_ids },
    { "avc_aud_inp_4",    0, MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_S_VID_0),    at_aud_dec_drv_ids },
    { "avc_aud_inp_5",    0, MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_S_VID_1),    at_aud_dec_drv_ids },
    { "avc_aud_inp_6",    0, MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_S_VID_2),    at_aud_dec_drv_ids },
    { "avc_aud_inp_7",    0, MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_YPBPR_0),    at_aud_dec_drv_ids },
    { "avc_aud_inp_8",    0, MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_YPBPR_1),    at_aud_dec_drv_ids },
    { "avc_aud_inp_9",    0, MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_YPBPR_2),    at_aud_dec_drv_ids },
    { "avc_aud_inp_10",   0, MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_VGA_0),      at_aud_dec_drv_ids },
    { "avc_aud_inp_11",   0, MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_VGA_1),      at_aud_dec_drv_ids },
    { "avc_aud_inp_12",   0, MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_HDMI_0),     at_aud_dec_drv_ids },
    { "avc_aud_inp_13",   0, MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_HDMI_1),     at_aud_dec_drv_ids },
    { "avc_aud_inp_14",   0, MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_HDMI_2),     at_aud_dec_drv_ids },
    { "avc_aud_inp_15",   0, MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_HDMI_3),     at_aud_dec_drv_ids },
    { "avc_aud_inp_16",   0, MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_DVI_0),      at_aud_dec_drv_ids },
    { "avc_aud_inp_17",   0, MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_DVI_1),      at_aud_dec_drv_ids },
    { "avc_aud_inp_18",   0, MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_DVI_2),      at_aud_dec_drv_ids },
    { "avc_aud_inp_19",   0, MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_DVI_3),      at_aud_dec_drv_ids },
    { "avc_aud_inp_20",   0, MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_SCART_0),    at_aud_dec_drv_ids },
    { "avc_aud_inp_21",   0, MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_SCART_1),    at_aud_dec_drv_ids },
    { "avc_aud_inp_22",   0, MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_SCART_2),    at_aud_dec_drv_ids },
    { "avc_aud_inp_23",   0, MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_SCART_3),    at_aud_dec_drv_ids }    
};

static DRV_CUSTOM_T at_avc_comp_vid_custom[] =
{
    /*e_custom_type,              pv_arg,                       ui2_id(IGNORED)*/
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[0],  0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[1],  0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[2],  0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[3],  0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[4],  0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[5],  0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[6],  0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[7],  0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[8],  0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[9],  0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[10], 0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[11], 0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[12], 0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[13], 0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[14], 0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[15], 0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[16], 0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[17], 0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[18], 0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[19], 0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[20], 0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[21], 0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[22], 0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[23], 0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[24], 0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[25], 0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[26], 0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[27], 0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[28], 0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[29], 0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[30], 0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[31], 0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[32], 0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[33], 0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[34], 0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[35], 0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[36], 0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[37], 0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[38], 0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[39], 0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[40], 0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[41], 0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[42], 0 },
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[43], 0 },    
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[44], 0 },        
    { DRV_CUSTOM_TYPE_COMMON_REG, (VOID*)&at_avc_comps_reg[45], 0 },        
    /* DO NOT REMOVE THIS LINE */
    DRV_CUSTOM_END
};

/*-----------------------------------------------------------------------------
 * AVC Combi Customization
 *---------------------------------------------------------------------------*/
static DRV_ID_T at_combi_0_drv_ids[] =
{
    MAKE_AVC_S_VIDEO_DRV_ID(0),
    MAKE_AVC_COMP_VID_DRV_ID(0),

    /* DO NOT REMOVE THIS LINE */
    DRV_ID_END
};

static DRV_ID_T at_combi_1_drv_ids[] =
{
    MAKE_AVC_S_VIDEO_DRV_ID(1),
    MAKE_AVC_COMP_VID_DRV_ID(1),

    /* DO NOT REMOVE THIS LINE */
    DRV_ID_END
};

static DRV_CUSTOM_TYPE_AVC_COMBI_REG_T at_avc_combi_reg[] =
{
    /*s_name,        t_drv_id,                 pt_combi_drv_ids,   pt_connection_ids */
    { "avc_combi_0", MAKE_AVC_COMBI_DRV_ID(0), at_combi_0_drv_ids, at_tvd_drv_ids },
    { "avc_combi_1", MAKE_AVC_COMBI_DRV_ID(1), at_combi_1_drv_ids, at_tvd_drv_ids },
};

static DRV_CUSTOM_T at_avc_combi_custom[] =
{
    /*e_custom_type,                 pv_arg,                       ui2_id(IGNORED)*/
    { DRV_CUSTOM_TYPE_AVC_COMBI_REG, (VOID*)&at_avc_combi_reg[0],  0 },
    { DRV_CUSTOM_TYPE_AVC_COMBI_REG, (VOID*)&at_avc_combi_reg[1],  0 },

    /* DO NOT REMOVE THIS LINE */
    DRV_CUSTOM_END
};

/*-----------------------------------------------------------------------------
 * Post Registration Customization
 *---------------------------------------------------------------------------*/
/* Input Source Group Lists */
/* TV */
static DRV_ID_T at_input_1_drv_ids[] =
{
    MAKE_TVD_DRV_ID(0),
    MAKE_TVD_DRV_ID(1),
#ifdef MW_TV_AV_BYPASS_SUPPORT
    MAKE_TVD_DRV_ID(2),
    MAKE_TVD_DRV_ID(3),
#endif
    MAKE_TUNER_TER_DIG_DRV_ID(1),
    MAKE_TUNER_CAB_DIG_DRV_ID(1),

#ifdef ANA_TUNER_SUPPORT
    MAKE_TUNER_TER_ANA_DRV_ID(0),
    MAKE_TUNER_CAB_ANA_DRV_ID(0),
    
    MAKE_AVC_SCART_DRV_ID(AVC_SCART_TYPE_SOURCE_SINK + 0),  /* SCART-1 input & output */
    MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_SCART_0),    
#endif

    /* DO NOT REMOVE THIS LINE */
    DRV_ID_END
};

/* AV1, COMBI S-VIDEO-1 */
static DRV_ID_T at_input_2_drv_ids[] =
{
    MAKE_TVD_DRV_ID(0),
    MAKE_TVD_DRV_ID(1),
#ifdef MW_TV_AV_BYPASS_SUPPORT
    MAKE_TVD_DRV_ID(2),
    MAKE_TVD_DRV_ID(3),
#endif
    MAKE_AVC_COMBI_DRV_ID(0),
    MAKE_AVC_S_VIDEO_DRV_ID(0),
    MAKE_AVC_COMP_VID_DRV_ID(0),
    MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_COMP_VID_0),

    /* DO NOT REMOVE THIS LINE */
    DRV_ID_END
};

#ifndef SYS_EVB_PCB
#else
/* AV2, COMBI S-VIDEO-1 */
static DRV_ID_T at_input_3_drv_ids[] =
{
    MAKE_TVD_DRV_ID(0),
    MAKE_TVD_DRV_ID(1),
#ifdef MW_TV_AV_BYPASS_SUPPORT
    MAKE_TVD_DRV_ID(2),
    MAKE_TVD_DRV_ID(3),
#endif
    MAKE_AVC_COMBI_DRV_ID(1),
    MAKE_AVC_S_VIDEO_DRV_ID(1),
    MAKE_AVC_COMP_VID_DRV_ID(1),
    MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_COMP_VID_1),

    /* DO NOT REMOVE THIS LINE */
    DRV_ID_END
};
#endif

#if 0
/* AV3 */
static DRV_ID_T at_input_4_drv_ids[] =
{
    MAKE_TVD_DRV_ID(0),
    MAKE_TVD_DRV_ID(1),
#ifdef MW_TV_AV_BYPASS_SUPPORT
    MAKE_TVD_DRV_ID(2),
    MAKE_TVD_DRV_ID(3),
#endif
    MAKE_AVC_COMP_VID_DRV_ID(2),
    MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_COMP_VID_2),

    /* DO NOT REMOVE THIS LINE */
    DRV_ID_END
};
#endif 

/* YPbPr-1 */
static DRV_ID_T at_input_5_drv_ids[] =
{
    MAKE_TVD_DRV_ID(0),
    MAKE_TVD_DRV_ID(1),
#ifdef MW_TV_AV_BYPASS_SUPPORT
    MAKE_TVD_DRV_ID(2),
    MAKE_TVD_DRV_ID(3),
#endif
    MAKE_AVC_Y_PB_PR_DRV_ID(0),
    MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_YPBPR_0),

    /* DO NOT REMOVE THIS LINE */
    DRV_ID_END
};

/* SACRT-1 */
static DRV_ID_T at_input_6_drv_ids[] =
{
    MAKE_TVD_DRV_ID(0),
    MAKE_TVD_DRV_ID(1),
#ifdef MW_TV_AV_BYPASS_SUPPORT
    MAKE_TVD_DRV_ID(2),
    MAKE_TVD_DRV_ID(3),
#endif
    MAKE_AVC_SCART_DRV_ID(AVC_SCART_TYPE_SOURCE + 0),       /* SCART-1 input */
    MAKE_AVC_SCART_DRV_ID(AVC_SCART_TYPE_SINK + 0),         /* SCART-1 output */
    MAKE_AVC_SCART_DRV_ID(AVC_SCART_TYPE_MONITOR + 0),      /* SCART-1 pin-8 detection */
#ifdef MW_TV_AV_BYPASS_SUPPORT
    MAKE_AVC_SCART_DRV_ID(AVC_SCART_TYPE_SOURCE_SINK + 0),       /* SCART-1 input */
#endif    
    MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_SCART_0),

    /* DO NOT REMOVE THIS LINE */
    DRV_ID_END
};

/* SACRT-2 */
static DRV_ID_T at_input_7_drv_ids[] =
{
    MAKE_TVD_DRV_ID(0),
    MAKE_TVD_DRV_ID(1),
#ifdef MW_TV_AV_BYPASS_SUPPORT
    MAKE_TVD_DRV_ID(2),
    MAKE_TVD_DRV_ID(3),
#endif
    MAKE_AVC_SCART_DRV_ID(AVC_SCART_TYPE_SOURCE + 1),       /* SCART-2 input */
    MAKE_AVC_SCART_DRV_ID(AVC_SCART_TYPE_SINK + 1),         /* SCART-2 output */
    MAKE_AVC_SCART_DRV_ID(AVC_SCART_TYPE_MONITOR + 1),      /* SCART-2 pin-8 detection */
#ifdef MW_TV_AV_BYPASS_SUPPORT
    MAKE_AVC_SCART_DRV_ID(AVC_SCART_TYPE_SOURCE_SINK + 1),       /* SCART-1 input */
#endif    
    MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_SCART_1),

    /* DO NOT REMOVE THIS LINE */
    DRV_ID_END
};

/* VGA */
static DRV_ID_T at_input_8_drv_ids[] =
{
    MAKE_TVD_DRV_ID(0),
    MAKE_TVD_DRV_ID(1),
#ifdef MW_TV_AV_BYPASS_SUPPORT
    MAKE_TVD_DRV_ID(2),
    MAKE_TVD_DRV_ID(3),
#endif
    MAKE_AVC_VGA_DRV_ID(0),
    MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_VGA_0),

    /* DO NOT REMOVE THIS LINE */
    DRV_ID_END
};

/* HDMI-1 */
static DRV_ID_T at_input_9_drv_ids[] =
{
    MAKE_TVD_DRV_ID(0),
    MAKE_TVD_DRV_ID(1),
#ifdef MW_TV_AV_BYPASS_SUPPORT
    MAKE_TVD_DRV_ID(2),
    MAKE_TVD_DRV_ID(3),
#endif
    MAKE_AVC_HDMI_DRV_ID(0),
    MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_HDMI_0),

    /* DO NOT REMOVE THIS LINE */
    DRV_ID_END
};

/*mt5389m1v2, there is only HDMI-1*/
#ifndef SYS_EVB_PCB 
/* HDMI-2 */
static DRV_ID_T at_input_10_drv_ids[] =
{
    MAKE_TVD_DRV_ID(0),
    MAKE_TVD_DRV_ID(1),
#ifdef MW_TV_AV_BYPASS_SUPPORT
    MAKE_TVD_DRV_ID(2),
    MAKE_TVD_DRV_ID(3),
#endif
    MAKE_AVC_HDMI_DRV_ID(1),
    MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_HDMI_1),

    /* DO NOT REMOVE THIS LINE */
    DRV_ID_END
};

/* HDMI-3 */
static DRV_ID_T at_input_11_drv_ids[] =
{
    MAKE_TVD_DRV_ID(0),
    MAKE_TVD_DRV_ID(1),
#ifdef MW_TV_AV_BYPASS_SUPPORT
    MAKE_TVD_DRV_ID(2),
    MAKE_TVD_DRV_ID(3),
#endif
    MAKE_AVC_HDMI_DRV_ID(2),
    MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_HDMI_2),

    /* DO NOT REMOVE THIS LINE */
    DRV_ID_END
};

/* HDMI-4 */
static DRV_ID_T at_input_12_drv_ids[] =
{
    MAKE_TVD_DRV_ID(0),
    MAKE_TVD_DRV_ID(1),
#ifdef MW_TV_AV_BYPASS_SUPPORT
    MAKE_TVD_DRV_ID(2),
    MAKE_TVD_DRV_ID(3),
#endif
    MAKE_AVC_HDMI_DRV_ID(3),
    MAKE_AVC_AUDIO_INP_DRV_ID(AUD_INP_COMP_ID_FOR_HDMI_3),

    /* DO NOT REMOVE THIS LINE */
    DRV_ID_END
};
#endif

static DRV_REG_GRP_NAME_T at_reg_grps[] =
{
    { SN_TUNER_GRP_0, at_input_1_drv_ids  },
    { SN_AVC_INP_00,  at_input_2_drv_ids  },
#ifndef SYS_EVB_PCB
#else   
    { SN_AVC_INP_01,  at_input_3_drv_ids  },
#endif    
  /*{ SN_AVC_INP_02,  at_input_4_drv_ids  },*/
    { SN_AVC_INP_03,  at_input_5_drv_ids  },
    { SN_AVC_INP_05,  at_input_6_drv_ids  },
    { SN_AVC_INP_06,  at_input_7_drv_ids  },
    { SN_AVC_INP_07,  at_input_8_drv_ids  }, 
    { SN_AVC_INP_08,  at_input_9_drv_ids  },
#ifndef SYS_EVB_PCB
    { SN_AVC_INP_09,  at_input_10_drv_ids  },
    { SN_AVC_INP_10,  at_input_11_drv_ids  },
    { SN_AVC_INP_11,  at_input_12_drv_ids  }
#endif    
};

/* Component Exclusive Lists */
static DRV_ID_T at_comp_excl_list1_drv_ids[] =
{
#ifdef MW_TV_AV_BYPASS_SUPPORT
#else
    #ifdef ANA_TUNER_SUPPORT
        MAKE_TUNER_CAB_ANA_DRV_ID(0),
        MAKE_TUNER_TER_ANA_DRV_ID(0),
    #endif
#endif /*MW_TV_AV_BYPASS_SUPPORT*/

    MAKE_AVC_COMP_VID_DRV_ID(0),
    MAKE_AVC_COMP_VID_DRV_ID(1),
    MAKE_AVC_COMP_VID_DRV_ID(2),
    MAKE_AVC_S_VIDEO_DRV_ID(0),
    MAKE_AVC_S_VIDEO_DRV_ID(1),
    MAKE_AVC_S_VIDEO_DRV_ID(2),

    MAKE_AVC_SCART_DRV_ID(AVC_SCART_TYPE_SOURCE + 0),       /* SCART-1 input */
    MAKE_AVC_SCART_DRV_ID(AVC_SCART_TYPE_SOURCE + 1),       /* SCART-2 input */
    
    /* DO NOT REMOVE THIS LINE */
    DRV_ID_END
};

static DRV_ID_T at_comp_excl_list2_drv_ids[] =
{
#ifdef MW_TV_AV_BYPASS_SUPPORT
#else
    #ifdef ANA_TUNER_SUPPORT
        MAKE_TUNER_CAB_ANA_DRV_ID(0),
        MAKE_TUNER_TER_ANA_DRV_ID(0),
    #endif
#endif /*MW_TV_AV_BYPASS_SUPPORT*/

    MAKE_AVC_Y_PB_PR_DRV_ID(0),
    MAKE_AVC_Y_PB_PR_DRV_ID(1),
    MAKE_AVC_VGA_DRV_ID(0),
    
    MAKE_AVC_SCART_DRV_ID(AVC_SCART_TYPE_SOURCE + 0),       /* SCART-1 input */
    MAKE_AVC_SCART_DRV_ID(AVC_SCART_TYPE_SOURCE + 1),       /* SCART-2 input */
    
    /* DO NOT REMOVE THIS LINE */
    DRV_ID_END
};

static DRV_ID_T at_comp_excl_list3_drv_ids[] =
{
    MAKE_AVC_HDMI_DRV_ID(0),
#ifndef SYS_EVB_PCB    
    MAKE_AVC_HDMI_DRV_ID(1),
    MAKE_AVC_HDMI_DRV_ID(2),
    MAKE_AVC_HDMI_DRV_ID(3),
#endif    
    MAKE_AVC_DVI_DRV_ID(0),

    /* DO NOT REMOVE THIS LINE */
    DRV_ID_END
};

#ifdef MW_PIP_POP_SUPPORT
static DRV_ID_T at_comp_excl_list4_drv_ids[] =
{
#ifdef ANA_TUNER_SUPPORT
    MAKE_TUNER_CAB_ANA_DRV_ID(0),
    MAKE_TUNER_TER_ANA_DRV_ID(0),
#endif

    MAKE_AVC_COMP_VID_DRV_ID(0),
    MAKE_AVC_COMP_VID_DRV_ID(1),
    MAKE_AVC_COMP_VID_DRV_ID(2),
    MAKE_AVC_S_VIDEO_DRV_ID(0),
    MAKE_AVC_S_VIDEO_DRV_ID(1),
    MAKE_AVC_S_VIDEO_DRV_ID(2),

    MAKE_AVC_SCART_DRV_ID(AVC_SCART_TYPE_SOURCE + 0),       /* SCART-1 input */
    MAKE_AVC_SCART_DRV_ID(AVC_SCART_TYPE_SOURCE + 1),       /* SCART-2 input */
    
    /* DO NOT REMOVE THIS LINE */
    DRV_ID_END
};

static DRV_ID_T at_comp_excl_list5_drv_ids[] =
{
    MAKE_AVC_Y_PB_PR_DRV_ID(0),
    MAKE_AVC_Y_PB_PR_DRV_ID(1),
    MAKE_AVC_VGA_DRV_ID(0),
    
    MAKE_AVC_SCART_DRV_ID(AVC_SCART_TYPE_SOURCE + 0),       /* SCART-1 input */
    MAKE_AVC_SCART_DRV_ID(AVC_SCART_TYPE_SOURCE + 1),       /* SCART-2 input */
    
    /* DO NOT REMOVE THIS LINE */
    DRV_ID_END
};

static DRV_ID_T at_comp_excl_list6_drv_ids[] =
{
    MAKE_AVC_HDMI_DRV_ID(0),
#ifndef SYS_EVB_PCB    
    MAKE_AVC_HDMI_DRV_ID(1),
    MAKE_AVC_HDMI_DRV_ID(2),
    MAKE_AVC_HDMI_DRV_ID(3),
#endif    
    MAKE_AVC_DVI_DRV_ID(0),

    /* DO NOT REMOVE THIS LINE */
    DRV_ID_END
};
#endif

static DRV_REG_COMP_EXCL_T t_comp_excl_grp1 =
{
    at_comp_excl_list1_drv_ids
};

static DRV_REG_COMP_EXCL_T t_comp_excl_grp2 =
{
    at_comp_excl_list2_drv_ids
};

static DRV_REG_COMP_EXCL_T t_comp_excl_grp3 =
{
    at_comp_excl_list3_drv_ids
};

#ifdef MW_PIP_POP_SUPPORT
static DRV_REG_COMP_EXCL_T t_comp_excl_grp4 =
{
    at_comp_excl_list4_drv_ids
};

static DRV_REG_COMP_EXCL_T t_comp_excl_grp5 =
{
    at_comp_excl_list5_drv_ids
};

static DRV_REG_COMP_EXCL_T t_comp_excl_grp6 =
{
    at_comp_excl_list6_drv_ids
};
#endif

static DRV_CUSTOM_T at_post_reg_custom[] =
{
    { DRV_CUSTOM_TYPE_COMMON_REG_GRP_NAME,  (VOID*)&at_reg_grps[0],        0 },
    { DRV_CUSTOM_TYPE_COMMON_REG_GRP_NAME,  (VOID*)&at_reg_grps[1],        0 },
    { DRV_CUSTOM_TYPE_COMMON_REG_GRP_NAME,  (VOID*)&at_reg_grps[2],        0 },
    { DRV_CUSTOM_TYPE_COMMON_REG_GRP_NAME,  (VOID*)&at_reg_grps[3],        0 },
    { DRV_CUSTOM_TYPE_COMMON_REG_GRP_NAME,  (VOID*)&at_reg_grps[4],        0 },
    { DRV_CUSTOM_TYPE_COMMON_REG_GRP_NAME,  (VOID*)&at_reg_grps[5],        0 },
    { DRV_CUSTOM_TYPE_COMMON_REG_GRP_NAME,  (VOID*)&at_reg_grps[6],        0 },
#ifndef SYS_EVB_PCB
    { DRV_CUSTOM_TYPE_COMMON_REG_GRP_NAME,  (VOID*)&at_reg_grps[7],        0 },
    { DRV_CUSTOM_TYPE_COMMON_REG_GRP_NAME,  (VOID*)&at_reg_grps[8],        0 },
    { DRV_CUSTOM_TYPE_COMMON_REG_GRP_NAME,  (VOID*)&at_reg_grps[9],        0 },
#else
    { DRV_CUSTOM_TYPE_COMMON_REG_GRP_NAME,  (VOID*)&at_reg_grps[7],        0 },
#endif
    { DRV_CUSTOM_TYPE_COMMON_REG_COMP_EXCL, (VOID*)&t_comp_excl_grp1,      0 },
    { DRV_CUSTOM_TYPE_COMMON_REG_COMP_EXCL, (VOID*)&t_comp_excl_grp2,      0 },
    { DRV_CUSTOM_TYPE_COMMON_REG_COMP_EXCL, (VOID*)&t_comp_excl_grp3,      0 },
#ifdef MW_PIP_POP_SUPPORT
    { DRV_CUSTOM_TYPE_COMMON_REG_COMP_EXCL_AP,  (VOID*)&t_comp_excl_grp4,      0 },
    { DRV_CUSTOM_TYPE_COMMON_REG_COMP_EXCL_AP,  (VOID*)&t_comp_excl_grp5,      0 },
    { DRV_CUSTOM_TYPE_COMMON_REG_COMP_EXCL_AP,  (VOID*)&t_comp_excl_grp6,      0 },            
#endif

    /* DO NOT REMOVE THIS LINE */
    DRV_CUSTOM_END
};

/*-----------------------------------------------------------------------------
 * Video Plane Customization
 *---------------------------------------------------------------------------*/
static DRV_CUSTOM_VID_PLA_XLAT_DISP_FMT_EX_INFO_T at_asp_ratio[] =
{
    { VID_PLA_DISP_FMT_NORMAL,           asp_ratio_normal_ex_fct     },
    { VID_PLA_DISP_FMT_LETTERBOX,        asp_ratio_letterbox_ex_fct  },
    { VID_PLA_DISP_FMT_PAN_SCAN,         asp_ratio_pan_scan_ex_fct   },
    { VID_PLA_DISP_FMT_NON_LINEAR_ZOOM,  asp_ratio_panorama_ex_fct   },
    { VID_PLA_DISP_FMT_DOT_BY_DOT,       asp_ratio_dot_by_dot_ex_fct },
    { VID_PLA_DISP_FMT_CUSTOM_0,         asp_ratio_auto_ex_fct       },
    { VID_PLA_DISP_FMT_UNKNOWN,          asp_ratio_no_video_fct      }
};

extern VID_QA_TBL_T     at_vid_qa_tbl[];

extern VOID vid_pla_overscan_fct (
    const UINT16                            ui2_vdp_id,
    const DRV_CUST_INPUT_SRC_TYPE_T         e_src_type,
    const DRV_CUSTOM_TIMING_INFO_T*         pt_src_timing,
    UINT16                                  *pui2_up,       /* OUT */
    UINT16                                  *pui2_down,     /* OUT */
    UINT16                                  *pui2_left,     /* OUT */
    UINT16                                  *pui2_right );  /* OUT */

extern VOID vid_pla_update_overscan_tbl (
    const UINT16                            ui2_vdp_id,
    const DRV_CUST_INPUT_SRC_TYPE_T         e_src_type,
    const DRV_CUSTOM_TIMING_INFO_T*         pt_src_timing,
    const UINT16                            ui2_up,
    const UINT16                            ui2_down,
    const UINT16                            ui2_left,
    const UINT16                            ui2_right );

extern UINT32 vid_pla_vid_qa_tbl_query_fct (
    const UINT16                            ui2_vdp_id,
    const DRV_TYPE_T                        e_src_type,
    const VID_PLA_PIC_MODE_T                e_pic_mode,
    VID_QA_MIN_MAX_DFT_T**                  ppt_min_max_dft_lst ); /* OUT */

static DRV_CUSTOM_T at_video_plane_custom[] =
{
    { DRV_CUSTOM_TYPE_VID_PLA_XLAT_DISP_FMT_EX,      (VOID*)&at_asp_ratio[0],  MAX_COMP_ID },
    { DRV_CUSTOM_TYPE_VID_PLA_XLAT_DISP_FMT_EX,      (VOID*)&at_asp_ratio[1],  MAX_COMP_ID },
    { DRV_CUSTOM_TYPE_VID_PLA_XLAT_DISP_FMT_EX,      (VOID*)&at_asp_ratio[2],  MAX_COMP_ID },
    { DRV_CUSTOM_TYPE_VID_PLA_XLAT_DISP_FMT_EX,      (VOID*)&at_asp_ratio[3],  MAX_COMP_ID },
    { DRV_CUSTOM_TYPE_VID_PLA_XLAT_DISP_FMT_EX,      (VOID*)&at_asp_ratio[4],  MAX_COMP_ID },
    { DRV_CUSTOM_TYPE_VID_PLA_XLAT_DISP_FMT_EX,      (VOID*)&at_asp_ratio[5],  MAX_COMP_ID },
    { DRV_CUSTOM_TYPE_VID_PLA_XLAT_DISP_FMT_EX,      (VOID*)&at_asp_ratio[6],  MAX_COMP_ID },
    { DRV_CUSTOM_TYPE_VID_PLA_VID_QA_MIN_MAX_DFT, (VOID*)at_vid_qa_tbl,     MAX_COMP_ID },
    { DRV_CUSTOM_TYPE_VID_PLA_OVERSCAN_CB,        (VOID*)vid_pla_overscan_fct, MAX_COMP_ID },
    { DRV_CUSTOM_TYPE_VID_PLA_UPDATE_OVERSCAN_TBL, (VOID*)vid_pla_update_overscan_tbl, MAX_COMP_ID },
    { DRV_CUSTOM_TYPE_VID_PLA_VID_QA_TBL_QUERY_CB, (VOID*)vid_pla_vid_qa_tbl_query_fct, MAX_COMP_ID },

    /* DO NOT REMOVE THIS LINE */
    DRV_CUSTOM_END
};

/*-----------------------------------------------------------------------------
 * Plane mixer Customization
 *---------------------------------------------------------------------------*/
extern DISP_QA_TBL_T     at_disp_qa_tbl[];

static DRV_CUSTOM_T at_plane_mxr_custom[] =
{
    { DRV_CUSTOM_TYPE_PLA_MXR_DISP_QA_MIN_MAX_DFT, (VOID*)at_disp_qa_tbl, MAX_COMP_ID },

    /* DO NOT REMOVE THIS LINE */
    DRV_CUSTOM_END
};

/*-----------------------------------------------------------------------------
 * Driver Customization
 *---------------------------------------------------------------------------*/
static DRV_CUSTOM_DIR_T at_drv_cust_dirs[] =
{
    /*e_first_type         e_last_type          pt_custom */
    { DRVT_AVC_COMP_VIDEO, DRVT_AVC_SPDIF,      (VOID*)at_avc_comp_vid_custom },
    { DRVT_AVC_COMBI,      DRVT_AVC_COMBI,      (VOID*)at_avc_combi_custom    },
    { DRVT_UNKNOWN,        DRVT_UNKNOWN,        (VOID*)at_post_reg_custom     },
    { DRVT_VID_PLANE,      DRVT_VID_PLANE,      (VOID*)at_video_plane_custom  },
    { DRVT_PLA_MXR,        DRVT_PLA_MXR,        (VOID*)at_plane_mxr_custom    },

    /* DO NOT REMOVE THIS LINE */
    DRV_CUSTOM_DIR_END
};

/*-----------------------------------------------------------------------------
                    functions implementation
 ----------------------------------------------------------------------------*/
/* MHF Linux - jzuo */
static DRV_ID_T* _alloc_copy_drv_id(DRV_ID_T* pSrc, INT32 *pt_size)	
{
	DRV_ID_T* pTmp;
	pTmp = pSrc;
	*pt_size = 0;
	while (!IS_DRV_ID_END(pTmp))
	{
		(*pt_size)++;
		pTmp++;
	}
	pTmp = (DRV_ID_T*)ssma_alloc(sizeof(DRV_ID_T)*((*pt_size)+1));
	x_memcpy(pTmp,pSrc,sizeof(DRV_ID_T)*((*pt_size)+1));
	return pTmp;
}

static DISP_QA_MIN_MAX_DFT_T *_alloc_copy_qa_min_max_dft(DISP_QA_MIN_MAX_DFT_T *pSrc, INT32 *pt_size)
{
	DISP_QA_MIN_MAX_DFT_T *pTmp;
	pTmp = pSrc;
	*pt_size = 0;
	while (pTmp->e_disp_qa_type != DISP_QA_TYPE_UNKNOWN)
	{
		pTmp++;
		(*pt_size)++;
	}
	pTmp = (DISP_QA_MIN_MAX_DFT_T*)ssma_alloc(sizeof(DISP_QA_MIN_MAX_DFT_T)*((*pt_size)+1));
	x_memcpy(pTmp,pSrc,sizeof(DISP_QA_MIN_MAX_DFT_T)*((*pt_size)+1));
	return pTmp;
}

static DISP_QA_TBL_T*_alloc_copy_qa_tbl(CUSTOM_TYPE_T e_type,DISP_QA_TBL_T* pSrc, INT32 *pt_size)
{
	DISP_QA_TBL_T* pTmp;
	DISP_QA_TBL_T* p_min_max_dft;
	INT32 i_size;
	pTmp = pSrc;
	*pt_size = 0;
	while (pTmp->e_src_type != DRVT_UNKNOWN)
	{
		(*pt_size)++;
		pTmp++;
	}
	pTmp = (DISP_QA_TBL_T*)ssma_alloc(sizeof(DISP_QA_TBL_T)*((*pt_size)+1));
	x_memcpy(pTmp,pSrc,sizeof(DISP_QA_TBL_T)*((*pt_size)+1));
	p_min_max_dft = pTmp;
	while (p_min_max_dft->e_src_type != DRVT_UNKNOWN)
	{
	    p_min_max_dft->pt_min_max_dft_lst = _alloc_copy_qa_min_max_dft(p_min_max_dft->pt_min_max_dft_lst,&i_size);
            p_min_max_dft++;
	}
	return pTmp;
}

static DRV_CUSTOM_VID_PLA_XLAT_DISP_FMT_EX_INFO_T *_alloc_copy_vid_pla(CUSTOM_TYPE_T e_type,DRV_CUSTOM_VID_PLA_XLAT_DISP_FMT_EX_INFO_T* pSrc)
{
	DRV_CUSTOM_VID_PLA_XLAT_DISP_FMT_EX_INFO_T* pTmp;
	INT32	i_size;
	if (e_type == DRV_CUSTOM_TYPE_VID_PLA_XLAT_DISP_FMT_EX)
	{
		pTmp = (DRV_CUSTOM_VID_PLA_XLAT_DISP_FMT_EX_INFO_T*)ssma_alloc(sizeof(DRV_CUSTOM_VID_PLA_XLAT_DISP_FMT_EX_INFO_T));
		x_memcpy(pTmp,pSrc,sizeof(DRV_CUSTOM_VID_PLA_XLAT_DISP_FMT_EX_INFO_T));
	}
	else if (e_type == DRV_CUSTOM_TYPE_VID_PLA_VID_QA_MIN_MAX_DFT)
	{
		pTmp = (DRV_CUSTOM_VID_PLA_XLAT_DISP_FMT_EX_INFO_T *)_alloc_copy_qa_tbl(e_type,(DISP_QA_TBL_T*)pSrc,&i_size);
	}
	else
	{
	    pTmp = pSrc;
	}
	return pTmp;
}

static DRV_REG_GRP_NAME_T* _alloc_copy_grp_name(CUSTOM_TYPE_T e_type,DRV_REG_GRP_NAME_T* pSrc)
{
	DRV_REG_GRP_NAME_T* pTmp;
        DRV_REG_COMP_EXCL_T* pTmpExcel;
	INT32	i_size;
	if (e_type == DRV_CUSTOM_TYPE_COMMON_REG_GRP_NAME)
	{
	    pTmp = (DRV_REG_GRP_NAME_T*)ssma_alloc(sizeof(DRV_REG_GRP_NAME_T));
	    x_memcpy(pTmp,pSrc,sizeof(DRV_REG_GRP_NAME_T));
	    pTmp->pt_drv_ids = _alloc_copy_drv_id(pSrc->pt_drv_ids,&i_size);
	}
	else if (e_type == DRV_CUSTOM_TYPE_COMMON_REG_COMP_EXCL)
	{
	    pTmpExcel = (DRV_REG_COMP_EXCL_T*)ssma_alloc(sizeof(DRV_REG_COMP_EXCL_T));
	    x_memcpy(pTmpExcel,pSrc,sizeof(DRV_REG_COMP_EXCL_T)); 
            pTmp = (DRV_REG_GRP_NAME_T*)pTmpExcel;
	}
	return pTmp;
}

static DRV_CUSTOM_TYPE_AVC_COMBI_REG_T* _alloc_copy_combi_reg(CUSTOM_TYPE_T e_type,DRV_CUSTOM_TYPE_AVC_COMBI_REG_T* pSrc)
{
	DRV_CUSTOM_TYPE_AVC_COMBI_REG_T* pTmp;
	INT32	i_size;
	pTmp = (DRV_CUSTOM_TYPE_AVC_COMBI_REG_T*)ssma_alloc(sizeof(DRV_CUSTOM_TYPE_AVC_COMBI_REG_T));
	x_memcpy(pTmp,pSrc,sizeof(DRV_CUSTOM_TYPE_AVC_COMBI_REG_T));
	pTmp->pt_connection_ids = _alloc_copy_drv_id(pSrc->pt_connection_ids,&i_size);
	return pTmp;
}

static DRV_REG_T* _alloc_copy_drv_reg(CUSTOM_TYPE_T e_type,DRV_REG_T* pSrc)
{
    DRV_REG_T* pTmp;
	INT32	i_size;
	pTmp = (DRV_REG_T*)ssma_alloc(sizeof(DRV_REG_T));
	x_memcpy(pTmp,pSrc,sizeof(DRV_REG_T));
	pTmp->pt_connection_ids = _alloc_copy_drv_id(pSrc->pt_connection_ids,&i_size);
	return pTmp;
}

static DRV_CUSTOM_T* _alloc_copy_drv_custom(DRV_CUSTOM_T* pSrc,INT32 *pt_size)
{
	DRV_CUSTOM_T* pTmp ;
	DRV_CUSTOM_T* p_arg_list;
	INT32 i_size;
	pTmp = pSrc;
	*pt_size = 0;
	while (!IS_DRV_CUSTOM_END(pTmp))
	{
		(*pt_size)++;
		pTmp++;
	}

	pTmp = (DRV_CUSTOM_T*)ssma_alloc(sizeof(DRV_CUSTOM_T)*((*pt_size)+1));
	x_memcpy(pTmp,pSrc,sizeof(DRV_CUSTOM_T)*((*pt_size)+1));
	p_arg_list = pTmp;
	while (!IS_DRV_CUSTOM_END(p_arg_list))
	{
		if (pSrc == at_avc_comp_vid_custom)
		{
			p_arg_list->pv_arg = _alloc_copy_drv_reg(p_arg_list->e_custom_type,p_arg_list->pv_arg);
		}
		else if (pSrc == at_avc_combi_custom)
		{
			p_arg_list->pv_arg = _alloc_copy_combi_reg(p_arg_list->e_custom_type,p_arg_list->pv_arg);
		}
		else if (pSrc == at_post_reg_custom)
		{
			p_arg_list->pv_arg = _alloc_copy_grp_name(p_arg_list->e_custom_type,p_arg_list->pv_arg);
		}
		else if (pSrc == at_video_plane_custom)
		{
			p_arg_list->pv_arg = _alloc_copy_vid_pla(p_arg_list->e_custom_type,p_arg_list->pv_arg);
		}
		else if (pSrc == at_plane_mxr_custom)
		{
			p_arg_list->pv_arg = _alloc_copy_qa_tbl(p_arg_list->e_custom_type,p_arg_list->pv_arg,&i_size);
		}
		p_arg_list++;
	}
	return pTmp;
}

static DRV_CUSTOM_DIR_T *_alloc_copy_custom_dir(DRV_CUSTOM_DIR_T * pSrc, INT32 *pt_size)
{
	DRV_CUSTOM_DIR_T *pTmp;
	DRV_CUSTOM_DIR_T *p_cust_list;
	INT32 i_size;
	pTmp = pSrc;
	*pt_size = 0;
	while (!IS_DRV_CUSTOM_DIR_END(pTmp))
	{
		(*pt_size)++;
		pTmp++;
	}
	pTmp = (DRV_CUSTOM_DIR_T*)ssma_alloc(sizeof(DRV_CUSTOM_DIR_T)*((*pt_size)+1));
	x_memcpy(pTmp,pSrc,sizeof(DRV_CUSTOM_DIR_T)*((*pt_size)+1));
	p_cust_list = pTmp;
	while (!IS_DRV_CUSTOM_DIR_END(p_cust_list))
	{
		p_cust_list->pt_custom = _alloc_copy_drv_custom(p_cust_list->pt_custom,&i_size);
		p_cust_list++;
	}
	return pTmp;
}

VOID make_copy_drv_custom(GEN_CONFIG_T* pt_drv_config )
{
    INT32 i_size;
    pt_drv_config->pv_config = _alloc_copy_custom_dir(at_drv_cust_dirs,&i_size);
}
/* MHF Linux - jzuo */

VOID init_drv_custom( GEN_CONFIG_T* pt_drv_config )
{
    INT32 i_size; /* MHF Linux - jzuo */
    if ( !pt_drv_config )
    {
        return;
    }

    pt_drv_config->ui2_version = DRV_CUSTOM_VERSION;
    pt_drv_config->pv_config = (VOID*)at_drv_cust_dirs;
    g_pv_drv_config = (VOID *)_alloc_copy_custom_dir(at_drv_cust_dirs,&i_size); /* MHF Linux - jzuo */
    pt_drv_config->z_config_size = sizeof(at_drv_cust_dirs);
}

