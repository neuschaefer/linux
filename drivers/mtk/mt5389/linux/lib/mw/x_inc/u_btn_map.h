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

#ifndef _U_BTN_MAP_H_
#define _U_BTN_MAP_H_

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* Definition of key source */
#define KEY_SRC_REMOTE_CTRL         ((unsigned long) 0x00000000)    /**<RC key define        */
#define KEY_SRC_FRONT_PANEL         ((unsigned long) 0x10000000)    /**<RC key define        */
#define KEY_SRC_SW                  ((unsigned long) 0x20000000)    /**<RC key define        */
#define KEY_SRC_SW_1                ((unsigned long) 0x30000000)    /**<RC key define        */
#define KEY_SRC_SW_2                ((unsigned long) 0x40000000)    /**<RC key define        */

/* Definiton of key group */
#define KEY_GROUP_DIGIT             ((unsigned long) 0x00010000)    /**<RC key define        */
#define KEY_GROUP_ALPHA             ((unsigned long) 0x00020000)    /**<RC key define        */
#define KEY_GROUP_CURSOR            ((unsigned long) 0x00030000)    /**<RC key define        */
#define KEY_GROUP_SEL_CTRL          ((unsigned long) 0x00040000)    /**<RC key define        */
#define KEY_GROUP_PRG_CTRL          ((unsigned long) 0x00050000)    /**<RC key define        */
#define KEY_GROUP_AUD_CTRL          ((unsigned long) 0x00060000)    /**<RC key define        */
#define KEY_GROUP_TTX_CTRL          ((unsigned long) 0x00070000)    /**<RC key define        */
#define KEY_GROUP_FCT_CTRL          ((unsigned long) 0x00080000)    /**<RC key define        */
#define KEY_GROUP_STRM_CTRL         ((unsigned long) 0x00090000)    /**<RC key define        */
#define KEY_GROUP_DVD_CTRL          ((unsigned long) 0x000a0000)    /**<RC key define        */
#define KEY_GROUP_USER_DEF          ((unsigned long) 0x000b0000)    /**<RC key define        */
#define KEY_GROUP_KEYBOARD          ((unsigned long) 0x000c0000)    /**<RC key define        */
#define KEY_GROUP_IR_MOUSE          ((unsigned long) 0x000d0000)    /**<RC key define        */ 
#define KEY_GROUP_NETWORK           ((unsigned long) 0x000e0000)    /**<RC key define        */ 
#define KEY_GROUP_MAX               ((unsigned long) 0x000f0000)    /**<RC key define        */

/* Combination of key group mask */
#define KEY_GROUP_TO_MASK(_val)     ((UINT64)(1 << (((((unsigned long)_val) >> 16) & 0xFFF) - 1)))    /**<RC key define        */
#define KEY_GROUP_NON_FCT_CTRL      ((UINT64) (KEY_GROUP_TO_MASK(KEY_GROUP_DIGIT)     |   \
                                               KEY_GROUP_TO_MASK(KEY_GROUP_ALPHA)     |   \
                                               KEY_GROUP_TO_MASK(KEY_GROUP_CURSOR)    |   \
                                               KEY_GROUP_TO_MASK(KEY_GROUP_SEL_CTRL)  |   \
                                               KEY_GROUP_TO_MASK(KEY_GROUP_PRG_CTRL)  |   \
                                               KEY_GROUP_TO_MASK(KEY_GROUP_AUD_CTRL)  |   \
                                               KEY_GROUP_TO_MASK(KEY_GROUP_TTX_CTRL)  |   \
                                               KEY_GROUP_TO_MASK(KEY_GROUP_STRM_CTRL) |   \
                                               KEY_GROUP_TO_MASK(KEY_GROUP_DVD_CTRL)  |   \
                                               KEY_GROUP_TO_MASK(KEY_GROUP_KEYBOARD)  |   \
                                               KEY_GROUP_TO_MASK(KEY_GROUP_IR_MOUSE)  |   \
                                               KEY_GROUP_TO_MASK(KEY_GROUP_USER_DEF)  |   \
                                               KEY_GROUP_TO_MASK(KEY_GROUP_NETWORK)))    /**<RC key define        */
                                    
#define KEY_GROUP_ALL               ((UINT64) (KEY_GROUP_TO_MASK(KEY_GROUP_FCT_CTRL)  |   \
                                               KEY_GROUP_NON_FCT_CTRL))    /**<RC key define        */
                                    
/* Definiton of key ID */           
#define BTN_DIGIT_0                 ((unsigned long)(KEY_GROUP_DIGIT | ((unsigned long) '0')))    /**<RC key define        */
#define BTN_DIGIT_1                 ((unsigned long)(KEY_GROUP_DIGIT | ((unsigned long) '1')))    /**<RC key define        */
#define BTN_DIGIT_2                 ((unsigned long)(KEY_GROUP_DIGIT | ((unsigned long) '2')))    /**<RC key define        */
#define BTN_DIGIT_3                 ((unsigned long)(KEY_GROUP_DIGIT | ((unsigned long) '3')))    /**<RC key define        */
#define BTN_DIGIT_4                 ((unsigned long)(KEY_GROUP_DIGIT | ((unsigned long) '4')))    /**<RC key define        */
#define BTN_DIGIT_5                 ((unsigned long)(KEY_GROUP_DIGIT | ((unsigned long) '5')))    /**<RC key define        */
#define BTN_DIGIT_6                 ((unsigned long)(KEY_GROUP_DIGIT | ((unsigned long) '6')))    /**<RC key define        */
#define BTN_DIGIT_7                 ((unsigned long)(KEY_GROUP_DIGIT | ((unsigned long) '7')))    /**<RC key define        */
#define BTN_DIGIT_8                 ((unsigned long)(KEY_GROUP_DIGIT | ((unsigned long) '8')))    /**<RC key define        */
#define BTN_DIGIT_9                 ((unsigned long)(KEY_GROUP_DIGIT | ((unsigned long) '9')))    /**<RC key define        */
#define BTN_DIGIT_PLUS_5            ((unsigned long)(KEY_GROUP_DIGIT | 0x0000f000))    /**<RC key define        */
#define BTN_DIGIT_PLUS_10           ((unsigned long)(KEY_GROUP_DIGIT | 0x0000f001))    /**<RC key define        */
#define BTN_DIGIT_PLUS_20           ((unsigned long)(KEY_GROUP_DIGIT | 0x0000f002))    /**<RC key define       */
#define BTN_DIGIT_PLUS_100          ((unsigned long)(KEY_GROUP_DIGIT | 0x0000f003))    /**<RC key define        */
#define BTN_DIGIT_DOT               ((unsigned long)(KEY_GROUP_DIGIT | 0x0000f004))    /**<RC key define        */
                                    
