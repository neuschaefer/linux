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
/*! @file u_drv_cust.h
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *
 *  @par Description:
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_RES_MNGR_DRV_CUST D Interface
 *  @ingroup  groupMW_RES_MNGR
 *  @brief    The u_drv_cust.h module defined D interface is exported for AP to
 *            set driver parameters.
 *
 *  @see groupMW
 *  @{
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_DRV_CUST_H_
#define _U_DRV_CUST_H_

/*-----------------------------------------------------------------------------
                    include files
 -----------------------------------------------------------------------------*/
#include "inc/x_common.h"

#ifdef TARGET_SPEC_DRV_CUST_CUSTOMIZED_HDR_FILE
#include TARGET_SPEC_DRV_CUST_CUSTOMIZED_HDR_FILE
#endif

/*-----------------------------------------------------------------------------
    <<General>>  macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

#define DRV_CUST_COMP_ID                                           ((UINT16) 0)    /**< ID of the cust drv component */

#define DRV_CUSTOM_ARG_NO_REF_OFFSET                      ((UINT32) 0x00010000)    /**<        */
#define DRV_CUSTOM_ARG_USE_REF_OFFSET                     ((UINT32) 0x00020000)    /**<        */
#define DRV_CUSTOM_ARG_END_OFFSET                         ((UINT32) 0x000f0000)    /**<        */

/*-----------------------------------------------------------------------------
    <<Audio driver>>  macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

#define DRV_CUSTOM_AUD_INP_FLAG_NONE                        MAKE_BIT_MASK_32(0)    /**<        */
#define DRV_CUSTOM_AUD_INP_FLAG_ATV                         MAKE_BIT_MASK_32(1)    /**<        */
#define DRV_CUSTOM_AUD_INP_FLAG_DTV                         MAKE_BIT_MASK_32(2)    /**<        */
#define DRV_CUSTOM_AUD_INP_FLAG_YPBPR                       MAKE_BIT_MASK_32(3)    /**<        */
#define DRV_CUSTOM_AUD_INP_FLAG_VGA                         MAKE_BIT_MASK_32(4)    /**<        */
#define DRV_CUSTOM_AUD_INP_FLAG_HDMI                        MAKE_BIT_MASK_32(5)    /**<        */
#define DRV_CUSTOM_AUD_INP_FLAG_DVI                         MAKE_BIT_MASK_32(6)    /**<        */
#define DRV_CUSTOM_AUD_INP_FLAG_DVD                         MAKE_BIT_MASK_32(7)    /**<        */
#define DRV_CUSTOM_AUD_INP_FLAG_MM                          MAKE_BIT_MASK_32(8)    /**<        */

/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      DRV_CUSTOM_AUD_TYPE_BEGIN = 0,
 *      DRV_CUSTOM_AUD_TYPE_HEADPHONE_VOL = DRV_CUSTOM_ARG_NO_REF_OFFSET,
 *      DRV_CUSTOM_AUD_TYPE_MTS_NUM_CHECK,
 *      DRV_CUSTOM_AUD_TYPE_MTS_NUM_PILOT,
 *      DRV_CUSTOM_AUD_TYPE_MTS_PILOT_HIGH,
 *      DRV_CUSTOM_AUD_TYPE_MTS_PILOT_LOW,
 *      DRV_CUSTOM_AUD_TYPE_MTS_NUM_SAP,
 *      DRV_CUSTOM_AUD_TYPE_MTS_SAP_HIGH,
 *      DRV_CUSTOM_AUD_TYPE_MTS_SAP_LOW,
 *      DRV_CUSTOM_AUD_TYPE_MTS_HDEV,
 *      DRV_CUSTOM_AUD_TYPE_MTS_CARRIER_SHIFT,
 *      DRV_CUSTOM_AUD_TYPE_MTS_FM_SATU,
 *      DRV_CUSTOM_AUD_TYPE_MTS_FM_SATU_HIGH,
 *      DRV_CUSTOM_AUD_TYPE_MTS_FM_SATU_LOW,
 *      DRV_CUSTOM_AUD_TYPE_MTS_FM_MUTE,
 *      DRV_CUSTOM_AUD_TYPE_MTS_FM_MUTE_HIGH,
 *      DRV_CUSTOM_AUD_TYPE_MTS_FM_MUTE_LOW,
 *      DRV_CUSTOM_AUD_TYPE_MTS_FINE_VOLUME,
 *      DRV_CUSTOM_AUD_TYPE_SAP_FINE_VOLUME,
 *      DRV_CUSTOM_AUD_TYPE_MTS_PILOT_OFFSET,
 *      DRV_CUSTOM_AUD_TYPE_SAP_MUTE_LOW,
 *      DRV_CUSTOM_AUD_TYPE_SAP_MUTE_HIGH,
 *      DRV_CUSTOM_AUD_TYPE_A2_NUM_CHECK,
 *      DRV_CUSTOM_AUD_TYPE_A2_NUM_DOUBLE,
 *      DRV_CUSTOM_AUD_TYPE_A2_MONO_WEIGHT,
 *      DRV_CUSTOM_AUD_TYPE_A2_STEREO_WEIGHT,
 *      DRV_CUSTOM_AUD_TYPE_A2_DUAL_WEIGHT,
 *      DRV_CUSTOM_AUD_TYPE_A2_HDEV,
 *      DRV_CUSTOM_AUD_TYPE_A2_CARRIER_SHIFT,
 *      DRV_CUSTOM_AUD_TYPE_A2_FM_MUTE,
 *      DRV_CUSTOM_AUD_TYPE_A2_FM_MUTE_HIGH,
 *      DRV_CUSTOM_AUD_TYPE_A2_FM_MUTE_LOW,
 *      DRV_CUSTOM_AUD_TYPE_A2_FINE_VOLUME,
 *      DRV_CUSTOM_AUD_TYPE_PAL_CORRECT,
 *      DRV_CUSTOM_AUD_TYPE_PAL_SYNC_LOOP,
 *      DRV_CUSTOM_AUD_TYPE_PAL_ERROR,
 *      DRV_CUSTOM_AUD_TYPE_PAL_PARITY_ERROR,
 *      DRV_CUSTOM_AUD_TYPE_PAL_NUM_FRAMES,
 *      DRV_CUSTOM_AUD_TYPE_PAL_HDEV,
 *      DRV_CUSTOM_AUD_TYPE_PAL_AM_MUTE,
 *      DRV_CUSTOM_AUD_TYPE_PAL_AM_MUTE_HIGH,
 *      DRV_CUSTOM_AUD_TYPE_PAL_AM_MUTE_LOW,
 *      DRV_CUSTOM_AUD_TYPE_PAL_CARRIER_SHIFT,
 *      DRV_CUSTOM_AUD_TYPE_PAL_FM_MUTE,
 *      DRV_CUSTOM_AUD_TYPE_PAL_FM_MUTE_HIGH,
 *      DRV_CUSTOM_AUD_TYPE_PAL_FM_MUTE_LOW,
 *      DRV_CUSTOM_AUD_TYPE_PAL_FINE_VOLUME,
 *      DRV_CUSTOM_AUD_TYPE_AM_FINE_VOLUME,
 *      DRV_CUSTOM_AUD_TYPE_NICAM_FINE_VOLUME,
 *      DRV_CUSTOM_AUD_TYPE_FM_SATU_MUTE,
 *      DRV_CUSTOM_AUD_TYPE_NON_EU,
 *      DRV_CUSTOM_AUD_TYPE_BBE_PRESCALE_LEVEL,
 *      DRV_CUSTOM_AUD_TYPE_BBE_PROCESSS,
 *      DRV_CUSTOM_AUD_TYPE_BBE_LO_CONTOUR,
 *      DRV_CUSTOM_AUD_TYPE_BBE_3D_GAIN,
 *      DRV_CUSTOM_AUD_TYPE_VOL_GAIN = DRV_CUSTOM_ARG_USE_REF_OFFSET,
 *      DRV_CUSTOM_AUD_TYPE_AVL_GAIN,
 *      DRV_CUSTOM_AUD_TYPE_SPEAKER_SIZE,
 *      DRV_CUSTOM_AUD_TYPE_SPEAKER_TYPE,
 *      DRV_CUSTOM_AUD_TYPE_SPECTRUM_INFO,
 *      DRV_CUSTOM_AUD_TYPE_TEST_TONE,
 *      DRV_CUSTOM_AUD_TYPE_CUT_OFF_FREQ,
 *      DRV_CUSTOM_AUD_TYPE_LOUDNESS,
 *      DRV_CUSTOM_AUD_TYPE_SMART_AUDIO,
 *      DRV_CUSTOM_AUD_VOL_CURVE,
 *      DRV_CUSTOM_AUD_HEADPHONE_MODE,
 *      DRV_CUSTOM_AUD_TYPE_DD_CONTROL,
 *      DRV_CUSTOM_AUD_TYPE_AQ_VER,
 *      DRV_CUSTOM_AUD_TYPE_BASS_BOOSTER,
 *      DRV_CUSTOM_AUD_TYPE_END = DRV_CUSTOM_ARG_END_OFFSET
 *  } DRV_CUSTOM_AUD_TYPE_T;
 *  @endcode
 *  @li@c  DRV_CUSTOM_AUD_TYPE_BEGIN                         -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_HEADPHONE_VOL                 -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_MTS_NUM_CHECK                 -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_MTS_NUM_PILOT                 -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_MTS_PILOT_HIGH                -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_MTS_PILOT_LOW                 -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_MTS_NUM_SAP                   -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_MTS_SAP_HIGH                  -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_MTS_SAP_LOW                   -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_MTS_HDEV                      -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_MTS_CARRIER_SHIFT             -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_MTS_FM_SATU                   -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_MTS_FM_SATU_HIGH              -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_MTS_FM_SATU_LOW               -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_MTS_FM_MUTE                   -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_MTS_FM_MUTE_HIGH              -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_MTS_FM_MUTE_LOW               -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_MTS_FINE_VOLUME               -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_SAP_FINE_VOLUME               -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_MTS_PILOT_OFFSET              -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_SAP_MUTE_LOW                  -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_SAP_MUTE_HIGH                 -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_A2_NUM_CHECK                  -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_A2_NUM_DOUBLE                 -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_A2_MONO_WEIGHT                -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_A2_STEREO_WEIGHT              -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_A2_DUAL_WEIGHT                -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_A2_HDEV                       -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_A2_CARRIER_SHIFT              -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_A2_FM_MUTE                    -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_A2_FM_MUTE_HIGH               -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_A2_FM_MUTE_LOW                -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_A2_FINE_VOLUME                -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_PAL_CORRECT                   -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_PAL_SYNC_LOOP                 -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_PAL_ERROR                     -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_PAL_PARITY_ERROR              -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_PAL_NUM_FRAMES                -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_PAL_HDEV                      -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_PAL_AM_MUTE                   -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_PAL_AM_MUTE_HIGH              -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_PAL_AM_MUTE_LOW               -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_PAL_CARRIER_SHIFT             -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_PAL_FM_MUTE                   -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_PAL_FM_MUTE_HIGH              -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_PAL_FM_MUTE_LOW               -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_PAL_FINE_VOLUME               -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_AM_FINE_VOLUME                -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_NICAM_FINE_VOLUME             -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_FM_SATU_MUTE                  -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_NON_EU                        -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_BBE_PRESCALE_LEVEL            -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_BBE_PROCESSS                  -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_BBE_LO_CONTOUR                -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_BBE_3D_GAIN                   -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_VOL_GAIN                      -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_AVL_GAIN                      -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_SPEAKER_SIZE                  -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_SPEAKER_TYPE                  -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_SPECTRUM_INFO                 -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_TEST_TONE                     -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_CUT_OFF_FREQ                  -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_LOUDNESS                      -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_SMART_AUDIO                   -
 *  @li@c  DRV_CUSTOM_AUD_VOL_CURVE                          -
 *  @li@c  DRV_CUSTOM_AUD_HEADPHONE_MODE                     -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_DD_CONTROL                    -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_AQ_VER                        -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_BASS_BOOSTER                  -
 *  @li@c  DRV_CUSTOM_AUD_TYPE_END                           -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DRV_CUSTOM_AUD_TYPE_BEGIN = 0,

    /*
     * The argument of the following enumerations SHOULD NOT be a pointer
     */

    DRV_CUSTOM_AUD_TYPE_HEADPHONE_VOL = DRV_CUSTOM_ARG_NO_REF_OFFSET,    /* INT16, Head phone volume */

    /* MTS FACTORY MODE */
    DRV_CUSTOM_AUD_TYPE_MTS_NUM_CHECK,                                   /* UINT16, numbers of check */
    DRV_CUSTOM_AUD_TYPE_MTS_NUM_PILOT,                                   /* UINT16, numbers of pilot */
    DRV_CUSTOM_AUD_TYPE_MTS_PILOT_HIGH,                                  /* UINT16, pilot higher threshold */
    DRV_CUSTOM_AUD_TYPE_MTS_PILOT_LOW,                                   /* UINT16, pilot lower threshold */
    DRV_CUSTOM_AUD_TYPE_MTS_NUM_SAP,                                     /* UINT16, numbers of sap */
    DRV_CUSTOM_AUD_TYPE_MTS_SAP_HIGH,                                    /* UINT16, sap higher threshold */
    DRV_CUSTOM_AUD_TYPE_MTS_SAP_LOW,                                     /* UINT16, sap lower threshold */
    DRV_CUSTOM_AUD_TYPE_MTS_HDEV,                                        /* BOOL, high deviation mode on/off */
    DRV_CUSTOM_AUD_TYPE_MTS_CARRIER_SHIFT,                               /* BOOL, carrier shift mode on/off */
    DRV_CUSTOM_AUD_TYPE_MTS_FM_SATU,                                     /* BOOL, fm saturation mute mode on/off */
    DRV_CUSTOM_AUD_TYPE_MTS_FM_SATU_HIGH,                                /* UINT8, fm saturation mute mode on/off */
    DRV_CUSTOM_AUD_TYPE_MTS_FM_SATU_LOW,                                 /* UINT8, fm saturation mute mode on/off */
    DRV_CUSTOM_AUD_TYPE_MTS_FM_MUTE,                                     /* BOOL, carrier sense mute mode on/off */
    DRV_CUSTOM_AUD_TYPE_MTS_FM_MUTE_HIGH,                                /* UINT8, higher carrier sense mute threshold */
    DRV_CUSTOM_AUD_TYPE_MTS_FM_MUTE_LOW,                                 /* UINT8, lower carrier sense mute threshold */
    DRV_CUSTOM_AUD_TYPE_MTS_FINE_VOLUME,                                 /* UINT8, fine tune volume for mono and stereo */
    DRV_CUSTOM_AUD_TYPE_SAP_FINE_VOLUME,                                 /* UINT8, fine tune volume for sap */
    DRV_CUSTOM_AUD_TYPE_MTS_PILOT_OFFSET,                                /* BOOL, pilot shift calibration mode on/off */
    DRV_CUSTOM_AUD_TYPE_SAP_MUTE_LOW,                                    /* UINT8, sap mute lower threshold */
    DRV_CUSTOM_AUD_TYPE_SAP_MUTE_HIGH,                                   /* UINT8, sap mute higher threshold */

    /* A2 FACTORY MODE */
    DRV_CUSTOM_AUD_TYPE_A2_NUM_CHECK,                                    /* UINT16, numbers of check */
    DRV_CUSTOM_AUD_TYPE_A2_NUM_DOUBLE,                                   /* UINT16, numbers of double check */
    DRV_CUSTOM_AUD_TYPE_A2_MONO_WEIGHT,                                  /* UINT16, mono wieght */
    DRV_CUSTOM_AUD_TYPE_A2_STEREO_WEIGHT,                                /* UINT16, stereo wieght */
    DRV_CUSTOM_AUD_TYPE_A2_DUAL_WEIGHT,                                  /* UINT16, dual wieght */
    DRV_CUSTOM_AUD_TYPE_A2_HDEV,                                         /* BOOL, high deviation mode on/off */
    DRV_CUSTOM_AUD_TYPE_A2_CARRIER_SHIFT,                                /* BOOL, carrier shift mode on/off */
    DRV_CUSTOM_AUD_TYPE_A2_FM_MUTE,                                      /* UINT8, carrier sense mute mode on/off */
    DRV_CUSTOM_AUD_TYPE_A2_FM_MUTE_HIGH,                                 /* UINT8, higher carrier sense mute threshold */
    DRV_CUSTOM_AUD_TYPE_A2_FM_MUTE_LOW,                                  /* UINT8, lower carrier sense mute threshold */
    DRV_CUSTOM_AUD_TYPE_A2_FINE_VOLUME,                                  /* UINT8, fine tune volume */

    /* PAL FACTORY MODE */
    DRV_CUSTOM_AUD_TYPE_PAL_CORRECT,                                     /* UINT16, correct threshold */
    DRV_CUSTOM_AUD_TYPE_PAL_SYNC_LOOP,                                   /* UINT16, total sync loop */
    DRV_CUSTOM_AUD_TYPE_PAL_ERROR,                                       /* UINT16, error threshold */
    DRV_CUSTOM_AUD_TYPE_PAL_PARITY_ERROR,                                /* UINT16, parity error threshold */
    DRV_CUSTOM_AUD_TYPE_PAL_NUM_FRAMES,                                  /* UINT16, every number frames */
    DRV_CUSTOM_AUD_TYPE_PAL_HDEV,                                        /* BOOL, high deviation mode on/off */
    DRV_CUSTOM_AUD_TYPE_PAL_AM_MUTE,                                     /* UINT8, AM carrier sense mute mode on/off */
    DRV_CUSTOM_AUD_TYPE_PAL_AM_MUTE_HIGH,                                /* UINT8, AM higher carrier sense mute threshold */
    DRV_CUSTOM_AUD_TYPE_PAL_AM_MUTE_LOW,                                 /* UINT8, AM lower carrier sense mute threshold */
    DRV_CUSTOM_AUD_TYPE_PAL_CARRIER_SHIFT,                               /* BOOL, carrier shift mode on/off */
    DRV_CUSTOM_AUD_TYPE_PAL_FM_MUTE,                                     /* UINT8, FM carrier sense mute mode on/off */
    DRV_CUSTOM_AUD_TYPE_PAL_FM_MUTE_HIGH,                                /* UINT8, FM higher carrier sense mute threshold */
    DRV_CUSTOM_AUD_TYPE_PAL_FM_MUTE_LOW,                                 /* UINT8, FM lower carrier sense mute threshold */
    DRV_CUSTOM_AUD_TYPE_PAL_FINE_VOLUME,                                 /* UINT8, fine tune volume */
    DRV_CUSTOM_AUD_TYPE_AM_FINE_VOLUME,                                  /* UINT8, fine tune volume */
    DRV_CUSTOM_AUD_TYPE_NICAM_FINE_VOLUME,                               /* UINT8, fine tune volume */

    /* EU FACTORY MODE */
    DRV_CUSTOM_AUD_TYPE_FM_SATU_MUTE,                                    /* BOOL, fm saturation mute mode on/off */
    DRV_CUSTOM_AUD_TYPE_NON_EU,                                          /* BOOL, non-EU mode on/off */

    /* BBE FACTORY MODE */
    DRV_CUSTOM_AUD_TYPE_BBE_PRESCALE_LEVEL,                              /* INT8, prescale level */
    DRV_CUSTOM_AUD_TYPE_BBE_PROCESSS,                                    /* UINT8, BBE process */
    DRV_CUSTOM_AUD_TYPE_BBE_LO_CONTOUR,                                  /* UINT8, Lo contour */
    DRV_CUSTOM_AUD_TYPE_BBE_3D_GAIN,                                     /* UINT8, 3D gain */

	/* DTS ENDIAN MODE */
	DRV_CUSTOM_AUD_TYPE_DTS_ENDIAN,

    /*
     * The argument of the following enumerations SHOULD BE a pointer
     */

    DRV_CUSTOM_AUD_TYPE_VOL_GAIN = DRV_CUSTOM_ARG_USE_REF_OFFSET,        /* AUD_TYPE_VOL_GAIN_INFO_T, Volume (by source) */
    DRV_CUSTOM_AUD_TYPE_AVL_GAIN,                                        /* AUD_TYPE_AVL_GAIN_INFO_T, AVL setting */
    DRV_CUSTOM_AUD_TYPE_SPEAKER_SIZE,                                    /* DRV_CUSTOM_AUD_SPK_SIZE_T, Speaker size, large or small */
    DRV_CUSTOM_AUD_TYPE_SPEAKER_TYPE,                                    /* DRV_CUSTOM_AUD_SPK_TYPE_T, Speaker type. depend on customer's definition */
    DRV_CUSTOM_AUD_TYPE_SPECTRUM_INFO,                                   /* AUD_TYPE_SPECTRUM_INFO_T, Spectrum Info */
    DRV_CUSTOM_AUD_TYPE_TEST_TONE,                                       /* DRV_CUSTOM_AUD_TYPE_TEST_TONE, Test Tone Setting */
    DRV_CUSTOM_AUD_TYPE_CUT_OFF_FREQ,                                    /* DRV_CUSTOM_AUD_CUT_OFF_FREQ_T, cut-off frequency */
    DRV_CUSTOM_AUD_TYPE_LOUDNESS,                                        /* DRV_CUSTOM_AUD_LOUDNESS_T, loudness */
    DRV_CUSTOM_AUD_TYPE_SMART_AUDIO,                                     /* SMART_AUD_GROUP_INFO_T, Smart Audio */
    DRV_CUSTOM_AUD_VOL_CURVE,                                            /* DRV_CUSTOM_AUD_VOL_CURVE_T, Volume Curve */
    DRV_CUSTOM_AUD_HEADPHONE_MODE,                                       /* DRV_CUSTOM_AUD_HEADPHONE_MODE_T, headphone mode */
    DRV_CUSTOM_AUD_TYPE_DD_CONTROL,                                      /* AUD_TYPE_DD_CONTROL_T, dolby digital control */
    DRV_CUSTOM_AUD_TYPE_AQ_VER,                                          /* DRV_CUSTOM_AUD_TYPE_AQ_VER_T, AQ version */
    DRV_CUSTOM_AUD_TYPE_BASS_BOOSTER,                                    /* AUD_TYPE_BASS_BOOSTER, Bass Booster (Virtual Bass) */

    DRV_CUSTOM_AUD_TYPE_END = DRV_CUSTOM_ARG_END_OFFSET
} DRV_CUSTOM_AUD_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      DRV_CUSTOM_AUD_AVL_TARGET_LEVEL,
 *      DRV_CUSTOM_AUD_AVL_THRESHOLD,
 *      DRV_CUSTOM_AUD_AVL_ADJUST_RATE
 *  } DRV_CUSTOM_AVL_ADJ_T;
 *  @endcode
 *  @li@c  DRV_CUSTOM_AUD_AVL_TARGET_LEVEL                   -
 *  @li@c  DRV_CUSTOM_AUD_AVL_THRESHOLD                      -
 *  @li@c  DRV_CUSTOM_AUD_AVL_ADJUST_RATE                    -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DRV_CUSTOM_AUD_AVL_TARGET_LEVEL,
    DRV_CUSTOM_AUD_AVL_THRESHOLD,
    DRV_CUSTOM_AUD_AVL_ADJUST_RATE
} DRV_CUSTOM_AVL_ADJ_T;

