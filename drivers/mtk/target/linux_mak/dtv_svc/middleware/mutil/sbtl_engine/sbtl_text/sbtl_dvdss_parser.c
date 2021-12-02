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
 * $RCSfile: sbtl_dvdss_parser.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description:
 *
 * History:
 *
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
#include "inc/common.h"
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "dbg/x_dbg.h"
#include "inc/x_mm_common.h"
#include "unicode/x_uc_str.h"

#include "mutil/mm_util.h"

#include "mutil/sbtl_engine/sbtl_dbg.h"
#include "mutil/sbtl_engine/sbtl_struct.h"
#include "mutil/sbtl_engine/sbtl_utils.h"
#include "mutil/sbtl_engine/sbtl_register.h"
#include "mutil/sbtl_engine/sbtl_state.h"
#include "mutil/sbtl_engine/sbtl_text/sbtl_text_page_inst_mngr.h"
#include "mutil/sbtl_engine/sbtl_text/sbtl_dvdss_parser.h"



#else /*__KERNEL__*/
#include "x_os.h"
#include "x_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "x_dbg.h"
#include "x_mm_common.h"
#include "x_uc_str.h"

#include "../../mm_util.h"

#include "../sbtl_dbg.h"
#include "../sbtl_struct.h"
#include "../sbtl_utils.h"
#include "../sbtl_register.h"
#include "../sbtl_state.h"

#include "sbtl_text_page_inst_mngr.h"
#include "sbtl_dvdss_parser.h"
#endif /*__KERNEL__*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define PTS_FREQ        ((UINT64)(90000))
#define TIMER_PERIOD    500

#define CARRIAGE_RETURN             0x0d
#define LINE_FEED                   0x0a
#define CHAR_TAB                    0x09
#define CHAR_SPACE                  0x20

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    data declarations, extern, static, const
 ----------------------------------------------------------------------------*/
 
