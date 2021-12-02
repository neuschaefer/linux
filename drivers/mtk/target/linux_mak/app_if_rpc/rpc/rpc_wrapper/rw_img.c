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
 * $RCSfile:$
 * $Revision: #5 $
 * $Date: 2012/08/28 $
 * $Author: xiuqin.wang $
 * $SWAuthor: Jun Zuo  $
 *
 * Description: 
 *         This file contains all Table Manager API related implementations.
 *         
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/
#include "handle/handle.h"
#include "img/x_img.h"
#include "aee/aee.h"
#include "img/c_img.h"
#include "img/u_img.h"
#include "img/tfx/u_img_tfx.h"
#include "img/tfx/c_img_tfx.h"
#include "img/tfx/x_img_tfx.h"

#include "rpc.h"
#include "ri_common.h"
#include "ri_desc.h"

#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>



static int mem_fd = -1;
/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name:  c_img_fopen
 *
 * Description: This function will open an image file, parse the image header, 
 *              and return the image handle.
 *
 * Inputs:  ps_name         Specifies the file name string which indicates the image file location, and 
 *                          it is not case sensitive.
 *          ui1_open_mode   specifies the open mode of the open operation such as IMG_READ_FILE_MODE.
 *          pf_nfy          specify the callback function to receive the notification of image library.
 *          pv_tag          The client data specified by the client and the callback function will send it again.
 *
 * Outputs: ph_img      specify the image handle which retrieve image information about the file.
 *
 * Returns: IMGR_OK                     Success. The image file opening is successful. 
 *          IMGR_FILE_OR_DIR_MISSING    FAIL. The file name or the path is not valid.
 *          IMGR_OUT_OF_HANDLE          FAIL. The system runs out of handles.
 *          IMGR_FCT_NOT_SUPPORT        FAIL. This function is not supported.
 *          IMGR_IMG_NOT_SUPPORT        FAIL. This image has no engine to decode.
 *          IMGR_INV_ARG                FAIL. Invalid arguments.
 *          IMGR_OUT_OF_MEMORY          FAIL. The system is out of memory.
 *          IMGR_FILE_ERR               FAIL. File system operation has error.
 *          IMGR_INTERNAL_ERR           FAIL. The image decoder occurs error during parsing the header. 
 ----------------------------------------------------------------------------*/
static INT32 _x_img_fopen (  const CHAR*     ps_name,        
                            UINT8           ui1_open_mode,  
                            c_img_nfy_fct   pf_nfy,         
                            const VOID*     pv_tag,         
                            HANDLE_T*       ph_img)
{
	RPC_DECL(5, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_REF_STR,		 ps_name);
    RPC_ARG_INP(ARG_TYPE_UINT8,    ui1_open_mode);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,	      pf_nfy);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,        pv_tag);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,      ph_img);

    RPC_DO_OP("x_img_fopen");

    RPC_RETURN(ARG_TYPE_INT32, IMGR_INV_ARG);
}

extern INT32 c_img_fopen (  const CHAR*     ps_name,        
                            UINT8           ui1_open_mode,  
                            c_img_nfy_fct   pf_nfy,         
                            const VOID*     pv_tag,         
                            HANDLE_T*       ph_img)
{
    HANDLE_T  h_aux;    
    INT32     i4_res;       
 
    i4_res = aee_grab_handle_resource( AEE_FLAG, 1, &h_aux);       
    if (i4_res == AEER_OK)    
    {        
        i4_res = _x_img_fopen (ps_name,        
                        ui1_open_mode,  
                        pf_nfy,         
                        pv_tag,         
                        ph_img);
        if (i4_res == IMGR_OK)        
        {            
            handle_link_to_aux(h_aux, *ph_img);        
        }        
        else        
        {            
            aee_release_handle_resource(1);        
        }    
    }    
    else    
    {        
        i4_res = IMGR_OUT_OF_MEMORY;    
    }  
    
    return (i4_res);
}

/*-----------------------------------------------------------------------------
 * Name:  c_img_mopen
 *
 * Description: This function will open an image memory block, parse the image header, and return the image handle.
 *
 * Inputs:  pv_mem_data     Specifies the memory address which contain whole image data.
 *          z_mem_leng      specifies the length of memory data pv_memory_data.  
 *          ps_format       specifies what decoder will be invoked to decode the image.
 *          pf_nfy          specify the callback function to receive the notification of image library.
 *          pv_tag          The client data specified by the client and the callback function will send it again.
 *
 * Outputs: ph_img      specify the image handle which retrieve image information about the file.
 *
 * Returns: IMGR_OK                     Success. The image file opening is successful. 
 *          IMGR_OUT_OF_HANDLE          FAIL. The system runs out of handles.
 *          IMGR_FCT_NOT_SUPPORT        FAIL. This function is not supported.
 *          IMGR_INV_ARG                FAIL. Invalid arguments.
 *          IMGR_IMG_NOT_SUPPORT        FAIL. This image has no engine to decode.
 *          IMGR_OUT_OF_MEMORY          FAIL. The system is out of memory.
 *          IMGR_INTERNAL_ERR           FAIL. The image decoder occurs error during parsing the header. 
 ----------------------------------------------------------------------------*/

