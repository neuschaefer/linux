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
/*! @file u_inp_hdlr.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 * 1. If a possible type is DEV_AVC_COMBI, inp_hdler will open DSM for this type,
 *    and start to monitor its signal status
 * 2. when DSM reports signal status,
 *      2.1 if preferred type with signal, inp_hdler will select this type to connect
 *      2.2 if preferred type without signal, inp_hdler will select the first one with signal
 * 3. If one DEV_AVC_COMBI type includes several types, inp_hdlr should provide a way to
 *    interate specific type regardless of it with or without signal.
 * 4. If a possible type is not DEV_AVC_COMBI, inp_hdler will select this type to connect
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_SVCTX_INP_HDLR Input Handler
 *
 *  @ingroup groupMW_SVCTX
 *  @brief Provides external function TV Application
 *
 *  %MW Application API would be used by TV Application, lot of these APIs would
 *   be wrapped from Exported API
 *
 *  @see groupMW_SVCTX
 *  @{ 
 */
/*----------------------------------------------------------------------------*/
#ifndef _U_INP_HDLR_H_
#define _U_INP_HDLR_H_
/*-----------------------------------------------------------------------------
 *  include files
 *---------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_device_type.h"
#include "u_svctx.h"
#include "u_avc_handler.h"
/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#define SRC_TYPE_AVC                ((SRC_TYPE_T) 4)    /**<        */

/*------------------------------------------------------------------*/
/*! @enum SRC_AVC_HINT_TYPE_T
 *  @brief  Specify AVC hint type
 *  @code
 *  typedef enum
 *  {
 *      SRC_AVC_HINT_NONE = 0,
 *      SRC_AVC_HINT_SPECIFIC_DEVICE_ONLY,
 *      SRC_AVC_HINT_SPECIFIC_DEVICE_FIRST,
 *      SRC_AVC_HINT_BETTER_QUALITY_DEVICE_FIRST
 *  } SRC_AVC_HINT_TYPE_T;
 *  @endcode
 *  @li@c  SRC_AVC_HINT_NONE                                 -AVC hint type none
 *  @li@c  SRC_AVC_HINT_SPECIFIC_DEVICE_ONLY                 -AVC hint type specific device only
 *  @li@c  SRC_AVC_HINT_SPECIFIC_DEVICE_FIRST                -AVC hint type specific device first
 *  @li@c  SRC_AVC_HINT_BETTER_QUALITY_DEVICE_FIRST     -AVC hint type better quality device first
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SRC_AVC_HINT_NONE = 0,
    SRC_AVC_HINT_SPECIFIC_DEVICE_ONLY,
    SRC_AVC_HINT_SPECIFIC_DEVICE_FIRST,
    SRC_AVC_HINT_BETTER_QUALITY_DEVICE_FIRST
} SRC_AVC_HINT_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct  SRC_AVC_HINT_T
 *  @brief  Contain AVC hint info
 *  @code
 *  typedef struct _SRC_AVC_HINT_T
 *  {
 *      DEVICE_TYPE_T                   e_dev_type;
 *      SRC_AVC_HINT_TYPE_T             e_type;
 *  } SRC_AVC_HINT_T;
 *  @endcode
 *  @li@c  e_dev_type                                        -AVC hint device type
 *  @li@c  e_type                                            -AVC hint type
 */
/*------------------------------------------------------------------*/
typedef struct _SRC_AVC_HINT_T
{
    DEVICE_TYPE_T                   e_dev_type;
    SRC_AVC_HINT_TYPE_T             e_type;
} SRC_AVC_HINT_T;

