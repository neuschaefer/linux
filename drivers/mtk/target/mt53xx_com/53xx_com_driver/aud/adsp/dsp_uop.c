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
/***************  $Revision: #3 $       ****************/
/***************                                       ****************/
/***************   Description : DSP Control routines  ****************/
/***************                                       ****************/
/***************       Company : MediaTek Inc.         ****************/
/***************    Programmer : Jack Hsu              ****************/
/***************                 Peichen Chang         ****************/
/**********************************************************************/
#define _DSP_UOP_C

//#define EFFECT_TRANSPARENT_REINIT

#include "dsp_common.h"
#include "dsp_shm.h"
#include "dsp_uop.h"
#include "dsp_func.h"
#include "dsp_table.h"
#include "dsp_reg.h"
#include "d2rc_shm.h"
#include "drv_adsp.h"

#include "aud_if.h"
#include "aud_debug.h"
#include "drvcust_if.h"

#include "x_assert.h"
#include "x_chip_id.h"

#include "x_os.h"

#if 0
static UINT32 vEQTranGain(UINT32 dwBand);
#endif
static void vVolumeUOP(UINT16 wDspUop);
static void vMixSoundUOP(UINT16 wDspUop);
static void vFlowControlUOP(UINT16 wDspUop);
static void vChanConfigUOP(UINT16 wDspUop);
static void vGrp5UOP(UINT16 wDspUop);
static void vPinkUOP(UINT16 wDspUop);
static void vKaraokeUOP(UINT16 wDspUop);
static void vEQUOP(UINT16 wDspUop);
static void v3DUOP(UINT16 wDspUop);
static void vDecUOP(UINT16 wDspUop);
static BOOL fgDspAC3Uop(UINT8 u1DecId, UINT16 wDspUop);
static BOOL fgDspAACUop(UINT8 u1DecId, UINT16 wDspUop);
#if 0 ///1///def CC_VORBIS_SUPPORT ///def DSP_VORBIS_SUPPORT ///[Joel] For middle play
static BOOL vDspVorbisUop(UINT8 u1DecId, UINT16 wDspUop);
#endif
static BOOL fgDspPCMUop(UINT8 u1DecId, UINT16 wDspUop);
static void vSetAOutReinit(UINT8 u1DecId);
#ifdef EFFECT_TRANSPARENT_REINIT //add for transparent reinitial
static void vSetEffectReinit(UINT8 u1DecId);
static void vAoutReinitMute(UINT8 u1DecId);
#endif
static void vDspIECConfig(UINT8 u1DecId);

//static void DspIntOrSram (UINT32 u4Addr, UINT32 dwValue);
static void DspIntAndSram(UINT32 u4Addr, UINT32 dwValue);

extern UINT32 dwGetPEQFsIdx(UINT8 u1Set);
extern UINT32 dwGetPEQCosIdx(UINT8 u1Set);

//const UINT16 u2EQTblBandSZ[5] = {DSP_EQ_BAND1_SZ, DSP_EQ_BAND2_SZ,
//  	                             DSP_EQ_BAND3_SZ, DSP_EQ_BAND4_SZ,
//	                             DSP_EQ_BAND5_SZ};

#if 0
/***************************************************************************
     Function : vEQTranGain
	Description : None
		Parameter : EQ Band Number
		Return    : Gain
***************************************************************************/
UINT32 vEQTranGain(UINT32 dwBand)
{
    UINT8 u1LCnt;
    INT32 i4Bidx;
    UINT32 u4Idx;
    INT32 i4TmpG = 0;
    const INT16 *i2Gptr = &i2EQTblGain[dwBand*5];

    for (u1LCnt = 0; u1LCnt < 5; u1LCnt++)
    {
        u4Idx = u4ReadDspShmDWRD ((D_C_DRY + ((u1LCnt+1) << 2)));
        i4Bidx = (INT32)u4Idx - 20; // bias band gain from 0 ~ 40 to -20 ~ 20
        // 15.1*2.14=17.15
        i4TmpG += i4Bidx * (INT32)(*i2Gptr++);
    }

    i4TmpG = i4TmpG + (INT32)(1<<13);
    i4TmpG /= 0x4000;          //signed, 31.1

    i4TmpG = i4TmpG + (((INT32)u2EQTblBandSZ[dwBand]/5)>>1); // bias to unsigned
    i4TmpG *= 5;

    // check boundary
    if ( i4TmpG > (INT32)(u2EQTblBandSZ[dwBand]-5) )
    {
        i4TmpG = (INT32)(u2EQTblBandSZ[dwBand]-5);
    }
    else if ( i4TmpG < 0 )
    {
        i4TmpG = 0;
    }

    return ((UINT32)i4TmpG);

}
#endif

/***************************************************************************
     Function : vEQBandGainFlush
	Description : None
		Parameter : None
		Return    : None
***************************************************************************/
void vEQBandGainFlush (UINT8 u1DecId)
{
    UINT8 u1BankIdx;
    UINT32 u4DestAddr;
    UINT32 iLoopVar;
    UINT32 *pu4Table;
    UINT32 u4BandAddr;
    UINT32 u4BandShmIdx;

    UINT32 u4Ptr;
    VERIFY(DRVCUST_OptQuery(eAudioAvChlEqCoefTable, (UINT32 *)&u4Ptr)==0);

    if (u1DecId != AUD_DEC_MAIN)
    {
        return;
    }

    u4DestAddr = ADDR_RC2D_EQUALIZER_TABLE_NEW;
    u4BandShmIdx = B_EQ_BAND1;

    pu4Table = ((UINT32*)u4Ptr);

    for (u1BankIdx=0; u1BankIdx<AUD_EQ_IIR_BAND_NUM; u1BankIdx++)
    {
        u4BandAddr = uReadDspShmBYTE((u4BandShmIdx+u1BankIdx)) * 5;
        // copy filter coefficients (5) to DSP common DRAM
        for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
        {
	        WriteDspCommDram ((u4DestAddr+iLoopVar), pu4Table[u4BandAddr + iLoopVar]);
        }

        // move destination address to next band
        u4DestAddr += 5;
        // move table pointer to next band
        pu4Table += DSP_EQ_BAND_TABLE_SIZE;
    }

#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
    //pu4Table = (UINT32 *)&AEQ_5BAND_TABLE[0][0];//[u4FreqIdx][0];
    pu4Table = (UINT32*)(DRVCUST_OptGet(eAudioAeq5BandTable));
    u4DestAddr = ADDR_RC2D_AEQ_TABLE_NEW;

    for (u1BankIdx=0; u1BankIdx<AUD_AEQ_IIR_BAND_NUM; u1BankIdx++)//band nuber
    {
        // copy filter coefficients (5) to DSP common DRAM
        for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
        {
	        WriteDspCommDram ((u4DestAddr+iLoopVar), pu4Table[iLoopVar]);
        }

        // move destination address to next band
        u4DestAddr += 5;
        // move table pointer to next band
        pu4Table += 5;//DSP_AEQ_BAND_TABLE_SIZE;
    }
#endif
}

