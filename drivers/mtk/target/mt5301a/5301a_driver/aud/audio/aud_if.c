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
 * $RCSfile: aud_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file aud_if.c
 *  Audio decoder control interface primitive API
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN

#include "x_typedef.h"
#include "x_os.h"
#include "x_bim.h"
#include "x_printf.h"
#include "x_assert.h"
#include "x_util.h"

#include "feeder_if.h"
#include "aud_debug.h"
#include "aud_if.h"
#include "aud_drv.h"
#include "aud_cfg.h"
#include "aud_notify.h"
#include "dsp_common.h"
#include "drv_adsp.h"
#include "drvcust_if.h"
#ifdef DSP_TEST_PSRDMA
#include "aud_hw.h"
#endif

#ifdef CHANNEL_CHANGE_LOG
#include "x_timer.h"
#endif
#include "aud_dsp_cfg.h"
#include "nand_if.h"
LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define ADRV_DUAL_DEC_WAIT_CMD_Q     "AUD_DUAL_DEC_WAIT_CMD_Q"
#define AUD_CMD_QUEUE_SIZE      32
#define AUD_DUAL_DEC_TIME_OUT   20000

#ifdef CC_MT5387
#define CFO_ACC_PROB_SRAM_ADDR 0x2DF6   ///[Joel]0x2AF6
#else
#define CFO_ACC_PROB_SRAM_ADDR 0x36F6
#endif

#define USE_16BIT_ALIGNMENT

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------
#if defined(CC_MT5360B) || defined(CC_MT5360)
extern void _MT536xInternal_Mute(UINT8 u1DacId,BOOL fgEnable);
#else
extern void _MT8297_Mute(UINT8 u1DacId,BOOL fgEnable);
#endif
extern AUD_AOUT_PAD_CFG_T _grAOutPadCfg;
extern UINT32 _gu4AudCtrlType;

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
//extern UINT32 u4ReadDspSram(UINT32 u4Addr);

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static HANDLE_T _hSema;
static HANDLE_T _hDualDecSema;
static HANDLE_T _hCmdWaitSema[AUD_DEC_NUM];
static HANDLE_T _hAsynCmdSema;
static HANDLE_T _hMwCtrlSema;
#ifndef CC_MT5391_AUD_3_DECODER
static HANDLE_T _hAudDualDecWaitCmdQueue;
#endif

#ifndef CC_MT5391_AUD_3_DECODER
static BOOL _fgDualDecBlockMode;
#endif
//static UINT32 _u4NotifyInfo = 0;

#ifndef CC_MT5391_AUD_3_DECODER
static BOOL _afgDecPause[2] = {FALSE, FALSE};
#else
static BOOL _afgDecPause[3] = {FALSE, FALSE, FALSE};
#endif

static CHAR * _paAudCmd[] =
{
    "AUD_CMD_PLAY",
    "AUD_CMD_STOP",
    "AUD_CMD_RESET",
    "AUD_CMD_PAUSE",
    "AUD_CMD_AVSYNC",
    "AUD_CMD_LOADCODE",
    "AUD_CMD_RESUME",
    "AUD_CMD_CHANGE_FORMAT",
    "AUD_CMD_STOPSTREAM"
};

static UINT8 _au1ChannelVolume[AUD_DEC_NUM][AUD_CH_NUM];
static BOOL  _fgClipChnVolChanged = FALSE;

#ifdef CC_MT5391_AUD_3_DECODER
static AUD_HP_COND_NFY_T _aeHPCond[AUD_DEC_NUM] = {AUD_HP_COND_HEADPHONE_COND_UNPLUGED, AUD_HP_COND_HEADPHONE_COND_UNPLUGED, AUD_HP_COND_HEADPHONE_COND_UNPLUGED};
static BOOL _bfgMMDecFmtConflict[AUD_DEC_NUM] = {FALSE,FALSE,FALSE};
static BOOL _bfgMMDecFmtScramble[AUD_DEC_NUM] = {FALSE,FALSE,FALSE};
#else
static AUD_HP_COND_NFY_T _aeHPCond[AUD_DEC_NUM] = {AUD_HP_COND_HEADPHONE_COND_UNPLUGED, AUD_HP_COND_HEADPHONE_COND_UNPLUGED};
static BOOL _bfgMMDecFmtConflict[AUD_DEC_NUM] = {FALSE,FALSE};
static BOOL _bfgMMDecFmtScramble[AUD_DEC_NUM] = {FALSE,FALSE};
#endif

#ifdef CC_AUD_SETUP_MELODY
static BOOL _fgSetupPlayFinish = TRUE;
#endif

//static PCM_INPUT_TYPE_T rPcmEndian = PCM_16_BITS_BIG_ENDIAN;
static BOOL _fgLpcmTpyePS = FALSE;
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static void _AudAsynCmdSemaRelease(UINT8 u1DecId, AUD_DEC_CMD_T eAudDecCmd)
{
#ifndef CC_MT5391_AUD_3_DECODER
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD));
#endif
    LOG(5, "Async AUD Command %s done\n", _paAudCmd[eAudDecCmd]);

    VERIFY(x_sema_unlock(_hAsynCmdSema) == OSR_OK);
}

static void _AudIfFixWarning(void)
{
    UNUSED(_paAudCmd);
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// AUD_CommandDone
//
/** Audio notify command check.
 *
 *  @param  ucDecId   		Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param  eAudDecCmd   	audio command.
 *  @param  u4Result   		audio status result.		
 *
 *  @retval void
 */
 void AUD_CommandDone(UINT8 u1DecId,  AUD_DEC_CMD_T eAudDecCmd, UINT32 u4Result)
{
    AudDecNfyFct   pfAudDecNfy = NULL;
    UINT8          u1DecoderId = u1DecId;
    
#ifndef CC_MT5391_AUD_3_DECODER
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD));
#endif    
    LOG(7, "AUD_Command %s done\n", _paAudCmd[eAudDecCmd]);

    // Get notify function
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy));
 
    if (pfAudDecNfy != NULL)
    {
        // Notify middleware 
        if ((eAudDecCmd == AUD_CMD_PLAY) || (eAudDecCmd == AUD_CMD_PAUSE) ||
            (eAudDecCmd == AUD_CMD_STOP) || (eAudDecCmd == AUD_CMD_RESET) ||
            (eAudDecCmd == AUD_CMD_RESUME))
        {
            pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecoderId, AUD_COND_CTRL_DONE, 
                        _gu4AudCtrlType, u4Result);
        }
        else if (eAudDecCmd == AUD_CMD_STOPSTREAM)
        {
            pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecoderId, AUD_COND_AUD_CLIP_DONE, 
                        _gu4AudCtrlType, u4Result);
        }
    }

    _AudAsynCmdSemaRelease(u1DecId, eAudDecCmd);
    VERIFY(x_sema_unlock(_hCmdWaitSema[u1DecId]) == OSR_OK);
}

//-----------------------------------------------------------------------------
// AUD_WaitCommandDone
//
/** Audio wait notify command check.
 *
 *  @param  ucDecId   		Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param  eAudDecCmd   	audio command.
 *
 *  @retval void
 */  
void AUD_WaitCommandDone(UINT8 u1DecId, AUD_DEC_CMD_T eAudDecCmd)
{ 
#ifndef CC_MT5391_AUD_3_DECODER
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD));
#endif
    LOG(7, "AUD_WaitCommand %s\n", _paAudCmd[eAudDecCmd]);
    VERIFY(x_sema_lock(_hCmdWaitSema[u1DecId], X_SEMA_OPTION_WAIT) == OSR_OK);
}

#ifndef CC_MT5391_AUD_3_DECODER  //gallen 0521
void AUD_DualDecWaitCommandDone(AUD_DUAL_DEC_CMD_T eDualDecCmd, BOOL fgTimeOut)
{
    UINT32 u4DualDecCmd;
    UINT16 u2MsgIdx;
    SIZE_T zMsgSize = sizeof(UINT32);
    INT32  i4Result;
   
    if (fgTimeOut)
    {
        i4Result = x_msg_q_receive_timeout(&u2MsgIdx, &u4DualDecCmd, &zMsgSize, 
    	                   &_hAudDualDecWaitCmdQueue, 1, AUD_DUAL_DEC_TIME_OUT);
        VERIFY((i4Result == OSR_OK) || (i4Result == OSR_TIMEOUT));
        if (i4Result == OSR_TIMEOUT)
        {
            // Error handling... TODO
            ASSERT(0);
        }
    }
    else // wait until get new event
    {
        VERIFY(x_msg_q_receive(&u2MsgIdx, &u4DualDecCmd, &zMsgSize, 
                           &_hAudDualDecWaitCmdQueue, 1, X_MSGQ_OPTION_WAIT) == OSR_OK);
    }

    if (eDualDecCmd != (AUD_DUAL_DEC_CMD_T)u4DualDecCmd)
    {
        // Error handling... TODO
        ASSERT(0);    
    }
}

