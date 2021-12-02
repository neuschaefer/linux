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
/*! @file u_wgl_animation.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains public interfaces, constant and type 
 *         definitions of the Widget Library.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_WGL_ANIMATION   Animation Widget
 *
 *  @ingroup groupMW_WGL
 *  @brief Provides definitions and exported function of the widget animation.
 *
 *  %MW Application API would be used by TV Application, lot of these APIs would
 *   be wrapped from Exported API
 *
 *  @see groupMW_WGL
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_WGL_ANIMATION_H_
#define _U_WGL_ANIMATION_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_wgl.h"
#include "u_wgl_tools.h"



#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define WGL_ANIM_MAX_PATH_CNT_ONCE       10 /**< Maximum animation path count once is 10*/

#define WGL_ANIM_MAX_MAP_CNT 10 /**< Maximum action animation map table count is 10 at present*/

/**< The widget animation action type define*/
typedef UINT8 WGL_ANIM_ACTION_TYPE_T;
#define  WGL_ANIM_ACTION_INGORE   ((WGL_ANIM_ACTION_TYPE_T)    0x00)/*<Ingore action type*/
#define  WGL_ANIM_ACTION_FOCUS    ((WGL_ANIM_ACTION_TYPE_T)    0x01)/*<Focus action type*/
#define  WGL_ANIM_ACTION_UNFOCUS  ((WGL_ANIM_ACTION_TYPE_T)    0x02)/*<unfocus action type*/
#define  WGL_ANIM_ACTION_UP       ((WGL_ANIM_ACTION_TYPE_T)    0x03)/*<up action type*/
#define  WGL_ANIM_ACTION_DOWN     ((WGL_ANIM_ACTION_TYPE_T)    0x04)/*<down action type*/
#define  WGL_ANIM_ACTION_LEFT     ((WGL_ANIM_ACTION_TYPE_T)    0x05)/*<left action type*/
#define  WGL_ANIM_ACTION_RIGHT    ((WGL_ANIM_ACTION_TYPE_T)    0x06)/*<right action type*/
#define  WGL_ANIM_ACTION_ENABLE   ((WGL_ANIM_ACTION_TYPE_T)    0x07)/*<enable action type*/
#define  WGL_ANIM_ACTION_DISABLE  ((WGL_ANIM_ACTION_TYPE_T)    0x08) /*<disable action type*/
#define  WGL_ANIM_ACTION_HIGHLIGHT      ((WGL_ANIM_ACTION_TYPE_T)    0x09) /*<disable action type*/
#define  WGL_ANIM_ACTION_UNHIGHLIGHT    ((WGL_ANIM_ACTION_TYPE_T)    0x0A) /*<disable action type*/
#define  WGL_ANIM_ACTION_PAGE_UP        ((WGL_ANIM_ACTION_TYPE_T)    0x0B) /*<disable action type*/
#define  WGL_ANIM_ACTION_PAGE_DOWN      ((WGL_ANIM_ACTION_TYPE_T)    0x0C) /*<disable action type*/

#define  WGL_ANIM_ACTION_START    WGL_ANIM_ACTION_INGORE/*<Ingore action type*/
#define  WGL_ANIM_ACTION_END      WGL_ANIM_ACTION_PAGE_DOWN /*<The last action type*/

#define  WGL_ANIM_NULL_PAHT_ID                   ((UINT32)0xFFFF)/*<The null path id value*/

#define WGL_ANIM_DATA_FLAG_IGORE                  MAKE_BIT_MASK_32(0)/*<The animation data ingore mask*/
#define WGL_ANIM_DATA_FLAG_DATA_INF               MAKE_BIT_MASK_32(1)/*<The animation data information mask*/
#define WGL_ANIM_DATA_FLAG_PATH_MODE_INF          MAKE_BIT_MASK_32(2)/*<The animation data path mode information mask*/
#define WGL_ANIM_DATA_FLAG_PATH_ID_INF            MAKE_BIT_MASK_32(3)/*<The animation data id information mask*/

/*Animate type*/
#define WGL_ANIM_TYPE_NONE              ((UINT16) 0x0000)    /**<Move type animation*/
#define WGL_ANIM_TYPE_MOVE              ((UINT16) 0x0001)    /**<Move type animation*/
#define WGL_ANIM_TYPE_SIZE              ((UINT16) 0x0002)    /**<Size type animation */
#define WGL_ANIM_TYPE_ALPHA             ((UINT16) 0x0004)    /**<Alpha type animation*/
#define WGL_ANIM_TYPE_IMG               ((UINT16) 0x0008)    /**<Image type animation*/
#define WGL_ANIM_TYPE_SCALE             ((UINT16) 0x0010)    /**<Image type animation*/
#define WGL_ANIM_TYPE_BREATHE           ((UINT16) 0x0020)    /**<Image type animation*/

#define WGL_ANIM_TYPE_ALL               ((UINT16) 0xFFFF)
/*Animate Private State*/
#define ANIM_STATE_PLAYING              ((UINT16) 0x0020)    /**<Animate Private State*/
#define IN    /**<IN  */
#define OUT    /**< OUT */


#define WGL_ANIM_INFINITE_VALUE          ((UINT32) 0xFFFFFFFF)

typedef UINT32  ANIM_IMG_CUST_INFO_T;    /**< Contain custom mode infomation of image animation*/


#define _ANIM_TOTAL_END( _pt_cond )                                           \
            ( ((_pt_cond)->ui2_end_type == WGL_ANIM_TYPE_ALL) &&        \
              ((_pt_cond)->e_cond[WGL_ANIM_TOTAL] == WGL_ANIM_TOTAL_STEP_END))


