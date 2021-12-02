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
/***************  $Modtime:: 04/12/06 3:31p    $       ****************/
/***************  $Revision: #2 $       ****************/
/***************                                       ****************/
/***************   Description : DSP Control routines  ****************/
/***************                                       ****************/
/***************       Company : MediaTek Inc.         ****************/
/***************    Programmer : Jack Hsu              ****************/
/***************                 Peichen Chang         ****************/
/**********************************************************************/
#define _DSP_IRQ_C

#include "dsp_common.h"
#include "dsp_reg.h"
#include "dsp_func.h"
#include "dsp_uop.h"
#include "dsp_shm.h"
#include "dsp_intf.h"
#include "d2rc_shm.h"
#include "drv_adsp.h"
#include "dsp_rg_ctl.h"
#include "aud_drv.h"
#include "dsp_intf.h"

#include "aud_debug.h"

#ifdef CHANNEL_CHANGE_LOG
#include "x_timer.h"
#endif
void vDspIntSvc (void);
UINT32 u4IntFlowControl(UINT32 u4DspRIntData);
void vDspLogAoutEnablePTS(void);
//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
extern void vSendADSPTaskData(UINT32 u4Msg); //ADSP Data Queue.
extern void vGetADSPTaskData(UINT32 *u4Msg); //ADSP Data Queue.


#ifdef MIXSOUND_MIPS_MEASURE
UINT32 dwAoutIndex = 0;
UINT32 dwAoutMIPS = 0;

void vAoutMIPSQuery(void)
{
    Printf("dwAoutIndex: %d\n", dwAoutIndex);
    Printf("dwAoutMIPS: %d\n", dwAoutMIPS);    
    Printf("Cycle: %d\n", (dwAoutIndex == 0) ? 0 : dwAoutMIPS/dwAoutIndex);
}

void vAoutMIPSReset(void)
{
    dwAoutIndex = 0;
    dwAoutMIPS = 0;
}
#endif

