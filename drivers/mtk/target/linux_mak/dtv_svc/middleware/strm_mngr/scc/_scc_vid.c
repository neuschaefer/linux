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
 * $RCSfile: _scc_vid.c,v $
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
#include "res_mngr/drv/x_vid_plane.h"
#include "res_mngr/drv/x_tv_dec.h"
#include "res_mngr/drv/x_vid_dec.h"
#include "strm_mngr/x_sm.h"
#include "strm_mngr/sm_slctr.h"
#include "strm_mngr/video_hdlr/u_sm_video_hdlr.h"

#include "strm_mngr/_sm_utils.h"
#include "strm_mngr/scc/_scc_vid.h"

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

static HANDLE_T _get_vid_pla_from_vs( HANDLE_T h_vs )
{
    HANDLE_T      h_drv_clone;
    HANDLE_T      h_vid_pla;
    HANDLE_TYPE_T e_drv_type;
    SIZE_T        z_size;
    INT32         i4_ret;
    
    z_size = sizeof(HANDLE_T);
    i4_ret = x_sm_get( h_vs,
                       SM_VSH_GET_TYPE_VIDEO_PLANE,
                       (VOID*)&h_drv_clone,
                       &z_size );
    if ( i4_ret != SMR_OK )
    {
        return NULL_HANDLE;
    }
    
    h_vid_pla = sm_drv_hdl_clone_get_hdl(h_drv_clone);

    i4_ret = rm_get_comp_info_from_handle( 
        h_vid_pla,
        &e_drv_type,
        NULL,
        NULL,
        NULL,
        NULL );
    if ( i4_ret != RMR_OK || e_drv_type != DRVT_VID_PLANE )
    {
        return NULL_HANDLE;
    }
    
    return h_vid_pla;
}

static HANDLE_T _get_dec_from_vs( HANDLE_T h_vs, DRV_TYPE_T* pe_drv_type )
{
    HANDLE_T      h_drv_clone;
    HANDLE_T      h_tv_dec;
    SIZE_T        z_size;
    INT32         i4_ret;
    
    if ( !pe_drv_type )
    {
        return NULL_HANDLE;
    }
    
    z_size = sizeof(HANDLE_T);
    i4_ret = x_sm_get( h_vs,
                       SM_VSH_GET_TYPE_DECODER,
                       (VOID*)&h_drv_clone,
                       &z_size );
    if ( i4_ret != SMR_OK )
    {
        return NULL_HANDLE;
    }
    
    h_tv_dec = sm_drv_hdl_clone_get_hdl(h_drv_clone);

    i4_ret = rm_get_comp_info_from_handle( 
        h_tv_dec,
        pe_drv_type,
        NULL,
        NULL,
        NULL,
        NULL );
    if ( i4_ret != RMR_OK || 
         ( *pe_drv_type != DRVT_TV_DEC && *pe_drv_type != DRVT_VID_DEC) )
    {
        return NULL_HANDLE;
    }
    
    return h_tv_dec;
}

static BOOL _scc_comp_vid_free_fct( 
    HANDLE_T       h_handle,
    HANDLE_TYPE_T  e_drv_type,
    VOID*          pv_obj,
    VOID*          pv_tag,
    BOOL           b_req_handle )
{
    SCC_VIDEO_COMP_T*   pt_vid_comp = (SCC_VIDEO_COMP_T*)pv_obj;
    
    if ( b_req_handle )
    {
        return FALSE;
    }
    if ( !pv_obj )
    {
        SM_ASSERT( 0 );
    }
    
    _scc_drv_grp_close( pt_vid_comp->ah_vid_pla, SCC_COMP_MAX_NUM_DRV_COMPS );
    _scc_drv_grp_close( pt_vid_comp->ah_tv_dec, SCC_COMP_MAX_NUM_DRV_COMPS );
    _scc_drv_grp_close( pt_vid_comp->ah_vid_dec, SCC_COMP_MAX_NUM_DRV_COMPS );
    _scc_close_dummy_rm_pipe( &pt_vid_comp->h_dummy_pipe_tvd );
    x_mem_free( pv_obj );

    return TRUE;
}

