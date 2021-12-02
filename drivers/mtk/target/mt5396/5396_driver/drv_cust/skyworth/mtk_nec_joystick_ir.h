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
#ifndef TPV_JOYSTICK_IR_H
#define TPV_JOYSTICK_IR_H

#include "u_irrc_btn_def.h"
#include "mtk/mtk_ir_nec_tbl.h"
// #include "mtk/mtk_ir_rc5_code_tbl.h"
//#include "mtk/mtk_ir_rc5_func_tbl.h"
#ifdef DEFINE_IS_LOG
#undef DEFINE_IS_LOG
#endif
#ifdef CC_MTK_LOADER
#define DEFINE_IS_LOG(level, fmt...)   Printf(fmt)
#else /* CC_MTK_LOADER */
#define DEFINE_IS_LOG	IR_IsLog
#endif /* CC_MTK_LOADER */
#include "x_debug.h"
#include "ir_if.h"
//#include "ir_reg.h"
static const KEYUNIT_T _arTPV_TvKeyMap[] =
{
	{ 0x01, BTN_POWER },			/* 0x01fe, Power */
	{ 0x02, BTN_PRG_INFO }, 		/* 0x02fd, OSD */
	{ 0x03, BTN_P_EFFECT }, 		/* 0x03fc, Smart picture */ 
	{ 0x04, BTN_MUTE }, 			/* 0x04fb, Mute */
	{ 0x05, BTN_COMPOSITE },		/* 0x05fa, Direct change source to Video Group's source. * */
	{ 0x06, BTN_HDMI},			    /* 0x06f9, Direct change source to HDMI * */
    { 0x07, BTN_EXIT },             /* 0x07f8, Exit */	
	{ 0x08, BTN_INPUT_SRC },		/* 0x08f7, Input */
	{ 0x09, BTN_S_EFFECT }, 		/* 0x09f6, Smart sound * */
	{ 0x0a, BTN_MENU }, 			/* 0x0af5, Menu */
	{ 0x0b, BTN_CURSOR_UP },		/* 0x0bf4, Up Key */
	{ 0x0c, BTN_VOL_UP },			/* 0x0cf3, Volume Up */
	{ 0x0d, BTN_SELECT },			/* 0x0df2, Enter Key */
	{ 0x0f, BTN_CURSOR_DOWN },		/* 0x0ff0, Down Key */
	{ 0x10, BTN_VOL_DOWN }, 		/* 0x10ef, Volume Down */
	{ 0x13, BTN_SLEEP },			/* 0x13ec, Sleep */
	{ 0x14, BTN_ASPECT },			/* 0x14eb, Picture format */   
	{ 0x15, BTN_DIGIT_1 },			/* 0x15ea, Num1 */
	{ 0x16, BTN_DIGIT_2 },			/* 0x16e9, Num2 */
	{ 0x17, BTN_DIGIT_3 },			/* 0x17e8, Num3 */
	{ 0x18, BTN_PRG_UP },			/* 0x18e7, Channel Up */
	{ 0x19, BTN_DIGIT_4 },			/* 0x19e6, Num4 */
	{ 0x1a, BTN_DIGIT_5 },			/* 0x1ae5, Num5 */
	{ 0x1b, BTN_DIGIT_6 },			/* 0x1be4, Num6 */
	{ 0x1c, BTN_PRG_DOWN }, 		/* 0x1ce3, Channel Down */
	{ 0x1d, BTN_DIGIT_7 },			/* 0x1de2, Num7 */
	{ 0x1e, BTN_DIGIT_8 },			/* 0x1ee1, Num8 */
	{ 0x1f, BTN_DIGIT_9 },			/* 0x1fe0, Num9 */
	
	{ 0x20, BTN_KB_SMALL_A},			/* 0x1fe0, Num9 */	
	{ 0x21, BTN_KB_SMALL_B},			/* 0x1fe0, Num9 */	
	{ 0x22, BTN_KB_SMALL_C},			/* 0x1fe0, Num9 */	
	{ 0x23, BTN_KB_SMALL_D},			/* 0x1fe0, Num9 */	
	{ 0x24, BTN_KB_SMALL_E},			/* 0x1fe0, Num9 */	
	{ 0x25, BTN_KB_SMALL_F},			/* 0x1fe0, Num9 */	
	{ 0x26, BTN_KB_SMALL_G},			/* 0x1fe0, Num9 */	
	{ 0x27, BTN_KB_SMALL_H},			/* 0x1fe0, Num9 */	
	{ 0x28, BTN_KB_SMALL_I},			/* 0x1fe0, Num9 */	
	{ 0x29, BTN_KB_SMALL_J},			/* 0x1fe0, Num9 */	
	{ 0x2A, BTN_KB_SMALL_K},			/* 0x1fe0, Num9 */	
	{ 0x2B, BTN_KB_SMALL_L},			/* 0x1fe0, Num9 */	
	{ 0x2C, BTN_KB_SMALL_M},			/* 0x1fe0, Num9 */	
	{ 0x2D, BTN_KB_SMALL_N},			/* 0x1fe0, Num9 */	
	{ 0x2E, BTN_KB_SMALL_O},			/* 0x1fe0, Num9 */	
	{ 0x2F, BTN_KB_SMALL_P},			/* 0x1fe0, Num9 */	
	{ 0x30, BTN_KB_SMALL_Q},			/* 0x1fe0, Num9 */	
	{ 0x31, BTN_KB_SMALL_R},			/* 0x1fe0, Num9 */	
	{ 0x32, BTN_KB_SMALL_S},			/* 0x1fe0, Num9 */	
	{ 0x33, BTN_KB_SMALL_T},			/* 0x1fe0, Num9 */	
	{ 0x34, BTN_KB_SMALL_U},			/* 0x1fe0, Num9 */	
	{ 0x35, BTN_KB_SMALL_V},			/* 0x1fe0, Num9 */	
	{ 0x36, BTN_KB_SMALL_W},			/* 0x1fe0, Num9 */	
	{ 0x37, BTN_KB_SMALL_X},			/* 0x1fe0, Num9 */	
	{ 0x38, BTN_KB_SMALL_Y},			/* 0x1fe0, Num9 */	
	{ 0x39, BTN_KB_SMALL_Z},			/* 0x1fe0, Num9 */	
	
	{ 0x40, BTN_MTS },				/* 0x40bf, MTS/Lang */
	{ 0x41, BTN_DIGIT_0 },			/* 0x41be, Num0 */
	{ 0x44, BTN_PREV_PRG }, 		/* 0x44bb, Alternate Channel */
    { 0x46, BTN_TV },               /* 0x46b9, Direct change source to TV * */	
	{ 0x47, BTN_COMPONENT },		/* 0x47b8 , Direct change source to YPBPR * */
	{ 0x49, BTN_CURSOR_LEFT },		/* 0x49b6, Left Key */
	{ 0x4a, BTN_CURSOR_RIGHT }, 	/* 0x4ab5, Right Key */
	{ 0x4b, BTN_SUBPAGE },			/* 0x4bb4, Teletext - Subpage */
	{ 0x4c, BTN_RED },				/* 0x4cb3, Red Key */
	{ 0x4d, BTN_GREEN },			/* 0x4db2, Green Key */
	{ 0x4e, BTN_YELLOW },			/* 0x4eb1, Yellow Key */
	{ 0x4f, BTN_BLUE }, 			/* 0x4fb0, Blue Key */
	{ 0x50, BTN_TTX },				/* 0x50af, Teletext on/off */
	{ 0x52, BTN_SIZE }, 			/* 0x52ad, Teletext - Size * */
	{ 0x54, BTN_ROOT_MENU },		/* 0x54ab, Teletext - Index */
	{ 0x55, BTN_SUB_TITLE },		/* 0x55aa, Teletext - Subtitle * */
	{ 0x56, BTN_FREEZE },			/* 0x56a9, Teletext - Hold * */
	{ 0x57, BTN_REVEAL}, 			/* 0x57a8, Teletext - Reveal * */
	{ 0x58, BTN_EPG },				/* 0x58a7, EPG */
	{ 0x59, BTN_ROOT_MENU },		/* NET button!!! */
	{ 0x5a, BTN_RECORD},
	{ 0x5b, BTN_PAUSE},	
	{ 0x5c, BTN_FR },		        /* 0x5ca3, Previous */ 
	{ 0x5d, BTN_STOP },			/* 0x5da2, STOP, FAV_CH */  	
	{ 0x5e, BTN_PLAY },         /* 0x5ea1, DVD PLAY_PAUSE, Favorite */
	{ 0x5f, BTN_FF },			    /* 0x5fa0, Next */  	
	
	{ 0x60, BTN_KB_SHIFT},
	{ 0x64, BTN_KB_CAPITAL_A},			/* 0x1fe0, Num9 */	
	{ 0x65, BTN_KB_CAPITAL_B},			/* 0x1fe0, Num9 */	
	{ 0x66, BTN_KB_CAPITAL_C},			/* 0x1fe0, Num9 */	
	{ 0x67, BTN_KB_CAPITAL_D},			/* 0x1fe0, Num9 */	
	{ 0x68, BTN_KB_CAPITAL_E},			/* 0x1fe0, Num9 */	
	{ 0x69, BTN_KB_CAPITAL_F},			/* 0x1fe0, Num9 */	
	{ 0x6A, BTN_KB_CAPITAL_G},			/* 0x1fe0, Num9 */	
	{ 0x6B, BTN_KB_CAPITAL_H},			/* 0x1fe0, Num9 */	
	{ 0x6C, BTN_KB_CAPITAL_I},			/* 0x1fe0, Num9 */	
	{ 0x6D, BTN_KB_CAPITAL_J},			/* 0x1fe0, Num9 */	
	{ 0x6E, BTN_KB_CAPITAL_K},			/* 0x1fe0, Num9 */	
	{ 0x6F, BTN_KB_CAPITAL_L},			/* 0x1fe0, Num9 */	
	{ 0x70, BTN_KB_CAPITAL_M},			/* 0x1fe0, Num9 */	
	{ 0x71, BTN_KB_CAPITAL_N},			/* 0x1fe0, Num9 */	
	{ 0x72, BTN_KB_CAPITAL_O},			/* 0x1fe0, Num9 */	
	{ 0x73, BTN_KB_CAPITAL_P},			/* 0x1fe0, Num9 */	
	{ 0x74, BTN_KB_CAPITAL_Q},			/* 0x1fe0, Num9 */	
	{ 0x75, BTN_KB_CAPITAL_R},			/* 0x1fe0, Num9 */	
	{ 0x76, BTN_KB_CAPITAL_S},			/* 0x1fe0, Num9 */	
	{ 0x77, BTN_KB_CAPITAL_T},			/* 0x1fe0, Num9 */	
	{ 0x78, BTN_KB_CAPITAL_U},			/* 0x1fe0, Num9 */	
	{ 0x79, BTN_KB_CAPITAL_V},			/* 0x1fe0, Num9 */	
	{ 0x7A, BTN_KB_CAPITAL_W},			/* 0x1fe0, Num9 */	
	{ 0x7B, BTN_KB_CAPITAL_X},			/* 0x1fe0, Num9 */	
	{ 0x7C, BTN_KB_CAPITAL_Y},			/* 0x1fe0, Num9 */	
	{ 0x7D, BTN_KB_CAPITAL_Z},			/* 0x1fe0, Num9 */	
	{ 0x80, BTN_KB_ALT},
	{ 0x81, BTN_KB_CAPS_LOCK},	
	{ 0x84, BTN_KB_1},	
	{ 0x85, BTN_KB_2},	
	{ 0x86, BTN_KB_3},				
	{ 0x87, BTN_KB_4},	
	{ 0x88, BTN_KB_5},	
	{ 0x89, BTN_KB_6},	
	{ 0x8a, BTN_KB_7},	
	{ 0x8b, BTN_KB_8},						
	{ 0x8c, BTN_KB_9},		
	{ 0x8d, BTN_KB_0},			
    { 0x8E, BTN_KB_EXCLAMATION_MARK},
    { 0x8F, BTN_KB_NUMBER_SIGN},
    { 0x90, BTN_KB_DOLLAR_SIGN},
    { 0x91, BTN_KB_PERCENT_SIGN},
    { 0x92, BTN_KB_CIRCUMFLEX_ACCENT},
    { 0x93, BTN_KB_AMPERSAND},
    { 0x94, BTN_KB_ASTERISK},
    { 0x95, BTN_KB_TILDE},
    { 0x96, BTN_KB_QUESTION_MARK},
    { 0x97, BTN_KB_DELETE},
    { 0xA4, BTN_KB_MINUS_SIGN},
    { 0xA5, BTN_KB_LESS_THAN_SIGN},
    { 0xA6, BTN_KB_GREATER_THAN_SIGN},
    { 0xA7, BTN_KB_SQUARE_BRACKET_LEFT},
    { 0xA8, BTN_KB_SQUARE_BRACKET_RIGHT},
    { 0xA9, BTN_KB_PARENTHESIS_LEFT},            
    { 0xAA, BTN_KB_PARENTHESIS_RIGHT},
    { 0xAB, BTN_KB_BACKSLASH},
    { 0xAC, BTN_KB_PLUS_SIGN},
    { 0xAD, BTN_KB_COLON},
    { 0xAE, BTN_KB_APOSTROPHE},
    { 0xAF, BTN_KB_QUOTATION},
    { 0xB0, BTN_KB_UNDERSCORE},
    { 0xB1, BTN_KB_SPACE},
    { 0xB2, BTN_KB_BACKSPACE},
    { 0xB3, BTN_KB_SLASH},
    { 0xB4, BTN_KB_RETURN},
    { 0xB5, BTN_KB_TAB},
    { 0xB6, BTN_KB_EQUALS_SIGN},
    { 0xB7, BTN_KB_SEMICOLON},
    { 0xB8, BTN_KB_COMMA},
    { 0xB9, BTN_KB_PERIOD},
    { 0xBA, BTN_KB_AT},                                
        
};

