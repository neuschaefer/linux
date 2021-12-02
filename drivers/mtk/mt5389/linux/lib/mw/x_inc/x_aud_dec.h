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
 * $RCSfile: x_aud_dec.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains audio decoder driver specific definitions,
 *         which are exported.
 *---------------------------------------------------------------------------*/

#ifndef _X_AUD_DEC_H_
#define _X_AUD_DEC_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "x_common.h"
#include "x_rm.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/


/* Get operations */
#define AUD_DEC_GET_TYPE_CTRL                 (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 0))
#define AUD_DEC_GET_TYPE_INP_MODE             (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 1))
#define AUD_DEC_GET_TYPE_FMT                  (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 2))
#define AUD_DEC_GET_TYPE_VOLUME               (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 3))
#define AUD_DEC_GET_TYPE_MUTE                 (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 4))
#define AUD_DEC_GET_TYPE_SPDIF                (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 5))
#define AUD_DEC_GET_TYPE_HDCD_FLTR            (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 6))
#define AUD_DEC_GET_TYPE_LS_MODE              (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 7))
#define AUD_DEC_GET_TYPE_SOUND_EFFECT         (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 8))
#define AUD_DEC_GET_TYPE_DOWNMIX              (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 9))
#define AUD_DEC_GET_TYPE_DRC                  (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 10))
#define AUD_DEC_GET_TYPE_CAPABILITY           (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 11))
#define AUD_DEC_GET_TYPE_OUT_PORT             (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 12))
#define AUD_DEC_GET_TYPE_PCR_ID               (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 13))
#define AUD_DEC_GET_TYPE_AUD_INFO             (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 14))
#define AUD_DEC_GET_TYPE_CMPSS_MODE           (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 15))
#define AUD_DEC_GET_TYPE_AGC                  (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 16))
#define AUD_DEC_GET_TYPE_CHNL_DELAY           (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 17))
#define AUD_DEC_GET_TYPE_MAX_CHNL_DELAY       (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 18))
#define AUD_DEC_GET_TYPE_COPY_PROTECT         (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 19))
#define AUD_DEC_GET_TYPE_MTS                  (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 20))
#define AUD_DEC_GET_TYPE_SE_EQ_USER_NUM_BAND  (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 21))
#define AUD_DEC_GET_TYPE_SE_EQ_USER_LVL       (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 22))
#define AUD_DEC_GET_TYPE_SE_EQ_USER_INFO_BAND (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 23))
#define AUD_DEC_GET_TYPE_TV_AUD_SYS_MASK      (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 24))
#define AUD_DEC_GET_TYPE_TV_SYS_CAPABILITY    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 25))
#define AUD_DEC_GET_TYPE_AUD_SYS_CAPABILITY   (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 26))
#define AUD_DEC_GET_TYPE_AUD_CLIP             (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 27))
#define AUD_DEC_GET_TYPE_AUD_CLIP_INFO        (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 28))
#define AUD_DEC_GET_TYPE_PL2_CTRL             (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 29))
#define AUD_DEC_GET_TYPE_DOLBY_CERTI_MODE     (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 30))
#define AUD_DEC_GET_TYPE_OUTPORT_ADJ          (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 31))
#define AUD_DEC_GET_TYPE_BBE_MODE             (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 32))
#define AUD_DEC_GET_TYPE_AV_SYNC              (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 33))
#define AUD_DEC_GET_TYPE_HEADPHONE_DETECT_NFY (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 34))
#define AUD_DEC_GET_TYPE_HEADPHONE_COND       (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 35))
#define AUD_DEC_GET_TYPE_MUTE_EX              (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 36))
#define AUD_DEC_GET_TYPE_DECODE_STATUS        (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 37))
#define AUD_DEC_GET_TYPE_VOLUME_EX            (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 38))
//CC_AUD_DSP_SUPPORT_AUDYSSEY
#define AUD_DEC_GET_TYPE_AUDYSSEY_EFFECT      (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 39))
//CC_AUD_DSP_SUPPORT_AUDYSSEY
#define AUD_DEC_GET_TYPE_MM_MODE              (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 40))
#define AUD_DEC_GET_TYPE_CLIP_CUSTOM          (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 41))
#define AUD_DEC_GET_TYPE_BTN_SOUND_CTRL       (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 42))

#define AUD_DEC_GET_TYPE_CMPSS_DRC            (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 43))
#define AUD_DEC_GET_TYPE_MPEG_LEVEL_ADJ       (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 44))
#define AUD_DEC_GET_TYPE_POWER_ON_OFF_MUSIC_VOLUME               (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 45))
#define AUD_DEC_GET_TYPE_AUDIO_PTS              (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 46))
#define AUD_DEC_GET_TYPE_UPLOAD_INFO            (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 47))
#define AUD_DEC_GET_TYPE_BUFFER_INFO            (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 48))
#define AUD_DEC_GET_TYPE_UPLOAD_PCM_INFO        (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 49))
#define AUD_DEC_GET_TYPE_AFIFO_AMOUNT           (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 50))

/* should be remove */
#define AUD_DEC_GET_TYPE_TV_SYS_MASK          (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 66))


/* Set operations */
#define AUD_DEC_SET_TYPE_CTRL                  ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 0)) | RM_SET_TYPE_ARG_NO_REF)
#define AUD_DEC_SET_TYPE_NFY_FCT                (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 1))
#define AUD_DEC_SET_TYPE_INP_MODE              ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 2)) | RM_SET_TYPE_ARG_NO_REF)
#define AUD_DEC_SET_TYPE_FMT                    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 3))
#define AUD_DEC_SET_TYPE_VOLUME                 (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 4))
#define AUD_DEC_SET_TYPE_MUTE                  ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 5)) | RM_SET_TYPE_ARG_NO_REF)
#define AUD_DEC_SET_TYPE_SPDIF                 ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 6)) | RM_SET_TYPE_ARG_NO_REF)
#define AUD_DEC_SET_TYPE_HDCD_FLTR             ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 7)) | RM_SET_TYPE_ARG_NO_REF)
#define AUD_DEC_SET_TYPE_LS_MODE                (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 8))
#define AUD_DEC_SET_TYPE_SOUND_EFFECT           (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 9))
#define AUD_DEC_SET_TYPE_DOWNMIX               ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 10)) | RM_SET_TYPE_ARG_NO_REF)
#define AUD_DEC_SET_TYPE_DRC                   ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 11)) | RM_SET_TYPE_ARG_NO_REF)
#define AUD_DEC_SET_TYPE_MEM_BUFFER             (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 12))
#define AUD_DEC_SET_TYPE_NFY_AUD_CHG            (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 13))
#define AUD_DEC_SET_TYPE_OUT_PORT              ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 14)) | RM_SET_TYPE_ARG_NO_REF)
#define AUD_DEC_SET_TYPE_PCR_ID                ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 15)) | RM_SET_TYPE_ARG_NO_REF)
#define AUD_DEC_SET_TYPE_CMPSS_MODE            ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 16)) | RM_SET_TYPE_ARG_NO_REF)
#define AUD_DEC_SET_TYPE_AGC                   ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 17)) | RM_SET_TYPE_ARG_NO_REF)
#define AUD_DEC_SET_TYPE_CHNL_DELAY             (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 18))
#define AUD_DEC_SET_TYPE_COPY_PROTECT          ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 19)) | RM_SET_TYPE_ARG_NO_REF)
#define AUD_DEC_SET_TYPE_MTS                   ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 20)) | RM_SET_TYPE_ARG_NO_REF)
#define AUD_DEC_SET_TYPE_SE_EQ_USER_LVL         (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 21))
#define AUD_DEC_SET_TYPE_TV_AUD_SYS_MASK        (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 22))
#define AUD_DEC_SET_TYPE_AUD_CLIP               (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 23))
#define AUD_DEC_SET_TYPE_AUD_CLIP_INFO          (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 24))
#define AUD_DEC_SET_TYPE_PL2_CTRL               (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 25))
#define AUD_DEC_SET_TYPE_DOLBY_CERTI_MODE      ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 26)) | RM_SET_TYPE_ARG_NO_REF)
#define AUD_DEC_SET_TYPE_OUTPORT_ADJ            (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 27))
#define AUD_DEC_SET_TYPE_BBE_MODE              ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 28)) | RM_SET_TYPE_ARG_NO_REF)
#define AUD_DEC_SET_TYPE_AV_SYNC               ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 29)) | RM_SET_TYPE_ARG_NO_REF)
#define AUD_DEC_SET_TYPE_HEADPHONE_DETECT_NFY   (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 30))
#define AUD_DEC_SET_TYPE_MUTE_EX                (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 31))
#define AUD_DEC_SET_TYPE_AUD_CLIP_GAIN         ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 32)) | RM_SET_TYPE_ARG_NO_REF)
#define AUD_DEC_SET_TYPE_PIPVIDEO_PATH_INFO    ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 33)) | RM_SET_TYPE_ARG_NO_REF)
#define AUD_DEC_SET_TYPE_VOLUME_EX              (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 34))
#define AUD_DEC_SET_TYPE_DSP_SPEED             ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 35)) |RM_SET_TYPE_ARG_NO_REF)
#define AUD_DEC_SET_TYPE_CODEC_SPEC_INFO        (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 36))
#define AUD_DEC_SET_TYPE_CODEC                 ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 37)) | RM_SET_TYPE_ARG_NO_REF)
#define AUD_DEC_SET_TYPE_LINOUT_VOL_MODE        (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 38))
#define AUD_DEC_SET_TYPE_PEQ_IND                (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 39))
#define AUD_DEC_SET_TYPE_OVM                    ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 40)) | RM_SET_TYPE_ARG_NO_REF)
//CC_AUD_DSP_SUPPORT_AUDYSSEY
#define AUD_DEC_SET_TYPE_AUDYSSEY_EFFECT        ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 41)) | RM_SET_TYPE_ARG_NO_REF)
//CC_AUD_DSP_SUPPORT_AUDYSSEY
#define AUD_DEC_SET_TYPE_BTN_SOUND_CTRL         (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 42))
#define AUD_DEC_SET_TYPE_MM_MODE                ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 43)) | RM_SET_TYPE_ARG_NO_REF)
#define AUD_DEC_SET_TYPE_CLIP_CUSTOM            (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 44))

