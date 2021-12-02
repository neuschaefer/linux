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
#include "ir_if.h"
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_printf.h"
#include "x_stl_lib.h"
#include "x_pdwnc.h"
#include "x_bim.h"
#include "x_os.h"
#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif
#define DEFINE_IS_LOG   CLI_IsLog
#include "x_debug.h"
#include "x_rand.h"
#include "x_util.h"
#include "x_lint.h"

/*****************************************************************************/

#include "drv_common.h"
#include "drvcust_if.h"
#include "ir_if.h"
#include "ir_kp.h"

/******************************************************************************
* Declare imported functions.
******************************************************************************/
#ifdef IRRC_LOG
EXTERN VOID IRRC_ResetLog(VOID);
EXTERN UINT32 IRRC_GetLog(IRRC_LOGGER_T **pprLoggger);
#endif

/******************************************************************************
* Declare the internal functions.
******************************************************************************/
static void _RxCLI_Callback(UINT32 u4Info, const UINT8 *pu1Data)
{
    INT32 i, i4BitCnt;
    UINT32 *pu4Data;

    i4BitCnt = INFO_TO_BITCNT(u4Info);
    Printf("%d(%d)(%d)(%d) - ", i4BitCnt, INFO_TO_1STPULSE(u4Info),
        INFO_TO_2NDPULSE(u4Info), INFO_TO_3RDPULSE(u4Info));
    pu4Data = (UINT32 *)pu1Data;
    for (i=0; i*8 < (i4BitCnt-1); i++) {
        if (i%4==0) { Printf("(0x%08x)", pu4Data[i>>2]); }
        Printf("0x%02x ", pu1Data[i]);
    }
    Printf("\n");
}


/******************************************************************************
* Declare the Rx SubCli command functions.
******************************************************************************/
static INT32 _RxCLI_Init(INT32 i4Argc, const CHAR ** szArgv)
{
    const CHAR *szStr;
    INT32 i, i4Config, i4SaPeriod, i4Threshold;
//    INT32 i4Down, i4PBit;
//    UINT32 u4Key1, u4Key2;

    IRHW_RxSetCallback(_RxCLI_Callback, NULL);
    IRHW_RxRdConf(&i4Config, &i4SaPeriod, &i4Threshold);
//  i4Down = IRHW_Down(NULL);
//  i4PBit = IRHW_PowerBitNum(NULL);
//  u4Key1 = IRHW_PowerUpKey1(NULL);
//  u4Key2 = IRHW_PowerUpKey2(NULL);
    for (i=0; i<i4Argc; i++) {
        szStr = szArgv[i];
        if (szStr[0]=='-') {
            switch(szStr[1]) {
            case 'C': case 'c':                     /* Config */
                i++; szStr = szArgv[i];
                if (szStr[0]=='0' || StrToInt(szStr)!=0) {
                    i4Config = StrToInt(szStr);
                }
                break;
            case 'S': case 's':                     /* SaPeriod */
                i++; szStr = szArgv[i];
                if (szStr[0]=='0' || StrToInt(szStr)!=0) {
                    i4SaPeriod = StrToInt(szStr);
                }
                break;
            case 'T': case 't':                     /* Threshold */
                i++; szStr = szArgv[i];
                if (szStr[0]=='0' || StrToInt(szStr)!=0) {
                    i4Threshold = StrToInt(szStr);
                }
                break;
#if 0
            case 'D': case 'd':
                i++; szStr = szArgv[i];
                if (szStr[0]=='0' || StrToInt(szStr)!=0) {
                    i4Down = StrToInt(szStr);
                }
                break;
            case 'B': case 'b':
                i++; szStr = szArgv[i];
                if (szStr[0]=='0' || StrToInt(szStr)!=0) {
                    i4PBit = StrToInt(szStr);
                }
                break;
            case 'K': case 'k':
                if (szStr[2]=='1') {
                    i++; szStr = szArgv[i];
                    if (szStr[0]=='0' || StrToInt(szStr)!=0) {
                        u4Key1 = StrToInt(szStr);
                    }
                } else if (szStr[2]=='2') {
                    i++; szStr = szArgv[i];
                    if (szStr[0]=='0' || StrToInt(szStr)!=0) {
                       u4Key2 = StrToInt(szStr);
                    }
                }
                break;
#endif
            default: continue;
            }
        }
    }

//  IRHW_Down(&i4Down);
//  IRHW_PowerBitNum((UINT32*)&i4PBit);
//  IRHW_PowerUpKey1(&u4Key1);
//  IRHW_PowerUpKey2(&u4Key2);
    IRHW_RxInit(i4Config, i4SaPeriod, i4Threshold);

    IRHW_RxRdConf(&i4Config, &i4SaPeriod, &i4Threshold);
    Printf("%s %s\n", __DATE__, __TIME__);
    Printf("IrRx Conf:0x%08x\tSP:0x%08x\tTd:0x%08x\t\n",
            i4Config, i4SaPeriod, i4Threshold);
//    Printf("Down:%d\n" IRHW_Down(NULL));
//    Printf("     PowerKeyBit:%d\tPowerKey1:0x%08x\tPowerKey2:0x%08x\n",
//            IRHW_PowerBitNum(NULL), IRHW_PowerUpKey1(NULL), IRHW_PowerUpKey2(NULL));
    return 0;
}

static INT32 _RxCLI_Stop(INT32 i4Argc, const CHAR ** szArgv)
{
    IRHW_RxStop();
    return 0;
}

