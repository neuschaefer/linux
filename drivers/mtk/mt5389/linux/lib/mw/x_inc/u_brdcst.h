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
/*! @file u_brdcst.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains the Broadcast Connection Handler specific
 *         definitions that are exported.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_CONN_MNGR_BRDCST Broadcasting Handler
 *  @ingroup groupMW_CONN_MNGR
 *  @brief Provides a unified interface for digital TV connection
 *  %This module allows an application to create and control digital TV connection
 *   - including these sub modules:
 *  @see groupMW
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_BRDCST_H_
#define _U_BRDCST_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_cm.h"
#include "u_handle.h"
#include "u_tuner.h"
#include "u_scdb.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define BRDCST_HANDLER_DEFAULT_NAME "brdcst"    /**< Define the default name of broadcast handler       */

/* Connection command codes */
#define BRDCST_CONN_OFFSET                  ((CM_COMMAND_CODE_T) 0x80000000)    /**< Define the command code offset of broadcast handler                        */
#define BRDCST_CONN_SVC_LST_ID              (BRDCST_CONN_OFFSET +  0)           /**< Define the command code for SVL ID                                         */
#define BRDCST_CONN_SVC_LST_NAME            (BRDCST_CONN_OFFSET +  1)           /**< Define the command code for SVL name                                       */
#define BRDCST_CONN_SVL_REC_ID              (BRDCST_CONN_OFFSET +  2)           /**< Define the command code for SVL record ID                                  */
#define BRDCST_CONN_SVC_ID                  (BRDCST_CONN_OFFSET +  3)           /**< Define the command code for service ID                                     */
#define BRDCST_CONN_TS_LST_ID               (BRDCST_CONN_OFFSET +  4)           /**< Define the command code for TSL ID                                         */
#define BRDCST_CONN_TS_LST_NAME             (BRDCST_CONN_OFFSET +  5)           /**< Define the command code for TSL name                                       */
#define BRDCST_CONN_TSL_REC_ID              (BRDCST_CONN_OFFSET +  6)           /**< Define the command code for TSL record ID                                  */
//#ifdef MW_DVBS_DEV_ENABLE
#define BRDCST_CONN_SAT_LST_ID              (BRDCST_CONN_OFFSET +  7)           /**< Define the command code for TSL record ID                                  */
#define BRDCST_CONN_SAT_LST_NAME            (BRDCST_CONN_OFFSET +  8)           /**< Define the command code for TSL record ID                                  */
#define BRDCST_CONN_SATL_REC_ID             (BRDCST_CONN_OFFSET +  9)           /**< Define the command code for TSL record ID                                  */
//#endif
#define BRDCST_CONN_SIGNAL_LOST_DISCONNECT  (BRDCST_CONN_OFFSET + 10)           /**< Define the command code for disconnect condition when signal lose          */
#define BRDCST_CONN_DELIVERY_SYS            (BRDCST_CONN_OFFSET + 11)           /**< Define the command code for delivery system                                */
#define BRDCST_CONN_FREQUENCY               (BRDCST_CONN_OFFSET + 12)           /**< Define the command code for frequency                                      */
#define BRDCST_CONN_SYM_RATE                (BRDCST_CONN_OFFSET + 13)           /**< Define the command code for symbol rate                                    */
#define BRDCST_CONN_FEC_OUTER               (BRDCST_CONN_OFFSET + 14)           /**< Define the command code for FEC outer                                      */
#define BRDCST_CONN_FEC_INNER               (BRDCST_CONN_OFFSET + 15)           /**< Define the command code for FEC inner                                      */
#define BRDCST_CONN_MODULATION              (BRDCST_CONN_OFFSET + 16)           /**< Define the command code for modulation                                     */
#define BRDCST_CONN_ORBITAL_POSITION        (BRDCST_CONN_OFFSET + 17)           /**< Define the command code for orbital position                               */
#define BRDCST_CONN_POLARIZATION            (BRDCST_CONN_OFFSET + 18)           /**< Define the command code for polarization                                   */
#define BRDCST_CONN_BANDWIDTH               (BRDCST_CONN_OFFSET + 19)           /**< Define the command code for bandwidth                                      */
#define BRDCST_CONN_HIERARCHY               (BRDCST_CONN_OFFSET + 20)           /**< Define the command code for hierarchy                                      */
//#ifdef MW_DVBS_DEV_ENABLE
#define BRDCST_CONN_22K                     (BRDCST_CONN_OFFSET + 21)
#define BRDCST_CONN_TONE_BURST              (BRDCST_CONN_OFFSET + 22)
#define BRDCST_CONN_PORT                    (BRDCST_CONN_OFFSET + 23)
//#endif
#define BRDCST_CONN_CODE_RATE_HP            (BRDCST_CONN_OFFSET + 24)           /**< Define the command code for code rate HP                                   */
#define BRDCST_CONN_CODE_RATE_LP            (BRDCST_CONN_OFFSET + 25)           /**< Define the command code for code rate LP                                   */
#define BRDCST_CONN_GUARD_INTERVAL          (BRDCST_CONN_OFFSET + 26)           /**< Define the command code for guard interval                                 */
#define BRDCST_CONN_TRANS_MODE              (BRDCST_CONN_OFFSET + 27)           /**< Define the command code for trans mode                                     */
#define BRDCST_CONN_OTHER_FREQ_FLAG         (BRDCST_CONN_OFFSET + 28)           /**< Define the command code for other freq flag                                */
#define BRDCST_CONN_PAT_TIME_OUT            (BRDCST_CONN_OFFSET + 29)           /**< Define the command code for the time out of PAT table                      */
#define BRDCST_CONN_PMT_TIME_OUT            (BRDCST_CONN_OFFSET + 30)           /**< Define the command code for the time out of PMT table                      */
#define BRDCST_CONN_MGT_TIME_OUT            (BRDCST_CONN_OFFSET + 31)           /**< Define the command code for the time out of MGT table                      */
#define BRDCST_CONN_CVCT_TIME_OUT           (BRDCST_CONN_OFFSET + 32)           /**< Define the command code for the time out of CVCT table                     */
#define BRDCST_CONN_TVCT_TIME_OUT           (BRDCST_CONN_OFFSET + 33)           /**< Define the command code for the time out of TVCT table                     */
#define BRDCST_CONN_EIT_TIME_OUT            (BRDCST_CONN_OFFSET + 34)           /**< Define the command code for the time out of EIT table                      */
#define BRDCST_CONN_EIT_RETRY_DELAY         (BRDCST_CONN_OFFSET + 35)           /**< Define the command code for the retry delay of EIT table                   */
#define BRDCST_CONN_DISC_IF_COMP_BUSY       (BRDCST_CONN_OFFSET + 36)           /**< Define the command code for the disconnect condition when component busy   */
#define BRDCST_CONN_DISC_SUB_CONNECTIONS    (BRDCST_CONN_OFFSET + 37)           /**< Define the command code for the disconnect condition of sub connections    */
#define BRDCST_CONN_DISC_IF_NO_SERVICE      (BRDCST_CONN_OFFSET + 38)           /**< Define the command code for the disconnect condition when no service       */
#define BRDCST_CONN_HIERARCHY_PRIORITY      (BRDCST_CONN_OFFSET + 39)           /**< Define the command code for hierarchy priority                             */
#define BRDCST_CONN_REUSE_CONNECTION        (BRDCST_CONN_OFFSET + 40)           /**< Define the command code for reuse connection                               */
#define BRDCST_CONN_TUNER_CONN_MODE         (BRDCST_CONN_OFFSET + 41)           /**< Define the command code for tuner connection mode                          */
#define BRDCST_CONN_TUNER_NO_RIGHTS_OK      (BRDCST_CONN_OFFSET + 42)           /**< Define the command code for tuner no rights ok                             */
#define BRDCST_CONN_2_LANGUAGES_1_STREAM    (BRDCST_CONN_OFFSET + 43)           /**< Define the command code for 2 audio languages in one stream                */
#define BRDCST_CONN_N_LANGUAGES_1_STREAM    (BRDCST_CONN_OFFSET + 44)           /**< Define the command code for N audio languages in one stream                */
#define BRDCST_CONN_DUAL_MONO_FAKE_STEREO   (BRDCST_CONN_OFFSET + 45)           /**< Define the command code for dual mono fake stereo                          */
#define BRDCST_CONN_DUAL_MONO_MIXED_STEREO  (BRDCST_CONN_OFFSET + 46)           /**< Define the command code for mixed A+B                                   */
/* Riks-TV requirement - Begin */
#define BRDCST_CONN_ALTER_FREQ_TUNE         (BRDCST_CONN_OFFSET + 47)           /**< Define the command code for tune alternate frequency                       */
/* Riks-TV requirement - End */
//#ifdef MW_DVBS_DEV_ENABLE
#define BRDCST_CONN_DISEQ_TYPE              (BRDCST_CONN_OFFSET + 48)
#define BRDCST_CONN_CASCADE_TYPE            (BRDCST_CONN_OFFSET + 49)
#define BRDCST_CONN_CASCADE_PORT            (BRDCST_CONN_OFFSET + 50)
//#endif
#define BRDCST_CONN_DVB_T_T2_MODE           (BRDCST_CONN_OFFSET + 51)
#define BRDCST_CONN_DISC_IF_TSL_UPDATE      (BRDCST_CONN_OFFSET + 52)           /**< Define the command code tsl update                       */
#define BRDCST_CONN_DVBT2_PLP_ID            (BRDCST_CONN_OFFSET + 53)

