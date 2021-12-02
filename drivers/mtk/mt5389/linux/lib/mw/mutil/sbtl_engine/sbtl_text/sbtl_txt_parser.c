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
 * $RCSfile: sbtl_txt_parser.c,v $
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

#include "mutil/sbtl_engine/sbtl_dbg.h"
#include "mutil/sbtl_engine/sbtl_struct.h"
#include "mutil/sbtl_engine/sbtl_utils.h"
#include "mutil/sbtl_engine/sbtl_register.h"
#include "mutil/sbtl_engine/sbtl_state.h"
#include "mutil/sbtl_engine/sbtl_text/sbtl_text_page_inst_mngr.h"
#include "mutil/sbtl_engine/sbtl_text/sbtl_txt_parser.h"
#include "mutil/sbtl_engine/u_mm_sbtl_engine.h"



#else /*__KERNEL__*/
#include "x_os.h"
#include "x_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "x_dbg.h"
#include "x_mm_common.h"
#include "x_uc_str.h"


#include "../sbtl_dbg.h"
#include "../sbtl_struct.h"
#include "../sbtl_utils.h"
#include "../sbtl_register.h"
#include "../sbtl_state.h"

#include "sbtl_text_page_inst_mngr.h"
#include "sbtl_txt_parser.h"
#include "u_mm_sbtl_engine.h"

#endif /*__KERNEL__*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define PTS_FREQ        ((UINT64)(90000))
#define TIMER_PERIOD    500
#define SUB_TXT_SUPPORT_MORE

#define CARRIAGE_RETURN             0x0d
#define LINE_FEED                   0x0a
#define CHAR_TAB                    0x09
#define CHAR_SPACE                  0x20


enum  /* for _dSubtxtType */
{
  SUB_TXT_TYPE_UNKNOWN = 0x0,
  SUB_TXT_TYPE_1       = 0x1, /* HH:MM:SS:   */
  SUB_TXT_TYPE_2       = 0x2, /*  H:MM:SS:   */
  SUB_TXT_TYPE_3       = 0x3, /* HH:MM:SS=   */
  SUB_TXT_TYPE_4       = 0x4, /*  H:MM:SS=   */
  SUB_TXT_TYPE_5       = 0x5  /* HH:MM:SS,1= */
                              /* HH:MM:SS,2= */
};

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    data declarations, extern, static, const
 ----------------------------------------------------------------------------*/
 