/*------------------------------------------------------------------*/
/*! @enum WGL_ANIM_COND_T
 *  @brief This enum defines the animation condition types
 *  @code
 *  typedef enum _WGL_ANIM_COND_T
 *  {
 *      WGL_ANIM_TOTAL_STEP_END   = 0,
 *      WGL_ANIM_ONE_LOOP_END = 1,
 *      WGL_ANIM_ERROR = 2
 *  } WGL_ANIM_COND_T;
 *  @endcode
 *  @li@c  WGL_ANIM_TOTAL_STEP_END                           -Animation end by total step mode
 *  @li@c  WGL_ANIM_ONE_LOOP_END                             -Animation end by one loop mode
 *  @li@c  WGL_ANIM_ERROR                                    -Aniamtion end by error
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_ANIM_COND_T
{
    WGL_ANIM_RUNNING = 0,
    WGL_ANIM_TOTAL_STEP_END ,
    WGL_ANIM_ONE_LOOP_END ,
    WGL_ANIM_TYPE_END ,
    WGL_ANIM_ERROR 
} WGL_ANIM_COND_T;

/*------------------------------------------------------------------*/
/*! @enum WGL_ANIM_STATE_T
 *  @brief This enum defines the animation condition types
 *  @code
 *  typedef enum _WGL_ANIM_STATE_T
 *  {
 *      WGL_ANIM_STATE_NONE   = 0,
 *      WGL_ANIM_STATE_PLAYING ,
 *      WGL_ANIM_STATE_COMPLETE_END,
 *      WGL_ANIM_STATE_END
 *  } WGL_ANIM_STATE_T;
 *  @endcode
 *  @li@c  WGL_ANIM_STATE_NONE                           -Animation end by total step mode
 *  @li@c  WGL_ANIM_STATE_PLAYING                       -Indicate the animation is playing
 *  @li@c  WGL_ANIM_STATE_COMPLETE_END            -Indicate the animation has been completed automatically. In this state WGL_ANIM_COND_INF_T structure with ui2_end_type=WGL_ANIM_TYPE_ALL will be notified to user.
 *  @li@c  WGL_ANIM_STATE_END                             -Indicate the animation is ended by called c_wgl_end_animation().
                                                                                For scale animation and the animation which will affect its childern, the animaiton effect will not end until c_wgl_end_animation() is called.
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_ANIM_STATE_T
{
    WGL_ANIM_STATE_NONE = 0,
    WGL_ANIM_STATE_PLAYING ,
    WGL_ANIM_STATE_COMPLETE_END,
    WGL_ANIM_STATE_END,
} WGL_ANIM_STATE_T;

/*Image Set Play Mode*/
/*------------------------------------------------------------------*/
/*! @enum WGL_ANIM_PLAY_MODE_T
 *  @brief This enum defines the animation playing mode
 *  @code
 *  typedef enum _WGL_ANIM_PLAY_MODE_T
 *  {
 *      ANIM_PLAY_MODE_ONCE   = 0,
 *      ANIM_PLAY_MODE_LOOP   = 1,
 *      ANIM_PLAY_MODE_CUST   = 2
 *  } WGL_ANIM_PLAY_MODE_T;
 *  @endcode
 *  @li@c  ANIM_PLAY_MODE_ONCE                               -Once play mode,oly do once time 
 *  @li@c  ANIM_PLAY_MODE_LOOP                               -Loop mode,do animation loop,the loop 
 numbers is setting at begin animation.
 *  @li@c  ANIM_PLAY_MODE_CUST                               -Customer mode,the play mode is setted 
 by setting information at begin.
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_ANIM_PLAY_MODE_T
{
    ANIM_PLAY_MODE_ONCE   = 0,
    ANIM_PLAY_MODE_LOOP   = 1,
    ANIM_PLAY_MODE_CUST   = 2
} WGL_ANIM_PLAY_MODE_T;

/*------------------------------------------------------------------*/
/*! @enum WGL_ANIM_INF_TYPE
 *  @brief  This enum indificates the animation information type.
 *  @code
 * typedef enum _WGL_ANIM_INF_TYPE
 * {
 *   WGL_ANIM_MOVE   = 1,
 *   WGL_ANIM_SIZE,
 *   WGL_ANIM_ALPHA,
 *   WGL_ANIM_IMG,
 *   WGL_ANIM_SCALE,
 *   WGL_ANIM_BRTH,
 *   WGL_ANIM_TOTAL,
 *   WGL_ANIM_MAX_NUM
 *  } WGL_ANIM_INF_TYPE;
 *  @endcode
 *  @li@c  WGL_ANIM_MOVE                               -It indificates the animation move type information 
 *  @li@c  WGL_ANIM_SIZE                               -It indificates the animation size type information 
 *  @li@c  WGL_ANIM_ALPHA                               -It indificates the animation alpha type information 
 *  @li@c  WGL_ANIM_IMG                               -It indificates the animation image type information 
 *  @li@c  WGL_ANIM_SCALE                               -It indificates the animation scale type information 
 *  @li@c  WGL_ANIM_BRTH                               -It indificates the animation breathe type information 
 *  @li@c  WGL_ANIM_TOTAL                               -It indificates the animation all types information 
 *  @li@c  WGL_ANIM_MAX_NUM                               -It indificates the type information max numbler 
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_ANIM_INF_TYPE
{
    WGL_ANIM_MOVE   = 1,
    WGL_ANIM_SIZE,
    WGL_ANIM_ALPHA,
    WGL_ANIM_IMG,
    WGL_ANIM_SCALE,
    WGL_ANIM_BRTH,
    WGL_ANIM_TOTAL,
    WGL_ANIM_MAX_NUM
} WGL_ANIM_INF_TYPE;

/*------------------------------------------------------------------*/
/*! @enum WGL_ANIM_STD_REF_TYPE
 *  @brief  This enum indificates scale animation based point .
 *  @code
* typedef enum _WGL_ANIM_STD_REF_TYPE
* {
*    ANIM_STD_REF_LEFT_TOP   = 0,
*    ANIM_STD_REF_CENTER_CENTER   
* } WGL_ANIM_STD_REF_TYPE;
 *  @endcode
 *  @li@c  ANIM_STD_REF_LEFT_TOP                  -It indificates the animation scales base on left-top point and scales toward right-bottom directions. 
 *  @li@c  ANIM_STD_REF_CENTER_CENTER       -It indificates the animation scales base on widget's center-centr point and scales toward left,right,top,bottom directions. 
 */
/*------------------------------------------------------------------*/

typedef enum _WGL_ANIM_STD_REF_TYPE
{
    ANIM_STD_REF_LEFT_TOP   = 0,
    ANIM_STD_REF_CENTER_CENTER   
} WGL_ANIM_STD_REF_TYPE;

