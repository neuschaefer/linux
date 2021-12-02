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
 * $RCSfile: x_wgl_animation.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/2 $
 * $SWAuthor: Al Hsieh $
 * $MD5HEX: 0e2e07a337bd3c5d07a4c05bf52feec8 $
 *
 * Description: 
 *         This header file contains public interfaces, constant and type 
 *         definitions of the Widget Library.
 *---------------------------------------------------------------------------*/
#ifdef WGL_ANIM_SUPPORT 
 
#ifndef _X_WGL_ANIMATION_H_
#define _X_WGL_ANIMATION_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_wgl.h"
#include "u_wgl_tools.h"
#include "wgl_sets.h"
#include "u_wgl_animation.h"
#include "wgl_anim.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define WGL_ANIM_INIT_CTRL_LOGIC            ((UINT32)0x00000001)
#define WGL_ANIM_INIT_DEFAULT_CNT           ((UINT32)0x00000002)


/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @struct WGL_MOVE_ANIM_T
 *  @brief This struct defines the move type animation iformation
 *  @code
 *  typedef struct _WGL_MOVE_ANIM_T
 *  {
 *      INT16                           i2_start_x;         
 *      INT16                           i2_start_y;         
 *      INT16                           i2_end_x;           
 *      INT16                           i2_end_y;           
 *      UINT16                          ui2_total_steps;     
 *      UINT16                          ui2_frame_interval;  
 *  } WGL_MOVE_ANIM_T;
 *  @endcode
 *  @li@c  i2_start_x                                        -starting position X in WGL_COORDS_PARENT
 *  @li@c  i2_start_y                                        -starting position Y in WGL_COORDS_PARENT
 *  @li@c  i2_end_x                                          -ending position X in WGL_COORDS_PARENT
 *  @li@c  i2_end_y                                          -ending position Y in WGL_COORDS_PARENT
 *  @li@c  ui2_total_steps                                   -how many steps for this animation
 *  @li@c  ui2_frame_interval                                -frame interval
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_MOVE_ANIM_T
{
    INT16                           i2_start_x;         /* starting position X in WGL_COORDS_PARENT*/
    INT16                           i2_start_y;         /* starting position Y in WGL_COORDS_PARENT*/    
    INT16                           i2_end_x;           /* ending position X in WGL_COORDS_PARENT*/
    INT16                           i2_end_y;           /* ending position Y in WGL_COORDS_PARENT*/ 
    UINT16                          ui2_total_steps;     /* how many steps for this animation */
    UINT16                          ui2_frame_interval;  /* frame interval*/ 
    wgl_app_anim_type_cb_fct        pf_type_cb;
    
} WGL_MOVE_ANIM_T;


/*------------------------------------------------------------------*/
/*! @struct WGL_SIZE_ANIM_T
 *  @brief This struct defines the size type animation information
 *  @code
 *  typedef struct _WGL_SIZE_ANIM_T
 *  {
 *      INT16                           i2_start_x;         
 *      INT16                           i2_start_y;         
 *      INT16                           i2_end_x;           
 *      INT16                           i2_end_y;           
 *      UINT16                          ui2_total_steps;     
 *      UINT16                          ui2_frame_interval;  
 *      UINT16                          ui2_start_w;         
 *      UINT16                          ui2_start_h;         
 *      UINT16                          ui2_end_w;           
 *      UINT16                          ui2_end_h;           
 *  } WGL_SIZE_ANIM_T;
 *  @endcode
 *  @li@c  i2_start_x                                        -starting position X in WGL_COORDS_PAREN
 *  @li@c  i2_start_y                                        -starting position Y in WGL_COORDS_PARENT
 *  @li@c  i2_end_x                                          -ending position X in WGL_COORDS_PARENT
 *  @li@c  i2_end_y                                          -ending position Y in WGL_COORDS_PARENT
 *  @li@c  ui2_total_steps                                   -how many steps for this animation
 *  @li@c  ui2_frame_interval                                -frame interval
 *  @li@c  ui2_start_w                                       -starting width
 *  @li@c  ui2_start_h                                       -starting height
 *  @li@c  ui2_end_w                                         -ending width
 *  @li@c  ui2_end_h                                         -ending height
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_SIZE_ANIM_T
{
    INT16                           i2_start_x;         /* starting position X in WGL_COORDS_PARENT*/
    INT16                           i2_start_y;         /* starting position Y in WGL_COORDS_PARENT*/    
    INT16                           i2_end_x;           /* ending position X in WGL_COORDS_PARENT*/
    INT16                           i2_end_y;           /* ending position Y in WGL_COORDS_PARENT*/ 
    UINT16                          ui2_total_steps;     /* how many steps for this animation */
    UINT16                          ui2_frame_interval;  /* frame interval*/
    UINT16                          ui2_start_w;         /* starting width*/
    UINT16                          ui2_start_h;         /* starting height*/    
    UINT16                          ui2_end_w;           /* ending width*/
    UINT16                          ui2_end_h;           /* ending height*/
    WGL_ANIM_STD_REF_TYPE           e_ref_type;
    wgl_app_anim_type_cb_fct        pf_type_cb;
} WGL_SIZE_ANIM_T;


