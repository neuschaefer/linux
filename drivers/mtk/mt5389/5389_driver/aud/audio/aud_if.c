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
 * $Author: kai.wang $
 * $Date: 2012/08/30 $
 * $RCSfile: aud_if.c,v $
 * $Revision: #8 $
 *
 *---------------------------------------------------------------------------*/

/** @file aud_if.c
 *  Audio decoder control interface primitive API
 */

#define AUD_EXTERN_FILE

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
#include "adac_if.h"
#include "dsp_common.h"
#include "drv_adsp.h"
#include "drvcust_if.h"
#include "aud_hw.h"

#ifdef CHANNEL_CHANGE_LOG
#include "x_timer.h"
#endif
#include "aud_dsp_cfg.h"
#ifdef SUPPORT_FLASH_AQ
#include "nor_if.h"
#ifdef CC_NAND_ENABLE
#include "nand_if.h"
#endif
#include "storage_if.h"
#include "linux/file_ops.h"
#endif

#ifdef ENABLE_MULTIMEDIA
#include "swdmx_if.h"
#endif

#ifdef LINUX_TURNKEY_SOLUTION
extern int rm_copy_to_user(VOID *pv_user,VOID* pv_kernel,UINT32 size);
extern VOID rm_copy_from_user(VOID* pv_kernel,VOID *pv_user,UINT32 size);
#endif

LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define ADRV_DUAL_DEC_WAIT_CMD_Q     "AUD_DUAL_DEC_WAIT_CMD_Q"
#define AUD_CMD_QUEUE_SIZE      32
#define AUD_DUAL_DEC_TIME_OUT   20000
#define CFO_ACC_PROB_SRAM_ADDR 0x36F6

#define USE_16BIT_ALIGNMENT

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------
extern void _MT5365_Mute(UINT8 u1DacId, BOOL fgEnable);

extern AUD_AOUT_PAD_CFG_T _grAOutPadCfg;
extern UINT32 _gu4AudCtrlType;

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
extern UINT32 u4ReadDspSram(UINT32 u4Addr);
extern void vADSPIRQEnable(BOOL fgEnable);
extern UINT8 AUD_DspGetCategoryCode(void);
extern BOOL AUD_DspSetSCMS(UINT8 u1CopyRight, UINT8 u1CategoryCode);

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static HANDLE_T _hSema;
static HANDLE_T _hCmdWaitSema[AUD_DEC_MAX];
static HANDLE_T _hAsynCmdSema;
static HANDLE_T _hMwCtrlSema;

static BOOL _afgDecPause[AUD_DEC_MAX] = {FALSE, FALSE, FALSE, FALSE};

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

static UINT8 _au1ChannelVolume[AUD_DEC_MAX][AUD_CH_NUM];
static BOOL  _fgClipChnVolChanged = FALSE;

static AUD_HP_COND_NFY_T _aeHPCond[AUD_DEC_MAX] = {AUD_HP_COND_HEADPHONE_COND_UNPLUGED, AUD_HP_COND_HEADPHONE_COND_UNPLUGED,
                                                   AUD_HP_COND_HEADPHONE_COND_UNPLUGED, AUD_HP_COND_HEADPHONE_COND_UNPLUGED};
static BOOL _bfgMMDecFmtConflict[AUD_DEC_MAX] = {FALSE,FALSE,FALSE,FALSE};
static BOOL _bfgMMDecFmtScramble[AUD_DEC_MAX] = {FALSE,FALSE,FALSE,FALSE};

#ifdef CC_AUD_SETUP_MELODY
//The default value of _fgSetupPlayFinish should be FALSE.
//If APP finish init before logo music starting play, it will get TRUE when query logo music status.
static BOOL _fgSetupPlayFinish = FALSE; 
#endif

//static PCM_INPUT_TYPE_T rPcmEndian = PCM_16_BITS_BIG_ENDIAN;
static BOOL _fgLpcmTpyePS = FALSE;

static BOOL _fgPCMDetectRawMute = FALSE;

#ifdef CC_AUD_SETUP_MELODY
#ifdef  LINUX_TURNKEY_SOLUTION
VOID * pMelody_buf;
static UINT8 u1Melodyflag;
#endif
#endif

#ifdef CC_AUD_VBASS_SUPPORT
UINT8 u1VBassCutOffFreq = 0;
#endif

static BOOL _fgAudInit = FALSE;
static BOOL _fgAudInitOK = FALSE;
static HANDLE_T _hSemaAudInit = NULL_HANDLE;
static BOOL _fgSRS = TRUE;
static BOOL _fgMpeg4 = TRUE;

#ifdef CC_AUD_HDMI_SPDIF_CFG
AUD_OUTPUT_STATE _u1OutputState = DigitalMute_AnalogMute;
#endif

#ifdef  LINUX_TURNKEY_SOLUTION
static AUD_CLIP_TYPE_T _u1AudClip[3];
VOID * u1Clip_buf;
static UINT8 u1Clipfg;
#endif
static UINT16 u2AvSyncDbgFlag = 0;


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static void _AudAsynCmdSemaRelease(UINT8 u1DecId, AUD_DEC_CMD_T eAudDecCmd)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
        
    LOG(7, "Aud dec(%d) Async AUD_Command <%s> done\n", u1DecId, _paAudCmd[eAudDecCmd]);
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
    
    AUD_DEC_ID_VALIDATE(u1DecId);
    LOG(7, "Aud dec(%d) AUD_Command <%s> done\n", u1DecId,_paAudCmd[eAudDecCmd]);

    // Get notify function
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy));
 
    if (pfAudDecNfy != NULL)
    {
        // Notify middleware 
#ifdef CC_AUD_SETUP_MELODY        
        if( (eAudDecCmd == AUD_CMD_STOP) && (  _fgSetupPlayFinish == FALSE))
        {
            pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecoderId, AUD_COND_AUD_CLIP_DONE, 
                        _gu4AudCtrlType, u4Result);
        }
       
       else if ((eAudDecCmd == AUD_CMD_PLAY) || (eAudDecCmd == AUD_CMD_PAUSE) ||
            (eAudDecCmd == AUD_CMD_STOP) || (eAudDecCmd == AUD_CMD_RESET) ||
            (eAudDecCmd == AUD_CMD_RESUME))
        {
            pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecoderId, AUD_COND_CTRL_DONE, 
                        _gu4AudCtrlType, u4Result);
        }

#else
        if ((eAudDecCmd == AUD_CMD_PLAY) || (eAudDecCmd == AUD_CMD_PAUSE) ||
            (eAudDecCmd == AUD_CMD_STOP) || (eAudDecCmd == AUD_CMD_RESET) ||
            (eAudDecCmd == AUD_CMD_RESUME))
        {
            pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecoderId, AUD_COND_CTRL_DONE, 
                        _gu4AudCtrlType, u4Result);
        }

#endif
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
    AUD_DEC_ID_VALIDATE(u1DecId);
    LOG(7, "Aud dec(%d) AUD_WaitCommand <%s> to be done\n", u1DecId ,_paAudCmd[eAudDecCmd]);
    VERIFY(x_sema_lock(_hCmdWaitSema[u1DecId], X_SEMA_OPTION_WAIT) == OSR_OK);
}

#ifdef LINUX_TURNKEY_SOLUTION
extern int alsa_card_dummy_init(void);
#endif