/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @enum WGL_ANIM_COND_INF_T
 *  @brief This struction defines the animation playing mode
 *  @code
 *  typedef struct _WGL_ANIM_COND_INF_T
 *  {
 *       UINT16                          ui2_end_type;
 *       WGL_ANIM_COND_T                 e_cond[WGL_ANIM_MAX_NUM];
 *   }WGL_ANIM_COND_INF_T;
 *  @endcode
 *  @li@c  ui2_end_type                               -The aniamtion end type information 
 *  @li@c  e_cond[WGL_ANIM_MAX_NUM]      -The associated animation end type condition information.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_ANIM_COND_INF_T
{
    UINT16                          ui2_end_type;
    WGL_ANIM_COND_T                 e_cond[WGL_ANIM_MAX_NUM];
}WGL_ANIM_COND_INF_T;

/*------------------------------------------------------------------*/
/*! @enum WGL_ANIM_INF_T
 *  @brief This struction defines the animation playing mode
 *  @code
 *  typedef struct _WGL_ANIM_INF_T
 *  {
 *       WGL_ANIM_STATE_T                e_state;
 *       UINT16                          ui2_done_type;
 *   }WGL_ANIM_INF_T;
 *  @endcode
 *  @li@c  t_state-
 *  @li@c   ui2_done_type     -
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_ANIM_INF_T
{
    WGL_ANIM_STATE_T                e_state;
    UINT16                          ui2_done_type;
}WGL_ANIM_INF_T;
/*------------------------------------------------------------------*/
/*! @enum WGL_ANIM_IMG_INF_T
 *  @brief This structiondefines the animation image information.
 *  @code
 *  typedef struct _WGL_ANIM_IMG_INF_T
 *  {
 *     UINT16                          ui2_img_id;
 *     WGL_HIMG_TPL_T*                 pt_img;
 *   }WGL_ANIM_IMG_INF_T;
 *  @endcode
 *  @li@c  ui2_img_id                               -The image animation id information. 
 *  @li@c  pt_img                   -Point to image handle information that current image animation.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_ANIM_IMG_INF_T
{
    UINT16                          ui2_img_id;
    WGL_HIMG_TPL_T*                 pt_img;
}WGL_ANIM_IMG_INF_T;

/*------------------------------------------------------------------*/
/*! @enum WGL_ANIM_IMG_INF_T
 *  @brief This structiondefines the animation image information.
 *  @code
 *  typedef struct _WGL_ANIM_TICK_T
 *  {
 *      GL_POINT_T                      t_point;
 *      GL_SIZE_T                       t_size;
 *      GL_SIZE_T                       t_scale;
 *      UINT8                           ui1_alpha;
 *      WGL_ANIM_IMG_INF_T              t_img;
 *      WGL_ANIM_STD_REF_TYPE           e_ref_type; 
 *   }WGL_ANIM_TICK_T;
 *  @endcode
 *  @li@c  t_point                               -The point position of animation tick information
 *  @li@c  t_size                   -The size information of animation tick.
 *  @li@c  t_scale                   -The scale information of animation tick.
 *  @li@c  ui1_alpha                   -The alpha information of animation tick.
 *  @li@c  t_img                   -The image information of animation tick.
 *  @li@c  e_ref_type           -The scale direction type.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_ANIM_TICK_T
{
    GL_POINT_T                      t_point;
    GL_SIZE_T                       t_size;
    GL_SIZE_T                       t_scale;
    UINT8                           ui1_alpha;
    WGL_ANIM_IMG_INF_T              t_img;
    WGL_ANIM_STD_REF_TYPE           e_ref_type;    
}WGL_ANIM_TICK_T;

/*------------------------------------------------------------------*/
/*! @brief INT32 (*wgl_app_anim_callback_fct)(
                      HANDLE_T                h_widget,
                      UINT16                  ui2_anim_type,
                      WGL_ANIM_COND_T         e_anim_cond,
                      VOID*                   pv_tag)
 *  @param [in]  h_widget: Widget handle
 *  @param [in]  ui2_anim_type: Animation type
 *  @param [in]  e_anim_cond: Animation end condition type
 *  @param [in]  pv_tag: Other information
 *  @param [out]
 *  @note
 *  @see See the WGL_ANIM_CB_T struct.
 *  @return    -WGLR_OK:  The callback routin is successful.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef INT32 (*wgl_app_anim_callback_fct)(
                      HANDLE_T                h_widget,
                      UINT16                  ui2_anim_type,
                      WGL_ANIM_COND_INF_T     e_anim_cond,
                      VOID*                   pv_tag);

/*------------------------------------------------------------------*/
/*! @brief INT32 (*wgl_app_anim_type_cb_fct)(
                        UINT16                      ui2_cur_step,
                        VOID*                       pt_init, 
                        OUT WGL_ANIM_TICK_T*        pt_point)
 *  @param [in]  ui2_cur_step: The current step
 *  @param [in]  pt_init: Animation initial data information
 *  @param [in]  pt_point: Animation tick information
 *  @param [out]
 *  @note
 *  @see See the WGL_ANIM_TICK_T struct.
 *  @return    -WGLR_OK:  The callback routin is successful.
 *  @retval      -
 */
/*------------------------------------------------------------------*/

typedef INT32 (*wgl_app_anim_type_cb_fct)(
                        UINT16                      ui2_cur_step,
                        VOID*                       pt_init, /*WGL_ANIM_BEGIN_DATA_T*/
                        OUT WGL_ANIM_TICK_T*        pt_tick);


