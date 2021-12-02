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
/*! @file u_msvctx.h
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *
 *  @par Description:
 *       This file exports all media service context specific definitions and
 *       structures.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_MSVCTX
 *  @{
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_MSVCTX_H_
#define _U_MSVCTX_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "u_common.h"
#include "x_common.h"
#include "u_handle.h"
#include "x_dbg.h"
#include "u_dbg.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define MP3_MSVCTX_HANDLER_NAME "mp3 MSVCTX hdlr" /**< The name of MP3 Handler */
#define MP2_MSVCTX_HANDLER_NAME "mp2 MSVCTX hdlr" /**< The name of MP2 Handler */

#define MSVCTXR_OK                        ((INT32)   0)  /**< Successful */
#define MSVCTXR_INSUFFICIENT_BUFFER       ((INT32)  -1)  /**< Buffer is insufficient */
#define MSVCTXR_FILE_OR_DIR_MISSING       ((INT32)  -2)  /**< File or DIR is missing */
#define MSVCTXR_INV_HANDLE                ((INT32)  -3)  /**< Handle is invalid */
#define MSVCTXR_OUT_OF_HANDLE             ((INT32)  -4)  /**< Handle is out of usage */
#define MSVCTXR_OUT_OF_MEMORY             ((INT32)  -5)  /**< Memory is out of usage */
#define MSVCTXR_FCT_NOT_SUPPORT           ((INT32)  -6)  /**< Function is not supported */
#define MSVCTXR_NAME_EXIST                ((INT32)  -7)  /**< Name already exists */
#define MSVCTXR_INV_ARG                   ((INT32)  -8)  /**< Invalid arguments */
#define MSVCTXR_HANDLER_NOT_FOUND         ((INT32)  -9)  /**< Handler is not found */
#define MSVCTXR_INTERNAL_ERR              ((INT32) -10)  /**< Internal error */
#define MSVCTXR_REENTRY_LOADING           ((INT32) -11)  /**< Reentry loading occurs */
#define MSVCTXR_INITIAL_ERR               ((INT32) -12)  /**< Intitial error */
#define MSVCTXR_MSG_Q_ERR                 ((INT32) -13)  /**< Message queue is error */
#define MSVCTXR_FILE_ERR                  ((INT32) -14)  /**< File error */
#define MSVCTXR_CLI_ERROR                 ((INT32) -15)  /**< CLI error */
#define MSVCTXR_AUTOFREE_ERROR            ((INT32) -16)  /**< Autofree error */
#define MSVCTXR_FMT_NOT_SUPPORT           ((INT32) -17)  /**< Media format is not supported */
#define MSVCTXR_INDEX_NOT_FOUND           ((INT32) -18)  /**< Index is not supported */
#define MSVCTXR_INFO_NOT_READY            ((INT32) -19)  /**< Information is not ready */
#define MSVCTXR_NO_ELM_QUEUED             ((INT32) -20)  /**< No element queued */
#define MSVCTXR_NO_AUD_STM                ((INT32) -21)  /**< No audio stream */
#define MSVCTXR_NO_VID_STM                ((INT32) -22)  /**< No video stream */