static INT32 _RxCLI_PowerDown(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 i, u4Val;

    u4Val = WAK_IRRX;

    for (i=0; i<i4Argc; i++)
    {
        if (szArgv[i][0]!='-') { continue; }
        switch(szArgv[i][1]) {
        case 'G': case 'g':
            u4Val |= WAK_ALLGPIO;
            break;
        case 'V': case 'v':
            u4Val |= WAK_VGA;
            break;
        case 'S': case 's':
            u4Val |= WAK_ALLSRVAD;
            break;
        case 'R': case 'r':
            u4Val |= WAK_RTC;
            break;
        }
    }

    PDWNC_EnterPowerDown(u4Val, (UINT32)PDWNC_POWER_DOWN_UNKNOWN);
    return 0;
}

static INT32 _RxCLI_GetState(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

//    Printf("IRRX state is %d\n", IRHW_RxHwState());
    return 0;
}

#define MTK_IRRX_CONFIG         0x4121
#define MTK_IRRX_SAPERIOD       0x0037
#define MTK_IRRX_THRESHOLD      0x0001

#define RC56_IRRX_CONFIG        0x4005
#define RC56_IRRX_SAPERIOD      0x0032
#define RC56_IRRX_THRESHOLD     0x0301

static INT32 _u4CLIIrRxType = 0;

static void _Rx_Callback(UINT32 u4Info, const UINT8 *pu1Data)
{
    if (_u4CLIIrRxType == IRRX_NEC_TYPE)
    {
        if (INFO_TO_BITCNT(u4Info)==33)
        {
            Printf("1ST:%d 2ND:%d 3RD:%d GroupId:0x%04x KeyId:0x%02x\n", 
                INFO_TO_1STPULSE(u4Info), INFO_TO_2NDPULSE(u4Info), INFO_TO_3RDPULSE(u4Info),
                ((*((UINT32 *)pu1Data)) & 0xffff), ((*((UINT32 *)pu1Data)) >> 16) & 0xff);
        }
    }
    else if (_u4CLIIrRxType == IRRX_RC5_TYPE)
    {
        Printf("BitCnt:%d Data:0x%08x\n", INFO_TO_BITCNT(u4Info), *((UINT32 *)pu1Data));
    }
}

static INT32 _RxCLI_IrKeyParse(INT32 i4Argc, const CHAR **szArgv)
{
    PFN_IRRXCB_T pfnOld;

    if (i4Argc < 2)
    {
        Printf("ir.rx.parse [nec|rc5|none]\n");
        return 0;
    }

    switch(szArgv[1][0]) {
    case 'N': case 'n':
        if (szArgv[1][1]=='O' || szArgv[1][1]=='o')
        {
            PDWNC_Reboot();
        }
        IRHW_RxInit(MTK_IRRX_CONFIG, MTK_IRRX_SAPERIOD, MTK_IRRX_THRESHOLD);
        _u4CLIIrRxType = IRRX_NEC_TYPE;
        break;
    case 'R': case 'r':
        IRHW_RxInit(RC56_IRRX_CONFIG, RC56_IRRX_SAPERIOD, RC56_IRRX_THRESHOLD);
        _u4CLIIrRxType = IRRX_RC5_TYPE;
        break;
    default:
        Printf("ir.rx.parse [nec|rc5|none]\n");
        return 0;
    }
    IRHW_RxSetCallback(_Rx_Callback, &pfnOld);
    return 0;
}

static INT32 _RxCLI_SetParam(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i;
    UINT32 u4Val;

    for (i=1; i<i4Argc; i++) {
        if (szArgv[i][0]!='-') { continue; }
        switch(szArgv[i][1]) {
        case 'r':
            i++;
            u4Val = StrToInt(szArgv[i]);
            IRRX_QuerySetRepeat((INT32 *)&u4Val);
            break;
        case 't':
            i++;
            u4Val = StrToInt(szArgv[i]);
            IRRX_QuerySetRepeatTime(&u4Val);
            break;
        case '1':
            i++;
            u4Val = StrToInt(szArgv[i]);
            IRRX_QuerySet1stPulse(&u4Val);
            break;
        case '2':
            i++;
            u4Val = StrToInt(szArgv[i]);
            IRRX_QuerySet2ndPulse(&u4Val);
            break;
        case '3':
            i++;
            u4Val = StrToInt(szArgv[i]);
            IRRX_QuerySet3rdPulse(&u4Val);
            break;
        default:
            break;
        }
    }
    Printf("Repeat flag: %s\t", IRRX_QuerySetRepeat(NULL) ? "enabled" : "disabled");
    Printf("Conflict time: %d ms\n", IRRX_QuerySetRepeatTime(NULL));
    Printf("1st Pulse:%d\t2nd Pulse:%d\t3rd Pulse:%d\n", 
        IRRX_QuerySet1stPulse(NULL),
        IRRX_QuerySet2ndPulse(NULL),
        IRRX_QuerySet3rdPulse(NULL));
    return 0;
}

static INT32 _RxCLI_MtkInit(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    UINT32 u4Type = IRRX_NEC_TYPE;

    if (i4Argc >= 2)
    {
        switch(szArgv[1][0])
        {
        case 'R': case 'r':
            u4Type = IRRX_RC5_TYPE; 
            break;
        case 'U': case 'u':
            u4Type = IRRX_USR_TYPE; 
            break;
		case 'F': case 'f':
			u4Type = IRRX_FUNAI_FACTORY_TYPE; 
			break;
		case 'S': case 's':
			u4Type = IRRX_SHARP_TYPE; 
			break;
        case 'N': case 'n':
        default:
            u4Type = IRRX_NEC_TYPE; 
            break;
        }
    }
    
    i4Ret = IRRX_StopMtkIr();
    i4Ret |= IRRX_InitMtkIr(u4Type);
    return i4Ret;
}

static volatile INT32 _i4Loop = 1;
static HANDLE_T _hMtkThread;

