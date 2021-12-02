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
 * $RCSfile: sbtl_sami_parser.c,v $
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
#include "mutil/sbtl_engine/sbtl_widget.h"
#include "mutil/sbtl_engine/sbtl_text/sbtl_text_page_inst_mngr.h"
#include "mutil/sbtl_engine/sbtl_text/sbtl_sami_parser.h"
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
#include "../sbtl_widget.h"
#include "sbtl_text_page_inst_mngr.h"
#include "sbtl_sami_parser.h"
#include "u_mm_sbtl_engine.h"

#endif /*__KERNEL__*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define CARRIAGE_RETURN             0x0d
#define LINE_FEED                   0x0a
#define CHAR_TAB                    0x09
#define CHAR_SPACE                  0x20

#define PTS_FREQ        ((UINT64)(90000))
#define TIMER_PERIOD    500
#define CONTAINER_LEN   16
#define SAMI_CC_SZ      3
#define CONTENT_SIZE    256

#define SBTL_CLR_NAME_MAX_LEN     12


typedef struct _SAMI_PRIVATE_T
{
    UINT16          s_content_buf[CONTENT_SIZE];
    UINT8           s_text_buf[CONTENT_SIZE];
    UINT16          s_curr_class[CONTAINER_LEN];
    UINT8           ui1_total_stream_count;
    UINT8           ui1_stream_idx;
    SMAI_CC_INF_T   at_stream_info[SAMI_CC_SZ];
    BOOL            b_do_color;
}SAMI_PRIVATE_T;

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
static INT32 _sami_search_string(SBTL_ENGINE_T*     pt_engine, 
                                 CHAR*              s_string,
                                 BOOL               b_UTF16,
                                 BOOL*              pb_found,
                                 UINT8              ui1_whence,
                                 BOOL               b_back,
                                 UINT64*            pui8_offset);

static BOOL _sami_do_fnt_clr(SBTL_PAGE_INST_MNGR_T*     pt_mngr, BOOL b_UTF16);


static INT32 _sami_undo_fnt_clr(SBTL_PAGE_INST_MNGR_T*     pt_mngr);

/*-----------------------------------------------------------------------------
                    data declarations, extern, static, const
 ----------------------------------------------------------------------------*/
static CHAR s_sami_header[] = "<SAMI>";
static CHAR s_sync_header[] = "<SYNC";
static CHAR s_pclass_header[] = "<P Class";
static CHAR s_pclass_hdr_2[] = "<P>";
static CHAR s_br_header[] = "<br>";
static CHAR s_head_header[] = "HEAD>";
static CHAR s_body_header[] = "BODY>";
static CHAR s_comment_header[] = "<STYLE";
static CHAR s_comment_end_header[] = "</STYLE";
static CHAR s_fnt_clr_header[] = "<font color";

static CHAR s_clr_trans[] = "trans";
static CHAR s_clr_yellow[] = "yellow";
static CHAR s_clr_blue[] = "blue";
static CHAR s_clr_lgreen[] = "lightgreen";
static CHAR s_clr_black[] = "black";
static CHAR s_clr_violet[] = "violet";
static CHAR s_clr_white[] = "white";

typedef enum
{
    SBTL_SEARCH_BEGIN,
    SBTL_SEARCH_MIDDLE
} SBTL_SEARCH_TYPE_T;

typedef enum
{
    SBTL_CLR_TYPE_TRANS,
    SBTL_CLR_TYPE_WHITE,
    SBTL_CLR_TYPE_YELLOW,
    SBTL_CLR_TYPE_BLUE,
    SBTL_CLR_TYPE_LIGHTGREEN,
    SBTL_CLR_TYPE_BLACK,
    SBTL_CLR_TYPE_VOILET,
    SBTL_CLR_TYPE_LAST
} SBTL_CLR_TYPE_T;

