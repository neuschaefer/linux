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
/*! @file u_ch_hdlr.h 
 *  $File: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/project_x/middleware/svctx/ch_hdlr/u_ch_hdlr.h $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *
 */
/*----------------------------------------------------------------------------*/
#ifndef _U_CH_HDLR_H_
#define _U_CH_HDLR_H_
/*-----------------------------------------------------------------------------
 *  include files
 *---------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_tuner.h"
#include "u_sm.h"
#include "u_sm_cc_hdlr.h"
#include "u_analog_brdcst.h"

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_SVCTX_CH_HDLR Channel Handler
 *
 *  @ingroup groupMW_SVCTX
 *  @brief For user change channel.
 *
 *  %MW Application API would be used by Application, lot of these APIs would
 *   be wrapped from Exported API. Using these APIs, application could change channel.
 *
 *  @see groupMW_SVCTX
 *  @{ 
 */
/*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#define SRC_TYPE_BRDCST_SVL         ((SRC_TYPE_T) 0)    /**< select channel by svl */
#define SRC_TYPE_BRDCST_FREQ        ((SRC_TYPE_T) 1)    /**< select channel by frequency */

#define SVCTX_GRP_CH                                      (2<<SVCTX_TYPE_CODE_BITS)    /**<  channel group mask */
/* channel handler set types *********************************************************/
#define SVCTX_CH_SET_TYPE_IGNORE                          ((SVCTX_SET_TYPE_T)(SVCTX_GRP_CH | 0))    /**< skip this set type */
/* will not do any connection and treat such SVL record as no signal for client */
#define SVCTX_CH_SET_INVALID_SVL_REC_NW_MASK_BIT          ((SVCTX_SET_TYPE_T)(SVCTX_GRP_CH | 1))    /**< use invalid svl rec type */
#define SVCTX_CH_SET_AUD_AV_SYNC_MODE                     ((SVCTX_SET_TYPE_T)(SVCTX_GRP_CH | 2))    /**< use av sync mode type */
#define SVCTX_CH_SET_VID_I_FRAME_MODE                     ((SVCTX_SET_TYPE_T)(SVCTX_GRP_CH | 3))    /**< use video I fream type */
#define SVCTX_CH_SET_FREQ_LIST_UPDATED_CHK                ((SVCTX_SET_TYPE_T)(SVCTX_GRP_CH | 4))    /**< request to check frequency list update for RiksTV */
#define SVCTX_CH_SET_RECOVER_FROM_ALTER_TUNE              ((SVCTX_SET_TYPE_T)(SVCTX_GRP_CH | 5))    /**< request to recover from alternative freq tuning for RiksTV */
#define SVCTX_CH_SET_DELIVERY_SYSTEM_CHANGE               ((SVCTX_SET_TYPE_T)(SVCTX_GRP_CH | 6))    /**< request to notify to AP when delivery system change */

