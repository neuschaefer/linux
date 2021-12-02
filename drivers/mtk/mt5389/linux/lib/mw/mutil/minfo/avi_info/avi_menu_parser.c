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
 * $RCSfile: avi_menu_parser.c,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/4 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 9ce950eb85bf90ea84aa0781ae2e804f $ Asa
 *
 * Description: 
 *         This file contains the implementation of the avi parser function
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
#include "inc/x_mm_common.h"
#include "mutil/minfo/minfo.h"
#include "mutil/minfo/x_minfo.h"
#include "unicode/x_uc_str.h"
#include "avi_menu_parser.h"
#include "avi_parser.h"

#else

#include "x_os.h"

#include "u_common.h"
#include "x_mm_common.h"
#include "../minfo.h"
#include "../x_minfo.h"
#include "x_uc_str.h"
#include "avi_menu_parser.h"
#include "avi_parser.h"


#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
                                                                    
/* big endian for FourCC type */
#define dwBigFourCC(a,b,c,d)  ((((UINT32)a)<<24)|((b)<<16)|((c)<<8)|(d))


#define DIVX_MENU   dwBigFourCC('M', 'E', 'N', 'U')    /* DivxMediaManager */
#define DIVX_LGMN   dwBigFourCC('L', 'G', 'M', 'N')    /* LanguageMenu */
#define DIVX_WMEN   dwBigFourCC('W', 'M', 'E', 'N')    /* DivxMediaMenu */
#define DIVX_BMEN   dwBigFourCC('B', 'M', 'E', 'N')    /* ButtonMenu */
#define DIVX_MREC   dwBigFourCC('M', 'R', 'E', 'C')    /* MenuRectangle */
#define DIVX_MDIA   dwBigFourCC('M', 'D', 'I', 'A')    /* Media */
#define DIVX_MEDS   dwBigFourCC('M', 'E', 'D', 'S')    /* MediaSource */
#define DIVX_MEDT   dwBigFourCC('M', 'E', 'D', 'T')    /* MediaTrack */
#define DIVX_ACTN   dwBigFourCC('A', 'C', 'T', 'N')    /* Action */
#define DIVX_STAC   dwBigFourCC('S', 'T', 'A', 'C')    /* SubtitleSelectAction */
#define DIVX_ASAC   dwBigFourCC('A', 'S', 'A', 'C')    /* AudioSelectAction */
#define DIVX_PLAY   dwBigFourCC('P', 'L', 'A', 'Y')    /* PlayAction */
#define DIVX_PLY0   dwBigFourCC('P', 'L', 'Y', '0')    /* PlayFromCurrentOffset */
#define DIVX_MNTX   dwBigFourCC('M', 'N', 'T', 'X')    /* MenuTransitionAction */
#define DIVX_BNTX   dwBigFourCC('B', 'N', 'T', 'X')    /* ButtonTransitionAction */
#define DIVX_MNTK   dwBigFourCC('M', 'N', 'T', 'K')    /* MenuTracks */
#define DIVX_TITL   dwBigFourCC('T', 'I', 'T', 'L')    /* Title */
#define DIVX_CHAP   dwBigFourCC('C', 'H', 'A', 'P')    /* Chapter */
#define DIVX_TRTB   dwBigFourCC('T', 'R', 'T', 'B')    /* TranslationTable */
#define DIVX_TRLK   dwBigFourCC('T', 'R', 'L', 'K')    /* TranslationLookup */
#define DIVX_TRAN   dwBigFourCC('T', 'R', 'A', 'N')    /* Translation */


/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
DIVX_MENU_OBJ_BUF_LIST_T*   pt_curr_memory_node;

/*-----------------------------------------------------------------------------
                     functions declarations
----------------------------------------------------------------------------*/

static UINT32 _Word24cc(UINT16 ui2_Wrd)
{
  int i;
  UINT32 d4cc;

  d4cc = 0;
  for(i = 0; i < 4; i++)
  {
    d4cc |= (((ui2_Wrd >> (i * 4)) & 0xf) | 0x30) << ((3 - i) * 8);
  }

  return(d4cc);
}

/*-----------------------------------------------------------------------------
 * Name:  _Swap4Bytes
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: MINFOR_OK                 The API is successful. 
 *          MINFOR_INTERNAL_ERR        The library initialize failed.
 ----------------------------------------------------------------------------*/         
static UINT32 _Swap4Bytes(UINT32 u4In)
{
    UINT32 u4Ret;

    u4Ret = (u4In >> 24);
    u4Ret |= ((u4In >> 16) & 0x0ff) << 8;
    u4Ret |= ((u4In >>  8) & 0x0ff) << 16;
    u4Ret |= (u4In << 24);
    return u4In;
}

static INT32 divx_menu_parser_uninit (MINFO_OBJ_T* pt_minfo_obj)
{
    MINFO_AVI_RIFF_INF_T*       pt_avi_hdlr_obj;
    DIVX_MENU_OBJ_BUF_LIST_T*   pt_menu_obj_buf_node;
    DIVX_MENU_OBJ_BUF_LIST_T*   pt_menu_obj_buf_head;
    
    pt_avi_hdlr_obj = (MINFO_AVI_RIFF_INF_T*)pt_minfo_obj->pv_handler_obj;
    if (pt_avi_hdlr_obj->pui1_menu_buf != NULL)
    {
        _mm_util_mem_free(pt_avi_hdlr_obj->pui1_menu_buf);
        pt_avi_hdlr_obj->pui1_menu_buf = NULL;
    }

    pt_menu_obj_buf_head = pt_avi_hdlr_obj->pt_menu_obj_buf_list;
    while (pt_menu_obj_buf_head != NULL)
    {
        pt_menu_obj_buf_node = pt_menu_obj_buf_head;
        pt_menu_obj_buf_head = pt_menu_obj_buf_head->pt_next;
        if (pt_menu_obj_buf_node->pui1_block_mem != NULL)
        {
            _mm_util_mem_free(pt_menu_obj_buf_node->pui1_block_mem);
            pt_menu_obj_buf_node->pui1_block_mem = NULL;
        }
        _mm_util_mem_free(pt_menu_obj_buf_node);
        pt_menu_obj_buf_node = NULL;
    }
    pt_avi_hdlr_obj->pt_menu_obj_buf_list = NULL;
    pt_avi_hdlr_obj->pt_menu_object = NULL;
    
    return MINFOR_OK;
}


static INT32 divx_menu_parser_init(MINFO_OBJ_T* pt_minfo_obj, UINT32 ui4_chunk_sz)
{    
    MINFO_AVI_RIFF_INF_T*       pt_avi_hdlr_obj; 
    DIVX_MENU_OBJ_BUF_LIST_T*   pt_divx_menu_obj_buf_list;
    
    pt_avi_hdlr_obj = (MINFO_AVI_RIFF_INF_T*)pt_minfo_obj->pv_handler_obj;  

    pt_avi_hdlr_obj->pui1_menu_buf = (UINT8*)_mm_util_mem_alloc(ui4_chunk_sz);
    if (pt_avi_hdlr_obj->pui1_menu_buf == NULL)
    {
        return MINFOR_OUT_OF_MEMORY;
    }
    
    pt_divx_menu_obj_buf_list = 
        (DIVX_MENU_OBJ_BUF_LIST_T*)_mm_util_mem_alloc(sizeof(DIVX_MENU_OBJ_BUF_LIST_T));
    if (pt_divx_menu_obj_buf_list == NULL)
    {
        _mm_util_mem_free(pt_avi_hdlr_obj->pui1_menu_buf);
        return MINFOR_OUT_OF_MEMORY;
    }
    pt_divx_menu_obj_buf_list->pui1_block_mem = (UINT8*)_mm_util_mem_alloc(DIVX_MENU_OBJ_BUF_LENGTH);
    if (pt_divx_menu_obj_buf_list->pui1_block_mem == NULL)
    {
        _mm_util_mem_free(pt_avi_hdlr_obj->pui1_menu_buf);
        _mm_util_mem_free(pt_divx_menu_obj_buf_list);
        return MINFOR_OUT_OF_MEMORY;
    }
    pt_divx_menu_obj_buf_list->ui4_menu_obj_buf_length = DIVX_MENU_OBJ_BUF_LENGTH;
    pt_divx_menu_obj_buf_list->ui4_curr_buf_offset = 0;
    pt_divx_menu_obj_buf_list->pt_next = NULL;
    pt_avi_hdlr_obj->pt_menu_obj_buf_list = pt_divx_menu_obj_buf_list;
    pt_curr_memory_node = pt_avi_hdlr_obj->pt_menu_obj_buf_list;
    return MINFOR_OK;
}

