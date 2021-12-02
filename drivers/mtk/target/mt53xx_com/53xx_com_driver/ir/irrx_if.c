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
/*-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: irrx_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file irrx_if.c
 *  irrx_if.c provides IRRX export interface functions.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

// local header files
#include "ir_debug.h"
#include "ir_if.h"
#include "ir_ph.h"

#include "x_bim.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_timer.h"
#include "x_lint.h"
#include "x_printf.h"
#include "x_debug.h"
#include "x_pinmux.h"
#include "x_pdwnc.h"
#include "drv_common.h"
#include "drvcust_if.h"
//#ifdef CC_MT5360    
#include "pdwnc_drvif.h"
//#endif
#include "u_irrc_btn_def.h"

//-----------------------------------------------------------------------------
// Config define
//-----------------------------------------------------------------------------

#ifdef CC_MTK_LOADER
#define IRRX_NON_OSAI
#endif /* CC_MTK_LOADER */

#ifndef CC_WITH_DRVKEY_REPEAT
#define CC_WITH_DRVKEY_REPEAT   0
#endif /* CC_WITH_DRVKEY_REPEAT */

#ifndef CC_DRIVER_PROGRAM
#define CC_DRIVER_PROGRAM       0
#endif /* CC_DRIVER_PROGRAM */

#if CC_DRIVER_PROGRAM
#define KEY_SRC_FROM_BUTTON     0       // diag program
#else /* CC_DRIVER_PROGRAM */
#define KEY_SRC_FROM_BUTTON     KEY_SRC_FRONT_PANEL
#endif /* CC_DRIVER_PROGRAM */

#ifndef IC_VERIFICATION
#define IC_VERIFICATION         0
#endif /* IC_VERIFICATION */

#ifndef CC_IRRX_GROUPID
#define CC_IRRX_GROUPID         0x9f20
#endif /* CC_IRRX_GROUPID */
//#undef PDWNC_WRITE32
//#undef PDWNC_READ32
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define MTK_IRRX_TIMESLICE      200


//
// NEC constant
// 

// #define MTK_IRRX_PRIORITY       100
// #define MTK_IRRX_ITEMCNT        16

// #define MTK_IRRX_CONFIG         0x4121

// For 5381, 30 Mhz is used instead of 27 Mhz
// #define MTK_IRRX_SAPERIOD       0x0032  // 537x 27MHz
// #define MTK_IRRX_SAPERIOD       0x0037  // others 5381/82 30MHz
// #define MTK_IRRX_SAPERIOD       0x0040  // 5380 verification board.

// #define MTK_IRRX_THRESHOLD      0x0001

#define NEC_IRRX_CONFIG         0x4121
#if defined(CC_MT5365) || defined(CC_MT5395)
    #define NEC_IRRX_SAPERIOD       0x00EA
#else
    #define NEC_IRRX_SAPERIOD       0x001A
#endif
#define NEC_IRRX_THRESHOLD      0x0001


//
// RC56 constant
// 

#define RC56_IRRX_CONFIG        0x4005

// For 5391, 3 Mhz is used instead of 30 Mhz
//#define RC56_IRRX_SAPERIOD      0x0032
#if defined(CC_MT5365) || defined(CC_MT5395)
    #define RC56_IRRX_SAPERIOD      0x00B4
#else
    #define RC56_IRRX_SAPERIOD      0x0014
#endif

//#define RC56_IRRX_THRESHOLD     0x0301
#define RC56_IRRX_THRESHOLD     0x0001

//
// Sharp constant
// 

#define SHARP_IRRX_CONFIG        0x4101

// For 5391, 3 Mhz is used instead of 30 Mhz
//#define RC56_IRRX_SAPERIOD      0x0032
#if defined(CC_MT5365) || defined(CC_MT5395)
    #define SHARP_IRRX_SAPERIOD       0x0087
#else
    #define SHARP_IRRX_SAPERIOD       0x00f
#endif


//#define RC56_IRRX_THRESHOLD     0x0301
#define SHARP_IRRX_THRESHOLD     0x0002

//
// Funai Factory constant
// 
//#define CC_FUNAI_FACTORY_IR_EU 				1
#define FUNAI_FACTORY_IRRX_CONFIG        0x4B04
#define FUNAI_FACTORY_IRRX_SAPERIOD      0x004F
#define FUNAI_FACTORY_IRRX_THRESHOLD     0x0001

#define FUNAI_START_DATA_BIT_NUM 3

#ifndef CC_FUNAI_FACTORY_IR_EU
#define FUNAI_CTRL_BIT_NUM 8
#else
#define FUNAI_CTRL_BIT_NUM 10
#endif

#define FUNAI_S_DATA_BIT_NUM 8
#define FUNAI_L_DATA_BIT_NUM 12
#define FUNAI_PARITY_BIT_NUM 4
#define FUNAI_STOP_DATA_BIT_NUM 1
#define FUNAI_COMMON_DATA_NUM (FUNAI_START_DATA_BIT_NUM + FUNAI_CTRL_BIT_NUM + FUNAI_PARITY_BIT_NUM + FUNAI_STOP_DATA_BIT_NUM)

#ifndef CC_FUNAI_FACTORY_IR_EU
typedef union
{
	UINT32 u4FunaiCtrlData;
	struct
	{
		UINT32 ins			: 3;
		UINT32 length		: 2;
		UINT32 device			: 1;
		UINT32 ctrl		: 2;
	} u;
} __attribute__ ((packed)) FUNAI_SPLITTER_T;
#else
typedef union
{
	UINT32 u4FunaiCtrlData;
	struct
	{
		UINT32 ins			: 4;
		UINT32 length		: 2;
		UINT32 device			: 1;
		UINT32 ctrl		: 3;
	} u;
} __attribute__ ((packed)) FUNAI_SPLITTER_T;
#endif

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

#ifndef IRRX_NON_OSAI

// IRRX buffer waiting semaphore initialized flag & handle.
static BOOL _fgSemaInit = FALSE;
static HANDLE_T _hSemaKey = 0;      /* Only Initial at boot time, even reset
                                       or stop, it won't delete. */
                                       
static BOOL _fgBtnInit = FALSE;
static HANDLE_T _hBtnThread;        /* Only Initial at boot time, even reset
                                       or stop, it won't delete. */

// Only one single buffer _u4CurrKey
static UINT32 _u4CurrKey = BTN_NONE;
//static UINT32 _u4CurrRaw = 0;
static IRRX_RAW_DATA_T _rCurrRaw;

#endif /* IRRX_NON_OSAI */

// The IRRX key settings
static UINT32 _u4_1stPulse = 8;
static UINT32 _u4_2ndPulse = 0;
static UINT32 _u4_3rdPulse = 0;

#ifndef IRRX_NON_OSAI
// The same key will be blocked in MTK_IRRX_TIMESLICE ms now.
static UINT32 _u4IrRxTimeSlice = MTK_IRRX_TIMESLICE;
// Polling timeout, key up timeout for no key.
/*static*/UINT32 _u4IrRxTimeout = 200;//MTK_IRRX_TIMESLICE + 100;
// Repeat key will be ignored after _u4IrRxRepeatDuration
static UINT32 _u4IrRxRepeatDuration = 200;

// This variable is control the repeat key code enable/disable.
static INT32 _fgRepeat = 0;
#endif /* IRRX_NON_OSAI */

// This variable is the default value of IRRC type, 0: NEC, 1: RC5/6
static UINT32       _gu4IRRXType = 0;
static KEYARRAY_T   *_arKeyArray = NULL;
static UINT32       _u4IRKeyArraySize = 0;
static UINT32       _u4RC56KeyMapSize = 0;
static WAKEUPKEYARRAY_T *_arRC56KeyMapArray = NULL;

#define MAX_KEY_ARRAY   (_u4IRKeyArraySize)
#define RC56_KEY_SIZE   (_u4RC56KeyMapSize)

#define KEY_EQUAL(x, y)     ((x)==(y))

static PFN_XFER_USR_TO_CRYSTAL _pfnXferUsrToCrystal = NULL;
static PFN_SET_USR_WAKEUP_KEY  _pfnSetUsrWakeupKey = NULL;

// This function pointer is the key lookup function
static FN_KEYMAP_LOOKUP_T _pfnRC5KeyLookup = NULL;
static FN_KEYMAP_LOOKUP_T _pfnRC6KeyLookup = NULL;

static BOOL _fgSystemCodePositiveFilter = TRUE;
static UINT32 _u4NumSystemCodeToFilter = 0;
static UINT32 _au4SystemCodeToFilter[IRRX_MAX_NUM_ADDR_FILTER] = 
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0};
#if 0
static BOOL _fgKeyboardSendEventCode = TRUE;
#endif
#ifdef CHANNEL_CHANGE_LOG
HAL_TIME_T dt_press_key;
#endif    

//UINT32 IRRX_GetCrystalKey(UINT32 u4GroupId, UINT8 u1Data);
//UINT32 IRRX_ReadSvadc(UINT32 u4Channel);
//UINT32 IRRX_RawToCrystal(UINT32  u4AddrCode, UINT32 u4CmdCode);

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/** IRRX_PassFilterSystemCodeToT8032() send filter system code informaton to T8032
 *  @param u4Info the IRRX received related attributes of the current data.
 *  @param pu1Data the IRRX received and current data.
 */
//-----------------------------------------------------------------------------

static INT32 IRRX_PassPostiveFilterSystemCodeToT8032(BOOL fgEnable, UINT32 u4Num, UINT32 au4FilterArray[])
{
	UINT32 u4Val;
       PDWNC_T8032_CMD_T rCmd;        

	if(fgEnable)
	{
//                i4Val = EEPROM_Read(u4Addr, (UINT32)au1Data, 6);        
		for( u4Val = 0; u4Val < u4Num; u4Val++)
		{
			if(au4FilterArray[u4Val] != 0x07)
			{
				rCmd.u1Cmd = PDWNC_CMD_ARMT8032_SET_IR_FILTER_SYSTEM_CODE;
				rCmd.u1SubCmd = 0x1;
				//Printf("Notify T8032 sys code 0x%x\n",au4FilterArray[u4Val]);			
				x_memcpy(rCmd.au1Data, &au4FilterArray[u4Val], 4);
				Printf("Pass data0 to T8032 0x%x\n",rCmd.au1Data[0]);
				//Printf("Pass data1 to T8032 0x%x\n",rCmd.au1Data[1]);			
				//Printf("Pass data2 to T8032 0x%x\n",rCmd.au1Data[2]);			
				//Printf("Pass data3 to T8032 0x%x\n",rCmd.au1Data[3]);			
				if(PDWNC_T8032Cmd(&rCmd, NULL))// if ack mismatch=>filter system code fifo full
				{
					return -1;
				}
			}	
		}
		return 0;
	}	
	else
	{
			rCmd.u1Cmd = PDWNC_CMD_ARMT8032_SET_IR_FILTER_SYSTEM_CODE;
			rCmd.u1SubCmd = 0x2;
			rCmd.au1Data[0] = rCmd.au1Data[1] = rCmd.au1Data[2] = rCmd.au1Data[3] = 0;
		//x_memcpy(rCmd.au1Data, &au4FilterArray[i], 4);
			Printf("Turn off uP IR filter mode !\n");
		return PDWNC_T8032Cmd(&rCmd, NULL);// if ack mismatch=>filter system code fifo full
	}
}

//-----------------------------------------------------------------------------
/** _IRRX_KeyboardCodeSet() toggle the Keyboard button between Event codes and
 *  raw codes.
 *  @param u4CodeType the type indicates which kind of codes will be sent. 
 *  @param fgPositiveFilter the filter type.
 */
//-----------------------------------------------------------------------------
#if 0
static VOID _IRRX_KeyboardCodeSet(UINT32 u4CodeType, BOOL fgPositiveFilter)
{
    if ((u4CodeType == 0x100 && !fgPositiveFilter) ||
        (u4CodeType == 0x101 &&  fgPositiveFilter) )
    {
        _fgKeyboardSendEventCode = FALSE; /* send raw codes, simulate the IR codes */
    }
    else
    {
        _fgKeyboardSendEventCode = TRUE; /* default event codes */
    }
}
#endif
//-----------------------------------------------------------------------------
/** _IsSystemCodeFiltered() translate raw (addr + cmd) ir code to crystal code.
 *  @param u4Info the IRRX received related attributes of the current data.
 *  @param pu1Data the IRRX received and current data.
 */
