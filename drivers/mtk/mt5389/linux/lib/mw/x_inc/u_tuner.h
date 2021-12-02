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
/*! @file u_tuner.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains tuner driver specific definitions, which
 *         are exported.
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_TUNER_H_
#define _U_TUNER_H_

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_RES_MNGR_TUNER Tuner
 *  @ingroup groupMW_RES_MNGR
 *  @brief The u_tuner.h module defined Tuner Device is exported for AP to set driver parameters.
 *   
 *  @see groupMW
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_rm.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* Invalid direction and gain. */
#define INV_DIRECTION  ((UINT16) 0xffff)    /**<internel use        */
#define INV_GAIN       ((UINT16) 0xffff)    /**<internel use        */

/* Bandwidth */
/*------------------------------------------------------------------*/
/*! @enum TUNER_BANDWIDTH_T
 *  @brief internal use
 *  @code
 *  typedef enum
 *  {
 *      BW_UNKNOWN = 0,
 *      BW_6_MHz,
 *      BW_7_MHz,
 *      BW_8_MHz
 *  }   TUNER_BANDWIDTH_T;
 *  @endcode
 *  @li@c  BW_UNKNOWN                                        -
 *  @li@c  BW_6_MHz                                          -
 *  @li@c  BW_7_MHz                                          -
 *  @li@c  BW_8_MHz                                          -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    BW_UNKNOWN = 0,
    BW_6_MHz,
    BW_7_MHz,
    BW_8_MHz
}   TUNER_BANDWIDTH_T;

/* Modulation */
/*------------------------------------------------------------------*/
/*! @enum TUNER_MODULATION_T
 *  @brief modulation types, we only support 8 VSD, 64 QAM, 256 QAM, the other types are reserved.
 *  @code
 *  typedef enum
 *  {
 *      MOD_UNKNOWN = 0,
 *      MOD_PSK_8,
 *      MOD_VSB_8,
 *      MOD_VSB_16,
 *      MOD_QAM_16,
 *      MOD_QAM_32,
 *      MOD_QAM_64,
 *      MOD_QAM_80,
 *      MOD_QAM_96,
 *      MOD_QAM_112,
 *      MOD_QAM_128,
 *      MOD_QAM_160,
 *      MOD_QAM_192,
 *      MOD_QAM_224,
 *      MOD_QAM_256,
 *      MOD_QAM_320,
 *      MOD_QAM_384,
 *      MOD_QAM_448,
 *      MOD_QAM_512,
 *      MOD_QAM_640,
 *      MOD_QAM_768,
 *      MOD_QAM_896,
 *      MOD_QAM_1024,
 *      MOD_QPSK,
 *      MOD_OQPSK,
 *      MOD_BPSK,
 *      MOD_VSB_AM,
 *      MOD_QAM_4_NR,
 *      MOD_FM_RADIO
 *  }   TUNER_MODULATION_T;
 *  @endcode
 *  @li@c  MOD_UNKNOWN                                       - 
 *  @li@c  MOD_PSK_8                                         - 
 *  @li@c  MOD_VSB_8                                         - 
 *  @li@c  MOD_VSB_16                                        -
 *  @li@c  MOD_QAM_16                                        -
 *  @li@c  MOD_QAM_32                                        -
 *  @li@c  MOD_QAM_64                                        -
 *  @li@c  MOD_QAM_80                                        -
 *  @li@c  MOD_QAM_96                                        -
 *  @li@c  MOD_QAM_112                                       -
 *  @li@c  MOD_QAM_128                                       -
 *  @li@c  MOD_QAM_160                                       -
 *  @li@c  MOD_QAM_192                                       -
 *  @li@c  MOD_QAM_224                                       -
 *  @li@c  MOD_QAM_256                                       -
 *  @li@c  MOD_QAM_320                                       -
 *  @li@c  MOD_QAM_384                                       -
 *  @li@c  MOD_QAM_448                                       -
 *  @li@c  MOD_QAM_512                                       -
 *  @li@c  MOD_QAM_640                                       -
 *  @li@c  MOD_QAM_768                                       -
 *  @li@c  MOD_QAM_896                                       -
 *  @li@c  MOD_QAM_1024                                      -
 *  @li@c  MOD_QPSK                                          -
 *  @li@c  MOD_OQPSK                                         -
 *  @li@c  MOD_BPSK                                          -
 *  @li@c  MOD_VSB_AM                                        -
 *  @li@c  MOD_QAM_4_NR                                      -
 *  @li@c  MOD_FM_RADIO                                      -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MOD_UNKNOWN = 0,
    MOD_PSK_8,
    MOD_VSB_8,
    MOD_VSB_16,
    MOD_QAM_16,
    MOD_QAM_32,
    MOD_QAM_64,
    MOD_QAM_80,
    MOD_QAM_96,
    MOD_QAM_112,
    MOD_QAM_128,
    MOD_QAM_160,
    MOD_QAM_192,
    MOD_QAM_224,
    MOD_QAM_256,
    MOD_QAM_320,
    MOD_QAM_384,
    MOD_QAM_448,
    MOD_QAM_512,
    MOD_QAM_640,
    MOD_QAM_768,
    MOD_QAM_896,
    MOD_QAM_1024,    
    MOD_QPSK,
    MOD_OQPSK,
    MOD_BPSK,
    MOD_VSB_AM,
    MOD_QAM_4_NR,
    MOD_FM_RADIO
}   TUNER_MODULATION_T;

/* FEC inner */
/*------------------------------------------------------------------*/
/*! @enum TUNER_FEC_INNER_T
 *  @brief Internal use
 *  @code
 *  typedef enum
 *  {
 *      FEC_I_UNKNOWN = 0,
 *      FEC_I_NO_CODING,
 *      FEC_I_1_2,
 *      FEC_I_2_3,
 *      FEC_I_3_4,
 *      FEC_I_3_5,
 *      FEC_I_4_5,
 *      FEC_I_5_6,
 *      FEC_I_5_11,
 *      FEC_I_7_8,
 *      FEC_I_8_9,
 *      FEC_I_2_5
 *  }   TUNER_FEC_INNER_T;
 *  @endcode
 *  @li@c  FEC_I_UNKNOWN                                     -
 *  @li@c  FEC_I_NO_CODING                                   -
 *  @li@c  FEC_I_1_2                                         -
 *  @li@c  FEC_I_2_3                                         -
 *  @li@c  FEC_I_3_4                                         -
 *  @li@c  FEC_I_3_5                                         -
 *  @li@c  FEC_I_4_5                                         -
 *  @li@c  FEC_I_5_6                                         -
 *  @li@c  FEC_I_5_11                                        -
 *  @li@c  FEC_I_7_8                                         -
 *  @li@c  FEC_I_8_9                                         -
 *  @li@c  FEC_I_2_5                                         -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    FEC_I_UNKNOWN = 0,
    FEC_I_NO_CODING,
    FEC_I_1_2,
    FEC_I_2_3,
    FEC_I_3_4,
    FEC_I_3_5,
    FEC_I_4_5,
    FEC_I_5_6,
    FEC_I_5_11,
    FEC_I_7_8,
    FEC_I_8_9,
    FEC_I_2_5
}   TUNER_FEC_INNER_T;

/* FEC outer */
/*------------------------------------------------------------------*/
/*! @enum TUNER_FEC_OUTER_T
 *  @brief Internal use
 *  @code
 *  typedef enum
 *  {
 *      FEC_O_UNKNOWN = 0,
 *      FEC_O_NO_CODING,
 *      FEC_O_RS_204_188,
 *      FEC_O_
 *  }   TUNER_FEC_OUTER_T;
 *  @endcode
 *  @li@c  FEC_O_UNKNOWN                                     -
 *  @li@c  FEC_O_NO_CODING                                   -
 *  @li@c  FEC_O_RS_204_188                                  -
 *  @li@c  FEC_O_                                            -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    FEC_O_UNKNOWN = 0,
    FEC_O_NO_CODING,
    FEC_O_RS_204_188,
    FEC_O_
}   TUNER_FEC_OUTER_T;

/* Guard interval */
/*------------------------------------------------------------------*/
/*! @enum TUNER_GUARD_INTERVAL_T
 *  @brief Internal use
 *  @code
 *  typedef enum
 *  {
 *      GUARD_INT_UNKNOWN = 0,
 *      GUARD_INT_1_32,
 *      GUARD_INT_1_16,
 *      GUARD_INT_1_8,
 *      GUARD_INT_1_4,
 *      GUARD_INT_420,
 *      GUARD_INT_595,
 *      GUARD_INT_945
 *  }   TUNER_GUARD_INTERVAL_T;
 *  @endcode
 *  @li@c  GUARD_INT_UNKNOWN                                 -
 *  @li@c  GUARD_INT_1_32                                    -
 *  @li@c  GUARD_INT_1_16                                    -
 *  @li@c  GUARD_INT_1_8                                     -
 *  @li@c  GUARD_INT_1_4                                     -
 *  @li@c  GUARD_INT_420                                     -
 *  @li@c  GUARD_INT_595                                     -
 *  @li@c  GUARD_INT_945                                     -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    GUARD_INT_UNKNOWN = 0,
    GUARD_INT_1_32,
    GUARD_INT_1_16,
    GUARD_INT_1_8,
    GUARD_INT_1_4,
    GUARD_INT_420,
    GUARD_INT_595,
    GUARD_INT_945
}   TUNER_GUARD_INTERVAL_T;

/* hierarchy */
/*------------------------------------------------------------------*/
/*! @enum TUNER_HIERARCHY_T
 *  @brief Internal use
 *  @code
 *  typedef enum
 *  {
 *      HIR_UNKNOWN = 0,
 *      HIR_NONE,
 *      HIR_ALPHA_1,
 *      HIR_ALPHA_2,
 *      HIR_ALPHA_4
 *  }   TUNER_HIERARCHY_T;
 *  @endcode
 *  @li@c  HIR_UNKNOWN                                       -
 *  @li@c  HIR_NONE                                          -
 *  @li@c  HIR_ALPHA_1                                       -
 *  @li@c  HIR_ALPHA_2                                       -
 *  @li@c  HIR_ALPHA_4                                       -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    HIR_UNKNOWN = 0,
    HIR_NONE,
    HIR_ALPHA_1,
    HIR_ALPHA_2,
    HIR_ALPHA_4
}   TUNER_HIERARCHY_T;

/* hierarchy priority */
/*------------------------------------------------------------------*/
/*! @enum TUNER_HIERARCHY_PRIORITY_T
 *  @brief Internal use
 *  @code
 *  typedef enum
 *  {
 *      HIR_PRIORITY_UNKNOWN = 0,
 *      HIR_PRIORITY_NONE,
 *      HIR_PRIORITY_HIGH,
 *      HIR_PRIORITY_LOW
 *  }   TUNER_HIERARCHY_PRIORITY_T;
 *  @endcode
 *  @li@c  HIR_PRIORITY_UNKNOWN                              -
 *  @li@c  HIR_PRIORITY_NONE                                 -
 *  @li@c  HIR_PRIORITY_HIGH                                 -
 *  @li@c  HIR_PRIORITY_LOW                                  -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    HIR_PRIORITY_UNKNOWN = 0,
    HIR_PRIORITY_NONE,
    HIR_PRIORITY_HIGH,
    HIR_PRIORITY_LOW
}   TUNER_HIERARCHY_PRIORITY_T;

/* Polarization */
/*------------------------------------------------------------------*/
/*! @enum TUNER_POLARIZATION_T
 *  @brief Internal use
 *  @code
 *  typedef enum
 *  {
 *      POL_UNKNOWN = 0,
 *      POL_LIN_HORIZONTAL,
 *      POL_LIN_VERTICAL,
 *      POL_CIR_LEFT,
 *      POL_CIR_RIGHT
 *  }   TUNER_POLARIZATION_T;
 *  @endcode
 *  @li@c  POL_UNKNOWN                                       -
 *  @li@c  POL_LIN_HORIZONTAL                                -
 *  @li@c  POL_LIN_VERTICAL                                  -
 *  @li@c  POL_CIR_LEFT                                      -
 *  @li@c  POL_CIR_RIGHT                                     -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    POL_UNKNOWN = 0,
    POL_LIN_HORIZONTAL,
    POL_LIN_VERTICAL,
    POL_CIR_LEFT,
    POL_CIR_RIGHT
}   TUNER_POLARIZATION_T;

/* Transmission mode */
/*------------------------------------------------------------------*/
/*! @enum TUNER_TRANSMISSION_MODE_T
 *  @brief Internal use
 *  @code
 *  typedef enum
 *  {
 *      TRANS_MODE_UNDEFINED = 0,
 *      TRANS_MODE_2_K,
 *      TRANS_MODE_8_K,
 *      TRANS_MODE_SINGLE,
 *      TRANS_MODE_MULTI
 *  }   TUNER_TRANSMISSION_MODE_T;
 *  @endcode
 *  @li@c  TRANS_MODE_UNDEFINED                              -
 *  @li@c  TRANS_MODE_2_K                                    -
 *  @li@c  TRANS_MODE_8_K                                    -
 *  @li@c  TRANS_MODE_SINGLE                                 -
 *  @li@c  TRANS_MODE_MULTI                                  -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    TRANS_MODE_UNDEFINED = 0,
    TRANS_MODE_2_K,
    TRANS_MODE_8_K,
    TRANS_MODE_SINGLE,
    TRANS_MODE_MULTI
}   TUNER_TRANSMISSION_MODE_T;

/* Interleaving */
/*------------------------------------------------------------------*/
/*! @struct TUNER_INTERLEAVING_T
 *  @brief Internal use
 *  @code
 *  typedef struct
 *  {
 *      UINT8  ui1_interleaving_i;
 *      UINT8  ui1_interleaving_j;
 *  }   TUNER_INTERLEAVING_T;
 *  @endcode
 *  @li@c  ui1_interleaving_i                                -
 *  @li@c  ui1_interleaving_j                                -
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT8  ui1_interleaving_i;
    UINT8  ui1_interleaving_j;
}   TUNER_INTERLEAVING_T;

/* Tuner connection mode */
/*------------------------------------------------------------------*/
/*! @enum TUNER_CONNECTION_MODE_T
 *  @brief Internal use
 *  @code
 *  typedef enum
 *  {
 *      CONN_MODE_UNKNOWN = 0,
 *      CONN_MODE_SCAN,
 *      CONN_MODE_SURF
 *  }   TUNER_CONNECTION_MODE_T;
 *  @endcode
 *  @li@c  CONN_MODE_UNKNOWN                                 -
 *  @li@c  CONN_MODE_SCAN                                    -
 *  @li@c  CONN_MODE_SURF                                    -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CONN_MODE_UNKNOWN = 0,
    CONN_MODE_SCAN,
    CONN_MODE_SURF
}   TUNER_CONNECTION_MODE_T;

/* Timer interleaving */
/*------------------------------------------------------------------*/
/*! @enum TUNER_TIMER_INTERLEAVING_T
 *  @brief Internal use
 *  @code
 *  typedef enum
 *  {
 *      TIMER_INT_UNKNOWN = 0,
 *      TIMER_INT_240,
 *      TIMER_INT_720
 *  }   TUNER_TIMER_INTERLEAVING_T;
 *  @endcode
 *  @li@c  TIMER_INT_UNKNOWN                                 -
 *  @li@c  TIMER_INT_240                                     -
 *  @li@c  TIMER_INT_720                                     -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    TIMER_INT_UNKNOWN = 0,
    TIMER_INT_240,
    TIMER_INT_720
}   TUNER_TIMER_INTERLEAVING_T;

/*------------------------------------------------------------------*/
/*! @brief  Enumeration for tuner LNB types for DVB-S
 *  @code
 *  typedef enum _TUNER_LNB_TYPE_T
 *  {
 *      LNB_UNKNOWN,
 *      LNB_SINGLE_FREQ,
 *      LNB_DUAL_FREQ
 *  } TUNER_LNB_TYPE_T;
 *  @endcode
 *  @li@c  LNB_UNKNOWN                   - Unknown
 *  @li@c  LNB_SINGLE_FREQ               - Single frequency
 *  @li@c  LNB_DUAL_FREQ                 - Dual frequency
 */