static VOID* divx_mem_alloc(SIZE_T z_size)
{
    VOID* pv_buf;
    DIVX_MENU_OBJ_BUF_LIST_T*   pt_divx_menu_obj_buf_list;

    /* memory starting address need be multiple of 4 */
    z_size = ((z_size+3)>>2)<<2;
    if ((pt_curr_memory_node->ui4_curr_buf_offset + z_size) > DIVX_MENU_OBJ_BUF_LENGTH )
    {
        pt_divx_menu_obj_buf_list = 
            (DIVX_MENU_OBJ_BUF_LIST_T*)_mm_util_mem_alloc(sizeof(DIVX_MENU_OBJ_BUF_LIST_T));
        if (pt_divx_menu_obj_buf_list == NULL)
        {
            return NULL;
        }
        pt_divx_menu_obj_buf_list->pui1_block_mem = (UINT8*)_mm_util_mem_alloc(DIVX_MENU_OBJ_BUF_LENGTH);
        if (pt_divx_menu_obj_buf_list->pui1_block_mem == NULL)
        {
            return NULL;
        }
        pt_divx_menu_obj_buf_list->ui4_menu_obj_buf_length = DIVX_MENU_OBJ_BUF_LENGTH;
        pt_divx_menu_obj_buf_list->ui4_curr_buf_offset = 0;
        pt_divx_menu_obj_buf_list->pt_next = NULL;
        pt_curr_memory_node->pt_next = pt_divx_menu_obj_buf_list;
        pt_curr_memory_node = pt_curr_memory_node->pt_next;
    }
    pv_buf = (VOID*)(pt_curr_memory_node->pui1_block_mem + pt_curr_memory_node->ui4_curr_buf_offset);
    pt_curr_memory_node->ui4_curr_buf_offset += z_size;
    return pv_buf;
}

static INT32 get_MENU_Object(UINT8* pui1_chunk, UINT32* pui4_size, DIVX_MENU_OBJECT_T* pt_curr_menu)
{
    /* Main menu object. This should be the first and the only. */
    UINT8   ui1_num_ver = 3;
    CHAR    sz_ver_buff[4];
    INT32   i4_version;
    CHAR    sz_fourCC[5];
    UINT8*  pui1_chunk_ptr;

    pui1_chunk_ptr = pui1_chunk;
    memcpy(sz_fourCC, pui1_chunk_ptr, 4);
    sz_fourCC[4] = '\0';
    pui1_chunk_ptr += 4;


    memcpy(pt_curr_menu->sz_fourCC, sz_fourCC, 4);
    pt_curr_menu->sz_fourCC[4] = 0;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->ui4_chunk_size);
    pui1_chunk_ptr += 4;

    /* Get the file offset of the menu data */
    LOADL_QWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_divx_media_manager.ui8_offset);
    pui1_chunk_ptr += 8;

    /* Determine if the version indicator exists.  If it does read */
    /* the version.  If not it is version 0. */
    memcpy(sz_ver_buff, pui1_chunk_ptr, ui1_num_ver);
    sz_ver_buff[ui1_num_ver] = 0;
    memcpy(pt_curr_menu->Type.t_divx_media_manager.sz_versionID, sz_ver_buff, ui1_num_ver);
    pt_curr_menu->Type.t_divx_media_manager.sz_versionID[ui1_num_ver] = 0;
    if (0 == strcmp("VER", sz_ver_buff)) 
    {
        pui1_chunk_ptr += ui1_num_ver;

        /* read the version */
        LOADL_DWRD(pui1_chunk_ptr, i4_version);
        if (i4_version > 1) 
        {
            return MINFOR_INTERNAL_ERR;
        }
        pt_curr_menu->Type.t_divx_media_manager.i4_menu_version = i4_version;
        pui1_chunk_ptr += 4;
    }
    else 
    {
        pt_curr_menu->Type.t_divx_media_manager.i4_menu_version = 0;
    }

    /* Get this objects ID */
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->i4_objectID);
    pui1_chunk_ptr += 4;

    /* Get the starting menu ID */
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_divx_media_manager.i4_starting_menuID);
    pui1_chunk_ptr += 4;
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_divx_media_manager.i4_default_language_len);
    pui1_chunk_ptr += 4;
    if(pt_curr_menu->Type.t_divx_media_manager.i4_default_language_len > 0)
    {
        pt_curr_menu->Type.t_divx_media_manager.pi2_default_language = 
            (INT16*)divx_mem_alloc(pt_curr_menu->Type.t_divx_media_manager.i4_default_language_len);
        if (pt_curr_menu->Type.t_divx_media_manager.pi2_default_language == NULL)
        {
            return MINFOR_OUT_OF_MEMORY;
        }
        memcpy((INT8*)pt_curr_menu->Type.t_divx_media_manager.pi2_default_language, 
               pui1_chunk_ptr, 
               pt_curr_menu->Type.t_divx_media_manager.i4_default_language_len);
        pui1_chunk_ptr += pt_curr_menu->Type.t_divx_media_manager.i4_default_language_len;
    }

    *pui4_size = pui1_chunk_ptr - pui1_chunk;
    return MINFOR_OK;
}

static INT32 get_WMEN_Object(UINT8* pui1_chunk, UINT32* pui4_size, DIVX_MENU_OBJECT_T* pt_curr_menu)
{
    /* Divx Media menu object. */
    CHAR    sz_fourCC[5];
    UINT8*  pui1_chunk_ptr;

    pui1_chunk_ptr = pui1_chunk;
    
    memcpy(sz_fourCC, pui1_chunk_ptr, 4);
    sz_fourCC[4] = '\0';
    pui1_chunk_ptr += 4;


    memcpy(pt_curr_menu->sz_fourCC, sz_fourCC, 4);
    pt_curr_menu->sz_fourCC[4] = 0;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->ui4_chunk_size);
    pui1_chunk_ptr += 4;

    /* Get this objects ID */
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->i4_objectID);
    pui1_chunk_ptr += 4;
    
    /* Get the background video/audio ID */
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_divx_media_menu.i4_background_videoID);
    pui1_chunk_ptr += 4;
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_divx_media_menu.i4_background_audioID);
    pui1_chunk_ptr += 4;
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_divx_media_menu.i4_starting_menu_itemID);
    pui1_chunk_ptr += 4;
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_divx_media_menu.i4_enter_actionID);
    pui1_chunk_ptr += 4;
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_divx_media_menu.i4_exit_actionID);
    pui1_chunk_ptr += 4;
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_divx_media_menu.i4_menu_type_len);
    pui1_chunk_ptr += 4;
    if(pt_curr_menu->Type.t_divx_media_menu.i4_menu_type_len > 0)
    {
        pt_curr_menu->Type.t_divx_media_menu.pi2_menu_type = 
            (INT16*)divx_mem_alloc(pt_curr_menu->Type.t_divx_media_menu.i4_menu_type_len);
        if (pt_curr_menu->Type.t_divx_media_menu.pi2_menu_type == NULL)
        {
            return MINFOR_OUT_OF_MEMORY;
        }
        memcpy((INT8*)pt_curr_menu->Type.t_divx_media_menu.pi2_menu_type, 
               pui1_chunk_ptr, 
               pt_curr_menu->Type.t_divx_media_menu.i4_menu_type_len);
        pui1_chunk_ptr += pt_curr_menu->Type.t_divx_media_menu.i4_menu_type_len;
    }
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_divx_media_menu.i4_menu_name_len);
    pui1_chunk_ptr += 4;
    if(pt_curr_menu->Type.t_divx_media_menu.i4_menu_name_len > 0)
    {
        pt_curr_menu->Type.t_divx_media_menu.pi2_menu_name = 
            (INT16*)divx_mem_alloc(pt_curr_menu->Type.t_divx_media_menu.i4_menu_name_len);
        if (pt_curr_menu->Type.t_divx_media_menu.pi2_menu_name == NULL)
        {
            return MINFOR_OUT_OF_MEMORY;
        }
        memcpy((INT8*)pt_curr_menu->Type.t_divx_media_menu.pi2_menu_name, 
               pui1_chunk_ptr, 
               pt_curr_menu->Type.t_divx_media_menu.i4_menu_name_len);
        pui1_chunk_ptr += pt_curr_menu->Type.t_divx_media_menu.i4_menu_name_len;
    }

    *pui4_size = pui1_chunk_ptr - pui1_chunk;
    return MINFOR_OK;
}

static INT32 get_BMEN_Object(UINT8* pui1_chunk, UINT32* pui4_size, DIVX_MENU_OBJECT_T* pt_curr_menu)
{
    /* Divx Button menu object. */
    CHAR    sz_fourCC[5];
    UINT8*  pui1_chunk_ptr;

    pui1_chunk_ptr = pui1_chunk;
    
    memcpy(sz_fourCC, pui1_chunk_ptr, 4);
    sz_fourCC[4] = '\0';
    pui1_chunk_ptr += 4;


    memcpy(pt_curr_menu->sz_fourCC, sz_fourCC, 4);
    pt_curr_menu->sz_fourCC[4] = 0;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->ui4_chunk_size);
    pui1_chunk_ptr += 4;

    /* Get this objects ID */
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->i4_objectID);
    pui1_chunk_ptr += 4;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_button_menu.i4_overlayID);
    pui1_chunk_ptr += 4;
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_button_menu.i4_up_actionID);
    pui1_chunk_ptr += 4;
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_button_menu.i4_down_actionID);
    pui1_chunk_ptr += 4;
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_button_menu.i4_left_actionID);
    pui1_chunk_ptr += 4;
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_button_menu.i4_right_actionID);
    pui1_chunk_ptr += 4;
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_button_menu.i4_select_actionID);
    pui1_chunk_ptr += 4;
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_button_menu.i4_button_name_len);
    pui1_chunk_ptr += 4;
    if(pt_curr_menu->Type.t_button_menu.i4_button_name_len > 0)
    {
        pt_curr_menu->Type.t_button_menu.pi2_button_name = 
            (INT16*)divx_mem_alloc(pt_curr_menu->Type.t_button_menu.i4_button_name_len);
        if (pt_curr_menu->Type.t_button_menu.pi2_button_name == NULL)
        {
            return MINFOR_OUT_OF_MEMORY;
        }
        memcpy((INT8*)pt_curr_menu->Type.t_button_menu.pi2_button_name, 
               pui1_chunk_ptr, 
               pt_curr_menu->Type.t_button_menu.i4_button_name_len);
        pui1_chunk_ptr += pt_curr_menu->Type.t_button_menu.i4_button_name_len;
    }
    
    *pui4_size = pui1_chunk_ptr - pui1_chunk;
    return MINFOR_OK;
}