static VOID _scc_comp_vid_get_vid_pla_cap(
    SCC_VIDEO_COMP_T*   pt_vid_comp )
{
    INT32               i4_ret = RMR_OK;
    INT32               i4_idx = 0;
    SIZE_T              z_size;
    RM_COND_T           e_cond;
    
    while( i4_idx < SCC_COMP_MAX_NUM_DRV_COMPS && 
           pt_vid_comp->ah_vid_pla[i4_idx] != NULL_HANDLE )
    {
        z_size = sizeof(UINT32);
        i4_ret = rm_get( 
            pt_vid_comp->ah_vid_pla[i4_idx],
            FALSE,
            ANY_PORT_NUM,
            VID_PLA_GET_TYPE_CAPABILITY,
            (VOID*)&pt_vid_comp->aui4_vid_pla_cap[i4_idx],
            &z_size,
            &e_cond );
        if ( i4_ret != RMR_OK )
        {
            SM_SLCTR_DBG_INFO(( 
                "[SCC] %s:%d rm_get(VID_PLA_GET_TYPE_CAPABILITY) fail return %d\r\n",
                __FILE__, __LINE__,
                i4_ret ));
        }
        
        ++i4_idx;
    }    
}

static VOID _scc_comp_vid_get_tv_dec_cap(
    SCC_VIDEO_COMP_T*   pt_vid_comp )
{
    INT32               i4_ret = RMR_OK;
    INT32               i4_idx = 0;
    SIZE_T              z_size;
    RM_COND_T           e_cond;
    
    while( i4_idx < SCC_COMP_MAX_NUM_DRV_COMPS && 
           pt_vid_comp->ah_tv_dec[i4_idx] != NULL_HANDLE )
    {
        z_size = sizeof(UINT32);
        i4_ret = rm_get( 
            pt_vid_comp->ah_tv_dec[i4_idx],
            FALSE,
            ANY_PORT_NUM,
            TV_DEC_GET_TYPE_CAPABILITY,
            (VOID*)&pt_vid_comp->aui4_tv_dec_cap[i4_idx],
            &z_size,
            &e_cond );
        if ( i4_ret != RMR_OK )
        {
            SM_SLCTR_DBG_INFO(( 
                "[SCC] %s:%d rm_get(TV_DEC_GET_TYPE_CAPABILITY) fail return %d\r\n",
                __FILE__, __LINE__,
                i4_ret ));
        }
        
        ++i4_idx;
    }    
}

static VOID _scc_comp_vid_get_vid_dec_cap(
    SCC_VIDEO_COMP_T*   pt_vid_comp )
{
    INT32               i4_ret = RMR_OK;
    INT32               i4_idx = 0;
    SIZE_T              z_size;
    RM_COND_T           e_cond;
    
    while( i4_idx < SCC_COMP_MAX_NUM_DRV_COMPS && 
           pt_vid_comp->ah_vid_dec[i4_idx] != NULL_HANDLE )
    {
        z_size = sizeof(VID_DEC_CAPABILITY_INFO_T);
        i4_ret = rm_get( 
            pt_vid_comp->ah_vid_dec[i4_idx],
            FALSE,
            ANY_PORT_NUM,
            VID_DEC_GET_TYPE_CAPABILITY,
            (VOID*)&pt_vid_comp->at_vid_dec_cap[i4_idx],
            &z_size,
            &e_cond );
        if ( i4_ret != RMR_OK )
        {
            SM_SLCTR_DBG_INFO(( 
                "[SCC] %s:%d rm_get(VID_DEC_GET_TYPE_CAPABILITY) fail return %d\r\n",
                __FILE__, __LINE__,
                i4_ret ));
        }
        
        ++i4_idx;
    }    
}

