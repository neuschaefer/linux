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
 * $RCSfile: drv_if_ir.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file IR.c
 *  Driver interface: IR part implementation
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_assert.h"
#include "x_os.h"
#include "x_timer.h"
#include "ir_if.h"
#include "drvcust_if.h"
LINT_EXT_HEADER_END
#include "x_drv_if.h"
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define DRV_IR_BTN_BUFFER_SIZE 16
//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static HANDLE_T _hIrThread;
static DRV_IR_PARAM_T _rIrParam;
static PFN_DRV_IR_CALLBACK _pfnIrThreadCallback = NULL;
static DRV_IR_EVENT_T _arBtnBuffer[DRV_IR_BTN_BUFFER_SIZE];
static UINT8 _u1BtnBufferWp = 0; 
static UINT8 _u1BtnBufferRp = 0; 
static UINT32 _au4RepeatFreqTable[3] = 
{
    0,
    100000,
    50000
};

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static void _BtnEventNfy(DRV_IR_EVENT_TYPE_T rType, UINT32 u4Key)
{
    DRV_IR_EVENT_T rIrEvent;
    rIrEvent.rType = rType;
    rIrEvent.u4Key = u4Key;
    
    _pfnIrThreadCallback(rType,u4Key);      
      
}

static void _DataProcessingThread(void* pvArg)
{
    //static UINT32 u4PowerDown = 0;
    HAL_TIME_T rPrevTime = { 0, 0 };
    INT32 i4Ret;
    UINT32 u4Key, u4PrevKey;
    UINT32 u4Raw;
    //DTVCFG_T rDtvCfg;

    UNUSED(pvArg);
    u4PrevKey = 20; // for key up/down system, this time as small as possible.
    //LINT_SUPPRESS_NEXT_EXPRESSION(534);
    IRRX_QuerySetRepeatTime(&u4PrevKey);
    i4Ret = 1;
    //LINT_SUPPRESS_NEXT_EXPRESSION(534);
    IRRX_QuerySetRepeat(&i4Ret);
    u4PrevKey = BTN_NONE;

    while(1)
    {
        i4Ret = IRRX_PollMtkIr(&u4Key, &u4Raw);

// Power control related operations, currently not supported by miniDVB high level API        
/*
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
*/

        /* if no call back function, no need to check IR return Key. */
        if (_pfnIrThreadCallback == NULL) 
        { 
            continue; 
        }

        if (i4Ret == IR_SUCC) 
        {
            if (u4Key == BTN_NONE) /* polling timeout. */
            { 
                if (u4PrevKey != BTN_NONE) /* Here is a key up. */
                { 
                    _BtnEventNfy(DRV_IR_EVENT_UP, u4PrevKey);
                    u4PrevKey = BTN_NONE;
                }
            } 
            else if (u4Key == BTN_DRVKEY_REPEAT) 
            {
                /* If repeat the same key, notify repeat. */
  
            } 
            else if ((u4Key & KEY_SRC_DRIVER) == KEY_SRC_DRIVER) 
            {
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
                        _BtnEventNfy(DRV_IR_EVENT_DOWN, BTN_DVD);
                        _BtnEventNfy(DRV_IR_EVENT_UP, BTN_DVD);
                        break;
                    default:
                        if ((u4Channel / 100) != 0) {
                            _BtnEventNfy(DRV_IR_EVENT_DOWN, BTN_DIGIT_1);
                            _BtnEventNfy(DRV_IR_EVENT_UP, BTN_DIGIT_1);
                        }
                        u4Num = ((u4Channel / 10) % 10);
                        if (u4Num != 0) {
                            _BtnEventNfy(DRV_IR_EVENT_DOWN, (BTN_DIGIT_0 + u4Num));
                            _BtnEventNfy(DRV_IR_EVENT_UP, (BTN_DIGIT_0 + u4Num));
                        }
                        u4Num = (u4Channel % 10);
                        _BtnEventNfy(DRV_IR_EVENT_DOWN, (BTN_DIGIT_0 + u4Num));
                        _BtnEventNfy(DRV_IR_EVENT_UP, (BTN_DIGIT_0 + u4Num));
                        break;
                    }
                    u4PrevKey = BTN_NONE;
                }
            } 
            else if (u4PrevKey == u4Key) /* the same key with previous key. */
            { 
                HAL_TIME_T rTime, rDelta;
                
                HAL_GetTime(&rTime);
                HAL_GetDeltaTime(&rDelta, &rPrevTime, &rTime);
#if 0
                if ((rDelta.u4Seconds > 0) ||
                    (rDelta.u4Micros > (1000*IRRX_QuerySetRepeatDuration(NULL)))) {
                    _BtnEventNfy(DRV_IR_EVENT_UP, u4Key);
                    _BtnEventNfy(DRV_IR_EVENT_DOWN, u4Key);
                    
                } 
#else
                if ((_rIrParam.rRepeatFreq != DRV_IR_REPEAT_FREQ_NONE) && ((rDelta.u4Seconds > 0) ||
                    (rDelta.u4Micros > _au4RepeatFreqTable[(UINT32)(_rIrParam.rRepeatFreq)]))) 
                {
                    _BtnEventNfy(DRV_IR_EVENT_REPEAT, u4Key);                    
                } 
#endif
                                
                HAL_GetTime(&rPrevTime);
            } 
            else 
            { /* there is some key useful/valid. */
                if (u4PrevKey != BTN_NONE) {
                    _BtnEventNfy(DRV_IR_EVENT_UP, u4PrevKey);
                }
                _BtnEventNfy(DRV_IR_EVENT_DOWN, u4Key);
                u4PrevKey = u4Key;
                HAL_GetTime(&rPrevTime);

// EEPROM related operations, currently not supported by miniDVB high level API
/*
                if ((u4PowerDown!=u4Key) && (DRVCUST_OptGet(eStdbyUsePowerBtnCount))
                    && (u4Key == (BTN_POWER | KEY_SRC_FRONT_PANEL)))
                {
                    // Set POWER_BTN_ON from X to 0.
                    u4PowerDown = u4Key;
                    i4Ret = EEPDTV_Read((UINT64)0, (UINT32)(void *)&rDtvCfg, sizeof(rDtvCfg));
                    ASSERT(i4Ret==0);
                    rDtvCfg.u1Flags &= ~(DTVCFG_FLAG_POWER_BTN_ON);
                    i4Ret = EEPDTV_Write((UINT64)0, (UINT32)(void *)&rDtvCfg, sizeof(rDtvCfg));
                    ASSERT(i4Ret==0);
                }
                if ((u4PowerDown!=u4Key) && (DRVCUST_OptGet(eStdbyUsePowerBtnCount))
                    && (u4Key == (BTN_POWER | KEY_SRC_REMOTE_CTRL)))
                {
                    // Set POWER_BTN_ON from X to 1.
                    u4PowerDown = u4Key;
                    i4Ret = EEPDTV_Read((UINT64)0, (UINT32)(void *)&rDtvCfg, sizeof(rDtvCfg));
                    ASSERT(i4Ret==0);
                    rDtvCfg.u1Flags |= (DTVCFG_FLAG_POWER_BTN_ON);
                    i4Ret = EEPDTV_Write((UINT64)0, (UINT32)(void *)&rDtvCfg, sizeof(rDtvCfg));
                    ASSERT(i4Ret==0);
                }
*/                
            }
        }

        x_thread_delay(10);
    }
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Exported functionss
//-----------------------------------------------------------------------------