/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      DRV_CUSTOM_AUD_SPK_LARGE,
 *      DRV_CUSTOM_AUD_SPK_SMALL
 *  } DRV_CUSTOM_AUD_SPK_SIZE_T;
 *  @endcode
 *  @li@c  DRV_CUSTOM_AUD_SPK_LARGE                          -
 *  @li@c  DRV_CUSTOM_AUD_SPK_SMALL                          -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DRV_CUSTOM_AUD_SPK_LARGE,
    DRV_CUSTOM_AUD_SPK_SMALL
} DRV_CUSTOM_AUD_SPK_SIZE_T;

/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      DRV_CUSTOM_AUD_SPK_TYPE_A = 0,
 *      DRV_CUSTOM_AUD_SPK_TYPE_B
 *  } DRV_CUSTOM_AUD_SPK_TYPE_T;
 *  @endcode
 *  @li@c  DRV_CUSTOM_AUD_SPK_TYPE_A                         -
 *  @li@c  DRV_CUSTOM_AUD_SPK_TYPE_B                         -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DRV_CUSTOM_AUD_SPK_TYPE_A = 0,
    DRV_CUSTOM_AUD_SPK_TYPE_B
} DRV_CUSTOM_AUD_SPK_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      DRV_CUSTOM_AUD_CUT_OFF_70  = 70,
 *      DRV_CUSTOM_AUD_CUT_OFF_100 = 100,
 *      DRV_CUSTOM_AUD_CUT_OFF_200 = 200,
 *      DRV_CUSTOM_AUD_CUT_OFF_300 = 300
 *  } DRV_CUSTOM_AUD_CUT_OFF_FREQ_T;
 *  @endcode
 *  @li@c  DRV_CUSTOM_AUD_CUT_OFF_70                         -
 *  @li@c  DRV_CUSTOM_AUD_CUT_OFF_100                        -
 *  @li@c  DRV_CUSTOM_AUD_CUT_OFF_200                        -
 *  @li@c  DRV_CUSTOM_AUD_CUT_OFF_300                        -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DRV_CUSTOM_AUD_CUT_OFF_70  = 70,
    DRV_CUSTOM_AUD_CUT_OFF_100 = 100,
    DRV_CUSTOM_AUD_CUT_OFF_200 = 200,
    DRV_CUSTOM_AUD_CUT_OFF_300 = 300
} DRV_CUSTOM_AUD_CUT_OFF_FREQ_T;

/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      DRV_CUSTOM_AUD_LOUDNESS_OFF = 0,
 *      DRV_CUSTOM_AUD_LOUDNESS_ON
 *  } DRV_CUSTOM_AUD_LOUDNESS_T;
 *  @endcode
 *  @li@c  DRV_CUSTOM_AUD_LOUDNESS_OFF                       -
 *  @li@c  DRV_CUSTOM_AUD_LOUDNESS_ON                        -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DRV_CUSTOM_AUD_LOUDNESS_OFF = 0,
    DRV_CUSTOM_AUD_LOUDNESS_ON
} DRV_CUSTOM_AUD_LOUDNESS_T;

/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      DRV_CUSTOM_AUD_HEADPHONE_MODE_A = 0,
 *      DRV_CUSTOM_AUD_HEADPHONE_MODE_B,
 *      DRV_CUSTOM_AUD_HEADPHONE_MODE_MAX
 *  } DRV_CUSTOM_AUD_HEADPHONE_MODE_T;
 *  @endcode
 *  @li@c  DRV_CUSTOM_AUD_HEADPHONE_MODE_A                   -
 *  @li@c  DRV_CUSTOM_AUD_HEADPHONE_MODE_B                   -
 *  @li@c  DRV_CUSTOM_AUD_HEADPHONE_MODE_MAX                 -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DRV_CUSTOM_AUD_HEADPHONE_MODE_A = 0,   /* Speaker auto mute(in driver) when headphone plug-in */
    DRV_CUSTOM_AUD_HEADPHONE_MODE_B,       /* Speaker don't do auto mute(in driver) when headphone plug-in */
    DRV_CUSTOM_AUD_HEADPHONE_MODE_MAX      /* Maximum number of mode */
} DRV_CUSTOM_AUD_HEADPHONE_MODE_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _AUD_TYPE_VOL_GAIN_INFO_T
 *  {
 *      UINT32        ui4_input_mask;
 *      INT16         i2_vol_gain;
 *  } AUD_TYPE_VOL_GAIN_INFO_T;
 *  @endcode
 *  @li@c  ui4_input_mask                                    -
 *  @li@c  i2_vol_gain                                       -
 */
/*------------------------------------------------------------------*/
typedef struct _AUD_TYPE_VOL_GAIN_INFO_T
{
    UINT32        ui4_input_mask;
    INT16         i2_vol_gain;     /* in 0.5 db per unit */
} AUD_TYPE_VOL_GAIN_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _AUD_TYPE_AVL_GAIN_INFO_T
 *  {
 *      DRV_CUSTOM_AVL_ADJ_T       t_avl_gain;
 *      INT32         i4_gain;
 *  } AUD_TYPE_AVL_GAIN_INFO_T;
 *  @endcode
 *  @li@c  t_avl_gain                                        -
 *  @li@c  i4_gain                                           -
 */
/*------------------------------------------------------------------*/
typedef struct _AUD_TYPE_AVL_GAIN_INFO_T
{
    DRV_CUSTOM_AVL_ADJ_T       t_avl_gain;
    INT32         i4_gain;
} AUD_TYPE_AVL_GAIN_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _AUD_TYPE_SPECTRUM_INFO_T
 *  {
 *      INT32         ai4_bar[15];
 *  } AUD_TYPE_SPECTRUM_INFO_T;
 *  @endcode
 *  @li@c  ai4_bar[15]                                       -
 */
/*------------------------------------------------------------------*/
typedef struct _AUD_TYPE_SPECTRUM_INFO_T
{
    INT32         ai4_bar[15];     /* 15 Spectrum Bars */
} AUD_TYPE_SPECTRUM_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _AUD_TYPE_TEST_TONE
 *  {
 *      BOOL        fgPlayTestTone;
 *      UINT8       u1DecId;
 *      UINT32      u4OutputChlSelect;
 *      UINT32      u4ToneFreq;
 *  } AUD_TYPE_TEST_TONE;
 *  @endcode
 *  @li@c  fgPlayTestTone                                    -
 *  @li@c  u1DecId                                           -
 *  @li@c  u4OutputChlSelect                                 -
 *  @li@c  u4ToneFreq                                        -
 */
/*------------------------------------------------------------------*/
typedef struct _AUD_TYPE_TEST_TONE
{
    BOOL        fgPlayTestTone;
    UINT8       u1DecId;
    UINT32      u4OutputChlSelect;
    UINT32      u4ToneFreq;
} AUD_TYPE_TEST_TONE;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _SMART_AUD_INFO_T
 *  {
 *      UINT8    u1SmartAudSound1;
 *      UINT8    u1SmartAudSound2;
 *      UINT8    u1SmartAudSound3;
 *      UINT8    u1SmartAudSound4;
 *      UINT8    u1SmartAudSound5;
 *      UINT8    u1SmartAudSound6;
 *      UINT8    u1SmartAudSound7;
 *      UINT8    u1SmartAudBass;
 *      UINT8    u1SmartAudTreble;
 *      UINT8    u1SmartAudVSurr;
 *  //    UINT8    u1SmartAudBBE;
 *      UINT8    u1SmartAudFreq;
 *      UINT8    u1SmartAVL;
 *  //    UINT8    u1SmartAudLoudness;
 *      UINT8    u1SmartAudVSurrOn1;
 *      UINT8    u1SmartAudVSurrOn2;
 *      UINT8    u1SmartAudVSurrOn3;
 *      UINT8    u1SmartAudVSurrOn4;
 *      UINT8    u1SmartAudVSurrOn5;
 *      UINT8    u1SmartAudVSurrOn6;
 *      UINT8    u1SmartAudVSurrOff1;
 *      UINT8    u1SmartAudVSurrOff2;
 *      UINT8    u1SmartAudVSurrOff3;
 *      UINT8    u1SmartAudVSurrOff4;
 *      UINT8    u1SmartAudVSurrOff5;
 *      UINT8    u1SmartAudVSurrOff6;
 *      UINT8    u1SmartAudVSurrFo1;
 *      UINT8    u1SmartAudVSurrFo2;
 *      UINT8    u1SmartAudVSurrFo3;
 *      UINT8    u1SmartRev1;
 *      UINT8    u1SmartRev2;
 *      UINT8    u1SmartRev3;
 *      UINT8    u1SmartRev4;
 *      UINT8    u1SmartRev5;
 *      UINT8    u1SmartRev6;
 *      UINT8    u1SmartRev7;
 *      UINT8    u1SmartRev8;
 *      UINT8    u1SmartRev9;
 *      UINT8    u1SmartRev10;
 *  } SMART_AUD_INFO_T;
 *  @endcode
 *  @li@c  u1SmartAudSound1                                  -
 *  @li@c  u1SmartAudSound2                                  -
 *  @li@c  u1SmartAudSound3                                  -
 *  @li@c  u1SmartAudSound4                                  -
 *  @li@c  u1SmartAudSound5                                  -
 *  @li@c  u1SmartAudSound6                                  -
 *  @li@c  u1SmartAudSound7                                  -
 *  @li@c  u1SmartAudBass                                    -
 *  @li@c  u1SmartAudTreble                                  -
 *  @li@c  u1SmartAudVSurr                                   -
 *  @li@c  u1SmartAudBBE                                     -
 *  @li@c  u1SmartAudFreq                                    -
 *  @li@c  u1SmartAVL                                        -
 *  @li@c  u1SmartAudLoudness                                -
 *  @li@c  u1SmartAudVSurrOn1                                -
 *  @li@c  u1SmartAudVSurrOn2                                -
 *  @li@c  u1SmartAudVSurrOn3                                -
 *  @li@c  u1SmartAudVSurrOn4                                -
 *  @li@c  u1SmartAudVSurrOn5                                -
 *  @li@c  u1SmartAudVSurrOn6                                -
 *  @li@c  u1SmartAudVSurrOff1                               -
 *  @li@c  u1SmartAudVSurrOff2                               -
 *  @li@c  u1SmartAudVSurrOff3                               -
 *  @li@c  u1SmartAudVSurrOff4                               -
 *  @li@c  u1SmartAudVSurrOff5                               -
 *  @li@c  u1SmartAudVSurrOff6                               -
 *  @li@c  u1SmartAudVSurrFo1                                -
 *  @li@c  u1SmartAudVSurrFo2                                -
 *  @li@c  u1SmartAudVSurrFo3                                -
 *  @li@c  u1SmartRev1                                       -
 *  @li@c  u1SmartRev2                                       -
 *  @li@c  u1SmartRev3                                       -
 *  @li@c  u1SmartRev4                                       -
 *  @li@c  u1SmartRev5                                       -
 *  @li@c  u1SmartRev6                                       -
 *  @li@c  u1SmartRev7                                       -
 *  @li@c  u1SmartRev8                                       -
 *  @li@c  u1SmartRev9                                       -
 *  @li@c  u1SmartRev10                                      -
 */
/*------------------------------------------------------------------*/
typedef struct _SMART_AUD_INFO_T
{
    UINT8    u1SmartAudSound1;
    UINT8    u1SmartAudSound2;
    UINT8    u1SmartAudSound3;
    UINT8    u1SmartAudSound4;
    UINT8    u1SmartAudSound5;
    UINT8    u1SmartAudSound6;
    UINT8    u1SmartAudSound7;
    UINT8    u1SmartAudBass;
    UINT8    u1SmartAudTreble;
    UINT8    u1SmartAudVSurr;
//    UINT8    u1SmartAudBBE;
    UINT8    u1SmartAudFreq;
    UINT8    u1SmartAVL;
//    UINT8    u1SmartAudLoudness;
    UINT8    u1SmartAudVSurrOn1;
    UINT8    u1SmartAudVSurrOn2;
    UINT8    u1SmartAudVSurrOn3;
    UINT8    u1SmartAudVSurrOn4;
    UINT8    u1SmartAudVSurrOn5;
    UINT8    u1SmartAudVSurrOn6;
    UINT8    u1SmartAudVSurrOff1;
    UINT8    u1SmartAudVSurrOff2;
    UINT8    u1SmartAudVSurrOff3;
    UINT8    u1SmartAudVSurrOff4;
    UINT8    u1SmartAudVSurrOff5;
    UINT8    u1SmartAudVSurrOff6;
    UINT8    u1SmartAudVSurrFo1;
    UINT8    u1SmartAudVSurrFo2;
    UINT8    u1SmartAudVSurrFo3;
    UINT8    u1SmartRev1;
    UINT8    u1SmartRev2;
    UINT8    u1SmartRev3;
    UINT8    u1SmartRev4;
    UINT8    u1SmartRev5;
    UINT8    u1SmartRev6;
    UINT8    u1SmartRev7;
    UINT8    u1SmartRev8;
    UINT8    u1SmartRev9;
    UINT8    u1SmartRev10;
} SMART_AUD_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _SMART_AUD_GROUP_INFO_T
 *  {
 *      SMART_AUD_INFO_T    aSmartAud[8];
 *  } SMART_AUD_GROUP_INFO_T;
 *  @endcode
 *  @li@c  aSmartAud[8]                                      -
 */
/*------------------------------------------------------------------*/
typedef struct _SMART_AUD_GROUP_INFO_T
{
    SMART_AUD_INFO_T    aSmartAud[8];
} SMART_AUD_GROUP_INFO_T;


/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct
 *  {
 *      UINT8   ui1_point_num;
 *      UINT8   ui1_vol_points[8];
 *                                      ui1_vol_points[0] must be 0
 *      UINT32  ui4_vol_values[8];
 *  } DRV_CUSTOM_AUD_VOL_CURVE_T;
 *  @endcode
 *  @li@c  ui1_point_num                                     -
 *  @li@c  ui1_vol_points[8]                                 -
 *  @li@c  ui4_vol_values[8]                                 -
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT8   ui1_point_num;       /* number of point number. ui1_point_num >= 2 */
    UINT8   ui1_vol_points[8];   /* points of volume curve. The value should be in incremental sequence.
                                    ui1_vol_points[0] must be 0
                                    ui1_vol_points[ui1_point_num-1] must be 100 */
    UINT32  ui4_vol_values[8];   /* values of volume curve. */
} DRV_CUSTOM_AUD_VOL_CURVE_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _AUD_TYPE_DD_CONTROL_T
 *  {
 *      UINT8        u1DecId;
 *      UINT8        ui1_DDP_enable;
 *      UINT8        ui1_mute_output_enable;
 *      UINT8        ui1_DDCO_disable;
 *  } AUD_TYPE_DD_CONTROL_T;
 *  @endcode
 *  @li@c  u1DecId                                           -
 *  @li@c  ui1_DDP_enable                                    -
 *  @li@c  ui1_mute_output_enable                            -
 *  @li@c  ui1_DDCO_disable                                  -
 */
/*------------------------------------------------------------------*/
typedef struct _AUD_TYPE_DD_CONTROL_T
{
    UINT8        u1DecId;
    UINT8        ui1_DDP_enable;
    UINT8        ui1_mute_output_enable;
    UINT8        ui1_DDCO_disable;
} AUD_TYPE_DD_CONTROL_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _DRV_CUSTOM_AUD_TYPE_AQ_VER_T
 *  {
 *      CHAR     s_aq_ver[8];
 *  } DRV_CUSTOM_AUD_TYPE_AQ_VER_T;
 *  @endcode
 *  @li@c  s_aq_ver[8]                                       -
 */
/*------------------------------------------------------------------*/
typedef struct _DRV_CUSTOM_AUD_TYPE_AQ_VER_T
{
    CHAR     s_aq_ver[8];
} DRV_CUSTOM_AUD_TYPE_AQ_VER_T;

/*----------------------------------------------------------------------------*/
/*! @struct _AUD_TYPE_BASS_BOOSTER
 *  @brief  This structure contains Virtual Bass information,
 *          to use with DRV_CUSTOM_AUD_TYPE_BASS_BOOSTER
 *
 *  @code
 *  typedef struct _AUD_TYPE_BASS_BOOSTER
 *  {
 *    BOOL        fgEnable;
 *    UINT8       u1Gain1;
 *    UINT8       u1Gain2;
 *    UINT16      u2CutFreq;
 *  } AUD_TYPE_BASS_BOOSTER;
 *  @endcode
 *
 *  @li@c  fgEnable
 *  @li@c  u1Gain1
 *  @li@c  u1Gain2
 *  @li@c  u2CutFreq
 */
/*----------------------------------------------------------------------------*/
typedef struct _AUD_TYPE_BASS_BOOSTER{
    BOOL        fgEnable;
    UINT8       u1Gain1;
    UINT8       u1Gain2;
    UINT16      u2CutFreq;
} AUD_TYPE_BASS_BOOSTER;

/*-----------------------------------------------------------------------------
    <<Tuner driver>>  macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      DRV_CUSTOM_TUNER_SET_TYPE_BEGIN = 0,
 *      DRV_CUSTOM_TUNER_SET_TYPE_DIAG_OPEN = DRV_CUSTOM_ARG_USE_REF_OFFSET,
 *      DRV_CUSTOM_TUNER_SET_TYPE_DIAG_CLOSE,
 *      DRV_CUSTOM_TUNER_SET_TYPE_AGC,
 *      DRV_CUSTOM_TUNER_SET_TYPE_DESCRAMBLE,
 *      DRV_CUSTOM_TUNER_SET_TYPE_END = DRV_CUSTOM_ARG_END_OFFSET
 *  }   DRV_CUSTOM_TUNER_SET_TYPE_T;
 *  @endcode
 *  @li@c  DRV_CUSTOM_TUNER_SET_TYPE_BEGIN                   -
 *  @li@c  DRV_CUSTOM_TUNER_SET_TYPE_DIAG_OPEN               -
 *  @li@c  DRV_CUSTOM_TUNER_SET_TYPE_DIAG_CLOSE              -
 *  @li@c  DRV_CUSTOM_TUNER_SET_TYPE_AGC                     -
 *  @li@c  DRV_CUSTOM_TUNER_SET_TYPE_DESCRAMBLE              -
 *  @li@c  DRV_CUSTOM_TUNER_SET_TYPE_END                     -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DRV_CUSTOM_TUNER_SET_TYPE_BEGIN = 0,

    /*
     * The argument of the following enumerations SHOULD BE a pointer
     */

    DRV_CUSTOM_TUNER_SET_TYPE_DIAG_OPEN = DRV_CUSTOM_ARG_USE_REF_OFFSET,  /* NULL, open tuner diag */
    DRV_CUSTOM_TUNER_SET_TYPE_DIAG_CLOSE,                                 /* NULL, close tuner diag */
    DRV_CUSTOM_TUNER_SET_TYPE_AGC,                                        /* TUNER_TYPE_DIAG_AGC_T, Set tuner AGC */
    DRV_CUSTOM_TUNER_SET_TYPE_DESCRAMBLE,                                 /* BOOL, Set tuner descramble */

    DRV_CUSTOM_TUNER_SET_TYPE_END = DRV_CUSTOM_ARG_END_OFFSET

}   DRV_CUSTOM_TUNER_SET_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      DRV_CUSTOM_TUNER_TYPE_BEGIN = 0,
 *      DRV_CUSTOM_TUNER_TYPE_ANALOG,
 *      DRV_CUSTOM_TUNER_TYPE_DIGITAL,
 *      DRV_CUSTOM_TUNER_TYPE_END
 *  } DRV_CUSTOM_TUNER_TYPE_T;
 *  @endcode
 *  @li@c  DRV_CUSTOM_TUNER_TYPE_BEGIN                       -
 *  @li@c  DRV_CUSTOM_TUNER_TYPE_ANALOG                      -
 *  @li@c  DRV_CUSTOM_TUNER_TYPE_DIGITAL                     -
 *  @li@c  DRV_CUSTOM_TUNER_TYPE_END                         -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DRV_CUSTOM_TUNER_TYPE_BEGIN = 0,

    DRV_CUSTOM_TUNER_TYPE_ANALOG,
    DRV_CUSTOM_TUNER_TYPE_DIGITAL,

    DRV_CUSTOM_TUNER_TYPE_END

} DRV_CUSTOM_TUNER_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      DRV_CUSTOM_TUNER_GET_TYPE_BEGIN = 0,
 *      DRV_CUSTOM_TUNER_GET_TYPE_DIAG_CONTROL = DRV_CUSTOM_ARG_USE_REF_OFFSET,
 *      DRV_CUSTOM_TUNER_GET_TYPE_DIAG_CONTENT,
 *      DRV_CUSTOM_TUNER_GET_TYPE_AGC,
 *      DRV_CUSTOM_TUNER_GET_TYPE_END = DRV_CUSTOM_ARG_END_OFFSET
 *  }   DRV_CUSTOM_TUNER_GET_TYPE_T;
 *  @endcode
 *  @li@c  DRV_CUSTOM_TUNER_GET_TYPE_BEGIN                   -
 *  @li@c  DRV_CUSTOM_TUNER_GET_TYPE_DIAG_CONTROL            -
 *  @li@c  DRV_CUSTOM_TUNER_GET_TYPE_DIAG_CONTENT            -
 *  @li@c  DRV_CUSTOM_TUNER_GET_TYPE_AGC                     -
 *  @li@c  DRV_CUSTOM_TUNER_GET_TYPE_END                     -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DRV_CUSTOM_TUNER_GET_TYPE_BEGIN = 0,

    DRV_CUSTOM_TUNER_GET_TYPE_DIAG_CONTROL = DRV_CUSTOM_ARG_USE_REF_OFFSET,    /* TUNER_TYPE_DIAG_CONTROL_T, Get diag control */
    DRV_CUSTOM_TUNER_GET_TYPE_DIAG_CONTENT,                                    /* DRV_CUSTOM_TUNER_GET_TYPE_DIAG_CONTENT, Get diag content */
    DRV_CUSTOM_TUNER_GET_TYPE_AGC,                                             /* TUNER_TYPE_DIAG_AGC_T, Get tuner AGC */

    DRV_CUSTOM_TUNER_GET_TYPE_END = DRV_CUSTOM_ARG_END_OFFSET

}   DRV_CUSTOM_TUNER_GET_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _TUNER_TYPE_DIAG_CONTROL_T
 *  {
 *      UINT16        u2PollPeriodMs;
 *      UCHAR         ucCntOfDiagStruc;
 *  } TUNER_TYPE_DIAG_CONTROL_T;
 *  @endcode
 *  @li@c  u2PollPeriodMs                                    -
 *  @li@c  ucCntOfDiagStruc                                  -
 */
/*------------------------------------------------------------------*/
typedef struct _TUNER_TYPE_DIAG_CONTROL_T
{
    UINT16        u2PollPeriodMs;      /* Polling period in unit of ms */
    UCHAR         ucCntOfDiagStruc;    /* How many diag strucs to be displayed on OSD */
} TUNER_TYPE_DIAG_CONTROL_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _TUNER_TYPE_DIAG_CONTENT_T
 *  {
 *      CHAR          cDisplayName[16];
 *      CHAR          cDisplayValue[16];
 *  } TUNER_TYPE_DIAG_CONTENT_T;
 *  @endcode
 *  @li@c  cDisplayName[16]                                  -
 *  @li@c  cDisplayValue[16]                                 -
 */
