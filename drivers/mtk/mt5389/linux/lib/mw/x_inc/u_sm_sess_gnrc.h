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
/*! @file u_sm_sess_gnrc.h 
 *  $RCSfile: $
 *  $Revision: #2 $
 *  $Date: 2012/07/24 $
 *  $Author: jiangbo.wang $
 *  
 *  @par Description:
 *         This header file contains Stream Manager's generic session exported 
 *         definitions.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_STRM_MNGR_SESS_GNRC_HDLR
 *  @{ 
 */
/*----------------------------------------------------------------------------*/
#ifndef _U_SM_SESS_GNRC_H_
#define _U_SM_SESS_GNRC_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "u_common.h"
#include "x_mm_common.h"
#include "u_sm.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

/* set/get types *************************************************************/
#define SM_GRP_SESS_GNRC              (( SM_GRP_LAST_ENTRY + 10 ) << SM_CODE_BITS)    /**<        */
#define SM_GRP_TUNL_PCR               (( SM_GRP_LAST_ENTRY + 11 ) << SM_CODE_BITS)    /**<        */

/* get types *****************************************************************/
#define SM_SESS_GNRC_GET_TYPE_PCR              ((SM_GET_TYPE_T)(SM_GRP_SESS_GNRC | 0))    /**<        */
#define SM_SESS_GNRC_GET_TYPE_PCR_ID           ((SM_GET_TYPE_T)(SM_GRP_SESS_GNRC | 1))    /**<        */
#define SM_SESS_GNRC_GET_TYPE_STC              ((SM_GET_TYPE_T)(SM_GRP_SESS_GNRC | 2))    /**<        */
#define SM_SESS_GNRC_GET_TYPE_MEDIA_FMT        ((SM_GET_TYPE_T)(SM_GRP_SESS_GNRC | 3))    /**<        */
#define SM_SESS_GNRC_GET_TYPE_BLOCK_PSI        ((SM_GET_TYPE_T)(SM_GRP_SESS_GNRC | 4))    /**<        */
#define SM_SESS_GNRC_GET_TYPE_CURR_LBA         ((SM_GET_TYPE_T)(SM_GRP_SESS_GNRC | 5))    /**<        */
#define SM_SESS_GNRC_GET_TYPE_CURR_TICK_NUM    ((SM_GET_TYPE_T)(SM_GRP_SESS_GNRC | 6))    /**<        */
#define SM_SESS_GNRC_GET_TYPE_POST_INFO        ((SM_GET_TYPE_T)(SM_GRP_SESS_GNRC | 7))    /**<        */
#define SM_SESS_GNRC_GET_TYPE_UOP_CAP          ((SM_GET_TYPE_T)(SM_GRP_SESS_GNRC | 8))    /**<        */
#define SM_SESS_GNRC_GET_TYPE_STC_SIGNED       ((SM_GET_TYPE_T)(SM_GRP_SESS_GNRC | 9))    /**<        */
#define SM_SESS_GNRC_GET_TYPE_STC_ID           ((SM_GET_TYPE_T)(SM_GRP_SESS_GNRC | 10))    /**<        */


