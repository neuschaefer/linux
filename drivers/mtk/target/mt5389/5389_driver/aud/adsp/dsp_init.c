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
 * $Date: 2012/10/23 $
 * $RCSfile: dsp_init.c,v $
 * $Revision: #13 $
 *
 *---------------------------------------------------------------------------*/

/** @file dsp_init.c
 *  Brief of file dsp_init.c.
 *  Details of file dsp_init.c (optional).
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#define _DSP_INIT_C

#include "util.h"
#include "x_os.h"
#include "x_chip_id.h"

#include "dsp_common.h"
#include "dsp_intf.h"
#include "dsp_shm.h"
#include "dsp_rg_ctl.h"
#include "dsp_func.h"
#include "d2rc_shm.h"
#include "dsp_table.h"
#include "dsp_data.h"

#include "aud_if.h"
#include "aud_drv.h"
#include "drvcust_if.h"
#include "aud_debug.h"

// Temporaray used -> should be removed
#include "dsp_reg.h"

#include "x_assert.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

#define USE_16BIT_ALIGNMENT
//#define ATV_HDEV_AUTO_SWITCH



//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

// Please refer to drv_default.h or customer's configuration files in \drv_cust\config\XXX.h
#define CUTOFF_FREQ             GetAudioBassMngCutOffFreq()
#define SOFT_MUTE_ORDER         DRVCUST_OptGet(eAudioSoftMuteOrder)
#define IS_BYPASS_PROC()        DRVCUST_OptGet(eAudioBypassPostProc)
//#define AVC_TARGET_LEV          DRVCUST_OptGet(eAudioAvcTarget)
//#define AVC_GAIN_UP             DRVCUST_OptGet(eAudioAvcGainUp)
#define AVC_ATTACK_THRES        DRVCUST_OptGet(eAudioAvcAttackThres)
//#define AVC_ADJUST_RATE         DRVCUST_OptGet(eAudioAvcAdjustRate)
#define IS_SPKCFG_SSLN()        (((u4ReadDspShmDWRD(D_SPKCFG) & 0x1f020) == 0x18000)? 1:0 ) //andrew 07/1/8
//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
extern UINT32 dwGetPEQFsIdx(UINT8 u1Set);
extern UINT32 dwGetPEQCosIdx(UINT8 u1Set);

extern UINT32 GetAudioBassMngCutOffFreq(void);
extern UINT32 GetAudioLRSpkTypeLarge(void);
extern UINT32 GetAudioSLRSpkTypeLarge(void);
extern UINT32 GetAudioCenterSpkTypeLarge(void);
extern UINT32 GetAudioCH910SpkTypeLarge(void);
extern UINT32 GetAudioSubwooferOn(void);
extern UINT32 GetAudioVsClarity(void);
extern UINT32 GetAudioVsWidth(void);
extern UINT32 GetAudioVsLRGain(void);
extern UINT32 GetAudioVsXTalk(void);
extern UINT32 GetAudioVsOutputGain(void);
extern UINT32 GetAudioVsBassGain(void);
extern UINT32 GetAudioVsFO(void);
extern UINT32 GetAudioLimiterMode(void);
extern UINT32 GetAudioLimiterThreshold(void);

//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

static void vCustomOptInit (void);
static UINT8 vFineTuneVolTblIdxChk(UINT8 u1VolTabIdx);

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

void vDspReset(UINT8 u1DecId);
void vDecCommonInit (void);
void vDecCommonInitDec2 (void);
#ifdef CC_MT5391_AUD_3_DECODER
void vDecCommonInitDec3 (void);
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
void vDecCommonInitDec4 (void);
#endif

#ifdef CC_AUD_USE_FLASH_AQ
extern void vDspFlashAQOperation(AUD_AQ_TYPE_T eAQType);
#endif

#ifndef CC_AUD_HDMI_PARSER_2_0_SUPPORT
extern UINT32 _AudGetHDMICase (void);
#endif

/***************************************************************************
     Function : _u4DspGetSpkConfig
  Description : Load speaker size configuration from drv_cust
    Parameter : None
    Return    : None
***************************************************************************/
static UINT32 _u4DspGetSpkConfig(void)
{
    UINT32 u4SpkCfg = 0x17;  // Defualt set as full channel

    if (GetAudioLRSpkTypeLarge())
    {
        u4SpkCfg |= ((UINT32)0x3 << 13);
    }
    if (GetAudioSLRSpkTypeLarge())
    {
        u4SpkCfg |= ((UINT32)0x3 << 15);
    }
    if (GetAudioCenterSpkTypeLarge())
    {
        u4SpkCfg |= ((UINT32)0x1 << 12);
    }
    if (GetAudioCH910SpkTypeLarge())
    {
        u4SpkCfg |= ((UINT32)0x3 << 19);
    }
    if (GetAudioSubwooferOn())
    {
        u4SpkCfg |= ((UINT32)0x1 << 5);
    }

    return u4SpkCfg;
}

UINT8 u1DtsBigEndian = 1;