//-----------------------------------------------------------------------------
/** vEQ2SbassBandGainFlush
 *  Turn On/Off subwoofer
 *  @param  u1DecId          Decoder ID
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
void vEQ2SbassBandGainFlush (UINT8 u1DecId)
{
    UINT32 u4DestAddr;
    UINT32 iLoopVar;
    UINT32 *pu4Table;
    UINT32 u4ShmIdxBass, u4ShmIdxTreble;

    UINT32 u4Ptr;
    VERIFY(DRVCUST_OptQuery(eAudioBassTrebleCoefTable, (UINT32 *)&u4Ptr)==0);

    if (u1DecId != AUD_DEC_MAIN)
    {
        return;
    }

    u4DestAddr = ADDR_RC2D_SUPER_BASS_TABLE_NEW;
    u4ShmIdxBass = D_SBASSBOOSTGAIN;
    u4ShmIdxTreble = D_SBASSCLEARGAIN;

    pu4Table = (UINT32 *)u4Ptr + (u4ReadDspShmDWRD(u4ShmIdxBass) * 5);

    for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
    {
        WriteDspCommDram ((u4DestAddr+iLoopVar), pu4Table[iLoopVar]);
    }
    // move destination address to next band
    u4DestAddr += 5;

    pu4Table = (UINT32 *)u4Ptr + ((u4ReadDspShmDWRD(u4ShmIdxTreble) * 5) + (BASS_TREBLE_GAIN_MAX*5));

    for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
    {
        WriteDspCommDram ((u4DestAddr+iLoopVar), pu4Table[iLoopVar]);
    }
}

void DspIntOrSram (UINT32 u4Addr, UINT32 dwValue)
{
    _uDspSIntAddr = INT_RC2D_OR;
    _u4DspSIntLD = u4Addr;
    _u4DspSIntSD = dwValue;
    _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;
}

void DspIntAndSram (UINT32 u4Addr, UINT32 dwValue)
{
    _uDspSIntAddr = INT_RC2D_AND;
    _u4DspSIntLD = u4Addr;
    _u4DspSIntSD = dwValue;
    _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;
}

void vSetAOutReinit(UINT8 u1DecId)
{
    _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;
    _uDspSIntAddr = INT_RC2D_FLOW_CONTROL;

    if (u1DecId == AUD_DEC_AUX)
    {
        _uDspSIntAddr = INT_RC2D_FLOW_CONTROL_DEC2;
    }

    if (_rDspStatus[u1DecId].fgDspRealPlay)
    {
        // Use Transparent reinit during playing period to prevent audio stop
        _u4DspSIntSD = (UINT32) FLOW_CONTROL_AOUT_REINIT+FLOW_CONTROL_TRANSPARENT_REINIT;
        //_u4DspSIntSD = (UINT32) FLOW_CONTROL_AOUT_REINIT;
    }
    else
    {
        _u4DspSIntSD = (UINT32) FLOW_CONTROL_AOUT_REINIT;
    }
}


//#ifdef EFFECT_TRANSPARENT_REINIT //add for transparent reinitial
void vSetEffectReinit(UINT8 u1DecId)
{
    if (_rDspStatus[u1DecId].fgDspRealPlay)
    {
		if (u1DecId == AUD_DEC_MAIN) // trigger effect transparent reinitial
		{
			DspIntOrSram(SRAM_EFFECT_CONTROL,0x100);
		}
		else
		{
			DspIntOrSram(SRAM_EFFECT_CONTROL_DEC2,0x100);
		}
    }
    else
    {
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;
        _uDspSIntAddr = INT_RC2D_FLOW_CONTROL;

        if (u1DecId == AUD_DEC_AUX)
        {
            _uDspSIntAddr = INT_RC2D_FLOW_CONTROL_DEC2;
        }
        _u4DspSIntSD = (UINT32) FLOW_CONTROL_AOUT_REINIT;
    }
}
//#endif

void vAoutReinitMute(UINT8 u1DecId)
{
    UINT32 ch_index;
    UINT16 u2DspData;
    UINT32 u4Data;

    if ((u1DecId == AUD_DEC_MAIN) && _rDspStatus[AUD_DEC_MAIN].fgDspRealPlay)
    {
        WriteDspCommDram (ADDR_RC2D_MASTER_VOLUME, 0x0);
        vWriteDspSram(SRAM_SMOOTH_VOLUME, 0x0);
        u2DspData = 0;
        for (ch_index = 0; ch_index < 10; ch_index++)
        {
            if ( u2DspData < u2ReadDspShmWORD((W_CHDELAY_C+(ch_index*2))) )
            {
                u2DspData = u2ReadDspShmWORD((W_CHDELAY_C+(ch_index*2)));
            }
        }
        u4Data = (UINT32)(u2DspData * 5) / 34;
        if (u4Data > 0)
        {
            u4Data = u4Data + 5;
            if (u4Data > 100)
            {
                u4Data = 100;
            }
            x_thread_delay(u4Data);
        }
        _rDspStatus[AUD_DEC_MAIN].fgDspAoutMuted = TRUE;
    }
    else if ((u1DecId == AUD_DEC_AUX) && _rDspStatus[AUD_DEC_AUX].fgDspRealPlay)
    {
        WriteDspCommDram (ADDR_RC2D_MASTER_VOLUME_DEC2, 0x0);
        vWriteDspSram(SRAM_SMOOTH_VOLUME_DEC2, 0x0);
        u2DspData = 0;
        for (ch_index = 0; ch_index < 3; ch_index++)
        {
            if ( u2DspData < u2ReadDspShmWORD((W_CHDELAY_C_DEC2+(ch_index*2))) )
            {
                u2DspData = u2ReadDspShmWORD((W_CHDELAY_C_DEC2+(ch_index*2)));
            }
        }
        u4Data = (UINT32)(u2DspData * 5 )/ 34;
        u4Data = u4Data + 10;
        if (u4Data > 100)
        {
            u4Data = 100;
        }
        x_thread_delay(u4Data);
        _rDspStatus[AUD_DEC_AUX].fgDspAoutMuted = TRUE;
    }
}

#ifdef DSP_SUPPORT_NPTV
/***************************************************************************
  Function    : fgDspNTSCDemodUop
  Description : Routine NTSC Demodulation User Operation
  Parameter   : UOP
  Return      : Find UOP
***************************************************************************/
static BOOL fgDspNTSCDemodUop (UINT16 wDspUop, UINT8 bDspStrTyp)
{
    BOOL fgRet;

    fgRet = TRUE;
    _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;
    switch(wDspUop)
    {
    case UOP_DSP_DEMOD_CONFIG:
        switch (bDspStrTyp)
        {
        case PAL_STREAM:
        //case NICAM_STREAM:
            vWriteDspWORD (ADDR_DEM_ADC_GAIN,u2ReadDspShmWORD (W_ADC_GAIN));
            vWriteDspWORD (ADDR_DEM_USER_MODE,u2ReadDspShmWORD (W_USER_MODE));
            vWriteDspWORD (ADDR_DEM_ENFORCED_MODE,u2ReadDspShmWORD (W_ENFORCED_MODE));

            // Decoder 2
            vWriteDspWORD (ADDR_DEM_USER_MODE_DEC2,u2ReadDspShmWORD (W_USER_MODE));
            vWriteDspWORD (ADDR_DEM_ADC_GAIN_DEC2,u2ReadDspShmWORD (W_ADC_GAIN));
            vWriteDspWORD (ADDR_DEM_ENFORCED_MODE_DEC2,u2ReadDspShmWORD (W_ENFORCED_MODE));

		    //_uDspSIntAddr = INT_RC2D_FLOW_CONTROL;
		    //_u4DspSIntSD = (UINT32) FLOW_CONTROL_AOUT_REINIT;
			//vDspCmd(UOP_DSP_AOUT_REINIT);
		#if 0 // gallen, 20070514
			vSetEffectReinit(AUD_DEC_MAIN);
		    _fgDspSInt = TRUE;
		#else
			_rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
		#endif
		    break;
        case NTSC_STREAM:
        case FMFM_STREAM:
        case FMRDO_STREAM:
	        vWriteDspWORD (ADDR_DEM_ADC_GAIN,u2ReadDspShmWORD (W_ADC_GAIN));
            vWriteDspWORD (ADDR_DEM_USER_MODE,u2ReadDspShmWORD (W_USER_MODE));
            vWriteDspWORD (ADDR_DEM_SOURCE_MODE, u2ReadDspShmWORD (W_SOURCE_MODE));

            // Decoder 2
            vWriteDspWORD (ADDR_DEM_SOURCE_MODE_DEC2, u2ReadDspShmWORD (W_SOURCE_MODE));
            vWriteDspWORD (ADDR_DEM_USER_MODE_DEC2,u2ReadDspShmWORD (W_USER_MODE));
            vWriteDspWORD (ADDR_DEM_ADC_GAIN_DEC2,u2ReadDspShmWORD (W_ADC_GAIN));

            //don't send interrupt since ntscdec.pra v700607
            // _uDspSIntAddr = INT_RC2D_FLOW_CONTROL;
            // _u4DspSIntSD = (UINT32) FLOW_CONTROL_AOUT_REINIT;
            _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
            break;
        case A2_STREAM:
            vWriteDspWORD (ADDR_DEM_ADC_GAIN,u2ReadDspShmWORD (W_ADC_GAIN));
            vWriteDspWORD (ADDR_DEM_USER_MODE,u2ReadDspShmWORD (W_USER_MODE));

            // Decoder 2
            vWriteDspWORD (ADDR_DEM_USER_MODE_DEC2,u2ReadDspShmWORD (W_USER_MODE));
            vWriteDspWORD (ADDR_DEM_ADC_GAIN_DEC2,u2ReadDspShmWORD (W_ADC_GAIN));

            // _uDspSIntAddr = INT_RC2D_FLOW_CONTROL;
            // _u4DspSIntSD = (UINT32) FLOW_CONTROL_AOUT_REINIT;
         #if 0  // gallen, 20070524
                vSetEffectReinit(AUD_DEC_MAIN);
               _fgDspSInt = TRUE;
         #else
		    _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
	     #endif
            break;
        default:
            fgRet = FALSE;
            _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
            break; //exception
        }
        break;
    case UOP_DSP_DETECTION_CONFIG:
        switch (bDspStrTyp)
		{
		case PAL_STREAM:
		//case NICAM_STREAM:
			vWriteDspWORD (ADDR_PAL_CORRECT_THRESHOLD,
							u2ReadDspShmWORD (W_PAL_CORRECT_THRESHOLD));
			vWriteDspWORD (ADDR_PAL_TOTAL_SYNC1_LOOP,
							u2ReadDspShmWORD (W_PAL_TOTAL_SYNC1_LOOP));
			vWriteDspWORD (ADDR_PAL_ERR_THRESHOLD,
							u2ReadDspShmWORD (W_PAL_ERR_THRESHOLD));
			vWriteDspWORD (ADDR_PAL_PARITY_ERR_THRESHOLD,
							u2ReadDspShmWORD (W_PAL_PARITY_ERR_THRESHOLD));
			vWriteDspWORD (ADDR_PAL_EVERY_NUM_FRAMES,
							u2ReadDspShmWORD (W_PAL_EVERY_NUM_FRAMES));
			//for AM carrier sense mute
			vWriteDspWORD (ADDR_SECAM_L_MUTE_MODE,
							uReadDspShmBYTE (B_AM_MUTE_MODE));
			vWriteDspWORD (ADDR_SECAM_L_MUTE_THRESHOLD_HIGHT,
							uReadDspShmBYTE (B_AM_MUTE_THRESHOLD_HIGHT));
			vWriteDspWORD (ADDR_SECAM_L_MUTE_THRESHOLD_LOW,
							uReadDspShmBYTE (B_AM_MUTE_THRESHOLD_LOW));
			//for FM carrier sense mute
			vWriteDspWORD (ADDR_FM_CARRIER_MUTE_MODE,
							(uReadDspShmBYTE (B_FM_MUTE_MODE)& 0x01));
			vWriteDspWORD (ADDR_FM_CARRIER_MUTE_THRESHOLD_HIGHT,
							uReadDspShmBYTE (B_FM_MUTE_THRESHOLD_HIGHT_PAL));
			vWriteDspWORD (ADDR_FM_CARRIER_MUTE_THRESHOLD_LOW,
							uReadDspShmBYTE (B_FM_MUTE_THRESHOLD_LOW_PAL));
			//for EU/NON-EU FM
			vWriteDspWORD (ADDR_FM_NON_EU_MODE,
							uReadDspShmBYTE (B_NON_EU_FM_MODE));
			//for NICAM I boost or not
            vWriteDspWORD(ADDR_NICAM_I_BOOST_FLAG,
                            uReadDspShmBYTE(B_NICAM_I_BOOST_FLAG));

            // Decoder 2
            vWriteDspWORD (ADDR_SECAM_L_MUTE_MODE_DEC2,
							uReadDspShmBYTE (B_AM_MUTE_MODE));
            vWriteDspWORD (ADDR_SECAM_L_MUTE_THRESHOLD_HIGHT_DEC2,
							uReadDspShmBYTE (B_AM_MUTE_THRESHOLD_HIGHT));
            vWriteDspWORD (ADDR_SECAM_L_MUTE_THRESHOLD_LOW_DEC2,
							uReadDspShmBYTE (B_AM_MUTE_THRESHOLD_LOW));


			_rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
			break;
        case NTSC_STREAM:
            vWriteDspWORD (ADDR_MTS_NUM_CHECK,
                            u2ReadDspShmWORD (W_MTS_NUM_CHECK));
            vWriteDspWORD (ADDR_MTS_STEREO_MID,
                            u2ReadDspShmWORD (W_MTS_STEREO_MID));
            vWriteDspWORD (ADDR_MTS_STEREO_CON_MID,
                            u2ReadDspShmWORD (W_MTS_STEREO_CON_MID));
            vWriteDspWORD (ADDR_MTS_NUM_PILOT,
                            u2ReadDspShmWORD (W_MTS_NUM_PILOT));
            vWriteDspWORD (ADDR_MTS_NUM_SAP,
                            u2ReadDspShmWORD (W_MTS_NUM_SAP));
            vWriteDspWORD (ADDR_MTS_SAP_MID,
                            u2ReadDspShmWORD (W_MTS_SAP_MID));
            vWriteDspWORD (ADDR_MTS_SAP_CON_MID,
                            u2ReadDspShmWORD (W_MTS_SAP_CON_MID));
	        //for FM carrier sense mute
            vWriteDspWORD (ADDR_FM_CARRIER_MUTE_MODE,
                            (uReadDspShmBYTE (B_FM_MUTE_MODE)& 0x04)>>2);
            vWriteDspWORD (ADDR_FM_CARRIER_MUTE_THRESHOLD_HIGHT,
                            uReadDspShmBYTE  (B_FM_MUTE_THRESHOLD_HIGHT_MTS));
            vWriteDspWORD (ADDR_FM_CARRIER_MUTE_THRESHOLD_LOW,
                            uReadDspShmBYTE (B_FM_MUTE_THRESHOLD_LOW_MTS));
            vWriteDspWORD (ADDR_MTS_PILOT_OFFSET_DETECTION,
                            uReadDspShmBYTE (B_MTS_PILOT_OFFSET_DETECTION));
            vWriteDspWORD (ADDR_MTS_MUTE_SAP_LOW,
                            uReadDspShmBYTE (B_MTS_MUTE_SAP_LOW));
            vWriteDspWORD (ADDR_MTS_MUTE_SAP_HIGH,
                            uReadDspShmBYTE (B_MTS_MUTE_SAP_HIGH));
            vWriteDspWORD (ADDR_MTS_SAP_HP_NOISE_LOW,
                           uReadDspShmBYTE (B_SAP_HP_MUTE_LOW));
            vWriteDspWORD (ADDR_MTS_SAP_HP_NOISE_HIGH,
                           uReadDspShmBYTE (B_SAP_HP_MUTE_HIGH));
            vWriteDspWORD (ADDR_MTS_SAP_FILTER_SEL,
                           uReadDspShmBYTE (B_SAP_FILTER_SEL));
            vWriteDspWORD (ADDR_MTS_SAP_RATIO,
                           uReadDspShmBYTE (B_SAP_RATIO));

            _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
            break;
		case FMFM_STREAM:
			vWriteDspWORD (ADDR_FMFM_NUM_CHECK,
							u2ReadDspShmWORD (W_FMFM_NUM_CHECK));
			vWriteDspWORD (ADDR_FMFM_NUM_DOUBLE_CHECK,
							u2ReadDspShmWORD (W_FMFM_NUM_DOUBLE_CHECK));
			vWriteDspWORD (ADDR_FMFM_MONO_WEIGHT,
							u2ReadDspShmWORD (W_FMFM_MONO_WEIGHT));
			vWriteDspWORD (ADDR_FMFM_STEREO_WEIGHT,
							u2ReadDspShmWORD (W_FMFM_STEREO_WEIGHT));
			vWriteDspWORD (ADDR_FMFM_DUAL_WEIGHT,
							u2ReadDspShmWORD (W_FMFM_DUAL_WEIGHT));
           	vWriteDspWORD (ADDR_FMFM_DETECTION_CONFID,
					       	u2ReadDspShmWORD (W_FMFM_DETECT_CONFID));
	        //for FM carrier sense mute
	       	vWriteDspWORD (ADDR_FM_CARRIER_MUTE_MODE,
							(uReadDspShmBYTE (B_FM_MUTE_MODE)& 0x08)>>3);
		    vWriteDspWORD (ADDR_FM_CARRIER_MUTE_THRESHOLD_HIGHT,
							uReadDspShmBYTE (B_FM_MUTE_THRESHOLD_HIGHT_EIAJ));
		    vWriteDspWORD (ADDR_FM_CARRIER_MUTE_THRESHOLD_LOW,
							uReadDspShmBYTE (B_FM_MUTE_THRESHOLD_LOW_EIAJ));

            // Decoder 2
            vWriteDspWORD (ADDR_FMFM_NUM_CHECK_DEC2,
							u2ReadDspShmWORD (W_FMFM_NUM_CHECK));
            vWriteDspWORD (ADDR_FMFM_NUM_DOUBLE_CHECK_DEC2,
							u2ReadDspShmWORD (W_FMFM_NUM_DOUBLE_CHECK));
            vWriteDspWORD (ADDR_FMFM_MONO_WEIGHT_DEC2,
							u2ReadDspShmWORD (W_FMFM_MONO_WEIGHT));
            vWriteDspWORD (ADDR_FMFM_STEREO_WEIGHT_DEC2,
							u2ReadDspShmWORD (W_FMFM_STEREO_WEIGHT));
            vWriteDspWORD (ADDR_FMFM_DUAL_WEIGHT_DEC2,
							u2ReadDspShmWORD (W_FMFM_DUAL_WEIGHT));
            vWriteDspWORD (ADDR_FMFM_DETECTION_CONFID_DEC2,
					       	u2ReadDspShmWORD (W_FMFM_DETECT_CONFID));

		    _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
			break;
		case A2_STREAM:
			vWriteDspWORD (ADDR_A2_NUM_CHECK, u2ReadDspShmWORD (W_A2_NUM_CHECK));
			vWriteDspWORD (ADDR_A2_NUM_DOUBLE_CHECK,
							u2ReadDspShmWORD (W_A2_NUM_DOUBLE_CHECK));
			vWriteDspWORD (ADDR_A2_MONO_WEIGHT,
							u2ReadDspShmWORD (W_A2_MONO_WEIGHT));
			vWriteDspWORD (ADDR_A2_STEREO_WEIGHT,
							u2ReadDspShmWORD (W_A2_STEREO_WEIGHT));
			vWriteDspWORD (ADDR_A2_DUAL_WEIGHT,
							u2ReadDspShmWORD (W_A2_DUAL_WEIGHT));
	            //for FM carrier sense mute
	       	vWriteDspWORD (ADDR_FM_CARRIER_MUTE_MODE,
							(uReadDspShmBYTE (B_FM_MUTE_MODE)& 0x02)>>1);
            vWriteDspWORD (ADDR_FM_CARRIER_MUTE_THRESHOLD_HIGHT,
							uReadDspShmBYTE (B_FM_MUTE_THRESHOLD_HIGHT_A2));
            vWriteDspWORD (ADDR_FM_CARRIER_MUTE_THRESHOLD_LOW,
							uReadDspShmBYTE (B_FM_MUTE_THRESHOLD_LOW_A2));
			//for EU/NON-EU FM
			vWriteDspWORD (ADDR_FM_NON_EU_MODE,
							uReadDspShmBYTE (B_NON_EU_FM_MODE));
            if (u2ReadDspShmWORD (W_SOURCE_MODE) == SOURCE_NTSC_M)
            {
                vWriteDspWORD (ADDR_A2_STEREO_MONO_THRES, u2ReadDspShmWORD(W_A2_STEREO_MONO_THRES_M));
                vWriteDspWORD (ADDR_A2_MONO_STEREO_THRES, u2ReadDspShmWORD(W_A2_MONO_STEREO_THRES_M));
            }
            else
            {
                vWriteDspWORD (ADDR_A2_STEREO_MONO_THRES, u2ReadDspShmWORD(W_A2_STEREO_MONO_THRES_EU));
                vWriteDspWORD (ADDR_A2_MONO_STEREO_THRES, u2ReadDspShmWORD(W_A2_MONO_STEREO_THRES_EU));
            }
			 vWriteDspWORD (ADDR_A2_WEAK_RF_LVL_S2M_THRES, u2ReadDspShmWORD(W_A2_WEAK_RF_LVL_S2M_THRES_EU));
                      vWriteDspWORD (ADDR_A2_WEAK_RF_LVL_M2S_THRES, u2ReadDspShmWORD(W_A2_WEAK_RF_LVL_M2S_THRES_EU));
			_rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
			break;
        default:
			_rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
            break;
        }
        break;
    case UOP_DSP_DEMOD_FINE_TUNE_VOLUME:
        switch (bDspStrTyp)
		{
		case PAL_STREAM:
		//case NICAM_STREAM:
            vWriteDspWORD (ADDR_DEM_PAL_FINE_TUNE_VOLUME,
                            u4ReadDspShmDWRD(D_PAL_FINE_TUNE_VOLUME)>>8);
            vWriteDspWORD (ADDR_DEM_NICAM_FINE_TUNE_VOLUME,
                            u4ReadDspShmDWRD (D_NICAM_FINE_TUNE_VOLUME)>>8);
            vWriteDspWORD (ADDR_DEM_AM_FINE_TUNE_VOLUME,
                            u4ReadDspShmDWRD (D_AM_FINE_TUNE_VOLUME)>>8);

            // Decoder 2
            vWriteDspWORD (ADDR_DEM_PAL_FINE_TUNE_VOLUME_DEC2,
                            u4ReadDspShmDWRD(D_PAL_FINE_TUNE_VOLUME)>>8);
            vWriteDspWORD (ADDR_DEM_NICAM_FINE_TUNE_VOLUME_DEC2,
                            u4ReadDspShmDWRD (D_NICAM_FINE_TUNE_VOLUME)>>8);
            vWriteDspWORD (ADDR_DEM_AM_FINE_TUNE_VOLUME_DEC2,
                            u4ReadDspShmDWRD (D_AM_FINE_TUNE_VOLUME)>>8);
            _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
            break;
        case A2_STREAM:
            vWriteDspWORD (ADDR_DEM_PAL_FINE_TUNE_VOLUME,
                            u4ReadDspShmDWRD (D_A2_FINE_TUNE_VOLUME)>>8);

            // Decoder 2
            vWriteDspWORD (ADDR_DEM_PAL_FINE_TUNE_VOLUME_DEC2,
                            u4ReadDspShmDWRD (D_A2_FINE_TUNE_VOLUME)>>8);
            _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
            break;
        case NTSC_STREAM:
            vWriteDspWORD (ADDR_DEM_MTS_FINE_TUNE_VOLUME,
                            u4ReadDspShmDWRD  (D_MTS_FINE_TUNE_VOLUME)>>8);
            vWriteDspWORD (ADDR_DEM_SAP_FINE_TUNE_VOLUME,
                            u4ReadDspShmDWRD (D_SAP_FINE_TUNE_VOLUME)>>8);

            // Decoder 2
            vWriteDspWORD (ADDR_DEM_SAP_FINE_TUNE_VOLUME_DEC2,
                            u4ReadDspShmDWRD (D_SAP_FINE_TUNE_VOLUME)>>8);
            vWriteDspWORD (ADDR_DEM_MTS_FINE_TUNE_VOLUME_DEC2,
                            u4ReadDspShmDWRD  (D_MTS_FINE_TUNE_VOLUME)>>8);
            _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
            break;
        case FMFM_STREAM:
            //vWriteDspWORD (ADDR_DEM_FMFM_MONO_FINE_TUNE_VOLUME,
            //                u4ReadDspShmDWRD  (D_FMFM_MONO_FINE_TUNE_VOLUME)>>8);
            vWriteDspWORD (ADDR_DEM_FMFM_DUAL_FINE_TUNE_VOLUME,
                            u4ReadDspShmDWRD (D_FMFM_DUAL_FINE_TUNE_VOLUME)>>8);

            // Decoder 2
            vWriteDspWORD (ADDR_DEM_FMFM_DUAL_FINE_TUNE_VOLUME_DEC2,
                            u4ReadDspShmDWRD (D_FMFM_DUAL_FINE_TUNE_VOLUME)>>8);
            _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
            break;
         case FMRDO_STREAM:
            vWriteDspWORD (ADDR_DEM_FMRADIO_FINE_TUNE_VOLUME,
                            u4ReadDspShmDWRD  (D_FMRADIO_FINE_TUNE_VOLUME)>>8);
            break;
        default:
            _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
            break;
        }
        break;
    default:
        fgRet = FALSE;
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
        break; //exception
    }
    return fgRet;
}
#endif
/***************************************************************************
  Function    : vDecUOP
  Description : Decoding related User Operation
  Parameter   : None
  Return      : None
***************************************************************************/
void vDecUOP (UINT16 wDspUop)
{
    BOOL fgGetRet;

    // Decoder 1
    switch (_rDspVars[AUD_DEC_MAIN].bDspStrTyp)
    {
    case AC3_STREAM:
        fgGetRet = fgDspAC3Uop (AUD_DEC_MAIN, wDspUop);
        break;
    case AAC_STREAM:
        fgGetRet = fgDspAACUop (AUD_DEC_MAIN, wDspUop);
        break;
    case PCM_STREAM:
        fgGetRet = fgDspPCMUop (AUD_DEC_MAIN, wDspUop);
    	break;
#if 0 ///1///def CC_VORBIS_SUPPORT ///def DSP_VORBIS_SUPPORT   ///[Joel] For middle play
    case VORBIS_STREAM:
        fgGetRet = vDspVorbisUop(AUD_DEC_MAIN, wDspUop);
        break;
#endif
    default:
        fgGetRet = FALSE;
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
    }

    if(fgGetRet != FALSE)
        return;

    // Decoder 2
    switch (_rDspVars[AUD_DEC_AUX].bDspStrTyp)
    {
    case AC3_STREAM:
        fgGetRet = fgDspAC3Uop (AUD_DEC_AUX, wDspUop);
        break;
    case AAC_STREAM:
        fgGetRet = fgDspAACUop (AUD_DEC_AUX, wDspUop);
        break;
    case PCM_STREAM:
        fgGetRet = fgDspPCMUop (AUD_DEC_AUX, wDspUop);
        break;
#if 0 ///1///def CC_VORBIS_SUPPORT ///def DSP_VORBIS_SUPPORT   ///[Joel] For middle play
    case VORBIS_STREAM:
        fgGetRet = vDspVorbisUop(AUD_DEC_AUX, wDspUop);
        break;
#endif
    default:
        fgGetRet = FALSE;
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
    }

    if(fgGetRet != FALSE)
        return;

    // Decoder 3
    switch (_rDspVars[AUD_DEC_THIRD].bDspStrTyp)
    {
    case AAC_STREAM:
        fgGetRet = fgDspAACUop (AUD_DEC_THIRD, wDspUop);
        break;
    default:
        fgGetRet = FALSE;
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
    }    

    if (fgGetRet == FALSE)
    {
        LOG(7, "Warning: Receive undefined UOP: %.4x\n", wDspUop);
    }
}

/***************************************************************************
  Function    : vAtvUOP
  Description : ATV (radio, detection) related User Operation
  Parameter   : None
  Return      : None
***************************************************************************/
void vAtvUOP (UINT16 wDspUop)
{
    BOOL fgGetRet;

    switch (_rDspVars[AUD_DEC_MAIN].bDspStrTyp)
    {
#ifdef DSP_SUPPORT_NPTV
    case NTSC_STREAM:
    case FMFM_STREAM:
    case A2_STREAM:
    case PAL_STREAM:
    case FMRDO_STREAM:
        fgGetRet = fgDspNTSCDemodUop(wDspUop,_rDspVars[AUD_DEC_MAIN].bDspStrTyp);
        break;
#endif
    default:
        fgGetRet = FALSE;
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
    }

    if (fgGetRet == FALSE)
    {
        switch (_rDspVars[AUD_DEC_AUX].bDspStrTyp)
        {
#ifdef DSP_SUPPORT_NPTV
        case NTSC_STREAM:
        case FMFM_STREAM:
        case A2_STREAM:
        case PAL_STREAM:
        case FMRDO_STREAM:
            fgGetRet = fgDspNTSCDemodUop(wDspUop,_rDspVars[AUD_DEC_AUX].bDspStrTyp);
            break;
#endif
        default:
            fgGetRet = FALSE;
            _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
        }
    }
    if (fgGetRet == FALSE)
    {
        LOG(7, "Warning: Receive undefined UOP: %.4x\n", wDspUop);
    }
}

