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
 * $RCSfile: sbtl_divx_page_inst_mngr.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description: 
 *         
 *         
 *---------------------------------------------------------------------------*/

#ifndef _SBTL_DIVX_PAGE_INST_MNGR_H_
#define _SBTL_DIVX_PAGE_INST_MNGR_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#ifndef __KERNEL__
#include "mutil/sbtl_engine/sbtl_widget.h"
#include "mutil/sbtl_engine/u_mm_sbtl_engine.h"

#else /*__KERNEL__*/

#include "../sbtl_widget.h"
#include "../u_mm_sbtl_engine.h"

#endif /*__KERNEL__*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

#define SUBPICT_PACKHDR_TIMESTAMP_SIZE  (0x1B)
#define SUBPICT_PACKHDR_SIZE            (0x35)
            
#define SUBTITLEPARSER_SUCCESS 0
#define SUBTITLEPARSER_BUFFER_TOO_SMALL 1
#define SUBTITLEPARSER_INVALID_DATA 2
            
            
typedef struct _DIVX_SUBPICT_COLOR_T
{
    UINT8 ui1_red;
    UINT8 ui1_green;
    UINT8 ui1_blue;
}DIVX_SUBPICT_COLOR_T;

/*
   DivX Subpicture Packet header
*/

typedef struct _DIVX_SUBPICT_HDR_T
{
 /* 
  * Duration format : [HH:MM:SS.XXX-hh:mm:ss.xxx]
  * NOTE: This string is not null terminated! 
 */
    UINT8 aui1_duration[27];

 /* Dimensions and coordinates */
    UINT16 ui2_width;         
    UINT16 ui2_height;        
    UINT16 ui2_left;          
    UINT16 ui2_top;           
    UINT16 ui2_right;         
    UINT16 ui2_bottom;        
    UINT16 ui2_field_offset;  

 /* 2-bit (4 color) palette */
    DIVX_SUBPICT_COLOR_T t_background;
    DIVX_SUBPICT_COLOR_T t_pattern;
    DIVX_SUBPICT_COLOR_T t_emphasis1;
    DIVX_SUBPICT_COLOR_T t_emphasis2;

 /* Rle data */
    UINT8* pui1_rle_data;
}DIVX_SUBPICT_HDR_T;

typedef struct _SBTL_DIVX_SUBALPHA_HDR_T
{
    /*0x00 = transparent, 0xFF = opaque*/
    UINT8 ui1_background_transparency;
    UINT8 ui1_pattern_transparency;
    UINT8 ui1_emphasis1_transparency;
    UINT8 ui1_emphasis2_transparency;
}SBTL_DIVX_SUBALPHA_HDR_T;


typedef enum
{
    DIVX_STATE_NONE,
    DIVX_STATE_DATA_RDY,
    DIVX_STATE_DISPLAY,
    DIVX_STATE_END
} DIVX_PAGE_STATE_T;


typedef struct _SBTL_DIVX_MENU_PAGE_T
{
    UINT8*                                      aui1_pes_buf;
    UINT32                                      ui4_pes_buf_size;
    DLIST_ENTRY_T(_SBTL_DIVX_MENU_PAGE_T)       t_link;
} SBTL_DIVX_MENU_PAGE_T;


typedef struct _SBTL_DIVX_PAGE_MNGR_PRIVATE_T
{
    BOOL                                b_page_exist;
    DIVX_PAGE_STATE_T                   e_state;
    
    HANDLE_T                            h_sbtl_wgl;
    PTS_T                               t_pts_s;
    PTS_T                               t_pts_e;
    HANDLE_T                            h_sem;
    SBTL_WGL_SBTL_REGION_T              t_rgn;
    GL_COLOR_T                          at_clut[16];
    BOOL                                b_hd;
    DIVX_SUBPICT_HDR_T*                 pt_subpict_hdr;
    SBTL_DIVX_SUBALPHA_HDR_T            t_subalpha_hdr;
    
    UINT8*                              aui1_pes_buf;
    UINT32                              ui4_pes_buf_size;
    HANDLE_T                            h_timer;

    PTS_T                               t_append_stc;
    
    /*overscan*/
    UINT8                               ui1_ovs_top;
    UINT8                               ui1_ovs_bottom;
    UINT8                               ui1_ovs_left;
    UINT8                               ui1_ovs_right;
    UINT32                              ui4_logical_w;
    UINT32                              ui4_logical_h;

    UINT16                              ui2_menu_item;
    
    DLIST_T(_SBTL_DIVX_MENU_PAGE_T)     t_menu_page_list;
}SBTL_DIVX_PAGE_MNGR_PRIVATE_T;

/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/
#endif /* _SBTL_DIVX_PAGE_INST_MNGR_H_ */






