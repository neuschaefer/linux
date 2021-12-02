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
/*! @file u_analog_brdcst.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains the Analog Broadcast Connection Handler
 *         specific definitions that are exported.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_CONN_MNGR_ANALOG_BRDCST Analog Brdcst Handler
 *  @ingroup groupMW_CONN_MNGR
 *  @brief Provides a unified interface for digital TV connection
 *  %This module allows an application to create and control digital TV connection
 *   - including these sub modules:
 *  @see groupMW
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_ANALOG_BRDCST_H_
#define _U_ANALOG_BRDCST_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_cm.h"
#include "u_handle.h"
#include "u_tuner.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define ANALOG_BRDCST_HANDLER_DEFAULT_NAME "abrdcst"                               /**< Default name for Analog Brdcst handler                                    */

/* Connection command codes */
#define ANALOG_BRDCST_CONN_OFFSET           ((CM_COMMAND_CODE_T) 0x80020000)       /**< Define the command code offset of analog broadcast handler                */
#define ANALOG_BRDCST_COMMAND(_x)           (ANALOG_BRDCST_CONN_OFFSET + (_x))     /**< Macro for defining Analog Brdcst command code                             */

#define ANALOG_BRDCST_CONN_SVC_LST_ID                 ANALOG_BRDCST_COMMAND(0)     /**< Define the command code for SVL ID                                        */
#define ANALOG_BRDCST_CONN_SVC_LST_NAME               ANALOG_BRDCST_COMMAND(1)     /**< Define the command code for SVL name                                      */
#define ANALOG_BRDCST_CONN_SVL_REC_ID                 ANALOG_BRDCST_COMMAND(2)     /**< Define the command code for SVL record ID                                 */
#define ANALOG_BRDCST_CONN_TS_LST_ID                  ANALOG_BRDCST_COMMAND(3)     /**< Define the command code for TSL ID                                        */
#define ANALOG_BRDCST_CONN_TS_LST_NAME                ANALOG_BRDCST_COMMAND(4)     /**< Define the command code for TSL name                                      */
#define ANALOG_BRDCST_CONN_TSL_REC_ID                 ANALOG_BRDCST_COMMAND(5)     /**< Define the command code for TSL record ID                                 */
#define ANALOG_BRDCST_CONN_DISC_IF_TV_SIG_LOST        ANALOG_BRDCST_COMMAND(6)     /**< Define the command code for disconnect if TV signal is lost               */
#define ANALOG_BRDCST_CONN_DISC_IF_TUNER_SIG_LOST     ANALOG_BRDCST_COMMAND(7)     /**< Define the command code for disconnect if tuner signal is lost            */
#define ANALOG_BRDCST_CONN_DELIVERY_SYS               ANALOG_BRDCST_COMMAND(8)     /**< Define the command code for delivery system                               */
#define ANALOG_BRDCST_CONN_FREQUENCY                  ANALOG_BRDCST_COMMAND(9)     /**< Define the command code for frequency                                     */
#define ANALOG_BRDCST_CONN_TV_DECODER_NAME            ANALOG_BRDCST_COMMAND(10)    /**< Define the command code for TV decoder name                               */
#define ANALOG_BRDCST_CONN_TV_DECODER_COLOR_SYS       ANALOG_BRDCST_COMMAND(11)    /**< Define the command code for TV decoder color system setting               */
#define ANALOG_BRDCST_CONN_TV_SIGNAL_TIME_OUT         ANALOG_BRDCST_COMMAND(12)    /**< Define the command code for TV signal timeout length                      */
#define ANALOG_BRDCST_CONN_DISC_IF_COMP_BUSY          ANALOG_BRDCST_COMMAND(13)    /**< Define the command code for the disconnect condition when component busy  */
#define ANALOG_BRDCST_CONN_USE_SCART                  ANALOG_BRDCST_COMMAND(14)    /**< Define the command code for using scart                                   */
#define ANALOG_BRDCST_CONN_SCART_NAME                 ANALOG_BRDCST_COMMAND(15)    /**< Define the command code for scart name                                    */
#define ANALOG_BRDCST_CONN_SCART_INP_MODE             ANALOG_BRDCST_COMMAND(16)    /**< Define the command code for scart input mode setting                      */
#define ANALOG_BRDCST_CONN_FINE_TUNE                  ANALOG_BRDCST_COMMAND(17)    /**< Define the command code for fine tuning                                   */
#define ANALOG_BRDCST_CONN_DETECT_TV_SYSTEM           ANALOG_BRDCST_COMMAND(18)    /**< Define the command code for detecting TV system                           */
#define ANALOG_BRDCST_CONN_AUDIO_DECODER_NAME         ANALOG_BRDCST_COMMAND(19)    /**< Define the command code for audio decoder name                            */
#define ANALOG_BRDCST_CONN_FREQUENCY_BOUND            ANALOG_BRDCST_COMMAND(20)    /**< Define the command code for frequency bound setting                       */
#define ANALOG_BRDCST_CONN_MODULATION                 ANALOG_BRDCST_COMMAND(21)    /**< Define the command code for modulation setting                            */
#define ANALOG_BRDCST_CONN_TUNER_NO_RIGHTS_OK         ANALOG_BRDCST_COMMAND(22)    /**< Define the command code for tuner no rights ok                            */
#define ANALOG_BRDCST_CONN_SCART_NO_RIGHTS_OK         ANALOG_BRDCST_COMMAND(23)    /**< Define the command code for scart no rights ok                            */
#define ANALOG_BRDCST_CONN_TV_DECODER_NO_RIGHTS_OK    ANALOG_BRDCST_COMMAND(24)    /**< Define the command code for TV decoder no rights ok                       */
#define ANALOG_BRDCST_CONN_AUDIO_DECODER_NO_RIGHTS_OK ANALOG_BRDCST_COMMAND(25)    /**< Define the command code for audio decoder no rights ok                    */
#define ANALOG_BRDCST_CONN_ALL_NO_RIGHTS_OK           ANALOG_BRDCST_COMMAND(26)    /**< Define the command code for all components no rights ok                   */
#define ANALOG_BRDCST_CONN_IGNORE_TSL_UPDATES         ANALOG_BRDCST_COMMAND(27)    /**< Define the command code for ignoring TSL updates                          */
#define ANALOG_BRDCST_CONN_FINE_TUNE_OFFSET           ANALOG_BRDCST_COMMAND(28)    /**< Define the command code for fine tune frequency offset                    */
#define ANALOG_BRDCST_CONN_CENTRAL_FREQUENCY          ANALOG_BRDCST_COMMAND(29)    /**< Define the command code for fine tune central frequency                   */
#define ANALOG_BRDCST_CONN_CH_CHANGE_AUTO_DETECT_AUD_SYS  ANALOG_BRDCST_COMMAND(30)    /**< Define the command code for channel change auto detect aud system     */

