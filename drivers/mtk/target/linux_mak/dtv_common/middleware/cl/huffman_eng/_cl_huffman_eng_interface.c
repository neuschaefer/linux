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
 * $RCSfile: _cl_huffman_eng_interface.c,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/5 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 0a75cf3461eaf7d1bd31f7cbb1aa52f2 $ Asa
 *
 * Description: 
 *         This file contains all interface APIs of Huffman engine. 
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "cl/x_cl.h"
#include "cl/_cl.h"

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
extern INT32 _cl_huffman_decode(CL_HUFFMAN_TYPE_T e_huffman_type, const VOID* pv_src_buf,SIZE_T z_src_len, VOID* pv_dest_buf, SIZE_T z_skip_len, SIZE_T* pz_dest_len);
/*-----------------------------------------------------------------------------
 * Name:  _cl_prog_title_decode
 *
 * Description: This API will use PROGRAM_TITLE table to decode data  
 *              from source buffer to destination buffer
 *
 * Inputs:  e_decompress_type   specify the type of "decompress" function. Please refer to section 6.2.4         
 *          pv_src_buf          point to the source buffer, which contains source data waiting for decompression
 *          z_src_len           specify the length of the source buffer.
 *          ui4_param_type      specify type of decompression paramters, set CL_NULL_PARAM_TYPE is using default
 *          pv_param            specify decompression paramters, set NULL is using default
 *          z_skip_len          indicate the number of bytes to skip from the beginning of decompression output. 
 *          ph_prev_state       set NULL if one shot;specify previous decompress state
 *
 * Outputs: pv_dest_buf         point to the destination buffer.
 *          pz_dest_len         specify the really decompression length of destination buffer.
 *          ph_prev_state   next time decompress state
 *
 * Returns: CLR_OK                      Success.
 *          CLR_INV_ARG                 FAIL. Invalid arguments.
 *          CLR_OUT_OF_MEMORY           FAIL. The system is out of memory.
 *          CLR_FCT_NOT_SUPPORT         FAIL. This function is not supported.
 *          CLR_DECOMPRESS_ERROR        FAIL. The routine occurred error 
 *                                      during decompression algorithm process.
 *          CLR_INSUFFICIENT_BUFFER     FAIL. The destination buffer is not enough.
 ----------------------------------------------------------------------------*/

extern INT32 _cl_prog_title_decode (
            CL_ENG_DECOMPRESS_TYPE_T    e_decompress_type,
            VOID*                       pv_src_buf, 
            SIZE_T                      z_src_len, 
            UINT32                      ui4_param_type,  
            VOID*                       pv_param,
            VOID*                       pv_dest_buf, 
            SIZE_T                      z_skip_len, 
            SIZE_T*                     pz_dest_len,
            HANDLE_T*                   ph_prev_state
            )
{
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( (pv_dest_buf == NULL) ||
        (pz_dest_len == NULL))
    {
        return CLR_INV_ARG;
    }
    /************************************************************************/
    /* Perform decode algorithm                                             */
    /************************************************************************/
    if (ph_prev_state)
    {
        *ph_prev_state = NULL_HANDLE;
    }
    switch(e_decompress_type)
    {
    case CL_DECOMPRESS_TYPE:
        return _cl_huffman_decode(CL_PROGRAM_TITLE_TYPE, pv_src_buf, z_src_len, pv_dest_buf, z_skip_len, pz_dest_len);
    case CL_DECOMPRESS_ENTRY_TYPE:
        return CLR_FCT_NOT_SUPPORT;
    default:
        return CLR_INV_ARG;
    }
}

extern INT32 _cl_prog_dvbt_table1_decode (
            CL_ENG_DECOMPRESS_TYPE_T    e_decompress_type,
            VOID*                       pv_src_buf, 
            SIZE_T                      z_src_len, 
            UINT32                      ui4_param_type,  
            VOID*                       pv_param,
            VOID*                       pv_dest_buf, 
            SIZE_T                      z_skip_len, 
            SIZE_T*                     pz_dest_len,
            HANDLE_T*                   ph_prev_state
            )
{
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( (pv_dest_buf == NULL) ||
        (pz_dest_len == NULL))
    {
        return CLR_INV_ARG;
    }
    /************************************************************************/
    /* Perform decode algorithm                                             */
    /************************************************************************/
    if (ph_prev_state)
    {
        *ph_prev_state = NULL_HANDLE;
    }
    switch(e_decompress_type)
    {
    case CL_DECOMPRESS_TYPE:
        return _cl_huffman_decode(CL_PROGRAM_DVBT_TABLE1_TYPE, pv_src_buf, z_src_len, pv_dest_buf, z_skip_len, pz_dest_len);
    case CL_DECOMPRESS_ENTRY_TYPE:
        return CLR_FCT_NOT_SUPPORT;
    default:
        return CLR_INV_ARG;
    }
}


