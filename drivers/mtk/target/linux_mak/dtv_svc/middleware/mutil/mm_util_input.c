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
 * $RCSfile: mm_util_input.c,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/4 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 9ce950eb85bf90ea84aa0781ae2e804f $ Asa
 *
 * Description: 
 *         This file exports all input function API for media utility 
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#ifdef _WIN32_
#include "inc/x_os.h"
#else
#include "os/inc/x_os.h"
#endif

#include "inc/u_common.h"
#include "handle/u_handle.h"
#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif
#include "mm_util.h"
#include "mm_util_input.h"
#include "inc/x_mm_common.h"
#ifdef INET_SUPPORT
#include "inet/http/x_httpc_api.h"
#ifdef ENABLE_MMS_SUPPORT
#include "inet/mms/include/libmms/mms_streaming.h"
#endif
#endif
#include "dbg/x_dbg.h"
#include "dbg/u_dbg.h"
#else

#include "x_os.h"
#include "u_common.h"
#include "u_handle.h"
#include "mm_util.h"
#include "mm_util_input.h"
#include "x_mm_common.h"
#ifdef INET_SUPPORT
#include "x_httpc_api.h"
#endif
#include "u_dbg.h"
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
 * Name:  mm_util_fcopybytes_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_fcopybytes_fct(VOID*     pv_obj, 
                             VOID*     pv_buf, 
                             SIZE_T    z_buf_leng, 
                             SIZE_T    z_copy_size,
                             UINT32*   pui4_size)
{
#ifndef __NO_FM__
    UINT32              ui4_read;
    INT32               i4_ret;
    BOOL                b_buf_not_enough;
    SIZE_T              z_copy;
    MM_UTIL_INPUT_T*    pt_obj;
    
    if((pv_obj == NULL) || (pv_buf == NULL) || (pui4_size == NULL))
    {
        return INPUTR_FAIL;
    }

    pt_obj = (MM_UTIL_INPUT_T*)pv_obj;
    
    if (z_buf_leng < z_copy_size)
    {
        z_copy = z_buf_leng;
        b_buf_not_enough = TRUE;
    }
    else
    {
        z_copy = z_copy_size;
        b_buf_not_enough = FALSE;
    }
    
    i4_ret = x_fm_read(pt_obj->h_file, pv_buf, z_copy, &ui4_read);
    *pui4_size = ui4_read;
    if (i4_ret == FMR_OK)
    {
        if(ui4_read < z_copy)
        {
            return INPUTR_EOF;
        }
        else
        {
            if(b_buf_not_enough)
            {
                return INPUTR_NOT_ENOUGH_BUF;
            }
            else
            {
                return INPUTR_OK;
            }
        }
    }
    else if(i4_ret == FMR_EOF)
    {
        return INPUTR_EOF;
    }
    else
    {
        return INPUTR_FAIL;
    }
#else
    return INPUTR_FAIL;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_finput4bytes_fct
 *
 * Description: big endian for 4 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_finput4bytes_b_fct(VOID* pv_obj, UINT32* pui4_data)
{
#ifndef __NO_FM__
    UINT32              ui4_data;
    UINT32              ui4_read;
    INT32               i4_ret;
    MM_UTIL_INPUT_T*    pt_obj;
    
    if((pv_obj == NULL) || (pui4_data == NULL))
    {
        return INPUTR_FAIL;
    }

    pt_obj = (MM_UTIL_INPUT_T*)pv_obj;
    
    i4_ret = x_fm_read(pt_obj->h_file, (VOID*)&ui4_data, sizeof(ui4_data), &ui4_read);
    *pui4_data = ((ui4_data&0xff000000)>>24) 
                 | ((ui4_data&0xff0000)>>8) 
                 | ((ui4_data&0xff00)<<8) 
                 | ((ui4_data&0xff)<<24);
    if (i4_ret == FMR_OK)
    {
        return INPUTR_OK;
    }
    else if(i4_ret == FMR_EOF)
    {
        return INPUTR_EOF;
    }
    else
    {
        return INPUTR_FAIL;
    }
#else
    return INPUTR_FAIL;
#endif
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_finput3bytes_fct
 *
 * Description: big endian for 3 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_finput3bytes_b_fct(VOID* pv_obj, UINT32* pui4_data)
{
#ifndef __NO_FM__
    UINT32              ui4_data = 0;
    UINT32              ui4_read;
    INT32               i4_ret;
    MM_UTIL_INPUT_T*    pt_obj;

    if((pv_obj == NULL) || (pui4_data == NULL))
    {
        return INPUTR_FAIL;
    }
    
    pt_obj = (MM_UTIL_INPUT_T*)pv_obj;
    
    i4_ret = x_fm_read(pt_obj->h_file, (VOID*)&ui4_data, 3, &ui4_read);
    *pui4_data = ((ui4_data&0xff0000)>>16) 
                 | (ui4_data&0xff00) 
                 | ((ui4_data&0xff)<<16);
    if (i4_ret == FMR_OK)
    {
        return INPUTR_OK;
    }
    else if(i4_ret == FMR_EOF)
    {
        return INPUTR_EOF;
    }
    else
    {
        return INPUTR_FAIL;
    }
#else
    return INPUTR_FAIL;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_finput2bytes_b_fct
 *
 * Description: big endian for 2 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_finput2bytes_b_fct(VOID* pv_obj, UINT16* pui2_data)
{
#ifndef __NO_FM__
    UINT16              ui2_data;
    UINT32              ui4_read;
    INT32               i4_ret;
    MM_UTIL_INPUT_T*    pt_obj;

    if((pv_obj == NULL) || (pui2_data == NULL))
    {
        return INPUTR_FAIL;
    }
    
    pt_obj = (MM_UTIL_INPUT_T*)pv_obj;
    
    i4_ret = x_fm_read(pt_obj->h_file, (VOID*)&ui2_data, sizeof(ui2_data), &ui4_read);
    *pui2_data = ((ui2_data&0xff00)>>8) | ((ui2_data&0xff)<<8);
    if (i4_ret == FMR_OK)
    {
        return INPUTR_OK;
    }
    else if(i4_ret == FMR_EOF)
    {
        return INPUTR_EOF;
    }
    else
    {
        return INPUTR_FAIL;
    }
#else
    return INPUTR_FAIL;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_finput4bytes_l_fct
 *
 * Description: little endian for 4 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_finput4bytes_l_fct(VOID* pv_obj, UINT32* pui4_data)
{
#ifndef __NO_FM__
    UINT32              ui4_data;
    UINT32              ui4_read;
    INT32               i4_ret;
    MM_UTIL_INPUT_T*    pt_obj;
    
    if((pv_obj == NULL) || (pui4_data == NULL))
    {
        return INPUTR_FAIL;
    }
    pt_obj = (MM_UTIL_INPUT_T*)pv_obj;
    
    i4_ret = x_fm_read(pt_obj->h_file, (VOID*)&ui4_data, sizeof(ui4_data), &ui4_read);
    *pui4_data = ui4_data;
    if (i4_ret == FMR_OK)
    {
        return INPUTR_OK;
    }
    else if(i4_ret == FMR_EOF)
    {
        return INPUTR_EOF;
    }
    else
    {
        return INPUTR_FAIL;
    }
#else
    return INPUTR_FAIL;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_finput3bytes_l_fct
 *
 * Description: little endian for 3 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_finput3bytes_l_fct(VOID* pv_obj, UINT32* pui4_data)
{
#ifndef __NO_FM__
    UINT32              ui4_data = 0;
    UINT32              ui4_read;
    INT32               i4_ret;
    MM_UTIL_INPUT_T*    pt_obj;
    
    if((pv_obj == NULL) || (pui4_data == NULL))
    {
        return INPUTR_FAIL;
    }
    pt_obj = (MM_UTIL_INPUT_T*)pv_obj;
    
    i4_ret = x_fm_read(pt_obj->h_file, (VOID*)&ui4_data, 3, &ui4_read);
    *pui4_data = ui4_data;
    if (i4_ret == FMR_OK)
    {
        return INPUTR_OK;
    }
    else if(i4_ret == FMR_EOF)
    {
        return INPUTR_EOF;
    }
    else
    {
        return INPUTR_FAIL;
    }
#else
    return INPUTR_FAIL;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_finput2bytes_l_fct
 *
 * Description: little endian for 2 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_finput2bytes_l_fct(VOID* pv_obj, UINT16* pui2_data)
{
#ifndef __NO_FM__
    UINT16              ui2_data;
    UINT32              ui4_read;
    INT32               i4_ret;
    MM_UTIL_INPUT_T*    pt_obj;
    
    if((pv_obj == NULL) || (pui2_data == NULL))
    {
        return INPUTR_FAIL;
    }
    pt_obj = (MM_UTIL_INPUT_T*)pv_obj;
    
    i4_ret = x_fm_read(pt_obj->h_file, (VOID*)&ui2_data, sizeof(ui2_data), &ui4_read);
    *pui2_data = ui2_data;
    if (i4_ret == FMR_OK)
    {
        return INPUTR_OK;
    }
    else if(i4_ret == FMR_EOF)
    {
        return INPUTR_EOF;
    }
    else
    {
        return INPUTR_FAIL;
    }
#else
    return INPUTR_FAIL;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_finput1bytes_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_finput1bytes_fct(VOID* pv_obj, UINT8* pui1_data)
{
#ifndef __NO_FM__
    UINT8               ui1_data;
    UINT32              ui4_read;
    INT32               i4_ret;
    MM_UTIL_INPUT_T*    pt_obj;
    
    if((pv_obj == NULL) || (pui1_data == NULL))
    {
        return INPUTR_FAIL;
    }
    pt_obj = (MM_UTIL_INPUT_T*)pv_obj;
    
    i4_ret = x_fm_read(pt_obj->h_file, (VOID*)&ui1_data, sizeof(ui1_data), &ui4_read);
    *pui1_data = ui1_data;
    if (i4_ret == FMR_OK)
    {
        return INPUTR_OK;
    }
    else if(i4_ret == FMR_EOF)
    {
        return INPUTR_EOF;
    }
    else
    {
        return INPUTR_FAIL;
    }
#else
    return INPUTR_FAIL;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_fsetpos_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_fsetpos_fct(VOID* pv_obj, INT64 i8_offset, UINT8 ui1_whence)
{
#ifndef __NO_FM__
    INT32               i4_ret;
    UINT64              ui8_cur_pos;
    MM_UTIL_INPUT_T*    pt_obj;
    
    if(pv_obj == NULL)
    {
        return INPUTR_FAIL;
    }

    pt_obj = (MM_UTIL_INPUT_T*)pv_obj;
    
    i4_ret = x_fm_lseek(pt_obj->h_file, i8_offset, ui1_whence, &ui8_cur_pos);
    if(i4_ret == FMR_OK)
    {
        return INPUTR_OK;
    }
    else if(i4_ret == FMR_BUSY)
    {
        return INPUTR_BUSY;
    }
    else
    {
        return INPUTR_FAIL;
    }
#else
    return INPUTR_FAIL;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_fgetpos_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_fgetpos_fct(VOID* pv_obj, UINT64* pui8_cur_pos)
{
#ifndef __NO_FM__
    UINT64              ui8_cur_pos;
    INT32               i4_ret;
    MM_UTIL_INPUT_T*    pt_obj;
    
    if((pv_obj == NULL) || (pui8_cur_pos == NULL))
    {
        return INPUTR_FAIL;
    }
    pt_obj = (MM_UTIL_INPUT_T*)pv_obj;
    
    i4_ret = x_fm_lseek(pt_obj->h_file, 0, FM_SEEK_CUR, &ui8_cur_pos);
    *pui8_cur_pos = ui8_cur_pos;
    if (i4_ret == FMR_OK)
    {
        return INPUTR_OK;
    }
    else
    {
        return INPUTR_FAIL;
    }
#else
    return INPUTR_FAIL;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_mcopybytes_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_mcopybytes_fct(VOID*     pv_obj, 
                             VOID*     pv_buf, 
                             SIZE_T    z_buf_leng, 
                             SIZE_T    z_copy_size,
                             UINT32*   pui4_size)
{
    SIZE_T              z_copy;
    MM_UTIL_INPUT_T*    pt_obj;
    UCHAR*              puc_buff;
    BOOL                b_buf_not_enough;

    if((pv_obj == NULL) || (pv_buf == NULL) ||(pui4_size == NULL))
    {
        return INPUTR_FAIL;
    }
    
    pt_obj = (MM_UTIL_INPUT_T*)pv_obj;
    puc_buff = (UCHAR*)pt_obj->pv_buff_or_file_path_name;
    
    if (z_buf_leng < z_copy_size)
    {
        z_copy           = z_buf_leng;
        b_buf_not_enough = TRUE;
    }
    else
    {
        z_copy           = z_copy_size;
        b_buf_not_enough = FALSE;
    }
    
    if((UINT64)(pt_obj->i8_buff_pos + z_copy) > pt_obj->z_buff_leng)
    {
        z_copy = (SIZE_T)(pt_obj->z_buff_leng - pt_obj->i8_buff_pos);
    }
    x_memcpy(pv_buf, (VOID*)&puc_buff[pt_obj->i8_buff_pos], z_copy);
    pt_obj->i8_buff_pos = pt_obj->i8_buff_pos + z_copy;

    *pui4_size = z_copy;

    if(b_buf_not_enough)
    {
        return INPUTR_NOT_ENOUGH_BUF;
    }
    else
    {
        return INPUTR_OK;
    }
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_minput4bytes_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_minput4bytes_b_fct(VOID* pv_obj, UINT32* pui4_data)
{
    UINT32              ui4_ret = 0;
    MM_UTIL_INPUT_T*    pt_obj;
    UCHAR*              puc_buff;

    if((pv_obj == NULL) || (pui4_data == NULL))
    {
        return INPUTR_FAIL;
    }
    pt_obj = (MM_UTIL_INPUT_T*)pv_obj;
    puc_buff = (UCHAR*)pt_obj->pv_buff_or_file_path_name;

    if((UINT64)(pt_obj->i8_buff_pos + 4) > pt_obj->z_buff_leng)
    {
        return INPUTR_FAIL;
    }

    ui4_ret = (UINT32)(puc_buff[pt_obj->i8_buff_pos] << 24) 
              | (UINT32)(puc_buff[pt_obj->i8_buff_pos + 1] << 16) 
              | (UINT32)(puc_buff[pt_obj->i8_buff_pos + 2] << 8) 
              | (UINT32)(puc_buff[pt_obj->i8_buff_pos + 3]);
    *pui4_data = ui4_ret;
    pt_obj->i8_buff_pos = pt_obj->i8_buff_pos + 4;
    
    return INPUTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_minput3bytes_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_minput3bytes_b_fct(VOID* pv_obj, UINT32* pui4_data)
{
    UINT32              ui4_ret = 0;
    MM_UTIL_INPUT_T*    pt_obj;
    UCHAR*              puc_buff;
    
    if((pv_obj == NULL) || (pui4_data == NULL))
    {
        return INPUTR_FAIL;
    }

    pt_obj = (MM_UTIL_INPUT_T*)pv_obj;
    puc_buff = (UCHAR*)pt_obj->pv_buff_or_file_path_name;

    if((UINT64)(pt_obj->i8_buff_pos + 3) > pt_obj->z_buff_leng)
    {
        return INPUTR_FAIL;
    }

    ui4_ret = (UINT32)(puc_buff[pt_obj->i8_buff_pos] << 16) 
              | (UINT32)(puc_buff[pt_obj->i8_buff_pos + 1] << 8) 
              | (UINT32)(puc_buff[pt_obj->i8_buff_pos + 2]);
    *pui4_data = ui4_ret;
    pt_obj->i8_buff_pos = pt_obj->i8_buff_pos + 3;
    
    return INPUTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_minput2bytes_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_minput2bytes_b_fct(VOID* pv_obj, UINT16* pui2_data)
{
    UINT16              ui2_ret = 0;
    MM_UTIL_INPUT_T*    pt_obj;
    UCHAR*              puc_buff;
    
    if((pv_obj == NULL) || (pui2_data == NULL))
    {
        return INPUTR_FAIL;
    }

    pt_obj = (MM_UTIL_INPUT_T*)pv_obj;
    puc_buff = (UCHAR*)pt_obj->pv_buff_or_file_path_name;

    if((UINT64)(pt_obj->i8_buff_pos + 2) > pt_obj->z_buff_leng)
    {
        return INPUTR_FAIL;
    }

    ui2_ret = (UINT16)(puc_buff[pt_obj->i8_buff_pos] << 8) 
              | (UINT16)(puc_buff[pt_obj->i8_buff_pos + 1]);
    *pui2_data = ui2_ret;
    pt_obj->i8_buff_pos = pt_obj->i8_buff_pos + 2;
    
    return INPUTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_minput4bytes_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_minput4bytes_l_fct(VOID* pv_obj, UINT32* pui4_data)
{
    UINT32              ui4_ret = 0;
    MM_UTIL_INPUT_T*    pt_obj;
    UCHAR*              puc_buff;
    
    if((pv_obj == NULL) || (pui4_data == NULL))
    {
        return INPUTR_FAIL;
    }

    pt_obj = (MM_UTIL_INPUT_T*)pv_obj;
    puc_buff = (UCHAR*)pt_obj->pv_buff_or_file_path_name;
        
    if((UINT64)(pt_obj->i8_buff_pos + 4) > pt_obj->z_buff_leng)
    {
        return INPUTR_FAIL;
    }
    
    ui4_ret = (UINT32)(puc_buff[pt_obj->i8_buff_pos]) 
              | (UINT32)(puc_buff[pt_obj->i8_buff_pos + 1] << 8) 
              | (UINT32)(puc_buff[pt_obj->i8_buff_pos + 2] << 16) 
              | (UINT32)(puc_buff[pt_obj->i8_buff_pos + 3] << 24);
    *pui4_data = ui4_ret;
    pt_obj->i8_buff_pos = pt_obj->i8_buff_pos + 4;
    
    return INPUTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_minput3bytes_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_minput3bytes_l_fct(VOID* pv_obj, UINT32* pui4_data)
{
    UINT32              ui4_ret = 0;
    MM_UTIL_INPUT_T*    pt_obj;
    UCHAR*              puc_buff;
    
    if((pv_obj == NULL) || (pui4_data == NULL))
    {
        return INPUTR_FAIL;
    }


    pt_obj = (MM_UTIL_INPUT_T*)pv_obj;
    puc_buff = (UCHAR*)pt_obj->pv_buff_or_file_path_name;

    if((UINT64)(pt_obj->i8_buff_pos + 3) > pt_obj->z_buff_leng)
    {
        return INPUTR_FAIL;
    }

    ui4_ret = (UINT32)(puc_buff[pt_obj->i8_buff_pos]) 
              | (UINT32)(puc_buff[pt_obj->i8_buff_pos + 1] << 8) 
              | (UINT32)(puc_buff[pt_obj->i8_buff_pos + 2] << 16);
    *pui4_data = ui4_ret;
    pt_obj->i8_buff_pos = pt_obj->i8_buff_pos + 3;
    
    return INPUTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_minput2bytes_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_minput2bytes_l_fct(VOID* pv_obj, UINT16* pui2_data)
{
    UINT16              ui2_ret = 0;
    MM_UTIL_INPUT_T*    pt_obj;
    UCHAR*              puc_buff;
    
    if((pv_obj == NULL) || (pui2_data == NULL))
    {
        return INPUTR_FAIL;
    }

    pt_obj = (MM_UTIL_INPUT_T*)pv_obj;
    puc_buff = (UCHAR*)pt_obj->pv_buff_or_file_path_name;

    if((UINT64)(pt_obj->i8_buff_pos + 2) > pt_obj->z_buff_leng)
    {
        return INPUTR_FAIL;
    }

    ui2_ret = (UINT16)(puc_buff[pt_obj->i8_buff_pos]) 
              | (UINT16)(puc_buff[pt_obj->i8_buff_pos + 1] << 8);
    *pui2_data = ui2_ret;
    pt_obj->i8_buff_pos = pt_obj->i8_buff_pos + 2;
    
    return INPUTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_minput1bytes_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32  mm_util_minput1bytes_fct(VOID* pv_obj, UINT8* pui1_data)
{
    UINT8               ui1_ret;
    MM_UTIL_INPUT_T*    pt_obj;
    UCHAR*              puc_buff;
    
    if((pv_obj == NULL) || (pui1_data == NULL))
    {
        return INPUTR_FAIL;
    }

    pt_obj = (MM_UTIL_INPUT_T*)pv_obj;
    puc_buff = (UCHAR*)pt_obj->pv_buff_or_file_path_name;

    if((UINT64)(pt_obj->i8_buff_pos + 1) > pt_obj->z_buff_leng)
    {
        return INPUTR_FAIL;
    }

    ui1_ret = puc_buff[pt_obj->i8_buff_pos];
    *pui1_data = ui1_ret;
    pt_obj->i8_buff_pos = pt_obj->i8_buff_pos + 1;
    return INPUTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_msetpos_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_msetpos_fct(VOID* pv_obj, INT64 i8_offset, UINT8 ui1_whence)
{
    MM_UTIL_INPUT_T*    pt_obj;
    
    if(pv_obj == NULL)
    {
        return INPUTR_FAIL;
    }

    pt_obj = (MM_UTIL_INPUT_T*)pv_obj;
    
    switch (ui1_whence & (~FM_SEEK_CHK))
    {
        case FM_SEEK_BGN:
            pt_obj->i8_buff_pos = i8_offset;
            break;
            
        case FM_SEEK_CUR: 
            pt_obj->i8_buff_pos = pt_obj->i8_buff_pos + i8_offset;
            break;
            
        case FM_SEEK_END:
            pt_obj->i8_buff_pos = (INT64)pt_obj->z_buff_leng + i8_offset;
            break;
            
    }
    
    if (pt_obj->i8_buff_pos < 0)
    {
        pt_obj->i8_buff_pos = 0;
        return INPUTR_FAIL;
    }
    else if (pt_obj->i8_buff_pos > (INT64)pt_obj->z_buff_leng)
    {
        pt_obj->i8_buff_pos = (INT64)pt_obj->z_buff_leng;   
        return INPUTR_FAIL;
    }
    
    return INPUTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_mgetpos_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32  mm_util_mgetpos_fct(VOID* pv_obj, UINT64* pui8_cur_pos)
{
    MM_UTIL_INPUT_T*    pt_obj;

    if((pv_obj == NULL) || (pui8_cur_pos == NULL))
    {
        return INPUTR_FAIL;
    }
    
    pt_obj = (MM_UTIL_INPUT_T*)pv_obj;
    
    *pui8_cur_pos = (UINT64)pt_obj->i8_buff_pos;

    return INPUTR_OK;
}

#ifdef INET_SUPPORT
static MEDIA_MULTI_CACHE_T* mm_util_get_cache(MEDIA_FEEDER_NW_T* pt_feeder, 
                                              UINT64 ui8_addr, 
                                              UINT32 ui4_size)
{
    MEDIA_MULTI_CACHE_T *pt_cache;
    MEDIA_MULTI_CACHE_T *pt_pre_cache;
    
    /*find hit cache*/
    pt_cache = pt_feeder->pt_used_cache;
    pt_pre_cache = NULL;
    while (pt_cache)
    {
        if ((ui8_addr >= pt_cache->ui8_cache_pos) && 
            (ui8_addr < (pt_cache->ui8_cache_pos + pt_cache->ui4_read_count)))
        {
            if (pt_pre_cache == NULL)
            {
                /*pt_cache is pt_used_cache so do nothing*/
            }
            else
            {
                /*move pt_cache to head*/
                pt_pre_cache->pt_next = pt_cache->pt_next;
                pt_cache->pt_next = pt_feeder->pt_used_cache;
                pt_feeder->pt_used_cache = pt_cache;
            }
            
            return pt_cache;
        }
        
        pt_pre_cache = pt_cache;
        pt_cache = pt_cache->pt_next;
    }
    
    /*find free cache*/
    pt_cache = pt_feeder->pt_free_cache; 
    if (pt_cache)
    {
        /*remove from pt_free_cache and add as head of pt_used_cache*/
        pt_feeder->pt_free_cache = pt_cache->pt_next;
        pt_cache->pt_next = pt_feeder->pt_used_cache;
        pt_feeder->pt_used_cache = pt_cache;
        return pt_cache;
    }
    
    /*find replace cache*/
    pt_cache = pt_feeder->pt_used_cache;
    pt_pre_cache = NULL;
    while (pt_cache)
    {
        if (NULL == pt_cache->pt_next)
        {
            if (pt_pre_cache == NULL)
            {
                /*pt_cache is pt_used_cache so do nothing*/
            }
            else
            {
                /*move pt_cache to head*/
                pt_pre_cache->pt_next = pt_cache->pt_next;
                pt_cache->pt_next = pt_feeder->pt_used_cache;
                pt_feeder->pt_used_cache = pt_cache;
            }
            return pt_cache;
        }
        pt_pre_cache = pt_cache;
        pt_cache = pt_cache->pt_next;
    }
    
    return NULL;
}

#ifdef ENABLE_MMS_SUPPORT
/*-----------------------------------------------------------------------------
 * Name:  mm_util_mms_copybytes_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_mms_uri_copybytes_fct(VOID*     pv_obj, 
                             VOID*     pv_buf, 
                             SIZE_T    z_buf_leng, 
                             SIZE_T    z_copy_size,
                             UINT32*   pui4_size)
{
#ifndef __NO_FM__
    UINT32                  ui4_read;
    INT32                   i4_ret;
    SIZE_T                  z_copy;
    MEDIA_FEEDER_NW_T*      pt_obj;
    UINT32                  ui4_size;
    UINT32                  ui4_addr;
    UINT32                  ui4_cpy_size;
    UINT32                  ui4_mem_adr;
    UINT32                  ui4_cache_addr;
    UINT64                  ui8_pos;
    MM_UTIL_INPUT_T*        pt_input;
    MEDIA_MULTI_CACHE_T     *pt_cache;

    if((pv_obj == NULL) || (pv_buf == NULL) || (pui4_size == NULL))
    {
        return INPUTR_FAIL;
    }

    pt_input = (MM_UTIL_INPUT_T*)pv_obj;
    pt_obj = (MEDIA_FEEDER_NW_T*)pt_input->h_src_feeder;
    
    if (z_buf_leng < z_copy_size)
    {
        z_copy = z_buf_leng;
    }
    else
    {
        z_copy = z_copy_size;
    }

    ui4_size = z_copy;
    ui4_mem_adr = (UINT32)pv_buf;
    ui4_addr = (UINT32)(pt_obj->ui8_pos);
    ui8_pos = pt_obj->ui8_pos;
    
    i4_ret = INPUTR_OK;
    while (1)
    {   
        if (ui4_size == 0)
        {
            break;
        }
        
        if(pt_obj->b_canceled)
        {
#ifndef __KERNEL__
            x_dbg_stmt("!!!mm_util_mms_uri_copybytes_fct: cancel reading due to cancel flag \n");
#endif

            return INPUTR_FAIL;
        }
        
        pt_cache = mm_util_get_cache(pt_obj, ui4_addr, ui4_size);
        if (NULL == pt_cache)
        {
            while (1)
            {
#ifndef __KERNEL__
                x_dbg_stmt("!!!mm_util_mms_uri_copybytes_fct: No Cache \n");
#endif
            }
        }

        ui4_cache_addr = (UINT32)(pt_cache->ui8_cache_pos);
    
        if ((0 == pt_cache->ui4_read_count) ||
            (ui4_addr < ui4_cache_addr) || 
            (ui4_addr >= (ui4_cache_addr + pt_cache->ui4_read_count)))
        {
            /* update Cache */
            x_mms_fm_pull_byteseek(pt_obj->h_feeder, NULL, (INT64)ui4_addr, FM_SEEK_BGN, &ui8_pos);
            ui4_cache_addr = ui4_addr;
            i4_ret = x_mms_fm_pull_read(pt_obj->h_feeder, NULL, (CHAR*)pt_cache->aui1_cache_buf, NW_FEEDER_CACHE_SIZE, &ui4_read);
            if ((i4_ret!=FMR_OK) && (i4_ret!=FMR_EOF))
            {
                i4_ret = INPUTR_FAIL;
                break;
            }
            pt_cache->ui8_cache_pos = ui4_cache_addr;
            pt_cache->ui4_read_count = ui4_read;
        }

        if ((ui4_addr + ui4_size) < (ui4_cache_addr + pt_cache->ui4_read_count))
        {
            ui4_cpy_size = ui4_size;
        }
        else
        {
            ui4_cpy_size = (ui4_cache_addr + pt_cache->ui4_read_count) - ui4_addr;
        }

        if ((ui4_mem_adr+ui4_cpy_size)>((UINT32)pv_buf+(UINT32)z_buf_leng))
        {
            i4_ret = INPUTR_FAIL;
            break;
        }

        x_memcpy((void *) ui4_mem_adr,  
            (void *) (pt_cache->aui1_cache_buf + (ui4_addr - ui4_cache_addr)), 
            ui4_cpy_size);  

        ui4_addr += ui4_cpy_size;
        ui4_mem_adr += ui4_cpy_size;   

        if ((ui4_size <= ui4_cpy_size) || (pt_cache->ui4_read_count<NW_FEEDER_CACHE_SIZE))
        {
            break;
        }
        ui4_size -= ui4_cpy_size;
    }

    *pui4_size = (UINT32)(ui4_addr-pt_obj->ui8_pos);
    pt_obj->ui8_pos = ui4_addr;

    return i4_ret;
#else
    return INPUTR_FAIL;
#endif   
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_mms_uri_input4bytes_fct
 *
 * Description: big endian for 4 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_mms_uri_input4bytes_b_fct(VOID* pv_obj, UINT32* pui4_data)
{
#ifndef __NO_FM__
    UINT32              ui4_data;
    UINT32              ui4_read;
    INT32               i4_ret;
    
    if((pv_obj == NULL) || (pui4_data == NULL))
    {
        return INPUTR_FAIL;
    }

    i4_ret = mm_util_mms_uri_copybytes_fct(pv_obj, 
                                           (VOID*)&ui4_data, 
                                           4, 
                                           4,
                                           &ui4_read);
    *pui4_data = ((ui4_data&0xff000000)>>24) 
                 | ((ui4_data&0xff0000)>>8) 
                 | ((ui4_data&0xff00)<<8) 
                 | ((ui4_data&0xff)<<24);
    
    return i4_ret;
#else
    return INPUTR_FAIL;
#endif
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_mms_uri_input3bytes_fct
 *
 * Description: big endian for 3 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_mms_uri_input3bytes_b_fct(VOID* pv_obj, UINT32* pui4_data)
{
#ifndef __NO_FM__
    UINT32              ui4_data = 0;
    UINT32              ui4_read;
    INT32               i4_ret;

    if((pv_obj == NULL) || (pui4_data == NULL))
    {
        return INPUTR_FAIL;
    }
    
    i4_ret = mm_util_mms_uri_copybytes_fct(pv_obj, 
                                           (VOID*)&ui4_data, 
                                           3, 
                                           3,
                                           &ui4_read);
    *pui4_data = ((ui4_data&0xff0000)>>16) 
                 | (ui4_data&0xff00) 
                 | ((ui4_data&0xff)<<16);

    return i4_ret;

#else
    return INPUTR_FAIL;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_mms_uri_input2bytes_b_fct
 *
 * Description: big endian for 2 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_mms_uri_input2bytes_b_fct(VOID* pv_obj, UINT16* pui2_data)
{
#ifndef __NO_FM__
    UINT16              ui2_data;
    UINT32              ui4_read;
    INT32               i4_ret;

    if((pv_obj == NULL) || (pui2_data == NULL))
    {
        return INPUTR_FAIL;
    }
    
    i4_ret = mm_util_mms_uri_copybytes_fct(pv_obj, 
                                           (VOID*)&ui2_data, 
                                           2, 
                                           2,
                                           &ui4_read);
    *pui2_data = ((ui2_data&0xff00)>>8) | ((ui2_data&0xff)<<8);
    return i4_ret;
    
#else
    return INPUTR_FAIL;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_mms_uri_input4bytes_l_fct
 *
 * Description: little endian for 4 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_mms_uri_input4bytes_l_fct(VOID* pv_obj, UINT32* pui4_data)
{
#ifndef __NO_FM__
    UINT32              ui4_data;
    UINT32              ui4_read;
    INT32               i4_ret;
    
    if((pv_obj == NULL) || (pui4_data == NULL))
    {
        return INPUTR_FAIL;
    }
    
    i4_ret = mm_util_mms_uri_copybytes_fct(pv_obj, 
                                           (VOID*)&ui4_data, 
                                           4, 
                                           4,
                                           &ui4_read);
    *pui4_data = ui4_data;
    return i4_ret;
#else
    return INPUTR_FAIL;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_mms_uri_input3bytes_l_fct
 *
 * Description: little endian for 3 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_mms_uri_input3bytes_l_fct(VOID* pv_obj, UINT32* pui4_data)
{
#ifndef __NO_FM__
    UINT32              ui4_read;
    INT32               i4_ret;
    
    if((pv_obj == NULL) || (pui4_data == NULL))
    {
        return INPUTR_FAIL;
    }
    
    i4_ret = mm_util_mms_uri_copybytes_fct(pv_obj, 
                                           pui4_data, 
                                           3, 
                                           3,
                                           &ui4_read);

    return i4_ret;
#else
    return INPUTR_FAIL;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_mms_uri_input2bytes_l_fct
 *
 * Description: little endian for 2 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_mms_uri_input2bytes_l_fct(VOID* pv_obj, UINT16* pui2_data)
{
#ifndef __NO_FM__
    UINT32              ui4_read;
    INT32               i4_ret;
    
    if((pv_obj == NULL) || (pui2_data == NULL))
    {
        return INPUTR_FAIL;
    }

    i4_ret = mm_util_mms_uri_copybytes_fct(pv_obj, 
                                           pui2_data, 
                                           2, 
                                           2,
                                           &ui4_read);

    return i4_ret;

#else
    return INPUTR_FAIL;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_mms_uri_input1bytes_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_mms_uri_input1bytes_fct(VOID* pv_obj, UINT8* pui1_data)
{
#ifndef __NO_FM__
    UINT32              ui4_read;
    INT32               i4_ret;
    
    if((pv_obj == NULL) || (pui1_data == NULL))
    {
        return INPUTR_FAIL;
    }
 
    i4_ret = mm_util_mms_uri_copybytes_fct(pv_obj, 
                                           pui1_data, 
                                           1, 
                                           1,
                                           &ui4_read);

    return i4_ret;
#else
    return INPUTR_FAIL;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_mms_uri_setpos_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_mms_uri_setpos_fct(VOID* pv_obj, INT64 i8_offset, UINT8 ui1_whence)
{
#ifndef __NO_FM__
    MEDIA_FEEDER_NW_T*      pt_obj;
    INT64                   i8_pos;
    MM_UTIL_INPUT_T*        pt_input;
    if(pv_obj == NULL)
    {
        return INPUTR_FAIL;
    }

    pt_input = (MM_UTIL_INPUT_T*)pv_obj;
    pt_obj = (MEDIA_FEEDER_NW_T*)pt_input->h_src_feeder;
    
    switch (ui1_whence & (~FM_SEEK_CHK))
    {
        case MEDIA_SEEK_BGN:
            i8_pos = i8_offset;
            break;
            
        case MEDIA_SEEK_CUR: 
            i8_pos = (INT64)(pt_obj->ui8_pos);
            i8_pos = i8_pos + i8_offset;
            break;
            
        case MEDIA_SEEK_END:
            i8_pos = (INT64)(pt_obj->ui8_file_size);
            i8_pos = i8_pos + i8_offset;
            break;
            
        default:
            return INPUTR_FAIL;
    }

    /*Not Return Fail Just For The Case set end when FileSize=-1 OffSet=0*/
    if (i8_pos < 0 && (INT64)pt_obj->ui8_file_size != -1)
    {
        return INPUTR_FAIL;
    }

    if (i8_pos > (INT64)pt_obj->ui8_file_size)
    {
        return INPUTR_EOF;
    }

    pt_obj->ui8_pos = (UINT64)i8_pos;

    return INPUTR_OK;
#else
    return INPUTR_FAIL;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_mms_uri_getpos_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_mms_uri_getpos_fct(VOID* pv_obj, UINT64* pui8_cur_pos)
{
#ifndef __NO_FM__
    MEDIA_FEEDER_NW_T*      pt_obj;
    MM_UTIL_INPUT_T*        pt_input;

    
    if((pv_obj == NULL) || (pui8_cur_pos == NULL))
    {
        return INPUTR_FAIL;
    }
    pt_input = (MM_UTIL_INPUT_T*)pv_obj;
    pt_obj = (MEDIA_FEEDER_NW_T*)pt_input->h_src_feeder;

    *pui8_cur_pos = pt_obj->ui8_pos;

        return INPUTR_OK;

#else
    return INPUTR_FAIL;
#endif    
}
#endif

/*-----------------------------------------------------------------------------
 * Name:  mm_util_network_copybytes_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_network_uri_copybytes_fct(VOID*     pv_obj, 
                             VOID*     pv_buf, 
                             SIZE_T    z_buf_leng, 
                             SIZE_T    z_copy_size,
                             UINT32*   pui4_size)
{
#ifndef __NO_FM__
    UINT32                  ui4_read;
    INT32                   i4_ret;
    SIZE_T                  z_copy;
    MEDIA_FEEDER_NW_T*      pt_obj;
    UINT32                  ui4_size;
    UINT64                  ui8_addr;
    UINT32                  ui4_cpy_size;
    UINT32                  ui4_mem_adr;
    UINT64                  ui8_cache_addr;
    UINT64                  ui8_pos;
    MM_UTIL_INPUT_T*        pt_input;
    MEDIA_MULTI_CACHE_T     *pt_cache;

    if((pv_obj == NULL) || (pv_buf == NULL) || (pui4_size == NULL))
    {
        return INPUTR_FAIL;
    }

    pt_input = (MM_UTIL_INPUT_T*)pv_obj;
    pt_obj = (MEDIA_FEEDER_NW_T*)pt_input->h_src_feeder;
    
    if (z_buf_leng < z_copy_size)
    {
        z_copy = z_buf_leng;
    }
    else
    {
        z_copy = z_copy_size;
    }

    ui4_size = z_copy;
    ui4_mem_adr = (UINT32)pv_buf;
    ui8_addr = (pt_obj->ui8_pos);
    ui8_pos = pt_obj->ui8_pos;
    
    i4_ret = INPUTR_OK;
    while (1)
    {   
        if (ui4_size == 0)
        {
            break;
        }
        
        if(pt_obj->b_canceled)
        {
#ifndef __KERNEL__
            x_dbg_stmt("!!!mm_util_network_uri_copybytes_fct: cancel reading due to cancel flag \n");
#endif
            return INPUTR_FAIL;
        }
        
        pt_cache = mm_util_get_cache(pt_obj, ui8_addr, ui4_size);
        if (NULL == pt_cache)
        {
            while (1)
            {
#ifndef __KERNEL__
                x_dbg_stmt("!!!mm_util_network_uri_copybytes_fct: No Cache \n");
#endif
            }
        }

        ui8_cache_addr = (pt_cache->ui8_cache_pos);
    
        if ((0 == pt_cache->ui4_read_count) ||
            (ui8_addr < ui8_cache_addr) || 
            (ui8_addr >= (ui8_cache_addr + pt_cache->ui4_read_count)))
        {
            /* update Cache */
            i4_ret = x_httpc_fm_lseek(pt_obj->h_feeder, (INT64)ui8_addr, FM_SEEK_BGN, &ui8_pos);
            if (i4_ret!=FMR_OK)
            {
#ifndef __KERNEL__
                x_dbg_stmt("!!!x_httpc_fm_lseek failed, ret = %d \n", i4_ret);
#endif
                i4_ret = INPUTR_FAIL;
                break;
            }
            
            ui8_cache_addr = ui8_addr;
            i4_ret = x_httpc_fm_read(pt_obj->h_feeder, pt_cache->aui1_cache_buf, NW_FEEDER_CACHE_SIZE, &ui4_read);
            if ((i4_ret!=FMR_OK) && (i4_ret!=FMR_EOF))
            {
#ifndef __KERNEL__
                x_dbg_stmt("!!!x_httpc_fm_read failed, ret = %d \n", i4_ret);
#endif
                i4_ret = INPUTR_FAIL;
                break;
            }
            else
            {
#ifndef __KERNEL__
                x_dbg_stmt("!!!x_httpc_fm_read ok\n");
#endif
            }
            pt_cache->ui8_cache_pos = ui8_cache_addr;
            pt_cache->ui4_read_count = ui4_read;
        }

        if ((ui8_addr + ui4_size) < (ui8_cache_addr + pt_cache->ui4_read_count))
        {
            ui4_cpy_size = ui4_size;
        }
        else
        {
            ui4_cpy_size = (UINT32)((ui8_cache_addr + pt_cache->ui4_read_count) - ui8_addr);
        }

        if ((ui4_mem_adr+ui4_cpy_size)>((UINT32)pv_buf+(UINT32)z_buf_leng))
        {
            i4_ret = INPUTR_FAIL;
            break;
        }

        x_memcpy((void *) ui4_mem_adr,  
            (void *) (pt_cache->aui1_cache_buf + (ui8_addr - ui8_cache_addr)), 
            ui4_cpy_size);  

        ui8_addr += ui4_cpy_size;
        ui4_mem_adr += ui4_cpy_size;   

        if ((ui4_size <= ui4_cpy_size) || (pt_cache->ui4_read_count<NW_FEEDER_CACHE_SIZE))
        {
            break;
        }
        ui4_size -= ui4_cpy_size;
    }

    *pui4_size = (UINT32)(ui8_addr-pt_obj->ui8_pos);
    pt_obj->ui8_pos = ui8_addr;

    return i4_ret;
#else
    return INPUTR_FAIL;
#endif   
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_network_uri_input4bytes_fct
 *
 * Description: big endian for 4 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_network_uri_input4bytes_b_fct(VOID* pv_obj, UINT32* pui4_data)
{
#ifndef __NO_FM__
    UINT32              ui4_data;
    UINT32              ui4_read;
    INT32               i4_ret;
    
    if((pv_obj == NULL) || (pui4_data == NULL))
    {
        return INPUTR_FAIL;
    }

    i4_ret = mm_util_network_uri_copybytes_fct(pv_obj, 
                                           (VOID*)&ui4_data, 
                                           4, 
                                           4,
                                           &ui4_read);
    *pui4_data = ((ui4_data&0xff000000)>>24) 
                 | ((ui4_data&0xff0000)>>8) 
                 | ((ui4_data&0xff00)<<8) 
                 | ((ui4_data&0xff)<<24);
    
    return i4_ret;
#else
    return INPUTR_FAIL;
#endif
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_network_uri_input3bytes_fct
 *
 * Description: big endian for 3 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_network_uri_input3bytes_b_fct(VOID* pv_obj, UINT32* pui4_data)
{
#ifndef __NO_FM__
    UINT32              ui4_data = 0;
    UINT32              ui4_read;
    INT32               i4_ret;

    if((pv_obj == NULL) || (pui4_data == NULL))
    {
        return INPUTR_FAIL;
    }
    
    i4_ret = mm_util_network_uri_copybytes_fct(pv_obj, 
                                           (VOID*)&ui4_data, 
                                           3, 
                                           3,
                                           &ui4_read);
    *pui4_data = ((ui4_data&0xff0000)>>16) 
                 | (ui4_data&0xff00) 
                 | ((ui4_data&0xff)<<16);

    return i4_ret;

#else
    return INPUTR_FAIL;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_network_uri_input2bytes_b_fct
 *
 * Description: big endian for 2 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_network_uri_input2bytes_b_fct(VOID* pv_obj, UINT16* pui2_data)
{
#ifndef __NO_FM__
    UINT16              ui2_data;
    UINT32              ui4_read;
    INT32               i4_ret;

    if((pv_obj == NULL) || (pui2_data == NULL))
    {
        return INPUTR_FAIL;
    }
    
    i4_ret = mm_util_network_uri_copybytes_fct(pv_obj, 
                                           (VOID*)&ui2_data, 
                                           2, 
                                           2,
                                           &ui4_read);
    *pui2_data = ((ui2_data&0xff00)>>8) | ((ui2_data&0xff)<<8);
    return i4_ret;
    
#else
    return INPUTR_FAIL;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_network_uri_input4bytes_l_fct
 *
 * Description: little endian for 4 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_network_uri_input4bytes_l_fct(VOID* pv_obj, UINT32* pui4_data)
{
#ifndef __NO_FM__
    UINT32              ui4_data;
    UINT32              ui4_read;
    INT32               i4_ret;
    
    if((pv_obj == NULL) || (pui4_data == NULL))
    {
        return INPUTR_FAIL;
    }
    
    i4_ret = mm_util_network_uri_copybytes_fct(pv_obj, 
                                           (VOID*)&ui4_data, 
                                           4, 
                                           4,
                                           &ui4_read);
    *pui4_data = ui4_data;
    return i4_ret;
#else
    return INPUTR_FAIL;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_network_uri_input3bytes_l_fct
 *
 * Description: little endian for 3 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_network_uri_input3bytes_l_fct(VOID* pv_obj, UINT32* pui4_data)
{
#ifndef __NO_FM__
    UINT32              ui4_read;
    INT32               i4_ret;
    
    if((pv_obj == NULL) || (pui4_data == NULL))
    {
        return INPUTR_FAIL;
    }
    
    i4_ret = mm_util_network_uri_copybytes_fct(pv_obj, 
                                           pui4_data, 
                                           3, 
                                           3,
                                           &ui4_read);

    return i4_ret;
#else
    return INPUTR_FAIL;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_network_uri_input2bytes_l_fct
 *
 * Description: little endian for 2 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_network_uri_input2bytes_l_fct(VOID* pv_obj, UINT16* pui2_data)
{
#ifndef __NO_FM__
    UINT32              ui4_read;
    INT32               i4_ret;
    
    if((pv_obj == NULL) || (pui2_data == NULL))
    {
        return INPUTR_FAIL;
    }

    i4_ret = mm_util_network_uri_copybytes_fct(pv_obj, 
                                           pui2_data, 
                                           2, 
                                           2,
                                           &ui4_read);

    return i4_ret;

#else
    return INPUTR_FAIL;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_network_uri_input1bytes_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_network_uri_input1bytes_fct(VOID* pv_obj, UINT8* pui1_data)
{
#ifndef __NO_FM__
    UINT32              ui4_read;
    INT32               i4_ret;
    
    if((pv_obj == NULL) || (pui1_data == NULL))
    {
        return INPUTR_FAIL;
    }
 
    i4_ret = mm_util_network_uri_copybytes_fct(pv_obj, 
                                           pui1_data, 
                                           1, 
                                           1,
                                           &ui4_read);

    return i4_ret;
#else
    return INPUTR_FAIL;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_network_uri_setpos_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_network_uri_setpos_fct(VOID* pv_obj, INT64 i8_offset, UINT8 ui1_whence)
{
#ifndef __NO_FM__
    MEDIA_FEEDER_NW_T*      pt_obj;
    INT64                   i8_pos;
    MM_UTIL_INPUT_T*        pt_input;
    if(pv_obj == NULL)
    {
        return INPUTR_FAIL;
    }

    pt_input = (MM_UTIL_INPUT_T*)pv_obj;
    pt_obj = (MEDIA_FEEDER_NW_T*)pt_input->h_src_feeder;
    
    switch (ui1_whence & (~FM_SEEK_CHK))
    {
        case MEDIA_SEEK_BGN:
            i8_pos = i8_offset;
            break;
            
        case MEDIA_SEEK_CUR: 
            i8_pos = (INT64)(pt_obj->ui8_pos);
            i8_pos = i8_pos + i8_offset;
            break;
            
        case MEDIA_SEEK_END:
            i8_pos = (INT64)(pt_obj->ui8_file_size);
            i8_pos = i8_pos + i8_offset;
            break;
            
        default:
            return INPUTR_FAIL;
    }

    /*Not Return Fail Just For The Case set end when FileSize=-1 OffSet=0*/
    if (i8_pos < 0 && (INT64)pt_obj->ui8_file_size != -1)
    {
        return INPUTR_FAIL;
    }

    if ((UINT64)i8_pos > pt_obj->ui8_file_size)
    {
        return INPUTR_EOF;
    }

    pt_obj->ui8_pos = (UINT64)i8_pos;

    return INPUTR_OK;
#else
    return INPUTR_FAIL;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_network_uri_getpos_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_network_uri_getpos_fct(VOID* pv_obj, UINT64* pui8_cur_pos)
{
#ifndef __NO_FM__
    MEDIA_FEEDER_NW_T*      pt_obj;
    MM_UTIL_INPUT_T*        pt_input;

    
    if((pv_obj == NULL) || (pui8_cur_pos == NULL))
    {
        return INPUTR_FAIL;
    }
    pt_input = (MM_UTIL_INPUT_T*)pv_obj;
    pt_obj = (MEDIA_FEEDER_NW_T*)pt_input->h_src_feeder;

    *pui8_cur_pos = pt_obj->ui8_pos;

        return INPUTR_OK;

#else
    return INPUTR_FAIL;
#endif    
}
#endif


/*-----------------------------------------------------------------------------
 * Name:  mm_util_network_pull_copybytes_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_network_pull_copybytes_fct(VOID*     pv_obj, 
                             VOID*     pv_buf, 
                             SIZE_T    z_buf_leng, 
                             SIZE_T    z_copy_size,
                             UINT32*   pui4_size)
{
#ifndef __NO_FM__
    UINT32                  ui4_read;
    INT32                   i4_ret;
    SIZE_T                  z_copy;
    MEDIA_FEEDER_NW_T*      pt_obj;
    UINT32                  ui4_size;
    UINT64                  ui8_addr;
    UINT32                  ui4_cpy_size;
    UINT32                  ui4_mem_adr;
    UINT64                  ui8_cache_addr;
    UINT64                  ui8_pos;
    MM_UTIL_INPUT_T*        pt_input;
    UINT8*                  aui1_buf;
    
    if((pv_obj == NULL) || (pv_buf == NULL) || (pui4_size == NULL))
    {
        return INPUTR_FAIL;
    }

    pt_input = (MM_UTIL_INPUT_T*)pv_obj;
    pt_obj = (MEDIA_FEEDER_NW_T*)pt_input->h_src_feeder;
    
    if (z_buf_leng < z_copy_size)
    {
        z_copy = z_buf_leng;
    }
    else
    {
        z_copy = z_copy_size;
    }

#ifdef LINUX_TURNKEY_SOLUTION
    if (pt_obj->aui1_share_buf)
    {
        aui1_buf = pt_obj->aui1_share_buf;
    }
    else
#endif
    {
        aui1_buf = pt_obj->aui1_cache_buf;
    }

    ui4_size = z_copy;
    ui4_mem_adr = (UINT32)pv_buf;
    ui8_addr = pt_obj->ui8_pos;
    ui8_pos = pt_obj->ui8_pos;
    ui8_cache_addr = pt_obj->ui8_cache_pos;
    
    i4_ret = INPUTR_OK;
    while (1)
    {   
        if (ui4_size == 0)
        {
            break;
        }
        if(pt_obj->b_canceled)
        {
#ifndef __KERNEL__
            x_dbg_stmt("!!!mm_util_network_pull_copybytes_fct: cancel reading due to cancel flag \n");
#endif
            return INPUTR_FAIL;
        }
        if ((0 == pt_obj->ui4_read_count) ||
            (ui8_addr < ui8_cache_addr) || 
            (ui8_addr >= (ui8_cache_addr + (UINT64)pt_obj->ui4_read_count)))
        {
            /* update Cache */
            i4_ret = pt_obj->t_pull_read_fct_tbl.pf_byteseek(
                                                pt_obj->h_pull,
                                                pt_obj->pv_app_tag,
                                                (INT64)ui8_addr,
                                                FM_SEEK_BGN,
                                                &ui8_pos);
            if (i4_ret != FMR_OK)
            {
                i4_ret = INPUTR_FAIL;
                break;
            }
            ui8_cache_addr = ui8_addr;
#ifdef LINUX_TURNKEY_SOLUTION
            if (pt_obj->aui1_share_buf)
            {
                i4_ret = pt_obj->t_pull_read_fct_tbl.pf_read(
                                                    pt_obj->h_pull,
                                                    pt_obj->pv_app_tag,
                                                    pt_obj->aui1_phy_buf,
                                                    NW_FEEDER_CACHE_SIZE, 
                                                    &ui4_read);
            }
            else
#endif
            {
                i4_ret = pt_obj->t_pull_read_fct_tbl.pf_read(
                                                    pt_obj->h_pull,
                                                    pt_obj->pv_app_tag,
                                                    aui1_buf,
                                                    NW_FEEDER_CACHE_SIZE, 
                                                    &ui4_read);

            }

            if ((i4_ret!=FMR_OK) && (i4_ret!=FMR_EOF)
#ifndef __KERNEL__
                && (i4_ret != IMTK_PB_CB_ERROR_CODE_EOF)
#endif
                )
            {
                i4_ret = INPUTR_FAIL;
                break;
            }
            pt_obj->ui4_read_count = ui4_read;
        }

        if ((ui8_addr + ui4_size) < (ui8_cache_addr + pt_obj->ui4_read_count))
        {
            ui4_cpy_size = ui4_size;
        }
        else
        {
            ui4_cpy_size = (ui8_cache_addr + pt_obj->ui4_read_count) - ui8_addr;
        }

        if ((ui4_mem_adr + ui4_cpy_size - (UINT32)pv_buf) > (UINT32)z_buf_leng)
        {
            i4_ret = INPUTR_FAIL;
            break;
        }

        x_memcpy((void *) ui4_mem_adr,  
            (void *) (aui1_buf + (ui8_addr - ui8_cache_addr)), 
            ui4_cpy_size);  

        ui8_addr += ui4_cpy_size;
        ui4_mem_adr += ui4_cpy_size;   

        if ((ui4_size <= ui4_cpy_size) || (pt_obj->ui4_read_count<NW_FEEDER_CACHE_SIZE))
        {
            break;
        }
        ui4_size -= ui4_cpy_size;
        
#ifndef __KERNEL__
        if(i4_ret == IMTK_PB_CB_ERROR_CODE_EOF)
        {
            i4_ret = INPUTR_EOF;
            break;
        }
#endif
    }

    *pui4_size = ui8_addr-pt_obj->ui8_pos;
    pt_obj->ui8_pos = ui8_addr;
    pt_obj->ui8_cache_pos = ui8_cache_addr;

    if ((i4_ret==FMR_EOF) || (i4_ret == IMTK_PB_CB_ERROR_CODE_EOF))
    {
        i4_ret = INPUTR_EOF;
    }

    return i4_ret;

    
#else
    return INPUTR_FAIL;
#endif   
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_network_pull_input4bytes_fct
 *
 * Description: big endian for 4 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_network_pull_input4bytes_b_fct(VOID* pv_obj, UINT32* pui4_data)
{
#ifndef __NO_FM__
    UINT32              ui4_data = 0;
    UINT32              ui4_read = 0;
    INT32               i4_ret;
    
    if((pv_obj == NULL) || (pui4_data == NULL))
    {
        return INPUTR_FAIL;
    }

    i4_ret = mm_util_network_pull_copybytes_fct(pv_obj, 
                                           (VOID*)&ui4_data, 
                                           4, 
                                           4,
                                           &ui4_read);
    *pui4_data = ((ui4_data&0xff000000)>>24) 
                 | ((ui4_data&0xff0000)>>8) 
                 | ((ui4_data&0xff00)<<8) 
                 | ((ui4_data&0xff)<<24);
    
    return i4_ret;
#else
    return INPUTR_FAIL;
#endif
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_network_pull_input3bytes_fct
 *
 * Description: big endian for 3 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_network_pull_input3bytes_b_fct(VOID* pv_obj, UINT32* pui4_data)
{
#ifndef __NO_FM__
    UINT32              ui4_data = 0;
    UINT32              ui4_read = 0;
    INT32               i4_ret;

    if((pv_obj == NULL) || (pui4_data == NULL))
    {
        return INPUTR_FAIL;
    }
    
    i4_ret = mm_util_network_pull_copybytes_fct(pv_obj, 
                                           (VOID*)&ui4_data, 
                                           3, 
                                           3,
                                           &ui4_read);
    *pui4_data = ((ui4_data&0xff0000)>>16) 
                 | (ui4_data&0xff00) 
                 | ((ui4_data&0xff)<<16);

    return i4_ret;

#else
    return INPUTR_FAIL;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_network_pull_input2bytes_b_fct
 *
 * Description: big endian for 2 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_network_pull_input2bytes_b_fct(VOID* pv_obj, UINT16* pui2_data)
{
#ifndef __NO_FM__
    UINT16              ui2_data = 0;
    UINT32              ui4_read = 0;
    INT32               i4_ret;

    if((pv_obj == NULL) || (pui2_data == NULL))
    {
        return INPUTR_FAIL;
    }
    
    i4_ret = mm_util_network_pull_copybytes_fct(pv_obj, 
                                           (VOID*)&ui2_data, 
                                           2, 
                                           2,
                                           &ui4_read);
    *pui2_data = ((ui2_data&0xff00)>>8) | ((ui2_data&0xff)<<8);
    return i4_ret;
    
#else
    return INPUTR_FAIL;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_network_pull_input4bytes_l_fct
 *
 * Description: little endian for 4 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_network_pull_input4bytes_l_fct(VOID* pv_obj, UINT32* pui4_data)
{
#ifndef __NO_FM__
    UINT32              ui4_data = 0;
    UINT32              ui4_read = 0;
    INT32               i4_ret;
    
    if((pv_obj == NULL) || (pui4_data == NULL))
    {
        return INPUTR_FAIL;
    }
    
    i4_ret = mm_util_network_pull_copybytes_fct(pv_obj, 
                                           (VOID*)&ui4_data, 
                                           4, 
                                           4,
                                           &ui4_read);
    *pui4_data = ui4_data;
    return i4_ret;
#else
    return INPUTR_FAIL;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_network_pull_input3bytes_l_fct
 *
 * Description: little endian for 3 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_network_pull_input3bytes_l_fct(VOID* pv_obj, UINT32* pui4_data)
{
#ifndef __NO_FM__
    UINT32              ui4_read = 0;
    INT32               i4_ret;
    
    if((pv_obj == NULL) || (pui4_data == NULL))
    {
        return INPUTR_FAIL;
    }
    
    i4_ret = mm_util_network_pull_copybytes_fct(pv_obj, 
                                           pui4_data, 
                                           3, 
                                           3,
                                           &ui4_read);

    return i4_ret;
#else
    return INPUTR_FAIL;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_network_pull_input2bytes_l_fct
 *
 * Description: little endian for 2 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_network_pull_input2bytes_l_fct(VOID* pv_obj, UINT16* pui2_data)
{
#ifndef __NO_FM__
    UINT32              ui4_read = 0;
    INT32               i4_ret;
    
    if((pv_obj == NULL) || (pui2_data == NULL))
    {
        return INPUTR_FAIL;
    }

    i4_ret = mm_util_network_pull_copybytes_fct(pv_obj, 
                                           pui2_data, 
                                           2, 
                                           2,
                                           &ui4_read);

    return i4_ret;

#else
    return INPUTR_FAIL;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_network_pull_input1bytes_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_network_pull_input1bytes_fct(VOID* pv_obj, UINT8* pui1_data)
{
#ifndef __NO_FM__
    UINT32              ui4_read = 0;
    INT32               i4_ret;
    
    if((pv_obj == NULL) || (pui1_data == NULL))
    {
        return INPUTR_FAIL;
    }
 
    i4_ret = mm_util_network_pull_copybytes_fct(pv_obj, 
                                           pui1_data, 
                                           1, 
                                           1,
                                           &ui4_read);

    return i4_ret;
#else
    return INPUTR_FAIL;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_network_pull_setpos_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_network_pull_setpos_fct(VOID* pv_obj, INT64 i8_offset, UINT8 ui1_whence)
{
#ifndef __NO_FM__
    MEDIA_FEEDER_NW_T*      pt_obj;
    INT64                   i8_pos;
    MM_UTIL_INPUT_T*        pt_input;

    if(pv_obj == NULL)
    {
        return INPUTR_FAIL;
    }

    pt_input = (MM_UTIL_INPUT_T*)pv_obj;
    pt_obj = (MEDIA_FEEDER_NW_T*)pt_input->h_src_feeder;

    switch (ui1_whence & (~FM_SEEK_CHK))
    {
        case MEDIA_SEEK_BGN:
            i8_pos = i8_offset;
            break;
            
        case MEDIA_SEEK_CUR: 
            i8_pos = (INT64)(pt_obj->ui8_pos);
            i8_pos = i8_pos + i8_offset;
            break;
            
        case MEDIA_SEEK_END:
            i8_pos = (INT64)(pt_obj->ui8_file_size);
            i8_pos = i8_pos + i8_offset;
            break;
            
        default:
            return INPUTR_FAIL;
    }

    /*Not Return Fail Just For The Case set end when FileSize=-1 OffSet=0*/
    if (i8_pos < 0 && (INT64)pt_obj->ui8_file_size != -1)
    {
        return INPUTR_FAIL;
    }

    if ((UINT64)i8_pos > pt_obj->ui8_file_size)
    {
        return INPUTR_EOF;
    }
    
    if(ui1_whence & FM_SEEK_CHK && !pt_obj->b_seekable_checked)/* Check Seekable,only check once*/
    {  
        INT32                   i4_ret  = 0;
        UINT64                  ui8_pos = 0;
        
        i4_ret = pt_obj->t_pull_read_fct_tbl.pf_byteseek(
                                            pt_obj->h_pull,
                                            pt_obj->pv_app_tag,
                                            i8_pos,
                                            FM_SEEK_BGN|FM_SEEK_CHK,
                                            &ui8_pos);
        pt_obj->b_seekable_checked = TRUE;
        
        if (i4_ret == FMR_BUSY)
        {
#ifndef __KERNEL__
            x_dbg_stmt("\nAttention:******\\\\seek busy,sever do not support seek\\\\************\r\n");
#endif
            return INPUTR_BUSY;
        }
    }
    pt_obj->ui8_pos = (UINT64)i8_pos;
    
    return INPUTR_OK;
#else
    return INPUTR_FAIL;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name:  mm_util_network_pull_getpos_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 mm_util_network_pull_getpos_fct(VOID* pv_obj, UINT64* pui8_cur_pos)
{
    MEDIA_FEEDER_NW_T*      pt_obj;
    MM_UTIL_INPUT_T*        pt_input;

    
    if((pv_obj == NULL) || (pui8_cur_pos == NULL))
    {
        return INPUTR_FAIL;
    }
    pt_input = (MM_UTIL_INPUT_T*)pv_obj;
    pt_obj = (MEDIA_FEEDER_NW_T*)pt_input->h_src_feeder;

    *pui8_cur_pos = pt_obj->ui8_pos;

    return INPUTR_OK;  
}


#ifdef CLI_LVL_ALL
/*-----------------------------------------------------------------------------
 * Name:  cli_feeder_copybytes_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 cli_feeder_copybytes_fct(VOID*   pv_feeder_obj, 
                               VOID*   pv_buf, 
                               SIZE_T  z_buf_leng, 
                               SIZE_T  z_copy_size,
                               UINT32* pui4_size)
{
#if 1
    MM_UTIL_INPUT_T t_input;
    MEDIA_FEEDER_T* pt_feeder_obj;
    
    if((pv_feeder_obj == NULL) || (pv_buf == NULL))
    {
        return INPUTR_FAIL;
    }

    pt_feeder_obj = (MEDIA_FEEDER_T*)pv_feeder_obj;
    t_input.h_file = pt_feeder_obj->h_feeder;
    
    return mm_util_fcopybytes_fct(&t_input, 
                                  pv_buf, 
                                  z_buf_leng, 
                                  z_copy_size,
                                  pui4_size);
#else
#ifndef __NO_FM__

    UINT32          ui4_read;
    INT32           i4_ret;
    SIZE_T          z_copy;
    MEDIA_FEEDER_T* pt_feeder_obj;

    pt_feeder_obj = (MEDIA_FEEDER_T*)pv_feeder_obj;
    
    if (z_buf_leng < z_copy_size)
    {
        z_copy = z_buf_leng;
    }
    else
    {
        z_copy = z_copy_size;
    }
    
    i4_ret = x_fm_read(pt_feeder_obj->h_feeder, pv_buf, z_copy, &ui4_read);
    if (i4_ret == FMR_OK)
    {
        return ui4_read;
    }
    else
    {
        return 0;
    }

#else

    return (UINT32)-1;

#endif
#endif
}
/*-----------------------------------------------------------------------------
 * Name:  cli_feeder_input4bytes_fct
 *
 * Description: big endian for 4 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 cli_feeder_input4bytes_b_fct(VOID* pv_feeder_obj, UINT32* pui4_data)
{
#if 1
    MM_UTIL_INPUT_T t_input;
    MEDIA_FEEDER_T* pt_feeder_obj;
    
    if((pv_feeder_obj == NULL) || (pui4_data == NULL))
    {
        return INPUTR_FAIL;
    }

    pt_feeder_obj = (MEDIA_FEEDER_T*)pv_feeder_obj;
    t_input.h_file = pt_feeder_obj->h_feeder;
    
    return mm_util_finput4bytes_b_fct(&t_input, pui4_data);
#else
#ifndef __NO_FM__

    UINT32          ui4_data;
    UINT32          ui4_read;
    INT32           i4_ret;
    MEDIA_FEEDER_T* pt_feeder_obj;

    pt_feeder_obj = (MEDIA_FEEDER_T*)pv_feeder_obj;
    
    i4_ret = x_fm_read(pt_feeder_obj->h_feeder, 
                       (VOID*)&ui4_data, 
                       sizeof(ui4_data), 
                       &ui4_read);
    if (i4_ret == FMR_OK)
    {
        return ((ui4_data&0xff000000)>>24) 
               | ((ui4_data&0xff0000)>>8) 
               | ((ui4_data&0xff00)<<8) 
               | ((ui4_data&0xff)<<24);
    }
    else
    {
        return (UINT32)-1;
    }
    
#else

    return (UINT32)-1;

#endif    
#endif
}
/*-----------------------------------------------------------------------------
 * Name:  cli_feeder_input3bytes_fct
 *
 * Description: big endian for 3 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 cli_feeder_input3bytes_b_fct(VOID* pv_feeder_obj, UINT32* pui4_data)
{
#if 1
    MM_UTIL_INPUT_T t_input;
    MEDIA_FEEDER_T* pt_feeder_obj;
    
    if((pv_feeder_obj == NULL) || (pui4_data == NULL))
    {
        return INPUTR_FAIL;
    }

    pt_feeder_obj = (MEDIA_FEEDER_T*)pv_feeder_obj;
    t_input.h_file = pt_feeder_obj->h_feeder;
    
    return mm_util_finput3bytes_b_fct(&t_input, pui4_data);
#else
#ifndef __NO_FM__

    UINT32          ui4_data = 0;
    UINT32          ui4_read;
    INT32           i4_ret;
    MEDIA_FEEDER_T* pt_feeder_obj;

    pt_feeder_obj = (MEDIA_FEEDER_T*)pv_feeder_obj;
    
    i4_ret = x_fm_read(pt_feeder_obj->h_feeder, (VOID*)&ui4_data, 3, &ui4_read);
    if (i4_ret == FMR_OK)
    {
        return ((ui4_data&0xff0000)>>16) | (ui4_data&0xff00) | ((ui4_data&0xff)<<16);
    }
    else
    {
        return (UINT32)-1;
    }

#else

    return (UINT32)-1;

#endif   
#endif
}
/*-----------------------------------------------------------------------------
 * Name:  cli_feeder_input2bytes_b_fct
 *
 * Description: big endian for 2 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 cli_feeder_input2bytes_b_fct(VOID* pv_feeder_obj, UINT16* pui2_data)
{
#if 1
    MM_UTIL_INPUT_T t_input;
    MEDIA_FEEDER_T* pt_feeder_obj;
    
    if((pv_feeder_obj == NULL) || (pui2_data == NULL))
    {
        return INPUTR_FAIL;
    }

    pt_feeder_obj = (MEDIA_FEEDER_T*)pv_feeder_obj;
    t_input.h_file = pt_feeder_obj->h_feeder;
    
    return mm_util_finput2bytes_b_fct(&t_input, pui2_data);
#else
#ifndef __NO_FM__

    UINT16          ui2_data;
    UINT32          ui4_read;
    INT32           i4_ret;
    MEDIA_FEEDER_T* pt_feeder_obj;

    pt_feeder_obj = (MEDIA_FEEDER_T*)pv_feeder_obj;
    
    i4_ret = x_fm_read(pt_feeder_obj->h_feeder, 
                       (VOID*)&ui2_data, 
                       sizeof(ui2_data), 
                       &ui4_read);
    if (i4_ret == FMR_OK)
    {
        return ((ui2_data&0xff00)>>8) | ((ui2_data&0xff)<<8);
    }
    else
    {
        return (UINT16)-1;
    }

#else

    return (UINT16)-1;

#endif    
#endif
}

/*-----------------------------------------------------------------------------
 * Name:  cli_feeder_input4bytes_l_fct
 *
 * Description: little endian for 4 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 cli_feeder_input4bytes_l_fct(VOID* pv_feeder_obj, UINT32* pui4_data)
{
#if 1
    MM_UTIL_INPUT_T t_input;
    MEDIA_FEEDER_T* pt_feeder_obj;
    
    if((pv_feeder_obj == NULL) || (pui4_data == NULL))
    {
        return INPUTR_FAIL;
    }

    pt_feeder_obj = (MEDIA_FEEDER_T*)pv_feeder_obj;
    t_input.h_file = pt_feeder_obj->h_feeder;
    
    return mm_util_finput4bytes_l_fct(&t_input, pui4_data);
#else
#ifndef __NO_FM__

    UINT32          ui4_data;
    UINT32          ui4_read;
    INT32           i4_ret;
    MEDIA_FEEDER_T* pt_feeder_obj;

    pt_feeder_obj = (MEDIA_FEEDER_T*)pv_feeder_obj;
    
    i4_ret = x_fm_read(pt_feeder_obj->h_feeder, 
                       (VOID*)&ui4_data, 
                       sizeof(ui4_data), 
                       &ui4_read);
    if (i4_ret == FMR_OK)
    {
        return (ui4_data);
    }
    else
    {
        return (UINT32)-1;
    }

#else

    return (UINT32)-1;

#endif
#endif
}

/*-----------------------------------------------------------------------------
 * Name:  cli_feeder_input3bytes_l_fct
 *
 * Description: little endian for 3 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 cli_feeder_input3bytes_l_fct(VOID* pv_feeder_obj, UINT32* pui4_data)
{
#if 1
    MM_UTIL_INPUT_T t_input;
    MEDIA_FEEDER_T* pt_feeder_obj;
    
    if((pv_feeder_obj == NULL) || (pui4_data == NULL))
    {
        return INPUTR_FAIL;
    }

    pt_feeder_obj = (MEDIA_FEEDER_T*)pv_feeder_obj;
    t_input.h_file = pt_feeder_obj->h_feeder;
    
    return mm_util_finput3bytes_l_fct(&t_input, pui4_data);
#else
#ifndef __NO_FM__


    UINT32          ui4_data;
    UINT32          ui4_read;
    INT32           i4_ret;
    MEDIA_FEEDER_T* pt_feeder_obj;

    ui4_data = 0;
    pt_feeder_obj = (MEDIA_FEEDER_T*)pv_feeder_obj;
    
    i4_ret = x_fm_read(pt_feeder_obj->h_feeder, (VOID*)&ui4_data, 3, &ui4_read);
    if (i4_ret == FMR_OK)
    {
        return (ui4_data);
    }
    else
    {
        return (UINT32)-1;
    }
    
#else

    return (UINT32)-1;

#endif    
#endif
}


/*-----------------------------------------------------------------------------
 * Name:  cli_feeder_input2bytes_l_fct
 *
 * Description: little endian for 2 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 cli_feeder_input2bytes_l_fct(VOID* pv_feeder_obj, UINT16* pui2_data)
{
#if 1
    MM_UTIL_INPUT_T t_input;
    MEDIA_FEEDER_T* pt_feeder_obj;
    
    if((pv_feeder_obj == NULL) || (pui2_data == NULL))
    {
        return INPUTR_FAIL;
    }

    pt_feeder_obj = (MEDIA_FEEDER_T*)pv_feeder_obj;
    t_input.h_file = pt_feeder_obj->h_feeder;
    
    return mm_util_finput2bytes_l_fct(&t_input, pui2_data);
#else
#ifndef __NO_FM__

    UINT16          ui2_data;
    UINT32          ui4_read;
    INT32           i4_ret;
    MEDIA_FEEDER_T* pt_feeder_obj;

    pt_feeder_obj = (MEDIA_FEEDER_T*)pv_feeder_obj;
    
    i4_ret = x_fm_read(pt_feeder_obj->h_feeder, 
                       (VOID*)&ui2_data, 
                       sizeof(ui2_data), 
                       &ui4_read);
    if (i4_ret == FMR_OK)
    {
        return (ui2_data);
    }
    else
    {
        return (UINT16)-1;
    }

#else

    return (UINT16)-1;

#endif    
#endif
}

/*-----------------------------------------------------------------------------
 * Name:  cli_feeder_input1bytes_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32  cli_feeder_input1bytes_fct(VOID* pv_feeder_obj, UINT8* pui1_data)
{
#if 1
    MM_UTIL_INPUT_T t_input;
    MEDIA_FEEDER_T* pt_feeder_obj;
    
    if((pv_feeder_obj == NULL) || (pui1_data == NULL))
    {
        return INPUTR_FAIL;
    }

    pt_feeder_obj = (MEDIA_FEEDER_T*)pv_feeder_obj;
    t_input.h_file = pt_feeder_obj->h_feeder;
    
    return mm_util_finput1bytes_fct(&t_input, pui1_data);
#else
#ifndef __NO_FM__

    UINT8           ui1_data;
    UINT32          ui4_read;
    INT32           i4_ret;
    MEDIA_FEEDER_T* pt_feeder_obj;

    pt_feeder_obj = (MEDIA_FEEDER_T*)pv_feeder_obj;
    
    i4_ret = x_fm_read(pt_feeder_obj->h_feeder, 
                       (VOID*)&ui1_data, 
                       sizeof(ui1_data), 
                       &ui4_read);
    if (i4_ret == FMR_OK)
    {
        return ui1_data;
    }
    else
    {
        return (UINT8)-1;
    }

#else

    return (UINT8)-1;

#endif    
#endif
}

/*-----------------------------------------------------------------------------
 * Name:  cli_feeder_setpos_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 cli_feeder_setpos_fct(VOID* pv_feeder_obj, INT64 i8_offset, UINT8 ui1_whence)
{
#if 1
    MM_UTIL_INPUT_T t_input;
    MEDIA_FEEDER_T* pt_feeder_obj;
    
    if(pv_feeder_obj == NULL)
    {
        return INPUTR_FAIL;
    }

    pt_feeder_obj = (MEDIA_FEEDER_T*)pv_feeder_obj;
    t_input.h_file = pt_feeder_obj->h_feeder;
    
    return mm_util_fsetpos_fct(&t_input, i8_offset, ui1_whence);
#else
#ifndef __NO_FM__

    UINT64          ui8_cur_pos;
    MEDIA_FEEDER_T* pt_feeder_obj;

    pt_feeder_obj = (MEDIA_FEEDER_T*)pv_feeder_obj;
    
    return x_fm_lseek(pt_feeder_obj->h_feeder, i8_offset, ui1_whence, &ui8_cur_pos);

#else

    return -1;

#endif    
#endif
}
/*-----------------------------------------------------------------------------
 * Name:  cli_feeder_getpos_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 cli_feeder_getpos_fct(VOID* pv_feeder_obj, UINT64* pui8_cur_pos)
{
#if 1
    MM_UTIL_INPUT_T t_input;
    MEDIA_FEEDER_T* pt_feeder_obj;
    
    if((pv_feeder_obj == NULL) || (pui8_cur_pos == NULL))
    {
        return INPUTR_FAIL;
    }

    pt_feeder_obj = (MEDIA_FEEDER_T*)pv_feeder_obj;
    t_input.h_file = pt_feeder_obj->h_feeder;
    
    return mm_util_fgetpos_fct(&t_input, pui8_cur_pos);
#else
#ifndef __NO_FM__

    INT32           i4_ret;
    UINT64          ui8_cur_pos;
    MEDIA_FEEDER_T* pt_feeder_obj;

    pt_feeder_obj = (MEDIA_FEEDER_T*)pv_feeder_obj;
    
    i4_ret = x_fm_lseek(pt_feeder_obj->h_feeder, 0, FM_SEEK_CUR, &ui8_cur_pos);
    if (i4_ret == FMR_OK)
    {
        return ui8_cur_pos;
    }
    else
    {
        return (UINT64)-1;
    }

#else

    return (UINT64)-1;

#endif
#endif
}
#endif /* CLI_LVL_ALL */