#define AUD_DEC_SET_TYPE_CMPSS_DRC              ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 45)) | RM_SET_TYPE_ARG_NO_REF)
#define AUD_DEC_SET_TYPE_AUTO_DETECT_AUD_SYS    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 46))
#define AUD_DEC_SET_TYPE_MPEG_LEVEL_ADJ         ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 47)) | RM_SET_TYPE_ARG_NO_REF)
#define AUD_DEC_SET_TYPE_UPLOAD_MODE            (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 48))
#define AUD_DEC_SET_TYPE_PANFADE_ENABLE         (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 49))
#define AUD_DEC_SET_TYPE_PROTECT_INFO           ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 50)) | RM_SET_TYPE_ARG_NO_REF)
#define AUD_DEC_SET_TYPE_VIDEO_SCRAMBLE_STATUS  ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 51)) | RM_SET_TYPE_ARG_NO_REF)
#define AUD_DEC_SET_TYPE_AUDIO_ONLY_INFO        ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 52)) | RM_SET_TYPE_ARG_NO_REF)
// DDC control
#define AUD_DEC_SET_TYPE_DDC_MODE               (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 53))
#define AUD_DEC_SET_TYPE_KARAOKE                (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 54))
#define AUD_DEC_SET_TYPE_VDO_PICTURE_OFF        ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 55)) | RM_SET_TYPE_ARG_NO_REF)
#define AUD_DEC_SET_TYPE_OUT_PORT_LINK          ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 56)) | RM_SET_TYPE_ARG_NO_REF)
#define AUD_DEC_SET_TYPE_DC_POWER_OFF           ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 57)) | RM_SET_TYPE_ARG_NO_REF)
#define AUD_DEC_SET_TYPE_AD_MODE                ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 58)) | RM_SET_TYPE_ARG_NO_REF)
// NEW SPDIF
#define AUD_DEC_SET_TYPE_SPDIF_STATUS        ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 59)) | RM_SET_TYPE_ARG_NO_REF)
///for audio record
#define AUD_DEC_SET_TYPE_UPLOAD_CTRL          ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 60)) | RM_SET_TYPE_ARG_NO_REF)
#define AUD_DEC_SET_TYPE_UPLOAD_PERIOD          ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 61)) | RM_SET_TYPE_ARG_NO_REF)
#define AUD_DEC_SET_TYPE_UPLOAD_DATA_NFY   (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 62))
#define AUD_DEC_SET_TYPE_SRS_BYPASS              ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 63)) | RM_SET_TYPE_ARG_NO_REF)

/*For  endian mode in DTS (little or big)*/
#define AUD_DEC_SET_TYPE_ENDIAN_MODE          ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 64)) | RM_SET_TYPE_ARG_NO_REF)

/*For  Audio out device*/
#define AUD_DEC_SET_TYPE_OUT_DEVICE          ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 65)) | RM_SET_TYPE_ARG_NO_REF)

/* should be remove */
#define AUD_DEC_SET_TYPE_TV_SYS_MASK          (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 66))

#define AUD_DEC_SET_TYPE_STC_ID          ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 67)) | RM_SET_TYPE_ARG_NO_REF)

typedef enum
{
    AUD_DEC_DECODE_UNKNOWN = -1,
    AUD_DEC_DECODE_NO_DATA,
    AUD_DEC_DECODE_NORMAL,
    AUD_DEC_DECODE_ERROR,
    AUD_DEC_DECODE_NOT_SUPPORT,
    AUD_DEC_DECODE_FMT_CONFLICT
}   AUD_DEC_DECODE_STATUS_T;

typedef enum
{
    AUD_DEC_BBE_MODE_OFF = 0,
    AUD_DEC_BBE_MODE_BBE,
    AUD_DEC_BBE_MODE_BBE_VIVA
}   AUD_DEC_BBE_MODE_T;

//CC_AUD_DSP_SUPPORT_AUDYSSEY
typedef enum
{
    AUD_DEC_ADV_MODE_OFF = 0,
    AUD_DEC_ADV_MODE_LIGHT,
    AUD_DEC_ADV_MODE_MEDIUM,
    AUD_DEC_ADV_MODE_HEAVY
}   AUD_DEC_ADV_MODE_T;

typedef enum
{
    AUD_DEC_ABX_MODE_OFF = 0,
    AUD_DEC_ABX_MODE_ON,
}   AUD_DEC_ABX_MODE_T;

typedef enum
{
    AUD_DEC_ENDIAN_MODE_LITTLE = 0,
    AUD_DEC_ENDIAN_MODE_BIG
} AUD_DEC_ENDIAN_MODE_T;

typedef enum
{
    AUD_DEC_OUT_DEVICE_BUILD_SPEAKER = 0,
    AUD_DEC_OUT_DEVICE_USB_HEADPHONE,
    AUD_DEC_OUT_DEVICE_BLUETOOTH_EARPHONE
} AUD_DEC_OUT_DEVICE_T;

typedef enum
{
    AUD_DEC_AEQ_MODE_OFF = 0,
    AUD_DEC_AEQ_MODE_ON,
}   AUD_DEC_AEQ_MODE_T;
//CC_AUD_DSP_SUPPORT_AUDYSSEY

/* Notify conditions */
typedef enum
{
    AUD_DEC_COND_ERROR = -1,
    AUD_DEC_COND_CTRL_DONE,
    AUD_DEC_COND_AUD_CLIP_DONE,
    AUD_DEC_COND_MEM_BUFFER_DONE,
    AUD_DEC_COND_FLUSH_DONE,
    AUD_DEC_COND_FEED_ME_ON,
    AUD_DEC_COND_FEED_ME_OFF,
    AUD_DEC_COND_AUD_INFO_CHG,
    AUD_DEC_COND_TV_AUD_SYS_DETECTED,
    AUD_DEC_COND_AUD_DECODE_STATUS_CHG,
    AUD_DEC_COND_MM_EOS,
    AUD_DEC_COND_FM_RADIO_DET_DONE,
    AUD_DEC_COND_PROTECT_INFO,
    AUD_DEC_COND_UPLOAD_INFO_CHG
}   AUD_DEC_COND_T;


/* Control types. */
typedef enum
{
    AUD_DEC_CTRL_RESET = 0,
    AUD_DEC_CTRL_STOP,
    AUD_DEC_CTRL_PLAY,
    AUD_DEC_CTRL_PLAY_SYNC,
    AUD_DEC_CTRL_PLAY_AUD_CLIP,
    AUD_DEC_CTRL_FLUSH,
    AUD_DEC_CTRL_PAUSE,
    AUD_DEC_CTRL_RESUME,
    AUD_DEC_CTRL_DETECT_TV_AUD_SYS,
    AUD_DEC_CTRL_DETECT_TV_AUD_SYS_N_PLAY,
    AUD_DEC_CTRL_PLAY_AUD_MIXSND_CLIP
}   AUD_DEC_CTRL_T;

/*for HDMI protect information */
typedef enum
{
    AUD_DEC_PROTECT_INFO_UNKNOWN = -1,
    AUD_DEC_PROTECT_INFO_DMUTE_AMUTE,         /* Digtal mute and analog mute */
    AUD_DEC_PROTECT_INFO_DUNMUTE_AMUTE,       /* Digtal unmute and analog mute */
    AUD_DEC_PROTECT_INFO_DUNMUTE_AUNMUTE,     /* Digtal unmute and analog unmute */
    AUD_DEC_PROTECT_INFO_DMUTE_AUNMUTE        /* Digtal mute and analog unmute */
}   AUD_DEC_PROTECT_INFO_T;

