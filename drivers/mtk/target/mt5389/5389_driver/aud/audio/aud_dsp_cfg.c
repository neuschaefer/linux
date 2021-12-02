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
 * $Author: xiongfeng.wen $
 * $Date: 2012/12/19 $
 * $RCSfile: aud_dsp_cfg.c,v $
 * $Revision: #23 $
 *
 *---------------------------------------------------------------------------*/

/** @file aud_dsp_cfg.c
 *  Primitive APIs for audio and DSP configuration.
 */

#define AUD_EXTERN_FILE

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
//#include "dsp_data.h"
#include "drv_adsp.h"
#include "aud_if.h"
//#include "aud_hw.h"
#include "drvcust_if.h"
#include "adac_if.h"
//#include "drv_dsp_cfg.h"
//#include "dsp_func.h"
#include "dsp_table.h"
#include "aud_hw.h"



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

#define AUD_ADPAN_CH_NUM    2

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define VOL_CTL_SEMA_LOCK(u1DecId)      VERIFY(x_sema_lock(_ahVolCtlSema[u1DecId], X_SEMA_OPTION_WAIT) == OSR_OK)
#define VOL_CTL_SEMA_UNLOCK(u1DecId)    VERIFY(x_sema_unlock(_ahVolCtlSema[u1DecId]) == OSR_OK)

UINT8 _au1MWSetSoundModeIdx = 0xff;

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
extern void bHDMIAudioSigmaDeltaFineTuneSetting(UINT32 value1, UINT8 value2, UINT32 target, UINT8 bSDINOVR);
extern UINT32 DSP_GetAtvTvSystemStrength(TV_AUD_SYS_T eAudSys);
#ifdef SUPPORT_PHILIPS_LOUDNESS
extern void AUD_DspChEqLoudness(UINT8 u1DecId, INT8 *prEqvalue);
#endif
extern void vHDMISpdifOutEnable(BOOL fgEnable);
extern void vHDMIAudChannelStatus(BOOL* pfgCopyright, UINT8* pu1CategoryCode,
                                UINT8* p1WordLength, BOOL* pfgIsPcm);
extern AUD_WMAErr_NOTIFY _hWMAErrNotifyFunc;
#ifdef CC_53XX_SWDMX_V2
extern VOID *_prWMAErrInst;  // megaa 20090828
#endif
extern UINT32 u4ReadDspSram(UINT32 u4Addr);

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
#ifdef CC_AUD_MIXSOUND_SUPPORT
UINT32 _u4MixsoundVol = 10;
UINT32 _u4MixsoundLoop = 64;
#endif

// Dolby banner
static BOOL  fgBannerEnable = FALSE;

static HANDLE_T _ahVolCtlSema[AUD_DEC_NUM];
static HANDLE_T _ahEqCtlSema;
static HANDLE_T _ahChDelayCtlSema;
#ifdef CC_AUD_SOUND_MODE
static HANDLE_T _ahPSMCtlSema;
#endif
#ifdef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
static HANDLE_T _ahVSMCtlSema;
#endif
static HANDLE_T _ahSpdifCtlSema;
static HANDLE_T _hAsrcControlSema;
#if 0 //ndef CC_MT5391_AUD_3_DECODER
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
static INT16  _i2DVDSpdifVol = 24;
#ifdef CC_AUD_NEW_CV_TABLE
static UINT32 _au4CHVOLUME[MAX_VOL_LEVEL + 1];
#endif
#ifdef CC_AUD_SOUND_MODE
static UINT16 _aai1PeqFq[AUD_SM_NUM][PEQ_SETTING_MAX];
static UINT16 _aai1PeqBw[AUD_SM_NUM][PEQ_SETTING_MAX];
static INT8 _aai1PeqGain[AUD_SM_NUM][PEQ_SETTING_MAX];
static UINT8 _aai1BassTreble[AUD_SM_NUM][2];
#endif
static AUD_POST_SM_T _arSoundModeCase[AUD_SOUND_MODE_NUM][AUD_STREAM_FROM_NUM];
static INT8 _aaiAvcExtraGain[2][(UINT32)AUD_CH_NUM] ;
static UINT8 _au1SoundModeCurrentIdx = 0xff;

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
#ifdef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
static UINT32 _aai1VSMode[AUD_VS_NUM][AUD_SUR_PRAR_NUM];
#endif
static UINT16 _au1ChDelay[AUD_STREAM_FROM_NUM];
static INT16 _ai2AvcTargetLevel[AUD_STREAM_FROM_NUM];
static INT16 _ai2AvcGainUp[AUD_STREAM_FROM_NUM];
static INT16 _ai2AvcAdjRate[AUD_STREAM_FROM_NUM];
static INT16 _ai2AvcUiAdjRate[AUD_STREAM_FROM_NUM];
static UINT8 _u1IecCopyright=0x0;
static UINT8 _u1IecCategory=0x8c;

// For SPDIF
typedef struct _AUD_SPDIF_INFO_T
{
    UINT8 u1Copyright[SPDIF_REG_TYPE_NUM][AUD_STREAM_FROM_NUM];
    UINT8 u1Category[SPDIF_REG_TYPE_NUM][AUD_STREAM_FROM_NUM];
    UINT8 u1WordLength[SPDIF_REG_TYPE_NUM][AUD_STREAM_FROM_NUM];
    UINT8 eUpdateMode[SPDIF_REG_TYPE_NUM][AUD_STREAM_FROM_NUM];
}   AUD_SPDIF_INFO_T;

static AUD_SPDIF_INFO_T _aSpdifInfo;

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

static BOOL   _fgAtvMute= FALSE;

#ifdef CC_AUD_TV_SYS_FINE_TUNE_VOL
static UINT8  _aau1TVFineTuneVolume[SV_AUD_SYS_NUM];
#endif

static BOOL  _fgVideoScrambled = FALSE;

// used for ATV factory mode fine tune volume
static UINT8 _u1MtsFineVol=20;
static UINT8 _u1SapFineVol=20;
static UINT8 _u1A2FineVol=20;
static UINT8 _u1PalFineVol=20;
static UINT8 _u1AMFineVol=20;
static UINT8 _u1NicamFineVol=20;
static UINT8 _u1FmfmMonoFineVol=20;
static UINT8 _u1FmfmDualFineVol=20;

#ifdef CC_AUD_BBE_SUPPORT
static UINT8 _u1BbeMode = 0;
static INT8 _i1BbeLevel=0;
static UINT8 _u1BbeProcess=7*2;
static UINT8 _u1BbeLoContour=7*2;
static UINT8 _u1Bbe3dGain=80;
#endif

#ifdef CC_AUD_MIXSOUND_SUPPORT
BOOL _fgMixSndFlag = FALSE;
BOOL _fgMixSndMute = FALSE;
UINT8 _u1MixSndClipIdx = 0xff;
#endif

#ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
// Q index for on/off
static UINT8 _au1CDNotchQIdx[2] = {16, 0};
static UINT16 _au1CDNotchFc[2] = {1000, 0};
#endif

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
static UINT8 _u1AdFadeVolume;
static UINT8 _u1AdFadeVolumePrev;
static UINT8 _aau1AdPanVol[AUD_ADPAN_CH_NUM];
static UINT8 _aau1AdPanVolPrev[AUD_ADPAN_CH_NUM];
static UINT16 _u2AdFadeMode;
static INT16 _aai1ChlVolExtraGain[AUD_DEC_NUM][(UINT32)AUD_CH_NUM];
static INT8 _aai1ChlVolDolbyGainTB11[(UINT32)AUD_CH_NUM]; // Dolby Technical Bulletin 11.5
static UINT32 _aau4ChannelVolumeRawData[AUD_DEC_NUM][(UINT32)AUD_CH_NUM];
static BOOL  _aafgChlMute[AUD_DEC_NUM][(UINT32)AUD_CH_NUM];
static BOOL  _aafgChlOutPort[AUD_DEC_NUM][(UINT32)AUD_CH_NUM];
static BOOL _aafgSoundModeMute[AUD_DEC_NUM][(UINT32)AUD_CH_NUM];
static BOOL  _aafgChlHdmiChgFmtMute[AUD_DEC_NUM][(UINT32)AUD_CH_NUM];
#ifdef CC_AUD_HDMI_SPDIF_CFG
static BOOL  _aafgChlHdmiMuteAnalogOut[AUD_DEC_NUM][(UINT32)AUD_CH_NUM];
#endif
#ifdef CC_ENABLE_AV_SYNC
static BOOL  _aafgChDecPlayMute[AUD_DEC_NUM][(UINT32)AUD_CH_NUM];
#endif
static BOOL  _aafgChAtvMute[AUD_DEC_NUM][(UINT32)AUD_CH_NUM];
#ifdef CC_AUD_LINEOUT_VOL_OFFSET
static UINT8 _aau1ChannelVolOffset[AUD_DEC_NUM][(UINT32)AUD_CH_NUM];
#endif
static AUD_IEC_T _aeIecMode = AUD_IEC_CFG_PCM;
static AUD_IEC_RAWSRC_T _aeIecRawSrc = IEC_RAW_SRC_OFF;
static BOOL _afgIecEnable = TRUE;
static BOOL _afgIecMute = FALSE;
static AUD_IECINFO_T _gSetIecInfo;

static AUD_EQ_TYPE_T _eEqMode = AUD_EQ_CUST1;

#if 0 //ndef CC_MT5391_AUD_3_DECODER
static UINT32 _au1Balance[2] = {(MAX_BALANCE_LEVEL/2), (MAX_BALANCE_LEVEL/2)};
static BOOL _afgMuteEnable[AUD_DEC_NUM] = {FALSE, FALSE};
static BOOL _afgStopMuteEnable[AUD_DEC_NUM] = {FALSE, FALSE};
static BOOL _afgHdmiModeMuteEnable[AUD_DEC_NUM] = {FALSE, FALSE};
static BOOL _afgAgcEnable[AUD_DEC_NUM] = {TRUE, FALSE};
static BOOL _afgUserAgcFlag[AUD_DEC_NUM] = {TRUE, FALSE};
static AUD_DEC_STREAM_FROM_T _aeStreamSrc[AUD_DEC_NUM] = {AUD_STREAM_FROM_OTHERS, AUD_STREAM_FROM_OTHERS};
static BOOL _afgAllChannelMute[AUD_DEC_NUM] = {FALSE, FALSE};
#else
static UINT32 _au1Balance[3] = {(MAX_BALANCE_LEVEL/2), (MAX_BALANCE_LEVEL/2), (MAX_BALANCE_LEVEL/2)};
static BOOL _afgMuteEnable[AUD_DEC_NUM] = {FALSE, FALSE, FALSE};
static BOOL _afgStopMuteEnable[AUD_DEC_NUM] = {FALSE, FALSE, FALSE};
static BOOL _afgHdmiModeMuteEnable[AUD_DEC_NUM] = {FALSE, FALSE, FALSE};
static BOOL _afgAgcEnable[AUD_DEC_NUM] = {TRUE, FALSE, FALSE};
static BOOL _afgUserAgcFlag[AUD_DEC_NUM] = {TRUE, FALSE, FALSE};
static BOOL _fgHdmiMuteDelay = TRUE;
static AUD_DEC_STREAM_FROM_T _aeStreamSrc[AUD_DEC_NUM] = {AUD_STREAM_FROM_OTHERS, AUD_STREAM_FROM_OTHERS,AUD_STREAM_FROM_OTHERS};
static BOOL _afgAllChannelMute[AUD_DEC_NUM] = {FALSE, FALSE, FALSE};
#endif
// Negtive dB table, in steps of 0.5 dB
// 0dB, -0.5dB, -1dB, -1.5dB, -2dB, -2.5dB,.....
static const UINT32 _au4NegDBTable[] = {
    0x00020000,0x0001f8af,0x0001f178,0x0001ea5c,0x0001e35a,0x0001dc72,
    0x0001d5a3,0x0001ceed,0x0001c84f,0x0001c1c9,0x0001bb5b,0x0001b505,
    0x0001aec6,0x0001a89e,0x0001a28c,0x00019c91,0x000196ab,0x000190db,
    0x00018b20,0x0001857a,0x00017fe9,0x00017a6c,0x00017503,0x00016fae,
    0x00016a6d,0x0001653f,0x00016024,0x00015b1b,0x00015625,0x00015141,
    0x00014c6f,0x000147af,0x00014300,0x00013e62,0x000139d5,0x00013559,
    0x000130ed,0x00012c91,0x00012845,0x00012409,0x00011fdc,0x00011bbf,
    0x000117b1,0x000113b1,0x00010fc0,0x00010bde,0x0001080a,0x00010444,
    0x0001008c,0x0000fce1,0x0000f944,0x0000f5b4,0x0000f231,0x0000eebb,
    0x0000eb51,0x0000e7f4,0x0000e4a3,0x0000e15e,0x0000de25,0x0000daf8,
    0x0000d7d7,0x0000d4c1,0x0000d1b6,0x0000ceb6,0x0000cbc1,0x0000c8d7,
    0x0000c5f8,0x0000c323,0x0000c059,0x0000bd99,0x0000bae3,0x0000b837,
    0x0000b595,0x0000b2fc,0x0000b06d,0x0000ade7,0x0000ab6a,0x0000a8f7,
    0x0000a68c,0x0000a42a,0x0000a1d1,0x00009f81,0x00009d39,0x00009af9,
    0x000098c2,0x00009693,0x0000946c,0x0000924d,0x00009035,0x00008e25,
    0x00008c1d,0x00008a1c,0x00008822,0x00008630,0x00008445,0x00008261,
    0x00008084,0x00007ead,0x00007cdd,0x00007b14,0x00007951,0x00007795,
    0x000075df,0x0000742f,0x00007286,0x000070e3,0x00006f46,0x00006dae,
    0x00006c1c,0x00006a90,0x0000690a,0x00006789,0x0000660e,0x00006498,
    0x00006328,0x000061bd,0x00006057,0x00005ef6,0x00005d9a,0x00005c43,
    0x00005af1,0x000059a4,0x0000585c,0x00005718,0x000055d9,0x0000549e,
    0x00005368,0x00005236,0x00005109,0x00004fe0,0x00004ebb,0x00004d9b,
    0x00004c7f,0x00004b67,0x00004a53,0x00004943,0x00004837,0x0000472e,
    0x00004629,0x00004528,0x0000442b,0x00004331,0x0000423b,0x00004148,
    0x00004059,0x00003f6d,0x00003e85,0x00003da0,0x00003cbe,0x00003bdf,
    0x00003b04,0x00003a2c,0x00003957,0x00003885,0x000037b6,0x000036ea,
    0x00003621,0x0000355b,0x00003497,0x000033d6,0x00003318,0x0000325d,
    0x000031a4,0x000030ee,0x0000303b,0x00002f8a,0x00002edc,0x00002e30,
    0x00002d87,0x00002ce0,0x00002c3b,0x00002b99,0x00002af9,0x00002a5b,
    0x000029c0,0x00002927,0x00002890,0x000027fb,0x00002768,0x000026d7,
    0x00002648,0x000025bb,0x00002530,0x000024a7,0x00002420,0x0000239b,
    0x00002318,0x00002297,0x00002218,0x0000219b,0x00002120,0x000020a6,
    0x0000202e,0x00001fb8,0x00001f43,0x00001ed0,0x00001e5f,0x00001def,
    0x00001d81,0x00001d15,0x00001caa,0x00001c41,0x00001bd9,0x00001b73,
    0x00001b0e,0x00001aab,0x00001a49,0x000019e8,0x00001989,0x0000192b,
    0x000018ce,0x00001873,0x00001819,0x000017c0,0x00001769,0x00001713,
    0x000016be,0x0000166a,0x00001618,0x000015c7,0x00001577,0x00001528,
    0x000014da,0x0000148d,0x00001441,0x000013f6,0x000013ac,0x00001364,
    0x0000131d,0x000012d7,0x00001292,0x0000124e,0x0000120b,0x000011c9,
    0x00001187,0x00001146,0x00001106,0x000010c7,0x00001089,0x0000104c,
    0x00001010,0x00000fd5,0x00000f9b,0x00000f61,0x00000f28,0x00000ef0,
    0x00000eb9,0x00000e83,0x00000e4d,0x00000e18,0x00000de4,0x00000db1,
    0x00000d7e,0x00000d4c,0x00000d1b,0x00000ceb,0x00000cbb,0x00000c8c,
    0x00000c5e,0x00000c30,0x00000c03,0x00000bd7,0x00000bab,0x00000b80,
    0x00000b55,0x00000b2b,0x00000b02,0x00000ad9,0x00000ab1,0x00000a89,
    0x00000a62,0x00000a3c,0x00000a16,0x000009f1,0x000009cc,0x000009a8,
    0x00000984,0x00000961,0x0000093e,0x0000091c,0x000008fa,0x000008d9,
    0x000008b8,0x00000898,0x00000878,0x00000859,0x0000083a,0x0000081b,
    0x000007fd,0x000007df,0x000007c2,0x000007a5,0x00000789,0x0000076d,
    0x00000751,0x00000736,0x0000071b,0x00000701,0x000006e7,0x000006cd,
    0x000006b4,0x0000069b,0x00000682,0x0000066a,0x00000652,0x0000063a,
    0x00000623,0x0000060c,0x000005f5,0x000005df,0x000005c9,0x000005b3,
    0x0000059e,0x00000589,0x00000574,0x00000560,0x0000054c,0x00000538,
    0x00000524,0x00000511,0x000004fe,0x000004eb,0x000004d9,0x000004c7,
    0x000004b5,0x000004a3,0x00000492,0x00000481,0x00000470,0x0000045f,
    0x0000044f,0x0000043f,0x0000042f,0x0000041f,0x0000040f,0x00000400,
    0x000003f1,0x000003e2,0x000003d3,0x000003c5,0x000003b7,0x000003a9,
    0x0000039b,0x0000038d,0x00000380,0x00000373,0x00000366,0x00000359,
    0x0000034c,0x0000033f,0x00000333,0x00000327,0x0000031b,0x0000030f,
    0x00000303,0x000002f7,0x000002ec,0x000002e1,0x000002d6,0x000002cb,
    0x000002c0,0x000002b5,0x000002ab,0x000002a1,0x00000297,0x0000028d,
    0x00000283,0x00000279,0x0000026f,0x00000266,0x0000025d,0x00000254,
    0x0000024b,0x00000242,0x00000239,0x00000230,0x00000227,0x0000021f,
    0x00000217,0x0000020f,0x00000207,0x000001ff,0x000001f7,0x000001ef,
    0x000001e7,0x000001e0,0x000001d9,0x000001d2,0x000001cb,0x000001c4,
    0x000001bd,0x000001b6,0x000001af,0x000001a8,0x000001a1,0x0000019b,
    0x00000195,0x0000018f,0x00000189,0x00000183,0x0000017d,0x00000177,
    0x00000171,0x0000016b,0x00000165,0x0000015f,0x00000159,0x00000154,
    0x0000014f,0x0000014a,0x00000145,0x00000140,0x0000013b,0x00000136,
    0x00000131,0x0000012c,0x00000127,0x00000122,0x0000011d,0x00000118,
    0x00000113,0x0000010f,0x0000010b,0x00000107,0x00000103,0x000000ff,
    0x000000fb,0x000000f7,0x000000f3,0x000000ef,0x000000eb,0x000000e7,
    0x000000e3,0x000000df,0x000000db,0x000000d7,0x000000d3,0x000000cf,
    0x000000cc,0x000000c9,0x000000c6,0x000000c3,0x000000c0,0x000000bd,
    0x000000ba,0x000000b7,0x000000b4,0x000000b1,0x000000ae,0x000000ab,
    0x000000a8,0x000000a5,0x000000a2,0x0000009f,0x0000009c,0x00000099,
    0x00000096,0x00000093,0x00000090,0x0000008d,0x0000008a,0x00000088,
    0x00000086,0x00000084,0x00000082,0x00000080,0x0000007e,0x0000007c,
    0x0000007a,0x00000078,0x00000076,0x00000074,0x00000072,0x00000070,
    0x0000006e,0x0000006c,0x0000006a,0x00000068,0x00000066,0x00000064,
    0x00000062,0x00000060,0x0000005e,0x0000005c,0x0000005a,0x00000058,
    0x00000056,0x00000054,0x00000052,0x00000050,0x0000004e,0x0000004c,
    0x0000004a,0x00000048,0x00000046,0x00000044,0x00000043,0x00000042,
    0x00000041,0x00000040,0x0000003f,0x0000003e,0x0000003d,0x0000003c,
    0x0000003b,0x0000003a,0x00000039,0x00000038,0x00000037,0x00000036,
    0x00000035,0x00000034,0x00000033,0x00000032,0x00000031,0x00000030,
    0x0000002f,0x0000002e,0x0000002d,0x0000002c,0x0000002b,0x0000002a,
    0x00000029,0x00000028,0x00000027,0x00000026,0x00000025,0x00000024,
    0x00000023,0x00000022,0x00000021,0x00000020,0x0000001f,0x0000001e,
    0x0000001d,0x0000001c,0x0000001b,0x0000001a,0x00000019,0x00000018,
    0x00000017,0x00000016,0x00000015,0x00000014,0x00000013,0x00000012,
    0x00000011,0x00000010,0x0000000f,0x0000000e,0x0000000d,0x0000000c,
    0x0000000b,0x0000000a,0x00000009,0x00000008,0x00000007,0x00000006,
    0x00000005,0x00000004,0x00000003,0x00000002,0x00000001,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000

};

// Postive dB table, in steps of 0.5 dB
// 0dB, 0.5dB, 1dB, 1.5dB, 2dB, 2.5dB,.....
static const UINT32 _au4PosDBTable[] = {
    0x00020000,0x0002076b,0x00020ef2,0x00021695,0x00021e54,0x00022630,
    0x00022e29,0x00023640,0x00023e75,0x000246c8,0x00024f3a,0x000257cb,
    0x0002607c,0x0002694d,0x0002723f,0x00027b52,0x00028487,0x00028dde,
    0x00029758,0x0002a0f5,0x0002aab6,0x0002b49b,0x0002bea5,0x0002c8d4,
    0x0002d329,0x0002dda4,0x0002e846,0x0002f30f,0x0002fe00,0x0003091a,
    0x0003145d,0x00031fca,0x00032b61,0x00033723,0x00034311,0x00034f2b,
    0x00035b72,0x000367e7,0x0003748a,0x0003815c,0x00038e5d,0x00039b8f,
    0x0003a8f2,0x0003b686,0x0003c44d,0x0003d247,0x0003e075,0x0003eed7,
    0x0003fd6f,0x00040c3d,0x00041b42,0x00042a7e,0x000439f3,0x000449a1,
    0x0004598a,0x000469ae,0x00047a0d,0x00048aa9,0x00049b83,0x0004ac9b,
    0x0004bdf3,0x0004cf8b,0x0004e164,0x0004f380,0x000505df,0x00051882,
    0x00052b6a,0x00053e98,0x0005520e,0x000565cc,0x000579d3,0x00058e24,
    0x0005a2c1,0x0005b7aa,0x0005cce1,0x0005e267,0x0005f83c,0x00060e62,
    0x000624db,0x00063ba7,0x000652c8,0x00066a3e,0x0006820b,0x00069a31,
    0x0006b2b0,0x0006cb8a,0x0006e4c1,0x0006fe55,0x00071848,0x0007329b,
    0x00074d50,0x00076868,0x000783e5,0x00079fc8,0x0007bc12,0x0007d8c5,
    0x0007f5e3,0x0008136d,0x00083164,0x00084fca,0x00086ea1,0x00088deb,
    0x0008ada9,0x0008cddd,0x0008ee88,0x00090fac,0x0009314b,0x00095367,
    0x00097602,0x0009991d,0x0009bcba,0x0009e0db,0x000a0583,0x000a2ab3,
    0x000a506d,0x000a76b3,0x000a9d87,0x000ac4eb,0x000aece1,0x000b156b,
    0x000b3e8c,0x000b6845,0x000b9299,0x000bbd8a,0x000be91a,0x000c154c,
    0x000c4222,0x000c6f9e,0x000c9dc3,0x000ccc93,0x000cfc11,0x000d2c3f,
    0x000d5d20,0x000d8eb7,0x000dc106,0x000df40f,0x000e27d6,0x000e5c5d,
    0x000e91a7,0x000ec7b6,0x000efe8e,0x000f3632,0x000f6ea4,0x000fa7e7,
    0x000fe1ff,0x00101cee,0x001058b8,0x00109560,0x0010d2e9,0x00111156,
    0x001150ab,0x001190eb,0x0011d21a,0x0012143a,0x00125750,0x00129b5f,
    0x0012e06a,0x00132675,0x00136d84,0x0013b59b,0x0013febe,0x001448f0,
    0x00149435,0x0014e092,0x00152e0a,0x00157ca1,0x0015cc5c,0x00161d3f,
    0x00166f4e,0x0016c28e,0x00171703,0x00176cb1,0x0017c39d,0x00181bcb,
    0x00187541,0x0018d003,0x00192c15,0x0019897d,0x0019e840,0x001a4862,
    0x001aa9e9,0x001b0cda,0x001b713a,0x001bd70f,0x001c3e5d,0x001ca72b,
    0x001d117e,0x001d7d5b,0x001deac8,0x001e59cc,0x001eca6b,0x001f3cac,
    0x001fb095,0x0020262c,0x00209d78,0x0021167e,0x00219146,0x00220dd5,
    0x00228c32,0x00230c64,0x00238e72,0x00241263,0x0024983d,0x00252008,
    0x0025a9cb,0x0026358d,0x0026c356,0x0027532d,0x0027e51a,0x00287924,
    0x00290f53,0x0029a7b0,0x002a4242,0x002adf12,0x002b7e27,0x002c1f8b,
    0x002cc346,0x002d6960,0x002e11e3,0x002ebcd7,0x002f6a45,0x00301a37,
    0x0030ccb6,0x003181cb,0x00323980,0x0032f3df,0x0033b0f1,0x003470c1,
    0x00353359,0x0035f8c3,0x0036c109,0x00378c37,0x00385a56,0x00392b72,
    0x0039ff96,0x003ad6ce,0x003bb124,0x003c8ea4,0x003d6f5a,0x003e5352,
    0x003f3a98,0x00402538,0x0041133f,0x004204b9,0x0042f9b3,0x0043f23a,
    0x0044ee5c,0x0045ee25,0x0046f1a3,0x0047f8e4,0x004903f6,0x004a12e7,
    0x004b25c6,0x004c3ca1,0x004d5786,0x004e7685,0x004f99ad,0x0050c10d,
    0x0051ecb6,0x00531cb6,0x0054511f,0x00558a00,0x0056c76a,0x0058096e,
    0x0059501d,0x005a9b88,0x005bebc1,0x005d40da,0x005e9ae4,0x005ff9f2,
    0x00615e17,0x0062c766,0x006435f1,0x0065a9cc,0x0067230b,0x0068a1c2,
    0x006a2605,0x006bafe9,0x006d3f83,0x006ed4e8,0x0070702d,0x00721168,
    0x0073b8af,0x00756619,0x007719bc,0x0078d3b0,0x007a940c,0x007c5ae8,
    0x007e285c,0x007ffc80,0x0081d76d,0x0083b93c,0x0085a207,0x008791e8,
    0x008988f9,0x008b8755,0x008d8d17,0x008f9a5a,0x0091af3a,0x0093cbd4,
    0x0095f044,0x00981ca7,0x009a511a,0x009c8dbc,0x009ed2ab,0x00a12006,
    0x00a375ec,0x00a5d47c,0x00a83bd7,0x00aaac1d,0x00ad2570,0x00afa7f1,
    0x00b233c2,0x00b4c906,0x00b767e0,0x00ba1073,0x00bcc2e4,0x00bf7f57,
    0x00c245f1,0x00c516d8,0x00c7f232,0x00cad826,0x00cdc8db,0x00d0c479,
    0x00d3cb29,0x00d6dd14,0x00d9fa63,0x00dd2341,0x00e057d8,0x00e39854,
    0x00e6e4e1,0x00ea3dac,0x00eda2e3,0x00f114b3,0x00f4934b,0x00f81edb,
    0x00fbb793,0x00ff5da3,0x0103113d,0x0106d294,0x010aa1da,0x010e7f43,
    0x01126b03,0x01166550,0x011a6e60,0x011e8669,0x0122ada3,0x0126e447,
    0x012b2a8d,0x012f80b0,0x0133e6ea,0x01385d77,0x013ce494,0x01417c7e,
    0x01462574,0x014adfb5,0x014fab81,0x01548919,0x015978bf,0x015e7ab6,
    0x01638f42,0x0168b6a8,0x016df12e,0x01733f1b,0x0178a0b7,0x017e164b,
    0x0183a021,0x01893e84,0x018ef1c1,0x0194ba25,0x019a97fe,0x01a08b9c,
    0x01a69550,0x01acb56c,0x01b2ec43,0x01b93a29,0x01bf9f74,0x01c61c7a,
    0x01ccb194,0x01d35f1b,0x01da256a,0x01e104dc,0x01e7fdcf,0x01ef10a2,
    0x01f63db4,0x01fd8567,0x0204e81e,0x020c663d,0x0214002a
};


// Fade dB table, -0.3dB per step
static const UINT32 _au4FadeDBTable[] = {
    0x00020000, 0x1EE9E, 0x1DDD3, 0x1CD9A, 0x1BDEE, 0x1AECB,
    0x1A02B, 0x1920A, 0x18464, 0x17734, 0x16A77, 0x15E29,
    0x15246, 0x146CA, 0x13BB2, 0x130FA, 0x126A0, 0x11C9F,
    0x112F5, 0x109A0, 0x1009B, 0xF7E5, 0xEF7A, 0xE759,
    0xDF7F, 0xD7E8, 0xD094, 0xC97F, 0xC2A8, 0xBC0C,
    0xB5AA, 0xAF7F, 0xA98A, 0xA3C8, 0x9E39, 0x98DA,
    0x93A9, 0x8EA6, 0x89CE, 0x8520, 0x809B, 0x7C3E,
    0x7806, 0x73F3, 0x7003, 0x6C35, 0x6889, 0x64FC,
    0x618F, 0x5E3F, 0x5B0C, 0x57F4, 0x54F8, 0x5216,
    0x4F4C, 0x4C9B, 0x4A01, 0x477E, 0x4511, 0x42B8,
    0x4074, 0x3E44, 0x3C27, 0x3A1C, 0x3823, 0x363B,
    0x3464, 0x329D, 0x30E5, 0x2F3C, 0x2DA1, 0x2C15,
    0x2A96, 0x2923, 0x27BE, 0x2665, 0x2517, 0x23D4,
    0x229D, 0x2170, 0x204E, 0x1F35, 0x1E26, 0x1D20,
    0x1C22, 0x1B2E, 0x1A42, 0x195D, 0x1881, 0x17AC,
    0x16DE, 0x1618, 0x1557, 0x149E, 0x13EB, 0x133E,
    0x1296, 0x11F5, 0x1159, 0x10C2, 0x1030, 0xFA4,
    0xF1C, 0xE98, 0xE1A, 0xD9F, 0xD29, 0xCB6,
    0xC48, 0xBDD, 0xB76, 0xB12, 0xAB2, 0xA55,
    0x9FB, 0x9A4, 0x951, 0x900, 0x8B1, 0x866,
    0x81D, 0x7D6, 0x792, 0x750, 0x711, 0x6D3,
    0x698, 0x65F, 0x627, 0x5F2, 0x5BE, 0x58C,
    0x55C, 0x52D, 0x500, 0x4D5, 0x4AB, 0x482,
    0x45B, 0x435, 0x411, 0x3ED, 0x3CB, 0x3AA,
    0x38A, 0x36C, 0x34E, 0x331, 0x315, 0x2FA,
    0x2E1, 0x2C8, 0x2AF, 0x298, 0x281, 0x26C,
    0x257, 0x242, 0x22F, 0x21C, 0x209, 0x1F8,
    0x1E6, 0x1D6, 0x1C6, 0x1B7, 0x1A8, 0x199,
    0x18B, 0x17E, 0x171, 0x164, 0x158, 0x14D,
    0x141, 0x136, 0x12C, 0x122, 0x118, 0x10E,
    0x105, 0xFC, 0xF4, 0xEB, 0xE3, 0xDC,
    0xD4, 0xCD, 0xC6, 0xBF, 0xB9, 0xB2,
    0xAC, 0xA6, 0xA1, 0x9B, 0x96, 0x91,
    0x8C, 0x87, 0x83, 0x7E, 0x7A, 0x76,
    0x72, 0x6E, 0x6A, 0x66, 0x63, 0x60,
    0x5C, 0x59, 0x56, 0x53, 0x50, 0x4E,
    0x4B, 0x48, 0x46, 0x44, 0x41, 0x3F,
    0x3D, 0x3B, 0x39, 0x37, 0x35, 0x33,
    0x31, 0x30, 0x2E, 0x2C, 0x2B, 0x29,
    0x28, 0x27, 0x25, 0x24, 0x23, 0x22,
    0x20, 0x1F, 0x1E, 0x1D, 0x1C, 0x1B,
    0x1A, 0x19, 0x18, 0x18, 0x17, 0x16,
    0x15, 0x15, 0x14, 0
};

