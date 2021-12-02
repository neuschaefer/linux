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
 * $RCSfile: _cl_zip_eng_interface.c,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/14 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 18447aa39cd3ceef22a68d29fcc3e242 $ Asa
 *
 * Description: 
 *         This file contains all interface APIs of ZIP engine. 
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "handle/handle.h"
#include "handle/x_handle_grp.h"
#include "os/inc/x_os.h"
#include "inc/x_common.h"
#include "zlib.h"
#include "unzip.h"
#include "cl/x_cl.h"
#include "cl/_cl.h"
#include "cl/zip_eng/u_cl_zip_eng.h"
#include "inc/x_sys_name.h"
#include "dbg/x_dbg.h"
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
 #define DEF_MEM_LEVEL 8
/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
extern int unzGetDecompressEntryLen(unzFile file, SIZE_T* pz_query_buf_len);
#ifdef CL_SUPPORT_ARCHIVE
/*-----------------------------------------------------------------------------
 * Name:  _cl_zip_free_handle
 *
 * Description: callback function for free handle
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE    success. 
 *          FALSE   fail.
 ----------------------------------------------------------------------------*/

static BOOL _cl_zip_free_handle (
    HANDLE_T        h_handle,
    HANDLE_TYPE_T   e_type,
    VOID*           pv_obj,
    VOID*           pv_tag,
    BOOL            b_req_handle)
{
    if (pv_obj)
    {
        x_mem_free(pv_obj);
        pv_obj = NULL;
        return TRUE;
    }
    
    return FALSE;
}
/*-----------------------------------------------------------------------------
 * Name:  _cl_decomp_entry
 *
 * Description: decompress a entry data in a archive
 *
 * Inputs:  h_entry             the handle of the entry 
 *          z_skip_len          the byte number of skip destination buffer 
 *          pz_dest_len         the length of destination buffer.
 *
 * Outputs: pv_dest             the destination buffer.
 *          pz_dest_len         specify the really decompression length of destination buffer.
 *
 * Returns: CLR_OK                      Success.
 *          CLR_INV_ARG                 FAIL. Invalid arguments.
 *          CLR_INV_HANDLE              FAIL. Invalid handle
 *          CLR_OUT_OF_MEMORY           FAIL. out of memory.
 *          CLR_DECOMPRESS_ERROR        FAIL. occur error during decompression data
 ----------------------------------------------------------------------------*/
static INT32 _cl_decomp_entry ( 
          HANDLE_T          h_entry, 
          VOID*             pv_dest, 
          SIZE_T            z_skip_len, 
          SIZE_T*           pz_dest_len 
          )
{
    INT32 i4_read_bytes;
    VOID* pv_dummy;
    HANDLE_TYPE_T e_type;
    CL_ENTRY_OBJ_T* pt_entry_obj; 
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_dest == NULL) ||
        (pz_dest_len == NULL))
    {
        return CLR_INV_ARG;
    }
    /************************************************************************/
    /* Perform open entry                                                   */
    /************************************************************************/
    if (HR_OK != handle_get_type_obj(h_entry, &e_type, (VOID**)&pt_entry_obj))
    {
        return CLR_INV_HANDLE;
    }
    
    if ( (e_type != HT_CL_ETY_TYPE) ||
        (pt_entry_obj == NULL) )
    {
        return CLR_INV_HANDLE;
    }
    
    if (z_skip_len)
    {   /* read skip bytes into dummy buffer*/
        pv_dummy = (void*)x_mem_alloc(z_skip_len);
        if (pv_dummy == NULL)
        {
            return CLR_OUT_OF_MEMORY;
        }

        i4_read_bytes = unzReadCurrentFile(pt_entry_obj->pv_data, pv_dummy, z_skip_len);
        
        x_mem_free(pv_dummy);/* free dummy buffer first, because it will return when occurs error*/
        
        if (i4_read_bytes <= 0)
        {
            goto CL_DECOP_ENTRY_EXIT;
        }
    }
    /* read data to destination buffer */
    i4_read_bytes = unzReadCurrentFile(pt_entry_obj->pv_data, pv_dest, *pz_dest_len);

CL_DECOP_ENTRY_EXIT:
    
    if (i4_read_bytes < 0)
    { /*UNZ_ERRNO*/
        unzRestorePos(pt_entry_obj->pv_data);/* restore file position*/
        return CLR_DECOMPRESS_ERROR;  
    } 

    unzRestorePos(pt_entry_obj->pv_data);/* restore file position*/
    *pz_dest_len = i4_read_bytes;
    return CLR_OK;
}
/*-----------------------------------------------------------------------------
 * Name:  _cl_close_entry
 *
 * Description: close a entry in a archive
 *
 * Inputs:  pv_entry_obj       the handle of the archive 
 *
 * Outputs: -
 *
 * Returns: CLR_OK          Success.
 ----------------------------------------------------------------------------*/
static INT32 _cl_close_entry ( 
             VOID*        pv_entry_obj 
             )
{
    /*mini-unzip does not support entry handles,so it is a library specific difference*/
    return CLR_OK;
}
/*-----------------------------------------------------------------------------
 * Name:  _cl_open_entry_by_name
 *
 * Description: open a entry in a archive by a name string
 *
 * Inputs:  pv_archive_obj      the handle of the archive          
 *          ps_name             entry name string (NULL terminate)
 *
 * Outputs: ppv_entry_obj       point to the address of the handle of the entry
 *
 * Returns: CLR_OK                      Success.
 *          CLR_INV_ARG                 FAIL. Invalid arguments.
 *          CLR_BAD_ARCHIVE             FAIL. The archive is corrupt.
 *          CLR_FILE_OR_DIR_MISSING     FAIL. The entry file is missing, please check the path string.
 ----------------------------------------------------------------------------*/
static INT32 _cl_open_entry_by_name ( 
              VOID*             pv_archive_obj, 
              const CHAR*       ps_name, 
              VOID**            ppv_entry_obj 
              )
{
    INT32 i4_res;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_archive_obj == NULL) ||
        (ps_name == NULL) ||
        (ppv_entry_obj == NULL))
    {
        return CLR_INV_ARG;
    }
    /************************************************************************/
    /* Perform open entry                                                   */
    /************************************************************************/
    if (unzGoToFirstFile(pv_archive_obj) != UNZ_OK)
        {
            return CLR_BAD_ARCHIVE;
        }
    
    i4_res = unzLocateFile(pv_archive_obj, ps_name, 1/*case sensitive*/);
    if (i4_res != UNZ_OK)
    {
        if (i4_res != UNZ_END_OF_LIST_OF_FILE)
        {    
            return CLR_BAD_ARCHIVE;
        }
        else
        {
            return CLR_FILE_OR_DIR_MISSING;
        }
    }

    if (unzOpenCurrentFile(pv_archive_obj) != UNZ_OK)
    {
        return CLR_BAD_ARCHIVE;
    }
    
    *ppv_entry_obj = pv_archive_obj;
    /*mini-unzip does not support entry handles,so it is a library specific difference*/
    return CLR_OK;
}
/*-----------------------------------------------------------------------------
 * Name:  _cl_open_entry_by_index
 *
 * Description: open a entry in a archive by a index number
 *
 * Inputs:  pv_archive_obj          the handle of the archive          
 *          ui4_entry_index         index number (zero base)
 *
 * Outputs: ppv_entry_obj           point to the address of the handle of the entry
 *
 * Returns: CLR_OK                      Success.
 *          CLR_INV_ARG                 FAIL. Invalid arguments.
 *          CLR_BAD_ARCHIVE             FAIL. The archive is corrupt.
 ----------------------------------------------------------------------------*/
static INT32 _cl_open_entry_by_index ( 
              VOID*                 pv_archive_obj, 
              UINT32                ui4_entry_index, 
              VOID**                ppv_entry_obj 
              )
{
    INT32 i4_res;
    UINT32 i;
    unz_global_info t_global_info;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_archive_obj == NULL) ||
        (ppv_entry_obj == NULL))
    {
        return CLR_INV_ARG;
    }
    /************************************************************************/
    /* Perform open entry                                                   */
    /************************************************************************/
    i4_res = unzGetGlobalInfo(pv_archive_obj, &t_global_info);
    if(i4_res != UNZ_OK)
    {
        return CLR_BAD_ARCHIVE;
    }
    /* check if out of boundary*/
    if (ui4_entry_index >= t_global_info.number_entry)
    {
        return CLR_INV_ARG;
    }

    if (unzGoToFirstFile(pv_archive_obj) != UNZ_OK)
        {
            return CLR_BAD_ARCHIVE;
        }
    /* set current entry*/
    for (i=0;i<ui4_entry_index;i++)
    {
        if (unzGoToNextFile(pv_archive_obj) != UNZ_OK)
        {
            return CLR_BAD_ARCHIVE;
        }
    }
    
    if (unzOpenCurrentFile(pv_archive_obj) != UNZ_OK)
    {
        return CLR_BAD_ARCHIVE;
    }

    *ppv_entry_obj = pv_archive_obj;
    /*mini-unzip does not support entry handles,so it is a library specific difference*/
    return CLR_OK;
}
/*-----------------------------------------------------------------------------
 * Name:  _cl_open_archive
 *
 * Description: open the archive for reading writing operation
 *
 * Inputs:  ps_path             the full path of the archive           
 *          ui1_mode            access mode
 *
 * Outputs: ppv_archive_obj     point to the address of the handle of archive
 *
 * Returns: CLR_OK                  Success.
 *          CLR_INV_ARG             FAIL. Invalid arguments.
 *          CLR_BAD_ARCHIVE         FAIL. The archive is corrupt.
 *          CLR_FCT_NOT_SUPPORT     FAIL. The function is not supported.
 *          CLR_FILE_OR_DIR_MISSING FAIL. The file is missing, please check 
 *                                  the name string again.
 ----------------------------------------------------------------------------*/