/* Audio types. */
typedef enum
{
    AUD_DEC_TYPE_UNKNOWN = 0,
    AUD_DEC_TYPE_MONO,                 /* 1/0 */
    AUD_DEC_TYPE_MONO_SUB,             /* 1+sub-language */
    AUD_DEC_TYPE_DUAL_MONO,            /* 1+1 */
    AUD_DEC_TYPE_STEREO,               /* 2/0 */
    AUD_DEC_TYPE_STEREO_SUB,           /* 2+sub-language */
    AUD_DEC_TYPE_STEREO_DOLBY_SURROUND,/* 2/0, dolby surround */
    AUD_DEC_TYPE_SURROUND_2CH,         /* 2/1 */
    AUD_DEC_TYPE_SURROUND,             /* 3/1 */
    AUD_DEC_TYPE_3_0,                  /* 3/0 */
    AUD_DEC_TYPE_4_0,                  /* 2/2 */
    AUD_DEC_TYPE_5_1,                  /* 3/2.L */
    AUD_DEC_TYPE_7_1,                  /* 5/2.L */

    AUD_DEC_TYPE_FM_MONO_NICAM_MONO,
    AUD_DEC_TYPE_FM_MONO_NICAM_STEREO,
    AUD_DEC_TYPE_FM_MONO_NICAM_DUAL,
    AUD_DEC_TYPE_OTHERS,

    AUD_DEC_TYPE_5_0,                   /* 3/2 */
    AUD_DEC_TYPE_JOINT_STEREO,

    AUD_DEC_TYPE_FMRDO_MONO,
    AUD_DEC_TYPE_FMRDO_STEREO,

    AUD_DEC_TYPE_MONO_LFE,                 /* 1/0.L */
    AUD_DEC_TYPE_DUAL_MONO_LFE,            /* 1+1.L */
    AUD_DEC_TYPE_STEREO_LFE,               /* 2/0.L */
    AUD_DEC_TYPE_SURROUND_2CH_LFE,         /* 2/1.L */
    AUD_DEC_TYPE_SURROUND_LFE,             /* 3/1.L */
    AUD_DEC_TYPE_3_1,                  /* 3/0.L */
    AUD_DEC_TYPE_4_1                  /* 2/2.L */
}   AUD_DEC_AUD_TYPE_T;

/* audio decoder types. */
typedef enum
{
    AUD_DEC_DECODE_TYPE_UNKNOWN = 0,
    AUD_DEC_DECODE_TYPE_AC3,
    AUD_DEC_DECODE_TYPE_EAC3
} AUD_DEC_DECODE_TYPE_T;

/* Ouput port types. */
typedef enum
{
    AUD_DEC_OUT_PORT_OFF = 0,
    AUD_DEC_OUT_PORT_2_CH,
    AUD_DEC_OUT_PORT_5_1_CH,
    AUD_DEC_OUT_PORT_SPDIF,
    AUD_DEC_OUT_PORT_2_CH_BY_PASS,
    AUD_DEC_OUT_PORT_SPEAKER,
    AUD_DEC_OUT_PORT_HEADPHONE,
    AUD_DEC_OUT_PORT_TVSCART
}   AUD_DEC_OUT_PORT_T;

#define AUD_DEC_OUT_PORT_FLAG_NONE          ((UINT32) (0) )
#define AUD_DEC_OUT_PORT_FLAG_2_CH          MAKE_BIT_MASK_32(0)
#define AUD_DEC_OUT_PORT_FLAG_5_1_CH        MAKE_BIT_MASK_32(1)
#define AUD_DEC_OUT_PORT_FLAG_SPDIF         MAKE_BIT_MASK_32(2)
#define AUD_DEC_OUT_PORT_FLAG_2_CH_BY_PASS  MAKE_BIT_MASK_32(3)
#define AUD_DEC_OUT_PORT_FLAG_SPEAKER       MAKE_BIT_MASK_32(4)
#define AUD_DEC_OUT_PORT_FLAG_HEADPHONE     MAKE_BIT_MASK_32(5)
#define AUD_DEC_OUT_PORT_FLAG_TVSCART       MAKE_BIT_MASK_32(6)


/* SPDIF output types. */
typedef enum
{
    AUD_DEC_SPDIF_OFF = 0,
    AUD_DEC_SPDIF_RAW,
    AUD_DEC_SPDIF_PCM_16,
    AUD_DEC_SPDIF_PCM_24
}   AUD_DEC_SPDIF_TYPE_T;


/* mute types. */
typedef enum
{
    AUD_DEC_MUTE_OFF = 0,
    AUD_DEC_MUTE_L_CH,
    AUD_DEC_MUTE_R_CH,
    AUD_DEC_MUTE_ON
}   AUD_DEC_MUTE_TYPE_T;


/* HDCD Filter types. */
typedef enum
{
    AUD_DEC_HDCD_FLTR_OFF = 0,
    AUD_DEC_HDCD_FLTR_1X,
    AUD_DEC_HDCD_FLTR_2X
}   AUD_DEC_HDCD_FLTR_T;


/* equalizer types. */
typedef enum
{
    AUD_DEC_EQ_OFF = 0,
    AUD_DEC_EQ_ROCK,
    AUD_DEC_EQ_POP,
    AUD_DEC_EQ_LIVE,
    AUD_DEC_EQ_DANCE,
    AUD_DEC_EQ_TECHNO,
    AUD_DEC_EQ_CLASSIC,
    AUD_DEC_EQ_SOFT,
    AUD_DEC_EQ_USER_1,
    AUD_DEC_EQ_USER_2,
    AUD_DEC_EQ_USER_3,
    AUD_DEC_EQ_USER_4,
    AUD_DEC_EQ_USER_5,
    AUD_DEC_EQ_USER_6,
    AUD_DEC_EQ_USER_7,
    AUD_DEC_EQ_USER_8,
    AUD_DEC_EQ_BBE_TV,
    AUD_DEC_EQ_BBE_AV,
    AUD_DEC_EQ_SBASS
}   AUD_DEC_EQ_TYPE_T;

/* reverb modes. */
typedef enum
{
    AUD_DEC_REVERB_OFF = 0,
    AUD_DEC_REVERB_CONCERT,
    AUD_DEC_REVERB_LIVINGROOM,
    AUD_DEC_REVERB_HALL,
    AUD_DEC_REVERB_BATHROOM,
    AUD_DEC_REVERB_CAVE,
    AUD_DEC_REVERB_ARENA,
    AUD_DEC_REVERB_CHURCH
}   AUD_DEC_REVERB_MODE_T;


/* Audio formats. */
typedef enum
{
    AUD_DEC_FMT_UNKNOWN = 0,
    AUD_DEC_FMT_MPEG,
    AUD_DEC_FMT_AC3,
    AUD_DEC_FMT_PCM,
    AUD_DEC_FMT_MP3,
    AUD_DEC_FMT_AAC,
    AUD_DEC_FMT_DTS,
    AUD_DEC_FMT_WMA,
    AUD_DEC_FMT_RA,
    AUD_DEC_FMT_HDCD,
    AUD_DEC_FMT_MLP,
    AUD_DEC_FMT_MTS,   /* remove */
    AUD_DEC_FMT_EU_CANAL_PLUS,
    AUD_DEC_FMT_TV_SYS,
    AUD_DEC_FMT_FMRDO = 21,
    AUD_DEC_FMT_FMRDO_DET,
    AUD_DEC_FMT_SBCDEC = 23,
    AUD_DEC_FMT_SBCENC = 24,
    AUD_DEC_FMT_DRA = 28,
    AUD_DEC_FMT_COOK = 29,  // megaa: should match AUD_FMT_COOK which is 25 in aud_if.h
    AUD_DEC_FMT_G729DEC = 30,
    AUD_DEC_FMT_VORBIS = 31,
    AUD_DEC_FMT_WMAPRO = 32,
    AUD_DEC_FMT_HE_AAC,
    AUD_DEC_FMT_HE_AAC_V2,
    AUD_DEC_FMT_AMR = 37,        //37 amr-nb
    AUD_DEC_FMT_AWB = 38,        //38 amr-wb, match aud_if.h
}   AUD_DEC_FMT_T;

/* output port adjust struct. */
typedef struct
{
    AUD_DEC_OUT_PORT_T  e_out_port;
    BOOL                b_is_fixed;
}   AUD_DEC_OUTPORT_ADJ_T;

typedef struct
{
    AUD_DEC_FMT_T        e_aud_fmt;
    AUD_DEC_AUD_TYPE_T   e_aud_type;
    UINT32               ui4_sample_rate;
    UINT32               ui4_data_rate;
    UINT8                ui1_bit_depth;
    AUD_DEC_DECODE_TYPE_T e_dec_type;
} AUD_DEC_AUD_INFO_T;

typedef enum
{
    AUD_DEC_LINEOUT_VOL_MODE_FIXED = 0,
    AUD_DEC_LINEOUT_VOL_MODE_VARIABLE
}   AUD_DEC_LINEOUT_VOL_MODE_T;

typedef struct AUD_DEC_LINEOUT_VOL_T
{
    AUD_DEC_LINEOUT_VOL_MODE_T           e_mode;
    UINT8                                                ui1_level;
    BOOL                                                 b_mute;
}   AUD_DEC_LINEOUT_VOL_T;