void v3DUOP (UINT16 wDspUop)
{
    //UINT16 u2DspData;//, wDspData2;
    //UINT32 dwDspInfo;
    UINT8 bVsurrType;
    UINT32 u4DramAdrIdx;
#ifdef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
    UINT16 wVsurrType;
#endif

    UNUSED(u4DramAdrIdx);

    switch (wDspUop)
    {
    case UOP_DSP_PRO_LOGICII_CONFIG:
        vWriteDspWORD (ADDR_RC2D_PRO_LOGIC_II_CONFIG,
                       u2ReadDspShmWORD (W_PLIICONFIG));
        // if prologic is open -> close virtual surround
        /*
        if (u2DspData != 0)
        {
            vWriteDspWORD (ADDR_RC2D_VIRTUAL_SURROUND, 0);
        }
        else
        {
            vWriteDspWORD (ADDR_RC2D_VIRTUAL_SURROUND,
                           uReadDspShmBYTE (B_VSURRFLAG));
        }
        */
#ifdef EFFECT_TRANSPARENT_REINIT //add for transparent reinitial
        vAoutReinitMute(AUD_DEC_MAIN);
#endif
        vSetAOutReinit(AUD_DEC_MAIN);
        break;

    case UOP_DSP_PRO_LOGICII_MODE:
#ifdef EFFECT_TRANSPARENT_REINIT //add for transparent reinitial
        vAoutReinitMute(AUD_DEC_MAIN);
#endif
        vSetAOutReinit(AUD_DEC_MAIN);
        break;

    case UOP_DSP_VIRTUAL_SURROUND_FLAG:
#ifndef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
        bVsurrType = uReadDspShmBYTE (B_VSURRFLAG);
        vWriteDspWORD (ADDR_RC2D_VIRTUAL_SURROUND, (UINT16) bVsurrType);
#else
        wVsurrType = u2ReadDspShmWORD (W_VSURRFLAG);
        vWriteDspWORD (ADDR_RC2D_VIRTUAL_SURROUND, (UINT16) wVsurrType);
#endif
        /*
        // if virtual surround is open -> close prologic
        if (bVsurrType != 0)
        {
            vWriteDspWORD (ADDR_RC2D_PRO_LOGIC_II_CONFIG, 0);
        }
        else
        {
            // retrieve back pl2 config
            vWriteDspWORD (ADDR_RC2D_PRO_LOGIC_II_CONFIG,
                           u2ReadDspShmWORD (W_PLIICONFIG));
        }
        */
//#ifdef EFFECT_TRANSPARENT_REINIT //add for transparent reinitial
#if 1 //by gallen, use effect re-init to avoud soft-mute phenomena on by-pass channel
        vSetEffectReinit(AUD_DEC_MAIN);
#else
        vSetAOutReinit(AUD_DEC_MAIN);
#endif
        break;

    case UOP_DSP_VIRTUAL_SURROUND_GAIN:
        break;

    case UOP_DSP_VIRTUAL_SURROUND_WIDE:
        break;

    case UOP_DSP_VIRTUAL_SURROUND_DELAY:
        break;

    case UOP_DSP_REVERB_FLAG:
        break;

    case UOP_DSP_REVERB_GAIN:
        break;

    case UOP_DSP_SRS_WOW_FLAG:      //WOW
    case UOP_DSP_SRS_TSXT_FLAG:  //TSXT2CH
        if(AUD_IsSupportSRS())
        {
#ifndef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
            vWriteDspWORD (ADDR_RC2D_VIRTUAL_SURROUND, (UINT16)(uReadDspShmBYTE(B_VSURRFLAG)));
#else
            vWriteDspWORD (ADDR_RC2D_VIRTUAL_SURROUND, (UINT16)(u2ReadDspShmWORD(W_VSURRFLAG)));
#endif
        //#if 1//def EFFECT_TRANSPARENT_REINIT //add for transparent reinitial 
#ifdef EFFECT_TRANSPARENT_REINIT //add for transparent reinitial 
            vSetEffectReinit(AUD_DEC_MAIN);
#else
            vSetAOutReinit(AUD_DEC_MAIN);
            LOG(9, "\n\n TSHD reinit\n\n ");
#endif
        }
        break;

#ifdef CC_AUD_BBE_SUPPORT
    case UOP_DSP_BBE_MODE:
        bVsurrType = uReadDspShmBYTE (B_BBE_FLAG);
        u4DramAdrIdx = dReadDspCommDram(ADDR_RC2D_BBE_BASE) + ADDR_BBE_FLAG;
        vWriteDspWORD (u4DramAdrIdx, (UINT16) bVsurrType);
#ifdef EFFECT_TRANSPARENT_REINIT //add for transparent reinitial
        vSetEffectReinit(AUD_DEC_MAIN);
#else
        vSetAOutReinit(AUD_DEC_MAIN);
#endif
        break;
#endif

     case UOP_DSP_NEW_MTK_VSURR:
        WriteDspCommDram (ADDR_VSURR_CROS_PARA1,u4ReadDspShmDWRD(D_VSURR_CROS_TALK)); //0x8400
        WriteDspCommDram (ADDR_VSURR_CROS_PARA2,u4ReadDspShmDWRD(D_VSURR_WIDTH));  //0x8401
        WriteDspCommDram (ADDR_VSURR_CROS_PARA3,u4ReadDspShmDWRD(D_VSURR_LR_GAIN));  //0x8402
        WriteDspCommDram (ADDR_VSURR_GAIN_MPY,u4ReadDspShmDWRD(D_VSURR_CLARITY));  //0x8406
        WriteDspCommDram ((ADDR_VSURR_BASS_COEF + 3),u4ReadDspShmDWRD(D_VSURR_FO)); //0x843c
        WriteDspCommDram (ADDR_VSURR_CROS_PARA4,u4ReadDspShmDWRD(D_VSURR_BASS_GAIN));  //0x843e
        WriteDspCommDram (ADDR_VSURR_CROS_PARA5,u4ReadDspShmDWRD(D_VSURR_OUTPUT_GAIN)); //0x843f
        break;

    case UOP_DSP_AD_FLAG:       //Audio Description
        bVsurrType = uReadDspShmBYTE (B_ADFLAG); //bit 7
        vWriteDspWORD (ADDR_RC2D_AD_FLAG, (UINT16)(bVsurrType));
#ifdef CC_AUD_SUPPORT_MS10
        vWriteDspWORD (ADDR_RC2D_SYSTEM_CTRL,    u2ReadDspShmWORD (W_SYSTEM_CTRL));
#endif
        #ifdef EFFECT_TRANSPARENT_REINIT //add for transparent reinitial
        vSetEffectReinit(AUD_DEC_MAIN);
        #else
        vSetAOutReinit(AUD_DEC_MAIN);
        #endif
        break;
#ifdef CC_AUD_SKYPE_SUPPORT
    case UOP_DSP_AUX_MIX_MAIN:       //Aux decoder's output mix into Main decoder
        vWriteDspWORD (ADDR_RC2D_SYSTEM_CTRL,    u2ReadDspShmWORD (W_SYSTEM_CTRL));
        vWriteDspWORD (ADDR_RC2D_SYSTEM_CTRL2,    u2ReadDspShmWORD (W_SYSTEM_CTRL2));
        vWriteDspWORD (ADDR_RD2D_INTERNAL_BUF_IN_FLAG, u2ReadDspShmWORD (W_INTERNAL_BUF_IN_FLAG));
        #ifdef EFFECT_TRANSPARENT_REINIT //add for transparent reinitial
        vSetEffectReinit(AUD_DEC_MAIN);
        #else
        vSetAOutReinit(AUD_DEC_MAIN);
        #endif
        break;
#endif
/*=================== Secondary decoder ======================================*/
    case UOP_DSP_VIRTUAL_SURROUND_FLAG_DEC2:
        break;

    case UOP_DSP_REVERB_FLAG_DEC2:
        break;

    case UOP_DSP_REVERB_GAIN_DEC2:
        break;

    case UOP_DSP_VIRTUAL_SURROUND_GAIN_DEC2:
        break;

    case UOP_DSP_VIRTUAL_SURROUND_WIDE_DEC2:
        break;

    case UOP_DSP_VIRTUAL_SURROUND_DELAY_DEC2:
        break;

    default:
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
        break;
    }
}

void vEQUOP (UINT16 wDspUop)
{

    UINT32 u4DramAdrBase;

    UNUSED(u4DramAdrBase);

    switch (wDspUop)
    {
    case UOP_DSP_EQUALIZER_FLAG:
#ifdef	EFFECT_TRANSPARENT_REINIT
		//vChangeEQGainWithMute(TRUE);
#endif
        vWriteDspWORD (ADDR_RC2D_EQUALIZER_FLAG, uReadDspShmBYTE (B_EQFLAG));
//#ifdef	EFFECT_TRANSPARENT_REINIT
#if 1  //by gallen, use effect re-init to avoud soft-mute phenomena on by-pass channel
        vSetEffectReinit(AUD_DEC_MAIN);
#else
        vSetAOutReinit(AUD_DEC_MAIN);
#endif
        break;
    case UOP_DSP_EQUALIZER_CHANNEL_GAIN:
        WriteDspCommDram (ADDR_RC2D_EQUALIZER_DRY_LEVEL,
            (u4ReadDspShmDWRD (D_EQ_DRY)));
        break;
    case UOP_DSP_EQUALIZER_CHANNEL_CHANGE:
        WriteDspCommDram (ADDR_RC2D_EQUALIZER_DRY_LEVEL,
            (u4ReadDspShmDWRD (D_EQ_DRY)));
        // Band Gain
        vEQBandGainFlush(AUD_DEC_MAIN);
        // Enable update flag
        vWriteDspWORD(ADDR_RC2D_EQUALIZER_TABLE_CHANGE, 0x1);
        break;
    case UOP_DSP_SUPER_BASS_DELAY:
        break;
    case UOP_DSP_SUPER_BASS_BOOST_GAIN:
        // Band Gain
        vEQ2SbassBandGainFlush(AUD_DEC_MAIN);
        // Enable update flag
        vWriteDspWORD(ADDR_RC2D_SUPER_BASS_TABLE_CHANGE, 0x1);
        break;
    case UOP_DSP_SUPER_BASS_CLEAR_GAIN:
        // Band Gain
        vEQ2SbassBandGainFlush(AUD_DEC_MAIN);
        // Enable update flag
        vWriteDspWORD(ADDR_RC2D_SUPER_BASS_TABLE_CHANGE, 0x1);
        break;
    case UOP_DSP_EQUALIZER_FLAG_DEC2:
        break;
    case UOP_DSP_EQUALIZER_CHANNEL_GAIN_DEC2:
        break;
    case UOP_DSP_EQUALIZER_CHANNEL_CHANGE_DEC2:
        break;
    case UOP_DSP_SUPER_BASS_DELAY_DEC2:
        break;
    case UOP_DSP_SUPER_BASS_BOOST_GAIN_DEC2:
        break;
    case UOP_DSP_SUPER_BASS_CLEAR_GAIN_DEC2:
        break;
#ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
    case UOP_DSP_CDNOTCH_FLAG:
        u4DramAdrBase = dReadDspCommDram(ADDR_RC2D_CDNOTCH_BASE);
        WriteDspCommDram ((u4DramAdrBase+ADDR_CDNOTCH_USER_Q), uReadDspShmBYTE(B_CDNOTCH_USER_Q));
        WriteDspCommDram ((u4DramAdrBase+ADDR_CDNOTCH_USER_FC), u2ReadDspShmWORD(W_CDNOTCH_USER_FC));
        // Setup CDNOTCH enable bit
	    vWriteDspWORD (ADDR_RC2D_NEWPOST_FLAG,
		    ((wReadDspWORD(ADDR_RC2D_NEWPOST_FLAG)) & ~NEWPOST_FLAG_CDNOTCH_BIT) |
		    ((uReadDspShmBYTE(B_CDNOTCH_FLAG) > 0) ? NEWPOST_FLAG_CDNOTCH_BIT : 0));
        vSetAOutReinit(AUD_DEC_MAIN);
        break;
    case UOP_DSP_CDNOTCH_CFG:
        u4DramAdrBase = dReadDspCommDram(ADDR_RC2D_CDNOTCH_BASE);
        WriteDspCommDram ((u4DramAdrBase+ADDR_CDNOTCH_USER_Q), uReadDspShmBYTE(B_CDNOTCH_USER_Q));
        WriteDspCommDram ((u4DramAdrBase+ADDR_CDNOTCH_USER_FC), u2ReadDspShmWORD(W_CDNOTCH_USER_FC));
        vSetAOutReinit(AUD_DEC_MAIN);
        break;
#endif
#ifdef  KARAOKE_SUPPORT
    case UOP_DSP_KEYSHIFT_FLAG:
        // Setup keyshift enable bit
	    vWriteDspWORD (ADDR_RC2D_NEWPOST_FLAG,
		    ((wReadDspWORD(ADDR_RC2D_NEWPOST_FLAG)) & ~NEWPOST_FLAG_KEYSHIFT_BIT) |
		    ((uReadDspShmBYTE(B_KEYSHIFT_FLAG) > 0) ? NEWPOST_FLAG_KEYSHIFT_BIT : 0));
        vSetAOutReinit(AUD_DEC_MAIN);
        break;
    case UOP_DSP_KEYSHIFT_KEY:
        WriteDspCommDram ((ADDR_RC2D_KEYSHIFT_BASE+ADDR_KEYSHIFT_KEY), (uReadDspShmBYTE(B_KEYSHIFT_KEY)<<24)>>16);
        vSetAOutReinit(AUD_DEC_MAIN);
        break;
    case UOP_DSP_KEYSHIFT_FLAG_DEC2:
        // Setup keyshift enable bit
	    /*vWriteDspWORD (ADDR_RC2D_NEWPOST_FLAG_DEC2,
		    ((wReadDspWORD(ADDR_RC2D_NEWPOST_FLAG_DEC2)) & ~NEWPOST_FLAG_KEYSHIFT_BIT) |
		    ((uReadDspShmBYTE(B_KEYSHIFT_FLAG_DEC2) > 0) ? NEWPOST_FLAG_KEYSHIFT_BIT : 0));*/
        vSetAOutReinit(AUD_DEC_AUX);
        break;
    case UOP_DSP_KEYSHIFT_KEY_DEC2:
        WriteDspCommDram ((ADDR_RC2D_KEYSHIFT_BASE+ADDR_KEYSHIFT_KEY_DEC2), (uReadDspShmBYTE(B_KEYSHIFT_KEY_DEC2)<<24)>>16);
        vSetAOutReinit(AUD_DEC_AUX);
        break;
#endif
    default:
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
        break;
    }
}

/***************************************************************************
     Function : vKaraokeUOP
  Description : None
    Parameter : None
    Return    : None
***************************************************************************/
void vKaraokeUOP (UINT16 wDspUop)
{
    //UINT16 u2DspData;

    switch (wDspUop)
    {
    case UOP_DSP_KARAOKE_FLAG:
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;
        _uDspSIntAddr = INT_RC2D_KARAOKE_FLAG;
        _u4DspSIntSD = ((UINT32) uReadDspShmBYTE (B_KARAFLAG) & 0x7) << 8;
        // Trigger aout re init because ac3 need aout reinit to check post proc
        // to see if the downmix is done by ac3 or post
        // otherwise the dual mono will have a problem that the analog is main
        // but the downmix is main + sub
        vDspCmd(UOP_DSP_AOUT_REINIT);
        break;
    case UOP_DSP_LR_MIX_RATIO:
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;
        _uDspSIntAddr = INT_RC2D_LRCH_MIX_RATIO;
        _u4DspSIntSD = u4ReadDspShmDWRD (D_LRMIXRATIO);
        break;
    case UOP_DSP_KARAOKE_FLAG_DEC2:
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;
        _uDspSIntAddr = INT_RC2D_KARAOKE_FLAG_DEC2;
        _u4DspSIntSD = ((UINT32) uReadDspShmBYTE (B_KARAFLAG_DEC2) & 0x7) << 8;
        // Trigger aout re init because ac3 need aout reinit to check post proc
        // to see if the downmix is done by ac3 or post
        // otherwise the dual mono will have a problem that the analog is main
        // but the downmix is main + sub
        vDspCmd(UOP_DSP_AOUT_REINIT_DEC2);
        break;
    case UOP_DSP_LR_MIX_RATIO_DEC2:
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;
        _uDspSIntAddr = INT_RC2D_LRCH_MIX_RATIO_DEC2;
        _u4DspSIntSD = u4ReadDspShmDWRD (D_LRMIXRATIO_DEC2);
        break;
    default:
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
        break;
    }
}

/***************************************************************************
     Function : vPinkUOP
  Description : None
    Parameter : None
    Return    : None
***************************************************************************/
void vPinkUOP (UINT16 wDspUop)
{
    UINT16 u2DspData;

    switch (wDspUop)
    {
    case UOP_DSP_PINK_NOISE_OFF:
        vDspCmd (DSP_STOP);
        break;
    case UOP_DSP_PINK_NOISE_CHANGE:
        if (_rDspStatus[AUD_DEC_MAIN].fgDspPlay)
        {
            _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;
            _uDspSIntAddr = INT_RC2D_CHANNEL_USE;
            u2DspData = u2ReadDspShmWORD (W_PINKNOISE);
            vWriteDspWORD (ADDR_RC2D_CHANNEL_USE, u2DspData);
            _u4DspSIntSD = (u2DspData) << 8;
        }
        else
        {
            if (_rDspVars[AUD_DEC_MAIN].bDspStrTyp != PINK_NOISE_STREAM)
            {
                _rDspVars[AUD_DEC_MAIN].bDspStrTyp = PINK_NOISE_STREAM;
                vDspLoadRamCode (AUD_PINK_DEC1);
            }
            vDspCmd (DSP_PLAY);
        }
        break;
    default:
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
        ;
    }
}