// Pan dB table
static const UINT32 _au4PanDBTable[] = {
    0x20000,
    0x1CF5C,
    0x1A326,
    0x17ADA,
    0x155F4,
    0x1340C,
    0x114DD,
    0xF809,
    0xDD5E,
    0xC4A3,
    0xADA0,
    0x9833,
    0x843A,
    0x718B,
    0x6010,
    0x4FAC,
    0x404B,
    0x31D8,
    0x243F,
    0x1772,
    0xB61,
    0
};

static UINT32 _u4FadeDBTableSz = sizeof (_au4FadeDBTable)/sizeof(UINT32);
static UINT32 _u4PanDBTableSz = sizeof (_au4PanDBTable)/sizeof(UINT32);
static UINT32 _u4PosDBTableSz = sizeof(_au4PosDBTable)/sizeof(UINT32);
static UINT32 _u4NegDBTableSz = sizeof (_au4NegDBTable)/sizeof(UINT32);

#if 0 //ndef CC_MT5391_AUD_3_DECODER
static UINT32 _au4MasterVolShm[AUD_DEC_NUM] = {VOL_SHM_0_DB, VOL_SHM_0_DB};
#else
static UINT32 _au4MasterVolShm[AUD_DEC_NUM] = {VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB};
#endif
static BOOL   _fgMasterVolUseShmRawData = FALSE;
static BOOL   _fgChannelVolUseShmRawData = FALSE;
#ifdef SUPPORT_PHILIPS_LOUDNESS
static BOOL   _fgLoudnessEnable = FALSE;
#endif

static INT32 _i4AsrcBaseFreq = ASRC_BASE_FREQ_48K;
static INT32 _i4AsrcSmartVCXOAdjustLevel = 0;
static INT32 _i4AsrcSmartVCXOB2rtLevel = 0;
static INT32 _i4AsrcAVSyncAdjustLevel = 0;

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
    if (eStreamFrom >= AUD_STREAM_FROM_NUM)
    {
        LOG(0, "Incorrect input src setting!\n");
        ASSERT(0);
    }

    return eStreamFrom;
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
    if (i4Level >= (INT32)(_u4PosDBTableSz-1))
    {
        i4Val = (INT32)(_u4PosDBTableSz-1);
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

static UINT32 _DbToShmFade(INT32 i4Level)
{
    INT32 i4Val;

    // Check max and min value
    if (i4Level >= (INT32)_u4FadeDBTableSz)
    {
        i4Val = (INT32)_u4FadeDBTableSz;
    }
    else
    {
        i4Val = i4Level;
    }

    return _au4FadeDBTable[i4Val];
}

static UINT32 _DbToShmPan(INT32 i4Level)
{
    INT32 i4Val;

    // Check max and min value
    if (i4Level >= (INT32)_u4PanDBTableSz)
    {
        i4Val = (INT32)_u4PanDBTableSz;
    }
    else if (i4Level < (-(INT32)_u4PanDBTableSz))
    {
        i4Val = -(INT32)_u4PanDBTableSz;
    }
    else
    {
        i4Val = i4Level;
    }

    if ( i4Val < 0 )
		i4Val = -i4Val;

    return _au4PanDBTable[i4Val];
}

static UINT8 _BalancedVolume(AUD_CH_T eChannel, UINT8 u1Balance, UINT8 u1Volume)
{
    UINT32 u4BalancedVolume = u1Volume;

    if (u1Balance != (MAX_BALANCE_LEVEL/2))
    {
        if (u1Balance < (MAX_BALANCE_LEVEL/2))
        {
            if ((eChannel == AUD_CH_FRONT_RIGHT) ||
                ((eChannel == AUD_CH_DMX_RIGHT) &&
                 ((UINT8)DRVCUST_OptGet(eAudioDownmixWithBalance)==TRUE)))
            {
                u4BalancedVolume = ((u1Balance*2) * u1Volume) / MAX_VOL_LEVEL;
            }
        }
        else
        {
            if ((eChannel == AUD_CH_FRONT_LEFT) ||
                ((eChannel == AUD_CH_DMX_LEFT) &&
                 ((UINT8)DRVCUST_OptGet(eAudioDownmixWithBalance)==TRUE)))
            {
                u4BalancedVolume = (((MAX_VOL_LEVEL - u1Balance)*2) * u1Volume) / MAX_VOL_LEVEL;
            }
        }
    }

    return (UINT8)u4BalancedVolume;
}

#ifdef CC_AUD_NCSTOOL_SUPPORT
void _VolumeCtrl(UINT8 u1DecId, AUD_CH_T eChannel, UINT32 u4Value)
#else
static void _VolumeCtrl(UINT8 u1DecId, AUD_CH_T eChannel, UINT32 u4Value)
#endif
{
    UINT16 u2ShmIndex = 0;
    UINT16 u2UopIndex = 0; // invalid setting
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
    else if (u1DecId == AUD_DEC_AUX)
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
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u2ShmIndex = D_VOL_DEC3;
        u2UopIndex = UOP_DSP_MASTER_VOLUME_DEC3;
        switch(eChannel)
         {
             case AUD_CH_FRONT_LEFT:
                 u2ShmIndex = D_TRIM_L_DEC3;
                 u2UopIndex = UOP_DSP_TRIM_L_DEC3;
                 break;
             case AUD_CH_FRONT_RIGHT:
                 u2ShmIndex = D_TRIM_R_DEC3;
                 u2UopIndex = UOP_DSP_TRIM_R_DEC3;
                 break;
             case AUD_CH_ALL: /* MASTER_VOLUME */
                 u2ShmIndex = D_VOL_DEC3;
                 u2UopIndex = UOP_DSP_MASTER_VOLUME_DEC3;
                 break;

             default:
                 return;
         }
    }
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    else if (u1DecId == AUD_DEC_4TH)
    {
        u2ShmIndex = D_VOL_DEC4;
        u2UopIndex = UOP_DSP_MASTER_VOLUME_DEC4;
    }
#endif

    if (u2UopIndex != 0)
    {
        // valid setting
        vWriteShmUINT32(u2ShmIndex, u4VolumeShm);
        DSP_SendDspTaskCmd(u2UopIndex);
    }
}

static void _AudDspChannelVolChange(UINT8 u1DecId, AUD_CH_T eChl)
{
    AUD_DEC_STREAM_FROM_T eStreamSrc;
    UINT8  u1Value;
    INT8 i1ExtraGain;    
    UINT32 u4Volumn;
    UINT64 u8ChlVol;
    UINT32 u4ChlMute = FALSE;
    UINT32 u4HdmiChgMute = FALSE;
#ifdef CC_AUD_HDMI_SPDIF_CFG
    UINT32 u4HdmiMuteAnalogOut = FALSE;
#endif
#ifdef CC_ENABLE_AV_SYNC
    UINT8 u1DecPlayMute = 1;
#endif

#ifndef CC_MT5391_AUD_3_DECODER
    //20101018,By Daniel,For flash-lite clip using DEC3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return;
    //End
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#endif

    VERIFY(eChl < AUD_CH_NUM);
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
     if(u1DecId == AUD_DEC_MAIN)
     {
         i1ExtraGain = _aai1ChlVolExtraGain[u1DecId][eChl] + _aaiAvcExtraGain[_afgUserAgcFlag[u1DecId]][eChl];
         u8ChlVol = (UINT64)u4Volumn * ((UINT64)(_DbToShm(i1ExtraGain)));
         u4Volumn = (UINT32)(u8ChlVol / VOL_SHM_0_DB);
      }
     else
     {
         u8ChlVol = (UINT64)u4Volumn * ((UINT64)(_DbToShm(_aai1ChlVolExtraGain[u1DecId][eChl])));
         u4Volumn = (UINT32)(u8ChlVol / VOL_SHM_0_DB);
     }
     
     // Caculate DolbyGainTB11
    // u8ChlVol = (UINT64)u4Volumn * ((UINT64)(_DbToShm(_aai1ChlVolDolbyGainTB11[eChl])));
     //u4Volumn = (UINT32)(u8ChlVol / VOL_SHM_0_DB);

    // if play AD and update MAIN DECODER left/right channel volume, apply pan value to MAIN DECODER left/right channel volume
    if ((u1DecId == AUD_DEC_THIRD) && ((eChl == AUD_CH_FRONT_LEFT) || (eChl == AUD_CH_FRONT_RIGHT)))
    {
        // if didn't play AD, don't apply Pan value
        if ((eStreamSrc == AUD_STREAM_FROM_DIGITAL_TUNER) && (AUD_DRVGetAudioState(u1DecId) == AUD_ON_PLAY))
        {
            u8ChlVol = (UINT64)u4Volumn * ((UINT64)_DbToShmPan(_aau1AdPanVol[eChl]));
            u4Volumn = (UINT32)u8Div6432(u8ChlVol, (UINT64)VOL_SHM_0_DB, NULL);
        }
    }

    // Caculate channel mute
    // u4Volumn = _aafgChlMute[u1DecId][eChl] ? 0 : u4Volumn;

    if ((!_aafgChlMute[u1DecId][eChl]) &&
        _aafgChlOutPort[u1DecId][eChl] &&
        (!_afgAllChannelMute[u1DecId]) &&
        (!_aafgSoundModeMute[u1DecId][eChl]))
    {
        u4ChlMute = 1;
    }
    else
    {
        u4ChlMute = 0;
    }

	if ((eChl == AUD_CH_BYPASS_LEFT)||(eChl == AUD_CH_BYPASS_RIGHT))
	{

		VERIFY(eStreamSrc < AUD_STREAM_FROM_NUM);
        //u8ChlVol = (UINT64)u4Volumn * ((UINT64)(_DbToShm(_aai2InputSrcVol[u1DecId][eStreamSrc])));
        if(eStreamSrc == AUD_STREAM_FROM_LINE_IN)//&&(AUD_DspGetAvInputSrcId()==AUD_INPUT_ID_SCART_1))
        {
            u8ChlVol = (UINT64)u4Volumn * ((UINT64)(_DbToShm(_aai2InputSrcVol[u1DecId][eStreamSrc]+_ai2AvInputSrcVol[AUD_DspGetAvInputSrcId()])));
        }
        else
        {
            u8ChlVol = (UINT64)u4Volumn * ((UINT64)(_DbToShm(_aai2InputSrcVol[u1DecId][eStreamSrc])));
        }
        u4Volumn = (UINT32)(u8ChlVol / VOL_SHM_0_DB);
        u4Volumn *= u4ChlMute;

        LOG(7, "Bypass Channle Vol = %x\n", u4Volumn);

#if 0//#ifdef CC_AUD_LINEOUT_VOL_OFFSET


		VERIFY(eChl < AUD_CH_NUM);
        u1Value = _aau1ChannelVolOffset[u1DecId][eChl];
		if (u1Value >MAX_VOL_LEVEL) u1Value = MAX_VOL_LEVEL;

		u8ChlVol = (UINT64)u4Volumn * (UINT64)(_au4CHVOLUME[u1Value]);
        u4Volumn = (UINT32)(u8ChlVol / VOL_SHM_0_DB);

        LOG(7, "Bypass Channle Vol offset = %x\n", u4Volumn);
#endif


	}

    
    if (((eChl == AUD_CH_REAR_LEFT)||(eChl == AUD_CH_REAR_RIGHT))&&(eStreamSrc == AUD_STREAM_FROM_LINE_IN)&&(AUD_DspGetAvInputSrcId()==AUD_INPUT_ID_SCART_1))
    {

        u8ChlVol = (UINT64)u4Volumn * ((UINT64)(_DbToShm(_i2DVDSpdifVol)));
        u4Volumn = (UINT32)(u8ChlVol / VOL_SHM_0_DB);
        u4Volumn *= u4ChlMute;

        LOG(7, "SPDIF Channle Vol = %x\n", u4Volumn);

#if 0//#ifdef CC_AUD_LINEOUT_VOL_OFFSET


        VERIFY(eChl < AUD_CH_NUM);
        u1Value = _aau1ChannelVolOffset[u1DecId][eChl];
        if (u1Value >MAX_VOL_LEVEL) u1Value = MAX_VOL_LEVEL;

        u8ChlVol = (UINT64)u4Volumn * (UINT64)(_au4CHVOLUME[u1Value]);
        u4Volumn = (UINT32)(u8ChlVol / VOL_SHM_0_DB);

        LOG(7, "Bypass Channle Vol offset = %x\n", u4Volumn);
#endif


    }

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

    if ((eStreamSrc == AUD_STREAM_FROM_ANALOG_TUNER)&&_aafgChAtvMute[u1DecId][eChl])
    {
        u4Volumn = 0;
    }

#ifdef CC_AUD_HDMI_SPDIF_CFG
    u4HdmiMuteAnalogOut = _aafgChlHdmiMuteAnalogOut[u1DecId][eChl] ? 0 : 1;
    if (eStreamSrc == AUD_STREAM_FROM_HDMI)
    {
        u4Volumn = u4Volumn * u4HdmiMuteAnalogOut;
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

#if 0 //def CC_AUD_PANFADE_SUPPORT
    // if play Ad and update MAIN decoder master volume, apply fade value to to master volume
    if (u1DecId == AUD_DEC_MAIN)
    {
        // if didn't play AD, don't apply Fade value
        if (eStreamSrc == AUD_STREAM_FROM_DIGITAL_TUNER)
        {
            u8ValueShm = (UINT64)u4ValShm * ((UINT64)(_DbToShmFade(_u1AdFadeVolume)));
            u4ValShm = (UINT32)u8Div6432(u8ValueShm, (UINT64)VOL_SHM_0_DB, NULL);
        }
    }
#endif

    u4ValShm = fgMute ? 0 : u4ValShm;

    // Send UOP to DSP
    _VolumeCtrl(u1DecId, AUD_CH_ALL, u4ValShm);

    // Auto turn off speaker when master volume is 0
    if ((DRVCUST_OptGet(eAudioSpkAutoOffWhenVolMute) == 1) && (u1DecId == AUD_DEC_MAIN))
    //if((u1DecId == AUD_DEC_MAIN)&&_fgHdmiMuteDelay)
    {
        if (u4ValShm == 0)
        {
            // Wait DSP fade out            
            ADAC_GPIOVolMute(TRUE);
            x_thread_delay(50);
            ADAC_VolumeMuteSpeakerEnable(FALSE);
            LOG(8, "**** SPK Auto off due to volume is 0\n");;
        }
        else
        {
            ADAC_VolumeMuteSpeakerEnable(TRUE);            
            ADAC_GPIOVolMute(FALSE);
            LOG(8, "**** SPK Auto on due to volume is not 0\n");;
        }
    }
}

static void _AudDspSrcVolChange(UINT8 u1DecId, UINT32 u4Value)
{
    UINT16 u2ShmIndex = D_INSRC_GAIN;
    UINT16 u2UopIndex = UOP_DSP_INPUT_SRC_GAIN;

#if defined(CC_AUD_4_DECODER_SUPPORT)
    AUD_DEC_ID_VALIDATE(u1DecId);
    #if defined(CC_MT5365) || defined(CC_MT5395) //5368/96 support 4th dec post-proc
    if ((u1DecId == AUD_DEC_4TH)
        #ifndef CC_MT5193_AUD_3_DECODER
        //20101018,By Daniel,for flash-lite clip using dec3 notify path
        ||(u1DecId == AUD_DEC_THIRD)
        //End
        #endif
       )
    {
        return;
    }
    #endif
#elif defined(CC_MT5391_AUD_3_DECODER)
    AUD_DEC_ID_VALIDATE(u1DecId);
#else
    #if defined(CC_MT5365) || defined(CC_MT5395) //5368/96 support 4th dec post-proc
    //20101018,By Daniel,for flash-lite clip using dec3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return;
    //End
    #endif
    AUD_DEC_ID_VALIDATE(u1DecId);
#endif

    if (u1DecId == AUD_DEC_AUX)
    {
        u2ShmIndex = D_INSRC_GAIN_DEC2;
        u2UopIndex = UOP_DSP_INPUT_SRC_GAIN_DEC2;
    }
#ifdef CC_MT5391_AUD_3_DECODER
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u2ShmIndex = D_INSRC_GAIN_DEC3;
        u2UopIndex = UOP_DSP_INPUT_SRC_GAIN_DEC3;
    }
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    else if (u1DecId == AUD_DEC_4TH)
    {
        u2ShmIndex = D_INSRC_GAIN_DEC4;
        u2UopIndex = UOP_DSP_INPUT_SRC_GAIN_DEC4;
    }
#endif

    vWriteShmUINT32(u2ShmIndex, u4Value);
    DSP_SendDspTaskCmd(u2UopIndex);
}

static void _AudDspAgcEnalbe(UINT8 u1DecId, BOOL fgEnable)
{
    UINT16 u2UopIndex = UOP_DSP_AVC_CONTROL;

    AUD_DEC_ID_VALIDATE_2(u1DecId);

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

static void _AudDspIecConvertIecFlag (AUD_IECINFO_T *SetIecInfo, AUD_IEC_T *eIecFlag_p, BOOL *bLoadTrans_p)
{
    AUD_IEC_T eIecFlag;
    BOOL bLoadTrans;
    AUD_FMT_T eDecFormat = SetIecInfo->eDecFormat;
    AUD_IEC_T eIecCfg = SetIecInfo->eIecCfg;
#ifdef CC_MT5391_AUD_3_DECODER
    AUD_DEC_STREAM_FROM_T eStreamSrc = SetIecInfo->eStreamSrc;
#endif

    bLoadTrans = FALSE; // default do not load transcoder
    eIecFlag = AUD_IEC_CFG_PCM; // default PCM

    if (eIecCfg == AUD_IEC_CFG_RAW)
    {
#ifdef CC_MT5391_AUD_3_DECODER
        if (eDecFormat == AUD_FMT_AC3)
        {
            // all input sources, AC3 can output RAW
            eIecFlag = AUD_IEC_CFG_RAW;
        }
        else if (eDecFormat == AUD_FMT_AAC)
        {
            if (IS_SW_SupportDDCO() || IS_SW_SupportMS10())/*check IC bonding*/
            {
                if (eStreamSrc == AUD_STREAM_FROM_DIGITAL_TUNER)
                {
                    // for source = DTV
                    if (AUD_GetSampleFreq(AUD_DEC_AUX) == FS_48K)
                    {
                        if (!(DRVCUST_OptGet(eAudioMultichlDDCO) &&
                        	(u1GetAacChNum(AUD_DEC_AUX) <= 2)))
                        {
                            eIecFlag = AUD_IEC_CFG_RAW;
                            bLoadTrans = TRUE;
                            LOG(5,"Enable DDCO by IEC\n");
                        }
                    }
                }
                else
                {
                    // for source = MM, HDMI...
                    if (AUD_GetSampleFreq(AUD_DEC_MAIN) == FS_48K)
                    {
                        eIecFlag = AUD_IEC_CFG_RAW;
                        bLoadTrans = TRUE;
		      		}
                }
            }
            else
            {
                // IC is not bonded with DDCO
                eIecFlag = AUD_IEC_CFG_RAW;
                bLoadTrans = FALSE;
            }
        }
        else if (eDecFormat == AUD_FMT_DTS)
        {
                eIecFlag = AUD_IEC_CFG_RAW;
                bLoadTrans = TRUE;
                LOG(5, "Enable DTS encoder \n");
        }
#else // CC_MT5391_AUD_3_DECODER
        if (eDecFormat == AUD_FMT_AC3)
        {
                eIecFlag = AUD_IEC_CFG_RAW;
            }
            else if (eDecFormat == AUD_FMT_DTS)
            {
                eIecFlag = AUD_IEC_CFG_RAW;
                bLoadTrans = TRUE;
            }
#endif // CC_MT5391_AUD_3_DECODER
        }

    *eIecFlag_p = eIecFlag;
    *bLoadTrans_p = bLoadTrans;
}

static void _AudDspSetIec(AUD_IEC_T eIecCfg, BOOL fgEnable)
{
    UINT8 u1fgMute;
    AUD_IEC_T eIecFlag;
    BOOL bLoadTrans;
    AUD_DEC_STREAM_FROM_T eStreamSrc;
    AUD_FMT_T eDecFormat;
    SPDIF_REG_TYPE_T eSpdifRegType;
    UINT8 u1Copyright;
    UINT8 u1CategoryCode;
    UINT8 u1WordLength;
    AUD_IEC_RAWSRC_T eRawSource = IEC_RAW_SRC_OFF; // bit0 = dec1, bit1 = dec2
    SPDIF_BURST_INFO_T eBurstInfo;
    UINT16 u2Nsnum;
    AUD_INPUT_ID_T eInputId = _AUD_DspGetAvInputSrcId();

    /* Get stream source and user setting */
    eStreamSrc = _aeStreamSrc[AUD_DEC_MAIN]; // only need to check main decoder
    if (eStreamSrc >= AUD_STREAM_FROM_NUM)
    {
        eStreamSrc = AUD_STREAM_FROM_OTHERS;
    }

    // DVI case, convert stream source
    if ((eStreamSrc == AUD_STREAM_FROM_LINE_IN) &&
        (eInputId >= AUD_INPUT_ID_HDMI_0) && (eInputId <= AUD_INPUT_ID_DVI_4))
    {
        eStreamSrc = AUD_STREAM_FROM_OTHERS;
    }

    _gSetIecInfo.eStreamSrc  = eStreamSrc;
    _gSetIecInfo.eIecCfg= eIecCfg;
    
    if ((_aeStreamSrc[AUD_DEC_MAIN] == AUD_STREAM_FROM_LINE_IN) &&
    (_AUD_DspGetAvInputSrcId() == AUD_INPUT_ID_SCART_1) &&
    (eIecCfg == AUD_IEC_CFG_RAW)&&fgEnable)
    {
        AudSPDIFInBypass(TRUE);
    }
    else
    {
        AudSPDIFInBypass(FALSE);
    }

    /* Get decoder format */
#ifdef  CC_MT5391_AUD_3_DECODER
    switch (eStreamSrc)
    {
    case AUD_STREAM_FROM_DIGITAL_TUNER:
    case AUD_STREAM_FROM_ANALOG_TUNER:
        eDecFormat = _AudGetStrFormat(AUD_DEC_AUX);
        break;
    default:
        eDecFormat = _AudGetStrFormat(AUD_DEC_MAIN);
        break;
    }
#else
    eDecFormat = _AudGetStrFormat(AUD_DEC_MAIN);
#endif
    _gSetIecInfo.eDecFormat = eDecFormat;

    /* Convert IEC type for RAW type */
    _AudDspIecConvertIecFlag (&_gSetIecInfo, &eIecFlag, &bLoadTrans);

    eSpdifRegType = SPDIF_REG_TYPE_USER;
    if (eSpdifRegType >= SPDIF_REG_TYPE_NUM)
    {
        eSpdifRegType = SPDIF_REG_TYPE_DEFAULT;
    }
    _gSetIecInfo.eSpdifRegType = eSpdifRegType;

    // HDMI input, by source, update channel status
    if ((eStreamSrc == AUD_STREAM_FROM_HDMI) &&
     (_aSpdifInfo.eUpdateMode[eSpdifRegType][(UINT8)eStreamSrc] == IEC_CHANNEL_STATUS_BY_SOURCE))
    {
        // HDMI, by source mode, set channel status by HDMI driver
        BOOL fgCopyright, fgIsPcm;
        UINT8 u1WordLengthTmp;
        UNUSED(fgIsPcm);
        UNUSED(u1WordLengthTmp);

        vHDMIAudChannelStatus (&fgCopyright, &u1CategoryCode, &u1WordLengthTmp, &fgIsPcm);
        if (fgCopyright)
        {
            u1Copyright = IEC_COPY_NEVER;
        }
        else
        {
            u1Copyright = IEC_COPY_FREELY;
        }
        _aSpdifInfo.u1Copyright[eSpdifRegType][(UINT8)eStreamSrc] = u1Copyright;
    }

    /* get spdif information */
    u1Copyright = _aSpdifInfo.u1Copyright[eSpdifRegType][(UINT8)eStreamSrc];
    u1CategoryCode = _aSpdifInfo.u1Category[eSpdifRegType][(UINT8)eStreamSrc];
    u1WordLength = _aSpdifInfo.u1WordLength[eSpdifRegType][(UINT8)eStreamSrc];
    
    u1Copyright = IEC_COPY_NEVER;
    u1CategoryCode = IEC_NON_ORIGINAL_BROAD_CAST;

    // setup channel status to shared info....
    vWriteShmUINT8(B_IEC_COPYRIGHT, u1Copyright);
    vWriteShmUINT8(B_IEC_CATEGORY_CODE, u1CategoryCode);
    vWriteShmUINT8(B_IEC_WORD_LENGTH, u1WordLength);

    /* Load transcoder */
    switch (eDecFormat)
    {
    case AUD_FMT_AAC:
        //_AUD_DspSetDDCOEnable(bLoadTrans);
        break;
    case AUD_FMT_DTS:
		if (_AudGetStrFormat(AUD_DEC_AUX) == AUD_FMT_DTS)
            _AUD_DspSetDTSENCEnable(bLoadTrans);
        break;
    default:
        break;
    }

    /* set RAW data output source */
    if (eIecFlag)
    {
        switch (eStreamSrc)
        {
        case AUD_STREAM_FROM_DIGITAL_TUNER:
#ifdef  CC_MT5391_AUD_3_DECODER
            eRawSource = IEC_RAW_SRC_DEC2_ON; // dec1 off, dec2 on
#else
            eRawSource = IEC_RAW_SRC_DEC1_ON; // dec1 on, dec2 off
#endif
            break;
        default:
            eRawSource = IEC_RAW_SRC_DEC1_ON; // dec1 on, dec2 off
            break;
        }
    }

    /*set HDMI */
    if (eStreamSrc == AUD_STREAM_FROM_HDMI)
    {
        fgEnable = _aafgHdmiSetSpdifFlag && _aafgMWSetSpdifFlag && (!_afgIecMute);
    }
    else
    {
        fgEnable = _aafgMWSetSpdifFlag && (!_afgIecMute);
    }
    vHDMISpdifOutEnable(fgEnable);
    u1fgMute = (fgEnable) ? FALSE : TRUE;

    /* set IEC output status */
    {
        LOG (3,"Set SPDIF = %d, rc = 0x%x)\n", eIecFlag, eRawSource);

        vWriteShmUINT8(B_IECFLAG, eIecFlag);
        vWriteShmUINT8(B_IEC_MUTE, u1fgMute);
        vWriteShmUINT8(B_IEC_RAWSRC, (UINT8)eRawSource);

        if (eIecFlag == AUD_IEC_CFG_RAW)
        {
            switch (eDecFormat)
            {
            case AUD_FMT_AC3:
            case 	AUD_FMT_AAC: // DDCO
                eBurstInfo = BURST_INFO_AC3;
                u2Nsnum = 0x600; // 1536 samples
                break;
            case AUD_FMT_DTS:
                eBurstInfo = BURST_INFO_DTS;
                u2Nsnum = 0x200; // 512 samples
                break;
            default: // error case
                eBurstInfo = BURST_INFO_NULL_DATA;
                u2Nsnum = 0x0;
                break;
           }
        }
        else
        { // PCM
            eBurstInfo = BURST_INFO_NULL_DATA;
            u2Nsnum = 0x100; // 256 samples
        }
        vWriteShmUINT16(W_IEC_BURST_INFO, eBurstInfo);
        vWriteShmUINT16(W_IEC_NSNUM, u2Nsnum);
    }

	if ((_aeIecMode != eIecFlag) || (_afgIecEnable != fgEnable) || (_aeIecRawSrc != eRawSource))
    {
        /* set IEC clock */
        if ((eIecFlag==AUD_IEC_CFG_RAW) & (u1fgMute==FALSE))
        {
            AUD_WRITE32(REG_IEC_DIV, IEC_AUDCK_SEL | IEC_CLK_INDEPENDENT_SEL | IEC_LRCK_CYCLE | IEC_UPDATE_SEL_2T); // Sync from [DTV00094645] (IEC_UPDATE_SEL fine tune)
        }
        else
        {
            AUD_WRITE32(REG_IEC_DIV, (AUD_READ32(REG_IEC_DIV) & (~IEC_AUDCK_SEL)) & (~IEC_CLK_INDEPENDENT_SEL));
        }

        DSP_SendDspTaskCmd(UOP_DSP_IEC_FLAG);

        _aeIecMode = eIecFlag;
        _afgIecEnable = fgEnable;
        _aeIecRawSrc = eRawSource;
    }


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

#if defined(CC_AUD_4_DECODER_SUPPORT)
    AUD_DEC_ID_VALIDATE(u1DecId);
    #if defined(CC_MT5365) || defined(CC_MT5395) //5368/96 support 4th dec post-proc
    if ((u1DecId == AUD_DEC_4TH)
        #ifndef CC_MT5391_AUD_3_DECODER
        //20101018,By Daniel,for flash-lite clip using dec3 notify path
        ||(u1DecId == AUD_DEC_THIRD)
        //End
        #endif
        )
        return;
    #endif
#elif defined(CC_MT5391_AUD_3_DECODER)
    AUD_DEC_ID_VALIDATE(u1DecId);
#else
    #if defined(CC_MT5365) || defined(CC_MT5395) //5368/96 support 4th dec post-proc
    //20101018,By Daniel,for flash-lite clip using dec3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return;
    //End
    #endif
    AUD_DEC_ID_VALIDATE(u1DecId);
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
    else if ((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD))
#endif
    {
        _AudDspChannelVolChange(u1DecId, eAudChl);
    }
}

void _AUD_DspCfgSetDecType(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eDecFmt)
{
    UNUSED(eDecFmt);
    AUD_DEC_ID_VALIDATE(u1DecId);

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
    if (u1DecId < AUD_DEC_THIRD)
#endif
    {
        _AUD_DspVolumeChange(u1DecId, AUD_CH_ALL);
        _AUD_DspVolumeChange(u1DecId, AUD_CH_INPUT_SRC);
        _AUD_DspVolumeChange(u1DecId, AUD_CH_BYPASS_LEFT);
        _AUD_DspVolumeChange(u1DecId, AUD_CH_BYPASS_RIGHT);

#ifdef CC_AUD_USE_FLASH_AQ
        _AUD_FlashAQOperation(u1DecId,AQ_TYPE_CH_VOLUME, AQ_OP_EXE, (UINT16)eStreamFrom);
#endif //CC_AUD_USE_FLASH_AQ

    }
    VOL_CTL_SEMA_UNLOCK(u1DecId);
    // DolbyGainTB11 (Dolby Technical Bulletin 11.5)
    LOG(5, "_AUD_DspCfgSetDecType\n");
    _AUD_DspDolbyGainTB11(u1DecId);
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
    else if (u1DecId == AUD_DEC_AUX)
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
#ifdef CC_MT5391_AUD_3_DECODER
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u2ShmIndex = D_VOL_DEC3;
        u2UopIndex = UOP_DSP_MASTER_VOLUME_DEC3;
    }
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    else if (u1DecId == AUD_DEC_4TH)
    {
        u2ShmIndex = D_VOL_DEC4;
        u2UopIndex = UOP_DSP_MASTER_VOLUME_DEC4;
    }
#endif

    vWriteShmUINT32(u2ShmIndex, u4VolumeShm);
    DSP_SendDspTaskCmd(u2UopIndex);
}

