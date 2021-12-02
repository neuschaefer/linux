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
 * $RCSfile: c_midxbuld.c,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/2 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: ef3e324d0f4d339ae7a007dd03a47573 $ Asa
 *
 * Description: 
 *         This file contains all the media index builder interface APIs  
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

#include "inc/x_common.h"
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "x_midxbuld.h"
#include "aee/aee.h"
#include "dbg/x_dbg.h"

#else

#include "x_os.h"
#include "x_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "x_midxbuld.h"
#include "x_dbg.h"

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
 * Name:  c_midxbuld_fopen
 *
 * Description: This API opens a media file to do media indexing.
 *
 * Inputs:  ps_path_name    specify the path string of file which will be opened in minfo.
 *          pf_nfy          specify the function point of notification function.
 *          pv_tag          specify the tag which will be send back to AP when minfo notification.
 *          pt_format       if the application specify the format (except MEDIA_TYPE_UNKNOWN), media info module will use this information to select the right handler to deal with the media file.
 *
 * Outputs: pt_format       If the application specify the unknown format (ie. MEDIA_TYPE_UNKNOWN), media info module will use format recognizer to look it up; then return it to AP.
 *          ph_midxbuld        The parameter will retrieve a media index builder handle.
 *
 * Returns: MIDXBULDR    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/                       
INT32 c_midxbuld_fopen (
                        CHAR*                    ps_path_name,
                        x_midxbuld_nfy_fct       pf_nfy,      
                        VOID*                    pv_tag,     
                        MEDIA_FORMAT_T*          pt_format,
                        MIDXBULD_SOURCE_INFO_T*  pt_src_info,
                        MIDXBULD_KEY_INFO_T*     pt_key_info,
                        HANDLE_T*                ph_midxbuld)
{
    HANDLE_T  h_aux;    
    INT32     i4_res;       

    i4_res = aee_grab_handle_resource( AEE_FLAG, 1, &h_aux);       
    if (i4_res == AEER_OK)    
    {        
        i4_res = x_midxbuld_fopen (
                        ps_path_name,
                        pf_nfy,    
                        pv_tag, 
                        pt_format,
                        pt_src_info,
                        pt_key_info,
                        ph_midxbuld); 
        if (i4_res == MIDXBULDR_OK)        
        {            
            handle_link_to_aux(h_aux, *ph_midxbuld);        
        }        
        else        
        {            
            aee_release_handle_resource(1);        
        }    
    }    
    else    
    {        
        i4_res = MIDXBULDR_OUT_OF_MEMORY;    
    }  
    
    return (i4_res);
}

