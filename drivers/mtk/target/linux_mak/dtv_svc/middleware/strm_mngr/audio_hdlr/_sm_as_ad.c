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
 * $RCSfile: _sm_as_ad.c,v $
 * $Revision: #2 $
 * $Date: 2012/11/03 $
 * $Author: xingju.wang $
 * $CCRevision: /main/DTV_X_HQ_int/5 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: 56f622a9797b74510039f128bf7c7e2b $
 *
 * Description: 
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "res_mngr/drv/x_aud_dec.h"
#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/drv/x_avc.h"
#include "scdb/x_scdb_api.h"

#include "strm_mngr/x_sm.h"
#include "strm_mngr/sm_drv_comp.h"
#include "strm_mngr/sm_drv_map.h"
#include "strm_mngr/_sm_utils.h"
#include "strm_mngr/sess_gnrc_hdlr/x_sm_sess_gnrc.h"
#include "strm_mngr/audio_hdlr/_sm_as_ad.h"
#include "strm_mngr/scc/x_scc.h"
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/
static SELECT_T _sm_as_ad_comp_sel_cb( 
    DRV_TYPE_T   e_type,
    UINT16       ui2_id,
    VOID*        pv_comp_sel_data,
    const VOID*  pv_comp_data,
    SIZE_T       z_comp_data_len,
    UINT16       ui2_num_active,
    UINT16       ui2_num_wait,
    CTRL_TYPE_T  e_ctrl,
    BOOL         b_would_conflict );

static VOID _sm_as_ad_rm_nfy( 
    HANDLE_T   h_obj,
    UINT8      ui1_port,
    RM_COND_T  e_nfy_cond,
    VOID*      pv_tag,
    UINT32     ui4_data);

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: sm_as_aud_fmt_conv_scdb2drv
 *
 * Description: <function description>
 *
 * Inputs:  e_scdb_aud_enc      SCDB audio encoding.
 *          e_scdb_layer        SCDB MPEG audio layer.
 *
 * Outputs: pt_drv_aud_fmt      Reference to driver audio format definition.
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID sm_as_aud_fmt_conv_scdb2drv( 
    AUD_ENC_T           e_scdb_aud_enc,
    AUD_LAYER_T         e_scdb_layer,
    AUD_DEC_FMT_INFO_T* pt_drv_aud_fmt )
{
    if ( !pt_drv_aud_fmt )
    {
        return;
    }
    
    switch( e_scdb_aud_enc )
    {
    case AUD_ENC_MPEG_1:
    case AUD_ENC_MPEG_2:
        if ( e_scdb_layer == AUD_LAYER_3 )
        {
            pt_drv_aud_fmt->e_fmt = AUD_DEC_FMT_MP3;
        }
        else
        {
            pt_drv_aud_fmt->e_fmt = AUD_DEC_FMT_MPEG;
        }
        pt_drv_aud_fmt->pv_info = NULL;
        break;
    case AUD_ENC_AAC:
        pt_drv_aud_fmt->e_fmt = AUD_DEC_FMT_AAC;
        pt_drv_aud_fmt->pv_info = NULL;
        break;
    case AUD_ENC_AMR:
        pt_drv_aud_fmt->e_fmt = AUD_DEC_FMT_AMR;
        pt_drv_aud_fmt->pv_info = NULL;
        break;
    case AUD_ENC_AWB:
        pt_drv_aud_fmt->e_fmt = AUD_DEC_FMT_AWB;
        pt_drv_aud_fmt->pv_info = NULL;
        break;
#ifdef ENABLE_MULTIMEDIA        
    case AUD_ENC_WMA_V1:       
        pt_drv_aud_fmt->e_fmt = AUD_DEC_FMT_WMA;
        pt_drv_aud_fmt->pv_info = NULL;   
        break;
    case AUD_ENC_WMA_V2:       
        pt_drv_aud_fmt->e_fmt = AUD_DEC_FMT_WMA;
        pt_drv_aud_fmt->pv_info = NULL;   
        break;
    case AUD_ENC_WMA_V3:        
        pt_drv_aud_fmt->e_fmt = AUD_DEC_FMT_WMA;
        pt_drv_aud_fmt->pv_info = NULL;
        break;
    case AUD_ENC_WMA_PRO:       
        pt_drv_aud_fmt->e_fmt = AUD_DEC_FMT_WMAPRO;
        pt_drv_aud_fmt->pv_info = NULL;   
        break;
    case AUD_ENC_PCM:        
    case AUD_ENC_LPCM:      
        pt_drv_aud_fmt->e_fmt = AUD_DEC_FMT_PCM;
        pt_drv_aud_fmt->pv_info = NULL;
        break;
    case AUD_ENC_DTS:      
        pt_drv_aud_fmt->e_fmt = AUD_DEC_FMT_DTS;
        pt_drv_aud_fmt->pv_info = NULL;
        break;
    case AUD_ENC_COOK:      
        pt_drv_aud_fmt->e_fmt = AUD_DEC_FMT_COOK;
        pt_drv_aud_fmt->pv_info = NULL;
        break;
    case AUD_ENC_DRA:      
        pt_drv_aud_fmt->e_fmt = AUD_DEC_FMT_DRA;
        pt_drv_aud_fmt->pv_info = NULL;
        break;
    case AUD_ENC_VORBIS:      
        pt_drv_aud_fmt->e_fmt = AUD_DEC_FMT_VORBIS;
        pt_drv_aud_fmt->pv_info = NULL;
        break;
#endif
    case AUD_ENC_AC3:
    case AUD_ENC_UNKNOWN:
    default:
        pt_drv_aud_fmt->e_fmt = AUD_DEC_FMT_AC3;
        pt_drv_aud_fmt->pv_info = NULL;
        break;
    }
}