/*------------------------------------------------------------------*/
typedef struct _TUNER_TYPE_DIAG_CONTENT_T
{
    CHAR          cDisplayName[16];    /* A zero-end string indicating diag name */
    CHAR          cDisplayValue[16];   /* A zero-end string indicating diag value */
} TUNER_TYPE_DIAG_CONTENT_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _TUNER_TYPE_AGC_T
 *  {
 *      UINT8    ucAgcValue;
 *  } TUNER_TYPE_DIAG_AGC_T;
 *  @endcode
 *  @li@c  ucAgcValue                                        -
 */
/*------------------------------------------------------------------*/
typedef struct _TUNER_TYPE_AGC_T
{
    UINT8    ucAgcValue;
} TUNER_TYPE_DIAG_AGC_T;


/*-----------------------------------------------------------------------------
    <<Video driver>>  macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

#define DRV_CUSTOM_VID_MAIN                                    ((UINT32)    0)    /**<        */
#define DRV_CUSTOM_VID_SUB                                     ((UINT32)    1)    /**<        */

#ifndef DRV_CUSTOM_VID_MAX_SMART_PIC_SZ
#define DRV_CUSTOM_VID_MAX_SMART_PIC_SZ                                   (12)    /**<        */
#endif

#define LVDS_SPREAD_SPECTRUM_IGNORE                                 0xffffffff    /**<        */

/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      DRV_CUSTOM_VID_TYPE_BEGIN = 0,
 *      DRV_CUSTOM_VID_TYPE_ADC_R_GAIN = DRV_CUSTOM_ARG_NO_REF_OFFSET,
 *      DRV_CUSTOM_VID_TYPE_ADC_G_GAIN,
 *      DRV_CUSTOM_VID_TYPE_ADC_B_GAIN,
 *      DRV_CUSTOM_VID_TYPE_ADC_R_OFFSET,
 *      DRV_CUSTOM_VID_TYPE_ADC_G_OFFSET,
 *      DRV_CUSTOM_VID_TYPE_ADC_B_OFFSET,
 *      DRV_CUSTOM_VID_TYPE_ADC_R_MAX,
 *      DRV_CUSTOM_VID_TYPE_ADC_G_MAX,
 *      DRV_CUSTOM_VID_TYPE_ADC_B_MAX,
 *      DRV_CUSTOM_VID_TYPE_ADC_B_BLANK_LEVEL,
 *      DRV_CUSTOM_VID_TYPE_ADC_G_BLANK_LEVEL,
 *      DRV_CUSTOM_VID_TYPE_ADC_R_BLANK_LEVEL,
 *      DRV_CUSTOM_VID_TYPE_ADC_B_MAX_LEVEL,
 *      DRV_CUSTOM_VID_TYPE_ADC_G_MAX_LEVEL,
 *      DRV_CUSTOM_VID_TYPE_ADC_R_MAX_LEVEL,
 *      DRV_CUSTOM_VID_TYPE_ADC_EBGAIN_OVERRULE,
 *      DRV_CUSTOM_VID_TYPE_CLEAR_ADC_EEPROM,
 *      DRV_CUSTOM_VID_TYPE_ADC_EEPROM_VGA_R_GAIN,
 *      DRV_CUSTOM_VID_TYPE_ADC_EEPROM_VGA_G_GAIN,
 *      DRV_CUSTOM_VID_TYPE_ADC_EEPROM_VGA_B_GAIN,
 *      DRV_CUSTOM_VID_TYPE_ADC_EEPROM_VGA_R_OFFSET,
 *      DRV_CUSTOM_VID_TYPE_ADC_EEPROM_VGA_G_OFFSET,
 *      DRV_CUSTOM_VID_TYPE_ADC_EEPROM_VGA_B_OFFSET,
 *      DRV_CUSTOM_VID_TYPE_ADC_EEPROM_CMP_Y_GAIN,
 *      DRV_CUSTOM_VID_TYPE_ADC_EEPROM_CMP_PB_GAIN,
 *      DRV_CUSTOM_VID_TYPE_ADC_EEPROM_CMP_PR_GAIN,
 *      DRV_CUSTOM_VID_TYPE_ADC_EEPROM_CMP_Y_OFFSET,
 *      DRV_CUSTOM_VID_TYPE_ADC_EEPROM_CMP_PB_OFFSET,
 *      DRV_CUSTOM_VID_TYPE_ADC_EEPROM_CMP_PR_OFFSET,
 *      DRV_CUSTOM_VID_TYPE_ADC_EEPROM_SCART_R_GAIN,
 *      DRV_CUSTOM_VID_TYPE_ADC_EEPROM_SCART_G_GAIN,
 *      DRV_CUSTOM_VID_TYPE_ADC_EEPROM_SCART_B_GAIN,
 *      DRV_CUSTOM_VID_TYPE_ADC_EEPROM_SCART_R_OFFSET,
 *      DRV_CUSTOM_VID_TYPE_ADC_EEPROM_SCART_G_OFFSET,
 *      DRV_CUSTOM_VID_TYPE_ADC_EEPROM_SCART_B_OFFSET,
 *      DRV_CUSTOM_VID_TYPE_VGA_AUTOCOLOR_STATUS,
 *      DRV_CUSTOM_VID_TYPE_CMP_AUTOCOLOR_STATUS,
 *      DRV_CUSTOM_VID_TYPE_SCART_AUTOCOLOR_STATUS,
 *      DRV_CUSTOM_AUTOCOLOR_EEPROM_ENABLE,
 *      DRV_CUSTOM_DISP_TYPE_PC_VIDEO_ONOFF,
 *      DRV_CUSTOM_DISP_TYPE_OUTRGB_COLOR_ONOFF,
 *      DRV_CUSTOM_VID_TYPE_BRIGHTNESS_DFT,
 *      DRV_CUSTOM_VID_TYPE_CONTRAST_DFT,
 *      DRV_CUSTOM_VID_TYPE_SATURATION_DFT,
 *      DRV_CUSTOM_VID_TYPE_HUE_DFT,
 *      DRV_CUSTOM_VID_TYPE_YPBPR_VTH,
 *      DRV_CUSTOM_VID_TYPE_YPBPR_VTL,
 *      DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_MIN,
 *      DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_MAX,
 *      DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_DFT,
 *      DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_RATIO_TOP,
 *      DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_RATIO_STANDARD,
 *      DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_RATIO_BOTTOM,
 *      DRV_CUSTOM_DISP_TYPE_ADP_BACK_LIGHT_NORMAL,
 *      DRV_CUSTOM_DISP_TYPE_ADP_BACK_LIGHT_MIDDLE,
 *      DRV_CUSTOM_DISP_TYPE_ADP_BACK_LIGHT_HIGH,
 *      DRV_CUSTOM_DISP_TYPE_CT_RED_GAIN,
 *      DRV_CUSTOM_DISP_TYPE_CT_GREEN_GAIN,
 *      DRV_CUSTOM_DISP_TYPE_CT_BLUE_GAIN,
 *      DRV_CUSTOM_DISP_TYPE_CT_RED_OFFSET,
 *      DRV_CUSTOM_DISP_TYPE_CT_GREEN_OFFSET,
 *      DRV_CUSTOM_DISP_TYPE_CT_BLUE_OFFSET,
 *      DRV_CUSTOM_DISP_TYPE_CT_RED_GAIN_PIP,
 *      DRV_CUSTOM_DISP_TYPE_CT_GREEN_GAIN_PIP,
 *      DRV_CUSTOM_DISP_TYPE_CT_BLUE_GAIN_PIP,
 *      DRV_CUSTOM_DISP_TYPE_CT_RED_OFFSET_PIP,
 *      DRV_CUSTOM_DISP_TYPE_CT_GREEN_OFFSET_PIP,
 *      DRV_CUSTOM_DISP_TYPE_CT_BLUE_OFFSET_PIP,
 *      DRV_CUSTOM_DISP_TYPE_GAMMA_OFFSET,
 *      DRV_CUSTOM_DISP_TYPE_SPREAD_SPECTRUM,
 *      DRV_CUSTOM_DISP_TYPE_SMART_PICTURE_INDEX,
 *      DRV_CUSTOM_DISP_TYPE_HDMI_RANGE_MODE,
 *      DRV_CUSTOM_VID_TYPE_PQ_ALL_ONOFF,
 *      DRV_CUSTOM_VID_TYPE_TVD_ADC_GAIN_EEPROM,
 *      DRV_CUSTOM_VID_TYPE_TVD_ADC_MAX,
 *      DRV_CUSTOM_VID_TYPE_TVD_ADC_GAIN,
 *      DRV_CUSTOM_VID_TYPE_TVD_ADC_OFFSET_EEPROM,
 *      DRV_CUSTOM_VID_TYPE_TVD_ADC_OFFSET,
 *      DRV_CUSTOM_DISP_TYPE_CLARITY,
 *      DRV_CUSTOM_VID_TYPE_EPG_FLAG,
 *      DRV_CUSTOM_DISP_TYPE_GAMMA = DRV_CUSTOM_ARG_USE_REF_OFFSET,
 *      DRV_CUSTOM_VID_TYPE_NON_LINEAR_FACTOR,
 *      DRV_CUSTOM_DISP_TYPE_OUTRGB_COLOR,
 *      DRV_CUSTOM_DISP_TYPE_SMART_PICTURE,
 *      DRV_CUSTOM_DISP_TYPE_PICTURE_INFO,
 *      DRV_CUSTOM_DISP_TYPE_FEATURE_SELECTIOIN,
 *      DRV_CUSTOM_DISP_TYPE_HDMI_PCHD_MODE,
 *      DRV_CUSTOM_DISP_TYPE_PQ_VER,
 *      DRV_CUSTOM_DISP_TYPE_GAMMA_VER,
 *      DRV_CUSTOM_DISP_TYPE_SCE_VER,
 *      DRV_CUSTOM_DISP_TYPE_QTY_VER,
 *      DRV_CUSTOM_VID_TYPE_PQ_DEMO,
 *      DRV_CUSTOM_VID_TYPE_DEFAULT_GAMMA_VALUE,
 *      DRV_CUSTOM_VID_TYPE_COMP_GAMMA_VALUE,
 *      DRV_CUSTOM_VID_TYPE_GAMMA_SAVE_DELTA,
 *      DRV_CUSTOM_VID_TYPE_GAMMA_PATTERN,
 *      DRV_CUSTOM_VID_TYPE_MUTE_PATTERN,
 *      DRV_CUSTOM_VID_TYPE_SCE_PATTERN,
 *      DRV_CUSTOM_DISP_TYPE_AUTO_BACK_LIGHT_SETS,
 *      DRV_CUSTOM_DISP_TYPE_AUTO_COLORTEMP_SETS,
 *      DRV_CUSTOM_VID_TYPE_DOT_TO_DOT_PATTERN,
 *      DRV_CUSTOM_DISP_TYPE_PANEL_SELECT,
 *      DRV_CUSTOM_DISP_TYPE_LVDS_SPREAD_SPECTRUM_SETTING,
 *      DRV_CUSTOM_DISP_TYPE_LVDS_FORMAT,
 *      DRV_CUSTOM_DISP_TYPE_LVDS_BIT_DEPTH,
 *      DRV_CUSTOM_DISP_TYPE_LVDS_CHANNEL_SWAP,
 *      DRV_CUSTOM_DISP_TYPE_GET_PANEL_RESOLUTION,
 *      DRV_CUSTOM_VID_TYPE_END = DRV_CUSTOM_ARG_END_OFFSET
 *  }DRV_CUSTOM_VID_TYPE_T;
 *  @endcode
 *  @li@c  DRV_CUSTOM_VID_TYPE_BEGIN                         -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_R_GAIN                    -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_G_GAIN                    -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_B_GAIN                    -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_R_OFFSET                  -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_G_OFFSET                  -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_B_OFFSET                  -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_R_MAX                     -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_G_MAX                     -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_B_MAX                     -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_B_BLANK_LEVEL             -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_G_BLANK_LEVEL             -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_R_BLANK_LEVEL             -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_B_MAX_LEVEL               -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_G_MAX_LEVEL               -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_R_MAX_LEVEL               -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_EBGAIN_OVERRULE           -
 *  @li@c  DRV_CUSTOM_VID_TYPE_CLEAR_ADC_EEPROM              -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_EEPROM_VGA_R_GAIN         -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_EEPROM_VGA_G_GAIN         -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_EEPROM_VGA_B_GAIN         -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_EEPROM_VGA_R_OFFSET       -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_EEPROM_VGA_G_OFFSET       -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_EEPROM_VGA_B_OFFSET       -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_EEPROM_CMP_Y_GAIN         -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_EEPROM_CMP_PB_GAIN        -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_EEPROM_CMP_PR_GAIN        -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_EEPROM_CMP_Y_OFFSET       -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_EEPROM_CMP_PB_OFFSET      -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_EEPROM_CMP_PR_OFFSET      -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_EEPROM_SCART_R_GAIN       -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_EEPROM_SCART_G_GAIN       -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_EEPROM_SCART_B_GAIN       -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_EEPROM_SCART_R_OFFSET     -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_EEPROM_SCART_G_OFFSET     -
 *  @li@c  DRV_CUSTOM_VID_TYPE_ADC_EEPROM_SCART_B_OFFSET     -
 *  @li@c  DRV_CUSTOM_VID_TYPE_VGA_AUTOCOLOR_STATUS          -
 *  @li@c  DRV_CUSTOM_VID_TYPE_CMP_AUTOCOLOR_STATUS          -
 *  @li@c  DRV_CUSTOM_VID_TYPE_SCART_AUTOCOLOR_STATUS        -
 *  @li@c  DRV_CUSTOM_AUTOCOLOR_EEPROM_ENABLE                -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_PC_VIDEO_ONOFF               -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_OUTRGB_COLOR_ONOFF           -
 *  @li@c  DRV_CUSTOM_VID_TYPE_BRIGHTNESS_DFT                -
 *  @li@c  DRV_CUSTOM_VID_TYPE_CONTRAST_DFT                  -
 *  @li@c  DRV_CUSTOM_VID_TYPE_SATURATION_DFT                -
 *  @li@c  DRV_CUSTOM_VID_TYPE_HUE_DFT                       -
 *  @li@c  DRV_CUSTOM_VID_TYPE_YPBPR_VTH                     -
 *  @li@c  DRV_CUSTOM_VID_TYPE_YPBPR_VTL                     -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_MIN               -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_MAX               -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_DFT               -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_RATIO_TOP         -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_RATIO_STANDARD    -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_RATIO_BOTTOM      -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_ADP_BACK_LIGHT_NORMAL        -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_ADP_BACK_LIGHT_MIDDLE        -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_ADP_BACK_LIGHT_HIGH          -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_CT_RED_GAIN                  -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_CT_GREEN_GAIN                -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_CT_BLUE_GAIN                 -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_CT_RED_OFFSET                -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_CT_GREEN_OFFSET              -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_CT_BLUE_OFFSET               -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_CT_RED_GAIN_PIP              -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_CT_GREEN_GAIN_PIP            -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_CT_BLUE_GAIN_PIP             -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_CT_RED_OFFSET_PIP            -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_CT_GREEN_OFFSET_PIP          -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_CT_BLUE_OFFSET_PIP           -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_GAMMA_OFFSET                 -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_SPREAD_SPECTRUM              -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_SMART_PICTURE_INDEX          -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_HDMI_RANGE_MODE              -
 *  @li@c  DRV_CUSTOM_VID_TYPE_PQ_ALL_ONOFF                  -
 *  @li@c  DRV_CUSTOM_VID_TYPE_TVD_ADC_GAIN_EEPROM           -
 *  @li@c  DRV_CUSTOM_VID_TYPE_TVD_ADC_MAX                   -
 *  @li@c  DRV_CUSTOM_VID_TYPE_TVD_ADC_GAIN                  -
 *  @li@c  DRV_CUSTOM_VID_TYPE_TVD_ADC_OFFSET_EEPROM         -
 *  @li@c  DRV_CUSTOM_VID_TYPE_TVD_ADC_OFFSET                -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_CLARITY                      -
 *  @li@c  DRV_CUSTOM_VID_TYPE_EPG_FLAG                      -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_GAMMA                        -
 *  @li@c  DRV_CUSTOM_VID_TYPE_NON_LINEAR_FACTOR             -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_OUTRGB_COLOR                 -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_SMART_PICTURE                -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_PICTURE_INFO                 -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_FEATURE_SELECTIOIN           -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_HDMI_PCHD_MODE               -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_PQ_VER                       -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_GAMMA_VER                    -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_SCE_VER                      -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_QTY_VER                      -
 *  @li@c  DRV_CUSTOM_VID_TYPE_PQ_DEMO                       -
 *  @li@c  DRV_CUSTOM_VID_TYPE_DEFAULT_GAMMA_VALUE           -
 *  @li@c  DRV_CUSTOM_VID_TYPE_COMP_GAMMA_VALUE              -
 *  @li@c  DRV_CUSTOM_VID_TYPE_GAMMA_SAVE_DELTA              -
 *  @li@c  DRV_CUSTOM_VID_TYPE_GAMMA_PATTERN                 -
 *  @li@c  DRV_CUSTOM_VID_TYPE_MUTE_PATTERN                  -
 *  @li@c  DRV_CUSTOM_VID_TYPE_SCE_PATTERN                   -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_AUTO_BACK_LIGHT_SETS         -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_AUTO_COLORTEMP_SETS          -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_AUTO_COLORTEMP_GETS          -
 *  @li@c  DRV_CUSTOM_VID_TYPE_DOT_TO_DOT_PATTERN            -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_PANEL_SELECT                 -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_LVDS_SPREAD_SPECTRUM_SETTING -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_LVDS_FORMAT                  -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_LVDS_BIT_DEPTH               -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_LVDS_CHANNEL_SWAP            -
 *  @li@c  DRV_CUSTOM_DISP_TYPE_GET_PANEL_RESOLUTION         -
 *  @li@c  DRV_CUSTOM_VID_TYPE_END                           -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DRV_CUSTOM_VID_TYPE_BEGIN = 0,

    /*
     * The argument of the following enumerations SHOULD NOT be a pointer
     */

    /* ADC correction */
    DRV_CUSTOM_VID_TYPE_ADC_R_GAIN = DRV_CUSTOM_ARG_NO_REF_OFFSET,    /* UINT16, Red/Y gain (by source) */
    DRV_CUSTOM_VID_TYPE_ADC_G_GAIN,                                   /* UINT16, Green/Pr gain (by source) */
    DRV_CUSTOM_VID_TYPE_ADC_B_GAIN,                                   /* UINT16, Blue gain/Pb (by source) */
    DRV_CUSTOM_VID_TYPE_ADC_R_OFFSET,                                 /* UINT16, Red/Y offset (by source) */
    DRV_CUSTOM_VID_TYPE_ADC_G_OFFSET,                                 /* UINT16, Green/Pr offset (by source) */
    DRV_CUSTOM_VID_TYPE_ADC_B_OFFSET,                                 /* UINT16, Blue/Pb offset (by source) */
    DRV_CUSTOM_VID_TYPE_ADC_R_MAX,                                    /* UINT16, Red maximum level after auto color */
    DRV_CUSTOM_VID_TYPE_ADC_G_MAX,                                    /* UINT16, Green maximum level after auto color */
    DRV_CUSTOM_VID_TYPE_ADC_B_MAX,                                    /* UINT16, Blue maximum level after auto color */
    DRV_CUSTOM_VID_TYPE_ADC_B_BLANK_LEVEL,                            /* UINT16, */
    DRV_CUSTOM_VID_TYPE_ADC_G_BLANK_LEVEL,                            /* UINT16, */
    DRV_CUSTOM_VID_TYPE_ADC_R_BLANK_LEVEL,                            /* UINT16, */
    DRV_CUSTOM_VID_TYPE_ADC_B_MAX_LEVEL,                              /* UINT16, */
    DRV_CUSTOM_VID_TYPE_ADC_G_MAX_LEVEL,                              /* UINT16, */
    DRV_CUSTOM_VID_TYPE_ADC_R_MAX_LEVEL,                              /* UINT16, */
    DRV_CUSTOM_VID_TYPE_ADC_EBGAIN_OVERRULE,                          /* UINT16, EBGAIN_OVERRULE */
    DRV_CUSTOM_VID_TYPE_CLEAR_ADC_EEPROM,                             /* UINT16 */
    DRV_CUSTOM_VID_TYPE_ADC_EEPROM_VGA_R_GAIN,                        /* UINT16, read VGA R gain from EEPROM */
    DRV_CUSTOM_VID_TYPE_ADC_EEPROM_VGA_G_GAIN,                        /* UINT16, read VGA G gain from EEPROM */
    DRV_CUSTOM_VID_TYPE_ADC_EEPROM_VGA_B_GAIN,                        /* UINT16, read VGA B gain from EEPROM */
    DRV_CUSTOM_VID_TYPE_ADC_EEPROM_VGA_R_OFFSET,                      /* UINT16, read VGA R offset from EEPROM */
    DRV_CUSTOM_VID_TYPE_ADC_EEPROM_VGA_G_OFFSET,                      /* UINT16, read VGA R offset from EEPROM */
    DRV_CUSTOM_VID_TYPE_ADC_EEPROM_VGA_B_OFFSET,                      /* UINT16, read VGA R offset from EEPROM */
    DRV_CUSTOM_VID_TYPE_ADC_EEPROM_CMP_Y_GAIN,                        /* UINT16, read Y   gain from EEPROM */
    DRV_CUSTOM_VID_TYPE_ADC_EEPROM_CMP_PB_GAIN,                       /* UINT16, read PB gain from EEPROM */
    DRV_CUSTOM_VID_TYPE_ADC_EEPROM_CMP_PR_GAIN,                       /* UINT16, read PR gain from EEPROM */
    DRV_CUSTOM_VID_TYPE_ADC_EEPROM_CMP_Y_OFFSET,                      /* UINT16, read Y   Offset from EEPROM */
    DRV_CUSTOM_VID_TYPE_ADC_EEPROM_CMP_PB_OFFSET,                     /* UINT16, read PB Offset from EEPROM */
    DRV_CUSTOM_VID_TYPE_ADC_EEPROM_CMP_PR_OFFSET,                     /* UINT16, read PR Offset from EEPROM */
    DRV_CUSTOM_VID_TYPE_ADC_EEPROM_SCART_R_GAIN,                      /* UINT16, read SCART R gain from EEPROM */
    DRV_CUSTOM_VID_TYPE_ADC_EEPROM_SCART_G_GAIN,                      /* UINT16, read SCART G gain from EEPROM */
    DRV_CUSTOM_VID_TYPE_ADC_EEPROM_SCART_B_GAIN,                      /* UINT16, read SCART B gain from EEPROM */
    DRV_CUSTOM_VID_TYPE_ADC_EEPROM_SCART_R_OFFSET,                    /* UINT16, read SCART R Offset from EEPROM */
    DRV_CUSTOM_VID_TYPE_ADC_EEPROM_SCART_G_OFFSET,                    /* UINT16, read SCART G Offset from EEPROM */
    DRV_CUSTOM_VID_TYPE_ADC_EEPROM_SCART_B_OFFSET,                    /* UINT16, read SCART B Offset from EEPROM */
    DRV_CUSTOM_VID_TYPE_VGA_AUTOCOLOR_STATUS,                         /* UINT16, read VGA auto color was done or not status from EEPROM */
    DRV_CUSTOM_VID_TYPE_CMP_AUTOCOLOR_STATUS,                         /* UINT16, read CMP auto color was done or not status from EEPROM */
    DRV_CUSTOM_VID_TYPE_SCART_AUTOCOLOR_STATUS,                       /* UINT16, read SCART auto color was done or not  status from EEPROM */

    DRV_CUSTOM_AUTOCOLOR_EEPROM_ENABLE,                               /* UINT8, Enable EEPROM or not while autocolor */

    /* YPbPr Position */
    DRV_CUSTOM_VID_TYPE_YPBPR_H_POS,                                  /* UINT16, YPbPr horizontal position (by timing) */
    DRV_CUSTOM_VID_TYPE_YPBPR_V_POS,                                  /* UINT16, YPbPr vertical position (by timing) */

    DRV_CUSTOM_DISP_TYPE_PC_VIDEO_ONOFF,                              /* UINT8, PC Video mode */
    DRV_CUSTOM_DISP_TYPE_OUTRGB_COLOR_ONOFF,                          /* UINT8, Output mute */

    /* YPbPr SOGY slicer setting */
    DRV_CUSTOM_VID_TYPE_YPBPR_VTH,                                    /* UINT8, YPbPr Slicer high threshold */
    DRV_CUSTOM_VID_TYPE_YPBPR_VTL,                                    /* UINT8, YPbPr Slicer high threshold */

    /* Color temperature */
    DRV_CUSTOM_DISP_TYPE_CT_RED_GAIN,                                 /* UINT16, Color temperature red gain */
    DRV_CUSTOM_DISP_TYPE_CT_GREEN_GAIN,                               /* UINT16, Color temperature green gain */
    DRV_CUSTOM_DISP_TYPE_CT_BLUE_GAIN,                                /* UINT16, Color temperature blue gain */
    DRV_CUSTOM_DISP_TYPE_CT_RED_OFFSET,                               /* UINT16, Color temperature red offset */
    DRV_CUSTOM_DISP_TYPE_CT_GREEN_OFFSET,                             /* UINT16, Color temperature green offset */
    DRV_CUSTOM_DISP_TYPE_CT_BLUE_OFFSET,                              /* UINT16, Color temperature blue offset */
    DRV_CUSTOM_DISP_TYPE_CT_RED_GAIN_PIP,                             /* UINT16, Color temperature red gain */
    DRV_CUSTOM_DISP_TYPE_CT_GREEN_GAIN_PIP,                           /* UINT16, Color temperature green gain */
    DRV_CUSTOM_DISP_TYPE_CT_BLUE_GAIN_PIP,                            /* UINT16, Color temperature blue gain */
    DRV_CUSTOM_DISP_TYPE_CT_RED_OFFSET_PIP,                           /* UINT16, Color temperature red offset */
    DRV_CUSTOM_DISP_TYPE_CT_GREEN_OFFSET_PIP,                         /* UINT16, Color temperature green offset */
    DRV_CUSTOM_DISP_TYPE_CT_BLUE_OFFSET_PIP,                          /* UINT16, Color temperature blue offset */

    DRV_CUSTOM_DISP_TYPE_GAMMA_OFFSET,                                /* UINT32, Gamma offset */
    DRV_CUSTOM_DISP_TYPE_SPREAD_SPECTRUM,                             /* UINT16, Spread spectrum. 0-1000 */
    DRV_CUSTOM_DISP_TYPE_SMART_PICTURE_INDEX,                         /* UINT8, Smart Picture Index */
    DRV_CUSTOM_DISP_TYPE_HDMI_RANGE_MODE,                             /* UINT8, HDMI range mode change */
    DRV_CUSTOM_VID_TYPE_PQ_ALL_ONOFF,                                 /* UINT8, PQ effect all turn ON/OFF */

    /* TVD */
    DRV_CUSTOM_VID_TYPE_TVD_ADC_GAIN_EEPROM,                          /* UINT8, */
    DRV_CUSTOM_VID_TYPE_TVD_ADC_MAX,                                  /* UINT16, */
    DRV_CUSTOM_VID_TYPE_TVD_ADC_GAIN,                                 /* UINT8, */
    DRV_CUSTOM_VID_TYPE_TVD_ADC_OFFSET_EEPROM,                        /* UINT16, */
    DRV_CUSTOM_VID_TYPE_TVD_ADC_OFFSET,                               /* UINT16, */

    DRV_CUSTOM_DISP_TYPE_CLARITY,                                     /* INT8, Backlight Clarity */

    DRV_CUSTOM_VID_TYPE_EPG_FLAG,                                     /* BOOL, EPG Flag */

    /*
     * The argument of the following enumerations SHOULD BE a pointer
     */

    /* Backlight value */
    DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_MIN = DRV_CUSTOM_ARG_USE_REF_OFFSET,    /* VID_INFO_T, Backlight minimum (by source) */
    DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_MAX,                                    /* VID_INFO_T, Backlight maximum (by source) */
    DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_DFT,                                    /* VID_INFO_T, Backlight maximum (by source) */
    DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_RATIO_TOP,                              /* VID_INFO_T, Backlight radio top */
    DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_RATIO_STANDARD,                         /* VID_INFO_T, Backlight radio standard */
    DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_RATIO_BOTTOM,                           /* VID_INFO_T, Backlight radio bottom */
    DRV_CUSTOM_DISP_TYPE_ADP_BACK_LIGHT_NORMAL,                             /* VID_INFO_T, Adaptive backlight normal */
    DRV_CUSTOM_DISP_TYPE_ADP_BACK_LIGHT_MIDDLE,                             /* VID_INFO_T, Adaptive backlight middle */
    DRV_CUSTOM_DISP_TYPE_ADP_BACK_LIGHT_HIGH,                               /* VID_INFO_T, Adaptive backlight high */

    /* Default value */
    DRV_CUSTOM_VID_TYPE_BRIGHTNESS_DFT,                                     /* VID_INFO_T, Brightness default (by source) */
    DRV_CUSTOM_VID_TYPE_CONTRAST_DFT,                                       /* VID_INFO_T, Contrast default (by source) */
    DRV_CUSTOM_VID_TYPE_SATURATION_DFT,                                     /* VID_INFO_T, Saturation (color) default (by source) */
    DRV_CUSTOM_VID_TYPE_HUE_DFT,                                            /* VID_INFO_T, Hue (tint) default (by source) */

    DRV_CUSTOM_DISP_TYPE_GAMMA,                                             /* GAMMA_INFO_T, Gamma table */
    DRV_CUSTOM_VID_TYPE_NON_LINEAR_FACTOR,                                  /* VID_NON_LINEAR_INFO_T, Non-linear factor */
    DRV_CUSTOM_DISP_TYPE_OUTRGB_COLOR,                                      /* OUTRGB_INFO_T, Output mute */
    DRV_CUSTOM_DISP_TYPE_SMART_PICTURE,                                     /* SMART_PIC_GROUP_INFO_T, Smart Picture */
    DRV_CUSTOM_DISP_TYPE_PICTURE_INFO,                                      /* PIC_INFO_T, Picture info */
    DRV_CUSTOM_DISP_TYPE_FEATURE_SELECTIOIN,                                /* UINT32, Feature Selection */
    DRV_CUSTOM_DISP_TYPE_HDMI_PCHD_MODE,                                    /* DRV_CUSTOM_HDMI_PCHD_MODE, PC/HD mode change */

    DRV_CUSTOM_DISP_TYPE_PQ_VER,                                            /* CHAR*, PQ ver */
    DRV_CUSTOM_DISP_TYPE_GAMMA_VER,                                         /* CHAR*, Gamma ver */
    DRV_CUSTOM_DISP_TYPE_SCE_VER,                                           /* CHAR*, SCE ver */
    DRV_CUSTOM_DISP_TYPE_QTY_VER,                                           /* CHAR*, QTY ver */

    DRV_CUSTOM_VID_TYPE_PQ_DEMO,                                            /* PQ_DEMO_INFO_T, PQ Demo mode */

    /* Gamma Adjust */
    DRV_CUSTOM_VID_TYPE_DEFAULT_GAMMA_VALUE,                                /* DRV_CUSTOM_GAMMA_VALUE_T, */
    DRV_CUSTOM_VID_TYPE_COMP_GAMMA_VALUE,                                   /* DRV_CUSTOM_GAMMA_VALUE_T, */
    DRV_CUSTOM_VID_TYPE_GAMMA_SAVE_DELTA,                                   /* DRV_CUSTOM_GAMMA_DELTA_T, */
    DRV_CUSTOM_VID_TYPE_GAMMA_PATTERN,                                      /* DRV_CUSTOM_PATTERN_T, */

    /* Gamma Buf for 10 point */
    DRV_CUSTOM_VID_TYPE_GAMMA_BUF,                                          /* DRV_GAMMA_BUF_T */

    DRV_CUSTOM_VID_TYPE_MUTE_PATTERN,                                       /* DRV_CUSTOM_PATTERN_T, Mute pattern before RGB gain offset */
    DRV_CUSTOM_VID_TYPE_SCE_PATTERN,                                        /* DRV_CUSTOM_SCE_PATTERN_T, SCE pattern  */
    DRV_CUSTOM_VID_TYPE_DBC_DEBUG,                                          /* DRV_CUSTOM_DBC_DEBUG_T */
    DRV_CUSTOM_DISP_TYPE_AUTO_BACK_LIGHT_SETS,                              /* AUTO_BACKLIGHT_SETS_INFO_T, Backlight PWM V-Sync */
    DRV_CUSTOM_DISP_TYPE_AUTO_COLORTEMP_SETS,                               /* AUTO_COLORTEMP_SETS_INFO_T, Auto Color Temperature */
    DRV_CUSTOM_DISP_TYPE_AUTO_COLORTEMP_GETS,                               /* AUTO_COLORTEMP_GETS_INFO_T, Auto Color Temperature */
    DRV_CUSTOM_VID_TYPE_DOT_TO_DOT_PATTERN,                                 /* VID_TYPE_DOT_TO_DOT_PATTERN_T, DOT to DOT Pattern */
    DRV_CUSTOM_DISP_TYPE_PANEL_SELECT,                                      /* UINT16, Panel Selection */
    DRV_CUSTOM_DISP_TYPE_LVDS_SPREAD_SPECTRUM_SETTING,                      /* LVDS_SPREAD_SPECTRUM_INFO_T, LVDS Spread Spectrum Setting */
    DRV_CUSTOM_DISP_TYPE_LVDS_FORMAT,                                       /* DRV_CUSTOM_LVDS_FORMAT_T, LVDS format Setting */
    DRV_CUSTOM_DISP_TYPE_LVDS_BIT_DEPTH,                                    /* DRV_CUSTOM_LVDS_BIT_DEPTH_T, LVDS Bit Depth Setting */
    DRV_CUSTOM_DISP_TYPE_LVDS_CHANNEL_SWAP,                                 /* UINT16, LVDS Channel Swap Setting */
    DRV_CUSTOM_DISP_TYPE_GET_PANEL_RESOLUTION,                              /* VID_TYPE_PANEL_RESOLUTION_T, Get Panel Resolution */
	DRV_CUSTOM_VID_TYPE_PQ_DATA,

    DRV_CUSTOM_VID_TYPE_END = DRV_CUSTOM_ARG_END_OFFSET
}DRV_CUSTOM_VID_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      DTV_CUSTOM_VID_INP_FLAG_NONE,
 *      DRV_CUSTOM_VID_INP_FLAG_AV,
 *      DRV_CUSTOM_VID_INP_FLAG_ATV,
 *      DRV_CUSTOM_VID_INP_FLAG_DTV,
 *      DRV_CUSTOM_VID_INP_FLAG_YPBPR,
 *      DRV_CUSTOM_VID_INP_FLAG_VGA,
 *      DRV_CUSTOM_VID_INP_FLAG_HDMI,
 *      DRV_CUSTOM_VID_INP_FLAG_DVI
 *  } DRV_CUSTOM_VID_INP_T;
 *  @endcode
 *  @li@c  DTV_CUSTOM_VID_INP_FLAG_NONE                      -
 *  @li@c  DRV_CUSTOM_VID_INP_FLAG_AV                        -
 *  @li@c  DRV_CUSTOM_VID_INP_FLAG_ATV                       -
 *  @li@c  DRV_CUSTOM_VID_INP_FLAG_DTV                       -
 *  @li@c  DRV_CUSTOM_VID_INP_FLAG_YPBPR                     -
 *  @li@c  DRV_CUSTOM_VID_INP_FLAG_VGA                       -
 *  @li@c  DRV_CUSTOM_VID_INP_FLAG_HDMI                      -
 *  @li@c  DRV_CUSTOM_VID_INP_FLAG_DVI                       -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DTV_CUSTOM_VID_INP_FLAG_NONE,
    DRV_CUSTOM_VID_INP_FLAG_AV,
    DRV_CUSTOM_VID_INP_FLAG_ATV,
    DRV_CUSTOM_VID_INP_FLAG_DTV,
    DRV_CUSTOM_VID_INP_FLAG_YPBPR,
    DRV_CUSTOM_VID_INP_FLAG_VGA,
    DRV_CUSTOM_VID_INP_FLAG_HDMI,
    DRV_CUSTOM_VID_INP_FLAG_DVI
} DRV_CUSTOM_VID_INP_T;