/***************************************************************************
     Function : vGrp5UOP
  Description : None
    Parameter : None
    Return    : None
***************************************************************************/
void vGrp5UOP (UINT16 wDspUop)
{
    UINT16 u2DspData;
    //UINT32 u4Data;

    switch (wDspUop)
    {
        // first decoder
    case UOP_DSP_IEC_FLAG_DOWNMIX:
#ifdef EFFECT_TRANSPARENT_REINIT //add for transparent reinitial
        vAoutReinitMute(AUD_DEC_MAIN);
#endif
        vSetSpeakerConfig(AUD_DEC_MAIN);
        vDspIECConfig(AUD_DEC_MAIN);
        break;
    case UOP_DSP_IEC_FLAG:
#ifdef EFFECT_TRANSPARENT_REINIT //add for transparent reinitial
        vAoutReinitMute(AUD_DEC_MAIN);
#endif
        vDspIECConfig(AUD_DEC_MAIN);
        break;
    case UOP_DSP_IEC_SET_DATA:
        vDspSetFreq (AUD_DEC_MAIN, _rDspVars[AUD_DEC_MAIN].dwDacFreq, FALSE);
        break;
    case UOP_DSP_SPEED:
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;
        _uDspSIntAddr = INT_RC2D_SPEED;
        _u4DspSIntSD = ((UINT32) u2ReadDspShmWORD (W_SPEED)) << 8;
        break;
    case UOP_DSP_PROCESSING_MODE:
        // This uop can not set the 3th nibble ( bypass group)
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;
        _uDspSIntAddr = INT_RC2D_PROCESSING_MODE;
        u2DspData = u2ReadDspShmWORD (W_PROCMOD);
        //Check if DRC Auto
        // if Auto Drc -> if has virtual surround or eq or super bass or reverb
        //             -> set drc on
        if (u2DspData & 0x8)
        {
            //Turn off DRC
            u2DspData = u2DspData & 0xFFFD;
            //Check if Virtual Surround On
#ifndef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
            if (uReadDspShmBYTE (B_VSURRFLAG) & 0x9)    // bypass spatializer
#else
            if (u2ReadDspShmWORD(W_VSURRFLAG) & 0x9)    // bypass spatializer
#endif
            {
                u2DspData = u2DspData | 0x2;
            }

            //Check if Equalizer or Super Bass On
            if (uReadDspShmBYTE (B_EQFLAG) & 0xF)
            {
                u2DspData = u2DspData | 0x2;
            }

            //Check if Reverb On
            if (uReadDspShmBYTE (B_REVERBFLAG) & 0x1)
            {
                u2DspData = u2DspData | 0x2;
            }

            vWriteDspShmWORD (W_PROCMOD, u2DspData);
        }
        // if !AC3 ->  do not bass_ch_mixto_lfe in pram
        if (_rDspVars[AUD_DEC_MAIN].bDspStrTyp != AC3_STREAM)
        {
            u2DspData &= 0xffdf;
        }

        DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);

        // Reserved previous bypass setting
        u2DspData =
            (u2DspData & 0x0FFF) | (wReadDspWORD (ADDR_RC2D_PROCESSING_MODE) &
                                   0xF000);
        vWriteDspWORD(ADDR_RC2D_PROCESSING_MODE,u2DspData);
        _u4DspSIntSD = ((UINT32) u2DspData) << 8;
        // Master volume depends on drc -> redo it;
        vDspCmd (UOP_DSP_MASTER_VOLUME);
        break;

  case UOP_DSP_FRAC_SPEED:
    //sunman for play speed
        WriteDspCommDram(ADDR_RC2D_PLAY_SPEED, u4ReadDspShmDWRD(D_PLAY_SPEED));
#ifdef EFFECT_TRANSPARENT_REINIT //add for transparent reinitial
        vSetEffectReinit(AUD_DEC_MAIN);
#else
        vSetAOutReinit(AUD_DEC_MAIN);
#endif
        break;


    case UOP_DSP_BYPASS_PROC_MODE:	//Andrew Wen 061027
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;
        _uDspSIntAddr = INT_RC2D_PROCESSING_MODE;
        u2DspData = u2ReadDspShmWORD (W_PROCMOD);
        DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);
        //bypass group only
        u2DspData =
            (u2DspData & 0xFC00) |
            (wReadDspWORD (ADDR_RC2D_PROCESSING_MODE) & 0x3FF);

        vWriteDspWORD(ADDR_RC2D_PROCESSING_MODE,u2DspData);
        _u4DspSIntSD = ((UINT32) u2DspData) << 8;
        break;

    case UOP_DSP_IEC_DOWN_SAMPLE:
        vDspSetFreq (AUD_DEC_MAIN, _rDspVars[AUD_DEC_MAIN].dwDacFreq, FALSE);
        break;
    // Light added for AVC
    case UOP_DSP_AVC_CONTROL:
        WriteDspCommDram (ADDR_RC2D_AVC_TARGET_LEVEL,
                      (UINT16)(u2ReadDspShmWORD (W_AVC_TARGET_LEV))<<8);
        WriteDspCommDram (ADDR_RC2D_AVC_SILENCE_LEVEL,
                      u2ReadDspShmWORD (W_AVC_SILENCE_LEV)<<8);
        WriteDspCommDram (ADDR_RC2D_AVC_MAX_GAIN_UP,
                      u2ReadDspShmWORD (W_AVC_MAX_GAIN_UP)<<8);
        WriteDspCommDram (ADDR_RC2D_AVC_MAX_GAIN_DOWN,
                      u2ReadDspShmWORD (W_AVC_MAX_GAIN_DOWN)<<8);
#ifdef DSP_SUPPORT_SRSVIQ
        WriteDspCommDram (ADDR_RC2D_AVC_FLAG,
                     ( u2ReadDspShmWORD (W_AVC_FLAG)&1)<<8);
#else
        WriteDspCommDram (ADDR_RC2D_AVC_FLAG,
                      u2ReadDspShmWORD (W_AVC_FLAG)<<8);
#endif
        WriteDspCommDram (ADDR_RC2D_AVC_ATTACK_THRES,
                      u2ReadDspShmWORD (W_AVC_ATTACK_THRES)<<8);
        WriteDspCommDram (ADDR_RC2D_AVC_ADJUST_RATE,
                      u2ReadDspShmWORD (W_AVC_ADJUST_RATE));
		vWriteDspShmWORD (W_AVC_FLAG, u2ReadDspShmWORD (W_AVC_FLAG) & 0x1F);  // Clear AVC_FLAG[5] AVC slow mode.
		                                                                      // AVC slow mode is only used when AVC switched from Off to On.
		                                                                      // We clear this bit for channel changing, which used AVC fast mode.
		u2DspData = u2ReadDspShmWORD (W_DSP_AVC_UI_ADJUST_RATE); // UI_ADJUST_RATE @ 32k Hz
		if (_rDspVars[AUD_DEC_MAIN].dwTableFreq == SAMPLE_44K)
		{
			u2DspData = (u2DspData * 32) / 44;
		}
		else if (_rDspVars[AUD_DEC_MAIN].dwTableFreq == SAMPLE_48K)
		{
			u2DspData = (u2DspData * 2) / 3;
		}
        WriteDspCommDram (ADDR_RC2D_AVC_UI_ADJUST_RATE, u2DspData);
#ifdef DSP_SUPPORT_SRSVIQ
        //sunman_viq
        WriteDspCommDram (ADDR_RC2D_AVC_REFERENCE_LEVEL, u4ReadDspShmDWRD (D_VIQ_REF_LVL));
        WriteDspCommDram (ADDR_RC2D_AVC_MODE, u4ReadDspShmDWRD (D_VIQ_MODE));
        WriteDspCommDram (ADDR_RC2D_AVC_MAX_GAIN, u4ReadDspShmDWRD (D_VIQ_MAX_GAIN));
        WriteDspCommDram (ADDR_RC2D_AVC_INPUT_GAIN, u4ReadDspShmDWRD (D_VIQ_INPUT_GAIN));
        WriteDspCommDram (ADDR_RC2D_AVC_OUTPUT_GAIN, u4ReadDspShmDWRD (D_VIQ_OUTPUT_GAIN));
        WriteDspCommDram (ADDR_RC2D_AVC_NOISE_TH, u4ReadDspShmDWRD (D_VIQ_NOISE_TH));   //sunman_viq2

        switch( (dReadDspCommDram(ADDR_RC2D_AVC_MODE) & 0x3000)>>12 )
        {
        case 0://Light
            LOG(7, "\n\n L dwCoefPnt=%x, dwFacPnt=%x\n\n ", ADDR_SRS_VIQ_COEF1, ADDR_SRS_VIQ_G_FACTOR);
            WriteDspCommDram (ADDR_RC2D_AVC_MAX_GAIN,0x10000 );
            WriteDspCommDram (ADDR_SRS_VIQ_COEF1, 0x4);  //kSRSVolumeControlHLCoeff1Light
            WriteDspCommDram (ADDR_SRS_VIQ_COEF2, 0x2);   //kSRSVolumeControlHLCoeff2Light
            WriteDspCommDram (ADDR_SRS_VIQ_COEF3, 0x3);   //kSRSVolumeControlHLCoeff3Light
            WriteDspCommDram (ADDR_SRS_VIQ_G_FACTOR,0x6 );   //gfactor
        	 break;
        case 1://Normal
            LOG(7,"\n\n N dwCoefPnt=%x, dwFacPnt=%x\n\n ", ADDR_SRS_VIQ_COEF1, ADDR_SRS_VIQ_G_FACTOR);
            WriteDspCommDram (ADDR_RC2D_AVC_MAX_GAIN,0x20000 );
            WriteDspCommDram (ADDR_SRS_VIQ_COEF1, 0x8);
            WriteDspCommDram (ADDR_SRS_VIQ_COEF2, 0x4);
            WriteDspCommDram (ADDR_SRS_VIQ_COEF3, 0x6);
            WriteDspCommDram (ADDR_SRS_VIQ_G_FACTOR, 0x9);
            break;
        case 2://Heavy
            LOG(7,"\n\n H dwCoefPnt=%x, dwFacPnt=%x\n\n ", ADDR_SRS_VIQ_COEF1, ADDR_SRS_VIQ_G_FACTOR);
            WriteDspCommDram (ADDR_RC2D_AVC_MAX_GAIN, 0x40000);
            WriteDspCommDram (ADDR_SRS_VIQ_COEF1, 0xe);
            WriteDspCommDram (ADDR_SRS_VIQ_COEF2, 0x7);
            WriteDspCommDram (ADDR_SRS_VIQ_COEF3, 0xb);
            WriteDspCommDram (ADDR_SRS_VIQ_G_FACTOR,0xd );
            break;
        }
#if 0 //DSP_SUPPORT_SRS_SSHD
        if( dReadDspCommDram (ADDR_SRS_STUDIO_FLAG) != u2ReadDspShmWORD (W_SRS_STUDIO_FLAG) )
        {
               WriteDspCommDram (ADDR_SRS_STUDIO_FLAG, u2ReadDspShmWORD (W_SRS_STUDIO_FLAG));
               // vSetAOutReinit (TRUE);
        }
#endif
      vSetAOutReinit (AUD_DEC_MAIN);
#endif
    	 break;

       case UOP_DSP_DOWNMIX_POSITION:
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;
        _uDspSIntAddr = INT_RC2D_SYSTEM_SETUP;
        _u4DspSIntSD = ((UINT32) u2ReadDspShmWORD (W_SYSTEM_SETUP)) << 8;
	    vWriteDspWORD (ADDR_RC2D_SYSTEM_SETUP,   u2ReadDspShmWORD (W_SYSTEM_SETUP));
        break;

        // second decoder
    case UOP_DSP_IEC_FLAG_DOWNMIX_DEC2:
#ifdef EFFECT_TRANSPARENT_REINIT //add for transparent reinitial
        vAoutReinitMute(AUD_DEC_AUX);
#endif
        vSetSpeakerConfig(AUD_DEC_AUX);
        vDspIECConfig(AUD_DEC_AUX);
        break;
    case UOP_DSP_IEC_FLAG_DEC2:
#ifdef EFFECT_TRANSPARENT_REINIT //add for transparent reinitial
        vAoutReinitMute(AUD_DEC_AUX);
#endif
        vDspIECConfig (AUD_DEC_AUX);
        break;
    case UOP_DSP_SPEED_DEC2:
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;
        _uDspSIntAddr = INT_RC2D_SPEED_DEC2;
        _u4DspSIntSD = ((UINT32) u2ReadDspShmWORD (W_SPEED_DEC2)) << 8;
        break;
    case UOP_DSP_PROCESSING_MODE_DEC2:
        // This uop can not set the 3th nibble ( bypass group)
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;
        _uDspSIntAddr = INT_RC2D_PROCESSING_MODE_DEC2;
        u2DspData = u2ReadDspShmWORD (W_PROCMOD_DEC2);
        //Check if DRC Auto
        // if Auto Drc -> if has virtual surround or eq or super bass or reverb
        //             -> set drc on
        if (u2DspData & 0x8)
        {
            //Turn off DRC
            u2DspData = u2DspData & 0xFFFD;
            //Check if Virtual Surround On
#ifndef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
            if (uReadDspShmBYTE (B_VSURRFLAG_DEC2) & 0x9)    // bypass spatializer
#else
            if (u2ReadDspShmWORD (W_VSURRFLAG_DEC2) & 0x9)    // bypass spatializer
#endif
            {
                u2DspData = u2DspData | 0x2;
            }

            //Check if Equalizer or Super Bass On
            if (uReadDspShmBYTE (B_EQFLAG_DEC2) & 0xF)
            {
                u2DspData = u2DspData | 0x2;
            }

            //Check if Reverb On
            if (uReadDspShmBYTE (B_REVERBFLAG_DEC2) & 0x1)
            {
                u2DspData = u2DspData | 0x2;
            }

            vWriteDspShmWORD (W_PROCMOD_DEC2, u2DspData);
        }
        // if !AC3 ->  do not bass_ch_mixto_lfe in pram
        if (_rDspVars[AUD_DEC_AUX].bDspStrTyp != AC3_STREAM)
        {
            u2DspData &= 0xffdf;
        }

        // Reserved previous bypass setting
        u2DspData =
            (u2DspData & 0x0FFF) |
            (wReadDspWORD (ADDR_RC2D_PROCESSING_MODE_DEC2) & 0xF000);
        vWriteDspWORD(ADDR_RC2D_PROCESSING_MODE_DEC2,u2DspData);
        _u4DspSIntSD = ((UINT32) u2DspData) << 8;
        // Master volume depends on drc -> redo it;
        vDspCmd (UOP_DSP_MASTER_VOLUME_DEC2);
        break;

    case UOP_DSP_BYPASS_PROC_MODE_DEC2:
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;
        _uDspSIntAddr = INT_RC2D_PROCESSING_MODE_DEC2;
        u2DspData = u2ReadDspShmWORD (W_PROCMOD_DEC2);
        //bypass group only
        u2DspData =
            (u2DspData & 0xFC00) |
            (wReadDspWORD (ADDR_RC2D_PROCESSING_MODE_DEC2) & 0x3FF);
        vWriteDspWORD(ADDR_RC2D_PROCESSING_MODE_DEC2,u2DspData);
        _u4DspSIntSD = ((UINT32) u2DspData) << 8;
        break;

    case UOP_DSP_IEC_DOWN_SAMPLE_DEC2:
        vDspSetFreq (AUD_DEC_AUX, _rDspVars[AUD_DEC_AUX].dwDacFreq, FALSE);
        break;
    case UOP_DSP_AOUT_REINIT:
#ifdef EFFECT_TRANSPARENT_REINIT //add for transparent reinitial
        vAoutReinitMute(AUD_DEC_MAIN);
#endif
        vSetAOutReinit(AUD_DEC_MAIN);
        break;
    case UOP_DSP_AOUT_REINIT_DEC2:
#ifdef EFFECT_TRANSPARENT_REINIT //add for transparent reinitial
        vAoutReinitMute(AUD_DEC_AUX);
#endif
        vSetAOutReinit(AUD_DEC_AUX);
        break;
    case UOP_DSP_SET_STC_DIFF_BOUND:
        WriteDspCommDram (ADDR_RC2D_STC_DIFF_LOBOUND, u4ReadDspShmDWRD (D_STC_DIFF_LO));
        WriteDspCommDram (ADDR_RC2D_STC_DIFF_HIBOUND, u4ReadDspShmDWRD (D_STC_DIFF_HI));
        WriteDspCommDram (ADDR_RC2D_STC_DIFF_WSBOUND, u4ReadDspShmDWRD (D_STC_DIFF_WS));
        break;

#ifdef CC_MT5391_AUD_3_DECODER
    case UOP_DSP_BYPASS_PROC_MODE_DEC3:
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;
        _uDspSIntAddr = INT_RC2D_PROCESSING_MODE_DEC3;
        u2DspData = u2ReadDspShmWORD (W_PROCMOD_DEC3);
        //bypass group only
        u2DspData =
            (u2DspData & 0xFC00) |
            (wReadDspWORD (ADDR_RC2D_PROCESSING_MODE_DEC3) & 0x3FF);

        vWriteDspWORD(ADDR_RC2D_PROCESSING_MODE_DEC3,u2DspData);
        _u4DspSIntSD = ((UINT32) u2DspData) << 8;
        break;
#endif
#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
#ifdef CC_AUD_4_DECODER_SUPPORT
    case UOP_DSP_BYPASS_PROC_MODE_DEC4:
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;
        _uDspSIntAddr = INT_RC2D_PROCESSING_MODE_DEC4;
        u2DspData = u2ReadDspShmWORD (W_PROCMOD_DEC4);
        //bypass group only
        u2DspData =
            (u2DspData & 0xFC00) |
            (wReadDspWORD (ADDR_RC2D_PROCESSING_MODE_DEC4) & 0x3FF);

        vWriteDspWORD(ADDR_RC2D_PROCESSING_MODE_DEC4,u2DspData);
        _u4DspSIntSD = ((UINT32) u2DspData) << 8;
        break;
#endif
#endif

    case UOP_DSP_SET_STC_ID:   // 2_STC_Task
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;
        _uDspSIntAddr = INT_RC2D_SYSTEM_SETUP;
        _u4DspSIntSD = ((UINT32) u2ReadDspShmWORD (W_SYSTEM_SETUP)) << 8;
        vWriteDspWORD (ADDR_RC2D_SYSTEM_SETUP,   u2ReadDspShmWORD (W_SYSTEM_SETUP));
        break;
    case UOP_DSP_SET_STC_ID_DEC2:
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;
        _uDspSIntAddr = INT_RC2D_SYSTEM_SETUP_DEC2;
        _u4DspSIntSD = ((UINT32) u2ReadDspShmWORD (W_SYSTEM_SETUP_DEC2)) << 8;
        vWriteDspWORD (ADDR_RC2D_SYSTEM_SETUP_DEC2,   u2ReadDspShmWORD (W_SYSTEM_SETUP_DEC2));
        break;
#ifdef CC_MT5391_AUD_3_DECODER        
    case UOP_DSP_SET_STC_ID_DEC3:
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;
        _uDspSIntAddr = INT_RC2D_SYSTEM_SETUP_DEC3;
        _u4DspSIntSD = ((UINT32) u2ReadDspShmWORD (W_SYSTEM_SETUP_DEC3)) << 8;
        vWriteDspWORD (ADDR_RC2D_SYSTEM_SETUP_DEC3,   u2ReadDspShmWORD (W_SYSTEM_SETUP_DEC3));
        break;
#endif        

    default:
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
        break;
    }
}