//static TV_AUD_SYS_T _eTvSys = SV_NONE_DETECTED;
UINT32 u4DspIRQSvc (void)
{
    UINT32 u4DspRIntData;
    UINT8 bDspRIntAddr;
    UINT32 u4Msg = ADSPTASK_MSG_INTERRUPT;
    UINT32 u4DspDataBackup; //ADSP Data Queue.

    //u4DspRIntData = ReadREG(RO_D2R_SDA);
    u4DspRIntData = u4DspRegR_D2rSda();
    u4DspDataBackup = u4DspRIntData; //ADSP Data Queue.
    bDspRIntAddr = (u4DspRIntData & 0xFF);
    u4DspRIntData = (u4DspRIntData >> 8);

    switch (bDspRIntAddr)
    {
    case INT_D2RC_WAKEUP_OK:
        _rDspStatus[AUD_DEC_MAIN].fgDspWakeUpOk = TRUE;
        u4Msg = ADSPTASK_MSG_NO_COMMAND; // Refine ADSP control
        break;     
    case INT_D2RC_FLOW_CONTROL:  /* Just for LOG */  
        u4DspRIntData = (u4DspRIntData >> 8);
        DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);
        
        if (u4DspRIntData==D2RC_FLOW_CONTROL_MODE_CHANGE) 
        {
            if (_rDspVars[AUD_DEC_MAIN].bDspStrTyp == AC3_STREAM)
            {
                LOG(7, "[DSP_IRQ]Receive Dec0 AC3 ACMOD change Int, ACMOD = %x\n",dReadDspCommDram(ADDR_D2RC_AC3_ACMOD)>>8);
            }
            else if ((_rDspVars[AUD_DEC_MAIN].bDspStrTyp == MPEG12_STREAM) || (_rDspVars[AUD_DEC_MAIN].bDspStrTyp == MPEG3_STREAM))
            {
                LOG(7, "[DSP_IRQ]Receive Dec0 MPEG mode change Int, Mode = %x\n",dReadDspCommDram(ADDR_D2RC_MPEG_MODE)>>8);
            }
        }
        else if (u4DspRIntData==D2RC_FLOW_CONTROL_MODE_CHANGE_DEC2) 
        {
            if (_rDspVars[AUD_DEC_MAIN].bDspStrTyp == AC3_STREAM)
            {
                LOG(7, "[DSP_IRQ]Receive Dec1 AC3 ACMOD change Int, ACMOD = %x\n",dReadDspCommDram(ADDR_D2RC_AC3_ACMOD_DEC2)>>8);
            }
            else if ((_rDspVars[AUD_DEC_MAIN].bDspStrTyp == MPEG12_STREAM) || (_rDspVars[AUD_DEC_MAIN].bDspStrTyp == MPEG3_STREAM))
            {
                LOG(7, "[DSP_IRQ]Receive Dec1 MPEG mode change Int, Mode = %x\n",dReadDspCommDram(ADDR_D2RC_MPEG_MODE_DEC2)>>8);
            }
        }
#ifdef CC_MT5391_AUD_3_DECODER
        else if (u4DspRIntData==D2RC_FLOW_CONTROL_MODE_CHANGE_DEC3) 
        {
            if ((_rDspVars[AUD_DEC_MAIN].bDspStrTyp == MPEG12_STREAM) || (_rDspVars[AUD_DEC_MAIN].bDspStrTyp == MPEG3_STREAM))
            {
                LOG(7, "[DSP_IRQ]Receive Dec2 MPEG mode change Int, Mode = %x\n",dReadDspCommDram(ADDR_D2RC_MPEG_MODE_DEC3)>>8);
            }
        }
#endif
        vLogFlowControl(u4DspRIntData);
        u4Msg = u4IntFlowControl(u4DspRIntData); // refine ADSP control       
        break;
#ifdef DSP_SUPPORT_NPTV
    // refine ADSP control
    case INT_DSP_MTS_DETECTED_MODE:
        // we put (u4DspRIntData>>8) in high word of message
        u4Msg = (UINT32)(ADSPTASK_MSG_INT_MTS_MODE)|(u4DspRegR_D2rSda() & 0xFFFF0000L);
        break;
    case INT_DSP_PAL_DETECTED_MODE:
        // we put (u4DspRIntData>>8) in high word of message
        u4Msg = (UINT32)(ADSPTASK_MSG_INT_PAL_MODE)|(u4DspRegR_D2rSda() & 0xFFFF0000L);        
        break;
    case INT_DSP_JPN_DETECTED_MODE:
        // we put (u4DspRIntData>>8) in high word of message
        u4Msg = (UINT32)(ADSPTASK_MSG_INT_JPN_MODE)|(u4DspRegR_D2rSda() & 0xFFFF0000L);        
        break;
    case INT_DSP_MINER_NOTIFY:
        // we put (u4DspRIntData>>8) in high word of message
        u4Msg = (UINT32)(ADSPTASK_MSG_INT_MINER_NOTIFY)|(u4DspRegR_D2rSda() & 0xFFFF0000L);
        break;
    case INT_DSP_DETECTOR_NOTIFY:
        _rDspStatus[AUD_DEC_MAIN].fgDspDetected = TRUE;		
        // Notify middleware
        //_eTvSys = (TV_AUD_SYS_T)wReadDspWORD(ADDR_DETECTED_TYPE);
        // we put wReadDspWORD(ADDR_DETECTED_TYPE) in high word of message
        u4Msg = wReadDspWORD(ADDR_DETECTED_TYPE);
        u4Msg = u4Msg<<16;
        u4Msg = u4Msg|ADSPTASK_MSG_INT_DETECTOR_NOTIFY;
	  break;        
    case INT_DSP_CHANGE_ATV_STANDARD:
        u4Msg = (UINT32)(ADSPTASK_MSG_INT_ATV_CHANGE)|(u4DspRegR_D2rSda() & 0xFFFF0000L);
        LOG(7, "[DSP_IRQ]LOG FOR DECODER DETECTS STANDARD CHANGE\n");
	  break;  
    case INT_DSP_HDEV_AUTO_SWITCH:
        LOG(7, "[DSP_IRQ]High deviation mode auto switch...\n");
        u4Msg = (UINT32)(ADSPTASK_MSG_INT_ATV_HDEV_SWITCH)|(u4DspRegR_D2rSda() & 0xFFFF0000L);
        break;
    case INT_D2RC_FM_RDO_DET_FINISH:
        LOG(7, "[DSP_IRQ]FM Radio channel detection finished..\n");
        u4Msg = (UINT32)(ADSPTASK_MSG_INT_FM_RADIO_DET)|(u4DspRegR_D2rSda() & 0xFFFF0000L);
        break;
#endif
    case INT_D2RC_PTS_1ST_REFRESH:
        _rDspStatus[AUD_DEC_MAIN].fgDspPtsSet = TRUE;
        u4Msg = ADSPTASK_MSG_NO_COMMAND; //Refine ADSP control 
        if ((AUD_GetSyncDbgLvl()&AUD_DBG_SYNC_STC_DIFF) == AUD_DBG_SYNC_STC_DIFF)
        {
            LOG(5, "[DSP_IRQ]DSP interrupt RISC - PTS 1st refresh\n");
        vDspLogAoutEnablePTS();        
        }
        else
        {
            LOG(7, "[DSP_IRQ]DSP interrupt RISC - PTS 1st refresh\n");
        }        	        
        break;
    case INT_D2RC_PTS_1ST_REFRESH_DEC2:
        //_rDspStatus[AUD_DEC_MAIN].fgDspPtsSet = TRUE;
        u4Msg = ADSPTASK_MSG_NO_COMMAND;
        if ((AUD_GetSyncDbgLvl()&AUD_DBG_SYNC_STC_DIFF) == AUD_DBG_SYNC_STC_DIFF)
        {        
    	    LOG(5, "[DSP_IRQ]AUX DSP interrupt RISC - PTS 1st refresh\n");
        vDspLogAoutEnablePTS();        
        }
        else
        {
    	    LOG(7, "[DSP_IRQ]AUX DSP interrupt RISC - PTS 1st refresh\n");            
        }        
        #ifdef MIXSOUND_MIPS_MEASURE
        dwAoutIndex++;
        dwAoutMIPS += u4DspRIntData;
        #endif
        break;
    case INT_D2RC_PTS_1ST_REFRESH_DEC3:
        //_rDspStatus[AUD_DEC_MAIN].fgDspPtsSet = TRUE;
        u4Msg = ADSPTASK_MSG_NO_COMMAND;
        if ((AUD_GetSyncDbgLvl()&AUD_DBG_SYNC_STC_DIFF) == AUD_DBG_SYNC_STC_DIFF)
        {                
    	    LOG(5, "[DSP_IRQ]Third DSP interrupt RISC - PTS 1st refresh\n");
            vDspLogAoutEnablePTS();
        }
        else
        {
    	    LOG(7, "[DSP_IRQ]Third DSP interrupt RISC - PTS 1st refresh\n");            
        }
        break;        
	case INT_D2RC_PCM_DETECT_RAW_MUTE:
    	LOG(1, "[DSP_IRQ]DSP interrupt RISC - PCM detect RAW\n");
    	AUD_PCM_Detect_Raw_Mute(TRUE);
		break;
    case INT_D2RC_MP3_TYPE:
        switch ((u4DspRIntData >> 8))
        {
        case TYPE_INVALID:
        case TYPE_MP2:
             LOG(5, "[DSP_IRQ]Can not play MP2 bitstream\n");
            //_fgDspMpgTypGot = TRUE;
            //_fgDspGetMpgTyp = FALSE;
            //AUD_DSPCmdStop(AUD_DEC_MAIN);
            // notify audio driver about format conflict
            AUD_MM_Set_Dec_Fmt_Conflict(AUD_DEC_MAIN, TRUE);
            break;
        case TYPE_MP3:
            LOG(5, "[DSP_IRQ]Can not play MP3 bitstream\n");            
            // Stop at aout reinit
            // notify audio driver about format conflict
            AUD_MM_Set_Dec_Fmt_Conflict(AUD_DEC_MAIN, TRUE);
            break;
        default:
            break;
        }
        _rDspVars[AUD_DEC_MAIN].dwDspMpgTyp = (u4DspRIntData >> 8);
        u4Msg = ADSPTASK_MSG_NO_COMMAND;
        break;        
    case INT_D2RC_PRINT_DATA:
        if ((AUD_GetSyncDbgLvl()&AUD_DBG_SYNC_PTS_FRESH) == AUD_DBG_SYNC_PTS_FRESH)
        {
            LOG(5, "[DSP_IRQ]%.6x\n",u4DspRIntData);
        }
        else
        {
            LOG(7, "[DSP_IRQ]%.6x\n",u4DspRIntData);
        }
        u4Msg = ADSPTASK_MSG_NO_COMMAND; // Refine ADSP control
    	break;
    case INT_D2RC_DEEMPHASIS_NOTIFY:
    case INT_D2RC_DEEMPHASIS_NOTIFY_DEC2:
        u4Msg = ADSPTASK_MSG_NO_COMMAND;
        break;    	
    case INT_D2RC_LOG_DUMP:    
        vSendADSPTaskData(u4DspDataBackup);//ADSP Data Queue.
        u4Msg = ADSPTASK_MSG_INTERRUPT_DATA;
    default:
        break;
    }
    
    if(u4Msg !=ADSPTASK_MSG_INTERRUPT) // Refine ADSP control
    {
        DSP_ClearInt();
    }
    return u4Msg;
}