/* channel handler get types *********************************************************/
#define SVCTX_CH_GET_TYPE_IGNORE                          ((SVCTX_GET_TYPE_T)(SVCTX_GRP_CH | 0))    /**< skip this get type */
#define SVCTX_CH_GET_TYPE_CH_SRC                          ((SVCTX_GET_TYPE_T)(SVCTX_GRP_CH | 1)) /**< type = CH_SRC_TYPE_T*   size = sizeof(CH_SRC_TYPE_T)   */
#define SVCTX_CH_GET_SILENT_STATUS                        ((SVCTX_GET_TYPE_T)(SVCTX_GRP_CH | 2)) /**< type = BOOL*            size = sizeof(BOOL)            */
#define SVCTX_CH_GET_SILENT_SVC_REC_ID                    ((SVCTX_GET_TYPE_T)(SVCTX_GRP_CH | 3)) /**< type = UINT16*          size = sizeof(UINT16)          */
#define SVCTX_CH_GET_AUX_AUDIO_STATUS                     ((SVCTX_GET_TYPE_T)(SVCTX_GRP_CH | 4)) /**< type = SM_COND_T*       size = sizeof(SM_COND_T)       */
#define SVCTX_CH_GET_BRDCST_TYPE                          ((SVCTX_GET_TYPE_T)(SVCTX_GRP_CH | 5)) /**< type = BRDCST_TYPE_T*   size = sizeof(BRDCST_TYPE_T)   */
#define SVCTX_CH_GET_SRC_SVL_INFO                         ((SVCTX_GET_TYPE_T)(SVCTX_GRP_CH | 6)) /**< type = SRC_SVL_INFO_T*  size = sizeof(SRC_SVL_INFO_T)  */
#define SVCTX_CH_GET_SRC_FREQ_INFO                        ((SVCTX_GET_TYPE_T)(SVCTX_GRP_CH | 7)) /**< type = SRC_FREQ_INFO_T* size = sizeof(SRC_FREQ_INFO_T) */
#define SVCTX_CH_GET_FIRST_CH_SURF                        ((SVCTX_GET_TYPE_T)(SVCTX_GRP_CH | 8)) /**< type = BOOL*   size = sizeof(BOOL) */
#define SVCTX_CH_GET_NON_BRDCSTING_SVC                    ((SVCTX_GET_TYPE_T)(SVCTX_GRP_CH | 9)) /**< type = BOOL*   size = sizeof(BOOL) */
#ifdef SYS_MHP_SUPPORT
#define SVCTX_CH_GET_CUR_STC                              ((SVCTX_GET_TYPE_T)(SVCTX_GRP_CH | 10)) /**< type = STC_T*   size = sizeof(STC_T) */
#define SVCTX_CH_GET_SILENT_SVL_LST_ID                    ((SVCTX_GET_TYPE_T)(SVCTX_GRP_CH | 11)) /**< type = UINT16*  size = sizeof(UINT16)  */
#else
#define SVCTX_CH_GET_CURRENT_TS_ID                        ((SVCTX_GET_TYPE_T)(SVCTX_GRP_CH | 10)) /**< type = UINT16*  size = sizeof(UINT16)  */
#endif
#define SVCTX_CH_GET_AUD_ORIG_SCRAMBLE_STATUS             ((SVCTX_GET_TYPE_T)(SVCTX_GRP_CH | 12)) /**< type = BOOL*    size = sizeof(BOOL)  */
#define SVCTX_CH_GET_VID_ORIG_SCRAMBLE_STATUS             ((SVCTX_GET_TYPE_T)(SVCTX_GRP_CH | 13)) /**< type = BOOL*    size = sizeof(BOOL)  */

/* pv_get_info for SVCTX_CH_GET_TYPE_CH_SRC */
/*------------------------------------------------------------------*/
/*! @enum   CH_SRC_TYPE_T
 *  @brief  signal from tuner or scart
 *  @code
 *  typedef enum
 *  {
 *      CH_SRC_TYPE_UNKNOWN = 0,
 *      CH_SRC_TYPE_TUNER,
 *      CH_SRC_TYPE_SCART
 *  } CH_SRC_TYPE_T;
 *  @endcode
 *  @li@c  CH_SRC_TYPE_UNKNOWN                              unknown- unknown type
 *  @li@c  CH_SRC_TYPE_TUNER                                    tuner- signal from tuner
 *  @li@c  CH_SRC_TYPE_SCART                                    scart- signal from scart
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CH_SRC_TYPE_UNKNOWN = 0,
    CH_SRC_TYPE_TUNER,
    CH_SRC_TYPE_SCART
} CH_SRC_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct SILENT_SELECT_INFO_T
 *  @brief  no message nofity for select servcie
 *  @code
 *  typedef struct _SILENT_SELECT_INFO_T
 *  {
 *      BOOL                            b_enable;
 *      UINT16                          ui2_svl_rec_id;
 *  } SILENT_SELECT_INFO_T;
 *  @endcode
 *  @li@c  b_enable                                             b_enable- flag for open or close silent 
 *                                                                          function
 *  @li@c  ui2_svl_rec_id                                    ui2_svl_rec_id- identify for svl record
 */