/*-----------------------------------------------------------------------------
                    Internal functions implementations
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
static BOOL _is_number(UINT8 ui1_char)
{
    if(ui1_char >= '0' && ui1_char <= '9') /* 0x30 to 0x39 */
    {
        return TRUE;
    }
    
    return FALSE;
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
static BOOL _is_number_utf16(UINT16 ui2_char)
{
    if(ui2_char >= '0' && ui2_char <= '9') /* 0x30 to 0x39 */
    {
        return TRUE;
    }
    
    return FALSE;
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
static INT32 _txt_vaild_time_tag(SBTL_ENGINE_T*     pt_engine,
                                 MM_SBTL_ENCODING_T       e_encoding,
                                 UINT8*             pui1_text_type,
                                 UINT32*            pui4_time)
{
    INT32                           i4_ret;
    UINT8                           aui1_buf[20];
    UINT32                          ui4_hour = 0;
    UINT32                          ui4_min = 0;
    UINT32                          ui4_sec = 0;
    UINT16                          aui2_buf[20];
    
    if(pui1_text_type == NULL)    
    {
        return MMSBTLR_INV_ARG;
    }

    if (e_encoding == MM_SBTL_ENCODE_UTF16)
    {
        i4_ret = sbtl_feeder_inpnbyte(pt_engine, 40, (UINT8*)aui2_buf, NULL, MM_SBTL_FEEDER_SUBTITLE);

        if (i4_ret != MMSBTLR_OK)
        {
            return i4_ret;
        }        
        
        if (_is_number_utf16(aui2_buf[10]) && (aui2_buf[11] == ' ')) /* avoid 00:00:00:00 00:00:00:00 Dvdss*/
        {
             *pui1_text_type = SUB_TXT_TYPE_UNKNOWN;
        }
        else if(_is_number_utf16(aui2_buf[9]) && (aui2_buf[10] == ' ')) /* avoid 0:00:00:00 0:00:00:00 Dvdss*/
        {
            *pui1_text_type = SUB_TXT_TYPE_UNKNOWN;
        }
        else if(_is_number_utf16(aui2_buf[0]) && _is_number_utf16(aui2_buf[1]) &&
            (aui2_buf[2] == ':') && (aui2_buf[5] == ':') && (aui2_buf[8] == ':'))
        {
            *pui1_text_type = SUB_TXT_TYPE_1;  /* 00:00:00:*/
        }
        else if(_is_number_utf16(aui2_buf[0]) && _is_number_utf16(aui2_buf[1]) &&
            (aui2_buf[2] == ':') && (aui2_buf[5] == ':') && (aui2_buf[8] == '='))
        {
            *pui1_text_type = SUB_TXT_TYPE_3;  /* 00:00:00=*/
        }
#ifdef SUB_TXT_SUPPORT_MORE
        else if(_is_number_utf16(aui2_buf[0]) && _is_number_utf16(aui2_buf[1]) &&
            (aui2_buf[2] == ':') && (aui2_buf[5] == ':') &&
            (aui2_buf[8] == ',') && _is_number_utf16(aui2_buf[9]))
        {
            *pui1_text_type = SUB_TXT_TYPE_5;
        }
        else if(_is_number_utf16(aui2_buf[0]) &&
            (aui2_buf[1] == ':') && (aui2_buf[4] == ':') && (aui2_buf[7] == ':'))
        {
            *pui1_text_type = SUB_TXT_TYPE_2;  /* 0:00:00:*/
        }
        else if(_is_number_utf16(aui2_buf[0]) &&
            (aui2_buf[1] == ':') && (aui2_buf[4] == ':') && (aui2_buf[7] == '='))
        {
            *pui1_text_type = SUB_TXT_TYPE_4;  /* 0:00:00=*/
        }
#endif
        else
        {
            *pui1_text_type = SUB_TXT_TYPE_UNKNOWN;
        }


        switch(*pui1_text_type)
        {
        case SUB_TXT_TYPE_1:
        case SUB_TXT_TYPE_3:
            ui4_hour = sbtl_wasci2decimal(aui2_buf, 2);
            ui4_min = sbtl_wasci2decimal(aui2_buf+3, 2);
            ui4_sec = sbtl_wasci2decimal(aui2_buf+6, 2);
            break;
            
#ifdef SUB_TXT_SUPPORT_MORE
        case SUB_TXT_TYPE_2:
        case SUB_TXT_TYPE_4:
            ui4_hour = sbtl_wasci2decimal(aui2_buf, 1);
            ui4_min = sbtl_wasci2decimal(aui2_buf+2, 2);
            ui4_sec = sbtl_wasci2decimal(aui2_buf+5, 2);
            break;
            
        case SUB_TXT_TYPE_5:
            ui4_hour = sbtl_wasci2decimal(aui2_buf, 2);
            ui4_min = sbtl_wasci2decimal(aui2_buf+3, 2);
            ui4_sec = sbtl_wasci2decimal(aui2_buf+6, 2);
            break;
#endif
        default:
            break;
        }

    }
    else
    {
        i4_ret = sbtl_feeder_inpnbyte(pt_engine, 20, aui1_buf, NULL, MM_SBTL_FEEDER_SUBTITLE);
            
        if (i4_ret != MMSBTLR_OK)
        {
            return i4_ret;
        }
        
        if (_is_number(aui1_buf[10]) && (aui1_buf[11] == ' ')) /* avoid 00:00:00:00 00:00:00:00 Dvdss*/
        {
             *pui1_text_type = SUB_TXT_TYPE_UNKNOWN;
        }
        else if(_is_number(aui1_buf[9]) && (aui1_buf[10] == ' ')) /* avoid 0:00:00:00 0:00:00:00 Dvdss*/
        {
            *pui1_text_type = SUB_TXT_TYPE_UNKNOWN;
        }
        else if(_is_number(aui1_buf[0]) && _is_number(aui1_buf[1]) &&
            (aui1_buf[2] == ':') && (aui1_buf[5] == ':') && (aui1_buf[8] == ':'))
        {
            *pui1_text_type = SUB_TXT_TYPE_1;  /* 00:00:00:*/
        }
        else if(_is_number(aui1_buf[0]) && _is_number(aui1_buf[1]) &&
            (aui1_buf[2] == ':') && (aui1_buf[5] == ':') && (aui1_buf[8] == '='))
        {
            *pui1_text_type = SUB_TXT_TYPE_3;  /* 00:00:00=*/
        }
#ifdef SUB_TXT_SUPPORT_MORE
        else if(_is_number(aui1_buf[0]) && _is_number(aui1_buf[1]) &&
            (aui1_buf[2] == ':') && (aui1_buf[5] == ':') &&
            (aui1_buf[8] == ',') && _is_number(aui1_buf[9]))
        {
            *pui1_text_type = SUB_TXT_TYPE_5;
        }
        else if(_is_number(aui1_buf[0]) &&
            (aui1_buf[1] == ':') && (aui1_buf[4] == ':') && (aui1_buf[7] == ':'))
        {
            *pui1_text_type = SUB_TXT_TYPE_2;  /* 0:00:00:*/
        }
        else if(_is_number(aui1_buf[0]) &&
            (aui1_buf[1] == ':') && (aui1_buf[4] == ':') && (aui1_buf[7] == '='))
        {
            *pui1_text_type = SUB_TXT_TYPE_4;  /* 0:00:00=*/
        }
#endif
        else
        {
            *pui1_text_type = SUB_TXT_TYPE_UNKNOWN;
        }


        switch(*pui1_text_type)
        {
        case SUB_TXT_TYPE_1:
        case SUB_TXT_TYPE_3:
            ui4_hour = sbtl_asci2decimal(aui1_buf, 2);
            ui4_min = sbtl_asci2decimal(aui1_buf+3, 2);
            ui4_sec = sbtl_asci2decimal(aui1_buf+6, 2);
            break;
            
#ifdef SUB_TXT_SUPPORT_MORE
        case SUB_TXT_TYPE_2:
        case SUB_TXT_TYPE_4:
            ui4_hour = sbtl_asci2decimal(aui1_buf, 1);
            ui4_min = sbtl_asci2decimal(aui1_buf+2, 2);
            ui4_sec = sbtl_asci2decimal(aui1_buf+5, 2);
            break;
            
        case SUB_TXT_TYPE_5:
            ui4_hour = sbtl_asci2decimal(aui1_buf, 2);
            ui4_min = sbtl_asci2decimal(aui1_buf+3, 2);
            ui4_sec = sbtl_asci2decimal(aui1_buf+6, 2);
            break;
#endif
        default:
            break;
        }

    }    
    
    if(pui4_time != NULL)
    {
    *pui4_time = ui4_hour*3600 + ui4_min*60 + ui4_sec;
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
static INT32 _txt_unit_start(SBTL_ENGINE_T*        pt_engine,
                             MM_SBTL_ENCODING_T       e_encoding,
                             BOOL*                 pb_found,
                             UINT8*                pui1_text_type,
                             UINT32*               pui4_time)
{
    UINT8   ui1_text_type;
    INT32   i4_ret;
    UINT64  ui8_offset;

    ASSERT(pt_engine, ("pt_engine=NULL\n"));

    while(1)
    {
        sbtl_feeder_getpos(pt_engine, &ui8_offset, MM_SBTL_FEEDER_SUBTITLE);
        i4_ret = _txt_vaild_time_tag(pt_engine, e_encoding, &ui1_text_type, pui4_time);
        if (i4_ret != MMSBTLR_OK)
        {
            *pb_found = FALSE;
            return i4_ret;
        }
        
        switch(ui1_text_type)
        {
        case SUB_TXT_TYPE_1:
        case SUB_TXT_TYPE_3:
            if(e_encoding == MM_SBTL_ENCODE_UTF16)
            {
                sbtl_feeder_setpos(pt_engine, (INT64)(ui8_offset+18), MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);  /* skip xx:xx:xx: */
            }
            else
            {
                sbtl_feeder_setpos(pt_engine, (INT64)(ui8_offset+9), MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);  /* skip xx:xx:xx: */
            }
            *pb_found = TRUE;
            *pui1_text_type = ui1_text_type;
            return MMSBTLR_OK;
            
#ifdef SUB_TXT_SUPPORT_MORE
        case SUB_TXT_TYPE_2:
        case SUB_TXT_TYPE_4:
            if(e_encoding == MM_SBTL_ENCODE_UTF16)
            {
                sbtl_feeder_setpos(pt_engine, (INT64)(ui8_offset+16), MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);  /* skip xx:xx:xx: */
            }
            else
            {
                sbtl_feeder_setpos(pt_engine, (INT64)(ui8_offset+8), MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);  /* skip xx:xx:xx: */
            }            
            *pb_found = TRUE;
            *pui1_text_type = ui1_text_type;
            return MMSBTLR_OK;
            
        case SUB_TXT_TYPE_5:
            if(e_encoding == MM_SBTL_ENCODE_UTF16)
            {
                sbtl_feeder_setpos(pt_engine, (INT64)(ui8_offset+22), MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);  /* skip xx:xx:xx: */
            }
            else
            {
                sbtl_feeder_setpos(pt_engine, (INT64)(ui8_offset+11), MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);  /* skip xx:xx:xx: */
            }             
            *pb_found = TRUE;
            *pui1_text_type = ui1_text_type;
            return MMSBTLR_OK;
#endif
        default:
            sbtl_feeder_setpos(pt_engine, (INT64)(ui8_offset), MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
            break;
        }

        if(e_encoding == MM_SBTL_ENCODE_UTF16)
        {
            sbtl_feeder_setpos(pt_engine, 2, MEDIA_SEEK_CUR, MM_SBTL_FEEDER_SUBTITLE);
        }
        else
        {
            sbtl_feeder_setpos(pt_engine, 1, MEDIA_SEEK_CUR, MM_SBTL_FEEDER_SUBTITLE);
        }
    }
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
static INT32 _txt_unit_start_p(SBTL_ENGINE_T*        pt_engine,
                                MM_SBTL_ENCODING_T    e_encoding,
                                BOOL*                 pb_found,
                                UINT8*                pui1_text_type,
                                UINT32*               pui4_time,
                                UINT64                ui4_parse_size)
{
    UINT8   ui1_text_type;
    INT32   i4_ret;
    UINT64  ui8_offset;

    ASSERT(pt_engine, ("pt_engine=NULL\n"));

    if(ui4_parse_size <= 0)
    {
        *pb_found = FALSE;
        return MMSBTLR_INV_ARG;
    }

    while(1)
    {
        sbtl_feeder_getpos(pt_engine, &ui8_offset, MM_SBTL_FEEDER_SUBTITLE);
        if(ui8_offset > ui4_parse_size)
        {
            *pb_found = FALSE;
            return MMSBTLR_OK;
        }
        i4_ret = _txt_vaild_time_tag(pt_engine, e_encoding, &ui1_text_type, pui4_time);
        if (i4_ret != MMSBTLR_OK)
        {
            *pb_found = FALSE;
            return i4_ret;
        }
        
        switch(ui1_text_type)
        {
        case SUB_TXT_TYPE_1:
        case SUB_TXT_TYPE_3:
            if(e_encoding == MM_SBTL_ENCODE_UTF16)
            {
                sbtl_feeder_setpos(pt_engine, (INT64)(ui8_offset+18), MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);  /* skip xx:xx:xx: */
            }
            else
            {
                sbtl_feeder_setpos(pt_engine, (INT64)(ui8_offset+9), MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);  /* skip xx:xx:xx: */
            }
            *pb_found = TRUE;
            *pui1_text_type = ui1_text_type;
            return MMSBTLR_OK;
            
#ifdef SUB_TXT_SUPPORT_MORE
        case SUB_TXT_TYPE_2:
        case SUB_TXT_TYPE_4:
            if(e_encoding == MM_SBTL_ENCODE_UTF16)
            {
                sbtl_feeder_setpos(pt_engine, (INT64)(ui8_offset+16), MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);  /* skip xx:xx:xx: */
            }
            else
            {
                sbtl_feeder_setpos(pt_engine, (INT64)(ui8_offset+8), MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);  /* skip xx:xx:xx: */
            }            
            *pb_found = TRUE;
            *pui1_text_type = ui1_text_type;
            return MMSBTLR_OK;
            
        case SUB_TXT_TYPE_5:
            if(e_encoding == MM_SBTL_ENCODE_UTF16)
            {
                sbtl_feeder_setpos(pt_engine, (INT64)(ui8_offset+22), MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);  /* skip xx:xx:xx: */
            }
            else
            {
                sbtl_feeder_setpos(pt_engine, (INT64)(ui8_offset+11), MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);  /* skip xx:xx:xx: */
            }             
            *pb_found = TRUE;
            *pui1_text_type = ui1_text_type;
            return MMSBTLR_OK;
#endif
        default:
            sbtl_feeder_setpos(pt_engine, (INT64)(ui8_offset), MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
            break;
        }

        if(e_encoding == MM_SBTL_ENCODE_UTF16)
        {
            sbtl_feeder_setpos(pt_engine, 2, MEDIA_SEEK_CUR, MM_SBTL_FEEDER_SUBTITLE);
        }
        else
        {
            sbtl_feeder_setpos(pt_engine, 1, MEDIA_SEEK_CUR, MM_SBTL_FEEDER_SUBTITLE);
        }
    }
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
 static VOID _txt_load_content(SBTL_ENGINE_T*    pt_engine,
                                  UINT16*            s_buf,
                                  BOOL              b_append)
{
    UINT8                           aui1_tmp[2];
    UINT16                          aui2_tmp[2];
    UINT32                          ui4_done_bytes;
    UINT16                          ui2_len;
    UINT16                          ui2_idx ;
    UINT16                          ui2_idx2 ;
    INT32                           i4_ret;
    UINT64                          ui8_offset;
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*  pt_private;
    MM_SBTL_ENCODING_T              e_encoding;

    ASSERT(pt_engine, ("pt_engine=NULL\n"));
    ASSERT(s_buf, ("s_buf=NULL\n"));
    
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_engine->pt_page_mngr->pv_private;
    ASSERT(pt_private, ("pt_private=NULL\n"));
    
    if (!b_append)
    {
        ui2_len = 0;
    }
    else
    {
        ui2_len = (UINT16)x_strlen((const CHAR *)s_buf);
        s_buf[ui2_len] = LINE_FEED;
        ui2_len++;
    }

    if(ui2_len >= MM_SBTL_CACHE_MAX)
    {
        return ;
    }
    
    ui2_idx = 0;
    sbtl_feeder_getpos(pt_engine, &ui8_offset, MM_SBTL_FEEDER_SUBTITLE);
    e_encoding = pt_engine->t_cfg.e_encoding;
    x_memset(pt_private->aui1_text_buf,0,sizeof(UINT8)*MM_SBTL_CACHE_MAX);

    while (1)
    {
        
        if (e_encoding == MM_SBTL_ENCODE_UTF16)
        {
            ui2_idx2 = ui2_len + ui2_idx;
            i4_ret = sbtl_feeder_inpnbyte(pt_engine, 4, (UINT8*)aui2_tmp, &ui4_done_bytes, MM_SBTL_FEEDER_SUBTITLE);
            if (i4_ret != MMSBTLR_OK)
            {
                if (ui4_done_bytes)
                {
#ifdef MORE_CHANGE_LINE_SYMBOL
                    if (aui2_tmp[0] == CARRIAGE_RETURN || aui2_tmp[0] == LINE_FEED)
                    {
                        s_buf[ui2_idx2] = 0;
                    }
                    else
#endif
                    {
                        if (aui2_tmp[0] == '|')
                        {
                            aui2_tmp[0] = LINE_FEED;
                        }

                        sbtl_trans_to_w2s(pt_private->e_language, aui2_tmp[0], &s_buf[ui2_idx2]);
                        s_buf[ui2_idx2+1] = 0;
                    }
                }
                else
                {
                    s_buf[ui2_idx2] = 0;
                }
                break;
            }

            if ((aui2_tmp[0] == CARRIAGE_RETURN) && (aui2_tmp[1] == LINE_FEED))
            {
                s_buf[ui2_idx2] = 0;  /* 2 means skip CARRIAGE_RETURN & LINE_FEED */
                break;
            }
            else
            {
#ifdef MORE_CHANGE_LINE_SYMBOL
                if(aui2_tmp[0] == CARRIAGE_RETURN || aui2_tmp[0] == LINE_FEED)
                {
                    s_buf[ui2_idx2] = 0;   /* 1 means skip CARRIAGE_RETURN or LINE_FEED */
                    break;
                }
                else
#endif
                {
                    if (aui2_tmp[0] == '|')
                    {
                        aui2_tmp[0] = LINE_FEED;
                    }

                    sbtl_trans_to_w2s(MM_SBTL_LANG_UNICODE, aui2_tmp[0], &s_buf[ui2_idx2]);
                    ui2_idx ++;
                }
            }

            ui8_offset += 2;
        }
        else
        {
            if(ui2_idx >= MM_SBTL_CACHE_MAX)
            {
                break;
            }
            
            i4_ret = sbtl_feeder_inpnbyte(pt_engine, 2, aui1_tmp, &ui4_done_bytes, MM_SBTL_FEEDER_SUBTITLE);
            if (i4_ret != MMSBTLR_OK)
            {
                if (ui4_done_bytes)
                {
#ifdef MORE_CHANGE_LINE_SYMBOL
                    if (aui1_tmp[0] == CARRIAGE_RETURN || aui1_tmp[0] == LINE_FEED)
                    {
                        pt_private->aui1_text_buf[ui2_idx] = 0;
                    }
                    else
#endif
                    {
                        if (aui1_tmp[0] == '|')
                        {
                            aui1_tmp[0] = LINE_FEED;
                        }

                        pt_private->aui1_text_buf[ui2_idx] = (UINT8)aui1_tmp[0];
                    }
                }
                else
                {

                    pt_private->aui1_text_buf[ui2_idx] = 0;
                }
                break;
            } 
            
            if ((aui1_tmp[0] == CARRIAGE_RETURN) && (aui1_tmp[1] == LINE_FEED))
            {
                pt_private->aui1_text_buf[ui2_idx] = 0;  /* 2 means skip CARRIAGE_RETURN & LINE_FEED */
                break;
            }
            else
            {
#ifdef MORE_CHANGE_LINE_SYMBOL
                if(aui1_tmp[0] == CARRIAGE_RETURN || aui1_tmp[0] == LINE_FEED)
                {
                    pt_private->aui1_text_buf[ui2_idx] = 0;   /* 1 means skip CARRIAGE_RETURN or LINE_FEED */
                    break;
                }
                else
#endif
                {
                    if (aui1_tmp[0] == '|')
                    {
                        aui1_tmp[0] = LINE_FEED;
                    }

                    pt_private->aui1_text_buf[ui2_idx++] = aui1_tmp[0];

                }
            }  

            ui8_offset++;
        }

        sbtl_feeder_setpos(pt_engine, (INT64)ui8_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
    }
    
    if(e_encoding != MM_SBTL_ENCODE_UTF16)
    {
        if((ui2_len + ui2_idx) >= MM_SBTL_CACHE_MAX)
        {
            ui2_idx = MM_SBTL_CACHE_MAX - ui2_len;
        }
        
        if(ui2_idx >= MM_SBTL_CACHE_MAX)
        {
            ui2_idx = (UINT16)(MM_SBTL_CACHE_MAX - 1);
        }
        sbtl_trans_to_w2s_ex(e_encoding,
                             pt_private->aui1_text_buf,
                             ui2_idx,
                             &pt_private->w2s_text[ui2_len]);
    }

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
#ifdef SUB_TXT_SUPPORT_MORE
static BOOL _txt_unit_add_next(SBTL_ENGINE_T*      pt_engine,
                               MM_SBTL_ENCODING_T                e_encoding,
                               UINT32              ui4_next_time)
{
    UINT8   ui1_text_type;
    INT32   i4_ret;
    UINT32  ui4_time;
    UINT64  ui8_offset;
    
    sbtl_feeder_getpos(pt_engine, &ui8_offset, MM_SBTL_FEEDER_SUBTITLE);

    i4_ret = _txt_vaild_time_tag(pt_engine, e_encoding, &ui1_text_type, &ui4_time);
    if (i4_ret != MMSBTLR_OK)
    {
        return FALSE;
    }

    if(SUB_TXT_TYPE_5 == ui1_text_type)
    {
        if (ui4_time == ui4_next_time)
        {
            if (e_encoding == MM_SBTL_ENCODE_UTF16)
            {
                sbtl_feeder_setpos(pt_engine, (INT64)(ui8_offset+22), MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE); /* skip xx:xx:xx,x */
            }
            else
            {
                sbtl_feeder_setpos(pt_engine, (INT64)(ui8_offset+11), MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE); /* skip xx:xx:xx,x */
            }
            
            return TRUE;
        }
    }

    sbtl_feeder_setpos(pt_engine, (INT64)ui8_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
    
    return FALSE;
}
#endif

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
INT32 txt_seek_time_stamp(SBTL_PAGE_INST_MNGR_T*      pt_mngr,
                          PTS_T                       t_pts_current,
                          PTS_T*                      pt_pts_start,
                          PTS_T*                      pt_pts_end,
                          BOOL*                       pb_found)
{
    UINT8                           ui1_text_type;
    INT32                           i4_ret;
    BOOL                            b_found;
    UINT32                          ui4_time;
    UINT32                          ui4_next_time;
    UINT64                          ui8_offset;
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*  pt_private;
    MM_SBTL_ENCODING_T             e_encoding;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_pts_start, ("pt_pts_start=NULL\n"));
    ASSERT(pt_pts_end, ("pt_pts_end=NULL\n"));
    ASSERT(pb_found, ("pb_found=NULL\n"));

    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("pt_private=NULL\n"));

    e_encoding = pt_mngr->pt_engine->t_cfg.e_encoding;
    x_memset(pt_private->w2s_text,0,sizeof(UINT16)*MM_SBTL_CACHE_MAX);

    while (1)
    {
        sbtl_feeder_getpos(pt_mngr->pt_engine, &ui8_offset, MM_SBTL_FEEDER_SUBTITLE);
        
        i4_ret = _txt_unit_start(pt_mngr->pt_engine, 
                                 e_encoding, 
                                 &b_found, 
                                 &ui1_text_type, 
                                 &ui4_time);
        if (i4_ret != MMSBTLR_OK)
        {
            *pb_found = FALSE;
            return i4_ret;
        }

        if (!b_found)
        {
            *pb_found = FALSE;
            return MMSBTLR_OK;
        }

        *pt_pts_start = (UINT64)ui4_time*PTS_FREQ;
            
        while (1) /*skip the same STC line*/
        {
            i4_ret = _txt_unit_start(pt_mngr->pt_engine, e_encoding, &b_found, &ui1_text_type, &ui4_next_time);
            if (i4_ret != MMSBTLR_OK)
            {
                ui4_next_time = ui4_time + 2;
                break;
            }

#ifdef SUB_TXT_SUPPORT_MORE
            if ((ui4_next_time > ui4_time) || (ui1_text_type != SUB_TXT_TYPE_5))
#endif
            {
                break;
            }
        }

        *pt_pts_end = (UINT64)ui4_next_time*PTS_FREQ;

        if (t_pts_current < *pt_pts_end)
        {
            *pb_found = TRUE;
            sbtl_feeder_setpos(pt_mngr->pt_engine, (INT64)ui8_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
            break;
        }
    }

    _txt_unit_start(pt_mngr->pt_engine, e_encoding,&b_found, &ui1_text_type, &ui4_time);

    _txt_load_content(pt_mngr->pt_engine,
                      pt_private->w2s_text,
                      FALSE);
#ifdef SUB_TXT_SUPPORT_MORE
    while (_txt_unit_add_next(pt_mngr->pt_engine, e_encoding, ui4_next_time))
    {
        _txt_load_content(pt_mngr->pt_engine,
                          pt_private->w2s_text,
                          TRUE);
    }
#endif

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
INT32 txt_load_string(SBTL_PAGE_INST_MNGR_T*      pt_mngr)
{
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*  pt_private;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    if (pt_private == NULL)
    {
        return MMSBTLR_INV_ARG;
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
UINT32 txt_timer_period(SBTL_PAGE_INST_MNGR_T*    pt_mngr)
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
INT32 txt_init(SBTL_PAGE_INST_MNGR_T*     pt_mngr)
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
INT32 txt_deinit(SBTL_PAGE_INST_MNGR_T*   pt_mngr)
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
VOID txt_start(SBTL_PAGE_INST_MNGR_T*     pt_mngr)
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
VOID txt_stop(SBTL_PAGE_INST_MNGR_T*      pt_mngr)
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
BOOL txt_parser_verify(SBTL_ENGINE_T*     pt_engine)
{
    UINT8   ui1_count;
    UINT8   ui1_text_type;
    BOOL    b_found;
    UINT32  ui4_time;
    INT32   i4_ret;
    UINT64  ui8_offset;
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

    ui1_count = 0;
    
    sbtl_feeder_setpos(pt_engine, 0, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
    
    while(1)
    {
        if(b_UTF16 == TRUE)
        {
            i4_ret = _txt_unit_start_p(pt_engine, MM_SBTL_ENCODE_UTF16, &b_found, &ui1_text_type, &ui4_time, MM_SBTL_PARSE_MAX_LENGTH);
        }
        else
        {
            i4_ret = _txt_unit_start_p(pt_engine, MM_SBTL_ENCODE_ASCI, &b_found, &ui1_text_type, &ui4_time, MM_SBTL_PARSE_MAX_LENGTH);
        }
        
        if (i4_ret != MMSBTLR_OK)
        {
            return FALSE;
        }

        if (b_found)
        {
            ui1_count++;
            if(ui1_count == 3)
            {
                return TRUE;
            }
        }
        else
        {
            sbtl_feeder_getpos(pt_engine, &ui8_offset, MM_SBTL_FEEDER_SUBTITLE);
            
            if(ui8_offset > 256)
            {
                return FALSE;
            }
        }
    }
}