/*---------------------------------------------------------------------------*/
/*! @enum  MSVCTX_NFY_EVENT_TYPE_T
 *  @brief This structure enumerates all types of the notification of MSVCTX to
 *         AP.
 *
 *  @code
 *  typedef enum
 *  {
 *      MSVCTX_NFY_EVENT_PLAY_SVC_DONE = 0,
 *      MSVCTX_NFY_EVENT_STOP_SVC_DONE,
 *      MSVCTX_NFY_EVENT_PAUSE_SVC_DONE,
 *      MSVCTX_NFY_EVENT_NEXT_SVC_DONE,
 *      MSVCTX_NFY_EVENT_SET_POS_DONE,
 *      MSVCTX_NFY_EVENT_ELM_START_PLAYBACK,
 *      MSVCTX_NFY_EVENT_ELM_END_PLAYBACK,
 *      MSVCTX_NFY_EVENT_HANDLE_CLOSED,
 *      MSVCTX_NFY_EVENT_STREAM_INFO_CHANGED,
 *      MSVCTX_NFY_EVENT_AUD_STREAM_OPENED,
 *      MSVCTX_NFY_EVENT_VID_STREAM_OPENED,
 *      MSVCTX_NFY_EVENT_AUD_STREAM_CLOSED,
 *      MSVCTX_NFY_EVENT_VID_STREAM_CLOSED,
 *      MSVCTX_NFY_EVENT_ERROR,
 *      MSVCTX_NFY_EVENT_PARSING_ERROR,
 *      MSVCTX_NFY_EVENT_PARSING_DONE,
 *      MSVCTX_NFY_EVENT_INDEXING_DONE,
 *      MSVCTX_NFY_EVENT_INDEXING_ABORT,
 *      MSVCTX_NFY_EVENT_INDEXING_ERROR
 *  }   MSVCTX_NFY_EVENT_TYPE_T;
 *  @endcode
 *
 *  @li@c  MSVCTX_NFY_EVENT_PLAY_SVC_DONE - pv_data1: NULL, pv_data2: NULL
 *  @li@c  MSVCTX_NFY_EVENT_STOP_SVC_DONE - pv_data1: CM result, pv_data2: NULL
 *  @li@c  MSVCTX_NFY_EVENT_PAUSE_SVC_DONE - pv_data1: NULL, pv_data2: NULL
 *  @li@c  MSVCTX_NFY_EVENT_NEXT_SVC_DONE - pv_data1: previous elm handle,
 *                                          pv_data2: next elm handle
 *  @li@c  MSVCTX_NFY_EVENT_SET_POS_DONE - pv_data1: b_is_out_of_range,
 *                                         pv_data2: NULL
 *  @li@c  MSVCTX_NFY_EVENT_ELM_START_PLAYBACK - pv_data1: elm handle,
 *                                               pv_data2: NULL
 *  @li@c  MSVCTX_NFY_EVENT_ELM_END_PLAYBACK - pv_data1: elm handle,
 *                                             pv_data2: NULL
 *  @li@c  MSVCTX_NFY_EVENT_HANDLE_CLOSED - pv_data1: NULL, pv_data2: NULL
 *  @li@c  MSVCTX_NFY_EVENT_STREAM_INFO_CHANGED - pv_data1: h_stream,
 *                                                pv_data2: &ASH_AUDIO_INFO_T
 *  @li@c  MSVCTX_NFY_EVENT_AUD_STREAM_OPENED - pv_data1: h_stream,
 *                                              pv_data2: NULL
 *  @li@c  MSVCTX_NFY_EVENT_VID_STREAM_OPENED - pv_data1: h_stream,
 *                                              pv_data2: NULL
 *  @li@c  MSVCTX_NFY_EVENT_AUD_STREAM_CLOSED - pv_data1: h_stream,
 *                                              pv_data2: NULL
 *  @li@c  MSVCTX_NFY_EVENT_VID_STREAM_CLOSED - pv_data1: h_stream,
 *                                              pv_data2: NULL
 *  @li@c  MSVCTX_NFY_EVENT_ERROR - pv_data1: result, pv_data2: NULL
 *  @li@c  MSVCTX_NFY_EVENT_PARSING_ERROR - pv_data1: NULL, pv_data2: NULL
 *  @li@c  MSVCTX_NFY_EVENT_PARSING_DONE - pv_data1: NULL, pv_data2: NULL
 *  @li@c  MSVCTX_NFY_EVENT_INDEXING_DONE - pv_data1: NULL, pv_data2: NULL
 *  @li@c  MSVCTX_NFY_EVENT_INDEXING_ABORT - pv_data1: NULL, pv_data2: NULL
 *  @li@c  MSVCTX_NFY_EVENT_INDEXING_ERROR - pv_data1: NULL, pv_data2: NULL
 */
/*---------------------------------------------------------------------------*/

typedef enum
{
    MSVCTX_NFY_EVENT_PLAY_SVC_DONE = 0,
    MSVCTX_NFY_EVENT_STOP_SVC_DONE,
    MSVCTX_NFY_EVENT_PAUSE_SVC_DONE,
    MSVCTX_NFY_EVENT_NEXT_SVC_DONE,
    MSVCTX_NFY_EVENT_SET_POS_DONE,
    MSVCTX_NFY_EVENT_ELM_START_PLAYBACK,
    MSVCTX_NFY_EVENT_ELM_END_PLAYBACK,
    MSVCTX_NFY_EVENT_HANDLE_CLOSED,
    MSVCTX_NFY_EVENT_STREAM_INFO_CHANGED,
    MSVCTX_NFY_EVENT_AUD_STREAM_OPENED,
    MSVCTX_NFY_EVENT_VID_STREAM_OPENED,
    MSVCTX_NFY_EVENT_AUD_STREAM_CLOSED,
    MSVCTX_NFY_EVENT_VID_STREAM_CLOSED,
    MSVCTX_NFY_EVENT_ERROR,
    MSVCTX_NFY_EVENT_PARSING_ERROR,
    MSVCTX_NFY_EVENT_PARSING_DONE,
    MSVCTX_NFY_EVENT_INDEXING_DONE,
    MSVCTX_NFY_EVENT_INDEXING_ABORT,
    MSVCTX_NFY_EVENT_INDEXING_ERROR
}   MSVCTX_NFY_EVENT_TYPE_T;

