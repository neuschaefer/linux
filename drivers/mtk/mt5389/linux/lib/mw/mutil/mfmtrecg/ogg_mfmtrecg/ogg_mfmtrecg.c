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
 * ogg_mfmtrecg.c,v
 * 1.1.2.1 1.0
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
#include "mutil/mfmtrecg/u_mfmtrecg.h"
#include "dbg/x_dbg.h"
#include "dbg/u_dbg.h"
#include "mutil/mm_util.h"

#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif

#include "mutil/mm_util_input.h"
#include "util/x_lnk_list.h"
#include "inc/x_mm_common.h"

#else

#include "x_os.h"
#include "x_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "../u_mfmtrecg.h"
#include "x_dbg.h"
#include "u_dbg.h"

#include "../../mm_util.h"
#include "../../mm_util_input.h"

#include "x_lnk_list.h"
#include "x_mm_common.h"

#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define dwBigFourCC(a,b,c,d)  ((((UINT32)a)<<24)|((b)<<16)|((c)<<8)|(d))

#define OGG_HEADER_ID         dwBigFourCC('O', 'g', 'g', 'S')

#define OGG_MAX_HEAD_LEN                300
#define OGG_PAGE_SEGMENT_MAX			0x100

/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
typedef struct _OGG_MFMTRECG_HEADER_INFO_T
{
	UINT8*		ui1_buf;
	UINT8		ui1_header_type;
	UINT8		ui1_page_segment;
	UINT8		segment_table[OGG_PAGE_SEGMENT_MAX];
}OGG_MFMTRECG_HEADER_INFO_T;

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name:  ogg_has_video_confirm
 *
 * Description: this function will ask favor handler which match extension and favor
 *
 * Inputs:  h_file         specify the handle of the file
 *
 * Returns: 	TRUE    	Has Video.
 *			FALSE	Has no Video.
 *      The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