/***************************************************************************
     Function : vDSPIntSvc
  Description : DSP Interrupt Service
    Parameter : None
    Return    : None
***************************************************************************/
void vDspIntSvc (void)
{
    UINT8 bDspRIntAddr;
    UINT32 u4DspRIntData;
    UINT32 u4DspRIntData2;
    UINT32 u4DspRIntType;
    INT32 i4DspData;

    //Read Int Addr. and Data
    //u4DspRIntData = (UINT32) ReadREG(RO_D2R_SDA);
    u4DspRIntData = u4DspRegR_D2rSda();
    bDspRIntAddr = (u4DspRIntData & 0xFF);
    u4DspRIntData = (u4DspRIntData >> 8);
    i4DspData = 0;
    
    //Clear DSP Interrupt for MP Version
    //WriteREG(RO_D2R_SDA,0x0);
    DSP_ClearInt();

    //Interrupt Service
    switch (bDspRIntAddr)
    {
    case INT_D2RC_FLOW_CONTROL:
        break;
    case INT_D2RC_DSP_INFO:
        //Extract Mode
        u4DspRIntType = (u4DspRIntData >> 8);
        switch (u4DspRIntType)
        {
        case D2RC_DSP_INFO_AIN_UPDATE_ADC_TUNE_CFG:
            DSP_SetAdcTuneCfg();
        	break;
        default:
        	break;
        }
    	break;
//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
    case INT_D2RC_UPLOAD_INDEX:
    	 _AUD_DspDataUploadNotify (u4DspRIntData >> 8);
        LOG(9, "upload data updated = %d\n", u4DspRIntData >> 8);
        break;
//#endif
#if 1///def CC_VORBIS_SUPPORT
    case INT_D2RC_VORBIS_CODEBOOK:
        DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);
        _uVorbisIntDec = (UINT8)(dReadDspCommDram(ADDR_D2RC_RISC_VORBIS_INT_DEC)>>8);
        if((_rDspVars[AUD_DEC_MAIN].bDspStrTyp == VORBIS_STREAM)||(_rDspVars[AUD_DEC_AUX].bDspStrTyp == VORBIS_STREAM))  
        {
            Printf("***********************************************\n");
            Printf("********** INT_D2RC_VORBIS_CODEBOOK ***********\n");
            Printf("***********************************************\n");

            // Construct the Huffman Tree
            if(fgMakeVorbisCodebook() && (_uVorbisIntDec != 0))
            {
                // set dsp for codebook construction ready
                DspVorbisCodeBookResult(TRUE);
                Printf("*** INT_D2RC_VORBIS_CODEBOOK - PASS ***\n");
            }
            else
            {
                // codebook construction fails --> force reset ?
                /// vDspReset();
                ///[Joel] Use decoder stop to instead of vDspReset();
                DspVorbisCodeBookResult(FALSE);
                Printf("*** INT_D2RC_VORBIS_CODEBOOK - NG ***\n");
            }
        }
      break;
#endif
#if 0 // Move to vDspIntSvc_Data
    case INT_D2RC_LOG_DUMP:
        DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);
    	u4DspRIntType = dReadDspCommDram(ADDR_D2RC_RISC_LOG_DUMP_TYPE);
    	u4DspRIntData = dReadDspCommDram(ADDR_D2RC_RISC_LOG_DUMP_DATA);
    	switch (u4DspRIntType>>8)
    	{
    	case LOG_STC_DIFF:
    		if (u4DspRIntData & 0x800000)
    		{
    		    // (-) negative
    	        //u4DspRIntData = (~u4DspRIntData + 1) & 0xffffff;
    	        i4DspData = (INT32)(u4DspRIntData | 0xFF000000);
    		}
    		else
    		{
    		    // (+) positive
    		    i4DspData = (INT32)u4DspRIntData;
    		}
   	        LOG(9, "Main audio stc_diff (stc-pts) = %d\n", i4DspData);
    	    break;
    	case LOG_STC_DIFF_DEC2:
    		if(u4DspRIntData & 0x800000)
    		{
    		    // (-) negative
    	        //u4DspRIntData = (~u4DspRIntData + 1) & 0xffffff;
    	        i4DspData = (INT32)(u4DspRIntData | 0xFF000000);
    		}
    		else
    		{
    		    // (+) positive
    		    i4DspData = (INT32)u4DspRIntData;
    		}
   	        LOG(9, "Aux audio stc_diff (stc-pts) = %d\n", i4DspData);
    	    break;
    	case LOG_STC_DIFF_DEC3:
    		if( u4DspRIntData&0x800000 )
    		{
    		    // (-) negative
    	        //u4DspRIntData = (~u4DspRIntData + 1) & 0xffffff;
    	        i4DspData = (INT32)(u4DspRIntData | 0xFF000000);
    		}
    		else
    		{
    		    // (+) positive
    		    i4DspData = (INT32)u4DspRIntData;
    		}
   	        LOG(9, "3rd audio stc_diff (stc-pts) = %d\n", i4DspData);
    	    break;
    	case LOG_STC_ADJUST_FRAME:    		
    	    if (!(u4DspRIntData&0x800000))
    	    {    	        
    	        // (+) audio drop occurs
    	        LOG(7, "Main audio drop frame count = %d\n", u4DspRIntData>>8);
    	    }
    	    else
    	    {
    	        u4DspRIntData = (~u4DspRIntData + 1) & 0xffffff;    	            	        
    	        // (-) audio repeat occurs
    	        LOG(7, "Main audio repeat frame count = %d\n", u4DspRIntData>>8);
    	    }
    	    break;
        case LOG_STC_ADJUST_FRAME_DEC2:
    	    if (!(u4DspRIntData&0x800000))
    	    {    	        
    	        // (+) audio drop occurs
    	        LOG(7, "Aux audio drop frame count = %d\n", u4DspRIntData>>8);
    	    }
    	    else
    	    {
    	        u4DspRIntData = (~u4DspRIntData + 1) & 0xffffff;    	            	        
    	        // (-) audio repeat occurs
    	        LOG(7, "Aux audio repeat frame count = %d\n", u4DspRIntData>>8);
    	    }
    	    break;
        case LOG_STC_ADJUST_FRAME_DEC3:
    	    if (!(u4DspRIntData&0x800000))
    	    {    	        
    	        // (+) audio drop occurs
    	        LOG(7, "3rd audio drop frame count = %d\n", u4DspRIntData>>8);
    	    }
    	    else
    	    {
    	        u4DspRIntData = (~u4DspRIntData + 1) & 0xffffff;    	            	        
    	        // (-) audio repeat occurs
    	        LOG(7, "3rd audio repeat frame count = %d\n", u4DspRIntData>>8);
    	    }
    	    break;
    	case LOG_AOUT_OK_ENABLE:
    		u4DspRIntData <<= 8;
    	    u4DspRIntData |= (dReadDspCommDram(ADDR_D2RC_RISC_LOG_DUMP_DATA_2)>>8);
    	    u4DspRIntData2 = dReadDspCommDram(ADDR_D2RC_RISC_LOG_DUMP_DATA_3);    	    
    		u4DspRIntData2 <<= 8;
    	    u4DspRIntData2 |= (dReadDspCommDram(ADDR_D2RC_RISC_LOG_DUMP_DATA_4)>>8);
    		LOG(7, "Main audio start output. (PTS, STC)=(0x%08x, 0x%08x)\n", u4DspRIntData2,u4DspRIntData);
    		#ifdef CHANNEL_CHANGE_LOG
    		HAL_TIME_T dt;
    		HAL_GetTime(&dt);
    		LOG(0, " %u.%06u s [AV SYNC] 4 Main audio start output. (PTS, STC)=(0x%08x, 0x%08x)\n", dt.u4Seconds, dt.u4Micros, u4DspRIntData2,u4DspRIntData);
            #endif    		
    		break;
    	case LOG_AOUT_OK_ENABLE_DEC2:
    		u4DspRIntData <<= 8;
    	    u4DspRIntData |= (dReadDspCommDram(ADDR_D2RC_RISC_LOG_DUMP_DATA_2)>>8);
    	    u4DspRIntData2 = dReadDspCommDram(ADDR_D2RC_RISC_LOG_DUMP_DATA_3);    	    
    		u4DspRIntData2 <<= 8;
    	    u4DspRIntData2 |= (dReadDspCommDram(ADDR_D2RC_RISC_LOG_DUMP_DATA_4)>>8);
    		LOG(7, "Aux audio start output. (PTS, STC)=(0x%08x, 0x%08x)\n", u4DspRIntData2,u4DspRIntData);
    		break;
    	default:
    		break;
    	}
    	break;