#ifndef KEY_ARRAY
#define KEY_ARRAY
static const KEYARRAY_T _arKeyArray[] =
{
    { 0xbd00, (sizeof(_arTPV_TvKeyMap)/sizeof(KEYUNIT_T)), _arTPV_TvKeyMap },        
    { 0xe31c, (sizeof(_arMtkTvBigKeyMap)/sizeof(KEYUNIT_T)), _arMtkTvBigKeyMap }, 
};

#endif /* KEY_ARRAY */
//----------------------------------------------------------------------------
// IRRX customization
//----------------------------------------------------------------------------
#define DEFAULT_IRRX_TYPE               (IRRX_USR_TYPE)
#define IRRX_USR_KEYMAP_ARRAY           (_arKeyArray)
#define IRRX_USR_MAPARRAY_SIZE          (sizeof(_arKeyArray)/sizeof(KEYARRAY_T))
#define IRRX_NEC_KEYMAP_ARRAY	        (NULL)
#define IRRX_NEC_MAPARRAY_SIZE	        (0)
#define IRRX_RC5_KEYMAP_ARRAY           (NULL)
#define IRRX_RC5_KEYMAP_SIZE            (0)

//----------------------------------------------------------------------------
// IRRX Matsushita customization
//----------------------------------------------------------------------------
//The following is porting of Funai factory IR protocol
#define IRRX_XFER_USR_TO_CRYSTAL_FUNC	_XferTpvJoystickIrToCrystal
#define IRRX_SET_USR_WAKEUP_KEY_FUNC		_SetTpvJoystickIrWakeupKey
#define IRRX_USR_CONFIG             (0x4221) //(0x4221)
#define IRRX_USR_SAPERIOD           (0x0075)
#define IRRX_USR_THRESHOLD          (0x0001) //(0x0001)
//#define IRRX_CLK_SEL                (5)         //(6)
//#define IRRX_USR_PULSE1             (0xd)
#define IRRX_REPEAT_PULSE_UP    8// (2250 / 227) - 1==> 7
#define IRRX_REPEAT_PULSE_DOWN  5//