static void _RxCLI_IrThread(void *pvArgv)
{
    UINT32 u4Key, u4PrevKey = BTN_NONE;
    INT32 i4Ret;    
    IRRX_RAW_DATA_T rRaw;
    
#if 0
    u4PrevKey = 120;
    LINT_SUPPRESS_NEXT_EXPRESSION(534);
    IRRX_QuerySetRepeatTime(&u4PrevKey);
    i4Ret = 1;
    LINT_SUPPRESS_NEXT_EXPRESSION(534);
    IRRX_QuerySetRepeat(&i4Ret);
    u4PrevKey = BTN_NONE;
#endif

    Printf("MtkIrThread Start... Loop:%d\n", _i4Loop);
    while (_i4Loop) {
        i4Ret = IRRX_PollMtkIr(&u4Key, (IRRX_RAW_DATA_T *)&rRaw);
        if (i4Ret == IR_SUCC) {
            if (u4Key == BTN_NONE) { /* polling timeout. */
                if (u4PrevKey != BTN_NONE) { /* a key up. */
                    Printf("u4Key is 0x%08x up1.(p:0x%08x, c:0x%08x)\n", u4PrevKey, u4PrevKey, u4Key);
                    u4PrevKey = BTN_NONE;
                }
            } else {
                if ((u4PrevKey != u4Key) && (u4PrevKey != BTN_NONE)) {
                    Printf("u4Key is 0x%08x up3.(p:0x%08x, c:0x%08x)\n", u4PrevKey, u4PrevKey, u4Key);
                }
                if (u4Key != u4PrevKey) {
                    Printf("u4Key is 0x%08x down.(p:0x%08x, c:0x%08x)\n", u4Key, u4PrevKey, u4Key);
                    u4PrevKey = u4Key;
                }
            }
        } else {
            if (u4PrevKey != BTN_NONE) {
                Printf("u4Key is 0x%08x up2.(p:0x%08x, c:0x%08x)\n", u4PrevKey, u4PrevKey, u4Key);
                u4PrevKey = BTN_NONE;
            }
        }
        x_thread_delay(110);
    }
    Printf("MtkIrThread Exit... Loop:%d\n", _i4Loop);
}

static INT32 _RxCLI_MtkIr(INT32 i4Argc, const CHAR ** szArgv)
{
    const CHAR *szStr;
    INT32 i, i4Ret;

    for (i=0; i<i4Argc; i++) {
        szStr = szArgv[i];
        if (szStr[0]=='-') {
            switch(szStr[1]) {
            case 'E': case 'e':
                _i4Loop = 0;
                return 0;
            default:
                break;
            }
        }
    }

    IRRX_ResetMtkIr();
    _i4Loop = 1;
    i4Ret = x_thread_create(&_hMtkThread, MTK_CLIIR_NAME, MTK_CLIIR_STACK_SZ, MTK_CLIIR_PRIORITY, _RxCLI_IrThread, 0, NULL);
    if (i4Ret != OSR_OK) {
        Printf("Create thread to poll MTK-IR failed.\n");
        return 1;
    }
    Printf("Create thread to poll MTK-IR successfully.\n");
    return 0;
}

typedef struct __string2key{
    CHAR *szString;
    UINT32 u4Key;
} STR2KEY_T;

static STR2KEY_T _aStr2KeyArray[] = {
    { "0", BTN_DIGIT_0 },
    { "1", BTN_DIGIT_1 },
    { "2", BTN_DIGIT_2 },
    { "3", BTN_DIGIT_3 },
    { "4", BTN_DIGIT_4 },
    { "5", BTN_DIGIT_5 },
    { "6", BTN_DIGIT_6 },
    { "7", BTN_DIGIT_7 },
    { "8", BTN_DIGIT_8 },
    { "9", BTN_DIGIT_9 },
    { "hundred", BTN_DIGIT_PLUS_100 },
    { "dot", BTN_DIGIT_DOT },
    { "left", BTN_CURSOR_LEFT },
    { "right", BTN_CURSOR_RIGHT },
    { "up", BTN_CURSOR_UP },
    { "down", BTN_CURSOR_DOWN },
    { "exit", BTN_EXIT },      // exit
    { "ce", BTN_CE },      // exit
    { "select", BTN_SELECT },
    { "enter", BTN_SELECT },
    { "ch_up", BTN_PRG_UP },
    { "ch_down", BTN_PRG_DOWN },
    { "prevch", BTN_PREV_PRG },
    { "pop", BTN_POP },
    { "zoom", BTN_ZOOM },
    { "picsize", BTN_PICSIZE },    // swap
    { "favorite", BTN_FAVORITE },
    { "fav_ch", BTN_FAV_CH },
    { "osd", BTN_OSD },    // prg_info
    { "add_erase", BTN_ADD_ERASE },
    { "cc", BTN_CC },
    { "input", BTN_INPUT }, // input_src
    { "vol_up", BTN_VOL_UP },
    { "vol_down", BTN_VOL_DOWN },
    { "mute", BTN_MUTE },
    { "mts", BTN_MTS },
    { "power", BTN_POWER },
    { "menu", BTN_MENU },
    { "clock", BTN_CLOCK }, // epg
    { "update", BTN_UPDATE },    // mem_card
    { "red", BTN_RED },
    { "green", BTN_GREEN },
    { "yellow", BTN_YELLOW },
    { "blue", BTN_BLUE },
    { "actctrl", BTN_ACTCTRL },
    { "smartpic", BTN_SMARTPIC },
    { "tttv", BTN_TTTV },
    { "pippos", BTN_PIPPOS },
    { "colorsys", BTN_COLORSYS },
    { "capture", BTN_CAPTURE },
    { "sleep", BTN_SLEEP },
    { "index", BTN_INDEX },
    { "timer", BTN_TIMER },
    { "freeze", BTN_FREEZE },
    { "vchip", BTN_VCHIP },
    { "SNDEFCT", BTN_SNDEFCT },
    { "repeat", BTN_REPEAT },    
    { NULL, 0 },
};