/*------------------------------------------------------------------*/
/*! @struct WGL_ANIM_IMG_SEQ_T
 *  @brief This struct defines the image type animation sequence information
 *  @code
 *  typedef struct _WGL_ANIM_IMG_SEQ_T
 *  {
 *      UINT16                          ui2_len;    
 *      WGL_HIMG_TPL_T*                 pt_img_tpl;
 *  }WGL_ANIM_IMG_SEQ_T;
 *  @endcode
 *  @li@c  ui2_len                                           -The num of images informatiom
 *  @li@c  pt_img_tpl                                        -Point to images information
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_ANIM_IMG_SEQ_T
{
    UINT16                          ui2_len;    
    WGL_HIMG_TPL_T*                 pt_img_tpl;
}WGL_ANIM_IMG_SEQ_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_ANIM_PATH_ID_INFO_T
 *  @brief This struct defines path id information for begin animation.
 *  @code
 *  typedef struct _WGL_ANIM_PATH_ID_INFO_T
 *  {
 *     UINT16                ui2_path_cnt;
 *     UINT32                aui4_path_id[WGL_ANIM_MAX_PATH_CNT_ONCE];
 *     UINT32                aui4_srt_tick[WGL_ANIM_MAX_PATH_CNT_ONCE];
 *     UINT16                aui2_start_percent[WGL_ANIM_MAX_PATH_CNT_ONCE];       
 *     UINT16                aui2_end_percent[WGL_ANIM_MAX_PATH_CNT_ONCE];    
 *   } WGL_ANIM_PATH_ID_INFO_T;
 *  @endcode
 *  @li@c  ui2_path_cnt                                           -The path id count,less than WGL_ANIM_MAX_PATH_CNT_ONCE.
 *  @li@c  aui4_path_id[]                                        -Strore the path id information.
 *  @li@c  aui4_srt_tick[]                                        -Strore start tick information that associated with the path id.
 *  @li@c  aui2_start_percent[]                                -Strore start percent of the whole animation that associated with the path id.
 *  @li@c  aui2_end_percent[]                                -Strore start percent of the whole animation that associated with the path id.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_ANIM_PATH_ID_INFO_T
{
   UINT16                ui2_path_cnt;
   UINT32                aui4_path_id[WGL_ANIM_MAX_PATH_CNT_ONCE];
   UINT32                aui4_srt_tick[WGL_ANIM_MAX_PATH_CNT_ONCE];
   UINT16                aui2_start_percent[WGL_ANIM_MAX_PATH_CNT_ONCE];       
   UINT16                aui2_end_percent[WGL_ANIM_MAX_PATH_CNT_ONCE];    
} WGL_ANIM_PATH_ID_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_MOVE_ANIM_DATA_T
 *  @brief This struct defines move type animation data information.
 *  @code
 *  typedef struct _WGL_MOVE_ANIM_DATA_T
 *  {
 *      INT16                           i2_start_x;         
 *      INT16                           i2_start_y;         
 *      INT16                           i2_end_x;           
 *      INT16                           i2_end_y;              
 *   } WGL_MOVE_ANIM_DATA_T;
 *  @endcode
 *  @li@c  i2_start_x                       -The start of x coordination position.
 *  @li@c  i2_start_y                      -The start of y coordination position.
 *  @li@c  i2_end_x                      -The end of x coordination position.
 *  @li@c  i2_end_y                      -The end of y coordination position.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_MOVE_ANIM_DATA_T
{
    INT16                           i2_start_x;         
    INT16                           i2_start_y;         
    INT16                           i2_end_x;           
    INT16                           i2_end_y;              
} WGL_MOVE_ANIM_DATA_T;    

/*------------------------------------------------------------------*/
/*! @struct WGL_ALPHA_ANIM_DATA_T
 *  @brief This struct defines alpha type animation data information.
 *  @code
 *  typedef struct _WGL_ALPHA_ANIM_DATA_T
 *  {
 *      UINT8                           ui1_start_alpha;     
 *      UINT8                           ui1_end_alpha;       
 *   } WGL_ALPHA_ANIM_DATA_T; 
 *  @endcode
 *  @li@c  ui1_start_alpha                       -The start of alpha value.
 *  @li@c  ui1_end_alpha                      -The end of alpha value.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_ALPHA_ANIM_DATA_T
{
    UINT8                           ui1_start_alpha;     
    UINT8                           ui1_end_alpha;       
} WGL_ALPHA_ANIM_DATA_T;   

/*------------------------------------------------------------------*/
/*! @struct WGL_SIZE_ANIM_DATA_T
 *  @brief This struct defines size type animation data information.
 *  @code
 *  typedef struct _WGL_SIZE_ANIM_DATA_T
 *  {
 *      UINT32                           ui8_start_w;
 *      UINT32                           ui8_end_w;
 *      UINT32                           ui8_start_h;
 *      UINT32                           ui8_end_h;    
 *   } WGL_SIZE_ANIM_DATA_T;
 *  @endcode
 *  @li@c  ui8_start_w                       -The start of width value.
 *  @li@c  ui8_end_w                      -The end of width value.
 *  @li@c  ui8_start_h                      -The start of height value.
 *  @li@c  ui8_end_h                      -The end of height value.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_SIZE_ANIM_DATA_T
{
    UINT32                           ui8_start_w;
    UINT32                           ui8_end_w;
    UINT32                           ui8_start_h;
    UINT32                           ui8_end_h;    
} WGL_SIZE_ANIM_DATA_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_SCALE_ANIM_DATA_T
 *  @brief This struct defines scale type animation data information.
 *  @code
 *  typedef struct _WGL_SCALE_ANIM_DATA_T
 *  {
 *      WGL_ANIM_STD_REF_TYPE            e_ref_type;
 *      UINT32                           ui8_start_w;
 *      UINT32                           ui8_end_w;
 *      UINT32                           ui8_start_h;
 *      UINT32                           ui8_end_h;    
 *   } WGL_SCALE_ANIM_DATA_T;
 *  @endcode
 *  @li@c  e_ref_type                       -The scale direction type.
 *  @li@c  ui8_start_w                       -The start of width value.
 *  @li@c  ui8_end_w                      -The end of width value.
 *  @li@c  ui8_start_h                      -The start of height value.
 *  @li@c  ui8_end_h                      -The end of height value.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_SCALE_ANIM_DATA_T
{
    WGL_ANIM_STD_REF_TYPE            e_ref_type;
    UINT32                           ui8_start_w;
    UINT32                           ui8_end_w;
    UINT32                           ui8_start_h;
    UINT32                           ui8_end_h;    
} WGL_SCALE_ANIM_DATA_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_IMG_ANIM_DATA_T
 *  @brief This struct defines image type animation data information.
 *  @code
 *  typedef struct _WGL_IMG_ANIM_DATA_T
 *  {
 *      UINT8                           ui1_img_seq_id;
 *      UINT16                          ui2_cust_len;
 *      ANIM_IMG_CUST_INFO_T*           pt_cust_info;
 *   } WGL_IMG_ANIM_DATA_T;
 *  @endcode
 *  @li@c  ui1_img_seq_id                 -The image sequence id.
 *  @li@c  ui2_cust_len                      -The legth for custer mode animation.
 *  @li@c  pt_cust_info                      -The custer mode information,detail see the ANIM_IMG_CUST_INFO_T.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_IMG_ANIM_DATA_T
{
    UINT8                           ui1_img_seq_id;
    UINT16                          ui2_cust_len;
    ANIM_IMG_CUST_INFO_T*           pt_cust_info;
} WGL_IMG_ANIM_DATA_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_BREATHE_ANIM_DATA_T
 *  @brief This struct defines breathe type animation data information.
 *  @code
 *  typedef struct _WGL_BREATHE_ANIM_DATA_T
 *  {
 *      WGL_INSET_T                     t_breathe_inset;
 *      UINT8                           ui1_start_alpha;
 *      UINT8                           ui1_mid_alpha;
 *      UINT8                           ui1_end_alpha;
 *    } WGL_BREATHE_ANIM_DATA_T;
 *  @endcode
 *  @li@c  t_breathe_inset                       -The breath inset information.
 *  @li@c  ui1_start_alpha                      -The start alpha information.
 *  @li@c  ui1_mid_alpha                      -
 *  @li@c  ui1_end_alpha                      -
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_BREATHE_ANIM_DATA_T
{
    WGL_INSET_T                     t_breathe_inset;
    UINT8                           ui1_start_alpha;
    UINT8                           ui1_mid_alpha;
    UINT8                           ui1_end_alpha;
} WGL_BREATHE_ANIM_DATA_T; 


/*------------------------------------------------------------------*/
/*! @struct WGL_ANIM_DATA_INFO
 *  @brief This struct defines this animation data information,it is general data information for animation
 *  @code
 *  typedef struct _WGL_ANIM_DATA_INFO 
 *  {     
 *      UINT16                             ui2_anim_type;     
 *      union{
 *            WGL_MOVE_ANIM_DATA_T           t_move; 
 *            WGL_SIZE_ANIM_DATA_T           t_size;
 *            WGL_SCALE_ANIM_DATA_T          t_scale;    
 *            WGL_ALPHA_ANIM_DATA_T          t_alpha;   
 *            WGL_IMG_ANIM_DATA_T            t_img;    
 *            WGL_BREATHE_ANIM_DATA_T        t_breathe; 
 *        }u_data;
 *       wgl_app_anim_type_cb_fct            pf_type_cb;  
 *    } WGL_ANIM_DATA_INFO;
 *  @endcode
 *  @li@c  ui2_anim_type             -The animation type .
 *  @li@c  t_move                       -The move animation data information.
 *  @li@c  t_size                         -The size animation data information.
 *  @li@c  t_scale                        -The scale animation data information.
 *  @li@c  t_alpha                       -The alpha animation data information.
 *  @li@c  t_img                         -The image animation data information.
 *  @li@c  t_breathe                    -The breathe animation data information.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_ANIM_DATA_INFO
{    
    UINT16                             ui2_anim_type;     
    union{
        WGL_MOVE_ANIM_DATA_T           t_move; 
        WGL_SIZE_ANIM_DATA_T           t_size;
        WGL_SCALE_ANIM_DATA_T          t_scale;    
        WGL_ALPHA_ANIM_DATA_T          t_alpha;   
        WGL_IMG_ANIM_DATA_T            t_img;    
        WGL_BREATHE_ANIM_DATA_T        t_breathe; 
    }u_data;

    wgl_app_anim_type_cb_fct            pf_type_cb;  
} WGL_ANIM_DATA_INFO;

/*------------------------------------------------------------------*/
/*! @struct WGL_BREATHE_ANIM_DATA_T
 *  @brief This struct defines breathe type animation data information.
 *  @code
 *  typedef struct WGL_ANIM_PATH_DATA_T
 *  {
 *      UINT16                              ui2_anim_type; 
 *      UINT16                              ui2_total_steps;          
 *      UINT16                              ui2_frame_interval;      
 *      UINT32                              ui4_start_tick;
 *      WGL_ANIM_PLAY_MODE_T                e_play_mode; 
 *      UINT16                              ui2_loop_count;
 *      wgl_app_anim_type_cb_fct            pf_type_cb;  
 *    }WGL_ANIM_PATH_DATA_T;
 *  @endcode
 *  @li@c  ui2_anim_type                       -The animation type.
 *  @li@c  ui2_total_steps                      -The total step of this animation type.
 *  @li@c  ui2_frame_interval                      -The frame interval of this animation type.
 *  @li@c  ui4_start_tick                      -The start tick of this animation type.
 *  @li@c  e_play_mode                      -The play mode of this animation image type.
 *  @li@c  ui2_loop_count                      -The loop count of this animation image type.
 *  @li@c  pf_type_cb                      -The animation type callback for app define .
 */