/*------------------------------------------------------------------*/
typedef enum _TUNER_LNB_TYPE_T
{
    LNB_UNKNOWN,
    LNB_SINGLE_FREQ,
    LNB_DUAL_FREQ
} TUNER_LNB_TYPE_T;


/*------------------------------------------------------------------*/
/*! @enum TUNER_DVB_T_T2_MODE_T
 *  @brief Internal use
 *  @code
 *  typedef enum
 *  {
 *      TUNER_DVB_AUTO_T_T2 = 0,
 *      TUNER_DVB_T,
 *      TUNER_DVB_T2
 *  }   TUNER_DVB_T_T2_MODE_T;
 *  @endcode
 *  @li@c  TUNER_DVB_AUTO_T_T2                             -
 *  @li@c  TUNER_DVB_T                                     -
 *  @li@c  TUNER_DVB_T2                                    -
 */
/*------------------------------------------------------------------*/
typedef enum _TUNER_DVB_T_T2_MODE_T
{
    TUNER_DVB_AUTO_T_T2 = 0,
    TUNER_DVB_T,
    TUNER_DVB_T2,
} TUNER_DVB_T_T2_MODE_T;

#define DVBT2_PLP_LIST_MAX_COUNT  ((UINT8)255)
typedef struct _TUNER_DVBT2_PLP_LIST_INFO
	{
	UINT8 ui1_PLPLIST[DVBT2_PLP_LIST_MAX_COUNT];
	UINT8 ui1_current_count;
	}