static INT32 get_MREC_Object(UINT8* pui1_chunk, UINT32* pui4_size, DIVX_MENU_OBJECT_T* pt_curr_menu)
{
    /* Divx Menu Rectangle object. */
    CHAR    sz_fourCC[5];
    UINT8*  pui1_chunk_ptr;

    pui1_chunk_ptr = pui1_chunk;
    
    memcpy(sz_fourCC, pui1_chunk_ptr, 4);
    sz_fourCC[4] = '\0';
    pui1_chunk_ptr += 4;


    memcpy(pt_curr_menu->sz_fourCC, sz_fourCC, 4);
    pt_curr_menu->sz_fourCC[4] = 0;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->ui4_chunk_size);
    pui1_chunk_ptr += 4;

    /* Get this objects ID */
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->i4_objectID);
    pui1_chunk_ptr += 4;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_menu_rectangle.i4_left);
    pui1_chunk_ptr += 4;
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_menu_rectangle.i4_top);
    pui1_chunk_ptr += 4;
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_menu_rectangle.i4_width);
    pui1_chunk_ptr += 4;
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_menu_rectangle.i4_height);
    pui1_chunk_ptr += 4;
    
    *pui4_size = pui1_chunk_ptr - pui1_chunk;
    return MINFOR_OK;
}

static INT32 get_ACTN_Object(UINT8* pui1_chunk, UINT32* pui4_size, DIVX_MENU_OBJECT_T* pt_curr_menu)
{
    /* Divx Action object. */
    CHAR    sz_fourCC[5];
    UINT8*  pui1_chunk_ptr;

    pui1_chunk_ptr = pui1_chunk;
    
    memcpy(sz_fourCC, pui1_chunk_ptr, 4);
    sz_fourCC[4] = '\0';
    pui1_chunk_ptr += 4;


    memcpy(pt_curr_menu->sz_fourCC, sz_fourCC, 4);
    pt_curr_menu->sz_fourCC[4] = 0;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->ui4_chunk_size);
    pui1_chunk_ptr += 4;

    /* Get this objects ID */
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->i4_objectID);
    pui1_chunk_ptr += 4;
    
    pt_curr_menu->Type.t_action.i4_reserved = 0;
    
    *pui4_size = pui1_chunk_ptr - pui1_chunk;
    return MINFOR_OK;
}

static INT32 get_STAC_Object(UINT8* pui1_chunk, UINT32* pui4_size, DIVX_MENU_OBJECT_T* pt_curr_menu)
{
    /* Divx Subtitle Select Action object. */
    CHAR    sz_fourCC[5];
    UINT8*  pui1_chunk_ptr;

    pui1_chunk_ptr = pui1_chunk;
    
    memcpy(sz_fourCC, pui1_chunk_ptr, 4);
    sz_fourCC[4] = '\0';
    pui1_chunk_ptr += 4;


    memcpy(pt_curr_menu->sz_fourCC, sz_fourCC, 4);
    pt_curr_menu->sz_fourCC[4] = 0;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->ui4_chunk_size);
    pui1_chunk_ptr += 4;

    /* Get this objects ID */
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->i4_objectID);
    pui1_chunk_ptr += 4;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_subtitle_select_action.i4_titleID);
    pui1_chunk_ptr += 4;

    memcpy(pt_curr_menu->Type.t_subtitle_select_action.sz_trackID, pui1_chunk_ptr, 4);
    pt_curr_menu->Type.t_subtitle_select_action.sz_trackID[4] = 0;
    pui1_chunk_ptr += 4;
    
    *pui4_size = pui1_chunk_ptr - pui1_chunk;
    return MINFOR_OK;
}

static INT32 get_ASAC_Object(UINT8* pui1_chunk, UINT32* pui4_size, DIVX_MENU_OBJECT_T* pt_curr_menu)
{
    /* Divx Audio Select Action object. */
    CHAR    sz_fourCC[5];
    UINT8*  pui1_chunk_ptr;

    pui1_chunk_ptr = pui1_chunk;
    
    memcpy(sz_fourCC, pui1_chunk_ptr, 4);
    sz_fourCC[4] = '\0';
    pui1_chunk_ptr += 4;


    memcpy(pt_curr_menu->sz_fourCC, sz_fourCC, 4);
    pt_curr_menu->sz_fourCC[4] = 0;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->ui4_chunk_size);
    pui1_chunk_ptr += 4;

    /* Get this objects ID */
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->i4_objectID);
    pui1_chunk_ptr += 4;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_audio_select_action.i4_titleID);
    pui1_chunk_ptr += 4;

    memcpy(pt_curr_menu->Type.t_audio_select_action.sz_trackID, pui1_chunk_ptr, 4);
    pt_curr_menu->Type.t_audio_select_action.sz_trackID[4] = 0;
    pui1_chunk_ptr += 4;
    
    *pui4_size = pui1_chunk_ptr - pui1_chunk;
    return MINFOR_OK;
}

static INT32 get_PLAY_Object(UINT8* pui1_chunk, UINT32* pui4_size, DIVX_MENU_OBJECT_T* pt_curr_menu)
{
    /* Divx Play Action object. */
    CHAR    sz_fourCC[5];
    UINT8*  pui1_chunk_ptr;

    pui1_chunk_ptr = pui1_chunk;
    
    memcpy(sz_fourCC, pui1_chunk_ptr, 4);
    sz_fourCC[4] = '\0';
    pui1_chunk_ptr += 4;


    memcpy(pt_curr_menu->sz_fourCC, sz_fourCC, 4);
    pt_curr_menu->sz_fourCC[4] = 0;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->ui4_chunk_size);
    pui1_chunk_ptr += 4;

    /* Get this objects ID */
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->i4_objectID);
    pui1_chunk_ptr += 4;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_play_action.i4_titleID);
    pui1_chunk_ptr += 4;

    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_play_action.i4_media_objectID);
    pui1_chunk_ptr += 4;
    
    *pui4_size = pui1_chunk_ptr - pui1_chunk;
    return MINFOR_OK;
}

static INT32 get_PLY0_Object(UINT8* pui1_chunk, UINT32* pui4_size, DIVX_MENU_OBJECT_T* pt_curr_menu)
{
    /* Divx Play From Current Offset Action object. */
    CHAR    sz_fourCC[5];
    UINT8*  pui1_chunk_ptr;

    pui1_chunk_ptr = pui1_chunk;
    
    memcpy(sz_fourCC, pui1_chunk_ptr, 4);
    sz_fourCC[4] = '\0';
    pui1_chunk_ptr += 4;


    memcpy(pt_curr_menu->sz_fourCC, sz_fourCC, 4);
    pt_curr_menu->sz_fourCC[4] = 0;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->ui4_chunk_size);
    pui1_chunk_ptr += 4;

    /* Get this objects ID */
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->i4_objectID);
    pui1_chunk_ptr += 4;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_play_from_curr_offset_action.i4_titleID);
    pui1_chunk_ptr += 4;

    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_play_from_curr_offset_action.i4_media_objectID);
    pui1_chunk_ptr += 4;
    
    *pui4_size = pui1_chunk_ptr - pui1_chunk;
    return MINFOR_OK;
}

static INT32 get_MNTX_Object(UINT8* pui1_chunk, UINT32* pui4_size, DIVX_MENU_OBJECT_T* pt_curr_menu)
{
    /* Divx Menu Transition Action object. */
    CHAR    sz_fourCC[5];
    UINT8*  pui1_chunk_ptr;

    pui1_chunk_ptr = pui1_chunk;
    
    memcpy(sz_fourCC, pui1_chunk_ptr, 4);
    sz_fourCC[4] = '\0';
    pui1_chunk_ptr += 4;


    memcpy(pt_curr_menu->sz_fourCC, sz_fourCC, 4);
    pt_curr_menu->sz_fourCC[4] = 0;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->ui4_chunk_size);
    pui1_chunk_ptr += 4;

    /* Get this objects ID */
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->i4_objectID);
    pui1_chunk_ptr += 4;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_menu_transition_action.i4_menuID);
    pui1_chunk_ptr += 4;
    
    *pui4_size = pui1_chunk_ptr - pui1_chunk;
    return MINFOR_OK;
}

static INT32 get_BNTX_Object(UINT8* pui1_chunk, UINT32* pui4_size, DIVX_MENU_OBJECT_T* pt_curr_menu)
{
    /* Divx Button Transition Action object. */
    CHAR    sz_fourCC[5];
    UINT8*  pui1_chunk_ptr;

    pui1_chunk_ptr = pui1_chunk;
    
    memcpy(sz_fourCC, pui1_chunk_ptr, 4);
    sz_fourCC[4] = '\0';
    pui1_chunk_ptr += 4;


    memcpy(pt_curr_menu->sz_fourCC, sz_fourCC, 4);
    pt_curr_menu->sz_fourCC[4] = 0;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->ui4_chunk_size);
    pui1_chunk_ptr += 4;

    /* Get this objects ID */
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->i4_objectID);
    pui1_chunk_ptr += 4;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_button_transition_action.i4_button_menuID);
    pui1_chunk_ptr += 4;
    
    *pui4_size = pui1_chunk_ptr - pui1_chunk;
    return MINFOR_OK;
}