static CHAR* _sm_audio_hdlr_set_fmt_str(
    AUD_DEC_FMT_T   e_aud_format )
{
    switch( e_aud_format )
    {
    case AUD_DEC_FMT_UNKNOWN:
        return "AUD_DEC_FMT_UNKNOWN";

    case AUD_DEC_FMT_MPEG:
        return "AUD_DEC_FMT_MPEG";

    case AUD_DEC_FMT_AC3:
        return "AUD_DEC_FMT_AC3";

    case AUD_DEC_FMT_PCM:
        return "AUD_DEC_FMT_PCM";

    case AUD_DEC_FMT_MP3:
        return "AUD_DEC_FMT_MP3";

    case AUD_DEC_FMT_AAC:
        return "AUD_DEC_FMT_AAC";
        
    case AUD_DEC_FMT_DTS:
        return "AUD_DEC_FMT_DTS";

    case AUD_DEC_FMT_WMA:
        return "AUD_DEC_FMT_WMA";
        
    case AUD_DEC_FMT_RA:
        return "AUD_DEC_FMT_RA";
    
    case AUD_DEC_FMT_HDCD:
        return "AUD_DEC_FMT_HDCD";
     
    case AUD_DEC_FMT_MLP:
        return "AUD_DEC_FMT_MLP";
        
    case AUD_DEC_FMT_MTS:
        return "AUD_DEC_FMT_MTS";
        
    case AUD_DEC_FMT_EU_CANAL_PLUS:
        return "AUD_DEC_FMT_EU_CANAL_PLUS";
        
    case AUD_DEC_FMT_TV_SYS:
        return "AUD_DEC_FMT_TV_SYS";
        
    case AUD_DEC_FMT_FMRDO:
        return "AUD_DEC_FMT_FMRDO";
        
    case AUD_DEC_FMT_FMRDO_DET:
        return "AUD_DEC_FMT_FMRDO_DET";
        
    case AUD_DEC_FMT_SBCDEC:
        return "AUD_DEC_FMT_SBCDEC";
        
    case AUD_DEC_FMT_SBCENC:
        return "AUD_DEC_FMT_SBCENC";

    case AUD_DEC_FMT_DRA:
        return "AUD_DEC_FMT_DRA";
        
    case AUD_DEC_FMT_COOK:
        return "AUD_DEC_FMT_COOK";
        
    case AUD_DEC_FMT_G729DEC:
        return "AUD_DEC_FMT_G729DEC";        

    case AUD_DEC_FMT_VORBIS:
        return "AUD_DEC_FMT_VORBIS";    

    case AUD_DEC_FMT_WMAPRO:
        return "AUD_DEC_FMT_WMAPRO";    

    case AUD_DEC_FMT_HE_AAC:
        return "AUD_DEC_FMT_HE_AAC";    

    case AUD_DEC_FMT_HE_AAC_V2:
        return "AUD_DEC_FMT_HE_AAC_V2";  
        
    case AUD_DEC_FMT_AMR:
        return "AUD_DEC_FMT_AMR"; 
        
    case AUD_DEC_FMT_AWB:
        return "AUD_DEC_FMT_AWB"; 
        
    default:
        return "INVALID_VALUE";
    }
}

static VOID _sm_as_fmt_extract_frm_scdb_rec(SCDB_REC_T  t_scdb_rec,
                                                      AUD_ENC_T*   e_aud_enc,
                                                      AUD_LAYER_T* e_aud_layer)
{
    SCDB_REC_TYPE_T e_rec_type; 
    if ( !e_aud_enc || !e_aud_layer )
    {
        return;
    }
    e_rec_type = t_scdb_rec.e_rec_type;
    switch(e_rec_type)
    {
        case SCDB_REC_TYPE_AUDIO_MPEG:
            *e_aud_enc = t_scdb_rec.u.t_audio_mpeg.e_enc;
            *e_aud_layer = t_scdb_rec.u.t_audio_mpeg.e_layer;
            break;
        case SCDB_REC_TYPE_AUDIO_AVI:
            *e_aud_enc = t_scdb_rec.u.t_audio_avi.e_enc;
            *e_aud_layer = t_scdb_rec.u.t_audio_avi.e_layer;
            break;            
        case SCDB_REC_TYPE_AUDIO_ASF:
            *e_aud_enc = t_scdb_rec.u.t_audio_asf.e_enc;
            *e_aud_layer = t_scdb_rec.u.t_audio_asf.e_layer;
            break;
        case SCDB_REC_TYPE_AUDIO_MP4:
            *e_aud_enc = t_scdb_rec.u.t_audio_mp4.e_enc;
            *e_aud_layer = t_scdb_rec.u.t_audio_mp4.e_layer;
            break;     
        case SCDB_REC_TYPE_AUDIO_PS:
            *e_aud_enc = t_scdb_rec.u.t_audio_ps.e_enc;
            *e_aud_layer = t_scdb_rec.u.t_audio_ps.e_layer;
            break;
        case SCDB_REC_TYPE_AUDIO_MKV:
            *e_aud_enc = t_scdb_rec.u.t_audio_mkv.e_enc;
            *e_aud_layer = t_scdb_rec.u.t_audio_mkv.e_layer;
            break;
        case SCDB_REC_TYPE_AUDIO_MP3:
            *e_aud_enc = t_scdb_rec.u.t_audio_mp3.e_enc;
            *e_aud_layer = t_scdb_rec.u.t_audio_mp3.e_layer;
            break;
        case SCDB_REC_TYPE_AUDIO_ES:
            *e_aud_enc = t_scdb_rec.u.t_audio_es.e_enc;
            *e_aud_layer = t_scdb_rec.u.t_audio_es.e_layer;
            break;
        case SCDB_REC_TYPE_AUDIO_RM:
            *e_aud_enc = t_scdb_rec.u.t_audio_rm.e_enc;
            *e_aud_layer = t_scdb_rec.u.t_audio_rm.e_layer;
            break;   
        /*add for flv*/
        case SCDB_REC_TYPE_AUDIO_FLV:
            *e_aud_enc = t_scdb_rec.u.t_audio_flv.e_enc;
            *e_aud_layer = t_scdb_rec.u.t_audio_flv.e_layer;
            break;
	case SCDB_REC_TYPE_AUDIO_OGG:
	    *e_aud_enc = t_scdb_rec.u.t_audio_ogg.e_enc;
            *e_aud_layer = t_scdb_rec.u.t_audio_ogg.e_layer;
	    break;
        default:
            *e_aud_enc = t_scdb_rec.u.t_audio_mpeg.e_enc;
            *e_aud_layer = t_scdb_rec.u.t_audio_mpeg.e_layer;
            break;
    }
}

/*-----------------------------------------------------------------------------
 * Name: _sm_as_ad_comp_sel_cb
 *
 * Description: <function description>
 *
 * Inputs:  e_type           References.
 *          ui2_id           References.
 *          pv_comp_sel_data References.
 *          pv_comp_data     References.
 *          z_comp_data_len  References.
 *          b_in_use         References.
 *
 * Outputs: - 
 *
 * Returns: SEL_COMP_BREAK  Success.
 *
 ----------------------------------------------------------------------------*/
static SELECT_T _sm_as_ad_comp_sel_cb( 
    DRV_TYPE_T   e_type,
    UINT16       ui2_id,
    VOID*        pv_comp_sel_data,
    const VOID*  pv_comp_data,
    SIZE_T       z_comp_data_len,
    UINT16       ui2_num_active,
    UINT16       ui2_num_wait,
    CTRL_TYPE_T  e_ctrl,
    BOOL         b_would_conflict )
{
    SM_DRV_COMP_INFO_T     t_drv_descr = SM_DRV_COMP_INFO_INIT;
    
    t_drv_descr.e_type = e_type;
    t_drv_descr.ui2_comp_id = ui2_id;

    if ( sm_drv_map_exists( &t_drv_descr ) || b_would_conflict )
    {
        return SEL_NEXT;
    }
    return SEL_COMP_BREAK;
}