static INT32 _x_img_mopen ( const VOID*     pv_mem_data,  
                           SIZE_T          z_mem_leng,   
                           const CHAR*     ps_format,    
                           c_img_nfy_fct   pf_nfy,       
                           const VOID*     pv_tag,       
                           HANDLE_T*       ph_img)
{
	RPC_DECL(6, INT32);    

    RPC_ARG_INP(ARG_TYPE_REF_VOID,	 pv_mem_data);
    RPC_ARG_INP(ARG_TYPE_UINT32,      z_mem_leng);
    RPC_ARG_INP(ARG_TYPE_REF_CHAR,     ps_format);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,	      pf_nfy);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,        pv_tag);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,      ph_img);

    RPC_DO_OP("x_img_mopen");

    RPC_RETURN(ARG_TYPE_INT32, IMGR_INV_ARG);
}
extern INT32 c_img_mopen ( const VOID*     pv_mem_data,  
                           SIZE_T          z_mem_leng,   
                           const CHAR*     ps_format,    
                           c_img_nfy_fct   pf_nfy,       
                           const VOID*     pv_tag,       
                           HANDLE_T*       ph_img)
{
    HANDLE_T  h_aux;    
    INT32     i4_res;       

    i4_res = aee_grab_handle_resource( AEE_FLAG, 1, &h_aux);       
    if (i4_res == AEER_OK)    
    {        
        i4_res = _x_img_mopen (pv_mem_data,  
                        z_mem_leng,   
                        ps_format,    
                        pf_nfy,       
                        pv_tag,       
                        ph_img);
        if (i4_res == IMGR_OK)        
        {            
            handle_link_to_aux(h_aux, *ph_img);        
        }        
        else        
        {            
            aee_release_handle_resource(1);        
        }    
    }    
    else    
    {        
        i4_res = IMGR_OUT_OF_MEMORY;    
    }  

    return (i4_res);
}

/*-----------------------------------------------------------------------------
 * Name:  c_img_close
 *
 * Description: After all image library's operation, call this function to close 
 *              the image handle, and free all resource, which is occupied by the image handle.
 *
 * Inputs:  h_img   specify the already opened image handle.
 *
 * Outputs: -
 *
 * Returns: IMGR_OK                 Success. The image file opening is successful. 
 *          IMGR_INV_HANDLE         FAIL. The image handle is invalid.
 *          IMGR_FCT_NOT_SUPPORT    FAIL. This function is not supported.
 *          IMGR_INV_ARG            FAIL. Invalid arguments.
 *          IMGR_INTERNAL_ERR       FAIL. semaphore lock is failed.
 ----------------------------------------------------------------------------*/                                     
