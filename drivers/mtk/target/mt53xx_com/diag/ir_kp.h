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
// YTChen@2005/04/12, used in _RxCLI_SendIrPattern
#define BTN_DELAY_1S            ((UINT32) (KEY_GROUP_MAX | 0x00000001))
#define BTN_DELAY_2S            ((UINT32) (KEY_GROUP_MAX | 0x00000002))
#define BTN_DELAY_5S            ((UINT32) (KEY_GROUP_MAX | 0x00000003))
#define BTN_DELAY_10S           ((UINT32) (KEY_GROUP_MAX | 0x00000004))

static UINT32 _au4MtkKeyPattern0[] =
{
	BTN_GREEN,				/* 0x00, Green Key */
	BTN_YELLOW,				/* 0x01, Yellow Key */
	BTN_SMARTPIC,			/* 0x02, Smart PIC */
	BTN_DIGIT_0,			/* 0x03, Num0 */
	BTN_DIGIT_DOT,			/* 0x04, Blank button */
	BTN_TTTV,				/* 0x05, TT/TV */
	BTN_MENU,				/* 0x06, Menu */
	BTN_MUTE,				/* 0x07, Mute */
	BTN_DIGIT_8,			/* 0x08, Num8 */
	BTN_DIGIT_9,			/* 0x09, Num9 */
	BTN_EXIT,				/* 0x0a, Active Control */
	BTN_DIGIT_7,			/* 0x0b, Num7 */
	BTN_DIGIT_5,			/* 0x0c, Num5 */
	BTN_DIGIT_6,			/* 0x0d, Num6 */
	BTN_POWER,				/* 0x0e, Power */
	BTN_DIGIT_4,			/* 0x0f, Num4 */
	BTN_DIGIT_2,			/* 0x10, Num2 */
	BTN_DIGIT_3,			/* 0x11, Num3 */
	BTN_POP,				/* 0x12, POP */
	BTN_DIGIT_1,			/* 0x13, Num1 */
	BTN_RED,				/* 0x14, Red Key */
	BTN_CC,					/* 0x15, CC */
	BTN_PIPPOS,				/* 0x16, PIP POS */
	BTN_COLORSYS,			/* 0x17, Color System */
	BTN_PREV_PRG,			/* 0x18, Alternate Channel */
	BTN_FVRTCHNL,			/* 0x19, Favorite Channel */
	BTN_FAVORITE,			/* 0x1a, Favorite */
	BTN_CAPTURE,			/* 0x1b, Capture */
	BTN_CURSOR_LEFT,		/* 0x1c, Left Key */
	BTN_CURSOR_DOWN,		/* 0x1d, Down Key */
	BTN_INPUT,				/* 0x1e, Input */
	BTN_OSD,				/* 0x1f, OSD */
	BTN_SLEEP,				/* 0x40, Sleep */
	BTN_ZOOM,				/* 0x41, Zoom */
	BTN_BLUE,				/* 0x42, Blue Key */
	BTN_CE,					/* 0x43, CE */
	BTN_CURSOR_UP,			/* 0x44, Up Key */
	BTN_VOL_DOWN,			/* 0x45, Volume Down */
	BTN_INDEX,				/* 0x47, Index */
	BTN_CURSOR_RIGHT,		/* 0x48, Right Key */
	BTN_VOL_UP,				/* 0x49, Volume Up */
	BTN_CLOCK,				/* 0x4b, Clock */
	BTN_TIMER,				/* 0x4c, Timer */
	BTN_PRG_DOWN,			/* 0x4d, Channel Down */
	BTN_UPDATE,				/* 0x4f, Update */
	BTN_ADD_ERASE,			/* 0x50, ADD/Erase */
	BTN_PRG_UP,				/* 0x51, Channel Up */
	BTN_FREEZE,				/* 0x54, Freeze */
	BTN_VCHIP,				/* 0x55, V-Chip */
	BTN_PICSIZE,			/* 0x58, PIC SIZE */
	BTN_SNDEFCT,			/* 0x59, Sound Effect */
	BTN_SELECT,				/* 0x5c, Enter Key */
	BTN_MTS,				/* 0x5d, MTS/Lang */
	BTN_NONE,				/**0x60********************/ /* End of this array. */
};

