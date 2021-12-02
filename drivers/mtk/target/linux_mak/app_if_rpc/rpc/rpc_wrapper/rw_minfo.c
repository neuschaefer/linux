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
#include "svctx/x_svctx.h"
#include "handle/handle.h"
#include "mutil/midxbuld/x_midxbuld.h"

#include "rpc.h"
#include "ri_common.h"
#include "ri_desc.h"


INT32 _hndlr_x_minfo_nfy_fct(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    VOID *       pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{  
    if(ui4_num_args != 5)
    {
        return RPCR_INV_ARGS;
    }
    /*Maybe not needed*/
    pt_return->e_type = ARG_TYPE_VOID;
    
    ((x_minfo_nfy_fct)pv_cb_addr)(
            (HANDLE_T)pt_args[0].u.ui4_arg,
            (MINFO_NFY_EVENT_TYPE_T)pt_args[1].u.ui4_arg,
            (VOID*)pt_args[2].u.ui4_arg,
            (VOID*)pt_args[3].u.ui4_arg,
            pt_args[4].u.pv_arg);

    return RPCR_OK;
}


INT32 c_minfo_fopen (
                        CHAR*               ps_path_name,
                        x_minfo_nfy_fct     pf_nfy,
                        VOID*               pv_tag,
                        MEDIA_FORMAT_T*     pt_format,
                        HANDLE_T*           ph_minfo)
{
    RPC_DECL(5, INT32);

    RPC_ARG_INP(ARG_TYPE_REF_STR,    ps_path_name);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,   pf_nfy);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,   pv_tag);
    RPC_ARG_IO (ARG_TYPE_REF_DESC,   pt_format);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_minfo);
    
    RPC_DO_OP("x_minfo_fopen");
    
    RPC_RETURN(ARG_TYPE_INT32, MINFOR_INV_ARG);
}

INT32 c_minfo_mopen (
                        VOID*               pv_mem_data,
                        SIZE_T              z_mem_leng,
                        x_minfo_nfy_fct     pf_nfy,
                        VOID*               pv_tag,
                        MEDIA_FORMAT_T*     pt_format,
                        HANDLE_T*           ph_minfo)
{
    RPC_DECL(6, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_format, RPC_DESC_MEDIA_FORMAT_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_REF_VOID,   pv_mem_data);
    RPC_ARG_INP(ARG_TYPE_SIZE_T,     z_mem_leng);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,   pf_nfy);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,   pv_tag);
    RPC_ARG_IO (ARG_TYPE_REF_DESC,   pt_format);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_minfo);
    
    RPC_DO_OP("x_minfo_mopen");
    
    RPC_RETURN(ARG_TYPE_INT32, MINFOR_INV_ARG);
}

INT32 c_minfo_fopen_sync (
    CHAR*              ps_path_name,
    MEDIA_FORMAT_T*    pt_format,
    HANDLE_T*          ph_minfo
)
{
    RPC_DECL(3, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_format, RPC_DESC_MEDIA_FORMAT_T, NULL));

    RPC_ARG_INP(ARG_TYPE_REF_STR,    ps_path_name);
    RPC_ARG_IO (ARG_TYPE_REF_DESC,   pt_format);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_minfo);
    
    RPC_DO_OP("x_minfo_fopen_sync");
    
    RPC_RETURN(ARG_TYPE_INT32, MINFOR_INV_ARG);
}

INT32 c_minfo_mopen_sync(
    VOID*              pv_mem_data,
    SIZE_T             z_mem_leng,
    MEDIA_FORMAT_T*    pt_format,
    HANDLE_T*          ph_minfo
)
{
    RPC_DECL(4, INT32);

    RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_mem_data, z_mem_leng));            
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_format, RPC_DESC_MEDIA_FORMAT_T, NULL));
        
    RPC_ARG_INP(ARG_TYPE_REF_VOID,   pv_mem_data);
    RPC_ARG_INP(ARG_TYPE_SIZE_T,     z_mem_leng);
    RPC_ARG_IO (ARG_TYPE_REF_DESC,   pt_format);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_minfo);
    
    RPC_DO_OP("x_minfo_mopen_sync");
    
    RPC_RETURN(ARG_TYPE_INT32, MINFOR_INV_ARG);
}

