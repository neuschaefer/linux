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

//-----------------------------------------------------------------------------
//                    Include files
//-----------------------------------------------------------------------------
#include "ir_debug.h"
#include "ir_if.h"
#include "u_irrc_btn_def.h"

#include "x_printf.h"
#include "x_debug.h"
#include "x_pdwnc.h"
#include "x_timer.h"
#include "eeprom_if.h"
#include "drvcust_if.h"
#include "drv_common.h"
#include "x_irrc.h"
//#include "x_aud_dec.h"
#include "aud_if.h"
//-----------------------------------------------------------------------------
//                    Configuration
//-----------------------------------------------------------------------------
#define ENABLE_VGA_WORK_AROUND      (0)

//-----------------------------------------------------------------------------
//                    Macros, defines, typedefs, enums
//-----------------------------------------------------------------------------
// #define IRRC_STACK_SIZE            1024
// #define IRRC_THREAD_PRIORITY    100
#define IRRC_CALLBACK_NO            5
#define BTN_KEYPAD_POWER            (BTN_POWER | KEY_SRC_FRONT_PANEL)

//-----------------------------------------------------------------------------
//                    Variable definitions
//-----------------------------------------------------------------------------
static HANDLE_T             hIRRCThread;
static volatile UINT32      _fgIRRCDisconnectMiddleware = 0;
static volatile UINT32      _fgIRRCWaitPowerKeyReboot = 0;
static PFN_IRRXCB_THREAD_T  pfnIrrxThreadCallback = NULL;
static PFN_IRRXCB_THREAD_T  pfnIrrxCallbackTbl[IRRC_CALLBACK_NO] = {NULL, NULL, NULL, NULL, NULL};
static UINT32               _fgIRRCPowerButtonOff = 0;
static PFN_IS_USR_LONG_PRESS _pfnIsUsrLongPress = NULL;
#ifdef IRRC_LOG
static IRRC_LOGGER_T _rLogger;
#endif	

static BOOL                 _bIRRCLedOn = TRUE;
static BOOL                 _bIRRCBlinkLedEnable = TRUE;
static UINT32               _u4IRRCBlinkLedGpio = 0;
static UINT32               _u4IRRCBlinkLedOnVal = 0;
static UINT32               _u4IRRCBlinkDuration = 0;
static HANDLE_T             _hIRRCLedCtrlTimer ;
//static HANDLE_T             _hIRRCLedCtrlTimer = NULL;


#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389) 
UINT32 u4IR_AudInitFlag = 0;
#endif
#if defined(LINUX_TURNKEY_SOLUTION)||defined(__KERNEL__)
static BOOL _fgDfbRcThreadInit = FALSE;
static BOOL _fgRcSemaInit = FALSE;

//static HANDLE_T _hRcSemaKey = 0;
//static IRRX_RAW_DATA_T _rCurrRaw;  
UINT32 _CurrBtnState = BTN_NONE;
UINT32 _CurrBtn;

typedef struct _strctIRMsg
{
    IRRX_RAW_DATA_T rRaw;
    UINT32 u4BtnState;
    UINT32 u4Btn;
} DFB_IRRC_NOTIFY_MSG;

static HANDLE_T _hDfbMsgQue ;
//static HANDLE_T _hDfbMsgQue = NULL;

//static BOOL _fgRcMsgQInit = FALSE;

#if 0
static void *pv_irrc_nfy_tag;
static x_irrc_nfy_fct pf_irrc_usr_nfy_fct = NULL;
#endif
#endif

//-----------------------------------------------------------------------------
//                    Extern Functions
//-----------------------------------------------------------------------------
#if ENABLE_VGA_WORK_AROUND
EXTERN VOID TvdConnectMainToVGA( VOID );
EXTERN BOOL TvdGetMainSigStatus( VOID );
#else
#define TvdConnectMainToVGA()
#define TvdGetMainSigStatus()       (0)
#endif /* ENABLE_VGA_WORK_AROUND */

//extern void AUD_DspMixSndControl(UINT16 u2Mode);
//-----------------------------------------------------------------------------
//                    Intern Functions
//-----------------------------------------------------------------------------
static void IRRC_CallbackHandle(UINT32 u4BtnState, UINT32 u4Key, 
                                IRRX_RAW_DATA_T *prRawData);
UINT32 IRRC_IsPowerButtonOff(void);
#ifdef IRRC_LOG
VOID IRRC_ResetLog(VOID);


//-----------------------------------------------------------------------------
//                    private function.
//-----------------------------------------------------------------------------
static void _IRRC_AddLog(UINT32 u4Raw)
{
    _rLogger.arLog[_rLogger.u4CurrIdx].u4Raw = u4Raw;
    HAL_GetTime(&(_rLogger.arLog[_rLogger.u4CurrIdx].rTime));
    _rLogger.u4CurrIdx = (_rLogger.u4CurrIdx + 1) % IRRC_LOG_SIZE;
}
#endif


static BOOL _IRRC_TvdGetMainSigStatus(void)
{
    return TvdGetMainSigStatus();
}


#define COMBO_KEY_SUPPORT 1
#ifdef COMBO_KEY_SUPPORT
static UINT32 _CombKeyLookup(UINT32 u4Key1, UINT32 u4Key2)
{
    UINT32 u4IrKey, u4FpKey, i;
    COMB_IR_KP_T *prCombKeyList = NULL;

    if(DRVCUST_OptQuery(eCombKeyList, (UINT32 *)(void *)&prCombKeyList) == 0)
    {
        if((u4Key1 & KEY_SRC_FRONT_PANEL) == (u4Key2 & KEY_SRC_FRONT_PANEL))
        {//the two keys are both KEY_SRC_FRONT_PANEL or both KEY_SRC_REMOTE_CTRL, it must not be CombKey
            return BTN_NONE;
        }
        
        if(u4Key1 & KEY_SRC_FRONT_PANEL) // u4Key1 is from Kp (u4Key2 is from IR)
        {
            u4FpKey = u4Key1;
            u4IrKey = u4Key2;
        }
        else// u4Key1 is from Ir (u4Key2 is from Kp)
        {
            u4FpKey = u4Key2;
            u4IrKey = u4Key1;
        }
    //start to compare
        i = 0;
        while ((prCombKeyList) &&
                (prCombKeyList[i].u4CrystalKey != 0) &&
                (prCombKeyList[i].u4CrystalKey != BTN_NONE))
        {
            if ((prCombKeyList[i].u4IrBtn == u4IrKey) &&
                (prCombKeyList[i].u4KeyPadBtn == u4FpKey))
            {
                return prCombKeyList[i].u4CrystalKey;
            }
            i++;
        }
        return BTN_NONE;
    }
    else
    {
        return BTN_NONE;
    }
}
#endif