/* Broadcast-specific commands to use with x_cm_set/get */
#define ANALOG_BRDCST_SET_GET(_x)  (CM_CTRL_HANDLER_PRIVATE_OFFSET + (_x))         /**< Macro for defining Analog Brdcst set & get code                   */

#define ANALOG_BRDCST_CTRL_GET_SIGNAL_LEVEL           ANALOG_BRDCST_SET_GET(1)     /**< Define the command code to get signal level                       */
#define ANALOG_BRDCST_CTRL_GET_DBM_SIGNAL_LEVEL       ANALOG_BRDCST_SET_GET(2)     /**< Define the command code to get DBM signal level                   */
#define ANALOG_BRDCST_CTRL_GET_CAB_TUNE_INFO          ANALOG_BRDCST_SET_GET(3)     /**< Define the command code to get cable tuner info                   */
#define ANALOG_BRDCST_CTRL_GET_SAT_TUNE_INFO          ANALOG_BRDCST_SET_GET(4)     /**< Define the command code to get satellite tuner info               */
#define ANALOG_BRDCST_CTRL_GET_TER_TUNE_INFO          ANALOG_BRDCST_SET_GET(5)     /**< Define the command code to get terrestrial tuner info             */
#define ANALOG_BRDCST_CTRL_GET_SCART_PIN_8_STATUS     ANALOG_BRDCST_SET_GET(6)     /**< Define the command code to get scart pin 8 status                 */
#define ANALOG_BRDCST_CTRL_GET_SCART_PIN_16_STATUS    ANALOG_BRDCST_SET_GET(7)     /**< Define the command code to get scart pin 16 status                */
#define ANALOG_BRDCST_CTRL_GET_SCART_INP_CAP          ANALOG_BRDCST_SET_GET(8)     /**< Define the command code to get scart input capability             */
#define ANALOG_BRDCST_CTRL_GET_SCART_BYPASS           ANALOG_BRDCST_SET_GET(9)     /**< Define the command code to get scart bypass mode                  */
#define ANALOG_BRDCST_CTRL_GET_SCART_INP_MODE         ANALOG_BRDCST_SET_GET(10)    /**< Define the command code to get scart input mode                   */
#define ANALOG_BRDCST_CTRL_GET_TV_AUD_SYS_MASK        ANALOG_BRDCST_SET_GET(11)    /**< Define the command code to get TV system and Audio system mask    */
#define ANALOG_BRDCST_CTRL_GET_TV_DEC_COLOR_SYS       ANALOG_BRDCST_SET_GET(12)    /**< Define the command code to get TV decoder color system            */
#define ANALOG_BRDCST_CTRL_GET_AGC                    ANALOG_BRDCST_SET_GET(13)    /**< Define the command code to get AGC                                */
#define ANALOG_BRDCST_CTRL_GET_DBM_SNR                ANALOG_BRDCST_SET_GET(14)    /**< Define the command code to get DBM Signal-to-noise ratio          */
#define ANALOG_BRDCST_CTRL_GET_KHZ_CFO                ANALOG_BRDCST_SET_GET(15)    /**< Define the command code to get KHZ CFO                            */
#define ANALOG_BRDCST_CTRL_GET_TUNER_NEXT_STEP_SIZE   ANALOG_BRDCST_SET_GET(16)    /**< Define the command code to get tuner next step size               */
#define ANALOG_BRDCST_CTRL_GET_RFAGC                  ANALOG_BRDCST_SET_GET(17)    /**< Define the command code to get RFAGC                              */

