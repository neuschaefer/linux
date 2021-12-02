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
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: smc_hp.c,v $
 * $Revision: #1 $
 *
 * Description: 
 *         This header file contains Interfaces of SmartCard.
 *---------------------------------------------------------------------------*/

/** @file smc_hp.c
 *  smc_hp.c is the Interfaces of SmartCard.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_typedef.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_gpio.h"
#include "smc_debug.h"
#include "smc_drvif.h"
#include "smc_apdu.h"
#include "smc_bcas.h"
#include "smc_hp.h"
#include "drvcust_if.h"


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
#define SMC_HOTPLUG_EVENT_THREAD (0) // Hotplug event callback function invoked in thread or ISR context


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define SMC_MAX_HOTPLUG_MSG_COUNT 32


//-----------------------------------------------------------------------------
// Static variable
//-----------------------------------------------------------------------------
#if SMC_HOTPLUG_EVENT_THREAD
static HANDLE_T _hSmcHotPlugThread = NULL;
static HANDLE_T _hSmcHotPlugMsgq = NULL;
#endif
static PFN_SMC_HOTPLUG_CALLBACK _pfnSmcHotPlugCb = NULL; 
#ifdef CC_FPGA   
static SMC_CARD_STATUS_T _rSmcCardStatus = SMC_CARD_PRESENT;
#else
static SMC_CARD_STATUS_T _rSmcCardStatus = SMC_CARD_ABSENT;
#endif


//-----------------------------------------------------------------------------
// Static function
//-----------------------------------------------------------------------------
#if SMC_HOTPLUG_EVENT_THREAD
static void _SmcHotPlugIsr(INT32 i4Gpio, BOOL fgStatus)
{
    UINT8 u1Msg;
    UINT16 u2MsgCount = 0;
    VERIFY(i4Gpio == SMC_HOTPLUG_GPIO);
    u1Msg = (UINT8)((SMC_HOTPLUG_GPIO_POLARITY)?(fgStatus):(!fgStatus));        
    VERIFY(x_msg_q_num_msgs(_hSmcHotPlugMsgq, &u2MsgCount) == OSR_OK);
    if(u2MsgCount < SMC_MAX_HOTPLUG_MSG_COUNT)
    {
        VERIFY(x_msg_q_send(_hSmcHotPlugMsgq, (const VOID*)&u1Msg, sizeof(UINT8), 100) == OSR_OK);    
    }
    else
    {
        SMC_ERROR_PRINT("Hotplug message queue overflow!\n");
    }
}
#else
static void _SmcHotPlugIsr(INT32 i4Gpio, BOOL fgStatus)
{
    VERIFY(i4Gpio == SMC_HOTPLUG_GPIO);
    if((BOOL)SMC_HOTPLUG_GPIO_POLARITY == fgStatus)
    {
        _rSmcCardStatus = SMC_CARD_PRESENT;  
    }
    else
    {
        _rSmcCardStatus = SMC_CARD_ABSENT;      
    }    
    if(_pfnSmcHotPlugCb)
    {
        _pfnSmcHotPlugCb((UINT8)((SMC_HOTPLUG_GPIO_POLARITY)?(fgStatus):(!fgStatus)));       
    }        
}
#endif


#if SMC_HOTPLUG_EVENT_THREAD
static void _SmcHotPlugThread(void* pvArg)
{
    UINT16 u2Idx;
    UINT8 u1Msg;
    SIZE_T rSize = sizeof(UINT8);

    UNUSED(pvArg);    
    
    while(1)
    {
        u2Idx = 0;        
        if(x_msg_q_receive(&u2Idx, (VOID*)&u1Msg , &rSize, &_hSmcHotPlugMsgq, 1 , X_MSGQ_OPTION_WAIT) == OSR_OK)
        {
            _SMC_Op_Lock();
            if(u1Msg)
            {   
                SMC_LOG_PRINT("Hot plug detected.\n");            
                if(_SMC_Reactivate() == 0)
                {
                    _rSmcCardStatus = SMC_CARD_ACTIVATED;                        
                    SMC_LOG_PRINT("Card activated.\n");                                
                    _SMC_RequestIFS(0xFE);                     
                    SMC_LOG_PRINT("IFS reqeusted automatically.\n");                    
                }
                else
                {
                    _rSmcCardStatus = SMC_CARD_INIT_FAIL;            
                }
            }        
            else
            {  
                _rSmcCardStatus = SMC_CARD_ABSENT;            
                SMC_LOG_PRINT("Unplug detected.\n");                          
            }
            _SMC_Op_Unlock();            
            if(_pfnSmcHotPlugCb)
            {
                _pfnSmcHotPlugCb(u1Msg);       
            }          
        }
    }    	

}

static INT32 _SmcInitHotPlugThread(void)
{  
    INT32 i4Ret = 0;
    i4Ret = x_msg_q_create(&_hSmcHotPlugMsgq, "SMC_HP_M", sizeof(UINT32), SMC_MAX_HOTPLUG_MSG_COUNT);        
    i4Ret = x_thread_create(&_hSmcHotPlugThread, "SMC_HP_T", 2048, 100, _SmcHotPlugThread, 0, NULL);
    return i4Ret;
}
#endif

//-----------------------------------------------------------------------------
// Inter-file function
//-----------------------------------------------------------------------------
INT32 _SMC_InitHotPlugDetector (void)
{
    INT32 i4Set = 1;
#if SMC_HOTPLUG_EVENT_THREAD    
    UINT8 u1Msg = SMC_HOTPLUG_GPIO_POLARITY;
    VERIFY(_SmcInitHotPlugThread() == 0);  
    VERIFY(x_msg_q_send(_hSmcHotPlugMsgq, (const VOID*)&u1Msg, sizeof(UINT8), 100) == OSR_OK);    
#endif
    GPIO_Reg(SMC_HOTPLUG_GPIO, GPIO_TYPE_INTR_BOTH, _SmcHotPlugIsr);
    GPIO_Intrq(SMC_HOTPLUG_GPIO, &i4Set);
    return 0;
}

void _SMC_SetHotPlugCb (PFN_SMC_HOTPLUG_CALLBACK pfnCb)
{
    _pfnSmcHotPlugCb = pfnCb;
    return;
}

SMC_CARD_STATUS_T _SMC_GetStatus (void)
{
    return _rSmcCardStatus;
}

void _SMC_SetStatus (SMC_CARD_STATUS_T rStatus)
{
    _rSmcCardStatus = rStatus;
    
}