extern INT32 c_img_close (HANDLE_T h_img) 
{
	RPC_DECL(1, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,          h_img);

    RPC_DO_OP("x_img_close");

    RPC_RETURN(ARG_TYPE_INT32, IMGR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name:  c_img_get_dimension
 *
 * Description: This function will retrieve the width and height of the primary image or thumbnail of the image handle
 *
 * Inputs:  h_img   specify the already opened image handle.
 *          e_type  specify what type of dimension want to get.
 *
 * Outputs: pz_width    retrieve the width of the image.
 *          pz_height   retrieve the height of the image.
 *
 * Returns: IMGR_OK                 Success. The API execution is successful. 
 *          IMGR_INV_HANDLE         FAIL. The image handle is invalid.
 *          IMGR_INV_ARG            FAIL. The pz_width or pz_height is NULL.
 *          IMGR_FCT_NOT_SUPPORT    FAIL. This function is not supported.
 ----------------------------------------------------------------------------*/
extern INT32 c_img_get_dimension ( HANDLE_T         h_img,      
                                   IMG_INTRA_TYPE_T e_type,     
                                   SIZE_T*          pz_width,   
                                   SIZE_T*          pz_height) 
{
	RPC_DECL(4, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,           h_img);
    RPC_ARG_INP(ARG_TYPE_INT32,	          e_type);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    pz_width);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,   pz_height);

    RPC_DO_OP("x_img_get_dimension");

    RPC_RETURN(ARG_TYPE_INT32, IMGR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name:  c_img_stop
 *
 * Description: This function can stop these types IMG_STOP_OP_TYPE_T of image operation.
 *
 * Inputs:  h_img       specify the image handle that want to stop.
 *          e_stop_type specify the type of stop operation. 
 *
 * Outputs: -
 *
 * Returns: IMGR_OK                 Success. The API execution is successful. 
 *          IMGR_INV_ARG            FAIL. Invalid arguments.
 *          IMGR_INV_HANDLE         FAIL. The image handle is invalid.
 *          IMGR_FCT_NOT_SUPPORT    FAIL. This function is not supported.
 *          IMGR_INTERNAL_ERR       FAIL. semaphore lock is failed.
 ----------------------------------------------------------------------------*/                                                      
extern INT32 c_img_stop (  HANDLE_T            h_img,        
                           IMG_STOP_OP_TYPE_T  e_stop_type)    
{
    RPC_DECL(2, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,           h_img);
    RPC_ARG_INP(ARG_TYPE_INT32,	     e_stop_type);

    RPC_DO_OP("x_img_stop");

    RPC_RETURN(ARG_TYPE_INT32, IMGR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name:  c_img_sync_load
 *
 * Description: This API will start decoding the primary image or thumbnail to the target surface h_dst_surf, and it will not return until decoding image is complete.
 *
 * Inputs:  h_img           specify the image handle that want to decode.
 *          e_type          specify the type of loading source image.
 *          pv_type_data    Reserve field for GIF file format, and set it NULL now.
 *          e_rotate_type   specify the rotation type of this loading operation.
 *          ui4_src_x       specify the x offset value to the origin of source rectangle on the primary image. It is no use in thumbnail loading.
 *          ui4_src_y       specify the y offset value to the origin of source rectangle on the primary image. It is no use in thumbnail loading.
 *          ui4_src_w       specify the width of source rectangle on the primary image. It is no use in thumbnail loading.
 *          ui4_src_h       specify the height of source rectangle on the primary image. It is no use in thumbnail loading.
 *          ui4_dst_x       specify the x offset value to the origin of destination rectangle on the target surface.
 *          ui4_dst_y       specify the y offset value to the origin of destination rectangle on the target surface.
 *          ui4_dst_w       specify the width of destination rectangle on the target surface.
 *          ui4_dst_h       specify the height of destination rectangle on the target surface.
 *          h_dst_surf      specify the destination surface, which the image will draw on it.
 *
 * Outputs: h_dst_surf      specify the destination surface, which the image will draw on it.
 *
 * Returns: IMGR_OK                     Success. The API execution is successful. 
 *          IMGR_INV_ARG                FAIL. Invalid arguments.
 *          IMGR_INV_HANDLE             FAIL. The image handle is invalid.
 *          IMGR_FCT_NOT_SUPPORT        FAIL. This function is not supported.
 *          IMGR_INTERNAL_ERR           FAIL. Semaphore lock is failed or decoding error.   
 *          IMGR_IMG_NOT_SUPPORT        FAIL. This image does not have thumbnail.
 *          IMGR_FILE_OR_DIR_MISSING    FAIL. The file path is invalid.
 *          IMGR_REENTRY_LOADING        FAIL. Loading operation of image decoder is reentrant.
 *          IMGR_OUT_OF_MEMORY          FAIL. The system is out of memory.
 *          IMGR_ABORT_LOADING          FAIL. Loading operation is aborted.
 *          IMGR_FILE_ERR               FAIL. The file system has error.
 ----------------------------------------------------------------------------*/                 
extern INT32 c_img_sync_load ( HANDLE_T             h_img,        
                               IMG_INTRA_TYPE_T     e_type,       
                               IMG_TYPE_DATA_T*     pt_type_data, 
                               IMG_ROTATE_OP_TYPE_T e_rotate_type,
                               UINT32               ui4_src_x,     
                               UINT32               ui4_src_y,     
                               UINT32               ui4_src_w,   
                               UINT32               ui4_src_h,   
                               UINT32               ui4_dst_x,    
                               UINT32               ui4_dst_y,    
                               UINT32               ui4_dst_w,   
                               UINT32               ui4_dst_h,   
                               GL_HSURFACE_T        h_dst_surf)   
{
	RPC_DECL(13, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,           h_img);
    RPC_ARG_INP(ARG_TYPE_INT32,	          e_type);
	RPC_ARG_IO(ARG_TYPE_REF_DESC,         pt_type_data);
	RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_type_data, RPC_DESC_IMG_TYPE_DATA_T, NULL));
	//RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_type_data->pv_tag, sizeof(UINT32*)));
	RPC_CHECK(RPC_ADD_REF_VAL(RPC_DEFAULT_ID, &pt_type_data->pv_tag));
    RPC_ARG_INP(ARG_TYPE_INT32,    e_rotate_type);
    RPC_ARG_INP(ARG_TYPE_UINT32,	   ui4_src_x);
    RPC_ARG_INP(ARG_TYPE_UINT32,	   ui4_src_y);
    RPC_ARG_INP(ARG_TYPE_UINT32,	   ui4_src_w);
    RPC_ARG_INP(ARG_TYPE_UINT32,	   ui4_src_h);
    RPC_ARG_INP(ARG_TYPE_UINT32,	   ui4_dst_x);
    RPC_ARG_INP(ARG_TYPE_UINT32,	   ui4_dst_y);
    RPC_ARG_INP(ARG_TYPE_UINT32,	   ui4_dst_w);
    RPC_ARG_INP(ARG_TYPE_UINT32,	   ui4_dst_h);
    RPC_ARG_IO(ARG_TYPE_UINT32,       h_dst_surf);

    RPC_DO_OP("x_img_sync_load");

    RPC_RETURN(ARG_TYPE_INT32, IMGR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name:  c_img_async_load
 *
 * Description: This API will send a message to the image decoding thread and then return to caller.
 *
 * Inputs:  h_img           specify the image handle that want to decode.
 *          e_type          specify the type of loading source image.
 *          pv_type_data    Reserve field for GIF file format, and set it NULL now.
 *          e_rotate_type   specify the rotation type of this loading operation.
 *          ui4_src_x       specify the x offset value to the origin of source rectangle on the primary image. It is no use in thumbnail loading.
 *          ui4_src_y       specify the y offset value to the origin of source rectangle on the primary image. It is no use in thumbnail loading.
 *          ui4_src_w       specify the width of source rectangle on the primary image. It is no use in thumbnail loading.
 *          ui4_src_h       specify the height of source rectangle on the primary image. It is no use in thumbnail loading.
 *          ui4_dst_x       specify the x offset value to the origin of destination rectangle on the target surface.
 *          ui4_dst_y       specify the y offset value to the origin of destination rectangle on the target surface.
 *          ui4_dst_w       specify the width of destination rectangle on the target surface.
 *          ui4_dst_h       specify the height of destination rectangle on the target surface.
 *          h_dst_surf      specify the destination surface, which the image will draw on it.
 *
 * Outputs: h_dst_surf      specify the destination surface, which the image will draw on it.
 *
 * Returns: IMGR_OK                        Success. The API execution is successful. 
 *          IMGR_INV_HANDLE                FAIL. The image handle is invalid.
 ----------------------------------------------------------------------------*/                                             
extern INT32 c_img_async_load (    HANDLE_T             h_img,        
                                   IMG_INTRA_TYPE_T     e_type,       
                                   IMG_TYPE_DATA_T*     pt_type_data, 
                                   IMG_ROTATE_OP_TYPE_T e_rotate_type,
                                   UINT32               ui4_src_x,     
                                   UINT32               ui4_src_y,     
                                   UINT32               ui4_src_w,    
                                   UINT32               ui4_src_h,    
                                   UINT32               ui4_dst_x,     
                                   UINT32               ui4_dst_y,     
                                   UINT32               ui4_dst_w,    
                                   UINT32               ui4_dst_h,    
                                   GL_HSURFACE_T        h_dst_surf)
{
    RPC_DECL(13, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,           h_img);
    RPC_ARG_INP(ARG_TYPE_INT32,	          e_type);
    RPC_ARG_INP(ARG_TYPE_REF_STR,   pt_type_data);
    RPC_ARG_INP(ARG_TYPE_INT32,    e_rotate_type);
    RPC_ARG_INP(ARG_TYPE_UINT32,	   ui4_src_x);
    RPC_ARG_INP(ARG_TYPE_UINT32,	   ui4_src_y);
    RPC_ARG_INP(ARG_TYPE_UINT32,	   ui4_src_w);
    RPC_ARG_INP(ARG_TYPE_UINT32,	   ui4_src_h);
    RPC_ARG_INP(ARG_TYPE_UINT32,	   ui4_dst_x);
    RPC_ARG_INP(ARG_TYPE_UINT32,	   ui4_dst_y);
    RPC_ARG_INP(ARG_TYPE_UINT32,	   ui4_dst_w);
    RPC_ARG_INP(ARG_TYPE_UINT32,	   ui4_dst_h);
    RPC_ARG_IO(ARG_TYPE_UINT32,       h_dst_surf);

    RPC_DO_OP("c_img_async_load");

    RPC_RETURN(ARG_TYPE_INT32, IMGR_INV_ARG);
}
/*-----------------------------------------------------------------------------
 * Name:  c_img_get_info_len
 *
 * Description: This function will get the length of the IMG_INFO_TYPE_T type information about the image and information is except the height and width.
 *
 * Inputs:  h_img           specify the image handle to retrieve the information length.
 *          e_info_type     specify the type of information that want to retrieve the information.
 *          pv_type_data    Reserve field for GIF file format, and set it NULL now.
 *
 * Outputs: pui4_len      retrieve the information length.
 *
 * Returns: IMGR_OK                     Success. The API execution is successful. 
 *          IMGR_INV_ARG                FAIL. Invalid arguments.
 *          IMGR_INV_HANDLE             FAIL. The image handle is invalid.
 *          IMGR_FCT_NOT_SUPPORT        FAIL. This function is not supported.
 ----------------------------------------------------------------------------*/                            
extern INT32 c_img_get_info_len (  HANDLE_T        h_img, 
                                   IMG_INFO_TYPE_T e_info_type,
                                   const VOID*     pv_type_data,
                                   UINT32*         pui4_len)   
{
	RPC_DECL(4, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,           h_img);
    RPC_ARG_INP(ARG_TYPE_INT32,	     e_info_type);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,  pv_type_data);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    pui4_len);
	
	RPC_DO_OP("x_img_get_info_len");

    RPC_RETURN(ARG_TYPE_INT32, IMGR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name:  c_img_get_exif_info_len
 *
 * Description: This function will get the length of each EXIF field.
 *
 * Inputs:  h_img           specify the image handle to retrieve the information length.
 *          e_info_type     specify primary of thumbnail information to be gotten.
 *          at_info         specify the array containing the EXIF information.
 *          
 * Outputs: ui4_count       specify the count of the pat_info array.
 *
 * Returns: IMGR_OK                     Success. The API execution is successful. 
 *          IMGR_INV_ARG                FAIL. Invalid arguments.
 *          IMGR_INV_HANDLE             FAIL. The image handle is invalid.
 *          IMGR_FCT_NOT_SUPPORT        FAIL. This function is not supported.
 *          IMGR_NO_EXIF_DATA           No EXIF data.
 ----------------------------------------------------------------------------*/ 
INT32 c_img_get_exif_info_len(
    HANDLE_T            h_img,
    IMG_INTRA_TYPE_T    e_info_type,
    EXIF_INFO_T         *at_info,
    UINT32              ui4_count)
{
	RPC_DECL(4, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,           h_img);
    RPC_ARG_INP(ARG_TYPE_INT32,	     e_info_type);
    RPC_ARG_IO(ARG_TYPE_REF_DESC,         at_info);
	RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, at_info, RPC_DESC_EXIF_INFO_T, NULL)); 
    RPC_ARG_INP(ARG_TYPE_UINT32,       ui4_count);
	
	RPC_DO_OP("x_img_get_exif_info_len");

    RPC_RETURN(ARG_TYPE_INT32, IMGR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name:  c_img_get_info
 *
 * Description: This function will get the IMG_INFO_TYPE_T type information about the image and information is except the height and width.
 *
 * Inputs:  h_img           specify the image handle to retrieve the information length.
 *          e_info_type     specify the type of information that want to retrieve the information.
 *          pv_type_data    Reserve field for GIF file format, and set it NULL now.
 *          pv_info         specify the length of pv_info buffer size.
 *          pui4_len        specify the memory address which contains the retrieved information.
 *
 * Outputs: pv_info     specify the really size that the information data used.
 *          pui4_len    the memory which contain the information data
 *
 * Returns: IMGR_OK                     Success. The API execution is successful. 
 *          IMGR_INV_ARG                FAIL. Invalid arguments.
 *          IMGR_INV_HANDLE             FAIL. The image handle is invalid.
 *          IMGR_FCT_NOT_SUPPORT        FAIL. This function is not supported.
 ----------------------------------------------------------------------------*/                            
extern INT32 c_img_get_info (  HANDLE_T        h_img,    
                               IMG_INFO_TYPE_T e_info_type,
                               const VOID*     pv_type_data,
                               VOID*           pv_info,     
                               UINT32*         pui4_len)
{
	RPC_DECL(5, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,            h_img);
    RPC_ARG_INP(ARG_TYPE_INT32,	      e_info_type);
	RPC_ARG_INP(ARG_TYPE_REF_DESC,      pv_type_data);
	RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_type_data, RPC_DESC_IMG_TYPE_DATA_T, NULL));
	
	if (e_info_type == IMG_INFO_IMG_TYPE)
	{
		RPC_ARG_IO(ARG_TYPE_REF_INT32, pv_info); 
	}
	else if (e_info_type == IMG_INFO_PRIMARY_MEET_ALL_LIMITATION || e_info_type == IMG_INFO_THUMBNAIL_MEET_ALL_LIMITATION )
	{
		RPC_ARG_IO(ARG_TYPE_REF_BOOL,  pv_info);
	}
	else 
	{
		RPC_FAIL;
	}
	RPC_ARG_IO(ARG_TYPE_REF_UINT32,      pui4_len);
	
	RPC_DO_OP("x_img_get_info");

    RPC_RETURN(ARG_TYPE_INT32, IMGR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name:  c_img_get_exif_info
 *
 * Description: This function will get the data of each EXIF field.
 *
 * Inputs:  h_img           specify the image handle to retrieve the information length.
 *          e_info_type     specify primary or thumbnail information to be gotten.
 *          at_info         specify the array containing the EXIF information.
 *          ui4_count       specify the count of the pat_info array.
 *
 * Outputs: -
 *
 * Returns: IMGR_OK                     Success. The API execution is successful. 
 *          IMGR_INV_ARG                FAIL. Invalid arguments.
 *          IMGR_INV_HANDLE             FAIL. The image handle is invalid.
 *          IMGR_FCT_NOT_SUPPORT        FAIL. This function is not supported.
 *          IMGR_NO_EXIF_DATA           No EXIF data.
 ----------------------------------------------------------------------------*/ 
INT32 c_img_get_exif_info(
    HANDLE_T            h_img,
    IMG_INTRA_TYPE_T    e_info_type,
    EXIF_INFO_T         *at_info,
    UINT32              ui4_count)
{
	RPC_DECL(4, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,           h_img);
    RPC_ARG_INP(ARG_TYPE_INT32,	     e_info_type);
    RPC_ARG_IO(ARG_TYPE_REF_DESC,         at_info);
	RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, at_info, RPC_DESC_EXIF_INFO_T, NULL)); 
	RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, at_info->pv_data, at_info->z_size)); 
    RPC_ARG_INP(ARG_TYPE_UINT32,       ui4_count);
	
	RPC_DO_OP("x_img_get_exif_info");

    RPC_RETURN(ARG_TYPE_INT32, IMGR_INV_ARG);
}
static INT32 _hndlr_x_image_pull_associate_fct(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        VOID *       pv_cb_addr,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    VOID*   pv_virt_addr = 0;
    
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type = ARG_TYPE_INT32;

    if (-1 == mem_fd)
    {
        mem_fd = open("/dev/rmmgr", O_RDWR|O_SYNC);
        if(mem_fd < 0)
        {
            //mem_fd = 0;
             return RPCR_NOT_FOUND;
        }
    }
    //if(mem_fd != 0)
    {
        pv_virt_addr = mmap(0, 
                            (UINT32)pt_args[1].u.ui4_arg, 
                            PROT_READ|PROT_WRITE, 
                            MAP_SHARED, 
                            mem_fd, 
                            pt_args[0].u.ui4_arg);
    }

    pt_return->u.i4_arg = (INT32)pv_virt_addr;

    return RPCR_OK;
}
static INT32 _hndlr_x_image_pull_dissociate_fct(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        VOID *       pv_cb_addr,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type = ARG_TYPE_INT32;
    
    munmap((VOID*)pt_args[0].u.ui4_arg, (SIZE_T)pt_args[1].u.ui4_arg);
    
    pt_return->u.i4_arg = 0;

    return RPCR_OK;
}