/***************************************************************************
     Function : vFlowControlUOP
  Description : None
    Parameter : None
    Return    : None
***************************************************************************/
void vChanConfigUOP (UINT16 wDspUop)
{
    UINT32 dwDspInfo;
    UINT8  uIdx;

    //UINT16 u2DspData;
    if(wDspUop < UOP_DSP_CONFIG_SPEAKER_DEC2)
    {
#ifdef EFFECT_TRANSPARENT_REINIT //add for transparent reinitial
        vAoutReinitMute(AUD_DEC_MAIN);
#endif
        vSetAOutReinit(AUD_DEC_MAIN);
    }
    else
    {
#ifdef EFFECT_TRANSPARENT_REINIT //add for transparent reinitial
        vAoutReinitMute(AUD_DEC_AUX);
#endif
        vSetAOutReinit(AUD_DEC_AUX);
    }

    switch (wDspUop)
    {
        // first decoder
    case UOP_DSP_CHANGE_SPK_SIZE:
        break;  // only do aout re-init
    case UOP_DSP_CONFIG_SPEAKER:
        vSetSpeakerConfig(AUD_DEC_MAIN);
        break;
    case UOP_DSP_CONFIG_DELAY_ALL:      //Andrew Wen 2007/6/6 simplify all channel delay
        for (uIdx=0; uIdx<=AUD_CH_DMX_RIGHT; uIdx++)
        {
            WriteDspCommDram ((ADDR_RC2D_CH_DELAY_C + uIdx),
                          u2ReadDspShmWORD ((W_CHDELAY_C + (uIdx << 1))));
        }
        break;
    case UOP_DSP_CONFIG_DELAY_C:
    case UOP_DSP_CONFIG_DELAY_L:
    case UOP_DSP_CONFIG_DELAY_R:
    case UOP_DSP_CONFIG_DELAY_LS:
    case UOP_DSP_CONFIG_DELAY_RS:
    case UOP_DSP_CONFIG_DELAY_CH7:
    case UOP_DSP_CONFIG_DELAY_CH8:
    case UOP_DSP_CONFIG_DELAY_SUBWOOFER:
    case UOP_DSP_CONFIG_DELAY_CH9:
    case UOP_DSP_CONFIG_DELAY_CH10:
        dwDspInfo =
            (UINT16)((0xFF00 & wDspUop) - (0xFF00 & UOP_DSP_CONFIG_DELAY_C)) >> 8;
        WriteDspCommDram ((ADDR_RC2D_CH_DELAY_C + dwDspInfo),
                          u2ReadDspShmWORD ((W_CHDELAY_C + (dwDspInfo << 1))));
        break;
        // second decoder
    case UOP_DSP_CONFIG_SPEAKER_DEC2:
        vSetSpeakerConfig(AUD_DEC_AUX);
        break;
    case UOP_DSP_CONFIG_DELAY_C_DEC2:
    case UOP_DSP_CONFIG_DELAY_L_DEC2:
    case UOP_DSP_CONFIG_DELAY_R_DEC2:
        dwDspInfo =
            (UINT16)((0xFF00 & wDspUop) -
                          (0xFF00 & UOP_DSP_CONFIG_DELAY_C_DEC2)) >> 8;
        WriteDspCommDram ((ADDR_RC2D_CH_DELAY_C_DEC2 + dwDspInfo),
                          u2ReadDspShmWORD ((W_CHDELAY_C_DEC2 +
                                           (dwDspInfo << 1))));
       _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
       break;
    default:
       _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
       break;
    }
}

/***************************************************************************
     Function : vFlowControlUOP
  Description : None
    Parameter : None
    Return    : None
***************************************************************************/
void vFlowControlUOP (UINT16 wDspUop)
{
    _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;
    _uDspSIntAddr = INT_RC2D_FLOW_CONTROL;
    switch (wDspUop)
    {
    case DSP_PLAY:
        vDspPlayCommonInit ();  // decoder initial before playing
        _u4DspSIntSD = (UINT32) FLOW_CONTROL_PLAY;
        //AUD_UopCommandDone(0, wDspUop);
        LOG(7, "[DSP] Dec(0) change state -> Play -------------------\n");
        break;
    case DSP_STOP:
        if (!_rDspStatus[AUD_DEC_MAIN].fgDspStop)
        {
            //vAoutReinitMute(AUD_DEC_MAIN); //for no fade out problem w/ channel delay      //--->20111208 removed
            _u4DspSIntSD = (UINT32) (FLOW_CONTROL_STOP + FLOW_CONTROL_FLUSH);
        }
#ifdef DATA_DISC_WMA_SUPPORT
        else if ((_rDspVars[AUD_DEC_MAIN].bDspStrTyp == WMA_STREAM) ||
                   (_rDspVars[AUD_DEC_MAIN].bDspStrTyp == WMAPRO_STREAM))
        {
            //koro: WMA might stop before issuing stop
            AUD_UopCommandDone(AUD_DEC_MAIN,DSP_STOP);    //unlock sema
            _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;    //don't send interrupt
        }
#endif
        //AUD_UopCommandDone(0, wDspUop);
        LOG(7, "[DSP] Dec(0) change state -> Stop -------------------\n");
        break;
    case DSP_PLAY_DEC2:
        vDspPlayCommonInitDec2 ();
        _uDspSIntAddr = INT_RC2D_FLOW_CONTROL_DEC2;
        _u4DspSIntSD = (UINT32) FLOW_CONTROL_PLAY;
        //AUD_UopCommandDone(1, wDspUop);
        LOG(7, "[DSP] Dec(1) change state -> Play -------------------\n");
        break;
    case DSP_STOP_DEC2:
        if (!_rDspStatus[AUD_DEC_AUX].fgDspStop)
        {
#ifdef EFFECT_TRANSPARENT_REINIT //add for transparent reinitial
            vAoutReinitMute(AUD_DEC_AUX);
#endif
            _uDspSIntAddr = INT_RC2D_FLOW_CONTROL_DEC2;
            _u4DspSIntSD = (UINT32) (FLOW_CONTROL_STOP + FLOW_CONTROL_FLUSH);
        }
        //AUD_UopCommandDone(1, wDspUop);
        LOG(7, "[DSP] Dec(1) change state -> Stop -------------------\n");
        break;
#ifdef CC_MT5391_AUD_3_DECODER
    case DSP_PLAY_DEC3:
  #ifdef MP3ENC_DBG
        if(_rDspVars[AUD_DEC_THIRD].bDspStrTyp == MP3ENC_STREAM)
        {
          _u4DspSIntSD = (UINT32) ENC_CONTROL_RECORD;
          LOG(7, "[DSP] Dec(1) run MP3 Encoder\n");
        }
  #endif
        vDspPlayCommonInitDec3 ();
        _uDspSIntAddr = INT_RC2D_FLOW_CONTROL_DEC3;
        _u4DspSIntSD = (UINT32) FLOW_CONTROL_PLAY;
        //AUD_UopCommandDone(2, wDspUop);
        LOG(7, "[DSP] Dec(2) change state -> Play -------------------\n");
        break;
    case DSP_STOP_DEC3:
        if (!_rDspStatus[AUD_DEC_THIRD].fgDspStop)
        {
            _uDspSIntAddr = INT_RC2D_FLOW_CONTROL_DEC3;
            _u4DspSIntSD = (UINT32) (FLOW_CONTROL_STOP + FLOW_CONTROL_FLUSH);
        }
        //AUD_UopCommandDone(2, wDspUop);
        LOG(7, "[DSP] Dec(2) change state -> Stop -------------------\n");
        break;

    #ifdef MP3ENC_SUPPORT
    case DSP_MP3ENC_HIGHSPEED_DEC3:
        _uDspSIntAddr = INT_RC2D_FLOW_CONTROL_DEC2;
        _u4DspSIntSD = (UINT32) ENC_CONTROL_HIGHSPEED;
        LOG(7, "[DSP] Dec(1) run MP3 Encoder\n");
        LOG(7, "[DSP] Dec(1) change state -> MP3 High Speed Mode ------------------\n");
        break;
    #endif
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    case DSP_PLAY_DEC4:
        //Printf("DSP_UOP - DSP_PLAY_DEC4\n"); //SBCENC_TEST
        vDspPlayCommonInitDec4 ();
        _uDspSIntAddr = INT_RC2D_FLOW_CONTROL_DEC4;
        _u4DspSIntSD = (UINT32) FLOW_CONTROL_PLAY;
        LOG(7, "[DSP] Dec(3) change state -> Play -------------------\n");
        break;
    case DSP_STOP_DEC4:
        if (!_rDspStatus[AUD_DEC_4TH].fgDspStop)
        {
            _uDspSIntAddr = INT_RC2D_FLOW_CONTROL_DEC4;
            _u4DspSIntSD = (UINT32) (FLOW_CONTROL_STOP + FLOW_CONTROL_FLUSH);
        }
        LOG(7, "[DSP] Dec(3) change state -> Stop -------------------\n");
        break;
#endif
    case DSP_FLUSH:
        _u4DspSIntSD = (UINT32) FLOW_CONTROL_FLUSH;
        LOG(7, "[DSP] Dec(0) change state -> Flush ------------------\n");
        break;
    case DSP_PAUSE:
        _u4DspSIntSD = (UINT32) FLOW_CONTROL_PAUSE;
        LOG(7, "[DSP] Dec(0) change state -> Pause ------------------\n");
        break;
    case DSP_RESUME:
        _u4DspSIntSD = (UINT32) FLOW_CONTROL_RESUME;
        LOG(7, "[DSP] Dec(0) change state -> Resume -----------------\n");
        break;
    case DSP_FLUSH_DEC2:
        _uDspSIntAddr = INT_RC2D_FLOW_CONTROL_DEC2;
        _u4DspSIntSD = (UINT32) FLOW_CONTROL_FLUSH;
        LOG(7, "[DSP] Dec(1) change state -> Flush ------------------\n");
        break;
    case DSP_PAUSE_DEC2:
        _uDspSIntAddr = INT_RC2D_FLOW_CONTROL_DEC2;
        _u4DspSIntSD = (UINT32) FLOW_CONTROL_PAUSE;
        LOG(7, "[DSP] Dec(1) change state -> Pause ------------------\n");
        break;
    case DSP_RESUME_DEC2:
        _uDspSIntAddr = INT_RC2D_FLOW_CONTROL_DEC2;
        _u4DspSIntSD = (UINT32) FLOW_CONTROL_RESUME;
        LOG(7, "[DSP] Dec(1) change state -> Resume -----------------\n");
        break;
#ifdef CC_MT5391_AUD_3_DECODER
    case DSP_FLUSH_DEC3:
        _uDspSIntAddr = INT_RC2D_FLOW_CONTROL_DEC3;
        _u4DspSIntSD = (UINT32) FLOW_CONTROL_FLUSH;

  #ifdef MP3ENC_SUPPORT
        if(_rDspVars[AUD_DEC_THIRD].bDspStrTyp == MP3ENC_STREAM)
        {
          _u4DspSIntSD = (UINT32) ENC_CONTROL_FLUSH;
          LOG(7, "[DSP] Dec(2) run MP3 Encoder\n");
        }
  #endif

        LOG(7, "[DSP] Dec(2) change state -> Flush ------------------\n");
        break;

    case DSP_PAUSE_DEC3:
        _uDspSIntAddr = INT_RC2D_FLOW_CONTROL_DEC3;
        _u4DspSIntSD = (UINT32) FLOW_CONTROL_PAUSE;
    #ifdef MP3ENC_SUPPORT
        if(_rDspVars[AUD_DEC_THIRD].bDspStrTyp == MP3ENC_STREAM)
        {
          _u4DspSIntSD = (UINT32) ENC_CONTROL_PAUSE;
          LOG(7, "[DSP] Dec(2) run MP3 Encoder\n");
        }
    #endif
        LOG(7, "[DSP] Dec(2) change state -> Pause ------------------\n");
        break;

    case DSP_RESUME_DEC3:
        _uDspSIntAddr = INT_RC2D_FLOW_CONTROL_DEC3;
        _u4DspSIntSD = (UINT32) FLOW_CONTROL_RESUME;
    #ifdef MP3ENC_SUPPORT
        if(_rDspVars[AUD_DEC_THIRD].bDspStrTyp == MP3ENC_STREAM)
        {
          _u4DspSIntSD = (UINT32) ENC_CONTROL_RESUME;
          LOG(7, "[DSP] Dec(2) run MP3 Encoder\n");
        }
    #endif
        LOG(7, "[DSP] Dec(2) change state -> Resume -----------------\n");
        break;

#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    case DSP_FLUSH_DEC4:
        _uDspSIntAddr = INT_RC2D_FLOW_CONTROL_DEC4;
        _u4DspSIntSD = (UINT32) FLOW_CONTROL_FLUSH;
        LOG(7, "[DSP] Dec(3) change state -> Flush ------------------\n");
        break;
#endif
    case DSP_WAKEUP:
        _uDspSIntAddr = INT_RC2D_INTERRUPT_CTRL;
        _u4DspSIntSD = (UINT32) RISC_WAKEUP_DSP;
        _rDspStatus[AUD_DEC_MAIN].fgDspWakeUp = TRUE;
        LOG(7, "[DSP] Change state -> Wakeup ------------------------\n");
        break;
    case DSP_GETMPEG_TYPE:
        vDspPlayCommonInit ();  // decoder initial before playing
        _u4DspSIntSD = (UINT32) FLOW_CONTROL_PLAY;
        _rDspStatus[AUD_DEC_MAIN].fgDspGetMpgTyp = TRUE; // Will be cleared at sendsampling rate
        _rDspStatus[AUD_DEC_MAIN].fgDspMpgTypGot = FALSE;
        _rDspVars[AUD_DEC_MAIN].dwDspMpgTyp = TYPE_INVALID;
        _rDspStatus[AUD_DEC_MAIN].fgDspStopIssue = FALSE;
        _rDspStatus[AUD_DEC_MAIN].fgDspPlay = TRUE;
        _rDspStatus[AUD_DEC_MAIN].fgDspStop = FALSE;
        LOG(7, "[DSP] Dec(0) change state -> Get MPEG Type ----------\n");
        break;
    case DSP_GETMPEG_TYPE_DEC2:
        vDspPlayCommonInitDec2 ();  // decoder initial before playing
        _uDspSIntAddr = INT_RC2D_FLOW_CONTROL_DEC2;
        _u4DspSIntSD = (UINT32) FLOW_CONTROL_PLAY;
        _rDspStatus[AUD_DEC_AUX].fgDspGetMpgTyp = TRUE; // Will be cleared at sendsampling rate
        _rDspStatus[AUD_DEC_AUX].fgDspMpgTypGot = FALSE;
        _rDspVars[AUD_DEC_AUX].dwDspMpgTyp = TYPE_INVALID;
        _rDspStatus[AUD_DEC_AUX].fgDspStopIssue = FALSE;
        _rDspStatus[AUD_DEC_AUX].fgDspPlay = TRUE;
        _rDspStatus[AUD_DEC_AUX].fgDspStop = FALSE;
        LOG(7, "[DSP] Dec(1) change state -> Get MPEG Type ----------\n");
        break;
#ifdef CC_MT5391_AUD_3_DECODER
    case DSP_GETMPEG_TYPE_DEC3:
        vDspPlayCommonInitDec3 ();  // decoder initial before playing
        _uDspSIntAddr = INT_RC2D_FLOW_CONTROL_DEC3;
        _u4DspSIntSD = (UINT32) FLOW_CONTROL_PLAY;
        _rDspStatus[AUD_DEC_THIRD].fgDspGetMpgTyp = TRUE; // Will be cleared at sendsampling rate
        _rDspStatus[AUD_DEC_THIRD].fgDspMpgTypGot = FALSE;
        _rDspVars[AUD_DEC_THIRD].dwDspMpgTyp = TYPE_INVALID;
        _rDspStatus[AUD_DEC_THIRD].fgDspStopIssue = FALSE;
        _rDspStatus[AUD_DEC_THIRD].fgDspPlay = TRUE;
        _rDspStatus[AUD_DEC_THIRD].fgDspStop = FALSE;
        LOG(7, "[DSP] Dec(2) change state -> Get MPEG Type ----------\n");
        break;
#endif
    #if (!defined(CC_MT5368) && !defined(CC_MT5396) && !defined(CC_MT5389))
    case DSP_RESET_READ_POINTER:
        _uDspSIntAddr = INT_RC2D_BUFPNT_RESET;
        _u4DspSIntSD = (UINT32) 0;
        LOG(7, "[DSP] Dec(0) change state -> Reset Read Pointer -----\n");
        break;
    case DSP_RESET_READ_POINTER_DEC2:
        _uDspSIntAddr = INT_RC2D_BUFPNT_RESET_DEC2;
        _u4DspSIntSD = (UINT32) 0;
        LOG(7, "[DSP] Dec(1) change state -> Reset Read Pointer -----\n");
        break;
#ifdef CC_MT5391_AUD_3_DECODER
    case DSP_RESET_READ_POINTER_DEC3:
        _uDspSIntAddr = INT_RC2D_BUFPNT_RESET_DEC3;
        _u4DspSIntSD = (UINT32) 0;
        LOG(7, "[DSP] Dec(2) change state -> Reset Read Pointer -----\n");
        break;
#endif
    #endif

    case DSP_CLEAR_AOUT_WAIT:
         _uDspSIntAddr = INT_RC2D_AND;
         _u4DspSIntLD = SRAM_DECODER_CONTROL;
         _u4DspSIntSD = 0xFBFF00; // Clear one bit
         _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;
         break;
    case DSP_CLEAR_AOUT_WAIT_DEC2:
         _uDspSIntAddr = INT_RC2D_AND;
         _u4DspSIntLD = SRAM_DECODER_CONTROL_DEC2;
         _u4DspSIntSD = 0xFBFF00; // Clear one bit
         _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;
         break;
    default:
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
    }
}