static INT32 _RxCLI_SendKey(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret, i;
    UINT32 u4Key;

    if (i4Argc != 2) {
        Printf("%s [KeyId] - send key id into ir key queue\n", szArgv[0]);
        return 1;
    }

    if ((szArgv[1][0] == '0') && (szArgv[1][1] == 'x')) {
        u4Key = StrToInt(szArgv[1]);
        // i4Ret = IRRX_SendMtkIr(BTN_POWER);
        i4Ret = IRRX_SendMtkIr(u4Key);
        if (i4Ret != OSR_OK) {
            Printf("%s send key:0x%08x failed and get return %d\n", szArgv[0], u4Key, i4Ret);
            return 1;
        }
        return 0;
    }

    for (i=0;
        ((_aStr2KeyArray[i].szString != NULL) &&
                (x_strcmp(szArgv[1], _aStr2KeyArray[i].szString)!=0));
        i++) { }

    u4Key = _aStr2KeyArray[i].u4Key;
    i4Ret = IRRX_SendMtkIr(u4Key);
    if (i4Ret != OSR_OK) {
        Printf("%s send key:0x%08x failed and get return %d\n", szArgv[0], u4Key, i4Ret);
        return 1;
    }

    // Printf("%s send key:0x%08x successfully\n", szArgv[0], u4Key);
    return 0;
}

static INT32 _IRCLI_Mtrcdbg(INT32 i4Argc, const CHAR ** szArgv)
{
	IRRX_RAW_DATA_T rRaw;
	INT32 i4Ret;
	if (i4Argc != 2) 
	{
		Printf("%s [KeyId] - mtrc_dbg [debug flag] - set mtrc debug log enable/disable\n", szArgv[0]);
		return 1;
	}
	x_memset((VOID *)(&rRaw),0,sizeof(IRRX_RAW_DATA_T));
	rRaw.au1Data[0]= StrToInt(szArgv[1]);
	rRaw.u4Len=1;
	if ((szArgv[1][0] == '0') ||(szArgv[1][0] == '1') ) 
		{
			i4Ret = IRRX_SendDebugMtkIr(BTN_DEBUG_ENABLE,&rRaw);
			//Printf("%s send key:0x%08x successfully\n", szArgv[0], StrToInt(szArgv[1]));
			if (i4Ret != OSR_OK)
				{
					Printf("%s send key:0x%08x failed and get return %d\n", szArgv[0], StrToInt(szArgv[1]), i4Ret);
					return 1;
				}
			return 0;
		}
	else
		{
			Printf("input error\n");
			return 1;
		}
}

static INT32 _IRCLI_LongPressThreshold(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4Val;
	if (i4Argc > 2) 
	{
		Printf("%s [ms] - lpt [ms] - set long press timeout\n", szArgv[0]);
		return 1;
	}
	if (i4Argc == 1) 
	{
		Printf("Now long press timeout %d ms\n", IRRX_QuerySetRepeatDuration(NULL));
		return 1;
	}
	u4Val = StrToInt(szArgv[1]);
	IRRX_QuerySetRepeatDuration(&u4Val);
	Printf("Now long press timeout %d ms\n", IRRX_QuerySetRepeatDuration(NULL));
	return 0;
}


extern UINT32 _u4IrRxTimeout ;

static INT32 _IRCLI_PTO(INT32 i4Argc, const CHAR ** szArgv)
{
	IRRX_RAW_DATA_T rRaw;
	//INT32 i4Ret;
	x_memset(rRaw.au1Data, 0, IRRX_MAX_RAW_DATA_SIZE); 
	rRaw.u1Protocol = 0;
	rRaw.u4Len = 0;
	if (i4Argc != 2) 
	{
		Printf("pto [ms] - set ir button up polling timeout\n", szArgv[0]);
		Printf("_u4IrRxTimeout is %dms now\n", _u4IrRxTimeout);
		return 1;
	}
	
	_u4IrRxTimeout = StrToInt(szArgv[1]);
    Printf("Set _u4IrRxTimeout as %dms\n", _u4IrRxTimeout);
	return 0;
}


static INT32 _RxCLI_SendKeyRaw(INT32 i4Argc, const CHAR ** szArgv)
{
    IRRX_RAW_DATA_T rRaw;
    INT32 i4Ret, i;
    UINT32 u4Key;

    if (i4Argc < 4) {
        Printf("%s [KeyId] [IR Type] [Raw byte0] [Raw byte1]... - send key id and raw data into ir key queue\n", szArgv[0]);
        return 1;
    }

    u4Key = StrToInt(szArgv[1]);

    rRaw.u1Protocol = (UINT8)StrToInt(szArgv[2]);

    rRaw.u4Len = (i4Argc - 3)>IRRX_MAX_RAW_DATA_SIZE?IRRX_MAX_RAW_DATA_SIZE:(i4Argc - 3);
    for(i = 0; i < rRaw.u4Len; i++)
    {
        rRaw.au1Data[i] = (UINT8)StrToInt(szArgv[i + 3]);
    }
    i4Ret = IRRX_SendMtkIrRaw(u4Key, &rRaw);
    if (i4Ret != OSR_OK) {
        Printf("%s send key:0x%08x failed and get return %d\n", szArgv[0], u4Key, i4Ret);
        return 1;
    }
    return 0;
}

