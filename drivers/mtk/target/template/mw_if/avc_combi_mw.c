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
 * $RCSfile: avc_combi_mw.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This header file contains specific implementation.
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "x_os.h"
#include "x_rm.h"
#include "x_avc.h"
#include "x_dbg.h"

#include "d_drv_cust_util.h"
#include "avc_combi_mw.h"
#include "tvsrc_mw.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define MAX_NUM_COMBI_COMPS     8
#define MAX_NUM_DRVS_IN_COMBI   8

#define AVC_DBG_STMT( n )    { if ( b_avc_combi_dbg ) {x_dbg_stmt n;} }

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static BOOL ab_is_combi_registered[MAX_NUM_COMBI_COMPS] = 
{
    FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE
};

static BOOL ab_is_combi_connected[MAX_NUM_COMBI_COMPS] = 
{
    FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE
};

static BOOL ab_avc_combi_no_disc_list[MAX_NUM_COMBI_COMPS] = 
{
    FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE
};

static DRV_ID_T at_drv_comps_in_combi[MAX_NUM_COMBI_COMPS][MAX_NUM_DRVS_IN_COMBI];

static UINT32 aui4_drv_comps_sig_sts[MAX_NUM_COMBI_COMPS][MAX_NUM_DRVS_IN_COMBI];

static AVC_NFY_INFO_T at_combi_nfy_info[MAX_NUM_COMBI_COMPS];


static BOOL b_avc_combi_dbg = FALSE;

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/

static CHAR* avc_combi_get_drv_type_str( DRV_TYPE_T e_avc_type )
{
    switch ( e_avc_type )
    {
    case DRVT_AVC_COMP_VIDEO:
        return "DRVT_AVC_COMP_VIDEO";

    case DRVT_AVC_S_VIDEO:
        return "DRVT_AVC_S_VIDEO";

    case DRVT_AVC_Y_PB_PR:
        return "DRVT_AVC_Y_PB_PR";

    case DRVT_AVC_VGA:
        return "DRVT_AVC_VGA";

    case DRVT_AVC_SCART:
        return "DRVT_AVC_SCART";

    case DRVT_AVC_DVI:
        return "DRVT_AVC_DVI";

    case DRVT_AVC_HDMI:
        return "DRVT_AVC_HDMI";

    case DRVT_AVC_AUDIO_INP:
        return "DRVT_AVC_AUDIO_INP";

    case DRVT_AVC_SPDIF:
        return "DRVT_AVC_SPDIF";

    case DRVT_AVC_COMBI:
        return "DRVT_AVC_COMBI";
    
    default:
        return "NONE_AVC_DRV_TYPE";
    }
}

static CHAR* avc_combi_get_sig_sts_str( UINT32 ui4_sig_status )
{
    if ( (ui4_sig_status & AVC_STATE_TV_SIGNAL_UNKNOWN) )
    {
        return "<SIG:TV_UNKNOWN>";
    }    
    else if ( (ui4_sig_status & AVC_STATE_TV_SIGNAL) &&
              (ui4_sig_status & AVC_STATE_AUDIO_SIGNAL) )
    {
        return "<SIG:TV+AUD>";
    }
    else if ( (ui4_sig_status & AVC_STATE_TV_SIGNAL) )
    {
        return "<SIG:TV>";
    }
    else if ( (ui4_sig_status & AVC_STATE_AUDIO_SIGNAL) )
    {
        return "<SIG:AUD>";
    }
    else
    {
        return "<SIG:NONE>";
    }
}