#define KEY_EQUAL(x, y) ((x)==(y))
#define MAX_KEY_ARRAY IRRX_USR_MAPARRAY_SIZE

#ifndef CC_MTK_LOADER 
//-----------------------------------------------------------------------------
// static functions
//-----------------------------------------------------------------------------
static UINT32 _CrystalLookup(const KEYUNIT_T *prKeyMap,
    UINT32 u4Size, UINT32 u4Data)
{
    UINT32 u4Min, u4Max, u4Avg;

    if (prKeyMap == NULL)
    {
        return BTN_INVALID;
    }

    if(u4Size == 1)
    {
        if (KEY_EQUAL(prKeyMap[0].u4Key, u4Data))
        {
            return prKeyMap[0].u4Crystal;
        }        
        else
        {
            return BTN_INVALID;
        }
    }

    u4Min = 0;
    u4Max = u4Size - 1;

    while (u4Max > u4Min)
    {
        u4Avg = (u4Max + u4Min) / 2;

        if (KEY_EQUAL(prKeyMap[u4Min].u4Key, u4Data))
        {
            return prKeyMap[u4Min].u4Crystal;
        }

        if (KEY_EQUAL(prKeyMap[u4Max].u4Key, u4Data))
        {
            return prKeyMap[u4Max].u4Crystal;
        }

        if (KEY_EQUAL(prKeyMap[u4Avg].u4Key, u4Data))
        {
            return prKeyMap[u4Avg].u4Crystal;
        }
        else if (prKeyMap[u4Avg].u4Key > u4Data)
        {
            u4Max = u4Avg;
        }
        else if (prKeyMap[u4Avg].u4Key < u4Data)
        {
            if (u4Min == u4Avg)
            {
                u4Max = u4Avg;
            }
            else
            {
                u4Min = u4Avg;
            }
        }
    }
    return BTN_INVALID;
}