/* Loudspeader modes. */
typedef enum
{
    AUD_DEC_LS_MODE_OFF = 0,
    AUD_DEC_LS_MODE_LARGE_FREQ_RANGE,
    AUD_DEC_LS_MODE_SMALL_FREQ_RANGE
}   AUD_DEC_LS_MODE_T;


/* Loudspeader types. */
typedef enum
{
    AUD_DEC_LS_FRONT_LEFT = 0,
    AUD_DEC_LS_FRONT_RIGHT,
    AUD_DEC_LS_REAR_LEFT,
    AUD_DEC_LS_REAR_RIGHT,
    AUD_DEC_LS_CENTER,
    AUD_DEC_LS_SUB_WOOFER
}   AUD_DEC_LS_T;

#define AUD_DEV_LS_FLAG_NONE             ((UINT32) (0) )
#define AUD_DEC_LS_FLAG_FRONT_LEFT       MAKE_BIT_MASK_32(0)
#define AUD_DEC_LS_FLAG_FRONT_RIGHT      MAKE_BIT_MASK_32(1)
#define AUD_DEC_LS_FLAG_REAR_LEFT        MAKE_BIT_MASK_32(2)
#define AUD_DEC_LS_FLAG_REAR_RIGHT       MAKE_BIT_MASK_32(3)
#define AUD_DEC_LS_FLAG_CENTER           MAKE_BIT_MASK_32(4)
#define AUD_DEC_LS_FLAG_SUB_WOOFER       MAKE_BIT_MASK_32(5)


/* Sound Effects. */
typedef enum
{
    AUD_DEC_SE_TRIM = 0,
    AUD_DEC_SE_DELAY,
    AUD_DEC_SE_SUPERBASS,
    AUD_DEC_SE_EQUALIZER,
    AUD_DEC_SE_REVERB,
    AUD_DEC_SE_BASS,
    AUD_DEC_SE_TREBLE,
    AUD_DEC_SE_BALANCE,
    AUD_DEC_SE_POSTDRC,
    AUD_DEC_SE_VOLUME,
    AUD_DEC_SE_SURROUND,
    AUD_DEC_SE_PROLOG_II,
    AUD_DEC_SE_SOUND_MODE,
    AUD_DEC_SE_SPH_MODE,
    AUD_DEC_SE_VOLUME_OFFSET,
    AUD_DEC_SE_HP_VOLUME_OFFSET,
    AUD_DEC_SE_SURROUND_EX,
    AUD_DEC_SE_SUPERWOOFER,
    AUD_DEC_SE_SOUND_MODE_START
}   AUD_DEC_SOUND_EFFECT_T;

//CC_AUD_DSP_SUPPORT_AUDYSSEY
/* Audyssey Effects. */
typedef enum
{
    AUD_DEC_AUDYSSEY_ADV = 0,
    AUD_DEC_AUDYSSEY_ABX,
    AUD_DEC_AUDYSSEY_AEQ
}   AUD_DEC_AUDYSSEY_EFFECT_T;
//CC_AUD_DSP_SUPPORT_AUDYSSEY

typedef struct _AUD_SE_SURROUND_EX_INFO_T
{
    UINT8           ui1Clarity;
    UINT8           ui1Width;
    UINT8           ui1LRGain;
    UINT8           ui1Xtalk;
    UINT8           ui1OutputGain;
    UINT8           ui1BassGain;
    UINT8           ui1Fo1;
    UINT8           ui1Fo2;
    UINT8           ui1Fo3;
    UINT8           ui1Mode;
}   AUD_SE_SURROUND_EX_INFO_T;

/* Downmix modes. */
typedef enum
{
    AUD_DEC_DM_OFF = 0,
    AUD_DEC_DM_LT_RT,
    AUD_DEC_DM_STEREO,
    AUD_DEC_DM_VIR_SURR,
    AUD_DEC_DM_MONO,
    AUD_DEC_DM_DUAL1,
    AUD_DEC_DM_DUAL2,
    AUD_DEC_DM_DUAL_MIX,
    AUD_DEC_DM_3_STEREO,
    AUD_DEC_DM_DUAL_OFF
}   AUD_DEC_DOWNMIX_MODE_T;


/* Dolby Dynamice Range Control. */
typedef enum
{
    AUD_DEC_DRC_OFF = 0,
    AUD_DEC_DRC_10,
    AUD_DEC_DRC_20,
    AUD_DEC_DRC_30,
    AUD_DEC_DRC_40,
    AUD_DEC_DRC_50,
    AUD_DEC_DRC_60,
    AUD_DEC_DRC_70,
    AUD_DEC_DRC_80,
    AUD_DEC_DRC_90,
    AUD_DEC_DRC_100
}   AUD_DEC_DRC_T;


/* Sample bits. */
typedef enum
{
    AUD_DEC_SAMPLE_BITS_16 = 0,
    AUD_DEC_SAMPLE_BITS_18,
    AUD_DEC_SAMPLE_BITS_20,
    AUD_DEC_SAMPLE_BITS_24
}   AUD_DEC_SAMPLE_BITS_T;


/* Sample frequency. */
typedef enum
{
    AUD_DEC_SAMPLE_FREQ_8 = 0,
    AUD_DEC_SAMPLE_FREQ_11_025,
    AUD_DEC_SAMPLE_FREQ_12,
    AUD_DEC_SAMPLE_FREQ_16,
    AUD_DEC_SAMPLE_FREQ_22_050,
    AUD_DEC_SAMPLE_FREQ_24,
    AUD_DEC_SAMPLE_FREQ_32,
    AUD_DEC_SAMPLE_FREQ_44_100,
    AUD_DEC_SAMPLE_FREQ_48,
    AUD_DEC_SAMPLE_FREQ_96
}   AUD_DEC_SAMPLE_FREQ_T;


/*The tuned volume types */
typedef enum
{
    AUD_DEC_ALL_CH = 0,
    AUD_DEC_INDIVIDUAL_CH,
    AUD_DEC_TEST_TONE
}   AUD_DEC_VOL_TYPE_T;


/* Dolby Digital compression mode. */
typedef enum
{
    AUD_DEC_CMPSS_MODE_LINE = 0,
    AUD_DEC_CMPSS_MODE_CUSTOM0,
    AUD_DEC_CMPSS_MODE_CUSTOM1,
    AUD_DEC_CMPSS_MODE_RF
}   AUD_DEC_CMPSS_MODE_T;

/* Dolby Range compression mode. */
typedef enum
{
    AUD_DEC_CMPSS_DRC_MODE_COMPRESSED = 0,
    AUD_DEC_CMPSS_DRC_MODE_STANDARD,
    AUD_DEC_CMPSS_DRC_MODE_OFF
}   AUD_DEC_CMPSS_DRC_MODE_T;

/* Dsp Speed mode. */
typedef enum
{
    AUD_DEC_DSP_SPEED_0 = 0,
    AUD_DEC_DSP_SPEED_2,
    AUD_DEC_DSP_SPEED_4,
    AUD_DEC_DSP_SPEED_8,
    AUD_DEC_DSP_SPEED_16,
    AUD_DEC_DSP_SPEED_32
}   AUD_DEC_DSP_SPEED_MODE_T;



/* Capability channels. */
#define AUD_DEC_CAP_TYPE_UNKNOWN     (((UINT32) 1) << AUD_DEC_TYPE_UNKNOWN)
#define AUD_DEC_CAP_TYPE_MONO        (((UINT32) 1) << AUD_DEC_TYPE_MONO)
#define AUD_DEC_CAP_TYPE_DUAL_MONO   (((UINT32) 1) << AUD_DEC_TYPE_DUAL_MONO)
#define AUD_DEC_CAP_TYPE_STEREO      (((UINT32) 1) << AUD_DEC_TYPE_STEREO)
#define AUD_DEC_CAP_TYPE_STEREO_DOLBY_SURROUND      (((UINT32) 1) << AUD_DEC_TYPE_STEREO_DOLBY_SURROUND)
#define AUD_DEC_CAP_TYPE_3_0         (((UINT32) 1) << AUD_DEC_TYPE_3_0)
#define AUD_DEC_CAP_TYPE_4_0         (((UINT32) 1) << AUD_DEC_TYPE_4_0)
#define AUD_DEC_CAP_TYPE_5_1         (((UINT32) 1) << AUD_DEC_TYPE_5_1)
#define AUD_DEC_CAP_TYPE_7_1         (((UINT32) 1) << AUD_DEC_TYPE_7_1)
#define AUD_DEC_CAP_TYPE_OTHERS      (((UINT32) 1) << AUD_DEC_TYPE_OTHERS)


/* Capability downmix mode. */
#define AUD_DEC_CAP_DM_OFF           (((UINT32) 1) << AUD_DEC_DM_OFF)
#define AUD_DEC_CAP_DM_LT_RT         (((UINT32) 1) << AUD_DEC_DM_LT_RT)
#define AUD_DEC_CAP_DM_STEREO        (((UINT32) 1) << AUD_DEC_DM_STEREO)
#define AUD_DEC_CAP_DM_VIR_SURR      (((UINT32) 1) << AUD_DEC_DM_VIR_SURR)