static UINT32 _au4MtkKeyPattern1[] =
{
	BTN_SELECT,
	// SD
	BTN_DELAY_2S,
	BTN_DIGIT_2,
	BTN_DIGIT_3,
	BTN_DIGIT_4,
	BTN_DIGIT_5,
	BTN_DIGIT_6,
	BTN_DIGIT_5,
	BTN_DIGIT_4,
	BTN_DIGIT_3,
	BTN_DIGIT_2,
	BTN_DIGIT_1,
	//
	BTN_PRG_UP,
	BTN_PRG_UP,
	BTN_PRG_UP,
	BTN_PRG_UP,
	BTN_PRG_UP,
	BTN_PRG_UP,
	BTN_PRG_UP,
	BTN_PRG_UP,
	BTN_PRG_UP,
	BTN_PRG_DOWN,
	BTN_PRG_DOWN,
	BTN_PRG_DOWN,
	BTN_PRG_DOWN,
	BTN_PRG_DOWN,
	BTN_PRG_DOWN,
	BTN_PRG_DOWN,
	BTN_PRG_DOWN,
	BTN_PRG_DOWN,
	//
	BTN_DIGIT_2,
	BTN_PRG_UP,
	BTN_PRG_UP,
	BTN_PRG_UP,
	BTN_PRG_UP,
	BTN_CURSOR_LEFT,
	BTN_PRG_DOWN,
	BTN_PRG_DOWN,
	BTN_PRG_DOWN,
	BTN_PRG_DOWN,
	//
	BTN_DIGIT_3,
	BTN_PRG_UP,
	BTN_PRG_UP,
	BTN_PRG_DOWN,
	BTN_PRG_DOWN,
	BTN_CURSOR_UP,
	BTN_PRG_DOWN,
	BTN_PRG_DOWN,
	BTN_PRG_UP,
	BTN_PRG_UP,
	//
	BTN_DIGIT_4,
	BTN_PRG_DOWN,
	BTN_PRG_UP,
	BTN_PRG_DOWN,
	BTN_PRG_UP,
	BTN_CURSOR_DOWN,
	BTN_CURSOR_DOWN,
	BTN_CURSOR_DOWN,
	BTN_PRG_DOWN,
	BTN_PRG_UP,
	BTN_PRG_DOWN,
	BTN_PRG_UP,
	//
	BTN_DIGIT_5,
	BTN_CURSOR_UP,
	BTN_CURSOR_UP,
	BTN_CURSOR_DOWN,
	BTN_CURSOR_DOWN,
	BTN_CURSOR_LEFT,
	BTN_CURSOR_LEFT,
	BTN_CURSOR_RIGHT,
	BTN_CURSOR_RIGHT,
	//
	BTN_DIGIT_6,
	BTN_CURSOR_RIGHT,
	BTN_CURSOR_RIGHT,
	BTN_CURSOR_RIGHT,
	BTN_CURSOR_UP,
	BTN_DIGIT_2,
	BTN_DIGIT_6,
	BTN_CURSOR_RIGHT,
	BTN_CURSOR_DOWN,
	BTN_CURSOR_DOWN,
	BTN_CURSOR_RIGHT,
	BTN_DIGIT_1,
	BTN_DIGIT_6,
	BTN_CURSOR_LEFT,
	BTN_CURSOR_LEFT,
	BTN_CURSOR_LEFT,
	BTN_DIGIT_5,
	BTN_CURSOR_DOWN,
	BTN_CURSOR_LEFT,
	BTN_CURSOR_DOWN,
	BTN_SELECT,
	//
	BTN_MENU,
	BTN_CURSOR_DOWN,
	BTN_DELAY_10S,
	BTN_SELECT,
	// HD
	BTN_DELAY_2S,
	BTN_DIGIT_2,
	BTN_DIGIT_3,
	BTN_DIGIT_4,
	BTN_DIGIT_3,
	BTN_DIGIT_2,
	BTN_DIGIT_1,
	//
	BTN_PRG_UP,
	BTN_PRG_UP,
	BTN_PRG_UP,
	BTN_PRG_UP,
	BTN_PRG_DOWN,
	BTN_PRG_DOWN,
	BTN_PRG_DOWN,
	BTN_PRG_DOWN,
	//
	BTN_DIGIT_2,
	BTN_CURSOR_LEFT,
	BTN_SELECT,
	BTN_DIGIT_3,
	BTN_CURSOR_UP,
	BTN_SELECT,
	BTN_DIGIT_4,
	BTN_CURSOR_DOWN,
	BTN_CURSOR_DOWN,
	BTN_CURSOR_DOWN,
	BTN_DIGIT_3,
	//
	BTN_MENU,
	BTN_CURSOR_DOWN,
	BTN_SELECT,
	BTN_SELECT,
	// scale
	BTN_DELAY_2S,
	BTN_CURSOR_LEFT,
	BTN_CURSOR_LEFT,
	BTN_CURSOR_LEFT,
	BTN_CURSOR_UP,
	BTN_CURSOR_UP,
	BTN_CURSOR_UP,
	BTN_CURSOR_RIGHT,
	BTN_CURSOR_DOWN,
	BTN_CURSOR_RIGHT,
	BTN_CURSOR_RIGHT,
	BTN_CURSOR_DOWN,
	BTN_CURSOR_DOWN,
	//
	BTN_MENU,
	BTN_SELECT,
	BTN_CURSOR_DOWN,
	BTN_SELECT,
	// position
	BTN_DELAY_2S,
	BTN_CURSOR_LEFT,
	BTN_CURSOR_LEFT,
	BTN_CURSOR_LEFT,
	BTN_CURSOR_UP,
	BTN_CURSOR_UP,
	BTN_CURSOR_UP,
	BTN_CURSOR_RIGHT,
	BTN_CURSOR_RIGHT,
	BTN_CURSOR_RIGHT,
	BTN_CURSOR_DOWN,
	BTN_CURSOR_LEFT,
	BTN_CURSOR_DOWN,
	BTN_CURSOR_DOWN,
	BTN_CURSOR_RIGHT,
	//
	BTN_MENU,
	BTN_SELECT,
	BTN_CURSOR_DOWN,
	BTN_SELECT,
	// blending
	BTN_DELAY_2S,
	BTN_PRG_UP,
	BTN_DELAY_2S,
	BTN_PRG_UP,
	BTN_DELAY_2S,
	BTN_PRG_DOWN,
	BTN_DELAY_2S,
	BTN_PRG_DOWN,
	BTN_DELAY_2S,
	//
	BTN_MENU,
	BTN_SELECT,
	BTN_CURSOR_DOWN,
	BTN_SELECT,
	// flyin
	BTN_DELAY_2S,
	BTN_PRG_UP,
	BTN_DELAY_2S,
	BTN_PRG_UP,
	BTN_DELAY_2S,
	BTN_PRG_DOWN,
	BTN_DELAY_2S,
	BTN_PRG_DOWN,
	BTN_DELAY_2S,
	//
	BTN_MENU,
	BTN_SELECT,
	BTN_CURSOR_DOWN,
	BTN_SELECT,
	// rollover
	BTN_DELAY_2S,
	BTN_PRG_UP,
	BTN_DELAY_2S,
	BTN_PRG_UP,
	BTN_DELAY_2S,
	BTN_PRG_DOWN,
	BTN_DELAY_2S,
	BTN_PRG_DOWN,
	BTN_DELAY_2S,
	//
	BTN_MENU,
	BTN_SELECT,
	BTN_CURSOR_DOWN,
	BTN_SELECT,
	// gfx1
	BTN_DIGIT_1,
	BTN_DIGIT_2,
	BTN_DELAY_2S,
	BTN_DIGIT_3,
	BTN_DIGIT_4,
	BTN_DIGIT_5,
	BTN_DELAY_2S,
	BTN_DIGIT_6,
	BTN_DELAY_2S,
	BTN_DIGIT_7,
	BTN_DIGIT_8,
	BTN_DIGIT_9,
	BTN_DELAY_2S,
	//
	BTN_MENU,
	BTN_SELECT,
	BTN_CURSOR_DOWN,
	BTN_SELECT,
	// gfx2 - blending
	BTN_DELAY_10S,
	BTN_DELAY_10S,
	//
	BTN_MENU,
	BTN_SELECT,
	BTN_CURSOR_DOWN,
	BTN_SELECT,
	// gfx3 - bitblt
	BTN_DIGIT_1,
	BTN_DELAY_10S,
	BTN_DELAY_10S,
	BTN_DIGIT_2,
	BTN_DELAY_10S,
	BTN_DELAY_10S,
	BTN_DIGIT_3,
	BTN_DELAY_10S,
	BTN_DELAY_10S,
	BTN_DIGIT_4,
	BTN_DELAY_10S,
	BTN_DELAY_10S,
	//
	BTN_MENU,
	BTN_SELECT,
	BTN_CURSOR_DOWN,
	BTN_SELECT,
	// gfx4 - zoomin
	BTN_DELAY_10S,
	//
	BTN_MENU,
	BTN_SELECT,
	BTN_CURSOR_DOWN,
	BTN_SELECT,
	// gfx5 - zigzag
	BTN_DELAY_10S,
	BTN_DELAY_10S,
	BTN_DELAY_5S,
	//
	BTN_MENU,
	BTN_SELECT,
	BTN_CURSOR_DOWN,
	BTN_SELECT,
	//
	BTN_MENU,
	BTN_CURSOR_UP,
	BTN_CURSOR_UP,
	BTN_DELAY_5S,
};

