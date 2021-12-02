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
/*! @file u_avc_handler.h.h
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *
 *  @par Description:
 *       This header file contains the AVC Connection Handler specific
 *       definitions that are exported.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_CONN_MNGR_AVC_HDLR AVC Handler
 *  @ingroup  groupMW_CONN_MNGR
 *  @brief    Provides a unified interface for AVC connection
 *  @see      groupMW_CONN_MNGR
 *
 *  @{
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_AVC_HANDLER_H_
#define _U_AVC_HANDLER_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_cm.h"
#include "u_handle.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define AVC_HANDLER_DEFAULT_NAME    "avc_handler"    /**< Default name of AVC handler */

/* Connection command codes */
#define AVC_CONN_OFFSET                  ((CM_COMMAND_CODE_T) 0x80030000)    /**< Command codes offset */

#define AVC_CONN_CONNECTOR_TYPE             (AVC_CONN_OFFSET +  0)    /**< Connection command codes - Connector type */
#define AVC_CONN_DISC_IF_TV_SIG_LOST        (AVC_CONN_OFFSET +  1)    /**< Connection command codes - TV signal lost should trigger a disconnection or NOT */
#define AVC_CONN_TV_DECODER_NAME            (AVC_CONN_OFFSET +  2)    /**< Connection command codes - TV decoder name */
#define AVC_CONN_COMBI_INDEX                (AVC_CONN_OFFSET +  3)    /**< Connection command codes - Index of COMBI */
#define AVC_CONN_TV_SIGNAL_TIME_OUT         (AVC_CONN_OFFSET +  4)    /**< Connection command codes - Timeout value when doing TV signal detection */
#define AVC_CONN_DISC_IF_COMP_BUSY          (AVC_CONN_OFFSET +  5)    /**< Connection command codes - Disconnect if components is busy */
#define AVC_CONN_SCART_INP_MODE             (AVC_CONN_OFFSET +  6)    /**< Connection command codes - Input mode of SCART */
#define AVC_CONN_INPUT_NO_RIGHTS_OK         (AVC_CONN_OFFSET +  7)    /**< Connection command codes - No rights OK when connect to AVC */
#define AVC_CONN_TV_DECODER_NO_RIGHTS_OK    (AVC_CONN_OFFSET +  8)    /**< Connection command codes - No rights OK when connect to TV decoder */

#define AVC_HANDLER_OFFSET         ((CM_CTRL_TYPE_T) 0x04000000)
#define AVC_HANDLER_SET_GET(_x)    (CM_CTRL_HANDLER_PRIVATE_OFFSET + AVC_HANDLER_OFFSET + (_x))      /**< AVC-specific commands to use with x_cm_set/get */

#define AVC_HANDLER_CTRL_GET_SCART_PIN_8_STATUS     AVC_HANDLER_SET_GET(1)      /**< Get commands - Get PIN 8 status of SCART */
#define AVC_HANDLER_CTRL_GET_SCART_PIN_16_STATUS    AVC_HANDLER_SET_GET(2)      /**< Get commands - Get PIN 16 status of SCART */
#define AVC_HANDLER_CTRL_GET_SCART_INP_CAP          AVC_HANDLER_SET_GET(3)      /**< Get commands - Get capability of SCART */
#define AVC_HANDLER_CTRL_GET_SCART_INP_MODE         AVC_HANDLER_SET_GET(4)      /**< Get commands - Get input mode of SCART */
#define AVC_HANDLER_CTRL_GET_TV_DEC_COLOR_SYS       AVC_HANDLER_SET_GET(5)      /**< Get commands - Get color system of TV decoder */
#define AVC_HANDLER_CTRL_GET_SIGNAL_STRENGTH        AVC_HANDLER_SET_GET(6)      /**< Get commands - Get signal strength of TV decoder */

#define AVC_HANDLER_CTRL_SET_SCART_INP_MODE         AVC_HANDLER_SET_GET(101)    /**< Set commands - Set input mode of SCART */

