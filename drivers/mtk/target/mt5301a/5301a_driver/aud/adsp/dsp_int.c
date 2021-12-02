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
/***************  $Modtime:: 04/12/06 4:57p    $       ****************/
/***************  $Revision: #1 $       ****************/
/***************                                       ****************/
/***************   Description : DSP Control routines  ****************/
/***************                                       ****************/
/***************       Company : MediaTek Inc.         ****************/
/***************    Programmer : Bowen Ku              ****************/
/***************                 Jerry Shieh           ****************/
/***************                 Harrison Hsieh        ****************/
/***************                 Peichen Chang         ****************/
/***************                 SM Lee                ****************/
/**********************************************************************/
#define _DSP_INT_C

#include "dsp_general.h"
#include "dsp_reg.h"
#include "dsp_intf.h"
#include "d2rc_int.h"    // only use in this files
#include "dsp_func.h"
#include "dsp_table.h"
#include "dsp_uop.h"
#include "dsp_shm.h"
#include "dsp_data.h"
#include "dsp_rg_ctl.h"
#include "d2rc_shm.h"
#include "dsp_intf.h"

#include "aud_if.h"
#include "aud_debug.h"
#include "aud_drv.h"

#include "x_assert.h"
#include "x_hal_926.h"

void vDspSetTable (UINT32 u4FreqIdx);
void vDspSetTableDec2 (UINT32 u4FreqIdx);
void vDspSetSamplingRate (void);
void vDspSetSamplingRateDec2 (void);
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
void vDspSetTableDec3 (UINT32 u4FreqIdx);
void vDspSetSamplingRateDec3 (void);
#endif

#ifdef CC_AUD_BBE_SUPPORT
static void vSetBbeTable (UINT32 u4FreqIdx, BOOL fgFirstDecoder);
#endif

static void vSetBassManageTable (UINT32 u4FreqIdx, BOOL fgFirstDecoder);

#ifdef DSP_POST_PROCESS_ENABLE
static void vSetPostProcTable (UINT32 u4FreqIdx, BOOL fgFirstDecoder);
static void vSetupUpMix (UINT32 u4FreqIdx, BOOL fgFirstDecoder);
static void vSetupDownMix (UINT32 u4FreqIdx, BOOL fgFirstDecoder);
#endif

//extern void _AUD_DspSetDDCOEnable(BOOL fgEnable);
//extern AUD_IEC_T _GetIECFlag(UINT8 u1DecId);



// The buffer address is DSP relative (0x1e50)
#define DRAM_CMPT_HPF_BUFFER                 (DRAM_CMPT_BASE + 0x1e50)

UINT8 FC_TO_TABLE_INDEX(UINT16 u2Fc)
{
    // Check support range
    u2Fc = (u2Fc > HPF_FC_END) ? HPF_FC_END : u2Fc;
    u2Fc = (u2Fc < HPF_FC_STR) ? HPF_FC_STR : u2Fc;

    // Transfer to table index
    return ((u2Fc - HPF_FC_STR) / HPF_FC_STEP);
}

UINT32 dwGetPEQFsIdx(UINT8 u1Set)
{
    UINT32 u4FsIdx = 0;
    UINT32 u4FsIdx1 = 0;
    
#ifndef CC_NEW_POST_SUPPORT
    switch (_dwDspFreq)
    {
    case SFREQ_16K:                 
        u4FsIdx=16000;
        break;
    case SFREQ_32K:                
        u4FsIdx=0x83126;
        break;
    case SFREQ_8K:                  
        u4FsIdx=8000;
        break;
    case SFREQ_11K: 
        u4FsIdx=11000;
        break;
    case SFREQ_12K:
        u4FsIdx=12000;
        break;
    case SFREQ_22K:              
        u4FsIdx=22000;
        break;
    case SFREQ_44K:                   
        u4FsIdx=0x5f1be;
        break;
    case SFREQ_88K:                   
        u4FsIdx=88000;
        break;
    case SFREQ_176K:                 
        u4FsIdx=176000;
        break;
    case SFREQ_24K:                   
        u4FsIdx=24000;
        break;
    case SFREQ_48K:                   
        u4FsIdx=0x57619;// (1/48000)*(180/45)*512
        break;
    case SFREQ_96K:                   
        u4FsIdx=96000;
        break;
    case SFREQ_192K:                  
        u4FsIdx=192000;
        break;
    default:
        u4FsIdx=88000;
        break;
    }

    u4FsIdx1 = u2ReadDspShmWORD(W_PEQ_SET0_BW + 8*u1Set) * u4FsIdx;

    u4FsIdx1 = u4FsIdx1>>19;
    u4FsIdx1 = u4FsIdx1+0x08;
    u4FsIdx1 = u4FsIdx1>>4;

    LOG(7, "PEQ(%d) u4FsIdx = %d\n", u1Set,u4FsIdx1);
    
    // Check index range
    //if (u4FsIdx1 > sizeof(MTK_TAN_TABLE))
    if (u4FsIdx1 >= 512)
    {
        //u4FsIdx1 = sizeof(MTK_TAN_TABLE) - 1;
        u4FsIdx1 = 511;
        LOG(7, "Error PEQ BW idx\n");
    }
    return u4FsIdx1;   
#else
    switch (_dwDspFreq)
    {
    case SFREQ_16K:				 
        u4FsIdx=16000;
        break;
    case SFREQ_32K:				
        u4FsIdx=0x20c49;
        break;
    case SFREQ_8K: 				 
        u4FsIdx=8000;
        break;
    case SFREQ_11K: 
        u4FsIdx=11000;
        break;
    case SFREQ_12K:
        u4FsIdx=12000;
        break;
    case SFREQ_22K:			  
        u4FsIdx=22000;
        break;
    case SFREQ_44K:				   
        u4FsIdx=0x17c6f;
        break;
    case SFREQ_88K:				   
        u4FsIdx=88000;
        break;
    case SFREQ_176K:				  
        u4FsIdx=176000;
        break;
    case SFREQ_24K:				   
        u4FsIdx=24000;
        break;
    case SFREQ_48K:				   
        u4FsIdx=0x15d86;// (1/48000)*(180/45)*512
        break;
    case SFREQ_96K:				   
        u4FsIdx=96000;
        break;
    case SFREQ_192K:				   
        u4FsIdx=192000;
        break;
    default:
        u4FsIdx=88000;
        break;
    }

    u4FsIdx1 = u2ReadDspShmWORD(W_PEQ_SET0_BW + 8*u1Set) * u4FsIdx;
    u4FsIdx1 = u4FsIdx1>>16;
    u4FsIdx1 = u4FsIdx1+0x08;
    u4FsIdx1 = u4FsIdx1>>1;

    LOG(7, "PEQu4FsIdx = %x\n",u4FsIdx1);
 
    // Check index range
    if (u4FsIdx1 > sizeof(MTK_TAN_TABLE)/4)
    {
        u4FsIdx1 = sizeof(MTK_TAN_TABLE)/4 - 1;
        LOG(7, "Error PEQ BW idx\n");
    }
 
    return u4FsIdx1;
#endif
}

void vLoadPEQTable(void)
{
    UINT32 i;

    // Use _dwDspFreq instead
    //UNUSED(u4FreqIdx);
#ifndef CC_NEW_POST_SUPPORT
    for (i=0; i<8; i++)
    {   
        UINT32 u4Index;
        
        u4Index = dwGetPEQFsIdx(i);
        VERIFY(u4Index < 512);
   	    WriteDspCommDram (ADDR_PEQ1_BW + i*3, MTK_TAN_TABLE[u4Index]);
    }    
#else
    for (i=0; i<9; i++)
    {   
        UINT32 u4Index;
        u4Index = dwGetPEQFsIdx(i);
        VERIFY(u4Index < sizeof(MTK_TAN_TABLE)/4);
   	    WriteDspCommDram (ADDR_PEQ1_BW + i*3, MTK_TAN_TABLE[u4Index] );    
    } 
#endif	
}
#ifdef CC_AUD_SPEAKER_HEIGHT_MODE
void vLoadSPHTable(void)
{
	 UINT8 u1ModeIdx;	 
	 UINT32 iLoopVar;
	 UINT32 *pdwTable = NULL;
		
	u1ModeIdx = uReadDspShmBYTE(B_SPH_MODE);
	
	if (u1ModeIdx ==1)
	  pdwTable = (UINT32 *)&MTK_SHELF_2DB_TABLE[0];
  else if (u1ModeIdx == 2)
		pdwTable = (UINT32 *)&MTK_SHELF_4DB_TABLE[0];
	else if (u1ModeIdx == 3)
		pdwTable = (UINT32 *)&MTK_SHELF_6DB_TABLE[0];
	
	if (pdwTable == NULL)
	  return;
	
	for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
	{
		WriteDspCommDram ((ADDR_SHELF_R_COEF + iLoopVar),
						  pdwTable[iLoopVar]);
		WriteDspCommDram ((ADDR_SHELF_L_COEF + iLoopVar),
						  pdwTable[iLoopVar]);
	}
	
}
#endif

#ifdef CC_NEW_POST_MULTI_VS_SUPPORT
void vLoadVirtualSurroundTable(UINT32 u4FreqIdx)
{
	UINT16 u2ModeIdx;	
	UINT32 iLoopVar;
	UINT32 *pdwTable;


   u2ModeIdx = u2ReadDspShmWORD(W_VSURRFLAG);
	
   if (u2ModeIdx == VSS_MTK)
   {
#ifndef DSP_SUPPORT_SRSWOW
#ifndef DSP_SUPPORT_SRSTSXT
#ifdef CC_AUD_OLD_MTK_VSURR
			//MTK Vsurr Surround Filter Coefficient
			pdwTable = (UINT32 *)&MTK_VSURR_TABLE[0];
			for (iLoopVar = 0; iLoopVar < 20; iLoopVar++)
			{
				WriteDspCommDram ((ADDR_VSURR_IIR_COEF + iLoopVar),
								  pdwTable[iLoopVar]);
			}
	
			//MTK Vsurr Crosstalk Filter Coefficient
			pdwTable = (UINT32 *)&MTK_VSURR_CROS_TABLE[0];
			for (iLoopVar = 0; iLoopVar < 10; iLoopVar++)
			{
				WriteDspCommDram ((ADDR_VSURR_CROS_COEF + iLoopVar),
								  pdwTable[iLoopVar]);
			}
#else //CC_AUD_OLD_MTK_VSURR

			pdwTable = (UINT32 *)&MTK_VSURR_TABLE[0];
			for (iLoopVar = 0; iLoopVar < 20; iLoopVar++)
			{
				WriteDspCommDram ((ADDR_VSURR_IIR_COEF + iLoopVar),
								  pdwTable[iLoopVar]);
			}
	
			//MTK Vsurr Crosstalk Filter Coefficient
			pdwTable = (UINT32 *)&MTK_VSURR_CROS_TABLE[0];
			for (iLoopVar = 0; iLoopVar < 15; iLoopVar++)
			{
				WriteDspCommDram ((ADDR_VSURR_CROS_COEF + iLoopVar),
								  pdwTable[iLoopVar]);
			}
	
	
			pdwTable = (UINT32 *)&MTK_VSURR_BASS_TABLE[0];
			for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
			{
				WriteDspCommDram ((ADDR_VSURR_BASS_COEF + iLoopVar),
								  pdwTable[iLoopVar]);
			} 
	
			pdwTable = (UINT32 *)&MTK_VSURR_1STOP_TABLE[0];
			for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
			{
				WriteDspCommDram ((ADDR_VSURR_1STOP_COEF + iLoopVar),
								  pdwTable[iLoopVar]);
			} 
			 pdwTable = (UINT32 *)&MTK_VSURR_2STOP_TABLE[0];
			for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
			{
				WriteDspCommDram ((ADDR_VSURR_2STOP_COEF + iLoopVar),
								  pdwTable[iLoopVar]);
			} 

#endif //CC_AUD_OLD_MTK_VSURR
#endif //DSP_SUPPORT_SRSTSXT
#endif //DSP_SUPPORT_SRSWOW
	}
	if (u2ModeIdx == VSS_M2S)
	{
	//mono 2 stereo
		pdwTable = (UINT32 *)&MTK_COMB_R_TABLE[u4FreqIdx][0];
		for (iLoopVar = 0; iLoopVar < 70; iLoopVar++)
		{
			WriteDspCommDram ((ADDR_M2S_COMB_R_COEF + iLoopVar),
							  pdwTable[iLoopVar]);
		}

 
		pdwTable = (UINT32 *)&MTK_COMB_L_TABLE[u4FreqIdx][0];
		for (iLoopVar = 0; iLoopVar < 70; iLoopVar++)
		{
			WriteDspCommDram ((ADDR_M2S_COMB_L_COEF + iLoopVar),
							  pdwTable[iLoopVar]);
		}

		pdwTable = (UINT32 *)&MTK_M2S_ALLPASS_TABLE[u4FreqIdx][0];
		for (iLoopVar = 0; iLoopVar < 20; iLoopVar++)
		{
			WriteDspCommDram ((ADDR_M2S_ALLPASS_COEF + iLoopVar),
							  pdwTable[iLoopVar]);
		}
					  
	}
	else
	{
        WriteDspCommDram (ADDR_VSURR_CROS_PARA1,u4ReadDspShmDWRD(D_VSURR_CROS_TALK)); //0x8400
        WriteDspCommDram (ADDR_VSURR_CROS_PARA2,u4ReadDspShmDWRD(D_VSURR_WIDTH));  //0x8401
        WriteDspCommDram (ADDR_VSURR_CROS_PARA3,u4ReadDspShmDWRD(D_VSURR_LR_GAIN));  //0x8402
        WriteDspCommDram (ADDR_VSURR_GAIN_MPY,u4ReadDspShmDWRD(D_VSURR_CLARITY));  //0x8406
        WriteDspCommDram ((ADDR_VSURR_BASS_COEF + 3),u4ReadDspShmDWRD(D_VSURR_FO)); //0x843c
        WriteDspCommDram (ADDR_VSURR_CROS_PARA4,u4ReadDspShmDWRD(D_VSURR_BASS_GAIN));  //0x843e
        WriteDspCommDram (ADDR_VSURR_CROS_PARA5,u4ReadDspShmDWRD(D_VSURR_OUTPUT_GAIN)); //0x843f  

	}
	
}

#endif