#if !((!defined(LINUX_TURNKEY_SOLUTION)) && defined(__KERNEL__))//linux drv_build should not use it  
//static UINT32 _IRRC_IsNecLongPress(UINT32 rDelta,IRRX_RAW_DATA_T *rRaw, IRRX_RAW_DATA_T *rPrevRaw);
static UINT32 _IRRC_IsNecLongPress(UINT32 rDelta,IRRX_RAW_DATA_T *rRaw, IRRX_RAW_DATA_T *rPrevRaw)
{
/*    
case1: prev_raw is not repeat pulse, now_raw is repeat pulse ==> long press
case2: prev_raw is repeat pulse, now_raw is not repeat pulse ==> 2 shot
case3: prev_raw is repeat pulse, now_raw is repeat pulse also ==> long press
case4: prev_raw is not repeat pulse, now_raw is not repeat pulse==> 2 shot

Due to the design of IRRC flow, we don't need to check the whetehr format of IR frame is valid.
In other words, the BTN_NONE(Noise case) will not call the function
*/
    UNUSED(rDelta);
    if(!((rPrevRaw->au1Data[0] == 0) && (rPrevRaw->au1Data[1] == 0)) && 
       ((rRaw->au1Data[0] == 0) && (rRaw->au1Data[1] == 0))
       )
    {
        LOG(7, "[RC](%d) Repeat!\n", __LINE__);        
        return 1;
    }
    else if(((rPrevRaw->au1Data[0] == 0) && (rPrevRaw->au1Data[1] == 0)) && 
       !((rRaw->au1Data[0] == 0) && (rRaw->au1Data[1] == 0))
       )
    {
        LOG(7, "[RC](%d) Not Repeat!\n",__LINE__);
        return 0;
    }
    else if(((rPrevRaw->au1Data[0] == 0) && (rPrevRaw->au1Data[1] == 0)) && 
       ((rRaw->au1Data[0] == 0) && (rRaw->au1Data[1] == 0))
       )
    {
        LOG(7, "[RC](%d) Repeat!\n", __LINE__);                
        return 1;
    }
    else if(!((rPrevRaw->au1Data[0] == 0) && (rPrevRaw->au1Data[1] == 0)) && 
       !((rRaw->au1Data[0] == 0) && (rRaw->au1Data[1] == 0))
       )
    {
        LOG(7, "[RC](%d) Not Repeat!\n",__LINE__);
        return 0;
    }
    else
    {
        LOG(7, "[RC]Warning !!!(%d) Not Repeat!\n", __LINE__);
        return 0;
    }
        
}
//static UINT32 _IRRC_IsRC5LongPress(UINT32 rDelta,IRRX_RAW_DATA_T *rRaw, IRRX_RAW_DATA_T *rPrevRaw);
static UINT32 _IRRC_IsRC5LongPress(UINT32 rDelta,IRRX_RAW_DATA_T *rRaw, IRRX_RAW_DATA_T *rPrevRaw)
{
    //just check toggle bit
    UNUSED(rDelta);
    if(rPrevRaw->au1Data[0] != rRaw->au1Data[0])
    {
        LOG(7, "[RC](%d) Not Repeat!(PreRawByte0 0x%2x,RawByte0 0x%2x)\n", __LINE__, rPrevRaw->au1Data[0], rRaw->au1Data[0]);
        return 0;        
    }
    else
    {
        LOG(7, "[RC](%d) Repeat!(PreRawByte0 0x%2x,RawByte0 0x%2x)\n", __LINE__, rPrevRaw->au1Data[0], rRaw->au1Data[0]);
        return 1;        
    }    
}
//static UINT32 _IRRC_IsRC6LongPress(UINT32 rDelta,IRRX_RAW_DATA_T *rRaw, IRRX_RAW_DATA_T *rPrevRaw);
static UINT32 _IRRC_IsRC6LongPress(UINT32 rDelta,IRRX_RAW_DATA_T *rRaw, IRRX_RAW_DATA_T *rPrevRaw)
{//just check toggle bit
    UNUSED(rDelta);
    
    if(rPrevRaw->au1Data[0] != rRaw->au1Data[0])
    {
        LOG(7, "[RC](%d) Not Repeat!(PreRawByte0 0x%2x,RawByte0 0x%2x)\n", __LINE__, rPrevRaw->au1Data[0], rRaw->au1Data[0]);
        return 0;        
    }
    else
    {
        LOG(7, "[RC](%d) Repeat!(PreRawByte0 0x%2x,RawByte0 0x%2x)\n", __LINE__, rPrevRaw->au1Data[0], rRaw->au1Data[0]);
        return 1;        
    }    
}
//static UINT32 _IRRC_IsLongPress(UINT32 rDelta,IRRX_RAW_DATA_T *rRaw, IRRX_RAW_DATA_T *rPrevRaw);
static UINT32 _IRRC_IsLongPress(UINT32 rDelta,IRRX_RAW_DATA_T *rRaw, IRRX_RAW_DATA_T *rPrevRaw)
{
    if(DRVCUST_InitGet(eIrrcCheckRepeatByRaw) == 0)
    {    
        if ((rDelta > (1000 * IRRX_QuerySetRepeatDuration(NULL)))) 
        {
            LOG(7, "[RC]%d) Not Repeat! (rDelta: %d)", __LINE__, rDelta);
            return 0;//the period of coming of the two continusing frame is larger than threshold ==> not long press
        }
        else
        {
            LOG(7, "[RC]%d) Repeat! (rDelta: %d)", __LINE__, rDelta);
            return 1;
        }
        
    }
    else
    {
        if(rRaw->u1Protocol == IRRX_RAW_DATA_NEC)
        {
            return _IRRC_IsNecLongPress(rDelta, rRaw, rPrevRaw);        
        }
        
        else if(rRaw->u1Protocol == IRRX_RAW_DATA_RC5)
        {
            return _IRRC_IsRC5LongPress(rDelta, rRaw, rPrevRaw);
        }
        else if(rRaw->u1Protocol == IRRX_RAW_DATA_RC6)
        {
            return _IRRC_IsRC6LongPress(rDelta, rRaw, rPrevRaw);        
        }
    /*    
        else if(rRaw->u1Protocol == IRRX_RAW_DATA_SHARP)
        {
            return _IRRC_IsRC6LongPress(rDelta, rRaw, rPrevRaw);        
        }
    */    
        else //for Sony, Matsushita, Funai Factory IR, and the IR protocl without repeat signal
        {
            if(_pfnIsUsrLongPress != NULL)
            {
                return _pfnIsUsrLongPress(rDelta, rRaw, rPrevRaw);
            }
            else
            {            
                if ((rDelta > (1000 * IRRX_QuerySetRepeatDuration(NULL)))) 
                {
                    LOG(7, "[IRRC]%d) Not Repeat! ", __LINE__);                  
                    return 0;//the period of coming of the two continusing frame is larger than threshold ==> not long press
                }
                else
                {
                    LOG(7, "[IRRC]%d) Repeat! ", __LINE__);                                  
                    return 1;
                }
            }
        }
    }
}
//-----------------------------------------------------------------------------
//                    IRRC data processing thread
//-----------------------------------------------------------------------------
static void IRRC_DataProcessingThread(void* pvArg)
{
    HAL_TIME_T rPrevTime = { 0, 0 };
    HAL_TIME_T rPrevPowerTime = { 0, 0 };
    HAL_TIME_T rTime, rDelta;
    INT32 i4Ret;
    UINT32 u4Key, u4PrevKey, u4PrevPowerKey = BTN_NONE;
    IRRX_RAW_DATA_T u4PrevPowerRaw;
    IRRX_RAW_DATA_T rRaw, rPrevRaw;
#ifdef COMBO_KEY_SUPPORT
    static UINT32 _IsCombKeyDn = 0;
    UINT32 u4CombKey = BTN_NONE;
#endif
    UNUSED(pvArg);
    u4PrevKey = 20; // for key up/down system, this time as small as possible.
    LINT_SUPPRESS_NEXT_EXPRESSION(534);
    IRRX_QuerySetRepeatTime(&u4PrevKey);
    i4Ret = 1;
    LINT_SUPPRESS_NEXT_EXPRESSION(534);
    IRRX_QuerySetRepeat(&i4Ret);
    u4PrevKey = BTN_NONE;
    rPrevRaw.u4Len = 1;
    //x_memset(rPrevRaw.au1Data, 0, IRRX_MAX_RAW_DATA_SIZE);      
    x_memset(&rPrevRaw, 0, sizeof(IRRX_RAW_DATA_T));      
#ifdef IRRC_LOG
    IRRC_ResetLog();
#endif

    while(1)
    {
        i4Ret = IRRX_PollMtkIr(&u4Key, &rRaw);

        if (_fgIRRCDisconnectMiddleware)
        {
            if (_fgIRRCWaitPowerKeyReboot)
            {
                if (((u4Key & 0x0fffffff) == BTN_POWER) ||
                    (_IRRC_TvdGetMainSigStatus()))
                {
                    PDWNC_Reboot();
                }
            }
            x_thread_delay(50);
            continue;
        }

        /* if no call back function, no need to check IR return Key. */
        if (pfnIrrxThreadCallback==NULL) { continue; }
        if (i4Ret == IR_SUCC) {
            if (u4Key == BTN_KEYPAD_POWER) {
                _fgIRRCPowerButtonOff = (u4Key == (BTN_POWER | KEY_SRC_FRONT_PANEL));
                if (u4PrevPowerKey != BTN_KEYPAD_POWER)
                {
                     LOG(7, "%d) Key:0x%08x notify DOWN (PreKey:0x%08x)\n", __LINE__, u4Key, u4PrevKey);
                     LOG(7, "%d) Raw:0x%8x, PreRaw: 0x%8x\n", __LINE__, *(UINT32*)(rRaw.au1Data), *(UINT32*)(rPrevRaw.au1Data));
                    pfnIrrxThreadCallback(IRRX_COND_BTN_DOWN, u4Key, &rRaw);
                }
                u4PrevPowerKey = BTN_KEYPAD_POWER;
                //u4PrevPowerRaw = u4Raw;
                u4PrevPowerRaw.u1Protocol= rRaw.u1Protocol;                    
                u4PrevPowerRaw.u4Len = rRaw.u4Len;
                x_memcpy(u4PrevPowerRaw.au1Data, rRaw.au1Data, rRaw.u4Len);      
                HAL_GetTime(&rPrevPowerTime);
                u4Key = BTN_NONE;
            } else {
                if (u4PrevPowerKey == BTN_KEYPAD_POWER)
                {
                    HAL_GetTime(&rTime);
                    HAL_GetDeltaTime(&rDelta, &rPrevPowerTime, &rTime);
                    if ((rDelta.u4Seconds > 0) ||
                        (rDelta.u4Micros > (1000*IRRX_QuerySetRepeatDuration(NULL)))) {
                        LOG(7, "%d) PreKey:0x%08x notify UP (Key:0x%08x)\n", __LINE__, u4PrevPowerKey, u4Key);
                        LOG(7, "%d) Raw:0x%8x, PreRaw: 0x%8x\n", __LINE__, *(UINT32*)(rRaw.au1Data), *(UINT32*)(rPrevRaw.au1Data));
                        pfnIrrxThreadCallback(IRRX_COND_BTN_UP, u4PrevPowerKey, &u4PrevPowerRaw);
                        u4PrevPowerKey = BTN_NONE;
                        //u4PrevPowerRaw = 0;
                        x_memset(u4PrevPowerRaw.au1Data, 0, IRRX_MAX_RAW_DATA_SIZE);   
                    }
                }
            }
            if (u4Key == BTN_NONE) { /* polling timeout. */
                if (u4PrevKey != BTN_NONE) { /* Here is a key up. */
                    LOG(7,  "%d) PreKey:0x%08x notify UP (Key:0x%08x)\n", __LINE__, u4PrevKey, u4Key);                    
                    LOG(7, "%d) Raw:0x%8x, PreRaw: 0x%8x\n", __LINE__, *(UINT32*)(rRaw.au1Data), *(UINT32*)(rPrevRaw.au1Data));
                    pfnIrrxThreadCallback(IRRX_COND_BTN_UP, u4PrevKey, &rPrevRaw);
#ifdef IRRC_LOG                    
                    _IRRC_AddLog(rPrevRaw.au1Data[0]);
#endif                    
                    u4PrevKey = BTN_NONE;
                    rPrevRaw.u4Len = 1;
                    x_memset(rPrevRaw.au1Data, 0, IRRX_MAX_RAW_DATA_SIZE);      
#ifdef COMBO_KEY_SUPPORT 
                        if (_IsCombKeyDn == 1) 
                        {
                            LOG(9, "ComboKey notify Up\n");
                        }
                        _IsCombKeyDn = 0;
#endif
                    }
            }
#ifdef COMBO_KEY_SUPPORT            
            else if (_IsCombKeyDn == 1) 
            {
                /* If both of the two keys are not be released, do nothing. */
                LOG(9, "Discard keys...\n");
            }
#endif            
            else if (u4Key == BTN_DRVKEY_REPEAT) {
                /* If repeat, do nothing. */
            } else if ((u4Key & KEY_SRC_DRIVER) == KEY_SRC_DRIVER) {
                static UINT32 u4PrevDriverKey = BTN_NONE;
                UINT32 u4Channel, u4Num;

                if ((BTN_DRIVER1_FACTORY(0) < u4Key) && (u4Key < BTN_DRIVER1_FACTORY(128)))
                {
                    u4PrevDriverKey = u4Key;
                }
                else if ((BTN_DRIVER2_FACTORY(0) < u4Key) && (u4Key < BTN_DRIVER2_FACTORY(128)) &&
                        ((u4Key & 0xffff) == (u4PrevDriverKey & 0xffff)))
                {
                    u4Channel = (u4Key & 0xffff);
                    switch(u4Channel) {
                    case 0:
                        LOG(7,  "%d) Key:0x%08x notify DOWN\n", __LINE__, BTN_DVD);
                        LOG(7, "%d) Raw:0x%8x, PreRaw: 0x%8x\n", __LINE__, *(UINT32*)(rRaw.au1Data), *(UINT32*)(rPrevRaw.au1Data));                        
                        pfnIrrxThreadCallback(IRRX_COND_BTN_DOWN, BTN_DVD, &rRaw);
                        LOG(7,  "%d) Key:0x%08x notify UP\n", __LINE__, BTN_DVD);
                        LOG(7, "%d) Raw:0x%8x, PreRaw: 0x%8x\n", __LINE__, *(UINT32*)(rRaw.au1Data), *(UINT32*)(rPrevRaw.au1Data));
                        pfnIrrxThreadCallback(IRRX_COND_BTN_UP, BTN_DVD, &rRaw);
                        break;
                    default:
                        if ((u4Channel / 100) != 0) {
                            LOG(7,  "%d) Key:0x%08x notify DOWN\n", __LINE__, BTN_DIGIT_1);
                            LOG(7, "%d) Raw:0x%8x, PreRaw: 0x%8x\n", __LINE__, *(UINT32*)(rRaw.au1Data), *(UINT32*)(rPrevRaw.au1Data));
                            pfnIrrxThreadCallback(IRRX_COND_BTN_DOWN, BTN_DIGIT_1, &rRaw);
                            LOG(7,  "%d) Key:0x%08x notify UP\n", __LINE__, BTN_DIGIT_1);                        
                            LOG(7, "%d) Raw:0x%8x, PreRaw: 0x%8x\n", __LINE__, *(UINT32*)(rRaw.au1Data), *(UINT32*)(rPrevRaw.au1Data));
                            pfnIrrxThreadCallback(IRRX_COND_BTN_UP, BTN_DIGIT_1, &rRaw);
                        }
                        u4Num = ((u4Channel / 10) % 10);
                        if (u4Num != 0) {
                            LOG(7,  "%d) Key:0x%08x notify DOWN\n", __LINE__, BTN_DIGIT_0 + u4Num);
                            LOG(7, "%d) Raw:0x%8x, PreRaw: 0x%8x\n", __LINE__, *(UINT32*)(rRaw.au1Data), *(UINT32*)(rPrevRaw.au1Data));
                            pfnIrrxThreadCallback(IRRX_COND_BTN_DOWN, (BTN_DIGIT_0 + u4Num), &rRaw);
                            LOG(7,  "%d) Key:0x%08x notify UP\n", __LINE__, BTN_DIGIT_0 + u4Num);
                            LOG(7, "%d) Raw:0x%8x, PreRaw: 0x%8x\n", __LINE__, *(UINT32*)(rRaw.au1Data), *(UINT32*)(rPrevRaw.au1Data));
                            pfnIrrxThreadCallback(IRRX_COND_BTN_UP, (BTN_DIGIT_0 + u4Num), &rRaw);
                        }
                        u4Num = (u4Channel % 10);
                        LOG(7,  "%d) Key:0x%08x notify DOWN\n", __LINE__, BTN_DIGIT_0 + u4Num);
                        LOG(7, "%d) Raw:0x%8x, PreRaw: 0x%8x\n", __LINE__, *(UINT32*)(rRaw.au1Data), *(UINT32*)(rPrevRaw.au1Data));
                        pfnIrrxThreadCallback(IRRX_COND_BTN_DOWN, (BTN_DIGIT_0 + u4Num), &rRaw);
                        LOG(7,  "%d) Key:0x%08x notify UP\n", __LINE__, BTN_DIGIT_0 + u4Num);
                        LOG(7, "%d) Raw:0x%8x, PreRaw: 0x%8x\n", __LINE__, *(UINT32*)(rRaw.au1Data), *(UINT32*)(rPrevRaw.au1Data));
                        pfnIrrxThreadCallback(IRRX_COND_BTN_UP, (BTN_DIGIT_0 + u4Num), &rRaw);
                        break;
                    }
                    u4PrevKey = BTN_NONE;
                    rPrevRaw.u4Len = 1;
                    x_memset(rPrevRaw.au1Data, 0, IRRX_MAX_RAW_DATA_SIZE);                    
                }
                else if (u4PrevKey == u4Key) { /* the same key with previous key. */
                    HAL_TIME_T rTime, rDelta;
                    HAL_GetTime(&rTime);
                    HAL_GetDeltaTime(&rDelta, &rPrevTime, &rTime);
                    u4Key = BTN_INVALID;
                    if ((rDelta.u4Seconds > 0) ||
                        (rDelta.u4Micros > (1000*IRRX_QuerySetRepeatDuration(NULL)))) {
                         LOG(7,  "%d) Key:0x%08x notify UP (PreKey:0x%08x)\n", __LINE__, u4Key,u4PrevKey);
                         LOG(7, "%d) Raw:0x%8x, PreRaw: 0x%8x\n", __LINE__, *(UINT32*)(rRaw.au1Data), *(UINT32*)(rPrevRaw.au1Data));
                        pfnIrrxThreadCallback(IRRX_COND_BTN_UP, u4Key, &rRaw);
#ifdef IRRC_LOG                        
                        _IRRC_AddLog(rPrevRaw.au1Data[0]);                        
#endif                  
                        LOG(7,  "%d) Key:0x%08x notify DOWN(PreKey:0x%08x)\n", __LINE__, u4Key,u4PrevKey);    
                        LOG(7, "%d) Raw:0x%8x, PreRaw: 0x%8x\n", __LINE__, *(UINT32*)(rRaw.au1Data), *(UINT32*)(rPrevRaw.au1Data));
                        pfnIrrxThreadCallback(IRRX_COND_BTN_DOWN, u4Key, &rRaw);
                    } // otherwise, do nothing.

                    HAL_GetTime(&rPrevTime);
                } else { /* there is some key useful/valid. */

                    u4Key = BTN_INVALID;
/*
                    if ((u4Key & 0x0fffffffU) == BTN_POWER)
                    {
                        _fgIRRCPowerButtonOff = (u4Key == (BTN_POWER | KEY_SRC_FRONT_PANEL));
                    }
*/
                    if (u4PrevKey != BTN_NONE)
                    {
                        LOG(7,  "%d) PreKey:0x%08x notify UP(Key:0x%08x)\n", __LINE__, u4PrevKey, u4Key);
                        LOG(7, "%d) Raw:0x%8x, PreRaw: 0x%8x\n", __LINE__, *(UINT32*)(rRaw.au1Data), *(UINT32*)(rPrevRaw.au1Data));
                        pfnIrrxThreadCallback(IRRX_COND_BTN_UP, u4PrevKey, &rPrevRaw);
#ifdef IRRC_LOG                        
                        _IRRC_AddLog(rPrevRaw.au1Data[0]);                        
#endif
                    }
                    LOG(7,  "%d) Key:0x%08x notify DOWN (PreKey:0x%08x)\n", __LINE__, u4Key, u4PrevKey);
                    LOG(7, "%d) Raw:0x%8x, PreRaw: 0x%8x\n", __LINE__, *(UINT32*)(rRaw.au1Data), *(UINT32*)(rPrevRaw.au1Data));
                    pfnIrrxThreadCallback(IRRX_COND_BTN_DOWN, u4Key, &rRaw);
                    u4PrevKey = u4Key;
                    rPrevRaw.u1Protocol= rRaw.u1Protocol;                    
                    rPrevRaw.u4Len = rRaw.u4Len;
                    x_memcpy(rPrevRaw.au1Data, rRaw.au1Data, rRaw.u4Len);      
                    HAL_GetTime(&rPrevTime);
                }
            } else if (u4PrevKey == u4Key) { /* the same key with previous key. */
                HAL_TIME_T rTime, rDelta;
                HAL_GetTime(&rTime);
                HAL_GetDeltaTime(&rDelta, &rPrevTime, &rTime);
                LOG(9,  "%d) key:0x%8x Delta (%d)sec, (%d)us\n", __LINE__, u4Key, rDelta.u4Seconds,rDelta.u4Micros);
#ifdef CC_JOYSTICK_IR//SUPPORT_NEC_IR_MOUSE
                if(u4Key == BTN_IR_MOUSE_MOVE) 
                    {
                        pfnIrrxThreadCallback(IRRX_COND_BTN_UP, u4Key, &rRaw);
                        LOG(7,  "%d) Key:0x%08x notify UP(PreKey:0x%08x)\n", __LINE__, u4Key, u4PrevKey);
                        LOG(7, "%d) Raw:0x%8x, PreRaw: 0x%8x\n", __LINE__, *(UINT32*)(rRaw.au1Data), *(UINT32*)(rPrevRaw.au1Data));
                        pfnIrrxThreadCallback(IRRX_COND_BTN_DOWN, u4Key, &rRaw);  
                        LOG(7,  "%d) Key:0x%08x notify DOWN (PreKey:0x%08x)\n", __LINE__, u4Key, u4PrevKey);
                        LOG(7, "%d) Raw:0x%8x, PreRaw: 0x%8x\n", __LINE__, *(UINT32*)(rRaw.au1Data), *(UINT32*)(rPrevRaw.au1Data));
//                        LOG(9, "Joystic Raw Data (fisrt 3 bytes): 0x%02x%02x%02x\n",rRaw.au1Data[2], rRaw.au1Data[1], rRaw.au1Data[0]);
                        
                        u4PrevKey = u4Key;
                        rPrevRaw.u1Protocol= rRaw.u1Protocol;                    
                        rPrevRaw.u4Len = rRaw.u4Len;
                        x_memcpy(rPrevRaw.au1Data, rRaw.au1Data, rRaw.u4Len);                  
                    }
                else
#endif                 
                if ((rDelta.u4Seconds > 0) || (!_IRRC_IsLongPress(rDelta.u4Micros, &rRaw, &rPrevRaw))){
                    LOG(7,  "%d) Key:0x%08x notify UP (PreKey:0x%08x)\n", __LINE__, u4Key, u4PrevKey);
                    LOG(7, "%d) Raw:0x%8x, PreRaw: 0x%8x\n", __LINE__, *(UINT32*)(rRaw.au1Data), *(UINT32*)(rPrevRaw.au1Data));
                    pfnIrrxThreadCallback(IRRX_COND_BTN_UP, u4Key, &rRaw);
#ifdef IRRC_LOG                    
                    _IRRC_AddLog(rRaw.au1Data[0]);
#endif                    
                    LOG(7,  "%d) Key:0x%08x notify DOWN (PreKey:0x%08x)\n", __LINE__, u4Key, u4PrevKey);
                    LOG(7, "%d) Raw:0x%8x, PreRaw: 0x%8x\n", __LINE__, *(UINT32*)(rRaw.au1Data), *(UINT32*)(rPrevRaw.au1Data));
                    pfnIrrxThreadCallback(IRRX_COND_BTN_DOWN, u4Key, &rRaw);
                    u4PrevKey = u4Key;
                    rPrevRaw.u1Protocol= rRaw.u1Protocol;                    
                    rPrevRaw.u4Len = rRaw.u4Len;
                    x_memcpy(rPrevRaw.au1Data, rRaw.au1Data, rRaw.u4Len);  
                } // otherwise, do nothing.
                HAL_GetTime(&rPrevTime);
            } else { /* there is some key useful/valid. */
                if ((u4Key & 0x0fffffff) == BTN_POWER)
                {
                    _fgIRRCPowerButtonOff = (u4Key == (BTN_POWER | KEY_SRC_FRONT_PANEL));
                }
                if (u4PrevKey != BTN_NONE)
                {
                    LOG(7,  "%d) PreKey:0x%08x notify UP (Key:0x%08x)\n", __LINE__, u4PrevKey, u4Key);
                    LOG(7, "%d) Raw:0x%8x, PreRaw: 0x%8x\n", __LINE__, *(UINT32*)(rRaw.au1Data), *(UINT32*)(rPrevRaw.au1Data));
                    pfnIrrxThreadCallback(IRRX_COND_BTN_UP, u4PrevKey, &rPrevRaw);
#ifdef IRRC_LOG                    
                    _IRRC_AddLog(rPrevRaw.au1Data[0]);
#endif
                
#ifdef COMBO_KEY_SUPPORT 
                    if((u4CombKey = _CombKeyLookup(u4PrevKey, u4Key)) != BTN_NONE)
                    {
                        u4Key = u4CombKey;
                        LOG(7,  "%d)ComboKey notify DOWN(Key:0x%08x)\n", __LINE__, u4Key);
                        LOG(7, "%d) Raw:0x%8x, PreRaw: 0x%8x\n", __LINE__, *(UINT32*)(rRaw.au1Data), *(UINT32*)(rPrevRaw.au1Data));
                        pfnIrrxThreadCallback(IRRX_COND_BTN_DOWN, u4Key, &rRaw);
                        u4PrevKey = u4Key;
                        rPrevRaw.u1Protocol= rRaw.u1Protocol;                    
                        rPrevRaw.u4Len = rRaw.u4Len;
                        x_memcpy(rPrevRaw.au1Data, rRaw.au1Data, rRaw.u4Len);  
                        HAL_GetTime(&rPrevTime);
                        _IsCombKeyDn = 1;
                    }
#endif
                }
#ifdef COMBO_KEY_SUPPORT 
                if(_IsCombKeyDn != 1)
#else
                if(1)
#endif
                {
                   if(u4Key != BTN_NONE) 
                   {
                        LOG(7,  "%d) Key:0x%08x notify DOWN (PreKey:0x%08x)\n", __LINE__, u4Key, u4PrevKey);
                        LOG(7, "%d) Raw:0x%8x, PreRaw: 0x%8x\n", __LINE__, *(UINT32*)(rRaw.au1Data), *(UINT32*)(rPrevRaw.au1Data));
                        pfnIrrxThreadCallback(IRRX_COND_BTN_DOWN, u4Key, &rRaw);
                        u4PrevKey = u4Key;
                        rPrevRaw.u1Protocol= rRaw.u1Protocol;                    
                        rPrevRaw.u4Len = rRaw.u4Len;
                        x_memcpy(rPrevRaw.au1Data, rRaw.au1Data, rRaw.u4Len);  
                        HAL_GetTime(&rPrevTime);
                   }
                }
            }
        }
        x_thread_delay(10);
    }
}
#endif
static void IRRC_CallbackHandle(UINT32 u4BtnState, UINT32 u4Key, 
                                IRRX_RAW_DATA_T *prRawData)
{
    UINT32 u4Val;
#if 0    
    if(u4Key == BTN_NONE)
    {
        LOG(9,"[IR]IRRC_CallbackHandle: discard BTN_NONE...\n");
        return;
    }
#endif    
    for (u4Val = 0; u4Val < IRRC_CALLBACK_NO; u4Val++)
    {
        if (pfnIrrxCallbackTbl[u4Val] != NULL)
        {
            pfnIrrxCallbackTbl[u4Val](u4BtnState, u4Key, prRawData);
        }
    }
}