#define ANALOG_BRDCST_CTRL_SET_TUNE_ATTRIBUTES      ANALOG_BRDCST_SET_GET(101)     /**< Define the command code to set tuner attribute                    */
#define ANALOG_BRDCST_CTRL_SET_SCART_BYPASS         ANALOG_BRDCST_SET_GET(102)     /**< Define the command code to set scart bypass                       */
#define ANALOG_BRDCST_CTRL_SET_SCART_INP_MODE       ANALOG_BRDCST_SET_GET(103)     /**< Define the command code to set scart input mode                   */
#define ANALOG_BRDCST_CTRL_SET_DIAGNOSTICS          ANALOG_BRDCST_SET_GET(104)     /**< Define the command code to set diagnostics mode                   */
#define ANALOG_BRDCST_CTRL_SET_AUTO_SEARCH_MODE     ANALOG_BRDCST_SET_GET(105)     /**< Define the command code to set auto search mode for drivers       */
#define ANALOG_BRDCST_CTRL_SET_TUNER_FREQ_AUTO_NFY   ANALOG_BRDCST_SET_GET(106)     /**< Define the command code to set frequency notification function    */

/* Disconnection reasons */
#define ANALOG_BRDCST_DISC_AS_REQUESTED                             0U    /**< Disconnect reason of user request                            */
#define ANALOG_BRDCST_DISC_OTHER_REASON                             1U    /**< Disconnect reason of other reason                            */
#define ANALOG_BRDCST_DISC_NEW_CONNECTION                           2U    /**< Disconnect reason of new connection                          */
#define ANALOG_BRDCST_DISC_NO_TUNER_FOUND                           3U    /**< Disconnect reason of no tuner found                          */
#define ANALOG_BRDCST_DISC_NO_TV_DECODER_FOUND                      4U    /**< Disconnect reason of no tv decoder found                     */
#define ANALOG_BRDCST_DISC_COMP_BUSY                                5U    /**< Disconnect reason of component busy                          */
#define ANALOG_BRDCST_DISC_COULD_NOT_SET_TUNER_PROPERTIES           6U    /**< Disconnect reason of could not set tuner properties          */
#define ANALOG_BRDCST_DISC_COULD_NOT_SET_TV_DECODER_PROPERTIES      7U    /**< Disconnect reason of could not set tv decoder properties     */
#define ANALOG_BRDCST_DISC_COULD_NOT_CONNECT_TUNER                  8U    /**< Disconnect reason of could not connect tuner                 */
#define ANALOG_BRDCST_DISC_COULD_NOT_CONNECT_TV_DECODER             9U    /**< Disconnect reason of could not connect tv decoder            */
#define ANALOG_BRDCST_DISC_SVL_ERROR                               10U    /**< Disconnect reason of SVL error                               */
#define ANALOG_BRDCST_DISC_SVL_CLOSED                              11U    /**< Disconnect reason of SVL closed                              */
#define ANALOG_BRDCST_DISC_TSL_CLOSED                              12U    /**< Disconnect reason of TSL closed                              */
#define ANALOG_BRDCST_DISC_TUNER_SIGNAL_LOST                       13U    /**< Disconnect reason of tuner signal loss                       */
#define ANALOG_BRDCST_DISC_TUNER_UNKNOWN_LNB                       14U    /**< Disconnect reason of tuner LNB unknown                       */
#define ANALOG_BRDCST_DISC_TUNER_OUT_OF_RANGE                      15U    /**< Disconnect reason of tuenr frequency out of range            */
#define ANALOG_BRDCST_DISC_TV_SIGNAL_TIMER_EXPIRED                 16U    /**< Disconnect reason of tv signal timer expired                 */
#define ANALOG_BRDCST_DISC_TV_SIGNAL_LOST                          17U    /**< Disconnect reason of tv signal lost                          */
#define ANALOG_BRDCST_DISC_COULD_NOT_CONNECT_SCART                 18U    /**< Disconnect reason of could not connect cart                  */
#define ANALOG_BRDCST_DISC_NO_SCART_FOUND                          19U    /**< Disconnect reason of no scart component found                */
#define ANALOG_BRDCST_DISC_NO_AUDIO_DECODER_FOUND                  20U    /**< Disconnect reason of no audio decoder found                  */
#define ANALOG_BRDCST_DISC_COULD_NOT_SET_AUDIO_DECODER_PROPERTIES  21U    /**< Disconnect reason of could not set audio decoder properties  */
#define ANALOG_BRDCST_DISC_COULD_NOT_CONNECT_AUDIO_DECODER         22U    /**< Disconnect reason of could not connect audio decoder         */
#define ANALOG_BRDCST_DISC_COULD_NOT_STOP_TV_DECODER               23U    /**< Disconnect reason of could not stop tv decoder               */
#define ANALOG_BRDCST_DISC_COULD_NOT_STOP_AUDIO_DECODER            24U    /**< Disconnect reason of could not stop audio decoder            */