UINT32 _AUD_DspGetFineTuneVolume(UINT8 u1DecId, AUD_CH_T eChannel)
{
    UINT16 u2ShmIndex = 0;
    UINT32 u4VolumeShm = 0; /* 0 ~ 0x7fffff */

    if (u1DecId == AUD_DEC_MAIN)
    {
        switch (eChannel)
        {
            case AUD_CH_FRONT_LEFT:
                u2ShmIndex = D_TRIM_L;
                break;
            case AUD_CH_FRONT_RIGHT:
                u2ShmIndex = D_TRIM_R;
                break;
            case AUD_CH_REAR_LEFT:
                u2ShmIndex = D_TRIM_LS;
                break;
            case AUD_CH_REAR_RIGHT:
                u2ShmIndex = D_TRIM_RS;
                break;
            case AUD_CH_CENTER:
                u2ShmIndex = D_TRIM_C;
                break;
            case AUD_CH_SUB_WOOFER:
                u2ShmIndex = D_TRIM_SUB;
                break;
            case AUD_CH_BYPASS_LEFT:
                u2ShmIndex = D_TRIM_CH7;
                break;
            case AUD_CH_BYPASS_RIGHT:
                u2ShmIndex = D_TRIM_CH8;
                break;
            case AUD_CH_DMX_LEFT:
                u2ShmIndex = D_TRIM_CH9;
                break;
            case AUD_CH_DMX_RIGHT:
                u2ShmIndex = D_TRIM_CH10;
                break;
            case AUD_CH_ALL: /* MASTER_VOLUME */
                u2ShmIndex = D_VOL;
                break;
            default:
                break;
        }
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        u2ShmIndex = D_VOL_DEC2;
        switch(eChannel)
        {
            case AUD_CH_FRONT_LEFT:
                u2ShmIndex = D_TRIM_L_DEC2;
                break;
            case AUD_CH_FRONT_RIGHT:
                u2ShmIndex = D_TRIM_R_DEC2;
                break;
            case AUD_CH_ALL: /* MASTER_VOLUME */
            default:
                u2ShmIndex = D_VOL_DEC2;
                break;
        }
    }
#ifdef CC_MT5391_AUD_3_DECODER
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u2ShmIndex = D_VOL_DEC3;
    }
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    else if (u1DecId == AUD_DEC_4TH)
    {
        u2ShmIndex = D_VOL_DEC4;
    }
#endif

    if (u2ShmIndex != 0)
    {
      u4VolumeShm = u4ReadShmUINT32(u2ShmIndex);
    }

    return u4VolumeShm;
}

//-----------------------------------------------------------------------------
/** AUD_DspVolumeInit
 *  Initialize all variables for volume control
 *  @param  u1DecId          Audio decoder id (0: first decoder, 1: second decoder).
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
void _AUD_DspVolumeInit(void)
{
    static BOOL fgInit = FALSE;

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
        #ifdef SUPPORT_FLASH_AQ
        x_memcpy((VOID *)VIRTUAL((UINT32)_aai2InputSrcVol), (const VOID *)(rAudFlashAQ.aai2AudInputSrcVol), sizeof(_aai2InputSrcVol));
        #else
        VERIFY(DRVCUST_OptQuery(eAudioInputVolTable, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_aai2InputSrcVol), (const VOID *)u4Ptr, sizeof(_aai2InputSrcVol));
        #endif
#endif //CC_AUD_USE_NVM

#ifdef CC_AUD_INPUT_SRC_LINEOUT_VOL
        VERIFY(DRVCUST_OptQuery(eAudioInputLineOutVolTable, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_aai2InputSrcLineOutVol), (const VOID *)u4Ptr, sizeof(_aai2InputSrcLineOutVol));
#endif

#ifdef CC_AUD_USE_FLASH_AQ
        _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_SP_VOL, AQ_OP_INIT, 0);
        _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_LO_VOL, AQ_OP_INIT, 0);
#endif // CC_AUD_USE_FLASH_AQ

        VERIFY(DRVCUST_OptQuery(eAudioAvInputSrcVolTable, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_ai2AvInputSrcVol), (const VOID *)u4Ptr, sizeof(_ai2AvInputSrcVol));
#ifdef SUPPORT_FLASH_AQ
        _ai2AvInputSrcVol[AUD_INPUT_ID_SCART_1] = rAudFlashAQ.i2DVDInputSrcGain;
#endif

        VERIFY(DRVCUST_OptQuery(eAudioInputSrcAgcFlag, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_aafgInputSrcAgcFlag), (const VOID *)u4Ptr, sizeof(_aafgInputSrcAgcFlag));

        VERIFY(DRVCUST_OptQuery(eAudioInputSrcAgcVol, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_aau4InputSrcAgcVol), (const VOID *)u4Ptr, sizeof(_aau4InputSrcAgcVol));

#ifndef CC_AUD_USE_FLASH_AQ
    #ifdef SUPPORT_FLASH_AQ
        x_memcpy((VOID *)VIRTUAL((UINT32)_au4VOLUME), (const VOID *)(rAudFlashAQ.au4VolumeTable), sizeof(_au4VOLUME));
        #ifdef CC_AUD_NEW_CV_TABLE
        x_memcpy((VOID *)VIRTUAL((UINT32)_au4CHVOLUME), (const VOID *)(rAudFlashAQ.au4ChannelVolumeTable), sizeof(_au4CHVOLUME));
        #endif
    #else
        VERIFY(DRVCUST_OptQuery(eAudioVolTable, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_au4VOLUME), (const VOID *)u4Ptr, sizeof(_au4VOLUME));
        #ifdef CC_AUD_NEW_CV_TABLE
        VERIFY(DRVCUST_OptQuery(eAudioChannelVolTable, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_au4CHVOLUME), (const VOID *)u4Ptr, sizeof(_au4CHVOLUME));
        #endif
    #endif
#else //CC_AUD_USE_FLASH_AQ
        if(! _fgAQ_Enable[AQ_TYPE_VOL_TABLE] )
        {
            VERIFY(DRVCUST_OptQuery(eAudioVolTable, (UINT32 *)&u4Ptr) == 0);
            x_memcpy((VOID *)VIRTUAL((UINT32)_au4VOLUME), (const VOID *)u4Ptr, sizeof(_au4VOLUME));
        }
        if(! _fgAQ_Enable[AQ_TYPE_LO_VOL_TABLE])
        {
    #ifdef CC_AUD_NEW_CV_TABLE
            VERIFY(DRVCUST_OptQuery(eAudioChannelVolTable, (UINT32 *)&u4Ptr) == 0);
            x_memcpy((VOID *)VIRTUAL((UINT32)_au4CHVOLUME), (const VOID *)u4Ptr, sizeof(_au4CHVOLUME));
    #endif
        }
#endif  // CC_AUD_USE_FLASH_AQ

    #ifdef SUPPORT_FLASH_AQ
        x_memcpy((VOID *)VIRTUAL((UINT32)_aai1ChlVolExtraGain), (const VOID *)rAudFlashAQ.aai1AudChVolExtraGain, sizeof(_aai1ChlVolExtraGain));
        x_memcpy((VOID *)VIRTUAL(_aaiAvcExtraGain), (const VOID *)rAudFlashAQ.aai1AudAVCChVolExtraGain, sizeof(_aaiAvcExtraGain));
    #else
        VERIFY(DRVCUST_OptQuery(eAudioChlVolExtraGain, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_aai1ChlVolExtraGain), (const VOID *)u4Ptr, sizeof(_aai1ChlVolExtraGain));
    #endif

        _u1VolBaseLevel = DRVCUST_OptGet(eAudioVolBaseLevel);

#ifdef CC_AUD_LINEOUT_VOL_MODE

        _aau1LineOutFixVol = DRVCUST_OptGet(eAudioLineOutFixVol);
#endif

#ifdef CC_AUD_USE_FLASH_AQ
        if(! _fgAQ_Enable[AQ_TYPE_AVC] )
        {
            VERIFY(DRVCUST_OptQuery(eAudioAvcTarget, (UINT32 *)&u4Ptr) == 0);
            x_memcpy((VOID *)VIRTUAL((UINT32)_ai2AvcTargetLevel), (const VOID *)u4Ptr, sizeof(_ai2AvcTargetLevel));

            VERIFY(DRVCUST_OptQuery(eAudioAvcGainUp, (UINT32 *)&u4Ptr) == 0);
            x_memcpy((VOID *)VIRTUAL((UINT32)_ai2AvcGainUp), (const VOID *)u4Ptr, sizeof(_ai2AvcGainUp));

            VERIFY(DRVCUST_OptQuery(eAudioAvcAdjustRate, (UINT32 *)&u4Ptr) == 0);
            x_memcpy((VOID *)VIRTUAL((UINT32)_ai2AvcAdjRate), (const VOID *)u4Ptr, sizeof(_ai2AvcAdjRate));

            VERIFY(DRVCUST_OptQuery(eAudioAvcUiAdjustRate, (UINT32 *)&u4Ptr) == 0);
            x_memcpy((VOID *)VIRTUAL((UINT32)_ai2AvcUiAdjRate), (const VOID *)u4Ptr, sizeof(_ai2AvcUiAdjRate));
        }
#else
    #ifdef SUPPORT_FLASH_AQ
        x_memcpy((VOID *)VIRTUAL((UINT32)_ai2AvcTargetLevel), (const VOID *)rAudFlashAQ.ai2AudAvcTarget, sizeof(_ai2AvcTargetLevel));
        x_memcpy((VOID *)VIRTUAL((UINT32)_ai2AvcGainUp), (const VOID *)rAudFlashAQ.ai2AudAvcGainUp, sizeof(_ai2AvcGainUp));
        x_memcpy((VOID *)VIRTUAL((UINT32)_ai2AvcAdjRate), (const VOID *)rAudFlashAQ.ai2AudAvcAdjRate, sizeof(_ai2AvcAdjRate));
        x_memcpy((VOID *)VIRTUAL((UINT32)_ai2AvcUiAdjRate), (const VOID *)rAudFlashAQ.ai2AudAvcUiAdjRate, sizeof(_ai2AvcUiAdjRate));
    #else
        VERIFY(DRVCUST_OptQuery(eAudioAvcTarget, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_ai2AvcTargetLevel), (const VOID *)u4Ptr, sizeof(_ai2AvcTargetLevel));

        VERIFY(DRVCUST_OptQuery(eAudioAvcGainUp, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_ai2AvcGainUp), (const VOID *)u4Ptr, sizeof(_ai2AvcGainUp));

        VERIFY(DRVCUST_OptQuery(eAudioAvcAdjustRate, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_ai2AvcAdjRate), (const VOID *)u4Ptr, sizeof(_ai2AvcAdjRate));

        VERIFY(DRVCUST_OptQuery(eAudioAvcUiAdjustRate, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_ai2AvcUiAdjRate), (const VOID *)u4Ptr, sizeof(_ai2AvcUiAdjRate));
    #endif
#endif //CC_AUD_USE_FLASH_AQ

#ifdef CC_AUD_VOLUME_OFFSET
        VERIFY(DRVCUST_OptQuery(eAudioVolumeOffset, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_aai4VolOffsetMap), (const VOID *)u4Ptr, sizeof(_aai4VolOffsetMap));
#endif
#ifdef CC_AUD_TV_SYS_FINE_TUNE_VOL
        VERIFY(DRVCUST_OptQuery(eAudioTvSysFineTuneVol, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_aau1TVFineTuneVolume), (const VOID *)u4Ptr, sizeof(_aau1TVFineTuneVolume));
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
                _aafgChAtvMute[u1DecId][u1ChlNum] = FALSE;
            }
        }

        VERIFY(x_sema_create(&_ahVolCtlSema[AUD_DEC_MAIN], X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        VERIFY(x_sema_create(&_ahVolCtlSema[AUD_DEC_AUX], X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
#if 1 //def CC_MT5391_AUD_3_DECODER
        VERIFY(x_sema_create(&_ahVolCtlSema[AUD_DEC_THIRD], X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
#endif
#if 1 //def CC_AUD_4_DECODER_SUPPORT
        VERIFY(x_sema_create(&_ahVolCtlSema[AUD_DEC_4TH], X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
#endif

        _u2AdFadeMode = (((UINT16)DRVCUST_OptGet(eAudioPanFadeControl))<<9);

        fgInit = TRUE;
    }
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
#ifdef SUPPORT_FLASH_AQ
        x_memcpy((VOID *)VIRTUAL((UINT32)_aai1ChEqCfg), (const VOID *)rAudFlashAQ.aai1AudChlEqCfg, sizeof(_aai1ChEqCfg));
#else
        UINT32 u4Ptr;
        VERIFY(DRVCUST_OptQuery(eAudioAvChlEqTable, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_aai1ChEqCfg), (const VOID *)u4Ptr, sizeof(_aai1ChEqCfg));
#endif

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
        x_memcpy((VOID *)VIRTUAL((UINT32)_aai1PeqFq), (const VOID *)u4Ptr, sizeof(_aai1PeqFq));

        VERIFY(DRVCUST_OptQuery(eAudioPeqBwCfg, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_aai1PeqBw), (const VOID *)u4Ptr, sizeof(_aai1PeqBw));

        VERIFY(DRVCUST_OptQuery(eAudioPeqGainCfg, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_aai1PeqGain), (const VOID *)u4Ptr, sizeof(_aai1PeqGain));

        VERIFY(DRVCUST_OptQuery(eAudioBassTreble, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_aai1BassTreble), (const VOID *)u4Ptr, sizeof(_aai1BassTreble));
        VERIFY(DRVCUST_OptQuery(eAudioPSMCfg, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_arSoundModeCase), (const VOID *)u4Ptr, sizeof(_arSoundModeCase));

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
           x_memcpy((VOID *)VIRTUAL((UINT32)_aai1BassTreble), (const VOID *)_au1AQBassTrebleOffset, sizeof(_au1AQBassTrebleOffset));
       }

#endif //CC_AUD_USE_FLASH_AQ
		VERIFY(x_sema_create(&_ahPSMCtlSema, X_SEMA_TYPE_BINARY,
                             X_SEMA_STATE_UNLOCK) == OSR_OK);
        fgInit = TRUE;
    }

}
#endif
#ifdef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
void _AUD_DspVSMCfgInit(void)
{
    static BOOL fgInit = FALSE;

    // Init volume value
    if (!fgInit)
    {
        UINT32 u4Ptr;

        VERIFY(DRVCUST_OptQuery(eAudioVSModeCfg, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_aai1VSMode), (const VOID *)u4Ptr, sizeof(_aai1VSMode));
#ifdef CC_AUD_USE_FLASH_AQ
        if(_fgAQ_Enable[AQ_TYPE_MVS])
        {
            VERIFY(sizeof(_aai1VSMode) >= sizeof(_au4AQ_MVS));
            x_memcpy((VOID *)VIRTUAL((UINT32)_aai1VSMode), (const VOID *)_au4AQ_MVS, sizeof(_au4AQ_MVS));
        }
#endif

        VERIFY(x_sema_create(&_ahVSMCtlSema, X_SEMA_TYPE_BINARY,
            X_SEMA_STATE_UNLOCK) == OSR_OK);
        fgInit = TRUE;
    }
}

#endif
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

#ifdef SUPPORT_FLASH_AQ
        x_memcpy((VOID *)VIRTUAL((UINT32)_au1ChDelay), (const VOID *)rAudFlashAQ.au1AudChDelay, sizeof(_au1ChDelay));
#else
        UINT32 u4Ptr;
        VERIFY(DRVCUST_OptQuery(eAudioChannelDelay, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_au1ChDelay), (const VOID *)u4Ptr, sizeof(_au1ChDelay));
#endif
        _au1ChDelay[AUD_STREAM_FROM_DIGITAL_TUNER] = 612;
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

        // For Default
        VERIFY(DRVCUST_OptQuery(eAudioIecCopyright, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)(&_aSpdifInfo.u1Copyright[SPDIF_REG_TYPE_DEFAULT][0]),
        	(const VOID *)u4Ptr, sizeof(UINT8) * AUD_STREAM_FROM_NUM);
        VERIFY(DRVCUST_OptQuery(eAudioIecCategory, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)(&_aSpdifInfo.u1Category[SPDIF_REG_TYPE_DEFAULT][0]),
        	(const VOID *)u4Ptr, sizeof(UINT8) * AUD_STREAM_FROM_NUM);
        VERIFY(DRVCUST_OptQuery(eAudioIecWordLength, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)(&_aSpdifInfo.u1WordLength[SPDIF_REG_TYPE_DEFAULT][0]),
        	(const VOID *)u4Ptr, sizeof(UINT8) * AUD_STREAM_FROM_NUM);

        // For others, copy from default
        x_memcpy((VOID *)(&_aSpdifInfo.u1Copyright[SPDIF_REG_TYPE_USER][0]),
               (VOID *)(&_aSpdifInfo.u1Copyright[SPDIF_REG_TYPE_DEFAULT][0]),
               sizeof(UINT8) * AUD_STREAM_FROM_NUM);
        x_memcpy((VOID *)(&_aSpdifInfo.u1Category[SPDIF_REG_TYPE_USER][0]),
               (VOID *)(&_aSpdifInfo.u1Category[SPDIF_REG_TYPE_DEFAULT][0]),
               sizeof(UINT8) * AUD_STREAM_FROM_NUM);
        x_memcpy((VOID *)(&_aSpdifInfo.u1WordLength[SPDIF_REG_TYPE_USER][0]),
               (VOID *)(&_aSpdifInfo.u1WordLength[SPDIF_REG_TYPE_DEFAULT][0]),
               sizeof(UINT8) * AUD_STREAM_FROM_NUM);

        // By source or not
        VERIFY(DRVCUST_OptQuery(eAudioIecChannelStatusBySource, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)(&_aSpdifInfo.eUpdateMode[SPDIF_REG_TYPE_DEFAULT][0]),
        	(const VOID *)u4Ptr, sizeof(UINT8) * AUD_STREAM_FROM_NUM);

        x_memcpy((VOID *)(&_aSpdifInfo.eUpdateMode[SPDIF_REG_TYPE_USER][0]),
               (VOID *)(&_aSpdifInfo.eUpdateMode[SPDIF_REG_TYPE_DEFAULT][0]),
               sizeof(UINT8) * AUD_STREAM_FROM_NUM);


        VERIFY(x_sema_create(&_ahSpdifCtlSema, X_SEMA_TYPE_BINARY,
                             X_SEMA_STATE_UNLOCK) == OSR_OK);
        fgInit = TRUE;
    }
}

//-----------------------------------------------------------------------------
/** _AUD_DspAsrcInit
 *  Initialize ASRC configuration
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
void _AUD_DspAsrcInit(void)
{
    static BOOL fgInit = FALSE;

    if (!fgInit)
    {
        VERIFY(x_sema_create(&_hAsrcControlSema, X_SEMA_TYPE_BINARY,
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
#if 0
    static BOOL fgInit = FALSE;
    if (!fgInit)
    {
        AUD_DspChEqLoadPreset(AUD_DEC_MAIN, AUD_EQ_ROCK);
        AUD_DspChEqLoadPreset(AUD_DEC_AUX, AUD_EQ_ROCK);
        fgInit = TRUE;
    }
#endif

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

BOOL _AUD_DspGetChannelEqEnable(UINT8 u1DecId)
{
    UINT16 u2EqFlagIdx;
    if (u1DecId == AUD_DEC_MAIN)
    {
        u2EqFlagIdx = B_EQFLAG;
    }
    else
    {
        u2EqFlagIdx = B_EQFLAG_DEC2;
    }

    if (uReadShmUINT8(u2EqFlagIdx)&EQ_FLAG)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
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

    UNUSED(u1DecId);

    LOG(5,"CMD: set BassEnable: Dec(%d) Enable(%d)\n", u1DecId, (UINT8)fgEnable);

    u2EqFlagIdx = B_EQFLAG;
    u4UopCmd = UOP_DSP_EQUALIZER_FLAG;
    u1ShmFlag = uReadShmUINT8(B_EQFLAG);

    if ((fgEnable && !(u1ShmFlag& EQ_BASS_BOOST)) ||
        (!fgEnable && ((u1ShmFlag & EQ_BASS_BOOST) == EQ_BASS_BOOST)))
    {
        u1ShmFlag = fgEnable ? (u1ShmFlag | EQ_BASS_BOOST) : (u1ShmFlag & ~ EQ_BASS_BOOST);
        vWriteShmUINT8(u2EqFlagIdx, u1ShmFlag);
        DSP_SendDspTaskCmd(u4UopCmd);
    }
}

BOOL _AUD_DspGetBassBoostEnable(UINT8 u1DecId)
{
    UINT16 u2EqFlagIdx;

    UNUSED(u1DecId);

    u2EqFlagIdx = B_EQFLAG;

    if (uReadShmUINT8(u2EqFlagIdx) & EQ_BASS_BOOST)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
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

    UNUSED(u1DecId);

    LOG(5,"CMD: set BassGain: Dec(%d) Level(%d)\n", u1DecId, u1Level);

    if (!fgEnable)
    {
        AUD_DspBassBoostEnable(AUD_DEC_MAIN, TRUE);
        fgEnable = TRUE;
    }

    u2BassBoostGainIdx = D_SBASSBOOSTGAIN;
    u4UopCmd = UOP_DSP_SUPER_BASS_BOOST_GAIN;

    if (u1Level >= BASS_TREBLE_GAIN_MAX)
    {
        u1Level = BASS_TREBLE_GAIN_MAX - 1;
    }

    vWriteShmUINT32(u2BassBoostGainIdx, u1Level);
    DSP_SendDspTaskCmd(u4UopCmd);
    AUD_DspBassBoostEnable(AUD_DEC_MAIN, TRUE);
    x_thread_delay(10);

    #ifdef CC_AUD_BASS_ADJ_LFE    //andrew wen 07/1/25
    u1Level = u1Level<<2;
    if (u1Level > 100)
    {
        u1Level = 100;
    }
    _VolumeCtrl(AUD_DEC_MAIN, AUD_CH_LFE, _au4VOLUME[u1Level]);
    #endif
#else
    // Use EQ-based sbass
    INT8 i1SbassDb, i1SbassDbStart, i1SbassStep, i1TotalDb;
    INT8 i1LoopCnt, i1LoopIdx;
    UINT8     u1Idx, u1EqType;
    static INT8 _i1SbassDbOld=EQ_BAND_GAIN_MID;

    UNUSED(u1DecId);

    LOG(5,"CMD: set BassGain: Dec(%d) Level(%d)\n", u1DecId, u1Level);

    u1EqType = _eEqMode;
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
        AUD_DspChEqBand(AUD_DEC_MAIN, (UINT8)EQ_BAND_DRY, (UINT8)(_ai1ChEqFinalCfg[EQ_BAND_DRY]+EQ_DRY_GAIN_MID), FALSE);
        for (u1Idx = 1; u1Idx < (CH_EQ_BAND_NO + 1); u1Idx++)
        {
            AUD_DspChEqBand(AUD_DEC_MAIN, u1Idx, (UINT8)(_ai1ChEqFinalCfg[u1Idx]+EQ_BAND_GAIN_MID), (u1Idx == CH_EQ_BAND_NO));
        }
        VERIFY(x_sema_unlock(_ahEqCtlSema) == OSR_OK);

        i1SbassDbStart += i1SbassStep;
    }

    AUD_DspEqEnable(AUD_DEC_MAIN, TRUE);
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

    UNUSED(u1DecId);

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

    UNUSED(u1DecId);

    LOG(5,"CMD: set SBassEnable: Dec(%d) Enable(%d)\n", u1DecId, (UINT8)fgEnable);

    u2EqFlagIdx = B_EQFLAG;
    u4UopCmd = UOP_DSP_EQUALIZER_FLAG;

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

    UNUSED(u1DecId);

    LOG(5,"CMD: set TrebleEnable: Dec(%d) Enable(%d)\n", u1DecId, (UINT8)fgEnable);

    u2EqFlagIdx = B_EQFLAG;
    u4UopCmd = UOP_DSP_EQUALIZER_FLAG;
    u1ShmFlag = uReadShmUINT8(u2EqFlagIdx);

    if ((fgEnable && !(u1ShmFlag& EQ_TREBLE_BOOST)) ||
        (!fgEnable && ((u1ShmFlag & EQ_TREBLE_BOOST) == EQ_TREBLE_BOOST)))
    {
        u1ShmFlag = fgEnable ? (u1ShmFlag | EQ_TREBLE_BOOST) : (u1ShmFlag & ~ EQ_TREBLE_BOOST);
        vWriteShmUINT8(u2EqFlagIdx, u1ShmFlag);
        DSP_SendDspTaskCmd(u4UopCmd);
    }
}

BOOL _AUD_DspGetClearBoostEnable(UINT8 u1DecId)
{
    UINT16 u2EqFlagIdx;

    UNUSED(u1DecId);

    u2EqFlagIdx = B_EQFLAG;

    if (uReadShmUINT8(u2EqFlagIdx) & EQ_TREBLE_BOOST)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
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

    UNUSED(u1DecId);

    LOG(5,"CMD: set TrebleGain: Dec(%d) Level(%d)\n", u1DecId, u1Level);

    if (!fgEnable)
    {
        AUD_DspClearBoostEnable(AUD_DEC_MAIN, TRUE);
        fgEnable = TRUE;
    }

    u2ClearBoostGainIdx = D_SBASSCLEARGAIN;
    u4UopCmd = UOP_DSP_SUPER_BASS_CLEAR_GAIN;

    if (u1Level >= BASS_TREBLE_GAIN_MAX)
    {
        u1Level = BASS_TREBLE_GAIN_MAX - 1;
    }

    vWriteShmUINT32(u2ClearBoostGainIdx, u1Level);
    DSP_SendDspTaskCmd(u4UopCmd);
    AUD_DspClearBoostEnable(AUD_DEC_MAIN, TRUE);
    x_thread_delay(10);

#else
    // Use EQ-based sbass
    INT8 i1SbassDb, i1SbassDbStart, i1SbassStep, i1TotalDb;
    INT8 i1LoopCnt, i1LoopIdx;
    UINT8     u1Idx, u1EqType;
    static INT8 _i1SbassDbOld=EQ_BAND_GAIN_MID;

    UNUSED(u1DecId);

    LOG(5,"CMD: set TrebleGain: Dec(%d) Level(%d)\n", u1DecId, u1Level);

    u1EqType = _eEqMode;
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
        AUD_DspChEqBand(AUD_DEC_MAIN, (UINT8)EQ_BAND_DRY, (UINT8)(_ai1ChEqFinalCfg[EQ_BAND_DRY]+EQ_DRY_GAIN_MID), FALSE);
        for (u1Idx = 1; u1Idx < (CH_EQ_BAND_NO + 1); u1Idx++)
        {
            AUD_DspChEqBand(AUD_DEC_MAIN, u1Idx, (UINT8)(_ai1ChEqFinalCfg[u1Idx]+EQ_BAND_GAIN_MID), (u1Idx == CH_EQ_BAND_NO));
        }
        VERIFY(x_sema_unlock(_ahEqCtlSema) == OSR_OK);

        i1SbassDbStart += i1SbassStep;
    }

    AUD_DspEqEnable(AUD_DEC_MAIN, TRUE);
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

    UNUSED(u1DecId);
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

    UNUSED(u1DecId);

    u2EqFlagIdx = B_EQFLAG;
    u4UopCmd = UOP_DSP_EQUALIZER_FLAG;
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

    UNUSED(u1DecId);

    if (u1BandIdx >= EQ_BAND_MAX)
    {
        return;
    }

    u2ShmIndex = D_EQ_DRY;
    u4UopCmd = UOP_DSP_EQUALIZER_CHANNEL_CHANGE + 0x01000000;

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

    UNUSED(u1DecId);

    if (eEqType >= AUD_EQ_NUM)
    {
        eEqType = AUD_EQ_OFF;
    }

    _eEqMode = eEqType;

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
    AUD_DspChEqBand(AUD_DEC_MAIN, (UINT8)EQ_BAND_DRY, (UINT8)(_aai1ChEqCfg[eEqType][0]+EQ_DRY_GAIN_MID), FALSE);
#else
    AUD_DspChEqBand(AUD_DEC_MAIN, (UINT8)EQ_BAND_DRY, (UINT8)(_ai1ChEqFinalCfg[0]+EQ_DRY_GAIN_MID), FALSE);
#endif

    for (u1Idx = 1; u1Idx < (CH_EQ_BAND_NO + 1); u1Idx++)
    {
#ifndef  CC_AUD_EQ_BASED_SBASS
        AUD_DspChEqBand(AUD_DEC_MAIN, u1Idx, (UINT8)(_aai1ChEqCfg[eEqType][u1Idx]+EQ_BAND_GAIN_MID), (u1Idx == CH_EQ_BAND_NO));
#else
        AUD_DspChEqBand(AUD_DEC_MAIN, u1Idx, (UINT8)(_ai1ChEqFinalCfg[u1Idx]+EQ_BAND_GAIN_MID), (u1Idx == CH_EQ_BAND_NO));
#endif
    }
    VERIFY(x_sema_unlock(_ahEqCtlSema) == OSR_OK);

    AUD_DspEqEnable(AUD_DEC_MAIN, TRUE);
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

    UNUSED(u1DecId);

    if (u1BandIdx >= EQ_BAND_MAX)
    {
        return;
    }

    u2ShmIndex = D_EQ_DRY;
    u4UopCmd = UOP_DSP_EQUALIZER_FLAG;

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

    LOG(5,"CMD: set EQ mode: Dec(%d) Type(%d)\n", u1DecId, (UINT8)eEqType);

    UNUSED(u1DecId);

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

    _eEqMode = eEqType;

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
    AUD_DspChEqLoudness(AUD_DEC_MAIN, &(_aai1ChEqCfg[eEqType][1]));
#else
    AUD_DspChEqLoudness(AUD_DEC_MAIN, &(_ai1ChEqFinalCfg[1]));
#endif
#endif
    VERIFY(x_sema_lock(_ahEqCtlSema, X_SEMA_OPTION_WAIT) == OSR_OK);
#ifndef  CC_AUD_EQ_BASED_SBASS
    AUD_DspChEqBandMode(AUD_DEC_MAIN, (UINT8)EQ_BAND_DRY, (UINT8)(_aai1ChEqCfg[eEqType][0]+EQ_DRY_GAIN_MID), FALSE);
#else
    AUD_DspChEqBandMode(AUD_DEC_MAIN, (UINT8)EQ_BAND_DRY, (UINT8)(_ai1ChEqFinalCfg[0]+EQ_DRY_GAIN_MID), FALSE);
#endif

    for(u1Idx = 1; u1Idx < (CH_EQ_BAND_NO + 1); u1Idx++)
    {
#ifndef  CC_AUD_EQ_BASED_SBASS
        AUD_DspChEqBandMode(AUD_DEC_MAIN, u1Idx, (UINT8)(_aai1ChEqCfg[eEqType][u1Idx]+EQ_BAND_GAIN_MID), (u1Idx == CH_EQ_BAND_NO));
#else
        AUD_DspChEqBandMode(AUD_DEC_MAIN, u1Idx, (UINT8)(_ai1ChEqFinalCfg[u1Idx]+EQ_BAND_GAIN_MID), (u1Idx == CH_EQ_BAND_NO));
#endif
    }
    VERIFY(x_sema_unlock(_ahEqCtlSema) == OSR_OK);

    AUD_DspEqEnable(AUD_DEC_MAIN, TRUE);
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
    UNUSED(u1DecId);
    *eEqType = _eEqMode;
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

    UNUSED(u1DecId);

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

    UNUSED(u1DecId);

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

/** AUD_DspAC3DRCRange100
 *  setup range for dynamix range compression.
 *
 *  @param  uDRCLevel  DRC range 0~100.
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspAC3DRCRange100(UINT8 uDRCLevel,UINT8 u1DecId)
{
    UINT16 u2DRCLowIdx=D_AC3DYN_LOW;
    UINT16 u2DRCHighIdx=D_AC3DYN_HIGH;
    UINT16 u2DRCLowUop=UOP_DSP_AC3_DYNAMIC_LOW;
    UINT16 u2DRCHighUop=UOP_DSP_AC3_DYNAMIC_HIGH;
    UINT32 u2drclEVEL;
	u2drclEVEL= (UINT32)uDRCLevel*0x7fffff/100;
    if (u1DecId != 0)
    {
        u2DRCLowIdx=D_AC3DYN_LOW_DEC2;
        u2DRCHighIdx=D_AC3DYN_HIGH_DEC2;
        u2DRCLowUop=UOP_DSP_AC3_DYNAMIC_LOW_DEC2;
        u2DRCHighUop=UOP_DSP_AC3_DYNAMIC_HIGH_DEC2;
    }

    vWriteShmUINT32(u2DRCLowIdx, u2drclEVEL);
    DSP_SendDspTaskCmd(u2DRCLowUop);
    vWriteShmUINT32(u2DRCHighIdx, u2drclEVEL);
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

//-----------------------------------------------------------------------------
/** _AUD_DspGetAC3DRCRange100
 *  setup range for dynamix range compression.
 *
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval uDRCLevel  DRC range.
 *                               0x00000000-->0.0
 *                               0x007FFFFF-->1.0
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetAC3DRCRange100(UINT8 u1DecId)
{
    UINT16 u2DRCIdx;
    UINT32 u4DRCLevel;
    UINT8 uDRCLevelIndex;

    u2DRCIdx=(u1DecId==AUD_DEC_MAIN)? D_AC3DYN_LOW: D_AC3DYN_LOW_DEC2;
    u4DRCLevel = u4ReadShmUINT32(u2DRCIdx);

    for (uDRCLevelIndex = 0; uDRCLevelIndex <= 100; uDRCLevelIndex ++)
    {
        if (u4DRCLevel == (UINT32)uDRCLevelIndex*0x7fffff/100)
        {
            break;
        }
    }
    return uDRCLevelIndex;
}

#ifdef CC_AUD_SUPPORT_MS10
//-----------------------------------------------------------------------------
/** _AUD_DspSetDDCControl
 *  setup for DDC control.
 *
 *  @param  u2Control DDC control
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetDDCControl (UINT16 u2Control, UINT8 uDecIndx)
{
    UINT16 u2OldMode;
    UINT16 u2ShmIndex=W_DDC_CONTROL;
    UINT16 u2UopIndex=UOP_DSP_DDC_CONTROL;

    Printf ("_AUD_DspSetDDCControl = %d, %d\n", u2Control, uDecIndx);
  //u2Control = 1;
    if (!(IS_SW_SupportMS10()))
    {
        return;
    }
    else
    {
        if (uDecIndx!=0)
        {
            u2ShmIndex = W_DDC_CONTROL_DEC2;
            u2UopIndex = UOP_DSP_DDC_CONTROL_DEC2;
        }
    }

    u2OldMode = u2ReadShmUINT16 (u2ShmIndex);

    // only for DDC control, bit 2, 3
    u2Control = (u2OldMode & 0x3) | (u2Control << 2);

    if (u2Control != u2OldMode)
    {
        vWriteShmUINT16 (u2ShmIndex, u2Control);
        DSP_SendDspTaskCmd (u2UopIndex);
    }
}

//-----------------------------------------------------------------------------
/** _AUD_DspDDCStrmId
 *  setup for DDC associated stream id.
 *
 *  @param  u1StrmId stream id
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetDDCStrmId (UINT8 u1StrmId, UINT8 uDecIndx)
{
    UINT8 u1OldMode;
    UINT16 u2ShmIndex=B_DDC_STRMID;
    UINT16 u2UopIndex=UOP_DSP_DDC_STRMID;

    if (uDecIndx!=0)
    {
        u2ShmIndex = B_DDC_STRMID_DEC2;
        u2UopIndex = UOP_DSP_DDC_STRMID_DEC2;
    }

    u1OldMode = uReadShmUINT8 (u2ShmIndex);

    if (u1StrmId != u1OldMode)
    {
        vWriteShmUINT8 (u2ShmIndex, u1StrmId);
        DSP_SendDspTaskCmd (u2UopIndex);
    }
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetDDCAdEnable
 *  setup for.
 *
 *  @param  u1StrmId stream id
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetDDCAdEnable (UINT8 u1DecId, BOOL fgEnable)
{
#ifndef CC_MT5391_AUD_3_DECODER
    UNUSED(u1DecId);
    UNUSED(fgEnable);
    return;
#else
    UINT16 u2ShmIndex = B_ADFLAG;
    UINT32 u2UopIndex = UOP_DSP_AD_FLAG;
    UINT8   u1ADFLAGValue,u1OldAdFlag;

    DSP_SetTriDecMode(fgEnable);
    u1OldAdFlag = uReadShmUINT8(B_ADFLAG);
    u1ADFLAGValue = ((u1OldAdFlag & (0xFE)) | fgEnable) | (0x40);       //bit6: one buffer mode

    if (u1OldAdFlag != u1ADFLAGValue)
    {
        vWriteShmUINT8(u2ShmIndex, (UINT8)u1ADFLAGValue);
        DSP_SendDspTaskCmd(u2UopIndex);
    }
#endif
}

//-----------------------------------------------------------------------------
/** _AUD_DspDDCStrmId
 *  setup for DDC associated stream id.
 *
 *  @param  uMixer control of mixer
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetDDCMixer (UINT8 uMixer, UINT8 uDecIndx)
{
    UINT8 u1OldMode;
    UINT16 u2ShmIndex=B_DDC_ASSOC_MIX;
    UINT16 u2UopIndex=UOP_DSP_DDC_ASSOC_MIX;

    if (uDecIndx!=0)
    {
        u2ShmIndex = B_DDC_ASSOC_MIX_DEC2;
        u2UopIndex = UOP_DSP_DDC_ASSOC_MIX_DEC2;
    }

    u1OldMode = uReadShmUINT8 (u2ShmIndex);

    if (uMixer != u1OldMode)
    {
        vWriteShmUINT8 (u2ShmIndex, uMixer);
        DSP_SendDspTaskCmd (u2UopIndex);
    }
}

//-----------------------------------------------------------------------------
/** _AUD_DspDdtDmxRfMode
 *  setup for DDT dmx mode or RF mode.
 *
 *  @param  u1Mode: dmxmode/RF mode, u1DecId 0: first decoder 1: seconder decoder. 2: third decoder, bSetDmx 0: set dmx, 1: set RF
 *  @retval void
 *  u1Mode:
 *  00b: Line, Lt/Rt
 *  01b: Line, Lo/Ro
 *  10b: RF, Lt/Rt
 *  11b: RF, Lo/Ro
 */
