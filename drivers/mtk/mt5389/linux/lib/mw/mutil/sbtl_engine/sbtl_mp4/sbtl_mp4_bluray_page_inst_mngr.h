/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2004, MediaTek Inc.
 * All rights reserved.
 * 
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.  
 *-----------------------------------------------------------------------------
 * $RCSfile: sbtl_mp4_bluray_page_inst_mngr.h,v $
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

#ifndef _SBTL_MP4_BLURAY_PAGE_INST_MNGR_H_
#define _SBTL_MP4_BLURAY_PAGE_INST_MNGR_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
/* subidx stream number, 32 is DVD's maximal sp stream number */
#define MP4_BLURAY_STRM_NS 32


typedef enum
{
    MP4_BLURAY_STATE_NONE,
    MP4_BLURAY_STATE_DATA_RDY,
    MP4_BLURAY_STATE_DISPLAY,
    MP4_BLURAY_STATE_END
} MP4_BLURAY_PAGE_STATE_T;

typedef struct MM_SBTL_OBJ
{
    UINT16         ui2_id;
    UINT8          ui1_version_number;
    UINT16         ui2_h_position;
    UINT16         ui2_v_position;
    UINT16         ui2_h_size;
    UINT16         ui2_v_size;
    UINT32         ui4_data_len;
    UINT8*         pui1_data;
}MM_SBTL_OBJ_T;

typedef struct MM_SBTL_OBJ_MNGR
{
    UINT8                   ui1_obj_cnt;
    MM_SBTL_OBJ_T             apt_obj_lst[2];
}MM_SBTL_OBJ_MNGR_T;


typedef struct MM_SBTL_CLUT
{
    UINT8          ui1_id;
    UINT8          ui1_version_number;
    GL_COLOR_T     at_color_entry[256];
}MM_SBTL_CLUT_T;

typedef struct MM_SBTL_RGN
{
    UINT8          ui1_id;
    UINT16         ui2_h_position;
    UINT16         ui2_v_position;
    UINT16         ui2_width;
    UINT16         ui2_height;

    UINT16         ui2_ref_obj_id;

    UINT8*         pui1_rgn_buf;
    UINT32         ui4_rgn_buf_len;

    BOOL           b_is_render;
}MM_SBTL_RGN_T;

typedef struct MM_SBTL_RGN_MNGR
{
    UINT8                   ui1_rgn_cnt;
    MM_SBTL_RGN_T             apt_rgn_lst[2];
}MM_SBTL_RGN_MNGR_T;

typedef struct MM_SBTL_RGN_COMP
{
    UINT16                  ui2_ref_obj_id;
    UINT8                   ui1_ref_rgn_id;
    UINT16                  ui2_obj_h_position;
    UINT16                  ui2_obj_v_position;
}MM_SBTL_RGN_COMP_T;


typedef struct SBTL_MP4_BLURAY_SUB_INFO
{
    UINT16               ui2_video_width;
    UINT16               ui2_video_height;
    UINT8                ui1_video_frame_rate;
    UINT16               ui2_page_version_number;
    UINT8                ui1_page_state;
    UINT8                ui1_ref_clut_id;
    UINT8                ui1_rgn_comp_cnt;
    
    MM_SBTL_RGN_COMP_T    t_rgn_comp_lst[2];

    MM_SBTL_RGN_MNGR_T     t_rgn_mngr;
    MM_SBTL_CLUT_T         t_clut;
    MM_SBTL_OBJ_MNGR_T     t_obj_mngr;    
}SBTL_MP4_BLURAY_SUB_INFO_T;

typedef struct _SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T
{
    BOOL                                b_page_exist;
    MP4_BLURAY_PAGE_STATE_T             e_state;

    BOOL                                b_is_empty_smaple;

    HANDLE_T                            h_sbtl_wgl;
    PTS_T                               t_pts_s;
    PTS_T                               t_pts_e;
    PTS_T                               t_append_stc;
    PTS_T                               t_pts_pre_s;
    HANDLE_T                            h_sem;
    SBTL_WGL_SBTL_REGION_T              t_rgn;
//    GL_COLOR_T                          at_clut_sub[256];
    
    UINT8*                              aui1_pes_buf;
    UINT32                              ui4_pes_buf_size;
    
    HANDLE_T                            h_timer;
    
    UINT32                              ui4_frame_width;
    UINT32                              ui4_frame_height;

    SBTL_MP4_BLURAY_SUB_INFO_T          t_sub_info;

    UINT32                              ui4_logical_w;
    UINT32                              ui4_logical_h;
    
}SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T;

/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/
#endif /* _SBTL_MP4_BLURAY_PAGE_INST_MNGR_H_ */