/*-----------------------------------------------------------------------------
 * Name: _sm_as_ad_rm_nfy
 *
 * Description: <function description>
 *
 * Inputs:  h_obj      References.
 *          ui1_port   References.
 *          e_nfy_cond References.
 *          pv_tag     References.
 *          ui4_data   References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_as_ad_rm_nfy( 
    HANDLE_T   h_obj,
    UINT8      ui1_port,
    RM_COND_T  e_nfy_cond,
    VOID*      pv_tag,
    UINT32     ui4_data )
{
    SM_AUDIO_HDLR_MSG_T         t_msg;
    HANDLE_T                    h_stream = (HANDLE_T)pv_tag;
    RM_COND_T                   e_cond = RM_COND_IGNORE;
    
    if ( !x_handle_valid( h_stream ) )
    {
        return;
    }
    
    /* Return before send message to queue, because not handle the OPERATION info in sm_slctr */
    e_cond = rm_cond_chg( e_nfy_cond, 
                          RM_COND_IGNORE, 
                          COND_GRP_OPERATION );
    if( e_cond != RM_COND_NO_CHG )
    {   
        return;
    }
    t_msg.e_msg_type = SM_AUDIO_HDLR_MSG_RM_NFY;
    t_msg.h_stream = h_stream;
    
    t_msg.u.t_msg_rm.h_ad = h_obj;
    t_msg.u.t_msg_rm.ui1_port = ui1_port;
    t_msg.u.t_msg_rm.e_ad_cond = e_nfy_cond;
    t_msg.u.t_msg_rm.ui4_data = ui4_data;
    t_msg.u.t_msg_rm.b_should_update_comp_descr = TRUE;

    sm_as_send_msg( &t_msg);

}


/*-----------------------------------------------------------------------------
 * Name: _sm_as_ad_ctrl_nfy_fct
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
static VOID _sm_as_ad_ctrl_nfy_fct( 
    VOID*           pv_nfy_tag,
    AUD_DEC_COND_T  e_nfy_cond,
    UINT32          ui4_data_1, 
    UINT32          ui4_data_2 )
{
    HANDLE_T                     h_stream = (HANDLE_T)pv_nfy_tag;
    SM_AUDIO_HDLR_MSG_T          t_msg;
    
    if ( !x_handle_valid( h_stream ) )
    {
        return;
    }    
    
    t_msg.e_msg_type = SM_AUDIO_HDLR_MSG_AD_NFY;
    t_msg.h_stream = h_stream;
    
    t_msg.u.t_msg_ad.e_ad_nfy_cond = e_nfy_cond;
    t_msg.u.t_msg_ad.ui4_data_1 = ui4_data_1;
    t_msg.u.t_msg_ad.ui4_data_2 = ui4_data_2;
    
    if ( AUD_DEC_COND_AUD_INFO_CHG == e_nfy_cond )
    {
        SM_ASSERT( ui4_data_1 != 0 );
        t_msg.u.t_msg_ad.u.t_aud_info = *((AUD_DEC_AUD_INFO_T*)ui4_data_1);
    }
    
    if ( e_nfy_cond != AUD_DEC_COND_UPLOAD_INFO_CHG )
    {
        sm_as_send_msg( &t_msg );
    }
    
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: sm_as_open_ad
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_as_obj References.
 *
 * Outputs: - 
 *
 * Returns: SMR_INTERNAL_ERROR  Success.
 *          SMR_OK              Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_as_open_ad( HANDLE_T            h_stream,
                             SM_AUDIO_STREAM_T*  pt_as_obj )
{
    INT32                  i4_ret;
        
    if ( pt_as_obj->t_ad_descr.h_comp != NULL_HANDLE )
    {
        return SMR_INTERNAL_ERROR;   
    }
    /* open the component */
    sm_drv_map_lock();

    TMS_BEGIN( AS_HANDLE_AD_OPENED );
    i4_ret = sm_drv_comp_open( pt_as_obj->t_base.h_source,
                               DRVT_AUD_DEC,
                               pt_as_obj->t_ad_descr.t_info.ui2_comp_id,
                               pt_as_obj->t_ad_descr.t_info.s_logical_name,
                               NULL,   /* pv_sel_data */
                               &_sm_as_ad_comp_sel_cb,
                               (VOID*)h_stream,
                               &_sm_as_ad_rm_nfy,
                               &pt_as_obj->t_ad_descr );
    
    if ( i4_ret != SMR_OK )
    {
        sm_drv_map_unlock();
        return i4_ret;
    }
    
    sm_drv_map_unlock();
    
    /* Send RM notify message */
    {
        SM_AUDIO_HDLR_MSG_T  t_msg;
        
        t_msg.e_msg_type = SM_AUDIO_HDLR_MSG_RM_NFY;
        t_msg.h_stream = h_stream;
        
        t_msg.u.t_msg_rm.h_ad = pt_as_obj->t_ad_descr.h_comp;
        t_msg.u.t_msg_rm.ui1_port = pt_as_obj->t_ad_descr.t_info.ui1_in_port;
        t_msg.u.t_msg_rm.e_ad_cond = pt_as_obj->t_ad_descr.e_comp_cond;
        t_msg.u.t_msg_rm.ui4_data = 0;
        t_msg.u.t_msg_rm.b_should_update_comp_descr = FALSE;
        
        sm_as_send_msg( &t_msg );
    }
    
    return SMR_OK;    
}

/*-----------------------------------------------------------------------------
 * Name: sm_as_close_ad
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_as_obj References.
 *
 * Outputs: - 
 *
 * Returns: 
 *
 ----------------------------------------------------------------------------*/