static INT32 _avc_combi_connect(
    DRV_COMP_ID_T*   pt_comp_id,
    DRV_CONN_TYPE_T  e_conn_type,
    const VOID*      pv_conn_info,
    SIZE_T           z_conn_info_len,
    VOID*            pv_tag,
    x_rm_nfy_fct     pf_nfy)
{
    CRIT_STATE_T t_cs;
    UINT16       ui2_idx = 0;
    
    if ( !pt_comp_id                                 || 
         pt_comp_id->e_type!=DRVT_AVC_COMBI          ||
         pt_comp_id->ui2_id >= MAX_NUM_COMBI_COMPS   ||
         !ab_is_combi_registered[pt_comp_id->ui2_id] )
    {
        return RMR_DRV_INV_CONN_INFO;
    }
    
    t_cs = x_crit_start();
    
    at_combi_nfy_info[pt_comp_id->ui2_id].pv_tag = NULL;
    at_combi_nfy_info[pt_comp_id->ui2_id].pf_avc_nfy = NULL;
    ab_is_combi_connected[pt_comp_id->ui2_id] = TRUE;
    ab_avc_combi_no_disc_list[pt_comp_id->ui2_id] = FALSE;
    
    for( ui2_idx = 0; ui2_idx < MAX_NUM_DRVS_IN_COMBI; ui2_idx++ )
    {
        aui4_drv_comps_sig_sts[pt_comp_id->ui2_id][ui2_idx] = AVC_STATE_TV_SIGNAL_UNKNOWN;
    }
    
    x_crit_end(t_cs);
    
    /* TODO: Process connect */
    
    return RMR_OK;
}


static INT32 _avc_combi_disconnect(
    DRV_COMP_ID_T*   pt_comp_id,
    DRV_DISC_TYPE_T  e_disc_type,
    const VOID*      pv_disc_info,
    SIZE_T           z_disc_info_len )
{
    CRIT_STATE_T t_cs;

    if ( !pt_comp_id                                 || 
         pt_comp_id->e_type!=DRVT_AVC_COMBI          ||
         pt_comp_id->ui2_id >= MAX_NUM_COMBI_COMPS   ||
         !ab_is_combi_registered[pt_comp_id->ui2_id] ||
         !ab_is_combi_connected[pt_comp_id->ui2_id] )
    {
        return RMR_DRV_INV_DISC_INFO;
    }
    
    /* TODO: Process disconnect */
    
    t_cs = x_crit_start();
    
    at_combi_nfy_info[pt_comp_id->ui2_id].pv_tag = NULL;
    at_combi_nfy_info[pt_comp_id->ui2_id].pf_avc_nfy = NULL;
    ab_is_combi_connected[pt_comp_id->ui2_id] = FALSE;

    x_crit_end(t_cs);
    
    return RMR_OK;
}

