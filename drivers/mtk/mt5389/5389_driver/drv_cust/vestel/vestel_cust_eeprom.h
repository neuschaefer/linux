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
#ifndef MTK_CUST_EEPROM_H
#define MTK_CUST_EEPROM_H
//#include "drv_default.h"
enum
{
    EEP_LOADER_ENTER_STANDBY_INSTALLED_1,
    EEP_LOADER_ENTER_STANDBY_INSTALLED_2,
    EEP_LOADER_ENTER_STANDBY_INSTALLED_3,
    EEP_LOADER_ENTER_STANDBY_INSTALLED_4,
    EEP_LOADER_ENTER_STANDBY,
    EEP_LOADER_STANDBY_STATUS,
    EEP_TOUCHPAD_INSTALLED_1,
    EEP_TOUCHPAD_INSTALLED_2,
    EEP_TOUCHPAD_INSTALLED_3,
    EEP_TOUCHPAD_INSTALLED_4,
    EEP_TOUCHPAD_1_TOP,
    EEP_TOUCHPAD_1_BTM,
    EEP_TOUCHPAD_2_TOP,
    EEP_TOUCHPAD_2_BTM,
    EEP_TOUCHPAD_3_TOP,
    EEP_TOUCHPAD_3_BTM,
    EEP_TOUCHPAD_4_TOP,
    EEP_TOUCHPAD_4_BTM,
    EEP_TOUCHPAD_5_TOP,
    EEP_TOUCHPAD_5_BTM,
    EEP_TOUCHPAD_6_TOP,
    EEP_TOUCHPAD_6_BTM,
    EEP_TOUCHPAD_7_TOP,
    EEP_TOUCHPAD_7_BTM,
    EEP_TOUCHPAD_8_TOP,
    EEP_TOUCHPAD_8_BTM,
    EEP_TOUCHPAD_9_TOP,
    EEP_TOUCHPAD_9_BTM,
    EEP_TOUCHPAD_10_TOP,
    EEP_TOUCHPAD_10_BTM,
    EEP_TOUCHPAD_11_TOP,
    EEP_TOUCHPAD_11_BTM,
    EEP_LODER_VERSION_1,
    EEP_LODER_VERSION_2,
    EEP_LODER_VERSION_3,
    EEP_LODER_VERSION_4,
    EEP_OAD_ENABLE,
    EEP_TOUCHPAD_OR_MAGICBUTTON,
    EEP_SCART_WAKEUP_EN,
    EEP_VGA_WAKEUP_EN,
    EEP_BOOT_LOGO_EN,
    EEP_OAD_PKG_PHY_ADDR_1,
    EEP_OAD_PKG_PHY_ADDR_2,
    EEP_OAD_PKG_PHY_ADDR_3,
    EEP_OAD_PKG_PHY_ADDR_4,
    EEP_AUDIO_SRS_ENABLE,
    EEP_AUDIO_MPEG4_ENABLE,
    EEP_RTC_WAKEUP_BACKLT_EN,
    DRV_CUSTOM_EEPROM_END
};

#define EEP_LOADER_ENTER_STANDBY_INSTALLED_1_OFFSET EEP_LOADER_ENTER_STANDBY_INSTALLED_1
#define EEP_LOADER_ENTER_STANDBY_OFFSET EEP_LOADER_ENTER_STANDBY
#define EEP_LOADER_STANDBY_STATUS EEP_LOADER_STANDBY_STATUS
#define EEP_TOUCHPAD_INSTALLED_1_OFFSET EEP_TOUCHPAD_INSTALLED_1
#define EEP_TOUCHPAD_1_TOP_OFFSET EEP_TOUCHPAD_1_TOP
#define EEP_LOADER_VERSION EEP_LODER_VERSION_1
#define EEP_OAD_ENABLE_OFFSET EEP_OAD_ENABLE
#define EEP_TOUCHPAD_OR_MAGICBUTTON_OFFSET EEP_TOUCHPAD_OR_MAGICBUTTON
#define EEP_SCART_WAKEUP_EN_OFFSET EEP_SCART_WAKEUP_EN
#define EEP_AUDIO_SRS_ENABLE_OFFSET            EEP_AUDIO_SRS_ENABLE
#define EEP_AUDIO_MPEG4_ENABLE_OFFSET            EEP_AUDIO_MPEG4_ENABLE
#define EEP_VGA_WAKEUP_EN_OFFSET EEP_VGA_WAKEUP_EN
#define EEP_BOOT_LOGO_EN_OFFSET EEP_BOOT_LOGO_EN
#define EEP_OAD_PKG_PHY_ADDR_OFFSET EEP_OAD_PKG_PHY_ADDR_1
#define EEP_RTC_WAKEUP_BACKLT_EN_OFFSET EEP_RTC_WAKEUP_BACKLT_EN

#endif /*MTK_CUST_EEPROM_H */



