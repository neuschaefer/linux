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
 * $RCSfile: _scc_aud.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This header file contains specific implementation of SCC Video 
 *         Component.
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "res_mngr/rm.h"
#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/drv/x_aud_dec.h"
#include "strm_mngr/x_sm.h"
#include "strm_mngr/sm_slctr.h"
#include "strm_mngr/audio_hdlr/u_sm_audio_hdlr.h"

#include "strm_mngr/_sm_utils.h"
#include "strm_mngr/scc/_scc_aud.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/

static HANDLE_T _get_aud_dec_from_as( HANDLE_T h_as )
{
    HANDLE_T      h_drv_clone;
    HANDLE_T      h_aud_dec;
    HANDLE_TYPE_T e_drv_type;
    SIZE_T        z_size;
    INT32         i4_ret;
    
    z_size = sizeof(HANDLE_T);
    i4_ret = x_sm_get( 
        h_as,
        SM_ASH_GET_TYPE_AUDIO_DEC,
        (VOID*)&h_drv_clone,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return NULL_HANDLE;
    }
    
    h_aud_dec = sm_drv_hdl_clone_get_hdl(h_drv_clone);

    i4_ret = rm_get_comp_info_from_handle( 
        h_aud_dec,
        &e_drv_type,
        NULL,
        NULL,
        NULL,
        NULL );
    if ( i4_ret != RMR_OK || e_drv_type != DRVT_AUD_DEC )
    {
        return NULL_HANDLE;
    }
    
    return h_aud_dec;
}

static BOOL _scc_comp_aud_free_fct( 
    HANDLE_T       h_handle,
    HANDLE_TYPE_T  e_drv_type,
    VOID*          pv_obj,
    VOID*          pv_tag,
    BOOL           b_req_handle )
{
    SCC_AUD_COMP_T*   pt_aud_comp = (SCC_AUD_COMP_T*)pv_obj;
    
    if ( b_req_handle )
    {
        return FALSE;
    }
    if ( !pv_obj )
    {
        SM_ASSERT( 0 );
    }
    
    _scc_drv_grp_close( pt_aud_comp->ah_aud_dec, SCC_COMP_MAX_NUM_DRV_COMPS );
    x_mem_free( pv_obj );
    
    return TRUE;
}