static INT32 _hndlr_x_image_pull_open_fct(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        VOID *       pv_cb_addr,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type = ARG_TYPE_INT32;
    
    pt_return->u.i4_arg = ((imageFmOpenFunc)pv_cb_addr)(
                                 pt_args[0].u.pc_arg,
                                 pt_args[1].u.pui4_arg,
                                 pt_args[2].u.pv_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_image_pull_close_fct(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        VOID *       pv_cb_addr,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type = ARG_TYPE_INT32;
    
    pt_return->u.i4_arg = ((imageFmCloseFunc)pv_cb_addr)(
                                pt_args[0].u.ui4_arg,
                                pt_args[1].u.pv_arg);

    return RPCR_OK;
}


static INT32 _hndlr_x_image_pull_read_fct(
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
    pt_return->e_type = ARG_TYPE_INT32;
    
    pt_return->u.i4_arg = ((imageFmReadFunc)pv_cb_addr)(
                                pt_args[0].u.ui4_arg,
                                (UINT8*)pt_args[1].u.pv_arg,
                                (UINT32)pt_args[2].u.ui4_arg,
                                (UINT32*)pt_args[3].u.pui4_arg,
                                pt_args[4].u.pv_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_image_pull_write_fct(
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
    pt_return->e_type = ARG_TYPE_INT32;

    pt_return->u.i4_arg = ((imageFmWriteFunc)pv_cb_addr)(
                                pt_args[0].u.ui4_arg,
                                (UINT8*)pt_args[1].u.pv_desc,
                                (UINT32)pt_args[2].u.ui4_arg,
                                (UINT32*)pt_args[3].u.pui4_arg,
                                pt_args[4].u.pv_arg);

    return RPCR_OK;
}
static INT32 _hndlr_x_image_pull_seek_fct(
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
    pt_return->e_type = ARG_TYPE_INT32;

    pt_return->u.i4_arg = ((imageFmSeekFunc)pv_cb_addr)(
                                pt_args[0].u.ui4_arg,
                                (UINT64)pt_args[1].u.ui8_arg,
                                (UINT8)pt_args[2].u.ui1_arg,
                                (UINT64*)pt_args[3].u.pui8_arg,
                                pt_args[4].u.pv_arg);

    return RPCR_OK;
}
static INT32 _hndlr_x_image_pull_getInfo_fct(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        VOID *       pv_cb_addr,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type = ARG_TYPE_INT32;

    pt_return->u.i4_arg = ((imageFmGetInfoFunc)pv_cb_addr)(
                                pt_args[0].u.pc_arg,
                                (IMAGE_FILE_INFO_T*)pt_args[1].u.pv_desc,
                                pt_args[2].u.pv_arg);

    return RPCR_OK;
}
INT32 c_img_set_fm_function(IMAGE_PULL_FM_FUNC_T * pt_func)
{
	RPC_DECL(1, INT32);

	RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_func, RPC_DESC_IMAGE_PULL_FM_FUNC_T, NULL);
	RPC_ADD_REF_VAL(RPC_DEFAULT_ID, &pt_func->pv_app_tag);
	RPC_ARG_INP(ARG_TYPE_REF_DESC,   pt_func);

	RPC_DO_OP("x_img_set_fm_function");
	
	RPC_RETURN(ARG_TYPE_INT32, IMGR_RPC_ERROR);
}
/*-----------------------------------------------------------------------------
 * Name:  c_img_set_info
 *
 * Description: This function will set the IMG_INFO_TYPE_T type information about the image and information is except the height and width.
 *
 * Inputs:  h_img           specify the image handle to retrieve the information length.
 *          e_info_type     specify the type of information that want to retrieve the information.
 *          pv_type_data    Reserve field for GIF file format, and set it NULL now.
 *          pv_info         specify the length of pv_info buffer size.
 *          pui4_len        specify the information length.
 *
 * Outputs: -
 *
 * Returns: IMGR_OK                     Success. The API execution is successful. 
 *          IMGR_INV_ARG                FAIL. Invalid arguments.
 *          IMGR_INV_HANDLE             FAIL. The image handle is invalid.
 *          IMGR_FCT_NOT_SUPPORT        FAIL. This function is not supported.
 ----------------------------------------------------------------------------*/                            
extern INT32 c_img_set_info (  HANDLE_T        h_img,    
                               IMG_INFO_TYPE_T e_info_type,
                               const VOID*     pv_type_data,
                               VOID*           pv_info,     
                               UINT32          ui4_len)
{
	RPC_DECL(5, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,           h_img);
    RPC_ARG_INP(ARG_TYPE_INT32,	     e_info_type);
	RPC_ARG_INP(ARG_TYPE_REF_VOID,	pv_type_data);
	RPC_ARG_INP(ARG_TYPE_REF_VOID,	     pv_info);
    RPC_ARG_INP(ARG_TYPE_UINT32,         ui4_len);
	
	RPC_DO_OP("x_img_set_info");

    RPC_RETURN(ARG_TYPE_INT32, IMGR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name:  c_img_set_exif_info
 *
 * Description: This function will set the data of each EXIF field.
 *
 * Inputs:  h_img           specify the image handle to retrieve the information length.
 *          e_info_type     specify primary or thumbnail information to be set.
 *          at_info         specify the array containing the EXIF information.
 *          ui4_count       specify the count of the pat_info array.
 *
 * Outputs: -
 *
 * Returns: IMGR_OK                     Success. The API execution is successful. 
 *          IMGR_INV_ARG                FAIL. Invalid arguments.
 *          IMGR_INV_HANDLE             FAIL. The image handle is invalid.
 *          IMGR_FCT_NOT_SUPPORT        FAIL. This function is not supported.
 *          IMGR_NO_EXIF_DATA           No EXIF data.
 ----------------------------------------------------------------------------*/ 
INT32 c_img_set_exif_info(
    HANDLE_T            h_img,
    IMG_INTRA_TYPE_T    e_info_type,
    EXIF_INFO_T         *at_info,
    UINT32              ui4_count)
{
	RPC_DECL(4, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,           h_img);
    RPC_ARG_INP(ARG_TYPE_INT32,	     e_info_type);
	RPC_ARG_IO(ARG_TYPE_REF_DESC,         at_info);
	RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, at_info, RPC_DESC_EXIF_INFO_T, NULL)); 
	RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, at_info->pv_data, at_info->z_size)); 
    RPC_ARG_INP(ARG_TYPE_UINT32,       ui4_count);
	
	RPC_DO_OP("x_img_set_exif_info");

    RPC_RETURN(ARG_TYPE_INT32, IMGR_INV_ARG);
}
/*-----------------------------------------------------------------------------
 * Name:  c_img_open_exif
 *
 * Description: This function will open an JPG file, just parse the EXIF info, 
 *              and return the image handle. The main flow to get the exif info likes
 *              open_exif->get_exif_info_len->get_exif_info->close_exif
 *
 * Inputs:  pv_src      specifies the source file path string or memory address which contain image data and It depend on the second parameter e_type.
 *          z_len       specify the length of pv_src.
 *          e_type      specifies pv_src is a file path string or a memory address which contain image data.
 *
 * Outputs: ph_img      specify the image handle which retrieve image information about the file.
 *
 ----------------------------------------------------------------------------*/


INT32 c_img_open_exif(
    IMG_SRC_TYPE_T e_type,
    const VOID * pv_src,
    SIZE_T z_len,
    HANDLE_T * ph_img)
{
	RPC_DECL(4, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_INT32,           e_type);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,	      pv_src);
	RPC_ARG_INP(ARG_TYPE_UINT32,	       z_len);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,      ph_img);
	
	RPC_DO_OP("x_img_open_exif");

    RPC_RETURN(ARG_TYPE_INT32, IMGR_INV_ARG);
}