#define BRDCST_CONN_SUBSTREAM_FAKE_STEREO   (BRDCST_CONN_OFFSET + 54)           /**< Define the command code for E-AC3 substream  fake stereo   */
#define BRDCST_CONN_AUDIO_AD_TO_INDEPENDENT_AD      (BRDCST_CONN_OFFSET + 55)           /* Define the command code ad to nomal, used in finand for pre-mixed audio*/
#define BRDCST_CONN_START_FREQ              (BRDCST_CONN_OFFSET + 56)
#define BRDCST_CONN_END_FREQ				(BRDCST_CONN_OFFSET + 57)

#define BRDCST_CONN_WITHOUT_SCDB            (BRDCST_CONN_OFFSET + 58)           /**< Define the command code tsl update                       */
#define BRDCST_CONN_BE_PRE_CONN             (BRDCST_CONN_OFFSET + 59)           /**< Define the command code tsl update                       */

#define BRDCST_CONN_PROGRAM_NUMBER          BRDCST_CONN_SVC_ID                  /**< Define the command code for service ID (Program ID)                        */

#define BRDCST_HANDLER_OFFSET               ((CM_CTRL_TYPE_T) 0x01000000) 
#define BRDCST_HANDLER_SET_GET(_x)          (CM_CTRL_HANDLER_PRIVATE_OFFSET + BRDCST_HANDLER_OFFSET + (_x))   /**< brdcst handler specific commands to use with x_cm_set/get */

