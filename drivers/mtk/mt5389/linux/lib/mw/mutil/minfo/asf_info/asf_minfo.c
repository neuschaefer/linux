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
 * asf_minfo.c,v
 * 1.1.2.1 1.0
 * $Date: 2012/06/28 $ 2004/11/26
 * willyhuang
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 0a0e5908fac0e73885ce39efe82bcd61 $ Asa
 *
 * Description: 
 *         This file contains all the media info public APIs  
           Author: lianming mcn07123 
           History:
           (1)2008-3-10  lianming mcn07123  initial

 *---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

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
#include "dbg/x_dbg.h"

#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif

#include "util/x_lnk_list.h"
#include "asf_minfo_parser.h"
#include "asf_minfo.h"
#include "dbg/u_dbg.h"
#include "mutil/mm_util_input.h"
#include "asf_wmv8_intra.h"
#include "drm/x_drm.h"
#include "unicode/x_uc_str.h"

#else

#include "x_os.h"
#include "x_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "../u_minfo.h"
#include "../minfo.h"
#include "x_dbg.h"

#include "../../mm_util.h"

#include "x_lnk_list.h"
#include "asf_minfo_parser.h"
#include "asf_minfo.h"
#include "u_dbg.h"
#include "../../mm_util_input.h"
#include "asf_wmv8_intra.h"
#include "x_drm.h"
#include "x_uc_str.h"

#endif

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
 * Name:  asf_minfo_is_handler
 *
 * Description: This function confirm media format could be handled or not.
 *
 * Inputs:  ps_path_name specify the path name of the file
 *          pv_obj       specify the media info object  
 *
 * Outputs: pv_obj       modify data in the media info object  
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation 
 *                       has been completed.
 *          Other non-zero value :unsuccessful
 ----------------------------------------------------------------------------*/
INT32 asf_minfo_is_handler(VOID* pv_obj, MEDIA_FORMAT_T* pt_format)
{

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL) ||  (pt_format == NULL))
    {
        return MINFOR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/    
    if ((MEDIA_TYPE_CONTAINER == pt_format->e_media_type) 
        && (MEDIA_CONTNR_SUBTYPE_ASF 
            == pt_format->t_media_subtype.u.e_contnr_subtype))
    {
       return MINFOR_OK;
    }
    else
    {
       return MINFOR_FMT_NOT_SUPPORT;
    }
}

/*-----------------------------------------------------------------------------
 * Name:  asf_minfo_fopen
 *
 * Description: This function open a handler for a file
 *
 * Inputs:  ps_path_name  specify the path name of the file
 *          pv_obj        specify the media info object  
 *
 * Outputs: pv_obj       modify data in the media info object  
 *
 * Returns: MINFOR_OK    Success. The routine was successful and 
                         the operation has been completed.
           other non-zero value :unsuccessful

 * Author : lianming lin(mcn07123)
 *
 * History:
    (1)2008-3-12 : initial

 ----------------------------------------------------------------------------*/