extern INT32 _cl_prog_dvbt_table2_decode (
            CL_ENG_DECOMPRESS_TYPE_T    e_decompress_type,
            VOID*                       pv_src_buf, 
            SIZE_T                      z_src_len, 
            UINT32                      ui4_param_type,  
            VOID*                       pv_param,
            VOID*                       pv_dest_buf, 
            SIZE_T                      z_skip_len, 
            SIZE_T*                     pz_dest_len,
            HANDLE_T*                   ph_prev_state
            )
{
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( (pv_dest_buf == NULL) ||
        (pz_dest_len == NULL))
    {
        return CLR_INV_ARG;
    }
    /************************************************************************/
    /* Perform decode algorithm                                             */
    /************************************************************************/
    if (ph_prev_state)
    {
        *ph_prev_state = NULL_HANDLE;
    }
    switch(e_decompress_type)
    {
    case CL_DECOMPRESS_TYPE:
        return _cl_huffman_decode(CL_PROGRAM_DVBT_TABLE2_TYPE, pv_src_buf, z_src_len, pv_dest_buf, z_skip_len, pz_dest_len);
    case CL_DECOMPRESS_ENTRY_TYPE:
        return CLR_FCT_NOT_SUPPORT;
    default:
        return CLR_INV_ARG;
    }
}


/*-----------------------------------------------------------------------------
 * Name:  _cl_prog_desc_decode
 *
 * Description: This API will use PROGRAM_DESCRIPTION table to decode data  
 *              from source buffer to destination buffer
 *
 * Inputs:  e_decompress_type   specify the type of "decompress" function. Please refer to section 6.2.4         
 *          pv_src_buf          point to the source buffer, which contains source data waiting for decompression
 *          z_src_len           specify the length of the source buffer.
 *          ui4_param_type      specify type of decompression paramters, set CL_NULL_PARAM_TYPE is using default
 *          pv_param            specify decompression paramters, set NULL is using default
 *          z_skip_len          indicate the number of bytes to skip from the beginning of decompression output. 
 *          ph_prev_state       set NULL if one shot;specify previous decompress state
 *
 * Outputs: pv_dest_buf         point to the destination buffer.
 *          pz_dest_len         specify the really decompression length of destination buffer.
 *          ph_prev_state       next time decompress state
 *
 * Returns: CLR_OK                      Success.
 *          CLR_INV_ARG                 FAIL. Invalid arguments.
 *          CLR_OUT_OF_MEMORY           FAIL. The system is out of memory.
 *          CLR_FCT_NOT_SUPPORT         FAIL. This function is not supported.
 *          CLR_DECOMPRESS_ERROR        FAIL. The routine occurred error 
 *                                      during decompression algorithm process.
 *          CLR_INSUFFICIENT_BUFFER     FAIL. The destination buffer is not enough.
 ----------------------------------------------------------------------------*/

extern INT32 _cl_prog_desc_decode (
            CL_ENG_DECOMPRESS_TYPE_T    e_decompress_type,
            VOID*                       pv_src_buf, 
            SIZE_T                      z_src_len, 
            UINT32                      ui4_param_type,  
            VOID*                       pv_param,
            VOID*                       pv_dest_buf, 
            SIZE_T                      z_skip_len, 
            SIZE_T*                     pz_dest_len,
            HANDLE_T*                   ph_prev_state
            )
{
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( (pv_dest_buf == NULL) ||
        (pz_dest_len == NULL))
    {
        return CLR_INV_ARG;
    }
    /************************************************************************/
    /* Perform decode algorithm                                             */
    /************************************************************************/
    if (ph_prev_state)
    {
        *ph_prev_state = NULL_HANDLE;
    }
    switch(e_decompress_type)
    {
    case CL_DECOMPRESS_TYPE:
        return _cl_huffman_decode(CL_PROGRAM_DESCRIPTION_TYPE, pv_src_buf, z_src_len, pv_dest_buf, z_skip_len, pz_dest_len);
    case CL_DECOMPRESS_ENTRY_TYPE:
        return CLR_FCT_NOT_SUPPORT;
    default:
        return CLR_INV_ARG;
    }
}