TUNER_DVBT2_PLP_LIST_INFO;
/*------------------------------------------------------------------*/
/*! @brief  Enumeration for tuner Diseqc types for DVB-S
 *  @code
 *  typedef enum _TUNER_DISEQC_TYPE_T
 *  {
 *      DISEQC_NONE,
 *      DISEQC_2X1,
 *      DISEQC_4X1,
 *      DISEQC_8X1,
 *      DISEQC_16X1,
 *      DISEQC_1DOT2,
 *      DISEQC_USALS
 *  } TUNER_DISEQC_TYPE_T;
 *  @endcode
 *  @li@c  DISEQC_NONE                   - None
 *  @li@c  DISEQC_2X1                    - 2x1
 *  @li@c  DISEQC_4X1                    - 4x1
 *  @li@c  DISEQC_8X1                    - 8x1
 *  @li@c  DISEQC_16X1                   - 16x1
 *  @li@c  DISEQC_1DOT2                  - 1.2
 *  @li@c  DISEQC_USALS                  - USALS
 */
/*------------------------------------------------------------------*/
typedef enum _TUNER_DISEQC_TYPE_T
{
    DISEQC_NONE,
    DISEQC_2X1,
    DISEQC_4X1,
    DISEQC_8X1,
    DISEQC_16X1,
    DISEQC_1DOT2,
    DISEQC_USALS
} TUNER_DISEQC_TYPE_T;
/* Code Rate */
/*------------------------------------------------------------------*/
/*! @enum TUNER_CODRRATE_T
 *  @brief Internal use
 *  @code
 *  typedef enum _TUNER_CODRRATE_T
 *  {
 *      CODE_RATE04 = 0,
 *      CODE_RATE06,
 *      CODE_RATE08
 *  }   TUNER_CODRRATE_TT;
 *  @endcode
 *  @li@c  CODE_RATE04                                 -
 *  @li@c  CODE_RATE06                                     -
 *  @li@c  CODE_RATE08                                     -
 */
