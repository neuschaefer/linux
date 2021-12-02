/* This source file was automatically created by the */
/* tool 'MTK RPC Description tool', 'Version 1.10' on 'Mon Jan 28 13:43:31 2013'. */
/* Do NOT modify this source file. */



/* Start of source pre-amble file '/proj/mtk70515/ws_jifeng.fan_linux_vestel_mb82/DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/vm_linux/dtv_linux/project_x_linux/rpc/rpc_desc_input.h'. */

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

#include "rpc.h"
#include "svctx/x_svctx.h"
#include "svctx/rec_hdlr/u_rec_hdlr.h"
#include "evctx/x_evctx.h"
#include "flm/x_flm.h"
#include "rrctx/x_rrctx.h"
#include "dt/x_dt.h"
#include "tbl_mngr/x_tm.h"
#include "tbl_mngr/atsc_si_eng/x_atsc_si_eng.h"
#include "svl/x_svl.h"
#include "svl_bldr/x_sb.h"
#include "svl_bldr/sb_atsc_eng/x_sb_atsc_eng.h"
#include "svl_bldr/sb_ntsc_eng/x_sb_ntsc_eng.h"
#include "svl_bldr/sb_cqam_eng/x_sb_cqam_eng.h"
#include "svl_bldr/sb_dvb_eng/x_sb_dvb_eng.h"
#include "svl_bldr/sb_dvbc_eng/x_sb_dvbc_eng.h"
#include "svl_bldr/sb_pal_secam_eng/x_sb_pal_secam_eng.h"
#include "svl_bldr/sb_isdb_eng/x_sb_isdb_eng.h"
#include "lol/x_lol.h"
#include "nwl/x_nwl.h"
#include "tsl/x_tsl.h"
#include "pcl/x_pcl.h"
#include "io_mngr/x_iom.h"
#include "rrctx/rrctx_rrt_eng/x_rrctx_rrt_eng.h"
#include "evctx/evctx_atsc_eng/x_evctx_atsc_eng.h"
#include "conn_mngr/cm_playback/u_playback_handler.h"
#include "inc/x_mm_common.h"
#include "strm_mngr/scc/u_scc.h"
#include "strm_mngr/video_hdlr/u_sm_video_hdlr.h"
#include "img/x_img.h"
#include "cec_mngr/x_cecm.h"
#include "satl/x_satl.h"
#include "lst_mngr/x_lst_mngr.h"
#include "res_mngr/drv/u_drv_cust.h"
#include "df_mngr/x_df_mngr.h"
#include "tbl_mngr/psi_eng/x_psi_eng.h"
#include "cap/x_cap.h"
#include "ci/x_ci.h"
#ifdef ANDROID
#include "brdcst/a_brdcst_jni.h"
#include "channel/channel_service_wrapper.h"
#include "config/a_d_interface.h"
#include "config/a_cfg.h"
#include "input/input_service_client.h"
#include "scan/scan_service_client.h"
#include "scan/dvbc_scan_service_client.h"
#include "edb/u_edb.h"
#endif
#include "chip_spec/x_chip_spec.h"



/* End of source pre-amble file '/proj/mtk70515/ws_jifeng.fan_linux_vestel_mb82/DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/vm_linux/dtv_linux/project_x_linux/rpc/rpc_desc_input.h'. */

