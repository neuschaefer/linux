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
 * $RCSfile: c_cl.c,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/2 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 1ddbeefb100f7898981d8724d01691d2 $ Asa
 *
 * Description: 
 *         This file contains the interface generic APIs of GCL layer. 
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "handle/handle.h"
#include "cl/c_cl.h"
#include "cl/x_cl.h"
#include "aee/aee.h"
#include "dbg/x_dbg.h"
#include "cl/_cl.h"
#include "cl/cl_cli.h"

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
 * Name:  c_cl_open_eng
 *
 * Description: Open an engine for compression/decompression operation.
 *
 * Inputs:  ps_name     identifies sixteen characters engine name, and the character is case sensitive.
 *          pf_nfy      specify the notify callback function. 
 *          pv_tag      the argument prepared by the user, and it will send back to user in (*x_cl_nfy_fct).
 *
 * Outputs: ph_engine   The handle of the engine which want to be opened.
 *
 * Returns: CLR_OK                    The routine was successful and the operation has been completed.
 *          CLR_INV_ARG                The argument ps_name or ph_engine is NULL.
 *          CLR_ENG_NOT_FOUND        Cannot find the engine, which you want to open it.
 *          CLR_OUT_OF_MEMORY       There is insufficient memory available to allocate structure.
 *          CLR_OUT_OF_HANDLE         No more handles are available.
 ----------------------------------------------------------------------------*/
extern INT32 c_cl_open_eng (
             const CHAR*    ps_name,  
             x_cl_nfy_fct   pf_nfy,   
             VOID*          pv_tag,
             HANDLE_T*      ph_engine    
             )
{
    HANDLE_T  h_aux;    
    INT32     i4_res;       

    DBG_API(("[CL] c_cl_open_eng: ps_name:%s \t pf_nfy:%x \t pv_tag:%x \t ph_engine:%x \n\r"
        , ps_name, pf_nfy, pv_tag, ph_engine));
    
    i4_res = aee_grab_handle_resource( AEE_FLAG, 1, &h_aux);       
    if (i4_res == AEER_OK)    
    {        
        i4_res = x_cl_open_eng (ps_name, pf_nfy, pv_tag, ph_engine);
        if (i4_res == CLR_OK)        
        {            
            handle_link_to_aux(h_aux, *ph_engine);        
        }        
        else        
        {            
            aee_release_handle_resource(1);        
        }    
    }    
    else    
    {        
        i4_res = CLR_OUT_OF_MEMORY;    
    }  

    return (i4_res);
}

/*-----------------------------------------------------------------------------
 * Name:  c_cl_close
 *
 * Description: Close the entry, archive or engine after no more using
 *
 * Inputs:  h_handle    the handle of a object which you already opened.
 *
 * Outputs: -
 *
 * Returns: CLR_OK                The routine was successful and the operation has been completed.
 *          CLR_INV_ARG            The argument h_handle is a NULL_HANDLE.
 *          CLR_INV_HANDLE        The handle is not valid any more. 
 *          CLR_FCT_NOT_SUPPORT    The requested function was not supported.
 ----------------------------------------------------------------------------*/
extern INT32 c_cl_close (
             HANDLE_T           h_handle 
             )
{
    return x_cl_close(h_handle);
}

/*-----------------------------------------------------------------------------
 * Name:  c_cl_get_compress_len
 *
 * Description: This routine gets the length of compressed buffer after actually performs data compression from source buffer. 
 *
 * Inputs:  h_engine          identifies the handle of the engine which performs compression operation 
 *          pv_src_buf        point to the source buffer, and data in this buffer will be compressed. 
 *          z_src_len         specify the length of the source buffer. 
 *          ui4_param_type    specify type of compression paramters, set CL_NULL_PARAM_TYPE is using default
 *          pv_param          specify compression paramters, set NULL is using default
 *
 * Outputs: pz_dest_len contains the compressed length after source  buffer is compressed.
 *
 * Returns: CLR_OK                 The routine was successful and the operation has been completed.
 *          CLR_INV_ARG            The argument pv_src_buf or pz_dest_len contains a NULL pointer.
 *          CLR_INV_HANDLE         The handle is not valid any more.
 *          CLR_OUT_OF_MEMORY      The system runs out of memory.
 *          CLR_COMPRESS_ERROR     The routine occurred error during compression algorithm process, such as other third-party error code or the symbol not found in codebook.
 *          CLR_FCT_NOT_SUPPORT    The requested function was not supported.
 ----------------------------------------------------------------------------*/