#define BTN_CURSOR_LEFT             ((unsigned long)(KEY_GROUP_CURSOR | 0x0000f000))    /**<RC key define        */
#define BTN_CURSOR_RIGHT            ((unsigned long)(KEY_GROUP_CURSOR | 0x0000f001))    /**<RC key define        */
#define BTN_CURSOR_UP               ((unsigned long)(KEY_GROUP_CURSOR | 0x0000f002))    /**<RC key define        */
#define BTN_CURSOR_DOWN             ((unsigned long)(KEY_GROUP_CURSOR | 0x0000f003))    /**<RC key define        */

#ifdef  BTN_SELECT                  
#undef BTN_SELECT
#endif

#define BTN_SELECT                  ((unsigned long)(KEY_GROUP_SEL_CTRL | 0x0000f000))    /**<RC key define        */
#define BTN_EXIT                    ((unsigned long)(KEY_GROUP_SEL_CTRL | 0x0000f001))    /**<RC key define        */
#define BTN_CLEAR                   ((unsigned long)(KEY_GROUP_SEL_CTRL | 0x0000f002))    /**<RC key define        */
#define BTN_ENTER                   ((unsigned long)(KEY_GROUP_SEL_CTRL | 0x0000f003))    /**<RC key define        */
#define BTN_RETURN                  ((unsigned long)(KEY_GROUP_SEL_CTRL | 0x0000f004))    /**<RC key define        */
                                    
#define BTN_PRG_UP                  ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f000))    /**<RC key define        */
#define BTN_PRG_DOWN                ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f001))    /**<RC key define        */
#define BTN_PREV_PRG                ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f002))    /**<RC key define        */
#define BTN_CH_LIST                 ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f003))    /**<RC key define        */
#define BTN_FAV_CH                  ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f004))    /**<RC key define        */
#define BTN_FAV_CH_UP               ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f005))    /**<RC key define        */
#define BTN_FAV_CH_DOWN             ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f006))    /**<RC key define        */
#define BTN_FAVORITE                ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f007))    /**<RC key define        */
#define BTN_PIP                     ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f008))    /**<RC key define        */
#define BTN_PIP_CH_UP               ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f009))    /**<RC key define        */
#define BTN_PIP_CH_DOWN             ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f00a))    /**<RC key define        */
#define BTN_PIP_SIZE                ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f00b))    /**<RC key define        */
#define BTN_PIP_POS                 ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f00c))    /**<RC key define        */
#define BTN_PIP_INPUT_SRC           ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f00d))    /**<RC key define        */
#define BTN_POP                     ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f00e))    /**<RC key define        */
#define BTN_PIP_POP                 ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f00f))    /**<RC key define        */
#define BTN_SWAP                    ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f010))    /**<RC key define        */
#define BTN_FREEZE                  ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f011))    /**<RC key define        */
#define BTN_ZOOM                    ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f012))    /**<RC key define        */
#define BTN_ZOOM_IN                 ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f013))    /**<RC key define        */
#define BTN_ZOOM_OUT                ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f014))    /**<RC key define        */
#define BTN_ASPECT                  ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f015))    /**<RC key define        */
#define BTN_P_EFFECT                ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f016))    /**<RC key define        */
#define BTN_PRG_INFO                ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f017))    /**<RC key define        */
#define BTN_BANNER                  ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f018))    /**<RC key define        */
#define BTN_PRG_DETAIL              ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f019))    /**<RC key define        */
#define BTN_CC                      ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f01a))    /**<RC key define        */
#define BTN_ADD_ERASE               ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f01b))    /**<RC key define        */
#define BTN_SLEEP                   ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f01c))    /**<RC key define        */
#define BTN_TIMER                   ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f01d))    /**<RC key define        */
#define BTN_AUTO_SYNC               ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f01e))    /**<RC key define        */
#define BTN_INPUT_SRC               ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f01f))    /**<RC key define        */
#define BTN_TV                      ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f020))    /**<RC key define        */
#define BTN_TV_ANA                  ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f021))    /**<RC key define        */
#define BTN_TV_DIG                  ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f022))    /**<RC key define        */
#define BTN_TUNER_TER_ANA           ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f023))    /**<RC key define        */
#define BTN_TUNER_CAB_ANA           ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f024))    /**<RC key define        */
#define BTN_TUNER_SAT_ANA           ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f025))    /**<RC key define        */
#define BTN_TUNER_TER_DIG           ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f026))    /**<RC key define        */
#define BTN_TUNER_CAB_DIG           ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f027))    /**<RC key define        */
#define BTN_TUNER_SAT_DIG           ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f028))    /**<RC key define        */
#define BTN_COMPOSITE               ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f029))    /**<RC key define        */
#define BTN_COMPOSITE_1             ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f02a))    /**<RC key define        */
#define BTN_COMPOSITE_2             ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f02b))    /**<RC key define        */
#define BTN_COMPOSITE_3             ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f02c))    /**<RC key define        */
#define BTN_COMPOSITE_4             ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f02d))    /**<RC key define        */
#define BTN_S_VIDEO                 ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f02e))    /**<RC key define        */
#define BTN_S_VIDEO_1               ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f02f))    /**<RC key define        */
#define BTN_S_VIDEO_2               ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f030))    /**<RC key define        */
#define BTN_S_VIDEO_3               ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f031))    /**<RC key define        */
#define BTN_S_VIDEO_4               ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f032))    /**<RC key define        */
#define BTN_COMPONENT               ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f033))    /**<RC key define        */
#define BTN_COMPONENT_1             ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f034))    /**<RC key define        */
#define BTN_COMPONENT_2             ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f035))    /**<RC key define        */
#define BTN_COMPONENT_3             ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f036))    /**<RC key define        */
#define BTN_COMPONENT_4             ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f037))    /**<RC key define        */
#define BTN_VGA                     ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f038))    /**<RC key define        */
#define BTN_DVI                     ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f039))    /**<RC key define        */
#define BTN_DVI_1                   ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f03a))    /**<RC key define        */
#define BTN_DVI_2                   ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f03b))    /**<RC key define        */
#define BTN_DVI_3                   ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f03c))    /**<RC key define        */
#define BTN_DVI_4                   ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f03d))    /**<RC key define        */
#define BTN_HDMI                    ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f03e))    /**<RC key define        */
#define BTN_HDMI_1                  ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f03f))    /**<RC key define        */
#define BTN_HDMI_2                  ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f040))    /**<RC key define        */
#define BTN_HDMI_3                  ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f041))    /**<RC key define        */
#define BTN_HDMI_4                  ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f042))    /**<RC key define        */
#define BTN_HDMI_5                  ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f043))    /**<RC key define        */
#define BTN_SCART                   ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f044))    /**<RC key define        */
#define BTN_SCART_1                 ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f045))    /**<RC key define        */
#define BTN_SCART_2                 ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f046))    /**<RC key define        */
#define BTN_SCART_3                 ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f047))    /**<RC key define        */
#define BTN_SCART_4                 ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f048))    /**<RC key define        */
#define BTN_DVD                     ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f049))    /**<RC key define        */
#define BTN_ILINK                   ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f04a))    /**<RC key define        */
#define BTN_POWER_SAVING            ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f04b))    /**<RC key define        */
#define BTN_DTV_RADIO_ATV           ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f04c))    /**<RC key define        */
#define BTN_INP_SRC_1               ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f04d))    /**<RC key define        */
#define BTN_INP_SRC_2               ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f04e))    /**<RC key define        */
#define BTN_INP_SRC_3               ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f04f))    /**<RC key define        */
#define BTN_INP_SRC_4               ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f050))    /**<RC key define        */
#define BTN_INP_SRC_5               ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f051))    /**<RC key define        */
#define BTN_INP_SRC_6               ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f052))    /**<RC key define        */
#define BTN_INP_SRC_7               ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f053))    /**<RC key define        */
#define BTN_INP_SRC_8               ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f054))    /**<RC key define       */
#define BTN_INP_SRC_9               ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f055))    /**<RC key define        */
#define BTN_INP_SRC_10              ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f056))    /**<RC key define        */
#define BTN_INP_SRC_11              ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f057))    /**<RC key define        */
#define BTN_INP_SRC_12              ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f058))    /**<RC key define        */
#define BTN_INP_SRC_13              ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f059))    /**<RC key define        */
#define BTN_INP_SRC_14              ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f05a))    /**<RC key define        */
#define BTN_INP_SRC_15              ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f05b))    /**<RC key define        */
#define BTN_3D                      ((unsigned long)(KEY_GROUP_PRG_CTRL | 0x0000f05c))    /**<RC key define        */
                                    