/* Capability sound effect. */
#define AUD_DEC_CAP_SE_TRIM          (((UINT32) 1) << AUD_DEC_SE_TRIM)
#define AUD_DEC_CAP_SE_DELAY         (((UINT32) 1) << AUD_DEC_SE_DELAY)
#define AUD_DEC_CAP_SE_SUPERBASS     (((UINT32) 1) << AUD_DEC_SE_SUPERBASS)
#define AUD_DEC_CAP_SE_EQUALIZER     (((UINT32) 1) << AUD_DEC_SE_EQUALIZER)
#define AUD_DEC_CAP_SE_REVERB        (((UINT32) 1) << AUD_DEC_SE_REVERB)
#define AUD_DEC_CAP_SE_BASS          (((UINT32) 1) << AUD_DEC_SE_BASS)
#define AUD_DEC_CAP_SE_TREBLE        (((UINT32) 1) << AUD_DEC_SE_TREBLE)
#define AUD_DEC_CAP_SE_BALANCE       (((UINT32) 1) << AUD_DEC_SE_BALANCE)
#define AUD_DEC_CAP_SE_POSTDRC       (((UINT32) 1) << AUD_DEC_SE_POSTDRC)
#define AUD_DEC_CAP_SE_VOLUME        (((UINT32) 1) << AUD_DEC_SE_VOLUME)
#define AUD_DEC_CAP_SE_SURROUND      (((UINT32) 1) << AUD_DEC_SE_SURROUND)


/* Capability format. */
#define AUD_DEC_CAP_FMT_MPEG           (((UINT32) 1) << AUD_DEC_FMT_MPEG)
#define AUD_DEC_CAP_FMT_AC3            (((UINT32) 1) << AUD_DEC_FMT_AC3)
#define AUD_DEC_CAP_FMT_PCM            (((UINT32) 1) << AUD_DEC_FMT_PCM)
#define AUD_DEC_CAP_FMT_MP3            (((UINT32) 1) << AUD_DEC_FMT_MP3)
#define AUD_DEC_CAP_FMT_AAC            (((UINT32) 1) << AUD_DEC_FMT_AAC)
#define AUD_DEC_CAP_FMT_DTS            (((UINT32) 1) << AUD_DEC_FMT_DTS)
#define AUD_DEC_CAP_FMT_WMA            (((UINT32) 1) << AUD_DEC_FMT_WMA)
#define AUD_DEC_CAP_FMT_RA             (((UINT32) 1) << AUD_DEC_FMT_RA)
#define AUD_DEC_CAP_FMT_HDCD           (((UINT32) 1) << AUD_DEC_FMT_HDCD)
#define AUD_DEC_CAP_FMT_MLP            (((UINT32) 1) << AUD_DEC_FMT_MLP)
#define AUD_DEC_CAP_FMT_MTS            (((UINT32) 1) << AUD_DEC_FMT_MTS)
#define AUD_DEC_CAP_FMT_EU_CANAL_PLUS  (((UINT32) 1) << AUD_DEC_FMT_EU_CANAL_PLUS)
#define AUD_DEC_CAP_FMT_TV_SYS         (((UINT32) 1) << AUD_DEC_FMT_TV_SYS)
#define AUD_DEC_CAP_FMT_FMRDO          (((UINT32) 1) << AUD_DEC_FMT_FMRDO)
#define AUD_DEC_CAP_FMT_FMRDO_DET      (((UINT32) 1) << AUD_DEC_FMT_FMRDO_DET)
#define AUD_DEC_CAP_FMT_SBCDEC         (((UINT32) 1) << AUD_DEC_FMT_SBCDEC)
#define AUD_DEC_CAP_FMT_SBCENC         (((UINT32) 1) << AUD_DEC_FMT_SBCENC)
#define AUD_DEC_CAP_FMT_COOK           (((UINT32) 1) << AUD_DEC_FMT_COOK)  // megaa
#define AUD_DEC_CAP_FMT_DRA            (((UINT32) 1) << AUD_DEC_FMT_DRA)
#define AUD_DEC_CAP_FMT_VORBIS         (((UINT32) 1) << AUD_DEC_FMT_VORBIS)

//over 32bits, to put in ui4_fmt_ext
#define AUD_DEC_CAP_FMT_WMAPRO         (((UINT32) 1) << (AUD_DEC_FMT_WMAPRO-AUD_DEC_FMT_WMAPRO))
#define AUD_DEC_CAP_FMT_AMRNB          (((UINT32) 1) << (AUD_DEC_FMT_AMR-AUD_DEC_FMT_WMAPRO))
#define AUD_DEC_CAP_FMT_AMRWB          (((UINT32) 1) << (AUD_DEC_FMT_AWB-AUD_DEC_FMT_WMAPRO))

/* Notify function */
typedef VOID (*x_aud_dec_nfy_fct) (VOID*           pv_nfy_tag,
                                   AUD_DEC_COND_T  e_nfy_cond,
                                   UINT32          ui4_data_1,
                                   UINT32          ui4_data_2 );


/* capability info. */
typedef struct _AUD_DEC_CAPABILITY_INFO_T
{
    UINT32              ui4_fmt;
    UINT32              ui4_fmt_ext;
    UINT32              ui4_channel_type;
    UINT32              ui4_sound_effect;
    UINT32              ui4_downmix_mode;
    BOOL                b_is_outport_adj;
}   AUD_DEC_CAPABILITY_INFO_T;

/* Notify setting info. */
typedef struct _AUD_DEC_NFY_INFO_T
{
    VOID*               pv_tag;
    x_aud_dec_nfy_fct   pf_aud_dec_nfy;
}   AUD_DEC_NFY_INFO_T;

/* MP3 setting info. */
typedef struct _AUD_DEC_FMT_MP3_T
{
    UINT8                  ui1_num_ch;
    AUD_DEC_SAMPLE_BITS_T  e_sample_bits;
    AUD_DEC_SAMPLE_FREQ_T  e_samle_freq;
}   AUD_DEC_FMT_MP3_T;

/* PCM setting info. */
typedef struct _AUD_DEC_FMT_PCM_T
{
    UINT8                  ui1_num_ch;
    AUD_DEC_SAMPLE_BITS_T  e_sample_bits;
    AUD_DEC_SAMPLE_FREQ_T  e_samle_freq;

    BOOL  b_signed_data;
    BOOL  b_big_endian;

}   AUD_DEC_FMT_PCM_T;

/* Decode format setting info. */
typedef struct _AUD_DEC_FMT_INFO_T
{
    AUD_DEC_FMT_T   e_fmt;

    VOID*           pv_info;
}   AUD_DEC_FMT_INFO_T;

/* Sound Effect info. */
typedef struct _AUD_DEC_SE_INFO_T
{
    AUD_DEC_SOUND_EFFECT_T      e_se_type;

    VOID*                          pv_info;
}   AUD_DEC_SE_INFO_T;

/* Individual channel volume level setting extension. */
typedef struct _AUD_DEC_CH_VOLUME_EX_T
{
    AUD_DEC_LS_T        e_ls;
    UINT8               ui1_level;
}   AUD_DEC_CH_VOLUME_EX_T;

/* Volume level setting info extension. */
typedef struct _AUD_DEC_VOLUME_INFO_EX_T
{
    AUD_DEC_VOL_TYPE_T            e_vol_type;
    AUD_DEC_OUT_PORT_T            e_out_port;

    union
    {
        UINT8                     ui1_level;
        AUD_DEC_CH_VOLUME_EX_T    t_ch_vol;
    } u;
}   AUD_DEC_VOLUME_INFO_EX_T;

/* Individual channel volume level setting. */
typedef struct _AUD_DEC_CH_VOLUME_T
{
    AUD_DEC_OUT_PORT_T  e_out_port;
    AUD_DEC_LS_T        e_ls;

    UINT8               ui1_level;
}   AUD_DEC_CH_VOLUME_T;

/* Volume level setting info. */
typedef struct _AUD_DEC_VOLUME_INFO_T
{
    AUD_DEC_VOL_TYPE_T            e_vol_type;

    union
    {
        UINT8                   ui1_level;
        AUD_DEC_CH_VOLUME_T     t_ch_vol;
    } u;
}   AUD_DEC_VOLUME_INFO_T;


/* Individual channel trim level setting. */
typedef struct _AUD_DEC_CH_TRIM_T
{
    AUD_DEC_LS_T    e_ls;

    UINT8           ui1_level;
}   AUD_DEC_CH_TRIM_T;


/* Individual channel delay level setting. */
typedef struct _AUD_DEC_CH_DELAY_T
{
    AUD_DEC_LS_T    e_ls;

    UINT16          ui2_time;
}   AUD_DEC_CH_DELAY_T;


/* Individual channel loudspeaker mode setting. */
typedef struct _AUD_DEC_CH_LS_MODE_T
{
    AUD_DEC_LS_T          e_ls;

    AUD_DEC_LS_MODE_T   e_mode;
}   AUD_DEC_CH_LS_MODE_T;


