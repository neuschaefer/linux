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

#include "dsp_general.h"
#include "dsp_intf.h"
#include "dsp_func.h"
#include "dsp_shm.h"
#include "dsp_uop.h"
#include "dsp_rg_ctl.h"
#include "d2rc_shm.h"

#include "aud_debug.h"
#include "x_os.h"
#include "x_hal_926.h"
LINT_EXT_HEADER_END

void vDspPowerOff (void);

#ifndef DSP_NO_INFO
  #ifdef EMULATION_FPGA
static void UpdateChannelVolume (void);
static void vDspShowInfo (void);
  #endif
#endif
void vDspPowerOn (void);
void vDspSafeSendInt (UINT32 u4Addr, UINT32 u4Data);
    
/*
static BOOL fgFlushState(void);
*/

#ifdef EMULATION_FPGA
static void vInformBitRate (void);
#endif
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
             if(!_fgDspSInt) // For RISC to DSP interrupt enabled in vDspIntSvc()
             {
                BOOL u1GetCmdRet;
                u1GetCmdRet = fgGetDspCmd(u4Msg);
      	          if (fgCheckDspCmd(u4Msg))
                  { 
                     vDspUopSvc(u4Msg);
                  }
                  UNUSED(u1GetCmdRet);
             	}
             if (_fgDspSInt) // if need to send interrupt
             {
                 while( fgDspBusy() ) // Light: wait for DSP not busy
             	 {
             	     x_thread_delay(1);
             	 }
                 vDspSendInt ();
             }               
             _uDspState = (UINT8)DSP_CK_INT;
         break;
         
         case DSP_WAIT_POWER_OFF:
#ifndef CC_MT5391_AUD_3_DECODER     
//#if 1
             if(_fgDspStop && _fgDspStopDec2) // Light: wait for FLUSH_DONE command
#else
             if(_fgDspStop && _fgDspStopDec2 && _fgDspStopDec3) 
#endif
             {
                 vDspPowerOff();
                 _uDspState = (UINT8)DSP_IDLE; // Light: change state if poweroff is finished
             }
#ifndef CC_MT5391_AUD_3_DECODER    
//#if 1
             else if((!_fgDspStopIssue)||(!_fgDspStopIssueDec2)) // Light: Not receive stop command, continue to receive STOP command
#else
             else if((!_fgDspStopIssue)||(!_fgDspStopIssueDec2)||(!_fgDspStopIssueDec3))
#endif
             {
                BOOL u1GetCmdRet;
                 u1GetCmdRet = fgGetDspCmd(u4Msg);
#ifndef CC_MT5391_AUD_3_DECODER 
//#if 1
      	          if ((u4Msg==DSP_STOP)||(u4Msg==DSP_STOP_DEC2))
#else
                 if ((u4Msg==DSP_STOP)||(u4Msg==DSP_STOP_DEC2)||(u4Msg==DSP_STOP_DEC3))
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
                              vDspSendInt ();
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
    //vDspCmd(0x3a8);
}

/***************************************************************************
     Function : vDSPShowInfo
  Description : Pull information from DSP and notify 8032
    Parameter : None
    Return    : None
***************************************************************************/
#ifdef EMULATION_FPGA
void vDspShowInfo (void)
{

}
#endif // EMULATION_FPGA

/***************************************************************************
     Function : vDSPPowerOff
  Description : Stop DSP when Powen Off
    Parameter : None
    Return    : None
***************************************************************************/
void vDspPowerOff (void)
{

    HdStpDsp ();                // Disable dsp clk
    _fgDspWakeUp = FALSE;
}

/***************************************************************************
     Function : UpdateChannelVolume
  Description : Set Volume information of DSP to 8032
    Parameter : None
    Return    : None
***************************************************************************/
#ifdef EMULATION_FPGA
void UpdateChannelVolume (void)
{
#ifdef DSP_AUTO_MUTE
    UINT32 dwLevel;
#endif
    vUpdateShmVol ();
#ifdef DSP_AUTO_MUTE
    HalFlushInvalidateDCache(); //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro
    if (uReadDspShmBYTE (B_DACBIT) & 0x20)
    {
        //Check Volume Level
        dwLevel = dReadDspCommDram (ADDR_D2RC_CH_VOLUME_L);
        dwLevel += dReadDspCommDram (ADDR_D2RC_CH_VOLUME_R);
        dwLevel += dReadDspCommDram (ADDR_D2RC_CH_VOLUME_LS);
        dwLevel += dReadDspCommDram (ADDR_D2RC_CH_VOLUME_RS);
        dwLevel += dReadDspCommDram (ADDR_D2RC_CH_VOLUME_C);
        dwLevel += dReadDspCommDram (ADDR_D2RC_CH_VOLUME_SUBWOOFER);
        /* check if doing DAC auto mute
           if(dwLevel < dwDspInfo)
           {
           if ((bSharedReg(SR_ADAC_ST) & SV_ADAC_MUTE) == 0) // if DAC is not muted
           {
           vSetAdac(ADAC_CMD_DAC_MUTE, 1, 0);
           }
           }
           else
           {
           if ((bSharedReg(SR_ADAC_ST) & SV_ADAC_MUTE) == SV_ADAC_MUTE) // if DAC is muted
           {
           //Open DAC, Mute Disable
           vSetAdac(ADAC_CMD_DAC_MUTE, 0, 0);
           }
           }
         */
    }
#endif // DSP_AUTO_MUTE
}
#endif // EMULATION_FPGA
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
    _fgDspWakeupOK = FALSE;

    // Wake up Dsp
    SendDSPInt (INT_RC2D_INTERRUPT_CTRL, RISC_WAKEUP_DSP);
    
    // has wake up dsp
    _fgDspWakeUp = TRUE;

    // FIXME !!!
    _fgDspWakeupOK = TRUE;
        
    // Light: add timeout of DSP wakeup
    while(!_fgDspWakeupOK) // Light: wait for DSP wakeup
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
    
    _u4DspVersion = DSP_RISC_CODE_VERSION;
    dwFlashVer = u4GetDspFlashVersion ();
    WriteDspCommDram (ADDR_RISC_CODE_VERSION, _u4DspVersion);
    WriteDspCommDram (ADDR_RISC_FLASH_VERSION, dwFlashVer);
       
    fgRet = TRUE;
    return fgRet;
}

void vDspSafeSendInt (UINT32 u4Addr, UINT32 u4Data)
{
    UINT32 dwCnt = 0;

    while (1)
    {
        SLEEP (0);
        if (!fgDspBusy ())
        {
            SendDSPInt (u4Addr, u4Data);
            break;
        }
        dwCnt++;
        if (dwCnt > 0x500)
        {
            SendDSPInt (u4Addr, u4Data);
            _dwDspSIntFail++;
            break;
        }
    }
}

/***************************************************************************
     Function : vInformBitRate
  Description :
    Parameter : None
    Return    : None
***************************************************************************/
#ifdef EMULATION_FPGA
void vInformBitRate (void)
{
#if 0
    UINT32 dwBitRate;
    UINT32 dwFreq_tmp;
    UINT16 wTmp;

    if (_bDspStrTyp == PCM_STREAM)  // non-PCM
    {
        wTmp = 16 * 44100 * 2 / 1000;   // 16 bits * 44.1Khz * 2ch in Kbps unit
    }
    {
        dwBitRate = dReadDsopCommDram (ADDR_D2RC_RISC_INFO_BIT_RATE);
        wTmp = dwBitRate / 1000;
    }
#endif
}
#endif // EMULATION_FPGA
