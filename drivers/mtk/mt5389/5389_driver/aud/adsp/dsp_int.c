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
/***************  $Revision: #5 $       ****************/
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
#include "aud_dsp_cfg.h"

#include "x_assert.h"

extern UINT8 _au1MWSetSoundModeIdx;

void vDspSetTable (UINT8 u1DecId, UINT32 u4FreqIdx);
void vDspSetSamplingRate(UINT8 u1DecId);

#ifdef CC_AUD_BBE_SUPPORT
static void vSetBbeTable (UINT8 u1DecId);
#endif

#ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
static void vSetCDNotchTable(void);
#endif

#ifdef DSP_SUPPORT_SRS_SSHD //sunman_peq
static void vSetPeqTable(UINT32 u4FreqIdx, BOOL fgFirstDecoder);
#endif
#ifdef DSP_SUPPORT_SRS_PURESOUND 
static void vSetAeqTable(UINT32 u4FreqIdx, BOOL fgFirstDecoder);
#endif


#ifdef DSP_SUPPORT_SRSVIQ
//sunman_viq
static void vSetViqTable(UINT32 u4FreqIdx, BOOL fgFirstDecoder);
extern UINT32 dwDspCalOfst (UINT32 dwOfst);
#endif

static void vSetBassManageTable (UINT8 u1DecId);
static void vSetPostProcTable (UINT8 u1DecId, UINT32 u4FreqIdx);
static void vSetupUpMix (UINT8 u1DecId, UINT32 u4FreqIdx);
static void vSetupDownMix (UINT8 u1DecId, UINT32 u4FreqIdx);
#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
static void vSetABXTable(void);
#endif
static BOOL vSetIECRawDelay(UINT32 u4FreqIdx);

extern void _AUD_DspPostSoundMode(UINT8 u1DecId, UINT8 u1ModeIdx, BOOL fgWithMute);

#ifdef CC_AUD_FIR_SUPPORT
static void vSetFIRTable (UINT8 u1DecId, UINT32 u4FreqIdx);
#endif

//extern void _AUD_DspSetDDCOEnable(BOOL fgEnable);
//extern AUD_IEC_T _GetIECFlag(UINT8 u1DecId);

UINT8 FC_TO_TABLE_INDEX(UINT16 u2Fc)
{
    // Check support range
    u2Fc = (u2Fc > HPF_FC_END) ? HPF_FC_END : u2Fc;
    u2Fc = (u2Fc < HPF_FC_STR) ? HPF_FC_STR : u2Fc;

    // Transfer to table index
    return ((u2Fc - HPF_FC_STR) / HPF_FC_STEP);
}

#ifdef CC_AUD_PEQ_SUPPORT
UINT32 dwGetPEQFsIdx(UINT8 u1Set)
{
    UINT32 u4FsIdx = 0;
    UINT32 u4FsIdx1 = 0;
	UNUSED(u4FsIdx);

    switch (_rDspVars[AUD_DEC_MAIN].dwDspFreq)
    {
    /*
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
		*/
    case SFREQ_48K:
        u4FsIdx=0x15d86;// (1/48000)*(180/45)*(8192*2)
        break;
		/*
    case SFREQ_96K:
        u4FsIdx=96000;
        break;
    case SFREQ_192K:
        u4FsIdx=192000;
        break;
		*/
    default:
        u4FsIdx=0x15d86;
        break;
    }

#if 0
    u4FsIdx1 = u2ReadDspShmWORD(W_PEQ_SET0_BW + 8*u1Set) * u4FsIdx;
    u4FsIdx1 = u4FsIdx1>>16;
    u4FsIdx1 = u4FsIdx1+0x08;
    u4FsIdx1 = u4FsIdx1>>1;
#else
    u4FsIdx1 = u2ReadDspShmWORD(W_PEQ_SET0_BW + 8*u1Set);
    if(u4FsIdx1 > 23000)
    	{
    	u4FsIdx1 = 23000;
    	}

    if(u4FsIdx1 > 2000)
    	{
    	u4FsIdx1 = (u4FsIdx1 - 2000 + 1)/2 +2000;
    	}
#endif
	
    LOG(7, "dwDspFreq = %x\n",_rDspVars[AUD_DEC_MAIN].dwDspFreq);
    LOG(7, "PEQu4FsIdx = %x\n",u4FsIdx1);

    // Check index range
    if (u4FsIdx1 > sizeof(MTK_TAN_TABLE)/4)
    {
        u4FsIdx1 = sizeof(MTK_TAN_TABLE)/4 - 1;
        LOG(7, "Error PEQ BW idx\n");
    }

    return u4FsIdx1;
}

UINT32 dwGetPEQCosIdx(UINT8 u1Set)
{
    UINT32 u4FsIdx = 0;
#ifdef LINUX_TURNKEY_SOLUTION
	UINT32 u4FsIdTemp = 0;
#endif



    u4FsIdx = u2ReadDspShmWORD(W_PEQ_SET0_FC + 8*u1Set);
	if(u4FsIdx > 24000) // Check index range,FC less than 24K
	{
	    u4FsIdx = 24000;
	}
#ifdef LINUX_TURNKEY_SOLUTION
    u4FsIdTemp = u4FsIdx;
    u4FsIdx = (u4FsIdx*87+128)/256;
    u4FsIdTemp = ((u4FsIdTemp*3813)/10000 +128)/256;
    u4FsIdx = u4FsIdx + u4FsIdTemp;
#else
    u4FsIdx = (u4FsIdx/48000.00)*16*1024*256;//16-->(8192/1024*2pi/pi),256-->then right shift 8bit
    u4FsIdx = u4FsIdx +0x80;
    u4FsIdx = u4FsIdx>>8;
#endif
	if(u4FsIdx > 8191)
	{
	    u4FsIdx = 8191;
	}	
	LOG(7, "u4FcIdx cos = %d\n",u4FsIdx);

    return u4FsIdx;
}

void vLoadPEQTable(void)
{
    UINT32 i;
    UINT32 u4Index;
	UINT32 u4DramBase;

	u4DramBase = dReadDspCommDram(ADDR_RC2D_PEQ_BASE);
	
    for (i=0; i<9; i++)
    {
        //UINT32 u4Index;
        u4Index = dwGetPEQFsIdx(i);
        VERIFY(u4Index < sizeof(MTK_TAN_TABLE)/4);
   	    WriteDspCommDram ((u4DramBase + ADDR_PEQ1_BW + i*3), MTK_TAN_TABLE[u4Index] );
		u4Index = dwGetPEQCosIdx(i) ;
		WriteDspCommDram ((u4DramBase + ADDR_PEQ1_FC + i*3), COS_TABLE[u4Index]);
    }
}
#endif

#ifdef CC_AUD_SPEAKER_HEIGHT_MODE
void vLoadSPHTable(void)
{
	 UINT8 u1ModeIdx;
	 UINT32 iLoopVar;
	 UINT32 *pdwTable = NULL;
	 UINT32 u4DramBase;

	u1ModeIdx = uReadDspShmBYTE(B_SPH_MODE);
	u4DramBase = dReadDspCommDram(ADDR_RC2D_SPKHEIGHT_BASE);

    if (u1ModeIdx ==1)
    {
        pdwTable = (UINT32 *)&MTK_SHELF_2DB_TABLE[0];
    }
    else if (u1ModeIdx == 2)
    {
        pdwTable = (UINT32 *)&MTK_SHELF_4DB_TABLE[0];
    }
    else if (u1ModeIdx == 3)
    {
        pdwTable = (UINT32 *)&MTK_SHELF_6DB_TABLE[0];
    }

    if (pdwTable == NULL)
    {
        return;
    }

	for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
	{
		WriteDspCommDram ((u4DramBase + ADDR_SHELF_R_COEF + iLoopVar),
						  pdwTable[iLoopVar]);
		WriteDspCommDram ((u4DramBase + ADDR_SHELF_L_COEF + iLoopVar),
						  pdwTable[iLoopVar]);
	}
}
#endif

#ifdef CC_AUD_FIR_SUPPORT
void vSetFIRTable (UINT8 u1DecId, UINT32 u4FreqIdx)
{
	UINT8 u1ModeIdx;
    UINT32 iLoopVar;
    UINT32 *pdwTable = NULL;;
    UINT32 u4DramAdrIdx;

    UNUSED(u4FreqIdx);

#if 0//def CC_AUD_USE_FLASH_AQ
    Aud_FlashAQQuery(AQ_TYPE_FIR, (void*)FIR_TABLE);
#endif

	u1ModeIdx = uReadDspShmBYTE(B_FIR_ENABLE);
    if (u1ModeIdx ==1)
    {
        pdwTable = (UINT32 *)&FIR_TABLE_1[0];
    }
    else if (u1ModeIdx == 2)
    {
        pdwTable = (UINT32 *)&FIR_TABLE_2[0];
    }
    else if (u1ModeIdx == 3)
    {
        pdwTable = (UINT32 *)&FIR_TABLE_3[0];
    }
		

    if (pdwTable == NULL)
    {
        return;
    }

    if (u1DecId == AUD_DEC_MAIN)
    {
        u4DramAdrIdx = dReadDspCommDram(ADDR_RC2D_FIR_BASE) + ADDR_RC2D_FIR_FILTER_COEFF;
    
        for (iLoopVar = 0; iLoopVar < DSP_FIR_SZ; iLoopVar++)
        {
            WriteDspCommDram ((u4DramAdrIdx + iLoopVar), pdwTable[iLoopVar]);
        }
    }
    else                        // secondary decoder
    {
        /*
        for (iLoopVar = 0; iLoopVar < DSP_FIR_SZ; iLoopVar++)
        {
            WriteDspCommDram (ADDR_RC2D_FIR_FILTER_COEFF +
                              iLoopVar, pdwTable[iLoopVar]);
        }
        */
        // No dec2 now.
    }
    
}
#endif