static void IRRC_IRLEDCtrlFunc(HANDLE_T hHandle, VOID* pTag)
{
    INT32 i4Val;
    
    UNUSED(hHandle);

    if (!_bIRRCBlinkLedEnable)
    {
        if ((HANDLE_T *)_hIRRCLedCtrlTimer != NULL)
        {
            x_timer_stop(_hIRRCLedCtrlTimer);
        }
        _bIRRCLedOn = FALSE;    // Must turn on the LED
    }
    
    // On/Off the LED if IR is pressed
    _bIRRCLedOn = (_bIRRCLedOn) ? FALSE : TRUE;
    i4Val = (_bIRRCLedOn) ? _u4IRRCBlinkLedOnVal : (1 - _u4IRRCBlinkLedOnVal);
    VERIFY(GPIO_Output(_u4IRRCBlinkLedGpio, &i4Val)== i4Val);
}

static void IRRC_LEDBlinkFunc(UINT32 u4BtnState, UINT32 u4Key,
                              IRRX_RAW_DATA_T *prRawData)
{
    static UINT32 u4LastCond = IRRX_COND_BTN_UP;

    UNUSED(u4Key);
    UNUSED(prRawData);

    if (!_bIRRCBlinkLedEnable)
    {
        return;
    }
    
    //Stop blinking IR Led if system go to standby mode...
    if ((u4Key & (~KEY_SRC_FRONT_PANEL)) == BTN_POWER)
    {
         return;
    }
//    LOG(1, "[IRRC_LEDBlinkFunc] Reg.\n");
    if (u4BtnState == IRRX_COND_BTN_DOWN)
    {
        if (u4LastCond != u4BtnState)
        {
            if (((HANDLE_T *)_hIRRCLedCtrlTimer != NULL) && (_u4IRRCBlinkDuration > 0))
            {
                //LOG(1, "[IRRC_LEDBlinkFunc] Create timer.\n");
                UNUSED(x_timer_start(_hIRRCLedCtrlTimer, _u4IRRCBlinkDuration,
                                     X_TIMER_FLAG_REPEAT, IRRC_IRLEDCtrlFunc,
                                     NULL));
            }
            else
            {
                INT32 i4Val = 1 - _u4IRRCBlinkLedOnVal;
                UNUSED(GPIO_Output(_u4IRRCBlinkLedGpio, &i4Val));
                _bIRRCLedOn = FALSE;
            }
        }
    }
    else
    {
        if ((HANDLE_T *)_hIRRCLedCtrlTimer != NULL)
        {
            //LOG(1, "[IRRC_LEDBlinkFunc] Stop timer.\n");            
            x_timer_stop(_hIRRCLedCtrlTimer);
        }
        
        UNUSED(GPIO_Output((INT32)_u4IRRCBlinkLedGpio, 
                           (INT32 *)&_u4IRRCBlinkLedOnVal));
        _bIRRCLedOn = TRUE;
    }

    u4LastCond = u4BtnState;
}