static INT32 _cl_open_archive (
               CHAR*        ps_path, 
               UINT8        ui1_mode, 
               VOID**       ppv_archive_obj 
               )
{
    INT32 i4_res;
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((ps_path == NULL) ||
        (ppv_archive_obj == NULL))
    {
        return CLR_INV_ARG;
    }
    /* only support CL_READ_MODE mode */
    if ((ui1_mode & CL_WRITE_MODE) ||
        (ui1_mode & CL_CREATE_MODE) ||
        (ui1_mode & CL_APPEND_MODE))
    {
        return CLR_FCT_NOT_SUPPORT;
    }

    /************************************************************************/
    /* Perform open a archive                                               */
    /************************************************************************/
    if (ui1_mode & CL_READ_MODE)
    {
        i4_res = unzOpen3(ps_path, ppv_archive_obj);
        if ((i4_res == UNZ_ERRNO) ||
            (i4_res == UNZ_BADZIPFILE))
        {
            return CLR_BAD_ARCHIVE;
        }
        else if (i4_res == UNZ_FILEMISS)
        {
            return CLR_FILE_OR_DIR_MISSING;
        }
    }
    else
    {
        return CLR_FCT_NOT_SUPPORT;
    }

    return CLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _cl_close_archive
 *
 * Description: close the handle of the archive
 *
 * Inputs:  pv_archive_obj     point to the handle of archive
 *          
 * Outputs: -
 *
 * Returns: CLR_OK                      Success.
 *          CLR_BAD_ARCHIVE             FAIL. The archive is corrupt.
 *          CLR_INV_ARG                 FAIL. Invalid arguments.
 ----------------------------------------------------------------------------*/
static INT32 _cl_close_archive ( 
             VOID*      pv_archive_obj
             )
{
    INT32 i4_res;
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pv_archive_obj == NULL) 
    {
        return CLR_INV_ARG;
    }
    /************************************************************************/
    /* Perform close a archive                                              */
    /************************************************************************/
    i4_res = unzClose(pv_archive_obj);
    if (i4_res != UNZ_OK)
    {
        return CLR_BAD_ARCHIVE;
    }

    return CLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _cl_get_query_info_len
 *
 * Description: get the length of query info
 *
 * Inputs:  h_archive               the handle of archive
 *          e_type                  specify the query type
 *          pv_filter               specify the entry index (zero base) or NULL
 *
 * Outputs: pz_query_buf_len        the length of query info data 
 *
 * Returns: CLR_OK                      Success.
 *          CLR_INV_HANDLE              FAIL. Invalid handle
 *          CLR_INV_ARG                 FAIL. Invalid arguments.
 *          CLR_BAD_ARCHIVE             FAIL. The archive is corrupt.
 ----------------------------------------------------------------------------*/
static INT32 _cl_get_query_info_len ( 
                 HANDLE_T                h_archive, 
                 CL_QUERY_INFO_TYPE_T    e_type, 
                 VOID*                   pv_filter, 
                 SIZE_T*                 pz_query_buf_len 
                 )
{
    INT32 i4_res;
    UINT32 i;
    unz_global_info t_global_info;
    unz_file_info t_entry_info;
    UINT32 ui4_index;
    HANDLE_TYPE_T e_htype;
    CL_ARCHIVE_OBJ_T* pt_arc_obj; 
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pz_query_buf_len == NULL)||
        (((INT32)e_type < CL_QUERY_GLOBAL_COMMENT_TYPE) && ((INT32)e_type > CL_QUERY_ENTRY_COMMENT_TYPE)) ||
        ((pv_filter == NULL) && (e_type == CL_QUERY_ENTRY_COMMENT_TYPE)) ||
        ((pv_filter == NULL) && (e_type == CL_QUERY_ENTRY_INFO_TYPE)) ||
        ((pv_filter == NULL) && (e_type == CL_QUERY_ENTRY_NAME_TYPE)) ||
        ((pv_filter != NULL) && (e_type == CL_QUERY_GLOBAL_COMMENT_TYPE)))
    {
        return CLR_INV_ARG;
    }
    /************************************************************************/
    /* Perform query                                                        */
    /************************************************************************/
    if (HR_OK != handle_get_type_obj(h_archive, &e_htype, (VOID**)&pt_arc_obj))
    {
        return CLR_INV_HANDLE;
    }
    
    if ( (e_htype != HT_CL_ARC_TYPE) ||
        (pt_arc_obj == NULL) )
    {
        return CLR_INV_HANDLE;
    }
    
    i4_res = unzGetGlobalInfo(pt_arc_obj->pv_data, &t_global_info);
    if(i4_res != UNZ_OK)
    {
        return CLR_BAD_ARCHIVE;
    }
     
    if (e_type == CL_QUERY_GLOBAL_COMMENT_TYPE) /* query global */ 
    { 
        if (t_global_info.size_comment)
        {
            *pz_query_buf_len = t_global_info.size_comment + 1;/* NULL terminate*/
        }
        else
        {
            *pz_query_buf_len = 0;
        }
    }
    else /* CL_QUERY_ENTRY_INFO_TYPE, CL_QUERY_ENTRY_NAME_TYPE, CL_QUERY_ENTRY_COMMENT_TYPE */
    {
        /* Check if entry number is valid? */
        ui4_index = *((UINT32*)pv_filter);
        if (ui4_index >= t_global_info.number_entry)
        {
            return CLR_INV_ARG;
        }

        if (unzGoToFirstFile(pt_arc_obj->pv_data) != UNZ_OK)
        {
            return CLR_BAD_ARCHIVE;
        }
        /* set current entry*/
        for (i=0;i<ui4_index;i++)
        {
            if (unzGoToNextFile(pt_arc_obj->pv_data) != UNZ_OK)
            {
                return CLR_INV_ARG;
            }
        }
        /*get entry info*/
        if (unzGetCurrentFileInfo(pt_arc_obj->pv_data, &t_entry_info, NULL, 0, NULL, 0, NULL, 0) != UNZ_OK)
        {
            return CLR_BAD_ARCHIVE;
        }

        switch(e_type)
        {
        case CL_QUERY_ENTRY_INFO_TYPE:
            *pz_query_buf_len = sizeof(CL_ENTRY_INFO_T);
            
            break;
        case CL_QUERY_ENTRY_NAME_TYPE:
            if (t_entry_info.size_filename)
            {
                *pz_query_buf_len = t_entry_info.size_filename + 1/*NULL terminate*/;
            }
            else
            {
                *pz_query_buf_len = 0;
            }
            
            break;
        case CL_QUERY_ENTRY_COMMENT_TYPE:
            if(t_entry_info.size_file_comment)
            {
                *pz_query_buf_len = t_entry_info.size_file_comment + 1/*NULL terminate*/;
            }
            else
            {
                *pz_query_buf_len = 0;
            }
            
            break;
        default:
            *pz_query_buf_len = 0;
            return CLR_INV_ARG;
        }
    }
    return CLR_OK;
}

 /*-----------------------------------------------------------------------------
 * Name:  _cl_get_decompress_entry_len
 *
 * Description: get the decompressed length of a entry 
 *
 * Inputs:  h_entry             the handle of a entry
 *
 * Outputs: pz_query_buf_len    the length of query info data 
 *
 * Returns: CLR_OK                      Success.
 *          CLR_INV_HANDLE              FAIL. Invalid handle
 *          CLR_INV_ARG                 FAIL. Invalid arguments.
 *          CLR_BAD_ARCHIVE             FAIL. The archive is corrupt.
 ----------------------------------------------------------------------------*/
static INT32 _cl_get_decompress_entry_len ( 
          HANDLE_T              h_entry, 
          SIZE_T*               pz_query_buf_len 
          )
{
    HANDLE_TYPE_T e_type;
    CL_ENTRY_OBJ_T* pt_entry_obj; 
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (HR_OK != handle_get_type_obj(h_entry, &e_type, (VOID**)&pt_entry_obj))
    {
        return CLR_INV_HANDLE;
    }
    
    if ( (e_type != HT_CL_ETY_TYPE) ||
        (pt_entry_obj == NULL) )
    {
        return CLR_INV_HANDLE;
    }
    
    if (pz_query_buf_len == NULL)
    {
        return CLR_INV_ARG;
    }
    /************************************************************************/
    /* Perform query                                                        */
    /************************************************************************/
    if (UNZ_OK != unzGetDecompressEntryLen(pt_entry_obj->pv_data, pz_query_buf_len))
    {
        return CLR_BAD_ARCHIVE;
    }
   
    return CLR_OK;
}
/*-----------------------------------------------------------------------------
 * Name:  _cl_query_info
 *
 * Description: get the query info
 *
 * Inputs:  h_archive               the handle of archive
 *          e_type                  specify the query type
 *          pv_filter               specify the entry index (zero base) or NULL
 *          pz_query_buf_len        the length of query info data
 *
 * Outputs: pv_query_buf        the buffer address contain query info
 *          pz_query_buf_len    the length of query info data 
 *
 * Returns: CLR_OK                      Success.
 *          CLR_INV_ARG                 FAIL. Invalid arguments.
 *          CLR_INV_HANDLE              FAIL. Invalid handle
 *          CLR_BAD_ARCHIVE             FAIL. The archive is corrupt.
 *          CLR_INSUFFICIENT_BUFFER     FAIL. The length of destination buffer is not enough
 ----------------------------------------------------------------------------*/
