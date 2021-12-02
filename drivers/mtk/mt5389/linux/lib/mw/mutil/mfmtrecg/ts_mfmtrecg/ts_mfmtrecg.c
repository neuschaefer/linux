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
 * $RCSfile: ts_mfmtrecg.c,v $
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
#include "mutil/mfmtrecg/u_mfmtrecg.h"
#include "dbg/x_dbg.h"
#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif
#include "util/x_lnk_list.h"
#include "inc/x_mm_common.h"
#include "mutil/mm_util_memory.h"
#include "mutil/mm_util_input.h"

#else

#include "x_os.h"
#include "x_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "../u_mfmtrecg.h"
#include "x_dbg.h"
#include "x_lnk_list.h"
#include "x_mm_common.h"
#include "../../mm_util_memory.h"
#include "../../mm_util.h"
#include "../../mm_util_input.h"

#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

#define TS_LOOKUP_LENGTH    2048
#define TS_LOOKUP_STEPS     3
#define TS_PACKET_END_CODE  0
#define TS_SYNC_BYTE        0x47

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static UINT8 aui1_ts_pkt_size[] = {
                                    188,
                                    192,
                                    TS_PACKET_END_CODE
                                  };

static UINT8 aui1_ts_pkt_time[] = {
                                    192,
                                    TS_PACKET_END_CODE
                                  };

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

static UINT8 _ts_get_max_packet_size(VOID);
static BOOL _ts_check_time(UINT8 ui1_pkt_len);

static BOOL _ts_check_format(UINT8*     pui1_buf, 
                             SIZE_T     z_mem_leng, 
                             UINT8*     pui1_pkt_len, 
                             BOOL*      pb_time,
                             INT32*     pi4_status);

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/
static UINT8 _ts_get_max_packet_size(VOID)
{
    UINT8   ui1_idx;
    UINT8   ui1_max;

    ui1_max=0;
    for (ui1_idx=0;ui1_idx<(UINT8)(sizeof(aui1_ts_pkt_size)/sizeof(UINT8));ui1_idx++)
    {
        if (TS_PACKET_END_CODE == aui1_ts_pkt_size[ui1_idx])
        {
            break;
        }
        
        if (aui1_ts_pkt_size[ui1_idx] > ui1_max)
        {
            ui1_max = aui1_ts_pkt_size[ui1_idx];
        }
    }

    return ui1_max;
}

static BOOL _ts_check_time(UINT8 ui1_pkt_len)
{
    UINT8   ui1_idx;
    BOOL    b_found;

    b_found=FALSE;
    for (ui1_idx=0;ui1_idx<(UINT8)(sizeof(aui1_ts_pkt_time)/sizeof(UINT8));ui1_idx++)
    {
        if (TS_PACKET_END_CODE == aui1_ts_pkt_time[ui1_idx])
        {
            break;
        }
        
        if (aui1_ts_pkt_time[ui1_idx] == ui1_pkt_len)
        {
            b_found = TRUE;
            break;
        }
    }

    return b_found;
}