#ifdef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
void vLoadVirtualSurroundTable(void)
{
	UINT16 u2ModeIdx;
	UINT32 iLoopVar;
	UINT32 *pdwTable;
	UINT32 u4DramBase;

    u2ModeIdx = u2ReadDspShmWORD(W_VSURRFLAG);
	u4DramBase = dReadDspCommDram(ADDR_RC2D_M2S_BASE);

    if (u2ModeIdx == VSS_MTK)
    {
#ifndef DSP_SUPPORT_SRSWOW
#ifndef DSP_SUPPORT_SRSTSXT
        pdwTable = (UINT32 *)&MTK_VSURR_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 20; iLoopVar++)
        {
            WriteDspCommDram ((ADDR_VSURR_IIR_COEF + iLoopVar),
		        pdwTable[iLoopVar]);
        }

        // MTK Vsurr Crosstalk Filter Coefficient
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

#endif //DSP_SUPPORT_SRSTSXT
#endif //DSP_SUPPORT_SRSWOW
	}

	if (u2ModeIdx == VSS_M2S)
	{
	    // mono 2 stereo
		pdwTable = (UINT32 *)&MTK_COMB_R_TABLE[0];
		for (iLoopVar = 0; iLoopVar < 70; iLoopVar++)
		{
			WriteDspCommDram ((u4DramBase + ADDR_M2S_COMB_R_COEF + iLoopVar),
							  pdwTable[iLoopVar]);
		}

		pdwTable = (UINT32 *)&MTK_COMB_L_TABLE[0];
		for (iLoopVar = 0; iLoopVar < 70; iLoopVar++)
		{
			WriteDspCommDram ((u4DramBase + ADDR_M2S_COMB_L_COEF + iLoopVar),
							  pdwTable[iLoopVar]);
		}

		pdwTable = (UINT32 *)&MTK_M2S_ALLPASS_TABLE[0];
		for (iLoopVar = 0; iLoopVar < 20; iLoopVar++)
		{
			WriteDspCommDram ((u4DramBase +ADDR_M2S_ALLPASS_COEF + iLoopVar),
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
void vDspSetSamplingRate(UINT8 u1DecId)
{
    UINT32 u4FreqIdx = SFREQ_48K;
    BOOL fgValidFreq;
    UINT16 u2SpkCfg;
    UINT32 u4DataRate = 0;
    BOOL fgFreqChange = FALSE;

    AUD_DEC_ID_VALIDATE(u1DecId);

    fgValidFreq = TRUE;

#if defined(CC_MT5365) || defined(CC_MT5395) //5368/96 supports 4th decoder
    if (u1DecId != AUD_DEC_4TH)
#endif
    {
        if (_rDspStatus[u1DecId].fgDspAoutMuted)
        {
            if (u1DecId == AUD_DEC_MAIN)
            {
                vDspCmd(UOP_DSP_MASTER_VOLUME);
            }
            else if (u1DecId == AUD_DEC_AUX)
            {
                vDspCmd(UOP_DSP_MASTER_VOLUME_DEC2);
            }        
        #ifdef CC_MT5391_AUD_3_DECODER
            else if (u1DecId == AUD_DEC_THIRD)
            {
                vDspCmd(UOP_DSP_MASTER_VOLUME_DEC3);
            }
        #endif
        #if !defined(CC_MT5365) && !defined(CC_MT5395) //5368/96 supports 4th decoder
            #ifdef CC_AUD_4_DECODER_SUPPORT
            else if (u1DecId == AUD_DEC_4TH)
            {
                vDspCmd(UOP_DSP_MASTER_VOLUME_DEC4);
            }
            #endif
        #endif            

            _rDspStatus[u1DecId].fgDspAoutMuted = FALSE;
        }
    }

    // For mp3 get mpeg type
    // if it is mp3 -> it will go here
    //  we stop dsp here
    // else if it is mp2
    //  we stop dsp at dsp interrupt
    if ((u1DecId == AUD_DEC_MAIN) && (_rDspVars[u1DecId].bDspStrTyp == MPEG3_STREAM))
    {
        if (_rDspStatus[u1DecId].fgDspGetMpgTyp)
        {
            _rDspStatus[u1DecId].fgDspMpgTypGot = TRUE;
            _rDspStatus[u1DecId].fgDspGetMpgTyp = FALSE;
            vDspCmd (DSP_STOP);
            return;
        }
    }

#if 0 ///1///def CC_VORBIS_SUPPORT ///def DSP_VORBIS_SUPPORT   ///[Joel] For middle play
    if(_rDspVars[u1DecId].bDspStrTyp == VORBIS_STREAM)
    {
        AUD_DEC_ID_VALIDATE_2(u1DecId);

        /// _uVorbisMkTbl set ture by Dsp_uop.c command
        if(_uVorbisMkTbl[u1DecId])
        {
            _uVorbisTblDone[u1DecId] = TRUE;
            _uVorbisMkTbl[u1DecId] = FALSE;
            if(u1DecId == AUD_DEC_MAIN)
            {
                vDspCmd(DSP_STOP);
            }
            else if(u1DecId == AUD_DEC_AUX)
            {
                vDspCmd(DSP_STOP_DEC2);
            }
            return;
        }
    }
#endif

    //Extract SFreq
    if (u1DecId == AUD_DEC_MAIN)
    {
        if ((u2ReadDspShmWORD (W_SYSTEM_CTRL) & 0x3) == 0x3)
        {
            // Internal buffer is enabled, and dec2 write internal buffer
            // => dec1 sampling rate is the same as dec2
            u4FreqIdx = u4DspGetSampleRate(AUD_DEC_AUX);
        }
        else
        {
            u4FreqIdx = u4DspGetSampleRate(AUD_DEC_MAIN);
        }
    }
#ifdef CC_MT5391_AUD_3_DECODER
    else if ((u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD))
#else
    else if (u1DecId == AUD_DEC_AUX)
#endif
    {
        if ((u2ReadDspShmWORD (W_SYSTEM_CTRL) & 0x3) == 0x1)
        {
            // Internal buffer is enabled, and dec1 write internal buffer
            // => dec2 sampling rate is the same as dec1
            u4FreqIdx = u4DspGetSampleRate(AUD_DEC_MAIN);
        }
        else
        {
            u4FreqIdx = u4DspGetSampleRate(AUD_DEC_AUX);
        }
    }
#ifdef CC_AUD_4_DECODER_SUPPORT
    else if (u1DecId == AUD_DEC_4TH)
    {
        u4FreqIdx = u4DspGetSampleRate(AUD_DEC_MAIN);
    }
#endif

    // Move the check to AUD_SetSampleFreq() function, since in HDMI mode,
    // driver may modify sampling frequency, so checking _dwDspFreq only is not okay
    if ((u1DecId == AUD_DEC_MAIN) || (u4FreqIdx != _rDspVars[u1DecId].dwDspFreq))
    {
        _rDspVars[u1DecId].bCode3D = CODE3D_NONE;
        _rDspVars[u1DecId].bCode3DUpmix = CODE3D_NONE;
        _rDspVars[u1DecId].dwDspFreq = u4FreqIdx;
        fgValidFreq = TRUE;

        switch (u4FreqIdx)
        {
        case SFREQ_8K:        // 8K
            _rDspVars[u1DecId].dwDacFreq = (UINT32)FS_8K;
            _rDspVars[u1DecId].dwTableFreq = SAMPLE_32K;
            break;
        case SFREQ_16K:        // 16K
            _rDspVars[u1DecId].dwDacFreq = (UINT32)FS_16K;
            _rDspVars[u1DecId].dwTableFreq = SAMPLE_32K;
            break;
        case SFREQ_22K:        // 22K
            _rDspVars[u1DecId].dwDacFreq = (UINT32)FS_22K;
            _rDspVars[u1DecId].dwTableFreq = SAMPLE_32K;
            break;
        case SFREQ_24K:        // 24K
            _rDspVars[u1DecId].dwDacFreq = (UINT32)FS_24K;
            _rDspVars[u1DecId].dwTableFreq = SAMPLE_32K;
            break;
        case SFREQ_32K:        // 32K
            _rDspVars[u1DecId].dwDacFreq = (UINT32)FS_32K;
            _rDspVars[u1DecId].dwTableFreq = SAMPLE_32K;
            break;
        case SFREQ_44K:        // 44K
            _rDspVars[u1DecId].dwDacFreq = (UINT32)FS_44K;
            _rDspVars[u1DecId].dwTableFreq = SAMPLE_44K;
            break;
        case SFREQ_48K:        // 48K
        case SFREQ_52K:        //for line in
		case SFREQ_56K: 	   //for line in
        case SFREQ_62K:
        case SFREQ_64K:
            _rDspVars[u1DecId].dwDacFreq = (UINT32)FS_48K;
            _rDspVars[u1DecId].dwTableFreq = SAMPLE_48K;
            break;
        case SFREQ_88K:        // 88K
            _rDspVars[u1DecId].dwDacFreq = (UINT32)FS_88K;
            _rDspVars[u1DecId].dwTableFreq = SAMPLE_96K;
            break;
        case SFREQ_96K:        // 96K
            _rDspVars[u1DecId].dwDacFreq = (UINT32)FS_96K;
            _rDspVars[u1DecId].dwTableFreq = SAMPLE_96K;
            break;
        case SFREQ_176K:       // 176K
            _rDspVars[u1DecId].dwDacFreq = (UINT32)FS_176K;
            _rDspVars[u1DecId].dwTableFreq = SAMPLE_192K;
            break;
        case SFREQ_192K:       // 192K
            _rDspVars[u1DecId].dwDacFreq = (UINT32)FS_192K;
            _rDspVars[u1DecId].dwTableFreq = SAMPLE_192K;
            break;
        default:
            fgValidFreq = FALSE;
            break;
        }
    }
    
    fgFreqChange = TRUE;

    if (((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX)) && fgValidFreq)            // && _bDspStrTyp != SACD_STREAM)
    {
        vDspSetFreq (u1DecId, _rDspVars[u1DecId].dwDacFreq, fgFreqChange);
        //Work around for DDP sampling rate change test fail issue.
#ifdef CC_MT5391_AUD_3_DECODER        
        if ((u1DecId == AUD_DEC_AUX) && _IsDualDecMode())
#else
        if (u1DecId == AUD_DEC_MAIN)
#endif 
        {
            LOG(5, "====>Set dec0 Asrc input frequency when set dec1 sampling rate in TV source\n");
            _AUD_DspAsrcSetBaseFreq(_rDspVars[u1DecId].dwDacFreq);
        }      
        // IEC RAW delay
        vSetIECRawDelay (_rDspVars[u1DecId].dwDacFreq);
    }

    DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);

    if (u1DecId == AUD_DEC_MAIN)
    {
        u2SpkCfg = wReadDspWORD (ADDR_D2RC_RISC_INFO_INPUT_CH_CFG);
        vTrnsltSpkCfg (u2SpkCfg, &_rDspVars[u1DecId].tSrcSpkCfg);
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        u2SpkCfg = wReadDspWORD (ADDR_D2RC_RISC_INFO_INPUT_CH_CFG_DEC2);
        vTrnsltSpkCfg (u2SpkCfg, &_rDspVars[u1DecId].tSrcSpkCfg);
    }

    // Set DMX interrupt threshold according to bitrate
    if (_rDspVars[u1DecId].bDspStrTyp == MPEG12_STREAM)
    {
        if (u1DecId == AUD_DEC_MAIN)
        {
            u4DataRate = dReadDspCommDram(ADDR_D2RC_MPEG_BITRATE);
        }
        else if (u1DecId == AUD_DEC_AUX)
        {
            u4DataRate = dReadDspCommDram(ADDR_D2RC_MPEG_BITRATE_DEC2);
        }
#ifdef CC_MT5391_AUD_3_DECODER
        else if (u1DecId == AUD_DEC_THIRD)
        {
            u4DataRate = dReadDspCommDram(ADDR_D2RC_MPEG_BITRATE_DEC3);
        }
#endif

        if (u4DataRate >= 256000)
        {
            AUD_DrvDmxIntThreshold(u1DecId, 1);          // Set to 188*1
        }
    }

    if (!(_au1MWSetSoundModeIdx == 0xff))
    {
        _AUD_DspPostSoundMode(AUD_DEC_MAIN, _au1MWSetSoundModeIdx, FALSE); // Mute/Unmute is controlled by AP Start End 
    }    

    if (u1DecId == AUD_DEC_MAIN)
    {
        // move table to DSP working area
        vDspSetTable(AUD_DEC_MAIN, _rDspVars[u1DecId].dwTableFreq);
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        // move table to DSP working area
        vDspSetTable(AUD_DEC_AUX, _rDspVars[u1DecId].dwTableFreq);
    }

    // Load ramcode for PostProcessing
    // Let Dsp continue to play

    vDspSendSample(u1DecId);
}

void vDspUpdatePostEffect(void)
{
    vSetPostProcTable (AUD_DEC_MAIN, _rDspVars[AUD_DEC_MAIN].dwTableFreq);
    vSetupUpMix       (AUD_DEC_MAIN, _rDspVars[AUD_DEC_MAIN].dwTableFreq);
    vSetupDownMix     (AUD_DEC_MAIN, _rDspVars[AUD_DEC_MAIN].dwTableFreq);
#ifdef CC_AUD_BBE_SUPPORT
    vSetBbeTable(AUD_DEC_MAIN);
#endif
#ifdef DSP_SUPPORT_SRS_SSHD //sunman_peq
    vSetPeqTable(2, TRUE);
#endif
#ifdef DSP_SUPPORT_SRS_PURESOUND 
    vSetAeqTable(2, TRUE);
#endif
#ifdef DSP_SUPPORT_SRSVIQ
    vSetViqTable(2, TRUE);//sunman_viq
#endif
    vSetBassManageTable (AUD_DEC_MAIN);
	DspIntOrSram(SRAM_EFFECT_CONTROL,0x01000);
}

/***************************************************************************
     Function :
  Description : Set table ...
    Parameter :
    Return    :
***************************************************************************/
void vDspSetTable (UINT8 u1DecId, UINT32 u4FreqIdx)
{
    AUD_DEC_ID_VALIDATE_2(u1DecId);

    vSetPostProcTable (u1DecId, u4FreqIdx);
    vSetupUpMix       (u1DecId, u4FreqIdx);
    vSetupDownMix     (u1DecId, u4FreqIdx);
#ifdef CC_AUD_BBE_SUPPORT
    vSetBbeTable(u1DecId);
#endif
    vSetBassManageTable(u1DecId);
#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
    vSetABXTable ();
#endif

#ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
	vSetCDNotchTable();
#endif
#ifdef DSP_SUPPORT_SRS_SSHD //sunman_peq
    vSetPeqTable(u4FreqIdx, TRUE);
#endif
#ifdef DSP_SUPPORT_SRS_PURESOUND 
    vSetAeqTable(u4FreqIdx, TRUE);
#endif

#ifdef DSP_SUPPORT_SRSVIQ
    vSetViqTable(u4FreqIdx, TRUE);//sunman_viq
#endif

    //Check if DRC Auto and Set DRC Mode
    if (u1DecId == AUD_DEC_MAIN)
    {
        if (u2ReadDspShmWORD (W_PROCMOD) & 0x8)
        {
            vDspCmd (UOP_DSP_PROCESSING_MODE);
        }
    }
    else
    {
        if (u2ReadDspShmWORD (W_PROCMOD_DEC2) & 0x8)
        {
            vDspCmd (UOP_DSP_PROCESSING_MODE_DEC2);
        }
    }
}

#ifdef CC_AUD_HPF_SUPPORT
void vLoadHPFTable(void)
{
    const UINT32 * pu4TableAddr;
    UINT32   u4FcIndx;
    INT32    i;

    // Find out table address
    u4FcIndx = FC_TO_TABLE_INDEX(u2ReadDspShmWORD(W_HPF_FC));
    // LOG(0, "Fc Idx = %d\n", u4FcIndx);
    pu4TableAddr = &ORDER_6_FILTER_TABLE[u4FcIndx][0];

#if 1
    // Perform loading coefficient table
    for (i = 0; i < HPF_TABLE_SET_SZ; i++)
    {
        WriteDspCommDram ((DRAM_HPF_BUFFER + i), pu4TableAddr[i]);
        WriteDspCommDram ((DRAM_HPF_BUFFER + i + 5), pu4TableAddr[i]);
        WriteDspCommDram ((DRAM_HPF_BUFFER + i + 10), pu4TableAddr[i]);
    }

    // Re-initialize DSP working buffer
    for (i = HPF_TABLE_SET_SZ*3; i < 39; i++)
    {
        WriteDspCommDram (DRAM_HPF_BUFFER + i, 0x00000000);
    }
	

	// Find out table address
    u4FcIndx = FC_TO_TABLE_INDEX(u2ReadDspShmWORD(W_HPF2_FC));
    // LOG(0, "Fc Idx = %d\n", u4FcIndx);
    pu4TableAddr = &ORDER_6_FILTER_TABLE[u4FcIndx][0];

	// Perform loading coefficient table HPF2
    for (i = 0; i < HPF_TABLE_SET_SZ; i++)
    {
        WriteDspCommDram ((DRAM_HPF2_BUFFER + i), pu4TableAddr[i]);
        WriteDspCommDram ((DRAM_HPF2_BUFFER + i + 5), pu4TableAddr[i]);
        WriteDspCommDram ((DRAM_HPF2_BUFFER + i + 10), pu4TableAddr[i]);
    }

    // Re-initialize DSP working buffer HPF2
    for (i = HPF_TABLE_SET_SZ*3; i < 39; i++)
    {
        WriteDspCommDram (DRAM_HPF2_BUFFER + i, 0x00000000);
    }

#else

    /*b2*/ WriteDspCommDram (DRAM_HPF_BUFFER + 0, 0x003f6978);
	/*b1*/ WriteDspCommDram (DRAM_HPF_BUFFER + 1, 0xff812d12);
	/*a2*/ WriteDspCommDram (DRAM_HPF_BUFFER + 2, 0xffc12bac);
	/*a1*/ WriteDspCommDram (DRAM_HPF_BUFFER + 3, 0x007ed18a);
	/*b0*/ WriteDspCommDram (DRAM_HPF_BUFFER + 4, 0x003f6978);

	/*b2*/ WriteDspCommDram (DRAM_HPF_BUFFER + 5, 0x003f6978);
	/*b1*/ WriteDspCommDram (DRAM_HPF_BUFFER + 6, 0xff812d12);
	/*a2*/ WriteDspCommDram (DRAM_HPF_BUFFER + 7, 0xffc12bac);
	/*a1*/ WriteDspCommDram (DRAM_HPF_BUFFER + 8, 0x007ed18a);
	/*b0*/ WriteDspCommDram (DRAM_HPF_BUFFER + 9, 0x003f6978);

	/*b2*/ WriteDspCommDram (DRAM_HPF_BUFFER + 10, 0x003f6978);
	/*b1*/ WriteDspCommDram (DRAM_HPF_BUFFER + 11, 0xff812d12);
	/*a2*/ WriteDspCommDram (DRAM_HPF_BUFFER + 12, 0xffc12bac);
	/*a1*/ WriteDspCommDram (DRAM_HPF_BUFFER + 13, 0x007ed18a);
	/*b0*/ WriteDspCommDram (DRAM_HPF_BUFFER + 14, 0x003f6978);

    // Re-initialize DSP working buffer
    for (i = HPF_TABLE_SET_SZ; i < 39; i++)
    {
        WriteDspCommDram (DRAM_HPF_BUFFER + i, 0x00000000);
    }

#endif

}
#endif

#ifdef CC_AUD_VBASS_SUPPORT
void vDspVBassCutOffCoeff(void)
{
    UINT32 iLoopVar;
    UINT32 *pdwTable;
    UINT8 u1CutOffFreq;
	UINT32 u4DramBase;

    UNUSED(vDspVBassCutOffCoeff);
    
    u1CutOffFreq = AUD_GetVBassCutOff();
	u4DramBase = dReadDspCommDram(ADDR_RC2D_VBASS_BASE);
    LOG(7, "VBASSS: u1CutOffFreq = %d\n", u1CutOffFreq);
    pdwTable = (UINT32 *)&MTK_VBASS_CUTOFF_TABLE[u1CutOffFreq][0];
    for (iLoopVar = 25; iLoopVar < 30; iLoopVar++)
    {
        WriteDspCommDram ((u4DramBase + ADDR_VBASS_R_COEF + iLoopVar), pdwTable[iLoopVar-25]);    
        //LOG(7,"R: %d, 0x%x\n", iLoopVar, pdwTable[iLoopVar-25]);
    }
    pdwTable = (UINT32 *)&MTK_VBASS_CUTOFF_TABLE[u1CutOffFreq][0];
    for (iLoopVar = 25; iLoopVar < 30; iLoopVar++)
    {
        WriteDspCommDram ((u4DramBase + ADDR_VBASS_L_COEF + iLoopVar), pdwTable[iLoopVar-25]);        
        //LOG(7,"L: %d, 0x%x\n", iLoopVar, pdwTable[iLoopVar-25]);        
    }
}
#endif //CC_AUD_VBASS_SUPPORT

void vSetPostProcTable (UINT8 u1DecId, UINT32 u4FreqIdx)
{
    UINT32 iLoopVar;
    UINT32 *pdwTable;
	UINT32 u4DramBase;

	UNUSED(u4DramBase);
    UNUSED(pdwTable);

    if (u1DecId == AUD_DEC_MAIN) // First decoder
    {
        //================ Delay Factor ================
        WriteDspCommDram (ADDR_RC2D_CH_DELAY_FACTOR, DELAY_FACTOR[SAMPLE_48K]);
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

        // 0x1 (2.22), 0x02 (3.21), 0x03 (4.20), 0x04 (5.19)
#ifdef CC_AUD_7BAND_EQUALIZER
        vWriteDspWORD (ADDR_RC2D_EQUALIZER_BAND1_SHIFT_BIT,0x1);
        vWriteDspWORD (ADDR_RC2D_EQUALIZER_BAND2_SHIFT_BIT,0x3);
#else
        vWriteDspWORD (ADDR_RC2D_EQUALIZER_BAND1_SHIFT_BIT,0x1);
        vWriteDspWORD (ADDR_RC2D_EQUALIZER_BAND2_SHIFT_BIT,0x3);
#endif

        // Enable update flag
        vWriteDspWORD(ADDR_RC2D_EQUALIZER_TABLE_CHANGE, 0x1);

        //================ Super Bass Pipe ================
        vEQ2SbassBandGainFlush(AUD_DEC_MAIN);
        // Enable update flag
        vWriteDspWORD(ADDR_RC2D_SUPER_BASS_TABLE_CHANGE, 0x1);

        //================ AVC table ================
#ifndef DSP_SUPPORT_SRSVIQ //sunman_viq
        pdwTable = (UINT32 *)&AVC_PARA_TAB[0];
        for (iLoopVar=0; iLoopVar<38; iLoopVar++)
        {
            WriteDspCommDram ((ADDR_RC2D_AVC_AVERAGE_FACTOR_TABLE + iLoopVar),
                              pdwTable[iLoopVar]);
        }
#endif

#ifdef CC_AUD_SUPPORT_MS10
//================ MXM table ================
        pdwTable = (UINT32 *) &xmxchsclgainstolineartab[0];
        for (iLoopVar = 0; iLoopVar < 16; iLoopVar ++)
        {
            WriteDspCommDram ((ADDR_RC2D_XMX_TABLE + iLoopVar), pdwTable[iLoopVar]);
        }

        pdwTable = (UINT32 *) &xmxpgmscltolineartab[0];
        for (iLoopVar = 0; iLoopVar < 64; iLoopVar ++)
        {
            WriteDspCommDram ((ADDR_RC2D_XMX_TABLE + 16 + iLoopVar), pdwTable[iLoopVar]);
        }     

        pdwTable = (UINT32 *) &xmxgaininterptab[0];
        for (iLoopVar = 0; iLoopVar < 245; iLoopVar ++)
        {
            WriteDspCommDram ((ADDR_RC2D_XMX_TABLE + 16 + 64 + iLoopVar), pdwTable[iLoopVar]);
        }

        pdwTable = (UINT32 *) &xmxpanmean2chlineargaintab[0];
        for (iLoopVar = 0; iLoopVar < 42; iLoopVar ++)
        {
            WriteDspCommDram ((ADDR_RC2D_XMX_TABLE + 16 + 64 + 245 + iLoopVar), pdwTable[iLoopVar]);
        }
#endif

#ifdef CC_AUD_VBASS_SUPPORT
        //================ VBASS ================
        pdwTable = (UINT32 *)&MTK_VBASS_R_TABLE[0];
		u4DramBase = dReadDspCommDram(ADDR_RC2D_VBASS_BASE);
        for (iLoopVar = 0; iLoopVar < 70; iLoopVar++)
        {
            WriteDspCommDram (((u4DramBase+ADDR_VBASS_R_COEF) + iLoopVar),
                              pdwTable[iLoopVar]);
        }

        pdwTable = (UINT32 *)&MTK_VBASS_L_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 70; iLoopVar++)
        {
            WriteDspCommDram (((u4DramBase+ADDR_VBASS_L_COEF) + iLoopVar),
                              pdwTable[iLoopVar]);
        }
		vDspVBassCutOffCoeff(); 

        pdwTable = (UINT32 *)&MTK_VBASS_HIGHPASS_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 20; iLoopVar++)
        {
            WriteDspCommDram (((u4DramBase+ADDR_VBASS_HIGHPASS_COEF) + iLoopVar),
                              pdwTable[iLoopVar]);
        }
#endif
#ifndef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
        //================ Vsurr Surround ================
        if(!AUD_IsSupportSRS())    // SRS TSXT or SRS TSHD
        {
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
                //WriteDspCommDram ((ADDR_VSURR_1STOP_COEF + iLoopVar),pdwTable[iLoopVar]);
            }
             pdwTable = (UINT32 *)&MTK_VSURR_2STOP_TABLE[0];
            for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
            {
                //WriteDspCommDram ((ADDR_VSURR_2STOP_COEF + iLoopVar),pdwTable[iLoopVar]);
            }           
            WriteDspCommDram ((ADDR_VSURR_BASS_COEF + 3),u4ReadDspShmDWRD(D_VSURR_FO)); //0x843c
        }

#else
        vLoadVirtualSurroundTable();
#endif

#ifdef CC_AUD_SPEAKER_HEIGHT
        pdwTable = (UINT32 *)&MTK_SHELF_R_TABLE[0];
		u4DramBase = dReadDspCommDram(ADDR_RC2D_SPKHEIGHT_BASE);
        for (iLoopVar = 0; iLoopVar < 15; iLoopVar++)
        {
            WriteDspCommDram ((u4DramBase + ADDR_SHELF_R_COEF + iLoopVar),
                              pdwTable[iLoopVar]);
        }


        pdwTable = (UINT32 *)&MTK_SHELF_L_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 15; iLoopVar++)
        {
            WriteDspCommDram ((u4DramBase + ADDR_SHELF_L_COEF + iLoopVar),
                              pdwTable[iLoopVar]);
        }

        pdwTable = (UINT32 *)&MTK_ALLPASS1_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 10; iLoopVar++)
        {
            WriteDspCommDram ((u4DramBase + ADDR_ALLPASS1_COEF + iLoopVar),
                              pdwTable[iLoopVar]);
        }