static INT32 _cl_query_info ( 
          HANDLE_T              h_archive, 
          CL_QUERY_INFO_TYPE_T  e_type, 
          VOID*                 pv_filter, 
          VOID*                 pv_query_buf, 
          SIZE_T*               pz_query_buf_len 
          )
{
    INT32 i4_res;
    UINT32 i;
    UINT32 ui4_index;
    unz_global_info t_global_info;
    CL_ENTRY_INFO_T* pt_cl_entry_info;
    unz_file_info t_entry_info;
    HANDLE_TYPE_T e_htype;
    CL_ARCHIVE_OBJ_T* pt_arc_obj; 
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_query_buf == NULL) ||
        (pz_query_buf_len == NULL)||
        (((INT32)e_type < CL_QUERY_GLOBAL_COMMENT_TYPE) && ((INT32)e_type > CL_QUERY_ENTRY_COMMENT_TYPE)) ||
        ((pv_filter == NULL) && (e_type == CL_QUERY_ENTRY_COMMENT_TYPE)) ||
        ((pv_filter == NULL) && (e_type == CL_QUERY_ENTRY_INFO_TYPE)) ||
        ((pv_filter == NULL) && (e_type == CL_QUERY_ENTRY_NAME_TYPE)) ||
        ((pv_filter != NULL) && (e_type == CL_QUERY_GLOBAL_COMMENT_TYPE)))
    {
        return CLR_INV_ARG;
    }
    /************************************************************************/
    /* Perform query                                                        */
    /************************************************************************/
    if (HR_OK != handle_get_type_obj(h_archive, &e_htype, (VOID**)&pt_arc_obj))
    {
        return CLR_INV_HANDLE;
    }
    
    if ( (e_htype != HT_CL_ARC_TYPE) ||
        (pt_arc_obj == NULL) )
    {
        return CLR_INV_HANDLE;
    }
    /* query global */
    i4_res = unzGetGlobalInfo(pt_arc_obj->pv_data, &t_global_info);
    if(i4_res != UNZ_OK)
    {
        return CLR_BAD_ARCHIVE;
    }

    if (e_type == CL_QUERY_GLOBAL_COMMENT_TYPE) /* query global */ 
    { 
        /* Check buffer size */
        if(t_global_info.size_comment)
        {
            if (*pz_query_buf_len < t_global_info.size_comment + 1/*NULL terminate*/)
            {
                return CLR_INSUFFICIENT_BUFFER;
            }
            
            i4_res = unzGetGlobalComment(pt_arc_obj->pv_data, pv_query_buf, *pz_query_buf_len);
            if(i4_res < UNZ_OK)
            {
                return CLR_BAD_ARCHIVE;
            }
            /* append null terminate*/

            ((UCHAR*)pv_query_buf)[t_global_info.size_comment] = '\0';/*set NULL terminate*/
        }
        else
        {
            *pz_query_buf_len = 0;
            return CLR_OK;
        }                
    }
    else /* CL_QUERY_ENTRY_INFO_TYPE, CL_QUERY_ENTRY_NAME_TYPE, CL_QUERY_ENTRY_COMMENT_TYPE */
    {
        /* query a entry */
        ui4_index = *((UINT32*)pv_filter);
        if (ui4_index >= t_global_info.number_entry)
        {
            return CLR_INV_ARG;
        }

        if (unzGoToFirstFile(pt_arc_obj->pv_data) != UNZ_OK)
        {
            return CLR_BAD_ARCHIVE;
        }
        /* set current entry*/
        for (i=0;i<ui4_index;i++)
        {
            if (unzGoToNextFile(pt_arc_obj->pv_data) != UNZ_OK)
            {
                return CLR_BAD_ARCHIVE;
            }
        }

        /* get info */
        if (unzGetCurrentFileInfo(pt_arc_obj->pv_data, &t_entry_info, NULL, 0, NULL, 0, NULL, 0) != UNZ_OK)
        {
            return CLR_BAD_ARCHIVE;
        }

        switch(e_type)
        {
        case CL_QUERY_ENTRY_INFO_TYPE:
            /* check size */
            if (*pz_query_buf_len < sizeof(CL_ENTRY_INFO_T))
            {
                return CLR_INSUFFICIENT_BUFFER;
            }
            
            /* fill data*/
            pt_cl_entry_info = pv_query_buf;
            pt_cl_entry_info->ui2_compression_method = (UINT16)t_entry_info.compression_method;
            pt_cl_entry_info->ui4_compressed_size = t_entry_info.compressed_size;        
            pt_cl_entry_info->ui4_decompressed_size = t_entry_info.uncompressed_size;
            pt_cl_entry_info->t_date_time.ui2_sec = t_entry_info.tmu_date.tm_sec;
            pt_cl_entry_info->t_date_time.ui2_min = t_entry_info.tmu_date.tm_min;
            pt_cl_entry_info->t_date_time.ui2_hour = t_entry_info.tmu_date.tm_hour;
            pt_cl_entry_info->t_date_time.ui2_mday = t_entry_info.tmu_date.tm_mday;
            pt_cl_entry_info->t_date_time.ui2_mon = t_entry_info.tmu_date.tm_mon;
            pt_cl_entry_info->t_date_time.ui2_year = t_entry_info.tmu_date.tm_year;
            break;
            
        case CL_QUERY_ENTRY_NAME_TYPE:
            if (t_entry_info.size_filename)
            {
                /* check size */
                if (*pz_query_buf_len < (t_entry_info.size_filename + 1/*NULL terminate*/))
                {
                    return CLR_INSUFFICIENT_BUFFER;
                }

                /* fill data*/
                if (unzGetCurrentFileInfo(pt_arc_obj->pv_data, NULL, pv_query_buf, *pz_query_buf_len, NULL, 0, NULL, 0) != UNZ_OK)
                {
                    return CLR_BAD_ARCHIVE;
                }

                ((UCHAR*)pv_query_buf)[t_entry_info.size_filename] = '\0'; /*set NULL terminate*/
            }
            else
            {
                *pz_query_buf_len = 0;
                return CLR_OK;
            }
            break;

        case CL_QUERY_ENTRY_COMMENT_TYPE:
            
            if (t_entry_info.size_file_comment)
            {   /* check size */
                if (*pz_query_buf_len < (t_entry_info.size_file_comment + 1/*NULL terminate*/))
                {
                    return CLR_INSUFFICIENT_BUFFER;
                }

                /* fill data*/
                if (unzGetCurrentFileInfo(pt_arc_obj->pv_data, NULL, NULL, 0, NULL, 0, pv_query_buf, *pz_query_buf_len) != UNZ_OK)
                {
                    return CLR_BAD_ARCHIVE;
                }
                
                ((UCHAR*)pv_query_buf)[t_entry_info.size_file_comment] = '\0'; /*set NULL terminate*/
            }
            else
            {
                *pz_query_buf_len = 0;
                return CLR_OK;
            }    
            break;

        default:
            *pz_query_buf_len = 0;
            return CLR_INV_ARG;
        }
    }
    return CLR_OK;
}
/*-----------------------------------------------------------------------------
 * Name:  _cl_get_num_entries
 *
 * Description: This API will get the number of entries in the zip archive  
 *
 * Inputs:  h_archive               the handle of the archive          
 *
 * Outputs: pui4_num_entris         retrieve the number of entries
 *
 * Returns: CLR_OK                      Success.
 *          CLR_INV_ARG                 FAIL. Invalid arguments.
 *          CLR_INV_HANDLE              FAIL. Invalid handle
 *          CLR_BAD_ARCHIVE             FAIL. The archive is corrupt.
 ----------------------------------------------------------------------------*/
static INT32 _cl_get_num_entries (
               HANDLE_T     h_archive, 
               UINT32*      pui4_num_entris
               )
{
    INT32 i4_res;
    unz_global_info t_global_info;
    HANDLE_TYPE_T e_type;
    CL_ARCHIVE_OBJ_T* pt_arc_obj; 
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( pui4_num_entris == NULL )
    {
        return CLR_INV_ARG;
    }
    /************************************************************************/
    /* Perform close a archive                                              */
    /************************************************************************/
    if (HR_OK != handle_get_type_obj(h_archive, &e_type, (VOID**)&pt_arc_obj))
    {
        return CLR_INV_HANDLE;
    }
    
    if ( (e_type != HT_CL_ARC_TYPE) ||
        (pt_arc_obj == NULL) )
    {
        return CLR_INV_HANDLE;
    }
    i4_res = unzGetGlobalInfo(pt_arc_obj->pv_data, &t_global_info);
    if(i4_res != UNZ_OK)
    {
        return CLR_BAD_ARCHIVE;
    }

    *pui4_num_entris = t_global_info.number_entry;
    
    return CLR_OK;
}
#endif
/*-----------------------------------------------------------------------------
 * Name:  _cl_get_zip_comp_len
 *
 * Description: This API will get compressed output length that compress from source buffer to destination buffer.
 *
 * Inputs:  pv_src_buf          source buffer.          
 *          z_src_len           the length of source buffer.
 *          ui4_param_type      specify type of compression paramters, set CL_NULL_PARAM_TYPE is using default
 *          pv_param            specify compression paramters, set NULL is using default
 *
 * Outputs: pz_dest_len the length of destination buffer.
 *
 * Returns: CLR_OK                      Success.
 *          CLR_INV_ARG                 FAIL. Invalid arguments.
 *          CLR_OUT_OF_MEMORY           FAIL. The system is out of memory.
 *          CLR_COMPRESS_ERROR          FAIL. The routine occurred error 
 *                                      during compression algorithm process.
 ----------------------------------------------------------------------------*/
static INT32 _cl_get_zip_comp_len ( 
             const VOID*        pv_src_buf, 
             SIZE_T             z_src_len, 
             UINT32             ui4_param_type,  
             VOID*              pv_param,
             SIZE_T*            pz_dest_len
             )
{
    VOID* pv_tmp = NULL;
    INT32 i4_res;
    z_stream t_stream;
    CL_ZIP_COMPRESSION_T* pt_comp_param = (CL_ZIP_COMPRESSION_T*)pv_param;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( (pv_src_buf == NULL) ||
        (pz_dest_len == NULL) ||
        (z_src_len == 0) )
    {
        return CLR_INV_ARG;
    }
    if (pt_comp_param != NULL)
    {
        if (pt_comp_param->ui1_level > 9)
        {
            return CLR_INV_ARG;
        }
        if ((pt_comp_param->ui1_windowbits > 15)
            || (pt_comp_param->ui1_windowbits < 8))
        {
            return CLR_INV_ARG;
        }
        if ((pt_comp_param->ui1_mem_level > 9)
            || (pt_comp_param->ui1_mem_level < 1))
        {
            return CLR_INV_ARG;
        }
        if (pt_comp_param->e_strategy >= CL_ZIP_LAST_ONE)
        {
            return CLR_INV_ARG;
        }
    }
    /************************************************************************/
    /* Perform encode algorithm                                             */
    /************************************************************************/
    pv_tmp = (VOID*) x_mem_alloc(z_src_len);
    if(pv_tmp == NULL)
    {
        *pz_dest_len = 0;
        return CLR_OUT_OF_MEMORY;
    }
    x_memset(pv_tmp, 0, sizeof(z_src_len));
    x_memset(&t_stream, 0, sizeof(z_stream));
    /* fill data */
    t_stream.next_in = (Bytef*)pv_src_buf;
    t_stream.avail_in = (uInt)z_src_len;
    t_stream.next_out = pv_tmp;
    t_stream.avail_out = (uInt)z_src_len;
    t_stream.zalloc = (alloc_func)0;
    t_stream.zfree = (free_func)0;
    t_stream.opaque = (voidpf)0;

    /* initial engine */
    if (pt_comp_param)
    {
        i4_res = deflateInit2(&t_stream, pt_comp_param->ui1_level,
                       Z_DEFLATED, -pt_comp_param->ui1_windowbits, pt_comp_param->ui1_mem_level, pt_comp_param->e_strategy);
    }
    else
    {
        i4_res = deflateInit2(&t_stream, Z_BEST_COMPRESSION,
                       Z_DEFLATED, -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY);
    }
    if (i4_res != Z_OK) 
    {
        goto CL_GET_ZIP_COMP_LEN_EXIT;
    }
    
    do
    { /* recusive use fixed memory block to compress src buffer */
        i4_res = deflate(&t_stream, Z_FINISH);
        if ((i4_res != Z_OK) &&
            (i4_res!= Z_STREAM_END) )
        {
            goto CL_GET_ZIP_COMP_LEN_EXIT;
        }

        t_stream.next_out = pv_tmp;
        t_stream.avail_out = (uInt)z_src_len;
    } while(i4_res != Z_STREAM_END);
    
    *pz_dest_len = t_stream.total_out;
    /* deinitial engine */
    i4_res = deflateEnd(&t_stream);
    if (i4_res != Z_OK) 
    {
        goto CL_GET_ZIP_COMP_LEN_EXIT;
    }

    /************************************************************************/
    /*   return                                                             */
    /************************************************************************/

CL_GET_ZIP_COMP_LEN_EXIT:

    if (pv_tmp)
    {
        x_mem_free(pv_tmp);
    }

    switch(i4_res)
    {
    case Z_OK:
        return CLR_OK;
    case Z_MEM_ERROR:
        return CLR_OUT_OF_MEMORY;
    default:
        return CLR_COMPRESS_ERROR;
    }
}
/*-----------------------------------------------------------------------------
 * Name:  _cl_zip_comp
 *
 * Description: Compresses the source buffer into the destination buffer.
 *
 * Inputs:  pv_src_buf          source buffer.         
 *          z_src_len           the length of source buffer.
 *          pz_dest_len         the length of destination buffer.
 *          ui4_param_type      specify type of compression paramters, set CL_NULL_PARAM_TYPE is using default
 *          pv_param            specify compression paramters, set NULL is using default
 *
 * Outputs: pv_dest_buf     the destination buffer.
 *          pz_dest_len        the really compression length of destination buffer.
 *
 * Returns: CLR_OK                      Success.
 *          CLR_INV_ARG                 FAIL. Invalid arguments.
 *          CLR_OUT_OF_MEMORY           FAIL. The system is out of memory.
 *          CLR_COMPRESS_ERROR          FAIL. The routine occurred error 
 *                                      during compression algorithm process.
 ----------------------------------------------------------------------------*/
