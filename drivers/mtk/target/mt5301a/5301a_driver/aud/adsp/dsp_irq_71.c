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
/***************  $Revision: #1 $       ****************/
/***************                                       ****************/
/***************   Description : DSP Control routines  ****************/
/***************                                       ****************/
/***************       Company : MediaTek Inc.         ****************/
/***************    Programmer : Jack Hsu              ****************/
/***************                 Peichen Chang         ****************/
/**********************************************************************/
#define _DSP_IRQ_C

#include "dsp_general.h"
#include "dsp_reg.h"
#include "dsp_func.h"
#include "dsp_uop.h"
#include "dsp_shm.h"
#include "dsp_intf.h"
#include "d2rc_shm.h"
#include "drv_adsp.h"
#include "dsp_rg_ctl.h"
#include "aud_drv.h"

#include "aud_debug.h"

#include "x_hal_926.h"
#ifdef CHANNEL_CHANGE_LOG
#include "x_timer.h"
#endif
void vDspIntSvc (void);

static TV_AUD_SYS_T _eTvSys = SV_NONE_DETECTED;

UINT32 u4DspIRQSvc (void)
{
    UINT32 u4DspRIntData;
    UINT8 bDspRIntAddr;
    UINT32 u4Msg = ADSPTASK_MSG_INTERRUPT;

    //u4DspRIntData = ReadREG(RO_D2R_SDA);
    u4DspRIntData = u4DspRegR_D2rSda();
    bDspRIntAddr = (u4DspRIntData & 0xFF);
    u4DspRIntData = (u4DspRIntData >> 8);

    switch (bDspRIntAddr)
    {
    case INT_D2RC_WAKEUP_OK:
        _fgDspWakeupOK = TRUE;
        break;
   case INT_DSP_DETECTOR_NOTIFY:
        _fgDspDetected = TRUE;		
        // Notify middleware
        _eTvSys = (TV_AUD_SYS_T)wReadDspWORD(ADDR_DETECTED_TYPE);	
	  break;     
    case INT_D2RC_FLOW_CONTROL:  /* Just for LOG */  
        u4DspRIntData = (u4DspRIntData >> 8);
        HalFlushInvalidateDCache(); //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro
        if (u4DspRIntData==D2RC_FLOW_CONTROL_MODE_CHANGE) 
        {
            if (_bDspStrTyp == AC3_STREAM)
            {
                LOG(7, "Receive AC3 ACMOD change Int, ACMOD = %x\n",dReadDspCommDram(ADDR_D2RC_AC3_ACMOD)>>8);
            }
            else if (_bDspStrTyp == MPEG12_STREAM)
            {
                LOG(7, "Receive MPEG mode change Int, Mode = %x\n",dReadDspCommDram(ADDR_D2RC_MP2_MODE)>>8);
            }
        }
        else if (u4DspRIntData==D2RC_FLOW_CONTROL_MODE_CHANGE_DEC2) 
        {
            if (_bDspStrTyp == AC3_STREAM)
            {
                LOG(7, "Receive DEC2 AC3 ACMOD change Int, ACMOD = %x\n",dReadDspCommDram(ADDR_D2RC_AC3_ACMOD_DEC2)>>8);
            }
            else if (_bDspStrTyp == MPEG12_STREAM)
            {
                LOG(7, "Receive DEC2 MPEG mode change Int, Mode = %x\n",dReadDspCommDram(ADDR_D2RC_MP2_MODE_DEC2)>>8);
            }
        }
        
        if (u4DspRIntData == D2RC_FLOW_CONTROL_AOUT_ENABLE)
        {
            vDspAOutEnable();
            u4Msg = ADSPTASK_MSG_AOUT_ENABLE;
            LOG(5, "Dec1 Aout Enable\n");
        }
        else if (u4DspRIntData == D2RC_FLOW_CONTROL_AOUT_ENABLE_DEC2)
        {
            vDspAOutEnableDec2();
            u4Msg = ADSPTASK_MSG_AOUT_ENABLE_DEC2;
            LOG(5, "Dec2 Aout Enable\n");
            if (AUD_IsDualDecMode())
            {
                AUD_DualDecSetSyncSema(FALSE);
            }
        }
        if (u4DspRIntData == D2RC_FLOW_CONTROL_UPDATE_EFFECT)
        {
        	DSP_ClearInt();
        	u4Msg = ADSPTASK_MSG_UPDATE_EFFECT;
        }

        vLogFlowControl(u4DspRIntData);
            
        break;
#ifdef DSP_SUPPORT_NPTV
    case INT_DSP_MTS_DETECTED_MODE:  //MTS mode changed INT
        /*
        MTS_MONO      0x01
        MTS_STEREO    0x02
        MTS_SAP       0x04
        */
        switch ((u4DspRIntData >> 8))
        {
        case MTS_DETECTED_STEREO_SAP:
            //Notify UI the detected MTS modes.
            //((MTS_STEREO | MTS_SAP | MTS_MONO));
            break;
        case MTS_DETECTED_STEREO:
            //((MTS_STEREO | MTS_MONO));
            break;
        case MTS_DETECTED_SAP:
            //((MTS_SAP | MTS_MONO));
            break;
        case MTS_DETECTED_MONO:
            //(MTS_MONO);
            break;
        default: //exception
            break;
        }
        break;
#endif
    case INT_D2RC_PRINT_DATA:
            LOG(7, "%.6x\n",u4DspRIntData);
    	break;
    default:
        break;
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

#ifdef EMULATION_FPGA
    UINT32 dwDspReqData;
#endif
#ifdef VORBIS_SUPPORT
    UINT32 dwOfst, dwSz, dwFlashOfst;
    UINT8 *pbDspBuf;
#endif

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
#ifdef DSP_SUPPORT_NPTV
    case INT_DSP_MTS_DETECTED_MODE:
    case INT_DSP_PAL_DETECTED_MODE:
    case INT_DSP_JPN_DETECTED_MODE:    
    case INT_DSP_MINER_NOTIFY:
        vDspDemodDetModeNotify(bDspRIntAddr, (u4DspRIntData >> 8));
        break;
    case INT_DSP_DETECTOR_NOTIFY:	
        // Notify middleware
        vDspTvSysDetectedNotify(_eTvSys); 
        break;
#endif
    case INT_D2RC_MP3_TYPE:
        switch ((u4DspRIntData >> 8))
        {
        case TYPE_INVALID:
        case TYPE_MP2:
            _fgDspMpgTypGot = TRUE;
            _fgDspGetMpgTyp = FALSE;
            vDspCmd (DSP_STOP);
            break;
        case TYPE_MP3:
            // Stop at aout reinit
            break;
        default:
            break;
        }
        _dwDspMpgTyp = (u4DspRIntData >> 8);
        break;
    case INT_D2RC_DEEMPHASIS_NOTIFY:
        switch ((u4DspRIntData >> 8))
        {
        case DEEMPH_ENABLE:
            vSetAdac (ADAC_CMD_EMPH, TRUE, 0);
            break;
        case DEEMPH_DISABLE:
            vSetAdac (ADAC_CMD_EMPH, FALSE, 0);
            break;
        default:
            break;
        }
        break;
    case INT_D2RC_FLOW_CONTROL:
        //Extract Mode
        u4DspRIntData = (u4DspRIntData >> 8);

        UNUSED((fgIntFlowControl (u4DspRIntData)));
        break;
    case INT_D2RC_PTS_1ST_REFRESH:
        _fgDspPtsSet = TRUE;
    	LOG(9, "DSP interrupt RISC - PTS 1st refresh\n"); 
        break;
    case INT_D2RC_WAKEUP_OK:
        //To be added
        //DSP wake up OK
        // ...
        //Set Bonding Options
        _fgDspSInt = TRUE;
        _uDspSIntAddr = INT_RC2D_BONDING_OPTIONS;
/////////////////////////////////////////////////////////////////////////////////////
// Bonding Options
//
//    bit 0: Channel Setting 5.1ch(0)/2ch(1)
//    bit 1: Channel 6 (Center Back) Enable(0)/Disable(1)
//    bit 2: Channel 7 Enable(0)/Disable(1)
//    bit 3: (Reversed)
//    bit 4: DTS Decoding Enable(0)/Disable(1)
//    bit 5: WMA Decoding Enable(0)/Disable(1)
//    bit 6: HDCD Decoding Enable(0)/Disable(1)
//    bit 7: MLP Decoding Enable(0)/Disable(1)
//    bit 8: SACD Decodinng Enable(0)/Disable(1)
//    bit 9~15: (Reserved)
/////////////////////////////////////////////////////////////////////////////////////
        _u4DspSIntSD = ((_u4Bond & 0xFFFF) << 8);
	 LOG(9,"DSP interrupt RISC - wake up ok\n"); // Light added for interrupt log
        break;
#ifdef EMULATION_FPGA
    case INT_D2RC_READ_REG:
        // Warning !! u4DspRIntData only 3 bytes
        //dwDspReqData = ReadREG (u4DspRIntData);
        dwDspReqData = u4DspRegR_ByArguement(u4DspRIntData);
        vSendDspLongInt (INT_RC2D_RISC_ACK, 0x100, dwDspReqData);
        break;

    case INT_D2RC_WRITE_REG_ADR:
        // Warning !! u4DspRIntData only 3 bytes
        //WriteREG (u4DspRIntData, _u4DspWRiscData);
        vDspRegW_ByArguement(u4DspRIntData, _u4DspWRiscData);
        break;

    case INT_D2RC_WRITE_REG_DATA:
        _u4DspWRiscData = dwReadDspLongData () & 0xFF;
        _u4DspWRiscData = _u4DspWRiscData | (u4DspRIntData << 8);
        break;
    case INT_D2RC_PRINT_DRAM_LEN:
        _u4DspDumpDramLen = u4DspRIntData;
        break;
    case INT_D2RC_PRINT_DRAM_ADR:
        DumpDram (_u4DspDumpDramLen, u4DspRIntData);
        //SendDSPInt (INT_RC2D_RISC_ACK, 0x100);
        {
            UINT32 dwCnt = 0;

            while (1)
            {
                SLEEP (0);
                if (!fgDspBusy ())
                {
                    SendDSPInt (INT_RC2D_RISC_ACK, 0x100);
                    break;
                }
                dwCnt++;
                if (dwCnt > 0x500)
                {
                    SendDSPInt (INT_RC2D_RISC_ACK, 0x100);
                    _dwDspSIntFail++;
                    break;
                }
            }
        }
        break;
    case INT_D2RC_COMPARE_RESULT:
        if (fgCheckCompareResult ())
        {
            // Result is correct, continue going
            UINT32 dwCnt = 0;

            while (1)
            {
                SLEEP (0);
                if (!fgDspBusy ())
                {
                    SendDSPInt (INT_RC2D_RISC_ACK, 0x100);
                    break;
                }
                dwCnt++;
                if (dwCnt > 0x500)
                {
                    SendDSPInt (INT_RC2D_RISC_ACK, 0x100);
                    _dwDspSIntFail++;
                    break;
                }
            }
        }
        break;
#endif // EMULATION_FPGA
#ifdef VORBIS_SUPPORT
    case INT_D2RC_VORBIS_CODEBOOK:
        if (_bDspStrTyp == VORBIS_STREAM)
        {
            // Construct the Huffman Tree
            if (fgMakeVorbisCodebook ())
            {
                // set dsp for codebook construction ready
                SendDSPInt (INT_RC2D_VORBIS_TABLE, (UINT32) VORBIS_CODEBOOK_READY);   // short data & interrupt
            }
            else
            {
                // codebook construction fails --> force reset ?
                vDspResetDec1 ();
            }
        }
        break;
#endif
#ifdef IC_VERSION_A
    case INT_D2RC_DSP_INFO:
        //Extract Mode
        u4DspRIntType = (u4DspRIntData >> 8);
        switch (u4DspRIntType)
        {
        case D2RC_DSP_INFO_AIN_UPDATE_ADC_TUNE_CFG:

            if (BSP_GetIcVersion() == IC_VER_AA)
            {
                DSP_UpdateAdcTuneCfg_AA();
            }
            else if (BSP_GetIcVersion() >= IC_VER_AB)
            {
                DSP_UpdateAdcTuneCfg_AB();
            }
        	break;
        default:
        	break;
        }
    	break;
#endif
    case INT_D2RC_LOG_DUMP:
       HalFlushInvalidateDCache(); //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro    
    	u4DspRIntType = dReadDspCommDram(ADDR_D2RC_RISC_LOG_DUMP_TYPE);
    	u4DspRIntData = dReadDspCommDram(ADDR_D2RC_RISC_LOG_DUMP_DATA);
    	switch (u4DspRIntType>>8)
    	{
    	case LOG_STC_DIFF:
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
   	        LOG(9, "Main audio stc_diff (stc-pts) = %d\n", i4DspData);
    	    break;
    	case LOG_STC_DIFF_DEC2:
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
   	        LOG(9, "Aux audio stc_diff (stc-pts) = %d\n", i4DspData);
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