/***************************************************************************
     Function : vDspSetSamplingRate
  Description : setup sampling rate related configuration
                Init Src speaker config 
    Parameter :
    Return    :
***************************************************************************/
void vDspSetSamplingRate (void)
{
    //UINT32 u4FreqIdx;
    //BOOL fgValidFreq;
    UINT16 u2SpkCfg;
    //UINT16 wIecFlag;
    UINT32 u4DataRate = 0;
    BOOL fgFreqChange = FALSE;
    
    //fgValidFreq = TRUE;

    if (_fgDspAoutMuted)
    {
        vDspCmd(UOP_DSP_MASTER_VOLUME);
        _fgDspAoutMuted = FALSE;        
    }

    // For mp3 get mpeg type 
    // if it is mp3 -> it will go here 
    //  we stop dsp here
    // else if it is mp2
    //  we stop dsp at dsp interrupt
    if (_bDspStrTyp == MPEG3_STREAM)
    {
        if (_fgDspGetMpgTyp)
        {
            _fgDspMpgTypGot = TRUE;
            _fgDspGetMpgTyp = FALSE;
            vDspCmd (DSP_STOP);
            return;
        }
    }

    // 20040108 sammy add for play_speed control
    //_fgIECRAWOff = fgGetIecSetting (&wIecFlag, TRUE);
    //vWriteDspWORD (ADDR_RC2D_IEC_FLAG, wIecFlag);

    //Extract SFreq
    // Internal buffer is enabled, and dec2 write internal buffer
    // => dec1 sampling rate is the same as dec2
    /*
    if ((u2ReadDspShmWORD (W_SYSTEM_CTRL) & 0x3) == 0x3)
    {
        u4FreqIdx = dwDspSetTableAddr (FALSE);
    }
    else
    {
        u4FreqIdx = dwDspSetTableAddr (TRUE);
    }
*/
    // Move the check to AUD_SetSampleFreq() function, since in HDMI mode,
    // driver may modify sampling frequency, so checking _dwDspFreq only is not okay
    //if (u4FreqIdx != _dwDspFreq)
    #if 0 //SHINK_8223_CODE ...ASRC
    if (1)
    {
        _bCode3D = CODE3D_NONE;
        _bCode3DUpmix = CODE3D_NONE;
        _dwDspFreq = u4FreqIdx;
        fgValidFreq = TRUE;
        switch (u4FreqIdx)
        {
        case SFREQ_16K:        // 16K
            _dwDacFreq = (UINT32)FS_16K;
            _dwTableFreq = SAMPLE_32K;
            break;
        case SFREQ_22K:        // 22K
            _dwDacFreq = (UINT32)FS_22K;
            _dwTableFreq = SAMPLE_32K;
            break;
        case SFREQ_24K:        // 24K
            _dwDacFreq = (UINT32)FS_24K;
            _dwTableFreq = SAMPLE_32K;
            break;
        case SFREQ_32K:        // 32K
            _dwDacFreq = (UINT32)FS_32K;
            _dwTableFreq = SAMPLE_32K;
            break;
        case SFREQ_44K:        // 44K
            _dwDacFreq = (UINT32)FS_44K;
            _dwTableFreq = SAMPLE_44K;
            break;
        case SFREQ_48K:        // 48K
            _dwDacFreq = (UINT32)FS_48K;
            _dwTableFreq = SAMPLE_48K;
            break;
        case SFREQ_88K:        // 88K
            _dwDacFreq = (UINT32)FS_88K;
            _dwTableFreq = SAMPLE_96K;
            break;

        case SFREQ_96K:        // 96K
            _dwDacFreq = (UINT32)FS_96K;
            _dwTableFreq = SAMPLE_96K;
            break;
        case SFREQ_176K:       // 176K
            _dwDacFreq = (UINT32)FS_176K;
            _dwTableFreq = SAMPLE_192K;
            break;

        case SFREQ_192K:       // 192K
            _dwDacFreq = (UINT32)FS_192K;
            _dwTableFreq = SAMPLE_192K;
            break;
        default:
            fgValidFreq = FALSE;
            break;
        }
        fgFreqChange = TRUE;
    }
    //#endif
    if (fgValidFreq)            // && _bDspStrTyp != SACD_STREAM)
    {
        vDspSetFreq (_dwDacFreq, fgFreqChange, TRUE);
    }
    #else
        _dwDacFreq  =  FS_48K;
        _dwTableFreq = SAMPLE_48K;
        vDspSetFreq (_dwDacFreq, fgFreqChange, TRUE); //fix at 48k
    #endif

    vDspSetDeEmphDec (_bDspStrTyp);

    HalFlushInvalidateDCache(); //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro
    u2SpkCfg = wReadDspWORD (ADDR_D2RC_RISC_INFO_INPUT_CH_CFG);
    vTrnsltSpkCfg (u2SpkCfg, &_tSrcSpkCfg);

    // Set DMX interrupt threshold according to bitrate
    if (_bDspStrTyp == MPEG12_STREAM)
    {
        u4DataRate = dReadDspCommDram(ADDR_D2RC_MPEG_BITRATE);
        if (u4DataRate >= 256000)
        {
            AUD_DrvDmxIntThreshold(AUD_DEC_MAIN, 1);          // Set to 188*1
        }
    }    

    vDspSetTable (_dwTableFreq);    // move table to DSP working area
    // Load ramcode for PostProcessing
    // Let Dsp continue to play

    if (AUD_IsMMAVInitSync(AUD_DEC_MAIN) && !AUD_IsMMAoutEnable(AUD_DEC_MAIN))
    {
        //For MM, need to set av_sync_hold to DSP
        vDspSendSample(TRUE);
    }
    else // Normal case
    {
        vDspSendSample(FALSE);
    } 
}
void vDspUpdatePostEffect(void)
{
#ifdef DSP_POST_PROCESS_ENABLE
    vSetPostProcTable (_dwTableFreq, TRUE);
    vSetupUpMix       (_dwTableFreq, TRUE);
    vSetupDownMix     (_dwTableFreq, TRUE);
#else
    vWriteDspWORD (ADDR_RC2D_VIRTUAL_SURROUND,      0);
    vWriteDspWORD (ADDR_RC2D_PRO_LOGIC_II_CONFIG,   0);
  //  vWriteDspWORD (ADDR_RC2D_NEO6_FLAG,             0);
#endif

#ifdef CC_AUD_BBE_SUPPORT
    vSetBbeTable(_dwTableFreq, TRUE);
#endif   

    vSetBassManageTable (_dwTableFreq, TRUE);
	DspIntOrSram(SRAM_EFFECT_CONTROL,0x01000);	
}
/***************************************************************************
     Function : vDspSetSamplingRateDec2
  Description : setup sampling rate related configuration
                Init Src speaker config
    Parameter :
    Return    :
***************************************************************************/
void vDspSetSamplingRateDec2 (void)
{
    UINT32 u4FreqIdx;
    BOOL fgValidFreq;
    UINT16 u2SpkCfg;
    //UINT16 wIecFlag;
    UINT32 u4DataRate = 0;
    BOOL fgFreqChange = FALSE;

    fgValidFreq = TRUE;

    if (_fgDspAoutMutedDec2)
    {
        vDspCmd(UOP_DSP_MASTER_VOLUME_DEC2);
        _fgDspAoutMutedDec2 = FALSE;
    }

    // 20040108 sammy add for play_speed control
    //_fgIECRAWOffDec2 = fgGetIecSetting (&wIecFlag, FALSE);
    //vWriteDspWORD (ADDR_RC2D_IEC_FLAG_DEC2, wIecFlag);

    //Extract SFreq
    // Internal buffer is enabled, and dec1 write internal buffer
    // => dec2 sampling rate is the same as dec1
    if ((u2ReadDspShmWORD (W_SYSTEM_CTRL) & 0x3) == 0x1)
    {
        u4FreqIdx = dwDspSetTableAddr (TRUE);
    }
    else
    {
        u4FreqIdx = dwDspSetTableAddr (FALSE);
    }

    if (u4FreqIdx != _dwDspFreqDec2)
    //#if (1)
    {
        _bCode3DDec2 = CODE3D_NONE;
        _bCode3DUpmixDec2 = CODE3D_NONE;
        _dwDspFreqDec2 = u4FreqIdx;
        fgValidFreq = TRUE;
        switch (u4FreqIdx)
        {
        case SFREQ_16K:        // 16K
            _dwDacFreqDec2 = (UINT32)FS_16K;
            _dwTableFreqDec2 = SAMPLE_32K;
            break;
        case SFREQ_22K:        // 22K
            _dwDacFreqDec2 = (UINT32)FS_22K;
            _dwTableFreqDec2 = SAMPLE_32K;
            break;
        case SFREQ_24K:        // 24K
            _dwDacFreqDec2 = (UINT32)FS_24K;
            _dwTableFreqDec2 = SAMPLE_32K;
            break;
        case SFREQ_32K:        // 32K
            _dwDacFreqDec2 = (UINT32)FS_32K;
            _dwTableFreqDec2 = SAMPLE_32K;
            break;
        case SFREQ_44K:        // 44K
            _dwDacFreqDec2 = (UINT32)FS_44K;
            _dwTableFreqDec2 = SAMPLE_44K;
            break;
        case SFREQ_48K:        // 48K
            _dwDacFreqDec2 = (UINT32)FS_48K;
            _dwTableFreqDec2 = SAMPLE_48K;
            break;
        case SFREQ_88K:        // 88K
            _dwDacFreqDec2 = (UINT32)FS_88K;
            _dwTableFreqDec2 = SAMPLE_96K;
            break;

        case SFREQ_96K:        // 96K
            _dwDacFreqDec2 = (UINT32)FS_96K;
            _dwTableFreqDec2 = SAMPLE_96K;
            break;
        case SFREQ_176K:       // 176K
            _dwDacFreqDec2 = (UINT32)FS_176K;
            _dwTableFreqDec2 = SAMPLE_192K;
            break;
        case SFREQ_192K:       // 192K
            _dwDacFreqDec2 = (UINT32)FS_192K;
            _dwTableFreqDec2 = SAMPLE_192K;
            break;
        default:
            fgValidFreq = FALSE;
            break;
        }
        fgFreqChange = TRUE;
    }
    //#endif
    if (fgValidFreq)            // && _bDspStrTyp != SACD_STREAM)
    {
        vDspSetFreq (_dwDacFreqDec2, fgFreqChange, FALSE);
    }

    vDspSetDeEmphDec2 (_bDspStrTypDec2);

    HalFlushInvalidateDCache(); //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro
    u2SpkCfg = wReadDspWORD (ADDR_D2RC_RISC_INFO_INPUT_CH_CFG_DEC2);
    vTrnsltSpkCfg (u2SpkCfg, &_tSrcSpkCfgDec2);

    // Set DMX interrupt threshold according to bitrate
    if (_bDspStrTypDec2 == MPEG12_STREAM)
    {
        u4DataRate = dReadDspCommDram(ADDR_D2RC_MPEG_BITRATE_DEC2);
        if (u4DataRate >= 256000)
        {
            AUD_DrvDmxIntThreshold(AUD_DEC_AUX, 1);          // Set to 188*1 for specific bit-steream
        }
    }

    vDspSetTableDec2 (_dwTableFreqDec2);    // move table to DSP working area
    // Load ramcode for PostProcessing
    // Let Dsp continue to play
    
    if(AUD_IsMMAVInitSync(AUD_DEC_AUX) && !AUD_IsMMAoutEnable(AUD_DEC_AUX))
    {
        // For MM, need to set av_sync_hold to DSP
        vDspSendSampleDec2(TRUE);
    }
    else // Normal case
    {
        vDspSendSampleDec2(FALSE);
    } 
/*
#ifdef CC_MT5391_AUD_3_DECODER  	
   //check if need to enable DDCO or not
   if ((_GetIECFlag(AUD_DEC_AUX) == AUD_IEC_CFG_RAW)  && 
	    (_bDspStrTypDec2== AAC_STREAM) &&
	    (_AudGetStrSource(AUD_DEC_AUX) == AUD_STREAM_FROM_DIGITAL_TUNER) &&
	    (_dwDspFreqDec2==SFREQ_48K))
     {
              _AUD_DspSetDDCOEnable(TRUE);	
 			  
      }
     //////////////////////////////////////////////////////////////////
  #endif 
*/
	
}
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
/***************************************************************************
     Function : vDspSetSamplingRateDec3
  Description : setup sampling rate related configuration
                Init Src speaker config
    Parameter :
    Return    :
***************************************************************************/
void vDspSetSamplingRateDec3 (void)
{
    UINT32 u4FreqIdx;
    BOOL fgValidFreq;
    UINT16 u2SpkCfg;
    //UINT16 wIecFlag;
    UINT32 u4DataRate = 0;
    BOOL fgFreqChange = FALSE;

    // FIXME! Workaround for warning free
    UNUSED(fgValidFreq);
    UNUSED(u2SpkCfg);
    UNUSED(fgFreqChange);
    

    fgValidFreq = TRUE;

    if (_fgDspAoutMutedDec3)
    {
        vDspCmd(UOP_DSP_MASTER_VOLUME_DEC3);
        _fgDspAoutMutedDec3 = FALSE;
    }

    // 20040108 sammy add for play_speed control
    //_fgIECRAWOffDec3 = fgGetIecSetting (&wIecFlag, FALSE);
    //vWriteDspWORD (ADDR_RC2D_IEC_FLAG_DEC3, wIecFlag);

    //Extract SFreq
    // Internal buffer is enabled, and dec1 write internal buffer
    // => dec2 sampling rate is the same as dec1
    if ((u2ReadDspShmWORD (W_SYSTEM_CTRL) & 0x3) == 0x1)
    {
        u4FreqIdx = dwDspSetTableAddr (TRUE);
    }
    else
    {
        u4FreqIdx = dwDspSetTableAddr (FALSE);
    }

    if (u4FreqIdx != _dwDspFreqDec3)
    //#if (1)
    {
        _bCode3DDec3 = CODE3D_NONE;
        _bCode3DUpmixDec3 = CODE3D_NONE;
        _dwDspFreqDec3 = u4FreqIdx;
        fgValidFreq = TRUE;
        switch (u4FreqIdx)
        {
        case SFREQ_16K:        // 16K
            _dwDacFreqDec3 = (UINT32)FS_16K;
            _dwTableFreqDec3 = SAMPLE_32K;
            break;
        case SFREQ_22K:        // 22K
            _dwDacFreqDec3 = (UINT32)FS_22K;
            _dwTableFreqDec3 = SAMPLE_32K;
            break;
        case SFREQ_24K:        // 24K
            _dwDacFreqDec3 = (UINT32)FS_24K;
            _dwTableFreqDec3 = SAMPLE_32K;
            break;
        case SFREQ_32K:        // 32K
            _dwDacFreqDec3 = (UINT32)FS_32K;
            _dwTableFreqDec3 = SAMPLE_32K;
            break;
        case SFREQ_44K:        // 44K
            _dwDacFreqDec3 = (UINT32)FS_44K;
            _dwTableFreqDec3 = SAMPLE_44K;
            break;
        case SFREQ_48K:        // 48K
            _dwDacFreqDec3 = (UINT32)FS_48K;
            _dwTableFreqDec3 = SAMPLE_48K;
            break;
        case SFREQ_88K:        // 88K
            _dwDacFreqDec3 = (UINT32)FS_88K;
            _dwTableFreqDec3 = SAMPLE_96K;
            break;

        case SFREQ_96K:        // 96K
            _dwDacFreqDec3 = (UINT32)FS_96K;
            _dwTableFreqDec3 = SAMPLE_96K;
            break;
        case SFREQ_176K:       // 176K
            _dwDacFreqDec3 = (UINT32)FS_176K;
            _dwTableFreqDec3 = SAMPLE_192K;
            break;
        case SFREQ_192K:       // 192K
            _dwDacFreqDec3 = (UINT32)FS_192K;
            _dwTableFreqDec3 = SAMPLE_192K;
            break;
        default:
            fgValidFreq = FALSE;
            break;
        }
        fgFreqChange = TRUE;
    }
    //#endif
    //if (fgValidFreq)            // && _bDspStrTyp != SACD_STREAM)
    //{
    //    vDspSetFreq (_dwDacFreqDec3, fgFreqChange, FALSE);
   // }

    HalFlushInvalidateDCache(); //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro
    //u2SpkCfg = wReadDspWORD (ADDR_D2RC_RISC_INFO_INPUT_CH_CFG_DEC2);
    //vTrnsltSpkCfg (u2SpkCfg, &_tSrcSpkCfgDec2);

    // Set DMX interrupt threshold according to bitrate
    if (_bDspStrTypDec3 == MPEG12_STREAM)
    {
        u4DataRate = dReadDspCommDram(ADDR_D2RC_MPEG_BITRATE_DEC3);
        if (u4DataRate >= 256000)
        {
            AUD_DrvDmxIntThreshold(AUD_DEC_THIRD, 1);          // Set to 188*1
        }
    }    

    //vDspSetTableDec2 (_dwTableFreqDec2);    // move table to DSP working area
    // Load ramcode for PostProcessing
    // Let Dsp continue to play
    vDspSendSampleDec3 ();
}
#endif

