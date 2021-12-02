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
 * $RCSfile: sbtl_mp4_vobsub_page_inst_mngr.h,v $
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

#ifndef _SBTL_MP4_VOBSUB_PAGE_INST_MNGR_H_
#define _SBTL_MP4_VOBSUB_PAGE_INST_MNGR_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
/* subidx stream number, 32 is DVD's maximal sp stream number */
#define MP4_VOBSUB_STRM_NS 32


typedef enum
{
    MP4_VOBSUB_STATE_NONE,
    MP4_VOBSUB_STATE_DATA_RDY,
    MP4_VOBSUB_STATE_DISPLAY,
    MP4_VOBSUB_STATE_END
} MP4_VOBSUB_PAGE_STATE_T;


typedef struct _SBTL_MP4_VOBSUB_SUB_INFO_T
{
    UINT8*          aui1_spu_data;
    UINT8*          aui1_rle_data;
    UINT8*          aui1_decoded;
    
    UINT32          ui4_packetsize;
    UINT32          ui4_datasize;
    
    BOOL            b_pallete;

    BOOL            b_contrast;
    UINT16          ui2_contrast;
    
    BOOL            b_data_offset;
    UINT16          aui2_data_offset[2];
    INT32           i4_field_offset;
    INT32           i4_rle_size;

    BOOL            b_forced;

    BOOL            b_start_pts_offset;
    PTS_T           t_start_pts_offset;

    BOOL            b_end_pts_offset;
    PTS_T           t_end_pts_offset;

    BOOL            b_display_area;
    INT32           i4_left;
    INT32           i4_top;
    INT32           i4_right;
    INT32           i4_bottom;
    UINT16          ui2_width;
    UINT16          ui2_height;
    
}SBTL_MP4_VOBSUB_SUB_INFO_T;

typedef struct _SBTL_MP4_VOBSUB_PAGE_MNGR_PRIVATE_T
{
    BOOL                                b_page_exist;
    MP4_VOBSUB_PAGE_STATE_T             e_state;

    HANDLE_T                            h_sbtl_wgl;
    PTS_T                               t_pts_s;
    PTS_T                               t_pts_e;
    PTS_T                               t_append_stc;
    HANDLE_T                            h_sem;
    SBTL_WGL_SBTL_REGION_T              t_rgn;
    GL_COLOR_T                          at_clut_idx[16];
    GL_COLOR_T                          at_clut_sub[16];
    UINT8*                              aui1_pes_buf;
    PTS_T                               t_pts;
    UINT32                              ui4_pes_buf_size;
    UINT32                              ui4_pes_cur_idx;
    HANDLE_T                            h_timer;
    
    UINT32                              ui4_frame_width;
    UINT32                              ui4_frame_height;

    SBTL_MP4_VOBSUB_SUB_INFO_T          t_sub_info;

    UINT32                              ui4_logical_w;
    UINT32                              ui4_logical_h;
    
}SBTL_MP4_VOBSUB_PAGE_MNGR_PRIVATE_T;

/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/
#endif /* _SBTL_MP4_VOBSUB_PAGE_INST_MNGR_H_ */






