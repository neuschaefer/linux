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
/*! @file u_fm_radio.h
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *
 *  @par Description:
 *       This header file contains the FM Radio Connection Handler specific
 *       definitions that are exported.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_CONN_MNGR_FM_RADIO FM Radio
 *  @ingroup  groupMW_CONN_MNGR
 *  @brief    Provides a unified interface for FM radio connection
 *  @see      groupMW_CONN_MNGR
 *
 *  @{
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_FM_RADIO_H_
#define _U_FM_RADIO_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_cm.h"
#include "u_handle.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define FM_RADIO_HANDLER_DEFAULT_NAME                               "fm_radio"    /**< Default name of FM radio handler */

/* Connection command codes */
#define FM_RADIO_CONN_OFFSET                   ((CM_COMMAND_CODE_T) 0x80050000)   /**< Command codes offset */
#define FM_RADIO_COMMAND(_x)                      (FM_RADIO_CONN_OFFSET + (_x))

#define FM_RADIO_CONN_SVC_LST_ID                            FM_RADIO_COMMAND(0)   /**< Connection command codes - Service list ID */
#define FM_RADIO_CONN_SVC_LST_NAME                          FM_RADIO_COMMAND(1)   /**< Connection command codes - Service list name */
#define FM_RADIO_CONN_SVL_REC_ID                            FM_RADIO_COMMAND(2)   /**< Connection command codes - SVL record ID */
#define FM_RADIO_CONN_DELIVERY_SYS                          FM_RADIO_COMMAND(3)   /**< Connection command codes - Delivery system */
#define FM_RADIO_CONN_FREQUENCY                             FM_RADIO_COMMAND(4)   /**< Connection command codes - Frequency */
#define FM_RADIO_CONN_FREQUENCY_BOUND                       FM_RADIO_COMMAND(5)   /**< Connection command codes - Frequency bound */


#define FM_RADIO_HANDLER_OFFSET                             ((CM_CTRL_TYPE_T) 0x03000000) 
#define FM_RADIO_SET_GET(_x)            (CM_CTRL_HANDLER_PRIVATE_OFFSET + FM_RADIO_HANDLER_OFFSET + (_x))   /**< FM Radio specific commands to use with x_cm_set/get */

#define FM_RADIO_CTRL_SET_TUNE_FREQ                       FM_RADIO_SET_GET(101)   /**< Set commands - Set tuning frequency */
#define FM_RADIO_CTRL_SET_DET_CHANNEL                     FM_RADIO_SET_GET(102)   /**< Set commands - Set to detect channel */

#define FM_RADIO_DISC_AS_REQUESTED                                         0U     /**< Disconnection reasons - As requested */
#define FM_RADIO_DISC_OTHER_REASON                                         1U     /**< Disconnection reasons - Other reason */
#define FM_RADIO_DISC_NEW_CONNECTION                                       2U     /**< Disconnection reasons - New connection */
#define FM_RADIO_DISC_NO_TUNER_FOUND                                       3U     /**< Disconnection reasons - No tuner is found */
#define FM_RADIO_DISC_COMP_BUSY                                            4U     /**< Disconnection reasons - Components are busy */
#define FM_RADIO_DISC_COULD_NOT_SET_TUNER_PROPERTIES                       5U     /**< Disconnection reasons - Could not set tuner properties*/
#define FM_RADIO_DISC_COULD_NOT_CONNECT_TUNER                              6U     /**< Disconnection reasons - Could not coonect to tuner */
#define FM_RADIO_DISC_SVL_ERROR                                            7U     /**< Disconnection reasons - SVL error */
#define FM_RADIO_DISC_SVL_CLOSED                                           8U     /**< Disconnection reasons - SVL closed */
#define FM_RADIO_DISC_TUNER_SIGNAL_LOST                                    9U     /**< Disconnection reasons - Tuner signal lost */
#define FM_RADIO_DISC_TUNER_UNKNOWN_LNB                                   10U     /**< Disconnection reasons - Tuner unknow LNB */
#define FM_RADIO_DISC_TUNER_OUT_OF_RANGE                                  11U     /**< Disconnection reasons - Tuner is out of range */
#define FM_RADIO_DISC_NO_AUDIO_DECODER_FOUND                              12U     /**< Disconnection reasons - No audio decoder found */
#define FM_RADIO_DISC_COULD_NOT_SET_AUDIO_DECODER_PROPERTIES              13U     /**< Disconnection reasons - Cound not set audio decoder properties */
#define FM_RADIO_DISC_COULD_NOT_CONNECT_AUDIO_DECODER                     14U     /**< Disconnection reasons - Count not connect audio decoder */
#define FM_RADIO_DISC_COULD_NOT_STOP_AUDIO_DECODER                        15U     /**< Disconnection reasons - Count not stop audio decoder */

/*---------------------------------------------------------------------------*/
/*! @struct FM_RADIO_DETECT_DONE_T
 *  @brief  This structure contains the information when detection is done
 *
 *  @code
 *  typedef struct
 *  {
 *      BOOL      b_detected;
 *      UINT32    ui4_freq;
 *      UINT8     ui1_sgl_lvl;
 *  } FM_RADIO_DETECT_DONE_T;
 *  @endcode
 *
 *  @li@c   b_detected  - Detectd (TRUE or FALSE)
 *  @li@c   ui4_freq    - Frequency
 *  @li@c   ui1_sgl_lvl - Signal level
 */
/*---------------------------------------------------------------------------*/
typedef struct
{
    BOOL      b_detected;
    UINT32    ui4_freq;
    UINT8     ui1_sgl_lvl;
} FM_RADIO_DETECT_DONE_T;

#define FM_RADIO_CONN_INFO_DETECT_DONE                       CM_HANDLER_COND(1)    /**< FM radio handler's own conditions (CM_COND_T) - Detection is done */

/*! @} */

#endif /*  _U_FM_RADIO_H_ */