/***************************************************************************
     Function :
  Description :
    Parameter :
    Return    :
***************************************************************************/
void vDspSendSampleRate (BOOL fgFirstDecoder)
{
    UINT32 dwTryCnt;
    volatile BOOL *pfgSendSample = &_fgSendSampleDec2;
    UINT32 dwFlowCtrlHandle = INT_RC2D_FLOW_CONTROL;

    if (fgFirstDecoder)
    {
        pfgSendSample = &_fgSendSampleDec2;
        dwFlowCtrlHandle = INT_RC2D_FLOW_CONTROL_DEC2;
    }
    dwTryCnt = 0;


    while (*pfgSendSample)
    {
        dwTryCnt++;

        if (!fgDspBusy ())
        {
            SendDSPInt (dwFlowCtrlHandle, (FLOW_CONTROL_SAMPLING_RATE+FLOW_CONTROL_AOUT_WAIT_ENABLE));
            *pfgSendSample = FALSE;
        }

        if (dwTryCnt == DSP_TIMEOUT)
        {
            SendDSPInt (dwFlowCtrlHandle, (FLOW_CONTROL_SAMPLING_RATE+FLOW_CONTROL_AOUT_WAIT_ENABLE));
            *pfgSendSample = FALSE;
        }

    }
}

/***************************************************************************
     Function :
  Description :
    Parameter :
    Return    :
***************************************************************************/
BOOL fgIntFlowControl (UINT32 u4DspRIntData)
{
    UNUSED(u4DspRIntData);
    // LOG(9, "Rcv DSP flow control interrupt %d\n",u4DspRIntData);
    // Service Flow Control Interrupt
    #if 0 // refine ADSP control
    switch (u4DspRIntData)
    {
    case D2RC_FLOW_CONTROL_SAMPLING_RATE:
    	LOG(9, "DSP interrupt RISC - Set sampling rate\n");
        vDspSetSamplingRate ();
        break;
    case D2RC_FLOW_CONTROL_AOUT_ENABLE:
    	LOG(9, "DSP interrupt RISC - Aout enable\n");
        vDspAOutEnable ();
        break;
    case D2RC_FLOW_CONTROL_FLUSH_DONE:
    	LOG(9, "DSP interrupt RISC - Flush done\n");
        vDspFlushDone ();
        break;
    case D2RC_FLOW_CONTROL_PAUSE_OK:
        // none handling
        break;
        /* secondary decoder */
    case D2RC_FLOW_CONTROL_SAMPLING_RATE_DEC2:
    	LOG(9, "DSP interrupt RISC - Set sampling rate decoder2\n");
        vDspSetSamplingRateDec2 ();
        break;
    case D2RC_FLOW_CONTROL_AOUT_ENABLE_DEC2:
    	LOG(9, "DSP interrupt RISC - Aout enable decoder2\n");
        vDspAOutEnableDec2 ();
        break;
    case D2RC_FLOW_CONTROL_FLUSH_DONE_DEC2:
    	LOG(9, "DSP interrupt RISC - Flush done decoder2\n");
        vDspFlushDoneDec2 ();
        break;
    default:
        break;
    }

    vDspFlowControlNotify(u4DspRIntData);
    #endif
    return TRUE;
}

/***************************************************************************
     Function :
  Description : Set table ...
    Parameter :
    Return    :
***************************************************************************/
void vDspSetTable (UINT32 u4FreqIdx)
{
    BOOL fgProcMode = FALSE;

#ifdef DSP_POST_PROCESS_ENABLE
    vSetPostProcTable (u4FreqIdx, TRUE);
    vSetupUpMix       (u4FreqIdx, TRUE);
    vSetupDownMix     (u4FreqIdx, TRUE);
#else
    vWriteDspWORD (ADDR_RC2D_VIRTUAL_SURROUND,      0);
    vWriteDspWORD (ADDR_RC2D_PRO_LOGIC_II_CONFIG,   0);
   // vWriteDspWORD (ADDR_RC2D_NEO6_FLAG,             0);
#endif

#ifdef CC_AUD_BBE_SUPPORT
    vSetBbeTable(u4FreqIdx, TRUE);
#endif

    vSetBassManageTable (u4FreqIdx, TRUE);

    //Check if DRC Auto and Set DRC Mode
    if (u2ReadDspShmWORD (W_PROCMOD) & 0x8)
    {
        fgProcMode = TRUE;
    }
    if (fgProcMode)
    {
        vDspCmd (UOP_DSP_PROCESSING_MODE);
    }
 

}

/***************************************************************************
     Function :
  Description : Set table ...
    Parameter :
    Return    :
***************************************************************************/
void vDspSetTableDec2 (UINT32 u4FreqIdx)
{
    BOOL fgProcMode = FALSE;

#ifdef DSP_POST_PROCESS_ENABLE
    vSetPostProcTable (u4FreqIdx, FALSE);
    vSetupDownMix     (u4FreqIdx, FALSE);
#endif

    vSetBassManageTable (u4FreqIdx, FALSE);

    if (u2ReadDspShmWORD (W_PROCMOD_DEC2) & 0x8)
    {
        fgProcMode = TRUE;
    }
    if (fgProcMode)
    {
        vDspCmd (UOP_DSP_PROCESSING_MODE_DEC2);
    }

}

void vLoadHPFTable(void)
{
    const UINT32 * pu4TableAddr;
    UINT32   u4FcIndx;
    INT32    i;
    //UINT32   u4FreqTableIdx = 2;

    // Check frequency index range
    //u4FreqTableIdx = (u4FreqIdx > 2) ? 2 : u4FreqIdx;
        
    // Find out table address
    u4FcIndx = FC_TO_TABLE_INDEX(u2ReadDspShmWORD(W_HPF_FC));
    // LOG(0, "Fc Idx = %d\n", u4FcIndx);
    pu4TableAddr = &ORDER_6_FILTER_TABLE[u4FcIndx][0];


#if 1
    // Perform loading coefficient table
    for (i = 0; i < HPF_TABLE_SET_SZ; i++)
    {
        WriteDspCommDram ((DRAM_CMPT_HPF_BUFFER + i), pu4TableAddr[i]);
        WriteDspCommDram ((DRAM_CMPT_HPF_BUFFER + i + 5), pu4TableAddr[i]);
        WriteDspCommDram ((DRAM_CMPT_HPF_BUFFER + i + 10), pu4TableAddr[i]);
    }

    // Re-initialize DSP working buffer
    for (i = HPF_TABLE_SET_SZ*3; i < 39; i++)
    {
        WriteDspCommDram (DRAM_CMPT_HPF_BUFFER + i, 0x00000000);	
    }

#else

    /*b2*/ WriteDspCommDram (DRAM_CMPT_HPF_BUFFER + 0, 0x003f6978);
	/*b1*/ WriteDspCommDram (DRAM_CMPT_HPF_BUFFER + 1, 0xff812d12);
	/*a2*/ WriteDspCommDram (DRAM_CMPT_HPF_BUFFER + 2, 0xffc12bac);
	/*a1*/ WriteDspCommDram (DRAM_CMPT_HPF_BUFFER + 3, 0x007ed18a);
	/*b0*/ WriteDspCommDram (DRAM_CMPT_HPF_BUFFER + 4, 0x003f6978);	

	/*b2*/ WriteDspCommDram (DRAM_CMPT_HPF_BUFFER + 5, 0x003f6978);
	/*b1*/ WriteDspCommDram (DRAM_CMPT_HPF_BUFFER + 6, 0xff812d12);
	/*a2*/ WriteDspCommDram (DRAM_CMPT_HPF_BUFFER + 7, 0xffc12bac);
	/*a1*/ WriteDspCommDram (DRAM_CMPT_HPF_BUFFER + 8, 0x007ed18a);
	/*b0*/ WriteDspCommDram (DRAM_CMPT_HPF_BUFFER + 9, 0x003f6978);

	/*b2*/ WriteDspCommDram (DRAM_CMPT_HPF_BUFFER + 10, 0x003f6978);
	/*b1*/ WriteDspCommDram (DRAM_CMPT_HPF_BUFFER + 11, 0xff812d12);
	/*a2*/ WriteDspCommDram (DRAM_CMPT_HPF_BUFFER + 12, 0xffc12bac);
	/*a1*/ WriteDspCommDram (DRAM_CMPT_HPF_BUFFER + 13, 0x007ed18a);
	/*b0*/ WriteDspCommDram (DRAM_CMPT_HPF_BUFFER + 14, 0x003f6978);

    // Re-initialize DSP working buffer
    for (i = HPF_TABLE_SET_SZ; i < 39; i++)
    {
        WriteDspCommDram (DRAM_CMPT_HPF_BUFFER + i, 0x00000000);	
    }
   
#endif
	
}
    
void vSetPostProcTable (UINT32 u4FreqIdx, BOOL fgFirstDecoder)
{
#ifdef DSP_POST_PROCESS_ENABLE
    UINT32 iLoopVar;
    UINT32 *pdwTable;
    UINT32 dwBank;

    if (fgFirstDecoder) // First decoder
    {
        //================ Delay Factor ================ 
        WriteDspCommDram (ADDR_RC2D_CH_DELAY_FACTOR, DELAY_FACTOR[u4FreqIdx]);
        //Channel Time Delay
        for (iLoopVar = 0; iLoopVar < 10; iLoopVar++)
        {
            WriteDspCommDram ((ADDR_RC2D_CH_DELAY_C + iLoopVar),
                u2ReadDspShmWORD ((W_CHDELAY_C + (iLoopVar * 2))));
        }
        //================ Equalizer IIR Coefficient ================ 
        // 20050929 Jack move the "Set all Channel gain" from UOP to here to prevent the pop noise
        // Dry Gain
        WriteDspCommDram (ADDR_RC2D_EQUALIZER_DRY_LEVEL,
            (u4ReadDspShmDWRD (D_EQ_DRY)));
        vWriteDspWORD(ADDR_RC2D_EQUALIZER_BAND_NUM_1, (AUD_EQ_IIR_BAND_NUM-1));
        vEQBandGainFlush(AUD_DEC_MAIN);
        vWriteDspWORD (ADDR_RC2D_EQUALIZER_BAND1_SHIFT_BIT,
            ((UINT32)(EQ_SHIFT_BIT[0])));
        vWriteDspWORD (ADDR_RC2D_EQUALIZER_BAND2_SHIFT_BIT,
            ((UINT32)(EQ_SHIFT_BIT[1])));
        
        // Enable update flag
        vWriteDspWORD(ADDR_RC2D_EQUALIZER_TABLE_CHANGE, 0x1);

        //================ Super Bass Pipe ================ 
        //pdwTable =
        //    (UINT32 *) (SBASSPIPE_TABLE) + (u4FreqIdx * DSP_SBASSPIPE_SZ);
#ifdef DSP_EQ2_SBASS_SUPPORT
        vEQ2SbassBandGainFlush(AUD_DEC_MAIN);
        // Enable update flag
        vWriteDspWORD(ADDR_RC2D_SUPER_BASS_TABLE_CHANGE, 0x1);
#else
        pdwTable = (UINT32 *)&SBASSPIPE_TABLE[u4FreqIdx][0];
        for (iLoopVar = 0; iLoopVar < DSP_SBASSPIPE_SZ; iLoopVar++)
        {
            WriteDspCommDram ((ADDR_RC2D_SUPER_BASS_PIPE_FILTER_COEFF +
                              iLoopVar), pdwTable[iLoopVar]);
        }
        //Super Bass Boost
        //pdwTable =
        //    (UINT32 *) (SBASSBOOST_TABLE) + (u4FreqIdx * DSP_SBASSBOOST_SZ);
        pdwTable = (UINT32 *)&SBASSBOOST_TABLE[u4FreqIdx][0];
        for (iLoopVar = 0; iLoopVar < DSP_SBASSBOOST_SZ; iLoopVar++)
        {
            WriteDspCommDram ((ADDR_RC2D_SUPER_BASS_BOOST_FILTER_COEFF +
                              iLoopVar), pdwTable[iLoopVar]);
        }
        //Super Bass Clear
        //pdwTable =
        //    (UINT32 *) (SBASSCLEAR_TABLE) + (u4FreqIdx * DSP_SBASSCLEAR_SZ);
        pdwTable = (UINT32 *)&SBASSCLEAR_TABLE[u4FreqIdx][0];
        for (iLoopVar = 0; iLoopVar < DSP_SBASSCLEAR_SZ; iLoopVar++)
        {
            WriteDspCommDram ((ADDR_RC2D_SUPER_BASS_CLEAR_FILTER_COEFF +
                              iLoopVar), pdwTable[iLoopVar]);
        }
#endif  // DSP_EQ2_SBASS_SUPPORT    

        //================ Reverb IIR Filter Coefficient ================ 
        //pdwTable = (UINT32 *) (REVERB_TABLE) + (u4FreqIdx * DSP_REVERB_SZ);
        pdwTable = (UINT32 *)&REVERB_TABLE[0];
        for (iLoopVar = 0; iLoopVar < DSP_REVERB_SZ; iLoopVar++)
        {
            WriteDspCommDram ((ADDR_RC2D_REVERB_IIR_A + iLoopVar),
                              pdwTable[iLoopVar]);
        }
        //Reverb Gain
        WriteDspCommDram (ADDR_RC2D_REVERB_GAIN,
                          u4ReadDspShmDWRD (D_REVERBGAIN));
        //Reverb Bank Number
        for (iLoopVar = 0; iLoopVar < 4; iLoopVar++)
        {
            dwBank = (UINT32) (uReadDspShmBYTE ((B_REVERBBANK0 + iLoopVar)));
            if (dwBank > MAX_REVERB_BANK)
            {
                vShowError (ERR_ADSP_REVERB_MAX_BANK);
            }
            WriteDspCommDram ((ADDR_RC2D_REVERB_BANK_MAX + iLoopVar), dwBank);
        }
        
        //================  DC filter ================ 
        WriteDspCommDram (ADDR_RC2D_DCF_COEF, DCFilter_COEFF[u4FreqIdx]);

        //================ VBASS ================ 
        pdwTable = (UINT32 *)&MTK_VBASS_R_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 70; iLoopVar++)
        {
            WriteDspCommDram ((ADDR_VBASS_R_COEF + iLoopVar),
                              pdwTable[iLoopVar]);
        }
 
        pdwTable = (UINT32 *)&MTK_VBASS_L_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 70; iLoopVar++)
        {
            WriteDspCommDram ((ADDR_VBASS_L_COEF + iLoopVar),
                              pdwTable[iLoopVar]);
        }

        pdwTable = (UINT32 *)&MTK_VBASS_HIGHPASS_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 20; iLoopVar++)
        {
            WriteDspCommDram ((ADDR_VBASS_HIGHPASS_COEF + iLoopVar),
                              pdwTable[iLoopVar]);
        }
#ifndef CC_NEW_POST_MULTI_VS_SUPPORT
        //================ Vsurr Surround ================
#ifndef DSP_SUPPORT_SRSWOW
#ifndef DSP_SUPPORT_SRSTSXT
#ifndef DSP_SUPPORT_SRSTSHD //sunman_tshd
#ifdef CC_AUD_OLD_MTK_VSURR
        //MTK Vsurr Surround Filter Coefficient
        pdwTable = (UINT32 *)&MTK_VSURR_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 20; iLoopVar++)
        {
            WriteDspCommDram ((ADDR_VSURR_IIR_COEF + iLoopVar),
                              pdwTable[iLoopVar]);
        }

        //MTK Vsurr Crosstalk Filter Coefficient
        pdwTable = (UINT32 *)&MTK_VSURR_CROS_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 10; iLoopVar++)
        {
            WriteDspCommDram ((ADDR_VSURR_CROS_COEF + iLoopVar),
                              pdwTable[iLoopVar]);
        }