/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      DRV_CUSTOM_DISP_CT0,
 *      DRV_CUSTOM_DISP_CT1,
 *      DRV_CUSTOM_DISP_CT2,
 *      DRV_CUSTOM_DISP_CT3,
 *      DRV_CUSTOM_DISP_CT4,
 *      DRV_CUSTOM_DISP_CT5,
 *      DRV_CUSTOM_DISP_CT6
 *  } DRV_CUSTOM_COLOR_TEMP_T;
 *  @endcode
 *  @li@c  DRV_CUSTOM_DISP_CT0                               -
 *  @li@c  DRV_CUSTOM_DISP_CT1                               -
 *  @li@c  DRV_CUSTOM_DISP_CT2                               -
 *  @li@c  DRV_CUSTOM_DISP_CT3                               -
 *  @li@c  DRV_CUSTOM_DISP_CT4                               -
 *  @li@c  DRV_CUSTOM_DISP_CT5                               -
 *  @li@c  DRV_CUSTOM_DISP_CT6                               -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DRV_CUSTOM_DISP_CT0,
    DRV_CUSTOM_DISP_CT1,
    DRV_CUSTOM_DISP_CT2,
    DRV_CUSTOM_DISP_CT3,
    DRV_CUSTOM_DISP_CT4,
    DRV_CUSTOM_DISP_CT5,
    DRV_CUSTOM_DISP_CT6
} DRV_CUSTOM_COLOR_TEMP_T;

/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      DRV_CUSTOM_HDMI_AUTO,
 *      DRV_CUSTOM_HDMI_PC,
 *      DRV_CUSTOM_HDMI_HD
 *  } DRV_CUSTOM_HDMI_PCHD_MODE;
 *  @endcode
 *  @li@c  DRV_CUSTOM_HDMI_AUTO                              -
 *  @li@c  DRV_CUSTOM_HDMI_PC                                -
 *  @li@c  DRV_CUSTOM_HDMI_HD                                -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DRV_CUSTOM_HDMI_AUTO,
    DRV_CUSTOM_HDMI_PC,
    DRV_CUSTOM_HDMI_HD
} DRV_CUSTOM_HDMI_PCHD_MODE;

/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      DRV_CUSTOM_GAMMA_COLOR_TYPE_R,
 *      DRV_CUSTOM_GAMMA_COLOR_TYPE_G,
 *      DRV_CUSTOM_GAMMA_COLOR_TYPE_B
 *  } DRV_CUSTOM_GAMMA_COLOR_TYPE_T;
 *  @endcode
 *  @li@c  DRV_CUSTOM_GAMMA_COLOR_TYPE_R                     -
 *  @li@c  DRV_CUSTOM_GAMMA_COLOR_TYPE_G                     -
 *  @li@c  DRV_CUSTOM_GAMMA_COLOR_TYPE_B                     -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DRV_CUSTOM_GAMMA_COLOR_TYPE_R,
    DRV_CUSTOM_GAMMA_COLOR_TYPE_G,
    DRV_CUSTOM_GAMMA_COLOR_TYPE_B
} DRV_CUSTOM_GAMMA_COLOR_TYPE_T;

/*----------------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      DRV_CUSTOM_HDMI_RANGE_AUTO,
 *      DRV_CUSTOM_HDMI_RANGE_LIMIT,
 *      DRV_CUSTOM_HDMI_RANGE_FULL
 *  }DRV_CUSTOM_HDMI_RANGE_MODE;
 *  @endcode
 *
 *  @li@c  DRV_CUSTOM_HDMI_RANGE_AUTO   -
 *  @li@c  DRV_CUSTOM_HDMI_RANGE_LIMIT  -
 *  @li@c  DRV_CUSTOM_HDMI_RANGE_FULL   -
 */
/*----------------------------------------------------------------------------*/
typedef enum{
    DRV_CUSTOM_HDMI_RANGE_AUTO = 0,
    DRV_CUSTOM_HDMI_RANGE_LIMIT,
    DRV_CUSTOM_HDMI_RANGE_FULL
} DRV_CUSTOM_HDMI_RANGE_MODE;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _VID_INFO_T
 *  {
 *      DRV_CUSTOM_VID_INP_T        e_vid_inp;
 *      UINT16                      ui2Value;
 *      BOOL                        b_ignore_hw;
 *  } VID_INFO_T;
 *  @endcode
 *  @li@c  e_vid_inp                                         -
 *  @li@c  ui2Value                                          -
 *  @li@c  b_ignore_hw                                       -
 */
/*------------------------------------------------------------------*/
typedef struct _VID_INFO_T
{
    DRV_CUSTOM_VID_INP_T        e_vid_inp;
    UINT16                      ui2Value;
    BOOL                        b_ignore_hw;
} VID_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _GAMMA_INFO_T
 *  {
 *      DRV_CUSTOM_COLOR_TEMP_T     e_color_temp;
 *      UINT8                       ui1Channel;
 *      UINT8                       ui1Tbl[256];
 *  } GAMMA_INFO_T;
 *  @endcode
 *  @li@c  e_color_temp                                      -
 *  @li@c  ui1Channel                                        -
 *  @li@c  ui1Tbl[256]                                       -
 */
/*------------------------------------------------------------------*/
typedef struct _GAMMA_INFO_T
{
    DRV_CUSTOM_COLOR_TEMP_T     e_color_temp;
    UINT8                       ui1Channel;
    UINT8                       ui1Tbl[256];
} GAMMA_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _VID_NON_LINEAR_INFO_T
 *  {
 *      BOOL              b_auto;
 *      UINT32            ui4_path;
 *      UINT32            ui4_start;
 *      UINT32            ui4_slope;
 *      UINT32            ui4_middle;
 *      UINT32            ui4_stop;
 *  } VID_NON_LINEAR_INFO_T;
 *  @endcode
 *  @li@c  b_auto                                            -
 *  @li@c  ui4_path                                          -
 *  @li@c  ui4_start                                         -
 *  @li@c  ui4_slope                                         -
 *  @li@c  ui4_middle                                        -
 *  @li@c  ui4_stop                                          -
 */
/*------------------------------------------------------------------*/
typedef struct _VID_NON_LINEAR_INFO_T
{
    BOOL              b_auto;
    UINT32            ui4_path;      /* DRV_CUSTOM_VID_MAIN or DRV_CUSTOM_VID_SUB */
    UINT32            ui4_start;
    UINT32            ui4_slope;
    UINT32            ui4_middle;
    UINT32            ui4_stop;
} VID_NON_LINEAR_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _OUTRGB_INFO_T
 *  {
 *      UINT8                       ui1_r_value;
 *      UINT8                       ui1_g_value;
 *      UINT8                       ui1_b_value;
 *  } OUTRGB_INFO_T;
 *  @endcode
 *  @li@c  ui1_r_value                                       -
 *  @li@c  ui1_g_value                                       -
 *  @li@c  ui1_b_value                                       -
 */
/*------------------------------------------------------------------*/
typedef struct _OUTRGB_INFO_T
{
    UINT8                       ui1_r_value;
    UINT8                       ui1_g_value;
    UINT8                       ui1_b_value;
} OUTRGB_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _SMART_PIC_INFO_T
 *  {
 *      UINT8                       bSmartPicContrast;
 *      UINT8                       bSmartPicBacklight;
 *      UINT8                       bSmartPicBright;
 *      UINT8                       bSmartPicHue;
 *      UINT8                       bSmartPicSaturation;
 *      UINT8                       bSmartPicSharpness;
 *      UINT8                       bSmartPicColorTemp;
 *      UINT8                       bSmartPicFSByte0;
 *      UINT8                       bSmartPicFSByte1;
 *      UINT8                       bSmartPicFSByte2;
 *      UINT8                       bSmartPicFSByte3;
 *      UINT8                       bSmartPicRes1[2];
 *      UINT32                      wSmartPicFeatureSel[11];
 *      UINT8                       bSmartPicRes2[12];
 *  } SMART_PIC_INFO_T;
 *  @endcode
 *  @li@c  bSmartPicContrast                                 -
 *  @li@c  bSmartPicBacklight                                -
 *  @li@c  bSmartPicBright                                   -
 *  @li@c  bSmartPicHue                                      -
 *  @li@c  bSmartPicSaturation                               -
 *  @li@c  bSmartPicSharpness                                -
 *  @li@c  bSmartPicColorTemp                                -
 *  @li@c  bSmartPicFSByte0                                  -
 *  @li@c  bSmartPicFSByte1                                  -
 *  @li@c  bSmartPicFSByte2                                  -
 *  @li@c  bSmartPicFSByte3                                  -
 *  @li@c  bSmartPicRes1[2]                                  -
 *  @li@c  wSmartPicFeatureSel[11]                           -
 *  @li@c  bSmartPicRes2[12]                                 -
 */
/*------------------------------------------------------------------*/
typedef struct _SMART_PIC_INFO_T
{
    UINT8                       bSmartPicContrast;
    UINT8                       bSmartPicBacklight;
    UINT8                       bSmartPicBright;
    UINT8                       bSmartPicHue;
    UINT8                       bSmartPicSaturation;
    UINT8                       bSmartPicSharpness;
    UINT8                       bSmartPicColorTemp;
    UINT8                       bSmartPicFSByte0;
    UINT8                       bSmartPicFSByte1;
    UINT8                       bSmartPicFSByte2;
    UINT8                       bSmartPicFSByte3;
    UINT8                       bSmartPicRes1[2];
    UINT32                      wSmartPicFeatureSel[11];
    UINT8                       bSmartPicRes2[12];
} SMART_PIC_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _SMART_PIC_GROUP_INFO_T
 *  {
 *      SMART_PIC_INFO_T    aSmartPic[DRV_CUSTOM_VID_MAX_SMART_PIC_SZ];
 *      UINT8   bSmartPicPqRes[32];
 *  } SMART_PIC_GROUP_INFO_T;
 *  @endcode
 *  @li@c  SMART_PIC_INFO_T    aSmartPic[DRV_CUSTOM_VID_MAX_SMART_PIC_SZ]    -
 *  @li@c  bSmartPicPqRes[32]                                -
 */
