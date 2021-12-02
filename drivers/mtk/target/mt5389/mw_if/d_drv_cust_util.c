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
 * $RCSfile: d_drv_cust_util.c,v $
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
#include "d_drv_cust_util.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define MAX_NUM_SINK_COMPS   16

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/
static VOID  _cust_drv_id_2_drv_comp_reg( 
    const DRV_ID_T*           pt_drv_id,
    DRV_COMP_REG_T*           pt_drv_reg )
{
    if ( !pt_drv_id || !pt_drv_reg )
    {
        return;
    }
    
    pt_drv_reg->e_id_type = ID_TYPE_IND;
    pt_drv_reg->e_type = pt_drv_id->e_type;
    pt_drv_reg->u.t_ind.ui2_id = pt_drv_id->ui2_id;
    pt_drv_reg->u.t_ind.ui1_port = ALL_PORTS;
    pt_drv_reg->u.t_ind.pv_tag = NULL;
}

static INT32 _cust_drv_ids_2_drv_comp_regs(
    const DRV_ID_T*     pt_drv_ids,
    DRV_COMP_REG_T*     pt_reg_comp_ids,
    SIZE_T*             pz_num_entries )
{
    DRV_ID_T*           pt_drv_id_iter = (DRV_ID_T*)pt_drv_ids;
    SIZE_T              z_num_entries = 0;
    
    if ( !pt_drv_ids || !pt_reg_comp_ids || !pz_num_entries )
    {
        return -1;
    }
    
    while( pt_drv_id_iter &&
           !IS_DRV_ID_END(pt_drv_id_iter) )
    {
        _cust_drv_id_2_drv_comp_reg( 
            pt_drv_id_iter,
            &pt_reg_comp_ids[z_num_entries] );
        
        z_num_entries++;
        pt_drv_id_iter++;
        
        if ( z_num_entries >= *pz_num_entries )
        {
            return -1;
        }
    }
    
    *pz_num_entries = z_num_entries;
    
    return 0;
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
DRV_CUSTOM_DIR_T* d_drv_cust_get_dir_by_type( 
    DRV_TYPE_T              e_first_drv_type,
    DRV_TYPE_T              e_last_drv_type,
    const DRV_CUSTOM_DIR_T* pt_drv_cust_dirs )
{
    DRV_CUSTOM_DIR_T*    pt_drv_cust_dir = (DRV_CUSTOM_DIR_T*)pt_drv_cust_dirs;
    
    if ( !pt_drv_cust_dirs )
    {
        return NULL;
    }
    
    while ( !IS_DRV_CUSTOM_DIR_END(pt_drv_cust_dir) )
    {
        if ( pt_drv_cust_dir->e_first_type == e_first_drv_type &&
             pt_drv_cust_dir->e_last_type == e_last_drv_type )
        {
            return pt_drv_cust_dir;
        }
        pt_drv_cust_dir++;
    }
    
    return NULL;
}

INT32 d_drv_cust_reg_ex( 
    const DRV_REG_T*        pt_reg,
    x_rm_connect_fct        pf_connect,
    x_rm_disconnect_fct     pf_disconnect,
    x_rm_get_fct            pf_get,
    x_rm_set_fct            pf_set,
    UINT32                  u4_flags,
    CONN_DIR_TYPE_T         e_conn_dir )
{
    DRV_COMP_REG_T      at_reg_comp_ids[MAX_NUM_SINK_COMPS];
    DRV_COMP_REG_T      t_src_comp_reg;
    DRV_COMP_FCT_TBL_T  t_src_fct_tbl;
    SIZE_T              z_num_entries = 0;
    INT32               i4_ret;

    if ( !pt_reg )
    {
        return -1;
    }

    /* Set function table */
    t_src_fct_tbl.pf_rm_connect     = pf_connect;
    t_src_fct_tbl.pf_rm_disconnect  = pf_disconnect;
    t_src_fct_tbl.pf_rm_get         = pf_get;
    t_src_fct_tbl.pf_rm_set         = pf_set;

     /* Setup component ID */
    t_src_comp_reg.e_type           = pt_reg->t_drv_id.e_type;
    t_src_comp_reg.e_id_type        = ID_TYPE_IND;
    t_src_comp_reg.u.t_ind.ui2_id   = pt_reg->t_drv_id.ui2_id;
    t_src_comp_reg.u.t_ind.ui1_port = ALL_PORTS;
    t_src_comp_reg.u.t_ind.pv_tag   = NULL;
    
    i4_ret = x_rm_reg_comp(
        &t_src_comp_reg,
        1,
        1,
        pt_reg->s_name,
        u4_flags,
        &t_src_fct_tbl,
        NULL,
        0);
    if ( i4_ret != RMR_OK )
    {
        return -1;
    }
    
    z_num_entries = MAX_NUM_SINK_COMPS;
    i4_ret = _cust_drv_ids_2_drv_comp_regs(
        pt_reg->pt_connection_ids,
        at_reg_comp_ids,
        &z_num_entries );
    if ( i4_ret != 0 )
    {
        return -1;
    }
    
    if ( z_num_entries > 0 )
    {
        i4_ret = x_rm_reg_conn_list( 
            &t_src_comp_reg, 
            at_reg_comp_ids, 
            z_num_entries, 
            e_conn_dir, 
            FALSE );
        if (i4_ret != RMR_OK)
        {
            return -1;
        }
    }
    
    return RMR_OK;
}


INT32 d_drv_cust_reg( 
    const DRV_REG_T*        pt_reg,
    x_rm_connect_fct        pf_connect,
    x_rm_disconnect_fct     pf_disconnect,
    x_rm_get_fct            pf_get,
    x_rm_set_fct            pf_set,
    UINT32                  u4_flags )
{
    return d_drv_cust_reg_ex(pt_reg,
                             pf_connect,
                             pf_disconnect,
                             pf_get,
                             pf_set,
                             u4_flags,
                             CONN_DIR_OUT_TO_INP);
}


INT32 d_drv_cust_reg_grp_name( 
    const DRV_REG_GRP_NAME_T*  pt_reg_grp_name )
{
    DRV_COMP_REG_T      at_reg_comp_ids[MAX_NUM_SINK_COMPS];
    SIZE_T              z_num_entries = 0;
    INT32               i4_ret;
    
    if ( !pt_reg_grp_name )
    {
        return -1;
    }

    z_num_entries = MAX_NUM_SINK_COMPS;
    i4_ret = _cust_drv_ids_2_drv_comp_regs(
        pt_reg_grp_name->pt_drv_ids,
        at_reg_comp_ids,
        &z_num_entries );
    if ( i4_ret != 0 )
    {
        return -1;
    }
    
    if ( z_num_entries > 0 )
    {
        i4_ret = x_rm_reg_group_name(
            pt_reg_grp_name->s_grp_name,
            at_reg_comp_ids,
            z_num_entries );
        if ( i4_ret != RMR_OK )
        {
            return -1;
        }
    }
    
    return 0;
}

INT32 d_drv_cust_reg_comp_excl( 
    const DRV_REG_COMP_EXCL_T*  pt_reg_comp_excl )
{
    DRV_COMP_REG_T      at_reg_comp_ids[MAX_NUM_SINK_COMPS];
    DRV_COMP_REG_T      t_src_comp_reg;
    SIZE_T              z_num_entries = 0;
    INT32               i4_ret;
    
    if ( !pt_reg_comp_excl )
    {
        return -1;
    }
    
    _cust_drv_id_2_drv_comp_reg(
        &pt_reg_comp_excl->pt_drv_ids[0],
        &t_src_comp_reg );
    
    z_num_entries = MAX_NUM_SINK_COMPS;
    i4_ret = _cust_drv_ids_2_drv_comp_regs(
        &pt_reg_comp_excl->pt_drv_ids[1],
        at_reg_comp_ids,
        &z_num_entries );
    if ( i4_ret != 0 )
    {
        return -1;
    }
    
    if ( z_num_entries > 0 )
    {
        i4_ret = x_rm_reg_comp_excl_list( 
            &t_src_comp_reg,
            at_reg_comp_ids,
            z_num_entries );
        if ( i4_ret != RMR_OK )
        {
            return -1;
        }
    }
    
    return 0;
}

INT32 d_drv_cust_parse( 
    const DRV_CUSTOM_T*     pt_drv_cust_list,
    drv_cust_parse_fct      pf_parse,
    VOID*                   pv_tag )
{
    DRV_CUSTOM_T*  pt_custom = (DRV_CUSTOM_T*)pt_drv_cust_list;
    
    if ( !pt_drv_cust_list || !pf_parse )
    {
        return -1;
    }
    
    while ( pt_custom &&
            !IS_DRV_CUSTOM_END(pt_custom) )
    {
        DRVP_TYPE_T  e_drvp_ret;
        
        e_drvp_ret = pf_parse( pt_custom, pv_tag );
        
        if ( e_drvp_ret == DRVP_BREAK )
        {
            break;
        }
        else if ( e_drvp_ret == DRVP_ERROR )
        {
            return -1;
        }
        
        pt_custom++;
    }
    
    return 0;
}
