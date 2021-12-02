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
/***************    MTK CONFIDENTIAL & COPYRIGHTED     ****************/
/***************                                       ****************/
/***************  $Modtime:: 04/12/06 3:49p    $       ****************/
/***************  $Revision: #1 $       ****************/
/***************                                       ****************/
/***************   Description : DSP Control routines  ****************/
/***************                                       ****************/
/***************       Company : MediaTek Inc.         ****************/
/***************    Programmer : Jack Hsu              ****************/
/***************                 Peichen Chang         ****************/
/**********************************************************************/
#define _DSP_STATE_C
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN

#include "dsp_common.h"
#include "dsp_intf.h"
#include "dsp_func.h"
#include "dsp_shm.h"
#include "dsp_uop.h"
#include "dsp_rg_ctl.h"
#include "d2rc_shm.h"

#include "aud_debug.h"
#include "x_os.h"
#include "x_hal_5381.h"
LINT_EXT_HEADER_END

void vDspPowerOff (void);
void vDspPowerOn (void);
void vDspSafeSendInt (UINT32 u4Addr, UINT32 u4Data);
    
extern UINT32 u4GetDspFlashVersion (void);
extern BOOL fgCheckDspCmd(UINT32 u4DspCmd);


/***************************************************************************
     Function : vDSPState
  Description : DSP State Machine
    Parameter : None
    Return    : None
***************************************************************************/
void vDspState(UINT32 u4Msg)
{
    switch (_uDspState)
    {
    case DSP_CK_INT:
        if (!_rDspStatus[AUD_DEC_MAIN].fgDspSInt) // For RISC to DSP interrupt enabled in vDspIntSvc()
        {
            BOOL u1GetCmdRet;
            u1GetCmdRet = fgGetDspCmd(u4Msg);
            if (fgCheckDspCmd(u4Msg))
            { 
                vDspUopSvc(u4Msg);
            }
            UNUSED(u1GetCmdRet);
        }
            
        if (_rDspStatus[AUD_DEC_MAIN].fgDspSInt) // if need to send interrupt
        {
            while (fgDspBusy()) // Light: wait for DSP not busy
            {
                x_thread_delay(1);
            }
            vSendDspLongInt (_uDspSIntAddr, _u4DspSIntSD, _u4DspSIntLD);
            _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
        }               
        _uDspState = (UINT8)DSP_CK_INT;
        break;
         
    case DSP_WAIT_POWER_OFF:
#ifdef CC_AUD_4_DECODER_SUPPORT
        if (_rDspStatus[AUD_DEC_MAIN].fgDspStop &&
            _rDspStatus[AUD_DEC_AUX].fgDspStop &&
            _rDspStatus[AUD_DEC_THIRD].fgDspStop &&
            _rDspStatus[AUD_DEC_4TH].fgDspStop) 
#elif defined (CC_MT5391_AUD_3_DECODER)
        if (_rDspStatus[AUD_DEC_MAIN].fgDspStop &&
            _rDspStatus[AUD_DEC_AUX].fgDspStop &&
            _rDspStatus[AUD_DEC_THIRD].fgDspStop) 
#else
        if (_rDspStatus[AUD_DEC_MAIN].fgDspStop &&
            _rDspStatus[AUD_DEC_AUX].fgDspStop) // Light: wait for FLUSH_DONE command
#endif
        {
            vDspPowerOff();
            _uDspState = (UINT8)DSP_IDLE; // Light: change state if poweroff is finished
        }
#ifdef CC_AUD_4_DECODER_SUPPORT
        else if((!_rDspStatus[AUD_DEC_MAIN].fgDspStopIssue) ||
            (!_rDspStatus[AUD_DEC_AUX].fgDspStopIssue) ||
            (!_rDspStatus[AUD_DEC_THIRD].fgDspStopIssue) ||
            (!_rDspStatus[AUD_DEC_4TH].fgDspStopIssue))
#elif defined (CC_MT5391_AUD_3_DECODER)
        else if((!_rDspStatus[AUD_DEC_MAIN].fgDspStopIssue) ||
            (!_rDspStatus[AUD_DEC_AUX].fgDspStopIssue) ||
            (!_rDspStatus[AUD_DEC_THIRD].fgDspStopIssue))
#else
        else if((!_rDspStatus[AUD_DEC_MAIN].fgDspStopIssue) ||
            (!_rDspStatus[AUD_DEC_AUX].fgDspStopIssue)) // Light: Not receive stop command, continue to receive STOP command
#endif
        {
            BOOL u1GetCmdRet;
            u1GetCmdRet = fgGetDspCmd(u4Msg);
#ifdef CC_AUD_4_DECODER_SUPPORT
            if ((u4Msg==DSP_STOP)||(u4Msg==DSP_STOP_DEC2)||(u4Msg==DSP_STOP_DEC3)||(u4Msg==DSP_STOP_DEC4))
#elif defined (CC_MT5391_AUD_3_DECODER)
            if ((u4Msg==DSP_STOP)||(u4Msg==DSP_STOP_DEC2)||(u4Msg==DSP_STOP_DEC3))
#else
            if ((u4Msg==DSP_STOP)||(u4Msg==DSP_STOP_DEC2))
#endif
            {
                CRIT_STATE_T rCritState;                
                vDspUopSvc(u4Msg); // Set for DSP stop command
                while (TRUE)
                {
                    rCritState = x_crit_start();
                    if (fgDspBusy())
                    {
                        x_crit_end(rCritState);
                        x_thread_delay(1);
                    }
                    else
                    {
                        vSendDspLongInt (_uDspSIntAddr, _u4DspSIntSD, _u4DspSIntLD);
                        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
                        x_crit_end(rCritState);
                    }
                }    
            }
            UNUSED(u1GetCmdRet);
        }
        break;
             
    case DSP_IDLE:
        break;   
             
    default:
        _uDspState = (UINT8)DSP_CK_INT;
        break;   
    }
}

