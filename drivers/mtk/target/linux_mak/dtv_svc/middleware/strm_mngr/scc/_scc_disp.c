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
 * $RCSfile: _scc_disp.c,v $
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
#include "res_mngr/drv/x_plane_mxr.h"
#include "res_mngr/drv/x_tv_enc.h"
#include "res_mngr/drv/x_vid_plane.h"
#include "strm_mngr/x_sm.h"
#include "strm_mngr/sm_slctr.h"
#include "strm_mngr/video_hdlr/u_sm_video_hdlr.h"

#include "strm_mngr/_sm_utils.h"
#include "strm_mngr/scc/u_scc.h"
#include "strm_mngr/scc/_scc_disp.h"
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

static BOOL _pmx_check_get_cap( 
    SCC_DISP_COMP_T*    pt_disp_comp,
    UINT32              e_get_type,
    VOID*               pv_get_info,
    SIZE_T*             pz_get_size )
{
    UINT32   ui4_cap = pt_disp_comp->aui4_pla_mxr_cap[0];
    
    switch( e_get_type )
    {
    case PLA_MXR_GET_TYPE_CTRL:
        return TRUE;
        
    case PLA_MXR_GET_TYPE_BRIGHTNESS:
        return (ui4_cap&PLA_MXR_CAP_BRIGHTNESS)?TRUE:FALSE;
        
    case PLA_MXR_GET_TYPE_CONTRAST:
        return (ui4_cap&PLA_MXR_CAP_CONTRAST)?TRUE:FALSE;
        
    case PLA_MXR_GET_TYPE_HUE:
        return (ui4_cap&PLA_MXR_CAP_HUE)?TRUE:FALSE;
        
    case PLA_MXR_GET_TYPE_SATURATION:
        return (ui4_cap&PLA_MXR_CAP_SATURATION)?TRUE:FALSE;
        
    case PLA_MXR_GET_TYPE_GAMMA:
    case PLA_MXR_GET_TYPE_MAX_GAMMA_CURVE:
        return (ui4_cap&PLA_MXR_CAP_GAMMA)?TRUE:FALSE;
        
    case PLA_MXR_GET_TYPE_PLA_ORDER:
    case PLA_MXR_GET_TYPE_ADJ_PLA_BY_PORT:
    case PLA_MXR_GET_TYPE_ADJ_PLA_BY_ID_TYPE:
        return (ui4_cap&PLA_MXR_CAP_ADJ_PLA)?TRUE:FALSE;
        
    case PLA_MXR_GET_TYPE_RESOLUTION:
        return (ui4_cap&PLA_MXR_CAP_RESOLUTION)?TRUE:FALSE;
        
    case PLA_MXR_GET_TYPE_OUTPUT_PORT:
        return (ui4_cap&PLA_MXR_CAP_DIGITAL_OUT)?TRUE:FALSE;
        
    case PLA_MXR_GET_TYPE_BG:
        return (ui4_cap&PLA_MXR_CAP_BG)?TRUE:FALSE;
        
    case PLA_MXR_GET_TYPE_ASPECT_RATIO:
        return (ui4_cap&PLA_MXR_CAP_ASPECT_RATIO)?TRUE:FALSE;
        
    case PLA_MXR_GET_TYPE_DIGITAL_OUTPUT_FMT:
        return (ui4_cap&PLA_MXR_CAP_DIGITAL_OUTPUT_FMT)?TRUE:FALSE;
        
    case PLA_MXR_GET_TYPE_DITHER:
    case PLA_MXR_GET_TYPE_DITHER_CAP:
        return (ui4_cap&PLA_MXR_CAP_DITHER)?TRUE:FALSE;
    
    case PLA_MXR_GET_TYPE_MIN_MAX:
        {
            PLA_MXR_MIN_MAX_INFO_T*   pt_pmx_min_max = 
                (PLA_MXR_MIN_MAX_INFO_T*)pv_get_info;
            
            if ( !pt_pmx_min_max || 
                 !pz_get_size || 
                 *pz_get_size < sizeof(PLA_MXR_MIN_MAX_INFO_T) )
            {
                return FALSE;
            }
            
            return _pmx_check_get_cap( 
                pt_disp_comp, 
                pt_pmx_min_max->e_get_type,
                NULL,
                NULL );
        }
        
    case PLA_MXR_GET_TYPE_COLOR_GAIN:
    case PLA_MXR_GET_TYPE_COLOR_GAIN_MAX:
    case PLA_MXR_GET_TYPE_COLOR_GAIN_MIN:
        return (ui4_cap&PLA_MXR_CAP_COLOR_GAIN)?TRUE:FALSE;
        
    case PLA_MXR_GET_TYPE_COLOR_OFFSET:
        return (ui4_cap&PLA_MXR_CAP_COLOR_OFFSET)?TRUE:FALSE;
        
    case PLA_MXR_GET_TYPE_BACK_LIGHT:
        return (ui4_cap&PLA_MXR_CAP_BACK_LIGHT)?TRUE:FALSE;

    case PLA_MXR_GET_TYPE_ADP_BACK_LIGHT:
        return (ui4_cap&PLA_MXR_CAP_ADP_BACK_LIGHT)?TRUE:FALSE;

    case PLA_MXR_GET_TYPE_TV_MODE:
    case PLA_MXR_GET_TYPE_MJC:
    case PLA_MXR_GET_TYPE_TEST_PATTERN:
    case PLA_MXR_GET_TYPE_ADP_BACK_LIGHT_RNG:
    case PLA_MXR_GET_TYPE_BACK_LIGHT_ENERGY_SAVE:
    case PLA_MXR_GET_TYPE_FRAMERATE:
        return TRUE;
        
    default:
        return FALSE;
    }
}