//-----------------------------------------------------------------------------
static BOOL _IRRX_IsSystemCodeAllowed(UINT32 u4System)
{
    UINT32 i;
    BOOL fgRet = TRUE;
    
    if(_u4NumSystemCodeToFilter == 0)
    {
        return TRUE;
    }
    else if(_fgSystemCodePositiveFilter)
    {
        fgRet = FALSE;
    }
    for(i = 0; i < _u4NumSystemCodeToFilter; i++)
    {
        if(u4System == _au4SystemCodeToFilter[i])
        {
            if(_fgSystemCodePositiveFilter)
            {
                fgRet = TRUE;
            }
            else
            {
                fgRet = FALSE;
            }
            break;
        }
    }    
    return fgRet;
}

//-----------------------------------------------------------------------------
/** _IRRX_CrystalLookup()
 */
//-----------------------------------------------------------------------------
static UINT32 _IRRX_CrystalLookup(const KEYUNIT_T *prKeyMap,
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

//-----------------------------------------------------------------------------
/** IRRX_GetCrystalKey()
 */
//-----------------------------------------------------------------------------
UINT32 IRRX_GetCrystalKey(UINT32 u4GroupId, UINT8 u1Data)
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
            return _IRRX_CrystalLookup(_arKeyArray[i].prKeyMap,
                                _arKeyArray[i].u4Size, (UINT32)u1Data);
        }
    }

    return BTN_INVALID;
}

//-----------------------------------------------------------------------------
/** _IRRX_RC56KeyLookup()
 *  @param u4BitCnt bit count of pu1Data list
 *  @param pu1Data the pointer to the IR raw data.
 *  @return the Crystal button key. BTN_NONE is no key.
 */
//-----------------------------------------------------------------------------
static UINT32 _IRRX_RC56KeyLookup(UINT32 u4BitCnt, const UINT8 *pu1Data, IRRX_RAW_DATA_T *prRaw)
{
    UINT32 u4MtkCode;
    UINT32 u4Rc56Code;
    UINT32 u4Address;
    UINT32 u4Command;
    UINT32 u4CrystalCode;
    UINT32 u4Rc5Shift;
    UINT32 u4Start;
    UINT32 u4Mb;
    UINT32 u4Toggle;

    ASSERT(pu1Data != NULL);
    ASSERT(prRaw != NULL);
    u4MtkCode = *((UINT32*)(void*)pu1Data);

    // try rc6
    u4Rc56Code = IR_PH_Mt2Rc6Decode(u4MtkCode);
    if ((_pfnRC6KeyLookup != NULL) &&
        (u4BitCnt == 26) &&
        (IR_PH_Rc6Validate((UINT8)u4BitCnt, u4MtkCode, u4Rc56Code)))
    {
        IR_PH_Rc6Split(u4Rc56Code, &u4Start, &u4Mb, &u4Toggle, &u4Address, &u4Command);
        LOG(7, "RC6 Start:%d Mb:%d Toggle:%d Addr:%d Cmd:%d\n", u4Start, u4Mb, u4Toggle, u4Address, u4Command);

        prRaw->u1Protocol = IRRX_RAW_DATA_RC6;      
        prRaw->u4Len = 4;    
        *((UINT32*)prRaw->au1Data) = (u4Start & 0x3) | 
        	((u4Mb & 0x7) << 2) | 
        	((u4Toggle & 0x3) << 5) | 
        	((u4Address & 0xFF) << 7) | 
        	((u4Command & 0xFF) << 15) |
              ((0x2 & 0xF) << 28);
        
        LOG(7, "RC6 raw:0x%x\n", *(UINT32*)prRaw->au1Data);

        if(!_IRRX_IsSystemCodeAllowed(u4Address))
        {
            LOG(9, "RC56KeyLookup get BTN_NONE by system code filter\n");            
            return BTN_NONE;                
        }
        
        u4CrystalCode = _pfnRC6KeyLookup(u4Address, u4Command);
        if (u4CrystalCode != BTN_NONE)
        {
            return u4CrystalCode;
        }
    }

    // try rc5
    u4Rc5Shift = IR_PH_Mt2Rc5Shift((UINT8)u4BitCnt, u4MtkCode);
    if (u4Rc5Shift != 0)
    {
        u4Rc56Code = IR_PH_Mt2Rc5Decode(u4MtkCode << u4Rc5Shift);
        if ((_pfnRC5KeyLookup != NULL) &&
            (IR_PH_Rc5Validate((UINT8)u4BitCnt, u4MtkCode, u4Rc56Code)))
        {
            IR_PH_Rc5Split(u4Rc56Code, &u4Start, &u4Toggle, &u4Address, &u4Command);            
            u4Command = (u4Rc5Shift == 3) ? (u4Command+64) : (u4Command);
            LOG(7, "RC5 Start:%d Toggle:%d Addr:%d Cmd:%d\n", u4Start, u4Toggle, u4Address, u4Command);

            prRaw->u1Protocol = IRRX_RAW_DATA_RC5;      
            prRaw->u4Len = 4;            
            *((UINT32*)prRaw->au1Data) = (u4Start & 0x3) | 
                ((u4Toggle & 0x1) << 2) |
                ((u4Address & 0x1F) << 3) | 
                ((u4Command & 0x3F) << 8) | 
                ((0x1 & 0xF) << 28);
                        
            LOG(7, "RC5 raw:0x%x\n", *(UINT32*)prRaw->au1Data);

            if(!_IRRX_IsSystemCodeAllowed(u4Address))
            {
                LOG(9, "RC56KeyLookup get BTN_NONE by system code filter\n");            
                return BTN_NONE;                
            }
            
            u4CrystalCode = _pfnRC5KeyLookup(u4Address, u4Command);
            if (u4CrystalCode != BTN_NONE)
            {
                return u4CrystalCode;
            }
        }
    }

    LOG(9, "RC56KeyLookup get BTN_INVALID\n");
    return BTN_NONE;
}

//-----------------------------------------------------------------------------
/** _IRRX_XferRC5ToCrystal() the IRRX data to Crystal IRRX key code map
 *      function.
 *  @param u4Info the IRRX received related attributes of the current data.
 *  @param pu1Data the IRRX received and current data.
 *  @return the Crystal button key. BTN_NONE is no key.
 */
//-----------------------------------------------------------------------------
static UINT32 _IRRX_XferRC5ToCrystal(UINT32 u4Info, const UINT8 *pu1Data, IRRX_RAW_DATA_T *prRaw)
{
#if CC_WITH_DRVKEY_REPEAT
    static UINT32 _u4PrevBitCnt = 0;
    static UINT32 _u4PrevData = 0;
#endif
    UINT32 u4BitCnt, u4Data, u4Key;

    u4BitCnt = INFO_TO_BITCNT(u4Info);

    /* Check no RC56KeyMap */
    if (_arRC56KeyMapArray == NULL)
    {
        return BTN_NONE;
    }

    /* Check empty data. */
    if ((u4BitCnt < 25) || (u4BitCnt > 29) || (pu1Data==NULL))
    {
        LOG(11, "%s(%d) RC56 Info:0x%08x BTN_NONE\n", __FILE__, __LINE__,
            u4Info);
        return BTN_NONE;
    }

    u4Data = *((UINT32 *)(void *)pu1Data);

#if CC_WITH_DRVKEY_REPEAT
    if ((u4BitCnt == _u4PrevBitCnt) && (u4Data == _u4PrevData))
    {
        return BTN_DRVKEY_REPEAT;
    }
#endif

    u4Key = _IRRX_RC56KeyLookup(u4BitCnt, pu1Data, prRaw);

    if (u4Key == BTN_NONE)
    {
        LOG(9, "%s(%d) RC56 Info:0x%08x Data:0x%08x BTN_NONE\n", __FILE__, __LINE__,
            u4Info, u4Data);
        //u4Key = _IRRX_CrystalLookup(_arRC56KeyMap, RC56_KEY_SIZE, (UINT32)u4Data);
        u4Key = BTN_INVALID;
    }

    LOG(9, "%s(%d) RC56 Info:0x%08x Data:0x%08x BTN:0x%08x\n", __FILE__, __LINE__,
        u4Info, u4Data, u4Key);
                
    UNUSED(u4Data); // in relase

#if CC_WITH_DRVKEY_REPEAT
    _u4PrevBitCnt = u4BitCnt;
    _u4PrevData = u4Data;
#endif

    return u4Key;
}

//-----------------------------------------------------------------------------
/** _IRRX_SetRC5WakeupKey() to set RC5 wakeup key
 */
