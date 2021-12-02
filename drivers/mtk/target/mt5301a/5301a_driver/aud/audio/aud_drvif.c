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
 * $RCSfile: aud_drvif.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file aud_drvif.c
 *  Brief of file aud_drvif.c.
 *  Details of file aud_drvif.c (optional).
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

// KERNEL
#include "x_util.h"
#include "x_typedef.h"
#include "aud_if.h"
#include "aud_cfg.h"
#include "aud_drv.h"
#include "aud_drvif.h"
#include "aud_notify.h"
#include "aud_debug.h"
#include "dsp_intf.h"
#include "x_os.h"
#include "x_assert.h"


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define FIFO_FULL_THRESHOLD     (UINT32)(5*1024)
#ifndef __KERNEL__
#define FIFO_EMPTY_THRESHOLD    (UINT32)(2000) 
#else
// Just workaround ! FIXME !
#define FIFO_EMPTY_THRESHOLD    (UINT32)(3300) 
#endif

#define FIFO_EMPTY_THRESHOLD_AUI (UINT32)(10000) 

#define FIFO_EMPTY_RETRY_THRESHOLD (UINT32)(5)

#ifdef CC_ENABLE_AV_SYNC
extern  UINT8 _u6VideoSetCHdelay;
#endif

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
BOOL _fgFMRadioDetectionResult;


//-----------------------------------------------------------------------------
//  _SmpInit
//
/** Brief of _SmpInit.
 *  Details of _SmpInit (optional).
 *
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

BOOL AUD_IsFifoEmpty(UINT8 u1DecId)
{
    static UINT32 u4DataSizeLast = 0;
    static UINT32 u4RetryCnt = 0;
    UINT32 u4Rp;
    UINT32 u4Wp;
    UINT32 u4DataSize;
    UINT32 u4FrameSize;
    UINT32 u4DataThreshold = FIFO_EMPTY_THRESHOLD;
    UINT32 u4RetryThres = AUD_GetEmptyRetryThres();		
    BOOL   fgRet;
    AUD_DRV_STATE_T eDrvState;
    AUD_DEC_STREAM_FROM_T eAudDecStreamFrom;
    AUD_FMT_T eAudDecFormat;

#ifndef CC_MT5391_AUD_3_DECODER
//#if 1
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) ||  (u1DecId == AUD_DEC_THIRD));
#endif    

    eDrvState = AUD_DRVGetAudioState(u1DecId);

    AUD_GetRWPtr(u1DecId, &u4Rp, &u4Wp, &u4DataSize);
    AUD_DRVGetDecodeType(u1DecId, &eAudDecStreamFrom, &eAudDecFormat);
    if (eAudDecFormat == AUD_FMT_MPEG)
    {
        u4FrameSize = DSP_GetMpegFrameSize(u1DecId);
        if (u4FrameSize != 0)
        { 
            u4DataThreshold = (u4FrameSize * 5) / 2;  // Set to 2.5 frame size
        }    
    }
    else if (eAudDecFormat == AUD_FMT_WMA)
    {
/*      	u4FrameSize = DSP_GetWmaFrameSize(); 
        if (u4FrameSize != 0)
        {
        	u4DataThreshold = DSP_GetWmaFrameSize() + 4;  //WMA header by swdmx has 4 bytes
        }
*/
		u4RetryThres = 20;//Laihui0319: Originally 15, some files will still so fast to return fifo empty.
    }

    LOG(9, "Dec (%d) remaining data size = 0x%08x\n", u1DecId, u4DataSize);

#if 0
 
    //if (u4DataSZ < FIFO_EMPTY_THRESHOLD)
    if (u4DataSZ < (10000))
    {
        return TRUE;
    }   
    else
    {
        return FALSE;
    }   
#endif

#ifdef __KERNEL__
    if (u4DataSize < FIFO_EMPTY_THRESHOLD_AUI)
    {
        fgRet= TRUE;
        x_thread_delay(10);     
        return fgRet;
    }
#endif

    if (eDrvState == AUD_IDLE)
    {
        fgRet = TRUE;
    }
    else if (eDrvState == AUD_ON_PAUSE)
    {
        u4RetryCnt = 0;
        fgRet = FALSE;
    }
    else if ((u4DataSize < u4DataThreshold) && (u4DataSizeLast == u4DataSize))
    {
        fgRet = TRUE;
    }