/* Time-out values */
/*------------------------------------------------------------------*/
/*! @struct ANALOG_BRDCST_TIME_OUT_T
 *  @brief  For setting TV signal time out value
 *  @code
 *  typedef struct
 *  {
 *      UINT32  ui4_tv_signal_time_out;
 *  }   ANALOG_BRDCST_TIME_OUT_T;
 *  @endcode
 *  @li@c  ui4_tv_signal_time_out    - TV signal detection timeout length in millisecond
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT32  ui4_tv_signal_time_out;
}   ANALOG_BRDCST_TIME_OUT_T;

/* To use with ANALOG_BRDCST_CTRL_GET_SCART_PIN_8_STATUS command */
/*------------------------------------------------------------------*/
/*! @enum  ANALOG_BRDCST_CTRL_SCART_PIN_8_STATUS_T
 *  @brief Structure for indicating scart pin 8 status
 *  @code
 *  typedef enum
 *  {
 *      ANALOG_BRDCST_SCART_PIN_8_UNKNOWN,
 *      ANALOG_BRDCST_SCART_PIN_8_NO_SIGNAL,
 *      ANALOG_BRDCST_SCART_PIN_8_ASPECT_16_9,
 *      ANALOG_BRDCST_SCART_PIN_8_ASPECT_4_3
 *  }   ANALOG_BRDCST_CTRL_SCART_PIN_8_STATUS_T;
 *  @endcode
 *  @li@c  ANALOG_BRDCST_SCART_PIN_8_UNKNOWN                 - Pin 8 status unknown
 *  @li@c  ANALOG_BRDCST_SCART_PIN_8_NO_SIGNAL               - Pin 8 indicates no signal
 *  @li@c  ANALOG_BRDCST_SCART_PIN_8_ASPECT_16_9             - Pin 8 indicates aspect ratio is 16:9
 *  @li@c  ANALOG_BRDCST_SCART_PIN_8_ASPECT_4_3              - Pin 8 indicates aspect ratio is 4:3
 */
