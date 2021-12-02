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
 * $RCSfile: str_buf.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *   This file contains implementation for the mini-buffer module.
 *   The mini-buffer is used to store character string with minimal
 *   overhead needed.
 *
 *   The difference between using this mini buffer to store string
 *   and dynamically allocate string using x_mem_alloc() is that multiple
 *   strings are stored in one contiguous buffer, and that reference to the
 *   string stored in the mini-buffer will be via a string id (e.g.,
 *   an index), not via direct memory address.
 *
 *   The reason for using the string id instead of direct memory address is
 *   to facilate storing the mini-string buffer on filesystem. By using
 *   the string id, the client of mini-string buffer can access the same
 *   character string after the mini-string buffer is read back into
 *   memory from filesystem.
 *   
 *   The mini-buffer can grow as needed
 *
 *   The length of the character string stored in mini-buffer is limited
 *   to 255 characters (bytes).  
 *
 *   The maximum size of mini-string buffer size is limited to 65536 bytes.
 *
 *
 *   The string buffer consists of two part:
 *
 *   1. A buffer where a string object is stored.
 *   2. A array containing offset of string object in the buffer.
 *
 *   The first byte of the string object is the len (1-255 char) of
 *   the string to follow.  (The string object is similar to a Pacal string).
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
  include files
  --------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "util/x_end_cnvt.h"
#include "dbg/x_dbg.h"
#include "file_mngr/x_fm.h"
#include "util/u_str_buf.h"
#include "util/x_mem_buf.h"

/*-------------------------------------------------------------
 * Convert the return code of mem_buf to str_buf's.
 * Currently the return code assignment for them is the same
 *-------------------------------------------------------------*/
#define CONV_RET(_ret_)             _ret_

/*-----------------------------------------------------------------------------
 * Name: x_str_buf_create
 *
 * Description: Allocate a STR_BUF_T structure with user provided paramters.
 *
 * Inputs:  -
 *   ui2_nb_of_str  Initial number of string object array. If less than
 *                  STR_BUF_MIN_ARY_LEN, it will be set to the minimum
 *                  value.
 *
 *   ui4_buf_sz     Initial size of string buffer to store string.
 *                  If less than STR_BUF_MIN_BUF_SZ, then
 *                  it will be set to the minimum value.
 *
 * Outputs: -
 *   ppt_strbuf     Ref to a pointer to STR_BUF_T structure. On return,
 *                  it ref to allocated STR_BUF_T structure.
 *   
 * Returns:
 *   STRBUFR_OK          Success.
 *   STRBUFR_INV_ARG     'ppt_strbuf' is NULL.
 *   STRBUFR_OUT_OF_MEM  Out of memory.
 ----------------------------------------------------------------------------*/
INT32 x_str_buf_create
(
    UINT16           ui2_initial_nb_of_str,
    UINT32           ui4_initial_buf_sz,
    STR_BUF_T**      ppt_strbuf
)
{
    return CONV_RET(x_mem_buf_create(ui2_initial_nb_of_str,
                                     ui4_initial_buf_sz,
                                     (MEM_BUF_T**) ppt_strbuf));
}

/*-----------------------------------------------------------------------------
 * Name: x_str_buf_write_string
 *
 * Description:
 *   Make copy the input string and store the string in the string buffer.
 *   Return a string ID to the string stored in string buffer.
 *
 * Inputs:  -
 *   pt_strbuf    Ref to a pointer to STR_BUF_T structure.
 *
 *   ps_string    Specify the string to store. 
 *
 * Outputs: -
 *   pt_string_id An ID value referencing the copy of string in the
 *                   string buffer. 
 *   
 * Returns: -
 *   STRBUFR_OK         Success.
 *   STRBUFR_OUT_OF_MEM Failed to allocate space.
 *   STRBUFR_INV_ARG    pt_string_id is null
 ----------------------------------------------------------------------------*/