#define BTN_VOL_UP                  ((unsigned long)(KEY_GROUP_AUD_CTRL | 0x0000f000))    /**<RC key define        */
#define BTN_VOL_DOWN                ((unsigned long)(KEY_GROUP_AUD_CTRL | 0x0000f001))    /**<RC key define        */
#define BTN_MUTE                    ((unsigned long)(KEY_GROUP_AUD_CTRL | 0x0000f002))    /**<RC key define        */
#define BTN_MTS                     ((unsigned long)(KEY_GROUP_AUD_CTRL | 0x0000f003))    /**<RC key define        */
#define BTN_AUDIO                   ((unsigned long)(KEY_GROUP_AUD_CTRL | 0x0000f004))    /**<RC key define        */
#define BTN_MTS_AUDIO               ((unsigned long)(KEY_GROUP_AUD_CTRL | 0x0000f005))    /**<RC key define        */
#define BTN_PIP_AUDIO               ((unsigned long)(KEY_GROUP_AUD_CTRL | 0x0000f006))    /**<RC key define        */
#define BTN_S_EFFECT                ((unsigned long)(KEY_GROUP_AUD_CTRL | 0x0000f007))    /**<RC key define        */
#define BTN_TOOLS                   ((unsigned long)(KEY_GROUP_AUD_CTRL | 0x0000f008))    /**<RC key define        */
#define BTN_DEV_INFO                ((unsigned long)(KEY_GROUP_AUD_CTRL | 0x0000f009))    /**<RC key define        */

                                    
#define BTN_TTX                     ((unsigned long)(KEY_GROUP_TTX_CTRL | 0x0000f000))    /**<RC key define        */
#define BTN_INDEX                   ((unsigned long)(KEY_GROUP_TTX_CTRL | 0x0000f001))    /**<RC key define        */
#define BTN_REVEAL                  ((unsigned long)(KEY_GROUP_TTX_CTRL | 0x0000f002))    /**<RC key define        */
#define BTN_MIX                     ((unsigned long)(KEY_GROUP_TTX_CTRL | 0x0000f003))    /**<RC key define        */
#define BTN_SUBPAGE                 ((unsigned long)(KEY_GROUP_TTX_CTRL | 0x0000f004))    /**<RC key define        */
#define BTN_SIZE                    ((unsigned long)(KEY_GROUP_TTX_CTRL | 0x0000f005))    /**<RC key define        */
#define BTN_WIDE_MODE               ((unsigned long)(KEY_GROUP_TTX_CTRL | 0x0000f006))    /**<RC key define        */
                                    
#define BTN_POWER                   ((unsigned long)(KEY_GROUP_FCT_CTRL | 0x0000f000))    /**<RC key define        */
#define BTN_MENU                    ((unsigned long)(KEY_GROUP_FCT_CTRL | 0x0000f001))    /**<RC key define        */
#define BTN_EPG                     ((unsigned long)(KEY_GROUP_FCT_CTRL | 0x0000f002))    /**<RC key define        */
#define BTN_MEM_CARD                ((unsigned long)(KEY_GROUP_FCT_CTRL | 0x0000f003))    /**<RC key define        */
#define BTN_TEXT                    ((unsigned long)(KEY_GROUP_FCT_CTRL | 0x0000f004))    /**<RC key define        */
#define BTN_POWER_ON                ((unsigned long)(KEY_GROUP_FCT_CTRL | 0x0000f005))    /**<RC key define        */
#define BTN_POWER_OFF               ((unsigned long)(KEY_GROUP_FCT_CTRL | 0x0000f006))    /**<RC key define        */
                                    