#endif // if 0
    default:    
#ifdef ADSP_DEBUG        
        DBGLogB (bDspRIntAddr, u4DspRIntData >> 16, u4DspRIntData >> 8,
                 u4DspRIntData);  
#endif /* ADSP_DEBUG */
        break;
    }

    // Just fix warning message for -O2
    UNUSED(i4DspData);
    UNUSED(u4DspRIntData2);//ADSP Data Queue.    
}

/***************************************************************************
     Function : vDSPIntSvc_Data
  Description : DSP Interrupt Service with Data in queue (don't need data in register)
                Get Data in ISR and put in data queue.
                Then we can clear RISC busy in ISR, and handle the log in adsp thread.
    Parameter : None
    Return    : None
***************************************************************************/
void vDspIntSvc_Data (void)     //ADSP Data Queue.
{
    UINT8 bDspRIntAddr;
    UINT32 u4DspRIntData;
    UINT32 u4DspRIntData2;
    UINT32 u4DspRIntType;
    INT32 i4DspData;

    vGetADSPTaskData(&u4DspRIntData); //ADSP Data Queue.
    
    //Read Int Addr. and Data
    //u4DspRIntData = u4DspRegR_D2rSda(); //From Data queue
    bDspRIntAddr = (u4DspRIntData & 0xFF);
    u4DspRIntData = (u4DspRIntData >> 8);
    i4DspData = 0;
    
    //Clear DSP Interrupt in IRQ already.
    
    //Interrupt Service
    switch (bDspRIntAddr)
    {
    case INT_D2RC_LOG_DUMP:
        DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);
    	u4DspRIntType = dReadDspCommDram(ADDR_D2RC_RISC_LOG_DUMP_TYPE);
    	u4DspRIntData = dReadDspCommDram(ADDR_D2RC_RISC_LOG_DUMP_DATA);
    	switch (u4DspRIntType>>8)
    	{
    	case LOG_STC_DIFF:
    		if (u4DspRIntData & 0x800000)
    		{
    		    // (-) negative
    	        //u4DspRIntData = (~u4DspRIntData + 1) & 0xffffff;
    	        i4DspData = (INT32)(u4DspRIntData | 0xFF000000);
    		}
    		else
    		{
    		    // (+) positive
    		    i4DspData = (INT32)u4DspRIntData;
    		}
   	        LOG(9, "[DSP_TASK]Main audio stc_diff (stc-pts) = %d\n", i4DspData);
    	    break;
    	case LOG_STC_DIFF_DEC2:
    		if(u4DspRIntData & 0x800000)
    		{
    		    // (-) negative
    	        //u4DspRIntData = (~u4DspRIntData + 1) & 0xffffff;
    	        i4DspData = (INT32)(u4DspRIntData | 0xFF000000);
    		}
    		else
    		{
    		    // (+) positive
    		    i4DspData = (INT32)u4DspRIntData;
    		}
   	        LOG(9, "[DSP_TASK]Aux audio stc_diff (stc-pts) = %d\n", i4DspData);
    	    break;
    	case LOG_STC_DIFF_DEC3:
    		if( u4DspRIntData&0x800000 )
    		{
    		    // (-) negative
    	        //u4DspRIntData = (~u4DspRIntData + 1) & 0xffffff;
    	        i4DspData = (INT32)(u4DspRIntData | 0xFF000000);
    		}
    		else
    		{
    		    // (+) positive
    		    i4DspData = (INT32)u4DspRIntData;
    		}
   	        LOG(9, "[DSP_TASK]3rd audio stc_diff (stc-pts) = %d\n", i4DspData);
    	    break;
    	case LOG_STC_ADJUST_FRAME:    		
    	    if (!(u4DspRIntData&0x800000))
    	    {    	        
    	        // (+) audio drop occurs
    	        LOG(8, "[DSP_TASK]Main audio drop frame count = %d\n", u4DspRIntData>>8);
    	    }
    	    else
    	    {
    	        u4DspRIntData = (~u4DspRIntData + 1) & 0xffffff;    	            	        
    	        // (-) audio repeat occurs
    	        LOG(8, "[DSP_TASK]Main audio repeat frame count = %d\n", u4DspRIntData>>8);
    	    }
    	    break;
        case LOG_STC_ADJUST_FRAME_DEC2:
    	    if (!(u4DspRIntData&0x800000))
    	    {    	        
    	        // (+) audio drop occurs
    	        LOG(8, "[DSP_TASK]Aux audio drop frame count = %d\n", u4DspRIntData>>8);
    	    }
    	    else
    	    {
    	        u4DspRIntData = (~u4DspRIntData + 1) & 0xffffff;    	            	        
    	        // (-) audio repeat occurs
    	        LOG(8, "[DSP_TASK]Aux audio repeat frame count = %d\n", u4DspRIntData>>8);
    	    }
    	    break;
        case LOG_STC_ADJUST_FRAME_DEC3:
    	    if (!(u4DspRIntData&0x800000))
    	    {    	        
    	        // (+) audio drop occurs
    	        LOG(8, "[DSP_TASK]3rd audio drop frame count = %d\n", u4DspRIntData>>8);
    	    }
    	    else
    	    {
    	        u4DspRIntData = (~u4DspRIntData + 1) & 0xffffff;    	            	        
    	        // (-) audio repeat occurs
    	        LOG(8, "[DSP_TASK]3rd audio repeat frame count = %d\n", u4DspRIntData>>8);
    	    }
    	    break;
    	case LOG_AOUT_OK_ENABLE:
    		u4DspRIntData <<= 8;
    	    u4DspRIntData |= (dReadDspCommDram(ADDR_D2RC_RISC_LOG_DUMP_DATA_2)>>8);
    	    u4DspRIntData2 = dReadDspCommDram(ADDR_D2RC_RISC_LOG_DUMP_DATA_3);    	    
    		u4DspRIntData2 <<= 8;
    	    u4DspRIntData2 |= (dReadDspCommDram(ADDR_D2RC_RISC_LOG_DUMP_DATA_4)>>8);
    		LOG(7, "[DSP_TASK]Main audio start output. (PTS, STC)=(0x%08x, 0x%08x)\n", u4DspRIntData2,u4DspRIntData);
    		#ifdef CHANNEL_CHANGE_LOG
    		HAL_TIME_T dt;
    		HAL_GetTime(&dt);
    		LOG(0, "[DSP_TASK] %u.%06u s [AV SYNC] 4 Main audio start output. (PTS, STC)=(0x%08x, 0x%08x)\n", dt.u4Seconds, dt.u4Micros, u4DspRIntData2,u4DspRIntData);
            #endif    		
    		break;
    	case LOG_AOUT_OK_ENABLE_DEC2:
    		u4DspRIntData <<= 8;
    	    u4DspRIntData |= (dReadDspCommDram(ADDR_D2RC_RISC_LOG_DUMP_DATA_2)>>8);
    	    u4DspRIntData2 = dReadDspCommDram(ADDR_D2RC_RISC_LOG_DUMP_DATA_3);    	    
    		u4DspRIntData2 <<= 8;
    	    u4DspRIntData2 |= (dReadDspCommDram(ADDR_D2RC_RISC_LOG_DUMP_DATA_4)>>8);
    		LOG(7, "[DSP_TASK]Aux audio start output. (PTS, STC)=(0x%08x, 0x%08x)\n", u4DspRIntData2,u4DspRIntData);
    		break;
    	default:
    		break;
    	}
    	break;
    default:    
#ifdef ADSP_DEBUG        
        DBGLogB (bDspRIntAddr, u4DspRIntData >> 16, u4DspRIntData >> 8,
                 u4DspRIntData);  
#endif /* ADSP_DEBUG */
        break;
    }

    // Just fix warning message for -O2
    UNUSED(i4DspData);
}
// Refine ADSP control
/*
 * flow control handler within ADSP interrupt
 * u4DspRIntData: interrupt for flow control
 * return: message if send interupt to main handler
*/
UINT32 u4IntFlowControl(UINT32 u4DspRIntData)
{
    UINT32 u4RetMsg=ADSPTASK_MSG_INTERRUPT;
    switch(u4DspRIntData)
    {
        case D2RC_FLOW_CONTROL_AOUT_ENABLE:
            vDspAOutEnable(AUD_DEC_MAIN);
            u4RetMsg = ADSPTASK_MSG_INT_AOUT_ENABLE;
            LOG(5, "[DSP_IRQ]Dec0 Aout Enable\n");
        break;
        case D2RC_FLOW_CONTROL_AOUT_ENABLE_DEC2:
            vDspAOutEnable(AUD_DEC_AUX);
            u4RetMsg = ADSPTASK_MSG_INT_AOUT_ENABLE_DEC2;
            LOG(5, "[DSP_IRQ]Dec1 Aout Enable\n"); 
        break;
        case D2RC_FLOW_CONTROL_FLUSH_DONE:
            vDspFlushDone(AUD_DEC_MAIN);
            u4RetMsg = ADSPTASK_MSG_INT_STOP;
            LOG(5, "[DSP_IRQ]Dec0 flush done\n"); 
        break;
        case D2RC_FLOW_CONTROL_FLUSH_DONE_DEC2:
            vDspFlushDone(AUD_DEC_AUX);
            u4RetMsg = ADSPTASK_MSG_INT_STOP_DEC2;
            LOG(5, "[DSP_IRQ]Dec1 flush done\n"); 
        break;
        case D2RC_FLOW_CONTROL_UPDATE_EFFECT:
            u4RetMsg = ADSPTASK_MSG_INT_UPDATE_EFFECT;
            LOG(5, "[DSP_IRQ]Dec0 flow control update effect\n"); 
        break;
        case D2RC_FLOW_CONTROL_SAMPLING_RATE:
            u4RetMsg = ADSPTASK_MSG_INT_SAMPLE_RATE;
            LOG(5, "[DSP_IRQ]Dec0 set sampling rate\n"); 
        break;
        case D2RC_FLOW_CONTROL_SAMPLING_RATE_DEC2:
            u4RetMsg = ADSPTASK_MSG_INT_SAMPLE_RATE_DEC2;
            LOG(5, "[DSP_IRQ]Dec1 set sampling rate\n"); 
        break;
        case D2RC_FLOW_CONTROL_MODE_CHANGE:
            u4RetMsg = ADSPTASK_MSG_INT_MODE_CHANGE;
            LOG(5, "[DSP_IRQ]Dec0 mode change\n");             
        break;
        case D2RC_FLOW_CONTROL_MODE_CHANGE_DEC2:
            u4RetMsg = ADSPTASK_MSG_INT_MODE_CHANGE_DEC2;
            LOG(5, "[DSP_IRQ]Dec1 mode change\n");             
        break;        
        case D2RC_FLOW_CONTROL_PAUSE_OK:
        case D2RC_FLOW_CONTROL_PAUSE_OK_DEC2:
        // none handling
        break;
#ifdef CC_MT5391_AUD_3_DECODER
        case D2RC_FLOW_CONTROL_AOUT_ENABLE_DEC3:
            vDspAOutEnable(AUD_DEC_THIRD);
            u4RetMsg = ADSPTASK_MSG_INT_AOUT_ENABLE_DEC3;
            LOG(5, "[DSP_IRQ]Dec2 Aout Enable\n"); 
            //if (AUD_IsDualDecMode())
            //{
            //    AUD_DualDecSetSyncSema(FALSE);
            //}  
            break;   
        case D2RC_FLOW_CONTROL_FLUSH_DONE_DEC3:
            vDspFlushDone(AUD_DEC_THIRD);
            u4RetMsg = ADSPTASK_MSG_INT_STOP_DEC3;
            LOG(5, "[DSP_IRQ]Dec2 flush done\n"); 
            break;    
        case D2RC_FLOW_CONTROL_SAMPLING_RATE_DEC3:
            u4RetMsg = ADSPTASK_MSG_INT_SAMPLE_RATE_DEC3;
            LOG(5, "[DSP_IRQ]Dec2 set sampling rate\n");
            break;    
        case D2RC_FLOW_CONTROL_MODE_CHANGE_DEC3:
            u4RetMsg = ADSPTASK_MSG_INT_MODE_CHANGE_DEC3;
            LOG(5, "[DSP_IRQ]Dec2 mode change\n");                         
            break;            
#endif
        case D2RC_FLOW_CONTROL_SAMPLING_RATE_CHANGE:
            u4RetMsg = ADSPTASK_MSG_INT_SAMPLING_RATE_CHANGE;
            LOG(5, "[DSP_IRQ]Dec0 decoder's sampling rate change\n");                         
            break;
        case D2RC_FLOW_CONTROL_SAMPLING_RATE_CHANGE_DEC2:
            u4RetMsg = ADSPTASK_MSG_INT_SAMPLING_RATE_CHANGE_DEC2;
            LOG(5, "[DSP_IRQ]Dec1 decoder's sampling rate change\n");                         
            break;
#ifdef CC_AUD_4_DECODER_SUPPORT
        case D2RC_FLOW_CONTROL_AOUT_ENABLE_DEC4:
            vDspAOutEnable(AUD_DEC_4TH);
            u4RetMsg = ADSPTASK_MSG_INT_AOUT_ENABLE_DEC4;
            LOG(5, "[DSP_IRQ]Dec3 Aout Enable\n"); 
            break;   
        case D2RC_FLOW_CONTROL_FLUSH_DONE_DEC4:
            vDspFlushDone(AUD_DEC_4TH);
            u4RetMsg = ADSPTASK_MSG_INT_STOP_DEC4;
            LOG(5, "[DSP_IRQ]Dec3 flush done\n"); 
            break;    
        case D2RC_FLOW_CONTROL_SAMPLING_RATE_DEC4:
            u4RetMsg = ADSPTASK_MSG_INT_SAMPLE_RATE_DEC4;
            LOG(5, "[DSP_IRQ]Dec3 set sampling rate\n");
            break;    
        case D2RC_FLOW_CONTROL_MODE_CHANGE_DEC4:
            u4RetMsg = ADSPTASK_MSG_INT_MODE_CHANGE_DEC4;
            LOG(5, "[DSP_IRQ]Dec3 mode change\n");                         
            break;            
#endif
        default:
            break;
    }
    return(u4RetMsg);
}

