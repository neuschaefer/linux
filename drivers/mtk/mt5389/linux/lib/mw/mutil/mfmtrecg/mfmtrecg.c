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
 * $RCSfile: mfmtrecg.c,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 0a0e5908fac0e73885ce39efe82bcd61 $ Asa
 *
 * Description: 
 *         This file contains all the media format recognizer public APIs  
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
#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif
#include "mutil/mfmtrecg/u_mfmtrecg.h"
#include "mutil/mfmtrecg/mfmtrecg.h"
#include "mutil/mfmtrecg/mfmtrecg_cli.h"
#include "mutil/mm_util.h"
#include "dbg/x_dbg.h"
/*#include "libc/ctype.h"*/
#include "util/x_lnk_list.h"

#else

#include "x_os.h"
#include "x_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "u_mfmtrecg.h"
#include "mfmtrecg.h"
#include "mfmtrecg_cli.h"
#include "x_dbg.h"
#include "x_lnk_list.h"
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
SLIST_T(_MFMTRECG_HANDLER_REG_T) gt_mfmtrecg_handler_reg_list = {NULL};
static BOOL     gb_init = FALSE;


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
#ifdef __KERNEL__
CHAR* x_str_toupper (CHAR*  ps_str)
{
    if (ps_str != NULL)
    {
        CHAR*  ps_cursor;
        CHAR   c_char;
        
        ps_cursor = ps_str;
        
        while ((c_char = *ps_cursor) != '\0')
        {
            if ((c_char >= 'a') && (c_char <= 'z'))
            {
                *ps_cursor = c_char - 'a' + 'A';
            }
            
            ps_cursor++;
        }
    }
    
    return ps_str;
}
#endif