static UINT32 _au4MtkKeyPattern2[] =
{
	BTN_PRG_UP,
};

// HD+gfx
static UINT32 _au4MtkKeyPattern3[] =
{
	BTN_SELECT,
	// HD
	BTN_DELAY_2S,
	BTN_DIGIT_2,
	BTN_DIGIT_3,
	BTN_DIGIT_4,
	BTN_DIGIT_3,
	BTN_DIGIT_2,
	BTN_DIGIT_1,
	//
	BTN_PRG_UP,
	BTN_PRG_UP,
	BTN_PRG_UP,
	BTN_PRG_UP,
	BTN_PRG_DOWN,
	BTN_PRG_DOWN,
	BTN_PRG_DOWN,
	BTN_PRG_DOWN,
	//
	BTN_DIGIT_2,
	BTN_CURSOR_LEFT,
	BTN_SELECT,
	BTN_DIGIT_3,
	BTN_CURSOR_UP,
	BTN_SELECT,
	BTN_DIGIT_4,
	BTN_CURSOR_DOWN,
	BTN_CURSOR_DOWN,
	BTN_CURSOR_DOWN,
	BTN_DIGIT_3,
	//
	BTN_MENU,
	BTN_CURSOR_DOWN,
	BTN_SELECT,
	BTN_SELECT,
	// scale
	BTN_DELAY_2S,
	BTN_CURSOR_LEFT,
	BTN_CURSOR_LEFT,
	BTN_CURSOR_LEFT,
	BTN_CURSOR_UP,
	BTN_CURSOR_UP,
	BTN_CURSOR_UP,
	BTN_CURSOR_RIGHT,
	BTN_CURSOR_DOWN,
	BTN_CURSOR_RIGHT,
	BTN_CURSOR_RIGHT,
	BTN_CURSOR_DOWN,
	BTN_CURSOR_DOWN,
	//
	BTN_MENU,
	BTN_SELECT,
	BTN_CURSOR_DOWN,
	BTN_SELECT,
	// position
	BTN_DELAY_2S,
	BTN_CURSOR_LEFT,
	BTN_CURSOR_LEFT,
	BTN_CURSOR_LEFT,
	BTN_CURSOR_UP,
	BTN_CURSOR_UP,
	BTN_CURSOR_UP,
	BTN_CURSOR_RIGHT,
	BTN_CURSOR_RIGHT,
	BTN_CURSOR_RIGHT,
	BTN_CURSOR_DOWN,
	BTN_CURSOR_LEFT,
	BTN_CURSOR_DOWN,
	BTN_CURSOR_DOWN,
	BTN_CURSOR_RIGHT,
	//
	BTN_MENU,
	BTN_SELECT,
	BTN_CURSOR_DOWN,
	BTN_SELECT,
	// blending
	BTN_DELAY_2S,
	BTN_PRG_UP,
	BTN_DELAY_2S,
	BTN_PRG_UP,
	BTN_DELAY_2S,
	BTN_PRG_DOWN,
	BTN_DELAY_2S,
	BTN_PRG_DOWN,
	BTN_DELAY_2S,
	//
	BTN_MENU,
	BTN_SELECT,
	BTN_CURSOR_DOWN,
	BTN_SELECT,
	// flyin
	BTN_DELAY_2S,
	BTN_PRG_UP,
	BTN_DELAY_2S,
	BTN_PRG_UP,
	BTN_DELAY_2S,
	BTN_PRG_DOWN,
	BTN_DELAY_2S,
	BTN_PRG_DOWN,
	BTN_DELAY_2S,
	//
	BTN_MENU,
	BTN_SELECT,
	BTN_CURSOR_DOWN,
	BTN_SELECT,
	// rollover
	BTN_DELAY_2S,
	BTN_PRG_UP,
	BTN_DELAY_2S,
	BTN_PRG_UP,
	BTN_DELAY_2S,
	BTN_PRG_DOWN,
	BTN_DELAY_2S,
	BTN_PRG_DOWN,
	BTN_DELAY_2S,
	//
	BTN_MENU,
	BTN_SELECT,
	BTN_CURSOR_DOWN,
	BTN_SELECT,
	// gfx1
	BTN_DIGIT_1,
	BTN_DIGIT_2,
	BTN_DELAY_2S,
	BTN_DIGIT_3,
	BTN_DIGIT_4,
	BTN_DIGIT_5,
	BTN_DELAY_2S,
	BTN_DIGIT_6,
	BTN_DELAY_2S,
	BTN_DIGIT_7,
	BTN_DIGIT_8,
	BTN_DIGIT_9,
	BTN_DELAY_2S,
	//
	BTN_MENU,
	BTN_SELECT,
	BTN_CURSOR_DOWN,
	BTN_SELECT,
	// gfx2 - blending
	BTN_DELAY_10S,
	BTN_DELAY_10S,
	//
	BTN_MENU,
	BTN_SELECT,
	BTN_CURSOR_DOWN,
	BTN_SELECT,
	// gfx3 - bitblt
	BTN_DIGIT_1,
	BTN_DELAY_10S,
	BTN_DELAY_10S,
	BTN_DIGIT_2,
	BTN_DELAY_10S,
	BTN_DELAY_10S,
	BTN_DIGIT_3,
	BTN_DELAY_10S,
	BTN_DELAY_10S,
	BTN_DIGIT_4,
	BTN_DELAY_10S,
	BTN_DELAY_10S,
	//
	BTN_MENU,
	BTN_SELECT,
	BTN_CURSOR_DOWN,
	BTN_SELECT,
	// gfx4 - zoomin
	BTN_DELAY_10S,
	//
	BTN_MENU,
	BTN_SELECT,
	BTN_CURSOR_DOWN,
	BTN_SELECT,
	// gfx5 - zigzag
	BTN_DELAY_10S,
	BTN_DELAY_10S,
	BTN_DELAY_5S,
	//
	BTN_MENU,
	BTN_SELECT,
	BTN_CURSOR_DOWN,
	BTN_SELECT,
	//
	BTN_MENU,
	BTN_CURSOR_UP,
};