static BOOL _pmx_check_set_cap( 
    SCC_DISP_COMP_T*    pt_disp_comp,
    UINT32              e_set_type )
{
    UINT32   ui4_cap = pt_disp_comp->aui4_pla_mxr_cap[0];
    
    switch( e_set_type )
    {
    case PLA_MXR_SET_TYPE_CTRL:
        return TRUE;
        
    case PLA_MXR_SET_TYPE_BRIGHTNESS:
        return (ui4_cap&PLA_MXR_CAP_BRIGHTNESS)?TRUE:FALSE;
        
    case PLA_MXR_SET_TYPE_CONTRAST:
        return (ui4_cap&PLA_MXR_CAP_CONTRAST)?TRUE:FALSE;
        
    case PLA_MXR_SET_TYPE_HUE:
        return (ui4_cap&PLA_MXR_CAP_HUE)?TRUE:FALSE;
        
    case PLA_MXR_SET_TYPE_SATURATION:
        return (ui4_cap&PLA_MXR_CAP_SATURATION)?TRUE:FALSE;
        
    case PLA_MXR_SET_TYPE_GAMMA:
        return (ui4_cap&PLA_MXR_CAP_GAMMA)?TRUE:FALSE;
        
    case PLA_MXR_SET_TYPE_PLA_ORDER:
        return (ui4_cap&PLA_MXR_CAP_ADJ_PLA)?TRUE:FALSE;
        
    case PLA_MXR_SET_TYPE_RESOLUTION:
        return (ui4_cap&PLA_MXR_CAP_RESOLUTION)?TRUE:FALSE;
        
    case PLA_MXR_SET_TYPE_OUTPUT_PORT:
        return (ui4_cap&PLA_MXR_CAP_DIGITAL_OUT)?TRUE:FALSE;
        
    case PLA_MXR_SET_TYPE_BG:
        return (ui4_cap&PLA_MXR_CAP_BG)?TRUE:FALSE;
        
    case PLA_MXR_SET_TYPE_ASPECT_RATIO:
        return (ui4_cap&PLA_MXR_CAP_ASPECT_RATIO)?TRUE:FALSE;
        
    case PLA_MXR_SET_TYPE_DIGITAL_OUTPUT_FMT:
        return (ui4_cap&PLA_MXR_CAP_DIGITAL_OUTPUT_FMT)?TRUE:FALSE;
        
    case PLA_MXR_SET_TYPE_DITHER:
        return (ui4_cap&PLA_MXR_CAP_DITHER)?TRUE:FALSE;
        
    case PLA_MXR_SET_TYPE_COLOR_GAIN:
        return (ui4_cap&PLA_MXR_CAP_COLOR_GAIN)?TRUE:FALSE;
        
    case PLA_MXR_SET_TYPE_COLOR_OFFSET:
        return (ui4_cap&PLA_MXR_CAP_COLOR_OFFSET)?TRUE:FALSE;
        
    case PLA_MXR_SET_TYPE_BACK_LIGHT:
        return (ui4_cap&PLA_MXR_CAP_BACK_LIGHT)?TRUE:FALSE;

    case PLA_MXR_SET_TYPE_ADP_BACK_LIGHT:
        return (ui4_cap&PLA_MXR_CAP_ADP_BACK_LIGHT)?TRUE:FALSE;

    case PLA_MXR_SET_TYPE_BACK_LIGHT_CTRL:
        return (ui4_cap&PLA_MXR_CAP_BACK_LIGHT_CTRL)?TRUE:FALSE;

    case PLA_MXR_SET_TYPE_TV_MODE:
    case PLA_MXR_SET_TYPE_MJC:
    case PLA_MXR_SET_TYPE_TEST_PATTERN:
    case PLA_MXR_SET_TYPE_ADP_BACK_LIGHT_RNG:
    case PLA_MXR_SET_TYPE_BACK_LIGHT_ENERGY_SAVE:
    case PLA_MXR_SET_TYPE_FRAMERATE:
    case PLA_MXR_SET_TYPE_FORCE_FRAMERATE:
        return TRUE;
        
    default:
        return FALSE;
    }
}

static UINT32 _pmx_get_type_to_vid_get_type( UINT32 e_pmx_get_type )
{
    switch( e_pmx_get_type )
    {
    case PLA_MXR_GET_TYPE_BRIGHTNESS:
        return VID_PLA_GET_TYPE_BRIGHTNESS;
        
    case PLA_MXR_GET_TYPE_CONTRAST:
        return VID_PLA_GET_TYPE_CONTRAST;
        
    case PLA_MXR_GET_TYPE_HUE:
        return VID_PLA_GET_TYPE_HUE;
        
    case PLA_MXR_GET_TYPE_SATURATION:
        return VID_PLA_GET_TYPE_SATURATION;
        
    case PLA_MXR_GET_TYPE_COLOR_GAIN:
        return VID_PLA_GET_TYPE_COLOR_GAIN;

    case PLA_MXR_GET_TYPE_COLOR_GAIN_MAX:
        return VID_PLA_GET_TYPE_COLOR_GAIN_MAX;

    case PLA_MXR_GET_TYPE_COLOR_GAIN_MIN:
        return VID_PLA_GET_TYPE_COLOR_GAIN_MIN;
        
    case PLA_MXR_GET_TYPE_COLOR_OFFSET:
        return VID_PLA_GET_TYPE_COLOR_OFFSET;

    case PLA_MXR_GET_TYPE_COLOR_OFFSET_MIN:
        return VID_PLA_GET_TYPE_COLOR_OFFSET_MIN;

    case PLA_MXR_GET_TYPE_COLOR_OFFSET_MAX:
        return VID_PLA_GET_TYPE_COLOR_OFFSET_MAX;

    case PLA_MXR_GET_TYPE_MIN_MAX:
        return VID_PLA_GET_TYPE_MIN_MAX;
    
    default:
        return 0xFFFFFFFF;
    }
}