/*------------------------------------------------------------------*/
typedef enum _TUNER_CODRRATE_T
{
    CODE_RATE04 = 0,
    CODE_RATE06,    
    CODE_RATE08
} TUNER_CODRRATE_T;
/* Carrier Mode */
/*------------------------------------------------------------------*/
/*! @enum TUNER_CARRIER_MODE_T
 *  @brief Internal use
 *  @code
 *  typedef enum _TUNER_CARRIER_MODE_T
 *  {
 *      MUTI_CARRIER = 0,
 *      SINGAL_CARRIER 
 *  }   TUNER_CARRIER_MODE_T;
 *  @endcode
 *  @li@c  MUTI_CARRIER                                 -
 *  @li@c  SINGAL_CARRIER                                     -                                    
 */
/*------------------------------------------------------------------*/
typedef enum _TUNER_CARRIER_MODE_T
{
    MUTI_CARRIER = 0,
    SINGAL_CARRIER
} TUNER_CARRIER_MODE_T;
/* PN sequence */
/*------------------------------------------------------------------*/
/*! @enum TUNER_PN_SEQ_T
 *  @brief Internal use
 *  @code
 *  typedef enum _TUNER_PN_SEQ_T
 *  {
 *      VAR_PN_SEQ = 0,
 *      CON_PN_SEQ 
 *  }   TUNER_PN_SEQ_T;
 *  @endcode
 *  @li@c  VAR_PN_SEQ                                 -
 *  @li@c  CON_PN_SEQ                                     -                                    
 */
/*------------------------------------------------------------------*/
typedef enum _TUNER_PN_SEQ_T
{
    VAR_PN_SEQ = 0,
    CON_PN_SEQ
} TUNER_PN_SEQ_T;

/*------------------------------------------------------------------*/
/*! @enum TUNER_LNB_POWER_T
 *  @brief Internal use
 *  @code
 *  typedef enum _TUNER_LNB_POWER_T
 *  {
 *      LNB_POWER_OFF = 0,
 *      LNB_POWER_13V_18V ,
 *      LNB_POWER_14V_19V 
 *  }   TUNER_LNB_POWER_T;
 *  @endcode
 *  @li@c  VAR_PN_SEQ                                 -
 *  @li@c  CON_PN_SEQ                                     -                                    
 */
/*------------------------------------------------------------------*/
typedef enum _TUNER_LNB_POWER_T
{
    LNB_POWER_OFF = 0,
    LNB_POWER_13V_18V ,
    LNB_POWER_14V_19V 
} TUNER_LNB_POWER_T;

/*------------------------------------------------------------------*/
/*! @brief  Structure of tuner LNB information for DVB-S
 *  @code
 *  typedef struct _TUNER_LNB_INFO_T
 *  {
 *      TUNER_LNB_TYPE_T    e_lnb_type; 
 *      UINT16              ui2_lnb_low_freq; 
 *      UINT16              ui2_lnb_high_freq; 
 *      UINT16              ui2_lnb_switch_freq; 
 *  } TUNER_LNB_INFO_T;
 *  @endcode
 *  @li@c  e_lnb_type                    - LNB type
 *  @li@c  ui2_lnb_low_freq          - LNB low frequency in MHz
 *  @li@c  ui2_lnb_high_freq         - LNB high frequency in MHz
 *  @li@c  ui2_lnb_switch_freq       - LNB switch frequency in MHz
 */
/*------------------------------------------------------------------*/
typedef struct _TUNER_LNB_INFO_T
{
    TUNER_LNB_TYPE_T    e_lnb_type; /*LNB_UNKNOWN, LNB_SINGLE_FREQ, LNB_DUAL_FREQ*/
    UINT16              ui2_lnb_low_freq; /*in MHz*/
    UINT16              ui2_lnb_high_freq; /*in MHz*/
    UINT16              ui2_lnb_switch_freq; /*in MHz, for dual freq*/
} TUNER_LNB_INFO_T;

/*------------------------------------------------------------------*/
/*! @brief  Structure for tuner Diseqc information for DVB-S
 *  @code
 *  typedef struct _TUNER_DISEQC_INFO_T
 *  {
 *      TUNER_DISEQC_TYPE_T e_diseqc_type; 
 *      UINT8               ui1_port;
 *      UINT8               ui1_cascade_type; 
 *      UINT8               ui1_cascade_port;
 *      UINT8               ui1_22k; 
 *      UINT8               ui1_tone_burst; 
 *      TUNER_POLARIZATION_T e_polar; 
 *      UINT16              ui2_wxyz;
 *  } TUNER_DISEQC_INFO_T;
 *  @endcode
 *  @li@c  e_diseqc_type                 - Diseqc type: none, 2x1, 4x1, 8x1, 16x1, 1.2, usals
 *  @li@c  ui1_port                      - Port
 *  @li@c  ui1_cascade_type              - Casade type (none, 1)
 *  @li@c  ui1_cascade_port              - Casade port
 *  @li@c  ui1_22k                       - 22K (on/off)
 *  @li@c  ui1_tone_burst                - Tone burst (off, A, B)
 *  @li@c  e_polar                       - Polarity
 *  @li@c  ui2_wxyz                      - wxyz
 */