static UINT32 _GetCrystalKey(UINT32 u4GroupId, UINT8 u1Data)
{
    UINT32 i;

    if (_arKeyArray==NULL)
    {
        return BTN_INVALID;
    }

    for (i=0; i<MAX_KEY_ARRAY; i++)
    {
        if (u4GroupId == _arKeyArray[i].u4GroupId)
        {
            return _CrystalLookup(_arKeyArray[i].prKeyMap,
                                _arKeyArray[i].u4Size, (UINT32)u1Data);
        }
    }

    return BTN_INVALID;
}


//-----------------------------------------------------------------------------
/** _XferTpvJoystickIrToCrystal() the IRRX data to Crystal IRRX key code map
 *      function.
 *  @param u4Info the IRRX received related attributes of the current data.
 *  @param pu1Data the IRRX received and current data.
 *  @return the Crystal button key. BTN_NONE is no key.
 */
//-----------------------------------------------------------------------------
// The IRRX key settings
static UINT32 _u4_1stPulse = 0xe;
static UINT32 _u4_2ndPulse = 0;
static UINT32 _u4_3rdPulse = 0;
static UINT32 _u4IrRxRepeatDuration = 200;
static IRRX_RAW_DATA_T _rCurrBtnRaw;
#endif
#ifdef CC_MTK_LOADER
#define IRRX_NON_OSAI
#endif /* CC_MTK_LOADER */