/* set types *****************************************************************/
#define SM_SESS_GNRC_SET_TYPE_SCART_OUT        ((SM_SET_TYPE_T)(SM_GRP_SESS_GNRC | 0))    /**<        */
#define SM_SESS_GNRC_SET_TYPE_AV_SYNC          ((SM_SET_TYPE_T)(SM_GRP_SESS_GNRC | 1))    /**<        */
#define SM_SESS_GNRC_SET_TYPE_ADD_RANGE        ((SM_SET_TYPE_T)(SM_GRP_SESS_GNRC | 2))    /**<        */
#define SM_SESS_GNRC_SET_TYPE_DEL_RANGE        ((SM_SET_TYPE_T)(SM_GRP_SESS_GNRC | 3))    /**<        */
#define SM_SESS_GNRC_SET_TYPE_SPEED            ((SM_SET_TYPE_T)(SM_GRP_SESS_GNRC | 4))    /**<        */
#define SM_SESS_GNRC_SET_TYPE_STC              ((SM_SET_TYPE_T)(SM_GRP_SESS_GNRC | 5))    /**<        */
#define SM_SESS_GNRC_SET_TYPE_SEEK             ((SM_SET_TYPE_T)(SM_GRP_SESS_GNRC | 6))    /**<        */
#define SM_SESS_GNRC_SET_TYPE_BLOCK_PSI        ((SM_SET_TYPE_T)(SM_GRP_SESS_GNRC | 7))    /**<        */
#define SM_SESS_GNRC_SET_TYPE_REC_BUF_RANGE    ((SM_SET_TYPE_T)(SM_GRP_SESS_GNRC | 8))    /**<        */
#ifdef TIME_SHIFT_SUPPORT
#define SM_SESS_GNRC_SET_TYPE_MONITOR_LBA      ((SM_SET_TYPE_T)(SM_GRP_SESS_GNRC | 9))    /**<        */
#endif /* TIME_SHIFT_SUPPORT */
/*ab_repeat_driver*/
#define SM_SESS_GNRC_SET_TYPE_AB_POSITION      ((SM_SET_TYPE_T)(SM_GRP_SESS_GNRC | 10))
#define SM_SESS_GNRC_SET_TYPE_AV_SYNC_EX      ((SM_SET_TYPE_T)(SM_GRP_SESS_GNRC | 11))
#define SM_SESS_GNRC_SET_TYPE_STC_CTRL         ((SM_SET_TYPE_T)(SM_GRP_SESS_GNRC | 12))    /**<        */
#define SM_SESS_GNRC_SET_TYPE_MM_AUTOTEST     ((SM_SET_TYPE_T)(SM_GRP_SESS_GNRC | 13))
#define SM_SESS_GNRC_SET_TYPE_3D_PROPERTY     ((SM_SET_TYPE_T)(SM_GRP_SESS_GNRC | 14))
#define SM_SESS_GNRC_SET_TYPE_SEEK_WITH_PTS   ((SM_SET_TYPE_T)(SM_GRP_SESS_GNRC | 15))
#define SM_SESS_GNRC_SET_TYPE_SPEED_WITH_PTS  ((SM_SET_TYPE_T)(SM_GRP_SESS_GNRC | 16))    /**<        */
#define SM_SESS_GNRC_SET_TYPE_HDMVC_INFO      ((SM_SET_TYPE_T)(SM_GRP_SESS_GNRC | 17))

/*------------------------------------------------------------------*/
/*! @enum  SM_SESS_GNRC_MM_SYNC_T
 *  @brief  Multimedia sync mode.
 *  @code
 *  typedef enum
 *  {
 *      SM_SESS_GNRC_MM_SYNC_UNKNOWN  =   0,
 *      SM_SESS_GNRC_MM_SYNC_FREE_RUN ,     
 *      SM_SESS_GNRC_MM_SYNC_AUDIO_MASTER,
 *      SM_SESS_GNRC_MM_SYNC_VIDEO_MASTER,
 *      SM_SESS_GNRC_MM_SYNC_SYSTEM_MASTER        
 *  } SM_SESS_GNRC_MM_SYNC_T;
 *  @endcode
 *  @li@c  SM_SESS_GNRC_MM_SYNC_FREE_RUN                     - Sync mode in unsure mode .
 *  @li@c  SM_SESS_GNRC_MM_SYNC_FREE_RUN                     -Free run sync mode.
 *  @li@c  SM_SESS_GNRC_MM_SYNC_AUDIO_MASTER                 -Audio master sync mode.
 *  @li@c  SM_SESS_GNRC_MM_SYNC_VIDEO_MASTER                 -Video master sync mode.
 *  @li@c  SM_SESS_GNRC_MM_SYNC_SYSTEM_MASTER                -System master sync mode.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SM_SESS_GNRC_MM_SYNC_UNKNOWN       =   0,
    SM_SESS_GNRC_MM_SYNC_FREE_RUN,
    SM_SESS_GNRC_MM_SYNC_AUDIO_MASTER,
    SM_SESS_GNRC_MM_SYNC_VIDEO_MASTER,
    SM_SESS_GNRC_MM_SYNC_SYSTEM_MASTER        
} SM_SESS_GNRC_MM_SYNC_T;

/*------------------------------------------------------------------*/
/*! @enum  SM_SESS_GNRC_MM_SYNC_T
 *  @brief  Multimedia sync mode.
 *  @code
*typedef enum
*{
 *   SM_SESS_STC_AV_SYNC_MODE_FREE_RUN       =   0x0,
 *   SM_SESS_STC_AV_SYNC_MODE_AUDIO_MASTER,
 *   SM_SESS_STC_AV_SYNC_MODE_VIDEO_MASTER,
*    SM_SESS_STC_AV_SYNC_MODE_SYSTEM_MASTER    
*} SM_SESS_STC_AV_SYNC_MODE_T;
 *  @endcode
 *  @li@c  SM_SESS_STC_AV_SYNC_MODE_FREE_RUN                     - Free run sync mode .
 *  @li@c  SM_SESS_STC_AV_SYNC_MODE_AUDIO_MASTER                     -Audio master sync mode.
 *  @li@c  SM_SESS_STC_AV_SYNC_MODE_VIDEO_MASTER                 -Video master sync mode.
 *  @li@c  SM_SESS_STC_AV_SYNC_MODE_SYSTEM_MASTER                 -System sync mode.
  */