/*-----------------------------------------------------------------------------
 * Name:  _cl_get_prog_title_dec_len
 *
 * Description: This API will use PROGRAM_TITLE table to get decoded output length
 *
 * Inputs:  e_get_type          specify the type of "get" function. Please refer to section 6.2.2        
 *          pv_in_arg           point to the input argument which is needed to perform "get" function.
 *          z_in_arg            point to the input argument which is needed to perform "get" function.
 *                              It can be z_src_len or e_type in x_cl_get_query_info_len.
 *          ui4_param_type      specify type of decompression paramters, set CL_NULL_PARAM_TYPE is using default
 *          pv_param            specify decompression paramters, set NULL is using default
 *          pv_filter           specify the filter argument such as index number of an entry.
 *
 * Outputs: pv_info         specified the buffer which contains information returned by the engine.
 *          pz_info_len     specified the length of information buffer.
 *
 * Returns: CLR_OK                      Success.
 *          CLR_INV_ARG                 FAIL. Invalid arguments.
 *          CLR_OUT_OF_MEMORY           FAIL. The system is out of memory.
 *          CLR_FCT_NOT_SUPPORT         FAIL. This function is not supported.
 *          CLR_DECOMPRESS_ERROR        FAIL. The routine occurred error
                                        during decompression algorithm process.
 *          CLR_COMPRESS_ERROR          FAIL. The routine occurred error 
 *                                      during compression algorithm process.
 *          CLR_INSUFFICIENT_BUFFER     FAIL. The destination buffer is not enough.
 ----------------------------------------------------------------------------*/

extern INT32 _cl_get_prog_title_dec_len (
            CL_ENG_GET_TYPE_T        e_get_type,
            VOID*                    pv_in_arg,
            SIZE_T                   z_in_arg,
            UINT32                   ui4_param_type,  
            VOID*                    pv_param,
            VOID*                    pv_filter,
            VOID*                    pv_info,
            SIZE_T*                  pz_info_len
            )
{
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( ((INT32) e_get_type < CL_GET_COMPRESS_LEN_TYPE) ||
        ((INT32) e_get_type > CL_QUERY_INFO_TYPE) )
    {
        return CLR_INV_ARG;
    }
    /************************************************************************/
    /* Perform decode algorithm                                             */
    /************************************************************************/
    *pz_info_len = 0;
    switch(e_get_type)
    {
    case CL_GET_COMPRESS_LEN_TYPE:
        return CLR_FCT_NOT_SUPPORT;
    case CL_GET_DECOMPRESS_LEN_TYPE:
        return _cl_huffman_decode(CL_PROGRAM_TITLE_TYPE, pv_in_arg, z_in_arg, NULL, 0, pz_info_len);
    case CL_GET_NUM_ENTRIES_TYPE:
        return CLR_FCT_NOT_SUPPORT;
    case CL_GET_QUERY_INFO_LEN_TYPE:
        return CLR_FCT_NOT_SUPPORT;
    case CL_QUERY_INFO_TYPE:
        return CLR_FCT_NOT_SUPPORT;
    default:
        return CLR_INV_ARG;
    }
}

extern INT32 _cl_get_prog_dvbt_table1_dec_len (
            CL_ENG_GET_TYPE_T        e_get_type,
            VOID*                    pv_in_arg,
            SIZE_T                   z_in_arg,
            UINT32                   ui4_param_type,  
            VOID*                    pv_param,
            VOID*                    pv_filter,
            VOID*                    pv_info,
            SIZE_T*                  pz_info_len
            )
{
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( ((INT32) e_get_type < CL_GET_COMPRESS_LEN_TYPE) ||
        ((INT32) e_get_type > CL_QUERY_INFO_TYPE) )
    {
        return CLR_INV_ARG;
    }
    /************************************************************************/
    /* Perform decode algorithm                                             */
    /************************************************************************/
    *pz_info_len = 0;
    switch(e_get_type)
    {
    case CL_GET_COMPRESS_LEN_TYPE:
        return CLR_FCT_NOT_SUPPORT;
    case CL_GET_DECOMPRESS_LEN_TYPE:
        return _cl_huffman_decode(CL_PROGRAM_DVBT_TABLE1_TYPE, pv_in_arg, z_in_arg, NULL, 0, pz_info_len);
    case CL_GET_NUM_ENTRIES_TYPE:
        return CLR_FCT_NOT_SUPPORT;
    case CL_GET_QUERY_INFO_LEN_TYPE:
        return CLR_FCT_NOT_SUPPORT;
    case CL_QUERY_INFO_TYPE:
        return CLR_FCT_NOT_SUPPORT;
    default:
        return CLR_INV_ARG;
    }
}