static INT32 _vdp_get_min_max_curr( 
    HANDLE_T                h_vid,
    UINT32                  e_get_type,
    VOID*                   pv_arg,
    UINT16*                 pui2_min,
    UINT16*                 pui2_max,
    UINT16*                 pui2_curr )
{
    INT32                     i4_ret;
    SIZE_T                    z_size;
    VID_PLA_MIN_MAX_INFO_T    t_drv_min_max;
    VID_PLA_MAX_INFO_T        t_drv_max;

    switch( e_get_type )
    {
    case VID_PLA_GET_TYPE_ENHANCE:
        {
            VID_PLA_ENHANCE_T         t_enh;
            
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
                return i4_ret;
            }
            if ( t_drv_min_max.ui4_min_value >= t_drv_min_max.ui4_max_value )
            {
                return SMR_DRV_COMP_ERROR;
            }
            
            SCC_SET_VAL_16( pui2_min, t_drv_min_max.ui4_min_value );
            SCC_SET_VAL_16( pui2_max, t_drv_min_max.ui4_max_value );

            t_enh.e_mode = (VID_PLA_ENHANCE_MODE_T)(UINT32)pv_arg;
            t_enh.ui1_level = 0;
            
            z_size = sizeof(VID_PLA_ENHANCE_T);
            i4_ret = _scc_comp_vid_get( 
                h_vid,
                DRVT_VID_PLANE,
                e_get_type,
                (VOID*)&t_enh,
                &z_size );
            if ( i4_ret != SMR_OK )
            {
                return i4_ret;
            }
            if ( (UINT32)t_enh.ui1_level < t_drv_min_max.ui4_min_value ||
                 (UINT32)t_enh.ui1_level > t_drv_min_max.ui4_max_value )
            {
                return SMR_DRV_COMP_ERROR;
            }
            
            SCC_SET_VAL_16( pui2_curr, t_enh.ui1_level );    
        }
        break;
        
    case VID_PLA_GET_TYPE_BLENDING:
    case VID_PLA_GET_TYPE_BRIGHTNESS:
    case VID_PLA_GET_TYPE_CONTRAST:
    case VID_PLA_GET_TYPE_HUE:
    case VID_PLA_GET_TYPE_SATURATION:
    case VID_PLA_GET_TYPE_SHARPNESS:
    case VID_PLA_GET_TYPE_COLOR_SUPPRESS:
    case VID_PLA_GET_TYPE_H_POSITION:
    case VID_PLA_GET_TYPE_V_POSITION:
    case VID_PLA_GET_TYPE_H_SIZE:
    case VID_PLA_GET_TYPE_V_SIZE:
        {
            UINT16   ui2_value;

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
                return i4_ret;
            }
            if ( t_drv_min_max.ui4_min_value >= t_drv_min_max.ui4_max_value )
            {
                return SMR_DRV_COMP_ERROR;
            }
            
            SCC_SET_VAL_16( pui2_min, t_drv_min_max.ui4_min_value );
            SCC_SET_VAL_16( pui2_max, t_drv_min_max.ui4_max_value );
            
            z_size = sizeof(UINT16);
            i4_ret = _scc_comp_vid_get( 
                h_vid,
                DRVT_VID_PLANE,
                e_get_type,
                (VOID*)&ui2_value,
                &z_size );
            if ( i4_ret != SMR_OK )
            {
                return i4_ret;
            }
            if ( z_size == sizeof(UINT8) )
            {
                ui2_value = (UINT8)ui2_value;
            }
            if ( (UINT32)ui2_value < t_drv_min_max.ui4_min_value ||
                 (UINT32)ui2_value > t_drv_min_max.ui4_max_value )
            {
                return SMR_DRV_COMP_ERROR;
            }
            
            SCC_SET_VAL_16( pui2_curr, ui2_value );
        }
        break;

    case VID_PLA_GET_TYPE_CTI:
    case VID_PLA_GET_TYPE_ETI:
    case VID_PLA_GET_TYPE_NR:
    case VID_PLA_GET_TYPE_3D_NR:
    case VID_PLA_GET_TYPE_BLACK_LVL_EXT:
    case VID_PLA_GET_TYPE_WHITE_PEAK_LMT:
    case VID_PLA_GET_TYPE_FLESH_TONE:
    case VID_PLA_GET_TYPE_LUMA:
    case VID_PLA_GET_TYPE_BLACK_STRETCH:
    case VID_PLA_GET_TYPE_WHITE_STRETCH:
        {
            UINT16   ui2_value;

            t_drv_max.e_get_type = e_get_type;
            z_size = sizeof(VID_PLA_MAX_INFO_T);
            i4_ret = _scc_comp_vid_get( 
                h_vid,
                DRVT_VID_PLANE,
                VID_PLA_GET_TYPE_MAX,
                (VOID*)&t_drv_max,
                &z_size );
            if ( i4_ret != SMR_OK )
            {
                return i4_ret;
            }
            if ( t_drv_max.ui4_max_value > 255 )
            {
                return SMR_DRV_COMP_ERROR;
            }
            
            SCC_SET_VAL_16( pui2_min, 0 );
            SCC_SET_VAL_16( pui2_max, t_drv_max.ui4_max_value );
            
            z_size = sizeof(UINT16);
            i4_ret = _scc_comp_vid_get( 
                h_vid,
                DRVT_VID_PLANE,
                e_get_type,
                (VOID*)&ui2_value,
                &z_size );
            if ( i4_ret != SMR_OK )
            {
                return i4_ret;
            }
            if ( z_size == sizeof(UINT8) )
            {
                ui2_value = (UINT8)ui2_value;
            }
            if ( (UINT32)ui2_value > t_drv_max.ui4_max_value )
            {
                return SMR_DRV_COMP_ERROR;
            }
            
            SCC_SET_VAL_16( pui2_curr, ui2_value );
        }
        break;
        
    default:
        return SMR_NOT_ALLOW;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/

