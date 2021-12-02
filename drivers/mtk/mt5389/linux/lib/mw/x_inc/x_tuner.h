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
 * $RCSfile: x_tuner.h,v $
 * $Revision: #2 $
 * $Date: 2012/04/28 $
 * $Author: hongjun.chu $
 *
 * Description:
 *         This header file contains tuner driver specific definitions, which
 *         are exported.
 *---------------------------------------------------------------------------*/

#ifndef _X_TUNER_H_
#define _X_TUNER_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_tuner.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* Connect types */
#define TUNER_CONN_TYPE(_x)             (RM_CONN_TYPE_LAST_ENTRY + (_x))
#define TUNER_CONN_TYPE_SAT_DIG         TUNER_CONN_TYPE(0)
#define TUNER_CONN_TYPE_CAB_DIG         TUNER_CONN_TYPE(1)
#define TUNER_CONN_TYPE_TER_DIG         TUNER_CONN_TYPE(2)
#define TUNER_CONN_TYPE_SAT_ANA         TUNER_CONN_TYPE(3)
#define TUNER_CONN_TYPE_CAB_ANA         TUNER_CONN_TYPE(4)
#define TUNER_CONN_TYPE_TER_ANA         TUNER_CONN_TYPE(5)
#define TUNER_CONN_TYPE_CAB_DIG_OOB_TX  TUNER_CONN_TYPE(6)
#define TUNER_CONN_TYPE_CAB_DIG_OOB_RX  TUNER_CONN_TYPE(7)
#define TUNER_CONN_TYPE_SAT_ANA_SCART_OUT  TUNER_CONN_TYPE(8)
#define TUNER_CONN_TYPE_CAB_ANA_SCART_OUT  TUNER_CONN_TYPE(9)
#define TUNER_CONN_TYPE_TER_ANA_SCART_OUT  TUNER_CONN_TYPE(10)

#define TUNER_CONN_TYPE_PVT_ANA         TUNER_CONN_TYPE(128)

/* Disconnect reasons */
#define TUNER_DISC_REASON(_x)           (RM_DISC_REASON_LAST_ENTRY + (_x))
#define TUNER_DISC_REASON_SIGNAL_LOSS   TUNER_DISC_REASON(0)
#define TUNER_DISC_REASON_UNKNOWN_LNB   TUNER_DISC_REASON(1)
#define TUNER_DISC_REASON_OUT_OF_RANGE  TUNER_DISC_REASON(2)

/* Get operations */
#define TUNER_GET_TYPE(_x)                     (RM_GET_TYPE_LAST_ENTRY + (_x))
#define TUNER_GET_TYPE_SAT_DIG                 TUNER_GET_TYPE(1)
#define TUNER_GET_TYPE_CAB_DIG                 TUNER_GET_TYPE(2)
#define TUNER_GET_TYPE_TER_DIG                 TUNER_GET_TYPE(3)
#define TUNER_GET_TYPE_SAT_ANA                 TUNER_GET_TYPE(4)
#define TUNER_GET_TYPE_CAB_ANA                 TUNER_GET_TYPE(5)
#define TUNER_GET_TYPE_TER_ANA                 TUNER_GET_TYPE(6)
#define TUNER_GET_TYPE_CAB_DIG_OOB_TX          TUNER_GET_TYPE(7)
#define TUNER_GET_TYPE_CAB_DIG_OOB_RX          TUNER_GET_TYPE(8)
#define TUNER_GET_TYPE_SIGNAL_LEVEL            TUNER_GET_TYPE(9)
#define TUNER_GET_TYPE_STATE                   TUNER_GET_TYPE(10)
#define TUNER_GET_TYPE_LNB                     TUNER_GET_TYPE(11)
#define TUNER_GET_TYPE_NO_DISC_NFY             TUNER_GET_TYPE(12)
#define TUNER_GET_TYPE_DBM_SIGNAL_LEVEL        TUNER_GET_TYPE(13)
#define TUNER_GET_TYPE_ANA_ATTRIBUTE           TUNER_GET_TYPE(14)
#define TUNER_GET_TYPE_TV_SYS_GROUP            TUNER_GET_TYPE(15)
#define TUNER_GET_TYPE_TV_SYS_MASK             TUNER_GET_TYPE(16)
#define TUNER_GET_TYPE_DBM_SNR                 TUNER_GET_TYPE(17)
#define TUNER_GET_TYPE_AGC                     TUNER_GET_TYPE(18)
#define TUNER_GET_TYPE_PACKET_ERRORS           TUNER_GET_TYPE(19)
#define TUNER_GET_TYPE_INTERLEAVING            TUNER_GET_TYPE(20)
#define TUNER_GET_TYPE_KHZ_CFO                 TUNER_GET_TYPE(21)
#define TUNER_GET_TYPE_BER                     TUNER_GET_TYPE(22)
#define TUNER_GET_TYPE_DIG_ATTRIBUTE           TUNER_GET_TYPE(23)
#define TUNER_GET_TYPE_RAW_SIGNAL_LEVEL        TUNER_GET_TYPE(24)
#define TUNER_GET_TYPE_SIGNAL_LEVEL_TOLERANCE  TUNER_GET_TYPE(25)
#define TUNER_GET_TYPE_NEXT_STEP_SIZE          TUNER_GET_TYPE(26)
#define TUNER_GET_TYPE_PACKET_STATUS           TUNER_GET_TYPE(27)
#define TUNER_GET_TYPE_BER_TOLERANCE           TUNER_GET_TYPE(28)
#define TUNER_GET_TYPE_CELL_ID                 TUNER_GET_TYPE(29)
#define TUNER_GET_TYPE_SIGNAL_STATUS		   TUNER_GET_TYPE(30)
#define TUNER_GET_TYPE_RFAGC                   TUNER_GET_TYPE(31)
#define TUNER_GET_TYPE_TS_FMT                  TUNER_GET_TYPE(32)
#define TUNER_GET_TYPE_UEC                     TUNER_GET_TYPE(33)
#define TUNER_GET_TYPE_SQI                     TUNER_GET_TYPE(34)
#define TUNER_GET_TYPE_SSI                     TUNER_GET_TYPE(35)
#define TUNER_GET_TYPE_EWS_STATUS              TUNER_GET_TYPE(36)
#define TUNER_GET_TYPE_DVBT2_PLP               TUNER_GET_TYPE(37)