/*------------------------------------------------------------------*/
typedef enum
{
    SM_SESS_STC_AV_SYNC_MODE_FREE_RUN       =   0x0,
    SM_SESS_STC_AV_SYNC_MODE_AUDIO_MASTER,
    SM_SESS_STC_AV_SYNC_MODE_VIDEO_MASTER,
    SM_SESS_STC_AV_SYNC_MODE_SYSTEM_MASTER    
} SM_SESS_STC_AV_SYNC_MODE_T;

/*------------------------------------------------------------------*/
/*! @struct SM_SESS_GNRC_DATA_T
 *  @brief  Contains generic session info.
 *  @code
 *  typedef struct _SM_SESS_GNRC_DATA_T
 *  {
 *      BOOL                    b_disable_av_sync;
 *      MEDIA_FORMAT_T          t_media_fmt;
 *      SM_SESS_GNRC_MM_SYNC_T  e_mm_sync_mode;
 *      STC_T                   t_stc_init;
 *      BOOL                    b_es_with_pts_in;
 *  } SM_SESS_GNRC_DATA_T;
 *  @endcode
 *  @li@c  b_disable_av_sync                                 - whether disable AV sync .
 *  @li@c  t_media_fmt                                       - mutimedia format.
 *  @li@c  e_mm_sync_mode                                    -mutimedia sync mode
 *  @li@c  t_stc_init                                        -stc init value.
 *  @li@c  b_es_with_pts_in                                  -whether ES with PTS.
 */
/*------------------------------------------------------------------*/
typedef struct _SM_SESS_GNRC_DATA_T
{
	UINT32                  ui2_pcr_pid;
    BOOL                    b_disable_av_sync;
    MEDIA_FORMAT_T          t_media_fmt;
    MM_PRIORITY_TYPE_T      e_priority;
    SM_SESS_GNRC_MM_SYNC_T  e_mm_sync_mode;
    STC_T                   t_stc_init;
    BOOL                    b_es_with_pts_in;
    BOOL                    b_sub_freerun;
} SM_SESS_GNRC_DATA_T;

/* SM_SESS_GNRC_SET_TYPE_SCART_OUT *******************************************/
/*------------------------------------------------------------------*/
/*! @struct SM_SESS_GNRC_SCART_OUT_T
 *  @brief  Contain generic session scart out info.
 *  @code
 *  typedef struct _SM_SESS_GNRC_SCART_OUT_T
 *  {
 *      BOOL    b_set_by_name;
 *      union
 *      {
 *          CHAR*   s_scart_name;
 *          UINT16  ui2_comp_id;
 *      } u;
 *      BOOL    b_enable;
 *  } SM_SESS_GNRC_SCART_OUT_T;
 *  @endcode
 *  @li@c  b_set_by_name                                     -whether set by name type.
 *  @li@c  s_scart_name                                      -scart name.
 *  @li@c  ui2_comp_id                                       -component id.
 *  @li@c  b_enable                                          -whether enable.
 */
/*------------------------------------------------------------------*/
typedef struct _SM_SESS_GNRC_SCART_OUT_T
{
    BOOL    b_set_by_name;
    union
    {
        CHAR*   s_scart_name;
        UINT16  ui2_comp_id;
    } u;
    BOOL    b_enable;
} SM_SESS_GNRC_SCART_OUT_T;

/* SM_SESS_GNRC_SET_TYPE_AV_SYNC *********************************************/
/*------------------------------------------------------------------*/
/*! @enum  SM_SESS_GNRC_AV_SYNC_TYPE_T
 *  @brief  Identify AV sync mode.
 *  @code
 *  typedef enum 
 *  {
 *      SM_SESS_GNRC_AV_SYNC_OFF = 0,
 *      SM_SESS_GNRC_AV_SYNC_AUTO,
 *      SM_SESS_GNRC_AV_SYNC_FORCE_PCR_PID
 *  } SM_SESS_GNRC_AV_SYNC_TYPE_T;
 *  @endcode
 *  @li@c  SM_SESS_GNRC_AV_SYNC_OFF                          -sync off
 *  @li@c  SM_SESS_GNRC_AV_SYNC_AUTO                         -auto mode
 *  @li@c  SM_SESS_GNRC_AV_SYNC_FORCE_PCR_PID                -sync use force PCR PID mode.
 */