static INT32 _cl_zip_comp ( 
          const VOID*       pv_src_buf, 
          SIZE_T            z_src_len, 
          UINT32            ui4_param_type,  
          VOID*             pv_param,
          VOID*             pv_dest_buf, 
          SIZE_T*           pz_dest_len
          )
{
    INT32 i4_res;
    z_stream t_stream;
    CL_ZIP_COMPRESSION_T* pt_comp_param = (CL_ZIP_COMPRESSION_T*)pv_param;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( (pv_src_buf == NULL) ||
        (pz_dest_len == NULL) ||
        (pv_dest_buf == NULL) ||
        (z_src_len == 0) )
    {
        return CLR_INV_ARG;
    }
    if (pt_comp_param != NULL)
    {
        if (pt_comp_param->ui1_level > 9)
        {
            return CLR_INV_ARG;
        }
        if ((pt_comp_param->ui1_windowbits > 15)
            || (pt_comp_param->ui1_windowbits < 8))
        {
            return CLR_INV_ARG;
        }
        if ((pt_comp_param->ui1_mem_level > 9)
            || (pt_comp_param->ui1_mem_level < 1))
        {
            return CLR_INV_ARG;
        }
        if (pt_comp_param->e_strategy >= CL_ZIP_LAST_ONE)
        {
            return CLR_INV_ARG;
        }
    }
    /************************************************************************/
    /* Perform encode algorithm                                             */
    /************************************************************************/
    x_memset(&t_stream, 0, sizeof(z_stream));
    /* fill data */
    t_stream.next_in = (Bytef*)pv_src_buf;
    t_stream.avail_in = (uInt)z_src_len;
    t_stream.next_out = pv_dest_buf;
    t_stream.avail_out = (uInt)(*pz_dest_len);
    t_stream.zalloc = (alloc_func)0;
    t_stream.zfree = (free_func)0;
    t_stream.opaque = (voidpf)0;

    /* initial engine */
    if (pt_comp_param)
    {
        i4_res = deflateInit2(&t_stream, pt_comp_param->ui1_level,
                       Z_DEFLATED, -pt_comp_param->ui1_windowbits, pt_comp_param->ui1_mem_level, pt_comp_param->e_strategy);
    }
    else
    {
        i4_res = deflateInit2(&t_stream, Z_BEST_COMPRESSION,
                       Z_DEFLATED, -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY);
    }
    if (i4_res != Z_OK) 
    {
        goto CL_GET_ZIP_COMP_LEN_EXIT;
    }
    

    i4_res = deflate(&t_stream, Z_FINISH);
    if ((i4_res != Z_OK) &&
        (i4_res!= Z_STREAM_END) )
    {
        goto CL_GET_ZIP_COMP_LEN_EXIT;
    }

    *pz_dest_len = t_stream.total_out;
    /* deinitial engine */
    i4_res = deflateEnd(&t_stream);
    if (i4_res != Z_OK) 
    {
        goto CL_GET_ZIP_COMP_LEN_EXIT;
    }

    /************************************************************************/
    /*   return                                                             */
    /************************************************************************/

CL_GET_ZIP_COMP_LEN_EXIT:

    switch(i4_res)
    {
    case Z_OK:
        return CLR_OK;
    case Z_MEM_ERROR:
        return CLR_OUT_OF_MEMORY;
    default:
        return CLR_COMPRESS_ERROR;
    }
}
/*-----------------------------------------------------------------------------
 * Name:  _cl_get_zip_decomp_len
 *
 * Description: This API will get compressed output length that compress from source buffer to destination buffer.
 *
 * Inputs:  pv_src_buf          source buffer.         
 *          z_src_len           the length of source buffer.
 *          ui4_param_type      specify type of decompression paramters, set CL_NULL_PARAM_TYPE is using default
 *          pv_param            specify decompression paramters, set NULL is using default
 *
 * Outputs: pz_dest_len the length of destination buffer.
 *
 * Returns: CLR_OK                      Success.
 *          CLR_INV_ARG                 FAIL. Invalid arguments.
 *          CLR_OUT_OF_MEMORY           FAIL. The system is out of memory.
 *          CLR_DECOMPRESS_ERROR        FAIL. The routine occurred error 
 *                                      during decompression algorithm process.
 ----------------------------------------------------------------------------*/
static INT32 _cl_get_zip_decomp_len ( 
           const VOID*          pv_src_buf, 
           SIZE_T               z_src_len, 
           UINT32               ui4_param_type,  
           VOID*                pv_param,
           SIZE_T*              pz_dest_len 
           )
{
    INT32 i4_res;
    VOID* pv_tmp = NULL;
    z_stream t_stream;
    UINT8 ui1_wbits = 0;
    CL_ZIP_DECOMPRESSION_T* pt_decomp_param = (CL_ZIP_DECOMPRESSION_T*)pv_param;
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( (pv_src_buf == NULL) ||
        (pz_dest_len == NULL) ||
        (z_src_len == 0) )
    {
        return CLR_INV_ARG;
    }
    if (pt_decomp_param != NULL)
    {
        ui1_wbits = pt_decomp_param->ui1_windowbits;
        if ((ui1_wbits < 8)
            ||(ui1_wbits > 15))
        {
            return CLR_INV_ARG;
        }
    }
    
    /************************************************************************/
    /* Perform decode algorithm                                             */
    /************************************************************************/
    pv_tmp = (VOID*) x_mem_alloc(z_src_len);
    if(pv_tmp == NULL)
    {
        *pz_dest_len = 0;
        return CLR_OUT_OF_MEMORY;
    }
    x_memset(pv_tmp, 0, sizeof(z_src_len));
    x_memset(&t_stream, 0, sizeof(z_stream));
    /* fill data */    
    t_stream.next_in = (Bytef*)pv_src_buf;
    t_stream.avail_in = (uInt)z_src_len;
    t_stream.next_out = pv_tmp;
    t_stream.avail_out = (uInt)z_src_len;
    t_stream.zalloc = (alloc_func)0;
    t_stream.zfree = (free_func)0;
    
    /* initial engine */    
    if (pt_decomp_param)
    {
        i4_res = inflateInit2(&t_stream, -ui1_wbits);
    }
    else
    {
        i4_res = inflateInit2(&t_stream, -MAX_WBITS);
    }
    if (i4_res != Z_OK) 
    {
        goto CL_GET_ZIP_DECOMP_LEN_EXIT;
    }

    do
    { /* recusive use fixed memory block to decompress src buffer */
        i4_res = inflate(&t_stream, Z_BLOCK);
        if ((i4_res != Z_OK) &&
            (i4_res != Z_STREAM_END) )
        {
            goto CL_GET_ZIP_DECOMP_LEN_EXIT;
        }

        t_stream.next_out = pv_tmp;
        t_stream.avail_out = (uInt)z_src_len;
    } while(i4_res != Z_STREAM_END);
    
    *pz_dest_len = t_stream.total_out ;

    /*deinitial engine*/
    i4_res = inflateEnd(&t_stream);
 
    /************************************************************************/
    /*   return                                                             */
    /************************************************************************/
CL_GET_ZIP_DECOMP_LEN_EXIT:

    if (pv_tmp)
    {
        x_mem_free(pv_tmp);    
    }
    
    switch(i4_res)
    {
    case Z_OK:
        return CLR_OK;
    case Z_MEM_ERROR:
        return CLR_OUT_OF_MEMORY;
    default:
        return CLR_DECOMPRESS_ERROR;
    }
}
static INT32 _cl_get_zip_decomp_len_ex ( 
           const VOID*          pv_src_buf, 
           SIZE_T               z_src_len, 
           UINT32               ui4_param_type,  
           VOID*                pv_param,
           SIZE_T*              pz_dest_len 
           )
{
    INT32 i4_res;
    VOID* pv_tmp = NULL;
    z_stream t_stream;
    UINT8 ui1_wbits = 0;
    CL_ZIP_DECOMPRESSION_T* pt_decomp_param = (CL_ZIP_DECOMPRESSION_T*)pv_param;
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( (pv_src_buf == NULL) ||
        (pz_dest_len == NULL) ||
        (z_src_len == 0) )
    {
        return CLR_INV_ARG;
    }
    if (pt_decomp_param != NULL)
    {
        ui1_wbits = pt_decomp_param->ui1_windowbits;
        if ((ui1_wbits < 8)
            ||(ui1_wbits > 15))
        {
            return CLR_INV_ARG;
        }
    }
    
    /************************************************************************/
    /* Perform decode algorithm                                             */
    /************************************************************************/
    pv_tmp = (VOID*) x_mem_alloc(z_src_len);
    if(pv_tmp == NULL)
    {
        *pz_dest_len = 0;
        return CLR_OUT_OF_MEMORY;
    }
    x_memset(pv_tmp, 0, sizeof(z_src_len));
    x_memset(&t_stream, 0, sizeof(z_stream));
    /* fill data */    
    t_stream.next_in = (Bytef*)pv_src_buf;
    t_stream.avail_in = (uInt)z_src_len;
    t_stream.next_out = pv_tmp;
    t_stream.avail_out = (uInt)z_src_len;
    t_stream.zalloc = (alloc_func)0;
    t_stream.zfree = (free_func)0;
    
    /* initial engine */    
    if (pt_decomp_param)
    {
        i4_res = inflateInit2(&t_stream, ui1_wbits);
    }
    else
    {
        i4_res = inflateInit2(&t_stream, MAX_WBITS);
    }
    if (i4_res != Z_OK) 
    {
        goto CL_GET_ZIP_DECOMP_LEN_EXIT;
    }

    do
    { /* recusive use fixed memory block to decompress src buffer */
        i4_res = inflate(&t_stream, Z_BLOCK);
        if ((i4_res != Z_OK) &&
            (i4_res != Z_STREAM_END) )
        {
            goto CL_GET_ZIP_DECOMP_LEN_EXIT;
        }

        t_stream.next_out = pv_tmp;
        t_stream.avail_out = (uInt)z_src_len;
    } while(i4_res != Z_STREAM_END);
    
    *pz_dest_len = t_stream.total_out ;

    /*deinitial engine*/
    i4_res = inflateEnd(&t_stream);
 
    /************************************************************************/
    /*   return                                                             */
    /************************************************************************/
CL_GET_ZIP_DECOMP_LEN_EXIT:

    if (pv_tmp)
    {
        x_mem_free(pv_tmp);    
    }
    
    switch(i4_res)
    {
    case Z_OK:
        return CLR_OK;
    case Z_MEM_ERROR:
        return CLR_OUT_OF_MEMORY;
    default:
        return CLR_DECOMPRESS_ERROR;
    }
}