INT32 _mfmtrecg_fmt_to_ext(MEDIA_FORMAT_T* pt_format,
                                CHAR*           ps_extension, 
                                UINT32          ui4_len)
{
    BOOL b_find;
    
    if ((NULL == pt_format) 
        || (NULL == ps_extension) 
        || (ui4_len < MAX_EXTENSION_NAME_LEN))
    {
        return MFMTRECGR_INV_ARG;
    }

    x_memset(ps_extension, 0, ui4_len);
    b_find = TRUE;
    
    switch (pt_format->e_media_type)
    {
        case MEDIA_TYPE_CONTAINER:
            switch (pt_format->t_media_subtype.u.e_contnr_subtype)
            {
                case MEDIA_CONTNR_SUBTYPE_ASF:
                    x_strncpy(ps_extension, "ASF", 4);
                    break;
                case MEDIA_CONTNR_SUBTYPE_AVI:
                    x_strncpy(ps_extension, "AVI", 4);
                    break;
                case MEDIA_CONTNR_SUBTYPE_MP4:
                    x_strncpy(ps_extension, "MP4", 4);
                    break;
                case MEDIA_CONTNR_SUBTYPE_MATROSKA:
                    x_strncpy(ps_extension, "MKV", 4);
                    break;
                case MEDIA_CONTNR_SUBTYPE_RM:
                    x_strncpy(ps_extension, "RM", 3);
                    break;
                case MEDIA_CONTNR_SUBTYPE_UNKNOWN:
                default:
                    b_find = FALSE;
            }
            break;
        case MEDIA_TYPE_PS:
            x_strncpy(ps_extension, "DAT", 4);
            break;
        case MEDIA_TYPE_AUDIO_ES:
            switch (pt_format->t_media_subtype.u.e_aud_subtype)
            {
                case MEDIA_AUD_SUBTYPE_MP2:
                case MEDIA_AUD_SUBTYPE_MP3:
                    x_strncpy(ps_extension, "MP3", 4);
                    break;
                case MEDIA_AUD_SUBTYPE_WMA:
                    x_strncpy(ps_extension, "WMA", 4);
                    break;
                case MEDIA_AUD_SUBTYPE_LPCM:
                    x_strncpy(ps_extension, "PCM", 4);
                    break;
                case MEDIA_AUD_SUBTYPE_AAC:
                    x_strncpy(ps_extension, "AAC", 4);
                    break;
                case MEDIA_AUD_SUBTYPE_AMR:
                    x_strncpy(ps_extension, "AMR", 4);
                    break;
                case MEDIA_AUD_SUBTYPE_AWB:
                    x_strncpy(ps_extension, "AWB", 4);
                    break;
                case MEDIA_AUD_SUBTYPE_WAVE:
                    x_strncpy(ps_extension, "WAV", 4);
                    break;
                case MEDIA_AUD_SUBTYPE_AIF:
                    x_strncpy(ps_extension, "AIF", 4);
                    break;
                case MEDIA_AUD_SUBTYPE_AC3:
                    x_strncpy(ps_extension, "AC3", 4);
                    break;
                case MEDIA_AUD_SUBTYPE_UNKNOWN:
                default:
                    b_find = FALSE;
            }
            break;
        case MEDIA_TYPE_VIDEO_ES:
        case MEDIA_TYPE_IMAGE:
            b_find = FALSE;
            break;
        case MEDIA_TYPE_TS:
            x_strncpy(ps_extension, "TS", 3);
            break;
        case MEDIA_TYPE_UNKNOWN:
        default:
            b_find = FALSE;
    }
    
    if (TRUE == b_find)
    {
        return MFMTRECGR_OK;
    }
    else
    {
        return MFMTRECGR_INTERNAL_ERR;
    }
    
}
/*-----------------------------------------------------------------------------
 * Name:  mfmtrecg_ask_handler_in_favor_by_file
 *
 * Description: this function will ask favor handler which match extension and favor
 *
 * Inputs:  h_file         specify the handle of the file
 *          ps_extension   specify extension of the file
 *
 * Outputs: pt_format      retrieve the format of the file
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 mfmtrecg_ask_handler_in_favor_by_file(
                            HANDLE_T                    h_file,        
                            const CHAR*                 ps_extension,  
                            MEDIA_FORMAT_T*             pt_format)     
{
#ifndef __NO_FM__
    MFMTRECG_HANDLER_REG_T*     pt_reg; 
    handler_file_confirm_fct    pf_fconfirm = NULL;
    MFMREC_FILE_EXT_T* pt_file_ext;
    INT32 i4_ret;
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( (h_file == NULL_HANDLE) 
        || (ps_extension == NULL) 
        || (pt_format == NULL) )
    {
        return MFMTRECGR_INV_ARG;
    }    
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /* check engines */
    SLIST_FOR_EACH(pt_reg, &gt_mfmtrecg_handler_reg_list, t_link)
    {
        SLIST_FOR_EACH(pt_file_ext, &(pt_reg->t_favor_list), t_file_ext_list)
        {
           /*if (x_strstr(pt_file_ext->ps_file_ext, x_str_toupper((CHAR*)ps_extension)))*/
   
           if (!x_strncmp(pt_file_ext->ps_file_ext, 
                x_str_toupper((CHAR*)ps_extension),
                x_strlen(pt_file_ext->ps_file_ext)))
           {
               pf_fconfirm = pt_reg->t_handler_fct_tbl.pf_fconfirm;

               if (pf_fconfirm)
               {
                 i4_ret = pf_fconfirm(h_file, pt_format);
                 if (i4_ret == MFMTRECGR_OK)
                 {
                    return i4_ret;
                 }
                 else if(i4_ret == MFMTRECGR_FILE_ERR)
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
                             return MFMTRECGR_FILE_ERR;
                         }
                     }
                     else
                     {
                         return MFMTRECGR_FILE_ERR;
                     }
                     i4_ret = x_fm_lseek(h_file, 0, FM_SEEK_BGN, &ui8_cur_pos);
                 }
              }
           }          
        }
    }
#endif
    return MFMTRECGR_HANDLER_NOT_FOUND;

}
                            