static INT32 _redirect_pmx_get_type_to_vid_comp(
    SCC_DISP_COMP_T*    pt_disp_comp,
    UINT32              e_get_type,
    VOID*               pv_get_info,
    SIZE_T*             pz_get_size )
{
    INT32                  i4_ret;
    UINT32                 e_vid_get_type;
    DRV_TYPE_T             e_drv_type;
    VID_PLA_COLOR_GAIN_T   t_vid_color_gain;
    VOID*                  pv_vid_get_info = NULL;
    SIZE_T                 z_vid_get_size = 0;
    VID_PLA_MIN_MAX_INFO_T t_vid_min_max;
    UINT16                 ui2_vid_get_val = 0;
    
    e_vid_get_type = _pmx_get_type_to_vid_get_type( e_get_type );
    if ( e_vid_get_type == (UINT32)0xFFFFFFFF )
    {
        return SMR_NOT_SUPPORT;
    }
    
    switch( e_get_type )
    {
    case PLA_MXR_GET_TYPE_BRIGHTNESS:
        e_drv_type = DRVT_VID_PLANE;
        pv_vid_get_info = &ui2_vid_get_val;
        z_vid_get_size = sizeof(UINT16);
        break;
        
    case PLA_MXR_GET_TYPE_CONTRAST:
        e_drv_type = DRVT_VID_PLANE;
        pv_vid_get_info = &ui2_vid_get_val;
        z_vid_get_size = sizeof(UINT16);
        break;
        
    case PLA_MXR_GET_TYPE_HUE:
        e_drv_type = DRVT_VID_PLANE;
        pv_vid_get_info = &ui2_vid_get_val;
        z_vid_get_size = sizeof(UINT16);
        break;
        
    case PLA_MXR_GET_TYPE_SATURATION:
        e_drv_type = DRVT_VID_PLANE;
        pv_vid_get_info = &ui2_vid_get_val;
        z_vid_get_size = sizeof(UINT16);
        break;
        
    case PLA_MXR_GET_TYPE_COLOR_GAIN:
    case PLA_MXR_GET_TYPE_COLOR_GAIN_MAX:
    case PLA_MXR_GET_TYPE_COLOR_GAIN_MIN:
        e_drv_type = DRVT_VID_PLANE;
        pv_vid_get_info = (VOID*)&t_vid_color_gain;
        z_vid_get_size = sizeof(VID_PLA_COLOR_GAIN_T);
        break;

    case PLA_MXR_GET_TYPE_COLOR_OFFSET:
    case PLA_MXR_GET_TYPE_COLOR_OFFSET_MIN:
    case PLA_MXR_GET_TYPE_COLOR_OFFSET_MAX:
        e_drv_type = DRVT_VID_PLANE;
        pv_vid_get_info = (VOID*)&t_vid_color_gain;
        z_vid_get_size = sizeof(VID_PLA_COLOR_GAIN_T);
        break;

    case PLA_MXR_GET_TYPE_MIN_MAX:
        e_drv_type = DRVT_VID_PLANE;
        
        t_vid_min_max.e_get_type = _pmx_get_type_to_vid_get_type( 
            ((PLA_MXR_MIN_MAX_INFO_T*)pv_get_info)->e_get_type );
        if ( t_vid_min_max.e_get_type == (UINT32)0xFFFFFFFF )
        {
            return SMR_INV_ARG;
        }
        if ( !pz_get_size || *pz_get_size < sizeof(PLA_MXR_MIN_MAX_INFO_T) )
        {
            return SMR_INSUFFICIENT_SIZE;
        }
        pv_vid_get_info = (VOID*)&t_vid_min_max;
        z_vid_get_size = sizeof(VID_PLA_MIN_MAX_INFO_T);
        break;
    
    default:
        return SMR_INV_ARG;
    }

    if ( pt_disp_comp->h_vid_comp == NULL_HANDLE )
    {
        i4_ret = _scc_comp_vid_open( 
            NULL_HANDLE,
            pt_disp_comp->s_grp_name,
            &pt_disp_comp->h_vid_comp );
        if ( i4_ret != SMR_OK )
        {
            return SMR_DRV_COMP_NOT_FOUND;
        }
    }
    
    i4_ret = _scc_comp_vid_get( 
        pt_disp_comp->h_vid_comp,
        e_drv_type,
        e_vid_get_type,
        pv_vid_get_info,
        &z_vid_get_size );    
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    if ( PLA_MXR_GET_TYPE_COLOR_OFFSET == e_get_type ||
         PLA_MXR_GET_TYPE_COLOR_OFFSET_MIN == e_get_type ||
         PLA_MXR_GET_TYPE_COLOR_OFFSET_MAX == e_get_type ||
         PLA_MXR_GET_TYPE_COLOR_GAIN == e_get_type ||
         PLA_MXR_GET_TYPE_COLOR_GAIN_MIN == e_get_type ||
         PLA_MXR_GET_TYPE_COLOR_GAIN_MAX == e_get_type )
    {
        ((PLA_MXR_COLOR_GAIN_T*)pv_get_info)->ui2_r_gain = 
            t_vid_color_gain.ui2_r_gain;
        ((PLA_MXR_COLOR_GAIN_T*)pv_get_info)->ui2_g_gain = 
            t_vid_color_gain.ui2_g_gain;
        ((PLA_MXR_COLOR_GAIN_T*)pv_get_info)->ui2_b_gain = 
            t_vid_color_gain.ui2_b_gain;
        *pz_get_size = sizeof(PLA_MXR_COLOR_GAIN_T);
    }
    else if ( PLA_MXR_GET_TYPE_MIN_MAX == e_get_type )
    {
        ((PLA_MXR_MIN_MAX_INFO_T*)pv_get_info)->ui4_min_value = 
            t_vid_min_max.ui4_min_value;
        ((PLA_MXR_MIN_MAX_INFO_T*)pv_get_info)->ui4_max_value = 
            t_vid_min_max.ui4_max_value;
        *pz_get_size = sizeof(PLA_MXR_MIN_MAX_INFO_T);
    }
    else if ( PLA_MXR_GET_TYPE_BRIGHTNESS == e_get_type ||
              PLA_MXR_GET_TYPE_CONTRAST == e_get_type ||
              PLA_MXR_GET_TYPE_HUE == e_get_type ||
              PLA_MXR_GET_TYPE_SATURATION == e_get_type )
    {
        if ( *pz_get_size >= sizeof(UINT32) )
        {
            SCC_SET_VAL_32( pv_get_info, (UINT32)ui2_vid_get_val );
        }
        else if ( *pz_get_size == sizeof(UINT16) )
        {
            SCC_SET_VAL_16( pv_get_info, ui2_vid_get_val );
        }
        else if ( *pz_get_size == sizeof(UINT8) )
        {
            SCC_SET_VAL_8( pv_get_info, (UINT8)ui2_vid_get_val );
        }
        else
        {
            SM_ASSERT(0);
        }
        
        *pz_get_size = z_vid_get_size;
    }

    return SMR_OK;
}

