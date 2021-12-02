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
#include "handle/handle.h"

/* #include "evctx/evctx_slctr.h" */
#include "evctx/x_evctx_slctr.h" /* MHF Linux, use "x_evctx_slctr.h" in IDTV0801? -lzhang Nov/24/2009 */

#include "evctx/evctx_atsc_eng/u_evctx_atsc_eng.h"
#include "evctx/c_evctx.h"

#include "rpc.h"
#include "ri_common.h"

#include "ri_desc.h"



#define RET_ON_FAIL(stmt) \
do {\
    INT32 __i4_ret;\
    __i4_ret = (stmt);\
    if(__i4_ret != RPCR_OK)\
    {   \
        return __i4_ret;\
    }\
}while(0)


static INT32 _set_evctx_cmd_array( EVCTX_COMMAND_T *  pt_cmd_sets)
{
    UINT32 ui4_i;

    for (ui4_i = 0; pt_cmd_sets[ui4_i].e_code != EVCTX_CMD_END; ui4_i ++)
    {
        switch(pt_cmd_sets[ui4_i].e_code)
        {
            case EVCTX_CMD_SRC_INFO:
                RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                 &pt_cmd_sets[ui4_i].u, 
                                 RPC_DESC_EVCTX_COMMAND_T_u, 
                                 "u.ps_name"));    
                
                RET_ON_FAIL(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_cmd_sets[ui4_i].u.ps_name));
                break;    
            
            case EVCTX_CMD_SVL_REC:
                RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                 &pt_cmd_sets[ui4_i].u, 
                                 RPC_DESC_EVCTX_COMMAND_T_u, 
                                 "u.pt_svl_rec"));    
                RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                 &pt_cmd_sets[ui4_i].u, 
                                 RPC_DESC_SVL_REC_T, 
                                 NULL));    
                break;
            case EVCTX_CMD_TSL_REC:
                RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                 &pt_cmd_sets[ui4_i].u, 
                                 RPC_DESC_EVCTX_COMMAND_T_u, 
                                 "u.pt_tsl_rec"));    
                RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                 &pt_cmd_sets[ui4_i].u, 
                                 RPC_DESC_TSL_REC_T, 
                                 NULL));    

            break;
            default:                
                RI_LOG("Not resolved EVCTX CMD %d, val :0x%x\n", pt_cmd_sets[ui4_i].e_code, pt_cmd_sets[ui4_i].u.ps_name);
            break;
        }
    }

    RET_ON_FAIL(RPC_ADD_REF_DESC_ARR(RPC_DEFAULT_ID, 
                         ui4_i + 1, 
                         pt_cmd_sets, 
                         RPC_DESC_EVCTX_COMMAND_T, 
                         NULL));

    return RPCR_OK;
    
}

INT32 _hndlr_x_evctx_nfy_fct(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    VOID *       pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{  
    if(ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }   
    
    
    pt_return->e_type   = ARG_TYPE_VOID;
    
    ((x_evctx_nfy_fct)pv_cb_addr)(
            (HANDLE_T)pt_args[0].u.ui4_arg,
        (EVCTX_COND_T)pt_args[1].u.ui4_arg,
                      pt_args[2].u.pv_arg,
                      pt_args[3].u.ui4_arg);

    return RPCR_OK;
}


static INT32 _load_current_event(
    EVCTX_COMMAND_T*    pt_svc_cmd,
    VOID*               pv_nfy_tag,
    x_evctx_nfy_fct     pf_nfy,
    HANDLE_T*           ph_event,
    EVCTX_COND_T*       pe_cond)
{
    RPC_DECL(5, INT32);    
    
    RPC_CHECK(_set_evctx_cmd_array(pt_svc_cmd));
    
    RPC_ARG_INP(ARG_TYPE_REF_DESC,   pt_svc_cmd);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,   pv_nfy_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,   pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_event);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,  pe_cond);

    RPC_DO_OP("x_evctx_load_current_event");

    RPC_RETURN(ARG_TYPE_INT32, EVCTXR_SVC_UNAVAIL);
	
}

INT32 c_evctx_load_current_event (
                    EVCTX_COMMAND_T*    pt_svc_cmd,
                    VOID*               pv_nfy_tag,
                    x_evctx_nfy_fct     pf_nfy,
                    HANDLE_T*           ph_event,
                    EVCTX_COND_T*       pe_cond)
{
    INT32 i4_res;
    i4_res = _load_current_event(pt_svc_cmd,
                                 pv_nfy_tag,
                                 pf_nfy,
                                 ph_event,
                                 pe_cond);
     
    return i4_res;

}