/*-----------------------------------------------------------------------------
 * Name:  mfmtrecg_ask_all_handler_by_file
 *
 * Description: this function will ask all handler
 *
 * Inputs:  h_file         specify the handle of the file
 *          ps_extension   specify extension of the file
 *
 * Outputs: pt_format      retrieve the format of the file
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 mfmtrecg_ask_all_handler_by_file(
                            HANDLE_T                    h_file,        
                            const CHAR*                 ps_extension,  
                            MEDIA_FORMAT_T*             pt_format)     
{
#ifndef __NO_FM__
    MFMTRECG_HANDLER_REG_T*     pt_reg; 
    handler_file_confirm_fct    pf_fconfirm = NULL;
    INT32 i4_ret;
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( (h_file == NULL_HANDLE) 
        || (ps_extension == NULL) 
        || (pt_format == NULL) )
    {
        return MFMTRECGR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /* check engines */
    SLIST_FOR_EACH(pt_reg, &gt_mfmtrecg_handler_reg_list, t_link)
    {
        pf_fconfirm = pt_reg->t_handler_fct_tbl.pf_fconfirm;
        if (pf_fconfirm)
        {
            i4_ret = pf_fconfirm(h_file, pt_format);
            if (i4_ret == MFMTRECGR_OK)
            {
                return i4_ret;
            }
            else if(i4_ret == MFMTRECGR_FILE_ERR)
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
                        return MFMTRECGR_FILE_ERR;
                    }
                }
                else
                {
                    return MFMTRECGR_FILE_ERR;
                }
                i4_ret = x_fm_lseek(h_file, 0, FM_SEEK_BGN, &ui8_cur_pos);
            }
        }
    }
#endif
    return MFMTRECGR_HANDLER_NOT_FOUND;
}

/*-----------------------------------------------------------------------------
 * Name:  mfmtrecg_ask_handler_in_favor_by_mem_with_fmt
 *
 * Description: this function will ask favor handler which match given format
 *
 * Inputs:  pv_mem_data     specify the address of memory buffer which want to retrieve the format.
 *            z_mem_leng        specify the length of memory buffer which want to retrieve the format.
 *
 * Outputs: pt_format       retrieve the format of the media memory buffer.
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/

INT32 mfmtrecg_ask_handler_in_favor_by_mem_with_fmt(
                        const VOID*       pv_mem_data, 
                        SIZE_T            z_mem_leng,   
                        MEDIA_FORMAT_T*   pt_format) 

{
    CHAR                       ac_extension[MAX_EXTENSION_NAME_LEN]; 
    INT32                      i4_ret;
    MFMTRECG_HANDLER_REG_T*    pt_reg; 
    handler_mem_confirm_fct    pf_mconfirm = NULL;
    MFMREC_FILE_EXT_T*         pt_file_ext;
    MEDIA_FEEDER_T*            pt_src_feeder;

    i4_ret = _mfmtrecg_fmt_to_ext(pt_format, 
                                  ac_extension, 
                                  MAX_EXTENSION_NAME_LEN);
    pt_src_feeder = (MEDIA_FEEDER_T*)pv_mem_data;
    
    if (MFMTRECGR_OK == i4_ret)
    {

        /* check engines */
        SLIST_FOR_EACH(pt_reg, &gt_mfmtrecg_handler_reg_list, t_link)
        {
            SLIST_FOR_EACH(pt_file_ext, &(pt_reg->t_favor_list), t_file_ext_list)
            {
                /*if (x_strstr(pt_file_ext->ps_file_ext, x_str_toupper((CHAR*)ac_extension)))*/
                if(pt_src_feeder->b_canceled)
                {
#ifndef __KERNEL__
                    x_dbg_stmt("!!!mfmtrecg_ask_handler_in_favor_by_mem_with_fmt: canceled = true \n");
#endif
                    return MFMTRECGR_FILE_ERR;
                }
                if (!x_strncmp(pt_file_ext->ps_file_ext,
                               x_str_toupper((CHAR*)ac_extension), 
                               x_strlen(pt_file_ext->ps_file_ext)))
                {
                    pf_mconfirm = pt_reg->t_handler_fct_tbl.pf_mconfirm;

                    if (pf_mconfirm != NULL)
                    {
                        i4_ret = pf_mconfirm(pv_mem_data, z_mem_leng, pt_format);
                        if(i4_ret == MFMTRECGR_OK)
                        {
                            return i4_ret;
                        }
                        else
                        {
                            UINT32          ui4_box_type;
                            /*try to read begin 4 bytes*/
                            i4_ret = pt_src_feeder->t_feeder_be_fct_tbl.pf_set_pos(
                                                    &pt_src_feeder->h_feeder,
                                                    0,
                                                    MEDIA_SEEK_BGN);
                            if (i4_ret != MFMTRECGR_OK)
                            {
                                return MFMTRECGR_FILE_ERR;
                            }
                            i4_ret = pt_src_feeder->t_feeder_be_fct_tbl.pf_input4(
                                                            &pt_src_feeder->h_feeder, 
                                                            &ui4_box_type);
                            if (i4_ret != MFMTRECGR_OK)
                            {
                                return MFMTRECGR_FILE_ERR;
                            }
                            /*restore*/
                            i4_ret = pt_src_feeder->t_feeder_be_fct_tbl.pf_set_pos(
                                                    &pt_src_feeder->h_feeder,
                                                    0,
                                                    MEDIA_SEEK_BGN);
                        }
                    }
                }          
            }
        }
        return MFMTRECGR_INTERNAL_ERR;

    }
    else
    {
        return MFMTRECGR_INTERNAL_ERR;
    }

}