/*------------------------------------------------------------------*/
typedef struct _TUNER_DISEQC_INFO_T
{
    TUNER_DISEQC_TYPE_T e_diseqc_type; /*none, 2x1, 4x1, 8x1, 16x1, 1.2, usals*/
    UINT8               ui1_port;/*check u_tuner.h, TUNER_DISEQC_SAT_POSITION_0, TUNER_DISEQC_SAT_POSITION_1, 
                                                        TUNER_DISEQC_SAT_POSITION_2, TUNER_DISEQC_SAT_POSITION_3, TUNER_DISEQC_SAT_POSITION_DISABLE*/
    UINT8               ui1_cascade_type; /*none, 1*/
    UINT8               ui1_cascade_port;
    UINT8               ui1_22k; /*0: off, 1: on, 2:auto */ 
                                 /*if lnb_type is LNB_DUAL_FREQ, then 22k should be always auto. Otherwise, it can be selected as off or on.*/
    UINT8               ui1_tone_burst; /*0: burst 0, 1: burst 1*/
    TUNER_POLARIZATION_T e_polar; /*if lnb_type is LNB_DUAL_FREQ, then lnb power should be always auto. 
                                                          Otherwise, it can be selected as 14v (vertical) or 18v (horizontal).*/
    TUNER_LNB_POWER_T    e_lnb_power; /*LNB_POWER_OFF, LNB_POWER_13V_18V, LNB_POWER_14V_19V*/
    UINT16              ui2_wxyz;
} TUNER_DISEQC_INFO_T;

/* Analog cable setup structure */
/*------------------------------------------------------------------*/
/*! @struct TUNER_CAB_ANA_TUNE_INFO_T
 *  @brief The structure to detail the parameters for analog cable tuning.
 *  @code
 *  typedef struct _TUNER_CAB_ANA_TUNE_INFO_T
 *  {
 *      UINT32                   ui4_freq;
 *      UINT32                   ui4_freq_bound;
 *      UINT32                   ui4_central_freq;
 *      UINT32                   ui4_freq_offset;
 *      UINT32                   ui4_tv_sys_mask;
 *      TUNER_MODULATION_T       e_mod;
 *      BOOL                     b_fine_tune;
 *      BOOL                     b_no_disc_nfy;
 *      INT16                    i2_fine_tune_offset;
 *  }   TUNER_CAB_ANA_TUNE_INFO_T;
 *  @endcode
 *  @li@c  ui4_freq                                          - locked frequency
 *  @li@c  ui4_freq_bound                                    - reserved
 *  @li@c  ui4_central_freq                                  - central frequency of channel map
 *  @li@c  ui4_freq_offset                                   - the bias offset of locked channel frequency
 *  @li@c  ui4_tv_sys_mask                                   - tv system mask
 *  @li@c  e_mod                                             - modulation types
 *  @li@c  b_fine_tune                                       - ask driver to do the autofine tune or not
 *  @li@c  b_no_disc_nfy                                     - no notification when disconnected
 *  @li@c  i2_fine_tune_offset                               - the bias offset of user fine tune
 */
/*------------------------------------------------------------------*/
typedef struct _TUNER_CAB_ANA_TUNE_INFO_T
{
    UINT32                   ui4_freq;
    UINT32                   ui4_freq_bound;
    UINT32                   ui4_central_freq;
    UINT32                   ui4_freq_offset;
    UINT32                   ui4_tv_sys_mask;
    TUNER_MODULATION_T       e_mod;
    BOOL                     b_fine_tune;
    BOOL                     b_no_disc_nfy;
    INT16                    i2_fine_tune_offset;
#ifdef MW_CM_SET_COUNTRY_TO_TUNER
	ISO_3166_COUNT_T         t_country_code;
#endif
}   TUNER_CAB_ANA_TUNE_INFO_T;

/* Digital cable setup structure */
/*------------------------------------------------------------------*/
/*! @struct TUNER_CAB_DIG_TUNE_INFO_T
 *  @brief The structure to detail the parameters for digital cable tuning.
 *  @code
 *  typedef struct _TUNER_CAB_DIG_TUNE_INFO_T
 *  {
 *      UINT32                   ui4_freq;
 *      UINT32                   ui4_sym_rate;
 *      TUNER_FEC_OUTER_T        e_fec_outer;
 *      TUNER_FEC_INNER_T        e_fec_inner;
 *      TUNER_MODULATION_T       e_mod;
 *      TUNER_CONNECTION_MODE_T  e_conn_mode;
 *      BOOL                     b_no_disc_nfy;
 *  }   TUNER_CAB_DIG_TUNE_INFO_T;
 *  @endcode
 *  @li@c  ui4_freq                                          - locked frequency
 *  @li@c  ui4_sym_rate                                      -
 *  @li@c  e_fec_outer                                       -
 *  @li@c  e_fec_inner                                       -
 *  @li@c  e_mod                                             -
 *  @li@c  e_conn_mode                                       -
 *  @li@c  b_no_disc_nfy                                     -
 */
/*------------------------------------------------------------------*/
typedef struct _TUNER_CAB_DIG_TUNE_INFO_T
{
    UINT32                   ui4_freq;
    UINT32                   ui4_sym_rate;
    TUNER_FEC_OUTER_T        e_fec_outer;
    TUNER_FEC_INNER_T        e_fec_inner;
    TUNER_MODULATION_T       e_mod;
    TUNER_CONNECTION_MODE_T  e_conn_mode;
    BOOL                     b_no_disc_nfy;
#ifdef MW_CM_SET_COUNTRY_TO_TUNER
	ISO_3166_COUNT_T         t_country_code;
#endif
}   TUNER_CAB_DIG_TUNE_INFO_T;

/* Digital cable OOB transmit setup structure */
/*------------------------------------------------------------------*/
/*! @struct TUNER_CAB_DIG_OOB_TX_TUNE_INFO_T
 *  @brief reserved for future use
 *  @code
 *  typedef struct _TUNER_CAB_DIG_OOB_TX_TUNE_INFO_T
 *  {
 *      UINT32                   ui4_freq;
 *      UINT32                   ui4_data_rate;
 *      TUNER_MODULATION_T       e_mod;
 *      TUNER_CONNECTION_MODE_T  e_conn_mode;
 *      INT16                    i2_tx_power;
 *      BOOL                     b_no_disc_nfy;
 *  }   TUNER_CAB_DIG_OOB_TX_TUNE_INFO_T;
 *  @endcode
 *  @li@c  ui4_freq                                          -
 *  @li@c  ui4_data_rate                                     -
 *  @li@c  e_mod                                             -
 *  @li@c  e_conn_mode                                       -
 *  @li@c  i2_tx_power                                       -
 *  @li@c  b_no_disc_nfy                                     -
 */