/***************************************************************************
     Function : vVolumeUOP
  Description : None
    Parameter : None
    Return    : None
***************************************************************************/
void vVolumeUOP (UINT16 wDspUop)
{
    UINT32 dwDspInfo;
    UINT16 u2DspData;
//    UINT32 dwOutSpkCfg;

    DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);
    _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;
    switch (wDspUop)
    {
        // first decoder
    case UOP_DSP_INPUT_SRC_GAIN:
        WriteDspCommDram (ADDR_RC2D_INPUT_SRC_GAIN, u4ReadDspShmDWRD (D_INSRC_GAIN));
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
        break;
    case UOP_DSP_MASTER_VOLUME:
        _uDspSIntAddr = INT_RC2D_SMOOTH_VOLUME;
        _u4DspSIntSD = u4ReadDspShmDWRD (D_VOL);
        //Read Previous Volume
        dwDspInfo = dReadDspCommDram (ADDR_RC2D_MASTER_VOLUME);
        //Smoothly change volume
        WriteDspCommDram (ADDR_RC2D_MASTER_VOLUME, _u4DspSIntSD);
        WriteDspCommDram (ADDR_RC2D_MASTER_VOLUME_BACKUP, _u4DspSIntSD);

    #ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
        #ifdef MIXSOUND_GAIN_CHG_WITH_VOL
        {
            int i;
            for (i=0;i<MAX_AUD_MIXSND_STREAM_NUM;i++)
            {
                WriteDspCommDram (ADDR_RC2D_DRAM_MIXSOUND_CLIP1_GAIN+i*16, _u4DspSIntSD);
            }
        }
		#else
		{
			#ifdef ALSA_MIXSND_PATH
            //WriteDspCommDram (ADDR_RC2D_DRAM_MIXSOUND_CLIP1_EA+ALSA_MIXSND_STREAM_ID*16, (u4GetMixSoundStartByteAddr2()&0xffffff)+dTmp*ALSA_MIXSND_STREAM_ID+0x10000);
			if(0 == _u4DspSIntSD)
			{
				WriteDspCommDram (ADDR_RC2D_DRAM_MIXSOUND_CLIP1_GAIN+ALSA_MIXSND_STREAM_ID*16, _u4DspSIntSD);
			}
			else
			{
				WriteDspCommDram (ADDR_RC2D_DRAM_MIXSOUND_CLIP1_GAIN+ALSA_MIXSND_STREAM_ID*16, 0x20000);
			}
            #endif
		}
        #endif
    #endif
        // check if AVC is on
        if (u2ReadDspShmWORD (W_AVC_FLAG) != 0)
        {
            // AVC on, only use risc_volume to control master volume
            _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
        }
        //Check if DRC On
        u2DspData = u2ReadDspShmWORD (W_PROCMOD);
        //Check if DRC On
        if (u2DspData & 0x2)
        {
            if ((_u4DspSIntSD < dwDspInfo))
            {
                //Read current volume
                dwDspInfo = dReadDspCommDram (ADDR_D2RC_RISC_INFO_VOLUME_NOW);
                //Compare if current volume is different from setting
                if (dwDspInfo <= _u4DspSIntSD)
                {
                    _u4DspSIntSD = dwDspInfo;
                }
                else
                {
                    SendDSPInt (INT_RC2D_MASTER_VOLUME, _u4DspSIntSD);  // short data & interrupt
                }
            }
            else if (_u4DspSIntSD == dwDspInfo)
            {
                if (_u4DspSIntSD > 0x8000)
                {
                    //Change Volume
                    _uDspSIntAddr = INT_RC2D_SMOOTH_VOLUME;
                    _u4DspSIntSD = 0x8000;
                }
            }
            else
            {
                if (_u4DspSIntSD < 0x800)
                {
                    //Change Volume
                    SendDSPInt (INT_RC2D_MASTER_VOLUME, _u4DspSIntSD);  // short data & interrupt
                }
                else
                {
                    if (_u4DspSIntSD > 0x8000)
                    {
                        if (dwDspInfo == 0)
                        {
                            //Change Volume
                            _u4DspSIntSD = 0x8000;
                            //Change Volume
                            //SendDSPInt(INT_RC2D_MASTER_VOLUME, _u4DspSIntSD);    // short data & interrupt
                        }
                    }
                    else
                    {
                        //Don't Send Interrupt
                        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
                    }
                }
            }
        }
        break;
    case UOP_DSP_TRIM_C:
    case UOP_DSP_TRIM_L:
    case UOP_DSP_TRIM_R:
    case UOP_DSP_TRIM_LS:
    case UOP_DSP_TRIM_RS:
    case UOP_DSP_TRIM_CH8:
    case UOP_DSP_TRIM_CH7:
    case UOP_DSP_TRIM_SUBWOOFER:
        u2DspData = (UINT16)(wDspUop - UOP_DSP_TRIM_C) >> 8;
        WriteDspCommDram((ADDR_RC2D_CH_TRIM + u2DspData), u4ReadDspShmDWRD ((D_TRIM_C + (u2DspData << 2))));
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
        break;
    case UOP_DSP_TRIM_CH9:
    case UOP_DSP_TRIM_CH10:
        u2DspData = (UINT16)(wDspUop - UOP_DSP_TRIM_CH9) >> 8;
        if ((UINT8)DRVCUST_OptGet(eAudioDownmixPosition) == DOWNMIX_CH_BEFORE_AVC)
        {
            UINT32 u4TrimValue;

            u4TrimValue = u4ReadDspShmDWRD ((D_TRIM_CH9 + (u2DspData << 2))) << 3;
            if (u4TrimValue > 0x07fffff)
            {
                u4TrimValue = 0x07fffff;
            }
            WriteDspCommDram((ADDR_RC2D_CH_TRIM_CH9 + u2DspData), u4TrimValue);
        }
        else
        {
            WriteDspCommDram((ADDR_RC2D_CH_TRIM_CH9 + u2DspData), u4ReadDspShmDWRD ((D_TRIM_CH9 + (u2DspData << 2))));
        }
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
    	break;
    case UOP_DSP_TRIM_LFE:
        _uDspSIntAddr = INT_RC2D_CH_TRIM_LFE;
        _u4DspSIntSD = u4ReadDspShmDWRD (D_TRIM_LFE);
        break;
    case UOP_DSP_DIALOGUE_GAIN:
        _uDspSIntAddr = INT_RC2D_DIALOGUE_GAIN;
        _u4DspSIntSD = u4ReadDspShmDWRD (D_DIALOGUE);
        break;
    case UOP_DSP_RAW_MUTE:
        _rDspStatus[AUD_DEC_MAIN].fgIecMuteState = TRUE;
        u2DspData = wReadDspWORD (ADDR_RC2D_IEC_FLAG);
        u2DspData |= IEC_DSP_MUTE;
        vWriteDspWORD (ADDR_RC2D_IEC_FLAG, u2DspData);
        DspIntOrSram (SRAM_IEC_FLAG, 0x800000); // setup iec_flag bit 15 for raw mute
        break;
    case UOP_DSP_RAW_UNMUTE:
        _rDspStatus[AUD_DEC_MAIN].fgIecMuteState = FALSE;
        u2DspData = wReadDspWORD (ADDR_RC2D_IEC_FLAG);
        u2DspData &= ~IEC_DSP_MUTE;
        vWriteDspWORD (ADDR_RC2D_IEC_FLAG, u2DspData);
        DspIntAndSram (SRAM_IEC_FLAG, 0x7fff00);
        break;
        // second decoder
    case UOP_DSP_INPUT_SRC_GAIN_DEC2:
        WriteDspCommDram (ADDR_RC2D_INPUT_SRC_GAIN_DEC2, u4ReadDspShmDWRD (D_INSRC_GAIN_DEC2));
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
        break;
    case UOP_DSP_MASTER_VOLUME_DEC2:
        _uDspSIntAddr = INT_RC2D_SMOOTH_VOLUME_DEC2;
        _u4DspSIntSD = u4ReadDspShmDWRD (D_VOL_DEC2);
        //Read Previous Volume
        dwDspInfo = dReadDspCommDram (ADDR_RC2D_MASTER_VOLUME_DEC2);
        //Smoothly change volume
        WriteDspCommDram (ADDR_RC2D_MASTER_VOLUME_DEC2, _u4DspSIntSD);
        WriteDspCommDram (ADDR_RC2D_MASTER_VOLUME_BACKUP_DEC2, _u4DspSIntSD);
        break;
#ifdef CC_MT5391_AUD_3_DECODER
    case UOP_DSP_INPUT_SRC_GAIN_DEC3:
        WriteDspCommDram (ADDR_RC2D_INPUT_SRC_GAIN_DEC3, u4ReadDspShmDWRD (D_INSRC_GAIN_DEC3));
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
        break;
    case UOP_DSP_MASTER_VOLUME_DEC3:
        _uDspSIntAddr = INT_RC2D_SMOOTH_VOLUME_DEC3;
        _u4DspSIntSD = u4ReadDspShmDWRD (D_VOL_DEC3);
        //Read Previous Volume
        dwDspInfo = dReadDspCommDram (ADDR_RC2D_MASTER_VOLUME_DEC3);
        //Smoothly change volume
        WriteDspCommDram (ADDR_RC2D_MASTER_VOLUME_DEC3, _u4DspSIntSD);
        WriteDspCommDram (ADDR_RC2D_MASTER_VOLUME_BACKUP_DEC3, _u4DspSIntSD);
        break;
    case UOP_DSP_TRIM_L_DEC3:
    case UOP_DSP_TRIM_R_DEC3:
        u2DspData = (UINT16)(wDspUop - UOP_DSP_TRIM_L_DEC3) >> 8;
        WriteDspCommDram((ADDR_RC2D_CH_TRIM_DEC3_L + u2DspData), u4ReadDspShmDWRD ((D_TRIM_L_DEC3 + (u2DspData << 2))));
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
        break;
    case UOP_DSP_AD_FADE:
        WriteDspCommDram(ADDR_RC2D_AD_FADE_VALUE, u4ReadDspShmDWRD (D_FADEVALUE));
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
        break;
    case UOP_DSP_PANFADE_DSP_ENABLE:
     _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;
     _uDspSIntAddr = INT_RC2D_SYSTEM_SETUP;
     _u4DspSIntSD = ((UINT32) u2ReadDspShmWORD (W_SYSTEM_SETUP)) << 8;
     vWriteDspWORD (ADDR_RC2D_SYSTEM_SETUP,   u2ReadDspShmWORD (W_SYSTEM_SETUP));
     break;
#endif
#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
#ifdef CC_AUD_4_DECODER_SUPPORT
    case UOP_DSP_INPUT_SRC_GAIN_DEC4:
        WriteDspCommDram (ADDR_RC2D_INPUT_SRC_GAIN_DEC4, u4ReadDspShmDWRD (D_INSRC_GAIN_DEC4));
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
        break;
    case UOP_DSP_MASTER_VOLUME_DEC4:
        _uDspSIntAddr = INT_RC2D_SMOOTH_VOLUME_DEC4;
        _u4DspSIntSD = u4ReadDspShmDWRD (D_VOL_DEC4);
        //Read Previous Volume
        dwDspInfo = dReadDspCommDram (ADDR_RC2D_MASTER_VOLUME_DEC4);
        //Smoothly change volume
        WriteDspCommDram (ADDR_RC2D_MASTER_VOLUME_DEC4, _u4DspSIntSD);
        WriteDspCommDram (ADDR_RC2D_MASTER_VOLUME_BACKUP_DEC4, _u4DspSIntSD);
        break;
#endif
#endif
    case UOP_DSP_TRIM_L_DEC2:
    case UOP_DSP_TRIM_R_DEC2:
        u2DspData = (UINT16)(wDspUop - UOP_DSP_TRIM_L_DEC2) >> 8;
        WriteDspCommDram((ADDR_RC2D_CH_TRIM_DEC2_L + u2DspData), u4ReadDspShmDWRD ((D_TRIM_L_DEC2 + (u2DspData << 2))));
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
        break;
    case UOP_DSP_TRIM_LFE_DEC2:
        _uDspSIntAddr = INT_RC2D_CH_TRIM_LFE_DEC2;
        _u4DspSIntSD = u4ReadDspShmDWRD (D_TRIM_LFE_DEC2);
        break;
    case UOP_DSP_DIALOGUE_GAIN_DEC2:
        _uDspSIntAddr = INT_RC2D_DIALOGUE_GAIN_DEC2;
        _u4DspSIntSD = u4ReadDspShmDWRD (D_DIALOGUE_DEC2);
        break;
    case UOP_DSP_RAW_MUTE_DEC2:
        _rDspStatus[AUD_DEC_AUX].fgIecMuteState = TRUE;
        u2DspData = wReadDspWORD (ADDR_RC2D_IEC_FLAG_DEC2);
        u2DspData |= IEC_DSP_MUTE;
        vWriteDspWORD (ADDR_RC2D_IEC_FLAG_DEC2, u2DspData);
        DspIntOrSram (SRAM_IEC_FLAG_DEC2, 0x800000);    // setup iec_flag bit 15 for raw mute
        break;
    case UOP_DSP_RAW_UNMUTE_DEC2:
        _rDspStatus[AUD_DEC_AUX].fgIecMuteState = FALSE;
        u2DspData = wReadDspWORD (ADDR_RC2D_IEC_FLAG_DEC2);
        u2DspData &= ~IEC_DSP_MUTE;
        vWriteDspWORD (ADDR_RC2D_IEC_FLAG_DEC2, u2DspData);
        DspIntAndSram (SRAM_IEC_FLAG_DEC2, 0x7fff00);
        break;
    default:
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
    }
}

#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
#include "dsp_common.h"
#include "dsp_data.h"
#endif
extern UINT32 _AUD_DspGetMasterVolRawValue(UINT8 u1DecId);

/***************************************************************************
     Function : vMixSoundUOP
  Description : None
    Parameter : None
    Return    : None
***************************************************************************/
void vMixSoundUOP (UINT16 wDspUop)
{
    switch (wDspUop)
    {
#ifdef CC_AUD_MIXSOUND_SUPPORT
    case UOP_DSP_MIXSOUND_FLAG:
        // set mixsound_flag
        _uDspSIntAddr = INT_RC2D_MIXSOUND_FLAG;
        _u4DspSIntSD = ((UINT32) u2ReadDspShmWORD (W_MIXSOUND_FLAG)) << 8;
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;

        // set the status bit
        if (_u4DspSIntSD & 0x500)
        {
            int i;

            #ifndef NEW_MIXSOUND
            // MixSound play
            vWriteDspWORD (ADDR_RC2D_DRAM_MIXSOUND_STATUS, 0x1);
            #endif
            // set the config
            vWriteDspWORD (ADDR_RC2D_DRAM_MIXSOUND_CONFIG,
                           u2ReadDspShmWORD (W_MIXSOUND_CFG));
            // set the pcm length
            WriteDspCommDram (ADDR_RC2D_DRAM_MIXSOUND_PCM_LEN,
                              u4ReadDspShmDWRD (D_MIXSOUND_PLEN));
            // set the pcm address --> transfer physical address to dsp cmpt read address
            WriteDspCommDram (ADDR_RC2D_DRAM_MIXSOUND_PCM_ADDR,
                              (u4GetMixSoundStartByteAddr()-_u4DspCmptBuf[TYPE_MIX_DRAM_IDX])>>2);
            // set the pcm gain
            WriteDspCommDram (ADDR_RC2D_DRAM_MIXSOUND_GAIN,
                              u4ReadDspShmDWRD (D_MIXSOUND_GAIN));
            #ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
            #ifdef NEW_MIXSOUND
            if (wReadDspWORD(ADDR_RC2D_DRAM_MIXSOUND_STATUS)==0)
            #endif
            {
                #if 0 //def ALSA_MIXSND_PATH
                UINT32 dTmp = (u4GetMixSoundBufSize2()-0x10000)/(MAX_AUD_MIXSND_STREAM_NUM-1);
                #else
                UINT32 dTmp = u4GetMixSoundBufSize2()/MAX_AUD_MIXSND_STREAM_NUM;
                #endif
                Printf("[UOP_DSP_MIXSOUND_FLAG] Reset MIXSOUND_MEMPLAY related common dram\n");
                WriteDspCommDram (ADDR_RC2D_DRAM_MIXSOUND_PCM_BUF_PNT_OFST, (u4GetMixSoundStartByteAddr()&0xffffff));
                vWriteDspWORD(ADDR_RC2D_DRAM_MIXSOUND_CLIP_NO_1, MAX_AUD_MIXSND_STREAM_NUM-1);
                for (i=0;i<MAX_AUD_MIXSND_STREAM_NUM;i++)
                {
                    WriteDspCommDram (ADDR_RC2D_DRAM_MIXSOUND_CLIP1_RP+i*16, (u4GetMixSoundStartByteAddr2()&0xffffff)+dTmp*i);
                    WriteDspCommDram (ADDR_RC2D_DRAM_MIXSOUND_CLIP1_WP+i*16, (u4GetMixSoundStartByteAddr2()&0xffffff)+dTmp*i);
                    WriteDspCommDram (ADDR_RC2D_DRAM_MIXSOUND_CLIP1_SA+i*16, (u4GetMixSoundStartByteAddr2()&0xffffff)+dTmp*i);
                    WriteDspCommDram (ADDR_RC2D_DRAM_MIXSOUND_CLIP1_EA+i*16, (u4GetMixSoundStartByteAddr2()&0xffffff)+dTmp*(i+1));
                    #ifdef MIXSOUND_GAIN_CHG_WITH_VOL
                    WriteDspCommDram (ADDR_RC2D_DRAM_MIXSOUND_CLIP1_GAIN+i*16, _AUD_DspGetMasterVolRawValue(AUD_DEC_MAIN)); //dReadDspCommDram (ADDR_RC2D_MASTER_VOLUME));
                    #else
                    WriteDspCommDram (ADDR_RC2D_DRAM_MIXSOUND_CLIP1_GAIN+i*16, 0x4000);//set by AUD_DspSetGstGain
                    #endif
                }
                #ifdef ALSA_MIXSND_PATH
                //WriteDspCommDram (ADDR_RC2D_DRAM_MIXSOUND_CLIP1_EA+ALSA_MIXSND_STREAM_ID*16, (u4GetMixSoundStartByteAddr2()&0xffffff)+dTmp*ALSA_MIXSND_STREAM_ID+0x10000);
                if(0 == _AUD_DspGetMasterVolRawValue(AUD_DEC_MAIN))
                {
					WriteDspCommDram (ADDR_RC2D_DRAM_MIXSOUND_CLIP1_GAIN+ALSA_MIXSND_STREAM_ID*16, 0);
                }
				else
				{
                	WriteDspCommDram (ADDR_RC2D_DRAM_MIXSOUND_CLIP1_GAIN+ALSA_MIXSND_STREAM_ID*16, 0x20000);
				}
                #endif
            }
            #endif

            #ifdef NEW_MIXSOUND
            // MixSound play
            vWriteDspWORD (ADDR_RC2D_DRAM_MIXSOUND_STATUS, 0x1);
            #endif
        }
        break;
#endif
    case UOP_DSP_LIMITER_CONFIG:
        vWriteDspWORD (ADDR_RC2D_LIMITER_GAIN_MODE,
            (u2ReadDspShmWORD (W_LIMITER_MODE)));
        vWriteDspWORD (ADDR_RC2D_LIMITER_FLAG,
            (u2ReadDspShmWORD (W_LIMITER_FLAG)));
        WriteDspCommDram (ADDR_RC2D_LIMITER_FIX_THRESHOLD,
            (u4ReadDspShmDWRD (D_LIMITER_FIX_THRESHOLD)));
#ifdef CC_RATIO_LIMITER
        WriteDspCommDram (ADDR_RC2D_LIMITER_GAIN_RATIO,
            (u4ReadDspShmDWRD (D_LIMITER_GAIN_RATIO)));
#endif
#ifdef EFFECT_TRANSPARENT_REINIT //add for transparent reinitial
        vSetEffectReinit(AUD_DEC_MAIN);
#else
        vSetAOutReinit(AUD_DEC_MAIN);
#endif
        break;
    default:
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
    }
}