//-----------------------------------------------------------------------------
//                    public function.
//-----------------------------------------------------------------------------
UINT32 IRRC_IsPowerButtonOff()
{
    return _fgIRRCPowerButtonOff;
}

void IRRC_DisconnectMiddleware()
{
    _fgIRRCDisconnectMiddleware = 1;
}

void IRRC_WaitPowerKeyReboot()
{
    TvdConnectMainToVGA();
    _fgIRRCWaitPowerKeyReboot = 1;
}

void IRRC_LedBlinkConfig(BOOL bEnable)
{
    if(_bIRRCBlinkLedEnable != bEnable)
    {
        CRIT_STATE_T rCritState;
        rCritState = x_crit_start();
        Printf("_bIRRCBlinkLedEnable %d\n", bEnable);
        _bIRRCBlinkLedEnable = bEnable;
       if(!_bIRRCBlinkLedEnable)
      {
        if ((HANDLE_T *)_hIRRCLedCtrlTimer != NULL)
        {
            //LOG(1, "[IRRC_LEDBlinkFunc] Stop timer.\n");
            x_timer_stop(_hIRRCLedCtrlTimer);
            Printf("stop timer\n");
        }
        //IRRC_IRLEDCtrlFunc((HANDLE_T)NULL, NULL); //default On the Led
      }
        x_crit_end(rCritState);
    }

}
#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389) 
void IRRC_MixSound(UINT32 u4BtnState, UINT32 u4Key, IRRX_RAW_DATA_T* prRaw)
{
    UNUSED(u4Key);
    UNUSED(prRaw);
    if ((DRVCUST_InitGet(eFlagIrrxSoundSupport)) && u4IR_AudInitFlag == 0 && AUD_IsAudInit())
    {
        u4IR_AudInitFlag = 1;
        AUD_DspMixSndClip(AUD_MIXSND_CLIP1);
        if(DRVCUST_InitGet(eFlagIrrxSoundSupportAlwaysEnable))
        {
            LOG(0, "[MW_IRRC_NotifyFunc] enable sound play.\n");
            AUD_DspMixSndEnable(1);
        }
    }
    if (DRVCUST_InitGet(eFlagIrrxSoundSupport) && AUD_IsAudInit())
    {
        if(u4BtnState == IRRX_COND_BTN_DOWN)
        {
            LOG(9, "[MW_IRRC_NotifyFunc] sound play.\n");            
            AUD_DspMixSndControl(1);//AUD_DspMixSndControl(AUD_DEC_BTNSND_PLAY);
        }
    }
}
#endif 

