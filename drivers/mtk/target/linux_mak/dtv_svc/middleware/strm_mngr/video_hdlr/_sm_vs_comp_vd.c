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
 * $RCSfile: _sm_vs_comp_vd.c,v $
 * $Revision: #2 $
 * $Date: 2012/11/03 $
 * $Author: xingju.wang $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/17 $
 * $SWAuthor: Alec Lu $
 * $MD5HEX: ff30432460a0575709cf2e578efcfb8d $
 *
 * Description: 
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "dbg/x_dbg.h"
#include "dbg/dbg.h"
#include "res_mngr/rm.h"
#include "res_mngr/x_rm_dev_types.h"
#include "strm_mngr/x_sm.h"
#include "strm_mngr/_sm_utils.h"

#include "strm_mngr/video_hdlr/_sm_vs_state.h"
#include "strm_mngr/video_hdlr/_sm_video_hdlr.h"
#include "strm_mngr/video_hdlr/_sm_vs_comp_vd.h"
#include "strm_mngr/sm_drv_map.h"

#ifndef RM_AUTO_CONNECT
    #include "prc_mngr/prc_mngr.h"
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/
static SELECT_T _rm_comp_sel_fct_vd( 
                    DRV_TYPE_T   e_type,
                    UINT16       ui2_id,
                    VOID*        pv_comp_sel_data,
                    const VOID*  pv_comp_data,
                    SIZE_T       z_comp_data_len,
                    UINT16       ui2_num_active,
                    UINT16       ui2_num_wait,
                    CTRL_TYPE_T  e_ctrl,
                    BOOL         b_would_conflict );

static VOID _rm_nfy_fct_vd( HANDLE_T   h_obj,
                            UINT8      ui1_port,
                            RM_COND_T  e_nfy_cond,
                            VOID*      pv_tag,
                            UINT32     ui4_data);

static VOID _sm_vs_codec_convert_scdb_to_vd( 
    VID_ENC_T           e_enc,
    VID_DEC_CODEC_T*    pe_vd_codec_type );

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: _rm_comp_sel_fct_vd
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
static SELECT_T _rm_comp_sel_fct_vd( 
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

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if ( sm_drv_map_exists( &t_drv_descr ) )
#else
    if ( sm_drv_map_exists( &t_drv_descr ) || b_would_conflict )
#endif
    {
        return SEL_NEXT;
    }
    
    return SEL_COMP_BREAK;
}