/* Memory buffer info. */
typedef struct _AUD_DEC_MEM_BUFFER_INFO_T
{
    VOID*  pv_tag;

    UINT8*  pui1_data;

    SIZE_T  z_len;
}   AUD_DEC_MEM_BUFFER_INFO_T;

//CC_AUD_DSP_SUPPORT_AUDYSSEY
typedef struct _AUD_DEC_AUDYSSEY_INFO_T
{
    AUD_DEC_AUDYSSEY_EFFECT_T      e_se_type;
    UINT32                         ui4_info;
}   AUD_DEC_AUDYSSEY_INFO_T;
//CC_AUD_DSP_SUPPORT_AUDYSSEY

/* Channel Delay */
typedef enum
{
    AUD_DEC_CHNL_DELAY_TYPE_ALL = 0,
    AUD_DEC_CHNL_DELAY_TYPE_INDIVIDUAL_CH,
    AUD_DEC_CHNL_DELAY_TYPE_ON,
    AUD_DEC_CHNL_DELAY_TYPE_OFF
} AUD_DEC_CHNL_DELAY_TYPE_T;

typedef struct _AUD_DEC_CHNL_DELAY_IND_T
{
    UINT16                   ui2_fl_delay; /* front left */
    UINT16                   ui2_fr_delay; /* front right */
    UINT16                   ui2_c_delay;  /* center */
    UINT16                   ui2_rl_delay; /* rear left */
    UINT16                   ui2_rr_delay; /* rear right */
    UINT16                   ui2_sw_delay; /* sub-woofer */
} AUD_DEC_CHNL_DELAY_IND_T;

typedef struct _AUD_DEC_CHNL_DELAY_T
{
    AUD_DEC_CHNL_DELAY_TYPE_T   e_ch_delay_type;
    AUD_DEC_OUT_PORT_T          e_out_port;

    union
    {
        UINT16                       ui2_delay;
        AUD_DEC_CHNL_DELAY_IND_T     t_ind_delay;
    } u;
}   AUD_DEC_CHNL_DELAY_T;


/* Copy Protection */
typedef enum
{
    AUD_DEC_COPY_PROTECT_OFF = 0,
    AUD_DEC_COPY_PROTECT_NO_RESTRICT,  /* Cp-bit = 1, L-bit = 0 */
    AUD_DEC_COPY_NO_FURTHER_COPY,       /* Cp-bit = 0, L-bit = 1 */
    AUD_DEC_COPY_PROTECT_COPY_ONCE,   /* Cp-bit = 0, L-bit = 0 */
    AUD_DEC_COPY_PROTECT_NEVER_COPY,  /* Cp-bit = 0, L-bit = 1 */
} AUD_DEC_COPY_PROTECT_T;

typedef enum
{
    AUD_DEC_CP_MODE_DEFAULT = 0, // default
    AUD_DEC_CP_MODE_USER, // user
    AUD_DEC_CP_MODE_CI,  // CI plus
} AUD_DEC_CP_MODE_T;


typedef struct _AUD_DEC_COPY_PROTECT_INFO_T
{
    AUD_DEC_CP_MODE_T             t_mode;
    AUD_DEC_COPY_PROTECT_T    t_protect_info;
} AUD_DEC_COPY_PROTECT_INFO_T;

typedef struct _AUD_DEC_SPDIF_STATUS_INFO_T
{
    AUD_DEC_CP_MODE_T  t_mode;
    UINT8 ui1_category_code;
    UINT8 ui1_copy_right;
} AUD_DEC_SPDIF_STATUS_INFO_T;

/* MTS */
typedef enum
{
    AUD_DEC_MTS_UNKNOWN = 0,
    AUD_DEC_MTS_MONO,
    AUD_DEC_MTS_STERRO,
    AUD_DEC_MTS_SUB_LANG,
    AUD_DEC_MTS_DUAL1,
    AUD_DEC_MTS_DUAL2,
    AUD_DEC_MTS_NICAM_MONO,
    AUD_DEC_MTS_NICAM_STEREO,
    AUD_DEC_MTS_NICAM_DUAL1,
    AUD_DEC_MTS_NICAM_DUAL2,
    AUD_DEC_MTS_FM_MONO,
    AUD_DEC_MTS_FM_STEREO
} AUD_DEC_MTS_T;


/* Sound Effect User mode setting */
/* AUD_DEC_SET_TYPE_SE_EQ_USER_LVL/AUD_DEC_GET_TYPE_SE_EQ_USER_LVL ***********/
typedef struct _AUD_DEC_SE_QA_USER_LVL_T
{
    AUD_DEC_EQ_TYPE_T   e_eq_type;
    UINT8               ui1_band_idx;
    INT8                i1_level;
    INT16	        i2_level_ex;
    BOOL	        b_is_level_ex;
} AUD_DEC_SE_QA_USER_LVL_T;


/* AUD_DEC_GET_TYPE_SE_EQ_USER_INFO_BAND *************************************/
typedef struct _AUD_DEC_SE_QA_USER_INFO_BAND_T
{
    AUD_DEC_EQ_TYPE_T   e_eq_type;
    UINT8               ui1_band_idx;
    INT8                i1_max_lvl;
    INT8                i1_min_lvl;
    UINT32              ui4_freq_lower_bound;
    UINT32              ui4_freq_upper_bound;
} AUD_DEC_SE_QA_USER_INFO_BAND_T;


/* AUD_DEC_GET_TYPE_TV_AUD_SYS_MASK/AUD_DEC_SET_TYPE_TV_AUD_SYS_MASK *********/
typedef struct _AUD_DEC_TV_AUD_SYS_T
{
    UINT32              ui4_tv_sys_mask;
    UINT32              ui4_aud_sys_mask;
} AUD_DEC_TV_AUD_SYS_T;


/* AUD_DEC_GET_TYPE_AUD_CLIP/AUD_DEC_SET_TYPE_AUD_CLIP ***********************/
typedef enum
{
    AUD_DEC_AUD_CLIP_TYPE_UNKNOWN = 0,
    AUD_DEC_AUD_CLIP_TYPE_TONE,
    AUD_DEC_AUD_CLIP_TYPE_CLIP_BUFF,
    AUD_DEC_AUD_CLIP_TYPE_MIXSOUND, //CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
    AUD_DEC_AUD_CLIP_TYPE_CLIP_CUSTOM
}   AUD_DEC_AUD_CLIP_TYPE_T;

typedef enum
{
    AUD_DEC_CLIP_CUSTOM_MODE_CUSTOM1 = 0,
    AUD_DEC_CLIP_CUSTOM_MODE_CUSTOM2,
    AUD_DEC_CLIP_CUSTOM_MODE_CUSTOM3,
    AUD_DEC_CLIP_CUSTOM_MODE_CUSTOM4
} AUD_DEC_CLIP_CUSTOM_MODE_T;


/* audio presentation types, mono / stereo */
typedef enum
{
    AUD_DEC_CHANNEL_TYPE_UNKNOWN = 0,
    AUD_DEC_CHANNEL_TYPE_MONO,
    AUD_DEC_CHANNEL_TYPE_STEREO
}   AUD_DEC_CHANNEL_TYPE_T;

typedef enum
{
    AUD_DEC_CLIP_CUSTOM_SETTING_TYPE_POWER_ON =0 ,
    AUD_DEC_CLIP_CUSTOM_SETTING_TYPE_POWER_OFF
}   AUD_DEC_CLIP_CUSTOM_SETTING_TYPE_T;

typedef enum
{
    AUD_DEC_CLIP_CUSTOM_SETTING_MODE_ON = 0,
    AUD_DEC_CLIP_CUSTOM_SETTING_MODE_OFF
}   AUD_DEC_CLIP_CUSTOM_SETTING_MODE_T;

typedef struct _AUD_DEC_AUD_CLIP_CUSTIOM_T
{
    AUD_DEC_CLIP_CUSTOM_SETTING_TYPE_T   e_drv_clip_custom_setting_type;
    AUD_DEC_CLIP_CUSTOM_SETTING_MODE_T  e_drv_clip_custom_setting_mode;
} AUD_DEC_AUD_CLIP_CUSTIOM_T;


typedef struct _AUD_DEC_AUD_CLIP_BUFF_T
{
    AUD_DEC_FMT_T  e_fmt;
    UINT8*         pui1_clip_buff;
    SIZE_T         z_clip_buff_size;
    AUD_DEC_CHANNEL_TYPE_T  e_PCM_channel_type;
    UINT32                  ui4_PCM_SampleRate;
    UINT8                   ui1_PCM_BitDepth;
} AUD_DEC_AUD_CLIP_BUFF_T;

#define AUD_CLIP_REPEAT_CNT_FOREVER    ((UINT32) 0)

typedef struct _AUD_DEC_AUD_CLIP_T
{
    AUD_DEC_AUD_CLIP_TYPE_T      e_type;
    UINT32                       ui4_out_port_mask;
    UINT32                       ui4_ch_mask;
    UINT32                       ui4_repeat_cnt;

    union
    {
        UINT32                   ui4_tone_freq; /* Hz */
        AUD_DEC_AUD_CLIP_BUFF_T  t_clip_buff;
        AUD_DEC_CLIP_CUSTOM_MODE_T e_aud_dec_clip_custom_mode;
    } u;
} AUD_DEC_AUD_CLIP_T;