/*------------------------------------------------------------------*/
typedef struct _SMART_PIC_GROUP_INFO_T
{
    SMART_PIC_INFO_T    aSmartPic[DRV_CUSTOM_VID_MAX_SMART_PIC_SZ];
    UINT8               bSmartPicPqRes[32];
} SMART_PIC_GROUP_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _PIC_INFO_T
 *  {
 *      UINT8    bPicContrast;
 *      UINT8    bPicColor;
 *      UINT8    bPicMotionLevel;
 *      UINT8    bPicAveragePixelLevel;
 *      UINT8    bPicNoiseLevel;
 *      UINT8    bPicEnvLightLevel;
 *      UINT8    bPicPowerMeter;
 *      UINT8    bPicRes1;
 *      UINT8    bPicRes2;
 *  } PIC_INFO_T;
 *  @endcode
 *  @li@c  bPicContrast                                      -
 *  @li@c  bPicColor                                         -
 *  @li@c  bPicMotionLevel                                   -
 *  @li@c  bPicAveragePixelLevel                             -
 *  @li@c  bPicNoiseLevel                                    -
 *  @li@c  bPicEnvLightLevel                                 -
 *  @li@c  bPicPowerMeter                                    -
 *  @li@c  bPicRes1                                          -
 *  @li@c  bPicRes2                                          -
 */
/*------------------------------------------------------------------*/
typedef struct _PIC_INFO_T
{
    UINT8    bPicContrast;
    UINT8    bPicColor;
    UINT8    bPicMotionLevel;
    UINT8    bPicAveragePixelLevel;
    UINT8    bPicNoiseLevel;
    UINT8    bPicEnvLightLevel;
    UINT8    bPicPowerMeter;
    UINT8    bPicRes1;
    UINT8    bPicRes2;
} PIC_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct PQ_DEMO_INFO_T
 *  {
 *      UINT8    bDemoMode;
 *      UINT16  wHStart;
 *      UINT16  wHEnd;
 *      UINT16  wVStart;
 *      UINT16  wVEnd;
 *      UINT8    bDemoModules;
 *  } PQ_DEMO_INFO_T;
 *  @endcode
 *  @li@c  bDemoMode                                         -
 *  @li@c  wHStart                                           -
 *  @li@c  wHEnd                                             -
 *  @li@c  wVStart                                           -
 *  @li@c  wVEnd                                             -
 *  @li@c  bDemoModules                                      -
 */
/*------------------------------------------------------------------*/
typedef struct PQ_DEMO_INFO_T
{
    UINT8    bDemoMode;
    UINT16  wHStart;
    UINT16  wHEnd;
    UINT16  wVStart;
    UINT16  wVEnd;
    UINT8    bDemoModules;
} PQ_DEMO_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _DRV_CUSTOM_GAMMA_VALUE_T
 *  {
 *      DRV_CUSTOM_GAMMA_COLOR_TYPE_T    e_gamma_color_type;
 *      UINT16                           ui2_gamma_level;
 *      UINT16                           ui2_gamma_value;
 *  } DRV_CUSTOM_GAMMA_VALUE_T;
 *  @endcode
 *  @li@c  e_gamma_color_type                                -
 *  @li@c  ui2_gamma_level                                   -
 *  @li@c  ui2_gamma_value                                   -
 */
/*------------------------------------------------------------------*/
typedef struct _DRV_CUSTOM_GAMMA_VALUE_T
{
    DRV_CUSTOM_GAMMA_COLOR_TYPE_T    e_gamma_color_type;    /* Input */
    UINT16                           ui2_gamma_level;       /* Input */
    UINT16                           ui2_gamma_value;       /* Input when set; Output when get */
} DRV_CUSTOM_GAMMA_VALUE_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _DRV_CUSTOM_GAMMA_DELTA_T
 *  {
 *      UINT16    ui2_green_level[4];
 *      UINT16    ui2_delta_r[4];
 *      UINT16    ui2_delta_b[4];
 *  } DRV_CUSTOM_GAMMA_DELTA_T;
 *  @endcode
 *  @li@c  ui2_green_level[4]                                -
 *  @li@c  ui2_delta_r[4]                                    -
 *  @li@c  ui2_delta_b[4]                                    -
 */
/*------------------------------------------------------------------*/
typedef struct _DRV_CUSTOM_GAMMA_DELTA_T
{
    UINT16    ui2_green_level[4];      /*Input*/
    UINT16    ui2_delta_r[4];          /*Input*/
    UINT16    ui2_delta_b[4];          /*Input*/
} DRV_CUSTOM_GAMMA_DELTA_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _DRV_CUSTOM_PATTERN_T
 *  {
 *      UINT8     ui1_OnOff;
 *      UINT16    ui2_r;
 *      UINT16    ui2_g;
 *      UINT16    ui2_b;
 *  } DRV_CUSTOM_PATTERN_T;
 *  @endcode
 *  @li@c  ui1_OnOff                                         -
 *  @li@c  ui2_r                                             -
 *  @li@c  ui2_g                                             -
 *  @li@c  ui2_b                                             -
 */
/*------------------------------------------------------------------*/
typedef struct _DRV_CUSTOM_PATTERN_T
{
    UINT8     ui1_OnOff;   /*Input*/
    UINT16    ui2_r;       /*Input*/
    UINT16    ui2_g;       /*Input*/
    UINT16    ui2_b;       /*Input*/
} DRV_CUSTOM_PATTERN_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _DRV_GAMMA_BUF_T
 *  {
 *      UINT8     ui1_Index;
 *      UINT16    ui2_rx;
 *      UINT16    ui2_gx;
 *      UINT16    ui2_bx;
 *  }   DRV_GAMMA_BUF_T;
 *  @endcode
 *  @li@c  ui1_Index                                         -
 *  @li@c  ui2_rx                                            -
 *  @li@c  ui2_gx                                            -
 *  @li@c  ui2_bx                                            -
 */
/*------------------------------------------------------------------*/
typedef struct _DRV_GAMMA_BUF_T
{
    UINT8     ui1_Index;    /*Input*/
    UINT16    ui2_rx;       /*Input*/
    UINT16    ui2_gx;       /*Input*/
    UINT16    ui2_bx;       /*Input*/
}   DRV_GAMMA_BUF_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _DRV_GAMMA_GETBUF_T
 *  {
 *      UINT8     ui1_GetIndex;
 *      UINT16    ui2_Getrx;
 *      UINT16    ui2_Getgx;
 *      UINT16    ui2_Getbx;
 *  }   DRV_GAMMA_GETBUF_T;
 *  @endcode
 *  @li@c  ui1_GetIndex                                      -
 *  @li@c  ui2_Getrx                                         -
 *  @li@c  ui2_Getgx                                         -
 *  @li@c  ui2_Getbx                                         -
 */
/*------------------------------------------------------------------*/
typedef struct _DRV_GAMMA_GETBUF_T
{
    UINT8     ui1_GetIndex; /*Input*/
    UINT16    ui2_Getrx;    /*Outout*/
    UINT16    ui2_Getgx;    /*Outout*/
    UINT16    ui2_Getbx;    /*Outout*/
}   DRV_GAMMA_GETBUF_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _DRV_CUSTOM_SCE_PATTERN_T
 *  {
 *      UINT8     ui1_OnOff;
 *      UINT16    ui2_Ydata;
 *  } DRV_CUSTOM_SCE_PATTERN_T;
 *  @endcode
 *  @li@c  ui1_OnOff                                         -
 *  @li@c  ui2_Ydata                                         -
 */
/*------------------------------------------------------------------*/
typedef struct _DRV_CUSTOM_SCE_PATTERN_T
{
    UINT8     ui1_OnOff;   /*Input*/
    UINT16    ui2_Ydata;   /*Input*/
} DRV_CUSTOM_SCE_PATTERN_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _DRV_CUSTOM_DBC_DEBUG_T
 *  {
 *      UINT8 ui1DBC_MAGIC;
 *      UINT8 ui1BrightLevel;
 *  }   DRV_CUSTOM_DBC_DEBUG_T;
 *  @endcode
 *  @li@c  ui1DBC_MAGIC                                      -
 *  @li@c  ui1BrightLevel                                    -
 */
/*------------------------------------------------------------------*/
typedef struct _DRV_CUSTOM_DBC_DEBUG_T
{
    UINT8 ui1DBC_MAGIC;
    UINT8 ui1BrightLevel;
}   DRV_CUSTOM_DBC_DEBUG_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _AUTO_BACKLIGHT_SETS_INFO_T
 *  {
 *      UINT8    ui1BacklightLevel;
 *      UINT8    ui1AutoLightLimiterLevel;
 *      UINT8    ui1PowerSavingMode;
 *      UINT8    ui1LightSensorOnOff;
 *      UINT8    ui1AdvContrastEnhancerMode;
 *      UINT8    ui1PictureMode;
 *  } AUTO_BACKLIGHT_SETS_INFO_T;
 *  @endcode
 *  @li@c  ui1BacklightLevel                                 -
 *  @li@c  ui1AutoLightLimiterLevel                          -
 *  @li@c  ui1PowerSavingMode                                -
 *  @li@c  ui1LightSensorOnOff                               -
 *  @li@c  ui1AdvContrastEnhancerMode                        -
 *  @li@c  ui1PictureMode                                    -
 */
/*------------------------------------------------------------------*/
typedef struct _AUTO_BACKLIGHT_SETS_INFO_T
{
    UINT8    ui1BacklightLevel;
    UINT8    ui1AutoLightLimiterLevel;
    UINT8    ui1PowerSavingMode;
    UINT8    ui1LightSensorOnOff;
    UINT8    ui1AdvContrastEnhancerMode;
    UINT8    ui1PictureMode;
} AUTO_BACKLIGHT_SETS_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _AUTO_COLORTEMP_SETS_INFO_T
 *  {
 *      UINT8     ui1AmbientSensorOnOff;
 *      UINT8     ui1CTIndex;
 *      UINT16    ui1Bias_RGain;
 *      UINT16    ui1Bias_GGain;
 *      UINT16    ui1Bias_BGain;
 *      UINT16    ui1Bias_ROffset;
 *      UINT16    ui1Bias_GOffset;
 *      UINT16    ui1Bias_BOffset;
 *  } AUTO_COLORTEMP_SETS_INFO_T;
 *  @endcode
 *  @li@c  ui1AmbientSensorOnOff                             -
 *  @li@c  ui1CTIndex                                        -
 *  @li@c  ui1Bias_RGain                                     -
 *  @li@c  ui1Bias_GGain                                     -
 *  @li@c  ui1Bias_BGain                                     -
 *  @li@c  ui1Bias_ROffset                                   -
 *  @li@c  ui1Bias_GOffset                                   -
 *  @li@c  ui1Bias_BOffset                                   -
 */
/*------------------------------------------------------------------*/
typedef struct _AUTO_COLORTEMP_SETS_INFO_T
{
    UINT8     ui1AmbientSensorOnOff;   /* regarding to UI Ambient Sensor On/Off */
    UINT8     ui1CTIndex;              /* regarding to UI Selection of Cool/W1/W2/Neutral */
    UINT16    ui1Bias_RGain;           /* regarding to UI set about User WB */
    UINT16    ui1Bias_GGain;           /* regarding to UI set about User WB */
    UINT16    ui1Bias_BGain;           /* regarding to UI set about User WB */
    UINT16    ui1Bias_ROffset;         /* regarding to UI set about User WB */
    UINT16    ui1Bias_GOffset;         /* regarding to UI set about User WB */
    UINT16    ui1Bias_BOffset;         /* regarding to UI set about User WB */

} AUTO_COLORTEMP_SETS_INFO_T;


/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _AUTO_COLORTEMP_GETS_INFO_T
 *  {
 *      UINT8     ui1AutoColortempOnOff;
 *  } AUTO_COLORTEMP_SETS_INFO_T;
 *  @endcode
 *  @li@c  ui1AmbientSensorOnOff                             -
 */
/*------------------------------------------------------------------*/
typedef struct _AUTO_COLORTEMP_GETS_INFO_T
{
    UINT8     ui1AutoColortempOnOff;   /* regarding to AutoColortemp On/Off */

} AUTO_COLORTEMP_GETS_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _VID_TYPE_DOT_TO_DOT_PATTERN_T
 *  {
 *      BOOL        b_on_off;
 *      UINT32      ui4_rgb_value;
 *  } VID_TYPE_DOT_TO_DOT_PATTERN_T;
 *  @endcode
 *  @li@c  b_on_off                                          -
 *  @li@c  ui4_rgb_value                                     -
 */
/*------------------------------------------------------------------*/
typedef struct _VID_TYPE_DOT_TO_DOT_PATTERN_T
{
    BOOL        b_on_off;         /* 1 => Turn On, 0 => Turn Off */
    UINT32      ui4_rgb_value;
} VID_TYPE_DOT_TO_DOT_PATTERN_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _LVDS_SPREAD_SPECTRUM_INFO_T
 *  {
 *      UINT32    u4_freq;
 *      UINT32    u4_permillage;
 *  } LVDS_SPREAD_SPECTRUM_INFO_T;
 *  @endcode
 *  @li@c  u4_freq                                           -
 *  @li@c  u4_permillage                                     -
 */
/*------------------------------------------------------------------*/
typedef struct _LVDS_SPREAD_SPECTRUM_INFO_T
{
    UINT32    u4_freq;
    UINT32    u4_permillage;
} LVDS_SPREAD_SPECTRUM_INFO_T;

/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      DRV_CUSTOM_LVDS_FORMAT_JEIDA = 0,
 *      DRV_CUSTOM_LVDS_FORMAT_NS
 *  } DRV_CUSTOM_LVDS_FORMAT_T;
 *  @endcode
 *  @li@c  DRV_CUSTOM_LVDS_FORMAT_JEIDA                      -
 *  @li@c  DRV_CUSTOM_LVDS_FORMAT_NS                         -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DRV_CUSTOM_LVDS_FORMAT_JEIDA = 0,
    DRV_CUSTOM_LVDS_FORMAT_NS
} DRV_CUSTOM_LVDS_FORMAT_T;

/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      DRV_CUSTOM_LVDS_BIT_DEPTH_18BIT = 0,
 *      DRV_CUSTOM_LVDS_BIT_DEPTH_24BIT,
 *      DRV_CUSTOM_LVDS_BIT_DEPTH_30BIT
 *  } DRV_CUSTOM_LVDS_BIT_DEPTH_T;
 *  @endcode
 *  @li@c  DRV_CUSTOM_LVDS_BIT_DEPTH_18BIT                   -
 *  @li@c  DRV_CUSTOM_LVDS_BIT_DEPTH_24BIT                   -
 *  @li@c  DRV_CUSTOM_LVDS_BIT_DEPTH_30BIT                   -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DRV_CUSTOM_LVDS_BIT_DEPTH_18BIT = 0,
    DRV_CUSTOM_LVDS_BIT_DEPTH_24BIT,
    DRV_CUSTOM_LVDS_BIT_DEPTH_30BIT
} DRV_CUSTOM_LVDS_BIT_DEPTH_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _VID_TYPE_PANEL_RESOLUTION_T
 *  {
 *      UINT32    ui4_width;
 *      UINT32    ui4_height;
 *  } VID_TYPE_PANEL_RESOLUTION_T;
 *  @endcode
 *  @li@c  ui4_width                                         -
 *  @li@c  ui4_height                                        -
 */
/*------------------------------------------------------------------*/
typedef struct _VID_TYPE_PANEL_RESOLUTION_T
{
    UINT32    ui4_width;
    UINT32    ui4_height;
} VID_TYPE_PANEL_RESOLUTION_T;

/*-----------------------------------------------------------------------------
    <<Video Decoder>>  macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      DRV_CUSTOM_VID_DEC_TYPE_BEGIN = 0,
 *      DRV_CUSTOM_VID_DEC_TYPE_BOOT_LOGO_CTRL = DRV_CUSTOM_ARG_USE_REF_OFFSET,
 *      DRV_CUSTOM_VID_DEC_TYPE_BOOT_LOGO_ADDR,
 *      DRV_CUSTOM_VID_DEC_TYPE_END = DRV_CUSTOM_ARG_END_OFFSET
 *  } DRV_CUSTOM_VID_DEC_TYPE_T;
 *  @endcode
 *  @li@c  DRV_CUSTOM_VID_DEC_TYPE_BEGIN                     -
 *  @li@c  DRV_CUSTOM_VID_DEC_TYPE_BOOT_LOGO_CTRL            -
 *  @li@c  DRV_CUSTOM_VID_DEC_TYPE_BOOT_LOGO_ADDR            -
 *  @li@c  DRV_CUSTOM_VID_DEC_TYPE_END                       -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DRV_CUSTOM_VID_DEC_TYPE_BEGIN = 0,

    /*
     * The argument of the following enumerations SHOULD BE a pointer
     */

    DRV_CUSTOM_VID_DEC_TYPE_BOOT_LOGO_CTRL = DRV_CUSTOM_ARG_USE_REF_OFFSET,    /* VID_DEC_BOOT_LOGO_GET_T, boot logo control */
    DRV_CUSTOM_VID_DEC_TYPE_BOOT_LOGO_ADDR,                                    /* VID_DEC_BOOT_LOGO_ADDR_GET_T, boot logo address */
    DRV_CUSTOM_VID_DEC_TYPE_END = DRV_CUSTOM_ARG_END_OFFSET
} DRV_CUSTOM_VID_DEC_TYPE_T;


/*-----------------------------------------------------------------------------
    <<EXTMJC>>  macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

#define DRV_CUSTOM_EXTMJC_OSD_MODE_NORMAL                       ((UINT8)    0)    /**<        */
#define DRV_CUSTOM_EXTMJC_OSD_MODE_DMA                          ((UINT8)    1)    /**<        */
#define DRV_CUSTOM_EXTMJC_OSD_MODE_GRAPHIC                      ((UINT8)    2)    /**<        */

/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      DRV_CUSTOM_EXTMJC_TYPE_BEGIN = 0,
 *      DRV_CUSTOM_EXTMJC_TYPE_IS_ENABLE = DRV_CUSTOM_ARG_USE_REF_OFFSET,
 *      DRV_CUSTOM_EXTMJC_TYPE_SET_OSD_MODE,
 *      DRV_CUSTOM_EXTMJC_TYPE_END = DRV_CUSTOM_ARG_END_OFFSET
 *  } DRV_CUSTOM_EXTMJC_TYPE_T;
 *  @endcode
 *  @li@c  DRV_CUSTOM_EXTMJC_TYPE_BEGIN                -
 *  @li@c  DRV_CUSTOM_EXTMJC_TYPE_IS_ENABLE            -
 *  @li@c  DRV_CUSTOM_EXTMJC_TYPE_SET_OSD_MODE         -
 *  @li@c  DRV_CUSTOM_EXTMJC_TYPE_END                  -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DRV_CUSTOM_EXTMJC_TYPE_BEGIN = 0,

    /*
     * The argument of the following enumerations SHOULD BE a pointer
     */

    DRV_CUSTOM_EXTMJC_TYPE_IS_ENABLE = DRV_CUSTOM_ARG_USE_REF_OFFSET,    /* UINT8, get EXTMJC is enable or not */
    DRV_CUSTOM_EXTMJC_TYPE_SET_OSD_MODE,                                 /* EXTMJC_TYPE_SET_OSD_MODE_T, set EXTMJC OSD mode */

    DRV_CUSTOM_EXTMJC_TYPE_END = DRV_CUSTOM_ARG_END_OFFSET
} DRV_CUSTOM_EXTMJC_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _EXTMJC_TYPE_SET_OSD_MODE_T
 *  {
 *      UINT8    ui1_osd_mode;
 *      CHAR     s_msg_error[512];
 *  } EXTMJC_TYPE_SET_OSD_MODE_T;
 *  @endcode
 *  @li@c  ui1_osd_mode                                  -
 *  @li@c  s_msg_error                                   -
 */
/*------------------------------------------------------------------*/
typedef struct _EXTMJC_TYPE_SET_OSD_MODE_T
{
    UINT8    ui1_osd_mode;
    CHAR     s_msg_error[512];
} EXTMJC_TYPE_SET_OSD_MODE_T;

/*-----------------------------------------------------------------------------
    <<Misc>>  macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

#define MISC_TYPE_ALIVE_CHECK_CTRL_SDM_MODE                 MAKE_BIT_MASK_32(3)    /**<        */
#define MISC_TYPE_ALIVE_CHECK_CTRL_FACTORY_MODE             MAKE_BIT_MASK_32(5)    /**<        */
#define MISC_TYPE_ALIVE_CHECK_CTRL_CSM_MODE                 MAKE_BIT_MASK_32(6)    /**<        */
#define MISC_TYPE_ALIVE_CHECK_CTRL_SAM_MODE                 MAKE_BIT_MASK_32(7)    /**<        */
#define MISC_TYPE_ALIVE_CHECK_CTRL_UART_PANEL_MODE          MAKE_BIT_MASK_32(8)    /**<        */
#define MISC_TYPE_ALIVE_CHECK_CTRL_USB_MODE                 MAKE_BIT_MASK_32(9)    /**<        */

#define MISC_TYPE_ALIVE_CHECK_CTRL_MASK_ALL          \
      ( MISC_TYPE_ALIVE_CHECK_CTRL_SDM_MODE        | \
        MISC_TYPE_ALIVE_CHECK_CTRL_FACTORY_MODE    | \
        MISC_TYPE_ALIVE_CHECK_CTRL_CSM_MODE        | \
        MISC_TYPE_ALIVE_CHECK_CTRL_SAM_MODE        | \
        MISC_TYPE_ALIVE_CHECK_CTRL_UART_PANEL_MODE | \
        MISC_TYPE_ALIVE_CHECK_CTRL_USB_MODE)             /**<        */

#define MISC_TYPE_TV_STATUS_TV_ON                           MAKE_BIT_MASK_32(0)    /**<        */