/*------------------------------------------------------------------*/
typedef enum
{
    ANALOG_BRDCST_SCART_PIN_8_UNKNOWN,
    ANALOG_BRDCST_SCART_PIN_8_NO_SIGNAL,
    ANALOG_BRDCST_SCART_PIN_8_ASPECT_16_9,
    ANALOG_BRDCST_SCART_PIN_8_ASPECT_4_3
}   ANALOG_BRDCST_CTRL_SCART_PIN_8_STATUS_T;

/* To use with ANALOG_BRDCST_CTRL_GET_SCART_PIN_16_STATUS command */
/*------------------------------------------------------------------*/
/*! @enum  ANALOG_BRDCST_CTRL_SCART_PIN_16_STATUS_T
 *  @brief Structure for indicating scart pin 16 status
 *  @code
 *  typedef enum
 *  {
 *      ANALOG_BRDCST_SCART_PIN_16_UNKNOWN,
 *      ANALOG_BRDCST_SCART_PIN_16_COMPOSITE,
 *      ANALOG_BRDCST_SCART_PIN_16_RGB,
 *      ANALOG_BRDCST_SCART_PIN_16_TOGGLE
 *  }   ANALOG_BRDCST_CTRL_SCART_PIN_16_STATUS_T;
 *  @endcode
 *  @li@c  ANALOG_BRDCST_SCART_PIN_16_UNKNOWN                - Pin 16 status unknown
 *  @li@c  ANALOG_BRDCST_SCART_PIN_16_COMPOSITE              - Pin 16 indicates signal type is composite
 *  @li@c  ANALOG_BRDCST_SCART_PIN_16_RGB                    - Pin 16 indicates signal type is RGB
 *  @li@c  ANALOG_BRDCST_SCART_PIN_16_TOGGLE                 - Pin 16 indicates signal is toggling
 */
/*------------------------------------------------------------------*/
typedef enum
{
    ANALOG_BRDCST_SCART_PIN_16_UNKNOWN,
    ANALOG_BRDCST_SCART_PIN_16_COMPOSITE,
    ANALOG_BRDCST_SCART_PIN_16_RGB,
    ANALOG_BRDCST_SCART_PIN_16_TOGGLE
}   ANALOG_BRDCST_CTRL_SCART_PIN_16_STATUS_T;

/* To use with ANALOG_BRDCST_CTRL_GET_SCART_INP_CAP command */
#define ANALOG_BRDCST_SCART_INP_CAP_S_VIDEO    MAKE_BIT_MASK_32(0)    /**< Scart input supports S-Video signal        */
#define ANALOG_BRDCST_SCART_INP_CAP_COMPOSITE  MAKE_BIT_MASK_32(1)    /**< Scart input supports composite signal      */
#define ANALOG_BRDCST_SCART_INP_CAP_RGB        MAKE_BIT_MASK_32(2)    /**< Scart input supports RGB signal            */
#define ANALOG_BRDCST_SCART_INP_CAP_COMPONENT  MAKE_BIT_MASK_32(3)    /**< Scart input supports component signal      */