/*------------------------------------------------------------------*/
/*! @struct WGL_ALPHA_ANIM_T
 *  @brief This struct defines the alpha type animation information.
 *  @code
 *  typedef struct _WGL_ALPHA_ANIM_T
 *  {
 *      UINT8                           ui1_start_alpha;     
 *      UINT8                           ui1_end_alpha;       
 *      UINT16                          ui2_total_steps;     
 *      UINT16                          ui2_frame_interval;  
 *  } WGL_ALPHA_ANIM_T;
 *  @endcode
 *  @li@c  ui1_start_alpha                                   -starting alpha
 *  @li@c  ui1_end_alpha                                     -ending alpha
 *  @li@c  ui2_total_steps                                   -how many steps for this animation
 *  @li@c  ui2_frame_interval                                -frame interval
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_ALPHA_ANIM_T
{
    UINT8                           ui1_start_alpha;     /* starting alpha*/    
    UINT8                           ui1_end_alpha;       /* ending alpha*/
    UINT16                          ui2_total_steps;     /* how many steps for this animation */
    UINT16                          ui2_frame_interval;  /* frame interval*/
    wgl_app_anim_type_cb_fct        pf_type_cb;
} WGL_ALPHA_ANIM_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_IMG_ANIM_T
 *  @brief This struct defines the image type animation information.
 *  @code
 *  typedef struct _WGL_IMG_ANIM_T
 *  {
 *      WGL_ANIM_PLAY_MODE_T            e_play_mode;     
 *      UINT8                           ui1_img_seq_id;
 *      UINT16                          ui2_frame_interval;  
 *      UINT16                          ui2_loop_count;       
 *      UINT16                          ui2_cust_len;
 *      ANIM_IMG_CUST_INFO_T*           pt_cust_info;
 *  } WGL_IMG_ANIM_T;
 *  @endcode
 *  @li@c  e_play_mode                                       -Animation play mode
 *  @li@c  ui1_img_seq_id                                    -Image sequence id
 *  @li@c  ui2_frame_interval                                -Frame interval
 *  @li@c  ui2_loop_count                                    -How many count loops
 *  @li@c  ui2_cust_len                                      -The number of custom information
 *  @li@c  pt_cust_info                                      -Custom mode informations
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_IMG_ANIM_T
{
    WGL_ANIM_PLAY_MODE_T            e_play_mode;     
    UINT8                           ui1_img_seq_id;
    UINT16                          ui2_frame_interval;  
    UINT16                          ui2_loop_count;       
    UINT16                          ui2_cust_len;
    ANIM_IMG_CUST_INFO_T*           pt_cust_info;
    wgl_app_anim_type_cb_fct        pf_type_cb;    
} WGL_IMG_ANIM_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_ANIM_BEGIN_INNER_T
 *  @brief This struct define animation information for animation internal using.
 *  @code
 *  typedef struct _WGL_ANIM_BEGIN_INNER_T
 *  {
 *      UINT16                          ui2_type; 
 *      WGL_SIZE_ANIM_T                 t_size;
 *      WGL_ALPHA_ANIM_T                t_alpha;
 *      WGL_IMG_ANIM_T                  t_img;
 *  } WGL_ANIM_BEGIN_INNER_T;
 *  @endcode
 *  @li@c  ui2_type                                          -The animation type
 *  @li@c  t_size                                            -Contain the size type animation info
 *  @li@c  t_alpha                                           -Contain the alpha type animation info
 *  @li@c  t_img                                             -Contain the image type animation info.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_ANIM_BEGIN_INNER_T
{
    UINT16                          ui2_type; 
    WGL_MOVE_ANIM_T                 t_move;
    WGL_SIZE_ANIM_T                 t_size;
    WGL_ALPHA_ANIM_T                t_alpha;
    WGL_SIZE_ANIM_T                 t_scale;
    WGL_IMG_ANIM_T                  t_img;
} WGL_ANIM_BEGIN_INNER_T;