#if defined(LINUX_TURNKEY_SOLUTION)||defined (__KERNEL__)
#if 0
static UINT32 u4RCDownCnt = 0;
static UINT32 u4RCUpCnt = 0;
#endif
static void DFB_IRRC_NotifyFunc(UINT32 u4BtnState, UINT32 u4Key, IRRX_RAW_DATA_T* prRaw)
{
    INT32 i4Ret;
    SIZE_T zMsgSize = sizeof(DFB_IRRC_NOTIFY_MSG);
    DFB_IRRC_NOTIFY_MSG rMsg;    
#if 0    
    if(u4Key == BTN_NONE)
    {
        LOG(9,"[IR]DFB_IRRC_NotifyFunc: discard BTN_NONE...\n");
        return;
    }
#endif 
#if 0   
        if(u4BtnState == IRRX_COND_BTN_DOWN)
        {
            u4RCDownCnt++;
        }
        if(u4BtnState == IRRX_COND_BTN_UP)
        {
            u4RCUpCnt++;
            if(u4RCUpCnt != u4RCDownCnt)
            {
                printf("DFB_IRRC_NotifyFunc error: down is not equal! Up %d, Down %d\n",u4RCUpCnt, u4RCDownCnt);
                printf("InputThread hang for ICE debug!!\n");
                while(u4RCUpCnt);
            }
        }    
#endif   
/*
    _rCurrRaw.u1Protocol = (IRRC_PROTOCOL_T)(UINT32)prRaw->u1Protocol;
    _rCurrRaw.u4Len = (SIZE_T)prRaw->u4Len;        
    x_memcpy(_rCurrRaw.au1Data, prRaw->au1Data, prRaw->u4Len);
    _CurrBtnState = u4BtnState;
    _CurrBtn = u4Key;

    //LOG(9, "[MW_IRRC_NotifyFunc]: BtnState(0x%8x), Key(0x%8x)\n", u4BtnState, u4Key);
    i4Ret = x_sema_unlock(_hRcSemaKey);
*/
    rMsg.rRaw.u1Protocol = (IRRC_PROTOCOL_T)(UINT32)prRaw->u1Protocol;
    rMsg.rRaw.u4Len = (SIZE_T)prRaw->u4Len;
    x_memcpy(rMsg.rRaw.au1Data, prRaw->au1Data, prRaw->u4Len);
    rMsg.u4BtnState = u4BtnState;
    rMsg.u4Btn = u4Key;
    i4Ret = x_msg_q_send(_hDfbMsgQue, &rMsg, zMsgSize,1);
    if (i4Ret != OSR_OK)
    {
        LOG(1, "%s(%d) SemaKey is fault. i4Ret:%d\n", __FILE__, __LINE__, i4Ret);
    }
#if 0    
    if (pf_irrc_usr_nfy_fct != NULL)
    {
        pf_irrc_usr_nfy_fct(pv_irrc_nfy_tag, (IRRC_COND_T)u4BtnState, u4Key, &rRawData);
    }
#endif  
#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389) 
    IRRC_MixSound(u4BtnState, u4Key, prRaw);
#endif  
}