/*------------------------------------------------------------------*/
typedef struct _SILENT_SELECT_INFO_T
{
    BOOL                            b_enable;
#ifdef SYS_MHP_SUPPORT
    UINT16                          ui2_svc_lst_id;
#endif
    UINT16                          ui2_svl_rec_id;
} SILENT_SELECT_INFO_T;

/*------------------------------------------------------------------*/
/*! @enum   CHANNEL_DECODE_TYPE_T
 *  @brief  the type for channel decode
 *  @code
 *  typedef enum
 *  {
 *      CHANNEL_DECODE_TYPE_DISABLE = 0,
 *      CHANNEL_DECODE_TYPE_MANUAL,
 *      CHANNEL_DECODE_TYPE_AUTO,
 *      CHANNEL_DECODE_TYPE_LAST_VALID_ENTRY
 *  } CHANNEL_DECODE_TYPE_T;
 *  @endcode
 *  @li@c  CHANNEL_DECODE_TYPE_DISABLE                       disalbe- disable
 *  @li@c  CHANNEL_DECODE_TYPE_MANUAL                        manual- user operation
 *  @li@c  CHANNEL_DECODE_TYPE_AUTO                            auto- auto use ch_dec
 *  @li@c  CHANNEL_DECODE_TYPE_LAST_VALID_ENTRY     last- skip
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CHANNEL_DECODE_TYPE_DISABLE = 0,
    CHANNEL_DECODE_TYPE_MANUAL,
    CHANNEL_DECODE_TYPE_AUTO,
    CHANNEL_DECODE_TYPE_LAST_VALID_ENTRY
} CHANNEL_DECODE_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct CHANNEL_DECODE_DESC_T
 *  @brief  description for channel decode
 *  @code
 *  typedef struct _CHANNEL_DECODE_DESC_T
 *  {
 *      CHANNEL_DECODE_TYPE_T           e_type;
 *      CHAR                            s_scart_name[CONN_SRC_NAME_MAX_STRLEN+1];
 *  } CHANNEL_DECODE_DESC_T;
 *  @endcode
 *  @li@c  e_type                                            e_type- ref CHANNEL_DECODE_TYPE_T
 *  @li@c  s_scart_name[CONN_SRC_NAME_MAX_STRLEN+1]          name- scart name
 */
/*------------------------------------------------------------------*/
typedef struct _CHANNEL_DECODE_DESC_T
{
    CHANNEL_DECODE_TYPE_T           e_type;
    CHAR                            s_scart_name[CONN_SRC_NAME_MAX_STRLEN+1];
} CHANNEL_DECODE_DESC_T;

/*------------------------------------------------------------------*/
/*! @struct SRC_SVL_INFO_T
 *  @brief  the detail info for select service
 *  @code
 *  typedef struct _SRC_SVL_INFO_T  
 *  {
 *      UINT16                          ui2_svc_lst_id;
 *      UINT16                          ui2_svl_rec_id;
 *      UINT32                          ui4_channel_id;
 *      SILENT_SELECT_INFO_T            t_silent;
 *      CHANNEL_DECODE_DESC_T           t_ch_encode;
 *      BOOL                            b_detect_tv_sys;
 *      ANALOG_BRDCST_SCART_MODE_T      e_scart_mode;
 *      BOOL                            b_enable_fake_stereo;
 *      BOOL                            b_enable_signal_booster;
 *  } SRC_SVL_INFO_T;
 *  @endcode
 *  @li@c  ui2_svc_lst_id                            ui2_svc_lst_id- service list identify
 *  @li@c  ui2_svl_rec_id                            ui2_svl_rec_id- svl record identify
 *  @li@c  ui4_channel_id                           ui4_channel_id- channel identify
 *  @li@c  t_silent                                        t_silent- silent config for select service
 *  @li@c  t_ch_encode                               t_ch_encode- channel decode config
 *  @li@c  b_detect_tv_sys                         b_detect_tv_sys- is or not detect tv system
 *  @li@c  e_scart_mode                             e_scart_mode- scart mode
 *  @li@c  b_enable_fake_stereo                b_enable_fake_stereo- open or close fake 
 *                                                                  stereo configuration
 *  @li@c  b_enable_signal_booster            b_enable_signal_booster- open or close
 *                                                                   signal booster configuration
    @li@c  b_ad_to_independent_ad             let cm set ad's mix type to independent or not  
 */