extern INT32 c_cl_get_compress_len (
             HANDLE_T               h_engine,  
             const VOID*            pv_src_buf,
             SIZE_T                 z_src_len, 
             UINT32                 ui4_param_type,
             VOID*                  pv_param,      
             SIZE_T*                pz_dest_len
             )
{
    return x_cl_get_compress_len (h_engine, pv_src_buf, z_src_len, ui4_param_type, pv_param, pz_dest_len);
}
/*-----------------------------------------------------------------------------
 * Name:  c_cl_compress
 *
 * Description: This routine compresses data from source buffer to destination buffer.
 *
 * Inputs:  h_engine            identifies the handle of the engine, which performs compression operation.
 *          pv_src_buf          point to the source buffer, which contains source data waiting for compression.
 *          z_src_len           specify the length of the source buffer.
 *          ui4_param_type      specify type of compression paramters, set CL_NULL_PARAM_TYPE is using default
 *          pv_param            specify compression paramters, set NULL is using default
 *          pz_dest_len         specify the length of the destination buffer.
 *
 * Outputs: pv_dest_buf     point to the destination buffer.
 *          pz_dest_len     specify the really compression length of destination buffer.
 *
 * Returns: CLR_OK                     The routine was successful and the operation has been completed.
 *          CLR_INV_ARG                The argument pv_src_buf, pv_dest_buf or pz_dest_len contains a NULL pointer.
 *          CLR_INV_HANDLE             The handle is not valid any more.
 *          CLR_OUT_OF_MEMORY          The system runs out of memory.
 *          CLR_COMPRESS_ERROR         The routine occurred error during compression algorithm process, such as other third-party error code or the symbol not found in codebook.
 *          CLR_FCT_NOT_SUPPORT        The requested function was not supported. 
 *          CLR_INSUFFICIENT_BUFFER    The destination buffer is insufficient for data compression of source buffer.
 ----------------------------------------------------------------------------*/
extern INT32 c_cl_compress (
             HANDLE_T               h_engine,   
             const VOID*            pv_src_buf, 
             SIZE_T                 z_src_len,
             UINT32                 ui4_param_type,
             VOID*                  pv_param,  
             VOID*                  pv_dest_buf,
             SIZE_T*                pz_dest_len 
             )
{
    return x_cl_compress (h_engine, pv_src_buf, z_src_len, ui4_param_type, pv_param, pv_dest_buf, pz_dest_len);
}

/*-----------------------------------------------------------------------------
 * Name:  c_cl_get_decompress_len
 *
 * Description: Get the length of the decompressed buffer after really decompress data from source buffer.
 *
 * Inputs:  h_engine            identifies the handle of the engine which performs compression operation 
 *          pv_src_buf          point to the source buffer, which contains source data waiting for decompression. 
 *          z_src_len           specify the length of the source buffer.
 *          ui4_param_type      specify type of decompression paramters, set CL_NULL_PARAM_TYPE is using default
 *          pv_param            specify decompression paramters, set NULL is using default
 *
 * Outputs: pz_dest_len     contains the compressed length after source  buffer is compressed.
 *
 * Returns: CLR_OK                     The routine was successful and the operation has been completed.
 *          CLR_INV_ARG                The argument pv_src_buf or pz_dest_len contains a NULL pointer.
 *          CLR_INV_HANDLE             The handle is not valid any more.
 *          CLR_OUT_OF_MEMORY          The system runs out of memory.
 *          CLR_FCT_NOT_SUPPORT        The requested function was not supported.
 *          CLR_DECOMPRESS_ERROR       The routine occurred error during decompression algorithm process, such as the bit stream is damage.
 ----------------------------------------------------------------------------*/