#define BTN_PLAY                    ((unsigned long)(KEY_GROUP_STRM_CTRL | 0x0000f000))    /**<RC key define        */
#define BTN_PAUSE                   ((unsigned long)(KEY_GROUP_STRM_CTRL | 0x0000f001))    /**<RC key define        */
#define BTN_STOP                    ((unsigned long)(KEY_GROUP_STRM_CTRL | 0x0000f002))    /**<RC key define        */
#define BTN_RESUME                  ((unsigned long)(KEY_GROUP_STRM_CTRL | 0x0000f003))    /**<RC key define        */
#define BTN_RECORD                  ((unsigned long)(KEY_GROUP_STRM_CTRL | 0x0000f004))    /**<RC key define        */
#define BTN_NEXT                    ((unsigned long)(KEY_GROUP_STRM_CTRL | 0x0000f005))    /**<RC key define        */
#define BTN_PREV                    ((unsigned long)(KEY_GROUP_STRM_CTRL | 0x0000f006))    /**<RC key define        */
#define BTN_FF                      ((unsigned long)(KEY_GROUP_STRM_CTRL | 0x0000f007))    /**<RC key define        */
#define BTN_FR                      ((unsigned long)(KEY_GROUP_STRM_CTRL | 0x0000f008))    /**<RC key define        */
#define BTN_SF                      ((unsigned long)(KEY_GROUP_STRM_CTRL | 0x0000f009))    /**<RC key define        */
#define BTN_SR                      ((unsigned long)(KEY_GROUP_STRM_CTRL | 0x0000f00a))    /**<RC key define        */
#define BTN_STEPF                   ((unsigned long)(KEY_GROUP_STRM_CTRL | 0x0000f00b))    /**<RC key define        */
#define BTN_STEPR                   ((unsigned long)(KEY_GROUP_STRM_CTRL | 0x0000f00c))    /**<RC key define        */
#define BTN_PLAY_PAUSE              ((unsigned long)(KEY_GROUP_STRM_CTRL | 0x0000f00d))    /**<RC key define        */
#define BTN_PAUSE_STEP              ((unsigned long)(KEY_GROUP_STRM_CTRL | 0x0000f00e))    /**<RC key define        */
#define BTN_STOP_RESUME             ((unsigned long)(KEY_GROUP_STRM_CTRL | 0x0000f00f))    /**<RC key define        */
#define BTN_PLAY_ENTER              ((unsigned long)(KEY_GROUP_STRM_CTRL | 0x0000f010))    /**<RC key define        */
#define BTN_CEC_MENU                ((unsigned long)(KEY_GROUP_STRM_CTRL | 0x0000f011))    /**<RC key define        */
#define BTN_TIMESHIFT               ((unsigned long)(KEY_GROUP_STRM_CTRL | 0x0000f012))    /**<RC key define        */
                                    
#define BTN_EJECT                   ((unsigned long)(KEY_GROUP_DVD_CTRL | 0x0000f000))    /**<RC key define        */
#define BTN_TITLE_MENU              ((unsigned long)(KEY_GROUP_DVD_CTRL | 0x0000f001))    /**<RC key define        */
#define BTN_ROOT_MENU               ((unsigned long)(KEY_GROUP_DVD_CTRL | 0x0000f002))    /**<RC key define        */
#define BTN_GOTO                    ((unsigned long)(KEY_GROUP_DVD_CTRL | 0x0000f003))    /**<RC key define        */
#define BTN_MARK                    ((unsigned long)(KEY_GROUP_DVD_CTRL | 0x0000f004))    /**<RC key define        */
#define BTN_DIGEST                  ((unsigned long)(KEY_GROUP_DVD_CTRL | 0x0000f005))    /**<RC key define        */
#define BTN_PROGRAM                 ((unsigned long)(KEY_GROUP_DVD_CTRL | 0x0000f006))    /**<RC key define        */
#define BTN_VRMT                    ((unsigned long)(KEY_GROUP_DVD_CTRL | 0x0000f007))    /**<RC key define        */
#define BTN_PBC                     ((unsigned long)(KEY_GROUP_DVD_CTRL | 0x0000f008))    /**<RC key define        */
#define BTN_REPEAT                  ((unsigned long)(KEY_GROUP_DVD_CTRL | 0x0000f009))    /**<RC key define        */
#define BTN_REPEAT_A                ((unsigned long)(KEY_GROUP_DVD_CTRL | 0x0000f00a))    /**<RC key define        */
#define BTN_REPEAT_B                ((unsigned long)(KEY_GROUP_DVD_CTRL | 0x0000f00b))    /**<RC key define        */
#define BTN_REPEAT_A_B              ((unsigned long)(KEY_GROUP_DVD_CTRL | 0x0000f00c))    /**<RC key define        */
#define BTN_SUB_TITLE               ((unsigned long)(KEY_GROUP_DVD_CTRL | 0x0000f00d))    /**<RC key define        */
#define BTN_ANGLE                   ((unsigned long)(KEY_GROUP_DVD_CTRL | 0x0000f00e))    /**<RC key define        */
#define BTN_DISPLAY                 ((unsigned long)(KEY_GROUP_DVD_CTRL | 0x0000f00f))    /**<RC key define        */
#define BTN_RANDOM                  ((unsigned long)(KEY_GROUP_DVD_CTRL | 0x0000f010))    /**<RC key define        */
#define BTN_PAL_NTSC                ((unsigned long)(KEY_GROUP_DVD_CTRL | 0x0000f011))    /**<RC key define        */
#define BTN_SURROUND                ((unsigned long)(KEY_GROUP_DVD_CTRL | 0x0000f012))    /**<RC key define       */
#define BTN_EQUALIZER               ((unsigned long)(KEY_GROUP_DVD_CTRL | 0x0000f013))    /**<RC key define        */
#define BTN_HOME_DLIST              ((unsigned long)(KEY_GROUP_DVD_CTRL | 0x0000f014))    /**<RC key define        */
#define BTN_NEXT_DLIST              ((unsigned long)(KEY_GROUP_DVD_CTRL | 0x0000f015))    /**<RC key define        */
#define BTN_PREV_DLIST              ((unsigned long)(KEY_GROUP_DVD_CTRL | 0x0000f016))    /**<RC key define        */
#define BTN_KARAOKE                 ((unsigned long)(KEY_GROUP_DVD_CTRL | 0x0000f017))    /**<RC key define        */
#define BTN_AUD_KEY_PLUS            ((unsigned long)(KEY_GROUP_DVD_CTRL | 0x0000f018))    /**<RC key define        */
#define BTN_AUD_KEY_MINUS           ((unsigned long)(KEY_GROUP_DVD_CTRL | 0x0000f019))    /**<RC key define        */
#define BTN_AUD_KEY_RESUME          ((unsigned long)(KEY_GROUP_DVD_CTRL | 0x0000f01a))    /**<RC key define        */
#define BTN_ECHO_PLUS               ((unsigned long)(KEY_GROUP_DVD_CTRL | 0x0000f01b))    /**<RC key define        */
#define BTN_ECHO_MINUS              ((unsigned long)(KEY_GROUP_DVD_CTRL | 0x0000f01c))    /**<RC key define        */
#define BTN_VOCAL_ASSIST            ((unsigned long)(KEY_GROUP_DVD_CTRL | 0x0000f01d))    /**<RC key define        */
#define BTN_MENU_PBC                ((unsigned long)(KEY_GROUP_DVD_CTRL | 0x0000f01e))    /**<RC key define       */
#define BTN_TITLE_PBC               ((unsigned long)(KEY_GROUP_DVD_CTRL | 0x0000f01f))    /**<RC key define        */
                                    