//-----------------------------------------------------------------------------
/** IRRX_WaitMtkIr() to check there is any key available.
 *  @retval IR_FAIL run failed.
 *  @retval IR_OK run successfully.
 */
//-----------------------------------------------------------------------------
INT32 IRRC_WaitMtkIr(UINT32* prBtnState, UINT32* prKey, IRRX_RAW_DATA_T* rRawData)
{
    INT32 i4Ret;
    CRIT_STATE_T cState;
    UINT16 u2MsgIdx;
    DFB_IRRC_NOTIFY_MSG rMsg;
    SIZE_T zMsgSize = sizeof(DFB_IRRC_NOTIFY_MSG);
    if ((prBtnState == NULL) || (prKey == NULL) || (rRawData == NULL))
    {
        LOG(0, "[IRRC_WaitMtkIr]: Parameters error (null)\n");
        return IR_FAIL;
    }
    if (!_fgRcSemaInit || !_fgDfbRcThreadInit)
    {
        LOG(0, "[IRRC_WaitMtkIr]: _hRcSemaKey is uninit... \n");     
        *prKey = BTN_NONE;
        //x_thread_delay(1000000);
        return IR_SUCC;//IR_FAIL;
    }    
    
    LOG(10, "[IRRC_WaitMtkIr]: Sema lock\n");
    #if defined(CC_FAST_INIT) || defined(CC_SUPPORT_STR)
    //i4Ret = x_sema_lock_timeout(_hRcSemaKey, 500);  
    i4Ret = x_msg_q_receive_timeout(&u2MsgIdx, &rMsg, &zMsgSize, &_hDfbMsgQue, 1, 500);        
    #else
    //i4Ret = x_sema_lock(_hRcSemaKey, X_SEMA_OPTION_WAIT);  
    i4Ret = x_msg_q_receive(&u2MsgIdx, &rMsg, &zMsgSize, &_hDfbMsgQue, 1, X_MSGQ_OPTION_WAIT);    
    #endif
    LOG(10, "[IRRC_WaitMtkIr]: Sema unlock\n");
    LOG(10, "[IRRC_WaitMtkIr]: BtnState(0x%8x), Key(0x%8x)\n", rMsg.u4BtnState, rMsg.u4Btn);        
    if (i4Ret == OSR_OK)
    {
        cState = x_crit_start();
        
        rRawData->u1Protocol = rMsg.rRaw.u1Protocol;
        rRawData->u4Len = rMsg.rRaw.u4Len;
        x_memcpy(rRawData->au1Data, rMsg.rRaw.au1Data, rMsg.rRaw.u4Len);
        
        *prBtnState = rMsg.u4BtnState;
        *prKey = rMsg.u4Btn;
        x_crit_end(cState);

    }
    else
    {
        LOG(9, "[IRRX_WaitMtkIr]: _hRcSemaKey error(0x%x) \n", i4Ret);     
        #if !defined(CC_FAST_INIT) && !defined(CC_SUPPORT_STR)
        ASSERT((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));
        #endif
        *prKey = BTN_NONE;
        #if defined(CC_FAST_INIT) || defined(CC_SUPPORT_STR)
        return IR_SUCC;
        #else
        return IR_FAIL;
        #endif
    }

    if (*prKey == BTN_NONE)
    {
        LOG(9, "%s return Key:0x%08x\n", __FUNCTION__, *prKey);
    }
    return IR_SUCC;
}