/*------------------------------------------------------------------*/
typedef enum 
{
    SM_SESS_GNRC_AV_SYNC_OFF = 0,
    SM_SESS_GNRC_AV_SYNC_AUTO,
    SM_SESS_GNRC_AV_SYNC_FORCE_PCR_PID
} SM_SESS_GNRC_AV_SYNC_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct  SM_SESS_GNRC_AV_SYNC_INFO_T
 *  @brief  Contains generic session AV sync info.
 *  @code
 *  typedef struct _SM_SESS_GNRC_AV_SYNC_INFO_T
 *  {
 *      SM_SESS_GNRC_AV_SYNC_TYPE_T   e_av_sync_type;
 *      union
 *      {
 *          MPEG_2_PID_T              t_force_pcr_pid;
 *      } u;
 *  } SM_SESS_GNRC_AV_SYNC_INFO_T;
 *  @endcode
 *  @li@c  e_av_sync_type                                    -av sync type.
 *  @li@c  t_force_pcr_pid                                   - force PCR PID value.
 */
/*------------------------------------------------------------------*/
typedef struct _SM_SESS_GNRC_AV_SYNC_INFO_T
{
    SM_SESS_GNRC_AV_SYNC_TYPE_T   e_av_sync_type;
    
    union
    {
        MPEG_2_PID_T              t_force_pcr_pid;
    } u;
} SM_SESS_GNRC_AV_SYNC_INFO_T;

/*------------------------------------------------------------------*/
/*! @enum  SM_SESS_GNRC_UOP_T
 *  @brief  Specify Multimedia UOP type.
 *  @code
 *  typedef enum
 *  {
 *      _SM_SESS_GNRC_UOP_UNKNOWN = 0,
 *      _SM_SESS_GNRC_UOP_SEEK,
 *      _SM_SESS_GNRC_UOP_TRICK
 *  } SM_SESS_GNRC_UOP_T;
 *  @endcode
 *  @li@c  _SM_SESS_GNRC_UOP_UNKNOWN                         -unknown UOP type.
 *  @li@c  _SM_SESS_GNRC_UOP_SEEK                            -Use seek operation.
 *  @li@c  _SM_SESS_GNRC_UOP_TRICK                           -Use trick operation.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    _SM_SESS_GNRC_UOP_UNKNOWN = 0,
    _SM_SESS_GNRC_UOP_SEEK,
    _SM_SESS_GNRC_UOP_TRICK
} SM_SESS_GNRC_UOP_T;

/*------------------------------------------------------------------*/
/*! @struct  SM_SESS_GNRC_UOP_CAP_INFO_T
 *  @brief  Contains UOP operation info.
 *  @code
 *  typedef struct _SM_SESS_GNRC_UOP_CAP_INFO_T
 *  {
 *      SM_SESS_GNRC_UOP_T  e_uop;
 *      BOOL         b_is_allowed;
 *      union
 *      {
 *          MM_SPEED_TYPE_T t_speed;
 *      } u;
 *  } SM_SESS_GNRC_UOP_CAP_INFO_T;
 *  @endcode
 *  @li@c  e_uop                                             - UOP type.
 *  @li@c  b_is_allowed                                      - whether operation allow.
 *  @li@c  t_speed                                           -Mutimedia speed.
 */
/*------------------------------------------------------------------*/
typedef struct _SM_SESS_GNRC_UOP_CAP_INFO_T
{
    SM_SESS_GNRC_UOP_T  e_uop;
    BOOL         b_is_allowed;
    union
    {
        MM_SPEED_TYPE_T t_speed;
        /*to be extend*/
    } u;
} SM_SESS_GNRC_UOP_CAP_INFO_T;