void AUD_DualDecCommandDone(AUD_DUAL_DEC_CMD_T eAudDecCmd, UINT32 u4Result)
{
    UINT32 u4Msg = (UINT32)eAudDecCmd;
    if (_fgDualDecBlockMode)
    {
        VERIFY((x_msg_q_send(_hAudDualDecWaitCmdQueue, &u4Msg, sizeof(UINT32), 1)) == OSR_OK);
    }    
    UNUSED(u4Result);
}
#endif
//-----------------------------------------------------------------------------
// AUD_Init
//
/** Aud driver initialization. This function will create driver thread and let 
 *  DSP go on.
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
INT32 AUD_Init(void)
{
    static BOOL _fgInited = FALSE;
    UINT32 u4Index = 0;

    if (!_fgInited)
    {
        VERIFY (x_sema_create(&_hSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
#ifndef CC_MT5391_AUD_3_DECODER  //gallen 0521        
        VERIFY (x_sema_create(&_hDualDecSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
#endif
        VERIFY (x_sema_create(&(_hCmdWaitSema[0]), X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY (x_sema_create(&(_hCmdWaitSema[1]), X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
#ifdef CC_MT5391_AUD_3_DECODER
        VERIFY (x_sema_create(&(_hCmdWaitSema[2]), X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
#endif
        VERIFY (x_sema_create(&_hAsynCmdSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        VERIFY (x_sema_create(&_hMwCtrlSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        
#ifndef CC_MT5391_AUD_3_DECODER  //gallen 0521
        VERIFY((x_msg_q_create(&_hAudDualDecWaitCmdQueue, ADRV_DUAL_DEC_WAIT_CMD_Q, sizeof(UINT32), AUD_CMD_QUEUE_SIZE)) == OSR_OK);
#endif        
        _fgInited = TRUE;


#ifdef DSP_TEST_PSRDMA
    #ifndef	CC_MT5391_AUD_SUPPORT
        PARSER_WRITE32(0x1f0, 0);
        PARSER_WRITE32(0x1f0, 0x23);
    #else    
        PARSER_2_WRITE32(0xb0, 0);          // Reset parser
        PARSER_2_WRITE32(0xb0, 0x3);        // Enable parser
    #endif        
#endif
		AUD_HwInit();

#ifdef CC_AUD_USE_NVM
        AUD_NVM_Read();
#endif

#ifdef CC_AUD_USE_FLASH_AQ
        AUD_FlashAQ_Init();
#endif
        AUD_DRVInit();
        AUD_DspEqCfgInit();
#ifdef CC_AUD_SOUND_MODE
        AUD_DspPSMCfgInit();
        AUD_DspVSMCfgInit();
#endif
        AUD_DspChDelayInit();
        AUD_DspSpdifInit();
        AUD_DspVolumeInit(AUD_DEC_MAIN);
        AUD_DspVolumeInit(AUD_DEC_AUX);
#ifdef CC_MT5391_AUD_3_DECODER
        AUD_DspVolumeInit(AUD_DEC_THIRD);
#endif
        _AudIfFixWarning();
            
        //FIXME //busy waiting until init finished
        while(AUD_DRVGetAudioState(AUD_DEC_MAIN) != AUD_IDLE)  
        {   
            u4Index++;
            x_thread_delay(10);
       
            if(u4Index > 500)
            { 
                LOG(1, "DSP NOT POWER ON\n");
                ASSERT(0);
            //  return AUD_FAIL;
            }
        }
    }

    return AUD_OK;
}

//-----------------------------------------------------------------------------
// AUD_GetDecType
//
/** Get audio decoder type.
 *
 *  @param      u1DecId        Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      peStreamFrom   Stream source
 *  @param      prDecType      Stream format
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
INT32 AUD_GetDecType(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T * peStreamFrom, AUD_FMT_T * prDecType)
{
#ifndef CC_MT5391_AUD_3_DECODER
//#if 1
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD));
#endif
    if ((peStreamFrom != NULL) && (prDecType != NULL))
    {
        *peStreamFrom = AUD_STREAM_FROM_DIGITAL_TUNER;  //FIXME
        *prDecType    = AUD_FMT_AC3;    //FIXME 
    }    
    
    return AUD_OK;
}

//-----------------------------------------------------------------------------
// AUD_SetDecMemBuffer
//
/** Brief of AUD_SetDecMemBuffer.
 *  Details of AUD_SetDecMemBuffer (optional).
 *
 *  @param      u1DecId           Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      prDecMemBuffer    Memory buffer
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
INT32 AUD_SetDecMemBuffer(UINT8 u1DecId, const MEM_BUFFER_INFO_T * prDecMemBuffer)
{
#ifndef CC_MT5391_AUD_3_DECODER
//#if 1
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD));
#endif
    if (prDecMemBuffer != NULL)
    {
        VERIFY(AUD_DRVSetDecMemBuffer(u1DecId, prDecMemBuffer));
    }    
    
    return AUD_OK;
}

//-----------------------------------------------------------------------------
// AUD_SetDecType
//
/** Set audio decoder type.
 *
 *  @param      u1DecId        Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      eStreamFrom    Stream source
 *  @param      eDecFmt        Stream format
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
INT32 AUD_SetDecType(UINT8 u1DecId,  AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eDecFmt)
{
#ifndef CC_MT5391_AUD_3_DECODER
//#if 1
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD));
#endif
    VERIFY(x_sema_lock(_hSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    VERIFY(x_sema_lock(_hAsynCmdSema, X_SEMA_OPTION_WAIT) == OSR_OK);

#ifndef __MODEL_slt__
    #ifdef CC_MT5391
    //ADC Power down/on
    if ((eStreamFrom != AUD_STREAM_FROM_LINE_IN) && (u1DecId==AUD_DEC_MAIN))
    {
        _MT8297_ADC_PowerDown(TRUE);  
    }
    else
    {
        _MT8297_ADC_PowerDown(FALSE);  
    }
    #elif defined (CC_DYNAMIC_POWER_ONOFF)
    //ADC Power down/on
    if ((eStreamFrom != AUD_STREAM_FROM_LINE_IN) && (u1DecId==AUD_DEC_MAIN))
    {
        _MT536xInternalAdcPowerDown(TRUE);  
    }
    else
    {
        _MT536xInternalAdcPowerDown(FALSE);  
    }    
    #endif
#endif    

#if 1 //Use LPCM decoder if enable this code.
    if (eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
    {
        if (eDecFmt==AUD_FMT_PCM)
        {
            eDecFmt=AUD_FMT_LPCM;
        }
    }
#endif    

#ifdef USE_16BIT_ALIGNMENT
    if ((eStreamFrom == AUD_STREAM_FROM_LINE_IN) ||
        (eStreamFrom == AUD_STREAM_FROM_HDMI))
    {
        AUD_PCM_SETTING_T  ePcmSetting;

        ePcmSetting.eSampleFreq = FS_48K;
        ePcmSetting.ePcmDataInfo = PCM_16_BITS_BIG_ENDIAN;
            
        AUD_PcmSetting(u1DecId, &ePcmSetting);
    }
#endif

    if (!AUD_IsDecoderPlay(u1DecId))
    {
        INT32 i4Ret;
        // Try lock to make sure the semaphore was locked
        i4Ret = x_sema_lock(_hCmdWaitSema[u1DecId], X_SEMA_OPTION_NOWAIT);    
        VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
    
    	VERIFY(AUD_DRVSetDecodeType(u1DecId, eStreamFrom, eDecFmt));
    	AUD_WaitCommandDone(u1DecId, AUD_CMD_LOADCODE);
    }
    else
    {
		UNUSED(AUD_DRVChangeDecodeFormat(u1DecId, eStreamFrom, eDecFmt));
    }
   
    VERIFY(x_sema_unlock(_hSema) == OSR_OK);
    
    return AUD_OK;
}   

//-----------------------------------------------------------------------------
// AUD_DSPCmdPlay
//
/** Brief of AUD_DSPCmdPlay.
 *  Issue play command to audio decoder, note that this is a synchronous play
 *  function
 *
 *  @param      u1DecId        Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
INT32 AUD_DSPCmdPlay(UINT8 u1DecId)
{
    INT32 i4Ret;
#ifndef CC_MT5391_AUD_3_DECODER
//#if 1
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD));
#endif
    VERIFY(x_sema_lock(_hSema, X_SEMA_OPTION_WAIT) == OSR_OK);

#ifdef CC_MT5391_AUD_3_DECODER  //gallen 0713        
        if (u1DecId==AUD_DEC_THIRD) {
            AUD_DspADEnable(AUD_DEC_MAIN, TRUE);
        }    
#endif              

    if (_afgDecPause[u1DecId])
    {
        AUD_CommandDone(u1DecId, AUD_CMD_PLAY, AUD_OK);
    }
    else
    {
        // Try lock to make sure the semaphore was locked
        i4Ret = x_sema_lock(_hCmdWaitSema[u1DecId], X_SEMA_OPTION_NOWAIT);    
        VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
        VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_PLAY));
    }
    _afgDecPause[u1DecId] = FALSE;
    AUD_WaitCommandDone(u1DecId, AUD_CMD_PLAY);
    VERIFY(x_sema_unlock(_hSema) == OSR_OK);
    
    return AUD_OK;
}

//-----------------------------------------------------------------------------
// AUD_DSPCmdPlayAsyn
//
/** Brief of AUD_DSPCmdPlayAsyn.
 *  Issue play command to audio decoder, note that this is a asynchronous play
 *  function. There will be a audio decoder notification when the command was done
 *
 *  @param      u1DecId        Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
INT32 AUD_DSPCmdPlayAsyn(UINT8 u1DecId)
{
#ifndef CC_MT5391_AUD_3_DECODER
//#if 1
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD));
#endif

    #ifdef CHANNEL_CHANGE_LOG
    HAL_TIME_T dt;
    HAL_GetTime(&dt);
    LOG(0, " %u.%06u s [AV SYNC] 3 MW aud play\n", dt.u4Seconds, dt.u4Micros);
    #endif
    
    VERIFY(x_sema_lock(_hSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    VERIFY(x_sema_lock(_hAsynCmdSema, X_SEMA_OPTION_WAIT) == OSR_OK);
#ifdef CC_MT5391_AUD_3_DECODER  //gallen 0521        
        if (u1DecId==AUD_DEC_THIRD) {
            AUD_DspADEnable(AUD_DEC_MAIN, TRUE);
        }    
#endif          
    if (_afgDecPause[u1DecId])
    {
        AUD_CommandDone(u1DecId, AUD_CMD_PLAY, AUD_OK);
    }
    else
    {
        VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_PLAY));
    }
    _afgDecPause[u1DecId] = FALSE;
    VERIFY(x_sema_unlock(_hSema) == OSR_OK);
    return AUD_OK;
}

//-----------------------------------------------------------------------------
// AUD_DSPCmdResume
//
/** Brief of AUD_DSPCmdResume.
 *  Details of AUD_DSPCmdResume (optional).
 *
 *  @param        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)    
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
INT32 AUD_DSPCmdResume(UINT8 u1DecId)
{
#ifndef CC_MT5391_AUD_3_DECODER
//#if 1
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD));
#endif

    VERIFY(x_sema_lock(_hSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    if (AUD_IsDecoderPlay(u1DecId))
    {
        _afgDecPause[u1DecId] = FALSE;
        VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_RESUME));
    }
    else
    {
#ifndef CC_MT5391_AUD_3_DECODER  
        if(AUD_GetTargetCountry() != COUNTRY_EU){
            VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_PLAY));
        }else  {
            VERIFY(AUD_DualDecDrvCmd(AUD_DUAL_DEC_CMD_PLAY));    
        }
#else
        VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_PLAY));
#endif
    }
    VERIFY(x_sema_unlock(_hSema) == OSR_OK);
    
    return AUD_OK;
}

//-----------------------------------------------------------------------------
// AUD_DSPCmdPause
//
/** Brief of AUD_DSPCmdPause.
 *  Details of AUD_DSPCmdPause (optional).
 *
 *  @param        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)    
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
INT32 AUD_DSPCmdPause(UINT8 u1DecId)
{
#ifndef CC_MT5391_AUD_3_DECODER
//#if 1
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD));
#endif
    VERIFY(x_sema_lock(_hSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    if (AUD_IsDecoderPlay(u1DecId))
    {
        _afgDecPause[u1DecId] = TRUE;
    }
    VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_PAUSE));

    VERIFY(x_sema_unlock(_hSema) == OSR_OK);
    
    return AUD_OK;
}

//-----------------------------------------------------------------------------
// AUD_DSPCmdPauseAsyn
//
/** Brief of AUD_DSPCmdPauseAsyn.
 *  Details of AUD_DSPCmdPauseAsyn (optional).
 *
 *  @param[in]        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)    
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
INT32 AUD_DSPCmdPauseAsyn(UINT8 u1DecId)
{
#ifndef CC_MT5391_AUD_3_DECODER
//#if 1
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD));
#endif
    VERIFY(x_sema_lock(_hSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    VERIFY(x_sema_lock(_hAsynCmdSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    _afgDecPause[u1DecId] = TRUE;
    AUD_CommandDone(u1DecId, AUD_CMD_PAUSE, AUD_OK);
    //VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_STOP));
    VERIFY(x_sema_unlock(_hSema) == OSR_OK);
    
    return AUD_OK;
}

//-----------------------------------------------------------------------------
// AUD_DSPCmdStop
//
/** Brief of AUD_DSPCmdStop.
 *  Details of AUD_DSPCmdStop (optional).
 *
 *  @param        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)    
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
INT32 AUD_DSPCmdStop(UINT8 u1DecId)
{
    INT32 i4Ret;
#ifndef CC_MT5391_AUD_3_DECODER
//#if 1
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD));
#endif
    VERIFY(x_sema_lock(_hSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    // Try lock to make sure the semaphore was locked
    i4Ret = x_sema_lock(_hCmdWaitSema[u1DecId], X_SEMA_OPTION_NOWAIT);    
    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));

    VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_STOP));
    AUD_WaitCommandDone(u1DecId, AUD_CMD_STOP);
#ifdef CC_MT5391_AUD_3_DECODER  //gallen 0713        
        if (u1DecId==AUD_DEC_THIRD) {
            AUD_DspADEnable(AUD_DEC_MAIN, FALSE);
        }            
#endif                
    VERIFY(x_sema_unlock(_hSema) == OSR_OK);
    _afgDecPause[u1DecId] =  FALSE;

    return AUD_OK;
}

//-----------------------------------------------------------------------------
// AUD_DSPCmdStopAsyn
//
/** Brief of AUD_DSPCmdStopAsyn.
 *  Details of AUD_DSPCmdStopAsyn (optional).
 *
 *  @param        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)    
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
INT32 AUD_DSPCmdStopAsyn(UINT8 u1DecId)
{
#ifndef CC_MT5391_AUD_3_DECODER
//#if 1
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD));
#endif
    VERIFY(x_sema_lock(_hSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    VERIFY(x_sema_lock(_hAsynCmdSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_STOP));
#ifdef CC_MT5391_AUD_3_DECODER  //gallen 0521        
        if (u1DecId==AUD_DEC_THIRD) {
            AUD_DspADEnable(AUD_DEC_MAIN, FALSE);
        }            
#endif            
    VERIFY(x_sema_unlock(_hSema) == OSR_OK);
    _afgDecPause[u1DecId] =  FALSE;
    
    return AUD_OK;
}

//-----------------------------------------------------------------------------
// AUD_DSPCmdReset
//
/** Brief of AUD_DSPCmdReset.
 *  Details of AUD_DSPCmdReset (optional).
 *
 *  @param        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)    
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
INT32 AUD_DSPCmdReset(UINT8 u1DecId)
{
    INT32 i4Ret = 0;
#ifndef CC_MT5391_AUD_3_DECODER
//#if 1
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD));
#endif
    VERIFY(x_sema_lock(_hSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    // Try lock to make sure the semaphore was locked
    i4Ret = x_sema_lock(_hCmdWaitSema[u1DecId], X_SEMA_OPTION_NOWAIT);
    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
    VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_STOP));
    AUD_WaitCommandDone(u1DecId, AUD_CMD_STOP);
    VERIFY(x_sema_unlock(_hSema) == OSR_OK);    
    return AUD_OK;
}

//-----------------------------------------------------------------------------
// AUD_DSPCmdResetAsyn
//
/** Brief of AUD_DSPCmdResetAsyn.
 *  Details of AUD_DSPCmdResetAsyn (optional).
 *
 *  @param        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)    
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
INT32 AUD_DSPCmdResetAsyn(UINT8 u1DecId)
{
#ifndef CC_MT5391_AUD_3_DECODER
//#if 1
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD));
#endif
    VERIFY(x_sema_lock(_hSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    VERIFY(x_sema_lock(_hAsynCmdSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_STOP));
    VERIFY(x_sema_unlock(_hSema) == OSR_OK);    
    
    return AUD_OK;
}

//-----------------------------------------------------------------------------
// AUD_DSPCmdDualDecPlay
//
/** Brief of AUD_DSPCmdDualDecPlay.
 *  ONLY used in EURO mode
 *
 *  @param        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)    
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
#if 0
INT32 AUD_DSPCmdDualDecPlay(BOOL fgBlock)
{
    VERIFY(x_sema_lock(_hDualDecSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    _fgDualDecBlockMode = fgBlock;
    VERIFY(AUD_DualDecDrvCmd(AUD_DUAL_DEC_CMD_PLAY));
    if (fgBlock)
    {
        AUD_DualDecWaitCommandDone(AUD_DUAL_DEC_CMD_PLAY, FALSE);
    }
    else
    {
        // Do nothing ...
    }
    
    VERIFY(x_sema_unlock(_hDualDecSema) == OSR_OK);    
    return AUD_OK;    
}
#else
INT32 AUD_DSPCmdDualDecPlay(UINT8 u1DecId)  
{
    VERIFY(x_sema_lock(_hDualDecSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    if (u1DecId == AUD_DEC_MAIN)
    {
#ifndef CC_MT5391_AUD_3_DECODER
        _fgDualDecBlockMode = FALSE;
#endif
        VERIFY(AUD_DualDecDrvCmd(AUD_DUAL_DEC_CMD_PLAY));    
    }
#ifndef CC_MT5391_AUD_3_DECODER
//#if 1
    else //Audio Description
#else
    else if (u1DecId == AUD_DEC_THIRD)//Audio Description
#endif
    {
        // The only situation which will play decoder1 is audio description
#ifndef CC_MT5391_AUD_3_DECODER
        _fgDualDecBlockMode = FALSE;
#endif
        AUD_DRV_PlayDualDmx(TRUE);
        AUD_DspADEnable(AUD_DEC_MAIN, TRUE);
       //_AUD_DspProcMode(AUD_DEC_AUX, 0x9000);
    }
    
    VERIFY(x_sema_unlock(_hDualDecSema) == OSR_OK);    
    return AUD_OK;    
}
#endif //if 0

//-----------------------------------------------------------------------------
// AUD_DSPCmdDualDecStop
//
/** Brief of AUD_DSPCmdDualDecStop.
 *  ONLY used in EURO mode
 *
 *  @param        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)    
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
#if 0
INT32 AUD_DSPCmdDualDecStop(BOOL fgBlock)
{
    VERIFY(x_sema_lock(_hDualDecSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    
    _fgDualDecBlockMode = fgBlock;
    VERIFY(AUD_DualDecDrvCmd(AUD_DUAL_DEC_CMD_STOP));
    if (fgBlock)
    {
        AUD_DualDecWaitCommandDone(AUD_DUAL_DEC_CMD_STOP, FALSE);
    }
    else
    {
        // Do nothing ...
    }
    VERIFY(x_sema_unlock(_hDualDecSema) == OSR_OK);    
    return AUD_OK;
}

#else
INT32 AUD_DSPCmdDualDecStop(UINT8 u1DecId)  
{
    VERIFY(x_sema_lock(_hDualDecSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    
    if (u1DecId == AUD_DEC_MAIN)
    {
#ifndef CC_MT5391_AUD_3_DECODER    
        _fgDualDecBlockMode = FALSE;
#endif
        VERIFY(AUD_DualDecDrvCmd(AUD_DUAL_DEC_CMD_STOP));    
    }
#ifndef CC_MT5391_AUD_3_DECODER
//#if 1
    else //Audio Description
#else
    else if (u1DecId == AUD_DEC_THIRD)//Audio Description
#endif
    {
		// The only situation which will stop decoder1 is audio description
#ifndef CC_MT5391_AUD_3_DECODER    
        _fgDualDecBlockMode = FALSE;
#endif
        //_AUD_DspProcMode(AUD_DEC_AUX, 0xB000);
        AUD_DspADEnable(AUD_DEC_MAIN, FALSE);           
        AUD_DRV_PlayDualDmx(FALSE);       
    }
    VERIFY(x_sema_unlock(_hDualDecSema) == OSR_OK);    
    return AUD_OK;
}
#endif

//-----------------------------------------------------------------------------
// AUD_DSPCmdDualDecSetDecType
//
/** Brief of AUD_DSPCmdDualDecSetDecType.
 *  ONLY used in EURO mode
 *
 *  @param        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)    
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
INT32 AUD_DSPCmdDualDecSetDecType(AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eDecFmt)
{
    VERIFY(x_sema_lock(_hDualDecSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    UNUSED(AUD_DualDecSetDecType(eStreamFrom, eDecFmt));
    VERIFY(x_sema_unlock(_hDualDecSema) == OSR_OK);    
    return AUD_OK;
}


//-----------------------------------------------------------------------------
// AUD_RegNotifyFunc
//
/** Register audio notify function.
 *
 *  @param     pu4Handler   Notify function handler
 *  @param     pfNfyFunc    Notify function pointer
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
BOOL AUD_RegNotifyFunc(UINT32 * pu4Handler, AudDecNfyFct pfNfyFunc)
{
    return _AUD_RegNotifyFunc(pu4Handler, pfNfyFunc);
}

//-----------------------------------------------------------------------------
// AUD_DeRegNotifyFunc
//
/** Deregister audio notify function.
 *
 *  @param  u4Handler   Handler
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
BOOL AUD_DeRegNotifyFunc(UINT32 u4Handler)
{
    return _AUD_DeRegNotifyFunc(u4Handler);
}

//-----------------------------------------------------------------------------
// AUD_IsDecoderPlay
//
/** Brief of AUD_IsDecoderPlay.
 *  Details of AUD_IsDecoderPlay (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)   
 *
 *  @retval     TRUE	        
 *  @retval     FALSE	        
 */