/*------------------------------------------------------------------*/
typedef struct WGL_ANIM_PATH_DATA_T
{
    UINT16                              ui2_anim_type; 
    UINT16                              ui2_total_steps;          
    UINT16                              ui2_frame_interval;      
    UINT32                              ui4_start_tick;
    WGL_ANIM_PLAY_MODE_T                e_play_mode; 
    UINT16                              ui2_loop_count;
    wgl_app_anim_type_cb_fct            pf_type_cb;  
}WGL_ANIM_PATH_DATA_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_ANIMATION_PATH_MODE_T
 *  @brief This struct defines animation path mode information.
 *  @code
 *  typedef struct WGL_ANIMATION_PATH_MODE_T
 *  {
 *      UINT32                          ui4_path_id;
 *      UINT16                          ui2_count;
 *      WGL_ANIM_PATH_DATA_T*           pt_path_inf;
 *   }WGL_ANIMATION_PATH_MODE_T;
 *  @endcode
 *  @li@c  ui4_path_id                    -The path id information.
 *  @li@c  ui2_count                      -The path data count.
 *  @li@c  pt_path_inf                    -Contains the path data information.
 */
/*------------------------------------------------------------------*/
typedef struct WGL_ANIMATION_PATH_MODE_T
{
    UINT32                          ui4_path_id;
    UINT16                          ui2_count;
    WGL_ANIM_PATH_DATA_T*           pt_path_inf;
}WGL_ANIMATION_PATH_MODE_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_ANIMATION_DATA_T
 *  @brief This struct defines animation data information.
 *  @code
 *  typedef struct _WGL_ANIMATION_DATA_T
 *  {
 *     UINT32                          ui4_flag;
 *     UINT16                          ui2_count;
 *     WGL_ANIM_DATA_INFO*             pt_anim_data;  
 *     WGL_ANIM_PATH_DATA_T*           pt_path_mode;    
 *     WGL_ANIM_PATH_ID_INFO_T         t_path_id_inf; 
 *  }WGL_ANIMATION_DATA_T;
 *  @endcode
 *  @li@c  ui4_flag                    -This is animation data information mask.The flag can be setted:
 *      (1)WGL_ANIM_DATA_FLAG_IGORE,meaning this animation data has no information,can ingore.
 *      (2)WGL_ANIM_DATA_FLAG_DATA_INF,meaning it use pt_anim_data for animation data.The pointted size is 
 *         sizeof(WGL_ANIM_DATA_INFO)*ui2_count.
 *      (3)WGL_ANIM_DATA_FLAG_PATH_MODE_INF,meaning it use pt_path_mode for animation data.The pointted 
             size is  sizeof(WGL_ANIM_PATH_DATA_T)*ui2_count.
 *      (4)WGL_ANIM_DATA_FLAG_PATH_ID_INF,meaning it use t_path_id_inf for animation data.
 *          This is using by path mode format.It must set information into path library first. The realy path information 
 *          count in the t_path_id_inf must been same with ui2_count.Because every type data(pt_anim_data) has 
 *          one associated path information.
 *  @li@c  ui2_count                      -The path data count.
 *  @li@c  pt_anim_data                    -Contains the path data information,if the ui4_flag is set WGL_ANIM_DATA_FLAG_DATA_INF
 *  @li@c  pt_path_mode                    -Contains the path mode information.if the ui4_flag is set WGL_ANIM_DATA_FLAG_PATH_MODE_INF
 *  @li@c  t_path_id_inf                    -Contains the path id information.If the ui4_flag is set WGL_ANIM_DATA_FLAG_PATH_ID_INF.
 *
 *   Note: 
 *    1. The animation data can divided into two part data information:data information(pt_anim_data) and 
 *        path information(pt_path_mode or t_path_id_inf). The data information indicated animation type must be 
 *        same with path information indicated animation type.
 *    2.The pt_path_mode and t_path_id_inf are two different format to indicates the animation path information,
 *       so ui4_flag should not been setted both of WGL_ANIM_DATA_FLAG_PATH_MODE_INF and WGL_ANIM_DATA_FLAG_PATH_ID_INF .
 *    3. If flag setted WGL_ANIM_DATA_FLAG_DATA_INF and WGL_ANIM_DATA_FLAG_PATH_MODE_INF,the count 
 *        information of pt_anim_data and pt_path_mode must be same with ui2_count. 
 *    4. When the two type is sequence,the next one must be start from (the before end tick + 1) 
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_ANIMATION_DATA_T
{
    UINT32                          ui4_flag;
    UINT16                          ui2_count;

    WGL_ANIM_DATA_INFO*             pt_anim_data;  
    WGL_ANIM_PATH_DATA_T*           pt_path_mode;    
    WGL_ANIM_PATH_ID_INFO_T         t_path_id_inf; 
}WGL_ANIMATION_DATA_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_ANIM_BEGIN_DATA_T
 *  @brief This struct defines the begin data for animation.
 *  @code
 *  typedef struct _WGL_ANIM_BEGIN_DATA_T
 *  {
 *      WGL_ANIM_DATA_INFO              t_anim_data;  
 *      WGL_ANIM_PATH_DATA_T            t_path_mode;    
 *   }WGL_ANIM_BEGIN_DATA_T;
 *  @endcode
 *  @li@c  t_anim_data                       -The animation data infromation.
 *  @li@c  t_path_mode                      -The path mode data information.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_ANIM_BEGIN_DATA_T
{
    WGL_ANIM_DATA_INFO              t_anim_data;  
    WGL_ANIM_PATH_DATA_T            t_path_mode;    
}WGL_ANIM_BEGIN_DATA_T;


