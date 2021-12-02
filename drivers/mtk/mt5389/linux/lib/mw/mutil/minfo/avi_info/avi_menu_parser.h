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
 * $RCSfile: avi_menu_parser.h,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/2 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 0916b8010fb2d9ebf453b3d4efdbc5e9 $ Asa
 *
 * Description: 
 *         This file exports public API of the avi menu parser function
 *---------------------------------------------------------------------------*/

#ifndef _AVI_MENU_PARSER_H_
#define _AVI_MENU_PARSER_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#include "inc/u_common.h"
#include "handle/u_handle.h"
#include "mutil/minfo/minfo.h"
#include "mutil/mfmtrecg/u_mfmtrecg.h"

#else

#include "u_common.h"
#include "u_handle.h"
#include "../minfo.h"
#include "../../mfmtrecg/u_mfmtrecg.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
----------------------------------------------------------------------------*/

#define DIVX_MENU_OBJ_BUF_LENGTH     102400

/* Top level object in menu */
typedef struct _DIVX_MEDIA_MANAGER_T
{
    /* offset of the start of MRIF data (menu media source data) */
    UINT64                      ui8_offset;
    CHAR                        sz_versionID[4];
    INT32                       i4_menu_version;
    INT32                       i4_starting_menuID;
    INT32                       i4_default_language_len;
    INT16*                      pi2_default_language;
    /* reference menu object */
    struct _DIVX_MENU_OBJECT_T* pt_starting_menu_obj;
} DIVX_MEDIA_MANAGER_T;

/* The DivxMediaMenu Object */
typedef struct _DIVX_MEDIA_MENU_T
{
    INT32                       i4_background_videoID;
    INT32                       i4_background_audioID;
    INT32                       i4_starting_menu_itemID;
    INT32                       i4_enter_actionID;
    INT32                       i4_exit_actionID;
    INT32                       i4_menu_type_len;
    INT16*                      pi2_menu_type;
    INT32                       i4_menu_name_len;
    INT16*                      pi2_menu_name;
    /* reference menu object */
    struct _DIVX_MENU_OBJECT_T* pt_background_video_obj;
    struct _DIVX_MENU_OBJECT_T* pt_background_audio_obj;
    struct _DIVX_MENU_OBJECT_T* pt_starting_menu_item_obj;
    struct _DIVX_MENU_OBJECT_T* pt_enter_action_obj;
    struct _DIVX_MENU_OBJECT_T* pt_exit_action_obj;
} DIVX_MEDIA_MENU_T;

/* ButtonMenu Object */
typedef struct _BUTTON_MENU_T
{
    INT32                       i4_overlayID;
    INT32                       i4_up_actionID;
    INT32                       i4_down_actionID;
    INT32                       i4_left_actionID;
    INT32                       i4_right_actionID;
    INT32                       i4_select_actionID;
    INT32                       i4_button_name_len;
    INT16*                      pi2_button_name;
    /* reference menu object */
    struct _DIVX_MENU_OBJECT_T* pt_overlay_obj;
    struct _DIVX_MENU_OBJECT_T* pt_up_action_obj;
    struct _DIVX_MENU_OBJECT_T* pt_down_action_obj;
    struct _DIVX_MENU_OBJECT_T* pt_left_action_obj;
    struct _DIVX_MENU_OBJECT_T* pt_right_action_obj;
    struct _DIVX_MENU_OBJECT_T* pt_select_action_obj;
} BUTTON_MENU_T;

/* MenuRectangle Object */
typedef struct _MENU_RECTANGLE_T
{
    INT32   i4_left;
    INT32   i4_top;
    INT32 i4_width;
    INT32 i4_height;
} MENU_RECTANGLE_T;

/* Action Object */
typedef struct _ACTION_T
{
    INT32 i4_reserved;
} ACTION_T;

/* SubtitleSelectAction Object */
typedef struct _SUBTITLE_SELECT_ACTION_T
{
    INT32                       i4_titleID;
    CHAR                        sz_trackID[5];
    /* reference menu object */
    struct _DIVX_MENU_OBJECT_T* pt_title_obj;
} SUBTITLE_SELECT_ACTION_T;