extern INT32 c_cl_get_decompress_len (
             HANDLE_T                 h_engine,   
             const VOID*              pv_src_buf, 
             SIZE_T                   z_src_len,  
             UINT32                   ui4_param_type,
             VOID*                    pv_param, 
             SIZE_T*                  pz_dest_len 
             )
{
    return x_cl_get_decompress_len (h_engine, pv_src_buf, z_src_len, ui4_param_type, pv_param, pz_dest_len);
}

/*-----------------------------------------------------------------------------
 * Name:  c_cl_decompress
 *
 * Description: Decompress data from source buffer to destination buffer.
 *
 * Inputs:  h_engine            identifies the handle of the engine which performs compression operation 
 *          pv_src_buf          point to the source buffer. 
 *          z_src_len           specify the length of the source buffer. 
 *          z_skip_len          indicate the number of bytes to skip from the beginning of decompression output.
 *          ui4_param_type      specify type of decompression paramters, set CL_NULL_PARAM_TYPE is using default
 *          pv_param            specify decompression paramters, set NULL is using default
 *          pz_dest_len         specify the length of destination buffer.
 *          ph_prev_state       set NULL if one shot;specify previous decompress state
 *
 * Outputs: pv_dest_buf     point to the destination buffer. 
 *          pz_dest_len     specify the really decompression length of destination buffer.
 *          ph_prev_state   next time decompress state
 *
 * Returns: CLR_OK                   The routine was successful and the operation has been completed.
 *          CLR_INV_ARG              The argument pv_src_buf, pv_dest_buf or pz_dest_len contains a NULL pointer.
 *          CLR_INV_HANDLE           The handle is not valid any more.
 *          CLR_OUT_OF_MEMORY        The system runs out of memory.
 *          CLR_FCT_NOT_SUPPORT      The requested function was not supported.
 *          CLR_DECOMPRESS_ERROR     The routine occurred error during decompression algorithm process, such as the bit stream is damage.
 ----------------------------------------------------------------------------*/

extern INT32 c_cl_decompress (
             HANDLE_T       h_engine,   
             const VOID*    pv_src_buf, 
             SIZE_T         z_src_len,  
             SIZE_T         z_skip_len,
             UINT32         ui4_param_type,
             VOID*          pv_param, 
             VOID*          pv_dest_buf, 
             SIZE_T*        pz_dest_len,
             HANDLE_T*      ph_prev_state
             )
{
    return x_cl_decompress (h_engine, pv_src_buf, z_src_len, z_skip_len, ui4_param_type, pv_param, pv_dest_buf, pz_dest_len, ph_prev_state);
}
extern INT32 c_cl_decompress_ex (
             HANDLE_T       h_engine,   
             const VOID*    pv_src_buf, 
             SIZE_T         z_src_len,  
             SIZE_T         z_skip_len,
             UINT32         ui4_param_type,
             VOID*          pv_param, 
             VOID*          pv_dest_buf, 
             SIZE_T*        pz_dest_len,
             HANDLE_T*      ph_prev_state
             )
{
    return x_cl_decompress_ex (h_engine, pv_src_buf, z_src_len, z_skip_len, ui4_param_type, pv_param, pv_dest_buf, pz_dest_len, ph_prev_state);
}

/*-----------------------------------------------------------------------------
 * Name:  c_cl_open_archive
 *
 * Description: This routine creates and opens an archive for later entry operation.
 *
 * Inputs:  h_engine    specified the handle of an engine.
 *          ps_name     specified the path name string of the archive. 
 *          ui1_mode    specified the access permit mode, and only support CL_READ_MODE.
 *          pf_nfy      specify the notify callback function. When ph_archive is not valid any more, the callback function pf_nfy will be called. 
 *          pv_tag      the argument prepared by the user, and it will send back to user in (*x_cl_nfy_fct). 
 *
 * Outputs: ph_archive  retrieve the archive handle.
 *
 * Returns: CLR_OK                    The routine was successful and the operation has been completed.
 *          CLR_INV_ARG                The argument ps_path or ppv_harchive contains a NULL pointer or ui1_mode is outside of definition.
 *          CLR_INV_HANDLE             The handle is invalid.
 *          CLR_OUT_OF_MEMORY        The system runs out of memory.
 *          CLR_BAD_ARCHIVE            The archive is corrupted.
 *          CLR_OUT_OF_HANDLE         No more handles are available.
 *          CLR_FCT_NOT_SUPPORT        The requested function was not supported.
 *          CLR_FILE_OR_DIR_MISSING    The input arguments of the file or directory name are invalid.
 ----------------------------------------------------------------------------*/