static INT32 get_LGMN_Object(UINT8* pui1_chunk, UINT32* pui4_size, DIVX_MENU_OBJECT_T* pt_curr_menu)
{
    /* Divx Language menu object. */
    CHAR    sz_fourCC[5];
    UINT8*  pui1_chunk_ptr;

    pui1_chunk_ptr = pui1_chunk;
    
    memcpy(sz_fourCC, pui1_chunk_ptr, 4);
    sz_fourCC[4] = '\0';
    pui1_chunk_ptr += 4;


    memcpy(pt_curr_menu->sz_fourCC, sz_fourCC, 4);
    pt_curr_menu->sz_fourCC[4] = 0;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->ui4_chunk_size);
    pui1_chunk_ptr += 4;

    /* Get this objects ID */
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->i4_objectID);
    pui1_chunk_ptr += 4;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_language_menus.i4_language_code_len);
    pui1_chunk_ptr += 4;
    if(pt_curr_menu->Type.t_language_menus.i4_language_code_len > 0)
    {
        pt_curr_menu->Type.t_language_menus.pi2_language_code = 
            (INT16*)divx_mem_alloc(pt_curr_menu->Type.t_language_menus.i4_language_code_len);
        if (pt_curr_menu->Type.t_language_menus.pi2_language_code == NULL)
        {
            return MINFOR_OUT_OF_MEMORY;
        }
        memcpy((INT8*)pt_curr_menu->Type.t_language_menus.pi2_language_code, 
               pui1_chunk_ptr, 
               pt_curr_menu->Type.t_language_menus.i4_language_code_len);
        pui1_chunk_ptr += pt_curr_menu->Type.t_language_menus.i4_language_code_len;
    }
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_language_menus.i4_starting_menuID);
    pui1_chunk_ptr += 4;
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_language_menus.i4_root_menuID);
    pui1_chunk_ptr += 4;
    
    *pui4_size = pui1_chunk_ptr - pui1_chunk;
    return MINFOR_OK;
}

static INT32 get_MDIA_Object(UINT8* pui1_chunk, UINT32* pui4_size, DIVX_MENU_OBJECT_T* pt_curr_menu)
{
    /* Divx Media object. */
    CHAR    sz_fourCC[5];
    UINT8*  pui1_chunk_ptr;

    pui1_chunk_ptr = pui1_chunk;
    
    memcpy(sz_fourCC, pui1_chunk_ptr, 4);
    sz_fourCC[4] = '\0';
    pui1_chunk_ptr += 4;


    memcpy(pt_curr_menu->sz_fourCC, sz_fourCC, 4);
    pt_curr_menu->sz_fourCC[4] = 0;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->ui4_chunk_size);
    pui1_chunk_ptr += 4;

    /* Get this objects ID */
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->i4_objectID);
    pui1_chunk_ptr += 4;
    
    pt_curr_menu->Type.t_media.i4_reserved = 0;
    
    *pui4_size = pui1_chunk_ptr - pui1_chunk;
    return MINFOR_OK;
}

static INT32 get_MNTK_Object(UINT8* pui1_chunk, UINT32* pui4_size, DIVX_MENU_OBJECT_T* pt_curr_menu)
{
    /* Divx Menu Tracks object. */
    CHAR    sz_fourCC[5];
    UINT8*  pui1_chunk_ptr;

    pui1_chunk_ptr = pui1_chunk;
    
    memcpy(sz_fourCC, pui1_chunk_ptr, 4);
    sz_fourCC[4] = '\0';
    pui1_chunk_ptr += 4;


    memcpy(pt_curr_menu->sz_fourCC, sz_fourCC, 4);
    pt_curr_menu->sz_fourCC[4] = 0;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->ui4_chunk_size);
    pui1_chunk_ptr += 4;

    /* Get this objects ID */
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->i4_objectID);
    pui1_chunk_ptr += 4;
    
    pt_curr_menu->Type.t_menu_tracks.i4_reserved = 0;
    
    *pui4_size = pui1_chunk_ptr - pui1_chunk;
    return MINFOR_OK;
}

static INT32 get_MEDS_Object(UINT8* pui1_chunk, UINT32* pui4_size, DIVX_MENU_OBJECT_T* pt_curr_menu)
{
    /* Divx Media Source object. */
    CHAR    sz_fourCC[5];
    UINT8*  pui1_chunk_ptr;

    pui1_chunk_ptr = pui1_chunk;
    
    memcpy(sz_fourCC, pui1_chunk_ptr, 4);
    sz_fourCC[4] = '\0';
    pui1_chunk_ptr += 4;


    memcpy(pt_curr_menu->sz_fourCC, sz_fourCC, 4);
    pt_curr_menu->sz_fourCC[4] = 0;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->ui4_chunk_size);
    pui1_chunk_ptr += 4;

    /* Get this objects ID */
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->i4_objectID);
    pui1_chunk_ptr += 4;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_media_source.i4_reserved);
    pui1_chunk_ptr += 4;
    
    LOADL_QWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_media_source.ui8_riff_offset);
    pui1_chunk_ptr += 8;
    
    *pui4_size = pui1_chunk_ptr - pui1_chunk;
    return MINFOR_OK;
}

static INT32 get_MEDT_Object(UINT8* pui1_chunk, UINT32* pui4_size, DIVX_MENU_OBJECT_T* pt_curr_menu)
{
    /* Divx Media Track object. */
    CHAR    sz_fourCC[5];
    UINT8*  pui1_chunk_ptr;

    pui1_chunk_ptr = pui1_chunk;
    
    memcpy(sz_fourCC, pui1_chunk_ptr, 4);
    sz_fourCC[4] = '\0';
    pui1_chunk_ptr += 4;


    memcpy(pt_curr_menu->sz_fourCC, sz_fourCC, 4);
    pt_curr_menu->sz_fourCC[4] = 0;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->ui4_chunk_size);
    pui1_chunk_ptr += 4;

    /* Get this objects ID */
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->i4_objectID);
    pui1_chunk_ptr += 4;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_media_track.ui4_media_sourceID);
    pui1_chunk_ptr += 4;

    memcpy(pt_curr_menu->Type.t_media_track.sz_trackID, pui1_chunk_ptr, 4);
    pt_curr_menu->Type.t_media_track.sz_trackID[4] = 0;
    pui1_chunk_ptr += 4;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_media_track.i4_start_time);
    pui1_chunk_ptr += 4;
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_media_track.i4_end_time);
    pui1_chunk_ptr += 4;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_media_track.i4_language_code_len);
    pui1_chunk_ptr += 4;
    if(pt_curr_menu->Type.t_media_track.i4_language_code_len > 0)
    {
        pt_curr_menu->Type.t_media_track.pi2_language_code = 
            (INT16*)divx_mem_alloc(pt_curr_menu->Type.t_media_track.i4_language_code_len);
        if (pt_curr_menu->Type.t_media_track.pi2_language_code == NULL)
        {
            return MINFOR_OUT_OF_MEMORY;
        }
        memcpy((INT8*)pt_curr_menu->Type.t_media_track.pi2_language_code, 
               pui1_chunk_ptr, 
               pt_curr_menu->Type.t_media_track.i4_language_code_len);
        pui1_chunk_ptr += pt_curr_menu->Type.t_media_track.i4_language_code_len;
    }

    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_media_track.i4_translation_lookupID);
    pui1_chunk_ptr += 4;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_media_track.i4_type_len);
    pui1_chunk_ptr += 4;
    if(pt_curr_menu->Type.t_media_track.i4_type_len > 0)
    {
        pt_curr_menu->Type.t_media_track.pi2_type = 
            (INT16*)divx_mem_alloc(pt_curr_menu->Type.t_media_track.i4_type_len);
        if (pt_curr_menu->Type.t_media_track.pi2_type == NULL)
        {
            return MINFOR_OUT_OF_MEMORY;
        }
        memcpy((INT8*)pt_curr_menu->Type.t_media_track.pi2_type, 
               pui1_chunk_ptr, 
               pt_curr_menu->Type.t_media_track.i4_type_len);
        pui1_chunk_ptr += pt_curr_menu->Type.t_media_track.i4_type_len;
    }
    
    *pui4_size = pui1_chunk_ptr - pui1_chunk;
    return MINFOR_OK;
}