#define BTN_RED                     ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f000))    /**<RC key define        */
#define BTN_GREEN                   ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f001))    /**<RC key define        */
#define BTN_YELLOW                  ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f002))    /**<RC key define        */
#define BTN_BLUE                    ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f003))    /**<RC key define        */
#define BTN_FACTORY_MODE_1          ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f004))    /**<RC key define        */
#define BTN_FACTORY_MODE_2          ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f005))    /**<RC key define        */
#define BTN_FACTORY_MODE_3          ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f006))    /**<RC key define        */
#define BTN_FACTORY_MODE_4          ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f007))    /**<RC key define        */
#define BTN_FACTORY_MODE_5          ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f008))    /**<RC key define        */
#define BTN_FACTORY_MODE_6          ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f009))    /**<RC key define        */
#define BTN_FACTORY_MODE_7          ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f00a))    /**<RC key define        */
#define BTN_FACTORY_MODE_8          ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f00b))    /**<RC key define        */
#define BTN_FUNCTION_1              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f00c))    /**<RC key define        */
#define BTN_FUNCTION_2              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f00d))    /**<RC key define        */
#define BTN_FUNCTION_3              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f00e))    /**<RC key define        */
#define BTN_FUNCTION_4              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f00f))    /**<RC key define        */
#define BTN_FUNCTION_5              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f010))    /**<RC key define        */
#define BTN_FUNCTION_6              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f011))    /**<RC key define        */
#define BTN_FUNCTION_7              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f012))    /**<RC key define        */
#define BTN_FUNCTION_8              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f013))    /**<RC key define        */
#define BTN_FUNCTION_9              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f014))    /**<RC key define        */
#define BTN_FUNCTION_10             ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f015))    /**<RC key define        */
#define BTN_FUNCTION_11             ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f016))    /**<RC key define        */
#define BTN_FUNCTION_12             ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f017))    /**<RC key define        */
#define BTN_CUSTOM_1                ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f018))    /**<RC key define        */
#define BTN_CUSTOM_2                ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f019))    /**<RC key define        */
#define BTN_CUSTOM_3                ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f01a))    /**<RC key define        */
#define BTN_CUSTOM_4                ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f01b))    /**<RC key define        */
#define BTN_CUSTOM_5                ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f01c))    /**<RC key define        */
#define BTN_CUSTOM_6                ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f01d))    /**<RC key define        */
#define BTN_CUSTOM_7                ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f01e))    /**<RC key define        */
#define BTN_CUSTOM_8                ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f01f))    /**<RC key define        */
#define BTN_CUSTOM_9                ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f020))    /**<RC key define       */
#define BTN_CUSTOM_10               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f021))    /**<RC key define        */
#define BTN_CUSTOM_11               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f022))    /**<RC key define        */
#define BTN_CUSTOM_12               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f023))    /**<RC key define        */
#define BTN_CUSTOM_13               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f024))    /**<RC key define        */
#define BTN_CUSTOM_14               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f025))    /**<RC key define        */
#define BTN_CUSTOM_15               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f026))    /**<RC key define        */
#define BTN_CUSTOM_16               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f027))    /**<RC key define        */
#define BTN_CUSTOM_17               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f028))    /**<RC key define        */
#define BTN_CUSTOM_18               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f029))    /**<RC key define        */
#define BTN_CUSTOM_19               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f03a))    /**<RC key define        */
#define BTN_CUSTOM_20               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f03b))    /**<RC key define        */
#define BTN_CUSTOM_21               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f03c))    /**<RC key define        */
#define BTN_CUSTOM_22               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f03d))    /**<RC key define        */
#define BTN_CUSTOM_23               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f03e))    /**<RC key define        */
#define BTN_CUSTOM_24               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f03f))    /**<RC key define        */
#define BTN_CUSTOM_25               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f040))    /**<RC key define        */
#define BTN_CUSTOM_26               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f041))    /**<RC key define        */
#define BTN_CUSTOM_27               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f042))    /**<RC key define        */
#define BTN_CUSTOM_28               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f043))    /**<RC key define        */
#define BTN_CUSTOM_29               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f044))    /**<RC key define        */
#define BTN_CUSTOM_30               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f045))    /**<RC key define        */
#define BTN_CUSTOM_31               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f046))    /**<RC key define        */
#define BTN_CUSTOM_32               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f047))    /**<RC key define        */
#define BTN_CUSTOM_33               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f048))    /**<RC key define        */
#define BTN_CUSTOM_34               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f049))    /**<RC key define        */
#define BTN_CUSTOM_35               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f04a))    /**<RC key define        */
#define BTN_CUSTOM_36               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f04b))    /**<RC key define        */
#define BTN_CUSTOM_37               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f04c))    /**<RC key define        */
#define BTN_CUSTOM_38               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f04d))    /**<RC key define        */
#define BTN_CUSTOM_39               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f04e))    /**<RC key define        */
#define BTN_CUSTOM_40               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f04f))    /**<RC key define        */
#define BTN_CUSTOM_41               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f050))    /**<RC key define        */
#define BTN_CUSTOM_42               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f051))    /**<RC key define        */
#define BTN_CUSTOM_43               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f052))    /**<RC key define        */
#define BTN_CUSTOM_44               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f053))    /**<RC key define        */
#define BTN_CUSTOM_45               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f054))    /**<RC key define        */
#define BTN_CUSTOM_46               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f055))    /**<RC key define        */
#define BTN_CUSTOM_47               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f056))    /**<RC key define        */
#define BTN_CUSTOM_48               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f057))    /**<RC key define        */
#define BTN_CUSTOM_49               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f058))    /**<RC key define        */
#define BTN_CUSTOM_50               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f059))    /**<RC key define        */
#define BTN_CUSTOM_51               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f05a))    /**<RC key define        */
#define BTN_CUSTOM_52               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f05b))    /**<RC key define        */
#define BTN_CUSTOM_53               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f05c))    /**<RC key define        */
#define BTN_CUSTOM_54               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f05d))    /**<RC key define        */
#define BTN_CUSTOM_55               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f05e))    /**<RC key define        */
#define BTN_CUSTOM_56               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f05f))    /**<RC key define        */
#define BTN_CUSTOM_57               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f060))    /**<RC key define        */
#define BTN_CUSTOM_58               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f061))    /**<RC key define        */
#define BTN_CUSTOM_59               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f062))    /**<RC key define        */
#define BTN_CUSTOM_60               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f063))    /**<RC key define        */
#define BTN_CUSTOM_61               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f064))    /**<RC key define        */
#define BTN_CUSTOM_62               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f065))    /**<RC key define        */
#define BTN_CUSTOM_63               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f066))    /**<RC key define        */
#define BTN_CUSTOM_64               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f067))    /**<RC key define        */
#define BTN_CUSTOM_65               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f068))    /**<RC key define        */
#define BTN_CUSTOM_66               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f069))    /**<RC key define        */
#define BTN_CUSTOM_67               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f06a))    /**<RC key define        */
#define BTN_CUSTOM_68               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f06b))    /**<RC key define        */
#define BTN_CUSTOM_69               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f06c))    /**<RC key define        */
#define BTN_CUSTOM_70               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f06d))    /**<RC key define        */
#define BTN_CUSTOM_71               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f06f))    /**<RC key define        */
#define BTN_CUSTOM_72               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f070))    /**<RC key define        */
#define BTN_CUSTOM_73               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f071))    /**<RC key define        */
#define BTN_CUSTOM_74               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f072))    /**<RC key define        */
#define BTN_CUSTOM_75               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f073))    /**<RC key define        */
#define BTN_CUSTOM_76               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f074))    /**<RC key define        */
#define BTN_CUSTOM_77               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f075))    /**<RC key define        */
#define BTN_CUSTOM_78               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f076))    /**<RC key define        */
#define BTN_CUSTOM_79               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f077))    /**<RC key define        */
#define BTN_CUSTOM_80               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f078))    /**<RC key define        */
#define BTN_CUSTOM_81               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f079))    /**<RC key define        */
#define BTN_CUSTOM_82               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f07a))    /**<RC key define        */
#define BTN_CUSTOM_83               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f07b))    /**<RC key define        */
#define BTN_CUSTOM_84               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f07c))    /**<RC key define        */
#define BTN_CUSTOM_85               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f07d))    /**<RC key define        */
#define BTN_CUSTOM_86               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f07e))    /**<RC key define        */
#define BTN_CUSTOM_87               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f07f))    /**<RC key define        */
#define BTN_CUSTOM_88               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f080))    /**<RC key define        */
#define BTN_CUSTOM_89               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f081))    /**<RC key define        */
#define BTN_CUSTOM_90               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f082))    /**<RC key define        */
#define BTN_CUSTOM_91               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f083))    /**<RC key define        */
#define BTN_CUSTOM_92               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f084))    /**<RC key define        */
#define BTN_CUSTOM_93               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f085))    /**<RC key define        */
#define BTN_CUSTOM_94               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f086))    /**<RC key define        */
#define BTN_CUSTOM_95               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f087))    /**<RC key define        */
#define BTN_CUSTOM_96               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f088))    /**<RC key define        */
#define BTN_CUSTOM_97               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f089))    /**<RC key define        */
#define BTN_CUSTOM_98               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f08a))    /**<RC key define        */
#define BTN_CUSTOM_99               ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f08b))    /**<RC key define        */
#define BTN_CUSTOM_100              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f08c))    /**<RC key define        */
#define BTN_CUSTOM_101              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f08d))    /**<RC key define        */
#define BTN_CUSTOM_102              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f08e))    /**<RC key define        */
#define BTN_CUSTOM_103              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f08f))    /**<RC key define        */
#define BTN_CUSTOM_104              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f090))    /**<RC key define        */
#define BTN_CUSTOM_105              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f091))    /**<RC key define        */
#define BTN_CUSTOM_106              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f092))    /**<RC key define        */
#define BTN_CUSTOM_107              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f093))    /**<RC key define        */
#define BTN_CUSTOM_108              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f094))    /**<RC key define        */
#define BTN_CUSTOM_109              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f095))    /**<RC key define        */
#define BTN_CUSTOM_110              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f096))    /**<RC key define        */
#define BTN_CUSTOM_111              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f097))    /**<RC key define        */
#define BTN_CUSTOM_112              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f098))    /**<RC key define        */
#define BTN_CUSTOM_113              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f099))    /**<RC key define        */
#define BTN_CUSTOM_114              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f09a))    /**<RC key define        */
#define BTN_CUSTOM_115              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f09b))    /**<RC key define        */
#define BTN_CUSTOM_116              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f09c))    /**<RC key define        */
#define BTN_CUSTOM_117              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f09d))    /**<RC key define        */
#define BTN_CUSTOM_118              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f09e))    /**<RC key define        */
#define BTN_CUSTOM_119              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f09f))    /**<RC key define        */
#define BTN_CUSTOM_120              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f100))    /**<RC key define        */
#define BTN_CUSTOM_121              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f101))    /**<RC key define        */
#define BTN_CUSTOM_122              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f102))    /**<RC key define        */
#define BTN_CUSTOM_123              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f103))    /**<RC key define        */
#define BTN_CUSTOM_124              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f104))    /**<RC key define        */
#define BTN_CUSTOM_125              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f105))    /**<RC key define        */
#define BTN_CUSTOM_126              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f106))    /**<RC key define        */
#define BTN_CUSTOM_127              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f107))    /**<RC key define        */
#define BTN_CUSTOM_128              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f108))    /**<RC key define        */
#define BTN_CUSTOM_129              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f109))    /**<RC key define        */
#define BTN_CUSTOM_130              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f10a))    /**<RC key define        */
#define BTN_CUSTOM_131              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f10b))    /**<RC key define        */
#define BTN_CUSTOM_132              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f10c))    /**<RC key define        */
#define BTN_CUSTOM_133              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f10d))    /**<RC key define        */
#define BTN_CUSTOM_134              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f10e))    /**<RC key define        */
#define BTN_CUSTOM_135              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f10f))    /**<RC key define        */
#define BTN_CUSTOM_136              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f110))    /**<RC key define        */
#define BTN_CUSTOM_137              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f111))    /**<RC key define        */
#define BTN_CUSTOM_138              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f112))    /**<RC key define        */
#define BTN_CUSTOM_139              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f113))    /**<RC key define        */
#define BTN_CUSTOM_140              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f114))    /**<RC key define        */
#define BTN_CUSTOM_141              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f115))    /**<RC key define        */
#define BTN_CUSTOM_142              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f116))    /**<RC key define        */
#define BTN_CUSTOM_143              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f117))    /**<RC key define        */
#define BTN_CUSTOM_144              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f118))    /**<RC key define        */
#define BTN_CUSTOM_145              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f119))    /**<RC key define        */
#define BTN_CUSTOM_146              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f11a))    /**<RC key define        */
#define BTN_CUSTOM_147              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f11b))    /**<RC key define        */
#define BTN_CUSTOM_148              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f11c))    /**<RC key define        */
#define BTN_CUSTOM_149              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f11d))    /**<RC key define        */
#define BTN_CUSTOM_150              ((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f11e))    /**<RC key define        */
#define BTN_FACTORY_FRNT_PANEL  	((unsigned long)(KEY_GROUP_USER_DEF | 0x0000f11f))     /**<RC key define        */
#define BTN_INVALID                 ((unsigned long)(KEY_GROUP_MAX | 0x0000f000))    /**<RC key define        */