INT32 c_img_close_exif(HANDLE_T h_img)
{
	RPC_DECL(1, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,           h_img);
	
	RPC_DO_OP("x_img_close_exif");

    RPC_RETURN(ARG_TYPE_INT32, IMGR_INV_ARG);
}

INT32 c_img_get_capability(IMG_CAPS_T *pt_caps)
{
	RPC_DECL(1, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_caps, RPC_DESC_IMG_CAPS_T, NULL));

    RPC_ARG_OUT(ARG_TYPE_REF_DESC,           pt_caps);

	RPC_DO_OP("x_img_get_capability");

    RPC_RETURN(ARG_TYPE_INT32, IMGR_INV_ARG);
}

INT32 c_img_reparse_exif(HANDLE_T*      ph_img)
{
	RPC_DECL(1, INT32);    
    
    RPC_ARG_IO(ARG_TYPE_REF_UINT32,           ph_img);
	
	RPC_DO_OP("x_img_reparse_exif");

    RPC_RETURN(ARG_TYPE_INT32, IMGR_INV_ARG);
}

INT32 c_img_tfx_create(HANDLE_T * ph_tfx,
                       IMG_TFX_NFY_FCT_T pf_nfy_cb,
                       VOID * pv_tag)
{
    RPC_DECL(3, INT32);    
    
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,       ph_tfx);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,      pf_nfy_cb);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,         pv_tag);
	
	RPC_DO_OP("x_img_tfx_create");

    RPC_RETURN(ARG_TYPE_INT32, IMGR_INV_ARG);
}

