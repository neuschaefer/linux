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
/*----------------------------------------------------------------------------*/
/*! @file u_img_tfx.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This file contains all the common data for transition effect APIs  
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_IMG
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_IMG_TFX_H_
#define _U_IMG_TFX_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_handle.h"
#include "u_gl.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define TFX_NFY_START               ( 5)    /**<start transition effect        */
#define TFX_NFY_FRAME_END           ( 4)    /**<transition effect one frame end         */
#define TFX_NFY_PAUSE               ( 3)    /**<transition effect pause        */
#define TFX_NFY_STOP                ( 2)    /**<transition effect stop        */
#define TFX_NFY_END                 ( 1)    /**<transition effect end        */
#define TFX_NFY_SURF_ERROR          (-1)    /**<surface error        */
#define TFX_NFY_SEND_NEXT_FRAME_ERR (-2)    /**<transition effect next frame error        */
#define TFX_NFY_RENDER_ERROR        (-3)    /**<render error        */

#define TFX_COMMON_FIELD                    \
    VOID                *pv_src,            \
    INT32               i4_src_x,           \
    INT32               i4_src_y,           \
    UINT32              ui4_src_pitch,      \
    GL_COLORMODE_T      e_src_cm,           \
    VOID                *pv_dst,            \
    INT32               i4_dst_x,           \
    INT32               i4_dst_y,           \
    UINT32              ui4_dst_pitch,      \
    GL_COLORMODE_T      e_dst_cm,           \
    UINT32              ui4_width,          \
    UINT32              ui4_height,         \
    UINT32              ui4_frame_index,    \
    UINT32              ui4_total_frame,    \
    BOOL                b_redraw_all,       \
    TFX_AUX_DATA_T      *pt_aux,            \
    HANDLE_T            h_src,              \
    HANDLE_T            h_dst    /**<transition effect field        */

/*------------------------------------------------------------------*/
/*! @enum IMG_TFX_EFFECT_T
 *  @brief  enum of transition effect
 *  @code
 *  typedef enum
 *  {
 *      TFX_WIPE_LEFT,
 *      TFX_WIPE_RIGHT,
 *      TFX_WIPE_UP,
 *      TFX_WIPE_DOWN,
 *      TFX_BOX_IN,
 *      TFX_BOX_OUT,
 *      TFX_BLENDING,
 *      TFX_DISSOLVE,
 *      TFX_INTERLACE_H,
 *      TFX_INTERLACE_V,
 *      TFX_SPLIT_IN_H,
 *      TFX_SPLIT_OUT_H,
 *      TFX_SPLIT_IN_V,
 *      TFX_SPLIT_OUT_V,
 *      TFX_OSD_BLENDING,
 *      TFX_TOTAL_NUMBER
 *  } IMG_TFX_EFFECT_T;
 *  @endcode
 *  @li@c  TFX_WIPE_LEFT                                     - wipe left
 *  @li@c  TFX_WIPE_RIGHT                                    - wipe right
 *  @li@c  TFX_WIPE_UP                                       - wipe up
 *  @li@c  TFX_WIPE_DOWN                                     - wipe down
 *  @li@c  TFX_BOX_IN                                        - box in
 *  @li@c  TFX_BOX_OUT                                       - box out
 *  @li@c  TFX_BLENDING                                      - blending
 *  @li@c  TFX_DISSOLVE                                      - dissolve
 *  @li@c  TFX_INTERLACE_H                                   - interlace horzontal
 *  @li@c  TFX_INTERLACE_V                                   - interlace vertical
 *  @li@c  TFX_SPLIT_IN_H                                    - split in horzontal
 *  @li@c  TFX_SPLIT_OUT_H                                   -split out horzontal
 *  @li@c  TFX_SPLIT_IN_V                                    - split in vertical
 *  @li@c  TFX_SPLIT_OUT_V                                   -split out vertical
 *  @li@c  TFX_OSD_BLENDING                                  - osd blending
 *  @li@c  TFX_TOTAL_NUMBER                                  - enum number
 */
/*------------------------------------------------------------------*/
typedef enum
{
    TFX_WIPE_LEFT,
    TFX_WIPE_RIGHT,
    TFX_WIPE_UP,
    TFX_WIPE_DOWN,
    TFX_BOX_IN,
    TFX_BOX_OUT,
    TFX_BLENDING,
    TFX_DISSOLVE,
    TFX_INTERLACE_H,
    TFX_INTERLACE_V,
    TFX_SPLIT_IN_H,
    TFX_SPLIT_OUT_H,
    TFX_SPLIT_IN_V,
    TFX_SPLIT_OUT_V,
    TFX_OSD_BLENDING,

    TFX_TOTAL_NUMBER
} IMG_TFX_EFFECT_T;