/*Animation Begin Init Infomation*/
/*------------------------------------------------------------------*/
/*! @struct WGL_ANIM_BEGIN_T *  @brief This struct defines the animation begin need information
 *  @code
 *  typedef struct _WGL_ANIM_BEGIN_T
 *  {
 *      UINT16                          ui2_type; 
 *      WGL_MOVE_ANIM_T                 t_move;
 *      WGL_SIZE_ANIM_T                 t_size;
 *      WGL_ALPHA_ANIM_T                t_alpha;
 *      WGL_IMG_ANIM_T                  t_img;
 *  } WGL_ANIM_BEGIN_T;
 *  @endcode
 *  @li@c  ui2_type                                          -The animation type
 *  @li@c  t_move                                            -Contain the move type animation info.
 *  @li@c  t_size                                            -Contain the size type animation info
 *  @li@c  t_alpha                                           -Contain the alpha type animation info
 *  @li@c  t_img                                             -Contain the image type animation info.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_ANIM_BEGIN_T
{
    UINT16                          ui2_type; /*can be one or combination of four animate types*/     
    WGL_MOVE_ANIM_T                 t_move;
    WGL_SIZE_ANIM_T                 t_size;
    WGL_ALPHA_ANIM_T                t_alpha;
    WGL_IMG_ANIM_T                  t_img;
} WGL_ANIM_BEGIN_T;