void AUD_SetMMAudioOnly(BOOL fgMMAudioOnly)
{
	AUD_DrvSetMMAudioOnly(fgMMAudioOnly);
}
//-----------------------------------------------------------------------------

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
    UINT32 u4Index = 0;

	if(NULL_HANDLE == _hSemaAudInit)
	{
		VERIFY(x_sema_create(&_hSemaAudInit, X_SEMA_TYPE_BINARY,
									X_SEMA_STATE_UNLOCK) == OSR_OK);
	}
    VERIFY(x_sema_lock(_hSemaAudInit, X_SEMA_OPTION_WAIT) == OSR_OK);
    if (_fgAudInitOK)
    {
	    VERIFY(x_sema_unlock(_hSemaAudInit) == OSR_OK);
        return AUD_OK;
    }    

    if (!_fgAudInit)
    {
        VERIFY (x_sema_create(&_hSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        VERIFY (x_sema_create(&(_hCmdWaitSema[AUD_DEC_MAIN]), X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY (x_sema_create(&(_hCmdWaitSema[AUD_DEC_AUX]), X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY (x_sema_create(&(_hCmdWaitSema[AUD_DEC_THIRD]), X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY (x_sema_create(&(_hCmdWaitSema[AUD_DEC_4TH]), X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);

        VERIFY (x_sema_create(&_hAsynCmdSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        VERIFY (x_sema_create(&_hMwCtrlSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
    }
    
    if(AUD_LoadSRSFlag()==1)
    {
        AUD_SupportSRS(TRUE);
    }
    else
    {
        AUD_SupportSRS(FALSE);
    }

    if(AUD_LoadMpeg4Flag()==1)
    {
        AUD_SupportMpeg4(TRUE);
    }
    else
    {
        AUD_SupportMpeg4(FALSE);
    }
    
#ifdef CC_AUD_USE_FLASH_AQ
    AUD_FlashAQInit();
#endif
#ifdef SUPPORT_FLASH_AQ
    AUD_FlashAQInitEx();
#endif

    #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
    AUD_WRITE32(REG_AOUT_DRAM_CFG, AOUT2_USE_AOUT1_CLK | (0x1F<<ADD_DEC2_MIX_POS)); //enable MixSnd2 mix to AOUT1 10 CH + AOUT2 use AOUT1 clock
    #endif

#ifdef DSP_TEST_PSRDMA
    PARSER_WRITE32(REG_PAR_CTRL, 0);          // Reset parser
    PARSER_WRITE32(REG_PAR_CTRL, 0x3);        // Enable parser
#endif
    AUD_HwInit();

#ifdef CC_AUD_USE_NVM
    AUD_NVM_Read();
#endif

    AUD_DRVInit();
    AUD_DspEqCfgInit();
#ifdef CC_AUD_SOUND_MODE
    AUD_DspPSMCfgInit();
#endif
#ifdef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
    AUD_DspVSMCfgInit();
#endif
    AUD_DspChDelayInit();
    AUD_DspSpdifInit();
    AUD_DspVolumeInit();
    AUD_DspAsrcInit();
    AUD_DspSetCapability();

    _AudIfFixWarning();
            
    //FIXME //busy waiting until init finished
    if (!_fgAudInit)
    {
        while (AUD_DRVGetAudioState(AUD_DEC_MAIN) != AUD_IDLE)  
        {   
            u4Index++;
            x_thread_delay(10);
           
            if (u4Index > 500)
            { 
                LOG(1, "DSP NOT POWER ON\n");
                ASSERT(0);
                //  return AUD_FAIL;
            }
        }

        _fgAudInit = TRUE;
    }

    _fgAudInitOK = TRUE;

#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
    AUD_EnableMixSndClip();
#endif
    
    VERIFY(x_sema_unlock(_hSemaAudInit) == OSR_OK);
    return AUD_OK;
}

//-----------------------------------------------------------------------------
// AUD_IsAudInit
//
/** Query audio initialization status.
 *
 *  @retval TRUE    Audio is initialized
 *  @retval FALSE   Audio is not initialized
 */
//-----------------------------------------------------------------------------
BOOL AUD_IsAudInit(void)
{
    return _fgAudInit;
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
    AUD_DEC_ID_VALIDATE(u1DecId);
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
    AUD_DEC_ID_VALIDATE(u1DecId);
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
    INT32 i4Ret;
    LOG(5,"CMD: set DecType: Dec(%d) StreamFrom(%d) Fmt(%d)\n",
        u1DecId, (UINT8)eStreamFrom, (UINT8)eDecFmt);

    AUD_DEC_ID_VALIDATE(u1DecId);
    
    VERIFY(x_sema_lock(_hSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    VERIFY(x_sema_lock(_hAsynCmdSema, X_SEMA_OPTION_WAIT) == OSR_OK);
	
#ifndef __MODEL_slt__
    #ifdef CC_DYNAMIC_POWER_ONOFF
    //ADC Power down/on
    #endif
#endif    

#if 1 //Use LPCM decoder if enable this code.
    if (eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
    {
#ifndef CC_AUD_SKYPE_SUPPORT    
        if (eDecFmt==AUD_FMT_PCM)
#else
        if ((eDecFmt==AUD_FMT_PCM)&&(u1DecId==AUD_DEC_MAIN)) 
#endif   
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
        // Try lock to make sure the semaphore was locked
        i4Ret = x_sema_lock(_hCmdWaitSema[u1DecId], X_SEMA_OPTION_NOWAIT);    
        VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
    
    	VERIFY(AUD_DRVSetDecodeType(u1DecId, eStreamFrom, eDecFmt));
    	AUD_WaitCommandDone(u1DecId, AUD_CMD_LOADCODE);
    }
    else
    {
        // Try lock to make sure the semaphore was locked
        i4Ret = x_sema_lock(_hCmdWaitSema[u1DecId], X_SEMA_OPTION_NOWAIT);    
        VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
		
		UNUSED(AUD_DRVChangeDecodeFormat(u1DecId, eStreamFrom, eDecFmt));
		AUD_WaitCommandDone(u1DecId,AUD_CMD_CHANGE_FORMAT);
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
    
    AUD_DEC_ID_VALIDATE(u1DecId);
    
    VERIFY(x_sema_lock(_hSema, X_SEMA_OPTION_WAIT) == OSR_OK);

#ifdef CC_MT5391_AUD_3_DECODER  //gallen 0713        
  #if 1//def ENABLE_AUDIO_RECORD_SUPPORT
    if ((u1DecId == AUD_DEC_THIRD) && !(AUD_Dec3_Is_Mp3Enc()))
  #else
    if (u1DecId == AUD_DEC_THIRD)
  #endif
    {
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
    LOG(5,"CMD: set Play Asyn: Dec(%d)\n", u1DecId);

    AUD_DEC_ID_VALIDATE(u1DecId);

#ifdef CHANNEL_CHANGE_LOG
    HAL_TIME_T dt;
    HAL_GetTime(&dt);
    LOG(0, " %u.%06u s [AV SYNC] 3 MW aud play\n", dt.u4Seconds, dt.u4Micros);
#endif
    
    VERIFY(x_sema_lock(_hSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    VERIFY(x_sema_lock(_hAsynCmdSema, X_SEMA_OPTION_WAIT) == OSR_OK);
	
#ifdef CC_MT5391_AUD_3_DECODER  //gallen 0521        
  #if 1//def ENABLE_AUDIO_RECORD_SUPPORT
    if ((u1DecId == AUD_DEC_THIRD) && (!AUD_Dec3_Is_Mp3Enc()))
  #else
    if (u1DecId==AUD_DEC_THIRD)
  #endif
    {
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
    LOG(5,"CMD: set Resume: Dec(%d)\n", u1DecId);

    AUD_DEC_ID_VALIDATE(u1DecId);

    VERIFY(x_sema_lock(_hSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    
    if (AUD_IsDecoderPlay(u1DecId))
    {
        _afgDecPause[u1DecId] = FALSE;
        VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_RESUME));
    }
    else
    {
        VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_PLAY));
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
    LOG(5,"CMD: set Pause: Dec(%d)\n", u1DecId);

    AUD_DEC_ID_VALIDATE(u1DecId);
    
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
    AUD_DEC_ID_VALIDATE(u1DecId);
    
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

    LOG(5,"CMD: set Stop: Dec(%d)\n", u1DecId);
        
    AUD_DEC_ID_VALIDATE(u1DecId);
    
    VERIFY(x_sema_lock(_hSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    // Try lock to make sure the semaphore was locked
    i4Ret = x_sema_lock(_hCmdWaitSema[u1DecId], X_SEMA_OPTION_NOWAIT);    
    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));

    VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_STOP));
    AUD_WaitCommandDone(u1DecId, AUD_CMD_STOP);
#ifdef CC_MT5391_AUD_3_DECODER  //gallen 0713        
  #if 1//def ENABLE_AUDIO_RECORD_SUPPORT
    if ((u1DecId == AUD_DEC_THIRD) && (!AUD_Dec3_Is_Mp3Enc()))
  #else
    if (u1DecId==AUD_DEC_THIRD)
  #endif
    {
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
    LOG(5,"CMD: set Stop Asyn: Dec(%d)\n", u1DecId);

    AUD_DEC_ID_VALIDATE(u1DecId);
    
    VERIFY(x_sema_lock(_hSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    VERIFY(x_sema_lock(_hAsynCmdSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_STOP));
#ifdef CC_MT5391_AUD_3_DECODER  //gallen 0521        
  #if 1//def ENABLE_AUDIO_RECORD_SUPPORT
    if ((u1DecId == AUD_DEC_THIRD) && (!AUD_Dec3_Is_Mp3Enc()))
  #else
    if (u1DecId==AUD_DEC_THIRD)
  #endif
    {
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
    
    AUD_DEC_ID_VALIDATE(u1DecId);
    
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
    LOG(5,"CMD: set Reset Asyn: Dec(%d)\n", u1DecId);

    AUD_DEC_ID_VALIDATE(u1DecId);
    
    VERIFY(x_sema_lock(_hSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    VERIFY(x_sema_lock(_hAsynCmdSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_STOP));
    VERIFY(x_sema_unlock(_hSema) == OSR_OK);    
    
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
//-----------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------
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
// AUD_IsDecoderStop
//
/** Brief of AUD_IsDecoderStop.
 *  Details of AUD_IsDecoderStop (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)   
 *
 *  @retval     TRUE	        
 *  @retval     FALSE
*/
BOOL AUD_IsDecoderStop(UINT8 u1DecId)
{
	AUD_DRV_STATE_T eDrvState;
	eDrvState = AUD_DRVGetAudioState(u1DecId);
	return ( (eDrvState == AUD_WAIT_STOP) ||(eDrvState == AUD_WAIT_STOPSTREAM)||(eDrvState ==AUD_STOPPED))? TRUE: FALSE;
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

// AUD_GetAudioPTS
//
/** Brief of AUD_GetAudioPTS.
 *  Details of AUD_GetAudioPTS (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      prAudioPts       AudioPts   
 */
//-----------------------------------------------------------------------------
void AUD_GetAudioPTS(UINT8 u1DecId, UINT32 * rAudioPts )
{
    UNUSED(AUD_GetAudioPts(u1DecId, rAudioPts));
}

//-----------------------------------------------------------------------------
// AUD_GetStreamFrom
//
/** Brief of AUD_GetStreamFrom.
 *  Details of AUD_GetStreamFrom (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      peStreamFrom     Stream source 
 */
//-----------------------------------------------------------------------------
void AUD_GetStreamFrom(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T *peStreamFrom)
{
     UNUSED(AUD_DRVGetStreamFrom(u1DecId,  peStreamFrom));
}

#ifdef  LINUX_TURNKEY_SOLUTION
#if 1
//-----------------------------------------------------------------------------
// AudLoadAudioClip
//
/** Brief of AudLoadAudioClip.
 *  Details of AudLoadAudioClip (optional).
 *
 *  @param      u1DecId                   Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      u1BufferSize             ClipBufferSize
 *  @param      u1ClipBufferPointer   u1ClipBufferPointer
 *  @param      u1DecFmt                 DecFmt
 *  @param      u1ReaptNumber        ReaptNumber
 */
//-----------------------------------------------------------------------------
void AudLoadAudioClip(UINT8 u1DecId, UINT32 u1BufferSize, UINT32 u1ClipBufferPointer, AUD_FMT_T u1DecFmt, UINT32 u1ReaptNumber )
{
    
	AUD_CLIP_TYPE_T u1AudClip;
	u1AudClip.fgPlayTestTone = FALSE;
	u1AudClip.u4OutputChlSelect=0;
	u1AudClip.u4ToneFreq=1000;
    u1AudClip.eAudFmt = u1DecFmt;
	u1AudClip.eMemBuf.u4LoopCount=1;
	u1AudClip.u4RepeatCount=u1ReaptNumber; 
	u1AudClip.u4AudCtrlTypeAddr =0;

    u1Clip_buf = (void *)x_mem_alloc(u1BufferSize);
    u1Clipfg = 1;
    if (u1Clip_buf == NULL)
    {
        LOG(0, "allocate memory for AUD_CLIP_BUFF fail!\n");
        ASSERT(0); 
		return ;
    }
    rm_copy_from_user(u1Clip_buf, (UINT8 *)u1ClipBufferPointer,
          u1BufferSize);
    u1AudClip.eMemBuf.pData = u1Clip_buf;
	
    u1AudClip.eMemBuf.u4Length = u1BufferSize;
    //u1AudClip.u4RepeatCount = prClip->ui4_repeat_cnt;
    //u1AudClip.u4OutputChlSelect = prClip->ui4_ch_mask;
    //u1AudClip.u4AudCtrlTypeAddr = (UINT32)AUD_DEC_CTRL_PLAY_AUD_CLIP;
    x_memcpy((VOID *)VIRTUAL((UINT32)&_u1AudClip[u1DecId]), &u1AudClip, sizeof(AUD_CLIP_TYPE_T));
	AUD_SetAudClip(u1DecId, &_u1AudClip[u1DecId]);
}
#else
//-----------------------------------------------------------------------------
// AudLoadAudioClip
//
/** Brief of AudLoadAudioClip.
 *  Details of AudLoadAudioClip (optional).
 *
 *  @param      u1DecId                   Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      u1BufferSize             ClipBufferSize
 *  @param      u1ClipBufferPointer   u1ClipBufferPointer
 */
//-----------------------------------------------------------------------------

void AudLoadAudioClip(UINT8 u1DecId, UINT32 u1BufferSize, UINT32 u1ClipBufferPointer)
{
    
	AUD_CLIP_TYPE_T u1AudClip;
	u1AudClip.fgPlayTestTone = FALSE;
	u1AudClip.u4OutputChlSelect=0;
	u1AudClip.u4ToneFreq=1000;
    u1AudClip.eAudFmt = AUD_FMT_PCM;
#ifdef  LINUX_TURNKEY_SOLUTION
    u1Clip_buf = (void *)x_mem_alloc(u1BufferSize);
    u1Clipfg = 1;
    if (u1Clip_buf == NULL)
    {
        LOG(0, "allocate memory for AUD_CLIP_BUFF fail!\n");
        ASSERT(0);
    }
    rm_copy_from_user(u1Clip_buf, (UINT8 *)u1ClipBufferPointer,
          u1BufferSize);
    u1AudClip.eMemBuf.pData = u1Clip_buf;
	
#else
    //u1AudClip.eMemBuf.pData = prClip->u.t_clip_buff.pui1_clip_buff;
#endif
    u1AudClip.eMemBuf.u4Length = u1BufferSize;
    //u1AudClip.u4RepeatCount = prClip->ui4_repeat_cnt;
    //u1AudClip.u4OutputChlSelect = prClip->ui4_ch_mask;
    //u1AudClip.u4AudCtrlTypeAddr = (UINT32)AUD_DEC_CTRL_PLAY_AUD_CLIP;
    x_memcpy((VOID *)VIRTUAL((UINT32)&_u1AudClip[u1DecId]), &u1AudClip, sizeof(AUD_CLIP_TYPE_T));
	AUD_SetAudClip(u1DecId, &_u1AudClip[u1DecId]);

}
#endif
#endif

extern UINT32 u4GetUploadFIFOStart(void);
extern UINT32 u4GetUploadFIFOEnd(void);
extern UINT32 u4GetUploadWritePnt(void);

void AUD_RecordPCM(UINT32 u4BufferSize, UINT32 u4ClipBufferPointer)
{
    UINT32 u4Start,u4End,u4WP,u4RP,u4SendSize,u4TotalSendSize;
    
    // 0. Enable Upload Buffer
    AUD_DspSetUploadMode(4);
    AUD_DspUploadDataEnable();

    // 1. Wait WP exceeds the u4BufferSize
    u4Start = u4GetUploadFIFOStart();
    u4End = u4GetUploadFIFOEnd();
    Printf("u4Start: 0x%08x u4End: 0x%08x\n", u4Start, u4End);
    u4RP = u4Start;
    u4TotalSendSize = 0;

    // 2. Copy to user
    while (u4BufferSize)
    {        
        u4WP = u4GetUploadWritePnt();
        if (u4BufferSize > 0x1000)
        {
            u4SendSize = 0x1000;
        }
        else
        {
            u4SendSize = u4BufferSize;
        }
        if (((u4WP >= u4RP) && ((u4WP - u4RP) >= u4SendSize)) ||
            ((u4WP < u4RP) && ((u4End - u4Start - u4RP + u4WP) >= u4SendSize)))
        {
            Printf(".");
            #ifdef  LINUX_TURNKEY_SOLUTION            
            rm_copy_to_user((VOID*)(u4ClipBufferPointer+u4TotalSendSize),(VOID*)VIRTUAL(u4RP),u4SendSize);
            #endif
            u4BufferSize -= u4SendSize;
            u4TotalSendSize += u4SendSize;
            u4RP += u4SendSize;
            if (u4RP == u4End)
            {
                u4RP = u4Start;
            }
        }
        else
        { 
            x_thread_delay(10);
        }
        //Printf("u4BufferSize: 0x%08x u4ClipBufferPointer: 0x%08x\n", u4BufferSize, u4ClipBufferPointer);        
    }

    // 3. Disable Upload
    AUD_DspSetUploadMode(0);
    AUD_DspUploadDataEnable();    
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
// AUD_PlayAudClipWithRPN
//
/** Brief of AUD_PlayAudClipWithRPN.
 *  Details of AUD_PlayAudClipWithRPN (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      u4Channel      
 *  @param      ReaptNumber   
 *  @param      fgPlayTestTone   
 */
void AUD_PlayAudClipWithRPN(UINT8 u1DecId, UINT32 u4Channel, UINT32 ReaptNumber, BOOL fgPlayTestTone)
{
	
	AUD_SentReaptNumber(u1DecId,  ReaptNumber);
	AUD_PlayAudClip( u1DecId,  u4Channel,  fgPlayTestTone );
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

    AUD_DEC_ID_VALIDATE(u1DecId);
    
    x_memset((VOID *)VIRTUAL((UINT32)u4ClipChlVolumn), 0, sizeof(UINT8) * (UINT8)AUD_CH_NUM);
    
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

    LOG(5,"CMD: set Stop Aud Clip: Dec(%d)\n", u1DecId);
    
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

// AUD_DeleteAudClip

//-----------------------------------------------------------------------------
void AUD_DeleteAudioClip(void)
{
#ifdef  LINUX_TURNKEY_SOLUTION
	x_mem_free(u1Clip_buf);
#endif
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

// AUD_AvInputMuxSelByAD
//
/** Brief of AUD_AvInputMuxSelByAD.
 *  Details of AUD_AvInputMuxSelByAD(optional).
 *
 *  @param  UINT8 eADNum
 *
 */
//-----------------------------------------------------------------------------
void AUD_AvInputMuxSelByAD(UINT8 eADNum)
{
    AUD_AvMuxSelByAD(eADNum);
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

    LOG(5,"CMD: set Mute: Dec(%d)\n", u1DecId);
    
    AUD_DEC_ID_VALIDATE(u1DecId);
    
    AUD_DspMuteEnable(u1DecId, TRUE);

#ifdef CC_MT5391_AUD_3_DECODER
    if ((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX)) 
#endif    
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

    LOG(5,"CMD: set UnMute: Dec(%d)\n", u1DecId);
    
    AUD_DEC_ID_VALIDATE(u1DecId);
    
    AUD_DspMuteEnable(u1DecId, FALSE);

#ifdef CC_MT5391_AUD_3_DECODER
    if ((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX)) 
#endif    
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
    AUD_DEC_ID_VALIDATE(u1DecId);
    
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
        case FS_6K:
            rLpcmSetting.u1LpcmFreqGrp1 = 21; //LPCM_6K
            break;        
        case FS_8K:
            rLpcmSetting.u1LpcmFreqGrp1 = 20;
            break;        
        case FS_12K:
            rLpcmSetting.u1LpcmFreqGrp1 = 4;
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
        case FS_176K:
            rLpcmSetting.u1LpcmFreqGrp1 = 10;
            break;                            
        case FS_192K:
            rLpcmSetting.u1LpcmFreqGrp1 = 2;
            break; 
        case FS_88K:
            rLpcmSetting.u1LpcmFreqGrp1 = 9;
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
        else if (prLPcmInfo->ui2_bits_per_sample == 24) //24 bit PCM comes from WAV
        {
            rLpcmSetting.u1LpcmBitResGrp1 = 2;
        }        
        else if (prLPcmInfo->ui2_bits_per_sample == 32) //32 bit PCM comes from WAV
        {
            rLpcmSetting.u1LpcmBitResGrp1 = 4;
        }                
        else
        {
            rLpcmSetting.u1LpcmBitResGrp1 = 0;     //default is 16 bit.
        }

        
        // channel assignment
        rLpcmSetting.u1LpcmChAssignment = (prLPcmInfo->ui2_num_ch-1);  
        
        // decoding type
        if (prLPcmInfo->eLPcmType == LPCM_TYPE_MS_ADPCM)
        {
            rLpcmSetting.u2LpcmDecodingType = 0x11;
            rLpcmSetting.u2ADPCMDecodingType = 0x11;
            // block align
            rLpcmSetting.u2ADPCMBlockAlign = prLPcmInfo->ui2_block_align;            
        }
        else if (prLPcmInfo->eLPcmType == LPCM_TYPE_IMA_ADPCM)
        {
            #if 0  //After MW can set correct TYPE, remove this.
            rLpcmSetting.u1LpcmDecodingType = 0x11;
            rLpcmSetting.u1ADPCMDecodingType = 0x11;
            // block align
            rLpcmSetting.u2ADPCMBlockAlign = prLPcmInfo->ui2_block_align;
            #else  //This is correct IMA ADPCM setting.
            rLpcmSetting.u2LpcmDecodingType = 0x31;
            rLpcmSetting.u2ADPCMDecodingType = 0x31;
            // block align
            rLpcmSetting.u2ADPCMBlockAlign = prLPcmInfo->ui2_block_align*2;
            #endif
        }
        else if ((prLPcmInfo->eLPcmType == LPCM_TYPE_A_LAW)||(prLPcmInfo->eLPcmType == LPCM_TYPE_U_LAW))
        {
            rLpcmSetting.u2LpcmDecodingType = 0x101;
            if (prLPcmInfo->eLPcmType == LPCM_TYPE_U_LAW)
            {
                rLpcmSetting.u2LpcmDecodingType |= 0x200;
            }
            // block align
            rLpcmSetting.u2ADPCMBlockAlign = prLPcmInfo->ui2_block_align;     
            rLpcmSetting.u2ADPCMDecodingType = 0;
        }        
        else
        {
            if ((prLPcmInfo->b_big_endian) || (prLPcmInfo->ui2_bits_per_sample == 8)) //8 bit PCM should inverse byte??
            {    
                rLpcmSetting.u2LpcmDecodingType = 0x5;
                if (prLPcmInfo->b_signed_data)
                {
                    rLpcmSetting.u2LpcmDecodingType |= 0x80; //bit7 : Signed 8 bit 
                }                            
            }
            else
            {
                rLpcmSetting.u2LpcmDecodingType = 0xd;
            }
            
            if (prLPcmInfo->ui2_bits_per_sample == 24)
            {
                rLpcmSetting.u2LpcmDecodingType |= 0x2; //24 bit PCM comes from WAV
            }
            
            rLpcmSetting.u2ADPCMDecodingType = 0;
            // block align
            rLpcmSetting.u2ADPCMBlockAlign = prLPcmInfo->ui2_block_align;
        }
        
        AUD_SetLpcmTable(u1DecId, rLpcmSetting);
    }
}

//-----------------------------------------------------------------------------
// AUD_Ac3Setting
//
/** AC3 format setting
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      prPcmSetting   
 */
//-----------------------------------------------------------------------------
void AUD_Ac3Setting(UINT8 u1DecId, const AUD_AC3_SETTING_T * prAC3Setting)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    
    if (prAC3Setting != NULL)
    {
        AUD_DrvAc3Setting(u1DecId, prAC3Setting);
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

//-----------------------------------------------------------------------------
// AUD_GetMultimediaMode
//
/**Get Audio Multimedia Mode
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN)  
 *  @retval      Multimedia mode, 0:Normal 1: Karaoke
 */
//-----------------------------------------------------------------------------
AUD_MM_MODE_T AUD_GetMultimediaMode(UINT8 u1DecId)
{
   return AUD_DrvGetMultimediaMode(u1DecId);   
}

//-----------------------------------------------------------------------------
// AUD_SetMultimediaMode
//
/** Set Audio Multimedia Mode
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN)  
 *  @param      eMultimediaMode  Multimedia mode, 0: Normal 1: Karaoke
 *                                                2: Karaoke mic pass
 */
//-----------------------------------------------------------------------------
void AUD_SetMultimediaMode(UINT8 u1DecId, AUD_MM_MODE_T eMultimediaMode)
{
    LOG(5,"CMD: set MultimediaMode: Dec(%d) Mode(%d)\n",
        u1DecId, (UINT8)eMultimediaMode);
    AUD_DrvSetMultimediaMode(u1DecId, eMultimediaMode);
}

//-----------------------------------------------------------------------------
// AUD_SetAdMode
//
/** Set AD Mode
 *
 *  @param      fgAdMode  0: AD will not be turned on in this connection, 1: AD will be turned on
 */
void AUD_SetAdMode(BOOL fgAdMode)
{
    AUD_DrvSetAdMode(fgAdMode);
}

void _AUD_OutPadEnable(UINT8 u1DecId, BOOL fgEnable)
{
    AUD_OutPadEnable(u1DecId, fgEnable);
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

void AUD_AtunerSigStable(void)
{
    // FIXME
    // Should porting from 71 later
    AUD_DrvATunerLock();
}

 void Aud_SetTvSystemMask(UINT8 u1DecId, UINT32 u4Mask)
{
    _AudSetTvSysMask(u1DecId,  u4Mask);
}

#ifdef CC_SUPPORT_AUTO_DETECT_AUD_SYSTEM
  void Aud_SetTvAudSystemMask(UINT8 u1DecId, UINT32 u4TvMask, UINT32 u4TvAudMask, ISO_3166_COUNT_T country_code)
 {
     _AudSetTvAudSysMask(u1DecId,  u4TvMask, u4TvAudMask, country_code);
 }
#endif

 UINT32 Aud_GetTvSystemMask(UINT8 u1DecId)
{
   return  _AudGetTvSysMask(u1DecId);
}

 void Aud_SetDspSpeed(UINT8 u1DecId,UINT16 u2Speed)
{
   AUD_DspSpeed(u1DecId,u2Speed);
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
static BOOL _fgAudMwRealCtrl[AUD_DEC_MAX] = {0, 0, 0, 0};
    
void AUD_MWCtrl_Set(UINT8 u1DecId,BOOL fgPlay)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    VERIFY(x_sema_lock(_hMwCtrlSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    _fgAudMwRealCtrl[u1DecId] = fgPlay;
    
    VERIFY(x_sema_unlock(_hMwCtrlSema) == OSR_OK);    
}

BOOL AUD_MWCtrl_Get(UINT8 u1DecId)
{
    BOOL fgRet = 0;

    AUD_DEC_ID_VALIDATE(u1DecId);

    VERIFY(x_sema_lock(_hMwCtrlSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    fgRet = _fgAudMwRealCtrl[u1DecId];
    
    VERIFY(x_sema_unlock(_hMwCtrlSema) == OSR_OK);
    
    return fgRet;
}
void AUD_SetLPCMFmt(UINT8 u1DecId, UINT32 u4PriData)
{   // This function is for Program Stream Only. 
    
    AUD_LPCM_SETTING_T rLpcmSetting;

    /*======================================================================================
    *               u4PriData (This is Specification)
    *
    *   [ Program Stream ]
    * sampling_freq         5   0x1F000000 (>>24) (internal definition)
    *
    * audio_emphasis_flag   1   0x800000 (>>23)
    * audio_mute_flag       1   0x400000 (>>22)
    * reserved              1   0x200000 
    * audio_frame_number    5   0x1F0000 (>>16)
    *
    * quantization_word_len 2   0xC000   (>>14) (0 = 16bit, 1 = 20bit, 2 = 24bit,  3 = 8bit,  4 = 32bit(new, only from WAV))
    * sampling_freq         2   0x3000   (>>12) (0 = fs_48, 1 = fs_96, DVD spec.)   
    *                                           (if == 3, then check bit28 ~ bit 24, internal definition)
    * reserved              1   0x0800   
    * num of audio channels 3   0x0700   (>>8)
    *
    * drc control           8   0xFF
    *
    *
    *   [ Transport Stream ]
    * num of audio channels 4   0x0F000  (>>12) (0 = reserved. 1 = 2ch mono, 2 = reserverd,  3 = 2ch stereo)
    * sampling_freq         4   0x0F00   (>>8)  (1 = fs_48, 4 = fs_96, 5 = fs_192)
    * quantization_word_len 2   0x00C0   (>>6)  (1 = 16bit, 2 = 20bit, 3 = 24bit)
    * 
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
    BOOL  fgTypePS = TRUE;
    UINT32 u4SwDmxType;
    UINT8 u1SwDmxSrcId;

    UNUSED(u1QuantWord);
    UNUSED(u1Drc);
    UNUSED(u1SampRate);
    UNUSED(u1Channels);
    UNUSED(fgTypePS);
    UNUSED(u4SwDmxType);
    UNUSED(u1SwDmxSrcId);

    AUD_GET_SWDMXSrcId(u1DecId, &u1SwDmxSrcId);
#ifdef ENABLE_MULTIMEDIA               
#ifdef CC_53XX_SWDMX_V2 
    SWDMX_GetInfo(u1SwDmxSrcId, eSWDMX_CONTAINER_TYPE, &u4SwDmxType,0,0);
    if ( (u4SwDmxType == SWDMX_FMT_MPEG2_TS)||(u4SwDmxType == SWDMX_FMT_MPEG2_TS_192)
         ||(u4SwDmxType == SWDMX_FMT_MPEG2_TS_ZERO_192)||(u4SwDmxType == SWDMX_FMT_MPEG2_TS_ENCRYPT_192)
         ||(u4SwDmxType == SWDMX_FMT_MPEG2_TS_TIME_SHIFT) )
    {
        fgTypePS = FALSE;
    }
#endif //CC_53XX_SWDMX_V2
#endif //ENABLE_MULTIMEDIA

#if 1
    // if (fgTypePS)
    {
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
    }
#else
    //else // not PS, it's TS
    {
        u1Channels = (u4PriData & (0x0F000)) >> 12;
        if (u1Channels == 1)
        {
            u1Channels = 0; //mono
        }
        else    
        {
            u1Channels = 1; //stereo
        }

        u1SampRate = (u4PriData & (0x0F00)) >> 8;
        if (u1SampRate == 1)
        {
            u1SampRate = 0; //48k
        }    
        rLpcmSetting.u1LpcmFreqGrp1 = 0;
        rLpcmSetting.u1LpcmFreqGrp2 = u1SampRate;        
    
        u1QuantWord = (u4PriData & (0x0C0)) >> 6;
        u1QuantWord -= 1;
    }
#endif

    //LOG(9,"Q=0x%x, S=0x%x, C=0x%x, D=0x%x\n", u1QuantWord, u1SampRate, u1Channels, u1Drc);
    
    rLpcmSetting.u1LpcmBitShift = 0;    
    rLpcmSetting.u1LpcmDrcValue = 0x80;    
    rLpcmSetting.u1LpcmBitResGrp2 = 0;        
    rLpcmSetting.u1LpcmDrcFlag = 0; 
    
    rLpcmSetting.u2LpcmDecodingType = 0x5; // big endian, non-wav, vob       
    rLpcmSetting.u1LpcmChAssignment = u1Channels;        
    rLpcmSetting.u1LpcmBitResGrp1 = u1QuantWord;            
    rLpcmSetting.u2ADPCMDecodingType = 0;
        
    AUD_SetLpcmTable(u1DecId, rLpcmSetting);                    
}

void AUD_SetLPCMFmtPS(UINT8 u1DecId, UINT32 u4PriData)
{   // This function is for Program Stream Only. 
    
    AUD_LPCM_SETTING_T rLpcmSetting;

    /*======================================================================================
    *               u4PriData (This is Specification)
    *
    *   [ Program Stream ]
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
    *
    *   [ Transport Stream ]
    * num of audio channels 4   0x0F000  (>>12) (0 = reserved. 1 = 2ch mono, 2 = reserverd,  3 = 2ch stereo)
    * sampling_freq         4   0x0F00   (>>8)  (1 = fs_48, 4 = fs_96, 5 = fs_192)
    * quantization_word_len 2   0x00C0   (>>6)  (1 = 16bit, 2 = 20bit, 3 = 24bit)
    * 
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


    {
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
    }

    //LOG(9,"Q=0x%x, S=0x%x, C=0x%x, D=0x%x\n", u1QuantWord, u1SampRate, u1Channels, u1Drc);
    
    rLpcmSetting.u1LpcmBitShift = 0;    
    rLpcmSetting.u1LpcmDrcValue = 0x80;    
    rLpcmSetting.u1LpcmBitResGrp2 = 0;        
    rLpcmSetting.u1LpcmDrcFlag = 0; 
    
    rLpcmSetting.u2LpcmDecodingType = 0x5; // big endian, non-wav, vob       
    rLpcmSetting.u1LpcmChAssignment = u1Channels;        
    rLpcmSetting.u1LpcmBitResGrp1 = u1QuantWord;            
    rLpcmSetting.u2ADPCMDecodingType = 0;
        
    AUD_SetLpcmTable(u1DecId, rLpcmSetting);                    
}

void AUD_SetLPCMFmtTS(UINT8 u1DecId, UINT32 u4PriData)
{   // This function is for Program Stream Only. 
    
    AUD_LPCM_SETTING_T rLpcmSetting;

    /*======================================================================================
    *               u4PriData (This is Specification)
    *
    *   [ Program Stream ]
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
    *
    *   [ Transport Stream ]
    * num of audio channels 4   0x0F000  (>>12) (0 = reserved. 1 = 2ch mono, 2 = reserverd,  3 = 2ch stereo)
    * sampling_freq         4   0x0F00   (>>8)  (1 = fs_48, 4 = fs_96, 5 = fs_192)
    * quantization_word_len 2   0x00C0   (>>6)  (1 = 16bit, 2 = 20bit, 3 = 24bit)
    * 
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

    UNUSED(u1QuantWord);
    UNUSED(u1SampRate);
    UNUSED(u1Channels);

    {
        u1Channels = (u4PriData & (0x0F000)) >> 12;
#if 0        
        if (u1Channels == 1)
        {
            u1Channels = 0; //mono
        }
        else    
        {
            u1Channels = 1; //stereo
        }
#else
        switch (u1Channels)
        {
        case 1:
            u1Channels = 0; //mono            
            break;
        case 2:
        case 3:
            u1Channels = 1; //stereo
            break;            
        case 4:
        case 5:
        case 6:
        case 7:            
            u1Channels = 3; // 4 ch
            break;
        case 8:
        case 9:
            u1Channels = 5; // 6 ch
            break;            
        case 10:
        case 11:
            u1Channels = 7; // 8 ch
            break;
        default:
            u1Channels = 1; //stereo            
            break;        
        }
#endif

        u1SampRate = (u4PriData & (0x0F00)) >> 8;
        if (u1SampRate == 1)
        {
            u1SampRate = 0; //48k
        }    
        rLpcmSetting.u1LpcmFreqGrp1 = 0;
        rLpcmSetting.u1LpcmFreqGrp2 = u1SampRate;        
    
        u1QuantWord = (u4PriData & (0x0C0)) >> 6;
        u1QuantWord -= 1;
    }

    //LOG(9,"Q=0x%x, S=0x%x, C=0x%x, D=0x%x\n", u1QuantWord, u1SampRate, u1Channels, u1Drc);
    
    rLpcmSetting.u1LpcmBitShift = 0;    
    rLpcmSetting.u1LpcmDrcValue = 0x80;    
    rLpcmSetting.u1LpcmBitResGrp2 = 0;        
    rLpcmSetting.u1LpcmDrcFlag = 0; 
    
    rLpcmSetting.u2LpcmDecodingType = 0x5; // big endian, non-wav, vob       
    rLpcmSetting.u1LpcmChAssignment = u1Channels;        
    rLpcmSetting.u1LpcmBitResGrp1 = u1QuantWord;            
    rLpcmSetting.u2ADPCMDecodingType = 0;
        
    AUD_SetLpcmTable(u1DecId, rLpcmSetting);                    
}


BOOL AUD_IsLpcmFmtPS(void)
{
    BOOL fgFlag = FALSE;

    // if LPCM from program stream ?
    fgFlag = _fgLpcmTpyePS;

    return fgFlag;
}

void AUD_PCM_Detect_Raw_Mute(BOOL fgFlag)
{
    _fgPCMDetectRawMute = fgFlag;
}

BOOL AUD_Is_PCMDetectRaw(void)
{
	return _fgPCMDetectRawMute;
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
    if (_bfgMMDecFmtScramble[u1DecId] != fgFlag)
    {
        _bfgMMDecFmtScramble[u1DecId] = fgFlag;
        VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_AVSYNC));
    }
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
// AUD_MMClearEOSFlag
//
/** Clear Audio Multimedia EOS Flag
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN)  
 */
//-----------------------------------------------------------------------------
void AUD_MMClearEOSFlag(UINT8 u1DecId)
{
    _AudClearEOSFlag(u1DecId);
}

//-----------------------------------------------------------------------------
// AUD_MMClearPesCnt
//
/** Clear Audio Multimedia Pes Count
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN)  
 */
//-----------------------------------------------------------------------------
void AUD_MMClearPesCnt(UINT8 u1DecId)
{
    AUD_DrvClearPesCnt(u1DecId);
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
#define DRAM_QMIX_ADDR 0xe5f
INT32 AUD_MTS_CFO_IDX(void)
{   
    UINT32 u4Qmix, u4DcVal, u4SndSys, eDec2Type, u4SndSysAddr;
    INT32 i4ShiftBlocks;
    AUD_DEC_STREAM_FROM_T eStreamFrom;
    AUD_FMT_T eDecType;

    AUD_GetDecodeType(AUD_DEC_MAIN, &eStreamFrom, &eDecType);

    DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);     
    
    //eDec2Type = DSP_CommDramRead(ADDR_D2RC_DEC2RAM_CODE_VERSION) & 0x0F00; // decoder2 version
    eDec2Type = DSP_CommDramRead(0x3) & 0x0F00; // decoder2 version
    if ((eDecType == AUD_FMT_MTS) && AUD_IsDecoderPlay(AUD_DEC_MAIN)) //BTSC decoder 1
    {
        if (!AUD_DspGetCarrierShiftMode(AUD_ATV_DECODER_MTS))          
        {
            LOG(11,"\n\nNot opening carrier shift mode for MTS\n\n");
            return 999;    
        }

        //u4Qmix = u4ReadDspSram(0x2b2);// Q-mixer, used to judge if shifted (0x2aaab0 is no shift)
        u4Qmix = DSP_CommDramRead(DRAM_QMIX_ADDR); //Use dram is more stable than sram. (DSP code releated.)
        
        u4DcVal = u4ReadDspSram(CFO_ACC_PROB_SRAM_ADDR);    
        if (u4DcVal > 0x800000)
        {
            u4DcVal = (u4DcVal ^ 0xffffff) + 1;
            i4ShiftBlocks = (INT32)u4DcVal * -1;
            i4ShiftBlocks = (i4ShiftBlocks - 0x39580) / 0x72b00; 
        }
        else
        {
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
        u4Qmix = DSP_CommDramRead(DRAM_QMIX_ADDR);
        
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
        u4Qmix = DSP_CommDramRead(DRAM_QMIX_ADDR);
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
    
    DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);     
    
    //eDec2Type = DSP_CommDramRead(ADDR_D2RC_DEC2RAM_CODE_VERSION) & 0x0F00; // decoder2 version
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

UINT8 AUD_GetPIPVideoPath(void)
{
    UNUSED(AUD_GetPIPVideoPath);
    return AUD_DrvGetPIPVideoPath();
}

#ifdef CC_AUD_HDMI_SPDIF_CFG    ///[Joel] 20101011
///#define DBG_LVL     2
void AUD_HdmiCfgAnalogOutput(AUD_OUTPUT_STATE u1OutputState)
{
    _u1OutputState = u1OutputState;

    switch(u1OutputState)
    {
        case DigitalMute_AnalogMute:
            AUD_SPDIF_Output_HDMI_Rx(FALSE);
            AUD_SPDIF_Output_Mute(TRUE);
            AUD_DspHdmiMuteAnalogOutput(AUD_DEC_MAIN, TRUE);
            LOG(6,"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
            LOG(6,"**** DigitalMute_AnalogMute **** \n");
            LOG(6,"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
            break;
        case DigitalUnMute_AnalogMute:
            AUD_SPDIF_Output_HDMI_Rx(TRUE);
            AUD_SPDIF_Output_Mute(FALSE);
            AUD_DspHdmiMuteAnalogOutput(AUD_DEC_MAIN, TRUE);
            LOG(6,"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
            LOG(6,"**** DigitalUnMute_AnalogMute **** \n");
            LOG(6,"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
            break;
        case DigitalUnMute_AnalogUnMute:
            AUD_SPDIF_Output_HDMI_Rx(TRUE);
            AUD_SPDIF_Output_Mute(FALSE);
            AUD_DspHdmiMuteAnalogOutput(AUD_DEC_MAIN, FALSE);
            LOG(6,"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
            LOG(6,"**** DigitalUnMute_AnalogUnMute **** \n");
            LOG(6,"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
            break;
        case DigitalMute_AnalogUnMute:
            AUD_SPDIF_Output_HDMI_Rx(FALSE);
            AUD_SPDIF_Output_Mute(TRUE);
            AUD_DspHdmiMuteAnalogOutput(AUD_DEC_MAIN, FALSE);
            LOG(6,"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
            LOG(6,"**** DigitalMute_AnalogUnMute **** \n");
            LOG(6,"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
            break;
        default:
            AUD_SPDIF_Output_HDMI_Rx(FALSE);
            AUD_SPDIF_Output_Mute(FALSE);
            AUD_DspHdmiMuteAnalogOutput(AUD_DEC_MAIN, FALSE);
            break;
    }
}
#endif

UINT32 AUD_GetTargetCountry(void)
{
    UINT32 _u4AudTargetCountry;
    UINT32 u4Ret;

    UNUSED(u4Ret);

#ifdef NEW_COUNTRY_TYPE // MT5365, MT5395
    _u4AudTargetCountry = GetTargetCountryMask(COUNTRY_AUDIO_MASK);
    switch (_u4AudTargetCountry)
    {
    case COUNTRY_AUD_US:
        u4Ret = COUNTRY_US;
        break;
    case COUNTRY_AUD_EU:
        u4Ret = COUNTRY_EU;
        break;        
    case COUNTRY_AUD_TW:
        u4Ret = COUNTRY_TW;
        break;        
    case COUNTRY_AUD_JP:
        u4Ret = COUNTRY_JP;    
        break;        
    case COUNTRY_AUD_BZ:
        u4Ret = COUNTRY_BZ;    
        break;                
    default:
        u4Ret = COUNTRY_EU;
        break;        
    }
    return u4Ret;
    
#else                   // MT5363, MT5388 and old ICs

    _u4AudTargetCountry = GetTargetCountry();
    #ifdef COUNTRY_FROM_EEPROM
    _u4AudTargetCountry &= COUNTRY_AUDIO_MASK;
    _u4AudTargetCountry = _u4AudTargetCountry >> 5;
    #endif  
    return _u4AudTargetCountry;  
#endif // NEW_COUNTRY_TYPE
}

#ifdef CC_AUD_SETUP_MELODY
void AUD_PlaySetupMelody(UINT8 u1DecId, UINT32 u4Channel, UINT32 u4RepeatCount)
{
    AUD_CLIP_TYPE_T rAudClip;
    AUD_SETUP_MELODY_TABLE_T *rAudSetupMelody;
	UINT8 u1Volume;
	
	_fgSetupPlayFinish = FALSE;
	
	x_memset((VOID *)VIRTUAL((UINT32)&rAudClip), 0, sizeof(AUD_CLIP_TYPE_T));
    
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
    u1Volume = DRVCUST_OptGet(eAudioPowerOnMusicVol);
	
	if (rAudSetupMelody == 0)
    {        
        LOG(1,"No setup melody is prepared !!!!\n");
        _fgSetupPlayFinish = TRUE;
        return;
    }
#ifdef  LINUX_TURNKEY_SOLUTION
        pMelody_buf = (void *)x_mem_alloc((UINT32)rAudSetupMelody->u4Size);
        u1Melodyflag = 1;
       if (pMelody_buf == NULL)
       {
            ASSERT(0);
            printf("allocate fail!\n");
       }
#endif

#ifdef  LINUX_TURNKEY_SOLUTION
    x_memcpy((VOID *)VIRTUAL((UINT32)pMelody_buf), (UINT8*)rAudSetupMelody->prAudSetupMelody,
          rAudSetupMelody->u4Size);    
    rAudClip.eMemBuf.pData = pMelody_buf;
#else
	rAudClip.eMemBuf.pData = (UINT8*)rAudSetupMelody->prAudSetupMelody;
#endif
    rAudClip.eMemBuf.u4Length = rAudSetupMelody->u4Size;
	
	AUD_DspSetMelodyVol(u1Volume);
	//_aau1ChannelVolume[AUD_DEC_MAIN][AUD_CH_ALL] = u1Volume;		  
	
	AUD_DspAVCEnable(AUD_DEC_MAIN, 0);
   	_AUD_IgnoreDecNotify(u1DecId, TRUE);
    _AUD_IgnoreSetupPlayNotify(TRUE);
    AUD_SetAudClip(u1DecId, &rAudClip);
    AUD_PlayAudClip(u1DecId, rAudClip.u4OutputChlSelect,
                   rAudClip.fgPlayTestTone);
    _AUD_IgnoreDecNotify(u1DecId, FALSE);
}

UINT8 AUD_GetSetupMelodyVol(void)
{
	return (UINT8)DRVCUST_OptGet(eAudioPowerOnMusicVol);

}
BOOL AUD_IsSetupMelodyFinish(void)
{
    return _fgSetupPlayFinish;
}

void AUD_SetSetupPlayStatue(BOOL fgIsFinish)
{
	_fgSetupPlayFinish = fgIsFinish;
#ifdef  LINUX_TURNKEY_SOLUTION
	if(fgIsFinish ==TRUE)
	{
        if (u1Melodyflag == 1)
        {
            x_mem_free(pMelody_buf);
            u1Melodyflag = 0;
        }
	}
#endif
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

#ifdef SUPPORT_FLASH_AQ

FLASH_AQ_T rAudFlashAQ;
#ifndef AQ_FILE_NAME_TEMP
#define AQ_FILE_NAME_TEMP "/3rd/AQ.bin"
#endif
INT32 AUD_FlashAQReadEx(UINT8 u1AQPartition, UINT16 u2Offset, UINT8 *u1Data, UINT32 u4Length)
{
#if 1
    struct file *file = NULL;//= file_open(AQ_FILE_NAME, O_RDONLY, 0);
    UINT8 i =0;
    UINT64 u8Offset = (UINT64)u2Offset;
    UNUSED(u1AQPartition);

    file = file_open(AQ_FILE_NAME_TEMP, O_RDONLY, 0);
    if(!file)
    {
        file = file_open(AQ_FILE_NAME, O_RDONLY, 0);
    }

    if (!file)
    {
        memset((void*)u1Data, 0, u4Length);
        //return 0;
    }

    u4Length = file_read(file, (void*)u1Data, u4Length, &u8Offset);
    file_close(file);
    return 0;

#else
    //UINT8 u1AQPartition;
    //u1AQPartition = (UINT8)DRVCUST_OptGet(eAudioFlashAQPartition);
    if (u1AQPartition == 0xFF)
    {
        Printf("AudioFlashAQPartition is not defined !!\n");
        return -1;
    }

#if 1 //Daniel, 20110608, For Jun Wang integration.
    return (STORG_SyncRead(u1AQPartition, u2Offset, (VOID*)u1Data, u4Length));
#else
    if (DRVCUST_OptGet(eFlashPqUseNandFalsh))
    {
        return (STORG_SyncNandRead(u1AQPartition, u2Offset, (UINT8*)(VOID*)u1Data, u4Length));    
    }
    else
    {
        return NORSTG_Read(u1AQPartition, u2Offset, (UINT32)u1Data, u4Length);
    }
#endif
#endif
}


#if MULTIPLE_FLASH_AQ
extern UINT32 GetCurrentPanelIndex(void);
extern UINT16 vApiFlashPqGetQtyId(UINT8 bIndex);
#endif

void AUD_FlashAQInitEx(void)
{
    UINT8 u1Partition;
    UINT32 u4ChkSum = 0;
    UINT32 u4Tmp;
    UINT32 u4Size;
#if MULTIPLE_FLASH_AQ
    UINT16 u2FlashAqIndex;
    UINT32 u4Offset;
#endif
    static BOOL fgAQInit = FALSE;

    if(!fgAQInit)
    {
        u1Partition = (UINT8)DRVCUST_OptGet(eAudioFlashAQPartition);
#if MULTIPLE_FLASH_AQ
        u2FlashAqIndex = 0;//vApiFlashPqGetQtyId(GetCurrentPanelIndex());
        u4Size = ((UINT32)(&(rAudFlashAQ.u1ChkSum[3])) - (UINT32)(&(rAudFlashAQ.au1AudId[0])) + 1 - FLASH_AQ_HDR_OFFSET);
        u4Offset = FLASH_AQ_HDR_OFFSET + u2FlashAqIndex * u4Size;
        AUD_FlashAQReadEx(u1Partition, 0, (UINT8*)(&rAudFlashAQ), FLASH_AQ_HDR_OFFSET); //Read 'aqda' and vendor_version ID
        AUD_FlashAQReadEx(u1Partition, u4Offset, (UINT8*)(&rAudFlashAQ)+FLASH_AQ_HDR_OFFSET, u4Size);
        for (u4Tmp = 0;u4Tmp<u4Size-4;u4Tmp++)
        {
            u4ChkSum += *((UINT8*)(&rAudFlashAQ)+FLASH_AQ_HDR_OFFSET+u4Tmp);
        }    
#else
        u4Size = ((UINT32)(&(rAudFlashAQ.u1ChkSum[3])) - (UINT32)(&(rAudFlashAQ.au1AudId[0])) + 1)
        AUD_FlashAQReadEx(u1Partition, 0, (UINT8*)(&rAudFlashAQ), u4Size);
        for (u4Tmp = 0;u4Tmp<u4Size-4;u4Tmp++)
        {
            u4ChkSum += *((UINT8*)(&rAudFlashAQ)+u4Tmp);
        }
#endif

    LOG(0,"u4ChkSum = 0x%x.\n",u4ChkSum);
        LOG(0,"rAudFlashAQ.au1AudId[0] = 0x%x.\n",rAudFlashAQ.au1AudId[0]);
        LOG(0,"rAudFlashAQ.au1AudId[1] = 0x%x.\n",rAudFlashAQ.au1AudId[1]);
        LOG(0,"rAudFlashAQ.au1AudId[2] = 0x%x.\n",rAudFlashAQ.au1AudId[2]);
        LOG(0,"rAudFlashAQ.au1AudId[3] = 0x%x.\n",rAudFlashAQ.au1AudId[3]);

        if ((rAudFlashAQ.au1AudId[0] != 'a') ||
            (rAudFlashAQ.au1AudId[1] != 'q') ||
            (rAudFlashAQ.au1AudId[2] != 'd') ||
            (rAudFlashAQ.au1AudId[3] != 'a') ||
            (((u4ChkSum>>0)&0xff) != rAudFlashAQ.u1ChkSum[0]) ||
        (((u4ChkSum>>8)&0xff) != rAudFlashAQ.u1ChkSum[1]) ||
        (((u4ChkSum>>16)&0xff) != rAudFlashAQ.u1ChkSum[2]) ||
            (((u4ChkSum>>24)&0xff) != rAudFlashAQ.u1ChkSum[3]))
        {
            UINT32 u4Ptr;

            Printf("!!!! AUD_FlashAQInitEx - Partition: %d AQ ID: %d Offset: 0x%x size: %d", 
                    u1Partition, u2FlashAqIndex, u4Offset,u4Size);                
            Printf("!!!! AudioFlashAQPartition not update to flash correctly - %s!!!!\n", 
                    ((rAudFlashAQ.au1AudId[0] != 'a') ||
                     (rAudFlashAQ.au1AudId[1] != 'q') ||
                     (rAudFlashAQ.au1AudId[2] != 'd') ||
                     (rAudFlashAQ.au1AudId[3] != 'a')) ?
                    "aqda not match" : "checksum not match");

        VERIFY(DRVCUST_OptQuery(eAudioInputVolTable, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)rAudFlashAQ.aai2AudInputSrcVol), (const VOID *)u4Ptr, sizeof(rAudFlashAQ.aai2AudInputSrcVol));
        
        VERIFY(DRVCUST_OptQuery(eAudioVolTable, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)rAudFlashAQ.au4VolumeTable), (const VOID *)u4Ptr, sizeof(rAudFlashAQ.au4VolumeTable));
#ifdef CC_AUD_NEW_CV_TABLE
        VERIFY(DRVCUST_OptQuery(eAudioChannelVolTable, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)rAudFlashAQ.au4ChannelVolumeTable), (const VOID *)u4Ptr, sizeof(rAudFlashAQ.au4ChannelVolumeTable));
#endif
        
        VERIFY(DRVCUST_OptQuery(eAudioChlVolExtraGain, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)rAudFlashAQ.aai1AudChVolExtraGain), (const VOID *)u4Ptr, sizeof(rAudFlashAQ.aai1AudChVolExtraGain));

        VERIFY(DRVCUST_OptQuery(eAudioAvcTarget, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)rAudFlashAQ.ai2AudAvcTarget), (const VOID *)u4Ptr, sizeof(rAudFlashAQ.ai2AudAvcTarget));
        
        VERIFY(DRVCUST_OptQuery(eAudioAvcGainUp, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)rAudFlashAQ.ai2AudAvcGainUp), (const VOID *)u4Ptr, sizeof(rAudFlashAQ.ai2AudAvcGainUp));        
        
        VERIFY(DRVCUST_OptQuery(eAudioAvcAdjustRate, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)rAudFlashAQ.ai2AudAvcAdjRate), (const VOID *)u4Ptr, sizeof(rAudFlashAQ.ai2AudAvcAdjRate));
        
        VERIFY(DRVCUST_OptQuery(eAudioAvcUiAdjustRate, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)rAudFlashAQ.ai2AudAvcUiAdjRate), (const VOID *)u4Ptr, sizeof(rAudFlashAQ.ai2AudAvcUiAdjRate));        

        VERIFY(DRVCUST_OptQuery(eAudioAvChlEqTable, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)VIRTUAL((UINT32)rAudFlashAQ.aai1AudChlEqCfg), (const VOID *)u4Ptr, sizeof(rAudFlashAQ.aai1AudChlEqCfg));

        VERIFY(DRVCUST_OptQuery(eAudioChannelDelay, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)VIRTUAL((UINT32)rAudFlashAQ.au1AudChDelay), (const VOID *)u4Ptr, sizeof(rAudFlashAQ.au1AudChDelay));

        rAudFlashAQ.u4AudioVsClarity = DRVCUST_OptGet(eAudioMtkVsClarity);   //center gain, 
        rAudFlashAQ.u4AudioVsWidth = DRVCUST_OptGet(eAudioMtkVsWidth);   //surround gain,
        rAudFlashAQ.u4AudioVsLRGain = DRVCUST_OptGet(eAudioMtkVsLRGain); //L/R gain,
        rAudFlashAQ.u4AudioVsXTalk = DRVCUST_OptGet(eAudioMtkVsXtalk); //crosstalk,
        rAudFlashAQ.u4AudioVsOutputGain = DRVCUST_OptGet(eAudioMtkVsOutputGain); //output gain,   
        rAudFlashAQ.u4AudioVsBassGain = DRVCUST_OptGet(eAudioMtkVsBassGain); //bass enhancement,
        rAudFlashAQ.u4AudioVsFO = DRVCUST_OptGet(eAudioMtkVsFo); //FO,
                
        rAudFlashAQ.u4AudioLimiterMode = DRVCUST_OptGet(eAudioLimiterMode);

        rAudFlashAQ.u4AudioLimiterThreshold = DRVCUST_OptGet(eAudioLimiterFixThreshold);

            rAudFlashAQ.u1AudioLRSpkTypeLarge = DRVCUST_OptGet(eAudioSpkType);
            rAudFlashAQ.u1AudioSLRSpkTypeLarge = DRVCUST_OptGet(eAudioSurroundSpkType);
            rAudFlashAQ.u1AudioCenterSpkTypeLarge = DRVCUST_OptGet(eAudioCenterSpkType);
            rAudFlashAQ.u1AudioCH910SpkTypeLarge = DRVCUST_OptGet(eAudioCh910SpkType);
            rAudFlashAQ.u1AudioSubwooferOn = DRVCUST_OptGet(eAudioSubwooferOn);
            rAudFlashAQ.u2AudioBassMngCutoffFreq = DRVCUST_OptGet(eAudioBassMngCutOffFreq);
            x_memcpy((VOID *)VIRTUAL(rAudFlashAQ.aai1AudAVCChVolExtraGain), (const VOID *)u4Ptr, sizeof(rAudFlashAQ.aai1AudAVCChVolExtraGain));

        }
        else
        {
#if MULTIPLE_FLASH_AQ
            Printf("!!!! FlashAQ(%d) Init OK !!!!\n", u2FlashAqIndex);
#else
            Printf("!!!! FlashAQ Init OK !!!!\n");
#endif
        }
    }
    fgAQInit = TRUE;
}


void AUD_GetFlashAQVer(CHAR* str)
{
    x_memcpy((VOID*)VIRTUAL((UINT32)str),(VOID*)VIRTUAL((UINT32)rAudFlashAQ.au1AudVersion),8);
}
#else
INT32 AUD_FlashAQReadEx(UINT8 u1AQPartition, UINT16 u2Offset, UINT8 *u1Data, UINT32 u4Length)
{
    UNUSED(u1AQPartition);
    UNUSED(u2Offset);
    UNUSED(u1Data);
    UNUSED(u4Length);
    return 0;
}
void AUD_GetFlashAQVer(CHAR* str)
{
    UNUSED(str);
}
#endif

UINT32 GetAudioBassMngCutOffFreq(void)
{
#ifdef SUPPORT_FLASH_AQ
    return rAudFlashAQ.u2AudioBassMngCutoffFreq;
#else
    return DRVCUST_OptGet(eAudioBassMngCutOffFreq);
#endif
}

UINT32 GetAudioLRSpkTypeLarge(void)
{
#ifdef SUPPORT_FLASH_AQ
    return rAudFlashAQ.u1AudioLRSpkTypeLarge;
#else
    return DRVCUST_OptGet(eAudioSpkType);
#endif
}

UINT32 GetAudioSLRSpkTypeLarge(void)
{
#ifdef SUPPORT_FLASH_AQ
    return rAudFlashAQ.u1AudioSLRSpkTypeLarge;
#else
    return DRVCUST_OptGet(eAudioSurroundSpkType);
#endif
}

UINT32 GetAudioCenterSpkTypeLarge(void)
{
#ifdef SUPPORT_FLASH_AQ
    return rAudFlashAQ.u1AudioCenterSpkTypeLarge;
#else
    return DRVCUST_OptGet(eAudioCenterSpkType);
#endif
}

UINT32 GetAudioCH910SpkTypeLarge(void)
{
#ifdef SUPPORT_FLASH_AQ
    return rAudFlashAQ.u1AudioCH910SpkTypeLarge;
#else
    return DRVCUST_OptGet(eAudioCh910SpkType);
#endif
}

UINT32 GetAudioSubwooferOn(void)
{
#ifdef SUPPORT_FLASH_AQ
    return rAudFlashAQ.u1AudioSubwooferOn;
#else
    return DRVCUST_OptGet(eAudioSubwooferOn);
#endif
}

UINT32 GetAudioVsClarity(void)
{
#ifdef SUPPORT_FLASH_AQ
    return rAudFlashAQ.u4AudioVsClarity;
#else
    return DRVCUST_OptGet(eAudioMtkVsClarity);
#endif
}

UINT32 GetAudioVsWidth(void)
{
#ifdef SUPPORT_FLASH_AQ
    return rAudFlashAQ.u4AudioVsWidth;
#else
    return DRVCUST_OptGet(eAudioMtkVsWidth);
#endif
}

UINT32 GetAudioVsLRGain(void)
{
#ifdef SUPPORT_FLASH_AQ
    return rAudFlashAQ.u4AudioVsLRGain;
#else
    return DRVCUST_OptGet(eAudioMtkVsLRGain);
#endif
}

UINT32 GetAudioVsXTalk(void)
{
#ifdef SUPPORT_FLASH_AQ
    return rAudFlashAQ.u4AudioVsXTalk;
#else
    return DRVCUST_OptGet(eAudioMtkVsXtalk);
#endif
}

UINT32 GetAudioVsOutputGain(void)
{
#ifdef SUPPORT_FLASH_AQ
    return rAudFlashAQ.u4AudioVsOutputGain;
#else
    return DRVCUST_OptGet(eAudioMtkVsOutputGain);
#endif
}

UINT32 GetAudioVsBassGain(void)
{
#ifdef SUPPORT_FLASH_AQ
    return rAudFlashAQ.u4AudioVsBassGain;
#else
    return DRVCUST_OptGet(eAudioMtkVsBassGain);
#endif
}

UINT32 GetAudioVsFO(void)
{
#ifdef SUPPORT_FLASH_AQ
    return rAudFlashAQ.u4AudioVsFO;
#else
    return DRVCUST_OptGet(eAudioMtkVsFo);
#endif
}

UINT32 GetAudioLimiterMode(void)
{
#ifdef SUPPORT_FLASH_AQ
    return rAudFlashAQ.u4AudioLimiterMode;
#else
    return DRVCUST_OptGet(eAudioLimiterMode);
#endif
}

UINT32 GetAudioLimiterThreshold(void)
{
#ifdef SUPPORT_FLASH_AQ
    return rAudFlashAQ.u4AudioLimiterThreshold;
#else
    return DRVCUST_OptGet(eAudioLimiterFixThreshold);
#endif
}

UINT32 GetAudioHPLimiterThreshold(void)
{
#ifdef SUPPORT_FLASH_AQ
    return rAudFlashAQ.u4AudioHPLimiterThreshold;
//#else
  //  return DRVCUST_OptGet(eAudioHPLimiterFixThreshold);
#endif
}

//-----------------------------------------------------------------------------
/** Aud_FlashAQRead
 *  Read data from NAND Flash 
 *
 *  @param           
 *  @retval void
 */
//-----------------------------------------------------------------------------
#ifdef CC_AUD_USE_FLASH_AQ
INT32 AUD_FlashAQRead(UINT16 u2Offset, UINT8 *u1Data, UINT32 u4Length)
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

#if 1 //Daniel, 20110608, For Jun Wang integration
    return (STORG_SyncRead(u1AQPartition, u2Offset, (VOID*)u1Data, u4Length));
#else
    return (STORG_SyncNandRead(u1AQPartition, u2Offset, (UINT8*)(VOID*)u1Data, u4Length));
#endif
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
void AUD_FlashAQWrite(UINT16 u2Offset, UINT8 *u1Data, UINT32 u4size)
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

BOOL AUD_FlashAQCompVersion(CHAR *szCurVer, CHAR *szUpgVer)
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
/** Aud_FlashAQSetDefault
 *  Set default data from drvcust to NAND Flash.
 *
 *  @param           
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_FlashAQSetDefault(void)
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
    AUD_FlashAQWrite(0, pAudAQData, u4AudAQSize);

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
    AUD_FlashAQWrite(u4Offset, pAudAQData, u4AudAQSize);
#endif  // CC_AUD_NEW_AQ_PARSER  

}

//-----------------------------------------------------------------------------
/** Aud_FlashAQGetDscrpt
 *  Get the data description from drvcust.
 *
 *  @param           
 *  @retval void
 */
//-----------------------------------------------------------------------------
BOOL AUD_FlashAQGetDscrpt(AUD_AQ_DSCRPT_T** pAudFlashAQDscrpt)
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
/** Aud_FlashAQFillData
 *  Get the data from NAND flash. 
 *  Parsing them according to data description and fill them to data structure.
 *
 *  @param           
 *  @retval void
 */
//-----------------------------------------------------------------------------
#define AUD_AQ_READ_AMOUNT 10

void AUD_FlashAQFillData(BOOL fgDelayMode)
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
    AUD_FlashAQRead(FLASH_AQ_VER_OFFSET, (UINT8*)pData, FLASH_AQ_VER_LENGTH);
    if (x_strncmp((CHAR*)pData, "AQ", 2) !=0)
    {
        LOG(0, "Flash AQ data is not correct, using default value\n");
        ; //todo : Using default value if no flash AQ is available.
        return;
    }
        
    u1AudAQDscrptSize = (UINT8)DRVCUST_OptGet(eAudioFlashAQDataDscrptSize);
        
    if (AUD_FlashAQGetDscrpt(&eAudAQDscrpt)) //Get the Data Structure.
    {    
        u4Offset = 0;
        for (i=0; i<u1AudAQDscrptSize; i++)
        {
            u1X = eAudAQDscrpt[i].u1X;
            u1Y = eAudAQDscrpt[i].u1Y;
            u1Bytes = eAudAQDscrpt[i].u1Bytes;
            u4Temp = u1X * u1Y * u1Bytes;
            AUD_FlashAQRead(u4Offset, pData, u4Temp);
            AUD_FlashAQParsing(eAudAQDscrpt[i].eDataType, pData);
            u4Offset += u4Temp;
        }
        
#ifndef CC_AUD_NEW_AQ_PARSER        
        //External PEQ
        u4Temp = (FLASH_AQ_SRC_NUM * FLASH_AQ_SM_NUM * AQ_AMP_FS_NUM * AQ_EXT_PEQ_ITEMS) * 3;

        if (!fgDelayMode)
        {
            AUD_FlashAQRead(u4Offset, pData, u4Temp);
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
        AUD_FlashAQParsing(AQ_TYPE_EXT_PEQ, pData);        
        
#endif //n CC_AUD_NEW_AQ_PARSER
    }
    else
    {
        LOG(0, "Flash AQ data is not correct, using default value\n");
        ; //todo : Using default value if no flash AQ is available.
    }    

    if (AUD_FlashAQQuery(AQ_TYPE_POST, (void*)&u4AQPostScale))
    {
        ADAC_DigiAmp_SetPostScale(AUD_AMP_ID_LR, u4AQPostScale[0],u4AQPostScale[1]);
    }
}

void AUD_FlashAQInit(void)
{
    //Set all flag to False;
    AUD_FlashAQSetValue();

    //Fill all data into dram.
    AUD_FlashAQFillData(FALSE);
}

#endif /* CC_AUD_USE_FLASH_AQ */
//=================== END of Audio Flash AQ Functions ==============================

#ifdef CC_SUPPORT_AUTO_DETECT_AUD_SYSTEM
AUD_FMT_T AudAtvFmtDetection(void)
{
    return(_AudAtvFmtDetection());
}
#endif


void AUD_GetAudioTrickPts(UINT8 u1DecId, UINT32 *prAudioPts, BOOL fgOldPts)
{
    AUD_DrvGetAudioTrickPts(u1DecId, prAudioPts, fgOldPts);
}

#ifdef CC_AUD_VBASS_SUPPORT
VOID AUD_SetVBassCutOff(UINT8 u1Idx)
{
    UNUSED(u1VBassCutOffFreq);
    UNUSED(AUD_SetVBassCutOff);
    
    u1VBassCutOffFreq = u1Idx;
}
UINT8 AUD_GetVBassCutOff(void)
{
    UNUSED(AUD_GetVBassCutOff);
    
    return u1VBassCutOffFreq;
}
#endif //CC_AUD_VBASS_SUPPORT


#ifdef CC_FAST_INIT

static void AUD_pd_suspend(void)
{
    /////// Power Down Audio Module ///////
    /// Disable audio output stage.
    LOG(0, "[AUDS] AUD_pm_suspend - Power Down.\n");
    ADAC_Mute(TRUE);
    ADAC_GpioCodecMute(TRUE);
    ADAC_GpioAmpEnable(FALSE);
    ADAC_CodecMute(TRUE);
    ADAC_MuteCLK(TRUE); ///PWM CLK Fade-out to 0V.

    /// Disable MCLK Output
    _AUD_OutPadEnable(AUD_DEC_MAIN, FALSE);
    _AUD_OutPadEnable(AUD_DEC_AUX, FALSE);

    /// Disable SPDIF
    AUD_Spdif_Power_Down();

    /// Reset Amplifier
    ///_AUD_ResetAmp(TRUE);     /// by ADAC_GpioAmpEnable(FALSE); no need reset.

    /// Disable AADC
    _MT5365_ADCDOWN(TRUE);

    /// Power down Aud Pll
    AudPllPowerDown(APLL_ID_MAIN, 1);
    AudPllPowerDown(APLL_ID_AUX, 1);
    /////// Power Down Audio Module End ///////
}

//-----------------------------------------------------------------------------
/** AUD_pm_resume
 *  Resume function. 
 *
 *  @param           
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_pm_resume(void)
{
    LOG(0, "[AUDS] AUD_pm_resume.\n");
    DSP_DrvResume();
    AUD_Init();

    AUD_DrvThreadWakeup(AUD_DEC_MAIN);
    AUD_DrvThreadWakeup(AUD_DEC_AUX);
#ifdef CC_MT5391_AUD_3_DECODER    
    AUD_DrvThreadWakeup(AUD_DEC_THIRD);
#endif    
#ifdef CC_AUD_4_DECODER_SUPPORT
    AUD_DrvThreadWakeup(AUD_DEC_4TH);
#endif    
    LOG(0, "[AUDS] AUD_pm_resume Done.\n");
}

//-----------------------------------------------------------------------------
/** AUD_pm_suspend
 *  suspend function. 
 *
 *  @param           
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_pm_suspend(void)
{
    LOG(0, "[AUDS] AUD_pm_suspend.\n");
    AUD_DrvSuspend();
    ADAC_Suspend();
    AUD_DSPCmdSema_UnLock();
    AUD_NotifySuspend();    
    
    VERIFY(x_sema_unlock(_hCmdWaitSema[AUD_DEC_MAIN]) == OSR_OK);
    VERIFY(x_sema_unlock(_hCmdWaitSema[AUD_DEC_AUX]) == OSR_OK);
#ifdef CC_MT5391_AUD_3_DECODER    
    VERIFY(x_sema_unlock(_hCmdWaitSema[AUD_DEC_THIRD]) == OSR_OK);
#endif    
#ifdef CC_AUD_4_DECODER_SUPPORT
    VERIFY(x_sema_unlock(_hCmdWaitSema[AUD_DEC_4TH]) == OSR_OK);
#endif
    VERIFY(x_sema_unlock(_hAsynCmdSema) == OSR_OK);
    AUD_DspCfgSuspend();    
    DSP_DrvSuspend();

    /// Power Down Audio Module.
    AUD_pd_suspend();

    _fgAudInitOK = FALSE;
    LOG(0, "[AUDS] AUD_pm_suspend End.\n");
}
#endif

//-----------------------------------------------------------------------------
/** AUD_SetSyncDbgLvl
 *  Set AV Sync Debug Level. 
 *
 *  @param           
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_SetSyncDbgLvl(UINT16 u2Flag)
{
    UINT16 u2Tmp = 0;
    
    UNUSED(AUD_SetSyncDbgLvl);
    u2Tmp = DSP_GetPtsStcCtrlFlag();

    if ((u2Flag & AUD_DBG_SYNC_PTS_FRESH) == AUD_DBG_SYNC_PTS_FRESH)
    {
        u2Tmp |= (UINT16)(1<<4); //bit 4 is enable log.
    }
    else
    {
        u2Tmp &= (~(UINT16)(1<<4));
    }
    DSP_SetPtsStcCtrlFlag(u2Tmp);    
    
    u2AvSyncDbgFlag = u2Flag;
}
UINT16 AUD_GetSyncDbgLvl(void)
{
    UNUSED(AUD_GetSyncDbgLvl);
    UNUSED(u2AvSyncDbgFlag);
    
    return u2AvSyncDbgFlag;
}

BOOL AUD_IsFmtSupport(AUD_FMT_T eDecType)
{
    return AUD_DspIsFmtSupport(eDecType);
}
#ifdef LINUX_TURNKEY_SOLUTION
static UINT8* pTonePlayBuf;
#endif

void AUD_PlayBeep(UINT8 u1DecId,UINT8 frequency,UINT16 duration,UINT8 percent_volume_level)
{
    MEM_BUFFER_INFO_T  rMemBuf;
    AUD_PCM_SETTING_T  rPcmSetting;
    UINT8 u1Sel = 0;

    u1Sel = frequency;
     if (AUD_IsDecoderPlay(u1DecId))
    {
        AUD_StopMemStr();
        AUD_DSPCmdStop(u1DecId);
    }

    rMemBuf.u4LoopCount = duration; // Infinite loop

    rPcmSetting.eSampleFreq = FS_48K;
    rPcmSetting.ePcmDataInfo = PCM_24_BITS;

    //AUD_DspProcMode(u1DecId, 0xdc00);
    AUD_DspMasterVolume(u1DecId,percent_volume_level);
    AUD_SetDecType(u1DecId, AUD_STREAM_FROM_MEMORY, AUD_FMT_PCM);
    AUD_PcmSetting(u1DecId, (const AUD_PCM_SETTING_T *)&rPcmSetting);

#ifdef LINUX_TURNKEY_SOLUTION
    if (pTonePlayBuf)
    {
        BSP_FreeAlignedDmaMemory(pTonePlayBuf);
        pTonePlayBuf = NULL;
    }
#endif

    AUD_StrSelect(u1DecId, u1Sel, &rMemBuf);

#ifdef LINUX_TURNKEY_SOLUTION
    pTonePlayBuf = (UINT8 *)BSP_AllocAlignedDmaMemory(rMemBuf.u4Length, 32);
    x_memcpy((VOID*)VIRTUAL(pTonePlayBuf), (VOID*)VIRTUAL(rMemBuf.pData), rMemBuf.u4Length);
    rMemBuf.pData = (UINT8*)VIRTUAL(pTonePlayBuf);
#endif

    AUD_SetDecMemBuffer(u1DecId, &rMemBuf);
    AUD_DSPCmdPlay(u1DecId);
}


BOOL AUD_IsAACL2(void)
{
    return AUD_DspIsAACL2();
}

BOOL AUD_IsSupportSRS(void)
{
    return _fgSRS;
}

void AUD_SupportSRS(BOOL fgSRS)
{
    _fgSRS = fgSRS;
}

UINT8 AUD_SaveSRSFlag(UINT32 u4SRSFlag)
{
    INT32 i;
    UINT32 u4SRSFlagOffset;
    UINT32 u4SRSFlagOld;
    i = DRVCUST_OptQuery(eAudioSRSEnalbe, (UINT32 *)(void *)&u4SRSFlagOffset);
        
    if(i < 0)
    {
        printf("ERROR:AUD_SaveSRSFlag A\n");
        return 0;
    }
        
    if(1 == EEPROM_Read((UINT64)u4SRSFlagOffset, &u4SRSFlagOld, 4))
    {
        printf("ERROR:AUD_SaveSRSFlag B\n");
        return 0;
    }
    if(u4SRSFlagOld != u4SRSFlag)
    {
        if(1 == EEPROM_Write((UINT64)u4SRSFlagOffset, &u4SRSFlag, 4))
        {
            printf("ERROR:AUD_SaveSRSFlag C\n");
            return 0;
        }
    }
    
    printf("AUD_SaveSRSFlag u4SRSFlag=%d.\n",u4SRSFlag);
    return 1;
}

UINT8 AUD_LoadSRSFlag(void)
{
    INT32 i;
    UINT32 u4SRSFlagOffset;
    UINT32 u4SRSFlag;
    
    i = DRVCUST_OptQuery(eAudioSRSEnalbe, (UINT32 *)(void *)&u4SRSFlagOffset);
        
    if(i < 0)
    {
        printf("ERROR:AUD_LoadSRSFlag D\n");
        return 1;
    }
        
    if(1 == EEPROM_Read((UINT64)u4SRSFlagOffset, &u4SRSFlag, 4))
    {
        printf("ERROR:AUD_LoadSRSFlag E\n");
        return 1;
    }
    
    printf("AUD_LoadSRSFlag u4SRSFlag=%d.\n",u4SRSFlag);
    return u4SRSFlag;
}

UINT8 AUD_SaveMpeg4Flag(UINT32 u4Mpeg4Flag)
{
    INT32 i;
    UINT32 u4Mpeg4FlagOffset;
    UINT32 u4MPEG4FlagOld;
    i = DRVCUST_OptQuery(eAudioMpeg4Enalbe, (UINT32 *)(void *)&u4Mpeg4FlagOffset);
        
    if(i < 0)
    {
        printf("ERROR:AUD_SaveMpeg4Flag A\n");
        return 0;
    }
        
    if(1 == EEPROM_Read((UINT64)u4Mpeg4FlagOffset, &u4MPEG4FlagOld, 4))
    {
        printf("ERROR:AUD_SaveMpeg4Flag B\n");
        return 0;
    }
    if(u4MPEG4FlagOld != u4Mpeg4Flag)
    {
        if(1 == EEPROM_Write((UINT64)u4Mpeg4FlagOffset, &u4Mpeg4Flag, 4))
        {
            printf("ERROR:AUD_SaveMpeg4Flag C\n");
            return 0;
        }
    }
    
    printf("AUD_SaveMpeg4Flag u4SRSFlag=%d.\n",u4Mpeg4Flag);
    return 1;
}

UINT8 AUD_LoadMpeg4Flag(void)
{
    INT32 i;
    UINT32 u4Mpeg4FlagOffset;
    UINT32 u4Mpeg4Flag;
    
    i = DRVCUST_OptQuery(eAudioMpeg4Enalbe, (UINT32 *)(void *)&u4Mpeg4FlagOffset);
        
    if(i < 0)
    {
        printf("ERROR:AUD_LoadMpeg4Flag D\n");
        return 1;
    }
        
    if(1 == EEPROM_Read((UINT64)u4Mpeg4FlagOffset, &u4Mpeg4Flag, 4))
    {
        printf("ERROR:AUD_LoadMpeg4Flag E\n");
        return 1;
    }
    
    printf("AUD_LoadMpeg4Flag u4Mpeg4Flag=%d.\n",u4Mpeg4Flag);
    return u4Mpeg4Flag;
}

BOOL AUD_IsSupportMpeg4(void)
{
    return _fgMpeg4;
}

void AUD_SupportMpeg4(BOOL fgMpeg4)
{
    _fgMpeg4 = fgMpeg4;
}