#define BTN_WIDGET                  ((unsigned long)(KEY_GROUP_NETWORK | 0x0000f000))    /**<RC key define        */

/* Definiton of key ID for keyboard events */
#define BTN_KB_NULL                 ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000000))    /**<RC key define        */
#define BTN_KB_BACKSPACE            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000008))    /**<RC key define        */
#define BTN_KB_TAB                  ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000009))    /**<RC key define        */
#define BTN_KB_RETURN               ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000000D))    /**<RC key define        */
#define BTN_KB_CANCEL               ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000018))    /**<RC key define        */
#define BTN_KB_ESCAPE               ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000001B))    /**<RC key define        */
#define BTN_KB_SPACE                ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000020))    /**<RC key define        */
#define BTN_KB_EXCLAMATION_MARK     ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000021))    /**<RC key define        */
#define BTN_KB_QUOTATION            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000022))    /**<RC key define        */
#define BTN_KB_NUMBER_SIGN          ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000023))    /**<RC key define        */
#define BTN_KB_DOLLAR_SIGN          ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000024))    /**<RC key define        */
#define BTN_KB_PERCENT_SIGN         ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000025))    /**<RC key define        */
#define BTN_KB_AMPERSAND            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000026))    /**<RC key define        */
#define BTN_KB_APOSTROPHE           ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000027))    /**<RC key define        */
#define BTN_KB_PARENTHESIS_LEFT     ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000028))    /**<RC key define        */
#define BTN_KB_PARENTHESIS_RIGHT    ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000029))    /**<RC key define        */
#define BTN_KB_ASTERISK             ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000002A))    /**<RC key define        */
#define BTN_KB_PLUS_SIGN            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000002B))    /**<RC key define        */
#define BTN_KB_COMMA                ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000002C))    /**<RC key define        */
#define BTN_KB_MINUS_SIGN           ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000002D))    /**<RC key define        */
#define BTN_KB_PERIOD               ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000002E))    /**<RC key define        */
#define BTN_KB_SLASH                ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000002F))    /**<RC key define        */
#define BTN_KB_0                    ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000030))    /**<RC key define        */
#define BTN_KB_1                    ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000031))    /**<RC key define        */
#define BTN_KB_2                    ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000032))    /**<RC key define        */
#define BTN_KB_3                    ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000033))    /**<RC key define        */
#define BTN_KB_4                    ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000034))    /**<RC key define        */
#define BTN_KB_5                    ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000035))    /**<RC key define        */
#define BTN_KB_6                    ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000036))    /**<RC key define        */
#define BTN_KB_7                    ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000037))    /**<RC key define        */
#define BTN_KB_8                    ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000038))    /**<RC key define        */
#define BTN_KB_9                    ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000039))    /**<RC key define        */
#define BTN_KB_COLON                ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000003A))    /**<RC key define        */
#define BTN_KB_SEMICOLON            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000003B))    /**<RC key define        */
#define BTN_KB_LESS_THAN_SIGN       ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000003C))    /**<RC key define        */
#define BTN_KB_EQUALS_SIGN          ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000003D))    /**<RC key define        */
#define BTN_KB_GREATER_THAN_SIGN    ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000003E))    /**<RC key define        */
#define BTN_KB_QUESTION_MARK        ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000003F))    /**<RC key define        */
#define BTN_KB_AT                   ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000040))    /**<RC key define        */
#define BTN_KB_CAPITAL_A            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000041))    /**<RC key define        */
#define BTN_KB_CAPITAL_B            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000042))    /**<RC key define        */
#define BTN_KB_CAPITAL_C            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000043))    /**<RC key define        */
#define BTN_KB_CAPITAL_D            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000044))    /**<RC key define        */
#define BTN_KB_CAPITAL_E            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000045))    /**<RC key define        */
#define BTN_KB_CAPITAL_F            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000046))    /**<RC key define        */
#define BTN_KB_CAPITAL_G            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000047))    /**<RC key define        */
#define BTN_KB_CAPITAL_H            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000048))    /**<RC key define        */
#define BTN_KB_CAPITAL_I            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000049))    /**<RC key define        */
#define BTN_KB_CAPITAL_J            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000004A))    /**<RC key define        */
#define BTN_KB_CAPITAL_K            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000004B))    /**<RC key define        */
#define BTN_KB_CAPITAL_L            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000004C))    /**<RC key define        */
#define BTN_KB_CAPITAL_M            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000004D))    /**<RC key define        */
#define BTN_KB_CAPITAL_N            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000004E))    /**<RC key define        */
#define BTN_KB_CAPITAL_O            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000004F))    /**<RC key define        */
#define BTN_KB_CAPITAL_P            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000050))    /**<RC key define        */
#define BTN_KB_CAPITAL_Q            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000051))    /**<RC key define        */
#define BTN_KB_CAPITAL_R            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000052))    /**<RC key define        */
#define BTN_KB_CAPITAL_S            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000053))    /**<RC key define        */
#define BTN_KB_CAPITAL_T            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000054))    /**<RC key define        */
#define BTN_KB_CAPITAL_U            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000055))    /**<RC key define        */
#define BTN_KB_CAPITAL_V            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000056))    /**<RC key define        */
#define BTN_KB_CAPITAL_W            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000057))    /**<RC key define        */
#define BTN_KB_CAPITAL_X            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000058))    /**<RC key define        */
#define BTN_KB_CAPITAL_Y            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000059))    /**<RC key define        */
#define BTN_KB_CAPITAL_Z            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000005A))    /**<RC key define        */
#define BTN_KB_SQUARE_BRACKET_LEFT  ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000005B))    /**<RC key define        */
#define BTN_KB_BACKSLASH            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000005C))    /**<RC key define        */
#define BTN_KB_SQUARE_BRACKET_RIGHT ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000005D))    /**<RC key define        */
#define BTN_KB_CIRCUMFLEX_ACCENT    ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000005E))    /**<RC key define        */
#define BTN_KB_UNDERSCORE           ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000005F))    /**<RC key define        */
#define BTN_KB_GRAVE_ACCENT         ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000060))    /**<RC key define        */
#define BTN_KB_SMALL_A              ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000061))    /**<RC key define        */
#define BTN_KB_SMALL_B              ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000062))    /**<RC key define        */
#define BTN_KB_SMALL_C              ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000063))    /**<RC key define        */
#define BTN_KB_SMALL_D              ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000064))    /**<RC key define        */
#define BTN_KB_SMALL_E              ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000065))    /**<RC key define        */
#define BTN_KB_SMALL_F              ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000066))    /**<RC key define        */
#define BTN_KB_SMALL_G              ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000067))    /**<RC key define        */
#define BTN_KB_SMALL_H              ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000068))    /**<RC key define        */
#define BTN_KB_SMALL_I              ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000069))    /**<RC key define        */
#define BTN_KB_SMALL_J              ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000006A))    /**<RC key define        */
#define BTN_KB_SMALL_K              ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000006B))    /**<RC key define        */
#define BTN_KB_SMALL_L              ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000006C))    /**<RC key define        */
#define BTN_KB_SMALL_M              ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000006D))    /**<RC key define        */
#define BTN_KB_SMALL_N              ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000006E))    /**<RC key define        */
#define BTN_KB_SMALL_O              ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000006F))    /**<RC key define        */
#define BTN_KB_SMALL_P              ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000070))    /**<RC key define        */
#define BTN_KB_SMALL_Q              ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000071))    /**<RC key define        */
#define BTN_KB_SMALL_R              ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000072))    /**<RC key define        */
#define BTN_KB_SMALL_S              ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000073))    /**<RC key define        */
#define BTN_KB_SMALL_T              ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000074))    /**<RC key define        */
#define BTN_KB_SMALL_U              ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000075))    /**<RC key define        */
#define BTN_KB_SMALL_V              ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000076))    /**<RC key define        */
#define BTN_KB_SMALL_W              ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000077))    /**<RC key define        */
#define BTN_KB_SMALL_X              ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000078))    /**<RC key define        */
#define BTN_KB_SMALL_Y              ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000079))    /**<RC key define        */
#define BTN_KB_SMALL_Z              ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000007A))    /**<RC key define        */
#define BTN_KB_CURLY_BRACKET_LEFT   ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000007B))    /**<RC key define        */
#define BTN_KB_VERTICAL_BAR         ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000007C))    /**<RC key define        */
#define BTN_KB_CURLY_BRACKET_RIGHT  ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000007D))    /**<RC key define        */
#define BTN_KB_TILDE                ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000007E))    /**<RC key define        */
#define BTN_KB_DELETE               ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000007F))    /**<RC key define        */
#define BTN_KB_CURSOR_LEFT          ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000080))    /**<RC key define        */
#define BTN_KB_CURSOR_RIGHT         ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000081))    /**<RC key define        */
#define BTN_KB_CURSOR_UP            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000082))    /**<RC key define        */
#define BTN_KB_CURSOR_DOWN          ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000083))    /**<RC key define        */
#define BTN_KB_INSERT               ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000084))    /**<RC key define        */
#define BTN_KB_HOME                 ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000085))    /**<RC key define        */
#define BTN_KB_END                  ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000086))    /**<RC key define        */
#define BTN_KB_PAGE_UP              ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000087))    /**<RC key define        */
#define BTN_KB_PAGE_DOWN            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000088))    /**<RC key define        */
#define BTN_KB_PRINT                ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000089))    /**<RC key define        */
#define BTN_KB_PAUSE                ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000008A))    /**<RC key define        */
#define BTN_KB_F1                   ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000091))    /**<RC key define        */
#define BTN_KB_F2                   ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000092))    /**<RC key define        */
#define BTN_KB_F3                   ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000093))    /**<RC key define        */
#define BTN_KB_F4                   ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000094))    /**<RC key define        */
#define BTN_KB_F5                   ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000095))    /**<RC key define        */
#define BTN_KB_F6                   ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000096))    /**<RC key define        */
#define BTN_KB_F7                   ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000097))    /**<RC key define        */
#define BTN_KB_F8                   ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000098))    /**<RC key define        */
#define BTN_KB_F9                   ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000099))    /**<RC key define        */
#define BTN_KB_F10                  ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000009A))    /**<RC key define        */
#define BTN_KB_F11                  ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000009B))    /**<RC key define        */
#define BTN_KB_F12                  ((unsigned long)(KEY_GROUP_KEYBOARD | 0x0000009C))    /**<RC key define        */
#define BTN_KB_CAPS_LOCK            ((unsigned long)(KEY_GROUP_KEYBOARD | 0x000000A0))    /**<RC key define        */
#define BTN_KB_NUM_LOCK             ((unsigned long)(KEY_GROUP_KEYBOARD | 0x000000A1))    /**<RC key define        */
#define BTN_KB_SCROLL_LOCK          ((unsigned long)(KEY_GROUP_KEYBOARD | 0x000000A2))    /**<RC key define        */
#define BTN_KB_DOT_COM              ((unsigned long)(KEY_GROUP_KEYBOARD | 0x000000A3))    /**<RC key define        */