/*------------------------------------------------------------------*/
/*! @struct WGL_ANIM_DATA_T 
 *  @brief This struct defines the animation data information including init internal data and step ifnormtion.
 *  @code
 *  typedef struct _WGL_ANIM_DATA_T
 *  {
 *   WGL_ANIM_BEGIN_INNER_T          t_init;    
 *   UINT16                          ui2_move_step;   
 *   UINT16                          ui2_size_step;   
 *   UINT16                          ui2_alpha_step;    
 *   UINT16                          ui2_img_step;
 *   UINT16                          ui2_brth_step;   
 *   UINT16                          ui2_scale_step;   
 *   UINT32                          ui4_max_step; 
 *   BOOL                            b_loop_ntf_flg; 
 *   UINT16                          ui2_move_idx;
 *   UINT16                          ui2_size_idx;
 *   UINT16                          ui2_alpha_idx;
 *   UINT16                          ui2_img_idx;
 *   UINT16                          ui2_scale_idx;
 *   UINT16                          ui2_breathe_idx;
 *  } WGL_ANIM_DATA_T;
 *  @endcode
 *  @li@c  t_init                                          -The animation initial data informtion that is beginned.
 *  @li@c  ui2_move_step                            -The current move animation step.
 *  @li@c  ui2_size_step                              -The current size animation step.
 *  @li@c  ui2_alpha_step                            -The current alpha animation step.
 *  @li@c  ui2_img_step                              -The current image animation step.
 *  @li@c  ui2_brth_step                              -The current breathe animation step.
 *  @li@c  ui2_scale_step                             -The current scale animation step.
 *  @li@c  ui4_max_step                              -The current total animation step.
 *  @li@c  b_loop_ntf_flg                             -It is loop image aniamtion notify flag.
 *  @li@c  ui2_move_idx                              -The size type animation callback function
 *  @li@c  ui2_size_idx                                -The size type animation callback function
 *  @li@c  ui2_alpha_idx                              -The size type animation callback function
 *  @li@c  ui2_img_idx                                -The size type animation callback function
 *  @li@c  ui2_scale_idx                              -The size type animation callback function
 *  @li@c  ui2_breathe_idx                           -The size type animation callback function
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_ANIM_DATA_T
{
    WGL_ANIM_BEGIN_INNER_T          t_init;    
    UINT16                          ui2_move_step;   
    UINT16                          ui2_size_step;   
    UINT16                          ui2_alpha_step;    
    UINT16                          ui2_img_step;
    UINT16                          ui2_brth_step;   
    UINT16                          ui2_scale_step;   
    UINT32                          ui4_max_step; /*max image step*/
    BOOL                            b_loop_ntf_flg; 
    UINT16                          ui2_move_idx;
    UINT16                          ui2_size_idx;
    UINT16                          ui2_alpha_idx;
    UINT16                          ui2_img_idx;
    UINT16                          ui2_scale_idx;
    UINT16                          ui2_breathe_idx;
} WGL_ANIM_DATA_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_ANIM_DATA_T 
 *  @brief This struct defines the animation data information including init internal data and step ifnormtion.
 *  @code
 *  typedef struct _WGL_ANIM_CONTROL_LOGIC_T
 *  {
 *    HANDLE_T                        h_widget;
 *   WGL_ANIM_ATTR_TYPE              e_anim_attr;
 *   WGL_ANIM_TICK_DATA              t_current_data;
 *   UINT16                          ui2_anim_total;
 *   UINT16                          ui2_bgn_last;    
 *   UINT16                          ui2_bgn_cur;    
 *   WGL_ANIM_BEGIN_DATA_T*          pt_begin;           
 *  
 *   UINT16                          ui2_state;    
 *   WGL_ANIM_DATA_T                 t_data;
 *   UINT32                          ui4_tick_interval; 
 *   UINT32                          ui4_start_tick; 
 *   UINT32                          ui4_total_end_tick; 
 *   UINT32                          ui4_img_tick_start; 
 *   UINT32                          ui4_alpha_tick_start; 
 *   UINT32                          ui4_size_tick_start;   
 *   UINT32                          ui4_move_tick_start; 
 *   UINT32                          ui4_cust_tick;      
 *   UINT16                          ui2_img_seq_max_len; 
 *   UINT16                          ui2_begin_flag;
 *   WGL_ANIM_CB_T                   t_cond_cb;          
 *   WGL_ANIM_IMG_SEQ_T*             pt_img_seq;                           
 *  } WGL_ANIM_DATA_T;
 *  @endcode
 *  @li@c  h_widget                      -The aniamtion associated widget.
 *  @li@c  e_anim_attr                  -The animation attribute:
 *                                                    WGL_ANIM_ATTR_WIDGET:it is widget  aniamtion attribute;
 *                                                    WGL_ANIM_ATTR_WIDGET_ELEMENT:the widget element aniamtion .
 *  @li@c  t_current_data               -Contain the current animation data information.
 *  @li@c  ui2_anim_total               -The animation type total number.One animation type has WGL_ANIM_BEGIN_DATA_T to indicate.
 *  @li@c  ui2_bgn_last                  -The previos animation index that is beginned.
 *  @li@c  ui2_bgn_cur                  -The current animation index that is beginned.
 *  @li@c  pt_begin                       -Contian all the animation to begin data information.
 *  @li@c  ui2_state                       -The animation state.
 *  @li@c  t_data                           -The current animation data that is doing.
 *  @li@c  ui4_tick_interval             -system tick interval.
 *  @li@c  ui4_start_tick                 -animation start tick
 *  @li@c  ui4_total_end_tick          -The animation total end tick,if it is WGL_ANIM_INFINITE_VALUE,it indicate not end util stop or destroy animation.
 *  @li@c  ui4_img_tick_start          -The image animation start tick if it has been doing image animation.
 *  @li@c  ui4_alpha_tick_start        -The alpha animation start tick if it has been doing alpha animation.
 *  @li@c  ui4_size_tick_start          -The size animation start tick if it has been doing size animation.
 *  @li@c  ui4_move_tick_start        -The move animation start tick if it has been doing move animation.
 *  @li@c  ui4_cust_tick                  -The image animation start tick  for custom mode if it has been doing image animation.
 *  @li@c  ui2_img_seq_max_len      -The image sequence max length for setting.
 *  @li@c  ui2_begin_flag                -The animation type that is doing.
 *  @li@c  pt_img_seq                    -The image sequence information.
 */