/*-----------------------------------------------------------------------------
 * Name:  _cl_zip_inflate
 *
 * Description: Compresses the source buffer into the destination buffer.
 *
 * Inputs:  pv_src_buf          source buffer.         
 *          z_src_len           the length of source buffer.
 *          z_skip_len          the byte number of skip destination buffer
 *          ph_prev_state       set NULL if one shot;specify previous decompress state
 *
 * Outputs: pv_dest_buf the destination buffer.
 *          pz_dest_len the really decompression length of destination buffer.
 *          ph_prev_state   next time decompress state
 *
 * Returns: CLR_OK                      Success.
 *          CLR_INV_ARG                 FAIL. Invalid arguments.
 *          CLR_OUT_OF_MEMORY           FAIL. The system is out of memory.
 *          CLR_DECOMPRESS_ERROR        FAIL. The routine occurred error 
 *                                      during decompression algorithm process.
 *          CLR_INSUFFICIENT_BUFFER     FAIL. The destination buffer is not enough.
 ----------------------------------------------------------------------------*/
#define MAX_DUMMY_ARRAY_LENGTH 1024
static INT32 _cl_zip_inflate ( 
         z_stream*                   pt_stream,
         const VOID*                 pv_src_buf, 
         SIZE_T                      z_src_len, 
         VOID*                       pv_dest_buf, 
         SIZE_T                      z_skip_len, 
         SIZE_T*                     pz_dest_len
         )
{
    INT32 i4_ret;
    UINT8 aui1_dummy[MAX_DUMMY_ARRAY_LENGTH];
    SIZE_T z_left, z_one_time_size;

        
    if (z_skip_len)
    {/* read skip bytes first, and then read others to destination buffer */
        z_left = z_skip_len;          
        
        do{
            /* read dummy z_one_time_size*/
            if (z_left > MAX_DUMMY_ARRAY_LENGTH)
            {
                z_one_time_size = MAX_DUMMY_ARRAY_LENGTH;
            }
            else
            {
                z_one_time_size = z_left;
            }
            z_left = z_left - z_one_time_size;
            
            pt_stream->next_out = aui1_dummy;
            pt_stream->avail_out = (uInt)z_one_time_size;

            i4_ret = inflate(pt_stream, Z_SYNC_FLUSH);
            if ( (i4_ret != Z_OK) &&
                (i4_ret != Z_STREAM_END) )
            {
                goto CL_ZIP_INFLATE_EXIT;
            }
            
            pt_stream->total_out = pt_stream->total_out - z_one_time_size;
            
        }while(z_left > 0);
        
    }
       
    pt_stream->next_out = pv_dest_buf;
    pt_stream->avail_out = (uInt)*pz_dest_len;

    i4_ret = inflate(pt_stream, Z_SYNC_FLUSH);
    if ( (i4_ret != Z_OK) &&
        (i4_ret != Z_STREAM_END) )
    {
        goto CL_ZIP_INFLATE_EXIT;
    }

    *pz_dest_len = pt_stream->total_out ;
    return CLR_OK;
        
CL_ZIP_INFLATE_EXIT:
    
    switch(i4_ret)
    {
    case Z_OK:
        return CLR_OK;
    case Z_MEM_ERROR:
        return CLR_OUT_OF_MEMORY;
    default:
        return CLR_DECOMPRESS_ERROR;
    }
}
/*-----------------------------------------------------------------------------
 * Name:  _cl_zip_free_repeated_handle
 *
 * Description: callback function for free handle
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE    success. 
 *          FALSE   fail.
 ----------------------------------------------------------------------------*/
static BOOL _cl_zip_free_repeated_handle (
    HANDLE_T        h_handle,
    HANDLE_TYPE_T   e_type,
    VOID*           pv_obj,
    VOID*           pv_tag,
    BOOL            b_req_handle)
{
    DBG_API(("[CL] _cl_zip_free_repeated_handle: h_handle:%x \t e_type:%x \t pv_obj:%x \t pv_tag:%x \t b_req_handle:%x\n\r"
        , h_handle, e_type, pv_obj, pv_tag, b_req_handle));
    
    if (pv_obj)
    {
        inflateEnd((z_stream*)pv_obj);
        x_mem_free(pv_obj);
        pv_obj = NULL;
        return TRUE;
    }

    return FALSE;
}
/*-----------------------------------------------------------------------------
 * Name:  _cl_zip_decomp
 *
 * Description: Compresses the source buffer into the destination buffer.
 *
 * Inputs:  pv_src_buf          source buffer.         
 *          z_src_len           the length of source buffer.
 *          ui4_param_type      specify type of decompression paramters, set CL_NULL_PARAM_TYPE is using default
 *          pv_param            specify decompression paramters, set NULL is using default
 *          z_skip_len          the byte number of skip destination buffer
 *          ph_prev_state       set NULL if one shot;specify previous decompress state
 *
 * Outputs: pv_dest_buf the destination buffer.
 *          pz_dest_len the really decompression length of destination buffer.
 *          ph_prev_state   next time decompress state
 *
 * Returns: CLR_OK                      Success.
 *          CLR_INV_ARG                 FAIL. Invalid arguments.
 *          CLR_OUT_OF_MEMORY           FAIL. The system is out of memory.
 *          CLR_DECOMPRESS_ERROR        FAIL. The routine occurred error 
 *                                      during decompression algorithm process.
 *          CLR_INSUFFICIENT_BUFFER     FAIL. The destination buffer is not enough.
 ----------------------------------------------------------------------------*/
static INT32 _cl_zip_decomp ( 
         const VOID*        pv_src_buf, 
         SIZE_T             z_src_len, 
         UINT32             ui4_param_type,  
         VOID*              pv_param,
         VOID*              pv_dest_buf, 
         SIZE_T             z_skip_len, 
         SIZE_T*            pz_dest_len,
         HANDLE_T*          ph_prev_state
         )
{
    INT32 i4_ret;
    z_stream* pt_stream=NULL;
    z_stream t_stream;
    HANDLE_TYPE_T e_type;
    UINT8 ui1_wbits = 0;
    CL_ZIP_DECOMPRESSION_T* pt_decomp_param = (CL_ZIP_DECOMPRESSION_T*)pv_param;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_src_buf == NULL) ||
        (pz_dest_len == NULL) ||
        (pv_dest_buf == NULL) ||
        (z_src_len == 0))
    {
        return CLR_INV_ARG;
    }
    if (pt_decomp_param != NULL)
    {
        ui1_wbits = pt_decomp_param->ui1_windowbits;
        if ((ui1_wbits < 8)
            ||(ui1_wbits > 15))
        {
            return CLR_INV_ARG;
        }
    }
    /************************************************************************/
    /* Perform decode algorithm                                             */
    /************************************************************************/
    if (ph_prev_state)
    {/* has previous state or first time here*/
        i4_ret = handle_get_type_obj(*ph_prev_state, &e_type, (VOID**)&pt_stream);
        if ((i4_ret == HR_OK)
            && (pt_stream)
            && (e_type == HT_CL_REPEATED_TYPE))
        {
            /* has previous state */
            i4_ret = _cl_zip_inflate(pt_stream, pv_src_buf, z_src_len, pv_dest_buf, z_skip_len, pz_dest_len);
            if ((i4_ret != Z_OK) &&
                (i4_ret != Z_STREAM_END) )
            {
                goto CL_ZIP_DECOMP_EXIT;
            }
        }
        else
        {
           /* first time here */
            pt_stream = x_mem_alloc(sizeof(z_stream));
            if (pt_stream == NULL)
            {
                return CLR_OUT_OF_MEMORY;
            }
            x_memset(pt_stream, 0, sizeof(z_stream));
            /* allocate handle*/
            if (HR_OK != handle_alloc(HT_CL_REPEATED_TYPE, pt_stream, NULL, _cl_zip_free_repeated_handle, ph_prev_state))
            {
                x_mem_free(pt_stream);
                return CLR_OUT_OF_HANDLE;
            }
            
            pt_stream->next_in = (Bytef*)pv_src_buf;
            pt_stream->avail_in = (uInt)z_src_len;
            pt_stream->zalloc = (alloc_func)0;
            pt_stream->zfree = (free_func)0;
            
            /* initial engine */
            if (pt_decomp_param)
            {
                i4_ret = inflateInit2(pt_stream, -ui1_wbits);
            }
            else
            {
                i4_ret = inflateInit2(pt_stream, -MAX_WBITS);
            }
            if (i4_ret != Z_OK) 
            {
                goto CL_ZIP_DECOMP_EXIT;
            }
            i4_ret = _cl_zip_inflate(pt_stream, pv_src_buf, z_src_len, pv_dest_buf, z_skip_len, pz_dest_len);
            if ((i4_ret != Z_OK) &&
                (i4_ret != Z_STREAM_END) )
            {
                goto CL_ZIP_DECOMP_EXIT;
            }
        }
    }
    else
    {
        x_memset(&t_stream, 0, sizeof(z_stream));
        t_stream.next_in = (Bytef*)pv_src_buf;
        t_stream.avail_in = (uInt)z_src_len;

        t_stream.zalloc = (alloc_func)0;
        t_stream.zfree = (free_func)0;
        if (pt_decomp_param)
        {
            i4_ret = inflateInit2(&t_stream, -ui1_wbits);
        }
        else
        {
            i4_ret = inflateInit2(&t_stream, -MAX_WBITS);
        }
        if (i4_ret != Z_OK) 
        {
            goto CL_ZIP_DECOMP_EXIT;
        }
        
        i4_ret = _cl_zip_inflate(&t_stream, pv_src_buf, z_src_len, pv_dest_buf, z_skip_len, pz_dest_len);
        if ((i4_ret != Z_OK) &&
            (i4_ret != Z_STREAM_END) )
        {
            goto CL_ZIP_DECOMP_EXIT;
        }
        
        i4_ret = inflateEnd(&t_stream);
        if (i4_ret != Z_OK) 
        {
            goto CL_ZIP_DECOMP_EXIT;
        }
    }

    /************************************************************************/
    /*   return                                                             */
    /************************************************************************/ 