#else //CC_AUD_OLD_MTK_VSURR
        pdwTable = (UINT32 *)&MTK_VSURR_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 20; iLoopVar++)
        {
            WriteDspCommDram ((ADDR_VSURR_IIR_COEF + iLoopVar),
                              pdwTable[iLoopVar]);
        }

        //MTK Vsurr Crosstalk Filter Coefficient
        pdwTable = (UINT32 *)&MTK_VSURR_CROS_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 15; iLoopVar++)
        {
            WriteDspCommDram ((ADDR_VSURR_CROS_COEF + iLoopVar),
                              pdwTable[iLoopVar]);
        }


        pdwTable = (UINT32 *)&MTK_VSURR_BASS_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
        {
            WriteDspCommDram ((ADDR_VSURR_BASS_COEF + iLoopVar),
                              pdwTable[iLoopVar]);
        } 

        pdwTable = (UINT32 *)&MTK_VSURR_1STOP_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
        {
            WriteDspCommDram ((ADDR_VSURR_1STOP_COEF + iLoopVar),
                              pdwTable[iLoopVar]);
        } 
         pdwTable = (UINT32 *)&MTK_VSURR_2STOP_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
        {
            WriteDspCommDram ((ADDR_VSURR_2STOP_COEF + iLoopVar),
                              pdwTable[iLoopVar]);
        } 

        #ifdef CC_AUD_USE_NVM
        //AUD_NVM_Operation(AUD_NVM_VS_INIT, NULL, 0);
        // gallen, not to use NVM for TPV_PHILIPS on MT5362B
        #endif //CC_AUD_USE_NVM        
#endif //CC_AUD_OLD_MTK_VSURR
#endif //DSP_SUPPORT_SRSTSHD //sunman_tshd
#endif //DSP_SUPPORT_SRSTSXT
#endif //DSP_SUPPORT_SRSWOW

#else
        vLoadVirtualSurroundTable(u4FreqIdx);
#endif

#ifdef CC_AUD_SPEAKER_HEIGHT
        pdwTable = (UINT32 *)&MTK_SHELF_R_TABLE[u4FreqIdx][0];
        for (iLoopVar = 0; iLoopVar < 15; iLoopVar++)
        {
            WriteDspCommDram ((ADDR_SHELF_R_COEF + iLoopVar),
                              pdwTable[iLoopVar]);
        }

 
        pdwTable = (UINT32 *)&MTK_SHELF_L_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 15; iLoopVar++)
        {
            WriteDspCommDram ((ADDR_SHELF_L_COEF + iLoopVar),
                              pdwTable[iLoopVar]);
        }

        pdwTable = (UINT32 *)&MTK_ALLPASS1_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 10; iLoopVar++)
        {
            WriteDspCommDram ((ADDR_ALLPASS1_COEF + iLoopVar),
                              pdwTable[iLoopVar]);
        }

#ifdef CC_AUD_SPEAKER_HEIGHT_MODE
	  vLoadSPHTable();
#endif
#endif
        // ================ Load HPF table ================
        vLoadHPFTable();

        // ================ Load PEQ table ================ 
        vLoadPEQTable();

        pdwTable = (UINT32 *)&MTK_SPKCAL_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 40; iLoopVar++)
        {
            WriteDspCommDram ((ADDR_PEQ_COEF + iLoopVar),
                              pdwTable[iLoopVar]);
        }
        
    }
    else // Second decoder
    {                           
        //================ Delay Factor ================ 
        WriteDspCommDram (ADDR_RC2D_CH_DELAY_FACTOR_DEC2,
                          DELAY_FACTOR[u4FreqIdx]);
        //Channel Time Delay
        for (iLoopVar = 0; iLoopVar < 3; iLoopVar++)
        {
            WriteDspCommDram ((ADDR_RC2D_CH_DELAY_C_DEC2 + iLoopVar),
                              u2ReadDspShmWORD ((W_CHDELAY_C + (iLoopVar * 2))));
        }
        
        //================ Equalizer IIR Coefficient ================ 
        // 20050929 Jack move the "Set all Channel gain" from UOP to here to prevent the pop noise
        WriteDspCommDram (ADDR_RC2D_EQUALIZER_DRY_LEVEL_DEC2,
            (u4ReadDspShmDWRD (D_EQ_DRY_DEC2)));
        vWriteDspWORD(ADDR_RC2D_EQUALIZER_BAND_NUM_1_DEC2, (AUD_EQ_IIR_BAND_NUM-1));
        vEQBandGainFlush(AUD_DEC_AUX);
        vWriteDspWORD (ADDR_RC2D_EQUALIZER_BAND1_SHIFT_BIT_DEC2,
            ((UINT32)(EQ_SHIFT_BIT[0])));
        vWriteDspWORD (ADDR_RC2D_EQUALIZER_BAND2_SHIFT_BIT_DEC2,
            ((UINT32)(EQ_SHIFT_BIT[1])));        
        // Enable update flag
        vWriteDspWORD(ADDR_RC2D_EQUALIZER_TABLE_CHANGE_DEC2, 0x1);

#ifdef DSP_EQ2_SBASS_SUPPORT
        vEQ2SbassBandGainFlush(AUD_DEC_AUX);
        // Enable update flag
        vWriteDspWORD(ADDR_RC2D_SUPER_BASS_TABLE_CHANGE_DEC2, 0x1);
#else
        //================ Super Bass Pipe ================ 
        //pdwTable =
        //    (UINT32 *) (SBASSPIPE_TABLE) + (u4FreqIdx * DSP_SBASSPIPE_SZ);
        pdwTable = (UINT32 *)&SBASSPIPE_TABLE[0];
        for (iLoopVar = 0; iLoopVar < DSP_SBASSPIPE_SZ; iLoopVar++)
        {
            WriteDspCommDram ((ADDR_RC2D_SUPER_BASS_PIPE_FILTER_COEFF_DEC2 +
                              iLoopVar), pdwTable[iLoopVar]);
        }
        //Super Bass Boost
        //pdwTable =
        //    (UINT32 *) (SBASSBOOST_TABLE) + (u4FreqIdx * DSP_SBASSBOOST_SZ);
        pdwTable = (UINT32 *)&SBASSBOOST_TABLE[0];
        for (iLoopVar = 0; iLoopVar < DSP_SBASSBOOST_SZ; iLoopVar++)
        {
            WriteDspCommDram ((ADDR_RC2D_SUPER_BASS_BOOST_FILTER_COEFF_DEC2 +
                              iLoopVar), pdwTable[iLoopVar]);
        }
        //Super Bass Clear
        //pdwTable =
        //    (UINT32 *) (SBASSCLEAR_TABLE) + (u4FreqIdx * DSP_SBASSCLEAR_SZ);
        pdwTable = (UINT32 *)&SBASSCLEAR_TABLE[0];
        for (iLoopVar = 0; iLoopVar < DSP_SBASSCLEAR_SZ; iLoopVar++)
        {
            WriteDspCommDram ((ADDR_RC2D_SUPER_BASS_CLEAR_FILTER_COEFF_DEC2 +
                              iLoopVar), pdwTable[iLoopVar]);
        }
#endif // DSP_EQ2_SBASS_SUPPORT       

        //================ Reverb IIR Filter Coefficient ================ 
        //pdwTable = (UINT32 *) (REVERB_TABLE) + (u4FreqIdx * DSP_REVERB_SZ);
        pdwTable = (UINT32 *)&REVERB_TABLE[0];
        for (iLoopVar = 0; iLoopVar < DSP_REVERB_SZ; iLoopVar++)
        {
            WriteDspCommDram ((ADDR_RC2D_REVERB_IIR_A_DEC2 + iLoopVar),
                              pdwTable[iLoopVar]);
        }
        //Reverb Gain
        WriteDspCommDram (ADDR_RC2D_REVERB_GAIN_DEC2,
                          u4ReadDspShmDWRD (D_REVERBGAIN_DEC2));
        //Reverb Bank Number
        for (iLoopVar = 0; iLoopVar < 4; iLoopVar++)
        {
            dwBank = (UINT32) (uReadDspShmBYTE ((B_REVERBBANK0_DEC2 + iLoopVar)));
            if (dwBank > MAX_REVERB_BANK)
            {
                vShowError (ERR_ADSP_REVERB_MAX_BANK);
            }
            WriteDspCommDram ((ADDR_RC2D_REVERB_BANK_MAX_DEC2 + iLoopVar), dwBank);
        }
        // DC filter
        WriteDspCommDram (ADDR_RC2D_DCF_COEF_DEC2, DCFilter_COEFF[u4FreqIdx]);
    }
#endif // DSP_POST_PROCESS_ENABLE

}

#ifdef CC_AUD_BBE_SUPPORT
void vSetBbeTable (UINT32 u4FreqIdx, BOOL fgFirstDecoder)
{
    UINT32 iLoopVar;
    UINT32 *pdwTable;

    if (fgFirstDecoder)
    {
        // BBE VIVA 4 bbe_f Coefficients
        pdwTable = (UINT32 *)&BBEV4_BBE_F_TABLE[u4FreqIdx];
        WriteDspCommDram (ADDR_BBEV4_BBE_F, pdwTable[0]);

        //BBE VIVA 4 Surround Coefficient
        pdwTable = (UINT32 *)&BBEV4_IIR_COEF_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
        {
            WriteDspCommDram ((ADDR_BBEV4_IIR_COEF + iLoopVar), pdwTable[iLoopVar]);
        }
    }
}
#endif // CC_AUD_BBE_SUPPORT

void vSetBassManageTable (UINT32 u4FreqIdx, BOOL fgFirstDecoder)
{
    UINT32 iLoopVar;
    UINT32 *pdwTable;

    UINT32 iCutFreq;

    iCutFreq = u4ReadDspShmDWRD (D_CUTOFF_FREQ);
    
    //Bass Management LPF and HPF Coeff.
#ifdef BASS_FILTER_CUSTOMIZE
    if (iCutFreq == 50)
    {
        //pdwTable = (UINT32 *) (BASSM_TABLE_50) + (u4FreqIdx * DSP_BASSM_SZ);
        pdwTable = (UINT32 *)&BASSM_TABLE_50[0];
    }
    else if (iCutFreq == 60)
    {
        //pdwTable = (UINT32 *) (BASSM_TABLE_60) + (u4FreqIdx * DSP_BASSM_SZ);
        pdwTable = (UINT32 *)&BASSM_TABLE_60[0];
    }
    else if (iCutFreq == 80)
    {
        //pdwTable = (UINT32 *) (BASSM_TABLE_80) + (u4FreqIdx * DSP_BASSM_SZ);
        pdwTable = (UINT32 *)&BASSM_TABLE_80[0];
    }
    else if (iCutFreq == 90)
    {
        //pdwTable = (UINT32 *) (BASSM_TABLE_90) + (u4FreqIdx * DSP_BASSM_SZ);
        pdwTable = (UINT32 *)&BASSM_TABLE_90[0];
    }
    else if (iCutFreq == 110)
    {
        //pdwTable = (UINT32 *) (BASSM_TABLE_110) + (u4FreqIdx * DSP_BASSM_SZ);
        pdwTable = (UINT32 *)&BASSM_TABLE_110[0];
    }
    else if (iCutFreq == 120)
    {
        //pdwTable = (UINT32 *) (BASSM_TABLE_120) + (u4FreqIdx * DSP_BASSM_SZ);
        pdwTable = (UINT32 *)&BASSM_TABLE_120[0];
    }
    else if (iCutFreq == 130)
    {
        //pdwTable = (UINT32 *) (BASSM_TABLE_130) + (u4FreqIdx * DSP_BASSM_SZ);
        pdwTable = (UINT32 *)&BASSM_TABLE_130[0];
    }
    else if (iCutFreq == 140)
    {
        //pdwTable = (UINT32 *) (BASSM_TABLE_140) + (u4FreqIdx * DSP_BASSM_SZ);
        pdwTable = (UINT32 *)&BASSM_TABLE_140[0];
    }
    else if (iCutFreq == 150)
    {
        //pdwTable = (UINT32 *) (BASSM_TABLE_150) + (u4FreqIdx * DSP_BASSM_SZ);
        pdwTable = (UINT32 *)&BASSM_TABLE_150[0];
    }
    else if (iCutFreq == 160)
    {
        //pdwTable = (UINT32 *) (BASSM_TABLE_160) + (u4FreqIdx * DSP_BASSM_SZ);
        pdwTable = (UINT32 *)&BASSM_TABLE_160[0];
    }
    else if (iCutFreq == 170)
    {
        //pdwTable = (UINT32 *) (BASSM_TABLE_170) + (u4FreqIdx * DSP_BASSM_SZ);
        pdwTable = (UINT32 *)&BASSM_TABLE_170[0];
    }
    else if (iCutFreq == 180)
    {
        //pdwTable = (UINT32 *) (BASSM_TABLE_180) + (u4FreqIdx * DSP_BASSM_SZ);
        pdwTable = (UINT32 *)&BASSM_TABLE_180[0];
    }
    else if (iCutFreq == 190)
    {
        //pdwTable = (UINT32 *) (BASSM_TABLE_190) + (u4FreqIdx * DSP_BASSM_SZ);
        pdwTable = (UINT32 *)&BASSM_TABLE_190[0];
    }
    else
#endif // BASS_FILTER_CUSTOMIZE
    if (iCutFreq == 70)
    {
        //pdwTable = (UINT32 *) (BASSM_TABLE_70) + (u4FreqIdx * DSP_BASSM_SZ);
        pdwTable = (UINT32 *)&BASSM_TABLE_70[0];
    }
    else if (iCutFreq == 100)
    {
        //pdwTable = (UINT32 *) (BASSM_TABLE_100) + (u4FreqIdx * DSP_BASSM_SZ);
        pdwTable = (UINT32 *)&BASSM_TABLE_100[0];
    }
    else if (iCutFreq == 200)
    {
        //pdwTable = (UINT32 *) (BASSM_TABLE_200) + (u4FreqIdx * DSP_BASSM_SZ);
        pdwTable = (UINT32 *)&BASSM_TABLE_200[0];
    }
    else // 300Hz
    {
        pdwTable = (UINT32 *)&BASSM_TABLE_300[0];
    }
    
    if (fgFirstDecoder)
    {
        for (iLoopVar = 0; iLoopVar < DSP_BASSM_SZ; iLoopVar++)
        {
            WriteDspCommDram (ADDR_RC2D_BASS_MANAGEMENT_FILTER_COEFF +
                              iLoopVar, pdwTable[iLoopVar]);
        }
    }
    else                        // secondary decoder
    {
        for (iLoopVar = 0; iLoopVar < DSP_BASSM_SZ; iLoopVar++)
        {
            WriteDspCommDram (ADDR_RC2D_BASS_MANAGEMENT_FILTER_COEFF_DEC2 +
                              iLoopVar, pdwTable[iLoopVar]);
        }
    }
}