/*------------------------------------------------------------------*/
typedef struct _TUNER_CAB_DIG_OOB_TX_TUNE_INFO_T
{
    UINT32                   ui4_freq;
    UINT32                   ui4_data_rate;
    TUNER_MODULATION_T       e_mod;
    TUNER_CONNECTION_MODE_T  e_conn_mode;
    INT16                    i2_tx_power;
    BOOL                     b_no_disc_nfy;
}   TUNER_CAB_DIG_OOB_TX_TUNE_INFO_T;

/* Digital cable OOB receive setup structure */
/*------------------------------------------------------------------*/
/*! @struct TUNER_CAB_DIG_OOB_RX_TUNE_INFO_T
 *  @brief reserved for future use
 *  @code
 *  typedef struct _TUNER_CAB_DIG_OOB_RX_TUNE_INFO_T
 *  {
 *      UINT32                   ui4_freq;
 *      UINT32                   ui4_data_rate;
 *      TUNER_MODULATION_T       e_mod;
 *      TUNER_CONNECTION_MODE_T  e_conn_mode;
 *      BOOL                     b_spectrum_inv;
 *      BOOL                     b_no_disc_nfy;
 *  }   TUNER_CAB_DIG_OOB_RX_TUNE_INFO_T;
 *  @endcode
 *  @li@c  ui4_freq                                          -
 *  @li@c  ui4_data_rate                                     -
 *  @li@c  e_mod                                             -
 *  @li@c  e_conn_mode                                       -
 *  @li@c  b_spectrum_inv                                    -
 *  @li@c  b_no_disc_nfy                                     -
 */
/*------------------------------------------------------------------*/
typedef struct _TUNER_CAB_DIG_OOB_RX_TUNE_INFO_T
{
    UINT32                   ui4_freq;
    UINT32                   ui4_data_rate;
    TUNER_MODULATION_T       e_mod;
    TUNER_CONNECTION_MODE_T  e_conn_mode;
    BOOL                     b_spectrum_inv;
    BOOL                     b_no_disc_nfy;
}   TUNER_CAB_DIG_OOB_RX_TUNE_INFO_T;

/* Analog satellite setup structure */
/*------------------------------------------------------------------*/
/*! @struct TUNER_SAT_ANA_TUNE_INFO_T
 *  @brief reserved for future use.
 *  @code
 *  typedef struct _TUNER_SAT_ANA_TUNE_INFO_T
 *  {
 *      UINT32                   ui4_freq;
 *      UINT32                   ui4_freq_bound;
 *      UINT32                   ui4_central_freq;
 *      UINT32                   ui4_freq_offset;
 *      BOOL                     b_fine_tune;
 *      BOOL                     b_no_disc_nfy;
 *      INT16                    i2_fine_tune_offset;
 *  }   TUNER_SAT_ANA_TUNE_INFO_T;
 *  @endcode
 *  @li@c  ui4_freq                                          -
 *  @li@c  ui4_freq_bound                                    -
 *  @li@c  ui4_central_freq                                  -
 *  @li@c  ui4_freq_offset                                   -
 *  @li@c  b_fine_tune                                       -
 *  @li@c  b_no_disc_nfy                                     -
 *  @li@c  i2_fine_tune_offset                               -
 */
/*------------------------------------------------------------------*/
typedef struct _TUNER_SAT_ANA_TUNE_INFO_T
{
    UINT32                   ui4_freq;
    UINT32                   ui4_freq_bound;
    UINT32                   ui4_central_freq;
    UINT32                   ui4_freq_offset;
    BOOL                     b_fine_tune;
    BOOL                     b_no_disc_nfy;
    INT16                    i2_fine_tune_offset;
}   TUNER_SAT_ANA_TUNE_INFO_T;

/* Digital satellite setup structure */
/*------------------------------------------------------------------*/
/*! @struct TUNER_SAT_DIG_TUNE_INFO_T
 *  @brief reserved for future use.
 *  @code
 *  typedef struct _TUNER_SAT_DIG_TUNE_INFO_T
 *  {
 *      UINT32                   ui4_freq;
 *      UINT32                   ui4_sym_rate;
 *      TUNER_FEC_INNER_T        e_fec_inner;
 *      TUNER_MODULATION_T       e_mod;
 *      TUNER_POLARIZATION_T     e_pol;
 *      TUNER_CONNECTION_MODE_T  e_conn_mode;
 *      INT16                    i2_orb_pos;
 *      BOOL                     b_no_disc_nfy;
 *  }   TUNER_SAT_DIG_TUNE_INFO_T;
 *  @endcode
 *  @li@c  ui4_freq                                          -
 *  @li@c  ui4_sym_rate                                      -
 *  @li@c  e_fec_inner                                       -
 *  @li@c  e_mod                                             -
 *  @li@c  e_pol                                             -
 *  @li@c  e_conn_mode                                       -
 *  @li@c  i2_orb_pos                                        -
 *  @li@c  b_no_disc_nfy                                     -
 */
/*------------------------------------------------------------------*/
typedef struct _TUNER_SAT_DIG_TUNE_INFO_T
{
    UINT32                   ui4_freq;
    UINT32                   ui4_start_freq;
    UINT32                   ui4_end_freq;
    UINT32                   ui4_sym_rate;
    TUNER_FEC_INNER_T        e_fec_inner;
    TUNER_MODULATION_T       e_mod;
    TUNER_POLARIZATION_T     e_pol;
    TUNER_CONNECTION_MODE_T  e_conn_mode;
    INT16                    i2_orb_pos;
    BOOL                     b_no_disc_nfy;
    UINT32                   ui4_data;
}   TUNER_SAT_DIG_TUNE_INFO_T;