static const RPC_DESC_T t_rpc_decl_STYLE_T;
static const RPC_DESC_T t_rpc_decl_STYLE_STR_T;
static const RPC_DESC_T t_rpc_decl_STREAM_COMP_ID_T;
static const RPC_DESC_T t_rpc_decl_THREAD_DESCR_T;
static const RPC_DESC_T t_rpc_decl_TS_DESCR_T;
static const RPC_DESC_T t_rpc_decl_ARG_DESC_T_u;
static const RPC_DESC_T t_rpc_decl_ARG_DESC_T;
static const RPC_DESC_T t_rpc_decl_RPC_DESC_ENTRY_T_u;
static const RPC_DESC_T t_rpc_decl_RPC_DESC_ENTRY_T;
static const RPC_DESC_T t_rpc_decl_RPC_DESC_T;
static const RPC_DESC_T t_rpc_decl_OS_FNCT_T;
static const RPC_DESC_T t_rpc_decl_CM_COMMAND_T_u;
static const RPC_DESC_T t_rpc_decl_CM_COMMAND_T;
static const RPC_DESC_T t_rpc_decl_CM_CTRL_RM_SRC_COMP_T;
static const RPC_DESC_T t_rpc_decl_TUNER_INTERLEAVING_T;
static const RPC_DESC_T t_rpc_decl_TUNER_DVBT2_PLP_LIST_INFO;
static const RPC_DESC_T t_rpc_decl_TUNER_LNB_INFO_T;
static const RPC_DESC_T t_rpc_decl_TUNER_DISEQC_INFO_T;
static const RPC_DESC_T t_rpc_decl_TUNER_CAB_ANA_TUNE_INFO_T;
static const RPC_DESC_T t_rpc_decl_TUNER_CAB_DIG_TUNE_INFO_T;
static const RPC_DESC_T t_rpc_decl_TUNER_CAB_DIG_OOB_TX_TUNE_INFO_T;
static const RPC_DESC_T t_rpc_decl_TUNER_CAB_DIG_OOB_RX_TUNE_INFO_T;
static const RPC_DESC_T t_rpc_decl_TUNER_SAT_ANA_TUNE_INFO_T;
static const RPC_DESC_T t_rpc_decl_TUNER_SAT_DIG_TUNE_INFO_T;
static const RPC_DESC_T t_rpc_decl_TUNER_TER_ANA_TUNE_INFO_T;
static const RPC_DESC_T t_rpc_decl_TUNER_TER_DIG_TUNE_INFO_T;
static const RPC_DESC_T t_rpc_decl_DB_FIELD_INFO_T;
static const RPC_DESC_T t_rpc_decl_DB_SEARCH_T;
static const RPC_DESC_T t_rpc_decl_MEDIA_SUBTYPE_T;
static const RPC_DESC_T t_rpc_decl_MEDIA_FORMAT_T;
static const RPC_DESC_T t_rpc_decl_MM_PLAY_MODE_PL_T;
static const RPC_DESC_T t_rpc_decl_DRM_OUTPUT_SIGNAL_PROTECTION_INFO_T;
static const RPC_DESC_T t_rpc_decl_DIVX_DRM_INIT_CONFIG_T;
static const RPC_DESC_T t_rpc_decl_DIVX_DRM_REGISTRATION_INFO_T;
static const RPC_DESC_T t_rpc_decl_DIVX_DRM_DEACTIVATION_INFO_T;
static const RPC_DESC_T t_rpc_decl_DIVX_DRM_UI_HELP_INFO_T;
static const RPC_DESC_T t_rpc_decl_DIVX_DRM_MEMORY_RW_FUNC_INFO_T;
static const RPC_DESC_T t_rpc_decl_DIVX_DRM_BASIC_INFO_T;
static const RPC_DESC_T t_rpc_decl_DIVX_DRM_UNION_GENERIC_INFO_T;
static const RPC_DESC_T t_rpc_decl_MINFO_MPEG_2_STREAM_ID;
static const RPC_DESC_T t_rpc_decl_MINFO_MP3_AUDIO_STM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_MINFO_FLV_AUDIO_STM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_MINFO_FLV_VIDEO_STM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_MINFO_CMPB_VIDEO_STM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_MINFO_CMPB_AUDIO_STM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_MINFO_CMPB_SUBTL_STM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_MINFO_RM_VIDEO_STM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_MINFO_RM_AUDIO_STM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_MINFO_AIF_AUDIO_STM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_MINFO_AVI_AUDIO_STM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_MINFO_AVI_VIDEO_CODEC_XXDB_T;
static const RPC_DESC_T t_rpc_decl_MINFO_AVI_VIDEO_CODEC_INFO_T;
static const RPC_DESC_T t_rpc_decl_MINFO_AVI_VIDEO_STM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_MINFO_AVI_DIVX_SUBTITLE_STM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_MINFO_ASF_AUDIO_STM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_MINFO_ASF_VIDEO_STM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_MINFO_MP4_AUDIO_STM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_MINFO_MP4_VIDEO_STM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_MINFO_MP4_SUBTITLE_STM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_MINFO_MKV_TM_SCALE_FLOAT_T;
static const RPC_DESC_T t_rpc_decl_MINFO_MKV_CONTENT_ENCODING_T;
static const RPC_DESC_T t_rpc_decl_MINFO_MKV_AUDIO_STM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_MINFO_MKV_VIDEO_STM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_MINFO_MKV_SUBTITLE_STM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_FONT_INFO_T;
static const RPC_DESC_T t_rpc_decl_MINFO_PS_AUDIO_STM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_MINFO_PS_VIDEO_STM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_MINFO_PS_SUBTITLE_STM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_MINFO_EXT_SUBTITLE_STM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_MINFO_ES_VIDEO_STM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_MINFO_ES_AUDIO_STM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_OGG_STREAM_VORBIS_INFO_T;
static const RPC_DESC_T t_rpc_decl_OGG_STREAM_AUDIO_INFO_T;
static const RPC_DESC_T t_rpc_decl_MINFO_OGG_VIDEO_STM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_MINFO_OGG_TEXT_STM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_MINFO_OGG_AUDIO_VORBIS_SPEC_T;
static const RPC_DESC_T t_rpc_decl_MINFO_OGG_AUDIO_STM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_MINFO_TS_RD3D_REG_DESC_T;
static const RPC_DESC_T t_rpc_decl_MINFO_MVC_EXTENSION_DESC_T;
static const RPC_DESC_T t_rpc_decl_MINFO_TS_VIDEO_STM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_MINFO_TS_AUDIO_STM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_MINFO_TS_SBTL_STM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_MINFO_STM_ID_T;
static const RPC_DESC_T t_rpc_decl_MINFO_TYPE_STM_ATRBT_T_u;
static const RPC_DESC_T t_rpc_decl_MINFO_TYPE_STM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_MINFO_INFO_PIC_T;
static const RPC_DESC_T t_rpc_decl_MINFO_ID3_RAW_DATA_T;
static const RPC_DESC_T t_rpc_decl_MINFO_TYPE_ID3_TAG_T;
static const RPC_DESC_T t_rpc_decl_MINFO_TITLE_INFO_T;
static const RPC_DESC_T t_rpc_decl_MINFO_PLAYLIST_INFO_T;
static const RPC_DESC_T t_rpc_decl_MINFO_CHAP_INFO_T;
static const RPC_DESC_T t_rpc_decl_MINFO_DISP_INFO_T;
static const RPC_DESC_T t_rpc_decl_MINFO_TYPE_GENERIC_T;
static const RPC_DESC_T t_rpc_decl_MM_TM_T;
static const RPC_DESC_T t_rpc_decl_MINFO_TYPE_FRAME_RATE_T;
static const RPC_DESC_T t_rpc_decl_MINFO_TYPE_VIDEO_ES_INFO_T;
static const RPC_DESC_T t_rpc_decl_MINFO_TYPE_AUDIO_ES_INFO_T;
static const RPC_DESC_T t_rpc_decl_MINFO_TYPE_META_DATA_T;
static const RPC_DESC_T t_rpc_decl_MP4_SINGLE_FILE_INFO_T;
static const RPC_DESC_T t_rpc_decl_MP4_MULTI_FILE_INFO_T;
static const RPC_DESC_T t_rpc_decl_MP4_MINFO_EXTRA_INFO_T;
static const RPC_DESC_T t_rpc_decl_MINFO_TYPE_IDX_INFO_T;
static const RPC_DESC_T t_rpc_decl_MINFO_MP4_TRAK_ENCRYTION_INFO_T;
static const RPC_DESC_T t_rpc_decl_MINFO_MP4_ENCRYTION_INFO_T_u;
static const RPC_DESC_T t_rpc_decl_MINFO_MP4_ENCRYTION_INFO_T;
static const RPC_DESC_T t_rpc_decl_MINFO_TYPE_ENCRYPTION_INFO_T_u;
static const RPC_DESC_T t_rpc_decl_MINFO_TYPE_ENCRYPTION_INFO_T;
static const RPC_DESC_T t_rpc_decl_MINFO_DIVX_DRM_OBJ_T;
static const RPC_DESC_T t_rpc_decl_MINFO_TYPE_DRM_INFO_T_u;
static const RPC_DESC_T t_rpc_decl_MINFO_TYPE_DRM_INFO_T;
static const RPC_DESC_T t_rpc_decl_MINFO_TYPE_FONT_INFO_T;
static const RPC_DESC_T t_rpc_decl_MINFO_TYPE_DIVX_INFO_T;
static const RPC_DESC_T t_rpc_decl_MINFO_INFO_T_u;
static const RPC_DESC_T t_rpc_decl_MINFO_INFO_T;
static const RPC_DESC_T t_rpc_decl_MINFO_TYPE_RAW_DATA_T;
static const RPC_DESC_T t_rpc_decl_SCDB_MPEG_COMMON_T;
static const RPC_DESC_T t_rpc_decl_SCDB_COMMON_T;
static const RPC_DESC_T t_rpc_decl_SCDB_AUDIO_MPEG_T;
static const RPC_DESC_T t_rpc_decl_AVC_VID_DESC_T;
static const RPC_DESC_T t_rpc_decl_RD3D_REG_DESC_T;
static const RPC_DESC_T t_rpc_decl_MVC_EXTENSION_DESC_T;
static const RPC_DESC_T t_rpc_decl_SCDB_VIDEO_MPEG_T;
static const RPC_DESC_T t_rpc_decl_SCDB_PRIVATE_MPEG_T;
static const RPC_DESC_T t_rpc_decl_SCDB_DSMCC_MPEG_T;
static const RPC_DESC_T t_rpc_decl_CC_ATSC_LINE_21_T;
static const RPC_DESC_T t_rpc_decl_CC_ATSC_DTV_T;
static const RPC_DESC_T t_rpc_decl_SCDB_CLOSED_CAPTION_T;
static const RPC_DESC_T t_rpc_decl_SCDB_ISDB_CAPTION_T;
static const RPC_DESC_T t_rpc_decl_SCDB_ISDB_TEXT_T;
static const RPC_DESC_T t_rpc_decl_SCDB_SUBTITLE_DVB_T;
static const RPC_DESC_T t_rpc_decl_SCDB_TELETEXT_DVB_T;
static const RPC_DESC_T t_rpc_decl_SCDB_VBI_TELETEXT_DVB_T;
static const RPC_DESC_T t_rpc_decl_SCDB_VBI_DATA_DVB_T;
static const RPC_DESC_T t_rpc_decl_SCDB_AUDIO_ANALOG_T;
static const RPC_DESC_T t_rpc_decl_SCDB_VIDEO_ANALOG_T;
static const RPC_DESC_T t_rpc_decl_SCDB_AUDIO_DIGITAL_T;
static const RPC_DESC_T t_rpc_decl_SCDB_AUDIO_AVI_T;
static const RPC_DESC_T t_rpc_decl_SCDB_VIDEO_AVI_T;
static const RPC_DESC_T t_rpc_decl_SCDB_SUBTITLE_DIVX_T;
static const RPC_DESC_T t_rpc_decl_SCDB_AUDIO_MP4_T;
static const RPC_DESC_T t_rpc_decl_SCDB_SUBTITLE_MP4_T;
static const RPC_DESC_T t_rpc_decl_SCDB_VIDEO_MP4_T;
static const RPC_DESC_T t_rpc_decl_SCDB_AUDIO_ASF_T;
static const RPC_DESC_T t_rpc_decl_SCDB_VIDEO_ASF_T;
static const RPC_DESC_T t_rpc_decl_SCDB_AUDIO_PS_T;
static const RPC_DESC_T t_rpc_decl_SCDB_VIDEO_PS_T;
static const RPC_DESC_T t_rpc_decl_SCDB_VIDEO_MKV_T;
static const RPC_DESC_T t_rpc_decl_SCDB_AUDIO_MKV_T;
static const RPC_DESC_T t_rpc_decl_SCDB_VIDEO_RM_T;
static const RPC_DESC_T t_rpc_decl_SCDB_AUDIO_RM_T;
static const RPC_DESC_T t_rpc_decl_SCDB_AUDIO_FLV_T;
static const RPC_DESC_T t_rpc_decl_SCDB_VIDEO_FLV_T;
static const RPC_DESC_T t_rpc_decl_SCDB_SUBTITLE_MKV_T;
static const RPC_DESC_T t_rpc_decl_SCDB_AUDIO_MP3_T;
static const RPC_DESC_T t_rpc_decl_SCDB_VIDEO_ES_T;
static const RPC_DESC_T t_rpc_decl_SCDB_AUDIO_ES_T;
static const RPC_DESC_T t_rpc_decl_SCDB_SUBTITLE_PS_T;
static const RPC_DESC_T t_rpc_decl_SCDB_VIDEO_OGG_T;
static const RPC_DESC_T t_rpc_decl_SCDB_AUDIO_VORBIS_OGG_T;
static const RPC_DESC_T t_rpc_decl_SCDB_AUDIO_NORMAL_OGG_T;
static const RPC_DESC_T t_rpc_decl_SCDB_AUDIO_OGG_T;
static const RPC_DESC_T t_rpc_decl_SCDB_EXT_SUBTITLE_T;
static const RPC_DESC_T t_rpc_decl_SCDB_REC_T_u;
static const RPC_DESC_T t_rpc_decl_SCDB_REC_T;
static const RPC_DESC_T t_rpc_decl_DISEQC_10_SET_T;
static const RPC_DESC_T t_rpc_decl_BRDCST_CTRL_DISEQC_T;
static const RPC_DESC_T t_rpc_decl_BRDCST_CTRL_TUNE_ATTRIBUTES_T;
static const RPC_DESC_T t_rpc_decl_BRDCST_CTRL_AUDIO_STREAM_FMT_T;
static const RPC_DESC_T t_rpc_decl_BRDCST_CTRL_AUDIO_ENC_T;
static const RPC_DESC_T t_rpc_decl_BRDCST_TIME_OUT_T;
static const RPC_DESC_T t_rpc_decl_SM_COMMAND_T_u;
static const RPC_DESC_T t_rpc_decl_SM_COMMAND_T;
static const RPC_DESC_T t_rpc_decl_SM_OBSVR_FILTER_T;
static const RPC_DESC_T t_rpc_decl_SM_VSH_VBI_INFO_T;
static const RPC_DESC_T t_rpc_decl_SM_VSH_VBI_NFY_T;
static const RPC_DESC_T t_rpc_decl_SM_VSH_START_RENDER_INFO_T;
static const RPC_DESC_T t_rpc_decl_SM_VSH_VBI_REF_INFO_T_u;
static const RPC_DESC_T t_rpc_decl_SM_VSH_VBI_REF_INFO_T;
static const RPC_DESC_T t_rpc_decl_VSH_CAPTURE_INFO_T;
static const RPC_DESC_T t_rpc_decl_VSH_REGION_INFO_T;
static const RPC_DESC_T t_rpc_decl_VSH_REGION_CAPABILITY_INFO_T;
static const RPC_DESC_T t_rpc_decl_VSH_ENCHANCE_INFO_T;
static const RPC_DESC_T t_rpc_decl_VSH_SET_PLANE_ORDER_INFO_T;
static const RPC_DESC_T t_rpc_decl_VSH_GET_PLANE_ORDER_INFO_T;
static const RPC_DESC_T t_rpc_decl_VSH_QV_ROTATE_T;
static const RPC_DESC_T t_rpc_decl_VSH_QV_CTRL_INFO_T;
static const RPC_DESC_T t_rpc_decl_VSH_HDMI_AVI_INFOFRAME_INFO_T;
static const RPC_DESC_T t_rpc_decl_VSH_SRC_RESOLUTION_INFO_T;
static const RPC_DESC_T t_rpc_decl_VSH_VID_MIN_MAX_INFO_T;
static const RPC_DESC_T t_rpc_decl_VSH_CNT_ADV_INFO_T;
static const RPC_DESC_T t_rpc_decl_VSH_VGA_TIMING_OPTION_T;
static const RPC_DESC_T t_rpc_decl_VSH_AMBIG_VGA_TIMING_OPTION_T;
static const RPC_DESC_T t_rpc_decl_VSH_AMBIG_VGA_TIMING_INFO_T;
static const RPC_DESC_T t_rpc_decl_VSH_SEQUENCE_DATA_T;
static const RPC_DESC_T t_rpc_decl_VSH_SEQUENCE_RAW_T;
static const RPC_DESC_T t_rpc_decl_VSH_SEQUENCE_INFO_T_u;
static const RPC_DESC_T t_rpc_decl_VSH_SEQUENCE_INFO_T;
static const RPC_DESC_T t_rpc_decl_VSH_TIME_CODE_INFO_T;
static const RPC_DESC_T t_rpc_decl_VSH_PICTURE_INFO_T;
static const RPC_DESC_T t_rpc_decl_VSH_THUMBNAIL_INFO_T;
static const RPC_DESC_T t_rpc_decl_THUMBNAIL_GET_INFO_T;
static const RPC_DESC_T t_rpc_decl_ASH_VOLUME_INFO_T;
static const RPC_DESC_T t_rpc_decl_ASH_AUDIO_INFO_T;
static const RPC_DESC_T t_rpc_decl_ASH_AUD_SYS_INFO_T;
static const RPC_DESC_T t_rpc_decl_ASH_AUDIO_WMA_INFO_T;
static const RPC_DESC_T t_rpc_decl_ASH_AUDIO_AAC_INFO_T;
static const RPC_DESC_T t_rpc_decl_ASH_AUDIO_PCM_INFO_T;
static const RPC_DESC_T t_rpc_decl_ASH_AUDIO_COOK_INFO_T;
static const RPC_DESC_T t_rpc_decl_ASH_ADUIO_VORBIS_INFO_T;
static const RPC_DESC_T t_rpc_decl_ASH_ADUIO_AC3_INFO_T;
static const RPC_DESC_T t_rpc_decl_ASH_AUDIO_CODEC_SPEC_INFO_T_u;
static const RPC_DESC_T t_rpc_decl_ASH_AUDIO_CODEC_SPEC_INFO_T;
static const RPC_DESC_T t_rpc_decl_SCC_BG_COLOR_T;
static const RPC_DESC_T t_rpc_decl_SCC_VID_MODE_EX_T;
static const RPC_DESC_T t_rpc_decl_SCC_AUD_DEC_LINEOUT_VOL_T;
static const RPC_DESC_T t_rpc_decl_SCC_VID_OVER_SCAN_CLIPPER_T;
static const RPC_DESC_T t_rpc_decl_SCC_VID_VDRIPS_BUFF_T;
static const RPC_DESC_T t_rpc_decl_SCC_VID_IFRAME_BUFF_T;
static const RPC_DESC_T t_rpc_decl_SCC_VID_REGION_T;
static const RPC_DESC_T t_rpc_decl_SCC_VID_DEBLOCKING_INFO_T;
static const RPC_DESC_T t_rpc_decl_SCC_VID_LB_DETECT_NFY_INFO_T;
static const RPC_DESC_T t_rpc_decl_SCC_VID_LB_DETECT_CONFIG_T;
static const RPC_DESC_T t_rpc_decl_SCC_VID_MJC_T;
static const RPC_DESC_T t_rpc_decl_SCC_VID_SUPER_BLANK_T;
static const RPC_DESC_T t_rpc_decl_SCC_VID_SUPER_FREEZE_T;
static const RPC_DESC_T t_rpc_decl_SCC_ANGULAR_SCE_T;
static const RPC_DESC_T t_rpc_decl_SCC_VID_DEC_CAPABILITY_INFO_T;
static const RPC_DESC_T t_rpc_decl_SCC_VID_FREQ_T;
static const RPC_DESC_T t_rpc_decl_SCC_VID_SRC_DET_STATUS_T;
static const RPC_DESC_T t_rpc_decl_SCC_VID_SCR_MODE_T;
static const RPC_DESC_T t_rpc_decl_SCC_DISP_RESOLUTION_T;
static const RPC_DESC_T t_rpc_decl_SCC_DISP_SCREEN_RESOLUTION_T;
static const RPC_DESC_T t_rpc_decl_SCC_DISP_COLOR_GAIN_T;
static const RPC_DESC_T t_rpc_decl_SCC_DISP_RANGE_INFO_T;
static const RPC_DESC_T t_rpc_decl_SCC_AUD_LOUD_SPEAKER_T;
static const RPC_DESC_T t_rpc_decl_SCC_AUD_SE_TRIM_T;
static const RPC_DESC_T t_rpc_decl_SCC_AUD_VOLUME_INFO_T;
static const RPC_DESC_T t_rpc_decl_SCC_AUD_CHNL_DELAY_IND_T;
static const RPC_DESC_T t_rpc_decl_SCC_AUD_CHNL_DELAY_T;
static const RPC_DESC_T t_rpc_decl_SCC_AUD_SE_SURROUND_VALUE_T;
static const RPC_DESC_T t_rpc_decl_SCC_AUD_SE_SURROUND_INFO_T;
static const RPC_DESC_T t_rpc_decl_SCC_AUD_DEC_SET_TYPE_PEQ_INFO_T;
static const RPC_DESC_T t_rpc_decl_SCC_AUD_SE_QA_USER_LVL_T;
static const RPC_DESC_T t_rpc_decl_SCC_AUD_UPLOAD_DATA_NFT_INFO_T;
static const RPC_DESC_T t_rpc_decl_SCC_AUD_HDPHONE_DETECT_NFT_INFO_T;
static const RPC_DESC_T t_rpc_decl_SCC_AUD_MUTE_EX_INFO_T;
static const RPC_DESC_T t_rpc_decl_SCC_AU_SE_QA_USER_BAND_INFO_T;
static const RPC_DESC_T t_rpc_decl_SCC_AUD_CLIP_BUFF_T;
static const RPC_DESC_T t_rpc_decl_SCC_AUD_CLIP_T_u;
static const RPC_DESC_T t_rpc_decl_SCC_AUD_CLIP_T;
static const RPC_DESC_T t_rpc_decl_SCC_AUD_CAPABILITY_T;
static const RPC_DESC_T t_rpc_decl_SCC_AUD_UPLOAD_DATA_BUFFER_INFO_T;
static const RPC_DESC_T t_rpc_decl_SCC_AUD_UPLOAD_PCM_INFO_T;
static const RPC_DESC_T t_rpc_decl_SCC_AUD_KARAOKE_INFO_T;
static const RPC_DESC_T t_rpc_decl_SVL_SIMPLE_DATE_TIME_T;
static const RPC_DESC_T t_rpc_decl_SVL_ANALOG_T;
static const RPC_DESC_T t_rpc_decl_SVL_ATSC_T;
static const RPC_DESC_T t_rpc_decl_SVL_DVB_TS_SVC_ID_INFO_T;
static const RPC_DESC_T t_rpc_decl_SVL_DVB_SBTL_PREF_T;
static const RPC_DESC_T t_rpc_decl_SVL_DVB_T;
static const RPC_DESC_T t_rpc_decl_SVL_ISDB_T;
static const RPC_DESC_T t_rpc_decl_SVL_DTMB_T;
static const RPC_DESC_T t_rpc_decl_SVL_FMRDO_T;
static const RPC_DESC_T t_rpc_decl_SVL_REC_HEADER_T;
static const RPC_DESC_T t_rpc_decl_SVL_REC_T;
static const RPC_DESC_T t_rpc_decl_SVL_CLIENT_DATA_T;
static const RPC_DESC_T t_rpc_decl_SVL_MULTI_LANG_T;
static const RPC_DESC_T t_rpc_decl_SVL_MEM_DUMP_T;
static const RPC_DESC_T t_rpc_decl_FREQ_INFO_T;
static const RPC_DESC_T t_rpc_decl_ALTERNATE_TUNE_INFO_T;
static const RPC_DESC_T t_rpc_decl_TSL_REC_REFERENCE_T;
static const RPC_DESC_T t_rpc_decl_ISDB_TER_DIG_INFO_T;
static const RPC_DESC_T t_rpc_decl_TSL_REC_T;
static const RPC_DESC_T t_rpc_decl_TSL_MEM_DUMP_T;
static const RPC_DESC_T t_rpc_decl_EVCTX_KEY_TYPE_MASK_T;
static const RPC_DESC_T t_rpc_decl_EVCTX_COMMAND_T_u;
static const RPC_DESC_T t_rpc_decl_EVCTX_COMMAND_T;
static const RPC_DESC_T t_rpc_decl_EVCTX_OFFSET_T;
static const RPC_DESC_T t_rpc_decl_EVCTX_BRIEF_INFO_T;
static const RPC_DESC_T t_rpc_decl_EVCTX_EVT_CAT_T;
static const RPC_DESC_T t_rpc_decl_EVCTX_EVT_CAT_LIST_T;
static const RPC_DESC_T t_rpc_decl_EVCTX_RATING_T;
static const RPC_DESC_T t_rpc_decl_EVCTX_RATING_LIST_T;
static const RPC_DESC_T t_rpc_decl_EVCTX_CA_SYSTEM_ID_LIST_T;
static const RPC_DESC_T t_rpc_decl_EVCTX_EVENT_LINKAGE_T;
static const RPC_DESC_T t_rpc_decl_EVCTX_EVENT_LINKAGE_LIST_T;
static const RPC_DESC_T t_rpc_decl_EVCTX_IDX_LANG_T;
static const RPC_DESC_T t_rpc_decl_SM_CC_AUTO_SEL_T;
static const RPC_DESC_T t_rpc_decl_SM_CC_FILTER_T;
static const RPC_DESC_T t_rpc_decl_SM_CCH_NTSC_CC_VBI_PSR_CB_INFO_T;
static const RPC_DESC_T t_rpc_decl_SM_CCH_DTVCC_COLOR_T;
static const RPC_DESC_T t_rpc_decl_SM_CCH_DTVCC_ATTRIBS_INFO_T;
static const RPC_DESC_T t_rpc_decl_SM_CCH_EXISTED_CC_IND_T;
static const RPC_DESC_T t_rpc_decl_SM_CCH_CONFIG_T;
static const RPC_DESC_T t_rpc_decl_SM_SBTLH_STREAM_INFO;
static const RPC_DESC_T t_rpc_decl_SM_SBTLH_CONFIG_T;
static const RPC_DESC_T t_rpc_decl_SM_SBTLH_RENDER_INFO_T;
static const RPC_DESC_T t_rpc_decl_CP_INFO_T;
static const RPC_DESC_T t_rpc_decl_CP_INFO_LIST_T;
static const RPC_DESC_T t_rpc_decl_CA_ALG_INFO_T;
static const RPC_DESC_T t_rpc_decl_CA_KEY_INFO_T;
static const RPC_DESC_T t_rpc_decl_CA_SET_COMMAND_INFO_T_u_set_cmd_info;
static const RPC_DESC_T t_rpc_decl_CA_SET_COMMAND_INFO_T;
static const RPC_DESC_T t_rpc_decl_ANALOG_BRDCST_TIME_OUT_T;
static const RPC_DESC_T t_rpc_decl_ANALOG_BRDCST_CTRL_SCART_INP_CAP_T;
static const RPC_DESC_T t_rpc_decl_ANALOG_BRDCST_CTRL_SCART_BYPASS_T;
static const RPC_DESC_T t_rpc_decl_ANALOG_BRDCST_CTRL_TV_AUD_SYS_MASK_T;
static const RPC_DESC_T t_rpc_decl_ANALOG_BRDCST_CTRL_TUNE_ATTRIBUTES_T;
static const RPC_DESC_T t_rpc_decl_SILENT_SELECT_INFO_T;
static const RPC_DESC_T t_rpc_decl_CHANNEL_DECODE_DESC_T;
static const RPC_DESC_T t_rpc_decl_SRC_SVL_INFO_T;
static const RPC_DESC_T t_rpc_decl_SRC_FREQ_INFO_T;
static const RPC_DESC_T t_rpc_decl_AVC_HANDLER_TIME_OUT_T;
static const RPC_DESC_T t_rpc_decl_AVC_HANDLER_CTRL_SCART_INP_CAP_T;
static const RPC_DESC_T t_rpc_decl_SRC_AVC_HINT_T;
static const RPC_DESC_T t_rpc_decl_SRC_AVC_T;
static const RPC_DESC_T t_rpc_decl_SRC_AVC_INFO_T;
static const RPC_DESC_T t_rpc_decl_FE_INIT_DATA;
static const RPC_DESC_T t_rpc_decl_FE_INIT_FILE;
static const RPC_DESC_T t_rpc_decl_GL_SCREEN_STATUS_T;
static const RPC_DESC_T t_rpc_decl_GL_PLANE_CAPS_T;
static const RPC_DESC_T t_rpc_decl_GL_PLANE_CFG_T;
static const RPC_DESC_T t_rpc_decl_GL_PLANE_STATUS_T;
static const RPC_DESC_T t_rpc_decl_GL_POINT_T;
static const RPC_DESC_T t_rpc_decl_GL_SIZE_T;
static const RPC_DESC_T t_rpc_decl_GL_RECT_T;
static const RPC_DESC_T t_rpc_decl_GL_MATRIX2D_T;
static const RPC_DESC_T t_rpc_decl_GL_COLOR_T;
static const RPC_DESC_T t_rpc_decl_GL_DISPLAY_OPT_T;
static const RPC_DESC_T t_rpc_decl_GL_SURFACE_DESC_T;
static const RPC_DESC_T t_rpc_decl_GL_SURF_INFO_T;
static const RPC_DESC_T t_rpc_decl_GL_LOCK_INFO_T_at_subplane;
static const RPC_DESC_T t_rpc_decl_GL_LOCK_INFO_T;
static const RPC_DESC_T t_rpc_decl_GL_ARC_DESC_T;
static const RPC_DESC_T t_rpc_decl_GL_GPU_CAPS_T;
static const RPC_DESC_T t_rpc_decl_GL_BLT_EX_T;
static const RPC_DESC_T t_rpc_decl_GL_DRV_UPDATE_INFO_T;
static const RPC_DESC_T t_rpc_decl_GL_DRCS_INFO_T;
static const RPC_DESC_T t_rpc_decl_GL_TTX_COLOR_T;
static const RPC_DESC_T t_rpc_decl_GL_ISDB_CHAR_COLOR_T;
static const RPC_DESC_T t_rpc_decl_GL_ISDB_GRID_T;
static const RPC_DESC_T t_rpc_decl_IMG_GIF_FRM_RECT_T;
static const RPC_DESC_T t_rpc_decl_VID_PLA_NFY_INFO_T;
static const RPC_DESC_T t_rpc_decl_VID_PLA_DISP_DIMENSION_T;
static const RPC_DESC_T t_rpc_decl_VID_PLA_INJECT_VDP_T;
static const RPC_DESC_T t_rpc_decl_VID_PLA_BG_COLOR_T;
static const RPC_DESC_T t_rpc_decl_VID_PLA_VID_REGION_T;
static const RPC_DESC_T t_rpc_decl_VID_PLA_VID_REGION_CAPABILITY_T;
static const RPC_DESC_T t_rpc_decl_VID_PLA_GET_ASPECT_RATIO_TYPE_T;
static const RPC_DESC_T t_rpc_decl_VID_PLA_ENHANCE_T;
static const RPC_DESC_T t_rpc_decl_VID_PLA_QV_INFO_T;
static const RPC_DESC_T t_rpc_decl_VID_PLA_MIN_MAX_INFO_T;
static const RPC_DESC_T t_rpc_decl_VID_PLA_COLOR_GAIN_T;
static const RPC_DESC_T t_rpc_decl_VID_PLA_OVER_SCAN_CLIPPER_T;
static const RPC_DESC_T t_rpc_decl_VID_PLA_MAX_INFO_T;
static const RPC_DESC_T t_rpc_decl_VID_PLA_LB_DETECT_NFY_INFO_T;
static const RPC_DESC_T t_rpc_decl_VID_PLA_LB_DETECT_CONFIG_T;
static const RPC_DESC_T t_rpc_decl_VID_PLA_TIME_CODE_INFO_T;
static const RPC_DESC_T t_rpc_decl_VID_PLA_PICTURE_INFO_T;
static const RPC_DESC_T t_rpc_decl_VID_PLA_SUPER_FREEZE_T;
static const RPC_DESC_T t_rpc_decl_VID_PLA_SUPER_BLANK_T;
static const RPC_DESC_T t_rpc_decl_VID_PLA_ATOMIC_CMD_T_u;
static const RPC_DESC_T t_rpc_decl_VID_PLA_ATOMIC_CMD_T;
static const RPC_DESC_T t_rpc_decl_VID_PLA_FORCE_UNMUTE_T;
static const RPC_DESC_T t_rpc_decl_CAP_INPUT_T;
static const RPC_DESC_T t_rpc_decl_VID_PLA_ANGULAR_SCE_T;
static const RPC_DESC_T t_rpc_decl_EXIF_RATIONAL_DATA;
static const RPC_DESC_T t_rpc_decl_EXIF_SRATIONAL_DATA;
static const RPC_DESC_T t_rpc_decl_EXIF_INFO_T;
static const RPC_DESC_T t_rpc_decl_EXIF_CONTEXT_DATA;
static const RPC_DESC_T t_rpc_decl_IMG_ENG_DATA_T;
static const RPC_DESC_T t_rpc_decl_IMG_ANIMATE_T;
static const RPC_DESC_T t_rpc_decl_IMG_TYPE_DATA_T;
static const RPC_DESC_T t_rpc_decl_IMG_ENG_TYPE_DATA_T;
static const RPC_DESC_T t_rpc_decl_IMG_CAPS_T;
static const RPC_DESC_T t_rpc_decl_IMG_NFY_LOAD_WITH_TAG_T;
static const RPC_DESC_T t_rpc_decl_IMG_NFY_GIF_LOAD_PARAM_T;
static const RPC_DESC_T t_rpc_decl_CAP_OUTPUT_T;
static const RPC_DESC_T t_rpc_decl_IMAGE_FILE_INFO_T;
static const RPC_DESC_T t_rpc_decl_IMAGE_PULL_FM_FUNC_T;
static const RPC_DESC_T t_rpc_decl_WGL_ANIM_CONFIG_T;
static const RPC_DESC_T t_rpc_decl_WGL_CANVAS_CFG_T;
static const RPC_DESC_T t_rpc_decl_WGL_CVS_CLR_CFG_T;
static const RPC_DESC_T t_rpc_decl_WGL_CANVAS_INFO_T;
static const RPC_DESC_T t_rpc_decl_WGL_NOTIFY_DATA_T;
static const RPC_DESC_T t_rpc_decl_WGL_CREATE_INFO_T;
static const RPC_DESC_T t_rpc_decl_WGL_PACKED_PARAM_T;
static const RPC_DESC_T t_rpc_decl_WGL_KEY_LINK_T;
static const RPC_DESC_T t_rpc_decl_WGL_IMG_DECODE_FUNCTION_TABLE_T;
static const RPC_DESC_T t_rpc_decl_WGL_COLOR_BASIC_SET_T;
static const RPC_DESC_T t_rpc_decl_WGL_COLOR_STD_SET_T;
static const RPC_DESC_T t_rpc_decl_WGL_COLOR_EXT_SET_T;
static const RPC_DESC_T t_rpc_decl_WGL_COLOR_INFO_T;
static const RPC_DESC_T t_rpc_decl_WGL_IMG_BASIC_SET_T;
static const RPC_DESC_T t_rpc_decl_WGL_IMG_STD_SET_T;
static const RPC_DESC_T t_rpc_decl_WGL_IMG_EXT_SET_T;
static const RPC_DESC_T t_rpc_decl_WGL_IMG_INFO_T;
static const RPC_DESC_T t_rpc_decl_WGL_FONT_INFO_T;
static const RPC_DESC_T t_rpc_decl_WGL_FONT_BASIC_SET_T;
static const RPC_DESC_T t_rpc_decl_WGL_FONT_INFO_EX_T;
static const RPC_DESC_T t_rpc_decl_WGL_INSET_T;
static const RPC_DESC_T t_rpc_decl_WGL_TEXT_ATTR_T;
static const RPC_DESC_T t_rpc_decl_WGL_UPD_REGION_T;
static const RPC_DESC_T t_rpc_decl_WGL_SBTL_REGION_T;
static const RPC_DESC_T t_rpc_decl_WGL_SBTL_INIT_T;
static const RPC_DESC_T t_rpc_decl_WGL_SBTL_LOGI_SZ_T;
static const RPC_DESC_T t_rpc_decl_SBTL_MKV_CONTENT_ENCODING_T;
static const RPC_DESC_T t_rpc_decl_SBTL_MKV_CONTENT_ENCODINGS_T;
static const RPC_DESC_T t_rpc_decl_SBTL_MKV_CODEC_PRIVATE_T;
static const RPC_DESC_T t_rpc_decl_SBTL_MKV_SBTL_CMD_T;
static const RPC_DESC_T t_rpc_decl_SBTL_DISPLAY_INFO_CMD_T;
static const RPC_DESC_T t_rpc_decl_SBTL_LYRIC_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_SBTL_DISP_PAGE_T;
static const RPC_DESC_T t_rpc_decl_MM_SBTL_TM_OFST_INF;
static const RPC_DESC_T t_rpc_decl_MM_SBTL_RECORD_INF;
static const RPC_DESC_T t_rpc_decl_MM_SBTL_PTS_CB;
static const RPC_DESC_T t_rpc_decl_SMAI_CC_INF_T;
static const RPC_DESC_T t_rpc_decl_SAMI_STREAM_INFO_T;
static const RPC_DESC_T t_rpc_decl_SBTL_LANG_DESC_T;
static const RPC_DESC_T t_rpc_decl_MM_EXT_SBTL_INFO_u;
static const RPC_DESC_T t_rpc_decl_MM_EXT_SBTL_INFO;
static const RPC_DESC_T t_rpc_decl_MM_LYRIC_INFO_u;
static const RPC_DESC_T t_rpc_decl_MM_LYRIC_INFO;
static const RPC_DESC_T t_rpc_decl_MM_SVC_COMMAND_T_u;
static const RPC_DESC_T t_rpc_decl_MM_SVC_COMMAND_T;
static const RPC_DESC_T t_rpc_decl_DIVX_DRM_INFO_SVCTX_T;
static const RPC_DESC_T t_rpc_decl_DRM_INFO_SVCTX_T_u;
static const RPC_DESC_T t_rpc_decl_DRM_INFO_SVCTX_T;
static const RPC_DESC_T t_rpc_decl_MM_SVC_SBTL_DISP_MODE_T;
static const RPC_DESC_T t_rpc_decl_MM_SVC_SBTL_HILT_STL_T;
static const RPC_DESC_T t_rpc_decl_MM_SVC_SBTL_TIME_OFST_T;
static const RPC_DESC_T t_rpc_decl_MM_SVC_SBTL_FONT_ENC_T;
static const RPC_DESC_T t_rpc_decl_MM_SVC_SBTL_DISP_ATTR_T;
static const RPC_DESC_T t_rpc_decl_MM_SVC_SBTL_SHOW_HIDE_T;
static const RPC_DESC_T t_rpc_decl_SVC_MM_SBTL_LYRIC_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_VIDEO_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_PUSH_BUF_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_SVC_RENDER_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_SVC_POS_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_SVC_CUR_CHAP_ID_T;
static const RPC_DESC_T t_rpc_decl_MM_SVC_DUR_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_SVC_TIME_CODE_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_SVC_PTS_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_REPEAT_AB_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_SVC_REPEAT_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_SVC_TRICK_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_SVC_CHAPTER_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_SVC_TITLE_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_SVC_BTN_INFO_T_u;
static const RPC_DESC_T t_rpc_decl_MM_SVC_BTN_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_SVC_BTN_ACTION_INFO_T_u;
static const RPC_DESC_T t_rpc_decl_MM_SVC_BTN_ACTION_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_SVC_BUFFCTRL_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_SVC_TIME_TO_SIZE_T;
static const RPC_DESC_T t_rpc_decl_MM_SVC_BUF_STATUS_T;
static const RPC_DESC_T t_rpc_decl_MM_DRM_CTRL_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_MLN_DRM_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_DRM_INFO_T_u;
static const RPC_DESC_T t_rpc_decl_MM_DRM_INFO_T;
static const RPC_DESC_T t_rpc_decl_SRC_MM_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_TIMESHIFT_RANGE_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_STRM_ATRBT_T;
static const RPC_DESC_T t_rpc_decl_MM_STRM_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_3D_VID_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_STRM_CODEC_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_BUF_AGT_FLOW_CTRL_T;
static const RPC_DESC_T t_rpc_decl_MM_SVC_STOP_INFO;
static const RPC_DESC_T t_rpc_decl_ID_CVT_CNDTN_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_MHEG_SCREEN_MODE_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_ANA_AUTO_DETECTED_AUD_SYS_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_SCDB_SORT_PARAM_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_ATOMIC_CMD_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_VID_STREAM_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_AUD_STREAM_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_STREAM_INFO_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_VIDEO_MUTE_INFO_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_GET_SCDB_NUM_RECS_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_GET_SCDB_REC_BY_IDX_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_GET_USING_SCDB_REC_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_COPY_PROTECTION_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_CA_INFO_T;
static const RPC_DESC_T t_rpc_decl_SIGNAL_LOSS_DELAY_T;
static const RPC_DESC_T t_rpc_decl_SCART_OUT_DESC_T;
static const RPC_DESC_T t_rpc_decl_SCART_OUT_INFO_T;
static const RPC_DESC_T t_rpc_decl_CH_BLOCK_BIAS_FREQ_T;
static const RPC_DESC_T t_rpc_decl_RIKS_TV_T;
static const RPC_DESC_T t_rpc_decl_SRC_DESC_T_u_info;
static const RPC_DESC_T t_rpc_decl_SRC_DESC_T;
static const RPC_DESC_T t_rpc_decl_STRM_CC_DESC_T;
static const RPC_DESC_T t_rpc_decl_STRM_SUBTITLE_DESC_T;
static const RPC_DESC_T t_rpc_decl_STRM_TELTEX_DESC_T;
static const RPC_DESC_T t_rpc_decl_SNK_DESC_T_u_text_strm;
static const RPC_DESC_T t_rpc_decl_SNK_DESC_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_ASSC_CFG_T;
static const RPC_DESC_T t_rpc_decl_EVENT_CAPABILITY_T;
static const RPC_DESC_T t_rpc_decl_TEXT_CAPABILITY_T;
static const RPC_DESC_T t_rpc_decl_DEVICE_TYPE_PROFILE_T;
static const RPC_DESC_T t_rpc_decl_TUNER_OUT_INFO_T;
static const RPC_DESC_T t_rpc_decl_TUNER_OUT_DESC_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_VIDEO_THUMBNAIL_INFO_T;
static const RPC_DESC_T t_rpc_decl_ID_IND_T;
static const RPC_DESC_T t_rpc_decl_ID_LIST_T;
static const RPC_DESC_T t_rpc_decl_ID_RANGE_T;
static const RPC_DESC_T t_rpc_decl_DRV_COMP_ID_T;
static const RPC_DESC_T t_rpc_decl_DRV_COMP_REG_T_u;
static const RPC_DESC_T t_rpc_decl_DRV_COMP_REG_T;
static const RPC_DESC_T t_rpc_decl_DRV_COMP_FCT_TBL_T;
static const RPC_DESC_T t_rpc_decl_DIVX_DRM_CRYPTO_INFO_T;
static const RPC_DESC_T t_rpc_decl_DIVX_DRM_INFO_DRV_T;
static const RPC_DESC_T t_rpc_decl_DRM_INFO_DRV_T_u;
static const RPC_DESC_T t_rpc_decl_DRM_INFO_DRV_T;
static const RPC_DESC_T t_rpc_decl_DIVX_DRM_HW_SECRET_KEY_T;
static const RPC_DESC_T t_rpc_decl_FEEDER_SRC_INPUT_FCT_TBL_T;
static const RPC_DESC_T t_rpc_decl_MEDIA_FEEDER_T;
static const RPC_DESC_T t_rpc_decl_MM_PB_CTRL_PULL_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_PB_CTRL_PUSH_BUF_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_PB_CTRL_PUSH_MODE_INFO_T;
static const RPC_DESC_T t_rpc_decl_MEDIA_MULTI_CACHE_T;
static const RPC_DESC_T t_rpc_decl_MEDIA_FEEDER_NW_T;
static const RPC_DESC_T t_rpc_decl_STREAM_ID_PS_T;
static const RPC_DESC_T t_rpc_decl_STREAM_ID_T;
static const RPC_DESC_T t_rpc_decl_PVR_PIC_INFO_T;
static const RPC_DESC_T t_rpc_decl_PVR_TICK_DETAIL_T;
static const RPC_DESC_T t_rpc_decl_PVR_TICK_INDEX_T;
static const RPC_DESC_T t_rpc_decl_PVR_TICK_CTRL_BLK_T;
static const RPC_DESC_T t_rpc_decl_START_POS_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_RANGE_POS_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_POS_T;
static const RPC_DESC_T t_rpc_decl_MM_AB_POS_T;
static const RPC_DESC_T t_rpc_decl_MM_BUF_RANGE_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_REC_BUF_RANGE_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T;
static const RPC_DESC_T t_rpc_decl_MM_RANGE_ELEMT_IDXTBL_TS_ENTRY_T;
static const RPC_DESC_T t_rpc_decl_MM_RANGE_ELMT_IDXTBL_T_u;
static const RPC_DESC_T t_rpc_decl_MM_RANGE_ELMT_IDXTBL_T;
static const RPC_DESC_T t_rpc_decl_MM_RANGE_TRACK_TM_SCALE_T;
static const RPC_DESC_T t_rpc_decl_MM_AUDIO_VORBIS_OFFSET_T;
static const RPC_DESC_T t_rpc_decl_MM_RANGE_DECODER_T;
static const RPC_DESC_T t_rpc_decl_MM_RANGE_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_PVR_CRYPT_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_TRACK_CRYPT_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_ML_CRYPT_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_CRYPT_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_CRYPT_IV_T;
static const RPC_DESC_T t_rpc_decl_MM_DESC_CRYPT_INFO_T;
static const RPC_DESC_T t_rpc_decl_MM_CRYPT_CMD_T;
static const RPC_DESC_T t_rpc_decl_REC_UTIL_CLI_DUMP_TICK_CB_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_REC_ES_INFO_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_REC_ES_LIST_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_REC_DEL_DATA_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_REC_ATTR_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_REC_TIME_BOUND_INFO_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_REC_SPACE_BOUND_INFO_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_REC_STRG_DESC_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_REC_CONT_INFO_NORMAL_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_REC_CONT_INFO_CONT_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_REC_CONFIG_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_REC_CAP_INFO_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_REC_SYS_RES_INFO_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_REC_STATUS_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_REC_ES_AUTH_INFO_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_REC_ESL_AUTH_STATUS_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_REC_META_DB_OPEN_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_REC_META_DB_CLOSE_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_REC_META_DB_STORE_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_REC_SET_META_INFO_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_REC_GET_META_INFO_LEN_T;
static const RPC_DESC_T t_rpc_decl_SVCTX_REC_GET_META_INFO_T;
static const RPC_DESC_T t_rpc_decl_SRC_REC_INFO_T;
static const RPC_DESC_T t_rpc_decl_AVC_TYPE_MAPPING;
static const RPC_DESC_T t_rpc_decl_FM_FLASH_PART_BULK_WRITE_T;
static const RPC_DESC_T t_rpc_decl_FM_DIR_INFO_T;
static const RPC_DESC_T t_rpc_decl_FM_FILE_INFO_T;
static const RPC_DESC_T t_rpc_decl_FM_DIR_ENTRY_T;
static const RPC_DESC_T t_rpc_decl_FM_FS_INFO_T;
static const RPC_DESC_T t_rpc_decl_FM_MNT_PARM_T;
static const RPC_DESC_T t_rpc_decl_FM_PART_INFO_T;
static const RPC_DESC_T t_rpc_decl_FM_MNT_CB_T;
static const RPC_DESC_T t_rpc_decl_FM_CONFIRM_FILE_REPLACE_T;
static const RPC_DESC_T t_rpc_decl_FM_CONFIRM_DIR_REPLACE_T;
static const RPC_DESC_T t_rpc_decl_FM_COPY_FILE_PRGS_T;
static const RPC_DESC_T t_rpc_decl_FM_SEARCH_RESULT_T;
static const RPC_DESC_T t_rpc_decl_FLM_LIST_T;
static const RPC_DESC_T t_rpc_decl_FLM_REC_T;
static const RPC_DESC_T t_rpc_decl_RRCTX_VCHIP_TYPE_T;
static const RPC_DESC_T t_rpc_decl_RRCTX_VCHIP_RATE_SETTING_T;
static const RPC_DESC_T t_rpc_decl_RRCTX_RRT_RATE_SETTING_T;
static const RPC_DESC_T t_rpc_decl_DTG_T;
static const RPC_DESC_T t_rpc_decl_DELTA_DTG_T;
static const RPC_DESC_T t_rpc_decl_DT_CONFIG_T;
static const RPC_DESC_T t_rpc_decl_TM_SECOND_LOOP_ALL_DESCRS_T;
static const RPC_DESC_T t_rpc_decl_TM_SECOND_LOOP_DESCR_SEL_T;
static const RPC_DESC_T t_rpc_decl_TM_SECOND_LOOP_ALL_MULTI_DESCRS_T;
static const RPC_DESC_T t_rpc_decl_TM_SECOND_LOOP_MULTI_DESCR_SEL_T;
static const RPC_DESC_T t_rpc_decl_TM_FIRST_LOOP_DESCR_SEL_T;
static const RPC_DESC_T t_rpc_decl_TM_MAIN_BODY_DESCR_SEL_T;
static const RPC_DESC_T t_rpc_decl_TM_UPDATE_TIME_T;
static const RPC_DESC_T t_rpc_decl_TM_BUFFER_T;
static const RPC_DESC_T t_rpc_decl_ATSC_SI_MGT_INFO_T;
static const RPC_DESC_T t_rpc_decl_ATSC_SI_VCT_INFO_T;
static const RPC_DESC_T t_rpc_decl_ATSC_SI_RRT_INFO_T;
static const RPC_DESC_T t_rpc_decl_ATSC_SI_EIT_INFO_T;
static const RPC_DESC_T t_rpc_decl_ATSC_SI_TXT_LANG_INFO_T;
static const RPC_DESC_T t_rpc_decl_ATSC_SI_TXT_SEG_INFO_T;
static const RPC_DESC_T t_rpc_decl_ATSC_SI_LANG_SEG_IDX_INFO_T;
static const RPC_DESC_T t_rpc_decl_ATSC_SI_TXT_INFO_T;
static const RPC_DESC_T t_rpc_decl_ATSC_SI_TXT_DIM_VAL_IDX_INFO_T;
static const RPC_DESC_T t_rpc_decl_ATSC_SI_IDX_TIME_INFO_T;
static const RPC_DESC_T t_rpc_decl_ATSC_SI_SRC_ID_TIME_INFO_T;
static const RPC_DESC_T t_rpc_decl_ATSC_SI_SRC_EVT_ID_TIME_INFO_T;
static const RPC_DESC_T t_rpc_decl_EAS_EVENT_T;
static const RPC_DESC_T t_rpc_decl_EAS_LOC_INFO_T;
static const RPC_DESC_T t_rpc_decl_EAS_XCEPT_INFO_T;
static const RPC_DESC_T t_rpc_decl_EAS_EVT_CODE_INFO_T;
static const RPC_DESC_T t_rpc_decl_ATSC_SI_CONFIG_T;
static const RPC_DESC_T t_rpc_decl_SB_FREQ_RANGE_T;
static const RPC_DESC_T t_rpc_decl_SB_NEW_SVC_NFY_DATA;
static const RPC_DESC_T t_rpc_decl_SB_LCNV2_CH_LST_INFO;
static const RPC_DESC_T t_rpc_decl_SB_ATSC_OPEN_DATA_T;
static const RPC_DESC_T t_rpc_decl_SB_ATSC_SCAN_DATA_T;
static const RPC_DESC_T t_rpc_decl_SB_NTSC_OPEN_DATA_T;
static const RPC_DESC_T t_rpc_decl_SB_NTSC_SCAN_DATA_T;
static const RPC_DESC_T t_rpc_decl_SB_CQAM_OPEN_DATA_T;
static const RPC_DESC_T t_rpc_decl_SB_CQAM_SCAN_DATA_T;
static const RPC_DESC_T t_rpc_decl_SB_DVB_REGION_CODE_T;
static const RPC_DESC_T t_rpc_decl_SB_DVB_REGION_T;
static const RPC_DESC_T t_rpc_decl_SB_DVB_CH_LST_LCN_INFO;
static const RPC_DESC_T t_rpc_decl_SB_DVB_SCAN_INDEX_INFO;
static const RPC_DESC_T t_rpc_decl_SB_DVB_FREQ_BW_T;
static const RPC_DESC_T t_rpc_decl_SB_DVB_INIT_DATA_T;
static const RPC_DESC_T t_rpc_decl_SB_DVB_OPEN_DATA_T;
static const RPC_DESC_T t_rpc_decl_SB_DVB_SCAN_INFO_T;
static const RPC_DESC_T t_rpc_decl_SB_DVB_SVC_UPDATE_NFY_DATA;
static const RPC_DESC_T t_rpc_decl_SB_DVB_SCAN_DATA_T;
static const RPC_DESC_T t_rpc_decl_SB_DVB_TSL_REC_ID_FREQ_RANGE_T;
static const RPC_DESC_T t_rpc_decl_SB_DVB_ON_TS_SVC_ID_T;
static const RPC_DESC_T t_rpc_decl_SB_DVB_COUNTRY_ONID_T;
static const RPC_DESC_T t_rpc_decl_SB_DVB_ONID_COUNT_INFO_T;
static const RPC_DESC_T t_rpc_decl_SB_DVB_GET_OTHER_COUNTRY_ONID_INFO_T;
static const RPC_DESC_T t_rpc_decl_SB_DVB_NW_CHANGE_NFY_T;
static const RPC_DESC_T t_rpc_decl_REGION_CODE_INFO_t_link;
static const RPC_DESC_T t_rpc_decl_REGION_CODE_INFO;
static const RPC_DESC_T t_rpc_decl_REGION_CODE_INFO_LIST;
static const RPC_DESC_T t_rpc_decl_SB_DVB_LCN_CONFLICT_GROUP_T;
static const RPC_DESC_T t_rpc_decl_SB_DVB_LCN_CONFLICT_CHANNEL_T;
static const RPC_DESC_T t_rpc_decl_SB_DVBC_INIT_DATA_T;
static const RPC_DESC_T t_rpc_decl_SB_DVBC_OPEN_DATA_T;
static const RPC_DESC_T t_rpc_decl_SB_DVBC_SCAN_INFO_T;
static const RPC_DESC_T t_rpc_decl_SB_DVBC_DLVR_INFO_T;
static const RPC_DESC_T t_rpc_decl_SB_DVBC_TUNER_PARA_T;
static const RPC_DESC_T t_rpc_decl_SB_DVBC_SVC_UPDATE_NFY_DATA;
static const RPC_DESC_T t_rpc_decl_SB_DVB_SCANNED_CH_T;
static const RPC_DESC_T t_rpc_decl_SB_DVBC_NW_CHANGE_NFY_DATA;
static const RPC_DESC_T t_rpc_decl_SB_DVBC_SCAN_DATA_T;
static const RPC_DESC_T t_rpc_decl_SB_DVBC_TSL_REC_ID_FREQ_RANGE_T;
static const RPC_DESC_T t_rpc_decl_SB_DVBC_ON_TS_SVC_ID_T;
static const RPC_DESC_T t_rpc_decl_SB_DVBC_NW_CHANGE_NFY_T;
static const RPC_DESC_T t_rpc_decl_VBIF_SRC_ANA_VBI_T;
static const RPC_DESC_T t_rpc_decl_VBIF_SRC_DIG_VBI_ES_T;
static const RPC_DESC_T t_rpc_decl_VBIF_SRC_DIG_TTX_ES_T;
static const RPC_DESC_T t_rpc_decl_FLTR_INIT_TTX_PKT_T;
static const RPC_DESC_T t_rpc_decl_TTX_PKT_INFO_T;
static const RPC_DESC_T t_rpc_decl_FLTR_NFY_TTX_PKT_SRC_ANA_VBI_T;
static const RPC_DESC_T t_rpc_decl_FLTR_TTX_PKT_BUFF_ATTR_ANA_VBI_T;
static const RPC_DESC_T t_rpc_decl_FLTR_NFY_TTX_PKT_SRC_DIG_VBI_ES_T;
static const RPC_DESC_T t_rpc_decl_FLTR_TTX_PKT_BUFF_ATTR_DIG_VBI_ES_T;
static const RPC_DESC_T t_rpc_decl_FLTR_NFY_TTX_PKT_SRC_DIG_TTX_ES_T;
static const RPC_DESC_T t_rpc_decl_FLTR_TTX_PKT_BUFF_ATTR_DIG_TTX_ES_T;
static const RPC_DESC_T t_rpc_decl_FLTR_TTX_PKT_BUFF_INFO_T_u;
static const RPC_DESC_T t_rpc_decl_FLTR_TTX_PKT_BUFF_INFO_T;
static const RPC_DESC_T t_rpc_decl_FLTR_TTX_PKT_BUFF_TAG_T;
static const RPC_DESC_T t_rpc_decl_FLTR_TTX_PKT_BUFF_LOCK_INFO_T;
static const RPC_DESC_T t_rpc_decl_FLTR_TTX_PKT_BUFF_ATTR_T;
static const RPC_DESC_T t_rpc_decl_FLTR_NFY_TTX_PKT_T;
static const RPC_DESC_T t_rpc_decl_FLTR_INIT_TTX_MIP_T;
static const RPC_DESC_T t_rpc_decl_TTX_MIP_NODE_T;
static const RPC_DESC_T t_rpc_decl_FLTR_NFY_TTX_MIP_T;
static const RPC_DESC_T t_rpc_decl_FLTR_NI_INIT_TTX_HDR_CH_NAME_T;
static const RPC_DESC_T t_rpc_decl_FLTR_NI_INIT_ACI_T;
static const RPC_DESC_T t_rpc_decl_FLTR_INIT_NI_T;
static const RPC_DESC_T t_rpc_decl_FLTR_NI_NFY_TTX_HDR_CH_NAME_T;
static const RPC_DESC_T t_rpc_decl_FLTR_NI_NFY_VPS_DATA_T;
static const RPC_DESC_T t_rpc_decl_FLTR_NI_NFY_8_30_FMT_1_PRG_TTL_T;
static const RPC_DESC_T t_rpc_decl_FLTR_NI_NFY_TTX_DATE_TIME_T;
static const RPC_DESC_T t_rpc_decl_FLTR_NI_NFY_ACI_DETECTED_T;
static const RPC_DESC_T t_rpc_decl_FLTR_NI_NFY_ACI_SLCTN_PAGE_UPDATE_T;
static const RPC_DESC_T t_rpc_decl_FLTR_NI_NFY_ACI_DB_READY_T;
static const RPC_DESC_T t_rpc_decl_FLTR_NI_NFY_ACI_T;
static const RPC_DESC_T t_rpc_decl_FLTR_NFY_NI_T_u;
static const RPC_DESC_T t_rpc_decl_FLTR_NFY_NI_T;
static const RPC_DESC_T t_rpc_decl_FLTR_NI_ACI_DB_INFO_T;
static const RPC_DESC_T t_rpc_decl_FLTR_NI_ACI_CITY_LIST_INFO_T;
static const RPC_DESC_T t_rpc_decl_FLTR_NI_ACI_CITY_INFO_T;
static const RPC_DESC_T t_rpc_decl_FLTR_NI_ACI_CH_NUM_T;
static const RPC_DESC_T t_rpc_decl_FLTR_NI_ACI_CH_INFO_T;
static const RPC_DESC_T t_rpc_decl_FLTR_INIT_EU_EVN_T;
static const RPC_DESC_T t_rpc_decl_FLTR_EU_EVN_DATA_PRG_TTL_T;
static const RPC_DESC_T t_rpc_decl_FLTR_NFY_EU_EVN_T;
static const RPC_DESC_T t_rpc_decl_FLTR_INIT_EPG_T;
static const RPC_DESC_T t_rpc_decl_FLTR_NFY_EPG_T;
static const RPC_DESC_T t_rpc_decl_FLTR_INIT_EU_WSS_T;
static const RPC_DESC_T t_rpc_decl_FLTR_NFY_EU_WSS_T;
static const RPC_DESC_T t_rpc_decl_FLTR_INIT_EU_VBI_D2A_T;
static const RPC_DESC_T t_rpc_decl_SB_PAL_SECAM_INIT_DATA_T;
static const RPC_DESC_T t_rpc_decl_SB_PAL_SECAM_OPEN_DATA_T;
static const RPC_DESC_T t_rpc_decl_SB_PAL_SECAM_FREQ_PROGRESS_NFY_DATA;
static const RPC_DESC_T t_rpc_decl_SB_PAL_SECAM_ANAS_NFY_DATA;
static const RPC_DESC_T t_rpc_decl_SB_PAL_SECAM_CUSTOM_SVL_CONFIG;
static const RPC_DESC_T t_rpc_decl_SB_PAL_SECAM_ACI_NFY_DATA;
static const RPC_DESC_T t_rpc_decl_SB_PAL_SECAM_USER_OPERATION_DATA;
static const RPC_DESC_T t_rpc_decl_SB_PAL_SECAM_SCAN_DATA_T;
static const RPC_DESC_T t_rpc_decl_SB_ACI_SLCT_CITY_T;
static const RPC_DESC_T t_rpc_decl_SB_USER_OPERATION_T;
static const RPC_DESC_T t_rpc_decl_SB_INSERT_DEFAULT_SVL_TSL_T;
static const RPC_DESC_T t_rpc_decl_SB_ISDB_INIT_DATA_T;
static const RPC_DESC_T t_rpc_decl_SB_ISDB_OPEN_DATA_T;
static const RPC_DESC_T t_rpc_decl_SB_ISDB_SCAN_INFO_T;
static const RPC_DESC_T t_rpc_decl_SB_ISDB_SCAN_DATA_T;
static const RPC_DESC_T t_rpc_decl_LOL_REC_T;
static const RPC_DESC_T t_rpc_decl_LOL_MEM_DUMP_T;
static const RPC_DESC_T t_rpc_decl_NWL_REC_T;
static const RPC_DESC_T t_rpc_decl_NWL_MEM_DUMP_T;
static const RPC_DESC_T t_rpc_decl_PCL_GPIO_MONITOR_CTRL_T;
static const RPC_DESC_T t_rpc_decl_PCL_GPIO_MONITOR_CTRL_EX_T;
static const RPC_DESC_T t_rpc_decl_PCL_SIF_INFO_T;
static const RPC_DESC_T t_rpc_decl_PCL_SIF_INFO_BY_GPIO_T;
static const RPC_DESC_T t_rpc_decl_PCL_SERVO_AD_INFO_T;
static const RPC_DESC_T t_rpc_decl_PCL_WAKE_UP_CTRL_T;
static const RPC_DESC_T t_rpc_decl_RPT_EVT_ITVL_T;
static const RPC_DESC_T t_rpc_decl_DISABLE_EVTCODE_T;
static const RPC_DESC_T t_rpc_decl_MAGIC_UNIT_T;
static const RPC_DESC_T t_rpc_decl_MAGIC_T;
static const RPC_DESC_T t_rpc_decl_IRRC_SETTING_T;
static const RPC_DESC_T t_rpc_decl_IOM_RAW_DATA_TO_EVT_CODE_T;
static const RPC_DESC_T t_rpc_decl_IOM_RAW_BITS_TO_EVT_CODE_T;
static const RPC_DESC_T t_rpc_decl_IOM_SYS_CODE_FILTER_T;
static const RPC_DESC_T t_rpc_decl_IRRC_DATA_T;
static const RPC_DESC_T t_rpc_decl_DBG_DATA_T;
static const RPC_DESC_T t_rpc_decl_IOM_IND_T;
static const RPC_DESC_T t_rpc_decl_IOM_FPD_T;
static const RPC_DESC_T t_rpc_decl_IOM_UART_T;
static const RPC_DESC_T t_rpc_decl_UART_SETTING_T;
static const RPC_DESC_T t_rpc_decl_AUTO_SLEEP_TIMER_T;
static const RPC_DESC_T t_rpc_decl_RRCTX_RRT_KEY_PARAM_T;
static const RPC_DESC_T t_rpc_decl_EVCTX_ATSC_KEY_INFO_T;
static const RPC_DESC_T t_rpc_decl_EVCTX_ATSC_RATING_T;
static const RPC_DESC_T t_rpc_decl_EVCTX_ATSC_RATING_LIST_T;
static const RPC_DESC_T t_rpc_decl_EVCTX_ATSC_ENG_CONFIG_T;
static const RPC_DESC_T t_rpc_decl_PLAYBACK_CTRL_PULL_INFO_T;
static const RPC_DESC_T t_rpc_decl_PLAYBACK_FLOW_CTRL_INFO_T;
static const RPC_DESC_T t_rpc_decl_PLAYBACK_FEEDER_BUF_INFO_T;
static const RPC_DESC_T t_rpc_decl_PLAYBACK_MODE_CTRL_T;
static const RPC_DESC_T t_rpc_decl_PLAYBACK_GET_BUFFER_FULLNESS_T;
static const RPC_DESC_T t_rpc_decl_PLAYBACK_GET_BUFFER_STATUS_T;
static const RPC_DESC_T t_rpc_decl_PLAYBACK_HANDLER_ITEM_ATTRIBUTES_T;
static const RPC_DESC_T t_rpc_decl_PLAYBACK_HANDLER_ITEM_ATTRIBUTES_BY_INDEX_T;
static const RPC_DESC_T t_rpc_decl_PLAYBACK_HANDLER_SHARED_MEMORY_T;
static const RPC_DESC_T t_rpc_decl_PLAYBACK_HANDLER_TIME_TO_SIZE_T;
static const RPC_DESC_T t_rpc_decl_PLAYBACK_HANDLER_ITEM_SCDB_INFO_T;
static const RPC_DESC_T t_rpc_decl_PLAYBACK_HANDLER_ITEM_SVC_INFO_T;
static const RPC_DESC_T t_rpc_decl_PLAYBACK_HANDLER_ITEM_SVC_INFO_BY_ID_T;
static const RPC_DESC_T t_rpc_decl_PLAYBACK_HANDLER_ITEM_MPEG_2_INFO_T;
static const RPC_DESC_T t_rpc_decl_PLAYBACK_HANDLER_ITEM_ES_INFO_T;
static const RPC_DESC_T t_rpc_decl_PLAYBACK_CTRL_IBC_PARAM_SET_ASF_PACKET_INFO_T;
static const RPC_DESC_T t_rpc_decl_PLAYBACK_CTRL_IBC_AES_ENCRYPT_INFO_T;
static const RPC_DESC_T t_rpc_decl_PLAYBACK_CTRL_IBC_PARAM_ENCRYPT_INFO_T;
static const RPC_DESC_T t_rpc_decl_PLAYBACK_CTRL_IBC_PARAM_T;
static const RPC_DESC_T t_rpc_decl_PLAYBACK_HANDLER_ITEM_IBC_INFO_T;
static const RPC_DESC_T t_rpc_decl_PLAYBACK_HANDLER_ITEM_EXTRA_INFO_T;
static const RPC_DESC_T t_rpc_decl_PLAYBACK_HANDLER_QUEUE_URI_T;
static const RPC_DESC_T t_rpc_decl_PLAYBACK_HANDLER_QUEUE_FILE_T;
static const RPC_DESC_T t_rpc_decl_PLAYBACK_HANDLER_QUEUE_BUFFER_T;
static const RPC_DESC_T t_rpc_decl_PLAYBACK_HANDLER_QUEUE_PULL_T;
static const RPC_DESC_T t_rpc_decl_PSI_PID_SVC_ID_MAP_INFO_T;
static const RPC_DESC_T t_rpc_decl_PLAYBACK_PMT_LIST_T;
static const RPC_DESC_T t_rpc_decl_CECM_SET_CEC_FCT_PARAMS_T;
static const RPC_DESC_T t_rpc_decl_CECM_ARC_CTRL_T;
static const RPC_DESC_T t_rpc_decl_CECM_ACT_SRC_REVED_T;
static const RPC_DESC_T t_rpc_decl_CECM_VNDR_CMD_DATA_T;
static const RPC_DESC_T t_rpc_decl_CECM_SPEC_CMD_DATA_T;
static const RPC_DESC_T t_rpc_decl_CECM_VNDR_CMD_WITH_ID_DATA_T;
static const RPC_DESC_T t_rpc_decl_CECM_VNDR_RC_CODE_DATA_T;
static const RPC_DESC_T t_rpc_decl_CECM_DEV_LIST_NFY_T;
static const RPC_DESC_T t_rpc_decl_CECM_5V_PWR_INFO_T;
static const RPC_DESC_T t_rpc_decl_CECM_CTRL_T;
static const RPC_DESC_T t_rpc_decl_CECM_DEV_DISCOVERY_T;
static const RPC_DESC_T t_rpc_decl_CECM_LOG_ADD_LST_T;
static const RPC_DESC_T t_rpc_decl_CECM_WAKE_UP_CTRL_T;
static const RPC_DESC_T t_rpc_decl_CECM_ONE_TOUCH_PLAY_T;
static const RPC_DESC_T t_rpc_decl_CECM_ACT_SRC_T;
static const RPC_DESC_T t_rpc_decl_CECM_ROUTING_CHG_INFO_T;
static const RPC_DESC_T t_rpc_decl_CECM_ANA_SVC_T;
static const RPC_DESC_T t_rpc_decl_CECM_DIG_SVC_T;
static const RPC_DESC_T t_rpc_decl_CECM_REC_SRC_T;
static const RPC_DESC_T t_rpc_decl_CECM_TIMER_ANA_T;
static const RPC_DESC_T t_rpc_decl_CECM_TIMER_EXT_T;
static const RPC_DESC_T t_rpc_decl_CECM_TIMER_INFO_T;
static const RPC_DESC_T t_rpc_decl_CECM_TIMER_STS_T;
static const RPC_DESC_T t_rpc_decl_CECM_REPORT_PHY_ADDR_T;
static const RPC_DESC_T t_rpc_decl_CECM_POLLING_INFO_T;
static const RPC_DESC_T t_rpc_decl_CECM_TUNER_SVC_INFO_T;
static const RPC_DESC_T t_rpc_decl_CECM_TUNER_DEV_INFO_T;
static const RPC_DESC_T t_rpc_decl_CECM_OSD_STRING_INFO_T;
static const RPC_DESC_T t_rpc_decl_CECM_USER_CTRL_OPERAND_INFO_T;
static const RPC_DESC_T t_rpc_decl_CECM_USER_CTRL_WITH_OPERAND_T;
static const RPC_DESC_T t_rpc_decl_CECM_PWR_ON_DEV_STS_T;
static const RPC_DESC_T t_rpc_decl_CECM_FEATURE_ABORT_T;
static const RPC_DESC_T t_rpc_decl_CECM_SYS_AUD_CTRL_T;
static const RPC_DESC_T t_rpc_decl_CECM_AUD_DESCRIPTOR_T;
static const RPC_DESC_T t_rpc_decl_CECM_AUD_FMT_T;
static const RPC_DESC_T t_rpc_decl_CECM_DD_VNDR_CMD_PSR_INFO_T;
static const RPC_DESC_T t_rpc_decl_CECM_DD_GET_INFO_T;
static const RPC_DESC_T t_rpc_decl_CECM_DD_VNDR_CMD_INFO_T;
static const RPC_DESC_T t_rpc_decl_CECM_DD_VNDR_CMD_T;
static const RPC_DESC_T t_rpc_decl_CECM_DEV_DISCOVERY_CONFIG_T;
static const RPC_DESC_T t_rpc_decl_CECM_DEV_VNDR_INFO_T;
static const RPC_DESC_T t_rpc_decl_CECM_DEV_T;
static const RPC_DESC_T t_rpc_decl_CECM_NFY_T;
static const RPC_DESC_T t_rpc_decl_SATL_REC_T;
static const RPC_DESC_T t_rpc_decl_SATL_MEM_DUMP_T;
static const RPC_DESC_T t_rpc_decl_LST_MNGR_CLEAN_T;
static const RPC_DESC_T t_rpc_decl_AUD_TYPE_VOL_GAIN_INFO_T;
static const RPC_DESC_T t_rpc_decl_AUD_TYPE_AVL_GAIN_INFO_T;
static const RPC_DESC_T t_rpc_decl_AUD_TYPE_SPECTRUM_INFO_T;
static const RPC_DESC_T t_rpc_decl_AUD_TYPE_TEST_TONE;
static const RPC_DESC_T t_rpc_decl_SMART_AUD_INFO_T;
static const RPC_DESC_T t_rpc_decl_SMART_AUD_GROUP_INFO_T;
static const RPC_DESC_T t_rpc_decl_DRV_CUSTOM_AUD_VOL_CURVE_T;
static const RPC_DESC_T t_rpc_decl_AUD_TYPE_DD_CONTROL_T;
static const RPC_DESC_T t_rpc_decl_DRV_CUSTOM_AUD_TYPE_AQ_VER_T;
static const RPC_DESC_T t_rpc_decl_AUD_TYPE_BASS_BOOSTER;
static const RPC_DESC_T t_rpc_decl_TUNER_TYPE_DIAG_CONTROL_T;
static const RPC_DESC_T t_rpc_decl_TUNER_TYPE_DIAG_CONTENT_T;
static const RPC_DESC_T t_rpc_decl_TUNER_TYPE_DIAG_AGC_T;
static const RPC_DESC_T t_rpc_decl_VID_INFO_T;
static const RPC_DESC_T t_rpc_decl_GAMMA_INFO_T;
static const RPC_DESC_T t_rpc_decl_VID_NON_LINEAR_INFO_T;
static const RPC_DESC_T t_rpc_decl_OUTRGB_INFO_T;
static const RPC_DESC_T t_rpc_decl_SMART_PIC_INFO_T;
static const RPC_DESC_T t_rpc_decl_SMART_PIC_GROUP_INFO_T;
static const RPC_DESC_T t_rpc_decl_PIC_INFO_T;
static const RPC_DESC_T t_rpc_decl_PQ_DEMO_INFO_T;
static const RPC_DESC_T t_rpc_decl_DRV_CUSTOM_GAMMA_VALUE_T;
static const RPC_DESC_T t_rpc_decl_DRV_CUSTOM_GAMMA_DELTA_T;
static const RPC_DESC_T t_rpc_decl_DRV_CUSTOM_PATTERN_T;
static const RPC_DESC_T t_rpc_decl_DRV_GAMMA_BUF_T;
static const RPC_DESC_T t_rpc_decl_DRV_GAMMA_GETBUF_T;
static const RPC_DESC_T t_rpc_decl_DRV_CUSTOM_SCE_PATTERN_T;
static const RPC_DESC_T t_rpc_decl_DRV_CUSTOM_DBC_DEBUG_T;
static const RPC_DESC_T t_rpc_decl_AUTO_BACKLIGHT_SETS_INFO_T;
static const RPC_DESC_T t_rpc_decl_AUTO_COLORTEMP_SETS_INFO_T;
static const RPC_DESC_T t_rpc_decl_AUTO_COLORTEMP_GETS_INFO_T;
static const RPC_DESC_T t_rpc_decl_VID_TYPE_DOT_TO_DOT_PATTERN_T;
static const RPC_DESC_T t_rpc_decl_LVDS_SPREAD_SPECTRUM_INFO_T;
static const RPC_DESC_T t_rpc_decl_VID_TYPE_PANEL_RESOLUTION_T;
static const RPC_DESC_T t_rpc_decl_EXTMJC_TYPE_SET_OSD_MODE_T;
static const RPC_DESC_T t_rpc_decl_MISC_TYPE_TUNER_SIF_T;
static const RPC_DESC_T t_rpc_decl_MISC_TYPE_HDMI_EDID_T;
static const RPC_DESC_T t_rpc_decl_EDID_T;
static const RPC_DESC_T t_rpc_decl_MISC_TYPE_SIF_ADVANCED_T;
static const RPC_DESC_T t_rpc_decl_PANEL_SELECTION_CONFIG_T;
static const RPC_DESC_T t_rpc_decl_DRV_CUST_DRAM_CFG_T;
static const RPC_DESC_T t_rpc_decl_DRV_CUSTOM_MISC_CRNT_INP_SRC_INFO_T;
static const RPC_DESC_T t_rpc_decl_MISC_TYPE_HDMI_HDCP_T;
static const RPC_DESC_T t_rpc_decl_MISC_TYPE_VGA_EDID_T;
static const RPC_DESC_T t_rpc_decl_DRV_CUSTOM_MISC_LOADER_INFO_T;
static const RPC_DESC_T t_rpc_decl_MISC_TYPE_LED_DISPLAY_T;
static const RPC_DESC_T t_rpc_decl_MISC_TYPE_LED_NOTIFY_T;
static const RPC_DESC_T t_rpc_decl_MISC_TYPE_NAND_FLASH_WRITE_T;
static const RPC_DESC_T t_rpc_decl_MISC_TYPE_NAND_FLASH_BULK_WRITE_T;
static const RPC_DESC_T t_rpc_decl_MISC_TYPE_NOR_FLASH_BULK_WRITE_T;
static const RPC_DESC_T t_rpc_decl_AUD_TYPE_SET_INFO_T;
static const RPC_DESC_T t_rpc_decl_AUD_TYPE_GET_INFO_T;
static const RPC_DESC_T t_rpc_decl_AUD_TYPE_GET_MIN_MAX_INFO_T;
static const RPC_DESC_T t_rpc_decl_TUNER_TYPE_SET_INFO_T;
static const RPC_DESC_T t_rpc_decl_TUNER_TYPE_GET_INFO_T;
static const RPC_DESC_T t_rpc_decl_VID_TYPE_SET_INFO_T;
static const RPC_DESC_T t_rpc_decl_VID_TYPE_GET_INFO_T;
static const RPC_DESC_T t_rpc_decl_VID_TYPE_GET_GAMMA_INFO_T;
static const RPC_DESC_T t_rpc_decl_VID_TYPE_GET_MIN_MAX_INFO_T;
static const RPC_DESC_T t_rpc_decl_VID_DEC_TYPE_SET_INFO_T;
static const RPC_DESC_T t_rpc_decl_VID_DEC_TYPE_GET_INFO_T;
static const RPC_DESC_T t_rpc_decl_EXTMJC_TYPE_SET_INFO_T;
static const RPC_DESC_T t_rpc_decl_EXTMJC_TYPE_GET_INFO_T;
static const RPC_DESC_T t_rpc_decl_MISC_TYPE_SET_INFO_T;
static const RPC_DESC_T t_rpc_decl_MISC_TYPE_GET_INFO_T;
static const RPC_DESC_T t_rpc_decl_MISC_TYPE_GET_MIN_MAX_INFO_T;
static const RPC_DESC_T t_rpc_decl_CUST_SPEC_TYPE_SET_INFO_T;
static const RPC_DESC_T t_rpc_decl_CUST_SPEC_TYPE_GET_INFO_T;
static const RPC_DESC_T t_rpc_decl_DRV_CUST_OPERATION_INFO_T_u;
static const RPC_DESC_T t_rpc_decl_DRV_CUST_OPERATION_INFO_T;
static const RPC_DESC_T t_rpc_decl_PSI_TS_VER_INFO_T;
static const RPC_DESC_T t_rpc_decl_PSI_PID_SVC_ID_INFO_T;
static const RPC_DESC_T t_rpc_decl_PSI_STREAM_INFO_T;
static const RPC_DESC_T t_rpc_decl_PSI_CONFIG_T;
static const RPC_DESC_T t_rpc_decl_CAP_CAPTURE_INFO_T;
static const RPC_DESC_T t_rpc_decl_CAP_CAPABILITY_INFO_T;
static const RPC_DESC_T t_rpc_decl_CAP_LOGO_SAVE_INFO_T_u;
static const RPC_DESC_T t_rpc_decl_CAP_LOGO_SAVE_INFO_T;
static const RPC_DESC_T t_rpc_decl_CAP_OBJ_T;
static const RPC_DESC_T t_rpc_decl_CI_MMI_MENU_ITEM_LIST_T;
static const RPC_DESC_T t_rpc_decl_CI_MMI_ENQ_T;
static const RPC_DESC_T t_rpc_decl_CI_MMI_ENQ_ANS_T;
static const RPC_DESC_T t_rpc_decl_CI_MMI_ENQ_CLOSE_T;
static const RPC_DESC_T t_rpc_decl_CI_MMI_MENU_ITEM_T_t_link;
static const RPC_DESC_T t_rpc_decl_CI_MMI_MENU_ITEM_T;
static const RPC_DESC_T t_rpc_decl_CI_MMI_MENU_T;
static const RPC_DESC_T t_rpc_decl_CI_MMI_MENU_ANS_T;
static const RPC_DESC_T t_rpc_decl_CI_SLOT_INFO_T;
static const RPC_DESC_T t_rpc_decl_CI_HC_TUNE_T;
static const RPC_DESC_T t_rpc_decl_CI_HC_REPLACE_T;
static const RPC_DESC_T t_rpc_decl_CI_HC_CLR_REPLACE_T;
static const RPC_DESC_T t_rpc_decl_CI_CA_SYSTEM_ID_MATCH_T;
static const RPC_DESC_T t_rpc_decl_CI_CA_SYSTEM_ID_STATUS_T;
static const RPC_DESC_T t_rpc_decl_CI_CONFIG_T;