/***************************************************************************
     Function : vDecCommonInit
  Description : This function is used to init the decoder EACH time when it
                is played. In other words, it is to set the properties which
                is different song by song.
    Parameter : None
    Return    : None
***************************************************************************/
void vDecCommonInit (void)
{
    UINT16 u2DspData = 0;
    UINT32 dDspData;
    TV_AUD_SYS_T u1ChannelSrc;
    UINT16 u2ChannelSrcSet;
    UINT8 u1Temp1, u1Temp2;

    UNUSED(u1Temp1);
    UNUSED(u1Temp2);

    //CTRL3 NUM_1
    // ... should be in switch below
    vWriteDspWORD (ADDR_RC2D_CTRL_3_NUM_1, (UINT16)CTRL_3_NUM_1);
    // Bonding options
    vWriteDspWORD (ADDR_RC2D_BONDING_OPTIONS, u2ReadDspShmWORD (W_BONDING_OPTIONS));
    //Pink Noise Setting
    vWriteDspWORD (ADDR_RC2D_CHANNEL_USE, u2ReadDspShmWORD (W_PINKNOISE));
    //Soft Mute (Max. 7)
    vWriteDspWORD (ADDR_RC2D_VOLUME_SOFT_MUTE_ORDER,
        uReadDspShmBYTE (B_SOFTMUTEORDER));
    //Reset DMX rpnt
	WriteDspCommDram(ADDR_D2RC_RISC_INFO_REG_BUF_PNT , (AUD_READ32(REG_DMX_STR) << 2));

    DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);

    switch (_rDspVars[AUD_DEC_MAIN].bDspStrTyp)
    {
    case PCM_STREAM:
        dDspData = wReadDspWORD (ADDR_D2RC_INPUT_CH_CFG);
        dDspData = u2DspData & ~0xf;
        dDspData = u2DspData | 0x2;  // Hard set cdda channel config
        vWriteDspWORD (ADDR_D2RC_INPUT_CH_CFG, dDspData);
        vWriteDspWORD (ADDR_RC2D_PCM_INPUT_CHANNEL_NUM,
                       u2ReadDspShmWORD (W_PCM_INPUT_CHANNEL_NUM));
        vWriteDspWORD (ADDR_RC2D_PCM_INPUT_CH_ASSIGNMENT,
                       u2ReadDspShmWORD (W_PCM_INPUT_CH_ASSIGNMENT));
        vWriteDspWORD (ADDR_RC2D_PCM_INPUT_TYPE,
                       u2ReadDspShmWORD (W_PCM_INPUT_TYPE));  //0:pcm24 1:IMA 3:MS 2:pcm16 //msb=1: little endian
        vWriteDspWORD (ADDR_RC2D_PCM_INPUT_SAMPLING_RATE,
                       u2ReadDspShmWORD (W_PCM_INPUT_SAMPLING_RATE));
        vWriteDspWORD (ADDR_RC2D_PCM_PREBUF_DELAY_BANK,
                       u2ReadDspShmWORD (W_PCM_PREBUF_DELAY_BANK));
#if 0
        vWriteDspWORD (ADDR_RC2D_ADPCM_BLOCK_ALIGN,
                       u2ReadDspShmWORD (W_ADPCM_BLOCK_ALIGN));
        vWriteDspWORD (ADDR_RC2D_ADPCM_CHANNEL_NUM,
                       u2ReadDspShmWORD (B_ADPCM_CHANNEL_NUM));
#endif
        break;
    case AC3_STREAM:
        //AC3 Karaoke Mode
        vWriteDspWORD (ADDR_RC2D_AC3_KARAOKE_MODE,
                       u2ReadDspShmWORD (W_AC3KARAMOD));
        //AC3 Dual Mono Mode
        vWriteDspWORD (ADDR_RC2D_AC3_DUAL_MODE,
                       u2ReadDspShmWORD (W_AC3DUALMODE));
        //AC3 Compression Mode
        vWriteDspWORD (ADDR_RC2D_AC3_COMPRESS_MODE,
                       u2ReadDspShmWORD (W_AC3COMPMOD));	
        //AC3 Low Boost
        WriteDspCommDram (ADDR_RC2D_AC3_DYNAMIC_LOW,
                          u4ReadDspShmDWRD (D_AC3DYN_LOW));
        //AC3 High Cut
        WriteDspCommDram (ADDR_RC2D_AC3_DYNAMIC_HIGH,
                          u4ReadDspShmDWRD (D_AC3DYN_HIGH));
        // AC3 Auto downmix
        vWriteDspWORD (ADDR_RC2D_AC3_DOWNMIX,
                          uReadDspShmBYTE (B_AC3AUTODNMIX));

#ifdef CC_AUD_SUPPORT_MS10
	    // DDC control
        vWriteDspWORD (ADDR_RC2D_AC3_CONTROL, u2ReadDspShmWORD(W_DDC_CONTROL));
        // DDC associated stream id
        vWriteDspWORD (ADDR_RC2D_DDC_STRMID,  uReadDspShmBYTE (B_DDC_STRMID));
        // DDC mixer
        vWriteDspWORD (ADDR_RC2D_DDC_ASSOC_MIX, uReadDspShmBYTE (B_DDC_ASSOC_MIX));
#endif


#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
        vWriteDspWORD (ADDR_RC2D_AC3_CONTROL, u2ReadDspShmWORD(W_DDC_CONTROL) | (uReadDspShmBYTE(B_DEC_ENDIAN) ? 0x8 : 0x0));
#else
        /* Patch for wrong HDMI content from player
                Do not decode DDP and skip CRC error mute  if HDMI source to avoid this kind of HDMI content.
             */
        if (_AudGetHDMICase())
        {
            // force to DD and skip CRC error mute
            vWriteDspWORD(ADDR_RC2D_AC3_CONTROL, 0x4 | (uReadDspShmBYTE(B_DEC_ENDIAN) ? 0x8 : 0x0));
            LOG (3, "====> HDMI : DD, no CRC error mute\n");
        }
        else
        {
            vWriteDspWORD(ADDR_RC2D_AC3_CONTROL, u2ReadDspShmWORD(W_DDC_CONTROL) | (uReadDspShmBYTE(B_DEC_ENDIAN) ? 0x8 : 0x0));
            LOG (3, "====> HDMI : DDP, with CRC error mute\n");
        }
#endif
        break;

      case COOK_STREAM:
        vWriteDspWORD (ADDR_RC2D_COOK_SAMP_PER_FRAME,
            u2ReadDspShmWORD(W_COOK_SAMP_PER_FRAME));
        vWriteDspWORD (ADDR_RC2D_COOK_CHANNEL_NUM,
            u2ReadDspShmWORD(W_COOK_CHANNEL_NUM));
        vWriteDspWORD (ADDR_RC2D_COOK_INPUT_SAMPLING_RATE,
            u2ReadDspShmWORD(W_COOK_INPUT_SAMPLING_RATE));
        vWriteDspWORD (ADDR_RC2D_COOK_FRM_SZ_IN_BYTES,
            u2ReadDspShmWORD(W_COOK_FRM_SZ_IN_BYTES));
        vWriteDspWORD (ADDR_RC2D_COOK_REGION_NUM,
            u2ReadDspShmWORD(W_COOK_REGION_NUM));
        vWriteDspWORD (ADDR_RC2D_COOK_COUPLING_START_REGN,
            u2ReadDspShmWORD(W_COOK_COUPLING_START_REGN));
        vWriteDspWORD (ADDR_RC2D_COOK_COUPLING_QBITS,
            u2ReadDspShmWORD(W_COOK_COUPLING_QBITS));
        break;

    case MPEG12_STREAM:
        // MPEG sync mode
        vWriteDspWORD (ADDR_RC2D_MPEG_SYNC_MODE, 0x01);
        // MPEG deemphasis mode
        vWriteDspWORD (ADDR_RC2D_MPEG_DEEMPHASIS_MODE, 0x00);
         // MPEG CRC mode
        vWriteDspWORD (ADDR_RC2D_MPEG_CRC_MODE,
                       u2ReadDspShmWORD (W_MPEGERRDET));
        // MPEG DRC flag
        vWriteDspWORD (ADDR_RC2D_MPEG_DRC_FLAG, 0x00);
        break;
    case MPEG3_STREAM:
        // Normal play
        vWriteDspWORD (ADDR_RC2D_MP3_NORMAL_PLAY, 0x00);
        // MP3 sync mode
        vWriteDspWORD (ADDR_RC2D_MP3_SYNC_MODE, 0x01);
        // MP3 deemphasis mode
        vWriteDspWORD (ADDR_RC2D_MP3_DEEMPHASIS_MODE, 0x00);
        // MP3 CRC mode
        vWriteDspWORD (ADDR_RC2D_MP3_CRC_MODE,
                       u2ReadDspShmWORD (W_MPEGERRDET));
        // MP3 preparsing mode
        vWriteDspWORD (ADDR_RC2D_MP3_PREPARSING_MODE, 0x00);
        // MP3 smooth mode
        vWriteDspWORD (ADDR_RC2D_MP3_SMOOTH_FLAG, 0x00);
        // MP3 quality flag
        vWriteDspWORD (ADDR_RC2D_MP3_QUALITY_FLAG, 0x200);
        break;
#ifdef DATA_DISC_WMA_SUPPORT
    case WMA_STREAM:
        //WNA Packet No
        vWriteDspWORD (ADDR_RC2D_WMA_PACKET_NO,
                       u2ReadDspShmWORD (W_WMA_PACKET_NO));
        vWriteDspWORD (ADDR_RC2D_WMA_HEADER_SOURCE,
                       u2ReadDspShmWORD (W_WMA_HEADER_SOURCE));
        WriteDspCommDram (ADDR_RC2D_WMA_PACKET_COUNT,
                       ((u4ReadDspShmDWRD (D_WMA_PACKET_COUNT))<<8));
        WriteDspCommDram (ADDR_RC2D_WMA_PACKET_SIZE,
                       ((u4ReadDspShmDWRD (D_WMA_PACKET_SIZE))<<8));
        vWriteDspWORD (ADDR_RC2D_WMA_VERSION,
                       u2ReadDspShmWORD (W_WMA_VERSION));
        vWriteDspWORD (ADDR_RC2D_WMA_NUM_CHANNELS,
                       u2ReadDspShmWORD (W_WMA_NUM_CH));
        WriteDspCommDram (ADDR_RC2D_WMA_SAMPLE_PER_SECOND,
                       ((u4ReadDspShmDWRD (D_WMA_SAMPLE_PER_SEC))<<8));
        WriteDspCommDram (ADDR_RC2D_WMA_BYTES_PER_SECOND,
                       ((u4ReadDspShmDWRD (D_WMA_BYTE_PER_SEC))<<8));
        WriteDspCommDram (ADDR_RC2D_WMA_BLOCK_SIZE,
                       ((u4ReadDspShmDWRD (D_WMA_BLOCK_SIZE))<<8));
        vWriteDspWORD (ADDR_RC2D_WMA_ENCODER_OPTIONS,
                       u2ReadDspShmWORD (W_WMA_ENCODER_OPTION));
        //koro : if issue play without setting audio format (seek mode), wma need initial table
        vLoadDspDRAMTable (AUD_WMA_DEC1);
        vWriteDspWORD (ADDR_RC2D_DOWNLOAD_TABLE_FLAG, 0x1);

        break;
#endif

    case WMAPRO_STREAM:

        vWriteDspWORD (ADDR_RC2D_CTRL_3_NUM_1, (UINT16)0xC);
        //WNA Packet No
        vWriteDspWORD (ADDR_RC2D_WMAPRO_PACKET_NO,
                       u2ReadDspShmWORD (W_WMAPRO_PACKET_NO));
        vWriteDspWORD (ADDR_RC2D_WMAPRO_HEADER_SOURCE,
                       u2ReadDspShmWORD (W_WMAPRO_HEADER_SOURCE));
        vWriteDspWORD (ADDR_RC2D_WMAPRO_NUM_CHANNELS,
                       u2ReadDspShmWORD (W_WMAPRO_NUM_CH));
        WriteDspCommDram (ADDR_RC2D_WMAPRO_SAMPLE_PER_SECOND,
                       ((u4ReadDspShmDWRD (D_WMAPRO_SAMPLE_PER_SEC))<<8));
        WriteDspCommDram (ADDR_RC2D_WMAPRO_BYTES_PER_SECOND,
                       ((u4ReadDspShmDWRD (D_WMAPRO_BYTE_PER_SEC))<<8));
        WriteDspCommDram (ADDR_RC2D_WMAPRO_BLOCK_SIZE,
                       ((u4ReadDspShmDWRD (D_WMAPRO_BLOCK_SIZE))<<8));
        vWriteDspWORD (ADDR_RC2D_WMAPRO_ENCODER_OPTIONS,
                       u2ReadDspShmWORD (W_WMAPRO_ENCODER_OPTION));
        WriteDspCommDram (ADDR_RC2D_WMAPRO_CH_MASK,
                       ((u4ReadDspShmDWRD (D_WMAPRO_CH_MASK))<<8));
        vWriteDspWORD (ADDR_RC2D_WMAPRO_VALID_BITS,
                       u2ReadDspShmWORD (W_WMAPRO_VALID_BITS));
        vWriteDspWORD (ADDR_RC2D_WMAPRO_ADV_OPTION,
                       u2ReadDspShmWORD (W_WMAPRO_ADV_OPTION));
        WriteDspCommDram (ADDR_RC2D_WMAPRO_ADV_OPTION2,
                       ((u4ReadDspShmDWRD (D_WMAPRO_ADV_OPTION2))<<8));

        break;

    case PINK_NOISE_STREAM:
        //Pink Noise Setting
        /* 20041220: For testing, hard coded here, remove in the Future */
        //vWriteDspWORD (ADDR_RC2D_CHANNEL_USE, 0x06ff);
        //vWriteDspWORD (ADDR_RC2D_CHANNEL_USE, 0x4002);
        vWriteDspWORD (ADDR_RC2D_CHANNEL_USE, u2ReadDspShmWORD (W_PINKNOISE));
        break;
#ifdef DSP_SUPPORT_NPTV
    case NTSC_STREAM:
        vWriteDspWORD (ADDR_DEM_ADC_GAIN,u2ReadDspShmWORD (W_ADC_GAIN));
        vWriteDspWORD (ADDR_DEM_USER_MODE,u2ReadDspShmWORD (W_USER_MODE));
        vWriteDspWORD (ADDR_DEM_SOURCE_MODE,u2ReadDspShmWORD (W_SOURCE_MODE));
        vWriteDspWORD (ADDR_MTS_NUM_CHECK, u2ReadDspShmWORD (W_MTS_NUM_CHECK));
        vWriteDspWORD (ADDR_MTS_STEREO_MID,u2ReadDspShmWORD (W_MTS_STEREO_MID));
        vWriteDspWORD (ADDR_MTS_STEREO_CON_MID, u2ReadDspShmWORD (W_MTS_STEREO_CON_MID));
        vWriteDspWORD (ADDR_MTS_NUM_PILOT, u2ReadDspShmWORD (W_MTS_NUM_PILOT));
        vWriteDspWORD (ADDR_MTS_NUM_SAP, u2ReadDspShmWORD (W_MTS_NUM_SAP));
        vWriteDspWORD (ADDR_MTS_SAP_MID, u2ReadDspShmWORD (W_MTS_SAP_MID));
        vWriteDspWORD (ADDR_MTS_SAP_CON_MID,u2ReadDspShmWORD (W_MTS_SAP_CON_MID));
#ifdef CC_AUD_MTS_OUTPUT_HDEV_GAIN_SUPPORT
        vWriteDspWORD (ADDR_HDEV_MODE, (uReadDspShmBYTE (B_HDEV_MODE) & 0x10)>>3);  // set bit 1
#else
        vWriteDspWORD (ADDR_HDEV_MODE, (uReadDspShmBYTE (B_HDEV_MODE) & 0x04)>>2);
#endif
        vWriteDspWORD (ADDR_CARRIER_SHIFT_MODE, (uReadDspShmBYTE (B_CARRIER_SHIFT_MODE) & 0x04)>>2);
        vWriteDspWORD (ADDR_FM_SATU_MUTE_MODE,  (uReadDspShmBYTE (B_FM_SATU_MUTE_MODE)& 0x04)>>2);
        vWriteDspWORD (ADDR_FM_CARRIER_MUTE_MODE, (uReadDspShmBYTE (B_FM_MUTE_MODE) & 0x04)>>2);
        vWriteDspWORD (ADDR_FM_CARRIER_MUTE_THRESHOLD_HIGHT, uReadDspShmBYTE (B_FM_MUTE_THRESHOLD_HIGHT_MTS));
        vWriteDspWORD (ADDR_FM_CARRIER_MUTE_THRESHOLD_LOW, uReadDspShmBYTE (B_FM_MUTE_THRESHOLD_LOW_MTS));
        vWriteDspWORD (ADDR_DEM_MTS_FINE_TUNE_VOLUME,u4ReadDspShmDWRD  (D_MTS_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (ADDR_DEM_SAP_FINE_TUNE_VOLUME,u4ReadDspShmDWRD (D_SAP_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (ADDR_MTS_PILOT_OFFSET_DETECTION,uReadDspShmBYTE (B_MTS_PILOT_OFFSET_DETECTION));
        vWriteDspWORD (ADDR_MTS_MUTE_SAP_LOW,uReadDspShmBYTE (B_MTS_MUTE_SAP_LOW));
        vWriteDspWORD (ADDR_MTS_MUTE_SAP_HIGH,uReadDspShmBYTE (B_MTS_MUTE_SAP_HIGH));
        vWriteDspWORD (ADDR_MTS_SATU_MUTE_HIGH,uReadDspShmBYTE (B_SATU_MUTE_THRESHOLD_HIGH));
        vWriteDspWORD (ADDR_MTS_SATU_MUTE_LOW,uReadDspShmBYTE (B_SATU_MUTE_THRESHOLD_LOW));
        vWriteDspWORD (ADDR_DEFAULT_MTS_MODE, 0);  // no default mode
        vWriteDspWORD (ADDR_MTS_SAP_HP_NOISE_LOW,uReadDspShmBYTE (B_SAP_HP_MUTE_LOW));
        vWriteDspWORD (ADDR_MTS_SAP_HP_NOISE_HIGH,uReadDspShmBYTE (B_SAP_HP_MUTE_HIGH));
        vWriteDspWORD (ADDR_MTS_SAP_FILTER_SEL,uReadDspShmBYTE (B_SAP_FILTER_SEL));
        vWriteDspWORD (ADDR_MTS_SAP_RATIO,uReadDspShmBYTE (B_SAP_RATIO));
        break;
    case FMFM_STREAM:
        vWriteDspWORD (ADDR_DEM_ADC_GAIN,u2ReadDspShmWORD (W_ADC_GAIN));
        vWriteDspWORD (ADDR_DEM_USER_MODE,u2ReadDspShmWORD (W_USER_MODE));
        vWriteDspWORD (ADDR_DEM_SOURCE_MODE,u2ReadDspShmWORD (W_SOURCE_MODE));
        vWriteDspWORD (ADDR_FMFM_NUM_CHECK,u2ReadDspShmWORD (W_FMFM_NUM_CHECK));
        vWriteDspWORD (ADDR_FMFM_NUM_DOUBLE_CHECK,u2ReadDspShmWORD (W_FMFM_NUM_DOUBLE_CHECK));
        vWriteDspWORD (ADDR_FMFM_MONO_WEIGHT,u2ReadDspShmWORD (W_FMFM_MONO_WEIGHT));
        vWriteDspWORD (ADDR_FMFM_STEREO_WEIGHT,u2ReadDspShmWORD (W_FMFM_STEREO_WEIGHT));
        vWriteDspWORD (ADDR_FMFM_DUAL_WEIGHT,u2ReadDspShmWORD (W_FMFM_DUAL_WEIGHT));
        vWriteDspWORD (ADDR_FMFM_DETECTION_CONFID, u2ReadDspShmWORD (W_FMFM_DETECT_CONFID));
        vWriteDspWORD (ADDR_HDEV_MODE,  (uReadDspShmBYTE (B_HDEV_MODE) & 0x08)>>3);
        vWriteDspWORD (ADDR_CARRIER_SHIFT_MODE, (uReadDspShmBYTE (B_CARRIER_SHIFT_MODE) & 0x08)>>3);
        vWriteDspWORD (ADDR_FM_SATU_MUTE_MODE,  (uReadDspShmBYTE (B_FM_SATU_MUTE_MODE)& 0x08)>>3);
        vWriteDspWORD (ADDR_FM_CARRIER_MUTE_MODE,  (uReadDspShmBYTE (B_FM_MUTE_MODE) & 0x08)>>3);
        vWriteDspWORD (ADDR_FM_CARRIER_MUTE_THRESHOLD_HIGHT,  uReadDspShmBYTE (B_FM_MUTE_THRESHOLD_HIGHT_EIAJ));
        vWriteDspWORD (ADDR_FM_CARRIER_MUTE_THRESHOLD_LOW,  uReadDspShmBYTE (B_FM_MUTE_THRESHOLD_LOW_EIAJ));
       //vWriteDspWORD (ADDR_DEM_FMFM_MONO_FINE_TUNE_VOLUME,u4ReadDspShmDWRD  (D_FMFM_MONO_FINE_TUNE_VOLUME)>>8);
       // vWriteDspWORD (ADDR_DEM_FMFM_DUAL_FINE_TUNE_VOLUME,u4ReadDspShmDWRD (D_FMFM_DUAL_FINE_TUNE_VOLUME)>>8);
        break;
    case A2_STREAM:
        vWriteDspWORD (ADDR_DEM_ADC_GAIN,u2ReadDspShmWORD (W_ADC_GAIN));
        vWriteDspWORD (ADDR_DEM_USER_MODE, u2ReadDspShmWORD (W_USER_MODE));
        u1ChannelSrc = DSP_ReturnChannelSource ();
        u2ChannelSrcSet = SOURCE_A2_BG;
        if (u1ChannelSrc == SV_A2_BG)
        {
            u2ChannelSrcSet = SOURCE_A2_BG;
        }
        else if (u1ChannelSrc == SV_A2_DK1)
        {
            u2ChannelSrcSet = SOURCE_A2_DK1;
        }
        else if (u1ChannelSrc == SV_A2_DK2)
        {
            u2ChannelSrcSet = SOURCE_A2_DK2;
        }
        else if (u1ChannelSrc == SV_A2_DK3)
        {
            u2ChannelSrcSet = SOURCE_A2_DK3;
        }
        else if (u1ChannelSrc == SV_NTSC_M)
        {
            u2ChannelSrcSet = SOURCE_NTSC_M;
        }
        vWriteDspWORD (ADDR_DEM_SOURCE_MODE, u2ChannelSrcSet);
        vWriteDspWORD (ADDR_A2_NUM_CHECK, u2ReadDspShmWORD (W_A2_NUM_CHECK));
        vWriteDspWORD (ADDR_A2_NUM_DOUBLE_CHECK,u2ReadDspShmWORD (W_A2_NUM_DOUBLE_CHECK));
        vWriteDspWORD (ADDR_A2_MONO_WEIGHT,u2ReadDspShmWORD (W_A2_MONO_WEIGHT));
        vWriteDspWORD (ADDR_A2_STEREO_WEIGHT,u2ReadDspShmWORD (W_A2_STEREO_WEIGHT));
        vWriteDspWORD (ADDR_A2_DUAL_WEIGHT,u2ReadDspShmWORD (W_A2_DUAL_WEIGHT));
        vWriteDspWORD (ADDR_HDEV_MODE, (uReadDspShmBYTE (B_HDEV_MODE) & 0x02)>>1);
        vWriteDspWORD (ADDR_CARRIER_SHIFT_MODE, (uReadDspShmBYTE (B_CARRIER_SHIFT_MODE) & 0x02)>>1);
        vWriteDspWORD (ADDR_FM_SATU_MUTE_MODE,  (uReadDspShmBYTE (B_FM_SATU_MUTE_MODE)& 0x02)>>1);
        vWriteDspWORD (ADDR_FM_CARRIER_MUTE_MODE,  (uReadDspShmBYTE (B_FM_MUTE_MODE) & 0x02)>>1);
        vWriteDspWORD (ADDR_FM_CARRIER_MUTE_THRESHOLD_HIGHT,  uReadDspShmBYTE (B_FM_MUTE_THRESHOLD_HIGHT_A2));
        vWriteDspWORD (ADDR_FM_CARRIER_MUTE_THRESHOLD_LOW,  uReadDspShmBYTE (B_FM_MUTE_THRESHOLD_LOW_A2));
        vWriteDspWORD (ADDR_DEM_PAL_FINE_TUNE_VOLUME,u4ReadDspShmDWRD  (D_A2_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (ADDR_MANUAL_MTS_SW, uReadDspShmBYTE (B_MANUAL_MTS_SW));
        //vWriteDspWORD (ADDR_MANUAL_OUTPUT_MATRIX, uReadDspShmBYTE (B_MANUAL_OUTPUT_MATRIX));
        vWriteDspWORD (ADDR_FM_NON_EU_MODE,uReadDspShmBYTE  (B_NON_EU_FM_MODE));
        if (u2ChannelSrcSet == SOURCE_NTSC_M)
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
        //for dynamic detection control
        vWriteDspWORD(ADDR_DYNAMIC_DETECTIO_SWITCH,uReadDspShmBYTE(B_DYNAMIC_DETECTIO_SWITCH));
        break;
    case PAL_STREAM:
        vWriteDspWORD (ADDR_DEM_ADC_GAIN,u2ReadDspShmWORD (W_ADC_GAIN));
        vWriteDspWORD (ADDR_DEM_USER_MODE,u2ReadDspShmWORD (W_USER_MODE));
        vWriteDspWORD (ADDR_DEM_ENFORCED_MODE,u2ReadDspShmWORD (W_ENFORCED_MODE));
        u1ChannelSrc = DSP_ReturnChannelSource ();
        u2ChannelSrcSet = SOURCE_PAL_I; // default value
        if (u1ChannelSrc == SV_PAL_BG)
        {
            u2ChannelSrcSet = SOURCE_PAL_BG;
        }
        else if (u1ChannelSrc == SV_PAL_DK)
        {
            u2ChannelSrcSet = SOURCE_PAL_DK;
        }
        else if (u1ChannelSrc == SV_SECAM_L)
        {
            u2ChannelSrcSet = SOURCE_SECAM_L;
        }
        else if (u1ChannelSrc == SV_SECAM_BG)
        {
            u2ChannelSrcSet = SOURCE_SECAM_BG;
        }
        else if (u1ChannelSrc == SV_SECAM_DK)
        {
            u2ChannelSrcSet = SOURCE_SECAM_DK;
        }
        else if (u1ChannelSrc == SV_SECAM_L_PLUM)
        {
            u2ChannelSrcSet = SOURCE_SECAM_L;
        }
        vWriteDspWORD (ADDR_DEM_SOURCE_MODE,u2ChannelSrcSet);
        vWriteDspWORD (ADDR_PAL_CORRECT_THRESHOLD,u2ReadDspShmWORD (W_PAL_CORRECT_THRESHOLD));
        vWriteDspWORD (ADDR_PAL_TOTAL_SYNC1_LOOP,u2ReadDspShmWORD (W_PAL_TOTAL_SYNC1_LOOP));
        vWriteDspWORD (ADDR_PAL_ERR_THRESHOLD,u2ReadDspShmWORD (W_PAL_ERR_THRESHOLD));
        vWriteDspWORD (ADDR_PAL_PARITY_ERR_THRESHOLD,u2ReadDspShmWORD (W_PAL_PARITY_ERR_THRESHOLD));
        vWriteDspWORD (ADDR_PAL_EVERY_NUM_FRAMES,u2ReadDspShmWORD (W_PAL_EVERY_NUM_FRAMES));
#ifdef ATV_HDEV_AUTO_SWITCH
        vWriteDspWORD (ADDR_HDEV_MODE, ((uReadDspShmBYTE (B_HDEV_MODE) & 0x01)| 0x03));
#else
#ifdef CC_AUD_PAL_NEW_HDEV_SUPPORT
        vWriteDspWORD (ADDR_HDEV_MODE, ((uReadDspShmBYTE (B_HDEV_MODE) & 0x01) << 2) | 0x1);  // set bit 0 and 2
#else
        vWriteDspWORD (ADDR_HDEV_MODE, (uReadDspShmBYTE (B_HDEV_MODE) & 0x01));
#endif
#endif
        vWriteDspWORD (ADDR_CARRIER_SHIFT_MODE, (uReadDspShmBYTE (B_CARRIER_SHIFT_MODE) & 0x01));
        vWriteDspWORD (ADDR_FM_SATU_MUTE_MODE,  (uReadDspShmBYTE (B_FM_SATU_MUTE_MODE)& 0x01));
        //for AM carrier sense mute
        vWriteDspWORD (ADDR_SECAM_L_MUTE_MODE,  uReadDspShmBYTE (B_AM_MUTE_MODE));
        vWriteDspWORD (ADDR_SECAM_L_MUTE_THRESHOLD_HIGHT, uReadDspShmBYTE (B_AM_MUTE_THRESHOLD_HIGHT));
        vWriteDspWORD (ADDR_SECAM_L_MUTE_THRESHOLD_LOW,uReadDspShmBYTE (B_AM_MUTE_THRESHOLD_LOW));
        //for FM carrier sense mute
        vWriteDspWORD (ADDR_FM_CARRIER_MUTE_MODE,  (uReadDspShmBYTE (B_FM_MUTE_MODE) & 0x01));
        vWriteDspWORD (ADDR_FM_CARRIER_MUTE_THRESHOLD_HIGHT, uReadDspShmBYTE (B_FM_MUTE_THRESHOLD_HIGHT_PAL));
        vWriteDspWORD (ADDR_FM_CARRIER_MUTE_THRESHOLD_LOW,uReadDspShmBYTE (B_FM_MUTE_THRESHOLD_LOW_PAL));
        //for EU/NON-EU FM
        vWriteDspWORD (ADDR_FM_NON_EU_MODE,uReadDspShmBYTE  (B_NON_EU_FM_MODE));
        //for NICAM I boost or not
        vWriteDspWORD(ADDR_NICAM_I_BOOST_FLAG,uReadDspShmBYTE(B_NICAM_I_BOOST_FLAG));
         //for PAL FM BAD SIGNAL CHECK
	 //vWriteDspWORD (ADDR_PAL_HDEV_BAD_SIGNAL_CHECK,uReadDspShmBYTE (B_FM_BAD_SIGNAL));
        //for dynamic detection control
        vWriteDspWORD(ADDR_DYNAMIC_DETECTIO_SWITCH,uReadDspShmBYTE(B_DYNAMIC_DETECTIO_SWITCH));	    
        vWriteDspWORD (ADDR_DEM_PAL_FINE_TUNE_VOLUME,u4ReadDspShmDWRD  (D_PAL_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (ADDR_DEM_NICAM_FINE_TUNE_VOLUME,u4ReadDspShmDWRD (D_NICAM_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (ADDR_DEM_AM_FINE_TUNE_VOLUME,u4ReadDspShmDWRD  (D_AM_FINE_TUNE_VOLUME)>>8);
        break;
    case DETECTOR_STREAM:
        vWriteDspWORD (ADDR_DETECTOR_DEFAULT_MODE, uReadDspShmBYTE(B_DETECT_DEFAULT));
        vWriteDspWORD (ADDR_DETECTOR_START_FLAG, 1); // default turn on detection
        WriteDspCommDram (ADDR_DETECTOR_FM_NOISE_TH, u4ReadDspShmDWRD (D_DETECTOR_FM_NOISE_TH));
        break;
    case FMRDODET_STREAM:
        vWriteDspWORD (ADDR_DETECTOR_DEFAULT_MODE, uReadDspShmBYTE(B_DETECT_DEFAULT));
        vWriteDspWORD (ADDR_DETECTOR_START_FLAG, 1); // default turn on detection
        WriteDspCommDram (ADDR_FM_RDO_DET_INBAND_TH, u4ReadDspShmDWRD (D_FM_RDO_INBAND_TH));
        WriteDspCommDram (ADDR_FM_RDO_DET_NOISE_TH, u4ReadDspShmDWRD (D_FM_RDO_NOISE_TH));
        break;
    case FMRDO_STREAM:
        vWriteDspWORD (ADDR_DEM_ENFORCED_MODE, 0x1); // enable enforced
        vWriteDspWORD (ADDR_HDEV_MODE, (uReadDspShmBYTE (B_HDEV_MODE) & 0x00));
        vWriteDspWORD (ADDR_CARRIER_SHIFT_MODE, 0x0); // disable CFO
        vWriteDspWORD (ADDR_DEM_FMRADIO_FINE_TUNE_VOLUME,u4ReadDspShmDWRD  (D_FMRADIO_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (ADDR_DEM_USER_MODE,u2ReadDspShmWORD (W_USER_MODE));
        break;
#endif
    case G711DEC_STREAM:
        vWriteDspWORD (ADDR_RC2D_PCM_INPUT_TYPE,
                       u2ReadDspShmWORD (W_PCM_INPUT_TYPE));  //0:pcm24 1:IMA 3:MS 2:pcm16 //msb=1: little endian
        vWriteDspWORD (ADDR_RC2D_PCM_INPUT_SAMPLING_RATE,
                       u2ReadDspShmWORD (W_PCM_INPUT_SAMPLING_RATE));

        break;
    case G711ENC_STREAM:
        vWriteDspWORD (ADDR_RC2D_PCM_INPUT_TYPE,
                       u2ReadDspShmWORD (W_PCM_INPUT_TYPE));  //0:pcm24 1:IMA 3:MS 2:pcm16 //msb=1: little endian

        break;
    case LPCM_STREAM:
        #if 0
        // This is OK for lpcm 16 bit (from VOB file)
        WriteDspCommDram(ADDR_RC2D_LPCM_CHANNEL_ASSIGNMENT, 0x100);
        WriteDspCommDram(ADDR_RC2D_LPCM_FREQUENCY, 0x000000);
        WriteDspCommDram(ADDR_RC2D_LPCM_BIT_SHIFT, 0);
        WriteDspCommDram(ADDR_RC2D_LPCM_DRC_VALUE, 0x08000);
        WriteDspCommDram(ADDR_RC2D_LPCM_Q, 0x3000);
        WriteDspCommDram(ADDR_RC2D_LPCM_DRC_FLAG, 0x00);
        WriteDspCommDram(ADDR_RC2D_LPCM_DECODING_TYPE, 0xd00);
        /*
        WriteDspCommDram(ADDR_RC2D_LPCM_ADPCM_DEC_TYPE, 0x1100);
        WriteDspCommDram(ADDR_RC2D_LPCM_ADPCM_BITS_PER_SAMPLE, 0x400);
        WriteDspCommDram(ADDR_RC2D_ADPCM_BLOCK_ALIGN, 0x080000);
        WriteDspCommDram(ADDR_RC2D_ADPCM_CHANNEL_NUM, 0x200);
        */
        #else
        u1Temp1 = (u2ReadDspShmWORD(W_LPCM_FREQUENCY)>>8) & 0xFF;
        u1Temp2 = u2ReadDspShmWORD(W_LPCM_FREQUENCY) & 0xFF;

        vWriteDspWORD(ADDR_RC2D_LPCM_CHANNEL_ASSIGNMENT, uReadDspShmBYTE(B_LPCM_CH_ASSIGNMENT));
        //WriteDspCommDram(ADDR_RC2D_LPCM_FREQUENCY,  u2ReadDspShmWORD(W_LPCM_FREQUENCY));
        WriteDspCommDram(ADDR_RC2D_LPCM_FREQUENCY,  (u1Temp1<<12)+u1Temp2);
        vWriteDspWORD(ADDR_RC2D_LPCM_BIT_SHIFT,  uReadDspShmBYTE(B_LPCM_BIT_SHIFT));
        vWriteDspWORD(ADDR_RC2D_LPCM_DRC_VALUE,  uReadDspShmBYTE(B_LPCM_DRC_VALUE));
        WriteDspCommDram(ADDR_RC2D_LPCM_Q,  u2ReadDspShmWORD(W_LPCM_Q_VALUE));
        vWriteDspWORD(ADDR_RC2D_LPCM_DRC_FLAG,  uReadDspShmBYTE(B_LPCM_DRC_FLAG));
        vWriteDspWORD(ADDR_RC2D_LPCM_DECODING_TYPE,  u2ReadDspShmWORD(W_LPCM_DECODING_TYPE));

        if (uReadDspShmBYTE(W_ADPCM_DECODING_TYPE)!=0) //ADPCM
        {
            vWriteDspWORD(ADDR_RC2D_LPCM_ADPCM_DEC_TYPE, u2ReadDspShmWORD(W_ADPCM_DECODING_TYPE));
            vWriteDspWORD(ADDR_RC2D_LPCM_ADPCM_BITS_PER_SAMPLE, uReadDspShmBYTE(B_ADPCM_BIT_PERSAMPLE));
            vWriteDspWORD(ADDR_RC2D_ADPCM_BLOCK_ALIGN, u2ReadDspShmWORD(W_ADPCM_BLOCK_ALIGN));
            vWriteDspWORD(ADDR_RC2D_ADPCM_CHANNEL_NUM, uReadDspShmBYTE(B_ADPCM_CHANNEL_NUM));
        }
        #endif
        break;
    case DTSDEC_STREAM:
		if (AUD_GetDTSInfo())
		{
			//Big Endian
		    vWriteDspWORD (ADDR_RC2D_DTS_DRC_FLAG, 0xffd9);
	    }
		else
		{
			//Little Endian
			vWriteDspWORD (ADDR_RC2D_DTS_DRC_FLAG, 0xfffb);
		}
		//AUD_SetDTSInfo(DEC_BIG_ENDIAN); //reset to Big Endian
	    vWriteDspWORD (ADDR_RC2D_DTS_LFE_mixto_FRONT, uReadDspShmBYTE(B_DTS_BC_LFE_MIXTO_FRONT));
	    vWriteDspWORD (ADDR_RC2D_DTS_DRC_ONOFF, uReadDspShmBYTE(B_DTS_BC_DRC));
	    vWriteDspWORD (ADDR_RC2D_DTS_DIALNORM_ONOFF, uReadDspShmBYTE(B_DTS_BC_Dialnrom));
	    WriteDspCommDram (ADDR_RC2D_DTS_DRC_LOW, u4ReadDspShmDWRD (D_DTS_BC_DRC_LOW));
        WriteDspCommDram (ADDR_RC2D_DTS_DRC_MEDIUM, u4ReadDspShmDWRD (D_DTS_BC_DRC_MEDIUM));
        WriteDspCommDram (ADDR_RC2D_DTS_DRC_HIGH, u4ReadDspShmDWRD (D_DTS_BC_DRC_HIGH));
        WriteDspCommDram (ADDR_RC2D_DTS_DIALNORM_LOW, u4ReadDspShmDWRD (D_DTS_BC_Dialnorm_LOW));
        WriteDspCommDram (ADDR_RC2D_DTS_DIALNORM_MEDIUM, u4ReadDspShmDWRD (D_DTS_BC_Dialnorm_MEDIUM));
        WriteDspCommDram (ADDR_RC2D_DTS_DIALNORM_HIGH, u4ReadDspShmDWRD (D_DTS_BC_Dialnorm_HIGH));

        break;

    case AAC_STREAM:
#if 1//ndef CC_AUD_SUPPORT_MS10

        vWriteDspWORD (ADDR_RC2D_AAC_DOWNMIX_LEVEL,
                       AAC_DEFAULT_DOWNMIX_LEVEL);
        vWriteDspWORD (ADDR_RC2D_AAC_TARGET_LEVEL,
                       AAC_TARGET_LEVEL);
#else
        vWriteDspWORD (ADDR_RC2D_DDT_DUAL_MODE,
                       uReadDspShmBYTE (B_DDT_DUALMODE));
        vWriteDspWORD (ADDR_RC2D_DDT_DRC,
                       uReadDspShmBYTE (B_DDT_COMPVAL));
        vWriteDspWORD (ADDR_RC2D_DDT_DMX_MODE,
                       uReadDspShmBYTE (B_DDT_COMPMODE));
#endif
        vWriteDspWORD (ADDR_RC2D_DDT_CONTROL,
                       uReadDspShmBYTE(B_DDT_CONTROL));
        break;

    #if 1///def CC_VORBIS_SUPPORT
    case VORBIS_STREAM:
        vWriteDspWORD(ADDR_RC2D_VORBIS_PACKET_NO, u2ReadDspShmWORD(W_VORBISPACKET));
        vWriteDspWORD(ADDR_D2RC_VORBIS_TABLE_OK, 0);
        vWriteDspWORD(ADDR_D2RC_VORBIS_ABS_GRANULE_POS_LL16, 0);
        vWriteDspWORD(ADDR_D2RC_VORBIS_ABS_GRANULE_POS_LH16, 0);
        vWriteDspWORD(ADDR_D2RC_VORBIS_ABS_GRANULE_POS_OK, 0);
        /// Dec1: 8+30
        vWriteDspWORD (ADDR_RC2D_AOUT_BANK_256_NUM, (8+30));   ///[Joel] Vorbis decoder dram only support 8+30 Bank
        break;
    #endif
    default:
        break;
    }
}

/***************************************************************************
     Function : vDecCommonInitDec2 (for second decoder)
  Description : This function is used to init the decoder EACH time when it
                is played. In other words, it is to set the properties which
                is different song by song.
    Parameter : None
    Return    : None
***************************************************************************/
void vDecCommonInitDec2 (void)
{
    UINT32 u4DspData;
    TV_AUD_SYS_T u1ChannelSrc;
    UINT16 u2ChannelSrcSet;
    UINT8 u1Temp1, u1Temp2;

    UNUSED(u1Temp1);
    UNUSED(u1Temp2);

    //CTRL5 NUM_1
    // ... should be in switch below
    vWriteDspWORD (ADDR_RC2D_CTRL_5_NUM_1, (UINT16)CTRL_5_NUM_1);
    // Bonding options
    vWriteDspWORD (ADDR_RC2D_BONDING_OPTIONS, u2ReadDspShmWORD (W_BONDING_OPTIONS));
    //Pink Noise Setting
    vWriteDspWORD (ADDR_RC2D_CHANNEL_USE_DEC2, u2ReadDspShmWORD (W_PINKNOISE));

    //Soft Mute (Max. 7)
    vWriteDspWORD (ADDR_RC2D_VOLUME_SOFT_MUTE_ORDER_DEC2,
        u2ReadDspShmWORD (B_SOFTMUTEORDER_DEC2));

    DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);

    switch (_rDspVars[AUD_DEC_AUX].bDspStrTyp)
    {
    case PCM_STREAM:
        u4DspData = wReadDspWORD (ADDR_D2RC_INPUT_CH_CFG_DEC2);
        u4DspData = u4DspData & ~0xf;
        u4DspData = u4DspData | 0x2;  // Hard set cdda channel config
        vWriteDspWORD (ADDR_D2RC_INPUT_CH_CFG_DEC2, u4DspData);
        vWriteDspWORD (ADDR_RC2D_PCM_INPUT_CHANNEL_NUM_DEC2,
                       u2ReadDspShmWORD (W_PCM_INPUT_CHANNEL_NUM_DEC2));
        vWriteDspWORD (ADDR_RC2D_PCM_INPUT_CH_ASSIGNMENT_DEC2,
                       u2ReadDspShmWORD (W_PCM_INPUT_CH_ASSIGNMENT_DEC2));
        vWriteDspWORD (ADDR_RC2D_PCM_INPUT_TYPE_DEC2,
                       u2ReadDspShmWORD (W_PCM_INPUT_TYPE_DEC2));  //0:pcm24 1:IMA 3:MS 2:pcm16 //msb=1: little endian
        vWriteDspWORD (ADDR_RC2D_PCM_INPUT_SAMPLING_RATE_DEC2,
                       u2ReadDspShmWORD (W_PCM_INPUT_SAMPLING_RATE_DEC2));
#if 0
        vWriteDspWORD (ADDR_RC2D_ADPCM_BLOCK_ALIGN_DEC2,
                       u2ReadDspShmWORD (W_ADPCM_BLOCK_ALIGN));
        vWriteDspWORD (ADDR_RC2D_ADPCM_CHANNEL_NUM_DEC2,
                       u2ReadDspShmWORD (B_ADPCM_CHANNEL_NUM));
#endif
        break;
    case AC3_STREAM:
        //AC3 Karaoke Mode
        vWriteDspWORD (ADDR_RC2D_AC3_KARAOKE_MODE_DEC2,
                       u2ReadDspShmWORD (W_AC3KARAMOD_DEC2));
        //AC3 Dual Mono Mode
        vWriteDspWORD (ADDR_RC2D_AC3_DUAL_MODE_DEC2,
                       u2ReadDspShmWORD (W_AC3DUALMODE_DEC2));
        //AC3 Compression Mode
        vWriteDspWORD (ADDR_RC2D_AC3_COMPRESS_MODE_DEC2,
                       u2ReadDspShmWORD (W_AC3COMPMOD_DEC2));
        //AC3 Low Boost
        WriteDspCommDram (ADDR_RC2D_AC3_DYNAMIC_LOW_DEC2,
                          u4ReadDspShmDWRD (D_AC3DYN_LOW_DEC2));
        //AC3 High Cut
        WriteDspCommDram (ADDR_RC2D_AC3_DYNAMIC_HIGH_DEC2,
                          u4ReadDspShmDWRD (D_AC3DYN_HIGH_DEC2));
        // AC3 Auto downmix
        WriteDspCommDram (ADDR_RC2D_AC3_DOWNMIX_DEC2,
                          uReadDspShmBYTE (B_AC3AUTODNMIX_DEC2));
        // DDC control
        vWriteDspWORD (ADDR_RC2D_AC3_CONTROL_DEC2, u2ReadDspShmWORD(W_DDC_CONTROL_DEC2));
#ifdef CC_AUD_SUPPORT_MS10
        // DDC associated stream id
        vWriteDspWORD (ADDR_RC2D_DDC_STRMID_DEC2,  uReadDspShmBYTE (B_DDC_STRMID_DEC2));
        // DDC mixer
        vWriteDspWORD (ADDR_RC2D_DDC_ASSOC_MIX_DEC2, uReadDspShmBYTE (B_DDC_ASSOC_MIX_DEC2));
#endif //CC_AUD_SUPPORT_MS10
#ifdef CC_AUD_SUPPORT_MS10
	// XMX mixer in PCM decoder
        vWriteDspWORD (ADDR_RC2D_XMX_FLAG, 0x1);
#endif
        break;
    case MPEG12_STREAM:
        /* 20041220: For testing, hard coded here, remove in the Future */
        // MPEG sync no error checking
        vWriteDspWORD (ADDR_RC2D_MPEG_SYNC_MODE_DEC2, 0x1);
        //MPEG Error Detection
        vWriteDspWORD (ADDR_RC2D_CRC_FLAG,
                       u2ReadDspShmWORD (W_MPEGERRDET));
        break;
    case MPEG3_STREAM:
        // Normal play
        vWriteDspWORD (ADDR_RC2D_MP3_NORMAL_PLAY_DEC2, 0x00);
        // MP3 sync mode
        vWriteDspWORD (ADDR_RC2D_MP3_SYNC_MODE_DEC2, 0x01);
        // MP3 deemphasis mode
        vWriteDspWORD (ADDR_RC2D_MP3_DEEMPHASIS_MODE_DEC2, 0x00);
        // MP3 preparsing mode
        vWriteDspWORD (ADDR_RC2D_MP3_PREPARSING_MODE_DEC2, 0x00);
        break;
#ifdef DSP_SUPPORT_NPTV
    case NTSC_STREAM:
        vWriteDspWORD (ADDR_DEM_ADC_GAIN,u2ReadDspShmWORD (W_ADC_GAIN));
        vWriteDspWORD (ADDR_DEM_USER_MODE,u2ReadDspShmWORD (W_USER_MODE));
        vWriteDspWORD (ADDR_DEM_SOURCE_MODE,u2ReadDspShmWORD (W_SOURCE_MODE));
        vWriteDspWORD (ADDR_MTS_NUM_CHECK, u2ReadDspShmWORD (W_MTS_NUM_CHECK));
        vWriteDspWORD (ADDR_MTS_STEREO_MID,u2ReadDspShmWORD (W_MTS_STEREO_MID));
        vWriteDspWORD (ADDR_MTS_STEREO_CON_MID, u2ReadDspShmWORD (W_MTS_STEREO_CON_MID));
        vWriteDspWORD (ADDR_MTS_NUM_PILOT, u2ReadDspShmWORD (W_MTS_NUM_PILOT));
        vWriteDspWORD (ADDR_MTS_NUM_SAP, u2ReadDspShmWORD (W_MTS_NUM_SAP));
        vWriteDspWORD (ADDR_MTS_SAP_MID, u2ReadDspShmWORD (W_MTS_SAP_MID));
        vWriteDspWORD (ADDR_MTS_SAP_CON_MID,u2ReadDspShmWORD (W_MTS_SAP_CON_MID));
#ifdef CC_AUD_MTS_OUTPUT_HDEV_GAIN_SUPPORT
        vWriteDspWORD (ADDR_HDEV_MODE, (uReadDspShmBYTE (B_HDEV_MODE) & 0x10)>>3);  // set bit 1
#else
        vWriteDspWORD (ADDR_HDEV_MODE, (uReadDspShmBYTE (B_HDEV_MODE) & 0x04)>>2);
#endif
        vWriteDspWORD (ADDR_CARRIER_SHIFT_MODE, (uReadDspShmBYTE (B_CARRIER_SHIFT_MODE) & 0x04)>>2);
        vWriteDspWORD (ADDR_FM_SATU_MUTE_MODE,  (uReadDspShmBYTE (B_FM_SATU_MUTE_MODE)& 0x04)>>2);
        vWriteDspWORD (ADDR_FM_CARRIER_MUTE_MODE, (uReadDspShmBYTE (B_FM_MUTE_MODE) & 0x04)>>2);
        vWriteDspWORD (ADDR_FM_CARRIER_MUTE_THRESHOLD_HIGHT, uReadDspShmBYTE (B_FM_MUTE_THRESHOLD_HIGHT_MTS));
        vWriteDspWORD (ADDR_FM_CARRIER_MUTE_THRESHOLD_LOW, uReadDspShmBYTE (B_FM_MUTE_THRESHOLD_LOW_MTS));
        vWriteDspWORD (ADDR_DEM_MTS_FINE_TUNE_VOLUME,u4ReadDspShmDWRD  (D_MTS_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (ADDR_DEM_SAP_FINE_TUNE_VOLUME,u4ReadDspShmDWRD (D_SAP_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (ADDR_MTS_PILOT_OFFSET_DETECTION,uReadDspShmBYTE (B_MTS_PILOT_OFFSET_DETECTION));
        vWriteDspWORD (ADDR_MTS_MUTE_SAP_LOW,uReadDspShmBYTE (B_MTS_MUTE_SAP_LOW));
        vWriteDspWORD (ADDR_MTS_MUTE_SAP_HIGH,uReadDspShmBYTE (B_MTS_MUTE_SAP_HIGH));
        vWriteDspWORD (ADDR_MTS_SATU_MUTE_HIGH,uReadDspShmBYTE (B_SATU_MUTE_THRESHOLD_HIGH));
        vWriteDspWORD (ADDR_MTS_SATU_MUTE_LOW,uReadDspShmBYTE (B_SATU_MUTE_THRESHOLD_LOW));
        vWriteDspWORD (ADDR_DEFAULT_MTS_MODE, 0);  // no default mode
        vWriteDspWORD (ADDR_MTS_SAP_HP_NOISE_LOW,uReadDspShmBYTE (B_SAP_HP_MUTE_LOW));
        vWriteDspWORD (ADDR_MTS_SAP_HP_NOISE_HIGH,uReadDspShmBYTE (B_SAP_HP_MUTE_HIGH));
        vWriteDspWORD (ADDR_MTS_SAP_FILTER_SEL,uReadDspShmBYTE (B_SAP_FILTER_SEL));
        vWriteDspWORD (ADDR_MTS_SAP_RATIO,uReadDspShmBYTE (B_SAP_RATIO));
#ifdef CC_AUD_MTS_OUTPUT_LEVEL_GAIN_SUPPORT
        vWriteDspWORD (ADDR_MTS_OUTPUT_LEVEL_GAIN, u4ReadDspShmDWRD(D_MTS_OUTPUT_LEVEL_GAIN));
#endif

        // Decoder 2
        vWriteDspWORD (ADDR_DEM_SOURCE_MODE_DEC2,u2ReadDspShmWORD (W_SOURCE_MODE));
        vWriteDspWORD (ADDR_DEM_USER_MODE_DEC2,u2ReadDspShmWORD (W_USER_MODE));
        vWriteDspWORD (ADDR_DEM_ADC_GAIN_DEC2,u2ReadDspShmWORD (W_ADC_GAIN));
        vWriteDspWORD (ADDR_DEM_SAP_FINE_TUNE_VOLUME_DEC2,u4ReadDspShmDWRD (D_SAP_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (ADDR_DEM_MTS_FINE_TUNE_VOLUME_DEC2,u4ReadDspShmDWRD  (D_MTS_FINE_TUNE_VOLUME)>>8);
        break;
    case FMFM_STREAM:
        vWriteDspWORD (ADDR_DEM_ADC_GAIN,u2ReadDspShmWORD (W_ADC_GAIN));
        vWriteDspWORD (ADDR_DEM_USER_MODE,u2ReadDspShmWORD (W_USER_MODE));
        vWriteDspWORD (ADDR_DEM_SOURCE_MODE,u2ReadDspShmWORD (W_SOURCE_MODE));
        vWriteDspWORD (ADDR_FMFM_NUM_CHECK,u2ReadDspShmWORD (W_FMFM_NUM_CHECK));
        vWriteDspWORD (ADDR_FMFM_NUM_DOUBLE_CHECK,u2ReadDspShmWORD (W_FMFM_NUM_DOUBLE_CHECK));
        vWriteDspWORD (ADDR_FMFM_MONO_WEIGHT,u2ReadDspShmWORD (W_FMFM_MONO_WEIGHT));
        vWriteDspWORD (ADDR_FMFM_STEREO_WEIGHT,u2ReadDspShmWORD (W_FMFM_STEREO_WEIGHT));
        vWriteDspWORD (ADDR_FMFM_DUAL_WEIGHT,u2ReadDspShmWORD (W_FMFM_DUAL_WEIGHT));
        vWriteDspWORD (ADDR_FMFM_DETECTION_CONFID, u2ReadDspShmWORD (W_FMFM_DETECT_CONFID));
        vWriteDspWORD (ADDR_HDEV_MODE,  (uReadDspShmBYTE (B_HDEV_MODE) & 0x08)>>3);
        vWriteDspWORD (ADDR_CARRIER_SHIFT_MODE, (uReadDspShmBYTE (B_CARRIER_SHIFT_MODE) & 0x08)>>3);
        vWriteDspWORD (ADDR_FM_SATU_MUTE_MODE,  (uReadDspShmBYTE (B_FM_SATU_MUTE_MODE)& 0x08)>>3);
        vWriteDspWORD (ADDR_FM_CARRIER_MUTE_MODE,  (uReadDspShmBYTE (B_FM_MUTE_MODE) & 0x08)>>3);
        vWriteDspWORD (ADDR_FM_CARRIER_MUTE_THRESHOLD_HIGHT,  uReadDspShmBYTE (B_FM_MUTE_THRESHOLD_HIGHT_EIAJ));
        vWriteDspWORD (ADDR_FM_CARRIER_MUTE_THRESHOLD_LOW,  uReadDspShmBYTE (B_FM_MUTE_THRESHOLD_LOW_EIAJ));
        //vWriteDspWORD (ADDR_DEM_FMFM_MONO_FINE_TUNE_VOLUME,u4ReadDspShmDWRD  (D_FMFM_MONO_FINE_TUNE_VOLUME)>>8);
        //vWriteDspWORD (ADDR_DEM_FMFM_DUAL_FINE_TUNE_VOLUME,u4ReadDspShmDWRD (D_FMFM_DUAL_FINE_TUNE_VOLUME)>>8);

        // Decoder 2
        vWriteDspWORD (ADDR_DEM_SOURCE_MODE_DEC2,u2ReadDspShmWORD (W_SOURCE_MODE));
        vWriteDspWORD (ADDR_DEM_USER_MODE_DEC2,u2ReadDspShmWORD (W_USER_MODE));
        vWriteDspWORD (ADDR_DEM_ADC_GAIN_DEC2,u2ReadDspShmWORD (W_ADC_GAIN));
        vWriteDspWORD (ADDR_FMFM_NUM_CHECK_DEC2,u2ReadDspShmWORD (W_FMFM_NUM_CHECK));
        vWriteDspWORD (ADDR_FMFM_NUM_DOUBLE_CHECK_DEC2,u2ReadDspShmWORD (W_FMFM_NUM_DOUBLE_CHECK));
        vWriteDspWORD (ADDR_FMFM_MONO_WEIGHT_DEC2,u2ReadDspShmWORD (W_FMFM_MONO_WEIGHT));
        vWriteDspWORD (ADDR_FMFM_STEREO_WEIGHT_DEC2,u2ReadDspShmWORD (W_FMFM_STEREO_WEIGHT));
        vWriteDspWORD (ADDR_FMFM_DUAL_WEIGHT_DEC2,u2ReadDspShmWORD (W_FMFM_DUAL_WEIGHT));
        vWriteDspWORD (ADDR_FMFM_DETECTION_CONFID_DEC2, u2ReadDspShmWORD (W_FMFM_DETECT_CONFID));
        break;
    case A2_STREAM:
        vWriteDspWORD (ADDR_DEM_ADC_GAIN,u2ReadDspShmWORD (W_ADC_GAIN));
        vWriteDspWORD (ADDR_DEM_USER_MODE, u2ReadDspShmWORD (W_USER_MODE));
        u1ChannelSrc = DSP_ReturnChannelSource ();
        u2ChannelSrcSet = SOURCE_A2_BG;
        if (u1ChannelSrc == SV_A2_BG)
        {
            u2ChannelSrcSet = SOURCE_A2_BG;
        }
        else if (u1ChannelSrc == SV_A2_DK1)
        {
            u2ChannelSrcSet = SOURCE_A2_DK1;
        }
        else if (u1ChannelSrc == SV_A2_DK2)
        {
            u2ChannelSrcSet = SOURCE_A2_DK2;
        }
        else if (u1ChannelSrc == SV_A2_DK3)
        {
            u2ChannelSrcSet = SOURCE_A2_DK3;
        }
        vWriteDspWORD (ADDR_DEM_SOURCE_MODE, u2ChannelSrcSet);
        vWriteDspWORD (ADDR_A2_NUM_CHECK, u2ReadDspShmWORD (W_A2_NUM_CHECK));
        vWriteDspWORD (ADDR_A2_NUM_DOUBLE_CHECK,u2ReadDspShmWORD (W_A2_NUM_DOUBLE_CHECK));
        vWriteDspWORD (ADDR_A2_MONO_WEIGHT,u2ReadDspShmWORD (W_A2_MONO_WEIGHT));
        vWriteDspWORD (ADDR_A2_STEREO_WEIGHT,u2ReadDspShmWORD (W_A2_STEREO_WEIGHT));
        vWriteDspWORD (ADDR_A2_DUAL_WEIGHT,u2ReadDspShmWORD (W_A2_DUAL_WEIGHT));
        vWriteDspWORD (ADDR_HDEV_MODE, (uReadDspShmBYTE (B_HDEV_MODE) & 0x02)>>1);
        vWriteDspWORD (ADDR_CARRIER_SHIFT_MODE, (uReadDspShmBYTE (B_CARRIER_SHIFT_MODE) & 0x02)>>1);
        vWriteDspWORD (ADDR_FM_SATU_MUTE_MODE,  (uReadDspShmBYTE (B_FM_SATU_MUTE_MODE)& 0x02)>>1);
        vWriteDspWORD (ADDR_FM_CARRIER_MUTE_MODE,  (uReadDspShmBYTE (B_FM_MUTE_MODE) & 0x02)>>1);
        vWriteDspWORD (ADDR_FM_CARRIER_MUTE_THRESHOLD_HIGHT,  uReadDspShmBYTE (B_FM_MUTE_THRESHOLD_HIGHT_A2));
        vWriteDspWORD (ADDR_FM_CARRIER_MUTE_THRESHOLD_LOW,  uReadDspShmBYTE (B_FM_MUTE_THRESHOLD_LOW_A2));
        vWriteDspWORD (ADDR_DEM_PAL_FINE_TUNE_VOLUME,u4ReadDspShmDWRD  (D_A2_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (ADDR_MANUAL_MTS_SW, uReadDspShmBYTE (B_MANUAL_MTS_SW));
        //vWriteDspWORD (ADDR_MANUAL_OUTPUT_MATRIX, uReadDspShmBYTE (B_MANUAL_OUTPUT_MATRIX));
        vWriteDspWORD (ADDR_FM_NON_EU_MODE,uReadDspShmBYTE  (B_NON_EU_FM_MODE));
        // Decoder 2
        vWriteDspWORD (ADDR_DEM_SOURCE_MODE_DEC2, u2ChannelSrcSet);
        vWriteDspWORD (ADDR_DEM_USER_MODE_DEC2, u2ReadDspShmWORD (W_USER_MODE));
        vWriteDspWORD (ADDR_DEM_ADC_GAIN_DEC2,u2ReadDspShmWORD (W_ADC_GAIN));
        vWriteDspWORD (ADDR_DEM_PAL_FINE_TUNE_VOLUME_DEC2,u4ReadDspShmDWRD  (D_A2_FINE_TUNE_VOLUME)>>8);
        if (u2ChannelSrcSet == SOURCE_NTSC_M)
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
        //for dynamic detection control
        vWriteDspWORD(ADDR_DYNAMIC_DETECTIO_SWITCH,uReadDspShmBYTE(B_DYNAMIC_DETECTIO_SWITCH));
        break;
    case PAL_STREAM:
        vWriteDspWORD (ADDR_DEM_ADC_GAIN,u2ReadDspShmWORD (W_ADC_GAIN));
        vWriteDspWORD (ADDR_DEM_USER_MODE,u2ReadDspShmWORD (W_USER_MODE));
        vWriteDspWORD (ADDR_DEM_ENFORCED_MODE,u2ReadDspShmWORD (W_ENFORCED_MODE));
        u1ChannelSrc = DSP_ReturnChannelSource ();
        u2ChannelSrcSet = SOURCE_PAL_I; // default value
        if (u1ChannelSrc == SV_PAL_BG)
        {
            u2ChannelSrcSet = SOURCE_PAL_BG;
        }
        else if (u1ChannelSrc == SV_PAL_DK)
        {
            u2ChannelSrcSet = SOURCE_PAL_DK;
        }
        else if (u1ChannelSrc == SV_SECAM_L)
        {
            u2ChannelSrcSet = SOURCE_SECAM_L;
        }
        else if (u1ChannelSrc == SV_SECAM_BG)
        {
            u2ChannelSrcSet = SOURCE_SECAM_BG;
        }
        else if (u1ChannelSrc == SV_SECAM_DK)
        {
            u2ChannelSrcSet = SOURCE_SECAM_DK;
        }
        else if (u1ChannelSrc == SV_SECAM_L_PLUM)
        {
            u2ChannelSrcSet = SOURCE_SECAM_L;
        }
        vWriteDspWORD (ADDR_DEM_SOURCE_MODE,u2ChannelSrcSet);
        vWriteDspWORD (ADDR_PAL_CORRECT_THRESHOLD,u2ReadDspShmWORD (W_PAL_CORRECT_THRESHOLD));
        vWriteDspWORD (ADDR_PAL_TOTAL_SYNC1_LOOP,u2ReadDspShmWORD (W_PAL_TOTAL_SYNC1_LOOP));
        vWriteDspWORD (ADDR_PAL_ERR_THRESHOLD,u2ReadDspShmWORD (W_PAL_ERR_THRESHOLD));
        vWriteDspWORD (ADDR_PAL_PARITY_ERR_THRESHOLD,u2ReadDspShmWORD (W_PAL_PARITY_ERR_THRESHOLD));
        vWriteDspWORD (ADDR_PAL_EVERY_NUM_FRAMES,u2ReadDspShmWORD (W_PAL_EVERY_NUM_FRAMES));
#ifdef ATV_HDEV_AUTO_SWITCH
        vWriteDspWORD (ADDR_HDEV_MODE, ((uReadDspShmBYTE (B_HDEV_MODE) & 0x01)| 0x03));
#else
#ifdef CC_AUD_PAL_NEW_HDEV_SUPPORT
        vWriteDspWORD (ADDR_HDEV_MODE, ((uReadDspShmBYTE (B_HDEV_MODE) & 0x01) << 2) | 0x1);  // set bit 0 and 2
#else
        vWriteDspWORD (ADDR_HDEV_MODE, (uReadDspShmBYTE (B_HDEV_MODE) & 0x01));
#endif
#endif
        vWriteDspWORD (ADDR_CARRIER_SHIFT_MODE, (uReadDspShmBYTE (B_CARRIER_SHIFT_MODE) & 0x01));
        vWriteDspWORD (ADDR_FM_SATU_MUTE_MODE,  (uReadDspShmBYTE (B_FM_SATU_MUTE_MODE) & 0x01));
        //for AM carrier sense mute
        vWriteDspWORD (ADDR_SECAM_L_MUTE_MODE,  uReadDspShmBYTE (B_AM_MUTE_MODE));
        vWriteDspWORD (ADDR_SECAM_L_MUTE_THRESHOLD_HIGHT, uReadDspShmBYTE (B_AM_MUTE_THRESHOLD_HIGHT));
        vWriteDspWORD (ADDR_SECAM_L_MUTE_THRESHOLD_LOW,uReadDspShmBYTE (B_AM_MUTE_THRESHOLD_LOW));
        //for FM carrier sense mute
        vWriteDspWORD (ADDR_FM_CARRIER_MUTE_MODE,  (uReadDspShmBYTE (B_FM_MUTE_MODE) & 0x01));
        vWriteDspWORD (ADDR_FM_CARRIER_MUTE_THRESHOLD_HIGHT, uReadDspShmBYTE (B_FM_MUTE_THRESHOLD_HIGHT_PAL));
        vWriteDspWORD (ADDR_FM_CARRIER_MUTE_THRESHOLD_LOW,uReadDspShmBYTE (B_FM_MUTE_THRESHOLD_LOW_PAL));
        //for EU/NON-EU FM
        vWriteDspWORD (ADDR_FM_NON_EU_MODE,uReadDspShmBYTE  (B_NON_EU_FM_MODE));
        //for NICAM I boost or not
        vWriteDspWORD(ADDR_NICAM_I_BOOST_FLAG,uReadDspShmBYTE(B_NICAM_I_BOOST_FLAG));
         //for PAL FM BAD SIGNAL CHECK
        //vWriteDspWORD (ADDR_PAL_HDEV_BAD_SIGNAL_CHECK,uReadDspShmBYTE (B_FM_BAD_SIGNAL));
        //for dynamic detection control
        vWriteDspWORD(ADDR_DYNAMIC_DETECTIO_SWITCH,uReadDspShmBYTE(B_DYNAMIC_DETECTIO_SWITCH));
        vWriteDspWORD (ADDR_DEM_PAL_FINE_TUNE_VOLUME,u4ReadDspShmDWRD  (D_PAL_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (ADDR_DEM_NICAM_FINE_TUNE_VOLUME,u4ReadDspShmDWRD (D_NICAM_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (ADDR_DEM_AM_FINE_TUNE_VOLUME,u4ReadDspShmDWRD  (D_AM_FINE_TUNE_VOLUME)>>8);

        // Decoder 2
        vWriteDspWORD (ADDR_DEM_SOURCE_MODE_DEC2,u2ChannelSrcSet);
        vWriteDspWORD (ADDR_DEM_USER_MODE_DEC2,u2ReadDspShmWORD (W_USER_MODE));
        vWriteDspWORD (ADDR_DEM_ADC_GAIN_DEC2,u2ReadDspShmWORD (W_ADC_GAIN));
        vWriteDspWORD (ADDR_DEM_ENFORCED_MODE_DEC2,u2ReadDspShmWORD (W_ENFORCED_MODE));

        vWriteDspWORD (ADDR_DEM_PAL_FINE_TUNE_VOLUME_DEC2,u4ReadDspShmDWRD  (D_PAL_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (ADDR_DEM_NICAM_FINE_TUNE_VOLUME_DEC2,u4ReadDspShmDWRD (D_NICAM_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (ADDR_DEM_AM_FINE_TUNE_VOLUME_DEC2,u4ReadDspShmDWRD  (D_AM_FINE_TUNE_VOLUME)>>8);

        vWriteDspWORD (ADDR_SECAM_L_MUTE_MODE_DEC2,  uReadDspShmBYTE (B_AM_MUTE_MODE));
        vWriteDspWORD (ADDR_SECAM_L_MUTE_THRESHOLD_HIGHT_DEC2, uReadDspShmBYTE (B_AM_MUTE_THRESHOLD_HIGHT));
        vWriteDspWORD (ADDR_SECAM_L_MUTE_THRESHOLD_LOW_DEC2,uReadDspShmBYTE (B_AM_MUTE_THRESHOLD_LOW));
        break;

    case DETECTOR_STREAM:
        vWriteDspWORD (ADDR_DETECTOR_DEFAULT_MODE_DEC2, uReadDspShmBYTE(B_DETECT_DEFAULT));
        vWriteDspWORD (ADDR_DETECTOR_START_FLAG_DEC2, 1); // default turn on detection
        WriteDspCommDram (ADDR_DETECTOR_FM_NOISE_TH_DEC2, u4ReadDspShmDWRD (D_DETECTOR_FM_NOISE_TH));
        break;
#endif

    case LPCM_STREAM:
        u1Temp1 = (u2ReadDspShmWORD(W_LPCM_FREQUENCY_DEC2)>>8) & 0xFF;
        u1Temp2 = u2ReadDspShmWORD(W_LPCM_FREQUENCY_DEC2) & 0xFF;

        vWriteDspWORD(ADDR_RC2D_LPCM_CHANNEL_ASSIGNMENT_DEC2, uReadDspShmBYTE(B_LPCM_CH_ASSIGNMENT_DEC2));
        //WriteDspCommDram(ADDR_RC2D_LPCM_FREQUENCY,  u2ReadDspShmWORD(W_LPCM_FREQUENCY));
        WriteDspCommDram(ADDR_RC2D_LPCM_FREQUENCY_DEC2,  (u1Temp1<<12)+u1Temp2);
        vWriteDspWORD(ADDR_RC2D_LPCM_BIT_SHIFT_DEC2,  uReadDspShmBYTE(B_LPCM_BIT_SHIFT_DEC2));
        vWriteDspWORD(ADDR_RC2D_LPCM_DRC_VALUE_DEC2,  uReadDspShmBYTE(B_LPCM_DRC_VALUE_DEC2));
        WriteDspCommDram(ADDR_RC2D_LPCM_Q_DEC2,  u2ReadDspShmWORD(W_LPCM_Q_VALUE_DEC2));
        vWriteDspWORD(ADDR_RC2D_LPCM_DRC_FLAG_DEC2,  uReadDspShmBYTE(B_LPCM_DRC_FLAG_DEC2));
        vWriteDspWORD(ADDR_RC2D_LPCM_DECODING_TYPE_DEC2,  uReadDspShmBYTE(W_LPCM_DECODING_TYPE_DEC2));

        if (uReadDspShmBYTE(W_ADPCM_DECODING_TYPE_DEC2)!=0) //ADPCM
        {
            vWriteDspWORD(ADDR_RC2D_LPCM_ADPCM_DEC_TYPE_DEC2, uReadDspShmBYTE(W_ADPCM_DECODING_TYPE_DEC2));
            vWriteDspWORD(ADDR_RC2D_LPCM_ADPCM_BITS_PER_SAMPLE_DEC2, uReadDspShmBYTE(B_ADPCM_BIT_PERSAMPLE_DEC2));
            vWriteDspWORD(ADDR_RC2D_ADPCM_BLOCK_ALIGN_DEC2, u2ReadDspShmWORD(W_ADPCM_BLOCK_ALIGN_DEC2));
            vWriteDspWORD(ADDR_RC2D_ADPCM_CHANNEL_NUM_DEC2, uReadDspShmBYTE(B_ADPCM_CHANNEL_NUM_DEC2));
        }
        break;
    case DTSDEC_STREAM:
        vWriteDspWORD (ADDR_RC2D_DTS_DRC_FLAG_DEC2, 0xffd9);
	    vWriteDspWORD (ADDR_RC2D_DTS_LFE_mixto_FRONT_DEC2, uReadDspShmBYTE(B_DTS_BC_LFE_MIXTO_FRONT_DEC2));
	    vWriteDspWORD (ADDR_RC2D_DTS_DRC_ONOFF_DEC2, uReadDspShmBYTE(B_DTS_BC_DRC_DEC2));
	    vWriteDspWORD (ADDR_RC2D_DTS_DIALNORM_ONOFF_DEC2, uReadDspShmBYTE(B_DTS_BC_Dialnrom_DEC2));
	    WriteDspCommDram (ADDR_RC2D_DTS_DRC_LOW_DEC2, u4ReadDspShmDWRD (D_DTS_BC_DRC_LOW));
        WriteDspCommDram (ADDR_RC2D_DTS_DRC_MEDIUM_DEC2, u4ReadDspShmDWRD (D_DTS_BC_DRC_MEDIUM));
        WriteDspCommDram (ADDR_RC2D_DTS_DRC_HIGH_DEC2, u4ReadDspShmDWRD (D_DTS_BC_DRC_HIGH));
        WriteDspCommDram (ADDR_RC2D_DTS_DIALNORM_LOW_DEC2, u4ReadDspShmDWRD (D_DTS_BC_Dialnorm_LOW));
        WriteDspCommDram (ADDR_RC2D_DTS_DIALNORM_MEDIUM_DEC2, u4ReadDspShmDWRD (D_DTS_BC_Dialnorm_MEDIUM));
        WriteDspCommDram (ADDR_RC2D_DTS_DIALNORM_HIGH_DEC2, u4ReadDspShmDWRD (D_DTS_BC_Dialnorm_HIGH));

        break;

    case AAC_STREAM:
#if 1//ndef CC_AUD_SUPPORT_MS10

        vWriteDspWORD (ADDR_RC2D_AAC_DOWNMIX_LEVEL_DEC2,
                       AAC_DEFAULT_DOWNMIX_LEVEL);
        vWriteDspWORD (ADDR_RC2D_AAC_TARGET_LEVEL_DEC2,
                       AAC_TARGET_LEVEL);
#else
        vWriteDspWORD (ADDR_RC2D_DDT_DUAL_MODE_DEC2,
                       uReadDspShmBYTE (B_DDT_DUALMODE_DEC2));
        vWriteDspWORD (ADDR_RC2D_DDT_DRC_DEC2,
                       uReadDspShmBYTE (B_DDT_COMPVAL_DEC2));
        vWriteDspWORD (ADDR_RC2D_DDT_DMX_MODE_DEC2,
                       uReadDspShmBYTE (B_DDT_COMPMODE_DEC2));

#endif
        vWriteDspWORD (ADDR_RC2D_DDT_CONTROL_DEC2,
                       uReadDspShmBYTE(B_DDT_CONTROL));
        break;

    #if 1///def CC_VORBIS_SUPPORT
    case VORBIS_STREAM:
        vWriteDspWORD(ADDR_RC2D_VORBIS_PACKET_NO, u2ReadDspShmWORD(W_VORBISPACKET));
        vWriteDspWORD(ADDR_D2RC_VORBIS_TABLE_OK_DEC2, 0);
        vWriteDspWORD(ADDR_D2RC_VORBIS_ABS_GRANULE_POS_LL16_DEC2, 0);
        vWriteDspWORD(ADDR_D2RC_VORBIS_ABS_GRANULE_POS_LH16_DEC2, 0);
        vWriteDspWORD(ADDR_D2RC_VORBIS_ABS_GRANULE_POS_OK_DEC2, 0);
        /// Dec2: 8.
        vWriteDspWORD (ADDR_RC2D_AOUT_BANK_256_NUM_DEC2, 8);   ///[Joel] Vorbis decoder dram only support 8+30 Bank
        break;
    #endif
    default:
        break;
    }
}

#ifdef CC_MT5391_AUD_3_DECODER
/***************************************************************************
     Function : vDecCommonInitDec3 (for third decoder)
  Description : This function is used to init the decoder EACH time when it
                is played. In other words, it is to set the properties which
                is different song by song. Note currently we only allow PCM and MP2 played on dec3
    Parameter : None
    Return    : None
***************************************************************************/
void vDecCommonInitDec3 (void)
{
    // Bonding options
    vWriteDspWORD (ADDR_RC2D_BONDING_OPTIONS, u2ReadDspShmWORD (W_BONDING_OPTIONS));

    //Soft Mute (Max. 7)
    vWriteDspWORD (ADDR_RC2D_VOLUME_SOFT_MUTE_ORDER_DEC3,
        u2ReadDspShmWORD (B_SOFTMUTEORDER_DEC3));

    DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);

    switch (_rDspVars[AUD_DEC_THIRD].bDspStrTyp)
    {
    case PCM_STREAM:
        //u4DspData = wReadDspWORD (ADDR_D2RC_INPUT_CH_CFG_DEC3);
        //u4DspData = u4DspData & ~0xf;
        //u4DspData = u4DspData | 0x2;  // Hard set cdda channel config
        //vWriteDspWORD (ADDR_D2RC_INPUT_CH_CFG_DEC3, u4DspData);
        vWriteDspWORD (ADDR_RC2D_PCM_INPUT_CHANNEL_NUM_DEC3,
                       u2ReadDspShmWORD (W_PCM_INPUT_CHANNEL_NUM_DEC3));
        vWriteDspWORD (ADDR_RC2D_PCM_INPUT_CH_ASSIGNMENT_DEC3,
                       u2ReadDspShmWORD (W_PCM_INPUT_CH_ASSIGNMENT_DEC3));
        vWriteDspWORD (ADDR_RC2D_PCM_INPUT_TYPE_DEC3,
                       u2ReadDspShmWORD (W_PCM_INPUT_TYPE_DEC3));  //0:pcm24 1:IMA 3:MS 2:pcm16 //msb=1: little endian
        vWriteDspWORD (ADDR_RC2D_PCM_INPUT_SAMPLING_RATE_DEC3,
                       u2ReadDspShmWORD (W_PCM_INPUT_SAMPLING_RATE_DEC3));
#if 0
        vWriteDspWORD (ADDR_RC2D_ADPCM_BLOCK_ALIGN_DEC2,
                       u2ReadDspShmWORD (W_ADPCM_BLOCK_ALIGN));
        vWriteDspWORD (ADDR_RC2D_ADPCM_CHANNEL_NUM_DEC2,
                       u2ReadDspShmWORD (B_ADPCM_CHANNEL_NUM));
#endif
        break;

case AC3_STREAM:
	 // Use the same settings as decoder 2, fix me!!!!
        //AC3 Karaoke Mode
        vWriteDspWORD (ADDR_RC2D_AC3_KARAOKE_MODE_DEC3,
                       u2ReadDspShmWORD (W_AC3KARAMOD_DEC2));
        //AC3 Dual Mono Mode
        vWriteDspWORD (ADDR_RC2D_AC3_DUAL_MODE_DEC3,
                       u2ReadDspShmWORD (W_AC3DUALMODE_DEC2));
        //AC3 Compression Mode
        vWriteDspWORD (ADDR_RC2D_AC3_COMPRESS_MODE_DEC3,
                       u2ReadDspShmWORD (W_AC3COMPMOD_DEC2));
        //AC3 Low Boost
        WriteDspCommDram (ADDR_RC2D_AC3_DYNAMIC_LOW_DEC3,
                          u4ReadDspShmDWRD (D_AC3DYN_LOW_DEC2));
        //AC3 High Cut
        WriteDspCommDram (ADDR_RC2D_AC3_DYNAMIC_HIGH_DEC3,
                          u4ReadDspShmDWRD (D_AC3DYN_HIGH_DEC2));
        // AC3 Auto downmix
        WriteDspCommDram (ADDR_RC2D_AC3_DOWNMIX_DEC3,
                          uReadDspShmBYTE (B_AC3AUTODNMIX_DEC2));
        // DDC control
        vWriteDspWORD (ADDR_RC2D_AC3_CONTROL_DEC3, u2ReadDspShmWORD(W_DDC_CONTROL_DEC2));
        break;

    case MPEG12_STREAM:
        /* 20041220: For testing, hard coded here, remove in the Future */
        // MPEG sync no error checking
//      vWriteDspWORD (ADDR_RC2D_MPEG_SYNC_MODE_DEC2, 0x1);
        //MPEG Error Detection
        vWriteDspWORD (ADDR_RC2D_MPEG_CRC_FLAG_DEC3,
        u2ReadDspShmWORD (W_MPEGERRDET_DEC3));
        break;
#ifdef MP3ENC_SUPPORT
	case MP3ENC_STREAM:		///[Joel]20090327
		//Need set by MW
	    ///Printf("Mp3 Encoder Share Info Initialize \n");
		WriteDspCommDram(ADDR_RC2D_ENCODER_OUTBUF_READ_PNT_DEC3, _tPbsBlk[0].wSblk << 8);
		WriteDspCommDram(ADDR_RC2D_ENCODER_OUTBUF_WRITE_PNT_DEC3, _tPbsBlk[0].wSblk << 8);
		WriteDspCommDram(ADDR_RC2D_ENCODER_OUTBUF_SIZE_DEC3, (_tPbsBlk[0].wEblk - _tPbsBlk[0].wSblk) << 8);

		WriteDspCommDram(ADDR_RC2D_ENCODER_SFREQ_DEC3, 0x0100);			///48KHz
		WriteDspCommDram(ADDR_RC2D_ENCODER_BITRATE_DEC3, (UINT16)MP3ENC_BR<<8);		///0x0E00, 320kbps / 0x0900, 128Kbps / 0x0B00, 192Jbps
		WriteDspCommDram(ADDR_RC2D_ENCODER_CHANNEL_MODE_DEC3, 0x0000);	///Stereo
		WriteDspCommDram(ADDR_RC2D_ENCODER_MPEG_FLAG_DEC3, 0x0000); /// Set padding bit to never for fix frame size.

        //For upload info
        vWriteDspWORD (ADDR_RC2D_UPLOAD_DATA_FLAG, u2ReadDspShmWORD(W_UPLOAD_DATA_FLAG));
        vWriteDspWORD (ADDR_RC2D_UPLOAD_BLOCK_NUM, uReadDspShmBYTE(B_UPLOAD_BLOCK_NUM));
        vWriteDspWORD (ADDR_RC2D_UPLOAD_BLOCK_SIZE, u2ReadDspShmWORD(W_UPLOAD_BLOCK_SIZE));
        vWriteDspWORD (ADDR_RC2D_UPLOAD_BLK_INT_CNT, uReadDspShmBYTE(B_UPLOAD_BLK_INT_CNT));

		///Set RP,WP pointer address
		_tPbsBlk[0].dwRdPtrAdr = ADDR_RC2D_ENCODER_OUTBUF_READ_PNT_DEC3;
		_tPbsBlk[0].dwWrPtrAdr = ADDR_RC2D_ENCODER_OUTBUF_WRITE_PNT_DEC3;

		///Set Mp3 encoder putbit start & end address
        AUD_WRITE32(REG_PUTBS_BLK0_RISC,
        	(_tPbsBlk[0].wSblk << 16) | (_tPbsBlk[0].wEblk));
		AUD_WRITE32(REG_PUTBS_BLK1_RISC,
        	(_tPbsBlk[1].wSblk << 16) | (_tPbsBlk[1].wEblk));
		break;
#endif

    case AAC_STREAM:
#if 1//ndef CC_AUD_SUPPORT_MS10

        vWriteDspWORD (ADDR_RC2D_AAC_DOWNMIX_LEVEL_DEC3,
                       AAC_DEFAULT_DOWNMIX_LEVEL);
        vWriteDspWORD (ADDR_RC2D_AAC_TARGET_LEVEL_DEC3,
                       AAC_TARGET_LEVEL);
#else
        vWriteDspWORD (ADDR_RC2D_DDT_DUAL_MODE_DEC3,
                       uReadDspShmBYTE (B_DDT_DUALMODE_DEC3));
        vWriteDspWORD (ADDR_RC2D_DDT_DRC_DEC3,
                       uReadDspShmBYTE (B_DDT_COMPVAL_DEC3));
        vWriteDspWORD (ADDR_RC2D_DDT_DMX_MODE_DEC3,
                       uReadDspShmBYTE (B_DDT_COMPMODE_DEC3));
#endif
        vWriteDspWORD (ADDR_RC2D_DDT_CONTROL_DEC3,
                       uReadDspShmBYTE(B_DDT_CONTROL));
        break;

    default:
        break;
    }
}
#endif

#ifdef CC_AUD_4_DECODER_SUPPORT
/***************************************************************************
     Function : vDecCommonInitDec4 (for 4th decoder)
  Description : This function is used to init the decoder EACH time when it
                is played. In other words, it is to set the properties which
                is different song by song. Note currently we only allow PCM and MP2 played on dec3
    Parameter : None
    Return    : None
***************************************************************************/
void vDecCommonInitDec4 (void)
{
    // Bonding options
    vWriteDspWORD (ADDR_RC2D_BONDING_OPTIONS, u2ReadDspShmWORD (W_BONDING_OPTIONS));

    switch (_rDspVars[AUD_DEC_4TH].bDspStrTyp)
    {
    case SBCE_STREAM:
        //Printf("vDecCommonInitDec4 => SBC_STREAM\n"); //SBCENC_TEST
        vWriteDspWORD (ADDR_RC2D_SBCE_SAMPLE_FREQ_DEC4, uReadDspShmBYTE(B_SBCENC_SAMPLE_FREQ));
        vWriteDspWORD (ADDR_RC2D_SBCE_BLK_NUM_1_DEC4, uReadDspShmBYTE(B_SBCENC_BLK_NUM_1));
        vWriteDspWORD (ADDR_RC2D_SBCE_CH_NUM_1_DEC4, uReadDspShmBYTE(B_SBCENC_CH_NUM_1));
        vWriteDspWORD (ADDR_RC2D_SBCE_BITPOOL_DEC4, uReadDspShmBYTE(B_SBCENC_BITPOOL));
        vWriteDspWORD (ADDR_RC2D_SBCE_SAMPLE_NUM_DEC4, ((UINT32)uReadDspShmBYTE(B_SBCENC_BLK_NUM_1)+1)*8);
        WriteDspCommDram (ADDR_RC2D_SBCE_FRAME_SIZE_DEC4, ((UINT32)uReadDspShmBYTE(B_SBCENC_BLK_NUM_1)+1)*8*3);
        vWriteDspWORD (ADDR_RC2D_SBCE_HDR_0_DEC4,
                        ((uReadDspShmBYTE(B_SBCENC_SAMPLE_FREQ)<<6) |
                        ((uReadDspShmBYTE(B_SBCENC_BLK_NUM_1)>>2)<<4) |
                        (uReadDspShmBYTE(B_SBCENC_CH_NUM_1)<<3) | 0x01));
        {
            //SBCENC_TEST
            //Borrow the internal_buf4 and upload buffer
            UINT32 u4StartBlock,u4EndBlock;
            u4StartBlock = ((_u4SBCEncBuf[0]>>8)&0xffff);
            u4EndBlock = ((_u4SBCEncBuf[1]>>8)&0xffff);
            AUD_WRITE32(REG_PUTBS_BLK0_RISC,(u4StartBlock<<16) | u4EndBlock);
            AUD_WRITE32(REG_PUTBS_BLK1_RISC,(u4StartBlock<<16) | u4EndBlock);
            vWriteDspWORD(ADDR_RC2D_SBCE_WRITE_PTR_DEC4,u4StartBlock);
        }
        Printf("!!!!!!!!!!!!!!! SBCE_STREAM !!!!!!!!!!!!!\n");
        Printf("  ADDR_RC2D_SBCE_SAMPLE_FREQ_DEC4: 0x%08x\n", dReadDspCommDram(ADDR_RC2D_SBCE_SAMPLE_FREQ_DEC4));
        Printf("  ADDR_RC2D_SBCE_BLK_NUM_1_DEC4: 0x%08x\n", dReadDspCommDram(ADDR_RC2D_SBCE_BLK_NUM_1_DEC4));
        Printf("  ADDR_RC2D_SBCE_CH_NUM_1_DEC4: 0x%08x\n", dReadDspCommDram(ADDR_RC2D_SBCE_CH_NUM_1_DEC4));
        Printf("  ADDR_RC2D_SBCE_BITPOOL_DEC4: 0x%08x\n", dReadDspCommDram(ADDR_RC2D_SBCE_BITPOOL_DEC4));
        Printf("  ADDR_RC2D_SBCE_SAMPLE_NUM_DEC4: 0x%08x\n", dReadDspCommDram(ADDR_RC2D_SBCE_SAMPLE_NUM_DEC4));
        Printf("  ADDR_RC2D_SBCE_FRAME_SIZE_DEC4: 0x%08x\n", dReadDspCommDram(ADDR_RC2D_SBCE_FRAME_SIZE_DEC4));
        break;
    case PCM_STREAM:
        vWriteDspWORD (ADDR_RC2D_PCM_INPUT_CHANNEL_NUM_DEC4,
                       u2ReadDspShmWORD (W_PCM_INPUT_CHANNEL_NUM_DEC4));
        vWriteDspWORD (ADDR_RC2D_PCM_INPUT_CH_ASSIGNMENT_DEC4,
                       u2ReadDspShmWORD (W_PCM_INPUT_CH_ASSIGNMENT_DEC4));
        vWriteDspWORD (ADDR_RC2D_PCM_INPUT_TYPE_DEC4,
                       u2ReadDspShmWORD (W_PCM_INPUT_TYPE_DEC4));  //0:pcm24 1:IMA 3:MS 2:pcm16 //msb=1: little endian
        vWriteDspWORD (ADDR_RC2D_PCM_INPUT_SAMPLING_RATE_DEC4,
                       u2ReadDspShmWORD (W_PCM_INPUT_SAMPLING_RATE_DEC4));
        break;
    default:
        break;
    }
}
#endif

/***************************************************************************
     Function : vDspStateInit
  Description :Reset _rDspStatus variable and command queue
               Make sure dsp is in init wait play loop
               if not wake up -> Change to RC_INIT state and issue wakeup cmd
    Parameter :
    Return    :
***************************************************************************/
void vDspStateInit (void)
{
    UINT8 u1DecId;

    for (u1DecId=0; u1DecId < AUD_DEC_NUM; u1DecId++)
    {
        if (_rDspStatus[u1DecId].fgDspPlay)
        {
            vDspReset(u1DecId);
        }
    }

    _rDspStatus[AUD_DEC_MAIN].fgDspStop = TRUE;          // stop ok
    _rDspStatus[AUD_DEC_MAIN].fgDspStopIssue = TRUE;
    _rDspStatus[AUD_DEC_MAIN].fgDspPlay = FALSE;
    _rDspStatus[AUD_DEC_MAIN].fgDspRealPlay = FALSE;
    _rDspStatus[AUD_DEC_MAIN].fgDspFlush = FALSE;
    _rDspStatus[AUD_DEC_MAIN].fgSendSample = FALSE;
    _rDspStatus[AUD_DEC_MAIN].fgDspAoutMuted = FALSE;
    _rDspStatus[AUD_DEC_MAIN].fgDspReParsing = FALSE;
    _rDspStatus[AUD_DEC_MAIN].fgDspGetMpgTyp = FALSE;
    _rDspStatus[AUD_DEC_MAIN].fgDACOn = TRUE;
    _rDspVars[AUD_DEC_MAIN].dwDspReInitPts = 0;
    _rDspVars[AUD_DEC_MAIN].bDspStrTyp = (UINT8)0xFF;  //Set default to 0xff, since type 0 is AC3.
    _rDspVars[AUD_DEC_MAIN].dwDspForceResetCnt = 0;
    _rDspVars[AUD_DEC_MAIN].dwStcDiff = 0;

    /* secondary decoder */
    _rDspStatus[AUD_DEC_AUX].fgDspStop = TRUE;      // stop ok
    _rDspStatus[AUD_DEC_AUX].fgDspStopIssue = TRUE;
    _rDspStatus[AUD_DEC_AUX].fgDspPlay = FALSE;
    _rDspStatus[AUD_DEC_AUX].fgDspRealPlay = FALSE;
    _rDspStatus[AUD_DEC_AUX].fgDspFlush = FALSE;
    _rDspStatus[AUD_DEC_AUX].fgDspAoutMuted = FALSE;
    _rDspStatus[AUD_DEC_AUX].fgSendSample = FALSE;
    _rDspVars[AUD_DEC_AUX].dwDspForceResetCnt = 0;
    _rDspVars[AUD_DEC_AUX].bDspStrTyp = (UINT8)0xFF;  //Set default to 0xff, since type 0 is AC3.

#ifdef CC_MT5391_AUD_3_DECODER
     /* third decoder */
    _rDspStatus[AUD_DEC_THIRD].fgDspStop = TRUE;      // stop ok
    _rDspStatus[AUD_DEC_THIRD].fgDspStopIssue = TRUE;
    _rDspStatus[AUD_DEC_THIRD].fgDspPlay = FALSE;
    _rDspStatus[AUD_DEC_THIRD].fgDspRealPlay = FALSE;
    _rDspStatus[AUD_DEC_THIRD].fgDspFlush = FALSE;
    _rDspStatus[AUD_DEC_THIRD].fgDspAoutMuted = FALSE;
    _rDspVars[AUD_DEC_THIRD].dwDspForceResetCnt = 0;
    _rDspVars[AUD_DEC_THIRD].bDspStrTyp = (UINT8)0xFF;  //Set default to 0xff, since type 0 is AC3.
#endif

#ifdef CC_AUD_4_DECODER_SUPPORT
     /* fourth decoder */
    _rDspStatus[AUD_DEC_4TH].fgDspStop = TRUE;      // stop ok
    _rDspStatus[AUD_DEC_4TH].fgDspStopIssue = TRUE;
    _rDspStatus[AUD_DEC_4TH].fgDspPlay = FALSE;
    _rDspStatus[AUD_DEC_4TH].fgDspRealPlay = FALSE;
    _rDspStatus[AUD_DEC_4TH].fgDspFlush = FALSE;
    _rDspStatus[AUD_DEC_4TH].fgDspAoutMuted = FALSE;
    _rDspVars[AUD_DEC_4TH].dwDspForceResetCnt = 0;
    _rDspVars[AUD_DEC_4TH].bDspStrTyp = (UINT8)0xFF;  //Set default to 0xff, since type 0 is AC3.
#endif

    // DSP interrupt flag initial
    _rDspStatus[AUD_DEC_MAIN].fgDspRInt = FALSE;
    _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
    _rDspStatus[AUD_DEC_MAIN].fgDspForceReset = FALSE;
    _rDspVars[AUD_DEC_MAIN].dwDspSIntFail = 0;
    _rDspVars[AUD_DEC_MAIN].dwDspFreq = 0xffff;
    _rDspVars[AUD_DEC_MAIN].dwDspRamCodeType = (UINT8)-1;

    // DSP user operation initial
    _rDspStatus[AUD_DEC_MAIN].fgDspUop = FALSE;
    _rDspVars[AUD_DEC_MAIN].dwDspUop = 0;

    WriteDspCommDram (ADDR_RC2D_DOWNLOAD_TABLE_FLAG, 0);
    _rDspStatus[AUD_DEC_MAIN].fgDspPtsSet = FALSE;

    if (!_rDspStatus[AUD_DEC_MAIN].fgDspWakeUp)
    {
        // We can only load code when not wakeup
        // Because at this time dsp is not in cache
        //lint -e{506}
        //lint -e{774}
        if (_fgCommRAMFromFlash)
        {
            vDspLoadRamCode (AUD_COMMRAM);
        }

        /*lint -e{506}*/
        /*lint -e{774}*/
        if (_fgPostRamFromFlash)
        {
            vDspLoadRamCode (AUD_POSTRAM);
        }

        _uDspState = (UINT8)DSP_RC_INIT;
    }

#if 1///def CC_VORBIS_SUPPORT ///def DSP_VORBIS_SUPPORT
    _uVorbisMkTbl[AUD_DEC_MAIN] = FALSE;
    _uVorbisTblDone[AUD_DEC_MAIN] = FALSE;
    _uVorbisMkTbl[AUD_DEC_AUX] = FALSE;
    _uVorbisTblDone[AUD_DEC_AUX] = FALSE;
    _uVorbisIntDec = 0;
#endif
}

/***************************************************************************
     Function : vDspRest
  Description : Reset DSP ,affect DSP status
    Parameter :
    Return    :
****************************************************************************/
void vDspReset(UINT8 u1DecId)
{
    UINT32 dwTryCnt;
    UINT32 u4IntData = (UINT32)JOB_RESET_DECODER;

    _rDspVars[u1DecId].dwDspForceResetCnt++;

    if (!_rDspStatus[AUD_DEC_MAIN].fgDspWakeUp)
    {
        return;
    }

    if (u1DecId == AUD_DEC_AUX)
    {
        u4IntData = JOB_RESET_DECODER2;
    }
#ifdef CC_MT5391_AUD_3_DECODER
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u4IntData = JOB_RESET_JOB3;
    }
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    else if (u1DecId == AUD_DEC_4TH)
    {
        u4IntData = JOB_RESET_JOB4;
    }
#endif

    if (!_rDspStatus[u1DecId].fgDspForceReset)
    {
        dwTryCnt = 0;
        _rDspStatus[u1DecId].fgDspStop = TRUE;
        _rDspStatus[u1DecId].fgDspStopIssue = TRUE;
        _rDspStatus[u1DecId].fgDspPlay = FALSE;
        _rDspStatus[u1DecId].fgDspRealPlay = FALSE;
        _rDspStatus[u1DecId].fgDspFlush = FALSE;
        _rDspStatus[u1DecId].fgDspForceResetDec = TRUE;
        _rDspStatus[u1DecId].fgSendSample = FALSE;
        _rDspStatus[u1DecId].fgDspReParsing = FALSE;
        _rDspStatus[u1DecId].fgDspPtsSet = FALSE;
        _rDspVars[u1DecId].dwDspReInitPts = 0;
        _rDspVars[u1DecId].dwDspRamCodeType = (UINT32)-1;

        dwTryCnt = 0;

        while (TRUE)
        {
            CRIT_STATE_T rCritState;

            dwTryCnt++;

            rCritState = x_crit_start();
            if (!fgDspBusy ())
            {
                SendDSPInt (INT_RC2D_DSP_JOB_RESET, u4IntData);  // short data & interrupt
                vDspBlockPtrReset (TRUE);
                x_crit_end(rCritState);
                break;
            }
            else
            {
                x_crit_end(rCritState);
                if (dwTryCnt == DSP_TIMEOUT)
                {
                    break;
                }
            }
        }

        vDspDecDes(u1DecId);
    }
}

#ifdef CC_AUD_MIXSOUND_SUPPORT
extern UINT32 _u4MixsoundLoop;
#endif

/***************************************************************************
     Function : void vDspShareInfoInit(void)
  Description : DSP share info initialization(used when no flash download available)
    Parameter : None
    Return    : None
***************************************************************************/
void vDspShareInfoInit (void)
{
    int i;
#ifdef DSP_SUPPORT_NPTV
    AUD_MTS_FAC_T*     prMtsFactory;
    AUD_A2_FAC_T*     prA2Factory;
    AUD_PAL_FAC_T*     prPalFactory;
    UNUSED(DRVCUST_OptQuery(eAudioMtsFactory, (UINT32 *)(void *) &prMtsFactory));
    UNUSED(DRVCUST_OptQuery(eAudioA2Factory, (UINT32 *)(void *) &prA2Factory));
    UNUSED(DRVCUST_OptQuery(eAudioPalFactory, (UINT32 *)(void *) &prPalFactory));
#endif //DSP_SUPPORT_NPTV

    UNUSED(i);

    // for debug use
    vWriteDspShmDWRD (D_SPKCFG, _u4DspGetSpkConfig()); // full channel

    vWriteDspShmDWRD (D_SPKCFG_DEC2,     0x0); // LtRt

    // Bonding options check
#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
    vWriteDspShmWORD(W_BONDING_OPTIONS,
        ((IS_SupportH264()<<4) | (IS_SupportDTS()<<5) | (IS_SupportAVS()<<6)));

	if ( IS_SupportDOLBY() )
      
    {
         vWriteDspShmDWRD(W_BONDING_OPTIONS, (u2ReadDspShmWORD (W_BONDING_OPTIONS) ) |
          (IS_SW_SupportAC3() | (IS_SW_SupportEAC3()<<1) |
          (IS_SW_SupportDDCO()<<2) | (IS_SW_SupportMS10()<<3)));
    }
      
#else
        // Bonding options check
    vWriteDspShmDWRD(W_BONDING_OPTIONS,
        (IS_SupportAC3() | (IS_SupportEAC3()<<1) |
        (IS_SupportDDCO()<<2) | (IS_SupportMS10()<<3) |
        (IS_SupportH264()<<4) | (IS_SupportDTS()<<5)));

#endif

    // Reset channel delay
    vWriteDspShmWORD (W_CHDELAY_C,          0x0); //0x0
    vWriteDspShmWORD (W_CHDELAY_L,          0x0);
    vWriteDspShmWORD (W_CHDELAY_R,          0x0);
    vWriteDspShmWORD (W_CHDELAY_LS,         0x0);
    vWriteDspShmWORD (W_CHDELAY_RS,         0x0);
    vWriteDspShmWORD (W_CHDELAY_CH7,        0x0);
    vWriteDspShmWORD (W_CHDELAY_CH8,        0x0);
    vWriteDspShmWORD (W_CHDELAY_SUB,        0x0);
    vWriteDspShmWORD (W_CHDELAY_CH9,        0x0);
    vWriteDspShmWORD (W_CHDELAY_CH10,       0x0);

    vWriteDspShmWORD (W_CHDELAY_C_DEC2,     0);
    vWriteDspShmWORD (W_CHDELAY_L_DEC2,     0);
    vWriteDspShmWORD (W_CHDELAY_R_DEC2,     0);

    // Input Src Gain
    vWriteDspShmDWRD (D_INSRC_GAIN,         0x20000);   // 0x20000 is 0dB
    vWriteDspShmDWRD (D_INSRC_GAIN_DEC2,    0x20000);   // 0x20000 is 0dB
#ifdef CC_MT5391_AUD_3_DECODER
    vWriteDspShmDWRD (D_INSRC_GAIN_DEC3,    0x20000);   // 0x20000 is 0dB
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    vWriteDspShmDWRD (D_INSRC_GAIN_DEC4,    0x20000);   // 0x20000 is 0dB
#endif

    // Set IEC output channel selection to SPDIF
    // For temporary use, need to move to driver custermization
#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY_TEST_MODE
    vWriteDspShmBYTE (B_IEC_PCMCH,          0x0);//L,R
#else
    vWriteDspShmBYTE (B_IEC_PCMCH,          0x3);//7,8
#endif

    vWriteDspShmWORD (W_PROCMOD,    IS_BYPASS_PROC() ? 0xfc00 : (0xc00 | (IS_SPKCFG_SSLN()<<7)) ); // ch9/10 downmix
    vWriteDspShmBYTE (B_KARAFLAG,          0x0);
    vWriteDspShmDWRD (D_LRMIXRATIO,        0x400000);  // 0.5
    vWriteDspShmBYTE (B_KARAFLAG_DEC2,     0x0);
    vWriteDspShmDWRD (D_LRMIXRATIO_DEC2,   0x400000);  // 0.5

    vWriteDspShmWORD (W_PROCMOD_DEC2, 0x9400);		//downmix to LR
#ifdef CC_MT5391_AUD_3_DECODER
    vWriteDspShmDWRD (W_PROCMOD_DEC3,    0x9000);
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    vWriteDspShmDWRD (W_PROCMOD_DEC4,    0x9000);
#endif

    vWriteDspShmDWRD(D_SBASSBOOSTGAIN,  (BASS_TREBLE_GAIN_MAX-1)/2);
    vWriteDspShmDWRD(D_SBASSCLEARGAIN,  (BASS_TREBLE_GAIN_MAX-1)/2);

    vWriteDspShmBYTE (B_IECFLAG,         0);
    //vWriteDspShmBYTE (B_IECFLAG_DEC2,    0);
    vWriteDspShmBYTE (B_IEC_MAX_FREQ, SV_48K);
    vWriteDspShmBYTE (B_IEC_COPYRIGHT, IEC_COPY_FREELY);
    if (AUD_GetTargetCountry() == COUNTRY_EU)
    {
        vWriteDspShmBYTE (B_IEC_CATEGORY_CODE, IEC_NON_ORIGINAL_BROAD_CAST);
    }
    else
    {
        vWriteDspShmBYTE (B_IEC_CATEGORY_CODE, IEC_NON_ORIGINAL_BROAD_CAST);
    }
    vWriteDspShmBYTE (B_IEC_WORD_LENGTH, IEC_WORD_LENGTH_16BIT);
    vWriteDspShmBYTE (B_IECRAW_DELAY_FLAG, (UINT8)DRVCUST_OptGet(eAudioIecRawDelayFlag));
    vWriteDspShmWORD(W_IEC_RAW_OFFSET, (INT16)DRVCUST_OptGet(eAudioIecRawDelay));


    // DSP Group 0 Initial
    vWriteDspShmWORD (W_INTCTRL, 0);
    vWriteDspShmWORD (W_SYSTEM_CTRL,  0x3); // bit0: enable internal buffer, bit1: internal data is from dec1(0) or dec2(1)

#ifdef CC_AUD_DEC3_USE_FIFO1 // USE_FIFO3 : dec3 use fifo3
    vWriteDspShmWORD (W_SYSTEM_CTRL2, 0);
#else
    vWriteDspShmWORD (W_SYSTEM_CTRL2, 2); //bit1 is dec3 source sel.
#endif

    vWriteDspShmWORD (W_INTERNAL_BUF_IN_FLAG, 0x2);  // bit 1: fill internal buffer after volume_dec2
#ifdef CC_MT5391_AUD_3_DECODER
#ifndef CC_AUD_SUPPORT_MS10
    vWriteDspShmWORD (W_INTERNAL_BUF2_IN_FLAG, 0x4);  // bit 1: after volume
#else
    vWriteDspShmWORD (W_INTERNAL_BUF2_IN_FLAG, 0x2);  // bit 1: after volume
#endif
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    vWriteDspShmWORD (W_INTERNAL_BUF3_IN_FLAG, 0x1);
#endif

/*
#ifdef MP3ENC_SUPPORT
    // Internal Buffer4 from dec2 for ATV audio record.
    // Enable Internal Buffer4 for mp3enc
    vWriteDspShmWORD (W_SYSTEM_CTRL,(u2ReadDspShmWORD (W_SYSTEM_CTRL)|SHM_INBUF4_SRC_SEL));
    vWriteDspShmWORD (W_INTERNAL_BUF4_IN_FLAG, 0x02);   /// 1:Before postprocessor, 2:After postprocessor.
#endif
*/
    vWriteDspShmWORD (W_SYSTEM_SETUP,
        (SYSTEM_MASTER | USE_STC_A1 | AIN_BUF_RD_NOT_HOLD_WR | COPY_CH78_TO_LS_RS |
        (((UINT16)DRVCUST_OptGet(eAudioPanFadeControl))<<9) |
        (((UINT8)DRVCUST_OptGet(eAudioDownmixLR))<<6) |
        (((UINT8)DRVCUST_OptGet(eAudioDownmixPosition))<<4) |
        (((UINT8)DRVCUST_OptGet(eAudioMultiPairOutput))<<3) |
        (IS_SPKCFG_SSLN()<<2))); //FIXME
    //Audio Description
    vWriteDspShmWORD (W_SYSTEM_SETUP_DEC2,
        (SYSTEM_MASTER | USE_STC_A1 | AIN_BUF_RD_NOT_HOLD_WR)); //FIXME
#ifdef CC_MT5391_AUD_3_DECODER
    vWriteDspShmWORD (W_SYSTEM_SETUP_DEC3,
        (SYSTEM_MASTER | USE_STC_A1 | AIN_BUF_RD_NOT_HOLD_WR)); //FIXME
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    vWriteDspShmWORD (W_SYSTEM_SETUP_DEC4,
        (SYSTEM_MASTER | USE_STC_A1 | AIN_BUF_RD_NOT_HOLD_WR));
#endif

    vWriteDspShmWORD (W_SPEED,              0);
    vWriteDspShmBYTE (B_BIT,               24); // Bit Resolution
    vWriteDspShmBYTE (B_BANK576NUM,         9+13); // for mp3 modification
    vWriteDspShmBYTE (B_BANK384NUM,        12+20);
    vWriteDspShmBYTE (B_BANK320NUM,        14+24);
    vWriteDspShmBYTE (B_BANK256NUM,        18+30);//sunman for 250ms (48kHz) buffer
    vWriteDspShmWORD (W_SPEED_DEC2,         0);

    vWriteDspShmBYTE (B_BIT_DEC2,          24); // Bit Resolution
    vWriteDspShmBYTE (B_BANK576NUM_DEC2,    9); // for mp3 modification
    vWriteDspShmBYTE (B_BANK384NUM_DEC2,    12);
    vWriteDspShmBYTE (B_BANK320NUM_DEC2,    14);
    vWriteDspShmBYTE (B_BANK256NUM_DEC2,    18);
#ifdef CC_MT5391_AUD_3_DECODER
    vWriteDspShmBYTE (B_BIT_DEC3,          24); // Bit Resolution
    vWriteDspShmBYTE (B_BANK576NUM_DEC3,    9); // for mp3 modification
    vWriteDspShmBYTE (B_BANK384NUM_DEC3,    12);
    vWriteDspShmBYTE (B_BANK320NUM_DEC3,    14);
    vWriteDspShmBYTE (B_BANK256NUM_DEC3,    18);
#endif

    // Init volume related
    vWriteDspShmBYTE (B_VOLUPORDER,         5); /* volume display up order */
    vWriteDspShmBYTE (B_VOLDOWNORDER,       6); /* volume display down order */
#ifdef __MODEL_slt__
    vWriteDspShmBYTE (B_SOFTMUTEORDER,      0);
#else
    vWriteDspShmBYTE (B_SOFTMUTEORDER,      SOFT_MUTE_ORDER); /* pause key fade in/out */
                                                /* set to 3, fade-in/out time = 56/37 ms @ 32k/48k Hz */
                                                /*--------------------------*/
                                                // Light add comment,
                                                // softmute_num = 128 * 2^(softmute_order)
                                                // Set softmute_order = 0 won't do softmute!
                                                /*--------------------------*/
#endif //__MODEL_slt__

    vWriteDspShmWORD (W_ERRORMUTEBANK,      0);
    vWriteDspShmBYTE (B_IEC_DELAY,         0);
    vWriteDspShmBYTE (B_VOLUPORDER_DEC2,    3); /* volume display up order */
    vWriteDspShmBYTE (B_VOLDOWNORDER_DEC2,  6); /* volume display down order */
#ifdef __MODEL_slt__
    vWriteDspShmBYTE (B_SOFTMUTEORDER_DEC2, 0);
#else
    vWriteDspShmBYTE (B_SOFTMUTEORDER_DEC2, 4); /* pause key fade in/out */
#endif //__MODEL_slt__

#ifdef CC_MT5391_AUD_3_DECODER
#ifdef __MODEL_slt__
    vWriteDspShmBYTE (B_SOFTMUTEORDER_DEC3, 0);
#else
    vWriteDspShmBYTE (B_SOFTMUTEORDER_DEC3, 4); /* pause key fade in/out */
#endif
#endif //CC_MT5391_AUD_3_DECODER
     vWriteDspShmWORD (W_ERRORMUTEBANK_DEC2, 0);

    vWriteDspShmDWRD (D_VOL,            MASTER_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (D_TRIM_C,         CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (D_TRIM_L,         CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (D_TRIM_R,         CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (D_TRIM_LS,        CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (D_TRIM_RS,        CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (D_TRIM_CH7,       CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (D_TRIM_CH8,       CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (D_TRIM_SUB,       CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (D_TRIM_LFE,       CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (D_DIALOGUE,       CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (D_TRIM_CH9,       CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (D_TRIM_CH10,      CH_VOL_SHM_DEFAULT);

    vWriteDspShmDWRD (D_VOL_DEC2,       MASTER_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (D_TRIM_C_DEC2,    CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (D_TRIM_L_DEC2,    CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (D_TRIM_R_DEC2,    CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (D_TRIM_LFE_DEC2,  CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (D_DIALOGUE_DEC2,  CH_VOL_SHM_DEFAULT);

#ifdef CC_MT5391_AUD_3_DECODER
    vWriteDspShmDWRD (D_VOL_DEC3,       MASTER_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (D_TRIM_L_DEC3,  CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (D_TRIM_R_DEC3,  CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (D_DIALOGUE_DEC3,  CH_VOL_SHM_DEFAULT);
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    vWriteDspShmDWRD (D_VOL_DEC4,       MASTER_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (D_TRIM_L_DEC4,    CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (D_TRIM_R_DEC4,    CH_VOL_SHM_DEFAULT);
#endif
    //FOR DSP PAN FUNCTION INITIAL VALUES, CC_AUD_PANFADE_SUPPORT
    vWriteDspShmDWRD (D_FADEVALUE,      CH_VOL_SHM_DEFAULT);

    // Operation mode
    if (AUD_GetTargetCountry() == COUNTRY_EU)
    {
        vWriteDspShmDWRD (W_AC3COMPMOD, 3); // 0:Custome 0, 1: custom 1, 2:Line, 3:RF
        vWriteDspShmDWRD (W_AC3COMPMOD_DEC2, 3);
    }
    else
    {
        vWriteDspShmDWRD (W_AC3COMPMOD, 2); // 0:Custome 0, 1: custom 1, 2:Line, 3:RF
        vWriteDspShmDWRD (W_AC3COMPMOD_DEC2, 2);
    }
    vWriteDspShmDWRD (D_AC3DYN_LOW, 0x007FFFFF); // 1dB
    vWriteDspShmDWRD (D_AC3DYN_HIGH, 0x007FFFFF); // 1dB
    vWriteDspShmDWRD (D_AC3DYN_LOW_DEC2, 0x007FFFFF); // 1dB
    vWriteDspShmDWRD (D_AC3DYN_HIGH_DEC2, 0x007FFFFF); // 1dB
#if defined(CC_AUD_SUPPORT_MS10) && !defined(CC_AUD_DSP_US_MODEL) && !defined(CC_AUD_DSP_CHINA_MODEL)
    vWriteDspShmBYTE (B_DDT_DUALMODE, 0); // stereo
    vWriteDspShmBYTE (B_DDT_COMPVAL, 100);   //100 percentage
    vWriteDspShmBYTE (B_DDT_COMPMODE, DMX_MS10_RF);   // RF mode
    vWriteDspShmBYTE (B_DDT_DUALMODE_DEC2, 0); // stereo
    vWriteDspShmBYTE (B_DDT_COMPVAL_DEC2,100);   //100 percentage
    vWriteDspShmBYTE (B_DDT_COMPMODE_DEC2, DMX_MS10_RF);   // RF mode
    vWriteDspShmBYTE (B_DDT_DUALMODE_DEC3, 0); // stereo
    vWriteDspShmBYTE (B_DDT_COMPVAL_DEC3,100);   //100 percentage
    vWriteDspShmBYTE (B_DDT_COMPMODE_DEC3, DMX_MS10_RF);   // RF mode
    if (IS_SW_SupportMS10() || IS_SW_SupportDDCO())
    {
        vWriteDspShmBYTE (B_DDT_CONTROL, 0); // set 1 to byass MDC
    }
    else
    {
        vWriteDspShmBYTE (B_DDT_CONTROL, 1); // set 1 to byass MDC
    }
#else
    vWriteDspShmBYTE (B_DDT_CONTROL, 1); // set 1 to byass MDC
#endif

#ifdef FRAC_SPEED_SUPPORT
    vWriteDspShmDWRD(D_PLAY_SPEED, 0x00000000); //sunman for play speed : initial set not on
#endif

    // Light added for AVC
    vWriteDspShmWORD (W_AVC_TARGET_LEV,         (-20));
    vWriteDspShmWORD (W_AVC_SILENCE_LEV,        (-98));
    vWriteDspShmWORD (W_AVC_MAX_GAIN_UP,        6);
    vWriteDspShmWORD (W_AVC_MAX_GAIN_DOWN,      40);
    vWriteDspShmWORD (W_AVC_FLAG,               3);     // 3: On, 0: off
    vWriteDspShmWORD (W_AVC_ATTACK_THRES,       AVC_ATTACK_THRES);
    vWriteDspShmWORD (W_AVC_ADJUST_RATE,        0x80);  // (8.8) format, 0x80 = 0.5 dB
    vWriteDspShmWORD (W_DSP_AVC_UI_ADJUST_RATE, 0x80);  // (16.8) format, 4dB / sec @ 32k Hz

    // Mixsound
    vWriteDspShmWORD (W_MIXSOUND_FLAG,         0);
    vWriteDspShmWORD (W_MIXSOUND_CFG,     0x1f00); // LR/C/LsRs mix, Mono, LPCM, 1X
    vWriteDspShmDWRD (D_MIXSOUND_PADDR,   u4GetMixSoundStartByteAddr());
    vWriteDspShmDWRD (D_MIXSOUND_GAIN, DRVCUST_OptGet(eAudioMixsoundGain)); // 0x20000 --> 0dB
    vWriteDspShmDWRD (D_MIXSOUND_PLEN, 0);
    _u4MixsoundLoop = DRVCUST_OptGet(eAudioMixsoundLength);

    // System master
    vWriteDspShmDWRD (D_STC_DIFF_LO,       (STC_DIFF_BOUND_032MS * 1 *(1+u4GetStcDiffFactor(AUD_STC_LO))) );      // 32ms (1 frame of AC-3 @ 48kHz)
    vWriteDspShmDWRD (D_STC_DIFF_HI,       (STC_DIFF_BOUND_064MS * 2 * (1+u4GetStcDiffFactor(AUD_STC_HI))) );  // 128ms (4 frame of AC-3 @ 48kHz)
    vWriteDspShmDWRD (D_STC_DIFF_WS,       (STC_DIFF_BOUND_500MS * (1+u4GetStcDiffFactor(AUD_STC_WS))) );      // 0.5 second
    vWriteDspShmDWRD (D_STC_THRESHOLD,      STC_THR_42DB);                // -42dB
    vWriteDspShmDWRD (D_STC_DIFF_LO_DEC2,   STC_DIFF_BOUND_032MS * 1);        // 32ms (1 frame of AC-3 @ 48kHz)
    vWriteDspShmDWRD (D_STC_DIFF_HI_DEC2,  (STC_DIFF_BOUND_064MS * 2));   // 128ms (4 frame of AC-3 @ 48kHz)
    vWriteDspShmDWRD (D_STC_DIFF_WS_DEC2,   STC_DIFF_BOUND_500MS);        // 0.5 second

    vWriteDspShmDWRD (D_STC_THRESHOLD_DEC2, STC_THR_42DB);                // -36dB
    vWriteDspShmDWRD (D_STC_DIFF_LO_DEC3,   STC_DIFF_BOUND_032MS * 1);        // 32ms (1 frame of AC-3 @ 48kHz)
    vWriteDspShmDWRD (D_STC_DIFF_HI_DEC3,  (STC_DIFF_BOUND_064MS * 2));   // 128ms (4 frame of AC-3 @ 48kHz)
    vWriteDspShmDWRD (D_STC_DIFF_WS_DEC3,   STC_DIFF_BOUND_500MS);        // 0.5 second
    vWriteDspShmDWRD (D_STC_THRESHOLD_DEC3, STC_THR_36DB);                // -36dB

    vWriteDspShmDWRD (D_1ST_PTS_PRS_PNT,        0);
    vWriteDspShmWORD (W_1ST_PTS_STCH,           0);
    vWriteDspShmWORD (W_1ST_PTS_STCL,           0);
    vWriteDspShmDWRD (D_1ST_PTS_PRS_PNT_DEC2,   0);
    vWriteDspShmWORD (W_1ST_PTS_STCH_DEC2,      0);
    vWriteDspShmWORD (W_1ST_PTS_STCL_DEC2,      0);
    vWriteDspShmDWRD (D_1ST_PTS_PRS_PNT_DEC3,   0);
    vWriteDspShmWORD (W_1ST_PTS_STCH_DEC3,      0);
    vWriteDspShmWORD (W_1ST_PTS_STCL_DEC3,      0);

    vWriteDspShmDWRD (D_STC_THRESHOLD_DEC2, STC_THR_36DB); // -36dB

    // Decoding start point
    vWriteDspShmDWRD (D_DECODING_STR_PNT,       _u4AFIFO[AUD_DEC_MAIN][0]);
    vWriteDspShmDWRD (D_DECODING_STR_PNT_DEC2,  _u4AFIFO[AUD_DEC_AUX][0]);
#ifdef CC_AUD_DEC3_USE_FIFO1 // USE_FIFO3 : dec3 use fifo3
    vWriteDspShmDWRD (D_DECODING_STR_PNT_DEC3,  _u4AFIFO[AUD_DEC_MAIN][0]);
#else
    vWriteDspShmDWRD (D_DECODING_STR_PNT_DEC3,  _u4AFIFO[AUD_DEC_THIRD][0]);
#endif

    // DSP Group6 Initial
    //Individual Speaker Config Setting
    vWriteDspShmWORD (W_AC3_SPKCFG,         0xFFFF);
    vWriteDspShmWORD (W_MP2_SPKCFG,         0xFFFF);

    //Address Setting
    vWriteDspShmDWRD (D_MEMBACKUPADDR,      CMPT_POST_TEMP_BUFFER);
    vWriteDspShmDWRD (D_3D_BUF_ADDR,        CMPT_UPMIX_BUFFER); // 0x0d000
    vWriteDspShmDWRD (D_DNMX_BUF_ADR,       CMPT_DONWMIX_BUFFER);

    // DSP Group7 Initial
    vWriteDspShmBYTE (B_SPECTRUMUP,         SPECTRUM_HIGH_ORDER); //Spectrum Order
    vWriteDspShmBYTE (B_SPECTRUMDOWN,       SPECTRUM_LOW_ORDER);

    vWriteDspShmWORD (W_WMAPACKET,           0);
    vWriteDspShmWORD (W_VORBISPACKET,        0);    ///CC_VORBIS_SUPPORT
    // Bass filter cutoff freq
    vWriteDspShmDWRD (D_CUTOFF_FREQ,            CUTOFF_FREQ);
    vWriteDspShmDWRD (D_CUTOFF_FREQ_DEC2,  CUTOFF_FREQ);

    vWriteDspShmBYTE (B_AC3AUTODNMIX,        0); // default is off
    vWriteDspShmBYTE (B_AC3AUTODNMIX_DEC2,        0); // default is off

    // DDC control
#ifdef CC_AUD_DVBT_AC3_BYPASS
    vWriteDspShmWORD(W_DDC_CONTROL,        0x1);
    vWriteDspShmWORD(W_DDC_CONTROL_DEC2,        0x1);
#else
//    if (IS_SupportEAC3() || IS_SupportDDCO() || IS_SupportMS10())
    if (IS_SW_SupportEAC3() || IS_SW_SupportMS10())
    {   // Support EAC3
        vWriteDspShmWORD(W_DDC_CONTROL,        0x2);
        vWriteDspShmWORD(W_DDC_CONTROL_DEC2,        0x2);
    }
    else
    {   // Supprot AC3 only
        vWriteDspShmWORD(W_DDC_CONTROL,        0x0);
        vWriteDspShmWORD(W_DDC_CONTROL_DEC2,        0x0);
    }
#endif
#ifdef CC_AUD_SUPPORT_MS10
    // DDC associated strem id
    vWriteDspShmBYTE (B_DDC_STRMID,        0);
    vWriteDspShmBYTE (B_DDC_STRMID_DEC2,        0);
    // DDC mixer
    vWriteDspShmBYTE (B_DDC_ASSOC_MIX,        0);
    vWriteDspShmBYTE (B_DDC_ASSOC_MIX_DEC2,        0);
#endif

    // initialization of B_EQBANDNUM or B_EQBANDNUM_DEC2
    vWriteDspShmBYTE (B_EQBANDNUM,           4);
    vWriteDspShmBYTE (B_EQBANDNUM_DEC2,      4);

    //Virtual Surround
#ifndef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
    vWriteDspShmBYTE (B_VSURRFLAG,    VSS_NONE);
#else
    vWriteDspShmWORD (W_VSURRFLAG,    VSS_NONE);
#endif
    vWriteDspShmBYTE (B_VSURRDELAYNUM,       5);
    vWriteDspShmDWRD (D_VSURRGAIN,     0x20000);
    vWriteDspShmDWRD (D_VSURRWIDEGAIN, 0x20000);
    #if 1
    vWriteDspShmDWRD (D_VSURR_CLARITY,GetAudioVsClarity());   //center gain,
    vWriteDspShmDWRD (D_VSURR_WIDTH,GetAudioVsWidth());   //surround gain,
    vWriteDspShmDWRD (D_VSURR_LR_GAIN,GetAudioVsLRGain()); //L/R gain,
    vWriteDspShmDWRD (D_VSURR_CROS_TALK,GetAudioVsXTalk()); //crosstalk,
    vWriteDspShmDWRD (D_VSURR_OUTPUT_GAIN,GetAudioVsOutputGain()); //output gain,
    vWriteDspShmDWRD (D_VSURR_BASS_GAIN,GetAudioVsBassGain()); //bass enhancement,
    vWriteDspShmDWRD (D_VSURR_FO,GetAudioVsFO()); //FO,
    #else
    vWriteDspShmDWRD (D_VSURR_CLARITY,   0xa0000);   //center gain,
    vWriteDspShmDWRD (D_VSURR_WIDTH,   0xa0000);   //surround gain,
    vWriteDspShmDWRD (D_VSURR_LR_GAIN,   0x200000); //L/R gain,
    vWriteDspShmDWRD (D_VSURR_CROS_TALK,   0xa0000); //crosstalk,
    vWriteDspShmDWRD (D_VSURR_OUTPUT_GAIN,   0xc0000); //output gain,
    vWriteDspShmDWRD (D_VSURR_BASS_GAIN,   0x150000); //bass enhancement,
    vWriteDspShmDWRD (D_VSURR_FO,   0x07f4300); //FO,
    #endif

    _rDspVars[AUD_DEC_MAIN].b3DSurrMode = SV_OFF;

#ifdef DSP_SUPPORT_SRSTSXT
	/*
		Rice: Define all flags by nibble for easy maintaining.

		bit  0~ 3:	Bypass (0: No, 1: Yes)
		bit  4~ 7:	TS Disable (0: No, 1: Yes)
		bit  8~11:	TS Headphone Mode (0: No, 1: Yes)
		bit 12~15:	Input Mode	(0~12)
		bit 16~19:	FOCUS Enable (0: No, 1: Yes)
		bit 20~23:	TruBass Enable (0: No, 1: Yes)
		bit 24~27:	TruBass Speaker Size (0~7)
		bit 28~31:	reserved
	*/
	vWriteDspShmDWRD(D_SRS_TSXT_FLAGS,		   DRVCUST_OptGet(eAudioTSXTFlag)); //default speaker size 2
	vWriteDspShmDWRD(D_SRS_TSXT_FC_Elevation, DRVCUST_OptGet(eAudioTSXTElev));
	vWriteDspShmDWRD(D_SRS_TSXT_TB_TBCtrl,	   DRVCUST_OptGet(eAudioTSXTTBCtrl));
	vWriteDspShmDWRD(D_SRS_TSXT_Input_gain,    DRVCUST_OptGet(eAudioTSXTInputGain));
#endif //DSP_SUPPORT_SRSTSXT

    /* SRS TSHD2CH */
    if(AUD_IsSupportSRS())    // SRS TSXT or SRS TSHD
    {
#ifdef SUPPORT_FLASH_AQ
        vWriteDspShmDWRD(D_SRS_TSHD_INPUT_GAIN, rAudFlashAQ.u4SRSInpg*0x66666); //-8dB
        vWriteDspShmDWRD(D_SRS_TSHD_OUTPUT_GAIN, DRVCUST_OptGet(eAudioTSHDOutputGain)); //0dB
        vWriteDspShmDWRD(D_SRS_TSHD_Surr_Level, rAudFlashAQ.u4SRSSurLev*0x66666); //60%
        UINT8 u1SPKSizeMode;
        switch(rAudFlashAQ.u4SRSSPKSize)
        {
            case 40:
              u1SPKSizeMode = 0;
              break;
            case 60:
              u1SPKSizeMode = 1;
              break;            
            case 80:
              u1SPKSizeMode = 2;
              break;            
            case 100:
              u1SPKSizeMode = 3;
              break;            
            case 120:
              u1SPKSizeMode = 4;
              break;            
            case 150:
              u1SPKSizeMode = 5;
              break;  
            case 200:
              u1SPKSizeMode = 6;
              break;
            case 250:
              u1SPKSizeMode = 7;
              break;
            case 300:
              u1SPKSizeMode = 8;
              break;
            case 400:
              u1SPKSizeMode = 9;
              break; 
            default :
              u1SPKSizeMode = 0;
              break;  
        }
        vWriteDspShmBYTE(B_SRS_TSHD_SPKSIZE, u1SPKSizeMode); //150Hz
        vWriteDspShmDWRD(D_SRS_TSHD_TB_TBCtrl, rAudFlashAQ.u4SRSTruBass*0x66666);      //50%
        vWriteDspShmDWRD(D_SRS_TSHD_DefCtrl, rAudFlashAQ.u4SRSDefCtrl*0x66666);          //30%
        vWriteDspShmDWRD(D_SRS_TSHD_FC_Elevation,  rAudFlashAQ.u4SRSFocus*0x66666); //50%
#else
        vWriteDspShmDWRD(D_SRS_TSHD_INPUT_GAIN, DRVCUST_OptGet(eAudioTSHDInputGain)); //-8dB
        vWriteDspShmDWRD(D_SRS_TSHD_OUTPUT_GAIN, DRVCUST_OptGet(eAudioTSHDOutputGain)); //0dB
        vWriteDspShmDWRD(D_SRS_TSHD_Surr_Level, DRVCUST_OptGet(eAudioTSHDSurrLvl) ); //60%
        vWriteDspShmBYTE(B_SRS_TSHD_SPKSIZE, (UINT8)DRVCUST_OptGet(eAudioTSHDSpkSize)); //150Hz
        vWriteDspShmDWRD(D_SRS_TSHD_TB_TBCtrl, DRVCUST_OptGet(eAudioTSHDTbCtrl));      //50%
        vWriteDspShmDWRD(D_SRS_TSHD_DefCtrl, DRVCUST_OptGet(eAudioTSHDDefCtrl));          //30%
        vWriteDspShmDWRD(D_SRS_TSHD_FC_Elevation,  DRVCUST_OptGet(eAudioTSHDElevCtrl)); //50%
#endif    
        vWriteDspShmBYTE(B_SRS_TSHD_SUBSPKSIZE, 0);
        vWriteDspShmBYTE(B_SRS_TSHD_COFREQ, 0);
        vWriteDspShmBYTE(B_SRS_TSHD_INP_TYPE, 1 );
        vWriteDspShmBYTE(B_SRS_TSHD_CH_DEST, 7);
        vWriteDspShmDWRD(D_SRS_TSHD_LimiterCtrl, 0x00600000);
        if(AUD_IsSupportSRS())    // SRS TSXT or SRS TSHD
        {
            /*
                ;ts_mode_flags
                ;bit-0  enable Surround/WOW3D block
                ;bit-1  enable TruBass        block
                ;bit-2  enable Definition     block
                ;bit-3  enable Focus          block
                ;bit-4  enable WOW Limiter    block
                ;bit-5  enable C output
                ;bit-6  enable LFE output
                ;bit-7  enable Headphone mode    
                ;bit-8  enable SubTruBass
                ;bit-9  enable TrueSurround  Mode ts_trusurround
                ;bit-10 enable PassiveMatrix Mode ts_passive
                ;bit-11 set space and center
                ;bit-12 LtRt
                ;bit-13 PL2_music
                ;bit-15 move lfe to left
            */
             vWriteDspShmDWRD(D_SRS_TSHD_FLAGS, 0x0000141f); 
             vWriteDspShmDWRD(D_SRS_TSHD_Space_Ctrl, 0x0050a3d7);
             vWriteDspShmDWRD(D_SRS_TSHD_Center_Ctrl, 0x002d70a4);
        }
        else
        {
             vWriteDspShmDWRD(D_SRS_TSHD_FLAGS, 0x0000141f);
             vWriteDspShmDWRD(D_SRS_TSHD_Space_Ctrl, 0x00666666);
             vWriteDspShmDWRD(D_SRS_TSHD_Center_Ctrl, 0x00400000);
        }
    }


#ifdef DSP_SUPPORT_SRSVIQ
    //sunman_viq
#if 0
    vWriteDspShmDWRD (D_VIQ_REF_LVL, 0xFFA00000);
    vWriteDspShmDWRD (D_VIQ_MODE, 0x0100); //default light mode
    vWriteDspShmDWRD (D_VIQ_MAX_GAIN, 0x00078000); //default 15
    vWriteDspShmDWRD (D_VIQ_INPUT_GAIN, 0x00200000); //default 1 (0.25) scale 2
    vWriteDspShmDWRD (D_VIQ_OUTPUT_GAIN, 0x00133333); //default (0x15)
#else
    vWriteDspShmDWRD (D_VIQ_REF_LVL, DRVCUST_OptGet(eAudioVIQRefLvl));//default minus for master vol
    vWriteDspShmDWRD (D_VIQ_MODE, DRVCUST_OptGet(eAudioVIQMode)); //default light mode
    vWriteDspShmDWRD (D_VIQ_MAX_GAIN, DRVCUST_OptGet(eAudioVIQMaxGain)); //default 15
    vWriteDspShmDWRD (D_VIQ_INPUT_GAIN, DRVCUST_OptGet(eAudioVIQInputGain)); //default 1 (0.25) scale 2
    vWriteDspShmDWRD (D_VIQ_OUTPUT_GAIN, DRVCUST_OptGet(eAudioVIQOutputGain)); //default (0x15)
    vWriteDspShmDWRD (D_VIQ_NOISE_TH, DRVCUST_OptGet(eAudioVIQNoiseTh)); //sunman_viq2
#endif
#endif

#ifdef DSP_SUPPORT_SRS_SSHD
#if 0
//sunman_peq
    vWriteDspShmWORD (W_SRS_STUDIO_FLAG,0x00);                                                                                  //ON/OFF
    vWriteDspShmDWRD (D_SRS_PEQ_INPUT_GAIN, 0x80000);
    vWriteDspShmDWRD (D_SRS_PEQ_OUTPUT_GAIN, 0x80000);
    vWriteDspShmWORD (W_SRS_PEQ_BANDS_1, 0x700);
//sunman_lmt
    vWriteDspShmDWRD (D_SRS_LMT_INPUT_GAIN, 0x200000);
    vWriteDspShmDWRD (D_SRS_LMT_OUTPUT_GAIN, 0x200000);
    vWriteDspShmDWRD (D_SRS_LMT_BOOST, 0x168f6);
    vWriteDspShmDWRD (D_SRS_LMT_LEVEL, 0x7fffff);
    vWriteDspShmDWRD (D_SRS_LMT_DELAY, 22 << 8);
#else
//sunman_peq
    vWriteDspShmWORD (W_SRS_STUDIO_FLAG,0xA00);                                                                                  //ON/OFF
    vWriteDspShmDWRD (D_SRS_PEQ_INPUT_GAIN, DRVCUST_OptGet(eAudioSRSPEQInputGain));
    vWriteDspShmDWRD (D_SRS_PEQ_OUTPUT_GAIN, DRVCUST_OptGet(eAudioSRSPEQOutputGain));
    vWriteDspShmWORD (W_SRS_PEQ_BANDS_1, (DRVCUST_OptGet(eAudioSRSPEQBands)<<8));
//sunman_lmt
    vWriteDspShmDWRD (D_SRS_LMT_INPUT_GAIN, DRVCUST_OptGet(eAudioSRSLmtInputGain));
    vWriteDspShmDWRD (D_SRS_LMT_OUTPUT_GAIN, DRVCUST_OptGet(eAudioSRSLmtOutputGain));
    vWriteDspShmDWRD (D_SRS_LMT_BOOST, DRVCUST_OptGet(eAudioSRSLmtRefBoost));
    vWriteDspShmDWRD (D_SRS_LMT_LEVEL, DRVCUST_OptGet(eAudioSRSLmtRefLevel));
    vWriteDspShmDWRD (D_SRS_LMT_DELAY, (DRVCUST_OptGet(eAudioSRSLmtRefDelay) << 8));
	vWriteDspShmDWRD (D_SRS_LMT_COEFF, DRVCUST_OptGet(eAudioSRSLmtRefCoeff));
    vWriteDspShmDWRD (D_SRS_LMT_THRESH, DRVCUST_OptGet(eAudioSRSLmtRefThresh));
#endif
#endif

#ifdef DSP_SUPPORT_SRS_PURESOUND 
       //bofeng_aeq
	   vWriteDspShmWORD (W_SRS_PURESOUND_FLAG,0x1A00); 
	   vWriteDspShmDWRD (D_SRS_AEQ_INPUT_GAIN, DRVCUST_OptGet(eAudioSRSAeqInputGain));
	   vWriteDspShmDWRD (D_SRS_AEQ_OUTPUT_GAIN, DRVCUST_OptGet(eAudioSRSAeqOutputGain));
	   vWriteDspShmDWRD (D_SRS_AEQ_FILTER_SIZE, DRVCUST_OptGet(eAudioSRSAeqFilterSize));
       //bofeng_lmt
	   vWriteDspShmDWRD (D_SRS_LMT_INPUT_GAIN, DRVCUST_OptGet(eAudioSRSLmtInputGain));
	   vWriteDspShmDWRD (D_SRS_LMT_OUTPUT_GAIN, DRVCUST_OptGet(eAudioSRSLmtOutputGain));
	   vWriteDspShmDWRD (D_SRS_LMT_BOOST, DRVCUST_OptGet(eAudioSRSLmtRefBoost));
	   vWriteDspShmDWRD (D_SRS_LMT_LEVEL, DRVCUST_OptGet(eAudioSRSLmtRefLevel));
	   vWriteDspShmDWRD (D_SRS_LMT_DELAY, (DRVCUST_OptGet(eAudioSRSLmtRefDelay) << 8));
	   vWriteDspShmDWRD (D_SRS_LMT_COEFF, DRVCUST_OptGet(eAudioSRSLmtRefCoeff));
	   vWriteDspShmDWRD (D_SRS_LMT_THRESH, DRVCUST_OptGet(eAudioSRSLmtRefThresh));
		   
#endif


   // FIXME for testing
   vWriteDspShmWORD (W_PINKNOISE,       0x06ff);
   // Initialize for PCM dec/dec2
   vWriteDspShmWORD (W_PCM_INPUT_CHANNEL_NUM,           0);
   vWriteDspShmWORD (W_PCM_INPUT_CH_ASSIGNMENT,         0);
   vWriteDspShmWORD (W_PCM_INPUT_CHANNEL_NUM_DEC2,      0);
   vWriteDspShmWORD (W_PCM_INPUT_CH_ASSIGNMENT_DEC2,    0);

#ifdef USE_16BIT_ALIGNMENT
   vWriteDspShmWORD (W_PCM_INPUT_TYPE,2);
#else
   vWriteDspShmWORD (W_PCM_INPUT_TYPE,0);
#endif

   vWriteDspShmWORD (W_PCM_INPUT_TYPE_DEC2,0);
   vWriteDspShmWORD (W_PCM_INPUT_SAMPLING_RATE,8);
   vWriteDspShmWORD (W_PCM_INPUT_SAMPLING_RATE_DEC2,8);

#ifdef CC_MT5391_AUD_3_DECODER
   vWriteDspShmWORD (W_PCM_INPUT_CHANNEL_NUM_DEC3,      0);
   vWriteDspShmWORD (W_PCM_INPUT_CH_ASSIGNMENT_DEC3,    0);
   vWriteDspShmWORD (W_PCM_INPUT_TYPE_DEC3,0);
   vWriteDspShmWORD (W_PCM_INPUT_SAMPLING_RATE_DEC3,8);
#endif //CC_MT5391_AUD_3_DECODER

#ifdef CC_AUD_4_DECODER_SUPPORT
    vWriteDspShmWORD (W_PCM_INPUT_CHANNEL_NUM_DEC4,      0);
    vWriteDspShmWORD (W_PCM_INPUT_CH_ASSIGNMENT_DEC4,    0);
    vWriteDspShmWORD (W_PCM_INPUT_TYPE_DEC4,0);
    vWriteDspShmWORD (W_PCM_INPUT_SAMPLING_RATE_DEC4,8);
#endif //CC_AUD_4_DECODER_SUPPORT

   vWriteDspShmWORD (W_PCM_PREBUF_DELAY_BANK, 0); // 256 sample/unit,
                                                  //ie. 5.33 ms/uinit with 48 KHz
#ifdef DSP_SUPPORT_NPTV
   //all ATV audio shared shm
    vWriteDspShmWORD (W_USER_MODE,      0);
#ifdef __MODEL_slt__
    vWriteDspShmWORD (W_SOURCE_MODE,    0);
#else
    vWriteDspShmWORD (W_SOURCE_MODE,    1);
#endif //__MODEL_slt__
    vWriteDspShmWORD (W_ADC_GAIN,     0x8018);
    vWriteDspShmBYTE (B_HDEV_MODE,
        (((UINT8)prMtsFactory->fgHdevMode << 2) |
        ((UINT8)prA2Factory->fgHdevMode << 1) |
        ((UINT8)prPalFactory->fgHdevMode << 0)));

#ifdef __MODEL_slt__
    vWriteDspShmBYTE (B_CARRIER_SHIFT_MODE,   0);
#else
    vWriteDspShmBYTE (B_CARRIER_SHIFT_MODE,
        (((UINT8)prMtsFactory->fgCarrierShift << 2) |
        ((UINT8)prA2Factory->fgCarrierShift << 1)|
        ((UINT8)prPalFactory->fgCarrierShift << 0)));
#endif //__MODEL_slt__

    vWriteDspShmBYTE (B_FM_SATU_MUTE_MODE,
        (((UINT8)prMtsFactory->fgSatuMuteMode<< 2) |
        ((UINT8)prA2Factory->fgSatuMuteMode << 1) |
        ((UINT8)prPalFactory->fgSatuMuteMode << 0)));
    vWriteDspShmBYTE (B_FM_MUTE_MODE,
        (((UINT8)prMtsFactory->fgCarrierMuteMode<< 2) |
        ((UINT8)prA2Factory->fgCarrierMuteMode << 1) |
        ((UINT8)prPalFactory->fgCarrierMuteMode << 0)));
    vWriteDspShmBYTE (B_NON_EU_FM_MODE,
        (((UINT8)prA2Factory->fgNonEuMode) |
        ((UINT8)prPalFactory->fgNonEuMode)));
    vWriteDspShmBYTE(B_NICAM_I_BOOST_FLAG,
        ((UINT8)prPalFactory->fgNicamIBoost));

    // MTS Parameters
    vWriteDspShmWORD(W_MTS_NUM_CHECK, prMtsFactory->u2NumCheck);
    vWriteDspShmWORD(W_MTS_STEREO_MID, prMtsFactory->u2PilotLowerThd);
    vWriteDspShmWORD(W_MTS_STEREO_CON_MID, prMtsFactory->u2PilotHigherThd);
    vWriteDspShmWORD(W_MTS_NUM_PILOT, prMtsFactory->u2NumPilot);
    vWriteDspShmWORD(W_MTS_NUM_SAP, prMtsFactory->u2NumSap);
    vWriteDspShmWORD(W_MTS_SAP_MID, prMtsFactory->u2SapLowerThd);
    vWriteDspShmWORD(W_MTS_SAP_CON_MID, prMtsFactory->u2SapHigherThd);
    vWriteDspShmDWRD(D_SAP_FINE_TUNE_VOLUME,
        (VOLUME_FINE_TUNE[vFineTuneVolTblIdxChk(prMtsFactory->u1SapPrescale)]) << 8);
    vWriteDspShmDWRD(D_MTS_FINE_TUNE_VOLUME,
        (VOLUME_FINE_TUNE[vFineTuneVolTblIdxChk(prMtsFactory->u1MonoStereoPrescale)]) << 8);
    vWriteDspShmBYTE (B_FM_MUTE_THRESHOLD_HIGHT_MTS,prMtsFactory->u1CarrierMuteHigherThd);
    vWriteDspShmBYTE (B_FM_MUTE_THRESHOLD_LOW_MTS, prMtsFactory->u1CarrierMuteLowerThd);
    vWriteDspShmBYTE (B_MTS_PILOT_OFFSET_DETECTION, (UINT8)prMtsFactory->fgPilotOffsetDetect);
    vWriteDspShmBYTE (B_MTS_MUTE_SAP_LOW,   prMtsFactory->u1SapNoiseMuteLow);
    vWriteDspShmBYTE (B_MTS_MUTE_SAP_HIGH,  prMtsFactory->u1SapNoiseMuteHigh);
    vWriteDspShmBYTE (B_SATU_MUTE_THRESHOLD_HIGH,  prMtsFactory->u1SatuMuteHigherThd );
    vWriteDspShmBYTE (B_SATU_MUTE_THRESHOLD_LOW,  prMtsFactory->u1SatuMuteLowerThd );
    vWriteDspShmBYTE (B_SAP_HP_MUTE_HIGH,   35);
    vWriteDspShmBYTE (B_SAP_HP_MUTE_LOW,  10);
    ///vWriteDspShmBYTE (B_SAP_FILTER_SEL,   0);
    vWriteDspShmBYTE (B_SAP_FILTER_SEL,   (((prMtsFactory->u1SAPFilterType)<<1)|(prMtsFactory->u1SAPFilterMode)));
    vWriteDspShmBYTE (B_SAP_RATIO,   0x16); //Original :0x1C  //US FT issue ... SAP sensitivity
#ifdef CC_AUD_MTS_OUTPUT_LEVEL_GAIN_SUPPORT
    vWriteDspShmDWRD(D_MTS_OUTPUT_LEVEL_GAIN, prMtsFactory->u2OutputLevelGain);
#endif

    // FMFM Parameters
    vWriteDspShmWORD(W_FMFM_NUM_CHECK, 4);
    vWriteDspShmWORD(W_FMFM_NUM_DOUBLE_CHECK, 9);
    vWriteDspShmWORD(W_FMFM_MONO_WEIGHT, 1);
    vWriteDspShmWORD(W_FMFM_STEREO_WEIGHT, 1);
    vWriteDspShmWORD(W_FMFM_DUAL_WEIGHT, 1);
    vWriteDspShmDWRD(D_FMFM_MONO_FINE_TUNE_VOLUME, (0x2000) << 8);
    vWriteDspShmDWRD(D_FMFM_DUAL_FINE_TUNE_VOLUME, (0x2000) << 8);
    vWriteDspShmWORD(W_FMFM_DETECT_CONFID, 7);
    vWriteDspShmBYTE (B_FM_MUTE_THRESHOLD_HIGHT_EIAJ,   0x22);
    vWriteDspShmBYTE (B_FM_MUTE_THRESHOLD_LOW_EIAJ,   0x20);

    // A2 Parameters
    vWriteDspShmWORD(W_A2_NUM_CHECK, prA2Factory->u2NumCheck);
    vWriteDspShmWORD(W_A2_NUM_DOUBLE_CHECK,  prA2Factory->u2NumDoubleCheck);
    vWriteDspShmWORD(W_A2_MONO_WEIGHT, prA2Factory->u2MonoWeight);
    vWriteDspShmWORD(W_A2_STEREO_WEIGHT,prA2Factory->u2StereoWeight);
    vWriteDspShmWORD(W_A2_DUAL_WEIGHT, prA2Factory->u2DualWeight);
    vWriteDspShmDWRD(D_A2_FINE_TUNE_VOLUME,
        (VOLUME_FINE_TUNE[vFineTuneVolTblIdxChk(prA2Factory->u1A2Prescale)]) << 8);
    vWriteDspShmBYTE (B_FM_MUTE_THRESHOLD_HIGHT_A2, prA2Factory->u1CarrierMuteHigherThd);
    vWriteDspShmBYTE (B_FM_MUTE_THRESHOLD_LOW_A2,  prA2Factory->u1CarrierMuteLowerThd);
    vWriteDspShmBYTE (B_MANUAL_MTS_SW,0);
    vWriteDspShmBYTE (B_MANUAL_OUTPUT_MATRIX,0);
    vWriteDspShmBYTE (B_MANUAL_OUTPUT_MATRIX_DEC2,0);
    vWriteDspShmWORD (W_A2_MONO_STEREO_THRES_EU,   prA2Factory->u2Mono2StereoEU);
    vWriteDspShmWORD (W_A2_STEREO_MONO_THRES_EU, prA2Factory->u2Stereo2MonoEU);
    vWriteDspShmWORD (W_A2_MONO_STEREO_THRES_M,   prA2Factory->u2Mono2StereoM);
    vWriteDspShmWORD (W_A2_STEREO_MONO_THRES_M, prA2Factory->u2Stereo2MonoM);
    vWriteDspShmWORD (W_A2_WEAK_RF_LVL_M2S_THRES_EU, prA2Factory->u2WeakRfLvlM2SEU);
    vWriteDspShmWORD (W_A2_WEAK_RF_LVL_S2M_THRES_EU, prA2Factory->u2WeakRfLvlS2MEU);


    // PAL Parameters
    vWriteDspShmWORD(W_PAL_CORRECT_THRESHOLD, prPalFactory->u2CorrectThd);
    vWriteDspShmWORD(W_PAL_TOTAL_SYNC1_LOOP, prPalFactory->u2TotalSyncLoop);
    vWriteDspShmWORD(W_PAL_ERR_THRESHOLD, prPalFactory->u2ErrorThd);
    vWriteDspShmWORD(W_PAL_PARITY_ERR_THRESHOLD, prPalFactory->u2ParityErrorThd);
    vWriteDspShmWORD(W_PAL_EVERY_NUM_FRAMES, prPalFactory->u2EveryNumFrames);
    vWriteDspShmBYTE (B_AM_MUTE_MODE,   prPalFactory->fgAmMuteMode);
    vWriteDspShmBYTE (B_AM_MUTE_THRESHOLD_LOW,   prPalFactory->u1AmMuteLowerThd);
    vWriteDspShmBYTE (B_AM_MUTE_THRESHOLD_HIGHT,   prPalFactory->u1AmMuteHigherThd);
    vWriteDspShmBYTE (B_FM_MUTE_THRESHOLD_HIGHT_PAL,   prPalFactory->u1CarrierMuteHigherThd);
    vWriteDspShmBYTE (B_FM_MUTE_THRESHOLD_LOW_PAL, prPalFactory->u1CarrierMuteLowerThd);
    vWriteDspShmDWRD(D_PAL_FINE_TUNE_VOLUME,
        (VOLUME_FINE_TUNE[vFineTuneVolTblIdxChk(prPalFactory->u1PalPrescale)]) << 8);
    vWriteDspShmDWRD(D_AM_FINE_TUNE_VOLUME,
        (VOLUME_FINE_TUNE[vFineTuneVolTblIdxChk(prPalFactory->u1AmPrescale)]) << 8);
    vWriteDspShmDWRD(D_NICAM_FINE_TUNE_VOLUME,
        (VOLUME_FINE_TUNE[vFineTuneVolTblIdxChk(prPalFactory->u1NicamPrescale)]) << 8);

    //for PAL/A2, Dynamic detection control
    vWriteDspShmBYTE (B_DYNAMIC_DETECTIO_SWITCH, 0x00);   // bit 0 : 1 -> disable PAL -> A2 dynamic detection for PAL decoder
    //vWriteDspShmBYTE (B_DYNAMIC_DETECTIO_SWITCH, 0x11); // bit 4 : 1 -> disable A2 -> PAL dynamic detection for A2 decoder   

    //for PAL, check FM BAD SIGNAL
    vWriteDspShmBYTE (B_FM_BAD_SIGNAL, 0x0);
    vWriteDspShmBYTE (B_CANON_PLUS,   0);

    // Detection Parameters
    vWriteDspShmBYTE(B_DETECT_DEFAULT, 0);  // 0: PAL_DK, 1: SECAM-L
    vWriteDspShmDWRD(D_DETECTOR_FM_NOISE_TH  , 0x42000);

    //FM RADIO Parameters
    vWriteDspShmDWRD(D_FM_RDO_INBAND_TH  , 0x3000);
    vWriteDspShmDWRD(D_FM_RDO_NOISE_TH  , 0x15000);
    vWriteDspShmDWRD(D_FMRADIO_FINE_TUNE_VOLUME  , 0xcbd00);

#ifdef CC_AUD_BBE_SUPPORT
    // BBE default settings
    vWriteDspShmBYTE (B_BBE_FLAG, 0x0);
    vWriteDspShmBYTE (B_BBE_LEVEL, (UINT8)DRVCUST_OptGet(eAudioBbeLevel));
    vWriteDspShmBYTE (B_BBE_PROCESS, (UINT8)DRVCUST_OptGet(eAudioBbeProcess));
    vWriteDspShmBYTE (B_BBE_LO_CONTOUR, (UINT8)DRVCUST_OptGet(eAudioBbeLoContour));
    vWriteDspShmBYTE (B_BBE_3D_GAIN, (UINT8)DRVCUST_OptGet(eAudioBbe3dGain));
#endif

    //Audio Description setting
    vWriteDspShmBYTE (B_ADFLAG, (UINT8)DRVCUST_OptGet(eAudioADFlag));

    // Limiter mode
    vWriteDspShmWORD (W_LIMITER_MODE, GetAudioLimiterMode());
#if defined(CC_AUD_DSP_SUPPORT_AUDYSSEY_TEST_MODE)
    vWriteDspShmWORD (W_LIMITER_FLAG, 0x0);
#elif defined(DSP_SUPPORT_SRS_SSHD)
    vWriteDspShmWORD (W_LIMITER_FLAG, 0x20); //limiter[5] before volume compensate
#elif defined(DSP_SUPPORT_SRS_PURESOUND)
    vWriteDspShmWORD (W_LIMITER_FLAG, 0x0); //
#else
    vWriteDspShmWORD (W_LIMITER_FLAG, 0xffff);
#endif

    vWriteDspShmDWRD (D_LIMITER_FIX_THRESHOLD, GetAudioLimiterThreshold()); // default is 0x100000
#ifdef CC_RATIO_LIMITER
    vWriteDspShmDWRD (D_LIMITER_GAIN_RATIO, 0x07fffff); // default is 0x7fffff
#endif

#ifdef CC_AUD_HPF_SUPPORT
#ifdef DSP_SUPPORT_SRS_SSHD
    // Audio post-processing initialization
    vWriteDspShmBYTE(B_HPF_ENABLE, 1);
    vWriteDspShmWORD(W_HPF_FC, DRVCUST_OptGet(eAudioMtkHpfFc));  // Fc = 100Hz
    vWriteDspShmWORD(W_HPF2_FC, DRVCUST_OptGet(eAudioMtkHpf2Fc));  // Fc = 120Hz
#else
    // Audio post-processing initialization
    vWriteDspShmBYTE(B_HPF_ENABLE, 1);
    vWriteDspShmWORD(W_HPF_FC, DRVCUST_OptGet(eAudioMtkHpfFc));  // Fc = 100Hz
    vWriteDspShmWORD(W_HPF2_FC, DRVCUST_OptGet(eAudioMtkHpf2Fc));  // Fc = 120Hz
#endif
#endif

    // PEQ relative setting
#ifdef CC_AUD_PEQ_SUPPORT
#ifdef SUPPORT_FLASH_AQ
    vWriteDspShmBYTE(B_PEQ_ENABLE, rAudFlashAQ.u4PEQFlag);
#else
#ifdef CC_AUD_PEQ_INIT_ON
    vWriteDspShmBYTE(B_PEQ_ENABLE, 1);
#else
    vWriteDspShmBYTE(B_PEQ_ENABLE, 0);
#endif
#endif
#endif

#ifdef CC_AUD_SPEAKER_HEIGHT
#ifndef CC_AUD_SPEAKER_HEIGHT_MODE
    vWriteDspShmBYTE(B_SPH_ENABLE, 0);
#else
    vWriteDspShmBYTE(B_SPH_MODE, 0);
#endif
#endif

#ifdef CC_AUD_PEQ_SUPPORT
#ifdef SUPPORT_FLASH_AQ
    vWriteDspShmWORD(W_PEQ_SET0_FC  , rAudFlashAQ.au4PEQFrequency[0]);
    vWriteDspShmWORD(W_PEQ_SET1_FC  , rAudFlashAQ.au4PEQFrequency[1]);
    vWriteDspShmWORD(W_PEQ_SET2_FC  , rAudFlashAQ.au4PEQFrequency[2]);
    vWriteDspShmWORD(W_PEQ_SET3_FC  , rAudFlashAQ.au4PEQFrequency[3]);
    vWriteDspShmWORD(W_PEQ_SET4_FC  , rAudFlashAQ.au4PEQFrequency[4]);
    vWriteDspShmWORD(W_PEQ_SET5_FC  , rAudFlashAQ.au4PEQFrequency[5]);
    vWriteDspShmWORD(W_PEQ_SET6_FC  , rAudFlashAQ.au4PEQFrequency[6]);
    vWriteDspShmWORD(W_PEQ_SET7_FC  , rAudFlashAQ.au4PEQFrequency[7]);
    vWriteDspShmWORD(W_PEQ_SET8_FC  , rAudFlashAQ.au4PEQFrequency[8]);

    for (i=0; i<8; i++)
    {
        vWriteDspShmDWRD(D_PEQ_SET0_GAIN + i*8, rAudFlashAQ.au4PEQGain[i]);
           vWriteDspShmWORD(W_PEQ_SET0_BW + i*8, rAudFlashAQ.au4PEQBW[i]);
    }
    vWriteDspShmDWRD(D_PEQ_SET8_GAIN, rAudFlashAQ.au4PEQGain[8]);
    vWriteDspShmWORD(W_PEQ_SET8_BW, rAudFlashAQ.au4PEQBW[8]);
#else

    // FIXME
    vWriteDspShmWORD(W_PEQ_SET0_FC  , 50);
    vWriteDspShmWORD(W_PEQ_SET1_FC  , 100);
    vWriteDspShmWORD(W_PEQ_SET2_FC  , 200);
    vWriteDspShmWORD(W_PEQ_SET3_FC  , 300);
    vWriteDspShmWORD(W_PEQ_SET4_FC  , 1000);
    vWriteDspShmWORD(W_PEQ_SET5_FC  , 3000);
    vWriteDspShmWORD(W_PEQ_SET6_FC  , 5000);
    vWriteDspShmWORD(W_PEQ_SET7_FC  , 15000);
    vWriteDspShmWORD(W_PEQ_SET8_FC  , 18000);

    for (i=0; i<8; i++)
    {
        vWriteDspShmDWRD(D_PEQ_SET0_GAIN + i*8, 0x200000);
           vWriteDspShmWORD(W_PEQ_SET0_BW + i*8, 500);
    }
    vWriteDspShmDWRD(D_PEQ_SET8_GAIN, 0x200000);
    vWriteDspShmWORD(W_PEQ_SET8_BW, 500);
#endif    
#endif

#ifdef CC_AUD_VBASS_SUPPORT
    // Virtual Bass
    vWriteDspShmBYTE(B_VIR_BASS_ENABLE, 0);
    vWriteDspShmDWRD(D_VIR_BASS_GAIN, 0x1000);
    vWriteDspShmDWRD(D_VIR_BASS_GAIN_2, 0x300);
#endif

#ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
    vWriteDspShmBYTE(B_CDNOTCH_FLAG, 0);
    vWriteDspShmBYTE(B_CDNOTCH_USER_Q, 0);
    vWriteDspShmWORD(W_CDNOTCH_USER_FC, 0);
#endif

#ifdef  KARAOKE_SUPPORT
    vWriteDspShmBYTE(B_KEYSHIFT_FLAG, 0);
    vWriteDspShmBYTE(B_KEYSHIFT_KEY, 0);
#endif

#ifdef CC_AUD_CLIPPER_SUPPORT
    // Clipper enable
    vWriteDspShmBYTE(B_CLIPPER_ENABLE, 0);

    vWriteDspShmDWRD(D_CLIPPER_C_VALUE,   CLIPPER_DEFAULT_VALUE);
    vWriteDspShmDWRD(D_CLIPPER_L_VALUE,   CLIPPER_DEFAULT_VALUE);
    vWriteDspShmDWRD(D_CLIPPER_R_VALUE,   CLIPPER_DEFAULT_VALUE);
    vWriteDspShmDWRD(D_CLIPPER_LS_VALUE,  CLIPPER_DEFAULT_VALUE);
    vWriteDspShmDWRD(D_CLIPPER_RS_VALUE,  CLIPPER_DEFAULT_VALUE);
    vWriteDspShmDWRD(D_CLIPPER_SW_VALUE,  CLIPPER_DEFAULT_VALUE);
    vWriteDspShmDWRD(D_CLIPPER_CH7_VALUE, CLIPPER_DEFAULT_VALUE);
    vWriteDspShmDWRD(D_CLIPPER_CH8_VALUE, CLIPPER_DEFAULT_VALUE);
    vWriteDspShmDWRD(D_CLIPPER_CH9_VALUE, CLIPPER_DEFAULT_VALUE);
    vWriteDspShmDWRD(D_CLIPPER_CH10_VALUE,CLIPPER_DEFAULT_VALUE);
#endif

#endif //DSP_SUPPORT_NPTV

#ifdef CC_AUD_USE_NVM
    AUD_NVM_Init_From_EEPROM();
#endif  //CC_AUD_USE_NVM

    //DDCO flag
    vWriteDspShmBYTE (B_DDCO_FLAG,0x0);
    vWriteDspShmBYTE (B_DDCO_AGC_FLAG,0x0);
    vWriteDspShmBYTE (B_DDCO_LFE_LPF_FLAG,0x0);

    // DTS flag
    vWriteDspShmBYTE (B_DTS_ENCODER_FLAG, 0x0);
    vWriteDspShmBYTE (B_DTS_ENCODER_AMODE, 0x0);

//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
    // upload data
    vWriteDspShmWORD (W_UPLOAD_DATA_FLAG, 0x0);
    // Block Number     PCM=128,     MP3=128
    vWriteDspShmBYTE (B_UPLOAD_BLOCK_NUM, 128);
    // Block Size,      PCM=0x400,  MP3=0x3C0(320kbps)/0x240(192kbps)/0x180(128kbps)
    vWriteDspShmWORD (W_UPLOAD_BLOCK_SIZE, 0x400)
    vWriteDspShmBYTE (B_UPLOAD_BLK_INT_CNT, 16);
//#endif

#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
    //ADV flag
    vWriteDspShmBYTE (B_ADV_FLAG,0x0); //ON/OFF
    vWriteDspShmDWRD (D_ADV_chCalbGain_L,DRVCUST_OptGet(eAudioAdvChCalbGain_L));//0x8fb: dB,(10.14), FOR 2.1V=0dB
    vWriteDspShmDWRD (D_ADV_chCalbGain_R,DRVCUST_OptGet(eAudioAdvChCalbGain_R));//0x44f: dB,(10.14)

    vWriteDspShmBYTE (B_ABX_ENABLE, 0);      //Default On for testing
    vWriteDspShmBYTE (B_ABX_FILTER_SET, DRVCUST_OptGet(eAudioAbxFilterSet)); //0: default use FILTER Set 1
    vWriteDspShmBYTE (B_ABX_DRY_GAIN_INDX, DRVCUST_OptGet(eAudioAbxDryGainIndx)); //0: default 0dB
    vWriteDspShmBYTE (B_ABX_WET_GAIN_INDX, DRVCUST_OptGet(eAudioAbxWetGainIndx)); //6: default 3dB

    vWriteDspShmBYTE (B_AEQ_FLAG,0);
#endif

    vWriteDspShmBYTE (B_SBCENC_SAMPLE_FREQ,3);
    vWriteDspShmBYTE (B_SBCENC_BLK_NUM_1,(16-1));
    #if 1
    //for 48K/Stereo, frame_len = 12 + 2 * bitpool = 128 bytes (per 128 samples/2 channel)
    vWriteDspShmBYTE (B_SBCENC_CH_NUM_1,(2-1));
    vWriteDspShmBYTE (B_SBCENC_BITPOOL,58); //53
    #else
    //for 48K/Mono, frame_len = 8 + 2 * bitpool = 64 bytes (per 128 samples/1 channel)
    vWriteDspShmBYTE (B_SBCENC_CH_NUM_1,(1-1));
    vWriteDspShmBYTE (B_SBCENC_BITPOOL,28);
    #endif

#if 1//added by ling for DTS
    vWriteDspShmBYTE (B_DTS_BC_LFE_MIXTO_FRONT,0x0);
    vWriteDspShmBYTE (B_DTS_BC_DRC,0x9);
    vWriteDspShmBYTE (B_DTS_BC_Dialnrom,0x9);

    vWriteDspShmBYTE (B_DTS_BC_LFE_MIXTO_FRONT_DEC2,0x1);
    vWriteDspShmBYTE (B_DTS_BC_DRC_DEC2,0x9);
    vWriteDspShmBYTE (B_DTS_BC_Dialnrom_DEC2,0x9);

    vWriteDspShmDWRD(D_DTS_BC_DRC_LOW,   0x2AAAA7);
    vWriteDspShmDWRD(D_DTS_BC_DRC_MEDIUM,   0x55554e);
    vWriteDspShmDWRD(D_DTS_BC_DRC_HIGH,   0x7fffff);
    vWriteDspShmDWRD(D_DTS_BC_Dialnorm_LOW,   0x44444);
    vWriteDspShmDWRD(D_DTS_BC_Dialnorm_MEDIUM,   0x88888);
    vWriteDspShmDWRD(D_DTS_BC_Dialnorm_HIGH,   0x7fffff);
#endif

#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
    vWriteDspShmWORD (W_SYSTEM_CTRL2, u2ReadDspShmWORD(W_SYSTEM_CTRL2) | 0x80); //enable dec4 hook to mixsnd2
#endif
#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)) //CC_AUD_PLL_HALF
    vWriteDspShmWORD (W_SYSTEM_CTRL2, u2ReadDspShmWORD(W_SYSTEM_CTRL2) | 0x4);
#endif

#ifdef CC_AUD_FIR_SUPPORT
    vWriteDspShmBYTE(B_FIR_ENABLE, 0);
#endif

}

/***************************************************************************
     Function : vDSPInit
  Description : RISC Initialize DSP DRAM Value when Powen On
    Parameter : None
    Return    : None
***************************************************************************/
void vDspInit (void)
{
    UINT16 u2DspData;
    UINT8  iLoopVar;

    //CTRL1 NUM_1
    vWriteDspWORD (ADDR_RC2D_CTRL_1_NUM_1, CTRL_1_NUM_1);

    //Interrupt Control
    u2DspData = u2ReadDspShmWORD (W_INTCTRL);
    u2DspData = u2DspData | RAM_EXT_RISC_INT | TRAP_PRESET_ON ;
    vWriteDspWORD (ADDR_RC2D_INTERRUPT_CTRL, u2DspData);
    // Light add for internal buffer control
    vWriteDspWORD (ADDR_RC2D_SYSTEM_CTRL,    u2ReadDspShmWORD (W_SYSTEM_CTRL));
    vWriteDspWORD (ADDR_RC2D_SYSTEM_CTRL2,    u2ReadDspShmWORD (W_SYSTEM_CTRL2));
    vWriteDspWORD (ADDR_RD2D_INTERNAL_BUF_IN_FLAG, u2ReadDspShmWORD (W_INTERNAL_BUF_IN_FLAG));
    vWriteDspWORD (ADDR_RC2D_SYSTEM_SETUP,   u2ReadDspShmWORD (W_SYSTEM_SETUP));
    vWriteDspWORD (ADDR_RC2D_SYSTEM_SETUP_DEC2, u2ReadDspShmWORD (W_SYSTEM_SETUP_DEC2));
#ifdef CC_MT5391_AUD_3_DECODER
    vWriteDspWORD (ADDR_RC2D_SYSTEM_SETUP_DEC3,
        u2ReadDspShmWORD (W_SYSTEM_SETUP_DEC3));
    vWriteDspWORD (ADDR_RD2D_INTERNAL_BUF2_IN_FLAG, u2ReadDspShmWORD (W_INTERNAL_BUF2_IN_FLAG));
#endif

#ifdef CC_AUD_4_DECODER_SUPPORT
    vWriteDspWORD (ADDR_RC2D_SYSTEM_SETUP_DEC4,
        u2ReadDspShmWORD (W_SYSTEM_SETUP_DEC4));
    vWriteDspWORD (ADDR_RD2D_INTERNAL_BUF3_IN_FLAG, u2ReadDspShmWORD (W_INTERNAL_BUF3_IN_FLAG));
#endif

    //Volume
    WriteDspCommDram (ADDR_RC2D_MASTER_VOLUME,u4ReadDspShmDWRD (D_VOL));
    WriteDspCommDram (ADDR_RC2D_MASTER_VOLUME_BACKUP,u4ReadDspShmDWRD (D_VOL));
    WriteDspCommDram (ADDR_RC2D_MASTER_VOLUME_DEC2,u4ReadDspShmDWRD (D_VOL_DEC2));
    WriteDspCommDram (ADDR_RC2D_MASTER_VOLUME_BACKUP_DEC2,u4ReadDspShmDWRD (D_VOL_DEC2));
#ifdef CC_MT5391_AUD_3_DECODER
    WriteDspCommDram (ADDR_RC2D_MASTER_VOLUME_DEC3,u4ReadDspShmDWRD (D_VOL_DEC3));
    WriteDspCommDram (ADDR_RC2D_MASTER_VOLUME_BACKUP_DEC3,u4ReadDspShmDWRD (D_VOL_DEC3));
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    WriteDspCommDram (ADDR_RC2D_MASTER_VOLUME_DEC4,u4ReadDspShmDWRD (D_VOL_DEC4));
    WriteDspCommDram (ADDR_RC2D_MASTER_VOLUME_BACKUP_DEC4,u4ReadDspShmDWRD (D_VOL_DEC4));
#endif

    //Input Src Gain
    WriteDspCommDram (ADDR_RC2D_INPUT_SRC_GAIN,u4ReadDspShmDWRD (D_INSRC_GAIN));
    WriteDspCommDram (ADDR_RC2D_INPUT_SRC_GAIN_DEC2,u4ReadDspShmDWRD (D_INSRC_GAIN_DEC2));
#ifdef CC_MT5391_AUD_3_DECODER
    WriteDspCommDram (ADDR_RC2D_INPUT_SRC_GAIN_DEC3,u4ReadDspShmDWRD (D_INSRC_GAIN_DEC3));
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    WriteDspCommDram (ADDR_RC2D_INPUT_SRC_GAIN_DEC4,u4ReadDspShmDWRD (D_INSRC_GAIN_DEC4));
#endif

    // Speed
    WriteDspCommDram (ADDR_RC2D_SPEED, 0);
    WriteDspCommDram (ADDR_RC2D_SPEED_DEC2, 0);

    //Channel Trim and Dialogue Gain
    for (iLoopVar = 0; iLoopVar < 8; iLoopVar++)
    {
        WriteDspCommDram ((ADDR_RC2D_CH_TRIM + iLoopVar),
            u4ReadDspShmDWRD ((D_TRIM_C + (iLoopVar * 4))));
    }

    if ((UINT8)DRVCUST_OptGet(eAudioDownmixPosition) == DOWNMIX_CH_BEFORE_AVC)
    {
        UINT32 u4TrimValue;

        u4TrimValue = u4ReadDspShmDWRD (D_TRIM_CH9) << 3;
        if (u4TrimValue > 0x07fffff)
        {
            u4TrimValue = 0x07fffff;
        }

        WriteDspCommDram (ADDR_RC2D_CH_TRIM_CH9, u4TrimValue);

        u4TrimValue = u4ReadDspShmDWRD (D_TRIM_CH10) << 3;
        if (u4TrimValue > 0x07fffff)
        {
            u4TrimValue = 0x07fffff;
        }

        WriteDspCommDram (ADDR_RC2D_CH_TRIM_CH10, u4TrimValue);
    }
    else
    {
        WriteDspCommDram (ADDR_RC2D_CH_TRIM_CH9,   u4ReadDspShmDWRD (D_TRIM_CH9));
        WriteDspCommDram (ADDR_RC2D_CH_TRIM_CH10,  u4ReadDspShmDWRD (D_TRIM_CH10));
    }

    WriteDspCommDram (ADDR_RC2D_CH_TRIM_LFE,   u4ReadDspShmDWRD (D_TRIM_LFE));
    WriteDspCommDram (ADDR_RC2D_DIALOGUE_GAIN, u4ReadDspShmDWRD (D_DIALOGUE));
    WriteDspCommDram (ADDR_RC2D_DIALOGUE_GAIN_DEC2, u4ReadDspShmDWRD (D_DIALOGUE_DEC2));

    WriteDspCommDram (ADDR_RC2D_CH_TRIM_DEC2_L,u4ReadDspShmDWRD (D_TRIM_L_DEC2));
    WriteDspCommDram (ADDR_RC2D_CH_TRIM_DEC2_R,u4ReadDspShmDWRD (D_TRIM_R_DEC2));
    WriteDspCommDram (ADDR_RC2D_CH_TRIM_DEC2_LFE,u4ReadDspShmDWRD (D_TRIM_LFE_DEC2));

    //FOR DSP PAN FUNCTION INITIAL VALUES, CC_AUD_PANFADE_SUPPORT
    WriteDspCommDram (ADDR_RC2D_CH_TRIM_DEC3_L, u4ReadDspShmDWRD (D_TRIM_L_DEC3));
    WriteDspCommDram (ADDR_RC2D_CH_TRIM_DEC3_R, u4ReadDspShmDWRD (D_TRIM_R_DEC3));
    WriteDspCommDram (ADDR_RC2D_DIALOGUE_GAIN_DEC3, u4ReadDspShmDWRD (D_DIALOGUE_DEC3));
    WriteDspCommDram (ADDR_RC2D_AD_FADE_VALUE, u4ReadDspShmDWRD (D_FADEVALUE));

    //WriteDspCommDram (ADDR_RC2D_CH_TRIM_DEC4_L, u4ReadDspShmDWRD (D_TRIM_L_DEC4));
    //WriteDspCommDram (ADDR_RC2D_CH_TRIM_DEC4_R, u4ReadDspShmDWRD (D_TRIM_R_DEC4));

    //Other Information
    /* ------- Setup MixSound Coefficient ------- */

    for (iLoopVar = 0; iLoopVar < (3*5); iLoopVar++)
    {
        WriteDspCommDram (ADDR_RC2D_DRAM_MIXSOUND_UPX8_COEFF + iLoopVar,
            MIXSOUND_COEFF[iLoopVar]);
    }

    // system master information
    WriteDspCommDram (ADDR_RC2D_STC_DIFF_LOBOUND,
        u4ReadDspShmDWRD (D_STC_DIFF_LO));
    WriteDspCommDram (ADDR_RC2D_STC_DIFF_HIBOUND,
        u4ReadDspShmDWRD (D_STC_DIFF_HI));
    WriteDspCommDram (ADDR_RC2D_STC_DIFF_WSBOUND,
        u4ReadDspShmDWRD (D_STC_DIFF_WS));
    WriteDspCommDram (ADDR_RC2D_STC_LEVEL_THRESHOLD,
        u4ReadDspShmDWRD (D_STC_THRESHOLD));
    WriteDspCommDram (ADDR_RC2D_DECODING_STR_PNT,
        u4ReadDspShmDWRD (D_DECODING_STR_PNT));
    WriteDspCommDram (ADDR_RC2D_1ST_PTS_PRS_PNT,
        u4ReadDspShmDWRD (D_1ST_PTS_PRS_PNT));
    vWriteDspWORD (ADDR_RC2D_1ST_PTS_STCH,
        u2ReadDspShmWORD (W_1ST_PTS_STCH));
    vWriteDspWORD (ADDR_RC2D_1ST_PTS_STCL,
        u2ReadDspShmWORD (W_1ST_PTS_STCL));

    WriteDspCommDram (ADDR_RC2D_STC_DIFF_LOBOUND_DEC2,
        u4ReadDspShmDWRD (D_STC_DIFF_LO_DEC2));
    WriteDspCommDram (ADDR_RC2D_STC_DIFF_HIBOUND_DEC2,
        u4ReadDspShmDWRD (D_STC_DIFF_HI_DEC2));
    WriteDspCommDram (ADDR_RC2D_STC_DIFF_WSBOUND_DEC2,
        u4ReadDspShmDWRD (D_STC_DIFF_WS_DEC2));
    WriteDspCommDram (ADDR_RC2D_STC_LEVEL_THRESHOLD_DEC2,
        u4ReadDspShmDWRD (D_STC_THRESHOLD_DEC2));
    WriteDspCommDram (ADDR_RC2D_DECODING_STR_PNT_DEC2,
        u4ReadDspShmDWRD (D_DECODING_STR_PNT_DEC2));
    WriteDspCommDram (ADDR_RC2D_1ST_PTS_PRS_PNT_DEC2,
        u4ReadDspShmDWRD (D_1ST_PTS_PRS_PNT_DEC2));
    vWriteDspWORD (ADDR_RC2D_1ST_PTS_STCH_DEC2,
        u2ReadDspShmWORD (W_1ST_PTS_STCH_DEC2));
    vWriteDspWORD (ADDR_RC2D_1ST_PTS_STCL_DEC2,
        u2ReadDspShmWORD (W_1ST_PTS_STCL_DEC2));

    WriteDspCommDram (ADDR_RC2D_STC_DIFF_LOBOUND_DEC3,
        u4ReadDspShmDWRD (D_STC_DIFF_LO_DEC3));
    WriteDspCommDram (ADDR_RC2D_STC_DIFF_HIBOUND_DEC3,
        u4ReadDspShmDWRD (D_STC_DIFF_HI_DEC3));
    WriteDspCommDram (ADDR_RC2D_STC_DIFF_WSBOUND_DEC3,
        u4ReadDspShmDWRD (D_STC_DIFF_WS_DEC3));
    WriteDspCommDram (ADDR_RC2D_STC_LEVEL_THRESHOLD_DEC3,
        u4ReadDspShmDWRD (D_STC_THRESHOLD_DEC3));
    WriteDspCommDram (ADDR_RC2D_DECODING_STR_PNT_DEC3,
        u4ReadDspShmDWRD (D_DECODING_STR_PNT_DEC3));
    WriteDspCommDram (ADDR_RC2D_1ST_PTS_PRS_PNT_DEC3,
        u4ReadDspShmDWRD (D_1ST_PTS_PRS_PNT_DEC3));
    vWriteDspWORD (ADDR_RC2D_1ST_PTS_STCH_DEC3,
        u2ReadDspShmWORD (W_1ST_PTS_STCH_DEC3));
    vWriteDspWORD (ADDR_RC2D_1ST_PTS_STCL_DEC3,
        u2ReadDspShmWORD (W_1ST_PTS_STCL_DEC3));

    /* ------- Setup IEC ------- */
    IecInfoInit ();

    vWriteDspWORD (ADDR_RC2D_IEC_CHANNEL_CFG0, 0);
    vWriteDspWORD (ADDR_RC2D_IEC_CHANNEL_CFG1, 0);
    vWriteDspWORD (ADDR_RC2D_IEC_BURST_INFO, 0);

    /* ------- Setup Customer Option ------ */
    vCustomOptInit ();

    // Memory backup address
    WriteDspCommDram (ADDR_RC2D_MEMORY_BUFFER_ADDRESS,
        u4ReadDspShmDWRD (D_MEMBACKUPADDR));

    // Light added for AVC
    WriteDspCommDram (ADDR_RC2D_AVC_TARGET_LEVEL,
        (UINT16)(u2ReadDspShmWORD (W_AVC_TARGET_LEV))<<8);
    WriteDspCommDram (ADDR_RC2D_AVC_SILENCE_LEVEL,
        u2ReadDspShmWORD (W_AVC_SILENCE_LEV)<<8);
    WriteDspCommDram (ADDR_RC2D_AVC_MAX_GAIN_UP,
        u2ReadDspShmWORD (W_AVC_MAX_GAIN_UP)<<8);
    WriteDspCommDram (ADDR_RC2D_AVC_MAX_GAIN_DOWN,
        u2ReadDspShmWORD (W_AVC_MAX_GAIN_DOWN)<<8);
    WriteDspCommDram (ADDR_RC2D_AVC_FLAG,
        u2ReadDspShmWORD (W_AVC_FLAG)<<8);
    WriteDspCommDram (ADDR_RC2D_AVC_ATTACK_THRES,
        u2ReadDspShmWORD (W_AVC_ATTACK_THRES)<<8);
    WriteDspCommDram (ADDR_RC2D_AVC_ADJUST_RATE,
        u2ReadDspShmWORD (W_AVC_ADJUST_RATE));
    WriteDspCommDram (ADDR_RC2D_AVC_UI_ADJUST_RATE,
        u2ReadDspShmWORD (W_DSP_AVC_UI_ADJUST_RATE));

    // Audio Output Bank Number
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_576_NUM,
        uReadDspShmBYTE (B_BANK576NUM));
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_384_NUM,
        uReadDspShmBYTE (B_BANK384NUM));
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_320_NUM,
        uReadDspShmBYTE (B_BANK320NUM));
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_256_NUM,
        uReadDspShmBYTE (B_BANK256NUM));

    // Audio Output Bank Number for second decoder
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_576_NUM_DEC2,
        uReadDspShmBYTE (B_BANK576NUM_DEC2));
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_384_NUM_DEC2,
        uReadDspShmBYTE (B_BANK384NUM_DEC2));
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_320_NUM_DEC2,
        uReadDspShmBYTE (B_BANK320NUM_DEC2));
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_256_NUM_DEC2,
        uReadDspShmBYTE (B_BANK256NUM_DEC2));

#ifdef CC_MT5391_AUD_3_DECODER
    //Audio Output Bank Number for 3rd decoder
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_576_NUM_DEC3,
        uReadDspShmBYTE (B_BANK576NUM_DEC3));
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_384_NUM_DEC3,
        uReadDspShmBYTE (B_BANK384NUM_DEC3));
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_320_NUM_DEC3,
        uReadDspShmBYTE (B_BANK320NUM_DEC3));
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_256_NUM_DEC3,
        uReadDspShmBYTE (B_BANK256NUM_DEC3));
#endif

    //Audio Output Error Mute Bank Number
#ifdef DSP_ERROR_MUTE_ON
    // DSP will beep when under flow
    vWriteDspWORD (ADDR_RC2D_AOUT_ERROR_MUTE_BANK_NUM, 0x3ff);
    vWriteDspWORD (ADDR_RC2D_AOUT_ERROR_MUTE_BANK_NUM_DEC2, 0x3ff);
#else
    vWriteDspWORD (ADDR_RC2D_AOUT_ERROR_MUTE_BANK_NUM,
        u2ReadDspShmWORD (W_ERRORMUTEBANK));
    vWriteDspWORD (ADDR_RC2D_AOUT_ERROR_MUTE_BANK_NUM_DEC2,
        u2ReadDspShmWORD (W_ERRORMUTEBANK_DEC2));
#endif // DSP_ERROR_MUTE_ON

    //Volume LEVEL UP/DOWN ORDER
    vWriteDspWORD (ADDR_RC2D_VOLUME_LEVEL_UP_ORDER,
        uReadDspShmBYTE (B_VOLUPORDER));
    vWriteDspWORD (ADDR_RC2D_VOLUME_LEVEL_DOWN_ORDER,
        uReadDspShmBYTE (B_VOLDOWNORDER));
    //Soft Mute (Max. 7)
    vWriteDspWORD (ADDR_RC2D_VOLUME_SOFT_MUTE_ORDER,
        uReadDspShmBYTE (B_SOFTMUTEORDER));

    //Volume LEVEL UP/DOWN ORDER for second decoder
    vWriteDspWORD (ADDR_RC2D_VOLUME_LEVEL_UP_ORDER_DEC2,
        uReadDspShmBYTE (B_VOLUPORDER_DEC2));
    vWriteDspWORD (ADDR_RC2D_VOLUME_LEVEL_DOWN_ORDER_DEC2,
        uReadDspShmBYTE (B_VOLDOWNORDER_DEC2));
    //Soft Mute (Max. 7)
    vWriteDspWORD (ADDR_RC2D_VOLUME_SOFT_MUTE_ORDER_DEC2,
        uReadDspShmBYTE (B_SOFTMUTEORDER_DEC2));

    //Volume Smooth (Max. 15)
    vWriteDspWORD (ADDR_RC2D_VOLUME_SMOOTH_ORDER, (UINT16) 8); // Light modified for AVC, order should be <= 8
    //Volume Smooth (Max. 15)
    vWriteDspWORD (ADDR_RC2D_VOLUME_SMOOTH_ORDER_DEC2, (UINT16) 8);
 #ifdef CC_MT5391_AUD_3_DECODER
    vWriteDspWORD (ADDR_RC2D_VOLUME_SMOOTH_ORDER_DEC3, (UINT16) 8);
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    vWriteDspWORD (ADDR_RC2D_VOLUME_SMOOTH_ORDER_DEC4, (UINT16) 8);
#endif

    //DAC Bit Resolution
    vWriteDspWORD (ADDR_RC2D_DAC_BIT_RESOLUTION,
        (UINT16) (uReadDspShmBYTE (B_DACBIT) & 0x1f));
    vWriteDspWORD (ADDR_RC2D_DAC_BIT_RESOLUTION_DEC2,
        (UINT16) (uReadDspShmBYTE (B_DACBIT_DEC2) & 0x1f));
    //Audio Mute Threshold
    WriteDspCommDram (ADDR_RC2D_AUDIO_MUTE_THRESHOLD, 0x0);
    WriteDspCommDram (ADDR_RC2D_AUDIO_MUTE_THRESHOLD_DEC2, 0x0);

    //Set Mute Mode (Not Mute)

    //Prologic II
    vWriteDspWORD (ADDR_RC2D_PRO_LOGIC_II_CONFIG,
        u2ReadDspShmWORD (W_PLIICONFIG));
    vWriteDspWORD (ADDR_RC2D_PRO_LOGIC_II_MODE, u2ReadDspShmWORD (W_PLIIMODE));
    //Spectrum Speed
    vWriteDspWORD (ADDR_RC2D_SPECTRUM_HIGH_ORDER,
        uReadDspShmBYTE (B_SPECTRUMUP));
    vWriteDspWORD (ADDR_RC2D_SPECTRUM_LOW_ORDER,
        uReadDspShmBYTE (B_SPECTRUMDOWN));
    //Trap Setting
    vWriteDspWORD (ADDR_RC2D_TRAP_BPEN, (UINT16) TRAP_BPEN);
    vWriteDspWORD (ADDR_RC2D_TRAP_BP0, (UINT16) TRAP_BP0);
    vWriteDspWORD (ADDR_RC2D_TRAP_BPT0, (UINT16) TRAP_BPT0);
    vWriteDspWORD (ADDR_RC2D_TRAP_BP1, (UINT16) TRAP_BP1);
    vWriteDspWORD (ADDR_RC2D_TRAP_BPT1, (UINT16) TRAP_BPT1);
    vWriteDspWORD (ADDR_RC2D_TRAP_BP2, (UINT16) TRAP_BP2);
    vWriteDspWORD (ADDR_RC2D_TRAP_BPT2, (UINT16) TRAP_BPT2);
    vWriteDspWORD (ADDR_RC2D_TRAP_BP3, (UINT16) TRAP_BP3);
    vWriteDspWORD (ADDR_RC2D_TRAP_BPT3, (UINT16) TRAP_BPT3);
    vWriteDspWORD (ADDR_RC2D_TRAP_BP4, (UINT16) TRAP_BP4);
    vWriteDspWORD (ADDR_RC2D_TRAP_BPT4, (UINT16) TRAP_BPT4);
    vWriteDspWORD (ADDR_RC2D_TRAP_BP5, (UINT16) TRAP_BP5);
    vWriteDspWORD (ADDR_RC2D_TRAP_BPT5, (UINT16) TRAP_BPT5);
    vWriteDspWORD (ADDR_RC2D_TRAP_BP6, (UINT16) TRAP_BP6);
    vWriteDspWORD (ADDR_RC2D_TRAP_BPT6, (UINT16) TRAP_BPT6);
    vWriteDspWORD (ADDR_RC2D_TRAP_BP7, (UINT16) TRAP_BP7);
    vWriteDspWORD (ADDR_RC2D_TRAP_BPT7, (UINT16) TRAP_BPT7);

    _rDspVars[AUD_DEC_MAIN].tSrcSpkCfg.wDspSpkCfg = 0xFFFF;
    _rDspVars[AUD_DEC_AUX].tSrcSpkCfg.wDspSpkCfg = 0xFFFF;

    // set stream status initial value
    // dsp polling for MPEG4
    vWriteDspWORD (ADDR_RC2D_POLL_DRAM_TIMER_CNT, 0);
    vWriteDspWORD (ADDR_RC2D_POLL_DRAM_READ_UINT32_CNT_1, 0);
    vWriteDspWORD (ADDR_RC2D_PTS_STC_CTRL_FLAG, 0);
}

/***************************************************************************
     Function : vDspPlayCommonInit
  Description : RISC Initialize DSP DRAM Value when Decorder On (Play)
    Parameter : None
    Return    : None
***************************************************************************/
void vDspPlayCommonInit (void)
{
    UINT16 u2DspData;
    INT32  i;
    UINT32 u4DramBase;

    UNUSED(i);
    UNUSED(u4DramBase);

    // Light add for internal buffer control
    vWriteDspWORD (ADDR_RC2D_SYSTEM_CTRL,    u2ReadDspShmWORD (W_SYSTEM_CTRL));
    vWriteDspWORD (ADDR_RC2D_SYSTEM_CTRL2,    u2ReadDspShmWORD (W_SYSTEM_CTRL2));
    vWriteDspWORD (ADDR_RD2D_INTERNAL_BUF_IN_FLAG, u2ReadDspShmWORD (W_INTERNAL_BUF_IN_FLAG));
    vWriteDspWORD (ADDR_MANUAL_OUTPUT_MATRIX, uReadDspShmBYTE (B_MANUAL_OUTPUT_MATRIX));
#ifdef CC_MT5391_AUD_3_DECODER
    vWriteDspWORD (ADDR_RD2D_INTERNAL_BUF2_IN_FLAG, u2ReadDspShmWORD (W_INTERNAL_BUF2_IN_FLAG));
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    vWriteDspWORD (ADDR_RD2D_INTERNAL_BUF3_IN_FLAG, u2ReadDspShmWORD (W_INTERNAL_BUF3_IN_FLAG));
#endif

#ifdef CC_AUD_USE_FLASH_AQ
    vDspFlashAQOperation(AQ_TYPE_LIMITER);
    vDspFlashAQOperation(AQ_TYPE_SPKHEIGHT);
#endif //CC_AUD_USE_FLASH_AQ

#ifdef CC_AUD_SUPPORT_MS10
    vWriteDspWORD (ADDR_RC2D_AD_FLAG, uReadDspShmBYTE (B_ADFLAG));
#endif
#ifdef CC_AUD_VBASS_SUPPORT
    // New virtual bass
    u4DramBase = dReadDspCommDram(ADDR_RC2D_VBASS_BASE);
    vWriteDspWORD ((u4DramBase+ADDR_VBASS_CROS_PARA4), u4ReadDspShmDWRD(D_VIR_BASS_GAIN));
    vWriteDspWORD ((u4DramBase+ADDR_VBASS_CROS_PARA7), u4ReadDspShmDWRD(D_VIR_BASS_GAIN_2));
    vWriteDspWORD ((u4DramBase+ADDR_VBASS_CROS_PARA5), 0x3);
    WriteDspCommDram ((u4DramBase+ADDR_VBASS_CROS_PARA6), 0x1000);//0x1000,0db
    vWriteDspWORD ((u4DramBase+ADDR_EXPEND_GAIN), 0x3);
    vWriteDspWORD ((u4DramBase+ADDR_dram_attck_count), 0x1000);
    WriteDspCommDram ((u4DramBase+ADDR_VBASS_CROS_PARA8), 0x1000);
    WriteDspCommDram ((u4DramBase+ADDR_VBASS_CROS_PARA9), 0x1000);
#endif

#ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
    u4DramBase = dReadDspCommDram(ADDR_RC2D_CDNOTCH_BASE);
    WriteDspCommDram ((u4DramBase+ADDR_CDNOTCH_USER_FC), u2ReadDspShmWORD(W_CDNOTCH_USER_FC));
    WriteDspCommDram ((u4DramBase+ADDR_CDNOTCH_USER_Q), uReadDspShmBYTE(B_CDNOTCH_USER_Q));
#endif

    if(!AUD_IsSupportSRS())    // SRS TSXT or SRS TSHD
    {
        // New MTK Vsurr
        WriteDspCommDram (ADDR_VSURR_CROS_PARA1, u4ReadDspShmDWRD(D_VSURR_CROS_TALK));//crosstalk,0x8400
        WriteDspCommDram (ADDR_VSURR_CROS_PARA2, u4ReadDspShmDWRD(D_VSURR_WIDTH)); //surround gain,0x8401
        WriteDspCommDram (ADDR_VSURR_CROS_PARA3, u4ReadDspShmDWRD(D_VSURR_LR_GAIN) );//L/R gain,0x8402
        WriteDspCommDram (ADDR_VSURR_LIMITER_ON, 0x000);
        WriteDspCommDram (ADDR_VSURR_LIMITER_THRESHOLD, 0x1BA000);
        WriteDspCommDram (ADDR_VSURR_LIMITER_RSTEP, 0x2000);
        WriteDspCommDram (ADDR_VSURR_GAIN_MPY, u4ReadDspShmDWRD(D_VSURR_CLARITY));//center gain,0x8406
        WriteDspCommDram (ADDR_VSURR_GAIN_SL, 0x300);
        WriteDspCommDram (ADDR_VSURR_OUT_OF_PHASE, 0x0);
        WriteDspCommDram (ADDR_VSURR_DELAY_ON, 0x100);
        WriteDspCommDram (ADDR_VSURR_DELAY_SAMPLE_NUM_L, 0x30000);
        WriteDspCommDram (ADDR_VSURR_DELAY_SAMPLE_NUM_R, 0x30000);
        WriteDspCommDram (ADDR_VSURR_CROS_PARA4, u4ReadDspShmDWRD(D_VSURR_BASS_GAIN));//bass enhancement,0x843e
        WriteDspCommDram (ADDR_VSURR_CROS_PARA5, u4ReadDspShmDWRD(D_VSURR_OUTPUT_GAIN));//output gain,0x843f
        // WriteDspCommDram (ADDR_VSURR_CROS_PARA6, 0x60000);
    }

#ifdef CC_AUD_SILENCE_SUPPORT
    AUD_DEC_STREAM_FROM_T eAudDecStreamFrom;
    AUD_DRVGetStreamFrom(AUD_DEC_MAIN, &eAudDecStreamFrom);
    if  (eAudDecStreamFrom == AUD_STREAM_FROM_LINE_IN)
    {
        WriteDspCommDram (ADDR_RC2D_SILENCE_GAIN_MODE, (UINT32)DRVCUST_OptGet(eAudioSilenceMode));
    }
	else
	{
        WriteDspCommDram (ADDR_RC2D_SILENCE_GAIN_MODE, 0x0);
    }
	WriteDspCommDram (ADDR_RC2D_SILENCE_FIX_THRESHOLD, (UINT32)DRVCUST_OptGet(eAudioSilenceFixThreshold));
	WriteDspCommDram (ADDR_RC2D_SILENCE_WAIT_NUM, (((UINT32)DRVCUST_OptGet(eAudioSilenceAttackWait))*48/256) << 8);
	WriteDspCommDram (ADDR_RC2D_SILENCE_RELEASE_STEP,(0x7fffff/(((UINT32)DRVCUST_OptGet(eAudioSilenceReleaseStep))*48/256)));
	WriteDspCommDram (ADDR_RC2D_SILENCE_ATTACK_STEP,(0x7fffff/(((UINT32)DRVCUST_OptGet(eAudioSilenceAttackStep))*48/256)));
	WriteDspCommDram (ADDR_RC2D_SILENCE_ATTACK_GAIN,(UINT32)DRVCUST_OptGet(eAudioSilenceAttackFixGain));
#else
    WriteDspCommDram (ADDR_RC2D_SILENCE_GAIN_MODE, 0x0);
#endif

#ifdef CC_AUD_BBE_SUPPORT
    // BBE VIVA 4
    u4DramBase = dReadDspCommDram(ADDR_RC2D_BBE_BASE);
    WriteDspCommDram ((u4DramBase+ADDR_BBEV4_LEVEL_COEF), BBEV4_LEVEL_DB[uReadDspShmBYTE (B_BBE_LEVEL)]);
    WriteDspCommDram ((u4DramBase+ADDR_BBEV4_3D_GAIN), BBEV4_3D_GAIN[uReadDspShmBYTE (B_BBE_3D_GAIN)]);
    WriteDspCommDram ((u4DramBase+ADDR_BBEV4_LOW_COEF), BBEV4_DB_8[uReadDspShmBYTE (B_BBE_LO_CONTOUR)]); // 7dB
    WriteDspCommDram ((u4DramBase+ADDR_BBEV4_HIGH_COEF), BBEV4_DB_8[uReadDspShmBYTE (B_BBE_PROCESS)]);  // 7dB

    vWriteDspWORD ((u4DramBase+ADDR_BBEV4_BBE_ON), 0x1);
    vWriteDspWORD ((u4DramBase+ADDR_BBEV4_LOW_ON), 0x1);
    vWriteDspWORD ((u4DramBase+ADDR_BBEV4_MID_ON), 0x1);
    vWriteDspWORD ((u4DramBase+ADDR_BBEV4_HIGH_ON), 0x1);
#endif // CC_AUD_BBE_SUPPORT

    // Light: AVC need different initial mode for channel changing and UI Off/On setting (aout reinitial).
    WriteDspCommDram (ADDR_RC2D_AVC_FLAG, u2ReadDspShmWORD (W_AVC_FLAG)<<8);
    vWriteDspWORD(ADDR_RC2D_AVC_DSP_FLAG, 0x0);

    //System Setup
    vWriteDspWORD (ADDR_RC2D_SYSTEM_SETUP, u2ReadDspShmWORD (W_SYSTEM_SETUP));

    vWriteDspWORD (ADDR_RC2D_CTRL_2_NUM_1, (UINT16) CTRL_2_NUM_1);

    //Input Src Gain
    WriteDspCommDram (ADDR_RC2D_INPUT_SRC_GAIN, u4ReadDspShmDWRD (D_INSRC_GAIN));

    WriteDspCommDram (ADDR_RC2D_STC_DIFF, 0x0);

    // system master information
    WriteDspCommDram (ADDR_RC2D_STC_DIFF_LOBOUND,
        u4ReadDspShmDWRD (D_STC_DIFF_LO));
    WriteDspCommDram (ADDR_RC2D_STC_DIFF_HIBOUND,
        u4ReadDspShmDWRD (D_STC_DIFF_HI));
    WriteDspCommDram (ADDR_RC2D_STC_DIFF_WSBOUND,
        u4ReadDspShmDWRD (D_STC_DIFF_WS));
    WriteDspCommDram (ADDR_RC2D_STC_LEVEL_THRESHOLD,
        u4ReadDspShmDWRD (D_STC_THRESHOLD));
    WriteDspCommDram (ADDR_RC2D_DECODING_STR_PNT,
        u4ReadDspShmDWRD (D_DECODING_STR_PNT));
    WriteDspCommDram (ADDR_RC2D_1ST_PTS_PRS_PNT,
        u4ReadDspShmDWRD (D_1ST_PTS_PRS_PNT));
    vWriteDspWORD (ADDR_RC2D_1ST_PTS_STCH,
        u2ReadDspShmWORD (W_1ST_PTS_STCH));
    vWriteDspWORD (ADDR_RC2D_1ST_PTS_STCL,
        u2ReadDspShmWORD (W_1ST_PTS_STCL));

    //Reset Speed to 0
    vWriteDspWORD (ADDR_RC2D_SPEED, u2ReadDspShmWORD (W_SPEED));

    vWriteDspWORD (ADDR_RC2D_KARAOKE_FLAG, uReadDspShmBYTE (B_KARAFLAG));

    WriteDspCommDram (ADDR_RC2D_LRCH_MIX_RATIO,
        u4ReadDspShmDWRD (D_LRMIXRATIO));

    // Reset aout bank number to default value for some decoding will change it
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_576_NUM,
        uReadDspShmBYTE (B_BANK576NUM));
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_384_NUM,
        uReadDspShmBYTE (B_BANK384NUM));
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_320_NUM,
        uReadDspShmBYTE (B_BANK320NUM));
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_256_NUM,
        uReadDspShmBYTE (B_BANK256NUM));

    //Processing Mode
    u2DspData = u2ReadDspShmWORD (W_PROCMOD);
    if (_rDspVars[AUD_DEC_MAIN].bDspStrTyp != AC3_STREAM)
    {
        u2DspData &= 0xffdf;
    }

    vWriteDspWORD (ADDR_RC2D_PROCESSING_MODE, u2DspData);

    // Strange <- sammy
    //Bit Resolution (24 bit)
    vWriteDspWORD (ADDR_RC2D_BIT_RESOLUTION, uReadDspShmBYTE (B_BIT));

    //Decoding Relation Information
    vDecCommonInit ();

    //IEC delay
    vWriteDspWORD (ADDR_RC2D_IEC_DELAY, uReadDspShmBYTE (B_IEC_DELAY));

    //Equalizer Switch
    vWriteDspWORD (ADDR_RC2D_EQUALIZER_FLAG, uReadDspShmBYTE (B_EQFLAG));

    vEQ2SbassBandGainFlush(AUD_DEC_MAIN);

    //Speaker Config
    vSetSpeakerConfig(AUD_DEC_MAIN);

    //Prologic II
    vWriteDspWORD (ADDR_RC2D_PRO_LOGIC_II_MODE, u2ReadDspShmWORD (W_PLIIMODE));
    vWriteDspWORD (ADDR_RC2D_PRO_LOGIC_II_CONFIG,
        u2ReadDspShmWORD (W_PLIICONFIG));

    // Virtual surround flag
#ifndef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
    vWriteDspWORD (ADDR_RC2D_VIRTUAL_SURROUND, uReadDspShmBYTE (B_VSURRFLAG));
#else
    vWriteDspWORD (ADDR_RC2D_VIRTUAL_SURROUND, u2ReadDspShmWORD(W_VSURRFLAG));
#endif

#ifdef CC_AUD_SPEAKER_HEIGHT
	u4DramBase = dReadDspCommDram(ADDR_RC2D_SPKHEIGHT_BASE);
    WriteDspCommDram ((u4DramBase + ADDR_SPKHIGH_PARA4), 0x0f0000);
    WriteDspCommDram ((u4DramBase + ADDR_SPKHIGH_PARA5), 0x300);
#endif

    //Reverb Flag
    vWriteDspWORD (ADDR_RC2D_REVERB_FLAG, uReadDspShmBYTE (B_REVERBFLAG));

    // SPDIF Line/in
    vWriteDspWORD(ADDR_RC2D_SPDIF_FLAG, AUD_IsSpdifInSrc(AUD_DEC_MAIN));

	// DDCO flag
    // vWriteDspWORD (ADDR_RC2D_DDCO_FLAG, 0x0);	//only initial at power on
    vWriteDspWORD (ADDR_RC2D_DDCO_AGC_FLAG,  uReadDspShmBYTE (B_DDCO_AGC_FLAG));
    vWriteDspWORD (ADDR_RC2D_DDCO_LFE_LPF_FLAG,  uReadDspShmBYTE (B_DDCO_LFE_LPF_FLAG));

    // Audio mute threshold
    WriteDspCommDram (ADDR_RC2D_AUDIO_MUTE_THRESHOLD, 0x0);

    // Limiter mode
    vWriteDspWORD (ADDR_RC2D_LIMITER_GAIN_MODE, u2ReadDspShmWORD (W_LIMITER_MODE));
    vWriteDspWORD (ADDR_RC2D_LIMITER_FLAG, u2ReadDspShmWORD (W_LIMITER_FLAG));
    WriteDspCommDram (ADDR_RC2D_LIMITER_FIX_THRESHOLD, u4ReadDspShmDWRD (D_LIMITER_FIX_THRESHOLD));
#ifdef CC_RATIO_LIMITER
    WriteDspCommDram (ADDR_RC2D_LIMITER_GAIN_RATIO, u4ReadDspShmDWRD (D_LIMITER_GAIN_RATIO));
#endif
    vWriteDspWORD (ADDR_RC2D_LIMITER_RELEASE_TIME,0x200);
    vWriteDspWORD (ADDR_RC2D_LIMITER_RELEASE_TIME_O,0x20);

    //sunman for play speed
#ifdef FRAC_SPEED_SUPPORT
    WriteDspCommDram(ADDR_RC2D_PLAY_SPEED, 	u4ReadDspShmDWRD(D_PLAY_SPEED));
#endif

#ifdef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
	vWriteDspWORD (ADDR_RC2D_NEWPOST_FLAG,
        ((wReadDspWORD(ADDR_RC2D_NEWPOST_FLAG)) | NEWPOST_FLAG_M2S_BIT));
#endif

#ifdef CC_AUD_HPF_SUPPORT
    // Setup HPF enable bit
    vWriteDspWORD (ADDR_RC2D_NEWPOST_FLAG,
        ((wReadDspWORD(ADDR_RC2D_NEWPOST_FLAG)) & ~NEWPOST_FLAG_HPF_BIT) |
        ((uReadDspShmBYTE(B_HPF_ENABLE) > 0) ? NEWPOST_FLAG_HPF_BIT : 0));
#endif

#ifdef CC_AUD_CLIPPER_SUPPORT
    // Setup clipper enable bit
    vWriteDspWORD (ADDR_RC2D_NEWPOST_FLAG,
        ((wReadDspWORD(ADDR_RC2D_NEWPOST_FLAG)) & ~NEWPOST_FLAG_CLIPPER_BIT) |
        ((uReadDspShmBYTE(B_CLIPPER_ENABLE) > 0) ? NEWPOST_FLAG_CLIPPER_BIT : 0));
#endif

#ifdef CC_AUD_CLIPPER_SUPPORT
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
#endif

#ifdef CC_AUD_SPEAKER_HEIGHT
    vWriteDspWORD (ADDR_RC2D_NEWPOST_FLAG,
        ((wReadDspWORD(ADDR_RC2D_NEWPOST_FLAG)) & ~NEWPOST_FLAG_SPH_BIT) |
    #ifndef	CC_AUD_SPEAKER_HEIGHT_MODE
        ((uReadDspShmBYTE(B_SPH_ENABLE) > 0) ? NEWPOST_FLAG_SPH_BIT : 0));
    #else
        ((uReadDspShmBYTE(B_SPH_MODE) > 0) ? NEWPOST_FLAG_SPH_BIT : 0));
    #endif
#endif

#ifdef CC_AUD_PEQ_SUPPORT
    // Setup PEQ relative
    u4DramBase = dReadDspCommDram(ADDR_RC2D_PEQ_BASE);
    vWriteDspWORD (ADDR_RC2D_NEWPOST_FLAG,
        ((wReadDspWORD(ADDR_RC2D_NEWPOST_FLAG)) & ~NEWPOST_FLAG_PEQ_BIT) |
        ((uReadDspShmBYTE(B_PEQ_ENABLE) > 0) ? NEWPOST_FLAG_PEQ_BIT : 0));

    INT16 j;
    UINT32 temp;

    for (i=0; i<9; i++)
    {
        WriteDspCommDram ((u4DramBase + ADDR_PEQ1_FC + i*3), u2ReadDspShmWORD(W_PEQ_SET0_FC + i*8));
        WriteDspCommDram ((u4DramBase + ADDR_PEQ1_GAIN + i*3), u4ReadDspShmDWRD(D_PEQ_SET0_GAIN + i*8));
        j = dwGetPEQFsIdx(i) ;
        VERIFY(j < sizeof(MTK_TAN_TABLE)/4);
        temp = MTK_TAN_TABLE[j];

        if (j > sizeof(MTK_TAN_TABLE)/4)
        {
            j = (sizeof(MTK_TAN_TABLE)/2) - j;
            temp=-(MTK_TAN_TABLE[j]);
        }

        WriteDspCommDram ((u4DramBase + ADDR_PEQ1_BW + i*3), temp);
		j = dwGetPEQCosIdx(i) ;
		temp = COS_TABLE[j];
		WriteDspCommDram ((u4DramBase + ADDR_PEQ1_FC + i*3), temp);
    }
#endif

#ifdef CC_AUD_VBASS_SUPPORT
    vWriteDspWORD (ADDR_RC2D_NEWPOST_FLAG,
        ((wReadDspWORD(ADDR_RC2D_NEWPOST_FLAG)) & ~NEWPOST_FLAG_VIR_BASS_BIT) |
        ((uReadDspShmBYTE(B_VIR_BASS_ENABLE) > 0) ? NEWPOST_FLAG_VIR_BASS_BIT : 0));
#endif

#if 0 //Daniel, 20100202, Move to set sampling rate interrupt handling
#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
{
    UINT8 u1BankIdx;
    UINT32 u4DestAddr;
    UINT32 iLoopVar;
    UINT32 *pu4Table;

    pu4Table = (UINT32 *)&AEQ_5BAND_TABLE[0][0];//[u4FreqIdx][0];

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
}
#endif
#endif

//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
    //For upload info
    vWriteDspWORD (ADDR_RC2D_UPLOAD_DATA_FLAG, u2ReadDspShmWORD(W_UPLOAD_DATA_FLAG));
    vWriteDspWORD (ADDR_RC2D_UPLOAD_BLOCK_NUM, uReadDspShmBYTE(B_UPLOAD_BLOCK_NUM));
    vWriteDspWORD (ADDR_RC2D_UPLOAD_BLOCK_SIZE, u2ReadDspShmWORD(W_UPLOAD_BLOCK_SIZE));
    vWriteDspWORD (ADDR_RC2D_UPLOAD_BLK_INT_CNT, uReadDspShmBYTE(B_UPLOAD_BLK_INT_CNT));
    WriteDspCommDram (ADDR_RC2D_UPLOAD_WRITE_PTR, ((_u4DspCmptBuf[TYPE_COMM_DRAM_IDX] + AUD_UPLOAD_BUFFER_BASE)&0xffffff));
//#endif
#ifdef CC_AUD_FIR_SUPPORT
    vWriteDspWORD (ADDR_RC2D_NEWPOST_FLAG,
                ((wReadDspWORD(ADDR_RC2D_NEWPOST_FLAG)) & ~NEWPOST_FLAG_FIR_BIT) |
                ((uReadDspShmBYTE(B_FIR_ENABLE) > 0) ? NEWPOST_FLAG_FIR_BIT : 0));
#endif

}



/***************************************************************************
     Function : vDspPlayCommonInitDec2
  Description : RISC Initialize DSP DRAM Value when Second Decorder On (Play)
                Use first information temporarily
    Parameter : None
    Return    : None
***************************************************************************/
void vDspPlayCommonInitDec2 (void)
{
    UINT16 u2DspData;

    // Light add for internal buffer control
    vWriteDspWORD (ADDR_RC2D_SYSTEM_CTRL,    u2ReadDspShmWORD (W_SYSTEM_CTRL));
    vWriteDspWORD (ADDR_RC2D_SYSTEM_CTRL2,    u2ReadDspShmWORD (W_SYSTEM_CTRL2));
    vWriteDspWORD (ADDR_RD2D_INTERNAL_BUF_IN_FLAG, u2ReadDspShmWORD (W_INTERNAL_BUF_IN_FLAG));
    vWriteDspWORD (ADDR_MANUAL_OUTPUT_MATRIX_DEC2, uReadDspShmBYTE (B_MANUAL_OUTPUT_MATRIX_DEC2));

    //System Setup
    vWriteDspWORD (ADDR_RC2D_SYSTEM_SETUP_DEC2, u2ReadDspShmWORD (W_SYSTEM_SETUP_DEC2));

    //Input Src Gain
    WriteDspCommDram (ADDR_RC2D_INPUT_SRC_GAIN_DEC2, u4ReadDspShmDWRD (D_INSRC_GAIN_DEC2));

    /* Initial ctrl_4 */
    vWriteDspWORD (ADDR_RC2D_CTRL_4_NUM_1, (UINT16) CTRL_4_NUM_1);
    // WriteDspCommDram(ADDR_RC2D_STC_DIFF_DEC2, _dwStcDiffDec2);
    WriteDspCommDram (ADDR_RC2D_STC_DIFF_DEC2, 0x0);
    // system master information
    WriteDspCommDram (ADDR_RC2D_STC_DIFF_LOBOUND_DEC2,
        u4ReadDspShmDWRD (D_STC_DIFF_LO_DEC2));
    WriteDspCommDram (ADDR_RC2D_STC_DIFF_HIBOUND_DEC2,
        u4ReadDspShmDWRD (D_STC_DIFF_HI_DEC2));
    WriteDspCommDram (ADDR_RC2D_STC_DIFF_WSBOUND_DEC2,
        u4ReadDspShmDWRD (D_STC_DIFF_WS_DEC2));
    WriteDspCommDram (ADDR_RC2D_STC_LEVEL_THRESHOLD_DEC2,
        u4ReadDspShmDWRD (D_STC_THRESHOLD_DEC2));
    WriteDspCommDram (ADDR_RC2D_DECODING_STR_PNT_DEC2,
        u4ReadDspShmDWRD (D_DECODING_STR_PNT_DEC2));
    WriteDspCommDram (ADDR_RC2D_1ST_PTS_PRS_PNT_DEC2,
        u4ReadDspShmDWRD (D_1ST_PTS_PRS_PNT_DEC2));
    vWriteDspWORD (ADDR_RC2D_1ST_PTS_STCH_DEC2,
        u2ReadDspShmWORD (W_1ST_PTS_STCH_DEC2));
    vWriteDspWORD (ADDR_RC2D_1ST_PTS_STCL_DEC2,
        u2ReadDspShmWORD (W_1ST_PTS_STCL_DEC2));

    //Reset Speed to 0
    vWriteDspWORD (ADDR_RC2D_SPEED_DEC2, u2ReadDspShmWORD (W_SPEED));
    vWriteDspWORD (ADDR_RC2D_KARAOKE_FLAG_DEC2, uReadDspShmBYTE (B_KARAFLAG));
    WriteDspCommDram (ADDR_RC2D_LRCH_MIX_RATIO_DEC2,
        u4ReadDspShmDWRD (D_LRMIXRATIO));

    // Reset aout bank number to default value for some decoding will change it
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_576_NUM_DEC2,
        uReadDspShmBYTE (B_BANK576NUM_DEC2));
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_384_NUM_DEC2,
        uReadDspShmBYTE (B_BANK384NUM_DEC2));
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_320_NUM_DEC2,
        uReadDspShmBYTE (B_BANK320NUM_DEC2));
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_256_NUM_DEC2,
        uReadDspShmBYTE (B_BANK256NUM_DEC2));

    //Processing Mode
    u2DspData = u2ReadDspShmWORD (W_PROCMOD_DEC2);
    if (_rDspVars[AUD_DEC_AUX].bDspStrTyp != AC3_STREAM)
    {
        u2DspData &= 0xffdf;
    }

    vWriteDspWORD (ADDR_RC2D_PROCESSING_MODE_DEC2, u2DspData);
    //Bit Resolution (24 bit)
    vWriteDspWORD (ADDR_RC2D_BIT_RESOLUTION_DEC2,
        uReadDspShmBYTE (B_BIT_DEC2));

#ifdef CC_AUD_SUPPORT_MS10
	// XMX mixer in PCM decoder
	// reset it
	vWriteDspWORD (ADDR_RC2D_XMX_FLAG, 0x0);
#endif

    //Decoding Relation Information
    vDecCommonInitDec2 ();

    //IEC Latency
    vWriteDspWORD (ADDR_RC2D_IEC_DELAY_DEC2, uReadDspShmBYTE (B_IEC_DELAY));

    //Speaker Config
    vSetSpeakerConfig(AUD_DEC_AUX);

    // SPDIF Line/in
    vWriteDspWORD(ADDR_RC2D_SPDIF_FLAG_DEC2, AUD_IsSpdifInSrc(AUD_DEC_AUX));

    // Audio mute threshold
    WriteDspCommDram (ADDR_RC2D_AUDIO_MUTE_THRESHOLD_DEC2, 0x0);
}

#ifdef CC_MT5391_AUD_3_DECODER
/***************************************************************************
     Function : vDspPlayCommonInitDec3
  Description : RISC Initialize DSP DRAM Value when 3rd Decorder On (Play)
                Use first information temporarily
    Parameter : None
    Return    : None
***************************************************************************/
void vDspPlayCommonInitDec3 (void)
{
    UINT16 u2DspData;

#ifdef MP3ENC_SUPPORT
	if (_rDspVars[AUD_DEC_THIRD].bDspStrTyp == MP3ENC_STREAM)
	{
	    if((_AudGetStrSource(AUD_DEC_MAIN)==AUD_STREAM_FROM_DIGITAL_TUNER) || (_AudGetStrSource(AUD_DEC_MAIN)==AUD_STREAM_FROM_ANALOG_TUNER))
	    {
	        /// internal buffer4 from dec2
	        vWriteDspWORD (ADDR_RC2D_SYSTEM_CTRL, ((u2ReadDspShmWORD(W_SYSTEM_CTRL))|SHM_INBUF4_SRC_SEL));
	        vWriteDspSram(SRAM_INTBUF4_INPUT_FLAG, 0x0200);
	    }
	    else
	    {
	        /// internal buffer4 from dec1
	        vWriteDspWORD (ADDR_RC2D_SYSTEM_CTRL, ((u2ReadDspShmWORD(W_SYSTEM_CTRL))&(~SHM_INBUF4_SRC_SEL)));
	        vWriteDspSram(SRAM_INTBUF4_INPUT_FLAG, 0x0200);
	    }
    }
#else
    // Light add for internal buffer control
    vWriteDspWORD (ADDR_RC2D_SYSTEM_CTRL,    u2ReadDspShmWORD (W_SYSTEM_CTRL));
#endif
    vWriteDspWORD (ADDR_RC2D_SYSTEM_CTRL2,    u2ReadDspShmWORD (W_SYSTEM_CTRL2));
    vWriteDspWORD (ADDR_RD2D_INTERNAL_BUF_IN_FLAG, u2ReadDspShmWORD (W_INTERNAL_BUF_IN_FLAG));
    vWriteDspWORD (ADDR_RD2D_INTERNAL_BUF2_IN_FLAG, u2ReadDspShmWORD (W_INTERNAL_BUF2_IN_FLAG));

    //System Setup
    vWriteDspWORD (ADDR_RC2D_SYSTEM_SETUP_DEC3, u2ReadDspShmWORD (W_SYSTEM_SETUP_DEC3));

    //Input Src Gain
    WriteDspCommDram (ADDR_RC2D_INPUT_SRC_GAIN_DEC3, u4ReadDspShmDWRD (D_INSRC_GAIN_DEC3));

    /* Initial ctrl_4 */ //gallen, needs to modify accroding to new MT539x DSP
    vWriteDspWORD (ADDR_RC2D_CTRL_6_NUM_1, (UINT16) CTRL_6_NUM_1);

    WriteDspCommDram (ADDR_RC2D_STC_DIFF_DEC3, 0x0);
    // system master information
    WriteDspCommDram (ADDR_RC2D_STC_DIFF_LOBOUND_DEC3,
                      u4ReadDspShmDWRD (D_STC_DIFF_LO_DEC3));
    WriteDspCommDram (ADDR_RC2D_STC_DIFF_HIBOUND_DEC3,
                      u4ReadDspShmDWRD (D_STC_DIFF_HI_DEC3));
    WriteDspCommDram (ADDR_RC2D_STC_DIFF_WSBOUND_DEC3,
                      u4ReadDspShmDWRD (D_STC_DIFF_WS_DEC3));
    WriteDspCommDram (ADDR_RC2D_STC_LEVEL_THRESHOLD_DEC3,
                      u4ReadDspShmDWRD (D_STC_THRESHOLD_DEC3));
    WriteDspCommDram (ADDR_RC2D_1ST_PTS_PRS_PNT_DEC3,
                      u4ReadDspShmDWRD (D_1ST_PTS_PRS_PNT_DEC3));
    vWriteDspWORD (ADDR_RC2D_1ST_PTS_STCH_DEC3,
                      u2ReadDspShmWORD (W_1ST_PTS_STCH_DEC3));
    vWriteDspWORD (ADDR_RC2D_1ST_PTS_STCL_DEC3,
                      u2ReadDspShmWORD (W_1ST_PTS_STCL_DEC3));

    // Reset aout bank number to default value for some decoding will change it
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_576_NUM_DEC3,
        uReadDspShmBYTE (B_BANK576NUM_DEC3));
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_384_NUM_DEC3,
        uReadDspShmBYTE (B_BANK384NUM_DEC3));
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_320_NUM_DEC3,
        uReadDspShmBYTE (B_BANK320NUM_DEC3));
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_256_NUM_DEC3,
        uReadDspShmBYTE (B_BANK256NUM_DEC3));

    // Dec3 uses D_DECODING_STR_PNT for AD function
#ifdef CC_AUD_DEC3_USE_FIFO1 // USE_FIFO3 : dec3 use fifo3
    WriteDspCommDram (ADDR_RC2D_DECODING_STR_PNT_DEC3,
        u4ReadDspShmDWRD (D_DECODING_STR_PNT));
#else
    WriteDspCommDram (ADDR_RC2D_DECODING_STR_PNT_DEC3,
                      u4ReadDspShmDWRD (D_DECODING_STR_PNT_DEC3));
#endif

    //Processing Mode
    u2DspData = u2ReadDspShmWORD (W_PROCMOD_DEC3);
    vWriteDspWORD (ADDR_RC2D_PROCESSING_MODE_DEC3, u2DspData);

    //Decoding Relation Information
    vDecCommonInitDec3 ();
}
#endif

#ifdef CC_AUD_4_DECODER_SUPPORT
/***************************************************************************
     Function : vDspPlayCommonInitDec4
  Description : RISC Initialize DSP DRAM Value when 4th Decorder On (Play)
                Use first information temporarily
    Parameter : None
    Return    : None
***************************************************************************/
void vDspPlayCommonInitDec4 (void)
{
    UINT16 u2DspData;

    //Printf("vDspPlayCommonInitDec4\n"); //SBCENC_TEST

    vWriteDspWORD (ADDR_RC2D_SYSTEM_CTRL,    u2ReadDspShmWORD (W_SYSTEM_CTRL));
    vWriteDspWORD (ADDR_RC2D_SYSTEM_CTRL2,    u2ReadDspShmWORD (W_SYSTEM_CTRL2));
    vWriteDspWORD (ADDR_RD2D_INTERNAL_BUF3_IN_FLAG, u2ReadDspShmWORD (W_INTERNAL_BUF3_IN_FLAG));

    // System Setup
    vWriteDspWORD (ADDR_RC2D_SYSTEM_SETUP_DEC4, u2ReadDspShmWORD (W_SYSTEM_SETUP_DEC4));

    //Input Src Gain
    WriteDspCommDram (ADDR_RC2D_INPUT_SRC_GAIN_DEC4, u4ReadDspShmDWRD (D_INSRC_GAIN_DEC4));

    // Initial ctrl_7
    vWriteDspWORD (ADDR_RC2D_CTRL_7_NUM_1, (UINT16)CTRL_7_NUM_1);

    //Processing Mode
    u2DspData = u2ReadDspShmWORD (W_PROCMOD_DEC4);
    vWriteDspWORD (ADDR_RC2D_PROCESSING_MODE_DEC4, u2DspData);

    // Decoding Relation Information
    vDecCommonInitDec4 ();
}
#endif

/***************************************************************************
     Function : vCustomOptInit
  Description : Customer different setting option
    Parameter : None
    Return    : None
***************************************************************************/
void vCustomOptInit (void)
{
    UINT16 u2DspData;

    // Function oriented options
    u2DspData = 0;
    vWriteDspWORD (ADDR_RC2D_CUSTOMER_OPTION0,      (u2DspData|AUDIO_UPDATE_EFFECT_ONOFF));
    vWriteDspWORD (ADDR_RC2D_CUSTOMER_OPTION0_DEC2, u2DspData);

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
static UINT8 vFineTuneVolTblIdxChk(UINT8 u1VolTabIdx)
{
    UINT8 tbl_idx;
    tbl_idx = u1VolTabIdx;
    if (tbl_idx > 40) tbl_idx = 40;
    return tbl_idx;
}

#ifdef CC_AUD_USE_FLASH_AQ
void vDspFlashAQOperation(AUD_AQ_TYPE_T eAQType)
{
    UINT32 u4Value;

    switch (eAQType)
    {
    case AQ_TYPE_LIMITER:
        if (Aud_FlashAQQuery(AQ_TYPE_LIMITER, (void*)&u4Value))
        {
            vWriteDspShmDWRD (D_LIMITER_FIX_THRESHOLD, u4Value); // default is 0x100000
        }
        break;
    case AQ_TYPE_SPKHEIGHT:
        if (Aud_FlashAQQuery(AQ_TYPE_SPKHEIGHT, (void*)&u4Value))
        {
        #ifdef CC_AUD_SPEAKER_HEIGHT
            #ifndef CC_AUD_SPEAKER_HEIGHT_MODE
            vWriteDspShmBYTE(B_SPH_ENABLE, (UINT8)u4Value);
            #else
            vWriteDspShmBYTE(B_SPH_MODE, (UINT8)u4Value);
            #endif
        #endif
        }
        break;
    default:
        break;
    };
}
#endif // CC_AUD_USE_FLASH_AQ

static const UINT32 _arMtkOplFuncTab[AUD_OPL_NUM] =
{
    #ifdef CC_AUD_BBE_SUPPORT
    AUD_OPL_BBE,
    #endif
    #ifdef CC_AUD_VBASS_SUPPORT
    AUD_OPL_VIRTUAL_BASS,
    #endif
    #ifdef CC_AUD_PEQ_SUPPORT
    AUD_OPL_PEQ,
    #endif
	#ifdef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
    AUD_OPL_M2S,
    #endif
    //AUD_OPL_CLEAR_VOICE,
    #ifdef CC_AUD_SPEAKER_HEIGHT
    AUD_OPL_SPK_HEIGHT,
    #endif
    #ifdef CC_AUD_CLIPPER_SUPPORT
    AUD_OPL_CLIPPER,
    #endif
    //AUD_OPL_NOISE_REDUCTION,
    #ifdef KARAOKE_SUPPORT
    AUD_OPTL_KEY_SHIFT,
    #endif
    //AUD_OPL_VIQ,
    //AUD_OPL_LXP,
    #ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
    AUD_OPL_AUDYSSEY_ADV,
    AUD_OPL_AUDYSSEY_ABX,
    AUD_OPL_AUDYSSEY_AEQ,
    #endif
    #ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
    AUD_OPL_CDNOTCH,
    #endif
    #ifdef DSP_SUPPORT_SRS_SSHD
    AUD_OPL_SRS_PEQ,
    AUD_OPL_SRS_LMT, //sunman_lmt
    #endif
    #ifdef DSP_SUPPORT_SRSVIQ
    AUD_OPL_SRS_VIQ, //sunman_viq
    #endif
#ifdef CC_AUD_FIR_SUPPORT
	AUD_OPL_FIR,
#endif
    #ifdef DSP_SUPPORT_SRS_PURESOUND
    AUD_OPL_SRS_AEQ,
    AUD_OPL_SRS_LMT,
    #endif

    AUD_OPL_NUM
};

void vOplFuncInit(void)
{
    UINT32 *u4Ptr;
    UINT32 u4NormalDramAddr=ADDR_RC2D_OPL_FUNC_NORMAL_BASE;
    UINT32 u4CmptDramAddr=DRAM_OPL_FUNC_CMPT_BASE;
    UINT32 u4VctOffset, u4NormalSize, u4CmptSize;
    UINT8 u1Idx=0;

    u4Ptr = (UINT32 *)_arMtkOplFuncTab;
    // skip the last element: AUD_OPL_END
    // skip the last element: AUD_OPL_END
    //while (*(u4Ptr+u1Idx) != AUD_OPL_NUM && u1Idx < AUD_OPL_NUM)
    while ((u1Idx < AUD_OPL_NUM) && (*(u4Ptr+u1Idx) != AUD_OPL_NUM)) //Klocwork require, check boundary first, then acces array.
    {
        u4VctOffset = ADDR_RC2D_OPL_FUNC_VCT_BASE +
            _au4OptlFuncTableSize[*(u4Ptr+u1Idx)][0];
        u4NormalSize = _au4OptlFuncTableSize[*(u4Ptr+u1Idx)][1];
        u4CmptSize = _au4OptlFuncTableSize[*(u4Ptr+u1Idx)][2];
        if (u4NormalSize > 0)
        {
            WriteDspCommDram(u4VctOffset, u4NormalDramAddr);
            u4VctOffset++;
            u4NormalDramAddr += u4NormalSize;
        }

        if (u4CmptSize > 0)
        {
            WriteDspCommDram(u4VctOffset, u4CmptDramAddr);
            u4CmptDramAddr += u4CmptSize;
        }

        u1Idx++;

    }

}

//DSP_Vestel_8M
#define ROM_ADDR      0x0      // ROM
#define POST_ADDR     0x1000   // Post
#define CRI_ADDR      0x2800   // Critical
#define COMM_ADDR     0x3000   // Common
#define DEC3_ADDR     0x6800   // Job3
#define DEC0_ADDR     0x7000   // Job0
#define DEC2_ADDR     0xA000   // Job2 (use with job 0)
#define DEC2_VEC_ADDR 0xBF80   // Job2 vector
#define DEC1_ADDR     0xC000   // Job1
#define POST_VCT_ADDR 0x1000   // Post Vect
#define POST1_ADDR    0x1010   // Post 1
#define POST2_ADDR    0x1510   // Post 2
#define POST3_ADDR    0x1b10   // Post 3
#define POST4_ADDR    0x1e80   // Post 4

UINT32 _u4DspPraStart[DSPF_AWB_MODE_TBL_SA/2+1]=
{
//DSPF_IMAGE_SA                   0
    0xff,
//DSPF_COMM_CODE_SA               2
    COMM_ADDR,
//DSPF_ROM_CODE_SA                4
    ROM_ADDR,
//DSPF_DROM_SA                    6
    0xff,
//DSPF_PROM_SA                    8
    0xff,
//DSPF_AIN_INTRAM_SA              10
    0xff,
//DSPF_AIN_INTRAM_MINER_SA        12
    0xff,
//DSPF_CRITICAL_CODE_SA           14
    CRI_ADDR,
//DSPF_NTSC_DEC1_SA               16
    DEC0_ADDR,
//DSPF_NTSC_DEC2_SA               18
    DEC1_ADDR,
//DSPF_NTSCTBL_SA                 20
    0xff,
//DSPF_DETECTOR_DEC1_SA           22
    DEC0_ADDR,    
//DSPF_DETECTOR_DEC2_SA           24
    DEC1_ADDR,
//DSPF_DETECTORTBL_SA             26
    0xff,
//DSPF_A2_DEC1_SA                 28
    DEC0_ADDR,
//DSPF_A2_DEC2_SA                 30
    DEC1_ADDR,
//DSPF_A2TBL_SA                   32
    0xff,
//DSPF_PAL_DEC1_SA                34
    DEC0_ADDR,
//DSPF_PAL_DEC2_SA                36
    DEC1_ADDR,
//DSPF_PALTBL_SA                  38
    0xff,
//DSPF_FMFM_DEC1_SA               40
    DEC0_ADDR,
//DSPF_FMFM_DEC2_SA               42
    DEC1_ADDR,
//DSPF_FMFMTBL_SA                 44
    0xff,
//DSPF_MINER_DEC1_SA              46
    DEC0_ADDR,
//DSPF_MINER_DEC2_SA              48
    DEC1_ADDR,
//DSPF_MINERTBL_SA                50
    0xff,
//DSPF_FMRDODEC1_SA               52
    DEC0_ADDR,
//DSPF_FMRDODETDEC1_SA            54
    DEC0_ADDR,
//DSPF_FMRDOTBL_SA                56
    0xff,
//DSPF_ATV_COSTBL_SA              58
    0xff,
//DSPF_PCM_DEC1_SA                60
    DEC0_ADDR,
//DSPF_PCM_DEC2_SA                62
    DEC1_ADDR,
//DSPF_PCM_DEC3_SA                64
    ROM_ADDR,
//DSPF_MP2_DEC1_SA                66
    DEC0_ADDR,
//DSPF_MP2_DEC2_SA                68
    DEC1_ADDR,
//DSPF_MP2_DEC3_SA                70
    ROM_ADDR,
//DSPF_MP2_TABLE_SA               72
    0xff,
//DSPF_MP3_DEC1_SA                74
    DEC0_ADDR,
//DSPF_MP3_TABLE_SA               76
    0xff,
//DSPF_AC3_DEC1_SA                78
    DEC0_ADDR,
//DSPF_AC3_DEC2_SA                80
    DEC1_ADDR,
//DSPF_AC3_DEC3_SA                82
    ROM_ADDR,
//DSPF_AC3_TABLE_SA               84
    0xff,
//DSPF_AAC_DEC1_SA                86
    DEC0_ADDR,
//DSPF_AAC_DEC2_SA                88
    DEC1_ADDR,
//DSPF_AAC_DEC3_SA                90
    ROM_ADDR,
//DSPF_AAC_CMPT_TABLE_SA          92
    0xff,
//DSPF_AAC_TABLE_SA               94
    0xff,
//DSPF_DDCO_SA                    96
    0x7800, //??????
//DSPF_DDCO_TBL_SA                98
    0xff,
//DSPF_WMA_DEC1_SA                100
    DEC0_ADDR,
//DSPF_WMA_C_TABLE_SA             102
    0xff,
//DSPF_WMA_N_TABLE_SA             104
    0xff,
//DSPF_LPCM_DEC1_SA               106
    DEC0_ADDR,
//DSPF_LPCM_TBL_SA                108
    0xff,
//DSPF_PNOISE_SA                  110
    DEC0_ADDR,
//DSPF_SBCDEC1_SA                 112
    DEC0_ADDR,
//DSPF_SBC_TABLE_SA               114
    0xff,
//DSPF_MP3ENC_DEC2_SA	          116
    DEC1_ADDR,
//DSPF_MP3ENC_TABLE_SA	          118
    0xff,
//DSPF_COOK_DEC1_SA               120
    DEC0_ADDR,
//DSPF_COOKTBL_SA                 122
    0xff,
//DSPF_POST_VECT_SA               124
    POST_VCT_ADDR,
//DSPF_PL2_POST_SA                126
    0x1010,
//DSPF_PL2_TABLE_SA               128
    0xff,
//DSPF_VSURR_POST_SA              130
    POST2_ADDR,
//DSPF_BBE_VIVA4_SA               132
    POST3_ADDR,
//DSPF_VIQ_SA                     134
    0x1B80,   ///????????????????
//DSPF_VIQ_TBL_SA                 136
    0xff,
//DSPF_SBCENC4_SA                 138
    DEC3_ADDR,
//DSPF_SBCENC_TABLE_SA	          140
    0xff,
//DSPF_ADV_SA                     142
    0xff,
//DSPF_ADV_TBL_SA                 144
    0xff,
//DSPF_G711DEC1_SA                146
    DEC0_ADDR,
//DSPF_G711ENC1_SA                148
    DEC0_ADDR,
//DSPF_DROM_COMM_SA               150
    0xff,
//DSPF_DROM_MTS_SA                152
    0xff,
//DSPF_DROM_AC3_SA                154
    0xff,
//DSPF_DROM_MPEG_SA               156
    0xff,
//DSPF_DROM_AAC_SA                158
    0xff,
//DSPF_DTS_DEC1_SA                160
    DEC0_ADDR,
//DSPF_DTS_DEC2_SA                162
    DEC1_ADDR,
//DSPF_DTS_HUFF_TABLE_SA          164
    0xff,
//DSPF_DTS_ADPCM_VQ_TABLE_SA      166
    0xff,
//DSPF_DTS_ROM_TABLE_SA           168
    0xff,
//DSPF_DTS_BC_SA                  170
    0xff,  //????????
//DSPF_MP3_DEC2_SA                172
    DEC1_ADDR,
//DSPF_LPCM_DEC2_SA               174
    DEC1_ADDR,
//DSPF_DRA_DEC1_SA                176
    DEC0_ADDR,
//DSPF_DRA_TABLE_SA               178
    0xff,
//DSPF_DRA_DEC2_SA                180
    DEC1_ADDR,
//DSPF_VCT_MP2DEC3_SA             182
    DEC2_VEC_ADDR,
//DSPF_VCT_AACDEC3_SA             184
    DEC2_VEC_ADDR,
//DSPF_VCT_AC3DEC3_SA             186
    DEC2_VEC_ADDR,
//DSPF_VCT_PCMDEC3_SA             188
    DEC2_VEC_ADDR,
//DSPF_MP3ENC_DEC3_SA             190
    DEC2_ADDR,
//DSPF_VCT_MP3ENC_DEC3_SA         192
    DEC2_VEC_ADDR,
//DSPF_G729DEC2_SA                194
    DEC1_ADDR,
//DSPF_G729DEC_TABLE_SA           196
    0xff,
//DSPF_G729DEC1_SA                198
    DEC0_ADDR,
//DSPF_CDNOTCH_SA                 200
    POST4_ADDR,
//DSPF_VBASS_SA                   202
    POST4_ADDR,
//DSPF_SRS_TSHD_SA                204
    POST2_ADDR,
//DSPF_DTSENC_SA                  206
    DEC2_ADDR,
//DSPF_DDCO_TB3_SA                208
    0xff,
//DSPF_SPKHEIGHT_SA               210
    POST3_ADDR,
//DSPF_DDCO_TB4_SA                212
    0xff,
//DSPF_DDPCONV_SA                 214
    DEC3_ADDR, //????????
//DSPF_PCM_DEC4_SA                216
    DEC3_ADDR,
//DSPF_VORBIS_DEC1_SA             218 
    DEC0_ADDR,
//DSPF_VORBIS_CTBL_SA             220 
    0xff,
//DSPF_VORBIS_NTBL_SA             222 
    0xff,
//DSPF_VORBIS_DEC2_SA             224 
    DEC1_ADDR,
//DSPF_PTSINFO_SA                 226
    0xff,
//DSPF_AAC_DEC2EXT_SA             228
    (DEC3_ADDR+AAC_PRAM_DEC2_EXT_ADR*2),
//DSPF_WMAPRO_DEC1_SA             230
    DEC0_ADDR,
//DSPF_WMAPRO_C_TABLE_SA          232
    0xff,
//DSPF_WMAPRO_N_TABLE_SA          234
    0xff,
//DSPF_DDCO_CRC_TABLE_SA          236
    0xff,
//DSPF_SRS_WOWHD_SA               238
    POST2_ADDR,
//DSPF_KEYSHIFT_SA                240
    POST4_ADDR,
//DSPF_AMRDEC1_SA                 242
    DEC0_ADDR,
//DSPF_AMR_TBL_SA                 244
    0xff,
//DSPF_AMR_MODE_TBL_SA            246
    0xff,
//DSPF_AWBDEC1_SA                 248
    DEC0_ADDR,
//DSPF_AWB_TBL_SA                 250
    0xff,
//DSPF_AWB_MODE_TBL_SA            252
    0xff,
};

extern void CheckSumIt(UINT16 u2StartPc, UINT32 u4CodeSA, UINT32 u4CodeSZ);

void CheckSumAll(void)
{
    UINT32 u2Idx;
    UINT16 u2Size;

    u2Size = sizeof(_u4DspPraStart)/4;

    for (u2Idx=0; u2Idx<u2Size; u2Idx++)
    {
        // !!!!!!!!! Try this for use API in pre-built binary !!!!!!
        CheckSumIt(_u4DspPraStart[u2Idx], (u2Idx*2), (u2Idx*2+1));
    }
}