/* Analog terrestrial setup structure */
/*------------------------------------------------------------------*/
/*! @struct TUNER_TER_ANA_TUNE_INFO_T
 *  @brief The structure to detail the parameters for analog air tuning.
 *  @code
 *  typedef struct _TUNER_TER_ANA_TUNE_INFO_T
 *  {
 *      UINT32                   ui4_freq;
 *      UINT32                   ui4_freq_bound;
 *      UINT32                   ui4_central_freq;
 *      UINT32                   ui4_freq_offset;
 *      UINT32                   ui4_tv_sys_mask;
 *      TUNER_POLARIZATION_T     e_pol;      
 *      TUNER_MODULATION_T       e_mod;      
 *      UINT16                   ui2_dir;    
 *      UINT16                   ui2_gain;   
 *      BOOL                     b_fine_tune;
 *      BOOL                     b_no_disc_nfy;
 *      INT16                    i2_fine_tune_offset;
 *  }   TUNER_TER_ANA_TUNE_INFO_T;
 *  @endcode
 *  @li@c  ui4_freq                                          - locked frequency                              
 *  @li@c  ui4_freq_bound                                    - reserved                                      
 *  @li@c  ui4_central_freq                                  - central frequency of channel map              
 *  @li@c  ui4_freq_offset                                   - the bias offset of locked channel frequency   
 *  @li@c  ui4_tv_sys_mask                                   - tv system mask                                
 *  @li@c  e_pol                                             - internal use                            
 *  @li@c  e_mod                                             - modulation types     
 *  @li@c  ui2_dir                                           - internal use           
 *  @li@c  ui2_gain                                          - gain setting            
 *  @li@c  b_fine_tune                                       - request driver to do the autofine tune or not 
 *  @li@c  b_no_disc_nfy                                     - no notification when disconnected
 *  @li@c  i2_fine_tune_offset                               - the bias offset of user fine tune 
 */
/*------------------------------------------------------------------*/
typedef struct _TUNER_TER_ANA_TUNE_INFO_T
{
    UINT32                   ui4_freq;
    UINT32                   ui4_freq_bound;
    UINT32                   ui4_central_freq;
    UINT32                   ui4_freq_offset;
    UINT32                   ui4_tv_sys_mask;
    TUNER_POLARIZATION_T     e_pol;      /* Polarization. */
    TUNER_MODULATION_T       e_mod;      /* Modulation. */
    UINT16                   ui2_dir;    /* Direction in 0.1 degrees. Valid value range '0..3599' (inclusive). */
    UINT16                   ui2_gain;   /* Gain in 0.1 steps. Valid range '0..1000' (inclusive). */
    BOOL                     b_fine_tune;/* Apply fine-tune or not */
    BOOL                     b_no_disc_nfy;
    INT16                    i2_fine_tune_offset;
	/* DTMB New Add Variable Part */
	TUNER_CODRRATE_T         e_FecRate; 
	TUNER_CARRIER_MODE_T     e_CarrierMode; 
	TUNER_PN_SEQ_T           e_PnNumber;	
#ifdef MW_CM_SET_COUNTRY_TO_TUNER
    ISO_3166_COUNT_T         t_country_code;
#endif
}   TUNER_TER_ANA_TUNE_INFO_T;

/* Digital terrestrial setup structure */
/*------------------------------------------------------------------*/
/*! @struct TUNER_TER_DIG_TUNE_INFO_T
 *  @brief The structure to detail the parameters for digital air tuning.
 *  @code
 *  typedef struct _TUNER_TER_DIG_TUNE_INFO_T
 *  {
 *      UINT32                      ui4_freq;
 *      TUNER_BANDWIDTH_T           e_bandwidth;
 *      TUNER_FEC_OUTER_T           e_fec_outer;
 *      TUNER_FEC_INNER_T           e_fec_inner;
 *      TUNER_MODULATION_T          e_mod;
 *      TUNER_HIERARCHY_T           e_hierarchy;
 *      TUNER_HIERARCHY_PRIORITY_T  e_hierarchy_priority;
 *      TUNER_GUARD_INTERVAL_T      e_guard_int;
 *      TUNER_TRANSMISSION_MODE_T   e_trans_mode;
 *      TUNER_TIMER_INTERLEAVING_T  e_timer_int;
 *      BOOL                        b_other_freq;
 *      TUNER_POLARIZATION_T        e_pol;         
 *      TUNER_CONNECTION_MODE_T     e_conn_mode;
 *      UINT16                      ui2_dir;       
 *      UINT16                      ui2_gain;      
 *      BOOL                        b_no_disc_nfy;
 *  }   TUNER_TER_DIG_TUNE_INFO_T;
 *  @endcode
 *  @li@c  ui4_freq                                          - locked frequency
 *  @li@c  e_bandwidth                                       - internal use
 *  @li@c  e_fec_outer                                       - internal use
 *  @li@c  e_fec_inner                                       - internal use
 *  @li@c  e_mod                                             - modulation types 
 *  @li@c  e_hierarchy                                       - internal use
 *  @li@c  e_hierarchy_priority                              - internal use
 *  @li@c  e_guard_int                                       - internal use
 *  @li@c  e_trans_mode                                      - internal use
 *  @li@c  e_timer_int                                       - internal use
 *  @li@c  b_other_freq                                      - internal use
 *  @li@c  e_pol                                             - internal use
 *  @li@c  e_conn_mode                                       - internal use
 *  @li@c  ui2_dir                                           - internal use
 *  @li@c  ui2_gain                                          - internal use
 *  @li@c  b_no_disc_nfy                                     - no notification when disconnected
 */
/*------------------------------------------------------------------*/
typedef struct _TUNER_TER_DIG_TUNE_INFO_T
{
    UINT32                      ui4_freq;
    TUNER_BANDWIDTH_T           e_bandwidth;
    TUNER_FEC_OUTER_T           e_fec_outer;
    TUNER_FEC_INNER_T           e_fec_inner;
    TUNER_MODULATION_T          e_mod;
    TUNER_HIERARCHY_T           e_hierarchy;
    TUNER_HIERARCHY_PRIORITY_T  e_hierarchy_priority;
    TUNER_GUARD_INTERVAL_T      e_guard_int; 
    TUNER_TRANSMISSION_MODE_T   e_trans_mode;
    TUNER_TIMER_INTERLEAVING_T  e_timer_int; 
    BOOL                        b_other_freq;
    TUNER_POLARIZATION_T        e_pol;         /* Polarization. */
    TUNER_CONNECTION_MODE_T     e_conn_mode;
    UINT16                      ui2_dir;       /* Direction in 0.1 degrees. Valid value range '0..3599' (inclusive). */
    UINT16                      ui2_gain;      /* Gain in 0.1 steps. Valid range '0..1000' (inclusive). */
    BOOL                        b_no_disc_nfy;
	/* DTMB New Add Variable Part */	
	TUNER_CODRRATE_T            e_FecRate; 
	TUNER_CARRIER_MODE_T        e_CarrierMode; 
	TUNER_PN_SEQ_T             	e_PnNumber;	
	UINT32                      ui4_NCO;
	/* DVB-T/T2 related */
    TUNER_DVB_T_T2_MODE_T       e_dvb_t_t2_mode;
    UINT8                       ui1_plp_id; /*This 8-bit field uniquely identifies a data PLP
     within a T2 system. within a T2 network. the term is defined in EN 302 755*/
    UINT16                      ui2_t2_sys_id;
#ifdef MW_CM_SET_COUNTRY_TO_TUNER
	ISO_3166_COUNT_T            t_country_code;
#endif
}   TUNER_TER_DIG_TUNE_INFO_T;