static INT32 _RxCLI_PollKey(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    UINT32 u4Key;
    IRRX_RAW_DATA_T rRaw;
    	
    if (i4Argc != 1) {
        Printf("%s - Print poll key value from ir key queue.\n", szArgv[0]);
        return 1;
    }

    i4Ret = IRRX_PollMtkIr(&u4Key, (IRRX_RAW_DATA_T *)&rRaw);
    if (i4Ret != IR_SUCC) {
        Printf("%s poll MTKIR key failed. return %d\n", szArgv[0], i4Ret);
        return 1;
    }
#ifndef __KERNEL__    
    Printf("%s get key value:0x%08x\n", szArgv[0], u4Key);
    return 0;
#else
    return (INT32)u4Key;
#endif
}

static INT32 _RxCLI_PollRawKey(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    UINT32 u4Key;
    IRRX_RAW_DATA_T rRaw;
    	
    if (i4Argc != 1) {
        Printf("%s - Print poll key value from ir key queue.\n", szArgv[0]);
        return 1;
    }

    i4Ret = IRRX_PollMtkIr(&u4Key, &rRaw);
    if (i4Ret != IR_SUCC) {
        Printf("%s poll MTKIR key failed. return %d\n", szArgv[0], i4Ret);
        return 1;
    }
#ifndef __KERNEL__    
    Printf("%s get key value:0x%08x\n", szArgv[0], u4Key);
    return 0;
#else
    return (INT32)rRaw.au1Data[0];
#endif
}

#ifdef IRRC_LOG
static INT32 _RxCLI_PrintLog(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Num;
    UINT32 i;
    UINT32 u4GroupId;
    UINT32 u4Cnt = 0;
    
    IRRC_LOGGER_T *prLogger;
    u4Num = IRRC_GetLog(&prLogger);
    Printf("IRRC log: %4d record\n", u4Num);     
    for(i = prLogger->u4CurrIdx; i < u4Num; i++)
    {
        u4GroupId = (prLogger->arLog[i].u4Raw & 0xff) | ((0xff - (prLogger->arLog[i].u4Raw & 0xff)) << 8);
        Printf("[%4d] IR raw code:  0x%8x, MTK code: 0x%8x, time stamp: %d sec %d micro\n", 
        	u4Cnt,
        	prLogger->arLog[i].u4Raw, 
        	IRRX_RawToCrystal(u4GroupId , (prLogger->arLog[i].u4Raw >> 16) & 0xff),
        	prLogger->arLog[i].rTime.u4Seconds, 
        	prLogger->arLog[i].rTime.u4Micros);
        u4Cnt++;        
    }
    for(i = 0; i < prLogger->u4CurrIdx; i++)
    {
        u4GroupId = (prLogger->arLog[i].u4Raw & 0xff) | ((0xff - (prLogger->arLog[i].u4Raw & 0xff)) << 8);    
        Printf("[%4d] IR raw code:  0x%8x, MTK code: 0x%8x, timestamp: %d sec %d micro\n", 
        	u4Cnt,
        	prLogger->arLog[i].u4Raw, 
        	IRRX_RawToCrystal(u4GroupId, (prLogger->arLog[i].u4Raw >> 16) & 0xff),
        	prLogger->arLog[i].rTime.u4Seconds, 
        	prLogger->arLog[i].rTime.u4Micros);
        u4Cnt++;        
    }
    
    return 0;
}

static INT32 _RxCLI_ResetLog(INT32 i4Argc, const CHAR ** szArgv)
{
    IRRC_ResetLog();
    Printf("IRRC log cleared!\n");         
    return 0;
}
#endif

static INT32 _i4DelayMs = 400;
static INT32 _i4PatternNumber = 0;
static volatile INT32 _i4SendLoop = 1;
static BOOL _fgRandom = FALSE;

static void _RxCLI_SendIrPattern(void *pvArgv)
{
    INT32 i4Ret, i4Idx = 0;
    UINT32 u4Key;

    Printf("MtkThread Send Ir Pattern... Loop:%d\n", _i4SendLoop);

    while (_i4SendLoop) {
        u4Key = _au4MtkKeyPattern[_i4PatternNumber][i4Idx];

        if (_fgRandom) {
            i4Idx = random((_au4TotalPattern[_i4PatternNumber] - 1));
        } else {
            i4Idx++;
        }
        if (i4Idx >= _au4TotalPattern[_i4PatternNumber]) {
            i4Idx = 0;
        }

        if (u4Key == BTN_NONE)
            continue;
        else if (u4Key == BTN_DELAY_1S)
        {
            x_thread_delay(1000);
            continue;
        }
        else if (u4Key == BTN_DELAY_2S)
        {
            x_thread_delay(2000);
            continue;
        }
        else if (u4Key == BTN_DELAY_5S)
        {
            x_thread_delay(5000);
            continue;
        }
        else if (u4Key == BTN_DELAY_10S)
        {
            x_thread_delay(10000);
            continue;
        }
        i4Ret = IRRX_SendMtkIr(u4Key);
        if (i4Ret != 0) {
            Printf("Send u4Key:%d failed return %d\n", u4Key, i4Ret);
        }
        x_thread_delay(_i4DelayMs);
    }

    Printf("MtkThread Send Ir Pattern Exist... Loop:%d\n", _i4SendLoop);
}