/*    else if ((u4DataSize > u4DataThreshold) && (eAudDecFormat == AUD_FMT_WMA))
    {
        fgRet = FALSE;
    }
*/    
    else if (u4DataSizeLast == u4DataSize)
    {
        u4RetryCnt++;
        if (u4RetryCnt >= u4RetryThres)
        {
            fgRet = TRUE;
        }
        else
        {
            fgRet = FALSE;
        }
    }
    else
    {
        u4RetryCnt = 0;
        fgRet = FALSE;
    }   

    x_thread_delay(10);  
    u4DataSizeLast = u4DataSize;
    
    if (fgRet)
    {
        LOG(5, "FIFO is empty. Data (%d),  Retry (%d)\n", u4DataSizeLast, u4RetryCnt);    
        u4DataSizeLast = 0;
        u4RetryCnt = 0;
    }

    return fgRet;
}

BOOL AUD_IsFifoFull(UINT8 u1DecId)
{
    UINT32 u4Fifo1Start;
    UINT32 u4Fifo1End;
    UINT32 u4Fifo2Start;
    UINT32 u4Fifo2End;
    UINT32 u4FifoSZ;
    UINT32 u4Rp;
    UINT32 u4Wp;
    UINT32 u4DataSZ;

#ifndef CC_MT5391_AUD_3_DECODER
//#if 1
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) ||  (u1DecId == AUD_DEC_THIRD));
#endif

    VERIFY(AUD_GetAudFifo(&u4Fifo1Start, &u4Fifo1End, &u4Fifo2Start, &u4Fifo2End) == AUD_OK);
#ifndef CC_MT5391_AUD_3_DECODER
//#if 1    
    u4FifoSZ = (u1DecId == AUD_DEC_MAIN) ? (u4Fifo1End - u4Fifo1Start) : (u4Fifo2End - u4Fifo2Start);
#else
    u4FifoSZ = (u1DecId == AUD_DEC_AUX) ? (u4Fifo2End - u4Fifo2Start) : (u4Fifo1End - u4Fifo1Start);
#endif
    AUD_GetRWPtr(u1DecId, &u4Rp, &u4Wp, &u4DataSZ);

    if ((u4DataSZ + FIFO_FULL_THRESHOLD) > u4FifoSZ)
    {
        return TRUE;
    }   
    else
    {
        return FALSE;
    }   
}


INT32 AUD_GetAudFifo(UINT32 * pu4Fifo1Start, UINT32 * pu4Fifo1SEnd,UINT32 * pu4Fifo2Start,UINT32 * pu4Fifo2End)
{
    return _AUD_GetAudFifo(pu4Fifo1Start, pu4Fifo1SEnd, pu4Fifo2Start, pu4Fifo2End);
}

INT32 AUD_SetAudFifo(UINT8 u1DecId, UINT32  u4Address, UINT32 u4Length)
{
#ifndef CC_MT5391_AUD_3_DECODER
//#if 1
   VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) ||  (u1DecId == AUD_DEC_THIRD));
#endif
   // TODO 
   UNUSED(u4Address);
   UNUSED(u4Length);
   return AUD_OK;
}

void AUD_HdmiModeMute(UINT8 u1DecId, BOOL fgMute)
{
    AUD_DspHdmiModeMuteEnable(u1DecId, fgMute);
}

void AUD_AtvAvInitialSync(BOOL fgStable)
{
    _AUD_AtvAvInitialSync(fgStable);
}

void AUD_WaitDspFlush(UINT8 u1DecId)
{   
    if (AUD_GetTargetCountry() == COUNTRY_US)
	{
        if (u1DecId == AUD_DEC_AUX)
        {
            return ;
        }
	}
	else
	{
        #ifdef CC_AUD_SINGLE_DEC_MODE
            if (u1DecId == AUD_DEC_AUX)
            {
                return ;
            }
        #else
            // Do this when EURO dual decoder mode with AUX decoder in ATV
        #endif    
	}

    if (AUD_IsDecoderPlay(u1DecId))
    {
        UNUSED(_AUD_IgnoreDecNotify(u1DecId, TRUE));        
        UNUSED(AUD_DSPCmdStop(u1DecId));
        while (!DSP_RealStop(u1DecId))
        {
            x_thread_delay(10);
        }
        UNUSED(_AUD_IgnoreDecNotify(u1DecId, FALSE));
        //clear AoutWait bit if issue stop, to prevent DSP hang!
        if (_AudGetStrSource(AUD_DEC_MAIN)== AUD_STREAM_FROM_MULTI_MEDIA)
        {
            AUD_DrvMMAoutEnable(AUD_DEC_MAIN, TRUE);
            AUD_DrvMMAoutEnable_Flag(FALSE);
            AUD_DrvMMAoutReady_Flag(FALSE);
        }              
    }
    else
    {
        LOG(5, "Invalid AUD_WaitDspFlush condition\n");
    }
}