/* Broadcast-specific commands to use with x_cm_set/get */
#define BRDCST_CTRL_SET_TS_DESCR                BRDCST_HANDLER_SET_GET(0)     /**< Define the command code to set TS DESCR               */
#define BRDCST_CTRL_GET_SIGNAL_LEVEL            BRDCST_HANDLER_SET_GET(1)     /**< Define the command code to get signal level           */
#define BRDCST_CTRL_GET_DBM_SIGNAL_LEVEL        BRDCST_HANDLER_SET_GET(2)     /**< Define the command code to get DBM signal level       */
#define BRDCST_CTRL_GET_DELIVERY_SYS            BRDCST_HANDLER_SET_GET(3)     /**< Define the command code to get delivery system        */
#define BRDCST_CTRL_GET_CAB_TUNE_INFO           BRDCST_HANDLER_SET_GET(4)     /**< Define the command code to get cable tuner info       */
#define BRDCST_CTRL_GET_SAT_TUNE_INFO           BRDCST_HANDLER_SET_GET(5)     /**< Define the command code to get satellite tuner info    */
#define BRDCST_CTRL_GET_TER_TUNE_INFO           BRDCST_HANDLER_SET_GET(6)     /**< Define the command code to terrestrial tuner info     */
#define BRDCST_CTRL_GET_AGC                     BRDCST_HANDLER_SET_GET(7)     /**< Define the command code to get AGC                    */
#define BRDCST_CTRL_GET_DBM_SNR                 BRDCST_HANDLER_SET_GET(8)     /**< Define the command code to get DBM SNR                */
#define BRDCST_CTRL_GET_PACKET_ERRORS           BRDCST_HANDLER_SET_GET(9)     /**< Define the command code to get packet errors          */
#define BRDCST_CTRL_GET_INTERLEAVING            BRDCST_HANDLER_SET_GET(10)    /**< Define the command code to get interleaving           */
#define BRDCST_CTRL_GET_KHZ_CFO                 BRDCST_HANDLER_SET_GET(11)    /**< Define the command code to get KHZ CFO                */
#define BRDCST_CTRL_GET_BER                     BRDCST_HANDLER_SET_GET(12)    /**< Define the command code to get BER                    */
#define BRDCST_CTRL_GET_RAW_SIGNAL_LEVEL        BRDCST_HANDLER_SET_GET(13)    /**< Define the command code to get raw signal level       */
#define BRDCST_CTRL_GET_PACKET_STATUS           BRDCST_HANDLER_SET_GET(14)    /**< Define the command code to get packet status          */
#define BRDCST_CTRL_GET_BER_TOLERANCE           BRDCST_HANDLER_SET_GET(15)    /**< Define the command code to get BER tolerance          */
#define BRDCST_CTRL_GET_CELL_ID                 BRDCST_HANDLER_SET_GET(16)    /**< Define the command code to get cell ID                */
#define BRDCST_CTRL_GET_TUNER_STATUS            BRDCST_HANDLER_SET_GET(17)    /**< Define the command code to get tuner status           */
#define BRDCST_CTRL_GET_CONN_FREQ               BRDCST_HANDLER_SET_GET(18)    /**< Define the command code to get connected frequency - Riks-TV requirement */
#define BRDCST_CTRL_GET_UEC                     BRDCST_HANDLER_SET_GET(19)    /**< Define the command code to get number of uncorrected errors at the output of the TS */
#define BRDCST_CTRL_GET_SQI                     BRDCST_HANDLER_SET_GET(20)    /**< Define the command code to get SQI */
#define BRDCST_CTRL_GET_SSI                     BRDCST_HANDLER_SET_GET(21)    /**< Define the command code to get SSI */
#define BRDCST_CTRL_GET_CODERATE                BRDCST_HANDLER_SET_GET(22)    /**< Define the command code to get code rate */
#define BRDCST_CTRL_GET_AFTER_BER               BRDCST_HANDLER_SET_GET(23)    /**< Define the command code to get post-viterbi BER */
#define BRDCST_CTRL_GET_PAT_TS_ID               BRDCST_HANDLER_SET_GET(24)    /**< Define the command code to get TS ID in PAT */
#define BRDCST_CTRL_GET_EWS_STATUS              BRDCST_HANDLER_SET_GET(25)    /**< Define the command code to get EWS status */

