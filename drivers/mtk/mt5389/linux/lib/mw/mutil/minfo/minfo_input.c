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
 * $RCSfile: minfo_input.c,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/4 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 9ce950eb85bf90ea84aa0781ae2e804f $ Asa
 *
 * Description: 
 *         This file contains the implementation of the media info input function
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
#include "mutil/minfo/minfo.h"
#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif
#include "minfo_input.h"

#else

#include "x_os.h"
#include "u_common.h"
#include "u_handle.h"
#include "minfo.h"
#include "../mm_util.h"

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
 * Name:  minfo_fcopybytes_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
UINT32 minfo_fcopybytes_fct    (VOID* pv_minfo_obj, VOID* pv_buf, SIZE_T z_buf_leng, SIZE_T z_copy_size)
{

#ifndef __NO_FM__
    UINT32          ui4_read;
    INT32           i4_ret;
    SIZE_T          z_copy;
    MINFO_OBJ_T*    pt_minfo_obj = (MINFO_OBJ_T*)pv_minfo_obj;
    
    if (z_buf_leng < z_copy_size)
    {
        z_copy = z_buf_leng;
    }
    else
    {
        z_copy = z_copy_size;
    }
    
    i4_ret = x_fm_read(pt_minfo_obj->h_file, pv_buf, z_copy, &ui4_read);
    if (i4_ret == FMR_OK)
    {
        return ui4_read;
    }
    else
    {
        return 0;
    }
#else
    return 0;
#endif    
}
/*-----------------------------------------------------------------------------
 * Name:  minfo_finput4bytes_fct
 *
 * Description: big endian for 4 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
UINT32 minfo_finput4bytes_b_fct  (VOID* pv_minfo_obj)
{

#ifndef __NO_FM__
    UINT32          ui4_data;
    UINT32          ui4_read;
    INT32           i4_ret;
    MINFO_OBJ_T*    pt_minfo_obj = (MINFO_OBJ_T*)pv_minfo_obj;
    
    i4_ret = x_fm_read(pt_minfo_obj->h_file, (VOID*)&ui4_data, sizeof(ui4_data), &ui4_read);
    if (i4_ret == FMR_OK)
    {
        return ((ui4_data&0xff000000)>>24) | ((ui4_data&0xff0000)>>8)| ((ui4_data&0xff00)<<8) | ((ui4_data&0xff)<<24);
    }
    else
    {
        return (UINT32)-1;
    }
#else
    return 0;
#endif
}
/*-----------------------------------------------------------------------------
 * Name:  minfo_finput3bytes_fct
 *
 * Description: big endian for 3 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
UINT32 minfo_finput3bytes_b_fct  (VOID* pv_minfo_obj)
{

#ifndef __NO_FM__
    UINT32          ui4_data = 0;
    UINT32          ui4_read;
    INT32           i4_ret;
    MINFO_OBJ_T*    pt_minfo_obj = (MINFO_OBJ_T*)pv_minfo_obj;
    
    i4_ret = x_fm_read(pt_minfo_obj->h_file, (VOID*)&ui4_data, 3, &ui4_read);
    if (i4_ret == FMR_OK)
    {
        return ((ui4_data&0xff0000)>>16) | (ui4_data&0xff00) | ((ui4_data&0xff)<<16);
    }
    else
    {
        return (UINT32)-1;
    }
#else
    return 0;
#endif    
}
/*-----------------------------------------------------------------------------
 * Name:  minfo_finput2bytes_b_fct
 *
 * Description: big endian for 2 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
UINT16 minfo_finput2bytes_b_fct  (VOID* pv_minfo_obj)
{

#ifndef __NO_FM__
    UINT16          ui2_data;
    UINT32          ui4_read;
    INT32           i4_ret;
    MINFO_OBJ_T*    pt_minfo_obj = (MINFO_OBJ_T*)pv_minfo_obj;
    
    i4_ret = x_fm_read(pt_minfo_obj->h_file, (VOID*)&ui2_data, sizeof(ui2_data), &ui4_read);
    if (i4_ret == FMR_OK)
    {
        return ((ui2_data&0xff00)>>8) | ((ui2_data&0xff)<<8);
    }
    else
    {
        return (UINT16)-1;
    }
#else
    return 0;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name:  minfo_finput4bytes_l_fct
 *
 * Description: little endian for 4 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
UINT32 minfo_finput4bytes_l_fct  (VOID* pv_minfo_obj)
{

#ifndef __NO_FM__
    UINT32          ui4_data;
    UINT32          ui4_read;
    INT32           i4_ret;
    MINFO_OBJ_T*    pt_minfo_obj = (MINFO_OBJ_T*)pv_minfo_obj;
    
    i4_ret = x_fm_read(pt_minfo_obj->h_file, (VOID*)&ui4_data, sizeof(ui4_data), &ui4_read);
    if (i4_ret == FMR_OK)
    {
        return (ui4_data);
    }
    else
    {
        return (UINT32)-1;
    }
#else
    return 0;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name:  minfo_finput3bytes_l_fct
 *
 * Description: little endian for 3 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
UINT32 minfo_finput3bytes_l_fct  (VOID* pv_minfo_obj)
{

#ifndef __NO_FM__
    UINT32          ui4_data = 0;
    UINT32          ui4_read;
    INT32           i4_ret;
    MINFO_OBJ_T*    pt_minfo_obj = (MINFO_OBJ_T*)pv_minfo_obj;
    
    i4_ret = x_fm_read(pt_minfo_obj->h_file, (VOID*)&ui4_data, 3, &ui4_read);
    if (i4_ret == FMR_OK)
    {
        return (ui4_data);
    }
    else
    {
        return (UINT32)-1;
    }
#else
    return 0;
#endif    
}


/*-----------------------------------------------------------------------------
 * Name:  minfo_finput2bytes_l_fct
 *
 * Description: little endian for 2 bytes input
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
UINT16 minfo_finput2bytes_l_fct  (VOID* pv_minfo_obj)
{

#ifndef __NO_FM__
    UINT16          ui2_data;
    UINT32          ui4_read;
    INT32           i4_ret;
    MINFO_OBJ_T*    pt_minfo_obj = (MINFO_OBJ_T*)pv_minfo_obj;
    
    i4_ret = x_fm_read(pt_minfo_obj->h_file, (VOID*)&ui2_data, sizeof(ui2_data), &ui4_read);
    if (i4_ret == FMR_OK)
    {
        return (ui2_data);
    }
    else
    {
        return (UINT16)-1;
    }
#else
    return 0;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name:  minfo_finput1bytes_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
UINT8  minfo_finput1bytes_fct  (VOID* pv_minfo_obj)
{

#ifndef __NO_FM__
    UINT8           ui1_data;
    UINT32          ui4_read;
    INT32           i4_ret;
    MINFO_OBJ_T*    pt_minfo_obj = (MINFO_OBJ_T*)pv_minfo_obj;
    
    i4_ret = x_fm_read(pt_minfo_obj->h_file, (VOID*)&ui1_data, sizeof(ui1_data), &ui4_read);
    if (i4_ret == FMR_OK)
    {
        return ui1_data;
    }
    else
    {
        return (UINT8)-1;
    }
#else
    return 0;
#endif    
}
/*-----------------------------------------------------------------------------
 * Name:  minfo_fsetpos_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
UINT32 minfo_fsetpos_fct           (VOID* pv_minfo_obj, INT64 i8_offset, UINT8 ui1_whence)
{

#ifndef __NO_FM__
    UINT64          ui8_cur_pos;
    MINFO_OBJ_T*    pt_minfo_obj = (MINFO_OBJ_T*)pv_minfo_obj;
    
    return x_fm_lseek(pt_minfo_obj->h_file, i8_offset, ui1_whence, &ui8_cur_pos);
#else
    return 0;
#endif    
}
/*-----------------------------------------------------------------------------
 * Name:  minfo_fgetpos_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
UINT64  minfo_fgetpos_fct (VOID* pv_minfo_obj)
{

#ifndef __NO_FM__
    MINFO_OBJ_T*    pt_minfo_obj = (MINFO_OBJ_T*)pv_minfo_obj;
    UINT64          ui8_cur_pos;
    INT32           i4_ret;
    
    i4_ret = x_fm_lseek(pt_minfo_obj->h_file, 0, FM_SEEK_CUR, &ui8_cur_pos);
    if (i4_ret == FMR_OK)
    {
        return ui8_cur_pos;
    }
    else
    {
        return (UINT64)-1;
    }
#else
    return 0;
#endif    
}
/*-----------------------------------------------------------------------------
 * Name:  minfo_mcopybytes_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
UINT32 minfo_mcopybytes_fct    (VOID* pv_minfo_obj, VOID* pv_buf, SIZE_T z_buf_leng, SIZE_T z_copy_size)
{
    SIZE_T          z_copy;
    MINFO_OBJ_T*    pt_minfo_obj = (MINFO_OBJ_T*)pv_minfo_obj;
    UCHAR*          puc_buff = (UCHAR*)pt_minfo_obj->pv_buff_or_file_path_name;
    
    if (z_buf_leng < z_copy_size)
    {
        z_copy = z_buf_leng;
    }
    else
    {
        z_copy = z_copy_size;
    }
    x_memcpy(pv_buf, (VOID*)&puc_buff[pt_minfo_obj->i8_buff_pos], z_copy);
    pt_minfo_obj->i8_buff_pos = pt_minfo_obj->i8_buff_pos + z_copy;
    
    return z_copy;
}
/*-----------------------------------------------------------------------------
 * Name:  minfo_minput4bytes_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
UINT32 minfo_minput4bytes_fct (VOID* pv_minfo_obj)
{
    UINT32          ui4_ret = 0;
    MINFO_OBJ_T*    pt_minfo_obj = (MINFO_OBJ_T*)pv_minfo_obj;
    UCHAR*          puc_buff = (UCHAR*)pt_minfo_obj->pv_buff_or_file_path_name;

    ui4_ret = (UINT32)(puc_buff[pt_minfo_obj->i8_buff_pos] << 24) 
            | (UINT32)(puc_buff[pt_minfo_obj->i8_buff_pos + 1] << 16) 
            | (UINT32)(puc_buff[pt_minfo_obj->i8_buff_pos + 2] << 8) 
            | (UINT32)(puc_buff[pt_minfo_obj->i8_buff_pos + 3]);
    pt_minfo_obj->i8_buff_pos = pt_minfo_obj->i8_buff_pos + 4;
    
    return ui4_ret;
}
/*-----------------------------------------------------------------------------
 * Name:  minfo_minput3bytes_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
UINT32 minfo_minput3bytes_fct (VOID* pv_minfo_obj)
{
    UINT32          ui4_ret = 0;
    MINFO_OBJ_T*    pt_minfo_obj = (MINFO_OBJ_T*)pv_minfo_obj;
    UCHAR*          puc_buff = (UCHAR*)pt_minfo_obj->pv_buff_or_file_path_name;

    ui4_ret = (UINT32)(puc_buff[pt_minfo_obj->i8_buff_pos] << 16) 
            | (UINT32)(puc_buff[pt_minfo_obj->i8_buff_pos + 1] << 8) 
            | (UINT32)(puc_buff[pt_minfo_obj->i8_buff_pos + 2]);
    pt_minfo_obj->i8_buff_pos = pt_minfo_obj->i8_buff_pos + 3;
    
    return ui4_ret;
}
/*-----------------------------------------------------------------------------
 * Name:  minfo_minput2bytes_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
UINT16 minfo_minput2bytes_fct (VOID* pv_minfo_obj)
{
    UINT16          ui2_ret = 0;
    MINFO_OBJ_T*    pt_minfo_obj = (MINFO_OBJ_T*)pv_minfo_obj;
    UCHAR*          puc_buff = (UCHAR*)pt_minfo_obj->pv_buff_or_file_path_name;

    ui2_ret = (UINT16)(puc_buff[pt_minfo_obj->i8_buff_pos] << 8) 
            | (UINT16)(puc_buff[pt_minfo_obj->i8_buff_pos + 1]);
    pt_minfo_obj->i8_buff_pos = pt_minfo_obj->i8_buff_pos + 2;
    
    return ui2_ret;
}
/*-----------------------------------------------------------------------------
 * Name:  minfo_minput1bytes_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
UINT8  minfo_minput1bytes_fct (VOID* pv_minfo_obj)
{
    UINT8           ui1_ret;
    MINFO_OBJ_T*    pt_minfo_obj = (MINFO_OBJ_T*)pv_minfo_obj;
    UCHAR*          puc_buff = (UCHAR*)pt_minfo_obj->pv_buff_or_file_path_name;

    ui1_ret = puc_buff[pt_minfo_obj->i8_buff_pos];
    pt_minfo_obj->i8_buff_pos = pt_minfo_obj->i8_buff_pos + 1;
    return ui1_ret;
}
/*-----------------------------------------------------------------------------
 * Name:  minfo_msetpos_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
UINT32 minfo_msetpos_fct          (VOID* pv_minfo_obj, INT64 i8_offset, UINT8 ui1_whence)
{
    MINFO_OBJ_T*    pt_minfo_obj = (MINFO_OBJ_T*)pv_minfo_obj;
    
    switch (ui1_whence)
    {
        case FM_SEEK_BGN:
            pt_minfo_obj->i8_buff_pos = i8_offset;
            break;
            
        case FM_SEEK_CUR: 
            pt_minfo_obj->i8_buff_pos = pt_minfo_obj->i8_buff_pos + i8_offset;
            break;
            
        case FM_SEEK_END:
            pt_minfo_obj->i8_buff_pos = pt_minfo_obj->z_buff_leng + i8_offset;
            break;
            
    }
    
    if (pt_minfo_obj->i8_buff_pos < 0)
    {
        pt_minfo_obj->i8_buff_pos = 0;
    }
    else if (pt_minfo_obj->i8_buff_pos > pt_minfo_obj->z_buff_leng)
    {
        pt_minfo_obj->i8_buff_pos = pt_minfo_obj->z_buff_leng;            
    }
    return MINFOR_OK;
}
/*-----------------------------------------------------------------------------
 * Name:  minfo_mgetpos_fct
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
UINT64  minfo_mgetpos_fct (VOID* pv_minfo_obj)
{
    MINFO_OBJ_T*    pt_minfo_obj = (MINFO_OBJ_T*)pv_minfo_obj;
    
    return (UINT64)pt_minfo_obj->i8_buff_pos;
}