BOOL _AudFMRadioDetection(void)
{

    if (AUD_IsDecoderPlay(AUD_DEC_MAIN))
    {
        UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_MAIN, TRUE));
        UNUSED(AUD_DSPCmdStop(AUD_DEC_MAIN));
        while (!DSP_RealStop(AUD_DEC_MAIN))
        {
            x_thread_delay(10);
        }
        UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_MAIN, FALSE));
    }
    x_thread_delay(30);
    if (AUD_IsDecoderPlay(AUD_DEC_AUX))
    {
        UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_AUX, TRUE));
        UNUSED(AUD_DSPCmdStop(AUD_DEC_AUX));
        while (!DSP_RealStop(AUD_DEC_AUX))
        {
            x_thread_delay(10);
        }
        UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_AUX, FALSE));
    }
    _fgFMRadioDetectionResult = 0;
    UNUSED(AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_ANALOG_TUNER, AUD_FMT_FMRDO_DET));
    UNUSED(AUD_DSPCmdPlay(AUD_DEC_MAIN));
    x_thread_delay(400);
    return _fgFMRadioDetectionResult;
    
}

BOOL _AudPlayFMRadio(void)
{
    if (AUD_IsDecoderPlay(AUD_DEC_MAIN))
    {
        UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_MAIN, TRUE));
        UNUSED(AUD_DSPCmdStop(AUD_DEC_MAIN));
        while (!DSP_RealStop(AUD_DEC_MAIN))
        {
            x_thread_delay(10);
        }
        UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_MAIN, FALSE));
    }
    if (AUD_IsDecoderPlay(AUD_DEC_AUX))
    {
        UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_AUX, TRUE));
        UNUSED(AUD_DSPCmdStop(AUD_DEC_AUX));
        while (!DSP_RealStop(AUD_DEC_AUX))
        {
            x_thread_delay(10);
        }
        UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_AUX, FALSE));
    }
    
    UNUSED(AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_ANALOG_TUNER, AUD_FMT_FMRDO));
    x_thread_delay(20);
    UNUSED(AUD_DSPCmdPlay(AUD_DEC_MAIN));
    return TRUE;

}

 void Aud_SetTvSysMask(UINT8 u1DecId, UINT32 u4Mask)
{
   
   #if 1
    _AudSetTvSysMask(u1DecId,  u4Mask);
   #endif
   return;
}


void AUD_TriggerDspPlay(UINT8 u1DecId)
{   

//#ifndef CC_AUD_DVBT_SUPPORT 
	if(AUD_GetTargetCountry() != COUNTRY_EU)
	{
        if (u1DecId == AUD_DEC_AUX)
        {
            return ;
        }
	}
//#else
    else
    {
        #ifdef CC_AUD_SINGLE_DEC_MODE
            if (u1DecId == AUD_DEC_AUX)
            {
                return ;
            }
        #else
            // Do this when EURO dual decoder mode with AUX decoder in ATV
            //first need to do detection to get audio system then to set audio system information      
            AUD_DEC_STREAM_FROM_T  rAudDecStreamFrom;
            AUD_FMT_T              eAudDecFormat;
            UNUSED(AUD_DRVGetDecodeType(AUD_DEC_MAIN, &rAudDecStreamFrom, &eAudDecFormat));
            if  ((eAudDecFormat !=AUD_FMT_DETECTOR)&&(!AUD_IsDecoderPlay(AUD_DEC_AUX)))
            {

                AUD_FMT_T eAudFmt;
                eAudFmt = _AudAtvFmtDetection();
                UNUSED(AUD_SetDecType(AUD_DEC_AUX, AUD_STREAM_FROM_ANALOG_TUNER, eAudFmt)); 
            
                //Play AUX decoder 	  
                UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_AUX, TRUE));
                UNUSED(AUD_DSPCmdPlay(AUD_DEC_AUX));
                AUD_DualDecSetSyncSema(TRUE);
                UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_AUX, FALSE));   
            }
            return;
        #endif
    }
//#endif  
}


