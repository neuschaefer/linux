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

#ifndef RC56_IR_MAP
#define RC56_IR_MAP

// BTN_RED => BTN_PRG_INFO
// BTN_GREEN => BTN_FAVORITE
// BTN_YELLOW => BTN_ADD_ERASE
// BTN_BLUE => BTN_ASPECT/BTN_TIMER

#define GRP_TV1     0
#define GRP_DVD1    4
#define GRP_DVD     48

#if defined(IRRX_RC6_KEYMAP_FUNC)
#undef IRRX_RC6_KEYMAP_FUNC
#endif
#define IRRX_RC6_KEYMAP_FUNC (toshiba_rc6_ir2crystal)

DECLARE_MAP_BEGIN(toshiba_rc6_ir2crystal)
    // TV1 keys - 0
    MAP_GROUP_BEGIN(GRP_TV1)
        MAP_INSERT(  0,     BTN_DIGIT_0, "0")
        MAP_INSERT(  1,     BTN_DIGIT_1, "1")
        MAP_INSERT(  2,     BTN_DIGIT_2, "2")
        MAP_INSERT(  3,     BTN_DIGIT_3, "3")
        MAP_INSERT(  4,     BTN_DIGIT_4, "4")
        MAP_INSERT(  5,     BTN_DIGIT_5, "5")
        MAP_INSERT(  6,     BTN_DIGIT_6, "6")
        MAP_INSERT(  7,     BTN_DIGIT_7, "7")
        MAP_INSERT(  8,     BTN_DIGIT_8, "8")
        MAP_INSERT(  9,     BTN_DIGIT_9, "9")
        MAP_INSERT( 10,     BTN_PREV_PRG, "prev ch")
        MAP_INSERT( 12,     BTN_POWER, "power")
        MAP_INSERT( 13,     BTN_MUTE, "mute")
        MAP_INSERT( 15,     BTN_RED, "red/info")
        MAP_INSERT( 16,     BTN_VOL_UP, "vol +")
        MAP_INSERT( 17,     BTN_VOL_DOWN, "vol -")
        MAP_INSERT( 56,     BTN_INPUT_SRC, "source")
        MAP_INSERT( 70,     BTN_CC, "cc")
        MAP_INSERT( 71,     BTN_SLEEP, "sleep")
        MAP_INSERT( 78,     BTN_MTS, "sap")
        MAP_INSERT( 84,     BTN_MENU, "menu/exit")
        MAP_INSERT( 88,     BTN_CURSOR_UP, "cursor up/play.pause")
        MAP_INSERT( 89,     BTN_CURSOR_DOWN, "cursor down/stop")
        MAP_INSERT( 90,     BTN_CURSOR_LEFT, "cursor left/rwd")
        MAP_INSERT( 91,     BTN_CURSOR_RIGHT, "cursor right/fwd")
        MAP_INSERT( 92,     BTN_SELECT, "ok")
        MAP_INSERT( 32,     BTN_PRG_UP, "ch +")
        MAP_INSERT( 33,     BTN_PRG_DOWN, "ch -")
        MAP_INSERT(103,     BTN_FREEZE, "freeze")
        MAP_INSERT(109,     BTN_RED, "red")
        MAP_INSERT(110,     BTN_GREEN, "green")
        MAP_INSERT(111,     BTN_YELLOW, "yellow")
        MAP_INSERT(112,     BTN_BLUE, "blue")
        MAP_INSERT(210,     BTN_GREEN, "green/view")
        MAP_INSERT(211,     BTN_YELLOW, "yellow/[check]")
        MAP_INSERT(217,     BTN_DIGIT_DOT, "dot")
        MAP_INSERT(243,     BTN_P_EFFECT, "smart picture")
        MAP_INSERT(244,     BTN_S_EFFECT, "smart sound")
        MAP_INSERT(245,     BTN_BLUE, "blue/format")
        MAP_INSERT(248,     BTN_TIMER, "clock")
    MAP_GROUP_END()
    // DVD keys - 4
    MAP_GROUP_BEGIN(GRP_DVD1)
        MAP_INSERT(  0,     BTN_DIGIT_0, "0")
        MAP_INSERT(  1,     BTN_DIGIT_1, "1")
        MAP_INSERT(  2,     BTN_DIGIT_2, "2")
        MAP_INSERT(  3,     BTN_DIGIT_3, "3")
        MAP_INSERT(  4,     BTN_DIGIT_4, "4")
        MAP_INSERT(  5,     BTN_DIGIT_5, "5")
        MAP_INSERT(  6,     BTN_DIGIT_6, "6")
        MAP_INSERT(  7,     BTN_DIGIT_7, "7")
        MAP_INSERT(  8,     BTN_DIGIT_8, "8")
        MAP_INSERT(  9,     BTN_DIGIT_9, "9")