BOOL ogg_has_video_confirm(HANDLE_T	h_file)     
{
	INT32								i4_ret = 0;
	UINT64								ui8_cur_pos = 0;
	UINT64								ui8_offset = 0;
	UINT32								ui4_page_cnt = 0;
	UINT32  							ui4_box_type = 0;
	UINT32          					ui4_data = 0;
    UINT32          					ui4_read = 0;
	OGG_MFMTRECG_HEADER_INFO_T			ogg_mfmtrecg_header_info;
	UINT32								ui4_cnt = 0;
	const CHAR              			i1_video_header[7] = {0x01,'v','i','d','e','o','\0'};

	x_memset(&ogg_mfmtrecg_header_info, 0, sizeof(OGG_MFMTRECG_HEADER_INFO_T));
	ogg_mfmtrecg_header_info.ui1_buf = x_mem_alloc(OGG_MAX_HEAD_LEN + 1);
	if (ogg_mfmtrecg_header_info.ui1_buf == NULL)
	{
		return TRUE;
	}
	
	while (1)
	{
		i4_ret = x_fm_lseek(h_file, ui8_cur_pos, FM_SEEK_BGN, &ui8_offset);
	    if(i4_ret != FMR_OK)
	    {
	        break;
	    }

		i4_ret = x_fm_read(h_file, (VOID*)&ui4_data, sizeof(ui4_data), &ui4_read);
	    if(FMR_OK != i4_ret)
	    {
	        break;
	    }
		if(ui4_read < sizeof(ui4_data))
	    {
	    	i4_ret = MFMTRECGR_FILE_ERR;
			break;
	    }
	    ui4_box_type = ((ui4_data&0xff000000)>>24) 
	                	| ((ui4_data&0xff0000)>>8) 
	                	| ((ui4_data&0xff00)<<8) 
	                	| ((ui4_data&0xff)<<24);
	    if(ui4_box_type == OGG_HEADER_ID)
	    {
	    	x_fm_lseek(h_file, ui8_cur_pos, FM_SEEK_BGN, &ui8_offset);
			i4_ret = x_fm_read(h_file, (VOID*)ogg_mfmtrecg_header_info.ui1_buf, OGG_MAX_HEAD_LEN, &ui4_read);
			if (i4_ret != FMR_OK)
			{
				i4_ret = MFMTRECGR_FILE_ERR;
				break;
			}
			LOAD_BYTE(ogg_mfmtrecg_header_info.ui1_buf + 5,  ogg_mfmtrecg_header_info.ui1_header_type);
			LOAD_BYTE(ogg_mfmtrecg_header_info.ui1_buf + 26, ogg_mfmtrecg_header_info.ui1_page_segment);
			ui8_cur_pos += 27;
			
			if(ogg_mfmtrecg_header_info.ui1_page_segment <= 255 && ogg_mfmtrecg_header_info.ui1_page_segment > 0)
            {
                for(ui4_cnt=0; ui4_cnt < ogg_mfmtrecg_header_info.ui1_page_segment; ui4_cnt++)
                {
                    LOAD_BYTE(ogg_mfmtrecg_header_info.ui1_buf + 27 + ui4_cnt, ogg_mfmtrecg_header_info.segment_table[ui4_cnt]);
                    ui8_cur_pos ++;
                }
            }

			if ((ogg_mfmtrecg_header_info.ui1_header_type & 0x02) >> 1)
			{
				x_fm_lseek(h_file, ui8_cur_pos, FM_SEEK_BGN, &ui8_offset);
				i4_ret = x_fm_read(h_file, (VOID*)ogg_mfmtrecg_header_info.ui1_buf, 9, &ui4_read);
				if (i4_ret != FMR_OK)
				{
					i4_ret = MFMTRECGR_FILE_ERR;
					break;
				}

				if (!x_memcmp(ogg_mfmtrecg_header_info.ui1_buf, i1_video_header, 6))
				{
					i4_ret = MFMTRECGR_OK;
					break;
				}
			}

			for (ui4_cnt = 0; ui4_cnt < ogg_mfmtrecg_header_info.ui1_page_segment; ui4_cnt++)
            {
                ui8_cur_pos += ogg_mfmtrecg_header_info.segment_table[ui4_cnt];
            }
	    }
		
		ui4_page_cnt ++;
		if (ui4_page_cnt > 200)
		{
			if (ogg_mfmtrecg_header_info.ui1_buf != NULL)
			{
				x_mem_free(ogg_mfmtrecg_header_info.ui1_buf);
			}
			return FALSE;
		}
	}
	
	if (ogg_mfmtrecg_header_info.ui1_buf != NULL)
	{
		x_mem_free(ogg_mfmtrecg_header_info.ui1_buf);
	}
	if (i4_ret == MFMTRECGR_OK)
	{
		return TRUE;
	}
	
	return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name:  ogg_handler_file_confirm_fct
 *
 * Description: this function will ask favor handler which match extension and favor
 *
 * Inputs:  h_file         specify the handle of the file
 *
 * Outputs: pt_format      retrieve the format of the file
 *
 * Returns: MFMTRECGR_OK    Success. 
 *      The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 ogg_handler_file_confirm_fct(
                            HANDLE_T            h_file,        
                            MEDIA_FORMAT_T*     pt_format)     
{

#ifndef __NO_FM__

    INT32   		i4_ret = 0;
    UINT32  		ui4_box_type = 0;
    UINT64   		ui8_cur_pos = 0;
    UINT32          ui4_data = 0;
    UINT32          ui4_read = 0;
	
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pt_format == NULL) || (h_file == NULL_HANDLE))
    {
        return MFMTRECGR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    i4_ret = x_fm_lseek(h_file, 0, FM_SEEK_BGN, &ui8_cur_pos);
    if(i4_ret != FMR_OK)
    {
        return MFMTRECGR_FILE_ERR;
    }
    
    i4_ret = x_fm_read(h_file, (VOID*)&ui4_data, sizeof(ui4_data), &ui4_read);
    if(FMR_OK != i4_ret)
    {
        return MFMTRECGR_FILE_ERR;
    }
    if(ui4_read < sizeof(ui4_data))
    {
		return MFMTRECGR_INTERNAL_ERR;
    }
    ui4_box_type = ((ui4_data&0xff000000)>>24) 
                	| ((ui4_data&0xff0000)>>8) 
                	| ((ui4_data&0xff00)<<8) 
                	| ((ui4_data&0xff)<<24);
    if(ui4_box_type == OGG_HEADER_ID)
    {
    	if (!(ogg_has_video_confirm(h_file)))
		{
			pt_format->e_mm_svc_type = MEDIA_SVC_TYPE_AUDIO_ONLY;
		}
        pt_format->e_media_type = MEDIA_TYPE_CONTAINER;
        pt_format->t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_OGG;
        /*DBG_INFO(("INFO: File recognizer is OGG file \r\n")); */
        return MFMTRECGR_OK;
    }
    else
    {
    	return MFMTRECGR_INTERNAL_ERR;
    }

#else

    return MFMTRECGR_INTERNAL_ERR;

#endif
}
                            