INT32 x_str_buf_write_string
(
    STR_BUF_T*       pt_strbuf,
    const CHAR*      ps_string,
    STRING_ID_T*     pt_string_id
)
{
    INT32            i4_rc     = STRBUFR_OK;
    UINT32           ui4_len    = 0;

    if ( pt_string_id != NULL )
    {
        *pt_string_id = STR_BUF_NULL_STR_ID ;
            
        if ( ps_string                    != NULL     &&
             (ui4_len=x_strlen(ps_string)) >  0        
            )
        {
            i4_rc = x_mem_buf_write_object((MEM_BUF_T*) pt_strbuf,
                                           (VOID*) ps_string,
                                           ui4_len,
                                           (OBJECT_ID_T*) pt_string_id);
            i4_rc = CONV_RET(i4_rc);
        }
    }
    else
    {
        i4_rc = STRBUFR_INV_ARG;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: x_str_buf_get_strlen
 *
 * Description: This API get the length of string.
 *             
 *
 * Inputs:  -
 *   pt_strbuf    Ref to a pointer to STR_BUF_T structure.
 *
 *   t_string_id     Specify the string ID. 
 *
 * Outputs: -
 *   pui4_len     Length of the string object ref by the string ID.
 *
 * Returns:
 *   STRBUFR_OK          Success.
 *   STRBUFR_INV_STR_ID  string object is not found.
 ----------------------------------------------------------------------------*/
INT32 x_str_buf_get_strlen
(
    STR_BUF_T*       pt_strbuf,
    STRING_ID_T      t_string_id,
    UINT32*          pui4_len
)
{
    return CONV_RET(x_mem_buf_get_object_len((MEM_BUF_T*) pt_strbuf,
                                             (OBJECT_ID_T) t_string_id,
                                             pui4_len));
}

/*-----------------------------------------------------------------------------
 * Name: x_str_buf_get_string
 *   copy the string ref by the specified string id to the user provided
 *   buffer.
 *   If the buffer is too small, up to the buffer size - 1 bytes are copied.
 *   If the string ID is NULL or invalid, then null string is returned.
 *
 * Description: 
 *   pt_strbuf     Ref to a mini-buffer structure.           
 *
 *   t_string_id   Specify string ID to retrive.
 *
 *   pz_str_sz     size of 'ps_string' buffer.
 *
 * Inputs:  -
 *   ps_string     A buffer containing the retrived char string.
 *
 *   pz_str_len     Len of the string in 'ps_string' (not including the ending
 *                 '\0'.
 *
 * Outputs: -
 *
 * Returns: -
 *   STRBUFR_OK:      Success.
 *   STRBUFR_INV_ARG: ps_string or pz_str_len is NULL, or *pz_str_len is 0.
 *   STRBUFR_INV_STR_ID 't_string_id' is not a valid value or is not found.
 *   STRBUFR_INPUT_BUF_TOO_SMALL  Input buffer ps_string is too small.
 ----------------------------------------------------------------------------*/
INT32 x_str_buf_get_string
(
    STR_BUF_T*       pt_strbuf,
    STRING_ID_T      t_string_id,
    CHAR*            ps_string,
    SIZE_T*          pz_str_len
)
{
    INT32            i4_rc;
    SIZE_T           z_buf_len;

    if ( ps_string   != NULL   &&
         pz_str_len  != NULL   &&
         *pz_str_len >  0 )
    {
        z_buf_len = *pz_str_len;

        i4_rc=x_mem_buf_get_object((MEM_BUF_T*) pt_strbuf,
                                   (OBJECT_ID_T) t_string_id,
                                   (VOID*) ps_string,
                                   pz_str_len);
        if (i4_rc == MEMBUFR_OK || i4_rc == MEMBUFR_INPUT_BUF_TOO_SMALL)
        {
            if (*pz_str_len == z_buf_len)
            {
                /* the string length equals to the output buffer length, which
                 * means we have to replace the last char with the trailing \0
                 */
                (*pz_str_len)--;
                i4_rc = STRBUFR_INPUT_BUF_TOO_SMALL;
            }
            else
            {
                i4_rc = STRBUFR_OK;
            }
            ps_string[*pz_str_len]='\0';
        }
        else
        {
            /* If the string ID has a length of 0 or is invalid,
               then return a null string
            */
            ps_string[0] ='\0';
            /* *pz_str_len has already been set to 0 in x_mem_buf_get_object*/
            i4_rc = CONV_RET(i4_rc);
        }
    }
    else
    {
        i4_rc = STRBUFR_INV_ARG;
    }
    
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: x_str_buf_cmp_string
 *
 * Description: This API compare string referenced by string ID with
 *    specified string.
 *
 * Inputs:  
 *    pt_strbuf      Ref to a string buffer.
 *
 *    t_string_id    Specify string ID to compare with 'ps_string'.
 *
 *    ps_string      Specify string to compare.
 *
 * Outputs:
 *    None.
 *
 * Returns:
 *    STRBUFR_OK          String match.
 *    STRBUFR_FAIL        String not match.
 *    STRBUFR_INV_STR_ID  String associated with string ID is not found.
 ----------------------------------------------------------------------------*/
INT32 x_str_buf_cmp_string
(
    STR_BUF_T*       pt_strbuf,
    STRING_ID_T      t_string_id,
    const CHAR*      ps_string
)
{
    return CONV_RET(x_mem_buf_cmp_object((MEM_BUF_T*) pt_strbuf,
                                         (OBJECT_ID_T) t_string_id,
                                         (VOID*) ps_string,
                                         x_strlen(ps_string)));
                                /* x_strlen returns 0 when ps_string == NULL */
}

/*-----------------------------------------------------------------------------
 * Name: x_str_buf_del_string
 *
 * Description: This API deletes a specified string from mini-buffer.
 *
 * Inputs:  
 *    pt_strbuf      Ref to a string buffer.
 *
 *    t_string_id    Specify string ID to delete from 'pt_strbuf'.
 *                   If this string ID is invalid (e.g., NULL, not found),
 *                   then this API return without side effect.
 *
 * Outputs:
 *    None.
 *
 * Returns:
 *    STBUFR_OK          Success.
 *  
 ----------------------------------------------------------------------------*/
INT32 x_str_buf_del_string
(
    STR_BUF_T*       pt_strbuf,
    STRING_ID_T      t_string_id
)
{
    return CONV_RET(x_mem_buf_del_object((MEM_BUF_T*) pt_strbuf,
                                         (OBJECT_ID_T) t_string_id));
}

/*-----------------------------------------------------------------------------
 * Name: x_str_buf_del_all_string
 *
 * Description: This API deletes all strings from mini-buffer.
 *
 * Inputs:  
 *    pt_strbuf      Ref to a string buffer.
 *
 * Outputs:
 *    None.
 *
 * Returns:
 *    STBUFR_OK          Success.
 *    
 ----------------------------------------------------------------------------*/
INT32 x_str_buf_del_all_string
(
    STR_BUF_T*       pt_strbuf
)
{
    return CONV_RET(x_mem_buf_del_all_objects((MEM_BUF_T*) pt_strbuf));
}
    
/*-----------------------------------------------------------------------------
 * Name: x_str_buf_free
 *
 * Description: This free the memory resource for the specified
 *    mini-buffer.
 *
 * Inputs:  
 *    pt_strbuf      Ref to a string buffer.
 *
 * Outputs:
 *    None.
 *
 * Returns:
 *    STBUFR_OK          Success.
 *    
 ----------------------------------------------------------------------------*/
INT32 x_str_buf_free
(
    STR_BUF_T*       pt_strbuf
)
{
    return CONV_RET(x_mem_buf_free((MEM_BUF_T*) pt_strbuf));
}

/*-----------------------------------------------------------------------------
 * Name: x_str_buf_persistent_sz
 *
 * Description: This API computes the space (bytes) needed to store
 *    (serialize) the mini-buffer.on filesystem.
 *
 * Inputs:  
 *    pt_strbuf      Ref to a string buffer.
 *
 * Outputs:
 *    VOID
 *
 * Returns:
 *    STBUFR_OK          Success.
 *    
 ----------------------------------------------------------------------------*/
INT32 x_str_buf_persistent_sz
(
    STR_BUF_T*       pt_strbuf
)
{
    return x_mem_buf_persistent_sz((MEM_BUF_T*) pt_strbuf);
}

/*-----------------------------------------------------------------------------
 * Name: x_str_buf_serialize
 *
 * Description: This API compacts the mini buffer data into a serial form
 *    (e.g., stream of bytes) which can be stored on filesystem.
 *
 *    It returns addresses vector and number of bytes for each
 *    address which need to be copied out to serialize the data.
 *
 * Inputs:  
 *    pt_strbuf      Ref to a string buffer.
 *
 * Outputs:
 *    aui1_addr_vec  array contains address of mini-buf data to serialize.
 *
 *    aui4_byte_to_copy array contains the nb of bytes to copy for the
 *                      address specified in 'aui1_addr_vec'.
 *
 * Returns:
 *    STBUFR_OK          Success.
 *    
 ----------------------------------------------------------------------------*/
INT32 x_str_buf_serialize
(
    STR_BUF_T*       pt_strbuf,
    UINT32           aui4_byte_to_copy[],
    UINT8*           aui1_addr_vec[]
)
{
    return CONV_RET(x_mem_buf_serialize((MEM_BUF_T*) pt_strbuf,
                                        aui4_byte_to_copy,
                                        aui1_addr_vec));
}

/*-----------------------------------------------------------------------------
 * Name: x_str_buf_create_from_byte_stream
 *
 * Description: This API re-creates the mini buffer data from serial data
 *    (e.g., stream of bytes) that was saved.
 *
 * Inputs:  
 *    pt_strbuf      Ref to a string buffer.
 *
 * Outputs:
 *    aui1_addr_vec  array contains address of mini-buf data to serialize.
 *                   The data should be a sequence of
 *
 *                   size of offset array (UINT32)
 *                   size of internal string buffer (UINT32)
 *                   array of bytes represent offset array
 *                   array of bytes represent the string buffer.
 *                   
 * Returns:
 *    Ref to the new created mini-buffer object. If error, return NULL.
 *    
 ----------------------------------------------------------------------------*/
STR_BUF_T* x_str_buf_create_from_byte_stream
(
    UINT32           aui4_byte_to_copy[],
    UINT8*           aui1_addr_vec[]
)
{
    return (STR_BUF_T*) x_mem_buf_create_from_byte_stream(aui4_byte_to_copy,
                                                          aui1_addr_vec);
}

/*-----------------------------------------------------------------------------
 * Name: x_str_buf_dup
 *
 * Description: This API make a copy of mini-string buffer
 *
 * Inputs:  
 *    pt_strbuf      Ref to a string buffer.
 *
 * Outputs:
 *    None
 *
 * Returns:
 *    Ref to a new mini-buffer.
 *    
 ----------------------------------------------------------------------------*/
STR_BUF_T*  x_str_buf_dup
(
    STR_BUF_T*       pt_strbuf
)
{
    return (STR_BUF_T*) x_mem_buf_dup((MEM_BUF_T*) pt_strbuf);
}

/*-----------------------------------------------------------------------------
 * Name:  x_str_buf_diagnostic
 *
 * Description:
 *   This function dumps the information about string buffer memory usage
 *   and the data.
 *
 * Inputs:
 *   pt_strbuf     Ref to STR_BUF_T structure.
 *
 * Outputs:
 *   None.
 *   
 * Returns:
 *   void. 
 *   
 ----------------------------------------------------------------------------*/
VOID x_str_buf_diagnostic(STR_BUF_T*       pt_strbuf)
{
#ifdef  DEBUG
    x_dbg_stmt("This is the string buffer wrapper for memory buffer\n");
#endif
    x_mem_buf_diagnostic((MEM_BUF_T*) pt_strbuf);
}