static INT32 _load_next_event(
    EVCTX_COMMAND_T*    pt_svc_cmd,
    VOID*               pv_nfy_tag,
    x_evctx_nfy_fct     pf_nfy,
    HANDLE_T*           ph_event,
    EVCTX_COND_T*       pe_cond)
{
    RPC_DECL(5, INT32);    

    RPC_CHECK(_set_evctx_cmd_array(pt_svc_cmd));
    
    
    RPC_ARG_INP(ARG_TYPE_REF_DESC,   pt_svc_cmd);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,   pv_nfy_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,   pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_event);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,  pe_cond);

    RPC_DO_OP("x_evctx_load_next_event");

    RPC_RETURN(ARG_TYPE_INT32, EVCTXR_SVC_UNAVAIL);
	
}

INT32 c_evctx_load_next_event (
                    EVCTX_COMMAND_T*    pt_svc_cmd,
                    VOID*               pv_nfy_tag,
                    x_evctx_nfy_fct     pf_nfy,
                    HANDLE_T*           ph_event,
                    EVCTX_COND_T*       pe_cond)
{
    INT32     i4_res;

    i4_res = _load_next_event(pt_svc_cmd,
                              pv_nfy_tag,
                              pf_nfy,
                              ph_event,
                              pe_cond);
        

    return i4_res;
}

static INT32 _load_schedule_event(
    EVCTX_COMMAND_T*    pt_svc_cmd,
    EVCTX_OFFSET_T*     pt_offset,
    VOID*               pv_nfy_tag,
    x_evctx_nfy_fct     pf_nfy,
    HANDLE_T*           ph_event,
    EVCTX_COND_T*       pe_cond)
{
    RPC_DECL(6, INT32);    
    
    RPC_CHECK(_set_evctx_cmd_array(pt_svc_cmd));
    
    
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_offset,  RPC_DESC_EVCTX_OFFSET_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_REF_DESC,   pt_svc_cmd);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,   pt_offset);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,   pv_nfy_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,   pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_event);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,  pe_cond);

    RPC_DO_OP("x_evctx_load_schedule_event");

    RPC_RETURN(ARG_TYPE_INT32, EVCTXR_SVC_UNAVAIL);
	
}

INT32 c_evctx_load_schedule_event (
                    EVCTX_COMMAND_T*    pt_svc_cmd,
                    EVCTX_OFFSET_T*     pt_offset,
                    VOID*               pv_nfy_tag,
                    x_evctx_nfy_fct     pf_nfy,
                    HANDLE_T*           ph_event,
                    EVCTX_COND_T*       pe_cond)
{
    INT32     i4_res;

    i4_res = _load_schedule_event(pt_svc_cmd,
                                  pt_offset,
                                  pv_nfy_tag,
                                  pf_nfy,
                                  ph_event,
                                  pe_cond);

    return i4_res;
}


INT32 c_evctx_delete_event (
                    HANDLE_T            h_event)
{
    RPC_DECL(1, INT32);    
        
    RPC_ARG_INP(ARG_TYPE_UINT32, h_event);

    RPC_DO_OP("x_evctx_delete_event");

    RPC_RETURN(ARG_TYPE_INT32, EVCTXR_SVC_UNAVAIL);    
}


INT32 c_evctx_get_cond (
                    HANDLE_T            h_event,
                    EVCTX_COND_T*       pe_cond)
{
    RPC_DECL(2, INT32);    
            
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_event);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pe_cond);

    RPC_DO_OP("x_evctx_get_cond");

    RPC_RETURN(ARG_TYPE_INT32, EVCTXR_SVC_UNAVAIL);        
}