#ifdef CC_AUD_SPEAKER_HEIGHT_MODE
        vLoadSPHTable();
#endif
#endif

#ifdef CC_AUD_FIR_SUPPORT
        vSetFIRTable (AUD_DEC_MAIN, u4FreqIdx);
#endif

#ifdef CC_AUD_HPF_SUPPORT
        // ================ Load HPF table ================
        vLoadHPFTable();
#endif

#ifdef CC_AUD_PEQ_SUPPORT
        // ================ Load PEQ table ================
        
        vLoadPEQTable();

        pdwTable = (UINT32 *)&MTK_SPKCAL_TABLE[0];
		u4DramBase = dReadDspCommDram(ADDR_RC2D_PEQ_BASE);
        for (iLoopVar = 0; iLoopVar < 40; iLoopVar++)
        {
            WriteDspCommDram ((u4DramBase + ADDR_PEQ_COEF + iLoopVar),
                              pdwTable[iLoopVar]);
        }
#endif

#ifdef KARAOKE_SUPPORT
		pdwTable = (UINT32 *) & KEYSHIFT_TABLE[0];
        for (iLoopVar =0; iLoopVar < 132;iLoopVar++)
        {
            WriteDspCommDram ((ADDR_R2CD_KEYSHIFT_TABLE + iLoopVar), pdwTable[iLoopVar]);
        }
#endif

    }
    else // Second decoder
    {
        //================ Delay Factor ================
        WriteDspCommDram (ADDR_RC2D_CH_DELAY_FACTOR_DEC2,
                          DELAY_FACTOR[SAMPLE_48K]);
        //Channel Time Delay
        for (iLoopVar = 0; iLoopVar < 3; iLoopVar++)
        {
            WriteDspCommDram ((ADDR_RC2D_CH_DELAY_C_DEC2 + iLoopVar),
                              u2ReadDspShmWORD ((W_CHDELAY_C + (iLoopVar * 2))));
        }
    }
}

#ifdef CC_AUD_BBE_SUPPORT
void vSetBbeTable(UINT8 u1DecId)
{
    UINT32 iLoopVar;
    UINT32 *pdwTable;
    UINT32 u4DramAdrIdx;

    if (u1DecId == AUD_DEC_MAIN)
    {
        // BBE VIVA 4 bbe_f Coefficients
        pdwTable = (UINT32 *)&BBEV4_BBE_F_TABLE[0];
        u4DramAdrIdx = dReadDspCommDram(ADDR_RC2D_BBE_BASE) + ADDR_BBEV4_BBE_F;
        WriteDspCommDram (u4DramAdrIdx, pdwTable[0]);

        //BBE VIVA 4 Surround Coefficient
        pdwTable = (UINT32 *)&BBEV4_IIR_COEF_TABLE[0];
        u4DramAdrIdx = dReadDspCommDram(ADDR_RC2D_BBE_BASE) + ADDR_BBEV4_IIR_COEF;
        for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
        {
            WriteDspCommDram ((u4DramAdrIdx + iLoopVar), pdwTable[iLoopVar]);
        }
    }
}
#endif // CC_AUD_BBE_SUPPORT

#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY//ABX_SUPPORT
void vSetABXTable(void)
{
    UINT32 i;

    // Setup ABX enable bit
    WriteDspCommDram (ADDR_RC2D_AUDYSSEY_FLAG,
                ((dReadDspCommDram(ADDR_RC2D_AUDYSSEY_FLAG)) & ~ADDR_RC2D_ABX_ENABLE_BIT) |
                ((uReadDspShmBYTE(B_ABX_ENABLE) > 0) ? ADDR_RC2D_ABX_ENABLE_BIT : 0));

    //Default write 48KHz coef.
    for (i=0;i<98;i++)
    {
        WriteDspCommDram (ADDR_RC2D_ABX_IIR_COEF_ADDR_START+i, ABX_COEF_TABLE[uReadDspShmBYTE(B_ABX_FILTER_SET)][i]);
    }

    WriteDspCommDram (ADDR_RC2D_ABX_IIR_COEF_ADDR_START+98, ABX_GAIN_TABLE[uReadDspShmBYTE(B_ABX_DRY_GAIN_INDX)]);
    WriteDspCommDram (ADDR_RC2D_ABX_IIR_COEF_ADDR_START+99, ABX_GAIN_TABLE[uReadDspShmBYTE(B_ABX_WET_GAIN_INDX)]);
}
#endif

#ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
void vSetCDNotchTable(void)
{
    UINT32 *pu32Table;
    UINT32 u32DramAdr;
    UINT8 u1QIdx;

	u32DramAdr = dReadDspCommDram(ADDR_RC2D_CDNOTCH_BASE);
	u1QIdx = uReadDspShmBYTE(B_CDNOTCH_USER_Q);

    // CDNotch alpha_halfH table
    pu32Table = (UINT32 *)&CDNOTCH_ALPHA_HALFH_TABLE[(u1QIdx<<1)];
    WriteDspCommDram((u32DramAdr+ADDR_CDNOTCH_ALPHA_IDX), pu32Table[0]);
    WriteDspCommDram((u32DramAdr+ADDR_CDNOTCH_HALF_H_IDX), pu32Table[1]);
	LOG(7, "set CDNotch table done\n");
}
#endif