/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      DRV_CUSTOM_MISC_TYPE_BEGIN = 0,
 *      DRV_CUSTOM_MISC_TYPE_ALIVE_CHECK_CTRL = DRV_CUSTOM_ARG_NO_REF_OFFSET,
 *      DRV_CUSTOM_MISC_TYPE_CEC_MENU_LANG_SET,
 *      DRV_CUSTOM_MISC_TYPE_CEC_ENABLE_SET,
 *      DRV_CUSTOM_MISC_TYPE_CEC_ONE_TOUCH_PLAY_EN_SET,
 *      DRV_CUSTOM_MISC_TYPE_PANEL_NAME = DRV_CUSTOM_ARG_USE_REF_OFFSET,
 *      DRV_CUSTOM_MISC_TYPE_TUNER_SIF,
 *      DRV_CUSTOM_MISC_TYPE_HDMI_EDID,
 *      DTV_CUSTOM_MISC_TYPE_FORCE_MUTE_OFF,
 *      DRV_CUSTOM_MISC_TYPE_SIF_ADVANCED,
 *      DRV_CUSTOM_MISC_TYPE_TVD_MCDONE_CNT,
 *      DRV_CUSTOM_MISC_TYPE_PALNEL_MODE,
 *      DRV_CUSTOM_MISC_TYPE_HDMI_HPD_LOW,
 *      DRV_CUSTOM_MISC_TYPE_HDMI_BYPASS_AVIINFO,
 *      DRV_CUSTOM_MISC_TYPE_HDMI_KSV_VALUE,
 *      DRV_CUSTOM_MISC_TYPE_HDMI_DEEPCOLOR_STATUS,
 *      DRV_CUSTOM_MISC_TYPE_AUTO_SEARCH,
 *      DRV_CUSTOM_EEPROM_READ,
 *      DRV_CUSTOM_EEPROM_WRITE,
 *      DRV_CUSTOM_GET_NTPV_EEPROM_OFFSET,
 *      DRV_CUSTOM_GET_COUNTRY,
 *      DRV_CUSTOM_SET_COUNTRY,
 *      DRV_CUSTOM_MISC_SET_IMAGE_BANK,
 *      DRV_CUSTOM_MISC_QUERY_IMAGE_BANK,
 *      DRV_CUSTOM_MISC_SET_8032UP_BANK,
 *      DRV_CUSTOM_MISC_QUERY_8032UP_BANK,
 *      DRV_CUSTOM_MISC_TYPE_STARTUP_SOUND_STATUS,
 *      DRV_CUSTOM_MISC_TYPE_STARTUP_LOGO_STATUS,
 *      DRV_CUSTOM_MISC_TYPE_GET_T8032_VER,
 *      DRV_CUSTOM_MISC_TYPE_OSD_STATUS_CHG,
 *      DRV_CUSTOM_DRAM_CFG_SET,
 *      DRV_CUSTOM_DRAM_CFG_GET,
 *      DRV_CUSTOM_MISC_TYPE_CEC_MENU_LANG_GET,
 *      DRV_CUSTOM_MISC_TYPE_CEC_ENABLE_GET,
 *      DRV_CUSTOM_MISC_TYPE_CEC_ONE_TOUCH_PLAY_EN_GET,
 *      DRV_CUSTOM_SET_CRNT_INP_SRC_INFO,
 *      DRV_CUSTOM_MISC_TYPE_VDP_CB_ENABLE_SET,
 *      DRV_CUSTOM_MISC_TYPE_HDMI_HDCP,
 *      DRV_CUSTOM_MISC_TYPE_VGA_EDID,
 *      DRV_CUSTOM_MISC_TYPE_LOADER_INFO,
 *      DRV_CUSTOM_MISC_TYPE_LED_MODE,
 *      DRV_CUSTOM_MISC_TYPE_NAND_ERASE,
 *      DRV_CUSTOM_MISC_TYPE_NAND_WRITE,
 *      DRV_CUSTOM_MISC_TYPE_NAND_BULK_WRITE,
 *      DRV_CUSTOM_MISC_TYPE_NAND_NOR_WRITE,
 *      DRV_CUSTOM_MISC_TYPE_TV_STATUS,
 *      DRV_CUSTOM_MISC_TYPE_END = DRV_CUSTOM_ARG_END_OFFSET
 *  } DRV_CUSTOM_MISC_TYPE_T;
 *  @endcode
 *  @li@c  DRV_CUSTOM_MISC_TYPE_BEGIN                        -
 *  @li@c  DRV_CUSTOM_MISC_TYPE_ALIVE_CHECK_CTRL             -
 *  @li@c  DRV_CUSTOM_MISC_TYPE_CEC_MENU_LANG_SET            -
 *  @li@c  DRV_CUSTOM_MISC_TYPE_CEC_ENABLE_SET               -
 *  @li@c  DRV_CUSTOM_MISC_TYPE_CEC_ONE_TOUCH_PLAY_EN_SET    -
 *  @li@c  DRV_CUSTOM_MISC_TYPE_PANEL_NAME                   -
 *  @li@c  DRV_CUSTOM_MISC_TYPE_TUNER_SIF                    -
 *  @li@c  DRV_CUSTOM_MISC_TYPE_HDMI_EDID                    -
 *  @li@c  DTV_CUSTOM_MISC_TYPE_FORCE_MUTE_OFF               -
 *  @li@c  DRV_CUSTOM_MISC_TYPE_SIF_ADVANCED                 -
 *  @li@c  DRV_CUSTOM_MISC_TYPE_TVD_MCDONE_CNT               -
 *  @li@c  DRV_CUSTOM_MISC_TYPE_PALNEL_MODE                  -
 *  @li@c  DRV_CUSTOM_MISC_TYPE_HDMI_HPD_LOW                 -
 *  @li@c  DRV_CUSTOM_MISC_TYPE_HDMI_BYPASS_AVIINFO          -
 *  @li@c  DRV_CUSTOM_MISC_TYPE_HDMI_KSV_VALUE               -
 *  @li@c  DRV_CUSTOM_MISC_TYPE_HDMI_DEEPCOLOR_STATUS        -
 *  @li@c  DRV_CUSTOM_MISC_TYPE_AUTO_SEARCH                  -
 *  @li@c  DRV_CUSTOM_EEPROM_READ                            -
 *  @li@c  DRV_CUSTOM_EEPROM_WRITE                           -
 *  @li@c  DRV_CUSTOM_GET_NTPV_EEPROM_OFFSET                 -
 *  @li@c  DRV_CUSTOM_GET_COUNTRY                            -
 *  @li@c  DRV_CUSTOM_SET_COUNTRY                            -
 *  @li@c  DRV_CUSTOM_MISC_SET_IMAGE_BANK                    -
 *  @li@c  DRV_CUSTOM_MISC_QUERY_IMAGE_BANK                  -
 *  @li@c  DRV_CUSTOM_MISC_SET_8032UP_BANK                   -
 *  @li@c  DRV_CUSTOM_MISC_QUERY_8032UP_BANK                 -
 *  @li@c  DRV_CUSTOM_MISC_TYPE_STARTUP_SOUND_STATUS         -
 *  @li@c  DRV_CUSTOM_MISC_TYPE_STARTUP_LOGO_STATUS          -
 *  @li@c  DRV_CUSTOM_MISC_TYPE_GET_T8032_VER                -
 *  @li@c  DRV_CUSTOM_MISC_TYPE_OSD_STATUS_CHG               -
 *  @li@c  DRV_CUSTOM_DRAM_CFG_SET                           -
 *  @li@c  DRV_CUSTOM_DRAM_CFG_GET                           -
 *  @li@c  DRV_CUSTOM_MISC_TYPE_CEC_MENU_LANG_GET            -
 *  @li@c  DRV_CUSTOM_MISC_TYPE_CEC_ENABLE_GET               -
 *  @li@c  DRV_CUSTOM_MISC_TYPE_CEC_ONE_TOUCH_PLAY_EN_GET    -
 *  @li@c  DRV_CUSTOM_SET_CRNT_INP_SRC_INFO                  -
 *  @li@c  DRV_CUSTOM_MISC_TYPE_VDP_CB_ENABLE_SET            -
 *  @li@c  DRV_CUSTOM_MISC_TYPE_HDMI_HDCP                    -
 *  @li@c  DRV_CUSTOM_MISC_TYPE_VGA_EDID                     -
 *  @li@c  DRV_CUSTOM_MISC_TYPE_LOADER_INFO                  -
 *  @li@c  DRV_CUSTOM_MISC_TYPE_LED_MODE                     -
 *  @li@c  DRV_CUSTOM_MISC_TYPE_NAND_ERASE                   -
 *  @li@c  DRV_CUSTOM_MISC_TYPE_NAND_WRITE                   -
 *  @li@c  DRV_CUSTOM_MISC_TYPE_NAND_BULK_WRITE              -
 *  @li@c  DRV_CUSTOM_MISC_TYPE_NOR_BULK_WRITE               -
 *  @li@c  DRV_CUSTOM_MISC_TYPE_END                          -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DRV_CUSTOM_MISC_TYPE_BEGIN = 0,

    /*
     * The argument of the following enumerations SHOULD NOT be a pointer
     */

    DRV_CUSTOM_MISC_TYPE_ALIVE_CHECK_CTRL = DRV_CUSTOM_ARG_NO_REF_OFFSET,    /* UINT32, alive check control */

    /* CEC Configuration Set */
    DRV_CUSTOM_MISC_TYPE_CEC_MENU_LANG_SET,                                  /* UINT32, set CEC menu language */
    DRV_CUSTOM_MISC_TYPE_CEC_ENABLE_SET,                                     /* UINT32, set CEC enable status */
    DRV_CUSTOM_MISC_TYPE_CEC_ONE_TOUCH_PLAY_EN_SET,                          /* UINT32, set one touch play enable status */

    /*
     * The argument of the following enumerations SHOULD BE a pointer
     */

    DRV_CUSTOM_MISC_TYPE_PANEL_NAME = DRV_CUSTOM_ARG_USE_REF_OFFSET,         /* CHAR*, Panel Name */
    DRV_CUSTOM_MISC_TYPE_TUNER_SIF,                                          /* MISC_TYPE_TUNER_SIF_T, tuner SIF */
    DRV_CUSTOM_MISC_TYPE_HDMI_EDID,                                          /* MISC_TYPE_HDMI_EDID_T, HDMI EDID */
    DTV_CUSTOM_MISC_TYPE_FORCE_MUTE_OFF,                                     /* BOOL, Force to unmute video */
    DRV_CUSTOM_MISC_TYPE_SIF_ADVANCED,                                       /* MISC_TYPE_SIF_ADVANCED_T, SIF advance */

    DRV_CUSTOM_MISC_TYPE_TVD_MCDONE_CNT,                                     /* UINT8, TVD */
    DRV_CUSTOM_MISC_TYPE_PALNEL_MODE,                                        /* PANEL_SELECTION_CONFIG_T, Panel mode */

    DRV_CUSTOM_MISC_TYPE_HDMI_HPD_LOW,                                       /* UINT32, HDMI HPD adjustment */
    DRV_CUSTOM_MISC_TYPE_HDMI_BYPASS_AVIINFO,                                /* UINT8, HDMI bypass AV info */
    DRV_CUSTOM_MISC_TYPE_HDMI_KSV_VALUE,                                     /* MISC_TYPE_HDMI_EDID_T, HDMI KSV value */
    DRV_CUSTOM_MISC_TYPE_HDMI_DEEPCOLOR_STATUS,                              /* UINT8, HDMI deep color status */
    DRV_CUSTOM_MISC_TYPE_AUTO_SEARCH,                                        /* AUTO_SEARCH_MODE_T, auto search mode */

    DRV_CUSTOM_GET_NTPV_EEPROM_OFFSET,                                       /* UINT16, get NTPV EEPROM offset */
    DRV_CUSTOM_GET_COUNTRY,                                                  /* UINT8, get country */
    DRV_CUSTOM_SET_COUNTRY,                                                  /* UINT8, set country */

    /* set & query image bank */
    DRV_CUSTOM_MISC_SET_IMAGE_BANK,                                          /* UINT32, set main image bank */
    DRV_CUSTOM_MISC_QUERY_IMAGE_BANK,                                        /* UINT32, query main image bank */

    /* set & query 8032uP bank */
    DRV_CUSTOM_MISC_SET_8032UP_BANK,                                         /* UINT32, set T8032 up image bank */
    DRV_CUSTOM_MISC_QUERY_8032UP_BANK,                                       /* UINT32, query T8032 up image bank */

    DRV_CUSTOM_MISC_TYPE_STARTUP_SOUND_STATUS,                               /* BOOL, get startup sound status */
    DRV_CUSTOM_MISC_TYPE_STARTUP_LOGO_STATUS,                                /* BOOL, get startup logo status */
    DRV_CUSTOM_MISC_TYPE_GET_T8032_VER,                                      /* CHAR*, get T8032 version */
    DRV_CUSTOM_MISC_TYPE_OSD_STATUS_CHG,                                     /* BOOL, App. uses this type to notify driver that OSD status is changed */

    DRV_CUSTOM_DRAM_CFG_SET,                                                 /* DRV_CUST_DRAM_CFG_T, DRAM Configuration Set */
    DRV_CUSTOM_DRAM_CFG_GET,                                                 /* DRV_CUST_DRAM_CFG_T, DRAM Configuration Get */

    /* CEC Configuration Get */
    DRV_CUSTOM_MISC_TYPE_CEC_MENU_LANG_GET,                                  /* UINT32, get CEC menu language */
    DRV_CUSTOM_MISC_TYPE_CEC_ENABLE_GET,                                     /* UINT32, get CEC enable status */
    DRV_CUSTOM_MISC_TYPE_CEC_ONE_TOUCH_PLAY_EN_GET,                          /* UINT32, get one touch play enable status */

    DRV_CUSTOM_SET_CRNT_INP_SRC_INFO,                                        /* DRV_CUSTOM_MISC_CRNT_INP_SRC_INFO_T, set current input source info, including type & port number */
    DRV_CUSTOM_MISC_TYPE_VDP_CB_ENABLE_SET,                                  /* BOOL, disable VDP callback */
    DRV_CUSTOM_MISC_TYPE_HDMI_HDCP,                                          /* MISC_TYPE_HDMI_HDCP_T, HDMI HDCP Get/Set */
    DRV_CUSTOM_MISC_TYPE_VGA_EDID,                                           /* MISC_TYPE_VGA_EDID_T, VGA EDID Get/Set */
    DRV_CUSTOM_MISC_TYPE_LOADER_INFO,                                        /* DRV_CUSTOM_MISC_LOADER_INFO_T, get loadre info. */
    DRV_CUSTOM_MISC_TYPE_LED_MODE,                                           /* MISC_TYPE_LED_DISPLAY_T, control the LED display mode. */
    DRV_CUSTOM_MISC_TYPE_LED_NOTIFICATION,                                   /* MISC_TYPE_LED_NOTIFY_T, control the LED notification */

    DRV_CUSTOM_MISC_TYPE_NAND_ERASE,                                         /* UINT8, erase NAND flash by partition ID */
    DRV_CUSTOM_MISC_TYPE_NAND_WRITE,                                         /* MISC_TYPE_NAND_FLASH_WRITE_T, write NAND flash by partition ID */
    DRV_CUSTOM_MISC_TYPE_NAND_BULK_WRITE,                                    /* MISC_TYPE_NAND_FLASH_BULK_WRITE_T, bulk write NAND flash by partition ID */
    DRV_CUSTOM_MISC_TYPE_NOR_BULK_WRITE,                                     /* MISC_TYPE_NOR_FLASH_BULK_WRITE_T, bulk write NOR flash by partition ID */

    DRV_CUSTOM_MISC_TYPE_TV_STATUS,                                          /* UINT32, TV status */

    DRV_CUSTOM_MISC_TYPE_END = DRV_CUSTOM_ARG_END_OFFSET
} DRV_CUSTOM_MISC_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      PANEL_SELECTION_MODE_AUTO,
 *      PANEL_SELECTION_MODE_MANUAL,
 *      PANEL_SELECTION_MODE_LAST
 *  } PANEL_SELECTION_MODE_T;
 *  @endcode
 *  @li@c  PANEL_SELECTION_MODE_AUTO                         -
 *  @li@c  PANEL_SELECTION_MODE_MANUAL                       -
 *  @li@c  PANEL_SELECTION_MODE_LAST                         -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    PANEL_SELECTION_MODE_AUTO,
    PANEL_SELECTION_MODE_MANUAL,
    PANEL_SELECTION_MODE_LAST
} PANEL_SELECTION_MODE_T;

/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      DRV_CUSTOM_MISC_LED_GENERIC_ON,
 *      DRV_CUSTOM_MISC_LED_GENERIC_OFF,
 *      DRV_CUSTOM_MISC_LED_GENERIC_CUST_1
 *  } DRV_CUSTOM_MISC_LED_GENERIC_STATE_T;
 *  @endcode
 *  @li@c  DRV_CUSTOM_MISC_LED_GENERIC_ON                    -
 *  @li@c  DRV_CUSTOM_MISC_LED_GENERIC_OFF                   -
 *  @li@c  DRV_CUSTOM_MISC_LED_GENERIC_CUST_1                -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DRV_CUSTOM_MISC_LED_GENERIC_ON,
    DRV_CUSTOM_MISC_LED_GENERIC_OFF,
    DRV_CUSTOM_MISC_LED_GENERIC_CUST_1
} DRV_CUSTOM_MISC_LED_GENERIC_STATE_T;

/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      DRV_CUSTOM_MISC_LED_POWER_ON,
 *      DRV_CUSTOM_MISC_LED_POWER_OFF
 *  } DRV_CUSTOM_MISC_LED_POWER_STATE_T;
 *  @endcode
 *  @li@c  DRV_CUSTOM_MISC_LED_POWER_ON                    -
 *  @li@c  DRV_CUSTOM_MISC_LED_POWER_OFF                   -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DRV_CUSTOM_MISC_LED_POWER_ON,
    DRV_CUSTOM_MISC_LED_POWER_OFF
} DRV_CUSTOM_MISC_LED_POWER_STATE_T;

/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      DRV_CUSTOM_MISC_LED_STANDBY_ON,
 *      DRV_CUSTOM_MISC_LED_STANDBY_OFF
 *  } DRV_CUSTOM_MISC_LED_STANDBY_STATE_T;
 *  @endcode
 *  @li@c  DRV_CUSTOM_MISC_LED_STANDBY_ON                    -
 *  @li@c  DRV_CUSTOM_MISC_LED_STANDBY_OFF                   -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DRV_CUSTOM_MISC_LED_STANDBY_ON,
    DRV_CUSTOM_MISC_LED_STANDBY_OFF
} DRV_CUSTOM_MISC_LED_STANDBY_STATE_T;

/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      DRV_CUSTOM_MISC_LED_TIMER_CUST_1,
 *      DRV_CUSTOM_MISC_LED_TIMER_CUST_2
 *      DRV_CUSTOM_MISC_LED_TIMER_ALL,
 *      DRV_CUSTOM_MISC_LED_TIMER_OFF
 *  } DRV_CUSTOM_MISC_LED_TIMER_STATE_T;
 *  @endcode
 *  @li@c  DRV_CUSTOM_MISC_LED_TIMER_CUST_1                -
 *  @li@c  DRV_CUSTOM_MISC_LED_TIMER_CUST_2                -
 *  @li@c  DRV_CUSTOM_MISC_LED_TIMER_ALL                   -
 *  @li@c  DRV_CUSTOM_MISC_LED_TIMER_OFF                   -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DRV_CUSTOM_MISC_LED_TIMER_CUST_1,
    DRV_CUSTOM_MISC_LED_TIMER_CUST_2,
    DRV_CUSTOM_MISC_LED_TIMER_ALL,
    DRV_CUSTOM_MISC_LED_TIMER_OFF
} DRV_CUSTOM_MISC_LED_TIMER_STATE_T;

/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      DRV_CUSTOM_MISC_LED_GENERIC = 0,
 *      DRV_CUSTOM_MISC_LED_POWER,
 *      DRV_CUSTOM_MISC_LED_STANDBY,
 *      DRV_CUSTOM_MISC_LED_TIMER
 *  } DRV_CUSTOM_MISC_LED_TYPE_T;
 *  @endcode
 *  @li@c  DRV_CUSTOM_MISC_LED_GENERIC                  -
 *  @li@c  DRV_CUSTOM_MISC_LED_POWER                    -
 *  @li@c  DRV_CUSTOM_MISC_LED_STANDBY                  -
 *  @li@c  DRV_CUSTOM_MISC_LED_TIMER                    -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DRV_CUSTOM_MISC_LED_GENERIC = 0,
    DRV_CUSTOM_MISC_LED_POWER,
    DRV_CUSTOM_MISC_LED_STANDBY,
    DRV_CUSTOM_MISC_LED_TIMER
} DRV_CUSTOM_MISC_LED_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      DRV_CUSTOM_MISC_LED_NO_EVT = 0,
 *      DRV_CUSTOM_MISC_LED_STANDBY_EVT,
 *      DRV_CUSTOM_MISC_LED_BOOT_EVT,
 *      DRV_CUSTOM_MISC_LED_QUIET_BOOT_EVT,
 *      DRV_CUSTOM_MISC_LED_UPDATE_FLASHING_EVT
 *      DRV_CUSTOM_MISC_LED_STEADY_EVT,
 *  } DRV_CUSTOM_MISC_LED_NOTIFY_T;
 *  @endcode
 *  @li@c  DRV_CUSTOM_MISC_LED_NO_EVT                  -
 *  @li@c  DRV_CUSTOM_MISC_LED_STANDBY_EVT             -
 *  @li@c  DRV_CUSTOM_MISC_LED_BOOT_EVT                -
 *  @li@c  DRV_CUSTOM_MISC_LED_QUIET_BOOT_EVT          -
 *  @li@c  DRV_CUSTOM_MISC_LED_UPDATE_FLASHING_EVT     -
 *  @li@c  DRV_CUSTOM_MISC_LED_STEADY_EVT              -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DRV_CUSTOM_MISC_LED_NO_EVT = 0,
    DRV_CUSTOM_MISC_LED_STANDBY_EVT,
    DRV_CUSTOM_MISC_LED_BOOT_EVT,
    DRV_CUSTOM_MISC_LED_QUIET_BOOT_EVT,
    DRV_CUSTOM_MISC_LED_UPDATE_FLASHING_EVT,
    DRV_CUSTOM_MISC_LED_STEADY_EVT
} DRV_CUSTOM_MISC_LED_NOTIFY_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _MISC_TYPE_TUNER_SIF_T
 *  {
 *      UINT32      ui4_addr;
 *      UINT8       ui1_offset_len;
 *      UINT32      ui4_offset;
 *      UINT8       ui1_data;
 *  } MISC_TYPE_TUNER_SIF_T;
 *  @endcode
 *  @li@c  ui4_addr                                          -
 *  @li@c  ui1_offset_len                                    -
 *  @li@c  ui4_offset                                        -
 *  @li@c  ui1_data                                          -
 */
/*------------------------------------------------------------------*/
typedef struct _MISC_TYPE_TUNER_SIF_T
{
    UINT32      ui4_addr;              /* device address */
    UINT8       ui1_offset_len;        /* lenth of ui4_offset, from 0 up to 3 */
    UINT32      ui4_offset;            /* sub-address (or called offset) */
    UINT8       ui1_data;
} MISC_TYPE_TUNER_SIF_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _MISC_TYPE_HDMI_EDID_T
 *  {
 *      UINT8 ui1_hdmi_comp_id;
 *      UINT8 aui1_edid_buf[256];
 *      UINT8 aui1_hdmi_ksv[5];
 *  #ifdef CC_Support_Sil9285
 *      UINT8 aui1_NVRAMboottable_buf[64];
 *  #endif
 *  } MISC_TYPE_HDMI_EDID_T;
 *  @endcode
 *  @li@c  ui1_hdmi_comp_id                                  -
 *  @li@c  aui1_edid_buf[256]                                -
 *  @li@c  aui1_hdmi_ksv[5]                                  -
 *  @li@c  aui1_NVRAMboottable_buf[64]                       -
 */