void vPostProcUOP (UINT16 wDspUop)
{
    INT32 i;
    INT16 j;
	UINT32 temp;
	UINT32 u4DramBase;

    UNUSED(i);
    UNUSED(j);
    UNUSED(temp);
	UNUSED(u4DramBase);

    switch (wDspUop)
    {
#ifdef CC_AUD_HPF_SUPPORT
    case UOP_DSP_HPF_ENABLE:
        // Setup HPF enable bit
        vWriteDspWORD (ADDR_RC2D_NEWPOST_FLAG,
            ((wReadDspWORD(ADDR_RC2D_NEWPOST_FLAG)) & ~NEWPOST_FLAG_HPF_BIT) |
            ((uReadDspShmBYTE(B_HPF_ENABLE) > 0) ? NEWPOST_FLAG_HPF_BIT : 0));
        vSetAOutReinit(AUD_DEC_MAIN);
        break;
    case UOP_DSP_HPF_FC:
        // Trigger aout reinit, and proper table will be load when
        // set sampling rate -> vLoadHPFTable
        vSetAOutReinit(AUD_DEC_MAIN);
        break;
#endif
#ifdef CC_AUD_CLIPPER_SUPPORT
    case UOP_DSP_CLIPPER_ENABLE:
        // Setup clipper enable bit
        vWriteDspWORD (ADDR_RC2D_NEWPOST_FLAG,
            ((wReadDspWORD(ADDR_RC2D_NEWPOST_FLAG)) & ~NEWPOST_FLAG_CLIPPER_BIT) |
            ((uReadDspShmBYTE(B_CLIPPER_ENABLE) > 0) ? NEWPOST_FLAG_CLIPPER_BIT : 0));
        vSetAOutReinit(AUD_DEC_MAIN);
        break;
    case UOP_DSP_CLIPPER_VAL:
        // Setup clipper value
        WriteDspCommDram(ADDR_RC2D_CLIPPER_C_VAL, u4ReadDspShmDWRD(D_CLIPPER_C_VALUE));
        WriteDspCommDram(ADDR_RC2D_CLIPPER_L_VAL, u4ReadDspShmDWRD(D_CLIPPER_L_VALUE));
        WriteDspCommDram(ADDR_RC2D_CLIPPER_R_VAL, u4ReadDspShmDWRD(D_CLIPPER_R_VALUE));
        WriteDspCommDram(ADDR_RC2D_CLIPPER_LS_VAL, u4ReadDspShmDWRD(D_CLIPPER_LS_VALUE));
        WriteDspCommDram(ADDR_RC2D_CLIPPER_RS_VAL, u4ReadDspShmDWRD(D_CLIPPER_RS_VALUE));
        WriteDspCommDram(ADDR_RC2D_CLIPPER_SW_VAL, u4ReadDspShmDWRD(D_CLIPPER_SW_VALUE));
        WriteDspCommDram(ADDR_RC2D_CLIPPER_CH7_VAL, u4ReadDspShmDWRD(D_CLIPPER_CH7_VALUE));
        WriteDspCommDram(ADDR_RC2D_CLIPPER_CH8_VAL, u4ReadDspShmDWRD(D_CLIPPER_CH8_VALUE));
        WriteDspCommDram(ADDR_RC2D_CLIPPER_CH9_VAL, u4ReadDspShmDWRD(D_CLIPPER_CH9_VALUE));
        WriteDspCommDram(ADDR_RC2D_CLIPPER_CH10_VAL, u4ReadDspShmDWRD(D_CLIPPER_CH10_VALUE));
        vSetAOutReinit(AUD_DEC_MAIN);
        break;
#endif
#ifdef CC_AUD_VBASS_SUPPORT
    case UOP_DSP_VIR_BASS_ENABLE:
        vWriteDspWORD (ADDR_RC2D_NEWPOST_FLAG,
                ((wReadDspWORD(ADDR_RC2D_NEWPOST_FLAG)) & ~NEWPOST_FLAG_VIR_BASS_BIT) |
                ((uReadDspShmBYTE(B_VIR_BASS_ENABLE) > 0) ? NEWPOST_FLAG_VIR_BASS_BIT : 0));
        vSetAOutReinit(AUD_DEC_MAIN);
        break;
    case UOP_DSP_VIR_BASS_CONFIG:
		u4DramBase = dReadDspCommDram(ADDR_RC2D_VBASS_BASE);
        vWriteDspWORD ((u4DramBase+ADDR_VBASS_CROS_PARA4), u4ReadDspShmDWRD(D_VIR_BASS_GAIN));
        vWriteDspWORD ((u4DramBase+ADDR_VBASS_CROS_PARA7), u4ReadDspShmDWRD(D_VIR_BASS_GAIN_2));
        vSetAOutReinit(AUD_DEC_MAIN);
        break;
#endif
#ifdef CC_AUD_SPEAKER_HEIGHT
    case UOP_DSP_SPH_ENABLE:
	      vWriteDspWORD (ADDR_RC2D_NEWPOST_FLAG,
			  ((wReadDspWORD(ADDR_RC2D_NEWPOST_FLAG)) & ~NEWPOST_FLAG_SPH_BIT) |
#ifndef CC_AUD_SPEAKER_HEIGHT_MODE
			  ((uReadDspShmBYTE(B_SPH_ENABLE) > 0) ? NEWPOST_FLAG_SPH_BIT : 0));
#else
        ((uReadDspShmBYTE(B_SPH_MODE) > 0) ? NEWPOST_FLAG_SPH_BIT : 0));
#endif

        ////vSetEffectReinit(AUD_DEC_MAIN);
        vSetAOutReinit(AUD_DEC_MAIN);
		break;
#endif
#ifdef CC_AUD_SPEAKER_HEIGHT_MODE
    case UOP_DSP_SPH_CHANGE_MODE:
		    vSetEffectReinit(AUD_DEC_MAIN);
		break;
#endif

#ifdef CC_AUD_PEQ_SUPPORT
    case UOP_DSP_PEQ_ENABLE:
		u4DramBase = dReadDspCommDram(ADDR_RC2D_PEQ_BASE);
        vWriteDspWORD (ADDR_RC2D_NEWPOST_FLAG,
                ((wReadDspWORD(ADDR_RC2D_NEWPOST_FLAG)) & ~NEWPOST_FLAG_PEQ_BIT) |
                ((uReadDspShmBYTE(B_PEQ_ENABLE) > 0) ? NEWPOST_FLAG_PEQ_BIT : 0));

        for (i=0; i<9; i++)
        {
            WriteDspCommDram ((u4DramBase + ADDR_PEQ1_FC + i*3), u2ReadDspShmWORD(W_PEQ_SET0_FC + i*8));
   	        WriteDspCommDram ((u4DramBase + ADDR_PEQ1_GAIN + i*3), u4ReadDspShmDWRD(D_PEQ_SET0_GAIN + i*8));
            j=dwGetPEQFsIdx(i) ;
			VERIFY(j < sizeof(MTK_TAN_TABLE)/4);
            temp=MTK_TAN_TABLE[j];

           if (j > sizeof(MTK_TAN_TABLE)/4)
           {
                      j = (sizeof(MTK_TAN_TABLE)/2) - j;
                      temp=-(MTK_TAN_TABLE[j]);
           }

            WriteDspCommDram ((u4DramBase + ADDR_PEQ1_BW + i*3), temp);
			j = dwGetPEQCosIdx(i) ;
			temp=COS_TABLE[j];
			WriteDspCommDram ((u4DramBase + ADDR_PEQ1_FC + i*3), temp);
        }
        //vSetEffectReinit(AUD_DEC_MAIN);
        vSetAOutReinit(AUD_DEC_MAIN);
        break;
    case UOP_DSP_PEQ_CONFIG:
		u4DramBase = dReadDspCommDram(ADDR_RC2D_PEQ_BASE);
        vWriteDspWORD (ADDR_RC2D_NEWPOST_FLAG,
                ((wReadDspWORD(ADDR_RC2D_NEWPOST_FLAG)) & ~NEWPOST_FLAG_PEQ_BIT) |
                ((uReadDspShmBYTE(B_PEQ_ENABLE) > 0) ? NEWPOST_FLAG_PEQ_BIT : 0));

        // Load Fc, Gain
        for (i=0; i<9; i++)
        {
            WriteDspCommDram ((u4DramBase + ADDR_PEQ1_FC + i*3), u2ReadDspShmWORD(W_PEQ_SET0_FC + i*8));
   	        WriteDspCommDram ((u4DramBase + ADDR_PEQ1_GAIN + i*3), u4ReadDspShmDWRD(D_PEQ_SET0_GAIN + i*8));
            j=dwGetPEQFsIdx(i) ;
            LOG(7,"j=%d\n",j);
            // temp=sizeof(MTK_TAN_TABLE);
            // LOG(7,"sizeof=%d\n",temp);
			VERIFY(j < sizeof(MTK_TAN_TABLE)/4);
            temp=MTK_TAN_TABLE[j];
            LOG(7,"temp=%x\n",temp);
            if (j > (sizeof(MTK_TAN_TABLE)/4))
            {
                      j = (sizeof(MTK_TAN_TABLE)/2) - j;
                LOG(7,"j=%d",j);
                temp=-(MTK_TAN_TABLE[j]);
                LOG(7,"temp2=%x\n",temp);
            }

            WriteDspCommDram ((u4DramBase + ADDR_PEQ1_BW + i*3), temp );
			j = dwGetPEQCosIdx(i) ;
			temp=COS_TABLE[j];
			WriteDspCommDram ((u4DramBase + ADDR_PEQ1_FC + i*3), temp);
        }
        break;
#endif
    case UOP_DSP_DDCO_ENABLE:
        if (IS_SupportDDCO() || IS_SupportMS10())
        {
            vWriteDspWORD (ADDR_RC2D_DDCO_FLAG, uReadDspShmBYTE(B_DDCO_FLAG));
            vSetAOutReinit(AUD_DEC_MAIN);
        }
        break;
    case UOP_DSP_DDCO_AGC_ENABLE:
	   vWriteDspWORD (ADDR_RC2D_DDCO_AGC_FLAG, uReadDspShmBYTE(B_DDCO_AGC_FLAG));
	    break;

    case UOP_DSP_DDCO_LFE_LPF_ENABLE:
	    vWriteDspWORD (ADDR_RC2D_DDCO_LFE_LPF_FLAG, uReadDspShmBYTE(B_DDCO_LFE_LPF_FLAG));
        vSetAOutReinit(AUD_DEC_MAIN);
	    break;
//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
    case UOP_DSP_UPLOAD_DATA_ENABLE:
        vWriteDspWORD (ADDR_RC2D_UPLOAD_DATA_FLAG, u2ReadDspShmWORD(W_UPLOAD_DATA_FLAG));
        vWriteDspWORD (ADDR_RC2D_UPLOAD_BLOCK_NUM, uReadDspShmBYTE(B_UPLOAD_BLOCK_NUM));
        vWriteDspWORD (ADDR_RC2D_UPLOAD_BLOCK_SIZE, u2ReadDspShmWORD(W_UPLOAD_BLOCK_SIZE));
        vWriteDspWORD (ADDR_RC2D_UPLOAD_BLK_INT_CNT, uReadDspShmBYTE(B_UPLOAD_BLK_INT_CNT));        //vSetAOutReinit(AUD_DEC_MAIN);
        break;
//#endif
#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
     case UOP_DSP_ADV_ENABLE:
	    vWriteDspWORD (ADDR_RC2D_AUDYSSEY_FLAG,
                        uReadDspShmBYTE(B_ADV_FLAG)|(uReadDspShmBYTE(B_ABX_ENABLE)<<4)|(uReadDspShmBYTE(B_AEQ_FLAG)<<5));
	    WriteDspCommDram(ADDR_RC2D_ADV_CHCALBGAIN_L, u4ReadDspShmDWRD(D_ADV_chCalbGain_L));
        WriteDspCommDram(ADDR_RC2D_ADV_CHCALBGAIN_R, u4ReadDspShmDWRD(D_ADV_chCalbGain_R));
        //vSetEffectReinit(AUD_DEC_MAIN);
        vSetAOutReinit(AUD_DEC_MAIN);
        LOG(5,"#### Get UOP_DSP_ADV_ENABLE ######\n");
        break;
     case UOP_DSP_ADV_SET_chCalbGain:
	    WriteDspCommDram(ADDR_RC2D_ADV_CHCALBGAIN_L, u4ReadDspShmDWRD(D_ADV_chCalbGain_L));
        WriteDspCommDram(ADDR_RC2D_ADV_CHCALBGAIN_R, u4ReadDspShmDWRD(D_ADV_chCalbGain_R));
        LOG(5,"#### Get UOP_DSP_ADV_SET_chCalbGain ######\n");
        break;
     case UOP_DSP_ABX_FLAG:
        vWriteDspWORD (ADDR_RC2D_AUDYSSEY_FLAG,
                         uReadDspShmBYTE(B_ADV_FLAG)|(uReadDspShmBYTE(B_ABX_ENABLE)<<4)|(uReadDspShmBYTE(B_AEQ_FLAG)<<5));
        vSetAOutReinit (AUD_DEC_MAIN);
        break;
     case UOP_DSP_AEQ_FLAG:
        vWriteDspWORD (ADDR_RC2D_AUDYSSEY_FLAG,
                         uReadDspShmBYTE(B_ADV_FLAG)|(uReadDspShmBYTE(B_ABX_ENABLE)<<4)|(uReadDspShmBYTE(B_AEQ_FLAG)<<5));
        vSetAOutReinit (AUD_DEC_MAIN);
        break;
#endif

    case UOP_DSP_AOUT_REINIT_FOR_SAMPLING_RATE_CHANGE:
        LOG(5,"#### Get UOP_DSP_AOUT_REINIT_FOR_SAMPLING_RATE_CHANGE ######\n");
        vSetAOutReinit(AUD_DEC_MAIN);	 //for dec1
	    break;

   case UOP_DSP_AOUT_REINIT_FOR_SAMPLING_RATE_CHANGE_DEC2:
        LOG(5,"#### Get UOP_DSP_AOUT_REINIT_FOR_SAMPLING_RATE_CHANGE_DEC2 ######\n");
        vSetAOutReinit(AUD_DEC_AUX);  //for dec2
        break;

    case UOP_DSP_DUAL_MONO_MODE:
        vWriteDspWORD (ADDR_MANUAL_OUTPUT_MATRIX, uReadDspShmBYTE(B_MANUAL_OUTPUT_MATRIX));
        vSetEffectReinit(AUD_DEC_MAIN);
        break;
   case UOP_DSP_DUAL_MONO_MODE_DEC2:
        vWriteDspWORD (ADDR_MANUAL_OUTPUT_MATRIX_DEC2, uReadDspShmBYTE(B_MANUAL_OUTPUT_MATRIX_DEC2));
        vSetEffectReinit(AUD_DEC_MAIN);
	    break;

   case UOP_DSP_DTS_ENCODER_AMODE:
	    vWriteDspWORD( ADDR_RC2D_DTS_ENCODER_AMODE , uReadDspShmBYTE(B_DTS_ENCODER_AMODE));
		vSetAOutReinit (AUD_DEC_MAIN);
	    break;
   case UOP_DSP_DTS_ENCODER_ENABLE:
		vWriteDspWORD (ADDR_RC2D_DTS_ENCODER_FLAG,	 uReadDspShmBYTE(B_DTS_ENCODER_FLAG));
		vSetAOutReinit(AUD_DEC_MAIN);
	    break;

#ifdef DSP_SUPPORT_SRS_SSHD
//sunman_peq
    case UOP_DSP_SRS_STUDIO_FLAG:
        WriteDspCommDram (ADDR_SRS_STUDIO_FLAG, u2ReadDspShmWORD (W_SRS_STUDIO_FLAG));
        vSetAOutReinit (AUD_DEC_MAIN);
        break;
#endif
#ifdef DSP_SUPPORT_SRS_PURESOUND
	//bofeng_aeq
		case UOP_DSP_SRS_PURESOUND_FLAG:
			WriteDspCommDram (ADDR_SRS_PURESOUND_FLAG, u2ReadDspShmWORD (W_SRS_PURESOUND_FLAG));
			vSetAOutReinit (AUD_DEC_MAIN);
			break;
#endif

    case UOP_DSP_IS_WITH_VIDEO:
        if (uReadDspShmBYTE(B_IS_WITH_VIDEO) == 0x1)
        {
            DspIntOrSram(SRAM_EFFECT_TRANSITION_FLAG, (UINT32)IS_WITH_VIDEO);
        }
        else
        {
            DspIntAndSram(SRAM_EFFECT_TRANSITION_FLAG, (UINT32)(~IS_WITH_VIDEO));
        }
	    break;
    case UOP_DSP_IS_WITH_VIDEO_DEC2:
        if (uReadDspShmBYTE(B_IS_WITH_VIDEO_DEC2) == 0x1)
        {
            DspIntOrSram(SRAM_EFFECT_TRANSITION_FLAG_DEC2, (UINT32)IS_WITH_VIDEO);
        }
        else
        {
            DspIntAndSram(SRAM_EFFECT_TRANSITION_FLAG_DEC2, (UINT32)(~IS_WITH_VIDEO));
        }
	    break;
		
#ifdef CC_AUD_FIR_SUPPORT
	case UOP_DSP_FIR_ENABLE:
		vWriteDspWORD (ADDR_RC2D_NEWPOST_FLAG, 
				((wReadDspWORD(ADDR_RC2D_NEWPOST_FLAG)) & ~NEWPOST_FLAG_FIR_BIT) |
				((uReadDspShmBYTE(B_FIR_ENABLE) > 0) ? NEWPOST_FLAG_FIR_BIT : 0));
		vSetAOutReinit(AUD_DEC_MAIN);
		break;
#endif

    default:
        break;
    }
}