// SD+gfx
static UINT32 _au4MtkKeyPattern4[] =
{
	BTN_SELECT,
	// SD
	BTN_DELAY_2S,
	BTN_DIGIT_2,
	BTN_DIGIT_3,
	BTN_DIGIT_4,
	BTN_DIGIT_5,
	BTN_DIGIT_6,
	BTN_DIGIT_5,
	BTN_DIGIT_4,
	BTN_DIGIT_3,
	BTN_DIGIT_2,
	BTN_DIGIT_1,
	//
	BTN_PRG_UP,
	BTN_PRG_UP,
	BTN_PRG_UP,
	BTN_PRG_UP,
	BTN_PRG_UP,
	BTN_PRG_UP,
	BTN_PRG_UP,
	BTN_PRG_UP,
	BTN_PRG_UP,
	BTN_PRG_DOWN,
	BTN_PRG_DOWN,
	BTN_PRG_DOWN,
	BTN_PRG_DOWN,
	BTN_PRG_DOWN,
	BTN_PRG_DOWN,
	BTN_PRG_DOWN,
	BTN_PRG_DOWN,
	BTN_PRG_DOWN,
	//
	BTN_DIGIT_2,
	BTN_PRG_UP,
	BTN_PRG_UP,
	BTN_PRG_UP,
	BTN_PRG_UP,
	BTN_CURSOR_LEFT,
	BTN_PRG_DOWN,
	BTN_PRG_DOWN,
	BTN_PRG_DOWN,
	BTN_PRG_DOWN,
	//
	BTN_DIGIT_3,
	BTN_PRG_UP,
	BTN_PRG_UP,
	BTN_PRG_DOWN,
	BTN_PRG_DOWN,
	BTN_CURSOR_UP,
	BTN_PRG_DOWN,
	BTN_PRG_DOWN,
	BTN_PRG_UP,
	BTN_PRG_UP,
	//
	BTN_DIGIT_4,
	BTN_PRG_DOWN,
	BTN_PRG_UP,
	BTN_PRG_DOWN,
	BTN_PRG_UP,
	BTN_CURSOR_DOWN,
	BTN_CURSOR_DOWN,
	BTN_CURSOR_DOWN,
	BTN_PRG_DOWN,
	BTN_PRG_UP,
	BTN_PRG_DOWN,
	BTN_PRG_UP,
	//
	BTN_DIGIT_5,
	BTN_CURSOR_UP,
	BTN_CURSOR_UP,
	BTN_CURSOR_DOWN,
	BTN_CURSOR_DOWN,
	BTN_CURSOR_LEFT,
	BTN_CURSOR_LEFT,
	BTN_CURSOR_RIGHT,
	BTN_CURSOR_RIGHT,
	//
	BTN_DIGIT_6,
	BTN_CURSOR_RIGHT,
	BTN_CURSOR_RIGHT,
	BTN_CURSOR_RIGHT,
	BTN_CURSOR_UP,
	BTN_DIGIT_2,
	BTN_DIGIT_6,
	BTN_CURSOR_RIGHT,
	BTN_CURSOR_DOWN,
	BTN_CURSOR_DOWN,
	BTN_CURSOR_RIGHT,
	BTN_DIGIT_1,
	BTN_DIGIT_6,
	BTN_CURSOR_LEFT,
	BTN_CURSOR_LEFT,
	BTN_CURSOR_LEFT,
	BTN_DIGIT_5,
	BTN_CURSOR_DOWN,
	BTN_CURSOR_LEFT,
	BTN_CURSOR_DOWN,
	BTN_SELECT,
	//
	BTN_MENU,
	BTN_CURSOR_DOWN,
	BTN_CURSOR_DOWN,
	BTN_SELECT,
	BTN_CURSOR_DOWN,
	BTN_CURSOR_DOWN,
	BTN_CURSOR_DOWN,
	BTN_CURSOR_DOWN,
	BTN_CURSOR_DOWN,
	BTN_SELECT,
	// gfx1
	BTN_DIGIT_1,
	BTN_DIGIT_2,
	BTN_DELAY_2S,
	BTN_DIGIT_3,
	BTN_DIGIT_4,
	BTN_DIGIT_5,
	BTN_DELAY_2S,
	BTN_DIGIT_6,
	BTN_DELAY_2S,
	BTN_DIGIT_7,
	BTN_DIGIT_8,
	BTN_DIGIT_9,
	BTN_DELAY_2S,
	//
	BTN_MENU,
	BTN_SELECT,
	BTN_CURSOR_DOWN,
	BTN_SELECT,
	// gfx2 - blending
	BTN_DELAY_10S,
	BTN_DELAY_10S,
	//
	BTN_MENU,
	BTN_SELECT,
	BTN_CURSOR_DOWN,
	BTN_SELECT,
	// gfx3 - bitblt
	BTN_DIGIT_1,
	BTN_DELAY_10S,
	BTN_DELAY_10S,
	BTN_DIGIT_2,
	BTN_DELAY_10S,
	BTN_DELAY_10S,
	BTN_DIGIT_3,
	BTN_DELAY_10S,
	BTN_DELAY_10S,
	BTN_DIGIT_4,
	BTN_DELAY_10S,
	BTN_DELAY_10S,
	//
	BTN_MENU,
	BTN_SELECT,
	BTN_CURSOR_DOWN,
	BTN_SELECT,
	// gfx4 - zoomin
	BTN_DELAY_10S,
	//
	BTN_MENU,
	BTN_SELECT,
	BTN_CURSOR_DOWN,
	BTN_SELECT,
	// gfx5 - zigzag
	BTN_DELAY_10S,
	BTN_DELAY_10S,
	BTN_DELAY_5S,
	//
	BTN_MENU,
	BTN_SELECT,
	BTN_CURSOR_DOWN,
	BTN_SELECT,
	//
	BTN_MENU,
	BTN_CURSOR_UP,
	BTN_CURSOR_UP,
};

static UINT32* _au4MtkKeyPattern[] =
{
	_au4MtkKeyPattern0,
	_au4MtkKeyPattern1,
	_au4MtkKeyPattern2,
	_au4MtkKeyPattern3,
	_au4MtkKeyPattern4,
};

static UINT32 _au4TotalPattern[] =
{
	sizeof(_au4MtkKeyPattern0)/sizeof(UINT32),
	sizeof(_au4MtkKeyPattern1)/sizeof(UINT32),
	sizeof(_au4MtkKeyPattern2)/sizeof(UINT32),
	sizeof(_au4MtkKeyPattern3)/sizeof(UINT32),
	sizeof(_au4MtkKeyPattern4)/sizeof(UINT32),
};