/* Disconnection reasons */
#define AVC_DISC_AS_REQUESTED                           0U    /**< Disconnection reasons - As requested */
#define AVC_DISC_OTHER_REASON                           1U    /**< Disconnection reasons - Other reason */
#define AVC_DISC_NEW_CONNECTION                         2U    /**< Disconnection reasons - New connection */
#define AVC_DISC_NO_AVC_FOUND                           3U    /**< Disconnection reasons - No AVC is found */
#define AVC_DISC_NO_TV_DECODER_FOUND                    4U    /**< Disconnection reasons - No TV decoder is found */
#define AVC_DISC_COMP_BUSY                              5U    /**< Disconnection reasons - Components are busy */
#define AVC_DISC_COULD_NOT_SET_AVC_PROPERTIES           6U    /**< Disconnection reasons - Could not set properties of AVC */
#define AVC_DISC_COULD_NOT_SET_TV_DECODER_PROPERTIES    7U    /**< Disconnection reasons - Could not set properties of TV decoder */
#define AVC_DISC_COULD_NOT_CONNECT_AVC                  8U    /**< Disconnection reasons - Could not connect to AVC */
#define AVC_DISC_COULD_NOT_CONNECT_TV_DECODER           9U    /**< Disconnection reasons - Could not connect to TV decoder */
#define AVC_DISC_TV_SIGNAL_TIMER_EXPIRED               10U    /**< Disconnection reasons - Timer is expired */
#define AVC_DISC_TV_SIGNAL_LOST                        11U    /**< Disconnection reasons - Signal is lost */

/*---------------------------------------------------------------------------*/
/*! @struct AVC_HANDLER_TIME_OUT_T
 *  @brief  This structure contains the timeout information
 *
 *  @code
 *  typedef struct _AVC_HANDLER_TIME_OUT_T
 *  {
 *      UINT32    ui4_tv_signal_time_out;
 *  } AVC_HANDLER_TIME_OUT_T;
 *  @endcode
 *
 *  @li@c   ui4_tv_signal_time_out  - Timeout value when doing TV signal
 *                                    detection
 */
/*---------------------------------------------------------------------------*/
typedef struct _AVC_HANDLER_TIME_OUT_T
{
    UINT32    ui4_tv_signal_time_out;
} AVC_HANDLER_TIME_OUT_T;

/*---------------------------------------------------------------------------*/
/*! @enum  AVC_HANDLER_CTRL_SCART_PIN_8_STATUS_T
 *  @brief This structure enumerates all PIN 8 status of SCART and to be used
 *         with AVC_CTRL_GET_SCART_PIN_8_STATUS command
 *
 *  @code
 *  typedef enum
 *  {
 *      AVC_HANDLER_SCART_PIN_8_UNKNOWN,
 *      AVC_HANDLER_SCART_PIN_8_NO_SIGNAL,
 *      AVC_HANDLER_SCART_PIN_8_ASPECT_16_9,
 *      AVC_HANDLER_SCART_PIN_8_ASPECT_4_3
 *  } AVC_HANDLER_CTRL_SCART_PIN_8_STATUS_T;
 *  @endcode
 *
 *  @li@c  AVC_HANDLER_SCART_PIN_8_UNKNOWN     - PIN 8 status of SCART is unknown
 *  @li@c  AVC_HANDLER_SCART_PIN_8_NO_SIGNAL   - PIN 8 status of SCART is no signal
 *  @li@c  AVC_HANDLER_SCART_PIN_8_ASPECT_16_9 - PIN 8 status of SCART is 16:9
 *  @li@c  AVC_HANDLER_SCART_PIN_8_ASPECT_4_3  - PIN 8 status of SCART is 4:3
 */
/*---------------------------------------------------------------------------*/
typedef enum
{
    AVC_HANDLER_SCART_PIN_8_UNKNOWN,
    AVC_HANDLER_SCART_PIN_8_NO_SIGNAL,
    AVC_HANDLER_SCART_PIN_8_ASPECT_16_9,
    AVC_HANDLER_SCART_PIN_8_ASPECT_4_3
}   AVC_HANDLER_CTRL_SCART_PIN_8_STATUS_T;

/*---------------------------------------------------------------------------*/
/*! @enum  AVC_HANDLER_CTRL_SCART_PIN_16_STATUS_T
 *  @brief This structure enumerates all PIN 16 status of SCART and to be used
 *         with AVC_CTRL_GET_SCART_PIN_16_STATUS command
 *
 *  @code
 *  typedef enum
 *  {
 *      AVC_HANDLER_SCART_PIN_16_UNKNOWN,
 *      AVC_HANDLER_SCART_PIN_16_COMPOSITE,
 *      AVC_HANDLER_SCART_PIN_16_RGB,
 *      AVC_HANDLER_SCART_PIN_16_TOGGLE
 *  } AVC_HANDLER_CTRL_SCART_PIN_16_STATUS_T;
 *  @endcode
 *
 *  @li@c  AVC_HANDLER_SCART_PIN_16_UNKNOWN   - PIN 16 status of SCART is unknown
 *  @li@c  AVC_HANDLER_SCART_PIN_16_COMPOSITE - PIN 16 status of SCART is composite
 *  @li@c  AVC_HANDLER_SCART_PIN_16_RGB       - PIN 16 status of SCART is RGB
 *  @li@c  AVC_HANDLER_SCART_PIN_16_TOGGLE    - PIN 16 status of SCART is toggle
 */
