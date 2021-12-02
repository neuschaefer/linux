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
                    Include files
 ----------------------------------------------------------------------------*/
#include "x_rm.h"
#include "x_rm_dev_types.h"
#include "drv_comp_id.h"
#include "aud_if.h"
#include "drv_dsp_cfg.h"
#include "drv_name.h"
#include "../aud/include/aud_debug.h"
#include "../aud/audio/aud_drv.h"
#include "x_aud_dec.h"
#include "x_buf_agt.h"
#include "x_assert.h"
#include "drvcust_if.h"
#include "u_drv_cust.h"
#include "bagt_mw.h"

/*-----------------------------------------------------------------------------
                    Macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#ifdef CC_AUD_DVBT_SUPPORT
#ifdef CC_AUD_SINGLE_DEC_MODE
// Do nothing...
// Original single decoder
#else
#define CC_AUD_DUAL_DEC_SUPPORT
#endif
#endif

#define TREBLE_MAX_VAL          100
#define TREBLE_MIN_VAL          0
#define BASS_MAX_VAL            100
#define BASS_MIN_VAL            0

#define TREBLE_TABLE_MAX_VAL    28
#define TREBLE_TABLE_MIN_VAL    0
#define BASS_TABLE_MAX_VAL      28
#define BASS_TABLE_MIN_VAL      0

#define OUT_PORT_NUM            AUD_DEC_OUT_PORT_SPEAKER
#define AUD_MAX_DELAY_IN_MS     20

#ifdef CC_AUD_7BAND_EQUALIZER
#define CH_EQ_BAND_NUM          7
#else
#define CH_EQ_BAND_NUM          5
#endif

#define AUD_DEC_PIP_SUB         2   //decoder id "2" means audio in PIP sub mode

#ifdef CC_AUD_DVBT_SUPPORT
#ifndef CC_AUD_AUSTRALIA_SUPPORT
#define AUD_FMT_CAPABILITY      (UINT32)(AUD_DEC_CAP_FMT_MPEG |     \
                                                AUD_DEC_CAP_FMT_PCM |       \
                                                AUD_DEC_CAP_FMT_MP3 |       \
                                                AUD_DEC_CAP_FMT_EU_CANAL_PLUS |   \
                                                AUD_DEC_CAP_FMT_TV_SYS)
#else
#define AUD_FMT_CAPABILITY      (UINT32)(AUD_DEC_CAP_FMT_MPEG |             \
                                         AUD_DEC_CAP_FMT_AC3  |             \
                                         AUD_DEC_CAP_FMT_PCM  |             \
                                         AUD_DEC_CAP_FMT_MP3  |             \
                                         AUD_DEC_CAP_FMT_EU_CANAL_PLUS |    \
                                         AUD_DEC_CAP_FMT_TV_SYS)

#endif

#else
#define AUD_FMT_CAPABILITY      (UINT32)(AUD_DEC_CAP_FMT_MPEG |     \
                                                AUD_DEC_CAP_FMT_AC3 |       \
                                                AUD_DEC_CAP_FMT_PCM |       \
                                                AUD_DEC_CAP_FMT_MP3 |       \
                                                AUD_DEC_CAP_FMT_MTS |       \
                                                AUD_DEC_CAP_FMT_TV_SYS)
#endif

#define GET_MAP_VAL(u1ValMax, u1ValMin, u1IdxMax, u1IdxMin, u1Idx)        \
          ((UINT8)((UINT16)(((UINT16)u1ValMax - (UINT16)u1ValMin) * ((UINT16)u1Idx - (UINT16)u1IdxMin)) /   \
          (UINT16)((UINT16)u1IdxMax - (UINT16)u1IdxMin)) + u1ValMin)                   \

UINT8 GET_TREBLE_MAP_VAL(UINT8 u1Val)
{
      return GET_MAP_VAL(TREBLE_TABLE_MAX_VAL, TREBLE_TABLE_MIN_VAL,
                         TREBLE_MAX_VAL, TREBLE_MIN_VAL, u1Val);
}

UINT8 GET_BASS_MAP_VAL(UINT8 u1Val)
{
      return GET_MAP_VAL(BASS_TABLE_MAX_VAL, BASS_TABLE_MIN_VAL,
                      BASS_MAX_VAL, BASS_MIN_VAL, u1Val);
}

#define M_GET_VALIDATE(TYPE)                \
    if (*pz_get_info_len < sizeof(TYPE))    \
    {                                       \
        return RMR_DRV_NOT_ENOUGH_SPACE;    \
    }                                       \
    *pz_get_info_len = sizeof(TYPE);        \

#define M_SET_VALIDATE(TYPE)                \
    if (sizeof(TYPE) != z_set_info_len)     \
    {                                       \
        return RMR_DRV_INV_SET_INFO;        \
    }                                       \

/*-----------------------------------------------------------------------------
                    Local type definitions
 ----------------------------------------------------------------------------*/

/* Channel volume info. */
typedef struct _AUD_DEC_CH_VOL_T
{
    UINT8  u1VolL;
    UINT8  u1VolR;
    UINT8  u1VolSL;
    UINT8  u1VolSR;
    UINT8  u1VolC;
    UINT8  u1VolSW;
    UINT8  u1VolDmxL;
    UINT8  u1VolDmxR;
    UINT8  u1VolBypassL;
    UINT8  u1VolBypassR;
    UINT8  u1VolMaster;
}   AUD_DEC_CH_VOL_T;

#define VOL_BASE_LEVEL              ((UINT8)100)

/*-----------------------------------------------------------------------------
                    Local Variable definitions
 ----------------------------------------------------------------------------*/

static UINT16                  _au2MwConnectComp[2];
static AUD_DEC_FMT_INFO_T      _arAudDecFmt[2];
static AUD_FMT_T               _arAudDriverDecFmt[2];
static AUD_DEC_CTRL_T          _aeCtrlSetting[2] = {AUD_DEC_CTRL_STOP, AUD_DEC_CTRL_STOP};
static AUD_DEC_MUTE_TYPE_T     _aeMuteType[2] = {AUD_DEC_MUTE_OFF, AUD_DEC_MUTE_OFF};
static AUD_DEC_SPDIF_TYPE_T    _aeSpdif[2] = {AUD_DEC_SPDIF_PCM_24, AUD_DEC_SPDIF_PCM_24};
static AUD_DEC_HDCD_FLTR_T     _aeHdcdFilter[2] = {AUD_DEC_HDCD_FLTR_OFF, AUD_DEC_HDCD_FLTR_OFF};
static AUD_DEC_LS_MODE_T       _aeLsMode[2][6] = {{AUD_DEC_LS_MODE_LARGE_FREQ_RANGE, AUD_DEC_LS_MODE_LARGE_FREQ_RANGE,
                                                   AUD_DEC_LS_MODE_LARGE_FREQ_RANGE, AUD_DEC_LS_MODE_LARGE_FREQ_RANGE,
                                                   AUD_DEC_LS_MODE_LARGE_FREQ_RANGE, AUD_DEC_LS_MODE_LARGE_FREQ_RANGE},
                                                  {AUD_DEC_LS_MODE_LARGE_FREQ_RANGE, AUD_DEC_LS_MODE_LARGE_FREQ_RANGE,
                                                   AUD_DEC_LS_MODE_LARGE_FREQ_RANGE, AUD_DEC_LS_MODE_LARGE_FREQ_RANGE,
                                                   AUD_DEC_LS_MODE_LARGE_FREQ_RANGE, AUD_DEC_LS_MODE_LARGE_FREQ_RANGE}};

static AUD_DEC_SE_INFO_T       _arSeInfo[2];
static AUD_DEC_DOWNMIX_MODE_T  _aeDownmixMode[2] = {AUD_DEC_DM_OFF, AUD_DEC_DM_OFF};
static AUD_DEC_DRC_T           _aeDrc[2] = {AUD_DEC_DRC_OFF, AUD_DEC_DRC_OFF};
static UINT32                  _au4OutPort[2] = {AUD_DEC_OUT_PORT_FLAG_NONE, AUD_DEC_OUT_PORT_FLAG_NONE};
static AUD_DEC_MEM_BUFFER_INFO_T _arMemBuf[2];
static AUD_DEC_CMPSS_MODE_T _aeCmprssMode[2] = {AUD_DEC_CMPSS_MODE_LINE, AUD_DEC_CMPSS_MODE_LINE};

static AUD_DEC_CH_VOL_T _arChVol[2] =
                                     {{VOL_BASE_LEVEL, VOL_BASE_LEVEL, VOL_BASE_LEVEL, VOL_BASE_LEVEL, VOL_BASE_LEVEL, VOL_BASE_LEVEL,
                                       VOL_BASE_LEVEL, VOL_BASE_LEVEL, VOL_BASE_LEVEL, VOL_BASE_LEVEL, VOL_BASE_LEVEL},
                                      {VOL_BASE_LEVEL, VOL_BASE_LEVEL, VOL_BASE_LEVEL, VOL_BASE_LEVEL, VOL_BASE_LEVEL, VOL_BASE_LEVEL,
                                       VOL_BASE_LEVEL, VOL_BASE_LEVEL, VOL_BASE_LEVEL, VOL_BASE_LEVEL, VOL_BASE_LEVEL}};
static BOOL _afgAgcFlag[2] = {TRUE, TRUE};
static AUD_DEC_STREAM_FROM_T _aeStrSrc[2] = {AUD_STREAM_FROM_DIGITAL_TUNER, AUD_STREAM_FROM_DIGITAL_TUNER};
static UINT16 _aau2MasterDelay[2][AUD_DEC_OUT_PORT_SPEAKER+1] = {{0, 0, 0, 0, 0, 0},
                                                                {0, 0, 0, 0, 0, 0}};

static AUD_DEC_CHNL_DELAY_IND_T _arChlDelayInd[2][OUT_PORT_NUM];
static AUD_DEC_COPY_PROTECT_T _aeCopyProtect[2] = {AUD_DEC_COPY_PROTECT_NO_RESTRICT, AUD_DEC_COPY_PROTECT_NO_RESTRICT};
//static UINT8 _au1Balance[2] = {(MAX_VOL_LEVEL/2), (MAX_VOL_LEVEL/2)};
static BOOL _afgMute[2] = {FALSE, FALSE};
static BOOL _afgSpeakerPortOn[2] = {FALSE, FALSE};
static INT16 _i2HpVol = 0;
//static UINT32 _au4TvSysMask[2] = {0, 0};


#ifdef CC_AUD_DVBT_SUPPORT
static AUD_DEC_TV_AUD_SYS_T _arTvAudSysMask[2];
#endif

#ifdef CC_AUD_7BAND_EQUALIZER
static INT8 _ai1UserEqCfg[AUD_EQ_NUM][CH_EQ_BAND_NUM] = {{0, 0, 0, 0, 0, 0, 0},
                                                         {0, 0, 0, 0, 0, 0, 0},
                                                         {0, 0, 0, 0, 0, 0, 0},
                                                         {0, 0, 0, 0, 0, 0, 0},
                                                         {0, 0, 0, 0, 0, 0, 0},
                                                         {0, 0, 0, 0, 0, 0, 0},
                                                         {0, 0, 0, 0, 0, 0, 0},
                                                         {0, 0, 0, 0, 0, 0, 0},
                                                         {0, 0, 0, 0, 0, 0, 0},
                                                         {0, 0, 0, 0, 0, 0, 0},
                                                         {0, 0, 0, 0, 0, 0, 0},
                                                         {0, 0, 0, 0, 0, 0, 0},
                                                         {0, 0, 0, 0, 0, 0, 0},
                                                         {0, 0, 0, 0, 0, 0, 0},}; // BBE TV EQ configuration in unit of 1/10 dB
#else
static INT8 _ai1UserEqCfg[AUD_EQ_NUM][CH_EQ_BAND_NUM] = {{0, 0, 0, 0, 0},
                                                         {0, 0, 0, 0, 0},
                                                         {0, 0, 0, 0, 0},
                                                         {0, 0, 0, 0, 0},
                                                         {0, 0, 0, 0, 0},
                                                         {0, 0, 0, 0, 0},
                                                         {0, 0, 0, 0, 0},
                                                         {0, 0, 0, 0, 0},
                                                         {0, 0, 0, 0, 0},
                                                         {0, 0, 0, 0, 0},
                                                         {0, 0, 0, 0, 0},
                                                         {0, 0, 0, 0, 0},
                                                         {0, 0, 0, 0, 0},
                                                         {0, 0, 0, 0, 0},}; // BBE TV EQ configuration in unit of 1/10 dB
#endif

static AUD_DEC_AUD_CLIP_T _arAudClipSetType[2];
static AUD_DEC_AUD_CLIP_INFO_T _arAudClipInfo[2] = {{0, 0}, {0, 0}};
static AUD_CLIP_TYPE_T _arAudClip[2];
static BOOL _afgPlayAudClip[2] = {FALSE, FALSE};
static BOOL _afgTvSysDetecting[2] = {FALSE, FALSE};
static AUD_DEC_PL2_CTRL_INFO_T _rPrologic2Type;
static AUD_DEC_NFY_INFO_T      _arAudMwNotifyInfo[2];
static BOOL _afgDolbyBanner[2] = {FALSE, FALSE};
static AUD_DEC_BBE_MODE_T  _aeBbeMode[2] = {AUD_DEC_BBE_MODE_OFF, AUD_DEC_BBE_MODE_OFF};
static AUD_DEC_MUTE_EX_INFO_T _arAudMuteEx[2] = {0, 0};
static BOOL _fgPlayAudDscr = FALSE; //For Audio Description

extern void AUD_DspMasterVolume(UINT8 u1DecID, UINT8 u1Volume);
extern void _ADAC_SpeakerEnable(BOOL fgEnable);
extern void AUD_AVMuxSel(UINT8 u1Sel);
extern void _ADAC_MuteEnable(BOOL fgEnable);
extern void AUD_DspDvdDisable(void);
extern AUD_TYPE_T DSP_GetMtsSrcTyp(UINT8 u1DecId);
extern AUD_TYPE_T DSP_GetAudChlInfo(UINT8 u1DecId);
extern void DSP_SetEuroCanalPlusMode(BOOL fgEnable);
extern void AUD_AoutPADMux(AUD_CH_T ePAD_SDATA0, AUD_CH_T ePAD_SDATA1, AUD_CH_T ePAD_SDATA2,
                    AUD_CH_T ePAD_SDATA3, AUD_CH_T ePAD_SDATA4, AUD_CH_T ePAD_SDATA5,
                    UINT8 PAD_MCLK0);
extern void DSP_SetDetTvSrcMode(TV_AUD_SYS_T eAudSys);
extern UINT32 AUD_AudioDescriptionChlSel(UINT8 u1DecId, AUD_CHL_DEF_T eChl);

/*-----------------------------------------------------------------------------
                    Function prototype
 ----------------------------------------------------------------------------*/
static INT32 _AudConnect(DRV_COMP_ID_T*  pt_comp_id,
                        DRV_CONN_TYPE_T  e_conn_type,
                        const VOID*      pv_conn_info,
                        SIZE_T           z_conn_info_len,
                        VOID*            pv_tag,
                        x_rm_nfy_fct     pf_nfy);
static INT32 _AudDisconnect(DRV_COMP_ID_T*   pt_comp_id,
                        DRV_DISC_TYPE_T  e_disc_type,
                        const VOID*      pv_disc_info,
                        SIZE_T           z_disc_info_len);
static INT32 _AudGet(DRV_COMP_ID_T* pt_comp_id,
                    DRV_GET_TYPE_T  e_get_type,
                    VOID*           pv_get_info,
                    SIZE_T*         pz_get_info_len);
static INT32 _AudSet(DRV_COMP_ID_T* pt_comp_id,
                    DRV_SET_TYPE_T  e_set_type,
                    const VOID*     pv_set_info,
                    SIZE_T          z_set_info_len);

static INT32 _GetCapability(UINT8 u1DecId, AUD_DEC_CAPABILITY_INFO_T * prCapability);
static void _GetAudInfo(UINT8 u1DecId, AUD_DEC_AUD_INFO_T * prAudInfo);

/*-----------------------------------------------------------------------------
                    Local Function definitions
 ----------------------------------------------------------------------------*/

#define IS_TUNER_UNI_SYS()      TRUE
#define IS_TUNER_MULTI_SYS()    TRUE

#ifdef CC_AUD_DVBT_SUPPORT

static BOOL _IsMaskAmbiguousTvSys(UINT8 u1DecId)
{
#if 0
    if (_arTvAudSysMask[u1DecId].ui4_tv_sys_mask ==
                                (TV_SYS_MASK_A        |
                                 TV_SYS_MASK_B        |
                                 TV_SYS_MASK_C        |
                                 TV_SYS_MASK_D        |
                                 TV_SYS_MASK_E        |
                                 TV_SYS_MASK_F        |
                                 TV_SYS_MASK_G        |
                                 TV_SYS_MASK_H        |
                                 TV_SYS_MASK_I        |
                                 TV_SYS_MASK_J        |
                                 TV_SYS_MASK_K        |
                                 TV_SYS_MASK_K_PRIME  |
                                 TV_SYS_MASK_L        |
                                 TV_SYS_MASK_L_PRIME  |
                                 TV_SYS_MASK_N        |
                                 TV_SYS_MASK_M))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

#else
    UNUSED(u1DecId);
    return FALSE;
#endif
}