/* Set operations */
#define TUNER_SET_TYPE(_x)                  (RM_SET_TYPE_LAST_ENTRY + (_x))
#define TUNER_SET_TYPE_LNB                  TUNER_SET_TYPE(11)
#define TUNER_SET_TYPE_NO_DISC_NFY         (TUNER_SET_TYPE(12) | RM_SET_TYPE_ARG_NO_REF)
#define TUNER_SET_TYPE_TV_SYS_MASK          TUNER_SET_TYPE(13)
#define TUNER_SET_TYPE_CAB_ANA_TUNE_INFO    TUNER_SET_TYPE(14)
#define TUNER_SET_TYPE_SAT_ANA_TUNE_INFO    TUNER_SET_TYPE(15)
#define TUNER_SET_TYPE_TER_ANA_TUNE_INFO    TUNER_SET_TYPE(16)
#define TUNER_SET_TYPE_CI_CARD_TYPE         TUNER_SET_TYPE(17)
#define TUNER_SET_TYPE_DIAGNOSTICS         (TUNER_SET_TYPE(18) | RM_SET_TYPE_ARG_NO_REF)
#define TUNER_SET_TYPE_CAB_DIG_TUNE_INFO    TUNER_SET_TYPE(19)
#define TUNER_SET_TYPE_SAT_DIG_TUNE_INFO    TUNER_SET_TYPE(20)
#define TUNER_SET_TYPE_TER_DIG_TUNE_INFO    TUNER_SET_TYPE(21)
#define TUNER_SET_TYPE_TS_SP                TUNER_SET_TYPE(22)
#define TUNER_SET_TYPE_TS_TRISTATE          TUNER_SET_TYPE(23)
#define TUNER_SET_TYPE_FREQ_AUTO_NFY        TUNER_SET_TYPE(24)
#define TUNER_SET_TYPE_DISEQC_CMD           TUNER_SET_TYPE(25)
#define TUNER_SET_TYPE_SCAN_CONTINUE        TUNER_SET_TYPE(26)
#define TUNER_SET_TYPE_TS_OUTPUT            TUNER_SET_TYPE(27)
#define TUNER_SET_TYPE_NO_AUD_VID           TUNER_SET_TYPE(28)


/* Flags used during registration */
#define TUNER_FLAG_PVT_CONN_DATA                    ((UINT32) 0x00010000)
#define TUNER_FLAG_SIG_LEVEL_NOT_SUPPORTED          ((UINT32) 0x00020000)
#define TUNER_FLAG_DBM_LEVEL_NOT_SUPPORTED          ((UINT32) 0x00040000)
#define TUNER_FLAG_DEC_REQ_FOR_TV_SIG_DETECTION     ((UINT32) 0x00080000)
#define TUNER_FLAG_DEC_REQ_FOR_COLOR_SYS_DETECTION  ((UINT32) 0x00100000)


/* DVB delivery system descriptor tune setup */
#define TUNER_DVB_DESCR_TUNE_LEN  13

typedef UINT8 TUNER_DVB_DESCR_TUNE_INFO_T [TUNER_DVB_DESCR_TUNE_LEN];