/* AUD_DEC_GET_TYPE_AUD_CLIP_INFO ********************************************/
typedef struct _AUD_DEC_AUD_CLIP_INFO_T
{
    UINT32      ui4_duration;   /* msec */
    UINT32      ui4_time_position; /* msec */
} AUD_DEC_AUD_CLIP_INFO_T;


/* AUD_DEC_SET_TYPE_PL2_CTRL/AUD_DEC_GET_TYPE_PL2_CTRL ***********************/
typedef enum
{
    AUD_DEC_PL2_CTRL_SWITCH = 0,
    AUD_DEC_PL2_CTRL_MODE,
    AUD_DEC_PL2_CTRL_PANORAMA,
    AUD_DEC_PL2_CTRL_DIMENSION,
    AUD_DEC_PL2_CTRL_C_WIDTH
}   AUD_DEC_PL2_CTRL_T;

typedef enum
{
    AUD_DEC_PL2_SWITCH_ON = 0,
    AUD_DEC_PL2_SWITCH_OFF,
    AUD_DEC_PL2_SWITCH_AUTO
}   AUD_DEC_PL2_SWITCH_MODE_T;

typedef enum
{
    AUD_DEC_PL2_MODE_MODE_PROLGIC_EMULATION = 0,
    AUD_DEC_PL2_MODE_MODE_VIRTUAL,
    AUD_DEC_PL2_MODE_MODE_MUSIC,
    AUD_DEC_PL2_MODE_MODE_MOVIE,
    AUD_DEC_PL2_MODE_MODE_MATRIX,
    AUD_DEC_PL2_MODE_MODE_CUSTOM
}   AUD_DEC_PL2_MODE_MODE_T;

typedef enum
{
    AUD_DEC_PL2_PANORAMA_ON = 0,
    AUD_DEC_PL2_PANORAMA_OFF
}   AUD_DEC_PL2_PANORAMA_T;

typedef struct _AUD_DEC_PL2_VAL_MIN_MAX_T
{
    UINT8      ui1_curr_val;
    UINT8      ui1_min_val; /* Only used in AUD_DEC_GET_TYPE_PL2_CTRL */
    UINT8      ui1_max_val; /* Only used in AUD_DEC_GET_TYPE_PL2_CTRL */
} AUD_DEC_PL2_VAL_MIN_MAX_T;

typedef struct _AUD_DEC_PL2_CTRL_INFO_T
{
    AUD_DEC_PL2_CTRL_T    e_ctrl; /* IN */

    union
    {
        AUD_DEC_PL2_SWITCH_MODE_T   e_pl2_switch;
        AUD_DEC_PL2_MODE_MODE_T     e_pl2_mode;
        BOOL                        b_is_pl2_panorama_on;
        AUD_DEC_PL2_VAL_MIN_MAX_T   t_pl2_val;
    } u;

} AUD_DEC_PL2_CTRL_INFO_T;


/* AUD_DEC_GET_TYPE_AV_SYNC/AUD_DEC_SET_TYPE_AV_SYNC *************************/
typedef enum
{
    AUD_DEC_AV_SYNC_FREE_RUN = 0,
    AUD_DEC_AV_SYNC_AUD_SLAVE,
    AUD_DEC_AV_SYNC_AUD_MASTER
} AUD_DEC_AV_SYNC_INFO_T;

#ifdef CC_AUD_KARAOKE_MODE_SUPPORT
/* AUD_DEC_SET_TYPE_MM_MODE/AUD_DEC_SET_TYPE_MM_MODE *************************/
typedef enum
{
    AUD_DEC_MM_MODE_NORMAL = 0,
    AUD_DEC_MM_MODE_KARAOKE,
    AUD_DEC_MM_MODE_KARAOKE_MIC_PASS
}   AUD_DEC_MM_MODE_T;
#endif
/* AUD_DEC_GET_TYPE_HEADPHONE_COND *******************************************/
typedef enum
{
    AUD_DEC_HEADPHONE_COND_UNPLUGED = 0,
    AUD_DEC_HEADPHONE_COND_PLUGED
} AUD_DEC_HEADPHONE_COND_T;

/* AUD_DEC_GET_TYPE_HEADPHONE_DETECT_NFY/AUD_DEC_SET_TYPE_HEADPHONE_DETECT_NFY*/
typedef VOID (*x_aud_dec_hdph_detect_nfy_fct) (
    VOID*                     pv_nfy_tag,
    AUD_DEC_HEADPHONE_COND_T  e_nfy_cond,
    UINT32                    ui4_data_1,
    UINT32                    ui4_data_2 );

typedef struct _AUD_DEC_HDPH_DETECT_NFT_INFO_T
{
    x_aud_dec_hdph_detect_nfy_fct    pf_hdph_detect_nfy;
    VOID*                            pv_nfy_tag;
} AUD_DEC_HDPH_DETECT_NFT_INFO_T;

typedef VOID (*x_aud_dec_upload_data_nfy_fct) (
    UINT8                     u1DecId,
    VOID*                     pv_nfy_tag,
    AUD_DEC_COND_T            e_nfy_cond,
    UINT32                    ui4_data_1,
    UINT32                    ui4_data_2 );

typedef struct _AUD_DEC_UPLOAD_DATA_NFT_INFO_T
{
    x_aud_dec_upload_data_nfy_fct    pf_upload_data_nfy;
    VOID*                                        pv_nfy_tag;
} AUD_DEC_UPLOAD_DATA_NFT_INFO_T;


typedef struct _AUD_DEC_RM_SPEC_INFO_T
{
    UINT16          ui2_num_channel;
    UINT16          ui2_flavor_idx;
    UINT16          ui2_codec_frm_sz;
    UINT16          ui2_intlv_factor;
    UINT16          ui2_intlv_blk_sz;
    UINT32          ui4_sample_rate;
    BOOL            b_is_cook;
    void*           pv_codec_opaque_data;
} AUD_DEC_RM_SPEC_INFO_T;

/* AUD_DEC_GET_TYPE_MUTE_EX/AUD_DEC_SET_TYPE_MUTE_EX *************************/
typedef struct _AUD_DEC_MUTE_EX_INFO_T
{
    UINT32            ui4_muted_out_port_msk;
} AUD_DEC_MUTE_EX_INFO_T;

typedef struct _AUD_DEC_WMA_INFO_T
{
    UINT16 ui2_hdr_src;       /*W_WMA_HEADER_SOURCE*/
    UINT32 ui4_pkt_sz;        /*D_WMA_PACKET_SIZE*/
    UINT16 ui2_num_ch;        /*W_WMA_NUM_CH*/
    UINT32 ui4_sample_per_sec; /*D_WMA_SAMPLE_PER_SEC*/
    UINT32 ui4_bits_per_sec;   /*D_WMA_BYTE_PER_SEC*/
    UINT32 ui4_block_sz;       /*D_WMA_BLOCK_SIZE*/
    UINT16 ui2_encoder_opt;    /*W_WMA_ENCODER_OPTION*/
    UINT32 ui4_channel_mask;   /*D_WMA_CHANNEL_MASK*/
    UINT16 ui2_valid_bits;     /*W_WMA_VALID_BITS_PER_SAMPLE*/
    UINT16 ui2_adv_option;     /*W_WMA_ADVANCED_ENCODE_OPT*/
    UINT32 ui4_adv_option2;    /*W_WMA_ADVANCED_ENCODE_OPT2*/
} AUD_DEC_WMA_INFO_T;

typedef struct _AUD_DEC_AAC_INFO_T
{
    UINT32 ui4_num_ch;
    UINT32 ui4_sample_rate;
    UINT32 ui4_aac_profile;
    BOOL   b_frame_length;      //0: 1024,  1: 960
} AUD_DEC_AAC_INFO_T;

typedef enum
{
    AUD_DEC_PCM_TYPE_NORMAL,
    AUD_DEC_PCM_TYPE_MS_ADPCM,
    AUD_DEC_PCM_TYPE_IMA_ADPCM,
    AUD_DEC_PCM_TYPE_A_LAW,
    AUD_DEC_PCM_TYPE_U_LAW
} AUD_DEC_PCM_TYPE_T;

typedef struct _AUD_DEC_PCM_INFO_T
{
    AUD_DEC_PCM_TYPE_T e_pcm_type;
    UINT32             ui4_num_ch;
    UINT32             ui4_sample_rate;
    UINT16             ui2_block_align;
    UINT16             ui2_bits_per_sample;
    BOOL               b_big_endian;
    BOOL   b_info_not_specific;/*pcm info from dmx*/
	BOOL			   b_signed;
} AUD_DEC_PCM_INFO_T;

typedef struct _AUD_DEC_COOK_INFO_T
{
    UINT16          ui2_num_channel;
    UINT16          ui2_flavor_idx;
    UINT16          ui2_codec_frm_sz;
    UINT16          ui2_intlv_factor;
    UINT16          ui2_intlv_blk_sz;
    UINT32          ui4_sample_rate;
    void*           pv_codec_opaque_data;
} AUD_DEC_COOK_INFO_T;