extern INT32 _cl_get_prog_dvbt_table2_dec_len (
            CL_ENG_GET_TYPE_T        e_get_type,
            VOID*                    pv_in_arg,
            SIZE_T                   z_in_arg,
            UINT32                   ui4_param_type,  
            VOID*                    pv_param,
            VOID*                    pv_filter,
            VOID*                    pv_info,
            SIZE_T*                  pz_info_len
            )
{
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( ((INT32) e_get_type < CL_GET_COMPRESS_LEN_TYPE) ||
        ((INT32) e_get_type > CL_QUERY_INFO_TYPE) )
    {
        return CLR_INV_ARG;
    }
    /************************************************************************/
    /* Perform decode algorithm                                             */
    /************************************************************************/
    *pz_info_len = 0;
    switch(e_get_type)
    {
    case CL_GET_COMPRESS_LEN_TYPE:
        return CLR_FCT_NOT_SUPPORT;
    case CL_GET_DECOMPRESS_LEN_TYPE:
        return _cl_huffman_decode(CL_PROGRAM_DVBT_TABLE2_TYPE, pv_in_arg, z_in_arg, NULL, 0, pz_info_len);
    case CL_GET_NUM_ENTRIES_TYPE:
        return CLR_FCT_NOT_SUPPORT;
    case CL_GET_QUERY_INFO_LEN_TYPE:
        return CLR_FCT_NOT_SUPPORT;
    case CL_QUERY_INFO_TYPE:
        return CLR_FCT_NOT_SUPPORT;
    default:
        return CLR_INV_ARG;
    }
}


/*-----------------------------------------------------------------------------
 * Name:  _cl_get_prog_desc_dec_len
 *
 * Description: This API will use PROGRAM_DESCRIPTION table to get decoded output length
 *
 * Inputs:  e_get_type          specify the type of "get" function. Please refer to section 6.2.2        
 *          pv_in_arg           point to the input argument which is needed to perform "get" function.
 *          z_in_arg            point to the input argument which is needed to perform "get" function.
 *                              It can be z_src_len or e_type in x_cl_get_query_info_len.
 *          ui4_param_type      specify type of decompression paramters, set CL_NULL_PARAM_TYPE is using default
 *          pv_param            specify decompression paramters, set NULL is using default
 *          pv_filter           specify the filter argument such as index number of an entry.
 *
 * Outputs: pv_info         specified the buffer which contains information returned by the engine.
 *          pz_info_len     specified the length of information buffer.
 *
 * Returns: CLR_OK                      Success.
 *          CLR_INV_ARG                 FAIL. Invalid arguments.
 *          CLR_OUT_OF_MEMORY           FAIL. The system is out of memory.
 *          CLR_FCT_NOT_SUPPORT         FAIL. This function is not supported.
 *          CLR_DECOMPRESS_ERROR        FAIL. The routine occurred error
                                        during decompression algorithm process.
 *          CLR_COMPRESS_ERROR          FAIL. The routine occurred error 
 *                                      during compression algorithm process.
 *          CLR_INSUFFICIENT_BUFFER     FAIL. The destination buffer is not enough.
 ----------------------------------------------------------------------------*/

extern INT32 _cl_get_prog_desc_dec_len (
            CL_ENG_GET_TYPE_T        e_get_type,
            VOID*                    pv_in_arg,
            SIZE_T                   z_in_arg,
            UINT32                   ui4_param_type,  
            VOID*                    pv_param,
            VOID*                    pv_filter,
            VOID*                    pv_info,
            SIZE_T*                  pz_info_len
            )
{
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( ((INT32)e_get_type < CL_GET_COMPRESS_LEN_TYPE) ||
        ((INT32)e_get_type > CL_QUERY_INFO_TYPE) )
    {
        return CLR_INV_ARG;
    }
    /************************************************************************/
    /* Perform decode algorithm                                             */
    /************************************************************************/
    *pz_info_len = 0;
    switch(e_get_type)
    {
    case CL_GET_COMPRESS_LEN_TYPE:
        return CLR_FCT_NOT_SUPPORT;
    case CL_GET_DECOMPRESS_LEN_TYPE:
        return _cl_huffman_decode(CL_PROGRAM_DESCRIPTION_TYPE, pv_in_arg, z_in_arg, NULL, 0, pz_info_len);
    case CL_GET_NUM_ENTRIES_TYPE:
        return CLR_FCT_NOT_SUPPORT;
    case CL_GET_QUERY_INFO_LEN_TYPE:
        return CLR_FCT_NOT_SUPPORT;
    case CL_QUERY_INFO_TYPE:
        return CLR_FCT_NOT_SUPPORT;
    default:
        return CLR_INV_ARG;
    }
}