CL_ZIP_DECOMP_EXIT:
    
    switch(i4_ret)
    {
    case Z_OK:
        return CLR_OK;
    case Z_MEM_ERROR:
        return CLR_OUT_OF_MEMORY;
    default:
        return CLR_DECOMPRESS_ERROR;
    }
}

static INT32 _cl_zip_decomp_ex ( 
         const VOID*        pv_src_buf, 
         SIZE_T             z_src_len, 
         UINT32             ui4_param_type,  
         VOID*              pv_param,
         VOID*              pv_dest_buf, 
         SIZE_T             z_skip_len, 
         SIZE_T*            pz_dest_len,
         HANDLE_T*          ph_prev_state
         )
{
    INT32 i4_ret;
    z_stream* pt_stream=NULL;
    z_stream t_stream;
    HANDLE_TYPE_T e_type;
    UINT8 ui1_wbits = 0;
    CL_ZIP_DECOMPRESSION_T* pt_decomp_param = (CL_ZIP_DECOMPRESSION_T*)pv_param;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_src_buf == NULL) ||
        (pz_dest_len == NULL) ||
        (pv_dest_buf == NULL) ||
        (z_src_len == 0))
    {
        return CLR_INV_ARG;
    }
    if (pt_decomp_param != NULL)
    {
        ui1_wbits = pt_decomp_param->ui1_windowbits;
        if ((ui1_wbits < 8)
            ||(ui1_wbits > 15))
        {
            return CLR_INV_ARG;
        }
    }
    /************************************************************************/
    /* Perform decode algorithm                                             */
    /************************************************************************/
    if (ph_prev_state)
    {/* has previous state or first time here*/
        i4_ret = handle_get_type_obj(*ph_prev_state, &e_type, (VOID**)&pt_stream);
        if ((i4_ret == HR_OK)
            && (pt_stream)
            && (e_type == HT_CL_REPEATED_TYPE))
        {
            /* has previous state */
            i4_ret = _cl_zip_inflate(pt_stream, pv_src_buf, z_src_len, pv_dest_buf, z_skip_len, pz_dest_len);
            if ((i4_ret != Z_OK) &&
                (i4_ret != Z_STREAM_END) )
            {
                goto CL_ZIP_DECOMP_EXIT;
            }
        }
        else
        {
           /* first time here */
            pt_stream = x_mem_alloc(sizeof(z_stream));
            if (pt_stream == NULL)
            {
                return CLR_OUT_OF_MEMORY;
            }
            x_memset(pt_stream, 0, sizeof(z_stream));
            /* allocate handle*/
            if (HR_OK != handle_alloc(HT_CL_REPEATED_TYPE, pt_stream, NULL, _cl_zip_free_repeated_handle, ph_prev_state))
            {
                x_mem_free(pt_stream);
                return CLR_OUT_OF_HANDLE;
            }
            
            pt_stream->next_in = (Bytef*)pv_src_buf;
            pt_stream->avail_in = (uInt)z_src_len;
            pt_stream->zalloc = (alloc_func)0;
            pt_stream->zfree = (free_func)0;
            
            /* initial engine */
            if (pt_decomp_param)
            {
                i4_ret = inflateInit2(pt_stream, ui1_wbits);
            }
            else
            {
                i4_ret = inflateInit2(pt_stream, MAX_WBITS);
            }
            if (i4_ret != Z_OK) 
            {
                goto CL_ZIP_DECOMP_EXIT;
            }
            i4_ret = _cl_zip_inflate(pt_stream, pv_src_buf, z_src_len, pv_dest_buf, z_skip_len, pz_dest_len);
            if ((i4_ret != Z_OK) &&
                (i4_ret != Z_STREAM_END) )
            {
                goto CL_ZIP_DECOMP_EXIT;
            }
        }
    }
    else
    {
        x_memset(&t_stream, 0, sizeof(z_stream));
        t_stream.next_in = (Bytef*)pv_src_buf;
        t_stream.avail_in = (uInt)z_src_len;

        t_stream.zalloc = (alloc_func)0;
        t_stream.zfree = (free_func)0;
        if (pt_decomp_param)
        {
            i4_ret = inflateInit2(&t_stream, ui1_wbits);
        }
        else
        {
            i4_ret = inflateInit2(&t_stream, MAX_WBITS);
        }
        if (i4_ret != Z_OK) 
        {
            goto CL_ZIP_DECOMP_EXIT;
        }
        
        i4_ret = _cl_zip_inflate(&t_stream, pv_src_buf, z_src_len, pv_dest_buf, z_skip_len, pz_dest_len);
        if ((i4_ret != Z_OK) &&
            (i4_ret != Z_STREAM_END) )
        {
            goto CL_ZIP_DECOMP_EXIT;
        }
        
        i4_ret = inflateEnd(&t_stream);
        if (i4_ret != Z_OK) 
        {
            goto CL_ZIP_DECOMP_EXIT;
        }
    }

    /************************************************************************/
    /*   return                                                             */
    /************************************************************************/ 

CL_ZIP_DECOMP_EXIT:
    
    switch(i4_ret)
    {
    case Z_OK:
        return CLR_OK;
    case Z_MEM_ERROR:
        return CLR_OUT_OF_MEMORY;
    default:
        return CLR_DECOMPRESS_ERROR;
    }
}


/*-----------------------------------------------------------------------------
 * Name:  _cl_eng_get_fct
 *
 * Description: The API of engine interface and it is "Get" wrapper function. 
 *              It wrapes "x_cl_get_compress_len", "x_cl_get_decompress_len", 
 *              "x_cl_get_num_entries", "x_cl_get_query_info_len" and "x_cl_query_info"
 *
 * Inputs:  e_get_type      specify the type of "get" function. Please refer to section 6.2.2        
 *          pv_in_arg       point to the input argument which is needed to perform "get" function.
 *          z_in_arg        point to the input argument which is needed to perform "get" function.
 *                          It can be z_src_len or e_type in x_cl_get_query_info_len.
 *          ui4_param_type  specify type of decompression paramters, set CL_NULL_PARAM_TYPE is using default
 *          pv_param        specify decompression paramters, set NULL is using default
 *          pv_filter       specify the filter argument such as index number of an entry.
 *
 * Outputs: pv_info         specified the buffer which contains information returned by the engine.
 *          pz_info_len     specified the length of information buffer.
 *
 * Returns: CLR_OK                      Success.
 *          CLR_INV_ARG                 FAIL. Invalid arguments.
 *          CLR_INV_OBJ                 FAIL.The object is not valid any more.
 *          CLR_OUT_OF_MEMORY           FAIL. The system is out of memory.
 *          CLR_DECOMPRESS_ERROR        FAIL. The routine occurred error
                                        during decompression algorithm process.
 *          CLR_COMPRESS_ERROR          FAIL. The routine occurred error 
 *                                      during compression algorithm process.
 *          CLR_INSUFFICIENT_BUFFER     FAIL. The destination buffer is not enough.
 ----------------------------------------------------------------------------*/
extern INT32 _cl_zip_eng_get_fct (
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
#ifdef CL_SUPPORT_ARCHIVE
    HANDLE_T* pt_handle = pv_in_arg;
#endif
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( ((INT32)e_get_type < CL_GET_COMPRESS_LEN_TYPE) ||
        ((INT32)e_get_type > CL_QUERY_INFO_TYPE) )
    {
        return CLR_INV_ARG;
    }
    if (pv_param != NULL)
    {
        if ((ui4_param_type != CL_ZIP_ENGINE_COMPRESS_PARAM_TYPE)
            && (ui4_param_type != CL_ZIP_ENGINE_DECOMPRESS_PARAM_TYPE))
        {
            return CLR_INV_ARG;
        }
    }
    /************************************************************************/
    /* Perform decode algorithm                                             */
    /************************************************************************/
    switch(e_get_type)
    {
    case CL_GET_COMPRESS_LEN_TYPE:
        return _cl_get_zip_comp_len(pv_in_arg, z_in_arg, ui4_param_type, pv_param, pz_info_len);
    case CL_GET_DECOMPRESS_LEN_TYPE:
        return _cl_get_zip_decomp_len(pv_in_arg, z_in_arg, ui4_param_type, pv_param, pz_info_len);
    case CL_GET_DECOMPRESS_LEN_TYPE_EX:
        return _cl_get_zip_decomp_len_ex(pv_in_arg, z_in_arg, ui4_param_type, pv_param, pz_info_len);
#ifdef CL_SUPPORT_ARCHIVE
    case CL_GET_DECOMPRESS_ENTRY_LEN_TYPE:
        return _cl_get_decompress_entry_len(*pt_handle, pz_info_len);        
    case CL_GET_NUM_ENTRIES_TYPE:
        return _cl_get_num_entries(*pt_handle, (UINT32*)pz_info_len);
    case CL_GET_QUERY_INFO_LEN_TYPE:
            return _cl_get_query_info_len(*pt_handle, (CL_QUERY_INFO_TYPE_T)z_in_arg, pv_filter, pz_info_len);
    case CL_QUERY_INFO_TYPE:
            return _cl_query_info(*pt_handle, (CL_QUERY_INFO_TYPE_T)z_in_arg, pv_filter, pv_info, pz_info_len);
#endif
    default:
        return CLR_INV_ARG;
    }
}

/*-----------------------------------------------------------------------------
 * Name:  _cl_eng_compress_fct
 *
 * Description: The API of engine interface and it is "Compress" wrapper function. 
 *              It wrapes "x_cl_compress".
 *
 * Inputs:  e_compress_type     specify the type of "compress" function. Please refer to section 6.2.3          
 *          pv_src_buf          point to the source buffer, which contains source data waiting for compression
 *          z_src_len           specify the length of the source buffer.
 *          ui4_param_type      specify type of compression paramters, set CL_NULL_PARAM_TYPE is using default
 *          pv_param            specify compression paramters, set NULL is using default
 *          pz_dest_len         specified the length of destination buffer.
 *
 * Outputs: pv_dest_buf         point to the destination buffer.
 *          pz_dest_len         specify the actual required length of destination buffer.
 *
 * Returns: CLR_OK                      Success.
 *          CLR_INV_ARG                 FAIL. Invalid arguments.
 *          CLR_OUT_OF_MEMORY           FAIL. The system is out of memory.
 *          CLR_COMPRESS_ERROR          FAIL. The routine occurred error 
 *                                      during compression algorithm process.
 *          CLR_INSUFFICIENT_BUFFER     FAIL. The destination buffer is not enough.
 ----------------------------------------------------------------------------*/