//-----------------------------------------------------------------------------
BOOL AUD_IsDecoderPlay(UINT8 u1DecId)
{
    AUD_DRV_STATE_T eDrvState;
    eDrvState = AUD_DRVGetAudioState(u1DecId);
    return ((eDrvState == AUD_IDLE) || (eDrvState == AUD_UNINITIALIZED)) ? FALSE : TRUE;
}

//-----------------------------------------------------------------------------
// AUD_IsDecoderPlay
//
/** Brief of AUD_IsDecoderRealPlay.
 *  Details of AUD_IsDecoderRealPlay (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)   
 *
 *  @retval     TRUE	        
 *  @retval     FALSE	        
 */
//-----------------------------------------------------------------------------
BOOL AUD_IsDecoderRealPlay(UINT8 u1DecId)
{
    AUD_DRV_STATE_T eDrvState;
    eDrvState = AUD_DRVGetAudioState(u1DecId);
    return (eDrvState == AUD_ON_PLAY) ? TRUE : FALSE;
}

//-----------------------------------------------------------------------------
// AUD_GetDecodeType
//
/** Brief of AUD_GetDecodeType.
 *  Details of AUD_GetDecodeType (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      peStreamFrom     Stream source
 *  @param      peDecType        Stream format    
 */
//-----------------------------------------------------------------------------
void AUD_GetDecodeType(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T * peStreamFrom, AUD_FMT_T * peDecType)
{
    UNUSED(AUD_DRVGetDecodeType(u1DecId, peStreamFrom,  peDecType));
}

//-----------------------------------------------------------------------------
// AUD_SetAudClip
//
/** Brief of AUD_SetAudClip.
 *  Details of AUD_SetAudClip (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      peAudClip        
 */
//-----------------------------------------------------------------------------
void AUD_SetAudClip(UINT8 u1DecId, AUD_CLIP_TYPE_T * peAudClip)
{
    if (peAudClip)
    {
        UINT8 u1Sel, i;
        UINT8 u4OutChannel = 0;

        if (peAudClip->u4ToneFreq <= 200)
        {
            u1Sel = TEST_TONE_200_0dB;
        }
        else if (peAudClip->u4ToneFreq == 400)
        {
            u1Sel = TEST_TONE_400_0dB;
        }
        else if (peAudClip->u4ToneFreq == (3 * 1000))
        {
            u1Sel = TEST_TONE_1K3K_0dB;
        }
        else if (peAudClip->u4ToneFreq <= (1 * 1000))
        {
            u1Sel = TEST_TONE_1K_0dB;
        }
        else
        {
            u1Sel = TEST_TONE_20K_0dB;
        }
        
        if (peAudClip->fgPlayTestTone)
        {
            AUD_PCM_SETTING_T rPcmSetting;

            // Notes that tone table is 48K, 24-bits format
            rPcmSetting.eSampleFreq = FS_48K;
            rPcmSetting.ePcmDataInfo = PCM_24_BITS;
        
            peAudClip->eAudFmt = AUD_FMT_PCM;
            AUD_StrSelect(u1DecId, u1Sel, &peAudClip->eMemBuf);	
            AUD_DrvPcmSetting(u1DecId, &rPcmSetting);
        }
        
        for (i = (UINT8)AUD_CH_FRONT_LEFT; i <= (UINT8)AUD_CH_SUB_WOOFER; i++)
        {
            if (MAKE_BIT_MASK(i) & peAudClip->u4OutputChlSelect)
            {
                u4OutChannel = i;
                break;
            }
        }

        switch(u4OutChannel / 2)
        {
        case 0:  // L/R channel
            AUD_AoutPADMux(AUD_CH_FRONT_LEFT, AUD_CH_AUX_FRONT_LEFT,
                           AUD_CH_AUX_FRONT_LEFT, _grAOutPadCfg.ePadSdata3,
                           _grAOutPadCfg.ePadSdata4, _grAOutPadCfg.ePadSdata5,
                           0);
            break;
        case 1: // LS/RS channel
            AUD_AoutPADMux(AUD_CH_AUX_FRONT_LEFT, AUD_CH_FRONT_LEFT,
                           AUD_CH_AUX_FRONT_LEFT, _grAOutPadCfg.ePadSdata3,
                           _grAOutPadCfg.ePadSdata4, _grAOutPadCfg.ePadSdata5,
                           0);
            break;
        case 2: // C/Subwoofer channel
            AUD_AoutPADMux(AUD_CH_AUX_FRONT_LEFT, AUD_CH_AUX_FRONT_LEFT,
                           AUD_CH_FRONT_LEFT, _grAOutPadCfg.ePadSdata3,
                           _grAOutPadCfg.ePadSdata4, _grAOutPadCfg.ePadSdata5,
                           0);
            break;
        default:
            break;
        }
      
        peAudClip->eMemBuf.u4LoopCount = peAudClip->u4RepeatCount;
        UNUSED(AUD_SetDecType(u1DecId, AUD_STREAM_FROM_MEMORY, peAudClip->eAudFmt));
        UNUSED(AUD_SetDecMemBuffer(u1DecId, &peAudClip->eMemBuf));
    }
}