void vDspLogAoutEnablePTS(void)
{
    UINT32 u4Data1;
    UINT32 u4Data2;
    
    UNUSED(vDspLogAoutEnablePTS);
    UNUSED(u4Data1);
    UNUSED(u4Data2);
    DSP_FlushInvalidateDCacheSmall2(ADDR_D2RC_RISC_LOG_DUMP_DATA);    

    u4Data1 = dReadDspCommDram(ADDR_D2RC_RISC_LOG_DUMP_DATA);
    u4Data1 <<= 8;
    u4Data1 |= (dReadDspCommDram(ADDR_D2RC_RISC_LOG_DUMP_DATA_2)>>8);
    u4Data2 = dReadDspCommDram(ADDR_D2RC_RISC_LOG_DUMP_DATA_3);    	    
    u4Data2 <<= 8;
    u4Data2 |= (dReadDspCommDram(ADDR_D2RC_RISC_LOG_DUMP_DATA_4)>>8);
    
    if ((AUD_GetSyncDbgLvl()&AUD_DBG_SYNC_STC_DIFF) == AUD_DBG_SYNC_STC_DIFF)
    {
        LOG(5, "[DSP_IRQ](PTS,STC) = (0x%x,0x%x),ONLY for Sys-Master play dbg ref.\n", u4Data2,u4Data1);
    }
}