#define BTN_KB_ENTER BTN_KB_RETURN

#define BTN_KB_SHIFT                ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000100))    /**<RC key define        */
#define BTN_KB_CONTROL              ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000200))    /**<RC key define        */
#define BTN_KB_ALT                  ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000400))    /**<RC key define        */
#define BTN_KB_ALTGR                ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00000800))    /**<RC key define        */
#define BTN_KB_META                 ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00001000))    /**<RC key define        */
#define BTN_KB_SUPER                ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00002000))    /**<RC key define        */
#define BTN_KB_HYPER                ((unsigned long)(KEY_GROUP_KEYBOARD | 0x00004000))    /**<RC key define        */


#define BTN_IR_MOUSE_LEFT           ((unsigned long)(KEY_GROUP_IR_MOUSE | 0x0000f000))   /**<RC key define        */
#define BTN_IR_MOUSE_RIGHT          ((unsigned long)(KEY_GROUP_IR_MOUSE | 0x0000f001))   /**<RC key define        */
#define BTN_IR_MOUSE_MOVE           ((unsigned long)(KEY_GROUP_IR_MOUSE | 0x0000f002))   /**<RC key define        */

#define BTN_DEBUG_ENABLE         ((unsigned long)(KEY_GROUP_MAX | 0x0000f001))   /**<RC key define        */

/*----------------------------------------------------------------------------*/
/*! @} */                                          /* end of groupMW_RES_MNGR */
/*----------------------------------------------------------------------------*/
#endif