INT32 c_img_tfx_delete(HANDLE_T h_tfx)
{
    RPC_DECL(1, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,           h_tfx);
	
	RPC_DO_OP("x_img_tfx_delete");

    RPC_RETURN(ARG_TYPE_INT32, IMGR_INV_ARG);
}

INT32 c_img_tfx_play(HANDLE_T h_tfx,
                     GL_HSURFACE_T h_dst,
                     GL_HSURFACE_T h_src,
                     TFX_AUX_DATA_T * pt_aux,
                     IMG_TFX_EFFECT_T e_effect,
                     IMG_TFX_SCHEME_T e_scheme)
{
    RPC_DECL(6, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,           h_tfx);
    RPC_ARG_INP(ARG_TYPE_UINT32,           h_dst);
    RPC_ARG_INP(ARG_TYPE_UINT32,           h_src);
    RPC_ARG_INP(ARG_TYPE_REF_STR,         pt_aux);
    RPC_ARG_INP(ARG_TYPE_INT32,         e_effect);
    RPC_ARG_INP(ARG_TYPE_INT32,         e_scheme);
	
	RPC_DO_OP("x_img_tfx_play");

    RPC_RETURN(ARG_TYPE_INT32, IMGR_INV_ARG);
}

INT32 c_img_tfx_stop(HANDLE_T h_tfx)
{
    RPC_DECL(1, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,           h_tfx);
	
	RPC_DO_OP("x_img_tfx_stop");

    RPC_RETURN(ARG_TYPE_INT32, IMGR_INV_ARG);
}