#ifdef DSP_SUPPORT_SRS_SSHD
#if 0
void vSetPeqTable (UINT32 u4FreqIdx, BOOL fgFirstDecoder){//sunman_peq

    UINT32 i;
    UINT32 u32DramAdr;

    u4FreqIdx=2;

     u32DramAdr = dReadDspCommDram(ADDR_RC2D_SRS_PEQ_BASE);
     WriteDspCommDram (ADDR_SRS_STUDIO_FLAG, u2ReadDspShmWORD (W_SRS_STUDIO_FLAG));          
     WriteDspCommDram (u32DramAdr+ADDR_SRS_PEQ_INPUT_GAIN, u4ReadDspShmDWRD (D_SRS_PEQ_INPUT_GAIN));   
     WriteDspCommDram (u32DramAdr+ADDR_SRS_PEQ_OUTPUT_GAIN, u4ReadDspShmDWRD (D_SRS_PEQ_OUTPUT_GAIN));   
     WriteDspCommDram (u32DramAdr+ADDR_SRS_PEQ_BANDS_1, u2ReadDspShmWORD (W_SRS_PEQ_BANDS_1));   
     
     // fixed data
     for (i=0;i<80;i++)
     {
         WriteDspCommDram (u32DramAdr+ADDR_SRS_PEQ_COEF + i,  srs_peq[u4FreqIdx][i]);              
     }
     for (i=0;i<10;i++)
     {
         WriteDspCommDram (u32DramAdr+ADDR_SRS_HPF_COEF + i,  srs_hpf[u4FreqIdx][i]);              
     }
     //sunman_lmt
     u32DramAdr = dReadDspCommDram(ADDR_RC2D_SRS_HL_BASE);
     WriteDspCommDram (u32DramAdr+ADDR_SRS_LMT_INPUT_GAIN, u4ReadDspShmDWRD (D_SRS_LMT_INPUT_GAIN));          
     WriteDspCommDram (u32DramAdr+ADDR_SRS_LMT_OUTPUT_GAIN, u4ReadDspShmDWRD (D_SRS_LMT_OUTPUT_GAIN));   
     WriteDspCommDram (u32DramAdr+ADDR_SRS_LMT_BOOST, u4ReadDspShmDWRD (D_SRS_LMT_BOOST));   
     WriteDspCommDram (u32DramAdr+ADDR_SRS_LMT_LVL, u4ReadDspShmDWRD (D_SRS_LMT_LEVEL));   
     WriteDspCommDram (u32DramAdr+ADDR_SRS_LMT_DELAY, u4ReadDspShmDWRD (D_SRS_LMT_DELAY));   
     for (i=0;i<(3+240);i++) 
     {
         WriteDspCommDram (u32DramAdr+ADDR_SRS_LMT_COEF + i,  srs_lmt[i]);              
     }     

}
#else
static UINT8 useCustom = 0;
void vSetPeqTable (UINT32 u4FreqIdx, BOOL fgFirstDecoder){//sunman_peq

    UINT32 i, idx, u4Ptr;
    UINT32 u32DramAdr;
    static INT32  srs_peq_hpf_coef[3][40+10+10+10];
    u4FreqIdx=SAMPLE_48K;

    VERIFY(DRVCUST_OptQuery(eAudioSRSPeqHpfFactory, (UINT32 *) &u4Ptr)==0);
    x_memcpy((VOID *)srs_peq_hpf_coef, (const VOID *)u4Ptr, sizeof(srs_peq_hpf_coef));
   
    u32DramAdr = dReadDspCommDram(ADDR_RC2D_SRS_PEQ_BASE);
    WriteDspCommDram (ADDR_SRS_STUDIO_FLAG, u2ReadDspShmWORD (W_SRS_STUDIO_FLAG));          
    WriteDspCommDram (u32DramAdr+ADDR_SRS_PEQ_INPUT_GAIN, u4ReadDspShmDWRD (D_SRS_PEQ_INPUT_GAIN));   
    WriteDspCommDram (u32DramAdr+ADDR_SRS_PEQ_OUTPUT_GAIN, u4ReadDspShmDWRD (D_SRS_PEQ_OUTPUT_GAIN));   
    WriteDspCommDram (u32DramAdr+ADDR_SRS_PEQ_BANDS_1, u2ReadDspShmWORD (W_SRS_PEQ_BANDS_1));   
     
    // fixed data
    if(useCustom==0)
    {
        useCustom = 1;
        idx = 0;
        for (i=0;i<40;i++)
        {
            WriteDspCommDram (u32DramAdr+ADDR_SRS_PEQ_COEF + i,  srs_peq_hpf_coef[u4FreqIdx][idx++]);  
            //printf("write peq %x -> %x\n",u32DramAdr+ADDR_SRS_PEQ_COEF + i,srs_peq_hpf_coef[u4FreqIdx][idx-1] );
        }
        for (i=0;i<10;i++)
        {
            WriteDspCommDram (u32DramAdr+ADDR_SRS_HPF_COEF + i,  srs_peq_hpf_coef[u4FreqIdx][idx++]);              
           //printf("write 1st hpf %x -> %x\n",u32DramAdr+ADDR_SRS_HPF_COEF + i,srs_peq_hpf_coef[u4FreqIdx][idx-1] );
        }
		for (i=0;i<10;i++)
        {
            WriteDspCommDram (u32DramAdr+ADDR_SRS_HPF2_COEF + i,  srs_peq_hpf_coef[u4FreqIdx][idx++]);              
           //printf("write 1st hpf %x -> %x\n",u32DramAdr+ADDR_SRS_HPF_COEF + i,srs_peq_hpf_coef[u4FreqIdx][idx-1] );
        }
        for (i=0;i<10;i++)
        {
            WriteDspCommDram (u32DramAdr+ADDR_SRS_PEQ_HPF_COEF + i,  srs_peq_hpf_coef[u4FreqIdx][idx++]);              
            //printf("write 2nd hpf %x -> %x\n",u32DramAdr+ADDR_SRS_PEQ_HPF_COEF + i,srs_peq_hpf_coef[u4FreqIdx][idx-1] );
        }
        //sunman_lmt       
        u32DramAdr = dReadDspCommDram(ADDR_RC2D_SRS_HL_BASE);
        for (i=0;i<(3+240);i++) 
        {
            WriteDspCommDram (u32DramAdr+ADDR_SRS_LMT_TABLE + i,  srs_lmt[i]);              
        }  
		for (i=0;i<(256);i++) 
        {
            WriteDspCommDram (u32DramAdr+ADDR_SRS_LMT_LZTBL + i,  srs_lmt_lzTbl[i]<<8);              
        }
    }        
     u32DramAdr = dReadDspCommDram(ADDR_RC2D_SRS_HL_BASE);
    WriteDspCommDram (u32DramAdr+ADDR_SRS_LMT_INPUT_GAIN, u4ReadDspShmDWRD (D_SRS_LMT_INPUT_GAIN));          
    WriteDspCommDram (u32DramAdr+ADDR_SRS_LMT_OUTPUT_GAIN, u4ReadDspShmDWRD (D_SRS_LMT_OUTPUT_GAIN));   
    WriteDspCommDram (u32DramAdr+ADDR_SRS_LMT_BOOST, u4ReadDspShmDWRD (D_SRS_LMT_BOOST));   
    WriteDspCommDram (u32DramAdr+ADDR_SRS_LMT_LVL, u4ReadDspShmDWRD (D_SRS_LMT_LEVEL));   
    WriteDspCommDram (u32DramAdr+ADDR_SRS_LMT_DELAY, u4ReadDspShmDWRD (D_SRS_LMT_DELAY));   
	WriteDspCommDram (u32DramAdr+ADDR_SRS_LMT_COEFF, u4ReadDspShmDWRD (D_SRS_LMT_COEFF));   
    WriteDspCommDram (u32DramAdr+ADDR_SRS_LMT_THRESH, u4ReadDspShmDWRD (D_SRS_LMT_THRESH));  
}
#endif
#endif

#ifdef DSP_SUPPORT_SRS_PURESOUND
static UINT8 useCustom = 0;
void vSetAeqTable (UINT32 u4FreqIdx, BOOL fgFirstDecoder){//sunman_peq

    UINT32 i;
    UINT32 u32DramAdr;
	UINT32 u32AeqSize;

    // fixed data
    if(useCustom==0)
    {
        useCustom = 1;
        //sunman_lmt       
        u32DramAdr = dReadDspCommDram(ADDR_RC2D_SRS_HL_BASE);
        for (i=0;i<(3+240);i++) 
        {
            WriteDspCommDram (u32DramAdr+ADDR_SRS_LMT_TABLE + i,  srs_lmt[i]);              
        }
		for (i=0;i<(256);i++) 
        {
            WriteDspCommDram (u32DramAdr+ADDR_SRS_LMT_LZTBL + i,  srs_lmt_lzTbl[i]<<8);              
        }
				
    }        
    u32DramAdr = dReadDspCommDram(ADDR_RC2D_SRS_HL_BASE);
    WriteDspCommDram (u32DramAdr+ADDR_SRS_LMT_INPUT_GAIN, u4ReadDspShmDWRD (D_SRS_LMT_INPUT_GAIN));          
    WriteDspCommDram (u32DramAdr+ADDR_SRS_LMT_OUTPUT_GAIN, u4ReadDspShmDWRD (D_SRS_LMT_OUTPUT_GAIN));   
    WriteDspCommDram (u32DramAdr+ADDR_SRS_LMT_BOOST, u4ReadDspShmDWRD (D_SRS_LMT_BOOST));   
    WriteDspCommDram (u32DramAdr+ADDR_SRS_LMT_LVL, u4ReadDspShmDWRD (D_SRS_LMT_LEVEL));   
    WriteDspCommDram (u32DramAdr+ADDR_SRS_LMT_DELAY, u4ReadDspShmDWRD (D_SRS_LMT_DELAY));   
	WriteDspCommDram (u32DramAdr+ADDR_SRS_LMT_COEFF, u4ReadDspShmDWRD (D_SRS_LMT_COEFF));   
    WriteDspCommDram (u32DramAdr+ADDR_SRS_LMT_THRESH, u4ReadDspShmDWRD (D_SRS_LMT_THRESH));  

    //bofeng_aeq
	u32DramAdr = dReadDspCommDram(ADDR_RC2D_SRS_AEQ_BASE);
	WriteDspCommDram (ADDR_SRS_PURESOUND_FLAG, u2ReadDspShmWORD (W_SRS_PURESOUND_FLAG)); 
	WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_INPUT_GAIN, u4ReadDspShmDWRD (D_SRS_AEQ_INPUT_GAIN));          
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_OUTPUT_GAIN, u4ReadDspShmDWRD (D_SRS_AEQ_OUTPUT_GAIN)); 
	u32AeqSize = u4ReadDspShmDWRD (D_SRS_AEQ_FILTER_SIZE);
	if(u32AeqSize == 0)
	{
		//WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_FIR_ORDER, (63 << 16) + 1);
		//WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_SECTION, 8 << 16);//4
		//WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_FIR_LASTGAIN, 0x79AC6F80);
		//WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_LASTGAIN, 0x4B6BF580);
		WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_FIR_ORDER, (56 << 16) + 1);
		WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_SECTION, 4 << 16);//4
		WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_FIR_LASTGAIN, 0x6aafd700);
		WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_LASTGAIN, 0x5c447d78);
		
		//aeq iir
		u32DramAdr = dReadDspCommDram(ADDR_RC2D_SRS_AEQ_BASE);
		//for (i=0;i<(40);i++) 
        //{
        //    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_FILTER + i,  srs_aeq[i]);              
        //}
		//aeq fir
		//for (i=40;i<(40+64);i++) 
        //{
        //    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_FIR_FILTER + i - 40,  srs_aeq[i]);              
        //} 
		for (i=0;i<(20);i++) 
        {
            WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_FILTER + i,  srs_aeq[i]);              
        }
		//aeq fir
		for (i=20;i<(20+57);i++) 
        {
            WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_FIR_FILTER + i - 20,  srs_aeq[i]);              
        } 
	
	}
	else if(u32AeqSize == 1)
	{
		WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_FIR_ORDER, (49 << 16) + 2);
		WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_SECTION, 0);
		WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_FIR_LASTGAIN, 0x40000000);
		WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_LASTGAIN, 0x7fffffff);
		//aeq fir
		u32DramAdr = dReadDspCommDram(ADDR_RC2D_SRS_AEQ_BASE);
		//for (i=104;i<(104+50);i++) 
        //{
        //    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_FIR_FILTER + i - 104,  srs_aeq[i]);              
        //}
		for (i=77;i<(77+50);i++) 
        {
            WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_FIR_FILTER + i - 77,  srs_aeq[i]);              
        } 
	
	}
	else if(u32AeqSize == 2)
	{
		WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_FIR_ORDER, 0 + 4);
		WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_SECTION, 3 << 16);
		WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_FIR_LASTGAIN, 0x40000000);
		WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_LASTGAIN, 0x7fffffff);
		//aeq iir
		u32DramAdr = dReadDspCommDram(ADDR_RC2D_SRS_AEQ_BASE);
		//for (i=154;i<(154+15);i++) 
        //{
        //    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_FILTER + i - 154,  srs_aeq[i]);              
        //}
		for (i=127;i<(127+15);i++) 
        {
            WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_FILTER + i - 127,  srs_aeq[i]);              
        }
	
	}
	else
	{
		//WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_FIR_ORDER, (63 << 16) + 1);
		//WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_SECTION, 8 << 16);//4
		//WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_FIR_LASTGAIN, 0x79AC6F80);
		//WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_LASTGAIN, 0x4B6BF580);
		WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_FIR_ORDER, (56 << 16) + 1);
		WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_SECTION, 4 << 16);//4
		WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_FIR_LASTGAIN, 0x6aafd700);
		WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_LASTGAIN, 0x5c447d78);
		//aeq iir
		u32DramAdr = dReadDspCommDram(ADDR_RC2D_SRS_AEQ_BASE);
		//for (i=0;i<(40);i++) 
        //{
        //    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_FILTER + i,  srs_aeq[i]);              
        //}
		//aeq fir
		//for (i=40;i<(40+64);i++) 
        //{
        //    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_FIR_FILTER + i - 40,  srs_aeq[i]);              
        //} 
		for (i=0;i<(20);i++) 
        {
            WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_FILTER + i,  srs_aeq[i]);              
        }
		//aeq fir
		for (i=20;i<(20+57);i++) 
        {
            WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_FIR_FILTER + i - 20,  srs_aeq[i]);              
        } 
	
	}
	
	
}
#endif