static const RPC_DESC_T t_rpc_decl_STYLE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (STYLE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_STYLE_STR_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (STYLE_STR_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_STREAM_COMP_ID_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (STREAM_COMP_ID_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (STREAM_COMP_ID_T, pv_stream_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_THREAD_DESCR_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (THREAD_DESCR_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_TS_DESCR_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TS_DESCR_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_ARG_DESC_T_u =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((ARG_DESC_T*) NULL)->u),
    .ui4_num_entries = 15,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.pv_arg"
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.pc_arg"
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.pi1_arg"
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.pi2_arg"
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.pi4_arg"
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.pi8_arg"
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.pui1_arg"
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.pui2_arg"
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.pui4_arg"
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.pui8_arg"
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.pz_arg"
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.pb_arg"
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.ps_str"
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.pv_func"
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.pv_desc"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_ARG_DESC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ARG_DESC_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_ARG_DESC_T_u,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (ARG_DESC_T, u)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_RPC_DESC_ENTRY_T_u =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((RPC_DESC_ENTRY_T*) NULL)->u),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC | ARG_DIR_INP,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.ps_field_name"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_RPC_DESC_ENTRY_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (RPC_DESC_ENTRY_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC | ARG_DIR_INP,
            .pt_desc         = &t_rpc_decl_RPC_DESC_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (RPC_DESC_ENTRY_T, pt_desc)
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_RPC_DESC_ENTRY_T_u,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (RPC_DESC_ENTRY_T, u)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_RPC_DESC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (RPC_DESC_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_RPC_DESC_ENTRY_T,
            .ui4_num_entries = 0,

            {
                .z_offs = offsetof (RPC_DESC_T, at_desc_entries)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_OS_FNCT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (OS_FNCT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CM_COMMAND_T_u =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((CM_COMMAND_T*) NULL)->u),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC | ARG_DIR_INP,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.ps_name"
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.pv_ptr"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_CM_COMMAND_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CM_COMMAND_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_CM_COMMAND_T_u,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (CM_COMMAND_T, u)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_CM_CTRL_RM_SRC_COMP_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CM_CTRL_RM_SRC_COMP_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (CM_CTRL_RM_SRC_COMP_T, pv_stream_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_TUNER_INTERLEAVING_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TUNER_INTERLEAVING_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_TUNER_DVBT2_PLP_LIST_INFO =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TUNER_DVBT2_PLP_LIST_INFO),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_TUNER_LNB_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TUNER_LNB_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_TUNER_DISEQC_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TUNER_DISEQC_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_TUNER_CAB_ANA_TUNE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TUNER_CAB_ANA_TUNE_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_TUNER_CAB_DIG_TUNE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TUNER_CAB_DIG_TUNE_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_TUNER_CAB_DIG_OOB_TX_TUNE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TUNER_CAB_DIG_OOB_TX_TUNE_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_TUNER_CAB_DIG_OOB_RX_TUNE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TUNER_CAB_DIG_OOB_RX_TUNE_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_TUNER_SAT_ANA_TUNE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TUNER_SAT_ANA_TUNE_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_TUNER_SAT_DIG_TUNE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TUNER_SAT_DIG_TUNE_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_TUNER_TER_ANA_TUNE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TUNER_TER_ANA_TUNE_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_TUNER_TER_DIG_TUNE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TUNER_TER_DIG_TUNE_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_DB_FIELD_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DB_FIELD_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_DB_SEARCH_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DB_SEARCH_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (DB_SEARCH_T, pv_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MEDIA_SUBTYPE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MEDIA_SUBTYPE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MEDIA_FORMAT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MEDIA_FORMAT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_PLAY_MODE_PL_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_PLAY_MODE_PL_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_DRM_OUTPUT_SIGNAL_PROTECTION_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DRM_OUTPUT_SIGNAL_PROTECTION_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_DIVX_DRM_INIT_CONFIG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DIVX_DRM_INIT_CONFIG_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_DIVX_DRM_REGISTRATION_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DIVX_DRM_REGISTRATION_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_DIVX_DRM_DEACTIVATION_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DIVX_DRM_DEACTIVATION_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_DIVX_DRM_UI_HELP_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DIVX_DRM_UI_HELP_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_DIVX_DRM_MEMORY_RW_FUNC_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DIVX_DRM_MEMORY_RW_FUNC_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_DIVX_DRM_BASIC_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DIVX_DRM_BASIC_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_DIVX_DRM_UNION_GENERIC_INFO_T =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (DIVX_DRM_UNION_GENERIC_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_MPEG_2_STREAM_ID =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_MPEG_2_STREAM_ID),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_MP3_AUDIO_STM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_MP3_AUDIO_STM_ATRBT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_FLV_AUDIO_STM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_FLV_AUDIO_STM_ATRBT_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_FLV_AUDIO_STM_ATRBT_T, pui1_codec_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MINFO_FLV_VIDEO_STM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_FLV_VIDEO_STM_ATRBT_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_FLV_VIDEO_STM_ATRBT_T, pui1_codec_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MINFO_CMPB_VIDEO_STM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_CMPB_VIDEO_STM_ATRBT_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_CMPB_VIDEO_STM_ATRBT_T, pu1_buff)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MINFO_CMPB_AUDIO_STM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_CMPB_AUDIO_STM_ATRBT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_CMPB_SUBTL_STM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_CMPB_SUBTL_STM_ATRBT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_RM_VIDEO_STM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_RM_VIDEO_STM_ATRBT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_RM_AUDIO_STM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_RM_AUDIO_STM_ATRBT_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_RM_AUDIO_STM_ATRBT_T, pv_intlv_ptrn)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_RM_AUDIO_STM_ATRBT_T, pv_codec_opaque_data)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MINFO_AIF_AUDIO_STM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_AIF_AUDIO_STM_ATRBT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_AVI_AUDIO_STM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_AVI_AUDIO_STM_ATRBT_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_AVI_AUDIO_STM_ATRBT_T, pui1_codec_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MINFO_AVI_VIDEO_CODEC_XXDB_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_AVI_VIDEO_CODEC_XXDB_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_AVI_VIDEO_CODEC_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_AVI_VIDEO_CODEC_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_AVI_VIDEO_STM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_AVI_VIDEO_STM_ATRBT_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_AVI_VIDEO_STM_ATRBT_T, pui1_codec_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MINFO_AVI_DIVX_SUBTITLE_STM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_AVI_DIVX_SUBTITLE_STM_ATRBT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_ASF_AUDIO_STM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_ASF_AUDIO_STM_ATRBT_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_ASF_AUDIO_STM_ATRBT_T, pui1_codec_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MINFO_ASF_VIDEO_STM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_ASF_VIDEO_STM_ATRBT_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_ASF_VIDEO_STM_ATRBT_T, pui1_codec_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MINFO_MP4_AUDIO_STM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_MP4_AUDIO_STM_ATRBT_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_MP4_AUDIO_STM_ATRBT_T, pui1_codec_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MINFO_MP4_VIDEO_STM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_MP4_VIDEO_STM_ATRBT_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_MP4_VIDEO_STM_ATRBT_T, pui1_codec_info)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_MP4_VIDEO_STM_ATRBT_T, pui1_mvcC_codec_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MINFO_MP4_SUBTITLE_STM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_MP4_SUBTITLE_STM_ATRBT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_MKV_TM_SCALE_FLOAT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_MKV_TM_SCALE_FLOAT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_MKV_CONTENT_ENCODING_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_MKV_CONTENT_ENCODING_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_MKV_CONTENT_ENCODING_T, pui1_comp_settings_data)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_MKV_CONTENT_ENCODING_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_MKV_CONTENT_ENCODING_T, pt_next)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MINFO_MKV_AUDIO_STM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_MKV_AUDIO_STM_ATRBT_T),
    .ui4_num_entries = 3,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_MKV_AUDIO_STM_ATRBT_T, pui1_codec_info)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_MKV_CONTENT_ENCODING_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_MKV_AUDIO_STM_ATRBT_T, pt_content_encoding)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_MKV_AUDIO_STM_ATRBT_T, pui1_codec_private)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MINFO_MKV_VIDEO_STM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_MKV_VIDEO_STM_ATRBT_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_MKV_VIDEO_STM_ATRBT_T, pui1_codec_info)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_MKV_CONTENT_ENCODING_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_MKV_VIDEO_STM_ATRBT_T, pt_content_encoding)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MINFO_MKV_SUBTITLE_STM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_MKV_SUBTITLE_STM_ATRBT_T),
    .ui4_num_entries = 4,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_MKV_SUBTITLE_STM_ATRBT_T, pui1_codec_info)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_MKV_SUBTITLE_STM_ATRBT_T, pui1_content_encodings)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_MKV_CONTENT_ENCODING_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_MKV_SUBTITLE_STM_ATRBT_T, pt_content_encoding)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_MKV_CONTENT_ENCODING_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_MKV_SUBTITLE_STM_ATRBT_T, pt_curr_content_encoding)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_FONT_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FONT_INFO_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (FONT_INFO_T, pv_file_data)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_FONT_INFO_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (FONT_INFO_T, pt_next_font)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MINFO_PS_AUDIO_STM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_PS_AUDIO_STM_ATRBT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_PS_VIDEO_STM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_PS_VIDEO_STM_ATRBT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_PS_SUBTITLE_STM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_PS_SUBTITLE_STM_ATRBT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_EXT_SUBTITLE_STM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_EXT_SUBTITLE_STM_ATRBT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_ES_VIDEO_STM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_ES_VIDEO_STM_ATRBT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_ES_AUDIO_STM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_ES_AUDIO_STM_ATRBT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_OGG_STREAM_VORBIS_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (OGG_STREAM_VORBIS_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_OGG_STREAM_AUDIO_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (OGG_STREAM_AUDIO_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_OGG_VIDEO_STM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_OGG_VIDEO_STM_ATRBT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_OGG_TEXT_STM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_OGG_TEXT_STM_ATRBT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_OGG_AUDIO_VORBIS_SPEC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_OGG_AUDIO_VORBIS_SPEC_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_OGG_AUDIO_STM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_OGG_AUDIO_STM_ATRBT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_TS_RD3D_REG_DESC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_TS_RD3D_REG_DESC_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_MVC_EXTENSION_DESC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_MVC_EXTENSION_DESC_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_TS_VIDEO_STM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_TS_VIDEO_STM_ATRBT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_TS_AUDIO_STM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_TS_AUDIO_STM_ATRBT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_TS_SBTL_STM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_TS_SBTL_STM_ATRBT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_STM_ID_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_STM_ID_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_TYPE_STM_ATRBT_T_u =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((MINFO_TYPE_STM_ATRBT_T*) NULL)->u),
    .ui4_num_entries = 13,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_AVI_AUDIO_STM_ATRBT_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_avi_audio_stm_attr"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_AVI_VIDEO_STM_ATRBT_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_avi_video_stm_attr"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_ASF_AUDIO_STM_ATRBT_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_asf_audio_stm_attr"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_ASF_VIDEO_STM_ATRBT_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_asf_video_stm_attr"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_MP4_AUDIO_STM_ATRBT_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_mp4i_audio_stm_attr"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_MP4_VIDEO_STM_ATRBT_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_mp4i_video_stm_attr"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_MKV_AUDIO_STM_ATRBT_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_mkv_audio_stm_attr"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_MKV_VIDEO_STM_ATRBT_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_mkv_video_stm_attr"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_MKV_SUBTITLE_STM_ATRBT_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_mkv_sp_stm_attr"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_RM_AUDIO_STM_ATRBT_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_rm_audio_stm_attr"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_FLV_AUDIO_STM_ATRBT_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_flv_audio_stm_attr"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_FLV_VIDEO_STM_ATRBT_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_flv_video_stm_attr"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_CMPB_VIDEO_STM_ATRBT_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_cmpb_video_stm_attr"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MINFO_TYPE_STM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_TYPE_STM_ATRBT_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_TYPE_STM_ATRBT_T_u,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_TYPE_STM_ATRBT_T, u)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MINFO_INFO_PIC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_INFO_PIC_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_ID3_RAW_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_ID3_RAW_DATA_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_ID3_RAW_DATA_T, pui1_raw_data)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_ID3_RAW_DATA_T, pz_data_size)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MINFO_TYPE_ID3_TAG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_TYPE_ID3_TAG_T),
    .ui4_num_entries = 8,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_ID3_RAW_DATA_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_TYPE_ID3_TAG_T, t_id3_raw_data)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_TYPE_ID3_TAG_T, pui1_img_data_buf)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_TYPE_ID3_TAG_T, pwc_str_id3_year)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_TYPE_ID3_TAG_T, pwc_str_id3_title)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_TYPE_ID3_TAG_T, pwc_str_id3_artist)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_TYPE_ID3_TAG_T, pwc_str_id3_album)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_TYPE_ID3_TAG_T, pwc_str_id3_track)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_TYPE_ID3_TAG_T, pwc_str_id3_genre)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MINFO_TITLE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_TITLE_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_PLAYLIST_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_PLAYLIST_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_CHAP_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_CHAP_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_DISP_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_DISP_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_TYPE_GENERIC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_TYPE_GENERIC_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_TM_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_TM_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_TYPE_FRAME_RATE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_TYPE_FRAME_RATE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_TYPE_VIDEO_ES_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_TYPE_VIDEO_ES_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_TYPE_AUDIO_ES_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_TYPE_AUDIO_ES_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_TYPE_META_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_TYPE_META_DATA_T),
    .ui4_num_entries = 9,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_TYPE_META_DATA_T, pwc_str_title)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_TYPE_META_DATA_T, pwc_str_director)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_TYPE_META_DATA_T, pwc_str_copyright)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_TYPE_META_DATA_T, pwc_str_genre)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_TYPE_META_DATA_T, pwc_str_rating)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_TYPE_META_DATA_T, pwc_str_artist)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_TYPE_META_DATA_T, pwc_str_album)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_TYPE_META_DATA_T, pwc_str_track)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_TYPE_META_DATA_T, pwc_str_year)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MP4_SINGLE_FILE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MP4_SINGLE_FILE_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MP4_MULTI_FILE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MP4_MULTI_FILE_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MP4_MINFO_EXTRA_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MP4_MINFO_EXTRA_INFO_T),
    .ui4_num_entries = 3,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MP4_MINFO_EXTRA_INFO_T, pt_mp4_cmov_info)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MP4_MINFO_EXTRA_INFO_T, pt_piff_inet_info)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MP4_MINFO_EXTRA_INFO_T, pt_mp4_multi_file_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MINFO_TYPE_IDX_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_TYPE_IDX_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MP4_MINFO_EXTRA_INFO_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_TYPE_IDX_INFO_T, t_mp4_extra_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MINFO_MP4_TRAK_ENCRYTION_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_MP4_TRAK_ENCRYTION_INFO_T),
    .ui4_num_entries = 3,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_MP4_TRAK_ENCRYTION_INFO_T, pui1_ct_id)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_MP4_TRAK_ENCRYTION_INFO_T, pui1_satr_ctm)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_MP4_TRAK_ENCRYTION_INFO_T, pui1_satr_hmac_val)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MINFO_MP4_ENCRYTION_INFO_T_u =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((MINFO_MP4_ENCRYTION_INFO_T*) NULL)->u),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_MP4_TRAK_ENCRYTION_INFO_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_video_encrytion_info"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_MP4_TRAK_ENCRYTION_INFO_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_audio_encrytion_info"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MINFO_MP4_ENCRYTION_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_MP4_ENCRYTION_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_MP4_ENCRYTION_INFO_T_u,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_MP4_ENCRYTION_INFO_T, u)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MINFO_TYPE_ENCRYPTION_INFO_T_u =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((MINFO_TYPE_ENCRYPTION_INFO_T*) NULL)->u),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_MP4_ENCRYTION_INFO_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_minfo_mp4_encry_info"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MINFO_TYPE_ENCRYPTION_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_TYPE_ENCRYPTION_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_TYPE_ENCRYPTION_INFO_T_u,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_TYPE_ENCRYPTION_INFO_T, u)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MINFO_DIVX_DRM_OBJ_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_DIVX_DRM_OBJ_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_DIVX_DRM_OBJ_T, pui1_strd_chunk)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MINFO_TYPE_DRM_INFO_T_u =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((MINFO_TYPE_DRM_INFO_T*) NULL)->u),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_DIVX_DRM_OBJ_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_minfo_divx_drm_obj"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MINFO_TYPE_DRM_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_TYPE_DRM_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_TYPE_DRM_INFO_T_u,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_TYPE_DRM_INFO_T, u)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MINFO_TYPE_FONT_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_TYPE_FONT_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_FONT_INFO_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_TYPE_FONT_INFO_T, pt_font_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MINFO_TYPE_DIVX_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_TYPE_DIVX_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MINFO_INFO_T_u =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((MINFO_INFO_T*) NULL)->u),
    .ui4_num_entries = 7,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_TYPE_META_DATA_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_meta_data"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_TYPE_STM_ATRBT_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_stm_attribute"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_TYPE_ID3_TAG_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_id3_data"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_TYPE_IDX_INFO_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_idx_info"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_TYPE_DRM_INFO_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_drm_info"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_TYPE_ENCRYPTION_INFO_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_encrytion_info"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_TYPE_FONT_INFO_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_font_info"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MINFO_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_INFO_T_u,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_INFO_T, u)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MINFO_TYPE_RAW_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MINFO_TYPE_RAW_DATA_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MINFO_TYPE_RAW_DATA_T, pv_get_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SCDB_MPEG_COMMON_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_MPEG_COMMON_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_COMMON_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_COMMON_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_AUDIO_MPEG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_AUDIO_MPEG_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_AVC_VID_DESC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (AVC_VID_DESC_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_RD3D_REG_DESC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (RD3D_REG_DESC_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MVC_EXTENSION_DESC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MVC_EXTENSION_DESC_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_VIDEO_MPEG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_VIDEO_MPEG_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_PRIVATE_MPEG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_PRIVATE_MPEG_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_DSMCC_MPEG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_DSMCC_MPEG_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CC_ATSC_LINE_21_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CC_ATSC_LINE_21_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CC_ATSC_DTV_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CC_ATSC_DTV_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_CLOSED_CAPTION_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_CLOSED_CAPTION_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_ISDB_CAPTION_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_ISDB_CAPTION_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_ISDB_TEXT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_ISDB_TEXT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_SUBTITLE_DVB_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_SUBTITLE_DVB_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_TELETEXT_DVB_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_TELETEXT_DVB_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_VBI_TELETEXT_DVB_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_VBI_TELETEXT_DVB_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_VBI_DATA_DVB_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_VBI_DATA_DVB_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_AUDIO_ANALOG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_AUDIO_ANALOG_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_VIDEO_ANALOG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_VIDEO_ANALOG_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_AUDIO_DIGITAL_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_AUDIO_DIGITAL_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_AUDIO_AVI_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_AUDIO_AVI_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_VIDEO_AVI_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_VIDEO_AVI_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_SUBTITLE_DIVX_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_SUBTITLE_DIVX_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_AUDIO_MP4_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_AUDIO_MP4_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_SUBTITLE_MP4_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_SUBTITLE_MP4_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_VIDEO_MP4_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_VIDEO_MP4_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_AUDIO_ASF_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_AUDIO_ASF_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_VIDEO_ASF_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_VIDEO_ASF_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_AUDIO_PS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_AUDIO_PS_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_VIDEO_PS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_VIDEO_PS_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_VIDEO_MKV_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_VIDEO_MKV_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_AUDIO_MKV_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_AUDIO_MKV_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_VIDEO_RM_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_VIDEO_RM_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_AUDIO_RM_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_AUDIO_RM_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SCDB_AUDIO_RM_T, pv_intlv_ptrn)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SCDB_AUDIO_RM_T, pv_codec_opaque_data)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SCDB_AUDIO_FLV_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_AUDIO_FLV_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_VIDEO_FLV_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_VIDEO_FLV_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_SUBTITLE_MKV_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_SUBTITLE_MKV_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_AUDIO_MP3_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_AUDIO_MP3_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_VIDEO_ES_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_VIDEO_ES_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_AUDIO_ES_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_AUDIO_ES_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_SUBTITLE_PS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_SUBTITLE_PS_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_VIDEO_OGG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_VIDEO_OGG_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_AUDIO_VORBIS_OGG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_AUDIO_VORBIS_OGG_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_AUDIO_NORMAL_OGG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_AUDIO_NORMAL_OGG_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_AUDIO_OGG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_AUDIO_OGG_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_EXT_SUBTITLE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_EXT_SUBTITLE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCDB_REC_T_u =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((SCDB_REC_T*) NULL)->u),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_SCDB_AUDIO_RM_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_audio_rm"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SCDB_REC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCDB_REC_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_SCDB_REC_T_u,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SCDB_REC_T, u)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_DISEQC_10_SET_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DISEQC_10_SET_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BRDCST_CTRL_DISEQC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BRDCST_CTRL_DISEQC_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BRDCST_CTRL_TUNE_ATTRIBUTES_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BRDCST_CTRL_TUNE_ATTRIBUTES_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BRDCST_CTRL_AUDIO_STREAM_FMT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BRDCST_CTRL_AUDIO_STREAM_FMT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BRDCST_CTRL_AUDIO_ENC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BRDCST_CTRL_AUDIO_ENC_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_BRDCST_TIME_OUT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (BRDCST_TIME_OUT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SM_COMMAND_T_u =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((SM_COMMAND_T*) NULL)->u),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.ps_text"
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.pv_data"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SM_COMMAND_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SM_COMMAND_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_SM_COMMAND_T_u,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SM_COMMAND_T, u)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SM_OBSVR_FILTER_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SM_OBSVR_FILTER_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SM_OBSVR_FILTER_T, pui4_event_fltr)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SM_VSH_VBI_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SM_VSH_VBI_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SM_VSH_VBI_INFO_T, pv_vbi_data)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SM_VSH_VBI_NFY_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SM_VSH_VBI_NFY_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SM_VSH_VBI_NFY_T, pv_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SM_VSH_START_RENDER_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SM_VSH_START_RENDER_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SM_VSH_VBI_REF_INFO_T_u =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((SM_VSH_VBI_REF_INFO_T*) NULL)->u),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.pv_reserve"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SM_VSH_VBI_REF_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SM_VSH_VBI_REF_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_SM_VSH_VBI_REF_INFO_T_u,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SM_VSH_VBI_REF_INFO_T, u)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_VSH_CAPTURE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VSH_CAPTURE_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (VSH_CAPTURE_INFO_T, pui1_frame_buff)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_VSH_REGION_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VSH_REGION_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VSH_REGION_CAPABILITY_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VSH_REGION_CAPABILITY_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VSH_ENCHANCE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VSH_ENCHANCE_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VSH_SET_PLANE_ORDER_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VSH_SET_PLANE_ORDER_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VSH_GET_PLANE_ORDER_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VSH_GET_PLANE_ORDER_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VSH_QV_ROTATE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VSH_QV_ROTATE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VSH_QV_CTRL_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VSH_QV_CTRL_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VSH_HDMI_AVI_INFOFRAME_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VSH_HDMI_AVI_INFOFRAME_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VSH_SRC_RESOLUTION_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VSH_SRC_RESOLUTION_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VSH_VID_MIN_MAX_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VSH_VID_MIN_MAX_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VSH_CNT_ADV_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VSH_CNT_ADV_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VSH_VGA_TIMING_OPTION_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VSH_VGA_TIMING_OPTION_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VSH_AMBIG_VGA_TIMING_OPTION_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VSH_AMBIG_VGA_TIMING_OPTION_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VSH_AMBIG_VGA_TIMING_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VSH_AMBIG_VGA_TIMING_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VSH_SEQUENCE_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VSH_SEQUENCE_DATA_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VSH_SEQUENCE_RAW_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VSH_SEQUENCE_RAW_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (VSH_SEQUENCE_RAW_T, pc_buff)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_VSH_SEQUENCE_INFO_T_u =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((VSH_SEQUENCE_INFO_T*) NULL)->u),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_VSH_SEQUENCE_RAW_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_seq_raw"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_VSH_SEQUENCE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VSH_SEQUENCE_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_VSH_SEQUENCE_INFO_T_u,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (VSH_SEQUENCE_INFO_T, u)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_VSH_TIME_CODE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VSH_TIME_CODE_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VSH_PICTURE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VSH_PICTURE_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VSH_THUMBNAIL_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VSH_THUMBNAIL_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (VSH_THUMBNAIL_INFO_T, pc_canvas_buffer)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_THUMBNAIL_GET_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (THUMBNAIL_GET_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (THUMBNAIL_GET_INFO_T, u1CanvasBuffer)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_ASH_VOLUME_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ASH_VOLUME_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_ASH_AUDIO_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ASH_AUDIO_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_ASH_AUD_SYS_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ASH_AUD_SYS_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_ASH_AUDIO_WMA_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ASH_AUDIO_WMA_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_ASH_AUDIO_AAC_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ASH_AUDIO_AAC_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_ASH_AUDIO_PCM_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ASH_AUDIO_PCM_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_ASH_AUDIO_COOK_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ASH_AUDIO_COOK_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (ASH_AUDIO_COOK_INFO_T, pv_codec_opaque_data)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_ASH_ADUIO_VORBIS_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ASH_ADUIO_VORBIS_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_ASH_ADUIO_AC3_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ASH_ADUIO_AC3_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_ASH_AUDIO_CODEC_SPEC_INFO_T_u =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((ASH_AUDIO_CODEC_SPEC_INFO_T*) NULL)->u),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_ASH_AUDIO_COOK_INFO_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_cook_info"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_ASH_AUDIO_CODEC_SPEC_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ASH_AUDIO_CODEC_SPEC_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_ASH_AUDIO_CODEC_SPEC_INFO_T_u,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (ASH_AUDIO_CODEC_SPEC_INFO_T, u)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SCC_BG_COLOR_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_BG_COLOR_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCC_VID_MODE_EX_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_VID_MODE_EX_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCC_AUD_DEC_LINEOUT_VOL_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_AUD_DEC_LINEOUT_VOL_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCC_VID_OVER_SCAN_CLIPPER_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_VID_OVER_SCAN_CLIPPER_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCC_VID_VDRIPS_BUFF_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_VID_VDRIPS_BUFF_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SCC_VID_VDRIPS_BUFF_T, puc_vdrips_buff)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SCC_VID_IFRAME_BUFF_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_VID_IFRAME_BUFF_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SCC_VID_IFRAME_BUFF_T, puc_iframe_buff)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SCC_VID_REGION_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_VID_REGION_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCC_VID_DEBLOCKING_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_VID_DEBLOCKING_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCC_VID_LB_DETECT_NFY_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_VID_LB_DETECT_NFY_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SCC_VID_LB_DETECT_NFY_INFO_T, pv_nfy_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SCC_VID_LB_DETECT_CONFIG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_VID_LB_DETECT_CONFIG_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_SCC_VID_LB_DETECT_NFY_INFO_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SCC_VID_LB_DETECT_CONFIG_T, t_lb_detect_nfy_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SCC_VID_MJC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_VID_MJC_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCC_VID_SUPER_BLANK_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_VID_SUPER_BLANK_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCC_VID_SUPER_FREEZE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_VID_SUPER_FREEZE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCC_ANGULAR_SCE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_ANGULAR_SCE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCC_VID_DEC_CAPABILITY_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_VID_DEC_CAPABILITY_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCC_VID_FREQ_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_VID_FREQ_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCC_VID_SRC_DET_STATUS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_VID_SRC_DET_STATUS_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCC_VID_SCR_MODE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_VID_SCR_MODE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCC_DISP_RESOLUTION_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_DISP_RESOLUTION_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCC_DISP_SCREEN_RESOLUTION_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_DISP_SCREEN_RESOLUTION_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCC_DISP_COLOR_GAIN_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_DISP_COLOR_GAIN_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCC_DISP_RANGE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_DISP_RANGE_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCC_AUD_LOUD_SPEAKER_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_AUD_LOUD_SPEAKER_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCC_AUD_SE_TRIM_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_AUD_SE_TRIM_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCC_AUD_VOLUME_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_AUD_VOLUME_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCC_AUD_CHNL_DELAY_IND_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_AUD_CHNL_DELAY_IND_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCC_AUD_CHNL_DELAY_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_AUD_CHNL_DELAY_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCC_AUD_SE_SURROUND_VALUE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_AUD_SE_SURROUND_VALUE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCC_AUD_SE_SURROUND_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_AUD_SE_SURROUND_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCC_AUD_DEC_SET_TYPE_PEQ_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_AUD_DEC_SET_TYPE_PEQ_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCC_AUD_SE_QA_USER_LVL_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_AUD_SE_QA_USER_LVL_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCC_AUD_UPLOAD_DATA_NFT_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_AUD_UPLOAD_DATA_NFT_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SCC_AUD_UPLOAD_DATA_NFT_INFO_T, pv_nfy_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SCC_AUD_HDPHONE_DETECT_NFT_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_AUD_HDPHONE_DETECT_NFT_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SCC_AUD_HDPHONE_DETECT_NFT_INFO_T, pv_nfy_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SCC_AUD_MUTE_EX_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_AUD_MUTE_EX_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCC_AU_SE_QA_USER_BAND_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_AU_SE_QA_USER_BAND_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCC_AUD_CLIP_BUFF_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_AUD_CLIP_BUFF_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SCC_AUD_CLIP_BUFF_T, pui1_clip_buff)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SCC_AUD_CLIP_T_u =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((SCC_AUD_CLIP_T*) NULL)->u),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_SCC_AUD_CLIP_BUFF_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_clip_buff"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SCC_AUD_CLIP_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_AUD_CLIP_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_SCC_AUD_CLIP_T_u,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SCC_AUD_CLIP_T, u)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SCC_AUD_CAPABILITY_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_AUD_CAPABILITY_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCC_AUD_UPLOAD_DATA_BUFFER_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_AUD_UPLOAD_DATA_BUFFER_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCC_AUD_UPLOAD_PCM_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_AUD_UPLOAD_PCM_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCC_AUD_KARAOKE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCC_AUD_KARAOKE_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVL_SIMPLE_DATE_TIME_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVL_SIMPLE_DATE_TIME_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVL_ANALOG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVL_ANALOG_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVL_ATSC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVL_ATSC_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVL_DVB_TS_SVC_ID_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVL_DVB_TS_SVC_ID_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVL_DVB_SBTL_PREF_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVL_DVB_SBTL_PREF_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVL_DVB_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVL_DVB_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVL_ISDB_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVL_ISDB_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVL_DTMB_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVL_DTMB_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVL_FMRDO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVL_FMRDO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVL_REC_HEADER_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVL_REC_HEADER_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVL_REC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVL_REC_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVL_CLIENT_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVL_CLIENT_DATA_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SVL_CLIENT_DATA_T, pui1_data_buf)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SVL_MULTI_LANG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVL_MULTI_LANG_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SVL_MULTI_LANG_T, pui1_data_buf)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SVL_MEM_DUMP_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVL_MEM_DUMP_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SVL_MEM_DUMP_T, pv_data)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_FREQ_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FREQ_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_ALTERNATE_TUNE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ALTERNATE_TUNE_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_TSL_REC_REFERENCE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TSL_REC_REFERENCE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_ISDB_TER_DIG_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ISDB_TER_DIG_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_TSL_REC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TSL_REC_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_TSL_MEM_DUMP_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TSL_MEM_DUMP_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (TSL_MEM_DUMP_T, pv_data)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_EVCTX_KEY_TYPE_MASK_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (EVCTX_KEY_TYPE_MASK_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_EVCTX_COMMAND_T_u =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((EVCTX_COMMAND_T*) NULL)->u),
    .ui4_num_entries = 3,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC | ARG_DIR_INP,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.ps_name"
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_SVL_REC_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.pt_svl_rec"
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_TSL_REC_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.pt_tsl_rec"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_EVCTX_COMMAND_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (EVCTX_COMMAND_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_EVCTX_COMMAND_T_u,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (EVCTX_COMMAND_T, u)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_EVCTX_OFFSET_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (EVCTX_OFFSET_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_EVCTX_BRIEF_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (EVCTX_BRIEF_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (EVCTX_BRIEF_INFO_T, ps_event_title)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_EVCTX_EVT_CAT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (EVCTX_EVT_CAT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_EVCTX_EVT_CAT_LIST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (EVCTX_EVT_CAT_LIST_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_EVCTX_EVT_CAT_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (EVCTX_EVT_CAT_LIST_T, pt_types)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_EVCTX_RATING_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (EVCTX_RATING_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_EVCTX_RATING_LIST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (EVCTX_RATING_LIST_T),
    .ui4_num_entries = 3,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (EVCTX_RATING_LIST_T, ps_rating_descrption)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_EVCTX_RATING_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (EVCTX_RATING_LIST_T, pt_ratings)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_EVCTX_RATING_LIST_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (EVCTX_RATING_LIST_T, pt_next)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_EVCTX_CA_SYSTEM_ID_LIST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (EVCTX_CA_SYSTEM_ID_LIST_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (EVCTX_CA_SYSTEM_ID_LIST_T, pui2_ca_ids)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_EVCTX_EVENT_LINKAGE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (EVCTX_EVENT_LINKAGE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_EVCTX_EVENT_LINKAGE_LIST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (EVCTX_EVENT_LINKAGE_LIST_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_EVCTX_EVENT_LINKAGE_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (EVCTX_EVENT_LINKAGE_LIST_T, pt_event_linkage)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_EVCTX_IDX_LANG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (EVCTX_IDX_LANG_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (EVCTX_IDX_LANG_T, pt_lang)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SM_CC_AUTO_SEL_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SM_CC_AUTO_SEL_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SM_CC_FILTER_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SM_CC_FILTER_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SM_CCH_NTSC_CC_VBI_PSR_CB_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SM_CCH_NTSC_CC_VBI_PSR_CB_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SM_CCH_NTSC_CC_VBI_PSR_CB_INFO_T, pv_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SM_CCH_DTVCC_COLOR_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SM_CCH_DTVCC_COLOR_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SM_CCH_DTVCC_ATTRIBS_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SM_CCH_DTVCC_ATTRIBS_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SM_CCH_EXISTED_CC_IND_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SM_CCH_EXISTED_CC_IND_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SM_CCH_CONFIG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SM_CCH_CONFIG_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SM_SBTLH_STREAM_INFO =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SM_SBTLH_STREAM_INFO),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SM_SBTLH_CONFIG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SM_SBTLH_CONFIG_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SM_SBTLH_RENDER_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SM_SBTLH_RENDER_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SM_SBTLH_RENDER_INFO_T, pui1_rgn)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_CP_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CP_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CP_INFO_LIST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CP_INFO_LIST_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CA_ALG_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CA_ALG_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CA_KEY_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CA_KEY_INFO_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (CA_KEY_INFO_T, auc_key)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (CA_KEY_INFO_T, auc_iv)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_CA_SET_COMMAND_INFO_T_u_set_cmd_info =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((CA_SET_COMMAND_INFO_T*) NULL)->u_set_cmd_info),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_CA_KEY_INFO_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u_set_cmd_info.t_key_info"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_CA_SET_COMMAND_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CA_SET_COMMAND_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_CA_SET_COMMAND_INFO_T_u_set_cmd_info,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (CA_SET_COMMAND_INFO_T, u_set_cmd_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_ANALOG_BRDCST_TIME_OUT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ANALOG_BRDCST_TIME_OUT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_ANALOG_BRDCST_CTRL_SCART_INP_CAP_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ANALOG_BRDCST_CTRL_SCART_INP_CAP_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_ANALOG_BRDCST_CTRL_SCART_BYPASS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ANALOG_BRDCST_CTRL_SCART_BYPASS_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_ANALOG_BRDCST_CTRL_TV_AUD_SYS_MASK_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ANALOG_BRDCST_CTRL_TV_AUD_SYS_MASK_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_ANALOG_BRDCST_CTRL_TUNE_ATTRIBUTES_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ANALOG_BRDCST_CTRL_TUNE_ATTRIBUTES_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SILENT_SELECT_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SILENT_SELECT_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CHANNEL_DECODE_DESC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CHANNEL_DECODE_DESC_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SRC_SVL_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SRC_SVL_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SRC_FREQ_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SRC_FREQ_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_AVC_HANDLER_TIME_OUT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (AVC_HANDLER_TIME_OUT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_AVC_HANDLER_CTRL_SCART_INP_CAP_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (AVC_HANDLER_CTRL_SCART_INP_CAP_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SRC_AVC_HINT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SRC_AVC_HINT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SRC_AVC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SRC_AVC_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SRC_AVC_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SRC_AVC_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FE_INIT_DATA =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FE_INIT_DATA),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (FE_INIT_DATA, pui1_fontdata)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_FE_INIT_FILE =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FE_INIT_FILE),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (FE_INIT_FILE, ps_file)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_GL_SCREEN_STATUS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (GL_SCREEN_STATUS_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_GL_PLANE_CAPS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (GL_PLANE_CAPS_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_GL_PLANE_CFG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (GL_PLANE_CFG_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_GL_PLANE_STATUS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (GL_PLANE_STATUS_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_GL_POINT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (GL_POINT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_GL_SIZE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (GL_SIZE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_GL_RECT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (GL_RECT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_GL_MATRIX2D_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (GL_MATRIX2D_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_GL_COLOR_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (GL_COLOR_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_GL_DISPLAY_OPT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (GL_DISPLAY_OPT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_GL_SURFACE_DESC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (GL_SURFACE_DESC_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_GL_SURF_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (GL_SURF_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_GL_LOCK_INFO_T_at_subplane =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (((GL_LOCK_INFO_T*) NULL)->at_subplane [0]),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (GL_LOCK_INFO_T, at_subplane [0].pv_bits)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_GL_LOCK_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (GL_LOCK_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_GL_LOCK_INFO_T_at_subplane,
            .ui4_num_entries = 4,

            {
                .z_offs = offsetof (GL_LOCK_INFO_T, at_subplane)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_GL_ARC_DESC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (GL_ARC_DESC_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_GL_GPU_CAPS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (GL_GPU_CAPS_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_GL_BLT_EX_T =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (GL_BLT_EX_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_GL_DRV_UPDATE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (GL_DRV_UPDATE_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_GL_DRCS_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (GL_DRCS_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_GL_TTX_COLOR_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (GL_TTX_COLOR_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_GL_ISDB_CHAR_COLOR_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (GL_ISDB_CHAR_COLOR_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_GL_ISDB_GRID_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (GL_ISDB_GRID_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_IMG_GIF_FRM_RECT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (IMG_GIF_FRM_RECT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VID_PLA_NFY_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VID_PLA_NFY_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (VID_PLA_NFY_INFO_T, pv_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_VID_PLA_DISP_DIMENSION_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VID_PLA_DISP_DIMENSION_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VID_PLA_INJECT_VDP_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VID_PLA_INJECT_VDP_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (VID_PLA_INJECT_VDP_T, pv_src)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_VID_PLA_BG_COLOR_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VID_PLA_BG_COLOR_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VID_PLA_VID_REGION_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VID_PLA_VID_REGION_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VID_PLA_VID_REGION_CAPABILITY_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VID_PLA_VID_REGION_CAPABILITY_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VID_PLA_GET_ASPECT_RATIO_TYPE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VID_PLA_GET_ASPECT_RATIO_TYPE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VID_PLA_ENHANCE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VID_PLA_ENHANCE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VID_PLA_QV_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VID_PLA_QV_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VID_PLA_MIN_MAX_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VID_PLA_MIN_MAX_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VID_PLA_COLOR_GAIN_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VID_PLA_COLOR_GAIN_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VID_PLA_OVER_SCAN_CLIPPER_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VID_PLA_OVER_SCAN_CLIPPER_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VID_PLA_MAX_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VID_PLA_MAX_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VID_PLA_LB_DETECT_NFY_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VID_PLA_LB_DETECT_NFY_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (VID_PLA_LB_DETECT_NFY_INFO_T, pv_nfy_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_VID_PLA_LB_DETECT_CONFIG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VID_PLA_LB_DETECT_CONFIG_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_VID_PLA_LB_DETECT_NFY_INFO_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (VID_PLA_LB_DETECT_CONFIG_T, t_lb_detect_nfy_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_VID_PLA_TIME_CODE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VID_PLA_TIME_CODE_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VID_PLA_PICTURE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VID_PLA_PICTURE_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VID_PLA_SUPER_FREEZE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VID_PLA_SUPER_FREEZE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VID_PLA_SUPER_BLANK_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VID_PLA_SUPER_BLANK_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VID_PLA_ATOMIC_CMD_T_u =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((VID_PLA_ATOMIC_CMD_T*) NULL)->u),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.pv_data"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_VID_PLA_ATOMIC_CMD_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VID_PLA_ATOMIC_CMD_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_VID_PLA_ATOMIC_CMD_T_u,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (VID_PLA_ATOMIC_CMD_T, u)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_VID_PLA_FORCE_UNMUTE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VID_PLA_FORCE_UNMUTE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CAP_INPUT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CAP_INPUT_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (CAP_INPUT_T, puac_dst_buffer)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (CAP_INPUT_T, puac_src_buffer)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_VID_PLA_ANGULAR_SCE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VID_PLA_ANGULAR_SCE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_EXIF_RATIONAL_DATA =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (EXIF_RATIONAL_DATA),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_EXIF_SRATIONAL_DATA =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (EXIF_SRATIONAL_DATA),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_EXIF_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (EXIF_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (EXIF_INFO_T, pv_data)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_EXIF_CONTEXT_DATA =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (EXIF_CONTEXT_DATA),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_IMG_ENG_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (IMG_ENG_DATA_T),
    .ui4_num_entries = 3,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (IMG_ENG_DATA_T, pv_data)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (IMG_ENG_DATA_T, pv_data2)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (IMG_ENG_DATA_T, pv_type_data)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_IMG_ANIMATE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (IMG_ANIMATE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_IMG_TYPE_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (IMG_TYPE_DATA_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (IMG_TYPE_DATA_T, pv_tag)
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_IMG_ENG_DATA_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (IMG_TYPE_DATA_T, t_eng_data)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_IMG_ENG_TYPE_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (IMG_ENG_TYPE_DATA_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_IMG_CAPS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (IMG_CAPS_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_IMG_NFY_LOAD_WITH_TAG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (IMG_NFY_LOAD_WITH_TAG_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (IMG_NFY_LOAD_WITH_TAG_T, pv_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_IMG_NFY_GIF_LOAD_PARAM_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (IMG_NFY_GIF_LOAD_PARAM_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (IMG_NFY_GIF_LOAD_PARAM_T, pv_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_CAP_OUTPUT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CAP_OUTPUT_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (CAP_OUTPUT_T, puac_dst_buffer)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_IMAGE_FILE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (IMAGE_FILE_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_IMAGE_PULL_FM_FUNC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (IMAGE_PULL_FM_FUNC_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (IMAGE_PULL_FM_FUNC_T, pv_app_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_WGL_ANIM_CONFIG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (WGL_ANIM_CONFIG_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_WGL_CANVAS_CFG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (WGL_CANVAS_CFG_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_WGL_CVS_CLR_CFG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (WGL_CVS_CLR_CFG_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_WGL_CANVAS_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (WGL_CANVAS_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_WGL_NOTIFY_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (WGL_NOTIFY_DATA_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_WGL_CREATE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (WGL_CREATE_INFO_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_GL_RECT_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (WGL_CREATE_INFO_T, pt_rect)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (WGL_CREATE_INFO_T, pv_param)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_WGL_PACKED_PARAM_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (WGL_PACKED_PARAM_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (WGL_PACKED_PARAM_T, pv_param1)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (WGL_PACKED_PARAM_T, pv_param2)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_WGL_KEY_LINK_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (WGL_KEY_LINK_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_WGL_IMG_DECODE_FUNCTION_TABLE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (WGL_IMG_DECODE_FUNCTION_TABLE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_WGL_COLOR_BASIC_SET_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (WGL_COLOR_BASIC_SET_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_WGL_COLOR_STD_SET_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (WGL_COLOR_STD_SET_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_WGL_COLOR_EXT_SET_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (WGL_COLOR_EXT_SET_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_WGL_COLOR_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (WGL_COLOR_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_WGL_IMG_BASIC_SET_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (WGL_IMG_BASIC_SET_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_WGL_IMG_STD_SET_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (WGL_IMG_STD_SET_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_WGL_IMG_EXT_SET_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (WGL_IMG_EXT_SET_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_WGL_IMG_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (WGL_IMG_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_WGL_FONT_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (WGL_FONT_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_WGL_FONT_BASIC_SET_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (WGL_FONT_BASIC_SET_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_WGL_FONT_INFO_EX_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (WGL_FONT_INFO_EX_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_WGL_INSET_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (WGL_INSET_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_WGL_TEXT_ATTR_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (WGL_TEXT_ATTR_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (WGL_TEXT_ATTR_T, pv_attr_data)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_WGL_UPD_REGION_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (WGL_UPD_REGION_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_WGL_SBTL_REGION_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (WGL_SBTL_REGION_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_WGL_SBTL_INIT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (WGL_SBTL_INIT_T),
    .ui4_num_entries = 3,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (WGL_SBTL_INIT_T, pv_tag_lock_pib)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (WGL_SBTL_INIT_T, pv_tag_unlock_pib)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (WGL_SBTL_INIT_T, pv_tag_get_rgn)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_WGL_SBTL_LOGI_SZ_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (WGL_SBTL_LOGI_SZ_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SBTL_MKV_CONTENT_ENCODING_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SBTL_MKV_CONTENT_ENCODING_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SBTL_MKV_CONTENT_ENCODINGS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SBTL_MKV_CONTENT_ENCODINGS_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_SBTL_MKV_CONTENT_ENCODING_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SBTL_MKV_CONTENT_ENCODINGS_T, at_encodings)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SBTL_MKV_CODEC_PRIVATE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SBTL_MKV_CODEC_PRIVATE_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SBTL_MKV_CODEC_PRIVATE_T, aui1_codec_private)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SBTL_MKV_SBTL_CMD_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SBTL_MKV_SBTL_CMD_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_SBTL_MKV_CODEC_PRIVATE_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SBTL_MKV_SBTL_CMD_T, t_mkv_sbtl_prv)
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_SBTL_MKV_CONTENT_ENCODINGS_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SBTL_MKV_SBTL_CMD_T, t_mkv_sbtl_cont_encode)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SBTL_DISPLAY_INFO_CMD_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SBTL_DISPLAY_INFO_CMD_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SBTL_LYRIC_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SBTL_LYRIC_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_SBTL_DISP_PAGE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_SBTL_DISP_PAGE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_SBTL_TM_OFST_INF =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_SBTL_TM_OFST_INF),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_SBTL_RECORD_INF =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_SBTL_RECORD_INF),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_SBTL_PTS_CB =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_SBTL_PTS_CB),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MM_SBTL_PTS_CB, pv_pts_cb_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SMAI_CC_INF_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SMAI_CC_INF_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SAMI_STREAM_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SAMI_STREAM_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SBTL_LANG_DESC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SBTL_LANG_DESC_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SBTL_LANG_DESC_T, pc_sbtl_file_name)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SBTL_LANG_DESC_T, pc_related_file_name)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MM_EXT_SBTL_INFO_u =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((MM_EXT_SBTL_INFO*) NULL)->u),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.ps_full"
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.ps_ext_lst"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MM_EXT_SBTL_INFO =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_EXT_SBTL_INFO),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MM_EXT_SBTL_INFO_u,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MM_EXT_SBTL_INFO, u)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MM_EXT_SBTL_INFO, ps_prf_lang_lst)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MM_LYRIC_INFO_u =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((MM_LYRIC_INFO*) NULL)->u),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.ps_full"
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.ps_lyric_list"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MM_LYRIC_INFO =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_LYRIC_INFO),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MM_LYRIC_INFO_u,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MM_LYRIC_INFO, u)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MM_LYRIC_INFO, ps_prf_lang_lst)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MM_SVC_COMMAND_T_u =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((MM_SVC_COMMAND_T*) NULL)->u),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.ps_text"
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.pv_data"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MM_SVC_COMMAND_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_SVC_COMMAND_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MM_SVC_COMMAND_T_u,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MM_SVC_COMMAND_T, u)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_DIVX_DRM_INFO_SVCTX_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DIVX_DRM_INFO_SVCTX_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_DIVX_DRM_BASIC_INFO_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (DIVX_DRM_INFO_SVCTX_T, pt_divx_drm_basic_info)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_DRM_OUTPUT_SIGNAL_PROTECTION_INFO_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (DIVX_DRM_INFO_SVCTX_T, pt_drm_output_signal_protection_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_DRM_INFO_SVCTX_T_u =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((DRM_INFO_SVCTX_T*) NULL)->u),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_DIVX_DRM_INFO_SVCTX_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_divx_drm_info_svctx"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_DRM_INFO_SVCTX_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DRM_INFO_SVCTX_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_DRM_INFO_SVCTX_T_u,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (DRM_INFO_SVCTX_T, u)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MM_SVC_SBTL_DISP_MODE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_SVC_SBTL_DISP_MODE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_SVC_SBTL_HILT_STL_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_SVC_SBTL_HILT_STL_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_SVC_SBTL_TIME_OFST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_SVC_SBTL_TIME_OFST_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_SVC_SBTL_FONT_ENC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_SVC_SBTL_FONT_ENC_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_SVC_SBTL_DISP_ATTR_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_SVC_SBTL_DISP_ATTR_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_SVC_SBTL_SHOW_HIDE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_SVC_SBTL_SHOW_HIDE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVC_MM_SBTL_LYRIC_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVC_MM_SBTL_LYRIC_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_VIDEO_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_VIDEO_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_PUSH_BUF_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_PUSH_BUF_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MM_PUSH_BUF_INFO_T, pu1_push_buf)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MM_SVC_RENDER_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_SVC_RENDER_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_SVC_POS_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_SVC_POS_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_SVC_CUR_CHAP_ID_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_SVC_CUR_CHAP_ID_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_SVC_DUR_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_SVC_DUR_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_SVC_TIME_CODE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_SVC_TIME_CODE_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_SVC_PTS_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_SVC_PTS_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_REPEAT_AB_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_REPEAT_AB_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_SVC_REPEAT_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_SVC_REPEAT_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_SVC_TRICK_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_SVC_TRICK_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_SVC_CHAPTER_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_SVC_CHAPTER_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_SVC_TITLE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_SVC_TITLE_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_SVC_BTN_INFO_T_u =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((MM_SVC_BTN_INFO_T*) NULL)->u),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.ps_next"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MM_SVC_BTN_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_SVC_BTN_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MM_SVC_BTN_INFO_T_u,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MM_SVC_BTN_INFO_T, u)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MM_SVC_BTN_ACTION_INFO_T_u =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((MM_SVC_BTN_ACTION_INFO_T*) NULL)->u),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.ps_next"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MM_SVC_BTN_ACTION_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_SVC_BTN_ACTION_INFO_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MM_SVC_BTN_INFO_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MM_SVC_BTN_ACTION_INFO_T, t_btn_type)
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MM_SVC_BTN_ACTION_INFO_T_u,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MM_SVC_BTN_ACTION_INFO_T, u)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MM_SVC_BUFFCTRL_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_SVC_BUFFCTRL_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_SVC_TIME_TO_SIZE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_SVC_TIME_TO_SIZE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_SVC_BUF_STATUS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_SVC_BUF_STATUS_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_DRM_CTRL_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_DRM_CTRL_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_MLN_DRM_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_MLN_DRM_INFO_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MM_MLN_DRM_INFO_T, pui1_act_tkn_url)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MM_MLN_DRM_INFO_T, pui1_afl_tkn)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MM_DRM_INFO_T_u =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((MM_DRM_INFO_T*) NULL)->u),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MM_MLN_DRM_INFO_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_mln_drm_inf"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MM_DRM_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_DRM_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MM_DRM_INFO_T_u,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MM_DRM_INFO_T, u)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SRC_MM_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SRC_MM_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MM_DRM_INFO_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SRC_MM_INFO_T, t_ml_drm)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MM_TIMESHIFT_RANGE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_TIMESHIFT_RANGE_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_STRM_ATRBT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_STRM_ATRBT_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MINFO_TYPE_STM_ATRBT_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MM_STRM_ATRBT_T, t_stm_atrbt)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MM_STRM_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_STRM_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MM_STRM_ATRBT_T,
            .ui4_num_entries = 17,

            {
                .z_offs = offsetof (MM_STRM_INFO_T, at_stm_atrbt)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MM_3D_VID_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_3D_VID_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_STRM_CODEC_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_STRM_CODEC_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_BUF_AGT_FLOW_CTRL_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_BUF_AGT_FLOW_CTRL_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_SVC_STOP_INFO =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_SVC_STOP_INFO),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_ID_CVT_CNDTN_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ID_CVT_CNDTN_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVCTX_MHEG_SCREEN_MODE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_MHEG_SCREEN_MODE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVCTX_ANA_AUTO_DETECTED_AUD_SYS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_ANA_AUTO_DETECTED_AUD_SYS_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVCTX_SCDB_SORT_PARAM_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_SCDB_SORT_PARAM_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SVCTX_SCDB_SORT_PARAM_T, pv_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SVCTX_ATOMIC_CMD_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_ATOMIC_CMD_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SVCTX_ATOMIC_CMD_T, pv_data)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SVCTX_VID_STREAM_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_VID_STREAM_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVCTX_AUD_STREAM_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_AUD_STREAM_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVCTX_STREAM_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_STREAM_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVCTX_VIDEO_MUTE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_VIDEO_MUTE_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVCTX_GET_SCDB_NUM_RECS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_GET_SCDB_NUM_RECS_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVCTX_GET_SCDB_REC_BY_IDX_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_GET_SCDB_REC_BY_IDX_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_STREAM_COMP_ID_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SVCTX_GET_SCDB_REC_BY_IDX_T, t_strm_comp_id)
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_SCDB_REC_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SVCTX_GET_SCDB_REC_BY_IDX_T, t_scdb_rec)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SVCTX_GET_USING_SCDB_REC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_GET_USING_SCDB_REC_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_STREAM_COMP_ID_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SVCTX_GET_USING_SCDB_REC_T, t_strm_comp_id)
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_SCDB_REC_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SVCTX_GET_USING_SCDB_REC_T, t_scdb_rec)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SVCTX_COPY_PROTECTION_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_COPY_PROTECTION_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVCTX_CA_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_CA_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SIGNAL_LOSS_DELAY_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SIGNAL_LOSS_DELAY_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCART_OUT_DESC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCART_OUT_DESC_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SCART_OUT_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SCART_OUT_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_SCART_OUT_DESC_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SCART_OUT_INFO_T, pt_descs)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_CH_BLOCK_BIAS_FREQ_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CH_BLOCK_BIAS_FREQ_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_RIKS_TV_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (RIKS_TV_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SRC_DESC_T_u_info =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((SRC_DESC_T*) NULL)->u_info),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_SRC_MM_INFO_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u_info.t_mm"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SRC_DESC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SRC_DESC_T),
    .ui4_num_entries = 5,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SRC_DESC_T, ps_conn_src_name)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SRC_DESC_T, pv_details)
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_SCART_OUT_INFO_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SRC_DESC_T, t_scart_out)
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_SRC_DESC_T_u_info,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SRC_DESC_T, u_info)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SRC_DESC_T, ps_conn_src_type)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_STRM_CC_DESC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (STRM_CC_DESC_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_SM_CC_FILTER_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (STRM_CC_DESC_T, pt_filter)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (STRM_CC_DESC_T, pv_fltr_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_STRM_SUBTITLE_DESC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (STRM_SUBTITLE_DESC_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (STRM_SUBTITLE_DESC_T, pv_fltr_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_STRM_TELTEX_DESC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (STRM_TELTEX_DESC_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (STRM_TELTEX_DESC_T, pv_fltr_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SNK_DESC_T_u_text_strm =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((SNK_DESC_T*) NULL)->u_text_strm),
    .ui4_num_entries = 3,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_STRM_CC_DESC_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u_text_strm.t_cc"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_STRM_SUBTITLE_DESC_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u_text_strm.t_subtitle"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_STRM_TELTEX_DESC_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u_text_strm.t_telex"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SNK_DESC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SNK_DESC_T),
    .ui4_num_entries = 11,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SNK_DESC_T, ps_snk_grp_name)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SNK_DESC_T, ps_snk_grp_name_aux)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_VSH_REGION_INFO_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SNK_DESC_T, pt_video_region)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_VSH_REGION_INFO_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SNK_DESC_T, pt_disp_region)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_VSH_SET_PLANE_ORDER_INFO_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SNK_DESC_T, pt_video_plane_order)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SNK_DESC_T, pv_video_strm_fltr_tag)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_SCC_AUD_VOLUME_INFO_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SNK_DESC_T, pt_volume_info)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SNK_DESC_T, pv_audio_strm_fltr_tag)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_SM_CC_FILTER_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SNK_DESC_T, pt_cc_filter)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SNK_DESC_T, pv_cc_strm_fltr_tag)
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_SNK_DESC_T_u_text_strm,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SNK_DESC_T, u_text_strm)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SVCTX_ASSC_CFG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_ASSC_CFG_T),
    .ui4_num_entries = 6,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SVCTX_ASSC_CFG_T, pv_select_svc_nfy_tag)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SVCTX_ASSC_CFG_T, pv_video_strm_fltr_tag)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SVCTX_ASSC_CFG_T, pv_audio_strm_fltr_tag)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SVCTX_ASSC_CFG_T, pv_cc_strm_fltr_tag)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SVCTX_ASSC_CFG_T, pv_sbtl_strm_fltr_tag)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SVCTX_ASSC_CFG_T, pv_ttx_strm_fltr_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_EVENT_CAPABILITY_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (EVENT_CAPABILITY_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_TEXT_CAPABILITY_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TEXT_CAPABILITY_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_DEVICE_TYPE_PROFILE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DEVICE_TYPE_PROFILE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_TUNER_OUT_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TUNER_OUT_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_TUNER_OUT_DESC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TUNER_OUT_DESC_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVCTX_VIDEO_THUMBNAIL_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_VIDEO_THUMBNAIL_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SVCTX_VIDEO_THUMBNAIL_INFO_T, pv_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_ID_IND_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ID_IND_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (ID_IND_T, pv_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_ID_LIST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ID_LIST_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_ID_IND_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (ID_LIST_T, pt_list)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_ID_RANGE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ID_RANGE_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (ID_RANGE_T, pv_first_tag)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (ID_RANGE_T, pv_delta_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_DRV_COMP_ID_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DRV_COMP_ID_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (DRV_COMP_ID_T, pv_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_DRV_COMP_REG_T_u =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((DRV_COMP_REG_T*) NULL)->u),
    .ui4_num_entries = 3,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_ID_IND_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_ind"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_ID_RANGE_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_range"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_ID_LIST_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_list"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_DRV_COMP_REG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DRV_COMP_REG_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_DRV_COMP_REG_T_u,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (DRV_COMP_REG_T, u)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_DRV_COMP_FCT_TBL_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DRV_COMP_FCT_TBL_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_DIVX_DRM_CRYPTO_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DIVX_DRM_CRYPTO_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_DIVX_DRM_INFO_DRV_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DIVX_DRM_INFO_DRV_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_DIVX_DRM_CRYPTO_INFO_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (DIVX_DRM_INFO_DRV_T, pt_divx_drm_crypto_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_DRM_INFO_DRV_T_u =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((DRM_INFO_DRV_T*) NULL)->u),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_DIVX_DRM_INFO_DRV_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_divx_drm_info_drv"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_DRM_INFO_DRV_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DRM_INFO_DRV_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_DRM_INFO_DRV_T_u,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (DRM_INFO_DRV_T, u)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_DIVX_DRM_HW_SECRET_KEY_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DIVX_DRM_HW_SECRET_KEY_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FEEDER_SRC_INPUT_FCT_TBL_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FEEDER_SRC_INPUT_FCT_TBL_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MEDIA_FEEDER_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MEDIA_FEEDER_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_PB_CTRL_PULL_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_PB_CTRL_PULL_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MM_PB_CTRL_PULL_INFO_T, pv_app_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MM_PB_CTRL_PUSH_BUF_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_PB_CTRL_PUSH_BUF_INFO_T),
    .ui4_num_entries = 3,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MM_PB_CTRL_PUSH_BUF_INFO_T, pv_buffer)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MM_PB_CTRL_PUSH_BUF_INFO_T, pv_private)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MM_PB_CTRL_PUSH_BUF_INFO_T, pv_private2)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MM_PB_CTRL_PUSH_MODE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_PB_CTRL_PUSH_MODE_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MEDIA_MULTI_CACHE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MEDIA_MULTI_CACHE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MEDIA_FEEDER_NW_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MEDIA_FEEDER_NW_T),
    .ui4_num_entries = 6,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MM_PB_CTRL_PULL_INFO_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MEDIA_FEEDER_NW_T, t_pull_read_fct_tbl)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_MEDIA_MULTI_CACHE_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MEDIA_FEEDER_NW_T, pt_free_cache)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_MEDIA_MULTI_CACHE_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MEDIA_FEEDER_NW_T, pt_used_cache)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MEDIA_FEEDER_NW_T, aui1_phy_buf)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MEDIA_FEEDER_NW_T, aui1_share_buf)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MEDIA_FEEDER_NW_T, pv_app_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_STREAM_ID_PS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (STREAM_ID_PS_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_STREAM_ID_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (STREAM_ID_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_PVR_PIC_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PVR_PIC_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_PVR_TICK_DETAIL_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PVR_TICK_DETAIL_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_PVR_TICK_INDEX_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PVR_TICK_INDEX_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_PVR_TICK_DETAIL_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (PVR_TICK_INDEX_T, pt_tick_detail)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_PVR_TICK_CTRL_BLK_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PVR_TICK_CTRL_BLK_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (PVR_TICK_CTRL_BLK_T, pv_start_address)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (PVR_TICK_CTRL_BLK_T, pv_end_address)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_START_POS_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (START_POS_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_RANGE_POS_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_RANGE_POS_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_POS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_POS_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_AB_POS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_AB_POS_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_BUF_RANGE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_BUF_RANGE_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_REC_BUF_RANGE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_REC_BUF_RANGE_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_RANGE_ELEMT_IDXTBL_TS_ENTRY_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_RANGE_ELEMT_IDXTBL_TS_ENTRY_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_PVR_TICK_CTRL_BLK_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MM_RANGE_ELEMT_IDXTBL_TS_ENTRY_T, t_tick_ctrl_blk)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MM_RANGE_ELMT_IDXTBL_T_u =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((MM_RANGE_ELMT_IDXTBL_T*) NULL)->u),
    .ui4_num_entries = 3,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.pv_idx_tbl_entry"
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.pt_pb_idx_tbl_entry"
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_MM_RANGE_ELEMT_IDXTBL_TS_ENTRY_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.pt_ts_idx_tbl_entry"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MM_RANGE_ELMT_IDXTBL_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_RANGE_ELMT_IDXTBL_T),
    .ui4_num_entries = 4,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MM_RANGE_ELMT_IDXTBL_T_u,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MM_RANGE_ELMT_IDXTBL_T, u)
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_ASH_AUDIO_CODEC_SPEC_INFO_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MM_RANGE_ELMT_IDXTBL_T, t_aud_spec_info)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_MM_RANGE_ELMT_IDXTBL_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MM_RANGE_ELMT_IDXTBL_T, pt_next_partial)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MM_RANGE_ELMT_IDXTBL_T, pv_next_tbl)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MM_RANGE_TRACK_TM_SCALE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_RANGE_TRACK_TM_SCALE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_AUDIO_VORBIS_OFFSET_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_AUDIO_VORBIS_OFFSET_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_RANGE_DECODER_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_RANGE_DECODER_T),
    .ui4_num_entries = 3,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MM_RANGE_DECODER_T, pv_encoding_string)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MM_RANGE_DECODER_T, pui1_codec_private)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_MM_RANGE_DECODER_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MM_RANGE_DECODER_T, pt_next)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MM_RANGE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_RANGE_INFO_T),
    .ui4_num_entries = 6,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_MM_RANGE_ELMT_IDXTBL_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MM_RANGE_INFO_T, pt_idxtbl_lst)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_MM_RANGE_TRACK_TM_SCALE_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MM_RANGE_INFO_T, pt_track_tm_scale_lst)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MM_RANGE_INFO_T, pv_tag)
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_DRM_INFO_DRV_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MM_RANGE_INFO_T, t_drm_info_drv)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_MM_RANGE_DECODER_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MM_RANGE_INFO_T, pt_vid_decoder_info)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_MM_RANGE_DECODER_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MM_RANGE_INFO_T, pt_aud_decoder_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MM_PVR_CRYPT_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_PVR_CRYPT_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_TRACK_CRYPT_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_TRACK_CRYPT_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_ML_CRYPT_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_ML_CRYPT_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_CRYPT_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_CRYPT_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_CRYPT_IV_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_CRYPT_IV_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_DESC_CRYPT_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_DESC_CRYPT_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MM_CRYPT_CMD_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MM_CRYPT_CMD_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MM_CRYPT_CMD_T, pt_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_REC_UTIL_CLI_DUMP_TICK_CB_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (REC_UTIL_CLI_DUMP_TICK_CB_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVCTX_REC_ES_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_REC_ES_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVCTX_REC_ES_LIST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_REC_ES_LIST_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_SVCTX_REC_ES_INFO_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SVCTX_REC_ES_LIST_T, at_es_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SVCTX_REC_DEL_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_REC_DEL_DATA_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SVCTX_REC_DEL_DATA_T, s_head_file)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SVCTX_REC_ATTR_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_REC_ATTR_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVCTX_REC_TIME_BOUND_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_REC_TIME_BOUND_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVCTX_REC_SPACE_BOUND_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_REC_SPACE_BOUND_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVCTX_REC_STRG_DESC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_REC_STRG_DESC_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SVCTX_REC_STRG_DESC_T, s_strg_path)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SVCTX_REC_CONT_INFO_NORMAL_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_REC_CONT_INFO_NORMAL_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVCTX_REC_CONT_INFO_CONT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_REC_CONT_INFO_CONT_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SVCTX_REC_CONT_INFO_CONT_T, s_strg_path)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SVCTX_REC_CONFIG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_REC_CONFIG_T),
    .ui4_num_entries = 6,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SVCTX_REC_CONFIG_T, pv_nfy_tag)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SVCTX_REC_CONFIG_T, pv_rec_purpose)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SVCTX_REC_CONFIG_T, pv_bound_info)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SVCTX_REC_CONFIG_T, s_db_name)
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_SVCTX_REC_STRG_DESC_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SVCTX_REC_CONFIG_T, t_strg_desc)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SVCTX_REC_CONFIG_T, pv_cont_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SVCTX_REC_CAP_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_REC_CAP_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVCTX_REC_SYS_RES_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_REC_SYS_RES_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVCTX_REC_STATUS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_REC_STATUS_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVCTX_REC_ES_AUTH_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_REC_ES_AUTH_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVCTX_REC_ESL_AUTH_STATUS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_REC_ESL_AUTH_STATUS_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_SVCTX_REC_ES_AUTH_INFO_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SVCTX_REC_ESL_AUTH_STATUS_T, at_es_auth_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SVCTX_REC_META_DB_OPEN_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_REC_META_DB_OPEN_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SVCTX_REC_META_DB_OPEN_T, s_head_file)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SVCTX_REC_META_DB_CLOSE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_REC_META_DB_CLOSE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVCTX_REC_META_DB_STORE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_REC_META_DB_STORE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVCTX_REC_SET_META_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_REC_SET_META_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SVCTX_REC_SET_META_INFO_T, pv_data)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SVCTX_REC_GET_META_INFO_LEN_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_REC_GET_META_INFO_LEN_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SVCTX_REC_GET_META_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SVCTX_REC_GET_META_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SVCTX_REC_GET_META_INFO_T, pv_buf)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SRC_REC_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SRC_REC_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SRC_REC_INFO_T, pv_detail)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_AVC_TYPE_MAPPING =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (AVC_TYPE_MAPPING),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FM_FLASH_PART_BULK_WRITE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FM_FLASH_PART_BULK_WRITE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FM_DIR_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FM_DIR_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FM_FILE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FM_FILE_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FM_DIR_ENTRY_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FM_DIR_ENTRY_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FM_FS_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FM_FS_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FM_MNT_PARM_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FM_MNT_PARM_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FM_PART_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FM_PART_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FM_MNT_CB_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FM_MNT_CB_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (FM_MNT_CB_T, pv_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_FM_CONFIRM_FILE_REPLACE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FM_CONFIRM_FILE_REPLACE_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (FM_CONFIRM_FILE_REPLACE_T, ps_exist_file)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_FM_CONFIRM_DIR_REPLACE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FM_CONFIRM_DIR_REPLACE_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (FM_CONFIRM_DIR_REPLACE_T, ps_exist_dir)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_FM_COPY_FILE_PRGS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FM_COPY_FILE_PRGS_T),
    .ui4_num_entries = 3,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (FM_COPY_FILE_PRGS_T, ps_copying)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (FM_COPY_FILE_PRGS_T, ps_from)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (FM_COPY_FILE_PRGS_T, ps_to)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_FM_SEARCH_RESULT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FM_SEARCH_RESULT_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (FM_SEARCH_RESULT_T, pps_pathv)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_FLM_LIST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLM_LIST_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FLM_REC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLM_REC_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_RRCTX_VCHIP_TYPE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (RRCTX_VCHIP_TYPE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_RRCTX_VCHIP_RATE_SETTING_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (RRCTX_VCHIP_RATE_SETTING_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_RRCTX_RRT_RATE_SETTING_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (RRCTX_RRT_RATE_SETTING_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (RRCTX_RRT_RATE_SETTING_T, pui1_bu)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (RRCTX_RRT_RATE_SETTING_T, pb_lvl_block)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_DTG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DTG_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_DELTA_DTG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DELTA_DTG_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_DT_CONFIG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DT_CONFIG_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (DT_CONFIG_T, pv_src_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_TM_SECOND_LOOP_ALL_DESCRS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TM_SECOND_LOOP_ALL_DESCRS_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_TM_SECOND_LOOP_DESCR_SEL_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TM_SECOND_LOOP_DESCR_SEL_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (TM_SECOND_LOOP_DESCR_SEL_T, pui1_descr_tag_list)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_TM_SECOND_LOOP_ALL_MULTI_DESCRS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TM_SECOND_LOOP_ALL_MULTI_DESCRS_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_TM_SECOND_LOOP_MULTI_DESCR_SEL_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TM_SECOND_LOOP_MULTI_DESCR_SEL_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (TM_SECOND_LOOP_MULTI_DESCR_SEL_T, pui1_descr_tag_list)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_TM_FIRST_LOOP_DESCR_SEL_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TM_FIRST_LOOP_DESCR_SEL_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (TM_FIRST_LOOP_DESCR_SEL_T, pui1_descr_tag_list)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_TM_MAIN_BODY_DESCR_SEL_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TM_MAIN_BODY_DESCR_SEL_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (TM_MAIN_BODY_DESCR_SEL_T, pui1_descr_tag_list)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_TM_UPDATE_TIME_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TM_UPDATE_TIME_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_TM_BUFFER_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TM_BUFFER_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (TM_BUFFER_T, pui1_buffer)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_ATSC_SI_MGT_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ATSC_SI_MGT_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_ATSC_SI_VCT_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ATSC_SI_VCT_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_ATSC_SI_RRT_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ATSC_SI_RRT_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_ATSC_SI_EIT_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ATSC_SI_EIT_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_ATSC_SI_TXT_LANG_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ATSC_SI_TXT_LANG_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_ATSC_SI_TXT_SEG_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ATSC_SI_TXT_SEG_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_ATSC_SI_LANG_SEG_IDX_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ATSC_SI_LANG_SEG_IDX_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_ATSC_SI_TXT_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ATSC_SI_TXT_INFO_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (ATSC_SI_TXT_INFO_T, ps_txt)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (ATSC_SI_TXT_INFO_T, pz_txt_len)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_ATSC_SI_TXT_DIM_VAL_IDX_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ATSC_SI_TXT_DIM_VAL_IDX_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_ATSC_SI_IDX_TIME_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ATSC_SI_IDX_TIME_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_ATSC_SI_SRC_ID_TIME_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ATSC_SI_SRC_ID_TIME_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_ATSC_SI_SRC_EVT_ID_TIME_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ATSC_SI_SRC_EVT_ID_TIME_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_EAS_EVENT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (EAS_EVENT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_EAS_LOC_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (EAS_LOC_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_EAS_XCEPT_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (EAS_XCEPT_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_EAS_EVT_CODE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (EAS_EVT_CODE_INFO_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (EAS_EVT_CODE_INFO_T, ps_evt_code)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (EAS_EVT_CODE_INFO_T, pz_evt_code_len)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_ATSC_SI_CONFIG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (ATSC_SI_CONFIG_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_FREQ_RANGE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_FREQ_RANGE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_NEW_SVC_NFY_DATA =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_NEW_SVC_NFY_DATA),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_LCNV2_CH_LST_INFO =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_LCNV2_CH_LST_INFO),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_ATSC_OPEN_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_ATSC_OPEN_DATA_T),
    .ui4_num_entries = 3,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_ATSC_OPEN_DATA_T, ps_svl_name)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_ATSC_OPEN_DATA_T, ps_file_name)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_ATSC_OPEN_DATA_T, ps_tuner_name)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SB_ATSC_SCAN_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_ATSC_SCAN_DATA_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_NTSC_OPEN_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_NTSC_OPEN_DATA_T),
    .ui4_num_entries = 3,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_NTSC_OPEN_DATA_T, ps_svl_name)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_NTSC_OPEN_DATA_T, ps_file_name)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_NTSC_OPEN_DATA_T, ps_tuner_name)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SB_NTSC_SCAN_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_NTSC_SCAN_DATA_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_CQAM_OPEN_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_CQAM_OPEN_DATA_T),
    .ui4_num_entries = 3,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_CQAM_OPEN_DATA_T, ps_svl_name)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_CQAM_OPEN_DATA_T, ps_file_name)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_CQAM_OPEN_DATA_T, ps_tuner_name)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SB_CQAM_SCAN_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_CQAM_SCAN_DATA_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_DVB_REGION_CODE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_DVB_REGION_CODE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_DVB_REGION_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_DVB_REGION_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_DVB_CH_LST_LCN_INFO =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_DVB_CH_LST_LCN_INFO),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_DVB_SCAN_INDEX_INFO =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_DVB_SCAN_INDEX_INFO),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_DVB_FREQ_BW_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_DVB_FREQ_BW_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_DVB_INIT_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_DVB_INIT_DATA_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_DVB_OPEN_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_DVB_OPEN_DATA_T),
    .ui4_num_entries = 3,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_DVB_OPEN_DATA_T, ps_svl_name)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_DVB_OPEN_DATA_T, ps_file_name)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_DVB_OPEN_DATA_T, ps_tuner_name)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SB_DVB_SCAN_INFO_T =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (SB_DVB_SCAN_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_DVB_SVC_UPDATE_NFY_DATA =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_DVB_SVC_UPDATE_NFY_DATA),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_DVB_SCAN_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_DVB_SCAN_DATA_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_DVB_SCAN_DATA_T, pv_svc_update_nfy_tag)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_DVB_SCAN_DATA_T, pv_new_svc_nfy_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SB_DVB_TSL_REC_ID_FREQ_RANGE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_DVB_TSL_REC_ID_FREQ_RANGE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_DVB_ON_TS_SVC_ID_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_DVB_ON_TS_SVC_ID_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_DVB_COUNTRY_ONID_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_DVB_COUNTRY_ONID_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_DVB_COUNTRY_ONID_T, pui2_onid)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SB_DVB_ONID_COUNT_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_DVB_ONID_COUNT_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_DVB_GET_OTHER_COUNTRY_ONID_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_DVB_GET_OTHER_COUNTRY_ONID_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_SB_DVB_ONID_COUNT_INFO_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_DVB_GET_OTHER_COUNTRY_ONID_INFO_T, pt_onid_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SB_DVB_NW_CHANGE_NFY_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_DVB_NW_CHANGE_NFY_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_DVB_NW_CHANGE_NFY_T, pv_nw_change_nfy_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_REGION_CODE_INFO_t_link =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (((REGION_CODE_INFO*) NULL)->t_link),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_REGION_CODE_INFO,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (REGION_CODE_INFO, t_link.pt_prev)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_REGION_CODE_INFO,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (REGION_CODE_INFO, t_link.pt_next)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_REGION_CODE_INFO =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (REGION_CODE_INFO),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_REGION_CODE_INFO_t_link,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (REGION_CODE_INFO, t_link)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_REGION_CODE_INFO_LIST =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (REGION_CODE_INFO_LIST),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_REGION_CODE_INFO,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (REGION_CODE_INFO_LIST, pt_head)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_REGION_CODE_INFO,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (REGION_CODE_INFO_LIST, pt_tail)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SB_DVB_LCN_CONFLICT_GROUP_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_DVB_LCN_CONFLICT_GROUP_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_DVB_LCN_CONFLICT_CHANNEL_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_DVB_LCN_CONFLICT_CHANNEL_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_REGION_CODE_INFO_LIST,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_DVB_LCN_CONFLICT_CHANNEL_T, t_list)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SB_DVBC_INIT_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_DVBC_INIT_DATA_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_DVBC_OPEN_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_DVBC_OPEN_DATA_T),
    .ui4_num_entries = 3,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_DVBC_OPEN_DATA_T, ps_svl_name)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_DVBC_OPEN_DATA_T, ps_file_name)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_DVBC_OPEN_DATA_T, ps_tuner_name)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SB_DVBC_SCAN_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_DVBC_SCAN_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_DVBC_DLVR_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_DVBC_DLVR_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_DVBC_TUNER_PARA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_DVBC_TUNER_PARA_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_DVBC_SVC_UPDATE_NFY_DATA =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_DVBC_SVC_UPDATE_NFY_DATA),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_DVB_SCANNED_CH_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_DVB_SCANNED_CH_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_DVBC_NW_CHANGE_NFY_DATA =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_DVBC_NW_CHANGE_NFY_DATA),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_DVBC_SCAN_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_DVBC_SCAN_DATA_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_DVBC_SCAN_DATA_T, pv_svc_update_nfy_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SB_DVBC_TSL_REC_ID_FREQ_RANGE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_DVBC_TSL_REC_ID_FREQ_RANGE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_DVBC_ON_TS_SVC_ID_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_DVBC_ON_TS_SVC_ID_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_DVBC_NW_CHANGE_NFY_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_DVBC_NW_CHANGE_NFY_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_DVBC_NW_CHANGE_NFY_T, pv_nw_change_nfy_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_VBIF_SRC_ANA_VBI_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VBIF_SRC_ANA_VBI_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VBIF_SRC_DIG_VBI_ES_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VBIF_SRC_DIG_VBI_ES_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VBIF_SRC_DIG_TTX_ES_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VBIF_SRC_DIG_TTX_ES_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FLTR_INIT_TTX_PKT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_INIT_TTX_PKT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_TTX_PKT_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TTX_PKT_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FLTR_NFY_TTX_PKT_SRC_ANA_VBI_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_NFY_TTX_PKT_SRC_ANA_VBI_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_TTX_PKT_INFO_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (FLTR_NFY_TTX_PKT_SRC_ANA_VBI_T, pat_pkt_buf)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_FLTR_TTX_PKT_BUFF_ATTR_ANA_VBI_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_TTX_PKT_BUFF_ATTR_ANA_VBI_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FLTR_NFY_TTX_PKT_SRC_DIG_VBI_ES_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_NFY_TTX_PKT_SRC_DIG_VBI_ES_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (FLTR_NFY_TTX_PKT_SRC_DIG_VBI_ES_T, pv_data_pes_pkt)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_FLTR_TTX_PKT_BUFF_ATTR_DIG_VBI_ES_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_TTX_PKT_BUFF_ATTR_DIG_VBI_ES_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FLTR_NFY_TTX_PKT_SRC_DIG_TTX_ES_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_NFY_TTX_PKT_SRC_DIG_TTX_ES_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (FLTR_NFY_TTX_PKT_SRC_DIG_TTX_ES_T, pv_data_pes_pkt)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_FLTR_TTX_PKT_BUFF_ATTR_DIG_TTX_ES_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_TTX_PKT_BUFF_ATTR_DIG_TTX_ES_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FLTR_TTX_PKT_BUFF_INFO_T_u =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((FLTR_TTX_PKT_BUFF_INFO_T*) NULL)->u),
    .ui4_num_entries = 3,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_FLTR_NFY_TTX_PKT_SRC_ANA_VBI_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_src_ana_vbi"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_FLTR_NFY_TTX_PKT_SRC_DIG_VBI_ES_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_src_dig_vbi_es"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_FLTR_NFY_TTX_PKT_SRC_DIG_TTX_ES_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_src_dig_ttx_es"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_FLTR_TTX_PKT_BUFF_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_TTX_PKT_BUFF_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_FLTR_TTX_PKT_BUFF_INFO_T_u,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (FLTR_TTX_PKT_BUFF_INFO_T, u)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_FLTR_TTX_PKT_BUFF_TAG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_TTX_PKT_BUFF_TAG_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FLTR_TTX_PKT_BUFF_LOCK_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_TTX_PKT_BUFF_LOCK_INFO_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC | ARG_DIR_INP,
            .pt_desc         = &t_rpc_decl_FLTR_TTX_PKT_BUFF_TAG_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (FLTR_TTX_PKT_BUFF_LOCK_INFO_T, pt_buf_tag)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_FLTR_TTX_PKT_BUFF_INFO_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (FLTR_TTX_PKT_BUFF_LOCK_INFO_T, pt_buf_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_FLTR_TTX_PKT_BUFF_ATTR_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_TTX_PKT_BUFF_ATTR_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC | ARG_DIR_INP,
            .pt_desc         = &t_rpc_decl_FLTR_TTX_PKT_BUFF_TAG_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (FLTR_TTX_PKT_BUFF_ATTR_T, pt_buf_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_FLTR_NFY_TTX_PKT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_NFY_TTX_PKT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FLTR_INIT_TTX_MIP_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_INIT_TTX_MIP_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_TTX_MIP_NODE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TTX_MIP_NODE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FLTR_NFY_TTX_MIP_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_NFY_TTX_MIP_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (FLTR_NFY_TTX_MIP_T, pv_tag_mip)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_FLTR_NI_INIT_TTX_HDR_CH_NAME_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_NI_INIT_TTX_HDR_CH_NAME_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC | ARG_DIR_INP,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (FLTR_NI_INIT_TTX_HDR_CH_NAME_T, pv_tag_parse_ch_name)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_FLTR_NI_INIT_ACI_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_NI_INIT_ACI_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FLTR_INIT_NI_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_INIT_NI_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_FLTR_NI_INIT_TTX_HDR_CH_NAME_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (FLTR_INIT_NI_T, t_ttx_hdr_ch_name)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_FLTR_NI_NFY_TTX_HDR_CH_NAME_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_NI_NFY_TTX_HDR_CH_NAME_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (FLTR_NI_NFY_TTX_HDR_CH_NAME_T, ps_ch_name)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_FLTR_NI_NFY_VPS_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_NI_NFY_VPS_DATA_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FLTR_NI_NFY_8_30_FMT_1_PRG_TTL_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_NI_NFY_8_30_FMT_1_PRG_TTL_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FLTR_NI_NFY_TTX_DATE_TIME_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_NI_NFY_TTX_DATE_TIME_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FLTR_NI_NFY_ACI_DETECTED_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_NI_NFY_ACI_DETECTED_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FLTR_NI_NFY_ACI_SLCTN_PAGE_UPDATE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_NI_NFY_ACI_SLCTN_PAGE_UPDATE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FLTR_NI_NFY_ACI_DB_READY_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_NI_NFY_ACI_DB_READY_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FLTR_NI_NFY_ACI_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_NI_NFY_ACI_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FLTR_NFY_NI_T_u =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((FLTR_NFY_NI_T*) NULL)->u),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_FLTR_NI_NFY_TTX_HDR_CH_NAME_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_ttx_hdr_ch_name"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_FLTR_NFY_NI_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_NFY_NI_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_FLTR_NFY_NI_T_u,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (FLTR_NFY_NI_T, u)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_FLTR_NI_ACI_DB_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_NI_ACI_DB_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FLTR_NI_ACI_CITY_LIST_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_NI_ACI_CITY_LIST_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FLTR_NI_ACI_CITY_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_NI_ACI_CITY_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FLTR_NI_ACI_CH_NUM_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_NI_ACI_CH_NUM_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FLTR_NI_ACI_CH_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_NI_ACI_CH_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FLTR_INIT_EU_EVN_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_INIT_EU_EVN_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FLTR_EU_EVN_DATA_PRG_TTL_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_EU_EVN_DATA_PRG_TTL_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FLTR_NFY_EU_EVN_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_NFY_EU_EVN_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FLTR_INIT_EPG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_INIT_EPG_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FLTR_NFY_EPG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_NFY_EPG_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (FLTR_NFY_EPG_T, pv_tag_epg)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (FLTR_NFY_EPG_T, paui1_data)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_FLTR_INIT_EU_WSS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_INIT_EU_WSS_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FLTR_NFY_EU_WSS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_NFY_EU_WSS_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_FLTR_INIT_EU_VBI_D2A_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (FLTR_INIT_EU_VBI_D2A_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_PAL_SECAM_INIT_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_PAL_SECAM_INIT_DATA_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_PAL_SECAM_OPEN_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_PAL_SECAM_OPEN_DATA_T),
    .ui4_num_entries = 3,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_PAL_SECAM_OPEN_DATA_T, ps_svl_name)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_PAL_SECAM_OPEN_DATA_T, ps_file_name)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_PAL_SECAM_OPEN_DATA_T, ps_tuner_name)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SB_PAL_SECAM_FREQ_PROGRESS_NFY_DATA =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_PAL_SECAM_FREQ_PROGRESS_NFY_DATA),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_PAL_SECAM_ANAS_NFY_DATA =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_PAL_SECAM_ANAS_NFY_DATA),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_PAL_SECAM_CUSTOM_SVL_CONFIG =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_PAL_SECAM_CUSTOM_SVL_CONFIG),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_PAL_SECAM_ACI_NFY_DATA =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_PAL_SECAM_ACI_NFY_DATA),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_PAL_SECAM_USER_OPERATION_DATA =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_PAL_SECAM_USER_OPERATION_DATA),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_PAL_SECAM_SCAN_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_PAL_SECAM_SCAN_DATA_T),
    .ui4_num_entries = 6,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_PAL_SECAM_SCAN_DATA_T, ps_aud_name)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_PAL_SECAM_SCAN_DATA_T, pv_freq_nfy_tag)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_PAL_SECAM_SCAN_DATA_T, pv_user_operation_nfy_tag)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_PAL_SECAM_SCAN_DATA_T, pv_anas_nfy_tag)
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_FLTR_NI_INIT_TTX_HDR_CH_NAME_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_PAL_SECAM_SCAN_DATA_T, t_ttx_hdr_ch_name)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_PAL_SECAM_SCAN_DATA_T, pv_aci_nfy_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SB_ACI_SLCT_CITY_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_ACI_SLCT_CITY_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_USER_OPERATION_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_USER_OPERATION_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_INSERT_DEFAULT_SVL_TSL_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_INSERT_DEFAULT_SVL_TSL_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_ISDB_INIT_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_ISDB_INIT_DATA_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_ISDB_OPEN_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_ISDB_OPEN_DATA_T),
    .ui4_num_entries = 3,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_ISDB_OPEN_DATA_T, ps_svl_name)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_ISDB_OPEN_DATA_T, ps_file_name)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SB_ISDB_OPEN_DATA_T, ps_tuner_name)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_SB_ISDB_SCAN_INFO_T =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (SB_ISDB_SCAN_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SB_ISDB_SCAN_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SB_ISDB_SCAN_DATA_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_LOL_REC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (LOL_REC_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_LOL_MEM_DUMP_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (LOL_MEM_DUMP_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (LOL_MEM_DUMP_T, pv_data)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_NWL_REC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (NWL_REC_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_NWL_MEM_DUMP_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (NWL_MEM_DUMP_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (NWL_MEM_DUMP_T, pv_data)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_PCL_GPIO_MONITOR_CTRL_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PCL_GPIO_MONITOR_CTRL_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_PCL_GPIO_MONITOR_CTRL_EX_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PCL_GPIO_MONITOR_CTRL_EX_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_PCL_SIF_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PCL_SIF_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (PCL_SIF_INFO_T, pui1_data)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_PCL_SIF_INFO_BY_GPIO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PCL_SIF_INFO_BY_GPIO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (PCL_SIF_INFO_BY_GPIO_T, pui1_data)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_PCL_SERVO_AD_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PCL_SERVO_AD_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_PCL_WAKE_UP_CTRL_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PCL_WAKE_UP_CTRL_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_RPT_EVT_ITVL_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (RPT_EVT_ITVL_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_DISABLE_EVTCODE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DISABLE_EVTCODE_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (DISABLE_EVTCODE_T, pui4_evtcode_arr)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MAGIC_UNIT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MAGIC_UNIT_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MAGIC_UNIT_T, pui1_magic)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MAGIC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MAGIC_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_MAGIC_UNIT_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MAGIC_T, pt_magic_unit)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_IRRC_SETTING_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (IRRC_SETTING_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_IOM_RAW_DATA_TO_EVT_CODE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (IOM_RAW_DATA_TO_EVT_CODE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_IOM_RAW_BITS_TO_EVT_CODE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (IOM_RAW_BITS_TO_EVT_CODE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_IOM_SYS_CODE_FILTER_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (IOM_SYS_CODE_FILTER_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (IOM_SYS_CODE_FILTER_T, pui4_filter_arr)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_IRRC_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (IRRC_DATA_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_DBG_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DBG_DATA_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_IOM_IND_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (IOM_IND_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_IOM_FPD_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (IOM_FPD_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (IOM_FPD_T, ps_content)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_IOM_UART_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (IOM_UART_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (IOM_UART_T, pui1_data)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_UART_SETTING_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (UART_SETTING_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_AUTO_SLEEP_TIMER_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (AUTO_SLEEP_TIMER_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (AUTO_SLEEP_TIMER_T, pv_nfy_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_RRCTX_RRT_KEY_PARAM_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (RRCTX_RRT_KEY_PARAM_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (RRCTX_RRT_KEY_PARAM_T, pv_rrt_key_param)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_EVCTX_ATSC_KEY_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (EVCTX_ATSC_KEY_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (EVCTX_ATSC_KEY_INFO_T, pv_atsc_key_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_EVCTX_ATSC_RATING_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (EVCTX_ATSC_RATING_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_EVCTX_ATSC_RATING_LIST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (EVCTX_ATSC_RATING_LIST_T),
    .ui4_num_entries = 3,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (EVCTX_ATSC_RATING_LIST_T, ps_rating_descrption)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_EVCTX_ATSC_RATING_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (EVCTX_ATSC_RATING_LIST_T, pt_ratings)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_EVCTX_ATSC_RATING_LIST_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (EVCTX_ATSC_RATING_LIST_T, pt_next)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_EVCTX_ATSC_ENG_CONFIG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (EVCTX_ATSC_ENG_CONFIG_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_PLAYBACK_CTRL_PULL_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PLAYBACK_CTRL_PULL_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (PLAYBACK_CTRL_PULL_INFO_T, pv_app_tag)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_PLAYBACK_FLOW_CTRL_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PLAYBACK_FLOW_CTRL_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_PLAYBACK_FEEDER_BUF_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PLAYBACK_FEEDER_BUF_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_PLAYBACK_MODE_CTRL_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PLAYBACK_MODE_CTRL_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_PLAYBACK_GET_BUFFER_FULLNESS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PLAYBACK_GET_BUFFER_FULLNESS_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_PLAYBACK_GET_BUFFER_STATUS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PLAYBACK_GET_BUFFER_STATUS_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_PLAYBACK_HANDLER_ITEM_ATTRIBUTES_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PLAYBACK_HANDLER_ITEM_ATTRIBUTES_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_PLAYBACK_HANDLER_ITEM_ATTRIBUTES_BY_INDEX_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PLAYBACK_HANDLER_ITEM_ATTRIBUTES_BY_INDEX_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_PLAYBACK_HANDLER_SHARED_MEMORY_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PLAYBACK_HANDLER_SHARED_MEMORY_T),
    .ui4_num_entries = 3,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (PLAYBACK_HANDLER_SHARED_MEMORY_T, pv_buffer)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (PLAYBACK_HANDLER_SHARED_MEMORY_T, pv_private)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (PLAYBACK_HANDLER_SHARED_MEMORY_T, pv_private2)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_PLAYBACK_HANDLER_TIME_TO_SIZE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PLAYBACK_HANDLER_TIME_TO_SIZE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_PLAYBACK_HANDLER_ITEM_SCDB_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PLAYBACK_HANDLER_ITEM_SCDB_INFO_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_SCDB_REC_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (PLAYBACK_HANDLER_ITEM_SCDB_INFO_T, t_scdb_rec)
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_STREAM_COMP_ID_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (PLAYBACK_HANDLER_ITEM_SCDB_INFO_T, t_stream_id)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_PLAYBACK_HANDLER_ITEM_SVC_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PLAYBACK_HANDLER_ITEM_SVC_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_PLAYBACK_HANDLER_ITEM_SVC_INFO_BY_ID_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PLAYBACK_HANDLER_ITEM_SVC_INFO_BY_ID_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_PLAYBACK_HANDLER_ITEM_MPEG_2_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PLAYBACK_HANDLER_ITEM_MPEG_2_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_PLAYBACK_HANDLER_ITEM_ES_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PLAYBACK_HANDLER_ITEM_ES_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_PLAYBACK_CTRL_IBC_PARAM_SET_ASF_PACKET_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PLAYBACK_CTRL_IBC_PARAM_SET_ASF_PACKET_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_PLAYBACK_CTRL_IBC_AES_ENCRYPT_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PLAYBACK_CTRL_IBC_AES_ENCRYPT_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_PLAYBACK_CTRL_IBC_PARAM_ENCRYPT_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PLAYBACK_CTRL_IBC_PARAM_ENCRYPT_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_PLAYBACK_CTRL_IBC_PARAM_T =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (PLAYBACK_CTRL_IBC_PARAM_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_PLAYBACK_HANDLER_ITEM_IBC_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PLAYBACK_HANDLER_ITEM_IBC_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_PLAYBACK_HANDLER_ITEM_EXTRA_INFO_T =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (PLAYBACK_HANDLER_ITEM_EXTRA_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_PLAYBACK_HANDLER_QUEUE_URI_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PLAYBACK_HANDLER_QUEUE_URI_T),
    .ui4_num_entries = 5,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC | ARG_DIR_INP,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (PLAYBACK_HANDLER_QUEUE_URI_T, ps_path)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_PLAYBACK_HANDLER_ITEM_SCDB_INFO_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (PLAYBACK_HANDLER_QUEUE_URI_T, pt_scdb_info)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC | ARG_DIR_INP,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (PLAYBACK_HANDLER_QUEUE_URI_T, ps_proxy_url)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC | ARG_DIR_INP,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (PLAYBACK_HANDLER_QUEUE_URI_T, ps_agent_url)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC | ARG_DIR_INP,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (PLAYBACK_HANDLER_QUEUE_URI_T, ps_cookie_url)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_PLAYBACK_HANDLER_QUEUE_FILE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PLAYBACK_HANDLER_QUEUE_FILE_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC | ARG_DIR_INP,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (PLAYBACK_HANDLER_QUEUE_FILE_T, ps_path)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_PLAYBACK_HANDLER_ITEM_SCDB_INFO_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (PLAYBACK_HANDLER_QUEUE_FILE_T, pt_scdb_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_PLAYBACK_HANDLER_QUEUE_BUFFER_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PLAYBACK_HANDLER_QUEUE_BUFFER_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (PLAYBACK_HANDLER_QUEUE_BUFFER_T, pv_data)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_PLAYBACK_HANDLER_ITEM_SCDB_INFO_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (PLAYBACK_HANDLER_QUEUE_BUFFER_T, pt_scdb_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_PLAYBACK_HANDLER_QUEUE_PULL_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PLAYBACK_HANDLER_QUEUE_PULL_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_PLAYBACK_CTRL_PULL_INFO_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (PLAYBACK_HANDLER_QUEUE_PULL_T, t_pull_info)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_PLAYBACK_HANDLER_ITEM_SCDB_INFO_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (PLAYBACK_HANDLER_QUEUE_PULL_T, pt_scdb_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_PSI_PID_SVC_ID_MAP_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PSI_PID_SVC_ID_MAP_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_PLAYBACK_PMT_LIST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PLAYBACK_PMT_LIST_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_SET_CEC_FCT_PARAMS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_SET_CEC_FCT_PARAMS_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_ARC_CTRL_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_ARC_CTRL_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_ACT_SRC_REVED_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_ACT_SRC_REVED_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_VNDR_CMD_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_VNDR_CMD_DATA_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_SPEC_CMD_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_SPEC_CMD_DATA_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_VNDR_CMD_WITH_ID_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_VNDR_CMD_WITH_ID_DATA_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_VNDR_RC_CODE_DATA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_VNDR_RC_CODE_DATA_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_DEV_LIST_NFY_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_DEV_LIST_NFY_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_5V_PWR_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_5V_PWR_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_CTRL_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_CTRL_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_DEV_DISCOVERY_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_DEV_DISCOVERY_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_LOG_ADD_LST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_LOG_ADD_LST_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_WAKE_UP_CTRL_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_WAKE_UP_CTRL_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_ONE_TOUCH_PLAY_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_ONE_TOUCH_PLAY_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_ACT_SRC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_ACT_SRC_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_ROUTING_CHG_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_ROUTING_CHG_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_ANA_SVC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_ANA_SVC_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_DIG_SVC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_DIG_SVC_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_REC_SRC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_REC_SRC_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_TIMER_ANA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_TIMER_ANA_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_TIMER_EXT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_TIMER_EXT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_TIMER_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_TIMER_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_TIMER_STS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_TIMER_STS_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_REPORT_PHY_ADDR_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_REPORT_PHY_ADDR_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_POLLING_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_POLLING_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_TUNER_SVC_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_TUNER_SVC_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_TUNER_DEV_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_TUNER_DEV_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_OSD_STRING_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_OSD_STRING_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_USER_CTRL_OPERAND_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_USER_CTRL_OPERAND_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_USER_CTRL_WITH_OPERAND_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_USER_CTRL_WITH_OPERAND_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_PWR_ON_DEV_STS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_PWR_ON_DEV_STS_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_FEATURE_ABORT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_FEATURE_ABORT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_SYS_AUD_CTRL_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_SYS_AUD_CTRL_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_AUD_DESCRIPTOR_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_AUD_DESCRIPTOR_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_AUD_FMT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_AUD_FMT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_DD_VNDR_CMD_PSR_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_DD_VNDR_CMD_PSR_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_DD_GET_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_DD_GET_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_DD_VNDR_CMD_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_DD_VNDR_CMD_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_DD_VNDR_CMD_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_DD_VNDR_CMD_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_DEV_DISCOVERY_CONFIG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_DEV_DISCOVERY_CONFIG_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_DEV_VNDR_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_DEV_VNDR_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_DEV_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_DEV_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CECM_NFY_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CECM_NFY_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SATL_REC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SATL_REC_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SATL_MEM_DUMP_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SATL_MEM_DUMP_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (SATL_MEM_DUMP_T, pv_data)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_LST_MNGR_CLEAN_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (LST_MNGR_CLEAN_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_AUD_TYPE_VOL_GAIN_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (AUD_TYPE_VOL_GAIN_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_AUD_TYPE_AVL_GAIN_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (AUD_TYPE_AVL_GAIN_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_AUD_TYPE_SPECTRUM_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (AUD_TYPE_SPECTRUM_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_AUD_TYPE_TEST_TONE =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (AUD_TYPE_TEST_TONE),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SMART_AUD_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SMART_AUD_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SMART_AUD_GROUP_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SMART_AUD_GROUP_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_DRV_CUSTOM_AUD_VOL_CURVE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DRV_CUSTOM_AUD_VOL_CURVE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_AUD_TYPE_DD_CONTROL_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (AUD_TYPE_DD_CONTROL_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_DRV_CUSTOM_AUD_TYPE_AQ_VER_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DRV_CUSTOM_AUD_TYPE_AQ_VER_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_AUD_TYPE_BASS_BOOSTER =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (AUD_TYPE_BASS_BOOSTER),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_TUNER_TYPE_DIAG_CONTROL_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TUNER_TYPE_DIAG_CONTROL_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_TUNER_TYPE_DIAG_CONTENT_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TUNER_TYPE_DIAG_CONTENT_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_TUNER_TYPE_DIAG_AGC_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TUNER_TYPE_DIAG_AGC_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VID_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VID_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_GAMMA_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (GAMMA_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VID_NON_LINEAR_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VID_NON_LINEAR_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_OUTRGB_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (OUTRGB_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SMART_PIC_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SMART_PIC_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_SMART_PIC_GROUP_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (SMART_PIC_GROUP_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_PIC_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PIC_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_PQ_DEMO_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PQ_DEMO_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_DRV_CUSTOM_GAMMA_VALUE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DRV_CUSTOM_GAMMA_VALUE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_DRV_CUSTOM_GAMMA_DELTA_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DRV_CUSTOM_GAMMA_DELTA_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_DRV_CUSTOM_PATTERN_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DRV_CUSTOM_PATTERN_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_DRV_GAMMA_BUF_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DRV_GAMMA_BUF_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_DRV_GAMMA_GETBUF_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DRV_GAMMA_GETBUF_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_DRV_CUSTOM_SCE_PATTERN_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DRV_CUSTOM_SCE_PATTERN_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_DRV_CUSTOM_DBC_DEBUG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DRV_CUSTOM_DBC_DEBUG_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_AUTO_BACKLIGHT_SETS_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (AUTO_BACKLIGHT_SETS_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_AUTO_COLORTEMP_SETS_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (AUTO_COLORTEMP_SETS_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_AUTO_COLORTEMP_GETS_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (AUTO_COLORTEMP_GETS_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VID_TYPE_DOT_TO_DOT_PATTERN_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VID_TYPE_DOT_TO_DOT_PATTERN_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_LVDS_SPREAD_SPECTRUM_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (LVDS_SPREAD_SPECTRUM_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_VID_TYPE_PANEL_RESOLUTION_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VID_TYPE_PANEL_RESOLUTION_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_EXTMJC_TYPE_SET_OSD_MODE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (EXTMJC_TYPE_SET_OSD_MODE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MISC_TYPE_TUNER_SIF_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MISC_TYPE_TUNER_SIF_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MISC_TYPE_HDMI_EDID_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MISC_TYPE_HDMI_EDID_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_EDID_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (EDID_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MISC_TYPE_SIF_ADVANCED_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MISC_TYPE_SIF_ADVANCED_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MISC_TYPE_SIF_ADVANCED_T, pui1_data)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_PANEL_SELECTION_CONFIG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PANEL_SELECTION_CONFIG_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_DRV_CUST_DRAM_CFG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DRV_CUST_DRAM_CFG_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_DRV_CUSTOM_MISC_CRNT_INP_SRC_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DRV_CUSTOM_MISC_CRNT_INP_SRC_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MISC_TYPE_HDMI_HDCP_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MISC_TYPE_HDMI_HDCP_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MISC_TYPE_VGA_EDID_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MISC_TYPE_VGA_EDID_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_DRV_CUSTOM_MISC_LOADER_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DRV_CUSTOM_MISC_LOADER_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MISC_TYPE_LED_DISPLAY_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MISC_TYPE_LED_DISPLAY_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MISC_TYPE_LED_NOTIFY_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MISC_TYPE_LED_NOTIFY_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MISC_TYPE_NAND_FLASH_WRITE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MISC_TYPE_NAND_FLASH_WRITE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MISC_TYPE_NAND_FLASH_BULK_WRITE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MISC_TYPE_NAND_FLASH_BULK_WRITE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_MISC_TYPE_NOR_FLASH_BULK_WRITE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MISC_TYPE_NOR_FLASH_BULK_WRITE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_AUD_TYPE_SET_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (AUD_TYPE_SET_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (AUD_TYPE_SET_INFO_T, pv_set_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_AUD_TYPE_GET_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (AUD_TYPE_GET_INFO_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (AUD_TYPE_GET_INFO_T, pv_get_info)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (AUD_TYPE_GET_INFO_T, pz_size)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_AUD_TYPE_GET_MIN_MAX_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (AUD_TYPE_GET_MIN_MAX_INFO_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (AUD_TYPE_GET_MIN_MAX_INFO_T, pi2_min)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (AUD_TYPE_GET_MIN_MAX_INFO_T, pi2_max)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_TUNER_TYPE_SET_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TUNER_TYPE_SET_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (TUNER_TYPE_SET_INFO_T, pv_set_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_TUNER_TYPE_GET_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (TUNER_TYPE_GET_INFO_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (TUNER_TYPE_GET_INFO_T, pv_get_info)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (TUNER_TYPE_GET_INFO_T, pz_size)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_VID_TYPE_SET_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VID_TYPE_SET_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (VID_TYPE_SET_INFO_T, pv_set_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_VID_TYPE_GET_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VID_TYPE_GET_INFO_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (VID_TYPE_GET_INFO_T, pv_get_info)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (VID_TYPE_GET_INFO_T, pz_size)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_VID_TYPE_GET_GAMMA_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VID_TYPE_GET_GAMMA_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_GAMMA_INFO_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (VID_TYPE_GET_GAMMA_INFO_T, pu1GammaTbl)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_VID_TYPE_GET_MIN_MAX_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VID_TYPE_GET_MIN_MAX_INFO_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (VID_TYPE_GET_MIN_MAX_INFO_T, pui2_min)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (VID_TYPE_GET_MIN_MAX_INFO_T, pui2_max)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_VID_DEC_TYPE_SET_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VID_DEC_TYPE_SET_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (VID_DEC_TYPE_SET_INFO_T, pv_set_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_VID_DEC_TYPE_GET_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (VID_DEC_TYPE_GET_INFO_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (VID_DEC_TYPE_GET_INFO_T, pv_get_info)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (VID_DEC_TYPE_GET_INFO_T, pz_size)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_EXTMJC_TYPE_SET_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (EXTMJC_TYPE_SET_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (EXTMJC_TYPE_SET_INFO_T, pv_set_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_EXTMJC_TYPE_GET_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (EXTMJC_TYPE_GET_INFO_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (EXTMJC_TYPE_GET_INFO_T, pv_get_info)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (EXTMJC_TYPE_GET_INFO_T, pz_size)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MISC_TYPE_SET_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MISC_TYPE_SET_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MISC_TYPE_SET_INFO_T, pv_set_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MISC_TYPE_GET_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MISC_TYPE_GET_INFO_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MISC_TYPE_GET_INFO_T, pv_get_info)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MISC_TYPE_GET_INFO_T, pz_size)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_MISC_TYPE_GET_MIN_MAX_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (MISC_TYPE_GET_MIN_MAX_INFO_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MISC_TYPE_GET_MIN_MAX_INFO_T, pui2_min)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (MISC_TYPE_GET_MIN_MAX_INFO_T, pui2_max)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_CUST_SPEC_TYPE_SET_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CUST_SPEC_TYPE_SET_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (CUST_SPEC_TYPE_SET_INFO_T, pv_set_info)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_CUST_SPEC_TYPE_GET_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CUST_SPEC_TYPE_GET_INFO_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (CUST_SPEC_TYPE_GET_INFO_T, pv_get_info)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (CUST_SPEC_TYPE_GET_INFO_T, pz_size)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_DRV_CUST_OPERATION_INFO_T_u =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((DRV_CUST_OPERATION_INFO_T*) NULL)->u),
    .ui4_num_entries = 18,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_AUD_TYPE_SET_INFO_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_aud_set_info"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_AUD_TYPE_GET_INFO_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_aud_get_info"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_AUD_TYPE_GET_MIN_MAX_INFO_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_aud_get_min_max_info"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_TUNER_TYPE_SET_INFO_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_tuner_set_info"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_TUNER_TYPE_GET_INFO_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_tuner_get_info"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_VID_TYPE_SET_INFO_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_vid_set_info"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_VID_TYPE_GET_INFO_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_vid_get_info"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_VID_TYPE_GET_GAMMA_INFO_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_vid_get_gamma_info"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_VID_TYPE_GET_MIN_MAX_INFO_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_vid_get_min_max_info"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_VID_DEC_TYPE_SET_INFO_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_vid_dec_set_info"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_VID_DEC_TYPE_GET_INFO_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_vid_dec_get_info"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_EXTMJC_TYPE_SET_INFO_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_extmjc_set_info"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_EXTMJC_TYPE_GET_INFO_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_extmjc_get_info"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MISC_TYPE_SET_INFO_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_misc_set_info"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MISC_TYPE_GET_INFO_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_misc_get_info"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_MISC_TYPE_GET_MIN_MAX_INFO_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_misc_get_min_max_info"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_CUST_SPEC_TYPE_SET_INFO_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_cust_spec_set_info"
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_CUST_SPEC_TYPE_GET_INFO_T,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.t_cust_spec_get_info"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_DRV_CUST_OPERATION_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (DRV_CUST_OPERATION_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_DRV_CUST_OPERATION_INFO_T_u,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (DRV_CUST_OPERATION_INFO_T, u)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_PSI_TS_VER_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PSI_TS_VER_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_PSI_PID_SVC_ID_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PSI_PID_SVC_ID_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_PSI_STREAM_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PSI_STREAM_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_PSI_CONFIG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (PSI_CONFIG_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CAP_CAPTURE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CAP_CAPTURE_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CAP_CAPABILITY_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CAP_CAPABILITY_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CAP_LOGO_SAVE_INFO_T_u =
{
    .e_type          = ARG_TYPE_UNION,
    .z_size          = sizeof (((CAP_LOGO_SAVE_INFO_T*) NULL)->u),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .ps_field_name = "u.ps_path"
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_CAP_LOGO_SAVE_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CAP_LOGO_SAVE_INFO_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_CAP_LOGO_SAVE_INFO_T_u,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (CAP_LOGO_SAVE_INFO_T, u)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_CAP_OBJ_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CAP_OBJ_T),
    .ui4_num_entries = 5,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC | ARG_DIR_INP,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (CAP_OBJ_T, pv_nfy_tag)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC | ARG_DIR_INP,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (CAP_OBJ_T, pv_cust_tag)
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_CAP_LOGO_SAVE_INFO_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (CAP_OBJ_T, t_save_info)
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_CAP_INPUT_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (CAP_OBJ_T, t_cap_input)
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_CAP_OUTPUT_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (CAP_OBJ_T, t_cap_output)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_CI_MMI_MENU_ITEM_LIST_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CI_MMI_MENU_ITEM_LIST_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_CI_MMI_MENU_ITEM_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (CI_MMI_MENU_ITEM_LIST_T, pt_head)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_CI_MMI_ENQ_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CI_MMI_ENQ_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (CI_MMI_ENQ_T, w2s_text)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_CI_MMI_ENQ_ANS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CI_MMI_ENQ_ANS_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (CI_MMI_ENQ_ANS_T, w2s_text)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_CI_MMI_ENQ_CLOSE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CI_MMI_ENQ_CLOSE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CI_MMI_MENU_ITEM_T_t_link =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (((CI_MMI_MENU_ITEM_T*) NULL)->t_link),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_CI_MMI_MENU_ITEM_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (CI_MMI_MENU_ITEM_T, t_link.pt_next)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = &t_rpc_decl_CI_MMI_MENU_ITEM_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (CI_MMI_MENU_ITEM_T, t_link.pt_prev)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_CI_MMI_MENU_ITEM_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CI_MMI_MENU_ITEM_T),
    .ui4_num_entries = 2,
    {
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_CI_MMI_MENU_ITEM_T_t_link,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (CI_MMI_MENU_ITEM_T, t_link)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (CI_MMI_MENU_ITEM_T, w2s_item)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_CI_MMI_MENU_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CI_MMI_MENU_T),
    .ui4_num_entries = 4,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (CI_MMI_MENU_T, w2s_title)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (CI_MMI_MENU_T, w2s_subtitle)
            }
        },
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (CI_MMI_MENU_T, w2s_bottom)
            }
        },
        {
            .e_type          = ARG_TYPE_DESC,
            .pt_desc         = &t_rpc_decl_CI_MMI_MENU_ITEM_LIST_T,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (CI_MMI_MENU_T, t_list)
            }
        }
    }
};

static const RPC_DESC_T t_rpc_decl_CI_MMI_MENU_ANS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CI_MMI_MENU_ANS_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CI_SLOT_INFO_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CI_SLOT_INFO_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CI_HC_TUNE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CI_HC_TUNE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CI_HC_REPLACE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CI_HC_REPLACE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CI_HC_CLR_REPLACE_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CI_HC_CLR_REPLACE_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CI_CA_SYSTEM_ID_MATCH_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CI_CA_SYSTEM_ID_MATCH_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CI_CA_SYSTEM_ID_STATUS_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CI_CA_SYSTEM_ID_STATUS_T),
    .ui4_num_entries = 0
};

static const RPC_DESC_T t_rpc_decl_CI_CONFIG_T =
{
    .e_type          = ARG_TYPE_STRUCT,
    .z_size          = sizeof (CI_CONFIG_T),
    .ui4_num_entries = 1,
    {
        {
            .e_type          = ARG_TYPE_REF_DESC,
            .pt_desc         = NULL,
            .ui4_num_entries = 1,

            {
                .z_offs = offsetof (CI_CONFIG_T, at_src_type)
            }
        }
    }
};


static const RPC_DESC_T* at_rpc_desc_list [] =
{
    &t_rpc_decl_STYLE_T,
    &t_rpc_decl_STYLE_STR_T,
    &t_rpc_decl_STREAM_COMP_ID_T,
    &t_rpc_decl_THREAD_DESCR_T,
    &t_rpc_decl_TS_DESCR_T,
    &t_rpc_decl_ARG_DESC_T_u,
    &t_rpc_decl_ARG_DESC_T,
    &t_rpc_decl_RPC_DESC_ENTRY_T_u,
    &t_rpc_decl_RPC_DESC_ENTRY_T,
    &t_rpc_decl_RPC_DESC_T,
    &t_rpc_decl_OS_FNCT_T,
    &t_rpc_decl_CM_COMMAND_T_u,
    &t_rpc_decl_CM_COMMAND_T,
    &t_rpc_decl_CM_CTRL_RM_SRC_COMP_T,
    &t_rpc_decl_TUNER_INTERLEAVING_T,
    &t_rpc_decl_TUNER_DVBT2_PLP_LIST_INFO,
    &t_rpc_decl_TUNER_LNB_INFO_T,
    &t_rpc_decl_TUNER_DISEQC_INFO_T,
    &t_rpc_decl_TUNER_CAB_ANA_TUNE_INFO_T,
    &t_rpc_decl_TUNER_CAB_DIG_TUNE_INFO_T,
    &t_rpc_decl_TUNER_CAB_DIG_OOB_TX_TUNE_INFO_T,
    &t_rpc_decl_TUNER_CAB_DIG_OOB_RX_TUNE_INFO_T,
    &t_rpc_decl_TUNER_SAT_ANA_TUNE_INFO_T,
    &t_rpc_decl_TUNER_SAT_DIG_TUNE_INFO_T,
    &t_rpc_decl_TUNER_TER_ANA_TUNE_INFO_T,
    &t_rpc_decl_TUNER_TER_DIG_TUNE_INFO_T,
    &t_rpc_decl_DB_FIELD_INFO_T,
    &t_rpc_decl_DB_SEARCH_T,
    &t_rpc_decl_MEDIA_SUBTYPE_T,
    &t_rpc_decl_MEDIA_FORMAT_T,
    &t_rpc_decl_MM_PLAY_MODE_PL_T,
    &t_rpc_decl_DRM_OUTPUT_SIGNAL_PROTECTION_INFO_T,
    &t_rpc_decl_DIVX_DRM_INIT_CONFIG_T,
    &t_rpc_decl_DIVX_DRM_REGISTRATION_INFO_T,
    &t_rpc_decl_DIVX_DRM_DEACTIVATION_INFO_T,
    &t_rpc_decl_DIVX_DRM_UI_HELP_INFO_T,
    &t_rpc_decl_DIVX_DRM_MEMORY_RW_FUNC_INFO_T,
    &t_rpc_decl_DIVX_DRM_BASIC_INFO_T,
    &t_rpc_decl_DIVX_DRM_UNION_GENERIC_INFO_T,
    &t_rpc_decl_MINFO_MPEG_2_STREAM_ID,
    &t_rpc_decl_MINFO_MP3_AUDIO_STM_ATRBT_T,
    &t_rpc_decl_MINFO_FLV_AUDIO_STM_ATRBT_T,
    &t_rpc_decl_MINFO_FLV_VIDEO_STM_ATRBT_T,
    &t_rpc_decl_MINFO_CMPB_VIDEO_STM_ATRBT_T,
    &t_rpc_decl_MINFO_CMPB_AUDIO_STM_ATRBT_T,
    &t_rpc_decl_MINFO_CMPB_SUBTL_STM_ATRBT_T,
    &t_rpc_decl_MINFO_RM_VIDEO_STM_ATRBT_T,
    &t_rpc_decl_MINFO_RM_AUDIO_STM_ATRBT_T,
    &t_rpc_decl_MINFO_AIF_AUDIO_STM_ATRBT_T,
    &t_rpc_decl_MINFO_AVI_AUDIO_STM_ATRBT_T,
    &t_rpc_decl_MINFO_AVI_VIDEO_CODEC_XXDB_T,
    &t_rpc_decl_MINFO_AVI_VIDEO_CODEC_INFO_T,
    &t_rpc_decl_MINFO_AVI_VIDEO_STM_ATRBT_T,
    &t_rpc_decl_MINFO_AVI_DIVX_SUBTITLE_STM_ATRBT_T,
    &t_rpc_decl_MINFO_ASF_AUDIO_STM_ATRBT_T,
    &t_rpc_decl_MINFO_ASF_VIDEO_STM_ATRBT_T,
    &t_rpc_decl_MINFO_MP4_AUDIO_STM_ATRBT_T,
    &t_rpc_decl_MINFO_MP4_VIDEO_STM_ATRBT_T,
    &t_rpc_decl_MINFO_MP4_SUBTITLE_STM_ATRBT_T,
    &t_rpc_decl_MINFO_MKV_TM_SCALE_FLOAT_T,
    &t_rpc_decl_MINFO_MKV_CONTENT_ENCODING_T,
    &t_rpc_decl_MINFO_MKV_AUDIO_STM_ATRBT_T,
    &t_rpc_decl_MINFO_MKV_VIDEO_STM_ATRBT_T,
    &t_rpc_decl_MINFO_MKV_SUBTITLE_STM_ATRBT_T,
    &t_rpc_decl_FONT_INFO_T,
    &t_rpc_decl_MINFO_PS_AUDIO_STM_ATRBT_T,
    &t_rpc_decl_MINFO_PS_VIDEO_STM_ATRBT_T,
    &t_rpc_decl_MINFO_PS_SUBTITLE_STM_ATRBT_T,
    &t_rpc_decl_MINFO_EXT_SUBTITLE_STM_ATRBT_T,
    &t_rpc_decl_MINFO_ES_VIDEO_STM_ATRBT_T,
    &t_rpc_decl_MINFO_ES_AUDIO_STM_ATRBT_T,
    &t_rpc_decl_OGG_STREAM_VORBIS_INFO_T,
    &t_rpc_decl_OGG_STREAM_AUDIO_INFO_T,
    &t_rpc_decl_MINFO_OGG_VIDEO_STM_ATRBT_T,
    &t_rpc_decl_MINFO_OGG_TEXT_STM_ATRBT_T,
    &t_rpc_decl_MINFO_OGG_AUDIO_VORBIS_SPEC_T,
    &t_rpc_decl_MINFO_OGG_AUDIO_STM_ATRBT_T,
    &t_rpc_decl_MINFO_TS_RD3D_REG_DESC_T,
    &t_rpc_decl_MINFO_MVC_EXTENSION_DESC_T,
    &t_rpc_decl_MINFO_TS_VIDEO_STM_ATRBT_T,
    &t_rpc_decl_MINFO_TS_AUDIO_STM_ATRBT_T,
    &t_rpc_decl_MINFO_TS_SBTL_STM_ATRBT_T,
    &t_rpc_decl_MINFO_STM_ID_T,
    &t_rpc_decl_MINFO_TYPE_STM_ATRBT_T_u,
    &t_rpc_decl_MINFO_TYPE_STM_ATRBT_T,
    &t_rpc_decl_MINFO_INFO_PIC_T,
    &t_rpc_decl_MINFO_ID3_RAW_DATA_T,
    &t_rpc_decl_MINFO_TYPE_ID3_TAG_T,
    &t_rpc_decl_MINFO_TITLE_INFO_T,
    &t_rpc_decl_MINFO_PLAYLIST_INFO_T,
    &t_rpc_decl_MINFO_CHAP_INFO_T,
    &t_rpc_decl_MINFO_DISP_INFO_T,
    &t_rpc_decl_MINFO_TYPE_GENERIC_T,
    &t_rpc_decl_MM_TM_T,
    &t_rpc_decl_MINFO_TYPE_FRAME_RATE_T,
    &t_rpc_decl_MINFO_TYPE_VIDEO_ES_INFO_T,
    &t_rpc_decl_MINFO_TYPE_AUDIO_ES_INFO_T,
    &t_rpc_decl_MINFO_TYPE_META_DATA_T,
    &t_rpc_decl_MP4_SINGLE_FILE_INFO_T,
    &t_rpc_decl_MP4_MULTI_FILE_INFO_T,
    &t_rpc_decl_MP4_MINFO_EXTRA_INFO_T,
    &t_rpc_decl_MINFO_TYPE_IDX_INFO_T,
    &t_rpc_decl_MINFO_MP4_TRAK_ENCRYTION_INFO_T,
    &t_rpc_decl_MINFO_MP4_ENCRYTION_INFO_T_u,
    &t_rpc_decl_MINFO_MP4_ENCRYTION_INFO_T,
    &t_rpc_decl_MINFO_TYPE_ENCRYPTION_INFO_T_u,
    &t_rpc_decl_MINFO_TYPE_ENCRYPTION_INFO_T,
    &t_rpc_decl_MINFO_DIVX_DRM_OBJ_T,
    &t_rpc_decl_MINFO_TYPE_DRM_INFO_T_u,
    &t_rpc_decl_MINFO_TYPE_DRM_INFO_T,
    &t_rpc_decl_MINFO_TYPE_FONT_INFO_T,
    &t_rpc_decl_MINFO_TYPE_DIVX_INFO_T,
    &t_rpc_decl_MINFO_INFO_T_u,
    &t_rpc_decl_MINFO_INFO_T,
    &t_rpc_decl_MINFO_TYPE_RAW_DATA_T,
    &t_rpc_decl_SCDB_MPEG_COMMON_T,
    &t_rpc_decl_SCDB_COMMON_T,
    &t_rpc_decl_SCDB_AUDIO_MPEG_T,
    &t_rpc_decl_AVC_VID_DESC_T,
    &t_rpc_decl_RD3D_REG_DESC_T,
    &t_rpc_decl_MVC_EXTENSION_DESC_T,
    &t_rpc_decl_SCDB_VIDEO_MPEG_T,
    &t_rpc_decl_SCDB_PRIVATE_MPEG_T,
    &t_rpc_decl_SCDB_DSMCC_MPEG_T,
    &t_rpc_decl_CC_ATSC_LINE_21_T,
    &t_rpc_decl_CC_ATSC_DTV_T,
    &t_rpc_decl_SCDB_CLOSED_CAPTION_T,
    &t_rpc_decl_SCDB_ISDB_CAPTION_T,
    &t_rpc_decl_SCDB_ISDB_TEXT_T,
    &t_rpc_decl_SCDB_SUBTITLE_DVB_T,
    &t_rpc_decl_SCDB_TELETEXT_DVB_T,
    &t_rpc_decl_SCDB_VBI_TELETEXT_DVB_T,
    &t_rpc_decl_SCDB_VBI_DATA_DVB_T,
    &t_rpc_decl_SCDB_AUDIO_ANALOG_T,
    &t_rpc_decl_SCDB_VIDEO_ANALOG_T,
    &t_rpc_decl_SCDB_AUDIO_DIGITAL_T,
    &t_rpc_decl_SCDB_AUDIO_AVI_T,
    &t_rpc_decl_SCDB_VIDEO_AVI_T,
    &t_rpc_decl_SCDB_SUBTITLE_DIVX_T,
    &t_rpc_decl_SCDB_AUDIO_MP4_T,
    &t_rpc_decl_SCDB_SUBTITLE_MP4_T,
    &t_rpc_decl_SCDB_VIDEO_MP4_T,
    &t_rpc_decl_SCDB_AUDIO_ASF_T,
    &t_rpc_decl_SCDB_VIDEO_ASF_T,
    &t_rpc_decl_SCDB_AUDIO_PS_T,
    &t_rpc_decl_SCDB_VIDEO_PS_T,
    &t_rpc_decl_SCDB_VIDEO_MKV_T,
    &t_rpc_decl_SCDB_AUDIO_MKV_T,
    &t_rpc_decl_SCDB_VIDEO_RM_T,
    &t_rpc_decl_SCDB_AUDIO_RM_T,
    &t_rpc_decl_SCDB_AUDIO_FLV_T,
    &t_rpc_decl_SCDB_VIDEO_FLV_T,
    &t_rpc_decl_SCDB_SUBTITLE_MKV_T,
    &t_rpc_decl_SCDB_AUDIO_MP3_T,
    &t_rpc_decl_SCDB_VIDEO_ES_T,
    &t_rpc_decl_SCDB_AUDIO_ES_T,
    &t_rpc_decl_SCDB_SUBTITLE_PS_T,
    &t_rpc_decl_SCDB_VIDEO_OGG_T,
    &t_rpc_decl_SCDB_AUDIO_VORBIS_OGG_T,
    &t_rpc_decl_SCDB_AUDIO_NORMAL_OGG_T,
    &t_rpc_decl_SCDB_AUDIO_OGG_T,
    &t_rpc_decl_SCDB_EXT_SUBTITLE_T,
    &t_rpc_decl_SCDB_REC_T_u,
    &t_rpc_decl_SCDB_REC_T,
    &t_rpc_decl_DISEQC_10_SET_T,
    &t_rpc_decl_BRDCST_CTRL_DISEQC_T,
    &t_rpc_decl_BRDCST_CTRL_TUNE_ATTRIBUTES_T,
    &t_rpc_decl_BRDCST_CTRL_AUDIO_STREAM_FMT_T,
    &t_rpc_decl_BRDCST_CTRL_AUDIO_ENC_T,
    &t_rpc_decl_BRDCST_TIME_OUT_T,
    &t_rpc_decl_SM_COMMAND_T_u,
    &t_rpc_decl_SM_COMMAND_T,
    &t_rpc_decl_SM_OBSVR_FILTER_T,
    &t_rpc_decl_SM_VSH_VBI_INFO_T,
    &t_rpc_decl_SM_VSH_VBI_NFY_T,
    &t_rpc_decl_SM_VSH_START_RENDER_INFO_T,
    &t_rpc_decl_SM_VSH_VBI_REF_INFO_T_u,
    &t_rpc_decl_SM_VSH_VBI_REF_INFO_T,
    &t_rpc_decl_VSH_CAPTURE_INFO_T,
    &t_rpc_decl_VSH_REGION_INFO_T,
    &t_rpc_decl_VSH_REGION_CAPABILITY_INFO_T,
    &t_rpc_decl_VSH_ENCHANCE_INFO_T,
    &t_rpc_decl_VSH_SET_PLANE_ORDER_INFO_T,
    &t_rpc_decl_VSH_GET_PLANE_ORDER_INFO_T,
    &t_rpc_decl_VSH_QV_ROTATE_T,
    &t_rpc_decl_VSH_QV_CTRL_INFO_T,
    &t_rpc_decl_VSH_HDMI_AVI_INFOFRAME_INFO_T,
    &t_rpc_decl_VSH_SRC_RESOLUTION_INFO_T,
    &t_rpc_decl_VSH_VID_MIN_MAX_INFO_T,
    &t_rpc_decl_VSH_CNT_ADV_INFO_T,
    &t_rpc_decl_VSH_VGA_TIMING_OPTION_T,
    &t_rpc_decl_VSH_AMBIG_VGA_TIMING_OPTION_T,
    &t_rpc_decl_VSH_AMBIG_VGA_TIMING_INFO_T,
    &t_rpc_decl_VSH_SEQUENCE_DATA_T,
    &t_rpc_decl_VSH_SEQUENCE_RAW_T,
    &t_rpc_decl_VSH_SEQUENCE_INFO_T_u,
    &t_rpc_decl_VSH_SEQUENCE_INFO_T,
    &t_rpc_decl_VSH_TIME_CODE_INFO_T,
    &t_rpc_decl_VSH_PICTURE_INFO_T,
    &t_rpc_decl_VSH_THUMBNAIL_INFO_T,
    &t_rpc_decl_THUMBNAIL_GET_INFO_T,
    &t_rpc_decl_ASH_VOLUME_INFO_T,
    &t_rpc_decl_ASH_AUDIO_INFO_T,
    &t_rpc_decl_ASH_AUD_SYS_INFO_T,
    &t_rpc_decl_ASH_AUDIO_WMA_INFO_T,
    &t_rpc_decl_ASH_AUDIO_AAC_INFO_T,
    &t_rpc_decl_ASH_AUDIO_PCM_INFO_T,
    &t_rpc_decl_ASH_AUDIO_COOK_INFO_T,
    &t_rpc_decl_ASH_ADUIO_VORBIS_INFO_T,
    &t_rpc_decl_ASH_ADUIO_AC3_INFO_T,
    &t_rpc_decl_ASH_AUDIO_CODEC_SPEC_INFO_T_u,
    &t_rpc_decl_ASH_AUDIO_CODEC_SPEC_INFO_T,
    &t_rpc_decl_SCC_BG_COLOR_T,
    &t_rpc_decl_SCC_VID_MODE_EX_T,
    &t_rpc_decl_SCC_AUD_DEC_LINEOUT_VOL_T,
    &t_rpc_decl_SCC_VID_OVER_SCAN_CLIPPER_T,
    &t_rpc_decl_SCC_VID_VDRIPS_BUFF_T,
    &t_rpc_decl_SCC_VID_IFRAME_BUFF_T,
    &t_rpc_decl_SCC_VID_REGION_T,
    &t_rpc_decl_SCC_VID_DEBLOCKING_INFO_T,
    &t_rpc_decl_SCC_VID_LB_DETECT_NFY_INFO_T,
    &t_rpc_decl_SCC_VID_LB_DETECT_CONFIG_T,
    &t_rpc_decl_SCC_VID_MJC_T,
    &t_rpc_decl_SCC_VID_SUPER_BLANK_T,
    &t_rpc_decl_SCC_VID_SUPER_FREEZE_T,
    &t_rpc_decl_SCC_ANGULAR_SCE_T,
    &t_rpc_decl_SCC_VID_DEC_CAPABILITY_INFO_T,
    &t_rpc_decl_SCC_VID_FREQ_T,
    &t_rpc_decl_SCC_VID_SRC_DET_STATUS_T,
    &t_rpc_decl_SCC_VID_SCR_MODE_T,
    &t_rpc_decl_SCC_DISP_RESOLUTION_T,
    &t_rpc_decl_SCC_DISP_SCREEN_RESOLUTION_T,
    &t_rpc_decl_SCC_DISP_COLOR_GAIN_T,
    &t_rpc_decl_SCC_DISP_RANGE_INFO_T,
    &t_rpc_decl_SCC_AUD_LOUD_SPEAKER_T,
    &t_rpc_decl_SCC_AUD_SE_TRIM_T,
    &t_rpc_decl_SCC_AUD_VOLUME_INFO_T,
    &t_rpc_decl_SCC_AUD_CHNL_DELAY_IND_T,
    &t_rpc_decl_SCC_AUD_CHNL_DELAY_T,
    &t_rpc_decl_SCC_AUD_SE_SURROUND_VALUE_T,
    &t_rpc_decl_SCC_AUD_SE_SURROUND_INFO_T,
    &t_rpc_decl_SCC_AUD_DEC_SET_TYPE_PEQ_INFO_T,
    &t_rpc_decl_SCC_AUD_SE_QA_USER_LVL_T,
    &t_rpc_decl_SCC_AUD_UPLOAD_DATA_NFT_INFO_T,
    &t_rpc_decl_SCC_AUD_HDPHONE_DETECT_NFT_INFO_T,
    &t_rpc_decl_SCC_AUD_MUTE_EX_INFO_T,
    &t_rpc_decl_SCC_AU_SE_QA_USER_BAND_INFO_T,
    &t_rpc_decl_SCC_AUD_CLIP_BUFF_T,
    &t_rpc_decl_SCC_AUD_CLIP_T_u,
    &t_rpc_decl_SCC_AUD_CLIP_T,
    &t_rpc_decl_SCC_AUD_CAPABILITY_T,
    &t_rpc_decl_SCC_AUD_UPLOAD_DATA_BUFFER_INFO_T,
    &t_rpc_decl_SCC_AUD_UPLOAD_PCM_INFO_T,
    &t_rpc_decl_SCC_AUD_KARAOKE_INFO_T,
    &t_rpc_decl_SVL_SIMPLE_DATE_TIME_T,
    &t_rpc_decl_SVL_ANALOG_T,
    &t_rpc_decl_SVL_ATSC_T,
    &t_rpc_decl_SVL_DVB_TS_SVC_ID_INFO_T,
    &t_rpc_decl_SVL_DVB_SBTL_PREF_T,
    &t_rpc_decl_SVL_DVB_T,
    &t_rpc_decl_SVL_ISDB_T,
    &t_rpc_decl_SVL_DTMB_T,
    &t_rpc_decl_SVL_FMRDO_T,
    &t_rpc_decl_SVL_REC_HEADER_T,
    &t_rpc_decl_SVL_REC_T,
    &t_rpc_decl_SVL_CLIENT_DATA_T,
    &t_rpc_decl_SVL_MULTI_LANG_T,
    &t_rpc_decl_SVL_MEM_DUMP_T,
    &t_rpc_decl_FREQ_INFO_T,
    &t_rpc_decl_ALTERNATE_TUNE_INFO_T,
    &t_rpc_decl_TSL_REC_REFERENCE_T,
    &t_rpc_decl_ISDB_TER_DIG_INFO_T,
    &t_rpc_decl_TSL_REC_T,
    &t_rpc_decl_TSL_MEM_DUMP_T,
    &t_rpc_decl_EVCTX_KEY_TYPE_MASK_T,
    &t_rpc_decl_EVCTX_COMMAND_T_u,
    &t_rpc_decl_EVCTX_COMMAND_T,
    &t_rpc_decl_EVCTX_OFFSET_T,
    &t_rpc_decl_EVCTX_BRIEF_INFO_T,
    &t_rpc_decl_EVCTX_EVT_CAT_T,
    &t_rpc_decl_EVCTX_EVT_CAT_LIST_T,
    &t_rpc_decl_EVCTX_RATING_T,
    &t_rpc_decl_EVCTX_RATING_LIST_T,
    &t_rpc_decl_EVCTX_CA_SYSTEM_ID_LIST_T,
    &t_rpc_decl_EVCTX_EVENT_LINKAGE_T,
    &t_rpc_decl_EVCTX_EVENT_LINKAGE_LIST_T,
    &t_rpc_decl_EVCTX_IDX_LANG_T,
    &t_rpc_decl_SM_CC_AUTO_SEL_T,
    &t_rpc_decl_SM_CC_FILTER_T,
    &t_rpc_decl_SM_CCH_NTSC_CC_VBI_PSR_CB_INFO_T,
    &t_rpc_decl_SM_CCH_DTVCC_COLOR_T,
    &t_rpc_decl_SM_CCH_DTVCC_ATTRIBS_INFO_T,
    &t_rpc_decl_SM_CCH_EXISTED_CC_IND_T,
    &t_rpc_decl_SM_CCH_CONFIG_T,
    &t_rpc_decl_SM_SBTLH_STREAM_INFO,
    &t_rpc_decl_SM_SBTLH_CONFIG_T,
    &t_rpc_decl_SM_SBTLH_RENDER_INFO_T,
    &t_rpc_decl_CP_INFO_T,
    &t_rpc_decl_CP_INFO_LIST_T,
    &t_rpc_decl_CA_ALG_INFO_T,
    &t_rpc_decl_CA_KEY_INFO_T,
    &t_rpc_decl_CA_SET_COMMAND_INFO_T_u_set_cmd_info,
    &t_rpc_decl_CA_SET_COMMAND_INFO_T,
    &t_rpc_decl_ANALOG_BRDCST_TIME_OUT_T,
    &t_rpc_decl_ANALOG_BRDCST_CTRL_SCART_INP_CAP_T,
    &t_rpc_decl_ANALOG_BRDCST_CTRL_SCART_BYPASS_T,
    &t_rpc_decl_ANALOG_BRDCST_CTRL_TV_AUD_SYS_MASK_T,
    &t_rpc_decl_ANALOG_BRDCST_CTRL_TUNE_ATTRIBUTES_T,
    &t_rpc_decl_SILENT_SELECT_INFO_T,
    &t_rpc_decl_CHANNEL_DECODE_DESC_T,
    &t_rpc_decl_SRC_SVL_INFO_T,
    &t_rpc_decl_SRC_FREQ_INFO_T,
    &t_rpc_decl_AVC_HANDLER_TIME_OUT_T,
    &t_rpc_decl_AVC_HANDLER_CTRL_SCART_INP_CAP_T,
    &t_rpc_decl_SRC_AVC_HINT_T,
    &t_rpc_decl_SRC_AVC_T,
    &t_rpc_decl_SRC_AVC_INFO_T,
    &t_rpc_decl_FE_INIT_DATA,
    &t_rpc_decl_FE_INIT_FILE,
    &t_rpc_decl_GL_SCREEN_STATUS_T,
    &t_rpc_decl_GL_PLANE_CAPS_T,
    &t_rpc_decl_GL_PLANE_CFG_T,
    &t_rpc_decl_GL_PLANE_STATUS_T,
    &t_rpc_decl_GL_POINT_T,
    &t_rpc_decl_GL_SIZE_T,
    &t_rpc_decl_GL_RECT_T,
    &t_rpc_decl_GL_MATRIX2D_T,
    &t_rpc_decl_GL_COLOR_T,
    &t_rpc_decl_GL_DISPLAY_OPT_T,
    &t_rpc_decl_GL_SURFACE_DESC_T,
    &t_rpc_decl_GL_SURF_INFO_T,
    &t_rpc_decl_GL_LOCK_INFO_T_at_subplane,
    &t_rpc_decl_GL_LOCK_INFO_T,
    &t_rpc_decl_GL_ARC_DESC_T,
    &t_rpc_decl_GL_GPU_CAPS_T,
    &t_rpc_decl_GL_BLT_EX_T,
    &t_rpc_decl_GL_DRV_UPDATE_INFO_T,
    &t_rpc_decl_GL_DRCS_INFO_T,
    &t_rpc_decl_GL_TTX_COLOR_T,
    &t_rpc_decl_GL_ISDB_CHAR_COLOR_T,
    &t_rpc_decl_GL_ISDB_GRID_T,
    &t_rpc_decl_IMG_GIF_FRM_RECT_T,
    &t_rpc_decl_VID_PLA_NFY_INFO_T,
    &t_rpc_decl_VID_PLA_DISP_DIMENSION_T,
    &t_rpc_decl_VID_PLA_INJECT_VDP_T,
    &t_rpc_decl_VID_PLA_BG_COLOR_T,
    &t_rpc_decl_VID_PLA_VID_REGION_T,
    &t_rpc_decl_VID_PLA_VID_REGION_CAPABILITY_T,
    &t_rpc_decl_VID_PLA_GET_ASPECT_RATIO_TYPE_T,
    &t_rpc_decl_VID_PLA_ENHANCE_T,
    &t_rpc_decl_VID_PLA_QV_INFO_T,
    &t_rpc_decl_VID_PLA_MIN_MAX_INFO_T,
    &t_rpc_decl_VID_PLA_COLOR_GAIN_T,
    &t_rpc_decl_VID_PLA_OVER_SCAN_CLIPPER_T,
    &t_rpc_decl_VID_PLA_MAX_INFO_T,
    &t_rpc_decl_VID_PLA_LB_DETECT_NFY_INFO_T,
    &t_rpc_decl_VID_PLA_LB_DETECT_CONFIG_T,
    &t_rpc_decl_VID_PLA_TIME_CODE_INFO_T,
    &t_rpc_decl_VID_PLA_PICTURE_INFO_T,
    &t_rpc_decl_VID_PLA_SUPER_FREEZE_T,
    &t_rpc_decl_VID_PLA_SUPER_BLANK_T,
    &t_rpc_decl_VID_PLA_ATOMIC_CMD_T_u,
    &t_rpc_decl_VID_PLA_ATOMIC_CMD_T,
    &t_rpc_decl_VID_PLA_FORCE_UNMUTE_T,
    &t_rpc_decl_CAP_INPUT_T,
    &t_rpc_decl_VID_PLA_ANGULAR_SCE_T,
    &t_rpc_decl_EXIF_RATIONAL_DATA,
    &t_rpc_decl_EXIF_SRATIONAL_DATA,
    &t_rpc_decl_EXIF_INFO_T,
    &t_rpc_decl_EXIF_CONTEXT_DATA,
    &t_rpc_decl_IMG_ENG_DATA_T,
    &t_rpc_decl_IMG_ANIMATE_T,
    &t_rpc_decl_IMG_TYPE_DATA_T,
    &t_rpc_decl_IMG_ENG_TYPE_DATA_T,
    &t_rpc_decl_IMG_CAPS_T,
    &t_rpc_decl_IMG_NFY_LOAD_WITH_TAG_T,
    &t_rpc_decl_IMG_NFY_GIF_LOAD_PARAM_T,
    &t_rpc_decl_CAP_OUTPUT_T,
    &t_rpc_decl_IMAGE_FILE_INFO_T,
    &t_rpc_decl_IMAGE_PULL_FM_FUNC_T,
    &t_rpc_decl_WGL_ANIM_CONFIG_T,
    &t_rpc_decl_WGL_CANVAS_CFG_T,
    &t_rpc_decl_WGL_CVS_CLR_CFG_T,
    &t_rpc_decl_WGL_CANVAS_INFO_T,
    &t_rpc_decl_WGL_NOTIFY_DATA_T,
    &t_rpc_decl_WGL_CREATE_INFO_T,
    &t_rpc_decl_WGL_PACKED_PARAM_T,
    &t_rpc_decl_WGL_KEY_LINK_T,
    &t_rpc_decl_WGL_IMG_DECODE_FUNCTION_TABLE_T,
    &t_rpc_decl_WGL_COLOR_BASIC_SET_T,
    &t_rpc_decl_WGL_COLOR_STD_SET_T,
    &t_rpc_decl_WGL_COLOR_EXT_SET_T,
    &t_rpc_decl_WGL_COLOR_INFO_T,
    &t_rpc_decl_WGL_IMG_BASIC_SET_T,
    &t_rpc_decl_WGL_IMG_STD_SET_T,
    &t_rpc_decl_WGL_IMG_EXT_SET_T,
    &t_rpc_decl_WGL_IMG_INFO_T,
    &t_rpc_decl_WGL_FONT_INFO_T,
    &t_rpc_decl_WGL_FONT_BASIC_SET_T,
    &t_rpc_decl_WGL_FONT_INFO_EX_T,
    &t_rpc_decl_WGL_INSET_T,
    &t_rpc_decl_WGL_TEXT_ATTR_T,
    &t_rpc_decl_WGL_UPD_REGION_T,
    &t_rpc_decl_WGL_SBTL_REGION_T,
    &t_rpc_decl_WGL_SBTL_INIT_T,
    &t_rpc_decl_WGL_SBTL_LOGI_SZ_T,
    &t_rpc_decl_SBTL_MKV_CONTENT_ENCODING_T,
    &t_rpc_decl_SBTL_MKV_CONTENT_ENCODINGS_T,
    &t_rpc_decl_SBTL_MKV_CODEC_PRIVATE_T,
    &t_rpc_decl_SBTL_MKV_SBTL_CMD_T,
    &t_rpc_decl_SBTL_DISPLAY_INFO_CMD_T,
    &t_rpc_decl_SBTL_LYRIC_INFO_T,
    &t_rpc_decl_MM_SBTL_DISP_PAGE_T,
    &t_rpc_decl_MM_SBTL_TM_OFST_INF,
    &t_rpc_decl_MM_SBTL_RECORD_INF,
    &t_rpc_decl_MM_SBTL_PTS_CB,
    &t_rpc_decl_SMAI_CC_INF_T,
    &t_rpc_decl_SAMI_STREAM_INFO_T,
    &t_rpc_decl_SBTL_LANG_DESC_T,
    &t_rpc_decl_MM_EXT_SBTL_INFO_u,
    &t_rpc_decl_MM_EXT_SBTL_INFO,
    &t_rpc_decl_MM_LYRIC_INFO_u,
    &t_rpc_decl_MM_LYRIC_INFO,
    &t_rpc_decl_MM_SVC_COMMAND_T_u,
    &t_rpc_decl_MM_SVC_COMMAND_T,
    &t_rpc_decl_DIVX_DRM_INFO_SVCTX_T,
    &t_rpc_decl_DRM_INFO_SVCTX_T_u,
    &t_rpc_decl_DRM_INFO_SVCTX_T,
    &t_rpc_decl_MM_SVC_SBTL_DISP_MODE_T,
    &t_rpc_decl_MM_SVC_SBTL_HILT_STL_T,
    &t_rpc_decl_MM_SVC_SBTL_TIME_OFST_T,
    &t_rpc_decl_MM_SVC_SBTL_FONT_ENC_T,
    &t_rpc_decl_MM_SVC_SBTL_DISP_ATTR_T,
    &t_rpc_decl_MM_SVC_SBTL_SHOW_HIDE_T,
    &t_rpc_decl_SVC_MM_SBTL_LYRIC_INFO_T,
    &t_rpc_decl_MM_VIDEO_INFO_T,
    &t_rpc_decl_MM_PUSH_BUF_INFO_T,
    &t_rpc_decl_MM_SVC_RENDER_INFO_T,
    &t_rpc_decl_MM_SVC_POS_INFO_T,
    &t_rpc_decl_MM_SVC_CUR_CHAP_ID_T,
    &t_rpc_decl_MM_SVC_DUR_INFO_T,
    &t_rpc_decl_MM_SVC_TIME_CODE_INFO_T,
    &t_rpc_decl_MM_SVC_PTS_INFO_T,
    &t_rpc_decl_MM_REPEAT_AB_INFO_T,
    &t_rpc_decl_MM_SVC_REPEAT_INFO_T,
    &t_rpc_decl_MM_SVC_TRICK_INFO_T,
    &t_rpc_decl_MM_SVC_CHAPTER_INFO_T,
    &t_rpc_decl_MM_SVC_TITLE_INFO_T,
    &t_rpc_decl_MM_SVC_BTN_INFO_T_u,
    &t_rpc_decl_MM_SVC_BTN_INFO_T,
    &t_rpc_decl_MM_SVC_BTN_ACTION_INFO_T_u,
    &t_rpc_decl_MM_SVC_BTN_ACTION_INFO_T,
    &t_rpc_decl_MM_SVC_BUFFCTRL_INFO_T,
    &t_rpc_decl_MM_SVC_TIME_TO_SIZE_T,
    &t_rpc_decl_MM_SVC_BUF_STATUS_T,
    &t_rpc_decl_MM_DRM_CTRL_INFO_T,
    &t_rpc_decl_MM_MLN_DRM_INFO_T,
    &t_rpc_decl_MM_DRM_INFO_T_u,
    &t_rpc_decl_MM_DRM_INFO_T,
    &t_rpc_decl_SRC_MM_INFO_T,
    &t_rpc_decl_MM_TIMESHIFT_RANGE_INFO_T,
    &t_rpc_decl_MM_STRM_ATRBT_T,
    &t_rpc_decl_MM_STRM_INFO_T,
    &t_rpc_decl_MM_3D_VID_INFO_T,
    &t_rpc_decl_MM_STRM_CODEC_INFO_T,
    &t_rpc_decl_MM_BUF_AGT_FLOW_CTRL_T,
    &t_rpc_decl_MM_SVC_STOP_INFO,
    &t_rpc_decl_ID_CVT_CNDTN_T,
    &t_rpc_decl_SVCTX_MHEG_SCREEN_MODE_T,
    &t_rpc_decl_SVCTX_ANA_AUTO_DETECTED_AUD_SYS_T,
    &t_rpc_decl_SVCTX_SCDB_SORT_PARAM_T,
    &t_rpc_decl_SVCTX_ATOMIC_CMD_T,
    &t_rpc_decl_SVCTX_VID_STREAM_T,
    &t_rpc_decl_SVCTX_AUD_STREAM_T,
    &t_rpc_decl_SVCTX_STREAM_INFO_T,
    &t_rpc_decl_SVCTX_VIDEO_MUTE_INFO_T,
    &t_rpc_decl_SVCTX_GET_SCDB_NUM_RECS_T,
    &t_rpc_decl_SVCTX_GET_SCDB_REC_BY_IDX_T,
    &t_rpc_decl_SVCTX_GET_USING_SCDB_REC_T,
    &t_rpc_decl_SVCTX_COPY_PROTECTION_T,
    &t_rpc_decl_SVCTX_CA_INFO_T,
    &t_rpc_decl_SIGNAL_LOSS_DELAY_T,
    &t_rpc_decl_SCART_OUT_DESC_T,
    &t_rpc_decl_SCART_OUT_INFO_T,
    &t_rpc_decl_CH_BLOCK_BIAS_FREQ_T,
    &t_rpc_decl_RIKS_TV_T,
    &t_rpc_decl_SRC_DESC_T_u_info,
    &t_rpc_decl_SRC_DESC_T,
    &t_rpc_decl_STRM_CC_DESC_T,
    &t_rpc_decl_STRM_SUBTITLE_DESC_T,
    &t_rpc_decl_STRM_TELTEX_DESC_T,
    &t_rpc_decl_SNK_DESC_T_u_text_strm,
    &t_rpc_decl_SNK_DESC_T,
    &t_rpc_decl_SVCTX_ASSC_CFG_T,
    &t_rpc_decl_EVENT_CAPABILITY_T,
    &t_rpc_decl_TEXT_CAPABILITY_T,
    &t_rpc_decl_DEVICE_TYPE_PROFILE_T,
    &t_rpc_decl_TUNER_OUT_INFO_T,
    &t_rpc_decl_TUNER_OUT_DESC_T,
    &t_rpc_decl_SVCTX_VIDEO_THUMBNAIL_INFO_T,
    &t_rpc_decl_ID_IND_T,
    &t_rpc_decl_ID_LIST_T,
    &t_rpc_decl_ID_RANGE_T,
    &t_rpc_decl_DRV_COMP_ID_T,
    &t_rpc_decl_DRV_COMP_REG_T_u,
    &t_rpc_decl_DRV_COMP_REG_T,
    &t_rpc_decl_DRV_COMP_FCT_TBL_T,
    &t_rpc_decl_DIVX_DRM_CRYPTO_INFO_T,
    &t_rpc_decl_DIVX_DRM_INFO_DRV_T,
    &t_rpc_decl_DRM_INFO_DRV_T_u,
    &t_rpc_decl_DRM_INFO_DRV_T,
    &t_rpc_decl_DIVX_DRM_HW_SECRET_KEY_T,
    &t_rpc_decl_FEEDER_SRC_INPUT_FCT_TBL_T,
    &t_rpc_decl_MEDIA_FEEDER_T,
    &t_rpc_decl_MM_PB_CTRL_PULL_INFO_T,
    &t_rpc_decl_MM_PB_CTRL_PUSH_BUF_INFO_T,
    &t_rpc_decl_MM_PB_CTRL_PUSH_MODE_INFO_T,
    &t_rpc_decl_MEDIA_MULTI_CACHE_T,
    &t_rpc_decl_MEDIA_FEEDER_NW_T,
    &t_rpc_decl_STREAM_ID_PS_T,
    &t_rpc_decl_STREAM_ID_T,
    &t_rpc_decl_PVR_PIC_INFO_T,
    &t_rpc_decl_PVR_TICK_DETAIL_T,
    &t_rpc_decl_PVR_TICK_INDEX_T,
    &t_rpc_decl_PVR_TICK_CTRL_BLK_T,
    &t_rpc_decl_START_POS_INFO_T,
    &t_rpc_decl_MM_RANGE_POS_INFO_T,
    &t_rpc_decl_MM_POS_T,
    &t_rpc_decl_MM_AB_POS_T,
    &t_rpc_decl_MM_BUF_RANGE_INFO_T,
    &t_rpc_decl_MM_REC_BUF_RANGE_INFO_T,
    &t_rpc_decl_MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T,
    &t_rpc_decl_MM_RANGE_ELEMT_IDXTBL_TS_ENTRY_T,
    &t_rpc_decl_MM_RANGE_ELMT_IDXTBL_T_u,
    &t_rpc_decl_MM_RANGE_ELMT_IDXTBL_T,
    &t_rpc_decl_MM_RANGE_TRACK_TM_SCALE_T,
    &t_rpc_decl_MM_AUDIO_VORBIS_OFFSET_T,
    &t_rpc_decl_MM_RANGE_DECODER_T,
    &t_rpc_decl_MM_RANGE_INFO_T,
    &t_rpc_decl_MM_PVR_CRYPT_INFO_T,
    &t_rpc_decl_MM_TRACK_CRYPT_INFO_T,
    &t_rpc_decl_MM_ML_CRYPT_INFO_T,
    &t_rpc_decl_MM_CRYPT_INFO_T,
    &t_rpc_decl_MM_CRYPT_IV_T,
    &t_rpc_decl_MM_DESC_CRYPT_INFO_T,
    &t_rpc_decl_MM_CRYPT_CMD_T,
    &t_rpc_decl_REC_UTIL_CLI_DUMP_TICK_CB_T,
    &t_rpc_decl_SVCTX_REC_ES_INFO_T,
    &t_rpc_decl_SVCTX_REC_ES_LIST_T,
    &t_rpc_decl_SVCTX_REC_DEL_DATA_T,
    &t_rpc_decl_SVCTX_REC_ATTR_T,
    &t_rpc_decl_SVCTX_REC_TIME_BOUND_INFO_T,
    &t_rpc_decl_SVCTX_REC_SPACE_BOUND_INFO_T,
    &t_rpc_decl_SVCTX_REC_STRG_DESC_T,
    &t_rpc_decl_SVCTX_REC_CONT_INFO_NORMAL_T,
    &t_rpc_decl_SVCTX_REC_CONT_INFO_CONT_T,
    &t_rpc_decl_SVCTX_REC_CONFIG_T,
    &t_rpc_decl_SVCTX_REC_CAP_INFO_T,
    &t_rpc_decl_SVCTX_REC_SYS_RES_INFO_T,
    &t_rpc_decl_SVCTX_REC_STATUS_T,
    &t_rpc_decl_SVCTX_REC_ES_AUTH_INFO_T,
    &t_rpc_decl_SVCTX_REC_ESL_AUTH_STATUS_T,
    &t_rpc_decl_SVCTX_REC_META_DB_OPEN_T,
    &t_rpc_decl_SVCTX_REC_META_DB_CLOSE_T,
    &t_rpc_decl_SVCTX_REC_META_DB_STORE_T,
    &t_rpc_decl_SVCTX_REC_SET_META_INFO_T,
    &t_rpc_decl_SVCTX_REC_GET_META_INFO_LEN_T,
    &t_rpc_decl_SVCTX_REC_GET_META_INFO_T,
    &t_rpc_decl_SRC_REC_INFO_T,
    &t_rpc_decl_AVC_TYPE_MAPPING,
    &t_rpc_decl_FM_FLASH_PART_BULK_WRITE_T,
    &t_rpc_decl_FM_DIR_INFO_T,
    &t_rpc_decl_FM_FILE_INFO_T,
    &t_rpc_decl_FM_DIR_ENTRY_T,
    &t_rpc_decl_FM_FS_INFO_T,
    &t_rpc_decl_FM_MNT_PARM_T,
    &t_rpc_decl_FM_PART_INFO_T,
    &t_rpc_decl_FM_MNT_CB_T,
    &t_rpc_decl_FM_CONFIRM_FILE_REPLACE_T,
    &t_rpc_decl_FM_CONFIRM_DIR_REPLACE_T,
    &t_rpc_decl_FM_COPY_FILE_PRGS_T,
    &t_rpc_decl_FM_SEARCH_RESULT_T,
    &t_rpc_decl_FLM_LIST_T,
    &t_rpc_decl_FLM_REC_T,
    &t_rpc_decl_RRCTX_VCHIP_TYPE_T,
    &t_rpc_decl_RRCTX_VCHIP_RATE_SETTING_T,
    &t_rpc_decl_RRCTX_RRT_RATE_SETTING_T,
    &t_rpc_decl_DTG_T,
    &t_rpc_decl_DELTA_DTG_T,
    &t_rpc_decl_DT_CONFIG_T,
    &t_rpc_decl_TM_SECOND_LOOP_ALL_DESCRS_T,
    &t_rpc_decl_TM_SECOND_LOOP_DESCR_SEL_T,
    &t_rpc_decl_TM_SECOND_LOOP_ALL_MULTI_DESCRS_T,
    &t_rpc_decl_TM_SECOND_LOOP_MULTI_DESCR_SEL_T,
    &t_rpc_decl_TM_FIRST_LOOP_DESCR_SEL_T,
    &t_rpc_decl_TM_MAIN_BODY_DESCR_SEL_T,
    &t_rpc_decl_TM_UPDATE_TIME_T,
    &t_rpc_decl_TM_BUFFER_T,
    &t_rpc_decl_ATSC_SI_MGT_INFO_T,
    &t_rpc_decl_ATSC_SI_VCT_INFO_T,
    &t_rpc_decl_ATSC_SI_RRT_INFO_T,
    &t_rpc_decl_ATSC_SI_EIT_INFO_T,
    &t_rpc_decl_ATSC_SI_TXT_LANG_INFO_T,
    &t_rpc_decl_ATSC_SI_TXT_SEG_INFO_T,
    &t_rpc_decl_ATSC_SI_LANG_SEG_IDX_INFO_T,
    &t_rpc_decl_ATSC_SI_TXT_INFO_T,
    &t_rpc_decl_ATSC_SI_TXT_DIM_VAL_IDX_INFO_T,
    &t_rpc_decl_ATSC_SI_IDX_TIME_INFO_T,
    &t_rpc_decl_ATSC_SI_SRC_ID_TIME_INFO_T,
    &t_rpc_decl_ATSC_SI_SRC_EVT_ID_TIME_INFO_T,
    &t_rpc_decl_EAS_EVENT_T,
    &t_rpc_decl_EAS_LOC_INFO_T,
    &t_rpc_decl_EAS_XCEPT_INFO_T,
    &t_rpc_decl_EAS_EVT_CODE_INFO_T,
    &t_rpc_decl_ATSC_SI_CONFIG_T,
    &t_rpc_decl_SB_FREQ_RANGE_T,
    &t_rpc_decl_SB_NEW_SVC_NFY_DATA,
    &t_rpc_decl_SB_LCNV2_CH_LST_INFO,
    &t_rpc_decl_SB_ATSC_OPEN_DATA_T,
    &t_rpc_decl_SB_ATSC_SCAN_DATA_T,
    &t_rpc_decl_SB_NTSC_OPEN_DATA_T,
    &t_rpc_decl_SB_NTSC_SCAN_DATA_T,
    &t_rpc_decl_SB_CQAM_OPEN_DATA_T,
    &t_rpc_decl_SB_CQAM_SCAN_DATA_T,
    &t_rpc_decl_SB_DVB_REGION_CODE_T,
    &t_rpc_decl_SB_DVB_REGION_T,
    &t_rpc_decl_SB_DVB_CH_LST_LCN_INFO,
    &t_rpc_decl_SB_DVB_SCAN_INDEX_INFO,
    &t_rpc_decl_SB_DVB_FREQ_BW_T,
    &t_rpc_decl_SB_DVB_INIT_DATA_T,
    &t_rpc_decl_SB_DVB_OPEN_DATA_T,
    &t_rpc_decl_SB_DVB_SCAN_INFO_T,
    &t_rpc_decl_SB_DVB_SVC_UPDATE_NFY_DATA,
    &t_rpc_decl_SB_DVB_SCAN_DATA_T,
    &t_rpc_decl_SB_DVB_TSL_REC_ID_FREQ_RANGE_T,
    &t_rpc_decl_SB_DVB_ON_TS_SVC_ID_T,
    &t_rpc_decl_SB_DVB_COUNTRY_ONID_T,
    &t_rpc_decl_SB_DVB_ONID_COUNT_INFO_T,
    &t_rpc_decl_SB_DVB_GET_OTHER_COUNTRY_ONID_INFO_T,
    &t_rpc_decl_SB_DVB_NW_CHANGE_NFY_T,
    &t_rpc_decl_REGION_CODE_INFO_t_link,
    &t_rpc_decl_REGION_CODE_INFO,
    &t_rpc_decl_REGION_CODE_INFO_LIST,
    &t_rpc_decl_SB_DVB_LCN_CONFLICT_GROUP_T,
    &t_rpc_decl_SB_DVB_LCN_CONFLICT_CHANNEL_T,
    &t_rpc_decl_SB_DVBC_INIT_DATA_T,
    &t_rpc_decl_SB_DVBC_OPEN_DATA_T,
    &t_rpc_decl_SB_DVBC_SCAN_INFO_T,
    &t_rpc_decl_SB_DVBC_DLVR_INFO_T,
    &t_rpc_decl_SB_DVBC_TUNER_PARA_T,
    &t_rpc_decl_SB_DVBC_SVC_UPDATE_NFY_DATA,
    &t_rpc_decl_SB_DVB_SCANNED_CH_T,
    &t_rpc_decl_SB_DVBC_NW_CHANGE_NFY_DATA,
    &t_rpc_decl_SB_DVBC_SCAN_DATA_T,
    &t_rpc_decl_SB_DVBC_TSL_REC_ID_FREQ_RANGE_T,
    &t_rpc_decl_SB_DVBC_ON_TS_SVC_ID_T,
    &t_rpc_decl_SB_DVBC_NW_CHANGE_NFY_T,
    &t_rpc_decl_VBIF_SRC_ANA_VBI_T,
    &t_rpc_decl_VBIF_SRC_DIG_VBI_ES_T,
    &t_rpc_decl_VBIF_SRC_DIG_TTX_ES_T,
    &t_rpc_decl_FLTR_INIT_TTX_PKT_T,
    &t_rpc_decl_TTX_PKT_INFO_T,
    &t_rpc_decl_FLTR_NFY_TTX_PKT_SRC_ANA_VBI_T,
    &t_rpc_decl_FLTR_TTX_PKT_BUFF_ATTR_ANA_VBI_T,
    &t_rpc_decl_FLTR_NFY_TTX_PKT_SRC_DIG_VBI_ES_T,
    &t_rpc_decl_FLTR_TTX_PKT_BUFF_ATTR_DIG_VBI_ES_T,
    &t_rpc_decl_FLTR_NFY_TTX_PKT_SRC_DIG_TTX_ES_T,
    &t_rpc_decl_FLTR_TTX_PKT_BUFF_ATTR_DIG_TTX_ES_T,
    &t_rpc_decl_FLTR_TTX_PKT_BUFF_INFO_T_u,
    &t_rpc_decl_FLTR_TTX_PKT_BUFF_INFO_T,
    &t_rpc_decl_FLTR_TTX_PKT_BUFF_TAG_T,
    &t_rpc_decl_FLTR_TTX_PKT_BUFF_LOCK_INFO_T,
    &t_rpc_decl_FLTR_TTX_PKT_BUFF_ATTR_T,
    &t_rpc_decl_FLTR_NFY_TTX_PKT_T,
    &t_rpc_decl_FLTR_INIT_TTX_MIP_T,
    &t_rpc_decl_TTX_MIP_NODE_T,
    &t_rpc_decl_FLTR_NFY_TTX_MIP_T,
    &t_rpc_decl_FLTR_NI_INIT_TTX_HDR_CH_NAME_T,
    &t_rpc_decl_FLTR_NI_INIT_ACI_T,
    &t_rpc_decl_FLTR_INIT_NI_T,
    &t_rpc_decl_FLTR_NI_NFY_TTX_HDR_CH_NAME_T,
    &t_rpc_decl_FLTR_NI_NFY_VPS_DATA_T,
    &t_rpc_decl_FLTR_NI_NFY_8_30_FMT_1_PRG_TTL_T,
    &t_rpc_decl_FLTR_NI_NFY_TTX_DATE_TIME_T,
    &t_rpc_decl_FLTR_NI_NFY_ACI_DETECTED_T,
    &t_rpc_decl_FLTR_NI_NFY_ACI_SLCTN_PAGE_UPDATE_T,
    &t_rpc_decl_FLTR_NI_NFY_ACI_DB_READY_T,
    &t_rpc_decl_FLTR_NI_NFY_ACI_T,
    &t_rpc_decl_FLTR_NFY_NI_T_u,
    &t_rpc_decl_FLTR_NFY_NI_T,
    &t_rpc_decl_FLTR_NI_ACI_DB_INFO_T,
    &t_rpc_decl_FLTR_NI_ACI_CITY_LIST_INFO_T,
    &t_rpc_decl_FLTR_NI_ACI_CITY_INFO_T,
    &t_rpc_decl_FLTR_NI_ACI_CH_NUM_T,
    &t_rpc_decl_FLTR_NI_ACI_CH_INFO_T,
    &t_rpc_decl_FLTR_INIT_EU_EVN_T,
    &t_rpc_decl_FLTR_EU_EVN_DATA_PRG_TTL_T,
    &t_rpc_decl_FLTR_NFY_EU_EVN_T,
    &t_rpc_decl_FLTR_INIT_EPG_T,
    &t_rpc_decl_FLTR_NFY_EPG_T,
    &t_rpc_decl_FLTR_INIT_EU_WSS_T,
    &t_rpc_decl_FLTR_NFY_EU_WSS_T,
    &t_rpc_decl_FLTR_INIT_EU_VBI_D2A_T,
    &t_rpc_decl_SB_PAL_SECAM_INIT_DATA_T,
    &t_rpc_decl_SB_PAL_SECAM_OPEN_DATA_T,
    &t_rpc_decl_SB_PAL_SECAM_FREQ_PROGRESS_NFY_DATA,
    &t_rpc_decl_SB_PAL_SECAM_ANAS_NFY_DATA,
    &t_rpc_decl_SB_PAL_SECAM_CUSTOM_SVL_CONFIG,
    &t_rpc_decl_SB_PAL_SECAM_ACI_NFY_DATA,
    &t_rpc_decl_SB_PAL_SECAM_USER_OPERATION_DATA,
    &t_rpc_decl_SB_PAL_SECAM_SCAN_DATA_T,
    &t_rpc_decl_SB_ACI_SLCT_CITY_T,
    &t_rpc_decl_SB_USER_OPERATION_T,
    &t_rpc_decl_SB_INSERT_DEFAULT_SVL_TSL_T,
    &t_rpc_decl_SB_ISDB_INIT_DATA_T,
    &t_rpc_decl_SB_ISDB_OPEN_DATA_T,
    &t_rpc_decl_SB_ISDB_SCAN_INFO_T,
    &t_rpc_decl_SB_ISDB_SCAN_DATA_T,
    &t_rpc_decl_LOL_REC_T,
    &t_rpc_decl_LOL_MEM_DUMP_T,
    &t_rpc_decl_NWL_REC_T,
    &t_rpc_decl_NWL_MEM_DUMP_T,
    &t_rpc_decl_PCL_GPIO_MONITOR_CTRL_T,
    &t_rpc_decl_PCL_GPIO_MONITOR_CTRL_EX_T,
    &t_rpc_decl_PCL_SIF_INFO_T,
    &t_rpc_decl_PCL_SIF_INFO_BY_GPIO_T,
    &t_rpc_decl_PCL_SERVO_AD_INFO_T,
    &t_rpc_decl_PCL_WAKE_UP_CTRL_T,
    &t_rpc_decl_RPT_EVT_ITVL_T,
    &t_rpc_decl_DISABLE_EVTCODE_T,
    &t_rpc_decl_MAGIC_UNIT_T,
    &t_rpc_decl_MAGIC_T,
    &t_rpc_decl_IRRC_SETTING_T,
    &t_rpc_decl_IOM_RAW_DATA_TO_EVT_CODE_T,
    &t_rpc_decl_IOM_RAW_BITS_TO_EVT_CODE_T,
    &t_rpc_decl_IOM_SYS_CODE_FILTER_T,
    &t_rpc_decl_IRRC_DATA_T,
    &t_rpc_decl_DBG_DATA_T,
    &t_rpc_decl_IOM_IND_T,
    &t_rpc_decl_IOM_FPD_T,
    &t_rpc_decl_IOM_UART_T,
    &t_rpc_decl_UART_SETTING_T,
    &t_rpc_decl_AUTO_SLEEP_TIMER_T,
    &t_rpc_decl_RRCTX_RRT_KEY_PARAM_T,
    &t_rpc_decl_EVCTX_ATSC_KEY_INFO_T,
    &t_rpc_decl_EVCTX_ATSC_RATING_T,
    &t_rpc_decl_EVCTX_ATSC_RATING_LIST_T,
    &t_rpc_decl_EVCTX_ATSC_ENG_CONFIG_T,
    &t_rpc_decl_PLAYBACK_CTRL_PULL_INFO_T,
    &t_rpc_decl_PLAYBACK_FLOW_CTRL_INFO_T,
    &t_rpc_decl_PLAYBACK_FEEDER_BUF_INFO_T,
    &t_rpc_decl_PLAYBACK_MODE_CTRL_T,
    &t_rpc_decl_PLAYBACK_GET_BUFFER_FULLNESS_T,
    &t_rpc_decl_PLAYBACK_GET_BUFFER_STATUS_T,
    &t_rpc_decl_PLAYBACK_HANDLER_ITEM_ATTRIBUTES_T,
    &t_rpc_decl_PLAYBACK_HANDLER_ITEM_ATTRIBUTES_BY_INDEX_T,
    &t_rpc_decl_PLAYBACK_HANDLER_SHARED_MEMORY_T,
    &t_rpc_decl_PLAYBACK_HANDLER_TIME_TO_SIZE_T,
    &t_rpc_decl_PLAYBACK_HANDLER_ITEM_SCDB_INFO_T,
    &t_rpc_decl_PLAYBACK_HANDLER_ITEM_SVC_INFO_T,
    &t_rpc_decl_PLAYBACK_HANDLER_ITEM_SVC_INFO_BY_ID_T,
    &t_rpc_decl_PLAYBACK_HANDLER_ITEM_MPEG_2_INFO_T,
    &t_rpc_decl_PLAYBACK_HANDLER_ITEM_ES_INFO_T,
    &t_rpc_decl_PLAYBACK_CTRL_IBC_PARAM_SET_ASF_PACKET_INFO_T,
    &t_rpc_decl_PLAYBACK_CTRL_IBC_AES_ENCRYPT_INFO_T,
    &t_rpc_decl_PLAYBACK_CTRL_IBC_PARAM_ENCRYPT_INFO_T,
    &t_rpc_decl_PLAYBACK_CTRL_IBC_PARAM_T,
    &t_rpc_decl_PLAYBACK_HANDLER_ITEM_IBC_INFO_T,
    &t_rpc_decl_PLAYBACK_HANDLER_ITEM_EXTRA_INFO_T,
    &t_rpc_decl_PLAYBACK_HANDLER_QUEUE_URI_T,
    &t_rpc_decl_PLAYBACK_HANDLER_QUEUE_FILE_T,
    &t_rpc_decl_PLAYBACK_HANDLER_QUEUE_BUFFER_T,
    &t_rpc_decl_PLAYBACK_HANDLER_QUEUE_PULL_T,
    &t_rpc_decl_PSI_PID_SVC_ID_MAP_INFO_T,
    &t_rpc_decl_PLAYBACK_PMT_LIST_T,
    &t_rpc_decl_CECM_SET_CEC_FCT_PARAMS_T,
    &t_rpc_decl_CECM_ARC_CTRL_T,
    &t_rpc_decl_CECM_ACT_SRC_REVED_T,
    &t_rpc_decl_CECM_VNDR_CMD_DATA_T,
    &t_rpc_decl_CECM_SPEC_CMD_DATA_T,
    &t_rpc_decl_CECM_VNDR_CMD_WITH_ID_DATA_T,
    &t_rpc_decl_CECM_VNDR_RC_CODE_DATA_T,
    &t_rpc_decl_CECM_DEV_LIST_NFY_T,
    &t_rpc_decl_CECM_5V_PWR_INFO_T,
    &t_rpc_decl_CECM_CTRL_T,
    &t_rpc_decl_CECM_DEV_DISCOVERY_T,
    &t_rpc_decl_CECM_LOG_ADD_LST_T,
    &t_rpc_decl_CECM_WAKE_UP_CTRL_T,
    &t_rpc_decl_CECM_ONE_TOUCH_PLAY_T,
    &t_rpc_decl_CECM_ACT_SRC_T,
    &t_rpc_decl_CECM_ROUTING_CHG_INFO_T,
    &t_rpc_decl_CECM_ANA_SVC_T,
    &t_rpc_decl_CECM_DIG_SVC_T,
    &t_rpc_decl_CECM_REC_SRC_T,
    &t_rpc_decl_CECM_TIMER_ANA_T,
    &t_rpc_decl_CECM_TIMER_EXT_T,
    &t_rpc_decl_CECM_TIMER_INFO_T,
    &t_rpc_decl_CECM_TIMER_STS_T,
    &t_rpc_decl_CECM_REPORT_PHY_ADDR_T,
    &t_rpc_decl_CECM_POLLING_INFO_T,
    &t_rpc_decl_CECM_TUNER_SVC_INFO_T,
    &t_rpc_decl_CECM_TUNER_DEV_INFO_T,
    &t_rpc_decl_CECM_OSD_STRING_INFO_T,
    &t_rpc_decl_CECM_USER_CTRL_OPERAND_INFO_T,
    &t_rpc_decl_CECM_USER_CTRL_WITH_OPERAND_T,
    &t_rpc_decl_CECM_PWR_ON_DEV_STS_T,
    &t_rpc_decl_CECM_FEATURE_ABORT_T,
    &t_rpc_decl_CECM_SYS_AUD_CTRL_T,
    &t_rpc_decl_CECM_AUD_DESCRIPTOR_T,
    &t_rpc_decl_CECM_AUD_FMT_T,
    &t_rpc_decl_CECM_DD_VNDR_CMD_PSR_INFO_T,
    &t_rpc_decl_CECM_DD_GET_INFO_T,
    &t_rpc_decl_CECM_DD_VNDR_CMD_INFO_T,
    &t_rpc_decl_CECM_DD_VNDR_CMD_T,
    &t_rpc_decl_CECM_DEV_DISCOVERY_CONFIG_T,
    &t_rpc_decl_CECM_DEV_VNDR_INFO_T,
    &t_rpc_decl_CECM_DEV_T,
    &t_rpc_decl_CECM_NFY_T,
    &t_rpc_decl_SATL_REC_T,
    &t_rpc_decl_SATL_MEM_DUMP_T,
    &t_rpc_decl_LST_MNGR_CLEAN_T,
    &t_rpc_decl_AUD_TYPE_VOL_GAIN_INFO_T,
    &t_rpc_decl_AUD_TYPE_AVL_GAIN_INFO_T,
    &t_rpc_decl_AUD_TYPE_SPECTRUM_INFO_T,
    &t_rpc_decl_AUD_TYPE_TEST_TONE,
    &t_rpc_decl_SMART_AUD_INFO_T,
    &t_rpc_decl_SMART_AUD_GROUP_INFO_T,
    &t_rpc_decl_DRV_CUSTOM_AUD_VOL_CURVE_T,
    &t_rpc_decl_AUD_TYPE_DD_CONTROL_T,
    &t_rpc_decl_DRV_CUSTOM_AUD_TYPE_AQ_VER_T,
    &t_rpc_decl_AUD_TYPE_BASS_BOOSTER,
    &t_rpc_decl_TUNER_TYPE_DIAG_CONTROL_T,
    &t_rpc_decl_TUNER_TYPE_DIAG_CONTENT_T,
    &t_rpc_decl_TUNER_TYPE_DIAG_AGC_T,
    &t_rpc_decl_VID_INFO_T,
    &t_rpc_decl_GAMMA_INFO_T,
    &t_rpc_decl_VID_NON_LINEAR_INFO_T,
    &t_rpc_decl_OUTRGB_INFO_T,
    &t_rpc_decl_SMART_PIC_INFO_T,
    &t_rpc_decl_SMART_PIC_GROUP_INFO_T,
    &t_rpc_decl_PIC_INFO_T,
    &t_rpc_decl_PQ_DEMO_INFO_T,
    &t_rpc_decl_DRV_CUSTOM_GAMMA_VALUE_T,
    &t_rpc_decl_DRV_CUSTOM_GAMMA_DELTA_T,
    &t_rpc_decl_DRV_CUSTOM_PATTERN_T,
    &t_rpc_decl_DRV_GAMMA_BUF_T,
    &t_rpc_decl_DRV_GAMMA_GETBUF_T,
    &t_rpc_decl_DRV_CUSTOM_SCE_PATTERN_T,
    &t_rpc_decl_DRV_CUSTOM_DBC_DEBUG_T,
    &t_rpc_decl_AUTO_BACKLIGHT_SETS_INFO_T,
    &t_rpc_decl_AUTO_COLORTEMP_SETS_INFO_T,
    &t_rpc_decl_AUTO_COLORTEMP_GETS_INFO_T,
    &t_rpc_decl_VID_TYPE_DOT_TO_DOT_PATTERN_T,
    &t_rpc_decl_LVDS_SPREAD_SPECTRUM_INFO_T,
    &t_rpc_decl_VID_TYPE_PANEL_RESOLUTION_T,
    &t_rpc_decl_EXTMJC_TYPE_SET_OSD_MODE_T,
    &t_rpc_decl_MISC_TYPE_TUNER_SIF_T,
    &t_rpc_decl_MISC_TYPE_HDMI_EDID_T,
    &t_rpc_decl_EDID_T,
    &t_rpc_decl_MISC_TYPE_SIF_ADVANCED_T,
    &t_rpc_decl_PANEL_SELECTION_CONFIG_T,
    &t_rpc_decl_DRV_CUST_DRAM_CFG_T,
    &t_rpc_decl_DRV_CUSTOM_MISC_CRNT_INP_SRC_INFO_T,
    &t_rpc_decl_MISC_TYPE_HDMI_HDCP_T,
    &t_rpc_decl_MISC_TYPE_VGA_EDID_T,
    &t_rpc_decl_DRV_CUSTOM_MISC_LOADER_INFO_T,
    &t_rpc_decl_MISC_TYPE_LED_DISPLAY_T,
    &t_rpc_decl_MISC_TYPE_LED_NOTIFY_T,
    &t_rpc_decl_MISC_TYPE_NAND_FLASH_WRITE_T,
    &t_rpc_decl_MISC_TYPE_NAND_FLASH_BULK_WRITE_T,
    &t_rpc_decl_MISC_TYPE_NOR_FLASH_BULK_WRITE_T,
    &t_rpc_decl_AUD_TYPE_SET_INFO_T,
    &t_rpc_decl_AUD_TYPE_GET_INFO_T,
    &t_rpc_decl_AUD_TYPE_GET_MIN_MAX_INFO_T,
    &t_rpc_decl_TUNER_TYPE_SET_INFO_T,
    &t_rpc_decl_TUNER_TYPE_GET_INFO_T,
    &t_rpc_decl_VID_TYPE_SET_INFO_T,
    &t_rpc_decl_VID_TYPE_GET_INFO_T,
    &t_rpc_decl_VID_TYPE_GET_GAMMA_INFO_T,
    &t_rpc_decl_VID_TYPE_GET_MIN_MAX_INFO_T,
    &t_rpc_decl_VID_DEC_TYPE_SET_INFO_T,
    &t_rpc_decl_VID_DEC_TYPE_GET_INFO_T,
    &t_rpc_decl_EXTMJC_TYPE_SET_INFO_T,
    &t_rpc_decl_EXTMJC_TYPE_GET_INFO_T,
    &t_rpc_decl_MISC_TYPE_SET_INFO_T,
    &t_rpc_decl_MISC_TYPE_GET_INFO_T,
    &t_rpc_decl_MISC_TYPE_GET_MIN_MAX_INFO_T,
    &t_rpc_decl_CUST_SPEC_TYPE_SET_INFO_T,
    &t_rpc_decl_CUST_SPEC_TYPE_GET_INFO_T,
    &t_rpc_decl_DRV_CUST_OPERATION_INFO_T_u,
    &t_rpc_decl_DRV_CUST_OPERATION_INFO_T,
    &t_rpc_decl_PSI_TS_VER_INFO_T,
    &t_rpc_decl_PSI_PID_SVC_ID_INFO_T,
    &t_rpc_decl_PSI_STREAM_INFO_T,
    &t_rpc_decl_PSI_CONFIG_T,
    &t_rpc_decl_CAP_CAPTURE_INFO_T,
    &t_rpc_decl_CAP_CAPABILITY_INFO_T,
    &t_rpc_decl_CAP_LOGO_SAVE_INFO_T_u,
    &t_rpc_decl_CAP_LOGO_SAVE_INFO_T,
    &t_rpc_decl_CAP_OBJ_T,
    &t_rpc_decl_CI_MMI_MENU_ITEM_LIST_T,
    &t_rpc_decl_CI_MMI_ENQ_T,
    &t_rpc_decl_CI_MMI_ENQ_ANS_T,
    &t_rpc_decl_CI_MMI_ENQ_CLOSE_T,
    &t_rpc_decl_CI_MMI_MENU_ITEM_T_t_link,
    &t_rpc_decl_CI_MMI_MENU_ITEM_T,
    &t_rpc_decl_CI_MMI_MENU_T,
    &t_rpc_decl_CI_MMI_MENU_ANS_T,
    &t_rpc_decl_CI_SLOT_INFO_T,
    &t_rpc_decl_CI_HC_TUNE_T,
    &t_rpc_decl_CI_HC_REPLACE_T,
    &t_rpc_decl_CI_HC_CLR_REPLACE_T,
    &t_rpc_decl_CI_CA_SYSTEM_ID_MATCH_T,
    &t_rpc_decl_CI_CA_SYSTEM_ID_STATUS_T,
    &t_rpc_decl_CI_CONFIG_T
};

const RPC_DESC_T* __rpc_get_desc__ (UINT32  ui4_idx)
{
  return ((ui4_idx < 915) ? at_rpc_desc_list [ui4_idx] : NULL);
}