extern INT32 c_cl_open_archive (
             HANDLE_T       h_engine,  
             const CHAR*    ps_name,   
             UINT8          ui1_mode,  
             x_cl_nfy_fct   pf_nfy,    
             VOID*          pv_tag,
             HANDLE_T*      ph_archive     
             )
{
    HANDLE_T  h_aux;    
    INT32     i4_res;       

    DBG_API(("[CL] c_cl_open_archive: h_engine:%x \t ps_name:%s \t ui1_mode:%x \t pf_nfy:%x \t pv_tag:%x \t ph_archive:%x \n\r"
        , h_engine, ps_name, ui1_mode, pf_nfy, pv_tag, ph_archive)); 
    i4_res = aee_grab_handle_resource( AEE_FLAG, 1, &h_aux);       
    if (i4_res == AEER_OK)    
    {        
        i4_res = x_cl_open_archive (h_engine, ps_name, ui1_mode, pf_nfy, pv_tag, ph_archive);
        if (i4_res == CLR_OK)        
        {            
            handle_link_to_aux(h_aux, *ph_archive);        
        }        
        else        
        {            
            aee_release_handle_resource(1);        
        }    
    }    
    else    
    {        
        i4_res = CLR_OUT_OF_MEMORY;    
    }  

    return (i4_res);
}

/*-----------------------------------------------------------------------------
 * Name:  c_cl_open_entry_by_name
 *
 * Description: This routine creates or opens an entry, which has the path name string ps_name in the archive.
 *
 * Inputs:  h_archive   the handle of the archive. 
 *          ps_name     specified the path name string of the entry which will be opened. 
 *          pf_nfy      specify the notify callback function. When ph_entry is not valid any more, the callback function pf_nfy will be called. 
 *          pv_tag      the argument prepared by the user, and it will send back to user in (*x_cl_nfy_fct). 
 *
 * Outputs: ph_entry    retrieve the handle of the entry.
 *
 * Returns: CLR_OK                        The routine was successful and the operation has been completed.
 *          CLR_INV_ARG                    The argument pv_harchive, ps_name and ppv_hentry contain a NULL pointer.
 *          CLR_OUT_OF_MEMORY            The system runs out of memory.
 *          CLR_INV_HANDLE                The handle is not valid any more.
 *          CLR_BAD_ARCHIVE                The archive is corrupted.
 *          CLR_FILE_OR_DIR_MISSING        The input arguments of the file or directory name are invalid.
 *          CLR_OUT_OF_HANDLE             No more handles are available.
 *          CLR_FCT_NOT_SUPPORT            The requested function was not supported.
 ----------------------------------------------------------------------------*/
extern INT32 c_cl_open_entry_by_name (
             HANDLE_T       h_archive, 
             const CHAR*    ps_name,   
             x_cl_nfy_fct   pf_nfy,    
             VOID*          pv_tag,
             HANDLE_T*      ph_entry      
             )
{
    HANDLE_T  h_aux;    
    INT32     i4_res;       

    DBG_API(("[CL] c_cl_open_entry_by_name: h_archive:%x \t ps_name:%s \t pf_nfy:%x \t pv_tag:%x \t ph_entry:%x \n\r"
        , h_archive, ps_name, pf_nfy, pv_tag, ph_entry)); 
    i4_res = aee_grab_handle_resource( AEE_FLAG, 1, &h_aux);       
    if (i4_res == AEER_OK)    
    {        
        i4_res = x_cl_open_entry_by_name (h_archive, ps_name, pf_nfy, pv_tag, ph_entry);
        if (i4_res == CLR_OK)        
        {            
            handle_link_to_aux(h_aux, *ph_entry);        
        }        
        else        
        {            
            aee_release_handle_resource(1);        
        }    
    }    
    else    
    {        
        i4_res = CLR_OUT_OF_MEMORY;    
    } 
    
    return (i4_res);
}