/* CI CAM type conditions */
typedef enum
{
    TUNER_CI_CARD_TYPE_1 = 0,
    TUNER_CI_CARD_TYPE_2,
    TUNER_CI_CARD_TYPE_3
}   TUNER_CI_TYPE_T;

/* State conditions */
typedef enum
{
    TUNER_STATE_DISCONNECTED = 0,
    TUNER_STATE_CONNECTING,
    TUNER_STATE_CONNECTED,
    TUNER_STATE_DISCONNECTING
}   TUNER_STATE_T;





/* Status conditions */
typedef enum
{
    TUNER_COND_STATUS_GOT_SIGNAL = 0,
    TUNER_COND_STATUS_LOST_SIGNAL,
    TUNER_COND_STATUS_EWS_ON,
    TUNER_COND_STATUS_EWS_OFF,
    TUNER_COND_STATUS_AUTOSCAN_END
}   TUNER_COND_STATUS_T;

/* Tuner state structure */
typedef struct
{
    TUNER_STATE_T  e_state;
    BOOL           b_dig_signal;
}   TUNER_STATE_INFO_T;

/* Analog tuner attribute */
typedef struct
{
    UINT32  ui4_lower_bound_freq;       /* Lower bound of supported range */
    UINT32  ui4_upper_bound_freq;       /* Upper bound of supported range */
    UINT32  ui4_fine_tune_delta_freq;   /* Fine-tune capability */
}   TUNER_ANA_ATTRIBUTE_T;

/* Digital tuner attribute */
typedef struct
{
    UINT32  ui4_lower_bound_freq;       /* Lower bound of supported range */
    UINT32  ui4_upper_bound_freq;       /* Upper bound of supported range */
    UINT32  ui4_fine_tune_delta_freq;   /* Fine-tune capability */
    BOOL    b_auto_qam_capability;      /*Auto QAM detection capability.*/
    BOOL    b_auto_sr_capability;       /*Auto SR detection capability.*/
}   TUNER_DIG_ATTRIBUTE_T;

/* TV system group structure */
typedef struct
{
    UINT8   ui1_tv_sys_group_idx;       /* TV system group index */
    UINT32  ui4_tv_sys_mask;            /* Mask of TV systems */
}   TUNER_TV_SYS_GROUP_INFO_T;

/* Step size structure */
typedef struct
{
    INT32  i4_step_size;
}   TUNER_STEP_SIZE_T;

/* Packet status structure */
typedef struct
{
    UINT32  ui4_errors;
    UINT8   ui1_error_rate;
    UINT8   ui1_lost_rate;
}   TUNER_PACKET_STATUS_T;

/*********** MPEG Format structure *************************************/
typedef enum
{
    SP_UNKNOWN,
    SP_SERIAL,
    SP_PARALLEL,
} SIG_SP_T;

/* Polarities */
typedef enum
{
    POL_UNKNOWN_ACTIVE,
    POL_HIGH_ACTIVE,
    POL_LOW_ACTIVE,
} SIG_POL_T;

typedef enum
{
    BOOL_UNKNOWN,
    BOOL_FALSE,
    BOOL_TRUE,
} BOOL_T;

/* TS format structure */
typedef struct 
{
    SIG_SP_T  SerOrPar;
    SIG_POL_T ClockPolarity;
    BOOL_T    fgTristate ;
    UINT32 u4TsClockInKHz; // get TS clock in kHz
    BOOL_T fgGatedClock; // get if it is gated clock
    BOOL_T fgExternalDemod; // get if it is 3rd-party or in-house demod. ex: 3rd-party BOOL_TRUE; In-house: BOOL_FALSE.
} TS_FMT_T;

/* To use with ANALOG_BRDCST_CTRL_GET/SET_SCART_BYPASS commands */
/*------------------------------------------------------------------*/
/*! @struct ANALOG_BRDCST_CTRL_FREQ_AUTO_NFY_T
 *  @brief  Structure for indicating if frequency notify is enabled.
 *  @code
 *  typedef struct
 *  {
 *      BOOL  b_enable_freq_auto_nfy;
 *  }   ANALOG_BRDCST_CTRL_FREQ_AUTO_NFY_T;
 *  @endcode
 *  @li@c  b_enable_freq_auto_nfy   - Indicates if frequency notify is enabled or not.
 */
/*------------------------------------------------------------------*/
typedef struct
{
    BOOL  b_enable_freq_auto_nfy;
}   ANALOG_BRDCST_CTRL_FREQ_AUTO_NFY_T;


typedef TUNER_TV_SYS_GROUP_INFO_T TUNER_TV_SYS_GROUP_INFO;
typedef TUNER_ANA_ATTRIBUTE_T TUNER_ANA_ATTRIBUTE;

#endif /* _X_TUNER_H_ */