/*------------------------------------------------------------------*/
/*! @enum  SM_SESS_GNRC_POST_INFO_TYPE_T
 *  @brief  Specify post info.
 *  @code
 *  typedef enum
 *  {
 *      _SM_SESS_GNRC_POST_INFO_UNKNOWN = 0,
 *      _SM_SESS_GNRC_POST_INFO_MEDIA_BTE_OFST,
 *      _SM_SESS_GNRC_POST_INFO_MEDIA_TM_OFST,
 *      _SM_SESS_GNRC_POST_INFO_MEDIA_RENDER_OFST,
 *      _SM_SESS_GNRC_POST_INFO_RANGE_BTE_OFST,
 *      _SM_SESS_GNRC_POST_INFO_RANGE_TM_OFST
 *  } SM_SESS_GNRC_POST_INFO_TYPE_T;
 *  @endcode
 *  @li@c  _SM_SESS_GNRC_POST_INFO_UNKNOWN                   -
 *  @li@c  _SM_SESS_GNRC_POST_INFO_MEDIA_BTE_OFST            -
 *  @li@c  _SM_SESS_GNRC_POST_INFO_MEDIA_TM_OFST             -
 *  @li@c  _SM_SESS_GNRC_POST_INFO_MEDIA_RENDER_OFST         -
 *  @li@c  _SM_SESS_GNRC_POST_INFO_RANGE_BTE_OFST            -
 *  @li@c  _SM_SESS_GNRC_POST_INFO_RANGE_TM_OFST             -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    _SM_SESS_GNRC_POST_INFO_UNKNOWN = 0,
    _SM_SESS_GNRC_POST_INFO_MEDIA_BTE_OFST,
    _SM_SESS_GNRC_POST_INFO_MEDIA_TM_OFST,
    _SM_SESS_GNRC_POST_INFO_MEDIA_RENDER_OFST,
    _SM_SESS_GNRC_POST_INFO_RANGE_BTE_OFST,
    _SM_SESS_GNRC_POST_INFO_RANGE_TM_OFST
} SM_SESS_GNRC_POST_INFO_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct  SM_SESS_GNRC_RENDER_INFO_T
 *  @brief  Contains render info.
 *  @code
 *  typedef struct _SM_SESS_GNRC_RENDER_INFO_T
 *  {
 *      UINT64  ui8_pts_info;
 *      UINT64  ui8_i_frame_position;
 *      UINT64  ui8_frame_position;
 *      INT32   i4_temporal_reference;
 *      UINT16  ui2_decoding_order;
 *  }SM_SESS_GNRC_RENDER_INFO_T;
 *  @endcode
 *  @li@c  ui8_pts_info                                      -
 *  @li@c  ui8_i_frame_position                              -
 *  @li@c  ui8_frame_position                                -
 *  @li@c  i4_temporal_reference                             -
 *  @li@c  ui2_decoding_order                                -
 */
/*------------------------------------------------------------------*/
typedef struct _SM_SESS_GNRC_RENDER_INFO_T
{
    UINT64  ui8_stc;
    UINT64  ui8_aud_pts_info;
    UINT64  ui8_aud_frame_position;
    UINT64  ui8_i_pts_info;
    UINT64  ui8_pts_info;
    UINT64  ui8_i_frame_position;
    UINT64  ui8_frame_position;
    UINT64  ui8_frame_position_disp;
    INT32   i4_temporal_reference;
    UINT16  ui2_decoding_order;
    UINT32  ui4_timestap;
}SM_SESS_GNRC_RENDER_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct  SM_SESS_GNRC_POST_INFO_T
 *  @brief  Contains post info.
 *  @code
 *  typedef struct _SM_SESS_GNRC_FILE_INFO_T
 *  {
 *      SM_SESS_GNRC_POST_INFO_TYPE_T  e_type;
 *      UINT64 ui8_pos_info;
 *      union
 *      {
 *          SM_SESS_GNRC_RENDER_INFO_T render_info;              
 *      } u;
 *  } SM_SESS_GNRC_POST_INFO_T;
 *  @endcode
 *  @li@c  e_type                                            -
 *  @li@c  ui8_pos_info                                      -
 *  @li@c  render_info                                       -
 */
/*------------------------------------------------------------------*/
typedef struct _SM_SESS_GNRC_FILE_INFO_T
{
    SM_SESS_GNRC_POST_INFO_TYPE_T  e_type;
    UINT64 ui8_pos_info;
    union
    {
        SM_SESS_GNRC_RENDER_INFO_T render_info;              
    } u;
} SM_SESS_GNRC_POST_INFO_T;

/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/



#endif /* _U_SM_SESS_GNRC_H_ */
/*----------------------------------------------------------------------------*/
/*! @} */                                         /* end of groupMW_STRM_MNGR_SESS_GNRC_HDLR */
/*----------------------------------------------------------------------------*/