static BOOL _ts_check_format(UINT8*     pui1_buf, 
                             SIZE_T     z_mem_leng, 
                             UINT8*     pui1_pkt_len, 
                             BOOL*      pb_time,
                             INT32*     pi4_status)
{
    BOOL                b_check_time;
    BOOL                b_found;
    UINT8               ui1_idx;
    UINT8               ui1_step;
    UINT8               ui1_tmp;
    UINT32              ui4_pos;
    UINT32              ui4_time_total;
    MEDIA_FEEDER_T*     pt_src_feeder;
    UINT64              ui8_mem_pos;
    UINT32              ui4_time;
    INT32               i4_ret = 0;

    ui4_time_total = 0;
    ui8_mem_pos = 0;
    pt_src_feeder = NULL;
    b_found=FALSE;
    if(pi4_status)
    {
        *pi4_status = 0;
    }
    if (0 == z_mem_leng)
    {
        pt_src_feeder = (MEDIA_FEEDER_T *)pui1_buf;
        pt_src_feeder->t_feeder_be_fct_tbl.pf_get_pos(&pt_src_feeder->h_feeder, 
                                                      &ui8_mem_pos);
    }

    
    for (ui1_idx=0;ui1_idx<(UINT8)(sizeof(aui1_ts_pkt_size)/sizeof(UINT8));ui1_idx++)
    {
        if (TS_PACKET_END_CODE == aui1_ts_pkt_size[ui1_idx])
        {
            break;
        }

        b_check_time = _ts_check_time(aui1_ts_pkt_size[ui1_idx]);
        b_found = TRUE;
        for (ui1_step=0; ui1_step<TS_LOOKUP_STEPS; ui1_step++)
        {
            ui4_pos = (UINT32)ui1_step*(UINT32)aui1_ts_pkt_size[ui1_idx];
            if (b_check_time)
            {
                if (0 == z_mem_leng)
                {
                    pt_src_feeder->t_feeder_be_fct_tbl.pf_set_pos(&pt_src_feeder->h_feeder,
                                                                  (INT64)(ui8_mem_pos+ui4_pos),
                                                                  MEDIA_SEEK_BGN);

                    i4_ret = pt_src_feeder->t_feeder_be_fct_tbl.pf_input4(&pt_src_feeder->h_feeder, &ui4_time);
                    ui4_time_total += ui4_time; 

                    pt_src_feeder->t_feeder_be_fct_tbl.pf_set_pos(&pt_src_feeder->h_feeder,
                                                                  (INT64)(ui8_mem_pos+ui4_pos+4),
                                                                  MEDIA_SEEK_BGN);
                    
                    i4_ret = pt_src_feeder->t_feeder_be_fct_tbl.pf_input1(&pt_src_feeder->h_feeder, 
                                                                 &ui1_tmp);

                    if(i4_ret != INPUTR_OK)
                    {
                        if(pi4_status)
                        {
                            *pi4_status = i4_ret;                           
                        }
                        b_found = ui1_step > TS_LOOKUP_STEPS/2 ? TRUE:FALSE;
                        break;
                    }
                    if (ui1_tmp!=TS_SYNC_BYTE)
                    {
                        b_found = FALSE;
                        break;
                    }
                }
                else
                {
                    ui4_time_total = pui1_buf[ui4_pos];
                    ui4_time_total = ((ui4_time_total<<8)|pui1_buf[ui4_pos+1]);
                    ui4_time_total = ((ui4_time_total<<8)|pui1_buf[ui4_pos+2]);
                    ui4_time_total = ((ui4_time_total<<8)|pui1_buf[ui4_pos+3]);
                    if (pui1_buf[ui4_pos+4]!=TS_SYNC_BYTE)
                    {
                        b_found = FALSE;
                        break;
                    }
                }
            }
            else
            {
                if (0 == z_mem_leng)
                {
                    pt_src_feeder->t_feeder_be_fct_tbl.pf_set_pos(&pt_src_feeder->h_feeder,
                                                                  (INT64)(ui8_mem_pos+ui4_pos),
                                                                  MEDIA_SEEK_BGN);
                    
                    i4_ret = pt_src_feeder->t_feeder_be_fct_tbl.pf_input1(&pt_src_feeder->h_feeder, 
                                                                 &ui1_tmp);
                    if(i4_ret != INPUTR_OK)
                    {
                        if(pi4_status)
                        {
                            *pi4_status = i4_ret;                           
                        }
                        b_found = ui1_step > TS_LOOKUP_STEPS/2 ? TRUE:FALSE;
                        break;
                    }
                    if (ui1_tmp != TS_SYNC_BYTE)
                    {
                        b_found = FALSE;
                        break;
                    }

                }
                else
                {
                    if (pui1_buf[ui4_pos]!=TS_SYNC_BYTE)
                    {
                        b_found = FALSE;
                        break;
                    }

                }
            }
        }
        
        if (b_found)
        {
            if (b_check_time)
            {
                if (ui4_time_total)
                {
                    *pb_time = TRUE;
                }
                else
                {
                    *pb_time = FALSE;
                }
            }
            else
            {
                *pb_time = FALSE;
            }

            *pui1_pkt_len = aui1_ts_pkt_size[ui1_idx];
            break;
        }
    }
    return b_found;
}