/*-----------------------------------------------------------------------------
 * Name:  mfmtrecg_ask_all_handler_by_mem
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
INT32 mfmtrecg_ask_all_handler_by_mem(
                        const VOID*       pv_mem_data, 
                        SIZE_T            z_mem_leng,   
                        MEDIA_FORMAT_T*   pt_format) 
{
    INT32                      i4_ret   = 0;
    MFMTRECG_HANDLER_REG_T*    pt_reg; 
    handler_mem_confirm_fct    pf_mconfirm = NULL;
    MEDIA_FEEDER_T*            pt_src_feeder;
                
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
    pt_src_feeder = (MEDIA_FEEDER_T*)pv_mem_data;
    /* check engines */
    SLIST_FOR_EACH(pt_reg, &gt_mfmtrecg_handler_reg_list, t_link)
    {
        if(pt_src_feeder->b_canceled)
        {
#ifndef __KERNEL__
            x_dbg_stmt("!!!mfmtrecg_ask_all_handler_by_mem: canceled = true \n");
#endif            
            return MFMTRECGR_FILE_ERR;
        }
        pf_mconfirm = pt_reg->t_handler_fct_tbl.pf_mconfirm;
        if (pf_mconfirm)
        {
            i4_ret = pf_mconfirm(pv_mem_data, z_mem_leng, pt_format);
            if (i4_ret == MFMTRECGR_OK)
            {
                return i4_ret;
            }   
            else
            {
                UINT32          ui4_box_type;
                /*try to read begin 4 bytes*/
                i4_ret = pt_src_feeder->t_feeder_be_fct_tbl.pf_set_pos(
                                        &pt_src_feeder->h_feeder,
                                        0,
                                        MEDIA_SEEK_BGN);
                if (i4_ret != MFMTRECGR_OK)
                {
                    return MFMTRECGR_FILE_ERR;
                }
                i4_ret = pt_src_feeder->t_feeder_be_fct_tbl.pf_input4(
                                                &pt_src_feeder->h_feeder, 
                                                &ui4_box_type);
                if (i4_ret != MFMTRECGR_OK)
                {
                    return MFMTRECGR_FILE_ERR;
                }
                /*restore*/
                i4_ret = pt_src_feeder->t_feeder_be_fct_tbl.pf_set_pos(
                                        &pt_src_feeder->h_feeder,
                                        0,
                                        MEDIA_SEEK_BGN);
            }
        }
    }
    return MFMTRECGR_HANDLER_NOT_FOUND;

}
/*-----------------------------------------------------------------------------
 * Name:  mfmtrecg_register_handler
 *
 * Description: This API register the handler.
 *
 * Inputs:  ps_name    specifies sixteen characters of a handler name, and the character is case sensitive.
 *          ps_favor        specify the favor file extension string list, for example "mp3", the list must end with FILE_EXTENSION_END
 *          pt_fct_tbl       specify the function table of the engine.
 *
 * Outputs: NONE.
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/

INT32 mfmtrecg_register_handler (  
                            const CHAR*                             ps_name,    
                            const CHAR**                            ps_favor,
                            const MFMTRECG_HANDLER_FCT_TBL_T*       pt_fct_tbl)
{
    MFMTRECG_HANDLER_REG_T* pt_handler_reg = NULL;
    
    UINT8                   ui1_i = 0;
    CHAR*                   ps_ext;
    MFMREC_FILE_EXT_T*      pt_node_favor;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( (ps_name == NULL) 
        || (ps_favor == NULL) 
        || (pt_fct_tbl == NULL) )
    {
        return MFMTRECGR_INV_ARG;
    }
    
    /************************************************************************/
    /* Perform register handler                                              */
    /************************************************************************/

    /* check if the engine is exist?*/
    SLIST_FOR_EACH(pt_handler_reg, &gt_mfmtrecg_handler_reg_list, t_link)
    {
        if (x_strncmp(pt_handler_reg->s_handler_name, ps_name, MAX_HANDLER_NAME_LEN) == 0)
        {
            return MFMTRECGR_NAME_EXIST;
        }
    }

    /* malloc node */
    pt_handler_reg = (MFMTRECG_HANDLER_REG_T*) _mm_util_mem_alloc(sizeof(MFMTRECG_HANDLER_REG_T));
    if (pt_handler_reg == NULL)
    {
        return MFMTRECGR_OUT_OF_MEMORY;
    }
    x_memset(pt_handler_reg, 0, sizeof(MFMTRECG_HANDLER_REG_T));

    /*extension*/
    while(x_strncmp(ps_favor[ui1_i] ,FILE_EXTENSION_END_CODE,MAX_EXTENSION_NAME_LEN)!=0)
    {
        pt_node_favor = (MFMREC_FILE_EXT_T*) _mm_util_mem_alloc(sizeof(MFMREC_FILE_EXT_T));
        if(pt_node_favor == NULL)
        {
            return MFMTRECGR_OUT_OF_MEMORY;
        }
        x_memset(pt_node_favor, 0, sizeof(MFMREC_FILE_EXT_T));
        ps_ext = (CHAR*) _mm_util_mem_alloc(x_strlen(ps_favor[ui1_i]) + 1/*NULL terminator*/); 
        if(ps_ext  == NULL)
        {
            return MFMTRECGR_OUT_OF_MEMORY;
        }
        x_memset(ps_ext, 0, (x_strlen(ps_favor[ui1_i]) + 1));
        pt_node_favor->ps_file_ext = ps_ext;
        x_strncpy(pt_node_favor->ps_file_ext, ps_favor[ui1_i], x_strlen(ps_favor[ui1_i]));
        x_str_toupper(pt_node_favor->ps_file_ext);
        SLIST_INSERT_HEAD(pt_node_favor,&(pt_handler_reg->t_favor_list), t_file_ext_list);
        ui1_i++;
    }
    
    /*name*/
    x_strncpy(pt_handler_reg->s_handler_name, ps_name, MAX_HANDLER_NAME_LEN);
    
    pt_handler_reg->t_handler_fct_tbl.pf_fconfirm = pt_fct_tbl->pf_fconfirm;
    pt_handler_reg->t_handler_fct_tbl.pf_mconfirm = pt_fct_tbl->pf_mconfirm;
    pt_handler_reg->t_handler_fct_tbl.pf_mfmtrecg_cli_init
                                            = pt_fct_tbl->pf_mfmtrecg_cli_init;
    
    /*add new node: 
    */
    SLIST_INSERT_HEAD(pt_handler_reg, &gt_mfmtrecg_handler_reg_list, t_link);

    return MFMTRECGR_OK;
}
 /*-----------------------------------------------------------------------------
 * Name:  mfmtrecg_init
 *
 * Description: This function is a selector's API. This function is called by system initial procedure when system boot-up to init media format recognizer module.
 *
 * Inputs:  NONe.
 * Outputs: NONE.
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 mfmtrecg_init (void)
{ /*it could be called multiple times when mp3 aac ... recognizer inital*/

    if (gb_init == TRUE)
    {
        return MFMTRECGR_OK;
    }
    
    gb_init = TRUE;
    return MFMTRECGR_OK;
}