static INT32 _RxCLI_Pattern(INT32 i4Argc, const CHAR ** szArgv)
{
    const CHAR *szStr;
    INT32 i4Ret;

    szStr = szArgv[1];
    _fgRandom = FALSE;
    if (szStr[0]=='-') {
        switch(szStr[1]) {
        case 'E': case 'e':
            _i4SendLoop = 0;
            return 0;
        case 'R': case 'r':
            _fgRandom = TRUE;
            /* fall through */
        case 'I': case 'i':
            if (i4Argc < 2) {
                Printf("%s [-e] [-ri [delay ms] [pattern#]] - Have a thread to send keys to queue.\n",
                    szArgv[0]);
                return 1;
            }
            szStr = szArgv[2];
            if (i4Argc > 2 && StrToInt(szStr)!=0) {
                _i4DelayMs = StrToInt(szStr);
                Printf("_i4DelayMs is setting as %d\n", _i4DelayMs);
            }
            szStr = szArgv[3];
            if (i4Argc > 3) {
                _i4PatternNumber = StrToInt(szStr);
            }
            break;
        default:
            Printf("%s [-e] [-ri [delay ms]] - Have a thread to send keys to queue.\n", szArgv[0]);
            return 1;
        }
    }
    _i4SendLoop = 1;
    i4Ret = x_thread_create(&_hMtkThread, MTK_CLIIRSPTN_NAME, MTK_CLIIR_STACK_SZ, MTK_CLIIR_PRIORITY, _RxCLI_SendIrPattern, 0, NULL);
    if (i4Ret != OSR_OK) {
        Printf("Create thread to send MTK-IR stress pattern failed.\n");
        return 1;
    }
    Printf("Create thread to send MTK-IR stress pattern successfully.\n");
    return 0;
}

static INT32 _RxCLI_MemKeys(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i, i4KeyIdx;
    UINT32 u4KeyVal;

    if ((i4Argc & 1)==0) {
        Printf("%s [[keyidx1] [keyval1] [keyidx2] [keyval2] ...] - List/Modify key pattern\n",
            szArgv[0]);
        Printf("\t\t[keyidx] and [keyval] must be pair\n");
    }
    for (i=1; i<i4Argc; i+=2) {
        i4KeyIdx = StrToInt(szArgv[i]);
        u4KeyVal = StrToInt(szArgv[i+1]);

        if (i4KeyIdx >= 0 && i4KeyIdx < _au4TotalPattern[_i4PatternNumber]) {
            _au4MtkKeyPattern[_i4PatternNumber][i4KeyIdx] = u4KeyVal;
        } else {
            Printf("Input KeyIdx:%d is not valid, must >= 0 and < %d\n", i4KeyIdx, _au4TotalPattern[_i4PatternNumber]);
        }
    }

    Printf("\nIndex |  Key 0     |  Key 1     |  Key 2     |  Key 3     |");
    Printf("\n------+------------+------------+------------+------------+");
    for (i = 0; i<_au4TotalPattern[_i4PatternNumber]; i++) {
        if (i%4==0) {
            Printf("\n%4d  | 0x%08x |", i, _au4MtkKeyPattern[_i4PatternNumber][i]);
        } else {
            Printf(" 0x%08x |", _au4MtkKeyPattern[_i4PatternNumber][i]);
        }
    }
    return 0;
}

#ifdef CC_CLI
/******************************************************************************
* Declare the Tx SubCli command functions.
******************************************************************************/
static INT32 _TxCLI_Init(INT32 i4Argc, const CHAR ** szArgv)
{
#if 1 // TX
    const CHAR *szStr;
    INT32 i, i4Config, i4Modulation;
    IRTXWAVE_T rWave;

    IRHW_TxRdConf(&i4Config, &rWave, &i4Modulation);
    for (i=0; i<i4Argc; i++)
    {
        szStr = szArgv[i];
        if (szStr[0]=='-')
        {
            switch(szStr[1])
            {
            case 'C': case 'c':
                i++; szStr = szArgv[i];
                if (szStr[0]=='0' || StrToInt(szStr)!=0)
                {
                    i4Config = StrToInt(szStr);
                }
                break;
            case 'S': case 's':
                if (szStr[2]=='H' || szStr[2]=='h')
                {
                    i++; szStr = szArgv[i];
                    if (szStr[0]=='0' || StrToInt(szStr)!=0)
                    {
                        rWave.u4SyncHigh = StrToInt(szStr);
                    }
                }
                else if (szStr[2]=='L' || szStr[2]=='l')
                {
                    i++; szStr = szArgv[i];
                    if (szStr[0]=='0' || StrToInt(szStr)!=0)
                    {
                        rWave.u4SyncLow = StrToInt(szStr);
                    }
                }
                break;
            case '0':
                if (szStr[2]=='H' || szStr[2]=='h')
                {
                    i++; szStr = szArgv[i];
                    if (szStr[0]=='0' || StrToInt(szStr)!=0)
                    {
                        rWave.u4Data0High = StrToInt(szStr);
                    }
                }
                else if (szStr[2]=='L' || szStr[2]=='l')
                {
                    i++; szStr = szArgv[i];
                    if (szStr[0]=='0' || StrToInt(szStr)!=0)
                    {
                        rWave.u4Data0Low = StrToInt(szStr);
                    }
                }
                break;
            case '1':
                if (szStr[2]=='H' || szStr[2]=='h')
                {
                    i++; szStr = szArgv[i];
                    if (szStr[0]=='0' || StrToInt(szStr)!=0)
                    {
                        rWave.u4Data1High = StrToInt(szStr);
                    }
                }
                else if (szStr[2]=='L' || szStr[2]=='l')
                {
                    i++; szStr = szArgv[i];
                    if (szStr[0]=='0' || StrToInt(szStr)!=0)
                    {
                        rWave.u4Data1Low = StrToInt(szStr);
                    }
                }
                break;
            case 'M': case 'm':
                i++; szStr = szArgv[i];
                if (szStr[0]=='0' || StrToInt(szStr)!=0)
                {
                    i4Modulation = StrToInt(szStr);
                }
                break;
            default: continue;
            }
        }
    }

    IRHW_TxWrConf(i4Config, &rWave, i4Modulation);
    IRHW_TxRdConf(&i4Config, &rWave, &i4Modulation);
    Printf("IrTx Conf:0x%08x Mod:0x%08x\n", i4Config, i4Modulation);
    Printf("IrTx Sync H/L: 0x%08x/0x%08x\n", rWave.u4SyncHigh, rWave.u4SyncLow);
    Printf("IrTx Data0 H/L: 0x%08x/0x%08x\n", rWave.u4Data0High, rWave.u4Data0Low);
    Printf("IrTx Data1 H/L: 0x%08x/0x%08x\n", rWave.u4Data1High, rWave.u4Data1Low);
#endif

    return 0;
}