#define BRDCST_CTRL_SET_DIAGNOSTICS             BRDCST_HANDLER_SET_GET(50)    /**< Define the command code to set diagnostics            */
#define BRDCST_CTRL_SET_TUNE_ATTRIBUTES         BRDCST_HANDLER_SET_GET(51)    /**< Define the command code to set tuner attributes       */
#define BRDCST_CTRL_SET_AUDIO_STREAM_FMT        BRDCST_HANDLER_SET_GET(52)    /**< Define the command code to set audio stream format    */
#define BRDCST_CTRL_SET_FREQ_LIST_UPDATED_CHK   BRDCST_HANDLER_SET_GET(53)    /**< Define the command code to set frequency list updated check - Riks-TV requirement */
#define BRDCST_CTRL_SET_AUDIO_ENC               BRDCST_HANDLER_SET_GET(54)    /**< Define the command code to set audio stream format    */

//#ifdef MW_DVBS_DEV_ENABLE
#define BRDCST_CTRL_SET_DISEQC_COMMAND          BRDCST_HANDLER_SET_GET(55) 
#define BRDCST_CTRL_SET_TYPE_SCAN_CONTINUE      BRDCST_HANDLER_SET_GET(56) 
#define BRDCST_CTRL_SET_TYPE_22K                BRDCST_HANDLER_SET_GET(57) 
#define BRDCST_CTRL_SET_TYPE_SAT_POSITION       BRDCST_HANDLER_SET_GET(58) 
#define BRDCST_CTRL_SET_TYPE_TONE_BURST         BRDCST_HANDLER_SET_GET(59) 
//#endif
#define BRDCST_CTRL_SET_PRESENT_EVENT           BRDCST_HANDLER_SET_GET(60)    /**< Define the command code to set present event          */
/* this set command is for MHEG5 requirement */
#define BRDCST_CTRL_SET_SVL_REC_IN_SAME_TS      BRDCST_HANDLER_SET_GET(61)    /**< Define the command code to set SVL record ID in the same TS*/
#define BRDCST_CTRL_SET_TYPE_LNB_POWER          BRDCST_HANDLER_SET_GET(62) 