#ifdef DSP_SUPPORT_SRSVIQ
//sunman_viq
void vSetViqTable (UINT32 u4FreqIdx, BOOL fgFirstDecoder)
{
    UINT32 dwOfst;
    UINT32 dwSzConst, dwSzSamp;

    UINT32 dwTabAddr = CMPT_VIQ2_BUFFER * 4;  // Byte address
    /*UINT32 dwDspBuf;*/
    UINT32 dwDspCmptBuf;
    UINT32 dwFlashOfst;
    UINT32 dwTmp;
    
    u4FreqIdx=SAMPLE_48K;
    // copy Table to DRAM from flash
    //dwDspCmptBuf = dwGetDSPCSADR (TYPE_AUD_CMM);
    dwDspCmptBuf = dwGetDSPNSADR (TYPE_AUD_CMM);
    dwFlashOfst = u4GetDspImgAddr ();

    dwSzConst = 470; 	//sunman 472 DWORD independent with sampling rate	//sunman_viq
    dwSzSamp = 167 ;
    dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_VIQ_TBL_SA << 2));
    dwOfst = dwDspCalOfst (dwOfst);
    //dwSzAll = u4GetFlashData (dwFlashOfst + (DSPF_VIQ_TBL_SZ << 2));
    // ASSERT(dwSzAll != (dwSzConst+dwSzSamp*3));    
    vFlash2DramDma (dwDspCmptBuf + dwTabAddr, dwOfst, dwSzConst * 4);
    
    u4FreqIdx = SAMPLE_48K;
    dwOfst = dwOfst + dwSzConst*4 + u4FreqIdx*dwSzSamp*4; 	//sunman 96 DWORD data dependent with sampling rate    
    vFlash2DramDma (dwDspCmptBuf + dwTabAddr + dwSzConst * 4, dwOfst, dwSzSamp * 4);      

     WriteDspCommDram (ADDR_RC2D_AVC_REFERENCE_LEVEL, u4ReadDspShmDWRD (D_VIQ_REF_LVL));
     WriteDspCommDram (ADDR_RC2D_AVC_MODE, u4ReadDspShmDWRD (D_VIQ_MODE));   
     WriteDspCommDram (ADDR_RC2D_AVC_MAX_GAIN, u4ReadDspShmDWRD (D_VIQ_MAX_GAIN));
     WriteDspCommDram (ADDR_RC2D_AVC_INPUT_GAIN, u4ReadDspShmDWRD (D_VIQ_INPUT_GAIN));
     WriteDspCommDram (ADDR_RC2D_AVC_OUTPUT_GAIN, u4ReadDspShmDWRD (D_VIQ_OUTPUT_GAIN));
     WriteDspCommDram (ADDR_RC2D_AVC_NOISE_TH, u4ReadDspShmDWRD (D_VIQ_NOISE_TH));

    dwTmp = dReadDspCommDram(ADDR_RC2D_AVC_MODE);
    switch( (dwTmp & 0x3000)>>12 )
    {
    case 0://Light
        //printf("\n\n L dwCoefPnt=%x, dwFacPnt=%x\n\n ", ADDR_SRS_VIQ_COEF1, ADDR_SRS_VIQ_G_FACTOR);
        WriteDspCommDram (ADDR_RC2D_AVC_MAX_GAIN,0x10000 );   
        WriteDspCommDram (ADDR_SRS_VIQ_COEF1, 0x4);  //kSRSVolumeControlHLCoeff1Light
        WriteDspCommDram (ADDR_SRS_VIQ_COEF2, 0x2);   //kSRSVolumeControlHLCoeff2Light 
        WriteDspCommDram (ADDR_SRS_VIQ_COEF3, 0x3);   //kSRSVolumeControlHLCoeff3Light
        WriteDspCommDram (ADDR_SRS_VIQ_G_FACTOR,0x6 );   //gfactor
    	 break;
    case 1://Normal
        //printf("\n\n N dwCoefPnt=%x, dwFacPnt=%x\n\n ", ADDR_SRS_VIQ_COEF1, ADDR_SRS_VIQ_G_FACTOR);
        WriteDspCommDram (ADDR_RC2D_AVC_MAX_GAIN,0x20000 );   
        WriteDspCommDram (ADDR_SRS_VIQ_COEF1, 0x8);   
        WriteDspCommDram (ADDR_SRS_VIQ_COEF2, 0x4); 
        WriteDspCommDram (ADDR_SRS_VIQ_COEF3, 0x6);   
        WriteDspCommDram (ADDR_SRS_VIQ_G_FACTOR, 0x9);   
        break;
    case 2://Heavy
        //printf("\n\n H dwCoefPnt=%x, dwFacPnt=%x\n\n ", ADDR_SRS_VIQ_COEF1, ADDR_SRS_VIQ_G_FACTOR);
        WriteDspCommDram (ADDR_RC2D_AVC_MAX_GAIN, 0x40000);   
        WriteDspCommDram (ADDR_SRS_VIQ_COEF1, 0xe);
        WriteDspCommDram (ADDR_SRS_VIQ_COEF2, 0x7); 
        WriteDspCommDram (ADDR_SRS_VIQ_COEF3, 0xb);   
        WriteDspCommDram (ADDR_SRS_VIQ_G_FACTOR,0xd );   
        break; 
    default :// Set as Normal
        //printf("\n\n N dwCoefPnt=%x, dwFacPnt=%x\n\n ", ADDR_SRS_VIQ_COEF1, ADDR_SRS_VIQ_G_FACTOR);
        WriteDspCommDram (ADDR_RC2D_AVC_MAX_GAIN,0x20000 );   
        WriteDspCommDram (ADDR_SRS_VIQ_COEF1, 0x8);   
        WriteDspCommDram (ADDR_SRS_VIQ_COEF2, 0x4); 
        WriteDspCommDram (ADDR_SRS_VIQ_COEF3, 0x6);   
        WriteDspCommDram (ADDR_SRS_VIQ_G_FACTOR, 0x9);   
        break;    	
    }    
}
#endif


void vSetBassManageTable(UINT8 u1DecId)
{
    UINT32 iLoopVar;
    UINT32 *pdwTable;

    UINT32 iCutFreq;

    iCutFreq = u4ReadDspShmDWRD (D_CUTOFF_FREQ);

    //Bass Management LPF and HPF Coeff.
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
    else if (iCutFreq == 70)
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

    if (u1DecId == AUD_DEC_MAIN)
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

void vSetupUpMix(UINT8 u1DecId, UINT32 u4FreqIdx)
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

    if (u1DecId != AUD_DEC_MAIN)
    {
        return;
    }

    dwFlashOfst = u4GetDspImgAddr ();
#ifndef PROLOGICII_SUPPORT
    vWriteDspWORD (ADDR_RC2D_PRO_LOGIC_II_CONFIG, 0);
#else
    //Prologic II
    //vWriteDspWORD(ADDR_RC2D_PRO_LOGIC_II_CONFIG, u2DspData);
    DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);

    dDspData = wReadDspWORD (ADDR_RC2D_PRO_LOGIC_II_CONFIG);
    //Check if Pro Logic II
    if (dDspData & 0x8001)
    {
        //Prologic II
        vWriteDspWORD (ADDR_RC2D_PRO_LOGIC_II_MODE,
                       u2ReadDspShmWORD (W_PLIIMODE));
        if (_rDspVars[AUD_DEC_MAIN].bCode3DUpmix != CODE3D_PROLOGIC_II)
        {
            //Set _bCode3DUpmix = CODE3D_PROLOGIC_II
            _rDspVars[AUD_DEC_MAIN].bCode3DUpmix = CODE3D_PROLOGIC_II;
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
}

void vSetupDownMix (UINT8 u1DecId, UINT32 u4FreqIdx)
{
#ifndef DOWNMIX_SUPPORT
    //Virtual Surround
    //Be careful of multi-flag on
    //Virtual Surround & Spatializer
    vWriteDspWORD (ADDR_RC2D_VIRTUAL_SURROUND, 0);
#else
    UINT32 iLoopVar;
    UINT32 dwOfst = 0;
    UINT32 dwSz = 0;
    UINT32 dVSCfg;

    // 20030912 modified
    if (u1DecId == AUD_DEC_MAIN)
    {   // fisrt decoder
        DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);

        dVSCfg = wReadDspWORD (ADDR_RC2D_VIRTUAL_SURROUND) & 0xff;
        if (VSS_MTK == dVSCfg)
        {
            if ((_rDspVars[AUD_DEC_MAIN].bCode3D != CODE3D_VIRTUAL_SURROUND))
            {
                _rDspVars[AUD_DEC_MAIN].bCode3D = CODE3D_VIRTUAL_SURROUND;
            }
        }
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
     	    u4TblSrcAdr = (UINT32 *)&SRSTSXT2CH_TBL[u4FSid][0];
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
            if(_rDspVars[u1DecId].bCode3D != CODE3D_SRS_TSXT)
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
                _rDspVars[u1DecId].bCode3D = CODE3D_SRS_TSXT;
            }
        }
#endif //DSP_SUPPORT_SRSTSXT	
        else if ((VSS_SRS_TSXT == (dVSCfg & 0xFFFFFFFE))&&AUD_IsSupportSRS())
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
            
            u4FSid           = u4FreqIdx;
            if (u4FSid > SAMPLE_48K)
            {
                u4FSid = SAMPLE_48K;
            }
		    u4FSid=SAMPLE_48K;

            u4input_gain    = u4ReadDspShmDWRD(D_SRS_TSHD_INPUT_GAIN);      //0x00333333    ;//-8 dB
            u4output_gain  = u4ReadDspShmDWRD(D_SRS_TSHD_OUTPUT_GAIN); //0x007FFFFF    ;//0 dB
            u4tsxt_fg         = u4ReadDspShmDWRD(D_SRS_TSHD_FLAGS);
            u4headphone   = (u4tsxt_fg & 0x00000080) >> 7;
            u4wow_fc_ctrl  = u4ReadDspShmDWRD(D_SRS_TSHD_FC_Elevation);
            u4fc_elev         = u4ReadDspShmDWRD(D_SRS_TSHD_FC_Elevation);
            u4tb_ctrl          = u4ReadDspShmDWRD(D_SRS_TSHD_TB_TBCtrl);
            u4def_ctrl        = u4ReadDspShmDWRD(D_SRS_TSHD_DefCtrl);
            u4limiter_ctrl    = u4ReadDspShmDWRD(D_SRS_TSHD_LimiterCtrl);
            u4spksize        = (UINT32)uReadDspShmBYTE(B_SRS_TSHD_SPKSIZE);
            u4sub_spksize = (UINT32)uReadDspShmBYTE(B_SRS_TSHD_SUBSPKSIZE);
            u4co_freq        = (UINT32)uReadDspShmBYTE(B_SRS_TSHD_COFREQ);
            u4inp_type      = (UINT32)uReadDspShmBYTE(B_SRS_TSHD_INP_TYPE);
            u4ch_dest       = (UINT32)uReadDspShmBYTE(B_SRS_TSHD_CH_DEST);
            u4center_ctrl   = u4ReadDspShmDWRD(D_SRS_TSHD_Center_Ctrl);
            u4space_ctrl    = u4ReadDspShmDWRD(D_SRS_TSHD_Space_Ctrl);
            u4surr_lvl        = u4ReadDspShmDWRD(D_SRS_TSHD_Surr_Level);
            
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 0, u4tsxt_fg << 8);
            WriteDspCommDram (ADDR_SRS_TSHD_DATA +  1,u4fc_elev);         //m_Elevation ; 0x40000
            WriteDspCommDram (ADDR_SRS_TSHD_DATA +  2, 0    << 8);         //ts_mode
            WriteDspCommDram (ADDR_SRS_TSHD_DATA +  3, u4tb_ctrl);          //m_TruBassControl; 0.3  for test (u4tb_ctrl=0.6)
            WriteDspCommDram (ADDR_SRS_TSHD_DATA +  4, u4input_gain);     //m_InputGain; 0x40000 = 0.5
            //user un-adjustable parameters
            WriteDspCommDram (ADDR_SRS_TSHD_DATA +  5, 0x002d1eb8);     //m_RearSpaceControl
            WriteDspCommDram (ADDR_SRS_TSHD_DATA +  6, 0x00090625);     //m_RearCenterControl

   #if (0)
            //TSHD v1.2    
            WriteDspCommDram (ADDR_SRS_TSHD_DATA +  5, 0x00000000);     //m_RearSpaceControl
            WriteDspCommDram (ADDR_SRS_TSHD_DATA +  6, 0x00000000);     //m_RearCenterControl
    #endif
            WriteDspCommDram (ADDR_SRS_TSHD_DATA +  7, u4space_ctrl);     //m_FrontSpaceControl
            WriteDspCommDram (ADDR_SRS_TSHD_DATA +  8, u4center_ctrl);    //m_FrontCenterControl
            WriteDspCommDram (ADDR_SRS_TSHD_DATA +  9, u4def_ctrl   );      //m_definitionControl    0.3 scale 0
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 10, u4surr_lvl);        //SurroundLevel         0.6
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 11, 0x007e2bcf);      //tweak_gain              0.9857119009006160
            //WriteDspCommDram (ADDR_SRS_TSHD_DATA + 12, 0x005ae148);     //ts_bypass_gain          1
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 12, u4output_gain);   //ts_bypass_gain used as output gain
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 13, u4wow_fc_ctrl);   //ts_wow_focus_ctrl   0.4  scale 0
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 14, u4limiter_ctrl);     //ts_limiter_ctrl         0.75
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 15, 0x00400000);      //ts_LowPassMixGain    0.5
            //Constants
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 16, 0x0002d70a);  //ts_IntegrationConstant    0.71     scale 5
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 17, 0x00028f5c);  //ts_FeedbackConstant           0.02 scale 0
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 18, 0x000f3333);  //ts_IntegrationLimit        sacle  5 /* Set integrationLimit = 2 + truBassControl * 6 */
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 19, 0x007fffff);  //kWOWOne
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 20, 0x00200000);  //kWOWLimiterMinAdjustLevel   0.25 scale 0
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 21, 0x0019999a);  //kWOWLimiterLevelRatio         0.2 scale  0
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 22, 0x007fffff);  //kWOWLimiterMaxMultiplier        1  scale 0
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 23, 0x00400000);  //kWOWLimiterLevelMax        1 scale 1
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 24, 0xffd00000);  //kWOWLimiterLevelMin_s_Max    (0.25 -1) scale 1
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 25, 0x00400000);  //kWOWLimiterLevelDefault        1 scale 1
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
            u4TblSrcAdr += 150;// 15*4
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
            
            WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  0, 0x00400000 );    //kPlus12dB    scale 3
            WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  1, 0x005a3d71 );    //kPlus9dB    scale 2
            //WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  2, 0x005a3d71 );    //kPlus3dB    scale 1
            WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  2, 0x0071eb85 );    //kPlus5dB    scale 1
            WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  3, 0x005ae148 );    //kMinus3dB    scale 0
            WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  4, 0x0050a3d7 );    //kMinus4dB    scale 0
            WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  5, 0x0047ae14 );     //kMinus5dB    scale 0
            //WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  7, 0x00400000 );    //kMinus6dB    scale 0
            //WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  8, 0x005ae148 );    //kMinus9dB    scale -1
            WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  6, 0x00090625 );    //kMinus11dB_scale2
            WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  7, 0x00404189 );    //kMinus12dB    scale -1
            WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  8, 0x0050e560 );    //kMinus16dB    scale -2
            WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  9, 0x00408312 );    //kMinus18dB    scale -2
            WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST + 10, 0x00666666 );    //kMinus20dB    scale -3
            WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST + 11, 0x00551eb8 );    //kPoint665    scale  0
            WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST + 12, 0x00570a3d );    //kPoint34        scale -1
            WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST + 13, u4inp_type <<8);    //kPoint665    scale  0
            WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST + 14, u4ch_dest <<8);
            
            if (_rDspVars[AUD_DEC_MAIN].bCode3D != CODE3D_SRS_TSXT)
            {
                _rDspVars[AUD_DEC_MAIN].bCode3D = CODE3D_SRS_TSXT;
            }
        }
    }
    else
    {
        // second decoder
    }

    // Just fix warning message for -O2
    UNUSED(dwOfst);
    UNUSED(dwSz);
    UNUSED(u4FreqIdx);
    UNUSED(iLoopVar);