/*------------------------------------------------------------------*/
/*! @struct ANALOG_BRDCST_CTRL_SCART_INP_CAP_T
 *  @brief  Structure for getting scart input capability
 *  @code
 *  typedef struct
 *  {
 *      UINT32  ui4_flags;
 *  }   ANALOG_BRDCST_CTRL_SCART_INP_CAP_T;
 *  @endcode
 *  @li@c  ui4_flags         - Indicates signal types support by scart input.
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT32  ui4_flags;
}   ANALOG_BRDCST_CTRL_SCART_INP_CAP_T;

/* To use with ANALOG_BRDCST_CTRL_GET/SET_SCART_BYPASS commands */
/*------------------------------------------------------------------*/
/*! @struct ANALOG_BRDCST_CTRL_SCART_BYPASS_T
 *  @brief  Structure for indicating if scart bypass is enabled.
 *  @code
 *  typedef struct
 *  {
 *      BOOL  b_bypass;
 *  }   ANALOG_BRDCST_CTRL_SCART_BYPASS_T;
 *  @endcode
 *  @li@c  b_bypass   - Indicates if scart bypass is enabeld or not.
 */
/*------------------------------------------------------------------*/
typedef struct
{
    BOOL  b_bypass;
}   ANALOG_BRDCST_CTRL_SCART_BYPASS_T;

/* To use with ANALOG_BRDCST_CTRL_GET/SET_SCART_INP_MODE command */
/*------------------------------------------------------------------*/
/*! @enum  ANALOG_BRDCST_SCART_MODE_T
 *  @brief Enumeration of Scart input modes.
 *  @code
 *  typedef enum
 *  {
 *      ANALOG_BRDCST_SCART_MODE_UNKNOWN = 0,
 *      ANALOG_BRDCST_SCART_MODE_S_VIDEO,
 *      ANALOG_BRDCST_SCART_MODE_COMPOSITE,
 *      ANALOG_BRDCST_SCART_MODE_RGB,
 *      ANALOG_BRDCST_SCART_MODE_MIX,
 *      ANALOG_BRDCST_SCART_MODE_AUTO,
 *      ANALOG_BRDCST_SCART_MODE_COMPONENT
 *  }   ANALOG_BRDCST_SCART_MODE_T;
 *  @endcode
 *  @li@c  ANALOG_BRDCST_SCART_MODE_UNKNOWN                  - Scart input mode is unknown.
 *  @li@c  ANALOG_BRDCST_SCART_MODE_S_VIDEO                  - Scart input mode is S-Video
 *  @li@c  ANALOG_BRDCST_SCART_MODE_COMPOSITE                - Scart input mode is composite
 *  @li@c  ANALOG_BRDCST_SCART_MODE_RGB                      - Scart input mode is RGB
 *  @li@c  ANALOG_BRDCST_SCART_MODE_MIX                      - Scart input mode is mixed (input source is under TV)
 *  @li@c  ANALOG_BRDCST_SCART_MODE_AUTO                     - Scart input mode is auto
 *  @li@c  ANALOG_BRDCST_SCART_MODE_COMPONENT                - Scart input mode is component
 */
/*------------------------------------------------------------------*/
typedef enum
{
    ANALOG_BRDCST_SCART_MODE_UNKNOWN = 0,
    ANALOG_BRDCST_SCART_MODE_S_VIDEO,
    ANALOG_BRDCST_SCART_MODE_COMPOSITE,
    ANALOG_BRDCST_SCART_MODE_RGB,
    ANALOG_BRDCST_SCART_MODE_MIX,
    ANALOG_BRDCST_SCART_MODE_AUTO,
    ANALOG_BRDCST_SCART_MODE_COMPONENT
}   ANALOG_BRDCST_SCART_MODE_T;