typedef struct _AUD_DEC_VORBIS_INFO_T
{
    UINT8      ui1_vorbis_id[7];
    UINT32     ui4_vorbis_version;
    UINT8      ui1_audio_channel;
    UINT32     ui4_audio_sample_rate;
    UINT32     ui4_bitrate_maximun;
    UINT32     ui4_bitrate_norminal;
    UINT32     ui4_bitrate_minimun;
    UINT8      blocksize;
    UINT8      frameflag;
} AUD_DEC_VORBIS_INFO_T;

typedef struct _AUD_DEC_AC3_INFO_T
{
    UINT8      ui1_endian; // 0 = little, 1 = big
} AUD_DEC_AC3_INFO_T;


typedef struct _AUD_DEC_CODEC_SPEC_INFO_T
{
    AUD_DEC_FMT_T  e_fmt;
    union
    {
        AUD_DEC_WMA_INFO_T t_wma_info;
        AUD_DEC_AAC_INFO_T t_aac_info;
        AUD_DEC_PCM_INFO_T t_pcm_info;
        AUD_DEC_COOK_INFO_T t_cook_info;
        AUD_DEC_VORBIS_INFO_T t_vorbis_info;
        AUD_DEC_AC3_INFO_T t_ac3_info;
    } u;
} AUD_DEC_CODEC_SPEC_INFO_T;

typedef enum
{
    AUD_SE_SOUND_MODE_DEFAULT = 0,
    AUD_SE_SOUND_MODE_STANDARD ,
    AUD_SE_SOUND_MODE_DYNAMIC,
    AUD_SE_SOUND_MODE_CLEAR_VOICE,
    AUD_SE_SOUND_MODE_CUSTOM_1,
    AUD_SE_SOUND_MODE_CUSTOM_2,
    AUD_SE_SOUND_MODE_CUSTOM_3,
    AUD_SE_SOUND_MODE_CUSTOM_4,
    AUD_SE_SOUND_MODE_CUSTOM_5
}   AUD_SE_SOUND_MODE_T;

/* FM radio detect result */
typedef enum
{
    AUD_DEC_FM_RADIO_NON_DETECTED = 0,
    AUD_DEC_FM_RADIO_DETECTED
} AUD_DEC_FM_RADIO_DET_T;

/* equalizer types. */
typedef enum
{
    AUD_DEC_SURROUND_TYPE_OFF = 0,
    AUD_DEC_SURROUND_TYPE_MVS_DEFAULT,
    AUD_DEC_SURROUND_TYPE_MONO_TO_STEREO,
    AUD_DEC_SURROUND_TYPE_USER_1,
    AUD_DEC_SURROUND_TYPE_USER_2,
    AUD_DEC_SURROUND_TYPE_USER_3,
    AUD_DEC_SURROUND_TYPE_USER_4,
    AUD_DEC_SURROUND_TYPE_USER_5,
    AUD_DEC_SURROUND_TYPE_USER_6,
 }   AUD_DEC_SURROUND_TYPE_T;

typedef enum
{
    AUD_DEC_SE_SPH_MODE_OFF = 0,
    AUD_DEC_SE_SPH_MODE_DEFAULT,
    AUD_DEC_SE_SPH_MODE_MODE1,
    AUD_DEC_SE_SPH_MODE_MODE2

 }   AUD_DEC_SE_SPH_MODE_T;

typedef struct _AUD_DEC_SET_TYPE_PEQ_INFO_T
{
    UINT8           ui1_PEQSet;
    UINT32         ui4_Frequency;
    UINT16         ui2_BW;
    INT16           i2_Gain;
}   AUD_DEC_SET_TYPE_PEQ_INFO_T;

typedef enum
{
    AUD_DEC_BTNSND_MODE_CUSTOM1 = 0,
    AUD_DEC_BTNSND_MODE_CUSTOM2,
    AUD_DEC_BTNSND_MODE_CUSTOM3,
    AUD_DEC_BTNSND_MODE_CUSTOM4
} AUD_DEC_BTNSND_MODE_T;

typedef enum
{
    AUD_DEC_BTNSND_ENABLE_OFF= 0,
    AUD_DEC_BTNSND_ENABLE_ON
} AUD_DEC_BTNSND_ENABLE_T;

typedef enum
{
    AUD_DEC_BTNSND_ENABLE = 0,
    AUD_DEC_BTNSND_PLAY,
    AUD_DEC_BTNSND_SRC
} AUD_DEC_BTNSND_CTRL_T;

typedef struct _AUD_DEC_BTNSND_INFO_T
{
    AUD_DEC_BTNSND_CTRL_T      e_btnsnd_type;

    VOID*                          pv_info;
}   AUD_DEC_BTNSND_INFO_T;

typedef enum
{
    AUD_DEC_PANFADE_DISABLE = 0,
    AUD_DEC_PANFADE_ENABLE
} AUD_DEC_PANFADE_CTRL_T;

typedef struct _AUD_DEC_AUTO_DETECT_AUD_SYSTEM_INFO_T
{
    ISO_3166_COUNT_T  t_country_code;
    UINT32         ui4_aud_sys;
    UINT32         ui4_tv_sys;
}   AUD_DEC_AUTO_DETECT_AUD_SYSTEM_INFO_T;


//#ifdef CC_AUD_DATA_UPLOAD_SUPPORT
typedef enum
{
    AUD_DEC_UPLOAD_OFF = 0,
    AUD_DEC_UPLOAD_PCM,
    AUD_DEC_UPLOAD_MP3,
    AUD_DEC_UPLOAD_SBC,
    AUD_DEC_UPLOAD_PCM_WO_NOTIFY
} AUD_DEC_UPLOAD_MODE_T;

typedef struct
{
    UINT32 u4Pts;
    UINT32 u4Addr;
    UINT32 u4Size;
} AUD_DEC_UPLOAD_INFO_T;
//#endif

typedef struct
{
    UINT32 ui4_buffer_start_Addr;
    UINT32 ui4_bufer_Size;
} AUD_DEC_BUFFER_INFO_T;

typedef struct
{
    UINT32 ui4_pcm_bits;
    UINT32 ui4_pcm_channels;
    UINT32 ui4_pcm_sample_rate;
} AUD_DEC_UPLOAD_PCM_INFO_T;

/* DDC control
*/

// Setup mode
typedef enum
{
    AUD_DEC_DDC_ALL = 0,
    AUD_DEC_DDC_CONTROL,
    AUD_DEC_DDC_ID,
    AUD_DEC_DDC_MIXER
} AUD_DEC_DDC_MODE_T;

// Control mode
typedef enum
{
    AUD_DEC_DDC_SINGLE_IN_SINGLE_DEC = 0,
    AUD_DEC_DDC_SINGLE_IN_DUAL_DEC,
} AUD_DEC_DDC_CONTROL_MODE_T;

// ID
typedef enum
{
    AUD_DEC_DDC_ID_UNKNOWN = 0,
    AUD_DEC_DDC_ID_1,
    AUD_DEC_DDC_ID_2,
    AUD_DEC_DDC_ID_3,
} AUD_DEC_DDC_ID_T;

// Mixer mode
typedef enum
{
    AUD_DEC_DDC_MAIN_MAIN = 0, // Left_Right
    AUD_DEC_DDC_MIX_MIX,
    AUD_DEC_DDC_ASSOC_MAIN,
    AUD_DEC_DDC_ASSOC_ASSOC
} AUD_DEC_DDC_MIXER_MODE_T;

typedef struct _AUD_DEC_DDC_INFO_T
{
    AUD_DEC_DDC_MODE_T eIndex; // setup mode
    AUD_DEC_DDC_CONTROL_MODE_T eControl; // control mode
    AUD_DEC_DDC_ID_T eAssocId; // associated audio ID
    AUD_DEC_DDC_MIXER_MODE_T eMixerMode; // mixer mode
} AUD_DEC_DDC_INFO_T;

typedef enum
{
    AUD_DEC_KARAOKE_CONNECT = 0,
    AUD_DEC_KARAOKE_ENABLE,
    AUD_DEC_KARAOKE_ECHO,
    AUD_DEC_KARAOKE_VOCAL_CANEL,
    AUD_DEC_KARAOKE_KEY_SHIFT,
    AUD_DEC_KARAOKE_MIC_VOL1,
    AUD_DEC_KARAOKE_MIC_VOL2
} AUD_DEC_KARAOKE_CTRL_T;

typedef struct _AUD_DEC_KARAOKE_INFO_T
{
    AUD_DEC_KARAOKE_CTRL_T   e_karaoke_type;
    union
    {
        BOOL     fg_enable;
        UINT16   ui2_echo_val;
        BOOL     fg_vocal_cancel;
        UINT16   ui2_key_shift;
        UINT16   ui2_mic_vol1;
        UINT16   ui2_mic_vol2;
    } u;
}AUD_DEC_KARAOKE_INFO_T;

#endif /* _X_AUD_DEC_H_ */