static VOID _scc_comp_aud_get_aud_dec_cap(
    SCC_AUD_COMP_T*   pt_aud_comp )
{
    INT32               i4_ret = RMR_OK;
    INT32               i4_idx = 0;
    SIZE_T              z_size;
    RM_COND_T           e_cond;
    
    while( i4_idx < SCC_COMP_MAX_NUM_DRV_COMPS && 
           pt_aud_comp->ah_aud_dec[i4_idx] != NULL_HANDLE )
    {
        z_size = sizeof(AUD_DEC_CAPABILITY_INFO_T);
        i4_ret = rm_get( 
            pt_aud_comp->ah_aud_dec[i4_idx],
            FALSE,
            ANY_PORT_NUM,
            AUD_DEC_GET_TYPE_CAPABILITY,
            (VOID*)&pt_aud_comp->at_aud_dec_cap[i4_idx],
            &z_size,
            &e_cond );
        if ( i4_ret != RMR_OK )
        {
            SM_SLCTR_DBG_INFO(( 
                "[SCC] %s:%d rm_get(AUD_DEC_GET_TYPE_CAPABILITY) fail return %d\r\n",
                __FILE__, __LINE__,
                i4_ret ));
        }
        
        ++i4_idx;
    }    
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/

INT32 _scc_comp_aud_open( 
    HANDLE_T            h_as,
    const CHAR*         s_grp_name,
    HANDLE_T*           ph_comp )
{
    INT32               i4_ret = SMR_OK;
    SCC_AUD_COMP_T*   pt_aud_comp = NULL;
    
    if ( h_as == NULL_HANDLE && !s_grp_name )
    {
        return SMR_INV_ARG;
    }
    if ( !ph_comp )
    {
        return SMR_INV_ARG;
    }
    
    pt_aud_comp = (SCC_AUD_COMP_T*)x_mem_alloc( sizeof(SCC_AUD_COMP_T) );
    if ( !pt_aud_comp )
    {
        return SMR_INSUFFICIENT_MEMORY;
    }

    x_memset( pt_aud_comp, 0, sizeof(SCC_AUD_COMP_T) );
    pt_aud_comp->e_cmd_type = SCC_CMD_TYPE_AUDIO;
    
    if ( h_as != NULL_HANDLE )
    {
        HANDLE_T      h_drv_aud_dec;
                
        /* Open Audio Decoder */
        h_drv_aud_dec = _get_aud_dec_from_as( h_as );
        if ( h_drv_aud_dec == NULL_HANDLE )
        {
            x_mem_free( pt_aud_comp );
            return SMR_INV_HANDLE;
        }
        
        pt_aud_comp->ah_aud_dec[0] = _scc_open_drv_from_handle( h_drv_aud_dec );
        if ( pt_aud_comp->ah_aud_dec[0] == NULL_HANDLE )
        {
            x_mem_free( pt_aud_comp );
            return SMR_INV_HANDLE;;
        }
    }
    else /* use group name */
    {
        SIZE_T    z_size;
        
        x_strcpy( pt_aud_comp->s_grp_name, s_grp_name );
        
        z_size = SCC_COMP_MAX_NUM_DRV_COMPS;
        i4_ret = _scc_drv_grp_open_from_name(
            NULL_HANDLE,
            DRVT_AUD_DEC,
            s_grp_name,
            pt_aud_comp->ah_aud_dec,
            &z_size );
        if ( i4_ret != SMR_OK )
        {
            x_mem_free( pt_aud_comp );
            return SMR_DRV_COMP_NOT_FOUND;;
        }
    }
    
    _scc_comp_aud_get_aud_dec_cap( pt_aud_comp );
    
    i4_ret = handle_alloc( SM_HT_SCC_GRP_NAME,
                           (VOID*)pt_aud_comp,
                           (VOID*)pt_aud_comp->e_cmd_type,
                           &_scc_comp_aud_free_fct,
                           ph_comp );
    if ( i4_ret != HR_OK )
    {
        _scc_drv_grp_close( pt_aud_comp->ah_aud_dec, SCC_COMP_MAX_NUM_DRV_COMPS );
        x_mem_free( pt_aud_comp );
        return SMR_OUT_OF_HANDLES;
    }
    
    return SMR_OK;
}

INT32 _scc_comp_aud_close( 
    HANDLE_T            h_comp )
{
    INT32               i4_ret = HR_OK;
    HANDLE_TYPE_T       e_type;
    SCC_AUD_COMP_T*     pt_aud_comp = NULL;

    _scc_lock();
    
    i4_ret = handle_get_type_obj( 
        h_comp,
        &e_type,
        (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        _scc_unlock();
        return SMR_INV_HANDLE;
    }
    if ( pt_aud_comp->e_cmd_type != SCC_CMD_TYPE_AUDIO )
    {
        _scc_unlock();
        return SMR_INV_HANDLE;
    }
    
    i4_ret = handle_free( h_comp, FALSE );
    if ( i4_ret != HR_OK )
    {
        _scc_unlock();
        SM_SLCTR_DBG_INFO(( 
            "[SCC] %s:%d handle_free() fail return %d\r\n",
            __FILE__, __LINE__,
            i4_ret ));
        return SMR_INV_HANDLE;
    }
    
    _scc_unlock();
    return SMR_OK;
}

INT32 _scc_comp_aud_set( 
    HANDLE_T            h_comp,
    DRV_TYPE_T          e_drv_type,
    UINT32              e_drv_set_type,
    VOID*               pv_set_info,
    SIZE_T              z_set_size )
{
    INT32               i4_ret;
    INT32               i4_idx;
    HANDLE_TYPE_T       e_type;
    SCC_AUD_COMP_T*     pt_aud_comp = NULL;
    HANDLE_T*           ph_drv_array;
    RM_COND_T           e_cond;
    
    _scc_lock();

    i4_ret = handle_get_type_obj( 
        h_comp,
        &e_type,
        (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        _scc_unlock();
        return SMR_INV_HANDLE;
    }
    if ( pt_aud_comp->e_cmd_type != SCC_CMD_TYPE_AUDIO )
    {
        _scc_unlock();
        return SMR_INV_HANDLE;
    }
    
    if ( e_drv_type == DRVT_AUD_DEC )
    {
        ph_drv_array = pt_aud_comp->ah_aud_dec;
    }
    else
    {
        _scc_unlock();
        return SMR_INV_ARG;
    }
    
    i4_idx = 0;
    while( i4_idx < SCC_COMP_MAX_NUM_DRV_COMPS &&
           ph_drv_array[i4_idx] != NULL_HANDLE )
    {
        i4_ret = rm_set(
            ph_drv_array[i4_idx],
            FALSE,
            ANY_PORT_NUM,
            e_drv_set_type,
            pv_set_info,
            z_set_size,
            &e_cond);
        if (( i4_ret != RMR_OK )
            && ( i4_ret != RMR_ASYNC_NFY))
        {
            _scc_unlock();
            SM_SLCTR_DBG_INFO(( 
                "[SCC] %s:%d rm_set() fail return %d\r\n",
                __FILE__, __LINE__,
                i4_ret ));
            return SMR_DRV_COMP_ERROR;
        }
        
        i4_idx++;
    }
    
    _scc_unlock();
    
    return SMR_OK;
}

INT32 _scc_comp_aud_get( 
    HANDLE_T            h_comp,
    DRV_TYPE_T          e_drv_type,
    UINT32              e_drv_get_type,
    VOID*               pv_get_info,
    SIZE_T*             pz_get_size )
{
    INT32               i4_ret;
    INT32               i4_idx;
    HANDLE_TYPE_T       e_type;
    SCC_AUD_COMP_T*     pt_aud_comp = NULL;
    HANDLE_T*           ph_drv_array;
    RM_COND_T           e_cond;

    _scc_lock();
    
    i4_ret = handle_get_type_obj( 
        h_comp,
        &e_type,
        (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        _scc_unlock();
        return SMR_INV_HANDLE;
    }
    if ( pt_aud_comp->e_cmd_type != SCC_CMD_TYPE_AUDIO )
    {
        _scc_unlock();
        return SMR_INV_HANDLE;
    }
    
    if ( e_drv_type == DRVT_AUD_DEC )
    {
        ph_drv_array = pt_aud_comp->ah_aud_dec;
    }
    else
    {
        _scc_unlock();
        return SMR_INV_ARG;
    }
    
    i4_idx = 0;
    while( i4_idx < SCC_COMP_MAX_NUM_DRV_COMPS &&
           ph_drv_array[i4_idx] != NULL_HANDLE )
    {
        i4_ret = rm_get( 
            ph_drv_array[i4_idx],
            FALSE,
            ANY_PORT_NUM,
            e_drv_get_type,
            (VOID*)pv_get_info,
            pz_get_size,
            &e_cond );
        if ( i4_ret != RMR_OK )
        {
            _scc_unlock();
            SM_SLCTR_DBG_INFO(( 
                "[SCC] %s:%d rm_get() fail return %d\r\n",
                __FILE__, __LINE__,
                i4_ret ));
            return SMR_DRV_COMP_ERROR;
        }
        
        i4_idx++;
    }
    
    _scc_unlock();

    return SMR_OK;
}

VOID _scc_comp_aud_hdph_detect_nfy_fct (
    VOID*                     pv_nfy_tag,
    AUD_DEC_HEADPHONE_COND_T  e_nfy_cond,
    UINT32                    ui4_data_1, 
    UINT32                    ui4_data_2 )
{
    SM_SCC_MSG_T    t_msg;

    t_msg.e_msg_type = SM_SCC_MSG_TYPE_HDPHONE_NFY;
    t_msg.u.t_aud_hdphone_nfy.pv_nfy_tag = pv_nfy_tag;
    t_msg.u.t_aud_hdphone_nfy.ui4_data_1 = ui4_data_1;
    t_msg.u.t_aud_hdphone_nfy.ui4_data_2 = ui4_data_2;

    switch( e_nfy_cond )
    {
    case AUD_DEC_HEADPHONE_COND_UNPLUGED:
        t_msg.u.t_aud_hdphone_nfy.e_hdphone_cond = SCC_AUD_HDPHONE_COND_UNPLUGED;
        break;
    
    case AUD_DEC_HEADPHONE_COND_PLUGED:
        t_msg.u.t_aud_hdphone_nfy.e_hdphone_cond = SCC_AUD_HDPHONE_COND_PLUGED;
        break;
    
    default:
        return;
    }
    
    _scc_send_msg( &t_msg );
}

VOID _scc_comp_aud_upload_data_nfy_fct (
    UINT8           ui1_dec_id,
    VOID*           pv_nfy_tag,
    AUD_DEC_COND_T  e_nfy_cond,
    UINT32          ui4_data_1,
    UINT32          ui4_data_2 )
{
    SM_SCC_MSG_T    t_msg;

    switch( e_nfy_cond )
    {
    case AUD_DEC_COND_UPLOAD_INFO_CHG:
        {
            t_msg.e_msg_type = SM_SCC_MSG_TYPE_UPLOAD_DATA_NFY;
            t_msg.u.t_aud_upload_data_nfy.pv_nfy_tag = pv_nfy_tag;
            t_msg.u.t_aud_upload_data_nfy.ui4_data_1 = ui4_data_1;
            t_msg.u.t_aud_upload_data_nfy.ui4_data_2 = ui4_data_2;
            
            _scc_send_msg( &t_msg );
            /*x_dbg_stmt("   Audio decoder notify AUD_DEC_COND_UPLOAD_INFO_CHG add=%x,len=%x\r\n",ui4_data_1,ui4_data_2);*/
        }
        break;
    
    
    default:
        return;
    }
    
}