/***************************************************************************
     Function : vDSPUOPSvc
  Description : DSP User Operation Service
    Parameter : None
    Return    : None
***************************************************************************/
void vDspUopSvc (UINT32 u4DspUop)
{
    UINT16 wDspUop;

    wDspUop = (UINT16) (u4DspUop & 0xFFFF);
    _rDspVars[AUD_DEC_MAIN].dwDspUop = u4DspUop ;

    _u4DspSIntLD = 0;

    //Check ID1
    switch ((wDspUop & 0xFF))
    {
        //Volume
    case DSP_UOPID1:
        vVolumeUOP (wDspUop);
        break;
        //Microphone --> MixSound
    case DSP_UOPID2:
        vMixSoundUOP (wDspUop);
        break;
        //Flow Control
    case DSP_UOPID3:
        vFlowControlUOP (wDspUop);
        break;
        //Channel Configuration
    case DSP_UOPID4:
        vChanConfigUOP (wDspUop);
        break;
        //IEC, PTS, STC and Speed
    case DSP_UOPID5:
        vGrp5UOP (wDspUop);
        break;
        //Pink Noise
    case DSP_UOPID6:
        vPinkUOP (wDspUop);
        break;
        //Karaoke
    case DSP_UOPID7:
        vKaraokeUOP (wDspUop);
        break;
        //Equalizer
    case DSP_UOPID8:
        vEQUOP (wDspUop);
        break;
        // 3D Processing
    case DSP_UOPID9:
        v3DUOP (wDspUop);
        break;
        //Decoding Related
    case DSP_UOPIDA:
    case DSP_UOPIDB:
        vDecUOP (wDspUop);
        break;
    case DSP_UOPIDC:
        vPostProcUOP (wDspUop);
        break;
    case DSP_UOPIDD:
        vAtvUOP (wDspUop);
        break;        
    default:
        ;
    }
    if ((_rDspVars[AUD_DEC_MAIN].bDspStrTyp == PCM_STREAM) && (!_rDspStatus[AUD_DEC_MAIN].fgDspStop))
    {
        if (_rDspStatus[AUD_DEC_MAIN].fgDspSInt)
        {
            if ((_uDspSIntAddr == INT_RC2D_FLOW_CONTROL)
                && (_u4DspSIntSD == (UINT32) FLOW_CONTROL_AOUT_REINIT))
            {
                // reset state machine for first decoder
            }
        }
    }
    if ((_rDspVars[AUD_DEC_AUX].bDspStrTyp == PCM_STREAM) && (!_rDspStatus[AUD_DEC_AUX].fgDspStop))
    {
        if (_rDspStatus[AUD_DEC_MAIN].fgDspSInt)
        {
            if ((_uDspSIntAddr == INT_RC2D_FLOW_CONTROL)
                && (_u4DspSIntSD == (UINT32) FLOW_CONTROL_AOUT_REINIT))
            {
                // reset state machine for second decoder
            }
        }
    }

    _rDspStatus[AUD_DEC_MAIN].fgDspUop = FALSE;     // Clear fgDspUop
    _rDspVars[AUD_DEC_MAIN].dwDspUop = 0;           // Clear _dwDspUop

}

/***************************************************************************
     Function : vDspIECConfig
  Description : Routine handling IEC Configuration
    Parameter : None
    Return    : None
***************************************************************************/
// DVD audio related
void vDspIECConfig (UINT8 u1DecId)
{
    UINT16 u2IecFlag = 0;

    u2IecFlag = (UINT16)(uReadDspShmBYTE (B_IECFLAG) & 0x03);
    u2IecFlag |= (UINT16)uReadDspShmBYTE (B_IEC_RAWSRC);

    _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;

    if (u1DecId == AUD_DEC_MAIN)
    {
        _uDspSIntAddr = INT_RC2D_DECODER_CONTROL;
        vWriteDspWORD (ADDR_RC2D_IEC_FLAG, u2IecFlag);
    }
    else
    {
        _uDspSIntAddr = INT_RC2D_DECODER_CONTROL_DEC2;
        vWriteDspWORD (ADDR_RC2D_IEC_FLAG_DEC2, u2IecFlag);
    }

    if (((u1DecId == AUD_DEC_MAIN) && _rDspStatus[AUD_DEC_MAIN].fgDspRealPlay) ||
        ((u1DecId == AUD_DEC_AUX) && _rDspStatus[AUD_DEC_AUX].fgDspRealPlay))
    {
        // Use Transparent reinit during playing period to prevent audio stop
        _u4DspSIntSD = (UINT32) FLOW_CONTROL_AOUT_REINIT+FLOW_CONTROL_TRANSPARENT_REINIT;
    }
    else
    {
        _u4DspSIntSD = (UINT32) FLOW_CONTROL_AOUT_REINIT;
    }

    // 20030926
    // Because it is the same meaning but use vDspCmd needs
    // a round of state change
    // vDspCmd(UOP_DSP_IEC_DOWN_SAMPLE);
    //vDspSetFreq (u1DecId, _rDspVars[AUD_DEC_MAIN].dwDacFreq, FALSE);
}

/***************************************************************************
     Function : fgDspAC3Uop
  Description : Routine AC3 User Operation
    Parameter : UOP
    Return    : Find UOP
***************************************************************************/
BOOL fgDspAC3Uop (UINT8 u1DecId, UINT16 wDspUop)
{
    BOOL fgRet;

    fgRet = TRUE;
    _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;

    if (u1DecId == AUD_DEC_MAIN)
    {    // Decoder 1               
        switch (wDspUop)
        {                          
        case UOP_DSP_AC3_KARAOKE_MODE:
            _uDspSIntAddr = INT_RC2D_INT32;
            _u4DspSIntSD = ((UINT32) u2ReadDspShmWORD (W_AC3KARAMOD)) << 8;
            break;
        case UOP_DSP_AC3_DUAL_MONO_MODE:
            _uDspSIntAddr = INT_RC2D_INT33;
            _u4DspSIntSD = ((UINT32) u2ReadDspShmWORD (W_AC3DUALMODE)) << 8;
            break;
        case UOP_DSP_AC3_COMPRESSION_MODE:
            _uDspSIntAddr = INT_RC2D_INT34;
            _u4DspSIntSD = ((UINT32) u2ReadDspShmWORD (W_AC3COMPMOD)) << 8;
            break;
        case UOP_DSP_AC3_DYNAMIC_LOW:
            _uDspSIntAddr = INT_RC2D_INT35;
            _u4DspSIntSD = u4ReadDspShmDWRD (D_AC3DYN_LOW);
            break;
        case UOP_DSP_AC3_DYNAMIC_HIGH:
            _uDspSIntAddr = INT_RC2D_INT36;
            _u4DspSIntSD = u4ReadDspShmDWRD (D_AC3DYN_HIGH);
            break;
        case UOP_DSP_AC3_AC3AUTODNMIX:
            _uDspSIntAddr = INT_RC2D_INT37;
            _u4DspSIntSD = (UINT32) (uReadDspShmBYTE (B_AC3AUTODNMIX)) << 8;
            break;
#ifdef CC_AUD_SUPPORT_MS10
        case UOP_DSP_DDC_CONTROL:
            _uDspSIntAddr = INT_RC2D_INT39;
            _u4DspSIntSD = (UINT32) (u2ReadDspShmWORD(W_DDC_CONTROL)) << 8;
            break;
        case UOP_DSP_DDC_STRMID:
            _uDspSIntAddr = INT_RC2D_INT3A;
            _u4DspSIntSD = (UINT32) (uReadDspShmBYTE (B_DDC_STRMID)) << 8;
            break;
        case UOP_DSP_DDC_ASSOC_MIX:
            _uDspSIntAddr = INT_RC2D_INT38;
            _u4DspSIntSD = (UINT32) (uReadDspShmBYTE (B_DDC_ASSOC_MIX)) << 8;
            break;
#endif     
        default:
            fgRet = FALSE;
            _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
        }
    }
    else if (u1DecId == AUD_DEC_AUX)
    {    // Decoder 2                
        switch (wDspUop)
        {                           
        case UOP_DSP_AC3_KARAOKE_MODE_DEC2:
            _uDspSIntAddr = INT_RC2D_INT52;
            _u4DspSIntSD = ((UINT32) u2ReadDspShmWORD (W_AC3KARAMOD_DEC2)) << 8;
            break;
        case UOP_DSP_AC3_DUAL_MONO_MODE_DEC2:
            _uDspSIntAddr = INT_RC2D_INT53;
            _u4DspSIntSD = ((UINT32) u2ReadDspShmWORD (W_AC3DUALMODE_DEC2)) << 8;
            break;
        case UOP_DSP_AC3_COMPRESSION_MODE_DEC2:
            _uDspSIntAddr = INT_RC2D_INT54;
            _u4DspSIntSD = ((UINT32) u2ReadDspShmWORD (W_AC3COMPMOD_DEC2)) << 8;
            break;
        case UOP_DSP_AC3_DYNAMIC_LOW_DEC2:
            _uDspSIntAddr = INT_RC2D_INT55;
            _u4DspSIntSD = u4ReadDspShmDWRD (D_AC3DYN_LOW_DEC2);
            break;
        case UOP_DSP_AC3_DYNAMIC_HIGH_DEC2:
            _uDspSIntAddr = INT_RC2D_INT56;
            _u4DspSIntSD = u4ReadDspShmDWRD (D_AC3DYN_HIGH_DEC2);
            break;
        case UOP_DSP_AC3_AC3AUTODNMIX_DEC2:
            _uDspSIntAddr = INT_RC2D_INT57;
            _u4DspSIntSD = (UINT32) (uReadDspShmBYTE (B_AC3AUTODNMIX_DEC2)) << 8;
            break;
#ifdef CC_AUD_SUPPORT_MS10
        case UOP_DSP_DDC_CONTROL_DEC2:
            _uDspSIntAddr = INT_RC2D_INT58;
            _u4DspSIntSD = (UINT32) (u2ReadDspShmWORD(W_DDC_CONTROL_DEC2)) << 8;
            break;
        case UOP_DSP_DDC_STRMID_DEC2:
            _uDspSIntAddr = INT_RC2D_INT59;
            _u4DspSIntSD = (UINT32) (uReadDspShmBYTE (B_DDC_STRMID_DEC2)) << 8;
            break;
        case UOP_DSP_DDC_ASSOC_MIX_DEC2:
            _uDspSIntAddr = INT_RC2D_INT5A;
            _u4DspSIntSD = (UINT32) (uReadDspShmBYTE (B_DDC_ASSOC_MIX_DEC2)) << 8;
            break;
#endif
        default:
            fgRet = FALSE;
            _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
        }
    }
    else
    {
        fgRet = FALSE;
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
    }
    
    return fgRet;
}
/***************************************************************************
     Function : fgDspAACUop
  Description : Routine AAC User Operation
    Parameter : UOP
    Return    : Find UOP
***************************************************************************/
BOOL fgDspAACUop (UINT8 u1DecId, UINT16 wDspUop)
{
    BOOL fgRet;

    fgRet = TRUE;
    _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;

    if (u1DecId == AUD_DEC_MAIN)
    {
        switch (wDspUop)
        {
#ifdef  CC_AUD_SUPPORT_MS10
        case UOP_DSP_DDT_DUAL_MODE:
            _uDspSIntAddr = INT_RC2D_INT33;
            _u4DspSIntSD = ((UINT32) uReadDspShmBYTE (B_DDT_DUALMODE)) << 8;
            break;
        case UOP_DSP_DDT_COMPVAL:
             _uDspSIntAddr = INT_RC2D_INT35;
            _u4DspSIntSD = ((UINT32) uReadDspShmBYTE (B_DDT_COMPVAL)) << 8;
            break;
        case UOP_DSP_DDT_COMPMODE:
            _uDspSIntAddr = INT_RC2D_INT34;
            _u4DspSIntSD = ((UINT32) uReadDspShmBYTE (B_DDT_COMPMODE)) << 8;
            break;
#endif
        default:
            fgRet = FALSE;
            _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
        }
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        switch (wDspUop)
        {    
#ifdef  CC_AUD_SUPPORT_MS10
        case UOP_DSP_DDT_DUAL_MODE_DEC2:
            _uDspSIntAddr = INT_RC2D_INT53;
            _u4DspSIntSD = ((UINT32) uReadDspShmBYTE (B_DDT_DUALMODE_DEC2)) << 8;
            break;
        case UOP_DSP_DDT_COMPVAL_DEC2:
            _uDspSIntAddr = INT_RC2D_INT55;
            _u4DspSIntSD = ((UINT32) uReadDspShmBYTE (B_DDT_COMPVAL_DEC2)) << 8;
            break;
        case UOP_DSP_DDT_COMPMODE_DEC2:
            _uDspSIntAddr = INT_RC2D_INT54;
            _u4DspSIntSD = ((UINT32) uReadDspShmBYTE (B_DDT_COMPMODE_DEC2)) << 8;
            break;
#endif
        default:
            fgRet = FALSE;
            _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
        }
    }
    else if (u1DecId == AUD_DEC_THIRD)
    {
        switch (wDspUop)
        {    
#ifdef  CC_AUD_SUPPORT_MS10    
        case UOP_DSP_DDT_DUAL_MODE_DEC3:
            _uDspSIntAddr = INT_RC2D_INT69;
            _u4DspSIntSD = ((UINT32) uReadDspShmBYTE (B_DDT_DUALMODE_DEC3)) << 8;
            break;
        case UOP_DSP_DDT_COMPVAL_DEC3:
            _uDspSIntAddr = INT_RC2D_INT6B;
            _u4DspSIntSD = ((UINT32) uReadDspShmBYTE (B_DDT_COMPVAL_DEC3)) << 8;
            break;
        case UOP_DSP_DDT_COMPMODE_DEC3:
            _uDspSIntAddr = INT_RC2D_INT6A;
            _u4DspSIntSD = ((UINT32) uReadDspShmBYTE (B_DDT_COMPMODE_DEC3)) << 8;
            break;
#endif
        default:
            fgRet = FALSE;
            _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
        }
    }
    else
    {
        fgRet = FALSE;
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
    }
    
    return fgRet;
}
/***************************************************************************
     Function : fgDspPCMUop
  Description : Routine PCM User Operation
    Parameter : UOP
    Return    : Find UOP
***************************************************************************/
BOOL fgDspPCMUop (UINT8 u1DecId, UINT16 wDspUop)
{
    BOOL fgRet;

    fgRet = TRUE;
    _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;

    if (u1DecId == AUD_DEC_MAIN)
    {
        switch (wDspUop)
        {    // Decoder 1
        case UOP_DSP_PCM_INPUT_CH_ASSIGNMENT:
            _uDspSIntAddr = INT_RC2D_INT33;
            _u4DspSIntSD = (UINT32) (uReadDspShmBYTE (W_PCM_INPUT_CH_ASSIGNMENT));
            break;
        case UOP_DSP_PCM_INPUT_CHANNEL_NUM:
            _uDspSIntAddr = INT_RC2D_INT34;
            _u4DspSIntSD = (UINT32) (uReadDspShmBYTE (W_PCM_INPUT_CHANNEL_NUM));
            break;
        default:
    	    fgRet = FALSE;
            _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
        }
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        switch (wDspUop)
        {    // Decoder 2
        case UOP_DSP_PCM_INPUT_CH_ASSIGNMENT_DEC2:
            _uDspSIntAddr = INT_RC2D_INT53;
            _u4DspSIntSD = (UINT32) (uReadDspShmBYTE (W_PCM_INPUT_CH_ASSIGNMENT));
            break;
        case UOP_DSP_PCM_INPUT_CHANNEL_NUM_DEC2:
            _uDspSIntAddr = INT_RC2D_INT54;
            _u4DspSIntSD = (UINT32) (uReadDspShmBYTE (W_PCM_INPUT_CHANNEL_NUM));
            break;
        default:
        	fgRet = FALSE;
            _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
        }
    }
    else 
    {
    	fgRet = FALSE;
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
    }
    
    return fgRet;
}

#if 0 ///1///def CC_VORBIS_SUPPORT ///def DSP_VORBIS_SUPPORT   ///[Joel] For middle play
BOOL vDspVorbisUop(UINT8 u1DecId, UINT16 wDspUop)
{
    BOOL fgRet;

    fgRet = TRUE;
    _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;    /// Disable Interrupt

    if (u1DecId == AUD_DEC_MAIN)
    {    // Decoder 1
        switch(wDspUop)
        {
        case UOP_DSP_MAKE_VORBIS_TAB:
            if(!_uVorbisMkTbl[AUD_DEC_MAIN])
            {
                vWriteDspShmWORD(W_VORBISPACKET, FALSE);
                vDspPlayCommonInit();       // decoder initial before playing
                _uDspSIntAddr = INT_RC2D_FLOW_CONTROL;
                _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE; ///Enable Interrupt
                _u4DspSIntSD = (UINT32)FLOW_CONTROL_PLAY;
                _uVorbisMkTbl[AUD_DEC_MAIN] = TRUE;      // Will be cleared at sendsampling rate
                _uVorbisTblDone[AUD_DEC_MAIN] = FALSE;
                _rDspStatus[AUD_DEC_MAIN].fgDspStopIssue = FALSE;
                _rDspStatus[AUD_DEC_MAIN].fgDspPlay = TRUE;
                _rDspStatus[AUD_DEC_MAIN].fgDspStop = FALSE;
            }
            break;
        default:
        	fgRet = FALSE;
            _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
            break;
        }
    }
    else if (u1DecId == AUD_DEC_AUX)
    {    // Decoder 2
        switch(wDspUop)
        {
        case UOP_DSP_MAKE_VORBIS_TAB_DEC2:
            if(!_uVorbisMkTbl[AUD_DEC_AUX])
            {
                vWriteDspShmWORD(W_VORBISPACKET, FALSE);
                vDspPlayCommonInitDec2();   // decoder initial before playing
                _uDspSIntAddr = INT_RC2D_FLOW_CONTROL_DEC2;
                _rDspStatus[AUD_DEC_AUX].fgDspSInt = TRUE; ///Enable Interrupt
                _u4DspSIntSD = (UINT32)FLOW_CONTROL_PLAY;
                _uVorbisMkTbl[AUD_DEC_AUX] = TRUE;      // Will be cleared at sendsampling rate
                _uVorbisTblDone[AUD_DEC_AUX] = FALSE;
                _rDspStatus[AUD_DEC_AUX].fgDspStopIssue = FALSE;
                _rDspStatus[AUD_DEC_AUX].fgDspPlay = TRUE;
                _rDspStatus[AUD_DEC_AUX].fgDspStop = FALSE;
            }
            break;
        default:
            fgRet = FALSE;
            _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
            break;
        }
    }
    else
    {
        fgRet = FALSE;
        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
    }
    
    return fgRet;
}
#endif