/***************************************************************************
     Function : vDSPPowerOn
  Description : Initialize DSP when Powen On
    Parameter : None
    Return    : None

**  Caution   : Set Blk regs
                Ini dram values
                enable microphone
                reset dsp
                Some setting is NOT involved in this function, like Bistream
                buffer initialization.

***************************************************************************/
void vDspPowerOn (void)
{
    IniBlkRegs ();
    vDspInit ();                // initial DSP dram value
    HdRstDsp ();                // Reset Dsp
}

/***************************************************************************
     Function : vDSPPowerOff
  Description : Stop DSP when Powen Off
    Parameter : None
    Return    : None
***************************************************************************/
void vDspPowerOff (void)
{
    HdStpDsp ();                // Disable dsp clk
    _rDspStatus[AUD_DEC_MAIN].fgDspWakeUp = FALSE;
}

/***************************************************************************
     Function : fgDspInitState
  Description : Return true when wake up ok
    Parameter : None
    Return    : None
***************************************************************************/
BOOL fgDspInitState (void)
{
    UINT32 dwFlashVer;
    BOOL fgRet = FALSE;
    UINT32 dwWakeUpTimer=0;
    
    vDspPowerOn ();
    //lint -e{506}
    //lint -e{774}
    if (_fgCommRAMFromFlash)
    {
        vDspLoadRamCode (AUD_COMMRAM);
    }
    //lint -e{506}
    //lint -e{774}
    if (_fgPostRamFromFlash)
    {
        vDspLoadRamCode (AUD_POSTRAM);
    }
    _rDspStatus[AUD_DEC_MAIN].fgDspWakeUpOk = FALSE;

    // Wake up Dsp
    SendDSPInt (INT_RC2D_INTERRUPT_CTRL, RISC_WAKEUP_DSP);
    
    // has wake up dsp
    _rDspStatus[AUD_DEC_MAIN].fgDspWakeUp = TRUE;

    // FIXME !!!
    _rDspStatus[AUD_DEC_MAIN].fgDspWakeUpOk = TRUE;
        
    // Light: add timeout of DSP wakeup
    while(!_rDspStatus[AUD_DEC_MAIN].fgDspWakeUpOk) // Light: wait for DSP wakeup
    {
        dwWakeUpTimer++;
        x_thread_delay(1);
        if(dwWakeUpTimer>50) // Time out if > 50 ms
        {
            LOG(1, "Wake up fail over 50 ms\n");
            fgRet = FALSE;
            return fgRet;
        }
        else if((dwWakeUpTimer % 10) == 2) // Do reset again
        {
            LOG(1, "Wake up fail, and do DSP reset\n");
            //WriteREG (RW_ADSP_CFG, ReadREG (RW_ADSP_CFG) & ~ADSP_CFG_RST_B);
            //WriteREG (RW_ADSP_CFG, ReadREG (RW_ADSP_CFG) | ADSP_CFG_RST_B);
            DSP_ResetDsp();

            // Wake up Dsp
            SendDSPInt (INT_RC2D_INTERRUPT_CTRL, RISC_WAKEUP_DSP);
        }           
    }

    DSP_FlushInvalidateDCacheSmall2(ADDR_RISC_FLASH_VERSION);
    dwFlashVer = u4GetDspFlashVersion ();
    WriteDspCommDram (ADDR_RISC_FLASH_VERSION, dwFlashVer);
       
    fgRet = TRUE;
    return fgRet;
}

void vDspSafeSendInt (UINT32 u4Addr, UINT32 u4Data)
{
    UINT32 dwCnt = 0;

    while (1)
    {
        if (!fgDspBusy ())
        {
            SendDSPInt (u4Addr, u4Data);
            break;
        }
        dwCnt++;
        if (dwCnt > 0x500)
        {
            SendDSPInt (u4Addr, u4Data);
            _rDspVars[AUD_DEC_MAIN].dwDspSIntFail++;
            break;
        }
    }
}