static UINT32 _XferTpvJoystickIrToCrystal(UINT32 u4Info, const UINT8 *pu1Data, IRRX_RAW_DATA_T *prRaw)
{
#ifdef CC_MTK_LOADER        
    UNUSED(u4Info);
    UNUSED(pu1Data);
    UNUSED(prRaw);
    return BTN_NONE;
#else    
    static UINT32 u4PrevKey = BTN_NONE;
#ifndef IRRX_NON_OSAI
    static HAL_TIME_T rPrevTime = { 0, 0 };
    HAL_TIME_T rTime, rDelta;
#endif /* IRRX_NON_OSAI */
    UINT32 u4GrpId, u4BitCnt, u4Key;

    u4BitCnt = INFO_TO_BITCNT(u4Info);

    /* Check empty data. */
    if ((u4BitCnt==0) || (pu1Data==NULL))
    {
        LOG(9, "[Joysitck ir](%d) BTN_NONE\n", __LINE__);
        return BTN_NONE;
    }

#ifdef IRRX_NON_OSAI
    UNUSED(u4PrevKey);
#endif /* IRRX_NON_OSAI */

    /* Check repeat key. */
    if (u4BitCnt==1)
    {
#ifndef IRRX_NON_OSAI        
        if (
            (INFO_TO_1STPULSE(u4Info) > DRVCUST_InitGet(eIrrxRepeatPulseDown)) &&
            (INFO_TO_1STPULSE(u4Info) < DRVCUST_InitGet(eIrrxRepeatPulseUp)) &&
            (INFO_TO_2NDPULSE(u4Info) == 0) &&
            (INFO_TO_3RDPULSE(u4Info) == 0) &&
            (u4PrevKey != BTN_IR_MOUSE_MOVE)
            )
        {
            HAL_GetTime(&rTime);
            HAL_GetDeltaTime(&rDelta, &rPrevTime, &rTime);

            if ((rDelta.u4Seconds > 0) || 
                (rDelta.u4Micros > (1000*_u4IrRxRepeatDuration)))
            {
                LOG(9, "[Joysitck btn repeat](%d) return BTN_NONE, PrevKey 0x%8x\n", __LINE__, u4PrevKey);
                return BTN_NONE;
            }
            HAL_GetTime(&rPrevTime);

            prRaw->u4Len = _rCurrBtnRaw.u4Len ;
            prRaw->u1Protocol = IRRX_RAW_DATA_NONE;
            x_memcpy((void*)prRaw->au1Data, (void*)_rCurrBtnRaw.au1Data, prRaw->u4Len);
            LOG(9, "[Joysitck btn repeat](%d) REPEAT Frame, send PrevKey 0x%8x\n",__LINE__, u4PrevKey);
            return u4PrevKey;
        }
        else
        {
            LOG(9, "[Joysitck btn repeat](%d) return BTN_NONE, PrevKey 0x%8x\n", __LINE__, u4PrevKey);
            return BTN_NONE;
        }
#else /* IRRX_NON_OSAI */
        return BTN_NONE;
#endif /* IRRX_NON_OSAI */        
    }
    
    if((u4BitCnt == 33) && ((((UINT8)pu1Data[3]) + ((UINT8)pu1Data[2])) != 0xff))
    {
/*
X value is store at byte2, Y vlaue is stored at byte3 on this remoter, and Z value is not supported,
however turnkey IOM take byte1 as X value, byte2 as Y value and byte3 as Z value
*/
        u4Key = BTN_IR_MOUSE_MOVE;
        u4PrevKey = u4Key;        
        prRaw->u4Len = 4;
        prRaw->u1Protocol = IRRX_RAW_DATA_NONE;
        x_memcpy((void*)prRaw->au1Data, (void*)pu1Data, 4);
        prRaw->au1Data[1] = prRaw->au1Data[2]; //to support data struct of IOM
        prRaw->au1Data[2] = prRaw->au1Data[3]; //to support data struct of IOM        
        prRaw->au1Data[3] = 0; //to support data struct of IOM                
//mouse cursor on mtk browser will move up with negative Y, and move down with postive Y
        prRaw->au1Data[2] = 0xff - ((UINT8)(prRaw->au1Data[2]));      //~Y  
        printf("[IRRX]Joysitck ir key is: 0x%02x (x: 0x%2x, y: 0x%2x)\n",u4Key, pu1Data[1], pu1Data[2]);
        return u4Key;
    }

    /* Check invalid pulse. */
    if ((u4BitCnt!=33) ||
            ((_u4_1stPulse != 0xff) &&
             (INFO_TO_1STPULSE(u4Info) != (_u4_1stPulse-1)) &&
             (INFO_TO_1STPULSE(u4Info) != (_u4_1stPulse)) &&
             (INFO_TO_1STPULSE(u4Info) != (_u4_1stPulse+1))) ||
            ((_u4_2ndPulse != 0xff) &&
             (INFO_TO_2NDPULSE(u4Info) != (_u4_2ndPulse-1)) &&
             (INFO_TO_2NDPULSE(u4Info) != (_u4_2ndPulse)) &&
             (INFO_TO_2NDPULSE(u4Info) != (_u4_2ndPulse+1)))  ||
            ((_u4_3rdPulse != 0xff) &&
             (INFO_TO_3RDPULSE(u4Info) != (_u4_3rdPulse-1)) &&
             (INFO_TO_3RDPULSE(u4Info) != (_u4_3rdPulse)) &&
             (INFO_TO_3RDPULSE(u4Info) != (_u4_3rdPulse+1))))
    {
        LOG(9, "[Joysitck btn](%d) BTN_NONE\n", __LINE__);        
        return BTN_NONE;
    }

    u4GrpId = pu1Data[1];
    u4GrpId = (u4GrpId << 8) + pu1Data[0];

    /* Check invalid key. */
    if ((pu1Data[2]+pu1Data[3])!=0xff)
    {
        LOG(9, "[Joysitck btn](%d) BTN_NONE\n", __LINE__);
        return BTN_NONE;
    }

    /* Here, pu1Data[2] is the key of MTKDVD remote controller. */
    LOG(9, "CustomCode is 0x%4x, KeyCode is 0x%02x\n",u4GrpId, pu1Data[2]);

    u4Key = _GetCrystalKey(u4GrpId, pu1Data[2]);
#ifndef IRRX_NON_OSAI
    HAL_GetTime(&rPrevTime);
#endif /* IRRX_NON_OSAI */
    u4PrevKey = u4Key;

    prRaw->u4Len = 4;
    prRaw->u1Protocol = IRRX_RAW_DATA_NEC;
    x_memcpy((void*)prRaw->au1Data, (void*)pu1Data, 4);
    
    LOG(7, "[Joysitck btn] raw:0x%8x\n", (UINT32)*(prRaw->au1Data));
    
    _rCurrBtnRaw.u1Protocol = prRaw->u1Protocol;
    _rCurrBtnRaw.u4Len = prRaw->u4Len;
    x_memcpy(_rCurrBtnRaw.au1Data, prRaw->au1Data, prRaw->u4Len);    

    return u4Key;
#endif    
}