//-----------------------------------------------------------------------------
// AUD_ClipSetNotifyFunc
//
/** Brief of AUD_ClipSetNotifyFunc.
 *  Details of AUD_ClipSetNotifyFunc (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      pfClipNotify        
 */
//-----------------------------------------------------------------------------
void AUD_ClipSetNotifyFunc(UINT8 u1DecId, AUD_CLIP_NOTIFY pfClipNotify)
{
    AUD_DrvSetNotifyFunc(u1DecId, pfClipNotify);
}


//-----------------------------------------------------------------------------
// AUD_PlayAudClip
//
/** Brief of AUD_PlayAudClip.
 *  Details of AUD_PlayAudClip (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      u4Channel        
 *  @param      fgPlayTestTone   
 */
//-----------------------------------------------------------------------------
void AUD_PlayAudClip(UINT8 u1DecId, UINT32 u4Channel, BOOL fgPlayTestTone)
{
    UINT8 i;
    UINT8 u4ClipChlVolumn[AUD_CH_NUM];
    
    x_memset(u4ClipChlVolumn, 0, sizeof(UINT8) * (UINT8)AUD_CH_NUM);
    
    for (i = (UINT8)AUD_CH_FRONT_LEFT; i <= (UINT8)AUD_CH_SUB_WOOFER; i++)
    {
        // Get the original channel volumn
        _au1ChannelVolume[u1DecId][i] = 
            AUD_DspGetChannelVolume(u1DecId, (AUD_CH_T)i);
        
        // Set the new channel volumn
        // Only the select channel keeps the original volumn,
        // Others will set to be zero
        if (MAKE_BIT_MASK(i) & u4Channel)
        {
            if (fgPlayTestTone)
            {
                // If play the test tone, 
                // Output pad LS/RS, C/Subwoofer will set as L/R
                u4ClipChlVolumn[i % (UINT8)AUD_CH_REAR_LEFT] = 
                    _au1ChannelVolume[u1DecId][i];
            }
            else
            {
                u4ClipChlVolumn[i] = _au1ChannelVolume[u1DecId][i];
            }
        }
    }
    
    // Set the new channel volumn
    for (i = (UINT8)AUD_CH_FRONT_LEFT; i <= (UINT8)AUD_CH_SUB_WOOFER; i++)
    {
        AUD_DspChannelVolume(u1DecId, (AUD_CH_T)i, u4ClipChlVolumn[i]);
    }
    _fgClipChnVolChanged = TRUE;
    
    UNUSED(AUD_DSPCmdPlay(u1DecId));
}

//-----------------------------------------------------------------------------
// AUD_StopAudClip
//
/** Brief of AUD_StopAudClip.
 *  Details of AUD_StopAudClip (optional).
 *
 *  @param      u1DecId        Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 */
//-----------------------------------------------------------------------------
void AUD_StopAudClip(UINT8 u1DecId)
{
    UINT8 i;
    
    AUD_StopMemStr();
    UNUSED(AUD_DSPCmdStop(u1DecId));
    
    AUD_AoutPADMux(_grAOutPadCfg.ePadSdata0, _grAOutPadCfg.ePadSdata1,
                   _grAOutPadCfg.ePadSdata2, _grAOutPadCfg.ePadSdata3,
                   _grAOutPadCfg.ePadSdata4, _grAOutPadCfg.ePadSdata5, 0);
                   
    // Restore the channel volumn setting before play aud clip
    if (_fgClipChnVolChanged)
    {
        for (i = (UINT8)AUD_CH_FRONT_LEFT; i <= (UINT8)AUD_CH_SUB_WOOFER; i++)
        {
            AUD_DspChannelVolume(u1DecId, (AUD_CH_T)i, _au1ChannelVolume[u1DecId][i]);
        }
    }
    _fgClipChnVolChanged = FALSE; //Andrew Wen 2007/9/10 Sometime AP call StopClip before PlayClip 
}

//-----------------------------------------------------------------------------
// Aud_SetAudClipVolume
//
/** Brief of Aud_SetAudClipVolume.
 *  Details of Aud_SetAudClipVolume (optional).
 *
 *  @param      u1DecId        Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *              u1Volume       Clip Vol  0 ~ 30 mapping to -20 ~ +10dB
 */
//-----------------------------------------------------------------------------
void Aud_SetAudClipVolume(UINT8 u1DecId, UINT32 u4Channel, UINT8 u1Volume)
{
    INT32 i4Vol = 0;
    
    UNUSED(u4Channel);
    UNUSED(u1DecId);

    //0 ~ 30 mapping to -20 ~ +10 dB
    i4Vol = (u1Volume - 20) * 2; 

    AUD_DspChannelVolGain(u1DecId, (AUD_CH_T)u4Channel, i4Vol);
}

//-----------------------------------------------------------------------------
// AUD_AvInputMuxSel
//
/** Brief of AUD_AvInputMuxSel.
 *  Details of AUD_AvInputMuxSel (optional).
 *
 *  @param  eInputId   AV component id
 *
 */
//-----------------------------------------------------------------------------
void AUD_AvInputMuxSel(AUD_INPUT_ID_T eInputId)
{
    AUD_AvMuxSel(eInputId);
}

//-----------------------------------------------------------------------------
// AUD_SetCtrlType
//
/** Brief of AUD_SetCtrlType.
 *  Details of AUD_SetCtrlType (optional).
 *
 *  @param  u4MWSetCtrlType 
 *
 */
//-----------------------------------------------------------------------------
void AUD_SetCtrlType(UINT32 u4MWSetCtrlType)
{
    _gu4AudCtrlType = u4MWSetCtrlType;
}

#ifndef CC_MT5391_AUD_3_DECODER  //gallen 0821
//-----------------------------------------------------------------------------
// AUD_Mute
//
/** Brief of AUD_Mute.
 *  Details of AUD_Mute (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)    
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
INT32 AUD_Mute(UINT8 u1DecId)
{
    BOOL fMuteAllSource;
    
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
    AUD_DspMuteEnable(u1DecId, TRUE);
    
    fMuteAllSource = (BOOL)DRVCUST_OptGet(eAudioMuteAllSource);
    if (fMuteAllSource)
    {
        ADAC_Mute(TRUE);
    }
    return AUD_OK;
}

//-----------------------------------------------------------------------------
// AUD_UnMute
//
/** Brief of AUD_UnMute.
 *  Details of AUD_UnMute (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)    
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
INT32 AUD_UnMute(UINT8 u1DecId)
{
    BOOL fMuteAllSource;
    
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
    AUD_DspMuteEnable(u1DecId, FALSE);

    fMuteAllSource = (BOOL)DRVCUST_OptGet(eAudioMuteAllSource);
    if (fMuteAllSource)
    {    
    	ADAC_Mute(FALSE);
    }
    return AUD_OK;
}
#else
//-----------------------------------------------------------------------------
// AUD_Mute
//
/** Brief of AUD_Mute.
 *  Details of AUD_Mute (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)    
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
INT32 AUD_Mute(UINT8 u1DecId)
{
    BOOL fMuteAllSource;
    
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX)||(u1DecId == AUD_DEC_THIRD));
    AUD_DspMuteEnable(u1DecId, TRUE);

    if ((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX)) 
    {
        fMuteAllSource = (BOOL)DRVCUST_OptGet(eAudioMuteAllSource);
        if (fMuteAllSource)
        {
            ADAC_Mute(TRUE);
        }
    }    
    return AUD_OK;
}

//-----------------------------------------------------------------------------
// AUD_UnMute
//
/** Brief of AUD_UnMute.
 *  Details of AUD_UnMute (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)    
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
INT32 AUD_UnMute(UINT8 u1DecId)
{
    BOOL fMuteAllSource;
    
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX)||(u1DecId == AUD_DEC_THIRD));
    AUD_DspMuteEnable(u1DecId, FALSE);

    if ((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX)) 
    {
        fMuteAllSource = (BOOL)DRVCUST_OptGet(eAudioMuteAllSource);
        if (fMuteAllSource)
        {    
    	    ADAC_Mute(FALSE);
        }
    }    
    return AUD_OK;
}

//-----------------------------------------------------------------------------
// AUD_MuteByPassCh()
//
/** Brief of AUD_MuteByPassCh().
 *  Details of AUD_MuteByPassCh ().
 *  
  *  @param      fgMute          1: mute, o:un-mute  
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
INT32 AUD_MuteByPassCh(BOOL fgMute)
{
#if defined(CC_MT5360B) || defined(CC_MT5360)

    if(fgMute)
    {
        _MT536xInternal_Mute(INTERNAL_DAC_ID_THIRD, 1);
    }
    else
    {
        _MT536xInternal_Mute(INTERNAL_DAC_ID_THIRD, 0);
    }

#else

    if(fgMute)
    {
        _MT8297_Mute(INTERNAL_DAC_ID_THIRD, 1);
    }
    else
    {
        _MT8297_Mute(INTERNAL_DAC_ID_THIRD, 0);
    }

#endif
    return AUD_OK;
}

#endif
//-----------------------------------------------------------------------------
// AUD_BagtNfyFct
//
/** Brief of AUD_BagtNfyFct.
 *  Details of AUD_BagtNfyFct (optional).
 *
 *  @param      
 *
 *  @retval 
 */
//-----------------------------------------------------------------------------
BOOL AUD_BagtNfyFct (
    const VOID*                     pv_tag,
    AUD_BUF_AGT_NFY_COND_T     e_nfy_cond,
    UINT32                    ui4_data_1,
    UINT32                    ui4_data_2)
{

    return AUD_DrvBagtNfyFct(pv_tag, e_nfy_cond, ui4_data_1,ui4_data_2 );
}

//-----------------------------------------------------------------------------
// AUD_BagtSetSrcInfo
//
/** Brief of AUD_BagtSetSrcInfo.
 *  Details of AUD_BagtSetSrcInfo (optional).
 *
 *  @param      
 *
 *  @retval 
 */
//-----------------------------------------------------------------------------
void AUD_BagtSetSrcInfo (UINT16 ui2_buf_agt_id, const AUD_BUF_AGT_SRC_ALIGN_INFO_T*  t_data_blk, BOOL fgType)
{
    AUD_DrvBagtSetSrcInfo(ui2_buf_agt_id, t_data_blk, fgType);
}