/*-----------------------------------------------------------------------------
 * Name:  c_midxbuld_mopen
 *
 * Description: This API opens a media memory buffer to do media indexing.
 *
 * Inputs:  pv_mem_data     specify the memory address of media data buffer.
 *          z_mem_leng      specify the memory length of media data buffer.
 *          pf_nfy          specify the function point of notification function.
 *          pv_tag          specify the tag which will be send back to AP when minfo notification.
 *          pt_format       if the application specify the format (except MEDIA_TYPE_UNKNOWN), media info module will use this information to select the right handler to deal with the media file.
 *
 * Outputs: pt_format       If the application specify the unknown format (ie. MEDIA_TYPE_UNKNOWN), media info module will use format recognizer to look it up; then return it to AP.
 *          ph_midxbuld     The parameter will retrieve a media index builder handle.
 *
 * Returns: MIDXBULDR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/                       
INT32 c_midxbuld_mopen (
                        VOID*                    pv_mem_data,   
                        SIZE_T                   z_mem_leng,   
                        x_midxbuld_nfy_fct       pf_nfy,
                        VOID*                    pv_tag,  
                        MEDIA_FORMAT_T*          pt_format,
                        MIDXBULD_SOURCE_INFO_T*  pt_src_info,
                        MIDXBULD_KEY_INFO_T*     pt_key_info,
                        HANDLE_T*                ph_midxbuld)
{
    HANDLE_T  h_aux;    
    INT32     i4_res;       

    i4_res = aee_grab_handle_resource( AEE_FLAG, 1, &h_aux);       
    if (i4_res == AEER_OK)    
    {        
        i4_res = x_midxbuld_mopen (
                        pv_mem_data,
                        z_mem_leng,
                        pf_nfy,    
                        pv_tag, 
                        pt_format,
                        pt_src_info,
                        pt_key_info,
                        ph_midxbuld); 
        if (i4_res == MIDXBULDR_OK)        
        {            
            handle_link_to_aux(h_aux, *ph_midxbuld);        
        }        
        else        
        {            
            aee_release_handle_resource(1);        
        }    
    }    
    else    
    {        
        i4_res = MIDXBULDR_OUT_OF_MEMORY;    
    }  
    
    return (i4_res);
}

/*-----------------------------------------------------------------------------
 * Name:  c_midxbuld_close
 *
 * Description: TThis API closes the media index builder handle.
 *
 * Inputs:  h_midxbuld         specify the media index builder handle which will be closed.
 *
 * Outputs: 
 *
 * Returns: MIDXBULDR    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 c_midxbuld_close (HANDLE_T h_midxbuld)
{
    return x_midxbuld_close(h_midxbuld);
}

/*-----------------------------------------------------------------------------
 * Name:  c_midxbuld_start_indexing
 *
 * Description: specify the media index builder handle which will start indexing
 *
 * Inputs:  h_midxbuld      specify the media info handle which will be retrieved information.
 *          
 *
 * Outputs: 
 *
 * Returns: MIDXBULDR    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
 INT32 c_midxbuld_start_indexing (HANDLE_T h_midxbuld)   
{
    return x_midxbuld_start_indexing(h_midxbuld);
}

/*-----------------------------------------------------------------------------
 * Name:  c_midxbuld_stop_indexing
 *
 * Description: specify the media index builder handle which will stop indexing
 *
 * Inputs:  h_midxbuld      specify the media info handle which will be retrieved information.
 *          
 *
 * Outputs: 
 *
 * Returns: MIDXBULDR    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 c_midxbuld_stop_indexing (HANDLE_T h_midxbuld)   
{
    return x_midxbuld_stop_indexing(h_midxbuld);
}


/*-----------------------------------------------------------------------------
 * Name:  c_midxbuld_get_info
 *
 * Description: This API retrieves the information of media idx builder handle. It will return MINFO_NOT_READY, before parse media complete.
 *
 * Inputs:  h_midxbuld   specify the media idx buld handle which will be retrieved information.
 *          e_info_type  specify the information type of the media.
 *          pv_buf       specify the memory buffer which will contain the media information.
 *          pz_buf_leng  specify the length of the buffer.
 *
 * Outputs: pv_buf       specify the memory buffer which will contain the media information
 *          pz_buf_leng  If it is smaller than required, it will return MINFO_INSUFFICENT_MEM, and pz_buf_leng will contain required length.
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 c_midxbuld_get_info (
                        HANDLE_T                 h_midxbuld, 
                        MIDXBULD_GET_INFO_TYPE_T e_info_type, 
                        VOID*                    pv_buf,     
                        SIZE_T*                  pz_buf_leng)
{
    return x_midxbuld_get_info(h_midxbuld, e_info_type, pv_buf, pz_buf_leng);
}

/*-----------------------------------------------------------------------------
 * Name:  c_midxbuld_set_info
 *
 * Description: This API retrieves the information of media idx builder handle. It will return MINFO_NOT_READY, before parse media complete.
 *
 * Inputs:  h_midxbuld   specify the media idx buld handle which will be retrieved information.
 *          e_info_type  specify the information type of the media.
 *          pv_buf       specify the memory buffer which will contain the media information.
 *          pz_buf_leng  specify the length of the buffer.
 *
 * Outputs: pv_buf       specify the memory buffer which will contain the media information
 *          pz_buf_leng  If it is smaller than required, it will return MINFO_INSUFFICENT_MEM, and pz_buf_leng will contain required length.
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 c_midxbuld_set_info (
                        HANDLE_T                 h_midxbuld, 
                        MIDXBULD_SET_INFO_TYPE_T e_info_type, 
                        VOID*                    pv_buf,     
                        SIZE_T*                  pz_buf_leng)
{
    return x_midxbuld_set_info(h_midxbuld, e_info_type, pv_buf, pz_buf_leng);
}