static INT32 _redirect_pmx_set_type_to_vid_comp(
    SCC_DISP_COMP_T*    pt_disp_comp,
    UINT32              e_set_type,
    VOID*               pv_set_info,
    SIZE_T              z_set_size )
{
    INT32                 i4_ret;
    UINT32                e_vid_set_type;
    DRV_TYPE_T            e_drv_type;
    VID_PLA_COLOR_GAIN_T  t_vid_color_gain;
    VOID*                 pv_vid_set_info = NULL;
    SIZE_T                z_vid_set_size = 0;
    
    switch( e_set_type )
    {
    case PLA_MXR_SET_TYPE_BRIGHTNESS:
        e_vid_set_type = VID_PLA_SET_TYPE_BRIGHTNESS;
        e_drv_type = DRVT_VID_PLANE;
        pv_vid_set_info = pv_set_info;
        z_vid_set_size = sizeof(UINT8);
        break;
        
    case PLA_MXR_SET_TYPE_CONTRAST:
        e_vid_set_type = VID_PLA_SET_TYPE_CONTRAST;
        e_drv_type = DRVT_VID_PLANE;
        pv_vid_set_info = pv_set_info;
        z_vid_set_size = sizeof(UINT8);
        break;
        
    case PLA_MXR_SET_TYPE_HUE:
        e_vid_set_type = VID_PLA_SET_TYPE_HUE;
        e_drv_type = DRVT_VID_PLANE;
        pv_vid_set_info = pv_set_info;
        z_vid_set_size = sizeof(UINT8);
        break;
        
    case PLA_MXR_SET_TYPE_SATURATION:
        e_vid_set_type = VID_PLA_SET_TYPE_SATURATION;
        e_drv_type = DRVT_VID_PLANE;
        pv_vid_set_info = pv_set_info;
        z_vid_set_size = sizeof(UINT8);
        break;
        
    case PLA_MXR_SET_TYPE_COLOR_GAIN:
        e_vid_set_type = VID_PLA_SET_TYPE_COLOR_GAIN;
        e_drv_type = DRVT_VID_PLANE;
        
        t_vid_color_gain.ui2_r_gain = 
            ((PLA_MXR_COLOR_GAIN_T*)pv_set_info)->ui2_r_gain;
        t_vid_color_gain.ui2_g_gain = 
            ((PLA_MXR_COLOR_GAIN_T*)pv_set_info)->ui2_g_gain;
        t_vid_color_gain.ui2_b_gain = 
            ((PLA_MXR_COLOR_GAIN_T*)pv_set_info)->ui2_b_gain;
        pv_vid_set_info = (VOID*)&t_vid_color_gain;
        z_vid_set_size = sizeof(VID_PLA_COLOR_GAIN_T);
        break;
        
    case PLA_MXR_SET_TYPE_COLOR_OFFSET:
        e_vid_set_type = VID_PLA_SET_TYPE_COLOR_OFFSET;
        e_drv_type = DRVT_VID_PLANE;

        t_vid_color_gain.ui2_r_gain = 
            ((PLA_MXR_COLOR_GAIN_T*)pv_set_info)->ui2_r_gain;
        t_vid_color_gain.ui2_g_gain = 
            ((PLA_MXR_COLOR_GAIN_T*)pv_set_info)->ui2_g_gain;
        t_vid_color_gain.ui2_b_gain = 
            ((PLA_MXR_COLOR_GAIN_T*)pv_set_info)->ui2_b_gain;
        pv_vid_set_info = (VOID*)&t_vid_color_gain;
        z_vid_set_size = sizeof(VID_PLA_COLOR_GAIN_T);
        break;
    
    default:
        return SMR_NOT_SUPPORT;
    }

    if ( pt_disp_comp->h_vid_comp == NULL_HANDLE )
    {
        i4_ret = _scc_comp_vid_open( 
            NULL_HANDLE,
            pt_disp_comp->s_grp_name,
            &pt_disp_comp->h_vid_comp );
        if ( i4_ret != SMR_OK )
        {
            return SMR_DRV_COMP_NOT_FOUND;
        }
    }
    
    i4_ret = _scc_comp_vid_set( 
        pt_disp_comp->h_vid_comp,
        e_drv_type,
        e_vid_set_type,
        pv_vid_set_info,
        z_vid_set_size );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }
    
    return SMR_OK;
}

static HANDLE_T _get_pla_mxr_from_vs( HANDLE_T h_vs )
{
    HANDLE_T      h_drv_clone;
    HANDLE_T      h_pla_mxr;
    HANDLE_TYPE_T e_drv_type;
    SIZE_T        z_size;
    INT32         i4_ret;
    
    z_size = sizeof(HANDLE_T);
    i4_ret = x_sm_get( h_vs,
                       SM_VSH_GET_TYPE_PLANE_MIXER,
                       (VOID*)&h_drv_clone,
                       &z_size );
    if ( i4_ret != SMR_OK )
    {
        return NULL_HANDLE;
    }
    
    h_pla_mxr = sm_drv_hdl_clone_get_hdl(h_drv_clone);

    i4_ret = rm_get_comp_info_from_handle( 
        h_pla_mxr,
        &e_drv_type,
        NULL,
        NULL,
        NULL,
        NULL );
    if ( i4_ret != RMR_OK || e_drv_type != DRVT_PLA_MXR )
    {
        return NULL_HANDLE;
    }
    
    return h_pla_mxr;
}

static HANDLE_T _get_tv_enc_from_vs( HANDLE_T h_vs )
{
    HANDLE_T      h_drv_clone;
    HANDLE_T      h_tv_enc;
    HANDLE_TYPE_T e_drv_type;
    SIZE_T        z_size;
    INT32         i4_ret;
    
    z_size = sizeof(HANDLE_T);
    i4_ret = x_sm_get( h_vs,
                       SM_VSH_GET_TYPE_TV_ENCODER,
                       (VOID*)&h_drv_clone,
                       &z_size );
    if ( i4_ret != SMR_OK )
    {
        return NULL_HANDLE;
    }
    
    h_tv_enc = sm_drv_hdl_clone_get_hdl(h_drv_clone);

    i4_ret = rm_get_comp_info_from_handle( 
        h_tv_enc,
        &e_drv_type,
        NULL,
        NULL,
        NULL,
        NULL );
    if ( i4_ret != RMR_OK || e_drv_type != DRVT_TV_ENC )
    {
        return NULL_HANDLE;
    }
    
    return h_tv_enc;
}

static BOOL _scc_comp_disp_free_fct( 
    HANDLE_T       h_handle,
    HANDLE_TYPE_T  e_drv_type,
    VOID*          pv_obj,
    VOID*          pv_tag,
    BOOL           b_req_handle )
{
    SCC_DISP_COMP_T*   pt_disp_comp = (SCC_DISP_COMP_T*)pv_obj;
    
    if ( b_req_handle )
    {
        return FALSE;
    }
    if ( !pv_obj )
    {
        SM_ASSERT( 0 );
    }
    
    if ( pt_disp_comp != NULL && pt_disp_comp->h_vid_comp != NULL_HANDLE )
    {
       _scc_comp_vid_close( pt_disp_comp->h_vid_comp );
    }
    
    _scc_drv_grp_close( pt_disp_comp->ah_pla_mxr, SCC_COMP_MAX_NUM_DRV_COMPS );
    _scc_drv_grp_close( pt_disp_comp->ah_tv_enc, SCC_COMP_MAX_NUM_DRV_COMPS );
#ifdef MW_TV_AV_BYPASS_SUPPORT
    _scc_drv_grp_close( pt_disp_comp->ah_video_out, SCC_COMP_MAX_NUM_DRV_COMPS );
#endif
    x_mem_free( pv_obj );

    return TRUE;
}