//-----------------------------------------------------------------------------
// AUD_RegBagtReqestFunc
//
/** Register buffer agent notify function.
 *
 *  @param     pu4Handler   Notify function handler
 *  @param     pfNfyFunc    Notify function pointer
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
 //-----------------------------------------------------------------------------
BOOL AUD_RegBagtReqestFunc(UINT32 * pu4Handler, AudBagtDataReqFct pfBagtRqstFct)
{
    return AUD_RegBagtRqstFunc(pu4Handler, pfBagtRqstFct);
}

//-----------------------------------------------------------------------------
// AUD_BagtGetRqstId
//
/**Get current request ID of Buffer agent function.
 *
 *  @retval UINT32  current ID
 */
 //-----------------------------------------------------------------------------
UINT32  AUD_BagtGetRqstId ()
{
    return AUD_DrvBagtGetRqstId ();
}

//-----------------------------------------------------------------------------
// AUD_BagtGetCurrOffset
//
/**Get current byte offset of Buffer agent function.
 *
 *  @retval UINT32  current ID
 */
 //-----------------------------------------------------------------------------
UINT32  AUD_BagtGetCurrOffset ()
{
    return AUD_DrvBagtGetCurrOffset ();
}

// no comments because this is test function for CLI command
void AUD_BagtSetSrcInfoTest ()
{
    AUD_BUF_AGT_SRC_ALIGN_INFO_T t_data_blk;
    t_data_blk.ui4_align = 0x1000;
    t_data_blk.ui4_min_blk_size = 4;
    
    AUD_DrvBagtSetSrcInfo(0, &t_data_blk, TRUE);
}

// no comments because this is test function for CLI command
#define TEST_BUF_START    0x0179c000
#define TEST_BUF_END       0x0279c000
void AUD_BagtDataReqTest(    UINT16                  ui2_buf_agt_id,
                                                        const AUD_BUF_AGT_DATA_BLK_T*     pt_data_blk,
                                                        AUD_2_BAGT_NFY_COND_T eType)
{
    UINT8*               pv_to;
    static UINT8*               pv_from = (UINT8*)TEST_BUF_START;
    UINT32               pv_size;
    AUD_BUF_AGT_DATA_BLK_T r_data_1;
    UINT32 ui4_data_2 = 0;

    x_memset(&r_data_1, 0, sizeof (AUD_BUF_AGT_DATA_BLK_T));
    switch (eType)
    {
        case AUD_2_BAGT_NFY_COND_UNKNOWN:
            
            break;
        case AUD_2_BAGT_NFY_COND_DATA_REQ:
            if (pt_data_blk)
            {
                 pv_to =  pt_data_blk->pui1_buff;
                 pv_size = pt_data_blk->u4Size;
                 pv_size = (((UINT32)pv_from+pv_size) > TEST_BUF_END) ? (TEST_BUF_END-(UINT32)pv_from) : pv_size ;
                 x_memcpy(pv_to,pv_from, pv_size);
                 pv_from = (UINT8*) ((UINT32)pv_from+pv_size);  // for next address
                 pv_from = (pv_from == ( UINT8*)TEST_BUF_END) ? ( UINT8*)TEST_BUF_START : pv_from ;
    
                 r_data_1.pui1_buff = pt_data_blk->pui1_buff;
                 r_data_1.u4Size = pv_size;
                 r_data_1.ui4_req_id = pt_data_blk->ui4_req_id;
                 UNUSED(AUD_BagtNfyFct ( &ui2_buf_agt_id,AUD_BUF_AGT_NFY_COND_DATA_REQ_DONE ,(UINT32)&r_data_1,ui4_data_2));
            }
            break;
        case AUD_2_BAGT_NFY_COND_FLUSH_REQ:
            UNUSED(AUD_BagtNfyFct ( &ui2_buf_agt_id,AUD_BUF_AGT_NFY_COND_FLUSH_DONE,(UINT32)&r_data_1,ui4_data_2));
            break;
        case AUD_2_BAGT_NFY_COND_DATA_CONSUMED:
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------
// AUD_FeederSetRequestInfo
//
/** For feeder to set request information.
 *
 *  @param     prTransmit   Notify function handler
 *
 *  @retval    TRUE   ok
 *  @retval    FALSE  fail
 */
 //-----------------------------------------------------------------------------
BOOL AUD_FeederSetRequestInfo(FEEDER_TRANSMIT *prTransmit)
{
    return AUD_DrvFeederSetRequestInfo(prTransmit);
}

//-----------------------------------------------------------------------------
// AUD_PcmSetting
//
/** PCM format setting
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      prPcmSetting   
 */
//-----------------------------------------------------------------------------
void AUD_PcmSetting(UINT8 u1DecId, const AUD_PCM_SETTING_T * prPcmSetting)
{
 #ifndef CC_MT5391_AUD_3_DECODER
//#if 1
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD));
#endif
    if (prPcmSetting != NULL)
    {
        AUD_DrvPcmSetting(u1DecId, prPcmSetting);
    }    
}

void AUD_LPcmSetting(UINT8 u1DecId, AUD_LPCM_INFO_T * prLPcmInfo)    
{
    UINT32 u4LpcmSetting = 0; 
    AUD_LPCM_SETTING_T rLpcmSetting;
    
    UNUSED(u4LpcmSetting);
    UNUSED(rLpcmSetting);
    UNUSED(_fgLpcmTpyePS);

    _fgLpcmTpyePS = (prLPcmInfo->b_info_not_specific);

    if (!_fgLpcmTpyePS) // AVI
    {
        // Set LPCM format
        // common default
        rLpcmSetting.u1LpcmBitShift = 0;
        rLpcmSetting.u1LpcmDrcValue = 0x80;
        rLpcmSetting.u1LpcmDrcFlag = 0;
        // sampling rare
        rLpcmSetting.u1LpcmFreqGrp2 = 0xf;
        switch (prLPcmInfo->eSampleFreq)
        {
        case FS_8K:
            rLpcmSetting.u1LpcmFreqGrp1 = 20;
            break;        
        case FS_16K:
            rLpcmSetting.u1LpcmFreqGrp1 = 19;
            break;        
        case FS_32K:
            rLpcmSetting.u1LpcmFreqGrp1 = 16;
            break;
        case FS_11K:
            rLpcmSetting.u1LpcmFreqGrp1 = 12;
            break;            
        case FS_22K:
            rLpcmSetting.u1LpcmFreqGrp1 = 11;
            break;
        case FS_24K:
            rLpcmSetting.u1LpcmFreqGrp1 = 3;
            break;        
        case FS_44K:
            rLpcmSetting.u1LpcmFreqGrp1 = 8;
            break;                            
        case FS_96K:             
            rLpcmSetting.u1LpcmFreqGrp2 = 0;
            rLpcmSetting.u1LpcmFreqGrp1 = 1;
            break;
        default : //Set to 48K
            rLpcmSetting.u1LpcmFreqGrp2 = 0;                    
            rLpcmSetting.u1LpcmFreqGrp1 = 0;
            break;                    
        }

        // bit resolution
        rLpcmSetting.u1LpcmBitResGrp2 = 0;
        if (prLPcmInfo->ui2_bits_per_sample == 8) //there maybe 8 or 16 bit PCM
        {
            rLpcmSetting.u1LpcmBitResGrp1 = 3;
        }
        else
        {
            rLpcmSetting.u1LpcmBitResGrp1 = 0;
        }

        
        // channel assignment
        rLpcmSetting.u1LpcmChAssignment = (prLPcmInfo->ui2_num_ch-1);  
        // decoding type
        if ((prLPcmInfo->eLPcmType == LPCM_TYPE_MS_ADPCM)||(prLPcmInfo->eLPcmType == LPCM_TYPE_IMA_ADPCM))
        {
            rLpcmSetting.u1LpcmDecodingType = 0x11;
            rLpcmSetting.u1ADPCMDecodingType = 0x11;
    }
        else
        {
            if ((prLPcmInfo->b_big_endian) || (prLPcmInfo->ui2_bits_per_sample == 8)) //8 bit PCM should inverse byte??
            {
                rLpcmSetting.u1LpcmDecodingType = 0x5;
            }
            else
            {
                rLpcmSetting.u1LpcmDecodingType = 0xd;
            }
            rLpcmSetting.u1ADPCMDecodingType = 0;
    }
        // block align
        rLpcmSetting.u2ADPCMBlockAlign = prLPcmInfo->ui2_block_align;
    
        AUD_SetLpcmTable(AUD_DEC_MAIN, rLpcmSetting);
    }
}

//-----------------------------------------------------------------------------
// AUD_DtvLock
//
/** DTV audio lock check
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)  
 */
//-----------------------------------------------------------------------------
BOOL AUD_DtvLock(UINT8 u1DecId)
{
    return AUD_DrvDtvLock(u1DecId);
}


//-----------------------------------------------------------------------------
// AUD_SetAvSynMode
//
/** DTV audio sync check
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)  
 *  @param      eSynMode       Sync mode, 0:Freerun 1: Slave, 2:Master
 */
//-----------------------------------------------------------------------------
void AUD_SetAvSynMode(UINT8 u1DecId, AV_SYNC_MODE_T eSynMode)
{
    AUD_DrvSetAvSynMode(u1DecId,eSynMode);
}


void _AUD_OutPadEnable(UINT8 u1DecId, BOOL fgEnable)
{
    AUD_OutPadEnable(u1DecId, fgEnable);
}

void _AUD_SpdifOutPadEnable(UINT8 u1DecId, BOOL fgEnable)
{
    AUD_SpdifOutPadEnable(u1DecId, fgEnable);
}

void AUD_DrvClkCfgDualDecMode(BOOL fgEnable)
{
    AUD_ClkCfgDualDecMode(fgEnable);
}

void AUD_SetHeadphoneCond(UINT8 u1DecId, UINT8 u1HPCond)
{
    _aeHPCond[u1DecId] = (AUD_HP_COND_NFY_T)u1HPCond;
}

UINT8 AUD_GetHeadphoneCond(UINT8 u1DecId)
{
    return _aeHPCond[u1DecId];
}

//#ifdef CC_AUD_DVBT_SUPPORT
void AUD_AtunerSigStable(void)
{
    // FIXME
    // Should porting from 71 later
    AUD_DrvATunerLock();
}
//#endif

 void Aud_SetTvSystemMask(UINT8 u1DecId, UINT32 u4Mask)
{
    _AudSetTvSysMask(u1DecId,  u4Mask);
}

 UINT32 Aud_GetTvSystemMask(UINT8 u1DecId)
{
   return  _AudGetTvSysMask(u1DecId);
}

 void Aud_SetDspSpeed(UINT8 u1DecId,UINT16 u2Speed)
{
   AUD_DspSpeed(u1DecId,u2Speed);
}

  void AUD_SetDspDownmixPosition(UINT8 u1DecId,UINT16 u1DMPosition)
{
   AUD_DspDownmixPosition(u1DecId,u1DMPosition);
}

void AUD_SetDspMultiPairOutput(UINT8 u1DecId, UINT16 u1MultiPair) 
{
     	AUD_DspMultiPairOutput(u1DecId, u1MultiPair);
}

void AUD_SetDspLRDownmix(UINT8 u1DecId, BOOL fgEnable) 
{
     	AUD_DspLRDownmix(u1DecId, fgEnable);
}

//-----------------------------------------------------------------------------
// AUD_ReadDspPROM
//
/** Read PROM 16-bits instruction. 
 *  NOTICE: Do not use this function with Rom booting!
 *
 *  @param[in]        u4Addr           read PROM address  
 *
 *  @retval PROM value
 */
//-----------------------------------------------------------------------------
UINT16 AUD_ReadPROM (UINT32 u4Addr)
{
    vWriteDspSram (DspPROMTstAddr, DspPROMEnTst + (u4Addr << 8));
    return (u4ReadDspSram (DspPROMRdAddr) & DspPROMMsk);
}