#endif //DOWNMIX_SUPPORT

}

#if 1///def CC_VORBIS_SUPPORT

/* DSP Dram Space Definition */
/* decoder page 0x0000 ~ 0x0400 */
#define ADDR_VBSD2RC_DRAM_GENERAL_INFO                   0x0000        // 0x8000
    #define ADDR_VBSD2RC_DRAM_VORBIS_CODEBOOK_COUNT      (ADDR_VBSD2RC_DRAM_GENERAL_INFO+0x00)
#define ADDR_VBSD2RC_DRAM_ID_HEADER                      0x0020        // 0x8020
#define ADDR_VBSD2RC_DRAM_SETUP_HEADER                   0x0030        // 0x8030
#define ADDR_VBSD2RC_DRAM_CODEBOOK_PTR_ADR               0x0040        // 0x8040
#define ADDR_VBSD2RC_DRAM_COMMENT_HEADER                 0x0100        // 0x8100
#define ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_STACK            0x0180        // 0x8180 (0x0020)
/// Compact Dram, borrow dram for RISC calculate.
//#define ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_CODE             0x4000        // 0xc000 (0x2000)
//#define ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_VALUE            0x6000        // 0xe000 (0x2000)
/// Use OUTPUT_BUF_ADR dram. (016B00-8000 = EB00)
#define ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_CODE             0xEB00        // 0x16B00 (0x2000)
#define ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_VALUE            0x10B00       // 0x18B00 (0x2000)

/* For Function Declaration */
BOOL fgConstructHuffmanTree(UINT32 dwBookAdr, UINT32 dwBookLen) ;
BOOL fgMakeHuffmanCode(UINT32 dwBookAdr, UINT32 dwBookLen);
void vVorbisQuickSort(UINT32 dwValueAdr, UINT32 dwCodeAdr, UINT32 dwBookLen) ;

/* For Huffman Tree Construction */
#define LEFT_TREE   0
#define RIGHT_TREE  1

/* For Quick Sort Data */
#define BOOK_STACK_DEPTH  32

typedef struct QuickSortStack
{
  INT32 iBookStackTop ;
  UINT32 dwBookStackAdr ;
} QuickSortStack ;

/* For Time Consuming Computation */
#define VORBIS_HUFF_DEBUG   0
#if (VORBIS_HUFF_DEBUG)
UINT32  dwStcCount1, dwStcCount2 ;
#endif    

#define FEW_DRAM_FLUSH
#define DUAL_DEC_DRAM_ACCESS
#ifdef DUAL_DEC_DRAM_ACCESS
UINT32 VorbisReadDecDram(UINT32 u4Addr)
{
    UINT32 u4Data = 0;
    if(_uVorbisIntDec == 1)
    {
        #ifdef FEW_DRAM_FLUSH
        u4Data = DspDecCacheDramData(u4Addr);
        #else
        ReadDspDecCacheDram(u4Addr, u4Data);
        #endif
    }
    else if(_uVorbisIntDec == 2)
    {
        #ifdef FEW_DRAM_FLUSH
        u4Data = DspDec2CacheDramData(u4Addr);
        #else
        ReadDspDec2CacheDram(u4Addr, u4Data);
        #endif
    }
    else
    {
        LOG(0, "Read Dec Dram - _uVorbisIntDec = 0 !!!!!! \n");
        ASSERT(0);
    }
    return (u4Data);
}

VOID VorbisWriteDecDram(UINT32 u4Addr, UINT32 u4Data)
{
    if(_uVorbisIntDec == 1)
    {
        #ifdef FEW_DRAM_FLUSH
        DspDecCacheDramWriteData(u4Addr, u4Data);
        #else
        WriteDspDecCacheDram(u4Addr, u4Data);
        #endif
    }
    else if(_uVorbisIntDec == 2)
    {
        #ifdef FEW_DRAM_FLUSH
        DspDec2CacheDramWriteData(u4Addr, u4Data);
        #else
        WriteDspDec2CacheDram(u4Addr, u4Data);
        #endif
    }
    else
    {
        LOG(0, "Write Dec Dram - _uVorbisIntDec = 0 !!!!!! \n");
        ASSERT(0);
    }
}

///Without DRAM Flush
UINT32 VorbisDecDramData(UINT32 u4Addr)
{
    UINT32 u4Data = 0;
    if(_uVorbisIntDec == 1)
    {
        u4Data = DspDecCacheDramData(u4Addr);
    }
    else if(_uVorbisIntDec == 2)
    {
        u4Data = DspDec2CacheDramData(u4Addr);
    }
    else
    {
        LOG(0, "Read2 Dec Dram Data- _uVorbisIntDec = 0 !!!!!! \n");
        ASSERT(0);
    }
    return (u4Data);
}

#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
///for MT5368/96 use
///Some case dram= 0012 0034
///DRAM read  = 00 001200 (already >>8)
///DRAM write = 0012 0000 (already <<8)
UINT32 VorbisReadDecDram32(UINT32 u4Addr)
{
    UINT32 u4Data = 0;
    if(_uVorbisIntDec == 1)
    {
        #ifdef FEW_DRAM_FLUSH
        u4Data = DspDecCacheDramData32(u4Addr);
        #else
        ReadDspDecCacheDram32(u4Addr, u4Data);
        #endif
    }
    else if(_uVorbisIntDec == 2)
    {
        #ifdef FEW_DRAM_FLUSH
        u4Data = DspDec2CacheDramData32(u4Addr);
        #else
        ReadDspDec2CacheDram32(u4Addr, u4Data);
        #endif
    }
    else
    {
        LOG(0, "Read Dec Dram32 - _uVorbisIntDec = 0 !!!!!! \n");
        ASSERT(0);
    }
    return (u4Data);
}

VOID VorbisWriteDecDram32(UINT32 u4Addr, UINT32 u4Data)
{
    if(_uVorbisIntDec == 1)
    {
        #ifdef FEW_DRAM_FLUSH
        DspDecCacheDramWriteData32(u4Addr, u4Data);
        #else
        WriteDspDecCacheDram32(u4Addr, u4Data);
        #endif
    }
    else if(_uVorbisIntDec == 2)
    {
        #ifdef FEW_DRAM_FLUSH
        DspDec2CacheDramWriteData32(u4Addr, u4Data);
        #else
        WriteDspDec2CacheDram32(u4Addr, u4Data);
        #endif
    }
    else
    {
        LOG(0, "Write Dec Dram32 - _uVorbisIntDec = 0 !!!!!! \n");
        ASSERT(0);
    }
}

///Without DRAM Flush
UINT32 VorbisDecDramData32(UINT32 u4Addr)
{
    UINT32 u4Data = 0;
    if(_uVorbisIntDec == 1)
    {
        u4Data = DspDecCacheDramData32(u4Addr);
    }
    else if(_uVorbisIntDec == 2)
    {
        u4Data = DspDec2CacheDramData32(u4Addr);
    }
    else
    {
        LOG(0, "Read2 Dec Dram32 Data- _uVorbisIntDec = 0 !!!!!! \n");
        ASSERT(0);
    }
    return (u4Data);
}
#endif  ///(defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
#endif  ///DUAL_DEC_DRAM_ACCESS

/***************************************************************************
     Function : fgMakeVorbisCodebook
  Description : Construct Vorbis Codebook (Huffman Tree)
    Parameter : None
    Return    : TRUE/FALSE
***************************************************************************/
BOOL fgMakeVorbisCodebook(void)
{
  BOOL result = FALSE;
  UINT16 wBookNum ;
  UINT32 dwBookAdr ;
  UINT32 dwBookLen ;
  UINT16 iLoopVar ;
#ifdef VORBIS_HUFF_DEBUG  
  UINT32 dwStcStart ;  
  UINT32 dwStcEnd ;
  UNUSED(dwStcStart);
  UNUSED(dwStcEnd);
#endif  

#ifdef FEW_DRAM_FLUSH   ///Before read dram from dsp
 DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_DEC1_DRAM_IDX); 
 DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_DEC2_DRAM_IDX); 
 DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX); 
#endif

#ifdef DUAL_DEC_DRAM_ACCESS
  wBookNum = VorbisReadDecDram((UINT32)ADDR_VBSD2RC_DRAM_VORBIS_CODEBOOK_COUNT);
#else
  ReadDspDecCacheDram((UINT32)ADDR_VBSD2RC_DRAM_VORBIS_CODEBOOK_COUNT, wBookNum) ;
#endif

  wBookNum = (wBookNum >> 8) + 1 ;
  
  for( iLoopVar = 0 ; iLoopVar < wBookNum ; iLoopVar ++ )
  {
    /// Dsp set compact write address and length
    /// Risc make huffman code and write to dram.
#ifdef DUAL_DEC_DRAM_ACCESS
    dwBookAdr = VorbisReadDecDram((UINT32)(ADDR_VBSD2RC_DRAM_CODEBOOK_PTR_ADR+iLoopVar*2)) ;
    dwBookLen = VorbisReadDecDram((UINT32)(ADDR_VBSD2RC_DRAM_CODEBOOK_PTR_ADR+iLoopVar*2+1)) ;
#else
    ReadDspDecCacheDram((UINT32)(ADDR_VBSD2RC_DRAM_CODEBOOK_PTR_ADR+iLoopVar*2), dwBookAdr) ;
    ReadDspDecCacheDram((UINT32)(ADDR_VBSD2RC_DRAM_CODEBOOK_PTR_ADR+iLoopVar*2+1), dwBookLen) ;
#endif
    dwBookLen = dwBookLen >> 8 ;
    result = fgMakeHuffmanCode(dwBookAdr, dwBookLen) ;
    if(result)
    {
      if( dwBookLen )
      {
#if (VORBIS_HUFF_DEBUG==2)
        dwStcCount1 = 0 ;
        dwStcCount2 = 0 ;
#endif
        result = fgConstructHuffmanTree(dwBookAdr, dwBookLen) ;
#if (VORBIS_HUFF_DEBUG==2)
#ifdef DUAL_DEC_DRAM_ACCESS
        VorbisWriteDecDram((UINT32)(ADDR_VBSD2RC_DRAM_COMMENT_HEADER+0+iLoopVar), dwStcCount1) ;
        VorbisWriteDecDram((UINT32)(ADDR_VBSD2RC_DRAM_COMMENT_HEADER+64+iLoopVar), dwStcCount2) ;
#else
        WriteDspDecCacheDram((UINT32)(ADDR_VBSD2RC_DRAM_COMMENT_HEADER+0+iLoopVar), dwStcCount1) ;
        WriteDspDecCacheDram((UINT32)(ADDR_VBSD2RC_DRAM_COMMENT_HEADER+64+iLoopVar), dwStcCount2) ;
#endif
#endif        
        if(!result)
          break ;
      }        
    }
    else
    {
      break ;
    }
  }

#ifdef FEW_DRAM_FLUSH   ///After write dram to dsp
 DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_DEC1_DRAM_IDX); 
 DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_DEC2_DRAM_IDX); 
 DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX); 
#endif

  return(result);
}

