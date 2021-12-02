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
 * $RCSfile: mm_hdlr_uop_ps.c,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 0a0e5908fac0e73885ce39efe82bcd61 $ Asa
 *
 * Description: 
 *         This file contains all the media info public APIs  
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifdef _WIN32_
#include "inc/x_os.h"
#else
#include "os/inc/x_os.h"
#endif
#include "inc/x_common.h"
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "mutil/minfo/u_minfo.h"
#include "mutil/minfo/minfo.h"
#include "mutil/minfo/x_minfo.h"
#include "dbg/x_dbg.h"
#include "util/x_lnk_list.h"
#include "svctx/mm_hdlr/ps_uop/mm_hdlr_uop_ps.h"
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/

INT32 ps_uop_hdlr_open (MEDIA_DESCRIBT_T*   pt_obj)
{
    return SVCTXR_OK;
}

INT32 ps_uop_hdlr_is_hdlr(MEDIA_DESCRIBT_T*     pt_obj,
	                      MEDIA_FORMAT_T*       pt_favor_media_format)
{
    /*check paprameter*/
    if(pt_obj == NULL ||pt_favor_media_format ==NULL)
    {
        return SVCTXR_INV_ARG;
    }

    if (pt_favor_media_format->e_media_type == MEDIA_TYPE_PS
    && (pt_favor_media_format->t_media_subtype.u.e_ps_subtype == MEDIA_PS_SUBTYPE_DAT
    ||  pt_favor_media_format->t_media_subtype.u.e_ps_subtype == MEDIA_PS_SUBTYPE_VOB
    ||  pt_favor_media_format->t_media_subtype.u.e_ps_subtype == MEDIA_PS_SUBTYPE_MPG))
    {
        return SVCTXR_OK;
    }
    else
    {
        return SVCTXR_FAIL;
    }
}

INT32 ps_uop_hdlr_close (MEDIA_DESCRIBT_T*  pt_obj)
{
    return SVCTXR_OK;
}

INT32 ps_uop_hdlr_get_tm_code (MEDIA_DESCRIBT_T*            pt_obj,
                               MM_SVC_TIME_CODE_INFO_T*     pt_time_info)
{
    MM_SVC_TM_CODE_TYPE_T   e_tm_code_type;
    INT32                   i4_ret;
    
    /*check paprameter*/
    if (pt_obj == NULL ||pt_time_info ==NULL)
    {
        return SVCTXR_INV_ARG;
    }
    
    i4_ret = SVCTXR_OK;
    e_tm_code_type = pt_time_info->t_tm_code_type;
    switch(e_tm_code_type)
    {
    case MM_SVC_TM_CODE_TYPE_TITLE:
        i4_ret = SVCTXR_NOT_SUPPORT;
        break;
    case MM_SVC_TM_CODE_TYPE_CHAPTER:
        i4_ret = SVCTXR_NOT_SUPPORT;
        break;
    case MM_SVC_TM_CODE_TYPE_ALL:
        pt_time_info->ui8_time = 0;
        break;
    default:
        i4_ret = SVCTXR_NOT_SUPPORT;
        break;
    }
    return i4_ret;
}

INT32 ps_uop_hdlr_get_dur(MEDIA_DESCRIBT_T*         pt_obj, 
                          MM_SVC_DUR_INFO_T*        pt_time_dur)
{
    HANDLE_T            h_minfo;
    UINT16              ui2_rec_nums;
    SIZE_T              z_rec_sz;
    MM_SVC_DUR_TYPE_T   e_dur_type; 
    MINFO_INFO_T        t_media_info;  
    INT32               i4_ret;

    /*check paprameter*/
    if(pt_obj == NULL ||pt_time_dur ==NULL)
    {
        return SVCTXR_INV_ARG;
    }

    x_memset(&t_media_info, 0, sizeof (MINFO_INFO_T));
    i4_ret = SVCTXR_OK;
    
    e_dur_type = pt_time_dur->t_dur_type;

    h_minfo =  pt_obj->h_media_info;
    switch(e_dur_type)
    {
    case MM_SVC_DUR_TYPE_TITLE:
        i4_ret = SVCTXR_NOT_SUPPORT;
        break;
        
    case MM_SVC_DUR_TYPE_CHAPTER:
        i4_ret = SVCTXR_NOT_SUPPORT;
        break;
        
    case MM_SVC_DUR_TYPE_ALL:
        x_minfo_get_num_recs(h_minfo, MINFO_INFO_TYPE_GENERIC , &ui2_rec_nums);
        if(ui2_rec_nums == 0 ||ui2_rec_nums > 1 )
        {
            return SVCTXR_NOT_SUPPORT;
        }
        
        z_rec_sz = sizeof (MINFO_INFO_T);
        i4_ret = x_minfo_get_info(h_minfo, MINFO_INFO_TYPE_GENERIC, 1, &t_media_info, &z_rec_sz);
        if(i4_ret != MINFOR_OK)
        {
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
        }
        pt_time_dur->ui8_duration = t_media_info.u.t_general_info.ui8_pb_duration;
        break;
        
    case MM_SVC_DUR_TYPE_BUFFERED:
        i4_ret = SVCTXR_NOT_SUPPORT;
        break;
        
    default:
        i4_ret = SVCTXR_NOT_SUPPORT;
        break;
    }
    return i4_ret;
}

INT32 ps_uop_handler_search_fct (MEDIA_DESCRIBT_T*      pt_obj,
                                 MM_SVC_POS_INFO_T*     pt_trick_info)
{
    return SVCTXR_OK;
}

INT32 ps_uop_handler_btn_fct (MEDIA_DESCRIBT_T*     pt_obj,                          
                              MM_SVC_NAV_INFO_T*    pt_btn_info)
{   
    if(pt_obj == NULL ||pt_btn_info ==NULL)
    {
        return SVCTXR_INV_ARG;
    }
    return SVCTXR_OK;
}

INT32 ps_uop_handler_trick_fct (MEDIA_DESCRIBT_T*       pt_obj, 
                                MM_SVC_TRICK_INFO_T*    pt_trick_info)
{
    return SVCTXR_OK;
}

INT32 ps_uop_handler_get_range_fct(MEDIA_DESCRIBT_T*                pt_obj ,
                                   UINT32                           ui4_start_pts, 
                                   MIDXBULD_ELMT_TBL_ALLOC_INFO_T*  pt_range_info)
{
    pt_range_info->u_range.t_es.ui4_start_pts = 0 ;
    pt_range_info->u_range.t_es.ui4_end_pts = MIDXBULD_IDX_RANGE_FULL;
    pt_range_info->e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;

    return SVCTXR_OK;
}