/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 dvdss_seek_time_stamp(SBTL_PAGE_INST_MNGR_T*    pt_mngr,
                            PTS_T                     t_pts_current,
                            PTS_T*                    pt_pts_start,
                            PTS_T*                    pt_pts_end,
                            BOOL*                     pb_found)
{
    UINT8                           aui1_buf[23];
    UINT16                          aui2_buf[23];
    INT32                           i4_ret;
    UINT32                          ui4_hour;
    UINT32                          ui4_min;
    UINT32                          ui4_sec;
    UINT32                          ui4_ms;
    UINT64                          ui8_offset;
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*  pt_private;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("pt_private=NULL\n"));

    *pb_found = FALSE;
    while(1)
    {
        sbtl_feeder_getpos(pt_mngr->pt_engine, &ui8_offset, MM_SBTL_FEEDER_SUBTITLE);
        
        if (pt_private->b_UTF16)
        {
            i4_ret = sbtl_feeder_inpnbyte(pt_mngr->pt_engine, 46, (UINT8*)aui2_buf, NULL, MM_SBTL_FEEDER_SUBTITLE);
            if (i4_ret != MMSBTLR_OK)
            {
                *pb_found = FALSE;
                return i4_ret;
            }

            if((':' == aui2_buf[2]) && (':' == aui2_buf[5]) && 
               (':' == aui2_buf[8]) && (':' == aui2_buf[14]) && 
               (':' == aui2_buf[17]) && (':' == aui2_buf[20]) &&
               ((CHAR_SPACE == aui2_buf[11]) || (CHAR_TAB == aui2_buf[11])))

            {
                /*     00:00:19:90 00:00:25:30     */
                ui4_hour = sbtl_wasci2decimal(&aui2_buf[0], 2);
                ui4_min = sbtl_wasci2decimal(&aui2_buf[3], 2);
                ui4_sec = sbtl_wasci2decimal(&aui2_buf[6], 2);
                ui4_ms = 10*sbtl_wasci2decimal(&aui2_buf[9], 2);

                
                *pt_pts_start = (UINT64)_mm_div64(PTS_FREQ*(UINT64)ui4_ms, 1000, NULL)
                              + (UINT64)(PTS_FREQ*(UINT64)ui4_sec)
                              + (UINT64)(PTS_FREQ*(UINT64)ui4_min*60)
                              + (UINT64)(PTS_FREQ*(UINT64)ui4_hour*60);

                ui4_hour = sbtl_wasci2decimal(&aui2_buf[12], 1);
                ui4_min = sbtl_wasci2decimal(&aui2_buf[15], 2);
                ui4_sec = sbtl_wasci2decimal(&aui2_buf[18], 2);
                ui4_ms = 10*sbtl_wasci2decimal(&aui2_buf[21], 2);
                
                *pt_pts_end =   (UINT64)_mm_div64(PTS_FREQ*(UINT64)ui4_ms, 1000, NULL)
                              + (UINT64)(PTS_FREQ*(UINT64)ui4_sec)
                              + (UINT64)(PTS_FREQ*(UINT64)ui4_min*60)
                              + (UINT64)(PTS_FREQ*(UINT64)ui4_hour*60);

                if(t_pts_current < *pt_pts_end)
                {
                    *pb_found = TRUE;
                    break;
                }
            }

            
            sbtl_feeder_setpos(pt_mngr->pt_engine, (INT64)(ui8_offset+2), MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);

        }
        else
        {
        i4_ret = sbtl_feeder_inpnbyte(pt_mngr->pt_engine, 23, aui1_buf, NULL, MM_SBTL_FEEDER_SUBTITLE);
        if (i4_ret != MMSBTLR_OK)
        {
            *pb_found = FALSE;
            return i4_ret;
        }

        if((':' == aui1_buf[2]) && (':' == aui1_buf[5]) && 
           (':' == aui1_buf[8]) && (':' == aui1_buf[14]) && 
           (':' == aui1_buf[17]) && (':' == aui1_buf[20]) &&
           ((CHAR_SPACE == aui1_buf[11]) || (CHAR_TAB == aui1_buf[11])))

        {
                /*     00:00:19:90 00:00:25:30     */
            ui4_hour = sbtl_asci2decimal(&aui1_buf[0], 2);
            ui4_min = sbtl_asci2decimal(&aui1_buf[3], 2);
            ui4_sec = sbtl_asci2decimal(&aui1_buf[6], 2);
            ui4_ms = 10*sbtl_asci2decimal(&aui1_buf[9], 2);

            
            *pt_pts_start = (UINT64)_mm_div64(PTS_FREQ*(UINT64)ui4_ms, 1000, NULL)
                          + (UINT64)(PTS_FREQ*(UINT64)ui4_sec)
                          + (UINT64)(PTS_FREQ*(UINT64)ui4_min*60)
                          + (UINT64)(PTS_FREQ*(UINT64)ui4_hour*60);

            ui4_hour = sbtl_asci2decimal(&aui1_buf[12], 1);
            ui4_min = sbtl_asci2decimal(&aui1_buf[15], 2);
            ui4_sec = sbtl_asci2decimal(&aui1_buf[18], 2);
            ui4_ms = 10*sbtl_asci2decimal(&aui1_buf[21], 2);
            
            *pt_pts_end =   (UINT64)_mm_div64(PTS_FREQ*(UINT64)ui4_ms, 1000, NULL)
                          + (UINT64)(PTS_FREQ*(UINT64)ui4_sec)
                          + (UINT64)(PTS_FREQ*(UINT64)ui4_min*60)
                          + (UINT64)(PTS_FREQ*(UINT64)ui4_hour*60);

                if(t_pts_current < *pt_pts_end)
                {
                     *pb_found = TRUE;
                     break;
                }
            }
        
            sbtl_feeder_setpos(pt_mngr->pt_engine, (INT64)(ui8_offset+1), MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);

        }
    }
    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 dvdss_load_string(SBTL_PAGE_INST_MNGR_T*    pt_mngr)
{
    MEDIA_FEEDER_T*                 pt_feeder;
    UINT8                           ui1_idx2 = 0;
    UINT8                           ui1_idx = 0;
    INT32                           i4_ret;
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*   pt_private;
    UINT16                          ui2_tmp;
    MM_SBTL_ENCODING_T              e_encoding;
    
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));

    pt_feeder = pt_mngr->pt_engine->t_cfg.pt_feeder[MM_SBTL_FEEDER_SUBTITLE];
    ASSERT(pt_feeder, ("pt_feeder=NULL\n"));
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    if (pt_private == NULL || pt_feeder == NULL)
    {
        return MMSBTLR_INV_ARG;
    }
    
    e_encoding = pt_mngr->pt_engine->t_cfg.e_encoding;
    while (1)
    {
        if (pt_private->b_UTF16)
        {
            i4_ret = sbtl_feeder_inpnbyte(pt_mngr->pt_engine, 2, (UINT8*)&pt_private->w2s_text[ui1_idx], NULL, MM_SBTL_FEEDER_SUBTITLE);
        }
        else
        {
            i4_ret = sbtl_feeder_inp1byte(pt_mngr->pt_engine, &pt_private->aui1_text_buf[ui1_idx], MM_SBTL_FEEDER_SUBTITLE);
            ui2_tmp = pt_private->aui1_text_buf[ui1_idx];
        }
        
        if (i4_ret != MMSBTLR_OK)
        {
            break;
        }
        
        if (e_encoding == MM_SBTL_ENCODE_UTF16)
        {
            sbtl_trans_to_w2s(MM_SBTL_LANG_UNICODE, pt_private->w2s_text[ui1_idx], &ui2_tmp);
        }
        else
        {
            sbtl_trans_to_w2s(MM_SBTL_LANG_ASCI, pt_private->aui1_text_buf[ui1_idx], &ui2_tmp);
        }
        
        //sbtl_trans_to_w2s(pt_private->e_language, ui2_tmp, &pt_private->w2s_text[ui1_idx]);
        //ui2_tmp = pt_private->w2s_text[ui1_idx];

        if ((ui2_tmp == 0x0D) || (ui2_tmp == 0x0A))
        {
            //pt_private->w2s_text[ui1_idx] = 0;
            if (e_encoding == MM_SBTL_ENCODE_UTF16)
            {
                pt_private->w2s_text[ui1_idx] = 0;
            }
            else
            {
                pt_private->aui1_text_buf[ui1_idx] = 0;
            }
            break;
        }
        else if (ui2_tmp == '/')
        {
            ui1_idx2++;
        }
        else
        {
            ui1_idx2 = 0;
        }
        
        if (ui1_idx2>=2)
        {
            ui1_idx2 = 0;
            ui1_idx -= 1;
            //pt_private->w2s_text[ui1_idx] = 0x0A;
            if (e_encoding == MM_SBTL_ENCODE_UTF16)
            {
                pt_private->w2s_text[ui1_idx] = 0x0A;
            }
            else
            {
                pt_private->aui1_text_buf[ui1_idx] = 0x0A;
            }
        }
       
        ui1_idx++;
    }

    //pt_private->w2s_text[ui1_idx] = 0;
    if (e_encoding == MM_SBTL_ENCODE_UTF16)
    {
        pt_private->w2s_text[ui1_idx] = 0;
    }
    else
       {
        pt_private->aui1_text_buf[ui1_idx] = 0;
        sbtl_trans_to_w2s_ex(e_encoding,pt_private->aui1_text_buf,MM_SBTL_CACHE_MAX,pt_private->w2s_text);
    }    
    
    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