//-----------------------------------------------------------------------------
void _AUD_DspDdtDmxRfMode (UINT8 u1Mode, UINT8 u1DecId, BOOL bSetDmx)
{
    UINT8 u1SpkCfgDMMode, u1DdtDmxMode;

    u1DdtDmxMode = u1Mode;

    if (bSetDmx)        // set LtRt/LoRo dmx
    {
        u1SpkCfgDMMode = _AUD_DspGetSpeakerOutputConfig(u1DecId);
        switch(u1DdtDmxMode)
        {
            case 0: // LtRt
                if (u1SpkCfgDMMode != DMX_SPKCFG_LTRT)
                    _AUD_DspSpeakerOutputConfig(u1DecId, DMX_SPKCFG_LTRT);
                break;
            case 1: // LoRo
                if (u1SpkCfgDMMode != DMX_SPKCFG_LORO)
                    _AUD_DspSpeakerOutputConfig(u1DecId, DMX_SPKCFG_LORO);
                break;
            default:
                printf("incorrect downmix mode\n");
                break;
        }

    }
    else                // set Rf/Line dmx
    {

        UINT8  u1OldDMMode;
        UINT16 u2ShmIndex=B_DDT_COMPMODE;
        UINT16 u2UopIndex=UOP_DSP_DDT_COMPMODE;

        if (u1DecId!=0)
        {
            u2ShmIndex = B_DDT_COMPMODE_DEC2;
            u2UopIndex = UOP_DSP_DDT_COMPMODE_DEC2;
        }

        u1OldDMMode = uReadShmUINT8(u2ShmIndex);
        switch(u1DdtDmxMode)
        {
            case 0: // Line
                if (u1OldDMMode != DMX_MS10_LINE)
                    vWriteShmUINT8(u2ShmIndex, DMX_MS10_LINE);
                break;
            case 1: // RF
                if (u1OldDMMode != DMX_MS10_RF)
                    vWriteShmUINT8(u2ShmIndex, DMX_MS10_RF);
                break;
            default:
                printf("incorrect downmix mode\n");
                return;
        }

        DSP_SendDspTaskCmd(u2UopIndex);
    }

}

//-----------------------------------------------------------------------------
/** AUD_DspDdtDualMode
 *  setup for DDT dual mode.
 *
 *  @param  u1Mode: dual mode, u1DecId 0: first decoder 1: seconder decoder. 2: third decoder
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspDdtDualMode (UINT8 u1Mode, UINT8 u1DecId)
{
    UINT8 u1OldDMMode;
    UINT16 u2ShmIndex=B_DDT_DUALMODE;
    UINT16 u2UopIndex=UOP_DSP_DDT_DUAL_MODE;

    if (u1DecId == AUD_DEC_AUX)
    {
        u2ShmIndex = B_DDT_DUALMODE_DEC2;
        u2UopIndex = UOP_DSP_DDT_DUAL_MODE_DEC2;
    }
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u2ShmIndex = B_DDT_DUALMODE_DEC3;
        u2UopIndex = UOP_DSP_DDT_DUAL_MODE_DEC3;
    }
    u1OldDMMode = uReadShmUINT8(u2ShmIndex);

    if (u1Mode != u1OldDMMode)
    {
        vWriteShmUINT8(u2ShmIndex,u1Mode);
        DSP_SendDspTaskCmd(u2UopIndex);
    }

}


//-----------------------------------------------------------------------------
/** _AUD_DspDdtCompValRange
 *  setup for DDT DRC cut/boost factor.
 *
 *  @param  u1Mode: cut/boost factor 0~100, u1DecId 0: first decoder 1: seconder decoder. 2: third decoder
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspDdtCompValRange (UINT8 u1Mode, UINT8 u1DecId)
{
    UINT8 u1OldDMMode, u1NewDMMode;
    UINT16 u2ShmIndex1=B_DDT_COMPVAL;
    UINT16 u2UopIndex1=UOP_DSP_DDT_COMPVAL;

    if (u1DecId == AUD_DEC_AUX)
    {
        u2ShmIndex1 = B_DDT_COMPVAL_DEC2;
        u2UopIndex1 = UOP_DSP_DDT_COMPVAL_DEC2;
    }
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u2ShmIndex1 = B_DDT_COMPVAL_DEC3;
        u2UopIndex1 = UOP_DSP_DDT_COMPVAL_DEC3;
    }
    u1OldDMMode = uReadShmUINT8(u2ShmIndex1);
    u1NewDMMode = u1Mode;
    if (u1NewDMMode != u1OldDMMode)
    {
        vWriteShmUINT8(u2ShmIndex1,u1NewDMMode);
        DSP_SendDspTaskCmd(u2UopIndex1);
    }


}

//-----------------------------------------------------------------------------
/** _AUD_DspGetDdtDmxRfMode
 *  get DDT dmx mode.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder. 2: third decoder, bGetDmx 0: get dmx, 1: get RF
 *  u1Mode:
 *  00b: Line, Lt/Rt
 *  01b: Line, Lo/Ro
 *  10b: RF, Lt/Rt
 *  11b: RF, Lo/Ro
 *  @retval u1Mode: dmxmode/RF mode
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetDdtDmxRfMode (UINT8 u1DecId,BOOL bGetDmx)
{
    UINT8 u1Mode;

    if (bGetDmx)
    {
        u1Mode = _AUD_DspGetSpeakerOutputConfig(u1DecId);
        if (u1Mode == DMX_SPKCFG_LORO)
            return 1;                       // LoRo
        else if (u1Mode == DMX_SPKCFG_LTRT)
            return 0;                       // LtRt
        else
            return 0xFF;
    }
    else
    {
        UINT16 u2ShmIndex;

        u2ShmIndex = (u1DecId == AUD_DEC_MAIN)? B_DDT_COMPMODE : B_DDT_COMPMODE_DEC2;
        u1Mode = uReadShmUINT8(u2ShmIndex);

        if (u1Mode == DMX_MS10_LINE)        //Line
            return 0;
        else if (u1Mode == DMX_MS10_RF)     //RF
            return 1;
        else
            return 0xFF;

    }
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetDdtDualMode
 *  get DDT dual mode.
 *
 *  @param   u1DecId 0: first decoder 1: seconder decoder. 2: third decoder
 *  @retval u1Mode: dual mode
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetDdtDualMode (UINT8 u1DecId)
{
    UINT16 u2DmxIdx = B_DDT_DUALMODE;

    if (u1DecId==AUD_DEC_AUX || u1DecId==AUD_DEC_THIRD)
    {
        u2DmxIdx = B_DDT_DUALMODE_DEC2;
    }

    return(uReadShmUINT8(u2DmxIdx));

}


//-----------------------------------------------------------------------------
/** _AUD_DspGetDdtDrcRange
 *  get DDT DRC cut/boost factor.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder. 2: third decoder
 *  @retval u1Mode: cut/boost factor 0~100
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetDdtDrcRange (UINT8 u1DecId)
{
    UINT16 u2DmxIdx = B_DDT_COMPVAL;
    UINT8 u1DrcFactor;

    if (u1DecId != AUD_DEC_MAIN)
    {
        u2DmxIdx = B_DDT_COMPVAL_DEC2;
    }
    u1DrcFactor = uReadShmUINT8(u2DmxIdx);
    return u1DrcFactor;
}

#endif  // CC_AUD_SUPPORT_MS10

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

UINT32 _AUD_DspGetSpeakerConfig(UINT8 uDecIndex)
{
    UINT16 u2SpkCfgIndx = D_SPKCFG;
    if (uDecIndex != 0)
    {
        u2SpkCfgIndx = D_SPKCFG_DEC2;
    }
    return u4ReadShmUINT32(u2SpkCfgIndx);
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
void _AUD_DspSpeakerLargeSmallConfig(UINT8 u1DecIndex, UINT8 u1FrontPair, UINT8 u1Center, UINT8 u1Surround, UINT8 u1CH910)
{
    UINT32 u4Config = 0;
    UINT16 u2SpkCfgIndx = D_SPKCFG;
    UINT16 u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER;
    if (u1DecIndex != AUD_DEC_MAIN)
    {
        u2SpkCfgIndx = D_SPKCFG_DEC2;
        u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER_DEC2;
    }

    if (u1Center == 1)
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

void _AUD_DspGetSpeakerLargeSmallConfig(UINT8 u1DecIndex, UINT8* u1FrontPair, UINT8* u1Center, UINT8* u1Surround, UINT8* u1CH910)
{
    UINT16 u2SpkCfgIndx = D_SPKCFG;
    UINT32 u4Config;

    if (u1DecIndex != AUD_DEC_MAIN)
    {
        u2SpkCfgIndx = D_SPKCFG_DEC2;
    }

    u4Config = u4ReadShmUINT32(u2SpkCfgIndx);

    *u1FrontPair = ((u4Config & (0x06<<12))>0);
    *u1Center = ((u4Config & (0x3<<19))>>0);
    *u1Surround = ((u4Config & (0x18<<12))>>0);
    *u1CH910 = ((u4Config & (0x3<<19))>>0);
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
    UINT16 u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER;

    UINT8 u1CurSpkCfg = 0;

//    if ((u1SpkCfg & SPK_CFG_MASK) != _au4SpeakerOutCfg[u1DecIndex])
    {
        if (u1DecIndex != AUD_DEC_MAIN)
        {
            u2SpkCfgIndx = D_SPKCFG_DEC2;
            u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER_DEC2;
        }

        // If new setting is the same as the original
        u1CurSpkCfg= (u4ReadShmUINT32(u2SpkCfgIndx) & 0x7);
        if (u1CurSpkCfg == u1SpkCfg)
        {
            return;
        }

        vWriteShmUINT32(u2SpkCfgIndx, (u4ReadShmUINT32(u2SpkCfgIndx) & 0xfffffff8) | (UINT32)u1SpkCfg);
        DSP_SendDspTaskCmd(u2SpkCfgUop);
    }
}

UINT8 _AUD_DspGetSpeakerOutputConfig(UINT8 u1DecIndex)
{
    UINT16 u2SpkCfgIndx = D_SPKCFG;

    if (u1DecIndex != AUD_DEC_MAIN)
    {
        u2SpkCfgIndx = D_SPKCFG_DEC2;
    }

    return (u4ReadShmUINT32(u2SpkCfgIndx) & 0x7);
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

BOOL _AUD_DspGetSpeakerSubwooferEnable(UINT8 u1DecIndex)
{
    UINT16 u2SpkCfgIndx = D_SPKCFG;
    if (u1DecIndex != AUD_DEC_MAIN)
    {
        u2SpkCfgIndx = D_SPKCFG_DEC2;
    }

    if (u4ReadShmUINT32(u2SpkCfgIndx) & (1<<5))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
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
        if (AUD_GetTargetCountry() == COUNTRY_EU)
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

//-----------------------------------------------------------------------------
/** AUD_DspChannelDelayNoUop
 *  setup channel delay for individual channel.
 *
 *  @param  u2Delay 0.05 m/unit.
 *  @param  eChIndex 0(L), 1(R), 2(LS), 3(RS), 4(C), 5(SUB), 6(Bypass L), 7(Bypass R)
 *                   8(Downmix L), 9(Downmix R), 10(AUD L), 11(AUD R), 13(All), 14(LFE)
 *  @param  u1DecIndex 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspChannelDelayNoUop(UINT16 u2Delay, AUD_CH_T eChIndex, UINT8 uDecIndex)
{
    UINT16 u2ShmIndex;
    UINT8  uIdx;

    if ((eChIndex > AUD_CH_NUM) || (eChIndex == AUD_CH_INPUT_SRC))
    {
        return;
    }

    if (eChIndex == AUD_CH_ALL)
    {
        // Delay for all channels
        u2ShmIndex = W_CHDELAY_C;
        if((GetTargetCountry() == COUNTRY_EU) || (GetTargetCountry() == COUNTRY_JP))
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
        LOG(3,"$$$$$$ Enter _AUD_DspChannelDelayNoUop #######\n");
    }
    else
    {
        // Delay for individual channel
        switch (eChIndex)
        {
        case AUD_CH_FRONT_LEFT:
            u2ShmIndex = W_CHDELAY_L;
            break;
        case AUD_CH_FRONT_RIGHT:
            u2ShmIndex = W_CHDELAY_R;
            break;
        case AUD_CH_REAR_LEFT:
            u2ShmIndex = W_CHDELAY_LS;
            break;
        case AUD_CH_REAR_RIGHT:
            u2ShmIndex = W_CHDELAY_RS;
            break;
        case AUD_CH_CENTER:
            u2ShmIndex = W_CHDELAY_C;
            break;
        case AUD_CH_SUB_WOOFER:
        case AUD_CH_LFE:
            u2ShmIndex = W_CHDELAY_SUB;
            break;
        case AUD_CH_BYPASS_LEFT:
            u2ShmIndex = W_CHDELAY_CH7;
            break;
        case AUD_CH_BYPASS_RIGHT:
            u2ShmIndex = W_CHDELAY_CH8;
            break;
        case AUD_CH_DMX_LEFT:
            u2ShmIndex = W_CHDELAY_CH9;
            break;
        case AUD_CH_DMX_RIGHT:
            u2ShmIndex = W_CHDELAY_CH10;
            break;
        case AUD_CH_AUX_FRONT_LEFT:
            u2ShmIndex = W_CHDELAY_L_DEC2;
            break;
        case AUD_CH_AUX_FRONT_RIGHT:
            u2ShmIndex = W_CHDELAY_R_DEC2;
            break;
        default:
            u2ShmIndex = W_CHDELAY_C;
            break;
        }

        if (u2ReadShmUINT16(u2ShmIndex) == u2Delay)
        {
            return;
        }
        else
        {
            vWriteShmUINT16(u2ShmIndex, u2Delay);
        }
    }
}

UINT16 _AUD_DspGetChannelDelay(AUD_CH_T eChIndex)
{
    UINT16 u2ShmIndex;

    // Delay for individual channel
    switch (eChIndex)
    {
    case AUD_CH_FRONT_LEFT:
        u2ShmIndex = W_CHDELAY_L;
        break;
    case AUD_CH_FRONT_RIGHT:
        u2ShmIndex = W_CHDELAY_R;
        break;
    case AUD_CH_REAR_LEFT:
        u2ShmIndex = W_CHDELAY_LS;
        break;
    case AUD_CH_REAR_RIGHT:
        u2ShmIndex = W_CHDELAY_RS;
        break;
    case AUD_CH_CENTER:
        u2ShmIndex = W_CHDELAY_C;
        break;
    case AUD_CH_SUB_WOOFER:
    case AUD_CH_LFE:
        u2ShmIndex = W_CHDELAY_SUB;
        break;
    case AUD_CH_BYPASS_LEFT:
        u2ShmIndex = W_CHDELAY_CH7;
        break;
    case AUD_CH_BYPASS_RIGHT:
        u2ShmIndex = W_CHDELAY_CH8;
        break;
    case AUD_CH_DMX_LEFT:
        u2ShmIndex = W_CHDELAY_CH9;
        break;
    case AUD_CH_DMX_RIGHT:
        u2ShmIndex = W_CHDELAY_CH10;
        break;
    case AUD_CH_AUX_FRONT_LEFT:
        u2ShmIndex = W_CHDELAY_L_DEC2;
        break;
    case AUD_CH_AUX_FRONT_RIGHT:
        u2ShmIndex = W_CHDELAY_R_DEC2;
        break;
    default:
        u2ShmIndex = W_CHDELAY_C;
        break;
    }

    return u2ReadShmUINT16(u2ShmIndex);
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
        if (AUD_GetTargetCountry() == COUNTRY_EU)
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
    _AUD_DspChannelDelay(_au1ChDelay[(UINT8)eStreamFrom], AUD_CH_ALL, AUD_DEC_MAIN);
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
    //20101018,By Daniel,for flash-lite clip using dec3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return;
    //End
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#endif

    LOG(5,"CMD: set Channel Volume: Dec(%d) CH(%d) Volume(%d)\n",
        u1DecId, (UINT8)eChannel, u1Value);

    VOL_CTL_SEMA_LOCK(u1DecId);
    _fgChannelVolUseShmRawData = FALSE;
    _aau1ChannelVolume[u1DecId][eChannel] = u1Value;
    _AUD_DspVolumeChange(u1DecId, eChannel);
    VOL_CTL_SEMA_UNLOCK(u1DecId);
}

//-----------------------------------------------------------------------------
/** AUD_DspPanFadePesUpdateEn
 *  setup ad pan fade update by pes enable
 *
 *  @param  bEnable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspPanFadePesUpdateEn(BOOL bEnable)
{
    AUD_PanFadePesUpdateEn(bEnable);
}

//-----------------------------------------------------------------------------
/** _AUD_DspAdPanFadeEnable
 *  setup ad pan volume for individual channel.
 *
 *  @param  bEnable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspAdPanFadeEnable(BOOL bEnable)
{
    UINT32 u2UopIndex = UOP_DSP_PANFADE_DSP_ENABLE;
    UINT16 u4Config = 0;

    AUD_SetAdFadeEnable(bEnable);
    AUD_SetAdPanEnable(bEnable);

    if (bEnable == TRUE)
    {
        if (_u2AdFadeMode == 0)
        {
            _u2AdFadeMode = 1<<9;   // If eAudioPanFadeControl is 0, but AP turn on AD PAN FADE (default mode 1)
        }
        u4Config |= _u2AdFadeMode;  // Enable Fade & Pan
    }
    else
    {
        _u2AdFadeMode = (u2ReadShmUINT16(W_SYSTEM_SETUP) & 0x0600);
        u4Config |= 0x0;  // Disable Fade & Pan
    }

    vWriteShmUINT16(W_SYSTEM_SETUP, (u2ReadShmUINT16(W_SYSTEM_SETUP) & 0xF9FF) | u4Config);
    DSP_SendDspTaskCmd(u2UopIndex);
}

// 00: Pan disable, Fade disable
// 01: Pan enable, Fade CH7/8,CH9/10,CHL/R
// 10: Pan enable, Fade CH9/10, CHL/R
// 11: Pan enable, Fade CHL/R
UINT8 _AUD_DspGetAdPanFadeSetting(void)
{
    return ((u2ReadShmUINT16(W_SYSTEM_SETUP)&0x0600)>>9);
}

//-----------------------------------------------------------------------------
/** AUD_DspAdFadeVolume
 *  setup ad fade volume for individual channel.
 *
 *  @param  u1Value 0~255.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspAdFadeVolumeSet(UINT8 u1Value)
{
    _u1AdFadeVolume = u1Value;
}

//-----------------------------------------------------------------------------
/** _AUD_DspAdFadeVolumeUpdate
 *  setup ad fade volume for individual channel.
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
void _AUD_DspAdFadeVolumeUpdate(UINT8 u1DecId, AUD_CH_T eChannel)
{
    UINT16 u4AdFadeIndx = D_FADEVALUE;
    UINT16 u2AdFadeUop = UOP_DSP_AD_FADE;

    AUD_DEC_ID_VALIDATE(u1DecId);

    if (_u1AdFadeVolumePrev != _u1AdFadeVolume)
    {
        _u1AdFadeVolumePrev = _u1AdFadeVolume;
        VOL_CTL_SEMA_LOCK(u1DecId);
        _fgChannelVolUseShmRawData = FALSE;
        //_AUD_DspVolumeChange(u1DecId, eChannel);
        vWriteShmUINT32(u4AdFadeIndx, _DbToShmFade(_u1AdFadeVolume));
        DSP_SendDspTaskCmd(u2AdFadeUop);
        VOL_CTL_SEMA_UNLOCK(u1DecId);
    }
}

//-----------------------------------------------------------------------------
/** AUD_DspAdPanVolumeSet
 *  setup ad pan volume for individual channel.
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
 *  @param  u1Value -21~21.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspAdPanVolumeSet(UINT8 u1DecId, AUD_CH_T eChannel, UINT8 u1Value)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    AUD_ADPAN_CHANNEL_VALIDATE(eChannel);

    _aau1AdPanVol[eChannel] = u1Value;
}

//-----------------------------------------------------------------------------
/** _AUD_DspAdPanVolumeUpdate
 *  setup ad pan volume for individual channel.
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
void _AUD_DspAdPanVolumeUpdate(UINT8 u1DecId, AUD_CH_T eChannel)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    AUD_ADPAN_CHANNEL_VALIDATE(eChannel);

    if (_aau1AdPanVolPrev[eChannel] != _aau1AdPanVol[eChannel])
    {
        _aau1AdPanVolPrev[eChannel] = _aau1AdPanVol[eChannel];
        VOL_CTL_SEMA_LOCK(u1DecId);
        _fgChannelVolUseShmRawData = FALSE;
        _AUD_DspVolumeChange(u1DecId, eChannel);
        VOL_CTL_SEMA_UNLOCK(u1DecId);
    }
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
        //20101018,By Daniel,for flash-lite clip using dec3 notify path
        if (u1DecId > AUD_DEC_AUX)
            return;
        //End
		AUD_DEC_ID_VALIDATE_2(u1DecId);
#else
		AUD_DEC_ID_VALIDATE_3(u1DecId);
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
    LOG(5,"CMD: set ChannelMute: Dec(%d) Ch(%d) Mute(%d)\n",
        u1DecId, (UINT8)eChannel, (UINT8)fgMute);

#ifndef CC_MT5391_AUD_3_DECODER
    //20101018,By Daniel,for flash-lite clip using dec3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return;
    //End
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#endif
    VOL_CTL_SEMA_LOCK(u1DecId);
    _aafgChlMute[u1DecId][eChannel] = fgMute;
    _AUD_DspVolumeChange(u1DecId, eChannel);
    VOL_CTL_SEMA_UNLOCK(u1DecId);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSoundModeMute
 *  setup mute for individual channel for sound mode.
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
void _AUD_DspSoundModeMute(UINT8 u1DecId, AUD_CH_T eChannel, BOOL fgMute)
{
    LOG(5,"CMD: set SoundModeMute: Dec(%d) Ch(%d) Mute(%d)\n",
        u1DecId, (UINT8)eChannel, (UINT8)fgMute);

    AUD_DEC_ID_VALIDATE(u1DecId);

    VOL_CTL_SEMA_LOCK(u1DecId);
    _aafgSoundModeMute[u1DecId][eChannel] = fgMute;
    _AUD_DspVolumeChange(u1DecId, eChannel);
    VOL_CTL_SEMA_UNLOCK(u1DecId);
}

#ifdef CC_AUD_VOLUME_OFFSET
void _AUD_DspVolumeOffset(UINT8 u1DecId, UINT8 u1Index)
{
    INT32 i4Value;

    LOG(5,"CMD: set VolumeOffset: Dec(%d) Index(%d)\n", u1DecId, u1Index);

    AUD_DEC_ID_VALIDATE(u1DecId);

    if (u1Index > (AUD_VOLUME_OFFSET_IDX_NUM-1))
    {
        u1Index = AUD_VOLUME_OFFSET_IDX_NUM -1 ;
    }

    i4Value = _aai4VolOffsetMap[u1Index];

    _AUD_DspChannelVolGain(u1DecId, AUD_CH_FRONT_LEFT, i4Value);
    _AUD_DspChannelVolGain(u1DecId, AUD_CH_FRONT_RIGHT, i4Value);

    _AUD_DspChannelVolGain(u1DecId, AUD_CH_DMX_LEFT, i4Value);
    _AUD_DspChannelVolGain(u1DecId, AUD_CH_DMX_RIGHT, i4Value);

#ifdef CC_AUD_LINEOUT_VOL_MODE
    if (_aafgLineOutVolChangeFlag)
    {
        _AUD_DspChannelVolGain(u1DecId, AUD_CH_BYPASS_LEFT, i4Value);
        _AUD_DspChannelVolGain(u1DecId, AUD_CH_BYPASS_RIGHT, i4Value);
    }
#endif
}
#endif

void _AUD_DspClearSoundEnable(UINT8 u1DecId, BOOL fgEnable)
{
#ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
    static BOOL _fgCDNotchEnable = FALSE;
    static BOOL _fgInit = FALSE;

    if ((_fgCDNotchEnable != fgEnable) || (!_fgInit))
    {
        vWriteShmUINT8(B_CDNOTCH_FLAG, (UINT8)fgEnable);
        if (fgEnable)
        {
            vWriteShmUINT8(B_CDNOTCH_USER_Q, _au1CDNotchQIdx[0]);
            vWriteShmUINT16(W_CDNOTCH_USER_FC, _au1CDNotchFc[0]);
        }
        else
        {
            vWriteShmUINT8(B_CDNOTCH_USER_Q, _au1CDNotchQIdx[1]);
            vWriteShmUINT16(W_CDNOTCH_USER_FC, _au1CDNotchFc[1]);
        }

        DSP_SendDspTaskCmd(UOP_DSP_CDNOTCH_FLAG);
        _fgCDNotchEnable = fgEnable;
        _fgInit = TRUE;
    }
#endif

    UNUSED(u1DecId);
    UNUSED(fgEnable);
}

void _AUD_DspClearSoundCfg(UINT8 u1UserQIdx, UINT16 u2UserFc)
{
#ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
    static UINT8 _u1CDNotchQIdx = 0xff;
    static UINT16 _u2CDNotchFc = 0xffff;

    if ((_u1CDNotchQIdx == u1UserQIdx) && (_u2CDNotchFc == u2UserFc))
    {
        return;
    }

    _u1CDNotchQIdx = u1UserQIdx;
    _u2CDNotchFc = u2UserFc;

    vWriteShmUINT8(B_CDNOTCH_USER_Q, u1UserQIdx);
    vWriteShmUINT32(W_CDNOTCH_USER_FC, u2UserFc);
    DSP_SendDspTaskCmd(UOP_DSP_CDNOTCH_CFG);
#endif

    UNUSED(u1UserQIdx);
    UNUSED(u2UserFc);
}

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
    //20101018,By Daniel,for flash-lite clip using dec3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return;
    //End
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#endif

    if (eChannel >= AUD_CH_NUM)
    {
        LOG(0, "Incorrect parameter -- eChannel for _AUD_DspChannelVolGain\n");
        return;
    }

    VERIFY(x_sema_lock(_ahVolCtlSema[u1DecId], X_SEMA_OPTION_WAIT) == OSR_OK);

    // Update local variables
    _aai1ChlVolExtraGain[u1DecId][eChannel] = i4Value;
    _AudDspChannelVolChange(u1DecId, eChannel);

    VERIFY(x_sema_unlock(_ahVolCtlSema[u1DecId]) == OSR_OK);
}

//-----------------------------------------------------------------------------
/** _AUD_DspChannelVolDolbyGainTB11
 *  setup channel volume Dolby gain TB11 for individual channel.
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
void _AUD_DspChannelVolDolbyGainTB11(UINT8 u1DecId, AUD_CH_T eChannel, INT32 i4Value)
{
#ifndef CC_MT5391_AUD_3_DECODER
    //20101018,By Daniel,for flash-lite clip using dec3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return;
    //End
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#endif

    if (eChannel >= AUD_CH_NUM)
    {
        LOG(0, "Incorrect parameter -- eChannel for _AUD_DspChannelVolDolbyGainTB11\n");
        return;
    }

    VERIFY(x_sema_lock(_ahVolCtlSema[u1DecId], X_SEMA_OPTION_WAIT) == OSR_OK);

    // Update local variables
    _aai1ChlVolDolbyGainTB11[eChannel] = i4Value;
    _AudDspChannelVolChange(u1DecId, eChannel);

    VERIFY(x_sema_unlock(_ahVolCtlSema[u1DecId]) == OSR_OK);
}

/***************************************************************************
     Function : bAUD_IsDrcDec
     Description: check if the decoder has DRC functionality
     @param	u1DecId
     			AUD_DEC_MAIN: 1st dec
     			AUD_DEC_AUX: 2nd dec
     @retval	TRUE : IsDrcDec
                     FALSE: IsNotDrcDec
***************************************************************************/
BOOL _bAUD_IsDrcDec(UINT8 u1DecId)
{
    BOOL bRet = FALSE;

    if(u1DecId==AUD_DEC_MAIN)
    {
        if( (_rDspVars[u1DecId].bDspStrTyp==AC3_STREAM || _rDspVars[u1DecId].bDspStrTyp==AAC_STREAM)
            || ((_rDspVars[u1DecId].bDspStrTyp==PCM_STREAM) && ( _rDspVars[AUD_DEC_AUX].bDspStrTyp==AC3_STREAM || _rDspVars[AUD_DEC_AUX].bDspStrTyp==AAC_STREAM)))
        {
            LOG(5, "@@@@_bAUD_IsDrcDec: TRUE@@@@@\n");
            bRet = TRUE;
        }
    }

    return bRet;
}