static VOID _scc_comp_disp_get_pla_mxr_cap(
    SCC_DISP_COMP_T*   pt_disp_comp )
{
    INT32               i4_ret = RMR_OK;
    INT32               i4_idx = 0;
    SIZE_T              z_size;
    RM_COND_T           e_cond;
    
    while( i4_idx < SCC_COMP_MAX_NUM_DRV_COMPS && 
           pt_disp_comp->ah_pla_mxr[i4_idx] != NULL_HANDLE )
    {
        z_size = sizeof(UINT32);
        i4_ret = rm_get( 
            pt_disp_comp->ah_pla_mxr[i4_idx],
            FALSE,
            ANY_PORT_NUM,
            PLA_MXR_GET_TYPE_CAPABILITY,
            (VOID*)&pt_disp_comp->aui4_pla_mxr_cap[i4_idx],
            &z_size,
            &e_cond );
        if ( i4_ret != RMR_OK )
        {
            SM_SLCTR_DBG_INFO(( 
                "[SCC] %s:%d rm_get(PLA_MXR_GET_TYPE_CAPABILITY) fail return %d\r\n",
                __FILE__, __LINE__,
                i4_ret ));
        }
        
        ++i4_idx;
    }    
}

static VOID _scc_comp_disp_get_tv_enc_cap(
    SCC_DISP_COMP_T*   pt_disp_comp )
{
    INT32               i4_ret = RMR_OK;
    INT32               i4_idx = 0;
    SIZE_T              z_size;
    RM_COND_T           e_cond;
    
    while( i4_idx < SCC_COMP_MAX_NUM_DRV_COMPS && 
           pt_disp_comp->ah_tv_enc[i4_idx] != NULL_HANDLE )
    {
        z_size = sizeof(UINT32);
        i4_ret = rm_get( 
            pt_disp_comp->ah_tv_enc[i4_idx],
            FALSE,
            ANY_PORT_NUM,
            TV_ENC_GET_TYPE_FMT_CAP,
            (VOID*)&pt_disp_comp->aui4_tv_enc_fmt_cap[i4_idx],
            &z_size,
            &e_cond );
        if ( i4_ret != RMR_OK )
        {
            SM_SLCTR_DBG_INFO(( 
                "[SCC] %s:%d rm_get(PLA_MXR_GET_TYPE_CAPABILITY) fail return %d\r\n",
                __FILE__, __LINE__,
                i4_ret ));
        }
        
        ++i4_idx;
    }    
}