/***************************************************************************
     Function : fgMakeHuffmanCode
  Description : Make Huffman Codeword
    Parameter : Huffman Data Address, Huffman Data Length
    Return    : TRUE/FALSE
***************************************************************************/
BOOL fgMakeHuffmanCode(UINT32 dwBookAdr, UINT32 dwBookLen)
{
  UINT32 dwData ;
  UINT32 dwEntry ;
  UINT32 dwMarker[33] ;
  UINT32 iLoopVar, jLoopVar ;
  UINT32 dwValueAdr ;
  UINT32 dwCodeAdr ;
  UINT32 dwCodeLen ;
  UINT32 dwCodeCount ;
  dwCodeCount = 0 ;
  
  /* initial dwMarkder to zero, clean dwMarker[33]=0 */
  for( iLoopVar = 0 ; iLoopVar < 33 ; iLoopVar ++ )
  {
    *(dwMarker+iLoopVar) = 0 ;
  }
  
  /* copy the codebook code length & value to temp DRAM value */
  for( iLoopVar = 0 ; iLoopVar < dwBookLen ; iLoopVar ++ )
  {
#ifdef DUAL_DEC_DRAM_ACCESS
    #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
    dwData = VorbisReadDecDram32(dwBookAdr+iLoopVar) ;
    VorbisWriteDecDram32((UINT32)ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_VALUE+iLoopVar, dwData) ;
    #else
    dwData = VorbisReadDecDram(dwBookAdr+iLoopVar) ;
    VorbisWriteDecDram((UINT32)ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_VALUE+iLoopVar, dwData) ;
    #endif
#else
    ReadDspDecCacheDram(dwBookAdr+iLoopVar, dwData) ;
    WriteDspDecCacheDram((UINT32)ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_VALUE+iLoopVar, dwData) ;
#endif
  }
  
  dwValueAdr = (UINT32)ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_VALUE ;
  dwCodeAdr = (UINT32)ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_CODE ;
  
  for( iLoopVar = 0 ; iLoopVar < dwBookLen ; iLoopVar ++ )
  {
#ifdef DUAL_DEC_DRAM_ACCESS
    #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
    dwCodeLen = VorbisReadDecDram32(dwValueAdr+iLoopVar) ;
    #else
    dwCodeLen = VorbisReadDecDram(dwValueAdr+iLoopVar) ;
    #endif
#else
    ReadDspDecCacheDram(dwValueAdr+iLoopVar, dwCodeLen) ;
#endif
    // hi-16 = value, lo-16 = code length
    dwCodeLen = dwCodeLen & 0xffff ;
    if(( dwCodeLen > 0 ) && ( dwCodeLen < 33 ))
    {
        dwEntry = *(dwMarker+dwCodeLen) ;
        /* update ourself */
        if( dwCodeLen < 32 && (dwEntry >> dwCodeLen) )
        {
            /* error condition; the lengths must specify an overpopulated tree */
            return(FALSE);
        }

        /* bitreverse the words because our bitwise packer/unpacker is LSb
           endian */
#ifdef DUAL_DEC_DRAM_ACCESS
        #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
        VorbisWriteDecDram32(dwCodeAdr+dwCodeCount, dwEntry << (32-dwCodeLen)) ;
        #else
        VorbisWriteDecDram(dwCodeAdr+dwCodeCount, dwEntry << (32-dwCodeLen)) ;
        #endif
#else
        WriteDspDecCacheDram(dwCodeAdr+dwCodeCount, dwEntry << (32-dwCodeLen)) ;
#endif
        dwCodeCount ++ ;
    
        /* Look to see if the next shorter marker points to the node
	    above. if so, update it and repeat.  */
        for( jLoopVar = dwCodeLen ; jLoopVar > 0 ; jLoopVar -- )
        {
            if( *(dwMarker+jLoopVar) & 1 )
            {
	            /* have to jump branches */
                if( jLoopVar == 1 )
                    *(dwMarker+1) += 1 ;
  	            else
                    *(dwMarker+jLoopVar) = *(dwMarker+jLoopVar-1) << 1 ;
	            break; /* invariant says next upper marker would already
                          have been moved if it was on the same path */
            }
	        *(dwMarker+jLoopVar) += 1 ;
	    }
      
        /* prune the tree; the implicit invariant says all the longer
           markers were dangling from our just-taken node.  Dangle them
           from our *new* node. */
        for( jLoopVar = dwCodeLen + 1 ; jLoopVar < 33 ; jLoopVar ++ )
        {
    	    if( (*(dwMarker+jLoopVar) >> 1) == dwEntry)
    	    {
    	        dwEntry = *(dwMarker+jLoopVar);
    	        *(dwMarker+jLoopVar) = *(dwMarker+jLoopVar-1) << 1 ;
    	    }
    	    else
    	    {
    	        break;
    	    }	
        }    
    }
  }

  return(TRUE);  	
}

/***************************************************************************
     Function : vWriteHuffTree
  Description : Write the Huffman Tree Data to Specified DRAM
    Parameter : 
    Return    : None
***************************************************************************/
void vWriteHuffTree(UINT32 dwOutAdr, UINT32 *dwOutData, UINT32 dwInData, UINT32 *dwOutIndx)
{
  if( (*dwOutIndx)&1 )                          // second word in UINT32
  {
#ifdef DUAL_DEC_DRAM_ACCESS
    #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
    VorbisWriteDecDram32(dwOutAdr+(*dwOutIndx>>1), (*dwOutData | ( dwInData << 16))) ;
    #else
    VorbisWriteDecDram(dwOutAdr+(*dwOutIndx>>1), (*dwOutData | ( dwInData << 16))) ;
    #endif
#else
    WriteDspDecCacheDram(dwOutAdr+(*dwOutIndx>>1), (*dwOutData | ( dwInData << 16))) ;
#endif
  }
  else                                          // first word in UINT32
  {
    *dwOutData = dwInData & 0xffff ;
  }
  (*dwOutIndx) ++ ;
}

#define LEFT_TREE   0
#define RIGHT_TREE  1

/***************************************************************************
     Function : fgConstructHuffmanTree
  Description : Huffman Tree Construction
    Parameter : Huffman Data Address, Huffman Data Length
    Return    : TRUE/FALSE
***************************************************************************/
BOOL fgConstructHuffmanTree(UINT32 dwBookAdr, UINT32 dwBookLen)
{
  UINT32 dwTreeIndx=0, dwIndx ;
  UINT32 jLoopVar ;
  UINT8 bBranch ;
  UINT32 dwLeaf, dwMask, dwTmpCode; ///[Joel] no use, dwCodeIndx ;
  UINT32 dwLenNow, dwCodeNow ;
  UINT32 dwValueAdr, dwCodeAdr  ;
  UINT32 dwCodeLen, dwCodeValue, dwCode ;
  UINT32 dwTreeData, dwData ;
#if (VORBIS_HUFF_DEBUG==2)
  UINT32  dwStcStart, dwStcEnd ;
#endif    
 
  bBranch = LEFT_TREE ;
  dwLenNow = 1 ;
  dwCodeNow = 0 ;  
  dwValueAdr = (UINT32)ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_VALUE ;
  dwCodeAdr = (UINT32)ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_CODE ;
  if( dwBookLen > 1 ) 
  {

#if (VORBIS_HUFF_DEBUG==2)
    dwStcStart = dPRGetStcS() ;
#endif      	
    // sort the huffman code first
    vVorbisQuickSort(dwValueAdr, dwCodeAdr, dwBookLen) ;
#if (VORBIS_HUFF_DEBUG==2)
    dwStcEnd = dPRGetStcS() ;
    dwStcCount1 += (dwStcEnd-dwStcStart) ;
#endif      	

    dwTreeIndx = 0 ;
    dwIndx = 0 ;
#ifdef DUAL_DEC_DRAM_ACCESS
    #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
    dwCode = VorbisReadDecDram32(dwCodeAdr+0) ;
    dwCodeLen = VorbisReadDecDram32(dwValueAdr+0) ;
    #else
    dwCode = VorbisReadDecDram(dwCodeAdr+0) ;            
    dwCodeLen = VorbisReadDecDram(dwValueAdr+0) ;     
    #endif
#else
    ReadDspDecCacheDram(dwCodeAdr+0, dwCode) ;            
    ReadDspDecCacheDram(dwValueAdr+0, dwCodeLen) ;            
#endif
    dwCodeLen &= 0xffff ;
#if (VORBIS_HUFF_DEBUG==2)
    dwStcStart = dPRGetStcS() ;
#endif      	
    while( dwIndx < dwBookLen ) 
    {
      if( bBranch == LEFT_TREE ) 
      {   // branch to LEFT_TREE
        // check if code match & check if length match
        if( (dwCodeNow == dwCode) && (dwLenNow == dwCodeLen) ) 
        {            
     	  // match --> leaf
      	  bBranch = RIGHT_TREE ;
      	  dwTmpCode = 1 << (32-dwLenNow) ;
          // huff_tree[tree_index++] = 1 ;            
          vWriteHuffTree(dwBookAdr, &dwTreeData, (UINT32)1, &dwTreeIndx) ;         
          // huff_tree[tree_index++] = -(ogg_int32_t)huff_value[code_index] ;
    #ifdef DUAL_DEC_DRAM_ACCESS
          #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
          dwCodeValue = VorbisReadDecDram32(dwValueAdr+dwIndx) ;
          #else
          dwCodeValue = VorbisReadDecDram(dwValueAdr+dwIndx) ; 
          #endif
    #else
          ReadDspDecCacheDram(dwValueAdr+dwIndx, dwCodeValue) ;            
    #endif
          dwCodeValue = -(dwCodeValue >> 16) & 0xffff ;
          vWriteHuffTree(dwBookAdr, &dwTreeData, dwCodeValue, &dwTreeIndx) ;
          dwCodeNow += dwTmpCode ;
          dwIndx ++ ;
    #ifdef DUAL_DEC_DRAM_ACCESS
          #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
          dwCode = VorbisReadDecDram32(dwCodeAdr+dwIndx) ;            
          dwCodeLen = VorbisReadDecDram32(dwValueAdr+dwIndx) ; 
          #else
          dwCode = VorbisReadDecDram(dwCodeAdr+dwIndx) ;            
          dwCodeLen = VorbisReadDecDram(dwValueAdr+dwIndx) ;   
          #endif
    #else
          ReadDspDecCacheDram(dwCodeAdr+dwIndx, dwCode) ;            
          ReadDspDecCacheDram(dwValueAdr+dwIndx, dwCodeLen) ;            
    #endif
          dwCodeLen &= 0xffff ;
        } 
        else 
        {
          // internal node --> count the leafs below this node
          bBranch = LEFT_TREE ;
          dwMask = ((1<<dwLenNow) - 1) << (32-dwLenNow) ;
      	  dwLeaf = 0 ;      	  
      	  jLoopVar = dwIndx ;
          do 
          {
            dwLeaf ++ ;
            jLoopVar ++ ;
    #ifdef DUAL_DEC_DRAM_ACCESS
            #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
            dwData = VorbisReadDecDram32(dwCodeAdr+jLoopVar) ;
            #else
            dwData = VorbisReadDecDram(dwCodeAdr+jLoopVar) ;
            #endif
    #else
            ReadDspDecCacheDram(dwCodeAdr+jLoopVar, dwData) ;
    #endif
          } while( (dwData&dwMask) == dwCodeNow ) ;
          //huff_tree[tree_index++] = (leaf<<1)-1 ;
          vWriteHuffTree(dwBookAdr, &dwTreeData, (dwLeaf<<1)-1, &dwTreeIndx) ;
          dwLenNow ++ ;
        }
      }
      else  ///RIGHT_TREE
      {                  // branch to RIGHT_TREE
        if( (dwCodeNow == dwCode) && (dwLenNow == dwCodeLen) ) 
        {            
    	  // match --> leaf
          bBranch = RIGHT_TREE ;			// next branch to right
          //huff_tree[tree_index++] = -((Vbook[i].dwValLen>>16)&0xffff) ;
    #ifdef DUAL_DEC_DRAM_ACCESS
          #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
          dwCodeValue = VorbisReadDecDram32(dwValueAdr+dwIndx) ;
          #else
          dwCodeValue = VorbisReadDecDram(dwValueAdr+dwIndx) ;
          #endif
    #else
          ReadDspDecCacheDram(dwValueAdr+dwIndx, dwCodeValue) ;            
    #endif
          dwCodeValue = -(dwCodeValue >> 16) & 0xffff ;
          vWriteHuffTree(dwBookAdr, &dwTreeData, dwCodeValue, &dwTreeIndx) ;
          dwTmpCode = dwCodeNow >> (32-dwLenNow) ;
          while( dwTmpCode & 0x1 )
          {
            dwTmpCode >>= 1 ;
            dwLenNow -- ;
          }
          dwTmpCode ++ ;
          dwCodeNow = dwTmpCode << (32-dwLenNow) ;
          dwIndx ++ ;
    #ifdef DUAL_DEC_DRAM_ACCESS
          #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
          dwCode = VorbisReadDecDram32(dwCodeAdr+dwIndx) ;            
          dwCodeLen = VorbisReadDecDram32(dwValueAdr+dwIndx) ;    
          #else
          dwCode = VorbisReadDecDram(dwCodeAdr+dwIndx) ;            
          dwCodeLen = VorbisReadDecDram(dwValueAdr+dwIndx) ;       
          #endif
    #else
          ReadDspDecCacheDram(dwCodeAdr+dwIndx, dwCode) ;            
          ReadDspDecCacheDram(dwValueAdr+dwIndx, dwCodeLen) ;            
    #endif
          dwCodeLen &= 0xffff ;
        } 
        else
        {
          bBranch = LEFT_TREE ;
          dwLenNow ++ ;
        }
      }
    } // end of while( dwIndx < dwBookLen )
    if( dwTreeIndx & 0x1 )
    {
      // make a UINT32 to write --> hufftree entries = 2*n-1
      vWriteHuffTree(dwBookAdr, &dwTreeData, (UINT32)0, &dwTreeIndx) ;      
    }
    else
    {
      /* there is a error condition */
      return(FALSE) ;
    }
  }   // end of if( dwBookLen > 1 )
  else
  {
    // huff_tree[tree_index] = 0 ;
    vWriteHuffTree(dwBookAdr, &dwTreeData, (UINT32)0, &dwTreeIndx) ;      
  }
#if (VORBIS_HUFF_DEBUG==2)
    dwStcEnd = dPRGetStcS() ;
    dwStcCount2 += (dwStcEnd-dwStcStart) ;
#endif      	
	
  return(TRUE) ;
}   


/***************************************************************************
     Function : vVorbisExchange
  Description : exchange Value & Code data of index i & j
    Parameter : 
    Return    : 
***************************************************************************/
void vVorbisExchange(UINT32 dwValueAdr, UINT32 dwCodeAdr, UINT32 i, UINT32 j)
{
    UINT32 temp ;
#ifdef DUAL_DEC_DRAM_ACCESS    
    #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
    // exchange code
    temp = VorbisReadDecDram32(dwCodeAdr+i) ;
    VorbisWriteDecDram32(dwCodeAdr+i, VorbisDecDramData32(dwCodeAdr + j)) ;
    VorbisWriteDecDram32(dwCodeAdr+j, temp) ;
    // exchange value
    temp = VorbisReadDecDram32(dwValueAdr+i) ;
    VorbisWriteDecDram32(dwValueAdr+i, VorbisDecDramData32(dwValueAdr + j)) ;
    VorbisWriteDecDram32(dwValueAdr+j, temp) ;
    #else
    // exchange code
    temp = VorbisReadDecDram(dwCodeAdr+i) ;
    VorbisWriteDecDram(dwCodeAdr+i, VorbisDecDramData(dwCodeAdr + j)) ;
    VorbisWriteDecDram(dwCodeAdr+j, temp) ;
    // exchange value
    temp = VorbisReadDecDram(dwValueAdr+i) ;
    VorbisWriteDecDram(dwValueAdr+i, VorbisDecDramData(dwValueAdr + j)) ;
    VorbisWriteDecDram(dwValueAdr+j, temp) ;
    #endif
#else
    // exchange code
    ReadDspDecCacheDram(dwCodeAdr+i, temp) ;
    WriteDspDecCacheDram(dwCodeAdr+i, DspDecCacheDramData(dwCodeAdr + j)) ;
    WriteDspDecCacheDram(dwCodeAdr+j, temp) ;
    // exchange value
    ReadDspDecCacheDram(dwValueAdr+i, temp) ;
    WriteDspDecCacheDram(dwValueAdr+i, DspDecCacheDramData(dwValueAdr + j)) ;
    WriteDspDecCacheDram(dwValueAdr+j, temp) ;
#endif
}