/***************************************************************************
     Function : _AUD_DspDolbyGainTB11
     Description: Dolby Technical Bulletin 11.5 (addtional 3dB attenuation for EU)
     @param	u1DecId
     			AUD_DEC_MAIN: 1st dec
     			AUD_DEC_AUX: 2nd dec
     @retval	void
***************************************************************************/
void _AUD_DspDolbyGainTB11(UINT8 u1DecId)
{
    UINT16    u2CompMode=2;
    BOOL    bIsDrcDec;
    INT8    i1Gain[(UINT32)AUD_CH_NUM];
    UINT8    aud_ch;
    AUD_DEC_STREAM_FROM_T eStreamFrom;
    AUD_FMT_T eDecType;

    LOG(5, "\n");
    if(u1DecId==AUD_DEC_MAIN)
    {
        LOG(5, "@@@@_AUD_DspDolbyGainTB11: u1DecId=AUD_DEC_MAIN@@@@\n");
	AUD_GetDecodeType(AUD_DEC_MAIN, &eStreamFrom, &eDecType);
    }
    else
    {
        LOG(5, "@@@@_AUD_DspDolbyGainTB11: u1DecId is not equal to AUD_DEC_MAIN@@@@\n");
        LOG(5, "@@@@_AUD_DspDolbyGainTB11: No need to set DolbyGainTB11@@@@\n");

	return;
    }
    if(_rDspVars[u1DecId].bDspStrTyp==AC3_STREAM || (_rDspVars[u1DecId].bDspStrTyp==PCM_STREAM && _rDspVars[AUD_DEC_AUX].bDspStrTyp==AC3_STREAM))
    {
        u2CompMode = _AUD_DspGetAC3CompMode(u1DecId); // u2CompMode=(2=LINE Mode, 3=RF Mode)
    }
    else if(_rDspVars[u1DecId].bDspStrTyp==AAC_STREAM || (_rDspVars[u1DecId].bDspStrTyp==PCM_STREAM && _rDspVars[AUD_DEC_AUX].bDspStrTyp==AAC_STREAM))
    {
#ifdef CC_AUD_SUPPORT_MS10
        u2CompMode = _AUD_DspGetDdtDmxRfMode(u1DecId, FALSE)+2;
#endif
    }
    bIsDrcDec = _bAUD_IsDrcDec(u1DecId); // bIsDrcDec=(TRUE, FALSE)
    LOG(5, "@@@@_AUD_DspDolbyGainTB11: u2CompMode=%d, bIsDrcDec=%s@@@@", u2CompMode, (bIsDrcDec==TRUE ? "TRUE" : "FALSE"));
    // initial
    for(aud_ch=(UINT8)AUD_CH_FRONT_LEFT; aud_ch<=(UINT8)AUD_CH_DMX_RIGHT; aud_ch++)
    {
        i1Gain[(UINT32)aud_ch] = 0; // reset
    }

    if(AUD_GetTargetCountry() == COUNTRY_EU) // EU
    {
        LOG(5, "@@@@_AUD_DspDolbyGainTB11: COUNTRY_EU@@@@\n");
        if((eStreamFrom==AUD_STREAM_FROM_DIGITAL_TUNER) &&  (u2CompMode==3 || bIsDrcDec==FALSE)) // RF mode or Is not DrcDec
        {
            LOG(1, "@@@@_AUD_DspDolbyGainTB11: apply additional 8dB attenuation@@@@\n");
            i1Gain[(UINT32)AUD_CH_BYPASS_LEFT] = (INT8)(-8*2); // -8dB
            i1Gain[(UINT32)AUD_CH_BYPASS_RIGHT] = (INT8)(-8*2); // -8dB
        }
    }
    else // not EU
    {
        LOG(5, "@@@@_AUD_DspDolbyGainTB11: not COUNTRY_EU@@@@\n");
        if(u2CompMode==3 && bIsDrcDec==TRUE) // RF mode && IsDrcDec
        {
            for(aud_ch=(UINT8)AUD_CH_FRONT_LEFT; aud_ch<=(UINT8)AUD_CH_DMX_RIGHT; aud_ch++)
            {
                LOG(5, "@@@@@_AUD_DspDolbyGainTB11: compensate +3dB@@@@@\n");
                i1Gain[(UINT32)aud_ch] = (INT8)(3*2); // +3dB
            }
        }
    }
    LOG(5, "@@@@_AUD_DspDolbyGainTB11: Set Channel Volumn Gain@@@@\n");
    // set Channel Volumn Gain
    for(aud_ch=(UINT8)AUD_CH_FRONT_LEFT; aud_ch<=(UINT8)AUD_CH_DMX_RIGHT; aud_ch++)
    {
        LOG(5, "@@@@i1Gain[%d] = %d@@@@\n", aud_ch, (INT32)i1Gain[(UINT32)aud_ch]);
        _AUD_DspChannelVolDolbyGainTB11(AUD_DEC_MAIN, (AUD_CH_T)aud_ch, (INT32)i1Gain[(UINT32)aud_ch]);
    }
}