/*------------------------------------------------------------------*/
typedef struct _MISC_TYPE_HDMI_EDID_T
{
    UINT8 ui1_hdmi_comp_id;                      /* HDMI port comp. ID. */
    UINT8 aui1_edid_buf[256];                    /* Buffer to store EDID data. */
    UINT8 aui1_hdmi_ksv[5];                      /* Buffer to store BKSV */
#ifdef CC_Support_Sil9285
    UINT8 aui1_NVRAMboottable_buf[64];           /* Buffer to store NVRAMboottable data. */
#endif
} MISC_TYPE_HDMI_EDID_T;

/*----------------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _EDID_T
 *  {
 *      UINT32      u1EDID_ID;
 *      UINT8       u1SubAddr;
 *      UINT8       ui1_data;
 *  } EDID_T;
 *  @endcode
 *
 *  @li@c  u1EDID_ID -
 *  @li@c  u1SubAddr -
 *  @li@c  ui1_data  -
 */
/*----------------------------------------------------------------------------*/
typedef struct _EDID_T
{
    UINT32    u1EDID_ID;      /* device address */
    UINT8     u1SubAddr;      /* lenth of ui4_offset, from 0 up to 3 */
    UINT8     ui1_data;
} EDID_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _MISC_TYPE_SIF_ADVANCED_T
 *  {
 *      UINT8       ui1_channel;
 *      UINT16      ui2_clkdiv;
 *      UINT32      ui4_addr;
 *      UINT8       ui1_offset_len;
 *      UINT32      ui4_offset;
 *      UINT8       ui1_data_len;
 *      UINT8*      pui1_data;
 *  } MISC_TYPE_SIF_ADVANCED_T;
 *  @endcode
 *  @li@c  ui1_channel                                       -
 *  @li@c  ui2_clkdiv                                        -
 *  @li@c  ui4_addr                                          -
 *  @li@c  ui1_offset_len                                    -
 *  @li@c  ui4_offset                                        -
 *  @li@c  ui1_data_len                                      -
 *  @li@c  pui1_data                                         -
 */
/*------------------------------------------------------------------*/
typedef struct _MISC_TYPE_SIF_ADVANCED_T
{
    UINT8       ui1_channel;
    UINT16      ui2_clkdiv;
    UINT32      ui4_addr;              /* device address */
    UINT8       ui1_offset_len;        /* lenth of ui4_offset, from 0 up to 3 */
    UINT32      ui4_offset;            /* sub-address (or called offset) */
    UINT8       ui1_data_len;          /* Length if data */
    UINT8*      pui1_data;
} MISC_TYPE_SIF_ADVANCED_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct
 *  {
 *      PANEL_SELECTION_MODE_T    e_selection_mode;
 *      UINT32                    ui4_panel_id;
 *  } PANEL_SELECTION_CONFIG_T;
 *  @endcode
 *  @li@c  e_selection_mode                                  -
 *  @li@c  ui4_panel_id                                      -
 */
/*------------------------------------------------------------------*/
typedef struct
{
    PANEL_SELECTION_MODE_T    e_selection_mode;
    UINT32                    ui4_panel_id;
} PANEL_SELECTION_CONFIG_T;

/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *  	AUTOSEARCH_CHANGE_CHANNEL= 0,
 *  	AUTOSEARCH_SCAN ,
 *  	AUTOSEARCH_FINE_TUNE
 *  } AUTO_SEARCH_MODE_T;
 *  @endcode
 *  @li@c  AUTOSEARCH_CHANGE_CHANNEL                         - The autosearch mode is scan mode
 *  @li@c  AUTOSEARCH_SCAN                                   - The autosearch mode is change channel mode
 *  @li@c  AUTOSEARCH_FINE_TUNE                              - The autosearch mode is fine tune mode
 */
/*------------------------------------------------------------------*/
typedef enum
{
	AUTOSEARCH_CHANGE_CHANNEL= 0,
	AUTOSEARCH_SCAN ,
	AUTOSEARCH_FINE_TUNE
} AUTO_SEARCH_MODE_T;

/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      DRV_CUSTOM_MISC_CRNT_INP_SRC_TV = 0,
 *      DRV_CUSTOM_MISC_CRNT_INP_SRC_COMP_VIDEO,
 *      DRV_CUSTOM_MISC_CRNT_INP_SRC_S_VIDEO,
 *      DRV_CUSTOM_MISC_CRNT_INP_SRC_Y_PB_PR,
 *      DRV_CUSTOM_MISC_CRNT_INP_SRC_VGA,
 *      DRV_CUSTOM_MISC_CRNT_INP_SRC_SCART,
 *      DRV_CUSTOM_MISC_CRNT_INP_SRC_HDMI
 *  } DRV_CUSTOM_MISC_CRNT_INP_SRC_TYPE_T;
 *  @endcode
 *  @li@c  DRV_CUSTOM_MISC_CRNT_INP_SRC_TV                   -
 *  @li@c  DRV_CUSTOM_MISC_CRNT_INP_SRC_COMP_VIDEO           -
 *  @li@c  DRV_CUSTOM_MISC_CRNT_INP_SRC_S_VIDEO              -
 *  @li@c  DRV_CUSTOM_MISC_CRNT_INP_SRC_Y_PB_PR              -
 *  @li@c  DRV_CUSTOM_MISC_CRNT_INP_SRC_VGA                  -
 *  @li@c  DRV_CUSTOM_MISC_CRNT_INP_SRC_SCART                -
 *  @li@c  DRV_CUSTOM_MISC_CRNT_INP_SRC_HDMI                 -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DRV_CUSTOM_MISC_CRNT_INP_SRC_TV = 0,
    DRV_CUSTOM_MISC_CRNT_INP_SRC_COMP_VIDEO,
    DRV_CUSTOM_MISC_CRNT_INP_SRC_S_VIDEO,
    DRV_CUSTOM_MISC_CRNT_INP_SRC_Y_PB_PR,
    DRV_CUSTOM_MISC_CRNT_INP_SRC_VGA,
    DRV_CUSTOM_MISC_CRNT_INP_SRC_SCART,
    DRV_CUSTOM_MISC_CRNT_INP_SRC_HDMI
} DRV_CUSTOM_MISC_CRNT_INP_SRC_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _DRV_CUST_DRAM_CFG_T
 *  {
 *      UINT8       ui1_ssc_range;
 *      UINT8       ui1_ssc_modulation;
 *      UINT8       ui1_clk_driving;
 *      UINT8       ui1_clk_delay;
 *      UINT8       ui1_cmd_driving;
 *      UINT8       ui1_cmd_delay;
 *      UINT8       ui1_wdqs_driving;
 *      UINT8       ui1_wdqs0_delay;
 *      UINT8       ui1_wdqs1_delay;
 *      UINT8       ui1_wdqs2_delay;
 *      UINT8       ui1_wdqs3_delay;
 *      UINT8       ui1_wdq_driving;
 *      UINT8       ui1_wdq0_delay;
 *      UINT8       ui1_wdq1_delay;
 *      UINT8       ui1_wdq2_delay;
 *      UINT8       ui1_wdq3_delay;
 *  } DRV_CUST_DRAM_CFG_T;
 *  @endcode
 *  @li@c  ui1_ssc_range                                     -
 *  @li@c  ui1_ssc_modulation                                -
 *  @li@c  ui1_clk_driving                                   -
 *  @li@c  ui1_clk_delay                                     -
 *  @li@c  ui1_cmd_driving                                   -
 *  @li@c  ui1_cmd_delay                                     -
 *  @li@c  ui1_wdqs_driving                                  -
 *  @li@c  ui1_wdqs0_delay                                   -
 *  @li@c  ui1_wdqs1_delay                                   -
 *  @li@c  ui1_wdqs2_delay                                   -
 *  @li@c  ui1_wdqs3_delay                                   -
 *  @li@c  ui1_wdq_driving                                   -
 *  @li@c  ui1_wdq0_delay                                    -
 *  @li@c  ui1_wdq1_delay                                    -
 *  @li@c  ui1_wdq2_delay                                    -
 *  @li@c  ui1_wdq3_delay                                    -
 */
/*------------------------------------------------------------------*/
typedef struct _DRV_CUST_DRAM_CFG_T
{
    UINT8       ui1_ssc_range;
    UINT8       ui1_ssc_modulation;
    UINT8       ui1_clk_driving;
    UINT8       ui1_clk_delay;
    UINT8       ui1_cmd_driving;
    UINT8       ui1_cmd_delay;
    UINT8       ui1_wdqs_driving;
    UINT8       ui1_wdqs0_delay;
    UINT8       ui1_wdqs1_delay;
    UINT8       ui1_wdqs2_delay;
    UINT8       ui1_wdqs3_delay;
    UINT8       ui1_wdq_driving;
    UINT8       ui1_wdq0_delay;
    UINT8       ui1_wdq1_delay;
    UINT8       ui1_wdq2_delay;
    UINT8       ui1_wdq3_delay;
} DRV_CUST_DRAM_CFG_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct
 *  {
 *      DRV_CUSTOM_MISC_CRNT_INP_SRC_TYPE_T    e_type;
 *      UINT8                                  ui1_port_num;
 *  } DRV_CUSTOM_MISC_CRNT_INP_SRC_INFO_T;
 *  @endcode
 *  @li@c  e_type                                            -
 *  @li@c  ui1_port_num                                      -
 */
/*------------------------------------------------------------------*/
typedef struct
{
    DRV_CUSTOM_MISC_CRNT_INP_SRC_TYPE_T    e_type;
    UINT8                                  ui1_port_num;
} DRV_CUSTOM_MISC_CRNT_INP_SRC_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _MISC_TYPE_HDMI_HDCP_T
 *  {
 *      UINT8    aui1_hdcp_buf[320];
 *  } MISC_TYPE_HDMI_HDCP_T;
 *  @endcode
 *  @li@c  aui1_hdcp_buf[320]                                -
 */
/*------------------------------------------------------------------*/
typedef struct _MISC_TYPE_HDMI_HDCP_T
{
    UINT8    aui1_hdcp_buf[320];           /* Buffer to store HDMI HDCP data. */
} MISC_TYPE_HDMI_HDCP_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _MISC_TYPE_VGA_EDID_T
 *  {
 *      UINT8    aui1_edid_buf[256];
 *  } MISC_TYPE_VGA_EDID_T;
 *  @endcode
 *  @li@c  aui1_edid_buf[256]                                -
 */
/*------------------------------------------------------------------*/
typedef struct _MISC_TYPE_VGA_EDID_T
{
    UINT8    aui1_edid_buf[256];            /* Buffer to store VGA EDID data. */
} MISC_TYPE_VGA_EDID_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct
 *  {
 *      UINT8    s_version[32];
 *  } DRV_CUSTOM_MISC_LOADER_INFO_T;
 *  @endcode
 *  @li@c  s_version[32]                                     -
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT8    s_version[32];
} DRV_CUSTOM_MISC_LOADER_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _MISC_TYPE_LED_DISPLAY_T
 *  {
 *      DRV_CUSTOM_MISC_LED_TYPE_T                 e_led_type;
 *      union
 *      {
 *          DRV_CUSTOM_MISC_LED_GENERIC_STATE_T    e_generic_state;
 *          DRV_CUSTOM_MISC_LED_POWER_STATE_T      e_power_state;
 *          DRV_CUSTOM_MISC_LED_STANDBY_STATE_T    e_standby_state;
 *          DRV_CUSTOM_MISC_LED_TIMER_STATE_T      e_timer_state;
 *      } u_led_state;
 *  } MISC_TYPE_LED_DISPLAY_T;
 *  @endcode
 *  @li@c  e_led_type                                        -
 *  @li@c  e_generic_state                                   -
 *  @li@c  e_power_state                                     -
 *  @li@c  e_standby_state                                   -
 *  @li@c  e_timer_state                                     -
 */
/*------------------------------------------------------------------*/
typedef struct _MISC_TYPE_LED_DISPLAY_T
{
    DRV_CUSTOM_MISC_LED_TYPE_T                 e_led_type;
    union
    {
        DRV_CUSTOM_MISC_LED_GENERIC_STATE_T    e_generic_state;
        DRV_CUSTOM_MISC_LED_POWER_STATE_T      e_power_state;
        DRV_CUSTOM_MISC_LED_STANDBY_STATE_T    e_standby_state;
        DRV_CUSTOM_MISC_LED_TIMER_STATE_T      e_timer_state;
    } u_led_state;
} MISC_TYPE_LED_DISPLAY_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _MISC_TYPE_LED_NOTIFY_T
 *  {
 *      DRV_CUSTOM_MISC_LED_NOTIFY_T    e_led_notify_type;
 *  } MISC_TYPE_LED_NOTIFY_T;
 *  @endcode
 *  @li@c  e_led_notify_type             -
 */
/*------------------------------------------------------------------*/
typedef struct _MISC_TYPE_LED_NOTIFY_T
{
    DRV_CUSTOM_MISC_LED_NOTIFY_T    e_led_notify_type;
} MISC_TYPE_LED_NOTIFY_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _MISC_TYPE_NAND_FLASH_WRITE_T
 *  {
 *      UINT32    ui4_part_id;
 *      UINT32    ui4_offset;
 *      UINT32    ui4_size;
 *      UINT8     aui1_mem_buff[2 * 1024 * 1024];
 *  } MISC_TYPE_NAND_FLASH_WRITE_T;
 *  @endcode
 *  @li@c  ui4_part_id                                  -
 *  @li@c  ui4_offset                                   -
 *  @li@c  ui4_size                                     -
 *  @li@c  aui1_mem_buff                                -
 */
/*------------------------------------------------------------------*/
typedef struct _MISC_TYPE_NAND_FLASH_WRITE_T
{
    UINT32    ui4_part_id;
    UINT32    ui4_offset;
    UINT32    ui4_size;
    UINT8     aui1_mem_buff[2 * 1024 * 1024];
} MISC_TYPE_NAND_FLASH_WRITE_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _MISC_TYPE_NAND_FLASH_BULK_WRITE_T
 *  {
 *      UINT32    ui4_part_id;
 *      UINT32    ui4_offset;
 *      UINT32    ui4_size;
 *      UINT8     ui1_value;
 *  } MISC_TYPE_NAND_FLASH_BULK_WRITE_T;
 *  @endcode
 *  @li@c  ui4_part_id                                  -
 *  @li@c  ui4_offset                                   -
 *  @li@c  ui4_size                                     -
 *  @li@c  ui1_value                                    -
 */
/*------------------------------------------------------------------*/
typedef struct _MISC_TYPE_NAND_FLASH_BULK_WRITE_T
{
    UINT32    ui4_part_id;
    UINT32    ui4_offset;
    UINT32    ui4_size;
    UINT8     ui1_value;
} MISC_TYPE_NAND_FLASH_BULK_WRITE_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _MISC_TYPE_NOR_FLASH_BULK_WRITE_T
 *  {
 *      UINT32    ui4_part_id;
 *      UINT32    ui4_offset;
 *      UINT32    ui4_size;
 *      UINT8     ui1_value;
 *  } MISC_TYPE_NOR_FLASH_BULK_WRITE_T;
 *  @endcode
 *  @li@c  ui4_part_id                                  -
 *  @li@c  ui4_offset                                   -
 *  @li@c  ui4_size                                     -
 *  @li@c  ui1_value                                    -
 */
/*------------------------------------------------------------------*/
typedef struct _MISC_TYPE_NOR_FLASH_BULK_WRITE_T
{
    UINT32    ui4_part_id;
    UINT32    ui4_offset;
    UINT32    ui4_size;
    UINT8     ui1_value;
} MISC_TYPE_NOR_FLASH_BULK_WRITE_T;

/*-----------------------------------------------------------------------------
    <<Customer Specific>>  macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

#ifndef TARGET_SPEC_DRV_CUST_CUSTOMIZED_HDR_FILE

/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      DRV_CUSTOM_CUST_SPEC_TYPE_BEGIN = 0,
 *      DRV_CUSTOM_CUST_SPEC_TYPE_END
 *  } DRV_CUSTOM_CUST_SPEC_TYPE_T;
 *  @endcode
 *  @li@c  DRV_CUSTOM_CUST_SPEC_TYPE_BEGIN                   -
 *  @li@c  DRV_CUSTOM_CUST_SPEC_TYPE_END                     -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DRV_CUSTOM_CUST_SPEC_TYPE_BEGIN = 0,
    DRV_CUSTOM_CUST_SPEC_TYPE_END
} DRV_CUSTOM_CUST_SPEC_TYPE_T;
#endif

/*-----------------------------------------------------------------------------
    <<Top Level Inteface>>  macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      DRV_CUSTOM_OPERATION_TYPE_BEGIN = 0,
 *      DRV_CUSTOM_OPERATION_TYPE_AUD_SET,
 *      DRV_CUSTOM_OPERATION_TYPE_AUD_GET,
 *      DRV_CUSTOM_OPERATION_TYPE_AUD_GET_MIN_MAX,
 *      DRV_CUSTOM_OPERATION_TYPE_TUNER_SET,
 *      DRV_CUSTOM_OPERATION_TYPE_TUNER_GET,
 *      DRV_CUSTOM_OPERATION_TYPE_VID_SET,
 *      DRV_CUSTOM_OPERATION_TYPE_VID_GET,
 *      DRV_CUSTOM_OPERATION_TYPE_VID_GET_GAMMA,
 *      DRV_CUSTOM_OPERATION_TYPE_VID_GET_MIN_MAX,
 *      DRV_CUSTOM_OPERATION_TYPE_VID_DEC_SET,
 *      DRV_CUSTOM_OPERATION_TYPE_VID_DEC_GET,
 *      DRV_CUSTOM_OPERATION_TYPE_EXTMJC_SET,
 *      DRV_CUSTOM_OPERATION_TYPE_EXTMJC_GET,
 *      DRV_CUSTOM_OPERATION_TYPE_MISC_SET,
 *      DRV_CUSTOM_OPERATION_TYPE_MISC_GET,
 *      DRV_CUSTOM_OPERATION_TYPE_MISC_GET_MIN_MAX,
 *      DRV_CUSTOM_OPERATION_TYPE_CUST_SPEC_SET,
 *      DRV_CUSTOM_OPERATION_TYPE_CUST_SPEC_GET,
 *      DRV_CUSTOM_OPERATION_TYPE_END
 *  } DRV_CUSTOM_OPERATION_TYPE_T;
 *  @endcode
 *  @li@c  DRV_CUSTOM_OPERATION_TYPE_BEGIN                   -
 *  @li@c  DRV_CUSTOM_OPERATION_TYPE_AUD_SET                 -
 *  @li@c  DRV_CUSTOM_OPERATION_TYPE_AUD_GET                 -
 *  @li@c  DRV_CUSTOM_OPERATION_TYPE_AUD_GET_MIN_MAX         -
 *  @li@c  DRV_CUSTOM_OPERATION_TYPE_TUNER_SET               -
 *  @li@c  DRV_CUSTOM_OPERATION_TYPE_TUNER_GET               -
 *  @li@c  DRV_CUSTOM_OPERATION_TYPE_VID_SET                 -
 *  @li@c  DRV_CUSTOM_OPERATION_TYPE_VID_GET                 -
 *  @li@c  DRV_CUSTOM_OPERATION_TYPE_VID_GET_GAMMA           -
 *  @li@c  DRV_CUSTOM_OPERATION_TYPE_VID_GET_MIN_MAX         -
 *  @li@c  DRV_CUSTOM_OPERATION_TYPE_VID_DEC_SET             -
 *  @li@c  DRV_CUSTOM_OPERATION_TYPE_VID_DEC_GET             -
 *  @li@c  DRV_CUSTOM_OPERATION_TYPE_EXTMJC_SET              -
 *  @li@c  DRV_CUSTOM_OPERATION_TYPE_EXTMJC_GET              -
 *  @li@c  DRV_CUSTOM_OPERATION_TYPE_MISC_SET                -
 *  @li@c  DRV_CUSTOM_OPERATION_TYPE_MISC_GET                -
 *  @li@c  DRV_CUSTOM_OPERATION_TYPE_MISC_GET_MIN_MAX        -
 *  @li@c  DRV_CUSTOM_OPERATION_TYPE_CUST_SPEC_SET           -
 *  @li@c  DRV_CUSTOM_OPERATION_TYPE_CUST_SPEC_GET           -
 *  @li@c  DRV_CUSTOM_OPERATION_TYPE_END                     -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DRV_CUSTOM_OPERATION_TYPE_BEGIN = 0,

    /* Audio */
    DRV_CUSTOM_OPERATION_TYPE_AUD_SET,
    DRV_CUSTOM_OPERATION_TYPE_AUD_GET,
    DRV_CUSTOM_OPERATION_TYPE_AUD_GET_MIN_MAX,

    /* Tuner */
    DRV_CUSTOM_OPERATION_TYPE_TUNER_SET,
    DRV_CUSTOM_OPERATION_TYPE_TUNER_GET,

    /* Video */
    DRV_CUSTOM_OPERATION_TYPE_VID_SET,
    DRV_CUSTOM_OPERATION_TYPE_VID_GET,
    DRV_CUSTOM_OPERATION_TYPE_VID_GET_GAMMA,
    DRV_CUSTOM_OPERATION_TYPE_VID_GET_MIN_MAX,

    /* Video decoder */
    DRV_CUSTOM_OPERATION_TYPE_VID_DEC_SET,
    DRV_CUSTOM_OPERATION_TYPE_VID_DEC_GET,

    /* EXTMJC */
    DRV_CUSTOM_OPERATION_TYPE_EXTMJC_SET,
    DRV_CUSTOM_OPERATION_TYPE_EXTMJC_GET,

    /* Misc */
    DRV_CUSTOM_OPERATION_TYPE_MISC_SET,
    DRV_CUSTOM_OPERATION_TYPE_MISC_GET,
    DRV_CUSTOM_OPERATION_TYPE_MISC_GET_MIN_MAX,

    /* Customer's spesific API */
    DRV_CUSTOM_OPERATION_TYPE_CUST_SPEC_SET,
    DRV_CUSTOM_OPERATION_TYPE_CUST_SPEC_GET,

    DRV_CUSTOM_OPERATION_TYPE_END
} DRV_CUSTOM_OPERATION_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _AUD_TYPE_SET_INFO_T
 *  {
 *      DRV_CUSTOM_AUD_TYPE_T              e_aud_type;
 *      VOID*                              pv_set_info;
 *      SIZE_T                             z_size;
 *      BOOL                               b_store;
 *  } AUD_TYPE_SET_INFO_T;
 *  @endcode
 *  @li@c  e_aud_type                                        -
 *  @li@c  pv_set_info                                       -
 *  @li@c  z_size                                            -
 *  @li@c  b_store                                           -
 */
/*------------------------------------------------------------------*/
typedef struct _AUD_TYPE_SET_INFO_T
{
    DRV_CUSTOM_AUD_TYPE_T              e_aud_type;
    VOID*                              pv_set_info;
    SIZE_T                             z_size;
    BOOL                               b_store;
} AUD_TYPE_SET_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _AUD_TYPE_GET_INFO_T
 *  {
 *      DRV_CUSTOM_AUD_TYPE_T              e_aud_type;
 *      VOID*                              pv_get_info;
 *      SIZE_T*                            pz_size;
 *  } AUD_TYPE_GET_INFO_T;
 *  @endcode
 *  @li@c  e_aud_type                                        -
 *  @li@c  pv_get_info                                       -
 *  @li@c  pz_size                                           -
 */