static INT32 get_TITL_Object(UINT8* pui1_chunk, UINT32* pui4_size, DIVX_MENU_OBJECT_T* pt_curr_menu)
{
    /* Divx Title object. */
    CHAR    sz_fourCC[5];
    UINT8*  pui1_chunk_ptr;

    pui1_chunk_ptr = pui1_chunk;
    
    memcpy(sz_fourCC, pui1_chunk_ptr, 4);
    sz_fourCC[4] = '\0';
    pui1_chunk_ptr += 4;


    memcpy(pt_curr_menu->sz_fourCC, sz_fourCC, 4);
    pt_curr_menu->sz_fourCC[4] = 0;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->ui4_chunk_size);
    pui1_chunk_ptr += 4;

    /* Get this objects ID */
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->i4_objectID);
    pui1_chunk_ptr += 4;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_title.i4_translation_lookupID);
    pui1_chunk_ptr += 4;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_title.i4_default_audio_len);
    pui1_chunk_ptr += 4;
    if(pt_curr_menu->Type.t_title.i4_default_audio_len > 0)
    {
        pt_curr_menu->Type.t_title.pi2_default_audio_track = 
            (INT16*)divx_mem_alloc(pt_curr_menu->Type.t_title.i4_default_audio_len);
        if (pt_curr_menu->Type.t_title.pi2_default_audio_track == NULL)
        {
            return MINFOR_OUT_OF_MEMORY;
        }
        memcpy((INT8*)pt_curr_menu->Type.t_title.pi2_default_audio_track, 
               pui1_chunk_ptr, 
               pt_curr_menu->Type.t_title.i4_default_audio_len);
        pui1_chunk_ptr += pt_curr_menu->Type.t_title.i4_default_audio_len;
    }

    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_title.i4_default_subtitle_len);
    pui1_chunk_ptr += 4;
    if(pt_curr_menu->Type.t_title.i4_default_subtitle_len > 0)
    {
        pt_curr_menu->Type.t_title.pi2_default_subtitle_track = 
            (INT16*)divx_mem_alloc(pt_curr_menu->Type.t_title.i4_default_subtitle_len);
        if (pt_curr_menu->Type.t_title.pi2_default_subtitle_track == NULL)
        {
            return MINFOR_OUT_OF_MEMORY;
        }
        memcpy((INT8*)pt_curr_menu->Type.t_title.pi2_default_subtitle_track, 
               pui1_chunk_ptr, 
               pt_curr_menu->Type.t_title.i4_default_subtitle_len);
        pui1_chunk_ptr += pt_curr_menu->Type.t_title.i4_default_subtitle_len;
    }
    
    *pui4_size = pui1_chunk_ptr - pui1_chunk;
    return MINFOR_OK;
}

static INT32 get_CHAP_Object(UINT8* pui1_chunk, UINT32* pui4_size, DIVX_MENU_OBJECT_T* pt_curr_menu)
{
    /* Divx Chapter object. */
    CHAR    sz_fourCC[5];
    UINT8*  pui1_chunk_ptr;

    pui1_chunk_ptr = pui1_chunk;
    
    memcpy(sz_fourCC, pui1_chunk_ptr, 4);
    sz_fourCC[4] = '\0';
    pui1_chunk_ptr += 4;


    memcpy(pt_curr_menu->sz_fourCC, sz_fourCC, 4);
    pt_curr_menu->sz_fourCC[4] = 0;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->ui4_chunk_size);
    pui1_chunk_ptr += 4;

    /* Get this objects ID */
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->i4_objectID);
    pui1_chunk_ptr += 4;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_chapter.i4_translation_lookupID);
    pui1_chunk_ptr += 4;
        
    *pui4_size = pui1_chunk_ptr - pui1_chunk;
    return MINFOR_OK;
}

static INT32 get_TRTB_Object(UINT8* pui1_chunk, UINT32* pui4_size, DIVX_MENU_OBJECT_T* pt_curr_menu)
{
    /* Divx Translation Table object. */
    CHAR    sz_fourCC[5];
    UINT8*  pui1_chunk_ptr;

    pui1_chunk_ptr = pui1_chunk;
    
    memcpy(sz_fourCC, pui1_chunk_ptr, 4);
    sz_fourCC[4] = '\0';
    pui1_chunk_ptr += 4;


    memcpy(pt_curr_menu->sz_fourCC, sz_fourCC, 4);
    pt_curr_menu->sz_fourCC[4] = 0;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->ui4_chunk_size);
    pui1_chunk_ptr += 4;

    /* Get this objects ID */
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->i4_objectID);
    pui1_chunk_ptr += 4;
    
    pt_curr_menu->Type.t_translation_table.i4_reserved = 0;
        
    *pui4_size = pui1_chunk_ptr - pui1_chunk;
    return MINFOR_OK;
}

static INT32 get_TRLK_Object(UINT8* pui1_chunk, UINT32* pui4_size, DIVX_MENU_OBJECT_T* pt_curr_menu)
{
    /* Divx Translation Lookup object. */
    CHAR    sz_fourCC[5];
    UINT8*  pui1_chunk_ptr;

    pui1_chunk_ptr = pui1_chunk;
    
    memcpy(sz_fourCC, pui1_chunk_ptr, 4);
    sz_fourCC[4] = '\0';
    pui1_chunk_ptr += 4;


    memcpy(pt_curr_menu->sz_fourCC, sz_fourCC, 4);
    pt_curr_menu->sz_fourCC[4] = 0;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->ui4_chunk_size);
    pui1_chunk_ptr += 4;

    /* Get this objects ID */
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->i4_objectID);
    pui1_chunk_ptr += 4;
    
    pt_curr_menu->Type.t_translation_lookup.i4_reserved = 0;
        
    *pui4_size = pui1_chunk_ptr - pui1_chunk;
    return MINFOR_OK;
}

static INT32 get_TRAN_Object(UINT8* pui1_chunk, UINT32* pui4_size, DIVX_MENU_OBJECT_T* pt_curr_menu)
{
    /* Divx Translation object. */
    CHAR    sz_fourCC[5];
    UINT8*  pui1_chunk_ptr;

    pui1_chunk_ptr = pui1_chunk;
    
    memcpy(sz_fourCC, pui1_chunk_ptr, 4);
    sz_fourCC[4] = '\0';
    pui1_chunk_ptr += 4;


    memcpy(pt_curr_menu->sz_fourCC, sz_fourCC, 4);
    pt_curr_menu->sz_fourCC[4] = 0;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->ui4_chunk_size);
    pui1_chunk_ptr += 4;

    /* Get this objects ID */
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->i4_objectID);
    pui1_chunk_ptr += 4;
    
    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_translation.i4_language_code_len);
    pui1_chunk_ptr += 4;
    if(pt_curr_menu->Type.t_translation.i4_language_code_len > 0)
    {
        pt_curr_menu->Type.t_translation.pi2_language_code = 
            (INT16*)divx_mem_alloc(pt_curr_menu->Type.t_translation.i4_language_code_len);
        if (pt_curr_menu->Type.t_translation.pi2_language_code == NULL)
        {
            return MINFOR_OUT_OF_MEMORY;
        }
        memcpy((INT8*)pt_curr_menu->Type.t_translation.pi2_language_code, 
               pui1_chunk_ptr, 
               pt_curr_menu->Type.t_translation.i4_language_code_len);
        pui1_chunk_ptr += pt_curr_menu->Type.t_translation.i4_language_code_len;
    }

    LOADL_DWRD(pui1_chunk_ptr, pt_curr_menu->Type.t_translation.i4_value_len);
    pui1_chunk_ptr += 4;
    if(pt_curr_menu->Type.t_translation.i4_value_len > 0)
    {
        pt_curr_menu->Type.t_translation.pi2_value = 
            (INT16*)divx_mem_alloc(pt_curr_menu->Type.t_translation.i4_value_len);
        if (pt_curr_menu->Type.t_translation.pi2_value == NULL)
        {
            return MINFOR_OUT_OF_MEMORY;
        }
        memcpy((INT8*)pt_curr_menu->Type.t_translation.pi2_value, 
               pui1_chunk_ptr, 
               pt_curr_menu->Type.t_translation.i4_value_len);
        pui1_chunk_ptr += pt_curr_menu->Type.t_translation.i4_value_len;
    }
    
    *pui4_size = pui1_chunk_ptr - pui1_chunk;
    return MINFOR_OK;
}

static INT32 get_menu(UINT8* pui1_chunk, UINT32* pui4_size, DIVX_MENU_OBJECT_T* pt_curr_menu)
{
    INT32   i4_ret;
    UINT32  ui4_four_cc;          
    UINT8*  pui1_chunk_ptr;

    pui1_chunk_ptr = pui1_chunk;
    LOADB_DWRD(pui1_chunk_ptr, ui4_four_cc);

    switch (ui4_four_cc) 
    {
    case DIVX_MENU:
        i4_ret = get_MENU_Object(pui1_chunk, pui4_size, pt_curr_menu);
        break;
    case DIVX_WMEN:
        i4_ret = get_WMEN_Object(pui1_chunk, pui4_size, pt_curr_menu);
        break;
    case DIVX_BMEN:
        i4_ret = get_BMEN_Object(pui1_chunk, pui4_size, pt_curr_menu);
        break;
    case DIVX_MREC:
        i4_ret = get_MREC_Object(pui1_chunk, pui4_size, pt_curr_menu);
        break;
    case DIVX_ACTN:
        i4_ret = get_ACTN_Object(pui1_chunk, pui4_size, pt_curr_menu);
        break;
    case DIVX_STAC:
        i4_ret = get_STAC_Object(pui1_chunk, pui4_size, pt_curr_menu);
        break;
    case DIVX_ASAC:
        i4_ret = get_ASAC_Object(pui1_chunk, pui4_size, pt_curr_menu);
        break;
    case DIVX_PLAY:
        i4_ret = get_PLAY_Object(pui1_chunk, pui4_size, pt_curr_menu);
        break;
    case DIVX_PLY0:
        i4_ret = get_PLY0_Object(pui1_chunk, pui4_size, pt_curr_menu);
        break;
    case DIVX_MNTX:
        i4_ret = get_MNTX_Object(pui1_chunk, pui4_size, pt_curr_menu);
        break;
    case DIVX_BNTX:
        i4_ret = get_BNTX_Object(pui1_chunk, pui4_size, pt_curr_menu);
        break;
    case DIVX_LGMN:
        i4_ret = get_LGMN_Object(pui1_chunk, pui4_size, pt_curr_menu);
        break;
    case DIVX_MDIA:
        i4_ret = get_MDIA_Object(pui1_chunk, pui4_size, pt_curr_menu);
        break;
    case DIVX_MNTK:
        i4_ret = get_MNTK_Object(pui1_chunk, pui4_size, pt_curr_menu);
        break;
    case DIVX_MEDS:
        i4_ret = get_MEDS_Object(pui1_chunk, pui4_size, pt_curr_menu);
        break;
    case DIVX_MEDT:
        i4_ret = get_MEDT_Object(pui1_chunk, pui4_size, pt_curr_menu);
        break;
    case DIVX_TITL:
        i4_ret = get_TITL_Object(pui1_chunk, pui4_size, pt_curr_menu);
        break;
    case DIVX_CHAP:
        i4_ret = get_CHAP_Object(pui1_chunk, pui4_size, pt_curr_menu);
        break;
    case DIVX_TRTB:
        i4_ret = get_TRTB_Object(pui1_chunk, pui4_size, pt_curr_menu);
        break;
    case DIVX_TRLK:
        i4_ret = get_TRLK_Object(pui1_chunk, pui4_size, pt_curr_menu);
        break;
    case DIVX_TRAN:
        i4_ret = get_TRAN_Object(pui1_chunk, pui4_size, pt_curr_menu);
        break;
    default:
        i4_ret = MINFOR_INTERNAL_ERR;
        break;
    }
    return i4_ret;
}