static INT32 _get_color_val_min_max_curr( 
    HANDLE_T                   h_disp,
    UINT32                     e_get_type,
    SCC_DISP_COLOR_GAIN_ARG_T  e_cg_arg,
    UINT16*                    pui2_min,
    UINT16*                    pui2_max,
    UINT16*                    pui2_curr )
{
    INT32                        i4_ret;
    SIZE_T                       z_size;
    UINT32                       e_get_min;
    UINT32                       e_get_max;
    PLA_MXR_COLOR_GAIN_T         t_drv_cg_min, t_drv_cg_max;
    PLA_MXR_COLOR_GAIN_T         t_drv_cg_curr;
    
    if ( e_get_type == PLA_MXR_GET_TYPE_COLOR_GAIN )
    {
        e_get_min = PLA_MXR_GET_TYPE_COLOR_GAIN_MIN;
        e_get_max = PLA_MXR_GET_TYPE_COLOR_GAIN_MAX;
    }
    else if ( e_get_type == PLA_MXR_GET_TYPE_COLOR_OFFSET )
    {
        e_get_min = PLA_MXR_GET_TYPE_COLOR_OFFSET_MIN;
        e_get_max = PLA_MXR_GET_TYPE_COLOR_OFFSET_MAX;
    }
    else
    {
        return SMR_INV_ARG;
    }
    
    z_size = sizeof(PLA_MXR_COLOR_GAIN_T);
    i4_ret = _scc_comp_disp_get( 
        h_disp,
        DRVT_PLA_MXR,
        e_get_max,
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
        e_get_min,
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
        e_get_type,
        (VOID*)&t_drv_cg_curr,
        &z_size );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }
    
    if ( e_cg_arg == SCC_DISP_COLOR_GAIN_ARG_R )
    {
        SCC_SET_VAL_16( pui2_min, t_drv_cg_min.ui2_r_gain );
        SCC_SET_VAL_16( pui2_max, t_drv_cg_max.ui2_r_gain );
        SCC_SET_VAL_16( pui2_curr, t_drv_cg_curr.ui2_r_gain );
    }
    else if ( e_cg_arg == SCC_DISP_COLOR_GAIN_ARG_G )
    {
        SCC_SET_VAL_16( pui2_min, t_drv_cg_min.ui2_g_gain );
        SCC_SET_VAL_16( pui2_max, t_drv_cg_max.ui2_g_gain );
        SCC_SET_VAL_16( pui2_curr, t_drv_cg_curr.ui2_g_gain );
    }
    else if ( e_cg_arg == SCC_DISP_COLOR_GAIN_ARG_B )
    {
        SCC_SET_VAL_16( pui2_min, t_drv_cg_min.ui2_b_gain );
        SCC_SET_VAL_16( pui2_max, t_drv_cg_max.ui2_b_gain );
        SCC_SET_VAL_16( pui2_curr, t_drv_cg_curr.ui2_b_gain );
    }
    else
    {
        return SMR_INV_ARG;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
INT32 _scc_comp_disp_open( 
    HANDLE_T            h_vs,
    const CHAR*         s_grp_name,
    HANDLE_T*           ph_comp )
{
    INT32               i4_ret = SMR_OK;
    SCC_DISP_COMP_T*    pt_disp_comp = NULL;
    
    if ( h_vs == NULL_HANDLE && !s_grp_name )
    {
        return SMR_INV_ARG;
    }
    if ( !ph_comp )
    {
        return SMR_INV_ARG;
    }
    
    pt_disp_comp = (SCC_DISP_COMP_T*)x_mem_alloc( sizeof(SCC_DISP_COMP_T) );
    if ( !pt_disp_comp )
    {
        return SMR_INSUFFICIENT_MEMORY;
    }

    x_memset( pt_disp_comp, 0, sizeof(SCC_DISP_COMP_T) );
    pt_disp_comp->e_cmd_type = SCC_CMD_TYPE_DISP;
    
    if ( h_vs != NULL_HANDLE )
    {
        HANDLE_T      h_drv_disp_pla;
        HANDLE_T      h_drv_tv_enc;
                
        /* Open Plane Mixer */
        h_drv_disp_pla = _get_pla_mxr_from_vs( h_vs );
        if ( h_drv_disp_pla == NULL_HANDLE )
        {
            x_mem_free( pt_disp_comp );
            return SMR_INV_HANDLE;
        }
        
        pt_disp_comp->ah_pla_mxr[0] = _scc_open_drv_from_handle( h_drv_disp_pla );
        if ( pt_disp_comp->ah_pla_mxr[0] == NULL_HANDLE )
        {
            x_mem_free( pt_disp_comp );
            return SMR_INV_HANDLE;;
        }
        
        /* Open TV Encoder */
        h_drv_tv_enc = _get_tv_enc_from_vs( h_vs );
        if ( h_drv_tv_enc != NULL_HANDLE )
        {
            /* TV Encoder is optional */
            pt_disp_comp->ah_tv_enc[0] = _scc_open_drv_from_handle( h_drv_tv_enc );
            if ( pt_disp_comp->ah_tv_enc[0] == NULL_HANDLE )
            {
                _scc_drv_grp_close( pt_disp_comp->ah_pla_mxr, SCC_COMP_MAX_NUM_DRV_COMPS );
                x_mem_free( pt_disp_comp );
                return SMR_INV_HANDLE;;
            }
        }
    }
    else /* use group name */
    {
        SIZE_T    z_size;
#ifdef MW_TV_AV_BYPASS_SUPPORT
        BOOL      b_bypass = FALSE;
#endif
        
        x_strcpy( pt_disp_comp->s_grp_name, s_grp_name );
        
        z_size = SCC_COMP_MAX_NUM_DRV_COMPS;
        i4_ret = _scc_drv_grp_open_from_name(
            NULL_HANDLE,
            DRVT_PLA_MXR,
            s_grp_name,
            pt_disp_comp->ah_pla_mxr,
            &z_size );
        if ( i4_ret != SMR_OK )
        {
#ifdef MW_TV_AV_BYPASS_SUPPORT
            /* Ignore if there is no TV Encoder in case of Bypass */
            b_bypass = TRUE;
#else
            x_mem_free( pt_disp_comp );
            return SMR_DRV_COMP_NOT_FOUND;
#endif
        }
        
        z_size = SCC_COMP_MAX_NUM_DRV_COMPS;
        i4_ret = _scc_drv_grp_open_from_name(
            NULL_HANDLE,
            DRVT_TV_ENC,
            s_grp_name,
            pt_disp_comp->ah_tv_enc,
            &z_size );
        if ( i4_ret != SMR_OK )
        {
#ifdef MW_TV_AV_BYPASS_SUPPORT
            if ( b_bypass )
            {
                x_mem_free( pt_disp_comp );
                return SMR_DRV_COMP_NOT_FOUND;
            }
            else
#endif
            {
            /* Ignore if there is no TV Encoder */
            }
        }

#ifdef MW_TV_AV_BYPASS_SUPPORT
        z_size = SCC_COMP_MAX_NUM_DRV_COMPS;
        i4_ret = _scc_drv_grp_open_from_name(
            NULL_HANDLE,
            DRVT_VIDEO_OUT,
            s_grp_name,
            pt_disp_comp->ah_video_out,
            &z_size );
        if ( i4_ret != SMR_OK )
        {
            if ( b_bypass )
            {
                _scc_drv_grp_close( pt_disp_comp->ah_tv_enc, SCC_COMP_MAX_NUM_DRV_COMPS );
                x_mem_free( pt_disp_comp );
                return SMR_DRV_COMP_NOT_FOUND;
            }
            else
            {
            /* Ignore if there is no Video Out */
            }
        }
#endif
    }
    
    _scc_comp_disp_get_pla_mxr_cap( pt_disp_comp );
    _scc_comp_disp_get_tv_enc_cap( pt_disp_comp );
    
    i4_ret = handle_alloc( SM_HT_SCC_GRP_NAME,
                           (VOID*)pt_disp_comp,
                           (VOID*)pt_disp_comp->e_cmd_type,
                           &_scc_comp_disp_free_fct,
                           ph_comp );
    if ( i4_ret != HR_OK )
    {
        _scc_drv_grp_close( pt_disp_comp->ah_pla_mxr, SCC_COMP_MAX_NUM_DRV_COMPS );
        _scc_drv_grp_close( pt_disp_comp->ah_tv_enc, SCC_COMP_MAX_NUM_DRV_COMPS );
#ifdef MW_TV_AV_BYPASS_SUPPORT
        _scc_drv_grp_close( pt_disp_comp->ah_video_out, SCC_COMP_MAX_NUM_DRV_COMPS );
#endif
        x_mem_free( pt_disp_comp );
        return SMR_OUT_OF_HANDLES;
    }
    
    return SMR_OK;
}

INT32 _scc_comp_disp_close( 
    HANDLE_T            h_comp )
{
    INT32               i4_ret = HR_OK;
    HANDLE_TYPE_T       e_type;
    SCC_DISP_COMP_T*    pt_disp_comp = NULL;

    i4_ret = handle_get_type_obj( 
        h_comp,
        &e_type,
        (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }
    if ( pt_disp_comp->e_cmd_type != SCC_CMD_TYPE_DISP )
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

INT32 _scc_comp_disp_set( 
    HANDLE_T            h_comp,
    DRV_TYPE_T          e_drv_type,
    UINT32              e_drv_set_type,
    VOID*               pv_set_info,
    SIZE_T              z_set_size )
{
    INT32               i4_ret;
    UINT16              ui2_idx, ui2_num_drv;
    
    i4_ret = _scc_comp_disp_get_num_drv( 
        h_comp,
        e_drv_type,
        &ui2_num_drv );
    if ( i4_ret != SMR_OK || ui2_num_drv > SCC_COMP_MAX_NUM_DRV_COMPS )
    {
        return i4_ret;
    }
    if ( ui2_num_drv == 0 )
    {
        return SMR_DRV_COMP_NOT_FOUND;
    }
    
    ui2_idx = 0;
    while( ui2_idx < ui2_num_drv )
    {
        i4_ret = _scc_comp_disp_set_ex(
            h_comp,
            e_drv_type,
            ui2_idx++,
            e_drv_set_type,
            pv_set_info,
            z_set_size );
        if ( i4_ret != RMR_OK )
        {
            return i4_ret;
        }
    }
    
    return SMR_OK;
}

INT32 _scc_comp_disp_set_ex( 
    HANDLE_T            h_comp,
    DRV_TYPE_T          e_drv_type,
    UINT16              ui2_idx,
    UINT32              e_drv_set_type,
    VOID*               pv_set_info,
    SIZE_T              z_set_size )
{
    INT32               i4_ret;
    HANDLE_TYPE_T       e_type;
    SCC_DISP_COMP_T*    pt_disp_comp = NULL;
    HANDLE_T*           ph_drv_array;
    RM_COND_T           e_cond;
    
    i4_ret = handle_get_type_obj( 
        h_comp,
        &e_type,
        (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }
    if ( pt_disp_comp->e_cmd_type != SCC_CMD_TYPE_DISP )
    {
        return SMR_INV_HANDLE;
    }
    
    if ( e_drv_type == DRVT_PLA_MXR )
    {
        ph_drv_array = pt_disp_comp->ah_pla_mxr;
        
        if ( !_pmx_check_set_cap( pt_disp_comp, e_drv_set_type ) )
        {
            i4_ret = _redirect_pmx_set_type_to_vid_comp(
                pt_disp_comp,
                e_drv_set_type,
                pv_set_info,
                z_set_size );
            return i4_ret;
        }
    }
    else if ( e_drv_type == DRVT_TV_ENC )
    {
        ph_drv_array = pt_disp_comp->ah_tv_enc;
        if ( gb_not_disable_tve_as_stop &&
             ((TV_ENC_CTRL_T)(UINT32)pv_set_info == TV_ENC_CTRL_DISABLE) )
        {
            return SMR_OK;
        }
        if ( !gb_enable_tve_as_play &&
             ((TV_ENC_CTRL_T)(UINT32)pv_set_info == TV_ENC_CTRL_ENABLE) )
        {
            return SMR_OK;
        }
    }
#ifdef MW_TV_AV_BYPASS_SUPPORT
    else if ( e_drv_type == DRVT_VIDEO_OUT)
    {
        ph_drv_array = pt_disp_comp->ah_video_out;
    }
#endif
    else
    {
        return SMR_INV_ARG;
    }
    
    if ( ui2_idx >= SCC_COMP_MAX_NUM_DRV_COMPS )
    {
        return SMR_INV_ARG;
    }
    
    if ( ph_drv_array[ui2_idx] == NULL_HANDLE )
    {
        return SMR_DRV_COMP_NOT_FOUND;
    }

    i4_ret = rm_set(
        ph_drv_array[ui2_idx],
        FALSE,
        ANY_PORT_NUM,
        e_drv_set_type,
        pv_set_info,
        z_set_size,
        &e_cond);
    if ( i4_ret != RMR_OK )
    {
        SM_SLCTR_DBG_INFO(( 
            "[SCC] %s:%d rm_set() fail return %d\r\n",
            __FILE__, __LINE__,
            i4_ret ));
        return SMR_DRV_COMP_ERROR;
    }
    
    return SMR_OK;
}

INT32 _scc_comp_disp_get( 
    HANDLE_T            h_comp,
    DRV_TYPE_T          e_drv_type,
    UINT32              e_drv_get_type,
    VOID*               pv_get_info,
    SIZE_T*             pz_get_size )
{
    INT32               i4_ret;
    UINT16              ui2_idx, ui2_num_drv;
    
    i4_ret = _scc_comp_disp_get_num_drv( 
        h_comp,
        e_drv_type,
        &ui2_num_drv );
    if ( i4_ret != SMR_OK || ui2_num_drv > SCC_COMP_MAX_NUM_DRV_COMPS )
    {
        return i4_ret;
    }
    if ( ui2_num_drv == 0 )
    {
        return SMR_DRV_COMP_NOT_FOUND;
    }
    
    ui2_idx = 0;
    while( ui2_idx < ui2_num_drv )
    {
        i4_ret = _scc_comp_disp_get_ex( 
            h_comp,
            e_drv_type,
            ui2_idx++,
            e_drv_get_type,
            (VOID*)pv_get_info,
            pz_get_size );
        if ( i4_ret != RMR_OK )
        {
            return i4_ret;
        }
    }
    
    return SMR_OK;
}

INT32 _scc_comp_disp_get_ex( 
    HANDLE_T            h_comp,
    DRV_TYPE_T          e_drv_type,
    UINT16              ui2_idx,
    UINT32              e_drv_get_type,
    VOID*               pv_get_info,
    SIZE_T*             pz_get_size )
{
    INT32               i4_ret;
    HANDLE_TYPE_T       e_type;
    SCC_DISP_COMP_T*    pt_disp_comp = NULL;
    HANDLE_T*           ph_drv_array;
    RM_COND_T           e_cond;
    SIZE_T              z_org_size = 0;
    
    i4_ret = handle_get_type_obj( 
        h_comp,
        &e_type,
        (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }
    if ( pt_disp_comp->e_cmd_type != SCC_CMD_TYPE_DISP )
    {
        return SMR_INV_HANDLE;
    }
    
    if ( e_drv_type == DRVT_PLA_MXR )
    {
        ph_drv_array = pt_disp_comp->ah_pla_mxr;

        if ( !_pmx_check_get_cap( pt_disp_comp, e_drv_get_type, pv_get_info, pz_get_size ) )
        {
            i4_ret = _redirect_pmx_get_type_to_vid_comp(
                pt_disp_comp,
                e_drv_get_type,
                pv_get_info,
                pz_get_size );
            return i4_ret;
        }
    }
    else if ( e_drv_type == DRVT_TV_ENC )
    {
        ph_drv_array = pt_disp_comp->ah_tv_enc;
    }
    else
    {
        return SMR_INV_ARG;
    }
    
    if ( ui2_idx >= SCC_COMP_MAX_NUM_DRV_COMPS )
    {
        return SMR_INV_ARG;
    }
    
    if ( ph_drv_array[ui2_idx] == NULL_HANDLE )
    {
        return SMR_DRV_COMP_NOT_FOUND;
    }
    
    if ( pz_get_size )
    {
        z_org_size = *pz_get_size;
    }
    i4_ret = rm_get( 
        ph_drv_array[ui2_idx],
        FALSE,
        ANY_PORT_NUM,
        e_drv_get_type,
        (VOID*)pv_get_info,
        pz_get_size,
        &e_cond );
    if ( i4_ret != RMR_OK )
    {
        SM_SLCTR_DBG_INFO(( 
            "[SCC] %s:%d rm_get() fail return %d\r\n",
            __FILE__, __LINE__,
            i4_ret ));
        return SMR_DRV_COMP_ERROR;
    }
    if ( z_org_size == 2 && *pz_get_size == 1 )
    {
        *((UINT16*)pv_get_info) &= (UINT16)0x00FF;
    }
    
    return SMR_OK;
}

INT32 _scc_comp_disp_get_num_drv( 
    HANDLE_T            h_comp,
    DRV_TYPE_T          e_drv_type,
    UINT16*             pui2_num_drv )
{
    INT32               i4_ret;
    UINT16              ui2_idx;
    UINT16*             pui2_num_drv_rec;
    HANDLE_TYPE_T       e_type;
    SCC_DISP_COMP_T*    pt_disp_comp = NULL;
    HANDLE_T*           ph_drv_array;
    
    if ( !pui2_num_drv )
    {
        return SMR_INV_ARG;
    }
    
    i4_ret = handle_get_type_obj( 
        h_comp,
        &e_type,
        (VOID**)&pt_disp_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_disp_comp )
    {
        return SMR_INV_HANDLE;
    }
    if ( pt_disp_comp->e_cmd_type != SCC_CMD_TYPE_DISP )
    {
        return SMR_INV_HANDLE;
    }
    
    if ( e_drv_type == DRVT_PLA_MXR )
    {
        ph_drv_array = pt_disp_comp->ah_pla_mxr;
        pui2_num_drv_rec = &pt_disp_comp->ui2_num_pla_mxr;
    }
    else if ( e_drv_type == DRVT_TV_ENC )
    {
        ph_drv_array = pt_disp_comp->ah_tv_enc;
        pui2_num_drv_rec = &pt_disp_comp->ui2_num_tv_enc;
    }
#ifdef MW_TV_AV_BYPASS_SUPPORT
    else if ( e_drv_type == DRVT_VIDEO_OUT )
    {
        ph_drv_array = pt_disp_comp->ah_video_out;
        pui2_num_drv_rec = &pt_disp_comp->ui2_num_video_out;
    }
#endif
    else
    {
        return SMR_INV_ARG;
    }
    
    if ( *pui2_num_drv_rec == 0 && ph_drv_array[0] != NULL_HANDLE )
    {
        ui2_idx = 0;
        while( ph_drv_array[ui2_idx] != NULL_HANDLE &&
               ui2_idx < SCC_COMP_MAX_NUM_DRV_COMPS )
        {
            ++ui2_idx;
        }
    
        *pui2_num_drv = ui2_idx;
    }
    else
    {
        *pui2_num_drv = *pui2_num_drv_rec;
    }
    
    return SMR_OK;
}

INT32 _scc_comp_disp_get_min_max_curr( 
    HANDLE_T                h_disp, 
    DRV_TYPE_T              e_drv_type,
    UINT32                  e_get_type,
    VOID*                   pv_arg,
    UINT16*                 pui2_min,
    UINT16*                 pui2_max,
    UINT16*                 pui2_curr )
{
    INT32                     i4_ret;
    SIZE_T                    z_size;
    
    if ( e_drv_type != DRVT_PLA_MXR )
    {
        return SMR_NOT_ALLOW;
    }
    
    switch( e_get_type )
    {
    case PLA_MXR_GET_TYPE_COLOR_GAIN:
    case PLA_MXR_GET_TYPE_COLOR_OFFSET:
        i4_ret = _get_color_val_min_max_curr(
            h_disp,
            e_get_type,
            (SCC_DISP_COLOR_GAIN_ARG_T)(UINT32)pv_arg,
            pui2_min,
            pui2_max,
            pui2_curr );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
        break;
    
    case PLA_MXR_GET_TYPE_BRIGHTNESS:
    case PLA_MXR_GET_TYPE_CONTRAST:
    case PLA_MXR_GET_TYPE_HUE:
    case PLA_MXR_GET_TYPE_SATURATION:
    case PLA_MXR_GET_TYPE_BACK_LIGHT:
        {
            PLA_MXR_MIN_MAX_INFO_T    t_drv_min_max;
            UINT16                    ui2_value;
            
            t_drv_min_max.e_get_type = e_get_type;
            z_size = sizeof(PLA_MXR_MIN_MAX_INFO_T);
            i4_ret = _scc_comp_disp_get( 
                h_disp,
                DRVT_PLA_MXR,
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
            
            z_size = sizeof(UINT16);
            i4_ret = _scc_comp_disp_get( 
                h_disp,
                DRVT_PLA_MXR,
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
            
            SCC_SET_VAL_16( pui2_min, t_drv_min_max.ui4_min_value );
            SCC_SET_VAL_16( pui2_max, t_drv_min_max.ui4_max_value );
            SCC_SET_VAL_16( pui2_curr, ui2_value );
        }
        break;
    
    default:
        return SMR_NOT_ALLOW;
    }
    
    return SMR_OK;
}

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
INT32 _scc_disp_set_ctrl( 
    HANDLE_T                        h_disp,
    SCC_DISP_CTRL_TYPE_T            e_ctrl )
{
    INT32                        i4_ret;
    HANDLE_TYPE_T                e_type;
    SCC_DISP_COMP_T*             pt_disp_comp;
    PLA_MXR_CTRL_T               e_drv_pmx_ctrl;
    TV_ENC_CTRL_T                e_drv_tve_ctrl;
    
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
        e_drv_pmx_ctrl = PLA_MXR_CTRL_RESET;
        e_drv_tve_ctrl = TV_ENC_CTRL_RESET;
        break;
        
    case SCC_DISP_CTRL_DISABLE:
        e_drv_pmx_ctrl = PLA_MXR_CTRL_DISABLE;
        e_drv_tve_ctrl = TV_ENC_CTRL_DISABLE;
        break;

    case SCC_DISP_CTRL_ENABLE:
        e_drv_pmx_ctrl = PLA_MXR_CTRL_ENABLE;
        e_drv_tve_ctrl = TV_ENC_CTRL_ENABLE;
        break;

    default:
        return SMR_INV_SET_INFO;
    }
    
    i4_ret = _scc_comp_disp_set( 
        h_disp,
        DRVT_PLA_MXR,
        PLA_MXR_SET_TYPE_CTRL,
        (VOID*)(UINT32)e_drv_pmx_ctrl,
        sizeof( PLA_MXR_CTRL_T ) );
    if ( i4_ret != SMR_OK )
    {
        return SMR_INV_SET_INFO;
    }

    i4_ret = _scc_comp_disp_set( 
        h_disp,
        DRVT_TV_ENC,
        TV_ENC_SET_TYPE_CTRL,
        (VOID*)(UINT32)e_drv_tve_ctrl,
        sizeof( TV_ENC_CTRL_T ) );
    if ( i4_ret != SMR_OK && i4_ret != SMR_DRV_COMP_NOT_FOUND )
    {
        return SMR_INV_SET_INFO;
    }

    return SMR_OK;
}