//-----------------------------------------------------------------------------
// AUD_ReadDspDROM
//
/** Read DROM 24-bits data.
 *
 *  @param[in]        u4Addr           read DROM offset  
 *
 *  @retval DROM value
 */
//-----------------------------------------------------------------------------
UINT16 AUD_ReadDROM (UINT32 u4Addr)
{
    return (u4ReadDspSram (DspDROMRdAddr + u4Addr) & DspDROMMsk);
}

//-----------------------------------------------------------------------------
// AUD_Read32ByteID
//
/** Read ROM 32 bytes ID.
 *
 *  @param[in]        u1ID_p           ROM ID pointer
 *                    u4Num            ROM ID byte number    
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
INT32 AUD_Read32ByteID (UINT8 *u1ID_p, UINT32 u4Num)
{
    INT32 i;
    UINT32 tmp;
    UINT32 start = DspDROMVerOffset;
    
    for (i = u4Num >> 1; i != 0; i --)
    {
        tmp = AUD_ReadDROM (start ++);
        *u1ID_p ++ = tmp & 0xFF;
        *u1ID_p ++ = (tmp >> 8) & 0xFF;
    }

    return AUD_OK;	
}
static BOOL _fgAudMwRealCtrl[3] = {0, 0, 0};

void AUD_MWCtrl_Set(UINT8 u1DecId,BOOL fgPlay)
{
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD));

    VERIFY(x_sema_lock(_hMwCtrlSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    _fgAudMwRealCtrl[u1DecId] = fgPlay;
    
    VERIFY(x_sema_unlock(_hMwCtrlSema) == OSR_OK);    
}

BOOL AUD_MWCtrl_Get(UINT8 u1DecId)
{
    BOOL fgRet = 0;

    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD));

    VERIFY(x_sema_lock(_hMwCtrlSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    fgRet = _fgAudMwRealCtrl[u1DecId];
    
    VERIFY(x_sema_unlock(_hMwCtrlSema) == OSR_OK);
    
    return fgRet;
}
void AUD_SetLPCMFmt(UINT32 u4PriData)
{   // This function is for Program Stream Only. 
    
    AUD_LPCM_SETTING_T rLpcmSetting;

    /*======================================================================================
    *               u4PriData (This is Specification)
    * sampling_freq         5   0x1F000000 (>>24) (internal definition)
    *
    * audio_emphasis_flag   1   0x800000 (>>23)
    * audio_mute_flag       1   0x400000 (>>22)
    * reserved              1   0x200000 
    * audio_frame_number    5   0x1F0000 (>>16)
    *
    * quantization_word_len 2   0xC000   (>>14) (0 = 16bit, 1 = 20bit, 2 = 24bit,  3 = 8bit  )
    * sampling_freq         2   0x3000   (>>12) (0 = fs_48, 1 = fs_96, DVD spec.)   
    *                                           (if == 3, then check bit28 ~ bit 24, internal definition)
    * reserved              1   0x0800   
    * num of audio channels 3   0x0700   (>>8)
    *
    * drc control           8   0xFF
    *
    *           u1LpcmDecodingType  (internal definition)             
    *  bit 0: dvd audio / dvd-video, 
    *  bit 1: non-wav / not dvd video (wav)
    *  bit 2: aob / vob
    *  bit 3: large / little endian
    *  bit 4: non / ADPCM
    *
    * 
    *           Sampling Rate  (internal definition) 
    *  low  (Grp2)   0 = fs_48  1 = fs_96   8 = fs_441   9 = fs_882
    *  high (Grp1)   0 = fs_48  1 = fs_96   2 = fs_192   3 = fs_24 
    *                4 = fs_12  8 = fs_441  9 = fs_882  10 = fs_176
    *               11 = fs_22 12 = fs_11  16 = fs_32   19 = fs_16
    *               20 = fs_8
    *               ==> if Grp2 is 0xf, then Grp1 is taken.
    *
    *
    ============================================================================================*/

    UINT8 u1QuantWord;
    UINT8 u1SampRate;
    UINT8 u1Channels;
    UINT8 u1Drc;

    UNUSED(u1QuantWord);
    UNUSED(u1Drc);
    UNUSED(u1SampRate);
    UNUSED(u1Channels);

    u1QuantWord = (u4PriData & (0xC000)) >> 14;
    u1Channels = (u4PriData & (0x0700)) >> 8;
    u1Drc = (u4PriData & (0xFF));

    u1SampRate = (u4PriData & (0x3000)) >> 12;
    if (u1SampRate == 3)
    {
        u1SampRate = (u4PriData & (0x1F000000)) >> 24;    
        rLpcmSetting.u1LpcmFreqGrp1 = u1SampRate;
        rLpcmSetting.u1LpcmFreqGrp2 = 0xF;        
    }
    else
    {
        rLpcmSetting.u1LpcmFreqGrp1 = 0;
        rLpcmSetting.u1LpcmFreqGrp2 = u1SampRate;
    }        

    //LOG(9,"Q=0x%x, S=0x%x, C=0x%x, D=0x%x\n", u1QuantWord, u1SampRate, u1Channels, u1Drc);
    
    rLpcmSetting.u1LpcmBitShift = 0;    
    rLpcmSetting.u1LpcmDrcValue = 0x80;    
    rLpcmSetting.u1LpcmBitResGrp2 = 0;        
    rLpcmSetting.u1LpcmDrcFlag = 0; 
    
        rLpcmSetting.u1LpcmDecodingType = 0x5; // big endian, non-wav, vob       
        rLpcmSetting.u1LpcmChAssignment = u1Channels;        
        rLpcmSetting.u1LpcmBitResGrp1 = u1QuantWord;            
    rLpcmSetting.u1ADPCMDecodingType = 0;
        
    AUD_SetLpcmTable(AUD_DEC_MAIN, rLpcmSetting);                    
    }

BOOL AUD_IsLpcmFmtPS(void)
    {
    BOOL fgFlag = FALSE;

    // if LPCM from program stream ?
    fgFlag = _fgLpcmTpyePS;

    return fgFlag;
}


//-----------------------------------------------------------------------------
// AUD_MM_Set_Dec_Fmt_Conflict
//
/** For SWDMX callback AUD_DRV to notify that audio fomat is conflict.
 *  e.x. mpeg2 program stream with mp3 audio codec inside.
 *
 *  @param[in]        u1DecId          Decoder ID
 *                    fgFlag           Enable Flag
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_MM_Set_Dec_Fmt_Conflict(UINT8 u1DecId, BOOL fgFlag)
{
    _bfgMMDecFmtConflict[u1DecId] = fgFlag;
    }
        
BOOL AUD_Is_MMDecFmtConflict(UINT8 u1DecId)
{
    return _bfgMMDecFmtConflict[u1DecId];
}

//-----------------------------------------------------------------------------
// AUD_MM_Set_Dec_Fmt_Scramble
//
/** For SWDMX callback AUD_DRV to notify that audio fomat is scramble.
 *  e.x. ts file with scramble audio data
 *
 *  @param[in]        u1DecId          Decoder ID
 *                    fgFlag           Enable Flag
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_MM_Set_Dec_Fmt_Scramble(UINT8 u1DecId, BOOL fgFlag)
{
    _bfgMMDecFmtScramble[u1DecId] = fgFlag;
    VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_AVSYNC));
}

BOOL AUD_Is_MMDecFmtScramble(UINT8 u1DecId)
{
    return _bfgMMDecFmtScramble[u1DecId];
}


//-----------------------------------------------------------------------------
// AUD_MM_Set_Avsync_event
//
/** For SWDMX callback AUD_DRV to notify that audio avsync event.
 *  e.x. seek to file end could trigger audio check no data and send eos without wait for 4.5s time out 
 *
 *  @param[in] 
 *                    
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_MM_Set_Avsync_event(UINT8 u1DecId)
{
    VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_AVSYNC));
}



//-----------------------------------------------------------------------------
// AUD_MTS_CFO_IDX
//
/** For CID ATD query usage
 *
 *  @param[in]        none
 *
 *  @retval number of grids of CFO units
 */
