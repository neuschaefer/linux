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

#ifndef MTK_IR_RC5_H
#define MTK_IR_RC5_H

static const KEYUNIT_T _arRC56KeyMap[] =
{
    // RC6 Key map
    { 0x03800023, BTN_DIGIT_0 },
//    { 0x03800043, BTN_DIGIT_0 },
    { 0x03820023, BTN_PRG_UP },
//    { 0x03820043, BTN_PRG_UP },
    { 0x03840023, BTN_VOL_UP },
//    { 0x03840043, BTN_VOL_UP },
    { 0x03880023, BTN_DIGIT_8 },
//    { 0x03880043, BTN_DIGIT_8 },
    { 0x038d0023, BTN_CURSOR_UP },
//    { 0x038d0043, BTN_CURSOR_UP },
    { 0x038e0023, BTN_INPUT_SRC /* BTN_AV */ },
//    { 0x038e0043, BTN_INPUT_SRC /* BTN_AV */ },
    { 0x03900023, BTN_DIGIT_4 },
//    { 0x03900043, BTN_DIGIT_4 },
    { 0x03950023, BTN_MENU },
//    { 0x03950043, BTN_MENU },
    { 0x03978023, BTN_S_EFFECT /* BTN_SOUND */ },
//    { 0x03978043, BTN_S_EFFECT /* BTN_SOUND */ },
    { 0x03980023, BTN_POWER },
//    { 0x03980043, BTN_POWER },
    { 0x039d0023, BTN_SELECT /* BTN_OK */ },
//    { 0x039d0043, BTN_SELECT /* BTN_OK */ },
    { 0x03a00023, BTN_DIGIT_2 },
//    { 0x03a00043, BTN_DIGIT_2 },
    { 0x03a58023, BTN_EPG /* BTN_VIEW */ },
//    { 0x03a58043, BTN_EPG /* BTN_VIEW */ },
    { 0x03a80023, BTN_PREV_PRG /* BTN_A_CH */ },
//    { 0x03a80043, BTN_PREV_PRG /* BTN_A_CH */ },
    { 0x03ad0023, BTN_CURSOR_LEFT },
//    { 0x03ad0043, BTN_CURSOR_LEFT },
    { 0x03b00023, BTN_DIGIT_6 },
//    { 0x03b00043, BTN_DIGIT_6 },
    { 0x03c00023, BTN_DIGIT_1 },
//    { 0x03c00043, BTN_DIGIT_1 },
    { 0x03c20023, BTN_PRG_DOWN },
//    { 0x03c20043, BTN_PRG_DOWN },
    { 0x03c40023, BTN_VOL_DOWN },
//    { 0x03c40043, BTN_VOL_DOWN },
    { 0x03c80023, BTN_DIGIT_9 },
//    { 0x03c80043, BTN_DIGIT_9 },
    { 0x03cd0023, BTN_CURSOR_DOWN },
//    { 0x03cd0043, BTN_CURSOR_DOWN },
    { 0x03cd8023, BTN_DIGIT_DOT },
//    { 0x03cd8043, BTN_DIGIT_DOT },
    { 0x03d00023, BTN_DIGIT_5 },
//    { 0x03d00043, BTN_DIGIT_5 },
//    { 0x03d78023, BTN_POP /* BTN_FORMAT */ },
//    { 0x03d78043, BTN_POP /* BTN_FORMAT */ },
    { 0x03d80023, BTN_MUTE },
//    { 0x03d80043, BTN_MUTE },
    { 0x03e00023, BTN_DIGIT_3 },
//    { 0x03e00043, BTN_DIGIT_3 },
    { 0x03e58023, BTN_EXIT /* BTN_CHECK */ },
//    { 0x03e58043, BTN_EXIT /* BTN_CHECK */ },
//    { 0x03e78023, BTN_PIPPOS /* BTN_PICTURE */ },
//    { 0x03e78043, BTN_PIPPOS /* BTN_PICTURE */ },
    { 0x03ed0023, BTN_CURSOR_RIGHT },
//    { 0x03ed0043, BTN_CURSOR_RIGHT },
    { 0x03f00023, BTN_DIGIT_7 },
//    { 0x03f00043, BTN_DIGIT_7 },
    { 0x03f80023, BTN_PRG_INFO /* BTN_INFO */ },
//    { 0x03f80043, BTN_PRG_INFO /* BTN_INFO */ },
    // RC5 Key map
    { 0x072cd553, BTN_CURSOR_RIGHT },
//    { 0x072cd555, BTN_CURSOR_RIGHT },
    { 0x0734d553, BTN_MENU },
//    { 0x0734d555, BTN_MENU },
    { 0x074b5553, BTN_SELECT /* BTN_PC_TV */ },
//    { 0x074b5555, BTN_SELECT /* BTN_PC_TV */ },
    { 0x07533553, BTN_EXIT /* BTN_MINIMIZE */ },
//    { 0x07533555, BTN_EXIT /* BTN_MINIMIZE */ },
    { 0x0754d553, BTN_CURSOR_UP },
//    { 0x0754d555, BTN_CURSOR_UP },
    { 0x0e5a6aa5, BTN_EPG },    /* BTN_CLOCK */
//    { 0x0e5a6aa9, BTN_EPG },    /* BTN_CLOCK */
    { 0x0e5aaaa5, BTN_DIGIT_6 },
//    { 0x0e5aaaa9, BTN_DIGIT_6 },
    { 0x0e656aa5, BTN_CC },
//    { 0x0e656aa9, BTN_CC },
    { 0x0e6695a5, BTN_ZOOM /* BTN_SMART SCREEN */ },
//    { 0x0e6695a9, BTN_ZOOM /* BTN_SMART SCREEN */ },
//    { 0x0e6a6aa5, BTN_POP /* BTN_P<-P */ },
//    { 0x0e6a6aa9, BTN_POP /* BTN_P<-P */ },
    { 0x0e6aaaa5, BTN_DIGIT_2 },
//    { 0x0e6aaaa9, BTN_DIGIT_2 },
    { 0x0e96aaa5, BTN_POWER },
//    { 0x0e96aaa9, BTN_POWER },
    { 0x0e9aaaa5, BTN_DIGIT_4 },
//    { 0x0e9aaaa9, BTN_DIGIT_4 },
    { 0x0ea56aa5, BTN_INPUT_SRC /* BTN_AV */ },
//    { 0x0ea56aa9, BTN_INPUT_SRC /* BTN_AV */ },
    { 0x0ea6aaa5, BTN_DIGIT_8 },
//    { 0x0ea6aaa9, BTN_DIGIT_8 },
    { 0x0ea9aaa5, BTN_VOL_UP },
//    { 0x0ea9aaa9, BTN_VOL_UP },
    { 0x0eaa6aa5, BTN_PRG_UP },
//    { 0x0eaa6aa9, BTN_PRG_UP },
    { 0x0eaaaaa5, BTN_DIGIT_0 },
//    { 0x0eaaaaa9, BTN_DIGIT_0 },
    { 0x0eacd553, BTN_PRG_INFO },
//    { 0x0eacd555, BTN_PRG_INFO },
    { 0x0eccd553, BTN_CURSOR_LEFT },
//    { 0x0eccd555, BTN_CURSOR_LEFT },
    { 0x0ed2b553, BTN_SELECT },
//    { 0x0ed2b555, BTN_SELECT },
//    { 0x0ed2d553, BTN_PIPPOS },
//    { 0x0ed2d555, BTN_PIPPOS },
    { 0x0ed4d553, BTN_CURSOR_DOWN },
//    { 0x0ed4d555, BTN_CURSOR_DOWN },
    { 0x1d5aaaa5, BTN_DIGIT_7 },
//    { 0x1d5aaaa9, BTN_DIGIT_7 },
    { 0x1d6696a5, BTN_S_EFFECT /* BTN_SMART SOUND */ },
//    { 0x1d6696a9, BTN_S_EFFECT /* BTN_SMART SOUND */ },
    { 0x1d6a6aa5, BTN_MTS /* BTN_I-II */ },
//    { 0x1d6a6aa9, BTN_MTS /* BTN_I-II */ },
    { 0x1d6aaaa5, BTN_DIGIT_3 },
//    { 0x1d6aaaa9, BTN_DIGIT_3 },
    { 0x1d96aaa5, BTN_MUTE },
//    { 0x1d96aaa9, BTN_MUTE },
    { 0x1d9aaaa5, BTN_DIGIT_5 },
//    { 0x1d9aaaa9, BTN_DIGIT_5 },
    { 0x1da6aaa5, BTN_DIGIT_9 },
//    { 0x1da6aaa9, BTN_DIGIT_9 },
    { 0x1da9aaa5, BTN_VOL_DOWN },
//    { 0x1da9aaa9, BTN_VOL_DOWN },
    { 0x1daa6aa5, BTN_PRG_DOWN },
//    { 0x1daa6aa9, BTN_PRG_DOWN },
    { 0x1daaaaa5, BTN_DIGIT_1 },
//    { 0x1daaaaa9, BTN_DIGIT_1 },
};

#endif /* MTK_IR_RC5_H */