void vSetupUpMix (UINT32 u4FreqIdx,BOOL fgFirstDecoder)
{
#ifndef UPMIX_SUPPORT
    vWriteDspWORD (ADDR_RC2D_PRO_LOGIC_II_CONFIG, 0);
   // vWriteDspWORD (ADDR_RC2D_NEO6_FLAG, 0);
#else // UPMIX_SUPPORT
    UINT32 iLoopVar;
    UINT32 dDspData;
    UINT32 *pdwShareInfo;
    UINT32 dwOfst, dwSz;
    UINT32 dwCodeDestAdr, dwCodeSrcAdr;
    UINT32 dwFlashOfst;
    UINT32 dwFlashOfst1;

    dwFlashOfst = u4GetDspImgAddr ();
#ifndef PROLOGICII_SUPPORT
    vWriteDspWORD (ADDR_RC2D_PRO_LOGIC_II_CONFIG, 0);
#else
    //Prologic II
    //vWriteDspWORD(ADDR_RC2D_PRO_LOGIC_II_CONFIG, u2DspData);
    HalFlushInvalidateDCache(); //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro
    dDspData = wReadDspWORD (ADDR_RC2D_PRO_LOGIC_II_CONFIG);
    //Check if Pro Logic II
    if (dDspData & 0x8001)
    {
        //Prologic II
        vWriteDspWORD (ADDR_RC2D_PRO_LOGIC_II_MODE,
                       u2ReadDspShmWORD (W_PLIIMODE));
        if (_bCode3DUpmix != CODE3D_PROLOGIC_II)
        {
            //Set _bCode3DUpmix = CODE3D_PROLOGIC_II
            _bCode3DUpmix = CODE3D_PROLOGIC_II;
            //PL2 Buffer Address
            WriteDspCommDram (ADDR_RC2D_3D_BUFFER_ADDR,
                              u4ReadDspShmDWRD (D_3D_BUF_ADDR));
            //Prologic II Table
            dwFlashOfst1 = u4GetDspImgAddr ();
            pdwShareInfo = (UINT32 *) u4GetDspImgAddr ();
            pdwShareInfo +=
                u4GetFlashData (dwFlashOfst1 + (DSPF_PL2_TABLE_SA << 2));
            pdwShareInfo += (u4FreqIdx * DSP_PL2_SZ);
            for (iLoopVar = 0; iLoopVar < DSP_PL2_SZ; iLoopVar++)
            {
                WriteDspCommDram (ADDR_PL2_TABLE + iLoopVar,
                                  pdwShareInfo[iLoopVar]);
            }
            /*lint -e{506}*/
            /*lint -e{774}*/
            if (_fgPostRamFromFlash)
            {
                // Download program code
                dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_PL2_POST_SA << 2));
                dwSz = u4GetFlashData (dwFlashOfst + (DSPF_PL2_POST_SZ << 2));
                dwCodeDestAdr =
                    (dwGetDspIcacheAdr (TYPE_POST_ICACHE) +
                     MULTI_SURROUND_PRAM_ADR) * 4;
                // Rice: check if exceed to next section @20060804
                ASSERT((dwCodeDestAdr + ((dwSz - 1) * 4)) < 
                       ((dwGetDspIcacheAdr (TYPE_POST_ICACHE)+VIRTUAL_SURROUND_PRAM_ADR) * 4));
                dwCodeSrcAdr = u4GetDspImgAddr () + (dwOfst * 4);
                vFlash2DramDma (dwCodeDestAdr, dwCodeSrcAdr, (dwSz - 1) * 4);
            }
        }
    }
#endif

    //NEO:6
 //   vWriteDspWORD (ADDR_RC2D_NEO6_FLAG, 0);
#endif // UPMIX_SUPPORT
    UNUSED(fgFirstDecoder);
}

