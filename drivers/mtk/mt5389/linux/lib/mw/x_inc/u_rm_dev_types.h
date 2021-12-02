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
/*! @file u_rm_dev_types.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains the device type definitions, which are
 *         exported.
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_RM_DEV_TYPES_H_
#define _U_RM_DEV_TYPES_H_

/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_RES_MNGR
 *  @{
 */
/*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_rm.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* Component types */
#define DRVT_UNKNOWN  ((DRV_TYPE_T)  0)    /**<component type       */
#define DRVT_VIRTUAL  ((DRV_TYPE_T)  1)    /**<component type        */

#define DRVT_TUNER_SAT_DIG         ((DRV_TYPE_T)  16)    /**<component type        */
#define DRVT_TUNER_CAB_DIG         ((DRV_TYPE_T)  17)    /**<component type        */
#define DRVT_TUNER_TER_DIG         ((DRV_TYPE_T)  18)    /**<component type        */
#define DRVT_TUNER_SAT_ANA         ((DRV_TYPE_T)  19)    /**<component type        */
#define DRVT_TUNER_CAB_ANA         ((DRV_TYPE_T)  20)    /**<component type        */
#define DRVT_TUNER_TER_ANA         ((DRV_TYPE_T)  21)    /**<component type        */
#define DRVT_TUNER_CAB_DIG_OOB_TX  ((DRV_TYPE_T)  22)    /**<component type        */
#define DRVT_TUNER_CAB_DIG_OOB_RX  ((DRV_TYPE_T)  23)    /**<component type        */
#define DRVT_TUNER_SAT_ANA_SCART_OUT  ((DRV_TYPE_T)  24)    /**<component type        */
#define DRVT_TUNER_CAB_ANA_SCART_OUT  ((DRV_TYPE_T)  25)    /**<component type        */
#define DRVT_TUNER_TER_ANA_SCART_OUT  ((DRV_TYPE_T)  26)    /**<component type        */

#define DRVT_DEMUX_TS_PES_PACKET         ((DRV_TYPE_T)  32)    /**<component type        */
#define DRVT_DEMUX_TS_PES_PACKET_MEMORY  ((DRV_TYPE_T)  33)    /**<component type        */
#define DRVT_DEMUX_TS_PCR                ((DRV_TYPE_T)  34)    /**<component type        */
#define DRVT_DEMUX_TS_SECTION_MEMORY     ((DRV_TYPE_T)  35)    /**<component type        */
#define DRVT_DEMUX_TS_TS_PACKET          ((DRV_TYPE_T)  36)    /**<component type        */
#define DRVT_DEMUX_TS_TS_PACKET_MEMORY   ((DRV_TYPE_T)  37)    /**<component type        */
#define DRVT_DEMUX_PS_PES_PACKET         ((DRV_TYPE_T)  38)    /**<component type        */
#define DRVT_DEMUX_PS_PES_PACKET_MEMORY  ((DRV_TYPE_T)  39)    /**<component type        */
#define DRVT_DEMUX_SECTION_FILTER        ((DRV_TYPE_T)  40)    /**<component type        */
#define DRVT_DEMUX_CONTAINER_ES          ((DRV_TYPE_T)  41)    /**<component type        */
#define DRVT_DEMUX_CONTROL               ((DRV_TYPE_T)  42)    /**<component type       */
#define DRVT_DEMUX_CONTAINER_ES_MEMORY   ((DRV_TYPE_T)  43)    /**<component type        */

#define DRVT_DEMUX_REC          ((DRV_TYPE_T)  45)    /**<component type        */
#define DRVT_DEMUX_REC_PCR      ((DRV_TYPE_T)  46)    /**<component type        */
#define DRVT_PVR_CTRL           ((DRV_TYPE_T)  47)    /**<component type        */
#define DRVT_PVR_STREAM         ((DRV_TYPE_T)  48)    /**<component type        */
#define DRVT_PVR_STREAM_PCR     ((DRV_TYPE_T)  49)    /**<component type        */

#define DRVT_BROADCAST_CA       ((DRV_TYPE_T)  50)    /**<component type        */
#define DRVT_BROADCAST_CI       ((DRV_TYPE_T)  51)    /**<component type        */
#define DRVT_PLAYBACK_SEC_MNGR  ((DRV_TYPE_T)  52)    /**<component type        */