/* Connection reason */
#define BRDCST_CONNECT_OTHER_REASON                0U    /**< Connection reason of others                            */
#define BRDCST_CONNECT_NO_SERVICE                  1U    /**< Connection reason of no service                        */
#define BRDCST_CONNECT_PAT_TIMER_EXPIRED           2U    /**< Connection reason of PAT timer expired                 */
#define BRDCST_CONNECT_PMT_TIMER_EXPIRED           3U    /**< Connection reason of PMT timer expired                 */
#define BRDCST_CONNECT_MGT_TIMER_EXPIRED           4U    /**< Connection reason of MGT timer expired                 */
#define BRDCST_CONNECT_VCT_TIMER_EXPIRED           5U    /**< Connection reason of VCT timer expired                 */
#define BRDCST_CONNECT_WITHOUT_PMT_TABLE           6U    /**< Connection reason of load PAT table only               */

/* Disconnection reasons */
#define BRDCST_DISC_AS_REQUESTED                   0U    /**< Disconnect reason of user request                      */
#define BRDCST_DISC_OTHER_REASON                   1U    /**< Disconnect reason of other reason                      */
#define BRDCST_DISC_NEW_CONNECTION                 2U    /**< Disconnect reason of new connection                    */
#define BRDCST_DISC_PARENT_DISCONNECTED            3U    /**< Disconnect reason of parent disconnected               */
#define BRDCST_DISC_NO_COMP_FOUND                  4U    /**< Disconnect reason of unable found avaliable component  */
#define BRDCST_DISC_COMP_BUSY                      5U    /**< Disconnect reason of component busy                    */
#define BRDCST_DISC_COULD_NOT_SET_COMP_PROPERTIES  6U    /**< Disconnect reason of set component properties fail     */
#define BRDCST_DISC_COULD_NOT_CONNECT_COMP         7U    /**< Disconnect reason of could not connect component       */
#define BRDCST_DISC_COULD_NOT_LOAD_PAT             8U    /**< Disconnect reason of could load PAT table fail         */
#define BRDCST_DISC_COULD_NOT_LOAD_PMT             9U    /**< Disconnect reason of could load PMT table fail         */
#define BRDCST_DISC_COULD_NOT_LOAD_MGT            10U    /**< Disconnect reason of could load MGT table fail         */
#define BRDCST_DISC_COULD_NOT_LOAD_VCT            11U    /**< Disconnect reason of could load VCT table fail         */
#define BRDCST_DISC_PAT_TIMER_EXPIRED             12U    /**< Disconnect reason of PAT timer expired                 */
#define BRDCST_DISC_PMT_TIMER_EXPIRED             13U    /**< Disconnect reason of PMT timer expired                 */
#define BRDCST_DISC_MGT_TIMER_EXPIRED             14U    /**< Disconnect reason of MGT timer expired                 */
#define BRDCST_DISC_VCT_TIMER_EXPIRED             15U    /**< Disconnect reason of VCT timer expired                 */
#define BRDCST_DISC_NOT_FOUND_IN_VCT              16U    /**< Disconnect reason of unable found service in VCT       */
#define BRDCST_DISC_SVL_ERROR                     17U    /**< Disconnect reason of SVL error                         */
#define BRDCST_DISC_SVL_CLOSED                    18U    /**< Disconnect reason of SVL closed                        */
#define BRDCST_DISC_TSL_CLOSED                    19U    /**< Disconnect reason of TSL closed                        */
#define BRDCST_DISC_SATL_CLOSED                   20U    /**< Disconnect reason of SATL closed                       */
#define BRDCST_DISC_TUNER_SIGNAL_LOSS             21U    /**< Disconnect reason of tuner signal loss                 */
#define BRDCST_DISC_TUNER_UNKNOWN_LNB             22U    /**< Disconnect reason of tuner LNB unknown                 */
#define BRDCST_DISC_TUNER_OUT_OF_RANGE            23U    /**< Disconnect reason of tuer frequency out of range       */

/* Diseqc set reasons */
//#ifdef MW_DVBS_DEV_ENABLE
#define BRDCST_DISEQC_SET_SUCCESS                       0U
#define BRDCST_DISEQC_SET_MICROCONTROLLER_NOT_FOUND     1U
#define BRDCST_DISEQC_SET_INV_COMMAND                   2U
//#endif