INT32 IRRC_Init(UINT32 u4PriType)
{
    INT32       i4_return;
    /* Init MtkIr */
    LOG(1, "[IRRC_Init] : IR type 0x%x\n", u4PriType);
    if (IRRX_InitMtkIr(u4PriType) < 0)
    {
        LOG(0, "IRRC_Init(): chip init failed\n");
        return IR_FAIL;//(-1);
    }
    LOG(1, "[IRRC_Init] : Create IRRC Semaphore.\n"); 
    if (!_fgRcSemaInit)
    {
        i4_return = x_msg_q_create(&_hDfbMsgQue, "DfbIrrcMsgQue", 
                          sizeof(DFB_IRRC_NOTIFY_MSG), 16);
        if (i4_return != OSR_OK)
        {
            return IR_FAIL;
        }
        else
        {
            LOG(1, "[MW_IRRC_Init] IR init OK!\n");
        }
        _fgRcSemaInit = TRUE;
    }
	if(!_fgDfbRcThreadInit)	
	{
        if(DRVCUST_InitGet(eIrrcCheckRepeatByRaw) != 0)
        {
            UNUSED(DRVCUST_InitQuery(eIrrcIsUsrLongPress, (UINT32 *)(void *)&(_pfnIsUsrLongPress)));    
        }
        LOG(1, "[IRRC_Init] : Create IRRC data process thread.\n"); 
        i4_return = IRRC_ThreadInit(DFB_IRRC_NotifyFunc);
        if (i4_return)
        {
            Printf("IRRX_MW_Init(): failed [%d]\n", (UINT32)i4_return);
            return (-1);
        }
		_fgDfbRcThreadInit = TRUE;
	}		
#if 0
    UNUSED(DRVCUST_InitQuery(eIrrxSetUsrNotifyFunc, 
                             (UINT32 *)(void *)&(pf_irrc_usr_nfy_fct)));
#endif
    return IR_SUCC;
}
#endif