#define DRVT_VID_DEC    ((DRV_TYPE_T)  64)    /**<component type        */
#define DRVT_VID_PLANE  ((DRV_TYPE_T)  65)    /**<component type        */
#define DRVT_PLA_MXR    ((DRV_TYPE_T)  66)    /**<component type        */
#define DRVT_TV_ENC     ((DRV_TYPE_T)  67)    /**<component type        */
#define DRVT_AUD_DEC    ((DRV_TYPE_T)  68)    /**<component type        */
#define DRVT_PCR_DEC    ((DRV_TYPE_T)  69)    /**<component type        */
#define DRVT_JPG_DEC                        ((DRV_TYPE_T)  70)    /**<component type        */
#define DRVT_AUX_JPG_DEC                    ((DRV_TYPE_T)  71)    /**<component type        */
#define DRVT_BMP_DEC                        ((DRV_TYPE_T)  72)    /**<component type        */
#define DRVT_GIF_DEC                        ((DRV_TYPE_T)  73)    /**<component type        */
#define DRVT_PNG_DEC                        ((DRV_TYPE_T)  74)    /**<component type        */
#define DRVT_MNG_DEC                        ((DRV_TYPE_T)  75)    /**<component type        */
#define DRVT_RLE_DEC                        ((DRV_TYPE_T)  76)    /**<component type        */
#define DRVT_BDPGIG_DEC                     ((DRV_TYPE_T)  77)    /**<component type        */
#define DRVT_HDDVDSPU_DEC                   ((DRV_TYPE_T)  78)    /**<component type        */
#define DRVT_DVDSPU_DEC                     ((DRV_TYPE_T)  79)    /**<component type        */

#define DRVT_RTC                            ((DRV_TYPE_T)  80)    /**<component type        */

#define DRVT_HW_IDE     ((DRV_TYPE_T)  85)    /**<component type        */
#define DRVT_HW_FCI     ((DRV_TYPE_T)  86)    /**<component type       */
#define DRVT_HW_USB     ((DRV_TYPE_T)  87)    /**<component type        */
#define DRVT_HW_1394    ((DRV_TYPE_T)  88)    /**<component type        */
#define DRVT_HW_NAND    ((DRV_TYPE_T)  89)    /**<component type        */
#define DRVT_HW_NOR     ((DRV_TYPE_T)  90)    /**<component type        */
#define DRVT_HW_EEPROM  ((DRV_TYPE_T)  91)    /**<component type        */
#define DRVT_HW_ETHERNET    ((DRV_TYPE_T)  92)    /**<component type        */

#define DRVT_EEPROM           ((DRV_TYPE_T)  96)    /**<component type        */
#define DRVT_NOR_FLASH        ((DRV_TYPE_T)  97)    /**<component type        */
#define DRVT_NAND_FLASH       ((DRV_TYPE_T)  98)    /**<component type        */
#define DRVT_MEM_CARD         ((DRV_TYPE_T)  99)    /**<component type        */
#define DRVT_HARD_DISK        ((DRV_TYPE_T) 100)    /**<component type        */
#define DRVT_USB_MASS_STORAGE ((DRV_TYPE_T) 101)    /**<component type        */
#define DRVT_OPTICAL_DISC     ((DRV_TYPE_T) 102)    /**<component type        */
#define DRVT_OPTICAL_DRIVE    ((DRV_TYPE_T) 103)    /**<component type        */
#define DRVT_MEM_CARD_READER  ((DRV_TYPE_T) 104)    /**<component type        */
#define DRVT_HUB              ((DRV_TYPE_T) 105)    /**<component type        */
#define DRVT_USB_PTP_MTP      ((DRV_TYPE_T) 106)    /**<component type        */
#define DRVT_DLNA_DMS         ((DRV_TYPE_T) 107)    /**<component type        */
#define DRVT_USB_BT           ((DRV_TYPE_T) 108)    /**<component type        */
#define DRVT_SMB              ((DRV_TYPE_T) 109)    /**<component type        */

#ifndef SN_FLASH_USE_NAND
#define DRVT_FLASH            DRVT_NOR_FLASH
#else
#define DRVT_FLASH            DRVT_NAND_FLASH
#endif

#define DRVT_COM_RS_232  ((DRV_TYPE_T)  112)    /**<component type        */

#define DRVT_IND_POWER     ((DRV_TYPE_T)  128)    /**<component type        */
#define DRVT_IND_PLAYBACK  ((DRV_TYPE_T)  129)    /**<component type        */
#define DRVT_IND_RECORD    ((DRV_TYPE_T)  130)    /**<component type        */
#define DRVT_IND_FORWARD   ((DRV_TYPE_T)  131)    /**<component type        */
#define DRVT_IND_REWIND    ((DRV_TYPE_T)  132)    /**<component type        */
#define DRVT_IND_PAUSE     ((DRV_TYPE_T)  133)    /**<component type        */
#define DRVT_IND_MAIL      ((DRV_TYPE_T)  134)    /**<component type        */
#define DRVT_IND_REMINDER  ((DRV_TYPE_T)  135)    /**<component type        */