/*---------------------------------------------------------------------------*/
/*! @enum  MSVCTX_PLAY_SPEED_T
 *  @brief This structure enumerates all speed of playing.
 *
 *  @code
 *  typedef enum
 *  {
 *      MSVCTX_PLAY_SPEED_1X = 0,
 *  }   MSVCTX_PLAY_SPEED_T;
 *  @endcode
 *
 *  @li@c  MSVCTX_PLAY_SPEED_1X
 */
/*---------------------------------------------------------------------------*/

typedef enum
{
    MSVCTX_PLAY_SPEED_1X = 0
} MSVCTX_PLAY_SPEED_T;

/*---------------------------------------------------------------------------*/
/*! @enum  MSVCTX_INFO_TYPE_T
 *  @brief This structure enumerates all info type of MSVCTX.
 *
 *  @code
 *  typedef enum
 *  {
 *      MSVCTX_INFO_TYPE_ID3_ATTACHED_PIC = 0,
 *      MSVCTX_INFO_TYPE_ID3_ATTACHED_PIC_BITMASK,
 *      MSVCTX_INFO_TYPE_ID3_TITLE,
 *      MSVCTX_INFO_TYPE_ID3_ARTIST,
 *      MSVCTX_INFO_TYPE_ID3_ALBUM,
 *      MSVCTX_INFO_TYPE_ID3_YEAR,
 *      MSVCTX_INFO_TYPE_ID3_GENRE,
 *      MSVCTX_INFO_TYPE_ID3_TRACK,
 *      MSVCTX_INFO_TYPE_ID3_V1_LENG,
 *      MSVCTX_INFO_TYPE_ID3_V2_LENG,
 *      MSVCTX_INFO_TYPE_CUR_PLAY_STATUS,
 *      MSVCTX_INFO_TYPE_MEDIA_LENGTH_IN_SEC,
 *      MSVCTX_INFO_TYPE_MEDIA_SIZE_IN_BYTES,
 *      MSVCTX_INFO_TYPE_POS_CONV_TIME2BYTE,
 *      MSVCTX_INFO_TYPE_POS_CONV_BYTE2TIME,
 *      MSVCTX_INFO_TYPE_FILE_PATH,
 *      MSVCTX_INFO_TYPE_LAYER,
 *      MSVCTX_INFO_TYPE_LAST_ITEM
 *  }   MSVCTX_INFO_TYPE_T;
 *  @endcode
 *
 *  @li@c  MSVCTX_INFO_TYPE_ID3_ATTACHED_PIC - MINFO_INFO_PIC_T
 *  @li@c  MSVCTX_INFO_TYPE_ID3_ATTACHED_PIC_BITMASK - UINT32
 *  @li@c  MSVCTX_INFO_TYPE_ID3_TITLE - UTF16
 *  @li@c  MSVCTX_INFO_TYPE_ID3_ARTIST - MINFO_INFO_PIC_T
 *  @li@c  MSVCTX_INFO_TYPE_ID3_ALBUM - UTF16
 *  @li@c  MSVCTX_INFO_TYPE_ID3_YEAR - UTF16
 *  @li@c  MSVCTX_INFO_TYPE_ID3_GENRE - UTF16
 *  @li@c  MSVCTX_INFO_TYPE_ID3_TRACK - UTF16
 *  @li@c  MSVCTX_INFO_TYPE_ID3_V1_LENG -
 *  @li@c  MSVCTX_INFO_TYPE_ID3_V2_LENG -
 *  @li@c  MSVCTX_INFO_TYPE_CUR_PLAY_STATUS -
 *  @li@c  MSVCTX_INFO_TYPE_MEDIA_LENGTH_IN_SEC -
 *  @li@c  MSVCTX_INFO_TYPE_MEDIA_SIZE_IN_BYTES -
 *  @li@c  MSVCTX_INFO_TYPE_POS_CONV_TIME2BYTE -
 *  @li@c  MSVCTX_INFO_TYPE_POS_CONV_BYTE2TIME -
 *  @li@c  MSVCTX_INFO_TYPE_FILE_PATH -
 *  @li@c  MSVCTX_INFO_TYPE_LAYER - 0 none, 1 layer 1, 2 layer 2, 3 layer 3
 *  @li@c  MSVCTX_INFO_TYPE_LAST_ITEM - This item is only for argumant checking,
 *                                      do not remove or modify it
 */
/*---------------------------------------------------------------------------*/