extern INT32 _cl_zip_eng_compress_fct (
            CL_ENG_COMPRESS_TYPE_T   e_compress_type,
            const VOID*              pv_src_buf,
            SIZE_T                   z_src_len,
            UINT32                   ui4_param_type,  
            VOID*                    pv_param,
            VOID*                    pv_dest_buf,
            SIZE_T*                  pz_dest_len
            )

{
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (e_compress_type != CL_COMPRESS_TYPE)
    {
        return CLR_INV_ARG;
    }
    if (pv_param != NULL)
    {
        if (ui4_param_type != CL_ZIP_ENGINE_COMPRESS_PARAM_TYPE)
        {
            return CLR_INV_ARG;
        }
    }
    /************************************************************************/
    /* Perform decode algorithm                                             */
    /************************************************************************/
    switch(e_compress_type)
    {
    case CL_COMPRESS_TYPE:
        return _cl_zip_comp(pv_src_buf, z_src_len, ui4_param_type, pv_param, pv_dest_buf, pz_dest_len);
    default:
        return CLR_INV_ARG;
    }
}

/*-----------------------------------------------------------------------------
 * Name:  _cl_eng_decompress_fct
 *
 * Description: The API of engine interface and it is "Decompress" wrapper function. 
 *              It wrapes "x_cl_decompress" and "x_cl_decompress_entry".
 *
 * Inputs:  e_decompress_type   specify the type of "decompress" function. Please refer to section 6.2.4         
 *          e_op                specify the operation type such as oneshot or repeated.
 *          pv_src_buf          point to the source buffer, which contains source data waiting for decompression
 *          z_src_len           specify the length of the source buffer.
 *          ui4_param_type      specify type of compression paramters, set CL_NULL_PARAM_TYPE is using default
 *          pv_param            specify compression paramters, set NULL is using default
 *          z_skip_len          indicate the number of bytes to skip from the beginning of decompression output. 
 *          ph_prev_state       set NULL if one shot;specify previous decompress state
 *
 * Outputs: pv_dest_buf         point to the destination buffer.
 *          pz_dest_len         specify the really decompression length of destination buffer.
 *          ph_prev_state       next time decompress state
 *
 * Returns: CLR_OK                      Success.
 *          CLR_INV_OBJ                 FAIL.The object is not valid any more.
 *          CLR_INV_ARG                 FAIL. Invalid arguments.
 *          CLR_OUT_OF_MEMORY           FAIL. The system is out of memory.
 *          CLR_DECOMPRESS_ERROR        FAIL. The routine occurred error 
 *                                      during decompression algorithm process.
 *          CLR_INSUFFICIENT_BUFFER     FAIL. The destination buffer is not enough.
 ----------------------------------------------------------------------------*/
extern INT32 _cl_zip_eng_decompress_fct (
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
#ifdef CL_SUPPORT_ARCHIVE
    HANDLE_T* ph_handle = pv_src_buf;
#endif
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( (e_decompress_type != CL_DECOMPRESS_TYPE) &&
        (e_decompress_type != CL_DECOMPRESS_ENTRY_TYPE) && 
        (e_decompress_type != CL_DECOMPRESS_TYPE_EX))
    {
        return CLR_INV_ARG;
    }
    if (pv_param != NULL)
    {
        if (ui4_param_type != CL_ZIP_ENGINE_DECOMPRESS_PARAM_TYPE)
        {
            return CLR_INV_ARG;
        }
    }
    /************************************************************************/
    /* Perform decode algorithm                                             */
    /************************************************************************/
    switch(e_decompress_type)
    {
    case CL_DECOMPRESS_TYPE:
        return _cl_zip_decomp(pv_src_buf, z_src_len, ui4_param_type, pv_param, pv_dest_buf, z_skip_len, pz_dest_len, ph_prev_state);
    case CL_DECOMPRESS_TYPE_EX:
        return _cl_zip_decomp_ex(pv_src_buf, z_src_len, ui4_param_type, pv_param, pv_dest_buf, z_skip_len, pz_dest_len, ph_prev_state);
#ifdef CL_SUPPORT_ARCHIVE
    case CL_DECOMPRESS_ENTRY_TYPE:
        if (ph_prev_state)
        {
            *ph_prev_state = NULL;
        }
        return _cl_decomp_entry(*ph_handle, pv_dest_buf, z_skip_len, pz_dest_len);
#endif
    default:
        return CLR_INV_ARG;
    }
}
#ifdef CL_SUPPORT_ARCHIVE
/*-----------------------------------------------------------------------------
 * Name:  _cl_eng_open_fct
 *
 * Description: The API of engine interface and it is "Open" wrapper function. 
 *              It wrapes "x_cl_open_archive", "x_cl_open_entry_by_name" and "x_cl_open_entry_by_index".
 *
 * Inputs:  e_open_type     specify the type of "Open" function. Please refer to section 6.2.5         
 *          h_parent        specify the parent handle
 *          pv_filter       specify the filter argument of "open function", such as index number of an entry.
 *          ui1_mode        specified the access permit mode. 
 *
 * Outputs: ph_child   retrieve the child handle.
 *
 * Returns: CLR_OK                      Success.
 *          CLR_OUT_OF_HANDLE           FAIL. There is out of handle.
 *          CLR_OUT_OF_MEMORY           There is insufficient memory available to allocate structure.
 *          CLR_INV_HANDLE              FAIL. The handle is invalid.
 *          CLR_INV_ARG                 FAIL. Invalid arguments.
 *          CLR_BAD_ARCHIVE                The archive is corrupted.    
 *          CLR_FILE_OR_DIR_MISSING        The input arguments of the file or directory name
 *                                      are invalid.
 ----------------------------------------------------------------------------*/