//-----------------------------------------------------------------------------
INT32 AUD_MTS_CFO_IDX(void)
{   
    UINT32 u4Qmix, u4DcVal, u4SndSys, eDec2Type, u4SndSysAddr;
    INT32 i4ShiftBlocks;
    AUD_DEC_STREAM_FROM_T eStreamFrom;
    AUD_FMT_T eDecType;

    AUD_GetDecodeType(AUD_DEC_MAIN, &eStreamFrom, &eDecType);
    eDec2Type = DSP_CommDramRead(0x3) & 0x0F00; // decoder2 version
    if ((eDecType == AUD_FMT_MTS) && AUD_IsDecoderPlay(AUD_DEC_MAIN)) //BTSC decoder 1
    {
        if (!AUD_DspGetCarrierShiftMode(AUD_ATV_DECODER_MTS))          
        {
            LOG(11,"\n\nNot opening carrier shift mode for MTS\n\n");
            return 999;    
        }

        //u4Qmix = u4ReadDspSram(0x2b2);// Q-mixer, used to judge if shifted (0x2aaab0 is no shift)
        u4Qmix = DSP_CommDramRead(0xe5f); //Use dram is more stable than sram. (DSP code releated.)
        
        u4DcVal = u4ReadDspSram(CFO_ACC_PROB_SRAM_ADDR);    
        if (u4DcVal > 0x800000){
            u4DcVal = (u4DcVal ^ 0xffffff) + 1;
            i4ShiftBlocks = (INT32)u4DcVal * -1;
            i4ShiftBlocks = (i4ShiftBlocks - 0x39580) / 0x72b00; 
        }else{
            i4ShiftBlocks = (INT32)u4DcVal;
            i4ShiftBlocks = (i4ShiftBlocks + 0x39580) / 0x72b00; 
        }
        if(u4Qmix < 0x2aaab0){ //bigger frequency, smaller value
            i4ShiftBlocks += 1; 
        }
        if(u4Qmix > 0x2aaab0){ 
            i4ShiftBlocks -= 1; 
        }
        LOG(11,"\n\n BTSC carrier shift %d steps\n\n", i4ShiftBlocks);
    }
    else if ((eDec2Type == 0x0600) && AUD_IsDecoderPlay(AUD_DEC_AUX)) //BTSC decoder 2
    {
        LOG(11, "\n\nNot opening carrier shift mode Query for MTS_DEC2\n\n");
        return 999;     
    }
    else if (((eDecType == AUD_FMT_A2) && AUD_IsDecoderPlay(AUD_DEC_MAIN))
               ||((eDec2Type == 0x0700) && AUD_IsDecoderPlay(AUD_DEC_AUX))) //A2 decoder 2
    {
        if (!AUD_DspGetCarrierShiftMode(AUD_ATV_DECODER_A2))          
        {
             LOG(11,"\n\nNot opening carrier shift mode for A2\n\n");
             return 999;    
        }
        if (AUD_IsDecoderPlay(AUD_DEC_MAIN)) u4SndSysAddr = 0x150;
        else u4SndSysAddr = 0x1A0;

        u4SndSys = DSP_CommDramRead(u4SndSysAddr)>>8;
        //u4Qmix = u4ReadDspSram(0x2b9);// Q-mixer, used to judge if shifted (0x2aaab0 is no shift)
        u4Qmix = DSP_CommDramRead(0xe5f);
        
        u4DcVal = u4ReadDspSram(CFO_ACC_PROB_SRAM_ADDR);    
        if (u4DcVal > 0x800000){
            u4DcVal = (u4DcVal ^ 0xffffff) + 1;
            i4ShiftBlocks = (INT32)u4DcVal * -1;
            i4ShiftBlocks = (i4ShiftBlocks - 0x52000) / 0x0A4000; 
        }else{
            i4ShiftBlocks = (INT32)u4DcVal;
            i4ShiftBlocks = (i4ShiftBlocks + 0x52000) / 0x0A4000; 
        }
        if (u4SndSys & 0x1)
        {
            LOG(11,"\n\nNot support CFO query for A2_M\n\n");
            return 999; 
        }
        else if (u4SndSys & 0x2)
        {
            //A2 B/G
            if(u4Qmix < 0x3425F0){ //bigger frequency, smaller value
                i4ShiftBlocks += 1; 
            }
            if(u4Qmix > 0x3425F0){ 
               i4ShiftBlocks -= 1; 
            }
        }
        else if (u4SndSys & 0x1C)
        {
            //A2 D/K series
            if(u4Qmix < 0x3DA130){ //bigger frequency, smaller value
                i4ShiftBlocks += 1; 
            }
            if(u4Qmix > 0x3DA130){ 
               i4ShiftBlocks -= 1; 
            }
        }
        else
        {
            LOG(11, "\n\nIncorrect  A2 sound system\n\n");
            LOG(11, "u4SndSys = %x\n", u4SndSys);
            return 999; 
        }
        LOG(7,"\n\n A2 carrier shift %d steps\n\n", i4ShiftBlocks);
    }
    else if (((eDecType == AUD_FMT_PAL) && AUD_IsDecoderPlay(AUD_DEC_MAIN))    
               ||((eDec2Type == 0x0900) && AUD_IsDecoderPlay(AUD_DEC_AUX))) //PAL decoder 2
    {
        if (!AUD_DspGetCarrierShiftMode(AUD_ATV_DECODER_PAL))          
        {
             LOG(11,"\n\nNot opening carrier shift mode for PAL\n\n");
             return 999;    
        }
        if (AUD_IsDecoderPlay(AUD_DEC_MAIN)) u4SndSysAddr = 0x150;
        else u4SndSysAddr = 0x1A0;

        u4SndSys = DSP_CommDramRead(u4SndSysAddr)>>8;

        //u4Qmix = u4ReadDspSram(0x2b9);// Q-mixer, used to judge if shifted (0x2aaab0 is no shift)    
        u4Qmix = DSP_CommDramRead(0xe5f);
        if ((u4Qmix != 0x3DA130)&&(u4Qmix != 0x38E390)&&(u4Qmix != 0x3425F0))
        {
            LOG(8,"u4Qmix = 0x%x\n", u4Qmix);
        }
        
        u4DcVal = u4ReadDspSram(CFO_ACC_PROB_SRAM_ADDR);    
        if (u4DcVal > 0x800000){
            u4DcVal = (u4DcVal ^ 0xffffff) + 1;
            i4ShiftBlocks = (INT32)u4DcVal * -1;
            i4ShiftBlocks = (i4ShiftBlocks - 0x14F80) / 0x029F00; 
        }else{
            i4ShiftBlocks = (INT32)u4DcVal;
            i4ShiftBlocks = (i4ShiftBlocks + 0x14F80) / 0x029F00; 
        }
        if (u4SndSys & 0x1)
        {
            //PAL I
            UINT8 cal_cnt = 0;
            while (u4Qmix != 0x38E390)
            {
                if (u4Qmix < 0x38E390)
                {
                    u4Qmix += 0x26D0;
                    i4ShiftBlocks += 1;
                    cal_cnt++;
                }
                else if (u4Qmix > 0x38E390)
                {
                    u4Qmix -= 0x26D0;
                    i4ShiftBlocks -= 1; 
                    cal_cnt++;
                }
                if (cal_cnt>=25)
                {
                    i4ShiftBlocks = 0;
                    break;
                }
            }
        }
        else if (u4SndSys & 0x2)
        {
            //PAL B/G
            UINT8 cal_cnt = 0;
            while (u4Qmix != 0x3425F0)
            {
                if (u4Qmix < 0x3425F0)
                {
                    u4Qmix += 0x26D0;
                    i4ShiftBlocks += 1;
                    cal_cnt++;
                }
                else if (u4Qmix > 0x3425F0)
                {
                    u4Qmix -= 0x26D0;
                    i4ShiftBlocks -= 1; 
                    cal_cnt++;
                }
                if (cal_cnt>=25)
                {
                    i4ShiftBlocks = 0;
                    break;
                }
            }
        }
        else if (u4SndSys & 0x4)
        {
            //PAL D/K
            UINT8 cal_cnt = 0;
            while (u4Qmix != 0x3DA130)
            {
                if (u4Qmix < 0x3DA130)
                {
                    u4Qmix += 0x26D0;
                    i4ShiftBlocks += 1;
                    cal_cnt++;
                }
                else if (u4Qmix > 0x3DA130)
                {
                    u4Qmix -= 0x26D0;
                    i4ShiftBlocks -= 1; 
                    cal_cnt++; 
                }
                if (cal_cnt>=25)
                {
                    i4ShiftBlocks = 0;
                    break;
                }
            }
        }
        else
        {
            LOG(11, "\n\nIncorrect  PAL sound system for CFO query\n\n");
            LOG(11, "u4SndSys = %x\n", u4SndSys);
            return 999; 
        }
        LOG(11,"\n\n PAL carrier shift %d steps\n\n", i4ShiftBlocks);
    }
    else
    {
        LOG(11, "\nDecoder Type Incorrect(not BTSC, A2, PAL) for CFO query!\n\n");
        LOG(11, " eDecType = %d, eDec2Type  %d\n", eDecType, eDec2Type);
        return 999; 
    }
    
    return i4ShiftBlocks;
}
//-----------------------------------------------------------------------------
// AudCmdAtvHdevDetQuery
//
/** For CID ATD query usage
 *
 *  @param[in]        none
 *
 *  @retval TRUE(Hdev detected) / FALSE(Hdev not detected) /999 (Invalid)
 */
//-----------------------------------------------------------------------------
INT32 AudCmdAtvHdevDetQuery(void)
{

    UINT32 eDec2Type, i4HdevDetResult, u4DramValue;
    AUD_DEC_STREAM_FROM_T eStreamFrom;
    AUD_FMT_T eDecType;

    AUD_GetDecodeType(AUD_DEC_MAIN, &eStreamFrom, &eDecType);
    eDec2Type = DSP_CommDramRead(0x3) & 0x0F00; // decoder2 version
    if ((eDecType == AUD_FMT_MTS) && AUD_IsDecoderPlay(AUD_DEC_MAIN)) //BTSC decoder 1
    {       
         LOG(11, "\n\nQuery for Hdev mode detection  of PAL decoder1...\n\n");     
         i4HdevDetResult = u4ReadDspSram(0x15fa);
         LOG(11, "  ovm_det_result  = 0x%x \n", i4HdevDetResult);       
         return i4HdevDetResult;     
    }
    else if ((eDec2Type == 0x0600) && AUD_IsDecoderPlay(AUD_DEC_AUX)) //BTSC decoder 2
    {
        LOG(11, "\n\nNot opening Hdev detection Query for MTS_DEC2\n\n");
         return 999;     
    }
    else if ((eDecType == AUD_FMT_A2) && AUD_IsDecoderPlay(AUD_DEC_MAIN)) //A2 decoder1
    {
        LOG(11, "\n\nQuery for Hdev mode detection  of A2 decoder1...\n\n");
        return 0;
    }
   else if ((eDec2Type == 0x0700) && AUD_IsDecoderPlay(AUD_DEC_AUX)) //A2 decoder 2
    {
        LOG(11, "\n\nQuery for Hdev mode detection  of A2 decoder2...\n\n");
        return 0;       
    }
    else if ((eDecType == AUD_FMT_PAL) && AUD_IsDecoderPlay(AUD_DEC_MAIN)) //PAL decoder
    {
        u4DramValue = DSP_DecDramRead(9);
        LOG(11, "\n\nQuery for Hdev mode detection  of PAL decoder1...\n\n");            
        LOG(11, "   hdev_a2_result  = 0x%x \n", u4DramValue);
        i4HdevDetResult = (u4DramValue & 0x0100) >> 8;
        return i4HdevDetResult;
    }
    else if ((eDec2Type == 0x0900) && AUD_IsDecoderPlay(AUD_DEC_AUX)) //PAL decoder 2
    {
        //ReadDspDec2CacheDram(9,u4DramValue);
        u4DramValue = DSP_Dec2DramRead(9);
        LOG(11, "\n\nQuery for Hdev mode detection  of PAL decoder2...\n\n");            
        LOG(11, "   hdev_a2_result  = 0x%x \n", u4DramValue);
        i4HdevDetResult = (u4DramValue & 0x0100) >> 8;
        return i4HdevDetResult;
    }
    else
    {
        LOG(11, "\nDecoder Type Incorrect(not BTSC, A2, PAL) for ATV Hdev mode query!\n\n");
        LOG(11, " eDecType = %d, eDec2Type  %d\n", eDecType, eDec2Type);
        return 999; 
    }

}

//-----------------------------------------------------------------------------
// AUD_DSPCmdSema_Lock
//
/** Brief of AUD_DSPCmdSema_Lock.
 *  Details of AUD_DSPCmdStop (optional).
 *
 *  @param        
 *
 *  @retval
 */
//-----------------------------------------------------------------------------
void AUD_DSPCmdSema_Lock(void)              //  -- DSP Force --
{
    VERIFY(x_sema_lock(_hSema, X_SEMA_OPTION_WAIT) == OSR_OK);
}
void AUD_DSPCmdSema_UnLock(void)
{
    VERIFY(x_sema_unlock(_hSema) == OSR_OK);
}

UINT32 AUD_GetTargetCountry(void)
{
    UINT32 _u4AudTargetCountry;
    _u4AudTargetCountry = GetTargetCountry();
#ifdef COUNTRY_FROM_EEPROM
    _u4AudTargetCountry &= COUNTRY_AUDIO_MASK;
    _u4AudTargetCountry = _u4AudTargetCountry >> 5;
#endif
    return _u4AudTargetCountry;
}

#ifdef CC_AUD_SETUP_MELODY
void AUD_PlaySetupMelody(UINT8 u1DecId, UINT32 u4Channel, UINT32 u4RepeatCount)
{
    AUD_CLIP_TYPE_T rAudClip;
    AUD_SETUP_MELODY_TABLE_T *rAudSetupMelody;

	_fgSetupPlayFinish = FALSE;
	
	x_memset(&rAudClip, 0, sizeof(AUD_CLIP_TYPE_T));
    
    if (u1DecId >= AUD_DEC_NUM) 
    {   
        u1DecId = AUD_DEC_MAIN;
    }

    rAudClip.u4OutputChlSelect = u4Channel;
    rAudClip.u4RepeatCount = u4RepeatCount;
       
    rAudClip.fgPlayTestTone = FALSE;
    rAudClip.eAudFmt = AUD_FMT_MP3;

    //Aud_GetSetupMelodyInfo(&rAudClip);
    rAudSetupMelody = (AUD_SETUP_MELODY_TABLE_T*)DRVCUST_OptGet(eAudioLoaderMusicData);
    if (rAudSetupMelody == 0)
    {        
        LOG(0,"No setup melody is prepared !!!!\n");
        _fgSetupPlayFinish = TRUE;
        return;
    }

	rAudClip.eMemBuf.pData = (UINT8*)rAudSetupMelody->prAudSetupMelody;
    rAudClip.eMemBuf.u4Length = rAudSetupMelody->u4Size;
	
    _AUD_IgnoreDecNotify(u1DecId, TRUE);
    _AUD_IgnoreSetupPlayNotify(TRUE);
    AUD_SetAudClip(u1DecId, &rAudClip);
    AUD_PlayAudClip(u1DecId, rAudClip.u4OutputChlSelect,
                   rAudClip.fgPlayTestTone);
    _AUD_IgnoreDecNotify(u1DecId, FALSE);
}
BOOL AUD_IsSetupMelodyFinish(void)
{
    return _fgSetupPlayFinish;
}
void AUD_SetSetupPlayStatue(BOOL fgIsFinish)
{
	_fgSetupPlayFinish = fgIsFinish;
}

