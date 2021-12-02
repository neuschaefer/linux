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
 * $RCSfile: x_mfmtrecg.c,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 0a0e5908fac0e73885ce39efe82bcd61 $ Asa
 *
 * Description: 
 *         This file contains all the media format recognizer interface APIs  
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
#include "mutil/mfmtrecg/u_mfmtrecg.h"
#include "mutil/mfmtrecg/mfmtrecg.h"
#include "mutil/mfmtrecg/mfmtrecg_cli.h"
#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif
#include "dbg/x_dbg.h"
#include "dbg/u_dbg.h"
#include "mutil/mm_util.h"

#else

#include "x_os.h"
#include "x_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "u_mfmtrecg.h"
#include "mfmtrecg.h"
#include "mfmtrecg_cli.h"
#include "x_dbg.h"
#include "u_dbg.h"
#include "../mm_util.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
 #define MAX_FILE_EXTENSION_NAME 256
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
 * Name:  x_mfmtrecg_get_file_fmt
 *
 * Description: This API retrieves the format of the media file.
 *
 * Inputs:  ps_name         specify the path name of the media file which want to retrieve the format and it is not case sensitive.
 *
 * Outputs: pt_format       retrieve the format of the current media file.
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 x_mfmtrecg_get_file_fmt (
                                const CHAR*       ps_name,       
                                MEDIA_FORMAT_T*   pt_format)  
{
    
#ifndef __NO_FM__

    CHAR*                       ps_extension = NULL; 
    CHAR                        ac_extension[MAX_EXTENSION_NAME_LEN]; 
    INT32                       i4_ret = MFMTRECGR_INV_ARG;
    HANDLE_T                    h_file;
    BOOL                        b_fav_recg_only;
        
    DBG_API(("[mfmtrecg] x_mfmtrecg_get_file_fmt: ps_name:%s \t pt_format:%x\n\r"
        , ps_name, pt_format));  
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((ps_name == NULL)
        || (pt_format == NULL))
    {
        return MFMTRECGR_INV_ARG;
    }

    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /* Search backward. */
    ps_extension = x_strrchr( ps_name, '.' );/* ".mp3" */

    /************************************************************************/
    /*  check all handler                                                    */
    /************************************************************************/
    if (ps_extension)
    {
        b_fav_recg_only = pt_format->b_fav_recg_only;
        
        ps_extension++; /* cut off'.'=>"mp3" */ 
        x_memset(pt_format, 0, sizeof(MEDIA_FORMAT_T));
        x_memset(ac_extension, 0 , MAX_EXTENSION_NAME_LEN*sizeof(CHAR));
        x_memcpy(ac_extension, ps_extension, x_strlen(ps_extension));
        MUTIL_DBG_LOG_MW_0(("x_mfmtrecg : begin open file!\r\n"));
        /*MUTIL_DBG_LOG_MW_0(("x_mfmtrecg : file name : %s !\r\n",ps_name));*/
        i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_name, 
                           FM_READ_ONLY, 0777, 
                           FALSE, &h_file);
        MUTIL_DBG_LOG_MW_0(("x_mfmtrecg : end open file, ret = %d!\r\n",i4_ret));
        if (i4_ret == FMR_OK)
        {
            UINT64 ui8_cur_pos;
            UINT32 ui4_read;
            UINT32 ui4_code;
            i4_ret = x_fm_lseek(h_file, 0, FM_SEEK_BGN, &ui8_cur_pos);
            if (i4_ret == FMR_OK)
            {
                i4_ret = x_fm_read(h_file, &ui4_code, 4, &ui4_read);
                if (i4_ret != FMR_OK)
                {
                    x_fm_close(h_file);
                    return MFMTRECGR_FILE_ERR;
                }
            }
            else
            {
                x_fm_close(h_file);
                return MFMTRECGR_FILE_ERR;
            }
            i4_ret = x_fm_lseek(h_file, 0, FM_SEEK_BGN, &ui8_cur_pos);
            #if 0
            /* firstly, match favor string 
                         If ps extension need traverse the list ,which may improve performace
            */
            if ((x_strncmp(ps_extension, "mpg",  MAX_EXTENSION_NAME_LEN) != 0)
                && (x_strncmp(ps_extension, "mpeg", MAX_EXTENSION_NAME_LEN) != 0)
                && (x_strncmp(ps_extension ,"m1v", MAX_EXTENSION_NAME_LEN) != 0)
                && (x_strncmp(ps_extension ,"dat", MAX_EXTENSION_NAME_LEN) != 0)
                && (x_strncmp(ps_extension ,"vob", MAX_EXTENSION_NAME_LEN) != 0))
            {
                i4_ret = mfmtrecg_ask_handler_in_favor_by_file(h_file, 
                                                               ac_extension, 
                                                               pt_format);
            }
            else
            {
                i4_ret = MFMTRECGR_INTERNAL_ERR; /*call all handler to recognize*/
            }
            #endif
            /* firstly, match favor string */
            i4_ret = mfmtrecg_ask_handler_in_favor_by_file(h_file, 
                                                           ac_extension, 
                                                           pt_format);
            if (i4_ret == MFMTRECGR_OK ||
               i4_ret == MFMTRECGR_FILE_ERR)
            {
                /*do nothing*/
            }
            /* secondly, ask all handler */
            else if (i4_ret != MFMTRECGR_OK && (!b_fav_recg_only))
            {
                i4_ret = mfmtrecg_ask_all_handler_by_file(h_file, ac_extension, pt_format);
            }
        }      
        x_fm_close(h_file);
    }   
    return i4_ret;
#else
    return MFMTRECGR_INV_ARG;
#endif
}
/*-----------------------------------------------------------------------------
 * Name:  x_mfmtrecg_get_mem_fmt
 *
 * Description: This API retrieves the format of the media memory buffer.
 *
 * Inputs:  pv_mem_data     specify the address of memory buffer which want to retrieve the format.
 *          z_mem_leng      specify the length of memory buffer which want to retrieve the format.
 *
 * Outputs: pt_format       retrieve the format of the media memory buffer.
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 x_mfmtrecg_get_mem_fmt ( 
                        const VOID*       pv_mem_data, 
                        SIZE_T            z_mem_leng,   
                        MEDIA_FORMAT_T*   pt_format)   
{

    INT32 i4_ret;

    DBG_API(("[mfmtrecg] x_mfmtrecg_get_mem_fmt:pv_mem_data:%x \t z_mem_leng:%x \t pt_format:%x\n\r"
        , pv_mem_data, z_mem_leng, pt_format)); 
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_mem_data == NULL)
        || (pt_format == NULL))
    {
        return MFMTRECGR_INV_ARG;
    }

    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/

    i4_ret = mfmtrecg_ask_handler_in_favor_by_mem_with_fmt(pv_mem_data, z_mem_leng, pt_format);
    if (MFMTRECGR_OK != i4_ret)
    {
        if(MFMTRECGR_FILE_ERR == i4_ret)
        {
            return i4_ret;
        }
        return mfmtrecg_ask_all_handler_by_mem(pv_mem_data, z_mem_leng, pt_format);
    }
    else
    {
        return MFMTRECGR_OK;
    }
    
    
}