/*------------------------------------------------------------------*/
/*! @enum IMG_TFX_SCHEME_T
 *  @brief type of playing scheme
 *  @code
 *  typedef enum
 *  {
 *      TFX_DROP_FRAME,
 *      TFX_NO_DROP_FRAME
 *  } IMG_TFX_SCHEME_T;
 *  @endcode
 *  @li@c  TFX_DROP_FRAME                                    - drop frame
 *  @li@c  TFX_NO_DROP_FRAME                                 - no drop frame
 */
/*------------------------------------------------------------------*/
typedef enum
{
    TFX_DROP_FRAME,
    TFX_NO_DROP_FRAME
} IMG_TFX_SCHEME_T;

/*------------------------------------------------------------------*/
/*! @struct IMG_TFX_OSD_BLEND_T
 *  @brief osd alpha blend range
 *  @code
 *  typedef struct
 *  {
 *      UINT8           ui1_alpha_begin;
 *      UINT8           ui1_alpha_middle;
 *      UINT8           ui1_alpha_end;
 *      GL_HPLANE_T     h_plane;
 *  } IMG_TFX_OSD_BLEND_T;
 *  @endcode
 *  @li@c  ui1_alpha_begin                                   - begin of alpha blend
 *  @li@c  ui1_alpha_middle                                  - middle of alpha blend
 *  @li@c  ui1_alpha_end                                     - end of alpha blend
 *  @li@c  h_plane                                           - plane handle 
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT8           ui1_alpha_begin;
    UINT8           ui1_alpha_middle;
    UINT8           ui1_alpha_end;
    
    GL_HPLANE_T     h_plane;
} IMG_TFX_OSD_BLEND_T;

/*------------------------------------------------------------------*/
/*! @struct TFX_AUX_DATA_T
 *  @brief auxiliary data
 *  @code
 *  typedef struct
 *  {
 *      IMG_TFX_EFFECT_T        e_type;
 *      union
 *      {
 *          IMG_TFX_OSD_BLEND_T t_osd_blend_info;
 *      } u;
 *      GL_HSURFACE_T           h_aux_surf;
 *  } TFX_AUX_DATA_T;
 *  @endcode
 *  @li@c  e_type                                            - type of transition effect 
 *  @li@c  t_osd_blend_info                                  - osd blend information
 *  @li@c  h_aux_surf                                        - surface handle
 */
/*------------------------------------------------------------------*/
typedef struct
{
    IMG_TFX_EFFECT_T        e_type;

    union
    {
        IMG_TFX_OSD_BLEND_T t_osd_blend_info;
    } u;
    
    GL_HSURFACE_T           h_aux_surf;
} TFX_AUX_DATA_T;

/*------------------------------------------------------------------*/
/*! @brief Notify function of transition effect .
 *  @param [in] i4_status  specify the status of transition effect.
 *  @param [in] h_tfx       specify the transition effect handle.
 *  @param [in] pv_tag     specify the callback data.
 */
/*------------------------------------------------------------------*/
typedef VOID (*IMG_TFX_NFY_FCT_T)(INT32 i4_status, HANDLE_T h_tfx, VOID *pv_tag);


#ifdef MW_PHOTO_SHOW_ON_VDP

typedef INT32 (*IMG_TFX_RES_CALLBACK)   (GL_HSURFACE_T* h_surface_1,
										   GL_HSURFACE_T* h_surface_2,
										   BOOL           b_flag);  
#endif


/*------------------------------------------------------------------*/
/*! @brief render function of transition effect 
 *  @param [in] TFX_COMMON_FIELD specify the param for render.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*IMG_TFX_RENDER_FCT_T)(TFX_COMMON_FIELD);

/*------------------------------------------------------------------*/
/*! @struct TFX_FRAME_INFO_T
 *  @brief information of gif
 *  @code
 *  typedef struct
 *  {
 *      UINT32                  ui4_frame_number;
 *      UINT32                  ui4_duration;
 *      IMG_TFX_RENDER_FCT_T    pf_render_func;
 *  } TFX_FRAME_INFO_T;
 *  @endcode
 *  @li@c  ui4_frame_number                                  - frame number in gif
 *  @li@c  ui4_duration                                      - delay time of frame display
 *  @li@c  pf_render_func                                    - point to the render function
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT32                  ui4_frame_number;
    UINT32                  ui4_duration;
    IMG_TFX_RENDER_FCT_T    pf_render_func;
} TFX_FRAME_INFO_T;

#ifdef __cplusplus
}
#endif

#endif /* _U_IMG_TFX_H_ */
/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_IMG*/
/*----------------------------------------------------------------------------*/