/*------------------------------------------------------------------*/
typedef struct _SRC_SVL_INFO_T  /* SRC_DESC_T's pv_details when e_type = SRC_TYPE_BRDCST_SVL*/
{
    UINT16                          ui2_svc_lst_id;
    UINT16                          ui2_svl_rec_id;
    UINT32                          ui4_channel_id;

    SILENT_SELECT_INFO_T            t_silent;
    CHANNEL_DECODE_DESC_T           t_ch_encode;
    BOOL                            b_detect_tv_sys;
    ANALOG_BRDCST_SCART_MODE_T      e_scart_mode;
    BOOL                            b_enable_fake_stereo;
    BOOL                            b_enable_signal_booster;
    BOOL                            b_ninja_change;
    BOOL                            b_force_change; /* disconnect and connect */
    BOOL                            b_substream_fake_stereo;
    BOOL                            b_ad_to_independent_ad;
} SRC_SVL_INFO_T;

/* frequency info type */
/*------------------------------------------------------------------*/
/*! @enum   FREQ_INFO_TYPE_T
 *  @brief  freqence detail info for select service
 *  @code
 *  typedef enum
 *  {
 *      FREQ_INFO_TYPE_DIG_TER = 0,
 *      FREQ_INFO_TYPE_DIG_CAB,
 *      FREQ_INFO_TYPE_DIG_SAT,
 *      FREQ_INFO_TYPE_ANA_TER,
 *      FREQ_INFO_TYPE_ANA_CAB,
 *      FREQ_INFO_TYPE_ANA_SAT,
 *      FREQ_INFO_TYPE_FM_RADIO,
 *      FREQ_INFO_TYPE_LAST_VALID_ENTRY
 *  } FREQ_INFO_TYPE_T;
 *  @endcode
 *  @li@c  FREQ_INFO_TYPE_DIG_TER                            FREQ_INFO_TYPE_DIG_TER- 
 *                                                                                     digital terrestrial
 *  @li@c  FREQ_INFO_TYPE_DIG_CAB                            FREQ_INFO_TYPE_DIG_CAB- 
 *                                                                                     digital cable
 *  @li@c  FREQ_INFO_TYPE_DIG_SAT                            FREQ_INFO_TYPE_DIG_SAT- 
 *                                                                                     digital satellite
 *  @li@c  FREQ_INFO_TYPE_ANA_TER                            FREQ_INFO_TYPE_ANA_TER- 
 *                                                                                     analog terrestrial
 *  @li@c  FREQ_INFO_TYPE_ANA_CAB                            FREQ_INFO_TYPE_ANA_CAB- 
 *                                                                                      analog cable
 *  @li@c  FREQ_INFO_TYPE_ANA_SAT                            FREQ_INFO_TYPE_ANA_SAT- 
 *                                                                                      analog satellite
 *  @li@c  FREQ_INFO_TYPE_FM_RADIO                           FREQ_INFO_TYPE_FM_RADIO- 
 *                                                                                      fm radio
 *  @li@c  FREQ_INFO_TYPE_LAST_VALID_ENTRY            FREQ_INFO_TYPE_LAST_VALID_ENTRY- 
 *                                                                                      skip
 */
/*------------------------------------------------------------------*/
typedef enum
{
    FREQ_INFO_TYPE_DIG_TER = 0,
    FREQ_INFO_TYPE_DIG_CAB,
    FREQ_INFO_TYPE_DIG_SAT,
    FREQ_INFO_TYPE_ANA_TER,
    FREQ_INFO_TYPE_ANA_CAB,
    FREQ_INFO_TYPE_ANA_SAT,
    FREQ_INFO_TYPE_FM_RADIO,
    FREQ_INFO_TYPE_LAST_VALID_ENTRY
} FREQ_INFO_TYPE_T;