/***************************************************************************
     Function : dwVorbisQuickSortPush
  Description : codebook quick sort stack push command
    Parameter : dwData
    Return    : 
***************************************************************************/
void vVorbisQuickSortPush( QuickSortStack *sStack, UINT32 dwData )
{
/*  if( sStack->iBookStackTop >= (BOOK_STACK_DEPTH-1) ) {
    // ... overflow
  }
*/  
  (sStack->iBookStackTop)++ ;
#ifdef DUAL_DEC_DRAM_ACCESS    
  VorbisWriteDecDram(sStack->dwBookStackAdr+sStack->iBookStackTop, dwData) ;
#else
  WriteDspDecCacheDram(sStack->dwBookStackAdr+sStack->iBookStackTop, dwData) ;
#endif
}

/***************************************************************************
     Function : dwVorbisQuickSortPop
  Description : codebook quick sort stack pop command
    Parameter : 
    Return    : top of stack
***************************************************************************/
UINT32 dwVorbisQuickSortPop( QuickSortStack *sStack )
{
  UINT32 dwData ;
  
/*  if( stack->top_ptr < 0 ) {
    // ... underflow
  }
*/  
#ifdef DUAL_DEC_DRAM_ACCESS    
  dwData = VorbisReadDecDram(sStack->dwBookStackAdr+sStack->iBookStackTop) ;
#else
  ReadDspDecCacheDram(sStack->dwBookStackAdr+sStack->iBookStackTop, dwData) ;
#endif
  (sStack->iBookStackTop)-- ;
  return( dwData ) ;  
}

/***************************************************************************
     Function : dwVorbisQuickSortPartition
  Description : quick sort partition operation
    Parameter : codebook value adr, codebook code adr, left, right
    Return    : i
***************************************************************************/
UINT32 dwVorbisQuickSortPartition(UINT32 dwValueAdr, UINT32 dwCodeAdr, UINT32 dwLeft, UINT32 dwRight)
{
  UINT32 i, j ;
  UINT32 dwKey; ///[Joel]no use, dwCode ;
                ///[Joel]no use,UINT32 temp ;

#ifdef DUAL_DEC_DRAM_ACCESS    
    #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
    dwKey = VorbisReadDecDram32(dwCodeAdr+dwRight) ;
    #else
    dwKey = VorbisReadDecDram(dwCodeAdr+dwRight) ;
    #endif
#else
  ReadDspDecCacheDram(dwCodeAdr+dwRight, dwKey) ;            
#endif
  i = dwLeft ;
  j = dwRight-1 ;
  while( 1 ) 
  {
#ifdef DUAL_DEC_DRAM_ACCESS   
    #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
    while( VorbisDecDramData32(dwCodeAdr + i) < dwKey )
    {
        i ++ ;
    }
    while( VorbisDecDramData32(dwCodeAdr + j) > dwKey )
    {
        j -- ;
    }
    #else
    while( VorbisDecDramData(dwCodeAdr + i) < dwKey )
    {
        i ++ ;
    }
    while( VorbisDecDramData(dwCodeAdr + j) > dwKey )
    {
        j -- ;
    }
    #endif
#else
    while( DspDecCacheDramData(dwCodeAdr + i) < dwKey ) {
      i ++ ;
    }
    while( DspDecCacheDramData(dwCodeAdr + j) > dwKey ) {
      j -- ;
    }
#endif
    if( i >= j )
      break ;     
    vVorbisExchange(dwValueAdr, dwCodeAdr, i, j) ;
  }
  vVorbisExchange(dwValueAdr, dwCodeAdr, i, dwRight) ;
  
  return i ;
}

/***************************************************************************
     Function : vVorbisInsertSort
  Description : insert sort for huffman code when data is fewer
    Parameter : codebook value adr, codebook code adr, left, right
    Return    : 
***************************************************************************/
void vVorbisInsertSort(UINT32 dwValueAdr, UINT32 dwCodeAdr, UINT32 dwLeft, UINT32 dwRight)
{
  UINT32 i, j, dwCode, dwValue; ///[Joel]no use, dwTemp ;
  
  for( i = dwLeft+1 ; i <= dwRight ; i ++ ) 
  {
#ifdef DUAL_DEC_DRAM_ACCESS    
    #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
    dwCode = VorbisReadDecDram32(dwCodeAdr+i) ;            
    dwValue = VorbisReadDecDram32(dwValueAdr+i) ; 
    #else
    dwCode = VorbisReadDecDram(dwCodeAdr+i) ;            
    dwValue = VorbisReadDecDram(dwValueAdr+i) ; 
    #endif
#else
    ReadDspDecCacheDram(dwCodeAdr+i, dwCode) ;            
    ReadDspDecCacheDram(dwValueAdr+i, dwValue) ;            
#endif
    j = i ;

#ifdef DUAL_DEC_DRAM_ACCESS    
    #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
    while( (j > dwLeft) && (VorbisDecDramData32(dwCodeAdr + j-1) > dwCode) ) 
    { 
        VorbisWriteDecDram32(dwCodeAdr+j, VorbisDecDramData32(dwCodeAdr + j-1)) ;
        VorbisWriteDecDram32(dwValueAdr+j, VorbisDecDramData32(dwValueAdr + j-1)) ;
        j -- ;
    }
    #else
    while( (j > dwLeft) && (VorbisDecDramData(dwCodeAdr + j-1) > dwCode) ) 
    { 
        VorbisWriteDecDram(dwCodeAdr+j, VorbisDecDramData(dwCodeAdr + j-1)) ;
        VorbisWriteDecDram(dwValueAdr+j, VorbisDecDramData(dwValueAdr + j-1)) ;
        j -- ;
    }
    #endif
#else
    while( (j > dwLeft) && (DspDecCacheDramData(dwCodeAdr + j-1) > dwCode) ) 
    { 
        WriteDspDecCacheDram(dwCodeAdr+j, DspDecCacheDramData(dwCodeAdr + j-1)) ;
        WriteDspDecCacheDram(dwValueAdr+j, DspDecCacheDramData(dwValueAdr + j-1)) ;
        j -- ;
    }
#endif

#ifdef DUAL_DEC_DRAM_ACCESS   
    #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
    VorbisWriteDecDram32(dwCodeAdr+j, dwCode) ;            
    VorbisWriteDecDram32(dwValueAdr+j, dwValue) ;  
    #else
    VorbisWriteDecDram(dwCodeAdr+j, dwCode) ;            
    VorbisWriteDecDram(dwValueAdr+j, dwValue) ;  
    #endif
#else
    WriteDspDecCacheDram(dwCodeAdr+j, dwCode) ;            
    WriteDspDecCacheDram(dwValueAdr+j, dwValue) ;            
#endif
  }    
}

/***************************************************************************
     Function : vVorbisSort3
  Description : sort the 3 data indexed by i, j, k
    Parameter : codebook value adr, codebook code adr, i, j, k
    Return    : 
***************************************************************************/
void vVorbisSort3(UINT32 dwValueAdr, UINT32 dwCodeAdr, UINT32 i, UINT32 j, UINT32 k)
{
  ///[Joel]no use, UINT32 dwCode1, dwCode2 ;
#ifdef DUAL_DEC_DRAM_ACCESS    
    #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
    if( VorbisDecDramData32(dwCodeAdr + i) > VorbisDecDramData32(dwCodeAdr + j) ) 
        vVorbisExchange(dwValueAdr, dwCodeAdr, i, j) ;

    if( VorbisDecDramData32(dwCodeAdr + i) > VorbisDecDramData32(dwCodeAdr + k) ) 
        vVorbisExchange(dwValueAdr, dwCodeAdr, i, k) ;

    if( VorbisDecDramData32(dwCodeAdr + j) > VorbisDecDramData32(dwCodeAdr + k) ) 
        vVorbisExchange(dwValueAdr, dwCodeAdr, j, k) ;
    #else
    if( VorbisDecDramData(dwCodeAdr + i) > VorbisDecDramData(dwCodeAdr + j) ) 
        vVorbisExchange(dwValueAdr, dwCodeAdr, i, j) ;

    if( VorbisDecDramData(dwCodeAdr + i) > VorbisDecDramData(dwCodeAdr + k) ) 
        vVorbisExchange(dwValueAdr, dwCodeAdr, i, k) ;

    if( VorbisDecDramData(dwCodeAdr + j) > VorbisDecDramData(dwCodeAdr + k) ) 
        vVorbisExchange(dwValueAdr, dwCodeAdr, j, k) ;
    #endif
#else
  if( DspDecCacheDramData(dwCodeAdr + i) > DspDecCacheDramData(dwCodeAdr + j) ) 
    vVorbisExchange(dwValueAdr, dwCodeAdr, i, j) ;

  if( DspDecCacheDramData(dwCodeAdr + i) > DspDecCacheDramData(dwCodeAdr + k) ) 
    vVorbisExchange(dwValueAdr, dwCodeAdr, i, k) ;

  if( DspDecCacheDramData(dwCodeAdr + j) > DspDecCacheDramData(dwCodeAdr + k) ) 
    vVorbisExchange(dwValueAdr, dwCodeAdr, j, k) ;
#endif
}

/***************************************************************************
     Function : vVorbisQuickSort
  Description : quick sort for huffman code
    Parameter : codebook value adr, codebook code adr, codebook length
    Return    : 
***************************************************************************/
void vVorbisQuickSort(UINT32 dwValueAdr, UINT32 dwCodeAdr, UINT32 dwBookLen)
{
  QuickSortStack sBookStack ;
  UINT32 dwLeft, dwRight, dwIndex ;
  
  // stack init
  sBookStack.dwBookStackAdr = (UINT32)ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_STACK ;
  sBookStack.iBookStackTop = -1 ;
  
  dwLeft = 0 ;
  dwRight = dwBookLen-1 ;
  vVorbisQuickSortPush(&sBookStack, dwLeft) ;
  vVorbisQuickSortPush(&sBookStack, dwRight) ;
  do 
  {
    if( (dwRight - dwLeft) > 15 ) 
    {
        vVorbisSort3(dwValueAdr, dwCodeAdr, dwLeft, (dwLeft+dwRight)>>1, dwRight) ;
        vVorbisExchange(dwValueAdr, dwCodeAdr, (dwLeft+dwRight)>>1, dwRight-1) ;
        dwIndex = dwVorbisQuickSortPartition(dwValueAdr, dwCodeAdr, dwLeft, dwRight-1) ;
        if( (dwIndex-dwLeft) > (dwRight-dwIndex) ) 
        {
            vVorbisQuickSortPush(&sBookStack, dwLeft) ;
            vVorbisQuickSortPush(&sBookStack, dwIndex-1) ;
            dwLeft = dwIndex + 1 ;
        }
        else
        {
            vVorbisQuickSortPush(&sBookStack, dwIndex+1) ;
            vVorbisQuickSortPush(&sBookStack, dwRight) ;
            dwRight = dwIndex - 1 ;
        }
    } 
    else 
    {
        vVorbisInsertSort(dwValueAdr, dwCodeAdr, dwLeft, dwRight) ;
        dwRight = dwVorbisQuickSortPop(&sBookStack) ;
        dwLeft = dwVorbisQuickSortPop(&sBookStack) ;
    }
  }while( sBookStack.iBookStackTop >= 0 ) ;
}
#endif                               // end of CC_VORBIS_SUPPORT

//RAW_DELAY
#define IECRAW_SAMPLES_PER_BANK 256

BOOL vSetIECRawDelay(UINT32 u4FreqIdx)
{
    UINT32 u4Factor = 48;
    UINT32 u4ChDelay;
    UINT32 u4DelayBank = 0;
    UINT32 u4MaxNum;
    INT32 i4Sample, i4Delay;

    UNUSED(vSetIECRawDelay);
	
    switch (u4FreqIdx)
    {
    case FS_192K:
    	u4Factor = 192;    	
    	break;        
    case FS_96K:
    	u4Factor = 96;    	
    	break;    
    case FS_48K:
    	u4Factor = 48;    	
    	break;
    case FS_44K:
    	u4Factor = 44;    
    	break;
    case FS_32K:
    	u4Factor = 32;    
    	break;
    default:
    	break;
    }

    u4ChDelay= 0;
    // if IEC RAW delay is synced to channel delay?
    if (uReadDspShmBYTE(B_IECRAW_DELAY_FLAG) != 0)  
    {
        // the IEC RAW is modified accroding to "L" channel delay 
        u4ChDelay = (UINT32)(u2ReadDspShmWORD(W_CHDELAY_L) * u4Factor * 5) / 34; // samples
    }

    i4Delay = (INT32)(u2ReadDspShmWORD(W_IEC_RAW_OFFSET) << 16) >> 16; // signed extenstion
    i4Sample = u4ChDelay +  i4Delay * u4Factor; // samples
    if (i4Sample < 0)
    {
        i4Sample = 0;
    }
    u4DelayBank = (UINT32)((i4Sample + (IECRAW_SAMPLES_PER_BANK/2)) / 
    	IECRAW_SAMPLES_PER_BANK); // fix me!!! Current usage, only 256 bank supports IEC RAW output
    
    LOG(3, "IEC RAW delay = (%d) bank\n", u4DelayBank);

    // protection for invalid value
    u4MaxNum = uReadDspShmBYTE(B_BANK256NUM) - 10; // PCM = 6 + 4, fix me!!!!
     if (u4DelayBank > u4MaxNum)
    {
        LOG (1, "IEC RAW delay  %d is invalid! (MAX number is %d)\n", u4DelayBank, u4MaxNum);
        u4DelayBank = u4MaxNum;
    }

    WriteDspCommDram (ADDR_RC2D_IEC_DELAY, u4DelayBank);
    
    return TRUE;
}