#ifdef MW_DVBS_DEV_ENABLE
/* Command structure to tuner driver */
typedef struct
{
    /*UINT8                   ui1_framing;
    UINT8                   ui1_address;*/  /* add framing and address into aui1_cmd[] */
    UINT8                   aui1_cmd[8];    /* command + command bytes */
    UINT8                   ui1_cmd_len;    /* valid length of aui2_cmd */
    BOOL                    b_tone_burst;   /* 0->tone burst0, 1->tone burst1 */
    UINT8                   ui1_port;       /* For tuner driver use ,no use at preasent*/
    UINT8                   ui1_repeat_cmd; /* For tuner driver use ,,no use at preasent*/
    BOOL                    b_22k;                  /* 0-> 22k off,1->22k on */
    UINT8                   ui1_polarity_13v_18v;   /* 0->V(13v) ,1->H(18v)*/
    UINT8                   ui1_lnb_power;          /* 0->LNB_POWER_OFF, 1->LNB_POWER_13V_18V, 2->LNB_POWER_14V_19V*/
	UINT8                   ui1_control_mask;   
    /* control mask:bit'0'-control H/V; bit'1'-control 22k;bit'2'-control burst;bit'3'-send cmd*/
}   DISEQC_BUS_CMD_T;



#define TUNER_DISEQC_SAT_POSITION_1         ((UINT8)0)
#define TUNER_DISEQC_SAT_POSITION_2         ((UINT8)1)
#define TUNER_DISEQC_SAT_POSITION_3         ((UINT8)2)
#define TUNER_DISEQC_SAT_POSITION_4         ((UINT8)3)
#define TUNER_DISEQC_SAT_POSITION_DISABLE   ((UINT8)0xFF)

#define TUNER_DISEQC_22K_OFF    ((UINT8)0)
#define TUNER_DISEQC_22K_ON     ((UINT8)1)
#define TUNER_DISEQC_22K_AUTO   ((UINT8)2)

#define TUNER_DISEQC_TONE_BURST_0     ((UINT8)0)
#define TUNER_DISEQC_TONE_BURST_1     ((UINT8)1)


/* Control mask of diseqc bus command */
#define DISEQC_CMD_CTRL_POLARIZATION_MASK                     ((UINT8)0x01)
#define DISEQC_CMD_CTRL_22K_MASK                              ((UINT8)0x02)
#define DISEQC_CMD_CTRL_TONE_BURST_MASK                       ((UINT8)0x04)
#define DISEQC_CMD_CTRL_FULL_CMD_MASK                         ((UINT8)0x08)
#define DISEQC_CMD_CTRL_PORT_MASK                             ((UINT8)0x10)
#define DISEQC_CMD_CTRL_REPEAT_CMD_MASK                       ((UINT8)0x20)
#define DISEQC_CMD_CTRL_LNB_POWER_MASK                        ((UINT8)0x40)

/* <FRAMING><ADDRESS><COMMAND><DATA> */
/* <Framing  - Master side> */
#define DISEQC_FRAMING_1ST_TRANSMIT_NO_REPLY                  (0xE0)
#define DISEQC_FRAMING_RPT_TRANSMIT_NO_REPLY                  (0xE1)
#define DISEQC_FRAMING_1ST_TRANSMIT_REQ_REPLY                 (0xE2)
#define DISEQC_FRAMING_RPT_TRANSMIT_REQ_REPLY                 (0xE3)

/* <Framing - Slave side> */
#define DISEQC_FRAMING_SLV_OK_NO_ERROR                        (0xE4)
#define DISEQC_FRAMING_SLV_ERR_CMD_NOT_SUPPORTED              (0xE5)
#define DISEQC_FRAMING_SLV_ERR_PARITY_DETECTED_REQ_REPEAT     (0xE6)
#define DISEQC_FRAMING_SLV_ERR_CMD_NOT_RECOGNIZED_REQ_REPEAT  (0xE7)

/* <Address> */
#define DISEQC_ADDR_ANY_LNB_SWITCHER_SMATV                    (0x10)
#define DISEQC_ADDR_POLAR_POSITIONER                          (0x31)
#define DISEQC_ADDR_SUBSCRIBER_HEADENDS                       (0x71)

/* <Command> */
typedef enum
{
    /* Command - DiSEqC 1.0 */
    DISEQC_10_CMD_RESET_MICROCONTROLLER = 0x00, 
    DISEQC_10_CMD_SET_WRITE_TO_PORT_GRP_0 = 0x38,

    /* Command - DiSEqC 1.1 */
    DISEQC_11_CMD_SET_WRITE_TO_PORT_GRP_1 = 0x39,              /* Not implemented in reference */
    DISEQC_11_CMD_SET_WRITE_CHANNEL_FREQ = 0x58, 

    /* Command - DiSEqC 1.2 */
    DISEQC_12_CMD_SET_STOP_POSITIONER_MOVEMENT = 0x60,
    DISEQC_12_CMD_SET_DISABLE_LIMITS = 0x63,
    DISEQC_12_CMD_SET_EAST_LIMIT_AND_ENABLE_LIMITS = 0x66,
    DISEQC_12_CMD_SET_WEST_LIMIT_AND_ENABLE_LIMITS = 0x67,
    DISEQC_12_CMD_SET_DRIVE_MOTOR_EAST_AND_TIMEOUT_AND_STEPS = 0x68,
    DISEQC_12_CMD_SET_DRIVE_MOTOR_WEST_AND_TIMEOUT_AND_STEPS = 0x69,
    DISEQC_12_CMD_SET_STORE_SAT_POSITION_AND_ENABLE_LIMITS = 0x6A,
    DISEQC_12_CMD_SET_DRIVE_MOTOR_TO_SAT_POSITION_NN = 0x6B
}   DISEQC_COMMAND_T;
#endif

/*----------------------------------------------------------------------------*/
/*! @} */                                          /* end of groupMW_RES_MNGR */
/*----------------------------------------------------------------------------*/
#endif /* _U_TUNER_H_ */