//-----------------------------------------------------------------------------
static void _IRRX_SetRC5WakeupKey()
{
    UINT32 i, j, u4KeyNum, u4Val;

    if (_arRC56KeyMapArray == NULL)
    {
        LOG(1, "RC5 IR key array is not found.\n");
        return;
    }

    u4KeyNum = 0;
    vIO32Write4B(PDWNC_IREXP_EN, 0);//PDWNC_WRITE32(IREXP_EN_REG, 0);
    vIO32Write4B(PDWNC_EXP_BCNT, 0);//PDWNC_WRITE32(IREXP_BITNUM_REG, 0);
    vIO32Write4B(PDWNC_ENEXP_IRL, 0);//PDWNC_WRITE32(IREXP_L_BITMASK_REG, 0);
    if(DRVCUST_InitGet(eIrrxFlagAllWakeup))
    {
        // RC5 only check 0x0ff0 bits
        vIO32Write4B(PDWNC_ENEXP_IRM, 0xff0);//PDWNC_WRITE32(IREXP_M_BITMASK_REG, 0x0ff0);
        for (i=0; i < 2; i += 2)
        {
            u4Val = u4IO32Read4B(PDWNC_IREXP_EN);//PDWNC_READ32(IREXP_EN_REG);
            u4Val |= (IREXP_EN(i) | IREXP_EN((i + 1)));
            vIO32Write4B(PDWNC_IREXP_EN, u4Val);//PDWNC_WRITE32(IREXP_EN_REG, u4Val);
            
            u4Val = IR_PH_BiPhase(0x0, 5);
            vIO32Write4B(PDWNC_EXP_IRM0 + (8*i), u4Val << 3);//PDWNC_WRITE32(IREXP_M0_REG + (8*i), u4Val << 3);
            vIO32Write4B(PDWNC_EXP_IRL0 + (8*i), 0);
            vIO32Write4B(PDWNC_EXP_IRM0 + (8*(i+1)), u4Val << 4);
            vIO32Write4B(PDWNC_EXP_IRL0 + (8*(i+1)), 0);             
        }
        
    }
    else
    {
        // RC56 only check 0xffffff00 bits
        vIO32Write4B(PDWNC_ENEXP_IRM, 0x007fff00);//PDWNC_WRITE32(IREXP_M_BITMASK_REG, 0x007fff00);
        for (i=0; i<_u4RC56KeyMapSize; i++)
        {
            for (j=0; j<_arRC56KeyMapArray[i].u4Size; j++) // u4GroupId, prKeyMap
            {
                if ((u4KeyNum < MAX_EXP_KEY_NUM) && (_arRC56KeyMapArray[i].prKeyMap[j].u4Crystal == BTN_POWER) && _IRRX_IsSystemCodeAllowed(_arRC56KeyMapArray[i].u4GroupId))
                {
                    if(_arRC56KeyMapArray[i].prKeyMap[j].u1Type | WAKEUP_KEY_TYPE_RC5)
                    {                
                        RC5_SPLITTER_T rRc5Splitter;                                
                        
                        u4Val = u4IO32Read4B(PDWNC_IREXP_EN);//PDWNC_READ32(IREXP_EN_REG);
                        u4Val |= IREXP_EN(u4KeyNum);
                        vIO32Write4B(PDWNC_IREXP_EN, u4Val);//PDWNC_WRITE32(IREXP_EN_REG, u4Val);
                        
                        rRc5Splitter.u4Rc5Code = 0; 
                        rRc5Splitter.u.start = 3;
                        rRc5Splitter.u.toggle = 1;                    
                        rRc5Splitter.u.address = _arRC56KeyMapArray[i].u4GroupId;
                        rRc5Splitter.u.command = _arRC56KeyMapArray[i].prKeyMap[j].u4Key;
                        rRc5Splitter.u.reserved = 0;

                        u4Val = IR_PH_ReverseBit16((UINT16)(rRc5Splitter.u4Rc5Code & 0xffff));
                        u4Val = IR_PH_BiPhase(u4Val, 14);                        
                        u4Val >>= 2;
                        
                        vIO32Write4B(PDWNC_EXP_IRM0 + (8*u4KeyNum), u4Val);//PDWNC_WRITE32(IREXP_M0_REG + (8*u4KeyNum), u4Val);
                        vIO32Write4B(PDWNC_EXP_IRL0 + (8*u4KeyNum), 0);//PDWNC_WRITE32(IREXP_L0_REG + (8*u4KeyNum), 0);
                        u4KeyNum++;
                    }                
                    if((u4KeyNum < MAX_EXP_KEY_NUM) && (_arRC56KeyMapArray[i].prKeyMap[j].u1Type | WAKEUP_KEY_TYPE_RC6))
                    {                
                        RC6_SPLITTER_T rRc6Splitter;                                
                        
                        u4Val = u4IO32Read4B(PDWNC_IREXP_EN);//PDWNC_READ32(IREXP_EN_REG);
                        u4Val |= IREXP_EN(u4KeyNum);
                        vIO32Write4B(PDWNC_IREXP_EN, u4Val);
                        
                        rRc6Splitter.u4Rc6Mode0Code = 0;
                        rRc6Splitter.u.start = 3;
                        rRc6Splitter.u.mb = 0;
                        rRc6Splitter.u.toggle = 2;                    
                        rRc6Splitter.u.address = _arRC56KeyMapArray[i].u4GroupId;
                        rRc6Splitter.u.command = _arRC56KeyMapArray[i].prKeyMap[j].u4Key;
                        rRc6Splitter.u.tail = 0;                                             
                        u4Val = IR_PH_ReverseBit32(rRc6Splitter.u4Rc6Mode0Code);
                        
                        vIO32Write4B(PDWNC_EXP_IRM0 + (8*u4KeyNum), u4Val);//PDWNC_WRITE32(IREXP_M0_REG + (8*u4KeyNum), u4Val);
                        vIO32Write4B(PDWNC_EXP_IRL0 + (8*u4KeyNum), 0);//PDWNC_WRITE32(IREXP_L0_REG + (8*u4KeyNum), 0);
                        u4KeyNum++;
                    }
                }
            }
        }
        for (i=0; i<_u4RC56KeyMapSize; i++)
        {
            for (j=0; j<_arRC56KeyMapArray[i].u4Size; j++) // u4GroupId, prKeyMap
            {
                if (_IRRX_IsSystemCodeAllowed(_arRC56KeyMapArray[i].u4GroupId) &&
                	(((DRVCUST_InitGet(eIrrxFlagPrgUpDownWakeup)) &&
                     (u4KeyNum < MAX_EXP_KEY_NUM) &&
                     ((_arRC56KeyMapArray[i].prKeyMap[j].u4Crystal == BTN_PRG_UP) ||
                      (_arRC56KeyMapArray[i].prKeyMap[j].u4Crystal == BTN_PRG_DOWN))) ||
                    ((DRVCUST_InitGet(eStdbyUseVGAStatus)) &&
                     (u4KeyNum < MAX_EXP_KEY_NUM) &&
                     (u4IO32Read4B(PDWNC_WAKEN) & WAK_VGA) &&
                     ((_arRC56KeyMapArray[i].prKeyMap[j].u4Crystal == BTN_MENU) ||
                      (_arRC56KeyMapArray[i].prKeyMap[j].u4Crystal == BTN_INPUT_SRC)))))

                {
                    if(_arRC56KeyMapArray[i].prKeyMap[j].u1Type | WAKEUP_KEY_TYPE_RC5)
                    {
                        RC5_SPLITTER_T rRc5Splitter;                                
                        
                        u4Val = u4IO32Read4B(PDWNC_IREXP_EN);
                        u4Val |= IREXP_EN(u4KeyNum);
                        vIO32Write4B(PDWNC_IREXP_EN, u4Val);
                        
                        rRc5Splitter.u4Rc5Code = 0; 
                        rRc5Splitter.u.start = 3;
                        rRc5Splitter.u.toggle = 1;                    
                        rRc5Splitter.u.address = _arRC56KeyMapArray[i].u4GroupId;
                        rRc5Splitter.u.command = _arRC56KeyMapArray[i].prKeyMap[j].u4Key;
                        rRc5Splitter.u.reserved = 0;                         

                        u4Val = IR_PH_ReverseBit16((UINT16)(rRc5Splitter.u4Rc5Code & 0xffff));
                        u4Val = IR_PH_BiPhase(u4Val, 14);                        
                        u4Val >>= 2;
                        
                        vIO32Write4B(PDWNC_EXP_IRM0+ (8*u4KeyNum), u4Val);
                        vIO32Write4B(PDWNC_EXP_IRL0 + (8*u4KeyNum), 0);                    
                        u4KeyNum++;                        
                    }                
                    if((u4KeyNum < MAX_EXP_KEY_NUM) && (_arRC56KeyMapArray[i].prKeyMap[j].u1Type | WAKEUP_KEY_TYPE_RC6))
                    {
                        RC6_SPLITTER_T rRc6Splitter;                                
                        
                        u4Val = u4IO32Read4B(PDWNC_IREXP_EN);//PDWNC_READ32(IREXP_EN_REG);
                        u4Val |= IREXP_EN(u4KeyNum);
                        vIO32Write4B(PDWNC_IREXP_EN, u4Val);
                        
                        rRc6Splitter.u4Rc6Mode0Code = 0;
                        rRc6Splitter.u.start = 3;
                        rRc6Splitter.u.mb = 0;
                        rRc6Splitter.u.toggle = 2;                    
                        rRc6Splitter.u.address = _arRC56KeyMapArray[i].u4GroupId;
                        rRc6Splitter.u.command = _arRC56KeyMapArray[i].prKeyMap[j].u4Key;
                        rRc6Splitter.u.tail = 0;                         
                        u4Val = IR_PH_ReverseBit32(rRc6Splitter.u4Rc6Mode0Code);
                        
                        vIO32Write4B(PDWNC_EXP_IRM0 + (8*u4KeyNum), u4Val);
                        vIO32Write4B(PDWNC_EXP_IRL0 + (8*u4KeyNum), 0);
                        u4KeyNum++;                        
                    }
                }
            }
        }          
    }
}

//-----------------------------------------------------------------------------
/** _IRRX_XferNecToCrystal() the IRRX data to Crystal IRRX key code map
 *      function.
 *  @param u4Info the IRRX received related attributes of the current data.
 *  @param pu1Data the IRRX received and current data.
 *  @return the Crystal button key. BTN_NONE is no key.
 */
//-----------------------------------------------------------------------------
static UINT32 _IRRX_XferNecToCrystal(UINT32 u4Info, const UINT8 *pu1Data, IRRX_RAW_DATA_T *prRaw)
{
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
        LOG(9, "%s(%d) BTN_NONE\n", __FILE__, __LINE__);
        return BTN_NONE;
    }

#ifdef IRRX_NON_OSAI
    UNUSED(u4PrevKey);
#endif /* IRRX_NON_OSAI */

    /* Check repeat key. */
    if (u4BitCnt==1)
    {
#ifndef IRRX_NON_OSAI
        if ((INFO_TO_1STPULSE(u4Info) > DRVCUST_InitGet(eIrrxRepeatPulseDown)) &&
            (INFO_TO_1STPULSE(u4Info) < DRVCUST_InitGet(eIrrxRepeatPulseUp)) &&
            (INFO_TO_2NDPULSE(u4Info)==0) &&
            (INFO_TO_3RDPULSE(u4Info)==0))
        {
            HAL_GetTime(&rTime);
            HAL_GetDeltaTime(&rDelta, &rPrevTime, &rTime);

            if ((rDelta.u4Seconds > 0) || 
                (rDelta.u4Micros > (1000*_u4IrRxRepeatDuration)))
            {
                return BTN_NONE;
            }
            HAL_GetTime(&rPrevTime);

            prRaw->u4Len = 4;
            prRaw->u1Protocol = IRRX_RAW_DATA_NEC;
            if(DRVCUST_InitGet(eIrrcCheckRepeatByRaw) != 0)
            {
                x_memcpy((void*)prRaw->au1Data, (void*)pu1Data, 4);
            }
            else
            {
                x_memcpy((void*)prRaw->au1Data, (void*)_rCurrRaw.au1Data, 4);
            }
            return u4PrevKey;
        }
        else
        {
            LOG(9, "%s(%d) BTN_NONE\n", __FILE__, __LINE__);
            return BTN_NONE;
        }
#else /* IRRX_NON_OSAI */
        return BTN_NONE;
#endif /* IRRX_NON_OSAI */
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
        LOG(9, "%s(%d) BTN_NONE\n", __FILE__, __LINE__);
        return BTN_NONE;
    }

    u4GrpId = pu1Data[1];
    u4GrpId = (u4GrpId << 8) + pu1Data[0];

    /* Check invalid key. */
    if ((pu1Data[2]+pu1Data[3])!=0xff)
    {
        LOG(9, "%s(%d) BTN_NONE\n", __FILE__, __LINE__);
        return BTN_NONE;
    }

    /* Here, pu1Data[2] is the key of MTKDVD remote controller. */
    LOG(9, "KeyCode is 0x%02x\n", pu1Data[2]);

    u4Key = IRRX_GetCrystalKey(u4GrpId, pu1Data[2]);
#ifndef IRRX_NON_OSAI
    HAL_GetTime(&rPrevTime);
#endif /* IRRX_NON_OSAI */
    u4PrevKey = u4Key;

    prRaw->u4Len = 4;
    prRaw->u1Protocol = IRRX_RAW_DATA_NEC;
    x_memcpy((void*)prRaw->au1Data, (void*)pu1Data, 4);
    
    LOG(7, "NEC raw:0x%8x\n", (UINT32)prRaw->au1Data);

    return u4Key;
}

//-----------------------------------------------------------------------------
/** _IRRX_XferSharpToCrystal() the IRRX data to Crystal IRRX key code map
 *      function.
 *  @param u4Info the IRRX received related attributes of the current data.
 *  @param pu1Data the IRRX received and current data.
 *  @return the Crystal button key. BTN_NONE is no key.
 */
//-----------------------------------------------------------------------------
static UINT32 _IRRX_XferSharpToCrystal(UINT32 u4Info, const UINT8 *pu1Data, IRRX_RAW_DATA_T *prRaw)
{
    UINT32 u4Addr, u4BitCnt, u4Key = BTN_INVALID, u4Cmd, u4End;

    u4BitCnt = INFO_TO_BITCNT(u4Info);

    /* Check empty data. */
    if ((u4BitCnt==0) || (pu1Data==NULL))
    {
        LOG(9, "%s(%d) BTN_NONE\n", __FILE__, __LINE__);
        return BTN_NONE;
    }

    /* Check invalid pulse. */
    if (u4BitCnt != 15)
    {
        LOG(9, "%s(%d) BTN_NONE\n", __FILE__, __LINE__);
        return BTN_NONE;
    }

    u4Addr = pu1Data[0] & 0x1F;
    u4Cmd = ((pu1Data[0] >> 5) & 0x7) | ((pu1Data[1] & 0x1F) << 3);
    u4End = pu1Data[1] & 0x60;
    /* Check invalid key. */
    if(u4End == 0x20)
    {
        u4Key = IRRX_GetCrystalKey(u4Addr, u4Cmd);    
    }
    else if(u4End == 0x40)
    {
        u4Key = IRRX_GetCrystalKey(u4Addr, (~u4Cmd) & 0xFF);        
    }

    /* Here, pu1Data[2] is the key of MTKDVD remote controller. */
    LOG(9, "KeyCode is 0x%02x\n", u4Cmd);    

    prRaw->u1Protocol = IRRX_RAW_DATA_NONE;
    prRaw->u4Len = 4;
    *(UINT32*)prRaw->au1Data = *((UINT32*)pu1Data);
    
    LOG(7, "Sharp raw:0x%x\n", *(UINT32*)prRaw->au1Data);

    return u4Key;
}
//-----------------------------------------------------------------------------
// static functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/**  HalCountLeadingZero() Leave critical section, restore IRQ and/or FIQ status
 *  @param u4Flags - The return value of the corresponding \n
 *                     HalCriticalStart() or HalCriticalSemiStart()
 */
//-----------------------------------------------------------------------------
static UINT32 _HalCountLeadingZero(UINT32 r1)
{
     register UINT32 r0=0;
    #ifndef CC_MTK_LOADER
	#ifdef __KERNEL__
	__asm__ __volatile__("CLZ     r0, r1");
	#else
	  __asm
    {
        CLZ     r0, r1
    }
	#endif
	#endif
    return r0;
}