//        MAP_INSERT( 12,     BTN_POWER, "dvd power")   XXX
//        MAP_INSERT( 13,     BTN_MUTE, "dvd mute")     XXX
//        MAP_INSERT( 15,     BTN_DISPLAY, "dvd display, osd menu") XXX
//        MAP_INSERT( 19,     BTN_DIM, "dim")           XXX
//        MAP_INSERT( 28,     BTN_NONE, "shuffle")      XXX
        MAP_INSERT( 29,     BTN_REPEAT, "repeat")
        MAP_INSERT( 32,     BTN_NEXT, "chapter forward")
        MAP_INSERT( 33,     BTN_PREV, "chapter backward")
        MAP_INSERT( 40,     BTN_FF, "FF")
        MAP_INSERT( 41,     BTN_FR, "RWD/FR")
//        MAP_INSERT( 42,     BTN_SCAN, "scan")         XXX
        MAP_INSERT( 44,     BTN_PLAY, "play")
        MAP_INSERT( 48,     BTN_PAUSE, "pause")
        MAP_INSERT( 49,     BTN_STOP, "stop")
        MAP_INSERT( 59,     BTN_REPEAT_A_B, "repeat a-b")
        MAP_INSERT( 66,     BTN_EJECT, "eject")
        MAP_INSERT( 72,     BTN_PRG_INFO, "info/display")
        MAP_INSERT( 75,     BTN_SUB_TITLE, "subtitle")
        MAP_INSERT( 78,     BTN_MTS, "audio")
        MAP_INSERT( 80,     BTN_S_EFFECT, "sound mode")
        MAP_INSERT( 84,     BTN_TITLE_MENU, "disk/dvd menu")
        MAP_INSERT( 88,     BTN_CURSOR_UP, "cursor up")
        MAP_INSERT( 89,     BTN_CURSOR_DOWN, "cursor down")
        MAP_INSERT( 90,     BTN_CURSOR_LEFT, "cursor left")
        MAP_INSERT( 91,     BTN_CURSOR_RIGHT, "cursor right")
        MAP_INSERT( 92,     BTN_SELECT, "ok")
        MAP_INSERT(120,     BTN_DIGIT_PLUS_10, "+10")
        MAP_INSERT(123,     BTN_REPEAT_A_B, "repeat a-b")
        MAP_INSERT(130,     BTN_ROOT_MENU, "root/system menu")
//        MAP_INSERT(131,     BTN_NONE, "return/title")
        MAP_INSERT(133,     BTN_ANGLE, "angle")
//        MAP_INSERT(200,     BTN_NONE, "T-C") XXX
        MAP_INSERT(209,     BTN_ROOT_MENU, "disc menu")
//        MAP_INSERT(213,     BTN_PREVIEW, "preview")
        MAP_INSERT(227,     BTN_SUB_TITLE, "subtitle")
        MAP_INSERT(228,     BTN_KARAOKE, "karaoke")
        MAP_INSERT(231,     BTN_VOCAL_ASSIST, "vocal")
//        MAP_INSERT(232,     BTN_MODE, "mode")
//        MAP_INSERT(236,     BTN_KEY, "key")
        MAP_INSERT(247,     BTN_ZOOM, "zoom")
//        MAP_INSERT(251,     BTN_FTS, "fts")
    MAP_GROUP_END()
    // DVD keys - 48
    MAP_GROUP_BEGIN(GRP_DVD)
        MAP_INSERT(  0,     BTN_DIGIT_0, "0")
        MAP_INSERT(  1,     BTN_DIGIT_1, "1")
        MAP_INSERT(  2,     BTN_DIGIT_2, "2")
        MAP_INSERT(  3,     BTN_DIGIT_3, "3")
        MAP_INSERT(  4,     BTN_DIGIT_4, "4")
        MAP_INSERT(  5,     BTN_DIGIT_5, "5")
        MAP_INSERT(  6,     BTN_DIGIT_6, "6")
        MAP_INSERT(  7,     BTN_DIGIT_7, "7")
        MAP_INSERT(  8,     BTN_DIGIT_8, "8")
        MAP_INSERT(  9,     BTN_DIGIT_9, "9")
        MAP_INSERT( 29,     BTN_REPEAT, "repeat")
        MAP_INSERT( 32,     BTN_NEXT, "chapter forward")
        MAP_INSERT( 33,     BTN_PREV, "chapter backward")
        MAP_INSERT( 40,     BTN_FF, "FF")
        MAP_INSERT( 41,     BTN_FR, "RWD/FR")
        MAP_INSERT( 44,     BTN_PLAY, "play")
        MAP_INSERT( 48,     BTN_PAUSE, "pause")
        MAP_INSERT( 49,     BTN_STOP, "stop")
        MAP_INSERT( 59,     BTN_REPEAT_A_B, "repeat a-b")
        MAP_INSERT( 66,     BTN_EJECT, "eject")
        MAP_INSERT( 72,     BTN_PRG_INFO, "info")
        MAP_INSERT( 75,     BTN_SUB_TITLE, "subtitle")
        MAP_INSERT( 78,     BTN_MTS, "audio")
        MAP_INSERT( 80,     BTN_S_EFFECT, "sound mode")
        MAP_INSERT( 84,     BTN_TITLE_MENU, "disk/dvd menu")
        MAP_INSERT( 88,     BTN_CURSOR_UP, "cursor up")
        MAP_INSERT( 89,     BTN_CURSOR_DOWN, "cursor down")
        MAP_INSERT( 90,     BTN_CURSOR_LEFT, "cursor left")
        MAP_INSERT( 91,     BTN_CURSOR_RIGHT, "cursor right")
        MAP_INSERT( 92,     BTN_SELECT, "ok")
        MAP_INSERT(120,     BTN_DIGIT_PLUS_10, "+10")
        MAP_INSERT(123,     BTN_REPEAT_A_B, "repeat a-b")
        MAP_INSERT(130,     BTN_ROOT_MENU, "root menu")
        MAP_INSERT(133,     BTN_ANGLE, "angle")
        MAP_INSERT(209,     BTN_ROOT_MENU, "disc menu")
        MAP_INSERT(227,     BTN_SUB_TITLE, "subtitle")
        MAP_INSERT(228,     BTN_KARAOKE, "karaoke")
        MAP_INSERT(231,     BTN_VOCAL_ASSIST, "vocal")
        MAP_INSERT(247,     BTN_ZOOM, "zoom")
    MAP_GROUP_END()