//-----------------------------------------------------------------------------
/** _SetTpvJoystickIrWakeupKey() to set Funai wakeup key
 *  @param
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static void _SetTpvJoystickIrWakeupKey(void)
{
    UINT32 i, j, u4KeyNum, u4Val;

    if (_arKeyArray==NULL)
    {
        LOG(1, "NEC IR key array is not found.\n");
        return;
    }

    u4KeyNum = 0;
    vIO32Write4B(PDWNC_IREXP_EN, IREXP_BIT_EN); //PDWNC_WRITE32(IREXP_EN_REG, IREXP_BIT_EN);
    vIO32Write4B(PDWNC_EXP_BCNT, 0x21);
    vIO32Write4B(PDWNC_ENEXP_IRL, 0);    
/*
    if(DRVCUST_InitGet(eIrrxFlagUserDefineWakeup) && _arRC56KeyMapArray != NULL)
    {      
        vIO32Write4B(PDWNC_ENEXP_IRM, 0xffffffff);//PDWNC_WRITE32(IREXP_M_BITMASK_REG, 0xffffffff);
        for (i=0; i<_u4RC56KeyMapSize; i++)
        {
            for (j=0; j<_arRC56KeyMapArray[i].u4Size; j++) // u4GroupId, prKeyMap
            {
                if ((u4KeyNum < MAX_EXP_KEY_NUM) && (_arRC56KeyMapArray[i].prKeyMap[j].u1Type == WAKEUP_KEY_TYPE_NEC))
                {
                    u4Val = u4IO32Read4B(PDWNC_IREXP_EN);
                    u4Val |= IREXP_EN(u4KeyNum);
                    vIO32Write4B(PDWNC_IREXP_EN, u4Val);//PDWNC_WRITE32(IREXP_EN_REG, u4Val);
                    u4Val = (_arRC56KeyMapArray[i].u4GroupId) |
                            (_arRC56KeyMapArray[i].prKeyMap[j].u4Key << 16) |
                            ((0xff - _arRC56KeyMapArray[i].prKeyMap[j].u4Key) << 24);
                    vIO32Write4B(PDWNC_EXP_IRM0 + (8*u4KeyNum), u4Val);
                    vIO32Write4B(PDWNC_EXP_IRL0 + (8*u4KeyNum), 0);
                    u4KeyNum++;
                }
            }
        }    
        return;
    }
*/    
    if(DRVCUST_InitGet(eIrrxFlagAllWakeup))
    {
        vIO32Write4B(PDWNC_ENEXP_IRM, 0x0000ffff);

        for (i=0; i < MAX_KEY_ARRAY ; i++)
        {
            u4Val = u4IO32Read4B(PDWNC_IREXP_EN);
            u4Val |= IREXP_EN(i);
            vIO32Write4B(PDWNC_IREXP_EN, u4Val);    

            u4Val = _arKeyArray[i].u4GroupId;     
            vIO32Write4B(PDWNC_EXP_IRM0 + (8*i), u4Val);
            vIO32Write4B(PDWNC_EXP_IRL0 + (8*i), 0);    
        }
    }
    else
    {
        vIO32Write4B(PDWNC_ENEXP_IRM, 0xffffffff);
        for (i=0; i<MAX_KEY_ARRAY; i++)
        {
            for (j=0; j<_arKeyArray[i].u4Size; j++) // u4GroupId, prKeyMap
            {
                if ((u4KeyNum < MAX_EXP_KEY_NUM) && (_arKeyArray[i].prKeyMap[j].u4Crystal == BTN_POWER))
                {
                    u4Val = u4IO32Read4B(PDWNC_IREXP_EN);
                    u4Val |= IREXP_EN(u4KeyNum);
                    vIO32Write4B(PDWNC_IREXP_EN, u4Val);
                    u4Val = (_arKeyArray[i].u4GroupId) |
                            (_arKeyArray[i].prKeyMap[j].u4Key << 16) |
                            ((0xff - _arKeyArray[i].prKeyMap[j].u4Key) << 24);
                    vIO32Write4B(PDWNC_EXP_IRM0 + (8*u4KeyNum), u4Val);
                    vIO32Write4B(PDWNC_EXP_IRL0 + (8*u4KeyNum), 0);
                    u4KeyNum++;
                }
            }
        }
        for (i=0; i<MAX_KEY_ARRAY; i++)
        {
            for (j=0; j<_arKeyArray[i].u4Size; j++) // u4GroupId, prKeyMap
            {
                if (((DRVCUST_InitGet(eIrrxFlagPrgUpDownWakeup)) &&
                     (u4KeyNum < MAX_EXP_KEY_NUM) &&
                     ((_arKeyArray[i].prKeyMap[j].u4Crystal == BTN_PRG_UP) ||
                      (_arKeyArray[i].prKeyMap[j].u4Crystal == BTN_PRG_DOWN))) ||
                    ((DRVCUST_InitGet(eStdbyUseVGAStatus)) &&
                     (u4KeyNum < MAX_EXP_KEY_NUM) &&
                     (u4IO32Read4B(PDWNC_WAKEN) & WAK_VGA) &&
                     ((_arKeyArray[i].prKeyMap[j].u4Crystal == BTN_MENU) ||
                      (_arKeyArray[i].prKeyMap[j].u4Crystal == BTN_INPUT_SRC))))

                {
                    u4Val = u4IO32Read4B(PDWNC_IREXP_EN);
                    u4Val |= IREXP_EN(u4KeyNum);
                    vIO32Write4B(PDWNC_IREXP_EN, u4Val);
                    u4Val = (_arKeyArray[i].u4GroupId) |
                            (_arKeyArray[i].prKeyMap[j].u4Key << 16) |
                            ((0xff - _arKeyArray[i].prKeyMap[j].u4Key) << 24);
                    vIO32Write4B(PDWNC_EXP_IRM0 + (8*u4KeyNum), u4Val);
                    vIO32Write4B(PDWNC_EXP_IRL0 + (8*u4KeyNum), 0);
                    u4KeyNum++;
                }
            }
        }
    }
}

#endif /* TPV_JOYSTICK_IR_H */