static UINT32 _FunaiCountBits(UINT32 au4Data[],INT32 index)
{
    UINT32 u4Pos =0;
    INT32 i;
    for(i = index; i >= 0; i--)
    {
        if((u4Pos = _HalCountLeadingZero(~au4Data[i])) < 32)
        {
            break;
        }
        else
        {
            au4Data[i] = 0;
        }
    }
    au4Data[i] &= ~(0xFFFFFFFF << (32 - u4Pos));
    return ((i << 5) + (32 -u4Pos));
}
//-----------------------------------------------------------------------------
/** _XferFunaiToCrystal() the IRRX data to Crystal IRRX key code map
 *      function.
 *  @param u4Info the IRRX received related attributes of the current data.
 *  @param pu1Data the IRRX received and current data.
 *  @return the Crystal button key. BTN_NONE is no key.
 */
//-----------------------------------------------------------------------------
static UINT32 _XferFunaiToCrystal(UINT32 u4Info, const UINT8 *pu1Data, IRRX_RAW_DATA_T* prRaw)
{
    UINT32 u4Crystal;
    UINT32 i;
    UINT32 au4Data[12];
    UINT32 u4BitNum;
	UINT8 *pu1ptr =  (UINT8*)(&(au4Data[0]));
    FUNAI_SPLITTER_T rCtrlSplitter;

    u4Crystal = BTN_NONE;    
    for(i = 0; i < 3; i++)
    {
        au4Data[i] = u4IO32Read4B(PDWNC_IROSDATA0 + (i << 2));
    }

    u4BitNum = _FunaiCountBits(au4Data,2);
	LOG(9, "Receive bits: %d:",u4BitNum);
#if 0
#ifndef CC_FUNAI_FACTORY_IR_EU 
    rCtrlSplitter.u4FunaiCtrlData = (au4Data[0] >> (FUNAI_START_DATA_BIT_NUM - 1)) & 0xFF;
#else
    rCtrlSplitter.u4FunaiCtrlData = (au4Data[0] >> (FUNAI_START_DATA_BIT_NUM - 1)) & 0x3FF;
#endif
#else
	au4Data[0] = au4Data[0] >> 2;
	au4Data[0] |= ((au4Data[1] & 0x00000003) << 30);
	au4Data[1] = au4Data[1] >> 2;
	au4Data[1] |= (au4Data[2] & 0x00000003) << 30;
	au4Data[2] = au4Data[2] >> 2;
	#ifndef CC_FUNAI_FACTORY_IR_EU 
		rCtrlSplitter.u4FunaiCtrlData = au4Data[0] & 0xFF;
	#else
		rCtrlSplitter.u4FunaiCtrlData = au4Data[0] & 0x3FF;
	#endif
#endif

    if(rCtrlSplitter.u.length == 0)
    {
        if(u4BitNum == (FUNAI_COMMON_DATA_NUM - 1))
        {
            u4Crystal = BTN_INVALID;
        }
    }
    else if(rCtrlSplitter.u.length == 1)
    {
        if( (u4BitNum == (FUNAI_COMMON_DATA_NUM - 1 + FUNAI_S_DATA_BIT_NUM)) ||
            (u4BitNum == (FUNAI_COMMON_DATA_NUM - 1 + FUNAI_L_DATA_BIT_NUM)) ) 
        {
            u4Crystal = BTN_INVALID;
        }
    }    
    else if(rCtrlSplitter.u.length == 2)
    {
        if( (u4BitNum == (FUNAI_COMMON_DATA_NUM - 1 + FUNAI_S_DATA_BIT_NUM * 3)) ||
            (u4BitNum == (FUNAI_COMMON_DATA_NUM - 1 + FUNAI_L_DATA_BIT_NUM * 3)) )
        {
            u4Crystal = BTN_INVALID;
        }
    }
    else
    {
		if((u4BitNum == (FUNAI_COMMON_DATA_NUM - 1 + (FUNAI_L_DATA_BIT_NUM + FUNAI_S_DATA_BIT_NUM) * 3)) || (u4BitNum == (FUNAI_COMMON_DATA_NUM - 1 + FUNAI_S_DATA_BIT_NUM * 6)))
        {
            u4Crystal = BTN_INVALID;
        }    
    }

    //  TODO: check CRC

    prRaw->u4Len = ((u4BitNum -1) >> 3) + 1;
	if(prRaw->u4Len > IRRX_MAX_RAW_DATA_SIZE)
    {        
        LOG(5, "Warning! length of ir raw data larger than IRRX_MAX_RAW_DATA_SIZE(%d)!", IRRX_MAX_RAW_DATA_SIZE);	    
        prRaw->u4Len = IRRX_MAX_RAW_DATA_SIZE;
    }
    prRaw->u1Protocol = IRRX_RAW_DATA_FACTORY; //IRRX_RAW_DATA_FACTORY; // temp solution
    LOG(5, "Receive Funai bits:");
  //  pu1ptr = (UINT8*)(&(au4Data[0]));
    for(i = 0; i < (prRaw->u4Len); i++)
    {
        (prRaw->au1Data[i]) = pu1ptr[i];
        LOG(5, " 0x%2x", *(UINT32*)&(prRaw->au1Data[i << 2]));
    }


    LOG(5, "\n");
    
    // lookup table ??
    
    return u4Crystal;
}

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/** _IRRX_SetNecWakeupKey() to set NEC wakeup key
 */
//-----------------------------------------------------------------------------
static void _IRRX_SetNecWakeupKey()
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

//-----------------------------------------------------------------------------
/** _IRRX_SetSharpWakeupKey() to set SHARP wakeup key
 */