INT32 c_minfo_close (
                        HANDLE_T        h_minfo)
{
    RPC_DECL(1, INT32);

    RPC_ARG_OUT(ARG_TYPE_UINT32, h_minfo);
    
    RPC_DO_OP("x_minfo_close");
    
    RPC_RETURN(ARG_TYPE_INT32, MINFOR_INV_ARG);
}

INT32 c_minfo_get_info (
                        HANDLE_T            h_minfo,
                        MINFO_INFO_TYPE_T   e_info_type,
                        MINFO_REC_IDX_T     ui2_index,
                        VOID*               pv_buf,
                        SIZE_T*             pz_buf_leng)
{
    MINFO_INFO_T * pt_info;
    
    RPC_DECL(5, INT32);

    /* int ss = rpcu_tl_log_start();*/

    pt_info = pv_buf;
    
    switch(e_info_type)
    {
        case MINFO_INFO_TYPE_ID3_DATA:
        if(pz_buf_leng != NULL)            
        {            
            
            RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_buf, RPC_DESC_MINFO_INFO_T, NULL));
            RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, &pt_info->u, RPC_DESC_MINFO_INFO_T_u, "u.t_id3_data"));
            
			if((pt_info->u.t_id3_data.pwc_str_id3_year == NULL) &&
				(pt_info->u.t_id3_data.pwc_str_id3_title == NULL) &&
				(pt_info->u.t_id3_data.pwc_str_id3_artist == NULL) &&
				(pt_info->u.t_id3_data.pwc_str_id3_album == NULL) &&
				(pt_info->u.t_id3_data.pwc_str_id3_track == NULL) &&
				(pt_info->u.t_id3_data.pwc_str_id3_genre == NULL))
			{
				return SVCTXR_OK;
			}
			if((pt_info->u.t_id3_data.pwc_str_id3_year != NULL)&&(pt_info->u.t_id3_data.z_id3_year_len != 0))
            {
                RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_info->u.t_id3_data.pwc_str_id3_year, sizeof(UTF16_T) * pt_info->u.t_id3_data.z_id3_year_len));
            }
			if((pt_info->u.t_id3_data.pwc_str_id3_title != NULL)&&(pt_info->u.t_id3_data.z_id3_title_len != 0))
            {
                RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_info->u.t_id3_data.pwc_str_id3_title, sizeof(UTF16_T) * pt_info->u.t_id3_data.z_id3_title_len));
            }
			if((pt_info->u.t_id3_data.pwc_str_id3_artist != NULL)&&(pt_info->u.t_id3_data.z_id3_artist_len != 0))
            {
                RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_info->u.t_id3_data.pwc_str_id3_artist, sizeof(UTF16_T) * pt_info->u.t_id3_data.z_id3_artist_len));
            }
			if((pt_info->u.t_id3_data.pwc_str_id3_album != NULL)&&(pt_info->u.t_id3_data.z_id3_album_len != 0))
            {
                RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_info->u.t_id3_data.pwc_str_id3_album, sizeof(UTF16_T) * pt_info->u.t_id3_data.z_id3_album_len));
            }
			if((pt_info->u.t_id3_data.pwc_str_id3_track != NULL)&&(pt_info->u.t_id3_data.z_id3_track_len != 0))
            {
                RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_info->u.t_id3_data.pwc_str_id3_track, sizeof(UTF16_T) * pt_info->u.t_id3_data.z_id3_track_len));
            }
			if((pt_info->u.t_id3_data.pwc_str_id3_genre != NULL)&&(pt_info->u.t_id3_data.z_id3_genre_len != 0))
            {
                RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_info->u.t_id3_data.pwc_str_id3_genre, sizeof(UTF16_T) * pt_info->u.t_id3_data.z_id3_genre_len));
            }
        }
        break;
        case MINFO_INFO_TYPE_META_DATA:
        if(pz_buf_leng != NULL)            
        { 
            RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_buf, RPC_DESC_MINFO_INFO_T, NULL));
            RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, &pt_info->u, RPC_DESC_MINFO_INFO_T_u, "u.t_meta_data"));
            
			if((pt_info->u.t_meta_data.pwc_str_title == NULL) &&
				(pt_info->u.t_meta_data.pwc_str_director == NULL) &&
				(pt_info->u.t_meta_data.pwc_str_copyright == NULL) &&
				(pt_info->u.t_meta_data.pwc_str_genre == NULL) &&
				(pt_info->u.t_meta_data.pwc_str_artist == NULL) &&
				(pt_info->u.t_meta_data.pwc_str_album == NULL) &&
				(pt_info->u.t_meta_data.pwc_str_track == NULL) &&
				(pt_info->u.t_meta_data.pwc_str_year == NULL))
			{
				break;
			}
			
			if((pt_info->u.t_meta_data.pwc_str_title != NULL)&&(pt_info->u.t_meta_data.z_title_len != 0))
			{	
				RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_info->u.t_meta_data.pwc_str_title,	sizeof(UTF16_T) * pt_info->u.t_meta_data.z_title_len));
			}
			if((pt_info->u.t_meta_data.pwc_str_director != NULL)&&(pt_info->u.t_meta_data.z_director_len != 0))
			{	
				RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_info->u.t_meta_data.pwc_str_director, sizeof(UTF16_T) * pt_info->u.t_meta_data.z_director_len));
			}
			if((pt_info->u.t_meta_data.pwc_str_copyright != NULL)&&(pt_info->u.t_meta_data.z_copyright_len != 0))
			{	
				RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_info->u.t_meta_data.pwc_str_copyright,sizeof(UTF16_T) * pt_info->u.t_meta_data.z_copyright_len));
			}
			if((pt_info->u.t_meta_data.pwc_str_genre != NULL)&&(pt_info->u.t_meta_data.z_genre_len != 0))
			{	
				RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_info->u.t_meta_data.pwc_str_genre,	sizeof(UTF16_T) * pt_info->u.t_meta_data.z_genre_len));
			}
			if((pt_info->u.t_meta_data.pwc_str_artist != NULL)&&(pt_info->u.t_meta_data.z_artist_len != 0))
			{	
				RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_info->u.t_meta_data.pwc_str_artist,	sizeof(UTF16_T) * pt_info->u.t_meta_data.z_artist_len));
			}
			if((pt_info->u.t_meta_data.pwc_str_album != NULL)&&(pt_info->u.t_meta_data.z_album_len != 0))
			{	
				RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_info->u.t_meta_data.pwc_str_album,	sizeof(UTF16_T) * pt_info->u.t_meta_data.z_album_len));
			}
			if((pt_info->u.t_meta_data.pwc_str_track != NULL)&&(pt_info->u.t_meta_data.z_track_len != 0))
			{	
				RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_info->u.t_meta_data.pwc_str_track,	sizeof(UTF16_T) * pt_info->u.t_meta_data.z_track_len));
			}
			if((pt_info->u.t_meta_data.pwc_str_year != NULL)&&(pt_info->u.t_meta_data.z_year_len != 0))
			{	
				RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_info->u.t_meta_data.pwc_str_year, 	sizeof(UTF16_T) * pt_info->u.t_meta_data.z_year_len));
			}
        }
        break;
        default:
            printf("Not impled MINFO %d, %lx\n", e_info_type, (UINT32)pv_buf);
    }        
    
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_minfo);
    RPC_ARG_INP(ARG_TYPE_UINT32,     e_info_type);
    RPC_ARG_INP(ARG_TYPE_UINT16,     ui2_index);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,   pv_buf);
    RPC_ARG_IO (ARG_TYPE_REF_SIZE_T, pz_buf_leng);
    
    RPC_DO_OP("x_minfo_get_info");
    /* rpcu_tl_log_end(ss); */

    RPC_RETURN(ARG_TYPE_INT32, MINFOR_INV_ARG);
}

INT32 c_minfo_get_num_recs(
                   HANDLE_T                h_minfo,
                   MINFO_INFO_TYPE_T       e_info_type,
                   UINT16*                 pui2_num_recs)
{
    RPC_DECL(3, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_minfo);
    RPC_ARG_INP(ARG_TYPE_UINT32,     e_info_type);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16, pui2_num_recs);
    
    RPC_DO_OP("x_minfo_get_num_recs");
    
    RPC_RETURN(ARG_TYPE_INT32, MINFOR_INV_ARG);
}



INT32 c_rpc_reg_minfo_cb_hndlrs()
{
    RPC_REG_CB_HNDLR(x_minfo_nfy_fct);
    return RPCR_OK;
}




