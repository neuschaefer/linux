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
 * $RCSfile: _sm_bypass_comp_vbif.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/17 $
 * $SWAuthor: Ivan Wei $
 * $MD5HEX: ff30432460a0575709cf2e578efcfb8d $
 *
 * Description: 
 *---------------------------------------------------------------------------*/

#ifdef MW_TV_AV_BYPASS_SUPPORT

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "strm_mngr/bypass_hdlr/_sm_bypass_comp_vbif.h"

#include "vbi_fltr/x_vbi_fltr.h"

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

/*-----------------------------------------------------------------------------
 * Name: _sm_bypass_vbi_fltr_nfy
 *
 * Description: the (*x_vbif_nfy_fct) type callback function to be specified 
 *              when opening VBI Filter.
 *
 * Inputs:  See (*x_vbif_nfy_fct) for details.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_bypass_vbi_fltr_nfy (
    HANDLE_T             h_vbi_fltr,
    VBIF_COND_T          e_cond,
    VBIF_NFY_REASON_T    e_reason,
    VOID*                pv_tag,
    UINT32               ui4_data)
{
    HANDLE_T               h_stream = NULL_HANDLE;
    SM_BYPASS_STREAM_T*    pt_bypass_obj = NULL;

    h_stream = (HANDLE_T)pv_tag;
    pt_bypass_obj = (SM_BYPASS_STREAM_T*)sm_get_strm_obj( h_stream );
    if ( !pt_bypass_obj )
    {
        SM_ABORT( SM_DBG_ABRT_INV_STRM_OBJ );
    }
 
    pt_bypass_obj->e_vbif_cond = e_cond;

    if ( e_cond == VBIF_COND_CLOSED )
    {
        pt_bypass_obj->h_vbif = NULL_HANDLE;
    }
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: sm_bypass_comp_vbif_open
 *
 * Description: This API opens an instance of VBI Filter.
 *
 * Inputs:  h_stream      References.
 *          pt_bypass_obj References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
INT32 sm_bypass_comp_vbif_open (
    HANDLE_T               h_stream,
    SM_BYPASS_STREAM_T*    pt_bypass_obj )
{
    FLTR_INIT_EU_VBI_D2A_T    t_init_eu_vbi_d2a;
    VBIF_SRC_TYPE_T           e_src_type;
    VBIF_SRC_DIG_VBI_ES_T     t_vbi_src_dig_vbi_es;
    VBIF_SRC_DIG_TTX_ES_T     t_vbi_src_dig_ttx_es;
    VOID*                     pv_src_info;
    VOID*                     pv_tag_vbif_nfy;
    HANDLE_T                  h_vbi_fltr = NULL_HANDLE;
    INT32                     i4_ret;
    SM_SESSION_T*             pt_sess_obj = NULL;

    if ( pt_bypass_obj->t_bypass_comp_grp.t_tve.h_comp == NULL_HANDLE )
    {
        return SMR_INTERNAL_ERROR;
    }
    else
    {
        SIZE_T                z_size = 0;
        RM_COND_T             e_rm_cond;
        TV_ENC_ENC_CAP_INFO_T t_tve_cap = 0;
        
        z_size = sizeof(TV_ENC_ENC_CAP_INFO_T);
        i4_ret = rm_get( pt_bypass_obj->t_bypass_comp_grp.t_tve.h_comp,
            FALSE,
            0,
            TV_ENC_GET_TYPE_ENC_CAP,
            &t_tve_cap,
            &z_size,
            &e_rm_cond );
        if ( i4_ret != RMR_OK )
        {
            SM_BYPASS_DBG_INFO((
                "[SM] BYPASS: sm_bypass_state_do_started(0x%.8x), "
                "rm_get(TV_ENC_GET_TYPE_ENC_CAP) error return %d\r\n",
                (UINT32)h_stream, i4_ret ));
        }
        else if ( ( t_tve_cap & TV_ENC_ENC_CAP_VBI ) == 0 )
        {
            return SMR_OK;
        }
    }

    pt_sess_obj = (SM_SESSION_T*)sm_get_sess_obj( pt_bypass_obj->t_base.h_sess );
    if ( !pt_sess_obj )
    {
        return SMR_INV_HANDLE;
    }

    t_init_eu_vbi_d2a.h_connection_enc = pt_bypass_obj->t_base.h_connection;

    if ( pt_bypass_obj->t_vbif_src.e_src_type == BSH_VBIF_SRC_TYPE_DIG_VBI_ES )
    {
        e_src_type = SRC_TYPE_DIG_VBI_ES;
        t_vbi_src_dig_vbi_es.h_connection = pt_bypass_obj->t_base.h_connection;
        t_vbi_src_dig_vbi_es.t_pid = pt_bypass_obj->t_vbif_src.ui2_pid;
        t_vbi_src_dig_vbi_es.h_pm_service = pt_sess_obj->h_pm_service;
        pv_src_info = (VOID*) &t_vbi_src_dig_vbi_es;
    }
    else if ( pt_bypass_obj->t_vbif_src.e_src_type == BSH_VBIF_SRC_TYPE_DIG_TTX_ES )
    {
        e_src_type = SRC_TYPE_DIG_TTX_ES;
        t_vbi_src_dig_ttx_es.h_connection = pt_bypass_obj->t_base.h_connection;
        t_vbi_src_dig_ttx_es.t_pid = pt_bypass_obj->t_vbif_src.ui2_pid;
        t_vbi_src_dig_ttx_es.h_pm_service = pt_sess_obj->h_pm_service;
        pv_src_info = (VOID*) &t_vbi_src_dig_ttx_es;
    }
    else
    {
        return SMR_INV_ARG;
    }

    pv_tag_vbif_nfy = (VOID*)h_stream;
    
    i4_ret = x_vbif_open(
                 FLTR_TYPE_EU_VBI_D2A,
                 (VOID*)&t_init_eu_vbi_d2a,
                 e_src_type,
                 pv_src_info,
                 FLTR_MODE_PUMP,
                 pv_tag_vbif_nfy,
                 _sm_bypass_vbi_fltr_nfy,
                 &h_vbi_fltr,
                 &pt_bypass_obj->e_vbif_cond );

    if ( i4_ret == VBIFR_OK )
    {
        pt_bypass_obj->h_vbif = h_vbi_fltr;
    }
    else
    {
        return SMR_INTERNAL_ERROR;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_bypass_comp_vbif_close
 *
 * Description: This API close the VBI Filter instance.
 *
 * Inputs:  h_stream      References.
 *          pt_bypass_obj References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
INT32 sm_bypass_comp_vbif_close (
    SM_BYPASS_STREAM_T*    pt_bypass_obj,
    BOOL                   b_sync)
{
    if ( pt_bypass_obj->h_vbif != NULL_HANDLE )
    {
        SM_BYPASS_DBG_INFO(( "[SM] BYPASS: Freeing VBI Filter handle...\r\n" ));

        if ( b_sync )
        {
            x_vbif_close_sync( pt_bypass_obj->h_vbif, &pt_bypass_obj->e_vbif_cond );
            pt_bypass_obj->h_vbif = NULL_HANDLE;
        }
        else
        {
            x_vbif_close( pt_bypass_obj->h_vbif, &pt_bypass_obj->e_vbif_cond );
        }
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_bypass_comp_vbif_enable
 *
 * Description: This API opens an instance of VBI Filter.
 *
 * Inputs:  pt_bypass_obj References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
INT32 sm_bypass_comp_vbif_enable ( SM_BYPASS_STREAM_T*    pt_bypass_obj )
{
    INT32          i4_ret;

    if ( pt_bypass_obj->h_vbif != NULL_HANDLE )
    {
        i4_ret = x_vbif_start( pt_bypass_obj->h_vbif,
                               &pt_bypass_obj->e_vbif_cond );

        if ( i4_ret != VBIFR_OK )
        {
            return SMR_INTERNAL_ERROR;
        }
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_bypass_comp_vbif_disable
 *
 * Description: This API opens an instance of VBI Filter.
 *
 * Inputs:  pt_bypass_obj References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
INT32 sm_bypass_comp_vbif_disable ( SM_BYPASS_STREAM_T*    pt_bypass_obj )
{
    INT32          i4_ret;

    if ( pt_bypass_obj->h_vbif != NULL_HANDLE )
    {
        i4_ret = x_vbif_stop( pt_bypass_obj->h_vbif,
                              &pt_bypass_obj->e_vbif_cond );

        if ( i4_ret != VBIFR_OK )
        {
            return SMR_INTERNAL_ERROR;
        }
    }

    return SMR_OK;
}

#endif /* MW_TV_AV_BYPASS_SUPPORT */

