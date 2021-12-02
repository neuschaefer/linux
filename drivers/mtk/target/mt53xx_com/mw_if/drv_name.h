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
#if ! defined(DRV_NAME_H)
#define DRV_NAME_H

#include "x_sys_name.h"

/* Component names */
#define DRVN_UNKNOWN                    "UNKNOWN"

#define DRVN_TUNER_SAT_DIG              SN_MAIN_TUNER_SAT_DIG
#define DRVN_TUNER_CAB_DIG              SN_MAIN_TUNER_CAB_DIG
#define DRVN_TUNER_TER_DIG              SN_MAIN_TUNER_TER_DIG
#define DRVN_TUNER_SAT_ANA              NULL
#define DRVN_TUNER_CAB_ANA              NULL
#define DRVN_TUNER_TER_ANA              NULL
#define DRVN_TUNER_CAB_DIG_OOB_TX       NULL
#define DRVN_TUNER_CAB_DIG_OOB_RX       NULL

#define DRVN_DEMUX_TS_PES_PACKET        NULL
#define DRVN_DEMUX_TS_PES_PACKET_MEMORY NULL
#define DRVN_DEMUX_TS_PCR               NULL
#define DRVN_DEMUX_TS_SECTION_MEMORY    SN_DEMUX_TS_SECTION_MEMORY
#define DRVN_DEMUX_TS_TS_PACKET         NULL
#define DRVN_DEMUX_TS_TS_PACKET_MEMORY  NULL
#define DRVN_DEMUX_PS_PES_PACKET        NULL
#define DRVN_DEMUX_PS_PES_PACKET_MEMORY NULL
#define DRVN_DEMUX_SECTION_FILTER       SN_DEMUX_SECTION_FILTER

#define DRVN_BROADCAST_CA               NULL
#define DRVN_BROADCAST_CI               NULL
#define DRVN_PLAYBACK_SEC_MNGR          NULL

#define DRVN_VID_DEC                    NULL
#define DRVN_VID_PLANE                  NULL
#define DRVN_PLA_MXR                    NULL
#define DRVN_TV_ENC                     NULL
#define DRVN_AUD_DEC                    NULL
#define DRVN_PCR_DEC                    NULL
#define DRVN_AVC_SCART_OUT              NULL

#define DRVN_RTC                        NULL
#define DRVN_STC_CTRL                   NULL

#define DRVN_EEPROM                     SN_EEPROM_WITH_IDX
#define DRVN_NOR_FLASH                  SN_NOR_FLASH_WITH_IDX
#define DRVN_NAND_FLASH                 SN_NAND_FLASH_WITH_IDX
#define DRVN_MEM_CARD_MS                SN_MEM_CARD_0
#define DRVN_MEM_CARD_SD                SN_MEM_CARD_1
#define DRVN_MEM_CARD_CF                SN_MEM_CARD_2
#define DRVN_MEM_CARD_SMXD              SN_MEM_CARD_3
#define DRVN_HARD_DISK                  NULL
#define DRVN_USB_MASS                   SN_USB_MASS_STORAGE_WITH_IDX
#define DRVN_USB_PTP_MTP                SN_USB_PTP_MTP_WITH_IDX

#define DRVN_COM_RS_232                 SN_COM_RS_232_DBG_PORT

#define DRVN_IND_POWER                  SN_IND_POWER
#define DRVN_IND_PLAYBACK               SN_IND_PLAYBACK
#define DRVN_IND_RECORD                 SN_IND_RECORD
#define DRVN_IND_FORWARD                SN_IND_FORWARD
#define DRVN_IND_REWIND                 SN_IND_REWIND
#define DRVN_IND_PAUSE                  SN_IND_PAUSE
#define DRVN_IND_MAIL                   SN_IND_MAIL
#define DRVN_IND_REMINDER               SN_IND_REMINDER

#define DRVN_FP_DISPLAY                 SN_FP_DISPLAY

#define DRVN_IRRC                       SN_IRRC

#define DRVN_OSD_PLANE                  SN_OSD_PL_GRAPHIC

#define DRVN_GPU                        SN_OSD_PL_IMAGE

#define DRVN_POD                        SN_POD

#define DRVN_CRYPTO_RANDOM_NUM          SN_CRYPTO_RANDOM_NUM
#define DRVN_CRYPTO_SHA_1               SN_CRYPTO_SHA_1
#define DRVN_CRYPTO_DFAST               SN_CRYPTO_DFAST
#define DRVN_CRYPTO_RSA                 SN_CRYPTO_RSA
#define DRVN_CRYPTO_DIFFIE_HELLMAN      SN_CRYPTO_DIFFIE_HELLMAN
#define DRVN_CRYPTO_3DES                SN_CRYPTO_3DES

#endif //DRV_NAME_H
