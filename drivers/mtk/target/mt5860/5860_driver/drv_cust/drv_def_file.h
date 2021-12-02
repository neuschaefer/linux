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

#ifndef DRV_DEF_FILE_H
#define DRV_DEF_FILE_H

#ifndef LOADER_LOGO_H_FILE
#define LOADER_LOGO_H_FILE              "mtk/MediaTek_600x100x5.h"
#endif

#ifndef DRV_LOGO_H_FILE
#define DRV_LOGO_H_FILE              "mtk/MediaTek_DrvLogo.h"
#endif

#ifndef AUDIO_INCLUDE_FILE
#define AUDIO_INCLUDE_FILE              "mtk/mtk_aud.h"
#endif

#ifndef CC_UBOOT
#ifndef GPIO_INCLUDE_FILE
#define GPIO_INCLUDE_FILE               "mtk/mtk_gpio.h"
#endif

#ifndef VGA_INCLUDE_FILE
#define VGA_INCLUDE_FILE                "mtk/mtk_vga.h"
#endif

#ifndef EDID_INCLUDE_FILE
#define EDID_INCLUDE_FILE                "mtk/mtk_edid.h"
#endif

#ifndef VGA_AUTOCOLOR_TABLE_INCLUDE_FILE
#define VGA_AUTOCOLOR_TABLE_INCLUDE_FILE                    "mtk/mtk_vga_autocolor_table.h"
#endif

#ifndef NONLINEAR_INCLUDE_FILE
#define NONLINEAR_INCLUDE_FILE          "mtk/mtk_nonlinear_tbl.h"
#endif

#ifndef T8032UP_INCLUDE_FILE
#define T8032UP_INCLUDE_FILE            "mtk/mtk_t8032fw.h"
#endif
#ifndef QTYTBL_INCLUDE_FILE
#define QTYTBL_INCLUDE_FILE     "mtk/QTY_DEFAULT/mtk_QtyTbl.h"
#endif

#ifndef COLOR_PARAM_INCLUDE_FILE
#define COLOR_PARAM_INCLUDE_FILE        "mtk/PANEL_DEFAULT/color_param.h"
#endif

#ifndef CUSTOM_VIDEO_DRV_BODY_FILE
#define CUSTOM_VIDEO_DRV_BODY_FILE  "mtk/default_video_drv.c"
#endif

#ifndef CUSTOM_BACKLIGHT_DRV_BODY_FILE
#define CUSTOM_BACKLIGHT_DRV_BODY_FILE  "mtk/default_backlight_drv.c"
#endif


#ifndef RTC_EXT_IMPL
#define RTC_EXT_IMPL                    "mtk/mtk_rtc_ext.c"
#endif

#ifndef NPTV_SOURCE_TABLE
#define NPTV_SOURCE_TABLE               "mtk/source_table.c" 
#endif

#ifndef HDMI_GPIO_CONF
#define HDMI_GPIO_CONF                  "mtk/hdmi_conf.c" 
#endif

#ifndef TCON_CUST_FILE
#define TCON_CUST_FILE                  "mtk/dummy_tcon_cust.c" 
#endif

#ifndef DMX_INCLUDE_FILE
#define DMX_INCLUDE_FILE               "mtk/mtk_dmx.h"
#endif

#ifndef CUSTOM_AUDIO_AMP_FILE
    #define CUSTOM_AUDIO_AMP_FILE       AUDIO_AMP_CUST_FILE_NAME
#endif // #ifndef CUSTOM_AUDIO_AMP_FILE

#endif // CC_UBOOT

#ifndef PDWNC_EXT_IMPL
#define PDWNC_EXT_IMPL                  "mtk/mtk_pdwnc_ext.c"
#endif

#ifndef SIF_INCLUDE_FILE
#define SIF_INCLUDE_FILE               "mtk/mtk_sif_cfg.h"
#endif



#endif /* DRV_DEF_FILE_H */