void vSetupDownMix (UINT32 u4FreqIdx, BOOL fgFirstDecoder)
{
#ifndef DOWNMIX_SUPPORT
    //Virtual Surround
    //Be careful of multi-flag on
    //Virtual Surround & Spatializer
    vWriteDspWORD (ADDR_RC2D_VIRTUAL_SURROUND, 0);
    vWriteDspWORD (ADDR_RC2D_VIRTUAL_SURROUND_DEC2, 0);
#else
    UINT32 iLoopVar;
    UINT32 *pdwShareInfo;
    UINT32 dwOfst = 0;
    UINT32 dwSz = 0;
    //UINT32 dwCodeDestAdr, dwCodeSrcAdr;
    //UINT32 dwFlashOfst;
    UINT32 dVSCfg;

    //dwFlashOfst = u4GetDspImgAddr () ;
    //Virtual Surround
    //Be careful of multi-flag on
    //Virtual Surround & Spatializer

    // 20030912 modified
    if (fgFirstDecoder)
    {   // fisrt decoder
        HalFlushInvalidateDCache(); //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro
        dVSCfg = wReadDspWORD (ADDR_RC2D_VIRTUAL_SURROUND) & 0xff;    
        if (VSS_MTK == dVSCfg)
        {
#if (0)        
            //reinitial raised from first decoder
            //configuration for first decoder
            //Virtual Surround Gain
            WriteDspCommDram (ADDR_RC2D_VIRTUAL_SURROUND_GAIN,
                              u4ReadDspShmDWRD (D_VSURRGAIN));
            //Virtual Surround Widen Gain
            WriteDspCommDram (ADDR_RC2D_VIRTUAL_SURROUND_WIDEN_GAIN,
                              u4ReadDspShmDWRD (D_VSURRWIDEGAIN));
            //Virtual Surround Delay Number
            vWriteDspWORD (ADDR_RC2D_VIRTUAL_SURROUND_DELAY_NUMBER,
                           (UINT16) uReadDspShmBYTE (B_VSURRDELAYNUM));
#endif            
            if ((_bCode3D != CODE3D_VIRTUAL_SURROUND))
            {
#if (0)            
                //Virtual Surround Cross Filter Coefficient
                //pdwShareInfo =
                //    (UINT32 *) (VSURR_TABLE) + (_dwTableFreq * DSP_VSURR_SZ);
                pdwShareInfo = (UINT32 *)&VSURR_TABLE[_dwTableFreq][0];
                for (iLoopVar = 0; iLoopVar < DSP_VSURR_SZ; iLoopVar++)
                {
                    WriteDspCommDram (ADDR_RC2D_VIRTUAL_SURROUND_FILTER_COEFF +
                                      iLoopVar, pdwShareInfo[iLoopVar]);
                }
#endif                
                _bCode3D = CODE3D_VIRTUAL_SURROUND;
            }
        }
        #ifdef DSP_SUPPORT_SRSWOW// Rice: for SRSes
        else if (VSS_SRS_WOW == (dVSCfg & 0xFFFFFFFE))
        {
            UINT32 u4TblOfst;
            UINT32 u4TblDstAdr;
            const UINT32 *u4TblSrcAdr;
            UINT32 u4FSid = u4FreqIdx;
            UINT32 u4spksize;
            UINT32 u4BIMPROT;

            // Get SRS WOW Table Start Address
            u4TblOfst     = u4GetDspImgAddr ();
            u4TblSrcAdr   = (UINT32 *)u4TblOfst;
            u4TblSrcAdr  += 
            u4GetFlashData (u4TblOfst + (DSPF_SRS_WOW_TBL_SA << 2));

            // Chk Input Fs
            if (u4FSid < SAMPLE_32K) u4FSid = SAMPLE_32K; 
            if (u4FSid > SAMPLE_48K) u4FSid = SAMPLE_48K;

            //Init. WOW Data Section: Control Parameters
            {
                u4spksize = u4ReadDspShmDWRD(D_SRS_WOW_SPKSIZE);
                WriteDspCommDram(ADDR_SRS_WOW_DATA+ 0, u4ReadDspShmDWRD(D_SRS_WOW_ENABLE )); //mEnable*
                WriteDspCommDram(ADDR_SRS_WOW_DATA+ 1, u4ReadDspShmDWRD(D_SRS_WOW_LMENB  )); //mLimiterEnable*
                WriteDspCommDram(ADDR_SRS_WOW_DATA+ 8, u4ReadDspShmDWRD(D_SRS_WOW_BRIGHT )); //mBrightness*
                WriteDspCommDram(ADDR_SRS_WOW_DATA+ 9, u4ReadDspShmDWRD(D_SRS_WOW_WOWCTRL)); //mWowControl*
                WriteDspCommDram(ADDR_SRS_WOW_DATA+10, u4ReadDspShmDWRD(D_SRS_WOW_TBCTRL )); //mTruBassControl*
                WriteDspCommDram(ADDR_SRS_WOW_DATA+11, u4ReadDspShmDWRD(D_SRS_WOW_LMCTRL )); //mLimiterControl*
                // User non-adjustable
                WriteDspCommDram(ADDR_SRS_WOW_DATA+ 2, 0x00000100); //mHighBitRate
                WriteDspCommDram(ADDR_SRS_WOW_DATA+ 3, 0x00000000); //mBypassCompressor**
                WriteDspCommDram(ADDR_SRS_WOW_DATA+ 4, 0x00000100); //mTBSpeakerSize
                WriteDspCommDram(ADDR_SRS_WOW_DATA+ 5, 0x00000000); //mSRSSpeakerSize
                //WriteDspCommDram(ADDR_SRS_WOW_DATA+ 6, 0x00200000); //mInputGain: -6dB
                WriteDspCommDram(ADDR_SRS_WOW_DATA+ 6, 0x002DC243); //mInputGain: -3dB
                WriteDspCommDram(ADDR_SRS_WOW_DATA+ 7, 0x00400000); //mProcessOutputGain
                WriteDspCommDram(ADDR_SRS_WOW_DATA+12, 0x00400000); //mLimiterGain
                WriteDspCommDram(ADDR_SRS_WOW_DATA+13, 0x00400000); //mLimiterInverseGain
                WriteDspCommDram(ADDR_SRS_WOW_DATA+14, 0x00400000); //mTruBassReferenceLevel
                WriteDspCommDram(ADDR_SRS_WOW_DATA+15, 0x00200000); //mLimiterLevel_msb
                WriteDspCommDram(ADDR_SRS_WOW_DATA+16, 0x00000000); //mLimiterLevel_lsb
                WriteDspCommDram(ADDR_SRS_WOW_DATA+17, 0x00000000); //mFasdLevel
                WriteDspCommDram(ADDR_SRS_WOW_DATA+18, 0x00000000); //mIntegrator
                WriteDspCommDram(ADDR_SRS_WOW_DATA+19, 0x00166667); //integrationLimit
            }
            
            // Init. WOW RODATA Section: D/L Table
    	    u4TblDstAdr  = ADDR_SRS_WOW_RODATA;
            #if 0
    	    u4TblSrcAdr += u4FSid * DSP_TBL_SRS_WOW_FSENTSZ;
            #else
    	    u4TblSrcAdr = &SRSWOW_TBL[0];
            #endif
    	    //SRS WOW Tables Step 1.: 6 dwords
            for (iLoopVar = 0; iLoopVar < 6; iLoopVar++)
            {
                WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
            }
            u4TblDstAdr += 6;
            u4TblSrcAdr += 6;

            //SRS WOW Tables Step 2.: 8 dwords
            for (iLoopVar = 0; iLoopVar < 8; iLoopVar++)
            {
                WriteDspCommDram (u4TblDstAdr+iLoopVar,
                u4TblSrcAdr[u4spksize/* mTBSpeakerSize */*8 + iLoopVar]);
            }
            u4TblDstAdr += 8;
            u4TblSrcAdr += 8*8; /* 8 sets of 8 words */

            //SRS WOW Tables Step 3.: 6 dwords
            for (iLoopVar = 0; iLoopVar < 6; iLoopVar++)
            {
                WriteDspCommDram (u4TblDstAdr+iLoopVar,
                u4TblSrcAdr[(0/*mSRSSpeakerSize: 0: speaker mode, 1: headphone mode*/*12)+(1/*mHighBitRate*/*6) + iLoopVar]);
            }
            u4TblDstAdr += 6;
            u4TblSrcAdr += 4*6; /* 4 sets of 6 words */

            //SRS WOW Tables Step 4.: 16 dwords
            for (iLoopVar = 0; iLoopVar < 16; iLoopVar++)
            {
                WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
            }

            IO_WRITE32(BIM_BASE, 0x660, 0x00000001);
			IO_WRITE32(BIM_BASE, 0x670, 0x00002000);
			IO_WRITE32(BIM_BASE, 0x674, 0x00500000);
			IO_WRITE32(BIM_BASE, 0x678, 0x00000060);
			IO_WRITE32(BIM_BASE, 0x67c, 0x90000000);
			u4BIMPROT = IO_READ32(BIM_BASE, 0x650);

			if ((u4BIMPROT&0x00FF0000)!= 0xFF0000){
				WriteDspCommDram(ADDR_SRS_WOW_RODATA+36, 0);
                //LOG( 9,"u4BIMPROT (0x%8x)\n",u4BIMPROT);
			}else{
				
				WriteDspCommDram(ADDR_SRS_WOW_RODATA+36, 0x100);
                //LOG( 9,"u4BIMPROT (0x%8x)\n",u4BIMPROT);	
			}

			IO_WRITE32(BIM_BASE, 0x670, 0x00000000);
			u4BIMPROT = IO_READ32(BIM_BASE, 0x650);
			
            // D/L DSP Code
            if(_bCode3D != CODE3D_SRS_WOW)
            {
                #if 0
                UINT32 u4CodeDestAdr,
                UINT32 u4CodeSrcAdr;
                UINT32 u4StrAddr = u4GetDspImgAddr ();

                dwOfst = u4GetFlashData (u4StrAddr + (DSPF_SRS_WOW_SA << 2));
                dwSz   = u4GetFlashData (u4StrAddr + (DSPF_SRS_WOW_SZ << 2));

                u4CodeDestAdr =
                    (dwGetDspIcacheAdr (TYPE_POST_ICACHE) + DOWNMIX_PRAM_ADR) * 4;
                u4CodeSrcAdr = u4GetDspImgAddr () + (dwOfst * 4);
                vFlash2DramDma (u4CodeDestAdr, u4CodeSrcAdr, (dwSz - 1) * 4);
                #endif
                _bCode3D = CODE3D_SRS_WOW;
            }
        }
        #endif //DSP_SUPPORT_SRSWOW
        #ifdef DSP_SUPPORT_SRSTSXT
        else if (VSS_SRS_TSXT == (dVSCfg & 0xFFFFFFFE))
        {
            UINT32 u4TblDstAdr;
            UINT32 *u4TblSrcAdr;
            UINT32 u4FSid;
		    UINT32 u4tsxt_fg;
		    UINT32 u4fc_elev;
		    UINT32 u4tb_ctrl;
            UINT32 u4input_gain;
		    UINT32 u4spksize;
		    UINT32 u4BIMPROT;

            u4FSid       = u4FreqIdx;
		    u4tsxt_fg    = u4ReadDspShmDWRD(D_SRS_TSXT_FLAGS);
		    u4fc_elev    = u4ReadDspShmDWRD(D_SRS_TSXT_FC_Elevation);
		    u4tb_ctrl    = u4ReadDspShmDWRD(D_SRS_TSXT_TB_TBCtrl);
            u4input_gain = u4ReadDspShmDWRD(D_SRS_TSXT_Input_gain);
		    u4spksize    = (u4tsxt_fg>>24) & 0x0000000F;

            // Chk Input Fs
            if (u4FSid > SAMPLE_48K) u4FSid = SAMPLE_48K;

            // Init. TSXT Data Section: Control Paremeters
            {
    			// User adjustable
                WriteDspCommDram (ADDR_SRS_TSXT_DATA + 0, ((u4tsxt_fg >> 16) & 0x0000000F) << 8);	//m_fc.m_Enable; 0x00010, false or true
        		WriteDspCommDram (ADDR_SRS_TSXT_DATA + 1, u4fc_elev);	                            //m_Elevation ; 0x40000
        		WriteDspCommDram (ADDR_SRS_TSXT_DATA + 2, ((u4tsxt_fg >> 20) & 0x0000000F) << 8);	//m_tbc.m_Enable; 0x00010, false or true
        		WriteDspCommDram (ADDR_SRS_TSXT_DATA + 3, ((u4tsxt_fg >>  4) & 0x0000000F) << 8);	//m_TruSurround; 0x00000, false or true
        		WriteDspCommDram (ADDR_SRS_TSXT_DATA + 4, ((u4tsxt_fg) & 0x0000000F) << 8);	        //Bypass
        		WriteDspCommDram (ADDR_SRS_TSXT_DATA + 6, ((u4tsxt_fg >> 12) & 0x0000000F) << 8);	//Channel mode
        		WriteDspCommDram (ADDR_SRS_TSXT_DATA + 7, ((u4tsxt_fg >>  8) & 0x0000000F) << 8);	//m_Headphone; 0x00000, false or true
        		WriteDspCommDram (ADDR_SRS_TSXT_DATA + 8, u4tb_ctrl);	                            //m_TruBassControl; 0x4cccd

        		//user un-adjustable parameters
        		#if 0
        		WriteDspCommDram (ADDR_SRS_TSXT_DATA +  5, 0x00400000);	 //m_InputGain; 0x40000 = -6dB
        		#else
        		//WriteDspCommDram (ADDR_SRS_TSXT_DATA +  5, 0x005ae140);	 //-3dB
        		WriteDspCommDram (ADDR_SRS_TSXT_DATA +  5, u4input_gain);	 //-3dB
                #endif
        		WriteDspCommDram (ADDR_SRS_TSXT_DATA +  9, 0x005a3d71);	 //m_RearSpaceControl
        		WriteDspCommDram (ADDR_SRS_TSXT_DATA + 10, 0x0050a3d7);  //m_FrontSpaceControl
        		WriteDspCommDram (ADDR_SRS_TSXT_DATA + 11, 0x002d70a0);  //m_FrontCenterControl
        		WriteDspCommDram (ADDR_SRS_TSXT_DATA + 12, 0x00090620);  //m_RearCenterControl
        		WriteDspCommDram (ADDR_SRS_TSXT_DATA + 13, 0x00000000);  //m_ExpandorLevel
        		WriteDspCommDram (ADDR_SRS_TSXT_DATA + 14, 0x00120670);  //m_ExpandorGain, varies with input gain
            }

    		// Rice: only load RODATA, TSXT2CH's DATA section shall be set in API.
    	    u4TblDstAdr  = ADDR_SRS_TSXT_RODATA;
     	    u4TblSrcAdr = &SRSTSXT2CH_TBL[0];
            // Init. TSXT Data Section: D/L Table
		    //SRS TSXT Tables Step 1.: 18 dwords (M2S)
            for (iLoopVar = 0; iLoopVar < 18; iLoopVar++)
            {
                WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
            }
    		u4TblDstAdr += 18;
    		u4TblSrcAdr += 18;
		    //SRS TSXT Tables Step 2.: 11 dwords (TB)
            for (iLoopVar = 0; iLoopVar < 11; iLoopVar++)
            {
                WriteDspCommDram (u4TblDstAdr+iLoopVar, 
                    u4TblSrcAdr[u4spksize/* SpeakerSize */*11 + iLoopVar]);
            }
            u4TblDstAdr += 11;
            u4TblSrcAdr += 11*8; /* 8 sets of 11 words */
            
		    //SRS TSXT Tables Step 3.: 2 dwords ("Attack" & "Decay") (TB)
            for (iLoopVar = 0; iLoopVar < 2; iLoopVar++)
            {
                WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
            }
    		u4TblDstAdr += 2;
    		u4TblSrcAdr += 2;

     		//SRS TSXT Tables Step 4.: 9 dwords (TS 2-ch), skip 9 dwords (for future TS multi-ch)
            for (iLoopVar = 0; iLoopVar < 9; iLoopVar++)
            {
                WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
            }
            u4TblDstAdr += 9;
            u4TblSrcAdr += 18;  //skip 9 dwords 

            //SRS TSXT Tables Step 6.: 8 dwords (FOCUS)
            for (iLoopVar = 0; iLoopVar < 8; iLoopVar++)
            {
                WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
            }
		    u4TblDstAdr += 8;
		    u4TblSrcAdr += 8;

            //SRS TSXT Tables Step 7.: 9 dwords (constants)
            for (iLoopVar = 0; iLoopVar < 9; iLoopVar++)
            {
                WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
            }

			
       		IO_WRITE32(BIM_BASE, 0x660, 0x00000001);
			IO_WRITE32(BIM_BASE, 0x670, 0x00002000);
			IO_WRITE32(BIM_BASE, 0x674, 0x00500000);
			IO_WRITE32(BIM_BASE, 0x678, 0x00000060);
			IO_WRITE32(BIM_BASE, 0x67c, 0x90000000);
			u4BIMPROT = IO_READ32(BIM_BASE, 0x650);
           	//LOG( 9,"u4BIMPROT (0x%8x)\n",u4BIMPROT);

#ifndef __KERNEL__
			if ((u4BIMPROT&0x0000FF00)!= 0xFF00){
				WriteDspCommDram(ADDR_SRS_TSXT_RODATA+57, 0);
			}else{
				
			    WriteDspCommDram(ADDR_SRS_TSXT_RODATA+57, 0x200);
			}
#else
            WriteDspCommDram(ADDR_SRS_TSXT_RODATA+57, 0x200);
#endif

			IO_WRITE32(BIM_BASE, 0x670, 0x00000000);
			u4BIMPROT = IO_READ32(BIM_BASE, 0x650);

            // D/L DSP Code
            if(_bCode3D != CODE3D_SRS_TSXT)
            {
                #if 0
                UINT32 u4CodeDestAdr,
                UINT32 u4CodeSrcAdr;
                UINT32 u4StrAddr = u4GetDspImgAddr ();

                dwOfst = u4GetFlashData (u4StrAddr + (DSPF_SRS_TSXT_SA << 2));
                dwSz   = u4GetFlashData (u4StrAddr + (DSPF_SRS_TSXT_SZ << 2));

                u4CodeDestAdr =
                    (dwGetDspIcacheAdr (TYPE_POST_ICACHE) + DOWNMIX_PRAM_ADR) * 4;
                u4CodeSrcAdr = u4GetDspImgAddr () + (dwOfst * 4);
                vFlash2DramDma (u4CodeDestAdr, u4CodeSrcAdr, (dwSz - 1) * 4);
                #endif
                _bCode3D = CODE3D_SRS_TSXT;
            }
        }
        #endif //DSP_SUPPORT_SRSTSXT
        #ifdef DSP_SUPPORT_SRSTSHD //sunman_tshd
        else if (VSS_SRS_TSXT == (dVSCfg & 0xFFFFFFFE))
        {        
		UINT32 u4TblDstAdr;
		UINT32 *u4TblSrcAdr;
		UINT32 u4FSid;
		UINT32 u4tsxt_fg;
		UINT32 u4fc_elev;
		UINT32 u4tb_ctrl;
		UINT32 u4input_gain;
		UINT32 u4spksize;
		//UINT32 u4BIMPROT;
		//UINT32 u4temp,u4temp1,u4temp2,u4temp3,u4temp4,u4temp5,u4temp6,u4temp7,u4temp8;
		UINT32 u4headphone;
		UINT32 u4def_ctrl;	
		UINT32 u4limiter_ctrl;
		UINT32 u4sub_spksize;
		UINT32 u4co_freq;
		UINT32 u4inp_type;
		UINT32 u4ch_dest;
		UINT32 u4wow_fc_ctrl;
		UINT32 u4center_ctrl, u4space_ctrl;
		UINT32 u4surr_lvl;
		UINT32 u4output_gain;

		u4FSid       	= u4FreqIdx;
		if (u4FSid > SAMPLE_48K) 
            		u4FSid = SAMPLE_48K;

		u4input_gain	= u4ReadDspShmDWRD(D_SRS_TSHD_INPUT_GAIN);//0x00333333	;//-8 dB		 
		u4output_gain	= u4ReadDspShmDWRD(D_SRS_TSHD_OUTPUT_GAIN);//0x007FFFFF	;//0 dB
		u4tsxt_fg   	= u4ReadDspShmDWRD(D_SRS_TSHD_FLAGS);
		u4headphone  = (u4tsxt_fg & 0x00000080) >> 7;
		u4wow_fc_ctrl = u4ReadDspShmDWRD(D_SRS_TSHD_WOW_FC_ctrl);
		u4fc_elev		= u4ReadDspShmDWRD(D_SRS_TSHD_FC_Elevation);
		u4tb_ctrl		= u4ReadDspShmDWRD(D_SRS_TSHD_TB_TBCtrl);
		u4def_ctrl		= u4ReadDspShmDWRD(D_SRS_TSHD_DefCtrl);
		u4limiter_ctrl	= u4ReadDspShmDWRD(D_SRS_TSHD_LimiterCtrl);

		u4spksize    	= (UINT32)uReadDspShmBYTE(B_SRS_TSHD_SPKSIZE);
		u4sub_spksize  	= (UINT32)uReadDspShmBYTE(B_SRS_TSHD_SUBSPKSIZE);
		u4co_freq		= (UINT32)uReadDspShmBYTE(B_SRS_TSHD_COFREQ);

		u4inp_type  	= (UINT32)uReadDspShmBYTE(B_SRS_TSHD_INP_TYPE);
		u4ch_dest		= (UINT32)uReadDspShmBYTE(B_SRS_TSHD_CH_DEST);

		u4center_ctrl	= u4ReadDspShmDWRD(D_SRS_TSHD_Center_Ctrl);
		u4space_ctrl	= u4ReadDspShmDWRD(D_SRS_TSHD_Space_Ctrl);

		u4surr_lvl 	= u4ReadDspShmDWRD(D_SRS_TSHD_Surr_Level);

		WriteDspCommDram (ADDR_SRS_TSHD_DATA + 0, u4tsxt_fg << 8);	
		WriteDspCommDram (ADDR_SRS_TSHD_DATA +  1,u4fc_elev);    	//m_Elevation ; 0x40000
		WriteDspCommDram (ADDR_SRS_TSHD_DATA +  2, 0    << 8);			//ts_mode
		WriteDspCommDram (ADDR_SRS_TSHD_DATA +  3, u4tb_ctrl);	//m_TruBassControl; 0.3  for test (u4tb_ctrl=0.6)
		
		//user un-adjustable parameters
              WriteDspCommDram (ADDR_SRS_TSHD_DATA +  4, u4input_gain);	 //m_InputGain; 0x40000 = 0.5
		WriteDspCommDram (ADDR_SRS_TSHD_DATA +  5, 0x00000000);	 //m_RearSpaceControl
		WriteDspCommDram (ADDR_SRS_TSHD_DATA +  6, 0x00000000);  	//m_RearCenterControl
		WriteDspCommDram (ADDR_SRS_TSHD_DATA +  7, u4space_ctrl);  	//m_FrontSpaceControl
		WriteDspCommDram (ADDR_SRS_TSHD_DATA +  8, u4center_ctrl);  	//m_FrontCenterControl
		WriteDspCommDram (ADDR_SRS_TSHD_DATA +  9, u4def_ctrl   );  	//m_definitionControl    0.3 scale 0
		WriteDspCommDram (ADDR_SRS_TSHD_DATA + 10, u4surr_lvl);  	//SurroundLevel		 0.6
		WriteDspCommDram (ADDR_SRS_TSHD_DATA + 11, 0x007e2bcf);  	//tweak_gain              0.9857119009006160
		//WriteDspCommDram (ADDR_SRS_TSHD_DATA + 12, 0x005ae148);  //ts_bypass_gain      	1
		WriteDspCommDram (ADDR_SRS_TSHD_DATA + 12, u4output_gain);  //ts_bypass_gain used as output gain    
		WriteDspCommDram (ADDR_SRS_TSHD_DATA + 13, u4wow_fc_ctrl);  //ts_wow_focus_ctrl   0.4  scale 0
		WriteDspCommDram (ADDR_SRS_TSHD_DATA + 14, u4limiter_ctrl);  //ts_limiter_ctrl         0.75
		WriteDspCommDram (ADDR_SRS_TSHD_DATA + 15, 0x00400000);  //ts_LowPassMixGain	0.5
		//Constants
		WriteDspCommDram (ADDR_SRS_TSHD_DATA + 16, 0x0002d70a);  //ts_IntegrationConstant	0.71 	scale 5
		WriteDspCommDram (ADDR_SRS_TSHD_DATA + 17, 0x00028f5c);  //ts_FeedbackConstant	       0.02 scale 0	
		WriteDspCommDram (ADDR_SRS_TSHD_DATA + 18, 0x000f3333);  //ts_IntegrationLimit		sacle  5 /* Set integrationLimit = 2 + truBassControl * 6 */		
		WriteDspCommDram (ADDR_SRS_TSHD_DATA + 19, 0x007fffff);  //kWOWOne			
		WriteDspCommDram (ADDR_SRS_TSHD_DATA + 20, 0x00200000);  //kWOWLimiterMinAdjustLevel   0.25 scale 0
		WriteDspCommDram (ADDR_SRS_TSHD_DATA + 21, 0x0019999a);  //kWOWLimiterLevelRatio	     0.2 scale  0
		WriteDspCommDram (ADDR_SRS_TSHD_DATA + 22, 0x007fffff);  //kWOWLimiterMaxMultiplier		1  scale 0
		WriteDspCommDram (ADDR_SRS_TSHD_DATA + 23, 0x00400000);  //kWOWLimiterLevelMax		1 scale 1
		WriteDspCommDram (ADDR_SRS_TSHD_DATA + 24, 0xffd00000);  //kWOWLimiterLevelMin_s_Max	(0.25 -1) scale 1
		WriteDspCommDram (ADDR_SRS_TSHD_DATA + 25, 0x00400000);  //kWOWLimiterLevelDefault		1 scale 1
		//need backup data
		WriteDspCommDram (ADDR_SRS_TSHD_DATA + 26, 0x00000000);  //ts_RefLevel		
		WriteDspCommDram (ADDR_SRS_TSHD_DATA + 27, 0x00000000);  //ts_FasdLevel		
		WriteDspCommDram (ADDR_SRS_TSHD_DATA + 28, 0x00000000);  //ts_Integrator		
		WriteDspCommDram (ADDR_SRS_TSHD_DATA + 29, 0x00200000);  //ts_limiter_lv_msb
		WriteDspCommDram (ADDR_SRS_TSHD_DATA + 30, 0x00000000);  //ts_limiter_lv_lsb  
		WriteDspCommDram (ADDR_SRS_TSHD_DATA + 31, 0x00000000);  //ts_sub_FasdLevel		
		WriteDspCommDram (ADDR_SRS_TSHD_DATA + 32, 0x00000000);  //ts_sub_Integrator		
		
		//ts_Attack, ts_Decay, kWOWLimiterDecay	
		WriteDspCommDram (ADDR_SRS_TSHD_DATA + 47, SRSTSHD_M_TBL[u4FSid][0]);  //ts_Attack
		WriteDspCommDram (ADDR_SRS_TSHD_DATA + 48, SRSTSHD_M_TBL[u4FSid][1]);  //ts_Decay
		WriteDspCommDram (ADDR_SRS_TSHD_DATA + 49, SRSTSHD_M_TBL[u4FSid][2]);  //kWOWLimiterDecay
		
		u4TblSrcAdr = (UINT32 *)&SRSTSHD_M_TBL[u4FSid][3];

           	//M2S (6)
		u4TblDstAdr  = ADDR_SRS_TSHD_M2S_COEF;
		for (iLoopVar = 0; iLoopVar < 30; iLoopVar++)
		{
			WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
		}
		u4TblSrcAdr += 30;

           	//persp front
		u4TblDstAdr  = ADDR_SRS_TSHD_PERSP_FR_COEF;		
		for (iLoopVar = 0; iLoopVar < 15; iLoopVar++)
		{
			WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
		}
		u4TblSrcAdr += 15;

           	//FOCUS
		u4TblDstAdr  = ADDR_SRS_TSHD_FOCUS_COEF;		
		for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
		{
			WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
		}
		u4TblSrcAdr += 5;

           	//persp rear
		u4TblDstAdr  = ADDR_SRS_TSHD_PERSP_RR_COEF;		
		for (iLoopVar = 0; iLoopVar < 15; iLoopVar++)
		{
			WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
		}
		u4TblSrcAdr += 15;

		//TB, sub TB
		u4TblDstAdr  = ADDR_SRS_TSHD_TB_COEF;		
		for (iLoopVar = 0; iLoopVar < 15; iLoopVar++)
		{
			WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[u4spksize*15 + iLoopVar]);
		}
		u4TblDstAdr  = ADDR_SRS_TSHD_SUB_TB_COEF;		
		for (iLoopVar = 0; iLoopVar < 15; iLoopVar++)
		{
			WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[u4sub_spksize*15 + iLoopVar]);
		}		
		u4TblSrcAdr += 60;// 15*4

           	//Def
		u4TblDstAdr  = ADDR_SRS_TSHD_DEF_COEF;
		for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
		{
			WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
		}
		u4TblSrcAdr += 5;

		//cross over
		u4TblDstAdr  = ADDR_SRS_TSHD_CROSS_COEF;
		for (iLoopVar = 0; iLoopVar < 10; iLoopVar++)
		{
			WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[u4co_freq*10 + iLoopVar]);
		}
		u4TblSrcAdr += 40; //10*4

		//SRS3D
		u4TblDstAdr  = ADDR_SRS_TSHD_SRS3D_COEF;
		for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
		{
			WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[u4headphone*5 + iLoopVar]);
		}
		for (iLoopVar = 5; iLoopVar < 10; iLoopVar++)
		{
			WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar+5]);
		}
		u4TblSrcAdr += 15; //5*2+5		
				
		WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  0, 0x00400000 );	//kPlus12dB	scale 3
		WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  1, 0x005a3d71 );	//kPlus9dB	scale 2	
		//WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  2, 0x005a3d71 );	//kPlus3dB	scale 1
		WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  2, 0x0071eb85 );	//kPlus5dB	scale 1
		WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  3, 0x005ae148 );	//kMinus3dB	scale 0
		WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  4, 0x0050a3d7 );	//kMinus4dB	scale 0
		WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  5, 0x0047ae14 ); 	//kMinus5dB	scale 0
		//WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  7, 0x00400000 );	//kMinus6dB	scale 0
		//WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  8, 0x005ae148 );	//kMinus9dB	scale -1
		WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  6, 0x00090625 );	//kMinus11dB_scale2	
		WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  7, 0x00404189 );	//kMinus12dB	scale -1
		WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  8, 0x0050e560 );	//kMinus16dB	scale -2
		WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  9, 0x00408312 );	//kMinus18dB	scale -2
		WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST + 10, 0x00666666 );	//kMinus20dB	scale -3
		WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST + 11, 0x00551eb8 );	//kPoint665	scale  0
		WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST + 12, 0x00570a3d );	//kPoint34		scale -1

		WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST + 13, u4inp_type <<8);	//kPoint665	scale  0
		WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST + 14, u4ch_dest <<8);

            if(_bCode3D != CODE3D_SRS_TSXT)
            {                
                _bCode3D = CODE3D_SRS_TSXT;
            }
	}
        #endif //DSP_SUPPORT_SRSTSHD
    }
    else
    {
        // second decoder
        dVSCfg = wReadDspWORD (ADDR_RC2D_VIRTUAL_SURROUND_DEC2) & 0xff;
        if (VSS_MTK == dVSCfg)
        {
            //reinitial raised from first decoder
            //configuration for first decoder
            //Virtual Surround Gain
            WriteDspCommDram (ADDR_RC2D_VIRTUAL_SURROUND_GAIN_DEC2,
                              u4ReadDspShmDWRD (D_VSURRGAIN_DEC2));
            //Virtual Surround Widen Gain
            WriteDspCommDram (ADDR_RC2D_VIRTUAL_SURROUND_WIDEN_GAIN_DEC2,
                              u4ReadDspShmDWRD (D_VSURRWIDEGAIN_DEC2));
            //Virtual Surround Delay Number
            vWriteDspWORD (ADDR_RC2D_VIRTUAL_SURROUND_DELAY_NUMBER_DEC2,
                       (UINT16) uReadDspShmBYTE (B_VSURRDELAYNUM_DEC2));
            if ((_bCode3DDec2 != CODE3D_VIRTUAL_SURROUND))
            {
                //Virtual Surround Cross Filter Coefficient
                //pdwShareInfo =
                //    (UINT32 *) (VSURR_TABLE) + (_dwTableFreqDec2 * DSP_VSURR_SZ);
                pdwShareInfo = (UINT32 *)&VSURR_TABLE[0];
                for (iLoopVar = 0; iLoopVar < DSP_VSURR_SZ; iLoopVar++)
                {
                   WriteDspCommDram (ADDR_RC2D_VIRTUAL_SURROUND_FILTER_COEFF_DEC2 +
                                     iLoopVar, pdwShareInfo[iLoopVar]);
                }
                _bCode3DDec2 = CODE3D_VIRTUAL_SURROUND;
            }
        }
    }

    // Just fix warning message for -O2
    UNUSED(dwOfst);
    UNUSED(dwSz);
    UNUSED(u4FreqIdx);
