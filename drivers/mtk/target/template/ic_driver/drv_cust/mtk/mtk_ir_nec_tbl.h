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

#ifndef MTK_IR_NEC_H
#define MTK_IR_NEC_H

// The MTK TV controller IRRX key map
static const KEYUNIT_T _arMtkTvKeyMap[] =
{
    { 0x00, BTN_GREEN },            /* 0x00, Green Key */
    { 0x01, BTN_YELLOW },           /* 0x01, Yellow Key */
//    { 0x02, BTN_SMARTPIC },         /* 0x02, Smart PIC */
    { 0x03, BTN_DIGIT_0 },          /* 0x03, Num0 */
    { 0x04, BTN_DIGIT_DOT },        /* 0x04, Blank button */
    { 0x05, BTN_TTX },              /* 0x05, TT/TV */
    { 0x06, BTN_MENU },             /* 0x06, Menu */
    { 0x07, BTN_MUTE },             /* 0x07, Mute */
    { 0x08, BTN_DIGIT_8 },          /* 0x08, Num8 */
    { 0x09, BTN_DIGIT_9 },          /* 0x09, Num9 */
    { 0x0a, BTN_ASPECT },           /* 0x0a, Active Control */
    { 0x0b, BTN_DIGIT_7 },          /* 0x0b, Num7 */
    { 0x0c, BTN_DIGIT_5 },          /* 0x0c, Num5 */
    { 0x0d, BTN_DIGIT_6 },          /* 0x0d, Num6 */
    { 0x0e, BTN_POWER },            /* 0x0e, Power */
    { 0x0f, BTN_DIGIT_4 },          /* 0x0f, Num4 */
    { 0x10, BTN_DIGIT_2 },          /* 0x10, Num2 */
    { 0x11, BTN_DIGIT_3 },          /* 0x11, Num3 */
    { 0x12, BTN_POP },              /* 0x12, POP */
    { 0x13, BTN_DIGIT_1 },          /* 0x13, Num1 */
    { 0x14, BTN_RED },              /* 0x14, Red Key */
    { 0x15, BTN_CC },               /* 0x15, CC */
    { 0x16, BTN_PIP_POS },          /* 0x16, PIP POS */
    { 0x17, BTN_TV },               /* 0x17, Color System */
    { 0x18, BTN_PREV_PRG },         /* 0x18, Alternate Channel */
    { 0x19, BTN_FAV_CH },           /* 0x19, Favorite Channel */
    { 0x1a, BTN_FAVORITE },         /* 0x1a, Favorite */
//    { 0x1b, BTN_CAPTURE },          /* 0x1b, Capture */
    { 0x1c, BTN_CURSOR_LEFT },      /* 0x1c, Left Key */
    { 0x1d, BTN_CURSOR_DOWN },      /* 0x1d, Down Key */
    { 0x1e, BTN_INPUT_SRC },        /* 0x1e, Input */
    { 0x1f, BTN_PRG_INFO },         /* 0x1f, OSD */
    { 0x40, BTN_SLEEP },            /* 0x40, Sleep */
    { 0x41, BTN_ZOOM },             /* 0x41, Zoom */
    { 0x42, BTN_BLUE },             /* 0x42, Blue Key */
    { 0x43, BTN_EXIT },             /* 0x43, CE */
    { 0x44, BTN_CURSOR_UP },        /* 0x44, Up Key */
    { 0x45, BTN_VOL_DOWN },         /* 0x45, Volume Down */
    { 0x47, BTN_INDEX },            /* 0x47, Index */
    { 0x48, BTN_CURSOR_RIGHT },     /* 0x48, Right Key */
    { 0x49, BTN_VOL_UP },           /* 0x49, Volume Up */
    { 0x4b, BTN_EPG },              /* 0x4b, Clock */
    { 0x4c, BTN_TIMER },            /* 0x4c, Timer */
    { 0x4d, BTN_PRG_DOWN },         /* 0x4d, Channel Down */
    { 0x4f, BTN_MEM_CARD },         /* 0x4f, Update */
    { 0x50, BTN_ADD_ERASE },        /* 0x50, ADD/Erase */
    { 0x51, BTN_PRG_UP },           /* 0x51, Channel Up */
    { 0x54, BTN_FREEZE },           /* 0x54, Freeze */
//    { 0x55, BTN_VCHIP  },           /* 0x55, V-Chip */
    { 0x58, BTN_PIP_SIZE },         /* 0x58, PIC SIZE */
    { 0x59, BTN_S_EFFECT },         /* 0x59, Sound Effect */
    { 0x5c, BTN_SELECT },           /* 0x5c, Enter Key */
    { 0x5d, BTN_MTS }               /* 0x5d, MTS/Lang */
};