/*------------------------------------------------------------------*/
/*! @struct WGL_ANIM_ACTION_MAP_T
 *  @brief This struct defines animation action map information.
 *  @code
 *  typedef struct _WGL_ANIM_ACTION_MAP_T
 *  {
 *      WGL_ANIM_ACTION_TYPE_T                e_anim_action_type;
 *      UINT32                                            ui4_flag;
 *      WGL_ANIM_CONTEXT_T                    t_anim_context;
 *     WGL_ANIMATION_DATA_T                  t_anim_data; 
 *   }WGL_ANIM_ACTION_MAP_T; 
 *  @endcode
 *  @li@c  e_anim_action_type                       -The animation type.
 *  @li@c  ui4_flag                                 -The flag is for indicating this action,the private 
 *                                                          definition can see the private widget type definintion.
 *  @li@c  t_anim_context                      -The animation context type.
 *  @li@c  t_anim_data                      -The animation data information.The ui4_flag in the 
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_ANIM_ACTION_MAP_T
{
    WGL_ANIM_ACTION_TYPE_T                e_anim_action_type;
    UINT32                                ui4_flag;
    WGL_ANIM_CONTEXT_T                    t_anim_context;
    WGL_ANIMATION_DATA_T                  t_anim_data; 
}WGL_ANIM_ACTION_MAP_T; 

/*------------------------------------------------------------------*/
/*! @struct WGL_ANIM_ACTION_MAP_T
 *  @brief This struct defines animation tick information .
 *  @code
 *  typedef struct _WGL_ANIM_TICK_INFO
 *  {
 *      UINT16                          ui2_inf_rsn;
 *      UINT32                          aui4_tick[WGL_ANIM_MAX_NUM];
 *      WGL_ANIM_TICK_T                 t_tick_inf;
 *    }WGL_ANIM_TICK_INFO;
 *  @endcode
 *  @li@c  ui2_inf_rsn                       -The animation reason information.
 *  @li@c  aui4_tick[]                      -The animation reason associated tick that contain the animation type tick.
 *  @li@c  t_tick_inf                      -The animation tick data information.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_ANIM_TICK_INFO
{
    UINT16                          ui2_inf_rsn;
    UINT32                          aui4_tick[WGL_ANIM_MAX_NUM];
    WGL_ANIM_TICK_T                 t_tick_inf;
}WGL_ANIM_TICK_INFO; 

/*------------------------------------------------------------------*/
/*! @struct WGL_ANIM_ACTION_MAP_T
 *  @brief This struct defines animation tick information .
 *  @code
 *  typedef struct _WGL_ANIM_TICK_DATA
 *  {
 *      UINT16                          ui2_state;
 *      UINT16                          ui2_begin_flag;
 *      UINT32                          ui4_tick_seq;
 *      GL_RECT_T                       t_cur_rect;
 *      WGL_ANIM_TICK_INFO              t_old_tick;
*      WGL_ANIM_TICK_INFO              t_new_tick;
 * } WGL_ANIM_TICK_DATA;
 *  @endcode
 *  @li@c  ui2_state                       -The animation state.
 *  @li@c  ui2_begin_flag               -The animation begin type information
 *  @li@c  ui4_tick_seq                  -The animation tick  seq.
 *  @li@c  t_cur_rect                  -The animation current region.
 *  @li@c  t_old_tick                  -Contain the old tick information.
 *  @li@c  t_new_tick                  -Contain the new tick information.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_ANIM_TICK_DATA
{
    UINT16                          ui2_state;
    UINT16                          ui2_begin_flag;
    UINT32                          ui4_tick_seq;
    UINT16                          ui2_cur_rsn;
    GL_RECT_T                       t_cur_rect;
    
    WGL_ANIM_TICK_INFO              t_old_tick;
    WGL_ANIM_TICK_INFO              t_new_tick;
} WGL_ANIM_TICK_DATA;    


  /**
    This command is used to get the min size (GL_SIZE_T) of image for the special Animation Widget.
    
    Output arguments:
    
    pv_param1:   Pointer to a GL_SIZE_T value of image.
    
    pv_param2:   Reserved. It must be NULL.
    
    Returns:
    
    WGLR_OK: The function call is successful.
    
    WGLR_FUNC_NOT_IMPL: This function will not implement.
    
    WGLR_INV_HANDLE: To get image handle is not valid.
    
    WGLR_INTERNAL_ERROR: The error occure when doing in the middleware wiget.
    
    WGLR_INV_ARG:  The information to get is not valid information.
    */