extern INT32 _cl_zip_eng_open_fct (
            CL_ENG_OPEN_TYPE_T    e_open_type,
            HANDLE_T            h_parent,
            VOID*                pv_filter,
            UINT8                ui1_mode, 
            HANDLE_T*           ph_child
            )
{
    INT32 i4_res;
    UINT32* pui4_index;
    VOID* pv_data;
    CL_ENG_OBJ_T* pt_eng_obj;
    CL_ENTRY_OBJ_T* pt_ety_obj;
    CL_ARCHIVE_OBJ_T* pt_arc_obj;
    HANDLE_TYPE_T e_type;
    x_cl_nfy_fct pf_nfy_fct;
        
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( ((INT32)e_open_type < CL_OPEN_ARCHIVE_TYPE) ||
        ((INT32)e_open_type > CL_OPEN_ENTRY_BY_INDEX_TYPE) )
    {
        return CLR_INV_ARG;
    }
    /************************************************************************/
    /* Perform decode algorithm                                             */
    /************************************************************************/
    switch(e_open_type)
    {
    case CL_OPEN_ARCHIVE_TYPE:
        if (HR_OK != handle_get_type_obj(h_parent, &e_type, (VOID**)&pt_eng_obj))
        {
            return CLR_INV_HANDLE;
        }
        if (e_type != HT_CL_ENG_TYPE)
        {
            return CLR_INV_HANDLE;
        }    
        i4_res = _cl_open_archive(pv_filter, ui1_mode, &pv_data);
        if (i4_res == CLR_OK)
        {
            pt_arc_obj = x_mem_alloc(sizeof(CL_ARCHIVE_OBJ_T));
            if (pt_arc_obj == NULL)
            {
                return CLR_OUT_OF_MEMORY;
            }
            x_memset(pt_arc_obj, 0, sizeof(CL_ARCHIVE_OBJ_T));
			
            /* lock link list operation semaphore and wait link list update complete */
            if ( x_sema_lock(pt_eng_obj->h_linklist_op_lock, X_SEMA_OPTION_WAIT ) != OSR_OK )
            {
                return CLR_INV_HANDLE;
            }
			
            /* fill link */
            SLIST_INIT(&(pt_arc_obj->t_child_entry_list));
            SLIST_INSERT_HEAD(pt_arc_obj, &(pt_eng_obj->t_child_archive_list), t_archive_link);
			
            /* unlock link list operation semaphore */
            if ( x_sema_unlock(pt_eng_obj->h_linklist_op_lock) != OSR_OK )
            {
                return CLR_INV_HANDLE;
            }
			
            /* fill data */
            pt_arc_obj->pf_nfy = pt_eng_obj->pf_nfy;
            pt_arc_obj->pv_tag = pt_eng_obj->pv_tag;
            pt_arc_obj->pt_eng_reg = pt_eng_obj->pt_eng_reg;
            pt_arc_obj->pv_data = pv_data;
            pt_arc_obj->pt_eng_obj = pt_eng_obj;
            pt_arc_obj->h_self = *ph_child;
            
            /* allocate handle*/
            if (HR_OK != handle_alloc(HT_CL_ARC_TYPE, pt_arc_obj, NULL, _cl_zip_free_handle, ph_child))
            {
                return CLR_OUT_OF_HANDLE;
            }
            return CLR_OK;
        }
        else
        {
            return i4_res;
        }
    case CL_OPEN_ENTRY_BY_NAME_TYPE:
        if (HR_OK != handle_get_type_obj(h_parent, &e_type, (VOID**)&pt_arc_obj))
        {
            return CLR_INV_HANDLE;
        }
        if (e_type != HT_CL_ARC_TYPE)
        {
            return CLR_INV_HANDLE;
        }
		
        pt_eng_obj = pt_arc_obj->pt_eng_obj;
        
        /* check if already open a entry */
        if (!SLIST_IS_EMPTY(&(pt_arc_obj->t_child_entry_list)))
        {
            /* auto close a entry */
            while((pt_ety_obj = SLIST_FIRST(&pt_arc_obj->t_child_entry_list)) != NULL)
            {
                /* real close entry */
                _cl_close_archive(pt_ety_obj->pv_data);
				
                /* lock link list operation semaphore and wait link list update complete */
                if ( x_sema_lock(pt_eng_obj->h_linklist_op_lock, X_SEMA_OPTION_WAIT ) != OSR_OK )
                {
                    return CLR_INV_HANDLE;
                }
				
                /* remove link */
                SLIST_REMOVE(pt_ety_obj, t_entry_link);

                /* unlock link list operation semaphore */
                if ( x_sema_unlock(pt_eng_obj->h_linklist_op_lock) != OSR_OK )
                {
                    return CLR_INV_HANDLE;
                }
				
                /* free handle */
                pf_nfy_fct = pt_ety_obj->pf_nfy;
                if(pf_nfy_fct)
                {
                    pf_nfy_fct(pt_ety_obj->h_self, pt_ety_obj->pv_tag, CL_NFY_HANDLE_CLOSED_TYPE);
                }
                handle_free(pt_ety_obj->h_self, TRUE);
            }
        }
        
        i4_res = _cl_open_entry_by_name(pt_arc_obj->pv_data, pv_filter, &pv_data);
        if (i4_res == CLR_OK)
        {
            pt_ety_obj = x_mem_alloc(sizeof(CL_ENTRY_OBJ_T));
            if (pt_ety_obj == NULL)
            {
                return CLR_OUT_OF_MEMORY;
            }
            x_memset(pt_ety_obj, 0, sizeof(CL_ENTRY_OBJ_T));

            /* lock link list operation semaphore and wait link list update complete */
            if ( x_sema_lock(pt_eng_obj->h_linklist_op_lock, X_SEMA_OPTION_WAIT ) != OSR_OK )
            {
                return CLR_INV_HANDLE;
            }
			
            /* fill link */
            SLIST_INSERT_HEAD(pt_ety_obj, &(pt_arc_obj->t_child_entry_list), t_entry_link);
			
            /* unlock link list operation semaphore */
            if ( x_sema_unlock(pt_eng_obj->h_linklist_op_lock) != OSR_OK )
            {
                return CLR_INV_HANDLE;
            }
				
            /* fill data */
            pt_ety_obj->pt_eng_obj = pt_arc_obj->pt_eng_obj;
            pt_ety_obj->pf_nfy = pt_arc_obj->pf_nfy;
            pt_ety_obj->pv_tag = pt_arc_obj->pv_tag;
            pt_ety_obj->pt_eng_reg = pt_arc_obj->pt_eng_reg;
            pt_ety_obj->pv_data = pv_data;
            pt_ety_obj->h_self = *ph_child;

            /* allocate handle*/
            if (HR_OK != handle_alloc(HT_CL_ETY_TYPE, pt_ety_obj, NULL, _cl_zip_free_handle, ph_child))
            {
                return CLR_OUT_OF_HANDLE;
            }
            return CLR_OK;
        }
        else
        {
            return i4_res;
        }
    case CL_OPEN_ENTRY_BY_INDEX_TYPE:
        if (pv_filter)
        {
            pui4_index = pv_filter;
            if (HR_OK != handle_get_type_obj(h_parent, &e_type, (VOID**)&pt_arc_obj))
            {
                return CLR_INV_HANDLE;
            }
            
            if (e_type != HT_CL_ARC_TYPE)
            {
                return CLR_INV_HANDLE;
            }
			
            pt_eng_obj = pt_arc_obj->pt_eng_obj;
            
            /* check if already open a entry */
            if (!SLIST_IS_EMPTY(&(pt_arc_obj->t_child_entry_list)))
            {
                /* auto close a entry */
                while((pt_ety_obj = SLIST_FIRST(&pt_arc_obj->t_child_entry_list)) != NULL)
                {
                    /* real close entry */
                    _cl_close_archive(pt_ety_obj->pv_data);
					
                    /* lock link list operation semaphore and wait link list update complete */
                    if ( x_sema_lock(pt_eng_obj->h_linklist_op_lock, X_SEMA_OPTION_WAIT ) != OSR_OK )
                    {
                        return CLR_INV_HANDLE;
                    }
					
                    /* remove link */
                    SLIST_REMOVE(pt_ety_obj, t_entry_link);

                    /* unlock link list operation semaphore */
                    if ( x_sema_unlock(pt_eng_obj->h_linklist_op_lock) != OSR_OK )
                    {
                        return CLR_INV_HANDLE;
                    }
					
                    /* free handle */
                    pf_nfy_fct = pt_ety_obj->pf_nfy;
                    if(pf_nfy_fct)
                    {
                        pf_nfy_fct(pt_ety_obj->h_self, pt_ety_obj->pv_tag, CL_NFY_HANDLE_CLOSED_TYPE);
                    }
                    handle_free(pt_ety_obj->h_self, TRUE);
                }
            }
            
            i4_res = _cl_open_entry_by_index(pt_arc_obj->pv_data, *pui4_index, &pv_data);
            if (i4_res == CLR_OK)
            {
                pt_ety_obj = x_mem_alloc(sizeof(CL_ENTRY_OBJ_T));
                if (pt_ety_obj == NULL)
                {
                    return CLR_OUT_OF_MEMORY;
                } 
                x_memset(pt_ety_obj, 0, sizeof(CL_ENTRY_OBJ_T));
				
                /* lock link list operation semaphore and wait link list update complete */
                if ( x_sema_lock(pt_eng_obj->h_linklist_op_lock, X_SEMA_OPTION_WAIT ) != OSR_OK )
                {
                    return CLR_INV_HANDLE;
                }
				
                /* fill link */
                SLIST_INSERT_HEAD(pt_ety_obj, &(pt_arc_obj->t_child_entry_list), t_entry_link);
				
                /* unlock link list operation semaphore */
                if ( x_sema_unlock(pt_eng_obj->h_linklist_op_lock) != OSR_OK )
                {
                    return CLR_INV_HANDLE;
                }
				
                /* fill data */
                pt_ety_obj->pt_eng_obj = pt_arc_obj->pt_eng_obj;
                pt_ety_obj->pf_nfy = pt_arc_obj->pf_nfy;
                pt_ety_obj->pv_tag = pt_arc_obj->pv_tag;
                pt_ety_obj->pt_eng_reg = pt_arc_obj->pt_eng_reg;
                pt_ety_obj->pv_data = pv_data;
                pt_ety_obj->h_self = *ph_child;

                /* allocate handle*/
                if (HR_OK != handle_alloc(HT_CL_ETY_TYPE, pt_ety_obj, NULL, _cl_zip_free_handle, ph_child))
                {
                    return CLR_OUT_OF_HANDLE;
                }
                return CLR_OK;
            }
            else
            {
                return i4_res;
            }
        }
        else
        {
            return CLR_INV_ARG;
        }
    default:
        return CLR_INV_ARG;
    }
}

/*-----------------------------------------------------------------------------
 * Name:  _cl_eng_close_fct
 *
 * Description: The API of engine interface and it is "Close" wrapper function. 
 *              It wrapes "x_cl_close_archive"and "x_cl_close_entry".
 *
 * Inputs:  h_handle   Specify the handle will be closed.
 *
 * Outputs: -
 *
 * Returns: CLR_OK                      Success.
 *          CLR_INV_OBJ                 FAIL.The object is not valid any more.
 *          CLR_INV_ARG                 FAIL. Invalid arguments.
 *          CLR_INV_HANDLE              FAIL. The handle is invalid.
 ----------------------------------------------------------------------------*/
extern INT32 _cl_zip_eng_close_fct (
            HANDLE_T            h_handle)
{
    INT32 i4_res; 
    VOID* pv_obj; 
    HANDLE_TYPE_T e_type;
    CL_ARCHIVE_OBJ_T *pt_arc_obj;
    CL_ENTRY_OBJ_T* pt_ety_obj;
    x_cl_nfy_fct pf_nfy_fct;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/ 
    
    /************************************************************************/
    /* Perform decode algorithm                                             */
    /************************************************************************/
    if (HR_OK != handle_get_type_obj(h_handle, &e_type, &pv_obj))
    {
        return CLR_INV_HANDLE;
    }
    
    switch(e_type)
    {
    case HT_CL_ARC_TYPE:
        pt_arc_obj = (CL_ARCHIVE_OBJ_T*)pv_obj;
        
        /* execute sub-entry's close  */
        while((pt_ety_obj = SLIST_FIRST(&pt_arc_obj->t_child_entry_list)) != NULL)
        {
            /* lock link list operation semaphore and wait link list update complete */
            if ( x_sema_lock(pt_ety_obj->pt_eng_obj->h_linklist_op_lock, X_SEMA_OPTION_WAIT ) != OSR_OK )
            {
                return CLR_INV_HANDLE;
            }
			
            /* remove link */
            SLIST_REMOVE(pt_ety_obj, t_entry_link);
			
            /* unlock link list operation semaphore */
            if ( x_sema_unlock(pt_ety_obj->pt_eng_obj->h_linklist_op_lock) != OSR_OK )
            {
                return CLR_INV_HANDLE;
            }
  
            /* notify child entry */
            pf_nfy_fct = pt_ety_obj->pf_nfy;
            if(pf_nfy_fct)
            {
                pf_nfy_fct(pt_ety_obj->h_self, pt_ety_obj->pv_tag, CL_NFY_HANDLE_CLOSED_TYPE);
            }
            
            /* execute sub-entry's close  */
            _cl_close_entry(pt_ety_obj->pv_data);
            
            /* handle free */
            handle_free(pt_ety_obj->h_self,TRUE); 
        }
		
        /* lock link list operation semaphore and wait link list update complete */
        if ( x_sema_lock(pt_arc_obj->pt_eng_obj->h_linklist_op_lock, X_SEMA_OPTION_WAIT ) != OSR_OK )
        {
             return CLR_INV_HANDLE;
        }
			
        /* remove link */
        SLIST_REMOVE(pt_arc_obj, t_archive_link);
		
        /*unlock link list operation semaphore */
        if ( x_sema_unlock(pt_arc_obj->pt_eng_obj->h_linklist_op_lock) != OSR_OK )
        {
            return CLR_INV_HANDLE;
        }
			
        /* notify archive close */
        pf_nfy_fct = pt_arc_obj->pf_nfy;
        if(pf_nfy_fct)
        {
            pf_nfy_fct(pt_arc_obj->h_self, pt_arc_obj->pv_tag, CL_NFY_HANDLE_CLOSED_TYPE);
        }
        
        /* execute archive's close  */
        i4_res = _cl_close_archive(pt_arc_obj->pv_data);
        
        /* handle free */
        handle_free(h_handle,TRUE);
        return i4_res;
        
    case HT_CL_ETY_TYPE:
        pt_ety_obj = (CL_ENTRY_OBJ_T*)pv_obj;

        /* lock link list operation semaphore and wait link list update complete */
        if ( x_sema_lock(pt_ety_obj->pt_eng_obj->h_linklist_op_lock, X_SEMA_OPTION_WAIT ) != OSR_OK )
        {
             return CLR_INV_HANDLE;
        }
		
        /* remove link */
        SLIST_REMOVE(pt_ety_obj, t_entry_link);
		
        /*unlock link list operation semaphore */
        if ( x_sema_unlock(pt_ety_obj->pt_eng_obj->h_linklist_op_lock) != OSR_OK )
        {
            return CLR_INV_HANDLE;
        }
		
        /* notify entry close */
        pf_nfy_fct = pt_ety_obj->pf_nfy;
        if(pf_nfy_fct)
        {
            pf_nfy_fct(pt_ety_obj->h_self, pt_ety_obj->pv_tag, CL_NFY_HANDLE_CLOSED_TYPE);
        }
        
        /* execute archive's close  */
        i4_res = _cl_close_entry(pt_ety_obj->pv_data);
        
        /* handle free */
        handle_free(h_handle,TRUE);
        return i4_res;
    default:
        return CLR_INV_ARG;
    }
}
#endif