/*------------------------------------------------------------------*/

typedef struct _WGL_ANIM_CONTROL_LOGIC_T
{
#ifdef CLI_LVL_ALL     
    UINT32                          ui4_mem_size;/*this field must be the first field in the structure*/
#endif  
    /*For animation is an attribute of widget*/
    HANDLE_T                        h_widget;
    WGL_ANIM_ATTR_TYPE              e_anim_attr;
    WGL_ANIM_TICK_DATA              t_current_data;
    UINT16                          ui2_anim_total;
    UINT16                          ui2_bgn_last;    
    UINT16                          ui2_bgn_cur;    
    WGL_ANIM_BEGIN_DATA_T*          pt_begin;           
    
    UINT16                          ui2_state;      
    WGL_ANIM_INF_T                  t_info;
    WGL_ANIM_DATA_T                 t_data;             
    UINT32                          ui4_tick_interval;  
    UINT32                          ui4_start_tick;  
    UINT32                          ui4_total_end_tick;  
    UINT32                          ui4_img_tick_start;     
    UINT32                          ui4_alpha_tick_start;   
    UINT32                          ui4_size_tick_start;    
    UINT32                          ui4_move_tick_start;    
    UINT32                          ui4_cust_tick;      
    UINT16                          ui2_img_seq_max_len;
    UINT16                          ui2_begin_flag;
    WGL_ANIM_IMG_SEQ_T*             pt_img_seq;                           
} WGL_ANIM_CONTROL_LOGIC_T;


/*-----------------------------------------------------------------------------
                    functions
 ----------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @brief Initial the global animation path lib .If it will use animation path mode,it should initial it.
 *  @param  -
 *  @note    -
 *  @see  -
 *  @return  WGLR_OK               -Initial Ok.
 *  @retval     
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_anim_path_init(VOID);

/*------------------------------------------------------------------*/
/*! @brief  Adding the path mode to path lib.
 *  @param[In]  -pt_path_mode: contain the path mode information.Every path information has path id to indicate.
 *  @param[In]  -ui2_path_cnt: The count of path mode to add.
 *  @param[Out]  -pui2_added_cnt: The added number..
 *  @note    -
 *  @see  -
 *  @return  WGLR_OK               -Add sucessful.
 *               WGLR_INV_ARG      -Some parameter is not right.
 *  @retval     
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_anim_lib_add_path(
    IN WGL_ANIMATION_PATH_MODE_T*     pt_path_mode,
    IN UINT16                         ui2_path_cnt,
    OUT UINT16*                       pui2_added_cnt);

/*------------------------------------------------------------------*/
/*! @brief Remove the path mode from the path lib.
 *  @param    -pui4_path_id: The path id that to remove.
 *  @note    -ui2_path_cnt: The remove number.
 *  @see  -
 *  @return  WGLR_OK               -Remove path mode successful
 *               WGLR_INV_ARG      -Some parameter is not right.
 *  @retval     
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_anim_lib_remove_path(
    IN UINT32*                        pui4_path_id,
    IN UINT16                         ui2_path_cnt);

/*------------------------------------------------------------------*/
/*! @brief  Get path mode information by the path id.
 *  @param[In]  -ui4_path_id: The id for this path mode.
 *  @note    -
 *  @see  -
 *  @return  WGLR_OK               -Get path mode successful
 *               WGLR_INV_ARG      -Some parameter is not right.
 *  @retval     
 */