static INT32 _TxCLI_Stop(INT32 i4Argc, const CHAR ** szArgv)
{
    Printf("IrTx Stop do nothing.\n");
    return 0;
}

static INT32 _TxCLI_Send(INT32 i4Argc, const CHAR ** szArgv)
{
#if 1 // TX
    INT32 i4BitNum;
    INT32 ai4Data[4];
 
    i4BitNum = StrToInt(szArgv[1]);

    if ((i4BitNum==0) || (i4Argc < 3) || (i4Argc > 5)) {
        Printf("%s [bitnum] [data1] [data2]\n");
        return 1;
    }

    ai4Data[0] = StrToInt(szArgv[2]);

    if (i4Argc > 3) ai4Data[1] = StrToInt(szArgv[3]);
    if (i4Argc > 4) ai4Data[2] = StrToInt(szArgv[4]);
    if (i4BitNum >=0 && i4BitNum<97) {
        IRHW_TxSendData(ai4Data, i4BitNum);
    } else {
        Printf("Invalid arguement.\n");
        return 1;
    }
#endif
    return 0;
}

#if 0 // TX
static HANDLE_T _hTxThrd;
static volatile INT32 _i4TxLoop = 1;

static void _TxCLI_RunThread(void *pvDummy)
{
    INT32 ai4Data[4];
    IRTXWAVE_T rWave;

    rWave.u4SyncHigh = 0x3b4c0;
    rWave.u4SyncLow = 0x1da60;
    rWave.u4Data0High = 0x3b10;
    rWave.u4Data0Low = 0x3b10;
    rWave.u4Data1High = 0x3b10;
    rWave.u4Data1Low = 0xb23e;
    IRHW_TxWrConf(0x56, &rWave, 0x00e602b2);
    ai4Data[0] = 0x12345678;
    _i4TxLoop = 1;
    while (_i4TxLoop) {
        IRHW_TxSendData(ai4Data, 32);
    }
    Printf("TxThread exit\n");
}
#endif

static INT32 _TxCLI_Test(INT32 i4Argc, const CHAR ** szArgv)
{
#if 0 // TX
    INT32 i4Ret;

    i4Ret = x_thread_create(&_hTxThrd, MTK_CLIIRTX_NAME, 512, MTK_CLIIR_PRIORITY, _TxCLI_RunThread, 0, NULL);
    if (i4Ret != OSR_OK) {
        Printf("Create thread to watchdog failed.\n");
        return 1;
    }
    Printf("Create thread to watchdog successfully.\n");
#endif
    return 0;
}

static INT32 _TxCLI_MtkInit(INT32 i4Argc, const CHAR ** szArgv)
{
    return 0;
}
#endif /* CC_CLI */

/******************************************************************************
* Declare the SubCli command lists.
******************************************************************************/
CLI_EXEC_T arMTKIRCmdTbl[] = {
	DECLARE_CMD(_RxCLI_MtkInit, init, init, "ir.rx.mtk.init [rc5|nec|sharp|user|funai_factory] - Initial mtk IR."),
    DECLARE_CMD(_RxCLI_MtkIr, thread, td, "ir.rx.mtk.td [-e] [-g [GrpId]] - Have a polling thread to get IR key to print."),
    DECLARE_CMD(_RxCLI_SendKey, send, send, "ir.rx.mtk.send [KeyId] - send key id into ir key queue"),
	DECLARE_CMD(_RxCLI_SendKeyRaw, sendraw, sendraw, "ir.rx.mtk.sendraw [KeyId] [IR type] [Raw byte0] [Raw byte1] ...- send key id into ir key queue"),    
    DECLARE_CMD(_RxCLI_PollKey, poll, poll, "ir.rx.mtk.poll - Print poll key value from ir key queue."),
    DECLARE_CMD(_RxCLI_PollRawKey, pollraw, raw, "ir.rx.mtk.poll - Print poll raw key value from ir key queue."),    
    DECLARE_CMD(_RxCLI_Pattern, keypattern, kp, "ir.rx.mtk.kp [-e] [-ri [delay ms] [pattern#]] - Have a thread to send keys to queue."),
    DECLARE_CMD(_RxCLI_MemKeys, memkeys, mk, "ir.rx.mtk.mk [[keyidx1] [keyval1] [keyidx2] [keyval2]...] - List/Modify key pattern\n"),
    // XXX
    DECLARE_END_ITEM(),
};

CLI_EXEC_T arRXCmdTbl[] = {
    DECLARE_G_CMD(_RxCLI_SendKey, send, send, "ir.rx.send [KeyId] - send key id into ir key queue"),
    DECLARE_CMD(_RxCLI_Init, init, init, "ir.rx.init -c [conf] -s [sap] -t [thld]"),
    DECLARE_CMD(_RxCLI_Stop, stop, stop, "ir.rx.stop"),
    DECLARE_G_CMD(_RxCLI_PowerDown, down, down, "ir.rx.down"),
    DECLARE_G_CMD(_RxCLI_GetState, state, state, "ir.rx.state"),
    DECLARE_G_CMD(_RxCLI_IrKeyParse, parse, parse, "ir.rx.parse [nec|rc5]"),
#ifdef IRRC_LOG    
    DECLARE_G_CMD(_RxCLI_PrintLog, printlog, pl, "ir.rx.printlog - print irrc log buffer history"),    
    DECLARE_G_CMD(_RxCLI_ResetLog, resetlog, rl, "ir.rx.resetlog - reset irrc log buffer history"),        
#endif    
    DECLARE_CMD(_RxCLI_SetParam, setparam, sp, "ir.rx.setparam -g [id] -[1|2|3] [pulse]"),
    DECLARE_SUBLIST(MTKIR, mtk, mtk, "MTK TV IR control functions"),
    // XXX
    DECLARE_END_ITEM(),
};

