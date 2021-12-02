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
// The Changhong TV user controller IRRX key map, the u4Key must from small to big as sort for _IRRX_CrystalLookup "1/2" method 
static const KEYUNIT_T _arUserKeyMap[] =
{
    { 0x00, BTN_DIGIT_0 },          // DIGIT_0 
    { 0x01, BTN_DIGIT_1 },          // DIGIT_1 
    { 0x02, BTN_DIGIT_2 },          // DIGIT_2 
    { 0x03, BTN_DIGIT_3 },          // DIGIT_3 
    { 0x04, BTN_DIGIT_4 },          // DIGIT_4 
    { 0x05, BTN_DIGIT_5 },          // DIGIT_5
    { 0x06, BTN_DIGIT_6 },          // DIGIT_6
    { 0x07, BTN_DIGIT_7 },          // DIGIT_7
    { 0x08, BTN_DIGIT_8 },          // DIGIT_8
    { 0x09, BTN_DIGIT_9 },          // DIGIT_9
    { 0x0A, BTN_SELECT },           // SELECT OK
    { 0x0B, BTN_INVALID},           // NO USE
    { 0x0C, BTN_INVALID},           // NO USE
    { 0x0D, BTN_INVALID},           // NO USE
    { 0x0E, BTN_INVALID},           // NO USE
    { 0x0F, BTN_INVALID},           // NO USE
    { 0x10, BTN_MUTE},              // MUTE
    { 0x11, BTN_P_EFFECT },        	// PICTURE MODE
    { 0x12, BTN_POWER},            	// POWER
    { 0x13, BTN_INVALID},           // NO USE
    { 0x14, BTN_INPUT_SRC},      	// TV/AV 
    { 0x15, BTN_INVALID },              	// AUTO
    { 0x16, BTN_PRG_INFO},        	// DISPLAY INFO
    { 0x17, BTN_CC},              	// SUBTITLE
    { 0x18, BTN_CH_LIST},          	// PROGRAM GUIDE
    { 0x19, BTN_CURSOR_UP},     	// UP
    { 0x1a, BTN_VOL_UP},            // VOL +
    { 0x1b, BTN_PRG_UP},            // PROGRAM +
    { 0x1c, BTN_MTS},               // AUDIO TRACK
    { 0x1d, BTN_CURSOR_DOWN},  		// DOWN
    { 0x1e, BTN_VOL_DOWN},      	// VOL -
    { 0x1f, BTN_PRG_DOWN},      	// PROGRAM -
    { 0x40, BTN_PREV_PRG},        	// RECALL
    { 0x41, BTN_INVALID},           // NO USE
    { 0x42, BTN_INVALID},           // FAVORITE
    { 0x43, BTN_INVALID},                // 3D
    { 0x44, BTN_EXIT},              // EXIT
    { 0x45, BTN_ASPECT},           	// ZOOM
    { 0x46, BTN_CURSOR_LEFT},     	// LEFT
    { 0x47, BTN_CURSOR_RIGHT},  	// RIGHT
    { 0x48, BTN_INVALID },      	// POWER SAVE
    { 0x49, BTN_INVALID },              // EYE
    { 0x4A, BTN_RED},               // RED
    { 0x4B, BTN_GREEN},            	// GREEN
    { 0x4C, BTN_YELLOW},          	// YELLOW
    { 0x4D, BTN_BLUE},              // CYAN
    { 0x4E, BTN_FF },               // FF
    { 0x4F, BTN_FR},                // FR
    { 0x50, BTN_INVALID},         	// NO USE
    { 0x51, BTN_INVALID},           // ONLINE
    { 0x52, BTN_INVALID },     	// SHORTCUT
    { 0x53, BTN_INVALID},         	// NO USE
    { 0x54, BTN_INVALID},         	// NO USE
    { 0x55, BTN_INVALID },             // HELP
    { 0x56, BTN_NEXT},              // NEXT
    { 0x57, BTN_PAUSE},            	// START/PAUSE
    { 0x58, BTN_INVALID},         	// NO USE
    { 0x59, BTN_PREV},              // PREV
    { 0x5A, BTN_S_EFFECT },      	// SOUND MODE
    { 0x5B, BTN_MENU },            	// MENU  
    { 0x5C, BTN_INVALID},         	// NO USE
    { 0x5D, BTN_MEM_CARD},     		// USB
    { 0x5E, BTN_INVALID},         	// NO USE
    { 0x5F, BTN_INVALID},         	// NO USE
};
static const KEYUNIT_T _arMtkTvBigKeyMap[] =
{
    { 0x00, BTN_RED },
    { 0x01, BTN_GREEN },
    { 0x02, BTN_POWER },
    { 0x03, BTN_YELLOW },
    { 0x04, BTN_ZOOM },
    { 0x05, BTN_BLUE },
    { 0x06, BTN_TIMER },
    { 0x07, BTN_MEM_CARD },
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
// The Changhong TV factory controller IRRX key map, the u4Key must from small to big as sort for _IRRX_CrystalLookup "1/2" method 
static const KEYUNIT_T _arFactoryKeyMap[] =
{
    { 0x00, BTN_DIGIT_0 },        	// DIGIT_0 
    { 0x01, BTN_DIGIT_1 },         	// DIGIT_1 
    { 0x02, BTN_DIGIT_2 },          // DIGIT_2 
    { 0x03, BTN_DIGIT_3 },          // DIGIT_3 
    { 0x04, BTN_DIGIT_4 },          // DIGIT_4 
    { 0x05, BTN_DIGIT_5 },          // DIGIT_5
    { 0x06, BTN_DIGIT_6 },          // DIGIT_6
    { 0x07, BTN_DIGIT_7 },          // DIGIT_7
    { 0x08, BTN_DIGIT_8 },          // DIGIT_8
    { 0x09, BTN_DIGIT_9 },          // DIGIT_9
    { 0x0A, BTN_SELECT },          	// SELECT OK
    { 0x0B, BTN_INVALID},           // NO USE
    { 0x0C, BTN_HDMI},             	// YPBPR
    { 0x0D, BTN_S_EFFECT},          // SOUND MODE
    { 0x0E, BTN_MEM_CARD},          // MMP
    { 0x0F, BTN_VGA},               // VGA
    { 0x10, BTN_FACTORY_MODE_1},	// FACTORY:M
    { 0x11, BTN_INVALID},       	// COLORSYS
    { 0x12, BTN_POWER},             // POWER
    { 0x13, BTN_INVALID},           // NO USE
    { 0x14, BTN_MENU},              // MENU  
    { 0x15, BTN_INVALID},           // NO USE
    { 0x16, BTN_COMPOSITE_2},      	// AV2
    { 0x17, BTN_INVALID},           // NO USE
    { 0x18, BTN_COMPOSITE_1},      	// AV1
    { 0x19, BTN_CURSOR_UP},         // UP
    { 0x1a, BTN_VOL_UP},            // VOL +
    { 0x1b, BTN_PRG_UP},            // PROGRAM +
    { 0x1C, BTN_INVALID},           // NO USE
    { 0x1d, BTN_CURSOR_DOWN},    	// DOWN
    { 0x1e, BTN_VOL_DOWN},          // VOL -
    { 0x1f, BTN_PRG_DOWN},          // PROGRAM -
    { 0x40, BTN_INPUT_SRC},         // TV/AV
    { 0x41, BTN_INVALID},           // NO USE
    { 0x42, BTN_INVALID},         // PC COLORAUTO
    { 0x43, BTN_P_EFFECT},          // PIC MODE
    { 0x44, BTN_INVALID},     	// YPBPR COLORAUTO
    { 0x45, BTN_INVALID},       	// FACTORY OUT
    { 0x46, BTN_CURSOR_LEFT},       // LEFT
    { 0x47, BTN_CURSOR_RIGHT},    	// RIGHT
    { 0x48, BTN_S_VIDEO },        	// SVIDEO
    { 0x49, BTN_MTS },             	// NICAM
    { 0x4A, BTN_INVALID},        // PC AUTO
    { 0x4B, BTN_INVALID},           // NO USE
    { 0x4C, BTN_INVALID},           // NO USE
    { 0x4D, BTN_INVALID},       // BALANCE
    { 0x4E, BTN_INVALID},           // NO USE
    { 0x4F, BTN_INVALID},           // NO USE
    { 0x50, BTN_INVALID},           // NO USE
    { 0x51, BTN_INVALID},                // 3D
    { 0x52, BTN_PRG_INFO},          // DISPLAY
    { 0x53, BTN_P_EFFECT},          // PIC MODE
    { 0x54, BTN_INVALID},        // SEARCH
    { 0x55, BTN_INVALID},           // NO USE
    { 0x56, BTN_INVALID},           // NO USE
    { 0x57, BTN_INVALID},           // NO USE
    { 0x58, BTN_MUTE},              // MUTE
    { 0x59, BTN_INVALID},           // NO USE
    { 0x5A, BTN_INVALID },       // SOUND SYS
    { 0x5B, BTN_INVALID },          // TV PID  
    { 0x5C, BTN_INVALID},        // MAC
    { 0x5D, BTN_COMPONENT},         // YPBPR
    { 0x5E, BTN_INVALID},           // NO USE
    { 0x5F, BTN_INVALID},           // NO USE
};


#endif /* MTK_IR_NEC_H */