INT32 ri_event_key_add_ref_desc(
    EVCTX_KEY_TYPE_T    e_key_type, 
    VOID*               pv_key_info,
    UINT32              ui4_arg_idx)
{
    switch(e_key_type)
    {
        case EVCTX_KEY_TYPE_IDX_EVENT_TITLE:
        case EVCTX_KEY_TYPE_IDX_EVENT_TEXT:
        case EVCTX_KEY_TYPE_IDX_EVENT_EXT_TEXT:
        case EVCTX_KEY_TYPE_IDX_RATING:
        /*case EVCTX_KEY_TYPE_IDX_EVENT_EXT_ITEM_TEXT:*/
            RET_ON_FAIL(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, ui4_arg_idx, ARG_TYPE_REF_DESC));
            RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID,
                                         pv_key_info, 
                                         RPC_DESC_EVCTX_IDX_LANG_T, 
                                         NULL)); 
            RET_ON_FAIL(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID,
                                         ((EVCTX_IDX_LANG_T *)pv_key_info)->pt_lang, 
                                         sizeof(ISO_639_LANG_T)));   
            break;
            
        case EVCTX_KEY_TYPE_IDX_EVENT_CATEGORY:
        /*case EVCTX_KEY_TYPE_IDX_EVENT_GROUP:
        case EVCTX_KEY_TYPE_IDX_COMPONENT:
        case EVCTX_KEY_TYPE_IDX_AUDIO_COMPONENT:*/
            RET_ON_FAIL(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, ui4_arg_idx, ARG_TYPE_UINT16));
            break;

        case EVCTX_KEY_TYPE_EVENT_TITLE:     
        case EVCTX_KEY_TYPE_EVENT_TEXT:
        case EVCTX_KEY_TYPE_EVENT_EXT_TEXT:
        case EVCTX_KEY_TYPE_RATING:
        case EVCTX_KEY_TYPE_SVC_TEXT:
        case EVCTX_KEY_TYPE_SVC_NAME:
            RET_ON_FAIL(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, ui4_arg_idx, ARG_TYPE_REF_STR));
            break;
        case EVCTX_KEY_TYPE_ENG_SPECIFIC:            
            RET_ON_FAIL(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, ui4_arg_idx, ARG_TYPE_REF_DESC));
            /*Fix this ,only for ATSC */
            RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID,
                                       pv_key_info, 
                                       RPC_DESC_EVCTX_ATSC_KEY_INFO_T, 
                                       NULL)); 
            RET_ON_FAIL(RPC_ADD_REF_VAL(RPC_DEFAULT_ID,
                                      &((EVCTX_ATSC_KEY_INFO_T *)pv_key_info)->pv_atsc_key_info));   
            break;
        default:
            RI_ERR("[ RPC INTERFACE ] Unsupported e_key_type %d in wrapper \n", e_key_type);
            RET_ON_FAIL(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, ui4_arg_idx, ARG_TYPE_REF_VOID));
            break;
    }
    return RPCR_OK;
}


INT32 c_evctx_get_event_info_len (
                    HANDLE_T            h_event,
                    EVCTX_KEY_TYPE_T    e_key_type,
                    VOID*               pv_key_info,
                    SIZE_T*             pz_event_info_len,
                    EVCTX_COND_T*       pe_cond)
{
    BOOL b_log = rpcu_tl_log_start(); 
    RPC_DECL(5, INT32);    

    ri_event_key_add_ref_desc(e_key_type,  pv_key_info, 2);
    
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_event);
    RPC_ARG_INP(ARG_TYPE_UINT32,     e_key_type);
    RPC_ARG_INP(ARG_TYPE_VARIABLE,   pv_key_info);
    RPC_ARG_OUT(ARG_TYPE_REF_SIZE_T, pz_event_info_len);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,  pe_cond);

    RPC_DO_OP("x_evctx_get_event_info_len");
    rpcu_tl_log_end(b_log);
    RPC_RETURN(ARG_TYPE_INT32, EVCTXR_SVC_UNAVAIL);    
}