/*-----------------------------------------------------------------------------
 * Name:  c_cl_open_entry_by_index
 *
 * Description: This routine opens an entry in the archive by index of entry numbers.
 *
 * Inputs:  h_archive       the handle of the archive. 
 *          ui4_entry_index specified the index of the entry which will be opened, and it is 0 base.
 *          pf_nfy          specify the notify callback function. When ph_entry is not valid any more, the callback function pf_nfy will be called.
 *          pv_tag          the argument prepared by the user, and it will send back to user in (*x_cl_nfy_fct).
 *
 * Outputs: ph_entry    retrieve the handle of the entry. 
 *
 * Returns: CLR_OK                The routine was successful and the operation has been completed.
 *          CLR_INV_ARG            The argument pv_harchive and ppv_hentry contain a NULL pointer or ui4_entry_index is out of entry number range.
 *          CLR_OUT_OF_MEMORY    The system runs out of memory.
 *          CLR_BAD_ARCHIVE        The archive is corrupted.
 *          CLR_INV_HANDLE        The handle is not valid any more.
 *          CLR_OUT_OF_HANDLE     No more handles are available.
 *          CLR_FCT_NOT_SUPPORT    The requested function was not supported.
 ----------------------------------------------------------------------------*/
extern INT32 c_cl_open_entry_by_index (
             HANDLE_T       h_archive,       
             UINT32         ui4_entry_index, 
             x_cl_nfy_fct   pf_nfy,          
             VOID*          pv_tag,
             HANDLE_T*      ph_entry           
             )
{
    HANDLE_T  h_aux;    
    INT32     i4_res;       

    DBG_API(("[CL] c_cl_open_entry_by_index: h_archive:%x \t ui4_entry_index:%x \t pf_nfy:%x \t pv_tag:%x \t ph_entry:%x \n\r"
        , h_archive, ui4_entry_index, pf_nfy, pv_tag, ph_entry)); 
    i4_res = aee_grab_handle_resource( AEE_FLAG, 1, &h_aux);       
    if (i4_res == AEER_OK)    
    {        
        i4_res = x_cl_open_entry_by_index (h_archive, ui4_entry_index, pf_nfy, pv_tag, ph_entry);
        if (i4_res == CLR_OK)        
        {            
            handle_link_to_aux(h_aux, *ph_entry);        
        }        
        else        
        {            
            aee_release_handle_resource(1);        
        }    
    }    
    else    
    {        
        i4_res = CLR_OUT_OF_MEMORY;    
    }  
    return (i4_res);
}

/*-----------------------------------------------------------------------------
 * Name:  c_cl_decompress_entry
 *
 * Description: This Routine will read and decompress data from the current entry file to the destination buffer, after open an entry in the archive.
 *
 * Inputs:  h_entry         the handle of the entry.
 *          z_skip_len      indicate the number of Bytes which shall be skipped before data shall be actually written to the destination buffer.
 *          pz_dest_len     specified the length of the destination buffer.
 *
 * Outputs:  pv_dest      the destination buffer that contains reading data.
 *           pz_dest_len  specify the really decompression length of destination buffer.
 *
 * Returns: CLR_OK                    The routine was successful and the operation has been completed.
 *          CLR_INV_ARG                The argument pv_dest or pz_dest_len contain a NULL pointer.
 *          CLR_INV_HANDLE            The handle is not valid any more.
 *          CLR_DECOMPRESS_ERROR    The routine occurred error during decompression algorithm process, such as the bit stream is damage.
 *          CLR_INSUFFICIENT_BUFFER    The destination buffer is insufficient for data decompression.
 *          CLR_OUT_OF_MEMORY        The system runs out of memory.
 *          CLR_FCT_NOT_SUPPORT        The requested function was not supported.
 ----------------------------------------------------------------------------*/
extern INT32 c_cl_decompress_entry (
             HANDLE_T       h_entry,        
             SIZE_T         z_skip_len, 
             VOID*          pv_dest,
             SIZE_T*        pz_dest_len
             )
{
    return x_cl_decompress_entry (h_entry, z_skip_len, pv_dest, pz_dest_len);
}