/* To use with ANALOG_BRDCST_CTRL_GET_AUDIO_TV_SYS_MASK command */
/*------------------------------------------------------------------*/
/*! @struct ANALOG_BRDCST_CTRL_TV_AUD_SYS_MASK_T
 *  @brief  Structure contains TV system mask, and audio system mask
 *  @code
 *  typedef struct
 *  {
 *      UINT32  ui4_tv_sys_mask;
 *      UINT32  ui4_aud_sys_mask;
 *  }   ANALOG_BRDCST_CTRL_TV_AUD_SYS_MASK_T;
 *  @endcode
 *  @li@c  ui4_tv_sys_mask       - TV system mask
 *  @li@c  ui4_aud_sys_mask      - Audio system mask
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT32  ui4_tv_sys_mask;
    UINT32  ui4_aud_sys_mask;
}   ANALOG_BRDCST_CTRL_TV_AUD_SYS_MASK_T;

/* To use with ANALOG_BRDCST_CTRL_SET_TUNE_ATTRIBUTES command */
#define ANALOG_BRDCST_TUNE_ATTRIBUTES_TV_SYS              ((UINT32) 0x01)    /**< Set TV system tune attribute          */
#define ANALOG_BRDCST_TUNE_ATTRIBUTES_AUDIO_SYS           ((UINT32) 0x02)    /**< Set audio system tune attribute       */
#define ANALOG_BRDCST_TUNE_ATTRIBUTES_FREQUENCY           ((UINT32) 0x04)    /**< Set frequency tune attribute          */
#define ANALOG_BRDCST_TUNE_ATTRIBUTES_FREQUENCY_BOUND     ((UINT32) 0x08)    /**< Set frequency bound tune attribute    */
#define ANALOG_BRDCST_TUNE_ATTRIBUTES_MODULATION          ((UINT32) 0x10)    /**< Set modulation tune attribute         */
#define ANALOG_BRDCST_TUNE_ATTRIBUTES_FINE_TUNE           ((UINT32) 0x20)    /**< Set fine tune attribute               */
#define ANALOG_BRDCST_TUNE_ATTRIBUTES_IGNORE_TSL_UPDATES  ((UINT32) 0x40)    /**< Set ignore TSL update tune attribute  */
#define ANALOG_BRDCST_TUNE_ATTRIBUTES_CENTRAL_FREQUENCY   ((UINT32) 0x80)    /**< Set central frequency tune attribute  */

/*------------------------------------------------------------------*/
/*! @struct ANALOG_BRDCST_CTRL_TUNE_ATTRIBUTES_T
 *  @brief  Structure used to indicate tune attributes.
 *  @code
 *  typedef struct
 *  {
 *      UINT32              ui4_flags;
 *      UINT32              ui4_tv_sys_mask;
 *      UINT32              ui4_aud_sys_mask;
 *      UINT32              ui4_freq;
 *      UINT32              ui4_freq_bound;
 *      TUNER_MODULATION_T  e_mod;
 *      BOOL                b_fine_tune;
 *      BOOL                b_ignore_tsl_updates;
 *      INT16               i2_fine_tune_offset;
 *  }   ANALOG_BRDCST_CTRL_TUNE_ATTRIBUTES_T;
 *  @endcode
 *  @li@c  ui4_flags                                         - Flags to indicate which attributes are used
 *  @li@c  ui4_tv_sys_mask                                   - TV system mask attribute
 *  @li@c  ui4_aud_sys_mask                                  - Audio system mask attribute
 *  @li@c  ui4_freq                                          - Frequency attribute
 *  @li@c  ui4_freq_bound                                    - Frequency bound attribute
 *  @li@c  e_mod                                             - Tuner modulation mode attribute
 *  @li@c  b_fine_tune                                       - Fine tune enabled or not attribute
 *  @li@c  b_ignore_tsl_updates                              - Ignore TSl updates enabled or not attribute
 *  @li@c  i2_fine_tune_offset                               - Fine tune offset attribute
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT32              ui4_flags;
    UINT32              ui4_tv_sys_mask;
    UINT32              ui4_aud_sys_mask;
    UINT32              ui4_freq;
    UINT32              ui4_freq_bound;
    UINT32              ui4_central_freq;
    TUNER_MODULATION_T  e_mod;
    BOOL                b_fine_tune;
    BOOL                b_ignore_tsl_updates;
    INT16               i2_fine_tune_offset;
}   ANALOG_BRDCST_CTRL_TUNE_ATTRIBUTES_T;

#endif /*  _U_ANALOG_BRDCST_H_ */

/*----------------------------------------------------------------------------------*/
/*! @} */                                 /* end of groupMW_CONN_MNGR_ANALOG_BRDCST */
/*----------------------------------------------------------------------------------*/