static INT32 get_object_by_id(DIVX_MENU_OBJECT_ID_LIST_T*   pt_head_menu_id_node,
                              INT32                         i4_object_id,
                              DIVX_MENU_OBJECT_T**          ppt_menu)
{
    DIVX_MENU_OBJECT_ID_LIST_T* pt_menu_id_node;   

    pt_menu_id_node = pt_head_menu_id_node;
    *ppt_menu = NULL;

    if(i4_object_id < 0)
    {
        return MINFOR_OK;
    }
    
    while(pt_menu_id_node != NULL)
    {
        if (pt_menu_id_node->i4_objectID == i4_object_id)
        {
            *ppt_menu = pt_menu_id_node->pt_menu;
            break;
        }
        pt_menu_id_node = pt_menu_id_node->pt_next;
    }
#if 1
    if(*ppt_menu == NULL)
    {
        return MINFOR_INTERNAL_ERR;
    }
    else
#endif        
    {
        return MINFOR_OK;
    }
}

static INT32 get_MENU_reference(DIVX_MENU_OBJECT_T*         pt_curr_menu,
                                DIVX_MENU_OBJECT_ID_LIST_T* pt_head_menu_id_node)
{
    INT32    i4_ret;
    
    i4_ret = get_object_by_id(pt_head_menu_id_node, 
                              pt_curr_menu->Type.t_divx_media_manager.i4_starting_menuID, 
                              &(pt_curr_menu->Type.t_divx_media_manager.pt_starting_menu_obj));
    
    return i4_ret;
}

static INT32 get_WMEN_reference(DIVX_MENU_OBJECT_T*         pt_curr_menu,
                                DIVX_MENU_OBJECT_ID_LIST_T* pt_head_menu_id_node)
{
    INT32    i4_ret;
    
    /* background video object */
    i4_ret = get_object_by_id(pt_head_menu_id_node, 
                              pt_curr_menu->Type.t_divx_media_menu.i4_background_videoID, 
                              &(pt_curr_menu->Type.t_divx_media_menu.pt_background_video_obj));
    
    if (i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }

    /* background audio object */
    i4_ret = get_object_by_id(pt_head_menu_id_node, 
                              pt_curr_menu->Type.t_divx_media_menu.i4_background_audioID, 
                              &(pt_curr_menu->Type.t_divx_media_menu.pt_background_audio_obj));
    
    if (i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }

    /* starting button menu object */
    i4_ret = get_object_by_id(pt_head_menu_id_node, 
                              pt_curr_menu->Type.t_divx_media_menu.i4_starting_menu_itemID, 
                              &(pt_curr_menu->Type.t_divx_media_menu.pt_starting_menu_item_obj));
    
    if (i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }

    /* enter action */
    i4_ret = get_object_by_id(pt_head_menu_id_node, 
                              pt_curr_menu->Type.t_divx_media_menu.i4_enter_actionID, 
                              &(pt_curr_menu->Type.t_divx_media_menu.pt_enter_action_obj));
    
    if (i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }

    /* exit action */
    i4_ret = get_object_by_id(pt_head_menu_id_node, 
                              pt_curr_menu->Type.t_divx_media_menu.i4_exit_actionID, 
                              &(pt_curr_menu->Type.t_divx_media_menu.pt_exit_action_obj));
    
    if (i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }
    
    return MINFOR_OK;
}

static INT32 get_BMEN_reference(DIVX_MENU_OBJECT_T*         pt_curr_menu,
                                DIVX_MENU_OBJECT_ID_LIST_T* pt_head_menu_id_node)
{
    INT32   i4_ret;
    
    /* the object of the overlay(MediaTrack) */
    i4_ret = get_object_by_id(pt_head_menu_id_node, 
                              pt_curr_menu->Type.t_button_menu.i4_overlayID, 
                              &(pt_curr_menu->Type.t_button_menu.pt_overlay_obj));
    
    if (i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }
    
    /* the object of the button (ButtonMenu) by user UP */
    i4_ret = get_object_by_id(pt_head_menu_id_node, 
                              pt_curr_menu->Type.t_button_menu.i4_up_actionID, 
                              &(pt_curr_menu->Type.t_button_menu.pt_up_action_obj));
    
    if (i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }
    
    /* the object of the button (ButtonMenu) by user DOWN */
    i4_ret = get_object_by_id(pt_head_menu_id_node, 
                              pt_curr_menu->Type.t_button_menu.i4_down_actionID, 
                              &(pt_curr_menu->Type.t_button_menu.pt_down_action_obj));
    
    if (i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }
    
    /* the object of the button (ButtonMenu) by user LEFT */
    i4_ret = get_object_by_id(pt_head_menu_id_node, 
                              pt_curr_menu->Type.t_button_menu.i4_left_actionID, 
                              &(pt_curr_menu->Type.t_button_menu.pt_left_action_obj));
    
    if (i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }
    
    /* the object of the button (ButtonMenu) by user RIGHT */
    i4_ret = get_object_by_id(pt_head_menu_id_node, 
                              pt_curr_menu->Type.t_button_menu.i4_right_actionID, 
                              &(pt_curr_menu->Type.t_button_menu.pt_right_action_obj));
    
    if (i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }
    
    /* the object of the list of actions (ACTION) */
    i4_ret = get_object_by_id(pt_head_menu_id_node, 
                              pt_curr_menu->Type.t_button_menu.i4_select_actionID, 
                              &(pt_curr_menu->Type.t_button_menu.pt_select_action_obj));
    
    if (i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }
    
    return MINFOR_OK;
}

static INT32 get_STAC_reference(DIVX_MENU_OBJECT_T*         pt_curr_menu,
                                DIVX_MENU_OBJECT_ID_LIST_T* pt_head_menu_id_node)
{
    INT32   i4_ret;
    
    i4_ret = get_object_by_id(pt_head_menu_id_node, 
                              pt_curr_menu->Type.t_subtitle_select_action.i4_titleID, 
                              &(pt_curr_menu->Type.t_subtitle_select_action.pt_title_obj));
    
    if (i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }
    
    return MINFOR_OK;
}

static INT32 get_ASAC_reference(DIVX_MENU_OBJECT_T*         pt_curr_menu,
                                DIVX_MENU_OBJECT_ID_LIST_T* pt_head_menu_id_node)
{
    INT32    i4_ret;
    
    i4_ret = get_object_by_id(pt_head_menu_id_node, 
                              pt_curr_menu->Type.t_audio_select_action.i4_titleID, 
                              &(pt_curr_menu->Type.t_audio_select_action.pt_title_obj));
    
    if (i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }
    
    return MINFOR_OK;
}

static INT32 get_PLAY_reference(DIVX_MENU_OBJECT_T*         pt_curr_menu,
                                DIVX_MENU_OBJECT_ID_LIST_T* pt_head_menu_id_node)
{
    INT32   i4_ret;
    
    /* the object of the title(Title) to playback */
    i4_ret = get_object_by_id(pt_head_menu_id_node, 
                              pt_curr_menu->Type.t_play_action.i4_titleID, 
                              &(pt_curr_menu->Type.t_play_action.pt_title_obj));
    
    if (i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }

    /* the object for the chapter point(Chapter) */
    i4_ret = get_object_by_id(pt_head_menu_id_node, 
                              pt_curr_menu->Type.t_play_action.i4_media_objectID, 
                              &(pt_curr_menu->Type.t_play_action.pt_media_obj));
    
    if (i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }
    
    return MINFOR_OK;
}

