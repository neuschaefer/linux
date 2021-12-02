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
 * $RCSfile: aud_dsp_cfg.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file aud_dsp_cfg.c
 *  Primitive APIs for audio and DSP configuration.
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
// KERNEL


LINT_EXT_HEADER_BEGIN
#include "x_util.h"
#include "x_typedef.h"
#include "x_os.h"
#include "x_bim.h"
#include "x_printf.h"
#include "x_assert.h"
#include "x_hal_5381.h"
#include "x_hal_io.h"
#include "aud_dsp_cfg.h"
#include "aud_debug.h"
#include "aud_drv.h"
#include "aud_cfg.h"
#include "drv_adsp.h"

#include "dsp_intf.h"
#include "dsp_common.h"
#include "dsp_uop.h"
#include "dsp_shm.h"
#include "drv_adsp.h"
#include "aud_if.h"
//#include "aud_hw.h"
#include "drvcust_if.h"
#include "adac_if.h"
//#include "drv_dsp_cfg.h"
//#include "dsp_func.h"
#include "dsp_table.h"



LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//#define CC_AUD_TV_ONLY_AGC
//#define CC_AUD_TPV_SPECIAL_AGC

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

// Audio channel number: AUD_CH_FRONT_LEFT = 0, AUD_CH_FRONT_RIGHT, AUD_CH_REAR_LEFT,
//                       AUD_CH_REAR_RIGHT, AUD_CH_CENTER, AUD_CH_SUB_WOOFER,
//                       AUD_CH_ALL

#define CHANNEL_VOL_MAX_SHM_VALUE   (0x20000)  /* 0 dB*/
#define MAX_VOL_LEVEL               (100)
#define VOL_ZERO_BASE               (0)

#define MAX_BALANCE_LEVEL           (100)

#define EQ_FLAG                     (1 << 0)
#define EQ_TREBLE_BOOST             (1 << 1)
#define EQ_SUPERBASS_BOOST          (1 << 2)
#define EQ_BASS_BOOST               (1 << 3)

#define VOL_SHM_0_DB                (0x20000)
#define VOL_SHM_NEG_6_DB            (0x20000/2)
#define VOL_SHM_NEG_12_DB           (0x20000/4)

#define INPUT_SRC_VOL_MAX           96
#define INPUT_SRC_VOL_MIN           -128

#define DSP_SHM_SPK_SIZE_MASK       ((UINT32)(0x1f) << 12)

/* used for ATV factory mode */
#define PAL_MASK (1 << 0)
#define A2_MASK (1 << 1)
#define MTS_MASK (1 << 2)
#define FMFM_MASK (1 << 3)
#ifdef CC_AUD_MTS_OUTPUT_HDEV_GAIN_SUPPORT
#define MTS_NEW_MASK (1 << 4)
#endif

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define VOL_CTL_SEMA_LOCK(u1DecId)      VERIFY(x_sema_lock(_ahVolCtlSema[u1DecId], X_SEMA_OPTION_WAIT) == OSR_OK)
#define VOL_CTL_SEMA_UNLOCK(u1DecId)    VERIFY(x_sema_unlock(_ahVolCtlSema[u1DecId]) == OSR_OK)

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
extern void vDspLoadRamCode (UINT32 u4Type);
#ifdef SUPPORT_PHILIPS_LOUDNESS
extern void AUD_DspChEqLoudness(UINT8 u1DecId, INT8 *prEqvalue);
#endif
extern void vHDMISpdifOutEnable(BOOL fgEnable);
extern void vHDMIAudChannelStatus(BOOL* pfgCopyright, UINT8* pu1CategoryCode, 
                                UINT8* p1WordLength, BOOL* pfgIsPcm);
extern AUD_WMAErr_NOTIFY _hWMAErrNotifyFunc; 
//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

static AUD_DEC_STREAM_FROM_T _AudDspEuAtvStrmSrcTransfer(AUD_DEC_STREAM_FROM_T eStreamFrom);
#define AUD_EU_ATV_STREAM_SRC_TRANSFER(eStrSrc) _AudDspEuAtvStrmSrcTransfer(eStrSrc)

#ifdef CC_AUD_USE_NVM
UINT32 AUD_NVM_Operation(AUD_NVM_OP_T uOP, UINT8* puNvmBuffer, UINT16 u2Offset);
#endif
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static HANDLE_T _ahVolCtlSema[AUD_DEC_NUM];
static HANDLE_T _ahEqCtlSema;
static HANDLE_T _ahChDelayCtlSema;
#ifdef CC_AUD_SOUND_MODE
static HANDLE_T _ahPSMCtlSema;
static HANDLE_T _ahVSMCtlSema;
#endif
static HANDLE_T _ahSpdifCtlSema;
#ifndef CC_MT5391_AUD_3_DECODER
//#if 1   
static UINT32   _au4AdjVol[AUD_DEC_NUM] = {VOL_SHM_0_DB, VOL_SHM_0_DB};
#else
static UINT32   _au4AdjVol[AUD_DEC_NUM] = {VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB};
#endif

// Customization table, will load via DRVCUST_OptQuery() function
static INT16  _aai2InputSrcVol[AUD_DEC_NUM][AUD_STREAM_FROM_NUM];
static UINT32 _aau4InputSrcAgcVol[AUD_DEC_NUM][AUD_STREAM_FROM_NUM]; 
static INT16  _ai2AvInputSrcVol[AUD_INPUT_ID_MAX];
static BOOL   _aafgInputSrcAgcFlag[AUD_DEC_NUM][AUD_STREAM_FROM_NUM]; 
static UINT8 _u1VolBaseLevel;
static UINT32 _au4VOLUME[MAX_VOL_LEVEL + 1];
#ifdef CC_AUD_NEW_CV_TABLE
static UINT32 _au4CHVOLUME[MAX_VOL_LEVEL + 1];
#endif
#ifdef CC_AUD_SOUND_MODE
static UINT16 _aai1PeqFq[AUD_SM_NUM][PEQ_SETTING_MAX];
static UINT16 _aai1PeqBw[AUD_SM_NUM][PEQ_SETTING_MAX];
static INT8 _aai1PeqGain[AUD_SM_NUM][PEQ_SETTING_MAX];
static UINT8 _aai1BassTreble[AUD_SM_NUM][2];
static AUD_POST_SM_T _arSoundModeCase[AUD_SOUND_MODE_NUM][AUD_STREAM_FROM_NUM];
#endif
#ifdef CC_AUD_VOLUME_OFFSET
static INT32 _aai4VolOffsetMap[AUD_VOLUME_OFFSET_IDX_NUM];
#endif
static INT8   _aai1ChEqCfg[AUD_EQ_NUM][CH_EQ_BAND_NO+1];
#ifdef CC_AUD_EQ_BASED_SBASS
static INT8   _ai1ChEqFinalCfg[CH_EQ_BAND_NO+1];
#endif
#ifdef CC_AUD_INPUT_SRC_LINEOUT_VOL
static INT16  _aai2InputSrcLineOutVol[AUD_DEC_NUM][AUD_STREAM_FROM_NUM];
#endif
#ifdef CC_NEW_POST_MULTI_VS_SUPPORT
static UINT32 _aai1VSMode[AUD_VS_NUM][AUD_SUR_PRAR_NUM];
#endif
static UINT16 _au2ChDelay[AUD_STREAM_FROM_NUM];
static UINT8 _au1IecCopyright[AUD_STREAM_FROM_NUM];
static UINT8 _au1IecCategory[AUD_STREAM_FROM_NUM+1];    // Last is for DTV copyright case
static UINT8 _au1IecWordLength[AUD_STREAM_FROM_NUM];
static INT16 _ai2AvcTargetLevel[AUD_STREAM_FROM_NUM];
static INT16 _ai2AvcGainUp[AUD_STREAM_FROM_NUM];
static INT16 _ai2AvcAdjRate[AUD_STREAM_FROM_NUM];

static BOOL   _aafgHdmiSetSpdifFlag=TRUE;
static BOOL   _aafgMWSetSpdifFlag=FALSE;
static AUD_IEC_T _aeMWIec;
#ifdef CC_AUD_LINEOUT_VOL_MODE
static UINT8  _aafgLineOutVolChangeFlag = 0;
static UINT8  _aau1LineOutFixVol;
#endif
#ifdef CC_ENABLE_AV_SYNC
static BOOL   _fgDecPlayMute=FALSE;
#endif

#ifdef CC_AUD_TV_SYS_FINE_TUNE_VOL
static UINT8  _aau1TVFineTuneVolume[SV_AUD_SYS_NUM];
#endif


// used for ATV factory mode fine tune volume
static UINT8 _u1MtsFineVol=20;
static UINT8 _u1SapFineVol=20;
static UINT8 _u1A2FineVol=20;
static UINT8 _u1PalFineVol=20;
static UINT8 _u1AMFineVol=20;
static UINT8 _u1NicamFineVol=20;
static UINT8 _u1FmfmMonoFineVol=20;
static UINT8 _u1FmfmDualFineVol=20;

static UINT8 _u1BbeMode = 0;
static INT8 _i1BbeLevel=0;
static UINT8 _u1BbeProcess=7*2;
static UINT8 _u1BbeLoContour=7*2;
static UINT8 _u1Bbe3dGain=80;

static const UINT32 _au4Ac3DrcRange[DRC_RANGE_MAX] =
{
    0x00000000,0x000FFFFF,0x001FFFFF,0x002FFFFF,0x003FFFFF,0x004FFFFF,0x005FFFFF,
    0x006FFFFF,0x007FFFFF
};

/*
static const UINT32 _au4VirSurGain[VSURR_GAIN_MAX] = 
{
    0x00001031,0x00001462,0x000019A9,0x0000204E,0x000028AB,0x00003333,0x00004075,0x00005126,
    0x00006628,0x0000809C,0x0000A1E9,0x0000CBD5,0x0001009C,0x0001430D,0x000196B2,0x00020000
};
*/

static const REVERB_MODE_T _arReverMode[AUD_REVERB_MODE_MAX] = {
    {0x200000, 79, 67, 59, 47},     /* 01 Concert */
    {0x180000, 41, 31, 23, 13},     /* 02 Living Room */
    {0x300000, 79, 59, 37, 19},     /* 03 Hall */
    {0x400000, 19, 17, 11,  7},     /* 04 Bathroom */
    {0x600000, 59, 47, 37, 23},     /* 05 Cave */
    {0x100000, 79, 73, 71, 67},     /* 06 Arena */
    {0x500000, 73, 67, 31, 11}      /* 07 Church */
};

#ifndef CC_AUD_EQ_BASED_SBASS
#ifndef DSP_EQ2_SBASS_SUPPORT
static const UINT32 _au4GainBand[BASS_TREBLE_GAIN_MAX] =
{
    0xFFFF3315,0xFFFF3950,0xFFFF404E,0xFFFF4827,0xFFFF50F5,0xFFFF5AD6,0xFFFF65EB,0xFFFF725A,
    0xFFFF804E,0xFFFF8FF6,0xFFFFA187,0xFFFFB53C,0xFFFFCB5A,0xFFFFE42A,0x00000000,0x00001F3C,
    0x00004248,0x0000699C,0x000095BB,0x0000C73D,0x0000FEC9,0x00013D1C,0x0001830A,0x0001D181,
    0x0002298B,0x00028C52,0x0002FB27,0x00037782,0x0004030A
};
#endif
#endif
// -10 ~ +10 dB, 0.5 dB/step
static const UINT32 _au4DryGain[EQ_DRY_GAIN_INDEX_MAX] =
{
    0x000050f4, 0x000055c0, 0x00005ad5, 0x00006037, 0x000065ea, 0x00006bf4, 0x0000725a, 0x00007920,
    0x0000804e, 0x000087e8, 0x00008ff6, 0x0000987d, 0x0000a186, 0x0000ab19, 0x0000b53c, 0x0000bff9,
    0x0000cb59, 0x0000d766, 0x0000e429, 0x0000f1ae, 0x00010000, 0x00010f2b, 0x00011f3d, 0x00013042,
    0x00014249, 0x00015562, 0x0001699c, 0x00017f09, 0x000195bc, 0x0001adc6, 0x0001c73d, 0x0001e237,
    0x0001feca, 0x00021d0e, 0x00023d1d, 0x00025f12, 0x0002830b, 0x0002a925, 0x0002d182, 0x0002fc42,
    0x0003298b
};

/*
static const UINT32 _au4ChEqUop[EQ_CH_MAX] = {
    UOP_DSP_EQUALIZER_CHANNEL_CHANGE + (0x00010000 * EQ_CH_C),
    UOP_DSP_EQUALIZER_CHANNEL_CHANGE + (0x00010000 * EQ_CH_L),
    UOP_DSP_EQUALIZER_CHANNEL_CHANGE + (0x00010000 * EQ_CH_R),
    UOP_DSP_EQUALIZER_CHANNEL_CHANGE + (0x00010000 * EQ_CH_LS),
    UOP_DSP_EQUALIZER_CHANNEL_CHANGE + (0x00010000 * EQ_CH_RS),
    UOP_DSP_EQUALIZER_CHANNEL_CHANGE + (0x00010000 * EQ_CH_CH7),
    UOP_DSP_EQUALIZER_CHANNEL_CHANGE + (0x00010000 * EQ_CH_CH8),
    UOP_DSP_EQUALIZER_CHANNEL_CHANGE_DEC2 + (0x00010000 * EQ_CH_L_DEC2),
    UOP_DSP_EQUALIZER_CHANNEL_CHANGE_DEC2 + (0x00010000 * EQ_CH_R_DEC2),
    UOP_DSP_EQUALIZER_CHANNEL_CHANGE + 0x01000000 // CH_ALL 
};
*/

#if (0)
static const UINT16 _au2ChEqShare[EQ_CH_MAX-1] = { /* not include channel all */
    DSP_EQ_BAND_START + (4 * EQ_BAND_MAX * EQ_CH_C),
    DSP_EQ_BAND_START + (4 * EQ_BAND_MAX * EQ_CH_L),
    DSP_EQ_BAND_START + (4 * EQ_BAND_MAX * EQ_CH_R),
    DSP_EQ_BAND_START + (4 * EQ_BAND_MAX * EQ_CH_LS),
    DSP_EQ_BAND_START + (4 * EQ_BAND_MAX * EQ_CH_RS),
    DSP_EQ_BAND_START + (4 * EQ_BAND_MAX * EQ_CH_CH7),
    DSP_EQ_BAND_START + (4 * EQ_BAND_MAX * EQ_CH_CH8),
    DSP_EQ_BAND_START_DEC2 + (4 * EQ_BAND_MAX * EQ_CH_L_DEC2),
    DSP_EQ_BAND_START_DEC2 + (4 * EQ_BAND_MAX * EQ_CH_R_DEC2)
};
#endif

#ifdef CC_AUD_7BAND_EQUALIZER
static const UINT8 _aau1EqBandExtend[CH_EQ_BAND_NO][2] =
{
    {0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 1}
};
#else
static const UINT8 _aau1EqBandExtend[CH_EQ_BAND_NO][2] =
{
    {0, 1}, {1, 2}, {3, 2}, {5, 2}, {7, 1}
};
#endif

static const AUD_EQ_BAND_INFO_T _arEqInfo[] = 
{
    {0, 20, -20, 20, 200},      // 100 Hz
    {1, 20, -20, 200, 500},     // 300 Hz
    {2, 20, -20, 500, 2000},    // 1K Hz
    {3, 20, -20, 2000, 6000},   // 3K Hz
    {4, 20, -20, 6000, 20000},  // 10K Hz
};

static PL2_SETTING_T _rPL2Setting;

// Channel volume relative
static UINT8 _aau1ChannelVolume[AUD_DEC_NUM][(UINT32)AUD_CH_NUM];
static INT32 _aau1ChlVolExtraGain[AUD_DEC_NUM][(UINT32)AUD_CH_NUM];
static UINT32 _aau4ChannelVolumeRawData[AUD_DEC_NUM][(UINT32)AUD_CH_NUM];
static BOOL  _aafgChlMute[AUD_DEC_NUM][(UINT32)AUD_CH_NUM]; 
static BOOL  _aafgChlOutPort[AUD_DEC_NUM][(UINT32)AUD_CH_NUM]; 
static BOOL  _aafgChlHdmiChgFmtMute[AUD_DEC_NUM][(UINT32)AUD_CH_NUM]; 
#ifdef CC_ENABLE_AV_SYNC
static BOOL  _aafgChDecPlayMute[AUD_DEC_NUM][(UINT32)AUD_CH_NUM]; 
#endif
#ifdef CC_AUD_LINEOUT_VOL_OFFSET
static UINT8 _aau1ChannelVolOffset[AUD_DEC_NUM][(UINT32)AUD_CH_NUM];
#endif
static AUD_IEC_T _aeIecMode = AUD_IEC_CFG_PCM;
static BOOL _afgIecEnable = TRUE;

#ifndef CC_MT5391_AUD_3_DECODER
//#if 1   
static UINT32 _au1Balance[2] = {(MAX_BALANCE_LEVEL/2), (MAX_BALANCE_LEVEL/2)};
static BOOL _afgMuteEnable[AUD_DEC_NUM] = {FALSE, FALSE};
static BOOL _afgStopMuteEnable[AUD_DEC_NUM] = {FALSE, FALSE};
static BOOL _afgHdmiModeMuteEnable[AUD_DEC_NUM] = {FALSE, FALSE};
static BOOL _afgAgcEnable[AUD_DEC_NUM] = {TRUE, FALSE};
static BOOL _afgUserAgcFlag[AUD_DEC_NUM] = {TRUE, FALSE};
static AUD_DEC_STREAM_FROM_T _aeStreamSrc[AUD_DEC_NUM] = {AUD_STREAM_FROM_OTHERS, AUD_STREAM_FROM_OTHERS};
static AUD_EQ_TYPE_T _aeEqMode[AUD_DEC_NUM] = {AUD_EQ_OFF, AUD_EQ_OFF};
#else
static UINT32 _au1Balance[3] = {(MAX_BALANCE_LEVEL/2), (MAX_BALANCE_LEVEL/2), (MAX_BALANCE_LEVEL/2)};
static BOOL _afgMuteEnable[AUD_DEC_NUM] = {FALSE, FALSE, FALSE};
static BOOL _afgStopMuteEnable[AUD_DEC_NUM] = {FALSE, FALSE, FALSE};
static BOOL _afgHdmiModeMuteEnable[AUD_DEC_NUM] = {FALSE, FALSE, FALSE};
static BOOL _afgAgcEnable[AUD_DEC_NUM] = {TRUE, FALSE, FALSE};

static BOOL _afgUserAgcFlag[AUD_DEC_NUM] = {TRUE, FALSE, FALSE};
static AUD_DEC_STREAM_FROM_T _aeStreamSrc[AUD_DEC_NUM] = {AUD_STREAM_FROM_OTHERS, AUD_STREAM_FROM_OTHERS,AUD_STREAM_FROM_OTHERS};
static AUD_EQ_TYPE_T _aeEqMode[AUD_DEC_NUM] = {AUD_EQ_OFF, AUD_EQ_OFF, AUD_EQ_OFF};
#endif
// Negtive dB table, in steps of 0.5 dB
// 0dB, -0.5dB, -1dB, -1.5dB, -2dB, -2.5dB,.....
static const UINT32 _au4NegDBTable[] = {
    0x00020000,0x0001e35b,0x0001c852,0x0001aecb,0x000196b2,0x00017ff2,
    0x00016a77,0x00015631,0x0001430c,0x000130fa,0x00011feb,0x00010fd0,
    0x0001009b,0x0000f240,0x0000e4b3,0x0000d7e8,0x0000cbd4,0x0000c06d,
    0x0000b5aa,0x0000ab80,0x0000a1e8,0x000098da,0x0000904d,0x0000883a,
    0x0000809b,0x0000796a,0x0000729f,0x00006c35,0x00006628,0x00006071,
    0x00005b0c,0x000055f4,0x00005125,0x00004c9b,0x00004852,0x00004446,
    0x00004074,0x00003cd9,0x00003972,0x0000363b,0x00003333,0x00003055,
    0x00002da1,0x00002b14,0x000028ab,0x00002665,0x0000243f,0x00002238,
    0x0000204e,0x00001e7f,0x00001cca,0x00001b2e,0x000019a9,0x00001839,
    0x000016de,0x00001597,0x00001462,0x0000133e,0x0000122a,0x00001126,
    0x00001030,0x00000f49,0x00000e6e,0x00000d9f,0x00000cdc,0x00000c24,
    0x00000b76,0x00000ad2,0x00000a37,0x000009a4,0x0000091a,0x00000898,
    0x0000081d,0x000007a9,0x0000073b,0x000006d3,0x00000672,0x00000615,
    0x000005be,0x0000056c,0x0000051e,0x000004d5,0x00000490,0x0000044e,
    0x00000411,0x000003d6,0x0000039f,0x0000036c,0x0000033b,0x0000030c,
    0x000002e1,0x000002b7,0x00000290,0x0000026c,0x00000249,0x00000228,
    0x00000209,0x000001ec,0x000001d1,0x000001b7,0x0000019e,0x00000187,
    0x00000171,0x0000015c,0x00000149,0x00000136,0x00000125,0x00000115,
    0x00000105,0x000000f6,0x000000e9,0x000000dc,0x000000cf,0x000000c4,
    0x000000b9,0x000000ae,0x000000a5,0x0000009b,0x00000093,0x0000008a,
    0x00000083,0x0000007b,0x00000074,0x0000006e,0x00000068,0x00000062,
    0x0000005c,0x00000057,0x00000052,0x0000004e,0x00000049,0x00000045,
    0x00000041,0x0000003e,0x0000003a,0x00000037,0x00000034,0x00000031,
    0x0000002e,0x0000002b,0x00000029,0x00000027,0x00000024,0x00000022,
    0x00000020,0x0000001f,0x0000001d,0x0000001b,0x0000001a,0x00000018,
    0x00000017,0x00000016,0x00000014,0x00000013,0x00000012,0x00000011,
    0x00000010,0x0000000f,0x0000000e,0x0000000d,0x0000000d,0x0000000c,
    0x0000000b,0x0000000b,0x0000000a,0x00000009,0x00000009,0x00000008,
    0x00000008,0x00000007,0x00000007,0x00000006,0x00000006,0x00000006,
    0x00000005,0x00000005,0x00000005,0x00000004,0x00000004,0x00000004,
    0x00000004,0x00000003,0x00000003,0x00000003,0x00000003,0x00000003,
    0x00000002,0x00000002,0x00000002,0x00000002,0x00000002,0x00000002,
    0x00000002,0x00000001,0x00000001,0x00000001,0x00000001,0x00000001,
    0x00000001,0x00000001,0x00000001,0x00000001,0x00000001,0x00000001,
    0x00000001,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000
};  

// Postive dB table, in steps of 0.5 dB
// 0dB, 0.5dB, 1dB, 1.5dB, 2dB, 2.5dB,.....
static const UINT32 _au4PosDBTable[] = {
    0x00020000,0x00021e56,0x00023e79,0x00026083,0x00028491,0x0002aac3,
    0x0002d338,0x0002fe12,0x00032b77,0x00035b8c,0x00038e7a,0x0003c46d,
    0x0003fd93,0x00043a1b,0x00047a39,0x0004be24,0x00050615,0x0005524a,
    0x0005a303,0x0005f884,0x00065316,0x0006b305,0x000718a5,0x00078449,
    0x0007f64f,0x00086f16,0x0008ef05,0x00097688,0x000a0614,0x000a9e20,
    0x000b3f30,0x000be9ca,0x000c9e80,0x000d5deb,0x000e28ae,0x000eff75,
    0x000fe2f5,0x0010d3f1,0x0011d334,0x0012e197,0x00140000,0x00152f61,
    0x001670bc,0x0017c521,0x00192db2,0x001aaba1,0x001c4031,0x001decb9,
    0x001fb2a7,0x0021937a,0x002390ca,0x0025ac48,0x0027e7be,0x002a4510,
    0x002cc640,0x002f6d6e,0x00323cdb,0x003536e9,0x00385e1e,0x003bb528,
    0x003f3edc,0x0042fe3b,0x0046f672,0x004b2ae0,0x004f9f16,0x005456dc,
    0x00595633,0x005ea158,0x00643cc9,0x006a2d48,0x007077e0,0x007721e6,
    0x007e3104,0x0085ab35,0x008d96d2,0x0095fa95,0x009edd9b,0x00a8476e,
    0x00b2400c,0x00bccfea,0x00c80000,0x00d3d9ca,0x00e06758,0x00edb350,
    0x00fbc8fb,0x010ab44c,0x011a81ec,0x012b3f43,0x013cfa88,0x014fc2c4,
    0x0163a7e7,0x0178bad5,0x018f0d6e,0x01a6b2a3,0x01bfbe85,0x01da4655
};

static UINT32 _u4PosDBTableSz = sizeof(_au4PosDBTable)/sizeof(UINT32);
static UINT32 _u4NegDBTableSz = sizeof (_au4NegDBTable)/sizeof(UINT32);

#ifndef CC_MT5391_AUD_3_DECODER
//#if 1    
static UINT32 _au4MasterVolShm[AUD_DEC_NUM] = {VOL_SHM_0_DB, VOL_SHM_0_DB};
#else
static UINT32 _au4MasterVolShm[AUD_DEC_NUM] = {VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB};
#endif
static BOOL   _fgMasterVolUseShmRawData = FALSE;
static BOOL   _fgChannelVolUseShmRawData = FALSE;
#ifdef SUPPORT_PHILIPS_LOUDNESS
static BOOL   _fgLoudnessEnable = FALSE;
#endif
//-------------------------------------------------------------------------------------
// NVM Releated declarations
//-------------------------------------------------------------------------------------
static UINT8  _au1VolStep[VOL_TABLE_CHK_PNT] = {0,16,32,49,66,83,100};
#ifdef CC_AUD_USE_NVM   // For customers use EEPROM and FLASH audio parameters
// ==== EEPROM Releated =====
static UINT8  auAudNvmBuffer[AUD_NVM_LENGTH];   //Buffer read from EEPROM
static BOOL   _fgNvmInit = FALSE;              
static UINT32 u4NvmVolTab[VOL_TABLE_CHK_PNT] = {0, 0x1030, 0x4ca0, 0xbb00, 0x13100, 0x17510, 0x20000};
static AUD_IPT_SRC_GAIN_T _rNVMIuputSrcGain;     // Input Source Gain   
static AUD_VSURR_CFG_T _rNVMSurrCfg;             // Surround 
static BOOL _fgNvmFlat = FALSE;                  // Flat Frequency Response
static AUD_NVM_DATA_T aeNvmData[AUD_NVM_LENGTH]; // Customer NVM Data Structure
// ==== PROFILE Releated ====
static UINT8  _u1AudProfile[3] = {0,0,0};        // The profile number to be referenced is stored in NVM
static BOOL   _fgAudNVMLipSync[3] = {FALSE,FALSE,FALSE}; 
static UINT16 _u2AudNVMProfileOffset[3] = {AUD_NVM_PROFILE_OFFSET1, AUD_NVM_PROFILE_OFFSET2, AUD_NVM_PROFILE_OFFSET3};
#ifdef SUPPORT_PHILIPS_LOUDNESS
static INT8 i1Loundness[CH_EQ_BAND_NO] = {+5,-3,+2,0,+2,+2,+5};
#endif
#endif //CC_AUD_USE_NVM

#ifdef CC_AUD_USE_FLASH_AQ
static UINT8 _au1AQChVolume[FLASH_AQ_COMM_SRC_NUM][FLASH_AQ_COMM_CH_NUM];
static UINT8 _au1AQLevel[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][AQ_LEVEL_ITEMS];
static UINT8 _au1AQBassTrebleOffset[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][AQ_BASS_ITEMS];
static UINT8 _au1AQ_EQLevel[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][AQ_EQ_ITEMS];
static UINT8 _au1AQ_DRC[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][AQ_DRC_ITEMS];
static UINT8 _au1AQ_AVC[FLASH_AQ_SRC_NUM][AQ_AVC_ITEMS];
static UINT32 _au4AQ_LIMITER[FLASH_AQ_SRC_NUM][AQ_LIMITER_ITEMS];
static UINT8 _au1AQ_SPH[FLASH_AQ_SRC_NUM][AQ_SPH_ITEMS];
static UINT32 _au4AQ_MVS[FLASH_AQ_SCENE_NUM][AQ_MVS_ITEMS];
static UINT32 _au4AQ_PostScale[AQ_POSTSCL_ITEMS];
static UINT8 _au1AQ_PEQ[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][AQ_PEQ_ITEMS];
static UINT8 _au1AQ_SP_Vol[FLASH_AQ_SRC_NUM];
static UINT8 _au1AQ_LO_Vol[FLASH_AQ_SRC_NUM];
static UINT32 _au4AQ_EXT_PEQ[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][AQ_AMP_FS_NUM][AQ_EXT_PEQ_ITEMS];
static BOOL  _fgAQ_Enable[AQ_TYPE_NUM];
#ifndef CC_AUD_NEW_AQ_PARSER
static UINT8 _au1AQSrc_Comm_Convert[AUD_STREAM_FROM_NUM] = {0, //AUD_STREAM_FROM_OTHERS 
                                                            3, //AUD_STREAM_FROM_DIGITAL_TUNER
                                                            1, //AUD_STREAM_FROM_ANALOG_TUNER
                                                            0, //AUD_STREAM_FROM_SPDIF,
                                                            0, //AUD_STREAM_FROM_LINE_IN
                                                            2, //AUD_STREAM_FROM_HDMI, 
                                                            4, //AUD_STREAM_FROM_MEMORY, 
                                                            4, //AUD_STREAM_FROM_BUF_AGT, 
                                                            4, //AUD_STREAM_FROM_FEEDER, 
                                                            4};//AUD_STREAM_FROM_MULTI_MEDIA
static UINT8 _au1AQSrc_Convert[AUD_STREAM_FROM_NUM] = {0, //AUD_STREAM_FROM_OTHERS 
                                                       3, //AUD_STREAM_FROM_DIGITAL_TUNER
                                                       2, //AUD_STREAM_FROM_ANALOG_TUNER
                                                       0, //AUD_STREAM_FROM_SPDIF,
                                                       0, //AUD_STREAM_FROM_LINE_IN
                                                       1, //AUD_STREAM_FROM_HDMI, 
                                                       1, //AUD_STREAM_FROM_MEMORY, 
                                                       1, //AUD_STREAM_FROM_BUF_AGT, 
                                                       1, //AUD_STREAM_FROM_FEEDER, 
                                                       1};//AUD_STREAM_FROM_MULTI_MEDIA                                                            
static UINT8 _au1AQSmod_Convert[AUD_SOUND_MODE_NUM+1] = {0,1,0,2,3}; //mapping to Dynamic, Standard, Clear, Cust
#else //CC_AUD_NEW_AQ_PARSER
static UINT8 _au1AQSrc_Comm_Convert[AUD_STREAM_FROM_NUM] = {0, //AUD_STREAM_FROM_OTHERS 
                                                            0, //AUD_STREAM_FROM_DIGITAL_TUNER
                                                            1, //AUD_STREAM_FROM_ANALOG_TUNER
                                                            0, //AUD_STREAM_FROM_SPDIF,
                                                            0, //AUD_STREAM_FROM_LINE_IN
                                                            2, //AUD_STREAM_FROM_HDMI, 
                                                            3, //AUD_STREAM_FROM_MEMORY, 
                                                            3, //AUD_STREAM_FROM_BUF_AGT, 
                                                            3, //AUD_STREAM_FROM_FEEDER, 
                                                            3};//AUD_STREAM_FROM_MULTI_MEDIA
static UINT8 _au1AQSrc_Convert[AUD_STREAM_FROM_NUM] = {0, //AUD_STREAM_FROM_OTHERS 
                                                       2, //AUD_STREAM_FROM_DIGITAL_TUNER
                                                       2, //AUD_STREAM_FROM_ANALOG_TUNER
                                                       0, //AUD_STREAM_FROM_SPDIF,
                                                       0, //AUD_STREAM_FROM_LINE_IN
                                                       1, //AUD_STREAM_FROM_HDMI, 
                                                       1, //AUD_STREAM_FROM_MEMORY, 
                                                       1, //AUD_STREAM_FROM_BUF_AGT, 
                                                       1, //AUD_STREAM_FROM_FEEDER, 
                                                       1};//AUD_STREAM_FROM_MULTI_MEDIA 
#endif  // CC_AUD_NEW_AQ_PARSER
#endif  // CC_AUD_USE_FLASH_AQ
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static AUD_DEC_STREAM_FROM_T _AudDspEuAtvStrmSrcTransfer(AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    if (eStreamFrom >=AUD_STREAM_FROM_NUM )
    {
        LOG(0, "Incorrect input src setting!\n");
        ASSERT(0);
    }
#ifndef CC_MT5391_AUD_3_DECODER
//#ifdef CC_AUD_DVBT_SUPPORT
    if(AUD_GetTargetCountry() == COUNTRY_EU)
	{
        if (eStreamFrom == AUD_STREAM_FROM_LINE_IN)
        {
            if (AUD_IsDualDecMode())
            {
                return AUD_STREAM_FROM_ANALOG_TUNER;
            }
        }
        return eStreamFrom;
    }
    else
    {
//#else
        return eStreamFrom;
    }
//#endif
#else
    return eStreamFrom;
#endif
}

AUD_IEC_T _GetIECFlag(UINT8 u1DecId)
{
    return _aeIecMode;    
}

#if 0
static void _DSP_IecOff()
{
    vWriteShmUINT8(B_IECFLAG, (UINT8)AUD_IEC_CFG_PCM);
    vWriteShmUINT8(B_IEC_MUTE, 0x1);        
    DSP_SendDspTaskCmd(UOP_DSP_IEC_FLAG);

    x_thread_delay(100);
    _afgIecEnable = FALSE;
}
#endif

static UINT32 _DbToShm(INT32 i4Level)
{
    INT32 i4Val;

    // Check max and min value
    if (i4Level >= (INT32)_u4PosDBTableSz)
    {
        i4Val = (INT32)_u4PosDBTableSz;    
    }
    else if (i4Level < (-(INT32)_u4NegDBTableSz))
    {
        i4Val = -(INT32)_u4NegDBTableSz;   
    }
    else
    {
        i4Val = i4Level;
    }
    
    return (i4Val <= 0) ? _au4NegDBTable[-i4Val] : _au4PosDBTable[i4Val];
}

static UINT8 _BalancedVolume(AUD_CH_T eChannel, UINT8 u1Balance, UINT8 u1Volume)
{
    UINT32 u4BalancedVolume = 0;

    if (u1Balance == (MAX_BALANCE_LEVEL/2))
    {
        u4BalancedVolume = u1Volume;
    }
    else
    {
        if (u1Balance < (MAX_BALANCE_LEVEL/2))
        {
            if ((eChannel == AUD_CH_FRONT_RIGHT) ||
                (eChannel == AUD_CH_REAR_RIGHT) ||
                (eChannel == AUD_CH_DMX_RIGHT))
            {
                u4BalancedVolume = ((u1Balance*2) * u1Volume) / MAX_VOL_LEVEL;
            }
            else
            {
                u4BalancedVolume = u1Volume;
            }
        }
        else
        {
            if ((eChannel == AUD_CH_FRONT_LEFT) ||
                (eChannel == AUD_CH_REAR_LEFT) ||
                (eChannel == AUD_CH_DMX_LEFT))
            {
                u4BalancedVolume = (((MAX_VOL_LEVEL - u1Balance)*2) * u1Volume) / MAX_VOL_LEVEL;
            }   
            else
            {
                u4BalancedVolume = u1Volume;
            }
            
        }
    }

    return (UINT8)u4BalancedVolume;
}

static void _VolumeCtrl(UINT8 u1DecId, AUD_CH_T eChannel, UINT32 u4Value)
{
    UINT16 u2ShmIndex = 0;
    UINT16 u2UopIndex = 0;
    UINT32 u4VolumeShm = 0; /* 0 ~ 0x7fffff */

    u4VolumeShm = u4Value;
    
    if (u1DecId == AUD_DEC_MAIN)
    {
        switch (eChannel)
        {
            case AUD_CH_FRONT_LEFT:
                u2ShmIndex = D_TRIM_L;
                u2UopIndex = UOP_DSP_TRIM_L;
                break;
            case AUD_CH_FRONT_RIGHT:
                u2ShmIndex = D_TRIM_R;
                u2UopIndex = UOP_DSP_TRIM_R;
                break;
            case AUD_CH_REAR_LEFT:
                u2ShmIndex = D_TRIM_LS;
                u2UopIndex = UOP_DSP_TRIM_LS;
                break;
            case AUD_CH_REAR_RIGHT:
                u2ShmIndex = D_TRIM_RS;
                u2UopIndex = UOP_DSP_TRIM_RS;
                break;
            case AUD_CH_CENTER:
                u2ShmIndex = D_TRIM_C;
                u2UopIndex = UOP_DSP_TRIM_C;
                break;  
            case AUD_CH_SUB_WOOFER:
                u2ShmIndex = D_TRIM_SUB;
                u2UopIndex = UOP_DSP_TRIM_SUBWOOFER;
                break;
            case AUD_CH_BYPASS_LEFT:
                u2ShmIndex = D_TRIM_CH7;
                u2UopIndex = UOP_DSP_TRIM_CH7;
                break;  
            case AUD_CH_BYPASS_RIGHT:
                u2ShmIndex = D_TRIM_CH8;
                u2UopIndex = UOP_DSP_TRIM_CH8;
                break;       
            case AUD_CH_DMX_LEFT:
                u2ShmIndex = D_TRIM_CH9;
                u2UopIndex = UOP_DSP_TRIM_CH9;
                break;
            case AUD_CH_DMX_RIGHT:
                u2ShmIndex = D_TRIM_CH10;
                u2UopIndex = UOP_DSP_TRIM_CH10;
                break;
            case AUD_CH_ALL: /* MASTER_VOLUME */
                u2ShmIndex = D_VOL;
                u2UopIndex = UOP_DSP_MASTER_VOLUME;
                break; 
            case AUD_CH_LFE:
            	u2ShmIndex = D_TRIM_LFE;
                u2UopIndex = UOP_DSP_TRIM_LFE;
            	break;                     
            default:
                break;
        }
    }
#ifndef CC_MT5391_AUD_3_DECODER    
//#if 1
    else
#else
    else if (u1DecId == AUD_DEC_AUX)
#endif
    {
        u2ShmIndex = D_VOL_DEC2;
        u2UopIndex = UOP_DSP_MASTER_VOLUME_DEC2;

        switch(eChannel)
        {
            case AUD_CH_FRONT_LEFT:
                u2ShmIndex = D_TRIM_L_DEC2;
                u2UopIndex = UOP_DSP_TRIM_L_DEC2;
                break;
            case AUD_CH_FRONT_RIGHT:
                u2ShmIndex = D_TRIM_R_DEC2;
                u2UopIndex = UOP_DSP_TRIM_R_DEC2;
                break;
            case AUD_CH_ALL: /* MASTER_VOLUME */
                u2ShmIndex = D_VOL_DEC2;
                u2UopIndex = UOP_DSP_MASTER_VOLUME_DEC2;
                break;
                
            default:
                return;
        }
    }
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
    else
    {
        u2ShmIndex = D_VOL_DEC3;
        u2UopIndex = UOP_DSP_MASTER_VOLUME_DEC3;
    }
#endif
  
    vWriteShmUINT32(u2ShmIndex, u4VolumeShm);
    DSP_SendDspTaskCmd(u2UopIndex);
}

static void _AudDspChannelVolChange(UINT8 u1DecId, AUD_CH_T eChl)
{
    AUD_DEC_STREAM_FROM_T eStreamSrc; 
    UINT8  u1Value;
    UINT32 u4Volumn;
    UINT64 u8ChlVol;
    UINT32 u4ChlMute = FALSE;
    UINT32 u4HdmiChgMute = FALSE;
#ifdef CC_ENABLE_AV_SYNC
    UINT8 u1DecPlayMute = 1;
#endif

    eStreamSrc = _aeStreamSrc[u1DecId];

    // Channel volume = ChannelMute * Balance * ExtraGain

    // Caculate balance ..
    if (!_fgChannelVolUseShmRawData)
    {
        u1Value = _BalancedVolume(eChl, _au1Balance[u1DecId], _aau1ChannelVolume[u1DecId][eChl]);
        if (u1Value >MAX_VOL_LEVEL) u1Value = MAX_VOL_LEVEL;
#ifndef CC_AUD_NEW_CV_TABLE
        u4Volumn = _au4VOLUME[u1Value];
#else
		u4Volumn = _au4CHVOLUME[u1Value];
#endif
    }
    else
    {
        // Notes that channel volume with raw data doen't support balance function
        u4Volumn = _aau4ChannelVolumeRawData[u1DecId][eChl];
    }

    // Caculate extra gain    
    u8ChlVol = (UINT64)u4Volumn * ((UINT64)(_DbToShm(_aau1ChlVolExtraGain[u1DecId][eChl])));
    u4Volumn = (UINT32)(u8ChlVol / VOL_SHM_0_DB);

    // Caculate channel mute
    // u4Volumn = _aafgChlMute[u1DecId][eChl] ? 0 : u4Volumn;
    
    if ((!_aafgChlMute[u1DecId][eChl]) && _aafgChlOutPort[u1DecId][eChl])
    {
        u4ChlMute = 1;
    }
    else
    {
        u4ChlMute = 0;
    }

#ifdef CC_AUD_INPUT_SRC_LINEOUT_VOL
	if ((eChl == AUD_CH_BYPASS_LEFT)||(eChl == AUD_CH_BYPASS_RIGHT))
	{
			
			VERIFY(eStreamSrc < AUD_STREAM_FROM_NUM);
            u8ChlVol = (UINT64)u4Volumn * ((UINT64)(_DbToShm(_aai2InputSrcLineOutVol[u1DecId][eStreamSrc])));	
            u4Volumn = (UINT32)(u8ChlVol / VOL_SHM_0_DB);
            u4Volumn *= u4ChlMute;

            LOG(7, "Bypass Channle Vol = %x\n", u4Volumn);

#ifdef CC_AUD_LINEOUT_VOL_OFFSET

			
			VERIFY(eChl < AUD_CH_NUM);
            u1Value = _aau1ChannelVolOffset[u1DecId][eChl];
			if (u1Value >MAX_VOL_LEVEL) u1Value = MAX_VOL_LEVEL;

			u8ChlVol = (UINT64)u4Volumn * (UINT64)(_au4CHVOLUME[u1Value]);
            u4Volumn = (UINT32)(u8ChlVol / VOL_SHM_0_DB);
			
            LOG(7, "Bypass Channle Vol offset = %x\n", u4Volumn);
#endif


	}
#endif /* CC_AUD_INPUT_SRC_LINEOUT_VOL */   

    u4HdmiChgMute = _aafgChlHdmiChgFmtMute[u1DecId][eChl] ? 0 : 1;

#ifdef CC_ENABLE_AV_SYNC
    u1DecPlayMute = _aafgChDecPlayMute[u1DecId][eChl] ? 0 : 1;

    if (eStreamSrc == AUD_STREAM_FROM_HDMI)
    {
        u4Volumn = u4ChlMute *  u4HdmiChgMute * u4Volumn * u1DecPlayMute;
    }
    else
    {
        u4Volumn = u4ChlMute *  u4Volumn * u1DecPlayMute;
    }
#else
    if (eStreamSrc == AUD_STREAM_FROM_HDMI)
    {
        u4Volumn = u4ChlMute *  u4HdmiChgMute * u4Volumn;
    }
    else
    {
        u4Volumn = u4ChlMute *  u4Volumn;
    }
#endif 
    
    _VolumeCtrl(u1DecId, eChl, u4Volumn);
}

static void _AudDspMasterVolChange(UINT8 u1DecId)
{
    AUD_DEC_STREAM_FROM_T eStreamSrc; 
    UINT32 u4ValShm;
    UINT64 u8ValueShm;
    BOOL fgMute;
    
    eStreamSrc = _aeStreamSrc[u1DecId];
    
    // Apply AGC flag and calculate adjusted volume
    if (_afgUserAgcFlag[u1DecId])
    {
        UINT64 u8AgcAdj;
        u8AgcAdj = (_aafgInputSrcAgcFlag[u1DecId][AUD_EU_ATV_STREAM_SRC_TRANSFER(eStreamSrc)] && _afgUserAgcFlag[u1DecId]) ?
                   (UINT64)_aau4InputSrcAgcVol[u1DecId] [AUD_EU_ATV_STREAM_SRC_TRANSFER(eStreamSrc)] : (UINT64)VOL_SHM_0_DB;
        _au4AdjVol[u1DecId] = (UINT32)(u8AgcAdj);                           
    }
    else
    {
        _au4AdjVol[u1DecId] = VOL_SHM_0_DB;
    }

    // Check if mute
    if (eStreamSrc == AUD_STREAM_FROM_HDMI) //HDMI source
    {
        // Apply mute control
        fgMute =  (_afgMuteEnable[u1DecId] || _afgHdmiModeMuteEnable[u1DecId] ||_afgStopMuteEnable[u1DecId]) ? TRUE : FALSE;
        if (fgMute)
        {
            LOG(9, "**** AUD_MUTE ****\n");
        }
        else
        {
            LOG(9, "**** AUD_UnMUTE ****\n");
        }   
    }
    else //Other source;
    {
        fgMute = (_afgMuteEnable[u1DecId]||_afgStopMuteEnable[u1DecId])? TRUE : FALSE;
    }

    // Master volume = Mute * AdjVol * Master volume   
    if (_fgMasterVolUseShmRawData)
    {
        u8ValueShm = (((UINT64)_au4MasterVolShm[u1DecId]) * ((UINT64)_au4AdjVol[u1DecId]))/(UINT64)VOL_SHM_0_DB;
    }
    else
    {
        u8ValueShm = (((UINT64)_au4VOLUME[_aau1ChannelVolume[u1DecId][AUD_CH_ALL]]) * ((UINT64)_au4AdjVol[u1DecId]))/(UINT64)VOL_SHM_0_DB;
    }
    
    u4ValShm = (UINT32)u8ValueShm;

    u4ValShm = fgMute ? 0 : u4ValShm;

    // Send UOP to DSP  
    _VolumeCtrl(u1DecId, AUD_CH_ALL, u4ValShm);
            
    // Auto turn off speaker when master volume is 0
    if ((DRVCUST_OptGet(eAudioSpkAutoOffWhenVolMute) == 1) && (u1DecId == AUD_DEC_MAIN))
    {
        if (u4ValShm == 0)
        {
            // Wait DSP fade out
            x_thread_delay(50);
            ADAC_VolumeMuteSpeakerEnable(FALSE);
            LOG(8, "**** SPK Auto off due to volume is 0\n");;
        }
        else
        {
            ADAC_VolumeMuteSpeakerEnable(TRUE);
            LOG(8, "**** SPK Auto on due to volume is not 0\n");;
        }
    }
    
}


static void _AudDspSrcVolChange(UINT8 u1DecId, UINT32 u4Value)
{
    UINT16 u2ShmIndex = 0; //Andrew Wen 061026 
    UINT16 u2UopIndex = 0;
#ifndef CC_MT5391_AUD_3_DECODER
//#if 1
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
     VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD));
#endif
    
    if (u1DecId == AUD_DEC_MAIN)
    {
        u2ShmIndex = D_INSRC_GAIN;
        u2UopIndex = UOP_DSP_INPUT_SRC_GAIN;             
    }
#ifndef CC_MT5391_AUD_3_DECODER   
//#if 1
    else
#else
    else if (u1DecId == AUD_DEC_AUX)
#endif
    {
        u2ShmIndex = D_INSRC_GAIN_DEC2;
        u2UopIndex = UOP_DSP_INPUT_SRC_GAIN_DEC2;               
    }
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
    else
    {
        u2ShmIndex = D_INSRC_GAIN_DEC3;
        u2UopIndex = UOP_DSP_INPUT_SRC_GAIN_DEC3;             
    }
#endif

    vWriteShmUINT32(u2ShmIndex, u4Value);
    DSP_SendDspTaskCmd(u2UopIndex);        
}

static void _AudDspAgcEnalbe(UINT8 u1DecId, BOOL fgEnable)
{
    UINT16 u2UopIndex = UOP_DSP_AVC_CONTROL;
    VERIFY((u1DecId == AUD_DEC_MAIN)||(u1DecId == AUD_DEC_AUX));

    if (u1DecId == AUD_DEC_MAIN)
    {
        if (_afgAgcEnable[AUD_DEC_MAIN] != fgEnable)
        {
            if (fgEnable) 
            {
                vWriteShmUINT16(W_AVC_FLAG,0x23); // enable AVC slow mode for UI enable/disable
            }
            else
            {
                vWriteShmUINT16(W_AVC_FLAG,0);
            }
            DSP_SendDspTaskCmd(u2UopIndex);
            LOG(8, "AUD AGC FLAG = %x\n", fgEnable);
            //AUD_DspProcMode(u1DecId, (fgEnable == TRUE) ? 0xc00 : 0xfc00); // FIXME !!
        }
        _afgAgcEnable[AUD_DEC_MAIN] = fgEnable;
    }
    else // 2nd decoder doesn't support AVC (post-processing)
    {
        if (_afgAgcEnable[AUD_DEC_AUX] != fgEnable)
        {
            if (fgEnable) 
            {
                vWriteShmUINT16(W_AVC_FLAG_DEC2,0x23); // enable AVC slow mode for UI enable/disable
            }
            else
            {
                vWriteShmUINT16(W_AVC_FLAG_DEC2,0);
            }
            DSP_SendDspTaskCmd(u2UopIndex);
            LOG(8, "AUD AGC FLAG = %x\n", fgEnable);
            //AUD_DspProcMode(u1DecId, (fgEnable == TRUE) ? 0xc00 : 0xfc00); // FIXME !!
        }
        _afgAgcEnable[AUD_DEC_AUX] = fgEnable;
    }
}

static void _AudDspSetIec(AUD_IEC_T eIecCfg, BOOL fgEnable)
{
    AUD_IEC_T eIecFlag;
    UINT8 u1fgMute;
    UINT8 u1Copyright, u1CategoryCode, u1WordLength;

    //added by ling
    if (_aeStreamSrc[AUD_DEC_MAIN] == AUD_STREAM_FROM_OTHERS)
    {
        return;
    }

    eIecFlag = eIecCfg;
    
    // Check copyright and category code
    u1Copyright = _au1IecCopyright[(UINT8)_aeStreamSrc[AUD_DEC_MAIN]];
    u1CategoryCode = _au1IecCategory[(UINT8)_aeStreamSrc[AUD_DEC_MAIN]];
    u1WordLength = _au1IecWordLength[(UINT8)_aeStreamSrc[AUD_DEC_MAIN]];
    
    if ((_aeStreamSrc[AUD_DEC_MAIN] == AUD_STREAM_FROM_LINE_IN) &&
        (_AUD_DspGetAvInputSrcId() >= AUD_INPUT_ID_HDMI_0) &&
        (_AUD_DspGetAvInputSrcId() <= AUD_INPUT_ID_HDMI_4))
    {
        // DVI mode
        u1Copyright = _au1IecCopyright[AUD_STREAM_FROM_OTHERS];
        u1CategoryCode = _au1IecCategory[AUD_STREAM_FROM_OTHERS];
        u1WordLength = _au1IecWordLength[AUD_STREAM_FROM_OTHERS];
    }
#if 0    //MT5387Merge  temp mark since there is no vHDMIAudChannelStatus in drv_hdmi.c
    else if ((_aeStreamSrc[AUD_DEC_MAIN] == AUD_STREAM_FROM_HDMI) &&
        (u1Copyright == IEC_CH_STATUS_BY_SOURCE))
    {
        BOOL fgCopyright, fgIsPcm;
        UINT8 u1CategoryCodeTmp, u1WordLengthTmp;

        UNUSED(fgIsPcm);
            
        vHDMIAudChannelStatus(&fgCopyright, &u1CategoryCodeTmp, &u1WordLengthTmp, &fgIsPcm);

        if (fgCopyright)
        {
            u1Copyright = IEC_COPY_NEVER;
        }
        else
        {
            u1Copyright = IEC_COPY_FREELY;
        }
    }
    else if ((_aeStreamSrc[AUD_DEC_MAIN] == AUD_STREAM_FROM_HDMI) &&
        (u1CategoryCode == IEC_CH_STATUS_BY_SOURCE))
    {
        BOOL fgCopyright, fgIsPcm;
        UINT8 u1CategoryCodeTmp, u1WordLengthTmp;

        UNUSED(fgIsPcm);
            
        vHDMIAudChannelStatus(&fgCopyright, &u1CategoryCodeTmp, &u1WordLengthTmp, &fgIsPcm);
        u1CategoryCode = u1CategoryCodeTmp;
    }
#endif    

    // Update to share info if not DTV by source
    if (!((_aeStreamSrc[AUD_DEC_MAIN] == AUD_STREAM_FROM_DIGITAL_TUNER) &&
        (u1Copyright == IEC_CH_STATUS_BY_SOURCE)))
    {
        vWriteShmUINT8(B_IEC_COPYRIGHT, u1Copyright);
    }

    if (!((_aeStreamSrc[AUD_DEC_MAIN] == AUD_STREAM_FROM_DIGITAL_TUNER) &&
        (u1CategoryCode == IEC_CH_STATUS_BY_SOURCE)))
    {
        vWriteShmUINT8(B_IEC_CATEGORY_CODE, u1CategoryCode);
    }    

    vWriteShmUINT8(B_IEC_WORD_LENGTH, u1WordLength);    
    
    //check HDMI's command
    if (_aeStreamSrc[AUD_DEC_MAIN] == AUD_STREAM_FROM_HDMI)
    {
        fgEnable = _aafgHdmiSetSpdifFlag && _aafgMWSetSpdifFlag;
    }
    else
    {
        fgEnable = _aafgMWSetSpdifFlag;
    }
    vHDMISpdifOutEnable(fgEnable);

    // set default
    AUD_SPDIF_Output_HDMI_Rx (FALSE);

    if (_fgDspWakeupOK == TRUE)
    {
        if (_aeStreamSrc[AUD_DEC_MAIN] == AUD_STREAM_FROM_HDMI)
        {
            if (eIecFlag == AUD_IEC_CFG_RAW)
            {
                if (_bDspStrTyp == AC3_STREAM)
                {
                    LOG(5,"Enable HDMI bypass mode by AC-3 RAW input\n");	
                    AUD_SPDIF_Output_HDMI_Rx (TRUE);
                }
            }
            else
            {
                LOG(5,"Disable HDMI bypass mode\n");	
            }
        }
    }

    if (fgEnable)
    {
        u1fgMute = FALSE;
    }   
    else
    {
        u1fgMute = TRUE;
    }

    // Convert IEC type for RAW type
    if (_fgDspWakeupOK == TRUE)
    {
        if (eIecFlag == AUD_IEC_CFG_RAW)
        {
            eIecFlag = AUD_IEC_CFG_PCM;

#ifdef CC_MT5391_AUD_3_DECODER
            if ((_aeStreamSrc[AUD_DEC_MAIN] == AUD_STREAM_FROM_DIGITAL_TUNER) &&
                (_AudGetStrFormat(AUD_DEC_AUX) == AUD_FMT_AC3))
            {
                eIecFlag = AUD_IEC_CFG_RAW;
            }
            else if ((_aeStreamSrc[AUD_DEC_MAIN] == AUD_STREAM_FROM_DIGITAL_TUNER) &&
                (_bDspStrTypDec2 == AAC_STREAM) &&
                (_dwDspFreqDec2 == SFREQ_48K) &&
                (AUD_IsDecoderRealPlay(AUD_DEC_AUX)))
            {
                UINT8 u1Mode;
                
                u1Mode= u1GetAacChNum(AUD_DEC_AUX);
                
                if ((u1Mode >=1) && ( u1Mode <=8))
                {
                    _AUD_DspSetDDCOEnable(TRUE);
                    LOG(5,"Enable DDCO by IEC\n");	
                
                    eIecFlag = AUD_IEC_CFG_RAW;
                }
            }
            //else if (_bDspStrTyp == AC3_STREAM)
            //koro: for DTV 3 decoder case, AC3 should not be in dec1
            else if ((_AudGetStrFormat(AUD_DEC_MAIN) == AUD_FMT_AC3) && (_aeStreamSrc[AUD_DEC_MAIN] != AUD_STREAM_FROM_DIGITAL_TUNER))
            {
                eIecFlag = AUD_IEC_CFG_RAW;
            }
            else
            {
                _AUD_DspSetDDCOEnable(FALSE);
                LOG(5,"Disable DDCO by IEC\n");
            }
#else
            if (_bDspStrTyp == AC3_STREAM)
            {
                eIecFlag = AUD_IEC_CFG_RAW;
            }
#endif             
        }
#ifdef CC_MT5391_AUD_3_DECODER        
        else
        {
            _AUD_DspSetDDCOEnable(FALSE);
            LOG(5,"Disable DDCO by IEC\n");
        }
#endif        
    }

    if ((_aeIecMode != eIecFlag) || (_afgIecEnable != fgEnable))
    {     
        // If PCM -> RAW, disable output first, then switch to RAW to avoid noise 
        // for bad design decoder, bad code FIXME !!
        //if ((_aeIecMode == AUD_IEC_CFG_PCM) && _afgIecEnable &&
        //    (eIecFlag == AUD_IEC_CFG_RAW) && fgEnable)
       // {
            //_DSP_IecOff();
       // }

        vWriteShmUINT8(B_IECFLAG, eIecFlag);
        vWriteShmUINT8(B_IEC_MUTE, u1fgMute);
        
        DSP_SendDspTaskCmd(UOP_DSP_IEC_FLAG);
    }

    _aeIecMode = eIecFlag;
    _afgIecEnable = fgEnable;    
}

//-----------------------------------------------------------------------------
/** AUD_DspVolumeChange
 *   Setup master volume, or input source gain, or channel trim to DSP.
 *
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *  @param  eAudChl  : 0: front left channel. 
 *                              1: front right channel.  
 *                              2: rear left channel. 
 *                              3: rear right channel. 
 *                              4: center channel.
 *                              5: sub woofer channel. 
 *                              6: bypass left channel. 
 *                              7: bypass right channel. 
 *                              8: downmix left channel. 
 *                              9: downmix right channel.
 *                            10: aux front left channel.
 *                            11: aux front right channel.
 *                            12: input source gain.
 *                            13: all channels    ( For master volume control ). 
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspVolumeChange(UINT8 u1DecId, AUD_CH_T eAudChl)
{
    AUD_DEC_STREAM_FROM_T eStreamSrc; 

#ifndef CC_MT5391_AUD_3_DECODER
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
     VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD));
#endif    

    if (eAudChl >= AUD_CH_NUM)
    {
        return;
    }
    
    eStreamSrc = AUD_EU_ATV_STREAM_SRC_TRANSFER(_aeStreamSrc[u1DecId]);
        
    if (eAudChl == AUD_CH_ALL)
    {
        _AudDspMasterVolChange(u1DecId);
    }
    else if (eAudChl == AUD_CH_INPUT_SRC)
    {
        INT32 i4InputSrcVol;

        if (eStreamSrc == AUD_STREAM_FROM_LINE_IN)
        {
            // i4InputSrcVol = InputSrcVol + AvInputSrcVol (in dB)
            i4InputSrcVol = _aai2InputSrcVol[u1DecId][eStreamSrc]  + _ai2AvInputSrcVol[AUD_DspGetAvInputSrcId()];
            _AudDspSrcVolChange(u1DecId, _DbToShm(i4InputSrcVol));
        }
        else
        {
            _AudDspSrcVolChange(u1DecId, _DbToShm(_aai2InputSrcVol[u1DecId][eStreamSrc]));
        }   
    }
#ifndef CC_MT5391_AUD_3_DECODER   // gallen 0821,    
    else    // channel volume
#else
    else if ((u1DecId == AUD_DEC_MAIN)||(u1DecId == AUD_DEC_AUX))
#endif
    {
        _AudDspChannelVolChange(u1DecId, eAudChl);
    }
}

void _AUD_DspCfgSetDecType(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eDecFmt)
{
    UNUSED(eDecFmt);
#ifndef CC_MT5391_AUD_3_DECODER
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
     VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD));
#endif
    VOL_CTL_SEMA_LOCK(u1DecId); 

    _aeStreamSrc[u1DecId] = eStreamFrom;

    if (u1DecId<= AUD_DEC_AUX) 
    {
        // Adjust agc
        if ((_aafgInputSrcAgcFlag[u1DecId][eStreamFrom] && _afgUserAgcFlag[u1DecId]))
        {
            _AudDspAgcEnalbe(u1DecId, TRUE);
        }
        else
        {
            _AudDspAgcEnalbe(u1DecId, FALSE);   
        }
    }    
    
#ifdef CC_MT5391_AUD_3_DECODER
    if (u1DecId != AUD_DEC_THIRD)
#endif
    {
        _AUD_DspVolumeChange(u1DecId, AUD_CH_ALL);
        _AUD_DspVolumeChange(u1DecId, AUD_CH_INPUT_SRC);
#ifdef CC_AUD_INPUT_SRC_LINEOUT_VOL
        _AUD_DspVolumeChange(u1DecId, AUD_CH_BYPASS_LEFT);
        _AUD_DspVolumeChange(u1DecId, AUD_CH_BYPASS_RIGHT);
#endif

#ifdef CC_AUD_USE_FLASH_AQ
        _AUD_FlashAQOperation(u1DecId,AQ_TYPE_CH_VOLUME, AQ_OP_EXE, (UINT16)eStreamFrom);
#endif //CC_AUD_USE_FLASH_AQ

    }
    VOL_CTL_SEMA_UNLOCK(u1DecId); 
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** AUD_DspFineTuneVolume
 *  Fine tune master volume/trim, and set it to DSP
 *  @param  u1DecId          Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  eChannel  : 0: front left channel. 
 *                               1: front right channel.  
 *                               2: rear left channel. 
 *                               3: rear right channel. 
 *                               4: center channel.
 *                               5: sub woofer channel. 
 *                               6: bypass left channel. 
 *                               7: bypass right channel. 
 *                               8: downmix left channel. 
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ). 
 *  @param  u4ShmValue       Volume (0x20000 is equal to 0 dBFs, max value: 0x7fffff)
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
void _AUD_DspFineTuneVolume(UINT8 u1DecId, AUD_CH_T eChannel, UINT32 u4ShmValue)
{
    UINT16 u2ShmIndex = 0;
    UINT16 u2UopIndex = 0;
    UINT32 u4VolumeShm = 0; /* 0 ~ 0x7fffff */

     UINT64 u8Vol;

    // Share memory = volume * adjust_ratio   
    u8Vol = ((UINT64)u4ShmValue *(UINT64)_au4AdjVol[u1DecId])/(UINT64)VOL_SHM_0_DB;
    u4VolumeShm = (UINT32)u8Vol;
    
    
    if (u1DecId == AUD_DEC_MAIN)
    {
        switch (eChannel)
        {
            case AUD_CH_FRONT_LEFT:
                u2ShmIndex = D_TRIM_L;
                u2UopIndex = UOP_DSP_TRIM_L;
                break;
            case AUD_CH_FRONT_RIGHT:
                u2ShmIndex = D_TRIM_R;
                u2UopIndex = UOP_DSP_TRIM_R;
                break;
            case AUD_CH_REAR_LEFT:
                u2ShmIndex = D_TRIM_LS;
                u2UopIndex = UOP_DSP_TRIM_LS;
                break;
            case AUD_CH_REAR_RIGHT:
                u2ShmIndex = D_TRIM_RS;
                u2UopIndex = UOP_DSP_TRIM_RS;
                break;
            case AUD_CH_CENTER:
                u2ShmIndex = D_TRIM_C;
                u2UopIndex = UOP_DSP_TRIM_C;
                break;  
            case AUD_CH_SUB_WOOFER:
                u2ShmIndex = D_TRIM_SUB;
                u2UopIndex = UOP_DSP_TRIM_SUBWOOFER;
                break;
            case AUD_CH_DMX_LEFT:
                u2ShmIndex = D_TRIM_CH9;
                u2UopIndex = UOP_DSP_TRIM_CH9;
                break;
            case AUD_CH_DMX_RIGHT:
                u2ShmIndex = D_TRIM_CH10;
                u2UopIndex = UOP_DSP_TRIM_CH10;
                break;
            case AUD_CH_ALL: /* MASTER_VOLUME */
                u2ShmIndex = D_VOL;
                u2UopIndex = UOP_DSP_MASTER_VOLUME;
                break;              
            default:
                break;
        }
    }
    else
    {
        u2ShmIndex = D_VOL_DEC2;
        u2UopIndex = UOP_DSP_MASTER_VOLUME_DEC2;

        switch(eChannel)
        {
            case AUD_CH_FRONT_LEFT:
                u2ShmIndex = D_TRIM_L_DEC2;
                u2UopIndex = UOP_DSP_TRIM_L_DEC2;
                break;
            case AUD_CH_FRONT_RIGHT:
                u2ShmIndex = D_TRIM_R;
                u2UopIndex = UOP_DSP_TRIM_R_DEC2;
                break;
            case AUD_CH_ALL: /* MASTER_VOLUME */
                u2ShmIndex = D_VOL_DEC2;
                u2UopIndex = UOP_DSP_MASTER_VOLUME_DEC2;
                break;
                
            default:
                return;
        }
    }
  
    vWriteShmUINT32(u2ShmIndex, u4VolumeShm);
    DSP_SendDspTaskCmd(u2UopIndex);
}


//-----------------------------------------------------------------------------
/** AUD_DspVolumeInit
 *  Initialize all variables for volume control
 *  @param  u1DecId          Audio decoder id (0: first decoder, 1: second decoder). 
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
void _AUD_DspVolumeInit(UINT8 u1DecId)
{
    static BOOL fgInit = FALSE;

#ifndef CC_MT5391_AUD_3_DECODER 
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) ||  (u1DecId == AUD_DEC_THIRD));
#endif

    // Init volume value
    if (!fgInit)
    {
        UINT32 u4Ptr;
        UINT8  u1DecId;
        UINT8  u1ChlNum;

        // Initialize customization table
#ifdef CC_AUD_USE_NVM
        AUD_NVM_Operation(AUD_NVM_VOL_INIT, NULL, 0);
#else        
        VERIFY(DRVCUST_OptQuery(eAudioInputVolTable, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)_aai2InputSrcVol, (const VOID *)u4Ptr, sizeof(_aai2InputSrcVol));
#endif //CC_AUD_USE_NVM        

#ifdef CC_AUD_INPUT_SRC_LINEOUT_VOL
        VERIFY(DRVCUST_OptQuery(eAudioInputLineOutVolTable, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)_aai2InputSrcLineOutVol, (const VOID *)u4Ptr, sizeof(_aai2InputSrcLineOutVol));
#endif

#ifdef CC_AUD_USE_FLASH_AQ
        _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_SP_VOL, AQ_OP_INIT, 0);
        _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_LO_VOL, AQ_OP_INIT, 0);
#endif // CC_AUD_USE_FLASH_AQ

        VERIFY(DRVCUST_OptQuery(eAudioAvInputSrcVolTable, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)_ai2AvInputSrcVol, (const VOID *)u4Ptr, sizeof(_ai2AvInputSrcVol));

        VERIFY(DRVCUST_OptQuery(eAudioInputSrcAgcFlag, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)_aafgInputSrcAgcFlag, (const VOID *)u4Ptr, sizeof(_aafgInputSrcAgcFlag));

        VERIFY(DRVCUST_OptQuery(eAudioInputSrcAgcVol, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)_aau4InputSrcAgcVol, (const VOID *)u4Ptr, sizeof(_aau4InputSrcAgcVol));

#ifndef CC_AUD_USE_FLASH_AQ
        VERIFY(DRVCUST_OptQuery(eAudioVolTable, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)_au4VOLUME, (const VOID *)u4Ptr, sizeof(_au4VOLUME));

    #ifdef CC_AUD_NEW_CV_TABLE //xiaoyi
		VERIFY(DRVCUST_OptQuery(eAudioChannelVolTable, (UINT32 *)&u4Ptr) == 0);
		x_memcpy((VOID *)_au4CHVOLUME, (const VOID *)u4Ptr, sizeof(_au4CHVOLUME));
    #endif

#else //CC_AUD_USE_FLASH_AQ
        if(! _fgAQ_Enable[AQ_TYPE_VOL_TABLE] )
        {
            VERIFY(DRVCUST_OptQuery(eAudioVolTable, (UINT32 *)&u4Ptr) == 0);
            x_memcpy((VOID *)_au4VOLUME, (const VOID *)u4Ptr, sizeof(_au4VOLUME));
        }
        if(! _fgAQ_Enable[AQ_TYPE_LO_VOL_TABLE])
        {
    #ifdef CC_AUD_NEW_CV_TABLE
            VERIFY(DRVCUST_OptQuery(eAudioChannelVolTable, (UINT32 *)&u4Ptr) == 0);
            x_memcpy((VOID *)_au4CHVOLUME, (const VOID *)u4Ptr, sizeof(_au4CHVOLUME));
    #endif
        }
#endif  // CC_AUD_USE_FLASH_AQ
        VERIFY(DRVCUST_OptQuery(eAudioChlVolExtraGain, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)_aau1ChlVolExtraGain, (const VOID *)u4Ptr, sizeof(_aau1ChlVolExtraGain));

        _u1VolBaseLevel = DRVCUST_OptGet(eAudioVolBaseLevel);

#ifdef CC_AUD_LINEOUT_VOL_MODE

        _aau1LineOutFixVol = DRVCUST_OptGet(eAudioLineOutFixVol);
#endif

#ifdef CC_AUD_USE_FLASH_AQ
        if(! _fgAQ_Enable[AQ_TYPE_AVC] )
        {
            VERIFY(DRVCUST_OptQuery(eAudioAvcTarget, (UINT32 *)&u4Ptr) == 0);
            x_memcpy((VOID *)_ai2AvcTargetLevel, (const VOID *)u4Ptr, sizeof(_ai2AvcTargetLevel));

            VERIFY(DRVCUST_OptQuery(eAudioAvcGainUp, (UINT32 *)&u4Ptr) == 0);
            x_memcpy((VOID *)_ai2AvcGainUp, (const VOID *)u4Ptr, sizeof(_ai2AvcGainUp));        

            VERIFY(DRVCUST_OptQuery(eAudioAvcAdjustRate, (UINT32 *)&u4Ptr) == 0);
            x_memcpy((VOID *)_ai2AvcAdjRate, (const VOID *)u4Ptr, sizeof(_ai2AvcAdjRate));        
        }
#else        
        VERIFY(DRVCUST_OptQuery(eAudioAvcTarget, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)_ai2AvcTargetLevel, (const VOID *)u4Ptr, sizeof(_ai2AvcTargetLevel));

        VERIFY(DRVCUST_OptQuery(eAudioAvcGainUp, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)_ai2AvcGainUp, (const VOID *)u4Ptr, sizeof(_ai2AvcGainUp));        

        VERIFY(DRVCUST_OptQuery(eAudioAvcAdjustRate, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)_ai2AvcAdjRate, (const VOID *)u4Ptr, sizeof(_ai2AvcAdjRate));
#endif //CC_AUD_USE_FLASH_AQ

#ifdef CC_AUD_VOLUME_OFFSET
        VERIFY(DRVCUST_OptQuery(eAudioVolumeOffset, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)_aai4VolOffsetMap, (const VOID *)u4Ptr, sizeof(_aai4VolOffsetMap));
#endif
#ifdef CC_AUD_TV_SYS_FINE_TUNE_VOL
        VERIFY(DRVCUST_OptQuery(eAudioTvSysFineTuneVol, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)_aau1TVFineTuneVolume, (const VOID *)u4Ptr, sizeof(_aau1TVFineTuneVolume));		
#endif
        // Initialize local variables ..
        for (u1DecId = 0; u1DecId < (UINT8)AUD_DEC_NUM; u1DecId ++)
        {
            for (u1ChlNum = 0; u1ChlNum < (UINT8)AUD_CH_NUM; u1ChlNum++)
            {
                _aafgChlMute[u1DecId][u1ChlNum] = FALSE;
                _aafgChlOutPort[u1DecId][u1ChlNum] = TRUE;
                _aau1ChannelVolume[u1DecId][u1ChlNum] = _u1VolBaseLevel;  
#ifdef CC_AUD_LINEOUT_VOL_OFFSET
                _aau1ChannelVolOffset[u1DecId][u1ChlNum] = _u1VolBaseLevel;  
#endif
                _aafgChlHdmiChgFmtMute[u1DecId][u1ChlNum] = FALSE;
                _aau4ChannelVolumeRawData[u1DecId][u1ChlNum] = VOL_SHM_0_DB;
#ifdef CC_ENABLE_AV_SYNC		        
                _aafgChDecPlayMute[u1DecId][u1ChlNum] = FALSE;
#endif          
            }
        }
            
        fgInit = TRUE;
    }

    VERIFY(x_sema_create(&_ahVolCtlSema[u1DecId], X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
     
}


//-----------------------------------------------------------------------------
/** AUD_DspEqCfgInit
 *  Initialize variables for equalizer preset configuration
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
void _AUD_DspEqCfgInit(void)
{
    static BOOL fgInit = FALSE;
 
    // Init volume value
    if (!fgInit)
    {
        UINT32 u4Ptr;
        VERIFY(DRVCUST_OptQuery(eAudioAvChlEqTable, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)_aai1ChEqCfg, (const VOID *)u4Ptr, sizeof(_aai1ChEqCfg));

        VERIFY(x_sema_create(&_ahEqCtlSema, X_SEMA_TYPE_BINARY,
                             X_SEMA_STATE_UNLOCK) == OSR_OK);
     
        fgInit = TRUE;
    }

}
#ifdef CC_AUD_SOUND_MODE
//-----------------------------------------------------------------------------
/** AUD_DspPeqCfgInit
 *  Initialize variables for PEQ configuration
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
void _AUD_DspPSMCfgInit(void)
{
    static BOOL fgInit = FALSE;
 
    // Init volume value
    if (!fgInit)
    {
        UINT32 u4Ptr;
        VERIFY(DRVCUST_OptQuery(eAudioPeqFrCfg, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)_aai1PeqFq, (const VOID *)u4Ptr, sizeof(_aai1PeqFq));

        VERIFY(DRVCUST_OptQuery(eAudioPeqBwCfg, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)_aai1PeqBw, (const VOID *)u4Ptr, sizeof(_aai1PeqBw));

        VERIFY(DRVCUST_OptQuery(eAudioPeqGainCfg, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)_aai1PeqGain, (const VOID *)u4Ptr, sizeof(_aai1PeqGain));

        VERIFY(DRVCUST_OptQuery(eAudioBassTreble, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)_aai1BassTreble, (const VOID *)u4Ptr, sizeof(_aai1BassTreble));
        VERIFY(DRVCUST_OptQuery(eAudioPSMCfg, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)_arSoundModeCase, (const VOID *)u4Ptr, sizeof(_arSoundModeCase));

#ifdef CC_AUD_USE_FLASH_AQ
        UINT8 i,j,k;
        UINT16 u2Temp;
       if(_fgAQ_Enable[AQ_TYPE_PEQ])
       {
         VERIFY(FLASH_AQ_SRC_NUM*FLASH_AQ_SM_NUM < AUD_SM_NUM);
         for(i=0; i< FLASH_AQ_SRC_NUM ;i++)
         {
            for(k = 0; k<FLASH_AQ_SM_NUM; k++)
            {
                for(j=0; j< AQ_PEQ_ITEMS;j+=5)
                {
                    u2Temp =_au1AQ_PEQ[i][k][j]<<8;
                    u2Temp += _au1AQ_PEQ[i][k][j+1];
                    _aai1PeqFq[i*k][j/5] = u2Temp;
                    
                    u2Temp = _au1AQ_PEQ[i][k][j+2]<<8;
                    u2Temp += _au1AQ_PEQ[i][k][j+3];
                    _aai1PeqBw[i*k][j/5] = u2Temp;

                    _aai1PeqGain[i*k][j/5] = (INT8)_au1AQ_PEQ[i][k][j+4];
                }
            }
         }
       }
       if(_fgAQ_Enable[AQ_TYPE_BASS_TREBLE])
       {
           VERIFY(sizeof(_aai1BassTreble) >= sizeof(_au1AQBassTrebleOffset));
           x_memcpy((VOID *)_aai1BassTreble, (const VOID *)_au1AQBassTrebleOffset, sizeof(_au1AQBassTrebleOffset));
       }
       
#endif //CC_AUD_USE_FLASH_AQ
		VERIFY(x_sema_create(&_ahPSMCtlSema, X_SEMA_TYPE_BINARY,
                             X_SEMA_STATE_UNLOCK) == OSR_OK);
    
        fgInit = TRUE;
    }

}
void _AUD_DspVSMCfgInit(void)
{
    static BOOL fgInit = FALSE;
 
    // Init volume value
    if (!fgInit)
    {
        UINT32 u4Ptr;

        VERIFY(DRVCUST_OptQuery(eAudioVSModeCfg, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)_aai1VSMode, (const VOID *)u4Ptr, sizeof(_aai1VSMode));
#ifdef CC_AUD_USE_FLASH_AQ
    if(_fgAQ_Enable[AQ_TYPE_MVS])
    {
        VERIFY(sizeof(_aai1VSMode) >= sizeof(_au4AQ_MVS));
        x_memcpy((VOID *)_aai1VSMode, (const VOID *)_au4AQ_MVS, sizeof(_au4AQ_MVS));
    }
#endif

		VERIFY(x_sema_create(&_ahVSMCtlSema, X_SEMA_TYPE_BINARY,
                             X_SEMA_STATE_UNLOCK) == OSR_OK);
    
        fgInit = TRUE;
    }

}

#endif //CC_AUD_SOUND_MODE
//-----------------------------------------------------------------------------
/** _AUD_DspChDelayInit
 *  Initialize table for channel delay configuration
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
void _AUD_DspChDelayInit(void)
{
    static BOOL fgInit = FALSE;

    // Init volume value
    if (!fgInit)
    {
        UINT32 u4Ptr;
        
        VERIFY(DRVCUST_OptQuery(eAudioChannelDelay, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)_au2ChDelay, (const VOID *)u4Ptr, sizeof(_au2ChDelay));

        VERIFY(x_sema_create(&_ahChDelayCtlSema, X_SEMA_TYPE_BINARY,
                             X_SEMA_STATE_UNLOCK) == OSR_OK);
     
        fgInit = TRUE;
    }    
}


//-----------------------------------------------------------------------------
/** _AUD_DspSpdifInit
 *  Initialize SPDIF configuration
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSpdifInit(void)
{
    static BOOL fgInit = FALSE;

    if (!fgInit)
    {
        UINT32 u4Ptr;
        
        VERIFY(DRVCUST_OptQuery(eAudioIecCopyright, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)_au1IecCopyright, (const VOID *)u4Ptr, sizeof(_au1IecCopyright));

        VERIFY(DRVCUST_OptQuery(eAudioIecCategory, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)_au1IecCategory, (const VOID *)u4Ptr, sizeof(_au1IecCategory));

        VERIFY(DRVCUST_OptQuery(eAudioIecWordLength, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)_au1IecWordLength, (const VOID *)u4Ptr, sizeof(_au1IecWordLength));        

        VERIFY(x_sema_create(&_ahSpdifCtlSema, X_SEMA_TYPE_BINARY,
                             X_SEMA_STATE_UNLOCK) == OSR_OK);     
        fgInit = TRUE;
    }    
}


// *********************************************************************
// Function : void AUD_DspReverbEnable(UINT8 u1DecId, BOOL fgEnable)
// Description : turn on / off reverb control
// Parameter :  TRUE / FALSE
// Return    :  None.
// *********************************************************************
void _AUD_DspReverbEnable(UINT8 u1DecId, BOOL fgEnable)
{
// Remove for new post-processing
    UINT16 u2ReverbFlagIdx;
    UINT32 u4UopCmd;

    if (u1DecId == AUD_DEC_MAIN)
    {
        u2ReverbFlagIdx = B_REVERBFLAG;
        u4UopCmd = UOP_DSP_REVERB_FLAG;
    }
    else
    {
        u2ReverbFlagIdx = B_REVERBFLAG_DEC2;
        u4UopCmd = UOP_DSP_REVERB_FLAG_DEC2;
    }

    vWriteShmUINT8(u2ReverbFlagIdx, (UINT8)fgEnable);
    DSP_SendDspTaskCmd(u4UopCmd);  
}

//-----------------------------------------------------------------------------
/** AUD_DspReverbMode
 *  load the predefined reverberation parameters sets.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  bMode From mode 0 to mode 7. Mode 0 will turn the reverberation effect off.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspReverbMode(UINT8 u1DecId, UINT8 u1Mode) 
{
    if (u1Mode == AUD_REVERB_MODE_OFF)
    {
        AUD_DspReverbEnable(u1DecId, FALSE);
    }
    else if (u1Mode <= AUD_REVERB_MODE_MAX)
    {
        u1Mode = u1Mode - 1;
    
        if (u1DecId == AUD_DEC_MAIN)
        {
            vWriteShmUINT32(D_REVERBGAIN,  _arReverMode[u1Mode].u4Gain);
            vWriteShmUINT8(B_REVERBBANK0, _arReverMode[u1Mode].u1ReverbBank0);
            vWriteShmUINT8(B_REVERBBANK1, _arReverMode[u1Mode].u1ReverbBank1);
            vWriteShmUINT8(B_REVERBBANK2, _arReverMode[u1Mode].u1ReverbBank2);
            vWriteShmUINT8(B_REVERBBANK3, _arReverMode[u1Mode].u1ReverbBank3);
        }
        else
        {
            vWriteShmUINT32(D_REVERBGAIN_DEC2,  _arReverMode[u1Mode].u4Gain);
            vWriteShmUINT8(B_REVERBBANK0_DEC2, _arReverMode[u1Mode].u1ReverbBank0);
            vWriteShmUINT8(B_REVERBBANK1_DEC2, _arReverMode[u1Mode].u1ReverbBank1);
            vWriteShmUINT8(B_REVERBBANK2_DEC2, _arReverMode[u1Mode].u1ReverbBank2);
            vWriteShmUINT8(B_REVERBBANK3_DEC2, _arReverMode[u1Mode].u1ReverbBank3);
        }
  
        AUD_DspReverbEnable(u1DecId, TRUE);
    }
}

// *********************************************************************
// Function :  void AUD_DspBassBoostEnable(UINT8 u1DecId, BOOL fgEnable) 
// Description : turn on /off the bass boost control
// Parameter : bMode - ADSP_ON / ADSP_OFF
// Return    : None.
// *********************************************************************
void _AUD_DspChannelEqEnable(UINT8 u1DecId, BOOL fgEnable) 
{
    UINT16 u2EqFlagIdx;
    UINT32 u4UopCmd;
    EQ_FLAG_UNION_T rEqFlag;
    static BOOL fgInit = FALSE;
  
    if (!fgInit)
    {
        AUD_DspChEqLoadPreset(AUD_DEC_MAIN, AUD_EQ_ROCK);
        AUD_DspChEqLoadPreset(AUD_DEC_AUX, AUD_EQ_ROCK);
        fgInit = TRUE;
    }    

    if (u1DecId == AUD_DEC_MAIN)
    {
        u2EqFlagIdx = B_EQFLAG;
        u4UopCmd = UOP_DSP_EQUALIZER_FLAG;
    }
    else
    {
        u2EqFlagIdx = B_EQFLAG_DEC2;
        u4UopCmd = UOP_DSP_EQUALIZER_FLAG_DEC2;
    }
  
    rEqFlag.u1Eq = uReadShmUINT8(u2EqFlagIdx);
    rEqFlag.rField.fgChEqOn = fgEnable;
    
    vWriteShmUINT8(u2EqFlagIdx, rEqFlag.u1Eq);
    DSP_SendDspTaskCmd(u4UopCmd);
}

// *********************************************************************
// Function :  void AUD_DspSpkSizeCfg(UINT8 u1DecId, UINT32 u4SpkSize) 
// Description : Set the bass management cut-off frequency
// Parameter : u2SpkSize in xx Hz
// Return    : None.
// *********************************************************************
void _AUD_DspSpkSizeCfg(UINT8 u1DecId, UINT32 u4SpkSize)
{
    UINT16 u2ShmIdx;
    UINT32 u4UopCmd;
    UINT32 u4CutFreq;
  
    if (u1DecId == AUD_DEC_MAIN)
    {
        u2ShmIdx = D_CUTOFF_FREQ;
        u4UopCmd = UOP_DSP_CHANGE_SPK_SIZE;
    }
    else
    {
        u2ShmIdx = D_CUTOFF_FREQ_DEC2;
        u4UopCmd = UOP_DSP_CHANGE_SPK_SIZE;   // since only do aout re-init, therefore re-use uop of dec1.
    }
  
    // Check if duplicated setting 
    u4CutFreq = u4ReadShmUINT32(u2ShmIdx);
    if (u4CutFreq == u4SpkSize)
    {
        return;
    }
  
    vWriteShmUINT32(u2ShmIdx, u4SpkSize);
    DSP_SendDspTaskCmd(u4UopCmd);
}


// *********************************************************************
// Function :  UINT32 _AUD_DspSpkSizeQry(UINT8 u1DecId) 
// Description : Query the bass management cut-off frequency
// Parameter : Decoder ID
// Return    : None.
// *********************************************************************
UINT32 _AUD_DspSpkSizeQry(UINT8 u1DecId) //Andrew Wen 07/8/20 
{
    UINT32 iCutFreq;

    iCutFreq = u4ReadShmUINT32 (D_CUTOFF_FREQ);
    return iCutFreq;
}

//-----------------------------------------------------------------------------
/** AUD_DspBassBoostEnable
 *  turn on /off the bass boost control.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  fgEnable ADSP_ON / ADSP_OFF.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspBassBoostEnable(UINT8 u1DecId, BOOL fgEnable) 
{
    UINT16 u2EqFlagIdx;
    UINT32 u4UopCmd;
    //EQ_FLAG_UNION_T rEqFlag;
    UINT8 u1ShmFlag;
  
    if (u1DecId == AUD_DEC_MAIN)
    {
        u2EqFlagIdx = B_EQFLAG;
        u4UopCmd = UOP_DSP_EQUALIZER_FLAG;
    }
    else
    {
        u2EqFlagIdx = B_EQFLAG_DEC2;
        u4UopCmd = UOP_DSP_EQUALIZER_FLAG_DEC2;
    }
 
/*  
    rEqFlag.u1Eq = uReadShmUINT8(u2EqFlagIdx);
    rEqFlag.rField.fgBassBoostOn = fgEnable;
  
    vWriteShmUINT8(u2EqFlagIdx, rEqFlag.u1Eq);
    DSP_SendDspTaskCmd(u4UopCmd);
*/  

    u1ShmFlag = uReadShmUINT8(u2EqFlagIdx);
  
    if ((fgEnable && !(u1ShmFlag& EQ_BASS_BOOST)) ||
        (!fgEnable && ((u1ShmFlag & EQ_BASS_BOOST) == EQ_BASS_BOOST)))
    {
  
        u1ShmFlag = fgEnable ? (u1ShmFlag | EQ_BASS_BOOST) : (u1ShmFlag & ~ EQ_BASS_BOOST);
        vWriteShmUINT8(u2EqFlagIdx, u1ShmFlag);
        DSP_SendDspTaskCmd(u4UopCmd);  
    }     
  
}


//-----------------------------------------------------------------------------
/** AUD_DspBassBoostGain
 *   set the bass boost gain value.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  u1Level the setting value is from 0 to 28, level 14 means 0db. A large 
 *                         number will boost the bass and a smaller number will attenuate the bass.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspBassBoostGain(UINT8 u1DecId, UINT8 u1Level)
{
#ifndef CC_AUD_EQ_BASED_SBASS
    // Use original sbass
    UINT16 u2BassBoostGainIdx;
    UINT32 u4UopCmd;
    static BOOL fgEnable = FALSE;
  
    if (!fgEnable)
    {
        AUD_DspBassBoostEnable(u1DecId, TRUE);
        fgEnable = TRUE;
    }    
   
    if (u1DecId == AUD_DEC_MAIN)
    {
        u2BassBoostGainIdx = D_SBASSBOOSTGAIN;
        u4UopCmd = UOP_DSP_SUPER_BASS_BOOST_GAIN;
    }
    else
    {
        u2BassBoostGainIdx = D_SBASSBOOSTGAIN_DEC2;
        u4UopCmd = UOP_DSP_SUPER_BASS_BOOST_GAIN_DEC2;
    }
  
    if (u1Level >= BASS_TREBLE_GAIN_MAX)
    {
        u1Level = BASS_TREBLE_GAIN_MAX - 1;
    }

#ifdef DSP_EQ2_SBASS_SUPPORT
    vWriteShmUINT32(u2BassBoostGainIdx, u1Level);
    DSP_SendDspTaskCmd(u4UopCmd);
    AUD_DspBassBoostEnable(u1DecId, TRUE);
    x_thread_delay(10);
#else
    vWriteShmUINT32(u2BassBoostGainIdx, _au4GainBand[u1Level]);
    DSP_SendDspTaskCmd(u4UopCmd);    
#endif
    
    #ifdef CC_AUD_BASS_ADJ_LFE    //andrew wen 07/1/25
    u1Level = u1Level<<2;
    if (u1Level > 100)
        u1Level = 100;
    _VolumeCtrl(u1DecId, AUD_CH_LFE, _au4VOLUME[u1Level]);
    #endif 
#else
    // Use EQ-based sbass
    INT8 i1SbassDb, i1SbassDbStart, i1SbassStep, i1TotalDb;
    INT8 i1LoopCnt, i1LoopIdx;
    UINT8     u1Idx, u1EqType;
    static INT8 _i1SbassDbOld=EQ_BAND_GAIN_MID;
      
    u1EqType = _aeEqMode[u1DecId];
    i1SbassDb = (((INT8)u1Level * 2) * EQ_BAND_GAIN_DB_MAX) / BASS_TREBLE_GAIN_DB_MAX;

    if (i1SbassDb == _i1SbassDbOld)
    {
        return;
    }
    else if (i1SbassDb == EQ_BAND_GAIN_MID)
    {   /* add for "reset default" option in menu */
        i1SbassDbStart = EQ_BAND_GAIN_MID;
        i1SbassStep = 0;
        i1LoopCnt = 1;
    }
    else if (i1SbassDb > _i1SbassDbOld)
    {
        i1SbassDbStart = _i1SbassDbOld + 1;
        i1SbassStep = 1;
        i1LoopCnt = i1SbassDb - _i1SbassDbOld;        
    }
    else
    {
        i1SbassDbStart = _i1SbassDbOld - 1;
        i1SbassStep = -1;
        i1LoopCnt = _i1SbassDbOld - i1SbassDb;            
    }

    for (i1LoopIdx=0; i1LoopIdx<i1LoopCnt; i1LoopIdx++)
    {
        _aai1ChEqCfg[AUD_EQ_SBASS][AUD_EQ_BAND_1] = i1SbassDbStart - EQ_BAND_GAIN_MID;
        i1TotalDb = _aai1ChEqCfg[u1EqType][AUD_EQ_BAND_1] + _aai1ChEqCfg[AUD_EQ_SBASS][AUD_EQ_BAND_1];
        
        if (i1TotalDb > EQ_BAND_GAIN_SIDE_INDEX_MAX)
        {
            i1TotalDb = EQ_BAND_GAIN_SIDE_INDEX_MAX;
        }

        if (i1TotalDb < -EQ_BAND_GAIN_SIDE_INDEX_MAX)
        {
            i1TotalDb = -EQ_BAND_GAIN_SIDE_INDEX_MAX;
        } 
        
        _ai1ChEqFinalCfg[AUD_EQ_BAND_1] = i1TotalDb;
     
        VERIFY(x_sema_lock(_ahEqCtlSema, X_SEMA_OPTION_WAIT) == OSR_OK);
        AUD_DspChEqBand(u1DecId, (UINT8)EQ_BAND_DRY, (UINT8)(_ai1ChEqFinalCfg[EQ_BAND_DRY]+EQ_DRY_GAIN_MID), FALSE);
        for (u1Idx = 1; u1Idx < (CH_EQ_BAND_NO + 1); u1Idx++)
        {
            AUD_DspChEqBand(u1DecId, u1Idx, (UINT8)(_ai1ChEqFinalCfg[u1Idx]+EQ_BAND_GAIN_MID), (u1Idx == CH_EQ_BAND_NO));
        }
        VERIFY(x_sema_unlock(_ahEqCtlSema) == OSR_OK);
          
        i1SbassDbStart += i1SbassStep;
    }
    
    AUD_DspEqEnable(u1DecId, TRUE);    
    _i1SbassDbOld = i1SbassDb;
#endif
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetBassBoostGain
 *   get the bass boost gain value.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder). 
 *
 *  @retval bass gain value
 */
//-----------------------------------------------------------------------------
UINT32 _AUD_DspGetBassBoostGain(UINT8 u1DecId)
{
    UINT32 u4Level;

    u4Level = u4ReadShmUINT32(D_SBASSBOOSTGAIN);
    return u4Level;
}

//-----------------------------------------------------------------------------
/** AUD_DspTrebleBoostCtrl
 *   turn on / off treble boost control.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  fgEnable  ADSP_ON / ADSP_OFF.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSuperBassEnable(UINT8 u1DecId, BOOL fgEnable)
{
    UINT16 u2EqFlagIdx;
    UINT32 u4UopCmd;
    UINT8  u1ShmFlag;
    //EQ_FLAG_UNION_T rEqFlag;
  
    if (u1DecId == AUD_DEC_MAIN)
    {
        u2EqFlagIdx = B_EQFLAG;
        u4UopCmd = UOP_DSP_EQUALIZER_FLAG;
    }
    else
    {
        u2EqFlagIdx = B_EQFLAG_DEC2;
        u4UopCmd = UOP_DSP_EQUALIZER_FLAG_DEC2;
    }
  
    //vWriteShmUINT8(u2EqFlagIdx, rEqFlag.u1Eq);
    //DSP_SendDspTaskCmd(u4UopCmd);
    u1ShmFlag = uReadShmUINT8(u2EqFlagIdx);
    if ((fgEnable && !(u1ShmFlag& EQ_SUPERBASS_BOOST)) ||
        (!fgEnable && ((u1ShmFlag & EQ_SUPERBASS_BOOST) == EQ_SUPERBASS_BOOST)))
    {

        u1ShmFlag = fgEnable ? (u1ShmFlag | EQ_SUPERBASS_BOOST) : (u1ShmFlag & ~ EQ_SUPERBASS_BOOST);
        vWriteShmUINT8(u2EqFlagIdx, u1ShmFlag);
        DSP_SendDspTaskCmd(u4UopCmd);  
    }     
  
}

// *********************************************************************
// Function : void AUD_DspTrebleBoostCtrl(BYTE bMode)
// Description : turn on / off treble boost control
// Parameter : ADSP_ON / ADSP_OFF
// Return    : None.
// *********************************************************************
void _AUD_DspClearBoostEnable(UINT8 u1DecId, BOOL fgEnable)
{
    UINT16 u2EqFlagIdx;
    UINT32 u4UopCmd;
    //EQ_FLAG_UNION_T rEqFlag;
    UINT8   u1ShmFlag;
    
    if (u1DecId == AUD_DEC_MAIN)
    {
        u2EqFlagIdx = B_EQFLAG;
        u4UopCmd = UOP_DSP_EQUALIZER_FLAG;
    }
    else
    {
        u2EqFlagIdx = B_EQFLAG_DEC2;
        u4UopCmd = UOP_DSP_EQUALIZER_FLAG_DEC2;
    }

/*
    rEqFlag.u1Eq = uReadShmUINT8(u2EqFlagIdx);
    rEqFlag.rField.fgClearBoostOn = fgEnable;
  
    vWriteShmUINT8(u2EqFlagIdx, rEqFlag.u1Eq);
    DSP_SendDspTaskCmd(u4UopCmd);
*/

    u1ShmFlag = uReadShmUINT8(u2EqFlagIdx);
  
    if ((fgEnable && !(u1ShmFlag& EQ_TREBLE_BOOST)) ||
        (!fgEnable && ((u1ShmFlag & EQ_TREBLE_BOOST) == EQ_TREBLE_BOOST)))
    {
  
        u1ShmFlag = fgEnable ? (u1ShmFlag | EQ_TREBLE_BOOST) : (u1ShmFlag & ~ EQ_TREBLE_BOOST);
        vWriteShmUINT8(u2EqFlagIdx, u1ShmFlag);
        DSP_SendDspTaskCmd(u4UopCmd);  
    }     

  
}

//-----------------------------------------------------------------------------
/** AUD_DspClearBoostGain
 *   set the clear boost gain value.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  u1Level  range from 0 to 28, level 14 means 0db.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspClearBoostGain(UINT8 u1DecId, UINT8 u1Level)
{
#ifndef CC_AUD_EQ_BASED_SBASS
    // Use original sbass
    UINT16 u2ClearBoostGainIdx;
    UINT32 u4UopCmd;
    static BOOL fgEnable = FALSE;
  
    if (!fgEnable)
    {
        AUD_DspClearBoostEnable(u1DecId, TRUE);
        fgEnable = TRUE;
    }    
   
    if (u1DecId == AUD_DEC_MAIN)
    {
        u2ClearBoostGainIdx = D_SBASSCLEARGAIN;
        u4UopCmd = UOP_DSP_SUPER_BASS_CLEAR_GAIN;
    }
    else
    {
        u2ClearBoostGainIdx = D_SBASSCLEARGAIN_DEC2;
        u4UopCmd = UOP_DSP_SUPER_BASS_CLEAR_GAIN_DEC2;
    }
  
    if (u1Level >= BASS_TREBLE_GAIN_MAX)
    {
        u1Level = BASS_TREBLE_GAIN_MAX - 1;
    }

#ifdef DSP_EQ2_SBASS_SUPPORT
    vWriteShmUINT32(u2ClearBoostGainIdx, u1Level);
    DSP_SendDspTaskCmd(u4UopCmd); 
    AUD_DspClearBoostEnable(u1DecId, TRUE);
    x_thread_delay(10);
#else
    vWriteShmUINT32(u2ClearBoostGainIdx, _au4GainBand[u1Level]);
    DSP_SendDspTaskCmd(u4UopCmd);
#endif    

#else
    // Use EQ-based sbass
    INT8 i1SbassDb, i1SbassDbStart, i1SbassStep, i1TotalDb;
    INT8 i1LoopCnt, i1LoopIdx;
    UINT8     u1Idx, u1EqType;
    static INT8 _i1SbassDbOld=EQ_BAND_GAIN_MID;
      
    u1EqType = _aeEqMode[u1DecId];
    i1SbassDb = (((INT8)u1Level * 2) * EQ_BAND_GAIN_DB_MAX) / BASS_TREBLE_GAIN_DB_MAX;

    if (i1SbassDb == _i1SbassDbOld)
    {
        return;
    }
    else if (i1SbassDb == EQ_BAND_GAIN_MID)
    {   /* add for "reset default" option in menu */
        i1SbassDbStart = EQ_BAND_GAIN_MID;
        i1SbassStep = 0;
        i1LoopCnt = 1;
    }
    else if (i1SbassDb > _i1SbassDbOld)
    {
        i1SbassDbStart = _i1SbassDbOld + 1;
        i1SbassStep = 1;
        i1LoopCnt = i1SbassDb - _i1SbassDbOld;        
    }
    else
    {
        i1SbassDbStart = _i1SbassDbOld - 1;
        i1SbassStep = -1;
        i1LoopCnt = _i1SbassDbOld - i1SbassDb;            
    }

    for (i1LoopIdx=0; i1LoopIdx<i1LoopCnt; i1LoopIdx++)
    {
        _aai1ChEqCfg[AUD_EQ_SBASS][AUD_EQ_BAND_LAST] = i1SbassDbStart - EQ_BAND_GAIN_MID;
        i1TotalDb = _aai1ChEqCfg[u1EqType][AUD_EQ_BAND_LAST] + _aai1ChEqCfg[AUD_EQ_SBASS][AUD_EQ_BAND_LAST];
        
        if (i1TotalDb > EQ_BAND_GAIN_SIDE_INDEX_MAX)
        {
            i1TotalDb = EQ_BAND_GAIN_SIDE_INDEX_MAX;
        }

        if (i1TotalDb < -EQ_BAND_GAIN_SIDE_INDEX_MAX)
        {
            i1TotalDb = -EQ_BAND_GAIN_SIDE_INDEX_MAX;
        } 
        
        _ai1ChEqFinalCfg[AUD_EQ_BAND_LAST] = i1TotalDb;
     
        VERIFY(x_sema_lock(_ahEqCtlSema, X_SEMA_OPTION_WAIT) == OSR_OK);
        AUD_DspChEqBand(u1DecId, (UINT8)EQ_BAND_DRY, (UINT8)(_ai1ChEqFinalCfg[EQ_BAND_DRY]+EQ_DRY_GAIN_MID), FALSE);
        for (u1Idx = 1; u1Idx < (CH_EQ_BAND_NO + 1); u1Idx++)
        {
            AUD_DspChEqBand(u1DecId, u1Idx, (UINT8)(_ai1ChEqFinalCfg[u1Idx]+EQ_BAND_GAIN_MID), (u1Idx == CH_EQ_BAND_NO));
        }
        VERIFY(x_sema_unlock(_ahEqCtlSema) == OSR_OK);
          
        i1SbassDbStart += i1SbassStep;
    }
    
    AUD_DspEqEnable(u1DecId, TRUE);    
    _i1SbassDbOld = i1SbassDb;
#endif
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetClearBoostGain
 *   get the clear boost gain value.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder). 
 *
 *  @retval treble gain value
 */
//-----------------------------------------------------------------------------
UINT32 _AUD_DspGetClearBoostGain(UINT8 u1DecId)
{
    UINT32 u4Level;

    u4Level = u4ReadShmUINT32(D_SBASSCLEARGAIN);
    return u4Level;
}

// *********************************************************************
// Function : void AUD_DspEqEnable(UINT8 u1DecId, BOOL fgEnable)
// Description : turn on / off equalizer control
// Parameter :  TRUE / FALSE
// Return    :  None.
// *********************************************************************
void _AUD_DspEqEnable(UINT8 u1DecId, BOOL fgEnable)
{
    UINT16 u2EqFlagIdx;
    UINT32 u4UopCmd;
    UINT8   u1ShmFlag;
  
    if (u1DecId == AUD_DEC_MAIN)
    {
        u2EqFlagIdx = B_EQFLAG;
        u4UopCmd = UOP_DSP_EQUALIZER_FLAG;
    }
    else
    {
        u2EqFlagIdx = B_EQFLAG_DEC2;
        u4UopCmd = UOP_DSP_EQUALIZER_FLAG_DEC2;
    }
  
    u1ShmFlag = uReadShmUINT8(u2EqFlagIdx);
   
    if ((fgEnable && !(u1ShmFlag& EQ_FLAG)) ||
        (!fgEnable && ((u1ShmFlag & EQ_FLAG) == EQ_FLAG)))
    {
  
        u1ShmFlag = fgEnable ? (u1ShmFlag | EQ_FLAG) : (u1ShmFlag & ~ EQ_FLAG);
        vWriteShmUINT8(u2EqFlagIdx, u1ShmFlag);
        DSP_SendDspTaskCmd(u4UopCmd);  
    }    
}

// *********************************************************************
// Function : AUD_DspChEqBand(strucDspChBand rChBand, BYTE bLevel, BYTE fgNotify)
// Description : used to adjusting the equalizer's parameter for each channel
// Parameter : u1BandIdx -- EQ band index
//             bLevel  -- range is from 0x0 to 0x28
//             fgNotify -- to indicate changing the parameter immediately or not
// Return    : None.
// *********************************************************************
void _AUD_DspChEqBand(UINT8 u1DecId, UINT8 u1BandIdx, UINT8 u1Level, BOOL fgNotify) 
{
    UINT8 u1Idx;
    UINT32 u4Val;
    UINT16 u2ShmIndex;
    UINT32 u4UopCmd;
    
    if (u1BandIdx >= EQ_BAND_MAX)
    {
        return;
    }

    if (u1DecId == AUD_DEC_MAIN)
    {
        u2ShmIndex = D_EQ_DRY;
        u4UopCmd = UOP_DSP_EQUALIZER_CHANNEL_CHANGE + 0x01000000;
    }
    else
    {
        u2ShmIndex = D_EQ_DRY_DEC2;
        u4UopCmd = UOP_DSP_EQUALIZER_CHANNEL_CHANGE_DEC2 + 0x01000000;
    }
    
    if (u1BandIdx  == EQ_BAND_DRY)
    {
        if (u1Level >= EQ_DRY_GAIN_INDEX_MAX)
        {
            u1Level = EQ_DRY_GAIN_INDEX_MAX - 1;
        }
        
        u4Val = _au4DryGain[u1Level];
        vWriteShmUINT32(u2ShmIndex, u4Val);
    }
    else
    {
        if (u1Level >= EQ_BAND_GAIN_INDEX_MAX)
        {
            u1Level = EQ_BAND_GAIN_INDEX_MAX - 1;
        }

        u2ShmIndex += (4 + _aau1EqBandExtend[u1BandIdx-1][0]);

        for (u1Idx=0; u1Idx<_aau1EqBandExtend[u1BandIdx-1][1]; u1Idx++)
        {
            vWriteShmUINT8((u2ShmIndex+u1Idx), u1Level);
        }      
    }

    if (fgNotify)
    {
        DSP_SendDspTaskCmd(u4UopCmd);
    }
}

//-----------------------------------------------------------------------------
/** AUD_DspChEqLoadPreset
 *   load the predefined channel equalizer setting.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  eEqType  the valid type number is from 0 to type 7.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspChEqLoadPreset(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType) 
{
    UINT8     u1Idx;
#ifdef CC_AUD_EQ_BASED_SBASS
    INT8    i1TotalDb;
#endif  
      
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
    
    if (eEqType >= AUD_EQ_NUM)
    {
        eEqType = AUD_EQ_OFF;
    }

    _aeEqMode[u1DecId] = eEqType;
  
#ifdef CC_AUD_EQ_BASED_SBASS
    for (u1Idx=0; u1Idx<(CH_EQ_BAND_NO + 1); u1Idx++)
    {
        i1TotalDb = _aai1ChEqCfg[eEqType][u1Idx] + _aai1ChEqCfg[AUD_EQ_SBASS][u1Idx];
        if (u1Idx == (UINT8)EQ_BAND_DRY)
        {
            // Dry gain value
            if (i1TotalDb > EQ_DRY_GAIN_SIDE_INDEX_MAX)
            {
                 i1TotalDb = EQ_DRY_GAIN_SIDE_INDEX_MAX;
            }

            if (i1TotalDb < -EQ_DRY_GAIN_SIDE_INDEX_MAX)
            {
                 i1TotalDb = -EQ_DRY_GAIN_SIDE_INDEX_MAX;
            }                        
        }
        else
        {
            // Band gain value
            if (i1TotalDb > EQ_BAND_GAIN_SIDE_INDEX_MAX)
            {
                 i1TotalDb = EQ_BAND_GAIN_SIDE_INDEX_MAX;
            }

            if (i1TotalDb < -EQ_BAND_GAIN_SIDE_INDEX_MAX)
            {
                 i1TotalDb = -EQ_BAND_GAIN_SIDE_INDEX_MAX;
            }                
        }
        _ai1ChEqFinalCfg[u1Idx] = i1TotalDb;
    }
#endif    
  
    VERIFY(x_sema_lock(_ahEqCtlSema, X_SEMA_OPTION_WAIT) == OSR_OK);
#ifndef  CC_AUD_EQ_BASED_SBASS
    AUD_DspChEqBand(u1DecId, (UINT8)EQ_BAND_DRY, (UINT8)(_aai1ChEqCfg[eEqType][0]+EQ_DRY_GAIN_MID), FALSE);
#else
    AUD_DspChEqBand(u1DecId, (UINT8)EQ_BAND_DRY, (UINT8)(_ai1ChEqFinalCfg[0]+EQ_DRY_GAIN_MID), FALSE);
#endif

    for (u1Idx = 1; u1Idx < (CH_EQ_BAND_NO + 1); u1Idx++)
    {
#ifndef  CC_AUD_EQ_BASED_SBASS
        AUD_DspChEqBand(u1DecId, u1Idx, (UINT8)(_aai1ChEqCfg[eEqType][u1Idx]+EQ_BAND_GAIN_MID), (u1Idx == CH_EQ_BAND_NO));
#else
        AUD_DspChEqBand(u1DecId, u1Idx, (UINT8)(_ai1ChEqFinalCfg[u1Idx]+EQ_BAND_GAIN_MID), (u1Idx == CH_EQ_BAND_NO));
#endif
    }
    VERIFY(x_sema_unlock(_ahEqCtlSema) == OSR_OK);
      
    AUD_DspEqEnable(u1DecId, TRUE);
}


// *********************************************************************
// Function : AUD_DspChEqBandMode(strucDspChBand rChBand, BYTE bLevel, BYTE fgNotify)
// Description : used to adjusting the equalizer's parameter for each channel and
// Parameter : u1BandIdx -- EQ band index
//             bLevel  -- range is from 0 to 28
//             fgNotify -- to indicate changing the parameter immediately or not
// Return    : None.
// *********************************************************************
void _AUD_DspChEqBandMode(UINT8 u1DecId, UINT8 u1BandIdx, UINT8 u1Level, BOOL fgNotify) 
{
    UINT8 u1Idx;
    UINT32 u4Val;
    UINT16 u2ShmIndex;
    UINT32 u4UopCmd;
    
    if (u1BandIdx >= EQ_BAND_MAX)
    {
        return;
    }

    if (u1DecId == AUD_DEC_MAIN)
    {
        u2ShmIndex = D_EQ_DRY;
        u4UopCmd = UOP_DSP_EQUALIZER_FLAG;
    }
    else
    {
        u2ShmIndex = D_EQ_DRY_DEC2;
        u4UopCmd = UOP_DSP_EQUALIZER_FLAG_DEC2;
    }
    
    if (u1BandIdx == EQ_BAND_DRY)
    {
        if (u1Level >= EQ_DRY_GAIN_INDEX_MAX)
        {
            u1Level = EQ_DRY_GAIN_INDEX_MAX - 1;
        }
        
        u4Val = _au4DryGain[u1Level];
        vWriteShmUINT32(u2ShmIndex, u4Val);
    }
    else
    {
        if (u1Level >= EQ_BAND_GAIN_INDEX_MAX)
        {
            u1Level = EQ_BAND_GAIN_INDEX_MAX - 1;
        }
        
        u2ShmIndex += (4 + _aau1EqBandExtend[u1BandIdx-1][0]);

        for (u1Idx=0; u1Idx<_aau1EqBandExtend[u1BandIdx-1][1]; u1Idx++)
        {
            vWriteShmUINT8((u2ShmIndex+u1Idx), u1Level);
        }     
    }

    if (fgNotify)
    {
        DSP_SendDspTaskCmd(u4UopCmd);
    }
}

//-----------------------------------------------------------------------------
/** AUD_DspChEqLoadPresetChgMode
 *   load the predefined channel equalizer setting for mode changing.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  eEqType  the valid type number is from 0 to type 7.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspChEqLoadPresetChgMode(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType) 
{
    UINT8     u1Idx;
#ifdef CC_AUD_EQ_BASED_SBASS
    INT8    i1TotalDb;
#endif

    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
  
    if (eEqType >= AUD_EQ_NUM)
    {
        eEqType = AUD_EQ_OFF;
    }

#ifdef CC_AUD_USE_FLASH_AQ
    #ifndef CC_AUD_NEW_AQ_PARSER
    if (eEqType == AUD_EQ_OFF) //In Flash AQ mode, when Sound mode is not custom, audio driver set EQ. 
    {
        eEqType = AUD_EQ_CUST1;
    }
    #endif
#endif // CC_AUD_USE_FLASH_AQ

    _aeEqMode[u1DecId] = eEqType;    

#ifdef CC_AUD_EQ_BASED_SBASS
    for (u1Idx=0; u1Idx<(CH_EQ_BAND_NO + 1); u1Idx++)
    {
        i1TotalDb = _aai1ChEqCfg[eEqType][u1Idx] + _aai1ChEqCfg[AUD_EQ_SBASS][u1Idx];
        if (u1Idx == (UINT8)EQ_BAND_DRY)
        {   
            // Dry gain value
            if (i1TotalDb > EQ_DRY_GAIN_SIDE_INDEX_MAX)
            {
                 i1TotalDb = EQ_DRY_GAIN_SIDE_INDEX_MAX;
            }

            if (i1TotalDb < -EQ_DRY_GAIN_SIDE_INDEX_MAX)
            {
                 i1TotalDb = -EQ_DRY_GAIN_SIDE_INDEX_MAX;
            }                        
        }
        else
        {
            // Band gain value
            if (i1TotalDb > EQ_BAND_GAIN_SIDE_INDEX_MAX)
            {
                 i1TotalDb = EQ_BAND_GAIN_SIDE_INDEX_MAX;
            }

            if (i1TotalDb < -EQ_BAND_GAIN_SIDE_INDEX_MAX)
            {
                 i1TotalDb = -EQ_BAND_GAIN_SIDE_INDEX_MAX;
            }                
        }
        _ai1ChEqFinalCfg[u1Idx] = i1TotalDb;
    }
#endif    

#ifdef SUPPORT_PHILIPS_LOUDNESS
#ifndef  CC_AUD_EQ_BASED_SBASS
    AUD_DspChEqLoudness(u1DecId,&(_aai1ChEqCfg[eEqType][1]));
#else
    AUD_DspChEqLoudness(u1DecId,&(_ai1ChEqFinalCfg[1]));
#endif
#endif
    VERIFY(x_sema_lock(_ahEqCtlSema, X_SEMA_OPTION_WAIT) == OSR_OK);
#ifndef  CC_AUD_EQ_BASED_SBASS
    AUD_DspChEqBandMode(u1DecId, (UINT8)EQ_BAND_DRY, (UINT8)(_aai1ChEqCfg[eEqType][0]+EQ_DRY_GAIN_MID), FALSE);
#else
    AUD_DspChEqBandMode(u1DecId, (UINT8)EQ_BAND_DRY, (UINT8)(_ai1ChEqFinalCfg[0]+EQ_DRY_GAIN_MID), FALSE);
#endif
    
    for(u1Idx = 1; u1Idx < (CH_EQ_BAND_NO + 1); u1Idx++)
    {
#ifndef  CC_AUD_EQ_BASED_SBASS
        AUD_DspChEqBandMode(u1DecId, u1Idx, (UINT8)(_aai1ChEqCfg[eEqType][u1Idx]+EQ_BAND_GAIN_MID), (u1Idx == CH_EQ_BAND_NO));
#else
        AUD_DspChEqBandMode(u1DecId, u1Idx, (UINT8)(_ai1ChEqFinalCfg[u1Idx]+EQ_BAND_GAIN_MID), (u1Idx == CH_EQ_BAND_NO));
#endif
    }
    VERIFY(x_sema_unlock(_ahEqCtlSema) == OSR_OK);
    
    AUD_DspEqEnable(u1DecId, TRUE);
}

//-----------------------------------------------------------------------------
/** AUD_DspChEqLoadPreset
 *   Get the current equalizer type.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  eEqType  
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspGetEqType(UINT8 u1DecId, AUD_EQ_TYPE_T* eEqType) 
{
    *eEqType = _aeEqMode[u1DecId];
}

//-----------------------------------------------------------------------------
/** _AUD_DspChSetEqTable
 *   set the band gain of a specific equalizer.
 *
 *  @param  u1DecId  Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  eEqType  the valid type number is from 0 to type 7.
 *  @param  eqValue  band gain, valid value from -20 ~ 20
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspChSetEqTable(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType, INT8 eqValue[]) 
{
    UINT8     u1Idx;

    VERIFY(x_sema_lock(_ahEqCtlSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    for (u1Idx = 0; u1Idx < (CH_EQ_BAND_NO + 1); u1Idx++)
    {       
        _aai1ChEqCfg[eEqType][u1Idx] = eqValue[u1Idx];
    }
    VERIFY(x_sema_unlock(_ahEqCtlSema) == OSR_OK);
    
}

//-----------------------------------------------------------------------------
/** _AUD_DspChQryEqTable
 *   query the band gain of a specific equalizer.
 *
 *  @param  u1DecId  Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  eEqType  the valid type number is from 0 to type 7.
 *  @param  eqValue  band gain, valid value from -20 ~ 20
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspChQryEqTable(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType, INT8 eqValue[]) 
{
    UINT8     u1Idx;

    for (u1Idx = 0; u1Idx < (CH_EQ_BAND_NO + 1); u1Idx++)
    {       
        eqValue[u1Idx] = _aai1ChEqCfg[eEqType][u1Idx];
    }    
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetLimiterMode
 *   set limiter mode
 *
 *  @param  u1Mode  limiter mode (0: off, 1: adaptive mode, 2: fixed mode). 
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetLimiterMode(UINT8 u1Mode)
{
    vWriteShmUINT16 (W_LIMITER_MODE, u1Mode);
    DSP_SendDspTaskCmd(UOP_DSP_LIMITER_CONFIG);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetLimiterFlag
 *   set limiter flag
 *
 *  @param  u1Flag  limiter flag (0: off, 1: on)
 *                         bit 0: vol
 *                         bit 1: vsurr
 *                         bit 2: sbass
 *                         bit 3: EQ
 *                         bit 4: reverb
 *                         bit 5: speaker
 *                         bit 6: voice
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetLimiterFlag(UINT8 u1Flag)
{
    vWriteShmUINT16 (W_LIMITER_FLAG, u1Flag);
    DSP_SendDspTaskCmd(UOP_DSP_LIMITER_CONFIG);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetLimiterThreshold
 *   set limiter threshold
 *
 *  @param  u4Thre  limiter threshold (0x0 ~ 0x7fffff)
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetLimiterThreshold(UINT32 u4Thre)
{
    vWriteShmUINT32 (D_LIMITER_FIX_THRESHOLD, u4Thre);
    DSP_SendDspTaskCmd(UOP_DSP_LIMITER_CONFIG);
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetLimiterConfig
 *   get limiter configuration
 *
 *  @param  *pu1Mode  mode
 *  @param  *pu2Flag  flag
 *  @param  *pu4Thre  threshold
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspGetLimiterConfig(UINT8 *pu1Mode, UINT16 *pu2Flag, UINT32 *pu4Thre)
{
    *pu1Mode = (UINT8)u2ReadShmUINT16 (W_LIMITER_MODE);
    *pu2Flag = u2ReadShmUINT16 (W_LIMITER_FLAG);
    *pu4Thre = u4ReadShmUINT32 (D_LIMITER_FIX_THRESHOLD);
}

#ifdef CC_RATIO_LIMITER
//-----------------------------------------------------------------------------
/** _AUD_DspSetLimiterGainRatio
 *   set limiter gain ratio
 *
 *  @param  u4Ratio  limiter gain ratio (0x0 ~ 0x7fffff)
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetLimiterGainRatio(UINT32 u4Ratio)
{
    vWriteShmUINT32 (D_LIMITER_GAIN_RATIO, u4Ratio);
    DSP_SendDspTaskCmd(UOP_DSP_LIMITER_CONFIG);
}
#endif

//-----------------------------------------------------------------------------
/** AUD_DspPL2Config
 *  main processing routine for Prologic 2.
 *
 *  @param  uCtrl control
                         PL2CTRL_SWITCH          0.
                         PL2CTRL_MODE            1.
                         PL2CTRL_PANORAMA        2.
                         PL2CTRL_DIMENSION       3.  
                         PL2CTRL_C_WIDTH         4.
 *  @param  uMode corresponding parameters for PL2 control (uCtrl).
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspPL2Config(UINT8 uCtrl, UINT8 uMode)
{
    UINT16 u2Config;
    UINT16 u2Mode;
    _rPL2Setting.u2Config = u2ReadShmUINT16(W_PLIICONFIG);
    _rPL2Setting.u2Mode = u2ReadShmUINT16(W_PLIIMODE);
    u2Config = _rPL2Setting.u2Config;
    u2Mode = _rPL2Setting.u2Mode;
    switch (uCtrl)
    {
    case PL2CTRL_SWITCH:
        if (uMode == PL2CTRL_ON)
        {
            _rPL2Setting.u2Config |=PL2_ENABLE;
            _rPL2Setting.u2Config &=~(PL2_AUTO_ENABLE|PL2_MODE_AUTO);
        }
        else if (uMode == PL2CTRL_AUTO)
        {
            _rPL2Setting.u2Config |=(PL2_ENABLE|PL2_AUTO_ENABLE|PL2_MODE_AUTO);  
        }
        else if (uMode == PL2CTRL_OFF)
        {
            _rPL2Setting.u2Config &=~(PL2_ENABLE|PL2_AUTO_ENABLE|PL2_MODE_AUTO);
        }
        break;
    case PL2CTRL_MODE:
        if ((uMode & 0x1) == PL2CTRL_MODE_AUTO)
        {
            _rPL2Setting.u2Config |=(PL2_PCD_AUTO|PL2_MODE_AUTO);
        }
        else
        {
            _rPL2Setting.u2Config &=~(PL2_MODE_AUTO);
            _rPL2Setting.u2Mode = uMode>>1;
        }
        break;      
    case PL2CTRL_PANORAMA:
        if (uMode == PL2CTRL_PAN_ON)
        {
            _rPL2Setting.u2Config |=PL2_PANORAMA;
        }
        else
        {
            _rPL2Setting.u2Config &=~PL2_PANORAMA;         
        }
        break;   
    case PL2CTRL_DIMENSION:
        /* clean field for dimension control*/
        _rPL2Setting.u2Config &= ~(PL2_DIM(0xf));
        _rPL2Setting.u2Config |= PL2_DIM((UINT16)uMode);        
        break;    
    case PL2CTRL_C_WIDTH:    
        /* clean field for center width configuration*/
        _rPL2Setting.u2Config &= ~(PL2_CWIDTH(0xf));
        _rPL2Setting.u2Config |= PL2_CWIDTH((UINT16)uMode);
        break;    
    default:
        return;
    }
    if ((u2Config == _rPL2Setting.u2Config) && 
        (u2Mode == _rPL2Setting.u2Mode))
    {
        /* if configuration is the same, don't send UOP*/
        return;
    }
    vWriteShmUINT16(W_PLIICONFIG, _rPL2Setting.u2Config);
    vWriteShmUINT16(W_PLIIMODE, _rPL2Setting.u2Mode);
    DSP_SendDspTaskCmd(UOP_DSP_PRO_LOGICII_CONFIG);
}
/******************************************************************************
* Function      : AUD_DspPL2ConfigInit
* Description   : initialization for PL2 configuration
* Parameter     : u2PL2cfg--> W_PLIICONFIG,u2PL2Mode-->W_PLIIMODE
* Return        : None 
******************************************************************************/
void _AUD_DspPL2ConfigInit(UINT16 u2PL2cfg,UINT16 u2PL2Mode) 
{
    /*
      NOTE: 
          We should call this function before audio play.
          We can have some special initialization ex. last memory from EEPROM
    */
    vWriteShmUINT16(W_PLIICONFIG,u2PL2cfg);
    vWriteShmUINT16(W_PLIIMODE,u2PL2Mode);
}
/******************************************************************************
* Function      : AUD_DspAC3KaraMode
* Description   : setup for AC3 karaoke mode
* Parameter     : u2KaraMode: Karaoke mode,uDecIndx: 0: first decoder 1: seconder decoder
* Return        : None 
******************************************************************************/
void _AUD_DspAC3KaraMode(UINT16 u2KaraMode,UINT8 uDecIndx) 
{
    UINT16 u2OldKaraMode;
    UINT16 u2ShmIndex=W_AC3KARAMOD;
    UINT16 u2UopIndex=UOP_DSP_AC3_KARAOKE_MODE;

    if (uDecIndx!=0)
    {
        u2ShmIndex = W_AC3KARAMOD_DEC2;
        u2UopIndex = UOP_DSP_AC3_KARAOKE_MODE_DEC2;
    }
   
    u2OldKaraMode = u2ReadShmUINT16(u2ShmIndex);

    if (u2OldKaraMode !=u2KaraMode)
    {
        vWriteShmUINT16(u2ShmIndex,u2KaraMode);
        DSP_SendDspTaskCmd(u2UopIndex);
    }
}
/******************************************************************************
* Function      : AUD_DspAC3DualMono
* Description   : setup for AC3 dual mono mode
* Parameter     : u2DMMode: dual mono mode,uDecIndx: 0: first decoder 1: seconder decoder
* Return        : None 
******************************************************************************/
void _AUD_DspAC3DualMono(UINT16 u2DMMode,UINT8 uDecIndx) 
{
    UINT16 u2OldDMMode;
    UINT16 u2ShmIndex=W_AC3DUALMODE;
    UINT16 u2UopIndex=UOP_DSP_AC3_DUAL_MONO_MODE;

    if (uDecIndx!=0)
    {
        u2ShmIndex = W_AC3DUALMODE_DEC2;
        u2UopIndex = UOP_DSP_AC3_DUAL_MONO_MODE;
    }
   
    u2OldDMMode = u2ReadShmUINT16(u2ShmIndex);

    if (u2DMMode != u2OldDMMode)
    {
        vWriteShmUINT16(u2ShmIndex,u2DMMode);
        DSP_SendDspTaskCmd(u2UopIndex);
    }
}

//-----------------------------------------------------------------------------
/** AUD_DspAC3CompMode
 *  setup for compression mode.
 *
 *  @param  u2CompMode compression mode.
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspAC3CompMode(UINT16 u2CompMode,UINT8 uDecIndx) 
{
    UINT16 u2OldDMMode;
    UINT16 u2ShmIndex=W_AC3COMPMOD;
    UINT16 u2UopIndex=UOP_DSP_AC3_COMPRESSION_MODE;

    if (uDecIndx!=0)
    {
        u2ShmIndex = W_AC3COMPMOD_DEC2;
        u2UopIndex = UOP_DSP_AC3_COMPRESSION_MODE_DEC2;
    }
   
    u2OldDMMode = u2ReadShmUINT16(u2ShmIndex);

    if (u2CompMode != u2OldDMMode)
    {
        vWriteShmUINT16(u2ShmIndex,u2CompMode);
        DSP_SendDspTaskCmd(u2UopIndex);
    }
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetAC3CompMode
 *  setup for compression mode.
 *
  *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval u2CompMode compression mode.
 */
//-----------------------------------------------------------------------------
UINT16 _AUD_DspGetAC3CompMode(UINT8 uDecIndx) 
{
    UINT16 u2DMMode;
    UINT16 u2ShmIndex=W_AC3COMPMOD;

    u2ShmIndex = (uDecIndx == AUD_DEC_MAIN)? W_AC3COMPMOD : W_AC3COMPMOD_DEC2;
   
    u2DMMode = u2ReadShmUINT16(u2ShmIndex);
    return u2DMMode;

}

//-----------------------------------------------------------------------------
/** AUD_DspAC3DRCRange
 *  setup range for dynamix range compression.
 *
 *  @param  uDRCLevel  DRC range.
 *                               0x00000000-->0.0
 *                               0x007FFFFF-->1.0
 *                               0.0 ~ 1.0 step 0.125
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspAC3DRCRange(UINT8 uDRCLevel,UINT8 u1DecId)
{
    UINT16 u2DRCLowIdx=D_AC3DYN_LOW;
    UINT16 u2DRCHighIdx=D_AC3DYN_HIGH;
    UINT16 u2DRCLowUop=UOP_DSP_AC3_DYNAMIC_LOW;
    UINT16 u2DRCHighUop=UOP_DSP_AC3_DYNAMIC_HIGH;
    
    if (uDRCLevel >= DRC_RANGE_MAX)
    {
        uDRCLevel = DRC_RANGE_MAX - 1;
    }
    if (u1DecId != 0)
    {
        u2DRCLowIdx=D_AC3DYN_LOW_DEC2;
        u2DRCHighIdx=D_AC3DYN_HIGH_DEC2;
        u2DRCLowUop=UOP_DSP_AC3_DYNAMIC_LOW_DEC2;
        u2DRCHighUop=UOP_DSP_AC3_DYNAMIC_HIGH_DEC2;  
    }
    
    vWriteShmUINT32(u2DRCLowIdx, _au4Ac3DrcRange[uDRCLevel]);
    DSP_SendDspTaskCmd(u2DRCLowUop);
    vWriteShmUINT32(u2DRCHighIdx, _au4Ac3DrcRange[uDRCLevel]);
    DSP_SendDspTaskCmd(u2DRCHighUop);  
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetAC3DRCRange
 *  setup range for dynamix range compression.
 *
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval uDRCLevel  DRC range.
 *                               0x00000000-->0.0
 *                               0x007FFFFF-->1.0
 *                               0.0 ~ 1.0 step 0.125
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetAC3DRCRange(UINT8 u1DecId)
{
    UINT16 u2DRCIdx;
    UINT32 u4DRCLevel;
    UINT8 uDRCLevelIndex;

    u2DRCIdx=(u1DecId==AUD_DEC_MAIN)? D_AC3DYN_LOW: D_AC3DYN_LOW_DEC2;
    u4DRCLevel = u4ReadShmUINT32(u2DRCIdx);
    
    for (uDRCLevelIndex = 0; uDRCLevelIndex<DRC_RANGE_MAX; uDRCLevelIndex++ )
    {
        if (u4DRCLevel == _au4Ac3DrcRange[uDRCLevelIndex])
        {
            break;
        }
    }
    return uDRCLevelIndex;
    
}

/******************************************************************************
* Function      : AdspSpeakerConfig
* Description   : speaker configuration setup
* Parameter     : u4SpkCfg: Speaker config,uDecIndx: 0: first decoder 1: seconder decoder
* Return        : None 
******************************************************************************/
/*
  D_SPKCFG,D_SPKCFG_DEC2 (currently second decoder only support 2/0,1/0,3/0 configuration)
  length: 24 bits
  description is the following
  bit 0 ~ 2:
    b000: 2/0 (LT/RT downmix: prologic compatible) 
    b001: 1/0
    b010: 2/0 (LO/RO)
    b011: 3/0
    b100: 2/1
    b101: 3/1
    b110: 2/2
    b111: 3/2
  bit 5: Subwoofer Channel present(1)/absent(0)
  bit 12: Center Channel large(1)/small(0)
  bit 13: Left Channel large(1)/small(0)
  bit 14: Right Channel large(1)/small(0)
  bit 15: Left Surround Channel large(1)/small(0)
  bit 16: Right Surround Channel large(1)/small(0)
*/
void _AUD_DspSpeakerConfig(UINT32 u4SpkCfg,UINT8 uDecIndex)
{
    UINT16 u2SpkCfgIndx = D_SPKCFG;
    UINT16 u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER;
    if (uDecIndex!=0)
    {
        u2SpkCfgIndx = D_SPKCFG_DEC2;
        u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER_DEC2;
    }
    vWriteShmUINT32(u2SpkCfgIndx, u4SpkCfg);
    DSP_SendDspTaskCmd(u2SpkCfgUop);
}

/******************************************************************************
* Function      : AUD_DspSpeakerSizeConfig
* Description   : speaker configuration setup
* Parameter     : u4SpkCfg: Speaker config,uDecIndx: 0: first decoder 1: seconder decoder
* Return        : None 
******************************************************************************/
/*
  D_SPKCFG,D_SPKCFG_DEC2 (currently second decoder only support 2/0,1/0,3/0 configuration)
  length: 24 bits
  description is the following
  bit 0 ~ 2:
    b000: 2/0 (LT/RT downmix: prologic compatible) 
    b001: 1/0
    b010: 2/0 (LO/RO)
    b011: 3/0
    b100: 2/1
    b101: 3/1
    b110: 2/2
    b111: 3/2
  bit 5: Subwoofer Channel present(1)/absent(0)
  bit 12: Center Channel large(1)/small(0)
  bit 13: Left Channel large(1)/small(0)
  bit 14: Right Channel large(1)/small(0)
  bit 15: Left Surround Channel large(1)/small(0)
  bit 16: Right Surround Channel large(1)/small(0)
*/

void _AUD_DspSpeakerSizeConfig(UINT8 uDecIndex, BOOL fgFrontLarge, BOOL fgSurroundLarge, BOOL fgCenterLarge)
{
    UINT16 u2SpkCfgIndx = D_SPKCFG;
    UINT16 u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER;
    UINT32 u4SpkCfg = 0;
    UNUSED(fgSurroundLarge);
    
    if (uDecIndex!=0)
    {
        u2SpkCfgIndx = D_SPKCFG_DEC2;
        u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER_DEC2;
        u4SpkCfg = u4ReadShmUINT32(D_SPKCFG_DEC2) & ~DSP_SHM_SPK_SIZE_MASK;
    }
    else
    {
        u4SpkCfg = u4ReadShmUINT32(D_SPKCFG) & ~DSP_SHM_SPK_SIZE_MASK;
    }

    if (fgFrontLarge)
    {
        u4SpkCfg |= ((UINT32)0x3 << 13);    
    }
    if (fgFrontLarge)
    {
        u4SpkCfg |= ((UINT32)0x3 << 15);    
    }
    if (fgCenterLarge)
    {
        u4SpkCfg |= ((UINT32)0x1 << 12);    
    }
    
    vWriteShmUINT32(u2SpkCfgIndx, u4SpkCfg);
    DSP_SendDspTaskCmd(u2SpkCfgUop);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetSpeakerConfig
 *  Get speaker configuration
 *  @param  u1DecIndex          Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  pu4SpkCfg           Speaker configuration.
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
void _AUD_DspGetSpeakerConfig(UINT8 u1DecIndex, UINT32 *pu4SpkCfg)
{
    UINT16 u2SpkCfgIndx = D_SPKCFG;
    
    if (u1DecIndex!=0)
    {
        u2SpkCfgIndx = D_SPKCFG_DEC2;   
    }

    if (pu4SpkCfg != NULL)
    {
        *pu4SpkCfg = u4ReadShmUINT32(u2SpkCfgIndx);
    }    
}

//-----------------------------------------------------------------------------
/** AUD_DspSpeakerLargeSmallConfig
 *  Speaker size configuration of each channel pair.
 *  @param  u1DecIndex          Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  u1FrontPair          Speaker size configuration for front left and right channels. (1: large, 0: small)
 *  @param  u2Center             Speaker size configuration for center channel. (1: large, 0: small)
 *  @param  u1Surround          Speaker size configuration for left and right surround channels. (1: large, 0: small)
 *  @param  u1CH910              Speaker size configuration for CH9/10 channels. (1: large, 0: small)
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
// large(1)/small(0)
void _AUD_DspSpeakerLargeSmallConfig(UINT8 u1DecIndex, UINT8 u1FrontPair, UINT8 u2Center, UINT8 u1Surround, UINT8 u1CH910)
{
    UINT32 u4Config = 0;
    UINT16 u2SpkCfgIndx = D_SPKCFG;
    UINT16 u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER;
    if (u1DecIndex != AUD_DEC_MAIN)
    {
        u2SpkCfgIndx = D_SPKCFG_DEC2;
        u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER_DEC2;
    }

    if (u2Center == 1)
    {
        u4Config |= 0x01 << 12; // bit 12
    }   
    if (u1FrontPair == 1)
    {
        u4Config |= 0x06 << 12; // bit 13 & 14
    }   
    if (u1Surround == 1)
    {
        u4Config |= 0x18 << 12; // bit 15 & 16
    }   
    if (u1CH910 == 1)
    {
        u4Config |= 0x3 << 19; // bit 20 & 19
    }   

    vWriteShmUINT32(u2SpkCfgIndx, (u4ReadShmUINT32(u2SpkCfgIndx) & 0xffe60fff) | u4Config);
    DSP_SendDspTaskCmd(u2SpkCfgUop);
}

//-----------------------------------------------------------------------------
/** AUD_DspSpeakerLsConfig
 *  setup speaker size of each channel.
 *
 *  @param  u1DecIndex 0: first decoder 1: seconder decoder.
 *  @param  u1L  large(1)/small(0).
 *  @param  u1R  large(1)/small(0).                       
 *  @param  u1Ls   large(1)/small(0).                      
 *  @param  u1Rs   large(1)/small(0).                      
 *  @param  u1C  large(1)/small(0).
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSpeakerLsConfig(UINT8 u1DecIndex, UINT8 u1L, UINT8 u1R, UINT8 u1Ls, UINT8 u1Rs, UINT8 u1C)
{
    UINT32 u4Config = 0;
    UINT16 u2SpkCfgIndx = D_SPKCFG;
    UINT16 u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER;
    if (u1DecIndex != AUD_DEC_MAIN)
    {
        u2SpkCfgIndx = D_SPKCFG_DEC2;
        u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER_DEC2;
    }

    if (u1C == 1)
    {
        u4Config |= 0x01 << 12; // bit 12
    }   
    if (u1L == 1)
    {
        u4Config |= 0x01 << 13; // bit 13
    }   
    if (u1R == 1)
    {
        u4Config |= 0x01 << 14; // bit 14   
    }   
    if (u1Ls == 1)
    {
        u4Config |= 0x01 << 15; // bit 15       
    }   
    if (u1Rs == 1)
    {
        u4Config |= 0x01 << 16; // bit 16
    }   
    
    vWriteShmUINT32(u2SpkCfgIndx, (u4ReadShmUINT32(u2SpkCfgIndx) & 0xfffe0fff) | u4Config);
    DSP_SendDspTaskCmd(u2SpkCfgUop);
}

//static UINT32 _au4SpeakerOutCfg[AUD_DEC_NUM]= {7,7};
//#define SPK_CFG_MASK (0x07)

//-----------------------------------------------------------------------------
/** AUD_DspSpeakerOutputConfig
 *  set speaker output config.
 *
 *  @param  u1DecIndex 0: first decoder 1: seconder decoder.
 *  @param  u1SpkCfg.  
 *                              bit 0 ~ 2:.
 *                              b000: 2/0 (LT/RT downmix: prologic compatible). 
 *                              b001: 1/0.
 *                              b010: 2/0 (LO/RO).
 *                              b011: 3/0.
 *                              b100: 2/1.
 *                              b101: 3/1.
 *                              b110: 2/2.
 *                              b111: 3/2.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSpeakerOutputConfig(UINT8 u1DecIndex, UINT8 u1SpkCfg)
{
    UINT16 u2SpkCfgIndx = D_SPKCFG;
    UINT16 u2AutoDmxIndx = B_AC3AUTODNMIX;
    UINT16 u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER;

    UINT8 u1CurSpkCfg = 0;

//    if ((u1SpkCfg & SPK_CFG_MASK) != _au4SpeakerOutCfg[u1DecIndex])
    {
        if (u1DecIndex != AUD_DEC_MAIN)
        {
            u2SpkCfgIndx = D_SPKCFG_DEC2;
            u2AutoDmxIndx = B_AC3AUTODNMIX_DEC2;
            u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER_DEC2;
        }

        // If new setting is the same as the original
        u1CurSpkCfg= (u4ReadShmUINT32(u2SpkCfgIndx) & 0x7);
        if (u1CurSpkCfg == u1SpkCfg)
        {
            return;
        }
        
        vWriteShmUINT32(u2SpkCfgIndx, (u4ReadShmUINT32(u2SpkCfgIndx) & 0xfffffff8) | (UINT32)u1SpkCfg);
        vWriteShmUINT8(u2AutoDmxIndx, FALSE); // turn off auto downmix
        DSP_SendDspTaskCmd(u2SpkCfgUop);
    }
}

//-----------------------------------------------------------------------------
/** AUD_DspSpeakerSubwooferEnable
 *  Turn On/Off subwoofer
 *  @param  u1DecIndex          Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  fgEnable              Subwoofer channel is turned on or off. (1: on, 0: off)
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
// bit 5: Subwoofer Channel present(1)/absent(0)
void _AUD_DspSpeakerSubwooferEnable(UINT8 u1DecIndex, BOOL fgEnable)
{
    UINT32 u4Config = 0;
    UINT16 u2SpkCfgIndx = D_SPKCFG;
    UINT16 u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER;
    if (u1DecIndex != AUD_DEC_MAIN)
    {
        u2SpkCfgIndx = D_SPKCFG_DEC2;
        u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER_DEC2;
    }
    if (fgEnable)
    {
        u4Config |= 1<<5;
    }    
    else
    {
        u4Config = 0;
    }   
    
    vWriteShmUINT32(u2SpkCfgIndx, (u4ReadShmUINT32(u2SpkCfgIndx) & 0xffffffdf) | u4Config);
    DSP_SendDspTaskCmd(u2SpkCfgUop);
}

//-----------------------------------------------------------------------------
/** AUD_DspChannelDelay
 *  setup channel delay for individual channel.
 *
 *  @param  u2Delay 0.05 m/unit.
 *  @param  eChIndex 0(L), 1(R), 2(LS), 3(RS), 4(C), 5(SUB), 6(Bypass L), 7(Bypass R)
 *                   8(Downmix L), 9(Downmix R), 10(AUD L), 11(AUD R), 13(All), 14(LFE)
 *  @param  u1DecIndex 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspChannelDelay(UINT16 u2Delay, AUD_CH_T eChIndex, UINT8 uDecIndex)
{
    UINT16 u2ShmIndex;
    UINT16 u2UopIndex;
    UINT8  uIdx;

    if ((eChIndex > AUD_CH_NUM) || (eChIndex == AUD_CH_INPUT_SRC))
    {
        return;
    }
    
    if (eChIndex == AUD_CH_ALL)
    {
        // Delay for all channels
        u2ShmIndex = W_CHDELAY_C;
        if(AUD_GetTargetCountry() == COUNTRY_EU)
        {
            vWriteShmUINT16( W_CHDELAY_C , u2Delay);
            vWriteShmUINT16( W_CHDELAY_L , u2Delay);
            vWriteShmUINT16( W_CHDELAY_R , u2Delay);
            vWriteShmUINT16( W_CHDELAY_LS , u2Delay);
            vWriteShmUINT16( W_CHDELAY_RS , u2Delay);
            vWriteShmUINT16( W_CHDELAY_SUB , u2Delay);
            vWriteShmUINT16( W_CHDELAY_CH9 , u2Delay);
            vWriteShmUINT16( W_CHDELAY_CH10 , u2Delay);
        }
        else
        {          
            for (uIdx=0; uIdx<=AUD_CH_DMX_RIGHT; uIdx++)
            {
                vWriteShmUINT16(u2ShmIndex, u2Delay);
                u2ShmIndex += 2;
            }
        }
        u2UopIndex=UOP_DSP_CONFIG_DELAY_ALL;
        DSP_SendDspTaskCmd(u2UopIndex);
        LOG(3,"$$$$$$ Enter _AUD_DspChannelDelay #######\n");
    }
    else
    {
        // Delay for individual channel
        switch (eChIndex)
        {
        case AUD_CH_FRONT_LEFT: 
            u2ShmIndex = W_CHDELAY_L;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_L;
            break;
        case AUD_CH_FRONT_RIGHT:
            u2ShmIndex = W_CHDELAY_R;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_R;        
            break;
        case AUD_CH_REAR_LEFT: 
            u2ShmIndex = W_CHDELAY_LS;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_LS;        
            break;
        case AUD_CH_REAR_RIGHT: 
            u2ShmIndex = W_CHDELAY_RS;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_RS;        
            break;
        case AUD_CH_CENTER: 
            u2ShmIndex = W_CHDELAY_C;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_C;        
            break;
        case AUD_CH_SUB_WOOFER: 
        case AUD_CH_LFE:
            u2ShmIndex = W_CHDELAY_SUB;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_SUBWOOFER;        
            break;
        case AUD_CH_BYPASS_LEFT: 
            u2ShmIndex = W_CHDELAY_CH7;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_CH7;        
            break;
        case AUD_CH_BYPASS_RIGHT: 
            u2ShmIndex = W_CHDELAY_CH8;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_CH8;            
            break;
        case AUD_CH_DMX_LEFT: 
            u2ShmIndex = W_CHDELAY_CH9;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_CH9;            
            break;
        case AUD_CH_DMX_RIGHT: 
            u2ShmIndex = W_CHDELAY_CH10;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_CH10;            
            break;
        case AUD_CH_AUX_FRONT_LEFT: 
            u2ShmIndex = W_CHDELAY_L_DEC2;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_L_DEC2;            
            break;
        case AUD_CH_AUX_FRONT_RIGHT: 
            u2ShmIndex = W_CHDELAY_R_DEC2;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_R_DEC2;               
            break;
        default:
            u2ShmIndex = W_CHDELAY_C;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_C;        
            break;
        }      

        if (u2ReadShmUINT16(u2ShmIndex) == u2Delay)
        {
            return;
        }
        else
        {
            vWriteShmUINT16(u2ShmIndex, u2Delay);
            DSP_SendDspTaskCmd(u2UopIndex);
        }
    }
}

#ifdef CC_ENABLE_AV_SYNC
//-----------------------------------------------------------------------------
/** _AUD_DspChannelDelay_initial
 *  setup channel delay for individual channel.
 *
 *  @param  u2Delay 0.05 m/unit.
 *  @param  eChIndex 0(L), 1(R), 2(LS), 3(RS), 4(C), 5(SUB), 6(Bypass L), 7(Bypass R)
 *                   8(Downmix L), 9(Downmix R), 10(AUD L), 11(AUD R), 13(All), 14(LFE)
 *  @param  u1DecIndex 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspChannelDelay_initial(UINT16 u2Delay, AUD_CH_T eChIndex, UINT8 uDecIndex)
{
    UINT16 u2ShmIndex;
    UINT16 u2UopIndex;
    UINT8  uIdx;

    if ((eChIndex > AUD_CH_NUM) || (eChIndex == AUD_CH_INPUT_SRC))
    {
        return;
    }
    
    if (eChIndex == AUD_CH_ALL)
    {
        // Delay for all channels
        u2ShmIndex = W_CHDELAY_C;
        if(AUD_GetTargetCountry() == COUNTRY_EU)
        {
            vWriteShmUINT16( W_CHDELAY_C , u2Delay);
            vWriteShmUINT16( W_CHDELAY_L , u2Delay);
            vWriteShmUINT16( W_CHDELAY_R , u2Delay);
            vWriteShmUINT16( W_CHDELAY_LS , u2Delay);
            vWriteShmUINT16( W_CHDELAY_RS , u2Delay);
            vWriteShmUINT16( W_CHDELAY_SUB , u2Delay);
            vWriteShmUINT16( W_CHDELAY_CH9 , u2Delay);
            vWriteShmUINT16( W_CHDELAY_CH10 , u2Delay);
        }
        else
        {          
            for (uIdx=0; uIdx<=AUD_CH_DMX_RIGHT; uIdx++)
            {
                vWriteShmUINT16(u2ShmIndex, u2Delay);
                u2ShmIndex += 2;
            }
        }
        //u2UopIndex=UOP_DSP_CONFIG_DELAY_ALL;
        //DSP_SendDspTaskCmd(u2UopIndex);
        LOG(3,"$$$$$$ Enter _AUD_DspChannelDelay INITIAL. This will not send uop to DSP #######\n");
    }
    else
    {
        // Delay for individual channel
        switch (eChIndex)
        {
        case AUD_CH_FRONT_LEFT: 
            u2ShmIndex = W_CHDELAY_L;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_L;
            break;
        case AUD_CH_FRONT_RIGHT:
            u2ShmIndex = W_CHDELAY_R;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_R;        
            break;
        case AUD_CH_REAR_LEFT: 
            u2ShmIndex = W_CHDELAY_LS;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_LS;        
            break;
        case AUD_CH_REAR_RIGHT: 
            u2ShmIndex = W_CHDELAY_RS;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_RS;        
            break;
        case AUD_CH_CENTER: 
            u2ShmIndex = W_CHDELAY_C;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_C;        
            break;
        case AUD_CH_SUB_WOOFER: 
        case AUD_CH_LFE:
            u2ShmIndex = W_CHDELAY_SUB;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_SUBWOOFER;        
            break;
        case AUD_CH_BYPASS_LEFT: 
            u2ShmIndex = W_CHDELAY_CH7;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_CH7;        
            break;
        case AUD_CH_BYPASS_RIGHT: 
            u2ShmIndex = W_CHDELAY_CH8;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_CH8;            
            break;
        case AUD_CH_DMX_LEFT: 
            u2ShmIndex = W_CHDELAY_CH9;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_CH9;            
            break;
        case AUD_CH_DMX_RIGHT: 
            u2ShmIndex = W_CHDELAY_CH10;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_CH10;            
            break;
        case AUD_CH_AUX_FRONT_LEFT: 
            u2ShmIndex = W_CHDELAY_L_DEC2;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_L_DEC2;            
            break;
        case AUD_CH_AUX_FRONT_RIGHT: 
            u2ShmIndex = W_CHDELAY_R_DEC2;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_R_DEC2;               
            break;
        default:
            u2ShmIndex = W_CHDELAY_C;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_C;        
            break;
        }      

        if (u2ReadShmUINT16(u2ShmIndex) == u2Delay)
        {
            return;
        }
        else
        {
            vWriteShmUINT16(u2ShmIndex, u2Delay);
            DSP_SendDspTaskCmd(u2UopIndex);
        }
    }
}
#endif





//-----------------------------------------------------------------------------
/** AUD_DspChannelVolume
 *  Get channel volume of individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel. 
 *                               1: front right channel.  
 *                               2: rear left channel. 
 *                               3: rear right channel. 
 *                               4: center channel.
 *                               5: sub woofer channel. 
 *                               6: bypass left channel. 
 *                               7: bypass right channel. 
 *                               8: downmix left channel. 
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ). 
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetChannelVolume(UINT8 u1DecId, AUD_CH_T eChannel)
{
    // Check input, for lint
    if (u1DecId >= AUD_DEC_NUM)
    {
        u1DecId = 0;
    }
    
    return _aau1ChannelVolume[u1DecId][eChannel];
}

//-----------------------------------------------------------------------------
/** AUD_DspChannelDelay
 *  setup channel delay by input source.
 *
 *  @param  eStreamFrom
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspInputChannelDelay(AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    _AUD_DspChannelDelay(_au2ChDelay[(UINT8)eStreamFrom], AUD_CH_ALL, AUD_DEC_MAIN);
}

//-----------------------------------------------------------------------------
/** AUD_DspChannelVolume
 *  setup channel volume for individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel. 
 *                               1: front right channel.  
 *                               2: rear left channel. 
 *                               3: rear right channel. 
 *                               4: center channel.
 *                               5: sub woofer channel. 
 *                               6: bypass left channel. 
 *                               7: bypass right channel. 
 *                               8: downmix left channel. 
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ). 
 *  @param  u1Value 0~100.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspChannelVolume(UINT8 u1DecId, AUD_CH_T eChannel, UINT8 u1Value)
{
#ifndef CC_MT5391_AUD_3_DECODER
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD));
#endif
    VOL_CTL_SEMA_LOCK(u1DecId);
    _fgChannelVolUseShmRawData = FALSE;
    _aau1ChannelVolume[u1DecId][eChannel] = u1Value;
    _AUD_DspVolumeChange(u1DecId, eChannel);
    VOL_CTL_SEMA_UNLOCK(u1DecId);
}

#ifdef CC_AUD_LINEOUT_VOL_OFFSET
//-----------------------------------------------------------------------------
/** _AUD_DspLineOutVolOffset
 *  setup bypass channel volume offset 
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Value 0~100.
 *  @retval void
 */
//-----------------------------------------------------------------------------

void _AUD_DspLineOutVolOffset( UINT8 u1DecId, UINT8 u1Value)
{
#ifndef CC_MT5391_AUD_3_DECODER
		VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
		VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD));
#endif
		VOL_CTL_SEMA_LOCK(u1DecId);
		_aau1ChannelVolOffset[u1DecId][AUD_CH_BYPASS_LEFT] = u1Value;
		_aau1ChannelVolOffset[u1DecId][AUD_CH_BYPASS_RIGHT] = u1Value;
		_AUD_DspVolumeChange(u1DecId, AUD_CH_BYPASS_LEFT);
		_AUD_DspVolumeChange(u1DecId, AUD_CH_BYPASS_RIGHT);
		VOL_CTL_SEMA_UNLOCK(u1DecId);
}
#endif
//-----------------------------------------------------------------------------
/** _AUD_DspChannelVolShmRawValue
 *  Set master volume use share memory raw data .
 *
 *  Note that AUD_DspChannelVolume & _AUD_DspChannelVolShmRawValue can only select
 *  one to use
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u4VolShm : 0 ~ 0x20000
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspChannelVolShmRawValue(UINT8 u1DecId, AUD_CH_T eChannel, UINT32 u4VolShm)
{
    VOL_CTL_SEMA_LOCK(u1DecId);
    _fgChannelVolUseShmRawData = TRUE;
    _aau4ChannelVolumeRawData[u1DecId][eChannel] = u4VolShm;
    _AUD_DspVolumeChange(u1DecId, eChannel);
    VOL_CTL_SEMA_UNLOCK(u1DecId);
}

//-----------------------------------------------------------------------------
/** _AUD_DspChannelMute
 *  setup mute for individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel. 
 *                               1: front right channel.  
 *                               2: rear left channel. 
 *                               3: rear right channel. 
 *                               4: center channel.
 *                               5: sub woofer channel. 
 *                               6: bypass left channel. 
 *                               7: bypass right channel. 
 *                               8: downmix left channel. 
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ). 
 *  @param  fgMute TRUE, FALSE
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspChannelMute(UINT8 u1DecId, AUD_CH_T eChannel, BOOL fgMute)
{   
#ifndef CC_MT5391_AUD_3_DECODER
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD));
#endif
    VOL_CTL_SEMA_LOCK(u1DecId);
    _aafgChlMute[u1DecId][eChannel] = fgMute;
    _AUD_DspVolumeChange(u1DecId, eChannel);
    VOL_CTL_SEMA_UNLOCK(u1DecId);
}
#ifdef CC_AUD_VOLUME_OFFSET
void _AUD_DspVolumeOffset(UINT8 u1DecId,UINT8 u1Index)
{
    INT32 i4Value;

    if( u1Index > AUD_VOLUME_OFFSET_IDX_NUM-1 )
    u1Index = AUD_VOLUME_OFFSET_IDX_NUM -1 ;
	
    i4Value = (_aai4VolOffsetMap[u1Index] -128);
    
    _AUD_DspChannelVolGain(u1DecId,AUD_CH_FRONT_LEFT,i4Value);
    _AUD_DspChannelVolGain(u1DecId,AUD_CH_FRONT_RIGHT,i4Value);

    if(_aafgLineOutVolChangeFlag)
    {
        _AUD_DspChannelVolGain(u1DecId,AUD_CH_BYPASS_LEFT,i4Value);
        _AUD_DspChannelVolGain(u1DecId,AUD_CH_BYPASS_RIGHT,i4Value);
    }
    else
    {
        _AUD_DspChannelVolGain(u1DecId,AUD_CH_BYPASS_LEFT,0);//fix
        _AUD_DspChannelVolGain(u1DecId,AUD_CH_BYPASS_RIGHT,0);//fix
    }

}
#endif
//-----------------------------------------------------------------------------
/** AUD_DspChannelVolGain
 *  setup channel volume extra gain for individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel. 
 *                               1: front right channel.  
 *                               2: rear left channel. 
 *                               3: rear right channel. 
 *                               4: center channel.
 *                               5: sub woofer channel. 
 *                               6: bypass left channel. 
 *                               7: bypass right channel. 
 *                               8: downmix left channel. 
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ). 
 *  @param  i4Value     -256~96.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspChannelVolGain(UINT8 u1DecId, AUD_CH_T eChannel, INT32 i4Value)
{
#ifndef CC_MT5391_AUD_3_DECODER
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD));
#endif

    if (eChannel >= AUD_CH_NUM)
    {
        LOG(0, "Incorrect parameter -- eChannel for _AUD_DspChannelVolGain\n");
        return;
    }

    VERIFY(x_sema_lock(_ahVolCtlSema[u1DecId], X_SEMA_OPTION_WAIT) == OSR_OK);
    
    // Update local variables
    _aau1ChlVolExtraGain[u1DecId][eChannel] = i4Value;
    _AudDspChannelVolChange(u1DecId, eChannel);
    
    VERIFY(x_sema_unlock(_ahVolCtlSema[u1DecId]) == OSR_OK);
}

//-----------------------------------------------------------------------------
/** _AUD_DspChannelOutPort
 *  setup output port for individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel. 
 *                               1: front right channel.  
 *                               2: rear left channel. 
 *                               3: rear right channel. 
 *                               4: center channel.
 *                               5: sub woofer channel. 
 *                               6: bypass left channel. 
 *                               7: bypass right channel. 
 *                               8: downmix left channel. 
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ). 
 *  @param  fgEnable TRUE, FALSE
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspChannelOutPort(UINT8 u1DecId, AUD_CH_T eChannel, BOOL fgEnable)
{   
    VOL_CTL_SEMA_LOCK(u1DecId);
    _aafgChlOutPort[u1DecId][eChannel] = fgEnable;
    _AUD_DspVolumeChange(u1DecId, eChannel);
    VOL_CTL_SEMA_UNLOCK(u1DecId);
}

//-----------------------------------------------------------------------------
/** AUD_DspBalance
 *  setup balance.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Balance  : balance value (0~100).
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspBalance(UINT8 u1DecId, UINT8 u1Balance)
{
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
    VOL_CTL_SEMA_LOCK(u1DecId);
     
    _au1Balance[u1DecId] = u1Balance;

    if ((UINT8)DRVCUST_OptGet(eAudioDownmixPosition) == DOWNMIX_CH_DO_ALL_POST_PROC)
    {
    _AUD_DspVolumeChange(u1DecId, AUD_CH_DMX_LEFT);
    _AUD_DspVolumeChange(u1DecId, AUD_CH_DMX_RIGHT);
    }

    _AUD_DspVolumeChange(u1DecId, AUD_CH_FRONT_LEFT);
    _AUD_DspVolumeChange(u1DecId, AUD_CH_FRONT_RIGHT);
    _AUD_DspVolumeChange(u1DecId, AUD_CH_REAR_LEFT);
    _AUD_DspVolumeChange(u1DecId, AUD_CH_REAR_RIGHT);
    
    VOL_CTL_SEMA_UNLOCK(u1DecId);
}

//-----------------------------------------------------------------------------
/** AUD_DspMasterVolumeFineTune
 *  setup master volume to DSP directly.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u4Volume  : volume value (0~0x7fffff).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
// 2006/08/28, created for tuning volume table
void _AUD_DspMasterVolumeFineTune(UINT8 u1DecId, UINT32 u4Volume)
{
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
    VOL_CTL_SEMA_LOCK(u1DecId);
    vWriteShmUINT32(D_VOL, u4Volume);
    DSP_SendDspTaskCmd(UOP_DSP_MASTER_VOLUME);
    VOL_CTL_SEMA_UNLOCK(u1DecId);
}

//-----------------------------------------------------------------------------
/** AUD_DspMasterVolume
 *  set master volume.
 *
 *  Note that AUD_DspMasterVolume & _AUD_DspMasterVolShmRawValue can only select
 *  one to use
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Volume  : volume value (0~100).
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspMasterVolume(UINT8 u1DecId, UINT8 u1Volume)
{
#ifndef CC_MT5391_AUD_3_DECODER
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD));
#endif
    VOL_CTL_SEMA_LOCK(u1DecId);
    _fgMasterVolUseShmRawData = FALSE;
    _aau1ChannelVolume[u1DecId][AUD_CH_ALL] = u1Volume;
    _AUD_DspVolumeChange(u1DecId, AUD_CH_ALL);

#ifdef CC_AUD_LINEOUT_VOL_MODE
    if(_aafgLineOutVolChangeFlag)
    {
       _aau1ChannelVolume[u1DecId][AUD_CH_BYPASS_LEFT] = u1Volume;
       _aau1ChannelVolume[u1DecId][AUD_CH_BYPASS_RIGHT] = u1Volume;
       _AUD_DspVolumeChange(u1DecId, AUD_CH_BYPASS_LEFT);
       _AUD_DspVolumeChange(u1DecId, AUD_CH_BYPASS_RIGHT);
	}
#endif

    VOL_CTL_SEMA_UNLOCK(u1DecId);
}

//-----------------------------------------------------------------------------
/** _AUD_DspMasterVolShmRawValue
 *  Set master volume use share memory raw data .
 *
 *  Note that AUD_DspMasterVolume & _AUD_DspMasterVolShmRawValue can only select
 *  one to use
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u4VolShm : 0 ~ 0x20000
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspMasterVolShmRawValue(UINT8 u1DecId, UINT32 u4VolShm)
{
#ifndef CC_MT5391_AUD_3_DECODER
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD));
#endif
    VOL_CTL_SEMA_LOCK(u1DecId);
    _fgMasterVolUseShmRawData = TRUE;
    _au4MasterVolShm[u1DecId] = u4VolShm; 
    _AudDspMasterVolChange(u1DecId);
    VOL_CTL_SEMA_UNLOCK(u1DecId);
}

#ifdef CC_AUD_LINEOUT_VOL_MODE
BOOL AUD_GetLineOutVolumeMode(UINT8 u1DecId)
{
	return _aafgLineOutVolChangeFlag;
}

//-----------------------------------------------------------------------------
/** _AUD_SetLineOutVolumeMode
 *  Set line out volume control mode.
 * 
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  fgMode : 0 : fixed 1:variable as master volume 
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_SetLineOutVolumeMode(UINT8 u1DecId, UINT8 fgMode)
{
    _aafgLineOutVolChangeFlag = fgMode;
    vWriteShmUINT8(B_LINEOUT_VOL_MODE, (UINT8)fgMode);
}
UINT8 _AUD_DspGetLineOutFixVol(void)
{
   return _aau1LineOutFixVol;
}
#endif

//-----------------------------------------------------------------------------
/** AUD_DspAVCEnable
 *  enable AVC.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  fgEnable  : on/off.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspAVCEnable(UINT8 u1DecId, BOOL fgEnable)
{
    BOOL fgAgcEnable;
    
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
    VOL_CTL_SEMA_LOCK(u1DecId);

    if (_afgUserAgcFlag[u1DecId] != fgEnable)
    {
        // Check input source, then adjust AGC
        fgAgcEnable = fgEnable && _aafgInputSrcAgcFlag[u1DecId][AUD_EU_ATV_STREAM_SRC_TRANSFER(_aeStreamSrc[u1DecId])];
        _AudDspAgcEnalbe(u1DecId, fgAgcEnable);
        _afgUserAgcFlag[u1DecId] = fgEnable;
    }
    _afgUserAgcFlag[u1DecId] = fgEnable;

    _AUD_DspVolumeChange(u1DecId, AUD_CH_ALL);
    VOL_CTL_SEMA_UNLOCK(u1DecId);
}

//-----------------------------------------------------------------------------
/** AUD_DspChangeAVCPara
 *  setup automatic volume control parameters.
 *
 *  @param  u2Type     parameter selection
 *                             0: AVC on/off (u2Value = 0: AVC off, 1: AVC on)
 *                             1: AVC target level (dB) (u2Value = -6 ~ -24)
 *                             3: Max gain up (dB) (u2Value = 0 ~ 8)
 *                             5: Adjust rate (dB/128ms)
 *                             6: Adjust rate when AVC is turned on in UI (dB/128ms)
 *  @param  u2Value    parameter value
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspChange2BandAVCPara(UINT8 u1Band, UINT16 u2Type, UINT16 u2Value, UINT8 u1DecId)
{
    UINT16 u2ShmIndex;
    UINT16 u2UopIndex = UOP_DSP_AVC_CONTROL;

    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
    VERIFY((u1Band == 0) || (u1Band == 1));

    if (u1Band == 0)
    {
    switch(u2Type)
    {
    case 0:     //En/Dis
        if (u2Value==1)
            {
            AUD_DspAVCEnable(0,TRUE);
            }
        else if (u2Value==0)
            {
            AUD_DspAVCEnable(0,FALSE);
            }
        return;
    case 1:     //Target Lvl
        u2ShmIndex = W_AVC_TARGET_LEV;
        break;
    case 2:     //Silence Lvl
        u2ShmIndex = W_AVC_SILENCE_LEV;
        break;
    case 3:     //Max Gain Up
        u2ShmIndex = W_AVC_MAX_GAIN_UP;
        break;
    case 4:     //Max Gain Down
        u2ShmIndex = W_AVC_MAX_GAIN_DOWN;
        break;
    case 5:     //Adj Rate
        u2ShmIndex = W_AVC_ADJUST_RATE;
        break;
    case 6:     //UI Adj Rate
        u2ShmIndex = W_DSP_AVC_UI_ADJUST_RATE;
        break;
    case 7:     //Attack threshold
        u2ShmIndex = W_AVC_ATTACK_THRES;
        break;
    default:
        return;
    }    
    }
    else
    {
    switch(u2Type)
    {
    case 0:     //En/Dis
        if (u2Value==1)
            {
            AUD_DspAVCEnable(AUD_DEC_AUX, TRUE);
            }
        else if (u2Value==0)
            {
            AUD_DspAVCEnable(AUD_DEC_AUX, FALSE);
            }
        return;
    case 1:     //Target Lvl
        u2ShmIndex = W_AVC_TARGET_LEV_DEC2;
        break;
    case 2:     //Silence Lvl
        u2ShmIndex = W_AVC_SILENCE_LEV_DEC2;
        break;
    case 3:     //Max Gain Up
        u2ShmIndex = W_AVC_MAX_GAIN_UP_DEC2;
        break;
    case 4:     //Max Gain Down
        u2ShmIndex = W_AVC_MAX_GAIN_DOWN_DEC2;
        break;
    case 5:     //Adj Rate
        u2ShmIndex = W_AVC_ADJUST_RATE_DEC2;
        break;
    case 6:     //UI Adj Rate
        u2ShmIndex = W_DSP_AVC_UI_ADJUST_RATE_DEC2;
        break;
    case 7:     //Attack threshold
        u2ShmIndex = W_AVC_ATTACK_THRES_DEC2;
        break;
    default:
        return;
    }    
    }

    vWriteShmUINT16(u2ShmIndex, u2Value);
    DSP_SendDspTaskCmd(u2UopIndex);

}

//-----------------------------------------------------------------------------
/** AUD_DspChangeAVCPara
 *  setup automatic volume control parameters.
 *
 *  @param  u2Type     parameter selection
 *                             0: AVC on/off (u2Value = 0: AVC off, 1: AVC on)
 *                             1: AVC target level (dB) (u2Value = -6 ~ -24)
 *                             3: Max gain up (dB) (u2Value = 0 ~ 8)
 *                             5: Adjust rate (dB/128ms)
 *                             6: Adjust rate when AVC is turned on in UI (dB/128ms)
 *  @param  u2Value    parameter value
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspChangeAVCPara(UINT16 u2Type, UINT16 u2Value, UINT8 u1DecId)
{
    UINT16 u2ShmIndex;
    UINT16 u2UopIndex = UOP_DSP_AVC_CONTROL;

    VERIFY(u1DecId == AUD_DEC_MAIN);

    switch(u2Type)
    {
    case 0:     //En/Dis
        if (u2Value==1)
            {
            AUD_DspAVCEnable(0,TRUE);
            }
        else if (u2Value==0)
            {
            AUD_DspAVCEnable(0,FALSE);
            }
        return;
    case 1:     //Target Lvl
        u2ShmIndex = W_AVC_TARGET_LEV;
        break;
    case 2:     //Silence Lvl
        u2ShmIndex = W_AVC_SILENCE_LEV;
        break;
    case 3:     //Max Gain Up
        u2ShmIndex = W_AVC_MAX_GAIN_UP;
        break;
    case 4:     //Max Gain Down
        u2ShmIndex = W_AVC_MAX_GAIN_DOWN;
        break;
    case 5:     //Adj Rate
        u2ShmIndex = W_AVC_ADJUST_RATE;
        break;
    case 6:     //UI Adj Rate
        u2ShmIndex = W_DSP_AVC_UI_ADJUST_RATE;
        break;
    case 7:     //Attack threshold
        u2ShmIndex = W_AVC_ATTACK_THRES;
        break;
    default:
        return;
    }    

    vWriteShmUINT16(u2ShmIndex, u2Value);
    DSP_SendDspTaskCmd(u2UopIndex);

}
void _AUD_DspGetAVCPara(UINT16 u2Type, UINT16* u2Value, UINT8 u1DecId)
{
    UINT16 u2ShmIndex;

    VERIFY(u1DecId == AUD_DEC_MAIN);

    switch(u2Type)
    {
    case 0:     //En/Dis
        u2ShmIndex = W_AVC_FLAG;
        break;
    case 1:     //Target Lvl
        u2ShmIndex = W_AVC_TARGET_LEV;
        break;
    case 2:     //Silence Lvl
        u2ShmIndex = W_AVC_SILENCE_LEV;
        break;
    case 3:     //Max Gain Up
        u2ShmIndex = W_AVC_MAX_GAIN_UP;
        break;
    case 4:     //Max Gain Down
        u2ShmIndex = W_AVC_MAX_GAIN_DOWN;
        break;
    case 5:     //Adj Rate
        u2ShmIndex = W_AVC_ADJUST_RATE;
        break;
    case 6:     //UI Adj Rate
        u2ShmIndex = W_DSP_AVC_UI_ADJUST_RATE;
        break;
    case 7:     //Attack threshold
        u2ShmIndex = W_AVC_ATTACK_THRES;
        break;
    default:
        return;
    }    
    
    *u2Value = u2ReadShmUINT16(u2ShmIndex);

}  
void _AUD_DspChangeAVCPara_NoReinit(UINT16 u2Type, INT16 u2Value, UINT8 u1DecId)
{
    UINT16 u2ShmIndex;
    VERIFY(u1DecId == AUD_DEC_MAIN);
    
    switch(u2Type)
    {
    case 1:     //Target Lvl
        u2ShmIndex = W_AVC_TARGET_LEV;
        break;
    case 2:     //Silence Lvl
        u2ShmIndex = W_AVC_SILENCE_LEV;
        break;
    case 3:     //Max Gain Up
        u2ShmIndex = W_AVC_MAX_GAIN_UP;
        break;
    case 4:     //Max Gain Down
        u2ShmIndex = W_AVC_MAX_GAIN_DOWN;
        break;
    case 5:     //Adj Rate
        u2ShmIndex = W_AVC_ADJUST_RATE;
        break;
    case 6:     //UI Adj Rate
        u2ShmIndex = W_DSP_AVC_UI_ADJUST_RATE;
        break;
    case 7:     //Attack threshold
        u2ShmIndex = W_AVC_ATTACK_THRES;
        break;
    default:
        return;
    }    

    vWriteShmUINT16(u2ShmIndex, u2Value);    
}
//-----------------------------------------------------------------------------
/** _AUD_DspInputChangeAVCPara
 *  setup automatic volume control parameters by input source.
 *
 *  @param  eStreamFrom
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspInputChangeAVCPara(AUD_DEC_STREAM_FROM_T eStreamFrom)
{
#ifdef CC_AUD_USE_FLASH_AQ
#ifndef CC_AUD_NEW_AQ_PARSER
    UINT8 eConvertSrc;

    eConvertSrc = _au1AQSrc_Convert[eStreamFrom];
    VERIFY( eConvertSrc < FLASH_AQ_SRC_NUM );

    if (_fgAQ_Enable[AQ_TYPE_AVC])
    {
        _AUD_DspChangeAVCPara_NoReinit(1, (UINT16)(INT8)(_au1AQ_AVC[eConvertSrc][0]), AUD_DEC_MAIN);
        _AUD_DspChangeAVCPara_NoReinit(3, _au1AQ_AVC[eConvertSrc][1], AUD_DEC_MAIN);
        _AUD_DspChangeAVCPara_NoReinit(5, _au1AQ_AVC[eConvertSrc][2], AUD_DEC_MAIN);
        _AUD_DspChangeAVCPara_NoReinit(6, _au1AQ_AVC[eConvertSrc][3], AUD_DEC_MAIN);
        DSP_SendDspTaskCmd(UOP_DSP_AOUT_REINIT);        
        return;
    }
#endif //CC_AUD_NEW_AQ_PARSER
#endif //CC_AUD_USE_FLASH_AQ

    AUD_DspChangeAVCPara(1,  _ai2AvcTargetLevel[(UINT8)eStreamFrom],  AUD_DEC_MAIN);
    AUD_DspChangeAVCPara(3,  _ai2AvcGainUp[(UINT8)eStreamFrom],  AUD_DEC_MAIN);
    AUD_DspChangeAVCPara(5, _ai2AvcAdjRate[(UINT8)eStreamFrom], AUD_DEC_MAIN);
}
    
//-----------------------------------------------------------------------------
/** AUD_DspMuteEnable
 *  audio mute control.
 *
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *  @param  fgEnable   Mute/unmute. (1: mute audio, 0: unmute audio)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspMuteEnable(UINT8 u1DecId, BOOL fgEnable)
{
#ifndef CC_MT5391_AUD_3_DECODER  //gallen 0821
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX)||(u1DecId == AUD_DEC_THIRD));
#endif    
    VOL_CTL_SEMA_LOCK(u1DecId);
 
    _afgMuteEnable[u1DecId] = fgEnable;
    _AUD_DspVolumeChange(u1DecId, AUD_CH_ALL);
	
#ifdef CC_AUD_LINEOUT_VOL_MODE
    if(_aafgLineOutVolChangeFlag)
    {
        _aafgChlMute[u1DecId][AUD_CH_BYPASS_LEFT] = fgEnable;
        _aafgChlMute[u1DecId][AUD_CH_BYPASS_RIGHT] = fgEnable;
        _AUD_DspVolumeChange(u1DecId, AUD_CH_BYPASS_LEFT);
        _AUD_DspVolumeChange(u1DecId, AUD_CH_BYPASS_RIGHT);
    }
#endif
    VOL_CTL_SEMA_UNLOCK(u1DecId);
}

//-----------------------------------------------------------------------------
/** _AUD_DspStopMuteEnable
 *  audio mute control.
 *
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *  @param  fgEnable   Mute/unmute. (1: mute audio, 0: unmute audio)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspStopMuteEnable(UINT8 u1DecId, BOOL fgEnable)
{
#ifndef CC_MT5391_AUD_3_DECODER  //gallen 0821
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX)||(u1DecId == AUD_DEC_THIRD));
#endif    
    VOL_CTL_SEMA_LOCK(u1DecId);
 
    _afgStopMuteEnable[u1DecId] = fgEnable;
    _AUD_DspVolumeChange(u1DecId, AUD_CH_ALL);

    VOL_CTL_SEMA_UNLOCK(u1DecId);
}

//-----------------------------------------------------------------------------
/** AUD_DspHdmiModeMuteEnable
 *  audio mute control in HDMI mode.
 *
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *  @param  fgEnable   Mute/unmute. (1: mute audio, 0: unmute audio)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspHdmiModeMuteEnable(UINT8 u1DecId, BOOL fgEnable)
{
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
    VOL_CTL_SEMA_LOCK(u1DecId);
    
    _afgHdmiModeMuteEnable[u1DecId] = fgEnable;
    _AUD_DspVolumeChange(u1DecId, AUD_CH_ALL);

    VOL_CTL_SEMA_UNLOCK(u1DecId);
}

//-----------------------------------------------------------------------------
/** AUD_DspHdmiModeMuteEnable
 *  audio mute control in HDMI mode.
 *
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *  @param  fgEnable   Mute/unmute. (1: mute audio, 0: unmute audio)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
 void AUD_DspHdmiChangeFormatMuteEnable(UINT8 u1DecId, BOOL fgEnable)
{
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
    VOL_CTL_SEMA_LOCK(u1DecId);
    
    _aafgChlHdmiChgFmtMute[u1DecId][AUD_CH_FRONT_LEFT] = fgEnable;
    _aafgChlHdmiChgFmtMute[u1DecId][AUD_CH_FRONT_RIGHT] = fgEnable;
    _aafgChlHdmiChgFmtMute[u1DecId][AUD_CH_BYPASS_LEFT] = fgEnable;
    _aafgChlHdmiChgFmtMute[u1DecId][AUD_CH_BYPASS_RIGHT] = fgEnable;
    _aafgChlHdmiChgFmtMute[u1DecId][AUD_CH_DMX_LEFT] = fgEnable;
    _aafgChlHdmiChgFmtMute[u1DecId][AUD_CH_DMX_RIGHT] = fgEnable;
    
    _AUD_DspVolumeChange(u1DecId, AUD_CH_FRONT_LEFT);
    _AUD_DspVolumeChange(u1DecId, AUD_CH_FRONT_RIGHT);
    _AUD_DspVolumeChange(u1DecId, AUD_CH_BYPASS_LEFT);
    _AUD_DspVolumeChange(u1DecId, AUD_CH_BYPASS_RIGHT);
    _AUD_DspVolumeChange(u1DecId, AUD_CH_DMX_LEFT);
    _AUD_DspVolumeChange(u1DecId, AUD_CH_DMX_RIGHT);

    VOL_CTL_SEMA_UNLOCK(u1DecId);
}
#ifdef CC_ENABLE_AV_SYNC
 void AUD_DspDecPlayMuteEnable(UINT8 u1DecId, BOOL fgEnable)
{
    if (fgEnable != _fgDecPlayMute)
    {
        VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
        VOL_CTL_SEMA_LOCK(u1DecId);

        _aafgChDecPlayMute[u1DecId][AUD_CH_FRONT_LEFT] = fgEnable;
        _aafgChDecPlayMute[u1DecId][AUD_CH_FRONT_RIGHT] = fgEnable;
        _aafgChDecPlayMute[u1DecId][AUD_CH_BYPASS_LEFT] = fgEnable;
        _aafgChDecPlayMute[u1DecId][AUD_CH_BYPASS_RIGHT] = fgEnable;
        _aafgChDecPlayMute[u1DecId][AUD_CH_DMX_LEFT] = fgEnable;
        _aafgChDecPlayMute[u1DecId][AUD_CH_DMX_RIGHT] = fgEnable;

        _AUD_DspVolumeChange(u1DecId, AUD_CH_FRONT_LEFT);
        _AUD_DspVolumeChange(u1DecId, AUD_CH_FRONT_RIGHT);
        _AUD_DspVolumeChange(u1DecId, AUD_CH_BYPASS_LEFT);
        _AUD_DspVolumeChange(u1DecId, AUD_CH_BYPASS_RIGHT);
        _AUD_DspVolumeChange(u1DecId, AUD_CH_DMX_LEFT);
        _AUD_DspVolumeChange(u1DecId, AUD_CH_DMX_RIGHT);

        VOL_CTL_SEMA_UNLOCK(u1DecId);
    }

    _fgDecPlayMute = fgEnable;
}
#endif
//-----------------------------------------------------------------------------
/** AUD_DspBassMngEnalbe
 *  Bass management control.
 *
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *  @param  fgEnable   Bass management enable. (1: enable, 0: disable)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspBassMngEnalbe(UINT8 u1DecId, BOOL fgEnable)
{
    UINT16 u2ShmIndex = W_PROCMOD;
    UINT16 u2UopIndex = UOP_DSP_BYPASS_PROC_MODE;
    
    if (u1DecId != AUD_DEC_MAIN)
    {
        u2ShmIndex = W_PROCMOD_DEC2;
        u2UopIndex = UOP_DSP_BYPASS_PROC_MODE_DEC2;
    }

    if (fgEnable)
    {
        vWriteShmUINT16(u2ShmIndex, u2ReadShmUINT16(u2ShmIndex) & ~((UINT16)(BYPASS_BASS_MANAGEMENT | BYPASS_POST_PROCESSING)));
    }
    else
    {
        vWriteShmUINT16(u2ShmIndex, u2ReadShmUINT16(u2ShmIndex) | (UINT16)BYPASS_BASS_MANAGEMENT);
    }   
    
    DSP_SendDspTaskCmd(u2UopIndex);
}

void _AUD_DspProcMode(UINT8 u1DecId, UINT16 u2Mode)
{
    UINT16 u2ShmIndex = W_PROCMOD;
    UINT16 u2UopIndex = UOP_DSP_BYPASS_PROC_MODE;
#ifndef CC_MT5391_AUD_3_DECODER 
//#if 1
    if (u1DecId != AUD_DEC_MAIN)
    {
        u2ShmIndex = W_PROCMOD_DEC2;
        u2UopIndex = UOP_DSP_BYPASS_PROC_MODE_DEC2;
    }
#else
    if (u1DecId == AUD_DEC_AUX)
    {
        u2ShmIndex = W_PROCMOD_DEC2;
        u2UopIndex = UOP_DSP_BYPASS_PROC_MODE_DEC2;
    }
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u2ShmIndex = W_PROCMOD_DEC3;
        u2UopIndex = UOP_DSP_BYPASS_PROC_MODE_DEC3;
    }
#endif
    vWriteShmUINT16(u2ShmIndex, u2Mode);
    DSP_SendDspTaskCmd(u2UopIndex);
}

#ifdef CC_NEW_POST_MULTI_VS_SUPPORT
void _AUD_DspVSModeChange(UINT8 u1DecId, UINT8 u1ModeIdx)
{
	UINT32 u4VSParameter;
	UINT8 eVSCfgType;

    if (u1ModeIdx > AUD_VS_NUM-1)
        u1ModeIdx = AUD_VS_NUM-1;

    if (u1DecId == AUD_DEC_MAIN)
    {
      for (eVSCfgType = 0; eVSCfgType < AUD_SUR_PRAR_NUM; eVSCfgType++)
        {
           u4VSParameter = _aai1VSMode[u1ModeIdx][eVSCfgType];
           switch(eVSCfgType)        
           {
               case AUD_SUR_CLARITY:
                   vWriteShmUINT32 (D_VSURR_CLARITY,   u4VSParameter);   //center gain, 
                   break;
               case AUD_SUR_WIDTH:
                   vWriteShmUINT32 (D_VSURR_WIDTH,   u4VSParameter);   //surround gain,
                   break;
               case AUD_SUR_LRGAIN:
                   vWriteShmUINT32 (D_VSURR_LR_GAIN,   u4VSParameter); //L/R gain,
                   break;
               case AUD_SUR_XTALK:
                   vWriteShmUINT32 (D_VSURR_CROS_TALK,   u4VSParameter); //crosstalk,
                   break;
               case AUD_SUR_OUTPUT_GAIN:
                   vWriteShmUINT32 (D_VSURR_OUTPUT_GAIN,   u4VSParameter); //output gain,   
                   break;
               case AUD_SUR_BASS_GAIN:
                   vWriteShmUINT32 (D_VSURR_BASS_GAIN,   u4VSParameter); //bass enhancement,
                   break;    
               case AUD_SUR_FO:
                   vWriteShmUINT32 (D_VSURR_FO,   u4VSParameter); //FO,
                   break;        
               default :
                   break;
           }	
            //DSP_SendDspTaskCmd (UOP_DSP_NEW_MTK_VSURR);
        }    
    }
}
#endif //CC_NEW_POST_MULTI_VS_SUPPORT
#ifdef CC_AUD_SOUND_MODE
void _AUD_DspPostSoundModeChange(UINT8 u1DecId, UINT8 u1ModeIdx)
{
	UINT8 u1PEQSet;
	UINT32 u4Frequency;
	UINT16 u2BW;
	INT16 i2Gain;

	UINT8 u1Level;

    if (u1ModeIdx > AUD_SM_NUM-1)
        u1ModeIdx = AUD_SM_NUM-1;

	
    if (u1DecId == AUD_DEC_MAIN)
    {
      for (u1PEQSet=0; u1PEQSet < PEQ_SETTING_MAX; u1PEQSet++)
      {
        u4Frequency = _aai1PeqFq[u1ModeIdx][u1PEQSet];
        u2BW = _aai1PeqBw[u1ModeIdx][u1PEQSet];
        i2Gain = _aai1PeqGain[u1ModeIdx][u1PEQSet];
        _AUD_DspSetPEQCfg(u1DecId, u1PEQSet, u4Frequency, u2BW, i2Gain);
       }

    u1Level = _aai1BassTreble[u1ModeIdx][0];
    _AUD_DspBassBoostGain(u1DecId, u1Level);
	
    u1Level = _aai1BassTreble[u1ModeIdx][1];
    _AUD_DspClearBoostGain(u1DecId, u1Level);

    }

}
#endif // CC_AUD_SOUND_MODE
void _AUD_DspPostSoundMode(UINT8 u1DecId,UINT8 u1ModeIdx)
{
	UINT8 u1ModeCase;
    AUD_DEC_STREAM_FROM_T eStreamFrom;
    
    UNUSED(u1ModeCase);
    
#ifdef CC_AUD_USE_FLASH_AQ
  #ifndef CC_AUD_NEW_AQ_PARSER
    u1ModeIdx = _au1AQSmod_Convert[u1ModeIdx];
  #endif //CC_AUD_NEW_AQ_PARSER
#endif
    
    if (u1ModeIdx > AUD_SOUND_MODE_NUM-1)
        u1ModeIdx = AUD_SOUND_MODE_NUM-1;

    AUD_DRVGetStreamFrom(u1DecId, &eStreamFrom)	;

	VERIFY(eStreamFrom < AUD_STREAM_FROM_NUM);
	
#ifdef CC_AUD_SOUND_MODE     	
    u1ModeCase = _arSoundModeCase[u1ModeIdx][eStreamFrom];
    _AUD_DspPostSoundModeChange(u1DecId, u1ModeCase);
#endif //CC_AUD_SOUND_MODE

#ifdef CC_AUD_USE_FLASH_AQ
#ifndef CC_AUD_NEW_AQ_PARSER
    _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_EQ, AQ_OP_EXE, (UINT16)u1ModeIdx);        
    _AUD_FlashAQPeriphSMChange(eStreamFrom, u1ModeIdx);
    _AUD_FlashAQPeriph_EXTPEQ(eStreamFrom, u1ModeIdx, FS_48K);
#endif //CC_AUD_NEW_AQ_PARSER
#endif //CC_AUD_USE_FLASH_AQ

}


#ifndef CC_NEW_POST_MULTI_VS_SUPPORT
//-----------------------------------------------------------------------------
/** AUD_DspVirtualSurroundEnalbe
 *  enable virtual surround.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  fgEnable  : on/off.
 *  @retval void
 */
//-----------------------------------------------------------------------------
//sunman_tshd
void _AUD_DspVirtualSurroundEnalbe(UINT8 u1DecId, BOOL fgEnable)
{

#if defined(DSP_SUPPORT_SRSWOW)
    // SRS WOW
    UINT16 u2ShmIndex = B_VSURRFLAG;
    UINT16 u2UopIndex = UOP_DSP_SRS_WOW_FLAG;
    
    if (u1DecId == AUD_DEC_MAIN)
    {
        if (fgEnable)
        {
            AUD_DspSpeakerOutputConfig(0, 0); // 2/0 LtRt // FIXME    
            vWriteShmUINT8(u2ShmIndex, (UINT8)(0x01<<2));
        }
        else
        {
            AUD_DspSpeakerOutputConfig(0, 2);
            vWriteShmUINT8(u2ShmIndex, 0);
        }
        DSP_SendDspTaskCmd(u2UopIndex);
    }

#elif defined(DSP_SUPPORT_SRSTSXT) || defined(DSP_SUPPORT_SRSTSHD)
    // SRS TSXT or SRS TSHD
    UINT16 u2ShmIndex = B_VSURRFLAG;
    UINT16 u2UopIndex = UOP_DSP_SRS_TSXT_FLAG;
    UINT8 u8Flag = uReadShmUINT8(u2ShmIndex);
    if (u1DecId == AUD_DEC_MAIN)
    {
        if (fgEnable)
        {
            //AUD_DspSpeakerOutputConfig(0, 0); // 2/0 LtRt // FIXME    
            if(u8Flag==(0x01<<5))
                return;            
            vWriteShmUINT8(u2ShmIndex, (UINT8)(0x01<<5)|(UINT8)(fgEnable));
        }
        else
        {
            //AUD_DspSpeakerOutputConfig(0, 2);
            if(u8Flag==0)
                return;
            vWriteShmUINT8(u2ShmIndex, 0);
        }
        DSP_SendDspTaskCmd(u2UopIndex);
    }
#else //DSP_SUPPORT_SRSTSXT
    // MTK Virtual Surround
    UINT16 u2ShmIndex = B_VSURRFLAG;
    UINT16 u2UopIndex = UOP_DSP_VIRTUAL_SURROUND_FLAG;
    
    if (u1DecId != AUD_DEC_MAIN)
    {
        u2ShmIndex = B_VSURRFLAG_DEC2;
	u2UopIndex = UOP_DSP_VIRTUAL_SURROUND_FLAG_DEC2;
    }

    vWriteShmUINT8(u2ShmIndex, (UINT8)fgEnable);
    DSP_SendDspTaskCmd(u2UopIndex);
#endif
}

#else // CC_NEW_POST_MULTI_VS_SUPPORT
//-----------------------------------------------------------------------------
/** AUD_DspVirtualSurroundEnalbe
 *  enable virtual surround.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1ModeIdx  : 0: Off
 *                                 1: MVS
 *                                 2: M2S
 *                                 3: ....
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspVirtualSurroundEnalbe(UINT8 u1DecId, UINT8 u1ModeIdx)
{

#ifdef DSP_SUPPORT_SRSWOW     // SRS WOW
    UINT16 u2ShmIndex = W_VSURRFLAG;
    UINT16 u2UopIndex = UOP_DSP_SRS_WOW_FLAG;
    
    if (u1DecId == AUD_DEC_MAIN)
    {
	 if(u1ModeIdx == 1 )		
        {
            AUD_DspSpeakerOutputConfig(0, 0); // 2/0 LtRt // FIXME    
            vWriteShmUINT16(u2ShmIndex, (UINT16)(0x01<<2));
        }
        else
        {
            AUD_DspSpeakerOutputConfig(0, 2);
            vWriteShmUINT16(u2ShmIndex, 0);
        }
        DSP_SendDspTaskCmd(u2UopIndex);
    }
#else //DSP_SUPPORT_SRSWOW 

#ifdef DSP_SUPPORT_SRSTSXT        // SRS TSXT
    UINT16 u2ShmIndex = W_VSURRFLAG;
    UINT16 u2UopIndex = UOP_DSP_SRS_TSXT_FLAG;
    
    if (u1DecId == AUD_DEC_MAIN)
    {
        if(u1ModeIdx == 1 )		
        {
            AUD_DspSpeakerOutputConfig(0, 0); // 2/0 LtRt // FIXME    
            vWriteShmUINT16(u2ShmIndex, (UINT16)(0x01<<5));
        }
        else
        {
            AUD_DspSpeakerOutputConfig(0, 2);
            vWriteShmUINT16(u2ShmIndex, 0);
        }
        DSP_SendDspTaskCmd(u2UopIndex);
    }
#else //DSP_SUPPORT_SRSTSXT
    // MTK Virtual Surround
    /*
    AP   SH
    0--> 0     MVS and M2S off
    1--> 1     MVS ON :Normal 
    2--> 1<<8  M2S ON
    3--> 1     MVS ON :Sports
    4--> 1     MVS ON :Music
    5--> 1     MVS ON :Cinema
    6--> 1     MVS ON :Game 
    */
    UINT16 u2ShmIndex = W_VSURRFLAG;
    UINT16 u2UopIndex = UOP_DSP_VIRTUAL_SURROUND_FLAG;
    
    if (u1DecId != AUD_DEC_MAIN)
    {
        u2ShmIndex = W_VSURRFLAG_DEC2;
        u2UopIndex = UOP_DSP_VIRTUAL_SURROUND_FLAG_DEC2;
    }

	if(u1ModeIdx == 2)	
		vWriteShmUINT16(u2ShmIndex, (UINT16)(0x01<<8));
	else if(u1ModeIdx < 2)
        vWriteShmUINT16(u2ShmIndex, u1ModeIdx);
		else
        vWriteShmUINT16(u2ShmIndex, 0x01);
		
	if ( u1ModeIdx != 2 )
	{
	    if (u1ModeIdx > 2)
		    u1ModeIdx = u1ModeIdx - 2 ;
	    else 
		    u1ModeIdx = 0;

		_AUD_DspVSModeChange(u1DecId,u1ModeIdx);
	}
    DSP_SendDspTaskCmd(u2UopIndex);
#endif //DSP_SUPPORT_SRSTSXT
#endif //DSP_SUPPORT_SRSWOW
}
#endif //CC_NEW_POST_MULTI_VS_SUPPORT

void _AUD_DspADModeSet(UINT8 u1DecId, UINT16 u2Value)
{
    UINT16 u2ShmIndex = B_ADFLAG;
    UINT32 u2UopIndex = UOP_DSP_AD_FLAG;    	
    UINT16 u4Config = 0;
	   
     if (u2Value == 0)
    {
		   u4Config |= 0x2 ;  // AD will added to CH9/10
     }	 
     if (u2Value == 1)
     {
		   u4Config |= 0x8 ; //AD will added to L/R 
      }	 
      if (u2Value == 2)
      {
		   u4Config |= 0xA ; //AD will added to CH9/10 & L/R 
       }	 
		 
	 vWriteShmUINT8(u2ShmIndex , (uReadShmUINT8(B_ADFLAG) & 0xFFF1) | u4Config);	 
	 DSP_SendDspTaskCmd(u2UopIndex);

}



//Audio Description
void _AUD_DspADEnable(UINT8 u1DecId, BOOL fgEnable)
{
    UINT16 u2ShmIndex = B_ADFLAG;
    UINT32 u2UopIndex = UOP_DSP_AD_FLAG;
    UINT8   u1ADFLAGValue;

     u1ADFLAGValue = (uReadShmUINT8(B_ADFLAG) & (0xFE)) | fgEnable;
    vWriteShmUINT8(u2ShmIndex, (UINT8)u1ADFLAGValue);
    DSP_SendDspTaskCmd(u2UopIndex);
#ifndef CC_MT5391_AUD_3_DECODER
//#if 1
    if (fgEnable)
    {
        _AUD_DspProcMode(AUD_DEC_AUX, 0x9000); //turn off bit 9:  L/R is the same as Down-Mix Output
    }
    else
    {
        _AUD_DspProcMode(AUD_DEC_AUX, 0xB000);
    }     
#else
    if (fgEnable)
    {
        _AUD_DspProcMode(AUD_DEC_THIRD, 0x9000); //turn off bit 9:  L/R is the same as Down-Mix Output
    }
    else
    {
        //_AUD_DspProcMode(AUD_DEC_THIRD, 0xB000);
         _AUD_DspProcMode(AUD_DEC_THIRD, 0x9000); // gallen, test on 20080505   
    }     

#endif
}

//-----------------------------------------------------------------------------
/** AUD_DspBbeMode
 *  Set BBE mode.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1BbeMode : 0: OFF, 1: BBE, 2: BBE ViVA4.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspBbeMode(UINT8 u1DecId, UINT8 u1BbeMode)
{
    UINT16 u2ShmIndex = B_BBE_FLAG;
    UINT32 u2UopIndex = UOP_DSP_BBE_MODE;
    
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));

    // BBE is only for main decoder
    if (u1DecId == AUD_DEC_MAIN)
    {
       _u1BbeMode = u1BbeMode;
        vWriteShmUINT8(u2ShmIndex, u1BbeMode);
        DSP_SendDspTaskCmd(u2UopIndex);    
    }
}

//-----------------------------------------------------------------------------
/** AUD_DspMonoDownMix
 *  enable mono downmix.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  fgEnable  : on/off.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspMonoDownMix(UINT8 u1DecId, BOOL fgEnable)
{
    UINT16 u2ShmIndex = B_KARAFLAG;
    UINT32 u2UopIndex = UOP_DSP_KARAOKE_FLAG; // Use karaoke UOP to do mono downmix
    
    UINT8 u1CurSetting = 0;
    
    if (u1DecId != AUD_DEC_MAIN)
    {
        u2ShmIndex = B_KARAFLAG_DEC2;
        u2UopIndex = UOP_DSP_KARAOKE_FLAG_DEC2;
    }

    u1CurSetting = uReadShmUINT8(u2ShmIndex);

    if ((BOOL)u1CurSetting == fgEnable)
    {
        return;
    }

    vWriteShmUINT8(u2ShmIndex, (UINT8)fgEnable);
    DSP_SendDspTaskCmd(u2UopIndex);
}

//-----------------------------------------------------------------------------
/** AUD_DspSetKaraokeMixRatio
 *  Set karaoke mix ratio
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u4MixRatio  : 0x0 ~ 0x800000 (for R channel)
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetKaraokeMixRatio(UINT8 u1DecId, UINT32 u4MixRatio)
{
    UINT16 u2ShmIndex = D_LRMIXRATIO;
    UINT32 u2UopIndex = UOP_DSP_LR_MIX_RATIO;

    if (u1DecId != AUD_DEC_MAIN)
    {
        u2ShmIndex = D_LRMIXRATIO_DEC2;
        u2UopIndex = UOP_DSP_LR_MIX_RATIO_DEC2;    
    }
    
    vWriteShmUINT32(u2ShmIndex, u4MixRatio);
    DSP_SendDspTaskCmd(u2UopIndex);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetMatrixOutMode
 *  setup for matrix output mode.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eMode 0: stereo (disable), 1: L mono, 2: R mono, 3: mix mono
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetMatrixOutMode(UINT8 u1DecId, AUD_MATRIX_OUT_T eMode) 
{
    UINT8 u1OldMode, u1Mode;
    UINT16 u2ShmIndex=B_MANUAL_OUTPUT_MATRIX;
    UINT16 u2UopIndex=UOP_DSP_DUAL_MONO_MODE;

    AUD_DEC_ID_VALIDATE(u1DecId);

    u1Mode = (UINT8)eMode;

    if (u1DecId != AUD_DEC_MAIN)
    {
        u2ShmIndex = B_MANUAL_OUTPUT_MATRIX_DEC2;
        u2UopIndex = UOP_DSP_DUAL_MONO_MODE_DEC2;
    }
   
    u1OldMode = uReadShmUINT8(u2ShmIndex);

    if (u1Mode != u1OldMode)
    {
        vWriteShmUINT8(u2ShmIndex,u1Mode);
        DSP_SendDspTaskCmd(u2UopIndex);
    }
}

//-----------------------------------------------------------------------------
/** AUD_DspAutoDmx
 *  Setup auto downmix mode for AC3
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  fgEnable  : 0: disable AC3 auto downmix, 1: enable AC3 auto downmix
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspAutoDmx(UINT8 u1DecId, BOOL fgEnable)
{
    UINT16 u2ShmIndex = B_AC3AUTODNMIX;
    UINT32 u2UopIndex = UOP_DSP_AC3_AC3AUTODNMIX;

    //CLI_Parser("aud.uop.spkuop.ch 0 2"); // switch speaker config to 2/0 Lo/Ro
    AUD_DspSpeakerOutputConfig(u1DecId, 2);
    
    if (u1DecId != AUD_DEC_MAIN)
    {
        u2ShmIndex = B_AC3AUTODNMIX_DEC2;
        u2UopIndex = UOP_DSP_AC3_AC3AUTODNMIX_DEC2;
    }

    if (fgEnable)
    {
        vWriteShmUINT8(u2ShmIndex, TRUE);
    }       
    else
    {
        vWriteShmUINT8(u2ShmIndex, FALSE);
    }   

    DSP_SendDspTaskCmd(u2UopIndex);
    DSP_SendDspTaskCmd(UOP_DSP_AOUT_REINIT);
}

//-----------------------------------------------------------------------------
/** AUD_DspBypassPostProcess
 *  Bypass audio post-processing.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  fgBypass  : 1: bypass post-processing.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspBypassPostProcess(UINT8 u1DecId, BOOL fgBypass)
{
    UINT16 u2Mode;
    UINT16 u2ShmIndex = W_PROCMOD;
    UINT32 u2UopIndex = UOP_DSP_BYPASS_PROC_MODE;

    u2Mode = fgBypass ? 0xfc00 : 0xc00;
#ifndef CC_MT5391_AUD_3_DECODER
//#if 1
    if (u1DecId != AUD_DEC_MAIN)
    {
        u2ShmIndex = W_PROCMOD_DEC2;
        u2UopIndex = UOP_DSP_BYPASS_PROC_MODE_DEC2;
    }
#else
    if (u1DecId == AUD_DEC_AUX)
    {
        u2ShmIndex = W_PROCMOD_DEC2;
        u2UopIndex = UOP_DSP_BYPASS_PROC_MODE_DEC2;
    } 
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u2ShmIndex = W_PROCMOD_DEC3;
        u2UopIndex = UOP_DSP_BYPASS_PROC_MODE_DEC3;
    }
#endif

    vWriteShmUINT16(u2ShmIndex, u2Mode);
    DSP_SendDspTaskCmd(u2UopIndex);
}

//-----------------------------------------------------------------------------
/** AUD_DspSoundEffectFlag
 *  Enable all audio sound effect. (EQ, Bass/Treble, Reverb, Surround, AVC)
 *
 *  @param  u1DecId  0: first decoder 1: seconder decoder.
 *  @param  fgFlag     0: disable, 1: enable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSoundEffectFlag(UINT8 u1DecId, BOOL fgFlag)
{
    UNUSED(u1DecId);
       
    AUD_DspChannelEqEnable(AUD_DEC_MAIN, fgFlag); 
    AUD_DspBassBoostEnable(AUD_DEC_MAIN, fgFlag); 
    AUD_DspClearBoostEnable(AUD_DEC_MAIN, fgFlag);
    AUD_DspReverbEnable(AUD_DEC_MAIN, fgFlag);
    AUD_DspVirtualSurroundEnalbe(AUD_DEC_MAIN, fgFlag);
    AUD_DspAVCEnable(AUD_DEC_MAIN, fgFlag);
}

//-----------------------------------------------------------------------------
/** AUD_DspIECConfig
 *  Routine handling IEC Configuration.
 *
 *  @param  eIecCfg  0: PCM; 1:RAW.
 *  @param  fgEnable  on/off.
 *  @retval  TRUE
 */
//-----------------------------------------------------------------------------
BOOL _AUD_DspIECConfig(AUD_IEC_T eIecCfg, BOOL fgEnable)
{
    VERIFY(x_sema_lock(_ahSpdifCtlSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    _aafgMWSetSpdifFlag = fgEnable;
    _aeMWIec = eIecCfg;
    _AudDspSetIec(_aeMWIec, fgEnable);
    VERIFY(x_sema_unlock(_ahSpdifCtlSema) == OSR_OK);
    
    return TRUE;
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetIEC
 *  Routine handling IEC Configuration.
 *
 *  @param  u1DecId
 *  @retval  N/A
 *
 *  Note: This function may be called twice. It can be done by more smart way 
 *  if we can get decoder format early. New control flow is needed for better performance.
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetIEC(UINT8 u1DecId)
{
    UNUSED(u1DecId);	 

        _AUD_DspIECConfig(_aeMWIec, _aafgMWSetSpdifFlag);
}

//-----------------------------------------------------------------------------
/** _AUD_DspIecChannel
 *  Routine handling IEC Configuration.
 *
 *  @param  eIecChannel  select audio channel,ex L/R.
 *  @retval  TRUE
 */
//-----------------------------------------------------------------------------
BOOL _AUD_DspIecChannel(AUD_IEC_CH_T eIecChannel)
{
    vWriteShmUINT8(B_IEC_PCMCH, (UINT8)eIecChannel);
    DSP_SendDspTaskCmd(UOP_DSP_IEC_FLAG);

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _AUD_DspIecCopyright
 *  Routine handling IEC Configuration.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Enable 0: disable 1: enable
 *  @retval  TRUE
 */
//-----------------------------------------------------------------------------
BOOL _AUD_DspIecCopyright(UINT8 u1Enable, UINT8 u1CategoryCode)
{
    if (u1Enable == TRUE)
    {
        vWriteShmUINT8(B_IEC_COPYRIGHT, IEC_COPY_NEVER);
    }
    else
    {
        vWriteShmUINT8(B_IEC_COPYRIGHT, IEC_COPY_FREELY);
    }

    vWriteShmUINT8(B_IEC_CATEGORY_CODE, u1CategoryCode);
   
    DSP_SendDspTaskCmd(UOP_DSP_IEC_FLAG);
    
    return TRUE;
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetIecCopyright
 *  Routine handling IEC Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  copyright value
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetIecCopyright(AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    return _au1IecCopyright[(UINT8)eStreamFrom];
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetIecCategoryCode
 *  Routine handling IEC Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  category code value
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetIecCategoryCode(AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    return _au1IecCategory[(UINT8)eStreamFrom];
}

void _AUD_DspMixsoundControl(UINT16 u1Mode)
{
    vWriteShmUINT16(W_MIXSOUND_FLAG,u1Mode);
    DSP_SendDspTaskCmd(UOP_DSP_MIXSOUND_FLAG);
}

//-----------------------------------------------------------------------------
/** AUD_DspSetDemodOutputMode
 *  Set MTS output mode.
 *
 *  @param  u1Output    0: Output (Mono,Mono)
 *                               1: If stereo exists, Output (L,R) otherwise (Mono,Mono)
 *                               2: If SAP exists, Output (S,S) otherwise (L,R) otherwise (Mono,Mono)
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetDemodOutputMode(UINT8 u1Output)
{
    DSP_SetDemodOutputMode(u1Output);
}

//SRS WOW
void _AUD_DspSetSRSWOW(UINT8 u1Mode)
{
    // SRS recommend LtRt
    AUD_DspSpeakerOutputConfig(0, 0); // 2/0 LtRt // FIXME
    DSP_SRSWOWSetMode(u1Mode);
}

void _AUD_DspSetSRSWOWSpkSz(UINT8 u1Mode)
{
    DSP_SRSWOWSetSpkSize(u1Mode);
}

void _AUD_DspSetSRSWOWBright(UINT8 u1Mode)
{
    DSP_SRSWOWBright(u1Mode);
}

void _AUD_DspSetSRSWOWTBLvl(UINT8 u1Mode)
{
    DSP_SRSWOWTBLvl(u1Mode);
}

void _AUD_DspSetSRSWOWWidth(UINT8 u1Mode)
{
    DSP_SRSWOWWidth(u1Mode);
}

void _AUD_DspSetSRSTSXTSpkSz(UINT8 u1Mode)
{
    DSP_SRSTSXTSetSpkSize(u1Mode);
}

void _AUD_DspSetSRSTSXTSetMode(UINT8 u1Mode)
{
    DSP_SRSTSXTSetMode(u1Mode);
}

void _AUD_DspSetSRSTSXTElev(UINT8 u1Mode)
{
    DSP_SRSTSXTElev(u1Mode);
}
void _AUD_DspSetSRSTSXTinputgain(UINT32 u4Mode)
{
    DSP_SRSTSXTinputgain(u4Mode);
}
void _AUD_DspSetSRSTSXTTBLvl(UINT8 u1Mode)
{
    DSP_SRSTSXTTBLvl(u1Mode);
}

//-----------------------------------------------------------------------------
/** AUD_DspSetDetectDefaultMode
 *   call this function before issue detection command.
 *
 *  @param  u1Mode 0: PAL_DK, 4: SECAM-L.
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetDetectDefaultMode(UINT8 u1Mode)
{
    vWriteShmUINT8(B_DETECT_DEFAULT, u1Mode);
}

//-----------------------------------------------------------------------------
/** AUD_DspSetUserDefinedEqCfg
 *   setup user difined EQ config.
 *
 *  @param  u1DecId  0:first decoder 1: seconder decoder.
 *  @param  eEqType  0: off   others: 1~12.
 *  @param  u1BandIdx  0~4.
 *  @param  i1Value -100 ~ +100
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetUserDefinedEqCfg(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType, UINT8 u1BandIdx, INT8 i1Value)
{
    INT8 i1MapValue;

    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));

    if (u1BandIdx >= CH_EQ_BAND_NO)
    {
        return;
    }
    if (eEqType >= AUD_EQ_NUM)
    {
        return;
    }

    i1MapValue = (i1Value * EQ_BAND_GAIN_SIDE_INDEX_MAX) / 100;
    
    VERIFY(x_sema_lock(_ahEqCtlSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    _aai1ChEqCfg[eEqType][u1BandIdx + 1] = i1MapValue;
    VERIFY(x_sema_unlock(_ahEqCtlSema) == OSR_OK);
    
    if (_aeEqMode[u1DecId] == eEqType)
    {
        // FIXME !!
        AUD_DspChEqLoadPreset(u1DecId, eEqType);
    }
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetUserDefinedSuitEqCfg
 *   setup user difined EQ config.
 *
 *  @param  u1DecId  0:first decoder 1: seconder decoder.
 *  @param  eEqType  0: off   others: 1~12.
 *  @param  i1Band1Value -20 ~ +20  (range -10 ~ +10 dB, 0.5 dB/step).
 *  @param  i1Band2Value -20 ~ +20  (range -10 ~ +10 dB, 0.5 dB/step).
 *  @param  i1Band3Value -20 ~ +20  (range -10 ~ +10 dB, 0.5 dB/step).
 *  @param  i1Band4Value -20 ~ +20  (range -10 ~ +10 dB, 0.5 dB/step).
 *  @param  i1Band5Value -20 ~ +20  (range -10 ~ +10 dB, 0.5 dB/step).
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetUserDefinedSuitEqCfg(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType, INT8 i1Band1Value, 
                                 INT8 i1Band2Value, INT8 i1Band3Value, INT8 i1Band4Value, 
                                 INT8 i1Band5Value)
{
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));

    if (eEqType >= AUD_EQ_NUM)
    {
        return;
    }
    VERIFY(x_sema_lock(_ahEqCtlSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    _aai1ChEqCfg[eEqType][1] = i1Band1Value;
    _aai1ChEqCfg[eEqType][2] = i1Band2Value;
    _aai1ChEqCfg[eEqType][3] = i1Band3Value;
    _aai1ChEqCfg[eEqType][4] = i1Band4Value;
    _aai1ChEqCfg[eEqType][5] = i1Band5Value;
    VERIFY(x_sema_unlock(_ahEqCtlSema) == OSR_OK);
    
    if (_aeEqMode[u1DecId] == eEqType)
    {
        // FIXME !!
        AUD_DspChEqLoadPreset(u1DecId, eEqType);
    }

}

//-----------------------------------------------------------------------------
/** _AUD_DspSetUserDefinedSuitEqCfg
 *   setup user difined EQ config.
 *
 *  @param  u1DecId  0:first decoder 1: seconder decoder.
 *  @param  eEqType  0: off   others: 1~12.
 *  @param  i1Band1Value -20 ~ +20  (range -10 ~ +10 dB, 0.5 dB/step).
 *  @param  i1Band2Value -20 ~ +20  (range -10 ~ +10 dB, 0.5 dB/step).
 *  @param  i1Band3Value -20 ~ +20  (range -10 ~ +10 dB, 0.5 dB/step).
 *  @param  i1Band4Value -20 ~ +20  (range -10 ~ +10 dB, 0.5 dB/step).
 *  @param  i1Band5Value -20 ~ +20  (range -10 ~ +10 dB, 0.5 dB/step).
 *  @param  i1Band6Value -20 ~ +20  (range -10 ~ +10 dB, 0.5 dB/step).
 *  @param  i1Band7Value -20 ~ +20  (range -10 ~ +10 dB, 0.5 dB/step).
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetUserDefinedSuitEqCfg7Bands(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType, INT8 i1Band1Value, 
                                 INT8 i1Band2Value, INT8 i1Band3Value, INT8 i1Band4Value, 
                                 INT8 i1Band5Value, INT8 i1Band6Value, INT8 i1Band7Value)
{
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));

    if (eEqType >= AUD_EQ_NUM)
    {
        return;
    }
    VERIFY(x_sema_lock(_ahEqCtlSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    _aai1ChEqCfg[eEqType][1] = i1Band1Value;
    _aai1ChEqCfg[eEqType][2] = i1Band2Value;
    _aai1ChEqCfg[eEqType][3] = i1Band3Value;
    _aai1ChEqCfg[eEqType][4] = i1Band4Value;
    _aai1ChEqCfg[eEqType][5] = i1Band5Value;

#ifdef CC_AUD_7BAND_EQUALIZER    
    _aai1ChEqCfg[eEqType][6] = i1Band6Value;
    _aai1ChEqCfg[eEqType][7] = i1Band7Value;
#endif

    VERIFY(x_sema_unlock(_ahEqCtlSema) == OSR_OK);
    
    if (_aeEqMode[u1DecId] == eEqType)
    {
        // FIXME !!
        AUD_DspChEqLoadPreset(u1DecId, eEqType);
    }
}

//-----------------------------------------------------------------------------
/** AUD_DspSetUserDefinedEqCfg
 *   Get user difined EQ config.
 *
 *  @param  eEqType  0: off   others: 1~12.
 *  @param  u1BandIdx  0~4.
 *  @param  i1Value (-20 ~ +20)
 *
 *  @retval  TRUE: success
 *              FALSE: failed
 */
//-----------------------------------------------------------------------------
BOOL _AUD_DspGetUserDefineEqCfg(AUD_EQ_TYPE_T eEqType, UINT8 u1BandIdx, INT8* pi1Value)
{
    if (u1BandIdx >= CH_EQ_BAND_NO)
    {
        return FALSE;
    }
    if (eEqType >= AUD_EQ_NUM)
    {
        return FALSE;
    }

    if (pi1Value != NULL)
    {
        VERIFY(x_sema_lock(_ahEqCtlSema, X_SEMA_OPTION_WAIT) == OSR_OK);
        *pi1Value = _aai1ChEqCfg[eEqType][u1BandIdx + 1] ;
        VERIFY(x_sema_unlock(_ahEqCtlSema) == OSR_OK);
    }   

    return TRUE;
}

//-----------------------------------------------------------------------------
/** AUD_DspGetUserEqBandNum
 *   Get user defined band number.
 *   
 *  @retval  CH_EQ_BAND_NO = 5.
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetUserEqBandNum(void)
{
    return CH_EQ_BAND_NO;
}

//-----------------------------------------------------------------------------
/** AUD_DspGetEqBandInfo
 *   Get user defined band info.
 *   
 *  @param  prEqBandInfo  structure of eq band info (AUD_EQ_BAND_INFO_T).
 *  @retval  void 
 */
//-----------------------------------------------------------------------------
void _AUD_DspGetEqBandInfo(AUD_EQ_BAND_INFO_T* prEqBandInfo)
{
    UINT8 u1BandIdx;
    if (prEqBandInfo != NULL)
    {
        u1BandIdx = prEqBandInfo->ui1_band_idx;
        if (u1BandIdx>=5) return;	//Currently 5 sets of EQ coeffs are defined.
        x_memcpy((VOID *) prEqBandInfo, (const VOID *)&_arEqInfo[u1BandIdx], sizeof(AUD_EQ_BAND_INFO_T));
    }
}

//-----------------------------------------------------------------------------
/** AUD_DspGetSrcVol
 *   get source volume.
 *   
 *  @param  u1DecId  0:first decoder 1: seconder decoder.
 *  @param  eStrmSrc      AUD_STREAM_FROM_OTHERS = 0,         // ex. Pink Noise Generator.
                                   AUD_STREAM_FROM_DIGITAL_TUNER,      // Digital tuner. 
                                   AUD_STREAM_FROM_ANALOG_TUNER,       // ex. Analog tuner. 
                                   AUD_STREAM_FROM_SPDIF,              // SPDIF-in.
                                   AUD_STREAM_FROM_LINE_IN,            // Line in.
                                   AUD_STREAM_FROM_HDMI,               // HDMI in.
                                   AUD_STREAM_FROM_MEMORY,             // Memory.
                                   AUD_STREAM_FROM_MULTI_MEDIA,             // Multi-media
 *  @retval  input source volume 
 */
//-----------------------------------------------------------------------------
INT16 _AUD_DspGetSrcVol(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStrmSrc)
{
#ifndef CC_MT5391_AUD_3_DECODER 
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD));
#endif
    if (eStrmSrc <AUD_STREAM_FROM_NUM)
    {
        return _aai2InputSrcVol[u1DecId][AUD_EU_ATV_STREAM_SRC_TRANSFER(eStrmSrc)];
    }
    else
    {
        LOG(3, "Invalid eStrmSrc in AUD_DspGetSrcVol\n");
        return 0;
    }
}
#ifdef CC_AUD_INPUT_SRC_LINEOUT_VOL
//-----------------------------------------------------------------------------
/** _AUD_DspLinoutVol
 *   set source volume.
 *   
 *  @param  u1DecId  0:first decoder 1: seconder decoder.
 *  @param  eStrmSrc      AUD_STREAM_FROM_OTHERS = 0,         // ex. Pink Noise Generator.
                                   AUD_STREAM_FROM_DIGITAL_TUNER,      // Digital tuner.
                                   AUD_STREAM_FROM_ANALOG_TUNER,       // ex. Analog tuner.
                                   AUD_STREAM_FROM_SPDIF,              // SPDIF-in.
                                   AUD_STREAM_FROM_LINE_IN,            // Line in.
                                   AUD_STREAM_FROM_HDMI,               // HDMI in.
                                   AUD_STREAM_FROM_MEMORY,             // Memory.
                                   AUD_STREAM_FROM_MULTI_MEDIA,             // Multi-media

 *  @param  i2Vol  -128 ~ 96                          
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSrcLinoutVol(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStrmSrc, INT16 i2Vol)
{
#ifndef CC_MT5391_AUD_3_DECODER 
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD));
#endif

    if (i2Vol >= (INT16)INPUT_SRC_VOL_MAX)
    {
        i2Vol = INPUT_SRC_VOL_MAX;
    }
    else if (i2Vol <= (INT16)INPUT_SRC_VOL_MIN)
    {
        i2Vol = INPUT_SRC_VOL_MIN;
    }

    if (eStrmSrc <AUD_STREAM_FROM_NUM)
    {
        VOL_CTL_SEMA_LOCK(u1DecId);
        _aai2InputSrcLineOutVol[u1DecId][AUD_EU_ATV_STREAM_SRC_TRANSFER(eStrmSrc)] = i2Vol;
        _AUD_DspVolumeChange(u1DecId, AUD_CH_BYPASS_LEFT);
        _AUD_DspVolumeChange(u1DecId, AUD_CH_BYPASS_RIGHT);
        VOL_CTL_SEMA_UNLOCK(u1DecId);
    }
    else
    {
        LOG(3, "Invalid eStrmSrc in _AUD_DspLinoutVol\n");
    }
}
#endif

//-----------------------------------------------------------------------------
/** AUD_DspSetSrcVol
 *   set source volume.
 *   
 *  @param  u1DecId  0:first decoder 1: seconder decoder.
 *  @param  eStrmSrc      AUD_STREAM_FROM_OTHERS = 0,         // ex. Pink Noise Generator.
                                   AUD_STREAM_FROM_DIGITAL_TUNER,      // Digital tuner.
                                   AUD_STREAM_FROM_ANALOG_TUNER,       // ex. Analog tuner.
                                   AUD_STREAM_FROM_SPDIF,              // SPDIF-in.
                                   AUD_STREAM_FROM_LINE_IN,            // Line in.
                                   AUD_STREAM_FROM_HDMI,               // HDMI in.
                                   AUD_STREAM_FROM_MEMORY,             // Memory.
                                   AUD_STREAM_FROM_MULTI_MEDIA,             // Multi-media

 *  @param  i2Vol  -128 ~ 96                          
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetSrcVol(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStrmSrc, INT16 i2Vol)
{
#ifndef CC_MT5391_AUD_3_DECODER 
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#else
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD));
#endif

    if (i2Vol >= (INT16)INPUT_SRC_VOL_MAX)
    {
        i2Vol = INPUT_SRC_VOL_MAX;
    }
    else if (i2Vol <= (INT16)INPUT_SRC_VOL_MIN)
    {
        i2Vol = INPUT_SRC_VOL_MIN;
    }

    if (eStrmSrc <AUD_STREAM_FROM_NUM)
    {
        VOL_CTL_SEMA_LOCK(u1DecId);
        _aai2InputSrcVol[u1DecId][AUD_EU_ATV_STREAM_SRC_TRANSFER(eStrmSrc)] = i2Vol;
        _AUD_DspVolumeChange(u1DecId, AUD_CH_INPUT_SRC);
#if 0//def CC_AUD_INPUT_SRC_LINEOUT_VOL
        _AUD_DspVolumeChange(u1DecId, AUD_CH_BYPASS_LEFT);
        _AUD_DspVolumeChange(u1DecId, AUD_CH_BYPASS_RIGHT);
#endif
        VOL_CTL_SEMA_UNLOCK(u1DecId);
    }
    else
    {
        LOG(3, "Invalid eStrmSrc in AUD_DspSetSrcVol\n");
    }
}
        
//-----------------------------------------------------------------------------
/** AUD_DspSetAvSrcVol
 *   set AV source volume.
 *   
 *  @param  eInputSrcId         AUD_INPUT_ID_COMP_VID_0 = 0,
                                          AUD_INPUT_ID_COMP_VID_1,
                                          AUD_INPUT_ID_COMP_VID_2,
                                          AUD_INPUT_ID_COMP_VID_3,
                                          AUD_INPUT_ID_S_VID_0,
                                          AUD_INPUT_ID_S_VID_1,
                                          AUD_INPUT_ID_S_VID_2,
                                          AUD_INPUT_ID_YPBPR_0,
                                          AUD_INPUT_ID_YPBPR_1,
                                          AUD_INPUT_ID_YPBPR_2,
                                          AUD_INPUT_ID_VGA_0,
                                          AUD_INPUT_ID_VGA_1,
                                          AUD_INPUT_ID_HDMI_0,
                                          AUD_INPUT_ID_HDMI_1,
                                          AUD_INPUT_ID_HDMI_2,
                                          AUD_INPUT_ID_HDMI_3,
                                          AUD_INPUT_ID_DVI_0,
                                          AUD_INPUT_ID_DVI_1,
                                          AUD_INPUT_ID_DVI_2,
                                          AUD_INPUT_ID_DVI_3,
                                          AUD_INPUT_ID_SCART_0,
                                          AUD_INPUT_ID_SCART_1,
                                          AUD_INPUT_ID_SCART_2, 
                                          AUD_INPUT_ID_SCART_3,
                                          AUD_INPUT_ID_AUXIN_0,
                                          AUD_INPUT_ID_AUXIN_1,
 *  @param  i2Vol  -256 ~ 256.                          
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetAvSrcVol(AUD_INPUT_ID_T eInputSrcId , INT16 i2Vol)
{
    if (eInputSrcId >= AUD_INPUT_ID_MAX)
    {
        return;
    }

    VOL_CTL_SEMA_LOCK(AUD_DEC_MAIN);    

    _ai2AvInputSrcVol[eInputSrcId] = i2Vol;
    _AUD_DspVolumeChange(AUD_DEC_MAIN, AUD_CH_INPUT_SRC);
    
    VOL_CTL_SEMA_UNLOCK(AUD_DEC_MAIN);      
}

AUD_INPUT_ID_T _AUD_DspGetAvInputSrcId(void)
{
    AUD_INPUT_ID_T eInputId;
    eInputId = AUD_GetAvInputSrcId();
    if (eInputId >= AUD_INPUT_ID_MAX)
    {
        LOG(0, "Incorrect AvInputSrcId!!!\n");
        ASSERT(0);
    }
    return eInputId;
}

//-----------------------------------------------------------------------------
/** AUD_DspGetAvInputSrcDvdId
 *
 *                 
 *  @retval  
 */
//-----------------------------------------------------------------------------
AUD_INPUT_ID_T _AUD_DspGetAvInputSrcDvdId(void)
{
    return (AUD_INPUT_ID_T)DRVCUST_OptGet(eAudioDvdInputChl);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetAtvTvSys
 *
 *  @param  prTvAudSys  tv audio system structure.               
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspGetAtvTvSys(MW_TV_AUD_SYS_T * prTvAudSys)
{
    if (prTvAudSys != NULL)
    {
        DSP_GetAtvTvSys(prTvAudSys);
    }   
}

//-----------------------------------------------------------------------------
/** AUD_DspGetAtvTvSystem
 *
 *  @param  void      
 *  @retval  TV_AUD_SYS_T
 */
//-----------------------------------------------------------------------------
TV_AUD_SYS_T _AUD_DspGetAtvTvSystem(void)
{
    return DSP_GetAtvTvSystem();
}

#ifdef CC_AUD_TV_SYS_FINE_TUNE_VOL
void _AUD_SetTVSysFineTuneVol(TV_AUD_SYS_T eAudSys)
{
	UINT8 u1Value;

	if ( eAudSys >= SV_AUD_SYS_NUM )
		eAudSys = SV_NONE_DETECTED;
	
	u1Value = _aau1TVFineTuneVolume[eAudSys];

	VERIFY (u1Value < sizeof(VOLUME_FINE_TUNE)/4);
		
    switch(eAudSys)
	{
    case SV_A2_BG:
    case SV_A2_DK1:
    case SV_A2_DK2:
    case SV_A2_DK3:
	   vWriteShmUINT32(D_A2_FINE_TUNE_VOLUME, ((UINT32) VOLUME_FINE_TUNE[u1Value] << 8));
       break;
    case SV_PAL_I:
    case SV_PAL_BG:
    case SV_PAL_DK:
	   vWriteShmUINT32(D_PAL_FINE_TUNE_VOLUME, ((UINT32) VOLUME_FINE_TUNE[u1Value] << 8));
       break;
    default:
       break;
	}
}
#endif
//-----------------------------------------------------------------------------
/** AUD_DspSetTvSrcMode
 *   set tv source mode.
 *   
 *  @param  eAudSys      SV_NONE_DETECTED = 0x0,  
                                   SV_MTS           = 0x1,    
                                   SV_FM_FM         = 0x2,     
                                   SV_NTSC_M        = 0x3,   
                                   SV_A2_BG         = 0x4,    
                                   SV_A2_DK1        = 0x5,     
                                   SV_A2_DK2        = 0x6,   
                                   SV_A2_DK3        = 0x7,     
                                   SV_PAL_I         = 0x8,     
                                   SV_PAL_BG        = 0x9,    
                                   SV_PAL_DK        = 0xa,     
                                   SV_SECAM_L       = 0xb,    
                                   SV_SECAM_L_PLUM  = 0xc,    
                                   SV_SECAM_BG      = 0xd,    
                                   SV_SECAM_DK      = 0xe    
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetTvSrcMode(TV_AUD_SYS_T eAudSys)
{
    DSP_SetTvSrcMode(eAudSys);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetTvSrcMode
 *   get tv source mode.
 *   
 *  @retval  eAudSys
 */
//-----------------------------------------------------------------------------
TV_AUD_SYS_T _AUD_DspGetTvSrcMode(void)
{
    return DSP_GetTvSrcMode();
}

//-----------------------------------------------------------------------------
/** AUD_DspSetAtvOutputMode
 *   set tv source mode.
 *   
 *  @param  u1DecId 0:first decoder 1: seconder decoder.
 *  @param  eSoundMode         UNKNOWN = 0,
                                           MONO,
                                           STEREO,
                                           SUB_LANG,
                                           DUAL1,
                                           DUAL2,
                                           NICAM_MONO,
                                           NICAM_STEREO,
                                           NICAM_DUAL1,
                                           NICAM_DUAL2,
                                           A2_DUAL1,
                                           A2_DUAL2,
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetAtvOutputMode(UINT8 u1DecId, AUD_SOUND_MODE_T eSoundMode)
{
	DSP_SetAtvOutputMode(u1DecId, eSoundMode);
}

//-----------------------------------------------------------------------------
/** AUD_DspHdmiSpdifOutEnable
 *   SPDIF output control in HDMI mode.
 *   
 *  @param  fgEnable    0: disable SPDIF output, 1: enable SPDIF output
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspHdmiSpdifOutEnable(BOOL fgEnable)
{
	_aafgHdmiSetSpdifFlag=fgEnable;   
	_AudDspSetIec(_aeMWIec,fgEnable);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetSoundMode
 *   SPDIF output control in HDMI mode.
 *   
 *  @param  u1DecId 0:first decoder 1: seconder decoder.
 *
 *  @retval AUD_SOUND_MODE_T
 */
//-----------------------------------------------------------------------------
AUD_SOUND_MODE_T _AUD_DspGetSoundMode(UINT8 u1DecId)
{
    return DSP_GetSoundMode(u1DecId);   
}

//-----------------------------------------------------------------------------
/** AUD_DspGetSpectrumInfo
 *   Get audio spectrum information.
 *   
 *  @param  u1DecId 0:first decoder 1: seconder decoder.
 *  @param  u1BandNum      1~15 .  
 *  @retval  u4Spectrum
 */
//-----------------------------------------------------------------------------
UINT32 _AUD_DspGetSpectrumInfo(UINT8 u1DecId, UINT8 u1BandNum)
{
    UINT32 prSpectrumAddr;
    UINT32 u4Spectrum;
    static BOOL fgEnable = FALSE;
  
    if (!fgEnable)
    {
        AUD_DspSpectrumEnable(u1DecId, TRUE);
        fgEnable = TRUE;
    }    
    
    if ((u1BandNum < 1) || (u1BandNum > 15))
    {
        return 0;
    }
    prSpectrumAddr = DSP_VIRTUAL_ADDR(DSP_GetEQSpectrumAddr(u1DecId));
            
    //Band 1 start from <ADDR_D2RC_SPECTRUM_BASE> word 2
    prSpectrumAddr += (u1BandNum<<1)*4; 
        
    if ((prSpectrumAddr&0x3) > 0) //address must be 4-alignment.
    {
        return 0;
    }
        
    u4Spectrum = *(UINT32*)prSpectrumAddr;        
        
    return  u4Spectrum;        
}

//-----------------------------------------------------------------------------
/** AUD_DspSpectrumEnable
 *   Enable audio spectrum calculation.
 *   
 *  @param  u1DecId    0:first decoder 1: seconder decoder.
 *  @param  fgEnable    0: disable calculation, 1: enable calculation.
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSpectrumEnable(UINT8 u1DecId, BOOL fgEnable)
{    
    UINT16 u2Mode;
    UINT16 u2ShmIndex = W_PROCMOD;
    UINT32 u2UopIndex = UOP_DSP_PROCESSING_MODE;
#ifndef CC_MT5391_AUD_3_DECODER
//#if 1
    if (u1DecId != AUD_DEC_MAIN)
#else
    if (u1DecId == AUD_DEC_AUX)
#endif
    {
        u2ShmIndex = W_PROCMOD_DEC2;
    }

    u2Mode = u2ReadShmUINT16(W_PROCMOD)&(0xfffe);      
    u2Mode |= (fgEnable);
        
    vWriteShmUINT16(u2ShmIndex, u2Mode);
    DSP_SendDspTaskCmd(u2UopIndex);
}

/***************************************************************************
     Function : AUD_AudioSetStcDiffBound
  Description : Set STC PTS difference bound. Bound would be (x MS * (1+uStcDiff))
    Parameter : uStcDiff: 0 ~ 255  for low, high, and worst bound.
    Return    : None
    Andrew Wen 2007/2/2
***************************************************************************/
void _AUD_AudioSetStcDiffBound(UINT8 u1DecId, UINT8 uStcDiffLo, UINT8 uStcDiffHi, UINT8 uStcDiffWs)
{         
    UINT32 u2UopIndex = UOP_DSP_SET_STC_DIFF_BOUND;
    UNUSED(u1DecId);
    
    DSP_SetStcDiffBound(uStcDiffLo, uStcDiffHi, uStcDiffWs);        
    DSP_SendDspTaskCmd(u2UopIndex);
}

/***************************************************************************
    Function : AUD_DDBannerEnable
    Description : Turn on this funtion to close some effect and post processing.
                  For DD test only.
    Parameter : 
    Return    : None
    Andrew Wen 2007/2/12
***************************************************************************/
void _AUD_DDBannerEnable(UINT8 u1DecId, BOOL fgEnable)
{
    static BOOL  fgBannerEnable = FALSE;
    /*    
    UINT16 u2ShmIndex;
    static UINT8 u1VsEnable = 0;
    static UINT8 u1AgcEnable = 0;
    static UINT32 u4DownmixMode = 0;    
    */
    
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
    if (fgBannerEnable == fgEnable)
    {
        return;
    }
    fgBannerEnable = fgEnable;
                
    AUD_DrvSetDDBanner(fgEnable);

    if (fgEnable)
    {
        //Backup original setting
        /*
        u2ShmIndex = B_VSURRFLAG;  //surr
        u1VsEnable = uReadShmUINT8(u2ShmIndex);
        u1AgcEnable = _afgUserAgcFlag[u1DecId];  //Agc
        u2ShmIndex = D_SPKCFG;     //downmix
        u4DownmixMode =(u4ReadShmUINT32(u2ShmIndex) & 0x7);
        */
        
        // enlarge stc_diff bound.  check 0x82a4 ~ 82a6
        AUD_AudioSetStcDiffBound(u1DecId,3,3,2);
        // add audio start decoding pts
        AUD_AudioPTSDelayEnable(u1DecId, 1);
        // turn off surr        check 0x1549 (should be 0)
        AUD_DspVirtualSurroundEnalbe(u1DecId, FALSE);
        // turn off agc         check 0x1428 (should be 0)
        AUD_DspAVCEnable(u1DecId, FALSE);
        // turn off ChannelEQ   check 0x1543 (should be 0)
        AUD_DspChannelEqEnable(u1DecId, FALSE);
        // disable BassBoost    check 0x1543 (should be 0)
        AUD_DspBassBoostEnable(u1DecId, FALSE); 
        // disable Treble       check 0x1543 (should be 0)
        AUD_DspClearBoostEnable(u1DecId, FALSE);
        // disable Reverb       check 0x1544 (should be 0)
        AUD_DspReverbEnable(u1DecId, FALSE);
#ifdef CC_AUD_DDBANNER        
        // downmix Lt/Rt
        AUD_DspSpeakerOutputConfig(u1DecId, (u4ReadShmUINT32(D_SPKCFG) & 0xfffffff8));
#endif
        // Large Speaker
        AUD_DspSpeakerLsConfig(u1DecId,1,1,1,1,1);
        
        // enable new demux function
        DMX_SetIgnorePESLen(TRUE);
    }
    else
    {   
        //Reset related config 
        AUD_AudioSetStcDiffBound(AUD_DEC_MAIN,0,0,0);
        AUD_AudioPTSDelayEnable(AUD_DEC_MAIN, 0);        
        /*
        AUD_DspVirtualSurroundEnalbe(u1DecId, u1VsEnable);
        AUD_DspAVCEnable(u1DecId, u1AgcEnable);
        u2ShmIndex = D_SPKCFG;
        AUD_DspSpeakerOutputConfig(u1DecId, ((u4ReadShmUINT32(u2ShmIndex) & 0xfffffff8)|u4DownmixMode) );
        */
    }    
    
}

// *********************************************************************
// Function : AUD_DspSetPalDetection
// Description : demodulation related configuration
// Parameter : 
// Return    : 
// *********************************************************************
void _AUD_DspSetPalDetection ( AUD_FACTORY_PAL_DETECTION_ITEM_T eChangedItem, 
                                                                                        UINT16 u2Value) 
{
    UINT16 u2ShmIndex = W_PAL_CORRECT_THRESHOLD;
    
    switch (eChangedItem) 
        {
        case AUD_FACTORY_PAL_CORRECT:
            u2ShmIndex = W_PAL_CORRECT_THRESHOLD;
            break;
        case AUD_FACTORY_PAL_SYNC_LOOP:
            u2ShmIndex = W_PAL_TOTAL_SYNC1_LOOP;
            break;
        case AUD_FACTORY_PAL_ERROR:
            u2ShmIndex = W_PAL_ERR_THRESHOLD;
            break;
        case AUD_FACTORY_PAL_PARITY_ERROR:
            u2ShmIndex = W_PAL_PARITY_ERR_THRESHOLD;
            break;
        case AUD_FACTORY_PAL_NUM_FRAMES:
            u2ShmIndex = W_PAL_EVERY_NUM_FRAMES;
            break;
        default:
            break;
        }
    vWriteShmUINT16(u2ShmIndex, u2Value);
    DSP_SendDspTaskCmd(UOP_DSP_DETECTION_CONFIG);
}

// *********************************************************************
// Function : AUD_DspGetPalDetection
// Description : demodulation related configuration
// Parameter : 
// Return    : 
// *********************************************************************
UINT16 _AUD_DspGetPalDetection ( AUD_FACTORY_PAL_DETECTION_ITEM_T eItem) 
{
    UINT16 u2ShmIndex = W_PAL_CORRECT_THRESHOLD;
    
    switch (eItem) 
        {
        case AUD_FACTORY_PAL_CORRECT:
            u2ShmIndex = W_PAL_CORRECT_THRESHOLD;
            break;
        case AUD_FACTORY_PAL_SYNC_LOOP:
            u2ShmIndex = W_PAL_TOTAL_SYNC1_LOOP;
            break;
        case AUD_FACTORY_PAL_ERROR:
            u2ShmIndex = W_PAL_ERR_THRESHOLD;
            break;
        case AUD_FACTORY_PAL_PARITY_ERROR:
            u2ShmIndex = W_PAL_PARITY_ERR_THRESHOLD;
            break;
        case AUD_FACTORY_PAL_NUM_FRAMES:
            u2ShmIndex = W_PAL_EVERY_NUM_FRAMES;
            break;
        default:
            break;
        }
    return u2ReadShmUINT16(u2ShmIndex);
}

// *********************************************************************
// Function : AUD_DspSetA2Detection
// Description : demodulation related configuration
// Parameter :
// Return    :
// *********************************************************************
void _AUD_DspSetA2Detection ( AUD_FACTORY_A2_DETECTION_ITEM_T eChangedItem, 
                                                                                        UINT16 u2Value) 
{
    UINT16 u2ShmIndex = W_A2_NUM_CHECK;
    
    switch (eChangedItem) 
        {
        case AUD_FACTORY_A2_NUM_CHECK:
            u2ShmIndex = W_A2_NUM_CHECK;
            break;
        case AUD_FACTORY_A2_NUM_DOUBLE:
            u2ShmIndex = W_A2_NUM_DOUBLE_CHECK;
            break;
        case AUD_FACTORY_A2_MONO_WEIGHT:
            u2ShmIndex = W_A2_MONO_WEIGHT;
            break;
        case AUD_FACTORY_A2_STEREO_WEIGHT:
            u2ShmIndex = W_A2_STEREO_WEIGHT;
            break;
        case AUD_FACTORY_A2_DUAL_WEIGHT:
            u2ShmIndex = W_A2_DUAL_WEIGHT;
            break;
        default:
            break;
        }
    vWriteShmUINT16(u2ShmIndex, u2Value);
    DSP_SendDspTaskCmd(UOP_DSP_DETECTION_CONFIG);
}

// *********************************************************************
// Function : AUD_DspSetA2Detection
// Description : demodulation related configuration
// Parameter :
// Return    :
// *********************************************************************
UINT16 _AUD_DspGetA2Detection ( AUD_FACTORY_A2_DETECTION_ITEM_T eItem)
{
    UINT16 u2ShmIndex = W_A2_NUM_CHECK;
    
    switch (eItem) 
        {
        case AUD_FACTORY_A2_NUM_CHECK:
            u2ShmIndex = W_A2_NUM_CHECK;
            break;
        case AUD_FACTORY_A2_NUM_DOUBLE:
            u2ShmIndex = W_A2_NUM_DOUBLE_CHECK;
            break;
        case AUD_FACTORY_A2_MONO_WEIGHT:
            u2ShmIndex = W_A2_MONO_WEIGHT;
            break;
        case AUD_FACTORY_A2_STEREO_WEIGHT:
            u2ShmIndex = W_A2_STEREO_WEIGHT;
            break;
        case AUD_FACTORY_A2_DUAL_WEIGHT:
            u2ShmIndex = W_A2_DUAL_WEIGHT;
            break;
        default:
            break;
        }
    return u2ReadShmUINT16(u2ShmIndex);
}

// *********************************************************************
// Function : AUD_DspSetA2Threshold
// Description : demodulation related configuration
// Parameter :
// Return    :
// *********************************************************************
void _AUD_DspSetA2Threshold ( AUD_FACTORY_A2_THRESHOLD_ITEM_T eChangedItem, 
                                                                                        UINT16 u2Value) 
{
    UINT16 u2ShmIndex = W_A2_MONO_STEREO_THRES_EU;
    UINT16 u2SrcMode = u2ReadShmUINT16(W_SOURCE_MODE);
    
    switch (eChangedItem) 
        {
        case AUD_FACTORY_A2_MONO2STEREO:
            u2ShmIndex = (u2SrcMode == SOURCE_NTSC_M)? 
                W_A2_MONO_STEREO_THRES_M : W_A2_MONO_STEREO_THRES_EU;
            break;
        case AUD_FACTORY_A2_STEREO2MONO:
            u2ShmIndex = (u2SrcMode == SOURCE_NTSC_M)? 
                W_A2_STEREO_MONO_THRES_M : W_A2_STEREO_MONO_THRES_EU;
            break;
        default:
            break;
        }
    vWriteShmUINT16(u2ShmIndex, u2Value);
    DSP_SendDspTaskCmd(UOP_DSP_DETECTION_CONFIG);
}

// *********************************************************************
// Function : AUD_DspGetA2Threshold
// Description : demodulation related configuration
// Parameter :
// Return    :
// *********************************************************************
UINT16 _AUD_DspGetA2Threshold ( AUD_FACTORY_A2_THRESHOLD_ITEM_T eChangedItem) 
{
    UINT16 u2ShmIndex = W_A2_MONO_STEREO_THRES_EU;
    UINT16 u2SrcMode = u2ReadShmUINT16(W_SOURCE_MODE);
    
    switch (eChangedItem) 
        {
        case AUD_FACTORY_A2_MONO2STEREO:
            u2ShmIndex = (u2SrcMode == SOURCE_NTSC_M)? 
                W_A2_MONO_STEREO_THRES_M : W_A2_MONO_STEREO_THRES_EU;
            break;
        case AUD_FACTORY_A2_STEREO2MONO:
            u2ShmIndex = (u2SrcMode == SOURCE_NTSC_M)? 
                W_A2_STEREO_MONO_THRES_M : W_A2_STEREO_MONO_THRES_EU;
            break;
        default:
            break;
        }
    return u2ReadShmUINT16(u2ShmIndex);
}

/******************************************************************************
* Function      : AUD_DspSetMtsDetection
* Description   : Set NPTV MTS Detection Configurations
* Parameter     : u2NumCheck, u2StereoMid, u2StereoConMid, u2Pilot3Mid,
*                 u2Pilot3ConMid, u2SapMid, u2SapConMid
* Return        : None
******************************************************************************/
void _AUD_DspSetMtsDetection ( AUD_FACTORY_MTS_DETECTION_ITEM_T eChangedItem, 
                                                                                        UINT16 u2Value) 
{
    UINT16 u2ShmIndex = W_MTS_NUM_CHECK;
    
    switch (eChangedItem) 
        {
        case AUD_FACTORY_MTS_NUM_CHECK:
            u2ShmIndex = W_MTS_NUM_CHECK;
            break;
        case AUD_FACTORY_MTS_NUM_PILOT:
            u2ShmIndex = W_MTS_NUM_PILOT;
            break;
        case AUD_FACTORY_MTS_PILOT_HIGH:
            u2ShmIndex = W_MTS_STEREO_CON_MID;
            break;
        case AUD_FACTORY_MTS_PILOT_LOW:
            u2ShmIndex = W_MTS_STEREO_MID;
            break;
        case AUD_FACTORY_MTS_NUM_SAP:
            u2ShmIndex = W_MTS_NUM_SAP;
            break;
        case AUD_FACTORY_MTS_SAP_HIGH:
            u2ShmIndex = W_MTS_SAP_CON_MID;
            break;
        case AUD_FACTORY_MTS_SAP_LOW:
            u2ShmIndex = W_MTS_SAP_MID;
            break;
        default:
            break;
        }
    vWriteShmUINT16(u2ShmIndex, u2Value);
    DSP_SendDspTaskCmd(UOP_DSP_DETECTION_CONFIG);
}

/******************************************************************************
* Function      : AUD_DspGetMtsDetection
* Description   : Set NPTV MTS Detection Configurations
* Parameter     : u2NumCheck, u2StereoMid, u2StereoConMid, u2Pilot3Mid,
*                 u2Pilot3ConMid, u2SapMid, u2SapConMid
* Return        : None
******************************************************************************/
UINT16 _AUD_DspGetMtsDetection ( AUD_FACTORY_MTS_DETECTION_ITEM_T eItem) 
{
    UINT16 u2ShmIndex = W_MTS_NUM_CHECK;
    
    switch (eItem) 
        {
        case AUD_FACTORY_MTS_NUM_CHECK:
            u2ShmIndex = W_MTS_NUM_CHECK;
            break;
        case AUD_FACTORY_MTS_NUM_PILOT:
            u2ShmIndex = W_MTS_NUM_PILOT;
            break;
        case AUD_FACTORY_MTS_PILOT_HIGH:
            u2ShmIndex = W_MTS_STEREO_CON_MID;
            break;
        case AUD_FACTORY_MTS_PILOT_LOW:
            u2ShmIndex = W_MTS_STEREO_MID;
            break;
        case AUD_FACTORY_MTS_NUM_SAP:
            u2ShmIndex = W_MTS_NUM_SAP;
            break;
        case AUD_FACTORY_MTS_SAP_HIGH:
            u2ShmIndex = W_MTS_SAP_CON_MID;
            break;
        case AUD_FACTORY_MTS_SAP_LOW:
            u2ShmIndex = W_MTS_SAP_MID;
            break;
        default:
            break;
        }
    return u2ReadShmUINT16(u2ShmIndex);
}

// *********************************************************************
// Function : AUD_DspSetFmfmDetection
// Description : demodulation related configuration
// Parameter :
// Return    :
// *********************************************************************
void _AUD_DspSetFmfmDetection ( AUD_FACTORY_FMFM_DETECTION_ITEM_T eChangedItem, 
                                                                                        UINT16 u2Value) 
{
    UINT16 u2ShmIndex = W_FMFM_NUM_CHECK;
    
    switch (eChangedItem) 
        {
        case AUD_FACTORY_FMFM_NUM_CHECK:
            u2ShmIndex = W_FMFM_NUM_CHECK;
            break;
        case AUD_FACTORY_FMFM_NUM_DOUBLE:
            u2ShmIndex = W_FMFM_NUM_DOUBLE_CHECK;
            break;
        case AUD_FACTORY_FMFM_MONO_WEIGHT:
            u2ShmIndex = W_FMFM_MONO_WEIGHT;
            break;
        case AUD_FACTORY_FMFM_STEREO_WEIGHT:
            u2ShmIndex = W_FMFM_STEREO_WEIGHT;
            break;
        case AUD_FACTORY_FMFM_DUAL_WEIGHT:
            u2ShmIndex = W_FMFM_DUAL_WEIGHT;
            break;
        case AUD_FACTORY_FMFM_DETECT_CONFID:
            u2ShmIndex = W_FMFM_DETECT_CONFID;
            break;            
        default:
            break;
        }
    vWriteShmUINT16(u2ShmIndex, u2Value);
    DSP_SendDspTaskCmd(UOP_DSP_DETECTION_CONFIG);
}

// *********************************************************************
// Function : AUD_DspGetFmfmDetection
// Description : demodulation related configuration
// Parameter :
// Return    :
// *********************************************************************
UINT16 _AUD_DspGetFmfmDetection ( AUD_FACTORY_FMFM_DETECTION_ITEM_T eItem) 
{
    UINT16 u2ShmIndex = W_FMFM_NUM_CHECK;
    
    switch (eItem) 
        {
        case AUD_FACTORY_FMFM_NUM_CHECK:
            u2ShmIndex = W_FMFM_NUM_CHECK;
            break;
        case AUD_FACTORY_FMFM_NUM_DOUBLE:
            u2ShmIndex = W_FMFM_NUM_DOUBLE_CHECK;
            break;
        case AUD_FACTORY_FMFM_MONO_WEIGHT:
            u2ShmIndex = W_FMFM_MONO_WEIGHT;
            break;
        case AUD_FACTORY_FMFM_STEREO_WEIGHT:
            u2ShmIndex = W_FMFM_STEREO_WEIGHT;
            break;
        case AUD_FACTORY_FMFM_DUAL_WEIGHT:
            u2ShmIndex = W_FMFM_DUAL_WEIGHT;
            break;
        case AUD_FACTORY_FMFM_DETECT_CONFID:
            u2ShmIndex = W_FMFM_DETECT_CONFID;
            break;            
        default:
            break;
        }
    return u2ReadShmUINT16(u2ShmIndex);
}

// *********************************************************************
// Function : AUD_DspSetHdevMode
// Description : set high deveation mode on/off
// Parameter : bit0 : PAL
//                  bit1 : A2
//                  bit2 : MTS
//                  bit3 : FMFM
// Return    :
// *********************************************************************
void _AUD_DspSetHdevMode ( AUD_ATV_DECODER_TYPE_T eDecType, BOOL fgEnable)
{
    UINT8 u1HdevTemp;  
    UINT8 u1HdevMsak=0;  
    
    switch(eDecType)
        {
            case AUD_ATV_DECODER_PAL:
                u1HdevMsak = PAL_MASK;
                break;
            case AUD_ATV_DECODER_A2:
                u1HdevMsak = A2_MASK;
                break;
            case AUD_ATV_DECODER_MTS:
#ifdef CC_AUD_MTS_OUTPUT_HDEV_GAIN_SUPPORT
            u1HdevMsak = MTS_NEW_MASK;
#else
            u1HdevMsak = MTS_MASK;
#endif
                break;
            case AUD_ATV_DECODER_FMFM:
                u1HdevMsak = FMFM_MASK;
                break;   
            default:
                break;
        }
    if (fgEnable)
        {
        u1HdevTemp = uReadShmUINT8(B_HDEV_MODE) |u1HdevMsak;
        }
    else
        {
        u1HdevTemp = uReadShmUINT8(B_HDEV_MODE) & (~u1HdevMsak);
        }
    if (u1HdevTemp !=  uReadShmUINT8(B_HDEV_MODE))
	{
		vWriteShmUINT8(B_HDEV_MODE, u1HdevTemp);
	}
}

// *********************************************************************
// Function : AUD_DspGetHdevMode
// Description : set high deveation mode on/off
// Parameter : bit0 : PAL
//                  bit1 : A2
//                  bit2 : MTS
//                  bit3 : FMFM
// Return    :
// *********************************************************************
BOOL _AUD_DspGetHdevMode (AUD_ATV_DECODER_TYPE_T eDecType)
{
    UINT8 u1HdevMsak=0;  
    switch(eDecType)
        {
            case AUD_ATV_DECODER_PAL:
                u1HdevMsak = PAL_MASK;
                break;
            case AUD_ATV_DECODER_A2:
                u1HdevMsak = A2_MASK;
                break;
            case AUD_ATV_DECODER_MTS:
#ifdef CC_AUD_MTS_OUTPUT_HDEV_GAIN_SUPPORT
                u1HdevMsak = MTS_NEW_MASK;
#else
                u1HdevMsak = MTS_MASK;
#endif
                break;
            case AUD_ATV_DECODER_FMFM:
                u1HdevMsak = FMFM_MASK;
                break;   
            default:
                break;
        }
   return !((uReadShmUINT8(B_HDEV_MODE) & (u1HdevMsak)) == 0);

}

// *********************************************************************
// Function : AUD_DspSetAmMute
// Description : set AM mute mode on/off and thresholds
// Parameter : 
// Return    :
// *********************************************************************
void _AUD_DspSetAmMute( AUD_FACTORY_PAL_AM_MUTE_ITEM_T eChangedItem, 
                                                                                        UINT8 u1Value) 
{
    UINT16 u2ShmIndex = B_AM_MUTE_MODE;
    
    switch (eChangedItem) 
        {
        case AUD_FACTORY_PAL_AM_MUTE:
            u2ShmIndex = B_AM_MUTE_MODE;
            break;
        case AUD_FACTORY_PAL_AM_MUTE_HIGH:
            u2ShmIndex = B_AM_MUTE_THRESHOLD_HIGHT;
            break;
        case AUD_FACTORY_PAL_AM_MUTE_LOW:
            u2ShmIndex = B_AM_MUTE_THRESHOLD_LOW;
            break;
        default:
            break;
        }
    vWriteShmUINT8(u2ShmIndex, u1Value);
    DSP_SendDspTaskCmd(UOP_DSP_DETECTION_CONFIG);
}

// *********************************************************************
// Function : AUD_DspGetAmMute
// Description : set AM mute mode on/off and thresholds
// Parameter : 
// Return    :
// *********************************************************************
UINT8 _AUD_DspGetAmMute( AUD_FACTORY_PAL_AM_MUTE_ITEM_T eItem) 
{
    UINT16 u2ShmIndex = B_AM_MUTE_MODE;
    
    switch (eItem) 
        {
        case AUD_FACTORY_PAL_AM_MUTE:
            u2ShmIndex = B_AM_MUTE_MODE;
            break;
        case AUD_FACTORY_PAL_AM_MUTE_HIGH:
            u2ShmIndex = B_AM_MUTE_THRESHOLD_HIGHT;
            break;
        case AUD_FACTORY_PAL_AM_MUTE_LOW:
            u2ShmIndex = B_AM_MUTE_THRESHOLD_LOW;
            break;
        default:
            break;
        }
    return uReadShmUINT8(u2ShmIndex);
}

// *********************************************************************
// Function : Aud_DspSetFmMute
// Description : set FM mute mode on/off and thresholds
// Parameter : bit0 : PAL
//                  bit1 : A2
//                  bit2 : MTS
//                  bit3 : FMFM
// Return    :
// *********************************************************************
void _AUD_DspSetFmMute ( AUD_ATV_DECODER_TYPE_T eDecType, 
                                                            AUD_FACTORY_FM_MUTE_ITEM_T eChangedItem, 
                                                                  UINT8 u1Value) 
{
    UINT16 u2ShmIndex = B_FM_MUTE_MODE;
    UINT8 u1Mask= 0;
    
    switch (eChangedItem) 
        {
        case AUD_FACTORY_FM_MUTE:
            switch (eDecType)
                {
                case AUD_ATV_DECODER_PAL:
                    u1Mask = PAL_MASK;
                    break;
                case AUD_ATV_DECODER_A2:
                    u1Mask = A2_MASK;
                    break;
                case AUD_ATV_DECODER_MTS:
                    u1Mask = MTS_MASK;
                    break;
                case AUD_ATV_DECODER_FMFM:
                    u1Mask = FMFM_MASK;
                    break;
                default:
                    break;
                }
            u2ShmIndex = B_FM_MUTE_MODE;
            if (u1Value == 0)
                {
                u1Value = uReadShmUINT8(B_FM_MUTE_MODE) & (~u1Mask);
                }
            else
                {
                u1Value = uReadShmUINT8(B_FM_MUTE_MODE) | u1Mask;
                }
            break;
        case AUD_FACTORY_FM_MUTE_HIGH:
            switch (eDecType)
                {
                case AUD_ATV_DECODER_PAL:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_HIGHT_PAL;
                    break;
                case AUD_ATV_DECODER_A2:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_HIGHT_A2;
                    break;
                case AUD_ATV_DECODER_MTS:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_HIGHT_MTS;
                    break;
                case AUD_ATV_DECODER_FMFM:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_HIGHT_EIAJ;
                    break;
                default:
                    break;
                }
            break;
        case AUD_FACTORY_FM_MUTE_LOW:
            switch (eDecType)
                {
                case AUD_ATV_DECODER_PAL:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_LOW_PAL;
                    break;
                case AUD_ATV_DECODER_A2:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_LOW_A2;
                    break;
                case AUD_ATV_DECODER_MTS:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_LOW_MTS;
                    break;
                case AUD_ATV_DECODER_FMFM:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_LOW_EIAJ;
                    break;
                default:
                    break;
                }
            break;
        default:
            break;
        }
    UNUSED(u1Mask);
    vWriteShmUINT8(u2ShmIndex, u1Value);
    DSP_SendDspTaskCmd(UOP_DSP_DETECTION_CONFIG);
}

// *********************************************************************
// Function : Aud_DspGetFmMute
// Description : set FM mute mode on/off and thresholds
// Parameter : bit0 : PAL
//                  bit1 : A2
//                  bit2 : MTS
//                  bit3 : FMFM
// Return    :
// *********************************************************************
UINT8 _AUD_DspGetFmMute ( AUD_ATV_DECODER_TYPE_T eDecType,AUD_FACTORY_FM_MUTE_ITEM_T eItem) 
{
    UINT16 u2ShmIndex = B_FM_MUTE_MODE;
    UINT8 u1Mask= 0;
    UINT8 u1Return= 0;    
    switch (eItem) 
        {
        case AUD_FACTORY_FM_MUTE:
            switch (eDecType)
                {
                case AUD_ATV_DECODER_PAL:
                    u1Mask = PAL_MASK;
                    break;
                case AUD_ATV_DECODER_A2:
                    u1Mask = A2_MASK;
                    break;
                case AUD_ATV_DECODER_MTS:
                    u1Mask = MTS_MASK;
                    break;
                case AUD_ATV_DECODER_FMFM:
                    u1Mask = FMFM_MASK;
                    break;
                default:
                    break;
                }
            u1Return = (UINT8)!((uReadShmUINT8(B_FM_MUTE_MODE) & (u1Mask)) == 0);
            break;
        case AUD_FACTORY_FM_MUTE_HIGH:
            switch (eDecType)
                {
                case AUD_ATV_DECODER_PAL:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_HIGHT_PAL;
                    break;
                case AUD_ATV_DECODER_A2:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_HIGHT_A2;
                    break;
                case AUD_ATV_DECODER_MTS:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_HIGHT_MTS;
                    break;
                case AUD_ATV_DECODER_FMFM:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_HIGHT_EIAJ;
                    break;
                default:
                    break;
                }
            u1Return = uReadShmUINT8(u2ShmIndex);
            break;
        case AUD_FACTORY_FM_MUTE_LOW:
            switch (eDecType)
                {
                case AUD_ATV_DECODER_PAL:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_LOW_PAL;
                    break;
                case AUD_ATV_DECODER_A2:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_LOW_A2;
                    break;
                case AUD_ATV_DECODER_MTS:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_LOW_MTS;
                    break;
                case AUD_ATV_DECODER_FMFM:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_LOW_EIAJ;
                    break;
                default:
                    break;
                }
            u1Return = uReadShmUINT8(u2ShmIndex);
            break;
        default:
            break;
        }
    return u1Return;
}

// *********************************************************************
// Function : AUD_DspSetCarrierShiftMode
// Description :set carrier shift mode on/off 
// Parameter : bit0 : PAL
//                  bit1 : A2
//                  bit2 : MTS
//                  bit3 : FMFM
// Return    :
// *********************************************************************
void _AUD_DspSetCarrierShiftMode ( AUD_ATV_DECODER_TYPE_T eDecType, BOOL fgEnable)
{
    UINT8 u1CarrierShift;  
    UINT8 u1Msak=0;  
    
    switch(eDecType)
        {
            case AUD_ATV_DECODER_PAL:
                u1Msak = PAL_MASK;
                break;
            case AUD_ATV_DECODER_A2:
                u1Msak = A2_MASK;
                break;
            case AUD_ATV_DECODER_MTS:
                u1Msak = MTS_MASK;
                break;
            case AUD_ATV_DECODER_FMFM:
                u1Msak = FMFM_MASK;
                break;   
            default:
                break;
        }
    if (fgEnable)
        {
        u1CarrierShift = uReadShmUINT8(B_CARRIER_SHIFT_MODE) |u1Msak;
        }
    else
        {
        u1CarrierShift = uReadShmUINT8(B_CARRIER_SHIFT_MODE) & (~u1Msak);
        }
    if (u1CarrierShift !=  uReadShmUINT8(B_CARRIER_SHIFT_MODE))
	{
		vWriteShmUINT8(B_CARRIER_SHIFT_MODE, u1CarrierShift);
	}
}

// *********************************************************************
// Function : AUD_DspGetCarrierShiftMode
// Description : set high deveation mode on/off
// Parameter : bit0 : PAL
//                  bit1 : A2
//                  bit2 : MTS
//                  bit3 : FMFM
// Return    :
// *********************************************************************
BOOL _AUD_DspGetCarrierShiftMode (AUD_ATV_DECODER_TYPE_T eDecType)
{
    UINT8 u1Msak=0;  
    switch(eDecType)
        {
            case AUD_ATV_DECODER_PAL:
                u1Msak = PAL_MASK;
                break;
            case AUD_ATV_DECODER_A2:
                u1Msak = A2_MASK;
                break;
            case AUD_ATV_DECODER_MTS:
                u1Msak = MTS_MASK;
                break;
            case AUD_ATV_DECODER_FMFM:
                u1Msak = FMFM_MASK;
                break;   
            default:
                break;
        }
   return !((uReadShmUINT8(B_CARRIER_SHIFT_MODE) & (u1Msak)) == 0);

}

// *********************************************************************
// Function : AUD_DspSetNonEUMode
// Description : set Non-EU mode on/off 
// Parameter : 
// Return    :
// *********************************************************************
void _AUD_DspSetNonEUMode (BOOL fgEnable) 
{
	if (((UINT8)fgEnable != uReadShmUINT8 (B_NON_EU_FM_MODE)))
	{
		vWriteShmUINT8 (B_NON_EU_FM_MODE, (UINT8)fgEnable);
		DSP_SendDspTaskCmd(UOP_DSP_DETECTION_CONFIG);
	}
}

// *********************************************************************
// Function : AUD_DspGetNonEUMode
// Description : set Non-EU mode on/off 
// Parameter : 
// Return    :
// *********************************************************************
BOOL _AUD_DspGetNonEUMode (void) 
{
    return (BOOL)uReadShmUINT8(B_NON_EU_FM_MODE);
}

// *********************************************************************
// Function : void AUD_DspSetFMSatuMuteMode() large
// Description : demodulation related configuration
// Parameter : bit0 : PAL
//                  bit1 : A2
//                  bit2 : MTS
//                  bit3 : FMFM
// Return    :
// *********************************************************************
void _AUD_DspSetFMSatuMuteMode ( AUD_ATV_DECODER_TYPE_T eDecType, BOOL fgEnable)
{
    UINT8 u1SatuMute;  
    UINT8 u1Msak=0;  
    
    switch(eDecType)
        {
            case AUD_ATV_DECODER_PAL:
                u1Msak = PAL_MASK;
                break;
            case AUD_ATV_DECODER_A2:
                u1Msak = A2_MASK;
                break;
            case AUD_ATV_DECODER_MTS:
                u1Msak = MTS_MASK;
                break;
            case AUD_ATV_DECODER_FMFM:
                u1Msak = FMFM_MASK;
                break;   
            default:
                break;
        }
    if (fgEnable)
        {
        u1SatuMute = uReadShmUINT8(B_FM_SATU_MUTE_MODE) |u1Msak;
        }
    else
        {
        u1SatuMute = uReadShmUINT8(B_FM_SATU_MUTE_MODE) & (~u1Msak);
        }
    if (u1SatuMute !=  uReadShmUINT8(B_FM_SATU_MUTE_MODE))
	{
		vWriteShmUINT8(B_FM_SATU_MUTE_MODE, u1SatuMute);
	}
}

// *********************************************************************
// Function : void AUD_DspGetFMSatuMuteMode() large
// Description : demodulation related configuration
// Parameter : bit0 : PAL
//                  bit1 : A2
//                  bit2 : MTS
//                  bit3 : FMFM
// Return    :
// *********************************************************************
BOOL _AUD_DspGetFMSatuMuteMode (AUD_ATV_DECODER_TYPE_T eDecType)
{
    UINT8 u1Msak=0;  
    switch(eDecType)
        {
            case AUD_ATV_DECODER_PAL:
                u1Msak = PAL_MASK;
                break;
            case AUD_ATV_DECODER_A2:
                u1Msak = A2_MASK;
                break;
            case AUD_ATV_DECODER_MTS:
                u1Msak = MTS_MASK;
                break;
            case AUD_ATV_DECODER_FMFM:
                u1Msak = FMFM_MASK;
                break;   
            default:
                break;
        }
   return !((uReadShmUINT8(B_FM_SATU_MUTE_MODE) & (u1Msak)) == 0);
}

//-----------------------------------------------------------------------------
/** AUD_DspSetPALFineVolume
 *   Set fine tune volume for PAL.
 *   
 *  @param  u1Index      0~40 .  
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetPALFineVolume (UINT8 u1Index) 
{
    DSP_SetPALFineVolume(u1Index) ;
    _u1PalFineVol = u1Index;
}

//-----------------------------------------------------------------------------
/** AUD_DspGetPALFineVolume
 *   Get fine tune volume for PAL.
 *     
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetPALFineVolume (void) 
{
    return _u1PalFineVol;
}

//-----------------------------------------------------------------------------
/** AUD_DspSetNicamFineVolume
 *   Set fine tune volume for NICAM.
 *   
 *  @param  u1Index      0~40 .  
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetNicamFineVolume (UINT8 u1Index) 
{
    DSP_SetNICAMFineVolume(u1Index);
    _u1NicamFineVol = u1Index;
}

//-----------------------------------------------------------------------------
/** AUD_DspGetNicamFineVolume
 *   Get fine tune volume for NICAM.
 *     
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetNicamFineVolume (void) 
{
    return _u1NicamFineVol;
}

//-----------------------------------------------------------------------------
/** AUD_DspSetAMFineVolume
 *   Set fine tune volume for AM.
 *   
 *  @param  u1Index      0~40 .  
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetAMFineVolume (UINT8 u1Index)
{
    DSP_SetAMFineVolume(u1Index);  
    _u1AMFineVol = u1Index;
}

//-----------------------------------------------------------------------------
/** AUD_DspGetAMFineVolume
 *   Get fine tune volume for AM.
 *     
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetAMFineVolume (void) 
{
    return _u1AMFineVol;
}

//-----------------------------------------------------------------------------
/** AUD_DspSetA2FineVolume
 *   Set fine tune volume for A2.
 *   
 *  @param  u1Index      0~40 .  
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetA2FineVolume (UINT8 u1Index) 
{
    DSP_SetA2FineVolume(u1Index) ;	
    _u1A2FineVol = u1Index;
}

//-----------------------------------------------------------------------------
/** AUD_DspGetA2FineVolume
 *   Get fine tune volume for A2.
 *     
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetA2FineVolume (void) 
{
    return _u1A2FineVol;
}

//-----------------------------------------------------------------------------
/** AUD_DspSetMtsMonoFineVolume
 *   Set fine tune volume for MTS mono.
 *   
 *  @param  u1Index      0~40 .  
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetMtsMonoFineVolume (UINT8 u1Index) 
{
    DSP_SetMTSFineVolume(u1Index) ;	
    _u1MtsFineVol = u1Index;
}

//-----------------------------------------------------------------------------
/** AUD_DspGetMtsMonoFineVolume
 *   Get fine tune volume for MTS mono.
 *     
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetMtsMonoFineVolume (void) 
{
    return _u1MtsFineVol;
}

//-----------------------------------------------------------------------------
/** AUD_DspSetSAPFineVolume
 *   Set fine tune volume for SAP.
 *   
 *  @param  u1Index      0~40 .  
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetSAPFineVolume(UINT8 u1Index)
{
    DSP_SetSAPFineVolume(u1Index);  
    _u1SapFineVol = u1Index;
}

//-----------------------------------------------------------------------------
/** AUD_DspGetSapFineVolume
 *   Get fine tune volume for SAP.
 *     
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetSapFineVolume (void) 
{
    return _u1SapFineVol;
}

//-----------------------------------------------------------------------------
/** AUD_DspSetFmfmMonoFineVolume
 *   Set fine tune volume for FMFM mono.
 *   
 *  @param  u1Index      0~40 .  
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetFmfmMonoFineVolume (UINT8 u1Index) 
{
    DSP_SetFmfmMonoFineVolume(u1Index) ;	
    _u1FmfmMonoFineVol = u1Index;
}

//-----------------------------------------------------------------------------
/** AUD_DspGetFmfmMonoFineVolume
 *   Get fine tune volume for FMFM mono.
 *     
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetFmfmMonoFineVolume (void) 
{
    return _u1FmfmMonoFineVol;
}

//-----------------------------------------------------------------------------
/** AUD_DspSetFmfmDualFineVolume
 *   Set fine tune volume for FMFM dual.
 *   
 *  @param  u1Index      0~40 .  
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetFmfmDualFineVolume(UINT8 u1Index)
{
    DSP_SetFmfmDualFineVolume(u1Index);  
    _u1FmfmDualFineVol = u1Index;
}

//-----------------------------------------------------------------------------
/** AUD_DspGetFmfmDualFineVolume
 *   Get fine tune volume for FMFM dual.
 *   
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetFmfmDualFineVolume (void) 
{
    return _u1FmfmDualFineVol;
}

// *********************************************************************
// Function : AUD_DspSetMtsPilotDetection
// Description : set MTS pilot offset detection on/off
// Parameter : 
// Return    :
// *********************************************************************
void _AUD_DspSetMtsPilotDetection (BOOL fgEnable) 
{
	if (((UINT8)fgEnable != uReadShmUINT8 (B_MTS_PILOT_OFFSET_DETECTION)))
	{
		vWriteShmUINT8 (B_MTS_PILOT_OFFSET_DETECTION, (UINT8)fgEnable);
		DSP_SendDspTaskCmd(UOP_DSP_DETECTION_CONFIG);
	}
}

// *********************************************************************
// Function : AUD_DspGetMtsPilotDetection
// Description : Get MTS pilot offset detection on/off
// Parameter : 
// Return    :
// *********************************************************************
BOOL _AUD_DspGetMtsPilotDetection (void) 
{
    return (BOOL)uReadShmUINT8(B_MTS_PILOT_OFFSET_DETECTION);
}
// *********************************************************************
// Function : _AUD_DspSetSapMute
// Description : set SAP mute thresholds
// Parameter : 
// Return    :
// *********************************************************************
void _AUD_DspSetSapMute( AUD_FACTORY_SAP_MUTE_ITEM_T eChangedItem, 
                                                                                        UINT8 u1Value) 
{
    UINT16 u2ShmIndex = B_MTS_MUTE_SAP_LOW;
    
    switch (eChangedItem) 
        {
        case AUD_FACTORY_SAP_MUTE_LOWER:
            u2ShmIndex = B_MTS_MUTE_SAP_LOW;
            break;
        case AUD_FACTORY_SAP_MUTE_HIGHER:
            u2ShmIndex = B_MTS_MUTE_SAP_HIGH;
            break;
        case AUD_FACTORY_SAP_HP_MUTE_HIGHER:
            u2ShmIndex = B_SAP_HP_MUTE_HIGH;
            break;
        case AUD_FACTORY_SAP_HP_MUTE_LOWER:
            u2ShmIndex = B_SAP_HP_MUTE_LOW;
            break;
        case AUD_FACTORY_SAP_FILTER_SEL:
            u2ShmIndex = B_SAP_FILTER_SEL;
            break;                           
        case AUD_FACTORY_SAP_RATIO:
            u2ShmIndex = B_SAP_RATIO;
            break;                           
        
        default:
            break;
        }
    vWriteShmUINT8(u2ShmIndex, u1Value);
    DSP_SendDspTaskCmd(UOP_DSP_DETECTION_CONFIG);
}

// *********************************************************************
// Function : _AUD_DspGetSapMute
// Description : set SAP mute thresholds
// Parameter : 
// Return    :
// *********************************************************************
UINT8 _AUD_DspGetSapMute( AUD_FACTORY_SAP_MUTE_ITEM_T eItem) 
{
    UINT16 u2ShmIndex = B_MTS_MUTE_SAP_LOW;
    
    switch (eItem) 
        {
        case AUD_FACTORY_SAP_MUTE_LOWER:
            u2ShmIndex = B_MTS_MUTE_SAP_LOW;
            break;
        case AUD_FACTORY_SAP_MUTE_HIGHER:
            u2ShmIndex = B_MTS_MUTE_SAP_HIGH;
            break;
        case AUD_FACTORY_SAP_HP_MUTE_HIGHER:
            u2ShmIndex = B_SAP_HP_MUTE_HIGH;
            break;
        case AUD_FACTORY_SAP_HP_MUTE_LOWER:
            u2ShmIndex = B_SAP_HP_MUTE_LOW;
            break;
        case AUD_FACTORY_SAP_FILTER_SEL:
            u2ShmIndex = B_SAP_FILTER_SEL;
            break;               
        case AUD_FACTORY_SAP_RATIO:
            u2ShmIndex = B_SAP_RATIO;
            break;               
            
        default:
            break;
        }
    return uReadShmUINT8(u2ShmIndex);
}

// *********************************************************************
// Function : _AUD_DspSetFMSatuMuteTh
// Description : Set saturation mute thresholds
// Parameter : 
// Return    :
// *********************************************************************
void _AUD_DspSetFMSatuMuteTh( AUD_FACTORY_SATU_MUTE_ITEM_T eChangedItem, 
                                                                                        UINT8 u1Value) 
{
    UINT16 u2ShmIndex = B_SATU_MUTE_THRESHOLD_HIGH;
    
    switch (eChangedItem) 
        {
        case AUD_FACTORY_SATU_MUTE_LOWER:
            u2ShmIndex = B_SATU_MUTE_THRESHOLD_LOW;
            break;
        case AUD_FACTORY_SATU_MUTE_HIGHER:
            u2ShmIndex = B_SATU_MUTE_THRESHOLD_HIGH;
            break;
        default:
            break;
        }
    vWriteShmUINT8(u2ShmIndex, u1Value);
    DSP_SendDspTaskCmd(UOP_DSP_DETECTION_CONFIG);
}

// *********************************************************************
// Function : _AUD_DspGetFMSatuMuteTh
// Description : Set saturation mute thresholds
// Parameter : 
// Return    :
// *********************************************************************
UINT8 _AUD_DspGetFMSatuMuteTh( AUD_FACTORY_SATU_MUTE_ITEM_T eItem) 
{
    UINT16 u2ShmIndex = B_SATU_MUTE_THRESHOLD_HIGH;
    
    switch (eItem) 
        {
        case AUD_FACTORY_SATU_MUTE_LOWER:
            u2ShmIndex = B_SATU_MUTE_THRESHOLD_LOW;
            break;
        case AUD_FACTORY_SATU_MUTE_HIGHER:
            u2ShmIndex = B_SATU_MUTE_THRESHOLD_HIGH;
            break;
        default:
            break;
        }
    return uReadShmUINT8(u2ShmIndex);
}

//-----------------------------------------------------------------------------
/** AUD_DspSetBbeLevel
 *   Set BBE level
 * 
 *  @param  i1Level         0 ~ -2
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetBbeLevel (INT8 i1Level)
{
    if (_i1BbeLevel != i1Level)
    {
        _i1BbeLevel = i1Level;
        DSP_SetBbeLevel((UINT8)(-i1Level));    
    }    
}

//-----------------------------------------------------------------------------
/** AUD_DspSetBbeProcess
 *   Set BBE Process boost gain (high part)
 * 
 *  @param  u1BoostDb         3*2~12*2
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetBbeProcess (UINT8 u1BoostDb)
{
    if (_u1BbeProcess != u1BoostDb)
    {
        _u1BbeProcess = u1BoostDb;
        DSP_SetBbeProcess(u1BoostDb);    
    }    
}

//-----------------------------------------------------------------------------
/** AUD_DspSetBbeLoContour
 *   Set BBE Lo Contour boost gain (low part)
 * 
 *  @param  u1BoostDb         3*2~12*2
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetBbeLoContour(UINT8 u1BoostDb)
{
    if (_u1BbeLoContour != u1BoostDb)
    {
        _u1BbeLoContour = u1BoostDb;
        DSP_SetBbeLoContour(u1BoostDb);    
    }    
}

//-----------------------------------------------------------------------------
/** AUD_DspSetBbe3dGain
 *   Set BBE ViVA 3D gain
 * 
 *  @param  u13dGain         0 ~ 101
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetBbe3dGain(UINT8 u13dGain)
{
    if (_u1Bbe3dGain != u13dGain)
    {
        _u1Bbe3dGain = u13dGain;
        DSP_SetBbe3dGain(u13dGain);    
    }    
}

//-----------------------------------------------------------------------------
/** AUD_DspGetBbeMode
 *   Get BBE mode
 * 
 *  @retval  BBE mode
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetBbeMode(void)
{
    return _u1BbeMode;
}

//-----------------------------------------------------------------------------
/** AUD_DspGetBbeLevel
 *   Get BBE level
 * 
 *  @retval  BBE level
 */
//-----------------------------------------------------------------------------
INT8 AUD_DspGetBbeLevel(void)
{
    return _i1BbeLevel;
}

//-----------------------------------------------------------------------------
/** AUD_DspGetBbeProcess
 *   Get BBE Process
 * 
 *  @retval  BBE Process
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetBbeProcess(void)
{
    return _u1BbeProcess;
}

//-----------------------------------------------------------------------------
/** AUD_DspGetBbeLoContour
 *   Get BBE Lo Contour
 * 
 *  @retval  BBE Lo Contour
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetBbeLoContour(void)
{
    return _u1BbeLoContour;
}

//-----------------------------------------------------------------------------
/** AUD_DspGetBbe3dGain
 *   Get BBE 3D gain
 * 
 *  @retval  BBE 3D gain
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetBbe3dGain(void)
{
    return _u1Bbe3dGain;
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetVolTable
 *   Set volume curve
 * 
 *  @retval  void
 */
//-----------------------------------------------------------------------------
#ifdef CC_SET_VOLUME
void _AUD_DspSetVolTable(UINT8 u1Idx, UINT32 u4Vol)
{
	if((u1Idx <= 100)&&(u4Vol <=0x20000))
		_au4VOLUME[u1Idx]  = u4Vol;
}
#endif

#ifdef CC_AUD_NEW_CV_TABLE
void _AUD_DspSetCVTable(UINT8 u1Idx,UINT32 u4Vol)
{
	if((u1Idx <= 100)&&(u4Vol <=0x20000))
		_au4CHVOLUME[u1Idx]  = u4Vol;
}
UINT32 _AUD_DspGetCVTable(UINT8 u1VolTabIdx)
{
    return _au4CHVOLUME[u1VolTabIdx];
}
#endif

//-----------------------------------------------------------------------------
/** _AUD_DspSetVolumeTable
 *   Modify volume table according to user setting.
 *   
 *  @param  u4VolTabChkPnt
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetVolumeTable(UINT32* u4VolTabChkPnt)
{
    //Andrew Wen 07/8/20 
    UINT8 i;
    UINT8 j;
    UINT8 u1Step;    
    UINT32 u2VolStep;

    for (i=0; i<VOL_TABLE_CHK_PNT; i++)
    {
        LOG(7,"u4VolTabChkPnt[%d] = %x\n", i, u4VolTabChkPnt[i]);
        _au4VOLUME[_au1VolStep[i]] = u4VolTabChkPnt[i];
        if (i < (VOL_TABLE_CHK_PNT-1))
        {
            u1Step = (_au1VolStep[i+1] - _au1VolStep[i]);
            u2VolStep = u4VolTabChkPnt[i+1] - u4VolTabChkPnt[i];
            if (u2VolStep >  0)
            {
                u2VolStep = u2VolStep / u1Step;
            }

            for (j = (_au1VolStep[i]+1) ; j < _au1VolStep[i+1] ; j++)
            {
                if (j>MAX_VOL_LEVEL) 
                {
                    LOG(0, "Inocrrect table index when setting vlume table\n");
                    break;
                }
                _au4VOLUME[j] = _au4VOLUME[j-1] + u2VolStep;
            }
        }
    }
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetVolumeTable
 *   Query volume table.
 *   
 *  @param  u1VolTabIdx
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
UINT32 _AUD_DspGetVolumeTable(UINT8 u1VolTabIdx)
{
    return _au4VOLUME[u1VolTabIdx];
}


void _AUD_DspSetPCMChannelNumber(UINT8 u1DecId, UINT16 u2ChannelNo)
{
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
    if(u1DecId == AUD_DEC_MAIN)
    {
        vWriteShmUINT16(W_PCM_INPUT_CHANNEL_NUM, u2ChannelNo);
    }
    else
    {
        vWriteShmUINT16(W_PCM_INPUT_CHANNEL_NUM_DEC2, u2ChannelNo);
    }
}


//-----------------------------------------------------------------------------
/** _AUD_DspGetVolumeBaseLevel
 *   Get volume base level (0dB level).
 *   
 *  @retval  void
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetVolumeBaseLevel(void)
{
    return _u1VolBaseLevel;
}

//-----------------------------------------------------------------------------
/** _AUD_Surround_Get_Config
 *   Get new mtk vsurr config
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_Surround_Get_Config(AUD_VSURR_CFG_T* prVsurrCfg)
{
    DSP_Get_VSurr_Cfg(prVsurrCfg);
}

//-----------------------------------------------------------------------------
/** _AUD_Surround_Set_Config
 *   Set new mtk vsurr config
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_Surround_Set_Config(AUD_VSURR_CFG_TYPE_T eVSCfgType, UINT32 u4VSParameter)
{
    DSP_Set_VSurr_Cfg(eVSCfgType,u4VSParameter);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetPcmPreBufferLength
 *   Set PCM pre-buffering length
 *   
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetPcmPreBufferLength(UINT8 u1DecId, UINT16 u2Length)
{
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));

    if (u1DecId == AUD_DEC_MAIN)
    {
        vWriteShmUINT16(W_PCM_PREBUF_DELAY_BANK, u2Length);
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        vWriteShmUINT16(W_PCM_PREBUF_DELAY_BANK_DEC2, u2Length);
    }
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetPcmPreBufferLength
 *   Get PCM pre-buffering length
 *   
 *  @retval  void
 */
//-----------------------------------------------------------------------------
UINT16 _AUD_DspGetPcmPreBufferLength(UINT8 u1DecId)
{
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));

    return u2ReadShmUINT16((u1DecId == AUD_DEC_MAIN) ? 
                             W_PCM_PREBUF_DELAY_BANK : W_PCM_PREBUF_DELAY_BANK_DEC2);
    
}

//-----------------------------------------------------------------------------
/** _AUD_GetNicamStatus
 *   Get Nicam exists or not
 *   
 *  @retval  TURE/FALSE
 */
//-----------------------------------------------------------------------------
BOOL _AUD_GetNicamStatus(void)
{
    AUD_TYPE_T      _eAudType;
    _eAudType = DSP_GetAudChlInfo(AUD_DEC_AUX);
    if ((_eAudType == AUD_TYPE_FM_MONO_NICAM_MONO)
    	||(_eAudType == AUD_TYPE_FM_MONO_NICAM_STEREO)
    	||(_eAudType == AUD_TYPE_FM_MONO_NICAM_DUAL))
    {
         LOG(0,"NICAM exists\n");
        return TRUE;
    }
    else
    {
        LOG(0,"NICAM doesn't exist\n");	
        return FALSE;
    }
}

#ifdef SUPPORT_PHILIPS_LOUDNESS
AUD_TYPE_T _AUD_GetAudInfo(UINT8 u1DecId) 
{
    return DSP_GetAudChlInfo(u1DecId);
}
#endif

//-----------------------------------------------------------------------------
/** _AUD_GetDtvAudInfo
 *   Get Nicam exists or not
 *   
 *  @retval  TURE/FALSE
 */
//-----------------------------------------------------------------------------

void _AUD_GetDtvAudInfo(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo) 
{
    DSP_GetDtvAudInfo( u1DecId, prAudInfo); 
}

//-----------------------------------------------------------------------------
/** AUD_DspSpeed
 *  Set dsp speed (non-fractional)
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Speed  : 0:normal play
 *                            : 2:FF 2-times
 *                            : 4:FF 4-times
 *                            : 8:FF 8-times
 *                            : 16:FF 16-times
 *                            : 32:FF 32-times
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSpeed(UINT8 u1DecId, UINT16 u1Speed)
{
    UINT16 u2ShmIndex = W_SPEED;
    UINT32 u2UopIndex = UOP_DSP_SPEED;

    if (u1DecId != AUD_DEC_MAIN)
    {
        u2ShmIndex = W_SPEED_DEC2;
        u2UopIndex = UOP_DSP_SPEED_DEC2;
    }

    vWriteShmUINT16(u2ShmIndex, u1Speed);
    DSP_SendDspTaskCmd(u2UopIndex);
}

//-----------------------------------------------------------------------------
/** AUD_DspDownmixPosition
 *  Set Downmix Channel (CH9/10) Position
 *
 *  @param  u1DecId 0       : first decoder 1: seconder decoder.
 *  @param  u1DMPosition   : 0: Downmix channel(CH9/10) do ALL post processing
 *                                   : 1: doing downmix after input source gain and before AVC
 *                                   : 2: Reserved
 *                                   : 3: Downmix channel(CH9/10) do not do post processing

 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspDownmixPosition(UINT8 u1DecId, UINT16 u1DMPosition)
{
    UINT32 u2UopIndex = UOP_DSP_DOWNMIX_POSITION;
    UINT16 u4Config = 0;
    if (u1DecId == AUD_DEC_MAIN)
    {
      if (u1DMPosition == 0)
      {
        u4Config |= 0x0 << 4;  // Downmix channel(CH9/10) do ALL post processing
      }   
      if (u1DMPosition == 1)
      {
        u4Config |= 0x1 << 4; //doing downmix after input source gain and before AVC 
      }   
      if (u1DMPosition == 2)
      {
        u4Config |= 0x2 << 4; 
      }   
      if (u1DMPosition == 3)
      {
        u4Config |= 0x3 << 4; // Downmix channel(CH9/10) do not do post processing 
      }   
      vWriteShmUINT16(W_SYSTEM_SETUP, (u2ReadShmUINT16(W_SYSTEM_SETUP) & 0xFFCF) | u4Config);
      DSP_SendDspTaskCmd(u2UopIndex);
    }
}

//-----------------------------------------------------------------------------
/** AUD_DspMultiPairOutput
 *  Copy CH9/10 to other channel(LS/RS/C/SUB)
 *
 *  @param  u1DecId 0       : first decoder 1: seconder decoder.
 *  @param  u1MultiPair   : 0: OFF
 *                                   : 1: ON

 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspMultiPairOutput(UINT8 u1DecId, UINT16 u1MultiPair)
{
     UINT32 u2UopIndex = UOP_DSP_DOWNMIX_POSITION;
    UINT16 u4Config = 0;
    if (u1DecId == AUD_DEC_MAIN)
    {
      if (u1MultiPair == 0)
      {
        u4Config |= 0x0 << 3;  // Turn off bit 3
        vWriteShmUINT16(W_SYSTEM_SETUP, (u2ReadShmUINT16(W_SYSTEM_SETUP) & 0xFFF7) | u4Config);
        DSP_SendDspTaskCmd(u2UopIndex);    	  
        _AUD_DspSpeakerOutputConfig(AUD_DEC_MAIN,0x2); //set speaker cfg to be 3/2  channel  

      }   
      if (u1MultiPair == 1)
      {
        u4Config |= 0x1 << 3; //Turn on bit3
        vWriteShmUINT16(W_SYSTEM_SETUP, (u2ReadShmUINT16(W_SYSTEM_SETUP) & 0xFFF7) | u4Config);
        DSP_SendDspTaskCmd(u2UopIndex);    	  
         vWriteShmUINT32(D_SPKCFG, (u4ReadShmUINT32(D_SPKCFG) & 0xffffffdf) | ((UINT32)0x1<<5) );//turn on subwoofer channel   
        _AUD_DspSpeakerOutputConfig(AUD_DEC_MAIN,0x7); //set speaker cfg to be 2/0  channel  
      }   	  	
    }
}

//-----------------------------------------------------------------------------
/** _AUD_DspLRDownmix
 *  Downmix to L/R channel after Reverbation and before BassManagement
 *
 *  @param  u1DecId 0       : first decoder 1: seconder decoder.
 *  @param  u1MultiPair   : 0: OFF
 *                                   : 1: ON

 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspLRDownmix(UINT8 u1DecId, BOOL fgEnable)
{
     UINT32 u2UopIndex = UOP_DSP_DOWNMIX_POSITION;
    UINT16 u4Config = 0;
    if (u1DecId == AUD_DEC_MAIN)
    {
      if (fgEnable)
      {
        u4Config |= 0x1 << 6;  // Turn on bit 6

      }   
      else
      {
        u4Config |= 0x0 << 6; //Turn off bit6
      }   	 
	 vWriteShmUINT16(W_SYSTEM_SETUP, (u2ReadShmUINT16(W_SYSTEM_SETUP) & 0xFFBF) | u4Config);
        DSP_SendDspTaskCmd(u2UopIndex);    	  
  
    }
}



#ifdef DATA_DISC_WMA_SUPPORT
//-----------------------------------------------------------------------------
/** AUD_DspWMAHeader
 *  Set dsp WMA header
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder. 
 *  @param  prWmaInfo Wma header information
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspWMAHeader(UINT8 u1DecId,AUD_WMA_CFG_T* prWmaInfo)
{
    if (u1DecId != AUD_DEC_MAIN)
    {
        return; //currently we only support WMA in dec1
    }
    if (prWmaInfo->u2HdrSrc == 0){
//    	vWriteShmUINT16(W_WMA_PACKET_NO, 0);
    	vWriteShmUINT16(W_WMA_HEADER_SOURCE, 0);
    }else{
//        vWriteShmUINT16(W_WMA_PACKET_NO, 0);
        vWriteShmUINT16(W_WMA_HEADER_SOURCE, 1);
        vWriteShmUINT32(D_WMA_PACKET_COUNT, 0x7fff);
        vWriteShmUINT32(D_WMA_PACKET_SIZE, prWmaInfo->u4Pktsz);
        vWriteShmUINT16(W_WMA_VERSION, 0x161);
        vWriteShmUINT16(W_WMA_NUM_CH, prWmaInfo->u2Numch); 
        vWriteShmUINT32(D_WMA_SAMPLE_PER_SEC, prWmaInfo->u4Sampersec);
        vWriteShmUINT32(D_WMA_BYTE_PER_SEC, prWmaInfo->u4Bpersec);
        vWriteShmUINT32(D_WMA_BLOCK_SIZE, prWmaInfo->u4Blocksz);
        vWriteShmUINT16(W_WMA_ENCODER_OPTION, prWmaInfo->u2Encoderopt);        
    }
}
void AUD_DrvSetWMAErrFunc(UINT8 u1DecId, AUD_WMAErr_NOTIFY pfWMAErrNotify){
	UNUSED(u1DecId);
    UNUSED(pfWMAErrNotify);
    _hWMAErrNotifyFunc =  pfWMAErrNotify;  
}
#endif //DATA_DISC_WMA_SUPPORT

void AUD_SetLpcmTable(UINT8 u1DecId, AUD_LPCM_SETTING_T rLpcmSetting)
{
    UNUSED(u1DecId) ;
    
    //decoder 2, decoder 3 ... todo...
    vWriteShmUINT8(B_LPCM_CH_ASSIGNMENT, rLpcmSetting.u1LpcmChAssignment);
    vWriteShmUINT16(W_LPCM_FREQUENCY, (((UINT16)rLpcmSetting.u1LpcmFreqGrp1) << 8) + rLpcmSetting.u1LpcmFreqGrp2);
    vWriteShmUINT8(B_LPCM_BIT_SHIFT, rLpcmSetting.u1LpcmBitShift);
    vWriteShmUINT8(B_LPCM_DRC_VALUE, rLpcmSetting.u1LpcmDrcValue);
    vWriteShmUINT16(W_LPCM_Q_VALUE, (rLpcmSetting.u1LpcmBitResGrp1 << 12)+rLpcmSetting.u1LpcmBitResGrp2);
    vWriteShmUINT8(B_LPCM_DRC_FLAG, rLpcmSetting.u1LpcmDrcFlag);
    vWriteShmUINT16(W_LPCM_DECODING_TYPE, rLpcmSetting.u1LpcmDecodingType);

    // ADPCM
    if (rLpcmSetting.u1ADPCMDecodingType!=0)
    {
        vWriteShmUINT8(B_ADPCM_CHANNEL_NUM, rLpcmSetting.u1LpcmChAssignment+1);
        vWriteShmUINT8(B_ADPCM_BIT_PERSAMPLE, rLpcmSetting.u1LpcmBitResGrp1);
        vWriteShmUINT16(W_ADPCM_DECODING_TYPE, rLpcmSetting.u1ADPCMDecodingType);        
        vWriteShmUINT16(W_ADPCM_BLOCK_ALIGN, rLpcmSetting.u2ADPCMBlockAlign);
    }
    else
    {
        vWriteShmUINT8(B_ADPCM_CHANNEL_NUM, 0);
        vWriteShmUINT8(B_ADPCM_BIT_PERSAMPLE, 0);
        vWriteShmUINT16(W_ADPCM_DECODING_TYPE, 0);
        vWriteShmUINT16(W_ADPCM_BLOCK_ALIGN, 0);
    }
}        

//-----------------------------------------------------------------------------
/** _AUD_DspSetDualDecMode
 *   
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetDualDecMode(BOOL fgEnable)
{
    DSP_SetDualDecMode(fgEnable);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetHPFEnable
 *   
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetHPFEnable(BOOL fgEnable)
{
    UINT16  u2ShmIndex = B_HPF_ENABLE;
    UINT16 u2UopIndex = UOP_DSP_HPF_ENABLE;

    vWriteShmUINT8(u2ShmIndex, (UINT8)fgEnable);
    DSP_SendDspTaskCmd(u2UopIndex);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetHPFFc
 *   
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetHPFFc(UINT16 u2Fc)
{
    UINT16  u2ShmIndex = W_HPF_FC;
    UINT16 u2UopIndex = UOP_DSP_HPF_FC;

    vWriteShmUINT16(u2ShmIndex, u2Fc);
    DSP_SendDspTaskCmd(u2UopIndex);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetClipperEnable
 *   
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetClipperEnable(BOOL fgEnable)
{
    UINT16  u2ShmIndex = B_CLIPPER_ENABLE;
    UINT16 u2UopIndex = UOP_DSP_CLIPPER_ENABLE;

    vWriteShmUINT8(u2ShmIndex, (UINT8)fgEnable);
    DSP_SendDspTaskCmd(u2UopIndex);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetClipperValue
 *   
 *  @param  value
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetClipperValue(AUD_CH_T eChannel, UINT32 u4Value)
{
    UINT16  u2ShmIndex;
    UINT16 u2UopIndex = UOP_DSP_CLIPPER_VAL;

    switch(eChannel)
    {
    case AUD_CH_FRONT_LEFT:
        u2ShmIndex = D_CLIPPER_L_VALUE;
        break;
    case AUD_CH_FRONT_RIGHT:
        u2ShmIndex = D_CLIPPER_R_VALUE;
        break;
    case AUD_CH_REAR_LEFT:
        u2ShmIndex = D_CLIPPER_LS_VALUE;
        break;
    case AUD_CH_REAR_RIGHT:
        u2ShmIndex = D_CLIPPER_RS_VALUE;
        break;
    case AUD_CH_CENTER:
        u2ShmIndex = D_CLIPPER_C_VALUE;
        break;
    case AUD_CH_SUB_WOOFER:
        u2ShmIndex = D_CLIPPER_SW_VALUE;
        break;
    case AUD_CH_BYPASS_LEFT:
        u2ShmIndex = D_CLIPPER_CH7_VALUE;
        break;
    case AUD_CH_BYPASS_RIGHT:
        u2ShmIndex = D_CLIPPER_CH8_VALUE;
        break;
    case AUD_CH_DMX_LEFT:
        u2ShmIndex = D_CLIPPER_CH9_VALUE;
        break;
    case AUD_CH_DMX_RIGHT:
        u2ShmIndex = D_CLIPPER_CH10_VALUE;
        break;
    default:
        u2ShmIndex = D_CLIPPER_L_VALUE;
    } 

    vWriteShmUINT32(u2ShmIndex, u4Value);
    DSP_SendDspTaskCmd(u2UopIndex);
}

#ifdef CC_AUD_SPEAKER_HEIGHT
#ifndef CC_AUD_SPEAKER_HEIGHT_MODE
//-----------------------------------------------------------------------------
/** _AUD_DspSetSPHEnable
 *   
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetSPHEnable(BOOL fgEnable)
{
    UINT16  u2ShmIndex = B_SPH_ENABLE;
    UINT16 u2UopIndex = UOP_DSP_SPH_ENABLE;

    vWriteShmUINT8(u2ShmIndex, (UINT8)fgEnable);
    DSP_SendDspTaskCmd(u2UopIndex);
}
#else
//-----------------------------------------------------------------------------
/** _AUD_DspSetSPHMode
 *   
 *  @param  0:off 1:2DB 2:4DB 3:6DB
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetSPHMode(UINT8 u1Mode)
{
    UINT16  u2ShmIndex = B_SPH_MODE;
    UINT16 u2UopIndex = UOP_DSP_SPH_ENABLE;
	UINT8 u1SphCurMode;

	u1SphCurMode = uReadShmUINT8(B_SPH_MODE);
	vWriteShmUINT8(u2ShmIndex, (UINT8)u1Mode);

	if ((u1Mode > 0) && (u1SphCurMode > 0))
		u2UopIndex = UOP_DSP_SPH_CHANGE_MODE;
	DSP_SendDspTaskCmd(u2UopIndex);
}
#endif
#endif

//-----------------------------------------------------------------------------
/** _AUD_DspSetPEQEnable
 *   
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetPEQEnable(BOOL fgEnable)
{
    UINT16  u2ShmIndex = B_PEQ_ENABLE;
    UINT16 u2UopIndex = UOP_DSP_PEQ_ENABLE;

    vWriteShmUINT8(u2ShmIndex, (UINT8)fgEnable);
    DSP_SendDspTaskCmd(u2UopIndex);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetPEQCfg2
 *   
 *  @param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param u1Set                      PEQ set (0 ~ 7)
 *  @param u4Frequency                Frequency (20 ~ 20K)
 *  @param eQValue                    Q
 *  @param i2Gain                     Gain (+-12dB) with 0.5dB/step
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetPEQCfg2(UINT8 u1DecId, UINT8 u1PEQSet, UINT32 u4Frequency,
                            AUD_PEQ_Q_TYPE_T eQValue, INT16 i2Gain)
{
    UINT16  u2ShmAddrPEQFc;
    UINT16  u2ShmAddrPEQGain;
    UINT16  u2ShmAddrPEQBW;
    UINT16  u2UopIndex = UOP_DSP_PEQ_CONFIG;

    UINT16  u2PEQFc;
    UINT32  u4PEQGain;
    UINT16  u2PEQBW;
    
    UNUSED(u1DecId);

    // Check u1PEQSet range
    if (u1PEQSet > 8)//(u1PEQSet > 7)
    {
        u1PEQSet = 8;//u1PEQSet = 7;
    }

    // Share memory address
    u2ShmAddrPEQFc   = W_PEQ_SET0_FC   + 8*u1PEQSet;
    u2ShmAddrPEQGain = D_PEQ_SET0_GAIN + 8*u1PEQSet;
    u2ShmAddrPEQBW   = W_PEQ_SET0_BW   + 8*u1PEQSet;

    switch(eQValue)
    {
    case AUD_PEQ_Q_0_33:
        u2PEQBW = (30303*u4Frequency / 10000);
        break;
    case AUD_PEQ_Q_0_43:
        u2PEQBW = (23255*u4Frequency / 10000);
        break;
    case AUD_PEQ_Q_0_56:
        u2PEQBW = (17857*u4Frequency / 10000);
        break;   
    case AUD_PEQ_Q_0_75:
        u2PEQBW = (13333*u4Frequency / 10000);
        break;       
    case AUD_PEQ_Q_1_00:
        u2PEQBW = (10000*u4Frequency / 10000);
        break;        
    case AUD_PEQ_Q_1_20:
        u2PEQBW = (8333*u4Frequency / 10000);
        break;        
    case AUD_PEQ_Q_1_50:
        u2PEQBW = (6666*u4Frequency / 10000);
        break;        
    case AUD_PEQ_Q_1_80:
        u2PEQBW = (5555*u4Frequency / 10000);
        break;        
    case AUD_PEQ_Q_2_20:
        u2PEQBW = (4545*u4Frequency / 10000);
        break;        
    case AUD_PEQ_Q_2_27:
        u2PEQBW = (3703*u4Frequency / 10000);
        break;        
    case AUD_PEQ_Q_3_30:
        u2PEQBW = (3030*u4Frequency / 10000);
        break;        
    case AUD_PEQ_Q_3_39:
        u2PEQBW = (2564*u4Frequency / 10000);//u2PEQBW = (2949*u4Frequency / 10000);
        break;        
    case AUD_PEQ_Q_4_70:
        u2PEQBW = (2127*u4Frequency / 10000);//u2PEQBW = (2727*u4Frequency / 10000);
        break;        
    case AUD_PEQ_Q_5_60:
        u2PEQBW = (1785*u4Frequency / 10000);
        break;        
    case AUD_PEQ_Q_6_80:
        u2PEQBW = (1470*u4Frequency / 10000);
        break;        
    case AUD_PEQ_Q_8_20:
        u2PEQBW = (1219*u4Frequency / 10000);
        break;        
    default:
        u2PEQBW = (10000*u4Frequency / 10000);
        break;
    }
  
    u2PEQFc = (UINT16)u4Frequency;
    u4PEQGain = _DbToShm((INT32)i2Gain) << 4;
    
    vWriteShmUINT16(u2ShmAddrPEQFc,   u2PEQFc);
    vWriteShmUINT32(u2ShmAddrPEQGain, u4PEQGain);
    vWriteShmUINT16(u2ShmAddrPEQBW,   u2PEQBW);

    DSP_SendDspTaskCmd(u2UopIndex);

}

//-----------------------------------------------------------------------------
/** _AUD_DspSetPEQCfg
 *   
 *  @param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param u1Set                      PEQ set (0 ~ 7)
 *  @param u4Frequency                Frequency (20 ~ 20K)
 *  @param u2BW                       Bandwidth (xx Hz)
 *  @param i2Gain                     Gain (+-12dB) with 0.5dB/step
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetPEQCfg(UINT8 u1DecId, UINT8 u1PEQSet, UINT32 u4Frequency,
                            UINT16 u2BW, INT16 i2Gain)
{
    UINT16  u2ShmAddrPEQFc;
    UINT16  u2ShmAddrPEQGain;
    UINT16  u2ShmAddrPEQBW;
    UINT16  u2UopIndex = UOP_DSP_PEQ_CONFIG;

    UINT16  u2PEQFc;
    UINT32  u4PEQGain;
    UINT16  u2PEQBW;
    
    UNUSED(u1DecId);

    // Check u1PEQSet range
    if (u1PEQSet > 8)//(u1PEQSet > 7)
    {
        u1PEQSet = 8;//u1PEQSet = 7;
    }

    // Share memory address
    u2ShmAddrPEQFc   = W_PEQ_SET0_FC   + 8*u1PEQSet;
    u2ShmAddrPEQGain = D_PEQ_SET0_GAIN + 8*u1PEQSet;
    u2ShmAddrPEQBW   = W_PEQ_SET0_BW   + 8*u1PEQSet;

    u2PEQFc   = (UINT16)u4Frequency;
    u4PEQGain = _DbToShm((INT32)i2Gain) << 4;
    u2PEQBW   = (UINT16)u2BW;
    
    vWriteShmUINT16(u2ShmAddrPEQFc,   u2PEQFc);
    vWriteShmUINT32(u2ShmAddrPEQGain, u4PEQGain);
    vWriteShmUINT16(u2ShmAddrPEQBW,   u2PEQBW);

    DSP_SendDspTaskCmd(u2UopIndex);

}

//-----------------------------------------------------------------------------
/** _AUD_DspSetVirBassEnable
 *   
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetVirBassEnable(BOOL fgEnable)
{
    UINT16  u2ShmIndex = B_VIR_BASS_ENABLE;
    UINT16 u2UopIndex = UOP_DSP_VIR_BASS_ENABLE;

    vWriteShmUINT8(u2ShmIndex, (UINT8)fgEnable);
    DSP_SendDspTaskCmd(u2UopIndex);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetVirBassCfg
 *   
 *  @param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param u4Frequency                Frequency (20 ~ xx)
 *  @param u4Gain                     
 *  @param u4Gain2                    
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetVirBassCfg(UINT8 u1DecId, UINT32 u4Frequency, UINT32 u4Gain, 
                        UINT32 u4Gain2)
{
    UNUSED(u1DecId);
    UNUSED(u4Frequency);

    UINT16 u2ShmIndex = D_VIR_BASS_GAIN;
    UINT16 u2UopIndex = UOP_DSP_VIR_BASS_CONFIG;

    UNUSED(u2ShmIndex);

    vWriteShmUINT32(D_VIR_BASS_GAIN, u4Gain);
    vWriteShmUINT32(D_VIR_BASS_GAIN_2, u4Gain2);
    DSP_SendDspTaskCmd(u2UopIndex);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetDDCOEnable
 *   
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetDDCOEnable(BOOL fgEnable)
{
    UINT16  u2ShmIndex = B_DDCO_FLAG;
    UINT16 u2UopIndex = UOP_DSP_DDCO_ENABLE;
	
     if ((fgEnable == TRUE) && (uReadShmUINT8(B_DDCO_FLAG)==0x1))
    {
        return;
    }	
    
    if ((fgEnable == FALSE) && (uReadShmUINT8(B_DDCO_FLAG)==0x0))
    {
        return;
    }		 

    if (fgEnable == TRUE)
    {
       vDspLoadRamCode(AUD_DDCO);	
    }	 

    vWriteShmUINT8(u2ShmIndex, (UINT8)fgEnable);
    DSP_SendDspTaskCmd(u2UopIndex);
}


//-----------------------------------------------------------------------------
/** _AUD_DspSetDDCOAgcEnable
 *   
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetDDCOAgcEnable(BOOL fgEnable)
{
    UINT16  u2ShmIndex = B_DDCO_AGC_FLAG;
    UINT16 u2UopIndex = UOP_DSP_DDCO_AGC_ENABLE;
	
    vWriteShmUINT8(u2ShmIndex, (UINT8)fgEnable);
    DSP_SendDspTaskCmd(u2UopIndex);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetDDCOLFEEnable
 *   
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetDDCOLFEEnable(BOOL fgEnable)
{
    UINT16  u2ShmIndex = B_DDCO_LFE_LPF_FLAG;
    UINT16 u2UopIndex = UOP_DSP_DDCO_LFE_LPF_ENABLE;
	
    vWriteShmUINT8(u2ShmIndex, (UINT8)fgEnable);
    DSP_SendDspTaskCmd(u2UopIndex);
}

#ifdef SUPPORT_PHILIPS_LOUDNESS
//-----------------------------------------------------------------------------
/** _AUD_DspSetLoudness
 *   
 *  @param  u1DecId, enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetLoudness(UINT8 u1DecId, BOOL fgEnable)
{
    // 120Hz  : +5, 
    // 200Hz  : -3
    // 500Hz  : +2 
    // 1.2KHz :  0
    // 3  KHz : +2
    // 7.5KHz : +2
    // 12 KHz : +5
    UINT8 u1Idx;
    INT8 i1EqValue;
    AUD_EQ_TYPE_T  eEqType;

    UNUSED(u1DecId);
    UNUSED(fgEnable);   

#ifdef CC_AUD_7BAND_EQUALIZER   // currently only support 7band EQ + Loudness
    if (_fgLoudnessEnable == fgEnable)
    {
        return;
    }    

    VERIFY(x_sema_lock(_ahEqCtlSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    _AUD_DspGetEqType(u1DecId, &eEqType);
    _fgLoudnessEnable = fgEnable;    

    if (eEqType != AUD_EQ_OFF)
    {
        for (u1Idx = 1; u1Idx <= CH_EQ_BAND_NO; u1Idx++)
        {       
            i1EqValue = _aai1ChEqCfg[eEqType][u1Idx];

            if (fgEnable)
            {
                i1EqValue += i1Loundness[u1Idx-1];            
            }
            else
            {
                i1EqValue -= i1Loundness[u1Idx-1];            
            }
        
            i1EqValue = (i1EqValue > 28) ? 28: i1EqValue;
            i1EqValue = (i1EqValue <-28) ? -28: i1EqValue;
            _aai1ChEqCfg[eEqType][u1Idx] = i1EqValue;                
        }
    }
    
    VERIFY(x_sema_unlock(_ahEqCtlSema) == OSR_OK);
#endif
}

void AUD_DspChEqLoudness(UINT8 u1DecId, INT8 *prEqvalue)
{
    UINT8 u1Idx;
    INT8 i1EqValue;    
    
    for(u1Idx = 0; u1Idx <CH_EQ_BAND_NO; u1Idx++)
    {
        i1EqValue = prEqvalue[u1Idx];

        if (_fgLoudnessEnable)
        {
            i1EqValue += i1Loundness[u1Idx];            
        }
        else
        {
            i1EqValue -= i1Loundness[u1Idx];            
        }
        
        i1EqValue = (i1EqValue > 28) ? 28: i1EqValue;
        i1EqValue = (i1EqValue <-28) ? -28: i1EqValue;
        prEqvalue[u1Idx] = i1EqValue;
    }
}


BOOL _AUD_DspGetLoudness(UINT8 u1DecId, BOOL fgEnable)
{
    return _fgLoudnessEnable;
}

#endif

void _AUD_DspSetAoutReinit(void)
{
    UINT16 u2UopIndex = UOP_DSP_AOUT_REINIT_FOR_SAMPLING_RATE_CHANGE;
    UINT16 u2UopIndex2 = UOP_DSP_AOUT_REINIT_FOR_SAMPLING_RATE_CHANGE_DEC2;

    DSP_SendDspTaskCmd(u2UopIndex2);	
    DSP_SendDspTaskCmd(u2UopIndex);
}




//-----------------------------------------------------------------------------
// Begin of NVM and Profile Functions 
//   for customers use EEPROM and FLASH audio parameters
//   1. _AUD_NVM_Get_Profile
//   2. _AUD_NVM_Read
//   3. _AUD_NVM_Function
//   4. AUD_NVM_Init_From_EEPROM
//   5. AUD_NVM_Init_To_EEPROM
//   6. AUD_NVM_Operation
//-----------------------------------------------------------------------------
UINT32 _AUD_NVM_Get_Profile(UINT32* pAudProfileStart, UINT32* pAudProfileEnd, UINT8* u1Profile)
{
#ifndef CC_AUD_USE_NVM
    return 1;
#else
    *pAudProfileStart =  AUD_NVM_PROFILE_START;
    *pAudProfileEnd = AUD_NVM_PROFILE_END;
    *u1Profile = _u1AudProfile[0];

    if ((AUD_NVM_PROFILE_START + 128*_u1AudProfile[0]) > AUD_NVM_PROFILE_END)   //FIXME!! nvm
    {
        return 1;
    }

    *pAudProfileStart += (_u1AudProfile[0]*128);
    return 0;
#endif //CC_AUD_USE_NVM
}

UINT32 _AUD_NVM_Read(void)
{
#ifndef CC_AUD_USE_NVM
    return 1;
#else
    UINT8 uTempBuf[10];
    UINT8 i;
    UINT32 u4Ptr;
    UINT32 u4NvmLength; //Customer NVM Data Length

    UNUSED(_fgNvmFlat);

    // Check if NVM is enabled                
    if ( !(DRVCUST_OptGet(eAudioNvmEnable)) )
    {
        return 1;   //NVM is not ON.
    }

    // ------- Read NVM from EEPROM ----------------------
    if (EEPROM_Read((UINT64)AUD_NVM_OFFSET, (UINT32)auAudNvmBuffer, AUD_NVM_LENGTH))
    {
        return 1;   //EEPROM Read Fail, return 1
    }
    
    // Get Customer NVM Data Length
    u4NvmLength = DRVCUST_OptGet(eAudioNvmLength);

    // Get Customer NVM Data Structure and Data
    VERIFY(DRVCUST_OptQuery(eAudioNvmData, (UINT32 *)&u4Ptr) == 0);
    x_memcpy((VOID *)aeNvmData, (const VOID *)u4Ptr, sizeof(AUD_NVM_DATA_T) * u4NvmLength);
      
    // ------- Read Profile Releated Data from EEPROM -----
    for (i=0; i<3; i++)
    {
        if (EEPROM_Read((UINT64)_u2AudNVMProfileOffset[i], (UINT32)uTempBuf, 10))
        {
            return 1;
        }
        _fgAudNVMLipSync[i] = uTempBuf[2] & 0x04; // bit2 of byte 3
        _u1AudProfile[i] = uTempBuf[9] & 0x1f;    // bit0~4 of byte 10;
    }
    if (_u1AudProfile[0] != _u1AudProfile[1])
    {
        if (_u1AudProfile[1] == _u1AudProfile[2])
        {
            _u1AudProfile[0] = _u1AudProfile[1];
            _fgAudNVMLipSync[0] = _fgAudNVMLipSync[0];
        }
    }    
    if (_u1AudProfile[0] > 10)   //FIXME!!! if NVM not initialized, use profile 0
    {
        _u1AudProfile[0] = 0;
    }
    // ---------------------------------------------------------------------
    
    _fgNvmInit = TRUE;
    return 0;
#endif //CC_AUD_USE_NVM
}

#ifdef CC_AUD_USE_NVM
UINT32 _AUD_NVM_Function(AUD_NVM_DATA_T rNvmData, UINT32 u4Data)
{
    INT16 i2PreScale;

    UNUSED(i2PreScale);

    switch (rNvmData.eDataType)
    {
        case AUD_NVM_MTS_OBSRV_TIME :
            vWriteShmUINT16(W_MTS_NUM_CHECK, (UINT16)u4Data);
            break;
        case AUD_NVM_MTS_STEREO_TIME :
            vWriteShmUINT16(W_MTS_NUM_PILOT, (UINT16)u4Data);
            break;
        case AUD_NVM_MTS_SAP_TIME :
            vWriteShmUINT16(W_MTS_NUM_SAP, (UINT16)u4Data);
            break;
        case AUD_NVM_MTS_UP_THR_BTSC :
            vWriteShmUINT16(W_MTS_STEREO_CON_MID, (UINT16)u4Data);                
            break;
        case AUD_NVM_MTS_LO_THR_BTSC :
            vWriteShmUINT16(W_MTS_STEREO_MID, (UINT16)u4Data);
            break;
        case AUD_NVM_MTS_UP_THR_SAP :
            vWriteShmUINT16(W_MTS_SAP_CON_MID, (UINT16)u4Data);
            break;
        case AUD_NVM_MTS_LO_THR_SAP:
            vWriteShmUINT16(W_MTS_SAP_MID, (UINT16)u4Data);
            break;
        case AUD_NVM_MTS_NMUTE_FM_UPTHR :
            vWriteShmUINT8(B_FM_MUTE_THRESHOLD_HIGHT_MTS, (UINT8)u4Data);
            break;    
        case AUD_NVM_MTS_NMUTE_FM_LOTHR :
            vWriteShmUINT8(B_FM_MUTE_THRESHOLD_LOW_MTS, (UINT8)u4Data);
            break;
        case AUD_NVM_MTS_NMUTE_SAP_UPTHR :
            vWriteShmUINT8(B_MTS_MUTE_SAP_HIGH, (UINT8)u4Data);
            break;
        case AUD_NVM_MTS_NMUTE_SAP_LOTHR :
            vWriteShmUINT8(B_MTS_MUTE_SAP_LOW, (UINT8)u4Data);
            break;
        case AUD_NVM_MTS_PRE_BTSC_STMONO :
            vWriteShmUINT32(D_MTS_FINE_TUNE_VOLUME, u4Data);
            break;
        case AUD_NVM_MTS_PRE_SAP :
            vWriteShmUINT32(D_SAP_FINE_TUNE_VOLUME, u4Data);
            break;
        case AUD_NVM_MTS_HIGH_DEV :
        {
            UINT8 u1Mode;
            u1Mode = uReadShmUINT8(B_HDEV_MODE) & 0xfb;
            if ((UINT8)u4Data)
            {
                u1Mode |= 0x04;
            }
            vWriteShmUINT8(B_HDEV_MODE, u1Mode);
            break;
        }
        case AUD_NVM_MTS_CARRIER_SHIFT :
        {
            UINT8 u1Mode;
            u1Mode = uReadShmUINT8(B_CARRIER_SHIFT_MODE) & 0xfb;
            if ((UINT8)u4Data)
            {
                u1Mode |= 0x04;
            }
            vWriteShmUINT8(B_CARRIER_SHIFT_MODE, u1Mode);
            break;
        }
        case AUD_NVM_MTS_FMCARR_MUTE :
        {
            UINT8 u1Mode;
            u1Mode = uReadShmUINT8(B_FM_MUTE_MODE) & 0xfb;
            if ((UINT8)u4Data)
            {
                u1Mode |= 0x04;
            }
            vWriteShmUINT8(B_FM_MUTE_MODE, u1Mode);
            break;        
        }    
        case AUD_NVM_AVC_ADJUST_RATE :
            vWriteShmUINT16(W_AVC_ADJUST_RATE, (UINT16)u4Data);
            break;
        case AUD_NVM_AVC_UI_ADJUST_RATE :
            vWriteShmUINT16 (W_DSP_AVC_UI_ADJUST_RATE, (UINT16)u4Data); // (16.8) format, 4dB / sec @ 32k Hz
            break;
        case AUD_NVM_AVC_LEVEL :
            vWriteShmUINT16 (W_AVC_TARGET_LEV, (UINT16)(INT8)u4Data);
            break;
        case AUD_NVM_AVC_GAIN_UP :
            vWriteShmUINT16 (W_AVC_MAX_GAIN_UP, (UINT16)u4Data);
            break;
        case AUD_NVM_LIPSYNC_A :
            vWriteShmUINT16 (W_CHDELAY_C, (UINT16)u4Data);
            vWriteShmUINT16 (W_CHDELAY_L, (UINT16)u4Data);
            vWriteShmUINT16 (W_CHDELAY_R, (UINT16)u4Data);
            vWriteShmUINT16 (W_CHDELAY_LS, (UINT16)u4Data);
            vWriteShmUINT16 (W_CHDELAY_RS, (UINT16)u4Data);
            vWriteShmUINT16 (W_CHDELAY_CH7, (UINT16)u4Data);
            vWriteShmUINT16 (W_CHDELAY_CH8, (UINT16)u4Data);
            vWriteShmUINT16 (W_CHDELAY_SUB, (UINT16)u4Data);
            vWriteShmUINT16 (W_CHDELAY_CH9, (UINT16)u4Data);
            vWriteShmUINT16 (W_CHDELAY_CH10, (UINT16)u4Data);
            break;
        case AUD_NVM_LIPSYNC_B : //HP delay for TPV_PHILIP
            vWriteShmUINT16 (W_CHDELAY_CH9, (UINT16)u4Data);
            vWriteShmUINT16 (W_CHDELAY_CH10, (UINT16)u4Data);
            break;
        case AUD_NVM_LIPSYNC_C : //SPDIF delay for TPV_PHILIP
            vWriteShmUINT16 (W_CHDELAY_CH7, (UINT16)u4Data);
            vWriteShmUINT16 (W_CHDELAY_CH8, (UINT16)u4Data);
            break;    
        case AUD_NVM_PL2_ON :
        {   
            UINT16 u2PL2Cfg;
            u2PL2Cfg = u2ReadShmUINT16(W_PLIICONFIG) & (0xFFFE);
            vWriteShmUINT16 (W_PLIICONFIG, (u2PL2Cfg | (UINT16)(u4Data & 0x1)));
            break;
        }
        case AUD_NVM_PL2_MODE :
            vWriteShmUINT16 (W_PLIIMODE, (UINT16)u4Data);
            break;    
        case AUD_NVM_REVERB:
        {
            #if 0   // Reverb is removed
            UINT8 u1Mode;    
            if ((UINT8)u4Data > 0)
            {
                u1Mode = (UINT8)u4Data - 1;
                vWriteShmUINT8(B_REVERBFLAG, 1);
                vWriteShmUINT32(D_REVERBGAIN,  _arReverMode[u1Mode].u4Gain);
                vWriteShmUINT8(B_REVERBBANK0, _arReverMode[u1Mode].u1ReverbBank0);
                vWriteShmUINT8(B_REVERBBANK1, _arReverMode[u1Mode].u1ReverbBank1);
                vWriteShmUINT8(B_REVERBBANK2, _arReverMode[u1Mode].u1ReverbBank2);
                vWriteShmUINT8(B_REVERBBANK3, _arReverMode[u1Mode].u1ReverbBank3);
            }
            else
            {
                vWriteShmUINT8(B_REVERBFLAG, 0);
            }    
            #endif
            break;
        }        
        case AUD_NVM_VOL_MIN :
            u4NvmVolTab[0] = u4Data;
            break;
        case AUD_NVM_VOL_MAX :
            u4NvmVolTab[VOL_TABLE_CHK_PNT-1] = u4Data;
            break;
        case AUD_NVM_VOL_STEP1 :
            u4NvmVolTab[1] = u4Data;
            break;
        case AUD_NVM_VOL_STEP2 :
            u4NvmVolTab[2] = u4Data;
            break;
        case AUD_NVM_VOL_STEP3 :
            u4NvmVolTab[3] = u4Data;
            break;
        case AUD_NVM_VOL_STEP4 :
            u4NvmVolTab[4] = u4Data;
            break;
        case AUD_NVM_VOL_STEP5 :
            u4NvmVolTab[5] = u4Data;
            break;
        case AUD_NVM_PREHDMI :
            i2PreScale = ((UINT8)u4Data - 128);
            if (i2PreScale < -24)
            {
                i2PreScale = -24; //at most -12dB
            }
            _rNVMIuputSrcGain.i2NvmInputSrcHDMI = i2PreScale;
            break;
        case AUD_NVM_PREAV :
            i2PreScale = ((UINT8)u4Data - 128);
            if (i2PreScale < -24)
            {
                i2PreScale = -24; //at most -12dB
            }            
            _rNVMIuputSrcGain.i2NvmInputSrcAV = i2PreScale;
            break;
        case AUD_NVM_PREUSB :
            i2PreScale = ((UINT8)u4Data - 128);
            if (i2PreScale < -24)
            {
                i2PreScale = -24; //at most -12dB
            }
            _rNVMIuputSrcGain.i2NvmInputSrcUSB = i2PreScale;
            break;
        case AUD_NVM_PREATV :
            i2PreScale = ((UINT8)u4Data - 128);
            if (i2PreScale < -24)
            {
                i2PreScale = -24; //at most -12dB
            }            
            _rNVMIuputSrcGain.i2NvmInputSrcATV = i2PreScale;
            break;            
        case AUD_NVM_PREDTV :
            i2PreScale = ((UINT8)u4Data - 128);
            if (i2PreScale < -24)
            {
                i2PreScale = -24; //at most -12dB
            }                        
            _rNVMIuputSrcGain.i2NvmInputSrcDTV = i2PreScale;
            break;    
        case AUD_NVM_BASS_SUBWOOFER :
        {
            UINT32 u4Config;
            u4Config = u4ReadShmUINT32(D_SPKCFG) & 0xffffffdf;
            if ((UINT8)u4Data)
            {
                u4Config |= 0x20;
            }
            vWriteShmUINT32(D_SPKCFG, u4Config);
            break;
        }
        case AUD_NVM_BASS_SPKSIZE :   
        {
            UINT32 u4Config;
            u4Config = u4ReadShmUINT32(D_SPKCFG) & 0xffff9fff;
            if ((UINT8)u4Data)
            {
                u4Config |= 0x6000;
            }
            vWriteShmUINT32(D_SPKCFG, u4Config);
            break;
        }
        case AUD_NVM_BBE_LEVEL :
            vWriteShmUINT8 (B_BBE_LEVEL, (UINT8)u4Data);
            _i1BbeLevel = (UINT8)u4Data;
            break;            
        case AUD_NVM_BBE_UPGAIN :
            vWriteShmUINT8 (B_BBE_PROCESS, (UINT8)u4Data);
            _u1BbeProcess = (UINT8)u4Data;
            break;
        case AUD_NVM_BBE_LOGAIN :
            vWriteShmUINT8 (B_BBE_LO_CONTOUR, (UINT8)u4Data);
            _u1BbeLoContour = (UINT8)u4Data;
            break;
        case AUD_NVM_BBE_3DGAIN :
            vWriteShmUINT8 (B_BBE_3D_GAIN, (UINT8)u4Data);
            _u1Bbe3dGain = (UINT8)u4Data;
            break;
        case AUD_NVM_VS_CLARITY :
            _rNVMSurrCfg.u4Clarity = u4Data;
            break;
        case AUD_NVM_VS_WIDTH :
            _rNVMSurrCfg.u4Width = u4Data;
            break;
        case AUD_NVM_VS_LRGAIN :
            _rNVMSurrCfg.u4LRGain = u4Data;
            break;
        case AUD_NVM_VS_XTALK_GIAN :
            _rNVMSurrCfg.u4Xtalk = u4Data;
            break;
        case AUD_NVM_VS_OPGAIN :
            _rNVMSurrCfg.u4OutputGain = u4Data;
            break;
        case AUD_NVM_VS_SWGAIN :
            _rNVMSurrCfg.u4BassGain = u4Data;
            break;            
        case AUD_NVM_VS_FO :
            _rNVMSurrCfg.u4Fo = u4Data;
            break;
        case AUD_NVM_FLAT :
        {
            UINT16 u2Mode;
            if ((UINT8)u4Data == 1)
            {            
                //bypass delay and post-processing, keep trim and bass management (1010)
                u2Mode = u2ReadShmUINT16(W_PROCMOD) | (0xA000);
                vWriteShmUINT16 (W_PROCMOD, u2Mode);
                _fgNvmFlat = TRUE;
            }
            else
            {
                _fgNvmFlat = FALSE;
            }
            break;
        }                
        default:
            break;        
    }
    
    return 0;
}

UINT32 AUD_NVM_Init_From_EEPROM(void)
{
    UINT32 u4NvmLength;  //Customer NVM Data Length
    UINT32 u4NvmData;
    UINT8  i,j;

    // Check if NVM is enabled                
    if ( !(DRVCUST_OptGet(eAudioNvmEnable)) )
    {
        return 1;   //NVM is not ON.
    }

    if (!(_fgNvmInit))
    {
        return 1;   //NVM data have not been read from EEPROM.
    }
        
    // Get Customer NVM Data Length.
    u4NvmLength = DRVCUST_OptGet(eAudioNvmLength);

    // Initialize Audio from NVM values
    for (i=0; i < u4NvmLength; i++)
    {
        u4NvmData = 0;
        for (j=0; j < aeNvmData[i].u1Length; j++)
        {
            u4NvmData =((auAudNvmBuffer[(aeNvmData[i].u2Offset)+j]  << (8*j)) | u4NvmData);    
        }
        _AUD_NVM_Function(aeNvmData[i], u4NvmData);
    }

    return 0;
}

UINT32 AUD_NVM_Init_To_EEPROM(void)
{
    UINT8  uNvmBuffer[AUD_NVM_LENGTH];    // Buffer read from EEPROM
    UINT32 u4NvmLength;                   // Customer NVM Data Length
    UINT8  i,j;

    // Check if NVM is enabled                
    if ( !(DRVCUST_OptGet(eAudioNvmEnable)) )
    {
        return 1;   //NVM is not ON.
    }

    // Get Customer NVM Data Length.
    u4NvmLength = DRVCUST_OptGet(eAudioNvmLength);
       
    // Clean uNvmBuffer
    for (i=0; i < AUD_NVM_LENGTH; i++)
    {
        uNvmBuffer[i] = 0xFF;
    }
    
    // Write Default Value to NVM.
    for (i=0; i < u4NvmLength; i++)
    {
        for (j=0; j < aeNvmData[i].u1Length; j++)
        {
            uNvmBuffer[(aeNvmData[i].u2Offset + j)] = ( (aeNvmData[i].u4DefaultValue) >> (j*8) ) & 0xFF;
            LOG(9,"uNvmBuffer[%d] = 0x%x\n", (aeNvmData[i].u2Offset + j), uNvmBuffer[(aeNvmData[i].u2Offset + j)] );
        }
    }

    if (EEPROM_Write((UINT64)AUD_NVM_OFFSET, (UINT32)uNvmBuffer, AUD_NVM_LENGTH))
    {
        return 1;   //EEPROM Read Fail
    }
    
    return 0;
}

BOOL Aud_NVM_GetCountry(void)
{
    UINT8  u1AudProfile[3] = {0,0,0};
    UINT16 u2AudNVMProfileOffset[3] = {0x00D0,0x08E0,0x13C0}; 
    UINT8 uTempBuf;    
    BOOL  fgLatam = TRUE;
    UINT8 i;    

    //if ( !(DRVCUST_OptGet(eAudioNvmGetCountry)) )
    // by gallen at 20081107
    {
        return TRUE;
    }
    
    // LC09M InputMux Cust
    for (i=0; i<3; i++)
    {
        if (EEPROM_Read((UINT64)u2AudNVMProfileOffset[i], (UINT32)&uTempBuf, 1))
        {
            LOG(0,"### LC09M Get Country from NVM Fail!\n");
            return fgLatam;
        }
        u1AudProfile[i] = uTempBuf & 0x3;    // bit0~1
    }

    if (u1AudProfile[0] != u1AudProfile[1])
    {
        if (u1AudProfile[1] == u1AudProfile[2])
        {
            u1AudProfile[0] = u1AudProfile[1];
        }
    }        
    LOG(0,"### LC09M Get Country =%d\n", u1AudProfile[0]);

    if (u1AudProfile[0] == 1)   // 1: AP, 2: Latam
    {
        fgLatam = FALSE;        // False means query specail input mux table
    }

    return fgLatam;    
}

#endif //#ifdef CC_AUD_USE_NVM

UINT32 AUD_NVM_Operation(AUD_NVM_OP_T uOP, UINT8* puNvmBuffer, UINT16 u2Offset)
{
#ifndef CC_AUD_USE_NVM
    return 1;       // NVM if not defined
#else
    if ( !(DRVCUST_OptGet(eAudioNvmEnable)) )
    {
        return 1;   // NVM is not ON.
    }
    switch (uOP)
    {
        case AUD_NVM_READ :
            if (EEPROM_Read((UINT64)AUD_NVM_OFFSET, (UINT32)puNvmBuffer, AUD_NVM_LENGTH))
            {
                return 1;   //EEPROM Read Fail
            }
            break;
            
        case AUD_NVM_WRITE :
            if (u2Offset > (AUD_NVM_LENGTH))
            {
                return 1;
            }
            if (EEPROM_Write((UINT64)(AUD_NVM_OFFSET + u2Offset), (UINT32)puNvmBuffer, 1))
            {
                return 1;   //EEPROM Read Fail
            }
            break;
            
        case AUD_NVM_INIT :
            if (AUD_NVM_Init_To_EEPROM())
            {
                return 1;
            }
            break;
            
        case AUD_NVM_VOL_INIT :
            _AUD_DspSetVolumeTable(u4NvmVolTab);
            _aai2InputSrcVol[AUD_DEC_MAIN][AUD_STREAM_FROM_HDMI] = _rNVMIuputSrcGain.i2NvmInputSrcHDMI;
            _aai2InputSrcVol[AUD_DEC_MAIN][AUD_STREAM_FROM_LINE_IN] = _rNVMIuputSrcGain.i2NvmInputSrcAV;
            _aai2InputSrcVol[AUD_DEC_MAIN][AUD_STREAM_FROM_BUF_AGT] = _rNVMIuputSrcGain.i2NvmInputSrcUSB;
            _aai2InputSrcVol[AUD_DEC_MAIN][AUD_STREAM_FROM_MULTI_MEDIA] = _rNVMIuputSrcGain.i2NvmInputSrcUSB;
            _aai2InputSrcVol[AUD_DEC_MAIN][AUD_STREAM_FROM_DIGITAL_TUNER] = _rNVMIuputSrcGain.i2NvmInputSrcDTV;
            _aai2InputSrcVol[AUD_DEC_MAIN][AUD_STREAM_FROM_ANALOG_TUNER] = _rNVMIuputSrcGain.i2NvmInputSrcATV;
            break;
            
        case AUD_NVM_VS_INIT :
            DSP_Set_VSurr_Cfg(AUD_SUR_CLARITY, _rNVMSurrCfg.u4Clarity);
            DSP_Set_VSurr_Cfg(AUD_SUR_LRGAIN, _rNVMSurrCfg.u4LRGain);
            DSP_Set_VSurr_Cfg(AUD_SUR_XTALK, _rNVMSurrCfg.u4Xtalk);
            DSP_Set_VSurr_Cfg(AUD_SUR_OUTPUT_GAIN, _rNVMSurrCfg.u4OutputGain);
            DSP_Set_VSurr_Cfg(AUD_SUR_BASS_GAIN, _rNVMSurrCfg.u4BassGain);
            DSP_Set_VSurr_Cfg(AUD_SUR_FO, _rNVMSurrCfg.u4Fo);
            break;

    }
    
    return 0;
#endif  //CC_AUD_USE_NVM
}

// =================== End of NVM and Profile Functions ====================================
//------------------------------------------------------------------------------------------
/** Aud_FlashAQParsing
 *  Parsing binary data from Flash
 *
 *  @param  eAQDataType         AQ data type
 *  @param  pData               pointer to binary data array
 *  @retval void
 */
//-------------------------------------------------------------------------------------------
#ifdef CC_AUD_USE_FLASH_AQ
void Aud_FlashAQSetValue(void)
{
    UINT8 i;  

    for (i=0; i<=AQ_TYPE_NUM; i++)
    {
        _fgAQ_Enable[i] = FALSE;
    }        
}

BOOL Aud_FlashAQQuery(AUD_AQ_TYPE_T eAQDataType, VOID* *pData)
{
    BOOL fgRet = FALSE;
    AUD_DEC_STREAM_FROM_T eStreamFrom;
    UINT8 eConvertSrc;

    AUD_DRVGetStreamFrom(AUD_DEC_MAIN, &eStreamFrom);
    VERIFY(eStreamFrom < AUD_STREAM_FROM_NUM);

    eConvertSrc = _au1AQSrc_Convert[eStreamFrom];
    VERIFY( eConvertSrc < FLASH_AQ_SRC_NUM );
    
    switch (eAQDataType)
    {
    case AQ_TYPE_POST:
        if (_fgAQ_Enable[AQ_TYPE_POST])
        {
            *(UINT32*)pData = _au4AQ_PostScale[0];
            *(UINT32*)(pData+1) = _au4AQ_PostScale[1];
            fgRet = TRUE;            
        }
        else
        {
            fgRet = FALSE;
        }        
        break;    
    case AQ_TYPE_SPKHEIGHT:
        if (_fgAQ_Enable[AQ_TYPE_SPKHEIGHT])
        {
        
            *(UINT32*)pData = _au1AQ_SPH[eConvertSrc][0];
            fgRet = TRUE;            
        }
        else
        {
            fgRet = FALSE;
        }        
        break;    
    case AQ_TYPE_LIMITER:
        if (_fgAQ_Enable[AQ_TYPE_LIMITER])
        {
            *(UINT32*)pData = _au4AQ_LIMITER[eConvertSrc][0];
            fgRet = TRUE;            
        }
        else
        {
            fgRet = FALSE;
        }        
        break;    
    default:
        break;
    };

    return fgRet;
}

void Aud_FlashAQParsing(AUD_AQ_TYPE_T eAQDataType, UINT8* pData)
{
    UINT16 i;  //loop index
    UINT16 j; //loop index
    UINT16 u2Offset;
    UINT32 u4TmpValue;
    
    //-- remove these after all data are applied to flow.
    UNUSED(_au1AQSrc_Convert);
    UNUSED(_au1AQLevel);
    UNUSED(_au1AQBassTrebleOffset);
    UNUSED(_au1AQ_EQLevel);
    UNUSED(_au1AQ_DRC);
    UNUSED(_au1AQ_AVC);
    UNUSED(_au4AQ_LIMITER);
    UNUSED(_au1AQ_SPH);
    UNUSED(_au4AQ_MVS);
    UNUSED(_au4AQ_PostScale);
    UNUSED(_au1AQ_PEQ);
    UNUSED(_au1AQ_LO_Vol);    
    UNUSED(_au4AQ_EXT_PEQ);
    
    switch (eAQDataType)
    {
        case AQ_TYPE_VERSION:
            //LOG(3,"AQ version is %s\n", pData);
            break;

        case AQ_TYPE_CH_VOLUME:
            u2Offset = 0;
            for (i=0; i<FLASH_AQ_COMM_SRC_NUM; i++)
            {
                for(j=0; j<FLASH_AQ_COMM_CH_NUM; j++)
                {
                    _au1AQChVolume[i][j] = pData[u2Offset];
                    u2Offset += 1;
                }
            }
            _fgAQ_Enable[AQ_TYPE_CH_VOLUME] = TRUE;
            break;
            
        case AQ_TYPE_SRC_VOLUME:
            for (i=0; i<=AUD_STREAM_FROM_MEMORY; i++)
            {
                _aai2InputSrcVol[AUD_DEC_MAIN][i] = (INT16)(pData[i] - 128);
            }
            // Table doesn't provide, so use the same value as MEMORY
            for (i=AUD_STREAM_FROM_BUF_AGT; i<=AUD_STREAM_FROM_MULTI_MEDIA; i++)
            {
                _aai2InputSrcVol[AUD_DEC_MAIN][i] = (INT16)(pData[AUD_STREAM_FROM_MEMORY] - 128);
            }
            break;
            
        case AQ_TYPE_VOL_TABLE:
            for (i=0; i<MAX_VOL_LEVEL+1; i++)
            {
                j = i*3;
                u4TmpValue = (UINT32)(pData[j]<<16) + (UINT32)(pData[j+1]<<8) + (UINT32)(pData[j+2]);
                _au4VOLUME[i]= u4TmpValue;
            }            
            _fgAQ_Enable[AQ_TYPE_VOL_TABLE] = TRUE;
            break;
#ifdef CC_AUD_NEW_CV_TABLE
        case AQ_TYPE_LO_VOL_TABLE:
            for (i=0; i<MAX_VOL_LEVEL+1; i++)
            {
                j = i*3;
                u4TmpValue = (UINT32)(pData[j]<<16) + (UINT32)(pData[j+1]<<8) + (UINT32)(pData[j+2]);
                _au4CHVOLUME[i]= u4TmpValue;
            }            
            _fgAQ_Enable[AQ_TYPE_LO_VOL_TABLE] = TRUE;
            break;
#endif //CC_AUD_NEW_CV_TABLE

         case AQ_TYPE_PEQ:
             for (j=0; j<(FLASH_AQ_SRC_NUM * FLASH_AQ_SM_NUM * AQ_PEQ_ITEMS); j++)
             {
                 *((UINT8*)(_au1AQ_PEQ) + j) = (UINT8)pData[j];
             }            
             _fgAQ_Enable[AQ_TYPE_PEQ] = TRUE;
         break;

         case AQ_TYPE_SP_VOL:
             for (i=0; i<(FLASH_AQ_SRC_NUM); i++)
             {
                 *((UINT8*)(_au1AQ_SP_Vol) + i) = (UINT8)pData[i];
             }            
             _fgAQ_Enable[AQ_TYPE_SP_VOL] = TRUE;
         break;
         
         case AQ_TYPE_LO_VOL:
             for (i=0; i<(FLASH_AQ_SRC_NUM); i++)
             {
                 *((UINT8*)(_au1AQ_LO_Vol) + i) = (UINT8)pData[i];
             }            
             _fgAQ_Enable[AQ_TYPE_LO_VOL] = TRUE;
            break;

        case AQ_TYPE_LEVEL:
            for (i=0; i<(FLASH_AQ_SRC_NUM * FLASH_AQ_SM_NUM * AQ_LEVEL_ITEMS); i++)
            {
                *((UINT8*)(_au1AQLevel) + i) = (UINT8)pData[i];
            }            
            _fgAQ_Enable[AQ_TYPE_LEVEL] = TRUE;
            break;
            
        case AQ_TYPE_BASS_TREBLE:
            for (i=0; i<(FLASH_AQ_SRC_NUM * FLASH_AQ_SM_NUM * AQ_BASS_ITEMS); i++)
            {
                *((UINT8*)(_au1AQBassTrebleOffset) + i) = (UINT8)pData[i];
            }            
            _fgAQ_Enable[AQ_TYPE_BASS_TREBLE] = TRUE;
            break;

        case AQ_TYPE_EQ:
            for (i=0; i<(FLASH_AQ_SRC_NUM * FLASH_AQ_SM_NUM * AQ_EQ_ITEMS); i++)
            {
                *((UINT8*)(_au1AQ_EQLevel) + i) = (UINT8)pData[i];
            }            
            _fgAQ_Enable[AQ_TYPE_EQ] = TRUE;            
            break;
            
        case AQ_TYPE_DRC:
            for (i=0; i<(FLASH_AQ_SRC_NUM * FLASH_AQ_SM_NUM * AQ_DRC_ITEMS); i++)
            {
                *((UINT8*)(_au1AQ_DRC) + i) = (UINT8)pData[i];
            }            
            _fgAQ_Enable[AQ_TYPE_DRC] = TRUE;                        
            break;
            
        case AQ_TYPE_AVC:
            for (i=0; i<(FLASH_AQ_SRC_NUM * AQ_AVC_ITEMS); i++)
            {
                *((UINT8*)(_au1AQ_AVC) + i) = (UINT8)pData[i];
            }
#ifdef CC_AUD_NEW_AQ_PARSER            
            for(i = 0; i < AUD_STREAM_FROM_NUM ; i++ )
            {
                j = _au1AQSrc_Convert[i];
                VERIFY( j < FLASH_AQ_SRC_NUM );
                _ai2AvcTargetLevel[i] = (INT16)_au1AQ_AVC[j][1];
                _ai2AvcGainUp[i] = (INT16)_au1AQ_AVC[j][2];
                _ai2AvcAdjRate[i]= (INT16)(_au1AQ_AVC[j][3]<<8 + _au1AQ_AVC[j][4]);
            }            
#endif //CC_AUD_NEW_AQ_PARSER
            _fgAQ_Enable[AQ_TYPE_AVC] = TRUE;                                    
            break;
            
        case AQ_TYPE_LIMITER:
            for (i=0; i<(FLASH_AQ_SRC_NUM * AQ_LIMITER_ITEMS); i++)
            {
                j = i*3;
                u4TmpValue = (UINT32)(pData[j]<<16) + (UINT32)(pData[j+1]<<8) + (UINT32)(pData[j+2]);
                *((UINT32*)(_au4AQ_LIMITER) + i) = u4TmpValue;
            }            
            _fgAQ_Enable[AQ_TYPE_LIMITER] = TRUE;                                                
            break;
            
        case AQ_TYPE_SPKHEIGHT:
            for (i=0; i<(FLASH_AQ_SRC_NUM * AQ_SPH_ITEMS); i++)
            {
                *((UINT8*)(_au1AQ_SPH) + i) = (UINT8)pData[i];
            }            
            _fgAQ_Enable[AQ_TYPE_SPKHEIGHT] = TRUE;                                                            
            break;

        case AQ_TYPE_MVS:
            for (i=0; i<(FLASH_AQ_SCENE_NUM * AQ_MVS_ITEMS); i++)
            {
                j = i*3;
                u4TmpValue = (UINT32)(pData[j]<<16) + (UINT32)(pData[j+1]<<8) + (UINT32)(pData[j+2]);
                *((UINT32*)(_au4AQ_MVS) + i) = u4TmpValue;
            }            
            _fgAQ_Enable[AQ_TYPE_MVS] = TRUE;                                                                        
            break;
            
        case AQ_TYPE_POST:
            for (i=0; i<AQ_POSTSCL_ITEMS; i++)
            {
                j = i*3;
                u4TmpValue = (UINT32)(pData[j]<<16) + (UINT32)(pData[j+1]<<8) + (UINT32)(pData[j+2]);
                *((UINT32*)(_au4AQ_PostScale) + i) = u4TmpValue;
            }            
            _fgAQ_Enable[AQ_TYPE_POST] = TRUE;                                                                                    
            break;            
                                    
        case AQ_TYPE_EXT_PEQ:
            for (i=0; i<(FLASH_AQ_SRC_NUM*FLASH_AQ_SM_NUM*AQ_AMP_FS_NUM*AQ_EXT_PEQ_ITEMS); i++)
            {
                j = i*3;
                u4TmpValue = (UINT32)(pData[j]<<16) + (UINT32)(pData[j+1]<<8) + (UINT32)(pData[j+2]);                             
                *((UINT32*)(_au4AQ_EXT_PEQ) + i) = u4TmpValue;
            }            
            _fgAQ_Enable[AQ_TYPE_EXT_PEQ] = TRUE;
            break;
                                    
        default:
            break;
    }
}

void _AUD_FlashAQPeriphSMChange(AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 u1ModeCase)
{
    UINT8 eConvertSrc,eConvertFs;
    UINT8 i;
    AUD_AQ_AMP_DRC_T eDrcData;
    static AUD_DEC_STREAM_FROM_T eOriStreamFrom = AUD_STREAM_FROM_OTHERS;
    static UINT8 u1OriModeCase = 0xFF;

    UNUSED(i);
    UNUSED(eConvertSrc);
    UNUSED(eDrcData);
    UNUSED(eConvertFs);

    //Skip if mode and source is the same.
    if ((eOriStreamFrom == eStreamFrom) && (u1OriModeCase == u1ModeCase))
    {
        return;
    }
    else
    {
        eOriStreamFrom = eStreamFrom;
        u1OriModeCase = u1ModeCase;        
    }
    
    eConvertSrc = _au1AQSrc_Convert[eStreamFrom];

    // Channel Level        
    if (_fgAQ_Enable[AQ_TYPE_LEVEL])
    {        
        for (i=0; i<=2; i++)
        {
            ADAC_DigiAmp_SetChannelVol(AUD_AMP_ID_LR,i,_au1AQLevel[eConvertSrc][u1ModeCase][i]);
        }
    }

    // Dual Band DRC.
    if (_fgAQ_Enable[AQ_TYPE_DRC])
    {       
        x_memcpy(&eDrcData, &(_au1AQ_DRC[eConvertSrc][u1ModeCase]), sizeof(AUD_AQ_AMP_DRC_T));
        ADAC_DigiAmp_SetDualDRC(AUD_AMP_ID_LR, eDrcData);        
    }
}


void _AUD_FlashAQPeriph_EXTPEQ(AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 u1ModeCase, SAMPLE_FREQ_T eFS)
{
    UINT8 eConvertSrc,eConvertFs;
    UINT8 i;
    AUD_AQ_EXTPEQ_DATA_T rPEQData;
    static AUD_DEC_STREAM_FROM_T eOriStreamFrom = AUD_STREAM_FROM_OTHERS;
    static UINT8 u1OriModeCase = 0xFF;
    static SAMPLE_FREQ_T eOriFS = FS_64K;
    
    UNUSED(i);
    UNUSED(eConvertSrc);
    UNUSED(rPEQData);
    UNUSED(eFS);
    UNUSED(eConvertFs);

    //Skip if mode and source is the same.
    if ((eOriStreamFrom == eStreamFrom) && (u1OriModeCase == u1ModeCase) && (eOriFS == eFS))
    {
        return;
}
    else
    {
        eOriStreamFrom = eStreamFrom;
        u1OriModeCase = u1ModeCase;        
        eOriFS = eFS;        
    }
    
    eConvertSrc = _au1AQSrc_Convert[eStreamFrom];    
    // External PEQ
    if (_fgAQ_Enable[AQ_TYPE_EXT_PEQ])
    {       
        if (eFS == FS_32K)
            eConvertFs = AQ_AMP_FS_32K;
        else if(eFS == FS_44K)
            eConvertFs = AQ_AMP_FS_44K;
        else
            eConvertFs = AQ_AMP_FS_48K;            

        x_memcpy(&rPEQData, &(_au4AQ_EXT_PEQ[eConvertSrc][u1ModeCase][eConvertFs]), sizeof(AUD_AQ_EXTPEQ_DATA_T));
        ADAC_DigiAmp_SetPEQ(AUD_AMP_ID_LR, rPEQData);        
    }
}



void _AUD_FlashAQOperation(UINT8 u1DecId, AUD_AQ_TYPE_T eAQ_Type, AUD_AQ_OP_TYPE_T eAQ_OPType, UINT16 u2Data)
{
    UINT8 u1ChIdx;
    UINT8 eConvertSrc;
    UINT8 i;
    INT8  eqValue[CH_EQ_BAND_NO+1];    
    AUD_DEC_STREAM_FROM_T eStreamFrom;
        
    UNUSED(eStreamFrom);        
    UNUSED(u1ChIdx);
    UNUSED(eConvertSrc);
    UNUSED(eAQ_OPType);
    UNUSED(u1DecId);
    UNUSED(i);    
    UNUSED(eqValue);

    switch (eAQ_Type)
    {
    
    case AQ_TYPE_CH_VOLUME:
        // Apply the Channel Volume Table.
        if ((u1DecId == AUD_DEC_MAIN) && (_fgAQ_Enable[AQ_TYPE_CH_VOLUME]))
        {
            //The StreamFrom in AQ Table is not in the same order as our emu type.
            eConvertSrc = _au1AQSrc_Comm_Convert[(UINT8)u2Data];
        #ifndef CC_AUD_NEW_AQ_PARSER            
            for (u1ChIdx = AUD_CH_FRONT_LEFT; u1ChIdx<=AUD_CH_DMX_RIGHT; u1ChIdx++)
            {
                _aau1ChannelVolume[u1DecId][(AUD_CH_T)u1ChIdx] = _au1AQChVolume[eConvertSrc][(AUD_CH_T)u1ChIdx];
                _AUD_DspVolumeChange(u1DecId, (AUD_CH_T)u1ChIdx);
            }
        #else
            for (u1ChIdx = AUD_CH_FRONT_LEFT; u1ChIdx<=AUD_CH_FRONT_RIGHT; u1ChIdx++)
            {
                _aau1ChannelVolume[u1DecId][(AUD_CH_T)u1ChIdx] = _au1AQChVolume[eConvertSrc][(AUD_CH_T)u1ChIdx];
                _AUD_DspVolumeChange(u1DecId, (AUD_CH_T)u1ChIdx);
            }
            for (u1ChIdx = AUD_CH_BYPASS_LEFT; u1ChIdx<=AUD_CH_BYPASS_RIGHT; u1ChIdx++)
            {
                _aau1ChannelVolume[u1DecId][(AUD_CH_T)u1ChIdx] = _au1AQChVolume[eConvertSrc][(AUD_CH_T)u1ChIdx - 4];
                _AUD_DspVolumeChange(u1DecId, (AUD_CH_T)u1ChIdx);
            }
        #endif // CC_AUD_NEW_AQ_PARSER
        }
        break;            

    case AQ_TYPE_SP_VOL:        
        if (_fgAQ_Enable[AQ_TYPE_SP_VOL])
        {
            for (i= 0;i<AUD_STREAM_FROM_NUM;i++)
            {
                eConvertSrc = _au1AQSrc_Convert[i];
                VERIFY( eConvertSrc < FLASH_AQ_SRC_NUM );
                _aai2InputSrcVol[AUD_DEC_MAIN][i] = (INT16)(_au1AQ_SP_Vol[eConvertSrc]-128);
            }
        }
        break;
        
    case AQ_TYPE_LO_VOL:        
        #ifdef CC_AUD_INPUT_SRC_LINEOUT_VOL
        if (_fgAQ_Enable[AQ_TYPE_LO_VOL])
        {
            for (i= 0;i<AUD_STREAM_FROM_NUM;i++)
            {
                eConvertSrc = _au1AQSrc_Convert[i];
                VERIFY( eConvertSrc < FLASH_AQ_SRC_NUM );
                _aai2InputSrcLineOutVol[AUD_DEC_MAIN][i] = (INT16)(_au1AQ_LO_Vol[eConvertSrc]-128);
            }
        }
        #endif // CC_AUD_INPUT_SRC_LINEOUT_VOL        
        break;
        
    case AQ_TYPE_EQ:
        if ((u1DecId == AUD_DEC_MAIN) && (_fgAQ_Enable[AQ_TYPE_EQ]))
        {
            AUD_DRVGetStreamFrom(u1DecId, &eStreamFrom);
            //The StreamFrom in AQ Table is not in the same order as our emu type.
            eConvertSrc = _au1AQSrc_Comm_Convert[(UINT8)eStreamFrom];
            eqValue[0] = 0;
            for (i = 1; i < CH_EQ_BAND_NO; i++)
            {         
                eqValue[i] = (INT8)_au1AQ_EQLevel[eConvertSrc][u2Data][i] - (EQ_BAND_GAIN_DB_MAX * 2);
            }        
            AUD_DspChSetEqTable(AUD_DEC_MAIN, AUD_EQ_CUST1, eqValue); //Sound mode EQ use CUST1 ...
            _AUD_DspChEqLoadPresetChgMode(AUD_DEC_MAIN, AUD_EQ_CUST1);
        }
        break;
        
    default:
        break;  
    };        

}

#endif /* CC_AUD_USE_FLASH_AQ */

// =================== End of Audio Flash AQ Functions ===============================================


//-----------------------------------------------------------------------------
/** _AUD_DspGetMpegAudInfo
 *   
 *  @param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param *prAudInfo                audio info structure pointer
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspGetMpegAudInfo(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo)
{
    UNUSED(_GetMpegAudInfo(u1DecId, prAudInfo));
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetMp3PreparsingMode
 *   
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetMp3PreparsingMode(BOOL fgEnable)
{
    DSP_SetMp3PreparsingMode(fgEnable);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetMp3PreparsingMode
 *   
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
BOOL _AUD_DspIsMp3Vbr(void)
{
    return DSP_IsMp3Vbr();
}


