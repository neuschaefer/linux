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
 * $RCSfile: sm_drv_comp.c,v $
 * $Revision: #2 $
 * $Date: 2012/07/20 $
 * $Author: xingju.wang $
 * $CCRevision: /main/DTV_X_HQ_int/11 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: fd0bf10172342e0c16f2562fa2e3c9c5 $
 *
 * Description:
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "res_mngr/x_rm_dev_types.h"

#include "strm_mngr/x_sm.h"
#include "strm_mngr/_sm_utils.h"
#include "strm_mngr/sm_drv_map.h"
#include "strm_mngr/sm_drv_comp.h"

#include "strm_mngr/scc/x_scc.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
BOOL                        gb_not_disable_tve_as_stop;
BOOL                        gb_enable_tve_as_play;
const SM_DRV_COMP_DESCR_T   t_sm_drv_comp_initor = SM_DRV_COMP_DESCR_INIT;

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/

static INT32 _sm_drv_comp_open_port( SM_DRV_COMP_DESCR_T*    pt_comp,
                                     VOID*                   pv_tag )
{
    INT32 i4_ret;

    pt_comp->e_comp_port_cond_org = pt_comp->e_comp_port_cond;

    i4_ret = rm_open_port( pt_comp->h_comp,
                           pt_comp->t_info.ui1_in_port,
                           (VOID*)pv_tag,
                           &pt_comp->h_comp_port,
                           &pt_comp->e_comp_port_cond );
    if ( i4_ret != RMR_OK )
    {
        return SMR_DRV_COMP_ERROR;
    }

    return SMR_OK;
}

static INT32 _sm_drv_comp_connect_port( SM_DRV_COMP_DESCR_T*    pt_from_comp,
                                        HANDLE_T                h_to_comp,
                                        VOID*                   pv_tag )
{
    INT32 i4_ret;

    pt_from_comp->e_comp_port_cond_org = pt_from_comp->e_comp_port_cond;

    if (pt_from_comp->t_info.e_type == DRVT_PLA_MXR )
    {
         x_scc_disp_pmx_lock();
    }
    i4_ret = rm_connect( pt_from_comp->h_comp_port,
                         RM_CONN_TYPE_HANDLE,
                         (VOID*)h_to_comp,
                         sizeof(HANDLE_T),
                         (VOID*)pv_tag,
                         &pt_from_comp->e_comp_port_cond );
    if (pt_from_comp->t_info.e_type == DRVT_PLA_MXR )
    {
         x_scc_disp_pmx_unlock();
    }
    if ( i4_ret == RMR_ASYNC_NFY )
    {
        return SMR_ASYNC_NFY;
    }
    else if ( i4_ret != RMR_OK && i4_ret != RMR_HARD_WIRED )
    {
        return SMR_DRV_COMP_ERROR;
    }

    return SMR_OK;
}

static INT32 _sm_drv_comp_disconnect_port( SM_DRV_COMP_DESCR_T* pt_from_comp )
{
    INT32 i4_ret;

    if ( sm_rm_cond_filter( pt_from_comp->e_comp_port_cond,
                            COND_GRP_CONN_STATE ) == RM_COND_HARD_WIRED )
    {
        return SMR_OK;
    }

    pt_from_comp->e_comp_port_cond_org = pt_from_comp->e_comp_port_cond;

    if (pt_from_comp->t_info.e_type == DRVT_PLA_MXR )
    {
         x_scc_disp_pmx_lock();
    }
    i4_ret = rm_disconnect( pt_from_comp->h_comp_port,
                            RM_DISC_TYPE_IGNORE,
                            (VOID*)NULL,
                            0,
                            &pt_from_comp->e_comp_port_cond );
    if (pt_from_comp->t_info.e_type == DRVT_PLA_MXR )
    {
         x_scc_disp_pmx_unlock();
    }
    if ( i4_ret == RMR_ASYNC_NFY )
    {
        return SMR_ASYNC_NFY;
    }
    if ( i4_ret != RMR_OK )
    {
        return SMR_DRV_COMP_ERROR;
    }

    return SMR_OK;
}