INT32 IRRC_ThreadInit(PFN_IRRXCB_THREAD_T pfnThredCallback)
{
    /* Init MtkIr */
    if (IRRX_InitMtkIr(DRVCUST_OptGet(eIrrxType)) < 0)
    {
        LOG(1, "IRRC_Init(): chip init failed\n");
        return (-1);
    }

    pfnIrrxThreadCallback = IRRC_CallbackHandle;
    UNUSED(IRRC_RegCallbackFunc(pfnThredCallback));

    if ((DRVCUST_InitQuery(eIrrxLedBlinkGpio, &_u4IRRCBlinkLedGpio) == 0) &&
        (DRVCUST_InitQuery(eIrrxLedBlinkOnVal, &_u4IRRCBlinkLedOnVal) == 0))
    {
        // Create the timer for LED blink control 
        // and register ir notify function
        if (x_timer_create(&_hIRRCLedCtrlTimer) == OSR_OK)
        {
            UINT32 u4Val;
            if (DRVCUST_InitQuery(eIrrxLedBlinkDuration, &u4Val) == 0)
            {
                _u4IRRCBlinkDuration = u4Val;
            }
            LOG(1, "IRRC_LEDBlinkFunc Reg.\n"); 
            UNUSED(IRRC_RegCallbackFunc(IRRC_LEDBlinkFunc));
        }
        else
        {
            LOG(5, "IRRC_Init(): Create IR LED Blink timer fail !\n");
        }
    }
   
#if !((!defined(LINUX_TURNKEY_SOLUTION)) && defined(__KERNEL__))//linux drv_build should not use it    
    if(DRVCUST_InitGet(eIrrcCheckRepeatByRaw) != 0)
    {
        UNUSED(DRVCUST_InitQuery(eIrrcIsUsrLongPress, (UINT32 *)(void *)&(_pfnIsUsrLongPress)));
    }
/* Create IRRC data processing thread */
    if (x_thread_create(&hIRRCThread,
                        "IRRC Thread",
                        IRRC_STACK_SIZE,
                        IRRC_THREAD_PRIORITY,
                        IRRC_DataProcessingThread,
                        0,
                        NULL) != OSR_OK)
    {
        LOG(1, "\nError: Create CLI thread fail\n");

        /* Hold the system */
        while (1) {}
    }
#endif    
//    pfnIrrxThreadCallback = pfnThredCallback;

    return 0;
}

INT32 IRRC_RegCallbackFunc(PFN_IRRXCB_THREAD_T pfnCallback)
{
    UINT32 u4Val;

    if (pfnCallback == NULL)
    {
        LOG(7, "[IRRC_RegCallbackFunc] The reg function is NULL\n");
        return -1;
    }
    
    for (u4Val = 0; u4Val < IRRC_CALLBACK_NO; u4Val++)
    {
        if (pfnIrrxCallbackTbl[u4Val] == NULL)
        {
            pfnIrrxCallbackTbl[u4Val] = pfnCallback;
            return 0;
        }
    }
    
    LOG(7, "[IRRC_RegCallbackFunc] The notify function table is full\n");
    return -1;
}

INT32 IRRC_UnRegCallbackFunc(void)
{
    UINT32 u4Val;

    for (u4Val = 0; u4Val < IRRC_CALLBACK_NO; u4Val++)
    {
        if (pfnIrrxCallbackTbl[u4Val] != NULL)
        {
            pfnIrrxCallbackTbl[u4Val] = NULL;
        }
    }
    
    LOG(7, "[IRRC_UnRegCallbackFunc] The notify function table is empty\n");
    return 0;
}

#ifdef IRRC_LOG
VOID IRRC_ResetLog(VOID)
{
    x_memset((void*)&_rLogger, 0, sizeof(_rLogger));
}

UINT32 IRRC_GetLog(IRRC_LOGGER_T **pprLoggger)
{
    *pprLoggger = &_rLogger;
    return IRRC_LOG_SIZE;
}
#endif