static INT32 get_PLY0_reference(DIVX_MENU_OBJECT_T*         pt_curr_menu,
                                DIVX_MENU_OBJECT_ID_LIST_T* pt_head_menu_id_node)
{
    INT32   i4_ret;
    
    /* the object of the title(TITLE) to playback */
    i4_ret = get_object_by_id(pt_head_menu_id_node, 
                              pt_curr_menu->Type.t_play_from_curr_offset_action.i4_titleID, 
                              &(pt_curr_menu->Type.t_play_from_curr_offset_action.pt_title_obj));
    
    if (i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }

    /* the object for the chapter point(Chapter) */
    i4_ret = get_object_by_id(pt_head_menu_id_node, 
                              pt_curr_menu->Type.t_play_from_curr_offset_action.i4_media_objectID, 
                              &(pt_curr_menu->Type.t_play_from_curr_offset_action.pt_media_obj));
    
    if (i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }
    
    return MINFOR_OK;
}

static INT32 get_MNTX_reference(DIVX_MENU_OBJECT_T*         pt_curr_menu,
                                DIVX_MENU_OBJECT_ID_LIST_T* pt_head_menu_id_node)
{
    INT32 i4_ret;
    
    /* the object of the menu(DivXMediaMenu) into which to transition */
    i4_ret = get_object_by_id(pt_head_menu_id_node, 
                              pt_curr_menu->Type.t_menu_transition_action.i4_menuID, 
                              &(pt_curr_menu->Type.t_menu_transition_action.pt_menu_obj));
    
    if (i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }
    
    return MINFOR_OK;
}

static INT32 get_BNTX_reference(DIVX_MENU_OBJECT_T*         pt_curr_menu,
                                DIVX_MENU_OBJECT_ID_LIST_T* pt_head_menu_id_node)
{
    INT32   i4_ret;
    
    /* the object of the button(ButtonMenu) to become active */
    i4_ret = get_object_by_id(pt_head_menu_id_node, 
                              pt_curr_menu->Type.t_button_transition_action.i4_button_menuID, 
                              &(pt_curr_menu->Type.t_button_transition_action.pt_button_menu_obj));
    
    if (i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }
    
    return MINFOR_OK;
}

static INT32 get_LGMN_reference(DIVX_MENU_OBJECT_T*         pt_curr_menu,
                                DIVX_MENU_OBJECT_ID_LIST_T* pt_head_menu_id_node)
{
    INT32   i4_ret;
    
    /* the object of the menu(DivXMediaMenu) to be played first */
    i4_ret = get_object_by_id(pt_head_menu_id_node, 
                              pt_curr_menu->Type.t_language_menus.i4_starting_menuID, 
                              &(pt_curr_menu->Type.t_language_menus.pt_starting_menu_obj));
    
    if (i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }
    
    /* the object of the default menu(DivXMediaMenu) */
    i4_ret = get_object_by_id(pt_head_menu_id_node, 
                              pt_curr_menu->Type.t_language_menus.i4_root_menuID, 
                              &(pt_curr_menu->Type.t_language_menus.pt_root_menu_obj));
    
    if (i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }
    
    return MINFOR_OK;
}

static INT32 get_MEDT_reference(DIVX_MENU_OBJECT_T*         pt_curr_menu,
                                DIVX_MENU_OBJECT_ID_LIST_T* pt_head_menu_id_node)
{
    INT32   i4_ret;
    
    /* the object of the media source(MediaSource) which contains the data described */
    i4_ret = get_object_by_id(pt_head_menu_id_node, 
                              pt_curr_menu->Type.t_media_track.ui4_media_sourceID, 
                              &(pt_curr_menu->Type.t_media_track.pt_media_source_obj));
    
    if (i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }
    
    /* the object of the TranslationLookup which contains strings used to describe the media */
    i4_ret = get_object_by_id(pt_head_menu_id_node, 
                              pt_curr_menu->Type.t_media_track.i4_translation_lookupID, 
                              &(pt_curr_menu->Type.t_media_track.pt_translation_lookup_obj));
    
    if (i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }
    
    return MINFOR_OK;
}

static INT32 get_TITL_reference(DIVX_MENU_OBJECT_T*         pt_curr_menu,
                                DIVX_MENU_OBJECT_ID_LIST_T* pt_head_menu_id_node)
{
    INT32   i4_ret;
    
    /* the object of the TranslationLookup that contains strings used to describe this title */
    i4_ret = get_object_by_id(pt_head_menu_id_node, 
                              pt_curr_menu->Type.t_title.i4_translation_lookupID, 
                              &(pt_curr_menu->Type.t_title.pt_translation_lookup_obj));
    
    if (i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }
    
    return MINFOR_OK;
}

static INT32 get_CHAP_reference(DIVX_MENU_OBJECT_T*         pt_curr_menu,
                                DIVX_MENU_OBJECT_ID_LIST_T* pt_head_menu_id_node)
{
    INT32   i4_ret;
    
    /* the object of the TranslationLookup which contains strings used to describe this chapter */
    i4_ret = get_object_by_id(pt_head_menu_id_node, 
                     pt_curr_menu->Type.t_chapter.i4_translation_lookupID, 
                     &(pt_curr_menu->Type.t_chapter.pt_translation_lookup_obj));
    
    if (i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }
    
    return MINFOR_OK;
}