static GL_COLOR_T at_sbtl_clr_tbl[SBTL_CLR_TYPE_LAST]={
    {255,   {   0 }, {   0 }, {   0 } },/*trans*/
    {255,   {  255}, {  255}, {  255} },/*white*/
    {255,   {  255}, {  255}, {    0} },/*yellow*/
    {255,   {   0 }, {   0 }, {  255} },/*blue*/
    {255,   {  128}, {  255}, {  128} },/*lightgreen*/
    {255,   {   0 }, {   0 }, {   0 } },/*black*/
    {255,   {  128}, {   0 }, {  255} },/*violet*/
}; 

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
static INT32 _sami_search_char(SBTL_ENGINE_T*     pt_engine, 
                                    UINT16             ui2_char,
                                    BOOL               b_UTF16,
                               BOOL*              pb_found)
{
    UINT8       ui1_tmp;
    INT32       i4_ret;
    UINT16      ui2_tmp;

    ASSERT(pt_engine, ("pt_engine=NULL\n"));
    
    do
    {
        if(b_UTF16 == TRUE)
        {
            i4_ret = sbtl_feeder_inpnbyte(pt_engine, 2, (UINT8*)&ui2_tmp, NULL, MM_SBTL_FEEDER_SUBTITLE);
        }
        else
        {
            i4_ret = sbtl_feeder_inp1byte(pt_engine, &ui1_tmp, MM_SBTL_FEEDER_SUBTITLE);
            ui2_tmp = ui1_tmp;
        }
        
        if (i4_ret != MMSBTLR_OK)
        {
            *pb_found = FALSE;
            return i4_ret;
        }
        
        if (ui2_tmp == ui2_char)
        {
            *pb_found = TRUE;
    
            return MMSBTLR_OK;
        }
    }while(1);
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
static INT32 _sami_search_string(SBTL_ENGINE_T*     pt_engine, 
                                      CHAR*              s_string,
                                      BOOL               b_UTF16,
                                      BOOL*              pb_found,
                                      UINT8              ui1_whence,
                                      BOOL               b_back,
                                      UINT64*            pui8_offset)
{
    UINT64      ui8_offset;
    UINT64      ui8_found_offset;
    UINT8       ui1_len;
    UINT8       ui1_idx;
    UINT8       ui1_tmp;
    INT32       i4_ret;
    UINT16      ui2_tmp;
    
    ASSERT(pt_engine, ("pt_engine=NULL\n"));

    ui1_len = (UINT8)x_strlen((const CHAR *)s_string);
    
    sbtl_feeder_getpos(pt_engine, &ui8_offset, MM_SBTL_FEEDER_SUBTITLE);

    if (ui1_whence == SBTL_SEARCH_BEGIN)
    {
        for (ui1_idx=0; ui1_idx<ui1_len; ui1_idx++)
        {
            if(b_UTF16 == TRUE)
            {
                i4_ret = sbtl_feeder_inpnbyte(pt_engine, 2, (UINT8*)&ui2_tmp, NULL, MM_SBTL_FEEDER_SUBTITLE);
            }
            else
            {
                i4_ret = sbtl_feeder_inp1byte(pt_engine, &ui1_tmp, MM_SBTL_FEEDER_SUBTITLE);
                ui2_tmp = ui1_tmp;
            }

            if (i4_ret != MMSBTLR_OK)
            {
                sbtl_feeder_setpos(pt_engine, (INT64)ui8_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
                *pb_found = FALSE;
                return i4_ret;
            }
            
            if (ui2_tmp != (UINT16)s_string[ui1_idx])
            {
                sbtl_feeder_setpos(pt_engine, (INT64)ui8_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
                *pb_found = FALSE;

                return MMSBTLR_NOT_FOUND;
            }
        }
        
        if (pui8_offset)
        {
            sbtl_feeder_getpos(pt_engine, &ui8_found_offset, MM_SBTL_FEEDER_SUBTITLE);
            *pui8_offset = (b_UTF16 == TRUE) ? (ui8_found_offset - ui1_len * 2) : (ui8_found_offset - ui1_len);
        }

        if (b_back)
        {
            sbtl_feeder_setpos(pt_engine, (INT64)ui8_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
        }
        
        *pb_found = TRUE;
        return MMSBTLR_OK;

    }
    else if (ui1_whence == SBTL_SEARCH_MIDDLE)
    {
        do
        {
            if(b_UTF16 == TRUE)
            {
                i4_ret = sbtl_feeder_inpnbyte(pt_engine, 2, (UINT8*)&ui2_tmp, NULL, MM_SBTL_FEEDER_SUBTITLE);
            }
            else
            {
                i4_ret = sbtl_feeder_inp1byte(pt_engine, &ui1_tmp, MM_SBTL_FEEDER_SUBTITLE);
                ui2_tmp = ui1_tmp;
            }
            
            if (i4_ret != MMSBTLR_OK)
            {
                sbtl_feeder_setpos(pt_engine, (INT64)ui8_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
                *pb_found = FALSE;
                return i4_ret;
            }

            if (ui2_tmp == (UINT16)s_string[0])
            {
                i4_ret = _sami_search_string(pt_engine, 
                                             &s_string[1],
                                             b_UTF16,
                                             pb_found,
                                             SBTL_SEARCH_BEGIN,
                                             FALSE,
                                             &ui8_found_offset);
                
                if (i4_ret!=MMSBTLR_OK && i4_ret!=MMSBTLR_NOT_FOUND)
                {
                    sbtl_feeder_setpos(pt_engine, (INT64)ui8_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
                    *pb_found = FALSE;
                    return i4_ret;
                }

                if (*pb_found == TRUE)
                {   
                    if (pui8_offset)
                    {
                        *pui8_offset = (b_UTF16 == TRUE) ? (ui8_found_offset-2) : (ui8_found_offset-1);
                    }
                    if (b_back)
                    {
                        sbtl_feeder_setpos(pt_engine, (INT64)ui8_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
                    }
                    return MMSBTLR_OK;
                }
            }
        }while(1);
    }
    else
    {
        *pb_found = FALSE;
        return MMSBTLR_NOT_SUPPORT;
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
static INT32 _sami_search_string_p(SBTL_ENGINE_T*     pt_engine, 
                                      CHAR*              s_string,
                                      BOOL               b_UTF16,
                                      BOOL*              pb_found,
                                      UINT8              ui1_whence,
                                      BOOL               b_back,
                                      UINT64*            pui8_offset,
                                      UINT64             ui8_parse_size)
{
    UINT64      ui8_offset;
    UINT64      ui8_offset1;
    UINT64      ui8_found_offset;
    UINT8       ui1_len;
    UINT8       ui1_idx;
    UINT8       ui1_tmp;
    INT32       i4_ret;
    UINT16      ui2_tmp;
    
    ASSERT(pt_engine, ("pt_engine=NULL\n"));

    ui1_len = (UINT8)x_strlen((const CHAR *)s_string);
    
    sbtl_feeder_getpos(pt_engine, &ui8_offset, MM_SBTL_FEEDER_SUBTITLE);
    if(ui8_offset > ui8_parse_size)
    {
        *pb_found = FALSE;
        return MMSBTLR_OK;
    }

    if (ui1_whence == SBTL_SEARCH_BEGIN)
    {
        for (ui1_idx=0; ui1_idx<ui1_len; ui1_idx++)
        {
            if(b_UTF16 == TRUE)
            {
                i4_ret = sbtl_feeder_inpnbyte(pt_engine, 2, (UINT8*)&ui2_tmp, NULL, MM_SBTL_FEEDER_SUBTITLE);
            }
            else
            {
                i4_ret = sbtl_feeder_inp1byte(pt_engine, &ui1_tmp, MM_SBTL_FEEDER_SUBTITLE);
                ui2_tmp = ui1_tmp;
            }

            if (i4_ret != MMSBTLR_OK)
            {
                sbtl_feeder_setpos(pt_engine, (INT64)ui8_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
                *pb_found = FALSE;
                return i4_ret;
            }
            
            if (ui2_tmp != (UINT16)s_string[ui1_idx])
            {
                sbtl_feeder_setpos(pt_engine, (INT64)ui8_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
                *pb_found = FALSE;

                return MMSBTLR_NOT_FOUND;
            }
        }
        
        if (pui8_offset)
        {
            sbtl_feeder_getpos(pt_engine, &ui8_found_offset, MM_SBTL_FEEDER_SUBTITLE);
            *pui8_offset = (b_UTF16 == TRUE) ? (ui8_found_offset - ui1_len * 2) : (ui8_found_offset - ui1_len);
        }

        if (b_back)
        {
            sbtl_feeder_setpos(pt_engine, (INT64)ui8_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
        }
        
        *pb_found = TRUE;
        return MMSBTLR_OK;

    }
    else if (ui1_whence == SBTL_SEARCH_MIDDLE)
    {
        do
        {
            sbtl_feeder_getpos(pt_engine, &ui8_offset1, MM_SBTL_FEEDER_SUBTITLE);
            if(ui8_offset1 > ui8_parse_size)
            {
                *pb_found = FALSE;
                return MMSBTLR_OK;
            }
            
            if(b_UTF16 == TRUE)
            {
                i4_ret = sbtl_feeder_inpnbyte(pt_engine, 2, (UINT8*)&ui2_tmp, NULL, MM_SBTL_FEEDER_SUBTITLE);
            }
            else
            {
                i4_ret = sbtl_feeder_inp1byte(pt_engine, &ui1_tmp, MM_SBTL_FEEDER_SUBTITLE);
                ui2_tmp = ui1_tmp;
            }
            
            if (i4_ret != MMSBTLR_OK)
            {
                sbtl_feeder_setpos(pt_engine, (INT64)ui8_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
                *pb_found = FALSE;
                return i4_ret;
            }

            if (ui2_tmp == (UINT16)s_string[0])
            {
                i4_ret = _sami_search_string_p(pt_engine, 
                                             &s_string[1],
                                             b_UTF16,
                                             pb_found,
                                             SBTL_SEARCH_BEGIN,
                                             FALSE,
                                             &ui8_found_offset,
                                             ui8_parse_size);
                
                if (i4_ret!=MMSBTLR_OK && i4_ret!=MMSBTLR_NOT_FOUND)
                {
                    sbtl_feeder_setpos(pt_engine, (INT64)ui8_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
                    *pb_found = FALSE;
                    return i4_ret;
                }

                if (*pb_found == TRUE)
                {   
                    if (pui8_offset)
                    {
                        *pui8_offset = (b_UTF16 == TRUE) ? (ui8_found_offset-2) : (ui8_found_offset-1);
                    }
                    if (b_back)
                    {
                        sbtl_feeder_setpos(pt_engine, (INT64)ui8_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
                    }
                    return MMSBTLR_OK;
                }
            }
        }while(1);
    }
    else
    {
        *pb_found = FALSE;
        return MMSBTLR_NOT_SUPPORT;
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
static INT32 _sami_get_pts(SBTL_PAGE_INST_MNGR_T*       pt_mngr,
                           BOOL                         b_UTF16,
                           PTS_T*                       pt_pts)
{
    SBTL_ENGINE_T*      pt_engine;
    UINT8               ui1_tmp;
    UINT8               ui1_idx;
    UINT32              ui4_number;
    INT32               i4_ret;
    UINT16              ui2_tmp;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));

    ui1_idx = 0;
    ui4_number = 0;
    pt_engine = pt_mngr->pt_engine;
    
    do
    {
        if(b_UTF16 == TRUE)
        {
            i4_ret = sbtl_feeder_inpnbyte(pt_engine, 2, (UINT8*)&ui2_tmp, NULL, MM_SBTL_FEEDER_SUBTITLE);
        }
        else
        {
        i4_ret = sbtl_feeder_inp1byte(pt_engine, &ui1_tmp, MM_SBTL_FEEDER_SUBTITLE);
            ui2_tmp = ui1_tmp;
        }    
        
        if (i4_ret != MMSBTLR_OK)
        {
            return i4_ret;
        }

        if (ui2_tmp >= (UINT16)'0' && ui2_tmp <= (UINT16)'9')
        {
            ui4_number = ui4_number * 10 + (ui2_tmp - (UINT16)'0');
        }
        else if (0 == ui1_idx)
        {
            return MMSBTLR_NOT_FOUND;
        }
        else
        {
            break;
        }
        
        ui1_idx++;
    }while(1);
    
    *pt_pts = ui4_number * (PTS_FREQ / 1000);

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
static INT32 _sami_class_name(SBTL_PAGE_INST_MNGR_T*    pt_mngr, BOOL b_UTF16)
{
    INT32                               i4_ret;
    UINT8                               ui1_tmp;
    UINT8                               ui1_idx;
    SBTL_ENGINE_T*                      pt_engine;
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;
    SAMI_PRIVATE_T*                     pt_sami_private;
    UINT16                              ui2_tmp;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));

    pt_engine = pt_mngr->pt_engine;
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_engine, ("pt_engine=NULL\n"));
    ASSERT(pt_private, ("pt_private=NULL\n"));

    pt_sami_private = (SAMI_PRIVATE_T*)pt_private->pv_data;
    ASSERT(pt_sami_private, ("pt_sami_private=NULL\n"));

    ui1_idx = 0;
    
    do
    {
        if(b_UTF16 == TRUE)
        {
            i4_ret = sbtl_feeder_inpnbyte(pt_engine, 2, (UINT8*)&ui2_tmp, NULL, MM_SBTL_FEEDER_SUBTITLE);
        }
        else
        {
            i4_ret = sbtl_feeder_inp1byte(pt_engine, &ui1_tmp, MM_SBTL_FEEDER_SUBTITLE);
            ui2_tmp = ui1_tmp;
        }  
        
        if (i4_ret != MMSBTLR_OK)
        {
            return i4_ret;
        }

        if (ui1_idx >= CONTAINER_LEN)
        {
            pt_sami_private->s_curr_class[ui1_idx-1] = 0;
            return MMSBTLR_OK;
        }
        
        if (ui2_tmp == (UINT16)'>')
        {
            pt_sami_private->s_curr_class[ui1_idx] = 0;
            return MMSBTLR_OK;
        }

        sbtl_trans_to_w2s(pt_private->e_language, ui2_tmp, &pt_sami_private->s_curr_class[ui1_idx]);

        ui1_idx++;
    }while(1);
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
static BOOL _sami_class_cmp(SBTL_PAGE_INST_MNGR_T*      pt_mngr)
{
    INT32                               i4_ret;
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;
    SAMI_PRIVATE_T*                     pt_sami_private;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("pt_private=NULL\n"));

    pt_sami_private = (SAMI_PRIVATE_T*)pt_private->pv_data;
    ASSERT(pt_sami_private, ("pt_sami_private=NULL\n"));
    
    if (pt_sami_private->ui1_total_stream_count == 1)
    {
        return TRUE;
    }

    if (pt_sami_private->ui1_stream_idx >= SAMI_CC_SZ)
    {
        return FALSE;
    }
    
    i4_ret = x_strcmp((const CHAR *)pt_sami_private->s_curr_class, 
                      (const CHAR *)pt_sami_private->at_stream_info[pt_sami_private->ui1_stream_idx].s_class_name);
    
    if (i4_ret == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
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
static BOOL _sami_next_rdy(SBTL_PAGE_INST_MNGR_T*       pt_mngr, BOOL b_UTF16)
{
    SBTL_ENGINE_T*      pt_engine;
    UINT64              ui8_offset;
    BOOL                b_found;
    INT32               i4_ret;
    
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));

    pt_engine = pt_mngr->pt_engine;
    ASSERT(pt_engine, ("pt_engine=NULL\n"));
    
    sbtl_feeder_getpos(pt_engine, &ui8_offset, MM_SBTL_FEEDER_SUBTITLE);

    i4_ret = _sami_search_string(pt_engine, 
                                 s_sync_header,
                                 b_UTF16,
                                 &b_found,
                                 SBTL_SEARCH_MIDDLE,
                                 FALSE,
                                 NULL);
    
    if (MMSBTLR_OK==i4_ret && b_found)
    {
        i4_ret = _sami_search_string(pt_engine, 
                                     s_sync_header,
                                     b_UTF16,
                                     &b_found,
                                     SBTL_SEARCH_MIDDLE,
                                     FALSE,
                                     NULL);
        
        if (MMSBTLR_OK==i4_ret && b_found)
        {
            sbtl_feeder_setpos(pt_engine, (INT64)ui8_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
        }
        else
        {
            b_found = FALSE;
        }
    }
    else
    {
        b_found = FALSE;
    }
    
    return b_found;
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
static INT32 _sami_next_unit(SBTL_PAGE_INST_MNGR_T*         pt_mngr,
                             BOOL                           b_UTF16,
                             PTS_T*                         pt_pts)
{
    SBTL_ENGINE_T*      pt_engine;
    BOOL                b_found;
    INT32               i4_ret;
    
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));

    pt_engine = pt_mngr->pt_engine;
    ASSERT(pt_engine, ("pt_engine=NULL\n"));

    i4_ret = _sami_search_string(pt_engine, 
                                 s_sync_header,
                                 b_UTF16,
                                 &b_found,
                                 SBTL_SEARCH_MIDDLE,
                                 FALSE,
                                 NULL);

    if (MMSBTLR_OK==i4_ret)
    {
        if (b_found)
        {
        }
        else
        {
            return MMSBTLR_NOT_FOUND;
        }
    }
    else
    {
        return i4_ret;
    }

    i4_ret = _sami_search_char(pt_engine, 
                               (UINT16)'=',
                               b_UTF16,
                               &b_found);
    if (MMSBTLR_OK==i4_ret)
    {
        if (b_found)
        {
        }
        else
        {
            return MMSBTLR_NOT_FOUND;
        }
    }
    else
    {
        return i4_ret;
    }

    i4_ret = _sami_get_pts(pt_mngr, b_UTF16, pt_pts);

    return i4_ret;
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
static BOOL _sami_find_class_until_sync(SBTL_PAGE_INST_MNGR_T* pt_mngr, BOOL b_UTF16)
{
    BOOL                                b_found;
    INT32                               i4_ret;
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;
    SAMI_PRIVATE_T*                     pt_sami_private;
    SBTL_ENGINE_T*                      pt_engine;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));

    pt_engine = pt_mngr->pt_engine;
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_engine, ("pt_engine=NULL\n"));
    ASSERT(pt_private, ("pt_private=NULL\n"));

    pt_sami_private = (SAMI_PRIVATE_T*)pt_private->pv_data;
    ASSERT(pt_sami_private, ("pt_sami_private=NULL\n"));

    if (pt_sami_private->ui1_total_stream_count == 1)
    {
        /* GIEC's amelie_cafe.smi has no <P> after <SYNC>,
               so we shouldn't search <P.*/
               
        return (TRUE);
    }
    
    while (1)
    {
        i4_ret = _sami_search_char(pt_engine, 
                                   (UINT16)'<',
                                   b_UTF16,
                                   &b_found);

        if (i4_ret != MMSBTLR_OK || !b_found)
        {
            return FALSE;
        }

        if(b_UTF16 == TRUE)
        {
            sbtl_feeder_setpos(pt_engine, -2, MEDIA_SEEK_CUR, MM_SBTL_FEEDER_SUBTITLE);
        }
        else
        {
        sbtl_feeder_setpos(pt_engine, -1, MEDIA_SEEK_CUR, MM_SBTL_FEEDER_SUBTITLE);
        }
        
        i4_ret = _sami_search_string(pt_engine, 
                                 s_sync_header,
                                 b_UTF16,
                                 &b_found,
                                 SBTL_SEARCH_BEGIN,
                                 TRUE,
                                 NULL);
    
        if (MMSBTLR_OK==i4_ret && b_found)
        {
            return FALSE;
        }
        else if(MMSBTLR_EOT == i4_ret)
        {
            return FALSE;
        }
        else
        {
            i4_ret = _sami_search_string(pt_engine, 
                                         s_pclass_header,
                                         b_UTF16,
                                         &b_found,
                                         SBTL_SEARCH_BEGIN,
                                         TRUE,
                                         NULL);
                
            if (MMSBTLR_OK==i4_ret && b_found)
            {
                i4_ret = _sami_search_char(pt_engine, 
                                           (UINT16)'=',
                                           b_UTF16,
                                           &b_found);
        
                if (i4_ret != MMSBTLR_OK)
                {
                    return FALSE;
                }
                
                i4_ret = _sami_class_name(pt_mngr, b_UTF16);  /* looking for the class name*/
                if (i4_ret != MMSBTLR_OK)
                {
                    return FALSE;
                }
                
                if (_sami_class_cmp(pt_mngr))
                {
                    return TRUE;
                }

            }
            else if(MMSBTLR_NOT_FOUND == i4_ret)
            {
                i4_ret = _sami_search_string(pt_engine, 
                                             s_pclass_hdr_2,
                                             b_UTF16,
                                             &b_found,
                                             SBTL_SEARCH_BEGIN,
                                             TRUE,
                                             NULL);
                if (MMSBTLR_OK==i4_ret && b_found)
                {
                    return TRUE;
                }
            }
            else if(MMSBTLR_EOT == i4_ret)
            {
                return FALSE;
            }
        }

        if(b_UTF16 == TRUE)
        {
            sbtl_feeder_setpos(pt_engine, 2, MEDIA_SEEK_CUR, MM_SBTL_FEEDER_SUBTITLE);
        }
        else
        {
            sbtl_feeder_setpos(pt_engine, 1, MEDIA_SEEK_CUR, MM_SBTL_FEEDER_SUBTITLE);
        }
    }

}

static BOOL _sami_do_fnt_clr(SBTL_PAGE_INST_MNGR_T*     pt_mngr, BOOL b_UTF16)
{
    UINT8                   ui1_tmp;
    INT32                   i4_ret;
    SBTL_ENGINE_T*          pt_engine;
    UINT16                  ui2_tmp;
    BOOL                    b_found = FALSE;
    UINT16                  ui2_i = 0;
    UINT16                  aui2_clr_name[SBTL_CLR_NAME_MAX_LEN];
    SBTL_CLR_TYPE_T         e_found_clr = SBTL_CLR_TYPE_LAST;
    WGL_COLOR_INFO_T        t_color;    
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;
    
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    pt_engine = pt_mngr->pt_engine;
    ASSERT(pt_engine, ("pt_engine=NULL\n"));
    ASSERT(pt_private, ("pt_private=NULL\n"));

    i4_ret = _sami_search_char(pt_engine, 
                              (UINT8)'=',
                              b_UTF16,
                             &b_found);
    
    if (i4_ret == MMSBTLR_EOT || !b_found)
    {
        return FALSE;
    }

    x_memset(aui2_clr_name,  0 , sizeof(UINT16)*SBTL_CLR_NAME_MAX_LEN);
    do{
          if(b_UTF16 == TRUE)
          {
              i4_ret = sbtl_feeder_inpnbyte(pt_engine, 2, (UINT8*)&ui2_tmp, NULL, MM_SBTL_FEEDER_SUBTITLE);
          }
          else
          {
              i4_ret = sbtl_feeder_inp1byte(pt_engine, &ui1_tmp, MM_SBTL_FEEDER_SUBTITLE);
              ui2_tmp = ui1_tmp;
          }
        
          if (i4_ret != MMSBTLR_OK)
          {
              return FALSE;
          }

          if(ui2_tmp == '>')
          {
               if(b_UTF16 == TRUE)
               {
                   sbtl_feeder_setpos(pt_engine, -2, MEDIA_SEEK_CUR, MM_SBTL_FEEDER_SUBTITLE);
               }
               else
               {
                   sbtl_feeder_setpos(pt_engine, -1, MEDIA_SEEK_CUR, MM_SBTL_FEEDER_SUBTITLE);
               }
               break;
          }
          else if(ui2_i == '=')
          {
                ui2_i = 0;
                x_memset(aui2_clr_name,  0 , sizeof(UINT16)*SBTL_CLR_NAME_MAX_LEN);
          }
          else if(ui2_i < SBTL_CLR_NAME_MAX_LEN)
          {
               aui2_clr_name[ui2_i++] = ui2_tmp;
          }
          else
          {
               break;
          }
     }while(1);


     if(sbtl_cmp_string(aui2_clr_name, ui2_i, s_clr_white)==TRUE)
     {
         e_found_clr = SBTL_CLR_TYPE_WHITE;
     }
     else if(sbtl_cmp_string(aui2_clr_name, ui2_i, s_clr_yellow)==TRUE)
     {
         e_found_clr = SBTL_CLR_TYPE_YELLOW;
     }
     else if(sbtl_cmp_string(aui2_clr_name, ui2_i, s_clr_blue)==TRUE)
     {
         e_found_clr = SBTL_CLR_TYPE_BLUE;
     }
     else if(sbtl_cmp_string(aui2_clr_name, ui2_i, s_clr_lgreen)==TRUE)
     {
         e_found_clr = SBTL_CLR_TYPE_LIGHTGREEN;
     }
     else if(sbtl_cmp_string(aui2_clr_name, ui2_i, s_clr_black)==TRUE)
     {
         e_found_clr = SBTL_CLR_TYPE_BLACK;
     }
     else if(sbtl_cmp_string(aui2_clr_name, ui2_i, s_clr_violet)==TRUE)
     {
         e_found_clr = SBTL_CLR_TYPE_VOILET;
     }
     else if(sbtl_cmp_string(aui2_clr_name, ui2_i, s_clr_trans)==TRUE)
     {
         e_found_clr = SBTL_CLR_TYPE_TRANS;
     }
            
     x_memset(&t_color, 0, sizeof(WGL_COLOR_INFO_T));
     if(e_found_clr != SBTL_CLR_TYPE_LAST)
     {
         t_color.e_type = MM_SBTL_DFT_FG_TYPE;
         x_memcpy ((VOID *)&t_color.u_color_data.t_standard.t_enable, 
                 & at_sbtl_clr_tbl[e_found_clr], 
                 sizeof(GL_COLOR_T));

         x_memcpy ((VOID *)&t_color.u_color_data.t_standard.t_disable, 
                   &at_sbtl_clr_tbl[e_found_clr], 
                   sizeof(GL_COLOR_T));

         x_memcpy ((VOID *)&t_color.u_color_data.t_standard.t_highlight, 
                   &at_sbtl_clr_tbl[e_found_clr], 
                   sizeof(GL_COLOR_T));

         i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                                SBTL_WGL_CMD_GL_SET_COLOR,
                                (VOID*)SBTL_WGL_CLR_TEXT, /* text color */
                                (VOID*)&t_color);
         if (MMSBTLR_OK == i4_ret)
         {
            return TRUE;
         }
     }  
    return FALSE;
}

static INT32 _sami_undo_fnt_clr(SBTL_PAGE_INST_MNGR_T*     pt_mngr)
{
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;
    INT32                               i4_ret; 
    
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
  
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("pt_private=NULL\n"));
    
    i4_ret = sbtl_wgl_do_cmd (pt_private->h_sbtl_wgl,
                           SBTL_WGL_CMD_GL_SET_COLOR,
                           (VOID*)SBTL_WGL_CLR_TEXT, /* text color */
                           (VOID*)&pt_mngr->pt_engine->t_cfg.t_fg_color_info);
    
    if (MMSBTLR_OK != i4_ret)
    {
       return (MMSBTLR_WGL_FAIL);
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
static VOID _sami_escape_useless(SBTL_PAGE_INST_MNGR_T*     pt_mngr, BOOL b_UTF16)
{
    UINT8                   ui1_tmp;
    INT32                   i4_ret;
    SBTL_ENGINE_T*          pt_engine;
    UINT16                  ui2_tmp;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    
    pt_engine = pt_mngr->pt_engine;
    ASSERT(pt_engine, ("pt_engine=NULL\n"));

    while(1)
    {
        if(b_UTF16 == TRUE)
        {
            i4_ret = sbtl_feeder_inpnbyte(pt_engine, 2, (UINT8*)&ui2_tmp, NULL, MM_SBTL_FEEDER_SUBTITLE);
        }
        else
        {
            i4_ret = sbtl_feeder_inp1byte(pt_engine, &ui1_tmp, MM_SBTL_FEEDER_SUBTITLE);
            ui2_tmp = ui1_tmp;
        }
        
        if (i4_ret != MMSBTLR_OK)
        {
            break;
        }
        
        if(!((ui2_tmp == CARRIAGE_RETURN) || (ui2_tmp == LINE_FEED) || (ui2_tmp == ' ') || (ui2_tmp == CHAR_TAB)))
        {
            if (b_UTF16 == TRUE)
            {
                sbtl_feeder_setpos(pt_engine, -2, MEDIA_SEEK_CUR, MM_SBTL_FEEDER_SUBTITLE);
            }
            else
            {
                sbtl_feeder_setpos(pt_engine, -1, MEDIA_SEEK_CUR, MM_SBTL_FEEDER_SUBTITLE);
            }
            break;
        }
    }

    return;
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
static VOID _sami_content(SBTL_PAGE_INST_MNGR_T*    pt_mngr, BOOL b_UTF16)
{
    BOOL                                b_found;
    UINT8                               ui1_tmp;
    INT32                               i4_ret;
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;
    SAMI_PRIVATE_T*                     pt_sami_private;
    SBTL_ENGINE_T*                      pt_engine;
    UINT32                              ui4_idx;
    UINT16                              ui2_tmp;
    MM_SBTL_ENCODING_T                  e_encoding;
    
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    
    pt_engine = pt_mngr->pt_engine;
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_engine, ("pt_engine=NULL\n"));
    ASSERT(pt_private, ("pt_private=NULL\n"));
    
    e_encoding = pt_mngr->pt_engine->t_cfg.e_encoding;
    pt_sami_private = (SAMI_PRIVATE_T*)pt_private->pv_data;
    ASSERT(pt_sami_private, ("pt_sami_private=NULL\n"));

    ui4_idx = 0;

    pt_sami_private->b_do_color = FALSE;
    while(1)
    {
        if(b_UTF16 == TRUE)
        {
            i4_ret = sbtl_feeder_inpnbyte(pt_engine, 2, (UINT8*)&ui2_tmp, NULL, MM_SBTL_FEEDER_SUBTITLE);
        }
        else
        {
        i4_ret = sbtl_feeder_inp1byte(pt_engine, &ui1_tmp, MM_SBTL_FEEDER_SUBTITLE);
            ui2_tmp = ui1_tmp;
        }
        
        if (i4_ret != MMSBTLR_OK)
        {
            break;
        }
        
        if (ui2_tmp == '<')
        {
            if (b_UTF16 == TRUE)
            {
                sbtl_feeder_setpos(pt_engine, -2, MEDIA_SEEK_CUR, MM_SBTL_FEEDER_SUBTITLE);
            }
            else
            {
                sbtl_feeder_setpos(pt_engine, -1, MEDIA_SEEK_CUR, MM_SBTL_FEEDER_SUBTITLE);
            }
            
            i4_ret = _sami_search_string(pt_engine, 
                                         s_sync_header,
                                         b_UTF16,
                                         &b_found,
                                         SBTL_SEARCH_BEGIN,
                                         TRUE,
                                         NULL);
                
            if (MMSBTLR_OK==i4_ret && b_found)
            {
                if(b_UTF16 == TRUE)
                {
                    pt_sami_private->s_content_buf[ui4_idx] = 0;
                }
                else
                {
                    pt_sami_private->s_text_buf[ui4_idx] = 0;
                }
                
                break;
            }
            else if(MMSBTLR_EOT == i4_ret)
            {
                if(b_UTF16 == TRUE)
                {
                    pt_sami_private->s_content_buf[ui4_idx] = 0;
                }
                else
                {
                    pt_sami_private->s_text_buf[ui4_idx] = 0;
                }
                break;
            }

            i4_ret = _sami_search_string(pt_engine, 
                                         s_br_header,
                                         b_UTF16,
                                         &b_found,
                                         SBTL_SEARCH_BEGIN,
                                         TRUE,
                                         NULL);
                
            if (MMSBTLR_OK==i4_ret && b_found)
            {
                if (b_UTF16 == TRUE)
                {
                    sbtl_feeder_setpos(pt_engine, 8, MEDIA_SEEK_CUR, MM_SBTL_FEEDER_SUBTITLE);
                    pt_sami_private->s_content_buf[ui4_idx++] = (UINT16)'<';
                    pt_sami_private->s_content_buf[ui4_idx++] = (UINT16)'b';
                    pt_sami_private->s_content_buf[ui4_idx++] = (UINT16)'r';
                    pt_sami_private->s_content_buf[ui4_idx++] = (UINT16)'>';
                }
                else
                {
                    sbtl_feeder_setpos(pt_engine, 4, MEDIA_SEEK_CUR, MM_SBTL_FEEDER_SUBTITLE);
                    pt_sami_private->s_text_buf[ui4_idx++] = (UINT8)'<';
                    pt_sami_private->s_text_buf[ui4_idx++] = (UINT8)'b';
                    pt_sami_private->s_text_buf[ui4_idx++] = (UINT8)'r';
                    pt_sami_private->s_text_buf[ui4_idx++] = (UINT8)'>';
                }            
                
                
                continue;
            }
            else if(MMSBTLR_EOT == i4_ret)
            {
                if(b_UTF16 == TRUE)
                {
                    pt_sami_private->s_content_buf[ui4_idx] = 0;
                }
                else
                {
                    pt_sami_private->s_text_buf[ui4_idx] = 0;
                }
                break;
            }

            i4_ret = _sami_search_string(pt_engine, 
                                         s_pclass_header,
                                         b_UTF16,
                                         &b_found,
                                         SBTL_SEARCH_BEGIN,
                                         TRUE,
                                         NULL);
                
            if(MMSBTLR_EOT == i4_ret)
            {
                if(b_UTF16 == TRUE)
                {
                    pt_sami_private->s_content_buf[ui4_idx] = 0;
                }
                else
                {
                    pt_sami_private->s_text_buf[ui4_idx] = 0;
                }
                break;
            }
            else if (b_found && pt_sami_private->ui1_total_stream_count>1)
            {
                if(b_UTF16 == TRUE)
                {
                    pt_sami_private->s_content_buf[ui4_idx] = 0;
                }
                else
                {
                    pt_sami_private->s_text_buf[ui4_idx] = 0;
                }
                break;
            }
            else if (b_found == TRUE)
            {
                _sami_search_char(pt_engine, 
                                  (UINT8)'>',
                                  b_UTF16,
                                  &b_found);
                continue;
            }

            i4_ret = _sami_search_string(pt_engine, 
                                 s_fnt_clr_header,
                                 b_UTF16,
                                 &b_found,
                                 SBTL_SEARCH_BEGIN,
                                 FALSE,
                                 NULL);
    
            if (MMSBTLR_OK==i4_ret && b_found)
            {
                pt_sami_private->b_do_color = _sami_do_fnt_clr(pt_mngr,b_UTF16);
            }
            else if(MMSBTLR_EOT == i4_ret)
            {
                if(b_UTF16 == TRUE)
                {
                    pt_sami_private->s_content_buf[ui4_idx] = 0;
                }
                else
                {
                    pt_sami_private->s_text_buf[ui4_idx] = 0;
                }
                break;
            }
 
            _sami_search_char(pt_engine, 
                             (UINT8)'>',
                             b_UTF16,
                             &b_found);
            continue;
 
        }
        else
        {
            if (ui4_idx>=CONTENT_SIZE-1)
            {
                if(b_UTF16 == TRUE)
                {
                    pt_sami_private->s_content_buf[ui4_idx] = 0;
                }
                else
                {
                    pt_sami_private->s_text_buf[ui4_idx] = 0;
                }
                break;
            }

            if(b_UTF16 == TRUE)
            {
                pt_sami_private->s_content_buf[ui4_idx++] = ui2_tmp;
            }
            else
            {
                pt_sami_private->s_text_buf[ui4_idx++] = ui1_tmp;
            }

            //sbtl_trans_to_w2s(pt_private->e_language, ui2_tmp, &pt_sami_private->s_content_buf[ui4_idx++]);
        }
    }
    
    if(b_UTF16 == FALSE)
    {
        sbtl_trans_to_w2s_ex(e_encoding,pt_sami_private->s_text_buf,CONTENT_SIZE,pt_sami_private->s_content_buf);
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
static VOID _sami_get_data(SBTL_PAGE_INST_MNGR_T*   pt_mngr, BOOL b_UTF16)
{
    SAMI_PRIVATE_T*                     pt_sami_private;
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));

    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("pt_private=NULL\n"));

    pt_sami_private = (SAMI_PRIVATE_T*)pt_private->pv_data;
    ASSERT(pt_sami_private, ("pt_sami_private=NULL\n"));

    _sami_escape_useless(pt_mngr, b_UTF16);
    _sami_content(pt_mngr, b_UTF16);

    if(pt_sami_private->b_do_color == FALSE)
    {
        _sami_undo_fnt_clr(pt_mngr);
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
static BOOL _sami_strcmp(UINT16* s_str1, UINT8* s_str2, UINT8 ui1_len)
{
    UINT8 ui1_idx;

    for (ui1_idx=0; ui1_idx<ui1_len; ui1_idx++)
    {
        if (s_str1[ui1_idx] != (UINT16)s_str2[ui1_idx])
        {
            return FALSE;
        }
    }
    
    return TRUE;
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
static VOID _sami_parse(SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
#define SMI_IGNORE_LTGT 1

    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;
    SAMI_PRIVATE_T*                     pt_sami_private;
    UINT32                              ui4_idx;
    UINT32                              ui4_dest_idx;
    UINT32                              ui4_text_len;
    UINT16*                             s_src;
    UINT16*                             s_dest;
    BOOL                                b_nbsp;
#if SMI_IGNORE_LTGT  
    BOOL                                b_ltgt;
#endif

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));

    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("pt_private=NULL\n"));

    pt_sami_private = (SAMI_PRIVATE_T*)pt_private->pv_data;
    ASSERT(pt_sami_private, ("pt_sami_private=NULL\n"));

    ui4_idx = 0;
    ui4_dest_idx = 0;
    b_nbsp = FALSE;
    
#if SMI_IGNORE_LTGT 
    b_ltgt = FALSE;
#endif

    ui4_text_len = sbtl_get_utf16_string_len(pt_sami_private->s_content_buf, CONTENT_SIZE);

    s_src = pt_sami_private->s_content_buf;
    s_dest = pt_private->w2s_text;
    
    ui4_idx = 0;
    while (ui4_idx < ui4_text_len)
    {
        if ((s_src[ui4_idx] == CARRIAGE_RETURN) && (s_src[ui4_idx+1] == LINE_FEED))
        {
            ui4_idx = ui4_idx + 2;   /* skip the CARRIAGE_RETURN & LINE_FEED */
            /* nothing to save here */
        }
#ifdef MORE_CHANGE_LINE_SYMBOL
        else if (s_src[ui4_idx] == CARRIAGE_RETURN || s_src[ui4_idx] == LINE_FEED)
        {
            ui4_idx++;
        }
#endif
        else if ((ui4_idx <= (ui4_text_len - 4)) && (s_src[ui4_idx] == (UINT8)'<') && (s_src[ui4_idx + 1] == (UINT8)'b') && (s_src[ui4_idx + 2] == (UINT8)'r') && (s_src[ui4_idx + 3] == (UINT8)'>'))
        {
            ui4_idx = ui4_idx + 4;
            s_dest[ui4_dest_idx++] = LINE_FEED;
            /* change this content into CARRIAGE RETURN + LINE_FEED */
        }
        else if (((ui4_idx + 6) <= ui4_text_len) && _sami_strcmp(&(s_src[ui4_idx]), (UINT8 *)"&nbsp;", 6))
        {
            s_dest[ui4_dest_idx++] = (UINT8)' ';
            ui4_idx = ui4_idx + 6;    /* &nbsp; */
            b_nbsp = TRUE;
        }
        else if (((ui4_idx + 4) <= ui4_text_len) && _sami_strcmp(&(s_src[ui4_idx]), (UINT8 *)"&lt;", 4))
        {
#if SMI_IGNORE_LTGT  
            b_ltgt = TRUE; /* Set it true to ignore the char until file the "&gt;" */
#else
            s_dest[ui4_dest_idx++] = (UINT8)'<'; /* replace with '<' char */
#endif
            ui4_idx += 4;
        }
        else if (((ui4_idx + 4) <= ui4_text_len) && _sami_strcmp(&(s_src[ui4_idx]), (UINT8 *)"&gt;", 4))
        {
#if SMI_IGNORE_LTGT  
            b_ltgt = FALSE; /* Set it false as the ending for matching "&lt;" */
#else
            s_dest[ui4_dest_idx++] = (UINT8)'>'; /* replace with '>' char */
#endif
            ui4_idx += 4;
        }
        else if (((ui4_idx + 5) <= ui4_text_len) && _sami_strcmp(&(s_src[ui4_idx]), (UINT8 *)"&amp;", 5))
        {
            s_dest[ui4_dest_idx++] = (UINT8)'&';
            ui4_idx += 5;
        }
        else if (((ui4_idx + 6) <= ui4_text_len) && _sami_strcmp(&(s_src[ui4_idx]), (UINT8 *)"&quot;", 6))
        {
            s_dest[ui4_dest_idx++] = (UINT8)'\"';
            ui4_idx += 6;
        }
        else
        {
#if SMI_IGNORE_LTGT
            if ( !b_ltgt ) /*Ignore the char between  "&lt;" and "&gt;" */
#endif
            {
                s_dest[ui4_dest_idx++] = s_src[ui4_idx];
            }
            /* fill the byte into share memory directly */
            ui4_idx = ui4_idx + 1;
        }
    }

    
    if (b_nbsp && (ui4_dest_idx == 1))
    {
        /* This subtitle section contains only &nbsp; with possible CR/LF.
           Treat it as "turn off subtitle" command. */
        s_dest[0] = 0;
    }
    else
    {
        s_dest[ui4_dest_idx++] = 0;
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
static BOOL _sami_chk_head_exist(SBTL_PAGE_INST_MNGR_T*     pt_mngr, BOOL b_UTF16)
{
    BOOL                                b_found;
    INT32                               i4_ret;
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;
    SAMI_PRIVATE_T*                     pt_sami_private;
    SBTL_ENGINE_T*                      pt_engine;
    
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));

    pt_engine = pt_mngr->pt_engine;
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_engine, ("pt_engine=NULL\n"));
    ASSERT(pt_private, ("pt_private=NULL\n"));

    pt_sami_private = (SAMI_PRIVATE_T*)pt_private->pv_data;
    ASSERT(pt_sami_private, ("pt_sami_private=NULL\n"));

    while(1)
    {
        i4_ret = _sami_search_char(pt_engine, 
                                   (UINT8)'<',
                                   b_UTF16,
                                   &b_found);

        if (i4_ret != MMSBTLR_OK || !b_found)
        {
            return FALSE;
        }
        
        i4_ret = _sami_search_string(pt_engine, 
                                     s_head_header,
                                     b_UTF16,
                                     &b_found,
                                     SBTL_SEARCH_BEGIN,
                                     TRUE,
                                     NULL);
        
        if (MMSBTLR_OK==i4_ret && b_found)
        {
            /*<HEAD> encountered*/
            return TRUE;
        }
        else if(MMSBTLR_EOT == i4_ret)
        {
            return FALSE;
        }

        i4_ret = _sami_search_string(pt_engine, 
                                     s_body_header,
                                     b_UTF16,
                                     &b_found,
                                     SBTL_SEARCH_BEGIN,
                                     TRUE,
                                     NULL);
        
        if (MMSBTLR_OK==i4_ret && b_found)
        {
            /*<BODY> encountered, so <HEAD> probably doesn't exist.*/
            pt_sami_private->ui1_total_stream_count = 1;
            pt_sami_private->at_stream_info[0].aui1_lang[0] = 0xFF;
            pt_sami_private->at_stream_info[0].aui1_lang[1] = 0xFF;
            
            return FALSE;
        }
        else if(MMSBTLR_EOT == i4_ret)
        {
            return FALSE;
        }
    }
    /*return TRUE;*/
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
static BOOL _sami_name(SBTL_PAGE_INST_MNGR_T*       pt_mngr,
                           BOOL                         b_UTF16,
                           UINT8                        ui1_stream_idx,
                           UINT64                       ui8_start_offset,
                           UINT64                       ui8_end_offset)
{
    UINT8                               ui1_tmp;
    INT32                               i4_ret;
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;
    SAMI_PRIVATE_T*                     pt_sami_private;
    SBTL_ENGINE_T*                      pt_engine;
    UINT32                              ui4_idx = 0;
    BOOL                                b_skip_dot = TRUE;  /*skip preceding dots*/
    UINT64                              ui8_offset;
    UINT16                              ui2_tmp;
    
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    
    pt_engine = pt_mngr->pt_engine;
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_engine, ("pt_engine=NULL\n"));
    ASSERT(pt_private, ("pt_private=NULL\n"));
    
    pt_sami_private = (SAMI_PRIVATE_T*)pt_private->pv_data;
    ASSERT(pt_sami_private, ("pt_sami_private=NULL\n"));

    while(TRUE)
    {
        sbtl_feeder_getpos(pt_engine, &ui8_offset, MM_SBTL_FEEDER_SUBTITLE);

        if (ui8_offset > ui8_end_offset)
        {
            return FALSE;
        }
        
        if(b_UTF16 == TRUE)
        {
            i4_ret = sbtl_feeder_inpnbyte(pt_engine, 2, (UINT8*)&ui2_tmp, NULL, MM_SBTL_FEEDER_SUBTITLE);
        }
        else
        {
        i4_ret = sbtl_feeder_inp1byte(pt_engine, &ui1_tmp, MM_SBTL_FEEDER_SUBTITLE);
            ui2_tmp = ui1_tmp;
        }
        
        if (i4_ret != MMSBTLR_OK)
        {
            return FALSE;
        }

        if (ui2_tmp == ' ')
        {
            pt_sami_private->at_stream_info[ui1_stream_idx].s_class_name[ui4_idx] = 0;
            pt_sami_private->at_stream_info[ui1_stream_idx].ui1_name_len = (UINT8)ui4_idx;
            return TRUE;
        }
        
        if (!(ui2_tmp == '.' && b_skip_dot))
        {
            sbtl_trans_to_w2s(pt_private->e_language, 
                              ui2_tmp, 
                              &pt_sami_private->at_stream_info[ui1_stream_idx].s_class_name[ui4_idx]);
            
            ui4_idx++;
            b_skip_dot = FALSE;
        }
        
        if (ui4_idx == (CONTAINER_LEN - 1))
        {
            pt_sami_private->at_stream_info[ui1_stream_idx].s_class_name[ui4_idx] = 0;
            pt_sami_private->at_stream_info[ui1_stream_idx].ui1_name_len = (UINT8)ui4_idx;
            return (!b_skip_dot);
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
static BOOL _sami_container(SBTL_PAGE_INST_MNGR_T*      pt_mngr,
                            BOOL                        b_UTF16,
                            UINT64                      ui8_start_offset,
                            UINT64                      ui8_end_offset)
{
    UINT8                               ui1_tmp;
    INT32                               i4_ret;
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;
    SAMI_PRIVATE_T*                     pt_sami_private;
    SBTL_ENGINE_T*                      pt_engine;
    UINT64                              ui8_cur_offset;
    UINT16                              ui2_tmp;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    
    pt_engine = pt_mngr->pt_engine;
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_engine, ("pt_engine=NULL\n"));
    ASSERT(pt_private, ("pt_private=NULL\n"));
    
    pt_sami_private = (SAMI_PRIVATE_T*)pt_private->pv_data;
    ASSERT(pt_sami_private, ("pt_sami_private=NULL\n"));

    while(1)
    {
        if(b_UTF16 == TRUE)
        {
            i4_ret = sbtl_feeder_inpnbyte(pt_engine, 2, (UINT8*)&ui2_tmp, NULL, MM_SBTL_FEEDER_SUBTITLE);
        }
        else
        {
            i4_ret = sbtl_feeder_inp1byte(pt_engine, &ui1_tmp, MM_SBTL_FEEDER_SUBTITLE);
            ui2_tmp = ui1_tmp;
        }
        
        if (i4_ret != MMSBTLR_OK)
        {
            return FALSE;
        }

        sbtl_feeder_getpos(pt_engine, &ui8_cur_offset, MM_SBTL_FEEDER_SUBTITLE);

        if (ui8_cur_offset > ui8_end_offset)
        {
            return FALSE;
        }
        
        if (ui2_tmp == '.')
        {
            /* get the container name here*/
            if (pt_sami_private->ui1_total_stream_count >= SAMI_CC_SZ)
            {
                return FALSE;
            }
            
            if (_sami_name(pt_mngr,
                           b_UTF16,
                           pt_sami_private->ui1_total_stream_count,
                           ui8_start_offset,
                           ui8_end_offset))   /*the index assume to be 0 here*/
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
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
static VOID _sami_class_analysis(SBTL_PAGE_INST_MNGR_T*     pt_mngr,
                                      BOOL                       b_UTF16,
                                      UINT64                     ui8_start_offset,
                                      UINT64                     ui8_end_offset)
{
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;
    SAMI_PRIVATE_T*                     pt_sami_private;
    
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("pt_private=NULL\n"));

    pt_sami_private = (SAMI_PRIVATE_T*)pt_private->pv_data;
    ASSERT(pt_sami_private, ("pt_sami_private=NULL\n"));

    while(1)
    {
        if (!_sami_container(pt_mngr, b_UTF16, ui8_start_offset, ui8_end_offset))
        {
            break;
        }
        
        if ((++pt_sami_private->ui1_total_stream_count) > SAMI_CC_SZ)
        {
            pt_sami_private->ui1_total_stream_count = SAMI_CC_SZ;
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
static VOID _sami_class_info(SBTL_PAGE_INST_MNGR_T*     pt_mngr, BOOL b_UTF16)
{
    BOOL                                b_found;
    INT32                               i4_ret;
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;
    SAMI_PRIVATE_T*                     pt_sami_private;
    SBTL_ENGINE_T*                      pt_engine;
    UINT64                              ui8_start_offset;
    UINT64                              ui8_end_offset;
    
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));

    pt_engine = pt_mngr->pt_engine;
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_engine, ("pt_engine=NULL\n"));
    ASSERT(pt_private, ("pt_private=NULL\n"));

    pt_sami_private = (SAMI_PRIVATE_T*)pt_private->pv_data;
    ASSERT(pt_sami_private, ("pt_sami_private=NULL\n"));

    pt_sami_private->ui1_total_stream_count = 0;
    
    if (!_sami_chk_head_exist(pt_mngr, b_UTF16))
    {
        /* 041201: GIEC's amelie_cafe.smi has no <HEAD> nor <STYLE>.*/
        return;
    }

    i4_ret = _sami_search_string(pt_engine, 
                                 s_comment_header,
                                 b_UTF16,
                                 &b_found,
                                 SBTL_SEARCH_MIDDLE,
                                 FALSE,
                                 &ui8_start_offset);
    
    if (MMSBTLR_OK==i4_ret && b_found)
    {
    }
    else if(MMSBTLR_EOT == i4_ret)
    {
        return;
    }
    else
    {
        return;
    }

    i4_ret = _sami_search_string(pt_engine, 
                                 s_comment_end_header,
                                 b_UTF16,
                                 &b_found,
                                 SBTL_SEARCH_MIDDLE,
                                 TRUE,
                                 &ui8_end_offset);
    
    if (MMSBTLR_OK==i4_ret && b_found)
    {
    }
    else if(MMSBTLR_EOT == i4_ret)
    {
        return;
    }
    else
    {
        return;
    }

    _sami_class_analysis(pt_mngr,
                         b_UTF16,
                         ui8_start_offset,
                         ui8_end_offset);
}

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
INT32 sami_seek_time_stamp(SBTL_PAGE_INST_MNGR_T*   pt_mngr,
                           PTS_T                    t_pts_current,
                           PTS_T*                   pt_pts_start,
                           PTS_T*                   pt_pts_end,
                           BOOL*                    pb_found)
{
    INT32                               i4_ret;
    SBTL_ENGINE_T*                      pt_engine;
    UINT64                              ui8_offset;
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;
    
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));

    pt_engine = pt_mngr->pt_engine;
    ASSERT(pt_engine, ("pt_engine=NULL\n"));

    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("pt_private=NULL\n"));
    
    do
    {
        if (!_sami_next_rdy(pt_mngr,pt_private->b_UTF16))
        {
            *pb_found = FALSE;
            return MMSBTLR_NOT_FOUND;
        }

        i4_ret = _sami_next_unit(pt_mngr, pt_private->b_UTF16, pt_pts_start);
        if (i4_ret != MMSBTLR_OK)
        {
            return i4_ret;
        }
        
        if (_sami_find_class_until_sync(pt_mngr, pt_private->b_UTF16))
        {
            _sami_get_data(pt_mngr, pt_private->b_UTF16);

            sbtl_feeder_getpos(pt_engine, &ui8_offset, MM_SBTL_FEEDER_SUBTITLE);

            i4_ret = _sami_next_unit(pt_mngr, pt_private->b_UTF16, pt_pts_end);
            if (i4_ret != MMSBTLR_OK)
            {
                return i4_ret;
            }
            if (t_pts_current<*pt_pts_end)
            {
                sbtl_feeder_setpos(pt_engine, (INT64)ui8_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
                *pb_found = TRUE;
                return MMSBTLR_OK;
            }

            sbtl_feeder_setpos(pt_engine, (INT64)ui8_offset, MEDIA_SEEK_BGN, MM_SBTL_FEEDER_SUBTITLE);
        }
        else
        {
            *pb_found = FALSE;
            return MMSBTLR_NOT_FOUND;
        }
        
    }while(1);
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
INT32 sami_load_string(SBTL_PAGE_INST_MNGR_T*   pt_mngr)
{
/*    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;
    
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;*/

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    _sami_parse(pt_mngr);
    

/*        x_uc_ps_to_w2s((const CHAR *)pt_private->aui1_text_buf,
                   pt_private->w2s_text,
                   256);  */
                   
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
UINT32 sami_timer_period(SBTL_PAGE_INST_MNGR_T*     pt_mngr)
{
    ASSERT(pt_mngr->pt_engine->t_cfg.i4_speed > 0, ("pt_mngr->pt_engine.t_cfg.i4_speed=0\n"));

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
INT32 sami_init(SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;
    SAMI_PRIVATE_T*                     pt_sami_private;
    
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    
    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("pt_private=NULL\n"));
    
    pt_private->pv_data = x_mem_alloc(sizeof(SAMI_PRIVATE_T));
    
    if (NULL == pt_private->pv_data)
    {
        return MMSBTLR_INSUFFICIENT_MEMORY;
    }

    pt_sami_private = (SAMI_PRIVATE_T*)pt_private->pv_data;
    pt_sami_private->ui1_stream_idx = (UINT8)pt_mngr->pt_engine->t_cfg.ui4_stream_id;
    
    _sami_class_info(pt_mngr, pt_private->b_UTF16);
    
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
INT32 sami_deinit(SBTL_PAGE_INST_MNGR_T*    pt_mngr)
{
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));

    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("pt_private=NULL\n"));

    if (pt_private->pv_data)
    {
        x_mem_free(pt_private->pv_data);
        pt_private->pv_data = NULL;
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
VOID sami_start(SBTL_PAGE_INST_MNGR_T*  pt_mngr)
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
VOID sami_stop(SBTL_PAGE_INST_MNGR_T*   pt_mngr)
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
VOID sami_config(SBTL_PAGE_INST_MNGR_T*     pt_mngr,
                 UINT32                     ui4_set_type)
{
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;
    SAMI_PRIVATE_T*                     pt_sami_private;
    
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));

    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("pt_private=NULL\n"));
    
    pt_sami_private = (SAMI_PRIVATE_T*)pt_private->pv_data;
    ASSERT(pt_sami_private, ("pt_sami_private=NULL\n"));

    switch (ui4_set_type)
    {
    case EVN_MM_SBTL_SET_STREAM_INDEX:
        pt_sami_private->ui1_stream_idx = (UINT8)pt_mngr->pt_engine->t_cfg.ui4_stream_id;
        break;
        
    default:
        break;
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
INT32 sami_get_info( SBTL_PAGE_INST_MNGR_T*        pt_mngr,
                     UINT32                        ui4_get_type,
                     VOID*                         pv_get_info,
                     SIZE_T                        z_get_info_len)
{
    SBTL_TEXT_PAGE_MNGR_PRIVATE_T*      pt_private;
    SAMI_PRIVATE_T*                     pt_sami_private;
    SAMI_STREAM_INFO_T*                 pt_stream_info;
    
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));

    pt_private = (SBTL_TEXT_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("pt_private=NULL\n"));

    pt_sami_private = (SAMI_PRIVATE_T*)pt_private->pv_data;
    ASSERT(pt_sami_private, ("pt_sami_private=NULL\n"));

    switch (ui4_get_type)
    {
    case EVN_MM_SBTL_GET_STREAM_INFO:
        if (z_get_info_len < sizeof (SAMI_STREAM_INFO_T))
        {
            pv_get_info = NULL;
        }
        else
        {
            pt_stream_info = (SAMI_STREAM_INFO_T*)pv_get_info;
            pt_stream_info->ui1_stream_idx = pt_sami_private->ui1_stream_idx;
            pt_stream_info->ui1_total_stream_count = pt_sami_private->ui1_total_stream_count;
            x_memcpy(pt_stream_info->at_stream_info, pt_sami_private->at_stream_info, sizeof(SMAI_CC_INF_T)*SAMI_CC_SZ);
        }
        break;
        
    default:
        break;
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
BOOL sami_parser_verify(SBTL_ENGINE_T*  pt_engine)
{
    BOOL    b_found;
    INT32   i4_ret;
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

    i4_ret = _sami_search_string_p(pt_engine, 
                                 s_sami_header,
                                 b_UTF16,
                                 &b_found,
                                 SBTL_SEARCH_MIDDLE,
                                 TRUE,
                                 NULL,
                                 MM_SBTL_PARSE_MAX_LENGTH);

    if (MMSBTLR_OK == i4_ret)
    {
        return b_found;
    }
    
    return FALSE;
}