void AUD_AtunerSigStableNotify(BOOL fgSatble)
{
//#ifdef CC_AUD_DVBT_SUPPORT
//    static BOOL fgInit = FALSE;
	if(AUD_GetTargetCountry() == COUNTRY_EU)
	{
	  if (fgSatble)
	  {
        AUD_AtunerSigStable();
	  }
	}        
//    if (!fgInit)
//    {
//        AUD_ScartOutInit();
//        fgInit = TRUE;
//    }
//#else
    else
    {
        UNUSED(fgSatble);
    }
//#endif
}

void AUD_MMAoutEnable(UINT8 u1DecId, BOOL fgEnable)
{
    AUD_DrvMMAoutEnable(u1DecId, fgEnable);
}

#ifdef CC_ENABLE_AV_SYNC
void AUD_DrvSetChDelay(UINT8 u1Delay)
{
    AUD_DEC_STREAM_FROM_T eAudDecStreamFrom;
    AUD_FMT_T              eAudDecFormat;
    UINT16                     u2Delay;    
    UINT8                      u1Version;

    UNUSED(eAudDecStreamFrom);
    AUD_DRVGetDecodeType(AUD_DEC_AUX, &eAudDecStreamFrom, &eAudDecFormat);
    _u6VideoSetCHdelay=u1Delay;  //store the delay num from Video
    
    if (_AudGetStrSource(AUD_DEC_MAIN) == AUD_STREAM_FROM_DIGITAL_TUNER)
   {
        if (eAudDecFormat==AUD_FMT_MPEG)
       {
          //u2Delay=_u6VideoSetCHdelay+80;
          u2Delay=_u6VideoSetCHdelay+50;
          LOG(3,"@@@@@ It is MPEG @@@@@@\n");
        }
        else if(eAudDecFormat==AUD_FMT_AC3)
       {
          //u2Delay=_u6VideoSetCHdelay+120;
          u2Delay=_u6VideoSetCHdelay+60; 
          LOG(3,"@@@@@ It is AC3 @@@@@@\n");
        }
        else if(eAudDecFormat==AUD_FMT_AAC)
        {
            // Get AAC version
            u1Version = u1GetAacVersion(AUD_DEC_AUX);
            if (u1Version==0x0) //AAC_LC
            {            
               u2Delay=_u6VideoSetCHdelay;               
                LOG(3,"@@@@@ It is AAC_LC @@@@@@\n");
             }          
             else//HE-AAC
            {
                u2Delay= 0;                      
                 LOG(3,"@@@@@ It is HE-AAC @@@@@@\n");
            }   
         }
         else
         {
             u2Delay=_u6VideoSetCHdelay;        
              LOG(3,"@@@@@ It is others @@@@@@\n");
         }
     }
    else
    {
        u2Delay=_u6VideoSetCHdelay;        
         LOG(3,"@@@@@ It is others  & NOT DTV source@@@@@@\n");
    }
    
    
    if (!AUD_IsIgnoreVdpInfo() )
    {
        LOG(3,"[SRM_AVSYNC]&&&&&&& AUD_DrvSetChDelay &&&&&&&&&\n");
        //set delay
         //AUD_DspChannelDelay(((u1Delay*34)/5)+(u2Delay_init*34/5), AUD_CH_ALL, AUD_DEC_MAIN);
        AUD_DspChannelDelay(((u2Delay*34)/5), AUD_CH_ALL, AUD_DEC_MAIN); 
         // unmute
         
         _fgwaitForAoutReinit=TRUE;
       // AUD_DspDecPlayMuteEnable(AUD_DEC_MAIN, FALSE);    
        //LOG(7," [SRM_AVSYNC] AUD_DrvSetChDelay , _fgwaitForAoutReinit=TRUE!!!! ######\n");
         
        // set flag for audio driver
        AUD_VdpReady(TRUE);
    }
}

void AUD_VdpModeChange(void)
{
    UNUSED(AUD_VdpModeChange);
    _AudSrmStableCb(0,0,0, AUD_VIDEO_IS_UNSTABLE);
}

void AUD_VdpModeChangeDone(void)
{
    UNUSED(AUD_VdpModeChangeDone);
    _AudSrmStableCb(0,0,0, AUD_VIDEO_IS_STABLE);
}
    
#endif //CC_ENABLE_AV_SYNC
void AUD_ClearEOSFlag(UINT8 u1DecId)
{
    _AudClearEOSFlag(u1DecId);
}


UINT32 AUD_GetEmptyRetryThres(void)
{
	return 	FIFO_EMPTY_RETRY_THRESHOLD;
}