INT32 c_evctx_get_event_info (
                    HANDLE_T            h_event,
                    EVCTX_KEY_TYPE_T    e_key_type,
                    VOID*               pv_key_info,
                    SIZE_T*             pz_event_info_len,
                    VOID*               pv_event_info,
                    EVCTX_COND_T*       pe_cond)
{
    
    RPC_DECL(6, INT32);    

    ri_event_key_add_ref_desc(e_key_type,  pv_key_info, 2);

    if(pz_event_info_len)
    {   
        if(e_key_type == EVCTX_KEY_TYPE_RATING)
        {/*
             * Add this desc as size of pv_envent_info and we need add description for the type of 
             * EVCTX_RATING_LIST_T. Because the server will fill some new meta data into this buffer. 
             * RPC will do the address fixing if the address in the buffer range.
             * But currently new meta list data cannot linked automaticly. FIxme.
             */
            memset(pv_event_info, 0, *pz_event_info_len);
			 
            RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_event_info, *pz_event_info_len));
        }
        else
        {
            RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_event_info, *pz_event_info_len));
        }
    }
    else
    {
        RPC_FAIL;
    }
    
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_event);
    RPC_ARG_INP(ARG_TYPE_UINT32,     e_key_type);
    RPC_ARG_INP(ARG_TYPE_VARIABLE,   pv_key_info);
    RPC_ARG_IO (ARG_TYPE_REF_SIZE_T, pz_event_info_len);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,   pv_event_info);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,  pe_cond);

    RPC_DO_OP("x_evctx_get_event_info");

    /* support for EVCTX_RATING_LIST_T */
    if(e_key_type == EVCTX_KEY_TYPE_RATING)
    {
    	SIZE_T 	z_event_info_len = 0;
		SIZE_T 	z_temp = 0;
		UINT8* 	pui1_data;
		EVCTX_RATING_LIST_T* pt_list;

		pt_list = (EVCTX_RATING_LIST_T *)pv_event_info;

		pui1_data = (UINT8*)pv_event_info;
		
		do
		{		
			z_temp = 0;
			
			pt_list->ui2_rating_region = (UINT16)*(pui1_data + z_event_info_len);
			pt_list->ui2_rated_dimensions = (UINT16)*(pui1_data + 2 + z_event_info_len);

			/*Rebuild the pt_ratings*/
			if(pt_list->ui2_rated_dimensions != 0)
			{
				pt_list->pt_ratings = (EVCTX_RATING_T*)(pui1_data + 16 + z_event_info_len);
			}
			else
			{
				pt_list->pt_ratings = NULL;
			}

			/*Rebuild the ps_rating_descrption */
			if(pt_list->ps_rating_descrption != NULL)
			{
				pt_list->ps_rating_descrption = (CHAR*)(pui1_data + 16 + z_event_info_len +	pt_list->ui2_rated_dimensions*4);
				z_temp = strlen(pt_list->ps_rating_descrption)*sizeof(CHAR) + 1;
				z_temp = ((z_temp + 3) / 4) * 4;
			}		

			/*Rebuild the pt_next*/
			z_event_info_len = z_event_info_len + 16 + pt_list->ui2_rated_dimensions * 4 + z_temp;

			if((SIZE_T)*pz_event_info_len > z_event_info_len + sizeof(EVCTX_RATING_LIST_T))
			{
				pt_list->pt_next = (EVCTX_RATING_LIST_T*)(pui1_data + z_event_info_len);	
				pt_list = pt_list->pt_next;
			}
			else
			{
				pt_list->pt_next = NULL;
			}
		}while((SIZE_T)*pz_event_info_len > z_event_info_len);
    }
    
    RPC_RETURN(ARG_TYPE_INT32, EVCTXR_SVC_UNAVAIL);  

}


INT32 c_evctx_get_brief_info_len (
                    HANDLE_T            h_event,
                    ISO_639_LANG_T*     pt_iso_639_lang,
                    SIZE_T*             pz_brief_info_len,
                    EVCTX_COND_T*       pe_cond)
{
    RPC_DECL(4, INT32);    

                
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_event);
    RPC_ARG_INP(ARG_TYPE_REF_STR,    pt_iso_639_lang);
    RPC_ARG_IO (ARG_TYPE_REF_SIZE_T, pz_brief_info_len);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,  pe_cond);

    RPC_DO_OP("x_evctx_get_brief_info_len");

    RPC_RETURN(ARG_TYPE_INT32, EVCTXR_SVC_UNAVAIL);  
}


INT32 c_evctx_get_brief_info (
                    HANDLE_T            h_event,
                    ISO_639_LANG_T*     pt_iso_639_lang,
                    SIZE_T*             pz_brief_info_len,
                    VOID*               pv_brief_info,
                    EVCTX_COND_T*       pe_cond)
{
    RPC_DECL(5, INT32);    
    
    if(pz_brief_info_len)
    {    
        RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_brief_info, *pz_brief_info_len));
    }
    else
    {
        RPC_FAIL;
    }    
                
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_event);    
    RPC_ARG_INP(ARG_TYPE_REF_STR,    pt_iso_639_lang);
    RPC_ARG_IO (ARG_TYPE_REF_SIZE_T, pz_brief_info_len);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,   pv_brief_info);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,  pe_cond);

    RPC_DO_OP("x_evctx_get_brief_info");

    RPC_RETURN(ARG_TYPE_INT32, EVCTXR_SVC_UNAVAIL);  
}


INT32 c_evctx_set_country_code (
                    BRDCST_TYPE_T       e_brdcst_type,
                    ISO_3166_COUNT_T*   pt_country_code,
                    UINT16              ui2_region_id)
{
    RPC_DECL(3, INT32);    
    
                
    RPC_ARG_INP(ARG_TYPE_UINT8,      e_brdcst_type);
    RPC_ARG_INP(ARG_TYPE_REF_STR,    pt_country_code);
    RPC_ARG_INP(ARG_TYPE_UINT16,     ui2_region_id);

    RPC_DO_OP("x_evctx_set_country_code");

    RPC_RETURN(ARG_TYPE_INT32, EVCTXR_SVC_UNAVAIL);      
   
}


INT32 c_rpc_reg_evctx_cb_hndlrs()
{
    RPC_REG_CB_HNDLR(x_evctx_nfy_fct);    
    return RPCR_OK;
}