static INT32 _sm_drv_comp_multi_ports_sel_first(
                    SM_DRV_COMP_DESCR_T* pt_from_comp,
                    HANDLE_T             h_to_comp,
                    VOID*                pv_tag )
{
    INT32            i4_ret;
    UINT32           ui4_i = 0;
    UINT8            ui1_port = 0;
    UINT16           ui2_num_entries;
    CONN_STATE_T     at_conn_state[MAX_NUM_PORTS];
    CONN_DIR_TYPE_T  at_conn_dir[MAX_NUM_PORTS];

    i4_ret = rm_get_num_conn_states( pt_from_comp->h_comp,
                                     h_to_comp,
                                     &ui2_num_entries );
    if ( i4_ret != RMR_OK || ui2_num_entries > MAX_NUM_PORTS )
    {
        return SMR_DRV_COMP_ERROR;
    }

    i4_ret = rm_get_conn_state( pt_from_comp->h_comp,
                                h_to_comp,
                                at_conn_state,
                                &ui2_num_entries,
                                at_conn_dir );
    if ( i4_ret != RMR_OK || ui2_num_entries >= MAX_NUM_PORTS )
    {
        return SMR_DRV_COMP_ERROR;
    }

    for( ui4_i = 0; ui4_i < ui2_num_entries; ++ui4_i )
    {
        if ( at_conn_state[ui4_i].ui1_conn_state != CONN_STATE_DISCONNECTED &&
             at_conn_state[ui4_i].ui1_conn_state != CONN_STATE_HARD_WIRED )
        {
            continue;
        }
        ui1_port = at_conn_state[ui4_i].ui1_port_1;
        pt_from_comp->t_info.ui1_in_port = ui1_port;

        i4_ret = _sm_drv_comp_open_port( pt_from_comp, pv_tag );
        if ( i4_ret != SMR_OK )
        {
            continue;
        }

        i4_ret = _sm_drv_comp_connect_port( pt_from_comp,
                                            h_to_comp,
                                            pv_tag );
        if ( i4_ret != SMR_OK )
        {
            i4_ret = rm_close( pt_from_comp->h_comp_port,
                               TRUE,
                               &pt_from_comp->e_comp_port_cond );
            if ( i4_ret != RMR_OK )
            {
                return SMR_DRV_COMP_ERROR;
            }

            /* check condition */
            pt_from_comp->h_comp_port = NULL_HANDLE;
            pt_from_comp->t_info.ui1_in_port = ANY_PORT_NUM;
            continue;
        }

        break; /* break if any port is sussessfully opened and connected */
    }

    if ( pt_from_comp->h_comp_port == NULL_HANDLE ||
         pt_from_comp->t_info.ui1_in_port == ANY_PORT_NUM )
    {
        return SMR_DRV_COMP_ERROR;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/

INT32 sm_drv_comp_close_ex( SM_DRV_COMP_DESCR_T*        pt_comp )
{
    INT32               i4_ret;

    if ( !pt_comp )
    {
        SM_SLCTR_DBG_ERR(( "[SM] %s:%d, ERROR! NULL drver component "
                           "descriptor.\r\n",
                           __FILE__, __LINE__ ));
        return SMR_INV_ARG;
    }
    if ( pt_comp->h_comp_port == NULL_HANDLE &&
         pt_comp->h_comp_clone == NULL_HANDLE &&
         pt_comp->h_comp == NULL_HANDLE )
    {
        return SMR_OK;
    }

    if ( pt_comp->h_comp_port != NULL_HANDLE )
    {
        pt_comp->e_comp_port_cond_org = pt_comp->e_comp_port_cond;

        i4_ret = rm_close( pt_comp->h_comp_port,
                           TRUE,
                           &pt_comp->e_comp_port_cond );
        if ( i4_ret != RMR_OK && i4_ret != RMR_INV_HANDLE )
        {
            SM_SLCTR_DBG_ERR(( "[SM] %s:%d, ERROR! rm_close() fail return %d\r\n",
                               __FILE__, __LINE__, i4_ret ));
            SM_ABORT( SM_DBG_ABRT_CANNOT_CLOSE_DRIVER_COMP );
        }

        pt_comp->h_comp_port = NULL_HANDLE;
    }
    if ( pt_comp->h_comp_clone != NULL_HANDLE )
    {
        i4_ret = handle_free( pt_comp->h_comp_clone, FALSE );
        if ( i4_ret != HR_OK && i4_ret != HR_INV_HANDLE )
        {
            SM_SLCTR_DBG_ERR(( "[SM] %s:%d, ERROR! handle_free() fail return %d\r\n",
                               __FILE__, __LINE__, i4_ret ));
        }
        pt_comp->h_comp_clone = NULL_HANDLE;
    }
    if ( pt_comp->h_comp != NULL_HANDLE )
    {
        pt_comp->e_comp_cond_org = pt_comp->e_comp_cond;

        if ( pt_comp->e_comp_cond != RM_COND_CLOSED )
        {
            i4_ret = rm_close( pt_comp->h_comp,
                               TRUE,
                               &pt_comp->e_comp_cond );
            if ( i4_ret != RMR_OK && i4_ret != RMR_INV_HANDLE )
            {
                SM_SLCTR_DBG_ERR(( "[SM] %s:%d, ERROR! rm_close() fail return %d\r\n",
                                   __FILE__, __LINE__, i4_ret ));
                SM_ABORT( SM_DBG_ABRT_CANNOT_CLOSE_DRIVER_COMP );
            }
        }
        pt_comp->h_comp = NULL_HANDLE;
    }

    return SMR_OK;
}

INT32 sm_drv_comp_close( SM_DRV_COMP_DESCR_T*        pt_comp )
{
    INT32               i4_ret;

    if ( !pt_comp )
    {
        return SMR_INV_ARG;
    }
    if ( pt_comp->h_comp_port == NULL_HANDLE &&
         pt_comp->h_comp_clone == NULL_HANDLE &&
         pt_comp->h_comp == NULL_HANDLE )
    {
        return SMR_OK;
    }

    sm_drv_map_lock();

    i4_ret = sm_drv_comp_close_ex( pt_comp );
    if ( i4_ret != SMR_OK )
    {
    }

    i4_ret = sm_drv_map_del( &pt_comp->t_info );
    if ( i4_ret < SMDMR_OK )
    {
        SM_SLCTR_DBG_ERR(( "[SM] ERROR! sm_drv_map_del() fail return %d\r\n",
                           i4_ret ));
    }

    sm_drv_map_unlock();

    return SMR_OK;
}

INT32 sm_drv_comp_connect( SM_DRV_COMP_DESCR_T*  pt_from_comp,
                           HANDLE_T              h_to_comp,
                           VOID*                 pv_tag )
{
    INT32 i4_ret;

    if ( pt_from_comp->h_comp_port != NULL_HANDLE )
    {
        return _sm_drv_comp_connect_port( pt_from_comp,
                                          h_to_comp,
                                          pv_tag );
    }
    else
    {
        pt_from_comp->e_comp_cond_org = pt_from_comp->e_comp_cond;

        if (pt_from_comp->t_info.e_type == DRVT_PLA_MXR )
        {
             x_scc_disp_pmx_lock();
        }
        i4_ret = rm_connect( pt_from_comp->h_comp,
                             RM_CONN_TYPE_HANDLE,
                             (VOID*)h_to_comp,
                             sizeof(HANDLE_T),
                             (VOID*)pv_tag,
                             &pt_from_comp->e_comp_cond );
        if (pt_from_comp->t_info.e_type == DRVT_PLA_MXR )
        {
             x_scc_disp_pmx_unlock();
        }
        if ( i4_ret == RMR_MULTI_INP_PORT_COMP )
        {
            SM_DRV_COMP_INFO_T    t_old_info;

            t_old_info = pt_from_comp->t_info;

            sm_drv_map_lock();

            /* If there is multiple ports, resolve it */
            i4_ret = _sm_drv_comp_multi_ports_sel_first( pt_from_comp,
                                                         h_to_comp,
                                                         pv_tag );
            if ( i4_ret == SMR_OK )
            {
#if (defined(MW_MM_DUAL_VID_SUPPORT) && !(defined(MW_MM_FAKE_DAUL_VID))) 
                i4_ret = sm_drv_map_del( &t_old_info );
                if ( i4_ret == SMR_OK || i4_ret == SMDMR_DRV_NOT_EXIST )
                {
                    i4_ret = sm_drv_map_add( &pt_from_comp->t_info );
                    if ( i4_ret == SMR_OK || i4_ret == SMDMR_DRV_ALREADY_EXIST )
                    {
                         i4_ret = SMR_OK;
                    }
                }
#else
                i4_ret = sm_drv_map_del( &t_old_info );
                if ( i4_ret == SMR_OK )
                {
                    i4_ret = sm_drv_map_add( &pt_from_comp->t_info );
                    if ( i4_ret == SMR_OK )
                    {
                    }
                }
#endif
            }

            sm_drv_map_unlock();

            return i4_ret;
        }
        else if ( i4_ret == RMR_ASYNC_NFY )
        {
            return SMR_ASYNC_NFY;
        }
        else if ( i4_ret != RMR_OK && i4_ret != RMR_HARD_WIRED )
        {
            return SMR_DRV_COMP_ERROR;
        }
    }

    return SMR_OK;
}

INT32 sm_drv_comp_disconnect( SM_DRV_COMP_DESCR_T* pt_from_comp )
{
    INT32 i4_ret;

    if ( sm_rm_cond_filter( pt_from_comp->e_comp_cond,
                            COND_GRP_CONN_STATE ) == RM_COND_HARD_WIRED )
    {
        return SMR_OK;
    }

    if ( pt_from_comp->h_comp_port != NULL_HANDLE )
    {
        return _sm_drv_comp_disconnect_port( pt_from_comp );
    }
    else
    {
        pt_from_comp->e_comp_cond_org = pt_from_comp->e_comp_cond;

        if (pt_from_comp->t_info.e_type == DRVT_PLA_MXR )
        {
             x_scc_disp_pmx_lock();
        }
        i4_ret = rm_disconnect( pt_from_comp->h_comp,
                                RM_DISC_TYPE_IGNORE,
                                (VOID*)NULL,
                                0,
                                &pt_from_comp->e_comp_cond );
        if (pt_from_comp->t_info.e_type == DRVT_PLA_MXR )
        {
             x_scc_disp_pmx_unlock();
        }
        if ( i4_ret == RMR_INV_HANDLE )
        {
            SM_SLCTR_DBG_ERR((
                "[SM] rm_disconnect(0x%.8x) fail, return %d!\r\n",
                (UINT32)pt_from_comp->h_comp, i4_ret ));
            pt_from_comp->h_comp = NULL_HANDLE;
            return SMR_INV_HANDLE;
        }
        else if ( i4_ret == RMR_ASYNC_NFY )
        {
            return SMR_ASYNC_NFY;
        }
        else if ( i4_ret != RMR_OK )
        {
            SM_SLCTR_DBG_ERR((
                "[SM] rm_disconnect() fail, return %d!\r\n",
                i4_ret ));
            return SMR_DRV_COMP_ERROR;
        }
    }

    return SMR_OK;
}

INT32 sm_drv_comp_get_info( SM_DRV_COMP_DESCR_T*    pt_comp )
{
    INT32 i4_ret;

    if ( !pt_comp || pt_comp->h_comp == NULL_HANDLE )
    {
        return SMR_INV_ARG;
    }

    i4_ret = rm_get_comp_info_from_handle( pt_comp->h_comp,
                                           &pt_comp->t_info.e_type,
                                           &pt_comp->t_info.ui2_comp_id,
                                           &pt_comp->t_info.ui1_num_in_port,
                                           &pt_comp->t_info.ui1_num_out_port,
                                           &pt_comp->t_info.ui4_comp_flags );
    if ( i4_ret != RMR_OK )
    {
        return SMR_INTERNAL_ERROR;
    }

    return SMR_OK;
}

INT32 sm_drv_comp_open( HANDLE_T                    h_source,
                        DRV_TYPE_T                  e_drv_type,
                        UINT16                      ui2_comp_id,
                        const CHAR*                 ps_name,
                        VOID*                       pv_sel_data,
                        rm_comp_sel_fct             pf_sel_fct,
                        VOID*                       pv_nfy_tag,
                        rm_nfy_fct                  pf_nfy,
                        SM_DRV_COMP_DESCR_T*        pt_comp )
{
    INT32               i4_ret;

    if ( !pt_comp )
    {
        return SMR_INV_ARG;
    }

    sm_drv_map_lock();

    i4_ret = sm_drv_comp_open_ex(
        h_source,
        e_drv_type,
        ui2_comp_id,
        ps_name,
        pv_sel_data,
        pf_sel_fct,
        pv_nfy_tag,
        pf_nfy,
        pt_comp );
    if ( i4_ret != SMR_OK )
    {
        sm_drv_map_unlock();
        return i4_ret;
    }

    /* add driver to driver map */
    i4_ret = sm_drv_map_add( &pt_comp->t_info );
    if ( i4_ret != SMDMR_OK && i4_ret !=SMDMR_DRV_ALREADY_EXIST)
    {
        sm_drv_comp_close( pt_comp );

        sm_drv_map_unlock();
        return ( ( i4_ret==SMDMR_DRV_ALREADY_EXIST )?
                 SMR_DRV_ALREADY_USED_BY_OTHER_STRM:SMR_DRV_COMP_ERROR );
    }

    sm_drv_map_unlock();

    return SMR_OK;
}

INT32 sm_drv_comp_open_ex(
    HANDLE_T                    h_source,
    DRV_TYPE_T                  e_drv_type,
    UINT16                      ui2_comp_id,
    const CHAR*                 ps_name,
    VOID*                       pv_sel_data,
    rm_comp_sel_fct             pf_sel_fct,
    VOID*                       pv_nfy_tag,
    rm_nfy_fct                  pf_nfy,
    SM_DRV_COMP_DESCR_T*        pt_comp )
{
    INT32               i4_ret;
    HANDLE_T            h_comp = NULL_HANDLE;
    RM_COND_T           e_comp_cond = RM_COND_IGNORE;
#ifdef MW_TV_AV_BYPASS_SUPPORT
    HANDLE_TYPE_T       e_hdl_type;
#endif

    if ( !pt_comp )
    {
        return SMR_INV_ARG;
    }

    pt_comp->e_comp_cond_org = pt_comp->e_comp_cond;

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if ( h_source != NULL_HANDLE )
    {
        i4_ret = x_handle_get_type( h_source, &e_hdl_type);
        if ( i4_ret != HR_OK )
        {
            return SMR_INV_ARG;
        }
    }
#endif

    i4_ret = rm_open_comp( h_source,
#ifdef MW_TV_AV_BYPASS_SUPPORT
                           ((h_source!=NULL_HANDLE && e_hdl_type!=RMT_PIPE)?OBJ_COMP_REL_SRC:OBJ_COMP_REL_IGNORE),
#else
                           ((h_source!=NULL_HANDLE)?OBJ_COMP_REL_SRC:OBJ_COMP_REL_IGNORE),
#endif
                           e_drv_type,
                           ui2_comp_id,
                           ps_name,
#ifdef MW_TV_AV_BYPASS_SUPPORT
                           ((e_drv_type==DRVT_VID_DEC || e_drv_type==DRVT_TV_DEC)?CTRL_TYPE_MASTER_SLAVE:CTRL_TYPE_SHARED),
#else
                           ((e_drv_type==DRVT_TV_DEC)?CTRL_TYPE_MASTER_SLAVE:CTRL_TYPE_SHARED),
#endif
                           pv_sel_data, /*pv_comp_sel_data*/
                           pf_sel_fct,
                           pv_nfy_tag, /*pv_tag*/
                           pf_nfy,
                           &h_comp,
                           &e_comp_cond );
    if ( i4_ret != RMR_OK )
    {
        if ( i4_ret == RMR_NO_COMP_FOUND )
        {
            return SMR_DRV_COMP_NOT_FOUND;
        }
        return SMR_INTERNAL_ERROR;
    }

    pt_comp->h_comp = h_comp;
    pt_comp->e_comp_cond = e_comp_cond;

    /* close the component, if multicomponent is not resolved in
       select callback funtion */
    if ( sm_rm_cond_filter( pt_comp->e_comp_cond, COND_GRP_REASON ) ==
         RM_COND_REASON_MULTI_COMPS )
    {
        sm_drv_comp_close_ex( pt_comp );
        return SMR_INTERNAL_ERROR;
    }

    /* Retrive all driver information */
    i4_ret = sm_drv_comp_get_info( pt_comp );
    if ( i4_ret != SMR_OK )
    {
        sm_drv_comp_close_ex( pt_comp );
        return SMR_DRV_COMP_ERROR;
    }

    /* open port handle if input port exists */
    if ( pt_comp->t_info.ui1_in_port != ANY_PORT_NUM &&
         pt_comp->t_info.ui1_num_in_port > 1 )
    {
        i4_ret = _sm_drv_comp_open_port( pt_comp, pv_nfy_tag );
        if ( i4_ret != SMR_OK )
        {
            sm_drv_comp_close_ex( pt_comp );
            return SMR_DRV_COMP_ERROR;
        }
    }

    /* Clone the driver handle */
    pt_comp->h_comp_clone = sm_drv_hdl_clone( pt_comp->h_comp );
    if ( pt_comp->h_comp_clone == NULL_HANDLE )
    {
        sm_drv_comp_close_ex( pt_comp );
        return SMR_OUT_OF_HANDLES;
    }

    return SMR_OK;
}

SELECT_T sm_drv_comp_sel_unused_comp_fct( DRV_TYPE_T   e_type,
                                          UINT16       ui2_id,
                                          VOID*        pv_comp_sel_data,
                                          const VOID*  pv_comp_data,
                                          SIZE_T       z_comp_data_len,
                                          UINT16       ui2_num_active,
                                          UINT16       ui2_num_wait,
                                          CTRL_TYPE_T  e_ctrl,
                                          BOOL         b_would_conflict)
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