INT32 _scc_comp_vid_open( 
    HANDLE_T            h_vs,
    const CHAR*         s_grp_name,
    HANDLE_T*           ph_comp )
{
    INT32               i4_ret = SMR_OK;
    SCC_VIDEO_COMP_T*   pt_vid_comp = NULL;
    
    if ( h_vs == NULL_HANDLE && !s_grp_name )
    {
        return SMR_INV_ARG;
    }
    if ( !ph_comp )
    {
        return SMR_INV_ARG;
    }
    
    pt_vid_comp = (SCC_VIDEO_COMP_T*)x_mem_alloc( sizeof(SCC_VIDEO_COMP_T) );
    if ( !pt_vid_comp )
    {
        return SMR_INSUFFICIENT_MEMORY;
    }

    x_memset( pt_vid_comp, 0, sizeof(SCC_VIDEO_COMP_T) );
    pt_vid_comp->e_cmd_type = SCC_CMD_TYPE_VIDEO;
    
    if ( h_vs != NULL_HANDLE )
    {
        HANDLE_T      h_drv_vid_pla;
        HANDLE_T      h_drv_dec;
        DRV_TYPE_T    e_dec_drv_type = DRVT_UNKNOWN;
                
        /* Open Video Plane */
        h_drv_vid_pla = _get_vid_pla_from_vs( h_vs );
        if ( h_drv_vid_pla == NULL_HANDLE )
        {
            x_mem_free( pt_vid_comp );
            return SMR_INV_HANDLE;
        }
        
        pt_vid_comp->ah_vid_pla[0] = _scc_open_drv_from_handle( h_drv_vid_pla );
        if ( pt_vid_comp->ah_vid_pla[0] == NULL_HANDLE )
        {
            x_mem_free( pt_vid_comp );
            return SMR_INV_HANDLE;;
        }
        
        /* Open TV Decoder */
        h_drv_dec = _get_dec_from_vs( h_vs, &e_dec_drv_type );
        if ( h_drv_dec != NULL_HANDLE && e_dec_drv_type == DRVT_TV_DEC )
        {
            /* TV Decoder is optional */
            pt_vid_comp->ah_tv_dec[0] = _scc_open_drv_from_handle( h_drv_dec );
            if ( pt_vid_comp->ah_tv_dec[0] == NULL_HANDLE )
            {
                _scc_drv_grp_close( pt_vid_comp->ah_vid_pla, SCC_COMP_MAX_NUM_DRV_COMPS );
                x_mem_free( pt_vid_comp );
                return SMR_INV_HANDLE;;
            }
        }
        else if ( h_drv_dec != NULL_HANDLE && e_dec_drv_type == DRVT_VID_DEC )
        {
            pt_vid_comp->ah_vid_dec[0] = _scc_open_drv_from_handle( h_drv_dec );
            if ( pt_vid_comp->ah_vid_dec[0] == NULL_HANDLE )
            {
                _scc_drv_grp_close( pt_vid_comp->ah_vid_pla, SCC_COMP_MAX_NUM_DRV_COMPS );
                x_mem_free( pt_vid_comp );
                return SMR_INV_HANDLE;;
            }
        }
    }
    else /* use group name */
    {
        SIZE_T    z_size;
        
        x_strcpy( pt_vid_comp->s_grp_name, s_grp_name );
        
        if ( RMR_OK != _scc_open_dummy_rm_pipe( &pt_vid_comp->h_dummy_pipe_tvd ) )
        {
            pt_vid_comp->h_dummy_pipe_tvd = NULL_HANDLE;
        }
        
        z_size = SCC_COMP_MAX_NUM_DRV_COMPS;
        i4_ret = _scc_drv_grp_open_from_name(
            pt_vid_comp->h_dummy_pipe_tvd,
            DRVT_TV_DEC,
            s_grp_name,
            pt_vid_comp->ah_tv_dec,
            &z_size );
        if ( i4_ret != SMR_OK )
        {
            /* Ignore when the system has no TV Decoder */
            _scc_close_dummy_rm_pipe( &pt_vid_comp->h_dummy_pipe_tvd );
        }

        z_size = SCC_COMP_MAX_NUM_DRV_COMPS;
        i4_ret = _scc_drv_grp_open_from_name(
            NULL_HANDLE,
            DRVT_VID_PLANE,
            s_grp_name,
            pt_vid_comp->ah_vid_pla,
            &z_size );
        if ( i4_ret != SMR_OK )
        {
            _scc_drv_grp_close( pt_vid_comp->ah_tv_dec, SCC_COMP_MAX_NUM_DRV_COMPS );
            _scc_close_dummy_rm_pipe( &pt_vid_comp->h_dummy_pipe_tvd );
            x_mem_free( pt_vid_comp );
            return SMR_DRV_COMP_NOT_FOUND;;
        }
    }
    
    _scc_comp_vid_get_vid_pla_cap( pt_vid_comp );
    _scc_comp_vid_get_tv_dec_cap( pt_vid_comp );
    
    i4_ret = handle_alloc( SM_HT_SCC_GRP_NAME,
                           (VOID*)pt_vid_comp,
                           (VOID*)pt_vid_comp->e_cmd_type,
                           &_scc_comp_vid_free_fct,
                           ph_comp );
    if ( i4_ret != HR_OK )
    {
        _scc_drv_grp_close( pt_vid_comp->ah_vid_pla, SCC_COMP_MAX_NUM_DRV_COMPS );
        _scc_drv_grp_close( pt_vid_comp->ah_tv_dec, SCC_COMP_MAX_NUM_DRV_COMPS );
        _scc_drv_grp_close( pt_vid_comp->ah_vid_dec, SCC_COMP_MAX_NUM_DRV_COMPS );
        _scc_close_dummy_rm_pipe( &pt_vid_comp->h_dummy_pipe_tvd );
        x_mem_free( pt_vid_comp );
        return SMR_OUT_OF_HANDLES;
    }
    
    return SMR_OK;
}