/* AudioSelectAction Object */
typedef struct _AUDIO_SELECT_ACTION_T
{
    INT32                       i4_titleID;
    CHAR                        sz_trackID[5];
    /* reference menu object */
    struct _DIVX_MENU_OBJECT_T* pt_title_obj;
} AUDIO_SELECT_ACTION_T;

/* PlayAction Object */
typedef struct _PLAY_ACTION_T
{
    INT32                       i4_titleID;
    INT32                       i4_media_objectID;
    /* reference menu object */
    struct _DIVX_MENU_OBJECT_T* pt_title_obj;
    struct _DIVX_MENU_OBJECT_T* pt_media_obj;
} PLAY_ACTION_T;

/* PlayFromCurrentOffset Action Object */
typedef struct _PLAY_FROM_CURRENT_OFFSET_ACTION_T
{
    INT32                       i4_titleID;
    INT32                       i4_media_objectID;
    /* reference menu object */
    struct _DIVX_MENU_OBJECT_T* pt_title_obj;
    struct _DIVX_MENU_OBJECT_T* pt_media_obj;
} PLAY_FROM_CURRENT_OFFSET_ACTION_T;

/* MenuTransitionAction Object */
typedef struct _MENU_TRANSITION_ACTION_T
{
    INT32                       i4_menuID;
    /* reference menu object */
    struct _DIVX_MENU_OBJECT_T* pt_menu_obj;
} MENU_TRANSITION_ACTION_T;

/* ButtonTransitionAction Object */
typedef struct _BUTTON_TRANSITION_ACTION_T
{
    INT32                       i4_button_menuID;
    /* reference menu object */
    struct _DIVX_MENU_OBJECT_T* pt_button_menu_obj;
} BUTTON_TRANSITION_ACTION_T;

/* LanguageMenus Object */
typedef struct _LANGUAGE_MENUS_T
{
    INT32                       i4_language_code_len;
    INT16*                      pi2_language_code;
    INT32                       i4_starting_menuID;
    INT32                       i4_root_menuID;
    /* reference menu object */
    struct _DIVX_MENU_OBJECT_T* pt_starting_menu_obj;
    struct _DIVX_MENU_OBJECT_T* pt_root_menu_obj;
} LANGUAGE_MENUS_T;

/* Media Object */
typedef struct _MEDIA_T
{
    INT32 i4_reserved;
} MEDIA_T;

/* MenuTracks Object */
typedef struct _MENU_TRACKS_T
{
    INT32 i4_reserved;
} MENU_TRACKS_T;

/* MediaSource Object */
typedef struct _MEDIA_SOURCE_T
{
    INT32   i4_reserved;
    UINT64  ui8_riff_offset;
} MEDIA_SOURCE_T;

/* MediaTrack Object */
typedef struct _MEDIA_TRACK_T
{
    UINT32                      ui4_media_sourceID;
    CHAR                        sz_trackID[5];
    INT32                       i4_start_time;
    INT32                       i4_end_time;
    INT32                       i4_language_code_len;
    INT16*                      pi2_language_code;
    INT32                       i4_translation_lookupID;
    INT32                       i4_type_len;
    INT16*                      pi2_type;
    /* reference menu object */
    struct _DIVX_MENU_OBJECT_T* pt_media_source_obj;
    struct _DIVX_MENU_OBJECT_T* pt_translation_lookup_obj;
} MEDIA_TRACK_T;

/* Title Object */
typedef struct _TITLE_T
{
    INT32                       i4_translation_lookupID;
    INT32                       i4_default_audio_len;
    INT16*                      pi2_default_audio_track;
    INT32                       i4_default_subtitle_len;
    INT16*                      pi2_default_subtitle_track;
    /* reference menu object */
    struct _DIVX_MENU_OBJECT_T* pt_translation_lookup_obj;
} TITLE_T;

/* Chapter Object */
typedef struct _CHAPTER_T
{
    INT32                       i4_translation_lookupID;
    /* reference menu object */
    struct _DIVX_MENU_OBJECT_T* pt_translation_lookup_obj;
} CHAPTER_T;