static INT32 get_reference(DIVX_MENU_OBJECT_ID_LIST_T* pt_head_menu_id_node)
{
    DIVX_MENU_OBJECT_ID_LIST_T* pt_menu_id_node;
    DIVX_MENU_OBJECT_T*         pt_menu_object;
    INT32                       i4_ret;
    UINT32                      ui4_four_cc;       

    pt_menu_id_node = pt_head_menu_id_node;
    while(pt_menu_id_node != NULL)
    {
        pt_menu_object = pt_menu_id_node->pt_menu;
        ui4_four_cc = dwBigFourCC(pt_menu_object->sz_fourCC[0], 
                                  pt_menu_object->sz_fourCC[1], 
                                  pt_menu_object->sz_fourCC[2],  
                                  pt_menu_object->sz_fourCC[3]);
        switch(ui4_four_cc)
        {
        case DIVX_MENU:
            i4_ret = get_MENU_reference(pt_menu_object, pt_head_menu_id_node);
            break;
        case DIVX_WMEN:
            i4_ret = get_WMEN_reference(pt_menu_object, pt_head_menu_id_node);
            break;
        case DIVX_BMEN:
            i4_ret = get_BMEN_reference(pt_menu_object, pt_head_menu_id_node);
            break;
        case DIVX_STAC:
            i4_ret = get_STAC_reference(pt_menu_object, pt_head_menu_id_node);
            break;
        case DIVX_ASAC:
            i4_ret = get_ASAC_reference(pt_menu_object, pt_head_menu_id_node);
            break;
        case DIVX_PLAY:
            i4_ret = get_PLAY_reference(pt_menu_object, pt_head_menu_id_node);
            break;
        case DIVX_PLY0:
            i4_ret = get_PLY0_reference(pt_menu_object, pt_head_menu_id_node);
            break;
        case DIVX_MNTX:
            i4_ret = get_MNTX_reference(pt_menu_object, pt_head_menu_id_node);
            break;
        case DIVX_BNTX:
            i4_ret = get_BNTX_reference(pt_menu_object, pt_head_menu_id_node);
            break;
        case DIVX_LGMN:
            i4_ret = get_LGMN_reference(pt_menu_object, pt_head_menu_id_node);
            break;
        case DIVX_MEDT:
            i4_ret = get_MEDT_reference(pt_menu_object, pt_head_menu_id_node);
            break;
        case DIVX_TITL:
            i4_ret = get_TITL_reference(pt_menu_object, pt_head_menu_id_node);
            break;
        case DIVX_CHAP:
            i4_ret = get_CHAP_reference(pt_menu_object, pt_head_menu_id_node);
            break;
        default:
            i4_ret = MINFOR_OK;
        }
        
        if (i4_ret != MINFOR_OK)
        {
            break;
        }
        pt_menu_id_node = pt_menu_id_node->pt_next;
    }
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  avi_minfo_menu_parse
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: MINFOR_OK                 The API is successful. 
 *          MINFOR_INTERNAL_ERR       The library initialize failed.
 ----------------------------------------------------------------------------*/                            
INT32 avi_minfo_menu_parse (MINFO_OBJ_T*    pt_minfo_obj)
{
    setpos_fct          pf_set_pos;    
    copybytes_fct       pf_copybyte;
    input4bytes_fct     pf_inp4byte;

    MINFO_AVI_RIFF_INF_T*   pt_avi_hdlr_obj;
    UINT8*                  pui1_menu_buf;
    UINT32                  ui4_four_cc;
    UINT32                  ui4_chk_sz;
    UINT32                  ui4_chk_data_sz;
    INT32                   i4_ret = MINFOR_INTERNAL_ERR;
    DIVX_MENU_OBJECT_T*         pt_curr_menu_obj;
    DIVX_MENU_OBJECT_T*         pt_par_menu_obj;

    DIVX_MENU_OBJECT_ID_LIST_T* pt_head_menu_id_node;
    DIVX_MENU_OBJECT_ID_LIST_T* pt_last_menu_id_node;
    DIVX_MENU_OBJECT_ID_LIST_T* pt_new_menu_id_node;
        

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_minfo_obj == NULL)
    {
        return MINFOR_INTERNAL_ERR;
    }
    if ((pt_minfo_obj->t_input_fct_tbl.pf_set_pos == NULL)
        || (pt_minfo_obj->t_input_fct_tbl.pf_copybytes== NULL)
        || (pt_minfo_obj->t_input_fct_tbl.pf_input4== NULL))
    {
        return MINFOR_INTERNAL_ERR;
    }
    DBG_INFO(("API: avi_minfo_menu_parse \r\n"));
    
    pf_set_pos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pf_copybyte = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    pf_inp4byte = pt_minfo_obj->t_input_fct_tbl.pf_input4;
    
    pt_avi_hdlr_obj = (MINFO_AVI_RIFF_INF_T*)pt_minfo_obj->pv_handler_obj;   

    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /* seek to MENU beginning */
    pf_set_pos(pt_minfo_obj, 
               pt_avi_hdlr_obj->m_u4CurParserOffset, 
               MEDIA_SEEK_BGN);
    
    /* check  -- "MENU" */
    pf_inp4byte(pt_minfo_obj, &ui4_four_cc);
    ui4_four_cc =_Swap4Bytes(ui4_four_cc);
    if (0 != x_memcmp(&ui4_four_cc, "MENU", 4))
    {
        /*not a valid AVI file*/
        return MINFOR_INTERNAL_ERR;    
    }
    pf_inp4byte(pt_minfo_obj, &ui4_chk_sz);
    ui4_chk_sz += 8;

    /* menu parser init first */
    i4_ret = divx_menu_parser_init(pt_minfo_obj, ui4_chk_sz);
    if (i4_ret != MINFOR_OK)
    {
        return i4_ret;
    }
    
    /* seek to MENU beginning again */
    pf_set_pos(pt_minfo_obj, 
               pt_avi_hdlr_obj->m_u4CurParserOffset, 
               MEDIA_SEEK_BGN);
    /* copy file data to temp buf */
    pf_copybyte(pt_minfo_obj, pt_avi_hdlr_obj->pui1_menu_buf, ui4_chk_sz, ui4_chk_sz);
    pui1_menu_buf = pt_avi_hdlr_obj->pui1_menu_buf;

    /* parser DivxMediaManager object first */ 
    pt_avi_hdlr_obj->pt_menu_object = (DIVX_MENU_OBJECT_T*)divx_mem_alloc(sizeof(DIVX_MENU_OBJECT_T));
    if (pt_avi_hdlr_obj->pt_menu_object == NULL)
    {
        divx_menu_parser_uninit(pt_minfo_obj);
        return MINFOR_OUT_OF_MEMORY;
    }
    pt_par_menu_obj = NULL;
    pt_curr_menu_obj = pt_avi_hdlr_obj->pt_menu_object;

    pt_curr_menu_obj->ui4_offset = pt_avi_hdlr_obj->m_u4CurParserOffset;
    pt_curr_menu_obj->pt_parent = NULL;
    pt_curr_menu_obj->pt_child_list = NULL;
    pt_curr_menu_obj->ui4_child_count = 0;
    i4_ret = get_menu(pui1_menu_buf, 
                      &ui4_chk_data_sz, 
                      pt_curr_menu_obj);
    if (i4_ret != MINFOR_OK)
    {
        divx_menu_parser_uninit(pt_minfo_obj);
        return i4_ret;
    }
    pt_avi_hdlr_obj->m_u4CurParserOffset += ui4_chk_data_sz;
    pui1_menu_buf += ui4_chk_data_sz;

    /* set menu object with ID list */
    pt_new_menu_id_node = 
        (DIVX_MENU_OBJECT_ID_LIST_T*)divx_mem_alloc(sizeof(DIVX_MENU_OBJECT_ID_LIST_T));
    if (pt_new_menu_id_node == NULL)
    {
        divx_menu_parser_uninit(pt_minfo_obj);
        return MINFOR_OUT_OF_MEMORY;
    }
    pt_new_menu_id_node->i4_objectID = pt_curr_menu_obj->i4_objectID;
    pt_new_menu_id_node->pt_menu = pt_curr_menu_obj;
    pt_new_menu_id_node->pt_next = NULL;
    pt_head_menu_id_node = pt_new_menu_id_node;
    pt_last_menu_id_node = pt_new_menu_id_node;

    while(1)
    {
        if (pt_curr_menu_obj == NULL)
        {
            break;
        }

        if (pt_avi_hdlr_obj->m_u4CurParserOffset 
           >= (pt_curr_menu_obj->ui4_offset + pt_curr_menu_obj->ui4_chunk_size))
        {
            DIVX_MENU_OBJECT_LIST_T* pt_last_menu_list;
            DIVX_MENU_OBJECT_LIST_T* pt_child;
            pt_par_menu_obj = pt_curr_menu_obj->pt_parent;
            if(pt_par_menu_obj == NULL)
            {
                break;
            }
            pt_last_menu_list = pt_par_menu_obj->pt_child_list;
            if (pt_last_menu_list == NULL)
            {
                pt_last_menu_list = 
                    (DIVX_MENU_OBJECT_LIST_T*)divx_mem_alloc(sizeof(DIVX_MENU_OBJECT_LIST_T));
                if (pt_last_menu_list == NULL)
                {
                    divx_menu_parser_uninit(pt_minfo_obj);
                    return MINFOR_OUT_OF_MEMORY;
                }
                pt_last_menu_list->pt_menu = pt_curr_menu_obj;
                pt_last_menu_list->pt_next = NULL;
                pt_par_menu_obj->pt_child_list = pt_last_menu_list;
            }
            else
            {
                while(pt_last_menu_list->pt_next != NULL)
                {
                    pt_last_menu_list = pt_last_menu_list->pt_next;
                }
                pt_child = (DIVX_MENU_OBJECT_LIST_T*)divx_mem_alloc(sizeof(DIVX_MENU_OBJECT_LIST_T));
                if (pt_child == NULL)
                {
                    divx_menu_parser_uninit(pt_minfo_obj);
                    return MINFOR_OUT_OF_MEMORY;
                }
                pt_child->pt_menu = pt_curr_menu_obj;
                pt_child->pt_next = NULL;
                pt_last_menu_list->pt_next = pt_child;
            }
            pt_par_menu_obj->ui4_child_count ++;
            pt_curr_menu_obj = pt_par_menu_obj;
            continue;
        }
        else
        {
            pt_par_menu_obj = pt_curr_menu_obj;
        }

        pt_curr_menu_obj = (DIVX_MENU_OBJECT_T*)divx_mem_alloc(sizeof(DIVX_MENU_OBJECT_T));
        if (pt_curr_menu_obj == NULL)
        {
            divx_menu_parser_uninit(pt_minfo_obj);
            return MINFOR_OUT_OF_MEMORY;
        }
        pt_curr_menu_obj->ui4_offset = pt_avi_hdlr_obj->m_u4CurParserOffset;
        pt_curr_menu_obj->pt_parent = pt_par_menu_obj;
        pt_curr_menu_obj->pt_child_list = NULL;
        pt_curr_menu_obj->ui4_child_count = 0;
        i4_ret = get_menu(pui1_menu_buf, 
                          &ui4_chk_data_sz, 
                          pt_curr_menu_obj);
        if (i4_ret != MINFOR_OK)
        {
            divx_menu_parser_uninit(pt_minfo_obj);
            return i4_ret;
        }
        pt_avi_hdlr_obj->m_u4CurParserOffset += ui4_chk_data_sz;
        pui1_menu_buf += ui4_chk_data_sz;

        DBG_INFO(("Current offset = %d \r\n", pt_curr_menu_obj->ui4_offset));
        DBG_INFO(("Menu type is %s \r\n", pt_curr_menu_obj->sz_fourCC));
        DBG_INFO(("Chunk size = %d \r\n", pt_curr_menu_obj->ui4_chunk_size));
        DBG_INFO(("Object ID = %d \r\n", pt_curr_menu_obj->i4_objectID));

        /* set menu object with ID list */
        pt_new_menu_id_node = 
            (DIVX_MENU_OBJECT_ID_LIST_T*)divx_mem_alloc(sizeof(DIVX_MENU_OBJECT_ID_LIST_T));
        if (pt_new_menu_id_node == NULL)
        {
            divx_menu_parser_uninit(pt_minfo_obj);
            return MINFOR_OUT_OF_MEMORY;
        }
        pt_new_menu_id_node->i4_objectID = pt_curr_menu_obj->i4_objectID;
        pt_new_menu_id_node->pt_menu = pt_curr_menu_obj;
        pt_new_menu_id_node->pt_next = NULL;
        pt_last_menu_id_node->pt_next = pt_new_menu_id_node;
        pt_last_menu_id_node = pt_last_menu_id_node->pt_next;
    }

    i4_ret = get_reference(pt_head_menu_id_node);
    if (i4_ret != MINFOR_OK)
    {
        divx_menu_parser_uninit(pt_minfo_obj);
        return i4_ret;
    }

    /* free menu temp buf after finish menu parser */
    if (pt_avi_hdlr_obj->pui1_menu_buf != NULL)
    {
        _mm_util_mem_free(pt_avi_hdlr_obj->pui1_menu_buf);
        pt_avi_hdlr_obj->pui1_menu_buf = NULL;
    }

    return MINFOR_OK;  
}

/*-----------------------------------------------------------------------------
 * Name:  avi_minfo_menu_free
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: MINFOR_OK                 The API is successful. 
 *          MINFOR_INTERNAL_ERR       The library initialize failed.
 ----------------------------------------------------------------------------*/   
INT32 avi_minfo_menu_free (MINFO_OBJ_T*    pt_minfo_obj)
{
    return divx_menu_parser_uninit(pt_minfo_obj);
}