/* To use with BRDCST_CTRL_SET_TUNE_ATTRIBUTES command */
#define BRDCST_TUNE_ATTRIBUTES_FREQUENCY           ((UINT32) 0x0001)    /**< Broadcast tune attribute of frequency          */
#define BRDCST_TUNE_ATTRIBUTES_SYMBOL_RATE         ((UINT32) 0x0002)    /**< Broadcast tune attribute of symbol rate        */
#define BRDCST_TUNE_ATTRIBUTES_BANDWIDTH           ((UINT32) 0x0004)    /**< Broadcast tune attribute of bandwidth          */
#define BRDCST_TUNE_ATTRIBUTES_FEC_OUTER           ((UINT32) 0x0008)    /**< Broadcast tune attribute of FEC outer          */
#define BRDCST_TUNE_ATTRIBUTES_FEC_INNER           ((UINT32) 0x0010)    /**< Broadcast tune attribute of FEC inner          */
#define BRDCST_TUNE_ATTRIBUTES_MODULATION          ((UINT32) 0x0020)    /**< Broadcast tune attribute of modulation         */
#define BRDCST_TUNE_ATTRIBUTES_HIERARCHY           ((UINT32) 0x0040)    /**< Broadcast tune attribute of hierarchy          */
#define BRDCST_TUNE_ATTRIBUTES_HIERARCHY_PRIORITY  ((UINT32) 0x0080)    /**< Broadcast tune attribute of hierarchy priority */
#define BRDCST_TUNE_ATTRIBUTES_GUARD_INTERVAL      ((UINT32) 0x0100)    /**< Broadcast tune attribute of guard interval     */
#define BRDCST_TUNE_ATTRIBUTES_TRANS_MODE          ((UINT32) 0x0200)    /**< Broadcast tune attribute of transmission mode  */
#define BRDCST_TUNE_ATTRIBUTES_POLARIZATION        ((UINT32) 0x0400)    /**< Broadcast tune attribute of polarization       */
#define BRDCST_TUNE_ATTRIBUTES_ORBITAL_POSITION    ((UINT32) 0x0800)    /**< Broadcast tune attribute of orbital position   */
#define BRDCST_TUNE_ATTRIBUTES_DIRECTION           ((UINT32) 0x1000)    /**< Broadcast tune attribute of direction          */
#define BRDCST_TUNE_ATTRIBUTES_GAIN                ((UINT32) 0x2000)    /**< Broadcast tune attribute of gain               */
#define BRDCST_TUNE_ATTRIBUTES_TUNER_CONN_MODE     ((UINT32) 0x4000)    /**< Broadcast tune attribute of tuner conn        */
//#ifdef MW_DVBS_DEV_ENABLE
#define BRDCST_TUNE_ATTRIBUTES_DISEQC_TYPE         ((UINT32) 0x8000)    /**< Broadcast tune attribute of tuner conneect mode */
#define BRDCST_TUNE_ATTRIBUTES_PORT                ((UINT32) 0x10000)    /**< Broadcast tune attribute of tuner conneect mode */
#define BRDCST_TUNE_ATTRIBUTES_CASCADE_TYPE        ((UINT32) 0x20000)    /**< Broadcast tune attribute of tuner conneect mode */
#define BRDCST_TUNE_ATTRIBUTES_CASCADE_PORT        ((UINT32) 0x40000)    /**< Broadcast tune attribute of tuner conneect mode */
#define BRDCST_TUNE_ATTRIBUTES_22K                 ((UINT32) 0x80000)    /**< Broadcast tune attribute of tuner conneect mode */
#define BRDCST_TUNE_ATTRIBUTES_TONE_BURST          ((UINT32) 0x100000)   /**< Broadcast tune attribute of tuner conneect mode */
#define BRDCST_TUNE_ATTRIBUTES_WXYZ                ((UINT32) 0x200000)    /**< Broadcast tune attribute of tuner conneect mode */

/* This structure contains the DiSEqC protocol information which will be transmitted */
/*#define BRDCST_DISEQC_22K_OFF               ((UINT8)0)
#define BRDCST_DISEQC_22K_ON                ((UINT8)1)
#define BRDCST_DISEQC_22K_UNKNOWN           ((UINT8)2)

#define BRDCST_DISEQC_SAT_POSITION_UNKNOWN  ((UINT8)0)
#define BRDCST_DISEQC_SAT_POSITION_1        ((UINT8)1)
#define BRDCST_DISEQC_SAT_POSITION_2        ((UINT8)2)
#define BRDCST_DISEQC_SAT_POSITION_3        ((UINT8)3)
#define BRDCST_DISEQC_SAT_POSITION_4        ((UINT8)4)

#define BRDCST_DISEQC_POL_KNOWN             ((UINT8)POL_UNKNOWN)             
#define BRDCST_DISEQC_POL_VERTICAL_13V      ((UINT8)POL_LIN_VERTICAL)
#define BRDCST_DISEQC_POL_HORIZONTAL_18V    ((UINT8)POL_LIN_HORIZONTAL)

#define BRDCST_DISEQC_TONE_BURST_OFF        ((UINT8)0)
#define BRDCST_DISEQC_TONE_BURST_A          ((UINT8)1)
#define BRDCST_DISEQC_TONE_BURST_B          ((UINT8)2)
*/