#endif //DOWNMIX_SUPPORT
    
}

#ifdef VORBIS_SUPPORT

/* DSP Dram Space Definition */
/* decoder page 0x0000 ~ 0x0400 */
#define ADDR_VBSD2RC_DRAM_GENERAL_INFO                   0x0000 // 0x8000
#define ADDR_VBSD2RC_DRAM_ID_HEADER                      0x0020 // 0x8020
#define ADDR_VBSD2RC_DRAM_SETUP_HEADER                   0x0030 // 0x8030
#define ADDR_VBSD2RC_DRAM_CODEBOOK_PTR_ADR               0x0040 // 0x8040
#define ADDR_VBSD2RC_DRAM_COMMENT_HEADER                 0x0100 // 0x8100

#define ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_STACK            0x0180 // 0x8180 (0x0020)
#define ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_CODE             0x4000 // 0xc000 (0x2000)
#define ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_VALUE            0x6000 // 0xe000 (0x2000)

#define ADDR_VBSD2RC_DRAM_VORBIS_CODEBOOK_COUNT          (ADDR_VBSD2RC_DRAM_GENERAL_INFO+0x00)

/* For Function Declaration */
BOOL fgConstructHuffmanTree (UINT32 dwBookAdr, UINT32 dwBookLen);
BOOL fgMakeHuffmanCode (UINT32 dwBookAdr, UINT32 dwBookLen);
void vVorbisQuickSort (UINT32 dwValueAdr, UINT32 dwCodeAdr, UINT32 dwBookLen);

/* For Emulation data */
#ifdef EMULATION_FPGA
/* Reuse the DSP DRAM when on target. Use local(PC) memory for efficiency on emulation FPGA */
UINT32 dwCodeList[0x2000];
UINT32 dwValueList[0x2000];
UINT32 dwBookStack[0x20];
#endif

/* For Huffman Tree Construction */
#define LEFT_TREE   0
#define RIGHT_TREE  1

/* For Quick Sort Data */
#define BOOK_STACK_DEPTH  32

typedef struct QuickSortStack
{
    INT iBookStackTop;
    UINT32 dwBookStackAdr;
}
QuickSortStack;

#ifndef EMULATION_FPGA
/* For Time Consuming Computation */
#define VORBIS_HUFF_DEBUG   2
#endif
#if (VORBIS_HUFF_DEBUG)
UINT32 dwStcCount1;
UINT32 dwStcCount2;
#endif

/***************************************************************************
     Function : fgMakeVorbisCodebook
  Description : Construct Vorbis Codebook (Huffman Tree)
    Parameter : None
    Return    : TRUE/FALSE
***************************************************************************/
BOOL fgMakeVorbisCodebook ()
{
    BOOL result;
    UINT16 wBookNum;
    UINT32 dwBookAdr;
    UINT32 dwBookLen;
    UINT16 iLoopVar;

#ifdef VORBIS_HUFF_DEBUG
    UINT32 dwStcStart;
    UINT32 dwStcEnd;
#endif

    wBookNum = dReadDspDecDram (ADDR_VBSD2RC_DRAM_VORBIS_CODEBOOK_COUNT);
    wBookNum = (wBookNum >> 8) + 1;

    for (iLoopVar = 0; iLoopVar < wBookNum; iLoopVar++)
    {
        dwBookAdr =
            dReadDspDecDram (ADDR_VBSD2RC_DRAM_CODEBOOK_PTR_ADR +
                             iLoopVar * 2);
        dwBookLen =
            dReadDspDecDram (ADDR_VBSD2RC_DRAM_CODEBOOK_PTR_ADR +
                             iLoopVar * 2 + 1);
        dwBookLen = dwBookLen >> 8;
        result = fgMakeHuffmanCode (dwBookAdr, dwBookLen);
        if (result)
        {
            if (dwBookLen)
            {
#if (VORBIS_HUFF_DEBUG==2)
                dwStcCount1 = 0;
                dwStcCount2 = 0;
#endif
                result = fgConstructHuffmanTree (dwBookAdr, dwBookLen);
#if (VORBIS_HUFF_DEBUG==2)
                WriteDspDecDram ((UINT32)
                                 (ADDR_VBSD2RC_DRAM_COMMENT_HEADER + 0 +
                                  iLoopVar), dwStcCount1);
                WriteDspDecDram ((UINT32)
                                 (ADDR_VBSD2RC_DRAM_COMMENT_HEADER + 64 +
                                  iLoopVar), dwStcCount2);
#endif
                if (!result)
                    break;
            }
        }
        else
        {
            break;
        }
    }

    return (result);
}

/***************************************************************************
     Function : fgMakeHuffmanCode
  Description : Make Huffman Codeword
    Parameter : Huffman Data Address, Huffman Data Length
    Return    : TRUE/FALSE
***************************************************************************/
BOOL fgMakeHuffmanCode (UINT32 dwBookAdr, UINT32 dwBookLen)
{
    UINT32 u4Data;
    UINT32 dwEntry;
    UINT32 dwMarker[33];
    UINT32 iLoopVar;
    UINT32 jLoopVar;
    UINT32 dwValueAdr;
    UINT32 dwCodeAdr;
    UINT32 dwCodeLen;
    UINT32 dwCodeCount;

    dwCodeCount = 0;

    /* initial dwMarkder to zero */
    for (iLoopVar = 0; iLoopVar < 33; iLoopVar++)
    {
        *(dwMarker + iLoopVar) = 0;
    }

    /* copy the codebook code length & value to temp DRAM value */
    for (iLoopVar = 0; iLoopVar < dwBookLen; iLoopVar++)
    {
        u4Data = dReadDspDecDram (dwBookAdr + iLoopVar);
#ifdef EMULATION_FPGA
        dwValueList[iLoopVar] = u4Data;
#else // on Target
        WriteDspDecCacheDram ((UINT32) ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_VALUE +
                              iLoopVar, u4Data);
#endif
    }

    dwValueAdr = (UINT32) ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_VALUE;
    dwCodeAdr = (UINT32) ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_CODE;

    for (iLoopVar = 0; iLoopVar < dwBookLen; iLoopVar++)
    {
#ifdef EMULATION_FPGA
        dwCodeLen = dwValueList[iLoopVar];
#else // on Target
        ReadDspDecCacheDram (dwValueAdr + iLoopVar, dwCodeLen);
#endif
        // hi-16 = value, lo-16 = code length
        dwCodeLen = dwCodeLen & 0xffff;
        if (dwCodeLen > 0)
        {
            dwEntry = *(dwMarker + dwCodeLen);
            /* update ourself */
            if (dwCodeLen < 32 && (dwEntry >> dwCodeLen))
            {
                /* error condition; the lengths must specify an overpopulated tree */
                return (FALSE);
            }

            /* bitreverse the words because our bitwise packer/unpacker is LSb
               endian */
#ifdef EMULATION_FPGA
            u4Data = dwEntry << (32 - dwCodeLen);
            dwCodeList[dwCodeCount] = u4Data;
#else // on Target
            WriteDspDecCacheDram (dwCodeAdr + dwCodeCount,
                                  dwEntry << (32 - dwCodeLen));
#endif
            dwCodeCount++;

            /* Look to see if the next shorter marker points to the node
               above. if so, update it and repeat.  */
            for (jLoopVar = dwCodeLen; jLoopVar > 0; jLoopVar--)
            {
                if (*(dwMarker + jLoopVar) & 1)
                {
                    /* have to jump branches */
                    if (jLoopVar == 1)
                        *(dwMarker + 1) += 1;
                    else
                        *(dwMarker + jLoopVar) =
                            *(dwMarker + jLoopVar - 1) << 1;
                    break;      /* invariant says next upper marker would already
                                   have been moved if it was on the same path */
                }
                *(dwMarker + jLoopVar) += 1;
            }

            /* prune the tree; the implicit invariant says all the longer
               markers were dangling from our just-taken node.  Dangle them
               from our *new* node. */
            for (jLoopVar = dwCodeLen + 1; jLoopVar < 33; jLoopVar++)
            {
                if ((*(dwMarker + jLoopVar) >> 1) == dwEntry)
                {
                    dwEntry = *(dwMarker + jLoopVar);
                    *(dwMarker + jLoopVar) = *(dwMarker + jLoopVar - 1) << 1;
                }
                else
                {
                    break;
                }
            }
        }
    }

    return (TRUE);
}

/***************************************************************************
     Function : vWriteHuffTree
  Description : Write the Huffman Tree Data to Specified DRAM
    Parameter : 
    Return    : None
***************************************************************************/
void vWriteHuffTree (UINT32 dwOutAdr, UINT32 * dwOutData, UINT32 dwInData,
                     UINT32 * dwOutIndx)
{
    if ((*dwOutIndx) & 1)       // second word in UINT32
    {
#ifdef EMULATION_FPGA
        *dwOutData = *dwOutData | (dwInData << 16);
        WriteDspDecDram (dwOutAdr + (*dwOutIndx >> 1), *dwOutData);
#else
        WriteDspDecDram (dwOutAdr + (*dwOutIndx >> 1),
                         (*dwOutData | (dwInData << 16)));
#endif
    }
    else                        // first word in UINT32
    {
        *dwOutData = dwInData & 0xffff;
    }
    (*dwOutIndx)++;
}


#define LEFT_TREE   0
#define RIGHT_TREE  1