INT32 asf_minfo_fopen(CHAR*  ps_path_name,  VOID* pv_obj)
{
    MINFO_OBJ_T*    pt_minfo_obj;

    DBG_INFO(("asf_minfo_fopen:begin to open asf\r\n"));
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL) || (ps_path_name == NULL))
    {
        DBG_ERROR(("asf_minfo_fopen:NULL\r\n"));
        return MINFOR_INV_ARG;
    }

    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj = (MINFO_OBJ_T*) pv_obj;
    if (pt_minfo_obj->pv_handler_obj)
    {
        _mm_util_mem_free(pt_minfo_obj->pv_handler_obj);
        pt_minfo_obj->pv_handler_obj = NULL;
    }

    pt_minfo_obj->pv_handler_obj = _mm_util_mem_alloc
                                   (sizeof(MINFO_ASF_INSTANCE_INFO_T));
    if (NULL == pt_minfo_obj->pv_handler_obj)
    {
        DBG_ERROR(("asf_minfo_fopen:pv_handler_obj allocate memory"
                   "(%d) failed\r\n",
                   sizeof(MINFO_ASF_INSTANCE_INFO_T)));
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(pt_minfo_obj->pv_handler_obj, 0, sizeof(MINFO_ASF_INSTANCE_INFO_T));


    /*temp buffer used to store parse data , should be verified later. */
    ((MINFO_ASF_INSTANCE_INFO_T*)(pt_minfo_obj->pv_handler_obj))->pui1_header_buf 
      = (UINT8*)_mm_util_mem_alloc(sizeof(UINT8) * ASF_MINFO_INIT_BUFF_LEN);
    if (NULL == ((MINFO_ASF_INSTANCE_INFO_T*)
                (pt_minfo_obj->pv_handler_obj))->pui1_header_buf)
    {
        DBG_ERROR(("asf_minfo_fopen:pui1_header_buf allocate memory failed\r\n"));
        _mm_util_mem_free(pt_minfo_obj->pv_handler_obj);
        pt_minfo_obj->pv_handler_obj = NULL;
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(((MINFO_ASF_INSTANCE_INFO_T*)
              (pt_minfo_obj->pv_handler_obj))->pui1_header_buf, 
              0, sizeof(UINT8) * ASF_MINFO_INIT_BUFF_LEN);
    
    ((MINFO_ASF_INSTANCE_INFO_T*)(pt_minfo_obj->pv_handler_obj))->ui4_buf_length
               = sizeof(UINT8) * ASF_MINFO_INIT_BUFF_LEN;
    
    DBG_INFO(("asf_minfo_fopen:end asf openning\r\n"));
 
    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  asf_minfo_mopen
 *
 * Description: This function open a handler for a memory buffer
 *
 * Inputs:  pv_mem_data  specify the address of the memory buffer
 *          z_mem_leng   specify the length of the buffer
 *          pv_obj       specify the media info object  
 *
 * Outputs: pv_obj       modify data in the media info object  
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation 
 *                       has been completed.
 *          other non-zero value :unsuccessful
 * Author : lianming lin(mcn07123)
 *
 * History:
 *   (1)2008-3-12 : initial

 ----------------------------------------------------------------------------*/
INT32 asf_minfo_mopen( VOID*    pv_mem_data, 
                       SIZE_T   z_mem_leng, 
                       VOID*    pv_obj)
{
    MINFO_OBJ_T*  pt_minfo_obj;
    INT32         i4_ret;
    UINT64        ui8_clip_pos;

    DBG_INFO(("asf_minfo_mopen:begin to open asf\r\n"));
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL) || (pv_mem_data == NULL))
    {
        DBG_ERROR(("asf_minfo_mopen:NULL\r\n"));
        return MINFOR_INV_ARG;
    }

    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj = (MINFO_OBJ_T*) pv_obj;

    /*set input functions again for asf litter endian*/
    if (z_mem_leng != 0)
    {
        pt_minfo_obj->t_input_fct_tbl.pf_copybytes = mm_util_mcopybytes_fct;
        pt_minfo_obj->t_input_fct_tbl.pf_input1 = mm_util_minput1bytes_fct;
        pt_minfo_obj->t_input_fct_tbl.pf_input2 = mm_util_minput2bytes_l_fct;
        pt_minfo_obj->t_input_fct_tbl.pf_input3 = mm_util_minput3bytes_l_fct;
        pt_minfo_obj->t_input_fct_tbl.pf_input4 = mm_util_minput4bytes_l_fct;
        pt_minfo_obj->t_input_fct_tbl.pf_set_pos = mm_util_msetpos_fct;
        pt_minfo_obj->t_input_fct_tbl.pf_get_pos = mm_util_mgetpos_fct;
    }
    else
    {
        MEDIA_FEEDER_T* pt_src_feeder; 
        pt_src_feeder = (MEDIA_FEEDER_T*)pv_mem_data;
        pt_minfo_obj->h_src_feeder = pt_src_feeder->h_feeder;
        pt_minfo_obj->t_input_fct_tbl.pf_copybytes = 
            pt_src_feeder->t_feeder_le_fct_tbl.pf_copybytes;
        pt_minfo_obj->t_input_fct_tbl.pf_input1 = 
            pt_src_feeder->t_feeder_le_fct_tbl.pf_input1;
        pt_minfo_obj->t_input_fct_tbl.pf_input2 = 
            pt_src_feeder->t_feeder_le_fct_tbl.pf_input2;
        pt_minfo_obj->t_input_fct_tbl.pf_input3 = 
            pt_src_feeder->t_feeder_le_fct_tbl.pf_input3;
        pt_minfo_obj->t_input_fct_tbl.pf_input4 = 
            pt_src_feeder->t_feeder_le_fct_tbl.pf_input4;
        pt_minfo_obj->t_input_fct_tbl.pf_set_pos = 
            pt_src_feeder->t_feeder_le_fct_tbl.pf_set_pos;
        pt_minfo_obj->t_input_fct_tbl.pf_get_pos = 
            pt_src_feeder->t_feeder_le_fct_tbl.pf_get_pos;

        /*To get clip size*/
        i4_ret = pt_minfo_obj->t_input_fct_tbl.pf_set_pos(pt_src_feeder, 
                                                          0, 
                                                          MEDIA_SEEK_END);
        if (MINFOR_OK != i4_ret)
        {
            DBG_ERROR(("asf_minfo_mopen:set error\r\n"));
            return MINFOR_INITIAL_ERR;
        }
        /*get the end position*/
        pt_minfo_obj->t_input_fct_tbl.pf_get_pos(pt_src_feeder, &ui8_clip_pos);
        pt_minfo_obj->z_buff_leng = (SIZE_T)ui8_clip_pos;
        
    }

    /*allocate asf handler memory*/
    if (pt_minfo_obj->pv_handler_obj)
    {
        _mm_util_mem_free(pt_minfo_obj->pv_handler_obj);
        pt_minfo_obj->pv_handler_obj = NULL;
    }

    pt_minfo_obj->pv_handler_obj = _mm_util_mem_alloc
                                   (sizeof(MINFO_ASF_INSTANCE_INFO_T));
    if (NULL == pt_minfo_obj->pv_handler_obj)
    {
        DBG_ERROR(("asf_minfo_mopen:pv_handler_obj allocate memory"
                   "(%d) failed\r\n",
                   sizeof(MINFO_ASF_INSTANCE_INFO_T)));
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(pt_minfo_obj->pv_handler_obj, 0, sizeof(MINFO_ASF_INSTANCE_INFO_T));


    /*temp buffer used to store parse data , should be verified later. */
    ((MINFO_ASF_INSTANCE_INFO_T*)(pt_minfo_obj->pv_handler_obj))->pui1_header_buf 
      = (UINT8*)_mm_util_mem_alloc(sizeof(UINT8) * ASF_MINFO_INIT_BUFF_LEN);
    if (NULL == ((MINFO_ASF_INSTANCE_INFO_T*)
                (pt_minfo_obj->pv_handler_obj))->pui1_header_buf)
    {
        DBG_ERROR(("asf_minfo_mopen:pui1_header_buf allocate memory failed\r\n"));
        _mm_util_mem_free(pt_minfo_obj->pv_handler_obj);
        pt_minfo_obj->pv_handler_obj = NULL;
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(((MINFO_ASF_INSTANCE_INFO_T*)
              (pt_minfo_obj->pv_handler_obj))->pui1_header_buf, 
              0, sizeof(UINT8) * ASF_MINFO_INIT_BUFF_LEN);
    
    ((MINFO_ASF_INSTANCE_INFO_T*)(pt_minfo_obj->pv_handler_obj))->ui4_buf_length
               = sizeof(UINT8) * ASF_MINFO_INIT_BUFF_LEN;
    
    DBG_INFO(("asf_minfo_mopen:end asf openning\r\n"));
 
    return MINFOR_OK;
}    

/*-----------------------------------------------------------------------------
 * Name:  asf_minfo_close
 *
 * Description: This function close a handler
 *
 * Inputs:  pv_obj       specify the media info object  
 *
 * Outputs: NULL
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation 
 *                        has been completed.
 *          other non-zero value :unsuccessful
 * Author : lianming lin(mcn07123)
 *
 * History:
 *   (1)2008-3-12 : initial

 ----------------------------------------------------------------------------*/
INT32 asf_minfo_close( VOID* pv_obj )
{
    MINFO_OBJ_T*                pt_minfo_obj;
    MINFO_ASF_INSTANCE_INFO_T*  pt_asf_instance_obj;  

    DBG_INFO(("asf_minfo_close:begin to close asf\r\n"));
   
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pv_obj == NULL)  
    {
        DBG_ERROR(("asf_minfo_close:NULL\r\n"));
        return MINFOR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj = (MINFO_OBJ_T*)pv_obj;
    pt_asf_instance_obj = (MINFO_ASF_INSTANCE_INFO_T*) 
                           pt_minfo_obj->pv_handler_obj;
    if (NULL == pt_asf_instance_obj)
    {
        DBG_ERROR(("asf_minfo_close:asf_instance_obj=NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }

    
    /*free dynamic resource which store all parsered information, this resources 
    are allocated during parsing*/
    (VOID)_asf_free_dynamic_resource(pt_minfo_obj);

    /*free temp buffer*/
    if (pt_asf_instance_obj->pui1_header_buf != NULL)
    {
        _mm_util_mem_free(pt_asf_instance_obj->pui1_header_buf);
        pt_asf_instance_obj->pui1_header_buf  = NULL;
    }
    
    /*free object handler*/
    if (pt_asf_instance_obj)
    {
        _mm_util_mem_free(pt_asf_instance_obj);
        pt_asf_instance_obj = NULL;
    }
    
    DBG_INFO(("asf_minfo_close:end asf closing\r\n"));
   
    return MINFOR_OK;
}    

/*-----------------------------------------------------------------------------
 * Name:  asf_minfo_parse
 *
 * Description: This function start parsing a asf file
 *
 * Inputs:  pv_obj       specify the media info object  
 *
 * Outputs: 
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation
 *                        has been completed.
 *          other non-zero value :unsuccessful
 * Author : lianming lin(mcn07123)
 *
 * History:
 *   (1)2008-3-12 : initial


 ----------------------------------------------------------------------------*/
INT32 asf_minfo_parse( VOID* pv_obj)
{

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pv_obj == NULL)
    {
        DBG_ERROR(("asf_minfo_parse:NULL\r\n"));
        return MINFOR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /* parse asf */
    return asf_minfo_object_parse((MINFO_OBJ_T*)pv_obj);

} 

/*-----------------------------------------------------------------------------
 * Name:  asf_minfo_get_num_recs
 *
 * Description:This function get information from a handler
 *
 * Inputs:  pv_obj       specify the media info object  
 *          e_info_type  
 *
 * Outputs: pui2_num_recs
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation 
 *                        has been completed.
 *          other non-zero value :unsuccessful
 * Author : lianming lin(mcn07123)
 *
 * History:
 *   (1)2008-3-12 : initial
 ----------------------------------------------------------------------------*/
INT32 asf_minfo_get_num_recs( VOID*                pv_obj, 
                              MINFO_INFO_TYPE_T    e_info_type, 
                              UINT16*              pui2_num_recs )
{
     MINFO_OBJ_T*               pt_minfo_obj;
     MINFO_ASF_INSTANCE_INFO_T* pt_asf_handler;
    
     /************************************************************************/
     /* Check parameter                                                      */
     /************************************************************************/
     if ((NULL == pv_obj ) || (NULL == pui2_num_recs ))
     {
        DBG_ERROR(("asf_minfo_get_num_recs:NULL\r\n"));
        return MINFOR_INV_ARG;
     }
     
     /************************************************************************/
     /* do                                                                   */
     /************************************************************************/
     pt_minfo_obj = (MINFO_OBJ_T*) pv_obj;
     pt_asf_handler = (MINFO_ASF_INSTANCE_INFO_T*) pt_minfo_obj->pv_handler_obj;
    
     if (pt_asf_handler)
     {
         /*whether parser is done? if not can not return info*/
         if ( FALSE == pt_minfo_obj->b_parsing_successful )
         {
            DBG_ERROR(("asf_minfo_get_num_recs:minfo is not ready\r\n"));
             return MINFOR_INFO_NOT_READY;    
         }
         
         switch(e_info_type)
         {
             case MINFO_INFO_TYPE_GENERIC:
                *pui2_num_recs = 1;  /*only one*/
                 break;                
             case MINFO_INFO_TYPE_VIDEO_ATTR:
                #ifdef __EMBEDED_STREAM_PROPER_OBJ__
                *pui2_num_recs = pt_asf_handler->t_all_strm_info.ui1_video_strm_num 
                 + pt_asf_handler->t_all_strm_info.ui1_hidden_video_strm_num;
                #else
                *pui2_num_recs = pt_asf_handler->t_all_strm_info.ui1_video_strm_num;
                #endif

                 break;
             case MINFO_INFO_TYPE_AUDIO_ATTR:
                #ifdef __EMBEDED_STREAM_PROPER_OBJ__
                *pui2_num_recs = pt_asf_handler->t_all_strm_info.ui1_audio_strm_num 
                + pt_asf_handler->t_all_strm_info.ui1_hidden_audio_strm_num;
                #else
                *pui2_num_recs = pt_asf_handler->t_all_strm_info.ui1_audio_strm_num;
                #endif
                 break;
             case MINFO_INFO_TYPE_SP_ATTR:
                 *pui2_num_recs = 0;
                 break;
             case MINFO_INFO_TYPE_IDX_INFO:
                 *pui2_num_recs = 1;
                 break;
             case MINFO_INFO_TYPE_META_DATA:
                 *pui2_num_recs = 1;
                 break;
             default:
              return MINFOR_INFO_NOT_FOUND;
         }
         return MINFOR_OK;
     }
     
     return MINFOR_FMT_NOT_SUPPORT;
     
}
                        

/*-----------------------------------------------------------------------------
 * Name:  asf_minfo_get_info
 *
 * Description:This function get information from a handler
 *
 * Inputs:  pv_obj    specify the media info object  
 *           e_info_type
 *          ui2_index
 * Outputs: pv_buf   output media stream infomation
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation 
 *                        has been completed.
 *          other non-zero value :unsuccessful
 * Author : lianming lin(mcn07123)
 *
 * History:
 *   (1)2008-3-12 : initial

 ----------------------------------------------------------------------------*/
INT32 asf_minfo_get_info( 
                            VOID*               pv_obj, 
                            MINFO_INFO_TYPE_T   e_info_type, 
                            MINFO_REC_IDX_T     ui2_index,                    
                            VOID*               pv_buf, 
                            SIZE_T*             pz_buf_leng)
{
    MINFO_OBJ_T*                       pt_minfo_obj;
    MINFO_ASF_INSTANCE_INFO_T*         pt_asf_handler;
    MINFO_INFO_T*                      pt_minfo_rec;
    MINFO_ASF_ALL_STRM_INFO_T*         pt_all_strm_info;   
    INT32                              i4_ret;

    MINFO_ASF_VIDEO_STM_ATRBT_T*       pt_asf_video_strm_attr;
    SeqHeaderInfo                      t_header_info;
    MINFO_TYPE_META_DATA_T*            pt_meta_data;
    ASF_CONTENT_DESCRIPTION_OBJECT_T*  pt_content_des;
    ASF_FILE_PROPERTIES_OBJECT_T*      pt_file_object;
    MINFO_ASF_PER_STRM_INFO_T*         p_strm_info_head;
    ASF_EX_STREAM_PROPERTIES_OBJECT_T* pt_ex_stream_obj;
    ASF_STREAM_PROPERTIES_OBJECT_T*    pt_stream_obj;
    /*Data object*/
    ASF_DATA_OBJECT_T*                 pt_data_object;
    UINT64                             ui8_create_tm;
    UINT64                             ui8_dividend;
    /*video es info*/
    BOOL                               b_get_vid_info = FALSE;
    MINFO_ASF_PER_VID_STRM_INF_T*      pt_vid_strm_inf;
    MINFO_TYPE_ID3_TAG_T*              pt_id3_tag_info; /* output */
    UINT64                             ui8_duration = 0;
    ASF_EXT_CONTENT_DES_OBJECT_T*      pt_ext_content_des;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((NULL == pv_obj) || (NULL == pz_buf_leng))
    {
        DBG_ERROR(("asf_minfo_get_info:NULL\r\n"));
        return MINFOR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_minfo_obj = (MINFO_OBJ_T*) pv_obj;
    pt_asf_handler = (MINFO_ASF_INSTANCE_INFO_T*)(pt_minfo_obj->pv_handler_obj);
    if (NULL == pt_asf_handler)
    {
        DBG_ERROR(("asf_minfo_get_info:pt_asf_handler NULL\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    
    if (pv_buf == NULL)
    {   
        /*get length only*/
        *pz_buf_leng = sizeof (MINFO_INFO_T);
        return MINFOR_OK;
    }
    else
    {
        /* get data */
        if (*pz_buf_leng < sizeof (MINFO_INFO_T))
        {
            DBG_ERROR(("ERROR: asf_minfo_get_info - insufficient buffer \r\n"));  
            return MINFOR_INSUFFICIENT_BUFFER;
        }
    }    
    pt_minfo_rec = (MINFO_INFO_T*)pv_buf;
    /*x_memset(pt_minfo_rec, 0, sizeof(MINFO_INFO_T));*/
    
    pt_all_strm_info = &(pt_asf_handler->t_all_strm_info);
    /* begin to get data */
    if (NULL != pt_asf_handler)
    {
        if (FALSE == pt_minfo_obj->b_parsing_successful)
        {
            DBG_ERROR(("asf_minfo_get_info:minfo is not ready\r\n"));
            return MINFOR_INFO_NOT_READY;    
        }
        /*x_memset(pt_minfo_rec, 0, sizeof(MINFO_INFO_T));*/

        x_memcpy((VOID *)&(pt_minfo_rec->t_format), 
                 (VOID *)&(pt_minfo_obj->t_media_format), 
                 sizeof(MEDIA_FORMAT_T));
        
        pt_minfo_rec->e_info_type = e_info_type;
        pt_minfo_rec->ui1_rec_idx = (UINT8)ui2_index;
        
        switch (e_info_type)
        {
            case MINFO_INFO_TYPE_GENERIC:
                i4_ret = _asf_get_generic_info(pt_minfo_obj, pt_minfo_rec);
                if (MINFOR_OK!= i4_ret)
                {
                    DBG_ERROR(("asf_minfo_get_info:error for generic\r\n"));
                    return MINFOR_INTERNAL_ERR;    
                }
                *pz_buf_leng = sizeof(MINFO_INFO_T);
                break;
                
            case MINFO_INFO_TYPE_VIDEO_ATTR:
                #ifdef __EMBEDED_STREAM_PROPER_OBJ__
                if (ui2_index > (pt_all_strm_info->ui1_video_strm_num  
                    + pt_all_strm_info->ui1_hidden_video_strm_num) )
                #else
                if (ui2_index > pt_all_strm_info->ui1_video_strm_num)
                #endif
                {
                    DBG_ERROR(("asf_minfo_get_info:exceeds error,%d, %d\r\n",
                               ui2_index, 
                               pt_all_strm_info->ui1_video_strm_num
                               - 1));
                    return MINFOR_OUT_OF_RECORD;
                }
                i4_ret = _asf_get_strm_atrbt_by_idx(pt_all_strm_info, 
                                                    ASF_STRM_VIDEO_TYPE,
                                                    ui2_index,
                                                    pt_minfo_rec);
                if (MINFOR_OK!= i4_ret)
                {
                    DBG_ERROR(("asf_minfo_get_info:error for video\r\n"));
                    return MINFOR_INTERNAL_ERR;    
                }
                
                /* HW not support WMV2 x8intra, call driver functon to verify it */
                pt_asf_video_strm_attr = 
                      &(pt_minfo_rec->u.t_stm_attribute.u.t_asf_video_stm_attr);
                t_header_info.u1Addr = pt_asf_video_strm_attr->pui1_codec_info;
                t_header_info.u4Len = pt_asf_video_strm_attr->ui4_codec_info_size;

                pt_minfo_rec->u.t_stm_attribute.b_is_codec_support = 
                             fgDecoderSupport(pt_asf_video_strm_attr->e_enc ,
                                              &t_header_info);
                                                           
                *pz_buf_leng = sizeof(MINFO_INFO_T);
                break;
                
            case MINFO_INFO_TYPE_AUDIO_ATTR:
                #ifdef __EMBEDED_STREAM_PROPER_OBJ__
                if (ui2_index > (pt_all_strm_info->ui1_audio_strm_num 
                     + pt_all_strm_info->ui1_hidden_audio_strm_num))
                #else
                if (ui2_index > pt_all_strm_info->ui1_audio_strm_num)
                #endif
                {
                    DBG_ERROR(("asf_minfo_get_info:exceeds error,%d, %d\r\n",
                               ui2_index, 
                               pt_all_strm_info->ui1_audio_strm_num 
                               - 1));
                    return MINFOR_OUT_OF_RECORD;
                }
                i4_ret = _asf_get_strm_atrbt_by_idx(pt_all_strm_info, 
                                                    ASF_STRM_AUDIO_TYPE,
                                                    ui2_index,
                                                    pt_minfo_rec);
                if (MINFOR_OK!= i4_ret)
                {
                    DBG_ERROR(("asf_minfo_get_info:error for audio\r\n"));
                    return MINFOR_INTERNAL_ERR;    
                }
                /*set language*/
                if (pt_asf_handler->t_language.ui2_lang_count != 0)
                {
                    x_memcpy(pt_minfo_rec->u.t_stm_attribute.u.t_asf_audio_stm_attr.s_lang,
                             pt_asf_handler->t_language.pt_languge[pt_asf_handler->t_language.ui2_lang_count-1],
                             ISO_639_LANG_LEN);
                    #ifndef __KERNEL__
                    x_dbg_stmt("!!!!audio attribute language %s\n", 
                                pt_minfo_rec->u.t_stm_attribute.u.t_asf_audio_stm_attr.s_lang);
                    #endif
                }
                
                /*set aduio stream codec support true by default */
                if (MINFO_INFO_AUD_ENC_UNKNOWN
                    != pt_minfo_rec->u.t_stm_attribute.u.t_asf_audio_stm_attr.e_enc)
                {
                    pt_minfo_rec->u.t_stm_attribute.b_is_codec_support = TRUE;
                }
                else
                {
                    pt_minfo_rec->u.t_stm_attribute.b_is_codec_support = FALSE;
                }
                *pz_buf_leng = sizeof(MINFO_INFO_T);
                break;
                
            case MINFO_INFO_TYPE_IDX_INFO:                    
                    if(pt_minfo_obj->t_media_format.e_media_type == MEDIA_TYPE_AUDIO_ES &&
                       pt_minfo_obj->t_media_format.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_WMA)
                    {
                        pt_file_object = &(pt_asf_handler->t_file_object);
                        
                        ui8_duration = 
                            _mm_div64(pt_file_object->ui8_play_duration * 9, 1000, NULL);
                        if(ui8_duration > pt_file_object->ui8_preroll * 90)
                        {
                            ui8_duration -= pt_file_object->ui8_preroll * 90;
                        }
                        else
                        {
                            ui8_duration = 0;   
                        }
                        if(ui8_duration <= (ASF_WMA_KEY_TBL_ENTRY_MAX_CNT * 90000)) /*no more than 1000 seconds*/
                        {
                            pt_asf_handler->t_idx_info.ui4_idx_type = MM_MINFO_IDX_TYPE_COMMON;
                            pt_asf_handler->t_idx_info.ui8_idx_offset = pt_asf_handler->t_idx_info.ui8_data_offset;
                            pt_asf_handler->t_idx_info.ui8_idx_sz = pt_asf_handler->t_idx_info.ui8_data_sz;
                        }
                    }
                    else if (pt_asf_handler->t_file_object.ui8_file_size 
                        != pt_minfo_obj->z_buff_leng)
                    {
                        /*If file size retrieved from file object is different 
                                              from the value gotten by file system API,then consider
                                              the file is corrupted and set index_table flag as false,
                                              2008-10-17*/
                        pt_asf_handler->t_idx_info.ui4_idx_type = MM_MINFO_IDX_TYPE_NONE;
                    }
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
                    if (pt_minfo_obj->t_media_format.e_media_type == MEDIA_TYPE_CONTAINER &&
                        pt_minfo_obj->t_media_format.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_ASF)
                    {
                        if (!pt_asf_handler->b_is_bad_interleaved)
                        {
                            pt_asf_handler->t_idx_info.ui4_1st_pkgnum = 0;
                        }
                        else if (pt_asf_handler->t_bd_pkt.b_met_all_strms)
                        {
                            pt_asf_handler->t_idx_info.ui4_1st_pkgnum = pt_asf_handler->t_bd_pkt.ui4_first_met_pkt;
                        }
                        else
                        {
                            pt_asf_handler->t_idx_info.ui4_1st_pkgnum = 4000;
                        }
                    }
#endif
                    x_memcpy((VOID *)&(pt_minfo_rec->u.t_idx_info),
                             (VOID *)&(pt_asf_handler->t_idx_info),
                             sizeof(MINFO_TYPE_IDX_INFO_T));
                    *pz_buf_leng = sizeof(MINFO_INFO_T);
                    break;
            case MINFO_INFO_TYPE_DRM_INFO:
                pt_minfo_rec->u.t_drm_info.t_minfo_drm_type = DRM_TYPE_NONE;
                break;
            /*case MINFO_INFO_TYPE_SP_ATTR:
                            break;*/
            case MINFO_INFO_TYPE_META_DATA:
                pt_meta_data = &(pt_minfo_rec->u.t_meta_data);
                /*x_memset(pt_meta_data, 0, sizeof(MINFO_TYPE_META_DATA_T));*/
                pt_meta_data->t_frame_rate.ui4_scale = 1;
                pt_content_des = &(pt_asf_handler->t_content_des);
                /*get content properties*/   
                pt_ext_content_des = &(pt_asf_handler->t_ext_content_des);

                COPY_DATA(  pt_content_des->pwsz_title, 
                            x_uc_w2s_strlen(pt_content_des->pwsz_title), 
                            pt_meta_data->pwc_str_title, 
                            &(pt_meta_data->z_title_len));
                
                COPY_DATA(pt_content_des->pwsz_copyright, 
                          x_uc_w2s_strlen(pt_content_des->pwsz_copyright), 
                          pt_meta_data->pwc_str_copyright, 
                          &(pt_meta_data->z_copyright_len));
                
                if(pt_content_des->pwsz_author)
                {
                    COPY_DATA(  pt_content_des->pwsz_author, 
                                x_uc_w2s_strlen(pt_content_des->pwsz_author), 
                                pt_meta_data->pwc_str_artist, 
                                &(pt_meta_data->z_artist_len));
                }
                else
                {
                    COPY_DATA(pt_ext_content_des->pwsz_album_artist, 
                              x_uc_w2s_strlen(pt_ext_content_des->pwsz_album_artist), 
                                pt_meta_data->pwc_str_artist, 
                                &(pt_meta_data->z_artist_len));
                }
                
                COPY_DATA(  pt_content_des->pwsz_author, 
                            x_uc_w2s_strlen(pt_content_des->pwsz_author), 
                            pt_meta_data->pwc_str_director, 
                            &(pt_meta_data->z_director_len));
                
                COPY_DATA(pt_ext_content_des->pwsz_album_title, 
                          x_uc_w2s_strlen(pt_ext_content_des->pwsz_album_title), 
                            pt_meta_data->pwc_str_album, 
                            &(pt_meta_data->z_album_len));
                
                
                COPY_DATA(pt_ext_content_des->pwsz_genre, 
                          x_uc_w2s_strlen(pt_ext_content_des->pwsz_genre), 
                            pt_meta_data->pwc_str_genre, 
                            &(pt_meta_data->z_genre_len));

                COPY_DATA(pt_ext_content_des->pwsz_year, 
                          x_uc_w2s_strlen(pt_ext_content_des->pwsz_year), 
                            pt_meta_data->pwc_str_year, 
                            &(pt_meta_data->z_year_len));

                /*if creation date exist*/
                pt_file_object = &(pt_asf_handler->t_file_object);
                if (pt_file_object->ui8_create_date != 0)
                {
                    pt_meta_data->b_cr_date_exist = TRUE;
                    ui8_create_tm = _mm_div64(pt_file_object->ui8_create_date,
                                              10000000,
                                              NULL);
                    _mm_gmtime((MM_TIME_T)ui8_create_tm,
                               MM_GMT_ASF_EPOCH_YEAR_M,
                               &(pt_meta_data->t_date));
                    
                }
                pt_meta_data->ui8_pb_duration = 
                    _mm_div64(pt_file_object->ui8_play_duration * 9, 1000, NULL);
                if(pt_meta_data->ui8_pb_duration > pt_file_object->ui8_preroll * 90)
                {
                    pt_meta_data->ui8_pb_duration -= pt_file_object->ui8_preroll * 90;
                }
                else
                {
                    pt_meta_data->ui8_pb_duration = 0;   
                }
                
                /*frame rate*/
                p_strm_info_head = pt_all_strm_info->p_strm_info_head;
                while (NULL != p_strm_info_head)
                {
                    pt_stream_obj = p_strm_info_head->pt_stream_obj;
                    if (NULL == pt_stream_obj)
                    {
                        DBG_ERROR(("asf_minfo_get_info:Exception!!!\r\n"));
                    }
                    if (ASFR_GUID_EQUAL 
                        != _asf_cmp_guid(&(pt_stream_obj->t_strm_type), 
                                         &gt_asf_object_stream_type_video))
                    {
                        p_strm_info_head = p_strm_info_head->pt_next_strm;
                        continue;
                    }
                    
                    pt_ex_stream_obj = p_strm_info_head->pt_ex_stream_obj;
                    if (NULL != pt_ex_stream_obj)
                    {
                        if (pt_ex_stream_obj->ui8_average_time_per_frame != 0)
                        {
                            /*0x2540BE400 = 1000*10000000*/
                            ui8_dividend = 0x2;
                            ui8_dividend <<= 32;
                            ui8_dividend += 0x540BE400;
                            pt_meta_data->t_frame_rate.ui4_rate = (UINT32)
                                _mm_div64(ui8_dividend, 
                                          pt_ex_stream_obj->ui8_average_time_per_frame,
                                          NULL);
                            pt_meta_data->t_frame_rate.ui4_scale = 1000;
                        }
                    }

                    /*video es info*/
                    if(p_strm_info_head && !b_get_vid_info)
                    {
                        pt_vid_strm_inf = (MINFO_ASF_PER_VID_STRM_INF_T*)
                                            p_strm_info_head->pv_type_info;
                    
                        pt_meta_data->t_video_es_info.i4_video_inf_width = 
                            pt_vid_strm_inf->t_vid_strm_atrbt.i4_width;
                        pt_meta_data->t_video_es_info.i4_video_inf_height = 
                            pt_vid_strm_inf->t_vid_strm_atrbt.i4_height;

                        b_get_vid_info = TRUE;
                    }
                    
                    p_strm_info_head = p_strm_info_head->pt_next_strm;
                }
                /*If no extented stream properties object set frame rate 0*/
                if (1000 != pt_meta_data->t_frame_rate.ui4_scale)
                {
                    pt_meta_data->t_frame_rate.ui4_rate  = 0;
                    pt_meta_data->t_frame_rate.ui4_scale = 1000;
                }

                /*calculate bitrate*/
                pt_data_object = &(pt_asf_handler->t_data_object);
                if (pt_meta_data->ui8_pb_duration != 0)
                {
                    pt_meta_data->ui4_bitrate = (UINT32)
                        _mm_div64((pt_data_object->ui8_size - 50) * 8 * 90000, 
                                   pt_meta_data->ui8_pb_duration, NULL);
                }                
                
                break;
            case MINFO_INFO_TYPE_ID3_DATA:
                pt_id3_tag_info = (MINFO_TYPE_ID3_TAG_T*)(&(pt_minfo_rec->u.t_id3_data));
                pt_content_des = &(pt_asf_handler->t_content_des);
                /*get content properties*/   
                pt_ext_content_des = &(pt_asf_handler->t_ext_content_des);

                COPY_DATA(  pt_content_des->pwsz_title, 
                            x_uc_w2s_strlen(pt_content_des->pwsz_title), 
                            pt_id3_tag_info->pwc_str_id3_title, 
                            &(pt_id3_tag_info->z_id3_title_len));
                if(pt_content_des->pwsz_author)
                {
                    COPY_DATA(  pt_content_des->pwsz_author, 
                                x_uc_w2s_strlen(pt_content_des->pwsz_author), 
                                pt_id3_tag_info->pwc_str_id3_artist, 
                                &(pt_id3_tag_info->z_id3_artist_len));
                }
                else
                {
                    COPY_DATA(pt_ext_content_des->pwsz_album_artist, 
                              x_uc_w2s_strlen(pt_ext_content_des->pwsz_album_artist), 
                                pt_id3_tag_info->pwc_str_id3_artist, 
                                &(pt_id3_tag_info->z_id3_artist_len));
                }

                COPY_DATA(pt_ext_content_des->pwsz_album_title, 
                          x_uc_w2s_strlen(pt_ext_content_des->pwsz_album_title), 
                            pt_id3_tag_info->pwc_str_id3_album, 
                            &(pt_id3_tag_info->z_id3_album_len));
                
                COPY_DATA(pt_ext_content_des->pwsz_genre, 
                          x_uc_w2s_strlen(pt_ext_content_des->pwsz_genre), 
                            pt_id3_tag_info->pwc_str_id3_genre, 
                            &(pt_id3_tag_info->z_id3_genre_len));

                COPY_DATA(pt_ext_content_des->pwsz_year, 
                          x_uc_w2s_strlen(pt_ext_content_des->pwsz_year), 
                            pt_id3_tag_info->pwc_str_id3_year, 
                            &(pt_id3_tag_info->z_id3_year_len)); 
                break;
                
            default:
             return MINFOR_INFO_NOT_FOUND;
        }
        return MINFOR_OK;
    }
    
    return MINFOR_FMT_NOT_SUPPORT;
} 

#ifdef __cplusplus
}
#endif