static INT32 _avc_combi_get(
    DRV_COMP_ID_T*  pt_comp_id,
    DRV_GET_TYPE_T  e_get_type,
    VOID*           pv_get_info,
    SIZE_T*         pz_get_info_len )
{
    DRV_ID_T*     pt_drv_id = NULL;
    
    if ( !pt_comp_id                                 || 
         pt_comp_id->e_type!=DRVT_AVC_COMBI          ||
         pt_comp_id->ui2_id >= MAX_NUM_COMBI_COMPS   ||
         !ab_is_combi_registered[pt_comp_id->ui2_id] )
    {
        return RMR_DRV_INV_GET_INFO;
    }

    switch( e_get_type )
    {
    case AVC_GET_TYPE_COMBI_SIGNAL_STATUS:
        {
            AVC_COMBI_SIGNAL_STATUS_T* pv_sig_sts = 
                (AVC_COMBI_SIGNAL_STATUS_T*)pv_get_info;
            
            if ( !pv_sig_sts                                    || 
                 pv_sig_sts->ui2_index >= MAX_NUM_DRVS_IN_COMBI ||
                 !pz_get_info_len )
            {
                return RMR_DRV_INV_GET_INFO;
            }
            if ( *pz_get_info_len < sizeof(UINT32) )
            {
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            
            pt_drv_id = 
                &at_drv_comps_in_combi[pt_comp_id->ui2_id][pv_sig_sts->ui2_index];
            
            /* TODO: Get status from the driver component. */
            pv_sig_sts->ui4_signal_status = 
                aui4_drv_comps_sig_sts[pt_comp_id->ui2_id][pv_sig_sts->ui2_index];
            
            *pz_get_info_len = sizeof(AVC_COMBI_SIGNAL_STATUS_T);
        }
        break;

    case AVC_GET_TYPE_NUM_COMP_IDS:
        {
            UINT16*     pui2_num_comp_id = (UINT16*)pv_get_info;
            UINT16      ui2_index = 0;
            
            if ( !pui2_num_comp_id                 || 
                 !pz_get_info_len )
            {
                return RMR_DRV_INV_GET_INFO;
            }
            if ( *pz_get_info_len < sizeof(UINT16) )
            {
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            
            pt_drv_id = 
                &at_drv_comps_in_combi[pt_comp_id->ui2_id][0];
            
            while( !IS_DRV_ID_END(pt_drv_id) &&
                   ui2_index < MAX_NUM_DRVS_IN_COMBI )
            {
                pt_drv_id = 
                    &at_drv_comps_in_combi[pt_comp_id->ui2_id][++ui2_index];
            }

            (*pui2_num_comp_id) = (UINT16)ui2_index;
        }
        break;

    case AVC_GET_TYPE_COMP_ID:
        {
            AVC_INDEX_COMP_ID_T* pt_idx_comp_id = (AVC_INDEX_COMP_ID_T*)pv_get_info;
            
            if ( !pt_idx_comp_id                                || 
                 !pz_get_info_len                               || 
                 pt_idx_comp_id->ui2_index >= MAX_NUM_DRVS_IN_COMBI )
            {
                return RMR_DRV_INV_GET_INFO;
            }
            if ( *pz_get_info_len < sizeof(AVC_INDEX_COMP_ID_T) )
            {
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            
            pt_drv_id = 
                &at_drv_comps_in_combi[pt_comp_id->ui2_id][pt_idx_comp_id->ui2_index];
            
            if ( IS_DRV_ID_END(pt_drv_id) )
            {
                return RMR_DRV_INV_GET_INFO;
            }
            
            pt_idx_comp_id->t_comp_id.pv_tag         = NULL;
            pt_idx_comp_id->t_comp_id.e_type         = pt_drv_id->e_type;
            pt_idx_comp_id->t_comp_id.ui2_id         = pt_drv_id->ui2_id;
            pt_idx_comp_id->t_comp_id.b_sel_out_port = FALSE;
            pt_idx_comp_id->t_comp_id.u.ui1_inp_port = ALL_PORTS;
        }
        break;

    case AVC_GET_TYPE_NO_CONN_DISC_NFY:
        if ( *pz_get_info_len < sizeof(AVC_NFY_INFO_T) )
        {
            return RMR_DRV_NOT_ENOUGH_SPACE;
        }
        *(BOOL*)pv_get_info = ab_avc_combi_no_disc_list[pt_comp_id->ui2_id];
        break;

    case AVC_GET_TYPE_CONT_COMBI_MON:
        if ( *pz_get_info_len < sizeof(UINT32) )
        {
            return RMR_DRV_NOT_ENOUGH_SPACE;
        }
        *(UINT32*)pv_get_info = 0xFFFFFFFF;
        break;
        
    default:
        return RMR_DRV_INV_GET_INFO;
    }

    return RMR_OK;
}

static INT32 _avc_combi_set(
    DRV_COMP_ID_T*  pt_comp_id,
    DRV_SET_TYPE_T  e_set_type,
    const VOID*     pv_set_info,
    SIZE_T          z_set_info_len )
{
    CRIT_STATE_T t_cs;

    if ( !pt_comp_id                                 || 
         pt_comp_id->e_type != DRVT_AVC_COMBI        ||
         pt_comp_id->ui2_id >= MAX_NUM_COMBI_COMPS   ||
         !ab_is_combi_registered[pt_comp_id->ui2_id] )
    {
        return RMR_DRV_INV_SET_INFO;
    }
    
    switch( e_set_type )
    {
    case AVC_SET_TYPE_NFY_FCT:
        {
            AVC_NFY_INFO_T*   pt_nfy_info = (AVC_NFY_INFO_T*)pv_set_info;
            
            if ( !pt_nfy_info || z_set_info_len < sizeof(AVC_NFY_INFO_T) )
            {
                return RMR_DRV_INV_SET_INFO;
            }
            
            t_cs = x_crit_start();
            
            at_combi_nfy_info[pt_comp_id->ui2_id] = *pt_nfy_info;
        
            x_crit_end(t_cs);
        }
        break;

    case AVC_SET_TYPE_NO_CONN_DISC_NFY:
        if ( z_set_info_len < sizeof(BOOL) )
        {
            return RMR_DRV_NOT_ENOUGH_SPACE;
        }
        ab_avc_combi_no_disc_list[pt_comp_id->ui2_id] = (BOOL)(UINT32)pv_set_info;
        break;        
    
    default:
        return RMR_DRV_INV_SET_INFO;
    }
    
    return RMR_OK;
}

static VOID avc_combi_sig_nfy_from_avc(
    DRV_TYPE_T  e_avc_type,
    UINT16      ui2_avc_id,
    VOID*       pv_nfy_tag,
    UINT32      ui4_sig_status,
    UINT32      ui4_data )
{
    INT32      i4_ret;
    UINT16     ui2_combi_id;
    UINT8      ui1_combi_idx;
    BOOL       b_is_combi_comp;
    
    b_is_combi_comp = avc_combi_is_a_combi_comp(
        e_avc_type,
        ui2_avc_id,
        &ui2_combi_id );

    if ( !b_is_combi_comp )
    {
        return;
    }
    
    if ( ui2_combi_id != (UINT16)(UINT32)pv_nfy_tag ||
         ui2_combi_id >= MAX_NUM_COMBI_COMPS )
    {
        return;
    }
    
    i4_ret = avc_combi_get_comp_idx( 
        ui2_combi_id,
        e_avc_type,
        ui2_avc_id,
        &ui1_combi_idx );
    if ( i4_ret != 0 )
    {
        return;
    }
    
    if ( at_combi_nfy_info[ui2_combi_id].pf_avc_nfy &&
         aui4_drv_comps_sig_sts[ui2_combi_id][ui1_combi_idx] != ui4_sig_status )
    {
        aui4_drv_comps_sig_sts[ui2_combi_id][ui1_combi_idx]
            = ui4_sig_status;
        
        AVC_DBG_STMT(( "[COMBI_%u] Notify %s signal status %s\r\n",
            ui2_combi_id,
            avc_combi_get_drv_type_str(e_avc_type),
            avc_combi_get_sig_sts_str(ui4_sig_status) ));
        
        at_combi_nfy_info[ui2_combi_id].pf_avc_nfy(
            at_combi_nfy_info[ui2_combi_id].pv_tag,
            AVC_COND_COMBI_SIGNAL_STATUS,
            ui4_sig_status,
            (UINT32)ui1_combi_idx );
    }
}

static DRVP_TYPE_T avc_combi_cust_parse(
    const DRV_CUSTOM_T*     pt_drv_cust,
    VOID*                   pv_tag )
{
    INT32     i4_ret;
    SIZE_T    z_num_drvs = 0;
    
    if ( !pt_drv_cust )
    {
        return DRVP_ERROR;
    }
    
    switch( pt_drv_cust->e_custom_type )
    {
    case DRV_CUSTOM_TYPE_AVC_COMBI_REG:
        {
            DRV_REG_T                        t_reg;
            DRV_ID_T*                        pt_drv_comp_iter = NULL;
            DRV_CUSTOM_TYPE_AVC_COMBI_REG_T* pt_combi_reg = 
                (DRV_CUSTOM_TYPE_AVC_COMBI_REG_T*)pt_drv_cust->pv_arg;
            
            if ( !pt_combi_reg || 
                 pt_combi_reg->t_drv_id.e_type != DRVT_AVC_COMBI ||
                 pt_combi_reg->t_drv_id.ui2_id >= MAX_NUM_COMBI_COMPS ||
                 ab_is_combi_registered[pt_combi_reg->t_drv_id.ui2_id]||
                 !pt_combi_reg->pt_combi_drv_ids )
            {
                return DRVP_ERROR;
            }
            
            /* Register COMBI driver */
            x_strcpy( t_reg.s_name, pt_combi_reg->s_name );
            t_reg.ui4_hw_addr = 0;
            t_reg.t_drv_id = pt_combi_reg->t_drv_id;
            t_reg.pt_connection_ids = pt_combi_reg->pt_connection_ids;
            
            i4_ret = d_drv_cust_reg( 
                &t_reg,
                _avc_combi_connect,
                _avc_combi_disconnect,
                _avc_combi_get,
                _avc_combi_set,
                ( AVC_FLAG_DEC_REQ_FOR_TV_SIG_DETECTION   |
                  AVC_FLAG_DEC_REQ_FOR_COLOR_SYS_DETECTION  ) );
            if ( i4_ret != 0 )
            {
                return DRVP_ERROR;
            }
            
            ab_is_combi_registered[pt_combi_reg->t_drv_id.ui2_id] = TRUE;
            
            /* Setup driver components combined by this COMBI */
            z_num_drvs = 0;
            pt_drv_comp_iter = pt_combi_reg->pt_combi_drv_ids;
            while ( z_num_drvs < MAX_NUM_DRVS_IN_COMBI &&
                    !IS_DRV_ID_END( pt_drv_comp_iter ) )
            {
                at_drv_comps_in_combi[pt_combi_reg->t_drv_id.ui2_id][z_num_drvs] =
                    *pt_drv_comp_iter;
                
                /* Setup the driver for signal notify to COMBI */
                MW_AVCSetNotifyOfCOMBI( 
                    pt_drv_comp_iter->e_type,
                    pt_drv_comp_iter->ui2_id,
                    avc_combi_sig_nfy_from_avc,
                    (VOID*)pt_combi_reg->t_drv_id.ui2_id );
                
                z_num_drvs++;
                pt_drv_comp_iter++;
            }
        }
        break;
    
    default:
        /* Ignore unknown types */
        break;
    }
    
    return DRVP_NEXT;
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
INT32 avc_combi_init( 
    const DRV_CUSTOM_DIR_T* pt_drv_cust_dir )
{
    INT32    i4_ret;
    UINT16   ui2_idx = 0;
    UINT16   ui2_comp_id = 0;
    
    if ( !pt_drv_cust_dir )
    {
        /* There is no CMOBI required, ignore COMBI init */
        return 0;
    }
    
    x_memset( at_combi_nfy_info, 
              0, 
              sizeof(AVC_NFY_INFO_T)* MAX_NUM_COMBI_COMPS );
    
    x_memset( at_drv_comps_in_combi, 
              0, 
              sizeof(DRV_ID_T)*MAX_NUM_COMBI_COMPS*MAX_NUM_DRVS_IN_COMBI );

    for( ui2_comp_id = 0; ui2_comp_id < MAX_NUM_COMBI_COMPS; ui2_comp_id++ )
    {
        for( ui2_idx = 0; ui2_idx < MAX_NUM_DRVS_IN_COMBI; ui2_idx++ )
        {
            aui4_drv_comps_sig_sts[ui2_comp_id][ui2_idx] = AVC_STATE_TV_SIGNAL_UNKNOWN;
        }
    }

    i4_ret = d_drv_cust_parse( 
        pt_drv_cust_dir->pt_custom,
        avc_combi_cust_parse,
        NULL );
    if ( i4_ret != 0 )
    {
        return -1;
    }

    return 0;
}

INT32 avc_combi_tv_dec_connect( 
    UINT16 ui2_tv_dec_id,
    UINT16 ui2_combi_id )
{
    DRV_ID_T*     pt_drv_comp_iter = NULL;
    SIZE_T        z_num_drvs = 0;
    
    if ( ui2_combi_id >= MAX_NUM_COMBI_COMPS ||
         !ab_is_combi_registered[ui2_combi_id] )
    {
        return -1;
    }
    
    pt_drv_comp_iter = 
        &at_drv_comps_in_combi[ui2_combi_id][0];

    while ( z_num_drvs < MAX_NUM_DRVS_IN_COMBI &&
            !IS_DRV_ID_END( pt_drv_comp_iter ) )
    {
        /* TODO: connect to a specific AVC component within COMBI.
           and start monitoring signal of the AVC. */
        
        aui4_drv_comps_sig_sts[ui2_combi_id][z_num_drvs] = 
            AVC_STATE_TV_SIGNAL_UNKNOWN;
        
        pt_drv_comp_iter++;
        z_num_drvs++;
    }

    return 0;
}

INT32 avc_combi_tv_dec_disconnect( 
    UINT16 ui2_combi_id )
{
    DRV_ID_T*     pt_drv_comp_iter = NULL;
    SIZE_T        z_num_drvs = 0;
    
    if ( ui2_combi_id >= MAX_NUM_COMBI_COMPS ||
         !ab_is_combi_registered[ui2_combi_id] )
    {
        return -1;
    }
    
    pt_drv_comp_iter = 
        &at_drv_comps_in_combi[ui2_combi_id][0];

    while ( z_num_drvs < MAX_NUM_DRVS_IN_COMBI &&
            !IS_DRV_ID_END( pt_drv_comp_iter ) )
    {
        /* TODO: disconnect to a specific AVC component within COMBI.
           and stop monitoring signal of the AVC. */
        
        aui4_drv_comps_sig_sts[ui2_combi_id][z_num_drvs] = 
            AVC_STATE_TV_SIGNAL_UNKNOWN;

        pt_drv_comp_iter++;
        z_num_drvs++;
    }

    return 0;
}

INT32 avc_combi_get_num_comps( 
    UINT16 ui2_combi_id,
    UINT8* pui1_num_comps )
{
    DRV_ID_T*     pt_drv_comp_iter = NULL;
    SIZE_T        z_num_drvs = 0;
    
    if ( ui2_combi_id >= MAX_NUM_COMBI_COMPS ||
         !ab_is_combi_registered[ui2_combi_id] ||
         !pui1_num_comps )
    {
        return -1;
    }
    
    pt_drv_comp_iter = 
        &at_drv_comps_in_combi[ui2_combi_id][0];

    while ( z_num_drvs < MAX_NUM_DRVS_IN_COMBI &&
            !IS_DRV_ID_END( pt_drv_comp_iter ) )
    {
        pt_drv_comp_iter++;
        z_num_drvs++;
    }
    
    *pui1_num_comps = (UINT8)z_num_drvs;
    
    return 0;
}

INT32 avc_combi_get_comp_by_idx(
    UINT16    ui2_combi_id,
    UINT8     ui1_idx,
    DRV_ID_T* pt_drv_comp )
{
    DRV_ID_T*     pt_drv_comp_iter = NULL;
    
    if ( ui2_combi_id >= MAX_NUM_COMBI_COMPS ||
         !ab_is_combi_registered[ui2_combi_id] ||
         !pt_drv_comp ||
         ui1_idx >= MAX_NUM_DRVS_IN_COMBI )
    {
        return -1;
    }
    
    pt_drv_comp_iter = 
        &at_drv_comps_in_combi[ui2_combi_id][ui1_idx];
    
    if ( IS_DRV_ID_END( pt_drv_comp_iter ) )
    {
        return -1;
    }
    
    *pt_drv_comp = *pt_drv_comp_iter;
    
    return 0;
}

INT32 avc_combi_get_default_comp(
    UINT16    ui2_combi_id,
    DRV_ID_T* pt_drv_comp )
{
    DRV_ID_T*     pt_drv_comp_iter = NULL;
    SIZE_T        z_num_drvs = 0;
    
    if ( ui2_combi_id >= MAX_NUM_COMBI_COMPS ||
         !ab_is_combi_registered[ui2_combi_id] ||
         !pt_drv_comp )
    {
        return -1;
    }
    
    pt_drv_comp_iter = 
        &at_drv_comps_in_combi[ui2_combi_id][0];

    while ( z_num_drvs < MAX_NUM_DRVS_IN_COMBI &&
            !IS_DRV_ID_END( pt_drv_comp_iter ) )
    {
        if ( pt_drv_comp_iter->e_type == DRVT_AVC_COMP_VIDEO )
        {
            *pt_drv_comp = *pt_drv_comp_iter;
            return 0;
        }
        
        pt_drv_comp_iter++;
        z_num_drvs++;
    }
    
    *pt_drv_comp = at_drv_comps_in_combi[ui2_combi_id][0];
    
    return 0;
}

INT32 avc_combi_get_comp_idx(
    UINT16      ui2_combi_id,
    DRV_TYPE_T  e_avc_type,
    UINT16      ui2_avc_id,
    UINT8*      pui1_comp_idx )
{
    INT32       i4_ret;
    DRV_ID_T    t_drv_comp_iter;
    UINT8       ui1_comp_idx;

    if ( ui2_combi_id >= MAX_NUM_COMBI_COMPS ||
         !ab_is_combi_registered[ui2_combi_id] ||
         !pui1_comp_idx )
    {
        return -1;
    }
    
    for( ui1_comp_idx = 0; 
         ui1_comp_idx < MAX_NUM_DRVS_IN_COMBI; 
         ui1_comp_idx++ )
    {
        i4_ret = avc_combi_get_comp_by_idx( 
            ui2_combi_id, 
            ui1_comp_idx,
            &t_drv_comp_iter );
        if ( i4_ret != 0 )
        {
            continue;
        }
        
        if ( t_drv_comp_iter.e_type == e_avc_type &&
             t_drv_comp_iter.ui2_id == ui2_avc_id )
        {
            *pui1_comp_idx = ui1_comp_idx;
            return 0;
        }
    }
    
    return -1;
}

BOOL avc_combi_is_a_combi_comp(
    DRV_TYPE_T  e_avc_type,
    UINT16      ui2_avc_id,
    UINT16*     pui2_combi_id )
{
    INT32       i4_ret;
    UINT16      ui2_combi_id = 0;
    
    for( ui2_combi_id = 0; ui2_combi_id < MAX_NUM_COMBI_COMPS; ui2_combi_id++ )
    {
        DRV_ID_T    t_drv_comp;
        UINT8       ui1_num_comps = 0;
        UINT8       ui1_comp_idx = 0;
        
        i4_ret = avc_combi_get_num_comps( 
            ui2_combi_id,
            &ui1_num_comps );
        if ( i4_ret != 0 )
        {
            continue;
        }
        
        for( ui1_comp_idx = 0; ui1_comp_idx < ui1_num_comps; ui1_comp_idx++ )
        {
            i4_ret = avc_combi_get_comp_by_idx( 
                ui2_combi_id, 
                ui1_comp_idx,
                &t_drv_comp );
            if ( i4_ret != 0 )
            {
                continue;
            }
            
            if ( t_drv_comp.e_type == e_avc_type &&
                 t_drv_comp.ui2_id == ui2_avc_id )
            {
                if ( pui2_combi_id )
                {
                    *pui2_combi_id = ui2_combi_id;
                }
                return TRUE;
            }
        }
        
    }
    
    return FALSE;
}


INT32 avc_combi_get_monitor_comp(
    UINT16    ui2_combi_id,
    DRV_ID_T* pt_drv_comp )
{
    DRV_ID_T*     pt_drv_comp_iter = NULL;
    SIZE_T        z_num_drvs = 0;
    DRV_ID_T      t_dft_comp;
    
    if ( ui2_combi_id >= MAX_NUM_COMBI_COMPS ||
         !ab_is_combi_registered[ui2_combi_id] ||
         !pt_drv_comp )
    {
        return -1;
    }


    if(avc_combi_get_default_comp(ui2_combi_id, &t_dft_comp) != 0)
    {
    	return -2;
    }

    pt_drv_comp_iter = 
        &at_drv_comps_in_combi[ui2_combi_id][0];

    while ( z_num_drvs < MAX_NUM_DRVS_IN_COMBI &&
            !IS_DRV_ID_END( pt_drv_comp_iter ) )
    {
        if ( (pt_drv_comp_iter->e_type != t_dft_comp.e_type) || (pt_drv_comp_iter->ui2_id != t_dft_comp.ui2_id))
        {
            *pt_drv_comp = *pt_drv_comp_iter;
            return 0;
        }
        
        pt_drv_comp_iter++;
        z_num_drvs++;
    }
    
    *pt_drv_comp = at_drv_comps_in_combi[ui2_combi_id][0];
    
    return -3;
}