/* frequency information */
/*------------------------------------------------------------------*/
/*! @struct SRC_FREQ_INFO_T
 *  @brief  freqence detail info for select service
 *  @code
 *  typedef struct _SRC_FREQ_INFO_T 
 *  {
 *      FREQ_INFO_TYPE_T                e_type;
 *      union {
 *          TUNER_TER_DIG_TUNE_INFO_T   t_dig_ter;
 *          TUNER_SAT_DIG_TUNE_INFO_T   t_dig_sat;
 *          TUNER_CAB_DIG_TUNE_INFO_T   t_dig_cab;
 *          TUNER_TER_ANA_TUNE_INFO_T   t_ana_ter;
 *          TUNER_SAT_ANA_TUNE_INFO_T   t_ana_sat;
 *          TUNER_CAB_ANA_TUNE_INFO_T   t_ana_cab;
 *      } u_data;
 *  } SRC_FREQ_INFO_T;
 *  @endcode
 *  @li@c  e_type                                            e_type- frequence source type
 *  @li@c  t_dig_ter                                         t_dig_ter- config digital terrestrial
 *  @li@c  t_dig_sat                                         t_dig_sat- config digital satellite
 *  @li@c  t_dig_cab                                         t_dig_cab- config digital cable
 *  @li@c  t_ana_ter                                         t_ana_ter- config analog terrestrial
 *  @li@c  t_ana_sat                                         t_ana_sat- config analog satellite
 *  @li@c  t_ana_cab                                         t_ana_cab- config analog cable
 */
/*------------------------------------------------------------------*/
typedef struct _SRC_FREQ_INFO_T /* SRC_DESC_T's pv_details when e_type = SRC_TYPE_BRDCST_FREQ */
{
    FREQ_INFO_TYPE_T                e_type;

#ifdef MW_DVBS_DEV_ENABLE
    UINT16                          ui2_sat_lst_rec_id;
    UINT16                          ui2_sat_lst_id;
#endif    
    
    union {
        TUNER_TER_DIG_TUNE_INFO_T   t_dig_ter;
        TUNER_SAT_DIG_TUNE_INFO_T   t_dig_sat;
        TUNER_CAB_DIG_TUNE_INFO_T   t_dig_cab;
        TUNER_TER_ANA_TUNE_INFO_T   t_ana_ter;
        TUNER_SAT_ANA_TUNE_INFO_T   t_ana_sat;
        TUNER_CAB_ANA_TUNE_INFO_T   t_ana_cab;
    } u_data;
} SRC_FREQ_INFO_T;

/*------------------------------------------------------------------*/
/*! @enum   SVCTX_AV_SYNC_MODE_T
 *  @brief  av sync type
 *  @code
 *  typedef enum
 *  {
 *      SVCTX_AV_SYNC_FREE_RUN = 0, 
 *      SVCTX_AV_SYNC_SLAVE,        
 *      SVCTX_AV_SYNC_MASTER        
 *  }   SVCTX_AV_SYNC_MODE_T;
 *  @endcode
 *  @li@c  SVCTX_AV_SYNC_FREE_RUN                        SVCTX_AV_SYNC_FREE_RUN- 
 *                                                                                  don't care av sync    
 *  @li@c  SVCTX_AV_SYNC_SLAVE                               SVCTX_AV_SYNC_SLAVE- 
 *                                                                                  slave sync av
 *  @li@c  SVCTX_AV_SYNC_MASTER                             SVCTX_AV_SYNC_MASTER- 
 *                                                                                   master sync av
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SVCTX_AV_SYNC_FREE_RUN = 0, /* No need to sync */
    SVCTX_AV_SYNC_SLAVE,        /* Sync to STC */
    SVCTX_AV_SYNC_MASTER        /* Update A-PTS to A-STC, may use in PCR or HardDisk playback */
}   SVCTX_AV_SYNC_MODE_T;

#endif /* _U_CH_HDLR_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                           /* end of groupMW_SVCTX_CH_HDLR */
/*----------------------------------------------------------------------------*/