/*-----------------------------------------------------------------------------
 * Name:  ts_handler_file_confirm_fct
 *
 * Description: this function will ask favor handler which match extension and favor
 *
 * Inputs:  h_file         specify the handle of the file
 *
 * Outputs: pt_format      retrieve the format of the file
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 ts_handler_file_confirm_fct(HANDLE_T              h_file,        
                                  MEDIA_FORMAT_T*       pt_format)     
{

#ifndef __NO_FM__

    UINT64  ui8_cur_pos;
    INT32   i4_ret;
    UINT32  ui4_read;
    UINT8*  pui1_buf;
    UINT32  ui4_buf_size;
    UINT32  ui4_check_size;
    UINT16  ui2_idx;
    BOOL    b_found;
    UINT8   ui1_pkt_len;
    BOOL    b_time;
    BOOL    b_system_B = FALSE;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pt_format == NULL)
        || (h_file == NULL_HANDLE))
    {
        return MFMTRECGR_INTERNAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pui1_buf = NULL;
    b_found = FALSE;
    do
    {
        ui4_check_size = TS_LOOKUP_STEPS*(UINT32)_ts_get_max_packet_size();
        ui4_buf_size = (UINT32)(ui4_check_size+TS_LOOKUP_LENGTH);
        pui1_buf = (UINT8 *)_mm_util_mem_alloc(ui4_buf_size);
        
        if (NULL == pui1_buf)
        {
            i4_ret = MFMTRECGR_OUT_OF_MEMORY;
            break;
        }

        i4_ret = x_fm_lseek(h_file, (INT64)0, FM_SEEK_BGN, &ui8_cur_pos);
        if (i4_ret != FMR_OK)
        {
            i4_ret = MFMTRECGR_FILE_ERR;
            break;
        }

        i4_ret = x_fm_read(h_file, pui1_buf, ui4_buf_size, &ui4_read);
        if (i4_ret != FMR_OK)
        {
            i4_ret = MFMTRECGR_FILE_ERR;
            break;
        }
        
        if (ui4_buf_size != ui4_read)
        {
            i4_ret = MFMTRECGR_INTERNAL_ERR;
            break;
        }

        for (ui2_idx=0; ui2_idx<TS_LOOKUP_LENGTH; ui2_idx++)
        {            
            b_found = _ts_check_format(pui1_buf+ui2_idx, ui4_check_size, &ui1_pkt_len, &b_time,NULL);
            if (b_found)
            {
                pt_format->e_media_type = MEDIA_TYPE_TS;
                switch (ui1_pkt_len)
                {
                case 188:
                    pt_format->t_media_subtype.u.e_ts_subtype = MEDIA_TS_SUBTYPE_TS_188; 
                    break;
                    
                case 192:
                    if (b_time)
                    {
                        pt_format->t_media_subtype.u.e_ts_subtype = MEDIA_TS_SUBTYPE_TS_192; 
                    }
                    else
                    {
                        pt_format->t_media_subtype.u.e_ts_subtype = MEDIA_TS_SUBTYPE_TS_192_ZERO; 
                    }
                    break;
                    
                default:
                    pt_format->e_media_type = MEDIA_TYPE_UNKNOWN;
                    pt_format->t_media_subtype.u.e_ts_subtype = MEDIA_TS_SUBTYPE_UNKNOWN; 
                    break;
                }
                               
                i4_ret = MFMTRECGR_OK;
                break;
            }
        }
    }while(0);

    if (pt_format->e_media_type == MEDIA_TYPE_TS)
    {
        UINT8   aui1_system_1[14];
        UINT8   aui1_system_2[14];
        UINT32  ui4_sysb_len;

        do
        {
            i4_ret = x_fm_lseek(h_file, (INT64)0, FM_SEEK_BGN, &ui8_cur_pos);
            if (i4_ret != FMR_OK)
            {
                break;
            }

            i4_ret = x_fm_read(h_file, aui1_system_1, 14, &ui4_read);
            if (i4_ret != FMR_OK)
            {                
                break;
            }


            ui4_sysb_len = (UINT32)aui1_system_1[13]            |
                           (((UINT32)aui1_system_1[12])<<8)     |
                           (((UINT32)aui1_system_1[11])<<16)    |
                           (((UINT32)aui1_system_1[10])<<24);
            if(ui4_sysb_len == 0)
            {
                break;
            }
            i4_ret = x_fm_lseek(h_file, (INT64)14+ui4_sysb_len, FM_SEEK_BGN, &ui8_cur_pos);
            if (i4_ret != FMR_OK)
            {
                break;
            }

            i4_ret = x_fm_read(h_file, aui1_system_2, 14, &ui4_read);
            if (i4_ret != FMR_OK)
            {               
                break;
            }

            b_system_B = TRUE;
            for (ui2_idx=0; ui2_idx<14; ui2_idx++)
            {
                if (aui1_system_1[ui2_idx] != aui1_system_2[ui2_idx])
                {
                    b_system_B = FALSE;
                    break;
                }
            }
            if(b_system_B)
            {
                pt_format->t_media_subtype.u.e_ts_subtype = MEDIA_TS_SUBTYPE_TS_192_SYSTEM_B;
            }
        }while(0);
    }

    if (pui1_buf)
    {
        _mm_util_mem_free(pui1_buf);
    }

    if (b_found)
    {
        return MFMTRECGR_OK;
    }
    else
    {
        pt_format->e_media_type = MEDIA_TYPE_UNKNOWN;
        pt_format->t_media_subtype.u.e_ts_subtype = MEDIA_TS_SUBTYPE_UNKNOWN; 
        if(MFMTRECGR_OK == i4_ret)
        {
            return MFMTRECGR_INTERNAL_ERR;
        }
        else
        {
            return i4_ret;
        }
    }
#else
    return MFMTRECGR_INTERNAL_ERR;
#endif
}
                            
/*-----------------------------------------------------------------------------
 * Name:  ts_handler_mem_confirm_fct
 *
 * Description: this function will ask all handler
 *
 * Inputs:  pv_mem_data     specify the address of memory buffer 
 *                          which want to retrieve the format.
 *          z_mem_leng      specify the length of memory buffer 
 *                          which want to retrieve the format.
 *
 * Outputs: pt_format       retrieve the format of the media memory buffer.
 *
 * Returns: MFMTRECGR_OK    Success. The routine was successful and the 
                            operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 ts_handler_mem_confirm_fct(const VOID*        pv_mem_data, 
                                 SIZE_T             z_mem_leng,   
                                 MEDIA_FORMAT_T*    pt_format) 
{
    UINT8*          pui1_buf;
    UINT32          ui4_buf_size;
    UINT16          ui2_idx;
    BOOL            b_found;
    UINT8           ui1_pkt_len;
    BOOL            b_time;
    MEDIA_FEEDER_T* pt_src_feeder;
    INT32           i4_status;
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pt_format == NULL)
        || (pv_mem_data == NULL))
    {
        return MFMTRECGR_INTERNAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_src_feeder = NULL;
    pui1_buf = NULL;
    b_found = FALSE;
    do
    {
        if (0 == z_mem_leng)
        {
            pt_src_feeder = (MEDIA_FEEDER_T *)pv_mem_data;
            if (NULL == pt_src_feeder->t_feeder_be_fct_tbl.pf_copybytes
                || NULL == pt_src_feeder->t_feeder_be_fct_tbl.pf_input4
                || NULL == pt_src_feeder->t_feeder_be_fct_tbl.pf_input3
                || NULL == pt_src_feeder->t_feeder_be_fct_tbl.pf_input2
                || NULL == pt_src_feeder->t_feeder_be_fct_tbl.pf_input1
                || NULL == pt_src_feeder->t_feeder_be_fct_tbl.pf_set_pos
                || NULL == pt_src_feeder->t_feeder_be_fct_tbl.pf_get_pos)
            {
                return MFMTRECGR_INTERNAL_ERR;
            }
        }
        
        ui4_buf_size = (UINT32)(TS_LOOKUP_STEPS*_ts_get_max_packet_size());
        
        for (ui2_idx=0; ui2_idx<TS_LOOKUP_LENGTH; ui2_idx++)
        {
            if (0 == z_mem_leng)
            {
                pt_src_feeder->t_feeder_be_fct_tbl.pf_set_pos(&pt_src_feeder->h_feeder,
                                                              (INT64)ui2_idx,
                                                              MEDIA_SEEK_BGN);
                #ifndef __KERNEL__
                //x_dbg_stmt("begin to _ts_check_format() \n");
                #endif
                b_found = _ts_check_format((UINT8*)pv_mem_data, z_mem_leng, &ui1_pkt_len, &b_time,&i4_status);
                #ifndef __KERNEL__
                //x_dbg_stmt("end  to _ts_check_format() b_found = %d, i4_status = %d \n", b_found, i4_status);
                #endif
                if(i4_status != MFMTRECGR_OK)
                {
                   ui2_idx  =  TS_LOOKUP_LENGTH;
                }
            }
            else
            {
                if (ui4_buf_size > z_mem_leng-ui2_idx)
                {
                    break;
                }

                pui1_buf = (UINT8*)pv_mem_data + ui2_idx;
                b_found = _ts_check_format(pui1_buf, z_mem_leng, &ui1_pkt_len, &b_time, NULL);
            }
            
            if (b_found)
            {
                pt_format->e_media_type = MEDIA_TYPE_TS;
                switch (ui1_pkt_len)
                {
                case 188:
                    pt_format->t_media_subtype.u.e_ts_subtype = MEDIA_TS_SUBTYPE_TS_188; 
                    break;
                    
                case 192:
                    if (b_time)
                    {
                        pt_format->t_media_subtype.u.e_ts_subtype = MEDIA_TS_SUBTYPE_TS_192; 
                    }
                    else
                    {
                        pt_format->t_media_subtype.u.e_ts_subtype = MEDIA_TS_SUBTYPE_TS_192_ZERO; 
                    }
                    break;
                    
                default:
                    pt_format->e_media_type = MEDIA_TYPE_UNKNOWN;
                    pt_format->t_media_subtype.u.e_ts_subtype = MEDIA_TS_SUBTYPE_UNKNOWN; 
                    break;
                }
                break;
            }
        }
    }while(0);

    if (b_found)
    {
        return MFMTRECGR_OK;
    }
    else
    {
        pt_format->e_media_type = MEDIA_TYPE_UNKNOWN;
        pt_format->t_media_subtype.u.e_ts_subtype = MEDIA_TS_SUBTYPE_UNKNOWN; 
        
        return MFMTRECGR_INTERNAL_ERR;
    }
}