/***************************************************************************
     Function : fgConstructHuffmanTree
  Description : Huffman Tree Construction
    Parameter : Huffman Data Address, Huffman Data Length
    Return    : TRUE/FALSE
***************************************************************************/
BOOL fgConstructHuffmanTree (UINT32 dwBookAdr, UINT32 dwBookLen)
{
    UINT32 dwTreeIndx;
    UINT32 dwIndx;
    UINT32 jLoopVar;
    UINT8 bBranch;
    UINT32 dwLeaf;
    UINT32 dwMask;
    UINT32 dwTmpCode;
    UINT32 dwCodeIndx;
    UINT32 dwLenNow;
    UINT32 dwCodeNow;
    UINT32 dwValueAdr;
    UINT32 dwCodeAdr;
    UINT32 dwCodeLen;
    UINT32 dwCodeValue;
    UINT32 dwCode;
    UINT32 dwTreeData;
    UINT32 u4Data;

#if (VORBIS_HUFF_DEBUG==2)
    UINT32 dwStcStart;
    UINT32 dwStcEnd;
#endif

    bBranch = LEFT_TREE;
    dwLenNow = 1;
    dwCodeNow = 0;
    dwValueAdr = (UINT32) ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_VALUE;
    dwCodeAdr = (UINT32) ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_CODE;
    if (dwBookLen > 1)
    {

#if (VORBIS_HUFF_DEBUG==2)
        dwStcStart = dPRGetStcS ();
#endif
        // sort the huffman code first
        vVorbisQuickSort (dwValueAdr, dwCodeAdr, dwBookLen);
#if (VORBIS_HUFF_DEBUG==2)
        dwStcEnd = dPRGetStcS ();
        dwStcCount1 += (dwStcEnd - dwStcStart);
#endif

        dwTreeIndx = 0;
        dwIndx = 0;
#ifdef EMULATION_FPGA
        dwCode = dwCodeList[0];
        dwCodeLen = dwValueList[0];
#else // on Target
        ReadDspDecCacheDram (dwCodeAdr + 0, dwCode);
        ReadDspDecCacheDram (dwValueAdr + 0, dwCodeLen);
#endif
        dwCodeLen &= 0xffff;
#if (VORBIS_HUFF_DEBUG==2)
        dwStcStart = dPRGetStcS ();
#endif
        while (dwIndx < dwBookLen)
        {
            if (bBranch == LEFT_TREE)
            {                   // branch to LEFT_TREE
                // check if code match & check if length match
                if ((dwCodeNow == dwCode) && (dwLenNow == dwCodeLen))
                {
                    // match --> leaf
                    bBranch = RIGHT_TREE;
                    dwTmpCode = 1 << (32 - dwLenNow);
                    // huff_tree[tree_index++] = 1 ;            
                    vWriteHuffTree (dwBookAdr, &dwTreeData, (UINT32) 1,
                                    &dwTreeIndx);
                    // huff_tree[tree_index++] = -(ogg_int32_t)huff_value[code_index] ;
#ifdef EMULATION_FPGA
                    dwCodeValue = dwValueList[dwIndx];
#else // on Target
                    ReadDspDecCacheDram (dwValueAdr + dwIndx, dwCodeValue);
#endif
                    dwCodeValue = -(dwCodeValue >> 16) & 0xffff;
                    vWriteHuffTree (dwBookAdr, &dwTreeData, dwCodeValue,
                                    &dwTreeIndx);
                    dwCodeNow += dwTmpCode;
                    dwIndx++;
#ifdef EMULATION_FPGA
                    dwCode = dwCodeList[dwIndx];
                    dwCodeLen = dwValueList[dwIndx];
#else // on Target
                    ReadDspDecCacheDram (dwCodeAdr + dwIndx, dwCode);
                    ReadDspDecCacheDram (dwValueAdr + dwIndx, dwCodeLen);
#endif
                    dwCodeLen &= 0xffff;
                }
                else
                {
                    // internal node --> count the leafs below this node
                    bBranch = LEFT_TREE;
                    dwMask = ((1 << dwLenNow) - 1) << (32 - dwLenNow);
                    dwLeaf = 0;
                    jLoopVar = dwIndx;
                    do
                    {
                        dwLeaf++;
                        jLoopVar++;
#ifdef EMULATION_FPGA
                        u4Data = dwCodeList[jLoopVar];
#else // on Target
                        ReadDspDecCacheDram (dwCodeAdr + jLoopVar, u4Data);
#endif
                    }
                    while ((u4Data & dwMask) == dwCodeNow);
                    //huff_tree[tree_index++] = (leaf<<1)-1 ;
                    vWriteHuffTree (dwBookAdr, &dwTreeData, (dwLeaf << 1) - 1,
                                    &dwTreeIndx);
                    dwLenNow++;
                }
            }
            else
            {                   // branch to RIGHT_TREE
                if ((dwCodeNow == dwCode) && (dwLenNow == dwCodeLen))
                {
                    // match --> leaf
                    bBranch = RIGHT_TREE;   // next branch to right
                    //huff_tree[tree_index++] = -((Vbook[i].dwValLen>>16)&0xffff) ;
#ifdef EMULATION_FPGA
                    dwCodeValue = dwValueList[dwIndx];
#else // on Target
                    ReadDspDecCacheDram (dwValueAdr + dwIndx, dwCodeValue);
#endif
                    dwCodeValue = -(dwCodeValue >> 16) & 0xffff;
                    vWriteHuffTree (dwBookAdr, &dwTreeData, dwCodeValue,
                                    &dwTreeIndx);
                    dwTmpCode = dwCodeNow >> (32 - dwLenNow);
                    while (dwTmpCode & 0x1)
                    {
                        dwTmpCode >>= 1;
                        dwLenNow--;
                    }
                    dwTmpCode++;
                    dwCodeNow = dwTmpCode << (32 - dwLenNow);
                    dwIndx++;
#ifdef EMULATION_FPGA
                    dwCode = dwCodeList[dwIndx];
                    dwCodeLen = dwValueList[dwIndx];
#else // on Target
                    ReadDspDecCacheDram (dwCodeAdr + dwIndx, dwCode);
                    ReadDspDecCacheDram (dwValueAdr + dwIndx, dwCodeLen);
#endif
                    dwCodeLen &= 0xffff;
                }
                else
                {
                    bBranch = LEFT_TREE;
                    dwLenNow++;
                }
            }
        }                       // end of while( dwIndx < dwBookLen )
        if (dwTreeIndx & 0x1)
        {
            // make a UINT32 to write --> hufftree entries = 2*n-1
            vWriteHuffTree (dwBookAdr, &dwTreeData, (UINT32) 0, &dwTreeIndx);
        }
        else
        {
            /* there is a error condition */
            return (FALSE);
        }
    }                           // end of if( dwBookLen > 1 )
    else
    {
        // huff_tree[tree_index] = 0 ;
        vWriteHuffTree (dwBookAdr, &dwTreeData, (UINT32) 0, &dwTreeIndx);
    }
#if (VORBIS_HUFF_DEBUG==2)
    dwStcEnd = dPRGetStcS ();
    dwStcCount2 += (dwStcEnd - dwStcStart);
#endif

    return (TRUE);
}


/***************************************************************************
     Function : vVorbisExchange
  Description : exchange Value & Code data of index i & j
    Parameter : 
    Return    : 
***************************************************************************/
void vVorbisExchange (UINT32 dwValueAdr, UINT32 dwCodeAdr, UINT32 i, UINT32 j)
{
    UINT32 temp;

#ifdef EMULATION_FPGA
    // exchange code
    temp = dwCodeList[i];
    dwCodeList[i] = dwCodeList[j];
    dwCodeList[j] = temp;
    // exchange value
    temp = dwValueList[i];
    dwValueList[i] = dwValueList[j];
    dwValueList[j] = temp;
#else
    HalFlushInvalidateDCache();
    // exchange code
    ReadDspDecCacheDram (dwCodeAdr + i, temp);
    WriteDspDecCacheDram (dwCodeAdr + i, DspDecCacheDramData (dwCodeAdr + j));
    WriteDspDecCacheDram (dwCodeAdr + j, temp);
    // exchange value
    ReadDspDecCacheDram (dwValueAdr + i, temp);
    WriteDspDecCacheDram (dwValueAdr + i,
                          DspDecCacheDramData (dwValueAdr + j));
    WriteDspDecCacheDram (dwValueAdr + j, temp);
#endif
}

/***************************************************************************
     Function : dwVorbisQuickSortPush
  Description : codebook quick sort stack push command
    Parameter : u4Data
    Return    : 
***************************************************************************/
void vVorbisQuickSortPush (QuickSortStack * sStack, UINT32 u4Data)
{
/*  if( sStack->iBookStackTop >= (BOOK_STACK_DEPTH-1) ) {
    // ... overflow
  }
*/
#ifdef EMULATION_FPGA
    dwBookStack[++(sStack->iBookStackTop)] = u4Data;
#else
    (sStack->iBookStackTop)++;
    WriteDspDecCacheDram (sStack->dwBookStackAdr + sStack->iBookStackTop,
                          u4Data);
#endif
}

/***************************************************************************
     Function : dwVorbisQuickSortPop
  Description : codebook quick sort stack pop command
    Parameter : 
    Return    : top of stack
***************************************************************************/
UINT32 dwVorbisQuickSortPop (QuickSortStack * sStack)
{
    UINT32 u4Data;

/*  if( stack->top_ptr < 0 ) {
    // ... underflow
  }
*/
#ifdef EMULATION_FPGA
    u4Data = dwBookStack[(sStack->iBookStackTop)--];
#else
    ReadDspDecCacheDram (sStack->dwBookStackAdr + sStack->iBookStackTop,
                         u4Data);
    (sStack->iBookStackTop)--;
#endif
    return (u4Data);
}

/***************************************************************************
     Function : dwVorbisQuickSortPartition
  Description : quick sort partition operation
    Parameter : codebook value adr, codebook code adr, left, right
    Return    : i
***************************************************************************/
UINT32 dwVorbisQuickSortPartition (UINT32 dwValueAdr, UINT32 dwCodeAdr, UINT32 dwLeft,
                                 UINT32 dwRight)
{
    UINT32 i;
    UINT32 j;
    UINT32 dwKey;
    UINT32 dwCode;
    UINT32 temp;

#ifdef EMULATION_FPGA
    dwKey = dwCodeList[dwRight];
#else // on Target
    ReadDspDecCacheDram (dwCodeAdr + dwRight, dwKey);
#endif
    i = dwLeft;
    j = dwRight - 1;
    HalFlushInvalidateDCache();
    while (1)
    {
#ifdef EMULATION_FPGA
        while (dwCodeList[i++] < dwKey);
        while (dwCodeList[j--] > dwKey);
        i--;
        j++;
#else // on Target
        while (DspDecCacheDramData (dwCodeAdr + i) < dwKey)
        {
            i++;
        }
        while (DspDecCacheDramData (dwCodeAdr + j) > dwKey)
        {
            j--;
        }
#endif
        if (i >= j)
            break;
        vVorbisExchange (dwValueAdr, dwCodeAdr, i, j);
    }
    vVorbisExchange (dwValueAdr, dwCodeAdr, i, dwRight);

    return i;
}

/***************************************************************************
     Function : vVorbisInsertSort
  Description : insert sort for huffman code when data is fewer
    Parameter : codebook value adr, codebook code adr, left, right
    Return    : 
***************************************************************************/
void vVorbisInsertSort (UINT32 dwValueAdr, UINT32 dwCodeAdr, UINT32 dwLeft,
                        UINT32 dwRight)
{
    UINT32 i;
    UINT32 j;
    UINT32 dwCode;
    UINT32 dwValue;
    UINT32 dwTemp;
    HalFlushInvalidateDCache();
    for (i = dwLeft + 1; i <= dwRight; i++)
    {
#ifdef EMULATION_FPGA
        dwCode = dwCodeList[i];
        dwValue = dwValueList[i];
#else // on Target
        ReadDspDecCacheDram (dwCodeAdr + i, dwCode);
        ReadDspDecCacheDram (dwValueAdr + i, dwValue);
#endif
        j = i;
#ifdef EMULATION_FPGA
        while ((j > dwLeft) && (dwCodeList[j - 1] > dwCode))
        {
#else // on Target        
        while ((j > dwLeft)
               && (DspDecCacheDramData (dwCodeAdr + j - 1) > dwCode))
        {
#endif
#ifdef EMULATION_FPGA
            dwCodeList[j] = dwCodeList[j - 1];
            dwValueList[j] = dwValueList[j - 1];
#else
            WriteDspDecCacheDram (dwCodeAdr + j,
                                  DspDecCacheDramData (dwCodeAdr + j - 1));
            WriteDspDecCacheDram (dwValueAdr + j,
                                  DspDecCacheDramData (dwValueAdr + j - 1));
#endif
            j--;
        }
#ifdef EMULATION_FPGA
        dwCodeList[j] = dwCode;
        dwValueList[j] = dwValue;
#else // on Target
        WriteDspDecCacheDram (dwCodeAdr + j, dwCode);
        WriteDspDecCacheDram (dwValueAdr + j, dwValue);
#endif
    }
}

/***************************************************************************
     Function : vVorbisSort3
  Description : sort the 3 data indexed by i, j, k
    Parameter : codebook value adr, codebook code adr, i, j, k
    Return    : 
***************************************************************************/
void vVorbisSort3 (UINT32 dwValueAdr, UINT32 dwCodeAdr, UINT32 i, UINT32 j, UINT32 k)
{
    UINT32 dwCode1;
    UINT32 dwCode2;
    HalFlushInvalidateDCache();
#ifdef EMULATION_FPGA
    if (dwCodeList[i] > dwCodeList[j])
#else // on Target    
    if (DspDecCacheDramData (dwCodeAdr + i) >
        DspDecCacheDramData (dwCodeAdr + j))
#endif
        vVorbisExchange (dwValueAdr, dwCodeAdr, i, j);

#ifdef EMULATION_FPGA
    if (dwCodeList[i] > dwCodeList[k])
#else // on Target
    if (DspDecCacheDramData (dwCodeAdr + i) >
        DspDecCacheDramData (dwCodeAdr + k))
#endif
        vVorbisExchange (dwValueAdr, dwCodeAdr, i, k);

#ifdef EMULATION_FPGA
    if (dwCodeList[j] > dwCodeList[k])
#else // on Target
    if (DspDecCacheDramData (dwCodeAdr + j) >
        DspDecCacheDramData (dwCodeAdr + k))
#endif
        vVorbisExchange (dwValueAdr, dwCodeAdr, j, k);
}

/***************************************************************************
     Function : vVorbisQuickSort
  Description : quick sort for huffman code
    Parameter : codebook value adr, codebook code adr, codebook length
    Return    :
***************************************************************************/
void vVorbisQuickSort (UINT32 dwValueAdr, UINT32 dwCodeAdr, UINT32 dwBookLen)
{
    QuickSortStack sBookStack;
    UINT32 dwLeft;
    UINT32 dwRight;
    UINT32 dwIndex;

    // stack init
    sBookStack.dwBookStackAdr = (UINT32) ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_STACK;
    sBookStack.iBookStackTop = -1;

    dwLeft = 0;
    dwRight = dwBookLen - 1;
    vVorbisQuickSortPush (&sBookStack, dwLeft);
    vVorbisQuickSortPush (&sBookStack, dwRight);
    do
    {
        if ((dwRight - dwLeft) > 15)
        {
            vVorbisSort3 (dwValueAdr, dwCodeAdr, dwLeft,
                          (dwLeft + dwRight) >> 1, dwRight);
            vVorbisExchange (dwValueAdr, dwCodeAdr, (dwLeft + dwRight) >> 1,
                             dwRight - 1);
            dwIndex =
                dwVorbisQuickSortPartition (dwValueAdr, dwCodeAdr, dwLeft,
                                            dwRight - 1);
            if ((dwIndex - dwLeft) > (dwRight - dwIndex))
            {
                vVorbisQuickSortPush (&sBookStack, dwLeft);
                vVorbisQuickSortPush (&sBookStack, dwIndex - 1);
                dwLeft = dwIndex + 1;
            }
            else
            {
                vVorbisQuickSortPush (&sBookStack, dwIndex + 1);
                vVorbisQuickSortPush (&sBookStack, dwRight);
                dwRight = dwIndex - 1;
            }
        }
        else
        {
            vVorbisInsertSort (dwValueAdr, dwCodeAdr, dwLeft, dwRight);
            dwRight = dwVorbisQuickSortPop (&sBookStack);
            dwLeft = dwVorbisQuickSortPop (&sBookStack);
        }
    }
    while (sBookStack.iBookStackTop >= 0);
}
#endif // end of VORBIS_SUPPORT