/*------------------------------------------------------------------*/
/*! @enum SRC_AVC_CHG_TYPE_T
 *  @brief  Specify AVC source change type
 *  @code
 *  typedef enum
 *  {
 *      SRC_AVC_CHG_MANUAL = 0,
 *      SRC_AVC_CHG_SCART_PIN8_AUTO
 *  } SRC_AVC_CHG_TYPE_T;
 *  @endcode
 *  @li@c  SRC_AVC_CHG_MANUAL                      -AVC source change type manual
 *  @li@c  SRC_AVC_CHG_SCART_PIN8_AUTO     -AVC source change type auto by scart PIN8
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SRC_AVC_CHG_MANUAL = 0,
    SRC_AVC_CHG_SCART_PIN8_AUTO
} SRC_AVC_CHG_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct  SRC_AVC_T
 *  @brief  Contain AVC info
 *  @code
 *  typedef struct _SRC_AVC_T  
 *  {
 *      DEVICE_TYPE_T                   e_video_type;  
 *      SRC_AVC_HINT_T                  t_video_hint;
 *      DEVICE_TYPE_T                   e_audio_type;  
 *      SRC_AVC_HINT_T                  t_audio_hint;
 *      AVC_HANDLER_SCART_MODE_T        e_scart_mode;
 *      SRC_AVC_CHG_TYPE_T              e_src_chg_type;
 *  } SRC_AVC_T;
 *  @endcode
 *  @li@c  e_video_type                                      -AVC info video type
 *  @li@c  t_video_hint                                      -AVC info video hint
 *  @li@c  e_audio_type                                      -AVC info audio type
 *  @li@c  t_audio_hint                                      -AVC info audio hint
 *  @li@c  e_scart_mode                                      -AVC info scart mode
 *  @li@c  e_src_chg_type                                    -AVC info source change type
 */
/*------------------------------------------------------------------*/
typedef struct _SRC_AVC_T  /* SRC_DESC_T's pv_details when e_type = SRC_TYPE_AVC*/
{
    DEVICE_TYPE_T                   e_video_type;  /* e.g DEV_AVC_COMBI     */
    SRC_AVC_HINT_T                  t_video_hint;

    DEVICE_TYPE_T                   e_audio_type;  /* e.g DEV_AVC_AUDIO_INP */
    SRC_AVC_HINT_T                  t_audio_hint;

    AVC_HANDLER_SCART_MODE_T        e_scart_mode;

    SRC_AVC_CHG_TYPE_T              e_src_chg_type; /* user manual change or monitor Scart PIN8 auto change */
} SRC_AVC_T;

/*------------------------------------------------------------------*/
/*! @struct  SRC_AVC_INFO_T
 *  @brief  Contain AVC info
 *  @code
 *  typedef struct _SRC_AVC_INFO_T
 *  {
 *      DEVICE_TYPE_T                   e_type;
 *      SRC_AVC_HINT_T                  t_hint;
 *  } SRC_AVC_INFO_T;
 *  @endcode
 *  @li@c  e_type                                            -AVC info type
 *  @li@c  t_hint                                            -AVC info hint type
 */
/*------------------------------------------------------------------*/
typedef struct _SRC_AVC_INFO_T
{
    DEVICE_TYPE_T                   e_type;
    SRC_AVC_HINT_T                  t_hint;
} SRC_AVC_INFO_T;

/*------------------------------------------------------------------*/
/*! @enum VIDEO_DEVICE_AUDIO_COND_T
 *  @brief  Specify video device audio condition type
 *  @code
 *  typedef enum
 *  {
 *      VIDEO_DEVICE_AUDIO_COND_UNKNOWN = 0,
 *      VIDEO_DEVICE_AUDIO_COND_WITH_AUDIO,
 *      VIDEO_DEVICE_AUDIO_COND_NO_AUDIO
 *  } VIDEO_DEVICE_AUDIO_COND_T;
 *  @endcode
 *  @li@c  VIDEO_DEVICE_AUDIO_COND_UNKNOWN       -video device audio condition type unknown
 *  @li@c  VIDEO_DEVICE_AUDIO_COND_WITH_AUDIO   -video device audio condition type with audio
 *  @li@c  VIDEO_DEVICE_AUDIO_COND_NO_AUDIO      -video device audio condition type no audio
 */
/*------------------------------------------------------------------*/
typedef enum
{
    VIDEO_DEVICE_AUDIO_COND_UNKNOWN = 0,
    VIDEO_DEVICE_AUDIO_COND_WITH_AUDIO,
    VIDEO_DEVICE_AUDIO_COND_NO_AUDIO
} VIDEO_DEVICE_AUDIO_COND_T;