#ifdef CC_CLI
CLI_EXEC_T arTXCmdTbl[] = {
    DECLARE_CMD(_TxCLI_Init, init, init, "ir.tx.init -c [conf] [-sh|sl|0h|0l|1h|1l] [tp] -m [mod]"),
    DECLARE_CMD(_TxCLI_Stop, stop, stop, "ir.tx.stop"),
    DECLARE_CMD(_TxCLI_Send, send, send, "ir.tx.send [bitnum] [data1] [data2]"),
    DECLARE_CMD(_TxCLI_Test, test, test, "ir.tx.test"),
    DECLARE_CMD(_TxCLI_MtkInit, mtki, mtki, "ir.tx.mtk"),
    // XXX
    DECLARE_END_ITEM(),
};
#endif /* CC_CLI */


/******************************************************************************
* Declare the macros and reglist for RegTest functions
******************************************************************************/
#define IR_BASE         BIM_BASE
#define IR_REG_LENGTH   0

#ifdef CC_CLI
extern REG_TEST_T arIRRgtList[];
#endif /* CC_CLI */


/******************************************************************************
* Declare the debug on/off/level and RegTest functions
******************************************************************************/
CLIMOD_DEBUG_FUNCTIONS(IR)

#ifdef CC_CLI
// CLIMOD_REGTST_FUNCTIONS(IR)
#endif /* CC_CLI */

/******************************************************************************
* End of Declaration
******************************************************************************/

#ifdef CC_CLI
static INT32 _IRCLI_Diag(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    if (i4Argc > 1 && szArgv[1][0]=='s') {
        #if defined(CC_MT8223)
        i4Ret = 0;
        #else
        i4Ret = IR_Diag();
        #endif
        Printf( "\n==================\n"
                " IR Diag %s!"
                "\n==================\n", ((i4Ret==0) ? "Success" : "Fail"));
        return i4Ret;
    }

#if 1
    /* NOR */
    i4Ret = CLI_Parser("nor.diag");
    Printf("=====================> nor.diag return %d\n", i4Ret);
    if (i4Ret!=0) return i4Ret;
    /* NAND */
//  i4Ret = CLI_Parser("nand.diag");
//  Printf("=====================> nand.diag return %d\n", i4Ret);
//  if (i4Ret!=0) return i4Ret;
    /* DMX */
    i4Ret = CLI_Parser("dmx.diag");
    Printf("=====================> dmx.diag return %d\n", i4Ret);
    if (i4Ret!=0) return i4Ret;
    /* PSR */
    CLI_Parser("mpv.d_l 0");
    i4Ret = CLI_Parser("mpv.init");
    Printf("=====================> mpv.init return %d\n", i4Ret);
    if (i4Ret!=0) return i4Ret;
    i4Ret = CLI_Parser("aud.init");
    Printf("=====================> aud.init return %d\n", i4Ret);
    if (i4Ret!=1) return 1;
    i4Ret = CLI_Parser("psr.init");
    Printf("=====================> psr.init return %d\n", i4Ret);
    if (i4Ret!=0) return i4Ret;
    i4Ret = CLI_Parser("psr.diag");
    Printf("=====================> psr.diag return %d\n", i4Ret);
    if (i4Ret!=0) return i4Ret;
    CLI_Parser("mpv.d_l 5");
//  /* IDE */
//  i4Ret = CLI_Parser("ide.diag");
//  Printf("=====================> ide.diag return %d\n", i4Ret);
//  if (i4Ret!=0) return i4Ret;
    /* FCI */
//  i4Ret = CLI_Parser("fci.diag");
//  Printf("=====================> fci.diag return %d\n", i4Ret);
//  if (i4Ret!=0) return i4Ret;
#endif

    #if defined(CC_MT8223)
    i4Ret = 0;
    #else
    i4Ret = IR_Diag();
    #endif
    Printf( "\n==================\n"
            " IR Diag %s!"
            "\n==================\n", ((i4Ret==0) ? "Success" : "Fail"));
    return i4Ret;
}
#endif /* CC_CLI */


CLI_EXEC_T arIrCmdTbl[] =
{
#ifdef CC_CLI
//    CLIMOD_REGTST_CLIENTRY(IR),
#endif /* CC_CLI */

    CLIMOD_DEBUG_CLIENTRY(IR),
    DECLARE_CMD(_IRCLI_Mtrcdbg, mtrc_dbg, mtrc_dbg, "dbg_mtrc [debug flag] - set mtrc debug log enable/disable\n"), 
    DECLARE_CMD(_IRCLI_LongPressThreshold, lpt, lpt, "lpt [ms] - set long press timeout\n"),     
    DECLARE_CMD(_IRCLI_PTO, pto, pto, "pto [ms] - set ir button up polling timeout\n"), 
#ifdef CC_CLI
    DECLARE_CMD(_IRCLI_Diag, diag, diag, "ir.diag"),
#endif /* CC_CLI */

    // IR RX/TX sub commands.
    DECLARE_G_SUBLIST(RX, rx, rx, "RX commands"),

#ifdef CC_CLI
    DECLARE_SUBLIST(TX, tx, tx, "TX commands"),
#endif /* CC_CLI */

    // XXX
    DECLARE_END_ITEM(),
};

/*****************************************************************************/

CLI_MAIN_COMMAND_ITEM(Ir)
{
    "ir",
    NULL,
    NULL,
    arIrCmdTbl,
    "Ir command",
    CLI_GUEST
};