void AUD_StopSetupMelody(UINT8 u1DecId)
{
    if (u1DecId >= AUD_DEC_NUM) 
    {   
        u1DecId = AUD_DEC_MAIN;
    }
    _AUD_IgnoreDecNotify(u1DecId, TRUE);
    AUD_StopAudClip(u1DecId);
    _AUD_IgnoreDecNotify(u1DecId, FALSE);
}

#endif /*CC_AUD_SETUP_MELODY*/


//================== Begin of Audio Flash AQ Functions ===============================
//-----------------------------------------------------------------------------
/** Aud_FlashAQRead
 *  Read data from NAND Flash 
 *
 *  @param           
 *  @retval void
 */
//-----------------------------------------------------------------------------
#ifdef CC_AUD_USE_FLASH_AQ
INT32 Aud_FlashAQRead(UINT16 u2Offset, UINT8 *u1Data, UINT32 u4Length)
{
    UINT8 u1AQPartition;
    UINT32 u2AQSize;    

    UNUSED(u2AQSize);
    u1Data[0] = '\0';        
    u1AQPartition = (UINT8)DRVCUST_OptGet(eAudioFlashAQPartition);
    if (u1AQPartition == 0xFF)
    {
        LOG(3,"AudioFlashAQPartition is not defined !!\n");
        return -1;
    }

#if 0    
    // check boundary 
    //u2AQSize = (UINT32)DRVCUST_OptGet(eAudioFlashAQSize);
    if (u2Offset > u2AQSize)
    {
        LOG(0,"Fail! AQ test read length larger than 0x%x\n", u2AQSize);
        return;
    }
#endif
    return (STORG_SyncNandRead(u1AQPartition, u2Offset, (UINT8*)(VOID*)u1Data, u4Length));

}

//-----------------------------------------------------------------------------
/** Aud_FlashAQWrite
 *  !!!! CAUTION !!!! This API is for CLI Testing ONLY !! 
 *  !!!! CAUTION !!!! Do not Apply to any normal flow besides Testing !!
 *  NANDPART_Write is NOT allowed to be called here.
 *
 *  @param           
 *  @retval void
 */
//-----------------------------------------------------------------------------
void Aud_FlashAQWrite(UINT16 u2Offset, UINT8 *u1Data, UINT32 u4size)
{
    UINT8 u1AQPartition;
    UINT32 u2AQSize;    
    UINT64 u8Offset;
    
    UNUSED(u2AQSize);
    u1AQPartition = (UINT8)DRVCUST_OptGet(eAudioFlashAQPartition);
    if (u1AQPartition == 0xFF)
    {
        LOG(3,"AudioFlashAQPartition is not defined !!\n");
        return;
    }    

#if 1
    // check boundary.
    //u2AQSize = (UINT32)DRVCUST_OptGet(eAudioFlashAQSize)
    if ((u2Offset + u4size) > AUD_FLASH_AQ_SIZE)
    {
        LOG(0,"AQ test write length larger than 0x%x\n", AUD_FLASH_AQ_SIZE);
        return;
    }
#endif    
    u8Offset = ((UINT64)u1AQPartition << 32) | ((UINT64)u2Offset);
    NANDPART_Write(u8Offset, (UINT32)u1Data, u4size);
}

BOOL Aud_FlashAQCompVersion(CHAR *szCurVer, CHAR *szUpgVer)
{
    if (x_strcmp(szUpgVer,szCurVer) >= 0)        
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
//-----------------------------------------------------------------------------
/** Aud_FlashAQ_SetDefault
 *  Set default data from drvcust to NAND Flash.
 *
 *  @param           
 *  @retval void
 */
//-----------------------------------------------------------------------------
void Aud_FlashAQ_SetDefault(void)
{
    UINT8* pAudAQData;
    UINT32 u4AudAQSize;
    UINT32 u4Offset;
    UINT32 i,j;

    pAudAQData = (UINT8 *)DRVCUST_OptGet(eAudioFlashAQData);
    u4AudAQSize = (UINT32)DRVCUST_OptGet(eAudioFlashAQDataSize); 

    if (pAudAQData == 0)
    {
        LOG(0, "No Default AQ data is defined !!\n");
        return;
    }
    Aud_FlashAQWrite(0, pAudAQData, u4AudAQSize);

#ifndef CC_AUD_NEW_AQ_PARSER
    // External PEQ Data
    u4Offset = u4AudAQSize;
    u4AudAQSize = (FLASH_AQ_SRC_NUM*FLASH_AQ_SM_NUM*AQ_AMP_FS_NUM*AQ_EXT_PEQ_ITEMS)*3; /* 3 bytes for every item in Flash.*/
    pAudAQData = (UINT8 *)BSP_AllocAlignedDmaMemory(u4AudAQSize, 32);    
    if (pAudAQData == NULL)
    {
        LOG(3,"Memory request for PEQ Data fail !!\n");
        return;
    }
    //All init data should be 0x000000,0x000000,0x000000,0x000000,0x4000000, 
    //                        0x000000,0x000000,0x000000,0x000000,0x4000000,
    //                        0x000000,0x000000,0x000000,0x000000,0x4000000,...
    for (i=0; i<(u4AudAQSize/3); i++)
    {
        j = i * 3;
        if ((i+1)%5 == 0)
        {
            *((UINT8*)pAudAQData +j)   = 0x40;
            *((UINT8*)pAudAQData +j+1) = 0x00;
            *((UINT8*)pAudAQData +j+2) = 0x00;            
        }
        else
        {
            *((UINT8*)pAudAQData +j)   = 0x00;
            *((UINT8*)pAudAQData +j+1) = 0x00;
            *((UINT8*)pAudAQData +j+2) = 0x00;            
        }
    }
    LOG(3, "u4Offset = %d, Write default PEQ data to Flash\n",u4Offset);
    Aud_FlashAQWrite(u4Offset, pAudAQData, u4AudAQSize);
#endif  // CC_AUD_NEW_AQ_PARSER  

}
//-----------------------------------------------------------------------------
/** Aud_FlashAQ_GetDscrpt
 *  Get the data description from drvcust.
 *
 *  @param           
 *  @retval void
 */
//-----------------------------------------------------------------------------
BOOL Aud_FlashAQ_GetDscrpt(AUD_AQ_DSCRPT_T** pAudFlashAQDscrpt)
{
    if ((DRVCUST_OptQuery(eAudioFlashAQDataDscrpt, (UINT32*)(void*)pAudFlashAQDscrpt)) != 0)
    {
        LOG(0, "No Flash AQ Table defined!");
        return FALSE;
    }

    if (pAudFlashAQDscrpt != 0)
    {
        return TRUE;
    }

    LOG(0, "Flash AQ Table is NULL");
    return FALSE;
}

//-----------------------------------------------------------------------------
/** Aud_FlashAQ_Fill_Data
 *  Get the data from NAND flash. 
 *  Parsing them according to data description and fill them to data structure.
 *
 *  @param           
 *  @retval void
 */
//-----------------------------------------------------------------------------
#define AUD_AQ_READ_AMOUNT 10
void Aud_FlashAQ_Fill_Data(BOOL fgDelayMode)
{    
    AUD_AQ_DSCRPT_T *eAudAQDscrpt;
    UINT8 u1AudAQDscrptSize;
    UINT32 u4Offset;
    UINT32 u4Temp,u4Temp1;
    UINT8 u1X, u1Y, u1Bytes;
    UINT8 pData[AUD_FLASH_AQ_BUF_SIZE];
    UINT8 i;
    UINT32 u4AQPostScale[2];

    UNUSED(u4AQPostScale);
    UNUSED(u4Temp1);

    // Check first 2 bytes 
    pData[0] = '\0';
    Aud_FlashAQRead(FLASH_AQ_VER_OFFSET, (UINT8*)pData, FLASH_AQ_VER_LENGTH);
    if (x_strncmp((CHAR*)pData, "AQ", 2) !=0)
    {
        LOG(0, "Flash AQ data is not correct, using default value\n");
        ; //todo : Using default value if no flash AQ is available.
        return;
    }
        
    u1AudAQDscrptSize = (UINT8)DRVCUST_OptGet(eAudioFlashAQDataDscrptSize);
        
    if (Aud_FlashAQ_GetDscrpt(&eAudAQDscrpt)) //Get the Data Structure.
    {    
        u4Offset = 0;
        for (i=0; i<u1AudAQDscrptSize; i++)
        {
            u1X = eAudAQDscrpt[i].u1X;
            u1Y = eAudAQDscrpt[i].u1Y;
            u1Bytes = eAudAQDscrpt[i].u1Bytes;
            u4Temp = u1X * u1Y * u1Bytes;
            Aud_FlashAQRead(u4Offset, pData, u4Temp);
            Aud_FlashAQParsing(eAudAQDscrpt[i].eDataType, pData);
            u4Offset += u4Temp;
        }
        
#ifndef CC_AUD_NEW_AQ_PARSER        
        //External PEQ
        u4Temp = (FLASH_AQ_SRC_NUM * FLASH_AQ_SM_NUM * AQ_AMP_FS_NUM * AQ_EXT_PEQ_ITEMS) * 3;

        if (!fgDelayMode)
        {
            Aud_FlashAQRead(u4Offset, pData, u4Temp);
            u4Offset += u4Temp;
        }
        else
        {
            #if 0
            //separating to small units.
            u4Temp1 = 0;
            if ((u4Temp % AUD_AQ_READ_AMOUNT)!=0)
            {
                u4Temp1 = u4Temp - (u4Temp/AUD_AQ_READ_AMOUNT)*AUD_AQ_READ_AMOUNT;
            }
            u4Temp = u4Temp/AUD_AQ_READ_AMOUNT;
            for(i=0;i<AUD_AQ_READ_AMOUNT;i++)
            {
                Aud_FlashAQRead(u4Offset, (UINT8*)(pData+u4Temp*i), u4Temp);
                u4Offset += u4Temp;
                x_thread_delay(100);
            }
            if (u4Temp1!=0)
            {
                Aud_FlashAQRead(u4Offset, (UINT8*)(pData+u4Temp*AUD_AQ_READ_AMOUNT), u4Temp1);
                u4Offset += u4Temp1;
            }
            #endif
        }

        LOG(3, "Fill all PEQ Data, u4Offset = %d\n", u4Offset);
        Aud_FlashAQParsing(AQ_TYPE_EXT_PEQ, pData);        
        
#endif //n CC_AUD_NEW_AQ_PARSER
    }
    else
    {
        LOG(0, "Flash AQ data is not correct, using default value\n");
        ; //todo : Using default value if no flash AQ is available.
    }    

    if (Aud_FlashAQQuery(AQ_TYPE_POST, (void*)&u4AQPostScale))
    {
        ADAC_DigiAmp_SetPostScale(AUD_AMP_ID_LR, u4AQPostScale[0],u4AQPostScale[1]);
    }
}

void AUD_FlashAQ_Init(void)
{
    //Set all flag to False;
    Aud_FlashAQSetValue();

    //Fill all data into dram.
    Aud_FlashAQ_Fill_Data(FALSE);
}

#endif /* CC_AUD_USE_FLASH_AQ */
//=================== END of Audio Flash AQ Functions ==============================

UINT8 AUD_GetPIPVideoPath(void)
{
    UNUSED(AUD_GetPIPVideoPath);
    return AUD_DrvGetPIPVideoPath();
}