typedef enum
{
    BRDCST_SET_DISEQC_NONE,
    BRDCST_SET_DISEQC_10_CMD_RESET_MICROCONTROLLER, 
    BRDCST_SET_DISEQC_10_CMD_SET_WRITE_TO_PORT_GRP_0
}   BRDCST_CTRL_DISEQC_COMMAND_T;

typedef struct
{
    UINT8                   ui1_22k;
    UINT8                   ui1_polarity_13v_18v;
    UINT8                   ui1_sat_position;
}   DISEQC_10_SET_T;

typedef struct
{
    BRDCST_CTRL_DISEQC_COMMAND_T    e_diseqc_cmd;
    union {
        DISEQC_10_SET_T             t_diseqc_10_set;
    } u;
    UINT8                           ui1_tone_burst;
    UINT8                           ui1_repeat_cmd;
    UINT8                           ui1_lnb_power;          /* 0->LNB_POWER_OFF, 1->LNB_POWER_13V_18V, 2->LNB_POWER_14V_19V*/
}   BRDCST_CTRL_DISEQC_T;

//#endif

/*------------------------------------------------------------------*/
/*! @struct BRDCST_CTRL_TUNE_ATTRIBUTES_T
 *  @brief This structure contains the tuner attributes which will transmit in
 *  @code
 *  typedef struct
 *  {
 *      UINT32                      ui4_flags;
 *      UINT32                      ui4_freq;
 *      UINT32                      ui4_sym_rate;
 *      TUNER_BANDWIDTH_T           e_bandwidth;
 *      TUNER_FEC_OUTER_T           e_fec_outer;
 *      TUNER_FEC_INNER_T           e_fec_inner;
 *      TUNER_MODULATION_T          e_mod;
 *      TUNER_HIERARCHY_T           e_hierarchy;
 *      TUNER_HIERARCHY_PRIORITY_T  e_hierarchy_priority;
 *      TUNER_GUARD_INTERVAL_T      e_guard_int;
 *      TUNER_TRANSMISSION_MODE_T   e_trans_mode;
 *      TUNER_POLARIZATION_T        e_pol;
 *      TUNER_CONNECTION_MODE_T     e_conn_mode;
 *      INT16                       i2_orb_pos;
 *      UINT16                      ui2_dir;
 *      UINT16                      ui2_gain;
 *  }   BRDCST_CTRL_TUNE_ATTRIBUTES_T;
 *  @endcode
 *  @li@c  ui4_flags              -   The Mask to indicate which value is available in this structure
 *  @li@c  ui4_freq               -   Frequency value
 *  @li@c  ui4_sym_rate           -   Symbol rate value
 *  @li@c  e_bandwidth            -   Bandwidth value
 *  @li@c  e_fec_outer            -   Outer Forward Error Correction(FEC) scheme
 *  @li@c  e_fec_inner            -   Inner Forward Error Correction(FEC) scheme
 *  @li@c  e_mod                  -   Modulation scheme
 *  @li@c  e_hierarchy            -   Specified whether the transmission is hierarchical and what the alpha value is
 *  @li@c  e_hierarchy_priority   -   Hierarchy priority
 *  @li@c  e_guard_int            -   Guard interval value
 *  @li@c  e_trans_mode           -   Number of carries of in an OFDM frame
 *  @li@c  e_pol                  -   Polarization of the transmitted signal
 *  @li@c  e_conn_mode            -   Tuner connection mode
 *  @li@c  i2_orb_pos             -   Orbital position in degrees
 *  @li@c  ui2_dir                -   Direction in 0.1 degrees. Valid value range '0..3599' (inclusive)
 *  @li@c  ui2_gain               -   Gain in 0.1 steps. Valid range '0..1000' (inclusive)
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT32                      ui4_flags;
    UINT32                      ui4_freq;
    UINT32                      ui4_sym_rate;
    TUNER_BANDWIDTH_T           e_bandwidth;
    TUNER_FEC_OUTER_T           e_fec_outer;
    TUNER_FEC_INNER_T           e_fec_inner;
    TUNER_MODULATION_T          e_mod;
    TUNER_HIERARCHY_T           e_hierarchy;
    TUNER_HIERARCHY_PRIORITY_T  e_hierarchy_priority;
    TUNER_GUARD_INTERVAL_T      e_guard_int;
    TUNER_TRANSMISSION_MODE_T   e_trans_mode;
    TUNER_POLARIZATION_T        e_pol;
    TUNER_CONNECTION_MODE_T     e_conn_mode;
    INT16                       i2_orb_pos;
    UINT16                      ui2_dir;
    UINT16                      ui2_gain;
//#ifdef MW_DVBS_DEV_ENABLE
    TUNER_DISEQC_TYPE_T         e_diseqc_type; //none, 2x1, 4x1, 8x1, 16x1, 1.2, usals
    UINT8                       ui1_port;
    UINT8                       ui1_cascade_type;//none, 1
    UINT8                       ui1_cascade_port;
    UINT8                       ui1_22k; //0: off, 1: on
    UINT8                       ui1_tone_burst; //0: off, 1: A, 2: B
    UINT16                      ui2_wxyz;
//#endif
}   BRDCST_CTRL_TUNE_ATTRIBUTES_T;


/*------------------------------------------------------------------*/
/*! @struct BRDCST_CTRL_AUDIO_STREAM_FMT_T
 *  @brief This structure contains the audio stream information which will stransmit in
 *  @code
 *  typedef struct
 *  {
 *      UINT16     ui2_pid;
 *      AUD_FMT_T  e_audio_format;
 *      UINT8      ui1_index;
 *  }   BRDCST_CTRL_AUDIO_STREAM_FMT_T;
 *  @endcode
 *  @li@c  ui2_pid                            -   PID of the elementary stream which audio stream transmit in
 *  @li@c  e_audio_format                     -   Audio format
 *  @li@c  ui1_index                          -   Indicate the audio stream index
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT16     ui2_pid;
    AUD_FMT_T  e_audio_format;
    UINT8      ui1_index;
}   BRDCST_CTRL_AUDIO_STREAM_FMT_T;

/*------------------------------------------------------------------*/
/*! @struct BRDCST_CTRL_AUDIO_ENC_T
 *  @brief This structure contains the audio stream information which will stransmit in
 *  @code
 *  typedef struct
 *  {
 *      UINT16     ui2_pid;
 *      AUD_FMT_T  e_audio_enc;
 *      UINT8      ui1_index;
 *  }   BRDCST_CTRL_AUDIO_ENC_T;
 *  @endcode
 *  @li@c  ui2_pid                            -   PID of the elementary stream which audio stream transmit in
 *  @li@c  e_audio_enc                        -   Audio encoder
 *  @li@c  ui1_index                          -   Indicate the audio stream index
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT16     ui2_pid;
    AUD_ENC_T  e_audio_enc;
    UINT8      ui1_index;
}   BRDCST_CTRL_AUDIO_ENC_T;

/* Time-out values */
/*------------------------------------------------------------------*/
/*! @struct BRDCST_TIME_OUT_T
 *  @brief This structure contains all time out of related PSI/SI/ATSC tables.
 *  @code
 *  typedef struct
 *  {
 *      UINT32  ui4_pat_time_out;
 *      UINT32  ui4_pmt_time_out;
 *      UINT32  ui4_mgt_time_out;
 *      UINT32  ui4_cvct_time_out;
 *      UINT32  ui4_tvct_time_out;
 *      UINT32  ui4_eit_time_out;
 *      UINT32  ui4_eit_retry_delay;
 *  }   BRDCST_TIME_OUT_T;
 *  @endcode
 *  @li@c  ui4_pat_time_out                 - Time out value of PAT loading
 *  @li@c  ui4_pmt_time_out                 - Time out value of PMT loading 
 *  @li@c  ui4_mgt_time_out                 - Time out value of MGT loading
 *  @li@c  ui4_cvct_time_out                - Time out value of CVCT loading
 *  @li@c  ui4_tvct_time_out                - Time out value of TVCT loading
 *  @li@c  ui4_eit_time_out                 - Time out value of EIT loading
 *  @li@c  ui4_eit_retry_delay              - Delay time of EIT retry loading
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT32  ui4_pat_time_out;
    UINT32  ui4_pmt_time_out;
    UINT32  ui4_mgt_time_out;
    UINT32  ui4_cvct_time_out;
    UINT32  ui4_tvct_time_out;
    UINT32  ui4_eit_time_out;
    UINT32  ui4_eit_retry_delay;
}   BRDCST_TIME_OUT_T;

#endif /*  _U_BRDCST_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                 /* end of groupMW_CONN_MNGR_BRDCST */
/*----------------------------------------------------------------------------*/