static BOOL _IsMaskSetToMtsSys(UINT8 u1DecId)
{
    if ((!_IsMaskAmbiguousTvSys(u1DecId)) &&
        ((_arTvAudSysMask[u1DecId].ui4_aud_sys_mask & AUDIO_SYS_MASK_BTSC) == AUDIO_SYS_MASK_BTSC) &&
        (((_arTvAudSysMask[u1DecId].ui4_tv_sys_mask & TV_SYS_MASK_M) == TV_SYS_MASK_M) ||
        ((_arTvAudSysMask[u1DecId].ui4_tv_sys_mask & TV_SYS_MASK_N) == TV_SYS_MASK_N)))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static BOOL _IsMaskSetToA2Sys(UINT8 u1DecId)
{
    if ((!_IsMaskAmbiguousTvSys(u1DecId)) &&
        (((_arTvAudSysMask[u1DecId].ui4_aud_sys_mask & AUDIO_SYS_MASK_FM_A2) == AUDIO_SYS_MASK_FM_A2) ||
        ((_arTvAudSysMask[u1DecId].ui4_aud_sys_mask & AUDIO_SYS_MASK_FM_A2_DK1) == AUDIO_SYS_MASK_FM_A2_DK1) ||
        ((_arTvAudSysMask[u1DecId].ui4_aud_sys_mask & AUDIO_SYS_MASK_FM_A2_DK2) == AUDIO_SYS_MASK_FM_A2_DK2)))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static BOOL _IsMaskSetToPalSys(UINT8 u1DecId)
{
    if ((!_IsMaskAmbiguousTvSys(u1DecId)) &&
        (((_arTvAudSysMask[u1DecId].ui4_aud_sys_mask & AUDIO_SYS_MASK_FM_MONO) == AUDIO_SYS_MASK_FM_MONO) ||
        ((_arTvAudSysMask[u1DecId].ui4_aud_sys_mask & AUDIO_SYS_MASK_AM) == AUDIO_SYS_MASK_AM) ||
        ((_arTvAudSysMask[u1DecId].ui4_aud_sys_mask & AUDIO_SYS_MASK_NICAM) == AUDIO_SYS_MASK_NICAM)))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static BOOL _IsMaskSetToFmfmSys(UINT8 u1DecId)
{
    if ((!_IsMaskAmbiguousTvSys(u1DecId)) &&
        ((_arTvAudSysMask[u1DecId].ui4_aud_sys_mask & AUDIO_SYS_MASK_FM_EIA_J) == AUDIO_SYS_MASK_FM_EIA_J))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static BOOL _IsRunDetection(UINT8 u1DecId)
{
    return _IsMaskAmbiguousTvSys(u1DecId);
}

#endif

static UINT8 _AudConvertDecId(UINT16 u2_comp_id, UINT8* u1DecId)
{
    if (u2_comp_id > (AUD_DEC_NUM - 1))
    {
        *u1DecId = AUD_DEC_MAIN;
        if (u2_comp_id == AUD_DEC_PIP_SUB) //pseduo decoder id for audio in PIP sub mode
        {
            AUD_DrvSetPIPVideoPath(AUD_DEC_MAIN, 1); //PIP sub mode : 1
            return 0;
        }
        else
        {
            return 1;   //middleware set wrong decoder id
        }
    }
    AUD_DrvSetPIPVideoPath(AUD_DEC_MAIN, 0); //PIP main mode : 0
    *u1DecId = (UINT8)u2_comp_id;
    return 0;
}


static void _AudMwNfyFunc(void *          pvNfyTag,
                          AUD_COND_NFY_T  eNfyCond,
                          UINT32          u4Data1,
                          UINT32          u4Data2)
{
    UINT32 u4NfyTag = 0;
    UINT8  u1DecId;

    u4NfyTag = ((UINT32)pvNfyTag & 0x1);
    u1DecId = (UINT8)u4NfyTag;

    UNUSED(u1DecId);

    //Audio Description
    // Interface with middleware
    //     Middleware set decoder0 means normal set/get
    //     Middleware set decoder1 means audio description set/get
    if ((!_fgPlayAudDscr)&&(u1DecId==AUD_DEC_AUX))
    {
        u1DecId = AUD_DEC_MAIN;
    }

    // Currently use only main decoder
    if (_arAudMwNotifyInfo[u1DecId].pf_aud_dec_nfy != NULL)
    {
        _arAudMwNotifyInfo[u1DecId].pf_aud_dec_nfy(_arAudMwNotifyInfo[u1DecId].pv_tag,
                                                        (AUD_DEC_COND_T)eNfyCond, u4Data1, u4Data2);
    }
    else
    {
        ASSERT(0);
    }
}

#ifdef CC_AUD_DVBT_SUPPORT
static TV_AUD_SYS_T _GetTvSysFormat(UINT8 u1DecId)
{
    TV_AUD_SYS_T eTvSys = SV_NONE_DETECTED;
    //_au4TvSysMask[u1DecId] = AUD_DspGetAtvTvSys();

    if (_IsMaskSetToA2Sys(u1DecId))
    {
        if (((_arTvAudSysMask[u1DecId].ui4_tv_sys_mask & TV_SYS_MASK_B) == TV_SYS_MASK_B) &&
            ((_arTvAudSysMask[u1DecId].ui4_tv_sys_mask & TV_SYS_MASK_G) == TV_SYS_MASK_G))
        {
            eTvSys =  SV_A2_BG;
        }
        else if (((_arTvAudSysMask[u1DecId].ui4_aud_sys_mask & AUDIO_SYS_MASK_FM_A2) == AUDIO_SYS_MASK_FM_A2))
        {
            eTvSys = SV_A2_DK3;
        }
        else if (((_arTvAudSysMask[u1DecId].ui4_aud_sys_mask & AUDIO_SYS_MASK_FM_A2_DK1) == AUDIO_SYS_MASK_FM_A2_DK1))
        {
            eTvSys = SV_A2_DK1;
        }
        else if (((_arTvAudSysMask[u1DecId].ui4_aud_sys_mask & AUDIO_SYS_MASK_FM_A2_DK2) == AUDIO_SYS_MASK_FM_A2_DK2))
        {
            eTvSys = SV_A2_DK2;
        }
        else if (((_arTvAudSysMask[u1DecId].ui4_tv_sys_mask & TV_SYS_MASK_M) == TV_SYS_MASK_M))
        {
            eTvSys = SV_NTSC_M;
        }
    }
    else if (_IsMaskSetToPalSys(u1DecId))
    {
        if ((_arTvAudSysMask[u1DecId].ui4_tv_sys_mask & TV_SYS_MASK_I) == TV_SYS_MASK_I)
        {
            eTvSys =  SV_PAL_I;
        }
        else if (((_arTvAudSysMask[u1DecId].ui4_tv_sys_mask & TV_SYS_MASK_D) == TV_SYS_MASK_D) &&
            ((_arTvAudSysMask[u1DecId].ui4_tv_sys_mask & TV_SYS_MASK_K) == TV_SYS_MASK_K))
        {
            eTvSys = SV_PAL_DK;
        }
        else if (((_arTvAudSysMask[u1DecId].ui4_tv_sys_mask & TV_SYS_MASK_B) == TV_SYS_MASK_B) &&
                 ((_arTvAudSysMask[u1DecId].ui4_tv_sys_mask & TV_SYS_MASK_G) == TV_SYS_MASK_G))
        {
            eTvSys = SV_PAL_BG;
        }
        else if (((_arTvAudSysMask[u1DecId].ui4_tv_sys_mask & TV_SYS_MASK_L) == TV_SYS_MASK_L))
        {
            eTvSys = SV_SECAM_L;
        }
        else if (((_arTvAudSysMask[u1DecId].ui4_tv_sys_mask & TV_SYS_MASK_L_PRIME) == TV_SYS_MASK_L_PRIME))
        {
            eTvSys = SV_SECAM_L_PLUM;
        }
    }
    else if (_IsMaskSetToMtsSys(u1DecId))
    {
        eTvSys = SV_MTS;
    }
    else if (_IsMaskSetToFmfmSys(u1DecId))
    {
        eTvSys = SV_FM_FM;
    }

    return eTvSys;
}

#endif

static void _AudSetDetTvSrcMode(UINT8 u1DecId)
{
    if ((Aud_GetTvSystemMask(u1DecId) & TV_SYS_MASK_L) == TV_SYS_MASK_L)
    {
        DSP_SetDetTvSrcMode(SV_SECAM_L);
        AUD_DspSetDetectDefaultMode(4); // SECAM-L
    }
    else if ((Aud_GetTvSystemMask(u1DecId) & TV_SYS_MASK_L_PRIME) == TV_SYS_MASK_L_PRIME)
    {
        DSP_SetDetTvSrcMode(SV_SECAM_L_PLUM);
        AUD_DspSetDetectDefaultMode(4); // SECAM-L or SECAM-L'
    }
    else
    {
        DSP_SetDetTvSrcMode(SV_SECAM_DK);
        AUD_DspSetDetectDefaultMode(0); // PAL-DK
    }
}

static void _AudGetCtrl(UINT8 u1DecId, AUD_DEC_CTRL_T * peAudCtrl)
{
    *peAudCtrl = _aeCtrlSetting[u1DecId];
}

static void _GetFormat(UINT8 u1DecId, AUD_DEC_FMT_INFO_T * prFormatInfo)
{
    prFormatInfo->e_fmt = _arAudDecFmt[u1DecId].e_fmt;
    prFormatInfo->pv_info = _arAudDecFmt[u1DecId].pv_info;
}

static void _GetVolume(UINT8 u1DecId, AUD_DEC_VOLUME_INFO_T * prChannelVol)
{
    if (prChannelVol->e_vol_type == AUD_DEC_ALL_CH)
    {
        prChannelVol->u.ui1_level = _arChVol[u1DecId].u1VolMaster;
    }
    else /* AUD_DEC_INDIVIDUAL_CH */
    {
        switch (prChannelVol->u.t_ch_vol.e_out_port)
        {
        case AUD_DEC_OUT_PORT_2_CH:       // This is for dmx channel
        case AUD_DEC_OUT_PORT_SPEAKER: // doing the same thing with "AUD_DEC_OUT_PORT_2_CH"
            switch (prChannelVol->u.t_ch_vol.e_ls)
            {
                case AUD_DEC_LS_FRONT_LEFT:
                    prChannelVol->u.t_ch_vol.ui1_level = _arChVol[u1DecId].u1VolDmxL;
                    break;
                case AUD_DEC_LS_FRONT_RIGHT:
                    prChannelVol->u.t_ch_vol.ui1_level = _arChVol[u1DecId].u1VolDmxR;
                    break;
                default:
                    break;
            }
            break;
        case AUD_DEC_OUT_PORT_5_1_CH:
            switch (prChannelVol->u.t_ch_vol.e_ls)
            {
                case AUD_DEC_LS_FRONT_LEFT:
                    prChannelVol->u.t_ch_vol.ui1_level = _arChVol[u1DecId].u1VolL;
                    break;
                case AUD_DEC_LS_FRONT_RIGHT:
                    prChannelVol->u.t_ch_vol.ui1_level = _arChVol[u1DecId].u1VolR;
                    break;
                case AUD_DEC_LS_REAR_LEFT:
                    prChannelVol->u.t_ch_vol.ui1_level = _arChVol[u1DecId].u1VolSL;
                    break;
                case AUD_DEC_LS_REAR_RIGHT:
                    prChannelVol->u.t_ch_vol.ui1_level = _arChVol[u1DecId].u1VolSR;
                    break;
                case AUD_DEC_LS_CENTER:
                    prChannelVol->u.t_ch_vol.ui1_level = _arChVol[u1DecId].u1VolC;
                    break;
                case AUD_DEC_LS_SUB_WOOFER:
                    prChannelVol->u.t_ch_vol.ui1_level = _arChVol[u1DecId].u1VolSW;
                    break;
                default:
                    break;
            }
            break;
        case AUD_DEC_OUT_PORT_SPDIF:
            // Do nothing
            break;
        case AUD_DEC_OUT_PORT_2_CH_BY_PASS:
            switch (prChannelVol->u.t_ch_vol.e_ls)
            {
                case AUD_DEC_LS_FRONT_LEFT:
                    prChannelVol->u.t_ch_vol.ui1_level = _arChVol[u1DecId].u1VolBypassL;
                    break;
                case AUD_DEC_LS_FRONT_RIGHT:
                    prChannelVol->u.t_ch_vol.ui1_level = _arChVol[u1DecId].u1VolBypassR;
                    break;
                default:
                    break;
            }
            break;
        }
    }
}
static void _GetMute(UINT8 u1DecId, AUD_DEC_MUTE_TYPE_T * peMuteType)
{
    *peMuteType = _aeMuteType[u1DecId];
}
static void _GetSpdif(UINT8 u1DecId, AUD_DEC_SPDIF_TYPE_T * peSpdif)
{
    *peSpdif = _aeSpdif[u1DecId];
}
static void _GetHdcdfilter(UINT8 u1DecId, AUD_DEC_HDCD_FLTR_T * peHdcdFilter)
{
    *peHdcdFilter = _aeHdcdFilter[u1DecId];
}
static void _GetLsMode(UINT8 u1DecId, AUD_DEC_CH_LS_MODE_T * prLsMode)
{
    AUD_DEC_LS_T eLsChannel;
    eLsChannel = prLsMode->e_ls;
    prLsMode->e_mode = _aeLsMode[u1DecId][eLsChannel];
}
static void _GetSoundEffect(UINT8 u1DecId, AUD_DEC_SE_INFO_T * prSoundEffect)
{
    prSoundEffect->e_se_type = _arSeInfo[u1DecId].e_se_type;
    prSoundEffect->pv_info = _arSeInfo[u1DecId].pv_info;
}
static void _GetDownMix(UINT8 u1DecId, AUD_DEC_DOWNMIX_MODE_T * peDownmixMode)
{
    *peDownmixMode = _aeDownmixMode[u1DecId];
}
static void _GetDrc(UINT8 u1DecId, AUD_DEC_DRC_T * peDrc)
{
    *peDrc = _aeDrc[u1DecId];
}

static void _GetOutPort(UINT8 u1DecId, UINT32 * pu4OutPort)
{
    *pu4OutPort = _au4OutPort[u1DecId];
}

static void _GetCompressionMode(UINT8 u1DecId, AUD_DEC_CMPSS_MODE_T * peCmprssMode)
{
    *peCmprssMode = _aeCmprssMode[u1DecId];
}

static void _GetAgcFlag(UINT8 u1DecId, BOOL * pfgAgcFlag)
{
    *pfgAgcFlag = _afgAgcFlag[u1DecId];
}

static void _GetChlDelay(UINT8 u1DecId, AUD_DEC_CHNL_DELAY_T * prChlDelay)
{
    VERIFY((prChlDelay->e_out_port) < (AUD_DEC_OUT_PORT_SPEAKER + 1));

    if (prChlDelay->e_ch_delay_type == AUD_DEC_CHNL_DELAY_TYPE_ALL)
    {
        prChlDelay->u.ui2_delay = _aau2MasterDelay[u1DecId][prChlDelay->e_out_port];
    }
    else
    {
        switch (prChlDelay->e_out_port)
        {
            case AUD_DEC_OUT_PORT_OFF:
                x_memcpy((VOID *)&(prChlDelay->u.t_ind_delay), (const VOID *)&_arChlDelayInd[u1DecId][AUD_DEC_OUT_PORT_OFF],
                                 sizeof(AUD_DEC_CHNL_DELAY_IND_T));
                break;
            case AUD_DEC_OUT_PORT_2_CH:
                x_memcpy((VOID *)&(prChlDelay->u.t_ind_delay), (const VOID *)&_arChlDelayInd[u1DecId][AUD_DEC_OUT_PORT_2_CH],
                                 sizeof(AUD_DEC_CHNL_DELAY_IND_T));
                break;
            case AUD_DEC_OUT_PORT_5_1_CH:
                x_memcpy((VOID *)&(prChlDelay->u.t_ind_delay), (const VOID *)&_arChlDelayInd[u1DecId][AUD_DEC_OUT_PORT_5_1_CH],
                                 sizeof(AUD_DEC_CHNL_DELAY_IND_T));
                break;
            case AUD_DEC_OUT_PORT_SPDIF:
                x_memcpy((VOID *)&(prChlDelay->u.t_ind_delay), (const VOID *)&_arChlDelayInd[u1DecId][AUD_DEC_OUT_PORT_SPDIF],
                                 sizeof(AUD_DEC_CHNL_DELAY_IND_T));
                break;
            case AUD_DEC_OUT_PORT_2_CH_BY_PASS:
                x_memcpy((VOID *)&(prChlDelay->u.t_ind_delay), (const VOID *)&_arChlDelayInd[u1DecId][AUD_DEC_OUT_PORT_2_CH_BY_PASS],
                                 sizeof(AUD_DEC_CHNL_DELAY_IND_T));
                break;
            case AUD_DEC_OUT_PORT_SPEAKER:
                x_memcpy((VOID *)&(prChlDelay->u.t_ind_delay), (const VOID *)&_arChlDelayInd[u1DecId][AUD_DEC_OUT_PORT_SPEAKER],
                                 sizeof(AUD_DEC_CHNL_DELAY_IND_T));
                break;
        }
    }
}

static void _GetCopyProtect(UINT8 u1DecId, AUD_DEC_COPY_PROTECT_T * peCopyProtect)
{
    *peCopyProtect = _aeCopyProtect[u1DecId];
}

static void _GetMts(UINT8 u1DecId, AUD_DEC_MTS_T * peMts)
{
    AUD_DEC_MTS_T eMtsMode = AUD_DEC_MTS_UNKNOWN;
    AUD_SOUND_MODE_T eSoundMode;

    eSoundMode = AUD_DspGetSoundMode(u1DecId);

    switch(eSoundMode)
    {
        case AUD_SOUND_MODE_UNKNOWN:
            eMtsMode = AUD_DEC_MTS_UNKNOWN;
            break;
        case AUD_SOUND_MODE_MONO:
            eMtsMode = AUD_DEC_MTS_MONO;
            break;
        case AUD_SOUND_MODE_STEREO:
            eMtsMode = AUD_DEC_MTS_STERRO;
            break;
        case AUD_SOUND_MODE_SUB_LANG:
            eMtsMode = AUD_DEC_MTS_SUB_LANG;
            break;
        case AUD_SOUND_MODE_DUAL1:
            eMtsMode = AUD_DEC_MTS_DUAL1;
            break;
        case AUD_SOUND_MODE_DUAL2:
            eMtsMode = AUD_DEC_MTS_DUAL2;
            break;
        case AUD_SOUND_MODE_NICAM_MONO:
            eMtsMode = AUD_DEC_MTS_NICAM_MONO;
            break;
        case AUD_SOUND_MODE_NICAM_STEREO:
            eMtsMode = AUD_DEC_MTS_NICAM_STEREO;
            break;
        case AUD_SOUND_MODE_NICAM_DUAL1:
            eMtsMode = AUD_DEC_MTS_NICAM_DUAL1;
            break;
        case AUD_SOUND_MODE_NICAM_DUAL2:
            eMtsMode = AUD_DEC_MTS_NICAM_DUAL2;
            break;
        default:
            eMtsMode = AUD_DEC_MTS_UNKNOWN;
    }

    *peMts = eMtsMode;
}

static void _GetMaxDelay(UINT8 u1DecId, UINT8 * pu1Delay)
{
    UNUSED(u1DecId);
    *pu1Delay = AUD_MAX_DELAY_IN_MS;
}

static void _GetUserEqBandNum(UINT8 u1DecId, UINT8 * pu1BandNum)
{
    UNUSED(u1DecId);
    *pu1BandNum = AUD_DspGetUserEqBandNum();
}

static void _GetUserEqLevel(UINT8 u1DecId, AUD_DEC_SE_QA_USER_LVL_T * prEqLevel)
{
    UNUSED(u1DecId);

    VERIFY(prEqLevel->ui1_band_idx < CH_EQ_BAND_NUM);
    VERIFY(prEqLevel->e_eq_type < (AUD_DEC_EQ_BBE_AV + 1));

    prEqLevel->i1_level = _ai1UserEqCfg[prEqLevel->e_eq_type][prEqLevel->ui1_band_idx];

/*
    if (!AUD_DspGetUserDefineEqCfg(prEqLevel->ui1_band_idx, &prEqLevel->i1_level))
    {
        LOG(3, "Error EQ number\n");
    }
*/
}

static void _GetUserEqInfo(UINT8 u1DecId, AUD_DEC_SE_QA_USER_INFO_BAND_T * prEqInfo)
{
    UNUSED(u1DecId);

    if (prEqInfo != NULL)
    {
        AUD_DspGetEqBandInfo((AUD_EQ_BAND_INFO_T *)prEqInfo);
    }
}

static void _GetAudInfo(UINT8 u1DecId, AUD_DEC_AUD_INFO_T * prAudInfo)
{
    AUD_DEC_STREAM_FROM_T  rAudDecStreamFrom;
    AUD_DEC_FMT_INFO_T     rAudDecFormat;
    UINT8  u1Acmode;
    UINT8  u1SmpRate;
    UINT16 u2DataRate = 0;

#ifndef CC_AUD_DUAL_DEC_SUPPORT
    // Get decoding format                            // FIXME !!
    AUD_DRVGetDecodeType(u1DecId, &rAudDecStreamFrom, (AUD_FMT_T *)&(rAudDecFormat.e_fmt));
    prAudInfo->e_aud_fmt = rAudDecFormat.e_fmt;
    UNUSED(rAudDecStreamFrom);
#else
    if (AUD_GetDualDecStrSrc() == AUD_STREAM_FROM_ANALOG_TUNER)
    {
        AUD_DRVGetDecodeType(AUD_DEC_AUX, &rAudDecStreamFrom, (AUD_FMT_T *)&(rAudDecFormat.e_fmt));
        if (rAudDecFormat.e_fmt >= AUD_DEC_FMT_TV_SYS)
        {
            rAudDecFormat.e_fmt = AUD_DEC_FMT_TV_SYS;
        }
        prAudInfo->e_aud_fmt = rAudDecFormat.e_fmt;
    }
    else
    {
        AUD_DRVGetDecodeType(u1DecId, &rAudDecStreamFrom, (AUD_FMT_T *)&(rAudDecFormat.e_fmt));
        prAudInfo->e_aud_fmt = rAudDecFormat.e_fmt;
        UNUSED(rAudDecStreamFrom);
    }
#endif

    switch (rAudDecFormat.e_fmt)
    {
    case AUD_DEC_FMT_TV_SYS:
        rAudDecFormat.e_fmt = AUD_DEC_FMT_TV_SYS;
        prAudInfo->ui4_sample_rate = 32000;
        prAudInfo->e_aud_type = (AUD_DEC_AUD_TYPE_T)DSP_GetAudChlInfo(AUD_DEC_AUX); // FIXME ! Hardcode ID
        break;
    case AUD_DEC_FMT_MTS: //ATV
        prAudInfo->ui4_sample_rate = 32000;
        prAudInfo->e_aud_type = (AUD_DEC_AUD_TYPE_T)DSP_GetMtsSrcTyp(AUD_DEC_MAIN); // FIXME ! Hardcode ID
        break;
    case AUD_DEC_FMT_MPEG:
        prAudInfo->ui4_sample_rate = 48000;
        prAudInfo->e_aud_type = AUD_DEC_TYPE_STEREO;
        break;
    case AUD_DEC_FMT_PCM:
        prAudInfo->ui4_sample_rate = 44100;
        prAudInfo->e_aud_type = AUD_DEC_TYPE_STEREO;
        break;
    case AUD_DEC_FMT_AC3: // DTV
        // Get audio information
        AUD_GetStreamInfo(u1DecId, &u1Acmode, &u1SmpRate, &u2DataRate);
        switch (u1Acmode)
        {
        case 0: // 1+1
            prAudInfo->e_aud_type = AUD_DEC_TYPE_DUAL_MONO;
            break;
        case 1: // 1/0
            prAudInfo->e_aud_type = AUD_DEC_TYPE_MONO;
            break;
        case 2: // 2/0
            prAudInfo->e_aud_type = AUD_DEC_TYPE_STEREO;
            break;
        case 3: // 3/0
            prAudInfo->e_aud_type = AUD_DEC_TYPE_3_0;
            break;
        case 4: // 2/1
            prAudInfo->e_aud_type = AUD_DEC_TYPE_SURROUND_2CH;
            break;
        case 5: // 3/1
            prAudInfo->e_aud_type = AUD_DEC_TYPE_SURROUND;
            break;
        case 6: // 2/2
            prAudInfo->e_aud_type = AUD_DEC_TYPE_4_0;
            break;
        case 7: // 3/2
            prAudInfo->e_aud_type = AUD_DEC_TYPE_5_1;
            break;
        case 8: // unknown
            prAudInfo->e_aud_type = AUD_DEC_TYPE_UNKNOWN;
            break;
        default: //
            LOG(5, "%s,", "Error acmode");
            prAudInfo->e_aud_type = AUD_DEC_TYPE_OTHERS;
            break;
        }

        switch (u1SmpRate)
        {
        case 0: // 48 KHz
            prAudInfo->ui4_sample_rate = 48000;
          break;
        case 1: // 44.1 KHz
            prAudInfo->ui4_sample_rate = 44100;
            break;
        case 2: // 32 KHz
            prAudInfo->ui4_sample_rate = 32000;
            break;
        default: // Error sample rate
            prAudInfo->ui4_sample_rate = 48000;
            LOG(5, "Unknown sample rate");
            break;
        }
        break;
    default:
        u1SmpRate = 0;
        break;
    }
    //Printf("%d Kbps,",u2DataRate);
    prAudInfo->ui4_data_rate = (UINT32)(u2DataRate * 1000);

    // FIXME // TODO
    prAudInfo->ui1_bit_depth = 24;
}

/*
static void _GetMemBuffer(UINT8 u1DecId, AUD_DEC_MEM_BUFFER_INFO_T * prMemBuf)
{
    x_memcpy(prMemBuf, &_arMemBuf[u1DecId], sizeof(AUD_DEC_MEM_BUFFER_INFO_T));
}
*/

static void _GetTvSysMask(UINT8 u1DecId, UINT32* pu4Mask)
{
    AUD_DEC_TV_AUD_SYS_T rAudTvSys;
    AUD_DspGetAtvTvSys((MW_TV_AUD_SYS_T *)(&rAudTvSys));
    *pu4Mask = rAudTvSys.ui4_tv_sys_mask;
}

static void _GetTvAudSysMask(UINT8 u1DecId, AUD_DEC_TV_AUD_SYS_T* parMask)
{
    UNUSED(u1DecId);
    AUD_DspGetAtvTvSys((MW_TV_AUD_SYS_T *)parMask);
}

static INT32 _GetTvSysCapability(UINT8 u1DecId, UINT32* pu4Capability)
{
    UNUSED(u1DecId);
    // FIXME !!
    if (IS_TUNER_MULTI_SYS())
    {
        *pu4Capability = TV_SYS_MASK_A        |
                         TV_SYS_MASK_B        |
                         TV_SYS_MASK_C        |
                         TV_SYS_MASK_D        |
                         TV_SYS_MASK_E        |
                         TV_SYS_MASK_F        |
                         TV_SYS_MASK_G        |
                         TV_SYS_MASK_H        |
                         TV_SYS_MASK_I        |
                         TV_SYS_MASK_J        |
                         TV_SYS_MASK_K        |
                         TV_SYS_MASK_K_PRIME  |
                         TV_SYS_MASK_L        |
                         TV_SYS_MASK_L_PRIME  |
                         TV_SYS_MASK_N ;
    }
    else // IS_TUNER_UNI_SYS()
    {
        *pu4Capability = TV_SYS_MASK_M;
    }

    return TRUE;
}

static void _GetAudClip(UINT8 u1DecId, AUD_DEC_AUD_CLIP_INFO_T* parAudClip)
{
    *parAudClip = _arAudClipInfo[u1DecId];
}

static void _GetPrologic2(UINT8 u1DecId, AUD_DEC_PL2_CTRL_INFO_T* prPL2)
{
    x_memcpy(prPL2, &_rPrologic2Type, sizeof(AUD_DEC_PL2_CTRL_INFO_T));
    if ((prPL2->e_ctrl == AUD_DEC_PL2_CTRL_DIMENSION) ||
        (prPL2->e_ctrl == AUD_DEC_PL2_CTRL_C_WIDTH))
    {
        prPL2->u.t_pl2_val.ui1_min_val = 0;
        prPL2->u.t_pl2_val.ui1_max_val = 0xf;
    }
}

static INT32 _GetCapability(UINT8 u1DecId, AUD_DEC_CAPABILITY_INFO_T * prCapability)
{
    prCapability->ui4_fmt = AUD_FMT_CAPABILITY;

    if (u1DecId == AUD_DEC_MAIN)
    {

        prCapability->ui4_channel_type = (AUD_DEC_CAP_TYPE_MONO      |
                                          AUD_DEC_CAP_TYPE_DUAL_MONO |
                                          AUD_DEC_CAP_TYPE_STEREO    |
                                          AUD_DEC_CAP_TYPE_5_1 );
    }
    else
    {
        prCapability->ui4_channel_type = (AUD_DEC_CAP_TYPE_MONO      |
                                          AUD_DEC_CAP_TYPE_DUAL_MONO |
                                          AUD_DEC_CAP_TYPE_STEREO );
    }

    prCapability->ui4_sound_effect = (AUD_DEC_CAP_SE_TRIM        |
                                      AUD_DEC_CAP_SE_DELAY       |
                                      AUD_DEC_CAP_SE_SUPERBASS   |
                                      AUD_DEC_CAP_SE_EQUALIZER   |
                                      AUD_DEC_CAP_SE_REVERB      |
                                      AUD_DEC_CAP_SE_BASS        |
                                      AUD_DEC_CAP_SE_POSTDRC     |
                                      AUD_DEC_CAP_SE_VOLUME);

    prCapability->ui4_downmix_mode = (AUD_DEC_CAP_DM_OFF         |
                                      AUD_DEC_CAP_DM_LT_RT       |
                                      AUD_DEC_CAP_DM_STEREO      |
                                      AUD_DEC_CAP_DM_VIR_SURR);

#ifdef CC_AUD_LINE_OUT_ADJ
    prCapability->b_is_outport_adj =  TRUE;
#else
    prCapability->b_is_outport_adj =  FALSE;
#endif

    return RMR_OK;
}

static void _GetDolbyBanner(UINT8 u1DecId, BOOL * pfgMode)
{
    *pfgMode = _afgDolbyBanner[u1DecId];
}

static void _GetOutPortAdj(UINT8 u1DecId, AUD_DEC_OUTPORT_ADJ_T * prOutportAdj)
{
#ifdef CC_AUD_LINE_OUT_ADJ
    switch (prOutportAdj->e_out_port)
    {
        case AUD_DEC_OUT_PORT_2_CH:
        case AUD_DEC_OUT_PORT_SPDIF:
            prOutportAdj->b_is_fixed = FALSE;
            break;
        case AUD_DEC_OUT_PORT_5_1_CH:
        case AUD_DEC_OUT_PORT_SPEAKER:
            prOutportAdj->b_is_fixed = TRUE;
            break;
        default:
            prOutportAdj->b_is_fixed = TRUE;
    }
#else
    prOutportAdj->b_is_fixed = TRUE;
#endif
}

static INT32 _GetAvSyncMode(UINT8 u1DecId, AUD_DEC_AV_SYNC_INFO_T *eSyncMode)
{
    AV_SYNC_MODE_T eMode;

    eMode = AUD_GetAvSynMode(u1DecId);

    if (eMode == AV_SYNC_SLAVE)
    {
        *eSyncMode = AUD_DEC_AV_SYNC_AUD_SLAVE;
    }
    else if (eMode == AV_SYNC_FREE_RUN)
    {
        *eSyncMode = AUD_DEC_AV_SYNC_FREE_RUN;
    }
    else if (eMode == AV_SYNC_AUDIO_MASTER)
    {
        *eSyncMode = AUD_DEC_AV_SYNC_AUD_MASTER;
    }
    else
    {
        return RMR_DRV_INV_SET_INFO;
    }

    return RMR_OK;
}

static void _GetBbeMode(UINT8 u1DecId, AUD_DEC_BBE_MODE_T * eBbeMode)
{
    *eBbeMode = _aeBbeMode[u1DecId];
}

static void _GetHeadphonePCond(UINT8 u1DecId, AUD_DEC_HEADPHONE_COND_T * eHPCond)
{
    //*eHPCond = AUD_GetHeadphoneCond(u1DecId);
    *eHPCond = AUD_DEC_HEADPHONE_COND_UNPLUGED;  //always return UNPLUGED
}

static void _GetMuteEx(UINT8 u1DecId, AUD_DEC_MUTE_EX_INFO_T * eMuteEx)
{
    *eMuteEx = _arAudMuteEx[AUD_DEC_MAIN];
}

static INT32 _AudSetCtrl(UINT8 u1DecId, AUD_DEC_CTRL_T  eAudCtrl)
{
    if (eAudCtrl == AUD_DEC_CTRL_RESET)
        _aeCtrlSetting[u1DecId] = AUD_DEC_CTRL_STOP;
    else
        _aeCtrlSetting[u1DecId] = eAudCtrl;


    AUD_SetCtrlType((UINT32)eAudCtrl);
    switch(eAudCtrl)
    {
        case AUD_DEC_CTRL_RESET:
            AUD_DSPCmdResetAsyn(u1DecId);
            return RMR_ASYNC_NFY;

        case AUD_DEC_CTRL_STOP:
            if (_afgPlayAudClip[u1DecId])
            {
                _afgPlayAudClip[u1DecId] = FALSE;
                AUD_StopAudClip(u1DecId);
            }
            else if (_afgTvSysDetecting[u1DecId])
            {
                _afgTvSysDetecting[u1DecId] = FALSE;
                AUD_DSPCmdStopAsyn(u1DecId);
            }
            else
            {
#ifndef CC_AUD_DUAL_DEC_SUPPORT
                AUD_DSPCmdStopAsyn(u1DecId);
#else
                AUD_DSPCmdDualDecStop(u1DecId);
#endif
            }

            if (u1DecId == AUD_DEC_AUX)
            {
                _fgPlayAudDscr = FALSE;
            }
            return RMR_ASYNC_NFY;

        case AUD_DEC_CTRL_PLAY:
            // Workaround here to set _afgTvSysDetecting here
            _afgTvSysDetecting[u1DecId] = FALSE;
            if (u1DecId == AUD_DEC_AUX)
            {
                _fgPlayAudDscr = TRUE;
            }
                //AV_SYNC_MODE_T eSyncMode = AV_SYNC_SLAVE;
                //AUD_SetAvSynMode(u1DecId, eSyncMode);
#ifndef CC_AUD_DUAL_DEC_SUPPORT
                AUD_DSPCmdPlayAsyn(u1DecId);
#else
                AUD_DSPCmdDualDecPlay(u1DecId);
#endif
            return RMR_ASYNC_NFY;

        case AUD_DEC_CTRL_PLAY_SYNC:
            // Workaround here to set _afgTvSysDetecting here
            _afgTvSysDetecting[u1DecId] = FALSE;
            {
                AV_SYNC_MODE_T eSyncMode = AV_SYNC_SLAVE;
                AUD_SetAvSynMode(u1DecId, eSyncMode);
#ifndef CC_AUD_DUAL_DEC_SUPPORT
                AUD_DSPCmdPlayAsyn(u1DecId);
#else
                AUD_DSPCmdDualDecPlay(u1DecId);
#endif
            }
            return RMR_ASYNC_NFY;

        case AUD_DEC_CTRL_PLAY_AUD_CLIP:
            if (_arAudClip[u1DecId].u4OutputChlSelect != AUD_DEV_LS_FLAG_NONE)
            {
                _afgPlayAudClip[u1DecId] = TRUE;
                if (AUD_IsDecoderPlay(u1DecId))
                {
                    return RMR_DRV_INV_SET_INFO;
                }
                AUD_PlayAudClip(u1DecId,
                                _arAudClip[u1DecId].u4OutputChlSelect,
                                _arAudClip[u1DecId].fgPlayTestTone);
            }
            return RMR_ASYNC_NFY;

        case AUD_DEC_CTRL_FLUSH:
            AUD_DSPCmdStopAsyn(u1DecId);
            return RMR_ASYNC_NFY;

        case AUD_DEC_CTRL_PAUSE:
            //AUD_DSPCmdPauseAsyn(u1DecId);
            AUD_DSPCmdPause(u1DecId);
            return RMR_ASYNC_NFY;

        case AUD_DEC_CTRL_RESUME:
            AUD_DSPCmdResume(u1DecId);
            return RMR_ASYNC_NFY;

        case AUD_DEC_CTRL_DETECT_TV_AUD_SYS:
            // Check audio decoder format at first
            //if (_arAudDriverDecFmt[u1DecId] != AUD_FMT_DETECTOR)
            _afgTvSysDetecting[u1DecId] = TRUE;
            AUD_DSPCmdStop(AUD_DEC_AUX);
            {
                AUD_SetDecType(u1DecId, AUD_STREAM_FROM_ANALOG_TUNER, AUD_FMT_DETECTOR);
                _arAudDriverDecFmt[u1DecId] = AUD_FMT_DETECTOR;
                _AudSetDetTvSrcMode(u1DecId);
            }
            UNUSED(_arAudDriverDecFmt);
            // Invoke audio playback command
            AUD_DSPCmdPlayAsyn(u1DecId);
            return RMR_ASYNC_NFY;

        default:
            LOG( 1, "SetCtrl Error\n");
            return RMR_DRV_INV_SET_INFO;
    }

 //   return RMR_OK;
}

static void _AudSetNotifyInfo(UINT8 u1DecId, const AUD_DEC_NFY_INFO_T * prNtyInfo)
{
    _arAudMwNotifyInfo[u1DecId].pv_tag = prNtyInfo->pv_tag;
    _arAudMwNotifyInfo[u1DecId].pf_aud_dec_nfy = prNtyInfo->pf_aud_dec_nfy;
}

static void _SetFormat(UINT8 u1DecId, const AUD_DEC_FMT_INFO_T * prFormatInfo)
{
    AUD_DEC_STREAM_FROM_T eStreamFrom;

    _arAudDecFmt[u1DecId].e_fmt = prFormatInfo->e_fmt;
    _arAudDecFmt[u1DecId].pv_info = prFormatInfo->pv_info;

    eStreamFrom = _aeStrSrc[u1DecId];

    // Check if EURO CANAL+ mode
    if ((_au2MwConnectComp[u1DecId]  == DRVT_AVC_SCART) &&
        (prFormatInfo->e_fmt == AUD_DEC_FMT_EU_CANAL_PLUS))
    {
        DSP_SetEuroCanalPlusMode(TRUE);
    }
    else
    {
        DSP_SetEuroCanalPlusMode(FALSE);
    }

    // Setup format ..
#ifndef CC_AUD_DVBT_SUPPORT
    // US model, load MTS code if stream source is ATV ..
    if ((eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER) ||
        ((_au2MwConnectComp[u1DecId]  == DRVT_AVC_SCART) && (prFormatInfo->e_fmt == AUD_DEC_FMT_TV_SYS)))
    {
    	_aeStrSrc[u1DecId] = AUD_STREAM_FROM_ANALOG_TUNER;
	    eStreamFrom = AUD_STREAM_FROM_ANALOG_TUNER;
        AUD_SetDecType(u1DecId, eStreamFrom, AUD_FMT_MTS);
    }
#else  // EU model ...
    if ((_au2MwConnectComp[u1DecId]  == DRVT_TUNER_SAT_ANA) ||
        (_au2MwConnectComp[u1DecId]  == DRVT_TUNER_CAB_ANA) ||
        (_au2MwConnectComp[u1DecId]  == DRVT_TUNER_TER_ANA) ||
        ((_au2MwConnectComp[u1DecId]  == DRVT_AVC_SCART) && (prFormatInfo->e_fmt == AUD_DEC_FMT_TV_SYS)))
    {
    	_aeStrSrc[u1DecId] = AUD_STREAM_FROM_ANALOG_TUNER;
	    eStreamFrom = AUD_STREAM_FROM_ANALOG_TUNER;

        // To invoke detection or just load proper decoding code
        if (_IsRunDetection(u1DecId))
        {
            AUD_SetDecType(u1DecId, eStreamFrom, AUD_FMT_DETECTOR);
            _arAudDriverDecFmt[u1DecId] = AUD_FMT_DETECTOR;
        }
        else if (_IsMaskSetToMtsSys(u1DecId))
        {
#ifndef CC_AUD_DUAL_DEC_SUPPORT
            AUD_SetDecType(u1DecId, eStreamFrom, AUD_FMT_MTS);
#else
            AUD_DSPCmdDualDecSetDecType(eStreamFrom, AUD_FMT_MTS);
#endif
            _arAudDriverDecFmt[u1DecId] = AUD_FMT_MTS;
        }
        else if (_IsMaskSetToA2Sys(u1DecId))
        {
#ifndef CC_AUD_DUAL_DEC_SUPPORT
            AUD_SetDecType(u1DecId, eStreamFrom, AUD_FMT_A2);
#else
            AUD_DSPCmdDualDecSetDecType(eStreamFrom, AUD_FMT_A2);
#endif
            AUD_DspSetTvSrcMode(_GetTvSysFormat(u1DecId));
            _arAudDriverDecFmt[u1DecId] = AUD_FMT_A2;
        }
        else if (_IsMaskSetToPalSys(u1DecId))
        {
#ifndef CC_AUD_DUAL_DEC_SUPPORT
            AUD_SetDecType(u1DecId, eStreamFrom, AUD_FMT_PAL);
#else
            AUD_DSPCmdDualDecSetDecType(eStreamFrom, AUD_FMT_PAL);
#endif
            AUD_DspSetTvSrcMode(_GetTvSysFormat(u1DecId));
            _arAudDriverDecFmt[u1DecId] = AUD_FMT_PAL;
        }
        else if (_IsMaskSetToFmfmSys(u1DecId))
        {
#ifndef CC_AUD_DUAL_DEC_SUPPORT
            AUD_SetDecType(u1DecId, eStreamFrom, AUD_FMT_FMFM);
#else
            AUD_DSPCmdDualDecSetDecType(eStreamFrom, AUD_FMT_FMFM);
#endif
            AUD_DspSetTvSrcMode(SV_FM_FM);
            _arAudDriverDecFmt[u1DecId] = AUD_FMT_FMFM;
        }
        else
        {
#ifndef CC_AUD_DUAL_DEC_SUPPORT
            AUD_SetDecType(u1DecId, eStreamFrom, AUD_FMT_MTS);
#else
            AUD_DSPCmdDualDecSetDecType(eStreamFrom, AUD_FMT_MTS);
#endif
            _arAudDriverDecFmt[u1DecId] = AUD_FMT_MTS;
            LOG(1, "Improper audio decoder format\n");
        }
    }
#endif // #ifndef CC_AUD_DVBT_SUPPORT

    else if (eStreamFrom == AUD_STREAM_FROM_LINE_IN)
    {
#ifndef CC_AUD_DUAL_DEC_SUPPORT
        AUD_SetDecType(u1DecId, eStreamFrom, AUD_FMT_PCM);
#else
        AUD_DSPCmdDualDecSetDecType(eStreamFrom, AUD_FMT_PCM);
#endif
        _arAudDriverDecFmt[u1DecId] = AUD_FMT_PCM;
    }
    else if (eStreamFrom == AUD_STREAM_FROM_HDMI)
    {
#ifndef CC_AUD_DUAL_DEC_SUPPORT
        AUD_SetDecType(u1DecId, eStreamFrom, AUD_FMT_PCM);
#else
        AUD_DSPCmdDualDecSetDecType(eStreamFrom, AUD_FMT_PCM);
#endif
        _arAudDriverDecFmt[u1DecId] = AUD_FMT_PCM;
    }
    else if (((_au2MwConnectComp[u1DecId]  == DRVT_AVC_SCART) && (prFormatInfo->e_fmt == AUD_DEC_FMT_PCM)) ||
             ((_au2MwConnectComp[u1DecId]  == DRVT_AVC_SCART) && (prFormatInfo->e_fmt == AUD_DEC_FMT_EU_CANAL_PLUS)))
    {
#ifndef CC_AUD_DUAL_DEC_SUPPORT
        AUD_SetDecType(u1DecId, AUD_STREAM_FROM_LINE_IN, AUD_FMT_PCM);
#else
        AUD_DSPCmdDualDecSetDecType(AUD_STREAM_FROM_LINE_IN, AUD_FMT_PCM);
#endif
        _arAudDriverDecFmt[u1DecId] = AUD_FMT_PCM;
    }
    else if ((_au2MwConnectComp[u1DecId]  == DRVT_AVC_SPDIF))
    {
#ifndef CC_AUD_DUAL_DEC_SUPPORT
        AUD_SetDecType(u1DecId, eStreamFrom, AUD_FMT_AC3);
#else
        AUD_DSPCmdDualDecSetDecType(eStreamFrom, AUD_FMT_AC3);
#endif
        _arAudDriverDecFmt[u1DecId] = AUD_FMT_AC3;
    }
    else
    {
#ifndef CC_AUD_DUAL_DEC_SUPPORT
        AUD_SetDecType(u1DecId, eStreamFrom, (AUD_FMT_T)prFormatInfo->e_fmt);
#else
        AUD_DSPCmdDualDecSetDecType(eStreamFrom, (AUD_FMT_T)prFormatInfo->e_fmt);
        //Audio Description
        if ((u1DecId == AUD_DEC_AUX)&&(eStreamFrom == AUD_DEC_FMT_MPEG))
        {
            AUD_SetDecType(u1DecId, eStreamFrom, (AUD_FMT_T)prFormatInfo->e_fmt);
        }
#endif
        _arAudDriverDecFmt[u1DecId] = (AUD_FMT_T)prFormatInfo->e_fmt;
    }
}

static void _SetVolume(UINT8 u1DecId, AUD_DEC_VOLUME_INFO_T * prChlVol)
{
    AUD_CH_T eAudCh = AUD_CH_ALL;

    if (prChlVol->e_vol_type == AUD_DEC_ALL_CH)
    {
        UINT8 u1Volume;

        _arChVol[u1DecId].u1VolMaster = prChlVol->u.ui1_level;
        u1Volume = prChlVol->u.ui1_level;
        eAudCh = AUD_CH_ALL;
        AUD_DspMasterVolume(u1DecId, u1Volume);
    }
    else if (prChlVol->e_vol_type == AUD_DEC_INDIVIDUAL_CH)
    {
        switch (prChlVol->u.t_ch_vol.e_out_port)
        {
        case AUD_DEC_OUT_PORT_2_CH:       // This is for dmx channel
        case AUD_DEC_OUT_PORT_SPEAKER: // doing the same thing with "AUD_DEC_OUT_PORT_2_CH"
            switch (prChlVol->u.t_ch_vol.e_ls)
            {
                case AUD_DEC_LS_FRONT_LEFT:
                    _arChVol[u1DecId].u1VolDmxL = prChlVol->u.t_ch_vol.ui1_level;
                    eAudCh = AUD_CH_DMX_LEFT;
                    break;
                case AUD_DEC_LS_FRONT_RIGHT:
                    _arChVol[u1DecId].u1VolDmxR = prChlVol->u.t_ch_vol.ui1_level;
                    eAudCh = AUD_CH_DMX_RIGHT;
                    break;
                default:
                    break;
            }
            break;
        case AUD_DEC_OUT_PORT_5_1_CH:
            switch (prChlVol->u.t_ch_vol.e_ls)
            {
                case AUD_DEC_LS_FRONT_LEFT:
                    _arChVol[u1DecId].u1VolL = prChlVol->u.t_ch_vol.ui1_level;
                    eAudCh = AUD_CH_FRONT_LEFT;
                    break;
                case AUD_DEC_LS_FRONT_RIGHT:
                    _arChVol[u1DecId].u1VolR = prChlVol->u.t_ch_vol.ui1_level;
                    eAudCh = AUD_CH_FRONT_RIGHT;
                    break;
                case AUD_DEC_LS_REAR_LEFT:
                    _arChVol[u1DecId].u1VolSL = prChlVol->u.t_ch_vol.ui1_level;
                    eAudCh = AUD_CH_REAR_LEFT;
                    break;
                case AUD_DEC_LS_REAR_RIGHT:
                    _arChVol[u1DecId].u1VolSR = prChlVol->u.t_ch_vol.ui1_level;
                    eAudCh = AUD_CH_REAR_RIGHT;
                    break;
                case AUD_DEC_LS_CENTER:
                    _arChVol[u1DecId].u1VolC = prChlVol->u.t_ch_vol.ui1_level;
                    eAudCh = AUD_CH_CENTER;
                    break;
                case AUD_DEC_LS_SUB_WOOFER:
                    _arChVol[u1DecId].u1VolSW = prChlVol->u.t_ch_vol.ui1_level;
                    eAudCh = AUD_CH_SUB_WOOFER;
                    break;
                default:
                    break;
            }
            break;
        case AUD_DEC_OUT_PORT_SPDIF:
            // Do nothing
            break;
        case AUD_DEC_OUT_PORT_2_CH_BY_PASS:
            switch (prChlVol->u.t_ch_vol.e_ls)
            {
                case AUD_DEC_LS_FRONT_LEFT:
                    _arChVol[u1DecId].u1VolBypassL = prChlVol->u.t_ch_vol.ui1_level;
                    eAudCh = AUD_CH_BYPASS_LEFT;
                    break;
                case AUD_DEC_LS_FRONT_RIGHT:
                    _arChVol[u1DecId].u1VolBypassR = prChlVol->u.t_ch_vol.ui1_level;
                    eAudCh = AUD_CH_BYPASS_RIGHT;
                    break;
                default:
                    break;
            }
            break;

        }
        AUD_DspChannelVolume(u1DecId, eAudCh, prChlVol->u.t_ch_vol.ui1_level);
    }
}
static void _SetMute(UINT8 u1DecId, AUD_DEC_MUTE_TYPE_T eMuteType)
{
    BOOL fgSpkOff = FALSE;

    _aeMuteType[u1DecId] = eMuteType;

    fgSpkOff = (_arAudMuteEx[u1DecId].ui4_muted_out_port_msk & AUD_DEC_OUT_PORT_FLAG_SPEAKER);
    fgSpkOff = (fgSpkOff | (!_afgSpeakerPortOn[u1DecId]));

    switch (eMuteType)
    {
        case AUD_DEC_MUTE_OFF:
            _afgMute[u1DecId] = FALSE;

            if (!fgSpkOff)
            {
                ADAC_SpeakerEnable(TRUE);
            }
            x_thread_delay(50);
            AUD_UnMute(u1DecId);
            break;

        case AUD_DEC_MUTE_ON:
            _afgMute[u1DecId] = TRUE;
            AUD_Mute(u1DecId);
            x_thread_delay(50);
            ADAC_SpeakerEnable(FALSE);
            break;

        default:
            break;
    }
}

static void _AudSetMuteEx(UINT8 u1DecId, AUD_DEC_MUTE_EX_INFO_T * eMuteType)
{
    UINT32 u4MuteMask = 0;
    BOOL   fgSpkOff = FALSE;

    u4MuteMask = eMuteType->ui4_muted_out_port_msk;
    _arAudMuteEx[u1DecId].ui4_muted_out_port_msk = u4MuteMask;

    fgSpkOff =  ((!_afgSpeakerPortOn[u1DecId]) | _afgMute[u1DecId]);

    if (u4MuteMask & AUD_DEC_OUT_PORT_FLAG_SPEAKER)
    {
        ADAC_SpeakerEnable(FALSE);      //Mute Speaker
    }
    else
    {
        if (!fgSpkOff)
        {
            ADAC_SpeakerEnable(TRUE);       //Unmute Speaker
        }
    }
}

static void _SetSpdif(UINT8 u1DecId, AUD_DEC_SPDIF_TYPE_T eSpdif)
{
    _aeSpdif[u1DecId] = eSpdif;
    switch(eSpdif)
    {
        case AUD_DEC_SPDIF_OFF:
            AUD_DspIECConfig(u1DecId, AUD_IEC_CFG_PCM, FALSE);
            break;
        case AUD_DEC_SPDIF_RAW:
            AUD_DspIECConfig(u1DecId, AUD_IEC_CFG_RAW, TRUE);
            break;
        case AUD_DEC_SPDIF_PCM_16:
            AUD_DspIECConfig(u1DecId, AUD_IEC_CFG_PCM, TRUE);
            break;
        case AUD_DEC_SPDIF_PCM_24:
            AUD_DspIECConfig(u1DecId, AUD_IEC_CFG_PCM, TRUE);
            break;
        default:
            break;
    }
}
static void _SetHdcdfilter(UINT8 u1DecId, AUD_DEC_HDCD_FLTR_T eHdcdFilter)
{
    _aeHdcdFilter[u1DecId] = eHdcdFilter;
}

static void _SetLsMode(UINT8 u1DecId, AUD_DEC_CH_LS_MODE_T * prChLsMode)
{
    UINT8 u1LMode;
    UINT8 u1RMode;
    UINT8 u1LsMode;
    UINT8 u1RsMode;
    UINT8 u1CMode;

    _aeLsMode[u1DecId][prChLsMode->e_ls] = prChLsMode->e_mode;

    u1LMode = (_aeLsMode[u1DecId][AUD_DEC_LS_FRONT_LEFT] == AUD_DEC_LS_MODE_SMALL_FREQ_RANGE) ? 0 : 1;
    u1RMode = (_aeLsMode[u1DecId][AUD_DEC_LS_FRONT_RIGHT] == AUD_DEC_LS_MODE_SMALL_FREQ_RANGE) ? 0 : 1;
    u1LsMode = (_aeLsMode[u1DecId][AUD_DEC_LS_REAR_LEFT] == AUD_DEC_LS_MODE_SMALL_FREQ_RANGE) ? 0 : 1;
    u1RsMode = (_aeLsMode[u1DecId][AUD_DEC_LS_REAR_RIGHT] == AUD_DEC_LS_MODE_SMALL_FREQ_RANGE) ? 0 : 1;
    u1CMode = (_aeLsMode[u1DecId][AUD_DEC_LS_CENTER] == AUD_DEC_LS_MODE_SMALL_FREQ_RANGE) ? 0 : 1;

    AUD_DspSpeakerLsConfig(u1DecId, u1LMode, u1RMode, u1LsMode, u1RsMode, u1CMode);
}

static void _SetChEqLoadPresetChgMode(UINT8 u1DecId, AUD_DEC_EQ_TYPE_T eEqType)
{
    AUD_EQ_TYPE_T eEqTypeDrv = AUD_EQ_OFF;

    if (eEqType == AUD_DEC_EQ_OFF)
    {
        eEqTypeDrv = AUD_EQ_OFF;
    }
    else if (eEqType == AUD_DEC_EQ_ROCK)
    {
        eEqTypeDrv = AUD_EQ_ROCK;
    }
    else if (eEqType == AUD_DEC_EQ_POP)
    {
        eEqTypeDrv = AUD_EQ_POP;
    }
    else if (eEqType == AUD_DEC_EQ_LIVE)
    {
        eEqTypeDrv = AUD_EQ_LIVE;
    }
    else if (eEqType == AUD_DEC_EQ_DANCE)
    {
        eEqTypeDrv = AUD_EQ_DANCE;
    }
    else if (eEqType == AUD_DEC_EQ_TECHNO)
    {
        eEqTypeDrv = AUD_EQ_TECHNO;
    }
    else if (eEqType == AUD_DEC_EQ_CLASSIC)
    {
        eEqTypeDrv = AUD_EQ_CLASSIC;
    }
    else if (eEqType == AUD_DEC_EQ_SOFT)
    {
        eEqTypeDrv = AUD_EQ_SOFT;
    }
    else if (eEqType == AUD_DEC_EQ_USER_1)
    {
        eEqTypeDrv = AUD_EQ_CUST1;
    }
    else if (eEqType == AUD_DEC_EQ_USER_2)
    {
        eEqTypeDrv = AUD_EQ_CUST2;
    }
    else if (eEqType == AUD_DEC_EQ_USER_3)
    {
        eEqTypeDrv = AUD_EQ_CUST3;
    }
    else if (eEqType == AUD_DEC_EQ_USER_4)
    {
        eEqTypeDrv = AUD_EQ_CUST4;
    }
    else if (eEqType == AUD_DEC_EQ_USER_5)
    {
        eEqTypeDrv = AUD_EQ_CUST5;
    }
    else if (eEqType == AUD_DEC_EQ_USER_6)
    {
        eEqTypeDrv = AUD_EQ_CUST6;
    }
    else if (eEqType == AUD_DEC_EQ_USER_7)
    {
        eEqTypeDrv = AUD_EQ_CUST7;
    }
    else if (eEqType == AUD_DEC_EQ_USER_8)
    {
        eEqTypeDrv = AUD_EQ_CUST8;
    }
    else if (eEqType == AUD_DEC_EQ_BBE_TV)
    {
        eEqTypeDrv = AUD_EQ_BBE_TV;
    }
    else if (eEqType == AUD_DEC_EQ_BBE_AV)
    {
        eEqTypeDrv = AUD_EQ_BBE_AV;
    }
    else if (eEqType == AUD_DEC_EQ_SBASS)
    {
        eEqTypeDrv = AUD_EQ_SBASS;
    }

    AUD_DspChEqLoadPresetChgMode(u1DecId, eEqTypeDrv);
}

static void _SetSoundEffect(UINT8 u1DecId, AUD_DEC_SE_INFO_T * prSeInfo)
{
    _arSeInfo[u1DecId].e_se_type =  prSeInfo->e_se_type;
    _arSeInfo[u1DecId].pv_info =  prSeInfo->pv_info;

    //FIXME

    switch(_arSeInfo[u1DecId].e_se_type)
    {
        case AUD_DEC_SE_TRIM:
            break;
        case AUD_DEC_SE_DELAY:
            break;
        case AUD_DEC_SE_SUPERBASS:
            AUD_DspSuperBassEnable(u1DecId, (BOOL)((UINT32)_arSeInfo[u1DecId].pv_info));
            break;
        case AUD_DEC_SE_EQUALIZER:
            _SetChEqLoadPresetChgMode(u1DecId, (AUD_DEC_EQ_TYPE_T)((UINT32)_arSeInfo[u1DecId].pv_info));
            break;
        case AUD_DEC_SE_REVERB:
            AUD_DspReverbMode(u1DecId, (AUD_DEC_REVERB_MODE_T)((UINT32)_arSeInfo[u1DecId].pv_info));
            break;
        case AUD_DEC_SE_BASS:
            AUD_DspBassBoostGain(u1DecId, GET_BASS_MAP_VAL((UINT8)((UINT32)_arSeInfo[u1DecId].pv_info)));
            break;
        case AUD_DEC_SE_TREBLE:
            AUD_DspClearBoostGain(u1DecId, GET_TREBLE_MAP_VAL((UINT8)((UINT32)_arSeInfo[u1DecId].pv_info)));
            break;
        case AUD_DEC_SE_BALANCE:
            AUD_DspBalance(u1DecId, (UINT8)((UINT32)_arSeInfo[u1DecId].pv_info));
            break;
        case AUD_DEC_SE_POSTDRC:
            break;
        case AUD_DEC_SE_VOLUME:
            break;
        case AUD_DEC_SE_SURROUND:
            AUD_DspVirtualSurroundEnalbe(u1DecId, (BOOL)((UINT32)_arSeInfo[u1DecId].pv_info));
            break;
        default:
            break;
    }
}

static void _SetDownMix(UINT8 u1DecId, AUD_DEC_DOWNMIX_MODE_T eDownmixMode)
{
    _aeDownmixMode[u1DecId] = eDownmixMode;
    switch(eDownmixMode)
    {
        case AUD_DEC_DM_OFF:
            AUD_DspSpeakerOutputConfig(u1DecId, 7); // 3/2
            //AUD_DspVirtualSurroundEnalbe(u1DecId, FALSE);
            AUD_DspMonoDownMix(u1DecId, FALSE);
            break;
        case AUD_DEC_DM_LT_RT:
            AUD_DspSpeakerOutputConfig(u1DecId, 0); // 2/0 LtRt
            //AUD_DspVirtualSurroundEnalbe(u1DecId, FALSE);
            AUD_DspMonoDownMix(u1DecId, FALSE);
            break;
        case AUD_DEC_DM_STEREO:
            AUD_DspSpeakerOutputConfig(u1DecId, 2); // 2/0 LtRt
            //AUD_DspVirtualSurroundEnalbe(u1DecId, FALSE);
            AUD_DspMonoDownMix(u1DecId, FALSE);
            break;
        case AUD_DEC_DM_VIR_SURR:
            AUD_DspSpeakerOutputConfig(u1DecId, 0); // 2/0 LtRt
            //AUD_DspVirtualSurroundEnalbe(u1DecId, TRUE);
            AUD_DspMonoDownMix(u1DecId, FALSE);
            break;
        case AUD_DEC_DM_MONO:
            AUD_DspSpeakerOutputConfig(u1DecId, 2); //
            //AUD_DspVirtualSurroundEnalbe(u1DecId, FALSE);
            AUD_DspMonoDownMix(u1DecId, TRUE);
            break;
        case AUD_DEC_DM_DUAL1:
            AUD_DspMonoDownMix(u1DecId, TRUE);
            AUD_DspSetKaraokeMixRatio(u1DecId, 0x0);
            break;
        case AUD_DEC_DM_DUAL2:
            AUD_DspMonoDownMix(u1DecId, TRUE);
            AUD_DspSetKaraokeMixRatio(u1DecId, 0x800000);
            break;
        default:
            break;
    }
}

static void _SetDrc(UINT8 u1DecId, AUD_DEC_DRC_T eDrc)
{
    _aeDrc[u1DecId] = eDrc;
    AUD_DspAC3DRCRange((UINT8)eDrc, u1DecId);
}

static void _SetOutPort(UINT8 u1DecId, UINT32 u4OutPort)
{
    _au4OutPort[u1DecId] = u4OutPort;
    AUD_IEC_T eSpdifOutFormat = AUD_IEC_CFG_PCM;
    BOOL fgSpkOff = FALSE;

    if (_aeSpdif[u1DecId] == AUD_DEC_SPDIF_RAW)
    {
        eSpdifOutFormat = AUD_IEC_CFG_RAW;
    }
    else if (_aeSpdif[u1DecId] == AUD_DEC_SPDIF_PCM_16)
    {
        eSpdifOutFormat = AUD_IEC_CFG_PCM;
    }
    else if (_aeSpdif[u1DecId] == AUD_DEC_SPDIF_PCM_24)
    {
        eSpdifOutFormat = AUD_IEC_CFG_PCM;
    }

    // Note that for hardware limitation in MT5351/MT5371,
    // AUD_DEC_OUT_PORT_FLAG_2_CH & AUD_DEC_OUT_PORT_FLAG_5_1_CH cannot open/close individually
    if ((u4OutPort & AUD_DEC_OUT_PORT_FLAG_2_CH) ||
        (u4OutPort & AUD_DEC_OUT_PORT_FLAG_5_1_CH))
    {
        _AUD_OutPadEnable(u1DecId, TRUE);
        ADAC_Enable(TRUE);
        ADAC_Mute(FALSE);
        ADAC_CodecMute(FALSE);
    }
    else
    {
        _AUD_OutPadEnable(u1DecId, FALSE);
        if (u1DecId == AUD_DEC_MAIN)
        {
            // For power off
            ADAC_GpioAmpEnable(FALSE);
            x_thread_delay(100);
            ADAC_CodecMute(TRUE);
            //x_thread_delay(100);
        }
    }

    if (u4OutPort & AUD_DEC_OUT_PORT_FLAG_SPDIF)
    {
        if (u1DecId == AUD_DEC_MAIN)
        {
            _AUD_SpdifOutPadEnable(u1DecId, TRUE);
            if (_aeSpdif[u1DecId] != AUD_DEC_SPDIF_OFF)
            {
                AUD_DspIECConfig(u1DecId, eSpdifOutFormat, TRUE);
            }
         }
    }
    else
    {
        if (u1DecId == AUD_DEC_MAIN)
        {
            _AUD_SpdifOutPadEnable(u1DecId, FALSE);
            AUD_DspIECConfig(u1DecId, eSpdifOutFormat, FALSE);
        }
    }

    if (u4OutPort & AUD_DEC_OUT_PORT_FLAG_SPEAKER)
    {
        fgSpkOff = ((_arAudMuteEx[u1DecId].ui4_muted_out_port_msk) & AUD_DEC_OUT_PORT_FLAG_SPEAKER);
        fgSpkOff = (fgSpkOff | _afgMute[u1DecId]);

		if (u1DecId == AUD_DEC_AUX)
        {
            AUD_AudioDescriptionChlSel(AUD_DEC_MAIN, AUD_CHL_DOWNMIX);
        }
		else
        {
            if (!fgSpkOff)
            {
                ADAC_SpeakerEnable(TRUE);
            }
        }
        _afgSpeakerPortOn[u1DecId] = TRUE;
    }
    else
    {
        _afgSpeakerPortOn[u1DecId] = FALSE;

        if (u1DecId == AUD_DEC_AUX) //decoder1 set AD on/off, decoder0 turn on/off speaker
        {
            AUD_AudioDescriptionChlSel(AUD_DEC_MAIN, AUD_CHL_L_R);
        }
        else
        {
            ADAC_SpeakerEnable(FALSE);
        }
    }
}

static INT32 _SetMemBuffer(UINT8 u1DecId, AUD_DEC_MEM_BUFFER_INFO_T * prMemBuf)
{
    MEM_BUFFER_INFO_T rMemBuf;

    rMemBuf.pData = prMemBuf->pui1_data;
    rMemBuf.u4Length = prMemBuf->z_len;
    rMemBuf.u4LoopCount = AUD_CLIP_REPEAT_CNT_FOREVER;

    x_memcpy(&_arMemBuf[u1DecId], prMemBuf, sizeof(AUD_DEC_MEM_BUFFER_INFO_T));
    AUD_SetDecMemBuffer(u1DecId, &rMemBuf);
    return RMR_ASYNC_NFY;
}

static void _SetCompressionMode(UINT8 u1DecId, AUD_DEC_CMPSS_MODE_T eCmprssMode)
{
    UINT8 u1cmprssMode = 0;

    _aeCmprssMode[u1DecId] = eCmprssMode;
    switch (eCmprssMode)
    {
        case AUD_DEC_CMPSS_MODE_LINE:
            u1cmprssMode = 2;
            break;
        case AUD_DEC_CMPSS_MODE_CUSTOM0:
            u1cmprssMode = 0;
            break;
        case AUD_DEC_CMPSS_MODE_CUSTOM1:
            u1cmprssMode = 1;
            break;
        case AUD_DEC_CMPSS_MODE_RF:
            u1cmprssMode = 3;
            break;
        default:
            break;
    }

    AUD_DspAC3CompMode(u1cmprssMode, u1DecId);
}

static void _SetAgcFlag(UINT8 u1DecId, BOOL fgAgcFlag)
{
    _afgAgcFlag[u1DecId] = fgAgcFlag;
    AUD_DspAVCEnable(u1DecId, fgAgcFlag);
}

static void _SetChlDelay(UINT8 u1DecId, const AUD_DEC_CHNL_DELAY_T * prChlDelay)
{
/*
    u2Delay: 0.05 m/unit
    uChIndex: 0:C 1:L 2:R 3:LS 4:RS 5: reserved 6: reserved 7: SUBWOOFER
*/
    VERIFY((prChlDelay->e_out_port) < (AUD_DEC_OUT_PORT_SPEAKER + 1));

    if (prChlDelay->e_ch_delay_type == AUD_DEC_CHNL_DELAY_TYPE_ALL)
    {
        _aau2MasterDelay[u1DecId][prChlDelay->e_out_port]  = prChlDelay->u.ui2_delay;
        if (u1DecId == AUD_DEC_MAIN)
        {
            AUD_DspChannelDelay(prChlDelay->u.ui2_delay , AUD_CH_ALL, AUD_DEC_MAIN);   //Andrew Wen 2007/6/6 simplify all channel delay
        #if 0
            AUD_DspChannelDelay(prChlDelay->u.ui2_delay , 0, AUD_DEC_MAIN);
            AUD_DspChannelDelay(prChlDelay->u.ui2_delay , 1, AUD_DEC_MAIN);
            AUD_DspChannelDelay(prChlDelay->u.ui2_delay , 2, AUD_DEC_MAIN);
            AUD_DspChannelDelay(prChlDelay->u.ui2_delay , 3, AUD_DEC_MAIN);
            AUD_DspChannelDelay(prChlDelay->u.ui2_delay , 4, AUD_DEC_MAIN);
            AUD_DspChannelDelay(prChlDelay->u.ui2_delay , 5, AUD_DEC_MAIN);
            AUD_DspChannelDelay(prChlDelay->u.ui2_delay , 6, AUD_DEC_MAIN);
            AUD_DspChannelDelay(prChlDelay->u.ui2_delay , 7, AUD_DEC_MAIN);
            AUD_DspChannelDelay(prChlDelay->u.ui2_delay , 8, AUD_DEC_MAIN);
            AUD_DspChannelDelay(prChlDelay->u.ui2_delay , 9, AUD_DEC_MAIN);
        #endif
        }
        else  // AUD_DEC_AUX
        {
            AUD_DspChannelDelay(prChlDelay->u.ui2_delay , AUD_CH_AUX_FRONT_LEFT, AUD_DEC_AUX);
            AUD_DspChannelDelay(prChlDelay->u.ui2_delay , AUD_CH_AUX_FRONT_RIGHT, AUD_DEC_AUX);
        }
    }
    else // AUD_DEC_CHNL_DELAY_TYPE_INDIVIDUAL_CH
    {
        switch (prChlDelay->e_out_port)
        {
            case AUD_DEC_OUT_PORT_OFF: // dummy port
                x_memcpy((VOID *)&_arChlDelayInd[u1DecId][AUD_DEC_OUT_PORT_OFF], (const VOID *)&(prChlDelay->u.t_ind_delay),
                                 sizeof(AUD_DEC_CHNL_DELAY_IND_T));
                break;
            case AUD_DEC_OUT_PORT_2_CH:
                x_memcpy((VOID *)&_arChlDelayInd[u1DecId][AUD_DEC_OUT_PORT_2_CH], (const VOID *)&(prChlDelay->u.t_ind_delay),
                                 sizeof(AUD_DEC_CHNL_DELAY_IND_T));
                break;
            case AUD_DEC_OUT_PORT_5_1_CH:
                x_memcpy((VOID *)&_arChlDelayInd[u1DecId][AUD_DEC_OUT_PORT_5_1_CH], (const VOID *)&(prChlDelay->u.t_ind_delay),
                                 sizeof(AUD_DEC_CHNL_DELAY_IND_T));
                AUD_DspChannelDelay(prChlDelay->u.t_ind_delay.ui2_c_delay , AUD_CH_CENTER, AUD_DEC_MAIN);
                AUD_DspChannelDelay(prChlDelay->u.t_ind_delay.ui2_fl_delay , AUD_CH_FRONT_LEFT, AUD_DEC_MAIN);
                AUD_DspChannelDelay(prChlDelay->u.t_ind_delay.ui2_fr_delay , AUD_CH_FRONT_RIGHT, AUD_DEC_MAIN);
                AUD_DspChannelDelay(prChlDelay->u.t_ind_delay.ui2_rl_delay , AUD_CH_REAR_LEFT, AUD_DEC_MAIN);
                AUD_DspChannelDelay(prChlDelay->u.t_ind_delay.ui2_rr_delay , AUD_CH_REAR_RIGHT, AUD_DEC_MAIN);
                AUD_DspChannelDelay(prChlDelay->u.t_ind_delay.ui2_sw_delay , AUD_CH_SUB_WOOFER, AUD_DEC_MAIN);
                break;
            case AUD_DEC_OUT_PORT_SPDIF: // driver not support
                x_memcpy((VOID *)&_arChlDelayInd[u1DecId][AUD_DEC_OUT_PORT_SPDIF], (const VOID *)&(prChlDelay->u.t_ind_delay),
                                 sizeof(AUD_DEC_CHNL_DELAY_IND_T));
                break;
            case AUD_DEC_OUT_PORT_2_CH_BY_PASS: // driver not support
                x_memcpy((VOID *)&_arChlDelayInd[u1DecId][AUD_DEC_OUT_PORT_2_CH_BY_PASS], (const VOID *)&(prChlDelay->u.t_ind_delay),
                                 sizeof(AUD_DEC_CHNL_DELAY_IND_T));
                break;
            case AUD_DEC_OUT_PORT_SPEAKER:
                x_memcpy((VOID *)&_arChlDelayInd[u1DecId][AUD_DEC_OUT_PORT_SPEAKER], (const VOID *)&(prChlDelay->u.t_ind_delay),
                                 sizeof(AUD_DEC_CHNL_DELAY_IND_T));
                break;
        }
    }
}

static void _SetCopyProtect(UINT8 u1DecId, AUD_DEC_COPY_PROTECT_T eCopyProtect)
{
    _aeCopyProtect[u1DecId] = eCopyProtect;
}

static void _SetMts(UINT8 u1DecId, AUD_DEC_MTS_T eMts)
{
    UINT32 u1DecoderId = u1DecId;
    UNUSED(u1DecId);

#ifdef CC_AUD_DUAL_DEC_SUPPORT
#ifndef CC_AUD_SINGLE_DEC_MODE
    u1DecoderId = AUD_DEC_AUX;
#endif
#endif

    switch (eMts)
    {
        case AUD_DEC_MTS_MONO:
            AUD_DspSetAtvOutputMode(u1DecoderId, AUD_SOUND_MODE_MONO);
            break;
        case AUD_DEC_MTS_STERRO:
            AUD_DspSetAtvOutputMode(u1DecoderId, AUD_SOUND_MODE_STEREO);
            break;
        case AUD_DEC_MTS_SUB_LANG:
            AUD_DspSetAtvOutputMode(u1DecoderId, AUD_SOUND_MODE_SUB_LANG);
            break;
        case AUD_DEC_MTS_DUAL1:
        	AUD_DspSetAtvOutputMode(u1DecoderId, AUD_SOUND_MODE_DUAL1);
        	break;
        case AUD_DEC_MTS_DUAL2:
        	AUD_DspSetAtvOutputMode(u1DecoderId, AUD_SOUND_MODE_DUAL2);
        	break;
        case AUD_DEC_MTS_NICAM_MONO:
        	AUD_DspSetAtvOutputMode(u1DecoderId, AUD_SOUND_MODE_NICAM_MONO);
        	break;
        case AUD_DEC_MTS_NICAM_STEREO:
        	AUD_DspSetAtvOutputMode(u1DecoderId, AUD_SOUND_MODE_NICAM_STEREO);
        	break;
        case AUD_DEC_MTS_NICAM_DUAL1:
        	AUD_DspSetAtvOutputMode(u1DecoderId, AUD_SOUND_MODE_NICAM_DUAL1);
        	break;
        case AUD_DEC_MTS_NICAM_DUAL2:
        	AUD_DspSetAtvOutputMode(u1DecoderId, AUD_SOUND_MODE_NICAM_DUAL2);
        	break;

        default:
            break;
    }
}

static void _SetUserEqLevel(UINT8 u1DecId, AUD_DEC_SE_QA_USER_LVL_T * prEqLvl)
{
    VERIFY(prEqLvl->ui1_band_idx < CH_EQ_BAND_NUM);
    VERIFY(prEqLvl->e_eq_type < (AUD_DEC_EQ_BBE_AV + 1));

    _ai1UserEqCfg[prEqLvl->e_eq_type][prEqLvl->ui1_band_idx] = prEqLvl->i1_level;
    // Since the level is in 1/10 dB unit, translate to 1/2 dB unit
    AUD_DspSetUserDefinedEqCfg(u1DecId, (AUD_EQ_TYPE_T)prEqLvl->e_eq_type, prEqLvl->ui1_band_idx, (prEqLvl->i1_level) /5);
}

static void _SetTvSysMask(UINT8 u1DecId, UINT32 u4Mask)
{
    Aud_SetTvSystemMask( u1DecId, u4Mask);
}

static void _SetTvAudSysMask(UINT8 u1DecId, AUD_DEC_TV_AUD_SYS_T* prMask)
{

#ifdef CC_AUD_DVBT_SUPPORT
    _arTvAudSysMask[u1DecId].ui4_tv_sys_mask = prMask->ui4_tv_sys_mask;
    _arTvAudSysMask[u1DecId].ui4_aud_sys_mask = prMask->ui4_aud_sys_mask;

#endif
    // FIXME !!
    Aud_SetTvSystemMask( u1DecId, prMask->ui4_tv_sys_mask);
}

static INT32 _SetAudClip(UINT8 u1DecId, AUD_DEC_AUD_CLIP_T * prClip)
{
    AUD_CLIP_TYPE_T rAudClip;

    if (AUD_IsDecoderPlay(u1DecId))
    {
        return (RMR_DRV_INV_SET_INFO);
    }

    if (prClip->ui4_ch_mask == AUD_DEV_LS_FLAG_NONE)
    {
        return (RMR_OK);
    }

    x_memcpy(&_arAudClipSetType[u1DecId], prClip, sizeof(AUD_DEC_AUD_CLIP_T));

    x_memset(&rAudClip, 0, sizeof(AUD_CLIP_TYPE_T));
    switch (prClip->e_type)
    {
    case AUD_DEC_AUD_CLIP_TYPE_TONE:
        rAudClip.fgPlayTestTone = TRUE;
        rAudClip.u4ToneFreq = prClip->u.ui4_tone_freq;
        break;
    case AUD_DEC_AUD_CLIP_TYPE_CLIP_BUFF:
        rAudClip.fgPlayTestTone = FALSE;
        rAudClip.eAudFmt = (AUD_FMT_T)prClip->u.t_clip_buff.e_fmt;
        rAudClip.eMemBuf.pData = prClip->u.t_clip_buff.pui1_clip_buff;
        rAudClip.eMemBuf.u4Length =
            (UINT32)prClip->u.t_clip_buff.z_clip_buff_size;
        break;

    default:
        return (RMR_DRV_INV_SET_INFO);
    }

    rAudClip.u4RepeatCount = prClip->ui4_repeat_cnt;
    rAudClip.u4OutputChlSelect = prClip->ui4_ch_mask;
    rAudClip.u4AudCtrlTypeAddr = (UINT32)AUD_DEC_CTRL_PLAY_AUD_CLIP;
    x_memcpy(&_arAudClip[u1DecId], &rAudClip, sizeof(AUD_CLIP_TYPE_T));
    AUD_SetAudClip(u1DecId, &_arAudClip[u1DecId]);
    return (RMR_OK);
}

static void _SetPrologic2(UINT8 u1DecId, AUD_DEC_PL2_CTRL_INFO_T * prPL2)
{
    UINT8 u1Ctrl, u1Mode;

    x_memcpy(&_rPrologic2Type, prPL2, sizeof(AUD_DEC_PL2_CTRL_INFO_T));
    u1Ctrl = (UINT8)prPL2->e_ctrl;

    switch (prPL2->e_ctrl)
    {
    case AUD_DEC_PL2_CTRL_SWITCH:
        u1Mode = (UINT8)prPL2->u.e_pl2_switch;
        break;

    case AUD_DEC_PL2_CTRL_MODE:
        u1Mode = PL2CTRL_MODE_MANUAL;
        u1Mode |= ((UINT8)prPL2->u.e_pl2_mode) << 1;
        break;

    case AUD_DEC_PL2_CTRL_PANORAMA:
        u1Mode = (UINT8)prPL2->u.b_is_pl2_panorama_on;
        break;

    case AUD_DEC_PL2_CTRL_DIMENSION:
    case AUD_DEC_PL2_CTRL_C_WIDTH:
        u1Mode = (UINT8)prPL2->u.t_pl2_val.ui1_curr_val;
        break;

    default:
        return;
    }
    AUD_DspPL2Config(u1Ctrl, u1Mode);
}

static void _SetDolbyBanner(UINT8 u1DecId, BOOL fgMode)
{
    _afgDolbyBanner[u1DecId] = fgMode;
    AUD_DDBannerEnable(u1DecId, fgMode);
}

static void _SetOutPortAdj(UINT8 u1DecId, AUD_DEC_OUTPORT_ADJ_T * pOutportAdj)
{
    AUD_DEC_CAPABILITY_INFO_T rDecCap;

    UNUSED(_GetCapability(u1DecId, &rDecCap));

    if (!rDecCap.b_is_outport_adj)
    {   // Not Adjustable .. do nothing ..
        return ;
    }
    else
    {
        if ((pOutportAdj->e_out_port == AUD_DEC_OUT_PORT_2_CH) ||
            (pOutportAdj->e_out_port == AUD_DEC_OUT_PORT_2_CH_BY_PASS))
        {
            if (pOutportAdj->b_is_fixed)
            {
                AUD_AoutPADMux(AUD_CH_FRONT_LEFT, AUD_CH_REAR_LEFT, AUD_CH_CENTER, AUD_CH_FRONT_LEFT, AUD_CH_BYPASS_LEFT, AUD_CH_AUX_FRONT_LEFT, 0);
            }
            else
            {
                AUD_AoutPADMux(AUD_CH_FRONT_LEFT, AUD_CH_REAR_LEFT, AUD_CH_CENTER, AUD_CH_FRONT_LEFT, AUD_CH_FRONT_LEFT, AUD_CH_AUX_FRONT_LEFT, 0);
            }
        }
        else if (pOutportAdj->e_out_port == AUD_DEC_OUT_PORT_SPDIF)
        {
            if (pOutportAdj->b_is_fixed)
            {
                AUD_DspIecChannel(u1DecId, AUD_IEC_PCM_CH_7_8);
            }
            else
            {
                AUD_DspIecChannel(u1DecId, AUD_IEC_PCM_CH_L_R);
            }
        }
    }
}

static INT32 _SetAvSyncMode(UINT8 u1DecId, AUD_DEC_AV_SYNC_INFO_T eSyncMode)
{
    AV_SYNC_MODE_T eMode;

    if (eSyncMode == AUD_DEC_AV_SYNC_AUD_SLAVE)
    {
        eMode = AV_SYNC_SLAVE;
    }
    else if (eSyncMode == AUD_DEC_AV_SYNC_FREE_RUN)
    {
        eMode = AV_SYNC_FREE_RUN;
    }
    else if (eSyncMode == AUD_DEC_AV_SYNC_AUD_MASTER)
    {
        eMode = AV_SYNC_AUDIO_MASTER;
    }
    else
    {
        eMode = AV_SYNC_SLAVE;
    }

    AUD_SetAvSynMode(u1DecId, eMode);
    return RMR_OK;
}

static BOOL _AudBagtNfyFct(
    VOID*                     pv_tag,
    AUD_BUF_AGT_NFY_COND_T     e_nfy_cond,
    UINT32                    ui4_data_1,
    UINT32                    ui4_data_2)
{
    if (e_nfy_cond == AUD_BUF_AGT_NFY_COND_ALIGN_INFO)
    {
        AUD_BagtSetSrcInfo( 0 , (const AUD_BUF_AGT_SRC_ALIGN_INFO_T*)ui4_data_1, FALSE);  // send the info to drv intf
        return TRUE;
    }
    else
    {
    return AUD_BagtNfyFct(pv_tag, e_nfy_cond, ui4_data_1,ui4_data_2 );
    }
}

static BOOL _AudBagtGetPosFct(
    VOID*       pv_tag,
    UINT32*     pui4_current_req_id,
    UINT32*     pui4_current_byte_offset)
{
    UINT8 u1DecId;

    u1DecId = (_aeStrSrc[AUD_DEC_MAIN] == AUD_STREAM_FROM_BUF_AGT) ?
                                                        AUD_DEC_MAIN : AUD_DEC_AUX ;
    if (AUD_IsDecoderPlay(u1DecId))
    {
        *pui4_current_req_id = AUD_BagtGetRqstId();
        *pui4_current_byte_offset = AUD_BagtGetCurrOffset();
        return TRUE;
    }
    else
    {
         return FALSE;
    }
}

static void _AudBagtGetSrcInfo(UINT16 ui2_buf_agt_id)
{
    AUD_BUF_AGT_SRC_ALIGN_INFO_T    t_data_blk;
    INT32                       i4_ret;

    i4_ret = buf_agt_mw_get_source_info(ui2_buf_agt_id, (BUF_AGT_SRC_ALIGN_INFO_T *)&t_data_blk);
    if (i4_ret != 0)
    {
        LOG(5,"[AUD] buf_agt_mw_get_source_info failed %d\n", i4_ret);
    }
    else
    {
        AUD_BagtSetSrcInfo( ui2_buf_agt_id , &t_data_blk, TRUE);  // send the info to drv intf
        LOG(5,"[AUD] buf_agt_mw_get_source_info align %d, min size %d\n",
               t_data_blk.ui4_align,
               t_data_blk.ui4_min_blk_size);
    }
}

static void _AudBagtDisconnect( UINT16  ui2_buf_agt_id)
{
    INT32               i4_ret;

    i4_ret = buf_agt_mw_disconnect(ui2_buf_agt_id);

    if (i4_ret != 0)
    {
        LOG(1,"[AUD] buf_agt_mw_disconnect failed %d\n", i4_ret);
    }
    else
    {
        LOG(1,"[AUD] buf_agt_mw_disconnect ok\n");
    }
}


static void _AudBagtSendRequest(    UINT16                  ui2_buf_agt_id,
                                                        AUD_BUF_AGT_DATA_BLK_T*     pt_data_blk,
                                                            AUD_2_BAGT_NFY_COND_T eType)

{
    INT32       i4_ret;
    switch (eType)
    {
        case AUD_2_BAGT_NFY_COND_UNKNOWN:

            break;
        case AUD_2_BAGT_NFY_COND_DATA_REQ:
            i4_ret = buf_agt_mw_data_req(ui2_buf_agt_id, (BUF_AGT_DATA_BLK_T *)pt_data_blk);
            if (i4_ret != 0)
            {
                 LOG(5,"[AUD] buf_agt_mw_data_req failed %d\n", i4_ret);
            }
            else
            {
                 LOG(5,"[AUD] buf_agt_mw_data_req ok\n");
            }
            break;
        case AUD_2_BAGT_NFY_COND_FLUSH_REQ:
            i4_ret = buf_agt_mw_flush(ui2_buf_agt_id);
            if (i4_ret != 0)
            {
                 LOG(5,"[AUD] buf_agt_mw_flush failed %d\n", i4_ret);
            }
            else
            {
                 LOG(5,"[AUD] buf_agt_mw_flush ok\n");
            }
            break;
        case AUD_2_BAGT_NFY_COND_DATA_CONSUMED:
            i4_ret = buf_agt_mw_data_consumed(ui2_buf_agt_id,(BUF_AGT_DATA_BLK_T *)pt_data_blk);
            if (i4_ret != 0)
            {
                 LOG(5,"[AUD] buf_agt_mw_data_consumed failed %d\n", i4_ret);
            }
            else
            {
                 LOG(5,"[AUD] buf_agt_mw_data_consumed ok\n");
            }
            break;
        default:
            break;
    }
}

#ifdef CC_AUD_BBE_SUPPORT
static void _SetBbeMode(UINT8 u1DecId, AUD_DEC_BBE_MODE_T eBbeMode)
{
    _aeBbeMode[u1DecId] = eBbeMode;
    AUD_DspBbeMode(u1DecId, (UINT8)eBbeMode);
}
#endif

static void _SetPIPVideoPathInfo(UINT8 u1DecId, BOOL u1videopath)
{
    AUD_DrvSetPIPVideoPath(u1DecId, u1videopath);
}

static BOOL _IsAudFmtValid(AUD_DEC_FMT_INFO_T *prFormatInfo)
{
    UINT32 u4FmtMask = ((UINT32) 1) << prFormatInfo->e_fmt;

    if ((u4FmtMask & AUD_FMT_CAPABILITY) == 0)
    {
        return FALSE;
    }

    return TRUE;
}

static INT32 _AudConnect(DRV_COMP_ID_T*  pt_comp_id,
                        DRV_CONN_TYPE_T  e_conn_type,
                        const VOID*      pv_conn_info,
                        SIZE_T           z_conn_info_len,
                        VOID*            pv_tag,
                        x_rm_nfy_fct     pf_nfy)
{
    DRV_COMP_ID_T* prCompReg;
    UINT8 u1DecId = AUD_DEC_MAIN;

    // Check component type and id

    if (pt_comp_id == NULL)
    {
        return (RMR_DRV_INV_CONN_INFO);
    }

    if (pt_comp_id->e_type != DRVT_AUD_DEC)
    {
        return (RMR_DRV_INV_CONN_INFO);
    }

/*
    if (pt_comp_id->ui2_id > (AUD_DEC_NUM - 1))
    {
        return (RMR_DRV_INV_CONN_INFO);
    }
*/
     //Convert Decoder ID
    if (_AudConvertDecId(pt_comp_id->ui2_id, &u1DecId))
    {
        return (RMR_DRV_INV_SET_INFO);
    }

    if (pt_comp_id->b_sel_out_port != FALSE)
    {
        return (RMR_DRV_INV_CONN_INFO);
    }

    if (pt_comp_id->u.ui1_inp_port > 1)
    {
        return (RMR_DRV_INV_CONN_INFO);
    }

    if (e_conn_type != RM_CONN_TYPE_COMP_ID)
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    prCompReg = (DRV_COMP_ID_T*)pv_conn_info;

    if ((prCompReg->e_type != DRVT_DEMUX_TS_PES_PACKET) &&
        (prCompReg->e_type != DRVT_AVC_HDMI) &&
        (prCompReg->e_type != DRVT_AVC_AUDIO_INP) &&
        (prCompReg->e_type != DRVT_AVC_SPDIF) &&
        (prCompReg->e_type != DRVT_TUNER_SAT_ANA) &&
        (prCompReg->e_type != DRVT_TUNER_CAB_ANA) &&
        (prCompReg->e_type != DRVT_TUNER_TER_ANA) &&
        (prCompReg->e_type != DRVT_AVC_SCART) &&
        (prCompReg->e_type != DRVT_BUF_AGENT))
    {
        return (RMR_DRV_INV_CONN_INFO);
    }
/*
    if ((prCompReg->e_type == DRVT_DEMUX_TS_PES_PACKET) &&
        (prCompReg->ui2_id > (DMX_PES_COMP_ID_END)))
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    if ((prCompReg->e_type == DRVT_AVC_HDMI) &&
        (prCompReg->ui2_id > (AVC_HDMI_COMP_NUM - 1)))
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    if ((prCompReg->e_type == DRVT_AVC_SPDIF) &&
        (prCompReg->ui2_id > (AVC_SPDIF_COMP_NUM - 1)))
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    if ((prCompReg->e_type == DRVT_TUNER_SAT_ANA) &&
        (prCompReg->ui2_id > (TUNER_ANA_COMP_NUM - 1)))
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    if ((prCompReg->e_type == DRVT_TUNER_CAB_ANA) &&
        (prCompReg->ui2_id > (TUNER_ANA_COMP_NUM - 1)))
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    if ((prCompReg->e_type == DRVT_TUNER_TER_ANA) &&
        (prCompReg->ui2_id > (TUNER_ANA_COMP_NUM - 1)))
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    if ((prCompReg->e_type == DRVT_AVC_AUDIO_INP) &&
        (prCompReg->ui2_id > (AVC_AUDIO_INP_NUM - 1)))
    {
       return (RMR_DRV_INV_CONN_INFO);
    }
*/
    // Handle connect
    _au2MwConnectComp[u1DecId] = (UINT32)prCompReg->e_type;

    switch (prCompReg->e_type)
    {
        case DRVT_DEMUX_TS_PES_PACKET:
#ifndef CC_AUD_DVBT_SUPPORT
            AUD_DRVSetDecodeType(u1DecId, AUD_STREAM_FROM_DIGITAL_TUNER, AUD_FMT_AC3);
#endif
            _aeStrSrc[u1DecId]  = AUD_STREAM_FROM_DIGITAL_TUNER;
            break;

        case DRVT_AVC_HDMI:
            AUD_DRVSetDecodeType(u1DecId, AUD_STREAM_FROM_HDMI, AUD_FMT_PCM);
            _aeStrSrc[u1DecId] = AUD_STREAM_FROM_HDMI;
            break;

        case DRVT_AVC_AUDIO_INP:
            AUD_DRVSetDecodeType(u1DecId, AUD_STREAM_FROM_LINE_IN, AUD_FMT_PCM);
            AUD_AvInputMuxSel((AUD_INPUT_ID_T)prCompReg->ui2_id);
            _aeStrSrc[u1DecId] = AUD_STREAM_FROM_LINE_IN;
            break;

        case DRVT_AVC_SPDIF:
            AUD_DRVSetDecodeType(u1DecId, AUD_STREAM_FROM_SPDIF, AUD_FMT_AC3);
            _aeStrSrc[u1DecId] = AUD_STREAM_FROM_SPDIF;
            break;
        case DRVT_TUNER_SAT_ANA:
        case DRVT_TUNER_CAB_ANA:
        case DRVT_TUNER_TER_ANA:
            AUD_DRVSetDecodeType(u1DecId, AUD_STREAM_FROM_ANALOG_TUNER, AUD_FMT_MTS);
            _aeStrSrc[u1DecId] = AUD_STREAM_FROM_ANALOG_TUNER;
            break;
        case DRVT_AVC_SCART:
            AUD_AvInputMuxSel((AUD_INPUT_ID_T)(prCompReg->ui2_id + AUD_INPUT_ID_SCART_0));
            _aeStrSrc[u1DecId] = AUD_STREAM_FROM_LINE_IN;
            break;
        case DRVT_BUF_AGENT:
            AUD_DRVSetDecodeType(u1DecId, AUD_STREAM_FROM_BUF_AGT, AUD_FMT_MP3);
            _aeStrSrc[u1DecId] = AUD_STREAM_FROM_BUF_AGT;
            buf_agt_mw_connect(prCompReg->ui2_id, _AudBagtNfyFct, _AudBagtGetPosFct, pv_tag);
             _AudBagtGetSrcInfo(prCompReg->ui2_id);
            break;
        default:
            LOG(1, "ERROR _AudConnect id\n");

    }

    return (RMR_OK);
}

static INT32 _AudDisconnect(DRV_COMP_ID_T* pt_comp_id,
                        DRV_DISC_TYPE_T  e_disc_type,
                        const VOID*      pv_disc_info,
                        SIZE_T           z_disc_info_len)
{
    DRV_COMP_ID_T* prCompReg;
    UINT8 u1DecId = AUD_DEC_MAIN;

    // Check component type and id
    if (pt_comp_id == NULL)
        return (RMR_DRV_INV_DISC_INFO);

    if (pt_comp_id->e_type != DRVT_AUD_DEC)
    {
        return (RMR_DRV_INV_DISC_INFO);
    }

/*
    if (pt_comp_id->ui2_id > (AUD_DEC_NUM - 1))
    {
        return (RMR_DRV_INV_DISC_INFO);
    }
*/
    //Convert Decoder ID
    if (_AudConvertDecId(pt_comp_id->ui2_id, &u1DecId))
    {
        return (RMR_DRV_INV_SET_INFO);
    }

    if (pt_comp_id->b_sel_out_port != FALSE)
    {
        return (RMR_DRV_INV_DISC_INFO);
    }

    if (pt_comp_id->u.ui1_inp_port > 1)
    {
        return (RMR_DRV_INV_DISC_INFO);
    }

    if (e_disc_type != RM_CONN_TYPE_COMP_ID)
    {
        return (RMR_DRV_INV_DISC_INFO);
    }

    prCompReg = (DRV_COMP_ID_T*)pv_disc_info;

    if ((prCompReg->e_type != DRVT_DEMUX_TS_PES_PACKET) &&
        (prCompReg->e_type != DRVT_AVC_COMP_VIDEO) &&
        (prCompReg->e_type != DRVT_AVC_HDMI) &&
        (prCompReg->e_type != DRVT_AVC_AUDIO_INP) &&
        (prCompReg->e_type != DRVT_AVC_SPDIF) &&
        (prCompReg->e_type != DRVT_TUNER_SAT_ANA) &&
        (prCompReg->e_type != DRVT_TUNER_CAB_ANA) &&
        (prCompReg->e_type != DRVT_TUNER_TER_ANA) &&
        (prCompReg->e_type != DRVT_AVC_SCART) &&
        (prCompReg->e_type != DRVT_BUF_AGENT))
    {
        return (RMR_DRV_INV_DISC_INFO);
    }

/*
    if ((prCompReg->e_type == DRVT_DEMUX_TS_PES_PACKET) &&
        (prCompReg->ui2_id > (DMX_PES_COMP_ID_END)))
    {
        return (RMR_DRV_INV_DISC_INFO);
    }

    if ((prCompReg->e_type == DRVT_AVC_HDMI) &&
        (prCompReg->ui2_id > (AVC_HDMI_COMP_NUM - 1)))
    {
        return (RMR_DRV_INV_DISC_INFO);
    }

    if ((prCompReg->e_type == DRVT_AVC_SPDIF) &&
        (prCompReg->ui2_id > (AVC_SPDIF_COMP_NUM - 1)))
    {
        return (RMR_DRV_INV_DISC_INFO);
    }

    if ((prCompReg->e_type == DRVT_TUNER_SAT_ANA) &&
        (prCompReg->ui2_id > (TUNER_ANA_COMP_NUM - 1)))
    {
        return (RMR_DRV_INV_DISC_INFO);
    }

    if ((prCompReg->e_type == DRVT_TUNER_CAB_ANA) &&
        (prCompReg->ui2_id > (TUNER_ANA_COMP_NUM - 1)))
    {
        return (RMR_DRV_INV_DISC_INFO);
    }

    if ((prCompReg->e_type == DRVT_TUNER_TER_ANA) &&
        (prCompReg->ui2_id > (TUNER_ANA_COMP_NUM - 1)))
    {
        return (RMR_DRV_INV_DISC_INFO);
    }

    if ((prCompReg->e_type == DRVT_AVC_AUDIO_INP) &&
        (prCompReg->ui2_id > (AVC_AUDIO_INP_NUM - 1)))
    {
        return (RMR_DRV_INV_DISC_INFO);
    }
*/
    if (prCompReg->e_type == DRVT_BUF_AGENT)
    {
        _AudBagtDisconnect(prCompReg->ui2_id);
    }
    // handle disconnect
    AUD_DspDvdDisable();

    // FIXME
    // Should do something

    return (RMR_OK);
}

static INT32 _AudGet(DRV_COMP_ID_T* pt_comp_id,
                    DRV_GET_TYPE_T  e_get_type,
                    VOID*           pv_get_info,
                    SIZE_T*         pz_get_info_len)
{
    UINT8 u1DecId = AUD_DEC_MAIN;

    // Check component type and id
    if (pt_comp_id == NULL)
        return (RMR_DRV_INV_GET_INFO);

    if (pt_comp_id->e_type != DRVT_AUD_DEC)
    {
        return (RMR_DRV_INV_GET_INFO);
    }

/*
    if (pt_comp_id->ui2_id > (AUD_DEC_NUM - 1))
    {
        return (RMR_DRV_INV_GET_INFO);
    }
*/
    //Convert Decoder ID
    if (_AudConvertDecId(pt_comp_id->ui2_id, &u1DecId))
    {
        return (RMR_DRV_INV_SET_INFO);
    }

    // Perform get opertion
    switch (e_get_type)
    {
        case AUD_DEC_GET_TYPE_CTRL:
            M_GET_VALIDATE(AUD_DEC_CTRL_T);
            _AudGetCtrl(u1DecId, (AUD_DEC_CTRL_T *)pv_get_info);
            break;

        case AUD_DEC_GET_TYPE_INP_MODE:
            M_GET_VALIDATE(BOOL);
            *(BOOL *)pv_get_info = AUD_GetOperationMode(u1DecId);
            break;

        case AUD_DEC_GET_TYPE_FMT:
            M_GET_VALIDATE(AUD_DEC_FMT_INFO_T);
            _GetFormat(u1DecId, (AUD_DEC_FMT_INFO_T *)pv_get_info);
            break;

        case AUD_DEC_GET_TYPE_VOLUME:
            M_GET_VALIDATE(AUD_DEC_VOLUME_INFO_T);
            _GetVolume(u1DecId, (AUD_DEC_VOLUME_INFO_T *)pv_get_info);
            break;

        case AUD_DEC_GET_TYPE_MUTE:
            M_GET_VALIDATE(AUD_DEC_MUTE_TYPE_T);
            _GetMute(u1DecId, (AUD_DEC_MUTE_TYPE_T *)pv_get_info);
            break;

        case AUD_DEC_GET_TYPE_SPDIF:
            M_GET_VALIDATE(AUD_DEC_SPDIF_TYPE_T);
            _GetSpdif(u1DecId, (AUD_DEC_SPDIF_TYPE_T *)pv_get_info);
            break;

        case AUD_DEC_GET_TYPE_HDCD_FLTR:
            M_GET_VALIDATE(AUD_DEC_HDCD_FLTR_T);
            _GetHdcdfilter(u1DecId, (AUD_DEC_HDCD_FLTR_T *)pv_get_info);
            break;

        case AUD_DEC_GET_TYPE_LS_MODE:
            M_GET_VALIDATE(AUD_DEC_CH_LS_MODE_T);
            _GetLsMode(u1DecId, (AUD_DEC_CH_LS_MODE_T *)pv_get_info);
            break;

        case AUD_DEC_GET_TYPE_SOUND_EFFECT:
            M_GET_VALIDATE(AUD_DEC_SE_INFO_T);
            _GetSoundEffect(u1DecId, (AUD_DEC_SE_INFO_T *)pv_get_info);
            break;

        case AUD_DEC_GET_TYPE_DOWNMIX:
            M_GET_VALIDATE(AUD_DEC_DOWNMIX_MODE_T);
            _GetDownMix(u1DecId, (AUD_DEC_DOWNMIX_MODE_T *)pv_get_info);
            break;

        case AUD_DEC_GET_TYPE_DRC:
            M_GET_VALIDATE(AUD_DEC_DRC_T);
            _GetDrc(u1DecId, (AUD_DEC_DRC_T *)pv_get_info);
            break;

        case AUD_DEC_GET_TYPE_CAPABILITY:
            M_GET_VALIDATE(AUD_DEC_CAPABILITY_INFO_T);
            _GetCapability(u1DecId, (AUD_DEC_CAPABILITY_INFO_T *)pv_get_info);
            break;

        case AUD_DEC_GET_TYPE_OUT_PORT:
            M_GET_VALIDATE(UINT32);
            _GetOutPort(u1DecId, (UINT32 *)pv_get_info);
            break;

        case AUD_DEC_GET_TYPE_PCR_ID:
            M_GET_VALIDATE(UINT16);
            *(UINT16 *)pv_get_info = (UINT16)AUD_GetStcId(u1DecId);
            break;

        case AUD_DEC_GET_TYPE_AUD_INFO:
            M_GET_VALIDATE(AUD_DEC_AUD_INFO_T);
            _GetAudInfo(u1DecId, (AUD_DEC_AUD_INFO_T *)pv_get_info);
            break;

        case AUD_DEC_GET_TYPE_CMPSS_MODE:
            M_GET_VALIDATE(AUD_DEC_CMPSS_MODE_T);
            _GetCompressionMode(u1DecId, (AUD_DEC_CMPSS_MODE_T *)pv_get_info);
            break;

        case AUD_DEC_GET_TYPE_AGC:
            M_GET_VALIDATE(BOOL);
            _GetAgcFlag(u1DecId, (BOOL *)pv_get_info);
            break;

        case AUD_DEC_GET_TYPE_CHNL_DELAY:
            M_GET_VALIDATE(AUD_DEC_CHNL_DELAY_T);
            _GetChlDelay(u1DecId, (AUD_DEC_CHNL_DELAY_T *)pv_get_info);
            break;

        case AUD_DEC_GET_TYPE_COPY_PROTECT:
            M_GET_VALIDATE(AUD_DEC_COPY_PROTECT_T);
            _GetCopyProtect(u1DecId, (AUD_DEC_COPY_PROTECT_T *)pv_get_info);
            break;

        case AUD_DEC_GET_TYPE_MTS:
            M_GET_VALIDATE(AUD_DEC_MTS_T);
            _GetMts(u1DecId, (AUD_DEC_MTS_T *)pv_get_info);
            break;

        case AUD_DEC_GET_TYPE_MAX_CHNL_DELAY:
            M_GET_VALIDATE(UINT8);
            _GetMaxDelay(u1DecId, (UINT8 *) pv_get_info);
            break;

        case AUD_DEC_GET_TYPE_SE_EQ_USER_NUM_BAND:
            M_GET_VALIDATE(UINT8);
            _GetUserEqBandNum(u1DecId, (UINT8 *) pv_get_info);
            break;

        case AUD_DEC_GET_TYPE_SE_EQ_USER_LVL:
            M_GET_VALIDATE(AUD_DEC_SE_QA_USER_LVL_T);
            _GetUserEqLevel(u1DecId, (AUD_DEC_SE_QA_USER_LVL_T *) pv_get_info);
            break;

        case AUD_DEC_GET_TYPE_SE_EQ_USER_INFO_BAND:
            M_GET_VALIDATE(AUD_DEC_SE_QA_USER_INFO_BAND_T);
            _GetUserEqInfo(u1DecId, (AUD_DEC_SE_QA_USER_INFO_BAND_T *) pv_get_info);
            break;

        case AUD_DEC_GET_TYPE_TV_SYS_CAPABILITY:
            M_GET_VALIDATE(UINT32);
            _GetTvSysCapability(u1DecId, (UINT32 *) pv_get_info);
            break;

        case AUD_DEC_GET_TYPE_TV_SYS_MASK:
            M_GET_VALIDATE(UINT32);
            _GetTvSysMask(u1DecId, (UINT32 *) pv_get_info);
            break;

        case AUD_DEC_GET_TYPE_TV_AUD_SYS_MASK:
            M_GET_VALIDATE(AUD_DEC_TV_AUD_SYS_T);
            _GetTvAudSysMask(u1DecId, (AUD_DEC_TV_AUD_SYS_T *) pv_get_info);
            break;

        case AUD_DEC_GET_TYPE_AUD_CLIP:
            M_GET_VALIDATE(AUD_DEC_AUD_CLIP_INFO_T);
            _GetAudClip(u1DecId, (AUD_DEC_AUD_CLIP_INFO_T *) pv_get_info);
            break;

        case AUD_DEC_GET_TYPE_PL2_CTRL:
            M_GET_VALIDATE(AUD_DEC_PL2_CTRL_INFO_T);
            _GetPrologic2(u1DecId, (AUD_DEC_PL2_CTRL_INFO_T *) pv_get_info);
            break;

        case AUD_DEC_GET_TYPE_DOLBY_CERTI_MODE:
            M_GET_VALIDATE(BOOL);
            _GetDolbyBanner(u1DecId, (BOOL *)pv_get_info);
            break;

        case AUD_DEC_GET_TYPE_OUTPORT_ADJ:
            M_GET_VALIDATE(AUD_DEC_OUTPORT_ADJ_T);
            _GetOutPortAdj(u1DecId, (AUD_DEC_OUTPORT_ADJ_T *)pv_get_info);
            break;

        case AUD_DEC_GET_TYPE_BBE_MODE:
            M_GET_VALIDATE(AUD_DEC_BBE_MODE_T);
            _GetBbeMode(u1DecId, (AUD_DEC_BBE_MODE_T *)pv_get_info);
            break;

        case AUD_DEC_GET_TYPE_AV_SYNC:
            M_GET_VALIDATE(AUD_DEC_AV_SYNC_INFO_T);
            _GetAvSyncMode(u1DecId, (AUD_DEC_AV_SYNC_INFO_T *)pv_get_info);
            break;

        case AUD_DEC_GET_TYPE_HEADPHONE_COND:
            M_GET_VALIDATE(AUD_DEC_HEADPHONE_COND_T);
            _GetHeadphonePCond(u1DecId, (AUD_DEC_AV_SYNC_INFO_T *)pv_get_info);
            break;

        case AUD_DEC_GET_TYPE_MUTE_EX:
            M_GET_VALIDATE(AUD_DEC_MUTE_EX_INFO_T);
            _GetMuteEx(u1DecId, (AUD_DEC_MUTE_EX_INFO_T *)pv_get_info);
            break;

        default:
            return (RMR_DRV_INV_GET_INFO);
    }

    return (RMR_OK);
}

static INT32 _AudSet(DRV_COMP_ID_T* pt_comp_id,
                    DRV_SET_TYPE_T  e_set_type,
                    const VOID*     pv_set_info,
                    SIZE_T          z_set_info_len)
{
    UINT8 u1DecId = AUD_DEC_MAIN;

    // Check component type and id
    if (pt_comp_id == NULL)
        return (RMR_DRV_INV_SET_INFO);

    if (pt_comp_id->e_type != DRVT_AUD_DEC)
    {
        return (RMR_DRV_INV_SET_INFO);
    }

    //Convert Decoder ID
    if (_AudConvertDecId(pt_comp_id->ui2_id, &u1DecId))
    {
        return (RMR_DRV_INV_SET_INFO);
    }
/*
    if (pt_comp_id->ui2_id > (AUD_DEC_NUM - 1))
    {
        return (RMR_DRV_INV_SET_INFO);
    }
*/
/*
    // Check set_info
    if (pv_set_info == NULL)
    {
        return (RMR_DRV_INV_SET_INFO);
    }
*/
    // Perform set opertion
    switch (e_set_type)
    {
        case AUD_DEC_SET_TYPE_CTRL:
            M_SET_VALIDATE(AUD_DEC_CTRL_T);
            return _AudSetCtrl(u1DecId, (AUD_DEC_CTRL_T)(UINT32)pv_set_info);

        case AUD_DEC_SET_TYPE_NFY_FCT:
            M_SET_VALIDATE(AUD_DEC_NFY_INFO_T);
            _AudSetNotifyInfo(u1DecId, (AUD_DEC_NFY_INFO_T *) pv_set_info);
            break;

        case AUD_DEC_SET_TYPE_INP_MODE:
            M_SET_VALIDATE(BOOL);
            AUD_SetOperationMode(u1DecId, (BOOL)(UINT32)pv_set_info);
            break;

        case AUD_DEC_SET_TYPE_FMT:
            M_SET_VALIDATE(AUD_DEC_FMT_INFO_T);
            if (!_IsAudFmtValid((AUD_DEC_FMT_INFO_T *)pv_set_info))
            {
                return RMR_DRV_INV_SET_INFO;
            }
            _SetFormat(u1DecId, (AUD_DEC_FMT_INFO_T *)pv_set_info);
            break;

        case AUD_DEC_SET_TYPE_VOLUME:
            M_SET_VALIDATE(AUD_DEC_VOLUME_INFO_T);
            _SetVolume(u1DecId, (AUD_DEC_VOLUME_INFO_T *)pv_set_info);
            break;

        case AUD_DEC_SET_TYPE_MUTE:
            M_SET_VALIDATE(AUD_DEC_MUTE_TYPE_T);
            _SetMute(u1DecId, (AUD_DEC_MUTE_TYPE_T)(UINT32)pv_set_info);
            break;

        case AUD_DEC_SET_TYPE_SPDIF:
            M_SET_VALIDATE(AUD_DEC_SPDIF_TYPE_T);
            _SetSpdif(u1DecId, (AUD_DEC_SPDIF_TYPE_T)(UINT32)pv_set_info);
            break;

        case AUD_DEC_SET_TYPE_HDCD_FLTR:
            M_SET_VALIDATE(AUD_DEC_HDCD_FLTR_T);
            _SetHdcdfilter(u1DecId, (AUD_DEC_HDCD_FLTR_T)(UINT32)pv_set_info);
            break;

        case AUD_DEC_SET_TYPE_LS_MODE:
            M_SET_VALIDATE(AUD_DEC_CH_LS_MODE_T);
            _SetLsMode(u1DecId, (AUD_DEC_CH_LS_MODE_T *)pv_set_info);
            break;

        case AUD_DEC_SET_TYPE_SOUND_EFFECT:
            M_SET_VALIDATE(AUD_DEC_SE_INFO_T);
            _SetSoundEffect(u1DecId, (AUD_DEC_SE_INFO_T *)pv_set_info);
            break;

        case AUD_DEC_SET_TYPE_DOWNMIX:
            M_SET_VALIDATE(AUD_DEC_DOWNMIX_MODE_T);
            _SetDownMix(u1DecId, (AUD_DEC_DOWNMIX_MODE_T)(UINT32)pv_set_info);
            break;

        case AUD_DEC_SET_TYPE_DRC:
            M_SET_VALIDATE(AUD_DEC_DRC_T);
            _SetDrc(u1DecId, (AUD_DEC_DRC_T)(UINT32)pv_set_info);
            break;

        case AUD_DEC_SET_TYPE_MEM_BUFFER:
            M_SET_VALIDATE(AUD_DEC_MEM_BUFFER_INFO_T);
            return _SetMemBuffer(u1DecId, (AUD_DEC_MEM_BUFFER_INFO_T *) pv_set_info);

        case AUD_DEC_SET_TYPE_NFY_AUD_CHG:
            M_SET_VALIDATE(BOOL);
            //TODO
            break;

        case AUD_DEC_SET_TYPE_OUT_PORT:
            M_SET_VALIDATE(UINT32);
            _SetOutPort(u1DecId, (UINT32)pv_set_info);
            break;

        case AUD_DEC_SET_TYPE_PCR_ID:
            M_SET_VALIDATE(UINT16);
            AUD_SetStcId(u1DecId, (AV_SYNC_STC_SELECT_T)(UINT32)pv_set_info);
            /* Assume that if you set pcr pid, then need to do AvSync */
            AUD_SetAvSynMode(u1DecId, AV_SYNC_SLAVE);
            break;

        case AUD_DEC_SET_TYPE_CMPSS_MODE:
            M_SET_VALIDATE(AUD_DEC_CMPSS_MODE_T);
            _SetCompressionMode(u1DecId, (AUD_DEC_CMPSS_MODE_T)(UINT32)pv_set_info);
            break;

        case AUD_DEC_SET_TYPE_AGC:
            M_SET_VALIDATE(BOOL);
            _SetAgcFlag(u1DecId, (BOOL)(UINT32)pv_set_info);
            break;

        case AUD_DEC_SET_TYPE_CHNL_DELAY:
            M_SET_VALIDATE(AUD_DEC_CHNL_DELAY_T);
            _SetChlDelay(u1DecId, (AUD_DEC_CHNL_DELAY_T *)pv_set_info);
            break;

        case AUD_DEC_SET_TYPE_COPY_PROTECT:
            M_SET_VALIDATE(AUD_DEC_COPY_PROTECT_T);
            _SetCopyProtect(u1DecId, (AUD_DEC_COPY_PROTECT_T)(UINT32)pv_set_info);
            break;

        case AUD_DEC_SET_TYPE_MTS:
            M_SET_VALIDATE(AUD_DEC_MTS_T);
            _SetMts(u1DecId, (AUD_DEC_MTS_T)(UINT32)pv_set_info);
            break;

         case AUD_DEC_SET_TYPE_SE_EQ_USER_LVL:
            M_SET_VALIDATE(AUD_DEC_SE_QA_USER_LVL_T);
            _SetUserEqLevel(u1DecId, (AUD_DEC_SE_QA_USER_LVL_T *)pv_set_info);
            break;

        case AUD_DEC_SET_TYPE_TV_SYS_MASK:
            M_SET_VALIDATE(UINT32);
            _SetTvSysMask(u1DecId, (UINT32)pv_set_info);
            break;
        case AUD_DEC_SET_TYPE_TV_AUD_SYS_MASK:
            M_SET_VALIDATE(AUD_DEC_TV_AUD_SYS_T);
            _SetTvAudSysMask(u1DecId, (AUD_DEC_TV_AUD_SYS_T *)pv_set_info);
            break;

        case AUD_DEC_SET_TYPE_AUD_CLIP:
            M_SET_VALIDATE(AUD_DEC_AUD_CLIP_T);
            return (_SetAudClip(u1DecId,
                                (AUD_DEC_AUD_CLIP_T *)pv_set_info));

        case AUD_DEC_SET_TYPE_PL2_CTRL:
            M_SET_VALIDATE(AUD_DEC_PL2_CTRL_INFO_T);
            _SetPrologic2(u1DecId,
                          (AUD_DEC_PL2_CTRL_INFO_T *)pv_set_info);
            break;

        case AUD_DEC_SET_TYPE_DOLBY_CERTI_MODE:
            M_SET_VALIDATE(BOOL);
            _SetDolbyBanner(u1DecId, (BOOL)(UINT32)pv_set_info);
            break;

        case AUD_DEC_SET_TYPE_OUTPORT_ADJ:
            M_SET_VALIDATE(AUD_DEC_OUTPORT_ADJ_T);
            _SetOutPortAdj(u1DecId, (AUD_DEC_OUTPORT_ADJ_T *)pv_set_info);
            break;

        case AUD_DEC_SET_TYPE_BBE_MODE:
#ifdef CC_AUD_BBE_SUPPORT
            M_SET_VALIDATE(AUD_DEC_BBE_MODE_T);
            _SetBbeMode(u1DecId, (AUD_DEC_BBE_MODE_T)(UINT32)pv_set_info);
#else
            LOG(1, "ERROR: BBE function is not supported\n");
#endif
            break;

        case AUD_DEC_SET_TYPE_AV_SYNC:
            M_SET_VALIDATE(AUD_DEC_AV_SYNC_INFO_T);
            _SetAvSyncMode(u1DecId, (AUD_DEC_AV_SYNC_INFO_T)(UINT32)pv_set_info);
            break;

        case AUD_DEC_SET_TYPE_MUTE_EX:
            M_SET_VALIDATE(AUD_DEC_MUTE_EX_INFO_T);
            _AudSetMuteEx(u1DecId, (AUD_DEC_MUTE_EX_INFO_T *) pv_set_info);
            break;
        case AUD_DEC_SET_TYPE_PIPVIDEO_PATH_INFO:
            M_SET_VALIDATE(BOOL);
            _SetPIPVideoPathInfo(u1DecId, (BOOL)(UINT32)pv_set_info);
            break;

        default:
            return (RMR_DRV_INV_SET_INFO);
    }

    return (RMR_OK);
}

/*-----------------------------------------------------------------------------
                    Global Function definitions
 ----------------------------------------------------------------------------*/

INT32 MW_AUD_Init(void)
{
    INT32 i4_return;
    DRV_COMP_REG_T tAudCompReg;
    //DRV_COMP_REG_T tDmxCompReg;
    DRV_COMP_FCT_TBL_T tAudFctTbl;
    UINT32 u4Handler = NULL;
    UINT32 u4Handler2 = NULL;
    UINT32 u4Flag;

    /* Init MtkAud driver */
    AUD_Init();

    /* Register middleware notify function */
    _arAudMwNotifyInfo[AUD_DEC_MAIN].pv_tag = 0;
    _arAudMwNotifyInfo[AUD_DEC_MAIN].pf_aud_dec_nfy = NULL;
    _arAudMwNotifyInfo[AUD_DEC_AUX].pv_tag = 0;
    _arAudMwNotifyInfo[AUD_DEC_AUX].pf_aud_dec_nfy = NULL;

    AUD_RegNotifyFunc(&u4Handler, _AudMwNfyFunc);

    /* Register buffer agent data request function */
    AUD_RegBagtReqestFunc(&u4Handler2, _AudBagtSendRequest);

    /* Setup component ID */
    tAudCompReg.e_type = DRVT_AUD_DEC;
    tAudCompReg.e_id_type = ID_TYPE_RANGE;
    tAudCompReg.u.t_range.ui2_first_id = AUD_DEC_MAIN;
    tAudCompReg.u.t_range.ui2_delta_id = 1;
    tAudCompReg.u.t_range.ui2_num_of_ids = AUD_DEC_NUM + 1;  /*there are 3 decoders 0 for MAIN , 1 for AD , 2 for SUB*/
    tAudCompReg.u.t_range.pv_first_tag = NULL;
    tAudCompReg.u.t_range.pv_delta_tag = NULL;
    tAudCompReg.u.t_range.ui1_port = 1;

    /* Setup component function table */
    tAudFctTbl.pf_rm_connect = _AudConnect;
    tAudFctTbl.pf_rm_disconnect = _AudDisconnect;
    tAudFctTbl.pf_rm_get = _AudGet;
    tAudFctTbl.pf_rm_set = _AudSet;

    u4Flag = DRV_FLAG_SINGLE_CONN_ON_OUTPUT;

    /* Register component with the Resource Manager database */
    i4_return = x_rm_reg_comp(&tAudCompReg,
                              1,
                              1,
                              DRVN_AUD_DEC,           //"aud decoder",
                              u4Flag,
                              &tAudFctTbl,
                              NULL,
                              0);
    if (i4_return != RMR_OK)
    {
        return i4_return;
    }

    return (RMR_OK);
}


/*-----------------------------------------------------------------------------
                    Global Function definitions
 ----------------------------------------------------------------------------*/

INT32 d_custom_aud_set (
    DRV_CUSTOM_AUD_TYPE_T     e_aud_type,
    VOID*                     pv_set_type,
    SIZE_T                    z_size,
    BOOL                      b_store
)
{
    // Check input parameters
    switch (e_aud_type)
    {
        case DRV_CUSTOM_AUD_TYPE_VOL_GAIN:
            if (z_size != sizeof(AUD_TYPE_VOL_GAIN_INFO_T))
            {
                return RMR_DRV_INV_SET_INFO;
            }
            break;
        case DRV_CUSTOM_AUD_TYPE_HEADPHONE_VOL:
            if (z_size != sizeof(INT16))
            {
                return RMR_DRV_INV_SET_INFO;
            }
            break;
        case DRV_CUSTOM_AUD_TYPE_AVL_GAIN:
            if (z_size != sizeof(AUD_TYPE_AVL_GAIN_INFO_T))
            {
                return RMR_DRV_INV_SET_INFO;
            }
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_NUM_CHECK:
        case DRV_CUSTOM_AUD_TYPE_MTS_NUM_PILOT:
        case DRV_CUSTOM_AUD_TYPE_MTS_PILOT_HIGH:
        case DRV_CUSTOM_AUD_TYPE_MTS_PILOT_LOW:
        case DRV_CUSTOM_AUD_TYPE_MTS_NUM_SAP:
        case DRV_CUSTOM_AUD_TYPE_MTS_SAP_HIGH:
        case DRV_CUSTOM_AUD_TYPE_MTS_SAP_LOW:
        case DRV_CUSTOM_AUD_TYPE_A2_NUM_CHECK:
        case DRV_CUSTOM_AUD_TYPE_A2_NUM_DOUBLE:
        case DRV_CUSTOM_AUD_TYPE_A2_MONO_WEIGHT:
        case DRV_CUSTOM_AUD_TYPE_A2_STEREO_WEIGHT:
        case DRV_CUSTOM_AUD_TYPE_A2_DUAL_WEIGHT:
        case DRV_CUSTOM_AUD_TYPE_PAL_CORRECT:
        case DRV_CUSTOM_AUD_TYPE_PAL_SYNC_LOOP:
        case DRV_CUSTOM_AUD_TYPE_PAL_ERROR:
        case DRV_CUSTOM_AUD_TYPE_PAL_PARITY_ERROR:
        case DRV_CUSTOM_AUD_TYPE_PAL_NUM_FRAMES:
            if (z_size != sizeof(UINT16))
            {
                return RMR_DRV_INV_SET_INFO;
            }
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_HDEV:
        case DRV_CUSTOM_AUD_TYPE_A2_HDEV:
        case DRV_CUSTOM_AUD_TYPE_PAL_HDEV:
        case DRV_CUSTOM_AUD_TYPE_MTS_CARRIER_SHIFT:
        case DRV_CUSTOM_AUD_TYPE_A2_CARRIER_SHIFT:
        case DRV_CUSTOM_AUD_TYPE_PAL_CARRIER_SHIFT:
        case DRV_CUSTOM_AUD_TYPE_MTS_FM_SATU:
        case DRV_CUSTOM_AUD_TYPE_FM_SATU_MUTE:
        case DRV_CUSTOM_AUD_TYPE_NON_EU:
        case DRV_CUSTOM_AUD_TYPE_MTS_PILOT_OFFSET:
            if (z_size != sizeof(BOOL))
            {
                return RMR_DRV_INV_SET_INFO;
            }
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_FM_MUTE:
        case DRV_CUSTOM_AUD_TYPE_MTS_FM_MUTE_HIGH:
        case DRV_CUSTOM_AUD_TYPE_MTS_FM_MUTE_LOW:
        case DRV_CUSTOM_AUD_TYPE_MTS_FINE_VOLUME:
        case DRV_CUSTOM_AUD_TYPE_SAP_FINE_VOLUME:
        case DRV_CUSTOM_AUD_TYPE_MTS_FM_SATU_HIGH:
        case DRV_CUSTOM_AUD_TYPE_MTS_FM_SATU_LOW:
        case DRV_CUSTOM_AUD_TYPE_SAP_MUTE_LOW:
        case DRV_CUSTOM_AUD_TYPE_SAP_MUTE_HIGH:
        case DRV_CUSTOM_AUD_TYPE_A2_FM_MUTE:
        case DRV_CUSTOM_AUD_TYPE_A2_FM_MUTE_HIGH:
        case DRV_CUSTOM_AUD_TYPE_A2_FM_MUTE_LOW:
        case DRV_CUSTOM_AUD_TYPE_A2_FINE_VOLUME:
        case DRV_CUSTOM_AUD_TYPE_PAL_AM_MUTE:
        case DRV_CUSTOM_AUD_TYPE_PAL_AM_MUTE_HIGH:
        case DRV_CUSTOM_AUD_TYPE_PAL_AM_MUTE_LOW:
        case DRV_CUSTOM_AUD_TYPE_PAL_FM_MUTE:
        case DRV_CUSTOM_AUD_TYPE_PAL_FM_MUTE_HIGH:
        case DRV_CUSTOM_AUD_TYPE_PAL_FM_MUTE_LOW:
        case DRV_CUSTOM_AUD_TYPE_PAL_FINE_VOLUME:
        case DRV_CUSTOM_AUD_TYPE_AM_FINE_VOLUME:
        case DRV_CUSTOM_AUD_TYPE_NICAM_FINE_VOLUME:
        case DRV_CUSTOM_AUD_TYPE_BBE_PRESCALE_LEVEL:
        case DRV_CUSTOM_AUD_TYPE_BBE_PROCESSS:
        case DRV_CUSTOM_AUD_TYPE_BBE_LO_CONTOUR:
        case DRV_CUSTOM_AUD_TYPE_BBE_3D_GAIN:
            if (z_size != sizeof(UINT8))
            {
                return RMR_DRV_INV_SET_INFO;
            }
            break;
        default:
            break;
    }

    // Implementation
    switch (e_aud_type)
    {
        case DRV_CUSTOM_AUD_TYPE_VOL_GAIN:
            if ((((AUD_TYPE_VOL_GAIN_INFO_T *)pv_set_type)->ui4_input_mask & DRV_CUSTOM_AUD_INP_FLAG_ATV) > 0)
            {
                AUD_DspSetSrcVol(AUD_DEC_MAIN, AUD_STREAM_FROM_ANALOG_TUNER, ((AUD_TYPE_VOL_GAIN_INFO_T *)pv_set_type)->i2_vol_gain);
            }
            if ((((AUD_TYPE_VOL_GAIN_INFO_T *)pv_set_type)->ui4_input_mask & DRV_CUSTOM_AUD_INP_FLAG_DTV) > 0)
            {
                AUD_DspSetSrcVol(AUD_DEC_MAIN, AUD_STREAM_FROM_DIGITAL_TUNER, ((AUD_TYPE_VOL_GAIN_INFO_T *)pv_set_type)->i2_vol_gain);
            }
            if (((((AUD_TYPE_VOL_GAIN_INFO_T *)pv_set_type)->ui4_input_mask & DRV_CUSTOM_AUD_INP_FLAG_YPBPR) > 0) ||
                ((((AUD_TYPE_VOL_GAIN_INFO_T *)pv_set_type)->ui4_input_mask & DRV_CUSTOM_AUD_INP_FLAG_VGA) > 0) ||
                ((((AUD_TYPE_VOL_GAIN_INFO_T *)pv_set_type)->ui4_input_mask & DRV_CUSTOM_AUD_INP_FLAG_DVI) > 0))
            {
                AUD_INPUT_ID_T eDvdId;
                INT32 i;
                eDvdId =  AUD_DspGetAvInputSrcDvdId();
                for (i=0 ; i<AUD_INPUT_ID_MAX; i++)
                {
                    if (i == (UINT32)eDvdId)
                    {
                        ;
                    }
                    else
                    {
                        AUD_DspSetAvSrcVol((AUD_INPUT_ID_T)i, ((AUD_TYPE_VOL_GAIN_INFO_T *)pv_set_type)->i2_vol_gain);
                    }
                }
            }
            if ((((AUD_TYPE_VOL_GAIN_INFO_T *)pv_set_type)->ui4_input_mask & DRV_CUSTOM_AUD_INP_FLAG_DVD) > 0)
            {
                AUD_INPUT_ID_T eDvdId;
                eDvdId =  AUD_DspGetAvInputSrcDvdId();
                AUD_DspSetAvSrcVol(eDvdId, ((AUD_TYPE_VOL_GAIN_INFO_T *)pv_set_type)->i2_vol_gain);
            }
            if ((((AUD_TYPE_VOL_GAIN_INFO_T *)pv_set_type)->ui4_input_mask & DRV_CUSTOM_AUD_INP_FLAG_HDMI) > 0)
            {
                AUD_DspSetSrcVol(AUD_DEC_MAIN, AUD_STREAM_FROM_HDMI, ((AUD_TYPE_VOL_GAIN_INFO_T *)pv_set_type)->i2_vol_gain);
            }
            break;
        case DRV_CUSTOM_AUD_TYPE_HEADPHONE_VOL:
            ADAC_HeadphoneVolCtl((INT16)((UINT32)pv_set_type));
            _i2HpVol = (INT16)((UINT32)pv_set_type);
            break;
        case DRV_CUSTOM_AUD_TYPE_AVL_GAIN:
            break;
        /* MTS factory mode */
        case DRV_CUSTOM_AUD_TYPE_MTS_NUM_CHECK:
            AUD_DspSetMtsDetection (AUD_FACTORY_MTS_NUM_CHECK,(UINT16)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_NUM_PILOT:
            AUD_DspSetMtsDetection (AUD_FACTORY_MTS_NUM_PILOT,(UINT16)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_PILOT_HIGH:
            AUD_DspSetMtsDetection (AUD_FACTORY_MTS_PILOT_HIGH,(UINT16)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_PILOT_LOW:
            AUD_DspSetMtsDetection (AUD_FACTORY_MTS_PILOT_LOW,(UINT16)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_NUM_SAP:
            AUD_DspSetMtsDetection (AUD_FACTORY_MTS_NUM_SAP,(UINT16)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_SAP_HIGH:
            AUD_DspSetMtsDetection (AUD_FACTORY_MTS_SAP_HIGH,(UINT16)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_SAP_LOW:
            AUD_DspSetMtsDetection (AUD_FACTORY_MTS_SAP_LOW,(UINT16)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_HDEV:
            AUD_DspSetHdevMode(AUD_ATV_DECODER_MTS, (BOOL)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_CARRIER_SHIFT:
            AUD_DspSetCarrierShiftMode(AUD_ATV_DECODER_MTS, (BOOL)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_FM_SATU:
            AUD_DspSetFMSatuMuteMode(AUD_ATV_DECODER_MTS, (BOOL)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_FM_SATU_HIGH:
            AUD_DspSetFMSatuMuteTh(AUD_FACTORY_SATU_MUTE_HIGHER,(UINT8)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_FM_SATU_LOW:
            AUD_DspSetFMSatuMuteTh(AUD_FACTORY_SATU_MUTE_LOWER,(UINT8)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_FM_MUTE:
            AUD_DspSetFmMute(AUD_ATV_DECODER_MTS,AUD_FACTORY_FM_MUTE,(UINT8)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_FM_MUTE_HIGH:
            AUD_DspSetFmMute(AUD_ATV_DECODER_MTS,AUD_FACTORY_FM_MUTE_HIGH,(UINT8)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_FM_MUTE_LOW:
            AUD_DspSetFmMute(AUD_ATV_DECODER_MTS,AUD_FACTORY_FM_MUTE_LOW,(UINT8)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_FINE_VOLUME:
            AUD_DspSetMtsMonoFineVolume((UINT8)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_SAP_FINE_VOLUME:
            AUD_DspSetSAPFineVolume((UINT8)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_PILOT_OFFSET:
            AUD_DspSetMtsPilotDetection((BOOL)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_SAP_MUTE_LOW:
            AUD_DspSetSapMute(AUD_FACTORY_SAP_MUTE_LOWER,(UINT8)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_SAP_MUTE_HIGH:
            AUD_DspSetSapMute(AUD_FACTORY_SAP_MUTE_HIGHER,(UINT8)((UINT32)pv_set_type));
            break;
        /* A2 factory mode */
        case DRV_CUSTOM_AUD_TYPE_A2_NUM_CHECK:
            AUD_DspSetA2Detection (AUD_FACTORY_A2_NUM_CHECK,(UINT16)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_A2_NUM_DOUBLE:
            AUD_DspSetA2Detection (AUD_FACTORY_A2_NUM_DOUBLE,(UINT16)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_A2_MONO_WEIGHT:
            AUD_DspSetA2Detection (AUD_FACTORY_A2_MONO_WEIGHT,(UINT16)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_A2_STEREO_WEIGHT:
            AUD_DspSetA2Detection (AUD_FACTORY_A2_STEREO_WEIGHT,(UINT16)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_A2_DUAL_WEIGHT:
            AUD_DspSetA2Detection (AUD_FACTORY_A2_DUAL_WEIGHT,(UINT16)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_A2_HDEV:
            AUD_DspSetHdevMode(AUD_ATV_DECODER_A2, (BOOL)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_A2_CARRIER_SHIFT:
            AUD_DspSetCarrierShiftMode(AUD_ATV_DECODER_A2, (BOOL)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_A2_FM_MUTE:
            AUD_DspSetFmMute(AUD_ATV_DECODER_A2,AUD_FACTORY_FM_MUTE,(UINT8)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_A2_FM_MUTE_HIGH:
            AUD_DspSetFmMute(AUD_ATV_DECODER_A2,AUD_FACTORY_FM_MUTE_HIGH,(UINT8)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_A2_FM_MUTE_LOW:
            AUD_DspSetFmMute(AUD_ATV_DECODER_A2,AUD_FACTORY_FM_MUTE_LOW,(UINT8)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_A2_FINE_VOLUME:
            AUD_DspSetA2FineVolume((UINT8)((UINT32)pv_set_type));
            break;
        /* PAL factory mode */
        case DRV_CUSTOM_AUD_TYPE_PAL_CORRECT:
            AUD_DspSetPalDetection (AUD_FACTORY_PAL_CORRECT,(UINT16)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_SYNC_LOOP:
            AUD_DspSetPalDetection (AUD_FACTORY_PAL_SYNC_LOOP,(UINT16)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_ERROR:
            AUD_DspSetPalDetection (AUD_FACTORY_PAL_ERROR,(UINT16)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_PARITY_ERROR:
            AUD_DspSetPalDetection (AUD_FACTORY_PAL_PARITY_ERROR,(UINT16)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_NUM_FRAMES:
            AUD_DspSetPalDetection (AUD_FACTORY_PAL_NUM_FRAMES,(UINT16)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_HDEV:
            AUD_DspSetHdevMode(AUD_ATV_DECODER_PAL, (BOOL)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_AM_MUTE:
            AUD_DspSetAmMute(AUD_FACTORY_PAL_AM_MUTE,(UINT8)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_AM_MUTE_HIGH:
            AUD_DspSetAmMute(AUD_FACTORY_PAL_AM_MUTE_HIGH,(UINT8)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_AM_MUTE_LOW:
            AUD_DspSetAmMute(AUD_FACTORY_PAL_AM_MUTE_LOW,(UINT8)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_CARRIER_SHIFT:
            AUD_DspSetCarrierShiftMode(AUD_ATV_DECODER_PAL, (BOOL)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_FM_MUTE:
            AUD_DspSetFmMute(AUD_ATV_DECODER_PAL,AUD_FACTORY_FM_MUTE,(UINT8)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_FM_MUTE_HIGH:
            AUD_DspSetFmMute(AUD_ATV_DECODER_PAL,AUD_FACTORY_FM_MUTE_HIGH,(UINT8)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_FM_MUTE_LOW:
            AUD_DspSetFmMute(AUD_ATV_DECODER_PAL,AUD_FACTORY_FM_MUTE_LOW,(UINT8)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_FINE_VOLUME:
            AUD_DspSetPALFineVolume((UINT8)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_AM_FINE_VOLUME:
            AUD_DspSetAMFineVolume((UINT8)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_NICAM_FINE_VOLUME:
            AUD_DspSetNicamFineVolume((UINT8)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_FM_SATU_MUTE:
            AUD_DspSetFMSatuMuteMode(AUD_ATV_DECODER_PAL, (BOOL)((UINT32)pv_set_type));
            break;
         /* non-EU factory mode */
        case DRV_CUSTOM_AUD_TYPE_NON_EU:
            AUD_DspSetNonEUMode((BOOL)((UINT32)pv_set_type));
            break;
        /* BBE factory mode */
        case DRV_CUSTOM_AUD_TYPE_BBE_PRESCALE_LEVEL:
            AUD_DspSetBbeLevel((INT8)((INT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_BBE_PROCESSS:
            AUD_DspSetBbeProcess((UINT8)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_BBE_LO_CONTOUR:
            AUD_DspSetBbeLoContour((UINT8)((UINT32)pv_set_type));
            break;
        case DRV_CUSTOM_AUD_TYPE_BBE_3D_GAIN:
            AUD_DspSetBbe3dGain((UINT8)((UINT32)pv_set_type));
            break;
        /* Test Tone */
        case DRV_CUSTOM_AUD_TYPE_TEST_TONE:
            {
                AUD_CLIP_TYPE_T rAudClip;
                UINT8 u1Retry = 0;

                if ( ((AUD_TYPE_TEST_TONE *)pv_set_type)->fgPlayTestTone )
                {
                    while ( AUD_IsDecoderPlay(((AUD_TYPE_TEST_TONE *)pv_set_type)->u1DecId) )
                    {
                        x_thread_delay(10); //FIXME !! wait for audio driver thread into stop state.
                        if (u1Retry > 10)
                        {
                            return RMR_DRV_INV_SET_INFO;
                        }
                        u1Retry++;
                    }

                    {
                        x_memset(&rAudClip, 0, sizeof(AUD_CLIP_TYPE_T));
                        rAudClip.u4OutputChlSelect = ((AUD_TYPE_TEST_TONE *)pv_set_type)->u4OutputChlSelect;
                        rAudClip.u4RepeatCount = 0;
                        rAudClip.fgPlayTestTone = TRUE;
                        rAudClip.u4ToneFreq = ((AUD_TYPE_TEST_TONE *)pv_set_type)->u4ToneFreq ;
                        AUD_SetAudClip(((AUD_TYPE_TEST_TONE *)pv_set_type)->u1DecId, &rAudClip);

                        AUD_PlayAudClip( ((AUD_TYPE_TEST_TONE *)pv_set_type)->u1DecId,
                                ((AUD_TYPE_TEST_TONE *)pv_set_type)->u4OutputChlSelect,
                                ((AUD_TYPE_TEST_TONE *)pv_set_type)->u4ToneFreq );
                    }
                }
                else
                {
                    AUD_StopAudClip( ((AUD_TYPE_TEST_TONE *)pv_set_type)->u1DecId );
                }
                break;
            }
        /* Cutoff Frequency */
        case DRV_CUSTOM_AUD_TYPE_CUT_OFF_FREQ:
            AUD_DspSpkSizeCfg(AUD_DEC_MAIN, (UINT32)pv_set_type);
            break;
	  case DRV_CUSTOM_AUD_VOL_CURVE:
		AUD_DspSetVolumeTable( ((DRV_CUSTOM_AUD_VOL_CURVE_T*)pv_set_type)->ui4_vol_values );
		break;

        default:
            break;
    }

    return RMR_OK;
}

INT32 d_custom_aud_get (
    DRV_CUSTOM_AUD_TYPE_T     e_aud_type,
    VOID*                     pv_get_type,
    SIZE_T*                   pz_size
)
{
    // Check input parameters
    if((pv_get_type == NULL) || (pz_size == NULL))
    {
        return RMR_DRV_INV_GET_INFO;
    }
    // Check pz_size
    switch (e_aud_type)
    {
        case DRV_CUSTOM_AUD_TYPE_VOL_GAIN:
            if (*pz_size < sizeof(AUD_TYPE_VOL_GAIN_INFO_T))
            {
                return RMR_DRV_INV_GET_INFO;
            }
            *pz_size = sizeof(AUD_TYPE_VOL_GAIN_INFO_T);
            break;
        case DRV_CUSTOM_AUD_TYPE_HEADPHONE_VOL:
            if (*pz_size < sizeof(INT16))
            {
                return RMR_DRV_INV_GET_INFO;
            }
            *pz_size = sizeof(INT16);
            break;
        case DRV_CUSTOM_AUD_TYPE_SPECTRUM_INFO: //Andrew Wen 07/1/25 EQ spectrum
            if (*pz_size < sizeof(AUD_TYPE_SPECTRUM_INFO_T))
            {
                return RMR_DRV_INV_GET_INFO;
            }
            *pz_size = sizeof(AUD_TYPE_SPECTRUM_INFO_T);
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_NUM_CHECK:
        case DRV_CUSTOM_AUD_TYPE_MTS_NUM_PILOT:
        case DRV_CUSTOM_AUD_TYPE_MTS_PILOT_HIGH:
        case DRV_CUSTOM_AUD_TYPE_MTS_PILOT_LOW:
        case DRV_CUSTOM_AUD_TYPE_MTS_NUM_SAP:
        case DRV_CUSTOM_AUD_TYPE_MTS_SAP_HIGH:
        case DRV_CUSTOM_AUD_TYPE_MTS_SAP_LOW:
        case DRV_CUSTOM_AUD_TYPE_A2_NUM_CHECK:
        case DRV_CUSTOM_AUD_TYPE_A2_NUM_DOUBLE:
        case DRV_CUSTOM_AUD_TYPE_A2_MONO_WEIGHT:
        case DRV_CUSTOM_AUD_TYPE_A2_STEREO_WEIGHT:
        case DRV_CUSTOM_AUD_TYPE_A2_DUAL_WEIGHT:
        case DRV_CUSTOM_AUD_TYPE_PAL_CORRECT:
        case DRV_CUSTOM_AUD_TYPE_PAL_SYNC_LOOP:
        case DRV_CUSTOM_AUD_TYPE_PAL_ERROR:
        case DRV_CUSTOM_AUD_TYPE_PAL_PARITY_ERROR:
        case DRV_CUSTOM_AUD_TYPE_PAL_NUM_FRAMES:
            if (*pz_size < sizeof(UINT16))
            {
                return RMR_DRV_INV_GET_INFO;
            }
            *pz_size = sizeof(UINT16);
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_HDEV:
        case DRV_CUSTOM_AUD_TYPE_A2_HDEV:
        case DRV_CUSTOM_AUD_TYPE_PAL_HDEV:
        case DRV_CUSTOM_AUD_TYPE_MTS_CARRIER_SHIFT:
        case DRV_CUSTOM_AUD_TYPE_A2_CARRIER_SHIFT:
        case DRV_CUSTOM_AUD_TYPE_PAL_CARRIER_SHIFT:
        case DRV_CUSTOM_AUD_TYPE_MTS_FM_SATU:
        case DRV_CUSTOM_AUD_TYPE_FM_SATU_MUTE:
        case DRV_CUSTOM_AUD_TYPE_NON_EU:
        case DRV_CUSTOM_AUD_TYPE_MTS_PILOT_OFFSET:
            if (*pz_size < sizeof(BOOL))
            {
                return RMR_DRV_INV_GET_INFO;
            }
            *pz_size = sizeof(BOOL);
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_FM_MUTE:
        case DRV_CUSTOM_AUD_TYPE_MTS_FM_MUTE_HIGH:
        case DRV_CUSTOM_AUD_TYPE_MTS_FM_MUTE_LOW:
        case DRV_CUSTOM_AUD_TYPE_MTS_FINE_VOLUME:
        case DRV_CUSTOM_AUD_TYPE_SAP_FINE_VOLUME:
        case DRV_CUSTOM_AUD_TYPE_MTS_FM_SATU_HIGH:
        case DRV_CUSTOM_AUD_TYPE_MTS_FM_SATU_LOW:
        case DRV_CUSTOM_AUD_TYPE_SAP_MUTE_LOW:
        case DRV_CUSTOM_AUD_TYPE_SAP_MUTE_HIGH:
        case DRV_CUSTOM_AUD_TYPE_A2_FM_MUTE:
        case DRV_CUSTOM_AUD_TYPE_A2_FM_MUTE_HIGH:
        case DRV_CUSTOM_AUD_TYPE_A2_FM_MUTE_LOW:
        case DRV_CUSTOM_AUD_TYPE_A2_FINE_VOLUME:
        case DRV_CUSTOM_AUD_TYPE_PAL_AM_MUTE:
        case DRV_CUSTOM_AUD_TYPE_PAL_AM_MUTE_HIGH:
        case DRV_CUSTOM_AUD_TYPE_PAL_AM_MUTE_LOW:
        case DRV_CUSTOM_AUD_TYPE_PAL_FM_MUTE:
        case DRV_CUSTOM_AUD_TYPE_PAL_FM_MUTE_HIGH:
        case DRV_CUSTOM_AUD_TYPE_PAL_FM_MUTE_LOW:
        case DRV_CUSTOM_AUD_TYPE_PAL_FINE_VOLUME:
        case DRV_CUSTOM_AUD_TYPE_AM_FINE_VOLUME:
        case DRV_CUSTOM_AUD_TYPE_NICAM_FINE_VOLUME:
        case DRV_CUSTOM_AUD_TYPE_BBE_PRESCALE_LEVEL:
        case DRV_CUSTOM_AUD_TYPE_BBE_PROCESSS:
        case DRV_CUSTOM_AUD_TYPE_BBE_LO_CONTOUR:
        case DRV_CUSTOM_AUD_TYPE_BBE_3D_GAIN:
            if (*pz_size < sizeof(UINT8))
            {
                return RMR_DRV_INV_GET_INFO;
            }
            *pz_size = sizeof(UINT8);
            break;
        case DRV_CUSTOM_AUD_TYPE_SMART_AUDIO:
            if(e_aud_type == DRV_CUSTOM_AUD_TYPE_SMART_AUDIO)
            {
                return RMR_DRV_INV_GET_INFO;  //always return invalid get
            }
            break;
        default:
            break;
    }

    // Implementation
    switch (e_aud_type)
    {
        case DRV_CUSTOM_AUD_TYPE_VOL_GAIN:
            if ((((AUD_TYPE_VOL_GAIN_INFO_T *)pv_get_type)->ui4_input_mask & DRV_CUSTOM_AUD_INP_FLAG_ATV) > 0)
            {
                ((AUD_TYPE_VOL_GAIN_INFO_T *)pv_get_type)->i2_vol_gain = AUD_DspGetSrcVol(AUD_DEC_MAIN, AUD_STREAM_FROM_ANALOG_TUNER);
            }
            else if ((((AUD_TYPE_VOL_GAIN_INFO_T *)pv_get_type)->ui4_input_mask & DRV_CUSTOM_AUD_INP_FLAG_DTV) > 0)
            {
                ((AUD_TYPE_VOL_GAIN_INFO_T *)pv_get_type)->i2_vol_gain = AUD_DspGetSrcVol(AUD_DEC_MAIN, AUD_STREAM_FROM_DIGITAL_TUNER);
            }
            else if (((((AUD_TYPE_VOL_GAIN_INFO_T *)pv_get_type)->ui4_input_mask & DRV_CUSTOM_AUD_INP_FLAG_YPBPR) > 0) ||
                ((((AUD_TYPE_VOL_GAIN_INFO_T *)pv_get_type)->ui4_input_mask & DRV_CUSTOM_AUD_INP_FLAG_VGA) > 0) ||
                ((((AUD_TYPE_VOL_GAIN_INFO_T *)pv_get_type)->ui4_input_mask & DRV_CUSTOM_AUD_INP_FLAG_DVI) > 0))
            {
                ((AUD_TYPE_VOL_GAIN_INFO_T *)pv_get_type)->i2_vol_gain = AUD_DspGetSrcVol(AUD_DEC_MAIN, AUD_STREAM_FROM_LINE_IN);
            }
            else if ((((AUD_TYPE_VOL_GAIN_INFO_T *)pv_get_type)->ui4_input_mask & DRV_CUSTOM_AUD_INP_FLAG_HDMI) > 0)
            {
                ((AUD_TYPE_VOL_GAIN_INFO_T *)pv_get_type)->i2_vol_gain = AUD_DspGetSrcVol(AUD_DEC_MAIN, AUD_STREAM_FROM_HDMI);
            }
            break;

        case DRV_CUSTOM_AUD_TYPE_HEADPHONE_VOL:
            *(INT16 *)pv_get_type = _i2HpVol;
            break;

        case DRV_CUSTOM_AUD_TYPE_SPECTRUM_INFO:
            {
                INT32 i;
                for (i=0 ; i<15; i++)
                {
                    ((AUD_TYPE_SPECTRUM_INFO_T *)pv_get_type)->ai4_bar[i] = AUD_DspGetSpectrumInfo(AUD_DEC_MAIN,i+1);
                }
            }
            break;
        /* MTS factory mode */
        case DRV_CUSTOM_AUD_TYPE_MTS_NUM_CHECK:
            *(UINT16 *)pv_get_type = AUD_DspGetMtsDetection (AUD_FACTORY_MTS_NUM_CHECK);
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_NUM_PILOT:
            *(UINT16 *)pv_get_type = AUD_DspGetMtsDetection (AUD_FACTORY_MTS_NUM_PILOT);
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_PILOT_HIGH:
            *(UINT16 *)pv_get_type = AUD_DspGetMtsDetection (AUD_FACTORY_MTS_PILOT_HIGH);
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_PILOT_LOW:
            *(UINT16 *)pv_get_type = AUD_DspGetMtsDetection (AUD_FACTORY_MTS_PILOT_LOW);
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_NUM_SAP:
            *(UINT16 *)pv_get_type = AUD_DspGetMtsDetection (AUD_FACTORY_MTS_NUM_SAP);
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_SAP_HIGH:
            *(UINT16 *)pv_get_type = AUD_DspGetMtsDetection (AUD_FACTORY_MTS_SAP_HIGH);
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_SAP_LOW:
            *(UINT16 *)pv_get_type = AUD_DspGetMtsDetection (AUD_FACTORY_MTS_SAP_LOW);
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_HDEV:
            *(BOOL *)pv_get_type = AUD_DspGetHdevMode(AUD_ATV_DECODER_MTS);
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_CARRIER_SHIFT:
            *(BOOL *)pv_get_type = AUD_DspGetCarrierShiftMode(AUD_ATV_DECODER_MTS);
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_FM_SATU:
            *(BOOL *)pv_get_type = AUD_DspGetFMSatuMuteMode(AUD_ATV_DECODER_MTS);
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_FM_SATU_HIGH:
            *(UINT8 *)pv_get_type = AUD_DspGetFMSatuMuteTh(AUD_FACTORY_SATU_MUTE_HIGHER);
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_FM_SATU_LOW:
            *(UINT8 *)pv_get_type = AUD_DspGetFMSatuMuteTh(AUD_FACTORY_SATU_MUTE_LOWER);
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_FM_MUTE:
            *(UINT8 *)pv_get_type = AUD_DspGetFmMute(AUD_ATV_DECODER_MTS,AUD_FACTORY_FM_MUTE);
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_FM_MUTE_HIGH:
            *(UINT8 *)pv_get_type = AUD_DspGetFmMute(AUD_ATV_DECODER_MTS,AUD_FACTORY_FM_MUTE_HIGH);
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_FM_MUTE_LOW:
            *(UINT8 *)pv_get_type = AUD_DspGetFmMute(AUD_ATV_DECODER_MTS,AUD_FACTORY_FM_MUTE_LOW);
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_FINE_VOLUME:
            *(UINT8 *)pv_get_type = AUD_DspGetMtsMonoFineVolume();
            break;
        case DRV_CUSTOM_AUD_TYPE_SAP_FINE_VOLUME:
            *(UINT8 *)pv_get_type = AUD_DspGetSapFineVolume();
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_PILOT_OFFSET:
            *(BOOL*)pv_get_type = AUD_DspGetMtsPilotDetection();
            break;
        case DRV_CUSTOM_AUD_TYPE_SAP_MUTE_LOW:
            *(UINT8 *)pv_get_type = AUD_DspGetSapMute(AUD_FACTORY_SAP_MUTE_LOWER);
            break;
        case DRV_CUSTOM_AUD_TYPE_SAP_MUTE_HIGH:
            *(UINT8 *)pv_get_type = AUD_DspGetSapMute(AUD_FACTORY_SAP_MUTE_HIGHER);
            break;
        /* A2 factory mode */
        case DRV_CUSTOM_AUD_TYPE_A2_NUM_CHECK:
            *(UINT16 *)pv_get_type = AUD_DspGetA2Detection (AUD_FACTORY_A2_NUM_CHECK);
            break;
        case DRV_CUSTOM_AUD_TYPE_A2_NUM_DOUBLE:
            *(UINT16 *)pv_get_type = AUD_DspGetA2Detection (AUD_FACTORY_A2_NUM_DOUBLE);
            break;
        case DRV_CUSTOM_AUD_TYPE_A2_MONO_WEIGHT:
            *(UINT16 *)pv_get_type = AUD_DspGetA2Detection (AUD_FACTORY_A2_MONO_WEIGHT);
            break;
        case DRV_CUSTOM_AUD_TYPE_A2_STEREO_WEIGHT:
            *(UINT16 *)pv_get_type = AUD_DspGetA2Detection (AUD_FACTORY_A2_STEREO_WEIGHT);
            break;
        case DRV_CUSTOM_AUD_TYPE_A2_DUAL_WEIGHT:
            *(UINT16 *)pv_get_type = AUD_DspGetA2Detection (AUD_FACTORY_A2_DUAL_WEIGHT);
            break;
        case DRV_CUSTOM_AUD_TYPE_A2_HDEV:
            *(BOOL *)pv_get_type = AUD_DspGetHdevMode(AUD_ATV_DECODER_A2);
            break;
        case DRV_CUSTOM_AUD_TYPE_A2_CARRIER_SHIFT:
            *(BOOL *)pv_get_type = AUD_DspGetCarrierShiftMode(AUD_ATV_DECODER_A2);
            break;
        case DRV_CUSTOM_AUD_TYPE_A2_FM_MUTE:
            *(UINT8 *)pv_get_type = AUD_DspGetFmMute(AUD_ATV_DECODER_A2,AUD_FACTORY_FM_MUTE);
            break;
        case DRV_CUSTOM_AUD_TYPE_A2_FM_MUTE_HIGH:
            *(UINT8 *)pv_get_type = AUD_DspGetFmMute(AUD_ATV_DECODER_A2,AUD_FACTORY_FM_MUTE_HIGH);
            break;
        case DRV_CUSTOM_AUD_TYPE_A2_FM_MUTE_LOW:
            *(UINT8 *)pv_get_type = AUD_DspGetFmMute(AUD_ATV_DECODER_A2,AUD_FACTORY_FM_MUTE_LOW);
            break;
        case DRV_CUSTOM_AUD_TYPE_A2_FINE_VOLUME:
            *(UINT8 *)pv_get_type = AUD_DspGetA2FineVolume();
            break;
        /* PAL factory mode */
        case DRV_CUSTOM_AUD_TYPE_PAL_CORRECT:
            *(UINT16 *)pv_get_type = AUD_DspGetPalDetection (AUD_FACTORY_PAL_CORRECT);
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_SYNC_LOOP:
            *(UINT16 *)pv_get_type = AUD_DspGetPalDetection (AUD_FACTORY_PAL_SYNC_LOOP);
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_ERROR:
            *(UINT16 *)pv_get_type = AUD_DspGetPalDetection (AUD_FACTORY_PAL_ERROR);
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_PARITY_ERROR:
            *(UINT16 *)pv_get_type = AUD_DspGetPalDetection (AUD_FACTORY_PAL_PARITY_ERROR);
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_NUM_FRAMES:
            *(UINT16 *)pv_get_type = AUD_DspGetPalDetection (AUD_FACTORY_PAL_NUM_FRAMES);
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_HDEV:
           *(BOOL *)pv_get_type =  AUD_DspGetHdevMode(AUD_ATV_DECODER_PAL);
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_AM_MUTE:
            *(UINT8 *)pv_get_type = AUD_DspGetAmMute(AUD_FACTORY_PAL_AM_MUTE);
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_AM_MUTE_HIGH:
            *(UINT8 *)pv_get_type = AUD_DspGetAmMute(AUD_FACTORY_PAL_AM_MUTE_HIGH);
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_AM_MUTE_LOW:
            *(UINT8 *)pv_get_type = AUD_DspGetAmMute(AUD_FACTORY_PAL_AM_MUTE_LOW);
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_CARRIER_SHIFT:
            *(BOOL *)pv_get_type =  AUD_DspGetCarrierShiftMode(AUD_ATV_DECODER_PAL);
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_FM_MUTE:
            *(UINT8 *)pv_get_type = AUD_DspGetFmMute(AUD_ATV_DECODER_PAL,AUD_FACTORY_FM_MUTE);
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_FM_MUTE_HIGH:
            *(UINT8 *)pv_get_type = AUD_DspGetFmMute(AUD_ATV_DECODER_PAL,AUD_FACTORY_FM_MUTE_HIGH);
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_FM_MUTE_LOW:
            *(UINT8 *)pv_get_type = AUD_DspGetFmMute(AUD_ATV_DECODER_PAL,AUD_FACTORY_FM_MUTE_LOW);
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_FINE_VOLUME:
            *(UINT8 *)pv_get_type = AUD_DspGetPALFineVolume();
            break;
        case DRV_CUSTOM_AUD_TYPE_AM_FINE_VOLUME:
            *(UINT8 *)pv_get_type = AUD_DspGetAMFineVolume();
            break;
        case DRV_CUSTOM_AUD_TYPE_NICAM_FINE_VOLUME:
            *(UINT8 *)pv_get_type = AUD_DspGetNicamFineVolume();
            break;
        case DRV_CUSTOM_AUD_TYPE_FM_SATU_MUTE:
            *(BOOL *)pv_get_type = AUD_DspGetFMSatuMuteMode(AUD_ATV_DECODER_PAL);
            break;
         /* non-EU factory mode */
        case DRV_CUSTOM_AUD_TYPE_NON_EU:
            *(BOOL *)pv_get_type =  AUD_DspGetNonEUMode();
            break;
        /* BBE factory mode */
        case DRV_CUSTOM_AUD_TYPE_BBE_PRESCALE_LEVEL:
            *(UINT8 *)pv_get_type = AUD_DspGetBbeLevel();
            break;
        case DRV_CUSTOM_AUD_TYPE_BBE_PROCESSS:
            *(UINT8 *)pv_get_type = AUD_DspGetBbeProcess();
            break;
        case DRV_CUSTOM_AUD_TYPE_BBE_LO_CONTOUR:
            *(UINT8 *)pv_get_type = AUD_DspGetBbeLoContour();
            break;
        case DRV_CUSTOM_AUD_TYPE_BBE_3D_GAIN:
            *(UINT8 *)pv_get_type = AUD_DspGetBbe3dGain();
            break;

        default:
            break;
    }
    return RMR_OK;
}

INT32 d_custom_aud_get_min_max (
    DRV_CUSTOM_AUD_TYPE_T     e_aud_type,
    INT16*                    pui2_min,
    INT16*                    pui2_max
)
{
    // Check input parameters
    if((pui2_min == NULL) || (pui2_max == NULL))
    {
        return RMR_DRV_INV_GET_INFO;
    }

    // Implementation
    switch (e_aud_type)
    {
        case DRV_CUSTOM_AUD_TYPE_VOL_GAIN:
            *pui2_min = -256;
            *pui2_max = 256;
            break;
        case DRV_CUSTOM_AUD_TYPE_HEADPHONE_VOL:
            ADAC_GetHeadphoneVolMaxMin(pui2_max,pui2_min);
            break;
        /* MTS factory mode */
        case DRV_CUSTOM_AUD_TYPE_MTS_NUM_CHECK:
            *pui2_min = 30;
            *pui2_max = 80;
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_NUM_PILOT:
           *pui2_min = 0;
           *pui2_max = 50;
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_PILOT_HIGH:
           *pui2_min = 200;
           *pui2_max = 120;
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_PILOT_LOW:
           *pui2_min = 80;
           *pui2_max = 150;
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_NUM_SAP:
           *pui2_min = 0;
           *pui2_max = 50;
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_SAP_HIGH:
           *pui2_min =150;
           *pui2_max = 210;
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_SAP_LOW:
           *pui2_min = 70;
           *pui2_max = 130;
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_HDEV:
           *pui2_min = 0;
           *pui2_max = 1;
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_CARRIER_SHIFT:
           *pui2_min = 0;
           *pui2_max = 1;
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_FM_SATU:
        case DRV_CUSTOM_AUD_TYPE_FM_SATU_MUTE:
           *pui2_min = 0;
           *pui2_max = 1;
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_FM_SATU_HIGH:
           *pui2_min = 25;
           *pui2_max = 255;
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_FM_SATU_LOW:
           *pui2_min = 1;
           *pui2_max = 25;
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_FM_MUTE:
           *pui2_min = 0;
           *pui2_max = 1;
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_FM_MUTE_HIGH:
           *pui2_min = 20;
           *pui2_max = 80;
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_FM_MUTE_LOW:
           *pui2_min = 110;
           *pui2_max = 180;
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_FINE_VOLUME:
           *pui2_min = 0;
           *pui2_max = 40;
            break;
        case DRV_CUSTOM_AUD_TYPE_SAP_FINE_VOLUME:
           *pui2_min = 0;
           *pui2_max = 40;
            break;
        case DRV_CUSTOM_AUD_TYPE_MTS_PILOT_OFFSET:
           *pui2_min = 0;
           *pui2_max = 1;
            break;
        case DRV_CUSTOM_AUD_TYPE_SAP_MUTE_LOW:
           *pui2_min = 0;
           *pui2_max = 110;
        case DRV_CUSTOM_AUD_TYPE_SAP_MUTE_HIGH:
           *pui2_min = 0;
           *pui2_max = 255;
            break;
        /* A2 factory mode */
        case DRV_CUSTOM_AUD_TYPE_A2_NUM_CHECK:
           *pui2_min = 1;
           *pui2_max = 60;
            break;
        case DRV_CUSTOM_AUD_TYPE_A2_NUM_DOUBLE:
           *pui2_min = 5;
           *pui2_max = 60;
            break;
        case DRV_CUSTOM_AUD_TYPE_A2_MONO_WEIGHT:
           *pui2_min = 1;
           *pui2_max = 20;
            break;
        case DRV_CUSTOM_AUD_TYPE_A2_STEREO_WEIGHT:
           *pui2_min = 1;
           *pui2_max = 20;
            break;
        case DRV_CUSTOM_AUD_TYPE_A2_DUAL_WEIGHT:
           *pui2_min = 1;
           *pui2_max = 20;
            break;
        case DRV_CUSTOM_AUD_TYPE_A2_HDEV:
           *pui2_min = 0;
           *pui2_max = 1;
            break;
        case DRV_CUSTOM_AUD_TYPE_A2_CARRIER_SHIFT:
           *pui2_min = 0;
           *pui2_max = 1;
            break;
        case DRV_CUSTOM_AUD_TYPE_A2_FM_MUTE:
           *pui2_min = 0;
           *pui2_max = 1;
            break;
        case DRV_CUSTOM_AUD_TYPE_A2_FM_MUTE_HIGH:
           *pui2_min = 0;
           *pui2_max = 96;
            break;
        case DRV_CUSTOM_AUD_TYPE_A2_FM_MUTE_LOW:
           *pui2_min = 0;
           *pui2_max = 96;
            break;
        case DRV_CUSTOM_AUD_TYPE_A2_FINE_VOLUME:
           *pui2_min = 0;
           *pui2_max = 40;
            break;
        /* PAL factory mode */
        case DRV_CUSTOM_AUD_TYPE_PAL_CORRECT:
           *pui2_min = 1;
           *pui2_max = 7;
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_SYNC_LOOP:
           *pui2_min = 1;
           *pui2_max = 1001;
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_ERROR:
           *pui2_min = 2;
           *pui2_max = 40;
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_PARITY_ERROR:
           *pui2_min = 1;
           *pui2_max = 255;
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_NUM_FRAMES:
           *pui2_min = 0;
           *pui2_max = 2048;
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_HDEV:
           *pui2_min = 0;
           *pui2_max = 1;
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_AM_MUTE:
           *pui2_min = 0;
           *pui2_max = 1;
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_AM_MUTE_HIGH:
           *pui2_min = 0;
           *pui2_max = 255;
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_AM_MUTE_LOW:
           *pui2_min = 0;
           *pui2_max = 255;
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_CARRIER_SHIFT:
           *pui2_min = 0;
           *pui2_max = 1;
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_FM_MUTE:
           *pui2_min = 0;
           *pui2_max = 1;
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_FM_MUTE_HIGH:
           *pui2_min = 0;
           *pui2_max = 96;
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_FM_MUTE_LOW:
           *pui2_min = 0;
           *pui2_max = 96;
            break;
        case DRV_CUSTOM_AUD_TYPE_PAL_FINE_VOLUME:
           *pui2_min = 0;
           *pui2_max = 40;
            break;
        case DRV_CUSTOM_AUD_TYPE_AM_FINE_VOLUME:
           *pui2_min = 0;
           *pui2_max = 40;
            break;
        case DRV_CUSTOM_AUD_TYPE_NICAM_FINE_VOLUME:
           *pui2_min = 0;
           *pui2_max = 40;
            break;
         /* non-EU factory mode */
        case DRV_CUSTOM_AUD_TYPE_NON_EU:
           *pui2_min = 0;
           *pui2_max = 1;
            break;
        /* BBE factory mode */
        case DRV_CUSTOM_AUD_TYPE_BBE_PRESCALE_LEVEL:
           *pui2_min = -5;
           *pui2_max = 0;
            break;
        case DRV_CUSTOM_AUD_TYPE_BBE_PROCESSS:
           *pui2_min = 3;
           *pui2_max = 12;
            break;
        case DRV_CUSTOM_AUD_TYPE_BBE_LO_CONTOUR:
           *pui2_min = 3;
           *pui2_max = 12;
            break;
        case DRV_CUSTOM_AUD_TYPE_BBE_3D_GAIN:
           *pui2_min = 0;
           *pui2_max = 100;
           break;

        default:
            break;
    }
    return RMR_OK;
}