#define DRVT_FP_DISPLAY  ((DRV_TYPE_T)  144)    /**<component type        */

#define DRVT_IRRC  ((DRV_TYPE_T)  160)    /**<component type        */

#define DRVT_OSD_PLANE  ((DRV_TYPE_T)  176)    /**<component type        */

#define DRVT_GPU  ((DRV_TYPE_T)  192)    /**<component type        */

#define DRVT_POD  ((DRV_TYPE_T)  208)    /**<component type        */

#define DRVT_CRYPTO_RANDOM_NUM      ((DRV_TYPE_T)  224)    /**<component type        */
#define DRVT_CRYPTO_SHA_1           ((DRV_TYPE_T)  225)    /**<component type        */
#define DRVT_CRYPTO_DFAST           ((DRV_TYPE_T)  226)    /**<component type        */
#define DRVT_CRYPTO_RSA             ((DRV_TYPE_T)  227)    /**<component type        */
#define DRVT_CRYPTO_DIFFIE_HELLMAN  ((DRV_TYPE_T)  228)    /**<component type        */
#define DRVT_CRYPTO_3DES            ((DRV_TYPE_T)  229)    /**<component type        */

#define DRVT_DESCRAMBLER            ((DRV_TYPE_T)  240)    /**<component type        */

#define DRVT_AVC_COMP_VIDEO  ((DRV_TYPE_T) 256)    /**<component type        */
#define DRVT_AVC_S_VIDEO     ((DRV_TYPE_T) 257)    /**<component type        */
#define DRVT_AVC_Y_PB_PR     ((DRV_TYPE_T) 258)    /**<component type        */
#define DRVT_AVC_VGA         ((DRV_TYPE_T) 259)    /**<component type        */
#define DRVT_AVC_SCART       ((DRV_TYPE_T) 260)    /**<component type        */
#define DRVT_AVC_SCART_OUT   ((DRV_TYPE_T) 261)    /**<component type        */
#define DRVT_AVC_DVI         ((DRV_TYPE_T) 262)    /**<component type        */
#define DRVT_AVC_HDMI        ((DRV_TYPE_T) 263)    /**<component type        */
#define DRVT_AVC_AUDIO_INP   ((DRV_TYPE_T) 264)    /**<component type        */
#define DRVT_AVC_SPDIF       ((DRV_TYPE_T) 265)    /**<component type        */
#define DRVT_AVC_COMBI       ((DRV_TYPE_T) 266)    /**<component type        */

#define DRVT_VIDEO_OUT              ((DRV_TYPE_T) 268)    /**<component type        */

#define DRVT_TV_DEC  ((DRV_TYPE_T) 272)    /**<component type        */

#define DRVT_1394_SRC   ((DRV_TYPE_T) 288)    /**<component type        */
#define DRVT_1394_DEST  ((DRV_TYPE_T) 289)    /**<component type       */
#define DRVT_PWR_CTRL  ((DRV_TYPE_T) 304)    /**<component type        */

#define DRVT_NET_DOCSIS_RFI         ((DRV_TYPE_T) 310)    /**<component type        */
#define DRVT_NET_ETHERNET_802_3     ((DRV_TYPE_T) 311)    /**<component type        */
#define DRVT_NET_ETHERNET_802_11    ((DRV_TYPE_T) 312)    /**<component type        */
#define DRVT_NET_USB                ((DRV_TYPE_T) 313)    /**<component type        */
#define DRVT_NET_SERIAL             ((DRV_TYPE_T) 314)    /**<component type        */
#define DRVT_NET_LOOPBACK           ((DRV_TYPE_T) 319)    /**<component type        */

#define DRVT_HDMI_CEC           ((DRV_TYPE_T) 340)    /**<component type        */

#define DRVT_BUF_AGENT      ((DRV_TYPE_T) 350)    /**<component type        */

#define DRVT_STC_CTRL        ((DRV_TYPE_T) 351)    /**<component type        */

#define DRVT_CUST_DRV       ((DRV_TYPE_T) 500)    /**<component type        */

#define DRVT_DRM          ((DRV_TYPE_T) 550)    /**<component type        */

#define DRVT_CHIP_SPEC    ((DRV_TYPE_T) 600)    /**<component type        */

/*----------------------------------------------------------------------------*/
/*! @} */                                          /* end of groupMW_RES_MNGR */
/*----------------------------------------------------------------------------*/

#endif /* _U_RM_DEV_TYPES_H */