INT32 sm_as_close_ad( 
    HANDLE_T            h_stream,
    SM_AUDIO_STREAM_T*  pt_as_obj )
{
    INT32 i4_ret;
    
    TMS_BEGIN( AS_HANDLE_AD_CLOSED );
    /* lock driver map, the sm_drv_comp_close() and delete from driver map 
       must be atomic. */
    sm_drv_map_lock();

    i4_ret = sm_drv_comp_close( &pt_as_obj->t_ad_descr );
    if ( i4_ret != SMR_OK )
    {
    }
    
    sm_drv_map_unlock();


    /* Send RM notify message */
    {
        SM_AUDIO_HDLR_MSG_T  t_msg;
        
        t_msg.e_msg_type = SM_AUDIO_HDLR_MSG_RM_NFY;
        t_msg.h_stream = h_stream;
        
        t_msg.u.t_msg_rm.h_ad = pt_as_obj->t_ad_descr.h_comp;
        t_msg.u.t_msg_rm.ui1_port = pt_as_obj->t_ad_descr.t_info.ui1_in_port;
        t_msg.u.t_msg_rm.e_ad_cond = pt_as_obj->t_ad_descr.e_comp_cond;
        t_msg.u.t_msg_rm.ui4_data = 0;
        t_msg.u.t_msg_rm.b_should_update_comp_descr = FALSE;
        
        sm_as_send_msg( &t_msg );
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_as_ad_connect
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_as_obj References.
 *
 * Outputs: - 
 *
 * Returns: 
 *
 ----------------------------------------------------------------------------*/
INT32 sm_as_ad_connect( 
    HANDLE_T            h_stream,
    SM_AUDIO_STREAM_T*  pt_as_obj )
{
    INT32               i4_ret;
    
#ifndef RM_AUTO_CONNECT
    HANDLE_T  h_dmx = NULL_HANDLE;
#endif /* RM_AUTO_CONNECT */

    if ( !IS_MULTIMEDIA_AUD_STRM(pt_as_obj))
    {
        i4_ret = rm_set( pt_as_obj->t_ad_descr.h_comp,
                         FALSE,
                         ANY_PORT_NUM,
                         AUD_DEC_SET_TYPE_PCR_ID,
                         (VOID*)((UINT32)pt_as_obj->t_base.ui2_pcr_id),
                         sizeof(UINT16),
                         &pt_as_obj->t_ad_descr.e_comp_cond);
        if ( i4_ret != RMR_OK )
        {
            return SMR_DRV_COMP_ERROR;
        }
    }

#ifndef RM_AUTO_CONNECT
    /*************************************************************************
        !!!CAUTION!!! connection all drivers individually,
        Better use auto connect feature of RM.
    *************************************************************************/
    if ( IS_MPEG_AUD_STRM(pt_as_obj) ||
         IS_MULTIMEDIA_AUD_STRM(pt_as_obj))
    {
        i4_ret = pm_get_component_info( pt_as_obj->t_base.h_pm_comp,
                                        &h_dmx );
        if ( i4_ret != PMR_OK )
        {
            return SMR_DRV_COMP_ERROR;
        }
        
        TMS_BEGIN( AS_HANDLE_AD_CONNECTED );
        i4_ret = sm_drv_comp_connect( &pt_as_obj->t_ad_descr,
                                      h_dmx,
                                      (VOID*)h_stream );
        if ( i4_ret != SMR_OK )
        {
            return SMR_DRV_COMP_ERROR;
        }
    }
    else if ( IS_DIGITAL_AUD_STRM(pt_as_obj) )
    {
        if ( pt_as_obj->e_feed_mod == FEED_MODE_PULL )
        {
            i4_ret = rm_set( pt_as_obj->t_ad_descr.h_comp,
                             FALSE,
                             ANY_PORT_NUM,
                             AUD_DEC_SET_TYPE_INP_MODE,
                             (VOID*)((UINT32)TRUE),
                             sizeof(BOOL),
                             &pt_as_obj->t_ad_descr.e_comp_cond);
            if ( i4_ret != RMR_OK )
            {
                return SMR_DRV_COMP_ERROR;
            }
        }
        
        TMS_BEGIN( AS_HANDLE_AD_CONNECTED );
        i4_ret = sm_drv_comp_connect( &pt_as_obj->t_ad_descr,
                                      pt_as_obj->t_base.h_source,
                                      (VOID*)h_stream );
        if ( i4_ret != SMR_OK )
        {
            return SMR_DRV_COMP_ERROR;
        }
    }
    else /* Analog Audio Stream, directly connect to source component */
    {
        DRV_COMP_ID_T   t_comp_id;
        UINT16          ui2_num;

        TMS_BEGIN( AS_HANDLE_AD_CONNECTED );
        i4_ret = rm_get_pipe_num_comps_of_type(
                                    pt_as_obj->t_base.h_source,
                                    DRVT_AVC_SCART,
                                    &ui2_num);
        if ((i4_ret == RMR_OK) && (ui2_num != 0))
        {
            UINT8   ui1_inp;
            UINT8   ui1_out;
            UINT16  ui2_idx;
            UINT32  ui4_flags;

            /* has SCART in connection, need to connect to SCART */
            for (ui2_idx = 0; ui2_idx < ui2_num; ui2_idx++)
            {
                i4_ret = rm_get_pipe_comp_info(
                                        pt_as_obj->t_base.h_source,
                                        DRVT_AVC_SCART,
                                        ui2_idx,
                                        &t_comp_id.ui2_id,
                                        &ui1_inp,
                                        &ui1_out,
                                        &ui4_flags);
                if (i4_ret != RMR_OK)
                {
                    return SMR_DRV_COMP_ERROR;
                }

                if ((ui4_flags & AVC_FLAG_SCART_SOURCE) == AVC_FLAG_SCART_SOURCE)
                {
                    break;
                }
            }

            if (ui2_idx == ui2_num)
            {
                /* No SCART source, skip SCART and connect to 
                   source component directly */
                i4_ret = sm_drv_comp_connect( &pt_as_obj->t_ad_descr,
                                              pt_as_obj->t_base.h_source,
                                              (VOID*)h_stream );
                if ( i4_ret != SMR_OK )
                {
                    return SMR_DRV_COMP_ERROR;
                }
            }
            else
            {
                /* SCART */
                t_comp_id.b_sel_out_port = TRUE;
                t_comp_id.e_type = DRVT_AVC_SCART;
                t_comp_id.pv_tag = NULL;
                t_comp_id.u.ui1_out_port = 0;

                pt_as_obj->t_ad_descr.e_comp_cond_org = pt_as_obj->t_ad_descr.e_comp_cond;

                i4_ret = rm_connect( pt_as_obj->t_ad_descr.h_comp,
                                     RM_CONN_TYPE_COMP_ID,
                                     (VOID*)&t_comp_id,
                                     sizeof (DRV_COMP_ID_T),
                                     (VOID*)h_stream,
                                     &pt_as_obj->t_ad_descr.e_comp_cond );
                if ( i4_ret != SMR_OK )
                {
                    return SMR_DRV_COMP_ERROR;
                }
            }
        }
        else
        {
            /* direct connect to source component */
            i4_ret = sm_drv_comp_connect( &pt_as_obj->t_ad_descr,
                                          pt_as_obj->t_base.h_source,
                                          (VOID*)h_stream );
            if ( i4_ret != SMR_OK )
            {
                return SMR_DRV_COMP_ERROR;
            }
        }
    }
#else /* RM_AUTO_CONNECT */
    i4_ret = sm_drv_comp_connect( &pt_as_obj->t_ad_descr,
                                  pt_as_obj->t_base.h_source,
                                  (VOID*)h_stream );
    if ( i4_ret != SMR_OK )
    {
        return SMR_DRV_COMP_ERROR;
    }
#endif /* RM_AUTO_CONNECT */
    SM_AS_DBG_INFO(( "[SM] Audio Decoder CONNECT\r\n" ));
    
    /* Send RM notify message */
    {
        SM_AUDIO_HDLR_MSG_T  t_msg;
        
        t_msg.e_msg_type = SM_AUDIO_HDLR_MSG_RM_NFY;
        t_msg.h_stream = h_stream;
        
        t_msg.u.t_msg_rm.h_ad = pt_as_obj->t_ad_descr.h_comp;
        t_msg.u.t_msg_rm.ui1_port = pt_as_obj->t_ad_descr.t_info.ui1_in_port;
        t_msg.u.t_msg_rm.e_ad_cond = pt_as_obj->t_ad_descr.e_comp_cond;
        t_msg.u.t_msg_rm.ui4_data = 0;
        t_msg.u.t_msg_rm.b_should_update_comp_descr = FALSE;
        
        sm_as_send_msg( &t_msg );
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_as_ad_disconnect
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_as_obj References.
 *
 * Outputs: - 
 *
 * Returns: SMR_INV_HANDLE      Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *          SMR_OK              Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_as_ad_disconnect( 
    HANDLE_T            h_stream,
    SM_AUDIO_STREAM_T*  pt_as_obj )
{
    INT32 i4_ret;

    /*if MW set downmix ,reset it when disconnect audio decoder*/
	if( pt_as_obj->b_downmix_recover)
    {
		i4_ret = rm_set( pt_as_obj->t_ad_descr.h_comp,
							FALSE,
							ANY_PORT_NUM,
							 AUD_DEC_SET_TYPE_DOWNMIX,
							(VOID*)(UINT32)pt_as_obj->e_downmix_mode,
							sizeof(AUD_DEC_DOWNMIX_MODE_T),
							&pt_as_obj->t_ad_descr.e_comp_cond);
		if ( i4_ret != RMR_OK && i4_ret != RMR_ASYNC_NFY )
		{
			   SM_AS_DBG_ERR(( "[SM] Set Audio Decoder AUD_DEC_SET_TYPE_DOWNMIX error %d\r\n",
				   i4_ret ));
		}
    }
    
    
    TMS_BEGIN( AS_HANDLE_AD_DISCONNECTED );
    i4_ret = sm_drv_comp_disconnect( &pt_as_obj->t_ad_descr );
    if ( i4_ret != SMR_OK && i4_ret != SMR_ASYNC_NFY )
    {
        return i4_ret;
    }
    SM_AS_DBG_INFO(( "[SM] Audio Decoder DISCONNECT\r\n" ));

    /* Send RM notify message */
    if ( i4_ret != SMR_ASYNC_NFY)
    {
        SM_AUDIO_HDLR_MSG_T  t_msg;
        
        t_msg.e_msg_type = SM_AUDIO_HDLR_MSG_RM_NFY;
        t_msg.h_stream = h_stream;
        
        t_msg.u.t_msg_rm.h_ad = pt_as_obj->t_ad_descr.h_comp;
        t_msg.u.t_msg_rm.ui1_port = pt_as_obj->t_ad_descr.t_info.ui1_in_port;
        t_msg.u.t_msg_rm.e_ad_cond = pt_as_obj->t_ad_descr.e_comp_cond;
        t_msg.u.t_msg_rm.ui4_data = 0;
        t_msg.u.t_msg_rm.b_should_update_comp_descr = FALSE;
        
        sm_as_send_msg( &t_msg );
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_as_ad_set_encoding_type
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_as_obj References.
 *
 * Outputs: - 
 *
 * Returns: SMR_INV_HANDLE      Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *          SMR_OK              Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_as_ad_set_encoding_type( 
    SM_AUDIO_STREAM_T*  pt_as_obj )
{
    INT32                 i4_ret;
    AUD_DEC_FMT_INFO_T    t_aud_fmt = { AUD_DEC_FMT_AC3, NULL };
    AUD_DEC_TV_AUD_SYS_T  t_tv_aud_sys;
    AUD_ENC_T             t_enc;  
    AUD_LAYER_T           t_layer;  

    if ( !pt_as_obj )
    {
        return SMR_INV_ARG;
    }
    _sm_as_fmt_extract_frm_scdb_rec(
           pt_as_obj->t_base.t_scdb_rec,
           &t_enc,
           &t_layer);

    pt_as_obj->t_enc = t_enc;
    pt_as_obj->t_layer = t_layer;

    /* Set audio format according to SCDB record */
    if ( IS_MPEG_AUD_STRM(pt_as_obj) ||
         IS_MULTIMEDIA_AUD_STRM(pt_as_obj))
    {
        sm_as_aud_fmt_conv_scdb2drv( 
            t_enc,
            t_layer,
            &t_aud_fmt );
    }
    else if ( IS_DIGITAL_AUD_STRM(pt_as_obj) )
    {
        sm_as_aud_fmt_conv_scdb2drv( 
            pt_as_obj->t_base.t_scdb_rec.u.t_audio_digital.e_enc,
            pt_as_obj->t_base.t_scdb_rec.u.t_audio_digital.e_layer,
            &t_aud_fmt );
    }
    else /* is analog audio stream */
    {
        if ( pt_as_obj->t_base.t_scdb_rec.u.t_audio_analog.e_enc != AUD_ENC_PCM )
        {
            SM_AS_DBG_ERR(( "[SM] ERROR! Analog Audio stream encoding is %u not PCM\r\n", 
                             (UINT32)pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.e_enc ));
        }

        if (pt_as_obj->t_base.t_scdb_rec.u.t_audio_analog.e_enc == AUD_ENC_TV_SYS)
        {
            t_tv_aud_sys.ui4_aud_sys_mask =
                pt_as_obj->t_base.t_scdb_rec.u.t_audio_analog.ui4_audio_sys;
            t_tv_aud_sys.ui4_tv_sys_mask  =
                pt_as_obj->t_base.t_scdb_rec.u.t_audio_analog.ui4_tv_sys;

            i4_ret = rm_set(pt_as_obj->t_ad_descr.h_comp,
                            FALSE,
                            ANY_PORT_NUM,
                            AUD_DEC_SET_TYPE_TV_AUD_SYS_MASK,
                            (VOID*)&t_tv_aud_sys,
                            sizeof(AUD_DEC_TV_AUD_SYS_T),
                            NULL);
            if ( i4_ret != RMR_OK )
            {
                return SMR_DRV_COMP_ERROR;
            }
    
            t_aud_fmt.e_fmt = AUD_DEC_FMT_TV_SYS;
        }
        else if (pt_as_obj->t_base.t_scdb_rec.u.t_audio_analog.e_enc == AUD_ENC_EU_CANAL_PLUS)
        {
            t_aud_fmt.e_fmt = AUD_DEC_FMT_EU_CANAL_PLUS;
        }
        else if (pt_as_obj->t_base.t_scdb_rec.u.t_audio_analog.e_enc == AUD_ENC_FM_RADIO)
        {
            t_aud_fmt.e_fmt = AUD_DEC_FMT_FMRDO;
        }
        else
        {
            t_aud_fmt.e_fmt = AUD_DEC_FMT_PCM;
        }
    }

    x_dbg_stmt("[SM] AS(0x%.8X), set audio format (%s)\r\n", 
                 pt_as_obj->t_base.h_stream, _sm_audio_hdlr_set_fmt_str((AUD_DEC_FMT_T)t_aud_fmt.e_fmt));
    
    i4_ret = rm_set( pt_as_obj->t_ad_descr.h_comp,
                     FALSE,
                     ANY_PORT_NUM,
                     AUD_DEC_SET_TYPE_FMT,
                     (VOID*)&t_aud_fmt,
                     sizeof(AUD_DEC_FMT_INFO_T),
                     &pt_as_obj->t_ad_descr.e_comp_cond);
    if ( i4_ret != RMR_OK )
    {
        x_dbg_stmt("[SM] AS(0x%.8X), set audio format type(%s) error!!! return (%d)\r\n",
                     pt_as_obj->t_base.h_stream, _sm_audio_hdlr_set_fmt_str((AUD_DEC_FMT_T)t_aud_fmt.e_fmt), i4_ret );
        return SMR_DRV_COMP_ERROR;
    }
    
    if ( IS_MPEG_AUD_STRM(pt_as_obj) )
    {
        SM_AS_DBG_INFO(( "[SM] Set Audio Decoder encoding to %u\r\n", 
                         (UINT32)pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.e_enc ));
    }
    else if ( IS_DIGITAL_AUD_STRM(pt_as_obj) )
    {
        SM_AS_DBG_INFO(( "[SM] Set Audio Decoder encoding to %u\r\n", 
                         (UINT32)pt_as_obj->t_base.t_scdb_rec.u.t_audio_digital.e_enc ));
    }
    else
    {
        SM_AS_DBG_INFO(( "[SM] Set Audio Decoder encoding to %u\r\n", 
                         (UINT32)pt_as_obj->t_base.t_scdb_rec.u.t_audio_analog.e_enc ));
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_as_ad_play_drvs
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_as_obj References.
 *
 * Outputs: - 
 *
 * Returns: SMR_INV_HANDLE      Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *          SMR_OK              Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_as_ad_play_drvs( 
    HANDLE_T            h_stream,
    SM_AUDIO_STREAM_T*  pt_as_obj,
    INT32               i4_speed )
{
    INT32                 i4_ret;
    SM_SESSION_T*         pt_sess_obj = NULL;
    BOOL b_is_sub = FALSE;
    HANDLE_T            h_scc_aud_comp;
    SM_COMMAND_T    at_cmd[3];  
    SCC_AUD_PL2_SWITCH_T e_value;
    
    pt_sess_obj = (SM_SESSION_T*)sm_get_sess_obj( pt_as_obj->t_base.h_sess );
    if ( !pt_sess_obj )
    {
        return SMR_INV_HANDLE;
    }
    
    if ( pt_as_obj->ui4_lastest_ad_event == SM_EVN_AS_AD_PLAYED )
    {
        return SMR_OK;
    }
    
    /* Set audio format according to SCDB record */
    i4_ret = sm_as_ad_set_encoding_type( pt_as_obj );
    if ( i4_ret != SMR_OK )
    {
        sm_as_send_msg_do_event(
               h_stream,
               SM_EVN_AS_CODEC_NOT_SUPPORT,
               (UINT32)0 );
        return SMR_DRV_COMP_ERROR;
    }
    /* command for opening an audio component */
    at_cmd[0].e_code     = SCC_CMD_CODE_GRP_TYPE;
    at_cmd[0].u.ui4_data = SCC_CMD_TYPE_AUDIO;
    at_cmd[1].e_code     = SCC_CMD_CODE_NAME;
    at_cmd[1].u.ps_text  = SN_PRES_MAIN_DISPLAY;
    at_cmd[2].e_code     = SM_CMD_CODE_END;

    /* open the audio component */    
    i4_ret = x_scc_comp_open(at_cmd,&h_scc_aud_comp);
    if(i4_ret != SMR_OK)
    {
        SM_AS_DBG_ERR(( "[SM] AS : openl scc component failed %d\r\n",
                            i4_ret ));
     }

    i4_ret = x_scc_aud_get_pl2_switch(h_scc_aud_comp,&e_value);
    if(i4_ret != SMR_OK)
    {
        SM_AS_DBG_ERR(( "[SM] AS : get pl2 switch failed %d\r\n",
                            i4_ret ));
    }
	
    i4_ret = x_scc_aud_get_downmix_mode(h_scc_aud_comp,&pt_as_obj->e_downmix_mode);
    if(i4_ret != SMR_OK)
    {
        SM_AS_DBG_ERR(( "[SM] AS : get downmix mode failed %d\r\n",
                            i4_ret ));
    }
	
    i4_ret = x_scc_comp_close(h_scc_aud_comp);
    if(i4_ret != SMR_OK)
    {
        SM_AS_DBG_ERR(( "[SM] AS :  close scc component failed %d\r\n",
                             i4_ret ));
    }
    /*Set Dual mono setting according to SCDB record*/
    if ( IS_MPEG_AUD_STRM(pt_as_obj)
         ||(IS_TS_AUD_STRM( pt_as_obj )&&IS_MULTIMEDIA_AUD_STRM( pt_as_obj ))
        
    )
    {
        AUD_DEC_DDC_INFO_T     t_drv_ddc_info;
        x_memset(&t_drv_ddc_info, 0, sizeof(AUD_DEC_DDC_INFO_T));
		
        AUD_DEC_DOWNMIX_MODE_T e_drv_aud_dec_dm_mode = AUD_DEC_DM_OFF;  
        if( e_value == SCC_AUD_PL2_SWITCH_ON)
        {
            e_drv_aud_dec_dm_mode = AUD_DEC_DM_OFF;
			pt_as_obj->b_downmix_recover = TRUE;
        }
        else
        {
            if(pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.e_aud_fmt == AUD_FMT_DUAL_MONO)
            {
                if(pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.ui1_index == AUDIO_INDEX_DUAL_0)
                {
                     /*Set Dual1 Mode*/ 
                     e_drv_aud_dec_dm_mode = AUD_DEC_DM_DUAL1;
                }
                else if(pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.ui1_index == AUDIO_INDEX_DUAL_1)
                {
                      /*Set Dual2 Mode*/ 
                     e_drv_aud_dec_dm_mode =  AUD_DEC_DM_DUAL2;
                }
                else
                {            
                    /*stereo is selected by default for new feature with CR[DTV00135123]*/
                    e_drv_aud_dec_dm_mode = AUD_DEC_DM_STEREO/*AUD_DEC_DM_DUAL1*/;  
                }
				pt_as_obj->b_downmix_recover = TRUE;
            }
			else 
			{
	         	if(pt_as_obj->e_force_downmix_mode != AUD_DEC_DM_NULL )
                {
                    e_drv_aud_dec_dm_mode = pt_as_obj->e_force_downmix_mode; 
					i4_ret = rm_set( pt_as_obj->t_ad_descr.h_comp,
	                            FALSE,
	                            ANY_PORT_NUM,
	                             AUD_DEC_SET_TYPE_DOWNMIX,
	                            (VOID*)(UINT32)e_drv_aud_dec_dm_mode,
	                            sizeof(AUD_DEC_DOWNMIX_MODE_T),
	                            &pt_as_obj->t_ad_descr.e_comp_cond);
	        		if ( i4_ret != RMR_OK && i4_ret != RMR_ASYNC_NFY )
	        		{
	               		SM_AS_DBG_ERR(( "[SM] Set Audio Decoder AUD_DEC_SET_TYPE_DOWNMIX error %d\r\n",
	                   	i4_ret ));
	        		}
					
                }
            else if((pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.e_enc == AUD_ENC_AC3) ||
                   (pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.e_enc == AUD_ENC_E_AC3))
            {   
                e_drv_aud_dec_dm_mode = AUD_DEC_DM_LT_RT;  
				pt_as_obj->b_downmix_recover = TRUE;
            }
			}


			/*set b_downmix_recover= true*/
			
        }

        if(pt_as_obj->b_downmix_recover)
        {
	        i4_ret = rm_set( pt_as_obj->t_ad_descr.h_comp,
	                            FALSE,
	                            ANY_PORT_NUM,
	                             AUD_DEC_SET_TYPE_DOWNMIX,
	                            (VOID*)(UINT32)e_drv_aud_dec_dm_mode,
	                            sizeof(AUD_DEC_DOWNMIX_MODE_T),
	                            &pt_as_obj->t_ad_descr.e_comp_cond);
	        if ( i4_ret != RMR_OK && i4_ret != RMR_ASYNC_NFY )
	        {
	               SM_AS_DBG_ERR(( "[SM] Set Audio Decoder AUD_DEC_SET_TYPE_DOWNMIX error %d\r\n",
	                   i4_ret ));
	        }
        }
		
        /*set multi_program stream info to driver*/
        if(pt_as_obj->b_ms10_mix_flag)
        {
            
            t_drv_ddc_info.eIndex    = AUD_DEC_DDC_ALL;
            t_drv_ddc_info.eControl  = AUD_DEC_DDC_SINGLE_IN_DUAL_DEC;
            t_drv_ddc_info.eMixerMode= AUD_DEC_DDC_MIX_MIX; /*sm default set mix_mix,if need,sm can implement SCC to AP or mw module*/
            t_drv_ddc_info.eAssocId  = (AUD_DEC_DDC_ID_T)(pt_as_obj->ui1_ms10_mix_id);
        }
        else /* set normal type stream control mode to driver*/
        {
            t_drv_ddc_info.eIndex    = AUD_DEC_DDC_CONTROL;
            t_drv_ddc_info.eControl  = AUD_DEC_DDC_SINGLE_IN_SINGLE_DEC;
            
        }
        i4_ret = rm_set( pt_as_obj->t_ad_descr.h_comp,
                            FALSE,
                            ANY_PORT_NUM,
                            AUD_DEC_SET_TYPE_DDC_MODE,
                            (VOID*)&t_drv_ddc_info,
                            sizeof(AUD_DEC_DDC_INFO_T),
                            &pt_as_obj->t_ad_descr.e_comp_cond);
        if ( i4_ret != RMR_OK && i4_ret != RMR_ASYNC_NFY )
        {
            SM_AS_DBG_ERR(( "[SM] Set Audio Decoder AUD_DEC_SET_TYPE_DDC_MODE error %d\r\n",
                   i4_ret ));
        }

      }

    if( !( IS_MULTIMEDIA_AUD_STRM(pt_as_obj)))
    {
        i4_ret = rm_set( pt_as_obj->t_ad_descr.h_comp,
                         FALSE,
                         ANY_PORT_NUM,
                         AUD_DEC_SET_TYPE_AV_SYNC,
                         (VOID*)pt_as_obj->e_av_sync,
                         sizeof(AUD_DEC_AV_SYNC_INFO_T),
                         &pt_as_obj->t_ad_descr.e_comp_cond);
        if ( i4_ret != RMR_OK && i4_ret != RMR_ASYNC_NFY )
        {
            SM_AS_DBG_ERR(( "[SM] Set Audio Decoder AUD_DEC_SET_TYPE_AV_SYNC error %d\r\n",
                i4_ret ));
            /* Ignore this error
            return SMR_DRV_COMP_ERROR;
            */
        }
    }   
    
    if ((x_strcmp(pt_sess_obj->s_dest_grp_name, SN_PRES_MAIN_DISPLAY) == 0 ) &&
        (! (x_strcmp(pt_as_obj->t_ad_descr .t_info.s_logical_name, SN_PRES_AUX_SOUND) == 0 )))
    {
        i4_ret = rm_set( pt_as_obj->t_ad_descr.h_comp,
                         FALSE,
                         ANY_PORT_NUM,
                         AUD_DEC_SET_TYPE_AD_MODE,
                         (VOID*)(UINT32)pt_as_obj->b_ad_mode,
                         sizeof(BOOL),
                         &pt_as_obj->t_ad_descr.e_comp_cond);
        if ( i4_ret != RMR_OK && i4_ret != RMR_ASYNC_NFY )
        {
            SM_AS_DBG_ERR(( "[SM] Set Audio Decoder AUD_DEC_SET_TYPE_AD_MODE error, return %d\r\n",
                i4_ret ));
        }
    }
     
    if (x_strcmp(pt_sess_obj->s_dest_grp_name, SN_PRES_SUB_DISPLAY) == 0 )
    {
        b_is_sub = TRUE;
    }
    i4_ret = rm_set( pt_as_obj->t_ad_descr.h_comp,
                             FALSE,
                             ANY_PORT_NUM,
                             AUD_DEC_SET_TYPE_PIPVIDEO_PATH_INFO,
                             (VOID*)(UINT32)b_is_sub,
                             sizeof(BOOL),
                             &pt_as_obj->t_ad_descr.e_comp_cond);
    if ( i4_ret != RMR_OK && i4_ret != RMR_ASYNC_NFY )
    {
        SM_AS_DBG_ERR(( "[SM] Set Audio Decoder AUD_DEC_SET_TYPE_PIPVIDEO_PATH_INFO error %d\r\n",
            i4_ret ));
    }
    pt_as_obj->e_ad_ctrl_type = ( (i4_speed == 0) ?
                                  AUD_DEC_CTRL_PAUSE :
                                  AUD_DEC_CTRL_PLAY );
    
    TMS_BEGIN( AS_HANDLE_AD_STARTED );
    i4_ret = rm_set( pt_as_obj->t_ad_descr.h_comp,
                     FALSE,
                     ANY_PORT_NUM,
                     AUD_DEC_SET_TYPE_CTRL,
                     (VOID*)pt_as_obj->e_ad_ctrl_type,
                     sizeof(AUD_DEC_CTRL_T),
                     &pt_as_obj->t_ad_descr.e_comp_cond);
    if ( i4_ret != RMR_OK && i4_ret != RMR_ASYNC_NFY )
    {
        return SMR_DRV_COMP_ERROR;    
    }
    SM_AS_DBG_INFO(( "[SM] Set Audio Decoder AUD_DEC_CTRL_PLAY\r\n" ));
    
    pt_as_obj->t_base.i4_speed = i4_speed;
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_as_ad_chg_speed
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_as_obj References.
 *
 * Outputs: - 
 *
 * Returns: SMR_INV_HANDLE      Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *          SMR_OK              Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_as_ad_chg_speed( 
    HANDLE_T            h_stream,
    SM_AUDIO_STREAM_T*  pt_as_obj,
    INT32               i4_speed )
{
    INT32                 i4_ret;
    SM_SESSION_T*         pt_sess_obj = NULL;
    
    pt_sess_obj = (SM_SESSION_T*)sm_get_sess_obj( pt_as_obj->t_base.h_sess );
    if ( !pt_sess_obj )
    {
        return SMR_INV_HANDLE;
    }
    
    if ( i4_speed == 0 && pt_as_obj->t_base.i4_speed != 0 )
    {
        i4_ret = rm_set( pt_as_obj->t_ad_descr.h_comp,
                         FALSE,
                         ANY_PORT_NUM,
                         AUD_DEC_SET_TYPE_CTRL,
                         (VOID*)AUD_DEC_CTRL_PAUSE,
                         sizeof(AUD_DEC_CTRL_T),
                         &pt_as_obj->t_ad_descr.e_comp_cond);
        if ( i4_ret != RMR_OK && i4_ret != RMR_ASYNC_NFY )
        {
            return SMR_DRV_COMP_ERROR;    
        }
        SM_AS_DBG_INFO(( "[SM] Set Audio Decoder AUD_DEC_CTRL_PAUSE\r\n" ));
    }
    else if ( i4_speed != 0 && pt_as_obj->t_base.i4_speed == 0 )
    {
        i4_ret = rm_set( pt_as_obj->t_ad_descr.h_comp,
                         FALSE,
                         ANY_PORT_NUM,
                         AUD_DEC_SET_TYPE_CTRL,
                         (VOID*)AUD_DEC_CTRL_RESUME,
                         sizeof(AUD_DEC_CTRL_T),
                         &pt_as_obj->t_ad_descr.e_comp_cond);
        if ( i4_ret != RMR_OK && i4_ret != RMR_ASYNC_NFY )
        {
            return SMR_DRV_COMP_ERROR;    
        }
        SM_AS_DBG_INFO(( "[SM] Set Audio Decoder AUD_DEC_CTRL_RESUME\r\n" ));
    }
    else
    {
        /* Ignore other cases */
        SM_AS_DBG_INFO(( "[SM] AS: Ignore speed %d\r\n", i4_speed ));
        return SMR_OK;
    }
    
    pt_as_obj->t_base.i4_speed = i4_speed;
    
    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: sm_as_ad_stop_drvs
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_as_obj References.
 *
 * Outputs: - 
 *
 * Returns: SMR_INTERNAL_ERROR  Success.
 *          SMR_OK              Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_as_ad_stop_drvs( 
    HANDLE_T              h_stream,
    SM_AUDIO_STREAM_T*    pt_as_obj )
{
    INT32                 i4_ret;

    if ( pt_as_obj->ui4_lastest_ad_event == SM_EVN_AS_AD_STOPPED )
    {
        return SMR_OK;
    }

    pt_as_obj->e_ad_ctrl_type = AUD_DEC_CTRL_STOP;

    TMS_BEGIN( AS_HANDLE_AD_STOPPED );
    i4_ret = rm_set( pt_as_obj->t_ad_descr.h_comp,
                  FALSE,
                  ANY_PORT_NUM,
                  AUD_DEC_SET_TYPE_CTRL,
                  (VOID*)pt_as_obj->e_ad_ctrl_type,
                  sizeof(AUD_DEC_CTRL_T),
                  &pt_as_obj->t_ad_descr.e_comp_cond);
    if ( i4_ret != RMR_OK && i4_ret != RMR_ASYNC_NFY )
    {
        return SMR_DRV_COMP_ERROR;
    }
    SM_AS_DBG_INFO(( "[SM] Set Audio Decoder AUD_DEC_CTRL_STOP\r\n" ));
    
    /* reset default speed to 100 */
    pt_as_obj->t_base.i4_speed = 100;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_as_ad_set_callback
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_as_obj References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
INT32 sm_as_ad_set_callback( 
    HANDLE_T              h_stream,
    SM_AUDIO_STREAM_T*    pt_as_obj )
{
    INT32                i4_ret;
    AUD_DEC_NFY_INFO_T   t_set_nfy_info;
    
    t_set_nfy_info.pf_aud_dec_nfy = _sm_as_ad_ctrl_nfy_fct;
    t_set_nfy_info.pv_tag = (VOID*)h_stream;
    
    i4_ret = rm_set( pt_as_obj->t_ad_descr.h_comp,
                     FALSE,
                     ANY_PORT_NUM,
                     AUD_DEC_SET_TYPE_NFY_FCT,
                     (VOID*)&t_set_nfy_info,
                     sizeof(AUD_DEC_NFY_INFO_T),
                     &pt_as_obj->t_ad_descr.e_comp_cond );
    if ( i4_ret != RMR_OK )
    {
        return SMR_DRV_COMP_ERROR;    
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_as_comp_ad_set_codec
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_vs_obj References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
INT32 sm_as_comp_ad_set_codec( HANDLE_T              h_stream,
                               SM_AUDIO_STREAM_T*    pt_as_obj )
{
    AUD_ENC_T             e_enc;
    AUD_DEC_FMT_T         e_fmt;
    INT32                 i4_ret;

    switch ( pt_as_obj->t_base.t_scdb_rec.e_rec_type )
    {
    case SCDB_REC_TYPE_AUDIO_MPEG:
        e_enc = pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.e_enc;
        break;

    case SCDB_REC_TYPE_AUDIO_AVI:
        e_enc = pt_as_obj->t_base.t_scdb_rec.u.t_audio_avi.e_enc;
        break;

    case SCDB_REC_TYPE_AUDIO_MP4:
        e_enc = pt_as_obj->t_base.t_scdb_rec.u.t_audio_mp4.e_enc;
        break;

    case SCDB_REC_TYPE_AUDIO_ASF:
        e_enc = pt_as_obj->t_base.t_scdb_rec.u.t_audio_asf.e_enc;
        break;

    case SCDB_REC_TYPE_AUDIO_PS:
        e_enc = pt_as_obj->t_base.t_scdb_rec.u.t_audio_ps.e_enc;
        break;

    case SCDB_REC_TYPE_AUDIO_MKV:
        e_enc = pt_as_obj->t_base.t_scdb_rec.u.t_audio_mkv.e_enc;
        break;

    case SCDB_REC_TYPE_AUDIO_RM:
        e_enc = pt_as_obj->t_base.t_scdb_rec.u.t_audio_rm.e_enc;
        break;

    /*add for flv*/
    case SCDB_REC_TYPE_AUDIO_FLV:
        e_enc = pt_as_obj->t_base.t_scdb_rec.u.t_audio_flv.e_enc;
        break;

    /*add for ogg*/
    case SCDB_REC_TYPE_AUDIO_OGG:
        e_enc = pt_as_obj->t_base.t_scdb_rec.u.t_audio_ogg.e_enc;
        break;
    default:
        e_enc = AUD_ENC_UNKNOWN;
    }

    switch( e_enc )
    {
    case AUD_ENC_MPEG_1:
    case AUD_ENC_MPEG_2:
        if ( pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.e_layer == AUD_LAYER_3 )
        {
            e_fmt = AUD_DEC_FMT_MP3;
        }
        else
        {
            e_fmt = AUD_DEC_FMT_MPEG;
        }
        break;
    case AUD_ENC_AAC:
        e_fmt = AUD_DEC_FMT_AAC;
        break;
#ifdef ENABLE_MULTIMEDIA        
    case AUD_ENC_WMA_V1:       
        e_fmt = AUD_DEC_FMT_WMA;
        break;
    case AUD_ENC_WMA_V2:       
        e_fmt = AUD_DEC_FMT_WMA;
        break;
    case AUD_ENC_WMA_V3:        
        e_fmt = AUD_DEC_FMT_WMA;
        break;
    case AUD_ENC_WMA_PRO:        
        e_fmt = AUD_DEC_FMT_WMAPRO;
        break;   
    case AUD_ENC_PCM:  
    case AUD_ENC_LPCM:      
        e_fmt = AUD_DEC_FMT_PCM;
        break;
    case AUD_ENC_COOK:      
        e_fmt = AUD_DEC_FMT_COOK;
        break;
    case AUD_ENC_AWB:      
        e_fmt = AUD_DEC_FMT_AWB;
        break;
    case AUD_ENC_AMR:      
        e_fmt = AUD_DEC_FMT_AMR;
        break;
    /*case AUD_ENC_LPCM:        
        pt_drv_aud_fmt->e_fmt = 19;
        pt_drv_aud_fmt->pv_info = NULL;
        break;*/
#endif
    case AUD_ENC_AC3:
    case AUD_ENC_UNKNOWN:
    default:
        e_fmt = AUD_DEC_FMT_AC3;
        break;
    }

    
    i4_ret = rm_set( pt_as_obj->t_ad_descr.h_comp,
                     FALSE,
                     ANY_PORT_NUM,
                     AUD_DEC_SET_TYPE_CODEC,
                     (VOID*)(UINT32)e_fmt,
                     sizeof(AUD_DEC_NFY_INFO_T),
                     &pt_as_obj->t_ad_descr.e_comp_cond );
    if ( i4_ret != RMR_OK )
    {
        return SMR_DRV_COMP_ERROR; 
    }

    return SMR_OK;
}