DECLARE_MAP_END()

#if defined(IRRX_RC5_KEYMAP_FUNC)
#undef IRRX_RC5_KEYMAP_FUNC
#endif

#define IRRX_RC5_KEYMAP_FUNC (toshiba_rc5_ir2crystal)

DECLARE_MAP_BEGIN(toshiba_rc5_ir2crystal)
    // TV1 keys
    MAP_GROUP_BEGIN(GRP_TV1)
        MAP_INSERT(  0,     BTN_DIGIT_0, "0")
        MAP_INSERT(  1,     BTN_DIGIT_1, "1")
        MAP_INSERT(  2,     BTN_DIGIT_2, "2")
        MAP_INSERT(  3,     BTN_DIGIT_3, "3")
        MAP_INSERT(  4,     BTN_DIGIT_4, "4")
        MAP_INSERT(  5,     BTN_DIGIT_5, "5")
        MAP_INSERT(  6,     BTN_DIGIT_6, "6")
        MAP_INSERT(  7,     BTN_DIGIT_7, "7")
        MAP_INSERT(  8,     BTN_DIGIT_8, "8")
        MAP_INSERT(  9,     BTN_DIGIT_9, "9")
        MAP_INSERT( 10,     BTN_P_EFFECT, "picture")
        MAP_INSERT( 11,     BTN_S_EFFECT, "sound")
        MAP_INSERT( 12,     BTN_POWER, "power")
        MAP_INSERT( 13,     BTN_MUTE, "mute")
        MAP_INSERT( 15,     BTN_PRG_INFO, "Display Info +")
        MAP_INSERT( 16,     BTN_VOL_UP, "vol +")
        MAP_INSERT( 17,     BTN_VOL_DOWN, "vol -")
        MAP_INSERT( 32,     BTN_PRG_UP, "ch +")
        MAP_INSERT( 33,     BTN_PRG_DOWN, "ch -")
        MAP_INSERT( 34,     BTN_PREV_PRG, "prev ch")
        MAP_INSERT( 35,     BTN_MTS, "mts")
//        MAP_INSERT( 36,     BTN_NONE, "incr.surr.") XXX
        MAP_INSERT( 38,     BTN_SLEEP, "sleep")
//        MAP_INSERT( 41,     BTN_ZOOM_IN, "zoom in?")
        MAP_INSERT( 43,     BTN_TIMER, "clock")
//        MAP_INSERT( 44,     BTN_HELP, "help, ?") XXX
        MAP_INSERT( 56,     BTN_INPUT_SRC, "source")
        MAP_INSERT( 58,     BTN_CC, "cc")
        MAP_INSERT( 60,     BTN_TTX, "teletext")
//        MAP_INSERT( 62,     BTN_NONE, "incr.pic") XXX
        MAP_INSERT( 80,     BTN_CURSOR_UP, "cursor up/play.pause")
        MAP_INSERT( 81,     BTN_CURSOR_DOWN, "cursor down/stop")
        MAP_INSERT( 82,     BTN_MENU, "menu/exit")
        MAP_INSERT( 85,     BTN_CURSOR_LEFT, "cursor left/rwd")
        MAP_INSERT( 86,     BTN_CURSOR_RIGHT, "cursor right/fwd")
//        MAP_INSERT( 87,     BTN_SELECT, "ok")
//        MAP_INSERT( 88,     BTN_PIP, "pip") XXX
        MAP_INSERT(121,     BTN_CH_LIST, "surf")
        MAP_INSERT(126,     BTN_ASPECT, "format")
    MAP_GROUP_END()
    // Smart keys - 3
    MAP_GROUP_BEGIN(3)
        MAP_INSERT( 10,     BTN_P_EFFECT, "smart block")
        MAP_INSERT( 11,     BTN_S_EFFECT, "smart music")
    MAP_GROUP_END()

    // DVD keys
    MAP_GROUP_BEGIN(GRP_DVD)
	// TVP said DVD without RC5 protocol.
    MAP_GROUP_END()

DECLARE_MAP_END()

#endif /*PHARMONEY_IR_MAP*/