UINT32 dvdss_timer_period(SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    ASSERT(pt_mngr->pt_engine->t_cfg.i4_speed>0, ("pt_mngr->pt_engine.t_cfg.i4_speed=0\n"));

    return (UINT32)((TIMER_PERIOD * 100)/pt_mngr->pt_engine->t_cfg.i4_speed);
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 dvdss_init(SBTL_PAGE_INST_MNGR_T*   pt_mngr)
{
    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 dvdss_deinit(SBTL_PAGE_INST_MNGR_T*     pt_mngr)
{
    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID dvdss_start(SBTL_PAGE_INST_MNGR_T*   pt_mngr)
{
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID dvdss_stop(SBTL_PAGE_INST_MNGR_T*    pt_mngr)
{
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
/*
00:00:19:90 00:00:25:30 the only sin left,//the last one.
*/
BOOL dvdss_parser_verify(SBTL_ENGINE_T*   pt_engine)
{
    INT32   i4_ret;
    UINT16  ui1_idx;
    UINT8   aui1_buf[24];
    UINT16  aui2_buf[24];
    BOOL    b_UTF16;
    UINT8   ui1_tmp;

    ASSERT(pt_engine, ("pt_engine=NULL\n"));
    
    b_UTF16 = FALSE;
    
    sbtl_feeder_setpos(pt_engine, 0, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
    sbtl_feeder_inpnbyte(pt_engine, 1, &ui1_tmp, NULL, MM_SBTL_FEEDER_SUBTITLE);
    
    if (0xFF == ui1_tmp)
    {
        sbtl_feeder_inpnbyte(pt_engine, 1, &ui1_tmp, NULL, MM_SBTL_FEEDER_SUBTITLE);
        if (0xFE == ui1_tmp)
        {
            b_UTF16 = TRUE;
        }
    }

    sbtl_feeder_setpos(pt_engine, 0, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
    
    ui1_idx = 0;
    
    for(ui1_idx=0; ui1_idx<256; ui1_idx++)
    {
        if (b_UTF16)
        {
            sbtl_feeder_setpos(pt_engine, (INT64)ui1_idx*2, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
             
            i4_ret = sbtl_feeder_inpnbyte(pt_engine, 48, (UINT8*)aui2_buf, NULL, MM_SBTL_FEEDER_SUBTITLE);
            if (i4_ret != MMSBTLR_OK)
            {
                return FALSE;
            }

            if((':' == aui2_buf[2]) && (':' == aui2_buf[5]) && 
               (':' == aui2_buf[8]) && (':' == aui2_buf[14]) && 
               (':' == aui2_buf[17]) && (':' == aui2_buf[20]) &&
               ((CHAR_SPACE == aui2_buf[11]) || (CHAR_TAB == aui2_buf[11])))
            {
                return TRUE;
            }

        }
        else
        {
            sbtl_feeder_setpos(pt_engine, (INT64)ui1_idx, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
         
            i4_ret = sbtl_feeder_inpnbyte(pt_engine, 24, aui1_buf, NULL, MM_SBTL_FEEDER_SUBTITLE);
            if (i4_ret != MMSBTLR_OK)
            {
                return FALSE;
            }

            if((':' == aui1_buf[2]) && (':' == aui1_buf[5]) && 
               (':' == aui1_buf[8]) && (':' == aui1_buf[14]) && 
               (':' == aui1_buf[17]) && (':' == aui1_buf[20]) &&
               ((CHAR_SPACE == aui1_buf[11]) || (CHAR_TAB == aui1_buf[11])))
             {
                return TRUE;
             }
        }

    }

    
    return FALSE;
}