/*-----------------------------------------------------------------------------
 * Name: _rm_nfy_fct_vd
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
static VOID _rm_nfy_fct_vd( HANDLE_T   h_obj,
                            UINT8      ui1_port,
                            RM_COND_T  e_nfy_cond,
                            VOID*      pv_tag,
                            UINT32     ui4_data)
{
    SM_VIDEO_HDLR_MSG_T         t_msg;
    HANDLE_T                    h_stream = (HANDLE_T)pv_tag;
    RM_COND_T                   e_cond   = RM_COND_IGNORE; 
    
    if ( !x_handle_valid( h_stream ) )
    {
        SM_VS_DBG_ERR((
            "[SM] VS, _rm_nfy_fct_vd(), h_stream(0x%.8X) is invalid!\r\n", h_stream ));
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
    
    t_msg.h_stream = h_stream;
    t_msg.e_msg_type = SM_VIDEO_HDLR_MSG_DEC_RM_NFY;
    
    t_msg.u.t_msg_rm.h_comp = h_obj;
    t_msg.u.t_msg_rm.ui1_port = ui1_port;
    t_msg.u.t_msg_rm.e_cond = e_nfy_cond;
    t_msg.u.t_msg_rm.ui4_data = ui4_data;
    t_msg.u.t_msg_rm.b_should_update_comp_descr = TRUE;
    
    sm_vs_send_msg( &t_msg );
}

static VOID _sm_vs_codec_convert_scdb_to_vd( 
    VID_ENC_T           e_enc,
    VID_DEC_CODEC_T*    pe_vd_codec_type )
{
    switch ( e_enc )
    {
#ifndef MW_CODEC_MPEG_1_DISABLE
        case VID_ENC_MPEG_1:
            *pe_vd_codec_type = VID_DEC_CODEC_MPEG_1;
            break;
#endif

#ifndef MW_CODEC_MPEG_2_DISABLE
        case VID_ENC_MPEG_2:
            *pe_vd_codec_type = VID_DEC_CODEC_MPEG_2;
            break;
#endif

#ifndef MW_CODEC_MPEG_4_DISABLE
        case VID_ENC_MPEG_4:
            *pe_vd_codec_type = VID_DEC_CODEC_MPEG_4;
            break;
#endif

#ifndef MW_CODEC_DIVX_311_DISABLE
        case VID_ENC_DIVX_311:
            *pe_vd_codec_type = VID_DEC_CODEC_DIVX_311;
            break;
#endif

#ifndef MW_CODEC_DIVX_4_DISABLE
        case VID_ENC_DIVX_4:
            *pe_vd_codec_type = VID_DEC_CODEC_DIVX_4;
            break;
#endif

#ifndef MW_CODEC_DIVX_5_DISABLE
        case VID_ENC_DIVX_5:
            *pe_vd_codec_type = VID_DEC_CODEC_DIVX_5;
            break;
#endif

#ifndef MW_CODEC_XVID_DISABLE
        case VID_ENC_XVID:
            *pe_vd_codec_type = VID_DEC_CODEC_XVID;
            break;
#endif

#ifndef MW_CODEC_WMV1_DISABLE
        case VID_ENC_WMV1:
            *pe_vd_codec_type = VID_DEC_CODEC_WMV1;
            break;
#endif

#ifndef MW_CODEC_WMV2_DISABLE            
        case VID_ENC_WMV2:
            *pe_vd_codec_type = VID_DEC_CODEC_WMV2;
            break;
#endif

#ifndef MW_CODEC_WMV3_DISABLE            
        case VID_ENC_WMV3:
            *pe_vd_codec_type = VID_DEC_CODEC_WMV3;
            break;
#endif

#ifndef MW_CODEC_WVC1_DISABLE            
        case VID_ENC_WVC1:
            *pe_vd_codec_type = VID_DEC_CODEC_WVC1;
            break;
#endif

#ifndef MW_CODEC_H264_DISABLE
        case VID_ENC_H264:
             *pe_vd_codec_type = VID_DEC_CODEC_H264;
            break;
#endif

#ifndef MW_CODEC_H263_DISABLE
        case VID_ENC_H263:
             *pe_vd_codec_type = VID_DEC_CODEC_H263;
            break;
#endif

#ifndef MW_CODEC_MJPEG_DISABLE
        case VID_ENC_MJPEG:
            *pe_vd_codec_type = VID_DEC_CODEC_MJPEG;
            break;
#endif

#ifndef MW_CODEC_RV8_DISABLE
        case VID_ENC_RV8:
            *pe_vd_codec_type = VID_DEC_CODEC_RV8;
            break;
#endif

#ifndef MW_CODEC_RV9_DISABLE
        case VID_ENC_RV9:
            *pe_vd_codec_type = VID_DEC_CODEC_RV9;
            break;   
#endif

#ifndef MW_CODEC_AVS_DISABLE            
        case VID_ENC_AVS:
            *pe_vd_codec_type = VID_DEC_CODEC_AVS;
            break; 
#endif

#ifndef MW_CODEC_NV12_DISABLE  
        case VID_ENC_NV12:
            *pe_vd_codec_type = VID_DEC_CODEC_NV12;
            break;
#endif

#ifndef MW_CODEC_VP8_DISABLE
        case VID_ENC_VP8:
            *pe_vd_codec_type = VID_DEC_CODEC_VP8 ;
            break;
#endif

#ifndef MW_CODEC_VP6_DISABLE
        case VID_ENC_VP6:
            *pe_vd_codec_type = VID_DEC_CODEC_VP6 ;
            break;
#endif

        case VID_ENC_UNKNOWN:
        default:
             *pe_vd_codec_type = VID_DEC_CODEC_UNKNOWN;
    }
}

static CHAR* _sm_video_hdlr_set_codec_str(
    VID_ENC_T   e_actual_enc )
{
    switch( e_actual_enc )
    {
    case VID_ENC_UNKNOWN:
        return "VID_ENC_UNKNOWN";

    case VID_ENC_MPEG_1:
        return "VID_ENC_MPEG_1";

    case VID_ENC_MPEG_2:
        return "VID_ENC_MPEG_2";

    case VID_ENC_MPEG_4:
        return "VID_ENC_MPEG_4";

    case VID_ENC_DIVX_311:
        return "VID_ENC_DIVX_311";

    case VID_ENC_DIVX_4:
        return "VID_ENC_DIVX_4";
        
    case VID_ENC_DIVX_5:
        return "VID_ENC_DIVX_5";

    case VID_ENC_XVID:
        return "VID_ENC_XVID";
        
    case VID_ENC_WMV1:
        return "VID_ENC_WMV1";
    
    case VID_ENC_WMV2:
        return "VID_ENC_WMV2";
     
    case VID_ENC_WMV3:
        return "VID_ENC_WMV3";
        
    case VID_ENC_WVC1:
        return "VID_ENC_WVC1";
        
    case VID_ENC_H264:
        return "VID_ENC_H264";
        
    case VID_ENC_H263:
        return "VID_ENC_H263";
        
    case VID_ENC_MJPEG:
        return "VID_ENC_MJPEG";
        
    case VID_ENC_RV8:
        return "VID_ENC_RV8";
        
    case VID_ENC_RV9:
        return "VID_ENC_RV9";
        
    case VID_ENC_SORENSON:
        return "VID_ENC_SORENSON";
        
    case VID_ENC_AVS:
        return "VID_ENC_AVS"; 
     
    case VID_ENC_NV12:
        return "VID_ENC_NV12"; 

    case VID_ENC_VP8:
        return "VID_ENC_VP8"; 

    case VID_ENC_VP6:
        return "VID_ENC_VP6"; 
             
    default:
        return "INVALID_VALUE";
    }
}

/*-----------------------------------------------------------------------------
 * Name: _sm_vs_comp_vd_ctrl_nfy
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
VOID _sm_vs_comp_vd_ctrl_nfy( 
    VOID*               pv_nfy_tag,
    VID_DEC_COND_T      e_nfy_cond,
    UINT32              ui4_data_1,
    UINT32              ui4_data_2 )
{
    SM_VIDEO_HDLR_MSG_T         t_msg;
    HANDLE_T                    h_stream = (HANDLE_T)pv_nfy_tag;
    
    if ( !x_handle_valid( h_stream ) )
    {
        SM_VS_DBG_ERR((
            "[SM] VS, _sm_vs_comp_vd_ctrl_nfy(), h_stream(0x%.8X) is invalid!\r\n", h_stream ));
        return;
    }
    
    t_msg.h_stream = h_stream;
    t_msg.e_msg_type = SM_VIDEO_HDLR_MSG_VD_CTRL_NFY;
    
    t_msg.u.t_msg_vd_ctrl.e_vd_nfy_cond = e_nfy_cond;
    t_msg.u.t_msg_vd_ctrl.ui4_data_1 = ui4_data_1;
    t_msg.u.t_msg_vd_ctrl.ui4_data_2 = ui4_data_2;
    
    sm_vs_send_msg( &t_msg );
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: sm_vs_comp_vd_open
 *
 * Description: <function description>
 *
 * Inputs:  h_stream      References.
 *          h_source      References.
 *          pt_comp_descr References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK              Success.
 *          SMR_DRV_COMP_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_comp_vd_open( HANDLE_T            h_stream,
                          HANDLE_T            h_source, 
                          SM_DRV_COMP_DESCR_T* pt_comp_descr )
{
    INT32 i4_ret;
    
    i4_ret = sm_drv_comp_open( h_source,
                               pt_comp_descr->t_info.e_type,
                               pt_comp_descr->t_info.ui2_comp_id,
                               pt_comp_descr->t_info.s_logical_name,
                               (VOID*)pt_comp_descr,
                               &_rm_comp_sel_fct_vd,
                               (VOID*)h_stream,
                               &_rm_nfy_fct_vd,
                               pt_comp_descr );
    if ( i4_ret != SMR_OK || pt_comp_descr->h_comp == NULL_HANDLE )
    {
        return SMR_DRV_COMP_NOT_FOUND;
    }

    /* Send RM notify message */
    {
        SM_VIDEO_HDLR_MSG_T         t_msg;
                
        t_msg.h_stream = h_stream;
        t_msg.e_msg_type = SM_VIDEO_HDLR_MSG_DEC_RM_NFY;
        
        t_msg.u.t_msg_rm.h_comp = pt_comp_descr->h_comp;
        t_msg.u.t_msg_rm.ui1_port = pt_comp_descr->t_info.ui1_in_port;
        t_msg.u.t_msg_rm.e_cond = pt_comp_descr->e_comp_cond;
        t_msg.u.t_msg_rm.ui4_data = 0;
        t_msg.u.t_msg_rm.b_should_update_comp_descr = FALSE;
        
        sm_vs_send_msg( &t_msg );
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_vs_comp_vd_connect
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_vs_obj References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK              Success.
 *          SMR_DRV_COMP_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_comp_vd_connect( HANDLE_T            h_stream, 
                             SM_VIDEO_STREAM_T*  pt_vs_obj )
{
    INT32              i4_ret;
    HANDLE_T           h_source;
#ifdef MW_TV_AV_BYPASS_SUPPORT
    CONN_STATE_T       at_conn_state[1];
    UINT16             ui2_num_entries = 1;
    CONN_DIR_TYPE_T    ae_conn_dir[1];
#endif

#ifndef RM_AUTO_CONNECT
    HANDLE_T  h_dmx = NULL_HANDLE;
    
    i4_ret = pm_get_component_info( pt_vs_obj->t_base.h_pm_comp,
                                    &h_dmx );
    if ( i4_ret != PMR_OK )
    {
        return SMR_DRV_COMP_ERROR;
    }
    h_source = h_dmx;
#else
    h_source = pt_vs_obj->t_base.h_source;
#endif

#ifdef MW_TV_AV_BYPASS_SUPPORT
    i4_ret = rm_get_conn_state( pt_vs_obj->t_vs_comp_grp.t_dec.h_comp,
                                h_source,
                                at_conn_state,
                                &ui2_num_entries,
                                ae_conn_dir);
    if ( i4_ret != RMR_OK )
    {
        return SMR_DRV_COMP_ERROR;
    }
    
    if ( !(at_conn_state[0].ui1_conn_state & CONN_STATE_CONNECTED) &&
         !(at_conn_state[0].ui1_conn_state & CONN_STATE_HARD_WIRED) )
#endif /* MW_TV_AV_BYPASS_SUPPORT */
    {
        i4_ret = sm_drv_comp_connect( &pt_vs_obj->t_vs_comp_grp.t_dec, 
                                      h_source,
                                      (VOID*)h_stream );
        if ( i4_ret != SMR_OK )
        {
            return SMR_DRV_COMP_ERROR;
        }
    
        /* Send RM notify message */
        {
            SM_VIDEO_HDLR_MSG_T         t_msg;
                    
            t_msg.h_stream = h_stream;
            t_msg.e_msg_type = SM_VIDEO_HDLR_MSG_DEC_RM_NFY;
            
            t_msg.u.t_msg_rm.h_comp = pt_vs_obj->t_vs_comp_grp.t_dec.h_comp;
            t_msg.u.t_msg_rm.ui1_port = pt_vs_obj->t_vs_comp_grp.t_dec.t_info.ui1_in_port;
            t_msg.u.t_msg_rm.e_cond = pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond;
            t_msg.u.t_msg_rm.ui4_data = 0;
            t_msg.u.t_msg_rm.b_should_update_comp_descr = FALSE;
            
            sm_vs_send_msg( &t_msg );
        }
    }
#ifdef MW_TV_AV_BYPASS_SUPPORT
    else
    {
        sm_vs_send_msg_do_event( h_stream, SM_EVN_VS_DEC_CONNECTED, 0 );
    }
#endif

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_vs_comp_vd_disconnect
 *
 * Description: <function description>
 *
 * Inputs:  pt_from_comp References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK              Success.
 *          SMR_DRV_COMP_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_comp_vd_disconnect( 
    HANDLE_T             h_stream, 
    SM_DRV_COMP_DESCR_T* pt_from_comp )
{
    INT32                i4_ret = SMR_OK;
#ifdef MW_TV_AV_BYPASS_SUPPORT
    RM_COND_T            e_cond = RM_COND_IGNORE;
    
    e_cond = rm_cond_chg( pt_from_comp->e_comp_cond, 
                          RM_COND_IGNORE, 
                          COND_GRP_CONN_ORIG );

    if ( e_cond == RM_COND_CONN_ORIGINATOR )
#endif
    {
        i4_ret = sm_drv_comp_disconnect(pt_from_comp);
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
        
        /* Send RM notify message */
        {
            SM_VIDEO_HDLR_MSG_T         t_msg;
                    
            t_msg.h_stream = h_stream;
            t_msg.e_msg_type = SM_VIDEO_HDLR_MSG_DEC_RM_NFY;
            
            t_msg.u.t_msg_rm.h_comp = pt_from_comp->h_comp;
            t_msg.u.t_msg_rm.ui1_port = pt_from_comp->t_info.ui1_in_port;
            t_msg.u.t_msg_rm.e_cond = pt_from_comp->e_comp_cond;
            t_msg.u.t_msg_rm.ui4_data = 0;
            t_msg.u.t_msg_rm.b_should_update_comp_descr = FALSE;
            
            sm_vs_send_msg( &t_msg );
        }
    }
#ifdef MW_TV_AV_BYPASS_SUPPORT
    else
    {
        sm_vs_send_msg_do_event( h_stream, SM_EVN_VS_DEC_DISCONNECTED, 0 );
    }
#endif
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: sm_vs_comp_vd_set_callback
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
VOID sm_vs_comp_vd_set_callback( HANDLE_T              h_stream,
                                 SM_VIDEO_STREAM_T*    pt_vs_obj )
{
    INT32                 i4_ret;
    VID_DEC_NFY_INFO_T    t_set_nfy_info;
    
    t_set_nfy_info.pf_vid_dec_nfy = _sm_vs_comp_vd_ctrl_nfy;
    t_set_nfy_info.pv_tag = (VOID*)h_stream;
    
    i4_ret = rm_set( pt_vs_obj->t_vs_comp_grp.t_dec.h_comp,
                  FALSE,
                  ANY_PORT_NUM,
                  VID_DEC_SET_TYPE_NFY_FCT,
                  (VOID*)&t_set_nfy_info,
                  sizeof(VID_DEC_NFY_INFO_T),
                  &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond );
    if ( i4_ret != RMR_OK )
    {
        SM_VS_DBG_ERR(( 
            "[SM] VS, rm_set(VID_DEC_SET_TYPE_NFY_FCT) fail, i4_ret = %d\r\n",
            i4_ret ));
        SM_ASSERT( 0 );
        return;
    }
}

/*-----------------------------------------------------------------------------
 * Name: sm_vs_comp_vd_set_codec
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
INT32 sm_vs_comp_vd_set_codec( HANDLE_T              h_stream,
                               SM_VIDEO_STREAM_T*    pt_vs_obj )
{
    VID_DEC_CODEC_T       e_vd_codec_type;
    INT32                 i4_ret;
#ifdef MW_TV_AV_BYPASS_SUPPORT
    RM_COND_T             e_cond = RM_COND_IGNORE;
    
    e_cond = rm_cond_chg( pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond, 
                          RM_COND_IGNORE, 
                          COND_GRP_CONN_ORIG );

    if ( e_cond == RM_COND_CONN_ORIGINATOR )
#endif
    {
        if ( !pt_vs_obj->b_codec_not_support )
        {
            return SMR_OK;
        }

        _sm_vs_codec_convert_scdb_to_vd( pt_vs_obj->e_actual_enc, &e_vd_codec_type );

        x_dbg_stmt( "[SM] VS(0x%.8X), set video codec(%s)\r\n", 
                     h_stream, _sm_video_hdlr_set_codec_str((VID_ENC_T)pt_vs_obj->e_actual_enc));
        
        i4_ret = rm_set( pt_vs_obj->t_vs_comp_grp.t_dec.h_comp,
                         FALSE,
                         ANY_PORT_NUM,
                         VID_DEC_SET_TYPE_CODEC,
                         (VOID*)(UINT32)e_vd_codec_type,
                         sizeof(VID_DEC_CODEC_T),
                         &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond );
        if ( i4_ret != RMR_OK )
        {
            SM_VS_DBG_ERR(( 
                "[SM] VS, rm_set(VID_DEC_SET_TYPE_CODEC) fail, i4_ret = %d\r\n",
                i4_ret ));
            sm_vs_send_msg_do_event( 
                h_stream,
                SM_EVN_VS_CODEC_NOT_SUPPORT,
                (UINT32)0 );
            return SMR_INTERNAL_ERROR;
        }
        pt_vs_obj->b_codec_not_support = FALSE;
    }
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_vs_comp_vd_get_actual_enc
 *
 * Description: <function description>
 *
 * Inputs:  pt_vs_obj  References
 *          
 *
 * Outputs: pt_vs_obj  References
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID sm_vs_comp_vd_get_actual_enc( SM_VIDEO_STREAM_T*    pt_vs_obj )
{
    switch ( pt_vs_obj->t_base.t_scdb_rec.e_rec_type )
    {
    case SCDB_REC_TYPE_VIDEO_MPEG:
        pt_vs_obj->e_actual_enc = pt_vs_obj->t_base.t_scdb_rec.u.t_video_mpeg.e_enc;
        break;

    case SCDB_REC_TYPE_VIDEO_AVI:
        pt_vs_obj->e_actual_enc = pt_vs_obj->t_base.t_scdb_rec.u.t_video_avi.e_enc;
        break;

    case SCDB_REC_TYPE_VIDEO_MP4:
        pt_vs_obj->e_actual_enc = pt_vs_obj->t_base.t_scdb_rec.u.t_video_mp4.e_enc;
        break;

    case SCDB_REC_TYPE_VIDEO_ASF:
        pt_vs_obj->e_actual_enc = pt_vs_obj->t_base.t_scdb_rec.u.t_video_asf.e_enc;
        break;

    case SCDB_REC_TYPE_VIDEO_MKV:
        pt_vs_obj->e_actual_enc = pt_vs_obj->t_base.t_scdb_rec.u.t_video_mkv.e_enc;
        break;

    case SCDB_REC_TYPE_VIDEO_PS:
        pt_vs_obj->e_actual_enc = pt_vs_obj->t_base.t_scdb_rec.u.t_video_ps.e_enc;
        break;

    case SCDB_REC_TYPE_VIDEO_RM:
        pt_vs_obj->e_actual_enc = pt_vs_obj->t_base.t_scdb_rec.u.t_video_rm.e_enc;
        break;
 
    case SCDB_REC_TYPE_VIDEO_FLV:
        pt_vs_obj->e_actual_enc = pt_vs_obj->t_base.t_scdb_rec.u.t_video_flv.e_enc;
        break;
    
    case SCDB_REC_TYPE_VIDEO_ES:
        pt_vs_obj->e_actual_enc = pt_vs_obj->t_base.t_scdb_rec.u.t_video_es.e_enc;
        break;
        
    case SCDB_REC_TYPE_VIDEO_OGG:
        pt_vs_obj->e_actual_enc = pt_vs_obj->t_base.t_scdb_rec.u.t_video_ogg.e_enc;
        break;
        
    default:
        pt_vs_obj->e_actual_enc = VID_ENC_UNKNOWN;
    }
}

/*-----------------------------------------------------------------------------
 * Name: sm_vs_codec_convert_vd_to_scdb
 *
 * Description: <function description>
 *
 * Inputs:  e_vd_codec_type  video codec type
 *          
 *
 * Outputs: pe_enc           scdb enc
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID sm_vs_comp_convert_vd_codec_to_scdb_enc( 
    VID_DEC_CODEC_T     e_vd_codec_type,
    VID_ENC_T*          pe_enc )
{
    switch ( e_vd_codec_type )
    {
    case VID_DEC_CODEC_MPEG_1:
        *pe_enc = VID_ENC_MPEG_1;
        break;

    case VID_DEC_CODEC_MPEG_2:
        *pe_enc =VID_ENC_MPEG_2 ;
        break;

    case VID_DEC_CODEC_MPEG_4:
        *pe_enc =VID_ENC_MPEG_4 ;
        break;

    case VID_DEC_CODEC_DIVX_311:
        *pe_enc =VID_ENC_DIVX_311 ;
        break;

    case VID_DEC_CODEC_DIVX_4:
        *pe_enc =VID_ENC_DIVX_4 ;
        break;

    case VID_DEC_CODEC_DIVX_5:
        *pe_enc =VID_ENC_DIVX_5 ;
        break;

    case VID_DEC_CODEC_XVID:
        *pe_enc =VID_ENC_XVID ;
        break;

    case VID_DEC_CODEC_WMV1:
        *pe_enc =VID_ENC_WMV1 ;
        break;
        
    case VID_DEC_CODEC_WMV2:
        *pe_enc =VID_ENC_WMV2 ;
        break;
        
    case VID_DEC_CODEC_WMV3:
        *pe_enc =VID_ENC_WMV3 ;
        break;
        
    case VID_DEC_CODEC_WVC1:
        *pe_enc =VID_ENC_WVC1 ;
        break;

    case VID_DEC_CODEC_H264:
         *pe_enc =VID_ENC_H264 ;
        break;

    case VID_DEC_CODEC_H263:
         *pe_enc =VID_ENC_H263 ;
        break;
        
    case VID_DEC_CODEC_MJPEG:
        *pe_enc =VID_ENC_MJPEG ;
        break;

    case VID_DEC_CODEC_RV8:
        *pe_enc =VID_ENC_RV8 ;
        break;

    case VID_DEC_CODEC_RV9:
        *pe_enc =VID_ENC_RV9 ;
        break;

    case VID_DEC_CODEC_AVS:
        *pe_enc =VID_ENC_AVS ;
        break;

    case VID_DEC_CODEC_NV12:
        *pe_enc =VID_ENC_NV12 ;
        break;

    case VID_DEC_CODEC_VP8:
        *pe_enc =VID_ENC_VP8 ;
        break;

    case VID_DEC_CODEC_VP6:
        *pe_enc =VID_ENC_VP6 ;
        break;
    
    case VID_DEC_CODEC_UNKNOWN:
    default:
         *pe_enc = VID_ENC_UNKNOWN;
    }
}