/*------------------------------------------------------------------*/
extern BOOL x_wgl_anim_lib_get_path(
    IN  UINT32                         ui4_path_id,
    OUT WGL_ANIMATION_PATH_MODE_T*     pt_path_mode);

/*------------------------------------------------------------------*/
/*! @brief  Get the path mode total number in the path lib.
 *  @param[Out]   -The count of path mode in the path lib
 *  @note    -
 *  @see  -
 *  @return  WGLR_OK               -Successful.
 *               WGLR_INV_ARG      -Some parameter is not right.
 *  @retval     
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_anim_lib_get_path_cnt(
        OUT UINT32*                        pui4_count);

/*------------------------------------------------------------------*/
/*! @brief Add the default action for e_type_widget type widget.
 *  @param[In]e_type_widget   -The widget type that will be added action map.
 *  @param[In]pt_action_map   -Action information.
 *  @param[In]ui2_count   -The count of action for this widget type
 *  @note    -
 *  @see  -
 *  @return  WGLR_OK               -Successful.
 *               WGLR_INV_ARG      -Some parameter is not right.
 *  @retval     
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_anim_add_default_actmap(
     IN HANDLE_TYPE_T                       e_type_widget,
     IN WGL_ANIM_ACTION_MAP_T*              pt_action_map,
     IN UINT16                              ui2_count);

/*------------------------------------------------------------------*/
/*! @brief Remove the action map for the special widget type.
 *  @param[In]e_type_widget    -The widget type
 *  @param[In]b_remove_all     -True:it will all remove action for the special type. False:it will remove the special action.
 *  @param[In]e_anim_action_type  -The action type,it is ingore when b_remove_all is true.
 *  @note    -
 *  @see  -
 *  @return  WGLR_OK               -Successful.
 *               WGLR_INV_ARG      -Some parameter is not right.
 *  @retval     
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_anim_remove_default_actmap(
    IN HANDLE_TYPE_T                        e_type_widget,
    IN BOOL                                 b_remove_all,
    IN WGL_ANIM_ACTION_TYPE_T               e_anim_action_type);

/*------------------------------------------------------------------*/
/*! @brief Get the action data information by special action of special widget type.
 *  @param[In]e_type_widget    -The widget type
 *  @param[In]e_anim_action_type    -The action type
 *  @param[Out]ppt_action_map    -Point to the action map data.
 *  @note    -
 *  @see  -
 *  @return  WGLR_OK               -
 *               WGLR_INV_ARG      -
 *  @retval     
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_anim_get_default_actmap(
  IN  HANDLE_TYPE_T                         e_type_widget,
  IN  WGL_ANIM_ACTION_TYPE_T                e_anim_action_type,
  OUT WGL_ANIM_ACTION_MAP_T**               ppt_action_map);

/*------------------------------------------------------------------*/
/*! @brief Set the listbox using default action map mode or not.
 *  @param[In]b_flag   -True:listbox uses default action map mode; False,not.
 *  @note    -
 *  @see  -
 *  @return  WGLR_OK               -
 *               WGLR_INV_ARG      -
 *  @retval     
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_set_lb_default_anim(BOOL b_flag);

/*------------------------------------------------------------------*/
/*! @brief Check the listbox using default action map mode or not.
 *  @param
 *  @note    -
 *  @see  -
 *  @return  TRUE               -Listbox will use default action map mode.
 *               FALSE              -Listbox will not use default action map mode.
 *  @retval     
 */
/*------------------------------------------------------------------*/
extern BOOL x_wgl_lb_default_anim_en(VOID);


#ifdef __cplusplus
}
#endif

#endif

#endif