/*-----------------------------------------------------------------------------
 * Name:  c_cl_get_decompress_entry_len
 *
 * Description: This Routine will get the length of the current entry file except z_skip_len.
 *
 * Inputs:  h_entry       the handle of the entry.
 *
 * Outputs: pz_dest_len    specify the actual length, which is needed to decompress the entry data.
 *
 * Returns: CLR_OK                The routine was successful and the operation has been completed.
 *          CLR_INV_ARG            The argument pz_dest_len contain a NULL pointer.
 *          CLR_INV_HANDLE        The handle is not valid any more.
 *          CLR_FCT_NOT_SUPPORT    The requested function was not supported.
 ----------------------------------------------------------------------------*/
extern INT32 c_cl_get_decompress_entry_len (
             HANDLE_T       h_entry,    
             SIZE_T*        pz_dest_len 
             )
{
    return x_cl_get_decompress_entry_len (h_entry, pz_dest_len);
}

/*-----------------------------------------------------------------------------
 * Name:  c_cl_query_info
 *
 * Description: This API will query information of the archive.
 *
 * Inputs:  h_archive        the handle of the archive.
 *          e_type           specified the type of the query operation.
 *          pv_filter        specified the querying filter to query the archive.
 *          pz_query_buf_len specified the length of the query buffer.
 *
 * Outputs: pv_query_buf        retrieve the query information in the buffer.
 *          pz_query_buf_len    specified the actual size of query information needed.
 *
 * Returns: CLR_OK                    The routine was successful and the operation has been completed.
 *          CLR_INV_ARG                The argument pv_query_buf, pv_filter or pz_query_buf_len contain a NULL pointer or e_type is out of range.
 *          CLR_INV_HANDLE            The handle is not valid any more
 *          CLR_INSUFFICIENT_BUFFER    The destination buffer is insufficient for data querying.
 *          CLR_FCT_NOT_SUPPORT        The requested function was not supported.
 ----------------------------------------------------------------------------*/
extern INT32 c_cl_query_info (
             HANDLE_T               h_archive,       
             CL_QUERY_INFO_TYPE_T   e_type,          
             VOID*                  pv_filter,       
             VOID*                  pv_query_buf,    
             SIZE_T*                pz_query_buf_len 
             )
{
    return x_cl_query_info (h_archive, e_type, pv_filter,  pv_query_buf, pz_query_buf_len);
}

/*-----------------------------------------------------------------------------
 * Name:  c_cl_get_query_info_len
 *
 * Description: This routine will get the length of query information of the archive
 *
 * Inputs:  h_archive    the handle of the archive.
 *          e_type       specified the type of the query operation.
 *          pv_filter    specified the querying filter to query the archive.
 *
 * Outputs: pz_query_buf_len    specified the actual size of query information needed.
 *
 * Returns: CLR_OK                The routine was successful and the operation has been completed.
 *          CLR_INV_ARG            The argument pv_filter or pz_query_buf_len contain a NULL pointer or e_type is out of range.
 *          CLR_INV_HANDLE        The handle is not valid any more.
 *          CLR_FCT_NOT_SUPPORT    The requested function was not supported.
 ----------------------------------------------------------------------------*/

extern INT32 c_cl_get_query_info_len (
             HANDLE_T               h_archive,       
             CL_QUERY_INFO_TYPE_T   e_type,          
             VOID*                  pv_filter,       
             SIZE_T*                pz_query_buf_len 
             )
{
    return x_cl_get_query_info_len (h_archive, e_type, pv_filter, pz_query_buf_len);
}

/*-----------------------------------------------------------------------------
 * Name:  c_cl_get_num_entries
 *
 * Description: This API will query information of the archive.
 *
 * Inputs:  h_archive          the handle of the archive.
 *
 * Outputs: pui4_num_entris    retrieve the number of entries
 *
 * Returns: CLR_OK              The routine was successful and the operation has been completed.
 *            CLR_INV_ARG         The argument pui4_num_entries contains a NULL pointer.            
 *            CLR_INV_HANDLE      The handle h_archive is not valid any more.            
 *            CLR_FCT_NOT_SUPPORT    The requested function was not supported.
 ----------------------------------------------------------------------------*/

extern INT32 c_cl_get_num_entries (
             HANDLE_T   h_archive,       
             UINT32*    pui4_num_entris 
             )
{
    return x_cl_get_num_entries (h_archive, pui4_num_entris);
}