typedef enum
{
    MSVCTX_INFO_TYPE_ID3_ATTACHED_PIC = 0,
    MSVCTX_INFO_TYPE_ID3_ATTACHED_PIC_BITMASK,
    MSVCTX_INFO_TYPE_ID3_TITLE,
    MSVCTX_INFO_TYPE_ID3_ARTIST,
    MSVCTX_INFO_TYPE_ID3_ALBUM,
    MSVCTX_INFO_TYPE_ID3_YEAR,
    MSVCTX_INFO_TYPE_ID3_GENRE,
    MSVCTX_INFO_TYPE_ID3_TRACK,
    MSVCTX_INFO_TYPE_ID3_V1_LENG,
    MSVCTX_INFO_TYPE_ID3_V2_LENG,
    MSVCTX_INFO_TYPE_CUR_PLAY_STATUS,
    MSVCTX_INFO_TYPE_MEDIA_LENGTH_IN_SEC,
    MSVCTX_INFO_TYPE_MEDIA_SIZE_IN_BYTES,
    MSVCTX_INFO_TYPE_POS_CONV_TIME2BYTE,
    MSVCTX_INFO_TYPE_POS_CONV_BYTE2TIME,
    MSVCTX_INFO_TYPE_FILE_PATH,
    MSVCTX_INFO_TYPE_LAYER,
    MSVCTX_INFO_TYPE_LAST_ITEM
} MSVCTX_INFO_TYPE_T;

/*---------------------------------------------------------------------------*/
/*! @enum  MSVCTX_CUR_STATUS_T
 *  @brief This structure enumerates all current status of MSVCTX.
 *
 *  @code
 *  typedef enum
 *  {
 *      MSVCTX_CUR_STATUS_STOP = 0,
 *      MSVCTX_CUR_STATUS_PAUSE,
 *      MSVCTX_CUR_STATUS_PLAY,
 *  }   MSVCTX_CUR_STATUS_T;
 *  @endcode
 *
 *  @li@c  MSVCTX_CUR_STATUS_STOP  - Current status is STOP
 *  @li@c  MSVCTX_CUR_STATUS_PAUSE - Current status is PAUSE
 *  @li@c  MSVCTX_CUR_STATUS_PLAY  - Current status is PLAY
 */
/*---------------------------------------------------------------------------*/

typedef enum
{
    MSVCTX_CUR_STATUS_STOP = 0,
    MSVCTX_CUR_STATUS_PAUSE,
    MSVCTX_CUR_STATUS_PLAY,
} MSVCTX_CUR_STATUS_T;

/*---------------------------------------------------------------------------*/
/*! @struct MSVCTX_OFFSET_CONVERT_T
 *  @brief  This structure contains the offset convert object information
 *
 *  @code
 *  typedef struct _MSVCTX_OFFSET_CONVERT_T
 *  {
 *      UINT32    ui4_time_offset;
 *      UINT32    ui4_byte_offset;
 *      BOOL      b_is_out_of_range;
 *  } MSVCTX_OFFSET_CONVERT_T;
 *  @endcode
 *
 *  @li@c   ui4_time_offset   - Offset in seconds
 *  @li@c   ui4_byte_offset   - Offset in bytes
 *  @li@c   b_is_out_of_range - Out of range or NOT
 */
/*---------------------------------------------------------------------------*/

typedef struct _MSVCTX_OFFSET_CONVERT_T
{
    UINT32    ui4_time_offset;
    UINT32    ui4_byte_offset;
    BOOL      b_is_out_of_range;
} MSVCTX_OFFSET_CONVERT_T;

/*---------------------------------------------------------------------------*/
/*! @brief This callback function is the media service context notification
 *         function. This will be called when media service context status
 *         changed. Playback functions in MSVCTX are asynchronous functions, so
 *         it needs this notification function to notify applications
 *
 *  @param [in] h_msvctx - Specifies which handle will be notified.
 *  @param [in] e_event - it identifies the event type of notification.
 *  @param [in] pv_data1 - specify the notification data1 which is followed with
 *                         the notification event.
 *  @param [in] pv_data2 - specify the notification data2 which is followed with
 *                         the notification event.
 *  @param [in] pv_tag - The client data specified by the client when callback
 *                       has been called, the callback function will send it
 *                       again.
 *
 *  @note
 *  @see   c_msvctx_open
 *
 *  @return
 *  @retval
 */
/*---------------------------------------------------------------------------*/

typedef VOID (*x_msvctx_nfy_fct) (
    HANDLE_T                   h_msvctx,
    MSVCTX_NFY_EVENT_TYPE_T	   e_event,
    const VOID*	               pv_data1,
    const VOID*	               pv_data2,
    const VOID*	               pv_tag
);

/*! @} */

#endif /* _U_MSVCTX_H_ */