INT32 _AUD_DspGetChannelVolGain(UINT8 u1DecId, AUD_CH_T eChannel)
{
    return _aai1ChlVolExtraGain[u1DecId][eChannel];
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
    LOG(5,"CMD: set Balance: Dec(%d) Index(%d)\n", u1DecId, u1Balance);

    AUD_DEC_ID_VALIDATE_2(u1DecId);
    VOL_CTL_SEMA_LOCK(u1DecId);

    _au1Balance[u1DecId] = u1Balance;

    _AUD_DspVolumeChange(u1DecId, AUD_CH_FRONT_LEFT);
    _AUD_DspVolumeChange(u1DecId, AUD_CH_FRONT_RIGHT);
    _AUD_DspVolumeChange(u1DecId, AUD_CH_REAR_LEFT);
    _AUD_DspVolumeChange(u1DecId, AUD_CH_REAR_RIGHT);
    _AUD_DspVolumeChange(u1DecId, AUD_CH_DMX_LEFT);
    _AUD_DspVolumeChange(u1DecId, AUD_CH_DMX_RIGHT);

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
    AUD_DEC_ID_VALIDATE_2(u1DecId);
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
    LOG(5,"CMD: set Master Volume: Dec(%d) Volume(%d)\n", u1DecId, u1Volume);

#if defined(CC_AUD_4_DECODER_SUPPORT)
    AUD_DEC_ID_VALIDATE(u1DecId);
    #if defined(CC_MT5365) || defined(CC_MT5395) //5368/96 support 4th dec post-proc
    if ((u1DecId == AUD_DEC_4TH)
        #ifndef CC_MT5391_AUD_3_DECODER
        //20101018,By Daniel,for flash-lite clip using dec3 notify path
        ||(u1DecId == AUD_DEC_THIRD)
        //End
        #endif
        )
        return;
    #endif
#elif defined(CC_MT5391_AUD_3_DECODER)
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#else
    #if defined(CC_MT5365) || defined(CC_MT5395) //5368/96 support 4th dec post-proc
    //20101018,By Daniel,for flash-lite clip using dec3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return;
    //End
    #endif
    AUD_DEC_ID_VALIDATE_2(u1DecId);
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

UINT8 _AUD_DspGetMasterVolume(UINT8 u1DecId)
{
    return _aau1ChannelVolume[u1DecId][AUD_CH_ALL];
}

void _AUD_DspMixSndVolume(UINT8 u1DecId, UINT8 u1Volume)
{
    UINT32 u4ValShm;
    UINT64 u8ValueShm;
    BOOL fgMute;

	LOG(9, "_AUD_DspMixSndVolume(), u1DecId(%d), Vol (%d)\n", u1DecId, u1Volume);

	_au4AdjVol[u1DecId] = VOL_SHM_0_DB;
    fgMute = (_afgMuteEnable[u1DecId]||_afgStopMuteEnable[u1DecId])? TRUE : FALSE;

    // Master volume = Mute * AdjVol * Master volume
    u8ValueShm = (((UINT64)_au4VOLUME[u1Volume]) * ((UINT64)_au4AdjVol[u1DecId]))/(UINT64)VOL_SHM_0_DB;

#ifdef CC_AUD_VOLUME_OFFSET
    // Apply volume offset
    u8ValueShm = ((UINT64)u8ValueShm * (UINT64)(_DbToShm((INT32)_aai1ChannelVolOffset[u1DecId][AUD_CH_ALL]))) / (UINT64)VOL_SHM_0_DB;
#endif
    u4ValShm = (UINT32)u8ValueShm;

    if (u4ValShm > _au4VOLUME[100])
    {
        u4ValShm = _au4VOLUME[100];
    }

    u4ValShm = fgMute ? 0 : u4ValShm;

	DSP_SetMixsndVolume(u4ValShm);

    // Send UOP to DSP
    //_VolumeCtrl(u1DecId, AUD_CH_ALL, u4ValShm);

	#if 0
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
	#endif
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
    //20101018,By Daniel,for flash-lite clip using dec3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return;
    //End
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_3(u1DecId);
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

    LOG(5,"CMD: set AVCEnable: Dec(%d) Enable(%d)\n", u1DecId, (UINT8)fgEnable);

    AUD_DEC_ID_VALIDATE_2(u1DecId);
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
    _AUD_DspVolumeChange(u1DecId,AUD_CH_FRONT_LEFT);
    _AUD_DspVolumeChange(u1DecId,AUD_CH_FRONT_RIGHT);
    _AUD_DspVolumeChange(u1DecId,AUD_CH_DMX_LEFT);
    _AUD_DspVolumeChange(u1DecId,AUD_CH_DMX_RIGHT);
    
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

    AUD_DEC_ID_VALIDATE_2(u1DecId);
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

    if (u1DecId != AUD_DEC_MAIN)
    {
        ASSERT(0);
    }

    switch (u2Type)
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

    if (u1DecId != AUD_DEC_MAIN)
    {
        ASSERT(0);
    }

    switch (u2Type)
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

    if (u1DecId != AUD_DEC_MAIN)
    {
        ASSERT(0);
    }

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

    _AUD_DspChangeAVCPara(1, _ai2AvcTargetLevel[(UINT8)eStreamFrom], AUD_DEC_MAIN);
    _AUD_DspChangeAVCPara(3, _ai2AvcGainUp[(UINT8)eStreamFrom], AUD_DEC_MAIN);
    _AUD_DspChangeAVCPara(5, _ai2AvcAdjRate[(UINT8)eStreamFrom], AUD_DEC_MAIN);
    _AUD_DspChangeAVCPara(6, _ai2AvcUiAdjRate[(UINT8)eStreamFrom], AUD_DEC_MAIN);
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
    //20101018,By Daniel,for flash-lite clip using dec3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return;
    //End
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_3(u1DecId);
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
/** _AUD_DspMuteAllEnable
 *  audio mute control.
 *
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *  @param  fgEnable   Mute/unmute. (1: mute audio, 0: unmute audio)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspMuteAllEnable(UINT8 u1DecId, BOOL fgEnable)
{
#ifndef CC_MT5391_AUD_3_DECODER  //gallen 0821
    //20101018,By Daniel,for flash-lite clip using dec3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return;
    //End
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#endif

    VOL_CTL_SEMA_LOCK(u1DecId);

    if (_afgAllChannelMute[u1DecId] != fgEnable)
    {
         _afgAllChannelMute[u1DecId] = fgEnable;
        _AUD_DspVolumeChange(u1DecId, AUD_CH_FRONT_LEFT);
        _AUD_DspVolumeChange(u1DecId, AUD_CH_FRONT_RIGHT);
        _AUD_DspVolumeChange(u1DecId, AUD_CH_REAR_LEFT);
        _AUD_DspVolumeChange(u1DecId, AUD_CH_REAR_RIGHT);
        _AUD_DspVolumeChange(u1DecId, AUD_CH_DMX_LEFT);
        _AUD_DspVolumeChange(u1DecId, AUD_CH_DMX_RIGHT);
        _AUD_DspVolumeChange(u1DecId, AUD_CH_BYPASS_LEFT);
        _AUD_DspVolumeChange(u1DecId, AUD_CH_BYPASS_RIGHT);
    }

    VOL_CTL_SEMA_UNLOCK(u1DecId);

    if (_afgIecMute != fgEnable)
    {
        _afgIecMute = fgEnable;
        _AUD_DspIECConfig(_aeMWIec, _aafgMWSetSpdifFlag);
    }

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
    //20101018,By Daniel,for flash-lite clip using dec3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return;
    //End
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_3(u1DecId);
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
    AUD_DEC_ID_VALIDATE_2(u1DecId);
    VOL_CTL_SEMA_LOCK(u1DecId);
    _fgHdmiMuteDelay = FALSE;

    _afgHdmiModeMuteEnable[u1DecId] = fgEnable;
    _AUD_DspVolumeChange(u1DecId, AUD_CH_ALL);
    _fgHdmiMuteDelay = TRUE;

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
    AUD_DEC_ID_VALIDATE_2(u1DecId);
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
#ifdef CC_AUD_HDMI_SPDIF_CFG
//-----------------------------------------------------------------------------
/** AUD_DspHdmiMuteAnalogOutput
 *  audio mute control by HDMI change status.
 *
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *  @param  fgEnable   Mute/unmute. (1: mute audio, 0: unmute audio)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
 void AUD_DspHdmiMuteAnalogOutput(UINT8 u1DecId, BOOL fgEnable)
{
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
    if (_ahVolCtlSema[u1DecId] == NULL)
    {
        return;
    }

    VOL_CTL_SEMA_LOCK(u1DecId);


    _aafgChlHdmiMuteAnalogOut[u1DecId][AUD_CH_BYPASS_LEFT] = fgEnable;
    _aafgChlHdmiMuteAnalogOut[u1DecId][AUD_CH_BYPASS_RIGHT] = fgEnable;
    _aafgChlHdmiMuteAnalogOut[u1DecId][AUD_CH_DMX_LEFT] = fgEnable;
    _aafgChlHdmiMuteAnalogOut[u1DecId][AUD_CH_DMX_RIGHT] = fgEnable;

    //Lineout
    _AUD_DspVolumeChange(u1DecId, AUD_CH_BYPASS_LEFT);
    _AUD_DspVolumeChange(u1DecId, AUD_CH_BYPASS_RIGHT);
    //Headphone
    _AUD_DspVolumeChange(u1DecId, AUD_CH_DMX_LEFT);
    _AUD_DspVolumeChange(u1DecId, AUD_CH_DMX_RIGHT);

    VOL_CTL_SEMA_UNLOCK(u1DecId);
}
#endif

#ifdef CC_ENABLE_AV_SYNC
 void AUD_DspDecPlayMuteEnable(UINT8 u1DecId, BOOL fgEnable)
{
    if (fgEnable != _fgDecPlayMute)
    {
        AUD_DEC_ID_VALIDATE_2(u1DecId);
        VOL_CTL_SEMA_LOCK(u1DecId);

        _aafgChDecPlayMute[u1DecId][AUD_CH_FRONT_LEFT] = fgEnable;
        _aafgChDecPlayMute[u1DecId][AUD_CH_FRONT_RIGHT] = fgEnable;
        _aafgChDecPlayMute[u1DecId][AUD_CH_BYPASS_LEFT] = fgEnable;
        _aafgChDecPlayMute[u1DecId][AUD_CH_BYPASS_RIGHT] = fgEnable;
        _aafgChDecPlayMute[u1DecId][AUD_CH_DMX_LEFT] = fgEnable;
        _aafgChDecPlayMute[u1DecId][AUD_CH_DMX_RIGHT] = fgEnable;
		_aafgChDecPlayMute[u1DecId][AUD_CH_REAR_LEFT] = fgEnable;
		_aafgChDecPlayMute[u1DecId][AUD_CH_REAR_RIGHT] = fgEnable;

        _AUD_DspVolumeChange(u1DecId, AUD_CH_FRONT_LEFT);
        _AUD_DspVolumeChange(u1DecId, AUD_CH_FRONT_RIGHT);
        _AUD_DspVolumeChange(u1DecId, AUD_CH_BYPASS_LEFT);
        _AUD_DspVolumeChange(u1DecId, AUD_CH_BYPASS_RIGHT);
        _AUD_DspVolumeChange(u1DecId, AUD_CH_DMX_LEFT);
        _AUD_DspVolumeChange(u1DecId, AUD_CH_DMX_RIGHT);
		_AUD_DspVolumeChange(u1DecId, AUD_CH_REAR_LEFT);
        _AUD_DspVolumeChange(u1DecId, AUD_CH_REAR_RIGHT);

        VOL_CTL_SEMA_UNLOCK(u1DecId);
    }

    _fgDecPlayMute = fgEnable;
}
#endif

void _AUD_DspAtvMuteEnable(UINT8 u1DecId, BOOL fgEnable)
{
    if (fgEnable != _fgAtvMute)
    {
        AUD_DEC_ID_VALIDATE_2(u1DecId);
        VOL_CTL_SEMA_LOCK(u1DecId);

        _aafgChAtvMute[u1DecId][AUD_CH_FRONT_LEFT] = fgEnable;
        _aafgChAtvMute[u1DecId][AUD_CH_FRONT_RIGHT] = fgEnable;
        _aafgChAtvMute[u1DecId][AUD_CH_BYPASS_LEFT] = fgEnable;
        _aafgChAtvMute[u1DecId][AUD_CH_BYPASS_RIGHT] = fgEnable;
        _aafgChAtvMute[u1DecId][AUD_CH_DMX_LEFT] = fgEnable;
        _aafgChAtvMute[u1DecId][AUD_CH_DMX_RIGHT] = fgEnable;
		_aafgChAtvMute[u1DecId][AUD_CH_REAR_LEFT] = fgEnable;
		_aafgChAtvMute[u1DecId][AUD_CH_REAR_RIGHT] = fgEnable;

        _AUD_DspVolumeChange(u1DecId, AUD_CH_FRONT_LEFT);
        _AUD_DspVolumeChange(u1DecId, AUD_CH_FRONT_RIGHT);
        _AUD_DspVolumeChange(u1DecId, AUD_CH_BYPASS_LEFT);
        _AUD_DspVolumeChange(u1DecId, AUD_CH_BYPASS_RIGHT);
        _AUD_DspVolumeChange(u1DecId, AUD_CH_DMX_LEFT);
        _AUD_DspVolumeChange(u1DecId, AUD_CH_DMX_RIGHT);
		_AUD_DspVolumeChange(u1DecId, AUD_CH_REAR_LEFT);
        _AUD_DspVolumeChange(u1DecId, AUD_CH_REAR_RIGHT);

        VOL_CTL_SEMA_UNLOCK(u1DecId);
    }

    _fgAtvMute = fgEnable;
}

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

BOOL _AUD_DspGetBassMngEnable(UINT8 u1DecId)
{
    UINT16 u2ShmIndex = W_PROCMOD;

    if (u1DecId != AUD_DEC_MAIN)
    {
        u2ShmIndex = W_PROCMOD_DEC2;
    }

    if (u2ReadShmUINT16(u2ShmIndex) & BYPASS_BASS_MANAGEMENT)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

BOOL _AUD_DspGetChannelVolumeEnable(UINT8 u1DecId)
{
    UINT16 u2ShmIndex = W_PROCMOD;
    if (u1DecId != AUD_DEC_MAIN)
    {
        u2ShmIndex = W_PROCMOD_DEC2;
    }

    if (u2ReadShmUINT16(u2ShmIndex) & BYPASS_CHANNEL_VOLUME)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

BOOL _AUD_DspGetDelayEnable(UINT8 u1DecId)
{
    UINT16 u2ShmIndex = W_PROCMOD;
    if (u1DecId != AUD_DEC_MAIN)
    {
        u2ShmIndex = W_PROCMOD_DEC2;
    }

    if (u2ReadShmUINT16(u2ShmIndex) & BYPASS_DELAY)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

BOOL _AUD_DspGetPostProcessingEnable(UINT8 u1DecId)
{
    UINT16 u2ShmIndex = W_PROCMOD;
    if (u1DecId == AUD_DEC_AUX)
    {
        u2ShmIndex = W_PROCMOD_DEC2;
    }
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u2ShmIndex = W_PROCMOD_DEC3;
    }
    else if (u1DecId == AUD_DEC_4TH)
    {
        u2ShmIndex = W_PROCMOD_DEC4;
    }

    if (u2ReadShmUINT16(u2ShmIndex) & BYPASS_POST_PROCESSING)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

void _AUD_DspProcMode(UINT8 u1DecId, UINT16 u2Mode)
{
    UINT16 u2ShmIndex = W_PROCMOD;
    UINT16 u2UopIndex = UOP_DSP_BYPASS_PROC_MODE;

#if defined(CC_AUD_4_DECODER_SUPPORT)
    AUD_DEC_ID_VALIDATE(u1DecId);
    #if defined(CC_MT5365) || defined(CC_MT5395) //5368/96 support 4th dec post-proc
    if ((u1DecId == AUD_DEC_4TH)
        #ifndef CC_MT5391_AUD_3_DECODER
        //20101018,By Daniel,for flash-lite clip using dec3 notify path
        ||(u1DecId == AUD_DEC_THIRD)
        //End
        #endif
        )
        return;
    #endif
#elif defined(CC_MT5391_AUD_3_DECODER)
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#else
    #if defined(CC_MT5365) || defined(CC_MT5395) //5368/96 support 4th dec post-proc
    //20101018,By Daniel,for flash-lite clip using dec3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return;
    //End
    #endif
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#endif

    if (u1DecId == AUD_DEC_AUX)
    {
        u2ShmIndex = W_PROCMOD_DEC2;
        u2UopIndex = UOP_DSP_BYPASS_PROC_MODE_DEC2;
    }
#ifdef CC_MT5391_AUD_3_DECODER
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u2ShmIndex = W_PROCMOD_DEC3;
        u2UopIndex = UOP_DSP_BYPASS_PROC_MODE_DEC3;
    }
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    else if (u1DecId == AUD_DEC_4TH)
    {
        u2ShmIndex = W_PROCMOD_DEC4;
        u2UopIndex = UOP_DSP_BYPASS_PROC_MODE_DEC4;
    }
#endif

    vWriteShmUINT16(u2ShmIndex, u2Mode);
    DSP_SendDspTaskCmd(u2UopIndex);
}

#ifdef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
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
		   LOG(6,"CMD: _AUD_DspVSModeChange: eVSCfgType(0x%x) eVSCfgType(0x%x)\n", eVSCfgType, u4VSParameter);
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
#endif //CC_AUD_NEW_POST_MULTI_VS_SUPPORT
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
void _AUD_DspPostSoundMode(UINT8 u1DecId, UINT8 u1ModeIdx, BOOL fgWithMute)
{
	UINT8 u1ModeCase;
    AUD_DEC_STREAM_FROM_T eStreamFrom;

    UNUSED(u1ModeCase);

    AUD_DEC_ID_VALIDATE(u1DecId);

    LOG(5,"CMD: set SoundMode: Dec(%d) Mode(%d)\n", u1DecId, u1ModeIdx);

    _au1MWSetSoundModeIdx = u1ModeIdx;

#ifdef CC_AUD_USE_FLASH_AQ
  #ifndef CC_AUD_NEW_AQ_PARSER
    u1ModeIdx = _au1AQSmod_Convert[u1ModeIdx];
  #endif //CC_AUD_NEW_AQ_PARSER
#endif

    if (u1ModeIdx > (AUD_SOUND_MODE_NUM-1))
    {
        u1ModeIdx = AUD_SOUND_MODE_NUM - 1;
    }

    AUD_DRVGetStreamFrom(u1DecId, &eStreamFrom)	;

	VERIFY(eStreamFrom < AUD_STREAM_FROM_NUM);

#ifdef CC_AUD_SOUND_MODE
    u1ModeCase = _arSoundModeCase[u1ModeIdx][eStreamFrom];
    _AUD_DspPostSoundModeChange(u1DecId, u1ModeCase);
#else
    u1ModeCase = _arSoundModeCase[u1ModeIdx][eStreamFrom];
    if(_au1SoundModeCurrentIdx == u1ModeCase)
    {
        LOG(5, "Redundent sound mode setting!\n");
        return;
    }
    else
    {
        _au1SoundModeCurrentIdx = u1ModeCase;
    }
#endif //CC_AUD_SOUND_MODE

    if (fgWithMute)
    {
        _AUD_DspSoundModeMute(u1DecId, AUD_CH_FRONT_LEFT, TRUE);
        _AUD_DspSoundModeMute(u1DecId, AUD_CH_FRONT_RIGHT, TRUE);
    }

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

//-----------------------------------------------------------------------------
/** _AUD_DspPostSoundModeStart
 *  Indicate sound mode start/end
 *
 *  @param  u1DecId          Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  fgStart          start(TRUE), end(FALSE).
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
void _AUD_DspPostSoundModeStart(UINT8 u1DecId, BOOL fgStart)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    if (fgStart)
    {
        // Move to set sound mode to avoid duplicated mute sound
        //_AUD_DspSoundModeMute(u1DecId, AUD_CH_FRONT_LEFT, TRUE);
        //_AUD_DspSoundModeMute(u1DecId, AUD_CH_FRONT_RIGHT, TRUE);
    }
    else
    {
        _AUD_DspSoundModeMute(u1DecId, AUD_CH_FRONT_LEFT, FALSE);
        _AUD_DspSoundModeMute(u1DecId, AUD_CH_FRONT_RIGHT, FALSE);
    }
}

#ifndef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
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
    // SRS TSXT or SRS TSHD
    UINT16 u2ShmIndex = B_VSURRFLAG;
    UINT16 u2UopIndex = UOP_DSP_SRS_TSXT_FLAG;
    UINT8 u1Flag = uReadShmUINT8(u2ShmIndex);
    
    if(AUD_IsSupportSRS())    // SRS TSXT or SRS TSHD
    {
        LOG(5,"CMD: set SRSEnable: Dec(%d) Enable(%d)\n", u1DecId, (UINT8)fgEnable);

        if (u1DecId == AUD_DEC_MAIN)
        {
            if (fgEnable)
            {
                //AUD_DspSpeakerOutputConfig(0, 0); // 2/0 LtRt // FIXME
                if ( (u1Flag & (0x01<<5)) != 0)  //VSS_SRS_TSXT
                {
                    return;
                }
                vWriteShmUINT8(u2ShmIndex, (UINT8)(0x01<<5)|(UINT8)(fgEnable));
            }
            else
            {
                //AUD_DspSpeakerOutputConfig(0, 2);
                if ( (u1Flag & (0x01<<5)) == 0)
                {
                    return;
                }
                vWriteShmUINT8(u2ShmIndex, 0);
            }
            DSP_SendDspTaskCmd(u2UopIndex);
        }
    }
    else
    {
// MTK Virtual Surround
        u2ShmIndex = B_VSURRFLAG;
        u2UopIndex = UOP_DSP_VIRTUAL_SURROUND_FLAG;

        LOG(5,"CMD: set VSEnable: Dec(%d) Enable(%d)\n", u1DecId, (UINT8)fgEnable);

        if (u1DecId != AUD_DEC_MAIN)
        {
            u2ShmIndex = B_VSURRFLAG_DEC2;
    	    u2UopIndex = UOP_DSP_VIRTUAL_SURROUND_FLAG_DEC2;
        }

        vWriteShmUINT8(u2ShmIndex, (UINT8)fgEnable);
        DSP_SendDspTaskCmd(u2UopIndex);
    }
}

#else // CC_AUD_NEW_POST_MULTI_VS_SUPPORT
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
			AUD_DspSpeakerOutputConfig(0, 0);
			vWriteShmUINT16(u2ShmIndex, 0);
		}
		DSP_SendDspTaskCmd(u2UopIndex);
	}
#else //DSP_SUPPORT_SRSTSXT

    UINT16 u2ShmIndex = W_VSURRFLAG;
    UINT16 u2UopIndex = UOP_DSP_VIRTUAL_SURROUND_FLAG;

    LOG(5,"CMD: set MVSEnable: Dec(%d) Mode(%d)\n", u1DecId, u1ModeIdx);

    if (u1DecId != AUD_DEC_MAIN)
    {
        u2ShmIndex = W_VSURRFLAG_DEC2;
        u2UopIndex = UOP_DSP_VIRTUAL_SURROUND_FLAG_DEC2;
    }

	if (u1ModeIdx == 2)
    {
        vWriteShmUINT16(u2ShmIndex, (UINT16)(0x01<<8));
    }
	else if (u1ModeIdx < 2)
    {
        vWriteShmUINT16(u2ShmIndex, u1ModeIdx);
    }
    else
    {
        vWriteShmUINT16(u2ShmIndex, 0x01);
    }

    if (u1ModeIdx != 2)
    {
        if (u1ModeIdx > 2)
        {
            u1ModeIdx = u1ModeIdx - 2 ;
        }
        else
        {
            u1ModeIdx = 0;
        }

        _AUD_DspVSModeChange(u1DecId,u1ModeIdx);
    }
    DSP_SendDspTaskCmd(u2UopIndex);
#endif
}
#endif //CC_AUD_NEW_POST_MULTI_VS_SUPPORT

BOOL _AUD_DspGetVirtualSurroundEnalbe(UINT8 u1DecId)
{
#ifndef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
    UINT16 u2ShmIndex = B_VSURRFLAG;
#else
    UINT16 u2ShmIndex = W_VSURRFLAG;
#endif
    if (u1DecId != AUD_DEC_MAIN)
    {
#ifndef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
	    u2ShmIndex = B_VSURRFLAG_DEC2;
#else
	    u2ShmIndex = W_VSURRFLAG_DEC2;
#endif
    }

    if (uReadShmUINT8(u2ShmIndex))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

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
#ifndef CC_AUD_SUPPORT_MS10
    UINT8   u1ADFLAGValue;
    UINT16 u2ShmIndex = B_ADFLAG;
    UINT32 u2UopIndex = UOP_DSP_AD_FLAG;

    u1ADFLAGValue = (uReadShmUINT8(B_ADFLAG) & (0xBE)) | fgEnable |(fgEnable<<6);

#ifndef CC_MT5391_AUD_3_DECODER
    //20101018,By Daniel,for flash-lite clip using dec3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return;
    //End
#endif

    vWriteShmUINT8(u2ShmIndex, (UINT8)u1ADFLAGValue);
    DSP_SendDspTaskCmd(u2UopIndex);
#else
#ifndef CC_MT5391_AUD_3_DECODER
    //20101018,By Daniel,for flash-lite clip using dec3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return;
    //End
#else

    UINT8   u1ADFLAGValue,u1OldAdFlag;
    UINT16 u2ShmIndex = B_ADFLAG;
    UINT32 u2UopIndex = UOP_DSP_AD_FLAG;
    DSP_SetTriDecMode(fgEnable);
    u1OldAdFlag = uReadShmUINT8(B_ADFLAG);
    u1ADFLAGValue = ((u1OldAdFlag & (0xBE)) | fgEnable) | 0xc0;      //clear bit6: one buffer mode, set bit7: AD do sample-by-sample
    if (u1OldAdFlag != u1ADFLAGValue)
    {
        vWriteShmUINT8(u2ShmIndex, (UINT8)u1ADFLAGValue);
        if (!fgEnable)
        {
            DSP_SendDspTaskCmd(u2UopIndex);
        }
    }
#endif
#endif
#ifndef CC_MT5391_AUD_3_DECODER
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

#ifdef CC_AUD_SKYPE_SUPPORT
//-----------------------------------------------------------------------------
/** _AUD_DspSkypeRingMix
 *  Enable Ring sound mix for skype application. Note that enable this means to enable
 *  "dec3->dec1 mixsing".
 *
 *  @param  fgEnable 0: disable 1: enable.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSkypeRingMix(BOOL fgEnable)
{
    UINT16 u2ShmIndex = B_ADFLAG;
    UINT32 u2UopIndex = UOP_DSP_AD_FLAG;
    UINT8   u1ADFLAGValue;

     u1ADFLAGValue = uReadShmUINT8(B_ADFLAG);

    if (fgEnable)
    {
        u1ADFLAGValue |= 0xB ;  // A =>Ring sound will be added to both L/R and CH9/10;
                                              // 1=> audio descript enable
    }
    else
    {
        u1ADFLAGValue &= 0xFFF0 ;
    }

    vWriteShmUINT8(u2ShmIndex, (UINT8)u1ADFLAGValue);
    DSP_SendDspTaskCmd(u2UopIndex);

}

//-----------------------------------------------------------------------------
/** _AUD_DspAuxMixToMain
 *  This function enables the mixing from AUX to Main, by setting internal_buffer_input_flag[9] and system_ctrl[0].
 *  "dec2->dec1 mixsing".
 *
 *  @param  fgEnable 0: disable 1: enable.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspAuxMixToMain(BOOL fgEnable)
{
    UINT8   u2Value;
    UINT32 u2UopIndex = UOP_DSP_AUX_MIX_MAIN;

     u2Value = u2ReadShmUINT16 (W_INTERNAL_BUF_IN_FLAG);

    if (fgEnable)
    {
        vWriteShmUINT16 (W_INTERNAL_BUF_IN_FLAG, (u2Value|0x200));  // bit 9: read from internal buffer, after volume
    }
    else
    {
        vWriteShmUINT16 (W_INTERNAL_BUF_IN_FLAG, (u2Value&0xFDFF));
    }
    DSP_SetDualDecMode(fgEnable);
    DSP_SetAuxMixMainMode(fgEnable);

    DSP_SendDspTaskCmd(u2UopIndex);

}
#endif

#ifdef CC_AUD_PCM_LINE_IN_LPF
void _AUD_DSPSetPCMLineInLPF(BOOL fgEnable)
{
    DSP_SetPCMLineInLPF(fgEnable);
    return;
}
#endif


#ifdef CC_AUD_BBE_SUPPORT
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

    AUD_DEC_ID_VALIDATE_2(u1DecId);

    // BBE is only for main decoder
    if (u1DecId == AUD_DEC_MAIN)
    {
       _u1BbeMode = u1BbeMode;
        vWriteShmUINT8(u2ShmIndex, u1BbeMode);
        DSP_SendDspTaskCmd(u2UopIndex);
    }
}
#endif

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

UINT8 _AUD_DspGetMatrixOutMode(UINT8 u1DecId)
{
    UINT16 u2ShmIndex=B_MANUAL_OUTPUT_MATRIX;
    if (u1DecId != AUD_DEC_MAIN)
    {
        u2ShmIndex = B_MANUAL_OUTPUT_MATRIX_DEC2;
    }

    return uReadShmUINT8(u2ShmIndex);
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
    UINT32 u2AoutReinitUopIndex = UOP_DSP_AOUT_REINIT;

    //CLI_Parser("aud.uop.spkuop.ch 0 2"); // switch speaker config to 2/0 Lo/Ro
    //AUD_DspSpeakerOutputConfig(u1DecId, 2);

    if (u1DecId != AUD_DEC_MAIN)
    {
        u2ShmIndex = B_AC3AUTODNMIX_DEC2;
        u2UopIndex = UOP_DSP_AC3_AC3AUTODNMIX_DEC2;
        u2AoutReinitUopIndex = UOP_DSP_AOUT_REINIT_DEC2;
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
    DSP_SendDspTaskCmd(u2AoutReinitUopIndex);
}

//-----------------------------------------------------------------------------
/** AUD_GetDspAutoDmx
 *  get auto downmix mode for AC3
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  fgEnable  : 0: disable AC3 auto downmix, 1: enable AC3 auto downmix
 *  @retval void
 */
//-----------------------------------------------------------------------------
BOOL _AUD_GetDspAutoDmx (UINT8 u1DecId)
{
    UINT16 u2ShmIndex = B_AC3AUTODNMIX;

    if (u1DecId != AUD_DEC_MAIN)
    {
        u2ShmIndex = B_AC3AUTODNMIX_DEC2;
    }

   return ((BOOL) uReadShmUINT8(u2ShmIndex));
}

//-----------------------------------------------------------------------------
/** _AUD_DspBypassPostProcess
 *  Bypass audio post-processing.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u2BypassMode: 1: bypass post-processing.
 *                      0x8000: bypass delay
 *                      0x4000: bypass trim
 *                      0x2000: bypass post-processing
 *                      0x1000: bypass bass management
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspBypassPostProcess(UINT8 u1DecId, UINT16 u2BypassMode)
{
    UINT16 u2Mode;
    UINT16 u2ShmIndex = W_PROCMOD;
    UINT32 u2UopIndex = UOP_DSP_BYPASS_PROC_MODE;

#if defined(CC_AUD_4_DECODER_SUPPORT)
    AUD_DEC_ID_VALIDATE(u1DecId);
    #if defined(CC_MT5365) || defined(CC_MT5395) //5368/96 support 4th dec post-proc
    if ((u1DecId == AUD_DEC_4TH)
    #ifndef CC_MT5391_AUD_3_DECODER
        //20101018,By Daniel,for flash-lite clip using dec3 notify path
        ||(u1DecId == AUD_DEC_THIRD)
        //End
    #endif
        )
        return;
    #endif
#elif defined(CC_MT5391_AUD_3_DECODER)
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#else
    #if defined(CC_MT5365) || defined(CC_MT5395) //5368/96 support 4th dec post-proc
    //20101018,By Daniel,for flash-lite clip using dec3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return;
    //End
    #endif
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#endif

    //u2Mode = fgBypass ? 0xfc00 : 0xc00;
    if (u2BypassMode == 1)
    {
        u2Mode = 0xf000;
    }
    else
    {
        u2Mode = (u2BypassMode & 0xf000);
    }

    if (u1DecId == AUD_DEC_AUX)
    {
        u2ShmIndex = W_PROCMOD_DEC2;
        u2UopIndex = UOP_DSP_BYPASS_PROC_MODE_DEC2;
    }
#ifdef CC_MT5391_AUD_3_DECODER
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u2ShmIndex = W_PROCMOD_DEC3;
        u2UopIndex = UOP_DSP_BYPASS_PROC_MODE_DEC3;
    }
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    else if (u1DecId == AUD_DEC_4TH)
    {
        u2ShmIndex = W_PROCMOD_DEC4;
        u2UopIndex = UOP_DSP_BYPASS_PROC_MODE_DEC4;
    }
#endif

    vWriteShmUINT16(u2ShmIndex, ((u2ReadShmUINT16(u2ShmIndex)&0x0fff) | u2Mode));
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
/** _AUD_IsSpdifTypeOff
 *  Is Spdif type is off
 *
 *  @retval  TRUE
 */
//-----------------------------------------------------------------------------
BOOL _AUD_IsSpdifTypeOff(void)
{
    return _aafgMWSetSpdifFlag;
}


//-----------------------------------------------------------------------------
/** _AUD_IsIECRaw
 *  Is IEC output RAW data.
 *
 *  @retval  TRUE
 */
//-----------------------------------------------------------------------------
BOOL _AUD_IsIECRaw(void)
{
    return ((_aeIecMode ==  AUD_IEC_CFG_RAW) & (_afgIecEnable == TRUE));
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
/** _AUD_DspSetIecRawFlag
 *  Enable/disable sync IEC RAW delay to channel delay automatically.
 *
 *  @param  u1Flag
 *  @retval  N/A
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetIecRawFlag (UINT8 u1Flag)
{
    vWriteShmUINT8(B_IECRAW_DELAY_FLAG, u1Flag);
    DSP_SendDspTaskCmd(UOP_DSP_IEC_FLAG);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetIecRawDelay
 *  Set IEC RAW delay.
 *
 *  @param  i2Delay (ms)
 *  @retval  N/A
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetIecRawDelay (INT16 i2Delay)
{
    vWriteShmUINT16(W_IEC_RAW_OFFSET, (UINT16)i2Delay);
    DSP_SendDspTaskCmd(UOP_DSP_IEC_FLAG);
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
/** _AUD_DspResetSpdifReg
 *   get SPDIF register type.
 *
 *  @param  SPDIF_REG_TYPE_T
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspResetSpdifReg (SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    _aSpdifInfo.u1Copyright[type][eStreamFrom] = _aSpdifInfo.u1Copyright[SPDIF_REG_TYPE_DEFAULT][eStreamFrom];
    _aSpdifInfo.u1Category[type][eStreamFrom] = _aSpdifInfo.u1Category[SPDIF_REG_TYPE_DEFAULT][eStreamFrom];
    _aSpdifInfo.u1WordLength[type][eStreamFrom] = _aSpdifInfo.u1WordLength[SPDIF_REG_TYPE_DEFAULT][eStreamFrom];
    _aSpdifInfo.eUpdateMode[type][eStreamFrom] = _aSpdifInfo.eUpdateMode[SPDIF_REG_TYPE_DEFAULT][eStreamFrom];

    _AUD_DspSetSpdifChannelStatus (type);
}

#ifndef CC_AUD_SUPPORT_SPDIF_V20
//-----------------------------------------------------------------------------
/** _AUD_DspSetSpdifRegType
 *   get SPDIF register type.
 *
 *  @param  SPIDF_REG_TYPE_T
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetSpdifRegType (SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    UNUSED(type);
    UNUSED(eStreamFrom);
}
#endif

//-----------------------------------------------------------------------------
/** _AUD_DspSetSpdifChannelStatus
 *   get SPDIF register type.
 *
 *  @param  SPDIF_REG_TYPE_T
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetSpdifChannelStatus (SPDIF_REG_TYPE_T type)
{
    AUD_DEC_STREAM_FROM_T eStreamFrom = _aeStreamSrc[AUD_DEC_MAIN]; // Now, it is only for main decoder.

    if (type >= SPDIF_REG_TYPE_NUM)
    {
        type = SPDIF_REG_TYPE_DEFAULT;
    }
    if (eStreamFrom >= AUD_STREAM_FROM_NUM)
    {
        eStreamFrom = AUD_STREAM_FROM_OTHERS;
    }

    vWriteShmUINT8(B_IEC_COPYRIGHT, IEC_COPY_NEVER);
    vWriteShmUINT8(B_IEC_CATEGORY_CODE, IEC_NON_ORIGINAL_BROAD_CAST);
    vWriteShmUINT8(B_IEC_WORD_LENGTH, _aSpdifInfo.u1WordLength[type][eStreamFrom]);

    DSP_SendDspTaskCmd(UOP_DSP_IEC_SET_DATA);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetSpdifCopyright
 *  Routine handling SPDIF Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  copyright value
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetSpdifCopyright(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 copyright)
{
    _aSpdifInfo.u1Copyright[type][(UINT8)eStreamFrom] = copyright;

    LOG (3, "SPDIF set copyright (%d) = %d for src = %d\n", type, copyright, eStreamFrom);

    // setup register type, setup will be feed into DSP....
    _AUD_DspSetSpdifChannelStatus (type);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetSpdifCategoryCode
 *  Routine handling SPDIF Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  copyright value
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetSpdifCategoryCode(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 categoryCode)
{
    _aSpdifInfo.u1Category[type][(UINT8)eStreamFrom] = categoryCode;

    LOG (3, "SPDIF set category code (%d) = %d for src = %d\n", type, categoryCode, eStreamFrom);

    // setup register type, setup will be feed into DSP....
    _AUD_DspSetSpdifChannelStatus (type);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetSpdifWordLength
 *  Routine handling SPDIF Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  copyright value
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetSpdifWordLength(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 wordLength)
{
    _aSpdifInfo.u1WordLength[type][(UINT8)eStreamFrom] = wordLength;

    LOG (3, "SPDIF set word length (%d) = %d for src = %d\n", type, wordLength, eStreamFrom);

    // setup register type, setup will be feed into DSP....
    _AUD_DspSetSpdifChannelStatus (type);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetSpdifUpdateMode
 *  Routine handling SPDIF Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  copyright value
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetSpdifUpdateMode(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 eUpdate)
{
    _aSpdifInfo.eUpdateMode[type][(UINT8)eStreamFrom] = eUpdate;

    LOG (3, "SPDIF set update mode (%d) = %d for src = %d\n", type, eUpdate, eStreamFrom);
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetSpdifCopyright
 *  Routine handling IEC Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  copyright value
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetSpdifCopyright(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    return _aSpdifInfo.u1Copyright[type][(UINT8)eStreamFrom];
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetSpdifCopyright
 *  Routine handling IEC Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  category code value
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetSpdifCategoryCode(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    return _aSpdifInfo.u1Category[type][(UINT8)eStreamFrom];
}


//-----------------------------------------------------------------------------
/** _AUD_DspGetSpdifWordLength
 *  Routine handling IEC Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  word length value
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetSpdifWordLength(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    return _aSpdifInfo.u1WordLength[type][(UINT8)eStreamFrom];
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetSpdifupdateMode
 *  Routine handling IEC Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  word length value
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetSpdifupdateMode(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    return _aSpdifInfo.eUpdateMode[type][(UINT8)eStreamFrom];
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetSpdifOutputMode
 *  Routine handling IEC Configuration.
 *
 *  @param  u1DecId     decoder ID
 *
 *  @retval  value
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetSpdifOutputMode(UINT8 u1DecId)
{
    UNUSED (u1DecId);

    return uReadShmUINT8(B_IECFLAG);
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetSpdifChannelStatus
 *  Routine handling get channel status.
 *
 *  @param
 *
 *  @retval
 */
//-----------------------------------------------------------------------------
BOOL _AUD_DspGetSpdifChannelStatus(SPDIF_CHANNEL_STATUS_T *rInfo)
{
    UINT8 u1WordLen, u1SampleRate;

    // get word length
    switch (uReadShmUINT8 (B_IEC_WORD_LENGTH))
    {
    case IEC_WORDLENGTH_24BITS:
    	u1WordLen = SPDIF_WORDLENGTH_MAX24_24BITS;
    	break;
    case IEC_WORDLENGTH_20BITS:
    	u1WordLen = SPDIF_WORDLENGTH_MAX24_20BITS;
    	break;
    case IEC_WORDLENGTH_16BITS:
    	u1WordLen = SPDIF_WORDLENGTH_MAX20_16BITS;
    	break;
    default:
    	u1WordLen = SPDIF_WORDLENGTH_MAX20_NOT_INDICATE;
    	break;
    }

    // get sample rate
    switch (AUD_GetSampleFreq(AUD_DEC_MAIN)) // always main in current solution
    {
    case FS_22K:
    	u1SampleRate = SPDIF_SAMPLE_RATE_22KHZ;
    	break;
    case FS_44K:
    	u1SampleRate = SPDIF_SAMPLE_RATE_44KHZ;
    	break;
    case FS_88K:
    	u1SampleRate = SPDIF_SAMPLE_RATE_88KHZ;
    	break;
    case FS_176K:
    	u1SampleRate = SPDIF_SAMPLE_RATE_176KHZ;
    	break;
    case FS_24K:
    	u1SampleRate = SPDIF_SAMPLE_RATE_24KHZ;
    	break;
    case FS_48K:
    	u1SampleRate = SPDIF_SAMPLE_RATE_48KHZ;
    	break;
    case FS_96K:
    	u1SampleRate = SPDIF_SAMPLE_RATE_96KHZ;
    	break;
    case FS_192K:
    	u1SampleRate = SPDIF_SAMPLE_RATE_192KHZ;
    	break;
    case FS_32K:
    	u1SampleRate = SPDIF_SAMPLE_RATE_32KHZ;
    	break;
    default:
    	u1SampleRate = SPDIF_SAMPLE_RATE_NOT_INDICATE;
    	break;
    }

    rInfo->u1Mode = 0; // consumer mode only
    rInfo->u1SampleMode = uReadShmUINT8 (B_IECFLAG);
    rInfo->u1Copyright = uReadShmUINT8 (B_IEC_COPYRIGHT);
    rInfo->u1PreEmphasis = 0;
    rInfo->u1Category = uReadShmUINT8 (B_IEC_CATEGORY_CODE);
    rInfo->u1SourceNum = 0;
    rInfo->u1CannelNum = 0;
    rInfo->u1SampleRate = u1SampleRate;
    rInfo->u1ClockAccuray = 0;
    rInfo->u1WordLength = u1WordLen;
    rInfo->u1OrgSampleRate = 0;

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _AUD_DspSpdifSetInfo
 *  Routine handling IEC Configuration.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Enable 0: disable 1: enable
 *  @retval  TRUE
 */
//-----------------------------------------------------------------------------
BOOL _AUD_DspSpdifSetInfo(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 u1CopyRight, UINT8 u1CategoryCode)
{
    _aSpdifInfo.u1Copyright[type][(UINT8)eStreamFrom] = u1CopyRight;
    _aSpdifInfo.u1Category[type][(UINT8)eStreamFrom] = u1CategoryCode;

   // setup register type, setup will be feed into DSP....
    _AUD_DspSetSpdifChannelStatus (type);

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _AUD_DspSpdifSetCopyProtect
 *  Routine handling IEC Configuration.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Enable 0: disable 1: enable
 *  @retval  TRUE
 */
//-----------------------------------------------------------------------------
#define IEC_CATEGORY_LBIT_NUMBER 7
#define IEC_CATEGORY_LBIT (1 << IEC_CATEGORY_LBIT_NUMBER)
#define IEC_CATEGORY_LBIT_MASK (~IEC_CATEGORY_LBIT)

BOOL _AUD_DspSpdifSetCopyProtect(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom, BOOL bCpBit, BOOL bLBit)
{
    UINT8 u1CategoryCode, u1CopyRight;

    u1CopyRight = (UINT8) bCpBit;
    u1CategoryCode = AUD_DspGetSpdifCategoryCode (type, eStreamFrom) & IEC_CATEGORY_LBIT_MASK;
    if (bLBit)
    {
        u1CategoryCode |= IEC_CATEGORY_LBIT;
    }

    _aSpdifInfo.u1Copyright[type][(UINT8)eStreamFrom] = u1CopyRight;
    _aSpdifInfo.u1Category[type][(UINT8)eStreamFrom] = u1CategoryCode;

   // setup register type, setup will be feed into DSP....
    _AUD_DspSetSpdifChannelStatus (type);

    return TRUE;
}

#ifdef CC_AUD_MIXSOUND_SUPPORT
//-----------------------------------------------------------------------------
/** AUD_DspMixSndEnable
 *  Mix sound enable.
 *
 *  @param  fgEnable TRUE(enable) FALSE(disable).
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspMixSndEnable(BOOL fgEnable)
{
	_fgMixSndFlag = fgEnable;
}

//-----------------------------------------------------------------------------
/** AUD_DspMixSndMute
 *  Mix sound mute.
 *
 *  @param  fgMutre TRUE(mute) FALSE(un-mute).
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspMixSndMute(BOOL fgMute)
{
	_fgMixSndMute = fgMute;
}

#ifndef NEW_MIXSOUND
#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
extern BOOL fgAudFeedMixSndThreadEnable; //FLASHLITE_CONFLICT
#endif
#endif

//-----------------------------------------------------------------------------
/** _AUD_DspMixSndControl
 *  Mix sound control.
 *
 *  @param  u1Mode play/stop.
 *  @retval  TRUE
 */
//-----------------------------------------------------------------------------
void _AUD_DspMixSndControl(UINT16 u2Mode)
{
#ifndef NEW_MIXSOUND
#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY //FLASHLITE_CONFLICT
    if (fgAudFeedMixSndThreadEnable)
        return;
#endif
#endif

    // Trigger play only when clip has ever been selected
    if ((_u1MixSndClipIdx != (UINT8)0xff) && (_fgMixSndFlag) && (!_fgMixSndMute))
    {
        vWriteShmUINT16(W_MIXSOUND_FLAG, u2Mode);
        DSP_SendDspTaskCmd(UOP_DSP_MIXSOUND_FLAG);
    }
    else
    {
        LOG(3, "ERROR: no mixsnd clip selected\n");
    }
}

#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY //FLASHLITE_CONFLICT
void _AUD_DspMixSndControl2(UINT16 u2Mode)
{
    vWriteShmUINT16(W_MIXSOUND_FLAG, u2Mode);
    DSP_SendDspTaskCmd(UOP_DSP_MIXSOUND_FLAG);
}
#endif

//-----------------------------------------------------------------------------
/** _AUD_DspMixSndData
 *  Select mix sound data source.
 *
 *  @param  u1ClipIdx 0~3
 *  @retval  TRUE
 */
//-----------------------------------------------------------------------------
void _AUD_DspMixSndClip(UINT8 u1ClipIdx)
{
    UINT32 u4Idx;
    UINT32 *pu4MixSndTab, u4TabSize;

    if (_u1MixSndClipIdx == u1ClipIdx)
    {
        return;
    }

    if (u1ClipIdx > AUD_MIXSND_CLIP_NUM)
    {
        u1ClipIdx = AUD_MIXSND_CLIP0;
    }

    if (u1ClipIdx == AUD_MIXSND_CLIP0)
    {
        u4TabSize = sizeof(_au4MixSndData0);
        pu4MixSndTab = (UINT32 *)&_au4MixSndData0[0];
        vWriteShmUINT32 (D_MIXSOUND_PLEN, ((u4TabSize/2)*_u4MixsoundLoop));
        vWriteShmUINT16 (W_MIXSOUND_CFG, 0x1f00); //mono
    }
    else if (u1ClipIdx == AUD_MIXSND_CLIP1)
    {
        u4TabSize = sizeof(_au4MixSndData0_Stereo);
        pu4MixSndTab = (UINT32 *)&_au4MixSndData0_Stereo[0];
        vWriteShmUINT32 (D_MIXSOUND_PLEN, ((u4TabSize/2/2)*_u4MixsoundLoop));
        vWriteShmUINT16 (W_MIXSOUND_CFG, 0x1f10); //stereo
    }
    else if (u1ClipIdx == AUD_MIXSND_CLIP2)
    {
        u4TabSize = sizeof(_au4MixSndData0_Stereo_LOnROff);
        pu4MixSndTab = (UINT32 *)&_au4MixSndData0_Stereo_LOnROff[0];
        vWriteShmUINT32 (D_MIXSOUND_PLEN, ((u4TabSize/2/2)*_u4MixsoundLoop));
        vWriteShmUINT16 (W_MIXSOUND_CFG, 0x1f10); //stereo
    }
    else
    {
        u4TabSize = sizeof(_au4MixSndData0);
        pu4MixSndTab = (UINT32 *)&_au4MixSndData0[0];
        vWriteShmUINT32 (D_MIXSOUND_PLEN, ((u4TabSize/2)*_u4MixsoundLoop));
        vWriteShmUINT16 (W_MIXSOUND_CFG, 0x1f00); //mono
    }

#ifndef NEW_MIXSOUND
#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
    if (!fgAudFeedMixSndThreadEnable) //FLASHLITE_CONFLICT
    {
#endif
#endif
        for (u4Idx=0; u4Idx<_u4MixsoundLoop; u4Idx++)
        {
            x_memcpy((VOID *)VIRTUAL((UINT32)_u4MixSndBuf+(u4TabSize*u4Idx)),
                (const VOID *)pu4MixSndTab, u4TabSize);
        }

        DSP_FlushInvalidateDCacheFree( _u4MixSndBuf, u4GetMixSoundBufSize());
#ifndef NEW_MIXSOUND
#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
    } //FLASHLITE_CONFLICT
#endif
#endif

    _u1MixSndClipIdx = u1ClipIdx;
}


//-----------------------------------------------------------------------------
/** _AUD_DspGetMixSndEnable
 *  Get mix sound enable.
 *
 *  @param  void.
 *  @retval  TRUE(enable) FALSE(disable).
 */
//-----------------------------------------------------------------------------
BOOL _AUD_DspGetMixSndEnable(void)
{
	return _fgMixSndFlag;
}


//-----------------------------------------------------------------------------
/** _AUD_DspGetMixSndClipIdx
 *  Get mix sound clip index.
 *
 *  @param   void.
 *  @retval  clip index.
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetMixSndClipIdx(void)
{
    return _u1MixSndClipIdx;
}
#endif


//-----------------------------------------------------------------------------
/** _AUD_GetVideoScrambleStatus
 *  Get VideoScrambleStatus
 *
 *  @param  void.
 *  @retval  TRUE(enable) FALSE(disable).
 */
//-----------------------------------------------------------------------------
BOOL _AUD_GetVideoScrambleStatus(void)
{
	return _fgVideoScrambled;
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

#ifdef DSP_SUPPORT_SRSTSXT
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
#endif

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

    UNUSED(u1DecId);

    if (u1BandIdx >= CH_EQ_BAND_NO)
    {
        return;
    }

    if (eEqType >= AUD_EQ_NUM)
    {
        return;
    }

    i1MapValue = i1Value; //* EQ_BAND_GAIN_SIDE_INDEX_MAX) / 100;

    VERIFY(x_sema_lock(_ahEqCtlSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    _aai1ChEqCfg[eEqType][u1BandIdx + 1] = i1MapValue;
    VERIFY(x_sema_unlock(_ahEqCtlSema) == OSR_OK);
    if (_eEqMode == eEqType)
    {
        // FIXME !!
        AUD_DspChEqLoadPreset(AUD_DEC_MAIN, eEqType);
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
        x_memcpy((VOID *)VIRTUAL((UINT32)prEqBandInfo), (const VOID *)&_arEqInfo[u1BandIdx], sizeof(AUD_EQ_BAND_INFO_T));
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
    //20101018,By Daniel,for flash-lite clip using dec3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return 0;
    //End
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_3(u1DecId);
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
    //20101018,By Daniel,for flash-lite clip using dec3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return;
    //End
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_3(u1DecId);
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
#if defined(CC_AUD_4_DECODER_SUPPORT)
    AUD_DEC_ID_VALIDATE(u1DecId);
    #if defined(CC_MT5365) || defined(CC_MT5395) //5368/96 support 4th dec post-proc
    if ((u1DecId == AUD_DEC_4TH)
        #ifndef CC_MT5391_AUD_3_DECODER
        //20101018,By Daniel,for flash-lite clip using dec3 notify path
        ||(u1DecId == AUD_DEC_THIRD)
        //End
        #endif
        )
        return;
    #endif
#elif defined(CC_MT5391_AUD_3_DECODER)
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#else
    #if defined(CC_MT5365) || defined(CC_MT5395) //5368/96 support 4th dec post-proc
    //20101018,By Daniel,for flash-lite clip using dec3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return;
    //End
    #endif
    AUD_DEC_ID_VALIDATE_2(u1DecId);
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

//-----------------------------------------------------------------------------
/** _AUD_DspGetAtvTvSystemStrength
 *
 *  @param  TV_AUD_SYS_T
 *  @retval  UINT32
 */
//-----------------------------------------------------------------------------
UINT32 _AUD_DspGetAtvTvSystemStrength(TV_AUD_SYS_T eAudSys)
{
    return (UINT32)DSP_GetAtvTvSystemStrength(eAudSys);
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
    VERIFY(x_sema_lock(_ahSpdifCtlSema, X_SEMA_OPTION_WAIT) == OSR_OK);
	_aafgHdmiSetSpdifFlag=fgEnable;
	_AudDspSetIec(_aeMWIec,fgEnable);
	VERIFY(x_sema_unlock(_ahSpdifCtlSema) == OSR_OK);
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

#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
	u4Spectrum = u4Spectrum>>8;
#endif

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
    //20101018,By Daniel,for flash-lite clip using dec3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return;
    //End
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
    Function : _AUD_VideoScrambled
    Description : Set Video Scramble Status
    Parameter : 1 : video srambled
    Return    : None
    Xiaowen Chen 2010/2/15
***************************************************************************/
void  _AUD_VideoScrambled(BOOL fgEnable)
{
    _fgVideoScrambled = fgEnable;

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
    /*
    UINT16 u2ShmIndex;
    static UINT8 u1VsEnable = 0;
    static UINT8 u1AgcEnable = 0;
    static UINT32 u4DownmixMode = 0;
    */
    AMP_DRV_CB_T *pfnDrvCbFuncTbl;

    AUD_DEC_ID_VALIDATE_2(u1DecId);
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

        pfnDrvCbFuncTbl = ADAC_GetDrvCallBackFuncTbl();
        if (pfnDrvCbFuncTbl->pfnDisableEffect != NULL)
        {
            pfnDrvCbFuncTbl->pfnDisableEffect ();
        }
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

/***************************************************************************
    Function : _AUD_GetDDBannerEnable
    Description : Get Dolby banner information.
                  For DD test only.
    Parameter :
    Return    : None
***************************************************************************/
BOOL _AUD_GetDDBannerEnable (void)
{
    return (fgBannerEnable);
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
	case AUD_FACTORY_A2_WEAK_RF_LVL_MONO2STEREO:
            u2ShmIndex = W_A2_WEAK_RF_LVL_M2S_THRES_EU;
            break;
	case AUD_FACTORY_A2_WEAK_RF_LVL_STEREO2MONO:
            u2ShmIndex = W_A2_WEAK_RF_LVL_S2M_THRES_EU;
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
	case AUD_FACTORY_A2_WEAK_RF_LVL_MONO2STEREO:
            u2ShmIndex = W_A2_WEAK_RF_LVL_M2S_THRES_EU;
            break;
	case AUD_FACTORY_A2_WEAK_RF_LVL_STEREO2MONO:
            u2ShmIndex = W_A2_WEAK_RF_LVL_S2M_THRES_EU;
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

//-----------------------------------------------------------------------------
/** _AUD_DspSetFmRadioFineVolume
 *   Set fine tune volume for MTS mono.
 *
 *  @param  u1Index      0~40 .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetFmRadioFineVolume (UINT8 u1Index)
{
    DSP_SetFmRadioFineVolume(u1Index) ;
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

#ifdef CC_AUD_BBE_SUPPORT
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
#endif // #ifdef CC_AUD_BBE_SUPPORT

#ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
void AUD_DspSetCDNotchQandFc(UINT32 u4UserQIdx, UINT32 u4UserFc)
{
    static UINT32 _u4CDNotchQIdx = 0;
    static UINT32 _u4CDNotchFc = 0;
	if (u4UserQIdx != _u4CDNotchQIdx || u4UserFc != _u4CDNotchFc)
	{
		_u4CDNotchQIdx = u4UserQIdx;
		_u4CDNotchFc = u4UserFc;
		DSP_SetCDNotchQandFc(u4UserQIdx, u4UserFc);
	}
}
#endif

#ifdef  KARAOKE_SUPPORT
void AUD_DspSetKeyshiftFlag(BOOL fgflag)
{
    static BOOL _fgflag = FALSE;
    static BOOL _fgInit = FALSE;

    if (fgflag != _fgflag || _fgInit == FALSE)
    {
        _fgInit = TRUE;
        _fgflag = fgflag;
        DSP_SetKeyshiftFlag(fgflag);
    }
}
void AUD_DspSetKeyshiftKey(INT8 i1key)
{
    static INT8 _i1key = 0;

    if (i1key != _i1key)
    {
        _i1key = i1key;
        DSP_SetKeyshiftKey(i1key);
    }
}

#endif

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
    AUD_DEC_ID_VALIDATE(u1DecId);

    if(u1DecId == AUD_DEC_MAIN)
    {
        vWriteShmUINT16(W_PCM_INPUT_CHANNEL_NUM, u2ChannelNo);
    }
    else if(u1DecId == AUD_DEC_AUX)
    {
        vWriteShmUINT16(W_PCM_INPUT_CHANNEL_NUM_DEC2, u2ChannelNo);
    }
    else if(u1DecId == AUD_DEC_THIRD)
    {
        vWriteShmUINT16(W_PCM_INPUT_CHANNEL_NUM_DEC3, u2ChannelNo);
    }
    else if(u1DecId == AUD_DEC_4TH)
    {
        vWriteShmUINT16(W_PCM_INPUT_CHANNEL_NUM_DEC4, u2ChannelNo);
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
/** _AUD_3D_AVLINK_Config
 *   transform 3D-video depth to the "width setting" of mtk vsurr
 *  u4DepthField : 0~32
 *  @retval  void
 */
//-----------------------------------------------------------------------------
 //audio depths links with video depths, by gallen/sammy, 20110526
#ifdef CC_3DTV_AUDIO_LINK_SUPPORT
void _AUD_3D_AVLINK_Config(UINT32 u4DepthField)
{
    UINT32 u4VSParameter;

    if (u4DepthField > 32) u4DepthField = 32;
    // Note Width       : Sound Width,          0 ~ 0x500000\n");
    // The mapping of "u4DepthField" to "Sound Width" shall be returned on customer's set.s
    u4VSParameter = (32-u4DepthField)*65536+0x200000;
    //[0, 32] -> [0x400000, 0x200000];

    DSP_Set_VSurr_Cfg(AUD_SUR_WIDTH,u4VSParameter);
    LOG(4, "Set MVS according to 3D video depth, depth = %d!\n", u4DepthField);
}
#endif

//-----------------------------------------------------------------------------
/** _AUD_DspSetPcmPreBufferLength
 *   Set PCM pre-buffering length
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetPcmPreBufferLength(UINT8 u1DecId, UINT16 u2Length)
{
    UNUSED(u1DecId);

    vWriteShmUINT16(W_PCM_PREBUF_DELAY_BANK, u2Length);
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
    UNUSED(u1DecId);

    return u2ReadShmUINT16(W_PCM_PREBUF_DELAY_BANK);
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


/** _AUD_GetAC3_EAC3_Info
 *
 *  @retval  u1version
 */
//-----------------------------------------------------------------------------

UINT8 _AUD_GetAC3_EAC3_Info(UINT8 u1DecId)
{
    UINT32 u1DDP=0;
	UINT8 u1Version=0xff;
	if(u1DecId== AUD_DEC_MAIN)
    {
        u1DDP=(UINT32)(u4ReadDspSram(0x3500)>>8);
    }
	else if(u1DecId==AUD_DEC_AUX)
	{
        u1DDP=(UINT32)(u4ReadDspSram(0x1c80)>>8);
	}
	if(u1DDP==0x24)
	{
	    u1Version=0x0;
	}
	if(u1DDP==0x25)
	{
	    u1Version=0x1;
	}
	return u1Version;
}

void _AUD_DspGetSpeed(UINT8 u1DecId, UINT16 *u1Speed)
{
    UINT16 u2ShmIndex = W_SPEED;

    UNUSED(_AUD_DspGetSpeed);

    if (u1DecId != AUD_DEC_MAIN)
    {
        u2ShmIndex = W_SPEED_DEC2;
    }

    *u1Speed = u2ReadShmUINT16(u2ShmIndex);
}


//-----------------------------------------------------------------------------
/** _AUD_DspDownmixPosition
 *  Set Downmix Channel (CH9/10) Position
 *
 *  @param  u1DMPosition   : 0: do ALL post processing
 *                         : 1: do downmix after input source gain and before AVC
 *                         : 2: Skip post-proc & do manual output matrix
 *                         : 3: Skip post-proc & skip manual output matrix
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspDownmixPosition(UINT8 u1DMPosition)
{
    UINT32 u2UopIndex = UOP_DSP_DOWNMIX_POSITION;
    UINT16 u4Config = 0;
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

UINT8 _AUD_DspGetDownmixPosition(void)
{
    return ((u2ReadShmUINT16(W_SYSTEM_SETUP)>>4)&0x3);
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

UINT16 _AUD_DspGetSystemSetup(UINT8 u1DecId)
{
    UINT32 u2ShmIndx = W_SYSTEM_SETUP;

    if (u1DecId == AUD_DEC_AUX)
    {
        u2ShmIndx = W_SYSTEM_SETUP_DEC2;
    }
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u2ShmIndx = W_SYSTEM_SETUP_DEC3;
    }
    else if (u1DecId == AUD_DEC_4TH)
    {
        u2ShmIndx = W_SYSTEM_SETUP_DEC4;
    }

    return u2ReadShmUINT16(u2ShmIndx);
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
    	vWriteShmUINT16(W_WMA_PACKET_NO, prWmaInfo->u2Pktno);
    	vWriteShmUINT16(W_WMA_HEADER_SOURCE, 0);
    }else{
        vWriteShmUINT16(W_WMA_PACKET_NO, 0);
        vWriteShmUINT16(W_WMA_HEADER_SOURCE, 1);
        vWriteShmUINT32(D_WMA_PACKET_COUNT, 0x7fff);
        vWriteShmUINT32(D_WMA_PACKET_SIZE, prWmaInfo->u4Pktsz);
        vWriteShmUINT16(W_WMA_VERSION, 0x161);
        vWriteShmUINT16(W_WMA_NUM_CH, prWmaInfo->u2Numch);
        vWriteShmUINT32(D_WMA_SAMPLE_PER_SEC, prWmaInfo->u4Sampersec);
        vWriteShmUINT32(D_WMA_BYTE_PER_SEC, prWmaInfo->u4Bpersec);
        vWriteShmUINT32(D_WMA_BLOCK_SIZE, prWmaInfo->u4Blocksz);
        vWriteShmUINT16(W_WMA_ENCODER_OPTION, prWmaInfo->u2Encoderopt);

        LOG(5, "[WMA Setting]\n");
        LOG(5, "  u4Pktsz: %x\n", prWmaInfo->u4Pktsz);
        LOG(5, "  u2NumCh: %x\n", prWmaInfo->u2Numch);
        LOG(5, "  u4Sampersec: %x\n", prWmaInfo->u4Sampersec);
        LOG(5, "  u4Bpersec: %x\n",  prWmaInfo->u4Bpersec);
        LOG(5, "  u4Blocksz: %x\n", prWmaInfo->u4Blocksz);
        LOG(5, "  u2Encoderopt: %x\n", prWmaInfo->u2Encoderopt);
    }
}
#ifdef CC_53XX_SWDMX_V2
void AUD_DrvSetWMAErrFunc(UINT8 u1DecId, AUD_WMAErr_NOTIFY pfWMAErrNotify, VOID *prInst){  // megaa 20090828
	UNUSED(u1DecId);
    UNUSED(pfWMAErrNotify);
    UNUSED(prInst);
    _hWMAErrNotifyFunc =  pfWMAErrNotify;
    _prWMAErrInst = prInst;  // megaa 20090828
}
#else
void AUD_DrvSetWMAErrFunc(UINT8 u1DecId, AUD_WMAErr_NOTIFY pfWMAErrNotify){
	UNUSED(u1DecId);
    UNUSED(pfWMAErrNotify);
    _hWMAErrNotifyFunc =  pfWMAErrNotify;
}
#endif
#endif //DATA_DISC_WMA_SUPPORT

void _AUD_DspWMAProHeader(UINT8 u1DecId,AUD_WMA_CFG_T* prWmaInfo)
{
    if (u1DecId != AUD_DEC_MAIN)
    {
        return; //currently we only support WMA in dec1
    }
    if (prWmaInfo->u2HdrSrc == 0){
    	vWriteShmUINT16(W_WMAPRO_PACKET_NO, prWmaInfo->u2Pktno);
    	vWriteShmUINT16(W_WMAPRO_HEADER_SOURCE, 0);
    }else{
        vWriteShmUINT16(W_WMAPRO_PACKET_NO, 0);
        vWriteShmUINT16(W_WMAPRO_HEADER_SOURCE, 1);
        vWriteShmUINT16(W_WMAPRO_NUM_CH, prWmaInfo->u2Numch);
        vWriteShmUINT32(D_WMAPRO_SAMPLE_PER_SEC, prWmaInfo->u4Sampersec);
        vWriteShmUINT32(D_WMAPRO_BYTE_PER_SEC, prWmaInfo->u4Bpersec);
        vWriteShmUINT32(D_WMAPRO_BLOCK_SIZE, prWmaInfo->u4Blocksz);
        vWriteShmUINT16(W_WMAPRO_ENCODER_OPTION, prWmaInfo->u2Encoderopt);

        //new for WMA Pro
        vWriteShmUINT32(D_WMAPRO_CH_MASK, prWmaInfo->u4ChannelMask);
        vWriteShmUINT16(W_WMAPRO_VALID_BITS, prWmaInfo->u2BitsPerSample);
        vWriteShmUINT16(W_WMAPRO_ADV_OPTION, prWmaInfo->u2AdvEncoderopt);
        vWriteShmUINT32(D_WMAPRO_ADV_OPTION2, prWmaInfo->u4AdvEncoderopt2);

        LOG(5, "[WMA Pro Setting]\n");
        LOG(5, "  u2NumCh: %x\n", prWmaInfo->u2Numch);
        LOG(5, "  u4Sampersec: %x\n", prWmaInfo->u4Sampersec);
        LOG(5, "  u4Bpersec: %x\n",  prWmaInfo->u4Bpersec);
        LOG(5, "  u4Blocksz: %x\n", prWmaInfo->u4Blocksz);
        LOG(5, "  u2Encoderopt: %x\n", prWmaInfo->u2Encoderopt);
        LOG(5, "  u4ChannelMask: %x\n", prWmaInfo->u4ChannelMask);
        LOG(5, "  u2BitsPerSample: %x\n", prWmaInfo->u2BitsPerSample);
        LOG(5, "  u2AdvEncoderopt: %x\n", prWmaInfo->u2AdvEncoderopt);
        LOG(5, "  u4AdvEncoderopt2: %x\n", prWmaInfo->u4AdvEncoderopt2);
    }
}

void AUD_SetLpcmTable(UINT8 u1DecId, AUD_LPCM_SETTING_T rLpcmSetting)
{
    UNUSED(u1DecId) ;

    //decoder 2, decoder 3 ... todo...
    if (u1DecId == AUD_DEC_MAIN)
    {
        vWriteShmUINT8(B_LPCM_CH_ASSIGNMENT, rLpcmSetting.u1LpcmChAssignment);
        vWriteShmUINT16(W_LPCM_FREQUENCY, (((UINT16)rLpcmSetting.u1LpcmFreqGrp1) << 8) + rLpcmSetting.u1LpcmFreqGrp2);
        vWriteShmUINT8(B_LPCM_BIT_SHIFT, rLpcmSetting.u1LpcmBitShift);
        vWriteShmUINT8(B_LPCM_DRC_VALUE, rLpcmSetting.u1LpcmDrcValue);
        vWriteShmUINT16(W_LPCM_Q_VALUE, (rLpcmSetting.u1LpcmBitResGrp1 << 12)+rLpcmSetting.u1LpcmBitResGrp2);
        vWriteShmUINT8(B_LPCM_DRC_FLAG, rLpcmSetting.u1LpcmDrcFlag);
        vWriteShmUINT16(W_LPCM_DECODING_TYPE, rLpcmSetting.u2LpcmDecodingType);

        // ADPCM
        if (rLpcmSetting.u2ADPCMDecodingType!=0)
        {
            vWriteShmUINT8(B_ADPCM_CHANNEL_NUM, rLpcmSetting.u1LpcmChAssignment+1);
            vWriteShmUINT8(B_ADPCM_BIT_PERSAMPLE, rLpcmSetting.u1LpcmBitResGrp1);
            vWriteShmUINT16(W_ADPCM_DECODING_TYPE, rLpcmSetting.u2ADPCMDecodingType);
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
    else
    {
    	vWriteShmUINT8(B_LPCM_CH_ASSIGNMENT_DEC2, rLpcmSetting.u1LpcmChAssignment);
    	vWriteShmUINT16(W_LPCM_FREQUENCY_DEC2, (rLpcmSetting.u1LpcmFreqGrp1 << 8) + rLpcmSetting.u1LpcmFreqGrp2);
    	vWriteShmUINT8(B_LPCM_BIT_SHIFT_DEC2, rLpcmSetting.u1LpcmBitShift);
    	vWriteShmUINT8(B_LPCM_DRC_VALUE_DEC2, rLpcmSetting.u1LpcmDrcValue);
    	vWriteShmUINT16(W_LPCM_Q_VALUE_DEC2, (rLpcmSetting.u1LpcmBitResGrp1 << 12)+rLpcmSetting.u1LpcmBitResGrp2);
    	vWriteShmUINT8(B_LPCM_DRC_FLAG_DEC2, rLpcmSetting.u1LpcmDrcFlag);
    	vWriteShmUINT16(W_LPCM_DECODING_TYPE_DEC2, rLpcmSetting.u2LpcmDecodingType);

    	// ADPCM
    	if (rLpcmSetting.u2ADPCMDecodingType!=0)
    	{
        	vWriteShmUINT8(B_ADPCM_CHANNEL_NUM_DEC2, rLpcmSetting.u1LpcmChAssignment+1);
        	vWriteShmUINT8(B_ADPCM_BIT_PERSAMPLE_DEC2, rLpcmSetting.u1LpcmBitResGrp1);
        	vWriteShmUINT16(W_ADPCM_DECODING_TYPE_DEC2, rLpcmSetting.u2ADPCMDecodingType);
        	vWriteShmUINT16(W_ADPCM_BLOCK_ALIGN_DEC2, rLpcmSetting.u2ADPCMBlockAlign);
    	}
    	else
   	 	{
            vWriteShmUINT8(B_ADPCM_CHANNEL_NUM_DEC2, 0);
            vWriteShmUINT8(B_ADPCM_BIT_PERSAMPLE_DEC2, 0);
        	vWriteShmUINT16(W_ADPCM_DECODING_TYPE_DEC2, 0);
            vWriteShmUINT16(W_ADPCM_BLOCK_ALIGN_DEC2, 0);
   		}
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

#ifdef CC_AUD_HPF_SUPPORT
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
    UINT16  u2UopIndex = UOP_DSP_HPF_FC;

    vWriteShmUINT16(u2ShmIndex, u2Fc);
    DSP_SendDspTaskCmd(u2UopIndex);
}
void _AUD_DspSetHPF2Fc(UINT16 u2Fc)
{
    UINT16  u2ShmIndex = W_HPF2_FC;
    UINT16  u2UopIndex = UOP_DSP_HPF_FC;

    vWriteShmUINT16(u2ShmIndex, u2Fc);
    DSP_SendDspTaskCmd(u2UopIndex);
}

#endif

#ifdef CC_AUD_CLIPPER_SUPPORT
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
#endif

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
#if 0 //sharp Q setting
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
#else
void _AUD_DspSetPEQCfg2(UINT8 u1DecId, UINT8 u1PEQSet, UINT32 u4Frequency,
                            UINT32 eQValue, INT16 i2Gain)
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

    // Check eQValue range
    if (eQValue == 0)
    {
        eQValue = 1;
    }
	if (eQValue > 100)
    {
        eQValue = 100;
    }

    // Check FC range
    if (u4Frequency > 24000)
    {
        u4Frequency = 24000;
    }

    u2PEQBW = (UINT16)(u4Frequency*10/eQValue);
    // Check BW range
    if ((u4Frequency*10/eQValue) > 23000)
    {
        u2PEQBW = 23000;
    }

    u2PEQFc = (UINT16)u4Frequency;
    u4PEQGain = _DbToShm((INT32)i2Gain) << 4;

    vWriteShmUINT16(u2ShmAddrPEQFc,   u2PEQFc);
    vWriteShmUINT32(u2ShmAddrPEQGain, u4PEQGain);
    vWriteShmUINT16(u2ShmAddrPEQBW,   u2PEQBW);

    DSP_SendDspTaskCmd(u2UopIndex);

}

#endif
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

#ifdef CC_AUD_VBASS_SUPPORT
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

    LOG(5,"CMD: set VBassEnable: Enable(%d)\n", (UINT8)fgEnable);

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

    LOG(5,"CMD: set VBassCfg: Dec(%d) Freq(%d) Gain(%d) Gain2(%d)\n",
        u1DecId, u4Frequency, u4Gain, u4Gain2);

    UNUSED(u2ShmIndex);

    vWriteShmUINT32(D_VIR_BASS_GAIN, u4Gain);
    vWriteShmUINT32(D_VIR_BASS_GAIN_2, u4Gain2);
    DSP_SendDspTaskCmd(u2UopIndex);
}

void _AUD_DspGetVirBassCfg(UINT8 u1DecId, BOOL *fgEnable, UINT32 *u4Gain, UINT32 *u4Gain2)
{
    UNUSED(u1DecId);
    UNUSED(_AUD_DspGetVirBassCfg);

    if (uReadShmUINT8(B_VIR_BASS_ENABLE) == 0)
    {
        *fgEnable = FALSE;
    }
    else
    {
        *fgEnable = TRUE;
    }
    *u4Gain = u4ReadShmUINT32(D_VIR_BASS_GAIN);
    *u4Gain2 = u4ReadShmUINT32(D_VIR_BASS_GAIN_2);
}
#endif

#ifdef CC_AUD_FIR_SUPPORT
//-----------------------------------------------------------------------------
/** _AUD_DspSetFIREnable
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetFIREnable(UINT8 u1ConeqMode)
{
    UINT16  u2ShmIndex = B_FIR_ENABLE;
    UINT16 u2UopIndex = UOP_DSP_FIR_ENABLE;
    UINT8 u1CurEnable;

    u1CurEnable = uReadShmUINT8(u2ShmIndex);
    if (u1CurEnable == u1ConeqMode)
    {
        return;
    }

    vWriteShmUINT8(u2ShmIndex, u1ConeqMode);
    DSP_SendDspTaskCmd(u2UopIndex);
}
#endif  //FIR


#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
//-----------------------------------------------------------------------------
/** _AUD_DspSetADVEnable
 *
 *  @param  disable/light/medium/heavy compression
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetADVEnable(UINT8 fgEnable)
{
    UINT16  u2ShmIndex = B_ADV_FLAG;
    UINT16 u2UopIndex = UOP_DSP_ADV_ENABLE;
    UINT8  adv_mode=0;

#if 0
    if (fgEnable == 0x1 |fgEnable == 0x2 |fgEnable == 0x3 )
    {
       //Daniel, 2009/12/7, ADV code will be automatically loaded with AUD_POSTRAM.
       DSP_LoadRamCode(AUD_ADV);
    }
#endif
    if (fgEnable == 0x0)//OFF
    {
        adv_mode=0;
    }
    else if(fgEnable == 0x1)//light compressioj
    {
        adv_mode=3;
    }
    else if(fgEnable == 0x2)//medium compressioj
    {
        adv_mode=5;
    }
   else if(fgEnable == 0x3)//heavy compressioj
    {
        adv_mode=9;
    }

    vWriteShmUINT8(u2ShmIndex, (UINT8)adv_mode);
    DSP_SendDspTaskCmd(u2UopIndex);
}


void _AudShowADVStatus(void)
{
    UINT32 chCalbGain_L,chCalbGain_R;

    chCalbGain_L=u4ReadShmUINT32(D_ADV_chCalbGain_L)	;
    chCalbGain_R=u4ReadShmUINT32(D_ADV_chCalbGain_R);
    LOG(1 ,"chCalbGain_L = 0x%08x\n", chCalbGain_L);
    LOG(1 ,"chCalbGain_R = 0x%08x\n", chCalbGain_R);

    // Fix -O2 warning message
    UNUSED(chCalbGain_L);
    UNUSED(chCalbGain_R);
}

//-----------------------------------------------------------------------------
/** _AUD_DspADVSetChGain
 *
 *  @param  ch, value
 *   ch:        0:L, 1:R
 *   value: dB (10.14), range: 0x0 ~ 0xffffff
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspADVSetChGain(UINT8 ch, UINT32 value)
{
    UINT32 u4ShmIndex = D_ADV_chCalbGain_L;
    UINT32 u2UopIndex = UOP_DSP_ADV_SET_chCalbGain;
    if (ch==0) //L
    {
       u4ShmIndex=D_ADV_chCalbGain_L;
    }
    else//R
    {
       u4ShmIndex=D_ADV_chCalbGain_R;
    }
     vWriteShmUINT32 (u4ShmIndex,value);//dB,(10.14)
    DSP_SendDspTaskCmd(u2UopIndex);
}

void _AUD_DspABXQuery(void)
{
    Printf("  ----------\n");
    Printf("  item 0: [DEC1] ABX Enable: %d (1-Enable,0-Disable)\n", uReadShmUINT8(B_ABX_ENABLE));
    Printf("  item 1: [DEC1] filter set: %d (0-Set 1,1-Set 2)\n", uReadShmUINT8(B_ABX_FILTER_SET));
    Printf("  item 2: [DEC1] Dry Gain Indx: %d (0~24, 0.5db/step)\n", uReadShmUINT8(B_ABX_DRY_GAIN_INDX));
    Printf("  item 3: [DEC1] Wet Gain Indx: %d (0~24, 0.5db/step)\n", uReadShmUINT8(B_ABX_WET_GAIN_INDX));
}

void _AUD_DspABXConfig(UINT8 item, UINT8 val)
{
    switch (item)
    {
        case 0:
            vWriteShmUINT8(B_ABX_ENABLE, val);
            DSP_SendDspTaskCmd(UOP_DSP_ABX_FLAG);
            break;

        case 1:
            vWriteShmUINT8(B_ABX_FILTER_SET, val);
            if (val == 0)
            {
                vWriteShmUINT8(B_ABX_DRY_GAIN_INDX,0);
                vWriteShmUINT8(B_ABX_WET_GAIN_INDX,6);
            }
            else
            {
                vWriteShmUINT8(B_ABX_DRY_GAIN_INDX,0);
                vWriteShmUINT8(B_ABX_WET_GAIN_INDX,12);
            }
            DSP_SendDspTaskCmd(UOP_DSP_ABX_FLAG);
            break;

        case 2:
            vWriteShmUINT8(B_ABX_DRY_GAIN_INDX, val);
            DSP_SendDspTaskCmd(UOP_DSP_ABX_FLAG);
            break;

        case 3:
            vWriteShmUINT8(B_ABX_WET_GAIN_INDX, val);
            DSP_SendDspTaskCmd(UOP_DSP_ABX_FLAG);
            break;
    }
}

void _AUD_DspAEQEnable(UINT8 u1DecId, BOOL fgEnable)
{
    UINT32 u4UopCmd;
    UINT8  u1ShmFlag;

    UNUSED(u1DecId);

    u4UopCmd = UOP_DSP_AEQ_FLAG;
    u1ShmFlag = uReadShmUINT8(B_AEQ_FLAG);

    if ((fgEnable && !u1ShmFlag) || (!fgEnable && u1ShmFlag))
    {
        u1ShmFlag = fgEnable;
        vWriteShmUINT8(B_AEQ_FLAG, u1ShmFlag);
        DSP_SendDspTaskCmd(u4UopCmd);
    }
}

void _AUD_DspAEQQuery(void)
{
    UINT8   u1ShmFlag;
    BOOL    fgStatus;

    u1ShmFlag = uReadShmUINT8(B_EQFLAG);
    fgStatus = (BOOL)(u1ShmFlag);

    if (fgStatus)
    {
        Printf("\nAudyssey EQ Status: Enable");
    }
    else
    {
        Printf("\nAudyssey EQ Status: Disable");
    }

}
#endif
//zy_dts
void  _AUD_DspSetDTSENCEnable(BOOL fgEnable)
{
    UINT16  u2ShmIndex = B_DTS_ENCODER_FLAG;
    UINT16  u2UopIndex = UOP_DSP_DTS_ENCODER_ENABLE;

	/*

	if ((fgEnable == TRUE) && (uReadShmUINT8(B_DTS_ENCODER_FLAG)==0x1))
    {
        return;
    }

    if ((fgEnable == FALSE) && (uReadShmUINT8(B_DTS_ENCODER_FLAG)==0x0))
    {
        return;
    }
    */
    if (fgEnable == TRUE)
    {
       DSP_LoadRamCode(AUD_DTSENC);
    }


    vWriteShmUINT8(u2ShmIndex, (UINT8)fgEnable);
    DSP_SendDspTaskCmd(u2UopIndex);

    //Fix AMODE = 9
	vWriteShmUINT8(B_DTS_ENCODER_AMODE,9);
    DSP_SendDspTaskCmd(UOP_DSP_DTS_ENCODER_AMODE);


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
#ifndef CC_AUD_SUPPORT_MS10
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
       DSP_LoadRamCode(AUD_DDCO);
    }

    vWriteShmUINT8(u2ShmIndex, (UINT8)fgEnable);
    DSP_SendDspTaskCmd(u2UopIndex);
#else
    //turn off MDC
    vWriteShmUINT8(B_DDT_CONTROL, (UINT8)fgEnable);

#endif
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

//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
extern void _AUD_DataUploadSetMode(UINT8 u1Mode);
extern void _AUD_DataUpload_Init (void);
extern void _AUD_DataUpload_start (void);
extern void _AUD_DataUploadQueueInit (UINT32 u4BaseAddr, UINT32 u4BlockSize, UINT32 u4BlockNum);

//-----------------------------------------------------------------------------
/** _AUD_DspSetUploadDataEnable
 *
 *  @param  void
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetUploadDataEnable (void)
{
    /// Set W_UPLOAD_DATA_FLAG at _AUD_DspSetUploadMode();
    UINT16 u2UopIndex = UOP_DSP_UPLOAD_DATA_ENABLE;
    DSP_SendDspTaskCmd(u2UopIndex);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetUploadMode
 *
 *  @param  0: off, 1: PCM, 2: MP3, 3: SBC Encoder, 4: ALSA, 5: WiFi
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetUploadMode(UINT8 u1Mode)
{
    _AUD_DataUploadSetMode(u1Mode);
    _AUD_DataUpload_Init();
    switch (u1Mode)
    {
        case 0://Off
            vWriteShmUINT16 (W_UPLOAD_DATA_FLAG, 0x8000); //reset when off is enabled
            vWriteShmUINT8 (B_UPLOAD_BLOCK_NUM, 128);
            vWriteShmUINT16 (W_UPLOAD_BLOCK_SIZE, 0x400);
            vWriteShmUINT8 (B_UPLOAD_BLK_INT_CNT, 16);
            break;
        case 1://PCM
            /// bit0 = 1
            vWriteShmUINT16 (W_UPLOAD_DATA_FLAG, 1<<15 | 1<<(u1Mode-1)); ///1<<15 for PCM record initial.
            vWriteShmUINT8 (B_UPLOAD_BLOCK_NUM, 128);
            // Block size = 256*2(Ch)*2(16bits) = 1024 bytes
            vWriteShmUINT16 (W_UPLOAD_BLOCK_SIZE, 0x400);
            vWriteShmUINT8 (B_UPLOAD_BLK_INT_CNT, 16);
            _AUD_DataUpload_start();
            break;
    #ifdef  MP3ENC_SUPPORT
        case 2://Mp3Enc
            /// bit1 = 1
            vWriteShmUINT16 (W_UPLOAD_DATA_FLAG, 1<<(u1Mode-1));
            vWriteShmUINT8 (B_UPLOAD_BLOCK_NUM, 128);
            ///Block Size, MP3=0x3C0(320kbps)/0x240(192kbps)/0x180(128kbps)
            vWriteShmUINT16 (W_UPLOAD_BLOCK_SIZE, MP3ENC_FRAME_SIZE);
            ///Evey 8 mp3 frame send one interrupt
            ///8*1152/48000 = 0.192sec
            vWriteShmUINT8 (B_UPLOAD_BLK_INT_CNT, 8);
            _AUD_DataUpload_start();
            break;
    #endif
        case 4:
            vWriteShmUINT16 (W_UPLOAD_DATA_FLAG, 0xe008); // [15] for initial, [14] for skip interrupt notify, [13] for little endian, [3] upload pcm after post-proc
            vWriteShmUINT8 (B_UPLOAD_BLOCK_NUM, AUD_UPLOAD_BUFFER_SIZE/0x400);
            vWriteShmUINT16 (W_UPLOAD_BLOCK_SIZE, 0x400);
            vWriteShmUINT8 (B_UPLOAD_BLK_INT_CNT, 16);
            break;
        case 5://WiFi
            vWriteShmUINT16 (W_UPLOAD_DATA_FLAG, 0xa008); ///1<<15 for PCM record initial.
            vWriteShmUINT8 (B_UPLOAD_BLOCK_NUM, 128);
            // Block size = 256*2(Ch)*2(16bits) = 1024 bytes
            vWriteShmUINT16 (W_UPLOAD_BLOCK_SIZE, 0x400);
            vWriteShmUINT8 (B_UPLOAD_BLK_INT_CNT, 16);
            _AUD_DataUpload_start();
            break;
       case 6:
            vWriteShmUINT16 (W_UPLOAD_DATA_FLAG, (1<<15) | (1<< 13) | 1); ///1<<15 for PCM (inter)record initial.
            vWriteShmUINT8 (B_UPLOAD_BLOCK_NUM, 128);
            // Block size = 256*2(Ch)*2(16bits) = 1024 bytes
            vWriteShmUINT16 (W_UPLOAD_BLOCK_SIZE, 0x400);
            vWriteShmUINT8 (B_UPLOAD_BLK_INT_CNT, 16);
            _AUD_DataUpload_start();
            break;
        case 3://SBC
        default:
            //TODO
            vWriteShmUINT16 (W_UPLOAD_DATA_FLAG, 0);
            vWriteShmUINT8 (B_UPLOAD_BLOCK_NUM, 128);
            vWriteShmUINT16 (W_UPLOAD_BLOCK_SIZE, 0x400);
            vWriteShmUINT8 (B_UPLOAD_BLK_INT_CNT, 16);
            break;
    }
    _AUD_DataUploadQueueInit (_u4DspCmptBuf[TYPE_COMM_DRAM_IDX] + AUD_UPLOAD_BUFFER_BASE,
                                u2ReadShmUINT16(W_UPLOAD_BLOCK_SIZE),
                                uReadShmUINT8(B_UPLOAD_BLOCK_NUM));
}
//#endif

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
    x_memcpy((VOID *)VIRTUAL((UINT32)aeNvmData), (const VOID *)u4Ptr, sizeof(AUD_NVM_DATA_T) * u4NvmLength);

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
void AUD_FlashAQSetValue(void)
{
    UINT8 i;

    for (i=0; i<=AQ_TYPE_NUM; i++)
    {
        _fgAQ_Enable[i] = FALSE;
    }
}

BOOL AUD_FlashAQQuery(AUD_AQ_TYPE_T eAQDataType, VOID* *pData)
{
    BOOL fgRet = FALSE;
    AUD_DEC_STREAM_FROM_T eStreamFrom;
    UINT8 eConvertSrc;
    UINT8 eConvertSoundMode;

    AUD_DRVGetStreamFrom(AUD_DEC_MAIN, &eStreamFrom);
    VERIFY(eStreamFrom < AUD_STREAM_FROM_NUM);

    eConvertSrc = _au1AQSrc_Convert[eStreamFrom];
    VERIFY( eConvertSrc < FLASH_AQ_SRC_NUM );

    if (_au1MWSetSoundModeIdx < sizeof(_au1AQSmod_Convert))
    {
        eConvertSoundMode = _au1AQSmod_Convert[_au1MWSetSoundModeIdx];
    }
    else
    {
        eConvertSoundMode = 0;
    }

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

void AUD_FlashAQParsing(AUD_AQ_TYPE_T eAQDataType, UINT8* pData)
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
        x_memcpy((VOID *)VIRTUAL((UINT32)&eDrcData), &(_au1AQ_DRC[eConvertSrc][u1ModeCase]), sizeof(AUD_AQ_AMP_DRC_T));
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

        x_memcpy((VOID *)VIRTUAL((UINT32)&rPEQData), &(_au4AQ_EXT_PEQ[eConvertSrc][u1ModeCase][eConvertFs]), sizeof(AUD_AQ_EXTPEQ_DATA_T));
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

/** _AUD_GetAacVersion
 *
 *  @param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @retval version
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_GetAacVersion(UINT8 u1DecId)
{
    return u1GetAacVersion(u1DecId);
}

//-----------------------------------------------------------------------------
/** _AUD_GetAacTSFormat;
 *
 *  @param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @retval TSFormat
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_GetAacTSFormat(UINT8 u1DecId)
{
    return u1GetAacTSFormat(u1DecId);
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetMpegAudInfoWithLayer
 *
 *  @param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param *prAudInfo                audio info structure pointer
 *  @param *prMPEGLayer           audio info structure pointer
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspGetMpegAudInfoWithLayer(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo, UINT8 *prMPEGLayer)
{
    UNUSED(_GetMpegAudInfoWithLayer(u1DecId, prAudInfo, prMPEGLayer));
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

//-----------------------------------------------------------------------------
/** AUD_DspCOOKParameter
 *  Set dsp COOK
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  prCookInfo Wma header information
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspCOOKParameter(UINT8 u1DecId, AUD_COOK_CFG_T* prCookInfo)
{
    UNUSED(u1DecId);

    vWriteShmUINT16 (W_COOK_SAMP_PER_FRAME, prCookInfo->u2SampPerFrame);
    vWriteShmUINT16 (W_COOK_CHANNEL_NUM, prCookInfo->u2ChanNum);
    vWriteShmUINT16 (W_COOK_INPUT_SAMPLING_RATE, prCookInfo->u2SampRate);
    vWriteShmUINT16 (W_COOK_FRM_SZ_IN_BYTES, prCookInfo->u2FrameSzInBytes);
    vWriteShmUINT16 (W_COOK_REGION_NUM, prCookInfo->u2RegnNum);
    vWriteShmUINT16 (W_COOK_COUPLING_START_REGN, prCookInfo->u2StartRegn);
    vWriteShmUINT16 (W_COOK_COUPLING_QBITS, prCookInfo->u2CplQBits);
}

#ifdef CC_AUD_SETUP_MELODY
void _AUD_DspSetMelodyVol(UINT8 u1Vol)
{
	_aau1ChannelVolume[AUD_DEC_MAIN][AUD_CH_ALL] = u1Vol;

}
#endif

#ifdef CC_AUD_MIXSOUND_SUPPORT

void _AUD_DspSetMixSndLoop(UINT32 u4Loop)
{
    UINT8 u1OldIdx = _u1MixSndClipIdx;

    _u1MixSndClipIdx = 0xff;
    _u4MixsoundLoop = u4Loop;
    _AUD_DspMixSndClip(u1OldIdx);
}

void _AUD_DspSetMixSndVol(UINT32 u4Vol)
{
	_u4MixsoundVol=u4Vol;
    vWriteShmUINT32(D_MIXSOUND_GAIN, _au4VOLUME[u4Vol]); // 100 --> 0
}

UINT32 _AUD_DspGetMixSndLoop(void)
{
    return _u4MixsoundLoop;
}

UINT32 _AUD_DspGetMixSndVol(void)
{
    return _u4MixsoundVol;
}
#endif

#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
void _AUD_DspSetMixSndLength(UINT32 u4Len)
{
    vWriteShmUINT32 (D_MIXSOUND_PLEN, u4Len); // 24*64 12kHz
}

void _AUD_DspSetMixSndGain(UINT32 u4Gain)
{
    vWriteShmUINT32 (D_MIXSOUND_GAIN,   u4Gain); // 0x20000 --> 0dB
}

UINT32 _AUD_DspGetMixSndGain(void)
{
    return u4ReadShmUINT32(D_MIXSOUND_GAIN);
}

void _AUD_DspSetMixSndStereo(UINT8 bOnOff)
{
    vWriteShmUINT16(W_MIXSOUND_CFG,(u2ReadShmUINT16(W_MIXSOUND_CFG)&0xffef) | (bOnOff ? 0x10 : 0x00));
}

BOOL _AUD_DspGetMixSndStereo(void)
{
    if (u2ReadShmUINT16(W_MIXSOUND_CFG)&0x10)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

// 0: 1x
// 1: 2x
// 2: 4x
// 3: 8x
void _AUD_DspSetMixSndUpSampleRatio(UINT8 bRatio)
{
    vWriteShmUINT16(W_MIXSOUND_CFG,(u2ReadShmUINT16(W_MIXSOUND_CFG)&0xfffc) | bRatio);
}

#ifndef NEW_MIXSOUND
void _AUD_DspSetMixSndMemPlay(UINT8 bOnOff)
{
    vWriteShmUINT16(W_MIXSOUND_CFG,(u2ReadShmUINT16(W_MIXSOUND_CFG)&0xffdf) | (bOnOff ? 0x20 : 0x00));
}
#endif

BOOL _AUD_DspIsMixSndPlay(void)
{
    return u2GetMixSoundStatus();
}

// 0x0100: Mix to L/R
// 0x0200: Mix to SL/SR
// 0x0400: Mix to Center
// 0x0800: Mix to CH7/CH8
// 0x1000: Mix to CH9/CH10
void _AUD_DspSetMixSndChannel(UINT16 wCh)
{
    vWriteShmUINT16(W_MIXSOUND_CFG,(u2ReadShmUINT16(W_MIXSOUND_CFG)&0xe0ff) | wCh);
}
#endif

//-----------------------------------------------------------------------------
/** _AUD_DspSetWithVideo
 *   Set is with video
 *
 *  @param  u1DecId         Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param  fgIsWithVideo   TRUE (with video). FALSE (no video)
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetWithVideo(UINT8 u1DecId, BOOL fgIsWithVideo)
{
    if (u1DecId > AUD_DEC_AUX)
    {
        return;
    }

    if (u1DecId == AUD_DEC_MAIN)
    {
        vWriteShmUINT8(B_IS_WITH_VIDEO, (UINT8)fgIsWithVideo);
        DSP_SendDspTaskCmd(UOP_DSP_IS_WITH_VIDEO);
    }
    else
    {
        vWriteShmUINT8(B_IS_WITH_VIDEO_DEC2, (UINT8)fgIsWithVideo);
        DSP_SendDspTaskCmd(UOP_DSP_IS_WITH_VIDEO_DEC2);
    }
}

//-----------------------------------------------------------------------------
/** AUD_DspIsWithVideo
 *   Is with video
 *
 *  @param  u1DecId         Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
BOOL _AUD_DspIsWithVideo(UINT8 u1DecId)
{
    BOOL fgIsWithVideo;

    if (u1DecId > AUD_DEC_AUX)
    {
        return TRUE;
    }

    if (u1DecId == AUD_DEC_MAIN)
    {
        fgIsWithVideo = (BOOL)(uReadShmUINT8(B_IS_WITH_VIDEO));
    }
    else
    {
        fgIsWithVideo = (BOOL)(uReadShmUINT8(B_IS_WITH_VIDEO_DEC2));
    }

    return fgIsWithVideo;
}

//-----------------------------------------------------------------------------
/** _AUD_DspAsrcSetBaseFreq
 *   Set ASRC base frequency
 *
 *  @param  u4BaseFreq    Freq Idx
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspAsrcSetBaseFreq(UINT32 u4BaseFreq)
{
    AUD_DEC_STREAM_FROM_T eStreamFrom;
    UINT8 u1Byte;
    INT32 i4Level;

#ifdef CC_MT5391_AUD_3_DECODER
    AUD_DRVGetStreamFrom(AUD_DEC_AUX, &eStreamFrom);
#else
    AUD_DRVGetStreamFrom(AUD_DEC_MAIN, &eStreamFrom);
#endif

    if (eStreamFrom != AUD_STREAM_FROM_DIGITAL_TUNER)
    {
        // Only for DTV case
        return;
    }

    VERIFY(x_sema_lock(_hAsrcControlSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    if (u4BaseFreq == FS_48K)
    {
        _i4AsrcBaseFreq = ASRC_BASE_FREQ_48K;
    }
    else if (u4BaseFreq == FS_44K)
    {
        _i4AsrcBaseFreq = ASRC_BASE_FREQ_44K;
    }
    else if (u4BaseFreq == FS_32K)
    {
        _i4AsrcBaseFreq = ASRC_BASE_FREQ_32K;
    }
    else if (u4BaseFreq == FS_24K)
    {
        _i4AsrcBaseFreq = ASRC_BASE_FREQ_24K;
    }
    else if (u4BaseFreq == FS_22K)
    {
        _i4AsrcBaseFreq = ASRC_BASE_FREQ_22K;
    }
    else if (u4BaseFreq == FS_16K)
    {
        _i4AsrcBaseFreq = ASRC_BASE_FREQ_16K;
    }
    else if (u4BaseFreq == FS_12K)
    {
        _i4AsrcBaseFreq = ASRC_BASE_FREQ_12K;
    }
    else if (u4BaseFreq == FS_11K)
    {
        _i4AsrcBaseFreq = ASRC_BASE_FREQ_11K;
    }
    else if (u4BaseFreq == FS_8K)
    {
        _i4AsrcBaseFreq = ASRC_BASE_FREQ_8K;
    }

    i4Level = (_i4AsrcBaseFreq * ((UINT32)_i4AsrcSmartVCXOB2rtLevel & 0x0ff)) >> 20;
    u1Byte = (UINT8)((_i4AsrcSmartVCXOB2rtLevel >> 8) & 0x01);

    if (u1Byte == 1)
    {
        // Adjust IFS faster
        _i4AsrcSmartVCXOAdjustLevel = i4Level;
    }
    else
    {
        // Adjust IFS slower
        _i4AsrcSmartVCXOAdjustLevel = -i4Level;
    }

    _AUD_DspAsrcControl();

    VERIFY(x_sema_unlock(_hAsrcControlSema) == OSR_OK);
}

//-----------------------------------------------------------------------------
/** _AUD_DspAsrcSetSmartVCXO
 *   Adjust ASRC SmartVCXO level
 *
 *  @param  u4AdjustLevel    video calculated level (0 ~ 255)
 *                     bit 8: 0 (slower), 1 (faster)
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspAsrcSetSmartVCXO(UINT32 u4AdjustLevel)
{
    UINT8 u1Byte;
    INT32 i4Level;
    AUD_DEC_STREAM_FROM_T eStreamFrom;	
#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389) || defined(CC_MT5398) || defined(CC_MT5880)	
	INT32 i4Levl2;
	SAMPLE_FREQ_T  eFs;
#endif	

#ifdef CC_MT5391_AUD_3_DECODER
    AUD_DRVGetStreamFrom(AUD_DEC_AUX, &eStreamFrom);
#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389) || defined(CC_MT5398) || defined(CC_MT5880)	
    eFs = AUD_GetSampleFreq(AUD_DEC_AUX);
#endif

#else
    AUD_DRVGetStreamFrom(AUD_DEC_MAIN, &eStreamFrom);
#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389) || defined(CC_MT5398) || defined(CC_MT5880)	
    eFs = AUD_GetSampleFreq(AUD_DEC_MAIN);
#endif
#endif

    if (eStreamFrom != AUD_STREAM_FROM_DIGITAL_TUNER)
    {
        // Only for DTV case
        return;
    }

    VERIFY(x_sema_lock(_hAsrcControlSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    _i4AsrcSmartVCXOB2rtLevel = u4AdjustLevel;
    i4Level = (_i4AsrcBaseFreq * (u4AdjustLevel & 0x0ff)) >> 20;
    u1Byte = (UINT8)((u4AdjustLevel >> 8) & 0x01);

#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389) || defined(CC_MT5398) || defined(CC_MT5880)
    
    AUD_DRVGetStreamFrom(AUD_DEC_MAIN, &eStreamFrom);  //we issue IEC clk fine tune when DTV source
    if (eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
    {
	    switch (eFs)
	    {
	        case FS_44K:				
			    if (u1Byte == 1)
		            i4Levl2 = 0x131205bc +  _i4AsrcSmartVCXOAdjustLevel *( 0x131205bc/_i4AsrcBaseFreq) ; 
			    else
				    i4Levl2 = 0x131205bc -  _i4AsrcSmartVCXOAdjustLevel *( 0x131205bc/_i4AsrcBaseFreq) ; 
			
			    bHDMIAudioSigmaDeltaFineTuneSetting(0x205bc01a, 0x21, i4Levl2 , 0x1);  //fine tune the IEC output clk in DTV
			    break;
		    case FS_48K:
		    case FS_32K:
		    default:
			    if (u1Byte == 1)
			        i4Levl2 = 0x14d867c3 +  _i4AsrcSmartVCXOAdjustLevel *( 0x14d867c3/_i4AsrcBaseFreq) ; 
			    else
				    i4Levl2 = 0x14d867c3 -  _i4AsrcSmartVCXOAdjustLevel *( 0x14d867c3/_i4AsrcBaseFreq) ; 
			
			    bHDMIAudioSigmaDeltaFineTuneSetting(0x867c3ece, 0x3d, i4Levl2 , 0x1);  //fine tune the IEC output clk in DTV
			    break;
	    }
    }
#endif	

    if (u1Byte == 1)
    {
        // Adjust IFS faster
        _i4AsrcSmartVCXOAdjustLevel = i4Level;
    }
    else
    {
        // Adjust IFS slower
        _i4AsrcSmartVCXOAdjustLevel = -i4Level;
    }
    _AUD_DspAsrcControl();

    VERIFY(x_sema_unlock(_hAsrcControlSema) == OSR_OK);
}

void _AUD_DspAsrcControl(void)
{
    INT32 i4FinalLevel;

    i4FinalLevel = _i4AsrcBaseFreq + _i4AsrcSmartVCXOAdjustLevel + _i4AsrcAVSyncAdjustLevel;

#ifdef CC_MT5391_AUD_3_DECODER
    // Set Dec2/Dec3 IFS
    vWriteDspSram(REG_DSP_SRC_FREQUENCY_0, (UINT32)i4FinalLevel);
    vWriteDspSram(REG_DSP_SRC_FREQUENCY_3, (UINT32)i4FinalLevel);

    if (_IsDualDecMode())
    {
        // Set Dec1 IFS
        vWriteDspSram(REG_DSP_SRC_FREQUENCY_2, (UINT32)i4FinalLevel);
    }
#else
    // Set Dec1 IFS
    vWriteDspSram(REG_DSP_SRC_FREQUENCY_2, (UINT32)i4FinalLevel);
#endif
}

//-----------------------------------------------------------------------------
/** _AUD_DspAsrcResetSmartVCXO
 *   Reset ASRC SmartVCXO
 *
 *  @param  void
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspAsrcResetSmartVCXO(void)
{
    _i4AsrcSmartVCXOAdjustLevel = 0;
    _i4AsrcSmartVCXOB2rtLevel = 0;
}

void AUD_DspCfgSuspend(void)
{
    VERIFY(x_sema_unlock(_ahVolCtlSema[AUD_DEC_MAIN]) == OSR_OK);
    VERIFY(x_sema_unlock(_ahVolCtlSema[AUD_DEC_AUX]) == OSR_OK);

#ifdef CC_MT5391_AUD_3_DECODER
    VERIFY(x_sema_unlock(_ahVolCtlSema[AUD_DEC_THIRD]) == OSR_OK);
#endif

#ifdef CC_AUD_4_DECODER_SUPPORT
    VERIFY(x_sema_unlock(_ahVolCtlSema[AUD_DEC_4TH]) == OSR_OK);
#endif
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetCapability
 *   Set Capability
 *
 *  @param      void
 *  @retval     void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetCapability(void)
{
    DSP_SetCapability();
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetCapability
 *   Get Capability
 *
 *  @param      void
 *  @retval     format capability
 */
//-----------------------------------------------------------------------------
UINT32 _AUD_DspGetCapability(UINT8 u1Idx)
{
    return DSP_GetCapability(u1Idx);
}

//-----------------------------------------------------------------------------
/** _AUD_DspIsFormatValid
 *  Is format valid
 *
 *  @param      u4FormatInfo: format index
 *  @retval     TRUE(valid). FALSE(invalid)
 */
//-----------------------------------------------------------------------------
BOOL _AUD_DspIsFormatValid(UINT32 u4FormatInfo)
{
    return DSP_IsFormatValid(u4FormatInfo);
}

BOOL _AUD_DspIsSupportAC3(void)
{
    return DSP_IsSupportAC3();
}

BOOL _AUD_DspIsSupportEAC3(void)
{
    return DSP_IsSupportEAC3();
}

BOOL _AUD_DspIsSupportDDCO(void)
{
    return DSP_IsSupportDDCO();
}


//-----------------------------------------------------------------------------
/** _AUD_DspQueryChannelDelay
 *  Query current channel delay.
 *
 *  @param      u1ChId:
 *  @retval     Delay Value in ms
 */
//-----------------------------------------------------------------------------
UINT32 _AUD_DspQueryChannelDelay(UINT8 u1ChId) //QUERY_DELAY
{
    UNUSED(_AUD_DspQueryChannelDelay);

    return DSP_GetChannelDelay(u1ChId);
}

UINT32 _AUD_DspGetMasterVolRawValue(UINT8 u1DecId)
{
    return _au4VOLUME[_aau1ChannelVolume[u1DecId][AUD_CH_ALL]];
}

#if 0 // 2_STC_Task_DEBUG_only
//Fixmed.... This API should move to DMX Driver.
#include "x_bim.h"
#define DMXCMD_READ32(offset)			IO_READ32(DEMUX0_BASE, ((offset) * 4))
void _DMX_FIXME_ReadSTC(UINT8 u1StcId, UINT32* u4StcBase)
{
    *u4StcBase = 0;

    switch (u1StcId)
    {
    case 0:
         *u4StcBase = PARSER_READ32(REG_A_STC_1);
         break;
    case 1:
        *u4StcBase = DMXCMD_READ32(143); //0x23c
         break;
    case 2:
        *u4StcBase = DMXCMD_READ32(163); //0x28c Need ECO IC
         break;
    }
}
#endif

#if 0 //Use DMX API, don't access DMX register directly.
#define DMXCMD_WRITE32(offset, value)	IO_WRITE32(DEMUX0_BASE, ((offset) * 4), (value))
void _DMX_SwitchStcForAudio(UINT8 u1StcId)
{
    UINT32 u4Value = 0;

    u4Value = DMXCMD_READ32(129);
    u4Value = u4Value & 0xFFFFFFFC;  // bit 0 & 1

    // Select DMX STC_1 or DMX_STC_2
    if (u1StcId == 1) //STC_1 : ID=0
        DMXCMD_WRITE32(129, ((u4Value)|2) ); // 129 * 4 =0x204
    else              //STC_2 : ID=1
        DMXCMD_WRITE32(129, ((u4Value)|3) ); // 129 * 4 =0x204
}
#endif

#if 0 //<--- Enable this when other module submit code.
extern BOOL DMX_SetStcToDspUpdate(UINT8 u1Id, BOOL fgEnable); //DMX_STC_1 : ID=0, DMX_STC_2 : ID=1
#endif
//-----------------------------------------------------------------------------
/** _AUD_DspSetStcId
 *  Set Stc ID for audio / DSP
 *
 *  @param
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetStcId(UINT8 u1DecId, UINT16 u2StcId) // 2_STC_Task
{
    UINT32 u4UopIndex = UOP_DSP_SET_STC_ID;
    UINT32 u4ShmIndex = W_SYSTEM_SETUP;
    UINT16 u2Config = 0;

    UNUSED(_AUD_DspSetStcId);

    // UOP according to Decoder ID.
    if (u1DecId == AUD_DEC_MAIN)
    {
        u4UopIndex = UOP_DSP_SET_STC_ID;
        u4ShmIndex = W_SYSTEM_SETUP;
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        u4UopIndex = UOP_DSP_SET_STC_ID_DEC2;
        u4ShmIndex = W_SYSTEM_SETUP_DEC2;
    }
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u4UopIndex = UOP_DSP_SET_STC_ID_DEC3;
        u4ShmIndex = W_SYSTEM_SETUP_DEC3;
    }
    else
    {
        LOG(3, "_AUD_DspSetStcId : Invalid Decoder ID %d \n", u1DecId);
        return;
    }

    // Set STC ID. default value is Parser STC.
    // STC for System : 0 means AUDIO, 1 means Demux. (There are 2 Demux STC, need to switch by RISC)
    // DSP system_setup bit14 : 1-audio,  0-demux STC
    u2Config = (u2ReadShmUINT16(u4ShmIndex) & 0xBFFF); //clear bit 14
    if (u2StcId == AUD_STC_AUDIO)
    {
        u2Config |= 0x1 << 14;
    }
    else if (u2StcId == AUD_STC_DMX)
    {
        u2Config |= 0x0 << 14;
        //_DMX_SwitchStcForAudio(1); // Enable Audio Update DMX_STC_1, should use DMX API
        #if 0 //<--- Enable this when other module submit code.
        DMX_SetStcToDspUpdate(0, TRUE);  // Enable Audio Update DMX_STC_1 (ID=0.)
        #endif
    }
    else
    {
        LOG(3, "_AUD_DspSetStcId : Dec(%d) Invalid STC ID %d \n", u1DecId, u2StcId);
        u2Config |= 0x1 << 14;
    }

    LOG(3, " _AUD_DspSetStcId, Set Dec(%d) STC ID to %d\n", u1DecId, u2StcId);
    vWriteShmUINT16(u4ShmIndex, u2Config);
    DSP_SendDspTaskCmd(u4UopIndex);

}

void _AUD_DspGetStcId(UINT8 u1DecId, UINT16 *u2StcId) // 2_STC_Task
{
    UINT32 u4ShmIndex = W_SYSTEM_SETUP;
    UINT16 u2Config = 0;

    UNUSED(_AUD_DspGetStcId);

    // UOP according to Decoder ID.
    if (u1DecId == AUD_DEC_MAIN)
    {
        u4ShmIndex = W_SYSTEM_SETUP;
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        u4ShmIndex = W_SYSTEM_SETUP_DEC2;
    }
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u4ShmIndex = W_SYSTEM_SETUP_DEC3;
    }
    else
    {
        LOG(3, "_AUD_DspGetStcId : Invalid Decoder ID %d \n", u1DecId);
        return;
    }

    u2Config = (u2ReadShmUINT16(u4ShmIndex) & 0x4000)>>14; //bit 14

    // STC for System : 0 means AUDIO, 1 means Demux.
    // DSP system_setup bit14 : 1-audio,  0-demux STC
    if (u2Config == 0)
    {
        *u2StcId = 1;
    }
    else
    {
        *u2StcId = 0;
    }

}

BOOL AUD_DspIsFmtSupport(AUD_FMT_T eDecType)
{
    BOOL fgRet=FALSE;

    switch(eDecType)
    {
        case AUD_FMT_MPEG:
        case AUD_FMT_PCM:
            fgRet = TRUE;
            break;
        case AUD_FMT_AAC:
            fgRet = (IS_SupportDDCO() || IS_SupportMS10() || IS_SupportH264())&&AUD_IsSupportMpeg4();
            break;
        case AUD_FMT_AC3:
            fgRet = (IS_SupportAC3() || IS_SupportEAC3() || IS_SupportMS10() || IS_SupportH264())&&AUD_IsSupportMpeg4();
            break;
        default :
            fgRet = FALSE;
            break;
    }
    return fgRet;
}

BOOL AUD_DspIsAACL2(void)
{
    UINT8 u1Mode;
    if(_rDspVars[AUD_DEC_AUX].bDspStrTyp == AAC_STREAM)
    {
        u1Mode= u1GetAacChNum(AUD_DEC_AUX);
        if(u1Mode >=3)
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
    else
    {
        return FALSE;
    }
}
UINT8 _AUD_DspGetCategoryCode(void)
{
    return _u1IecCategory;    
}



UINT8 _AUD_DspGetCopyright(void)
{
    return _u1IecCopyright;    
}

UINT8 AUD_DspGetCategoryCode()
{
    return _AUD_DspGetCategoryCode();
}



void _AUD_DspSetCopyright(UINT8 copyright)
{
    _u1IecCopyright = copyright;
}

void _AUD_DspSetCategoryCode(UINT8 categoryCode)
{
    _u1IecCategory = categoryCode;
}

BOOL _AUD_DspSetSCMS(UINT8 u1CopyRight, UINT8 u1CategoryCode)
{
    _AUD_DspSetCopyright(u1CopyRight);
    _AUD_DspSetCategoryCode(u1CategoryCode);
    _AUD_DspSetRegType();
}

BOOL AUD_DspSetSCMS(UINT8 u1CopyRight, UINT8 u1CategoryCode)
{
    return _AUD_DspSetSCMS(u1CopyRight,u1CategoryCode);
}


void _AUD_DspSetRegType (void)
{
    LOG (3, "Set _AUD_DspSetRegType  _u1IecCopyright = 0x%x _u1IecCategory = 0x%x.\n", _u1IecCopyright, _u1IecCategory);

    vWriteShmUINT8(B_IEC_COPYRIGHT, IEC_COPY_NEVER);
    vWriteShmUINT8(B_IEC_CATEGORY_CODE, IEC_NON_ORIGINAL_BROAD_CAST);

    DSP_SendDspTaskCmd(UOP_DSP_IEC_SET_DATA);
}

void AUD_SoftwareMuteQuery(void)
{
    UINT8 i;

    Printf("\nChannel Outport Setting: <0: close; 1: open>\n");
    for ( i=0; i< AUD_CH_AUX_FRONT_LEFT ;i++)
    {
	    Printf("    _aafgChlOutPort[DEC0][%d] = %d \n", i, _aafgChlOutPort[AUD_DEC_MAIN][i]);
    }

	for ( i=0; i< AUD_CH_REAR_LEFT ;i++)
    {
	    Printf("    _aafgChlOutPort[DEC1][%d] = %d \n", i, _aafgChlOutPort[AUD_DEC_AUX][i]);
    }

	Printf("\nSoftware mute control flag: <0: unmute; 1: mute>\n");
		
    for ( i=0; i< AUD_CH_AUX_FRONT_LEFT ;i++)
    {
	    Printf("    _aafgChlMute[DEC0][%d] = %d \n", i, _aafgChlMute[AUD_DEC_MAIN][i]);
    }	
    printf("\n");
    for ( i=0; i< AUD_CH_AUX_FRONT_LEFT ;i++)
    {
        Printf("    _aafgChlHdmiChgFmtMute[DEC0][%d] = %d \n",i, _aafgChlHdmiChgFmtMute[AUD_DEC_MAIN][i]);
    }
    printf("\n");

#ifdef CC_ENABLE_AV_SYNC
    for ( i=0; i< AUD_CH_AUX_FRONT_LEFT ;i++)
    {
        Printf("    _aafgChDecPlayMute[DEC0][%d] = %d \n", i,_aafgChDecPlayMute[AUD_DEC_MAIN][i]);
    }	
    printf("\n");
#endif

    for ( i=0; i< AUD_CH_AUX_FRONT_LEFT ;i++)
    {
        Printf("    _aafgChAtvMute[DEC0][%d] = %d \n", i,_aafgChAtvMute[AUD_DEC_MAIN][i]);
    }	
    printf("\n");


    for (i=0; i< AUD_DEC_NUM; i++)
	{
	
	    Printf("    _afgMuteEnable[DEC%d] = %d \n", i,_afgMuteEnable[i]);
	   
    }
	
    printf("\n");

    for (i=0; i< AUD_DEC_NUM; i++)
    {
	    Printf("    _afgHdmiModeMuteEnable[DEC%d] = %d \n", i,_afgHdmiModeMuteEnable[i]);        
    }
	
    printf("\n");

    for (i=0; i< AUD_DEC_NUM; i++)
    {
        Printf("    _afgStopMuteEnable[DEC%d] = %d \n", i,_afgStopMuteEnable[i]);            
    }
	
    printf("\n");

    for (i=0; i< AUD_DEC_NUM; i++)
    {
        Printf("    _afgAllChannelMute[DEC%d] = %d \n", i,_afgAllChannelMute[i]);          
    }
	
    printf("\n");

    
    for ( i=0; i< AUD_CH_AUX_FRONT_LEFT ;i++)
    {  
	     Printf("    _aafgSoundModeMute[DEC0][%d] = %d \n", i, _aafgSoundModeMute[AUD_DEC_MAIN][i]);
    }
    printf("\n");

    for ( i=0; i< AUD_CH_REAR_LEFT ;i++)
    {
		Printf("    _aafgSoundModeMute[DEC1][%d] = %d \n", i, _aafgSoundModeMute[AUD_DEC_AUX][i]);  
    }
	
}