DRV_ERR_CODE_T DRV_IR_SetParameter(DRV_IR_PARAM_T* prParam)
{
    VERIFY(prParam != NULL);
    _rIrParam.rIrType = prParam->rIrType;
    _rIrParam.pfnBtnEvent = prParam->pfnBtnEvent;
    _rIrParam.rRepeatFreq = prParam->rRepeatFreq;
    return DRV_E_OK;
}

DRV_ERR_CODE_T DRV_IR_SetHandler(PFN_DRV_IR_CALLBACK        pfnBtnEvent)
{
    VERIFY(pfnBtnEvent != NULL);
    _rIrParam.pfnBtnEvent = pfnBtnEvent;
    return DRV_E_OK;
}

DRV_ERR_CODE_T DRV_IR_Start(void)
{
    UINT32 u4IrType;
    
    if(_rIrParam.rIrType == DRV_IR_TYPE_NEC)
    {
        u4IrType = IRRX_NEC_TYPE;
    }
    else if(_rIrParam.rIrType == DRV_IR_TYPE_RC56)
    {
        u4IrType = IRRX_RC5_TYPE;
    }
    else
    {
        u4IrType = DRVCUST_OptGet(eIrrxType);
    }
    /* Init MtkIr */
    if (IRRX_InitMtkIr(u4IrType) < 0)
    {
        printf("IRRC_Init(): chip init failed\n");
        return DRV_E_FAIL;
    }
    
    /* Create IRRC data processing thread */
    if (x_thread_create(&_hIrThread,
                        "T_IR",
                        1024,
                        100,
                        _DataProcessingThread,
                        0,
                        NULL) != OSR_OK)
    {
        printf("\nError: Create CLI thread fail\n");

        /* Hold the system */
        while (1) {}
    }

    _pfnIrThreadCallback = _rIrParam.pfnBtnEvent;

    return DRV_E_OK;
}

DRV_ERR_CODE_T DRV_IR_Stop(void)
{
    _pfnIrThreadCallback = NULL;
    if(IRRX_StopMtkIr() == IR_SUCC)
    {
        return DRV_E_OK;
    }
    else
    {
        return DRV_E_FAIL;
    }
}

DRV_ERR_CODE_T DRV_IR_GetKey(DRV_IR_EVENT_T* prEvent, UINT32 u4Tag)
{
    if(u4Tag < DRV_IR_BTN_BUFFER_SIZE)
    {
        if(_u1BtnBufferRp == _u1BtnBufferWp)
        {       
            prEvent->rType = DRV_IR_EVENT_DUMMY;
            prEvent->u4Key = BTN_NONE;
            return DRV_E_FAIL;
        }
        prEvent->rType = _arBtnBuffer[_u1BtnBufferRp].rType;
        prEvent->u4Key = _arBtnBuffer[_u1BtnBufferRp].u4Key;
        _u1BtnBufferRp = (_u1BtnBufferRp + 1) % DRV_IR_BTN_BUFFER_SIZE;        
        return DRV_E_OK;
    }
    return DRV_E_FAIL;
}