#define WGL_CMD_ANIM_GET_MIN_SIZE               WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_ANIM, 0)    
    
    /**
    This command is used to set the image sequence information of an Animation Widget.
    
    Output arguments
    
    pv_param1:   Pointer to a UINT8 value that is iamge sequences index.
    
    pv_param2:   Point to a WGL_ANIM_IMG_SEQ_T value that is to set image information.
    
    Returns
    
    WGLR_OK: The function call is successful.
    
    WGLR_INV_ARG:    pv_param1 value is not valid,pv_param2 information is null or not valid.
    
    WGLR_OUT_OF_MEMORY: The image sequences of animation widget is full.
    
    WGLR_INV_STATE: The animation state is not right.
    */
#define WGL_CMD_ANIM_SET_IMG_SEQ                WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_ANIM, 1)    
    
    /**
    This command is used to set the condition call back function of an Animation Widget.
    
    Input arguments
    
    pv_param1   Pointer to a WGL_ANIM_CB_T value to specify the condition call back function. It can be NULL.
    
    pv_param2   Reserved. It must be NULL.
    
    Returns
    
    WGLR_OK The function call is successful.
    
    WGLR_INV_ARG    pv_param1 is NULL.
    */
#define WGL_CMD_ANIM_SET_COND_CB                WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_ANIM, 2)    
    
    /**
    This command is used to get the condition call back function of an Animation Widget.
    
    Output arguments
    
    pv_param1   Pointer to a WGL_ANIM_CB_T value to specify the condition call back function. It can be NULL.
    
    pv_param2   Reserved. It must be NULL.
    
    Returns
    
    WGLR_OK The function call is successful.
    
    WGLR_INV_ARG    pv_param1 is NULL, or the sum of i4_left and i4_right isn't less than Animation 
    
    Widget's width, or the sum of i4_top and i4_bottom isn't less than Animation Widget's height.
    */
#define WGL_CMD_ANIM_GET_COND_CB                WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_ANIM, 3)    
    
    
    
    /*Specific Command  */
    /**
    This command is used to set Animation Widget's foreground alignment.
    
    Input arguments
    
    pv_param1   Specifies the alignment style. It can be from WGL_AS_LEFT_TOP to WGL_AS_RIGHT_BOTTOM. 
    About the explanation of these alignment styles.
    
    pv_param2   Reserved. It must be NULL.
    
    Returns
    
    WGLR_OK The function call is successful.
    
    WGLR_INV_ARG    The style specified by pv_param1 is invalid.
     */
#define WGL_CMD_ANIM_SET_ALIGN                  WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_ANIM, 0)    
    
    /**
    This command returns the foreground's alignment of an Animation Widget.
    
    Input arguments
    
    pv_param1   Pointer to a UINT8 to receive its alignment style. It cannot be NULL.
    
    pv_param2   Reserved. It must be NULL.
    
    Returns
    
    WGLR_OK The function call is successful.
    
    WGLR_INV_ARG    The argument pv_param1 is NULL.
    */