INT32 c_img_tfx_pause(HANDLE_T h_tfx)
{
    RPC_DECL(1, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,           h_tfx);
	
	RPC_DO_OP("x_img_tfx_pause");

    RPC_RETURN(ARG_TYPE_INT32, IMGR_INV_ARG);
}

INT32 c_img_tfx_get_effect_info(IMG_TFX_EFFECT_T e_effect,
                                TFX_FRAME_INFO_T * pt_frame_info)
{
    RPC_DECL(2, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_INT32,           e_effect);
	RPC_ARG_OUT(ARG_TYPE_REF_STR,    pt_frame_info);
    
	RPC_DO_OP("x_img_tfx_get_effect_info");

    RPC_RETURN(ARG_TYPE_INT32, IMGR_INV_ARG);
}

INT32 c_img_tfx_set_effect_info(IMG_TFX_EFFECT_T e_effect,
                                TFX_FRAME_INFO_T * pt_frame_info)
{
    RPC_DECL(2, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_INT32,           e_effect);
	RPC_ARG_OUT(ARG_TYPE_REF_STR,    pt_frame_info);
    
	RPC_DO_OP("x_img_tfx_set_effect_info");

    RPC_RETURN(ARG_TYPE_INT32, IMGR_INV_ARG);
}
INT32 c_rpc_reg_img_cb_hndlrs()
{
	RPC_REG_CB_HNDLR(x_image_pull_associate_fct);
	RPC_REG_CB_HNDLR(x_image_pull_dissociate_fct);
	RPC_REG_CB_HNDLR(x_image_pull_open_fct);
	RPC_REG_CB_HNDLR(x_image_pull_close_fct);
	RPC_REG_CB_HNDLR(x_image_pull_read_fct);
	RPC_REG_CB_HNDLR(x_image_pull_write_fct);
	RPC_REG_CB_HNDLR(x_image_pull_seek_fct);
	RPC_REG_CB_HNDLR(x_image_pull_getInfo_fct);
	
	return RPCR_OK;
}