/*-----------------------------------------------------------------------------
 * Name:  ogg_handler_mem_confirm_fct
 *
 * Description: this function will ask all handler
 *
 * Inputs: pv_mem_data specify the address of memory buffer which want to retrieve the format.
 *          z_mem_leng      specify the length of memory buffer which want to retrieve the format.
 *
 * Outputs: pt_format       retrieve the format of the media memory buffer.
 *
 * Returns: MFMTRECGR_OK    Success. 
 * The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 ogg_handler_mem_confirm_fct(const VOID*       pv_mem_data, 
                                  		SIZE_T            z_mem_leng,   
                                  		MEDIA_FORMAT_T*   pt_format) 
{
    INT32           i4_ret;
    UINT32          ui4_box_type;
    UINT64          ui8_buff_pos = 0;
    MEDIA_FEEDER_T* pt_src_feeder;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_mem_data == NULL) || (pt_format == NULL))
    {
        return MFMTRECGR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    if(z_mem_leng == 0)
    {
        pt_src_feeder = (MEDIA_FEEDER_T*)pv_mem_data;
        i4_ret = pt_src_feeder->t_feeder_be_fct_tbl.pf_set_pos(
                                &pt_src_feeder->h_feeder,
                                0,
                                MEDIA_SEEK_BGN);
        if (i4_ret != INPUTR_OK)
        {
            return MFMTRECGR_INTERNAL_ERR;
        }
        i4_ret = pt_src_feeder->t_feeder_be_fct_tbl.pf_input4(
                                        &pt_src_feeder->h_feeder, 
                                        &ui4_box_type);
        if (i4_ret != INPUTR_OK)
        {
            return MFMTRECGR_INTERNAL_ERR;
        }
    }
    else
    {
		ui4_box_type = (UINT32)(((UINT8*)pv_mem_data)[ui8_buff_pos] << 24) 
	            			| (UINT32)(((UINT8*)pv_mem_data)[ui8_buff_pos + 1] << 16) 
	            			| (UINT32)(((UINT8*)pv_mem_data)[ui8_buff_pos + 2] << 8) 
	            			| (UINT32)(((UINT8*)pv_mem_data)[ui8_buff_pos + 3]);
		ui8_buff_pos += 4;
    }
    
    if(ui4_box_type == OGG_HEADER_ID)
    {
        pt_format->e_media_type = MEDIA_TYPE_CONTAINER;
        pt_format->t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_OGG;
        /*DBG_INFO(("INFO: File recognizer is OGG file \r\n")); */
        return MFMTRECGR_OK;
    }
    else
    {
    	return MFMTRECGR_INTERNAL_ERR;
    }
}