#define WGL_CMD_ANIM_GET_ALIGN                  WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_ANIM, 1)    
    
    /**
    This command is used to set the display margin of an Animation Widget's foreground. The i4_left 
    is for the left margin, the i4_right is for the right margin, the i4_top is for the top margin, 
    and the i4_bottom is for the bottom margin. All of them cannot be less than 0.
    
    Input arguments
    
    pv_param1   Pointer to a WGL_INSET_T value to specify the display margin. It cannot be NULL.
    
    pv_param2   Reserved. It must be NULL.
    
    Returns
    
    WGLR_OK The function call is successful.
    
    WGLR_INV_ARG    pv_param1 is NULL, or the sum of i4_left and i4_right isn't less than Animation 
    Widget's width, or the sum of i4_top and i4_bottom isn't less than Animation Widget's height.
    */
#define WGL_CMD_ANIM_SET_CNT_INSET              WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_ANIM, 2)    
    
    /**
    This command returns the display margin of an Icon Static's foreground. The i4_left is the left 
    margin, the i4_right is the right margin, the i4_top is the top margin, and the i4_bottom is the 
    bottom margin. 
    
    Input arguments
    
    pv_param1   Pointer to a WGL_INSET_T value to receive the display margin. It cannot be NULL.
    
    pv_param2   Reserved. It must be NULL.
    
    Returns
    
    WGLR_OK The function call is successful.
    
    WGLR_INV_ARG    The argument pv_param1 is NULL.
     */
#define WGL_CMD_ANIM_GET_CNT_INSET              WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_ANIM, 3)    
    
    /**
    This command set an image sequence data to the appointed index in image set of an animation widget
    . Animation images are set with this command.
    
    Input arguments
    pv_param1   Contains the index of the image set. The valid index is smaller 
    than ui2_max_img_set_len when create this widget. This argument must be of type UINT16.
    
    pv_param2   Pointer to a WGL_ANIM_IMG_SEQ_T structure.
    
    Returns
    
    WGLR_OK                      The function call is successful.
    
    WGLR_INV_ARG                    One of two arguments is invalid.
    
    WGLR_INV_STATE               Image sequence of the assigned index of the image set is now playing.
    */
#define WGL_CMD_ANIM_SET_CNT_PLAY_IMG           WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_ANIM, 4)    
    
    
    
     /**(WGL_CMD_ICON_GET_CNT_INSET - WGL_CMD_ICON_SET_ALIGN + 1)*/
#define ANIM_CNT_PRIVATE_START                  5      


/*-----------------------------------------------------------------------------
                    functions
 ----------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @brief To pack image custome information function
 *  @param [in]b_flag          -True:it should check custom mode id information. False,not.
 *  @param [in]ui2_img_id   -The index of animation image array.
 *  @param [in]ui2_interval  -The interval time value.
 *  @param [out]pt_cust_info       -Contains animation widget packed information.
 *  @note
 *  @see
 *  @return  WGLR_OK               -Packing animation widget information successfull.
 *               WGLR_INV_ARG      -The information to packed is not valid.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
INT32 wgl_anim_pack_img_cust_info(
    IN ANIM_IMG_CUST_INFO_T*       pt_cust_info,
    IN BOOL                        b_flag,
    IN UINT16                      ui2_img_id,
    IN UINT16                      ui2_interval
);
    

/*------------------------------------------------------------------*/
/*! @struct WGL_ANIM_INIT_T
 *  @brief This struct defines the initial information of animation
 *  @code
 *  typedef struct _WGL_ANIM_INIT_T
 *  {
 *      UINT32                      ui4_style;
 *      UINT16                      ui2_max_img_set_len;
 *  }  WGL_ANIM_INIT_T;
 *  @endcode
 *  @li@c  ui4_style                                         -Widget style
 *  @li@c  ui2_max_img_set_len                               -The max images at initial setting
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_ANIM_INIT_T
{
    UINT32                      ui4_style;
    UINT16                      ui2_max_img_set_len;
}  WGL_ANIM_INIT_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_ANIM_CB_T
 *  @brief This struct contains the animation callback information
 *  @code
 *  typedef struct _WGL_ANIM_CB_T
 *  {
 *      wgl_app_anim_callback_fct     pf_anim_callback;
 *      VOID*                         pv_tag;
 *  }WGL_ANIM_CB_T;
 *  @endcode
 *  @li@c  pf_anim_callback                                  -The animation callback function
 *  @li@c  pv_tag                                            -The animation callback tag
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_ANIM_CB_T
{
    wgl_app_anim_callback_fct     pf_anim_callback;
    VOID*                         pv_tag;
}WGL_ANIM_CB_T;


/*------------------------------------------------------------------*/
/*! @brief The widget animation path default callback function.
 *  @param [in]ui2_cur_step         -The current animation step.
 *  @param [in]pt_init                  -The animation initial data information with struction WGL_ANIM_BEGIN_DATA_T.
 *  @param [out]pt_tick       -The next step position point information.
 *  @note    -This is move animation callback default callback function,it is used for wgl_app_anim_type_cb_fct in 
 *                 the struct WGL_ANIM_DATA_INFO or WGL_ANIM_PATH_DATA_T. If both structions have 
 *                 wgl_app_anim_type_cb_fct,it will use the callback in WGL_ANIM_DATA_INFO.The priority of callback
 *                 in the WGL_ANIM_DATA_INFO is higher than that in WGL_ANIM_PATH_DATA_T.
 *  @see  WGL_ANIM_MOVE_PATH_JUMP
 *  @return  WGLR_OK               -The get next move animation start position successfull.
 *               WGLR_INV_ARG      -The parameter for the move animation information is not valid.
 *  @retval     
 */
/*------------------------------------------------------------------*/
INT32 wgl_anim_move_path_jump(
                        UINT16                      ui2_cur_step,
                        VOID*                       pt_init, 
                        OUT WGL_ANIM_TICK_T*        pt_tick);

/**This define the move animation default model for callback function.*/
#define WGL_ANIM_MOVE_PATH_JUMP         ((wgl_app_anim_type_cb_fct)wgl_anim_move_path_jump)


#ifdef __cplusplus
}

#endif

#endif
/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_WGL_ANIMATION*/
/*----------------------------------------------------------------------------*/