/*------------------------------------------------------------------*/
typedef struct _AUD_TYPE_GET_INFO_T
{
    DRV_CUSTOM_AUD_TYPE_T              e_aud_type;
    VOID*                              pv_get_info;
    SIZE_T*                            pz_size;
} AUD_TYPE_GET_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _AUD_TYPE_GET_MIN_MAX_INFO_T
 *  {
 *      DRV_CUSTOM_AUD_TYPE_T              e_aud_type;
 *      INT16*                             pi2_min;
 *      INT16*                             pi2_max;
 *  } AUD_TYPE_GET_MIN_MAX_INFO_T;
 *  @endcode
 *  @li@c  e_aud_type                                        -
 *  @li@c  pi2_min                                           -
 *  @li@c  pi2_max                                           -
 */
/*------------------------------------------------------------------*/
typedef struct _AUD_TYPE_GET_MIN_MAX_INFO_T
{
    DRV_CUSTOM_AUD_TYPE_T              e_aud_type;
    INT16*                             pi2_min;
    INT16*                             pi2_max;
} AUD_TYPE_GET_MIN_MAX_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _TUNER_TYPE_SET_INFO_T
 *  {
 *      DRV_CUSTOM_TUNER_TYPE_T            e_tuner_type;
 *      DRV_CUSTOM_TUNER_SET_TYPE_T        e_tuner_set_type;
 *      VOID*                              pv_set_info;
 *      SIZE_T                             z_size;
 *  } TUNER_TYPE_SET_INFO_T;
 *  @endcode
 *  @li@c  e_tuner_type                                      -
 *  @li@c  e_tuner_set_type                                  -
 *  @li@c  pv_set_info                                       -
 *  @li@c  z_size                                            -
 */
/*------------------------------------------------------------------*/
typedef struct _TUNER_TYPE_SET_INFO_T
{
    DRV_CUSTOM_TUNER_TYPE_T            e_tuner_type;
    DRV_CUSTOM_TUNER_SET_TYPE_T        e_tuner_set_type;
    VOID*                              pv_set_info;
    SIZE_T                             z_size;
} TUNER_TYPE_SET_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _TUNER_TYPE_GET_INFO_T
 *  {
 *      DRV_CUSTOM_TUNER_TYPE_T            e_tuner_type;
 *      DRV_CUSTOM_TUNER_GET_TYPE_T        e_tuner_get_type;
 *      VOID*                              pv_get_info;
 *      SIZE_T*                            pz_size;
 *  } TUNER_TYPE_GET_INFO_T;
 *  @endcode
 *  @li@c  e_tuner_type                                      -
 *  @li@c  e_tuner_get_type                                  -
 *  @li@c  pv_get_info                                       -
 *  @li@c  pz_size                                           -
 */
/*------------------------------------------------------------------*/
typedef struct _TUNER_TYPE_GET_INFO_T
{
    DRV_CUSTOM_TUNER_TYPE_T            e_tuner_type;
    DRV_CUSTOM_TUNER_GET_TYPE_T        e_tuner_get_type;
    VOID*                              pv_get_info;
    SIZE_T*                            pz_size;
} TUNER_TYPE_GET_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _VID_TYPE_SET_INFO_T
 *  {
 *      DRV_CUSTOM_VID_TYPE_T              e_vid_type;
 *      VOID*                              pv_set_info;
 *      SIZE_T                             z_size;
 *      BOOL                               b_store;
 *  } VID_TYPE_SET_INFO_T;
 *  @endcode
 *  @li@c  e_vid_type                                        -
 *  @li@c  pv_set_info                                       -
 *  @li@c  z_size                                            -
 *  @li@c  b_store                                           -
 */
/*------------------------------------------------------------------*/
typedef struct _VID_TYPE_SET_INFO_T
{
    DRV_CUSTOM_VID_TYPE_T              e_vid_type;
    VOID*                              pv_set_info;
    SIZE_T                             z_size;
    BOOL                               b_store;
} VID_TYPE_SET_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _VID_TYPE_GET_INFO_T
 *  {
 *      DRV_CUSTOM_VID_TYPE_T              e_vid_type;
 *      DRV_CUSTOM_VID_INP_T               e_vid_inp;
 *      VOID*                              pv_get_info;
 *      SIZE_T*                            pz_size;
 *  } VID_TYPE_GET_INFO_T;
 *  @endcode
 *  @li@c  e_vid_type                                        -
 *  @li@c  e_vid_inp                                         -
 *  @li@c  pv_get_info                                       -
 *  @li@c  pz_size                                           -
 */
/*------------------------------------------------------------------*/
typedef struct _VID_TYPE_GET_INFO_T
{
    DRV_CUSTOM_VID_TYPE_T              e_vid_type;
    DRV_CUSTOM_VID_INP_T               e_vid_inp;
    VOID*                              pv_get_info;
    SIZE_T*                            pz_size;
} VID_TYPE_GET_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _VID_TYPE_GET_GAMMA_INFO_T
 *  {
 *      GAMMA_INFO_T*                      pu1GammaTbl;
 *  } VID_TYPE_GET_GAMMA_INFO_T;
 *  @endcode
 *  @li@c  pu1GammaTbl                                       -
 */
/*------------------------------------------------------------------*/
typedef struct _VID_TYPE_GET_GAMMA_INFO_T
{
    GAMMA_INFO_T*                      pu1GammaTbl;
} VID_TYPE_GET_GAMMA_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _VID_TYPE_GET_MIN_MAX_INFO_T
 *  {
 *      DRV_CUSTOM_VID_TYPE_T              e_vid_type;
 *      DRV_CUSTOM_VID_INP_T               e_vid_inp;
 *      UINT16*                            pui2_min;
 *      UINT16*                            pui2_max;
 *  } VID_TYPE_GET_MIN_MAX_INFO_T;
 *  @endcode
 *  @li@c  e_vid_type                                        -
 *  @li@c  e_vid_inp                                         -
 *  @li@c  pui2_min                                          -
 *  @li@c  pui2_max                                          -
 */
/*------------------------------------------------------------------*/
typedef struct _VID_TYPE_GET_MIN_MAX_INFO_T
{
    DRV_CUSTOM_VID_TYPE_T              e_vid_type;
    DRV_CUSTOM_VID_INP_T               e_vid_inp;
    UINT16*                            pui2_min;
    UINT16*                            pui2_max;
} VID_TYPE_GET_MIN_MAX_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _VID_DEC_TYPE_SET_INFO_T
 *  {
 *      DRV_CUSTOM_VID_DEC_TYPE_T          e_vid_dec_type;
 *      VOID*                              pv_set_info;
 *      SIZE_T                             z_size;
 *      BOOL                               b_store;
 *  } VID_DEC_TYPE_SET_INFO_T;
 *  @endcode
 *  @li@c  e_vid_dec_type                                    -
 *  @li@c  pv_set_info                                       -
 *  @li@c  z_size                                            -
 *  @li@c  b_store                                           -
 */
/*------------------------------------------------------------------*/
typedef struct _VID_DEC_TYPE_SET_INFO_T
{
    DRV_CUSTOM_VID_DEC_TYPE_T          e_vid_dec_type;
    VOID*                              pv_set_info;
    SIZE_T                             z_size;
    BOOL                               b_store;
} VID_DEC_TYPE_SET_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _VID_DEC_TYPE_GET_INFO_T
 *  {
 *      DRV_CUSTOM_VID_DEC_TYPE_T          e_vid_dec_type;
 *      VOID*                              pv_get_info;
 *      SIZE_T*                            pz_size;
 *  } VID_DEC_TYPE_GET_INFO_T;
 *  @endcode
 *  @li@c  e_vid_dec_type                                    -
 *  @li@c  pv_get_info                                       -
 *  @li@c  pz_size                                           -
 */
/*------------------------------------------------------------------*/
typedef struct _VID_DEC_TYPE_GET_INFO_T
{
    DRV_CUSTOM_VID_DEC_TYPE_T          e_vid_dec_type;
    VOID*                              pv_get_info;
    SIZE_T*                            pz_size;
} VID_DEC_TYPE_GET_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _EXTMJC_TYPE_SET_INFO_T
 *  {
 *      DRV_CUSTOM_EXTMJC_TYPE_T           e_extmjc_type;
 *      VOID*                              pv_set_info;
 *      SIZE_T                             z_size;
 *      BOOL                               b_store;
 *  } EXTMJC_TYPE_SET_INFO_T;
 *  @endcode
 *  @li@c  e_extmjc_type                                     -
 *  @li@c  pv_set_info                                       -
 *  @li@c  z_size                                            -
 *  @li@c  b_store                                           -
 */
/*------------------------------------------------------------------*/
typedef struct _EXTMJC_TYPE_SET_INFO_T
{
    DRV_CUSTOM_EXTMJC_TYPE_T           e_extmjc_type;
    VOID*                              pv_set_info;
    SIZE_T                             z_size;
    BOOL                               b_store;
} EXTMJC_TYPE_SET_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _EXTMJC_TYPE_GET_INFO_T
 *  {
 *      DRV_CUSTOM_EXTMJC_TYPE_T           e_extmjc_type;
 *      VOID*                              pv_get_info;
 *      SIZE_T*                            pz_size;
 *  } EXTMJC_TYPE_GET_INFO_T;
 *  @endcode
 *  @li@c  e_extmjc_type                                    -
 *  @li@c  pv_get_info                                       -
 *  @li@c  pz_size                                           -
 */
/*------------------------------------------------------------------*/
typedef struct _EXTMJC_TYPE_GET_INFO_T
{
    DRV_CUSTOM_EXTMJC_TYPE_T           e_extmjc_type;
    VOID*                              pv_get_info;
    SIZE_T*                            pz_size;
} EXTMJC_TYPE_GET_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _MISC_TYPE_SET_INFO_T
 *  {
 *      DRV_CUSTOM_MISC_TYPE_T                  e_misc_type;
 *      VOID*                                   pv_set_info;
 *      SIZE_T                                  z_size;
 *      BOOL                                    b_store;
 *  } MISC_TYPE_SET_INFO_T;
 *  @endcode
 *  @li@c  e_misc_type                                       -
 *  @li@c  pv_set_info                                       -
 *  @li@c  z_size                                            -
 *  @li@c  b_store                                           -
 */
/*------------------------------------------------------------------*/
typedef struct _MISC_TYPE_SET_INFO_T
{
    DRV_CUSTOM_MISC_TYPE_T                  e_misc_type;
    VOID*                                   pv_set_info;
    SIZE_T                                  z_size;
    BOOL                                    b_store;
} MISC_TYPE_SET_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _MISC_TYPE_GET_INFO_T
 *  {
 *      DRV_CUSTOM_MISC_TYPE_T                  e_misc_type;
 *      VOID*                                   pv_get_info;
 *      SIZE_T*                                 pz_size;
 *  } MISC_TYPE_GET_INFO_T;
 *  @endcode
 *  @li@c  e_misc_type                                       -
 *  @li@c  pv_get_info                                       -
 *  @li@c  pz_size                                           -
 */
/*------------------------------------------------------------------*/
typedef struct _MISC_TYPE_GET_INFO_T
{
    DRV_CUSTOM_MISC_TYPE_T                  e_misc_type;
    VOID*                                   pv_get_info;
    SIZE_T*                                 pz_size;
} MISC_TYPE_GET_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _MISC_TYPE_GET_MIN_MAX_INFO_T
 *  {
 *      DRV_CUSTOM_MISC_TYPE_T                  e_misc_type;
 *      UINT16*                                 pui2_min;
 *      UINT16*                                 pui2_max;
 *  } MISC_TYPE_GET_MIN_MAX_INFO_T;
 *  @endcode
 *
 *  @li@c  e_misc_type                                       -
 *  @li@c  pui2_min                                          -
 *  @li@c  pui2_max                                          -
 */
/*------------------------------------------------------------------*/

typedef struct _MISC_TYPE_GET_MIN_MAX_INFO_T
{
    DRV_CUSTOM_MISC_TYPE_T                  e_misc_type;
    UINT16*                                 pui2_min;
    UINT16*                                 pui2_max;
} MISC_TYPE_GET_MIN_MAX_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _CUST_SPEC_TYPE_SET_INFO_T
 *  {
 *      DRV_CUSTOM_CUST_SPEC_TYPE_T             e_cust_spec_type;
 *      VOID*                                   pv_set_info;
 *      SIZE_T                                  z_size;
 *      BOOL                                    b_store;
 *  } CUST_SPEC_TYPE_SET_INFO_T;
 *  @endcode
 *  @li@c  e_cust_spec_type                                  -
 *  @li@c  pv_set_info                                       -
 *  @li@c  z_size                                            -
 *  @li@c  b_store                                           -
 */
/*------------------------------------------------------------------*/
typedef struct _CUST_SPEC_TYPE_SET_INFO_T
{
    DRV_CUSTOM_CUST_SPEC_TYPE_T             e_cust_spec_type;
    VOID*                                   pv_set_info;
    SIZE_T                                  z_size;
    BOOL                                    b_store;
} CUST_SPEC_TYPE_SET_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _CUST_SPEC_TYPE_GET_INFO_T
 *  {
 *      DRV_CUSTOM_CUST_SPEC_TYPE_T             e_cust_spec_type;
 *      VOID*                                   pv_get_info;
 *      SIZE_T*                                 pz_size;
 *  } CUST_SPEC_TYPE_GET_INFO_T;
 *  @endcode
 *  @li@c  e_cust_spec_type                                  -
 *  @li@c  pv_get_info                                       -
 *  @li@c  pz_size                                           -
 */
/*------------------------------------------------------------------*/
typedef struct _CUST_SPEC_TYPE_GET_INFO_T
{
    DRV_CUSTOM_CUST_SPEC_TYPE_T             e_cust_spec_type;
    VOID*                                   pv_get_info;
    SIZE_T*                                 pz_size;
} CUST_SPEC_TYPE_GET_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _DRV_CUST_OPERATION_INFO_T
 *  {
 *      DRV_CUSTOM_OPERATION_TYPE_T                      e_op_type;
 *      union
 *      {
 *          AUD_TYPE_SET_INFO_T                          t_aud_set_info;
 *          AUD_TYPE_GET_INFO_T                          t_aud_get_info;
 *          AUD_TYPE_GET_MIN_MAX_INFO_T                  t_aud_get_min_max_info;
 *          TUNER_TYPE_SET_INFO_T                        t_tuner_set_info;
 *          TUNER_TYPE_GET_INFO_T                        t_tuner_get_info;
 *          VID_TYPE_SET_INFO_T                          t_vid_set_info;
 *          VID_TYPE_GET_INFO_T                          t_vid_get_info;
 *          VID_TYPE_GET_GAMMA_INFO_T                    t_vid_get_gamma_info;
 *          VID_TYPE_GET_MIN_MAX_INFO_T                  t_vid_get_min_max_info;
 *          VID_DEC_TYPE_SET_INFO_T                      t_vid_dec_set_info;
 *          VID_DEC_TYPE_GET_INFO_T                      t_vid_dec_get_info;
 *          EXTMJC_TYPE_SET_INFO_T                       t_extmjc_set_info;
 *          EXTMJC_TYPE_GET_INFO_T                       t_extmjc_get_info;
 *          MISC_TYPE_SET_INFO_T                         t_misc_set_info;
 *          MISC_TYPE_GET_INFO_T                         t_misc_get_info;
 *          MISC_TYPE_GET_MIN_MAX_INFO_T                 t_misc_get_min_max_info;
 *          CUST_SPEC_TYPE_SET_INFO_T                    t_cust_spec_set_info;
 *          CUST_SPEC_TYPE_GET_INFO_T                    t_cust_spec_get_info;
 *      } u;
 *  } DRV_CUST_OPERATION_INFO_T;
 *  @endcode
 *  @li@c  e_op_type                                         -
 *  @li@c  t_aud_set_info                                    -
 *  @li@c  t_aud_get_info                                    -
 *  @li@c  t_aud_get_min_max_info                            -
 *  @li@c  t_tuner_set_info                                  -
 *  @li@c  t_tuner_get_info                                  -
 *  @li@c  t_vid_set_info                                    -
 *  @li@c  t_vid_get_info                                    -
 *  @li@c  t_vid_get_gamma_info                              -
 *  @li@c  t_vid_get_min_max_info                            -
 *  @li@c  t_vid_dec_set_info                                -
 *  @li@c  t_vid_dec_get_info                                -
 *  @li@c  t_extmjc_set_info                                 -
 *  @li@c  t_extmjc_get_info                                 -
 *  @li@c  t_misc_set_info                                   -
 *  @li@c  t_misc_get_info                                   -
 *  @li@c  t_misc_get_min_max_info                           -
 *  @li@c  t_cust_spec_set_info                              -
 *  @li@c  t_cust_spec_get_info                              -
 */
/*------------------------------------------------------------------*/
typedef struct _DRV_CUST_OPERATION_INFO_T
{
    DRV_CUSTOM_OPERATION_TYPE_T                      e_op_type;

    union
    {
        /* Audio */
        AUD_TYPE_SET_INFO_T                          t_aud_set_info;
        AUD_TYPE_GET_INFO_T                          t_aud_get_info;
        AUD_TYPE_GET_MIN_MAX_INFO_T                  t_aud_get_min_max_info;

        /* Tuner */
        TUNER_TYPE_SET_INFO_T                        t_tuner_set_info;
        TUNER_TYPE_GET_INFO_T                        t_tuner_get_info;

        /* Video */
        VID_TYPE_SET_INFO_T                          t_vid_set_info;
        VID_TYPE_GET_INFO_T                          t_vid_get_info;
        VID_TYPE_GET_GAMMA_INFO_T                    t_vid_get_gamma_info;
        VID_TYPE_GET_MIN_MAX_INFO_T                  t_vid_get_min_max_info;

        /* Video decoder */
        VID_DEC_TYPE_SET_INFO_T                      t_vid_dec_set_info;
        VID_DEC_TYPE_GET_INFO_T                      t_vid_dec_get_info;

        /* EXTMJC */
        EXTMJC_TYPE_SET_INFO_T                       t_extmjc_set_info;
        EXTMJC_TYPE_GET_INFO_T                       t_extmjc_get_info;

        /* Misc */
        MISC_TYPE_SET_INFO_T                         t_misc_set_info;
        MISC_TYPE_GET_INFO_T                         t_misc_get_info;
        MISC_TYPE_GET_MIN_MAX_INFO_T                 t_misc_get_min_max_info;

        /* Customer specific */
        CUST_SPEC_TYPE_SET_INFO_T                    t_cust_spec_set_info;
        CUST_SPEC_TYPE_GET_INFO_T                    t_cust_spec_get_info;
    } u;
} DRV_CUST_OPERATION_INFO_T;

/*-----------------------------------------------------------------------------
                    function declarations
 -----------------------------------------------------------------------------*/

/* Audio */
/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
extern INT32 d_custom_aud_set (
    DRV_CUSTOM_AUD_TYPE_T    e_aud_type,
    VOID*                    pv_set_info,
    SIZE_T                   z_size,
    BOOL                     b_store
);

/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
extern INT32 d_custom_aud_get (
    DRV_CUSTOM_AUD_TYPE_T          e_aud_type,
    VOID*                          pv_get_info,
    SIZE_T*                        pz_size
);

/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
extern INT32 d_custom_aud_get_min_max (
    DRV_CUSTOM_AUD_TYPE_T          e_aud_type,
    INT16*                         pui2_min,
    INT16*                         pui2_max
);

/* Tuner */
/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
extern INT32 d_custom_tuner_set (
    DRV_CUSTOM_TUNER_TYPE_T        e_tuner_type,
    DRV_CUSTOM_TUNER_SET_TYPE_T    e_tuner_set_type,
    VOID*                          pv_set_info,
    SIZE_T                         z_size
);

/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
extern INT32 d_custom_tuner_get (
    DRV_CUSTOM_TUNER_TYPE_T        e_tuner_type,
    DRV_CUSTOM_TUNER_GET_TYPE_T    e_tuner_get_type,
    VOID*                          pv_get_info,
    SIZE_T*                        pz_size
);


/* Video */
/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
extern INT32 d_custom_vid_set (
    DRV_CUSTOM_VID_TYPE_T    e_vid_type,
    VOID*                    pv_set_info,
    SIZE_T                   z_size,
    BOOL                     b_store
);

/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
extern INT32 d_custom_vid_get (
    DRV_CUSTOM_VID_TYPE_T    e_vid_type,
    DRV_CUSTOM_VID_INP_T     e_vid_inp,
    VOID*                    pv_get_info,
    SIZE_T*                  pz_size
);

/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
extern INT32 d_custom_vid_get_gamma(
    GAMMA_INFO_T*            pu1GammaTbl
);

/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
extern INT32 d_custom_vid_get_min_max (
    DRV_CUSTOM_VID_TYPE_T    e_vid_type,
    DRV_CUSTOM_VID_INP_T     e_vid_inp,
    UINT16*                  pui2_min,
    UINT16*                  pui2_max
);

/* Video dec*/
/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
extern INT32 d_custom_vid_dec_set (
    DRV_CUSTOM_VID_DEC_TYPE_T   e_vdec_type,
    VOID*                       pv_set_info,
    SIZE_T                      z_size,
    BOOL                        b_store
);

/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
extern INT32 d_custom_vid_dec_get (
    DRV_CUSTOM_VID_DEC_TYPE_T   e_vdec_type,
    VOID*                       pv_get_info,
    SIZE_T*                     pz_size
);

/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
extern INT32 d_custom_extmjc_set (
    DRV_CUSTOM_EXTMJC_TYPE_T    e_extmjc_type,
    VOID*                       pv_set_info,
    SIZE_T                      z_size,
    BOOL                        b_store
);

/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
extern INT32 d_custom_extmjc_get (
    DRV_CUSTOM_EXTMJC_TYPE_T    e_extmjc_type,
    VOID*                       pv_get_info,
    SIZE_T*                     pz_size
);

/* Misc */
/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
extern INT32 d_custom_misc_set (
    DRV_CUSTOM_MISC_TYPE_T        e_misc_type,
    VOID*                         pv_set_info,
    SIZE_T                        z_size,
    BOOL                          b_store
);

/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
extern INT32 d_custom_misc_get (
    DRV_CUSTOM_MISC_TYPE_T        e_misc_type,
    VOID*                         pv_get_info,
    SIZE_T*                       pz_size
);

/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
extern INT32 d_custom_misc_get_min_max (
    DRV_CUSTOM_MISC_TYPE_T        e_misc_type,
    UINT16*                       pui2_min,
    UINT16*                       pui2_max
);

/* Customer specific APIs */
/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
extern INT32 d_custom_cust_spec_set (
    DRV_CUSTOM_CUST_SPEC_TYPE_T    e_cust_spec_type,
    VOID*                          pv_set_info,
    SIZE_T                         z_size,
    BOOL                           b_store
);

/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
extern INT32 d_custom_cust_spec_get (
    DRV_CUSTOM_CUST_SPEC_TYPE_T    e_cust_spec_type,
    VOID*                          pv_get_info,
    SIZE_T*                        pz_size
);

/*----------------------------------------------------------------------------*/
/*! @} */                                          /* end of groupMW_RES_MNGR */
/*----------------------------------------------------------------------------*/

#endif /*_U_DRV_CUST_H_ */