/*---------------------------------------------------------------------------*/
typedef enum
{
    AVC_HANDLER_SCART_PIN_16_UNKNOWN,
    AVC_HANDLER_SCART_PIN_16_COMPOSITE,
    AVC_HANDLER_SCART_PIN_16_RGB,
    AVC_HANDLER_SCART_PIN_16_TOGGLE
} AVC_HANDLER_CTRL_SCART_PIN_16_STATUS_T;

#define AVC_HANDLER_SCART_INP_CAP_S_VIDEO      MAKE_BIT_MASK_32(0)    /**< To use with AVC_CTRL_GET_SCART_INP_CAP command - S-Video */
#define AVC_HANDLER_SCART_INP_CAP_COMPOSITE    MAKE_BIT_MASK_32(1)    /**< To use with AVC_CTRL_GET_SCART_INP_CAP command - Composite */
#define AVC_HANDLER_SCART_INP_CAP_RGB          MAKE_BIT_MASK_32(2)    /**< To use with AVC_CTRL_GET_SCART_INP_CAP command - RGB */
#define AVC_HANDLER_SCART_INP_CAP_COMPONENT    MAKE_BIT_MASK_32(3)    /**< To use with AVC_CTRL_GET_SCART_INP_CAP command - Component */

/*---------------------------------------------------------------------------*/
/*! @struct AVC_HANDLER_CTRL_SCART_INP_CAP_T
 *  @brief  This structure contains the input capability of SCART
 *
 *  @code
 *  typedef struct _AVC_HANDLER_CTRL_SCART_INP_CAP_T
 *  {
 *      UINT32    ui4_flags;
 *  } AVC_HANDLER_CTRL_SCART_INP_CAP_T;
 *  @endcode
 *
 *  @li@c   ui4_flags - Flags
 */
/*---------------------------------------------------------------------------*/

typedef struct _AVC_HANDLER_CTRL_SCART_INP_CAP_T
{
    UINT32    ui4_flags;
} AVC_HANDLER_CTRL_SCART_INP_CAP_T;

/*---------------------------------------------------------------------------*/
/*! @enum  AVC_HANDLER_SCART_MODE_T
 *  @brief This structure enumerates all SCART modes and to be used with
 *         AVC_HANDLER_CTRL_SCART_MODE_T command
 *
 *  @code
 *  typedef enum
 *  {
 *      AVC_HANDLER_SCART_MODE_UNKNOWN = 0,
 *      AVC_HANDLER_SCART_MODE_S_VIDEO,
 *      AVC_HANDLER_SCART_MODE_COMPOSITE,
 *      AVC_HANDLER_SCART_MODE_RGB,
 *      AVC_HANDLER_SCART_MODE_MIX,
 *      AVC_HANDLER_SCART_MODE_AUTO,
 *      AVC_HANDLER_SCART_MODE_COMPONENT
 *  } AVC_HANDLER_SCART_MODE_T;
 *  @endcode
 *
 *  @li@c  AVC_HANDLER_SCART_MODE_UNKNOWN   - SCART mode is unknown
 *  @li@c  AVC_HANDLER_SCART_MODE_S_VIDEO   - SCART mode is S-Video
 *  @li@c  AVC_HANDLER_SCART_MODE_COMPOSITE - SCART mode is Composite
 *  @li@c  AVC_HANDLER_SCART_MODE_RGB       - SCART mode is RGB
 *  @li@c  AVC_HANDLER_SCART_MODE_MIX       - SCART mode is MIX
 *  @li@c  AVC_HANDLER_SCART_MODE_AUTO      - SCART mode is auto
 *  @li@c  AVC_HANDLER_SCART_MODE_COMPONENT - SCART mode is component
 */
/*---------------------------------------------------------------------------*/

typedef enum
{
    AVC_HANDLER_SCART_MODE_UNKNOWN = 0,
    AVC_HANDLER_SCART_MODE_S_VIDEO,
    AVC_HANDLER_SCART_MODE_COMPOSITE,
    AVC_HANDLER_SCART_MODE_RGB,
    AVC_HANDLER_SCART_MODE_MIX,
    AVC_HANDLER_SCART_MODE_AUTO,
    AVC_HANDLER_SCART_MODE_COMPONENT
} AVC_HANDLER_SCART_MODE_T;

/*! @} */

#endif /* _U_AVC_HANDLER_H_ */