//-----------------------------------------------------------------------------
static void _IRRX_SetSharpWakeupKey()
{
    UINT32 i, j, u4KeyNum, u4Val;

    if (_arKeyArray==NULL)
    {
        LOG(1, "Sharp IR key array is not found.\n");
        return;
    }

    u4KeyNum = 0;
    vIO32Write4B(PDWNC_IREXP_EN, IREXP_BIT_EN);
    vIO32Write4B(PDWNC_EXP_BCNT, 0xf);
    vIO32Write4B(PDWNC_ENEXP_IRL, 0);    

    if(DRVCUST_InitGet(eIrrxFlagUserDefineWakeup) && _arRC56KeyMapArray != NULL)
    {      
        vIO32Write4B(PDWNC_ENEXP_IRM, 0xffffffff);
        for (i=0; i<_u4RC56KeyMapSize; i++)
        {
            for (j=0; j<_arRC56KeyMapArray[i].u4Size; j++) // u4GroupId, prKeyMap
            {
                if ((u4KeyNum < MAX_EXP_KEY_NUM) && (_arRC56KeyMapArray[i].prKeyMap[j].u1Type == WAKEUP_KEY_TYPE_SHARP))
                {
                    u4Val = u4IO32Read4B(PDWNC_IREXP_EN);
                    u4Val |= IREXP_EN(u4KeyNum);
                    vIO32Write4B(PDWNC_IREXP_EN, u4Val);
                    u4Val = (_arRC56KeyMapArray[i].u4GroupId) |
                            (_arRC56KeyMapArray[i].prKeyMap[j].u4Key << 5) |
                            (0x2000);
                    vIO32Write4B(PDWNC_EXP_IRM0 + (8*u4KeyNum), u4Val);
                    vIO32Write4B(PDWNC_EXP_IRL0 + (8*u4KeyNum), 0);
                    u4KeyNum++;
                }
            }
        }
        return;
    }
    
    if(DRVCUST_InitGet(eIrrxFlagAllWakeup))
    {
        vIO32Write4B(PDWNC_ENEXP_IRM, 0x00001f);

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
                            (_arKeyArray[i].prKeyMap[j].u4Key << 5) |
                            (0x2000);
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
                            (_arKeyArray[i].prKeyMap[j].u4Key << 5) |
                            (0x2000);
                    vIO32Write4B(PDWNC_EXP_IRM0 + (8*u4KeyNum), u4Val);
                    vIO32Write4B(PDWNC_EXP_IRL0 + (8*u4KeyNum), 0);
                    u4KeyNum++;
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------
/** _IRRX_XferToCrystal() the callback function from IRRX ISR routine
 *      function.
 *  @param u4Info the IRRX received related attributes of the current data.
 *  @param pu1Data the IRRX received and current data.
 */
//-----------------------------------------------------------------------------
static UINT32 _IRRX_XferToCrystal(UINT32 u4Info, const UINT8 *pu1Data, IRRX_RAW_DATA_T *prRaw)
{
    UINT32 u4CrystalKey = BTN_NONE;

    if (_gu4IRRXType == IRRX_RC5_TYPE)
    {
        u4CrystalKey = _IRRX_XferRC5ToCrystal(u4Info, pu1Data, prRaw);
    }
    else if (_gu4IRRXType == IRRX_SHARP_TYPE)
    {
        u4CrystalKey = _IRRX_XferSharpToCrystal(u4Info, pu1Data, prRaw);
    }        
    else if ((_gu4IRRXType == IRRX_USR_TYPE) && (_pfnXferUsrToCrystal != NULL))
    {
        u4CrystalKey = _pfnXferUsrToCrystal(u4Info, pu1Data, prRaw);
    }
    else if (_gu4IRRXType == IRRX_NEC_TYPE)
    {
        u4CrystalKey = _IRRX_XferNecToCrystal(u4Info, pu1Data, prRaw);
    }
	else if (_gu4IRRXType == IRRX_FUNAI_FACTORY_TYPE)
    {
        u4CrystalKey = _XferFunaiToCrystal(u4Info, pu1Data, prRaw);
    }
    return u4CrystalKey;
}

#ifndef IRRX_NON_OSAI
//-----------------------------------------------------------------------------
/** _IRRX_MtkIrCallback() the callback function from IRRX ISR routine
 *      function.
 *  @param u4Info the IRRX received related attributes of the current data.
 *  @param pu1Data the IRRX received and current data.
 */
//-----------------------------------------------------------------------------
static void _IRRX_MtkIrCallback(UINT32 u4Info, const UINT8 *pu1Data)
{
#if CC_WITH_DRVKEY_REPEAT
    static UINT32 u4PrevKey = BTN_NONE;
    static HAL_TIME_T rPrevTime = { 0, 0 };
    HAL_TIME_T rTime, rDelta;
#endif
    INT32 i4Ret;
    UINT32 u4CrystalKey;
//    UINT32 u4Raw;
    IRRX_RAW_DATA_T rRaw;
    rRaw.u1Protocol = IRRX_RAW_DATA_NONE;      
    rRaw.u4Len = 1;            

    if ((u4Info==0) || (pu1Data==NULL))
    {
        LOG(0, "%s(%d) NULL u4Info or pu1Data\n",__FILE__, __LINE__);
        ASSERT(0);
        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return;
    }

    u4CrystalKey = _IRRX_XferToCrystal(u4Info, pu1Data, &rRaw);
	//rTime, rDelta

    if (u4CrystalKey == BTN_NONE)
    {
        LOG(11, "%s(%d) u4CrystalKey is 0xffffffff u4Info:0x%08x ",
                __FILE__, __LINE__, u4Info);
        LOG(11, "pu1Data:0x%02x%02x%02x%02x\n",
                pu1Data[0], pu1Data[1], pu1Data[2], pu1Data[3]);
        return;
    }
    LOG(9, "u4CrystalKey is 0x%08x\n", u4CrystalKey);

#if CC_WITH_DRVKEY_REPEAT
    HAL_GetTime(&rTime);
    HAL_GetDeltaTime(&rDelta, &rPrevTime, &rTime);

    if (u4CrystalKey == BTN_DRVKEY_REPEAT)
    {
        if ((rDelta.u4Seconds > 0) || 
            (rDelta.u4Micros > (1000*_u4IrRxRepeatDuration)))
        {
            LOG(9, "Repeat code don't care after %d ms\n", _u4IrRxRepeatDuration);
            return;
        }
        if (!_fgRepeat)
        {
            u4CrystalKey = u4PrevKey;
        }
    }

    if ((u4CrystalKey == BTN_DRVKEY_REPEAT) || (u4CrystalKey == u4PrevKey))
    {
        if ((rDelta.u4Seconds == 0) &&
                    (rDelta.u4Micros < (1000*(_u4IrRxTimeSlice))))
        {
            LOG(7, "Repeat code but in %d timeslice.\n", (_u4IrRxTimeSlice));
            return;
        }
    }

    if ((u4CrystalKey == BTN_DRVKEY_REPEAT) && (_u4CurrKey != BTN_NONE))
    {
        LOG(9, "repeat key cannot overwrite an existed key code\n");
        return;
    }

    HAL_GetTime(&rPrevTime);
#endif

//    _u4CurrRaw = u4Raw;
    _rCurrRaw.u1Protocol = rRaw.u1Protocol;
    _rCurrRaw.u4Len = rRaw.u4Len;
    x_memcpy(_rCurrRaw.au1Data, rRaw.au1Data, rRaw.u4Len);

    _u4CurrKey = u4CrystalKey;

    i4Ret = x_sema_unlock(_hSemaKey);
    if (i4Ret != OSR_OK)
    {
        LOG(1, "%s(%d) SemaKey is fault. i4Ret:%d\n", __FILE__, __LINE__, i4Ret);
    }
}

static void _IRRX_BtnSendKey(UINT32 u4Key)
{
#if CC_WITH_DRVKEY_REPEAT
    static HAL_TIME_T rPrevTime = { 0, 0 };
    static UINT32 u4PrevKey = BTN_NONE;
    HAL_TIME_T rTime, rDelta;

    HAL_GetTime(&rTime);
    HAL_GetDeltaTime(&rDelta, &rPrevTime, &rTime);

    if ((u4Key == u4PrevKey) &&
        (rDelta.u4Seconds == 0) &&
                (rDelta.u4Micros < (1000*(_u4IrRxTimeSlice))))
    {
        HAL_GetTime(&rPrevTime);
        VERIFY(IRRX_SendMtkIr(BTN_DRVKEY_REPEAT)==0);
        return;
    }
    HAL_GetTime(&rPrevTime);
    u4PrevKey = u4Key;
#endif

    VERIFY(IRRX_SendMtkIr(u4Key)==0);
    return;
}

static UINT32 _IRRX_Dkbit2CrystalKey(MULTI_KEY_T const *prMultiKeyList, UINT32 u4Key)
{
    INT32 i;

    switch(u4Key)
    {
    case DKBIT_POWER:
        return BTN_POWER;
    case DKBIT_MENU:
        return BTN_MENU;
    case DKBIT_INPUT_SRC:
        return BTN_INPUT_SRC;
    case DKBIT_PRG_UP:
        return BTN_PRG_UP;
    case DKBIT_PRG_DOWN:
        return BTN_PRG_DOWN;
    case DKBIT_VOL_UP:
        return BTN_VOL_UP;
    case DKBIT_VOL_DOWN:
        return BTN_VOL_DOWN;
    case DKBIT_EJECT:
        return BTN_EJECT;
    case DKBIT_PLAY_PAUSE:
        return BTN_PLAY_PAUSE;
    case DKBIT_STOP:
        return BTN_STOP;
    case DKBIT_FACTORY:
        return BTN_FACTORY_MODE_1;
    case DKBIT_ENTER:
        return BTN_SELECT;	
    case DKBIT_EXIT:
        return BTN_EXIT;	        
    default:
        i = 0;
        while ((prMultiKeyList) &&
                (prMultiKeyList[i].u4CrystalKey != 0) &&
                (prMultiKeyList[i].u4CrystalKey != BTN_NONE))
        {
            if (prMultiKeyList[i].u4DrvKeyBits == u4Key)
            {
                return prMultiKeyList[i].u4CrystalKey;
            }
            i++;
        }
    }
    return BTN_NONE;
}
#endif /* IRRX_NON_OSAI */

static UINT32 _IRRX_ReadSvadc(UINT32 u4Channel)
{
    UINT32 u4Val;

    do {
        u4Val = PDWNC_ReadServoADCChannelValue(u4Channel);
    } while (u4Val != PDWNC_ReadServoADCChannelValue(u4Channel));
    return u4Val;
}

#ifndef IRRX_NON_OSAI

//lint -emacro({717}, POLL_CHECK_KEY)  to disable "do {...} while (0)" lint warning
#define POLL_CHECK_KEY(key)                         \
    do {                                            \
        u4Key |= key;                               \
    } while (0)

//-----------------------------------------------------------------------------
/* _IRRX_BtnPollThread()
 */
//-----------------------------------------------------------------------------
static void _IRRX_BtnPollThread(void *pvDummy)
{
    INT32 i, i4Val;
    UINT32 u4Val;

    UINT32 u4PrevKey = 0;
    UINT32 u4Count = 0;
    UINT32 u4Key, u4KeyCount, u4ThreadDelay;

    SGL_GPIO_T  *prSingleGpioKeyList = NULL;
    SGL_SERVO_T *prSingleServoKeyList = NULL;
    GREED_KEY_T *prGreedGpioKeyList = NULL;
    MULTI_KEY_T *prMultiKeyList = NULL;
#if IC_VERIFICATION
    static BOOL fgLoop = TRUE;
    while(fgLoop)
    {
        for(i = 0 ; i < 5; i++)
        {
            u4Val = _IRRX_ReadSvadc(i);
            LOG(7, "Servo %d's value: %d\n", i, u4Val);
        }
        x_thread_delay(100);
    }
    return;
#endif

    UNUSED(pvDummy);

    i = DRVCUST_OptQuery(eSingleGpioKeyList, (UINT32 *)(void *)&prSingleGpioKeyList);
    if (i < 0)
    {
        prSingleGpioKeyList = NULL;
    }
    i = DRVCUST_OptQuery(eSingleServoKeyList, (UINT32 *)(void *)&prSingleServoKeyList);
    if (i < 0)
    {
        prSingleServoKeyList = NULL;
    }
    i = DRVCUST_OptQuery(eGreedGpioKeyList, (UINT32 *)(void *)&prGreedGpioKeyList);
    if (i < 0)
    {
        prGreedGpioKeyList = NULL;
    }
    i = DRVCUST_OptQuery(eMultiKeyList, (UINT32 *)(void *)&prMultiKeyList);
    if (i < 0)
    {
        prMultiKeyList = NULL;
    }

    if ((prSingleGpioKeyList == NULL) &&
        (prSingleServoKeyList == NULL) &&
        (prGreedGpioKeyList == NULL))
    {
        LOG(1, "Support button but without key define.\n");
        return;
    }

    u4ThreadDelay = DRVCUST_OptGet(eKeyPadPollingTime);
    u4KeyCount = DRVCUST_OptGet(eKeyPadCheckTime);

    // Start button polling...
    while (1)
    {

		
        // Start to poll key pad status.
        u4Key = 0;

        // Check Gpio single key
        i = 0;
        while ((prSingleGpioKeyList) &&
                (prSingleGpioKeyList[i].u4Key != 0) &&
                (prSingleGpioKeyList[i].u4Key != DKBIT_NONE))
        {
            i4Val = GPIO_Input((INT32)prSingleGpioKeyList[i].u4Gpio);
            u4Val = (UINT32)i4Val;
            if (u4Val == prSingleGpioKeyList[i].u4Val)
            {
                POLL_CHECK_KEY(prSingleGpioKeyList[i].u4Key);
            }
            i++;
        }

        // Check Servo sigle key
        i = 0;
        while ((prSingleServoKeyList) &&
                (prSingleServoKeyList[i].u4Key != 0) &&
                (prSingleServoKeyList[i].u4Key != DKBIT_NONE))
        {
            u4Val = _IRRX_ReadSvadc(prSingleServoKeyList[i].u4Servo);
            if ((prSingleServoKeyList[i].u4Top >= u4Val) &&			//<Tom>2k71221
                (u4Val >= prSingleServoKeyList[i].u4Btm))
            {
                POLL_CHECK_KEY(prSingleServoKeyList[i].u4Key);
            }
            i++;
        }

        // Check Greed key
        i = 0;
        while ((prGreedGpioKeyList) &&
                (prGreedGpioKeyList[i].u4Key != 0) &&
                (prGreedGpioKeyList[i].u4Key != DKBIT_NONE))
        {
            i4Val = (INT32)prGreedGpioKeyList[i].u4Val;
            VERIFY(i4Val==GPIO_Output((INT32)prGreedGpioKeyList[i].u4GpioOut, &i4Val));
            if (i4Val == GPIO_Input((INT32)prGreedGpioKeyList[i].u4GpioIn))
            {
                POLL_CHECK_KEY(prGreedGpioKeyList[i].u4Key);
            }
            i4Val = !i4Val;
            VERIFY(i4Val==GPIO_Output((INT32)prGreedGpioKeyList[i].u4GpioOut, &i4Val));
            i++;
        }

        if (u4Key != 0)
        {
            LOG(9, "Count:%d KeyBit:0x%08x\n", u4Count, u4Key);
        }

        // Polling key logic
        if (u4Key == 0)
        {
            u4Count = 0;
        }
        else if (u4PrevKey != u4Key)
        {
            u4Count = 1;
        }
        else if (u4Count < u4KeyCount)
        {
            u4Count++;
        }
        else if (u4Count >= u4KeyCount)
        {
#if CC_WITH_DRVKEY_REPEAT
            static UINT32 u4Repeat = 0;
            static HAL_TIME_T rPrevTime = { 0, 0 };
            HAL_TIME_T rTime, rDelta;
#endif
            UINT32 u4CrystalKey;

            u4CrystalKey = _IRRX_Dkbit2CrystalKey(prMultiKeyList, u4Key);

            LOG(9, "Count:%d Key:0x%08x Crystal:0x%08x\n", u4Count, u4Key, u4CrystalKey);

#if 0
            if ((DRVCUST_OptGet(eFlagFactoryEdid)) && (u4CrystalKey == BTN_FACTORY_MODE_1))
            {
                i4Val2 = DRVCUST_OptQuery(eEdidEepromWPGpio, &u4Val);
                if (i4Val2 == 0)
                {
                    i4Val = (INT32)u4Val; // Gpio number
                    // Set the gpio as 0, i4Val2 is 0.
                    VERIFY(GPIO_Output(i4Val, &i4Val2)==i4Val2);
                }
            }
#endif

#if CC_WITH_DRVKEY_REPEAT
            HAL_GetTime(&rTime);
            HAL_GetDeltaTime(&rDelta, &rPrevTime, &rTime);
            if ((u4Repeat == u4CrystalKey) && (rDelta.u4Seconds == 0) &&
                (rDelta.u4Micros < (1000*400)))
            {
                _IRRX_BtnSendKey(BTN_DRVKEY_REPEAT);
            }
            else
            {
                _IRRX_BtnSendKey(u4CrystalKey | KEY_SRC_FROM_BUTTON);
                u4Repeat = u4CrystalKey;
            }
            HAL_GetTime(&rPrevTime);
#else
    #if 0
            if (_fgKeyboardSendEventCode)
            {
                _IRRX_BtnSendKey(u4CrystalKey | KEY_SRC_FROM_BUTTON);
            }
            else
            {
                //_u4CurrRaw = u4CrystalKey;
                _IRRX_BtnSendKey(BTN_FACTORY_FRNT_PANEL | KEY_SRC_FROM_BUTTON);
            }
    #else
            _IRRX_BtnSendKey(u4CrystalKey | KEY_SRC_FROM_BUTTON);
    #endif            
#endif
            u4Count = 0;
        }

        u4PrevKey = u4Key;
        x_thread_delay(u4ThreadDelay);
    }
}

//-----------------------------------------------------------------------------
/** _PoolMtkIr() to check there is any key available.
 *  @retval IR_FAIL run failed.
 *  @retval IR_OK run successfully.
 */
//-----------------------------------------------------------------------------
static INT32 _PollMtkIr(UINT32 *pu4Key, IRRX_RAW_DATA_T *prRaw)
{
    static UINT32 u4PrevKey = BTN_NONE;
    INT32 i4Ret;
    CRIT_STATE_T cState;

    if ((pu4Key == NULL) || (prRaw == NULL))
    {
        return IR_FAIL;
    }
    
    #ifdef CHANNEL_CHANGE_LOG
    if ((*pu4Key == BTN_PRG_DOWN) || (*pu4Key == BTN_PRG_UP) || (*pu4Key == BTN_PREV_PRG))
    {
        HAL_GetTime(&dt_press_key);
        LOG(0, " %u.%06u s [AV SYNC] 1 Change Channel\n", dt_press_key.u4Seconds, dt_press_key.u4Micros);
    }
    #endif
    
    PDWNC_IrLedCheck(1);    // turn on
    i4Ret = x_sema_lock_timeout(_hSemaKey, _u4IrRxTimeout);   
    if (i4Ret == OSR_OK)
    {
        cState = x_crit_start();
        
//        *pu4Raw = _u4CurrRaw;
        prRaw->u1Protocol = _rCurrRaw.u1Protocol;
        prRaw->u4Len = _rCurrRaw.u4Len;
        x_memcpy(prRaw->au1Data, _rCurrRaw.au1Data, _rCurrRaw.u4Len);
        
        *pu4Key = _u4CurrKey;
        _u4CurrKey = BTN_NONE;
        x_crit_end(cState);
    }
    else if (i4Ret == OSR_TIMEOUT)
    {
        prRaw->u1Protocol = 0;
        prRaw->u4Len = 0;
        x_memset(prRaw->au1Data, 0, IRRX_MAX_RAW_DATA_SIZE);
        *pu4Key = BTN_NONE;
    }
    else
    {
        ASSERT((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));
        *pu4Key = BTN_NONE;
        return IR_FAIL;
    }

    if (*pu4Key == BTN_NONE)
    {
        LOG(10, "%s return Key:0x%08x\n", __FUNCTION__, *pu4Key);
    }
    else
    {
        if (*pu4Key != BTN_DRVKEY_REPEAT)
        {
            u4PrevKey = *pu4Key;
        }
        if (!(u4PrevKey & KEY_SRC_FROM_BUTTON)) // only from IR.
        {
            PDWNC_IrLedCheck(0); // turn off.
        }
        LOG(7, "%s return Key:0x%08x\n", __FUNCTION__, *pu4Key);
    }

    return IR_SUCC;
}

//-----------------------------------------------------------------------------
/** _PoolSharpIr() to check there is any key available.
 *  @retval IR_FAIL run failed.
 *  @retval IR_OK run successfully.
 */
//-----------------------------------------------------------------------------
static INT32 _PollSharpIr(UINT32 *pu4Key, IRRX_RAW_DATA_T *prRaw)
{
    UINT32 u4Key, u4Key2, u4RawTemp;
    IRRX_RAW_DATA_T rRaw;
    
    if(_PollMtkIr(&u4Key, &rRaw) != IR_SUCC)
    {
        return IR_FAIL;    
    }

    if( (u4Key != BTN_NONE) && ((u4Key & KEY_SRC_FRONT_PANEL) == KEY_SRC_FRONT_PANEL))
    {
        *pu4Key = u4Key;
        
//        *pu4Raw = u4Raw;
        prRaw->u1Protocol = rRaw.u1Protocol;
        prRaw->u4Len = rRaw.u4Len;
        x_memcpy(prRaw->au1Data, rRaw.au1Data, rRaw.u4Len);

        return IR_SUCC;
    }

    u4RawTemp = *(UINT32*)rRaw.au1Data;

    if((*(UINT32*)rRaw.au1Data & 0x6000) == 0x4000)
    {            
        if(_PollMtkIr(&u4Key2, &rRaw) != IR_SUCC)
        {
            return IR_FAIL;
        }        
    }            
    if(u4Key == BTN_NONE || u4Key == BTN_INVALID)
    {            
        *pu4Key = u4Key;
//        *pu4Raw = u4Raw;
        prRaw->u1Protocol = rRaw.u1Protocol;
        prRaw->u4Len = rRaw.u4Len;
        x_memcpy(prRaw->au1Data, &u4RawTemp, rRaw.u4Len);

        return IR_SUCC;    
    }
    else
    {
        if(_PollMtkIr(&u4Key2, &rRaw) != IR_SUCC)
        {
            return IR_FAIL;
        }
        if((*(UINT32*)rRaw.au1Data & 0x6000) == 0x2000)
        {            
            if(_PollMtkIr(&u4Key2, &rRaw) != IR_SUCC)
            {
                return IR_FAIL;
            }        
        }        
        LOG(9, "1st frame: 0x%8x, 2nd frame: 0x%8x\n", u4Key, u4Key2);
        if(((u4RawTemp >> 5) & 0x3FF) + ((*(UINT32*)rRaw.au1Data >> 5) & 0x3FF) == 0x3FF)
        {
            *pu4Key = u4Key;
//            *pu4Raw = u4Raw;
        prRaw->u1Protocol = rRaw.u1Protocol;
        prRaw->u4Len = rRaw.u4Len;
        x_memcpy(prRaw->au1Data, rRaw.au1Data, rRaw.u4Len);            
        
        }
        else
        {
            *pu4Key = BTN_NONE;
        }
    }    

    return IR_SUCC;
}

    
#endif /* IRRX_NON_OSAI */

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** IRRX_IsTheKey() to check the ir register content is the key of input.
 *  @param u4Key The key to check.
 *  @retval TRUE for yes, FALSE for no.
 */
//-----------------------------------------------------------------------------
INT32 IRRX_IsTheKey(UINT32 u4Key)
{
    UINT32 u4Info;
    UINT32 au4IrRxData[2];
    IRRX_RAW_DATA_T rRaw;

    UNUSED(IRRX_InitMtkIrRegSetup(DRVCUST_InitGet(eIrrxType)));
    u4Info = u4IO32Read4B(PDWNC_IRH);
    au4IrRxData[0] = u4IO32Read4B(PDWNC_IRM);
    au4IrRxData[1] = u4IO32Read4B(PDWNC_IRL);
    LOG(7, "PWD:0x%08x RxIsr Info:0x%08x 0x%08x%08x\n",
            u4IO32Read4B(PDWNC_INTSTA),
            u4Info, au4IrRxData[1], au4IrRxData[0]);
    return (u4Key == _IRRX_XferToCrystal(u4Info, (const UINT8 *)au4IrRxData, &rRaw));
}

//-----------------------------------------------------------------------------
/** IRRX_InitMtkIrRegSetup() to initial IRRX register setup.
 *  @param u4Type to set RC56 or NEC IRRX type.
 *  @retval IR_FAIL run failed.
 *  @retval IR_OK run successfully.
 */
//-----------------------------------------------------------------------------
INT32 IRRX_InitMtkIrRegSetup(UINT32 u4Type)
{
    INT32 i4Ret;
    UINT32 i;
    // Initial IR data
    i4Ret = DRVCUST_InitQuery(eIrrxType, &_gu4IRRXType);
#ifndef IRRX_NON_OSAI
    i4Ret = DRVCUST_InitQuery(eIrrxKeyUpTimeout, &_u4IrRxTimeout);
    i4Ret = DRVCUST_InitQuery(eIrrxRepeatValidTime, &_u4IrRxRepeatDuration);
#endif /* IRRX_NON_OSAI */
    i4Ret = DRVCUST_InitQuery(eIrrxNecKeyMap, (UINT32 *)(void *)&(_arKeyArray));
    i4Ret = DRVCUST_InitQuery(eIrrxNecMapArraySize, &_u4IRKeyArraySize);
    i4Ret = DRVCUST_InitQuery(eIrrxNecPulse1, &_u4_1stPulse);
    i4Ret = DRVCUST_InitQuery(eIrrxNecPulse2, &_u4_2ndPulse);
    i4Ret = DRVCUST_InitQuery(eIrrxNecPulse3, &_u4_3rdPulse);
    i4Ret = DRVCUST_InitQuery(eIrrxRc5KeyMap, (UINT32 *)(void *)&(_arRC56KeyMapArray));
    i4Ret = DRVCUST_InitQuery(eIrrxRc5MapArraySize, &_u4RC56KeyMapSize);
    i4Ret = DRVCUST_InitQuery(eIrrxRc5KeyMapFunc, (UINT32 *)(void *)&(_pfnRC5KeyLookup));
    i4Ret = DRVCUST_InitQuery(eIrrxRc6KeyMapFunc, (UINT32 *)(void *)&(_pfnRC6KeyLookup));
    i4Ret = DRVCUST_InitQuery(eIrrxXferUsrToCrystalFunc, (UINT32 *)(void *)&(_pfnXferUsrToCrystal));
    i4Ret = DRVCUST_InitQuery(eIrrxSetUsrWakeupKeyFunc, (UINT32 *)(void *)&(_pfnSetUsrWakeupKey));

    // ASSERT((_gu4IRRXType!=IRRX_NEC_TYPE) || (_u4IRKeyArraySize!=0));
    // ASSERT((_gu4IRRXType!=IRRX_RC5_TYPE) || (_u4RC56KeyMapSize!=0));

    _gu4IRRXType = u4Type;
    if (u4Type == IRRX_RC5_TYPE)
    {
        _IRRX_SetRC5WakeupKey();
        IRHW_RxWrConf(RC56_IRRX_CONFIG, RC56_IRRX_SAPERIOD, RC56_IRRX_THRESHOLD);
    }
    else if (u4Type == IRRX_NEC_TYPE)
    {
        _IRRX_SetNecWakeupKey();
        IRHW_RxWrConf(NEC_IRRX_CONFIG, NEC_IRRX_SAPERIOD,
                    NEC_IRRX_THRESHOLD);
    }
    else if (u4Type == IRRX_SHARP_TYPE)
    {
        _IRRX_SetSharpWakeupKey();
        IRHW_RxWrConf(SHARP_IRRX_CONFIG, SHARP_IRRX_SAPERIOD, SHARP_IRRX_THRESHOLD);
    }   
	else if (u4Type == IRRX_FUNAI_FACTORY_TYPE)
    {
//        _IRRX_SetSharpWakeupKey();
        IRHW_RxWrConf(FUNAI_FACTORY_IRRX_CONFIG, FUNAI_FACTORY_IRRX_SAPERIOD, FUNAI_FACTORY_IRRX_THRESHOLD);
    }   
    else if ((u4Type == IRRX_USR_TYPE) && (_pfnSetUsrWakeupKey != NULL))
    {
        _pfnSetUsrWakeupKey();
        IRHW_RxWrConf((INT32)DRVCUST_InitGet(eIrrxUsrConfig),
                            (INT32)DRVCUST_InitGet(eIrrxUsrSaperiod),
                            (INT32)DRVCUST_InitGet(eIrrxUsrThreshold));
        if (DRVCUST_InitQuery(eUsrIRClkSel, &i) == 0)
        {
            vIO32WriteFldAlign(PDWNC_IRCKSEL, (UINT8)i, FLD_IRCLKSEL);
            Printf("Set IRCLKSEL as %d for USR TYPE IR!\n", u4IO32Read4B(PDWNC_IRCKSEL));                            
        }                            
    }
    else
    {
        LOG(1, "Error IRRX type :%d\n", u4Type);
        return IR_FAIL;
    }

    if (i4Ret != IR_SUCC)
    {
        return IR_FAIL;
    }

    return IR_SUCC;
}

#if defined(CC_FAST_INIT)
#ifndef IRRX_NON_OSAI
INT32 IRRX_InitMtkIr_pm_resume(UINT32 u4Type)
{
    INT32 i4Ret;
    UINT32 i;
    // Setup register and parameter.
    i4Ret = IRRX_InitMtkIrRegSetup(u4Type);
    if (i4Ret != IR_SUCC)
    {
        return IR_FAIL;
    }

    if (u4Type == IRRX_RC5_TYPE)
    {
        i4Ret = IRHW_RxInit(RC56_IRRX_CONFIG, RC56_IRRX_SAPERIOD,
                    RC56_IRRX_THRESHOLD);
    }
    else if (u4Type == IRRX_NEC_TYPE)
    {
        i4Ret = IRHW_RxInit(NEC_IRRX_CONFIG, NEC_IRRX_SAPERIOD,
                    NEC_IRRX_THRESHOLD);
    }
    else if (u4Type == IRRX_SHARP_TYPE)
    {
        i4Ret = IRHW_RxInit(SHARP_IRRX_CONFIG, SHARP_IRRX_SAPERIOD,
                    SHARP_IRRX_THRESHOLD);
    }    
    else if (u4Type == IRRX_USR_TYPE)
    {
        i4Ret = IRHW_RxInit((INT32)DRVCUST_InitGet(eIrrxUsrConfig),
                            (INT32)DRVCUST_InitGet(eIrrxUsrSaperiod),
                            (INT32)DRVCUST_InitGet(eIrrxUsrThreshold));
        if (DRVCUST_InitQuery(eUsrIRClkSel, &i) == 0)
        {
            vIO32WriteFldAlign(PDWNC_IRCKSEL, (UINT8)i, FLD_IRCLKSEL);
            Printf("Set IRCLKSEL as %d for USR TYPE IR!\n", u4IO32Read4B(PDWNC_IRCKSEL));                            
        }                             
    }

    return i4Ret;
}

INT32 IRRX_pm_resume(void)
{
    return IRRX_InitMtkIr_pm_resume(DRVCUST_OptGet(eIrrxType));
}
#endif
#endif

#ifndef IRRX_NON_OSAI
//-----------------------------------------------------------------------------
/** IRRX_InitMtkIr() to initialize the IRRX of MTK TV remote controller unit.
 *  @param u4Type to set RC56 or NEC IRRX type.
 *  @retval IR_FAIL run failed.
 *  @retval IR_OK run successfully.
 */
//-----------------------------------------------------------------------------
INT32 IRRX_InitMtkIr(UINT32 u4Type)
{
    INT32 i4Ret;
    PFN_IRRXCB_T pfnOld;
    UINT32 i;
    // Setup register and parameter.
    i4Ret = IRRX_InitMtkIrRegSetup(u4Type);
    if (i4Ret != IR_SUCC)
    {
        return IR_FAIL;
    }

    if (u4Type == IRRX_RC5_TYPE)
    {
        i4Ret = IRHW_RxInit(RC56_IRRX_CONFIG, RC56_IRRX_SAPERIOD,
                    RC56_IRRX_THRESHOLD);
    }
    else if (u4Type == IRRX_NEC_TYPE)
    {
        i4Ret = IRHW_RxInit(NEC_IRRX_CONFIG, NEC_IRRX_SAPERIOD,
                    NEC_IRRX_THRESHOLD);
    }
    else if (u4Type == IRRX_SHARP_TYPE)
    {
        i4Ret = IRHW_RxInit(SHARP_IRRX_CONFIG, SHARP_IRRX_SAPERIOD,
                    SHARP_IRRX_THRESHOLD);
    } 
   else if (u4Type == IRRX_FUNAI_FACTORY_TYPE)
    {
	    Printf("Set IRRX_FUNAI_FACTORY_TYPE!\n");
        i4Ret = IRHW_RxInit(FUNAI_FACTORY_IRRX_CONFIG, FUNAI_FACTORY_IRRX_SAPERIOD,
                    FUNAI_FACTORY_IRRX_THRESHOLD);
 		vIO32WriteFldAlign(PDWNC_IRCKSEL, 8, FLD_IRCLKSEL);
    } 
    else if (u4Type == IRRX_USR_TYPE)
    {
        i4Ret = IRHW_RxInit((INT32)DRVCUST_InitGet(eIrrxUsrConfig),
                            (INT32)DRVCUST_InitGet(eIrrxUsrSaperiod),
                            (INT32)DRVCUST_InitGet(eIrrxUsrThreshold));
        if (DRVCUST_InitQuery(eUsrIRClkSel, &i) == 0)
        {
            vIO32WriteFldAlign(PDWNC_IRCKSEL, (UINT8)i, FLD_IRCLKSEL);
            Printf("Set IRCLKSEL as %d for USR TYPE IR!\n", u4IO32Read4B(PDWNC_IRCKSEL));                            
        }                             
    }

    // Setup callback
    i4Ret = IRHW_RxSetCallback(_IRRX_MtkIrCallback, &pfnOld);
    if (i4Ret != IR_SUCC)
    {
        return IR_FAIL;
    }

    if (!_fgSemaInit)
    {
        i4Ret = x_sema_create(&_hSemaKey, X_SEMA_TYPE_BINARY,
                        X_SEMA_STATE_LOCK);
        if (i4Ret != OSR_OK)
        {
            return IR_FAIL;
        }
        _fgSemaInit = TRUE;
    }

    if (DRVCUST_OptGet(eFlagButtonSupport) && (!_fgBtnInit))
    {
        i4Ret = x_thread_create(&_hBtnThread, IRRX_BTN_THREAD_NAME,
                IRRX_BTN_STACK_SIZE, IRRX_BTN_THREAD_PRIORITY,
                _IRRX_BtnPollThread, 0, NULL);
        if (i4Ret != OSR_OK)
        {
            return IR_FAIL;
        }
        _fgBtnInit = TRUE;
    }

    return IR_SUCC;
}

//-----------------------------------------------------------------------------
/** IRRX_StopMtkIr() to stop the interrupt of IRRX of MTK TV remote controller
 *                  unit.
 *  @retval IR_FAIL run failed.
 *  @retval IR_OK run successfully.
 */
//-----------------------------------------------------------------------------
INT32 IRRX_StopMtkIr(void)
{
    INT32 i4Ret;

    i4Ret = IRHW_RxStop();
    return i4Ret;
}

//-----------------------------------------------------------------------------
/** IRRX_StopMtkIr() to stop the interrupt of IRRX of MTK TV remote controller
 *                  unit.
 *  @retval IR_FAIL run failed.
 *  @retval IR_OK run successfully.
 */
//-----------------------------------------------------------------------------
INT32 IRRX_ResetMtkIr(void)
{
    INT32 i4Ret;

    i4Ret = DRVCUST_OptQuery(eIrrxType, &_gu4IRRXType);
    i4Ret = IRRX_StopMtkIr();
    i4Ret |= IRRX_InitMtkIr(_gu4IRRXType);
    return i4Ret;
}

//-----------------------------------------------------------------------------
/** IRRX_StopSendMtkIr() to insert the IRRX key into buffer.
 *                  unit.
 *  @retval IR_FAIL run failed.
 *  @retval IR_OK run successfully.
 */
//-----------------------------------------------------------------------------
INT32 IRRX_SendMtkIr(UINT32 u4Key)
{
    INT32 i4Ret;

    LOG(9, "%s add Key:0x%08x\n", __FUNCTION__, u4Key);
    _u4CurrKey = u4Key;

    i4Ret = x_sema_unlock(_hSemaKey);
    if (i4Ret != OSR_OK)
    {
        LOG(1, "%s(%d) SemaKey is fault. i4Ret:%d\n", __FILE__, __LINE__, i4Ret);
        return i4Ret;
    }
    return 0;
}

INT32 IRRX_SendDebugMtkIr(UINT32 u4Key,IRRX_RAW_DATA_T* prRaw)
{
	INT32 i4Ret;
	
	LOG(9, "%s add Key:0x%08x\n", __FUNCTION__, u4Key);
	_u4CurrKey = u4Key;
	_rCurrRaw.u4Len = prRaw->u4Len;
	x_memcpy(_rCurrRaw.au1Data, prRaw->au1Data, _rCurrRaw.u4Len);
	i4Ret = x_sema_unlock(_hSemaKey);
	if (i4Ret != OSR_OK)
	{
		LOG(1, "%s(%d) SemaKey is fault. i4Ret:%d\n", __FILE__, __LINE__, i4Ret);
		return i4Ret;
	}
	return 0;
}


//-----------------------------------------------------------------------------
/** IRRX_StopSendMtkIr() to insert the IRRX key into buffer.
 *                  unit.
 *  @retval IR_FAIL run failed.
 *  @retval IR_OK run successfully.
 */
//-----------------------------------------------------------------------------
INT32 IRRX_SendMtkIrRaw(UINT32 u4Key, IRRX_RAW_DATA_T* prRaw)
{
    INT32 i4Ret;

    if(prRaw == NULL)
    {
        return -1;
    }

    LOG(9, "%s add Key:0x%08x\n", __FUNCTION__, u4Key);
    _u4CurrKey = u4Key;
    _rCurrRaw.u4Len = prRaw->u4Len;
    _rCurrRaw.u1Protocol = prRaw->u1Protocol;
    x_memcpy(_rCurrRaw.au1Data, prRaw->au1Data, _rCurrRaw.u4Len);

    i4Ret = x_sema_unlock(_hSemaKey);
    if (i4Ret != OSR_OK)
    {
        LOG(1, "%s(%d) SemaKey is fault. i4Ret:%d\n", __FILE__, __LINE__, i4Ret);
        return i4Ret;
    }
    return 0;
}

#ifdef CHANNEL_CHANGE_LOG
void IRRX_GetPressTime(HAL_TIME_T* pTime)
{
    pTime->u4Micros = dt_press_key.u4Micros;
    pTime->u4Seconds = dt_press_key.u4Seconds;
}

void IRRX_RstPressTime(void)
{
    dt_press_key.u4Micros = 0;
    dt_press_key.u4Seconds = 0;
}
#endif

//-----------------------------------------------------------------------------
/** IRRX_IRRX_PoolMtkIr() to check there is any key available.
 *  @retval IR_FAIL run failed.
 *  @retval IR_OK run successfully.
 */
//-----------------------------------------------------------------------------
INT32 IRRX_PollMtkIr(UINT32 *pu4Key, IRRX_RAW_DATA_T *prRaw)
{
    if(_gu4IRRXType == IRRX_SHARP_TYPE)
    {
        return _PollSharpIr(pu4Key, prRaw);    
    }
    else
    {
        return _PollMtkIr(pu4Key, prRaw);
    }
}


//-----------------------------------------------------------------------------
/** IRRX_WaitMtkIr() to check there is any key available.
 *  @retval IR_FAIL run failed.
 *  @retval IR_OK run successfully.
 */
//-----------------------------------------------------------------------------
INT32 IRRX_WaitMtkIr(UINT32 *pu4Key, UINT32 *pu4Raw)
{
    INT32 i4Ret;
    CRIT_STATE_T cState;

    if ((pu4Key == NULL) || (pu4Raw == NULL))
    {
        return IR_FAIL;
    }
  
    i4Ret = x_sema_lock(_hSemaKey, X_SEMA_OPTION_WAIT);   
    if (i4Ret == OSR_OK)
    {
        cState = x_crit_start();
//        *pu4Raw = _u4CurrRaw;
        *pu4Raw = _rCurrRaw.au1Data[0];

        *pu4Key = _u4CurrKey;
        _u4CurrKey = BTN_NONE;
        x_crit_end(cState);
    }
    else
    {
        ASSERT((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));
        *pu4Key = BTN_NONE;
        return IR_FAIL;
    }

    return IR_SUCC;
}


//-----------------------------------------------------------------------------
/** IRRX_WaitMtkIrTimeout() to check there is any key available.
 *  @retval IR_FAIL run failed.
 *  @retval IR_OK run successfully.
 */
//-----------------------------------------------------------------------------
INT32 IRRX_WaitMtkIrTimeout(UINT32 *pu4Key, IRRX_RAW_DATA_T *prRaw, UINT32 u4Timeout)
{
    INT32 i4Ret;
    CRIT_STATE_T cState;

    if ((pu4Key == NULL) || (prRaw == NULL))
    {
        return IR_FAIL;
    }
  
    i4Ret = x_sema_lock_timeout(_hSemaKey, u4Timeout);   
    if (i4Ret == OSR_OK)
    {
        cState = x_crit_start();
        
        prRaw->u1Protocol = _rCurrRaw.u1Protocol;
        prRaw->u4Len = _rCurrRaw.u4Len;
        x_memcpy(prRaw->au1Data, _rCurrRaw.au1Data, _rCurrRaw.u4Len);
        
        *pu4Key = _u4CurrKey;
        _u4CurrKey = BTN_NONE;
        x_crit_end(cState);
    }
    else if(i4Ret == OSR_TIMEOUT)
    {
        prRaw->u1Protocol = 0;
        prRaw->u4Len = 0;
        x_memset(prRaw->au1Data, 0, IRRX_MAX_RAW_DATA_SIZE);
        *pu4Key = BTN_NONE;
        return OSR_TIMEOUT;        
    }
    else
    {
        ASSERT((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));
        prRaw->u1Protocol = 0;
        prRaw->u4Len = 0;
        x_memset(prRaw->au1Data, 0, IRRX_MAX_RAW_DATA_SIZE);        
        *pu4Key = BTN_NONE;
        return IR_FAIL;
    }

    return IR_SUCC;
}


//-----------------------------------------------------------------------------
/** IRRX_QuerySetRepeat() to set the repeat flag
 *  @param pi4Data pointer to a value to set the repeat flag.
 *  @return the new setting repeat flag
 */
//-----------------------------------------------------------------------------
INT32 IRRX_QuerySetRepeat(const INT32 *pi4Data)
{
    if (pi4Data!=NULL) {
        _fgRepeat = *pi4Data;
    }
    return _fgRepeat;
}

//-----------------------------------------------------------------------------
/** IRRX_QuerySet1stPulse() to set the 1st pulse value of IRRX signal
 *  @param pi4Data pointer to a value to set the 1st pulse value
 *  @return the new setting 1st pulse value
 */
//-----------------------------------------------------------------------------
UINT32 IRRX_QuerySet1stPulse(const UINT32 *pu4Data)
{
    if (pu4Data!=NULL) {
        _u4_1stPulse = *pu4Data;
    }
    return _u4_1stPulse;
}

//-----------------------------------------------------------------------------
/** IRRX_QuerySet2ndPulse() to set the 2nd pulse value of IRRX signal
 *  @param pi4Data pointer to a value to set the 2nd pulse value
 *  @return the new setting 2nd pulse value
 */
//-----------------------------------------------------------------------------
UINT32 IRRX_QuerySet2ndPulse(const UINT32 *pu4Data)
{
    if (pu4Data!=NULL) {
        _u4_2ndPulse = *pu4Data;
    }
    return _u4_2ndPulse;
}

//-----------------------------------------------------------------------------
/** IRRX_QuerySet3rdPulse() to set the 3rd pulse value of IRRX signal
 *  @param pi4Data pointer to a value to set the 3rd pulse value
 *  @return the new setting 3rd pulse value
 */
//-----------------------------------------------------------------------------
UINT32 IRRX_QuerySet3rdPulse(const UINT32 *pu4Data)
{
    if (pu4Data!=NULL) {
        _u4_3rdPulse = *pu4Data;
    }
    return _u4_3rdPulse;
}

//-----------------------------------------------------------------------------
/** IRRX_QuerySetRepeatTime() to set the repeat time slice
 *  @param pi4Data pointer to a value to set the repeat time slice.
 *  @return the new setting repeat time slice.
 */
//-----------------------------------------------------------------------------
UINT32 IRRX_QuerySetRepeatTime(const UINT32 *pu4Data)
{
    if (pu4Data!=NULL) {
        _u4IrRxTimeSlice = *pu4Data;
    }
    return _u4IrRxTimeSlice;
}

//-----------------------------------------------------------------------------
/** IRRX_QuerySetTimeout() to set the IR polling timeout
 *  @param pi4Data pointer to a value to set the timeout value.
 *  @return the new setting repeat time slice.
 */
//-----------------------------------------------------------------------------
UINT32 IRRX_QuerySetTimeout(const UINT32 *pu4Data)
{
    if (pu4Data!=NULL) {
        _u4IrRxTimeout = *pu4Data;
    }
    return _u4IrRxTimeout;
}

//-----------------------------------------------------------------------------
/** IRRX_QuerySetRepeatDuration() to set the valid repeat time duration
 *  @param pi4Data pointer to a value to set the duration.
 *  @return the new setting repeat time slice.
 */
//-----------------------------------------------------------------------------
UINT32 IRRX_QuerySetRepeatDuration(const UINT32 *pu4Data)
{
    if (pu4Data!=NULL) {
        _u4IrRxRepeatDuration = *pu4Data;
    }
    return _u4IrRxRepeatDuration;
}
#endif /* IRRX_NON_OSAI */

UINT32 IRRX_ReadSvadc(UINT32 u4Channel)
{
    return _IRRX_ReadSvadc(u4Channel);
}


//-----------------------------------------------------------------------------
/** IRRX_RawToCrystal() translate raw (addr + cmd) ir code to crystal code.
 *  @param u4Info the IRRX received related attributes of the current data.
 *  @param pu1Data the IRRX received and current data.
 */
//-----------------------------------------------------------------------------
UINT32 IRRX_RawToCrystal(UINT32  u4AddrCode, UINT32 u4CmdCode)
{
    UINT32 u4CrystalKey = BTN_INVALID;

    if (_gu4IRRXType == IRRX_RC5_TYPE)
    {
        if(_pfnRC5KeyLookup != NULL)
        {
            u4CrystalKey = _pfnRC5KeyLookup((UINT8)u4AddrCode, (UINT8)u4CmdCode);    
        }
        else if(_pfnRC6KeyLookup != NULL)
        {
            u4CrystalKey = _pfnRC6KeyLookup((UINT8)u4AddrCode, (UINT8)u4CmdCode);    
        }
    }
    else if (_gu4IRRXType == IRRX_NEC_TYPE)
    {
        u4CrystalKey = IRRX_GetCrystalKey(u4AddrCode, (UINT8)u4CmdCode);
    }   
    else 
    {
        u4CrystalKey = BTN_INVALID;
    }

    if(u4CrystalKey == BTN_NONE)
    {
        u4CrystalKey = BTN_INVALID;
    }    
    return u4CrystalKey;
}

//-----------------------------------------------------------------------------
/** IRRX_RawBitsToCrystal() translate raw ir code bits to crystal code.
 *  @param u4Info the IRRX received related attributes of the current data.
 *  @param pu1Data the IRRX received and current data.
 */
//-----------------------------------------------------------------------------
UINT32 IRRX_RawBitsToCrystal(UINT32  u4Raw)
{
    UINT32 u4CrystalKey = BTN_INVALID;
    UINT32 u4AddrCode;
    UINT32 u4CmdCode;
    if((((u4Raw >> 28) & 0xf) == 0x1) && (_pfnRC5KeyLookup != NULL))
    {
        u4AddrCode = (u4Raw >> 3) & 0x1F;
        u4CmdCode = (u4Raw >> 8) & 0x3F;
        u4CmdCode = (u4Raw & 0x2) ? (u4CmdCode) : (u4CmdCode+64); // For extended RC5 code        
        u4CrystalKey = _pfnRC5KeyLookup((UINT8)u4AddrCode, (UINT8)u4CmdCode);       
    } 
    else if((((u4Raw >> 28) & 0xf) == 0x2) && (_pfnRC6KeyLookup != NULL))
    {
        u4AddrCode = (u4Raw >> 7) & 0xff;
        u4CmdCode = (u4Raw >> 15) & 0xff;
        u4CrystalKey = _pfnRC6KeyLookup((UINT8)u4AddrCode, (UINT8)u4CmdCode);       
    }
    else 
    {
        u4CrystalKey = BTN_INVALID;
    }

    if(u4CrystalKey == BTN_NONE)
    {
        u4CrystalKey = BTN_INVALID;
    }    
    return u4CrystalKey;
}

//-----------------------------------------------------------------------------
/** IRRX_FilterSystemCode() translate raw (addr + cmd) ir code to crystal code.
 *  @param u4Info the IRRX received related attributes of the current data.
 *  @param pu1Data the IRRX received and current data.
 */
//-----------------------------------------------------------------------------

INT32 IRRX_FilterSystemCode(BOOL fgEnable, BOOL fgPositiveFilter, UINT32 u4Num, UINT32 au4FilterArray[])
{
    UINT32 i;
    //Printf("IRRX_FilterSystemCode\n");
    if(!fgEnable)
    {
        _u4NumSystemCodeToFilter = 0;    
    }
    if(u4Num > IRRX_MAX_NUM_ADDR_FILTER)
    {
        return IR_FAIL;
    }
    _fgSystemCodePositiveFilter = fgPositiveFilter;
    _u4NumSystemCodeToFilter = u4Num;
    Printf("IRRX_FilterSystemCode: set postive system code...\n");
    for(i = 0; i < u4Num; i++)
    {
        Printf("(%d)nd system code: %d\n",i, au4FilterArray[i]);
        _au4SystemCodeToFilter[i] = au4FilterArray[i];
#if 0        
        if (au4FilterArray[i] == 0x100 || au4FilterArray[i] == 0x101)
        {
            _IRRX_KeyboardCodeSet(au4FilterArray[i], fgPositiveFilter);
        }
#endif        
    }
	if(fgPositiveFilter)
	{
	    if(0 != IRRX_PassPostiveFilterSystemCodeToT8032(fgEnable, u4Num, au4FilterArray))
	    {
	        Printf("Set system code to T8032 fail...\n");
		_u4NumSystemCodeToFilter = 0;
		return IR_FAIL;
	    }
	}
    return IR_SUCC;
}


INT32 IRRX_SetWakeupKeys(UINT32 u4NumKey, IRRX_RC56_WAKEUP_KEY_T arKeyArray[])
{
    UINT32 i, u4KeyNum, u4Val;
    if(arKeyArray == NULL)
    {
        return IR_FAIL;
    }

    u4KeyNum = 0;
    vIO32Write4B(PDWNC_IREXP_EN, 0);
    vIO32Write4B(PDWNC_EXP_BCNT, 0);
    vIO32Write4B(PDWNC_ENEXP_IRL, 0);

    // RC56 only check 0xffffff00 bits
    vIO32Write4B(PDWNC_ENEXP_IRM, 0x007fff00);
    for (i=0; i<u4NumKey; i++)
    {
        if(arKeyArray[i].i4Src == WAKEUP_KEY_TYPE_RC5)
        {                
            RC5_SPLITTER_T rRc5Splitter;                                
            
            u4Val = u4IO32Read4B(PDWNC_IREXP_EN);
            u4Val |= IREXP_EN(u4KeyNum);
            vIO32Write4B(PDWNC_IREXP_EN, u4Val);
            
            rRc5Splitter.u4Rc5Code = 0; 
            rRc5Splitter.u.start = 3;
            rRc5Splitter.u.toggle = 1;                    
            rRc5Splitter.u.address = (UINT32)arKeyArray[i].i4Sys;
            rRc5Splitter.u.command = (UINT32)arKeyArray[i].i4Cmd;
            rRc5Splitter.u.reserved = 0;
            
            u4Val = IR_PH_ReverseBit16((UINT16)(rRc5Splitter.u4Rc5Code & 0xffff));
            u4Val = IR_PH_BiPhase(u4Val, 14);                        
            u4Val >>= 2;
            
            vIO32Write4B(PDWNC_EXP_IRM0 + (8*u4KeyNum), u4Val);
            vIO32Write4B(PDWNC_EXP_IRL0 + (8*u4KeyNum), 0);
            u4KeyNum++;
        }                
        else if(arKeyArray[i].i4Src == WAKEUP_KEY_TYPE_RC6)
        {                
            RC6_SPLITTER_T rRc6Splitter;                                
            
            u4Val = u4IO32Read4B(PDWNC_IREXP_EN);
            u4Val |= IREXP_EN(u4KeyNum);
            vIO32Write4B(PDWNC_IREXP_EN, u4Val);
            
            rRc6Splitter.u4Rc6Mode0Code = 0;
            rRc6Splitter.u.start = 3;
            rRc6Splitter.u.mb = 0;
            rRc6Splitter.u.toggle = 2;                    
            rRc6Splitter.u.address = (UINT32)arKeyArray[i].i4Sys;
            rRc6Splitter.u.command = (UINT32)arKeyArray[i].i4Cmd;
            rRc6Splitter.u.tail = 0;                                             
            u4Val = IR_PH_ReverseBit32(rRc6Splitter.u4Rc6Mode0Code);
            
            vIO32Write4B(PDWNC_EXP_IRM0 + (8*u4KeyNum), u4Val);
            vIO32Write4B(PDWNC_EXP_IRL0 + (8*u4KeyNum), 0);
            u4KeyNum++;
        }
    }
    return IR_SUCC;
}

//-----------------------------------------------------------------------------
/** IRRX_IsTheKey() to check the ir register content is the key of input.
 *  @param u4Key The key to check.
 *  @retval TRUE for yes, FALSE for no.
 */
//-----------------------------------------------------------------------------
INT32 IRRX_GetActualWakeupKey(UINT32* pu4Type, UINT32* pu4AddrCode, UINT32* pu4CmdCode)
{
    UINT32 u4Info;
    UINT32 au4IrRxData[2];
    IRRX_RAW_DATA_T rRaw;
    UINT32 u4Raw;

    VERIFY(pu4AddrCode != NULL && pu4CmdCode != NULL);
 
    UNUSED(IRRX_InitMtkIrRegSetup(DRVCUST_InitGet(eIrrxType)));
    u4Info = u4IO32Read4B(PDWNC_IRH);
    au4IrRxData[0] = u4IO32Read4B(PDWNC_IRM);
    au4IrRxData[1] = u4IO32Read4B(PDWNC_IRL);
    Printf("PWD:0x%08x RxIsr Info:0x%08x 0x%08x%08x\n",
            u4IO32Read4B(PDWNC_INTSTA),
            u4Info, au4IrRxData[1], au4IrRxData[0]);
    u4Info = _IRRX_XferToCrystal(u4Info, (const UINT8 *)au4IrRxData, &rRaw);

    u4Raw = *(UINT32*)rRaw.au1Data;
    Printf("Raw data is: 0x%8x\n", u4Raw);  
        
    if(((u4Raw >> 28) & 0xf) == 0x1)
    {
        *pu4AddrCode = (u4Raw >> 3) & 0x1F;
        *pu4CmdCode = (u4Raw >> 8) & 0x3F;
        *pu4CmdCode = (u4Raw & 0x2) ? (*pu4CmdCode) : (*pu4CmdCode+64); // For extended RC5 code        
        *pu4Type = WAKEUP_KEY_TYPE_RC5;
        Printf("Wakeup by RC5: %d-%d\n", *pu4AddrCode, *pu4CmdCode);        
    } 
    else if(((u4Raw >> 28) & 0xf) == 0x2)
    {
        *pu4AddrCode = (u4Raw >> 7) & 0xff;
        *pu4CmdCode = (u4Raw >> 15) & 0xff;
        *pu4Type = WAKEUP_KEY_TYPE_RC6;        
        Printf("Wakeup by RC6: %d-%d\n", *pu4AddrCode, *pu4CmdCode);                
    }
    
    return 0;
}

#ifndef IRRX_NON_OSAI
void IRRX_BtnSendKey(UINT32 u4Key)
{
    return _IRRX_BtnSendKey(u4Key);
}
#endif