/*------------------------------------------------------------------*/
/*! @brief        Typedef for the input handle iterate devices in combi callback function. 
 *  @param [in] ui2_num_dev             - device number
 *  @param [in] ui2_idx                      - index
 *  @param [in] e_dev_type               - device type
 *  @param [in] b_in_using                 - in using or not
 *  @param [in] b_with_video_signal   - with video signal or not
 *  @param [in] b_with_audio_signal   - with audio signal or not
 *  @note
 *  @see
 *  @return None
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_inp_hdlr_iterate_devices_in_combi_fct)(
                    UINT16                      ui2_num_dev,
                    UINT16                      ui2_idx,
                    DEVICE_TYPE_T               e_dev_type,
                    BOOL                        b_in_using,
                    BOOL                        b_with_video_signal,
                    BOOL                        b_with_audio_signal
                    );

#define SVCTX_GRP_INP                                     (3<<SVCTX_TYPE_CODE_BITS)    /**<        */
/* Input handler set types *********************************************************/
#define SVCTX_INP_SET_TYPE_IGNORE                         ((SVCTX_SET_TYPE_T)(SVCTX_GRP_INP | 0))    /**<        */
#define SVCTX_INP_SET_TYPE_NEXT_DEVICE_IN_VIDEO_COMBI     ((SVCTX_SET_TYPE_T)(SVCTX_GRP_INP | 1))    /**<        */
#define SVCTX_INP_SET_TYPE_NEXT_DEVICE_IN_AUDIO_COMBI     ((SVCTX_SET_TYPE_T)(SVCTX_GRP_INP | 2))    /**<        */
#define SVCTX_INP_SET_TYPE_NEXT_SIGNALED_IN_VIDEO_COMBI   ((SVCTX_SET_TYPE_T)(SVCTX_GRP_INP | 3))    /**<        */
#define SVCTX_INP_SET_TYPE_NEXT_SIGNALED_IN_AUDIO_COMBI   ((SVCTX_SET_TYPE_T)(SVCTX_GRP_INP | 4))    /**<        */
#define SVCTX_INP_SET_TYPE_SELECT_DEVICE_IN_VIDEO_COMBI   ((SVCTX_SET_TYPE_T)(SVCTX_GRP_INP | 5))    /**<        */
#define SVCTX_INP_SET_TYPE_SELECT_DEVICE_IN_AUDIO_COMBI   ((SVCTX_SET_TYPE_T)(SVCTX_GRP_INP | 6))    /**<        */
#define SVCTX_INP_SET_TYPE_AUDIO_SRC_INFO                 ((SVCTX_SET_TYPE_T)(SVCTX_GRP_INP | 7)) /* type = SRC_AVC_INFO_T*             size = sizeof(SRC_AVC_INFO_T)            */    /**<        */

/* Input handler get types *********************************************************/
#define SVCTX_INP_GET_TYPE_IGNORE                         ((SVCTX_GET_TYPE_T)(SVCTX_GRP_INP | 0))    /**<        */
#define SVCTX_INP_GET_TYPE_ITERATE_DEVICES_IN_VIDEO_COMBI ((SVCTX_GET_TYPE_T)(SVCTX_GRP_INP | 1))    /**<        */
#define SVCTX_INP_GET_TYPE_ITERATE_DEVICES_IN_AUDIO_COMBI ((SVCTX_GET_TYPE_T)(SVCTX_GRP_INP | 2))    /**<        */
#define SVCTX_INP_GET_TYPE_VIDEO_DEVICE_AUDIO_COND        ((SVCTX_GET_TYPE_T)(SVCTX_GRP_INP | 3)) /* type = VIDEO_DEVICE_AUDIO_COND_T*  size = sizeof(VIDEO_DEVICE_AUDIO_COND_T) */    /**<        */
#define SVCTX_INP_GET_TYPE_SRC_AVC_INFO                   ((SVCTX_GET_TYPE_T)(SVCTX_GRP_INP | 4)) /* type = SRC_AVC_T*                  size = sizeof(SRC_AVC_T)                 */    /**<        */
#endif /* _U_INP_HDLR_H_ */
/*----------------------------------------------------------------------------*/
/*! @} */                                              /* end of groupMW_SVCTX_INP_HDLR*/
/*----------------------------------------------------------------------------*/