// The MTK TV controller IRRX key map
static const KEYUNIT_T _arMtkTvBigKeyMap[] =
{
    { 0x00, BTN_RED },
    { 0x01, BTN_GREEN },
    { 0x02, BTN_POWER },
    { 0x03, BTN_YELLOW },
    { 0x04, BTN_ZOOM },
    { 0x05, BTN_BLUE },
    { 0x06, BTN_TIMER },
    { 0x07, BTN_ANGLE },
    { 0x08, BTN_DIGIT_3 },
    { 0x09, BTN_DIGIT_2 },
    { 0x0a, BTN_DIGIT_1 },
    { 0x0b, BTN_SELECT },       // ENTER
    { 0x0c, BTN_DIGIT_9 },
    { 0x0d, BTN_DIGIT_8 },
    { 0x0e, BTN_DIGIT_7 },
    { 0x0f, BTN_FAV_CH },       // STOP, FAV_CH
    { 0x10, BTN_DIGIT_6 },
    { 0x11, BTN_DIGIT_5 },
    { 0x12, BTN_DIGIT_4 },
    { 0x13, BTN_CURSOR_DOWN },
    { 0x14, BTN_MENU },
    { 0x15, BTN_DIGIT_0 },
    { 0x16, BTN_FREEZE },
    { 0x17, BTN_RECORD },       // RECORD, CAPTURE
    { 0x18, BTN_PRG_INFO },     // INFO
    { 0x19, BTN_EPG },          // GUIDE
    { 0x1a, BTN_EXIT },
    { 0x1b, BTN_PREV },         // DVD previous, REVEAL
    { 0x1c, BTN_CURSOR_RIGHT },
    { 0x1d, BTN_CURSOR_UP },
    { 0x1e, BTN_CURSOR_LEFT },
    { 0x1f, BTN_P_EFFECT },     // P.EFFECT
    { 0x40, BTN_S_EFFECT },     // S.EFFECT
    { 0x42, BTN_PIP_SIZE },
    { 0x43, BTN_MTS },          // MTS/Audio
    { 0x44, BTN_REPEAT },       // DVD REPEAT, WIDE MODE
    { 0x45, BTN_SUB_TITLE },    // SUB_TITLE
    { 0x46, BTN_TITLE_PBC },    // DVD TITLE/PBC, MIX
    { 0x47, BTN_SWAP },
    { 0x48, BTN_FAVORITE },     // DVD PLAY_PAUSE, Favorite
    { 0x49, BTN_ADD_ERASE },    // DVD Eject, ADD_ERASE
    { 0x4a, BTN_NEXT },         // DVD next, SIZE
    { 0x4c, BTN_VOL_UP },
    { 0x4d, BTN_FR },           // DVD backward, TTX
    { 0x4e, BTN_PRG_DOWN },
    { 0x50, BTN_FF },           // DVD forward, SUBPAGE
    { 0x54, BTN_VOL_DOWN },
    { 0x55, BTN_PRG_UP },
    { 0x56, BTN_MUTE },
    { 0x57, BTN_PREV_PRG },
    { 0x58, BTN_PIP_POP },      // PIP/POP
    { 0x59, BTN_INPUT_SRC },    // Source
    { 0x5a, BTN_DIGIT_DOT },
    { 0x5b, BTN_ROOT_MENU },    // DVD Root menu, INDEX
    { 0x5c, BTN_PIP_POS },      // PIP POS
    { 0x5d, BTN_ASPECT },
    { 0x5e, BTN_CC },
    { 0x5f, BTN_SLEEP },
};

#endif /* MTK_IR_NEC_H */

