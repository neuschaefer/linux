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
 * $RCSfile: x_scc.c,v $
 * $Revision: #2 $
 * $Date: 2012/07/20 $
 * $Author: xingju.wang $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/46 $
 * $SWAuthor: Dennis Wang $
 * $MD5HEX: 182cd286bb743f5e9486639b86df2d46 $
 *
 * Description:
 *         This header file implements the Stream Component Controller.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/rm.h"
#include "handle/handle.h"

#include "strm_mngr/scc/x_scc.h"
#include "strm_mngr/_sm_utils.h"
#include "strm_mngr/sm_slctr.h"
#include "strm_mngr/sm_drv_comp.h"
#include "strm_mngr/sm_drv_map.h"
#include "strm_mngr/scc/_scc_vid.h"
#include "strm_mngr/scc/_scc_disp.h"
#include "strm_mngr/scc/_scc_aud.h"

#include "res_mngr/drv/x_aud_dec.h"
#include "res_mngr/drv/x_plane_mxr.h"
#include "res_mngr/drv/x_tv_enc.h"
#ifdef MW_TV_AV_BYPASS_SUPPORT
#include "res_mngr/drv/x_video_out.h"
#endif
#include "res_mngr/drv/x_vid_plane.h"
#include "res_mngr/drv/x_vid_dec.h"
#include "res_mngr/drv/x_tv_dec.h"
#include "res_mngr/drv/u_drv_cust.h"
#include "strm_mngr/video_hdlr/u_sm_video_hdlr.h"
#include "strm_mngr/audio_hdlr/u_sm_audio_hdlr.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define SCC_INV_SET_GET_TYPE        ((UINT32)0xFFFFFFFF)


/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
 
static UINT32  ui4_ap_muted_out_port_msk = 0;   

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/
static AUD_DEC_KARAOKE_CTRL_T _scc_karaoke_ctrl_2_drv_karaoke_ctrl(
    SCC_AUD_KARAOKE_CTRL_T e_scc_karaoke_ctrl )
{
    switch( e_scc_karaoke_ctrl )
    {
    case SCC_AUD_KARAOKE_CONNECT:
        return AUD_DEC_KARAOKE_CONNECT;
    case SCC_AUD_KARAOKE_ENABLE:
        return AUD_DEC_KARAOKE_ENABLE;
    case SCC_AUD_KARAOKE_ECHO:
        return AUD_DEC_KARAOKE_ECHO;
    case SCC_AUD_KARAOKE_VOCAL_CANEL:
        return AUD_DEC_KARAOKE_VOCAL_CANEL;
    case SCC_AUD_KARAOKE_KEY_SHIFT:
        return AUD_DEC_KARAOKE_KEY_SHIFT;
    case SCC_AUD_KARAOKE_MIC_VOL1:
        return AUD_DEC_KARAOKE_MIC_VOL1;
    case SCC_AUD_KARAOKE_MIC_VOL2:
        return AUD_DEC_KARAOKE_MIC_VOL2;
    default:
        break;
    }

    return AUD_DEC_KARAOKE_CONNECT;
}

static UINT16 _floor_16bits_drv_val_2_scc_val(
    UINT16          ui2_scc_max,
    UINT16          ui2_drv_val,
    UINT16          ui2_drv_min,
    UINT16          ui2_drv_max )
{
    UINT16    ui2_scc_val;

    ui2_scc_val = ( (ui2_drv_val - ui2_drv_min) * ui2_scc_max )
                  / ( ui2_drv_max - ui2_drv_min );

    return ui2_scc_val;
}

static UINT16 _ceil_16bits_scc_val_2_drv_val(
    UINT16          ui2_scc_max,
    UINT16          ui2_scc_val,
    UINT16          ui2_drv_min,
    UINT16          ui2_drv_max )
{
    UINT16    ui2_drv_val;
    UINT32    ui4_val_tmp1;
    UINT32    ui4_remainder;

    ui4_val_tmp1 = (ui2_drv_max - ui2_drv_min) * ui2_scc_val;
    ui4_remainder = ui4_val_tmp1 % ui2_scc_max;
    ui4_val_tmp1 = ui4_val_tmp1 / ui2_scc_max;

    if ( ui4_remainder )
    {
        ui2_drv_val = (UINT16)(ui4_val_tmp1 + 1);
    }
    else
    {
        ui2_drv_val = (UINT16)ui4_val_tmp1;
    }

    return ui2_drv_min + ui2_drv_val;
}

static SCC_AUD_OUT_PORT_T _drv_aud_outport_2_scc_aud_outport(
    AUD_DEC_OUT_PORT_T e_drv_outport )
{
    switch( e_drv_outport )
    {
    case AUD_DEC_OUT_PORT_2_CH:
        return SCC_AUD_OUT_PORT_2_CH;
    case AUD_DEC_OUT_PORT_5_1_CH:
        return SCC_AUD_OUT_PORT_5_1_CH;
    case AUD_DEC_OUT_PORT_SPDIF:
        return SCC_AUD_OUT_PORT_SPDIF;
    case AUD_DEC_OUT_PORT_2_CH_BY_PASS:
        return SCC_AUD_OUT_PORT_2_CH_BY_PASS;
    case AUD_DEC_OUT_PORT_SPEAKER:
        return SCC_AUD_OUT_PORT_SPEAKER;
    case AUD_DEC_OUT_PORT_HEADPHONE:
        return SCC_AUD_OUT_PORT_HEADPHONE;
    case AUD_DEC_OUT_PORT_OFF:
    default:
        break;
    }

    return SCC_AUD_OUT_PORT_OFF;
}

static AUD_DEC_OUT_PORT_T _scc_aud_outport_2_drv_aud_outport(
    SCC_AUD_OUT_PORT_T e_scc_outport )
{
    switch( e_scc_outport )
    {
    case SCC_AUD_OUT_PORT_2_CH:
        return AUD_DEC_OUT_PORT_2_CH;
    case SCC_AUD_OUT_PORT_5_1_CH:
        return AUD_DEC_OUT_PORT_5_1_CH;
    case SCC_AUD_OUT_PORT_SPDIF:
        return AUD_DEC_OUT_PORT_SPDIF;
    case SCC_AUD_OUT_PORT_2_CH_BY_PASS:
        return AUD_DEC_OUT_PORT_2_CH_BY_PASS;
    case SCC_AUD_OUT_PORT_SPEAKER:
        return AUD_DEC_OUT_PORT_SPEAKER;
    case SCC_AUD_OUT_PORT_HEADPHONE:
        return AUD_DEC_OUT_PORT_HEADPHONE;
    case SCC_AUD_OUT_PORT_TVSCART:
        return AUD_DEC_OUT_PORT_TVSCART;
    case SCC_AUD_OUT_PORT_OFF:
    default:
        break;
    }

    return AUD_DEC_OUT_PORT_OFF;
}


static AUD_DEC_CHNL_DELAY_TYPE_T _scc_aud_ch_delay_type_2_drv_aud_ch_delay_type(
    SCC_AUD_CHNL_DELAY_TYPE_T e_scc_ch_delay_type )
{
    switch( e_scc_ch_delay_type )
    {
    case SCC_AUD_CHNL_DELAY_TYPE_ALL:
        return AUD_DEC_CHNL_DELAY_TYPE_ALL;
    case SCC_AUD_CHNL_DELAY_TYPE_INDIVIDUAL_CH:
        return AUD_DEC_CHNL_DELAY_TYPE_INDIVIDUAL_CH;
    default:
        break;
    }
    return AUD_DEC_CHNL_DELAY_TYPE_ALL;
}



static SCC_AUD_CHANNEL_T _drv_aud_ch_2_scc_aud_ch( AUD_DEC_LS_T e_drv_ch )
{
    switch( e_drv_ch )
    {
    case AUD_DEC_LS_FRONT_RIGHT:
        return SCC_AUD_CHANNEL_FRONT_RIGHT;
    case AUD_DEC_LS_REAR_LEFT:
        return SCC_AUD_CHANNEL_REAR_LEFT;
    case AUD_DEC_LS_REAR_RIGHT:
        return SCC_AUD_CHANNEL_REAR_RIGHT;
    case AUD_DEC_LS_CENTER:
        return SCC_AUD_CHANNEL_CENTER;
    case AUD_DEC_LS_SUB_WOOFER:
        return SCC_AUD_CHANNEL_SUB_WOOFER;
    case AUD_DEC_LS_FRONT_LEFT:
    default:
        break;
    }

    return SCC_AUD_CHANNEL_FRONT_LEFT;
}

static AUD_DEC_LS_T _scc_aud_ch_2_drv_aud_ch( SCC_AUD_CHANNEL_T e_scc_ch )
{
    switch( e_scc_ch )
    {
    case SCC_AUD_CHANNEL_FRONT_RIGHT:
        return AUD_DEC_LS_FRONT_RIGHT;
    case SCC_AUD_CHANNEL_REAR_LEFT:
        return AUD_DEC_LS_REAR_LEFT;
    case SCC_AUD_CHANNEL_REAR_RIGHT:
        return AUD_DEC_LS_REAR_RIGHT;
    case SCC_AUD_CHANNEL_CENTER:
        return AUD_DEC_LS_CENTER;
    case SCC_AUD_CHANNEL_SUB_WOOFER:
        return AUD_DEC_LS_SUB_WOOFER;
    case SCC_AUD_CHANNEL_FRONT_LEFT:
    default:
        break;
    }

    return AUD_DEC_LS_FRONT_LEFT;
}

static VOID _drv_ch_volume_2_scc_aud_volume(
    const AUD_DEC_CH_VOLUME_T*     pt_drv_volume,
    SCC_AUD_VOLUME_INFO_T*         pt_scc_volume )
{
    if ( !pt_drv_volume || !pt_scc_volume )
    {
        return;
    }

    pt_scc_volume->e_out_port = _drv_aud_outport_2_scc_aud_outport(
        pt_drv_volume->e_out_port );
    pt_scc_volume->e_ch = _drv_aud_ch_2_scc_aud_ch( pt_drv_volume->e_ls );
    pt_scc_volume->ui1_volumn = pt_drv_volume->ui1_level;
}

static VOID _scc_aud_volume_2_drv_ch_volume(
    const SCC_AUD_VOLUME_INFO_T*   pt_scc_volume,
    AUD_DEC_CH_VOLUME_T*           pt_drv_volume )
{
    if ( !pt_drv_volume || !pt_scc_volume )
    {
        return;
    }

    pt_drv_volume->e_out_port = _scc_aud_outport_2_drv_aud_outport(
        pt_scc_volume->e_out_port );
    pt_drv_volume->e_ls = _scc_aud_ch_2_drv_aud_ch( pt_scc_volume->e_ch );
    pt_drv_volume->ui1_level = pt_scc_volume->ui1_volumn;
}

static UINT32 _scc_attrib_2_drv_pla_mxr_get_type( SCC_DISP_ATTRIB_T e_scc_att )
{
    switch( e_scc_att )
    {
    case SCC_DISP_ATTRIB_BRIGHTNESS:
        return PLA_MXR_GET_TYPE_BRIGHTNESS;

    case SCC_DISP_ATTRIB_CONTRAST:
        return PLA_MXR_GET_TYPE_CONTRAST;

    case SCC_DISP_ATTRIB_HUE:
        return PLA_MXR_GET_TYPE_HUE;

    case SCC_DISP_ATTRIB_SATURATION:
        return PLA_MXR_GET_TYPE_SATURATION;

    case SCC_DISP_ATTRIB_GAMMA:
        return PLA_MXR_GET_TYPE_GAMMA;

    case SCC_DISP_ATTRIB_DITHER:
        return PLA_MXR_GET_TYPE_DITHER;

    case SCC_DISP_ATTRIB_BACK_LIGHT:
        return PLA_MXR_GET_TYPE_BACK_LIGHT;

    case SCC_DISP_ATTRIB_ADP_BACK_LIGHT:
        return PLA_MXR_GET_TYPE_ADP_BACK_LIGHT;

    case SCC_DISP_ATTRIB_BACK_LIGHT_ENERGY_SAVE:
        return PLA_MXR_GET_TYPE_BACK_LIGHT_ENERGY_SAVE;

    default:
        break;
    }

    return SCC_INV_SET_GET_TYPE;
}

static UINT32 _scc_attrib_2_drv_pla_mxr_set_type( SCC_DISP_ATTRIB_T e_scc_att )
{
    switch( e_scc_att )
    {
    case SCC_DISP_ATTRIB_BRIGHTNESS:
        return PLA_MXR_SET_TYPE_BRIGHTNESS;

    case SCC_DISP_ATTRIB_CONTRAST:
        return PLA_MXR_SET_TYPE_CONTRAST;

    case SCC_DISP_ATTRIB_HUE:
        return PLA_MXR_SET_TYPE_HUE;

    case SCC_DISP_ATTRIB_SATURATION:
        return PLA_MXR_SET_TYPE_SATURATION;

    case SCC_DISP_ATTRIB_GAMMA:
        return PLA_MXR_SET_TYPE_GAMMA;

    case SCC_DISP_ATTRIB_DITHER:
        return PLA_MXR_SET_TYPE_DITHER;

    case SCC_DISP_ATTRIB_BACK_LIGHT:
        return PLA_MXR_SET_TYPE_BACK_LIGHT;

    case SCC_DISP_ATTRIB_ADP_BACK_LIGHT:
        return PLA_MXR_SET_TYPE_ADP_BACK_LIGHT;

    case SCC_DISP_ATTRIB_BACK_LIGHT_ENERGY_SAVE:
        return PLA_MXR_SET_TYPE_BACK_LIGHT_ENERGY_SAVE;

    default:
        break;
    }

    return SCC_INV_SET_GET_TYPE;
}

static AUD_DEC_EQ_TYPE_T _convert_scc_se_2_drv_se( SCC_AUD_SE_EQ_T e_eq_type )
{
    switch( e_eq_type )
    {
    case SCC_AUD_SE_EQ_ROCK:
        return AUD_DEC_EQ_ROCK;
    case SCC_AUD_SE_EQ_POP:
        return AUD_DEC_EQ_POP;
    case SCC_AUD_SE_EQ_LIVE:
        return AUD_DEC_EQ_LIVE;
    case SCC_AUD_SE_EQ_DANCE:
        return AUD_DEC_EQ_DANCE;
    case SCC_AUD_SE_EQ_TECHNO:
        return AUD_DEC_EQ_TECHNO;
    case SCC_AUD_SE_EQ_CLASSIC:
        return AUD_DEC_EQ_CLASSIC;
    case SCC_AUD_SE_EQ_SOFT:
        return AUD_DEC_EQ_SOFT;
    case SCC_AUD_SE_EQ_USER_1:
        return AUD_DEC_EQ_USER_1;
    case SCC_AUD_SE_EQ_USER_2:
        return AUD_DEC_EQ_USER_2;
    case SCC_AUD_SE_EQ_USER_3:
        return AUD_DEC_EQ_USER_3;
    case SCC_AUD_SE_EQ_USER_4:
        return AUD_DEC_EQ_USER_4;
    case SCC_AUD_SE_EQ_USER_5:
        return AUD_DEC_EQ_USER_5;
    case SCC_AUD_SE_EQ_USER_6:
        return AUD_DEC_EQ_USER_6;
    case SCC_AUD_SE_EQ_USER_7:
        return AUD_DEC_EQ_USER_7;
    case SCC_AUD_SE_EQ_USER_8:
        return AUD_DEC_EQ_USER_8;
    case SCC_AUD_SE_EQ_BBE_TV:
        return AUD_DEC_EQ_BBE_TV;
    case SCC_AUD_SE_EQ_BBE_AV:
        return AUD_DEC_EQ_BBE_AV;
    case SCC_AUD_SE_EQ_OFF:
    default:
        return AUD_DEC_EQ_OFF;
    }
}

static SCC_AUD_SE_EQ_T _convert_drv_se_2_scc_se( AUD_DEC_EQ_TYPE_T e_eq_type )
{
    switch( e_eq_type )
    {
    case AUD_DEC_EQ_ROCK:
        return SCC_AUD_SE_EQ_ROCK;
    case AUD_DEC_EQ_POP:
        return SCC_AUD_SE_EQ_POP;
    case AUD_DEC_EQ_LIVE:
        return SCC_AUD_SE_EQ_LIVE;
    case AUD_DEC_EQ_DANCE:
        return SCC_AUD_SE_EQ_DANCE;
    case AUD_DEC_EQ_TECHNO:
        return SCC_AUD_SE_EQ_TECHNO;
    case AUD_DEC_EQ_CLASSIC:
        return SCC_AUD_SE_EQ_CLASSIC;
    case AUD_DEC_EQ_SOFT:
        return SCC_AUD_SE_EQ_SOFT;
    case AUD_DEC_EQ_USER_1:
        return SCC_AUD_SE_EQ_USER_1;
    case AUD_DEC_EQ_USER_2:
        return SCC_AUD_SE_EQ_USER_2;
    case AUD_DEC_EQ_USER_3:
        return SCC_AUD_SE_EQ_USER_3;
    case AUD_DEC_EQ_USER_4:
        return SCC_AUD_SE_EQ_USER_4;
    case AUD_DEC_EQ_USER_5:
        return SCC_AUD_SE_EQ_USER_5;
    case AUD_DEC_EQ_USER_6:
        return SCC_AUD_SE_EQ_USER_6;
    case AUD_DEC_EQ_USER_7:
        return SCC_AUD_SE_EQ_USER_7;
    case AUD_DEC_EQ_USER_8:
        return SCC_AUD_SE_EQ_USER_8;
    case AUD_DEC_EQ_BBE_TV:
        return SCC_AUD_SE_EQ_BBE_TV;
    case AUD_DEC_EQ_BBE_AV:
        return SCC_AUD_SE_EQ_BBE_AV;
    case AUD_DEC_EQ_OFF:
    default:
        return SCC_AUD_SE_EQ_OFF;
    }
}

static VID_PLA_DISP_FMT_T _scc_scr_mode_2_dvr_disp_fmt( SCC_VID_SCREEN_MODE_T  e_scr_mode )
{
    switch( e_scr_mode )
    {
    case SCC_VID_SCREEN_MODE_NORMAL:
        return VID_PLA_DISP_FMT_NORMAL;
         
    case SCC_VID_SCREEN_MODE_LETTERBOX:
        return VID_PLA_DISP_FMT_LETTERBOX;
         
    case SCC_VID_SCREEN_MODE_PAN_SCAN:
        return VID_PLA_DISP_FMT_PAN_SCAN;
         
    case SCC_VID_SCREEN_MODE_NON_LINEAR_ZOOM:
        return VID_PLA_DISP_FMT_NON_LINEAR_ZOOM;
         
    case SCC_VID_SCREEN_MODE_DOT_BY_DOT:
        return VID_PLA_DISP_FMT_DOT_BY_DOT;
         
    case SCC_VID_SCREEN_MODE_CUSTOM_DEF_0:
        return VID_PLA_DISP_FMT_CUSTOM_0;
         
    case SCC_VID_SCREEN_MODE_CUSTOM_DEF_1:
        return VID_PLA_DISP_FMT_CUSTOM_1;
         
    case SCC_VID_SCREEN_MODE_CUSTOM_DEF_2:
        return VID_PLA_DISP_FMT_CUSTOM_2;
         
    case SCC_VID_SCREEN_MODE_CUSTOM_DEF_3:
        return VID_PLA_DISP_FMT_CUSTOM_3;
         
    case SCC_VID_SCREEN_MODE_CUSTOM_DEF_4:
        return VID_PLA_DISP_FMT_CUSTOM_4;
         
    case SCC_VID_SCREEN_MODE_CUSTOM_DEF_5:
        return VID_PLA_DISP_FMT_CUSTOM_5;
         
    case SCC_VID_SCREEN_MODE_CUSTOM_DEF_6:
        return VID_PLA_DISP_FMT_CUSTOM_6;
         
    case SCC_VID_SCREEN_MODE_CUSTOM_DEF_7:
        return VID_PLA_DISP_FMT_CUSTOM_7;
         
    case SCC_VID_SCREEN_MODE_NLZ_CUSTOM_DEF_0:
        return VID_PLA_DISP_FMT_NLZ_CUSTOM_0;
         
    case SCC_VID_SCREEN_MODE_NLZ_CUSTOM_DEF_1:
        return VID_PLA_DISP_FMT_NLZ_CUSTOM_1;
         
    case SCC_VID_SCREEN_MODE_NLZ_CUSTOM_DEF_2:
        return VID_PLA_DISP_FMT_NLZ_CUSTOM_2;
         
    case SCC_VID_SCREEN_MODE_NLZ_CUSTOM_DEF_3:
        return VID_PLA_DISP_FMT_NLZ_CUSTOM_3;
         
    case SCC_VID_SCREEN_MODE_UNKNOWN:
    default:
        return VID_PLA_DISP_FMT_UNKNOWN;
    }
}

static INT32 _scc_comp_open( BOOL                    b_is_grp_set_cmd,
                             SM_COMMAND_T*           pt_cmd_sets,
                             HANDLE_T*               ph_comp,
                             SM_COMMAND_T**          ppt_rest_cmd_sets )
{
    INT32               i4_ret;
    SM_COMMAND_T*       pt_iter;
    SCC_CMD_TYPE_T      e_cmd_type;
    HANDLE_T            h_stream = NULL_HANDLE;
    CHAR*               ps_grp_name = NULL;

    if ( !pt_cmd_sets || !ph_comp || !ppt_rest_cmd_sets )
    {
        return SMR_INV_ARG;
    }

    /* Process command SCC_CMD_CODE_GRP_TYPE */
    *ppt_rest_cmd_sets = pt_iter = pt_cmd_sets;

    if ( pt_iter->e_code != SCC_CMD_CODE_GRP_TYPE )
    {
        return SMR_INV_COMMAND;
    }
    e_cmd_type = (SCC_CMD_TYPE_T)pt_iter->u.ui4_data;

    /* Process command SCC_CMD_CODE_NAME or SCC_CMD_CODE_STRM_HANDLE */
    *ppt_rest_cmd_sets = ++pt_iter;

    if( pt_iter->e_code == SCC_CMD_CODE_NAME )
    {
        ps_grp_name = pt_iter->u.ps_text;
    }
    else if ( pt_iter->e_code == SCC_CMD_CODE_STRM_HANDLE )
    {
        h_stream = pt_iter->u.h_obj;
    }
    else
    {
        return SMR_INV_COMMAND;
    }

    /* Process command SCC_CMD_CODE_END or rest command sets */
    *ppt_rest_cmd_sets = ++pt_iter;

    if ( !b_is_grp_set_cmd && pt_iter->e_code != SM_CMD_CODE_END )
    {
        return SMR_INV_COMMAND;
    }

    /* Open the SCC component */
    switch( e_cmd_type )
    {
    case SCC_CMD_TYPE_VIDEO:
        i4_ret = _scc_comp_vid_open( h_stream, ps_grp_name, ph_comp );
        break;

    case SCC_CMD_TYPE_AUDIO:
        i4_ret = _scc_comp_aud_open( h_stream, ps_grp_name, ph_comp );
        break;

    case SCC_CMD_TYPE_DISP:
        i4_ret = _scc_comp_disp_open( h_stream, ps_grp_name, ph_comp );
        break;

    default:
        return SMR_INV_COMMAND;
    }

    return i4_ret;
}

static INT32 _scc_comp_vid_exec_sub_cmds(
    HANDLE_T                h_comp,
    SM_COMMAND_T*           pt_cmd_sets,
    SM_COMMAND_T**          ppt_rest_cmd_sets )
{
    INT32                      i4_ret = SMR_OK;
    SM_COMMAND_T*              pt_iter = pt_cmd_sets;

    while( pt_iter->e_code != SM_CMD_CODE_END &&
           pt_iter->e_code != SCC_CMD_CODE_GRP_TYPE )
    {
        switch( pt_iter->e_code )
        {
        case SCC_CMD_CODE_VIDEO_BACKGROUND:
            i4_ret = x_scc_vid_set_bg(
                h_comp,
                (SCC_BG_COLOR_T*)pt_iter->u.pv_data );
            break;

        case SCC_CMD_CODE_VIDEO_MODE:
            i4_ret = x_scc_vid_set_mode(
                h_comp,
                (SCC_VID_MODE_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_VIDEO_SCREEN_MODE:
            i4_ret = x_scc_vid_set_scr_mode(
                h_comp,
                (SCC_VID_SCREEN_MODE_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_VIDEO_ENHANCE:
            i4_ret = x_scc_vid_set_enhance(
                h_comp,
                (UINT16)pt_iter->u.ui2_data );
            break;

        case SCC_CMD_CODE_VIDEO_BLUR:
            i4_ret = x_scc_vid_set_blur(
                h_comp,
                (UINT16)pt_iter->u.ui2_data );
            break;

        case SCC_CMD_CODE_VIDEO_OVERSCAN_CLIPPER:
            i4_ret = x_scc_vid_set_overscan_clipper(
                h_comp,
                (SCC_VID_OVER_SCAN_CLIPPER_T*)pt_iter->u.pv_data );
            break;

        case SCC_CMD_CODE_VIDEO_CTI:
            i4_ret = x_scc_vid_set_cti(
                h_comp,
                (UINT16)pt_iter->u.ui2_data );
            break;

        case SCC_CMD_CODE_VIDEO_ETI:
            i4_ret = x_scc_vid_set_eti(
                h_comp,
                (UINT16)pt_iter->u.ui2_data );
            break;

        case SCC_CMD_CODE_VIDEO_SHARPNESS:
            i4_ret = x_scc_vid_set_sharpness(
                h_comp,
                (UINT16)pt_iter->u.ui2_data );
            break;

        case SCC_CMD_CODE_VIDEO_COLOR_SUPPRESS:
            i4_ret = x_scc_vid_set_color_suppress(
                h_comp,
                (UINT16)pt_iter->u.ui2_data );
            break;

        case SCC_CMD_CODE_VIDEO_NR:
            i4_ret = x_scc_vid_set_nr(
                h_comp,
                (UINT16)pt_iter->u.ui2_data );
            break;

        case SCC_CMD_CODE_VIDEO_3D_NR:
            i4_ret = x_scc_vid_set_3dnr(
                h_comp,
                (UINT16)pt_iter->u.ui2_data );
            break;

        case SCC_CMD_CODE_VIDEO_BLACK_LVL_EXT:
            i4_ret = x_scc_vid_set_black_lvl_ext(
                h_comp,
                (UINT16)pt_iter->u.ui2_data );
            break;

        case SCC_CMD_CODE_VIDEO_WHITE_PEAK_RDCT:
            i4_ret = x_scc_vid_set_white_peak_rdct(
                h_comp,
                (UINT16)pt_iter->u.ui2_data );
            break;

        case SCC_CMD_CODE_VIDEO_FLESH_TONE:
            i4_ret = x_scc_vid_set_flesh_tone(
                h_comp,
                (UINT16)pt_iter->u.ui2_data );
            break;

        case SCC_CMD_CODE_VIDEO_LUMA:
            i4_ret = x_scc_vid_set_luma(
                h_comp,
                (UINT16)pt_iter->u.ui2_data );
            break;

        case SCC_CMD_CODE_VIDEO_LETTER_BOX_DETECT:
            i4_ret = x_scc_vid_set_letter_box_detect(
                h_comp,
                (BOOL)pt_iter->u.b_boolean );
            break;

        case SCC_CMD_CODE_VIDEO_DYNAMIC_SCALING:
            i4_ret = x_scc_vid_set_dynamic_scaling(
                h_comp,
                (UINT16)pt_iter->u.ui2_data );
            break;

        case SCC_CMD_CODE_VIDEO_DEBLOCKING:
            i4_ret = x_scc_vid_set_deblocking(
                h_comp,
                (SCC_VID_DEBLOCKING_INFO_T*)pt_iter->u.pv_data );
            break;

        case SCC_CMD_CODE_VIDEO_SPLIT_SCRN_DEMO:
            i4_ret = x_scc_vid_set_split_scrn_demo(
                h_comp,
                (SCC_VID_SPLIT_SCRN_DEMO_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_VIDEO_H_POSITION:
            i4_ret = x_scc_vid_set_position_size(
                h_comp,
                VID_PLA_SET_TYPE_H_POSITION,
                (UINT16)pt_iter->u.ui2_data );
            break;

        case SCC_CMD_CODE_VIDEO_V_POSITION:
            i4_ret = x_scc_vid_set_position_size(
                h_comp,
                VID_PLA_SET_TYPE_V_POSITION,
                (UINT16)pt_iter->u.ui2_data );
            break;

        case SCC_CMD_CODE_VIDEO_H_SIZE:
            i4_ret = x_scc_vid_set_position_size(
                h_comp,
                VID_PLA_SET_TYPE_H_SIZE,
                (UINT16)pt_iter->u.ui2_data );
            break;

        case SCC_CMD_CODE_VIDEO_V_SIZE:
            i4_ret = x_scc_vid_set_position_size(
                h_comp,
                VID_PLA_SET_TYPE_V_SIZE,
                (UINT16)pt_iter->u.ui2_data );
            break;

        case SCC_CMD_CODE_VIDEO_FILM_MODE:
            i4_ret = x_scc_vid_set_film_mode(
                h_comp,
                (BOOL)pt_iter->u.b_boolean );
            break;

        case SCC_CMD_CODE_VIDEO_FORCE_UNMUTE_ON_NO_SIG:
            i4_ret = x_scc_vid_set_force_unmute_on_no_sig(
                h_comp,
                (BOOL)pt_iter->u.b_boolean );
            break;

        case SCC_CMD_CODE_VIDEO_PIC_MODE:
            i4_ret = x_scc_vid_set_pic_mode(
                h_comp,
                (SCC_VID_PIC_MODE_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_VIDEO_HDMI_MODE:
            i4_ret = x_scc_vid_set_hdmi_mode(
                h_comp,
                (SCC_VID_HDMI_MODE_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_VIDEO_COLOR_SYS:
            i4_ret = x_scc_vid_set_color_sys(
                h_comp,
                (SCC_VID_COLOR_SYS_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_VIDEO_LB_DETECT_CONFIG:
            i4_ret = x_scc_vid_set_lb_detect_config(
                h_comp,
                (SCC_VID_LB_DETECT_CONFIG_T*)pt_iter->u.pv_data );
            break;

        case SCC_CMD_CODE_VIDEO_RESET_VGA_TIMING_OPTION:
            i4_ret = x_scc_vid_set_reset_vga_timing_option( h_comp );
            break;

        case SCC_CMD_CODE_VIDEO_AMBIG_VGA_TIMING_CTRL:
            i4_ret = x_scc_vid_set_ambig_vga_timing_ctrl(
                h_comp,
                (BOOL)pt_iter->u.b_boolean );
            break;

        case SCC_CMD_CODE_VIDEO_DI_FILM_MODE:
            i4_ret = x_scc_vid_set_di_film_mode(
                h_comp,
                (SCC_VID_DI_FILM_MODE_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_VIDEO_DI_MA:
            i4_ret = x_scc_vid_set_di_ma(
                h_comp,
                (SCC_VID_DI_MA_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_VIDEO_DI_EDGE_PRESERVE:
            i4_ret = x_scc_vid_set_di_edge_preserve(
                h_comp,
                (SCC_VID_DI_EDGE_PRESERVE_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_VIDEO_MJC:
            i4_ret = x_scc_vid_set_mjc(
                h_comp,
                (SCC_VID_MJC_T*)pt_iter->u.pv_data );
            break;

        case SCC_CMD_CODE_VIDEO_XVYCC:
            i4_ret = x_scc_vid_set_xvycc(
                h_comp,
                (SCC_VID_XVYCC_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_VIDEO_WCG:
            i4_ret = x_scc_vid_set_wcg(
                h_comp,
                (SCC_VID_WCG_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_VIDEO_GAME_MODE:
            i4_ret = x_scc_vid_set_game_mode(
                h_comp,
                (SCC_VID_GAME_MODE_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_VIDEO_BLUE_STRETCH:
            i4_ret = x_scc_vid_set_blue_stretch(
                h_comp,
                (SCC_VID_BLUE_STRETCH_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_VIDEO_MPEG_NR:
            i4_ret = x_scc_vid_set_mpeg_nr(
                h_comp,
                (SCC_VID_MPEG_NR_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_VIDEO_SUPER_BLANK:
            i4_ret = x_scc_vid_set_super_blank(
                h_comp,
                (SCC_VID_SUPER_BLANK_T*)pt_iter->u.pv_data );
            break;

        case SCC_CMD_CODE_VIDEO_MODE_EX:
            i4_ret = x_scc_vid_set_mode_ex(
                h_comp,
                (SCC_VID_MODE_EX_T*)pt_iter->u.pv_data );
            break;

        case SCC_CMD_CODE_VIDEO_SIG_BOOSTER:
            i4_ret = x_scc_vid_set_sig_booster(
                h_comp,
                (BOOL)pt_iter->u.b_boolean );
            break;

        case SCC_CMD_CODE_VIDEO_SIG_BOOSTER_FLAG:
            i4_ret = x_scc_vid_set_sig_booster_flag(
                h_comp,
                (BOOL)pt_iter->u.b_boolean );
            break;

        case SCC_CMD_CODE_VIDEO_BLACK_STRETCH:
            i4_ret = x_scc_vid_set_black_stretch(
                h_comp,
                (SCC_VID_BLACK_STRETCH_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_VIDEO_WHITE_STRETCH:
            i4_ret = x_scc_vid_set_white_stretch(
                h_comp,
                (SCC_VID_WHITE_STRETCH_T)pt_iter->u.ui4_data );
            break;
  
        case SCC_CMD_CODE_VIDEO_BLACK_STRETCH_VAL:
            i4_ret = x_scc_vid_set_black_stretch_val(
                h_comp,
                (UINT16)pt_iter->u.ui2_data );
            break;

        case SCC_CMD_CODE_VIDEO_WHITE_STRETCH_VAL:
            i4_ret = x_scc_vid_set_white_stretch_val(
                h_comp,
                (UINT16)pt_iter->u.ui2_data );
            break;
            
        case SCC_CMD_CODE_VIDEO_DISP_REGION:
            i4_ret = x_scc_vid_set_disp_region(
                h_comp,
                (VSH_REGION_INFO_T*)pt_iter->u.pv_data );
            break;

        case SCC_CMD_CODE_VIDEO_COLORSPACE:
            i4_ret = x_scc_vid_set_colorspace(
                h_comp,
                (SCC_VID_COLORSPACE_T)pt_iter->u.ui4_data );
            break;
            
        case SCC_CMD_CODE_VIDEO_SUPER_FREEZE:
            i4_ret = x_scc_vid_set_super_freeze(
                h_comp,
                (SCC_VID_SUPER_FREEZE_T*)pt_iter->u.pv_data );
            break;
            
        case SCC_CMD_CODE_VIDEO_FLIP:
            i4_ret = x_scc_vid_set_flip(
                h_comp,
                (BOOL)pt_iter->u.b_boolean );
            break;
            
        case SCC_CMD_CODE_VIDEO_MIRROR:
            i4_ret = x_scc_vid_set_mirror(
                h_comp,
                (BOOL)pt_iter->u.b_boolean );
            break;

        case SCC_CMD_CODE_VIDEO_3D_MODE:
            i4_ret = x_scc_vid_set_3d_mode(
                h_comp,
                (SCC_3D_MODE_T)pt_iter->u.ui4_data );
            break;

         case SCC_CMD_CODE_VIDEO_3D_PARALLAX:
            i4_ret = x_scc_vid_set_3d_parallax(
                h_comp,
                (UINT32)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_VIDEO_3D_FLD_DEPTH:
            i4_ret = x_scc_vid_set_3d_fld_depth(
                h_comp,
                (UINT32)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_VIDEO_3D_LR_SWITCH:
            i4_ret = x_scc_vid_set_3d_lr_switch(
                h_comp,
                (SCC_3D_LR_SWITCH_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_VIDEO_3D_TO_2D:
            i4_ret = x_scc_vid_set_3d_to_2d(
                h_comp,
                (SCC_3D_TO_2D_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_VIDEO_3D_FPR:
            i4_ret = x_scc_vid_set_3d_fpr(
                h_comp,
                (SCC_3D_FPR_T)pt_iter->u.ui4_data );
            break;
         
        default:
            return SMR_INV_COMMAND;
        }
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }

        pt_iter++;
    }

    *ppt_rest_cmd_sets = pt_iter;

    return i4_ret;
}

static INT32 _scc_comp_aud_exec_sub_cmds(
    HANDLE_T                h_comp,
    SM_COMMAND_T*           pt_cmd_sets,
    SM_COMMAND_T**          ppt_rest_cmd_sets )
{
    INT32                      i4_ret = SMR_OK;
    SM_COMMAND_T*              pt_iter = pt_cmd_sets;

    while( pt_iter->e_code != SM_CMD_CODE_END &&
           pt_iter->e_code != SCC_CMD_CODE_GRP_TYPE )
    {
        switch( pt_iter->e_code )
        {
        case SCC_CMD_CODE_AUDIO_VOLUME:
            i4_ret = x_scc_aud_set_volume(
                h_comp,
                (SCC_AUD_VOLUME_INFO_T*)pt_iter->u.pv_data );
            break;

        case SCC_CMD_CODE_AUDIO_VOLUME_EX:
            i4_ret = x_scc_aud_set_volume_ex(
                h_comp,
                (SCC_AUD_VOLUME_INFO_T*)pt_iter->u.pv_data );
            break;

        case SCC_CMD_CODE_AUDIO_MUTE:
            if ( pt_iter->u.b_boolean )
            {
                i4_ret = x_scc_aud_mute( h_comp );
            }
            else
            {
                i4_ret = x_scc_aud_unmute( h_comp );
            }
            break;

        case SCC_CMD_CODE_AUDIO_SPDIF_TYPE:
            i4_ret = x_scc_aud_set_spdif_fmt(
                h_comp,
                (SCC_AUD_SPDIF_FMT_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_AUDIO_HDCD_FLTR:
            i4_ret = x_scc_aud_set_hdcd_filter(
                h_comp,
                (SCC_AUD_HDCD_FLTR_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_AUDIO_LOUD_SPEAKER:
            i4_ret = x_scc_aud_set_loudspeaker(
                h_comp,
                (SCC_AUD_LOUD_SPEAKER_T*)pt_iter->u.pv_data );
            break;

        case SCC_CMD_CODE_AUDIO_SE_TRIM:
            i4_ret = x_scc_aud_set_se_trim(
                h_comp,
                (SCC_AUD_SE_TRIM_T*)pt_iter->u.pv_data );
            break;

        case SCC_CMD_CODE_AUDIO_SE_SUPERBASS:
            i4_ret = x_scc_aud_set_se_superbase(
                h_comp,
                pt_iter->u.b_boolean );
            break;

        case SCC_CMD_CODE_AUDIO_SE_EQUALIZER:
            i4_ret = x_scc_aud_set_se_equalizer(
                h_comp,
                (SCC_AUD_SE_EQ_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_AUDIO_SE_REVERB:
            i4_ret = x_scc_aud_set_se_reverb(
                h_comp,
                (SCC_AUD_SE_REVERB_MODE_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_AUDIO_SE_BASS:
            i4_ret = x_scc_aud_set_se_base(
                h_comp,
                pt_iter->u.ui1_data );
            break;

        case SCC_CMD_CODE_AUDIO_SE_TREBLE:
            i4_ret = x_scc_aud_set_se_treble(
                h_comp,
                pt_iter->u.ui1_data );
            break;

        case SCC_CMD_CODE_AUDIO_SE_BALANCE:
            i4_ret = x_scc_aud_set_se_balance(
                h_comp,
                pt_iter->u.ui1_data );
            break;

        case SCC_CMD_CODE_AUDIO_SE_POSTDR:
            i4_ret = x_scc_aud_set_se_postdr(
                h_comp,
                pt_iter->u.b_boolean );
            break;

        case SCC_CMD_CODE_AUDIO_SE_VOLUME:
            i4_ret = x_scc_aud_set_se_volume(
                h_comp,
                (SCC_AUD_VOLUME_INFO_T*)pt_iter->u.pv_data );
            break;

        case SCC_CMD_CODE_AUDIO_SE_SURROUND:
            i4_ret = x_scc_aud_set_se_surround(
                h_comp,
                pt_iter->u.b_boolean );
            break;

        case SCC_CMD_CODE_AUDIO_DOWNMIX_MODE:
            i4_ret = x_scc_aud_set_downmix_mode(
                h_comp,
                (SCC_AUD_DOWNMIX_MODE_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_AUDIO_DOLBY_DRC:
            i4_ret = x_scc_aud_set_dolby_drc(
                h_comp,
                (SCC_AUD_DOLBY_DRC_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_AUDIO_OUT_PORT:
            i4_ret = x_scc_aud_set_output_port_ex(
                h_comp,
                pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_AUDIO_CMPSS_MODE:
            i4_ret = x_scc_aud_set_cmpss_mode(
                h_comp,
                (SCC_AUD_CMPSS_MDOE_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_AUDIO_AGC:
            i4_ret = x_scc_aud_set_agc(
                h_comp,
                pt_iter->u.b_boolean );
            break;

        case SCC_CMD_CODE_AUDIO_CHNL_DELAY:
            i4_ret = x_scc_aud_set_chnl_delay(
                h_comp,
                (SCC_AUD_CHNL_DELAY_T*)pt_iter->u.pv_data );
            break;

        case SCC_CMD_CODE_AUDIO_MTS:
            i4_ret = x_scc_aud_set_mts(
                h_comp,
                (SCC_AUD_MTS_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_AUDIO_SE_EQ_USER_LVL:
            i4_ret = x_scc_aud_set_se_eq_user_lvl(
                h_comp,
                (SCC_AUD_SE_QA_USER_LVL_T*)pt_iter->u.pv_data );
            break;
        case SCC_CMD_CODE_AUDIO_SE_EQ_USER_LVL_EX:
             i4_ret = x_scc_aud_set_se_eq_user_lvl_ex(
                h_comp,
                (SCC_AUD_SE_QA_USER_LVL_T*)pt_iter->u.pv_data );
            break;
        case SCC_CMD_CODE_AUDIO_PL2_SWITCH:
            i4_ret = x_scc_aud_set_pl2_switch(
                h_comp,
                (SCC_AUD_PL2_SWITCH_T)pt_iter->u.ui1_data );
            break;
        case SCC_CMD_CODE_AUDIO_PL2_MODE:
            i4_ret = x_scc_aud_set_pl2_mode(
                h_comp,
                (SCC_AUD_PL2_MODE_T)pt_iter->u.ui1_data );
            break;
        case SCC_CMD_CODE_AUDIO_PL2_PANORAMA:
            i4_ret = x_scc_aud_set_pl2_panorama(
                h_comp,
                (SCC_AUD_PL2_PANORAMA_T)pt_iter->u.ui1_data );
            break;
        case SCC_CMD_CODE_AUDIO_PL2_DIMENSION:
            i4_ret = x_scc_aud_set_pl2_dimension(
                h_comp,
                (SCC_AUD_PL2_DIMENSION_T)pt_iter->u.ui1_data );
            break;
        case SCC_CMD_CODE_AUDIO_PL2_C_WIDTH:
            i4_ret = x_scc_aud_set_pl2_c_width(
                h_comp,
                (SCC_AUD_PL2_C_WIDTH_T)pt_iter->u.ui1_data );
            break;
        case SCC_CMD_CODE_AUDIO_HDPHONE_NFY:
            i4_ret = x_scc_aud_set_hdphone_detect_nfy(
                h_comp,
                (SCC_AUD_HDPHONE_DETECT_NFT_INFO_T*)pt_iter->u.pv_data );
            break;
        case SCC_CMD_CODE_AUDIO_MUTE_EX:
            i4_ret = x_scc_aud_set_mute_ex(
                h_comp,
                (UINT32)pt_iter->u.ui4_data );
            break;
        case SCC_CMD_CODE_AUDIO_SE_SURROUND_EX:
#if 0
                i4_ret = x_scc_aud_set_se_surround_ex(
                h_comp,
                pt_iter->u.ui4_data );
                break;
#endif
            i4_ret = x_scc_aud_set_se_surround_ex(
                h_comp,
                 (SCC_AUD_SE_SURROUND_INFO_T*)pt_iter->u.pv_data );
            break;
        case SCC_CMD_CODE_AUDIO_SE_LINOUT_VOL_MODE:
            i4_ret = x_scc_aud_set_lineout_vol_mode(
                h_comp,
                (SCC_AUD_DEC_LINEOUT_VOL_T*)pt_iter->u.pv_data);
            break;
        case SCC_CMD_CODE_AUDIO_SE_SPH_MODE:
            i4_ret = x_scc_aud_set_se_sph_mode(
                h_comp,
                (SCC_AUD_DEC_SE_SPH_MODE_T)pt_iter->u.ui1_data );
            break;
        case SCC_CMD_CODE_AUDIO_SE_SOUND_MODE:
            i4_ret = x_scc_aud_set_se_sound_mode(
                h_comp,
                (SCC_AUD_SE_SOUND_MODE_T)pt_iter->u.ui1_data );
            break;
        case SCC_CMD_CODE_AUDIO_VOLUME_OFFSET:
            i4_ret = x_scc_aud_set_se_volume_offset(
                h_comp,
                pt_iter->u.ui1_data );
            break;
        case SCC_CMD_CODE_AUDIO_SET_PEQ_INFO:
            i4_ret = x_scc_aud_set_peq_info(
                h_comp,
                (SCC_AUD_DEC_SET_TYPE_PEQ_INFO_T*)pt_iter->u.pv_data);
            break;
        case SCC_CMD_CODE_AUDIO_SET_OVM:
            i4_ret = x_scc_aud_set_ovm(
                h_comp,
                pt_iter->u.ui4_data );
            break;
        case SCC_CMD_CODE_AUDIO_CLIP_VOLUME:
            i4_ret = x_scc_aud_set_clip_volume(
                h_comp,
                (SCC_AUD_VOLUME_INFO_T*)pt_iter->u.pv_data );
            break;
            
        default:
            return SMR_INV_COMMAND;
        }
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }

        pt_iter++;
    }

    *ppt_rest_cmd_sets = pt_iter;

    return i4_ret;
}

static INT32 _scc_comp_disp_exec_sub_cmds(
    HANDLE_T                h_comp,
    SM_COMMAND_T*           pt_cmd_sets,
    SM_COMMAND_T**          ppt_rest_cmd_sets )
{
    INT32                      i4_ret = SMR_OK;
    SM_COMMAND_T*              pt_iter = pt_cmd_sets;

    while( pt_iter->e_code != SM_CMD_CODE_END &&
           pt_iter->e_code != SCC_CMD_CODE_GRP_TYPE )
    {
        switch( pt_iter->e_code )
        {
        case SCC_CMD_CODE_DISP_CTRL:
            i4_ret = x_scc_disp_set_ctrl(
                h_comp,
                (SCC_DISP_CTRL_TYPE_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_DISP_BRIGHTNESS:
            i4_ret = x_scc_disp_set_attrib(
                h_comp,
                SCC_DISP_ATTRIB_BRIGHTNESS,
                (UINT16)pt_iter->u.ui2_data );
            break;

        case SCC_CMD_CODE_DISP_CONTRAST:
            i4_ret = x_scc_disp_set_attrib(
                h_comp,
                SCC_DISP_ATTRIB_CONTRAST,
                (UINT16)pt_iter->u.ui2_data );
            break;

        case SCC_CMD_CODE_DISP_HUE:
            i4_ret = x_scc_disp_set_attrib(
                h_comp,
                SCC_DISP_ATTRIB_HUE,
                (UINT16)pt_iter->u.ui2_data );
            break;

        case SCC_CMD_CODE_DISP_SATURATION:
            i4_ret = x_scc_disp_set_attrib(
                h_comp,
                SCC_DISP_ATTRIB_SATURATION,
                (UINT16)pt_iter->u.ui2_data );
            break;

        case SCC_CMD_CODE_DISP_GAMMA:
            i4_ret = x_scc_disp_set_attrib(
                h_comp,
                SCC_DISP_ATTRIB_GAMMA,
                (UINT16)pt_iter->u.ui2_data );
            break;

        case SCC_CMD_CODE_DISP_RESOLUTION:
            i4_ret = x_scc_disp_set_resolution(
                h_comp,
                (SCC_DISP_RESOLUTION_T*)pt_iter->u.pv_data );
            break;

        case SCC_CMD_CODE_DISP_ASPECT_RATIO:
            i4_ret = x_scc_disp_set_aspect_ratio(
                h_comp,
                (SCC_DISP_ASPECT_RATIO_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_DISP_FMT:
            i4_ret = x_scc_disp_set_fmt(
                h_comp,
                (SCC_DISP_FMT_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_DISP_DIGIT_OUTPUT:
            i4_ret = x_scc_disp_set_digit_output(
                h_comp,
                (SCC_DISP_DIGIT_OUT_MASK_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_DISP_BACKGROUND:
            i4_ret = x_scc_disp_set_bg(
                h_comp,
                (SCC_BG_COLOR_T*)pt_iter->u.pv_data );
            break;

        case SCC_CMD_CODE_DISP_TV_TYPE:
            i4_ret = x_scc_disp_set_tv_type(
                h_comp,
                (SCC_DISP_TV_TYPE_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_DISP_DIGIT_OUTPUT_FMT:
            i4_ret = x_scc_disp_set_digit_output_fmt(
                h_comp,
                (SCC_DISP_DIGIT_OUTPUT_FMT_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_DISP_DITHER:
            i4_ret = x_scc_disp_set_attrib(
                h_comp,
                SCC_DISP_ATTRIB_DITHER,
                pt_iter->u.ui2_data );
            break;

        case SCC_CMD_CODE_DISP_COLOR_GAIN:
            i4_ret = x_scc_disp_set_color_gain(
                h_comp,
                (SCC_DISP_COLOR_GAIN_T*)pt_iter->u.pv_data );
            break;

        case SCC_CMD_CODE_DISP_COLOR_OFFSET:
            i4_ret = x_scc_disp_set_color_offset(
                h_comp,
                (SCC_DISP_COLOR_GAIN_T*)pt_iter->u.pv_data );
            break;

        case SCC_CMD_CODE_DISP_BACK_LIGHT:
            i4_ret = x_scc_disp_set_attrib(
                h_comp,
                SCC_DISP_ATTRIB_BACK_LIGHT,
                pt_iter->u.ui2_data );
            break;

        case SCC_CMD_CODE_DISP_ADP_BACK_LIGHT:
            i4_ret = x_scc_disp_set_attrib(
                h_comp,
                SCC_DISP_ATTRIB_ADP_BACK_LIGHT,
                pt_iter->u.ui2_data );
            break;

        case SCC_CMD_CODE_DISP_TV_MODE:
            i4_ret = x_scc_disp_set_tv_mode(
                h_comp,
                (SCC_TV_MODE_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_DISP_MJC:
            i4_ret = x_scc_disp_set_mjc(
                h_comp,
                (SCC_DISP_MJC_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_DISP_TEST_PATTERN:
            i4_ret = x_scc_disp_set_test_pattern(
                h_comp,
                (SCC_DISP_TEST_PATTERN_T)pt_iter->u.ui4_data );
            break;

        case SCC_CMD_CODE_DISP_BACK_LIGHT_CTRL:
            i4_ret = x_scc_disp_set_back_light_ctrl(
                h_comp,
                (BOOL)pt_iter->u.b_boolean );
            break;

#ifdef MW_TV_AV_BYPASS_SUPPORT
        case SCC_CMD_CODE_DISP_BYPASS_MODE:
            i4_ret = x_scc_disp_set_bypass_mode(
                h_comp,
                (SCC_DISP_BYPASS_MODE_T)pt_iter->u.ui4_data );
            break;
#endif

        case SCC_CMD_CODE_DISP_ADP_BACK_LIGHT_RNG:
            i4_ret = x_scc_disp_set_adp_back_light_rng(
                h_comp,
                (SCC_DISP_RANGE_INFO_T*)pt_iter->u.pv_data );
            break;

        case SCC_CMD_CODE_DISP_BACK_LIGHT_ENERGY_SAVE:
            i4_ret = x_scc_disp_set_attrib(
                h_comp,
                SCC_DISP_ATTRIB_BACK_LIGHT_ENERGY_SAVE,
                pt_iter->u.ui2_data );
            break;

#ifdef MW_TV_AV_BYPASS_SUPPORT
        case SCC_CMD_CODE_DISP_TVE_CTRL_MODE:
            i4_ret = x_scc_disp_set_tve_ctrl_mode(
                h_comp, 
                (SCC_DISP_TVE_CTRL_MODE_T)pt_iter->u.ui4_data);
            break;
#endif

        default:
            return SMR_INV_COMMAND;
        }
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }

        pt_iter++;
    }

    *ppt_rest_cmd_sets = pt_iter;

    return i4_ret;
}

static INT32 _scc_comp_exec_sub_cmds(
    HANDLE_T                h_comp,
    SM_COMMAND_T*           pt_cmd_sets,
    SM_COMMAND_T**          ppt_rest_cmd_sets )
{
    INT32                      i4_ret;
    HANDLE_TYPE_T              e_hdl_type;
    SCC_CMD_TYPE_T             e_cmd_type;

    i4_ret = x_handle_get_info( h_comp, &e_hdl_type, (VOID**)&e_cmd_type );
    if ( i4_ret != HR_OK || e_hdl_type != SM_HT_SCC_GRP_NAME )
    {
        return SMR_INV_HANDLE;
    }
    if ( !pt_cmd_sets || !ppt_rest_cmd_sets )
    {
        return SMR_INV_ARG;
    }

    switch( e_cmd_type )
    {
    case SCC_CMD_TYPE_VIDEO:
        i4_ret = _scc_comp_vid_exec_sub_cmds( h_comp, pt_cmd_sets, ppt_rest_cmd_sets );
        break;

    case SCC_CMD_TYPE_AUDIO:
        i4_ret = _scc_comp_aud_exec_sub_cmds( h_comp, pt_cmd_sets, ppt_rest_cmd_sets );
        break;

    case SCC_CMD_TYPE_DISP:
        i4_ret = _scc_comp_disp_exec_sub_cmds( h_comp, pt_cmd_sets, ppt_rest_cmd_sets );
        break;

    default:
        return SMR_INV_HANDLE;
    }

    return i4_ret;
}

static INT32 _scc_comp_att_get_min_max_curr(
    HANDLE_T            h_comp,
    SM_CMD_CODE_T       e_scc_cmd_code,
    VOID*               pv_cmd_arg,
    UINT16*             pui2_min,
    UINT16*             pui2_max,
    UINT16*             pui2_curr )
{
    INT32      i4_ret;

    switch( e_scc_cmd_code )
    {
    case SCC_CMD_CODE_VIDEO_ENHANCE:
        i4_ret = _scc_comp_vid_get_min_max_curr(
            h_comp,
            DRVT_VID_PLANE,
            VID_PLA_GET_TYPE_ENHANCE,
            (VOID*)(UINT32)VID_PLA_ENHANCE,
            pui2_min,
            pui2_max,
            pui2_curr );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
        if ( (pui2_min && (*pui2_min > 0xFF)) ||
             (pui2_max && (*pui2_max > 0xFF)) )
        {
            return SMR_DRV_COMP_ERROR;
        }
        break;

    case SCC_CMD_CODE_VIDEO_BLUR:
        i4_ret = _scc_comp_vid_get_min_max_curr(
            h_comp,
            DRVT_VID_PLANE,
            VID_PLA_GET_TYPE_ENHANCE,
            (VOID*)(UINT32) VID_PLA_BLUR,
            pui2_min,
            pui2_max,
            pui2_curr );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
        if ( (pui2_min && (*pui2_min > 0xFF)) ||
             (pui2_max && (*pui2_max > 0xFF)) )
        {
            return SMR_DRV_COMP_ERROR;
        }
        break;

    case SCC_CMD_CODE_VIDEO_CTI:
        i4_ret = _scc_comp_vid_get_min_max_curr(
            h_comp,
            DRVT_VID_PLANE,
            VID_PLA_GET_TYPE_CTI,
            pv_cmd_arg,
            pui2_min,
            pui2_max,
            pui2_curr );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
        if ( (pui2_min && (*pui2_min > 0xFF)) ||
             (pui2_max && (*pui2_max > 0xFF)) )
        {
            return SMR_DRV_COMP_ERROR;
        }
        break;

    case SCC_CMD_CODE_VIDEO_ETI:
        i4_ret = _scc_comp_vid_get_min_max_curr(
            h_comp,
            DRVT_VID_PLANE,
            VID_PLA_GET_TYPE_ETI,
            pv_cmd_arg,
            pui2_min,
            pui2_max,
            pui2_curr );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
        if ( (pui2_min && (*pui2_min > 0xFF)) ||
             (pui2_max && (*pui2_max > 0xFF)) )
        {
            return SMR_DRV_COMP_ERROR;
        }
        break;

    case SCC_CMD_CODE_VIDEO_SHARPNESS:
        i4_ret = _scc_comp_vid_get_min_max_curr(
            h_comp,
            DRVT_VID_PLANE,
            VID_PLA_GET_TYPE_SHARPNESS,
            pv_cmd_arg,
            pui2_min,
            pui2_max,
            pui2_curr );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
        if ( (pui2_min && (*pui2_min > 0xFF)) ||
             (pui2_max && (*pui2_max > 0xFF)) )
        {
            return SMR_DRV_COMP_ERROR;
        }
        break;

    case SCC_CMD_CODE_VIDEO_COLOR_SUPPRESS:
        i4_ret = _scc_comp_vid_get_min_max_curr(
            h_comp,
            DRVT_VID_PLANE,
            VID_PLA_GET_TYPE_COLOR_SUPPRESS,
            pv_cmd_arg,
            pui2_min,
            pui2_max,
            pui2_curr );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
        if ( (pui2_min && (*pui2_min > 0xFF)) ||
             (pui2_max && (*pui2_max > 0xFF)) )
        {
            return SMR_DRV_COMP_ERROR;
        }
        break;

    case SCC_CMD_CODE_VIDEO_NR:
        i4_ret = _scc_comp_vid_get_min_max_curr(
            h_comp,
            DRVT_VID_PLANE,
            VID_PLA_GET_TYPE_NR,
            pv_cmd_arg,
            pui2_min,
            pui2_max,
            pui2_curr );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
        if ( (pui2_min && (*pui2_min > 0xFF)) ||
             (pui2_max && (*pui2_max > 0xFF)) )
        {
            return SMR_DRV_COMP_ERROR;
        }
        break;

    case SCC_CMD_CODE_VIDEO_3D_NR:
        i4_ret = _scc_comp_vid_get_min_max_curr(
            h_comp,
            DRVT_VID_PLANE,
            VID_PLA_GET_TYPE_3D_NR,
            pv_cmd_arg,
            pui2_min,
            pui2_max,
            pui2_curr );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
        if ( (pui2_min && (*pui2_min > 0xFF)) ||
             (pui2_max && (*pui2_max > 0xFF)) )
        {
            return SMR_DRV_COMP_ERROR;
        }
        break;

    case SCC_CMD_CODE_VIDEO_BLACK_LVL_EXT:
        i4_ret = _scc_comp_vid_get_min_max_curr(
            h_comp,
            DRVT_VID_PLANE,
            VID_PLA_GET_TYPE_BLACK_LVL_EXT,
            pv_cmd_arg,
            pui2_min,
            pui2_max,
            pui2_curr );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
        if ( (pui2_min && (*pui2_min > 0xFF)) ||
             (pui2_max && (*pui2_max > 0xFF)) )
        {
            return SMR_DRV_COMP_ERROR;
        }
        break;

    case SCC_CMD_CODE_VIDEO_WHITE_PEAK_RDCT:
        i4_ret = _scc_comp_vid_get_min_max_curr(
            h_comp,
            DRVT_VID_PLANE,
            VID_PLA_GET_TYPE_WHITE_PEAK_LMT,
            pv_cmd_arg,
            pui2_min,
            pui2_max,
            pui2_curr );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
        if ( (pui2_min && (*pui2_min > 0xFF)) ||
             (pui2_max && (*pui2_max > 0xFF)) )
        {
            return SMR_DRV_COMP_ERROR;
        }
        break;

    case SCC_CMD_CODE_VIDEO_FLESH_TONE:
        i4_ret = _scc_comp_vid_get_min_max_curr(
            h_comp,
            DRVT_VID_PLANE,
            VID_PLA_GET_TYPE_FLESH_TONE,
            pv_cmd_arg,
            pui2_min,
            pui2_max,
            pui2_curr );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
        if ( (pui2_min && (*pui2_min > 0xFF)) ||
             (pui2_max && (*pui2_max > 0xFF)) )
        {
            return SMR_DRV_COMP_ERROR;
        }
        break;

    case SCC_CMD_CODE_VIDEO_LUMA:
        i4_ret = _scc_comp_vid_get_min_max_curr(
            h_comp,
            DRVT_VID_PLANE,
            VID_PLA_GET_TYPE_LUMA,
            pv_cmd_arg,
            pui2_min,
            pui2_max,
            pui2_curr );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
        if ( (pui2_min && (*pui2_min > 0xFF)) ||
             (pui2_max && (*pui2_max > 0xFF)) )
        {
            return SMR_DRV_COMP_ERROR;
        }
        break;

    case SCC_CMD_CODE_VIDEO_H_POSITION:
        i4_ret = _scc_comp_vid_get_min_max_curr(
            h_comp,
            DRVT_VID_PLANE,
            VID_PLA_GET_TYPE_H_POSITION,
            pv_cmd_arg,
            pui2_min,
            pui2_max,
            pui2_curr );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
        if ( (pui2_min && (*pui2_min > 0xFF)) ||
             (pui2_max && (*pui2_max > 0xFF)) )
        {
            return SMR_DRV_COMP_ERROR;
        }
        break;

    case SCC_CMD_CODE_VIDEO_V_POSITION:
        i4_ret = _scc_comp_vid_get_min_max_curr(
            h_comp,
            DRVT_VID_PLANE,
            VID_PLA_GET_TYPE_V_POSITION,
            pv_cmd_arg,
            pui2_min,
            pui2_max,
            pui2_curr );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
        if ( (pui2_min && (*pui2_min > 0xFF)) ||
             (pui2_max && (*pui2_max > 0xFF)) )
        {
            return SMR_DRV_COMP_ERROR;
        }
        break;

    case SCC_CMD_CODE_VIDEO_H_SIZE:
        i4_ret = _scc_comp_vid_get_min_max_curr(
            h_comp,
            DRVT_VID_PLANE,
            VID_PLA_GET_TYPE_H_SIZE,
            pv_cmd_arg,
            pui2_min,
            pui2_max,
            pui2_curr );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
        if ( (pui2_min && (*pui2_min > 0xFF)) ||
             (pui2_max && (*pui2_max > 0xFF)) )
        {
            return SMR_DRV_COMP_ERROR;
        }
        break;

    case SCC_CMD_CODE_VIDEO_V_SIZE:
        i4_ret = _scc_comp_vid_get_min_max_curr(
            h_comp,
            DRVT_VID_PLANE,
            VID_PLA_GET_TYPE_V_SIZE,
            pv_cmd_arg,
            pui2_min,
            pui2_max,
            pui2_curr );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
        if ( (pui2_min && (*pui2_min > 0xFF)) ||
             (pui2_max && (*pui2_max > 0xFF)) )
        {
            return SMR_DRV_COMP_ERROR;
        }
        break;

    case SCC_CMD_CODE_DISP_BRIGHTNESS:
        i4_ret = _scc_comp_disp_get_min_max_curr(
            h_comp,
            DRVT_PLA_MXR,
            PLA_MXR_GET_TYPE_BRIGHTNESS,
            pv_cmd_arg,
            pui2_min,
            pui2_max,
            pui2_curr );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
        break;

    case SCC_CMD_CODE_DISP_CONTRAST:
        i4_ret = _scc_comp_disp_get_min_max_curr(
            h_comp,
            DRVT_PLA_MXR,
            PLA_MXR_GET_TYPE_CONTRAST,
            pv_cmd_arg,
            pui2_min,
            pui2_max,
            pui2_curr );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
        break;

    case SCC_CMD_CODE_DISP_HUE:
        i4_ret = _scc_comp_disp_get_min_max_curr(
            h_comp,
            DRVT_PLA_MXR,
            PLA_MXR_GET_TYPE_HUE,
            pv_cmd_arg,
            pui2_min,
            pui2_max,
            pui2_curr );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
        break;

    case SCC_CMD_CODE_DISP_SATURATION:
        i4_ret = _scc_comp_disp_get_min_max_curr(
            h_comp,
            DRVT_PLA_MXR,
            PLA_MXR_GET_TYPE_SATURATION,
            pv_cmd_arg,
            pui2_min,
            pui2_max,
            pui2_curr );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
        break;

    case SCC_CMD_CODE_DISP_COLOR_GAIN:
        i4_ret = _scc_comp_disp_get_min_max_curr(
            h_comp,
            DRVT_PLA_MXR,
            PLA_MXR_GET_TYPE_COLOR_GAIN,
            pv_cmd_arg,
            pui2_min,
            pui2_max,
            pui2_curr );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
        break;

    case SCC_CMD_CODE_DISP_COLOR_OFFSET:
        i4_ret = _scc_comp_disp_get_min_max_curr(
            h_comp,
            DRVT_PLA_MXR,
            PLA_MXR_GET_TYPE_COLOR_OFFSET,
            pv_cmd_arg,
            pui2_min,
            pui2_max,
            pui2_curr );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
        break;

    case SCC_CMD_CODE_DISP_BACK_LIGHT:
        i4_ret = _scc_comp_disp_get_min_max_curr(
            h_comp,
            DRVT_PLA_MXR,
            PLA_MXR_GET_TYPE_BACK_LIGHT,
            pv_cmd_arg,
            pui2_min,
            pui2_max,
            pui2_curr );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
        break;

    case SCC_CMD_CODE_VIDEO_BLACK_STRETCH_VAL:
        i4_ret = _scc_comp_vid_get_min_max_curr(
            h_comp,
            DRVT_VID_PLANE,
            VID_PLA_GET_TYPE_BLACK_STRETCH,
            pv_cmd_arg,
            pui2_min,
            pui2_max,
            pui2_curr );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
        if ( (pui2_min && (*pui2_min > 0xFF)) ||
             (pui2_max && (*pui2_max > 0xFF)) )
        {
            return SMR_DRV_COMP_ERROR;
        }
        break;

    case SCC_CMD_CODE_VIDEO_WHITE_STRETCH_VAL:
        i4_ret = _scc_comp_vid_get_min_max_curr(
            h_comp,
            DRVT_VID_PLANE,
            VID_PLA_GET_TYPE_WHITE_STRETCH,
            pv_cmd_arg,
            pui2_min,
            pui2_max,
            pui2_curr );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
        if ( (pui2_min && (*pui2_min > 0xFF)) ||
             (pui2_max && (*pui2_max > 0xFF)) )
        {
            return SMR_DRV_COMP_ERROR;
        }
        break;

    default:
        return SMR_NOT_ALLOW;
    }

    return SMR_OK;
}

static INT32 _scc_comp_att_set_value(
    HANDLE_T            h_comp,
    SM_CMD_CODE_T       e_scc_cmd_code,
    VOID*               pv_cmd_arg,
    UINT16              ui2_scc_value )
{
    INT32      i4_ret;

    SM_SLCTR_DBG_INFO(( "[SCC] In _scc_comp_att_set_value(cmd=%u, val=%u)\r\n",
                        e_scc_cmd_code, ui2_scc_value ));

    switch( e_scc_cmd_code )
    {
    case SCC_CMD_CODE_VIDEO_ENHANCE:
        i4_ret = x_scc_vid_set_enhance(
            h_comp,
            ui2_scc_value );
        break;

    case SCC_CMD_CODE_VIDEO_BLUR:
        i4_ret = x_scc_vid_set_blur(
            h_comp,
            ui2_scc_value );
        break;

    case SCC_CMD_CODE_VIDEO_CTI:
        i4_ret = x_scc_vid_set_cti(
            h_comp,
            ui2_scc_value );
        break;

    case SCC_CMD_CODE_VIDEO_ETI:
        i4_ret = x_scc_vid_set_eti(
            h_comp,
            ui2_scc_value );
        break;

    case SCC_CMD_CODE_VIDEO_SHARPNESS:
        i4_ret = x_scc_vid_set_sharpness(
            h_comp,
            ui2_scc_value );
        break;

    case SCC_CMD_CODE_VIDEO_COLOR_SUPPRESS:
        i4_ret = x_scc_vid_set_color_suppress(
            h_comp,
            ui2_scc_value );
        break;

    case SCC_CMD_CODE_VIDEO_NR:
        i4_ret = x_scc_vid_set_nr(
            h_comp,
            ui2_scc_value );
        break;

    case SCC_CMD_CODE_VIDEO_3D_NR:
        i4_ret = x_scc_vid_set_3dnr(
            h_comp,
            ui2_scc_value );
        break;

    case SCC_CMD_CODE_VIDEO_BLACK_LVL_EXT:
        i4_ret = x_scc_vid_set_black_lvl_ext(
            h_comp,
            ui2_scc_value );
        break;

    case SCC_CMD_CODE_VIDEO_WHITE_PEAK_RDCT:
        i4_ret = x_scc_vid_set_white_peak_rdct(
            h_comp,
            ui2_scc_value );
        break;

    case SCC_CMD_CODE_VIDEO_FLESH_TONE:
        i4_ret = x_scc_vid_set_flesh_tone(
            h_comp,
            ui2_scc_value );
        break;

    case SCC_CMD_CODE_VIDEO_LUMA:
        i4_ret = x_scc_vid_set_luma(
            h_comp,
            ui2_scc_value );
        break;

    case SCC_CMD_CODE_DISP_BRIGHTNESS:
        i4_ret = x_scc_disp_set_attrib(
            h_comp,
            SCC_DISP_ATTRIB_BRIGHTNESS,
            ui2_scc_value );
        break;

    case SCC_CMD_CODE_DISP_CONTRAST:
        i4_ret = x_scc_disp_set_attrib(
            h_comp,
            SCC_DISP_ATTRIB_CONTRAST,
            ui2_scc_value );
        break;

    case SCC_CMD_CODE_DISP_HUE:
        i4_ret = x_scc_disp_set_attrib(
            h_comp,
            SCC_DISP_ATTRIB_HUE,
            ui2_scc_value );
        break;

    case SCC_CMD_CODE_DISP_SATURATION:
        i4_ret = x_scc_disp_set_attrib(
            h_comp,
            SCC_DISP_ATTRIB_SATURATION,
            ui2_scc_value );
        break;

    case SCC_CMD_CODE_DISP_COLOR_GAIN:
        {
            SCC_DISP_COLOR_GAIN_T      t_color_gain;
            SCC_DISP_COLOR_GAIN_ARG_T  e_cg_arg =
                (SCC_DISP_COLOR_GAIN_ARG_T)(UINT32)pv_cmd_arg;

            i4_ret = x_scc_disp_get_color_gain(
                h_comp,
                &t_color_gain );
            if ( i4_ret != SMR_OK )
            {
                return i4_ret;
            }

            if ( e_cg_arg == SCC_DISP_COLOR_GAIN_ARG_R )
            {
                t_color_gain.ui2_r_gain = ui2_scc_value;
            }
            else if ( e_cg_arg == SCC_DISP_COLOR_GAIN_ARG_G )
            {
                t_color_gain.ui2_g_gain = ui2_scc_value;
            }
            else if ( e_cg_arg == SCC_DISP_COLOR_GAIN_ARG_B )
            {
                t_color_gain.ui2_b_gain = ui2_scc_value;
            }
            else
            {
                return SMR_INV_ARG;
            }

            i4_ret = x_scc_disp_set_color_gain(
                h_comp,
                &t_color_gain );
        }
        break;

    case SCC_CMD_CODE_DISP_COLOR_OFFSET:
        {
            SCC_DISP_COLOR_GAIN_T      t_color_gain;
            SCC_DISP_COLOR_GAIN_ARG_T  e_cg_arg =
                (SCC_DISP_COLOR_GAIN_ARG_T)(UINT32)pv_cmd_arg;

            i4_ret = x_scc_disp_get_color_offset(
                h_comp,
                &t_color_gain );
            if ( i4_ret != SMR_OK )
            {
                return i4_ret;
            }

            if ( e_cg_arg == SCC_DISP_COLOR_GAIN_ARG_R )
            {
                t_color_gain.ui2_r_gain = ui2_scc_value;
            }
            else if ( e_cg_arg == SCC_DISP_COLOR_GAIN_ARG_G )
            {
                t_color_gain.ui2_g_gain = ui2_scc_value;
            }
            else if ( e_cg_arg == SCC_DISP_COLOR_GAIN_ARG_B )
            {
                t_color_gain.ui2_b_gain = ui2_scc_value;
            }
            else
            {
                return SMR_INV_ARG;
            }

            i4_ret = x_scc_disp_set_color_offset(
                h_comp,
                &t_color_gain );
        }
        break;

    case SCC_CMD_CODE_DISP_BACK_LIGHT:
        i4_ret = x_scc_disp_set_attrib(
            h_comp,
            SCC_DISP_ATTRIB_BACK_LIGHT,
            ui2_scc_value );
        break;

    case SCC_CMD_CODE_DISP_ADP_BACK_LIGHT:
        i4_ret = x_scc_disp_set_attrib(
            h_comp,
            SCC_DISP_ATTRIB_ADP_BACK_LIGHT,
            ui2_scc_value );
        break;

    case SCC_CMD_CODE_VIDEO_H_POSITION:
        i4_ret = x_scc_vid_set_position_size(
            h_comp,
            VID_PLA_SET_TYPE_H_POSITION,
            ui2_scc_value );
        break;

    case SCC_CMD_CODE_VIDEO_V_POSITION:
        i4_ret = x_scc_vid_set_position_size(
            h_comp,
            VID_PLA_SET_TYPE_V_POSITION,
            ui2_scc_value );
        break;

    case SCC_CMD_CODE_VIDEO_H_SIZE:
        i4_ret = x_scc_vid_set_position_size(
            h_comp,
            VID_PLA_SET_TYPE_H_SIZE,
            ui2_scc_value );
        break;

    case SCC_CMD_CODE_VIDEO_V_SIZE:
        i4_ret = x_scc_vid_set_position_size(
            h_comp,
            VID_PLA_SET_TYPE_V_SIZE,
            ui2_scc_value );
        break;

    case SCC_CMD_CODE_VIDEO_BLACK_STRETCH_VAL:
        i4_ret = x_scc_vid_set_black_stretch_val(
            h_comp,
            ui2_scc_value );
        break;

    case SCC_CMD_CODE_VIDEO_WHITE_STRETCH_VAL:
        i4_ret = x_scc_vid_set_white_stretch_val(
            h_comp,
            ui2_scc_value );
        break;
        
    default:
        return SMR_NOT_ALLOW;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: x_scc_comp_close
 *
 * Description: <function description>
 *
 * Inputs:  h_comp              References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_comp_close( HANDLE_T    h_comp )
{
    INT32                      i4_ret;
    HANDLE_TYPE_T              e_hdl_type;
    SCC_CMD_TYPE_T             e_cmd_type;

    i4_ret = x_handle_get_info( h_comp, &e_hdl_type, (VOID**)&e_cmd_type );
    if ( i4_ret != HR_OK || e_hdl_type != SM_HT_SCC_GRP_NAME )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_cmd_type )
    {
    case SCC_CMD_TYPE_VIDEO:
        i4_ret = _scc_comp_vid_close( h_comp );
        break;

    case SCC_CMD_TYPE_AUDIO:
        i4_ret = _scc_comp_aud_close( h_comp );
        break;

    case SCC_CMD_TYPE_DISP:
        i4_ret = _scc_comp_disp_close( h_comp );
        break;

    default:
        return SMR_INV_HANDLE;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_comp_open
 *
 * Description: <function description>
 *
 * Inputs:  pt_cmd_sets         References.
 *
 * Outputs: ph_comp             References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_comp_open( SM_COMMAND_T*           pt_cmd_sets,
                       HANDLE_T*               ph_comp )
{
    SM_COMMAND_T*       pt_rest_cmd_sets = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:( pt_cmd_sets = 0x%.8X, ph_comp = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)pt_cmd_sets,
                       (UINT32)ph_comp ));

    return _scc_comp_open( FALSE, pt_cmd_sets, ph_comp, &pt_rest_cmd_sets );
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_comp_grp_set
 *
 * Description: <function description>
 *
 * Inputs:  pt_cmd_sets         References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_comp_grp_set( SM_COMMAND_T*        pt_cmd_sets )
{
    HANDLE_T        h_comp;
    INT32           i4_ret;
    SM_COMMAND_T*   pt_iter = pt_cmd_sets;

    SM_SLCTR_DBG_API(( "[SM] %s:( pt_cmd_sets = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)pt_cmd_sets ));

    SM_SLCTR_DBG_API(( "[SM] %s:( pt_cmd_sets = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)pt_cmd_sets ));

    if ( !pt_cmd_sets )
    {
        return SMR_INV_COMMAND;
    }

    while ( pt_iter && pt_iter->e_code != SM_CMD_CODE_END )
    {
        if ( (pt_iter->e_code & SM_CMD_CODE_PRIMARY_BIT_FLAG) )
        {
            i4_ret = _scc_comp_open( TRUE, pt_iter, &h_comp, &pt_iter );
            if ( i4_ret != SMR_OK )
            {
                return i4_ret;
            }

            /* process rest commands */
            if ( pt_iter &&
                 pt_iter->e_code != SM_CMD_CODE_END )
            {
                i4_ret = _scc_comp_exec_sub_cmds( h_comp, pt_iter, &pt_iter );
                if ( i4_ret != SMR_OK )
                {
                    x_scc_comp_close( h_comp );
                    return i4_ret;
                }
            }

            i4_ret = x_scc_comp_close( h_comp );
            if ( i4_ret != SMR_OK )
            {
                return i4_ret;
            }
        }
        else
        {
            return SMR_INV_COMMAND;
        }
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_comp_att_incr
 *
 * Description: <function description>
 *
 * Inputs:  h_comp          References.
 *          e_scc_cmd_code  References.
 *          pv_cmd_arg      References.
 *
 * Outputs: pui4_curr_value References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_comp_att_incr(
    HANDLE_T            h_comp,
    SM_CMD_CODE_T       e_scc_cmd_code,
    VOID*               pv_cmd_arg,
    UINT16*             pui2_curr_value )
{
    INT32               i4_ret;
    UINT16              ui2_min, ui2_max, ui2_curr;
    UINT16              ui2_scc_value;

    if ( !pui2_curr_value )
    {
        return SMR_INV_ARG;
    }

    i4_ret = _scc_comp_att_get_min_max_curr(
        h_comp,
        e_scc_cmd_code,
        pv_cmd_arg,
        &ui2_min,
        &ui2_max,
        &ui2_curr );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    if ( ui2_curr < ui2_max )
    {
        ++ui2_curr;

        ui2_scc_value = (UINT16)_floor_16bits_drv_val_2_scc_val(
            SCC_ATTRIB_VALUE_MAX,
            ui2_curr,
            ui2_min,
            ui2_max );

        i4_ret = _scc_comp_att_set_value(
            h_comp,
            e_scc_cmd_code,
            pv_cmd_arg,
            ui2_scc_value );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
    }
    else
    {
        ui2_scc_value = (UINT16)_floor_16bits_drv_val_2_scc_val(
            SCC_ATTRIB_VALUE_MAX,
            ui2_curr,
            ui2_min,
            ui2_max );
    }

    *pui2_curr_value = ui2_scc_value;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_comp_att_decr
 *
 * Description: <function description>
 *
 * Inputs:  h_comp          References.
 *          e_scc_cmd_code  References.
 *          pv_cmd_arg      References.
 *
 * Outputs: pui4_curr_value References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_comp_att_decr(
    HANDLE_T            h_comp,
    SM_CMD_CODE_T       e_scc_cmd_code,
    VOID*               pv_cmd_arg,
    UINT16*             pui2_curr_value )
{
    INT32               i4_ret;
    UINT16              ui2_min, ui2_max, ui2_curr;
    UINT16              ui2_scc_value;

    if ( !pui2_curr_value )
    {
        return SMR_INV_ARG;
    }

    i4_ret = _scc_comp_att_get_min_max_curr(
        h_comp,
        e_scc_cmd_code,
        pv_cmd_arg,
        &ui2_min,
        &ui2_max,
        &ui2_curr );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    if ( ui2_curr > ui2_min )
    {
        --ui2_curr;

        ui2_scc_value = (UINT16)_floor_16bits_drv_val_2_scc_val(
            SCC_ATTRIB_VALUE_MAX,
            ui2_curr,
            ui2_min,
            ui2_max );

        i4_ret = _scc_comp_att_set_value(
            h_comp,
            e_scc_cmd_code,
            pv_cmd_arg,
            ui2_scc_value );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
    }
    else
    {
        ui2_scc_value = (UINT16)_floor_16bits_drv_val_2_scc_val(
            SCC_ATTRIB_VALUE_MAX,
            ui2_curr,
            ui2_min,
            ui2_max );
    }

    *pui2_curr_value = ui2_scc_value;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_comp_att_get_num_steps
 *
 * Description: <function description>
 *
 * Inputs:  h_comp          References.
 *          e_scc_cmd_code  References.
 *          pv_cmd_arg      References.
 *
 * Outputs: pui4_num_steps  References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INV_ARG         The argument pui4_max_steps is invalid.
 *          SMR_NOT_ALLOW       The e_scc_cmd_code is not allowed to get MAX
 *                              steps.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_comp_att_get_num_steps(
    HANDLE_T            h_comp,
    SM_CMD_CODE_T       e_scc_cmd_code,
    VOID*               pv_cmd_arg,
    UINT16*             pui2_num_steps )
{
    INT32                        i4_ret;
    UINT16                       ui2_min, ui2_max;

    if ( !pui2_num_steps )
    {
        return SMR_INV_ARG;
    }

    i4_ret = _scc_comp_att_get_min_max_curr(
        h_comp,
        e_scc_cmd_code,
        pv_cmd_arg,
        &ui2_min,
        &ui2_max,
        NULL );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    *pui2_num_steps = ui2_max - ui2_min;

    return SMR_OK;
}


/* Video related API *********************************************************/
/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          e_mode              References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_mode( HANDLE_T                   h_vid,
                          SCC_VID_MODE_T             e_mode )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_MODE_T               e_drv_vdp_mode = VID_PLA_NORMAL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\te_mode           = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)e_mode ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_mode )
    {
    case SCC_VID_NORMAL:
        e_drv_vdp_mode = VID_PLA_NORMAL;
        break;
    case SCC_VID_BLANK:
        e_drv_vdp_mode = VID_PLA_BLANK;
        break;
    case SCC_VID_QUAD_VIDEO:
        e_drv_vdp_mode = VID_PLA_QUAD_VIDEO;
        break;
    case SCC_VID_DEINT:
        e_drv_vdp_mode = VID_PLA_DEINT;
        break;
    case SCC_VID_FREEZE:
        e_drv_vdp_mode = VID_PLA_FREEZE;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_MODE,
        (VOID*)(UINT32)e_drv_vdp_mode,
        sizeof(VID_PLA_MODE_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_NOT_SUPPORT;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_mode_ex
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          pt_mode             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_mode_ex( HANDLE_T                   h_vid,
                             const SCC_VID_MODE_EX_T*   pt_mode )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_MODE_T               e_drv_vdp_mode = VID_PLA_NORMAL;
    VID_PLA_VID_REGION_T         t_drv_region = {0};

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\te_mode           = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)pt_mode->e_mode ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( pt_mode->e_mode )
    {
    case SCC_VID_NORMAL:
        e_drv_vdp_mode = VID_PLA_NORMAL;
        break;
    case SCC_VID_BLANK:
        e_drv_vdp_mode = VID_PLA_BLANK;
        break;
    case SCC_VID_QUAD_VIDEO:
        e_drv_vdp_mode = VID_PLA_QUAD_VIDEO;
        break;
    case SCC_VID_DEINT:
        e_drv_vdp_mode = VID_PLA_DEINT;
        break;
    case SCC_VID_FREEZE:
        e_drv_vdp_mode = VID_PLA_FREEZE;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_MODE,
        (VOID*)(UINT32)e_drv_vdp_mode,
        sizeof(VID_PLA_MODE_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_NOT_SUPPORT;
    }

    t_drv_region.ui4_x = pt_mode->t_disp_region.ui4_x;
    t_drv_region.ui4_y = pt_mode->t_disp_region.ui4_y;
    t_drv_region.ui4_height = pt_mode->t_disp_region.ui4_height;
    t_drv_region.ui4_width = pt_mode->t_disp_region.ui4_width;

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_DISP_REGION,
        (VOID*)&t_drv_region,
        sizeof(VID_PLA_VID_REGION_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_NOT_SUPPORT;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_sig_booster
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          b_is_on             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_sig_booster(
    HANDLE_T                   h_vid,
    BOOL                       b_is_on )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tb_is_on           = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)b_is_on ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_TV_DEC,
        TV_DEC_SET_TYPE_SIG_BOOSTER,
        (VOID*)(UINT32)b_is_on,
        sizeof(BOOL) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_sig_booster_flag
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          b_is_on             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_sig_booster_flag(
    HANDLE_T                   h_vid,
    BOOL                       b_is_on )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tb_is_on           = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)b_is_on ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_TV_DEC,
        TV_DEC_SET_TYPE_SWITCH_SIG_BOOSTER,
        (VOID*)(UINT32)b_is_on,
        sizeof(BOOL) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_bg
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          pt_bg               References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_bg( HANDLE_T                     h_vid,
                        const SCC_BG_COLOR_T*        pt_bg )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_BG_COLOR_T           t_drv_vdp_bg_color = {0};

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tpt_bg            = 0x%.8X<R(%u),G(%u),B(%u)> )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pt_bg,
                       ((pt_bg)?pt_bg->ui1_red:(-1)),
                       ((pt_bg)?pt_bg->ui1_green:(-1)),
                       ((pt_bg)?pt_bg->ui1_blue:(-1)) ));

    if ( !pt_bg )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_vdp_bg_color.ui1_r = pt_bg->ui1_red;
    t_drv_vdp_bg_color.ui1_g = pt_bg->ui1_green;
    t_drv_vdp_bg_color.ui1_b = pt_bg->ui1_blue;

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_BG,
        (VOID*)&t_drv_vdp_bg_color,
        sizeof(VID_PLA_BG_COLOR_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_super_blank
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          pt_blank            References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_super_blank( HANDLE_T                     h_vid,
                                 const SCC_VID_SUPER_BLANK_T* pt_blank )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tb_is_on           = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pt_blank->b_enable ));
    
    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    if ( pt_blank->b_enable )
    {
        i4_ret = _scc_mute_build_comp_connect( h_vid, pt_blank );
        if ( i4_ret != SMR_OK )
        {
            return SMR_INV_SET_INFO;
        }
    }
    else
    {

        i4_ret = _scc_mute_teardown_comp_connect( h_vid );
        if ( i4_ret != SMR_OK )
        {
            return SMR_INV_SET_INFO;
        }
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_pic_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          e_pic_mode          References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_pic_mode( HANDLE_T               h_vid,
                              SCC_VID_PIC_MODE_T     e_pic_mode )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_PIC_MODE_T           e_drv_vdp_pic_mode = VID_PLA_PIC_MODE_DEFAULT;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\te_pic_mode       = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)e_pic_mode ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_pic_mode )
    {
    case SCC_VID_PIC_MODE_DEFAULT:
        e_drv_vdp_pic_mode = VID_PLA_PIC_MODE_DEFAULT;
        break;
    case SCC_VID_PIC_MODE_CUSTOM_1:
        e_drv_vdp_pic_mode = VID_PLA_PIC_MODE_CUSTOM_1;
        break;
    case SCC_VID_PIC_MODE_CUSTOM_2:
        e_drv_vdp_pic_mode = VID_PLA_PIC_MODE_CUSTOM_2;
        break;
    case SCC_VID_PIC_MODE_CUSTOM_3:
        e_drv_vdp_pic_mode = VID_PLA_PIC_MODE_CUSTOM_3;
        break;
    case SCC_VID_PIC_MODE_CUSTOM_4:
        e_drv_vdp_pic_mode = VID_PLA_PIC_MODE_CUSTOM_4;
        break;
    case SCC_VID_PIC_MODE_CUSTOM_5:
        e_drv_vdp_pic_mode = VID_PLA_PIC_MODE_CUSTOM_5;
        break;
    case SCC_VID_PIC_MODE_CUSTOM_6:
        e_drv_vdp_pic_mode = VID_PLA_PIC_MODE_CUSTOM_6;
        break;
    case SCC_VID_PIC_MODE_CUSTOM_7:
        e_drv_vdp_pic_mode = VID_PLA_PIC_MODE_CUSTOM_7;
        break;
    case SCC_VID_PIC_MODE_CUSTOM_8:
        e_drv_vdp_pic_mode = VID_PLA_PIC_MODE_CUSTOM_8;
        break;
    case SCC_VID_PIC_MODE_CUSTOM_9:
        e_drv_vdp_pic_mode = VID_PLA_PIC_MODE_CUSTOM_9;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_PIC_MODE,
        (VOID*)(UINT32)e_drv_vdp_pic_mode,
        sizeof(VID_PLA_PIC_MODE_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_hdmi_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          e_hdmi_mode         References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_hdmi_mode( HANDLE_T               h_vid,
                               SCC_VID_HDMI_MODE_T    e_hdmi_mode )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_HDMI_MODE_T          e_drv_vdp_hdmi_mode = VID_PLA_HDMI_MODE_UNKNOWN;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\te_hdmi_mode      = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)e_hdmi_mode ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_hdmi_mode )
    {
    case SCC_VID_HDMI_MODE_UNKNOWN:
        e_drv_vdp_hdmi_mode = VID_PLA_HDMI_MODE_UNKNOWN;
        break;
    case SCC_VID_HDMI_MODE_AUTO:
        e_drv_vdp_hdmi_mode = VID_PLA_HDMI_MODE_AUTO;
        break;
    case SCC_VID_HDMI_MODE_GRAPHIC:
        e_drv_vdp_hdmi_mode = VID_PLA_HDMI_MODE_GRAPHIC;
        break;
    case SCC_VID_HDMI_MODE_VIDEO:
        e_drv_vdp_hdmi_mode = VID_PLA_HDMI_MODE_VIDEO;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_HDMI_MODE,
        (VOID*)(UINT32)e_drv_vdp_hdmi_mode,
        sizeof(VID_PLA_HDMI_MODE_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_color_sys
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          e_color_sys         References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_color_sys( HANDLE_T               h_vid,
                               SCC_VID_COLOR_SYS_T    e_color_sys )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    TV_DEC_COLOR_SYS_T           e_drv_tvd_color_sys = TV_DEC_COLOR_SYS_UNKNOWN;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\te_color_sys      = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)e_color_sys ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_color_sys )
    {
    case SCC_VID_COLOR_SYS_UNKNOWN:
        e_drv_tvd_color_sys = TV_DEC_COLOR_SYS_UNKNOWN;
        break;
    case SCC_VID_COLOR_SYS_NTSC:
        e_drv_tvd_color_sys = TV_DEC_COLOR_SYS_NTSC;
        break;
    case SCC_VID_COLOR_SYS_NTSC_443:
        e_drv_tvd_color_sys = TV_DEC_COLOR_SYS_NTSC_443;
        break;
    case SCC_VID_COLOR_SYS_PAL:
        e_drv_tvd_color_sys = TV_DEC_COLOR_SYS_PAL;
        break;
    case SCC_VID_COLOR_SYS_PAL_N:
        e_drv_tvd_color_sys = TV_DEC_COLOR_SYS_PAL_N;
        break;
    case SCC_VID_COLOR_SYS_PAL_M:
        e_drv_tvd_color_sys = TV_DEC_COLOR_SYS_PAL_M;
        break;
    case SCC_VID_COLOR_SYS_PAL_60:
        e_drv_tvd_color_sys = TV_DEC_COLOR_SYS_PAL_60;
        break;
    case SCC_VID_COLOR_SYS_SECAM:
        e_drv_tvd_color_sys = TV_DEC_COLOR_SYS_SECAM;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_TV_DEC,
        TV_DEC_SET_TYPE_COLOR_SYS,
        (VOID*)(UINT32)e_drv_tvd_color_sys,
        sizeof(TV_DEC_COLOR_SYS_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_scr_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          e_scr_mode          References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_scr_mode( HANDLE_T               h_vid,
                              SCC_VID_SCREEN_MODE_T  e_scr_mode )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_DISP_FMT_T           e_drv_vdp_disp_fmt = VID_PLA_DISP_FMT_UNKNOWN;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\te_scr_mode       = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)e_scr_mode ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    e_drv_vdp_disp_fmt = _scc_scr_mode_2_dvr_disp_fmt( e_scr_mode );

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_DISP_FMT,
        (VOID*)(UINT32)e_drv_vdp_disp_fmt,
        sizeof(VID_PLA_DISP_FMT_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_enhance
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          ui2_enhance         References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_enhance( HANDLE_T                h_vid,
                             UINT16                  ui2_enhance )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_ENHANCE_T            t_drv_vdp_enh;
    VID_PLA_MIN_MAX_INFO_T       t_drv_min_max = {0};

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tui2_enhance      = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)ui2_enhance ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    if ( ui2_enhance > SCC_ATTRIB_VALUE_MAX )
    {
        return SMR_INV_SET_INFO;
    }

    t_drv_min_max.e_get_type = VID_PLA_GET_TYPE_ENHANCE;
    z_size = sizeof(VID_PLA_MIN_MAX_INFO_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MIN_MAX,
        (VOID*)&t_drv_min_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }
    if ( t_drv_min_max.ui4_min_value >= t_drv_min_max.ui4_max_value ||
         t_drv_min_max.ui4_max_value > 0xFF ||
         t_drv_min_max.ui4_min_value > 0xFF )
    {
        return SMR_DRV_COMP_ERROR;
    }

    t_drv_vdp_enh.e_mode = VID_PLA_ENHANCE;
    t_drv_vdp_enh.ui1_level = (UINT8)_ceil_16bits_scc_val_2_drv_val(
        SCC_ATTRIB_VALUE_MAX,
        ui2_enhance,
        (UINT16)t_drv_min_max.ui4_min_value,
        (UINT16)t_drv_min_max.ui4_max_value );

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_ENHANCE,
        (VOID*)&t_drv_vdp_enh,
        sizeof(VID_PLA_ENHANCE_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_blur
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          ui2_blur            References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_blur( HANDLE_T                   h_vid,
                          UINT16                     ui2_blur )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_ENHANCE_T            t_drv_vdp_enh;
    VID_PLA_MIN_MAX_INFO_T       t_drv_min_max = {0}; 

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tui2_blur         = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)ui2_blur ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    if ( ui2_blur > SCC_ATTRIB_VALUE_MAX )
    {
        return SMR_INV_SET_INFO;
    }

    t_drv_min_max.e_get_type = VID_PLA_GET_TYPE_ENHANCE;
    z_size = sizeof(VID_PLA_MIN_MAX_INFO_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MIN_MAX,
        (VOID*)&t_drv_min_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }
    if ( t_drv_min_max.ui4_min_value >= t_drv_min_max.ui4_max_value ||
         t_drv_min_max.ui4_max_value > 0xFF ||
         t_drv_min_max.ui4_min_value > 0xFF )
    {
        return SMR_DRV_COMP_ERROR;
    }

    t_drv_vdp_enh.e_mode = VID_PLA_BLUR;
    t_drv_vdp_enh.ui1_level = (UINT8)_ceil_16bits_scc_val_2_drv_val(
        SCC_ATTRIB_VALUE_MAX,
        ui2_blur,
        (UINT16)t_drv_min_max.ui4_min_value,
        (UINT16)t_drv_min_max.ui4_max_value );

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_ENHANCE,
        (VOID*)&t_drv_vdp_enh,
        sizeof(VID_PLA_ENHANCE_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_overscan_clipper
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pt_clipper          References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_overscan_clipper(
    HANDLE_T                   h_vid,
    const SCC_VID_OVER_SCAN_CLIPPER_T* pt_clipper )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_OVER_SCAN_CLIPPER_T  t_drv_clipper = {0};

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tpt_clipper       = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pt_clipper ));

    if ( !pt_clipper )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_clipper.ui4_top = pt_clipper->ui4_top;
    t_drv_clipper.ui4_bottom = pt_clipper->ui4_bottom;
    t_drv_clipper.ui4_left = pt_clipper->ui4_left;
    t_drv_clipper.ui4_right = pt_clipper->ui4_right;

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_OVER_SCAN_CLIPPER,
        (VOID*)&t_drv_clipper,
        sizeof(VID_PLA_OVER_SCAN_CLIPPER_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_cti
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: ui2_cti             References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_cti(
    HANDLE_T                   h_vid,
    UINT16                     ui2_cti )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_MIN_MAX_INFO_T       t_drv_min_max = {0};
    UINT8                        ui1_drv_cti = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tui2_cti          = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)ui2_cti ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    if ( ui2_cti > SCC_ATTRIB_VALUE_MAX )
    {
        return SMR_INV_SET_INFO;
    }

    t_drv_min_max.e_get_type = VID_PLA_GET_TYPE_CTI;
    z_size = sizeof(VID_PLA_MIN_MAX_INFO_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MIN_MAX,
        (VOID*)&t_drv_min_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }
    if ( t_drv_min_max.ui4_min_value >= t_drv_min_max.ui4_max_value ||
         t_drv_min_max.ui4_max_value > 0xFF ||
         t_drv_min_max.ui4_min_value > 0xFF )
    {
        return SMR_DRV_COMP_ERROR;
    }

    ui1_drv_cti = (UINT8)_ceil_16bits_scc_val_2_drv_val(
        SCC_ATTRIB_VALUE_MAX,
        ui2_cti,
        (UINT16)t_drv_min_max.ui4_min_value,
        (UINT16)t_drv_min_max.ui4_max_value );

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_CTI,
        (VOID*)(UINT32)ui1_drv_cti,
        sizeof(UINT8) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_eti
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: ui2_eti             References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_eti(
    HANDLE_T                   h_vid,
    UINT16                     ui2_eti )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_MIN_MAX_INFO_T       t_drv_min_max = {0};
    UINT8                        ui1_drv_eti = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tui2_eti          = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)ui2_eti ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    if ( ui2_eti > SCC_ATTRIB_VALUE_MAX )
    {
        return SMR_INV_SET_INFO;
    }

    t_drv_min_max.e_get_type = VID_PLA_GET_TYPE_ETI;
    z_size = sizeof(VID_PLA_MIN_MAX_INFO_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MIN_MAX,
        (VOID*)&t_drv_min_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }
    if ( t_drv_min_max.ui4_min_value >= t_drv_min_max.ui4_max_value ||
         t_drv_min_max.ui4_max_value > 0xFF ||
         t_drv_min_max.ui4_min_value > 0xFF )
    {
        return SMR_DRV_COMP_ERROR;
    }

    ui1_drv_eti = (UINT8)_ceil_16bits_scc_val_2_drv_val(
        SCC_ATTRIB_VALUE_MAX,
        ui2_eti,
        (UINT16)t_drv_min_max.ui4_min_value,
        (UINT16)t_drv_min_max.ui4_max_value );

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_ETI,
        (VOID*)(UINT32)ui1_drv_eti,
        sizeof(UINT8) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_sharpness
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: ui2_sharpness         References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_sharpness(
    HANDLE_T                   h_vid,
    UINT16                     ui2_sharpness )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_MIN_MAX_INFO_T       t_drv_min_max = {0};
    UINT8                        ui1_drv_sharpness = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tui2_sharpness      = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)ui2_sharpness ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    if ( ui2_sharpness > SCC_ATTRIB_VALUE_MAX )
    {
        return SMR_INV_SET_INFO;
    }

    t_drv_min_max.e_get_type = VID_PLA_GET_TYPE_SHARPNESS;
    z_size = sizeof(VID_PLA_MIN_MAX_INFO_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MIN_MAX,
        (VOID*)&t_drv_min_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }
    if ( t_drv_min_max.ui4_min_value >= t_drv_min_max.ui4_max_value ||
         t_drv_min_max.ui4_max_value > 0xFF ||
         t_drv_min_max.ui4_min_value > 0xFF )
    {
        return SMR_DRV_COMP_ERROR;
    }

    ui1_drv_sharpness = (UINT8)_ceil_16bits_scc_val_2_drv_val(
        SCC_ATTRIB_VALUE_MAX,
        ui2_sharpness,
        (UINT16)t_drv_min_max.ui4_min_value,
        (UINT16)t_drv_min_max.ui4_max_value );

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_SHARPNESS,
        (VOID*)(UINT32)ui1_drv_sharpness,
        sizeof(UINT8) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_color_suppress
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: ui2_color_supress   References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_color_suppress(
    HANDLE_T                   h_vid,
    UINT16                     ui2_color_supress )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_MIN_MAX_INFO_T       t_drv_min_max = {0};
    UINT8                        ui1_drv_color_supress = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tui2_color_supress = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)ui2_color_supress ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    if ( ui2_color_supress > SCC_ATTRIB_VALUE_MAX )
    {
        return SMR_INV_SET_INFO;
    }

    t_drv_min_max.e_get_type = VID_PLA_GET_TYPE_COLOR_SUPPRESS;
    z_size = sizeof(VID_PLA_MIN_MAX_INFO_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MIN_MAX,
        (VOID*)&t_drv_min_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }
    if ( t_drv_min_max.ui4_min_value >= t_drv_min_max.ui4_max_value ||
         t_drv_min_max.ui4_max_value > 0xFF ||
         t_drv_min_max.ui4_min_value > 0xFF )
    {
        return SMR_DRV_COMP_ERROR;
    }

    ui1_drv_color_supress = (UINT8)_ceil_16bits_scc_val_2_drv_val(
        SCC_ATTRIB_VALUE_MAX,
        ui2_color_supress,
        (UINT16)t_drv_min_max.ui4_min_value,
        (UINT16)t_drv_min_max.ui4_max_value );

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_COLOR_SUPPRESS,
        (VOID*)(UINT32)ui1_drv_color_supress,
        sizeof(UINT8) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_nr
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: ui2_nr   References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_nr(
    HANDLE_T                   h_vid,
    UINT16                     ui2_nr )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_MAX_INFO_T           t_drv_max  = {0};
    UINT8                        ui1_drv_nr = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tui2_nr = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)ui2_nr ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    if ( ui2_nr > SCC_ATTRIB_VALUE_MAX )
    {
        return SMR_INV_SET_INFO;
    }

    t_drv_max.e_get_type = VID_PLA_GET_TYPE_NR;
    z_size = sizeof(VID_PLA_MAX_INFO_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MAX,
        (VOID*)&t_drv_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }
    if ( t_drv_max.ui4_max_value > 0xFF )
    {
        return SMR_DRV_COMP_ERROR;
    }

    ui1_drv_nr = (UINT8)_ceil_16bits_scc_val_2_drv_val(
        SCC_ATTRIB_VALUE_MAX,
        ui2_nr,
        (UINT16)0,
        (UINT16)t_drv_max.ui4_max_value );

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_NR,
        (VOID*)(UINT32)ui1_drv_nr,
        sizeof(UINT8) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_3dnr
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: ui2_3dnr            References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_3dnr(
    HANDLE_T                   h_vid,
    UINT16                     ui2_3dnr )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_MAX_INFO_T           t_drv_max = {0};
    UINT8                        ui1_drv_3dnr = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tui2_3dnr          = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)ui2_3dnr ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    if ( ui2_3dnr > SCC_ATTRIB_VALUE_MAX )
    {
        return SMR_INV_SET_INFO;
    }

    t_drv_max.e_get_type = VID_PLA_GET_TYPE_3D_NR;
    z_size = sizeof(VID_PLA_MAX_INFO_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MAX,
        (VOID*)&t_drv_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }
    if ( t_drv_max.ui4_max_value > 0xFF )
    {
        return SMR_DRV_COMP_ERROR;
    }

    ui1_drv_3dnr = (UINT8)_ceil_16bits_scc_val_2_drv_val(
        SCC_ATTRIB_VALUE_MAX,
        ui2_3dnr,
        (UINT16)0,
        (UINT16)t_drv_max.ui4_max_value );

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_3D_NR,
        (VOID*)(UINT32)ui1_drv_3dnr,
        sizeof(UINT8) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_black_lvl_ext
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: ui2_black_lvl_ext   References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_black_lvl_ext(
    HANDLE_T                   h_vid,
    UINT16                     ui2_black_lvl_ext )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_MAX_INFO_T           t_drv_max = {0};
    UINT8                        ui1_drv_black_lvl_ext = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tui2_black_lvl_ext = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)ui2_black_lvl_ext ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    if ( ui2_black_lvl_ext > SCC_ATTRIB_VALUE_MAX )
    {
        return SMR_INV_SET_INFO;
    }

    t_drv_max.e_get_type = VID_PLA_GET_TYPE_BLACK_LVL_EXT;
    z_size = sizeof(VID_PLA_MAX_INFO_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MAX,
        (VOID*)&t_drv_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }
    if ( t_drv_max.ui4_max_value > 0xFF )
    {
        return SMR_DRV_COMP_ERROR;
    }

    ui1_drv_black_lvl_ext = (UINT8)_ceil_16bits_scc_val_2_drv_val(
        SCC_ATTRIB_VALUE_MAX,
        ui2_black_lvl_ext,
        (UINT16)0,
        (UINT16)t_drv_max.ui4_max_value );

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_BLACK_LVL_EXT,
        (VOID*)(UINT32)ui1_drv_black_lvl_ext,
        sizeof(UINT8) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_white_peak_rdct
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: ui2_white_peak_rdct   References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_white_peak_rdct(
    HANDLE_T                   h_vid,
    UINT16                     ui2_white_peak_rdct )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_MAX_INFO_T           t_drv_max = {0};
    UINT8                        ui1_drv_white_peak_rdct = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tui2_white_peak_rdct = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)ui2_white_peak_rdct ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    if ( ui2_white_peak_rdct > SCC_ATTRIB_VALUE_MAX )
    {
        return SMR_INV_SET_INFO;
    }

    t_drv_max.e_get_type = VID_PLA_GET_TYPE_WHITE_PEAK_LMT;
    z_size = sizeof(VID_PLA_MAX_INFO_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MAX,
        (VOID*)&t_drv_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }
    if ( t_drv_max.ui4_max_value > 0xFF )
    {
        return SMR_DRV_COMP_ERROR;
    }

    ui1_drv_white_peak_rdct = (UINT8)_ceil_16bits_scc_val_2_drv_val(
        SCC_ATTRIB_VALUE_MAX,
        ui2_white_peak_rdct,
        (UINT16)0,
        (UINT16)t_drv_max.ui4_max_value );

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_WHITE_PEAK_LMT,
        (VOID*)(UINT32)ui1_drv_white_peak_rdct,
        sizeof(UINT8) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_flesh_tone
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: ui2_flesh_tone   References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_flesh_tone(
    HANDLE_T                   h_vid,
    UINT16                     ui2_flesh_tone )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_MAX_INFO_T           t_drv_max = {0};
    UINT8                        ui1_drv_flesh_tone = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tui2_flesh_tone    = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)ui2_flesh_tone ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    if ( ui2_flesh_tone > SCC_ATTRIB_VALUE_MAX )
    {
        return SMR_INV_SET_INFO;
    }

    t_drv_max.e_get_type = VID_PLA_GET_TYPE_FLESH_TONE;
    z_size = sizeof(VID_PLA_MAX_INFO_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MAX,
        (VOID*)&t_drv_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }
    if ( t_drv_max.ui4_max_value > 0xFF )
    {
        return SMR_DRV_COMP_ERROR;
    }

    ui1_drv_flesh_tone = (UINT8)_ceil_16bits_scc_val_2_drv_val(
        SCC_ATTRIB_VALUE_MAX,
        ui2_flesh_tone,
        (UINT16)0,
        (UINT16)t_drv_max.ui4_max_value );

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_FLESH_TONE,
        (VOID*)(UINT32)ui1_drv_flesh_tone,
        sizeof(UINT8) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_luma
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: ui2_luma   References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_luma(
    HANDLE_T                   h_vid,
    UINT16                     ui2_luma )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_MAX_INFO_T           t_drv_max = {0};
    UINT8                        ui1_drv_luma = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tui2_luma          = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)ui2_luma ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    if ( ui2_luma > SCC_ATTRIB_VALUE_MAX )
    {
        return SMR_INV_SET_INFO;
    }

    t_drv_max.e_get_type = VID_PLA_GET_TYPE_LUMA;
    z_size = sizeof(VID_PLA_MAX_INFO_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MAX,
        (VOID*)&t_drv_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }
    if ( t_drv_max.ui4_max_value > 0xFF )
    {
        return SMR_DRV_COMP_ERROR;
    }

    ui1_drv_luma = (UINT8)_ceil_16bits_scc_val_2_drv_val(
        SCC_ATTRIB_VALUE_MAX,
        ui2_luma,
        (UINT16)0,
        (UINT16)t_drv_max.ui4_max_value );

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_LUMA,
        (VOID*)(UINT32)ui1_drv_luma,
        sizeof(UINT8) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_letter_box_detect
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: b_is_on             References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_letter_box_detect(
    HANDLE_T                   h_vid,
    BOOL                       b_is_on )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tb_is_on           = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)b_is_on ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_LETTER_BOX_DETECT,
        (VOID*)(UINT32)b_is_on,
        sizeof(BOOL) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_lb_detect_config
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          pt_config           References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_lb_detect_config(
    HANDLE_T                    h_vid,
    SCC_VID_LB_DETECT_CONFIG_T* pt_config )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_LB_DETECT_CONFIG_T   t_vid_lb_detect = {0};

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tpt_config           = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pt_config ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_vid_lb_detect.ui4_time_threshold = pt_config->ui4_time_threshold;
    t_vid_lb_detect.ui4_bounds_top = pt_config->ui4_bounds_top;
    t_vid_lb_detect.ui4_bounds_bottom = pt_config->ui4_bounds_bottom;
    t_vid_lb_detect.ui4_bounds_left = pt_config->ui4_bounds_left;
    t_vid_lb_detect.ui4_bounds_right = pt_config->ui4_bounds_right;
    t_vid_lb_detect.b_manual_mode = pt_config->b_manual_mode;

    pt_vid_comp->pf_lb_detect_nfy =
        pt_config->t_lb_detect_nfy_info.pf_lb_detect_nfy;
    pt_vid_comp->pv_lb_detect_nfy_tag =
        pt_config->t_lb_detect_nfy_info.pv_nfy_tag;

    t_vid_lb_detect.t_lb_detect_nfy_info.pf_lb_detect_nfy =
        _scc_comp_vid_lb_detect_nfy_fct;
    t_vid_lb_detect.t_lb_detect_nfy_info.pv_nfy_tag = (VOID*)(UINT32)h_vid;

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_LB_DETECT_CONFIG,
        (VOID*)(UINT32)&t_vid_lb_detect,
        sizeof(SCC_VID_LB_DETECT_CONFIG_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_dynamic_scaling
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: ui2_effect_time     References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_dynamic_scaling(
    HANDLE_T                   h_vid,
    UINT16                     ui2_effect_time )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tui2_effect_time   = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)ui2_effect_time ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    /* Effect time should not exceed 1000ms */
    if ( ui2_effect_time > 1000 )
    {
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_DYNAMIC_SCALING,
        (VOID*)(UINT32)ui2_effect_time,
        sizeof(UINT16) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_deblocking
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          pt_deblocking       References.
 *
 * Outputs: ui2_effect_time     References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_deblocking(
    HANDLE_T                   h_vid,
    SCC_VID_DEBLOCKING_INFO_T* pt_deblocking )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_DEC_DEBLOCKING_INFO_T    t_vid_deblock = {0};

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tpt_deblocking     = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pt_deblocking ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    /* Effect time should not exceed 1000ms */
    if ( !pt_deblocking ||
         pt_deblocking->u.t_region.ui4_x > 1000 ||
         pt_deblocking->u.t_region.ui4_y > 1000 ||
         pt_deblocking->u.t_region.ui4_w > 1000 ||
         pt_deblocking->u.t_region.ui4_h > 1000 )
    {
        return SMR_INV_ARG;
    }

    t_vid_deblock.b_is_on = pt_deblocking->b_is_on;
    t_vid_deblock.u.t_region.ui4_x = pt_deblocking->u.t_region.ui4_x;
    t_vid_deblock.u.t_region.ui4_y = pt_deblocking->u.t_region.ui4_y;
    t_vid_deblock.u.t_region.ui4_w = pt_deblocking->u.t_region.ui4_w;
    t_vid_deblock.u.t_region.ui4_h = pt_deblocking->u.t_region.ui4_h;

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_DEC,
        VID_DEC_SET_TYPE_DEBLOCKING,
        (VOID*)(UINT32)&t_vid_deblock,
        sizeof(VID_DEC_DEBLOCKING_INFO_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_split_scrn_demo
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          e_splt_scrn_demo    References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_split_scrn_demo(
    HANDLE_T                   h_vid,
    SCC_VID_SPLIT_SCRN_DEMO_T  e_splt_scrn_demo )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_SPLIT_SCRN_DEMO_T    e_drv_splt_scrn_demo = VID_PLA_SPLIT_SCRN_DEMO_OFF;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\te_splt_scrn_demo  = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)e_splt_scrn_demo ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_splt_scrn_demo )
    {
    case SCC_VID_SPLIT_SCRN_DEMO_OFF:
        e_drv_splt_scrn_demo = VID_PLA_SPLIT_SCRN_DEMO_OFF;
        break;

    case SCC_VID_SPLIT_SCRN_DEMO_LEFT:
        e_drv_splt_scrn_demo = VID_PLA_SPLIT_SCRN_DEMO_LEFT;
        break;

    case SCC_VID_SPLIT_SCRN_DEMO_RIGHT:
        e_drv_splt_scrn_demo = VID_PLA_SPLIT_SCRN_DEMO_RIGHT;
        break;

    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_SPLIT_SCRN_DEMO,
        (VOID*)(UINT32)e_drv_splt_scrn_demo,
        sizeof(VID_PLA_SPLIT_SCRN_DEMO_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_h_position
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          ui2_offset          References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_h_position(
    HANDLE_T                   h_vid,
    UINT16                     ui2_offset )
{
    return x_scc_vid_set_position_size(
            h_vid,
            VID_PLA_SET_TYPE_H_POSITION,
            ui2_offset );
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_v_position
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          ui2_offset          References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_v_position(
    HANDLE_T                   h_vid,
    UINT16                     ui2_offset )
{
    return x_scc_vid_set_position_size(
            h_vid,
            VID_PLA_SET_TYPE_V_POSITION,
            ui2_offset );
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_h_size
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          ui2_offset          References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_h_size(
    HANDLE_T                   h_vid,
    UINT16                     ui2_offset )
{
    return x_scc_vid_set_position_size(
            h_vid,
            VID_PLA_SET_TYPE_H_SIZE,
            ui2_offset );
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_v_size
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          ui2_offset          References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_v_size(
    HANDLE_T                   h_vid,
    UINT16                     ui2_offset )
{
    return x_scc_vid_set_position_size(
            h_vid,
            VID_PLA_SET_TYPE_V_SIZE,
            ui2_offset );
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_position_size
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          e_set_type          References.
 *          ui2_offset          References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_position_size(
    HANDLE_T                   h_vid,
    UINT32                     e_set_type,
    UINT16                     ui2_offset )
{
    INT32                        i4_ret;
/*
    SIZE_T                       z_size;
    VID_PLA_MIN_MAX_INFO_T       t_drv_min_max;
*/
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tui2_offset       = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)ui2_offset ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }
/*
    switch (e_set_type)
    {
    case VID_PLA_SET_TYPE_H_POSITION:
        t_drv_min_max.e_get_type = VID_PLA_GET_TYPE_H_POSITION;
        break;
    case VID_PLA_SET_TYPE_V_POSITION:
        t_drv_min_max.e_get_type = VID_PLA_GET_TYPE_V_POSITION;
        break;
    case VID_PLA_SET_TYPE_H_SIZE:
        t_drv_min_max.e_get_type = VID_PLA_GET_TYPE_H_SIZE;
        break;
    case VID_PLA_SET_TYPE_V_SIZE:
        t_drv_min_max.e_get_type = VID_PLA_GET_TYPE_V_SIZE;
        break;
    }
    z_size = sizeof(VID_PLA_MIN_MAX_INFO_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MIN_MAX,
        (VOID*)&t_drv_min_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }
    if ( t_drv_min_max.ui4_min_value >= t_drv_min_max.ui4_max_value ||
         t_drv_min_max.ui4_max_value > 0xFF ||
         t_drv_min_max.ui4_min_value > 0xFF )
    {
        return SMR_DRV_COMP_ERROR;
    }
*/
    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        e_set_type,
        (VOID*)(UINT32)ui2_offset,
        sizeof(UINT16) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_film_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          b_is_on             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_film_mode(
    HANDLE_T                   h_vid,
    BOOL                       b_is_on )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tb_is_on           = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)b_is_on ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_FILM_MODE,
        (VOID*)(UINT32)b_is_on,
        sizeof(BOOL) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_reset_vga_timing_option
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_reset_vga_timing_option(
    HANDLE_T                   h_vid )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_TV_DEC,
        TV_DEC_SET_TYPE_RESET_VGA_TIMING_OPTION,
        (VOID*)NULL,
        0 );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_ambig_vga_timing_ctrl
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          b_is_on             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_ambig_vga_timing_ctrl(
    HANDLE_T                   h_vid,
    BOOL                       b_is_on )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_TV_DEC,
        TV_DEC_SET_TYPE_AMBIG_VGA_TIMING_CTRL,
        (VOID*)(UINT32)b_is_on,
        sizeof(BOOL) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_thunmbnail_enable
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          b_is_on             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32    x_scc_vid_set_thumbnail_enable(
    HANDLE_T                   h_vid,
    BOOL                       b_is_on )
{
	 INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_THUMBNAIL_ENABLE,
        (VOID*)(UINT32)b_is_on,
        sizeof(BOOL) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;

}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_srm_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          e_srm_mode             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32    x_scc_vid_set_srm_mode(
    HANDLE_T                   h_vid,
    SCC_VID_SRM_MODE_T         e_srm_mode )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_SRM_MODE_T           e_drv_srm_mode = VID_PLA_SRM_MODE_OFF;
    
    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_srm_mode )
    {
    case SCC_VID_SRM_MODE_OFF:
        e_drv_srm_mode = VID_PLA_SRM_MODE_OFF;
        break;

    case SCC_VID_SRM_MODE_THUMBNAIL:
        e_drv_srm_mode = VID_PLA_SRM_MODE_THUMBNAIL;
        break;

    case SCC_VID_SRM_MODE_VIDEO:
        e_drv_srm_mode = VID_PLA_SRM_MODE_VIDEO;
        break;

    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_SRM_MODE,
        (VOID*)(UINT32)e_drv_srm_mode,
        sizeof(VID_PLA_SRM_MODE_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_force_unmute_on_no_sig
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          b_is_on             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_force_unmute_on_no_sig(
    HANDLE_T                   h_vid,
    BOOL                       b_is_on )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tb_is_on           = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)b_is_on ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_FORCE_UNMUTE_ON_NO_SIG,
        (VOID*)(UINT32)b_is_on,
        sizeof(BOOL) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_super_freeze
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          pt_freeze             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_super_freeze(
    HANDLE_T                   h_vid,
    const SCC_VID_SUPER_FREEZE_T*   pt_freeze )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_DEC_SUPER_FREEZE_T       t_dec_drv_freeze;
    VID_PLA_SUPER_FREEZE_T       t_vdp_drv_freeze;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tpt_freeze           = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pt_freeze ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    if (pt_freeze->e_type == SCC_VID_SUPER_FREEZE_TYPE_MEMO)
    {
        t_dec_drv_freeze.e_type = VID_DEC_SUPER_FREEZE_TYPE_MEMO;
        t_vdp_drv_freeze.e_type = VID_PLA_SUPER_FREEZE_TYPE_MEMO;
    }
    else if (pt_freeze->e_type == SCC_VID_SUPER_FREEZE_TYPE_FORCE)
    {
        t_dec_drv_freeze.e_type = VID_DEC_SUPER_FREEZE_TYPE_FORCE;
        t_vdp_drv_freeze.e_type = VID_PLA_SUPER_FREEZE_TYPE_FORCE;
    }
    else
    {
        return SMR_INV_SET_INFO;
    }
    t_dec_drv_freeze.b_enable = pt_freeze->b_enable;
    t_vdp_drv_freeze.b_enable = pt_freeze->b_enable;

    /* need set time shift force super freeze to decoder */
    if (pt_freeze->e_type == SCC_VID_SUPER_FREEZE_TYPE_FORCE)
    {
        i4_ret = _scc_comp_vid_set(
            h_vid,
            DRVT_VID_DEC,
            VID_DEC_SET_TYPE_SUPER_FREEZE,
            (VOID*)&t_dec_drv_freeze,
            sizeof(VID_DEC_SUPER_FREEZE_T) );
        if ( i4_ret != SMR_OK )
        {
            return SMR_INV_SET_INFO;
        }
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_SUPER_FREEZE,
        (VOID*)&t_vdp_drv_freeze,
        sizeof(VID_PLA_SUPER_FREEZE_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_force_unmute
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          pt_freeze             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_force_unmute(
    HANDLE_T                    h_vid,
    SCC_VID_FORCE_UNMUTE_TYPE_T e_unmute_type,
    BOOL                        b_is_on)
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_FORCE_UNMUTE_T       t_vdp_drv_force_unmute;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\te_unmute_type     = %u )\r\n"
                       "\t\tb_is_on           = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)e_unmute_type,
                       (UINT32)b_is_on));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    if (e_unmute_type == SCC_VID_FORCE_UNMUTE_TYPE_ON_NO_SIG)
    {
        t_vdp_drv_force_unmute.e_force_unmute_type = VID_PLA_FORCE_UNMUTE_TYPE_ON_NO_SIG;
    }
    else if (e_unmute_type == SCC_VID_FORCE_UNMUTE_TYPE_ATV_CHG_CH)
    {
        t_vdp_drv_force_unmute.e_force_unmute_type = VID_PLA_FORCE_UNMUTE_TYPE_ATV_CHG_CH;
    }
    else
    {
        return SMR_INV_SET_INFO;
    }
    t_vdp_drv_force_unmute.b_enable = b_is_on;

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_FORCE_UNMUTE,
        (VOID*)&t_vdp_drv_force_unmute,
        sizeof(VID_PLA_FORCE_UNMUTE_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_local_dimming_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          e_lcdim_mode             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_local_dimming_mode(
    HANDLE_T                        h_vid,
    SCC_VID_LOCAL_DIMMING_TYPE_T    e_lcdim_mode )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_LOCAL_DIMMING_MODE_T e_drv_lcdim_mode = VID_PLA_LOCAL_DIMMING_MODE_OFF;
    
    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_lcdim_mode )
    {
    case SCC_VID_LOCAL_DIMMING_TYPE_OFF:
        e_drv_lcdim_mode = VID_PLA_LOCAL_DIMMING_MODE_OFF;
        break;

    case SCC_VID_LOCAL_DIMMING_TYPE_ON:
        e_drv_lcdim_mode = VID_PLA_LOCAL_DIMMING_MODE_LOW;
        break;

    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_LCDIM,
        (VOID*)(UINT32)e_drv_lcdim_mode,
        sizeof(VID_PLA_LOCAL_DIMMING_MODE_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_local_dimming_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pe_lcdim_mode        References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_local_dimming_mode( 
    HANDLE_T                        h_vid,
    SCC_VID_LOCAL_DIMMING_TYPE_T*   pe_lcdim_mode )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_LOCAL_DIMMING_MODE_T e_drv_lcdim_mode = VID_PLA_LOCAL_DIMMING_MODE_OFF;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tpe_lcdim_mode     = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pe_lcdim_mode ));

    if ( !pe_lcdim_mode )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }


    z_size = sizeof(VID_PLA_LOCAL_DIMMING_MODE_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_LCDIM,
        (VOID*)&e_drv_lcdim_mode,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch( e_drv_lcdim_mode )
    {
    case VID_PLA_LOCAL_DIMMING_MODE_OFF:
        *pe_lcdim_mode = SCC_VID_LOCAL_DIMMING_TYPE_OFF;
        break;
    case VID_PLA_LOCAL_DIMMING_MODE_LOW:
        *pe_lcdim_mode = SCC_VID_LOCAL_DIMMING_TYPE_ON;
        break;
    default:
        return SMR_DRV_COMP_ERROR;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_super_freeze
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pb_is_on            References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_super_freeze(
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_on )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_SUPER_FREEZE_MODE_T  e_drv_mode = VID_PLA_SUPER_FREEZE_MODE_DISABLE;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid                = 0x%.8X\r\n"
                       "\t\tpb_is_on             = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pb_is_on ));

    if ( !pb_is_on )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(VID_PLA_SUPER_FREEZE_MODE_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_SUPER_FREEZE,
        (VOID*)&e_drv_mode,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }
    else if ( e_drv_mode == VID_PLA_SUPER_FREEZE_MODE_DISABLE )
    {
        *pb_is_on = FALSE;
    }
    else if ( e_drv_mode == VID_PLA_SUPER_FREEZE_MODE_ENABLE )
    {
        *pb_is_on = TRUE;
    }
    else
    {
        return SMR_INTERNAL_ERROR;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_force_unmute_on_no_sig
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pb_is_on            References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_force_unmute_on_no_sig(
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_on )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    BOOL                         b_is_on = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid                = 0x%.8X\r\n"
                       "\t\tpb_is_on             = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pb_is_on ));

    if ( !pb_is_on )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(BOOL);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_FORCE_UNMUTE_ON_NO_SIG,
        (VOID*)&b_is_on,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    *pb_is_on = b_is_on;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pe_mode             References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_mode( HANDLE_T                   h_vid,
                          SCC_VID_MODE_T*            pe_mode )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_MODE_T               e_drv_vdp_mode = VID_PLA_NORMAL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpe_mode          = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pe_mode ));

    if ( !pe_mode )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(VID_PLA_MODE_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MODE,
        (VOID*)&e_drv_vdp_mode,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch(e_drv_vdp_mode)
    {
    case VID_PLA_NORMAL:
        *pe_mode = SCC_VID_NORMAL;
        break;
    case VID_PLA_BLANK:
        *pe_mode = SCC_VID_BLANK;
        break;
    case VID_PLA_QUAD_VIDEO:
        *pe_mode = SCC_VID_QUAD_VIDEO;
        break;
    case VID_PLA_DEINT:
        *pe_mode = SCC_VID_DEINT;
        break;
    case VID_PLA_FREEZE:
        *pe_mode = SCC_VID_FREEZE;
        break;
    default:
        return SMR_DRV_COMP_ERROR;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_mode_capability
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          e_mode              References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_NOT_SUPPORT     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_mode_capability( HANDLE_T                   h_vid,
                                     SCC_VID_MODE_T             e_mode )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_MODE_T               e_drv_vdp_mode = VID_PLA_NORMAL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\te_mode           = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)e_mode ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_mode )
    {
    case SCC_VID_NORMAL:
        e_drv_vdp_mode = VID_PLA_NORMAL;
        break;
    case SCC_VID_BLANK:
        e_drv_vdp_mode = VID_PLA_BLANK;
        break;
    case SCC_VID_QUAD_VIDEO:
        e_drv_vdp_mode = VID_PLA_QUAD_VIDEO;
        break;
    case SCC_VID_DEINT:
        e_drv_vdp_mode = VID_PLA_DEINT;
        break;
    case SCC_VID_FREEZE:
        e_drv_vdp_mode = VID_PLA_FREEZE;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    z_size = sizeof(VID_PLA_MODE_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MODE_CAPABILITY,
        (VOID*)&e_drv_vdp_mode,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_NOT_SUPPORT;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_bg
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pt_bg               References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_bg( HANDLE_T                     h_vid,
                        SCC_BG_COLOR_T*              pt_bg )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_BG_COLOR_T           t_drv_vdp_bg_color = {0};

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tpt_bg            = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pt_bg ));

    if ( !pt_bg )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(VID_PLA_BG_COLOR_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_BG,
        (VOID*)&t_drv_vdp_bg_color,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    pt_bg->ui1_red   = t_drv_vdp_bg_color.ui1_r;
    pt_bg->ui1_green = t_drv_vdp_bg_color.ui1_g;
    pt_bg->ui1_blue  = t_drv_vdp_bg_color.ui1_b;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_pic_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pe_pic_mode         References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_pic_mode( HANDLE_T               h_vid,
                              SCC_VID_PIC_MODE_T*    pe_pic_mode )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_PIC_MODE_T           e_drv_vdp_pic_mode = VID_PLA_PIC_MODE_DEFAULT;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tpe_pic_mode      = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pe_pic_mode ));

    if ( !pe_pic_mode )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }


    z_size = sizeof(VID_PLA_PIC_MODE_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_PIC_MODE,
        (VOID*)&e_drv_vdp_pic_mode,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch( e_drv_vdp_pic_mode )
    {
    case VID_PLA_PIC_MODE_DEFAULT:
        *pe_pic_mode = SCC_VID_PIC_MODE_DEFAULT;
        break;
    case VID_PLA_PIC_MODE_CUSTOM_1:
        *pe_pic_mode = SCC_VID_PIC_MODE_CUSTOM_1;
        break;
    case VID_PLA_PIC_MODE_CUSTOM_2:
        *pe_pic_mode = SCC_VID_PIC_MODE_CUSTOM_2;
        break;
    case VID_PLA_PIC_MODE_CUSTOM_3:
        *pe_pic_mode = SCC_VID_PIC_MODE_CUSTOM_3;
        break;
    case VID_PLA_PIC_MODE_CUSTOM_4:
        *pe_pic_mode = SCC_VID_PIC_MODE_CUSTOM_4;
        break;
    case VID_PLA_PIC_MODE_CUSTOM_5:
        *pe_pic_mode = SCC_VID_PIC_MODE_CUSTOM_5;
        break;
    case VID_PLA_PIC_MODE_CUSTOM_6:
        *pe_pic_mode = SCC_VID_PIC_MODE_CUSTOM_6;
        break;
    case VID_PLA_PIC_MODE_CUSTOM_7:
        *pe_pic_mode = SCC_VID_PIC_MODE_CUSTOM_7;
        break;
    case VID_PLA_PIC_MODE_CUSTOM_8:
        *pe_pic_mode = SCC_VID_PIC_MODE_CUSTOM_8;
        break;
    case VID_PLA_PIC_MODE_CUSTOM_9:
        *pe_pic_mode = SCC_VID_PIC_MODE_CUSTOM_9;
        break;
    default:
        return SMR_DRV_COMP_ERROR;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_hdmi_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pe_hdmi_mode        References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_hdmi_mode( HANDLE_T               h_vid,
                               SCC_VID_HDMI_MODE_T*   pe_hdmi_mode )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_HDMI_MODE_T          e_drv_vdp_hdmi_mode = VID_PLA_HDMI_MODE_UNKNOWN;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tpe_hdmi_mode     = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pe_hdmi_mode ));

    if ( !pe_hdmi_mode )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }


    z_size = sizeof(VID_PLA_HDMI_MODE_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_HDMI_MODE,
        (VOID*)&e_drv_vdp_hdmi_mode,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch( e_drv_vdp_hdmi_mode )
    {
    case VID_PLA_HDMI_MODE_UNKNOWN:
        *pe_hdmi_mode = SCC_VID_HDMI_MODE_UNKNOWN;
        break;
    case VID_PLA_HDMI_MODE_AUTO:
        *pe_hdmi_mode = SCC_VID_HDMI_MODE_AUTO;
        break;
    case VID_PLA_HDMI_MODE_GRAPHIC:
        *pe_hdmi_mode = SCC_VID_HDMI_MODE_GRAPHIC;
        break;
    case VID_PLA_HDMI_MODE_VIDEO:
        *pe_hdmi_mode = SCC_VID_HDMI_MODE_VIDEO;
        break;
    default:
        return SMR_DRV_COMP_ERROR;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_color_sys
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pe_color_sys        References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_color_sys( HANDLE_T               h_vid,
                               SCC_VID_COLOR_SYS_T*   pe_color_sys )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    TV_DEC_COLOR_SYS_T           e_drv_tvd_color_sys = TV_DEC_COLOR_SYS_UNKNOWN;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tpe_color_sys      = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pe_color_sys ));

    if ( !pe_color_sys )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }


    z_size = sizeof(TV_DEC_COLOR_SYS_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_TV_DEC,
        TV_DEC_GET_TYPE_COLOR_SYS,
        (VOID*)&e_drv_tvd_color_sys,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch( e_drv_tvd_color_sys )
    {
    case TV_DEC_COLOR_SYS_UNKNOWN:
        *pe_color_sys = SCC_VID_COLOR_SYS_UNKNOWN;
        break;
    case TV_DEC_COLOR_SYS_NTSC:
        *pe_color_sys = SCC_VID_COLOR_SYS_NTSC;
        break;
    case TV_DEC_COLOR_SYS_NTSC_443:
        *pe_color_sys = SCC_VID_COLOR_SYS_NTSC_443;
        break;
    case TV_DEC_COLOR_SYS_PAL:
        *pe_color_sys = SCC_VID_COLOR_SYS_PAL;
        break;
    case TV_DEC_COLOR_SYS_PAL_N:
        *pe_color_sys = SCC_VID_COLOR_SYS_PAL_N;
        break;
    case TV_DEC_COLOR_SYS_PAL_M:
        *pe_color_sys = SCC_VID_COLOR_SYS_PAL_M;
        break;
    case TV_DEC_COLOR_SYS_PAL_60:
        *pe_color_sys = SCC_VID_COLOR_SYS_PAL_60;
        break;
    case TV_DEC_COLOR_SYS_SECAM:
        *pe_color_sys = SCC_VID_COLOR_SYS_SECAM;
        break;
    default:
        return SMR_DRV_COMP_ERROR;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_scr_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pe_scr_mode         References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_scr_mode( HANDLE_T               h_vid,
                              SCC_VID_SCREEN_MODE_T* pe_scr_mode )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_DISP_FMT_T           e_drv_vdp_disp_fmt = VID_PLA_DISP_FMT_UNKNOWN;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpe_scr_mode      = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pe_scr_mode ));

    if ( !pe_scr_mode )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }


    z_size = sizeof(VID_PLA_DISP_FMT_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_DISP_FMT,
        (VOID*)&e_drv_vdp_disp_fmt,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch( e_drv_vdp_disp_fmt )
    {
    case VID_PLA_DISP_FMT_UNKNOWN:
        *pe_scr_mode = SCC_VID_SCREEN_MODE_UNKNOWN;
        break;
    case VID_PLA_DISP_FMT_NORMAL:
        *pe_scr_mode = SCC_VID_SCREEN_MODE_NORMAL;
        break;
    case VID_PLA_DISP_FMT_LETTERBOX:
        *pe_scr_mode = SCC_VID_SCREEN_MODE_LETTERBOX;
        break;
    case VID_PLA_DISP_FMT_PAN_SCAN:
        *pe_scr_mode = SCC_VID_SCREEN_MODE_PAN_SCAN;
        break;
    case VID_PLA_DISP_FMT_USER_DEFINED:
        *pe_scr_mode = SCC_VID_SCREEN_MODE_CUSTOM_DEF_0;
        break;
    case VID_PLA_DISP_FMT_NON_LINEAR_ZOOM:
        *pe_scr_mode = SCC_VID_SCREEN_MODE_NON_LINEAR_ZOOM;
        break;
    case VID_PLA_DISP_FMT_DOT_BY_DOT:
        *pe_scr_mode = SCC_VID_SCREEN_MODE_DOT_BY_DOT;
        break;
    case VID_PLA_DISP_FMT_CUSTOM_0:
        *pe_scr_mode = SCC_VID_SCREEN_MODE_CUSTOM_DEF_0;
        break;
    case VID_PLA_DISP_FMT_CUSTOM_1:
        *pe_scr_mode = SCC_VID_SCREEN_MODE_CUSTOM_DEF_1;
        break;
    case VID_PLA_DISP_FMT_CUSTOM_2:
        *pe_scr_mode = SCC_VID_SCREEN_MODE_CUSTOM_DEF_2;
        break;
    case VID_PLA_DISP_FMT_CUSTOM_3:
        *pe_scr_mode = SCC_VID_SCREEN_MODE_CUSTOM_DEF_3;
        break;
    case VID_PLA_DISP_FMT_CUSTOM_4:
        *pe_scr_mode = SCC_VID_SCREEN_MODE_CUSTOM_DEF_4;
        break;
    case VID_PLA_DISP_FMT_CUSTOM_5:
        *pe_scr_mode = SCC_VID_SCREEN_MODE_CUSTOM_DEF_5;
        break;
    case VID_PLA_DISP_FMT_CUSTOM_6:
        *pe_scr_mode = SCC_VID_SCREEN_MODE_CUSTOM_DEF_6;
        break;
    case VID_PLA_DISP_FMT_CUSTOM_7:
        *pe_scr_mode = SCC_VID_SCREEN_MODE_CUSTOM_DEF_7;
        break;
    case VID_PLA_DISP_FMT_NLZ_CUSTOM_0:
        *pe_scr_mode = SCC_VID_SCREEN_MODE_NLZ_CUSTOM_DEF_0;
        break;
    case VID_PLA_DISP_FMT_NLZ_CUSTOM_1:
        *pe_scr_mode = SCC_VID_SCREEN_MODE_NLZ_CUSTOM_DEF_1;
        break;
    case VID_PLA_DISP_FMT_NLZ_CUSTOM_2:
        *pe_scr_mode = SCC_VID_SCREEN_MODE_NLZ_CUSTOM_DEF_2;
        break;
    case VID_PLA_DISP_FMT_NLZ_CUSTOM_3:
        *pe_scr_mode = SCC_VID_SCREEN_MODE_NLZ_CUSTOM_DEF_3;
        break;
    default:
        return SMR_DRV_COMP_ERROR;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_enhance
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pui2_enhance        References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_enhance( HANDLE_T                h_vid,
                             UINT16*                 pui2_enhance )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_ENHANCE_T            t_drv_vdp_enh = {0};
    VID_PLA_MIN_MAX_INFO_T       t_drv_min_max = {0};

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpui2_enhance     = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pui2_enhance ));

    if ( !pui2_enhance )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_min_max.e_get_type = VID_PLA_GET_TYPE_ENHANCE;
    z_size = sizeof(VID_PLA_MIN_MAX_INFO_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MIN_MAX,
        (VOID*)&t_drv_min_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }
    if ( t_drv_min_max.ui4_min_value >= t_drv_min_max.ui4_max_value ||
         t_drv_min_max.ui4_max_value > 0xFF ||
         t_drv_min_max.ui4_min_value > 0xFF )
    {
        return SMR_DRV_COMP_ERROR;
    }

    t_drv_vdp_enh.e_mode = VID_PLA_ENHANCE;
    t_drv_vdp_enh.ui1_level = 0;

    z_size = sizeof(VID_PLA_ENHANCE_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_ENHANCE,
        (VOID*)&t_drv_vdp_enh,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    if ( (UINT32)t_drv_vdp_enh.ui1_level > t_drv_min_max.ui4_max_value ||
         (UINT32)t_drv_vdp_enh.ui1_level < t_drv_min_max.ui4_min_value )
    {
        return SMR_DRV_COMP_ERROR;
    }

    *pui2_enhance = _floor_16bits_drv_val_2_scc_val(
        SCC_ATTRIB_VALUE_MAX,
        t_drv_vdp_enh.ui1_level,
        (UINT16)t_drv_min_max.ui4_min_value,
        (UINT16)t_drv_min_max.ui4_max_value );

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_blur
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pui1_blur           References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_blur( HANDLE_T                   h_vid,
                          UINT16*                    pui2_blur )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_ENHANCE_T            t_drv_vdp_enh;
    VID_PLA_MIN_MAX_INFO_T       t_drv_min_max = {0};

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpui2_blur        = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pui2_blur ));

    if ( !pui2_blur )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_min_max.e_get_type = VID_PLA_GET_TYPE_ENHANCE;
    z_size = sizeof(VID_PLA_MIN_MAX_INFO_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MIN_MAX,
        (VOID*)&t_drv_min_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }
    if ( t_drv_min_max.ui4_min_value >= t_drv_min_max.ui4_max_value ||
         t_drv_min_max.ui4_max_value > 0xFF ||
         t_drv_min_max.ui4_min_value > 0xFF )
    {
        return SMR_DRV_COMP_ERROR;
    }

    t_drv_vdp_enh.e_mode = VID_PLA_BLUR;
    t_drv_vdp_enh.ui1_level = 0;

    z_size = sizeof(VID_PLA_ENHANCE_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_ENHANCE,
        (VOID*)&t_drv_vdp_enh,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    if ( (UINT32)t_drv_vdp_enh.ui1_level > t_drv_min_max.ui4_max_value ||
         (UINT32)t_drv_vdp_enh.ui1_level < t_drv_min_max.ui4_min_value )
    {
        return SMR_DRV_COMP_ERROR;
    }

    *pui2_blur = _floor_16bits_drv_val_2_scc_val(
        SCC_ATTRIB_VALUE_MAX,
        t_drv_vdp_enh.ui1_level,
        (UINT16)t_drv_min_max.ui4_min_value,
        (UINT16)t_drv_min_max.ui4_max_value );

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_blur
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pui1_blur           References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_overscan_clipper(
    HANDLE_T                   h_vid,
    SCC_VID_OVER_SCAN_CLIPPER_T* pt_clipper )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_OVER_SCAN_CLIPPER_T  t_drv_clipper = {0};

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpt_clipper       = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pt_clipper ));

    if ( !pt_clipper )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(VID_PLA_OVER_SCAN_CLIPPER_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_OVER_SCAN_CLIPPER,
        (VOID*)&t_drv_clipper,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    pt_clipper->ui4_top = t_drv_clipper.ui4_top;
    pt_clipper->ui4_bottom = t_drv_clipper.ui4_bottom;
    pt_clipper->ui4_left = t_drv_clipper.ui4_left;
    pt_clipper->ui4_right = t_drv_clipper.ui4_right;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_cti
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pui2_cti            References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_cti( HANDLE_T                h_vid,
                         UINT16*                 pui2_cti )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_MIN_MAX_INFO_T       t_drv_min_max = {0};
    UINT8                        ui1_drv_cti = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpui2_cti         = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pui2_cti ));

    if ( !pui2_cti )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_min_max.e_get_type = VID_PLA_GET_TYPE_CTI;
    z_size = sizeof(VID_PLA_MIN_MAX_INFO_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MIN_MAX,
        (VOID*)&t_drv_min_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }
    if ( t_drv_min_max.ui4_min_value >= t_drv_min_max.ui4_max_value ||
         t_drv_min_max.ui4_max_value > 0xFF ||
         t_drv_min_max.ui4_min_value > 0xFF )
    {
        return SMR_DRV_COMP_ERROR;
    }

    z_size = sizeof(UINT8);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_CTI,
        (VOID*)&ui1_drv_cti,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    if ( (UINT32)ui1_drv_cti > t_drv_min_max.ui4_max_value ||
         (UINT32)ui1_drv_cti < t_drv_min_max.ui4_min_value )
    {
        return SMR_DRV_COMP_ERROR;
    }

    *pui2_cti = _floor_16bits_drv_val_2_scc_val(
        SCC_ATTRIB_VALUE_MAX,
        ui1_drv_cti,
        (UINT16)t_drv_min_max.ui4_min_value,
        (UINT16)t_drv_min_max.ui4_max_value );

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_deblocking
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pt_deblocking       References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_deblocking(
    HANDLE_T                   h_vid,
    SCC_VID_DEBLOCKING_INFO_T* pt_deblocking )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_DEC_DEBLOCKING_INFO_T    t_vid_deblock = {0};
    SIZE_T                       z_size = sizeof(VID_DEC_DEBLOCKING_INFO_T);

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tpt_deblocking     = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pt_deblocking ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    /* Effect time should not exceed 1000ms */
    if ( !pt_deblocking )
    {
        return SMR_INV_ARG;
    }

    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_DEC,
        VID_DEC_GET_TYPE_DEBLOCKING,
        (VOID*)(UINT32)&t_vid_deblock,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    pt_deblocking->b_is_on = t_vid_deblock.b_is_on;
    pt_deblocking->u.t_region.ui4_x = t_vid_deblock.u.t_region.ui4_x;
    pt_deblocking->u.t_region.ui4_y = t_vid_deblock.u.t_region.ui4_y;
    pt_deblocking->u.t_region.ui4_w = t_vid_deblock.u.t_region.ui4_w;
    pt_deblocking->u.t_region.ui4_h = t_vid_deblock.u.t_region.ui4_h;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_split_scrn_demo
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pe_splt_scrn_demo   References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_split_scrn_demo(
    HANDLE_T                   h_vid,
    SCC_VID_SPLIT_SCRN_DEMO_T* pe_splt_scrn_demo )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL; 
    VID_PLA_SPLIT_SCRN_DEMO_T    e_drv_splt_scr_demo = VID_PLA_SPLIT_SCRN_DEMO_OFF;
    SIZE_T                       z_size = sizeof(VID_PLA_SPLIT_SCRN_DEMO_T);

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tpe_splt_scrn_demo = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pe_splt_scrn_demo ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    /* Effect time should not exceed 1000ms */
    if ( !pe_splt_scrn_demo )
    {
        return SMR_INV_ARG;
    }

    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_SPLIT_SCRN_DEMO,
        (VOID*)(UINT32)&e_drv_splt_scr_demo,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    switch( e_drv_splt_scr_demo )
    {
    case VID_PLA_SPLIT_SCRN_DEMO_OFF:
        *pe_splt_scrn_demo = SCC_VID_SPLIT_SCRN_DEMO_OFF;
        break;

    case VID_PLA_SPLIT_SCRN_DEMO_LEFT:
        *pe_splt_scrn_demo = SCC_VID_SPLIT_SCRN_DEMO_LEFT;
        break;

    case VID_PLA_SPLIT_SCRN_DEMO_RIGHT:
        *pe_splt_scrn_demo = SCC_VID_SPLIT_SCRN_DEMO_RIGHT;
        break;

    default:
        return SMR_DRV_COMP_ERROR;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_eti
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pui2_eti            References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_eti( HANDLE_T                h_vid,
                         UINT16*                 pui2_eti )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_MIN_MAX_INFO_T       t_drv_min_max = {0};
    UINT8                        ui1_drv_eti = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpui2_eti         = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pui2_eti ));

    if ( !pui2_eti )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_min_max.e_get_type = VID_PLA_GET_TYPE_ETI;
    z_size = sizeof(VID_PLA_MIN_MAX_INFO_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MIN_MAX,
        (VOID*)&t_drv_min_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }
    if ( t_drv_min_max.ui4_min_value >= t_drv_min_max.ui4_max_value ||
         t_drv_min_max.ui4_max_value > 0xFF ||
         t_drv_min_max.ui4_min_value > 0xFF )
    {
        return SMR_DRV_COMP_ERROR;
    }

    z_size = sizeof(UINT8);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_ETI,
        (VOID*)&ui1_drv_eti,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    if ( (UINT32)ui1_drv_eti > t_drv_min_max.ui4_max_value ||
         (UINT32)ui1_drv_eti < t_drv_min_max.ui4_min_value )
    {
        return SMR_DRV_COMP_ERROR;
    }

    *pui2_eti = _floor_16bits_drv_val_2_scc_val(
        SCC_ATTRIB_VALUE_MAX,
        ui1_drv_eti,
        (UINT16)t_drv_min_max.ui4_min_value,
        (UINT16)t_drv_min_max.ui4_max_value );

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_sharpness
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pui2_sharpness            References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_sharpness( HANDLE_T                h_vid,
                             UINT16*                 pui2_sharpness )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_MIN_MAX_INFO_T       t_drv_min_max = {0};
    UINT8                        ui1_drv_sharpness = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpui2_sharpness         = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pui2_sharpness ));

    if ( !pui2_sharpness )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_min_max.e_get_type = VID_PLA_GET_TYPE_SHARPNESS;
    z_size = sizeof(VID_PLA_MIN_MAX_INFO_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MIN_MAX,
        (VOID*)&t_drv_min_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }
    if ( t_drv_min_max.ui4_min_value >= t_drv_min_max.ui4_max_value ||
         t_drv_min_max.ui4_max_value > 0xFF ||
         t_drv_min_max.ui4_min_value > 0xFF )
    {
        return SMR_DRV_COMP_ERROR;
    }

    z_size = sizeof(UINT8);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_SHARPNESS,
        (VOID*)&ui1_drv_sharpness,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    if ( (UINT32)ui1_drv_sharpness > t_drv_min_max.ui4_max_value ||
         (UINT32)ui1_drv_sharpness < t_drv_min_max.ui4_min_value )
    {
        return SMR_DRV_COMP_ERROR;
    }

    *pui2_sharpness = _floor_16bits_drv_val_2_scc_val(
        SCC_ATTRIB_VALUE_MAX,
        ui1_drv_sharpness,
        (UINT16)t_drv_min_max.ui4_min_value,
        (UINT16)t_drv_min_max.ui4_max_value );

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_color_supress
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pui2_color_supress  References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_color_supress( HANDLE_T                h_vid,
                                   UINT16*                 pui2_color_supress )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_MIN_MAX_INFO_T       t_drv_min_max = {0};
    UINT8                        ui1_drv_color_supress = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp             = 0x%.8X\r\n"
                       "\t\tpui2_color_supress = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pui2_color_supress ));

    if ( !pui2_color_supress )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_min_max.e_get_type = VID_PLA_GET_TYPE_COLOR_SUPPRESS;
    z_size = sizeof(VID_PLA_MIN_MAX_INFO_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MIN_MAX,
        (VOID*)&t_drv_min_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }
    if ( t_drv_min_max.ui4_min_value >= t_drv_min_max.ui4_max_value ||
         t_drv_min_max.ui4_max_value > 0xFF ||
         t_drv_min_max.ui4_min_value > 0xFF )
    {
        return SMR_DRV_COMP_ERROR;
    }

    z_size = sizeof(UINT8);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_COLOR_SUPPRESS,
        (VOID*)&ui1_drv_color_supress,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    if ( (UINT32)ui1_drv_color_supress > t_drv_min_max.ui4_max_value ||
         (UINT32)ui1_drv_color_supress < t_drv_min_max.ui4_min_value )
    {
        return SMR_DRV_COMP_ERROR;
    }

    *pui2_color_supress = _floor_16bits_drv_val_2_scc_val(
        SCC_ATTRIB_VALUE_MAX,
        ui1_drv_color_supress,
        (UINT16)t_drv_min_max.ui4_min_value,
        (UINT16)t_drv_min_max.ui4_max_value );

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_nr
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pui2_nr  References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_nr( HANDLE_T                h_vid,
                        UINT16*                 pui2_nr )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_MAX_INFO_T           t_drv_max = {0};
    UINT8                        ui1_drv_nr = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp             = 0x%.8X\r\n"
                       "\t\tpui2_nr            = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pui2_nr ));

    if ( !pui2_nr )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_max.e_get_type = VID_PLA_GET_TYPE_NR;
    z_size = sizeof(VID_PLA_MAX_INFO_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MAX,
        (VOID*)&t_drv_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }
    if ( t_drv_max.ui4_max_value > 0xFF )
    {
        return SMR_DRV_COMP_ERROR;
    }

    z_size = sizeof(UINT8);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_NR,
        (VOID*)&ui1_drv_nr,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    if ( (UINT32)ui1_drv_nr > t_drv_max.ui4_max_value )
    {
        return SMR_DRV_COMP_ERROR;
    }

    *pui2_nr = _floor_16bits_drv_val_2_scc_val(
        SCC_ATTRIB_VALUE_MAX,
        ui1_drv_nr,
        (UINT16)0,
        (UINT16)t_drv_max.ui4_max_value );

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_3dnr
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pui2_nr  References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_3dnr( HANDLE_T                h_vid,
                          UINT16*                 pui2_3dnr )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_MAX_INFO_T           t_drv_max = {0};
    UINT8                        ui1_drv_3dnr = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp             = 0x%.8X\r\n"
                       "\t\tpui2_3dnr          = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pui2_3dnr ));

    if ( !pui2_3dnr )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_max.e_get_type = VID_PLA_GET_TYPE_3D_NR;
    z_size = sizeof(VID_PLA_MAX_INFO_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MAX,
        (VOID*)&t_drv_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }
    if ( t_drv_max.ui4_max_value > 0xFF )
    {
        return SMR_DRV_COMP_ERROR;
    }

    z_size = sizeof(UINT8);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_3D_NR,
        (VOID*)&ui1_drv_3dnr,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    if ( (UINT32)ui1_drv_3dnr > t_drv_max.ui4_max_value )
    {
        return SMR_DRV_COMP_ERROR;
    }

    *pui2_3dnr = _floor_16bits_drv_val_2_scc_val(
        SCC_ATTRIB_VALUE_MAX,
        ui1_drv_3dnr,
        (UINT16)0,
        (UINT16)t_drv_max.ui4_max_value );

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_black_lvl_ext
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pui2_black_lvl_ext  References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_black_lvl_ext( HANDLE_T                h_vid,
                                   UINT16*                 pui2_black_lvl_ext )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_MAX_INFO_T           t_drv_max = {0};
    UINT8                        ui1_drv_black_lvl_ext = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp             = 0x%.8X\r\n"
                       "\t\tpui2_black_lvl_ext = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pui2_black_lvl_ext ));

    if ( !pui2_black_lvl_ext )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_max.e_get_type = VID_PLA_GET_TYPE_BLACK_LVL_EXT;
    z_size = sizeof(VID_PLA_MAX_INFO_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MAX,
        (VOID*)&t_drv_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }
    if ( t_drv_max.ui4_max_value > 0xFF )
    {
        return SMR_DRV_COMP_ERROR;
    }

    z_size = sizeof(UINT8);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_BLACK_LVL_EXT,
        (VOID*)&ui1_drv_black_lvl_ext,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    if ( (UINT32)ui1_drv_black_lvl_ext > t_drv_max.ui4_max_value )
    {
        return SMR_DRV_COMP_ERROR;
    }

    *pui2_black_lvl_ext = _floor_16bits_drv_val_2_scc_val(
        SCC_ATTRIB_VALUE_MAX,
        ui1_drv_black_lvl_ext,
        (UINT16)0,
        (UINT16)t_drv_max.ui4_max_value );

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_white_peak_rdct
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *
 * Outputs: pui2_white_peak_rdct  References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_white_peak_rdct( HANDLE_T            h_vid,
                                     UINT16*             pui2_white_peak_rdct )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_MAX_INFO_T           t_drv_max = {0};
    UINT8                        ui1_drv_white_peak_rdct = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp               = 0x%.8X\r\n"
                       "\t\tpui2_white_peak_rdct = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pui2_white_peak_rdct ));

    if ( !pui2_white_peak_rdct )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_max.e_get_type = VID_PLA_GET_TYPE_WHITE_PEAK_LMT;
    z_size = sizeof(VID_PLA_MAX_INFO_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MAX,
        (VOID*)&t_drv_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }
    if ( t_drv_max.ui4_max_value > 0xFF )
    {
        return SMR_DRV_COMP_ERROR;
    }

    z_size = sizeof(UINT8);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_WHITE_PEAK_LMT,
        (VOID*)&ui1_drv_white_peak_rdct,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    if ( (UINT32)ui1_drv_white_peak_rdct > t_drv_max.ui4_max_value )
    {
        return SMR_DRV_COMP_ERROR;
    }

    *pui2_white_peak_rdct = _floor_16bits_drv_val_2_scc_val(
        SCC_ATTRIB_VALUE_MAX,
        ui1_drv_white_peak_rdct,
        (UINT16)0,
        (UINT16)t_drv_max.ui4_max_value );

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_flesh_tone
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *
 * Outputs: pui2_flesh_tone       References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_flesh_tone( HANDLE_T            h_vid,
                                UINT16*             pui2_flesh_tone )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_MAX_INFO_T           t_drv_max = {0};
    UINT8                        ui1_drv_flesh_tone = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp               = 0x%.8X\r\n"
                       "\t\tpui2_flesh_tone      = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pui2_flesh_tone ));

    if ( !pui2_flesh_tone )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_max.e_get_type = VID_PLA_GET_TYPE_FLESH_TONE;
    z_size = sizeof(VID_PLA_MAX_INFO_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MAX,
        (VOID*)&t_drv_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }
    if ( t_drv_max.ui4_max_value > 0xFF )
    {
        return SMR_DRV_COMP_ERROR;
    }

    z_size = sizeof(UINT8);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_FLESH_TONE,
        (VOID*)&ui1_drv_flesh_tone,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    if ( (UINT32)ui1_drv_flesh_tone > t_drv_max.ui4_max_value )
    {
        return SMR_DRV_COMP_ERROR;
    }

    *pui2_flesh_tone = _floor_16bits_drv_val_2_scc_val(
        SCC_ATTRIB_VALUE_MAX,
        ui1_drv_flesh_tone,
        (UINT16)0,
        (UINT16)t_drv_max.ui4_max_value );

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_luma
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pui2_luma           References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_luma( HANDLE_T            h_vid,
                          UINT16*             pui2_luma )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_MAX_INFO_T           t_drv_max = {0};
    UINT8                        ui1_drv_luma = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp               = 0x%.8X\r\n"
                       "\t\tpui2_luma            = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pui2_luma ));

    if ( !pui2_luma )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_max.e_get_type = VID_PLA_GET_TYPE_LUMA;
    z_size = sizeof(VID_PLA_MAX_INFO_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MAX,
        (VOID*)&t_drv_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }
    if ( t_drv_max.ui4_max_value > 0xFF )
    {
        return SMR_DRV_COMP_ERROR;
    }

    z_size = sizeof(UINT8);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_LUMA,
        (VOID*)&ui1_drv_luma,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    if ( (UINT32)ui1_drv_luma > t_drv_max.ui4_max_value )
    {
        return SMR_DRV_COMP_ERROR;
    }

    *pui2_luma = _floor_16bits_drv_val_2_scc_val(
        SCC_ATTRIB_VALUE_MAX,
        ui1_drv_luma,
        (UINT16)0,
        (UINT16)t_drv_max.ui4_max_value );

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_letter_box_detect
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pb_is_on            References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_letter_box_detect(
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_on )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    BOOL                         b_is_on = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp               = 0x%.8X\r\n"
                       "\t\tpb_is_on             = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pb_is_on ));

    if ( !pb_is_on )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(BOOL);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_LETTER_BOX_DETECT,
        (VOID*)&b_is_on,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    *pb_is_on = b_is_on;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_lb_detect_config
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pt_config           References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_lb_detect_config(
    HANDLE_T                   h_vid,
    SCC_VID_LB_DETECT_CONFIG_T* pt_config )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_LB_DETECT_CONFIG_T   t_vid_lb_detect = {0};
    SIZE_T                       z_size = sizeof(VID_PLA_LB_DETECT_CONFIG_T);

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tpt_config         = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pt_config ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    if ( !pt_config )
    {
        return SMR_INV_ARG;
    }

    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_LB_DETECT_CONFIG,
        (VOID*)(UINT32)&t_vid_lb_detect,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    pt_config->ui4_time_threshold = t_vid_lb_detect.ui4_time_threshold;
    pt_config->ui4_bounds_top = t_vid_lb_detect.ui4_bounds_top;
    pt_config->ui4_bounds_bottom = t_vid_lb_detect.ui4_bounds_bottom;
    pt_config->ui4_bounds_left = t_vid_lb_detect.ui4_bounds_left;
    pt_config->ui4_bounds_right = t_vid_lb_detect.ui4_bounds_right;
    pt_config->b_manual_mode = t_vid_lb_detect.b_manual_mode;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_dynamic_scaling
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pui2_effect_time    References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_dynamic_scaling(
    HANDLE_T                   h_vid,
    UINT16*                    pui2_effect_time )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    UINT16                       ui2_effect_time = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp               = 0x%.8X\r\n"
                       "\t\tpui2_effect_time     = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pui2_effect_time ));

    if ( !pui2_effect_time )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(UINT16);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_DYNAMIC_SCALING,
        (VOID*)&ui2_effect_time,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    *pui2_effect_time = ui2_effect_time;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_h_position
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_h_position(
    HANDLE_T                   h_vid,
    UINT16*                    pui2_min,
    UINT16*                    pui2_max,
    UINT16*                    pui2_curr )
{
    return x_scc_vid_get_position_size(
            h_vid,
            VID_PLA_GET_TYPE_H_POSITION,
            pui2_min,
            pui2_max,
            pui2_curr );
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_v_position
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_v_position(
    HANDLE_T                   h_vid,
    UINT16*                    pui2_min,
    UINT16*                    pui2_max,
    UINT16*                    pui2_curr )
{
    return x_scc_vid_get_position_size(
            h_vid,
            VID_PLA_GET_TYPE_V_POSITION,
            pui2_min,
            pui2_max,
            pui2_curr );
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_h_size
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_h_size(
    HANDLE_T                   h_vid,
    UINT16*                    pui2_min,
    UINT16*                    pui2_max,
    UINT16*                    pui2_curr )
{
    return x_scc_vid_get_position_size(
            h_vid,
            VID_PLA_GET_TYPE_H_SIZE,
            pui2_min,
            pui2_max,
            pui2_curr );
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_v_size
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_v_size(
    HANDLE_T                   h_vid,
    UINT16*                    pui2_min,
    UINT16*                    pui2_max,
    UINT16*                    pui2_curr )
{
    return x_scc_vid_get_position_size(
            h_vid,
            VID_PLA_GET_TYPE_V_SIZE,
            pui2_min,
            pui2_max,
            pui2_curr );
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_position_size
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pui2_curr           References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_position_size(
    HANDLE_T                   h_vid,
    UINT32                     e_get_type,
    UINT16*                    pui2_min,
    UINT16*                    pui2_max,
    UINT16*                    pui2_curr )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_MIN_MAX_INFO_T       t_drv_min_max = {0};
    UINT16                       ui2_drv_offset = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid           = 0x%.8X\r\n"
                       "\t\tpui2_curr       = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pui2_curr ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_min_max.e_get_type = e_get_type;
    z_size = sizeof(VID_PLA_MIN_MAX_INFO_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MIN_MAX,
        (VOID*)&t_drv_min_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }
    if ( t_drv_min_max.ui4_min_value >= t_drv_min_max.ui4_max_value )
    {
        return SMR_DRV_COMP_ERROR;
    }

    *pui2_min = (UINT16)t_drv_min_max.ui4_min_value;
    *pui2_max = (UINT16)t_drv_min_max.ui4_max_value;

    if ( pui2_curr )
    {
        z_size = sizeof(UINT16);
        i4_ret = _scc_comp_vid_get(
            h_vid,
            DRVT_VID_PLANE,
            e_get_type,
            (VOID*)&ui2_drv_offset,
            &z_size );
        if ( i4_ret != SMR_OK )
        {
            return SMR_INV_GET_INFO;
        }

        if ( (UINT32)ui2_drv_offset > t_drv_min_max.ui4_max_value ||
             (UINT32)ui2_drv_offset < t_drv_min_max.ui4_min_value )
        {
            return SMR_DRV_COMP_ERROR;
        }

        *pui2_curr = ui2_drv_offset;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_film_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pb_is_on            References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_film_mode(
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_on )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    BOOL                         b_is_on = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid                = 0x%.8X\r\n"
                       "\t\tpb_is_on             = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pb_is_on ));

    if ( !pb_is_on )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(BOOL);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_FILM_MODE,
        (VOID*)&b_is_on,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    *pb_is_on = b_is_on;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_hdcp_key_exist
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pb_is_exist         References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_hdcp_key_exist(
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_exist )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    BOOL                         b_is_exist = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid                = 0x%.8X\r\n"
                       "\t\tpb_is_exist          = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pb_is_exist ));

    if ( !pb_is_exist )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(BOOL);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_HDCP_KEY_EXIST,
        (VOID*)&b_is_exist,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    *pb_is_exist = b_is_exist;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _scc_vid_iframe_nfy_fct
 *
 * Description: <function description>
 *
 * Inputs:  pv_nfy_tag References.
 *          e_nfy_cond References.
 *          ui4_data_1 References.
 *          ui4_data_2 References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _scc_vid_iframe_nfy_fct(
    VOID*           pv_nfy_tag,
    VID_DEC_COND_T  e_nfy_cond,
    UINT32          ui4_data_1,
    UINT32          ui4_data_2 )
{
    INT32               i4_ret;
    HANDLE_TYPE_T       e_type;
    SCC_VIDEO_COMP_T*   pt_vid_comp = NULL;
    SM_SCC_MSG_T        t_msg ;
    HANDLE_T            h_vid_iframe = (HANDLE_T)pv_nfy_tag;

    i4_ret = handle_get_type_obj( h_vid_iframe,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return;
    }

    switch (e_nfy_cond)
    {
        case VID_DEC_COND_ERROR:
            t_msg.u.t_vid_iframe_nfy.e_nfy_cond = SCC_VID_IFRAME_COND_ERROR;
            break;
        case VID_DEC_COND_CTRL_DONE:
            t_msg.u.t_vid_iframe_nfy.e_nfy_cond = SCC_VID_IFRAME_COND_CTRL_DONE;
            break;
        case VID_DEC_COND_I_FRAME_SET_BUFF_DONE:
            t_msg.u.t_vid_iframe_nfy.e_nfy_cond = SCC_VID_IFRAME_COND_SET_BUFF_DONE;
            break;
        case VID_DEC_COND_I_FRAME_DECODE_DONE:
            t_msg.u.t_vid_iframe_nfy.e_nfy_cond = SCC_VID_IFRAME_COND_DECODE_DONE;
            break;
#ifdef SYS_MHP_SUPPORT
        case VID_DEC_COND_THUMBNAIL_DONE:
            t_msg.u.t_vid_iframe_nfy.e_nfy_cond = SCC_VID_IFRAME_COND_THUMBNAIL_DONE;
            break;
#endif
        default:
            return;
    }
    t_msg.e_msg_type = SM_SCC_MSG_TYPE_IFRAME_NFY;
    t_msg.u.t_vid_iframe_nfy.h_vid = h_vid_iframe;
    t_msg.u.t_vid_iframe_nfy.pf_nfy_fct = pt_vid_comp->pf_iframe_nfy;
    t_msg.u.t_vid_iframe_nfy.pv_nfy_tag = pt_vid_comp->pv_iframe_nfy_tag;
    t_msg.u.t_vid_iframe_nfy.ui4_data_1 = ui4_data_1;
    t_msg.u.t_vid_iframe_nfy.ui4_data_2 = ui4_data_2;
    _scc_send_msg(&t_msg);

}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_iframe_nfy
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/

INT32 x_scc_vid_set_iframe_nfy(
    HANDLE_T                     h_vid,
    x_scc_vid_iframe_nfy_fct     pf_scc_iframe_nfy,
    VOID*                        pv_nfy_tag )
{
    INT32                         i4_ret;
    HANDLE_TYPE_T                 e_type;
    SCC_VIDEO_COMP_T*             pt_vid_comp = NULL;
    RM_COND_T                     e_cond = RM_COND_IGNORE ;
    VID_DEC_NFY_INFO_T            t_vd_nfy_info = {0};

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tpf_scc_iframe_nfy             = %x )\r\n",
                       "\t\tpv_nfy_tag             = %x )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)pf_scc_iframe_nfy ,
                       (INT32)pv_nfy_tag ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    pt_vid_comp->pf_iframe_nfy = pf_scc_iframe_nfy;
    pt_vid_comp->pv_iframe_nfy_tag = pv_nfy_tag;

    t_vd_nfy_info.pf_vid_dec_nfy = (x_vid_dec_nfy_fct)_scc_vid_iframe_nfy_fct;
    t_vd_nfy_info.pv_tag = (VOID*)h_vid;

    if(x_handle_valid(pt_vid_comp->t_iframe_vd_comp_descr.h_comp) == FALSE)
    {
        i4_ret = _scc_iframe_build_comp_connect(h_vid);
        if ( i4_ret != SMR_OK )
        {
            return SMR_INV_SET_INFO;
        }
    }

    i4_ret = rm_set(
        pt_vid_comp->t_iframe_vd_comp_descr.h_comp,
        FALSE,
        ANY_PORT_NUM,
        VID_DEC_SET_TYPE_I_FRAME_NFY_FCT,
        (VOID*)&t_vd_nfy_info,
        sizeof(VID_DEC_NFY_INFO_T),
        &e_cond);
    if ( i4_ret != RMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;

}

#ifdef SYS_MHP_SUPPORT
/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_mhp_iframe_nfy
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/

INT32 x_scc_vid_set_mhp_iframe_nfy(
    HANDLE_T                     h_vid,
    x_scc_vid_iframe_nfy_fct     pf_scc_iframe_nfy,
    VOID*                        pv_nfy_tag )
{
    INT32                         i4_ret;
    HANDLE_TYPE_T                 e_type;
    SCC_VIDEO_COMP_T*             pt_vid_comp = NULL;
    RM_COND_T                     e_cond = RM_COND_IGNORE;
    VID_DEC_NFY_INFO_T            t_vd_nfy_info = {0};

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid                = 0x%.8X\r\n"
                       "\t\tpf_scc_iframe_nfy    = %x )\r\n",
                       "\t\tpv_nfy_tag           = %x )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)pf_scc_iframe_nfy ,
                       (INT32)pv_nfy_tag ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    pt_vid_comp->pf_iframe_nfy = pf_scc_iframe_nfy;
    pt_vid_comp->pv_iframe_nfy_tag = pv_nfy_tag;

    t_vd_nfy_info.pf_vid_dec_nfy = (x_vid_dec_nfy_fct)_scc_vid_iframe_nfy_fct;
    t_vd_nfy_info.pv_tag = (VOID*)h_vid;

    if(x_handle_valid(pt_vid_comp->t_iframe_vd_comp_descr.h_comp) == FALSE)
    {
        i4_ret = _scc_mhp_iframe_comp_open(h_vid);
        if ( i4_ret != SMR_OK )
        {
            return SMR_INV_SET_INFO;
        }
    }

    i4_ret = rm_set(
        pt_vid_comp->t_iframe_vd_comp_descr.h_comp,
        FALSE,
        ANY_PORT_NUM,
        VID_DEC_SET_TYPE_I_FRAME_NFY_FCT,
        (VOID*)&t_vd_nfy_info,
        sizeof(VID_DEC_NFY_INFO_T),
        &e_cond);
    if ( i4_ret != RMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_mhp_iframe_close
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/

INT32 x_scc_vid_set_mhp_iframe_close(
    HANDLE_T                     h_vid )
{
    INT32                         i4_ret;
    HANDLE_TYPE_T                 e_type;
    SCC_VIDEO_COMP_T*             pt_vid_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid          = 0x%.8X\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid
                       ));
    
    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    sm_drv_comp_close( &pt_vid_comp->t_iframe_vd_comp_descr );
    
    return SMR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_iframe_alloc_buffer
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/

INT32 x_scc_vid_iframe_alloc_buffer(
    HANDLE_T                     h_vid,
    SCC_VID_IFRAME_BUFF_T*       pt_iframe_buf)
{
    INT32                         i4_ret;
    HANDLE_TYPE_T                 e_type;
    SCC_VIDEO_COMP_T*             pt_vid_comp = NULL;
    VID_DEC_I_FRAME_BUFF_T        t_drv_iframe_buff = {0};
    RM_COND_T                     e_cond = RM_COND_IGNORE;

#ifdef LINUX_TURNKEY_SOLUTION 
    SIZE_T                      z_get_size = 0;
#endif

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tpt_iframe_buf     = %x )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)pt_iframe_buf));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_iframe_buff.z_frame_size = pt_iframe_buf->z_iframe_size;
    t_drv_iframe_buff.pc_frame_buff_sp = NULL;

#ifdef LINUX_TURNKEY_SOLUTION
    z_get_size = sizeof(VID_DEC_I_FRAME_BUFF_T);
    i4_ret = rm_get(
        pt_vid_comp->t_iframe_vd_comp_descr.h_comp,
        FALSE,
        ANY_PORT_NUM,
        VID_DEC_GET_TYPE_ALLOC_I_FRAME_BUFF,
        (VOID*)&t_drv_iframe_buff,
        &z_get_size,
        &e_cond);
    if ( i4_ret != RMR_OK )
    {
        return SMR_INV_GET_INFO;
    }
#else
    i4_ret = rm_set(
        pt_vid_comp->t_iframe_vd_comp_descr.h_comp,
        FALSE,
        ANY_PORT_NUM,
        VID_DEC_SET_TYPE_ALLOC_I_FRAME_BUFF,
        (VOID*)&t_drv_iframe_buff,
        sizeof(VID_DEC_I_FRAME_BUFF_T),
        &e_cond);
    if ( i4_ret != RMR_OK )
    {
        return SMR_INV_SET_INFO;
    }
#endif

    pt_iframe_buf->puc_iframe_buff = t_drv_iframe_buff.pc_frame_buff_sp;

    return SMR_OK;

}
/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_iframe_buffer
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/

INT32 x_scc_vid_set_iframe_buffer(
    HANDLE_T                     h_vid,
    SCC_VID_IFRAME_BUFF_T*       pt_iframe_buf)
{
    INT32                         i4_ret;
    HANDLE_TYPE_T                 e_type;
    SCC_VIDEO_COMP_T*             pt_vid_comp = NULL;
    VID_DEC_I_FRAME_BUFF_T        t_drv_iframe_buff = {0};
    RM_COND_T                     e_cond = RM_COND_IGNORE;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tpt_iframe_buf     = %x )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)pt_iframe_buf));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_iframe_buff.pc_frame_buff_sp = pt_iframe_buf->puc_iframe_buff;
    t_drv_iframe_buff.z_frame_size = pt_iframe_buf->z_iframe_size;

    i4_ret = rm_set(
        pt_vid_comp->t_iframe_vd_comp_descr.h_comp,
        FALSE,
        ANY_PORT_NUM,
        VID_DEC_SET_TYPE_I_FRAME_BUFF,
        (VOID*)&t_drv_iframe_buff,
        sizeof(VID_DEC_I_FRAME_BUFF_T),
        &e_cond);
    if ( i4_ret != RMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;

}
/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_iframe_decode
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/

INT32 x_scc_vid_iframe_decode(
    HANDLE_T                     h_vid,
    SCC_VID_IFRAME_BUFF_T*       pt_iframe_buf)
{
    INT32                         i4_ret;
    HANDLE_TYPE_T                 e_type;
    SCC_VIDEO_COMP_T*             pt_vid_comp = NULL;
    VID_DEC_I_FRAME_BUFF_T        t_drv_iframe_buff = {0};
    RM_COND_T                     e_cond = RM_COND_IGNORE;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_iframe_buff.pc_frame_buff_sp = pt_iframe_buf->puc_iframe_buff;
    t_drv_iframe_buff.z_frame_size = pt_iframe_buf->z_iframe_size;

    i4_ret = rm_set(
        pt_vid_comp->t_iframe_vd_comp_descr.h_comp,
        FALSE,
        ANY_PORT_NUM,
        VID_DEC_SET_TYPE_I_FRAME_DECODE,
        (VOID*)&t_drv_iframe_buff,
        sizeof(VID_DEC_I_FRAME_BUFF_T),
        &e_cond);
    if ( i4_ret != RMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;

}
/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_iframe_free_buffer
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/

INT32 x_scc_vid_iframe_free_buffer(
    HANDLE_T                     h_vid,
    SCC_VID_IFRAME_BUFF_T*       pt_iframe_buf)
{
    INT32                         i4_ret;
    HANDLE_TYPE_T                 e_type;
    SCC_VIDEO_COMP_T*             pt_vid_comp = NULL;
    VID_DEC_I_FRAME_BUFF_T        t_drv_iframe_buff = {0};
    RM_COND_T                     e_cond = RM_COND_IGNORE;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tpt_iframe_buf             = %x )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)pt_iframe_buf));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_iframe_buff.pc_frame_buff_sp = pt_iframe_buf->puc_iframe_buff;
    t_drv_iframe_buff.z_frame_size = pt_iframe_buf->z_iframe_size;

    i4_ret = rm_set(
        pt_vid_comp->t_iframe_vd_comp_descr.h_comp,
        FALSE,
        ANY_PORT_NUM,
        VID_DEC_SET_TYPE_FREE_I_FRAME_BUFF,
        (VOID*)&t_drv_iframe_buff,
        sizeof(VID_DEC_I_FRAME_BUFF_T),
        &e_cond);
    if ( i4_ret != RMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;

}
/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_iframe_codec
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          e_codec             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/

INT32 x_scc_vid_set_iframe_codec(
    HANDLE_T                      h_vid,
    SCC_VID_IFRAME_CODEC_T        e_codec )
{
    INT32                         i4_ret;
    HANDLE_TYPE_T                 e_type;
    SCC_VIDEO_COMP_T*             pt_vid_comp = NULL;
    VID_DEC_CTRL_T                e_vd_ctrl_mode ;
    VID_DEC_CODEC_T               e_vd_codec_type = VID_DEC_CODEC_UNKNOWN;
    RM_COND_T                     e_cond = RM_COND_IGNORE; 
    SIZE_T                        z_size;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid              = 0x%.8X\r\n"
                       "\t\te_codec             = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)e_codec ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(VID_DEC_CTRL_T);
    i4_ret = rm_get(
        pt_vid_comp->t_iframe_vd_comp_descr.h_comp,
        FALSE,
        ANY_PORT_NUM,
        VID_DEC_GET_TYPE_CTRL,
        (VOID*)&e_vd_ctrl_mode,
        &z_size,
        &e_cond);
    if ( i4_ret != RMR_OK )
    {
        return SMR_INTERNAL_ERROR;
    }
    else if ( e_vd_ctrl_mode != VID_DEC_CTRL_STOP )
    {
        return SMR_INV_SET_INFO;
    }

    switch ( e_codec )
    {
        case SCC_VID_IFRAME_CODEC_MPEG_1:
            e_vd_codec_type = VID_DEC_CODEC_MPEG_1;
            break;

        case SCC_VID_IFRAME_CODEC_MPEG_2:
            e_vd_codec_type = VID_DEC_CODEC_MPEG_2;
            break;

        case SCC_VID_IFRAME_CODEC_MPEG_4:
            e_vd_codec_type = VID_DEC_CODEC_MPEG_4;
            break;

        case SCC_VID_IFRAME_CODEC_DIVX_311:
            e_vd_codec_type = VID_DEC_CODEC_DIVX_311;
            break;

        case SCC_VID_IFRAME_CODEC_DIVX_4:
            e_vd_codec_type = VID_DEC_CODEC_DIVX_4;
            break;

        case SCC_VID_IFRAME_CODEC_DIVX_5:
            e_vd_codec_type = VID_DEC_CODEC_DIVX_5;
            break;

        case SCC_VID_IFRAME_CODEC_XVID:
            e_vd_codec_type = VID_DEC_CODEC_XVID;
            break;

        case SCC_VID_IFRAME_CODEC_WMV1:
            e_vd_codec_type = VID_DEC_CODEC_WMV1;
            break;

        case SCC_VID_IFRAME_CODEC_WMV2:
            e_vd_codec_type = VID_DEC_CODEC_WMV2;
            break;

        case SCC_VID_IFRAME_CODEC_WMV3:
            e_vd_codec_type = VID_DEC_CODEC_WMV3;
            break;

        case SCC_VID_IFRAME_CODEC_WVC1:
            e_vd_codec_type = VID_DEC_CODEC_WVC1;
            break;

        case SCC_VID_IFRAME_CODEC_H264:
             e_vd_codec_type = VID_DEC_CODEC_H264;
            break;

        case SCC_VID_IFRAME_CODEC_UNKNOWN:
        default:
             e_vd_codec_type = VID_DEC_CODEC_UNKNOWN;
    }

    i4_ret = rm_set(
        pt_vid_comp->t_iframe_vd_comp_descr.h_comp,
        FALSE,
        ANY_PORT_NUM,
        VID_DEC_SET_TYPE_CODEC,
        (VOID*)(UINT32)e_vd_codec_type,
        sizeof(VID_DEC_CODEC_T) ,
        &e_cond);
    if ( i4_ret != RMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_iframe_ctrl
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          e_ctrl_mode         References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/

INT32 x_scc_vid_set_iframe_ctrl(
    HANDLE_T                      h_vid,
    SCC_VID_IFRAME_CTRL_T         e_ctrl_mode )
{
     INT32                        i4_ret;
     HANDLE_TYPE_T                e_type;
     SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
     VID_DEC_CTRL_T               e_drv_ctrl_mode = VID_DEC_CTRL_RESET;
     RM_COND_T                    e_cond = RM_COND_IGNORE;

     SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                        "\t\th_vid              = 0x%.8X\r\n"
                        "\t\te_ctrl_mode           = %d )\r\n",
                        __FUNCTION__,
                        (UINT32)h_vid,
                        (INT32)e_ctrl_mode ));

     i4_ret = handle_get_type_obj( h_vid,
                                   &e_type,
                                   (VOID**)&pt_vid_comp );
     if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
     {
         return SMR_INV_HANDLE;
     }

     switch( e_ctrl_mode )
     {
     case SCC_VID_IFRAME_CTRL_RESET:
         e_drv_ctrl_mode = VID_DEC_CTRL_RESET;
         break;
     case SCC_VID_IFRAME_CTRL_END:
         e_drv_ctrl_mode = VID_DEC_CTRL_STOP;
         break;
     case SCC_VID_IFRAME_CTRL_BEGIN:
         e_drv_ctrl_mode = VID_DEC_CTRL_PLAY_I_FRAME;
         break;
     default:
         return SMR_INV_SET_INFO;
     }

    if(e_drv_ctrl_mode == VID_DEC_CTRL_PLAY_I_FRAME)
    { /* build component graph first and then connect it */
        if(x_handle_valid(pt_vid_comp->t_iframe_vd_comp_descr.h_comp) == FALSE)
        {
            i4_ret = _scc_iframe_build_comp_connect(h_vid);
            if ( i4_ret != SMR_OK )
            {
                return SMR_INV_SET_INFO;
            }
        }
    }

    i4_ret = rm_set(
        pt_vid_comp->t_iframe_vd_comp_descr.h_comp,
        FALSE,
        ANY_PORT_NUM,
        VID_DEC_SET_TYPE_CTRL,
        (VOID*)(UINT32)e_drv_ctrl_mode,
        sizeof(VID_DEC_CTRL_T) ,
        &e_cond);
    if ( i4_ret != RMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

#ifdef SYS_MHP_SUPPORT
/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_mhp_iframe_ctrl
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          e_ctrl_mode         References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/

INT32 x_scc_vid_set_mhp_iframe_ctrl(
    HANDLE_T                      h_vid,
    SCC_VID_IFRAME_CTRL_T         e_ctrl_mode )
{
     INT32                        i4_ret;
     HANDLE_TYPE_T                e_type;
     SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
     VID_DEC_CTRL_T               e_drv_ctrl_mode = VID_DEC_CTRL_RESET;
     RM_COND_T                    e_cond = RM_COND_IGNORE;

     SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                        "\t\th_vid              = 0x%.8X\r\n"
                        "\t\te_ctrl_mode           = %d )\r\n",
                        __FUNCTION__,
                        (UINT32)h_vid,
                        (INT32)e_ctrl_mode ));

     i4_ret = handle_get_type_obj( h_vid,
                                   &e_type,
                                   (VOID**)&pt_vid_comp );
     if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
     {
         return SMR_INV_HANDLE;
     }

     switch( e_ctrl_mode )
     {
     case SCC_VID_IFRAME_CTRL_RESET:
         e_drv_ctrl_mode = VID_DEC_CTRL_RESET;
         break;
     case SCC_VID_IFRAME_CTRL_END:
         e_drv_ctrl_mode = VID_DEC_CTRL_STOP;
         break;
     case SCC_VID_IFRAME_CTRL_BEGIN:
         e_drv_ctrl_mode = VID_DEC_CTRL_PLAY_I_FRAME;
         break;
     default:
         return SMR_INV_SET_INFO;
     }

    if(e_drv_ctrl_mode == VID_DEC_CTRL_PLAY_I_FRAME)
    { /* build component graph first and then connect it */
        if(x_handle_valid(pt_vid_comp->t_iframe_vd_comp_descr.h_comp) == FALSE)
        {
            i4_ret = _scc_mhp_iframe_comp_open(h_vid);
            if ( i4_ret != SMR_OK )
            {
                return SMR_INV_SET_INFO;
            }
        }
    }

    i4_ret = rm_set(
        pt_vid_comp->t_iframe_vd_comp_descr.h_comp,
        FALSE,
        ANY_PORT_NUM,
        VID_DEC_SET_TYPE_CTRL,
        (VOID*)(UINT32)e_drv_ctrl_mode,
        sizeof(VID_DEC_CTRL_T) ,
        &e_cond);
    if ( i4_ret != RMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_iframe_resolution
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pui4_width    retrive the width of the iframe
 *          pui4_height  retrive the height of the iframe
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/

INT32 x_scc_vid_get_iframe_resolution(
    HANDLE_T                       h_vid,
    UINT32*                        pui4_width,
    UINT32*                        pui4_height)

{
    INT32                         i4_ret;
    HANDLE_TYPE_T                 e_type;
    SCC_VIDEO_COMP_T*             pt_vid_comp = NULL;
    VID_DEC_RESOLUTION_INFO_T     t_drv_resolution = {0};
    SIZE_T                        z_size;
    RM_COND_T                     e_cond = RM_COND_IGNORE;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tpui4_width             = %x )\r\n",
                       "\t\tpui4_width             = %x )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)pui4_width,
                       (INT32)pui4_height));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(VID_DEC_RESOLUTION_INFO_T);

    i4_ret = rm_get(
        pt_vid_comp->t_iframe_vd_comp_descr.h_comp,
        FALSE,
        ANY_PORT_NUM,
        VID_DEC_GET_TYPE_RESOLUTION,
        (VOID*)&t_drv_resolution,
        &z_size,
        &e_cond);
    if ( i4_ret != RMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    *pui4_width = t_drv_resolution.ui2_width;
    *pui4_height = t_drv_resolution.ui2_height;

    return SMR_OK;

}

/*-----------------------------------------------------------------------------
 * Name: _scc_swap_pla_z_order
 *
 * Description: <function description>
 *
 * Inputs:  pt_pmx           References.
 *          ui4_curr_adj_pla References.
 *          ui1_target_port  References.
 *          pui4_pla_order   References.
 *
 * Outputs: -
 *
 * Returns: SMR_INTERNAL_ERROR  Success.
 *          SMR_OK              Fail.
 *          SMR_NOT_ALLOW       Fail.
 *
 ----------------------------------------------------------------------------*/
static INT32 _scc_swap_pla_z_order(
    SM_DRV_COMP_DESCR_T*           pt_pmx,
    UINT32                         ui4_curr_adj_pla,
    UINT8                          ui1_target_port,
    UINT32*                        pui4_pla_order )
{
    INT32                             i4_ret;
    SIZE_T                            z_size;
    PLA_MXR_ADJ_PLA_BY_PORT_INFO_T    t_adj_pla_info = {0};

    /* get plane order adjustment */
    z_size = sizeof(PLA_MXR_ADJ_PLA_BY_PORT_INFO_T);
    t_adj_pla_info.ui1_port = ui1_target_port;
    i4_ret = rm_get( pt_pmx->h_comp,
                     FALSE,
                     ANY_PORT_NUM,
                     PLA_MXR_GET_TYPE_ADJ_PLA_BY_PORT,
                     (VOID*)(&t_adj_pla_info),
                     &z_size,
                     &pt_pmx->e_comp_cond );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INTERNAL_ERROR;
    }

    if ( pt_pmx->t_info.ui1_in_port >= MAX_NUM_PORTS ||
         ui1_target_port >= MAX_NUM_PORTS )
    {
        return SMR_INTERNAL_ERROR;
    }

    /* Check if target port could adjust to current z-order */
    if ( t_adj_pla_info.ui4_adj_zorder & MAKE_BIT_MASK_32(pui4_pla_order[pt_pmx->t_info.ui1_in_port]) )
    {
        UINT32  ui4_z_tmp = pui4_pla_order[pt_pmx->t_info.ui1_in_port];

        pui4_pla_order[pt_pmx->t_info.ui1_in_port] = pui4_pla_order[ui1_target_port];
        pui4_pla_order[ui1_target_port] = ui4_z_tmp;

        return SMR_OK;
    }

    return SMR_NOT_ALLOW;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_iframe_pla_order
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_iframe_pla_order(
    HANDLE_T                       h_vid,
    VSH_SET_PLANE_ORDER_INFO_T*    pt_order_info)
{
    INT32                          i4_ret;
    HANDLE_TYPE_T                  e_type;
    SCC_VIDEO_COMP_T*              pt_vid_comp = NULL;
    UINT32                         aui4_pla_order[MAX_NUM_PORTS];
    UINT32                         ui4_top, ui4_bottom, ui4_curr;
    UINT32                         ui4_num_pla, ui4_capa;
    UINT8                          ui1_i;
    SIZE_T                         z_size;
    PLA_MXR_ADJ_PLA_BY_ID_TYPE_INFO_T t_adj_pla_info = {0};

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\te_order_ctrl      = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       pt_order_info->e_order_ctrl));
    if ( !pt_order_info )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp || 
         pt_vid_comp->t_iframe_pmx_comp_descr.t_info.ui1_in_port >= MAX_NUM_PORTS )
    {
        return SMR_INV_HANDLE;
    }

    /* get capability */
    z_size = sizeof( UINT32 );
    i4_ret = rm_get( pt_vid_comp->t_iframe_pmx_comp_descr.h_comp,
                     FALSE,
                     ANY_PORT_NUM,
                     PLA_MXR_GET_TYPE_CAPABILITY,
                     (VOID*)(&ui4_capa),
                     &z_size,
                     &pt_vid_comp->t_iframe_pmx_comp_descr.e_comp_cond );
    if ( i4_ret != SMR_OK )
    {
        return SMR_DRV_COMP_ERROR;
    }
    if ( !(ui4_capa & PLA_MXR_CAP_ADJ_PLA) )
    {
        return SMR_NOT_ALLOW;
    }

    /* get current order */
    z_size = sizeof( UINT32 ) * MAX_NUM_PORTS;
    i4_ret = rm_get( pt_vid_comp->t_iframe_pmx_comp_descr.h_comp,
                     FALSE,
                     ANY_PORT_NUM,
                     PLA_MXR_GET_TYPE_PLA_ORDER,
                     (VOID*)(aui4_pla_order),
                     &z_size,
                     &pt_vid_comp->t_iframe_pmx_comp_descr.e_comp_cond );
    if ( i4_ret != SMR_OK )
    {
        return SMR_NOT_ALLOW;
    }
    ui4_num_pla = z_size / sizeof(UINT32);
    if ( ui4_num_pla >= MAX_NUM_PORTS )
    {
        return SMR_INTERNAL_ERROR;
    }

    /* get plane order adjustment */
    z_size = sizeof(PLA_MXR_ADJ_PLA_BY_ID_TYPE_INFO_T);
    t_adj_pla_info.e_plane_type = pt_vid_comp->t_iframe_vp_comp_descr.t_info.e_type;
    t_adj_pla_info.ui2_comp_id = pt_vid_comp->t_iframe_vp_comp_descr.t_info.ui2_comp_id;
    i4_ret = rm_get( pt_vid_comp->t_iframe_pmx_comp_descr.h_comp,
                     FALSE,
                     ANY_PORT_NUM,
                     PLA_MXR_GET_TYPE_ADJ_PLA_BY_ID_TYPE,
                     (VOID*)(&t_adj_pla_info),
                     &z_size,
                     &pt_vid_comp->t_iframe_pmx_comp_descr.e_comp_cond );
    if ( i4_ret != SMR_OK )
    {
        return SMR_NOT_ALLOW;
    }

    /* get top */
    for ( ui4_top = PLA_MXR_Z_ORDER_0; ui4_top <= PLA_MXR_Z_ORDER_31; ui4_top++ )
    {
        if ( t_adj_pla_info.ui4_adj_zorder & MAKE_BIT_MASK_32(ui4_top) )
        {
            break;
        }
    }

    /* get bottom */
    for ( ui4_bottom = PLA_MXR_Z_ORDER_31; ui4_bottom != PLA_MXR_Z_ORDER_0; ui4_bottom-- )
    {
        if ( t_adj_pla_info.ui4_adj_zorder & MAKE_BIT_MASK_32(ui4_bottom) )
        {
            break;
        }
    }

    ui4_curr = aui4_pla_order[pt_vid_comp->t_iframe_pmx_comp_descr.t_info.ui1_in_port];
    if ( ui4_curr < ui4_top || ui4_curr > ui4_bottom )
    {
        return SMR_NOT_ALLOW;
    }

    switch( pt_order_info->e_order_ctrl )
    {
    case VSH_PLANE_ORDER_CTRL_TOP:
        if ( ui4_top != ui4_curr )
        {
            for ( ui1_i = 0; ui1_i < (UINT8)ui4_num_pla; ++ui1_i )
            {
                /* Find the port that is not the current used port and is in
                   the top z-order */
                if ( ui1_i != pt_vid_comp->t_iframe_pmx_comp_descr.t_info.ui1_in_port &&
                     aui4_pla_order[ui1_i] == ui4_top )
                {
                    break;
                }
            }
            if ( (UINT32)ui1_i >= ui4_num_pla )
            {
                /* not found, just set current port to the top z-order */
                aui4_pla_order[pt_vid_comp->t_iframe_pmx_comp_descr.t_info.ui1_in_port] = ui4_top;
                break;
            }

            i4_ret = _scc_swap_pla_z_order(
                                       &pt_vid_comp->t_iframe_pmx_comp_descr,
                                       t_adj_pla_info.ui4_adj_zorder,
                                       (UINT8)ui1_i,
                                       aui4_pla_order );
            if ( i4_ret != SMR_OK )
            {
                return i4_ret;
            }
        }
        break;

    case VSH_PLANE_ORDER_CTRL_UP:
        {
            UINT8  ui1_z_idx = 0;
            UINT32 ui4_target_z = ui4_curr;

            /* find target z-order */
            /* Move up to next adjustable z-order */
            for ( ui1_z_idx = (UINT8)(ui4_curr-1); ui1_z_idx != 0; --ui1_z_idx )
            {
                if ( t_adj_pla_info.ui4_adj_zorder & MAKE_BIT_MASK_32(ui1_z_idx) )
                {
                    ui4_target_z = ui1_z_idx;
                    break;
                }
            }
            if ( ui4_target_z == ui4_curr )
            {
                return SMR_OK;
            }

            /* find target port number */
            for ( ui1_i = 0; ui1_i < (UINT8)ui4_num_pla; ++ui1_i )
            {
                if ( aui4_pla_order[ui1_i] == ui4_target_z )
                {
                    break;
                }
            }
            if ( (UINT32)ui1_i >= ui4_num_pla )
            {
                /* not found, just set current port to the target z-order */
                aui4_pla_order[pt_vid_comp->t_iframe_pmx_comp_descr.t_info.ui1_in_port] = ui4_target_z;
                break;
            }

            i4_ret = _scc_swap_pla_z_order(
                                       &pt_vid_comp->t_iframe_pmx_comp_descr,
                                       t_adj_pla_info.ui4_adj_zorder,
                                       (UINT8)ui1_i,
                                       aui4_pla_order );
            if ( i4_ret != SMR_OK )
            {
                return i4_ret;
            }
        }
        break;

    case VSH_PLANE_ORDER_CTRL_DOWN:
        {
            UINT32 ui4_target_z = ui4_curr;

            /* find target z-order */
            /* Move down to next adjustable z-order */
            for ( ui1_i = (UINT8)(ui4_curr+1); ui1_i < (UINT8)ui4_num_pla; ++ui1_i )
            {
                if ( t_adj_pla_info.ui4_adj_zorder & MAKE_BIT_MASK_32(ui1_i) )
                {
                    ui4_target_z = ui1_i;
                    break;
                }
            }
            if ( ui4_target_z == ui4_curr )
            {
                return SMR_OK;
            }

            /* find target port number */
            for ( ui1_i = 0; ui1_i < (UINT8)ui4_num_pla; ++ui1_i )
            {
                if ( aui4_pla_order[ui1_i] == ui4_target_z )
                {
                    break;
                }
            }
            if ( (UINT32)ui1_i >= ui4_num_pla )
            {
                /* not found, just set current port to the target z-order */
                aui4_pla_order[pt_vid_comp->t_iframe_pmx_comp_descr.t_info.ui1_in_port] = ui4_target_z;
                break;
            }

            i4_ret = _scc_swap_pla_z_order(
                                       &pt_vid_comp->t_iframe_pmx_comp_descr,
                                       t_adj_pla_info.ui4_adj_zorder,
                                       (UINT8)ui1_i,
                                       aui4_pla_order );
            if ( i4_ret != SMR_OK )
            {
                return i4_ret;
            }
        }
        break;

    case VSH_PLANE_ORDER_CTRL_SWAP:
        {
            for ( ui1_i = 0; ui1_i < (UINT8)ui4_num_pla; ++ui1_i )
            {
                if ( aui4_pla_order[ui1_i] == pt_order_info->u.ui1_layer )
                {
                   break;
                }
            }
            if ( (UINT32)ui1_i >= ui4_num_pla )
            {
                /* not found, just set current port to the target z-order */
                aui4_pla_order[pt_vid_comp->t_iframe_pmx_comp_descr.t_info.ui1_in_port] = pt_order_info->u.ui1_layer;
                break;
            }

            i4_ret = _scc_swap_pla_z_order(
                                       &pt_vid_comp->t_iframe_pmx_comp_descr,
                                       t_adj_pla_info.ui4_adj_zorder,
                                       (UINT8)ui1_i,
                                       aui4_pla_order );
            if ( i4_ret != SMR_OK )
            {
                return i4_ret;
            }
        }
        break;

    case VSH_PLANE_ORDER_CTRL_BOTTOM:
        if ( ui4_bottom != ui4_curr )
        {
            for ( ui1_i = 0; ui1_i < (UINT8)ui4_num_pla; ++ui1_i )
            {
                /* Find the port that is not the current used port and is in
                   the bottom z-order */
                if ( ui1_i != pt_vid_comp->t_iframe_pmx_comp_descr.t_info.ui1_in_port &&
                     aui4_pla_order[ui1_i] == ui4_bottom )
                {
                    break;
                }
            }
            if ( (UINT32)ui1_i >= ui4_num_pla )
            {
                /* not found, just set current port to the top z-order */
                aui4_pla_order[pt_vid_comp->t_iframe_pmx_comp_descr.t_info.ui1_in_port] = ui4_bottom;
                break;
            }

            i4_ret = _scc_swap_pla_z_order(
                                       &pt_vid_comp->t_iframe_pmx_comp_descr,
                                       t_adj_pla_info.ui4_adj_zorder,
                                       (UINT8)ui1_i,
                                       aui4_pla_order );
            if ( i4_ret != SMR_OK )
            {
                return i4_ret;
            }
        }
        break;

    default:
        return SMR_NOT_SUPPORT;
    }

    z_size = sizeof(UINT32) * pt_vid_comp->t_iframe_pmx_comp_descr.t_info.ui1_num_in_port;
    i4_ret = rm_set( pt_vid_comp->t_iframe_pmx_comp_descr.h_comp,
                  FALSE,
                  ANY_PORT_NUM,
                  PLA_MXR_SET_TYPE_PLA_ORDER,
                  (VOID*)(aui4_pla_order),
                  z_size,
                  &pt_vid_comp->t_iframe_pmx_comp_descr.e_comp_cond );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INTERNAL_ERROR;
    }

    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_iframe_src_region
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/

INT32 x_scc_vid_set_iframe_src_region(
    HANDLE_T                       h_vid,
    VSH_REGION_INFO_T*             pt_region_inf)
{
    INT32                         i4_ret;
    HANDLE_TYPE_T                 e_type;
    SCC_VIDEO_COMP_T*             pt_vid_comp = NULL;
    VID_PLA_VID_REGION_T          t_drv_region = {0};
    RM_COND_T                     e_cond = RM_COND_IGNORE;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tpt_region_inf             = %x )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)pt_region_inf));
    if ((pt_region_inf->ui4_x > VSH_REGION_MAX_WIDTH)
        || (pt_region_inf->ui4_y > VSH_REGION_MAX_HEIGHT)
        || (pt_region_inf->ui4_height > VSH_REGION_MAX_HEIGHT)
        || (pt_region_inf->ui4_width > VSH_REGION_MAX_WIDTH))
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_region.ui4_x = pt_region_inf->ui4_x;
    t_drv_region.ui4_y = pt_region_inf->ui4_y;
    t_drv_region.ui4_height = pt_region_inf->ui4_height;
    t_drv_region.ui4_width = pt_region_inf->ui4_width;

    i4_ret = rm_set(
        pt_vid_comp->t_iframe_vp_comp_descr.h_comp,
        FALSE,
        ANY_PORT_NUM,
        VID_PLA_SET_TYPE_SRC_REGION,
        (VOID*)&t_drv_region,
        sizeof(VID_PLA_VID_REGION_T) ,
        &e_cond);
    if ( i4_ret != RMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;

}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_iframe_disp_region
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/

INT32 x_scc_vid_set_iframe_disp_region(
    HANDLE_T                       h_vid,
    VSH_REGION_INFO_T*             pt_region_inf)
{
    INT32                         i4_ret;
    HANDLE_TYPE_T                 e_type;
    SCC_VIDEO_COMP_T*             pt_vid_comp = NULL;
    VID_PLA_VID_REGION_T          t_drv_region = {0};
    RM_COND_T                     e_cond;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tpt_region_inf     = %x )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)pt_region_inf));
    if ((pt_region_inf->ui4_x > VSH_REGION_MAX_WIDTH)
        || (pt_region_inf->ui4_y > VSH_REGION_MAX_HEIGHT)
        || (pt_region_inf->ui4_height > VSH_REGION_MAX_HEIGHT)
        || (pt_region_inf->ui4_width > VSH_REGION_MAX_WIDTH))
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_region.ui4_x = pt_region_inf->ui4_x;
    t_drv_region.ui4_y = pt_region_inf->ui4_y;
    t_drv_region.ui4_height = pt_region_inf->ui4_height;
    t_drv_region.ui4_width = pt_region_inf->ui4_width;

    i4_ret = rm_set(
        pt_vid_comp->t_iframe_vp_comp_descr.h_comp,
        FALSE,
        ANY_PORT_NUM,
        VID_PLA_SET_TYPE_DISP_REGION,
        (VOID*)&t_drv_region,
        sizeof(VID_PLA_VID_REGION_T) ,
        &e_cond);
    if ( i4_ret != RMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;

}
/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_iframe_src_region
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/

INT32 x_scc_vid_get_iframe_src_region(
    HANDLE_T                       h_vid,
    VSH_REGION_INFO_T*             pt_region_inf)
{
    INT32                         i4_ret;
    HANDLE_TYPE_T                 e_type;
    SCC_VIDEO_COMP_T*             pt_vid_comp = NULL;
    VID_PLA_VID_REGION_T          t_drv_region = {0};
    RM_COND_T                     e_cond;
    SIZE_T                        z_size;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tpt_region_inf             = %x )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)pt_region_inf));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(VID_PLA_VID_REGION_T);

    i4_ret = rm_get(
        pt_vid_comp->t_iframe_vp_comp_descr.h_comp,
        FALSE,
        ANY_PORT_NUM,
        VID_PLA_GET_TYPE_SRC_REGION,
        (VOID*)&t_drv_region,
        &z_size ,
        &e_cond);
    if ( i4_ret != RMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    pt_region_inf->ui4_height = t_drv_region.ui4_height;
    pt_region_inf->ui4_width = t_drv_region.ui4_width;
    pt_region_inf->ui4_x = t_drv_region.ui4_x;
    pt_region_inf->ui4_y = t_drv_region.ui4_y;

    return SMR_OK;

}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_iframe_disp_region
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/

INT32 x_scc_vid_get_iframe_disp_region(
    HANDLE_T                       h_vid,
    VSH_REGION_INFO_T*             pt_region_inf)
{
    INT32                         i4_ret;
    HANDLE_TYPE_T                 e_type;
    SCC_VIDEO_COMP_T*             pt_vid_comp = NULL;
    VID_PLA_VID_REGION_T          t_drv_region = {0};
    RM_COND_T                     e_cond;
    SIZE_T                        z_size;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tpt_region_inf             = %x )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)pt_region_inf));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = rm_get(
        pt_vid_comp->t_iframe_vp_comp_descr.h_comp,
        FALSE,
        ANY_PORT_NUM,
        VID_PLA_GET_TYPE_DISP_REGION,
        (VOID*)&t_drv_region,
        &z_size ,
        &e_cond);
    if ( i4_ret != RMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    pt_region_inf->ui4_height = t_drv_region.ui4_height;
    pt_region_inf->ui4_width = t_drv_region.ui4_width;
    pt_region_inf->ui4_x = t_drv_region.ui4_x;
    pt_region_inf->ui4_y = t_drv_region.ui4_y;

    return SMR_OK;

}
/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_iframe_surface
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/

INT32 x_scc_vid_get_iframe_surface(
    HANDLE_T                     h_vid,
    HANDLE_T                     h_surface)
{
#if 0
    INT32                            i4_ret;
    HANDLE_TYPE_T                    e_type;
    SIZE_T                          z_size;
    SCC_VIDEO_COMP_T*                pt_vid_comp;
    RM_COND_T                       e_cond;
    VID_DEC_GET_I_FRAME_SURFACE_T   t_drv_surf;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\th_surface             = %x )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)h_surface));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }
    z_size = sizeof(VID_DEC_GET_I_FRAME_SURFACE_T);

    i4_ret = rm_get(
        pt_vid_comp->t_iframe_vd_comp_descr.h_comp,
        FALSE,
        ANY_PORT_NUM,
        VID_DEC_GET_TYPE_I_FRAME_SURFACE,
        (VOID*)&t_drv_surf,
        &z_size,
        &e_cond);
    if ( i4_ret != RMR_OK )
    {
        return SMR_INV_SET_INFO;
    }
#endif
    return SMR_OK;

}

#ifdef SYS_MHP_SUPPORT
/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_iframe_not_display
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_iframe_not_display(
    HANDLE_T                      h_vid, 
    BOOL                          b_not_display )
{
    INT32                         i4_ret;
    HANDLE_TYPE_T                 e_type;
    SCC_VIDEO_COMP_T*             pt_vid_comp = NULL;
    RM_COND_T                     e_cond;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid              = 0x%.8X\r\n"
                       "\t\tb_not_display      = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)b_not_display ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    if(x_handle_valid(pt_vid_comp->t_iframe_vd_comp_descr.h_comp) == FALSE)
    {
        i4_ret = _scc_mhp_iframe_comp_open(h_vid);
        if ( i4_ret != SMR_OK )
        {
            return SMR_INV_SET_INFO;
        }
    }

    i4_ret = rm_set(
        pt_vid_comp->t_iframe_vd_comp_descr.h_comp,
        FALSE,
        ANY_PORT_NUM,
        VID_DEC_SET_TYPE_NOT_DISPLAY,
        (VOID*)(UINT32)b_not_display,
        sizeof(BOOL),
        &e_cond);
    if ( i4_ret != RMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;  
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_thumbnail
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_thumbnail(
    HANDLE_T                      h_vid, 
    VSH_THUMBNAIL_INFO_T*         pt_thumbnail_info )
{
    INT32                         i4_ret;
    SIZE_T                        z_size;
    HANDLE_TYPE_T                 e_type;
    SCC_VIDEO_COMP_T*             pt_vid_comp = NULL;
    RM_COND_T                     e_cond;
    VID_DEC_THUMBNAIL_INFO_T      t_drv_thumbnail_info = {0};

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid              = 0x%.8X\r\n"
                       "\t\tpt_thumbnail_info  = %x )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)pt_thumbnail_info ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }


    switch ( pt_thumbnail_info->e_canvas_colormode )
    {
    case VSH_COLORMODE_AYUV_CLUT2:
        t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_AYUV_CLUT2;
        break;
    case VSH_COLORMODE_AYUV_CLUT4:
        t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_AYUV_CLUT4;
        break;
    case VSH_COLORMODE_AYUV_CLUT8:
        t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_AYUV_CLUT8;
        break;
    case VSH_COLORMODE_UYVY_16:
        t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_UYVY_16;
        break;
    case VSH_COLORMODE_YUYV_16:
        t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_YUYV_16;
        break;
    case VSH_COLORMODE_AYUV_D8888:
        t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_AYUV_D8888;
        break;
    case VSH_COLORMODE_ARGB_CLUT2:
        t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_ARGB_CLUT2;
        break;
    case VSH_COLORMODE_ARGB_CLUT4:
        t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_ARGB_CLUT4;
        break;
    case VSH_COLORMODE_ARGB_CLUT8:
        t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_ARGB_CLUT8;
        break;
    case VSH_COLORMODE_RGB_D565:
        t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_RGB_D565;
        break;
    case VSH_COLORMODE_ARGB_D1555:
        t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_ARGB_D1555;
        break;
    case VSH_COLORMODE_ARGB_D4444:
        t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_ARGB_D4444;
        break;
    case VSH_COLORMODE_ARGB_D8888:
        t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_ARGB_D8888;
        break;
    case VSH_COLORMODE_YUV_420_BLK:
        t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_YUV_420_BLK;
        break;
    case VSH_COLORMODE_YUV_420_RS:
        t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_YUV_420_RS;
        break;
    case VSH_COLORMODE_YUV_422_BLK:
        t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_YUV_422_BLK;
        break;
    case VSH_COLORMODE_YUV_422_RS:
        t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_YUV_422_RS;
        break;
    case VSH_COLORMODE_YUV_444_BLK:
        t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_YUV_444_BLK;
        break;
    case VSH_COLORMODE_YUV_444_RS:
        t_drv_thumbnail_info.e_canvas_colormode = VID_DEC_COLORMODE_YUV_444_RS;
        break;
    default:
        return SMR_INV_GET_INFO;
    }
    t_drv_thumbnail_info.ui4_canvas_width = pt_thumbnail_info->ui4_canvas_width;      
    t_drv_thumbnail_info.ui4_canvas_height = pt_thumbnail_info->ui4_canvas_height;
    t_drv_thumbnail_info.ui4_canvas_pitch = pt_thumbnail_info->ui4_canvas_pitch;
    t_drv_thumbnail_info.ui4_thumbnail_x = pt_thumbnail_info->ui4_thumbnail_x;
    t_drv_thumbnail_info.ui4_thumbnail_y = pt_thumbnail_info->ui4_thumbnail_y;
    t_drv_thumbnail_info.ui4_thumbnail_width = pt_thumbnail_info->ui4_thumbnail_width;
    t_drv_thumbnail_info.ui4_thumbnail_height = pt_thumbnail_info->ui4_thumbnail_height;
    t_drv_thumbnail_info.pc_canvas_buffer = pt_thumbnail_info->pc_canvas_buffer;

    z_size = sizeof(VID_DEC_THUMBNAIL_INFO_T);

    i4_ret = rm_get(
        pt_vid_comp->t_iframe_vd_comp_descr.h_comp,
        FALSE,
        ANY_PORT_NUM,
        VID_DEC_GET_TYPE_THUMBNAIL,
        (VOID*)&t_drv_thumbnail_info,
        &z_size,
        &e_cond);
    if ( i4_ret != RMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;  
}
#endif

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_vdrips_nfy
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pui2_effect_time    References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_vdrips_nfy(
    HANDLE_T                     h_vid,
    x_scc_vid_vdrips_nfy_fct     pf_scc_vdrips_nfy,
    VOID*                        pv_nfy_tag )
{
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_vdrips_feed
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pui2_effect_time    References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_vdrips_feed(
    HANDLE_T                     h_vid,
    SCC_VID_VDRIPS_BUFF_T*       pt_vdrips_buf)
{
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_vdrips_play_ctrl
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pui2_effect_time    References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_vdrips_play_ctrl(
    HANDLE_T                      h_vid,
    SCC_VID_VDRIPS_PLAY_CTRL_T    e_ctrl_mode )
{
    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_vdrips_duration
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pui2_effect_time    References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_vdrips_duration(
    HANDLE_T                       h_vid,
    UINT32                         ui4_duration )
{
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_di_film_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          e_film_mode         References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_di_film_mode( HANDLE_T               h_vid,
                                  SCC_VID_DI_FILM_MODE_T e_film_mode )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_DI_FILM_MODE_T       e_vdp_di_film_mode = VID_PLA_DI_FILM_MODE_OFF;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\te_film_mode      = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)e_film_mode ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_film_mode )
    {
    case SCC_VID_DI_FILM_MODE_OFF:
        e_vdp_di_film_mode = VID_PLA_DI_FILM_MODE_OFF;
        break;
    case SCC_VID_DI_FILM_MODE_SLOW_PICTURE:
        e_vdp_di_film_mode = VID_PLA_DI_FILM_MODE_SLOW_PICTURE;
        break;
    case SCC_VID_DI_FILM_MODE_ACTION_PICTURE:
        e_vdp_di_film_mode = VID_PLA_DI_FILM_MODE_ACTION_PICTURE;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_DI_FILM_MODE,
        (VOID*)(UINT32)e_vdp_di_film_mode,
        sizeof(VID_PLA_DI_FILM_MODE_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_di_film_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pe_film_mode        References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_di_film_mode( HANDLE_T                h_vid,
                                  SCC_VID_DI_FILM_MODE_T* pe_film_mode )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_DI_FILM_MODE_T       e_vdp_film_mode = VID_PLA_DI_FILM_MODE_OFF;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpe_film_mode     = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pe_film_mode ));

    if ( !pe_film_mode )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }


    z_size = sizeof(VID_PLA_DI_FILM_MODE_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_DI_FILM_MODE,
        (VOID*)&e_vdp_film_mode,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch( e_vdp_film_mode )
    {
    case VID_PLA_DI_FILM_MODE_OFF:
        *pe_film_mode = SCC_VID_DI_FILM_MODE_OFF;
        break;
    case VID_PLA_DI_FILM_MODE_SLOW_PICTURE:
        *pe_film_mode = SCC_VID_DI_FILM_MODE_SLOW_PICTURE;
        break;
    case VID_PLA_DI_FILM_MODE_ACTION_PICTURE:
        *pe_film_mode = SCC_VID_DI_FILM_MODE_ACTION_PICTURE;
        break;
    default:
        return SMR_DRV_COMP_ERROR;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_di_ma
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          e_di_ma             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_di_ma( HANDLE_T               h_vid,
                           SCC_VID_DI_MA_T        e_di_ma )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_DI_MA_T              e_vdp_di_ma = VID_PLA_DI_MA_SLOW_PICTURE;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\te_di_ma          = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)e_di_ma ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_di_ma )
    {
    case SCC_VID_DI_MA_SLOW_PICTURE:
        e_vdp_di_ma = VID_PLA_DI_MA_SLOW_PICTURE;
        break;
    case SCC_VID_DI_MA_ACTION_PICTURE:
        e_vdp_di_ma = VID_PLA_DI_MA_ACTION_PICTURE;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_DI_MA,
        (VOID*)(UINT32)e_vdp_di_ma,
        sizeof(VID_PLA_DI_MA_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_di_ma
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pe_di_ma            References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_di_ma( HANDLE_T                h_vid,
                           SCC_VID_DI_MA_T*        pe_di_ma )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_DI_MA_T              e_vdp_di_ma = VID_PLA_DI_MA_SLOW_PICTURE;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpe_di_ma         = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pe_di_ma ));

    if ( !pe_di_ma )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }


    z_size = sizeof(VID_PLA_DI_MA_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_DI_MA,
        (VOID*)&e_vdp_di_ma,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch( e_vdp_di_ma )
    {
    case VID_PLA_DI_MA_SLOW_PICTURE:
        *pe_di_ma = SCC_VID_DI_MA_SLOW_PICTURE;
        break;
    case VID_PLA_DI_MA_ACTION_PICTURE:
        *pe_di_ma = SCC_VID_DI_MA_ACTION_PICTURE;
        break;
    default:
        return SMR_DRV_COMP_ERROR;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_di_edge_preserve
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          e_edge              References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_di_edge_preserve( HANDLE_T                      h_vid,
                                      SCC_VID_DI_EDGE_PRESERVE_T    e_edge )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_DI_EDGE_PRESERVE_T   e_vdp_di_edge_preserve = VID_PLA_DI_EDGE_PRESERVE_WEAK;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\te_edge           = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)e_edge ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_edge )
    {
    case SCC_VID_DI_EDGE_PRESERVE_WEAK:
        e_vdp_di_edge_preserve = VID_PLA_DI_EDGE_PRESERVE_WEAK;
        break;
    case SCC_VID_DI_EDGE_PRESERVE_STRONG:
        e_vdp_di_edge_preserve = VID_PLA_DI_EDGE_PRESERVE_STRONG;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_DI_EDGE_PRESERVE,
        (VOID*)(UINT32)e_vdp_di_edge_preserve,
        sizeof(VID_PLA_DI_EDGE_PRESERVE_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_di_edge_preserve
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pe_edge             References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_di_edge_preserve( HANDLE_T                       h_vid,
                                      SCC_VID_DI_EDGE_PRESERVE_T*    pe_edge )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_DI_EDGE_PRESERVE_T   e_vdp_di_edge_preserve = VID_PLA_DI_EDGE_PRESERVE_WEAK;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpe_edge          = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pe_edge ));

    if ( !pe_edge )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }


    z_size = sizeof(VID_PLA_DI_EDGE_PRESERVE_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_DI_EDGE_PRESERVE,
        (VOID*)&e_vdp_di_edge_preserve,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch( e_vdp_di_edge_preserve )
    {
    case VID_PLA_DI_EDGE_PRESERVE_WEAK:
        *pe_edge = SCC_VID_DI_EDGE_PRESERVE_WEAK;
        break;
    case VID_PLA_DI_EDGE_PRESERVE_STRONG:
        *pe_edge = SCC_VID_DI_EDGE_PRESERVE_STRONG;
        break;
    default:
        return SMR_DRV_COMP_ERROR;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_mjc
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          pt_mjc              References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_mjc( HANDLE_T                     h_vid,
                         const SCC_VID_MJC_T*         pt_mjc )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_MJC_EFFECT_T         e_vdp_mjc_effect = VID_PLA_MJC_EFFECT_OFF;
    VID_PLA_MJC_MODE_T           e_vdp_mjc_mode = VID_PLA_MJC_MODE_0;
    VID_PLA_MJC_DEMO_T           e_vdp_mjc_demo = VID_PLA_MJC_DEMO_OFF;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tpt_mjc           = 0x%.8X<effect(%d),mode(%d),demo(%d)> )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pt_mjc,
                       (INT32)((pt_mjc)?pt_mjc->e_effect:(-1)),
                       (INT32)((pt_mjc)?pt_mjc->e_mode:(-1)),
                       (INT32)((pt_mjc)?pt_mjc->e_demo:(-1)) ));

    if ( !pt_mjc )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( pt_mjc->e_effect )
    {
    case SCC_VID_MJC_EFFECT_OFF:
        e_vdp_mjc_effect = VID_PLA_MJC_EFFECT_OFF;
        break;
    case SCC_VID_MJC_EFFECT_LOW:
        e_vdp_mjc_effect = VID_PLA_MJC_EFFECT_LOW;
        break;
    case SCC_VID_MJC_EFFECT_MIDDLE:
        e_vdp_mjc_effect = VID_PLA_MJC_EFFECT_MIDDLE;
        break;
    case SCC_VID_MJC_EFFECT_HIGH:
        e_vdp_mjc_effect = VID_PLA_MJC_EFFECT_HIGH;
        break;
    default:
        return SMR_INV_SET_INFO;
    }
    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_MJC_EFFECT,
        (VOID*)(UINT32)e_vdp_mjc_effect,
        sizeof(VID_PLA_MJC_EFFECT_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch( pt_mjc->e_mode )
    {
    case SCC_VID_MJC_MODE_0:
        e_vdp_mjc_mode = VID_PLA_MJC_MODE_0;
        break;
    case SCC_VID_MJC_MODE_1:
        e_vdp_mjc_mode = VID_PLA_MJC_MODE_1;
        break;
    case SCC_VID_MJC_MODE_2:
        e_vdp_mjc_mode = VID_PLA_MJC_MODE_2;
        break;
    default:
        return SMR_INV_SET_INFO;
    }
    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_MJC_MODE,
        (VOID*)(UINT32)e_vdp_mjc_mode,
        sizeof(VID_PLA_MJC_MODE_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }


    switch( pt_mjc->e_demo )
    {
    case SCC_VID_MJC_DEMO_OFF:
        e_vdp_mjc_demo = VID_PLA_MJC_DEMO_OFF;
        break;
    case SCC_VID_MJC_DEMO_RIGHT:
        e_vdp_mjc_demo = VID_PLA_MJC_DEMO_RIGHT;
        break;
    case SCC_VID_MJC_DEMO_LEFT:
        e_vdp_mjc_demo = VID_PLA_MJC_DEMO_LEFT;
        break;
    default:
        return SMR_INV_SET_INFO;
    }
    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_MJC_DEMO,
        (VOID*)(UINT32)e_vdp_mjc_demo,
        sizeof(VID_PLA_MJC_DEMO_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_mjc
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pt_mjc              References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_mjc( HANDLE_T                     h_vid,
                         SCC_VID_MJC_T*               pt_mjc )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_MJC_EFFECT_T         e_vdp_mjc_effect = VID_PLA_MJC_EFFECT_OFF;
    VID_PLA_MJC_MODE_T           e_vdp_mjc_mode = VID_PLA_MJC_MODE_0;
    VID_PLA_MJC_DEMO_T           e_vdp_mjc_demo = VID_PLA_MJC_DEMO_OFF;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tpt_mjc           = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pt_mjc ));

    if ( !pt_mjc )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(VID_PLA_MJC_EFFECT_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MJC_EFFECT,
        (VOID*)&e_vdp_mjc_effect,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }
    switch( e_vdp_mjc_effect )
    {
    case VID_PLA_MJC_EFFECT_OFF:
        pt_mjc->e_effect = SCC_VID_MJC_EFFECT_OFF;
        break;
    case VID_PLA_MJC_EFFECT_LOW:
        pt_mjc->e_effect = SCC_VID_MJC_EFFECT_LOW;
        break;
    case VID_PLA_MJC_EFFECT_MIDDLE:
        pt_mjc->e_effect = SCC_VID_MJC_EFFECT_MIDDLE;
        break;
    case VID_PLA_MJC_EFFECT_HIGH:
        pt_mjc->e_effect = SCC_VID_MJC_EFFECT_HIGH;
        break;
    default:
        return SMR_DRV_COMP_ERROR;
    }

    z_size = sizeof(VID_PLA_MJC_MODE_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MJC_MODE,
        (VOID*)&e_vdp_mjc_mode,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }
    switch( e_vdp_mjc_mode )
    {
    case VID_PLA_MJC_MODE_0:
        pt_mjc->e_mode = SCC_VID_MJC_MODE_0;
        break;
    case VID_PLA_MJC_MODE_1:
        pt_mjc->e_mode = SCC_VID_MJC_MODE_1;
        break;
    default:
        return SMR_DRV_COMP_ERROR;
    }

    z_size = sizeof(VID_PLA_MJC_DEMO_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MJC_DEMO,
        (VOID*)&e_vdp_mjc_demo,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }
    switch( e_vdp_mjc_demo )
    {
    case VID_PLA_MJC_DEMO_OFF:
        pt_mjc->e_demo = SCC_VID_MJC_DEMO_OFF;
        break;
    case VID_PLA_MJC_DEMO_RIGHT:
        pt_mjc->e_demo = SCC_VID_MJC_DEMO_RIGHT;
        break;
    case VID_PLA_MJC_DEMO_LEFT:
        pt_mjc->e_demo = SCC_VID_MJC_DEMO_LEFT;
        break;
    default:
        return SMR_DRV_COMP_ERROR;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_xvycc
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          e_xvycc             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_xvycc( HANDLE_T               h_vid,
                           SCC_VID_XVYCC_T        e_xvycc )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_XVYCC_T              e_vdp_xvycc = VID_PLA_XVYCC_OFF;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\te_xvycc          = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)e_xvycc ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_xvycc )
    {
    case SCC_VID_XVYCC_OFF:
        e_vdp_xvycc = VID_PLA_XVYCC_OFF;
        break;
    case SCC_VID_XVYCC_ON:
        e_vdp_xvycc = VID_PLA_XVYCC_ON;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_XVYCC,
        (VOID*)(UINT32)e_vdp_xvycc,
        sizeof(VID_PLA_XVYCC_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_xvycc
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pe_xvycc            References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_xvycc( HANDLE_T                h_vid,
                           SCC_VID_XVYCC_T*        pe_xvycc )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_XVYCC_T              e_vdp_xvycc = VID_PLA_XVYCC_OFF;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpe_xvycc         = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pe_xvycc ));

    if ( !pe_xvycc )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }


    z_size = sizeof(VID_PLA_XVYCC_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_XVYCC,
        (VOID*)&e_vdp_xvycc,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch( e_vdp_xvycc )
    {
    case VID_PLA_XVYCC_OFF:
        *pe_xvycc = SCC_VID_XVYCC_OFF;
        break;
    case VID_PLA_XVYCC_ON:
        *pe_xvycc = SCC_VID_XVYCC_ON;
        break;
    default:
        return SMR_DRV_COMP_ERROR;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_wcg
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          e_wcg               References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_wcg( HANDLE_T               h_vid,
                         SCC_VID_WCG_T          e_wcg )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_WCG_T                e_vdp_wcg = VID_PLA_WCG_OFF;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\te_wcg            = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)e_wcg ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_wcg )
    {
    case SCC_VID_WCG_OFF:
        e_vdp_wcg = VID_PLA_WCG_OFF;
        break;
    case SCC_VID_WCG_ON:
        e_vdp_wcg = VID_PLA_WCG_ON;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_WCG,
        (VOID*)(UINT32)e_vdp_wcg,
        sizeof(VID_PLA_WCG_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_wcg
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pe_wcg              References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_wcg( HANDLE_T                h_vid,
                         SCC_VID_WCG_T*          pe_wcg )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_WCG_T                e_vdp_wcg = VID_PLA_WCG_OFF;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpe_wcg           = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pe_wcg ));

    if ( !pe_wcg )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }


    z_size = sizeof(VID_PLA_WCG_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_WCG,
        (VOID*)&e_vdp_wcg,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch( e_vdp_wcg )
    {
    case VID_PLA_WCG_OFF:
        *pe_wcg = SCC_VID_WCG_OFF;
        break;
    case VID_PLA_WCG_ON:
        *pe_wcg = SCC_VID_WCG_ON;
        break;
    default:
        return SMR_DRV_COMP_ERROR;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_game_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          e_game              References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_game_mode( HANDLE_T               h_vid,
                               SCC_VID_GAME_MODE_T    e_game )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_GAME_MODE_T          e_vdp_game = VID_PLA_GAME_MODE_OFF;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\te_game           = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)e_game ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_game )
    {
    case SCC_VID_GAME_MODE_OFF:
        e_vdp_game = VID_PLA_GAME_MODE_OFF;
        break;
    case SCC_VID_GAME_MODE_ON:
        e_vdp_game = VID_PLA_GAME_MODE_ON;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_GAME_MODE,
        (VOID*)(UINT32)e_vdp_game,
        sizeof(VID_PLA_GAME_MODE_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_game_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pe_game             References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_game_mode( HANDLE_T                h_vid,
                               SCC_VID_GAME_MODE_T*    pe_game )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_GAME_MODE_T          e_vdp_game = VID_PLA_GAME_MODE_OFF;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpe_game          = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pe_game ));

    if ( !pe_game )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }


    z_size = sizeof(VID_PLA_GAME_MODE_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_GAME_MODE,
        (VOID*)&e_vdp_game,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch( e_vdp_game )
    {
    case VID_PLA_GAME_MODE_OFF:
        *pe_game = SCC_VID_GAME_MODE_OFF;
        break;
    case VID_PLA_GAME_MODE_ON:
        *pe_game = SCC_VID_GAME_MODE_ON;
        break;
    default:
        return SMR_DRV_COMP_ERROR;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_blue_stretch
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          e_stretch           References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_blue_stretch( HANDLE_T                  h_vid,
                                  SCC_VID_BLUE_STRETCH_T    e_stretch )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_BLUE_STRETCH_T       e_vdp_stretch = VID_PLA_BLUE_STRETCH_OFF;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\te_stretch        = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)e_stretch ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_stretch )
    {
    case SCC_VID_BLUE_STRETCH_OFF:
        e_vdp_stretch = VID_PLA_BLUE_STRETCH_OFF;
        break;
    case SCC_VID_BLUE_STRETCH_RED:
        e_vdp_stretch = VID_PLA_BLUE_STRETCH_RED;
        break;
    case SCC_VID_BLUE_STRETCH_GREEN:
        e_vdp_stretch = VID_PLA_BLUE_STRETCH_GREEN;
        break;
    case SCC_VID_BLUE_STRETCH_BLUE:
        e_vdp_stretch = VID_PLA_BLUE_STRETCH_BLUE;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_BLUE_STRETCH,
        (VOID*)(UINT32)e_vdp_stretch,
        sizeof(VID_PLA_BLUE_STRETCH_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_blue_stretch
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pe_stretch          References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_blue_stretch( HANDLE_T                   h_vid,
                                  SCC_VID_BLUE_STRETCH_T*    pe_stretch )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_BLUE_STRETCH_T       e_vdp_stretch = VID_PLA_BLUE_STRETCH_OFF;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpe_stretch       = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pe_stretch ));

    if ( !pe_stretch )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }


    z_size = sizeof(VID_PLA_BLUE_STRETCH_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_BLUE_STRETCH,
        (VOID*)&e_vdp_stretch,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch( e_vdp_stretch )
    {
    case VID_PLA_BLUE_STRETCH_OFF:
        *pe_stretch = SCC_VID_BLUE_STRETCH_OFF;
        break;
    case VID_PLA_BLUE_STRETCH_RED:
        *pe_stretch = SCC_VID_BLUE_STRETCH_RED;
        break;
    case VID_PLA_BLUE_STRETCH_GREEN:
        *pe_stretch = SCC_VID_BLUE_STRETCH_GREEN;
        break;
    case VID_PLA_BLUE_STRETCH_BLUE:
        *pe_stretch = SCC_VID_BLUE_STRETCH_BLUE;
        break;
    default:
        return SMR_DRV_COMP_ERROR;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_mpeg_nr
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          e_nr                References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_mpeg_nr( HANDLE_T             h_vid,
                             SCC_VID_MPEG_NR_T    e_nr )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_MPEG_NR_T            e_vdp_mpeg_nr = VID_PLA_MPEG_NR_OFF;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\te_nr             = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)e_nr ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_nr )
    {
    case SCC_VID_MPEG_NR_OFF:
        e_vdp_mpeg_nr = VID_PLA_MPEG_NR_OFF;
        break;
    case SCC_VID_MPEG_NR_LOW:
        e_vdp_mpeg_nr = VID_PLA_MPEG_NR_LOW;
        break;
    case SCC_VID_MPEG_NR_MIDDLE:
        e_vdp_mpeg_nr = VID_PLA_MPEG_NR_MIDDLE;
        break;
    case SCC_VID_MPEG_NR_HIGH:
        e_vdp_mpeg_nr = VID_PLA_MPEG_NR_HIGH;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_MPEG_NR,
        (VOID*)(UINT32)e_vdp_mpeg_nr,
        sizeof(VID_PLA_MPEG_NR_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_mpeg_nr
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pe_nr               References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_mpeg_nr( HANDLE_T              h_vid,
                             SCC_VID_MPEG_NR_T*    pe_nr )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_MPEG_NR_T            e_vdp_mpeg_nr = VID_PLA_MPEG_NR_OFF;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpe_nr            = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pe_nr ));

    if ( !pe_nr )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }


    z_size = sizeof(VID_PLA_MPEG_NR_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MPEG_NR,
        (VOID*)&e_vdp_mpeg_nr,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch( e_vdp_mpeg_nr )
    {
    case VID_PLA_MPEG_NR_OFF:
        *pe_nr = SCC_VID_MPEG_NR_OFF;
        break;
    case VID_PLA_MPEG_NR_LOW:
        *pe_nr = SCC_VID_MPEG_NR_LOW;
        break;
    case VID_PLA_MPEG_NR_MIDDLE:
        *pe_nr = SCC_VID_MPEG_NR_MIDDLE;
        break;
    case VID_PLA_MPEG_NR_HIGH:
        *pe_nr = SCC_VID_MPEG_NR_HIGH;
        break;
    default:
        return SMR_DRV_COMP_ERROR;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_black_stretch
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          e_stretch           References.
 * 
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_black_stretch( HANDLE_T                  h_vid,
                                   SCC_VID_BLACK_STRETCH_T   e_stretch )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_BLACK_STRETCH_T      e_vdp_stretch = VID_PLA_BLACK_STRETCH_OFF;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid              = 0x%.8X\r\n"
                       "\t\te_black_stretch    = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)e_stretch ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_stretch )
    {
    case SCC_VID_BLACK_STRETCH_OFF:
        e_vdp_stretch = VID_PLA_BLACK_STRETCH_OFF;
        break;
    case SCC_VID_BLACK_STRETCH_LOW:
        e_vdp_stretch = VID_PLA_BLACK_STRETCH_LOW;
        break;
    case SCC_VID_BLACK_STRETCH_MID:
        e_vdp_stretch = VID_PLA_BLACK_STRETCH_MID;
        break;
    case SCC_VID_BLACK_STRETCH_HIGH:
        e_vdp_stretch = VID_PLA_BLACK_STRETCH_HIGH;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_BLACK_STRETCH,
        (VOID*)(UINT32)e_vdp_stretch,
        sizeof(VID_PLA_BLACK_STRETCH_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_black_stretch
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pe_stretch          References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_black_stretch( HANDLE_T                   h_vid,
                                   SCC_VID_BLACK_STRETCH_T*   pe_stretch )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_BLACK_STRETCH_T      e_vdp_stretch = VID_PLA_BLACK_STRETCH_OFF;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp             = 0x%.8X\r\n"
                       "\t\tpe_black_stretch   = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pe_stretch ));

    if ( !pe_stretch )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }


    z_size = sizeof(VID_PLA_BLACK_STRETCH_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_BLACK_STRETCH,
        (VOID*)&e_vdp_stretch,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch( e_vdp_stretch )
    {
    case VID_PLA_BLACK_STRETCH_OFF:
        *pe_stretch = SCC_VID_BLACK_STRETCH_OFF;
        break;
    case VID_PLA_BLACK_STRETCH_LOW:
        *pe_stretch = SCC_VID_BLACK_STRETCH_LOW;
        break;
    case VID_PLA_BLACK_STRETCH_MID:
        *pe_stretch = SCC_VID_BLACK_STRETCH_MID;
        break;
    case VID_PLA_BLACK_STRETCH_HIGH:
        *pe_stretch = SCC_VID_BLACK_STRETCH_HIGH;
        break;
    default:
        return SMR_DRV_COMP_ERROR;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_white_stretch
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          e_stretch           References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_white_stretch( HANDLE_T                  h_vid,
                                   SCC_VID_WHITE_STRETCH_T   e_stretch )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_WHITE_STRETCH_T      e_vdp_stretch = VID_PLA_WHITE_STRETCH_OFF;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid              = 0x%.8X\r\n"
                       "\t\te_white_stretch    = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)e_stretch ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_stretch )
    {
    case SCC_VID_WHITE_STRETCH_OFF:
        e_vdp_stretch = VID_PLA_WHITE_STRETCH_OFF;
        break;
    case SCC_VID_WHITE_STRETCH_LOW:
        e_vdp_stretch = VID_PLA_WHITE_STRETCH_LOW;
        break;
    case SCC_VID_WHITE_STRETCH_MID:
        e_vdp_stretch = VID_PLA_WHITE_STRETCH_MID;
        break;
    case SCC_VID_WHITE_STRETCH_HIGH:
        e_vdp_stretch = VID_PLA_WHITE_STRETCH_HIGH;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_WHITE_STRETCH,
        (VOID*)(UINT32)e_vdp_stretch,
        sizeof(VID_PLA_WHITE_STRETCH_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_white_stretch
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pe_stretch          References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_white_stretch( HANDLE_T                   h_vid,
                                   SCC_VID_WHITE_STRETCH_T*   pe_stretch )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_WHITE_STRETCH_T      e_vdp_stretch = VID_PLA_WHITE_STRETCH_OFF;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp             = 0x%.8X\r\n"
                       "\t\tpe_white_stretch   = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pe_stretch ));

    if ( !pe_stretch )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }


    z_size = sizeof(VID_PLA_WHITE_STRETCH_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_WHITE_STRETCH,
        (VOID*)&e_vdp_stretch,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch( e_vdp_stretch )
    {
    case VID_PLA_WHITE_STRETCH_OFF:
        *pe_stretch = SCC_VID_WHITE_STRETCH_OFF;
        break;
    case VID_PLA_WHITE_STRETCH_LOW:
        *pe_stretch = SCC_VID_WHITE_STRETCH_LOW;
        break;
    case VID_PLA_WHITE_STRETCH_MID:
        *pe_stretch = SCC_VID_WHITE_STRETCH_MID;
        break;
    case VID_PLA_WHITE_STRETCH_HIGH:
        *pe_stretch = SCC_VID_WHITE_STRETCH_HIGH;
        break;
    default:
        return SMR_DRV_COMP_ERROR;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_black_stretch_val
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          ui2_stretch         References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_black_stretch_val(
    HANDLE_T                   h_vid,
    UINT16                     ui2_stretch )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_MAX_INFO_T           t_drv_max = {0};
    UINT8                        ui1_drv_stretch = 0; 

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tui2_stretch       = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)ui2_stretch ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    if ( ui2_stretch > SCC_ATTRIB_VALUE_MAX )
    {
        return SMR_INV_SET_INFO;
    }

    t_drv_max.e_get_type = VID_PLA_GET_TYPE_BLACK_STRETCH;
    z_size = sizeof(VID_PLA_MAX_INFO_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MAX,
        (VOID*)&t_drv_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }
    if ( t_drv_max.ui4_max_value > 0xFF )
    {
        return SMR_DRV_COMP_ERROR;
    }

    ui1_drv_stretch = (UINT8)_ceil_16bits_scc_val_2_drv_val(
        SCC_ATTRIB_VALUE_MAX,
        ui2_stretch,
        (UINT16)0,
        (UINT16)t_drv_max.ui4_max_value );

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_BLACK_STRETCH,
        (VOID*)(UINT32)ui1_drv_stretch,
        sizeof(UINT8) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_black_stretch_val
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pui2_stretch        References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_black_stretch_val( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_stretch )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_MAX_INFO_T           t_drv_max = {0};
    UINT8                        ui1_drv_stretch = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp               = 0x%.8X\r\n"
                       "\t\tpui2_stretch         = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pui2_stretch ));

    if ( !pui2_stretch )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_max.e_get_type = VID_PLA_GET_TYPE_BLACK_STRETCH;
    z_size = sizeof(VID_PLA_MAX_INFO_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MAX,
        (VOID*)&t_drv_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }
    if ( t_drv_max.ui4_max_value > 0xFF )
    {
        return SMR_DRV_COMP_ERROR;
    }

    z_size = sizeof(UINT8);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_BLACK_STRETCH,
        (VOID*)&ui1_drv_stretch,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    if ( (UINT32)ui1_drv_stretch > t_drv_max.ui4_max_value )
    {
        return SMR_DRV_COMP_ERROR;
    }

    *pui2_stretch = _floor_16bits_drv_val_2_scc_val(
        SCC_ATTRIB_VALUE_MAX,
        ui1_drv_stretch,
        (UINT16)0,
        (UINT16)t_drv_max.ui4_max_value );

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_white_stretch_val
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          ui2_stretch         References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_white_stretch_val(
    HANDLE_T                   h_vid,
    UINT16                     ui2_stretch )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_MAX_INFO_T           t_drv_max = {0};
    UINT8                        ui1_drv_stretch = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tui2_stretch       = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)ui2_stretch ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    if ( ui2_stretch > SCC_ATTRIB_VALUE_MAX )
    {
        return SMR_INV_SET_INFO;
    }

    t_drv_max.e_get_type = VID_PLA_GET_TYPE_WHITE_STRETCH;
    z_size = sizeof(VID_PLA_MAX_INFO_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MAX,
        (VOID*)&t_drv_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }
    if ( t_drv_max.ui4_max_value > 0xFF )
    {
        return SMR_DRV_COMP_ERROR;
    }

    ui1_drv_stretch = (UINT8)_ceil_16bits_scc_val_2_drv_val(
        SCC_ATTRIB_VALUE_MAX,
        ui2_stretch,
        (UINT16)0,
        (UINT16)t_drv_max.ui4_max_value );

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_WHITE_STRETCH,
        (VOID*)(UINT32)ui1_drv_stretch,
        sizeof(UINT8) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_white_stretch_val
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pui2_stretch        References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_white_stretch_val(
    HANDLE_T                    h_vid,
    UINT16*                     pui2_stretch )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_MAX_INFO_T           t_drv_max = {0};
    UINT8                        ui1_drv_stretch = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp                  = 0x%.8X\r\n"
                       "\t\tpui2_stretch            = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pui2_stretch ));

    if ( !pui2_stretch )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_max.e_get_type = VID_PLA_GET_TYPE_WHITE_STRETCH;
    z_size = sizeof(VID_PLA_MAX_INFO_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MAX,
        (VOID*)&t_drv_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }
    if ( t_drv_max.ui4_max_value > 0xFF )
    {
        return SMR_DRV_COMP_ERROR;
    }

    z_size = sizeof(UINT8);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_WHITE_STRETCH,
        (VOID*)&ui1_drv_stretch,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    if ( (UINT32)ui1_drv_stretch > t_drv_max.ui4_max_value )
    {
        return SMR_DRV_COMP_ERROR;
    }

    *pui2_stretch = _floor_16bits_drv_val_2_scc_val(
        SCC_ATTRIB_VALUE_MAX,
        ui1_drv_stretch,
        (UINT16)0,
        (UINT16)t_drv_max.ui4_max_value );

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_disp_region
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pe_stretch          References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_disp_region( 
    HANDLE_T                       h_vid,
    const VSH_REGION_INFO_T*       pt_region_info )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_VID_REGION_T         t_drv_region = {0};

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tpt_region_info       = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)pt_region_info ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_region.ui4_x = pt_region_info->ui4_x;
    t_drv_region.ui4_y = pt_region_info->ui4_y;
    t_drv_region.ui4_width = pt_region_info->ui4_width;
    t_drv_region.ui4_height = pt_region_info->ui4_height;
    
    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_DISP_REGION,
        (VOID*)&t_drv_region,
        sizeof(VID_PLA_VID_REGION_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_vdp_atomic_cmd
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pe_stretch          References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_vdp_atomic_cmd( 
    HANDLE_T                       h_vid,
    SM_COMMAND_T*                  pt_atomic_cmd )
{
    INT32                        i4_ret = SMR_OK;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp;
    UINT32                       ui4_cmd_num = 0;
    SM_COMMAND_T*                pt_clint_iter = pt_atomic_cmd;
    VID_PLA_ATOMIC_CMD_T*        pt_vdp_atomic_cmd;
    VID_PLA_ATOMIC_CMD_T*        pt_vdp_iter;
    VSH_REGION_INFO_T*           pt_region_info;
    VID_PLA_VID_REGION_T         t_drv_region;
    SCC_VID_SCREEN_MODE_T        e_scr_mode;
    VID_PLA_DISP_FMT_T           e_drv_vdp_disp_fmt;
    SCC_VID_OVER_SCAN_CLIPPER_T* pt_clipper;
    VID_PLA_OVER_SCAN_CLIPPER_T  t_drv_clipper;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tpt_atomic_cmd       = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)pt_atomic_cmd ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    while( pt_clint_iter != NULL && 
           pt_clint_iter->e_code != SM_CMD_CODE_END )
    {
        ui4_cmd_num ++;
        pt_clint_iter ++;
    }
    ui4_cmd_num ++; /* add one for end command code */

    pt_vdp_atomic_cmd = 
        (VID_PLA_ATOMIC_CMD_T*) x_mem_alloc( 
            sizeof(VID_PLA_ATOMIC_CMD_T) * ui4_cmd_num );
    if ( pt_vdp_atomic_cmd == NULL )
    {
        return SMR_INSUFFICIENT_MEMORY;
    }
    x_memset(pt_vdp_atomic_cmd, 0, sizeof(VID_PLA_ATOMIC_CMD_T) * ui4_cmd_num );
    
    pt_clint_iter = pt_atomic_cmd;
    pt_vdp_iter = pt_vdp_atomic_cmd;
    while( pt_clint_iter != NULL &&
           pt_clint_iter->e_code != SM_CMD_CODE_END )
    {
        switch( pt_clint_iter->e_code )
        {
        case SCC_CMD_CODE_VIDEO_DISP_REGION:
            pt_vdp_iter->e_drv_set_type = VID_PLA_SET_TYPE_DISP_REGION;
            pt_region_info = (VSH_REGION_INFO_T*)pt_clint_iter->u.pv_data;
            
            t_drv_region.ui4_x = pt_region_info->ui4_x;
            t_drv_region.ui4_y = pt_region_info->ui4_y;
            t_drv_region.ui4_width = pt_region_info->ui4_width;
            t_drv_region.ui4_height = pt_region_info->ui4_height;
            pt_vdp_iter->u.pv_data = (VOID*)&t_drv_region;
            break;
            
        case SCC_CMD_CODE_VIDEO_SCREEN_MODE:
            pt_vdp_iter->e_drv_set_type = VID_PLA_SET_TYPE_DISP_FMT; 
            e_scr_mode = (SCC_VID_SCREEN_MODE_T)pt_clint_iter->u.ui4_data;

            e_drv_vdp_disp_fmt = _scc_scr_mode_2_dvr_disp_fmt( e_scr_mode );
            pt_vdp_iter->u.pv_data = (VOID*)(UINT32)e_drv_vdp_disp_fmt;
            break;

        case SCC_CMD_CODE_VIDEO_OVERSCAN_CLIPPER:            
            pt_vdp_iter->e_drv_set_type = VID_PLA_SET_TYPE_OVER_SCAN_CLIPPER;
            pt_clipper = (SCC_VID_OVER_SCAN_CLIPPER_T*)pt_clint_iter->u.pv_data;
            
            t_drv_clipper.ui4_top = pt_clipper->ui4_top;
            t_drv_clipper.ui4_bottom = pt_clipper->ui4_bottom;
            t_drv_clipper.ui4_left = pt_clipper->ui4_left;
            t_drv_clipper.ui4_right = pt_clipper->ui4_right;
            pt_vdp_iter->u.pv_data = (VOID*)&t_drv_clipper;
            break;
            
        default:
            SM_ASSERT(pt_clint_iter->e_code);
            break;
        }
            
        pt_clint_iter ++;
        pt_vdp_iter ++;
    }
    pt_vdp_iter->e_drv_set_type = VID_PLA_ATOMIC_CMD_CODE_END;
     
    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_ATOMIC_CMD_GRP,
        (VOID*)pt_vdp_atomic_cmd,
        sizeof(VID_PLA_ATOMIC_CMD_T) * ui4_cmd_num );
    if ( i4_ret != SMR_OK )
    {
        x_mem_free(pt_vdp_atomic_cmd);
        pt_vdp_atomic_cmd = NULL;
        return SMR_INV_SET_INFO;
    }
    
    x_mem_free(pt_vdp_atomic_cmd);
    pt_vdp_atomic_cmd = NULL;
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_colorspace
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          e_colorspace           References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_colorspace( HANDLE_T                  h_vid,
                                  SCC_VID_COLORSPACE_T    e_colorspace )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_COLORSPACE_T         e_vdp_colorspace = VID_PLA_COLORSPACE_AUTO;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\te_colorspace        = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)e_colorspace ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_colorspace )
    {
    case SCC_VID_COLORSPACE_AUTO:
        e_vdp_colorspace = VID_PLA_COLORSPACE_AUTO;
        break;
    case SCC_VID_COLORSPACE_FORCE_RGB:
        e_vdp_colorspace = VID_PLA_COLORSPACE_FORCE_RGB;
        break;
    case SCC_VID_COLORSPACE_FORCE_YCBCR:
        e_vdp_colorspace = VID_PLA_COLORSPACE_FORCE_YCBCR;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_COLORSPACE,
        (VOID*)(UINT32)e_vdp_colorspace,
        sizeof(VID_PLA_COLORSPACE_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_flip
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *             b_is_on           References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_flip( HANDLE_T                  h_vid,
                            BOOL                      b_is_on )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tb_is_on        = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)b_is_on ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }
    
    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_FLIP,
        (VOID*)(UINT32)b_is_on,
        sizeof(BOOL) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_mirror
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *             b_is_on           References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_mirror( HANDLE_T                  h_vid,
                                BOOL                      b_is_on )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tb_is_on          = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)b_is_on ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }
    
    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_MIRROR,
        (VOID*)(UINT32)b_is_on,
        sizeof(BOOL) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_flip
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pb_is_on            References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_flip(
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_on )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    BOOL                         b_is_on = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid                = 0x%.8X\r\n"
                       "\t\tpb_is_on             = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pb_is_on ));

    if ( !pb_is_on )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(BOOL);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_FLIP,
        (VOID*)&b_is_on,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    *pb_is_on = b_is_on;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_mirror
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pb_is_on            References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_mirror(
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_on )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    BOOL                         b_is_on = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid                = 0x%.8X\r\n"
                       "\t\tpb_is_on             = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pb_is_on ));

    if ( !pb_is_on )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(BOOL);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_MIRROR,
        (VOID*)&b_is_on,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    *pb_is_on = b_is_on;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_vdp_delay_time
 *
 * Description: query vdp setting delay time ms
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pui4_delay_time            References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_vdp_delay_time(
    HANDLE_T                   h_vid,
    UINT32*                    pui4_delay_time )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    UINT32                       ui4_delay_time = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid                = 0x%.8X\r\n"
                       "\t\tpui4_delay_time      = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pui4_delay_time ));

    if ( !pui4_delay_time )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(UINT32);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_DELAY_TIME,
        (VOID*)&ui4_delay_time,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    *pui4_delay_time = ui4_delay_time;

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _scc_vid_iframe_nfy_fct
 *
 * Description: <function description>
 *
 * Inputs:  pv_nfy_tag References.
 *          e_nfy_cond References.
 *          ui4_data_1 References.
 *          ui4_data_2 References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _scc_vid_src_detect_nfy_fct(
    VOID*           pv_nfy_tag,
    TV_DEC_COND_T   e_nfy_cond,
    UINT32          ui4_data_1,
    UINT32          ui4_data_2 )
{
    INT32               i4_ret;
    HANDLE_TYPE_T       e_type;
    SCC_VIDEO_COMP_T*   pt_vid_comp = NULL;
    SM_SCC_MSG_T        t_msg ;
    HANDLE_T            h_vid_src_detect = (HANDLE_T)pv_nfy_tag;

    i4_ret = handle_get_type_obj( h_vid_src_detect,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return;
    }

    switch (e_nfy_cond)
    {
        case TV_DEC_COND_ERROR:
            t_msg.u.t_vid_src_detect_nfy.e_nfy_cond = SCC_VID_SRC_DETECT_NFY_COND_ERROR;
            break;
        case TV_DEC_COND_CTRL_DONE:
            t_msg.u.t_vid_src_detect_nfy.e_nfy_cond = SCC_VID_SRC_DETECT_NFY_COND_DONE;
            break;
        default:
            return;
    }
    t_msg.e_msg_type = SM_SCC_MSG_TYPE_SRC_DETECT_NFY;
    t_msg.u.t_vid_src_detect_nfy.h_vid = h_vid_src_detect;
    t_msg.u.t_vid_src_detect_nfy.pf_nfy_fct = pt_vid_comp->pf_src_detect_nfy;
    t_msg.u.t_vid_src_detect_nfy.pv_nfy_tag = pt_vid_comp->pv_src_detect_nfy_tag;
    t_msg.u.t_vid_src_detect_nfy.ui4_data_1 = ui4_data_1;
    t_msg.u.t_vid_src_detect_nfy.ui4_data_2 = ui4_data_2;
    _scc_send_msg(&t_msg);

}


/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_src_detect
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pb_is_on            References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_src_detect_nfy(
    HANDLE_T                        h_vid, 
    x_scc_vid_src_detect_nfy_fct    pf_scc_vid_src_detect_nfy, 
    VOID*                           pv_nfy_tag) 
{
    INT32                         i4_ret;
    HANDLE_TYPE_T                 e_type;
    SCC_VIDEO_COMP_T*             pt_vid_comp = NULL;
    TV_DEC_SRC_DETECT_NFY_INFO_T  t_tvd_src_detect_nfy_info = {0};

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tpf_scc_vid_src_detect_nfy   = %x )\r\n",
                       "\t\tpv_nfy_tag             = %x )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)pf_scc_vid_src_detect_nfy ,
                       (INT32)pv_nfy_tag ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    pt_vid_comp->pf_src_detect_nfy = pf_scc_vid_src_detect_nfy;
    pt_vid_comp->pv_src_detect_nfy_tag = pv_nfy_tag;

    t_tvd_src_detect_nfy_info.pf_src_detect_nfy = 
        (x_tv_dec_src_detect_nfy_fct)_scc_vid_src_detect_nfy_fct;
    t_tvd_src_detect_nfy_info.pv_tag = (VOID*)h_vid;

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_TV_DEC,
        TV_DEC_SET_TYPE_SRC_DETECT_NFY_FCT,
        (VOID*)&t_tvd_src_detect_nfy_info,
        sizeof(TV_DEC_SRC_DETECT_NFY_INFO_T) );
    if ( i4_ret != RMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_3d_nav
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *          e_nav                 References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                Success.
 *          SMR_INTERNAL_ERROR    Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_3d_nav( 
    HANDLE_T                    h_vid,
    SCC_3D_NAV_T                e_nav )
{   
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_3D_NAV_T             e_drv_vdp_mode = VID_PLA_3D_NAV_OFF;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\te_nav            = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)e_nav ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_nav )
    {
    case SCC_3D_NAV_OFF:
        e_drv_vdp_mode = VID_PLA_3D_NAV_OFF;
        break;
    case SCC_3D_NAV_ON:
        e_drv_vdp_mode = VID_PLA_3D_NAV_ON;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_3D_NAV,
        (VOID*)(UINT32)e_drv_vdp_mode,
        sizeof(VID_PLA_3D_NAV_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_NOT_SUPPORT;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_3d_nav
 *
 * Description: <function description>
 *
 * Inputs:  h_vid              References.
 *          pe_nav             References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_3d_nav( 
    HANDLE_T                    h_vid,
    SCC_3D_NAV_T*               pe_nav )
{       
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_3D_NAV_T             e_drv_vdp_mode = VID_PLA_3D_NAV_OFF;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tpe_nav           = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pe_nav ));

    if ( pe_nav == NULL )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof( VID_PLA_3D_NAV_T );
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_3D_NAV,
        (VOID*)&e_drv_vdp_mode,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch( e_drv_vdp_mode )
    {
    case VID_PLA_3D_NAV_OFF:
        *pe_nav = SCC_3D_NAV_OFF;
        break;
    case VID_PLA_3D_NAV_ON:
        *pe_nav = SCC_3D_NAV_ON;
        break;
    default:
        return SMR_DRV_COMP_ERROR;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_3d_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          e_3d_mode           References.
 *
 * Outputs: 
 *
 *Returns: SMR_OK              Success.
 *         SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_3d_mode( 
    HANDLE_T                    h_vid,
    SCC_3D_MODE_T               e_3d_mode )
{     
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_3D_MODE_T            e_drv_vdp_mode = VID_PLA_3D_MODE_OFF;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\te_3d_mode        = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)e_3d_mode ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_3d_mode )
    {
    case SCC_3D_MODE_OFF:
        e_drv_vdp_mode = VID_PLA_3D_MODE_OFF;
        break;
    case SCC_3D_MODE_AUTO:
        e_drv_vdp_mode = VID_PLA_3D_MODE_AUTO;
        break;
    case SCC_3D_MODE_2D_TO_3D:
        e_drv_vdp_mode = VID_PLA_3D_MODE_2D_TO_3D;
        break;
    case SCC_3D_MODE_FRM_SEQ:
        e_drv_vdp_mode = VID_PLA_3D_MODE_FRM_SEQ;
        break;
    case SCC_3D_MODE_SIDE_SIDE:
        e_drv_vdp_mode = VID_PLA_3D_MODE_SIDE_SIDE;
        break;
    case SCC_3D_MODE_TOP_AND_BTM:
        e_drv_vdp_mode = VID_PLA_3D_MODE_TOP_AND_BTM;
        break;
    case SCC_3D_MODE_REALD:
        e_drv_vdp_mode = VID_PLA_3D_MODE_REALD;
        break;
    case SCC_3D_MODE_SENSIO:
        e_drv_vdp_mode = VID_PLA_3D_MODE_SENSIO;
        break;
    case SCC_3D_MODE_LINE_INTERLEAVE:
        e_drv_vdp_mode = VID_PLA_3D_MODE_LINE_INTERLEAVE;
        break;
    case SCC_3D_MODE_DOT_ALT:
        e_drv_vdp_mode = VID_PLA_3D_MODE_DOT_ALT;
        break;
    case SCC_3D_MODE_CHK_BOARD:
        e_drv_vdp_mode = VID_PLA_3D_MODE_CHK_BOARD;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_3D_MODE,
        (VOID*)(UINT32)e_drv_vdp_mode,
        sizeof(VID_PLA_3D_MODE_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_NOT_SUPPORT;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_3d_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *          pe_3d_mode            References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK               Success.
 *          SMR_INTERNAL_ERROR   Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_3d_mode( 
    HANDLE_T                    h_vid,
    SCC_3D_MODE_T*              pe_3d_mode )
{   
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_3D_MODE_T            e_drv_vdp_mode = VID_PLA_3D_MODE_OFF;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tpe_3d_mode       = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pe_3d_mode ));

    if ( pe_3d_mode == NULL )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof( VID_PLA_3D_MODE_T );
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_3D_MODE ,
        (VOID*)&e_drv_vdp_mode,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch( e_drv_vdp_mode )
    {
    case VID_PLA_3D_MODE_OFF:
        *pe_3d_mode = SCC_3D_MODE_OFF;
        break;
    case VID_PLA_3D_MODE_AUTO:
        *pe_3d_mode = SCC_3D_MODE_AUTO;
        break;
    case VID_PLA_3D_MODE_2D_TO_3D:
        *pe_3d_mode = SCC_3D_MODE_2D_TO_3D;
        break;
    case VID_PLA_3D_MODE_FRM_SEQ:
        *pe_3d_mode = SCC_3D_MODE_FRM_SEQ;
        break;
    case VID_PLA_3D_MODE_SIDE_SIDE:
        *pe_3d_mode = SCC_3D_MODE_SIDE_SIDE;
        break;
    case VID_PLA_3D_MODE_TOP_AND_BTM:
        *pe_3d_mode = SCC_3D_MODE_TOP_AND_BTM;
        break;
    case VID_PLA_3D_MODE_REALD:
        *pe_3d_mode = SCC_3D_MODE_REALD;
        break;
    case VID_PLA_3D_MODE_SENSIO:
        *pe_3d_mode = SCC_3D_MODE_SENSIO;
        break;
    case VID_PLA_3D_MODE_LINE_INTERLEAVE:
        *pe_3d_mode = SCC_3D_MODE_LINE_INTERLEAVE;
        break;
    case VID_PLA_3D_MODE_DOT_ALT:
        *pe_3d_mode = SCC_3D_MODE_DOT_ALT;
        break;
    case VID_PLA_3D_MODE_CHK_BOARD:
        *pe_3d_mode = SCC_3D_MODE_CHK_BOARD;
        break;
    default:
        return SMR_DRV_COMP_ERROR;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_3d_parallax
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                  References.
 *          ui4_prlx               References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                Success.
 *          SMR_INTERNAL_ERROR    Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_3d_parallax( 
    HANDLE_T                    h_vid,
    UINT32                      ui4_prlx )
{   
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tui4_prlx         = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)ui4_prlx ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_3D_PARALLAX,
        (VOID*)(UINT32)ui4_prlx,
        sizeof(UINT32) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_3d_parallax
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                  References.
 *          pui4_prlx              References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                 Success.
 *          SMR_INTERNAL_ERROR     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_3d_parallax( 
    HANDLE_T                    h_vid,
    UINT32*                     pui4_prlx )
{     
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tpui4_prlx        = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pui4_prlx ));

    if ( pui4_prlx == NULL )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof( UINT32 );
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_3D_PARALLAX,
        (VOID*)pui4_prlx,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }
 
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_3d_fld_depth
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                  References.
 *          ui4_fld_depth          References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                 Success.
 *          SMR_INTERNAL_ERROR     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_3d_fld_depth( 
    HANDLE_T                    h_vid,
    UINT32                      ui4_fld_depth )
{    
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tui4_fld_depth    = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)ui4_fld_depth ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_3D_FLD_DEPTH,
        (VOID*)(UINT32)ui4_fld_depth,
        sizeof(UINT32) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_3d_fld_depth
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                   References.
 *          pui4_fld_depth          References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                  Success.
 *          SMR_INTERNAL_ERROR      Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_3d_fld_depth( 
    HANDLE_T                    h_vid,
    UINT32*                     pui4_fld_depth )
{   
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tpui4_fld_depth   = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pui4_fld_depth ));

    if ( pui4_fld_depth == NULL )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof( UINT32 );
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_3D_FLD_DEPTH,
        (VOID*)pui4_fld_depth,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }
 
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_3d_lr_switch
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *          e_lr_switch           References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                Success.
 *          SMR_INTERNAL_ERROR    Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_3d_lr_switch( 
    HANDLE_T                    h_vid,
    SCC_3D_LR_SWITCH_T          e_lr_switch )
{   
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_3D_LR_SWITCH_T       e_drv_vdp_mode = VID_PLA_3D_LR_SWITCH_OFF;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\te_lr_switch      = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)e_lr_switch ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_lr_switch )
    {
    case SCC_3D_LR_SWITCH_OFF:
        e_drv_vdp_mode = VID_PLA_3D_LR_SWITCH_OFF;
        break;
    case SCC_3D_LR_SWITCH_ON:
        e_drv_vdp_mode = VID_PLA_3D_LR_SWITCH_ON;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_3D_LR_SWITCH,
        (VOID*)(UINT32)e_drv_vdp_mode,
        sizeof(VID_PLA_3D_LR_SWITCH_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_NOT_SUPPORT;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_3d_lr_switch
 *
 * Description: <function description>
 *
 * Inputs:  h_vid              References.
 *          pe_lr_switch       References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_3d_lr_switch( 
    HANDLE_T                    h_vid,
    SCC_3D_LR_SWITCH_T*         pe_lr_switch )
{       
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_3D_LR_SWITCH_T       e_drv_vdp_mode = VID_PLA_3D_LR_SWITCH_OFF;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tpe_lr_switch     = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pe_lr_switch ));

    if ( pe_lr_switch == NULL )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof( VID_PLA_3D_LR_SWITCH_T );
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_3D_LR_SWITCH,
        (VOID*)&e_drv_vdp_mode,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch( e_drv_vdp_mode )
    {
    case VID_PLA_3D_LR_SWITCH_OFF:
        *pe_lr_switch = SCC_3D_LR_SWITCH_OFF;
        break;
    case VID_PLA_3D_LR_SWITCH_ON:
        *pe_lr_switch = SCC_3D_LR_SWITCH_ON;
        break;
    default:
        return SMR_DRV_COMP_ERROR;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_3d_to_2d
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                References.
 *           e_3d_to_2d          References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK               Success.
 *          SMR_INTERNAL_ERROR   Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_3d_to_2d( 
    HANDLE_T                    h_vid,
    SCC_3D_TO_2D_T              e_3d_to_2d )
{      
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_3D_TO_2D_T           e_drv_vdp_mode = VID_PLA_3D_TO_2D_OFF;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\te_3d_to_2d       = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32) e_3d_to_2d ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_3d_to_2d )
    {
    case SCC_3D_TO_2D_OFF:
        e_drv_vdp_mode = VID_PLA_3D_TO_2D_OFF;
        break;
    case SCC_3D_TO_2D_LEFT:
        e_drv_vdp_mode = VID_PLA_3D_TO_2D_LEFT;
        break;
    case SCC_3D_TO_2D_RIGHT:
        e_drv_vdp_mode = VID_PLA_3D_TO_2D_RIGHT;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_3D_TO_2D,
        (VOID*)(UINT32)e_drv_vdp_mode,
        sizeof(VID_PLA_3D_TO_2D_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_NOT_SUPPORT;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_3d_to_2d
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *          e_3d_to_2d            References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                Success.
 *          SMR_INTERNAL_ERROR    Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_3d_to_2d( 
    HANDLE_T                    h_vid,
    SCC_3D_TO_2D_T*             pe_3d_to_2d )
{    
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_3D_TO_2D_T           e_drv_vdp_mode = VID_PLA_3D_TO_2D_OFF;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tpe_3d_to_2d      = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pe_3d_to_2d ));

    if ( pe_3d_to_2d == NULL )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(VID_PLA_3D_TO_2D_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_3D_TO_2D,
        (VOID*)&e_drv_vdp_mode,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch(e_drv_vdp_mode)
    {
    case VID_PLA_3D_TO_2D_OFF:
        *pe_3d_to_2d = SCC_3D_TO_2D_OFF;
        break;
    case VID_PLA_3D_TO_2D_LEFT:
        *pe_3d_to_2d = SCC_3D_TO_2D_LEFT;
        break;
    case VID_PLA_3D_TO_2D_RIGHT:
        *pe_3d_to_2d = SCC_3D_TO_2D_RIGHT;
        break;
    default:
        return SMR_DRV_COMP_ERROR;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_3d_fpr
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *          e_fpr                 References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                Success.
 *          SMR_INTERNAL_ERROR    Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_3d_fpr( 
    HANDLE_T                    h_vid,
    SCC_3D_FPR_T                e_fpr )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_3D_FPR_T             e_drv_vdp_mode = VID_PLA_3D_FPR_NORMAL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\te_fpr            = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)e_fpr ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_fpr )
    {
    case SCC_3D_FPR_NORMAL:
        e_drv_vdp_mode = VID_PLA_3D_FPR_NORMAL;
        break;
    case SCC_3D_FPR_FPR1:
        e_drv_vdp_mode = VID_PLA_3D_FPR_FPR1;
        break;
    case SCC_3D_FPR_FPR2:
        e_drv_vdp_mode = VID_PLA_3D_FPR_FPR2;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_3D_FPR,
        (VOID*)(UINT32)e_drv_vdp_mode,
        sizeof(VID_PLA_3D_FPR_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_NOT_SUPPORT;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_3d_fpr
 *
 * Description: <function description>
 *
 * Inputs:  h_vid              References.
 *          pe_fpr             References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_3d_fpr( 
    HANDLE_T                    h_vid,
    SCC_3D_FPR_T*               pe_fpr )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_3D_FPR_T             e_drv_vdp_mode = VID_PLA_3D_FPR_NORMAL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tpe_fpr           = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pe_fpr ));

    if ( pe_fpr == NULL )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof( VID_PLA_3D_FPR_T );
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_3D_FPR,
        (VOID*)&e_drv_vdp_mode,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch( e_drv_vdp_mode )
    {
    case VID_PLA_3D_FPR_NORMAL:
        *pe_fpr = SCC_3D_FPR_NORMAL;
        break;
    case VID_PLA_3D_FPR_FPR1:
        *pe_fpr = SCC_3D_FPR_FPR1;
        break;
    case VID_PLA_3D_FPR_FPR2:
        *pe_fpr = SCC_3D_FPR_FPR2;
        break;
    default:
        return SMR_DRV_COMP_ERROR;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_3d_protrude_scr
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                   References.
 *          ui4_pro_scr             References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                  Success.
 *          SMR_INTERNAL_ERROR      Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_3d_protrude_scr( 
    HANDLE_T                    h_vid,
    UINT32                      ui4_pro_scr )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tui4_pro_scr      = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)ui4_pro_scr ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_3D_PROTRUDE_SCR,
        (VOID*)(UINT32)ui4_pro_scr,
        sizeof(UINT32) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_3d_protrude_scr
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                   References.
 *          pui4_pro_scr            References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                  Success.
 *          SMR_INTERNAL_ERROR      Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_3d_protrude_scr( 
    HANDLE_T                    h_vid,
    UINT32*                     pui4_pro_scr )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tpui4_pro_scr     = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pui4_pro_scr ));

    if ( pui4_pro_scr == NULL )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof( UINT32 );
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_3D_PROTRUDE_SCR,
        (VOID*)pui4_pro_scr,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }
 
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_3d_distance_2_tv
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                   References.
 *          ui4_dis_2_tv            References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                  Success.
 *          SMR_INTERNAL_ERROR      Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_3d_distance_2_tv( 
    HANDLE_T                    h_vid,
    UINT32                      ui4_dis_2_tv )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tui4_dis_2_tv     = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)ui4_dis_2_tv ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_3D_DISTANCE_TV,
        (VOID*)(UINT32)ui4_dis_2_tv,
        sizeof(UINT32) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_3d_distance_2_tv
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                   References.
 *          pui4_dis_2_tv           References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                  Success.
 *          SMR_INTERNAL_ERROR      Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_3d_distance_2_tv( 
    HANDLE_T                    h_vid,
    UINT32*                     pui4_dis_2_tv )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tpui4_dis_2_tv    = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pui4_dis_2_tv ));

    if ( pui4_dis_2_tv == NULL )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof( UINT32 );
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_3D_DISTANCE_TV,
        (VOID*)pui4_dis_2_tv,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }
 
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_3d_osd_depth
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                   References.
 *          ui4_osd_depth           References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                  Success.
 *          SMR_INTERNAL_ERROR      Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_3d_osd_depth( 
    HANDLE_T                    h_vid,
    UINT32                      ui4_osd_depth )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tui4_osd_depth    = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)ui4_osd_depth ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_3D_OSD_DEPTH,
        (VOID*)(UINT32)ui4_osd_depth,
        sizeof(UINT32) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_3d_osd_depth
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                   References.
 *          pui4_osd_depth          References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                  Success.
 *          SMR_INTERNAL_ERROR      Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_3d_osd_depth( 
    HANDLE_T                    h_vid,
    UINT32*                     pui4_osd_depth )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tpui4_osd_depth   = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pui4_osd_depth ));

    if ( pui4_osd_depth == NULL )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof( UINT32 );
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_3D_OSD_DEPTH,
        (VOID*)pui4_osd_depth,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }
 
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_3d_fmt_cap
 *
 * Description: 
 *      get 3d fomat capability, from LSB to MSB is E_TDTV_UI_3D_MODE_OFF, E_TDTV_UI_3D_MODE_AUTO,
 * E_TDTV_UI_3D_MODE_TTD, E_TDTV_UI_3D_MODE_FS, E_TDTV_UI_3D_MODE_SBS, E_TDTV_UI_3D_MODE_TB,
 * E_TDTV_UI_3D_MODE_RD,  E_TDTV_UI_3D_MODE_SS, E_TDTV_UI_3D_MODE_LI,    E_TDTV_UI_3D_MODE_CB,
 * E_TDTV_UI_3D_MODE_DA,  E_TDTV_UI_3D_MODE_END
 * one format using two bits:    0: E_TDTV_UI_HIDE; 1: E_IDTV_UI_GREYOUT; 2: E_IDTV_UI_RSV; 3: E_IDTV_UI_ENABLE.
 *
 * Inputs:  h_vid                  References.
 *          pui4_fmt_cap           References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                 Success.
 *          SMR_INTERNAL_ERROR     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_3d_fmt_cap( 
    HANDLE_T                    h_vid,
    UINT32*                     pui4_fmt_cap )
{     
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tpui4_fmt_cap     = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pui4_fmt_cap ));

    if ( pui4_fmt_cap == NULL )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof( UINT32 );
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_3D_FMT_CAPABILITY,
        (VOID*)pui4_fmt_cap,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }
 
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_3d_ctrl_cap
 *
 * Description: 
 *       get 3d control capability, from LSB to MSB is E_TDTV_UI_CTRL_3D_MODE, E_TDTV_UI_CTRL_LR_INVERSE, 
 * E_TDTV_UI_CTRL_DEPTH_CTRL, E_TDTV_UI_CTRL_PROTRUDE, E_TDTV_UI_CTRL_DISTANCE_TV, E_TDTV_UI_CTRL_3D_TO_2D, 
 * E_TDTV_UI_CTRL_OSD_DEPTH,  E_TDTV_UI_CTRL_FPR, E_TDTV_UI_CTRL_TDNAVI, E_TDTV_UI_CTRL_IMG_SAFETY.
 * one ctrl using two bits:    0: E_TDTV_UI_HIDE; 1: E_IDTV_UI_GREYOUT; 2: E_IDTV_UI_RSV; 3: E_IDTV_UI_ENABLE.
 * Inputs:  h_vid                  References.
 *          pui4_ctrl_cap          References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                 Success.
 *          SMR_INTERNAL_ERROR     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_3d_ctrl_cap( 
    HANDLE_T                    h_vid,
    UINT32*                     pui4_ctrl_cap )
{     
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tpui4_ctrl_cap    = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pui4_ctrl_cap ));

    if ( pui4_ctrl_cap == NULL )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof( UINT32 );
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_3D_CTRL_CAPABILITY,
        (VOID*)pui4_ctrl_cap,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }
 
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_3d_nav_auto_chg_en
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                  References.
 *          e_auto_chg             References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                 Success.
 *          SMR_INTERNAL_ERROR     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_3d_nav_auto_chg_en( 
    HANDLE_T                    h_vid,
    SCC_3D_NAV_AUTO_CHG_T       e_auto_chg )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_3D_NAV_AUTO_CHG_T    e_drv_auto_chg = VID_PLA_3D_NAV_AUTO_CHG_MANUAL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\te_auto_chg       = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)e_auto_chg ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_auto_chg )
    {
    case SCC_3D_NAV_AUTO_CHG_MANUAL:
        e_drv_auto_chg = VID_PLA_3D_NAV_AUTO_CHG_MANUAL;
        break;
    case SCC_3D_NAV_AUTO_CHG_SEMI_AUTO:
        e_drv_auto_chg = VID_PLA_3D_NAV_AUTO_CHG_SEMI_AUTO;
        break;
    case SCC_3D_NAV_AUTO_CHG_AUTO:
        e_drv_auto_chg = VID_PLA_3D_NAV_AUTO_CHG_AUTO;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_3D_NAVI_AUTO_CHG_EN,
        (VOID*)(UINT32)e_drv_auto_chg,
        sizeof(VID_PLA_3D_NAV_AUTO_CHG_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_NOT_SUPPORT;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_3d_nav_auto_chg_en
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *          
 * Outputs: 
 *          pe_auto_chg           References.
 *
 * Returns: SMR_OK                 Success.
 *          SMR_INTERNAL_ERROR     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_3d_nav_auto_chg_en( 
    HANDLE_T                    h_vid,
    SCC_3D_NAV_AUTO_CHG_T*      pe_auto_chg )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_3D_NAV_AUTO_CHG_T    e_drv_auto_chg = VID_PLA_3D_NAV_AUTO_CHG_MANUAL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tpe_auto_chg      = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pe_auto_chg ));

    if ( pe_auto_chg == NULL )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof( VID_PLA_3D_NAV_AUTO_CHG_T );
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_3D_NAVI_AUTO_CHG_EN ,
        (VOID*)&e_drv_auto_chg,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch( e_drv_auto_chg )
    {
    case VID_PLA_3D_NAV_AUTO_CHG_MANUAL:
        *pe_auto_chg = SCC_3D_NAV_AUTO_CHG_MANUAL;
        break;
    case VID_PLA_3D_NAV_AUTO_CHG_SEMI_AUTO:
        *pe_auto_chg = SCC_3D_NAV_AUTO_CHG_SEMI_AUTO;
        break;
    case VID_PLA_3D_NAV_AUTO_CHG_AUTO:
        *pe_auto_chg = SCC_3D_NAV_AUTO_CHG_AUTO;
        break;
    default:
        return SMR_DRV_COMP_ERROR;
    }
    
    return SMR_OK;    
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_3d_image_safety_en
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                  References.
 *          e_img_safe             References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                 Success.
 *          SMR_INTERNAL_ERROR     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_3d_image_safety_en( 
    HANDLE_T                    h_vid,
    SCC_3D_IMAGE_SAFETY_T       e_img_safe )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_3D_IMAGE_SAFETY_T    e_drv_img_safe = VID_PLA_3D_IMAGE_SAFETY_OFF;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\te_img_safe       = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)e_img_safe ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_img_safe )
    {
    case SCC_3D_IMAGE_SAFETY_OFF:
        e_drv_img_safe = VID_PLA_3D_IMAGE_SAFETY_OFF;
        break;
    case SCC_3D_IMAGE_SAFETY_LOW:
        e_drv_img_safe = VID_PLA_3D_IMAGE_SAFETY_LOW;
        break;
    case SCC_3D_IMAGE_SAFETY_MID:
        e_drv_img_safe = VID_PLA_3D_IMAGE_SAFETY_MID;
        break;
    case SCC_3D_IMAGE_SAFETY_HIGH:
        e_drv_img_safe = VID_PLA_3D_IMAGE_SAFETY_HIGH;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_3D_IMAGE_SAFETY_EN,
        (VOID*)(UINT32)e_drv_img_safe,
        sizeof(VID_PLA_3D_IMAGE_SAFETY_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_NOT_SUPPORT;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_3d_image_safety_en
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *          
 * Outputs: 
 *          pe_img_safe           References.
 *
 * Returns: SMR_OK                 Success.
 *          SMR_INTERNAL_ERROR     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_3d_image_safety_en( 
    HANDLE_T                    h_vid,
    SCC_3D_IMAGE_SAFETY_T*      pe_img_safe )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_3D_IMAGE_SAFETY_T    e_drv_img_safe = VID_PLA_3D_IMAGE_SAFETY_OFF;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tpe_img_safe      = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pe_img_safe ));

    if ( pe_img_safe == NULL )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof( VID_PLA_3D_IMAGE_SAFETY_T );
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_3D_IMAGE_SAFETY_EN ,
        (VOID*)&e_drv_img_safe,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch( e_drv_img_safe )
    {
    case VID_PLA_3D_IMAGE_SAFETY_OFF:
        *pe_img_safe = SCC_3D_IMAGE_SAFETY_OFF;
        break;
    case VID_PLA_3D_IMAGE_SAFETY_LOW:
        *pe_img_safe = SCC_3D_IMAGE_SAFETY_LOW;
        break;
    case VID_PLA_3D_IMAGE_SAFETY_MID:
        *pe_img_safe = SCC_3D_IMAGE_SAFETY_MID;
        break;
    case VID_PLA_3D_IMAGE_SAFETY_HIGH:
        *pe_img_safe = SCC_3D_IMAGE_SAFETY_HIGH;
        break;
    default:
        return SMR_DRV_COMP_ERROR;
    }
    
    return SMR_OK;       
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_3d_nav_tag
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                  References.
 *          
 * Outputs: 
 *          pe_nav_tag             References.
 *
 * Returns: SMR_OK                 Success.
 *          SMR_INTERNAL_ERROR     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_3d_nav_tag( 
    HANDLE_T                    h_vid,
    SCC_3D_NAV_TAG_T*           pe_nav_tag )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    TV_DEC_TAG3D_TYPE_T          e_drv_nav_tag = TV_DEC_TAG3D_2D;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tpe_nav_tag       = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pe_nav_tag ));

    if ( pe_nav_tag == NULL )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof( TV_DEC_TAG3D_TYPE_T );
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_3D_NAVI_TAG ,
        (VOID*)&e_drv_nav_tag,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch( e_drv_nav_tag )
    {
    case TV_DEC_TAG3D_2D:
        *pe_nav_tag = SCC_3D_NAV_TAG_2D;
        break;
    case TV_DEC_TAG3D_MVC:
        *pe_nav_tag = SCC_3D_NAV_TAG_MVC;
        break;
    case TV_DEC_TAG3D_FP:
        *pe_nav_tag = SCC_3D_NAV_TAG_FP;
        break;
    case TV_DEC_TAG3D_FS:
        *pe_nav_tag = SCC_3D_NAV_TAG_FS;
        break;
    case TV_DEC_TAG3D_TB:
        *pe_nav_tag = SCC_3D_NAV_TAG_TB;
        break;
    case TV_DEC_TAG3D_SBS:
        *pe_nav_tag = SCC_3D_NAV_TAG_SBS;
        break;
    case TV_DEC_TAG3D_REALD:
        *pe_nav_tag = SCC_3D_NAV_TAG_REALD;
        break;
    case TV_DEC_TAG3D_SENSIO:
        *pe_nav_tag = SCC_3D_NAV_TAG_SENSIO;
        break;
    case TV_DEC_TAG3D_LA:
        *pe_nav_tag = SCC_3D_NAV_TAG_LA;
        break;
    case TV_DEC_TAG3D_TTDO:
        *pe_nav_tag = SCC_3D_NAV_TAG_TTDO;
        break;
    case TV_DEC_TAG3D_NOT_SUPPORT:
        *pe_nav_tag = SCC_3D_NAV_TAG_NOT_SUPPORT;
        break;
    default:
        return SMR_DRV_COMP_ERROR;
    }
    
    return SMR_OK; 
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_vid_dec_cap
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *          pt_vid_dec_cap        References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_vid_dec_cap( 
    HANDLE_T                              h_vid,
    SCC_VID_DEC_CAPABILITY_INFO_T*        pt_vid_dec_cap )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_DEC_CAPABILITY_INFO_T    t_drv_vid_dec_cap = {0};

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tpt_vid_dec_cap    = 0x%.8X  )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pt_vid_dec_cap ));

    if ( pt_vid_dec_cap == NULL )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(VID_DEC_CAPABILITY_INFO_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_DEC,
        VID_DEC_GET_TYPE_CAPABILITY,
        (VOID*)&t_drv_vid_dec_cap,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    pt_vid_dec_cap->ui4_fmt_timing = t_drv_vid_dec_cap.ui4_fmt_timing;
    pt_vid_dec_cap->ui4_codec_type = t_drv_vid_dec_cap.ui4_codec_type;
    pt_vid_dec_cap->b_3d_multi_track_available = t_drv_vid_dec_cap.b_3d_multi_track_available;
 
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_pedestal
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *          b_is_on               References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_pedestal(
    HANDLE_T                   h_vid,
    BOOL                       b_is_on )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tb_is_on           = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)b_is_on ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_PEDESTAL,
        (VOID*)(UINT32)b_is_on,
        sizeof(BOOL) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_hdmi_range
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *          e_hdmi_range          References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                Success.
 *          SMR_INTERNAL_ERROR    Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_hdmi_range( 
    HANDLE_T                    h_vid,
    SCC_HDMI_RANGE_T            e_hdmi_range )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_HDMI_RANGE_T         e_drv_vdp_mode = VID_PLA_HDMI_RANGE_AUTO;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\te_hdmi_range     = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32) e_hdmi_range ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_hdmi_range )
    {
    case SCC_HDMI_RANGE_AUTO:
        e_drv_vdp_mode = VID_PLA_HDMI_RANGE_AUTO;
        break;
    case SCC_HDMI_RANGE_LIMIT:
        e_drv_vdp_mode = VID_PLA_HDMI_RANGE_LIMIT;
        break;
    case SCC_HDMI_RANGE_FULL:
        e_drv_vdp_mode = VID_PLA_HDMI_RANGE_FULL;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_HDMI_RANGE,
        (VOID*)(UINT32)e_drv_vdp_mode,
        sizeof(VID_PLA_HDMI_RANGE_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_NOT_SUPPORT;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_overscan_enable
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *          pb_is_enable          References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_overscan_enable( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_enable )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    BOOL                         b_is_enable = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid               = 0x%.8X\r\n"
                       "\t\tpb_is_enable        = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pb_is_enable ));

    if ( pb_is_enable == NULL )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp ==NULL )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(BOOL);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_OVERSCAN_ENABLE,
        (VOID*)&b_is_enable,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    *pb_is_enable = b_is_enable;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_nonlinear_scaling_enable
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *          pb_is_enable          References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_nonlinear_scaling_enable( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_enable )
{ 
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    BOOL                         b_is_enable = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp               = 0x%.8X\r\n"
                       "\t\tpb_is_enable         = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pb_is_enable ));

    if ( pb_is_enable == NULL )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(BOOL);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_NON_LINEAR_SCALING,
        (VOID*)&b_is_enable,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    *pb_is_enable = b_is_enable;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_3d_panel_type
 *
 * Description: <function description>
 *
 * Inputs:  h_vid              References.
 *          pe_panel_type      References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_3d_panel_type( 
    HANDLE_T                    h_vid,
    SCC_3D_PANEL_TYPE_T*        pe_panel_type )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_3D_PANEL_TYPE_T      e_drv_vdp_mode = VID_PLA_2D_PANEL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tpe_panel_type    = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pe_panel_type ));

    if ( pe_panel_type == NULL )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof( VID_PLA_3D_PANEL_TYPE_T );
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_3D_PANEL_TYPE,
        (VOID*)&e_drv_vdp_mode,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch( e_drv_vdp_mode )
    {
    case VID_PLA_2D_PANEL:
        *pe_panel_type = SCC_2D_PANEL;
        break;
    case VID_PLA_3D_PR_PANEL:
        *pe_panel_type = SCC_3D_PR_PANEL;
        break;
    case VID_PLA_3D_SG_PANEL:
        *pe_panel_type = SCC_3D_SG_PANEL;
        break;
    default:
        return SMR_DRV_COMP_ERROR;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_vga_detect
 *
 * Description: <function description>
 *
 * Inputs:  h_vid            References.
 *          e_vga_detect     References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_vga_detect( 
    HANDLE_T                    h_vid,
    SCC_VGA_DETECT_T            e_vga_detect )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    TV_DEC_VGA_DETECT_T          e_drv_vga_detect = TV_DEC_VGA_ENABLE_DETECTION;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\te_vga_detect     = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (INT32)e_vga_detect ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_vga_detect )
    {
    case SCC_VGA_ENABLE_DETECTION:
        e_drv_vga_detect = TV_DEC_VGA_ENABLE_DETECTION;
        break;
    case SCC_VGA_DISABLE_DETECTION:
        e_drv_vga_detect = TV_DEC_VGA_DISABLE_DETECTION;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_TV_DEC,
        TV_DEC_SET_TYPE_VGA_DETECT,
        (VOID*)(UINT32)e_drv_vga_detect,
        sizeof(TV_DEC_VGA_DETECT_T));
    if ( i4_ret != SMR_OK )
    {
        return SMR_NOT_SUPPORT;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_vga_status
 *
 * Description: <function description>
 *
 * Inputs:  h_vid              References.
 *          pe_vga_status   References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_vga_status( 
    HANDLE_T                 h_vid,
    SCC_VGA_STATUS_T*        pe_vga_status )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    TV_DEC_VGA_STATUS_T          e_drv_vga_status = TV_DEC_VGA_SIGNAL_NO_SIGNAL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tpe_vga_status     = 0x%.8X  )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pe_vga_status ));

    if ( pe_vga_status == NULL )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(TV_DEC_VGA_STATUS_T);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_TV_DEC,
        TV_DEC_GET_TYPE_VGA_STATUS,
        (VOID*)&e_drv_vga_status,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch( e_drv_vga_status )
    {
    case TV_DEC_VGA_SIGNAL_STABLE:
        *pe_vga_status = SCC_VGA_SIGNAL_STABLE;
        break;
    case TV_DEC_VGA_SIGNAL_NO_SIGNAL:
        *pe_vga_status = SCC_VGA_SIGNAL_NO_SIGNAL;
        break;
    case TV_DEC_VGA_SIGNAL_NOT_SUPPORT:
        *pe_vga_status = SCC_VGA_SIGNAL_NOT_SUPPORT;
        break;
    default:
        return SMR_DRV_COMP_ERROR;
    }
 
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_static_gamma
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *          ui1_static_gamma      References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                Success.
 *          SMR_INTERNAL_ERROR    Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_static_gamma( 
    HANDLE_T                    h_vid,
    UINT8                       ui1_static_gamma )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tui1_static_gamma = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT8)ui1_static_gamma ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_STATIC_GAMMA,
        (VOID*)(UINT32)ui1_static_gamma,
        sizeof(UINT8) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_angular_sce
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *          pt_angular_sce        References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                Success.
 *          SMR_INTERNAL_ERROR    Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_angular_sce( 
    HANDLE_T                    h_vid,
    SCC_ANGULAR_SCE_T*          pt_angular_sce )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_ANGULAR_SCE_T        t_drv_angular_sce;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid            = 0x%.8X\r\n"
                       "\t\tpt_angular_sce   = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pt_angular_sce ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_angular_sce.ui1_color = pt_angular_sce->ui1_color;
    t_drv_angular_sce.ui1_ysh   = pt_angular_sce->ui1_ysh;
    t_drv_angular_sce.ui1_value = pt_angular_sce->ui1_value;
    
    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_ANGULAR_SCE,
        (VOID*)&t_drv_angular_sce,
        sizeof(VID_PLA_ANGULAR_SCE_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_down_bw_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          ui1_down_bw_mode    References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                 Success.
 *          SMR_INTERNAL_ERROR     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_down_bw_mode( 
    HANDLE_T                    h_vid,
    UINT8                       ui1_down_bw_mode )
{    
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tui1_down_bw_mode  = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)ui1_down_bw_mode ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_DOWN_BW_MODE,
        (VOID*)(UINT32)ui1_down_bw_mode,
        sizeof(UINT8) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_3d_mjc_enable
 *
 * Description: For query whether support 3D mjc in driver
 *
 * Inputs:  h_vid                 References.
 *          pb_is_enable          References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_3d_mjc_enable( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_enable )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    BOOL                         b_is_enable = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid               = 0x%.8X\r\n"
                       "\t\tpb_is_enable        = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pb_is_enable ));

    if ( pb_is_enable == NULL )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(BOOL);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_3D_MJC_ENABLE,
        (VOID*)&b_is_enable,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    *pb_is_enable = b_is_enable;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_freq
 *
 * Description: For AP query video v-freqency and h-freqency
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_freq( 
    HANDLE_T                h_vid,
    SCC_VID_FREQ_T*         pt_freq )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    TV_DEC_VID_FREQ_T            t_drv_vid_freq = {0};

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid           = 0x%.8X\r\n"
                       "\t\tpt_freq         = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pt_freq ));

    if ( pt_freq == NULL )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof( TV_DEC_VID_FREQ_T );
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_TV_DEC,
        TV_DEC_GET_TYPE_VID_FREQ,
        (VOID*)&t_drv_vid_freq,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    pt_freq->b_valid = t_drv_vid_freq.b_valid;
    pt_freq->ui2_v_freq = t_drv_vid_freq.ui2_v_freq;
    pt_freq->ui2_h_freq = t_drv_vid_freq.ui2_h_freq;
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_src_detect_status
 *
 * Description: For AP query source detect status.
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_src_detect_status( 
    HANDLE_T                    h_vid,
    SCC_VID_SRC_DET_STATUS_T*   pt_src_status )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    _TV_DEC_SOURCE_STATUS_T      t_drv_src_status = {0};

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid           = 0x%.8X\r\n"
                       "\t\tpt_src_status   = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pt_src_status ));

    if ( pt_src_status == NULL )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof( _TV_DEC_SOURCE_STATUS_T );
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_TV_DEC,
        TV_DEC_GET_TYPE_SRC_DETECT_STATUS,
        (VOID*)&t_drv_src_status,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    pt_src_status->ui2_av1_status    = t_drv_src_status.ui2_av1_status;
    pt_src_status->ui2_av2_status    = t_drv_src_status.ui2_av2_status;
    pt_src_status->ui2_ypbpr1_status = t_drv_src_status.ui2_ypbpr1_status;
    pt_src_status->ui2_hdmi1_status  = t_drv_src_status.ui2_hdmi1_status;
    pt_src_status->ui2_hdmi2_status  = t_drv_src_status.ui2_hdmi2_status;
    pt_src_status->ui2_hdmi3_status  = t_drv_src_status.ui2_hdmi3_status;
    pt_src_status->ui2_hdmi4_status  = t_drv_src_status.ui2_hdmi4_status;
    pt_src_status->ui2_vga_status    = t_drv_src_status.ui2_vga_status;
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_set_super_resolution
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          b_is_on             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_set_super_resolution(
    HANDLE_T                   h_vid,
    BOOL                       b_is_on )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid             = 0x%.8X\r\n"
                       "\t\tb_is_on           = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)b_is_on ));

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_vid_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = _scc_comp_vid_set(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_SET_TYPE_SUPER_RESOLUTION,
        (VOID*)(UINT32)b_is_on,
        sizeof(BOOL) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_asp_ratio_enable
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pb_is_enable    References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_asp_ratio_enable(
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_enable )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    BOOL                         b_is_enable = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid                = 0x%.8X\r\n"
                       "\t\tpb_is_enable         = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pb_is_enable ));

    if ( !pb_is_enable )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(BOOL);
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_ASP_RATIO_ENABLE,
        (VOID*)&b_is_enable,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    *pb_is_enable = b_is_enable;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_screen_mode_enable
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *
 * Outputs: pt_scr_enable       References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_vid_get_screen_mode_enable(
    HANDLE_T                   h_vid,
    SCC_VID_SCR_MODE_T*        pt_scr_enable )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_VIDEO_COMP_T*            pt_vid_comp = NULL;
    VID_PLA_GET_ASPECT_RATIO_TYPE_T t_drv_asp_ratio_enable;
        
    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_vid                = 0x%.8X\r\n"
                       "\t\tpt_scr_enable        = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_vid,
                       (UINT32)pt_scr_enable ));

    if ( !pt_scr_enable )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_asp_ratio_enable.ui2_asp_ratio_type = pt_scr_enable->ui2_scr_mode;
    z_size = sizeof( VID_PLA_GET_ASPECT_RATIO_TYPE_T );
    
    i4_ret = _scc_comp_vid_get(
        h_vid,
        DRVT_VID_PLANE,
        VID_PLA_GET_TYPE_ASP_RATIO_ENABLE,
        (VOID*)&t_drv_asp_ratio_enable,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    pt_scr_enable->b_scr_mode_enable = t_drv_asp_ratio_enable.b_e_get_info;
    
    return SMR_OK;
}

/* Display related API *******************************************************/
/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_set_ctrl
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *          e_ctrl              References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_set_ctrl(
    HANDLE_T                        h_disp,
    SCC_DISP_CTRL_TYPE_T            e_ctrl )
{
    INT32                        i4_ret, i2_i = 0;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    SM_SCC_MSG_T                 t_msg ; 
    HANDLE_T                     h_clone = NULL_HANDLE;
    SM_COMMAND_T                 at_cmd_sets[8];

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\te_ctrl           = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)e_ctrl ));

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_ctrl )
    {
    case SCC_DISP_CTRL_RESET:
    case SCC_DISP_CTRL_DISABLE:
    case SCC_DISP_CTRL_ENABLE:
        break;

    default:
        return SMR_INV_SET_INFO;
    }

    /* display */
    at_cmd_sets [i2_i].e_code       = SCC_CMD_CODE_GRP_TYPE;
    at_cmd_sets [i2_i++].u.ui4_data = SCC_CMD_TYPE_DISP;

    at_cmd_sets [i2_i].e_code       = SCC_CMD_CODE_NAME;
    at_cmd_sets [i2_i++].u.ps_text  = pt_disp_comp->s_grp_name;

    at_cmd_sets [i2_i].e_code       = SM_CMD_CODE_END;

    i4_ret = x_scc_comp_open( at_cmd_sets, &h_clone );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    t_msg.e_msg_type = SM_SCC_MSG_TYPE_DISP_CTRL;
    t_msg.u.t_disp_ctrl.h_disp = h_clone;
    t_msg.u.t_disp_ctrl.e_disp_ctrl = e_ctrl;

    i4_ret = _scc_send_msg( &t_msg );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_set_attrib
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *          e_attrib            References.
 *          ui1_value           References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_set_attrib( HANDLE_T             h_disp,
                             SCC_DISP_ATTRIB_T    e_attrib,
                             UINT16               ui2_value )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    UINT32                       e_drv_get_type, e_drv_set_type;
    UINT16                       ui2_drv_value;
    UINT16                       ui2_idx, ui2_num_drv;
    PLA_MXR_MIN_MAX_INFO_T       t_drv_min_max = { 0, 0, 0 };

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\te_attrib         = %d\r\n"
                       "\t\tui2_value        = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (INT32)e_attrib,
                       (UINT32)ui2_value ));


    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    if ( e_attrib == SCC_DISP_ATTRIB_GAMMA )
    {
        UINT8    ui1_max_gamma;

        i4_ret = x_scc_disp_get_max_gamma( h_disp, & ui1_max_gamma );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
        if ( ui2_value > (UINT16)ui1_max_gamma )
        {
            return SMR_INV_SET_INFO;
        }

        i4_ret = _scc_comp_disp_set(
            h_disp,
            DRVT_PLA_MXR,
            PLA_MXR_SET_TYPE_GAMMA,
            (VOID*)(UINT32)ui2_value,
            sizeof( UINT8 ) );
        if ( i4_ret != SMR_OK )
        {
            return SMR_INV_SET_INFO;
        }
    }
    else if ( e_attrib == SCC_DISP_ATTRIB_DITHER )
    {
        UINT32    ui4_dither_cap;

        i4_ret = x_scc_disp_get_dither_cap( h_disp, & ui4_dither_cap );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
        if ( ui2_value > 32 ||
             !( (1<<(ui2_value-1)) & ui4_dither_cap ) )
        {
            return SMR_INV_SET_INFO;
        }

        i4_ret = _scc_comp_disp_set(
            h_disp,
            DRVT_PLA_MXR,
            PLA_MXR_SET_TYPE_DITHER,
            (VOID*)(UINT32)ui2_value,
            sizeof( UINT8 ) );
        if ( i4_ret != SMR_OK )
        {
            return SMR_INV_SET_INFO;
        }
    }
    else
    {
        e_drv_get_type = _scc_attrib_2_drv_pla_mxr_get_type( e_attrib );
        e_drv_set_type = _scc_attrib_2_drv_pla_mxr_set_type( e_attrib );
        if ( e_drv_get_type == SCC_INV_SET_GET_TYPE ||
             e_drv_set_type == SCC_INV_SET_GET_TYPE )
        {
            return SMR_INV_GET_INFO;
        }

        i4_ret = _scc_comp_disp_get_num_drv(
            h_disp,
            DRVT_PLA_MXR,
            &ui2_num_drv );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }

        ui2_idx = 0;
        while( ui2_idx < ui2_num_drv &&
               ui2_idx < SCC_COMP_MAX_NUM_DRV_COMPS )
        {
            t_drv_min_max.e_get_type = e_drv_get_type;
            z_size = sizeof(PLA_MXR_MIN_MAX_INFO_T);
            i4_ret = _scc_comp_disp_get_ex(
                h_disp,
                DRVT_PLA_MXR,
                ui2_idx,
                PLA_MXR_GET_TYPE_MIN_MAX,
                (VOID*)&t_drv_min_max,
                &z_size );
            if ( i4_ret != SMR_OK )
            {
                return i4_ret;
            }
            if ( t_drv_min_max.ui4_min_value >= t_drv_min_max.ui4_max_value )
            {
                return SMR_DRV_COMP_ERROR;
            }

            /* Conver SCC range (0~1000) to driver range (min~max) */
            ui2_drv_value = _ceil_16bits_scc_val_2_drv_val(
                SCC_ATTRIB_VALUE_MAX,
                ui2_value,
                (UINT16)t_drv_min_max.ui4_min_value,
                (UINT16)t_drv_min_max.ui4_max_value );

            i4_ret = _scc_comp_disp_set_ex(
                h_disp,
                DRVT_PLA_MXR,
                ui2_idx,
                e_drv_set_type,
                (VOID*)(UINT32)ui2_drv_value,
                sizeof(UINT16) );
            if ( i4_ret != SMR_OK )
            {
                return SMR_INV_GET_INFO;
            }

            ui2_idx++;
        }
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_set_adp_back_light_rng
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *          pt_range            References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_set_adp_back_light_rng(
    HANDLE_T                        h_disp,
    const SCC_DISP_RANGE_INFO_T*    pt_range )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    UINT16                       ui2_idx, ui2_num_drv;
    PLA_MXR_MIN_MAX_INFO_T       t_drv_min_max = { 0, 0, 0 };
    PLA_MXR_RANGE_INFO_T         t_drv_range = { 0, 0 };

    if ( !pt_range ||
         pt_range->ui4_min_value >= pt_range->ui4_max_value )
    {
        return SMR_INV_ARG;
    }

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tui2_min          = %u\r\n"
                       "\t\tui2_max          = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)pt_range->ui4_min_value,
                       (UINT32)pt_range->ui4_max_value ));

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = _scc_comp_disp_get_num_drv(
        h_disp,
        DRVT_PLA_MXR,
        &ui2_num_drv );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    ui2_idx = 0;
    while( ui2_idx < ui2_num_drv &&
           ui2_idx < SCC_COMP_MAX_NUM_DRV_COMPS )
    {
        t_drv_min_max.e_get_type = PLA_MXR_GET_TYPE_BACK_LIGHT;
        z_size = sizeof(PLA_MXR_MIN_MAX_INFO_T);
        i4_ret = _scc_comp_disp_get_ex(
            h_disp,
            DRVT_PLA_MXR,
            ui2_idx,
            PLA_MXR_GET_TYPE_MIN_MAX,
            (VOID*)&t_drv_min_max,
            &z_size );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
        if ( t_drv_min_max.ui4_min_value >= t_drv_min_max.ui4_max_value )
        {
            return SMR_DRV_COMP_ERROR;
        }

        /* Convert SCC range (0~1000) to driver range (min~max) */
        t_drv_range.ui4_min_value = _ceil_16bits_scc_val_2_drv_val(
            SCC_ATTRIB_VALUE_MAX,
            (UINT16)pt_range->ui4_min_value,
            (UINT16)t_drv_min_max.ui4_min_value,
            (UINT16)t_drv_min_max.ui4_max_value );
        t_drv_range.ui4_max_value = _ceil_16bits_scc_val_2_drv_val(
            SCC_ATTRIB_VALUE_MAX,
            (UINT16)pt_range->ui4_max_value,
            (UINT16)t_drv_min_max.ui4_min_value,
            (UINT16)t_drv_min_max.ui4_max_value );


        i4_ret = _scc_comp_disp_set_ex(
            h_disp,
            DRVT_PLA_MXR,
            ui2_idx,
            PLA_MXR_SET_TYPE_ADP_BACK_LIGHT_RNG,
            (VOID*)&t_drv_range,
            sizeof(PLA_MXR_RANGE_INFO_T) );
        if ( i4_ret != SMR_OK )
        {
            return SMR_INV_GET_INFO;
        }

        ui2_idx++;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_set_resolution
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *          pt_resolution       References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INV_ARG         Fail.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_set_resolution(
    HANDLE_T                       h_disp,
    const SCC_DISP_RESOLUTION_T*   pt_resolution )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    PLA_MXR_RESOLUTION_INFO_T    t_drv_pmx_rsltn = {0};

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpt_resolution    = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)pt_resolution ));

    if ( !pt_resolution )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_pmx_rsltn.ui2_width = pt_resolution->ui2_width;
    t_drv_pmx_rsltn.ui2_height = pt_resolution->ui2_height;
    t_drv_pmx_rsltn.b_is_progressive = pt_resolution->b_is_progressive;
    t_drv_pmx_rsltn.ui2_curr_v_clk = pt_resolution->ui2_curr_v_clk;
    t_drv_pmx_rsltn.ui2_v_clk_max = pt_resolution->ui2_v_clk_max;
    t_drv_pmx_rsltn.ui2_v_clk_min = pt_resolution->ui2_v_clk_min;

    i4_ret = _scc_comp_disp_set(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_SET_TYPE_RESOLUTION,
        (VOID*)&t_drv_pmx_rsltn,
        sizeof(PLA_MXR_RESOLUTION_INFO_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_set_frame_rate
 *
 * Description: <function description>
 *
 * Inputs:  h_disp                   References.
 *          ui4_frame_rate           References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                  Success.
 *          SMR_INTERNAL_ERROR      Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_set_frame_rate( 
    HANDLE_T                    h_disp,
    UINT32                      ui4_frame_rate )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    UINT32                       ui4_drv_frame_rate;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tui4_frame_rate   = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)ui4_frame_rate ));

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_disp_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    ui4_drv_frame_rate = ui4_frame_rate;
    i4_ret = _scc_comp_disp_set(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_SET_TYPE_FRAMERATE,
        (VOID*)(UINT32)ui4_drv_frame_rate,
        sizeof(UINT32) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_get_frame_rate
 *
 * Description: <function description>
 *
 * Inputs:  h_disp                   References.
 *          pui4_frame_rate          References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                  Success.
 *          SMR_INTERNAL_ERROR      Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_get_frame_rate( 
    HANDLE_T                    h_disp,
    UINT32*                     pui4_frame_rate )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    UINT32                       ui4_drv_frame_rate;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpui4_frame_rate  = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)pui4_frame_rate ));

    if ( pui4_frame_rate == NULL )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_disp_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(UINT32);
    i4_ret = _scc_comp_disp_get(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_GET_TYPE_FRAMERATE,
        (VOID*)&ui4_drv_frame_rate,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    *pui4_frame_rate = ui4_drv_frame_rate;
 
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_set_force_frame_rate
 *
 * Description: <function description>
 *
 * Inputs:  h_disp                 References.
 *          ui4_force_fr           References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                  Success.
 *          SMR_INTERNAL_ERROR      Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_set_force_frame_rate( 
    HANDLE_T                    h_disp,
    UINT32                      ui4_force_fr )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    UINT32                       ui4_drv_force_fr;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tui4_force_fr     = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)ui4_force_fr ));

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_disp_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    ui4_drv_force_fr = ui4_force_fr;
    i4_ret = _scc_comp_disp_set(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_SET_TYPE_FORCE_FRAMERATE,
        (VOID*)(UINT32)ui4_drv_force_fr,
        sizeof(UINT32) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_set_aspect_ratio
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *          e_aspect_ratio      References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INV_ARG         Fail.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_set_aspect_ratio(
    HANDLE_T                h_disp,
    SCC_DISP_ASPECT_RATIO_T e_aspect_ratio )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    PLA_MXR_ASPECT_RATIO_T       e_drv_aspect_ratio = PLA_MXR_ASPECT_RATIO_4_3;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\te_aspect_ratio   = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)e_aspect_ratio ));

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_aspect_ratio )
    {
    case SCC_DISP_ASPECT_RATIO_4_3:
        e_drv_aspect_ratio = PLA_MXR_ASPECT_RATIO_4_3;
        break;
    case SCC_DISP_ASPECT_RATIO_16_9:
        e_drv_aspect_ratio = PLA_MXR_ASPECT_RATIO_16_9;
        break;
    case SCC_DISP_ASPECT_RATIO_2_21_1:
        e_drv_aspect_ratio = PLA_MXR_ASPECT_RATIO_2_21_1;
        break;
    case SCC_DISP_ASPECT_RATIO_UNKNOWN:
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_disp_set(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_SET_TYPE_ASPECT_RATIO,
        (VOID*)(UINT32)e_drv_aspect_ratio,
        sizeof(PLA_MXR_ASPECT_RATIO_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_set_fmt
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *          e_fmt               References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_set_fmt( HANDLE_T                h_disp,
                          SCC_DISP_FMT_T          e_fmt )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    TV_ENC_FMT_T                 e_drv_tve_fmt = TV_ENC_FMT_NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\te_fmt            = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (INT32)e_fmt ));

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_fmt )
    {
    case SCC_DISP_FMT_NULL:
        e_drv_tve_fmt = TV_ENC_FMT_NULL;
        break;
    case SCC_DISP_FMT_CVBS:
        e_drv_tve_fmt = TV_ENC_FMT_CVBS;
        break;
    case SCC_DISP_FMT_S_VIDEO:
        e_drv_tve_fmt = TV_ENC_FMT_S_VIDEO;
        break;
    case SCC_DISP_FMT_YBR:
        e_drv_tve_fmt = TV_ENC_FMT_YBR;
        break;
    case SCC_DISP_FMT_RGB:
        e_drv_tve_fmt = TV_ENC_FMT_RGB;
        break;
    default:
        break;
    }

    i4_ret = _scc_comp_disp_set(
        h_disp,
        DRVT_TV_ENC,
        TV_ENC_SET_TYPE_FMT,
        (VOID*)(UINT32)e_drv_tve_fmt,
        sizeof(TV_ENC_FMT_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_set_digit_output
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *          ui4_digit_out_mask  References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_set_digit_output(
    HANDLE_T                        h_disp,
    SCC_DISP_DIGIT_OUT_MASK_T       ui4_digit_out_mask )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    UINT32                       ui4_drv_digit_out_mask = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp             = 0x%.8X\r\n"
                       "\t\tui4_digit_out_mask = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (INT32)ui4_digit_out_mask ));

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    if ( ui4_digit_out_mask & SCC_DISP_DIGIT_OUT_DIGITAL )
    {
        ui4_drv_digit_out_mask |= PLA_MXR_OUTPUT_FLAG_DIGITAL;
    }
    if ( ui4_digit_out_mask & SCC_DISP_DIGIT_OUT_LVDS )
    {
        ui4_drv_digit_out_mask |= PLA_MXR_OUTPUT_FLAG_LVDS;
    }
    if ( ui4_digit_out_mask & SCC_DISP_DIGIT_OUT_CRT )
    {
        ui4_drv_digit_out_mask |= PLA_MXR_OUTPUT_FLAG_CRT;
    }

    i4_ret = _scc_comp_disp_set(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_SET_TYPE_OUTPUT_PORT,
        (VOID*)ui4_drv_digit_out_mask,
        sizeof(UINT32) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_set_tv_type
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *          e_tv_type           References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_set_tv_type( HANDLE_T            h_disp,
                              SCC_DISP_TV_TYPE_T  e_tv_type )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    TV_ENC_TV_TYPE_INFO_T        e_drv_tv_type = TV_ENC_TV_TYPE_NTSC_M;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\te_tv_type        = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (INT32)e_tv_type ));

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_tv_type )
    {
    case SCC_DISP_TV_TYPE_NTSC_M:
        e_drv_tv_type = TV_ENC_TV_TYPE_NTSC_M;
        break;
    case SCC_DISP_TV_TYPE_PAL_B:
        e_drv_tv_type = TV_ENC_TV_TYPE_PAL_B;
        break;
    case SCC_DISP_TV_TYPE_PAL_G:
        e_drv_tv_type = TV_ENC_TV_TYPE_PAL_G;
        break;
    case SCC_DISP_TV_TYPE_PAL_H:
        e_drv_tv_type = TV_ENC_TV_TYPE_PAL_H;
        break;
    case SCC_DISP_TV_TYPE_PAL_I:
        e_drv_tv_type = TV_ENC_TV_TYPE_PAL_I;
        break;
    case SCC_DISP_TV_TYPE_PAL_D:
        e_drv_tv_type = TV_ENC_TV_TYPE_PAL_D;
        break;
    case SCC_DISP_TV_TYPE_PAL_N:
        e_drv_tv_type = TV_ENC_TV_TYPE_PAL_N;
        break;
    case SCC_DISP_TV_TYPE_PAL_M:
        e_drv_tv_type = TV_ENC_TV_TYPE_PAL_M;
        break;
    case SCC_DISP_TV_TYPE_SECAM_B:
        e_drv_tv_type = TV_ENC_TV_TYPE_SECAM_B;
        break;
    case SCC_DISP_TV_TYPE_SECAM_G:
        e_drv_tv_type = TV_ENC_TV_TYPE_SECAM_G;
        break;
    case SCC_DISP_TV_TYPE_SECAM_H:
        e_drv_tv_type = TV_ENC_TV_TYPE_SECAM_H;
        break;
    case SCC_DISP_TV_TYPE_SECAM_D:
        e_drv_tv_type = TV_ENC_TV_TYPE_SECAM_D;
        break;
    case SCC_DISP_TV_TYPE_SECAM_K:
        e_drv_tv_type = TV_ENC_TV_TYPE_SECAM_K;
        break;
    case SCC_DISP_TV_TYPE_SECAM_K1:
        e_drv_tv_type = TV_ENC_TV_TYPE_SECAM_K1;
        break;
    case SCC_DISP_TV_TYPE_SECAM_L:
        e_drv_tv_type = TV_ENC_TV_TYPE_SECAM_L;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_disp_set(
        h_disp,
        DRVT_TV_ENC,
        TV_ENC_SET_TYPE_TV_TYPE,
        (VOID*)(UINT32)e_drv_tv_type,
        sizeof(TV_ENC_TV_TYPE_INFO_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_set_bg
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *          pt_bg               References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_set_bg( HANDLE_T                 h_disp,
                         const SCC_BG_COLOR_T*    pt_bg )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    PLA_MXR_BG_COLOR_T           t_drv_bg_color = {0};

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpt_bg            = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)pt_bg ));

    if ( !pt_bg )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_bg_color.ui1_r = pt_bg->ui1_red;
    t_drv_bg_color.ui1_g = pt_bg->ui1_green;
    t_drv_bg_color.ui1_b = pt_bg->ui1_blue;

    i4_ret = _scc_comp_disp_set(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_SET_TYPE_BG,
        (VOID*)&t_drv_bg_color,
        sizeof(PLA_MXR_BG_COLOR_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_set_digit_output_fmt
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *          e_digit_out_fmt     References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_set_digit_output_fmt(
    HANDLE_T                        h_disp,
    SCC_DISP_DIGIT_OUTPUT_FMT_T     e_digit_out_fmt )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    PLA_MXR_DIGITAL_OUTPUT_FMT_T e_drv_dgtl_out_fmt = PLA_MXR_DIGITAL_OUTPUT_FMT_YBR;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\te_digit_out_fmt  = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (INT32)e_digit_out_fmt ));

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_digit_out_fmt )
    {
    case SCC_DISP_DIGIT_OUTPUT_FMT_YBR:
        e_drv_dgtl_out_fmt = PLA_MXR_DIGITAL_OUTPUT_FMT_YBR;
        break;
    case SCC_DISP_DIGIT_OUTPUT_FMT_RGB:
        e_drv_dgtl_out_fmt = PLA_MXR_DIGITAL_OUTPUT_FMT_RGB;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_disp_set(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_SET_TYPE_DIGITAL_OUTPUT_FMT,
        (VOID*)(UINT32)e_drv_dgtl_out_fmt,
        sizeof(PLA_MXR_DIGITAL_OUTPUT_FMT_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_get_color_gain
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *          pt_color_gain       References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_set_color_gain(
    HANDLE_T                        h_disp,
    const SCC_DISP_COLOR_GAIN_T*    pt_color_gain )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    PLA_MXR_COLOR_GAIN_T         t_drv_color_gain = { TRUE, 0, 0, 0 };
    PLA_MXR_COLOR_GAIN_T         t_drv_cg_min = {0};
    PLA_MXR_COLOR_GAIN_T         t_drv_cg_max = {0};

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpt_color_gain    = 0x%.8x )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)pt_color_gain ));

    if ( !pt_color_gain )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    if ( pt_color_gain->ui2_r_gain > SCC_ATTRIB_VALUE_MAX ||
         pt_color_gain->ui2_g_gain > SCC_ATTRIB_VALUE_MAX ||
         pt_color_gain->ui2_b_gain > SCC_ATTRIB_VALUE_MAX )
    {
        return SMR_INV_SET_INFO;
    }

    z_size = sizeof(PLA_MXR_COLOR_GAIN_T);
    i4_ret = _scc_comp_disp_get(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_GET_TYPE_COLOR_GAIN_MAX,
        (VOID*)&t_drv_cg_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    z_size = sizeof(PLA_MXR_COLOR_GAIN_T);
    i4_ret = _scc_comp_disp_get(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_GET_TYPE_COLOR_GAIN_MIN,
        (VOID*)&t_drv_cg_min,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    if ( t_drv_cg_max.ui2_r_gain < t_drv_cg_min.ui2_r_gain ||
         t_drv_cg_max.ui2_g_gain < t_drv_cg_min.ui2_g_gain ||
         t_drv_cg_max.ui2_b_gain < t_drv_cg_min.ui2_b_gain )
    {
        return SMR_DRV_COMP_ERROR;
    }

    t_drv_color_gain.ui2_r_gain = _ceil_16bits_scc_val_2_drv_val(
        SCC_ATTRIB_VALUE_MAX,
        pt_color_gain->ui2_r_gain,
        t_drv_cg_min.ui2_r_gain,
        t_drv_cg_max.ui2_r_gain );

    t_drv_color_gain.ui2_g_gain = _ceil_16bits_scc_val_2_drv_val(
        SCC_ATTRIB_VALUE_MAX,
        pt_color_gain->ui2_g_gain,
        t_drv_cg_min.ui2_g_gain,
        t_drv_cg_max.ui2_g_gain );

    t_drv_color_gain.ui2_b_gain = _ceil_16bits_scc_val_2_drv_val(
        SCC_ATTRIB_VALUE_MAX,
        pt_color_gain->ui2_b_gain,
        t_drv_cg_min.ui2_b_gain,
        t_drv_cg_max.ui2_b_gain );

    i4_ret = _scc_comp_disp_set(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_SET_TYPE_COLOR_GAIN,
        (VOID*)&t_drv_color_gain,
        sizeof(PLA_MXR_COLOR_GAIN_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_get_color_offset
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *          pt_color_offset       References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_set_color_offset(
    HANDLE_T                        h_disp,
    const SCC_DISP_COLOR_GAIN_T*    pt_color_offset )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    PLA_MXR_COLOR_GAIN_T         t_drv_color_offset = { FALSE, 0, 0, 0 };
    PLA_MXR_COLOR_GAIN_T         t_drv_co_min = {0};
    PLA_MXR_COLOR_GAIN_T         t_drv_co_max = {0};

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpt_color_offset    = 0x%.8x )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)pt_color_offset ));

    if ( !pt_color_offset )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    if ( pt_color_offset->ui2_r_gain > SCC_ATTRIB_VALUE_MAX ||
         pt_color_offset->ui2_g_gain > SCC_ATTRIB_VALUE_MAX ||
         pt_color_offset->ui2_b_gain > SCC_ATTRIB_VALUE_MAX )
    {
        return SMR_INV_SET_INFO;
    }

    z_size = sizeof(SCC_DISP_COLOR_GAIN_T);
    i4_ret = _scc_comp_disp_get(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_GET_TYPE_COLOR_OFFSET_MAX,
        (VOID*)&t_drv_co_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    z_size = sizeof(SCC_DISP_COLOR_GAIN_T);
    i4_ret = _scc_comp_disp_get(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_GET_TYPE_COLOR_OFFSET_MIN,
        (VOID*)&t_drv_co_min,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    if ( t_drv_co_max.ui2_r_gain < t_drv_co_min.ui2_r_gain ||
         t_drv_co_max.ui2_g_gain < t_drv_co_min.ui2_g_gain ||
         t_drv_co_max.ui2_b_gain < t_drv_co_min.ui2_b_gain )
    {
        return SMR_DRV_COMP_ERROR;
    }

    t_drv_color_offset.ui2_r_gain = _ceil_16bits_scc_val_2_drv_val(
        SCC_ATTRIB_VALUE_MAX,
        pt_color_offset->ui2_r_gain,
        t_drv_co_min.ui2_r_gain,
        t_drv_co_max.ui2_r_gain );

    t_drv_color_offset.ui2_g_gain = _ceil_16bits_scc_val_2_drv_val(
        SCC_ATTRIB_VALUE_MAX,
        pt_color_offset->ui2_g_gain,
        t_drv_co_min.ui2_g_gain,
        t_drv_co_max.ui2_g_gain );

    t_drv_color_offset.ui2_b_gain = _ceil_16bits_scc_val_2_drv_val(
        SCC_ATTRIB_VALUE_MAX,
        pt_color_offset->ui2_b_gain,
        t_drv_co_min.ui2_b_gain,
        t_drv_co_max.ui2_b_gain );

    i4_ret = _scc_comp_disp_set(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_SET_TYPE_COLOR_OFFSET,
        (VOID*)&t_drv_color_offset,
        sizeof(PLA_MXR_COLOR_GAIN_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_set_tv_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: e_tv_mode           References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_set_tv_mode(
    HANDLE_T                        h_disp,
    SCC_TV_MODE_T                   e_tv_mode )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    PLA_MXR_TV_MODE_T            e_drv_tv_mode = PLA_MXR_TV_MODE_NORMAL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\te_tv_mode        = 0x%.8x )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)e_tv_mode ));

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_tv_mode )
    {
    case SCC_TV_MODE_NORMAL:
        e_drv_tv_mode = PLA_MXR_TV_MODE_NORMAL;
        break;

    case SCC_TV_MODE_PIP:
        e_drv_tv_mode = PLA_MXR_TV_MODE_PIP;
        break;

    case SCC_TV_MODE_POP:
        e_drv_tv_mode = PLA_MXR_TV_MODE_POP;
        break;

    default:
        return SMR_INV_ARG;
    }

    i4_ret = _scc_comp_disp_set(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_SET_TYPE_TV_MODE,
        (VOID*)e_drv_tv_mode,
        sizeof(PLA_MXR_TV_MODE_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_set_mjc
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: b_mjc               References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_set_mjc(
    HANDLE_T                        h_disp,
    SCC_DISP_MJC_T                  e_mjc )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    PLA_MXR_MJC_T                e_drv_mjc = PLA_MXR_MJC_OFF;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\te_mjc            = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)e_mjc ));

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_mjc )
    {
    case SCC_DISP_MJC_OFF:
        e_drv_mjc = PLA_MXR_MJC_OFF;
        break;

    case SCC_DISP_MJC_ON:
        e_drv_mjc = PLA_MXR_MJC_ON;
        break;

    case SCC_DISP_MJC_DNM:
        e_drv_mjc = PLA_MXR_MJC_DNM;
        break;

    default:
        return SMR_INV_ARG;
    }

    i4_ret = _scc_comp_disp_set(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_SET_TYPE_MJC,
        (VOID*)(UINT32)e_drv_mjc,
        sizeof(PLA_MXR_MJC_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_set_test_pattern
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: b_mjc               References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_set_test_pattern(
    HANDLE_T                        h_disp,
    SCC_DISP_TEST_PATTERN_T         e_pattern )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    PLA_MXR_TEST_PATTERN_T       e_drv_pattern = PLA_MXR_TEST_PATTERN_DISABLE;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\te_pattern        = 0x%.8x )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)e_pattern ));

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_pattern )
    {
    case SCC_DISP_TEST_PATTERN_DISABLE:
        e_drv_pattern = PLA_MXR_TEST_PATTERN_DISABLE;
        break;

    case SCC_DISP_TEST_PATTERN_1:
        e_drv_pattern = PLA_MXR_TEST_PATTERN_1;
        break;

    case SCC_DISP_TEST_PATTERN_2:
        e_drv_pattern = PLA_MXR_TEST_PATTERN_2;
        break;

    case SCC_DISP_TEST_PATTERN_3:
        e_drv_pattern = PLA_MXR_TEST_PATTERN_3;
        break;

    case SCC_DISP_TEST_PATTERN_4:
        e_drv_pattern = PLA_MXR_TEST_PATTERN_4;
        break;

    case SCC_DISP_TEST_PATTERN_5:
        e_drv_pattern = PLA_MXR_TEST_PATTERN_5;
        break;

    case SCC_DISP_TEST_PATTERN_6:
        e_drv_pattern = PLA_MXR_TEST_PATTERN_6;
        break;

    case SCC_DISP_TEST_PATTERN_7:
        e_drv_pattern = PLA_MXR_TEST_PATTERN_7;
        break;

    case SCC_DISP_TEST_PATTERN_8:
        e_drv_pattern = PLA_MXR_TEST_PATTERN_8;
        break;

    default:
        return SMR_INV_ARG;
    }

    i4_ret = _scc_comp_disp_set(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_SET_TYPE_TEST_PATTERN,
        (VOID*)e_drv_pattern,
        sizeof(PLA_MXR_TEST_PATTERN_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

#ifdef MW_TV_AV_BYPASS_SUPPORT
/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_set_bypass_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_set_bypass_mode(
    HANDLE_T                        h_disp,
    SCC_DISP_BYPASS_MODE_T          e_mode )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    VIDEO_OUT_MODE_T             e_video_out_mode = VIDEO_OUT_MODE_BLANK;
#if 0 /* mark for CR DTV00138842 */
    TV_ENC_CTRL_T                e_drv_tve_ctrl = TV_ENC_CTRL_DISABLE;
#endif

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\te_mode           = 0x%.8x )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)e_mode ));

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_mode )
    {
    case SCC_DISP_BYPASS_NORMAL:
        e_video_out_mode = VIDEO_OUT_MODE_NORMAL;
#if 0 
        e_drv_tve_ctrl = TV_ENC_CTRL_ENABLE;
#endif
        break;

    case SCC_DISP_BYPASS_BLANK:
        e_video_out_mode = VIDEO_OUT_MODE_BLANK;
#if 0 
        e_drv_tve_ctrl = TV_ENC_CTRL_DISABLE;
#endif
        break;

    default:
        return SMR_INV_ARG;
    }

    i4_ret = _scc_comp_disp_set(
        h_disp,
        DRVT_VIDEO_OUT,
        VIDEO_OUT_SET_TYPE_MODE,
        (VOID*)e_video_out_mode,
        sizeof(VIDEO_OUT_MODE_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

#if 0 
    i4_ret = _scc_comp_disp_set(
        h_disp,
        DRVT_TV_ENC,
        TV_ENC_SET_TYPE_CTRL,
        (VOID*)e_drv_tve_ctrl,
        sizeof(TV_ENC_CTRL_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }
#endif    

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_set_tve_not_disable_flag
 *
 * Description: <function description>
 *
 * Inputs:  b_not_disable_tve   
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
VOID x_scc_disp_set_tve_not_disable_flag( 
    BOOL                            b_not_disable_tve_as_stop )
{
    gb_not_disable_tve_as_stop = b_not_disable_tve_as_stop;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_set_tve_enable_flag
 *
 * Description: <function description>
 *
 * Inputs:  b_not_disable_tve   
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
VOID x_scc_disp_set_tve_enable_flag( 
    BOOL                            b_enable_tve_as_play )
{
    gb_enable_tve_as_play = b_enable_tve_as_play;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_set_tve_ctrl_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_set_tve_ctrl_mode(
    HANDLE_T                        h_disp,
    SCC_DISP_TVE_CTRL_MODE_T        e_tve_ctrl_mode )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL; 
    TV_ENC_CTRL_T                e_drv_tve_ctrl = TV_ENC_CTRL_DISABLE;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\te_mode           = 0x%.8x )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)e_tve_ctrl_mode ));

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_tve_ctrl_mode )
    {
    /* SCC_DISP_TVE_CTRL_ENABLE case NOT used now */
    /* if want to use this to enable, we need discuss! */
    case SCC_DISP_TVE_CTRL_ENABLE: 
        e_drv_tve_ctrl = TV_ENC_CTRL_ENABLE;
        break;

    case SCC_DISP_TVE_CTRL_DISABLE: 
        e_drv_tve_ctrl = TV_ENC_CTRL_DISABLE;
        break;

    default:
        return SMR_INV_ARG;
    } 

    i4_ret = _scc_comp_disp_set(
        h_disp,
        DRVT_TV_ENC,
        TV_ENC_SET_TYPE_CTRL,
        (VOID*)e_drv_tve_ctrl,
        sizeof(TV_ENC_CTRL_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

#endif

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_get_max_gamma
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: pui1_max_gamma      References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_get_max_gamma( HANDLE_T          h_disp,
                                UINT8*            pui1_max_gamma )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpui1_max_gamma   = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)pui1_max_gamma ));

    if ( !pui1_max_gamma )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(UINT8);
    i4_ret = _scc_comp_disp_get(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_GET_TYPE_MAX_GAMMA_CURVE,
        (VOID*)pui1_max_gamma,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_get_ctrl
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: pe_ctrl             References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_get_ctrl(
    HANDLE_T                        h_disp,
    SCC_DISP_CTRL_TYPE_T*           pe_ctrl )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    PLA_MXR_CTRL_T               e_drv_pmx_ctrl = PLA_MXR_CTRL_DISABLE;
    TV_ENC_CTRL_T                e_drv_tve_ctrl = TV_ENC_CTRL_DISABLE;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpe_ctrl          = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)pe_ctrl ));

    if ( !pe_ctrl )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof( PLA_MXR_CTRL_T );
    i4_ret = _scc_comp_disp_get(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_GET_TYPE_CTRL,
        (VOID*)&e_drv_pmx_ctrl,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    z_size = sizeof( TV_ENC_CTRL_T );
    i4_ret = _scc_comp_disp_get(
        h_disp,
        DRVT_TV_ENC,
        TV_ENC_GET_TYPE_CTRL,
        (VOID*)&e_drv_tve_ctrl,
        &z_size );
    if ( i4_ret == SMR_DRV_COMP_NOT_FOUND )
    {
        e_drv_tve_ctrl = TV_ENC_CTRL_ENABLE;
    }
    else if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    if ( e_drv_pmx_ctrl == PLA_MXR_CTRL_ENABLE &&
         e_drv_tve_ctrl == TV_ENC_CTRL_ENABLE )
    {
        *pe_ctrl = SCC_DISP_CTRL_ENABLE;
    }
    else
    {
        *pe_ctrl = SCC_DISP_CTRL_DISABLE;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_get_attrib
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *          e_attrib            References.
 *
 * Outputs: pui2_value          References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_get_attrib( HANDLE_T             h_disp,
                             SCC_DISP_ATTRIB_T    e_attrib,
                             UINT16*              pui2_value )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    UINT32                       e_drv_get_type;
    UINT16                       ui2_drv_value;
    PLA_MXR_MIN_MAX_INFO_T       t_drv_min_max = { 0, 0, 0 };

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\te_attrib         = %d\r\n"
                       "\t\tpui2_value       = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (INT32)e_attrib,
                       (UINT32)pui2_value ));

    if ( !pui2_value )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    if ( e_attrib == SCC_DISP_ATTRIB_GAMMA )
    {
        UINT8             ui1_gamma = 0;

        z_size = sizeof( UINT8 );
        i4_ret = _scc_comp_disp_get(
            h_disp,
            DRVT_PLA_MXR,
            PLA_MXR_GET_TYPE_GAMMA,
            (VOID*)&ui1_gamma,
            &z_size );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }

        *pui2_value = (UINT16)ui1_gamma;
    }
    else if ( e_attrib == SCC_DISP_ATTRIB_DITHER )
    {
        UINT8    ui1_dither;

        z_size = sizeof( UINT8 );
        i4_ret = _scc_comp_disp_get(
            h_disp,
            DRVT_PLA_MXR,
            PLA_MXR_GET_TYPE_DITHER,
            (VOID*)&ui1_dither,
            &z_size );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }

        *pui2_value = (UINT16)ui1_dither;
    }
    else
    {
        e_drv_get_type = _scc_attrib_2_drv_pla_mxr_get_type( e_attrib );
        if ( e_drv_get_type == SCC_INV_SET_GET_TYPE )
        {
            return SMR_INV_GET_INFO;
        }

        t_drv_min_max.e_get_type = e_drv_get_type;
        z_size = sizeof(PLA_MXR_MIN_MAX_INFO_T);
        i4_ret = _scc_comp_disp_get_ex(
            h_disp,
            DRVT_PLA_MXR,
            0,
            PLA_MXR_GET_TYPE_MIN_MAX,
            (VOID*)&t_drv_min_max,
            &z_size );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }

        z_size = sizeof(UINT16);
        i4_ret = _scc_comp_disp_get_ex(
            h_disp,
            DRVT_PLA_MXR,
            0,
            e_drv_get_type,
            (VOID*)&ui2_drv_value,
            &z_size );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }

        if ( t_drv_min_max.ui4_min_value >= t_drv_min_max.ui4_max_value ||
             (UINT32)ui2_drv_value > t_drv_min_max.ui4_max_value ||
             (UINT32)ui2_drv_value < t_drv_min_max.ui4_min_value )
        {
            return SMR_DRV_COMP_ERROR;
        }

        /* Conver driver range (min~max) to SCC range (0~1000) */
        *pui2_value = _floor_16bits_drv_val_2_scc_val(
            SCC_ATTRIB_VALUE_MAX,
            ui2_drv_value,
            (UINT16)t_drv_min_max.ui4_min_value,
            (UINT16)t_drv_min_max.ui4_max_value );
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_get_adp_back_light_rng
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: pt_range            References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INV_ARG         Fail.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_get_adp_back_light_rng(
    HANDLE_T                       h_disp,
    SCC_DISP_RANGE_INFO_T*         pt_range )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    PLA_MXR_MIN_MAX_INFO_T       t_drv_min_max = { 0, 0, 0 };
    PLA_MXR_RANGE_INFO_T         t_drv_range = { 0, 0 };

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpt_range         = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)pt_range ));

    if ( !pt_range )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_min_max.e_get_type = PLA_MXR_GET_TYPE_BACK_LIGHT;
    z_size = sizeof(PLA_MXR_MIN_MAX_INFO_T);
    i4_ret = _scc_comp_disp_get_ex(
        h_disp,
        DRVT_PLA_MXR,
        0,
        PLA_MXR_GET_TYPE_MIN_MAX,
        (VOID*)&t_drv_min_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    z_size = sizeof(PLA_MXR_RANGE_INFO_T);
    i4_ret = _scc_comp_disp_get_ex(
        h_disp,
        DRVT_PLA_MXR,
        0,
        PLA_MXR_GET_TYPE_ADP_BACK_LIGHT_RNG,
        (VOID*)&t_drv_range,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    if ( t_drv_range.ui4_min_value >= t_drv_range.ui4_max_value ||
         t_drv_range.ui4_max_value > t_drv_min_max.ui4_max_value ||
         t_drv_range.ui4_min_value < t_drv_min_max.ui4_min_value )
    {
        return SMR_DRV_COMP_ERROR;
    }

    /* Convert driver range (min~max) to SCC range (0~SCC_ATTRIB_VALUE_MAX) */
    pt_range->ui4_min_value = _floor_16bits_drv_val_2_scc_val(
        SCC_ATTRIB_VALUE_MAX,
        (UINT16)t_drv_range.ui4_min_value,
        (UINT16)t_drv_min_max.ui4_min_value,
        (UINT16)t_drv_min_max.ui4_max_value );
    pt_range->ui4_max_value = _floor_16bits_drv_val_2_scc_val(
        SCC_ATTRIB_VALUE_MAX,
        (UINT16)t_drv_range.ui4_max_value,
        (UINT16)t_drv_min_max.ui4_min_value,
        (UINT16)t_drv_min_max.ui4_max_value );

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_get_resolution
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: pt_resolution       References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INV_ARG         Fail.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_get_resolution(
    HANDLE_T                       h_disp,
    SCC_DISP_RESOLUTION_T*         pt_resolution )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    PLA_MXR_RESOLUTION_INFO_T    t_pmxr_resolution = {0};

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpt_resolution    = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)pt_resolution ));

    if ( !pt_resolution )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(PLA_MXR_RESOLUTION_INFO_T);
    i4_ret = _scc_comp_disp_get(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_GET_TYPE_RESOLUTION,
        (VOID*)&t_pmxr_resolution,
        &z_size );
    if ( i4_ret != RMR_OK )
    {
        return i4_ret;
    }

    pt_resolution->ui2_width        = t_pmxr_resolution.ui2_width;
    pt_resolution->ui2_height       = t_pmxr_resolution.ui2_height;
    pt_resolution->b_is_progressive = t_pmxr_resolution.b_is_progressive;
    pt_resolution->ui2_curr_v_clk   = t_pmxr_resolution.ui2_curr_v_clk;
    pt_resolution->ui2_v_clk_max    = t_pmxr_resolution.ui2_v_clk_max;
    pt_resolution->ui2_v_clk_min    = t_pmxr_resolution.ui2_v_clk_min;
    pt_resolution->ui2_panel_width  = t_pmxr_resolution.ui2_panel_width;
    pt_resolution->ui2_panel_height = t_pmxr_resolution.ui2_panel_height;

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_get_aspect_ratio
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: pe_aspect_ratio     References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INV_ARG         Fail.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_get_aspect_ratio(
    HANDLE_T                 h_disp,
    SCC_DISP_ASPECT_RATIO_T* pe_aspect_ratio )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    PLA_MXR_ASPECT_RATIO_T       e_aspect_ratio = PLA_MXR_ASPECT_RATIO_UNKNOWN;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpe_aspect_ratio   = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)pe_aspect_ratio ));

    if ( !pe_aspect_ratio )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(PLA_MXR_ASPECT_RATIO_T);
    i4_ret = _scc_comp_disp_get(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_GET_TYPE_ASPECT_RATIO,
        (VOID*)&e_aspect_ratio,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    if ( e_aspect_ratio == PLA_MXR_ASPECT_RATIO_4_3 )
    {
        *pe_aspect_ratio = SCC_DISP_ASPECT_RATIO_4_3;
    }
    else if ( e_aspect_ratio == PLA_MXR_ASPECT_RATIO_16_9 )
    {
        *pe_aspect_ratio = SCC_DISP_ASPECT_RATIO_16_9;
    }
    else if ( e_aspect_ratio == PLA_MXR_ASPECT_RATIO_2_21_1 )
    {
        *pe_aspect_ratio = SCC_DISP_ASPECT_RATIO_2_21_1;
    }
    else
    {
        *pe_aspect_ratio = SCC_DISP_ASPECT_RATIO_UNKNOWN;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_get_fmt
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: pe_fmt              References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_get_fmt( HANDLE_T                h_disp,
                          SCC_DISP_FMT_T*         pe_fmt )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    TV_ENC_FMT_T                 e_drv_fmt = TV_ENC_FMT_NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpe_fmt           = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)pe_fmt ));

    if ( !pe_fmt )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(TV_ENC_FMT_T);
    i4_ret = _scc_comp_disp_get(
        h_disp,
        DRVT_TV_ENC,
        TV_ENC_GET_TYPE_FMT,
        (VOID*)&e_drv_fmt,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    switch( e_drv_fmt )
    {
    case TV_ENC_FMT_NULL:
        *pe_fmt = SCC_DISP_FMT_NULL;
        break;
    case TV_ENC_FMT_CVBS:
        *pe_fmt = SCC_DISP_FMT_CVBS;
        break;
    case TV_ENC_FMT_S_VIDEO:
        *pe_fmt = SCC_DISP_FMT_S_VIDEO;
        break;
    case TV_ENC_FMT_YBR:
        *pe_fmt = SCC_DISP_FMT_YBR;
        break;
    case TV_ENC_FMT_RGB:
        *pe_fmt = SCC_DISP_FMT_RGB;
        break;
    default:
        return SMR_DRV_COMP_ERROR;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_get_digit_output
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: pui4_digit_out_mask References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_get_digit_output(
    HANDLE_T                        h_disp,
    SCC_DISP_DIGIT_OUT_MASK_T*      pui4_digit_out_mask )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    UINT32                       ui4_drv_digit_out_mask = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp               = 0x%.8X\r\n"
                       "\t\tpui4_digit_out_mask  = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)pui4_digit_out_mask ));

    if ( !pui4_digit_out_mask )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(UINT32);
    i4_ret = _scc_comp_disp_get(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_GET_TYPE_OUTPUT_PORT,
        (VOID*)&ui4_drv_digit_out_mask,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    *pui4_digit_out_mask = SCC_DISP_DIGIT_OUT_NONE;
    if ( ui4_drv_digit_out_mask & PLA_MXR_OUTPUT_FLAG_DIGITAL )
    {
        *pui4_digit_out_mask &= SCC_DISP_DIGIT_OUT_DIGITAL;
    }
    if ( ui4_drv_digit_out_mask & PLA_MXR_OUTPUT_FLAG_LVDS )
    {
        *pui4_digit_out_mask &= SCC_DISP_DIGIT_OUT_LVDS;
    }
    if ( ui4_drv_digit_out_mask & PLA_MXR_OUTPUT_FLAG_CRT )
    {
        *pui4_digit_out_mask &= SCC_DISP_DIGIT_OUT_CRT;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_get_tv_type
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: pe_tv_type          References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_get_tv_type( HANDLE_T            h_disp,
                              SCC_DISP_TV_TYPE_T* pe_tv_type )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    TV_ENC_TV_TYPE_INFO_T        e_tve_type = TV_ENC_TV_TYPE_NTSC_M;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpe_tv_type       = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)pe_tv_type ));

    if ( !pe_tv_type )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(TV_ENC_TV_TYPE_INFO_T);
    i4_ret = _scc_comp_disp_get(
        h_disp,
        DRVT_TV_ENC,
        TV_ENC_GET_TYPE_TV_TYPE,
        (VOID*)&e_tve_type,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    *pe_tv_type = SCC_DISP_TV_TYPE_NTSC_M;
    switch (e_tve_type)
    {
    case TV_ENC_TV_TYPE_NTSC_M:
        *pe_tv_type = SCC_DISP_TV_TYPE_NTSC_M;
        break;
    case TV_ENC_TV_TYPE_PAL_B:
        *pe_tv_type = SCC_DISP_TV_TYPE_PAL_B;
        break;
    case TV_ENC_TV_TYPE_PAL_G:
        *pe_tv_type = SCC_DISP_TV_TYPE_PAL_G;
        break;
    case TV_ENC_TV_TYPE_PAL_H:
        *pe_tv_type = SCC_DISP_TV_TYPE_PAL_H;
        break;
    case TV_ENC_TV_TYPE_PAL_I:
        *pe_tv_type = SCC_DISP_TV_TYPE_PAL_I;
        break;
    case TV_ENC_TV_TYPE_PAL_D:
        *pe_tv_type = SCC_DISP_TV_TYPE_PAL_D;
        break;
    case TV_ENC_TV_TYPE_PAL_N:
        *pe_tv_type = SCC_DISP_TV_TYPE_PAL_N;
        break;
    case TV_ENC_TV_TYPE_PAL_M:
        *pe_tv_type = SCC_DISP_TV_TYPE_PAL_M;
        break;
    case TV_ENC_TV_TYPE_SECAM_B:
        *pe_tv_type = SCC_DISP_TV_TYPE_SECAM_B;
        break;
    case TV_ENC_TV_TYPE_SECAM_G:
        *pe_tv_type = SCC_DISP_TV_TYPE_SECAM_G;
        break;
    case TV_ENC_TV_TYPE_SECAM_H:
        *pe_tv_type = SCC_DISP_TV_TYPE_SECAM_H;
        break;
    case TV_ENC_TV_TYPE_SECAM_D:
        *pe_tv_type = SCC_DISP_TV_TYPE_SECAM_D;
        break;
    case TV_ENC_TV_TYPE_SECAM_K:
        *pe_tv_type = SCC_DISP_TV_TYPE_SECAM_K;
        break;
    case TV_ENC_TV_TYPE_SECAM_K1:
        *pe_tv_type = SCC_DISP_TV_TYPE_SECAM_K1;
        break;
    case TV_ENC_TV_TYPE_SECAM_L:
        *pe_tv_type = SCC_DISP_TV_TYPE_SECAM_L;
        break;
    default:
        return SMR_DRV_COMP_ERROR;
    }

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_get_bg
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: pt_bg               References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_get_bg( HANDLE_T                 h_disp,
                         SCC_BG_COLOR_T*          pt_bg )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    PLA_MXR_BG_COLOR_T           t_bg_color = {0};

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpt_bg            = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)pt_bg ));

    if ( !pt_bg )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(PLA_MXR_BG_COLOR_T);
    i4_ret = _scc_comp_disp_get(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_GET_TYPE_BG,
        (VOID*)&t_bg_color,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    pt_bg->ui1_red = t_bg_color.ui1_r;
    pt_bg->ui1_green = t_bg_color.ui1_g;
    pt_bg->ui1_blue = t_bg_color.ui1_b;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_get_digit_output_fmt
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_get_digit_output_fmt(
    HANDLE_T                        h_disp,
    SCC_DISP_DIGIT_OUTPUT_FMT_T*    pe_digit_out_fmt )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    PLA_MXR_DIGITAL_OUTPUT_FMT_T e_drv_dgtl_out_fmt = PLA_MXR_DIGITAL_OUTPUT_FMT_RGB;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpe_digit_out_fmt = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)pe_digit_out_fmt ));

    if ( !pe_digit_out_fmt )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(PLA_MXR_DIGITAL_OUTPUT_FMT_T);
    i4_ret = _scc_comp_disp_get(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_GET_TYPE_DIGITAL_OUTPUT_FMT,
        (VOID*)&e_drv_dgtl_out_fmt,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    if ( e_drv_dgtl_out_fmt == PLA_MXR_DIGITAL_OUTPUT_FMT_RGB )
    {
        *pe_digit_out_fmt = SCC_DISP_DIGIT_OUTPUT_FMT_RGB;
    }
    else if ( e_drv_dgtl_out_fmt == PLA_MXR_DIGITAL_OUTPUT_FMT_YBR )
    {
        *pe_digit_out_fmt = SCC_DISP_DIGIT_OUTPUT_FMT_YBR;
    }
    else
    {
        return SMR_DRV_COMP_ERROR;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_get_dither_cap
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: pui1_max_dither      References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_get_dither_cap( HANDLE_T          h_disp,
                                 UINT32*           pui4_dither_cap )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    UINT32                       ui4_dither_cap = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpui4_dither_cap  = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)pui4_dither_cap ));

    if ( !pui4_dither_cap )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(UINT32);
    i4_ret = _scc_comp_disp_get(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_GET_TYPE_DITHER_CAP,
        (VOID*)&ui4_dither_cap,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    *pui4_dither_cap = ui4_dither_cap;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_get_fmt_cap
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: pui4_fmt_cap        References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_get_fmt_cap( HANDLE_T            h_disp,
                              SCC_DISP_FMT_CAP_T* pui4_fmt_cap )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    TV_ENC_FMT_CAP_INFO_T        ui4_drv_fmt_cap;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpui4_fmt_cap     = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)pui4_fmt_cap ));

    if ( !pui4_fmt_cap )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    if ( pt_disp_comp->ah_tv_enc[0] != NULL_HANDLE )
    {
        ui4_drv_fmt_cap = pt_disp_comp->aui4_tv_enc_fmt_cap[0];
    }
    else
    {
        return SMR_DRV_COMP_NOT_FOUND;
    }

    *pui4_fmt_cap = SCC_DISP_FMT_NULL;
    if( ui4_drv_fmt_cap & TV_ENC_FMT_CAP_CVBS )
    {
        *pui4_fmt_cap |= SCC_DISP_FMT_CVBS;
    }
    if( ui4_drv_fmt_cap & TV_ENC_FMT_CAP_S_VIDEO )
    {
        *pui4_fmt_cap |= SCC_DISP_FMT_S_VIDEO;
    }
    if( ui4_drv_fmt_cap & TV_ENC_FMT_CAP_YBR )
    {
        *pui4_fmt_cap |= SCC_DISP_FMT_YBR;
    }
    if( ui4_drv_fmt_cap & TV_ENC_FMT_CAP_RGB )
    {
        *pui4_fmt_cap |= SCC_DISP_FMT_RGB;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_get_color_gain
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: pt_color_gain       References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_get_color_gain(
    HANDLE_T                        h_disp,
    SCC_DISP_COLOR_GAIN_T*          pt_color_gain )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    PLA_MXR_COLOR_GAIN_T         t_drv_color_gain = { TRUE, 0, 0, 0 };
    PLA_MXR_COLOR_GAIN_T         t_drv_cg_min = {0}; 
    PLA_MXR_COLOR_GAIN_T         t_drv_cg_max = {0};

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpt_color_gain    = 0x%.8x )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)pt_color_gain ));

    if ( !pt_color_gain )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(PLA_MXR_COLOR_GAIN_T);
    i4_ret = _scc_comp_disp_get(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_GET_TYPE_COLOR_GAIN_MAX,
        (VOID*)&t_drv_cg_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    z_size = sizeof(PLA_MXR_COLOR_GAIN_T);
    i4_ret = _scc_comp_disp_get(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_GET_TYPE_COLOR_GAIN_MIN,
        (VOID*)&t_drv_cg_min,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    if ( t_drv_cg_max.ui2_r_gain < t_drv_cg_min.ui2_r_gain ||
         t_drv_cg_max.ui2_g_gain < t_drv_cg_min.ui2_g_gain ||
         t_drv_cg_max.ui2_b_gain < t_drv_cg_min.ui2_b_gain )
    {
        return SMR_DRV_COMP_ERROR;
    }

    z_size = sizeof(PLA_MXR_COLOR_GAIN_T);
    i4_ret = _scc_comp_disp_get(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_GET_TYPE_COLOR_GAIN,
        (VOID*)&t_drv_color_gain,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    if ( t_drv_color_gain.ui2_r_gain < t_drv_cg_min.ui2_r_gain ||
         t_drv_color_gain.ui2_g_gain < t_drv_cg_min.ui2_g_gain ||
         t_drv_color_gain.ui2_b_gain < t_drv_cg_min.ui2_b_gain ||
         t_drv_color_gain.ui2_r_gain > t_drv_cg_max.ui2_r_gain ||
         t_drv_color_gain.ui2_g_gain > t_drv_cg_max.ui2_g_gain ||
         t_drv_color_gain.ui2_b_gain > t_drv_cg_max.ui2_b_gain )
    {
        return SMR_DRV_COMP_ERROR;
    }

    pt_color_gain->ui2_r_gain = _floor_16bits_drv_val_2_scc_val(
        SCC_ATTRIB_VALUE_MAX,
        t_drv_color_gain.ui2_r_gain,
        t_drv_cg_min.ui2_r_gain,
        t_drv_cg_max.ui2_r_gain );

    pt_color_gain->ui2_g_gain = _floor_16bits_drv_val_2_scc_val(
        SCC_ATTRIB_VALUE_MAX,
        t_drv_color_gain.ui2_g_gain,
        t_drv_cg_min.ui2_g_gain,
        t_drv_cg_max.ui2_g_gain );

    pt_color_gain->ui2_b_gain = _floor_16bits_drv_val_2_scc_val(
        SCC_ATTRIB_VALUE_MAX,
        t_drv_color_gain.ui2_b_gain,
        t_drv_cg_min.ui2_b_gain,
        t_drv_cg_max.ui2_b_gain );

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_get_color_offset
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: pt_color_offset     References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_get_color_offset(
    HANDLE_T                        h_disp,
    SCC_DISP_COLOR_GAIN_T*          pt_color_offset )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    PLA_MXR_COLOR_GAIN_T         t_drv_color_offset = { TRUE, 0, 0, 0 };
    PLA_MXR_COLOR_GAIN_T         t_drv_co_min = {0};
    PLA_MXR_COLOR_GAIN_T         t_drv_co_max = {0};

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpt_color_offset  = 0x%.8x )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)pt_color_offset ));

    if ( !pt_color_offset )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(PLA_MXR_COLOR_GAIN_T);
    i4_ret = _scc_comp_disp_get(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_GET_TYPE_COLOR_OFFSET_MAX,
        (VOID*)&t_drv_co_max,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    z_size = sizeof(PLA_MXR_COLOR_GAIN_T);
    i4_ret = _scc_comp_disp_get(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_GET_TYPE_COLOR_OFFSET_MIN,
        (VOID*)&t_drv_co_min,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    if ( t_drv_co_max.ui2_r_gain < t_drv_co_min.ui2_r_gain ||
         t_drv_co_max.ui2_g_gain < t_drv_co_min.ui2_g_gain ||
         t_drv_co_max.ui2_b_gain < t_drv_co_min.ui2_b_gain )
    {
        return SMR_DRV_COMP_ERROR;
    }

    z_size = sizeof(PLA_MXR_COLOR_GAIN_T);
    i4_ret = _scc_comp_disp_get(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_GET_TYPE_COLOR_OFFSET,
        (VOID*)&t_drv_color_offset,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    if ( t_drv_color_offset.ui2_r_gain < t_drv_co_min.ui2_r_gain ||
         t_drv_color_offset.ui2_g_gain < t_drv_co_min.ui2_g_gain ||
         t_drv_color_offset.ui2_b_gain < t_drv_co_min.ui2_b_gain ||
         t_drv_color_offset.ui2_r_gain > t_drv_co_max.ui2_r_gain ||
         t_drv_color_offset.ui2_g_gain > t_drv_co_max.ui2_g_gain ||
         t_drv_color_offset.ui2_b_gain > t_drv_co_max.ui2_b_gain )
    {
        return SMR_DRV_COMP_ERROR;
    }

    pt_color_offset->ui2_r_gain = _floor_16bits_drv_val_2_scc_val(
        SCC_ATTRIB_VALUE_MAX,
        t_drv_color_offset.ui2_r_gain,
        t_drv_co_min.ui2_r_gain,
        t_drv_co_max.ui2_r_gain );

    pt_color_offset->ui2_g_gain = _floor_16bits_drv_val_2_scc_val(
        SCC_ATTRIB_VALUE_MAX,
        t_drv_color_offset.ui2_g_gain,
        t_drv_co_min.ui2_g_gain,
        t_drv_co_max.ui2_g_gain );

    pt_color_offset->ui2_b_gain = _floor_16bits_drv_val_2_scc_val(
        SCC_ATTRIB_VALUE_MAX,
        t_drv_color_offset.ui2_b_gain,
        t_drv_co_min.ui2_b_gain,
        t_drv_co_max.ui2_b_gain );

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_get_tv_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: pe_tv_mode          References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_get_tv_mode(
    HANDLE_T                        h_disp,
    SCC_TV_MODE_T*                  pe_tv_mode )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SIZE_T                       z_size;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    PLA_MXR_TV_MODE_T            e_drv_tv_mode = PLA_MXR_TV_MODE_NORMAL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpe_tv_mode       = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)pe_tv_mode ));

    if ( !pe_tv_mode )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(PLA_MXR_TV_MODE_T);
    i4_ret = _scc_comp_disp_get(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_GET_TYPE_TV_MODE,
        (VOID*)&e_drv_tv_mode,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    switch( e_drv_tv_mode )
    {
    case PLA_MXR_TV_MODE_NORMAL:
        *pe_tv_mode = SCC_TV_MODE_NORMAL;
        break;

    case PLA_MXR_TV_MODE_PIP:
        *pe_tv_mode = SCC_TV_MODE_PIP;
        break;

    case PLA_MXR_TV_MODE_POP:
        *pe_tv_mode = SCC_TV_MODE_POP;
        break;

    default:
        return SMR_DRV_COMP_ERROR;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_get_mjc
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: b_mjc          References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_get_mjc(
    HANDLE_T                        h_disp,
    SCC_DISP_MJC_T*                 pe_mjc )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SIZE_T                       z_size;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    PLA_MXR_MJC_T                e_drv_mjc = PLA_MXR_MJC_OFF;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpe_mjc           = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)pe_mjc ));

    if ( !pe_mjc )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(PLA_MXR_MJC_T);
    i4_ret = _scc_comp_disp_get(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_GET_TYPE_MJC,
        (VOID*)&e_drv_mjc,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    switch( e_drv_mjc )
    {
    case PLA_MXR_MJC_OFF:
        *pe_mjc = SCC_DISP_MJC_OFF;
        break;

    case PLA_MXR_MJC_ON:
        *pe_mjc = SCC_DISP_MJC_ON;
        break;

    case PLA_MXR_MJC_DNM:
        *pe_mjc = SCC_DISP_MJC_DNM;
        break;

    default:
        return SMR_INV_GET_INFO;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_get_test_pattern
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: pe_pattern          References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_get_test_pattern(
    HANDLE_T                        h_disp,
    SCC_DISP_TEST_PATTERN_T*        pe_pattern )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    PLA_MXR_TEST_PATTERN_T       e_drv_pattern = PLA_MXR_TEST_PATTERN_DISABLE;
    SIZE_T                       z_size = sizeof(PLA_MXR_TEST_PATTERN_T);

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpe_pattern       = 0x%.8x )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)pe_pattern ));

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    if ( !pe_pattern )
    {
        return SMR_INV_ARG;
    }

    i4_ret = _scc_comp_disp_get(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_GET_TYPE_TEST_PATTERN,
        (VOID*)(UINT32)&e_drv_pattern,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    switch( e_drv_pattern )
    {
    case PLA_MXR_TEST_PATTERN_DISABLE:
        *pe_pattern = SCC_DISP_TEST_PATTERN_DISABLE;
        break;

    case PLA_MXR_TEST_PATTERN_1:
        *pe_pattern = SCC_DISP_TEST_PATTERN_1;
        break;

    case PLA_MXR_TEST_PATTERN_2:
        *pe_pattern = SCC_DISP_TEST_PATTERN_2;
        break;

    case PLA_MXR_TEST_PATTERN_3:
        *pe_pattern = SCC_DISP_TEST_PATTERN_3;
        break;

    case PLA_MXR_TEST_PATTERN_4:
        *pe_pattern = SCC_DISP_TEST_PATTERN_4;
        break;

    case PLA_MXR_TEST_PATTERN_5:
        *pe_pattern = SCC_DISP_TEST_PATTERN_5;
        break;

    case PLA_MXR_TEST_PATTERN_6:
        *pe_pattern = SCC_DISP_TEST_PATTERN_6;
        break;

    case PLA_MXR_TEST_PATTERN_7:
        *pe_pattern = SCC_DISP_TEST_PATTERN_7;
        break;

    case PLA_MXR_TEST_PATTERN_8:
        *pe_pattern = SCC_DISP_TEST_PATTERN_8;
        break;

    default:
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_set_back_light_ctrl
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *          b_is_on             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_set_back_light_ctrl(
    HANDLE_T                  h_disp,
    BOOL                      b_is_on )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tb_is_on          = %u  )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)b_is_on ));

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = _scc_comp_disp_set(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_SET_TYPE_BACK_LIGHT_CTRL,
        (VOID*)(UINT32)b_is_on,
        sizeof(BOOL) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_get_back_light_ctrl
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: pb_is_on            References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_get_back_light_ctrl(
    HANDLE_T                  h_disp,
    BOOL*                     pb_is_on )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    BOOL                         b_is_on = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp           = 0x%.8X\r\n"
                       "\t\tpb_is_on         = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)pb_is_on ));

    if ( !pb_is_on )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(BOOL);
    i4_ret = _scc_comp_disp_get(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_GET_TYPE_BACK_LIGHT_CTRL,
        (VOID*)&b_is_on,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    *pb_is_on = b_is_on;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_get_screen_resolution
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: pt_scr_resolution   References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INV_ARG         Fail.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_disp_get_screen_resolution(
    HANDLE_T                       h_disp,
    SCC_DISP_SCREEN_RESOLUTION_T*  pt_scr_resolution )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp = NULL;
    PLA_MXR_RESOLUTION_INFO_T    t_pmxr_resolution = {0};

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_disp             = 0x%.8X\r\n"
                       "\t\tpt_scr_resolution  = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_disp,
                       (UINT32)pt_scr_resolution ));

    if ( pt_scr_resolution == NULL )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_disp,
                                  &e_type,
                                  (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_disp_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(PLA_MXR_RESOLUTION_INFO_T);
    i4_ret = _scc_comp_disp_get(
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_GET_TYPE_RESOLUTION,
        (VOID*)&t_pmxr_resolution,
        &z_size );
    if ( i4_ret != RMR_OK )
    {
        return i4_ret;
    }

    pt_scr_resolution->ui2_screen_width  = t_pmxr_resolution.ui2_panel_width;
    pt_scr_resolution->ui2_screen_height = t_pmxr_resolution.ui2_panel_height;

    return SMR_OK;
}

/* Audio related API *********************************************************/
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_volume
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          pt_volume           References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_volume( HANDLE_T                      h_aud,
                            const SCC_AUD_VOLUME_INFO_T*  pt_volume )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_VOLUME_INFO_T        t_aud_vol_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud                 = 0x%.8X\r\n"
                       "\t\te_out_port            = %d\r\n"
                       "\t\te_ch                  = %d\r\n"
                       "\t\tui1_volumn            = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)((pt_volume)?pt_volume->e_out_port:(-1)),
                       (INT32)((pt_volume)?pt_volume->e_ch:(-1)),
                       (INT32)((pt_volume)?pt_volume->ui1_volumn:(-1)) ));

    if (pt_volume->ui1_volumn > 100)
    {
        SM_SLCTR_DBG_ERR((
                    "[SCC] pt_scc_aud_vol_info->ui1_volumn > 100.\r\n" ));
        return SMR_INV_SET_INFO;
    }

    if ( (pt_volume->e_out_port == SCC_AUD_OUT_PORT_2_CH) &&
         (pt_volume->e_ch != SCC_AUD_CHANNEL_FRONT_LEFT) &&
         (pt_volume->e_ch != SCC_AUD_CHANNEL_FRONT_RIGHT) &&
         (pt_volume->e_ch != SCC_AUD_CHANNEL_ALL))
    {
        SM_SLCTR_DBG_ERR((
            "[SCC] %s:%d, ERROR!! SCC_AUD_OUT_PORT_2_CH with invalid channel = %u.\r\n",
            __FUNCTION__, __LINE__,
            pt_volume->e_ch ));
        return SMR_INV_SET_INFO;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    if (pt_volume->e_ch == SCC_AUD_CHANNEL_ALL)
    {
        t_aud_vol_info.e_vol_type = AUD_DEC_ALL_CH;
        t_aud_vol_info.u.ui1_level = pt_volume->ui1_volumn;
    }
    else
    {
        t_aud_vol_info.e_vol_type = AUD_DEC_INDIVIDUAL_CH;
        t_aud_vol_info.u.t_ch_vol.ui1_level = pt_volume->ui1_volumn;

        t_aud_vol_info.u.t_ch_vol.e_ls = _scc_aud_ch_2_drv_aud_ch(
            pt_volume->e_ch );

        t_aud_vol_info.u.t_ch_vol.e_out_port  =
            _scc_aud_outport_2_drv_aud_outport(
                pt_volume->e_out_port );
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_VOLUME,
        (VOID*)&t_aud_vol_info,
        sizeof(AUD_DEC_VOLUME_INFO_T) );
    if ( i4_ret != RMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/* Audio related API *********************************************************/
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_volume_ex
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          pt_volume           References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_volume_ex( HANDLE_T                      h_aud,
                            const SCC_AUD_VOLUME_INFO_T*  pt_volume )
{
	 INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_VOLUME_INFO_EX_T     t_aud_vol_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud                 = 0x%.8X\r\n"
                       "\t\te_out_port            = %d\r\n"
                       "\t\te_ch                  = %d\r\n"
                       "\t\tui1_volumn            = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)((pt_volume)?pt_volume->e_out_port:(-1)),
                       (INT32)((pt_volume)?pt_volume->e_ch:(-1)),
                       (INT32)((pt_volume)?pt_volume->ui1_volumn:(-1)) ));

    if (pt_volume->ui1_volumn > 100)
    {
        SM_SLCTR_DBG_ERR((
                    "[SCC] pt_scc_aud_vol_info->ui1_volumn > 100.\r\n" ));
        return SMR_INV_SET_INFO;
    }

    if ( (pt_volume->e_out_port == SCC_AUD_OUT_PORT_2_CH) &&
         (pt_volume->e_ch != SCC_AUD_CHANNEL_FRONT_LEFT) &&
         (pt_volume->e_ch != SCC_AUD_CHANNEL_FRONT_RIGHT) &&
         (pt_volume->e_ch != SCC_AUD_CHANNEL_ALL))
    {
        SM_SLCTR_DBG_ERR((
            "[SCC] %s:%d, ERROR!! SCC_AUD_OUT_PORT_2_CH with invalid channel = %u.\r\n",
            __FUNCTION__, __LINE__,
            pt_volume->e_ch ));
        return SMR_INV_SET_INFO;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    if (pt_volume->e_ch == SCC_AUD_CHANNEL_ALL)
    {
        t_aud_vol_info.e_vol_type = AUD_DEC_ALL_CH;
        t_aud_vol_info.u.ui1_level = pt_volume->ui1_volumn;
        t_aud_vol_info.e_out_port  =
            _scc_aud_outport_2_drv_aud_outport(
                pt_volume->e_out_port );
    }
    else
    {
        t_aud_vol_info.e_vol_type = AUD_DEC_INDIVIDUAL_CH;
        t_aud_vol_info.u.t_ch_vol.ui1_level = pt_volume->ui1_volumn;
        t_aud_vol_info.u.t_ch_vol.e_ls = _scc_aud_ch_2_drv_aud_ch(
            pt_volume->e_ch );
        t_aud_vol_info.e_out_port  =
            _scc_aud_outport_2_drv_aud_outport(
                pt_volume->e_out_port );
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_VOLUME_EX,
        (VOID*)&t_aud_vol_info,
        sizeof(AUD_DEC_VOLUME_INFO_T) );
    if ( i4_ret != RMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}




static BOOL _b_mute_1 = FALSE;
static BOOL _b_mute_2 = FALSE;

INT32 x_scc_aud_switch(
    HANDLE_T    h_aud,
    BOOL        b_on)
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_MUTE_TYPE_T          e_mute;

    SM_SLCTR_DBG_API(( "[SM] %s:( h_aud = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud ));

    i4_ret = handle_get_type_obj(h_aud,
                                 &e_type,
                                 (VOID**)&pt_aud_comp );

    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    e_mute = (!b_on || _b_mute_2) ? AUD_DEC_MUTE_ON : AUD_DEC_MUTE_OFF;

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_MUTE,
        (VOID*) e_mute,
        sizeof(AUD_DEC_MUTE_TYPE_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    _b_mute_1 = !b_on;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_mute
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_mute( HANDLE_T                            h_aud )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:( h_aud = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_MUTE,
        (VOID*)AUD_DEC_MUTE_ON,
        sizeof(AUD_DEC_MUTE_TYPE_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    _b_mute_2 = TRUE;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_unmute
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_unmute( HANDLE_T                          h_aud )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:( h_aud = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    if (!_b_mute_1)
    {
        i4_ret = _scc_comp_aud_set(
            h_aud,
            DRVT_AUD_DEC,
            AUD_DEC_SET_TYPE_MUTE,
            (VOID*)AUD_DEC_MUTE_OFF,
            sizeof(AUD_DEC_MUTE_TYPE_T) );
        if ( i4_ret != SMR_OK )
        {
            return SMR_INV_SET_INFO;
        }
    }

    _b_mute_2 = FALSE;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_spdif_fmt
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          e_spdif_fmt         References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_spdif_fmt( HANDLE_T                   h_aud,
                               SCC_AUD_SPDIF_FMT_T        e_spdif_fmt )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_SPDIF_TYPE_T         e_aud_dec_spdif_type;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud                  = 0x%.8X\r\n"
                       "\t\te_spdif_fmt            = %d\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)e_spdif_fmt ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_spdif_fmt )
    {
    case SCC_AUD_SPDIF_FMT_OFF:
        e_aud_dec_spdif_type = AUD_DEC_SPDIF_OFF;
        break;
    case SCC_AUD_SPDIF_FMT_RAW:
        e_aud_dec_spdif_type = AUD_DEC_SPDIF_RAW;
        break;
    case SCC_AUD_SPDIF_FMT_PCM_16:
        e_aud_dec_spdif_type = AUD_DEC_SPDIF_PCM_16;
        break;
    case SCC_AUD_SPDIF_FMT_PCM_24:
        e_aud_dec_spdif_type = AUD_DEC_SPDIF_PCM_24;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_SPDIF,
        (VOID*)e_aud_dec_spdif_type,
        sizeof(AUD_DEC_SPDIF_TYPE_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_hdcd_filter
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          e_hdcd_fltr         References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_hdcd_filter( HANDLE_T                 h_aud,
                                 SCC_AUD_HDCD_FLTR_T      e_hdcd_fltr )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_HDCD_FLTR_T          e_drv_aud_dec_hdcd_fltr;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\te_hdcd_fltr     = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)e_hdcd_fltr ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_hdcd_fltr )
    {
    case SCC_AUD_HDCD_FLTR_OFF:
        e_drv_aud_dec_hdcd_fltr = AUD_DEC_HDCD_FLTR_OFF;
        break;
    case SCC_AUD_HDCD_FLTR_1X:
        e_drv_aud_dec_hdcd_fltr = AUD_DEC_HDCD_FLTR_1X;
        break;
    case SCC_AUD_HDCD_FLTR_2X:
        e_drv_aud_dec_hdcd_fltr = AUD_DEC_HDCD_FLTR_2X;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_HDCD_FLTR,
        (VOID*)(UINT32)e_drv_aud_dec_hdcd_fltr,
        sizeof(AUD_DEC_HDCD_FLTR_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_loudspeaker
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          pt_ls_type          References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_loudspeaker(
    HANDLE_T                       h_aud,
    const SCC_AUD_LOUD_SPEAKER_T*  pt_ls_type )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_CH_LS_MODE_T         t_aud_ch_ls_mode;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud                  = 0x%.8X\r\n"
                       "\t\tpt_ls_type->e_channel  = %d\r\n"
                       "\t\tpt_ls_type->e_mode     = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)((pt_ls_type)?pt_ls_type->e_channel:(-1)),
                       (INT32)((pt_ls_type)?pt_ls_type->e_mode:(-1)) ));

    if ( !pt_ls_type )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( pt_ls_type->e_mode )
    {
    case SCC_AUD_LOUD_SPEAKER_MODE_OFF:
        t_aud_ch_ls_mode.e_mode = AUD_DEC_LS_MODE_OFF;
        break;
    case SCC_AUD_LOUD_SPEAKER_MODE_LARGE_FREQ_RANGE:
        t_aud_ch_ls_mode.e_mode = AUD_DEC_LS_MODE_LARGE_FREQ_RANGE;
        break;
    case SCC_AUD_LOUD_SPEAKER_MODE_SMALL_FREQ_RANGE:
        t_aud_ch_ls_mode.e_mode = AUD_DEC_LS_MODE_SMALL_FREQ_RANGE;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    if ( pt_ls_type->e_channel != SCC_AUD_CHANNEL_ALL )
    {
        t_aud_ch_ls_mode.e_ls =
            _scc_aud_ch_2_drv_aud_ch( pt_ls_type->e_channel );

        i4_ret = _scc_comp_aud_set(
            h_aud,
            DRVT_AUD_DEC,
            AUD_DEC_SET_TYPE_LS_MODE,
            (VOID*)&t_aud_ch_ls_mode,
            sizeof(AUD_DEC_CH_LS_MODE_T) );
        if ( i4_ret != SMR_OK )
        {
            return SMR_INV_SET_INFO;
        }
    }
    else /* set all channel */
    {
        for ( t_aud_ch_ls_mode.e_ls = AUD_DEC_LS_FRONT_LEFT;
              t_aud_ch_ls_mode.e_ls <= AUD_DEC_LS_SUB_WOOFER;
              t_aud_ch_ls_mode.e_ls++ )
        {
            i4_ret = _scc_comp_aud_set(
                h_aud,
                DRVT_AUD_DEC,
                AUD_DEC_SET_TYPE_LS_MODE,
                (VOID*)&t_aud_ch_ls_mode,
                sizeof(AUD_DEC_CH_LS_MODE_T) );
            if ( i4_ret != SMR_OK )
            {
                return SMR_INV_SET_INFO;
            }
        }
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_se_trim
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          pt_se_trim          References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_se_trim(
    HANDLE_T                        h_aud,
    const SCC_AUD_SE_TRIM_T*        pt_se_trim )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_SE_INFO_T            t_drv_se_info;
    AUD_DEC_CH_TRIM_T            t_drv_se_ch_trim;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tpt_se_trim      = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pt_se_trim ));

    if ( !pt_se_trim )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_se_info.e_se_type = AUD_DEC_SE_TRIM;
    t_drv_se_info.pv_info = (VOID*)&t_drv_se_ch_trim;

    t_drv_se_ch_trim.e_ls = _scc_aud_ch_2_drv_aud_ch( pt_se_trim->e_channel );

    if ( pt_se_trim->ui1_level > 100 )
    {
        return SMR_INV_GET_INFO;
    }
    t_drv_se_ch_trim.ui1_level = pt_se_trim->ui1_level;

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_SOUND_EFFECT,
        (VOID*)&t_drv_se_info,
        sizeof( AUD_DEC_SE_INFO_T ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_se_superbase
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          b_is_on             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_se_superbase(
    HANDLE_T                        h_aud,
    BOOL                            b_is_on )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_SE_INFO_T            t_drv_se_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tb_is_on         = %s )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       ((b_is_on)?"TRUE":"FALSE") ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_se_info.e_se_type = AUD_DEC_SE_SUPERBASS;
    t_drv_se_info.pv_info = (VOID*)(UINT32)b_is_on;

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_SOUND_EFFECT,
        (VOID*)&t_drv_se_info,
        sizeof( AUD_DEC_SE_INFO_T ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_se_equalizer
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          e_equalizer         References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_se_equalizer(
    HANDLE_T                        h_aud,
    SCC_AUD_SE_EQ_T                 e_equalizer )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_SE_INFO_T            t_drv_se_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\te_equalizer     = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)e_equalizer ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_se_info.e_se_type = AUD_DEC_SE_EQUALIZER;
    t_drv_se_info.pv_info = (VOID*)(UINT32)_convert_scc_se_2_drv_se( e_equalizer );

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_SOUND_EFFECT,
        (VOID*)&t_drv_se_info,
        sizeof( AUD_DEC_SE_INFO_T ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_se_reverb
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          e_reverb            References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_se_reverb(
    HANDLE_T                        h_aud,
    SCC_AUD_SE_REVERB_MODE_T        e_reverb )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_SE_INFO_T            t_drv_se_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\te_reverb        = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)e_reverb ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_se_info.e_se_type = AUD_DEC_SE_REVERB;

    switch( e_reverb )
    {
    case SCC_AUD_SE_REVERB_OFF:
        t_drv_se_info.pv_info = (VOID*)AUD_DEC_REVERB_OFF;
        break;
    case SCC_AUD_SE_REVERB_CONCERT:
        t_drv_se_info.pv_info = (VOID*)AUD_DEC_REVERB_CONCERT;
        break;
    case SCC_AUD_SE_REVERB_LIVINGROOM:
        t_drv_se_info.pv_info = (VOID*)AUD_DEC_REVERB_LIVINGROOM;
        break;
    case SCC_AUD_SE_REVERB_HALL:
        t_drv_se_info.pv_info = (VOID*)AUD_DEC_REVERB_HALL;
        break;
    case SCC_AUD_SE_REVERB_BATHROOM:
        t_drv_se_info.pv_info = (VOID*)AUD_DEC_REVERB_BATHROOM;
        break;
    case SCC_AUD_SE_REVERB_CAVE:
        t_drv_se_info.pv_info = (VOID*)AUD_DEC_REVERB_CAVE;
        break;
    case SCC_AUD_SE_REVERB_ARENA:
        t_drv_se_info.pv_info = (VOID*)AUD_DEC_REVERB_ARENA;
        break;
    case SCC_AUD_SE_REVERB_CHURCH:
        t_drv_se_info.pv_info = (VOID*)AUD_DEC_REVERB_CHURCH;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_SOUND_EFFECT,
        (VOID*)&t_drv_se_info,
        sizeof( AUD_DEC_SE_INFO_T ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_se_base
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          ui1_base            References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_se_base(
    HANDLE_T                        h_aud,
    UINT8                           ui1_base )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_SE_INFO_T            t_drv_se_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tui1_base        = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       ui1_base ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_se_info.e_se_type = AUD_DEC_SE_BASS;
    t_drv_se_info.pv_info = (VOID*)(UINT32)ui1_base;

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_SOUND_EFFECT,
        (VOID*)&t_drv_se_info,
        sizeof( AUD_DEC_SE_INFO_T ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_se_treble
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          ui1_base            References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_se_treble(
    HANDLE_T                        h_aud,
    UINT8                           ui1_treble )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_SE_INFO_T            t_drv_se_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tui1_treble      = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       ui1_treble ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_se_info.e_se_type = AUD_DEC_SE_TREBLE;
    t_drv_se_info.pv_info = (VOID*)(UINT32)ui1_treble;

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_SOUND_EFFECT,
        (VOID*)&t_drv_se_info,
        sizeof( AUD_DEC_SE_INFO_T ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_se_balance
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          ui1_balance         References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_se_balance(
    HANDLE_T                        h_aud,
    UINT8                           ui1_balance )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_SE_INFO_T            t_drv_se_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tui1_balance     = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       ui1_balance ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_se_info.e_se_type = AUD_DEC_SE_BALANCE;
    t_drv_se_info.pv_info = (VOID*)(UINT32)ui1_balance;

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_SOUND_EFFECT,
        (VOID*)&t_drv_se_info,
        sizeof( AUD_DEC_SE_INFO_T ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_se_postdr
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          b_is_on             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_se_postdr(
    HANDLE_T                        h_aud,
    BOOL                            b_is_on )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_SE_INFO_T            t_drv_se_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tb_is_on         = %s )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       ((b_is_on)?"TRUE":"FALSE") ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_se_info.e_se_type = AUD_DEC_SE_POSTDRC;
    t_drv_se_info.pv_info = (VOID*)(UINT32)b_is_on;

    z_size = sizeof( AUD_DEC_SE_INFO_T );
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_SOUND_EFFECT,
        (VOID*)&t_drv_se_info,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_se_volume
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          pt_volume           References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_se_volume(
    HANDLE_T                        h_aud,
    const SCC_AUD_VOLUME_INFO_T*    pt_volume )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_SE_INFO_T            t_drv_se_info;
    AUD_DEC_CH_VOLUME_T          t_drv_volume;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud                 = 0x%.8X\r\n"
                       "\t\te_out_port            = %d\r\n"
                       "\t\te_ch                  = %d\r\n"
                       "\t\tui1_volumn            = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)((pt_volume)?pt_volume->e_out_port:(-1)),
                       (INT32)((pt_volume)?pt_volume->e_ch:(-1)),
                       (INT32)((pt_volume)?pt_volume->ui1_volumn:(-1)) ));

    if ( !pt_volume )
    {
        return SMR_INV_ARG;
    }

    if (pt_volume->ui1_volumn > 100)
    {
        SM_SLCTR_DBG_ERR((
            "[SCC] %s:%d, ERROR!! pt_scc_aud_vol_info->ui1_volumn > 100.\r\n",
            __FUNCTION__, __LINE__ ));
        return SMR_INV_SET_INFO;
    }

    if ( (pt_volume->e_out_port == SCC_AUD_OUT_PORT_2_CH) &&
         (pt_volume->e_ch != SCC_AUD_CHANNEL_FRONT_LEFT) &&
         (pt_volume->e_ch != SCC_AUD_CHANNEL_FRONT_RIGHT) &&
         (pt_volume->e_ch != SCC_AUD_CHANNEL_ALL))
    {
        SM_SLCTR_DBG_ERR((
            "[SCC] %s:%d, ERROR!! SCC_AUD_OUT_PORT_2_CH with invalid channel = %u.\r\n",
            __FUNCTION__, __LINE__,
            pt_volume->e_ch ));
        return SMR_INV_SET_INFO;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_se_info.e_se_type = AUD_DEC_SE_VOLUME;
    t_drv_se_info.pv_info = (VOID*)&t_drv_volume;

    if ( pt_volume->e_ch == SCC_AUD_CHANNEL_ALL )
    {
        t_drv_volume.e_out_port = _scc_aud_outport_2_drv_aud_outport(
            pt_volume->e_out_port );
        t_drv_volume.ui1_level = pt_volume->ui1_volumn;

        for( t_drv_volume.e_ls = AUD_DEC_LS_FRONT_LEFT;
             t_drv_volume.e_ls <= AUD_DEC_LS_SUB_WOOFER;
             ++t_drv_volume.e_ls )
        {
            i4_ret = _scc_comp_aud_set(
                h_aud,
                DRVT_AUD_DEC,
                AUD_DEC_SET_TYPE_SOUND_EFFECT,
                (VOID*)&t_drv_se_info,
                sizeof( AUD_DEC_SE_INFO_T ) );
            if ( i4_ret != SMR_OK )
            {
                return SMR_INV_GET_INFO;
            }
        }
    }
    else
    {
        _scc_aud_volume_2_drv_ch_volume( pt_volume, &t_drv_volume );

        i4_ret = _scc_comp_aud_set(
            h_aud,
            DRVT_AUD_DEC,
            AUD_DEC_SET_TYPE_SOUND_EFFECT,
            (VOID*)&t_drv_se_info,
            sizeof( AUD_DEC_SE_INFO_T ) );
        if ( i4_ret != SMR_OK )
        {
            return SMR_INV_GET_INFO;
        }
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_se_surround
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          b_surround          References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_se_surround(
    HANDLE_T                        h_aud,
    BOOL                            b_is_on )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_SE_INFO_T            t_drv_se_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tb_is_on         = %s )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       ((b_is_on)?"TRUE":"FALSE") ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_se_info.e_se_type = AUD_DEC_SE_SURROUND;
    t_drv_se_info.pv_info = (VOID*)(UINT32)b_is_on;

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_SOUND_EFFECT,
        (VOID*)&t_drv_se_info,
        sizeof( AUD_DEC_SE_INFO_T ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_se_superwoofer
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          b_is_on          References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_se_superwoofer(
    HANDLE_T                        h_aud,
    BOOL                            b_is_on )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_SE_INFO_T            t_drv_se_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tb_is_on         = %s )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       ((b_is_on)?"TRUE":"FALSE") ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_se_info.e_se_type = AUD_DEC_SE_SUPERWOOFER;
    t_drv_se_info.pv_info = (VOID*)(UINT32)b_is_on;

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_SOUND_EFFECT,
        (VOID*)&t_drv_se_info,
        sizeof( AUD_DEC_SE_INFO_T ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_panfade
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          b_is_on          References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_panfade(
    HANDLE_T                        h_aud,
    BOOL         b_is_on)
{
    INT32                   i4_ret;
    HANDLE_TYPE_T           e_type;
    SCC_AUD_COMP_T*         pt_aud_comp = NULL;
    AUD_DEC_PANFADE_CTRL_T  e_dec_panfade;
    
    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                   "\t\th_aud           = 0x%.8X\r\n"
                   "\t\tb_is_on         = %s )\r\n",
                   __FUNCTION__,
                   (UINT32)h_aud,
                   ((b_is_on)?"TRUE":"FALSE") ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }
    e_dec_panfade=(b_is_on== TRUE) ?AUD_DEC_PANFADE_ENABLE : AUD_DEC_PANFADE_DISABLE;
    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_PANFADE_ENABLE,
        (VOID*)&e_dec_panfade,
        sizeof( AUD_DEC_PANFADE_CTRL_T ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_se_sound_mode_start
 *
 * Description: <function description>
 *
 * Inputs:  h_aud              References.
 *          b_is_on            References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_se_sound_mode_start(
    HANDLE_T                        h_aud,
    BOOL                            b_is_on )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_SE_INFO_T            t_drv_se_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tb_is_on         = %s )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       ((b_is_on)?"TRUE":"FALSE") ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_se_info.e_se_type = AUD_DEC_SE_SOUND_MODE_START;
    t_drv_se_info.pv_info = (VOID*)(UINT32)b_is_on;

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_SOUND_EFFECT,
        (VOID*)&t_drv_se_info,
        sizeof( AUD_DEC_SE_INFO_T ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_srs_bypass
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          b_is_on          References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_srs_bypass(
    HANDLE_T                        h_aud,
    BOOL                            b_is_on )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tb_is_on         = %s )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       ((b_is_on)?"TRUE":"FALSE") ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_SRS_BYPASS,
        (VOID*)(UINT32)b_is_on,
        sizeof( BOOL ) );

    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}


#if 0
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_se_surround_ex
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          ui4_surround_mask     References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_se_surround_ex( HANDLE_T                 h_aud,
                                 UINT32                          ui4_surround_mode )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp;
    UINT32                       ui4_drv_sourrond_mode = AUD_DEC_SURROUND_TYPE_OFF;
    AUD_DEC_SE_INFO_T            t_drv_se_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud                  = 0x%.8X\r\n"
                       "\t\tui4_surround_mode                 = %x\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)ui4_surround_mode));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }
    
    switch( ui4_surround_mode )
    {
        case SCC_AUD_SE_SURROUND_MODE_OFF:
            ui4_drv_sourrond_mode = AUD_DEC_SURROUND_TYPE_OFF;
            break;
        case SCC_AUD_SE_SURROUND_MODE_MVS_DEFAULT:
            ui4_drv_sourrond_mode = AUD_DEC_SURROUND_TYPE_MVS_DEFAULT;
            break;
        case SCC_AUD_SE_SURROUND_MODE_MONO_TO_STEREO:
            ui4_drv_sourrond_mode = AUD_DEC_SURROUND_TYPE_MONO_TO_STEREO;
            break;
        case SCC_AUD_SE_SURROUND_MODE_USER_1:
            ui4_drv_sourrond_mode = AUD_DEC_SURROUND_TYPE_USER_1;
            break;
        case SCC_AUD_SE_SURROUND_MODE_USER_2:
            ui4_drv_sourrond_mode = AUD_DEC_SURROUND_TYPE_USER_2;
            break;
        case SCC_AUD_SE_SURROUND_MODE_USER_3:
            ui4_drv_sourrond_mode = AUD_DEC_SURROUND_TYPE_USER_3;
            break;
        case SCC_AUD_SE_SURROUND_MODE_USER_4:
            ui4_drv_sourrond_mode = AUD_DEC_SURROUND_TYPE_USER_4;
            break;
        case SCC_AUD_SE_SURROUND_MODE_USER_5:
            ui4_drv_sourrond_mode = AUD_DEC_SURROUND_TYPE_USER_5;
            break;
        case SCC_AUD_SE_SURROUND_MODE_USER_6:
            ui4_drv_sourrond_mode = AUD_DEC_SURROUND_TYPE_USER_6;
            break;

        default:
            return SMR_INV_SET_INFO;
    }

    t_drv_se_info.e_se_type = AUD_DEC_SE_SURROUND;
    t_drv_se_info.pv_info = (VOID*)ui4_drv_sourrond_mode;
	
    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_SOUND_EFFECT,
        (VOID*)&t_drv_se_info,
        sizeof( AUD_DEC_SE_INFO_T ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }
    
    return SMR_OK;
}
#endif
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_se_surround_ex
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          pt_surround_ex     References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_se_surround_ex( HANDLE_T                 h_aud,
                                 SCC_AUD_SE_SURROUND_INFO_T*                pt_surround_ex )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    UINT32                       ui4_drv_sourrond_mode = AUD_DEC_SURROUND_TYPE_OFF;
    AUD_DEC_SE_INFO_T            t_drv_se_info;
    AUD_SE_SURROUND_EX_INFO_T    t_drv_se_ex_info;
    
    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud                  = 0x%.8X\r\n"
                       "\t\tpt_surround_ex                 = %x\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pt_surround_ex));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }
    if ( !pt_surround_ex )
    {
        return SMR_INV_ARG;
    }
    if( pt_surround_ex->e_surround_set_type == SCC_AUD_SE_SURROUND_SET_TYPE_USER )
    {
        switch( pt_surround_ex->u.e_se_surround_mode )
        {
            case SCC_AUD_SE_SURROUND_MODE_OFF:
                ui4_drv_sourrond_mode = AUD_DEC_SURROUND_TYPE_OFF;
                break;
            case SCC_AUD_SE_SURROUND_MODE_MVS_DEFAULT:
                ui4_drv_sourrond_mode = AUD_DEC_SURROUND_TYPE_MVS_DEFAULT;
                break;
            case SCC_AUD_SE_SURROUND_MODE_MONO_TO_STEREO:
                ui4_drv_sourrond_mode = AUD_DEC_SURROUND_TYPE_MONO_TO_STEREO;
                break;
            case SCC_AUD_SE_SURROUND_MODE_USER_1:
                ui4_drv_sourrond_mode = AUD_DEC_SURROUND_TYPE_USER_1;
                break;
            case SCC_AUD_SE_SURROUND_MODE_USER_2:
                ui4_drv_sourrond_mode = AUD_DEC_SURROUND_TYPE_USER_2;
                break;
            case SCC_AUD_SE_SURROUND_MODE_USER_3:
                ui4_drv_sourrond_mode = AUD_DEC_SURROUND_TYPE_USER_3;
                break;
            case SCC_AUD_SE_SURROUND_MODE_USER_4:
                ui4_drv_sourrond_mode = AUD_DEC_SURROUND_TYPE_USER_4;
                break;
            case SCC_AUD_SE_SURROUND_MODE_USER_5:
                ui4_drv_sourrond_mode = AUD_DEC_SURROUND_TYPE_USER_5;
                break;
            case SCC_AUD_SE_SURROUND_MODE_USER_6:
                ui4_drv_sourrond_mode = AUD_DEC_SURROUND_TYPE_USER_6;
                break;
        
            default:
                return SMR_INV_SET_INFO;
        }
        
        t_drv_se_info.e_se_type = AUD_DEC_SE_SURROUND;
        t_drv_se_info.pv_info = (VOID*)ui4_drv_sourrond_mode;

    }
    else if( pt_surround_ex->e_surround_set_type == SCC_AUD_SE_SURROUND_SET_TYPE_VALUE)
    {
        t_drv_se_info.e_se_type = AUD_DEC_SE_SURROUND_EX;
                
        t_drv_se_ex_info.ui1BassGain    = pt_surround_ex->u.t_se_srround_value.ui1BassGain;
        t_drv_se_ex_info.ui1Clarity       = pt_surround_ex->u.t_se_srround_value.ui1Clarity;
        t_drv_se_ex_info.ui1LRGain       = pt_surround_ex->u.t_se_srround_value.ui1LRGain;
        t_drv_se_ex_info.ui1OutputGain = pt_surround_ex->u.t_se_srround_value.ui1OutputGain;
        t_drv_se_ex_info.ui1Width        = pt_surround_ex->u.t_se_srround_value.ui1Width;
        t_drv_se_ex_info.ui1Xtalk         = pt_surround_ex->u.t_se_srround_value.ui1Xtalk;
        t_drv_se_ex_info.ui1Fo1           = pt_surround_ex->u.t_se_srround_value.ui1Fo1;
        t_drv_se_ex_info.ui1Fo2           = pt_surround_ex->u.t_se_srround_value.ui1Fo2;
        t_drv_se_ex_info.ui1Fo3           = pt_surround_ex->u.t_se_srround_value.ui1Fo3;
        t_drv_se_ex_info.ui1Mode         = pt_surround_ex->u.t_se_srround_value.ui1Mode;

        t_drv_se_info.pv_info = (VOID*)&t_drv_se_ex_info;
    }
    else 
    {
        return SMR_INV_SET_INFO;
    }
    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_SOUND_EFFECT,
        (VOID*)&t_drv_se_info,
        sizeof( AUD_DEC_SE_INFO_T ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }
    
    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_se_sound_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          e_sound_mode        References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_se_sound_mode( HANDLE_T                h_aud,
                                  SCC_AUD_SE_SOUND_MODE_T  e_sound_mode )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_SE_INFO_T            t_drv_se_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\te_reverb        = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)e_sound_mode ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_se_info.e_se_type = AUD_DEC_SE_SOUND_MODE;

    switch( e_sound_mode )
    {
    case SCC_AUD_SE_SOUND_MODE_DEFAULT:
        t_drv_se_info.pv_info = (VOID*)AUD_SE_SOUND_MODE_DEFAULT;
        break;
    case SCC_AUD_SE_SOUND_MODE_STANDARD:
        t_drv_se_info.pv_info = (VOID*)AUD_SE_SOUND_MODE_STANDARD;
        break;
    case SCC_AUD_SE_SOUND_MODE_DYNAMIC:
        t_drv_se_info.pv_info = (VOID*)AUD_SE_SOUND_MODE_DYNAMIC;
        break;
    case SCC_AUD_SE_SOUND_MODE_CLEAR_VOICE:
        t_drv_se_info.pv_info = (VOID*)AUD_SE_SOUND_MODE_CLEAR_VOICE;
        break;
    case SCC_AUD_SE_SOUND_MODE_CUSTOM_1:
        t_drv_se_info.pv_info = (VOID*)AUD_SE_SOUND_MODE_CUSTOM_1;
        break;
    case SCC_AUD_SE_SOUND_MODE_CUSTOM_2:
        t_drv_se_info.pv_info = (VOID*)AUD_SE_SOUND_MODE_CUSTOM_2;
        break;
    case SCC_AUD_SE_SOUND_MODE_CUSTOM_3:
        t_drv_se_info.pv_info = (VOID*)AUD_SE_SOUND_MODE_CUSTOM_3;
        break;
    case SCC_AUD_SE_SOUND_MODE_CUSTOM_4:
        t_drv_se_info.pv_info = (VOID*)AUD_SE_SOUND_MODE_CUSTOM_4;
        break;
    case SCC_AUD_SE_SOUND_MODE_CUSTOM_5:
        t_drv_se_info.pv_info = (VOID*)AUD_SE_SOUND_MODE_CUSTOM_5;
        break;

    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_SOUND_EFFECT,
        (VOID*)&t_drv_se_info,
        sizeof( AUD_DEC_SE_INFO_T ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_downmix_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          e_dm_mode           References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_downmix_mode( HANDLE_T                h_aud,
                                  SCC_AUD_DOWNMIX_MODE_T  e_dm_mode )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_DOWNMIX_MODE_T       e_drv_aud_dec_dm_mode;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\te_dm_mode       = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)e_dm_mode ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_dm_mode )
    {
    case SCC_AUD_DOWNMIX_MODE_OFF:
        e_drv_aud_dec_dm_mode = AUD_DEC_DM_OFF;
        break;
    case SCC_AUD_DOWNMIX_MODE_LT_RT:
        e_drv_aud_dec_dm_mode = AUD_DEC_DM_LT_RT;
        break;
    case SCC_AUD_DOWNMIX_MODE_STEREO:
        e_drv_aud_dec_dm_mode = AUD_DEC_DM_STEREO;
        break;
    case SCC_AUD_DOWNMIX_MODE_VIR_SURR:
        e_drv_aud_dec_dm_mode = AUD_DEC_DM_VIR_SURR;
        break;
    case SCC_AUD_DOWNMIX_MODE_MONO:
        e_drv_aud_dec_dm_mode = AUD_DEC_DM_MONO;
        break;
    case SCC_AUD_DOWNMIX_MODE_DUAL1:
        e_drv_aud_dec_dm_mode = AUD_DEC_DM_DUAL1;
        break;
    case SCC_AUD_DOWNMIX_MODE_DUAL2:
        e_drv_aud_dec_dm_mode = AUD_DEC_DM_DUAL2;
        break;
    case SCC_AUD_DOWNMIX_MODE_DUAL_MIX:
        e_drv_aud_dec_dm_mode = AUD_DEC_DM_DUAL_MIX;
        break;
    case SCC_AUD_DOWNMIX_MODE_3_STEREO:
        e_drv_aud_dec_dm_mode = AUD_DEC_DM_3_STEREO;
        break;
    case SCC_AUD_DOWNMIX_MODE_DUAL_OFF:
        e_drv_aud_dec_dm_mode = AUD_DEC_DM_DUAL_OFF;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_DOWNMIX,
        (VOID*)(UINT32)e_drv_aud_dec_dm_mode,
        sizeof(AUD_DEC_DOWNMIX_MODE_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_dolby_drc
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          e_drc               References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_dolby_drc( HANDLE_T                   h_aud,
                               SCC_AUD_DOLBY_DRC_T        e_drc )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_DRC_T                e_drv_aud_dec_drc;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\te_drc           = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)e_drc ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_drc )
    {
    case SCC_AUD_DOLBY_DRC_OFF:
        e_drv_aud_dec_drc = AUD_DEC_DRC_OFF;
        break;
    case SCC_AUD_DOLBY_DRC_10:
        e_drv_aud_dec_drc = AUD_DEC_DRC_10;
        break;
    case SCC_AUD_DOLBY_DRC_20:
        e_drv_aud_dec_drc = AUD_DEC_DRC_20;
        break;
    case SCC_AUD_DOLBY_DRC_30:
        e_drv_aud_dec_drc = AUD_DEC_DRC_30;
        break;
    case SCC_AUD_DOLBY_DRC_40:
        e_drv_aud_dec_drc = AUD_DEC_DRC_40;
        break;
    case SCC_AUD_DOLBY_DRC_50:
        e_drv_aud_dec_drc = AUD_DEC_DRC_50;
        break;
    case SCC_AUD_DOLBY_DRC_60:
        e_drv_aud_dec_drc = AUD_DEC_DRC_60;
        break;
    case SCC_AUD_DOLBY_DRC_70:
        e_drv_aud_dec_drc = AUD_DEC_DRC_70;
        break;
	case SCC_AUD_DOLBY_DRC_80:
		e_drv_aud_dec_drc = AUD_DEC_DRC_80;
		break;
	case SCC_AUD_DOLBY_DRC_90:
		e_drv_aud_dec_drc = AUD_DEC_DRC_90;
		break;
    case SCC_AUD_DOLBY_DRC_100:
        e_drv_aud_dec_drc = AUD_DEC_DRC_100;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_DRC,
        (VOID*)(UINT32)e_drv_aud_dec_drc,
        sizeof(AUD_DEC_DRC_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_cmpss_drc
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          e_cmpss_drc         References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_cmpss_drc( 
    HANDLE_T                            h_aud,
    SCC_AUD_CMPSS_DRC_MODE_T            e_cmpss_drc )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_CMPSS_DRC_MODE_T     e_drv_cmpss_drc;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\te_cmpss_drc           = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)e_cmpss_drc ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( e_cmpss_drc )
    {
    case SCC_AUD_CMPSS_DRC_MODE_COMPRESSED:
        e_drv_cmpss_drc = AUD_DEC_CMPSS_DRC_MODE_COMPRESSED;
        break;
    case SCC_AUD_CMPSS_DRC_MODE_STANDARD:
        e_drv_cmpss_drc = AUD_DEC_CMPSS_DRC_MODE_STANDARD;
        break;
    case SCC_AUD_CMPSS_DRC_MODE_OFF:
        e_drv_cmpss_drc = AUD_DEC_CMPSS_DRC_MODE_OFF;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_CMPSS_DRC,
        (VOID*)(UINT32)e_drv_cmpss_drc,
        sizeof(AUD_DEC_CMPSS_DRC_MODE_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_mpeg_level_adj
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          i1_level              References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_mpeg_level_adj(
    HANDLE_T                        h_aud,
    INT8                            i1_level )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\ti1_level       = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       i1_level ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }


    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_MPEG_LEVEL_ADJ,
        (VOID*)(UINT32)i1_level,
        sizeof( UINT8 ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_output_port
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          e_port              References.
 *          b_is_active         References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_output_port( HANDLE_T                 h_aud,
                                 SCC_AUD_OUT_PORT_T       e_port,
                                 BOOL                     b_is_active )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    UINT32                       ui4_drv_out_port_mask = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud                  = 0x%.8X\r\n"
                       "\t\te_port                 = %d\r\n"
                       "\t\tb_is_active            = %s )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)e_port,
                       (b_is_active)?"TRUE":"FALSE" ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(UINT32);
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_OUT_PORT,
        (VOID*)&ui4_drv_out_port_mask,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    switch ( e_port )
    {
    case SCC_AUD_OUT_PORT_OFF:
        return SMR_OK;

    case SCC_AUD_OUT_PORT_2_CH:
        if ( b_is_active )
        {
            ui4_drv_out_port_mask |= AUD_DEC_OUT_PORT_FLAG_2_CH;
        }
        else
        {
            ui4_drv_out_port_mask &= ~AUD_DEC_OUT_PORT_FLAG_2_CH;
        }
        break;
    case SCC_AUD_OUT_PORT_5_1_CH:
        if ( b_is_active )
        {
            ui4_drv_out_port_mask |= AUD_DEC_OUT_PORT_FLAG_5_1_CH;
        }
        else
        {
            ui4_drv_out_port_mask &= ~AUD_DEC_OUT_PORT_FLAG_5_1_CH;
        }
        break;
    case SCC_AUD_OUT_PORT_SPDIF:
        if ( b_is_active )
        {
            ui4_drv_out_port_mask |= AUD_DEC_OUT_PORT_FLAG_SPDIF;
        }
        else
        {
            ui4_drv_out_port_mask &= ~AUD_DEC_OUT_PORT_FLAG_SPDIF;
        }
        break;
    case SCC_AUD_OUT_PORT_2_CH_BY_PASS:
        if ( b_is_active )
        {
            ui4_drv_out_port_mask |= AUD_DEC_OUT_PORT_FLAG_2_CH_BY_PASS;
        }
        else
        {
            ui4_drv_out_port_mask &= ~AUD_DEC_OUT_PORT_FLAG_2_CH_BY_PASS;
        }
        break;
    case SCC_AUD_OUT_PORT_SPEAKER:
        if ( b_is_active )
        {
            ui4_drv_out_port_mask |= AUD_DEC_OUT_PORT_FLAG_SPEAKER;
        }
        else
        {
            ui4_drv_out_port_mask &= ~AUD_DEC_OUT_PORT_FLAG_SPEAKER;
        }
        break;
    case SCC_AUD_OUT_PORT_HEADPHONE:
        if ( b_is_active )
        {
            ui4_drv_out_port_mask |= AUD_DEC_OUT_PORT_FLAG_HEADPHONE;
        }
        else
        {
            ui4_drv_out_port_mask &= ~AUD_DEC_OUT_PORT_FLAG_HEADPHONE;
        }
        break;
    case SCC_AUD_OUT_PORT_TVSCART:
        if ( b_is_active )
        {
            ui4_drv_out_port_mask |= AUD_DEC_OUT_PORT_FLAG_TVSCART;
        }
        else
        {
            ui4_drv_out_port_mask &= ~AUD_DEC_OUT_PORT_FLAG_TVSCART;
        }
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_OUT_PORT,
        (VOID*)ui4_drv_out_port_mask,
        sizeof(UINT32) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_output_port_ex
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          ui4_enable_mask     References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_output_port_ex( HANDLE_T                 h_aud,
                                 UINT32                          ui4_enable_mask )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    UINT32                       ui4_drv_out_port_mask = AUD_DEC_OUT_PORT_FLAG_NONE;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud                  = 0x%.8X\r\n"
                       "\t\tui4_enable_mask                 = %x\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)ui4_enable_mask));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    if (ui4_enable_mask&SCC_AUD_OUT_PORT_2_CH)
    {
        ui4_drv_out_port_mask |= AUD_DEC_OUT_PORT_FLAG_2_CH;
    }

    if (ui4_enable_mask&SCC_AUD_OUT_PORT_5_1_CH)
    {
        ui4_drv_out_port_mask |= AUD_DEC_OUT_PORT_FLAG_5_1_CH;
    }

    if (ui4_enable_mask&SCC_AUD_OUT_PORT_SPDIF)
    {
        ui4_drv_out_port_mask |= AUD_DEC_OUT_PORT_FLAG_SPDIF;
    }

    if (ui4_enable_mask&SCC_AUD_OUT_PORT_2_CH_BY_PASS)
    {
        ui4_drv_out_port_mask |= AUD_DEC_OUT_PORT_FLAG_2_CH_BY_PASS;
    }

    if (ui4_enable_mask&SCC_AUD_OUT_PORT_SPEAKER)
    {
        ui4_drv_out_port_mask |= AUD_DEC_OUT_PORT_FLAG_SPEAKER;
    }
    if (ui4_enable_mask&SCC_AUD_OUT_PORT_HEADPHONE)
    {
        ui4_drv_out_port_mask |= AUD_DEC_OUT_PORT_FLAG_HEADPHONE;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_OUT_PORT,
        (VOID*)ui4_drv_out_port_mask,
        sizeof(UINT32) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_cmpss_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          e_cmpss_mode        References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_cmpss_mode( HANDLE_T                  h_aud,
                                SCC_AUD_CMPSS_MDOE_T      e_cmpss_mode )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_CMPSS_MODE_T         e_drv_aud_cmpss_mode;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\te_cmpss_mode    = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)e_cmpss_mode ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch (e_cmpss_mode)
    {
    case SCC_AUD_CMPSS_MDOE_LINE:
        e_drv_aud_cmpss_mode = AUD_DEC_CMPSS_MODE_LINE;
        break;
    case SCC_AUD_CMPSS_MDOE_CUSTOM_1:
        e_drv_aud_cmpss_mode = AUD_DEC_CMPSS_MODE_CUSTOM0;
        break;
    case SCC_AUD_CMPSS_MDOE_CUSTOM_2:
        e_drv_aud_cmpss_mode = AUD_DEC_CMPSS_MODE_CUSTOM1;
        break;
    case SCC_AUD_CMPSS_MDOE_RF:
        e_drv_aud_cmpss_mode = AUD_DEC_CMPSS_MODE_RF;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_CMPSS_MODE,
        (VOID*)e_drv_aud_cmpss_mode,
        sizeof(AUD_DEC_CMPSS_MODE_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_agc
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          b_agc               References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_agc( HANDLE_T                         h_aud,
                         BOOL                             b_agc )
{
    INT32               i4_ret;
    HANDLE_TYPE_T       e_type;
    SCC_AUD_COMP_T*     pt_aud_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tb_agc           = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)b_agc ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_AGC,
        (VOID*)(UINT32)b_agc,
        sizeof( BOOL ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_vdo_picture_off
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          b_is_on             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_vdo_picture_off(
    HANDLE_T                        h_aud,
    BOOL                            b_is_on )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tb_is_on         = %s )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       ((b_is_on)?"TRUE":"FALSE") ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }


    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_VDO_PICTURE_OFF,
       (VOID*)(UINT32)b_is_on,
        sizeof( BOOL ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_mts
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          e_mts               References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_mts( HANDLE_T                  h_aud,
                         SCC_AUD_MTS_T             e_mts )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_MTS_T                e_drv_mts;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\te_mts           = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)e_mts ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch (e_mts)
    {
    case SCC_AUD_MTS_DUAL1:
        e_drv_mts = AUD_DEC_MTS_DUAL1;
        break;
    case SCC_AUD_MTS_DUAL2:
        e_drv_mts = AUD_DEC_MTS_DUAL2;
        break;
    case SCC_AUD_MTS_MONO:
        e_drv_mts = AUD_DEC_MTS_MONO;
        break;
    case SCC_AUD_MTS_STEREO:
        e_drv_mts = AUD_DEC_MTS_STERRO;
        break;
    case SCC_AUD_MTS_SUB_LANG:
        e_drv_mts = AUD_DEC_MTS_SUB_LANG;
        break;
    case SCC_AUD_MTS_NICAM_MONO:
        e_drv_mts = AUD_DEC_MTS_NICAM_MONO;
        break;
    case SCC_AUD_MTS_NICAM_STEREO:
         e_drv_mts = AUD_DEC_MTS_NICAM_STEREO;
        break;
    case SCC_AUD_MTS_NICAM_DUAL1:
        e_drv_mts = AUD_DEC_MTS_NICAM_DUAL1;
        break;
    case SCC_AUD_MTS_NICAM_DUAL2:
        e_drv_mts = AUD_DEC_MTS_NICAM_DUAL2;
        break;
    case SCC_AUD_MTS_FM_MONO:
        e_drv_mts = AUD_DEC_MTS_FM_MONO;
        break;   
    case SCC_AUD_MTS_FM_STEREO:
        e_drv_mts = AUD_DEC_MTS_FM_STEREO;
        break;
        
    case SCC_AUD_MTS_UNKNOWN:
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_MTS,
        (VOID*)e_drv_mts,
        sizeof(AUD_DEC_MTS_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_se_eq_user_lvl
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          pt_eq_user_lvl      References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_se_eq_user_lvl(
    HANDLE_T                        h_aud,
    const SCC_AUD_SE_QA_USER_LVL_T* pt_eq_user_lvl )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_SE_QA_USER_LVL_T     t_drv_se_qa_user_lvl;
    UINT8                        ui1_num_band = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tpt_eq_user_lvl  = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)pt_eq_user_lvl ));

    if ( !pt_eq_user_lvl )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = x_scc_aud_get_se_eq_user_num_band( h_aud, &ui1_num_band );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }
    if ( pt_eq_user_lvl->ui1_band_idx >= ui1_num_band )
    {
        return SMR_INV_ARG;
    }

    t_drv_se_qa_user_lvl.e_eq_type =
        _convert_scc_se_2_drv_se( pt_eq_user_lvl->e_eq_type );
    t_drv_se_qa_user_lvl.ui1_band_idx = pt_eq_user_lvl->ui1_band_idx;
    t_drv_se_qa_user_lvl.i1_level = pt_eq_user_lvl->i1_level;
    t_drv_se_qa_user_lvl.i2_level_ex = 0;
    t_drv_se_qa_user_lvl.b_is_level_ex = FALSE;

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_SE_EQ_USER_LVL,
        (VOID*)&t_drv_se_qa_user_lvl,
        sizeof(AUD_DEC_SE_QA_USER_LVL_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_se_eq_user_lvl_ex
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          pt_eq_user_lvl      References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_se_eq_user_lvl_ex(
    HANDLE_T                        h_aud,
    const SCC_AUD_SE_QA_USER_LVL_T* pt_eq_user_lvl )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_SE_QA_USER_LVL_T     t_drv_se_qa_user_lvl;
    UINT8                        ui1_num_band = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tpt_eq_user_lvl  = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)pt_eq_user_lvl ));

    if ( !pt_eq_user_lvl )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = x_scc_aud_get_se_eq_user_num_band( h_aud, &ui1_num_band );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }
    if ( pt_eq_user_lvl->ui1_band_idx >= ui1_num_band )
    {
        return SMR_INV_ARG;
    }
    if (  !pt_eq_user_lvl->b_is_level_ex )
    {
        return SMR_INV_ARG;
    }
    t_drv_se_qa_user_lvl.e_eq_type =
        _convert_scc_se_2_drv_se( pt_eq_user_lvl->e_eq_type );
    t_drv_se_qa_user_lvl.ui1_band_idx = pt_eq_user_lvl->ui1_band_idx;
    t_drv_se_qa_user_lvl.i1_level = pt_eq_user_lvl->i1_level;
    t_drv_se_qa_user_lvl.i2_level_ex = pt_eq_user_lvl->i2_level_ex;
    t_drv_se_qa_user_lvl.b_is_level_ex = pt_eq_user_lvl->b_is_level_ex;

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_SE_EQ_USER_LVL,
        (VOID*)&t_drv_se_qa_user_lvl,
        sizeof(AUD_DEC_SE_QA_USER_LVL_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_chnl_delay
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_chnl_delay( HANDLE_T                  h_aud,
                                SCC_AUD_CHNL_DELAY_T*     pt_chnl_delay )
{
    INT32                  i4_ret;
    HANDLE_TYPE_T          e_type;
    SCC_AUD_COMP_T*        pt_aud_comp = NULL;
    AUD_DEC_CHNL_DELAY_T   t_drv_chnl_delay;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tpt_chnl_delay   = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pt_chnl_delay ));

    if ( !pt_chnl_delay )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_chnl_delay.e_out_port  = _scc_aud_outport_2_drv_aud_outport(
        pt_chnl_delay->e_out_port );

    if ( pt_chnl_delay->e_ch_delay_type == SCC_AUD_CHNL_DELAY_TYPE_ALL )
    {
        t_drv_chnl_delay.e_ch_delay_type = AUD_DEC_CHNL_DELAY_TYPE_ALL;
        t_drv_chnl_delay.u.ui2_delay = pt_chnl_delay->u.ui2_delay;
    }
    else if ( pt_chnl_delay->e_ch_delay_type == SCC_AUD_CHNL_DELAY_TYPE_INDIVIDUAL_CH )
    {
        t_drv_chnl_delay.e_ch_delay_type = AUD_DEC_CHNL_DELAY_TYPE_INDIVIDUAL_CH;

        /* front left */
        t_drv_chnl_delay.u.t_ind_delay.ui2_fl_delay =
            pt_chnl_delay->u.t_ind_delay.ui2_fl_delay;
        /* front right */
        t_drv_chnl_delay.u.t_ind_delay.ui2_fr_delay =
            pt_chnl_delay->u.t_ind_delay.ui2_fr_delay;
        /* center */
        t_drv_chnl_delay.u.t_ind_delay.ui2_c_delay =
            pt_chnl_delay->u.t_ind_delay.ui2_c_delay;
        /* rear left */
        t_drv_chnl_delay.u.t_ind_delay.ui2_rl_delay =
            pt_chnl_delay->u.t_ind_delay.ui2_rl_delay;
        /* rear right */
        t_drv_chnl_delay.u.t_ind_delay.ui2_rr_delay =
            pt_chnl_delay->u.t_ind_delay.ui2_rr_delay;
        /* sub-woofer */
        t_drv_chnl_delay.u.t_ind_delay.ui2_sw_delay =
            pt_chnl_delay->u.t_ind_delay.ui2_sw_delay;
    }
    else if ( pt_chnl_delay->e_ch_delay_type == SCC_AUD_CHNL_DELAY_TYPE_ON )
    {
        t_drv_chnl_delay.e_ch_delay_type = AUD_DEC_CHNL_DELAY_TYPE_ON;
    }
    else if ( pt_chnl_delay->e_ch_delay_type == SCC_AUD_CHNL_DELAY_TYPE_OFF )
    {
        t_drv_chnl_delay.e_ch_delay_type = AUD_DEC_CHNL_DELAY_TYPE_OFF;
    }
    else
    {
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_CHNL_DELAY,
        (VOID*)&t_drv_chnl_delay,
        sizeof( AUD_DEC_CHNL_DELAY_T ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_hdphone_detect_nfy
 *
 * Description: <function description>
 *
 * Inputs:  h_aud                   Audio Component handle.
 *          pt_hdph_nfy_info        Head Phone notify information.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_hdphone_detect_nfy(
    HANDLE_T                        h_aud,
    SCC_AUD_HDPHONE_DETECT_NFT_INFO_T* pt_hdph_nfy_info )
{
    INT32                            i4_ret;
    HANDLE_TYPE_T                    e_type;
    SCC_AUD_COMP_T*                  pt_aud_comp = NULL;
    AUD_DEC_HDPH_DETECT_NFT_INFO_T   t_drv_hdph_detect_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud             = 0x%.8X\r\n"
                       "\t\tpt_hdph_nfy_info  = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pt_hdph_nfy_info ));

    if ( !pt_hdph_nfy_info )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    pt_aud_comp->pf_aud_hdphone_detect_nfy =
        pt_hdph_nfy_info->pf_hdphone_detect_nfy;
    pt_aud_comp->pf_aud_hdphone_detect_nfy_tag =
        pt_hdph_nfy_info->pv_nfy_tag;

    t_drv_hdph_detect_info.pf_hdph_detect_nfy =
        _scc_comp_aud_hdph_detect_nfy_fct;
    t_drv_hdph_detect_info.pv_nfy_tag = (VOID*)(UINT32)h_aud;

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_HEADPHONE_DETECT_NFY,
        (VOID*)&t_drv_hdph_detect_info,
        sizeof( AUD_DEC_HDPH_DETECT_NFT_INFO_T ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_chnl_delay
 *
 * Description: <function description>
 *
 * Inputs:  h_aud                   Audio Component handle.
 *          ui4_muted_out_port_msk  Muted output port mask.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_mute_ex(
    HANDLE_T                        h_aud,
    UINT32                          ui4_muted_out_port_msk )
{
    INT32                            i4_ret;
    HANDLE_TYPE_T                    e_type;
    SCC_AUD_COMP_T*                  pt_aud_comp = NULL;
    UINT32                           ui4_drv_muted_out_port_msk;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud                  = 0x%.8X\r\n"
                       "\t\tui4_muted_out_port_msk = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)ui4_muted_out_port_msk ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }
    
    ui4_drv_muted_out_port_msk = 0;
    if ( ui4_muted_out_port_msk & SCC_AUD_OUT_PORT_2_CH )
    {
        ui4_drv_muted_out_port_msk |= AUD_DEC_OUT_PORT_FLAG_2_CH;
    }
    if ( ui4_muted_out_port_msk & SCC_AUD_OUT_PORT_5_1_CH )
    {
        ui4_drv_muted_out_port_msk |= AUD_DEC_OUT_PORT_FLAG_5_1_CH;
    }
    if ( ui4_muted_out_port_msk & SCC_AUD_OUT_PORT_SPDIF )
    {
        ui4_drv_muted_out_port_msk |= AUD_DEC_OUT_PORT_FLAG_SPDIF;
    }
    if ( ui4_muted_out_port_msk & SCC_AUD_OUT_PORT_2_CH_BY_PASS )
    {
        ui4_drv_muted_out_port_msk |= AUD_DEC_OUT_PORT_FLAG_2_CH_BY_PASS;
    }
    if ( ui4_muted_out_port_msk & SCC_AUD_OUT_PORT_SPEAKER )
    {
        ui4_ap_muted_out_port_msk |= SCC_AUD_OUT_PORT_SPEAKER;
        ui4_drv_muted_out_port_msk |= AUD_DEC_OUT_PORT_FLAG_SPEAKER;
    }
    else
    {
        ui4_ap_muted_out_port_msk &= ~SCC_AUD_OUT_PORT_SPEAKER;
    }
    if ( ui4_muted_out_port_msk & SCC_AUD_OUT_PORT_HEADPHONE )
    {
        ui4_ap_muted_out_port_msk |= SCC_AUD_OUT_PORT_HEADPHONE;
        ui4_drv_muted_out_port_msk |= AUD_DEC_OUT_PORT_FLAG_HEADPHONE;
    }
    else
    {
        ui4_ap_muted_out_port_msk &= ~AUD_DEC_OUT_PORT_FLAG_HEADPHONE;
    }
    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_MUTE_EX,
        (VOID*)&ui4_drv_muted_out_port_msk,
        sizeof( UINT32 ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_switch_ex
 *
 * Description: <function description>
 *
 * Inputs:  h_aud                   Audio Component handle.
 *          ui4_muted_out_port_msk  Muted output port mask.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_switch_ex(
    HANDLE_T                        h_aud,
    UINT32                          ui4_muted_out_port_msk )
{
    INT32                            i4_ret;
    HANDLE_TYPE_T                    e_type;
    SCC_AUD_COMP_T*                  pt_aud_comp = NULL;
    UINT32                           ui4_drv_muted_out_port_msk;
    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud                  = 0x%.8X\r\n"
                       "\t\tui4_muted_out_port_msk = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)ui4_muted_out_port_msk ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }
    
    ui4_drv_muted_out_port_msk = 0;
    if ( ui4_muted_out_port_msk & SCC_AUD_OUT_PORT_2_CH )
    {
        ui4_drv_muted_out_port_msk |= AUD_DEC_OUT_PORT_FLAG_2_CH;
    }
    if ( ui4_muted_out_port_msk & SCC_AUD_OUT_PORT_5_1_CH )
    {
        ui4_drv_muted_out_port_msk |= AUD_DEC_OUT_PORT_FLAG_5_1_CH;
    }
    if ( ui4_muted_out_port_msk & SCC_AUD_OUT_PORT_SPDIF )
    {
        ui4_drv_muted_out_port_msk |= AUD_DEC_OUT_PORT_FLAG_SPDIF;
    }
    if ( ui4_muted_out_port_msk & SCC_AUD_OUT_PORT_2_CH_BY_PASS )
    {
        ui4_drv_muted_out_port_msk |= AUD_DEC_OUT_PORT_FLAG_2_CH_BY_PASS;
    }
    if ( ui4_muted_out_port_msk & SCC_AUD_OUT_PORT_SPEAKER )
    {
        ui4_drv_muted_out_port_msk |= AUD_DEC_OUT_PORT_FLAG_SPEAKER;
    }
    else /*unmute speaker*/
    {
        if(ui4_ap_muted_out_port_msk & SCC_AUD_OUT_PORT_SPEAKER )
        {
            ui4_drv_muted_out_port_msk |= AUD_DEC_OUT_PORT_FLAG_SPEAKER;
        }
    }
    if ( ui4_muted_out_port_msk & SCC_AUD_OUT_PORT_HEADPHONE )
    {
        ui4_drv_muted_out_port_msk |= AUD_DEC_OUT_PORT_FLAG_HEADPHONE;
    }
    else/*unmute headphone*/
    {
        if(ui4_ap_muted_out_port_msk & SCC_AUD_OUT_PORT_HEADPHONE )
        {
            ui4_drv_muted_out_port_msk |= AUD_DEC_OUT_PORT_FLAG_HEADPHONE;
        }
    }
    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_MUTE_EX,
        (VOID*)&ui4_drv_muted_out_port_msk,
        sizeof( UINT32 ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_mute_single_port
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          e_port              References.
 *          b_is_active         References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_mute_single_port( 
    HANDLE_T                 h_aud,
    SCC_AUD_OUT_PORT_T       e_port,
    BOOL                     b_is_active )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    UINT32                       ui4_drv_out_port_mask = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud                  = 0x%.8X\r\n"
                       "\t\te_port                 = %d\r\n"
                       "\t\tb_is_active            = %s )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)e_port,
                       (b_is_active)?"TRUE":"FALSE" ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(UINT32);
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_MUTE_EX,
        (VOID*)&ui4_drv_out_port_mask,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    switch ( e_port )
    {
    case SCC_AUD_OUT_PORT_OFF:
        return SMR_OK;

    case SCC_AUD_OUT_PORT_2_CH:
        if ( b_is_active )
        {
            ui4_drv_out_port_mask |= AUD_DEC_OUT_PORT_FLAG_2_CH;
        }
        else
        {
            ui4_drv_out_port_mask &= ~AUD_DEC_OUT_PORT_FLAG_2_CH;
        }
        break;
    case SCC_AUD_OUT_PORT_5_1_CH:
        if ( b_is_active )
        {
            ui4_drv_out_port_mask |= AUD_DEC_OUT_PORT_FLAG_5_1_CH;
        }
        else
        {
            ui4_drv_out_port_mask &= ~AUD_DEC_OUT_PORT_FLAG_5_1_CH;
        }
        break;
    case SCC_AUD_OUT_PORT_SPDIF:
        if ( b_is_active )
        {
            ui4_drv_out_port_mask |= AUD_DEC_OUT_PORT_FLAG_SPDIF;
        }
        else
        {
            ui4_drv_out_port_mask &= ~AUD_DEC_OUT_PORT_FLAG_SPDIF;
        }
        break;
    case SCC_AUD_OUT_PORT_2_CH_BY_PASS:
        if ( b_is_active )
        {
            ui4_drv_out_port_mask |= AUD_DEC_OUT_PORT_FLAG_2_CH_BY_PASS;
        }
        else
        {
            ui4_drv_out_port_mask &= ~AUD_DEC_OUT_PORT_FLAG_2_CH_BY_PASS;
        }
        break;
    case SCC_AUD_OUT_PORT_SPEAKER:
        if ( b_is_active )
        {
            ui4_drv_out_port_mask |= AUD_DEC_OUT_PORT_FLAG_SPEAKER;
            ui4_ap_muted_out_port_msk |= SCC_AUD_OUT_PORT_SPEAKER;
        }
        else
        {
            ui4_drv_out_port_mask &= ~AUD_DEC_OUT_PORT_FLAG_SPEAKER;
            ui4_ap_muted_out_port_msk &= ~SCC_AUD_OUT_PORT_SPEAKER;
        }
        break;
    case SCC_AUD_OUT_PORT_HEADPHONE:
        if ( b_is_active )
        {
            ui4_drv_out_port_mask |= AUD_DEC_OUT_PORT_FLAG_HEADPHONE;
            ui4_ap_muted_out_port_msk |= SCC_AUD_OUT_PORT_HEADPHONE;
        }
        else
        {
            ui4_drv_out_port_mask &= ~AUD_DEC_OUT_PORT_FLAG_HEADPHONE;
            ui4_ap_muted_out_port_msk &= ~SCC_AUD_OUT_PORT_HEADPHONE;
        }
        break;
    case SCC_AUD_OUT_PORT_TVSCART:
        if ( b_is_active )
        {
            ui4_drv_out_port_mask |= AUD_DEC_OUT_PORT_FLAG_TVSCART;
        }
        else
        {
            ui4_drv_out_port_mask &= ~AUD_DEC_OUT_PORT_FLAG_TVSCART;
        }
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_MUTE_EX,
        (VOID*)&ui4_drv_out_port_mask,
        sizeof(UINT32) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_volume
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pt_volume           References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_volume( HANDLE_T                      h_aud,
                            SCC_AUD_VOLUME_INFO_T*        pt_volume )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_VOLUME_INFO_T        t_aud_vol_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tpt_volume       = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pt_volume ));

    if ( !pt_volume )
    {
        return SMR_INV_ARG;
    }

    if ((pt_volume->e_out_port == SCC_AUD_OUT_PORT_2_CH) &&
        (pt_volume->e_ch != SCC_AUD_CHANNEL_FRONT_LEFT) &&
        (pt_volume->e_ch != SCC_AUD_CHANNEL_FRONT_RIGHT) &&
        (pt_volume->e_ch != SCC_AUD_CHANNEL_ALL))
    {
        SM_SLCTR_DBG_ERR((
            "[SCC] %s:%d, ERROR!! SCC_AUD_OUT_PORT_2_CH with invalid channel = %u.\r\n",
            __FUNCTION__, __LINE__,
            pt_volume->e_ch ));
        return SMR_INV_GET_INFO;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    /* Set input parameters */

    if ( pt_volume->e_ch == SCC_AUD_CHANNEL_ALL )
    {
        t_aud_vol_info.e_vol_type = AUD_DEC_ALL_CH;
    }
    else
    {
        t_aud_vol_info.e_vol_type = AUD_DEC_INDIVIDUAL_CH;

        t_aud_vol_info.u.t_ch_vol.e_out_port =
            _scc_aud_outport_2_drv_aud_outport( pt_volume->e_out_port );
        t_aud_vol_info.u.t_ch_vol.e_ls =
            _scc_aud_ch_2_drv_aud_ch( pt_volume->e_ch );
    }

    z_size = sizeof(AUD_DEC_VOLUME_INFO_T);
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_VOLUME,
        (VOID*)&t_aud_vol_info,
        &z_size );
    if ( i4_ret != SMR_OK || (z_size != sizeof(AUD_DEC_VOLUME_INFO_T)) )
    {
        return SMR_INV_GET_INFO;
    }

    if ( pt_volume->e_ch == SCC_AUD_CHANNEL_ALL )
    {
        pt_volume->ui1_volumn = t_aud_vol_info.u.ui1_level;
    }
    else
    {
        pt_volume->ui1_volumn = t_aud_vol_info.u.t_ch_vol.ui1_level;
    }
    if ( pt_volume->ui1_volumn > 100 )
    {
        pt_volume->ui1_volumn = 100;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_is_muted
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pb_is_muted         References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_is_muted( HANDLE_T                        h_aud,
                          BOOL*                           pb_is_muted )
{
    INT32                        i4_ret;
#if 0
    SIZE_T                       z_size = 0;
#endif
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
#if 0
    AUD_DEC_MUTE_TYPE_T          e_aud_dec_mute_type;
#endif

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tpb_is_muted     = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pb_is_muted ));

    if ( !pb_is_muted )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

#if 0
    z_size = sizeof(AUD_DEC_MUTE_TYPE_T);
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_MUTE,
        (VOID*)&e_aud_dec_mute_type,
        &z_size );
    if ( i4_ret != SMR_OK || (z_size != sizeof(AUD_DEC_MUTE_TYPE_T)) )
    {
        return SMR_INV_GET_INFO;
    }

    switch (e_aud_dec_mute_type)
    {
    case AUD_DEC_MUTE_ON:
        *pb_is_muted = TRUE;
        break;
    case AUD_DEC_MUTE_OFF:
    default:
        *pb_is_muted = FALSE;
        break;
    }
#else
    *pb_is_muted = _b_mute_2;
#endif

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_spdif_fmt
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pe_spdif_fmt        References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_spdif_fmt( HANDLE_T                   h_aud,
                               SCC_AUD_SPDIF_FMT_T*       pe_spdif_fmt )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_SPDIF_TYPE_T         e_aud_dec_spdif_type;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tpe_spdif_fmt    = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pe_spdif_fmt ));

    if ( !pe_spdif_fmt )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(AUD_DEC_SPDIF_TYPE_T);
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_SPDIF,
        (VOID*)&e_aud_dec_spdif_type,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch (e_aud_dec_spdif_type)
    {
    case AUD_DEC_SPDIF_OFF:
        *pe_spdif_fmt = SCC_AUD_SPDIF_FMT_OFF;
        break;
    case AUD_DEC_SPDIF_RAW:
        *pe_spdif_fmt = SCC_AUD_SPDIF_FMT_RAW;
        break;
    case AUD_DEC_SPDIF_PCM_16:
        *pe_spdif_fmt = SCC_AUD_SPDIF_FMT_PCM_16;
        break;
    case AUD_DEC_SPDIF_PCM_24:
        *pe_spdif_fmt = SCC_AUD_SPDIF_FMT_PCM_24;
        break;
    default:
        return SMR_INV_GET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_hdcd_filter
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pe_hdcd_fltr        References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_hdcd_filter( HANDLE_T                 h_aud,
                                 SCC_AUD_HDCD_FLTR_T*     pe_hdcd_fltr )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_HDCD_FLTR_T          e_aud_dec_hdcd_fltr;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud         = 0x%.8X\r\n"
                       "\t\tpe_hdcd_fltr  = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pe_hdcd_fltr ));

    if ( !pe_hdcd_fltr )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(AUD_DEC_HDCD_FLTR_T);
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_HDCD_FLTR,
        (VOID*)&e_aud_dec_hdcd_fltr,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch (e_aud_dec_hdcd_fltr)
    {
    case AUD_DEC_HDCD_FLTR_OFF:
        *pe_hdcd_fltr = SCC_AUD_HDCD_FLTR_OFF;
        break;
    case AUD_DEC_HDCD_FLTR_1X:
        *pe_hdcd_fltr = SCC_AUD_HDCD_FLTR_1X;
        break;
    case AUD_DEC_HDCD_FLTR_2X:
        *pe_hdcd_fltr = SCC_AUD_HDCD_FLTR_2X;
        break;
    default:
        return SMR_INV_GET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_loudspeaker
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pt_ls_type          References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_loudspeaker( HANDLE_T                 h_aud,
                                 SCC_AUD_LOUD_SPEAKER_T*  pt_ls_type )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_CH_LS_MODE_T         t_aud_ch_ls_mode;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud         = 0x%.8X\r\n"
                       "\t\tpt_ls_type    = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pt_ls_type ));

    if ( !pt_ls_type )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch( pt_ls_type->e_channel )
    {
    case SCC_AUD_CHANNEL_ALL:
        t_aud_ch_ls_mode.e_ls = AUD_DEC_LS_FRONT_LEFT; /*get one channel*/
        break;
    case SCC_AUD_CHANNEL_FRONT_LEFT:
        t_aud_ch_ls_mode.e_ls = AUD_DEC_LS_FRONT_LEFT;
        break;
    case SCC_AUD_CHANNEL_FRONT_RIGHT:
        t_aud_ch_ls_mode.e_ls = AUD_DEC_LS_FRONT_RIGHT;
        break;
    case SCC_AUD_CHANNEL_REAR_LEFT:
        t_aud_ch_ls_mode.e_ls = AUD_DEC_LS_REAR_LEFT;
        break;
    case SCC_AUD_CHANNEL_REAR_RIGHT:
        t_aud_ch_ls_mode.e_ls = AUD_DEC_LS_REAR_RIGHT;
        break;
    case SCC_AUD_CHANNEL_CENTER:
        t_aud_ch_ls_mode.e_ls = AUD_DEC_LS_CENTER;
        break;
    case SCC_AUD_CHANNEL_SUB_WOOFER:
        t_aud_ch_ls_mode.e_ls = AUD_DEC_LS_SUB_WOOFER;
        break;
    default:
        return SMR_INV_GET_INFO;
    }

    z_size = sizeof(AUD_DEC_CH_LS_MODE_T);
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_LS_MODE,
        (VOID*)&t_aud_ch_ls_mode,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch (t_aud_ch_ls_mode.e_mode)
    {
    case AUD_DEC_LS_MODE_OFF:
        pt_ls_type->e_mode = SCC_AUD_LOUD_SPEAKER_MODE_OFF;
        break;
    case AUD_DEC_LS_MODE_LARGE_FREQ_RANGE:
        pt_ls_type->e_mode = SCC_AUD_LOUD_SPEAKER_MODE_LARGE_FREQ_RANGE;
        break;
    case AUD_DEC_LS_MODE_SMALL_FREQ_RANGE:
        pt_ls_type->e_mode = SCC_AUD_LOUD_SPEAKER_MODE_SMALL_FREQ_RANGE;
        break;
    default:
        return SMR_INV_GET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_se_trim
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pt_se_trim          References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_se_trim(
    HANDLE_T                        h_aud,
    SCC_AUD_SE_TRIM_T*              pt_se_trim )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_SE_INFO_T            t_drv_se_info;
    AUD_DEC_CH_TRIM_T            t_drv_se_ch_trim;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tpt_se_trim      = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pt_se_trim ));

    if ( !pt_se_trim )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_se_info.e_se_type = AUD_DEC_SE_TRIM;
    t_drv_se_info.pv_info = (VOID*)&t_drv_se_ch_trim;

    z_size = sizeof( AUD_DEC_SE_INFO_T );
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_SOUND_EFFECT,
        (VOID*)&t_drv_se_info,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    pt_se_trim->e_channel = _drv_aud_ch_2_scc_aud_ch( t_drv_se_ch_trim.e_ls );
    pt_se_trim->ui1_level = t_drv_se_ch_trim.ui1_level;
    if ( pt_se_trim->ui1_level > 100 )
    {
        pt_se_trim->ui1_level = 100;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_se_superbase
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pb_is_on            References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_se_superbase(
    HANDLE_T                        h_aud,
    BOOL*                           pb_is_on )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_SE_INFO_T            t_drv_se_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tpb_is_on        = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pb_is_on ));

    if ( !pb_is_on )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_se_info.e_se_type = AUD_DEC_SE_SUPERBASS;
    t_drv_se_info.pv_info = NULL;

    z_size = sizeof( AUD_DEC_SE_INFO_T );
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_SOUND_EFFECT,
        (VOID*)&t_drv_se_info,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    *pb_is_on = (( t_drv_se_info.pv_info )? TRUE: FALSE);

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_se_equalizer
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pt_se_trim          References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_se_equalizer(
    HANDLE_T                        h_aud,
    SCC_AUD_SE_EQ_T*                pe_equalizer )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_SE_INFO_T            t_drv_se_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tpe_equalizer    = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pe_equalizer ));

    if ( !pe_equalizer )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_se_info.e_se_type = AUD_DEC_SE_EQUALIZER;
    t_drv_se_info.pv_info = NULL;

    z_size = sizeof( AUD_DEC_SE_INFO_T );
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_SOUND_EFFECT,
        (VOID*)&t_drv_se_info,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    *pe_equalizer = _convert_drv_se_2_scc_se(
        (AUD_DEC_EQ_TYPE_T)(UINT32)t_drv_se_info.pv_info );

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_se_reverb
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pe_reverb           References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_se_reverb(
    HANDLE_T                        h_aud,
    SCC_AUD_SE_REVERB_MODE_T*       pe_reverb )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_SE_INFO_T            t_drv_se_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tpe_reverb       = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pe_reverb ));

    if ( !pe_reverb )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_se_info.e_se_type = AUD_DEC_SE_REVERB;
    t_drv_se_info.pv_info = NULL;

    z_size = sizeof( AUD_DEC_SE_INFO_T );
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_SOUND_EFFECT,
        (VOID*)&t_drv_se_info,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch( (AUD_DEC_REVERB_MODE_T)(UINT32)t_drv_se_info.pv_info )
    {
    case AUD_DEC_REVERB_OFF:
        *pe_reverb = SCC_AUD_SE_REVERB_OFF;
        break;
    case AUD_DEC_REVERB_CONCERT:
        *pe_reverb = SCC_AUD_SE_REVERB_CONCERT;
        break;
    case AUD_DEC_REVERB_LIVINGROOM:
        *pe_reverb = SCC_AUD_SE_REVERB_LIVINGROOM;
        break;
    case AUD_DEC_REVERB_HALL:
        *pe_reverb = SCC_AUD_SE_REVERB_HALL;
        break;
    case AUD_DEC_REVERB_BATHROOM:
        *pe_reverb = SCC_AUD_SE_REVERB_BATHROOM;
        break;
    case AUD_DEC_REVERB_CAVE:
        *pe_reverb = SCC_AUD_SE_REVERB_CAVE;
        break;
    case AUD_DEC_REVERB_ARENA:
        *pe_reverb = SCC_AUD_SE_REVERB_ARENA;
        break;
    case AUD_DEC_REVERB_CHURCH:
        *pe_reverb = SCC_AUD_SE_REVERB_CHURCH;
        break;
    default:
        return SMR_INV_GET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_se_base
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pui1_base           References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_se_base(
    HANDLE_T                        h_aud,
    UINT8*                          pui1_base )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_SE_INFO_T            t_drv_se_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tpui1_base       = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pui1_base ));

    if ( !pui1_base )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_se_info.e_se_type = AUD_DEC_SE_BASS;
    t_drv_se_info.pv_info = NULL;

    z_size = sizeof( AUD_DEC_SE_INFO_T );
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_SOUND_EFFECT,
        (VOID*)&t_drv_se_info,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    *pui1_base = (UINT8)(UINT32)t_drv_se_info.pv_info;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_se_treble
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pui1_base           References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_se_treble(
    HANDLE_T                        h_aud,
    UINT8*                          pui1_treble )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_SE_INFO_T            t_drv_se_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tpui1_treble     = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pui1_treble ));

    if ( !pui1_treble )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_se_info.e_se_type = AUD_DEC_SE_TREBLE;
    t_drv_se_info.pv_info = NULL;

    z_size = sizeof( AUD_DEC_SE_INFO_T );
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_SOUND_EFFECT,
        (VOID*)&t_drv_se_info,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    *pui1_treble = (UINT8)(UINT32)t_drv_se_info.pv_info;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_se_balance
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pui1_balance        References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_se_balance(
    HANDLE_T                        h_aud,
    UINT8*                          pui1_balance )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_SE_INFO_T            t_drv_se_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tpui1_balance    = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pui1_balance ));

    if ( !pui1_balance )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_se_info.e_se_type = AUD_DEC_SE_BALANCE;
    t_drv_se_info.pv_info = NULL;

    z_size = sizeof( AUD_DEC_SE_INFO_T );
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_SOUND_EFFECT,
        (VOID*)&t_drv_se_info,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    *pui1_balance = (UINT8)(UINT32)t_drv_se_info.pv_info;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_se_postdr
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pb_is_on            References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_se_postdr(
    HANDLE_T                        h_aud,
    BOOL*                           pb_is_on )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_SE_INFO_T            t_drv_se_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tpb_is_on        = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pb_is_on ));

    if ( !pb_is_on )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_se_info.e_se_type = AUD_DEC_SE_POSTDRC;
    t_drv_se_info.pv_info = NULL;

    z_size = sizeof( AUD_DEC_SE_INFO_T );
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_SOUND_EFFECT,
        (VOID*)&t_drv_se_info,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    *pb_is_on = (( t_drv_se_info.pv_info )? TRUE: FALSE);

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_se_volume
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pt_volume           References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_se_volume(
    HANDLE_T                        h_aud,
    SCC_AUD_VOLUME_INFO_T*          pt_volume )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_SE_INFO_T            t_drv_se_info;
    AUD_DEC_CH_VOLUME_T          t_drv_volume;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tpt_volume       = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pt_volume ));

    if ( !pt_volume )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_se_info.e_se_type = AUD_DEC_SE_VOLUME;
    t_drv_se_info.pv_info = (VOID*)&t_drv_volume;

    t_drv_volume.e_out_port = _scc_aud_outport_2_drv_aud_outport(
        pt_volume->e_out_port );
    t_drv_volume.e_ls = _scc_aud_ch_2_drv_aud_ch( pt_volume->e_ch );

    z_size = sizeof( AUD_DEC_SE_INFO_T );
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_SOUND_EFFECT,
        (VOID*)&t_drv_se_info,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    _drv_ch_volume_2_scc_aud_volume( &t_drv_volume, pt_volume );

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_se_surround
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pb_is_on            References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_se_surround(
    HANDLE_T                        h_aud,
    BOOL*                           pb_is_on )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_SE_INFO_T            t_drv_se_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tpb_is_on        = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pb_is_on ));

    if ( !pb_is_on )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_se_info.e_se_type = AUD_DEC_SE_SURROUND;
    t_drv_se_info.pv_info = NULL;

    z_size = sizeof( AUD_DEC_SE_INFO_T );
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_SOUND_EFFECT,
        (VOID*)&t_drv_se_info,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    *pb_is_on = (( t_drv_se_info.pv_info )? TRUE: FALSE);

    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_se_superwoofer
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pb_is_on            References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_se_superwoofer(
    HANDLE_T                        h_aud,
    BOOL*                           pb_is_on )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_SE_INFO_T            t_drv_se_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tpb_is_on        = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pb_is_on ));

    if ( !pb_is_on )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_se_info.e_se_type = AUD_DEC_SE_SUPERWOOFER;
    t_drv_se_info.pv_info = NULL;

    z_size = sizeof( AUD_DEC_SE_INFO_T );
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_SOUND_EFFECT,
        (VOID*)&t_drv_se_info,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    *pb_is_on = (( t_drv_se_info.pv_info )? TRUE: FALSE);

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_downmix_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pe_dm_mode          References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_downmix_mode( HANDLE_T                h_aud,
                                  SCC_AUD_DOWNMIX_MODE_T* pe_dm_mode )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_DOWNMIX_MODE_T       e_aud_dec_dm_mode;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud         = 0x%.8X\r\n"
                       "\t\tpe_dm_mode    = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pe_dm_mode ));

    if ( !pe_dm_mode )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(AUD_DEC_DOWNMIX_MODE_T);
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_DOWNMIX,
        (VOID*)&e_aud_dec_dm_mode,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch (e_aud_dec_dm_mode)
    {
    case AUD_DEC_DM_OFF:
        *pe_dm_mode = SCC_AUD_DOWNMIX_MODE_OFF;
        break;
    case AUD_DEC_DM_LT_RT:
        *pe_dm_mode = SCC_AUD_DOWNMIX_MODE_LT_RT;
        break;
    case AUD_DEC_DM_STEREO:
        *pe_dm_mode = SCC_AUD_DOWNMIX_MODE_STEREO;
        break;
    case AUD_DEC_DM_VIR_SURR:
        *pe_dm_mode = SCC_AUD_DOWNMIX_MODE_VIR_SURR;
        break;
    case AUD_DEC_DM_MONO:
        *pe_dm_mode = SCC_AUD_DOWNMIX_MODE_MONO;
        break;
    case AUD_DEC_DM_DUAL1:
        *pe_dm_mode = SCC_AUD_DOWNMIX_MODE_DUAL1;
        break;
    case AUD_DEC_DM_DUAL2:
        *pe_dm_mode = SCC_AUD_DOWNMIX_MODE_DUAL2;
        break;
    case AUD_DEC_DM_DUAL_MIX:
        *pe_dm_mode = SCC_AUD_DOWNMIX_MODE_DUAL_MIX;
        break;
    case AUD_DEC_DM_3_STEREO:
        *pe_dm_mode = SCC_AUD_DOWNMIX_MODE_3_STEREO;
        break;
    case AUD_DEC_DM_DUAL_OFF:
        *pe_dm_mode = SCC_AUD_DOWNMIX_MODE_DUAL_OFF;
        break;
    default:
        return SMR_INV_GET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_dolby_drc
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pe_drc              References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_dolby_drc( HANDLE_T                   h_aud,
                               SCC_AUD_DOLBY_DRC_T*       pe_drc )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_DRC_T                e_aud_dec_drc;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud         = 0x%.8X\r\n"
                       "\t\tpe_drc        = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pe_drc ));

    if ( !pe_drc )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(AUD_DEC_DRC_T);
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_DRC,
        (VOID*)&e_aud_dec_drc,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch (e_aud_dec_drc)
    {
    case AUD_DEC_DRC_OFF:
        *pe_drc = SCC_AUD_DOLBY_DRC_OFF;
        break;
    case AUD_DEC_DRC_10:
        *pe_drc = SCC_AUD_DOLBY_DRC_10;
        break;
    case AUD_DEC_DRC_20:
        *pe_drc = SCC_AUD_DOLBY_DRC_20;
        break;
    case AUD_DEC_DRC_30:
        *pe_drc = SCC_AUD_DOLBY_DRC_30;
        break;
    case AUD_DEC_DRC_40:
        *pe_drc = SCC_AUD_DOLBY_DRC_40;
        break;
    case AUD_DEC_DRC_50:
        *pe_drc = SCC_AUD_DOLBY_DRC_50;
        break;
    case AUD_DEC_DRC_60:
        *pe_drc = SCC_AUD_DOLBY_DRC_60;
        break;
    case AUD_DEC_DRC_70:
        *pe_drc = SCC_AUD_DOLBY_DRC_70;
        break;
	case AUD_DEC_DRC_80:
		*pe_drc = SCC_AUD_DOLBY_DRC_80;
		break;
	case AUD_DEC_DRC_90:
		*pe_drc = SCC_AUD_DOLBY_DRC_90;
		break;
    case AUD_DEC_DRC_100:
        *pe_drc = SCC_AUD_DOLBY_DRC_100;
        break;
    default:
        return SMR_INV_GET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_cmpss_drc
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pe_drc              References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_cmpss_drc( HANDLE_T                   h_aud,
                                      SCC_AUD_CMPSS_DRC_MODE_T*  pe_cmpss_drc )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_CMPSS_DRC_MODE_T     e_drv_cmpss_drc;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud         = 0x%.8X\r\n"
                       "\t\tpe_cmpss_drc  = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pe_cmpss_drc ));

    if ( !pe_cmpss_drc )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(AUD_DEC_CMPSS_DRC_MODE_T);
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_CMPSS_DRC,
        (VOID*)&e_drv_cmpss_drc,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch (e_drv_cmpss_drc)
    {
    case AUD_DEC_CMPSS_DRC_MODE_COMPRESSED:
        *pe_cmpss_drc = SCC_AUD_CMPSS_DRC_MODE_COMPRESSED;
        break;
    case AUD_DEC_CMPSS_DRC_MODE_STANDARD:
        *pe_cmpss_drc = SCC_AUD_CMPSS_DRC_MODE_STANDARD;
        break;
    case AUD_DEC_CMPSS_DRC_MODE_OFF:
        *pe_cmpss_drc = SCC_AUD_CMPSS_DRC_MODE_OFF;
        break;
    default:
        return SMR_INV_GET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_output_port
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          e_port              References.
 *
 * Outputs: pb_is_active        References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_output_port( HANDLE_T                 h_aud,
                                 SCC_AUD_OUT_PORT_T       e_port,
                                 BOOL*                    pb_is_active )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    UINT32                       ui4_drv_out_port_mask = 0;
    UINT32                       ui4_drv_out_port = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud         = 0x%.8X\r\n"
                       "\t\te_port        = 0x%.8X\r\n"
                       "\t\tpb_is_active  = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)e_port,
                       (UINT32)pb_is_active ));

    if ( !pb_is_active )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(UINT32);
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_OUT_PORT,
        (VOID*)&ui4_drv_out_port_mask,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch( e_port )
    {
    case SCC_AUD_OUT_PORT_2_CH:
        ui4_drv_out_port = AUD_DEC_OUT_PORT_FLAG_2_CH;
        break;
    case SCC_AUD_OUT_PORT_5_1_CH:
        ui4_drv_out_port = AUD_DEC_OUT_PORT_FLAG_5_1_CH;
        break;
    case SCC_AUD_OUT_PORT_SPDIF:
        ui4_drv_out_port = AUD_DEC_OUT_PORT_FLAG_SPDIF;
        break;
    case SCC_AUD_OUT_PORT_2_CH_BY_PASS:
        ui4_drv_out_port = AUD_DEC_OUT_PORT_FLAG_2_CH_BY_PASS;
        break;
    case SCC_AUD_OUT_PORT_SPEAKER:
        ui4_drv_out_port = AUD_DEC_OUT_PORT_FLAG_SPEAKER;
        break;
    case SCC_AUD_OUT_PORT_HEADPHONE:
        ui4_drv_out_port = AUD_DEC_OUT_PORT_FLAG_HEADPHONE;
        break;
    case SCC_AUD_OUT_PORT_TVSCART:
        ui4_drv_out_port = AUD_DEC_OUT_PORT_FLAG_TVSCART;
        break;
    case SCC_AUD_OUT_PORT_OFF:
        ui4_drv_out_port = AUD_DEC_OUT_PORT_FLAG_NONE;
        break;
    default:
        return SMR_INV_GET_INFO;
    }

    if ( ui4_drv_out_port_mask & ui4_drv_out_port )
    {
        *pb_is_active = TRUE;
    }
    else
    {
        *pb_is_active = FALSE;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_cmpss_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pe_cmpss_mode       References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_cmpss_mode( HANDLE_T                  h_aud,
                                SCC_AUD_CMPSS_MDOE_T*     pe_cmpss_mode )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_CMPSS_MODE_T         e_drv_aud_cmpss_mode;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud         = 0x%.8X\r\n"
                       "\t\tpe_cmpss_mode = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pe_cmpss_mode ));

    if ( !pe_cmpss_mode )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(AUD_DEC_CMPSS_MODE_T);
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_CMPSS_MODE,
        (VOID*)&e_drv_aud_cmpss_mode,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch( e_drv_aud_cmpss_mode )
    {
    case AUD_DEC_CMPSS_MODE_LINE:
        *pe_cmpss_mode = SCC_AUD_CMPSS_MDOE_LINE;
        break;
    case AUD_DEC_CMPSS_MODE_CUSTOM0:
        *pe_cmpss_mode = SCC_AUD_CMPSS_MDOE_CUSTOM_1;
        break;
    case AUD_DEC_CMPSS_MODE_CUSTOM1:
        *pe_cmpss_mode = SCC_AUD_CMPSS_MDOE_CUSTOM_2;
        break;
    case AUD_DEC_CMPSS_MODE_RF:
        *pe_cmpss_mode = SCC_AUD_CMPSS_MDOE_RF;
        break;
    default:
        return SMR_INV_GET_INFO;
    }

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_mpeg_level_adj
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pui1_level       References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_mpeg_level_adj( HANDLE_T                  h_aud,
                                    INT8*                 pi1_level )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud         = 0x%.8X\r\n"
                       "\t\tpi1_level = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pi1_level ));

    if ( !pi1_level )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(UINT8);
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_MPEG_LEVEL_ADJ,
        (VOID*)pi1_level,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    return SMR_OK;
}



/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_agc
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pb_agc              References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_agc( HANDLE_T                         h_aud,
                         BOOL*                            pb_agc )
{
    INT32               i4_ret;
    SIZE_T              z_size = 0;
    HANDLE_TYPE_T       e_type;
    SCC_AUD_COMP_T*     pt_aud_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud         = 0x%.8X\r\n"
                       "\t\tpb_agc        = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pb_agc ));

    if ( !pb_agc )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof( BOOL );
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_AGC,
        (VOID*)pb_agc,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_chnl_delay
 *
 * Description: <function description>
 *
 * Inputs:  h_aud           Audio component handle.
 *
 * Outputs: pt_chnl_delay   References
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_chnl_delay( HANDLE_T                  h_aud,
                                SCC_AUD_CHNL_DELAY_T*     pt_chnl_delay )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_CHNL_DELAY_T         t_drv_chnl_delay;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud         = 0x%.8X\r\n"
                       "\t\tpt_chnl_delay = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pt_chnl_delay ));

    if ( !pt_chnl_delay )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }


    t_drv_chnl_delay.e_ch_delay_type = _scc_aud_ch_delay_type_2_drv_aud_ch_delay_type(
        pt_chnl_delay->e_ch_delay_type);
    t_drv_chnl_delay.e_out_port  = _scc_aud_outport_2_drv_aud_outport(
        pt_chnl_delay->e_out_port );

    z_size = sizeof( AUD_DEC_CHNL_DELAY_T );
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_CHNL_DELAY,
        (VOID*)&t_drv_chnl_delay,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    if ( t_drv_chnl_delay.e_ch_delay_type == AUD_DEC_CHNL_DELAY_TYPE_ALL )
    {
        pt_chnl_delay->e_ch_delay_type = SCC_AUD_CHNL_DELAY_TYPE_ALL;
        pt_chnl_delay->u.ui2_delay = t_drv_chnl_delay.u.ui2_delay;
    }
    else if ( t_drv_chnl_delay.e_ch_delay_type == AUD_DEC_CHNL_DELAY_TYPE_INDIVIDUAL_CH )
    {
        pt_chnl_delay->e_ch_delay_type = SCC_AUD_CHNL_DELAY_TYPE_INDIVIDUAL_CH;
        pt_chnl_delay->u.t_ind_delay.ui2_fl_delay = t_drv_chnl_delay.u.t_ind_delay.ui2_fl_delay; /* front left */
        pt_chnl_delay->u.t_ind_delay.ui2_fr_delay = t_drv_chnl_delay.u.t_ind_delay.ui2_fr_delay; /* front right */
        pt_chnl_delay->u.t_ind_delay.ui2_c_delay = t_drv_chnl_delay.u.t_ind_delay.ui2_c_delay;  /* center */
        pt_chnl_delay->u.t_ind_delay.ui2_rl_delay = t_drv_chnl_delay.u.t_ind_delay.ui2_rl_delay; /* rear left */
        pt_chnl_delay->u.t_ind_delay.ui2_rr_delay = t_drv_chnl_delay.u.t_ind_delay.ui2_rr_delay; /* rear right */
        pt_chnl_delay->u.t_ind_delay.ui2_sw_delay = t_drv_chnl_delay.u.t_ind_delay.ui2_sw_delay; /* sub-woofer */
    }
    else
    {
        return SMR_INV_GET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_max_chnl_delay
 *
 * Description: <function description>
 *
 * Inputs:  h_aud           Audio component handle.
 *
 * Outputs: pui2_max_delay  References
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_max_chnl_delay( HANDLE_T              h_aud,
                                    UINT16*               pui2_max_delay )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud          = 0x%.8X\r\n"
                       "\t\tpui2_max_delay = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pui2_max_delay ));

    if ( !pui2_max_delay )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(UINT16);
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_MAX_CHNL_DELAY,
        (VOID*)pui2_max_delay,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_mts
 *
 * Description: <function description>
 *
 * Inputs:  h_aud           Audio component handle.
 *
 * Outputs: pe_mts          References
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_mts(
    HANDLE_T                        h_aud,
    SCC_AUD_MTS_T*                  pe_mts )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_MTS_T                e_drv_mts;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud          = 0x%.8X\r\n"
                       "\t\tpe_mts         = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pe_mts ));

    if ( !pe_mts )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(AUD_DEC_MTS_T);
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_MTS,
        (VOID*)&e_drv_mts,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch( e_drv_mts )
    {
    case AUD_DEC_MTS_DUAL1:
        *pe_mts = SCC_AUD_MTS_DUAL1;
        break;

    case AUD_DEC_MTS_DUAL2:
        *pe_mts = SCC_AUD_MTS_DUAL2;
        break;

    case AUD_DEC_MTS_MONO:
        *pe_mts = SCC_AUD_MTS_MONO;
        break;

    case AUD_DEC_MTS_STERRO:
        *pe_mts = SCC_AUD_MTS_STEREO;
        break;

    case AUD_DEC_MTS_SUB_LANG:
        *pe_mts = SCC_AUD_MTS_SUB_LANG;
        break;

    case AUD_DEC_MTS_UNKNOWN:
        *pe_mts = SCC_AUD_MTS_UNKNOWN;
        break;

    case AUD_DEC_MTS_NICAM_MONO:
        *pe_mts = SCC_AUD_MTS_NICAM_MONO;
        break;

    case AUD_DEC_MTS_NICAM_STEREO:
         *pe_mts = SCC_AUD_MTS_NICAM_STEREO;
        break;

    case AUD_DEC_MTS_NICAM_DUAL1:
        *pe_mts = SCC_AUD_MTS_NICAM_DUAL1;
        break;

    case AUD_DEC_MTS_NICAM_DUAL2:
        *pe_mts = SCC_AUD_MTS_NICAM_DUAL2;
        break;

    default:
        return SMR_DRV_COMP_ERROR;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_se_eq_user_lvl
 *
 * Description: <function description>
 *
 * Inputs:  h_aud           Audio component handle.
 *
 * Outputs: pt_eq_user_lvl  References
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_se_eq_user_lvl(
    HANDLE_T                        h_aud,
    SCC_AUD_SE_QA_USER_LVL_T*       pt_eq_user_lvl )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_SE_QA_USER_LVL_T     t_drv_se_qa_user_lvl ;
    UINT8                        ui1_num_band = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud          = 0x%.8X\r\n"
                       "\t\tpt_eq_user_lvl = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pt_eq_user_lvl ));

    if ( !pt_eq_user_lvl )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = x_scc_aud_get_se_eq_user_num_band( h_aud, &ui1_num_band );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }
    if ( pt_eq_user_lvl->ui1_band_idx >= ui1_num_band )
    {
        return SMR_INV_ARG;
    }

    t_drv_se_qa_user_lvl.e_eq_type =
        _convert_scc_se_2_drv_se( pt_eq_user_lvl->e_eq_type );
    z_size = sizeof(AUD_DEC_SE_QA_USER_LVL_T);
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_SE_EQ_USER_LVL,
        (VOID*)&t_drv_se_qa_user_lvl,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    pt_eq_user_lvl->ui1_band_idx = t_drv_se_qa_user_lvl.ui1_band_idx;
    pt_eq_user_lvl->i1_level = t_drv_se_qa_user_lvl.i1_level;
    pt_eq_user_lvl->i2_level_ex = t_drv_se_qa_user_lvl.i2_level_ex;
    pt_eq_user_lvl->b_is_level_ex = t_drv_se_qa_user_lvl.b_is_level_ex;
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_se_eq_user_num_band
 *
 * Description: <function description>
 *
 * Inputs:  h_aud           Audio component handle.
 *
 * Outputs: pui1_num_band   References
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_se_eq_user_num_band(
    HANDLE_T                        h_aud,
    UINT8*                          pui1_num_band )
{
    INT32                          i4_ret;
    SIZE_T                         z_size = 0;
    HANDLE_TYPE_T                  e_type;
    SCC_AUD_COMP_T*                pt_aud_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tpui1_num_band   = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pui1_num_band ));

    if ( !pui1_num_band )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(UINT8);
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_SE_EQ_USER_NUM_BAND,
        (VOID*)pui1_num_band,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_se_eq_user_info
 *
 * Description: <function description>
 *
 * Inputs:  h_aud           Audio component handle.
 *
 * Outputs: pt_eq_user_info References
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_se_eq_user_info(
    HANDLE_T                        h_aud,
    SCC_AU_SE_QA_USER_BAND_INFO_T*  pt_eq_user_info )
{
    INT32                          i4_ret;
    SIZE_T                         z_size = 0;
    HANDLE_TYPE_T                  e_type;
    SCC_AUD_COMP_T*                pt_aud_comp = NULL;
    AUD_DEC_SE_QA_USER_INFO_BAND_T t_drv_se_qa_user_info;
    UINT8                          ui1_num_band = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tpt_eq_user_info = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pt_eq_user_info ));

    if ( !pt_eq_user_info )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = x_scc_aud_get_se_eq_user_num_band( h_aud, &ui1_num_band );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }
    if ( pt_eq_user_info->ui1_band_idx >= ui1_num_band )
    {
        return SMR_INV_ARG;
    }

    t_drv_se_qa_user_info.e_eq_type  =
        _convert_scc_se_2_drv_se( pt_eq_user_info->e_eq_type );
    t_drv_se_qa_user_info.ui1_band_idx = pt_eq_user_info->ui1_band_idx;

    z_size = sizeof(AUD_DEC_SE_QA_USER_INFO_BAND_T);
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_SE_EQ_USER_INFO_BAND,
        (VOID*)&t_drv_se_qa_user_info,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    pt_eq_user_info->i1_max_lvl = t_drv_se_qa_user_info.i1_max_lvl;
    pt_eq_user_info->i1_min_lvl = t_drv_se_qa_user_info.i1_min_lvl;
    pt_eq_user_info->ui4_freq_lower_bound = t_drv_se_qa_user_info.ui4_freq_lower_bound;
    pt_eq_user_info->ui4_freq_upper_bound = t_drv_se_qa_user_info.ui4_freq_upper_bound;

    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _scc_audio_clip_nfy_fct
 *
 * Description: <function description>
 *
 * Inputs:  pv_nfy_tag References.
 *          e_nfy_cond References.
 *          ui4_data_1 References.
 *          ui4_data_2 References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _scc_audio_clip_nfy_fct(
    VOID*           pv_nfy_tag,
    AUD_DEC_COND_T  e_nfy_cond,
    UINT32          ui4_data_1,
    UINT32          ui4_data_2 )
{
    INT32               i4_ret;
    HANDLE_TYPE_T       e_type;
    SCC_AUD_COMP_T*     pt_aud_comp = NULL;
    SM_SCC_MSG_T        t_msg;
    HANDLE_T            h_aud_clip = (HANDLE_T)pv_nfy_tag;

    i4_ret = handle_get_type_obj( h_aud_clip,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return;
    }

    if((e_nfy_cond == AUD_DEC_COND_CTRL_DONE ) &&
        ((pt_aud_comp->e_aud_clip_ctrl_mode == SCC_AUD_CTRL_MODE_STOP_SYNC) ||
        (pt_aud_comp->e_aud_clip_ctrl_mode == SCC_AUD_CTRL_MODE_PLAY_SYNC))&&       
        (pt_aud_comp->e_aud_clip_type == SCC_AUD_CLIP_TYPE_CLIP_MIXSOUND))
    {
        
        i4_ret = x_sema_unlock( pt_aud_comp->h_aud_clip_lock);
        if ( i4_ret != OSR_OK )
        {
            SM_ABORT( SM_DBG_ABRT_CANNOT_UNLOCK_SEMAPHORE );
        }
    }
	else if((e_nfy_cond == AUD_DEC_COND_AUD_CLIP_DONE ) &&
        ((pt_aud_comp->e_aud_clip_ctrl_mode == SCC_AUD_CTRL_MODE_STOP_SYNC) ||
        (pt_aud_comp->e_aud_clip_ctrl_mode == SCC_AUD_CTRL_MODE_PLAY_SYNC))&&       
        (pt_aud_comp->e_aud_clip_type == SCC_AUD_CLIP_TYPE_CLIP_CUSTOM))
	{
			
	    i4_ret = x_sema_unlock( pt_aud_comp->h_aud_clip_lock);
	    if ( i4_ret != OSR_OK )
	    {
	        SM_ABORT( SM_DBG_ABRT_CANNOT_UNLOCK_SEMAPHORE );
	    }
	}
    
    switch (e_nfy_cond)
    {
        case AUD_DEC_COND_ERROR:
            t_msg.u.t_aud_clip_nfy.e_nfy_cond = SCC_AUD_CLIP_COND_ERROR;
			SM_SLCTR_DBG_INFO(( 
            "[SCC] %s:%d _scc_audio_clip_nfy_fct SCC_AUD_CLIP_COND_ERROR \r\n",
            __FILE__, __LINE__));
            break;
        case AUD_DEC_COND_CTRL_DONE:
            t_msg.u.t_aud_clip_nfy.e_nfy_cond = SCC_AUD_CLIP_COND_CTRL_DONE;
			 SM_SLCTR_DBG_INFO(( 
            "[SCC] %s:%d _scc_audio_clip_nfy_fct SCC_AUD_CLIP_COND_CTRL_DONE \r\n",
            __FILE__, __LINE__));
            break;
        case AUD_DEC_COND_AUD_CLIP_DONE:
            t_msg.u.t_aud_clip_nfy.e_nfy_cond = SCC_AUD_CLIP_COND_AUD_CLIP_DONE;
            break;
		    SM_SLCTR_DBG_INFO(( 
            "[SCC] %s:%d _scc_audio_clip_nfy_fct SCC_AUD_CLIP_COND_AUD_CLIP_DONE \r\n",
            __FILE__, __LINE__));
        default:
            return;
    }
    
    t_msg.e_msg_type = SM_SCC_MSG_TYPE_AUD_CLIP_NFY;
    t_msg.u.t_aud_clip_nfy.h_aud = h_aud_clip;
    t_msg.u.t_aud_clip_nfy.pf_nfy_fct = pt_aud_comp->pf_aud_clip_nfy;
    t_msg.u.t_aud_clip_nfy.pv_nfy_tag = pt_aud_comp->pv_aud_clip_nfy_tag;
    t_msg.u.t_aud_clip_nfy.ui4_data_1 = ui4_data_1;
    t_msg.u.t_aud_clip_nfy.ui4_data_2 = ui4_data_2;
    _scc_send_msg(&t_msg);

}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_clip
 *
 * Description: set audio clip playback data
 *
 * Inputs:  h_aud                Audio component handle.
 *          pt_aud_clip          Audio clip data structure
 *          pf_scc_aud_clip_nfy  SCC Audio clip notify function callback
 *          pv_nfy_tag           notify callback tag
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_clip(
    HANDLE_T                     h_aud,
    SCC_AUD_CLIP_T*              pt_aud_clip,
    x_scc_aud_clip_nfy_fct       pf_scc_aud_clip_nfy,
    VOID*                        pv_nfy_tag)
{
    INT32               i4_ret;
    HANDLE_TYPE_T       e_type;
    SCC_AUD_COMP_T*     pt_aud_comp = NULL;
    AUD_DEC_AUD_CLIP_T  t_aud_dec_clip;
    AUD_DEC_NFY_INFO_T  t_set_nfy_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\ttpt_aud_clip           = %x\r\n"
                       "\t\tpf_scc_aud_clip_nfy    = %x\r\n"
                       "\t\tpv_nfy_tag           = %x )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       pt_aud_clip,
                       pf_scc_aud_clip_nfy,
                       pv_nfy_tag));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    pt_aud_comp->pf_aud_clip_nfy = pf_scc_aud_clip_nfy;
    pt_aud_comp->pv_aud_clip_nfy_tag = pv_nfy_tag;
    pt_aud_comp->e_aud_clip_type = pt_aud_clip->e_type;
    t_aud_dec_clip.ui4_ch_mask = pt_aud_clip->ui4_ch_mask;
    t_aud_dec_clip.ui4_out_port_mask = pt_aud_clip->ui4_out_port_mask;
    t_aud_dec_clip.ui4_repeat_cnt = (UINT32)pt_aud_clip->ui4_repeat_cnt;
    switch(pt_aud_clip->e_type)
    {
    case SCC_AUD_CLIP_TYPE_TONE:
        t_aud_dec_clip.e_type = AUD_DEC_AUD_CLIP_TYPE_TONE;
        t_aud_dec_clip.u.ui4_tone_freq = pt_aud_clip->u.ui4_tone_freq;
        break;
    case SCC_AUD_CLIP_TYPE_CLIP_BUFF:
        t_aud_dec_clip.e_type = AUD_DEC_AUD_CLIP_TYPE_CLIP_BUFF;
        /* e_fmt latter setting */
        t_aud_dec_clip.u.t_clip_buff.pui1_clip_buff = pt_aud_clip->u.t_clip_buff.pui1_clip_buff;
        t_aud_dec_clip.u.t_clip_buff.z_clip_buff_size = pt_aud_clip->u.t_clip_buff.z_clip_buff_size;
        switch (pt_aud_clip->u.t_clip_buff.e_PCM_channel_type)
        {
            case SCC_AUD_CHANNEL_TYPE_UNKNOWN:
                t_aud_dec_clip.u.t_clip_buff.e_PCM_channel_type = AUD_DEC_CHANNEL_TYPE_UNKNOWN;
                break;
            case SCC_AUD_CHANNEL_TYPE_MONO:
                t_aud_dec_clip.u.t_clip_buff.e_PCM_channel_type = AUD_DEC_CHANNEL_TYPE_MONO;
                break;
            case SCC_AUD_CHANNEL_TYPE_STEREO:
                t_aud_dec_clip.u.t_clip_buff.e_PCM_channel_type = AUD_DEC_CHANNEL_TYPE_STEREO;
                break;
        }
        t_aud_dec_clip.u.t_clip_buff.ui4_PCM_SampleRate = pt_aud_clip->u.t_clip_buff.ui4_PCM_SampleRate;
        t_aud_dec_clip.u.t_clip_buff.ui1_PCM_BitDepth = pt_aud_clip->u.t_clip_buff.ui1_PCM_BitDepth;
        break;
    case SCC_AUD_CLIP_TYPE_CLIP_CUSTOM:
        t_aud_dec_clip.e_type = AUD_DEC_AUD_CLIP_TYPE_CLIP_CUSTOM;
        switch (pt_aud_clip->u.e_clip_custom_mode)
        {
            case SCC_AUD_CLIP_CUSTOM_MODE_CUSTOM1:
                t_aud_dec_clip.u.e_aud_dec_clip_custom_mode = AUD_DEC_CLIP_CUSTOM_MODE_CUSTOM1;
                break;
            case SCC_AUD_CLIP_CUSTOM_MODE_CUSTOM2:
                t_aud_dec_clip.u.e_aud_dec_clip_custom_mode = AUD_DEC_CLIP_CUSTOM_MODE_CUSTOM2;
                break;
            case SCC_AUD_CLIP_CUSTOM_MODE_CUSTOM3:
                t_aud_dec_clip.u.e_aud_dec_clip_custom_mode = AUD_DEC_CLIP_CUSTOM_MODE_CUSTOM3;
                break;
            case SCC_AUD_CLIP_CUSTOM_MODE_CUSTOM4:
                t_aud_dec_clip.u.e_aud_dec_clip_custom_mode = AUD_DEC_CLIP_CUSTOM_MODE_CUSTOM4;
                break;
        }
        break;
    case SCC_AUD_CLIP_TYPE_CLIP_MIXSOUND:
        t_aud_dec_clip.e_type = AUD_DEC_AUD_CLIP_TYPE_MIXSOUND;
        /* e_fmt latter setting */
        t_aud_dec_clip.u.t_clip_buff.pui1_clip_buff = pt_aud_clip->u.t_clip_buff.pui1_clip_buff;
        t_aud_dec_clip.u.t_clip_buff.z_clip_buff_size = pt_aud_clip->u.t_clip_buff.z_clip_buff_size;
        switch (pt_aud_clip->u.t_clip_buff.e_PCM_channel_type)
        {
            case SCC_AUD_CHANNEL_TYPE_UNKNOWN:
                t_aud_dec_clip.u.t_clip_buff.e_PCM_channel_type = AUD_DEC_CHANNEL_TYPE_UNKNOWN;
                break;
            case SCC_AUD_CHANNEL_TYPE_MONO:
                t_aud_dec_clip.u.t_clip_buff.e_PCM_channel_type = AUD_DEC_CHANNEL_TYPE_MONO;
                break;
            case SCC_AUD_CHANNEL_TYPE_STEREO:
                t_aud_dec_clip.u.t_clip_buff.e_PCM_channel_type = AUD_DEC_CHANNEL_TYPE_STEREO;
                break;
        }
        t_aud_dec_clip.u.t_clip_buff.ui4_PCM_SampleRate = pt_aud_clip->u.t_clip_buff.ui4_PCM_SampleRate;
        t_aud_dec_clip.u.t_clip_buff.ui1_PCM_BitDepth = pt_aud_clip->u.t_clip_buff.ui1_PCM_BitDepth;
        break;
    default:
        return SMR_INV_ARG;
    }

    if (pt_aud_clip->e_type == SCC_AUD_CLIP_TYPE_CLIP_BUFF ||
        pt_aud_clip->e_type == SCC_AUD_CLIP_TYPE_CLIP_MIXSOUND)
    {
        switch (pt_aud_clip->u.t_clip_buff.e_fmt)
        {
        case ASH_AUDIO_FMT_MPEG:
            t_aud_dec_clip.u.t_clip_buff.e_fmt = AUD_DEC_FMT_MPEG;
            break;
        case ASH_AUDIO_FMT_AC3:
            t_aud_dec_clip.u.t_clip_buff.e_fmt = AUD_DEC_FMT_AC3;
            break;
        case ASH_AUDIO_FMT_PCM:
            t_aud_dec_clip.u.t_clip_buff.e_fmt = AUD_DEC_FMT_PCM;
            break;
        case ASH_AUDIO_FMT_MP3:
            t_aud_dec_clip.u.t_clip_buff.e_fmt = AUD_DEC_FMT_MP3;
            break;
        case ASH_AUDIO_FMT_AAC:
            t_aud_dec_clip.u.t_clip_buff.e_fmt = AUD_DEC_FMT_AAC;
            break;
        case ASH_AUDIO_FMT_DTS:
            t_aud_dec_clip.u.t_clip_buff.e_fmt = AUD_DEC_FMT_DTS;
            break;
        case ASH_AUDIO_FMT_WMA:
            t_aud_dec_clip.u.t_clip_buff.e_fmt = AUD_DEC_FMT_WMA;
            break;
        case ASH_AUDIO_FMT_RA:
            t_aud_dec_clip.u.t_clip_buff.e_fmt = AUD_DEC_FMT_RA;
            break;
        case ASH_AUDIO_FMT_HDCD:
            t_aud_dec_clip.u.t_clip_buff.e_fmt = AUD_DEC_FMT_HDCD;
            break;
        case ASH_AUDIO_FMT_MLP:
            t_aud_dec_clip.u.t_clip_buff.e_fmt = AUD_DEC_FMT_MLP;
            break;
        case ASH_AUDIO_FMT_MTS:
            t_aud_dec_clip.u.t_clip_buff.e_fmt = AUD_DEC_FMT_MTS;
            break;
        case ASH_AUDIO_FMT_COOK:
            t_aud_dec_clip.u.t_clip_buff.e_fmt = AUD_DEC_FMT_COOK;
            break;
        case ASH_AUDIO_FMT_AMR:
            t_aud_dec_clip.u.t_clip_buff.e_fmt = AUD_DEC_FMT_AMR;
            break;
        case ASH_AUDIO_FMT_AWB:
            t_aud_dec_clip.u.t_clip_buff.e_fmt = AUD_DEC_FMT_AWB;
            break;
        default:
            return SMR_INV_ARG;
        }
    }

    /* set audio clip */
    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_AUD_CLIP,
        (VOID*)&t_aud_dec_clip,
        sizeof( AUD_DEC_AUD_CLIP_T ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }
    SM_SLCTR_DBG_INFO(( 
    "[SCC] %s:%d x_scc_aud_set_clip : AUD_DEC_SET_TYPE_AUD_CLIP\r\n",
     __FILE__, __LINE__));
    t_set_nfy_info.pf_aud_dec_nfy = _scc_audio_clip_nfy_fct;
    t_set_nfy_info.pv_tag = (VOID*)h_aud;
    /* set notify function */
    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_NFY_FCT,
        (VOID*)&t_set_nfy_info,
        sizeof( AUD_DEC_NFY_INFO_T ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_clip_gain
 *
 * Description: The linear gain converted to a fractional fixed point unsigned integer, as described
 *              in [BD-ROM3] part 1 I.4. The value to pass is the linear gain multiplied by 8192, and
 *              rounded to the closest integer.
 *
 * Inputs:  h_aud       Audio component handle.
 *
 * Outputs: ui4_gain    b0 ~b15 => fraction portion, b16~b31 => integer portion
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_clip_gain(
    HANDLE_T                      h_aud,
    UINT32                        ui4_gain )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tui4_gain       = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       ui4_gain ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_AUD_CLIP_GAIN,
        (VOID*)ui4_gain,
        sizeof( UINT32 ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_clip_ctrl
 *
 * Description: trigger audio clip play control
 *
 * Inputs:  h_aud                Audio component handle.
 *
 * Outputs: pe_given_ctrl_mode   play control mode(play/pause/stop/resume)
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_clip_ctrl(
    HANDLE_T                      h_aud,
    SCC_AUD_CTRL_MODE_T*          pe_given_ctrl_mode)
{
    INT32               i4_ret;
    HANDLE_TYPE_T       e_type;
    SCC_AUD_COMP_T*     pt_aud_comp = NULL;
    AUD_DEC_CTRL_T      e_cur_ctrl_mode;
    SIZE_T              z_size = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\ttpe_given_ctrl_mode           = %x\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       pe_given_ctrl_mode));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    /* get current driver play mode */
    z_size = sizeof(AUD_DEC_CTRL_T);
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_CTRL,
        (VOID*)&e_cur_ctrl_mode,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    /* get play mode */
    switch (e_cur_ctrl_mode)
    {
    case AUD_DEC_CTRL_STOP:
        if(pt_aud_comp->e_aud_clip_ctrl_mode == SCC_AUD_CTRL_MODE_STOP_SYNC )
        {
            *pe_given_ctrl_mode = SCC_AUD_CTRL_MODE_STOP_SYNC;
        }
        else
        {
        *pe_given_ctrl_mode = SCC_AUD_CTRL_MODE_STOP;
        }
        break;
    case AUD_DEC_CTRL_PLAY_AUD_CLIP:
    case AUD_DEC_CTRL_PLAY_AUD_MIXSND_CLIP:
        if(pt_aud_comp->e_aud_clip_ctrl_mode == SCC_AUD_CTRL_MODE_PLAY_SYNC )
        {
            *pe_given_ctrl_mode = SCC_AUD_CTRL_MODE_PLAY_SYNC;
        }
        else if(pt_aud_comp->e_aud_clip_ctrl_mode == SCC_AUD_CTRL_MODE_PLAY_MIXSND )     
        {           
            *pe_given_ctrl_mode = SCC_AUD_CTRL_MODE_PLAY_MIXSND;        
        }      
        else
        {
        *pe_given_ctrl_mode = SCC_AUD_CTRL_MODE_PLAY;
        }
        break;
    case AUD_DEC_CTRL_PAUSE:
        *pe_given_ctrl_mode = SCC_AUD_CTRL_MODE_PAUSE;
        break;
    case AUD_DEC_CTRL_RESUME:
        *pe_given_ctrl_mode = SCC_AUD_CTRL_MODE_RESUME;
        break;
    default:
        return SMR_INV_ARG;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_clip_ctrl
 *
 * Description: trigger audio clip play control
 *
 * Inputs:  h_aud                Audio component handle.
 *          e_given_ctrl_mode          play control mode(play/pause/stop)
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_clip_ctrl(
    HANDLE_T                      h_aud,
    SCC_AUD_CTRL_MODE_T           e_given_ctrl_mode)
{
    INT32               i4_ret;
    HANDLE_TYPE_T       e_type;
    SCC_AUD_COMP_T*     pt_aud_comp = NULL;
    AUD_DEC_CTRL_T      e_cur_ctrl_mode;
    SIZE_T              z_size = 0;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tte_given_ctrl_mode           = %x\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       e_given_ctrl_mode));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    
    /* get current driver play mode */
    z_size = sizeof(AUD_DEC_CTRL_T);
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_CTRL,
        (VOID*)&e_cur_ctrl_mode,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }
    
    if(((e_given_ctrl_mode == SCC_AUD_CTRL_MODE_PLAY_SYNC) || (e_given_ctrl_mode == SCC_AUD_CTRL_MODE_STOP_SYNC))&&
        ((pt_aud_comp->e_aud_clip_type != SCC_AUD_CLIP_TYPE_CLIP_CUSTOM)&& 
        (pt_aud_comp->e_aud_clip_type != SCC_AUD_CLIP_TYPE_CLIP_MIXSOUND)) )
    {
        /*only SCC_AUD_CLIP_TYPE_CLIP_CUSTOM and SCC_AUD_CLIP_TYPE_CLIP_MIXSOUND support sync play/stop*/
        return SMR_INV_SET_INFO;
    }

    if(((e_given_ctrl_mode == SCC_AUD_CTRL_MODE_PLAY)  || (e_given_ctrl_mode == SCC_AUD_CTRL_MODE_STOP))&&
        (pt_aud_comp->e_aud_clip_type == SCC_AUD_CLIP_TYPE_CLIP_CUSTOM) &&
        (pt_aud_comp->pf_aud_clip_nfy == NULL))
    {
        return SMR_INV_HANDLE;
    }

    /* check current mode */
    if ((e_given_ctrl_mode == SCC_AUD_CTRL_MODE_PLAY)
        && (e_cur_ctrl_mode == AUD_DEC_CTRL_PLAY))
    {
        /* audio decoder is playing other audio src */
        return SMR_ALREADY_PLAYED;
    }
    
    
    if (((e_given_ctrl_mode == SCC_AUD_CTRL_MODE_PLAY) && (e_cur_ctrl_mode == AUD_DEC_CTRL_PLAY_AUD_CLIP))
        || ((e_given_ctrl_mode == SCC_AUD_CTRL_MODE_PLAY_MIXSND) && (e_cur_ctrl_mode == AUD_DEC_CTRL_PLAY_AUD_MIXSND_CLIP))
        || ((e_given_ctrl_mode == SCC_AUD_CTRL_MODE_STOP) && (e_cur_ctrl_mode == AUD_DEC_CTRL_STOP))
        || ((e_given_ctrl_mode == SCC_AUD_CTRL_MODE_PAUSE) && (e_cur_ctrl_mode == AUD_DEC_CTRL_PAUSE))
        || ((e_given_ctrl_mode == SCC_AUD_CTRL_MODE_PLAY_SYNC) && (e_cur_ctrl_mode == AUD_DEC_CTRL_PLAY_AUD_CLIP))
        || ((e_given_ctrl_mode == SCC_AUD_CTRL_MODE_STOP_SYNC) && (e_cur_ctrl_mode == AUD_DEC_CTRL_STOP)))
    {
        return SMR_OK;
    }

    pt_aud_comp->e_aud_clip_ctrl_mode = e_given_ctrl_mode;
    
    /* set play mode */
    switch (e_given_ctrl_mode)
    {
    case SCC_AUD_CTRL_MODE_STOP:
    case SCC_AUD_CTRL_MODE_STOP_SYNC:
        e_cur_ctrl_mode = AUD_DEC_CTRL_STOP;
        break;
    case SCC_AUD_CTRL_MODE_PLAY:
    case SCC_AUD_CTRL_MODE_PLAY_SYNC:
        e_cur_ctrl_mode = AUD_DEC_CTRL_PLAY_AUD_CLIP;
        break;
    case SCC_AUD_CTRL_MODE_PLAY_MIXSND:
        e_cur_ctrl_mode = AUD_DEC_CTRL_PLAY_AUD_MIXSND_CLIP;
        break;
    case SCC_AUD_CTRL_MODE_PAUSE:
        e_cur_ctrl_mode = AUD_DEC_CTRL_PAUSE;
        break;
    case SCC_AUD_CTRL_MODE_RESUME:
        e_cur_ctrl_mode = AUD_DEC_CTRL_RESUME;
        break;
        
    default:
        return SMR_INV_ARG;
    }

    if((( e_given_ctrl_mode == SCC_AUD_CTRL_MODE_PLAY_SYNC) ||(e_given_ctrl_mode == SCC_AUD_CTRL_MODE_STOP_SYNC))&&
        ((pt_aud_comp->e_aud_clip_type == SCC_AUD_CLIP_TYPE_CLIP_CUSTOM)||
         (pt_aud_comp->e_aud_clip_type == SCC_AUD_CLIP_TYPE_CLIP_MIXSOUND)))
    {
        /***for CR DTV083236********************************************/
         UINT32      ui4_mute_mask;
         z_size = sizeof( UINT32 );
         i4_ret = _scc_comp_aud_get(
             h_aud,
             DRVT_AUD_DEC,
             AUD_DEC_GET_TYPE_MUTE_EX,
             (VOID*)&ui4_mute_mask,
             &z_size );
         if ( i4_ret != SMR_OK )
         {
             return SMR_DRV_COMP_ERROR;
         }
         if ( ui4_mute_mask & AUD_DEC_OUT_PORT_FLAG_SPEAKER )
         {
             ui4_mute_mask &= ~AUD_DEC_OUT_PORT_FLAG_SPEAKER;
         }
         if ( ui4_mute_mask & AUD_DEC_OUT_PORT_FLAG_HEADPHONE )
         {
             ui4_mute_mask &= ~AUD_DEC_OUT_PORT_FLAG_HEADPHONE;
         }
         
         i4_ret = _scc_comp_aud_set(
             h_aud,
             DRVT_AUD_DEC,
             AUD_DEC_SET_TYPE_MUTE_EX,
             (VOID*)&ui4_mute_mask,
             sizeof( UINT32 ) );
         if ( i4_ret != SMR_OK )
         {
             return SMR_DRV_COMP_ERROR;
         }
         /*******************************************************/
        i4_ret = x_sema_create( &(pt_aud_comp->h_aud_clip_lock),
                                X_SEMA_TYPE_BINARY,
                                X_SEMA_STATE_LOCK );
        if ( i4_ret != OSR_OK )
        {
            SM_ABORT( SM_DBG_ABRT_CANNOT_CREATE_SEMAPHORE );
        }
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_CTRL,
        (VOID*)e_cur_ctrl_mode,
        sizeof( AUD_DEC_CTRL_T ) );
    if ( i4_ret != SMR_OK )
    {
        x_sema_unlock( pt_aud_comp->h_aud_clip_lock);
        x_sema_delete(pt_aud_comp->h_aud_clip_lock);
        pt_aud_comp->h_aud_clip_lock = NULL_HANDLE;
        return SMR_INV_SET_INFO;
    }

    if((( e_given_ctrl_mode == SCC_AUD_CTRL_MODE_PLAY_SYNC) ||(e_given_ctrl_mode == SCC_AUD_CTRL_MODE_STOP_SYNC))&&
        ((pt_aud_comp->e_aud_clip_type == SCC_AUD_CLIP_TYPE_CLIP_CUSTOM)||
        (pt_aud_comp->e_aud_clip_type == SCC_AUD_CLIP_TYPE_CLIP_MIXSOUND)))
    {
        i4_ret = x_sema_lock_timeout( pt_aud_comp->h_aud_clip_lock, 3000 );
        if (( i4_ret != OSR_OK ) && ( i4_ret != OSR_TIMEOUT))
        {
            SM_ABORT( SM_DBG_ABRT_CANNOT_LOCK_SEMAPHORE );
        }
        
        if(e_given_ctrl_mode == SCC_AUD_CTRL_MODE_PLAY_SYNC)
        {
            i4_ret = _scc_comp_aud_set(
                            h_aud,
                            DRVT_AUD_DEC,
                            AUD_DEC_SET_TYPE_CTRL,
                            (VOID*) AUD_DEC_CTRL_STOP,
                            sizeof( AUD_DEC_CTRL_T ) );
            if ( i4_ret != SMR_OK )
        {
                return SMR_INV_SET_INFO;
            }
        }
        

        x_sema_unlock( pt_aud_comp->h_aud_clip_lock);
        x_sema_delete(pt_aud_comp->h_aud_clip_lock);
        pt_aud_comp->h_aud_clip_lock = NULL_HANDLE;        
    }

    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_clip_duration
 *
 * Description: get audio clip duration
 *
 * Inputs:  h_aud                Audio component handle.
 *
 *
 * Outputs: pui4_duration        retrieve the duration of the audio clip (ms)
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_clip_duration(
    HANDLE_T                       h_aud,
    UINT32*                        pui4_duration)
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_AUD_CLIP_INFO_T      t_clip_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tpui4_duration        = %x )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       pui4_duration ));


    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof( AUD_DEC_AUD_CLIP_INFO_T );
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_AUD_CLIP_INFO,
        (VOID*)&t_clip_info,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    *pui4_duration = t_clip_info.ui4_duration;

    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_clip_position
 *
 * Description: get audio clip duration
 *
 * Inputs:  h_aud                Audio component handle.
 *
 *
 * Outputs: pui4_duration        retrieve the current position of the audio clip(ms)
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/

INT32 x_scc_aud_set_clip_position(
    HANDLE_T                        h_aud,
    UINT32                          ui4_position )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_AUD_CLIP_INFO_T      t_aud_clip_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tui4_position       = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       ui4_position ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_aud_clip_info.ui4_time_position = ui4_position;

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_AUD_CLIP_INFO,
        (VOID*)&t_aud_clip_info,
        sizeof( AUD_DEC_AUD_CLIP_INFO_T ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;

}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_clip_position
 *
 * Description: get audio clip duration
 *
 * Inputs:  h_aud                Audio component handle.
 *
 *
 * Outputs: pui4_duration        retrieve the current position of the audio clip(ms)
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_clip_position(
    HANDLE_T                        h_aud,
    UINT32*                         pui4_position)
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_AUD_CLIP_INFO_T      t_clip_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tpui4_duration        = %x )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       pui4_position ));


    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof( AUD_DEC_AUD_CLIP_INFO_T );
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_AUD_CLIP_INFO,
        (VOID*)&t_clip_info,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    *pui4_position = t_clip_info.ui4_time_position;

    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_clip_volume
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          pt_volume           References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_clip_volume( HANDLE_T                      h_aud,
                            const SCC_AUD_VOLUME_INFO_T*  pt_volume )
{
	 INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_VOLUME_INFO_EX_T     t_aud_vol_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud                 = 0x%.8X\r\n"
                       "\t\te_out_port            = %d\r\n"
                       "\t\te_ch                  = %d\r\n"
                       "\t\tui1_volumn            = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)((pt_volume)?pt_volume->e_out_port:(-1)),
                       (INT32)((pt_volume)?pt_volume->e_ch:(-1)),
                       (INT32)((pt_volume)?pt_volume->ui1_volumn:(-1)) ));
    if ( !pt_volume )
    {
        return SMR_INV_ARG;
    }

    if (pt_volume->ui1_volumn > 100)
    {
        SM_SLCTR_DBG_ERR((
                    "[SCC] pt_scc_aud_vol_info->ui1_volumn > 100.\r\n" ));
        return SMR_INV_SET_INFO;
    }

    if ( (pt_volume->e_out_port == SCC_AUD_OUT_PORT_2_CH) &&
         (pt_volume->e_ch != SCC_AUD_CHANNEL_FRONT_LEFT) &&
         (pt_volume->e_ch != SCC_AUD_CHANNEL_FRONT_RIGHT) &&
         (pt_volume->e_ch != SCC_AUD_CHANNEL_ALL))
    {
        SM_SLCTR_DBG_ERR((
            "[SCC] %s:%d, ERROR!! SCC_AUD_OUT_PORT_2_CH with invalid channel = %u.\r\n",
            __FUNCTION__, __LINE__,
            pt_volume->e_ch ));
        return SMR_INV_SET_INFO;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_aud_vol_info.e_vol_type = AUD_DEC_TEST_TONE;
    t_aud_vol_info.u.t_ch_vol.ui1_level = pt_volume->ui1_volumn;
    t_aud_vol_info.u.t_ch_vol.e_ls = _scc_aud_ch_2_drv_aud_ch(
        pt_volume->e_ch );
    t_aud_vol_info.e_out_port  =
        _scc_aud_outport_2_drv_aud_outport(
            pt_volume->e_out_port );


    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_VOLUME_EX,
        (VOID*)&t_aud_vol_info,
        sizeof(AUD_DEC_VOLUME_INFO_T) );
    if ( i4_ret != RMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_clip_custom_setting
 *
 * Description: set output port whether is adjust or not
 *
 * Inputs:  h_aud                Audio component handle.
 *          e_port               specify set outport
 *          b_is_fixed           specify is fixed or not for the port
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_clip_custom_setting(
    HANDLE_T                        h_aud,
    SCC_AUD_CLIP_CUSTOM_SETTING_TYPE_T              e_setting_type,
    SCC_AUD_CLIP_CUSTOM_SETTING_MODE_T              e_setting_mode )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_AUD_CLIP_CUSTIOM_T   t_drv_clip_custom;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud                  = 0x%.8X\r\n"
                       "\t\te_setting_type                 = %d\r\n"
                       "\t\te_setting_mode            = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)e_setting_type,
                       (INT32)e_setting_mode ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch(e_setting_type)
    {
    case SCC_AUD_CLIP_CUSTOM_SETTING_TYPE_POWER_ON:
        t_drv_clip_custom.e_drv_clip_custom_setting_type = AUD_DEC_CLIP_CUSTOM_SETTING_TYPE_POWER_ON;
        break;
    case SCC_AUD_CLIP_CUSTOM_SETTING_TYPE_POWER_OFF:
        t_drv_clip_custom.e_drv_clip_custom_setting_type  = AUD_DEC_CLIP_CUSTOM_SETTING_TYPE_POWER_OFF;
        break;
    default:
        return SMR_INV_SET_INFO;
    }
    switch(e_setting_mode)
    {
    case SCC_AUD_CLIP_CUSTOM_SETTING_MODE_ON:
        t_drv_clip_custom.e_drv_clip_custom_setting_mode = AUD_DEC_CLIP_CUSTOM_SETTING_MODE_ON;
        break;
    case SCC_AUD_CLIP_CUSTOM_SETTING_MODE_OFF:
        t_drv_clip_custom.e_drv_clip_custom_setting_mode = AUD_DEC_CLIP_CUSTOM_SETTING_MODE_OFF;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_CLIP_CUSTOM,
        (VOID*)&t_drv_clip_custom,
        sizeof(AUD_DEC_AUD_CLIP_CUSTIOM_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_clip_custom_setting
 *
 * Description: get output port whether is adjust or not
 *
 * Inputs:  h_aud                Audio component handle.
 *          e_port               specify set outport
 *
 * Outputs:
 *          pb_is_fixed          retrive is fixed or not for the port
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_clip_custom_setting(
    HANDLE_T                        h_aud,
    SCC_AUD_CLIP_CUSTOM_SETTING_TYPE_T              e_setting_type,
    SCC_AUD_CLIP_CUSTOM_SETTING_MODE_T*              pe_setting_mode )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_AUD_CLIP_CUSTIOM_T   t_drv_clip_custom;
    
    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud                  = 0x%.8X\r\n"
                       "\t\te_setting_type                 = %d\r\n"
                       "\t\te_setting_mode            = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)e_setting_type,
                       (INT32)pe_setting_mode ));


    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch(e_setting_type)
    {
    case SCC_AUD_CLIP_CUSTOM_SETTING_TYPE_POWER_ON:
        t_drv_clip_custom.e_drv_clip_custom_setting_type = AUD_DEC_CLIP_CUSTOM_SETTING_TYPE_POWER_ON;
        break;
    case SCC_AUD_CLIP_CUSTOM_SETTING_TYPE_POWER_OFF:
        t_drv_clip_custom.e_drv_clip_custom_setting_type = AUD_DEC_CLIP_CUSTOM_SETTING_TYPE_POWER_OFF;
        break;
    default:
        return SMR_INV_GET_INFO;
    }

    z_size = sizeof( AUD_DEC_AUD_CLIP_CUSTIOM_T );
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_CLIP_CUSTOM,
        (VOID*)&t_drv_clip_custom,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }
    switch(t_drv_clip_custom.e_drv_clip_custom_setting_mode)
    {
    case  AUD_DEC_CLIP_CUSTOM_SETTING_MODE_ON  :
        *pe_setting_mode = SCC_AUD_CLIP_CUSTOM_SETTING_MODE_ON;
        break;
    case AUD_DEC_CLIP_CUSTOM_SETTING_MODE_OFF:
        *pe_setting_mode = SCC_AUD_CLIP_CUSTOM_SETTING_MODE_OFF;
        break;
    default:
        return SMR_INV_GET_INFO;
    }

    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_pl2_switch
 *
 * Description: set audio Pro Logic II switch
 *
 * Inputs:  h_aud                Audio component handle.
 *          e_value              specify mode value
 *
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_pl2_switch(
    HANDLE_T                        h_aud,
    SCC_AUD_PL2_SWITCH_T            e_value )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_PL2_CTRL_INFO_T      t_pl2_ctrl_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\te_value        = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       e_value ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_pl2_ctrl_info.e_ctrl = AUD_DEC_PL2_CTRL_SWITCH;
    switch (e_value)
    {
    case SCC_AUD_PL2_SWITCH_OFF:
        t_pl2_ctrl_info.u.e_pl2_switch = AUD_DEC_PL2_SWITCH_OFF;
        break;
    case SCC_AUD_PL2_SWITCH_ON:
        t_pl2_ctrl_info.u.e_pl2_switch = AUD_DEC_PL2_SWITCH_ON;
        break;
    case SCC_AUD_PL2_SWITCH_AUTO:
        t_pl2_ctrl_info.u.e_pl2_switch = AUD_DEC_PL2_SWITCH_AUTO;
        break;
    default:
        return SMR_INV_ARG;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_PL2_CTRL,
        (VOID*)&t_pl2_ctrl_info,
        sizeof( AUD_DEC_PL2_CTRL_INFO_T ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_pl2_mode
 *
 * Description: set audio Pro Logic II mode
 *
 * Inputs:  h_aud                Audio component handle.
 *          e_value              specify mode value
 *
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_pl2_mode(
    HANDLE_T                        h_aud,
    SCC_AUD_PL2_MODE_T              e_value )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_PL2_CTRL_INFO_T      t_pl2_ctrl_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\te_value        = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       e_value ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_pl2_ctrl_info.e_ctrl = AUD_DEC_PL2_CTRL_MODE;
    switch (e_value)
    {
    case SCC_AUD_PL2_MODE_PROLGIC_EMULATION:
        t_pl2_ctrl_info.u.e_pl2_mode = AUD_DEC_PL2_MODE_MODE_PROLGIC_EMULATION;
        break;
    case SCC_AUD_PL2_MODE_VIRTUAL:
        t_pl2_ctrl_info.u.e_pl2_mode = AUD_DEC_PL2_MODE_MODE_VIRTUAL;
        break;
    case SCC_AUD_PL2_MODE_MUSIC:
        t_pl2_ctrl_info.u.e_pl2_mode = AUD_DEC_PL2_MODE_MODE_MUSIC;
        break;
    case SCC_AUD_PL2_MODE_MOVIE:
        t_pl2_ctrl_info.u.e_pl2_mode = AUD_DEC_PL2_MODE_MODE_MOVIE;
        break;
    case SCC_AUD_PL2_MODE_MATRIX:
        t_pl2_ctrl_info.u.e_pl2_mode = AUD_DEC_PL2_MODE_MODE_MATRIX;
        break;
    case SCC_AUD_PL2_MODE_CUSTOM:
        t_pl2_ctrl_info.u.e_pl2_mode = AUD_DEC_PL2_MODE_MODE_CUSTOM;
        break;
    default:
        return SMR_INV_ARG;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_PL2_CTRL,
        (VOID*)&t_pl2_ctrl_info,
        sizeof( AUD_DEC_PL2_CTRL_INFO_T ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_pl2_panorama
 *
 * Description: set audio Pro Logic II panorama
 *
 * Inputs:  h_aud                Audio component handle.
 *          e_value              specify mode value
 *
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_pl2_panorama(
    HANDLE_T                        h_aud,
    SCC_AUD_PL2_PANORAMA_T          e_value )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_PL2_CTRL_INFO_T      t_pl2_ctrl_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\te_value        = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       e_value ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_pl2_ctrl_info.e_ctrl = AUD_DEC_PL2_CTRL_PANORAMA;
    switch (e_value)
    {
    case SCC_AUD_PL2_PANORAMA_OFF:
        t_pl2_ctrl_info.u.b_is_pl2_panorama_on = FALSE;
        break;
    case SCC_AUD_PL2_PANORAMA_ON:
        t_pl2_ctrl_info.u.b_is_pl2_panorama_on = TRUE;
        break;
    default:
        return SMR_INV_ARG;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_PL2_CTRL,
        (VOID*)&t_pl2_ctrl_info,
        sizeof( AUD_DEC_PL2_CTRL_INFO_T ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_pl2_dimension
 *
 * Description: set audio Pro Logic II dimension
 *
 * Inputs:  h_aud                Audio component handle.
 *          e_value              specify mode value
 *
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_pl2_dimension(
    HANDLE_T                        h_aud,
    SCC_AUD_PL2_DIMENSION_T         e_value )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_PL2_CTRL_INFO_T      t_pl2_ctrl_info;
    SIZE_T                       z_size;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\te_value        = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       e_value ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_pl2_ctrl_info.e_ctrl = AUD_DEC_PL2_CTRL_DIMENSION;

    z_size = sizeof( AUD_DEC_PL2_CTRL_INFO_T );
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_PL2_CTRL,
        (VOID*)&t_pl2_ctrl_info,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch (e_value)
    {
    case SCC_AUD_PL2_DIMENSION_LEVEL0_BACK:
        t_pl2_ctrl_info.u.t_pl2_val.ui1_curr_val = 0;
        break;
    case SCC_AUD_PL2_DIMENSION_LEVEL1:
        t_pl2_ctrl_info.u.t_pl2_val.ui1_curr_val = 1;
        break;
    case SCC_AUD_PL2_DIMENSION_LEVEL2:
        t_pl2_ctrl_info.u.t_pl2_val.ui1_curr_val = 2;
        break;
    case SCC_AUD_PL2_DIMENSION_LEVEL3:
        t_pl2_ctrl_info.u.t_pl2_val.ui1_curr_val = 3;
        break;
    case SCC_AUD_PL2_DIMENSION_LEVEL4:
        t_pl2_ctrl_info.u.t_pl2_val.ui1_curr_val = 4;
        break;
    case SCC_AUD_PL2_DIMENSION_LEVEL5:
        t_pl2_ctrl_info.u.t_pl2_val.ui1_curr_val = 5;
        break;
    case SCC_AUD_PL2_DIMENSION_LEVEL6_FRONT:
        t_pl2_ctrl_info.u.t_pl2_val.ui1_curr_val = 6;
        break;
    default:
        return SMR_INV_ARG;
    }

    if ((t_pl2_ctrl_info.u.t_pl2_val.ui1_curr_val > t_pl2_ctrl_info.u.t_pl2_val.ui1_max_val)
        || (t_pl2_ctrl_info.u.t_pl2_val.ui1_curr_val < t_pl2_ctrl_info.u.t_pl2_val.ui1_min_val))
    {
         return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_PL2_CTRL,
        (VOID*)&t_pl2_ctrl_info,
        sizeof( AUD_DEC_PL2_CTRL_INFO_T ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_pl2_c_width
 *
 * Description: set audio Pro Logic II center width
 *
 * Inputs:  h_aud                Audio component handle.
 *          e_value              specify mode value
 *
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_pl2_c_width(
    HANDLE_T                        h_aud,
    SCC_AUD_PL2_C_WIDTH_T           e_value )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_PL2_CTRL_INFO_T      t_pl2_ctrl_info;
    SIZE_T                       z_size;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\te_value        = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       e_value ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_pl2_ctrl_info.e_ctrl = AUD_DEC_PL2_CTRL_C_WIDTH;

    z_size = sizeof( AUD_DEC_PL2_CTRL_INFO_T );
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_PL2_CTRL,
        (VOID*)&t_pl2_ctrl_info,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch (e_value)
    {
    case SCC_AUD_PL2_C_WIDTH_LEVEL0_MIN_SPREAD:
        t_pl2_ctrl_info.u.t_pl2_val.ui1_curr_val = 0;
        break;
    case SCC_AUD_PL2_C_WIDTH_LEVEL1:
        t_pl2_ctrl_info.u.t_pl2_val.ui1_curr_val = 1;
        break;
    case SCC_AUD_PL2_C_WIDTH_LEVEL2:
        t_pl2_ctrl_info.u.t_pl2_val.ui1_curr_val = 2;
        break;
    case SCC_AUD_PL2_C_WIDTH_LEVEL3:
        t_pl2_ctrl_info.u.t_pl2_val.ui1_curr_val = 3;
        break;
    case SCC_AUD_PL2_C_WIDTH_LEVEL4:
        t_pl2_ctrl_info.u.t_pl2_val.ui1_curr_val = 4;
        break;
    case SCC_AUD_PL2_C_WIDTH_LEVEL5:
        t_pl2_ctrl_info.u.t_pl2_val.ui1_curr_val = 5;
        break;
    case SCC_AUD_PL2_C_WIDTH_LEVEL6:
        t_pl2_ctrl_info.u.t_pl2_val.ui1_curr_val = 6;
        break;
    case SCC_AUD_PL2_C_WIDTH_LEVEL7_MAX_SPREAD:
        t_pl2_ctrl_info.u.t_pl2_val.ui1_curr_val = 7;
        break;
    default:
        return SMR_INV_ARG;
    }

    if ((t_pl2_ctrl_info.u.t_pl2_val.ui1_curr_val > t_pl2_ctrl_info.u.t_pl2_val.ui1_max_val)
        || (t_pl2_ctrl_info.u.t_pl2_val.ui1_curr_val < t_pl2_ctrl_info.u.t_pl2_val.ui1_min_val))
    {
         return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_PL2_CTRL,
        (VOID*)&t_pl2_ctrl_info,
        sizeof( AUD_DEC_PL2_CTRL_INFO_T ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_pl2_switch
 *
 * Description: get audio Pro Logic II switch
 *
 * Inputs:  h_aud                Audio component handle.
 *          pe_value             retrive mode value
 *
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_pl2_switch(
    HANDLE_T                        h_aud,
    SCC_AUD_PL2_SWITCH_T*           pe_value )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_PL2_CTRL_INFO_T      t_pl2_ctrl_info;
    SIZE_T                       z_size;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tpe_value        = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       pe_value ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_pl2_ctrl_info.e_ctrl = AUD_DEC_PL2_CTRL_SWITCH;

    z_size = sizeof( AUD_DEC_PL2_CTRL_INFO_T );
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_PL2_CTRL,
        (VOID*)&t_pl2_ctrl_info,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch (t_pl2_ctrl_info.u.e_pl2_switch)
    {
    case AUD_DEC_PL2_SWITCH_ON:
        *pe_value = SCC_AUD_PL2_SWITCH_ON;
        break;
    case AUD_DEC_PL2_SWITCH_OFF:
        *pe_value = SCC_AUD_PL2_SWITCH_OFF;
        break;
    case AUD_DEC_PL2_SWITCH_AUTO:
        *pe_value = SCC_AUD_PL2_SWITCH_AUTO;
        break;
    default:
        return SMR_INV_ARG;
    }
    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_pl2_mode
 *
 * Description: get audio Pro Logic II mode
 *
 * Inputs:  h_aud                Audio component handle.
 *          pe_value             retrive mode value
 *
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_pl2_mode(
    HANDLE_T                        h_aud,
    SCC_AUD_PL2_MODE_T*             pe_value )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_PL2_CTRL_INFO_T      t_pl2_ctrl_info;
    SIZE_T                       z_size;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tpe_value        = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       pe_value ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_pl2_ctrl_info.e_ctrl = AUD_DEC_PL2_CTRL_MODE;

    z_size = sizeof( AUD_DEC_PL2_CTRL_INFO_T );
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_PL2_CTRL,
        (VOID*)&t_pl2_ctrl_info,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch (t_pl2_ctrl_info.u.e_pl2_mode)
    {
    case AUD_DEC_PL2_MODE_MODE_PROLGIC_EMULATION:
        *pe_value = SCC_AUD_PL2_MODE_PROLGIC_EMULATION;
        break;
    case AUD_DEC_PL2_MODE_MODE_VIRTUAL:
        *pe_value = SCC_AUD_PL2_MODE_VIRTUAL;
        break;
    case AUD_DEC_PL2_MODE_MODE_MUSIC:
        *pe_value = SCC_AUD_PL2_MODE_MUSIC;
        break;
    case AUD_DEC_PL2_MODE_MODE_MOVIE:
        *pe_value = SCC_AUD_PL2_MODE_MOVIE;
        break;
    case AUD_DEC_PL2_MODE_MODE_MATRIX:
        *pe_value = SCC_AUD_PL2_MODE_MATRIX;
        break;
    case AUD_DEC_PL2_MODE_MODE_CUSTOM:
        *pe_value = SCC_AUD_PL2_MODE_CUSTOM;
        break;
    default:
        return SMR_INV_ARG;
    }
    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_pl2_panorama
 *
 * Description: get audio Pro Logic II panorama
 *
 * Inputs:  h_aud                Audio component handle.
 *          pe_value             retrive mode value
 *
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_pl2_panorama(
    HANDLE_T                        h_aud,
    SCC_AUD_PL2_PANORAMA_T*         pe_value )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_PL2_CTRL_INFO_T      t_pl2_ctrl_info;
    SIZE_T                       z_size;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tpe_value        = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       pe_value ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_pl2_ctrl_info.e_ctrl = AUD_DEC_PL2_CTRL_PANORAMA;

    z_size = sizeof( AUD_DEC_PL2_CTRL_INFO_T );
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_PL2_CTRL,
        (VOID*)&t_pl2_ctrl_info,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch (t_pl2_ctrl_info.u.b_is_pl2_panorama_on)
    {
    case TRUE:
        *pe_value = SCC_AUD_PL2_PANORAMA_ON;
        break;
    case FALSE:
        *pe_value = SCC_AUD_PL2_PANORAMA_OFF;
        break;
    default:
        return SMR_INV_ARG;
    }
    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_pl2_dimension
 *
 * Description: get audio Pro Logic II dimenstion
 *
 * Inputs:  h_aud                Audio component handle.
 *          pe_value             retrive mode value
 *
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_pl2_dimension(
    HANDLE_T                        h_aud,
    SCC_AUD_PL2_DIMENSION_T*        pe_value )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_PL2_CTRL_INFO_T      t_pl2_ctrl_info;
    SIZE_T                       z_size;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tpe_value        = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       pe_value ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_pl2_ctrl_info.e_ctrl = AUD_DEC_PL2_CTRL_DIMENSION;

    z_size = sizeof( AUD_DEC_PL2_CTRL_INFO_T );
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_PL2_CTRL,
        (VOID*)&t_pl2_ctrl_info,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch (t_pl2_ctrl_info.u.t_pl2_val.ui1_curr_val)
    {
    case 0:
        *pe_value = SCC_AUD_PL2_DIMENSION_LEVEL0_BACK;
        break;
    case 1:
        *pe_value = SCC_AUD_PL2_DIMENSION_LEVEL1;
        break;
    case 2:
        *pe_value = SCC_AUD_PL2_DIMENSION_LEVEL2;
        break;
    case 3:
        *pe_value = SCC_AUD_PL2_DIMENSION_LEVEL3;
        break;
    case 4:
        *pe_value = SCC_AUD_PL2_DIMENSION_LEVEL4;
        break;
    case 5:
        *pe_value = SCC_AUD_PL2_DIMENSION_LEVEL5;
        break;
    case 6:
        *pe_value = SCC_AUD_PL2_DIMENSION_LEVEL6_FRONT;
        break;
    default:
        return SMR_INV_ARG;
    }
    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_pl2_c_width
 *
 * Description: get audio Pro Logic II center width
 *
 * Inputs:  h_aud                Audio component handle.
 *          pe_value             retrive mode value
 *
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_pl2_c_width(
    HANDLE_T                        h_aud,
    SCC_AUD_PL2_C_WIDTH_T*          pe_value )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_PL2_CTRL_INFO_T      t_pl2_ctrl_info;
    SIZE_T                       z_size;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tpe_value        = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       pe_value ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_pl2_ctrl_info.e_ctrl = AUD_DEC_PL2_CTRL_C_WIDTH;

    z_size = sizeof( AUD_DEC_PL2_CTRL_INFO_T );
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_PL2_CTRL,
        (VOID*)&t_pl2_ctrl_info,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch (t_pl2_ctrl_info.u.t_pl2_val.ui1_curr_val)
    {
    case 0:
        *pe_value = SCC_AUD_PL2_C_WIDTH_LEVEL0_MIN_SPREAD;
        break;
    case 1:
        *pe_value = SCC_AUD_PL2_C_WIDTH_LEVEL1;
        break;
    case 2:
        *pe_value = SCC_AUD_PL2_C_WIDTH_LEVEL2;
        break;
    case 3:
        *pe_value = SCC_AUD_PL2_C_WIDTH_LEVEL3;
        break;
    case 4:
        *pe_value = SCC_AUD_PL2_C_WIDTH_LEVEL4;
        break;
    case 5:
        *pe_value = SCC_AUD_PL2_C_WIDTH_LEVEL5;
        break;
    case 6:
        *pe_value = SCC_AUD_PL2_C_WIDTH_LEVEL6;
        break;
    case 7:
        *pe_value = SCC_AUD_PL2_C_WIDTH_LEVEL7_MAX_SPREAD;
        break;
    default:
        return SMR_INV_ARG;
    }
    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_dolby_certi_mode
 *
 * Description: get dolby certify mode
 *
 * Inputs:  h_aud                Audio component handle.
 *          pb_mode              retrive mode
 *
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_dolby_certi_mode(
   HANDLE_T                        h_aud,
   BOOL*                           pb_mode )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    BOOL                         b_mode;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tpb_mode     = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pb_mode ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(BOOL);
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_DOLBY_CERTI_MODE,
        (VOID*)&b_mode,
        &z_size );
    if ( i4_ret != SMR_OK || (z_size != sizeof(BOOL)) )
    {
        return SMR_INV_GET_INFO;
    }

    *pb_mode = b_mode;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_dolby_certi_mode
 *
 * Description: set dolby certify mode
 *
 * Inputs:  h_aud                Audio component handle.
 *          b_mode               set mode value
 *
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_dolby_certi_mode(
    HANDLE_T                        h_aud,
    BOOL                            b_mode )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tb_mode     = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)b_mode ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_DOLBY_CERTI_MODE,
        (VOID*)(UINT32)b_mode,
        sizeof(BOOL) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_output_port_adj
 *
 * Description: set output port whether is adjust or not
 *
 * Inputs:  h_aud                Audio component handle.
 *          e_port               specify set outport
 *          b_is_fixed           specify is fixed or not for the port
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_output_port_adj(
    HANDLE_T                        h_aud,
    SCC_AUD_OUT_PORT_T              e_port,
    BOOL                            b_is_fixed )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_OUTPORT_ADJ_T        t_outport_adj;


    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud                  = 0x%.8X\r\n"
                       "\t\te_port                 = %d\r\n"
                       "\t\b_is_fixed            = %s )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)e_port,
                       (b_is_fixed)?"TRUE":"FALSE" ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch(e_port)
    {
    case SCC_AUD_OUT_PORT_2_CH:
        t_outport_adj.e_out_port = AUD_DEC_OUT_PORT_2_CH;
        break;
    case SCC_AUD_OUT_PORT_5_1_CH:
        t_outport_adj.e_out_port = AUD_DEC_OUT_PORT_5_1_CH;
        break;
    case SCC_AUD_OUT_PORT_SPDIF:
        t_outport_adj.e_out_port = AUD_DEC_OUT_PORT_SPDIF;
        break;
    case SCC_AUD_OUT_PORT_2_CH_BY_PASS:
        t_outport_adj.e_out_port = AUD_DEC_OUT_PORT_2_CH_BY_PASS;
        break;
    case SCC_AUD_OUT_PORT_SPEAKER:
        t_outport_adj.e_out_port = AUD_DEC_OUT_PORT_SPEAKER;
        break;
    case SCC_AUD_OUT_PORT_HEADPHONE:
        t_outport_adj.e_out_port = AUD_DEC_OUT_PORT_HEADPHONE;
        break;
    default:
        return SMR_INV_GET_INFO;
    }

    t_outport_adj.b_is_fixed = b_is_fixed;

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_OUTPORT_ADJ,
        (VOID*)&t_outport_adj,
        sizeof(AUD_DEC_OUTPORT_ADJ_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_output_port_adj
 *
 * Description: get output port whether is adjust or not
 *
 * Inputs:  h_aud                Audio component handle.
 *          e_port               specify set outport
 *
 * Outputs:
 *          pb_is_fixed          retrive is fixed or not for the port
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_output_port_adj(
    HANDLE_T                        h_aud,
    SCC_AUD_OUT_PORT_T              e_port,
    BOOL*                           pb_is_fixed )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_OUTPORT_ADJ_T        t_outport_adj;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud                  = 0x%.8X\r\n"
                       "\t\te_port                 = %d\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)e_port));


    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch(e_port)
    {
    case SCC_AUD_OUT_PORT_2_CH:
        t_outport_adj.e_out_port = AUD_DEC_OUT_PORT_2_CH;
        break;
    case SCC_AUD_OUT_PORT_5_1_CH:
        t_outport_adj.e_out_port = AUD_DEC_OUT_PORT_5_1_CH;
        break;
    case SCC_AUD_OUT_PORT_SPDIF:
        t_outport_adj.e_out_port = AUD_DEC_OUT_PORT_SPDIF;
        break;
    case SCC_AUD_OUT_PORT_2_CH_BY_PASS:
        t_outport_adj.e_out_port = AUD_DEC_OUT_PORT_2_CH_BY_PASS;
        break;
    case SCC_AUD_OUT_PORT_SPEAKER:
        t_outport_adj.e_out_port = AUD_DEC_OUT_PORT_SPEAKER;
        break;
    case SCC_AUD_OUT_PORT_HEADPHONE:
        t_outport_adj.e_out_port = AUD_DEC_OUT_PORT_HEADPHONE;
        break;
    default:
        return SMR_INV_GET_INFO;
    }

    z_size = sizeof( AUD_DEC_OUTPORT_ADJ_T );
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_OUTPORT_ADJ,
        (VOID*)&t_outport_adj,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    *pb_is_fixed = t_outport_adj.b_is_fixed;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_capability
 *
 * Description: get audio capability
 *
 * Inputs:  h_aud                Audio component handle.
 *          pt_capability        retrive the audio capability
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_capability(
    HANDLE_T                        h_aud,
    SCC_AUD_CAPABILITY_T*           pt_capability )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_CAPABILITY_INFO_T    t_drv_capa;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud                  = 0x%.8X\r\n"
                       "\t\tpt_capability          = 0x%.8X\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)pt_capability));


    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof( AUD_DEC_CAPABILITY_INFO_T );
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_CAPABILITY,
        (VOID*)&t_drv_capa,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    pt_capability->b_is_outport_adj = t_drv_capa.b_is_outport_adj;
    pt_capability->ui4_channel_type = t_drv_capa.ui4_channel_type;
    pt_capability->ui4_downmix_mode = t_drv_capa.ui4_downmix_mode;
    pt_capability->ui4_fmt          = t_drv_capa.ui4_fmt;
    pt_capability->ui4_sound_effect = t_drv_capa.ui4_sound_effect;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_bbe_mode
 *
 * Description: set bbe mode
 *
 * Inputs:  h_aud                Audio component handle.
 *          e_mode               set bbe mode.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_bbe_mode(
    HANDLE_T                        h_aud,
    SCC_AUD_BBE_MODE_T              e_mode )
{
    INT32               i4_ret;
    HANDLE_TYPE_T       e_type;
    SCC_AUD_COMP_T*     pt_aud_comp = NULL;
    AUD_DEC_BBE_MODE_T  e_drv_mode;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\te_mode           = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)e_mode ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch(e_mode)
    {
    case SCC_AUD_BBE_MODE_OFF :
        e_drv_mode = AUD_DEC_BBE_MODE_OFF;
        break;
    case SCC_AUD_BBE_MODE_BBE :
        e_drv_mode = AUD_DEC_BBE_MODE_BBE;
        break;
    case SCC_AUD_BBE_MODE_BBE_VIVA :
        e_drv_mode = AUD_DEC_BBE_MODE_BBE_VIVA;
        break;
    default:
        return SMR_INV_GET_INFO;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_BBE_MODE,
        (VOID*)(UINT32)e_drv_mode,
        sizeof( AUD_DEC_BBE_MODE_T ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_bbe_mode
 *
 * Description: set bbe mode
 *
 * Inputs:  h_aud                Audio component handle.
 *          pe_mode              get bbe mode.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_bbe_mode(
    HANDLE_T                        h_aud,
    SCC_AUD_BBE_MODE_T*             pe_mode )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_BBE_MODE_T           e_drv_mode;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud                  = 0x%.8X\r\n"
                       "\t\tpe_mode                = 0x%.8X\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)pe_mode));


    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof( AUD_DEC_BBE_MODE_T );
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_BBE_MODE,
        (VOID*)&e_drv_mode,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch(e_drv_mode)
    {
    case AUD_DEC_BBE_MODE_OFF :
        *pe_mode = SCC_AUD_BBE_MODE_OFF;
        break;
    case AUD_DEC_BBE_MODE_BBE :
        *pe_mode = SCC_AUD_BBE_MODE_BBE;
        break;
    case AUD_DEC_BBE_MODE_BBE_VIVA :
        *pe_mode = SCC_AUD_BBE_MODE_BBE_VIVA;
        break;
    default:
        return SMR_INV_GET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_hdphone_detect_nfy
 *
 * Description: set bbe mode
 *
 * Inputs:  h_aud                Audio component handle.
 *
 * Outputs: pt_hdph_nfy_info     Head Phone notify info.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_hdphone_detect_nfy(
    HANDLE_T                           h_aud,
    SCC_AUD_HDPHONE_DETECT_NFT_INFO_T* pt_hdph_nfy_info )
{
    INT32                               i4_ret;
    HANDLE_TYPE_T                       e_type;
    SCC_AUD_COMP_T*                     pt_aud_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud                  = 0x%.8X\r\n"
                       "\t\tpt_hdph_nfy_info       = 0x%.8X\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)pt_hdph_nfy_info));


    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    if ( !pt_hdph_nfy_info )
    {
        return SMR_INV_ARG;
    }

    pt_hdph_nfy_info->pf_hdphone_detect_nfy =
        pt_aud_comp->pf_aud_hdphone_detect_nfy;
    pt_hdph_nfy_info->pv_nfy_tag =
        pt_aud_comp->pf_aud_hdphone_detect_nfy_tag;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_hdphone_cond
 *
 * Description: set bbe mode
 *
 * Inputs:  h_aud                Audio component handle.
 *
 * Outputs: pe_hdphone_cond      Head Phone condition info.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_hdphone_cond(
    HANDLE_T                        h_aud,
    SCC_AUD_HDPHONE_COND_T*         pe_hdphone_cond )
{
    INT32                               i4_ret;
    SIZE_T                              z_size = 0;
    HANDLE_TYPE_T                       e_type;
    SCC_AUD_COMP_T*                     pt_aud_comp = NULL;
    AUD_DEC_HEADPHONE_COND_T            e_drv_hdph_cond;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud                  = 0x%.8X\r\n"
                       "\t\tpe_hdphone_cond        = 0x%.8X\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)pe_hdphone_cond));


    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    if ( !pe_hdphone_cond )
    {
        return SMR_INV_ARG;
    }

    z_size = sizeof( AUD_DEC_HEADPHONE_COND_T );
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_HEADPHONE_COND,
        (VOID*)(UINT32)&e_drv_hdph_cond,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch( e_drv_hdph_cond )
    {
    case AUD_DEC_HEADPHONE_COND_UNPLUGED:
        *pe_hdphone_cond = SCC_AUD_HDPHONE_COND_UNPLUGED;
        break;

    case AUD_DEC_HEADPHONE_COND_PLUGED:
        *pe_hdphone_cond = SCC_AUD_HDPHONE_COND_PLUGED;
        break;

    default:
        return SMR_INV_GET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_mute_ex
 *
 * Description: set bbe mode
 *
 * Inputs:  h_aud                    Audio component handle.
 *
 * Outputs: pui4_muted_out_port_msk  Muted output port bitmask.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_mute_ex(
    HANDLE_T                        h_aud,
    UINT32*                         pui4_muted_out_port_msk )
{
    INT32                               i4_ret;
    SIZE_T                              z_size = 0;
    HANDLE_TYPE_T                       e_type;
    SCC_AUD_COMP_T*                     pt_aud_comp = NULL;
    UINT32                              ui4_drv_muted_out_port_msk;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud                  = 0x%.8X\r\n"
                       "\t\tpui4_muted_out_port_msk= 0x%.8X\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)pui4_muted_out_port_msk));


    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    if ( !pui4_muted_out_port_msk )
    {
        return SMR_INV_ARG;
    }

    z_size = sizeof( UINT32 );
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_MUTE_EX,
        (VOID*)&ui4_drv_muted_out_port_msk,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    *pui4_muted_out_port_msk = 0;
    if ( ui4_drv_muted_out_port_msk & AUD_DEC_OUT_PORT_FLAG_2_CH )
    {
        *pui4_muted_out_port_msk |= SCC_AUD_OUT_PORT_2_CH;
    }
    if ( ui4_drv_muted_out_port_msk & AUD_DEC_OUT_PORT_FLAG_5_1_CH )
    {
        *pui4_muted_out_port_msk |= SCC_AUD_OUT_PORT_5_1_CH;
    }
    if ( ui4_drv_muted_out_port_msk & AUD_DEC_OUT_PORT_FLAG_SPDIF )
    {
        *pui4_muted_out_port_msk |= SCC_AUD_OUT_PORT_SPDIF;
    }
    if ( ui4_drv_muted_out_port_msk & AUD_DEC_OUT_PORT_FLAG_2_CH_BY_PASS )
    {
        *pui4_muted_out_port_msk |= SCC_AUD_OUT_PORT_2_CH_BY_PASS;
    }
    if ( ui4_drv_muted_out_port_msk & AUD_DEC_OUT_PORT_FLAG_SPEAKER )
    {
        *pui4_muted_out_port_msk |= SCC_AUD_OUT_PORT_SPEAKER;
    }
    if ( ui4_drv_muted_out_port_msk & AUD_DEC_OUT_PORT_FLAG_HEADPHONE )
    {
        *pui4_muted_out_port_msk |= SCC_AUD_OUT_PORT_HEADPHONE;
    }

    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_lineout_vol_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          ui1_mode              References.
 *          ui1_level         References.
 *          b_mute           References
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_lineout_vol_mode( 
    HANDLE_T                 h_aud,
   SCC_AUD_DEC_LINEOUT_VOL_T*  pt_lineout_vol_mode)
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_LINEOUT_VOL_T        t_lineout_vol_mode;
    
    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud                  = 0x%.8X\r\n"
                       "\t\te_mode                 = %d\r\n"
                       "\t\tui1_level                 = %d\r\n"
                       "\t\tb_mute            = %s )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT8)pt_lineout_vol_mode->e_mode,
                       (UINT8)pt_lineout_vol_mode->ui1_level,
                       (pt_lineout_vol_mode->b_mute)?"TRUE":"FALSE" ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }
    if ( !pt_lineout_vol_mode )
    {
        return SMR_INV_ARG;
    }

   switch( pt_lineout_vol_mode->e_mode)
   {
   case  SCC_AUD_DEC_LINEOUT_VOL_MODE_FIXED:
       t_lineout_vol_mode.e_mode = AUD_DEC_LINEOUT_VOL_MODE_FIXED;
       break;

   case SCC_AUD_DEC_LINEOUT_VOL_MODE_VARIABLE:
       t_lineout_vol_mode.e_mode = AUD_DEC_LINEOUT_VOL_MODE_VARIABLE;
       break;

   default:
       return SMR_INV_GET_INFO;
   }

   t_lineout_vol_mode.ui1_level  = pt_lineout_vol_mode->ui1_level;
   t_lineout_vol_mode.b_mute    = pt_lineout_vol_mode->b_mute;

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_LINOUT_VOL_MODE,
        (VOID*)&t_lineout_vol_mode,
        sizeof(AUD_DEC_LINEOUT_VOL_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_se_sph_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          e_reverb            References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_se_sph_mode(
    HANDLE_T                        h_aud,
    SCC_AUD_DEC_SE_SPH_MODE_T        e_mode )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_SE_INFO_T            t_drv_se_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\te_mode        = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)e_mode ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_se_info.e_se_type = AUD_DEC_SE_SPH_MODE;
    switch( e_mode )
    {
    case  SCC_AUD_DEC_SE_SPH_MODE_OFF:
        t_drv_se_info.pv_info = (VOID*)AUD_DEC_SE_SPH_MODE_OFF;
        break;

    case SCC_AUD_DEC_SE_SPH_MODE_DEFAULT:
        t_drv_se_info.pv_info = (VOID*)AUD_DEC_SE_SPH_MODE_DEFAULT;
        break;
        
    case SCC_AUD_DEC_SE_SPH_MODE_MODE1:
        t_drv_se_info.pv_info = (VOID*)AUD_DEC_SE_SPH_MODE_MODE1;
        break;
        
    case SCC_AUD_DEC_SE_SPH_MODE_MODE2:
        t_drv_se_info.pv_info = (VOID*)AUD_DEC_SE_SPH_MODE_MODE2;
        break;
    default:
        return SMR_INV_GET_INFO;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_SOUND_EFFECT,
        (VOID*)&t_drv_se_info,
        sizeof( AUD_DEC_SE_INFO_T ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }


    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_se_volume_offset
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          ui1_balance         References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_se_volume_offset(
    HANDLE_T                        h_aud,
    UINT8                           ui1_offset )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_SE_INFO_T            t_drv_se_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tui1_offset     = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       ui1_offset ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_se_info.e_se_type = AUD_DEC_SE_VOLUME_OFFSET;
    t_drv_se_info.pv_info = (VOID*)(UINT32)ui1_offset;

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_SOUND_EFFECT,
        (VOID*)&t_drv_se_info,
        sizeof( AUD_DEC_SE_INFO_T ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_se_hp_volume_offset
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          ui1_balance         References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_se_hp_volume_offset(
    HANDLE_T                        h_aud,
    UINT8                           ui1_offset )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_SE_INFO_T            t_drv_se_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tui1_offset      = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       ui1_offset ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_se_info.e_se_type = AUD_DEC_SE_HP_VOLUME_OFFSET;
    t_drv_se_info.pv_info = (VOID*)(UINT32)ui1_offset;

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_SOUND_EFFECT,
        (VOID*)&t_drv_se_info,
        sizeof( AUD_DEC_SE_INFO_T ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_PEQ_info
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          ui1_mode              References.
 *          ui1_level         References.
 *          b_mute           References
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_peq_info( 
    HANDLE_T                 h_aud,
   SCC_AUD_DEC_SET_TYPE_PEQ_INFO_T*  pt_peq_info)
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_SET_TYPE_PEQ_INFO_T  t_drv_peq_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud                  = 0x%.8X\r\n"
                       "\t\tui1_PEQSet                 = %d\r\n"
                       "\t\tpt_peq_info->ui4_Frequency       = 0x%.8X\r\n"
                       "\t\tui2_BW            = %d )\r\n"
                       "\t\ti2_Gain            = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT8)pt_peq_info->ui1_PEQSet,
                       (UINT32)pt_peq_info->ui4_Frequency,
                       (UINT16)pt_peq_info->ui2_BW,
                       (INT16)pt_peq_info->i2_Gain
                       ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }
    if ( !pt_peq_info )
    {
        return SMR_INV_ARG;
    }

   t_drv_peq_info.ui1_PEQSet= pt_peq_info->ui1_PEQSet;
   t_drv_peq_info.ui4_Frequency= pt_peq_info->ui4_Frequency;
   t_drv_peq_info.ui2_BW= pt_peq_info->ui2_BW;
   t_drv_peq_info.i2_Gain= pt_peq_info->i2_Gain;

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_PEQ_IND,
        (VOID*)&t_drv_peq_info,
        sizeof(AUD_DEC_SET_TYPE_PEQ_INFO_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_ovm
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          ui1_value            References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_ovm(
    HANDLE_T                        h_aud,
    UINT32                           ui4_value )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tui1_value        = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       ui4_value ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_OVM,
        (VOID*)(UINT32)ui4_value,
        sizeof( UINT32 ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_btnsnd_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          e_mts               e_btnsnd_mode.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_btnsnd_mode( HANDLE_T                  h_aud,
                         SCC_AUD_BTNSND_MODE_T             e_btnsnd_mode )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_BTNSND_INFO_T        t_drv_btnsnd_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\te_btnsnd_mode           = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)e_btnsnd_mode ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }
    
    t_drv_btnsnd_info.e_btnsnd_type= AUD_DEC_BTNSND_SRC;
    switch (e_btnsnd_mode)
    {
    case SCC_AUD_BTNSND_MODE_CUSTOM1:
        t_drv_btnsnd_info.pv_info = (VOID*)AUD_DEC_BTNSND_MODE_CUSTOM1;
        break;
    case SCC_AUD_BTNSND_MODE_CUSTOM2:
        t_drv_btnsnd_info.pv_info = (VOID*)AUD_DEC_BTNSND_MODE_CUSTOM2;
        break;
    case SCC_AUD_BTNSND_MODE_CUSTOM3:
        t_drv_btnsnd_info.pv_info = (VOID*)AUD_DEC_BTNSND_MODE_CUSTOM3;
        break;
    case SCC_AUD_BTNSND_MODE_CUSTOM4:
        t_drv_btnsnd_info.pv_info = (VOID*)AUD_DEC_BTNSND_MODE_CUSTOM4;
        break;

    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_BTN_SOUND_CTRL,
        (VOID*)&t_drv_btnsnd_info,
        sizeof(AUD_DEC_BTNSND_INFO_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_btnsnd_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs:  pe_btnsnd_mode
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_btnsnd_mode( HANDLE_T                  h_aud,
                         SCC_AUD_BTNSND_MODE_T*             pe_btnsnd_mode )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_BTNSND_INFO_T        t_drv_btnsnd_info;
    SIZE_T                       z_size;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tpe_btnsnd_mode        = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       pe_btnsnd_mode ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_btnsnd_info.e_btnsnd_type = AUD_DEC_BTNSND_SRC;
    t_drv_btnsnd_info.pv_info           = NULL;

    z_size = sizeof( AUD_DEC_BTNSND_INFO_T );
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_BTN_SOUND_CTRL,
        (VOID*)&t_drv_btnsnd_info,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch ((AUD_DEC_BTNSND_MODE_T)(UINT32)t_drv_btnsnd_info.pv_info )
    {
    case AUD_DEC_BTNSND_MODE_CUSTOM1:
        *pe_btnsnd_mode = SCC_AUD_BTNSND_MODE_CUSTOM1;
        break;
    case AUD_DEC_BTNSND_MODE_CUSTOM2:
        *pe_btnsnd_mode = SCC_AUD_BTNSND_MODE_CUSTOM2;
        break;
    case AUD_DEC_BTNSND_MODE_CUSTOM3:
        *pe_btnsnd_mode = SCC_AUD_BTNSND_MODE_CUSTOM3;
        break;
    case AUD_DEC_BTNSND_MODE_CUSTOM4:
        *pe_btnsnd_mode = SCC_AUD_BTNSND_MODE_CUSTOM4;
            break;
    default:
        return SMR_INV_ARG;
    }
    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_btnsnd_play
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          e_mts               e_btnsnd_mode.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_btnsnd_play( HANDLE_T                  h_aud )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_BTNSND_INFO_T        t_drv_btnsnd_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }
    
    t_drv_btnsnd_info.e_btnsnd_type= AUD_DEC_BTNSND_PLAY;
    t_drv_btnsnd_info.pv_info = NULL;

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_BTN_SOUND_CTRL,
        (VOID*)&t_drv_btnsnd_info,
        sizeof(AUD_DEC_BTNSND_INFO_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_btnsnd_enable
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          e_btnsnd_enable               References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_btnsnd_enable( HANDLE_T                  h_aud,
                         SCC_AUD_BTNSND_ENABLE_T             e_btnsnd_enable )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_BTNSND_INFO_T        t_drv_btnsnd_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\te_btnsnd_enable           = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)e_btnsnd_enable ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }
    
    t_drv_btnsnd_info.e_btnsnd_type= AUD_DEC_BTNSND_ENABLE;
    switch (e_btnsnd_enable)
    {
    case SCC_AUD_BTNSND_ENABLE_ON:
        t_drv_btnsnd_info.pv_info = (VOID*)AUD_DEC_BTNSND_ENABLE_ON;
        break;
    case SCC_AUD_BTNSND_ENABLE_OFF:
        t_drv_btnsnd_info.pv_info = (VOID*)AUD_DEC_BTNSND_ENABLE_OFF;
        break;

    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_BTN_SOUND_CTRL,
        (VOID*)&t_drv_btnsnd_info,
        sizeof(AUD_DEC_BTNSND_INFO_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_btnsnd_enable
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs:  pe_btnsnd_enable
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_btnsnd_enable( HANDLE_T                  h_aud,
                         SCC_AUD_BTNSND_ENABLE_T*             pe_btnsnd_enable )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_BTNSND_INFO_T        t_drv_btnsnd_info;
    SIZE_T                       z_size;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tpe_btnsnd_enable        = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       pe_btnsnd_enable ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    t_drv_btnsnd_info.e_btnsnd_type = AUD_DEC_BTNSND_ENABLE;
    t_drv_btnsnd_info.pv_info           = NULL;

    z_size = sizeof( AUD_DEC_BTNSND_INFO_T );
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_BTN_SOUND_CTRL,
        (VOID*)&t_drv_btnsnd_info,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch ((AUD_DEC_BTNSND_ENABLE_T)(UINT32)t_drv_btnsnd_info.pv_info )
    {
    case AUD_DEC_BTNSND_ENABLE_ON:
        *pe_btnsnd_enable = SCC_AUD_BTNSND_ENABLE_ON;
        break;
    case AUD_DEC_BTNSND_ENABLE_OFF:
        *pe_btnsnd_enable = SCC_AUD_BTNSND_ENABLE_OFF;
        break;
    default:
        return SMR_INV_ARG;
    }
    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_av_sync_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          e_av_sync_mode               References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_av_sync_mode( HANDLE_T                  h_aud,
                         SCC_AUD_DEC_AV_SYNC_INFO_T             e_av_sync_mode )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_AV_SYNC_INFO_T       e_drv_av_sync_mode;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\te_av_sync_mode           = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)e_av_sync_mode ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }
    
    switch (e_av_sync_mode)
    {
    case SCC_AUD_DEC_AV_SYNC_FREE_RUN:
        e_drv_av_sync_mode = AUD_DEC_AV_SYNC_FREE_RUN;
        break;
    case SCC_AUD_DEC_AV_SYNC_AUD_SLAVE:
        e_drv_av_sync_mode = AUD_DEC_AV_SYNC_AUD_SLAVE;
        break;
    case SCC_AUD_DEC_AV_SYNC_AUD_MASTER:
        e_drv_av_sync_mode = AUD_DEC_AV_SYNC_AUD_MASTER;
        break;

    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_AV_SYNC,
        (VOID*)e_drv_av_sync_mode,
        sizeof(AUD_DEC_AV_SYNC_INFO_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_av_sync_mode
 *
 * Description: get audio av sync mode
 *
 * Inputs:  h_aud                Audio component handle.
 *          pe_value             retrive mode value
 *
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_av_sync_mode(
    HANDLE_T                        h_aud,
    SCC_AUD_DEC_AV_SYNC_INFO_T*             pe_av_sync_mode )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_AV_SYNC_INFO_T       e_drv_av_sync_mode;
    SIZE_T                       z_size;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tpe_av_sync_mode        = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       pe_av_sync_mode ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof( AUD_DEC_AV_SYNC_INFO_T );
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_AV_SYNC,
        (VOID*)&e_drv_av_sync_mode,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    switch (e_drv_av_sync_mode)
    {
    case AUD_DEC_AV_SYNC_FREE_RUN:
        *pe_av_sync_mode = SCC_AUD_DEC_AV_SYNC_FREE_RUN;
        break;
    case AUD_DEC_AV_SYNC_AUD_SLAVE:
        *pe_av_sync_mode = SCC_AUD_DEC_AV_SYNC_AUD_SLAVE;
        break;
    case AUD_DEC_AV_SYNC_AUD_MASTER:
        *pe_av_sync_mode = SCC_AUD_DEC_AV_SYNC_AUD_MASTER;
        break;
    default:
        return SMR_INV_ARG;
    }
    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_power_on_off_music_volume
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pt_volume           References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_power_on_off_music_volume( HANDLE_T                      h_aud,
                            SCC_AUD_VOLUME_INFO_T*        pt_volume )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_VOLUME_INFO_T        t_aud_vol_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tpt_volume       = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pt_volume ));

    if ( !pt_volume )
    {
        return SMR_INV_ARG;
    }

    if ((pt_volume->e_out_port == SCC_AUD_OUT_PORT_2_CH) &&
        (pt_volume->e_ch != SCC_AUD_CHANNEL_FRONT_LEFT) &&
        (pt_volume->e_ch != SCC_AUD_CHANNEL_FRONT_RIGHT) &&
        (pt_volume->e_ch != SCC_AUD_CHANNEL_ALL))
    {
        SM_SLCTR_DBG_ERR((
            "[SCC] %s:%d, ERROR!! SCC_AUD_OUT_PORT_2_CH with invalid channel = %u.\r\n",
            __FUNCTION__, __LINE__,
            pt_volume->e_ch ));
        return SMR_INV_GET_INFO;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    /* Set input parameters */

    if ( pt_volume->e_ch == SCC_AUD_CHANNEL_ALL )
    {
        t_aud_vol_info.e_vol_type = AUD_DEC_ALL_CH;
    }
    else
    {
        t_aud_vol_info.e_vol_type = AUD_DEC_INDIVIDUAL_CH;

        t_aud_vol_info.u.t_ch_vol.e_out_port =
            _scc_aud_outport_2_drv_aud_outport( pt_volume->e_out_port );
        t_aud_vol_info.u.t_ch_vol.e_ls =
            _scc_aud_ch_2_drv_aud_ch( pt_volume->e_ch );
    }

    z_size = sizeof(AUD_DEC_VOLUME_INFO_T);
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_POWER_ON_OFF_MUSIC_VOLUME,
        (VOID*)&t_aud_vol_info,
        &z_size );
    if ( i4_ret != SMR_OK || (z_size != sizeof(AUD_DEC_VOLUME_INFO_T)) )
    {
        return SMR_INV_GET_INFO;
    }

    if ( pt_volume->e_ch == SCC_AUD_CHANNEL_ALL )
    {
        pt_volume->ui1_volumn = t_aud_vol_info.u.ui1_level;
    }
    else
    {
        pt_volume->ui1_volumn = t_aud_vol_info.u.t_ch_vol.ui1_level;
    }
    if ( pt_volume->ui1_volumn > 100 )
    {
        pt_volume->ui1_volumn = 100;
    }

    return SMR_OK;
}


INT32 x_scc_aud_set_upload_ctrl(
    HANDLE_T                        h_aud,
    BOOL                            b_is_on )
{
#ifdef ENABLE_AUD_RECORD_SUPPORT
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;  
    

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tb_is_on         = %s )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       ((b_is_on)?"TRUE":"FALSE") ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }
    x_dbg_stmt("x_scc_aud_set_upload_ctrl b_is_on=%d\r\n",b_is_on);
    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_UPLOAD_CTRL,
        (VOID*)(UINT32)b_is_on,
        sizeof( AUD_DEC_UPLOAD_MODE_T ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }
#else
    UNUSED(h_aud);
    UNUSED(b_is_on);
    x_dbg_stmt("x_scc_aud_set_upload_ctrl Error! CC_AUD_DATA_UPLOAD_SUPPORT not defined!!!!! \r\n");
#endif
    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_upload_data_buffer_info
 *
 * Description: get audio capability
 *
 * Inputs:  h_aud                Audio component handle.
 *          pt_buffer_info        retrive the audio upload buffer info
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_upload_data_buffer_info(
    HANDLE_T                        h_aud,
    SCC_AUD_UPLOAD_DATA_BUFFER_INFO_T*           pt_buffer_info )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_BUFFER_INFO_T        t_drv_buffer_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud                  = 0x%.8X\r\n"
                       "\t\tpt_buffer_info          = 0x%.8X\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)pt_buffer_info));


    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof( AUD_DEC_BUFFER_INFO_T );
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_BUFFER_INFO,
        (VOID*)&t_drv_buffer_info,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    pt_buffer_info->ui4_buffer_start_addr= t_drv_buffer_info.ui4_buffer_start_Addr;
    pt_buffer_info->ui4_buffer_size   = t_drv_buffer_info.ui4_bufer_Size;

    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_hdphone_detect_nfy
 *
 * Description: <function description>
 *
 * Inputs:  h_aud                   Audio Component handle.
 *          pt_hdph_nfy_info        Head Phone notify information.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_upload_data_nfy(
    HANDLE_T                        h_aud,
    SCC_AUD_UPLOAD_DATA_NFT_INFO_T* pt_upload_data_nfy_info )
{
    INT32                            i4_ret;
    HANDLE_TYPE_T                    e_type;
    SCC_AUD_COMP_T*                  pt_aud_comp = NULL;
    AUD_DEC_UPLOAD_DATA_NFT_INFO_T   t_drv_upload_data_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud             = 0x%.8X\r\n"
                       "\t\tpt_upload_data_nfy_info  = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pt_upload_data_nfy_info ));

    if ( !pt_upload_data_nfy_info )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }
    
        pt_aud_comp->pf_aud_upload_data_nfy=
        pt_upload_data_nfy_info->pf_upload_data_nfy;
    pt_aud_comp->pf_aud_upload_data_nfy_tag =
        pt_upload_data_nfy_info->pv_nfy_tag;

    t_drv_upload_data_info.pf_upload_data_nfy =
        _scc_comp_aud_upload_data_nfy_fct;
    t_drv_upload_data_info.pv_nfy_tag= (VOID*)(UINT32)h_aud;

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_UPLOAD_DATA_NFY,
        (VOID*)&t_drv_upload_data_info,
        sizeof( AUD_DEC_UPLOAD_DATA_NFT_INFO_T ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_get_upload_pcm_info
 *
 * Description: <function description>
 *
 * Inputs:  h_aud        Audio component handle.
 *
 * Outputs: pt_pcm_info  References
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_get_upload_pcm_info( 
    HANDLE_T                        h_aud,
    SCC_AUD_UPLOAD_PCM_INFO_T*      pt_pcm_info )
{
    INT32                        i4_ret;
    SIZE_T                       z_size = 0;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_UPLOAD_PCM_INFO_T    t_drv_upload_pcm_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud          = 0x%.8X\r\n"
                       "\t\tpt_pcm_info    = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pt_pcm_info ));

    if ( pt_pcm_info == NULL )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_aud_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    z_size = sizeof(AUD_DEC_UPLOAD_PCM_INFO_T);
    i4_ret = _scc_comp_aud_get(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_GET_TYPE_UPLOAD_PCM_INFO,
        (VOID*)&t_drv_upload_pcm_info,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    pt_pcm_info->ui4_pcm_bits= t_drv_upload_pcm_info.ui4_pcm_bits;
    pt_pcm_info->ui4_pcm_channels = t_drv_upload_pcm_info.ui4_pcm_channels;
    pt_pcm_info->ui4_pcm_sample_rate = t_drv_upload_pcm_info.ui4_pcm_sample_rate;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_video_scramble_status
 *
 * Description: 
 *
 * Inputs: 
 *         
 *
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_video_scramble_status(
    HANDLE_T                        h_aud,
    BOOL                            b_scrambled )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tb_scrambled     = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)b_scrambled ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_VIDEO_SCRAMBLE_STATUS,
        (VOID*)(UINT32)b_scrambled,
        sizeof(BOOL) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_protect_info
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          e_protect_info      References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_protect_info( HANDLE_T               h_aud,
                            SCC_PROTECT_INFO_T           e_protect_info )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;
    AUD_DEC_PROTECT_INFO_T       e_drv_protect_info;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\te_protect_info  = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)e_protect_info ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_aud_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    switch ( e_protect_info )
    {
    case SCC_PROTECT_INFO_DMUTE_AMUTE:
        e_drv_protect_info = AUD_DEC_PROTECT_INFO_DMUTE_AMUTE;
        break;
    case SCC_PROTECT_INFO_DUNMUTE_AMUTE:
        e_drv_protect_info = AUD_DEC_PROTECT_INFO_DUNMUTE_AMUTE;
        break;
    case SCC_PROTECT_INFO_DUNMUTE_AUNMUTE:
        e_drv_protect_info = AUD_DEC_PROTECT_INFO_DUNMUTE_AUNMUTE;
        break;
    case SCC_PROTECT_INFO_DMUTE_AUNMUTE:
        e_drv_protect_info = AUD_DEC_PROTECT_INFO_DMUTE_AUNMUTE;
        break;
    case SCC_PROTECT_INFO_UNKNOWN:
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_PROTECT_INFO,
        (VOID*)e_drv_protect_info,
        sizeof(AUD_DEC_PROTECT_INFO_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _scc_audio_karaoke_nfy_fct
 *
 * Description: <function description>
 *
 * Inputs:  pv_nfy_tag References.
 *          e_nfy_cond References.
 *          ui4_data_1 References.
 *          ui4_data_2 References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _scc_audio_karaoke_nfy_fct(
    VOID*           pv_nfy_tag,
    AUD_DEC_COND_T  e_nfy_cond,
    UINT32          ui4_data_1,
    UINT32          ui4_data_2 )
{
   /*do nothing currently*/
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_karaoke
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          pt_se_trim          References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_karaoke(
    HANDLE_T                        h_aud,
    SCC_AUD_KARAOKE_INFO_T*         pt_karaoke_info )
{
    INT32                       i4_ret;
    HANDLE_TYPE_T               e_type;
    SCC_AUD_COMP_T*             pt_aud_comp = NULL;
    AUD_DEC_KARAOKE_INFO_T      t_drv_karaoke_info;
    AUD_DEC_KARAOKE_CTRL_T      e_drv_karaoke_ctrl;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud              = 0x%.8X\r\n"
                       "\t\tpt_karaoke_info    = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)pt_karaoke_info ));

    if ( !pt_karaoke_info )
    {
        return SMR_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    e_drv_karaoke_ctrl = _scc_karaoke_ctrl_2_drv_karaoke_ctrl(pt_karaoke_info->e_karaoke_type); 

    t_drv_karaoke_info.e_karaoke_type = e_drv_karaoke_ctrl;

    switch(e_drv_karaoke_ctrl)
    {
    case AUD_DEC_KARAOKE_ENABLE:
        t_drv_karaoke_info.u.fg_enable =pt_karaoke_info->u.b_karaoke_enable;
        break;
    case AUD_DEC_KARAOKE_ECHO:
        t_drv_karaoke_info.u.ui2_echo_val =pt_karaoke_info->u.ui2_echo_val;
        break;
    case AUD_DEC_KARAOKE_VOCAL_CANEL:
        t_drv_karaoke_info.u.fg_vocal_cancel =pt_karaoke_info->u.b_vocal_cancel;
        break;
    case AUD_DEC_KARAOKE_KEY_SHIFT:
        t_drv_karaoke_info.u.ui2_key_shift =pt_karaoke_info->u.ui2_key_shift;
        break;
    case AUD_DEC_KARAOKE_MIC_VOL1:
        t_drv_karaoke_info.u.ui2_mic_vol1 =pt_karaoke_info->u.ui2_mic_vol1;
        break;
    case AUD_DEC_KARAOKE_MIC_VOL2:
        t_drv_karaoke_info.u.ui2_mic_vol2 =pt_karaoke_info->u.ui2_mic_vol2;
        break;
    case AUD_DEC_KARAOKE_CONNECT:
        break;
    default:
        x_dbg_stmt("[SCC] Invalid input source info \r\n");
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_KARAOKE,
        (VOID*)&t_drv_karaoke_info,
        sizeof( AUD_DEC_KARAOKE_INFO_T ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }
    
    if( e_drv_karaoke_ctrl == AUD_DEC_KARAOKE_CONNECT)
    {
        AUD_DEC_NFY_INFO_T  t_set_nfy_info;
        
        t_set_nfy_info.pf_aud_dec_nfy = _scc_audio_karaoke_nfy_fct;
        t_set_nfy_info.pv_tag = (VOID*)h_aud;
        /* set notify function */
        i4_ret = _scc_comp_aud_set(
            h_aud,
            DRVT_AUD_DEC,
            AUD_DEC_SET_TYPE_NFY_FCT,
            (VOID*)&t_set_nfy_info,
            sizeof( AUD_DEC_NFY_INFO_T ) );
        if ( i4_ret != SMR_OK )
        {
            return SMR_INV_SET_INFO;
        }
    
        i4_ret = _scc_comp_aud_set(
            h_aud,
            DRVT_AUD_DEC,
            AUD_DEC_SET_TYPE_CTRL,
            (VOID*)AUD_DEC_CTRL_PLAY,
            sizeof( AUD_DEC_CTRL_T ) );
        if ( i4_ret != SMR_OK )
        {
            return SMR_INV_SET_INFO;
        }
    }
    
    if( e_drv_karaoke_ctrl == AUD_DEC_KARAOKE_ENABLE 
        && pt_karaoke_info->u.b_karaoke_enable ==FALSE)
    {
        i4_ret = _scc_comp_aud_set(
            h_aud,
            DRVT_AUD_DEC,
            AUD_DEC_SET_TYPE_CTRL,
            (VOID*)AUD_DEC_CTRL_STOP,
            sizeof( AUD_DEC_CTRL_T ) );
        if ( i4_ret != SMR_OK )
        {
            return SMR_INV_SET_INFO;
        }
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_out_port_link
 *
 * Description: set out port link enable or disable
 *
 * Inputs:  h_aud                Audio component handle.
 *          b_enable             link enable or not.
 *
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_out_port_link(
    HANDLE_T                        h_aud,
    BOOL                            b_enable )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud      = 0x%.8X\r\n"
                       "\t\tb_mode     = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)b_enable ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_OUT_PORT_LINK,
        (VOID*)(UINT32)b_enable,
        sizeof(BOOL) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_dc_power_off
 *
 * Description: set dc power off enable or disable
 *
 * Inputs:  h_aud                Audio component handle.
 *          b_enable             enable or not.
 *
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_dc_power_off(
    HANDLE_T                        h_aud,
    BOOL                            b_enable )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_AUD_COMP_T*              pt_aud_comp = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\tb_mode     = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (UINT32)b_enable ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || pt_aud_comp == NULL )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_DC_POWER_OFF,
        (VOID*)(UINT32)b_enable,
        sizeof(BOOL) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_GET_INFO;
    }

    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_endian_mode
 *
 * Description: set endian mode (little or big)
 *
 * Inputs:  h_aud                Audio component handle.
 *          e_mode               set bbe mode.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_endian_mode(
    HANDLE_T                        h_aud,
    SCC_AUD_ENDIAN_MODE_T              e_mode )
{
    INT32               i4_ret;
    HANDLE_TYPE_T       e_type;
    SCC_AUD_COMP_T*     pt_aud_comp = NULL;
    AUD_DEC_ENDIAN_MODE_T  e_drv_mode;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\te_mode           = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)e_mode ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch(e_mode)
    {
    case SCC_AUD_ENDIAN_MODE_LITTLE:
        e_drv_mode = AUD_DEC_ENDIAN_MODE_LITTLE;
        break;
    case SCC_AUD_ENDIAN_MODE_BIG:
        e_drv_mode = AUD_DEC_ENDIAN_MODE_BIG;
        break;
    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_ENDIAN_MODE,
        (VOID*)(UINT32)e_drv_mode,
        sizeof( AUD_DEC_ENDIAN_MODE_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_out_device
 *
 * Description: set out device 
 *
 * Inputs:  h_aud                Audio component handle.
 *          e_mode                Out device.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_aud_set_out_device(
    HANDLE_T                        h_aud,
    SCC_AUD_OUT_DEVICE_T              e_out_device )
{
    INT32               i4_ret;
    HANDLE_TYPE_T       e_type;
    SCC_AUD_COMP_T*     pt_aud_comp = NULL;
    AUD_DEC_OUT_DEVICE_T  e_drv_out_device;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_aud           = 0x%.8X\r\n"
                       "\t\te_mode           = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_aud,
                       (INT32)e_out_device ));

    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        return SMR_INV_HANDLE;
    }

    switch(e_out_device )
    {
    case SCC_AUD_OUT_DEVICE_BUILD_SPEAKER:
        e_drv_out_device = AUD_DEC_OUT_DEVICE_BUILD_SPEAKER;
        break;
    case SCC_AUD_OUT_DEVICE_USB_HEADPHONE:
        e_drv_out_device = AUD_DEC_OUT_DEVICE_USB_HEADPHONE;
        break;
    case SCC_AUD_OUT_DEVICE_BLUETOOTH_EARPHONE:
        e_drv_out_device = AUD_DEC_OUT_DEVICE_BLUETOOTH_EARPHONE;
        break;

    default:
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_aud_set(
        h_aud,
        DRVT_AUD_DEC,
        AUD_DEC_SET_TYPE_OUT_DEVICE,
        (VOID*)(UINT32)e_drv_out_device,
        sizeof( AUD_DEC_OUT_DEVICE_T) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_pmx_lock
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
VOID x_scc_disp_pmx_lock(VOID)
{
    _scc_pmx_lock();
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_disp_pmx_unlock
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
VOID x_scc_disp_pmx_unlock(VOID)
{
    _scc_pmx_unlock();
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_query_bootup_status
 *
 * Description: <function description> for boot status query
 *
 * Inputs: ui1_mode 
 *            Bit 0 for Sound, Bit 1 for Logo
 *
 * Outputs: pui1_status
 *            Bit 0 for Sound, Bit 1 for Logo
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_query_bootup_status(UINT8  ui1_mode, UINT8* pui1_status)
{
    INT32                           i4_ret;
    SIZE_T                          z_size_1;
    SIZE_T                          z_size_2;
    DRV_CUST_OPERATION_INFO_T       t_op_info;    
    BOOL                            b_is_finished;

    if (pui1_status == NULL)
    {
        return SMR_INV_ARG;
    }
    *pui1_status = 0;
    z_size_1 = sizeof(BOOL);

    if ((ui1_mode & SCC_BOOTUP_SOUND) > 0)
    {
        b_is_finished = TRUE;
        x_memset(&t_op_info, 
                 0, 
                 sizeof(DRV_CUST_OPERATION_INFO_T));
        t_op_info.e_op_type = DRV_CUSTOM_OPERATION_TYPE_MISC_GET;
        t_op_info.u.t_misc_get_info.e_misc_type = DRV_CUSTOM_MISC_TYPE_STARTUP_SOUND_STATUS;
        t_op_info.u.t_misc_get_info.pv_get_info = (VOID*)&b_is_finished;
        t_op_info.u.t_misc_get_info.pz_size = &z_size_1;
        
        z_size_2 = sizeof(DRV_CUST_OPERATION_INFO_T); 
        i4_ret = x_rm_get_comp(
                    DRVT_CUST_DRV,
                    DRV_CUST_COMP_ID,
                    FALSE,
                    ANY_PORT_NUM,
                    0,
                    &t_op_info,
                    &z_size_2
                    );
        if (i4_ret != RMR_OK)
        {
            x_dbg_stmt("[bootup status query] Get startup sound Fail! i4_ret=%d\r\n", i4_ret);
            return SMR_INV_GET_INFO;
        }
        
        if (b_is_finished)
        {
            *pui1_status |= SCC_BOOTUP_SOUND;
        }
    }

    if ((ui1_mode & SCC_BOOTUP_LOGO) > 0)
    {
        b_is_finished = TRUE;
        x_memset(&t_op_info, 
                 0, 
                 sizeof(DRV_CUST_OPERATION_INFO_T));
        t_op_info.e_op_type = DRV_CUSTOM_OPERATION_TYPE_MISC_GET;
        t_op_info.u.t_misc_get_info.e_misc_type = DRV_CUSTOM_MISC_TYPE_STARTUP_LOGO_STATUS;
        t_op_info.u.t_misc_get_info.pv_get_info = (VOID*)&b_is_finished;
        t_op_info.u.t_misc_get_info.pz_size = &z_size_1;
        
        z_size_2 = sizeof(DRV_CUST_OPERATION_INFO_T); 
        i4_ret = x_rm_get_comp(
                    DRVT_CUST_DRV,
                    DRV_CUST_COMP_ID,
                    FALSE,
                    ANY_PORT_NUM,
                    0,
                    &t_op_info,
                    &z_size_2
                    );
        if (i4_ret != RMR_OK)
        {
            x_dbg_stmt("[bootup status query] Get startup logo Fail! i4_ret=%d\r\n", i4_ret);
            return SMR_INV_GET_INFO;
        }

        if (b_is_finished)
        {
            *pui1_status |= SCC_BOOTUP_LOGO;
        }
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_set_crnt_inp_src_info
 *
 * Description: <function description> for set current input source info
 *
 * Inputs: e_inp_src_type       References
 *            ui1_port_num     References
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scc_set_crnt_inp_src_info(
    SCC_CRNT_INP_SRC_TYPE_T             e_inp_src_type, 
    UINT8                               ui1_port_num)
{
    INT32                               i4_ret;
#if 0 /* sync */
    SIZE_T                              z_size_1;
    SIZE_T                              z_size_2;
    DRV_CUST_OPERATION_INFO_T           t_op_info;
#else /* async */
    SM_SCC_MSG_T                        t_msg;
#endif

#if 0 /* sync */
    DRV_CUSTOM_MISC_CRNT_INP_SRC_INFO_T t_crnt_inp_src_info;
    
    z_size_1 = sizeof(DRV_CUSTOM_MISC_CRNT_INP_SRC_INFO_T);
    x_memset ( &t_crnt_inp_src_info, 
                0, 
                z_size_1);
    switch (e_inp_src_type)
    {
    case SCC_CRNT_INP_SRC_TV:
        t_crnt_inp_src_info.e_type = DRV_CUSTOM_MISC_CRNT_INP_SRC_TV;
        break;
    case SCC_CRNT_INP_SRC_COMP_VIDEO:
        t_crnt_inp_src_info.e_type = DRV_CUSTOM_MISC_CRNT_INP_SRC_COMP_VIDEO;
        break;
    case SCC_CRNT_INP_SRC_S_VIDEO:
        t_crnt_inp_src_info.e_type = DRV_CUSTOM_MISC_CRNT_INP_SRC_S_VIDEO;
        break;
    case SCC_CRNT_INP_SRC_Y_PB_PR:
        t_crnt_inp_src_info.e_type = DRV_CUSTOM_MISC_CRNT_INP_SRC_Y_PB_PR;
        break;
    case SCC_CRNT_INP_SRC_VGA:
        t_crnt_inp_src_info.e_type = DRV_CUSTOM_MISC_CRNT_INP_SRC_VGA;
        break;
    case SCC_CRNT_INP_SRC_SCART:
        t_crnt_inp_src_info.e_type = DRV_CUSTOM_MISC_CRNT_INP_SRC_SCART;
        break;
    case SCC_CRNT_INP_SRC_HDMI:
        t_crnt_inp_src_info.e_type = DRV_CUSTOM_MISC_CRNT_INP_SRC_HDMI;
        break;
    default:
        x_dbg_stmt("[SCC] Invalid input source type \r\n");
        return SMR_INV_ARG;
    }
    t_crnt_inp_src_info.ui1_port_num = ui1_port_num;

    x_memset(&t_op_info, 
             0, 
             sizeof(DRV_CUST_OPERATION_INFO_T));
    t_op_info.e_op_type = DRV_CUSTOM_OPERATION_TYPE_MISC_SET;
    t_op_info.u.t_misc_set_info.e_misc_type = DRV_CUSTOM_SET_CRNT_INP_SRC_INFO;
    t_op_info.u.t_misc_set_info.pv_set_info = (VOID*)&t_crnt_inp_src_info;
    t_op_info.u.t_misc_set_info.z_size = z_size_1;
    
    z_size_2 = sizeof(DRV_CUST_OPERATION_INFO_T); 
    i4_ret = x_rm_set_comp(
                DRVT_CUST_DRV,
                DRV_CUST_COMP_ID,
                FALSE,
                ANY_PORT_NUM,
                0,
                &t_op_info,
                z_size_2
                );
    if (i4_ret != RMR_OK)
    {
        x_dbg_stmt("[SCC] Set current input source info Fail! i4_ret=%d\r\n", i4_ret);
        return SMR_INV_SET_INFO;
    }
#else  /*async */   
    t_msg.e_msg_type = SM_SCC_MSG_TYPE_CRNT_INP_SRC_SET;
    t_msg.u.t_crnt_inp_src.e_inp_src_type = e_inp_src_type;
    t_msg.u.t_crnt_inp_src.ui1_port_num = ui1_port_num;

    i4_ret = _scc_send_msg( &t_msg );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    } 
#endif
    
    return SMR_OK;
}