INT32 _scc_comp_vid_close( 
    HANDLE_T            h_comp )
{
    INT32               i4_ret = HR_OK;
    HANDLE_TYPE_T       e_type;
    SCC_VIDEO_COMP_T*   pt_vid_comp = NULL;

    i4_ret = handle_get_type_obj( 
        h_comp,
        &e_type,
        (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }
    if ( pt_vid_comp->e_cmd_type != SCC_CMD_TYPE_VIDEO )
    {
        return SMR_INV_HANDLE;
    }
    
    i4_ret = handle_free( h_comp, FALSE );
    if ( i4_ret != HR_OK )
    {
        SM_SLCTR_DBG_INFO(( 
            "[SCC] %s:%d handle_free() fail return %d\r\n",
            __FILE__, __LINE__,
            i4_ret ));
        return SMR_INV_HANDLE;
    }
    
    return SMR_OK;
}


INT32 _scc_comp_vid_set( 
    HANDLE_T            h_comp,
    DRV_TYPE_T          e_drv_type,
    UINT32              e_drv_set_type,
    VOID*               pv_set_info,
    SIZE_T              z_set_size )
{
    INT32               i4_ret;
    INT32               i4_idx;
    HANDLE_TYPE_T       e_type;
    SCC_VIDEO_COMP_T*   pt_vid_comp = NULL;
    HANDLE_T*           ph_drv_array;
    RM_COND_T           e_cond;
    SIZE_T              z_size;
    
    i4_ret = handle_get_type_obj( 
        h_comp,
        &e_type,
        (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }
    if ( pt_vid_comp->e_cmd_type != SCC_CMD_TYPE_VIDEO )
    {
        return SMR_INV_HANDLE;
    }
    
    if ( e_drv_type == DRVT_VID_PLANE )
    {
        ph_drv_array = pt_vid_comp->ah_vid_pla;
    }
    else if ( e_drv_type == DRVT_TV_DEC )
    {
        ph_drv_array = pt_vid_comp->ah_tv_dec;
    }
    else if ( e_drv_type == DRVT_VID_DEC )
    {
#ifdef MW_TV_AV_BYPASS_SUPPORT
        if ( RMR_OK != _scc_open_dummy_rm_pipe( &pt_vid_comp->h_dummy_pipe_vd ) )
#endif
        {
            pt_vid_comp->h_dummy_pipe_vd = NULL_HANDLE;
        }

        z_size = SCC_COMP_MAX_NUM_DRV_COMPS;
        i4_ret = _scc_drv_grp_open_from_name(
            pt_vid_comp->h_dummy_pipe_vd,
            DRVT_VID_DEC,
            pt_vid_comp->s_grp_name,
            pt_vid_comp->ah_vid_dec,
            &z_size );
        if ( i4_ret != SMR_OK )
        {
            _scc_close_dummy_rm_pipe( &pt_vid_comp->h_dummy_pipe_vd );
            return SMR_DRV_COMP_ERROR;
        }

        _scc_comp_vid_get_vid_dec_cap( pt_vid_comp );
        
        ph_drv_array = pt_vid_comp->ah_vid_dec;
    }
    else
    {
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
        if ( i4_ret != RMR_OK )
        {
            if ( e_drv_type == DRVT_VID_DEC )
            {
                _scc_drv_grp_close( pt_vid_comp->ah_vid_dec, SCC_COMP_MAX_NUM_DRV_COMPS );
                _scc_close_dummy_rm_pipe( &pt_vid_comp->h_dummy_pipe_vd );
            }
            SM_SLCTR_DBG_INFO(( 
                "[SCC] %s:%d rm_set() fail return %d\r\n",
                __FILE__, __LINE__,
                i4_ret ));
            return SMR_DRV_COMP_ERROR;
        }
        
        i4_idx++;
    }
    
    if ( e_drv_type == DRVT_VID_DEC )
    {
        _scc_drv_grp_close( pt_vid_comp->ah_vid_dec, SCC_COMP_MAX_NUM_DRV_COMPS );
        _scc_close_dummy_rm_pipe( &pt_vid_comp->h_dummy_pipe_vd );
    }
        
    return SMR_OK;
}

INT32 _scc_comp_vid_get( 
    HANDLE_T            h_comp,
    DRV_TYPE_T          e_drv_type,
    UINT32              e_drv_get_type,
    VOID*               pv_get_info,
    SIZE_T*             pz_get_size )
{
    INT32               i4_ret;
    INT32               i4_idx;
    HANDLE_TYPE_T       e_type;
    SCC_VIDEO_COMP_T*   pt_vid_comp = NULL;
    HANDLE_T*           ph_drv_array;
    RM_COND_T           e_cond;
    SIZE_T              z_size;
    
    i4_ret = handle_get_type_obj( 
        h_comp,
        &e_type,
        (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }
    if ( pt_vid_comp->e_cmd_type != SCC_CMD_TYPE_VIDEO )
    {
        return SMR_INV_HANDLE;
    }
    
    if ( e_drv_type == DRVT_VID_PLANE )
    {
        ph_drv_array = pt_vid_comp->ah_vid_pla;
    }
    else if ( e_drv_type == DRVT_TV_DEC )
    {
        ph_drv_array = pt_vid_comp->ah_tv_dec;
    }
    else if ( e_drv_type == DRVT_VID_DEC )
    {
#ifdef MW_TV_AV_BYPASS_SUPPORT
        if ( RMR_OK != _scc_open_dummy_rm_pipe( &pt_vid_comp->h_dummy_pipe_vd ) )
#endif
        {
            pt_vid_comp->h_dummy_pipe_vd = NULL_HANDLE;
        }

        z_size = SCC_COMP_MAX_NUM_DRV_COMPS;
        i4_ret = _scc_drv_grp_open_from_name(
            pt_vid_comp->h_dummy_pipe_vd,
            DRVT_VID_DEC,
            pt_vid_comp->s_grp_name,
            pt_vid_comp->ah_vid_dec,
            &z_size );
        if ( i4_ret != SMR_OK )
        {
            _scc_close_dummy_rm_pipe( &pt_vid_comp->h_dummy_pipe_vd );
            return SMR_DRV_COMP_ERROR;
        }

        _scc_comp_vid_get_vid_dec_cap( pt_vid_comp );
        
        ph_drv_array = pt_vid_comp->ah_vid_dec;
    }
    else
    {
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
            if ( e_drv_type == DRVT_VID_DEC )
            {
                _scc_drv_grp_close( pt_vid_comp->ah_vid_dec, SCC_COMP_MAX_NUM_DRV_COMPS );
                _scc_close_dummy_rm_pipe( &pt_vid_comp->h_dummy_pipe_vd );
            }
            SM_SLCTR_DBG_INFO(( 
                "[SCC] %s:%d rm_get(%u) fail return %d\r\n",
                __FILE__, __LINE__, (UINT32)e_drv_get_type,
                i4_ret ));
            return SMR_DRV_COMP_ERROR;
        }
        
        i4_idx++;
    }
    
    if ( e_drv_type == DRVT_VID_DEC )
    {
        _scc_drv_grp_close( pt_vid_comp->ah_vid_dec, SCC_COMP_MAX_NUM_DRV_COMPS );
        _scc_close_dummy_rm_pipe( &pt_vid_comp->h_dummy_pipe_vd );
    }
    
    return SMR_OK;
}

INT32 _scc_comp_vid_get_min_max_curr( 
    HANDLE_T                h_vid,
    DRV_TYPE_T              e_drv_type,
    UINT32                  e_get_type,
    VOID*                   pv_arg,
    UINT16*                 pui2_min,
    UINT16*                 pui2_max,
    UINT16*                 pui2_curr )
{
    INT32                     i4_ret;
    
    if ( e_drv_type == DRVT_TV_DEC )
    {
        return SMR_NOT_ALLOW;
    }
    else if ( e_drv_type == DRVT_VID_PLANE )
    {
        i4_ret = _vdp_get_min_max_curr( 
            h_vid,
            e_get_type,
            pv_arg,
            pui2_min,
            pui2_max,
            pui2_curr );
        if ( i4_ret != SMR_OK )
        {
            return SMR_NOT_ALLOW;
        }
    }
    else
    {
        return SMR_NOT_ALLOW;
    }
    
    return SMR_OK;
}

VOID _scc_comp_vid_lb_detect_nfy_fct (
    VOID*                   pv_nfy_tag,
    UINT32                  ui4_src_width,
    UINT32                  ui4_src_height,
    UINT32                  ui4_active_width,
    UINT32                  ui4_active_height,
    UINT32                  ui4_active_x,
    UINT32                  ui4_active_y )
{
    SM_SCC_MSG_T    t_msg;

    t_msg.e_msg_type = SM_SCC_MSG_TYPE_LB_DETECT_NFY;
    t_msg.u.t_vid_lb_detect_nfy.pv_nfy_tag = pv_nfy_tag;
    t_msg.u.t_vid_lb_detect_nfy.ui4_src_width = ui4_src_width;
    t_msg.u.t_vid_lb_detect_nfy.ui4_src_height = ui4_src_height;
    t_msg.u.t_vid_lb_detect_nfy.ui4_active_width = ui4_active_width;
    t_msg.u.t_vid_lb_detect_nfy.ui4_active_height = ui4_active_height;
    t_msg.u.t_vid_lb_detect_nfy.ui4_active_x = ui4_active_x;
    t_msg.u.t_vid_lb_detect_nfy.ui4_active_y = ui4_active_y;

    _scc_send_msg( &t_msg );
}