/* TranslationTable Object */
typedef struct _TRANSLATION_TABLE_T
{
    INT32 i4_reserved;
} TRANSLATION_TABLE_T;

/* TranslationLookup Object */
typedef struct _TRANSLATION_LOOKUP_T
{
    INT32 i4_reserved;
} TRANSLATION_LOOKUP_T;

/* Translation Object */
typedef struct _TRANSLATION_T
{
    INT32   i4_language_code_len;
    INT16*  pi2_language_code;
    INT32   i4_value_len;
    INT16*  pi2_value;
} TRANSLATION_T;


typedef struct _DIVX_MENU_OBJECT_T
{
    /* menu object offset in the file */
    UINT32                              ui4_offset; 

    /* common data */
    /* FourCC code of this object type */
    CHAR                                sz_fourCC[5];
    /* the object size */
    UINT32                              ui4_chunk_size;
    /* the object ID tag */
    INT32                               i4_objectID;
    /* address of parent object */
    struct _DIVX_MENU_OBJECT_T*         pt_parent;
    /* head of children list */
    struct _DIVX_MENU_OBJECT_LIST_T*    pt_child_list;
    /* number of children */
    UINT32                              ui4_child_count;

    /* union object specific data */
    union Types
    {
        /* Top level object in menu */
        DIVX_MEDIA_MANAGER_T                t_divx_media_manager;
        DIVX_MEDIA_MENU_T                   t_divx_media_menu;
        BUTTON_MENU_T                       t_button_menu;
        MENU_RECTANGLE_T                    t_menu_rectangle;
        ACTION_T                            t_action;
        SUBTITLE_SELECT_ACTION_T            t_subtitle_select_action;
        AUDIO_SELECT_ACTION_T               t_audio_select_action;
        PLAY_ACTION_T                       t_play_action;
        PLAY_FROM_CURRENT_OFFSET_ACTION_T   t_play_from_curr_offset_action;
        MENU_TRANSITION_ACTION_T            t_menu_transition_action;
        BUTTON_TRANSITION_ACTION_T          t_button_transition_action;
        LANGUAGE_MENUS_T                    t_language_menus;
        MEDIA_T                             t_media;
        MENU_TRACKS_T                       t_menu_tracks;
        MEDIA_SOURCE_T                      t_media_source;
        MEDIA_TRACK_T                       t_media_track;
        TITLE_T                             t_title;
        CHAPTER_T                           t_chapter;
        TRANSLATION_TABLE_T                 t_translation_table;
        TRANSLATION_LOOKUP_T                t_translation_lookup;
        TRANSLATION_T                       t_translation;
    }Type;
}DIVX_MENU_OBJECT_T;

/* Menu Object List Node structure */
typedef struct _DIVX_MENU_OBJECT_LIST_T
{
    /* pointer to the actual object */
    DIVX_MENU_OBJECT_T*                 pt_menu;
    /* pointer to the next list object */
    struct _DIVX_MENU_OBJECT_LIST_T*    pt_next;
}DIVX_MENU_OBJECT_LIST_T;

/* Menu Object include ID List structure */
typedef struct _DIVX_MENU_OBJECT_ID_LIST_T
{
    INT32                               i4_objectID;
    /* pointer to the actual object */
    DIVX_MENU_OBJECT_T*                 pt_menu;
    /* pointer to the next list object */
    struct _DIVX_MENU_OBJECT_ID_LIST_T* pt_next;
}DIVX_MENU_OBJECT_ID_LIST_T;

/* buffer for menu object */
typedef struct _DIVX_MENU_OBJ_BUF_LIST_T
{
    UINT8*                              pui1_block_mem;
    UINT32                              ui4_menu_obj_buf_length;
    UINT32                              ui4_curr_buf_offset;
    struct _DIVX_MENU_OBJ_BUF_LIST_T*   pt_next;
}DIVX_MENU_OBJ_BUF_LIST_T;


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
extern INT32 avi_minfo_menu_parse (MINFO_OBJ_T*    pt_minfo_obj);
extern INT32 avi_minfo_menu_free (MINFO_OBJ_T*    pt_minfo_obj);

#endif /* _AVI_MENU_PARSER_H_ */

