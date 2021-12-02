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
 * $RCSfile: dsp_init.c,v $
 * $Revision: #1 $
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
#include "../../nptv/inc/sys/util.h"
#include "x_os.h"
#include "dsp_general.h"
#include "dsp_intf.h"
#include "dsp_shm.h"
#include "dsp_var_def.h"
#include "dsp_rg_ctl.h"
#include "dsp_func.h"
#include "d2rc_shm.h"
#include "dsp_table.h"

#include "aud_if.h"
#include "aud_drv.h"
#include "drvcust_if.h"
#include "aud_debug.h"

// Temporaray used -> should be removed
#include "dsp_reg.h"

#include "x_hal_926.h"
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
#define CUTOFF_FREQ             DRVCUST_OptGet(eAudioBassMngCutOffFreq)
#define SOFT_MUTE_ORDER         DRVCUST_OptGet(eAudioSoftMuteOrder)
#define IS_LARGE_SPEAKER()      DRVCUST_OptGet(eAudioSpkType)
#define IS_BYPASS_PROC()        DRVCUST_OptGet(eAudioBypassPostProc)
//#define AVC_TARGET_LEV          DRVCUST_OptGet(eAudioAvcTarget)
//#define AVC_GAIN_UP             DRVCUST_OptGet(eAudioAvcGainUp)
#define AVC_ATTACK_THRES        DRVCUST_OptGet(eAudioAvcAttackThres)
#define AVC_ADJUST_RATE         DRVCUST_OptGet(eAudioAvcAdjustRate)
#define IS_SPKCFG_SSLN()        (((u4ReadDspShmDWRD(D_SPKCFG) & 0x1f020) == 0x18000)? 1:0 ) //andrew 07/1/8
//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
extern UINT32 dwGetPEQFsIdx(UINT8 u1Set);

//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

static void vCustomOptInit (void);
static UINT8 vFineTuneVolTblIdxChk(UINT8 u1VolTabIdx);

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

void vDspResetDec1 (void);
void vDspResetDec2 (void);
//void vIECCommonInit (BOOL fgFirstDecoder);
void vDecCommonInit (void);
void vDecCommonInitDec2 (void);
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
void vDspResetDec3 (void);
void vDecCommonInitDec3 (void);
#endif

#ifdef CC_AUD_USE_FLASH_AQ
extern void vDspFlashAQOperation(AUD_AQ_TYPE_T eAQType);
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

    if (DRVCUST_OptGet(eAudioSpkType))
    {
        u4SpkCfg |= ((UINT32)0x3 << 13);    
    }
    if (DRVCUST_OptGet(eAudioSurroundSpkType))
    {
        u4SpkCfg |= ((UINT32)0x3 << 15);    
    }
    if (DRVCUST_OptGet(eAudioCenterSpkType))
    {
        u4SpkCfg |= ((UINT32)0x1 << 12);    
    }
    if (DRVCUST_OptGet(eAudioCh910SpkType))
    {
        u4SpkCfg |= ((UINT32)0x3 << 19);    
    }	
    if (DRVCUST_OptGet(eAudioSubwooferOn))
    {
        u4SpkCfg |= ((UINT32)0x1 << 5); 
    }

    return u4SpkCfg;
}

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
    vWriteDspWORD (ADDR_RC2D_CTRL_3_NUM_1, (UINT16) 12);
//Pink Noise Setting
    vWriteDspWORD (ADDR_RC2D_CHANNEL_USE, u2ReadDspShmWORD (W_PINKNOISE));
//Soft Mute (Max. 7)
    vWriteDspWORD (ADDR_RC2D_VOLUME_SOFT_MUTE_ORDER,
                   uReadDspShmBYTE (B_SOFTMUTEORDER));

    HalFlushInvalidateDCache(); //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro
    switch (_bDspStrTyp)
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
	 	vWriteDspWORD(ADDR_RC2D_BYPASS_ASRC_DEC2,0);

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
        WriteDspCommDram (ADDR_RC2D_AC3_DOWNMIX,
                          uReadDspShmBYTE (B_AC3AUTODNMIX));

#ifdef CC_AUD_DVBT_AC3_BYPASS
        //AC3 Bypass to SPDIF mode, 0x555 mean bypass mode on (no speaker output, just raw data to SPDIF)
        WriteDspCommDram (ADDR_RC2D_AC3_BYPASS_FLAG,        
                        0x555);
#else
        WriteDspCommDram (ADDR_RC2D_AC3_BYPASS_FLAG,        
                        0x0);
#endif //CC_AUD_DVBT_AC3_BYPASS
        
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
	vWriteDspWORD(ADDR_RC2D_BYPASS_ASRC_DEC2,0); // asrc enable
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
	 vWriteDspWORD(ADDR_RC2D_BYPASS_ASRC_DEC2,0); // asrc enable
        break;
#ifdef DATA_DISC_WMA_SUPPORT        
    case WMA_STREAM:
    	//vWriteDspWORD (ADDR_RC2D_CTRL_3_NUM_1, (UINT16) 0xb);
        //koro: reduce one shareinfo maxnum shareinfo = 0xa for 539x 
        vWriteDspWORD ( ADDR_RC2D_CTRL_3_NUM_1, (UINT16) 0xb);
        //WNA Packet No
        /*
        vWriteDspWORD ( ADDR_RC2D_WMA_PACKET_NO,
                       u2ReadDspShmWORD ( W_WMA_PACKET_NO));
		*/
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
	 vWriteDspWORD(ADDR_RC2D_BYPASS_ASRC_DEC2,0); // asrc enable
                       
        break;
#endif        
    case VORBIS_STREAM:
        //Vorbis Packet No
        vWriteDspWORD (ADDR_RC2D_VORBIS_PACKET_NO,
                       u2ReadDspShmWORD (W_VORBISPACKET));
        // clear Vorbis table ok flag
        vWriteDspWORD (ADDR_D2RC_VORBIS_TABLE_OK, 0);
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
/* Move to aud_cfg.c _AudDemodSetting                
      #ifdef CC_MT5391_AUD_SUPPORT 
	  //reset FEIF for SIF sigal
        IO_WRITE32(IO_VIRT+0x2B000, 0x4b4, 0x000f0000);
        IO_WRITE32(IO_VIRT+0x2B000, 0x4a4, 0x09c87800);	
        IO_WRITE32(IO_VIRT+0x2B000, 0x110, 0x00000001);
        IO_WRITE32(IO_VIRT+0x2B000, 0x110, 0x00000003);	
        ///////////////////////////////////////
       #endif
*/       
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
        vWriteDspWORD (ADDR_RC2D_NR_ENABLE,u2ReadDspShmWORD (W_NR_ENABLE));
        vWriteDspWORD (ADDR_RC2D_NR_MTS_TH1_HIGH,u2ReadDspShmWORD (W_NR_MTS_TH1_HIGH));
        vWriteDspWORD (ADDR_RC2D_NR_MTS_TH1_LOW,u2ReadDspShmWORD (W_NR_MTS_TH1_LOW));
        vWriteDspWORD (ADDR_RC2D_NR_MTS_TH2_HIGH,u2ReadDspShmWORD (W_NR_MTS_TH2_HIGH));
        vWriteDspWORD (ADDR_RC2D_NR_MTS_TH2_LOW,u2ReadDspShmWORD (W_NR_MTS_TH2_LOW));
        vWriteDspWORD (ADDR_RC2D_NR_SAP_TH1_HIGH,u2ReadDspShmWORD (W_NR_SAP_TH1_HIGH));
        vWriteDspWORD (ADDR_RC2D_NR_SAP_TH1_LOW,u2ReadDspShmWORD (W_NR_SAP_TH1_LOW));
        vWriteDspWORD (ADDR_RC2D_NR_SAP_TH2_HIGH,u2ReadDspShmWORD (W_NR_SAP_TH2_HIGH));
        vWriteDspWORD (ADDR_RC2D_NR_SAP_TH2_LOW,u2ReadDspShmWORD (W_NR_SAP_TH2_LOW));
        break;
    case FMFM_STREAM:
/* Move to aud_cfg.c _AudDemodSetting               
      #ifdef CC_MT5391_AUD_SUPPORT 
	  //reset FEIF for SIF sigal
        IO_WRITE32(IO_VIRT+0x2B000, 0x4b4, 0x000f0000);
        IO_WRITE32(IO_VIRT+0x2B000, 0x4a4, 0x09c87800);	
        IO_WRITE32(IO_VIRT+0x2B000, 0x110, 0x00000001);
        IO_WRITE32(IO_VIRT+0x2B000, 0x110, 0x00000003);	
        ///////////////////////////////////////
       #endif	
*/       
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
/* Move to aud_cfg.c _AudDemodSetting                 
      #ifdef CC_MT5391_AUD_SUPPORT 
	  //reset FEIF for SIF sigal
        IO_WRITE32(IO_VIRT+0x2B000, 0x4b4, 0x000f0000);
        IO_WRITE32(IO_VIRT+0x2B000, 0x4a4, 0x09c87800);	
        IO_WRITE32(IO_VIRT+0x2B000, 0x110, 0x00000001);
        IO_WRITE32(IO_VIRT+0x2B000, 0x110, 0x00000003);	
        ///////////////////////////////////////
       #endif	
*/       
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
        // for NR in A2 decoder
        vWriteDspWORD (ADDR_RC2D_NR_ENABLE,u2ReadDspShmWORD (W_NR_ENABLE));
        vWriteDspWORD (ADDR_RC2D_NR_A2_TH1_HIGH,u2ReadDspShmWORD (W_NR_A2_TH1_HIGH));
        vWriteDspWORD (ADDR_RC2D_NR_A2_TH1_LOW,u2ReadDspShmWORD (W_NR_A2_TH1_LOW));
        vWriteDspWORD (ADDR_RC2D_NR_A2_TH2_HIGH,u2ReadDspShmWORD (W_NR_A2_TH2_HIGH));
        vWriteDspWORD (ADDR_RC2D_NR_A2_TH2_LOW,u2ReadDspShmWORD (W_NR_A2_TH2_LOW));
        break;    
    case PAL_STREAM:
/* Move to aud_cfg.c _AudDemodSetting                 
       #ifdef CC_MT5391_AUD_SUPPORT 
	  //reset FEIF for SIF sigal
        IO_WRITE32(IO_VIRT+0x2B000, 0x4b4, 0x000f0000);
        IO_WRITE32(IO_VIRT+0x2B000, 0x4a4, 0x09c87800);	
        IO_WRITE32(IO_VIRT+0x2B000, 0x110, 0x00000001);
        IO_WRITE32(IO_VIRT+0x2B000, 0x110, 0x00000003);	
        ///////////////////////////////////////
       #endif
*/       
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
         //for PAL FM BAD SIGNAL CHECK 
	  vWriteDspWORD (ADDR_PAL_HDEV_BAD_SIGNAL_CHECK,uReadDspShmBYTE (B_FM_BAD_SIGNAL));
        vWriteDspWORD (ADDR_DEM_PAL_FINE_TUNE_VOLUME,u4ReadDspShmDWRD  (D_PAL_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (ADDR_DEM_NICAM_FINE_TUNE_VOLUME,u4ReadDspShmDWRD (D_NICAM_FINE_TUNE_VOLUME)>>8);       
        vWriteDspWORD (ADDR_DEM_AM_FINE_TUNE_VOLUME,u4ReadDspShmDWRD  (D_AM_FINE_TUNE_VOLUME)>>8);
        // for NR in PAL decoder
        vWriteDspWORD (ADDR_RC2D_NR_ENABLE,u2ReadDspShmWORD (W_NR_ENABLE));
        vWriteDspWORD (ADDR_RC2D_NR_PAL_TH1_HIGH,u2ReadDspShmWORD (W_NR_PAL_TH1_HIGH));
        vWriteDspWORD (ADDR_RC2D_NR_PAL_TH1_LOW,u2ReadDspShmWORD (W_NR_PAL_TH1_LOW));
        vWriteDspWORD (ADDR_RC2D_NR_PAL_TH2_HIGH,u2ReadDspShmWORD (W_NR_PAL_TH2_HIGH));
        vWriteDspWORD (ADDR_RC2D_NR_PAL_TH2_LOW,u2ReadDspShmWORD (W_NR_PAL_TH2_LOW));
        vWriteDspWORD (ADDR_RC2D_NR_PAL_HDEV_TH1_HIGH,u2ReadDspShmWORD (W_NR_PAL_HDEV_TH1_HIGH));
        vWriteDspWORD (ADDR_RC2D_NR_PAL_HDEV_TH1_LOW,u2ReadDspShmWORD (W_NR_PAL_HDEV_TH1_LOW));
        vWriteDspWORD (ADDR_RC2D_NR_PAL_HDEV_TH2_HIGH,u2ReadDspShmWORD (W_NR_PAL_HDEV_TH2_HIGH));
        vWriteDspWORD (ADDR_RC2D_NR_PAL_HDEV_TH2_LOW,u2ReadDspShmWORD (W_NR_PAL_HDEV_TH2_LOW));
        vWriteDspWORD (ADDR_RC2D_NR_AM_TH1_HIGH,u2ReadDspShmWORD (W_NR_AM_TH1_HIGH));
        vWriteDspWORD (ADDR_RC2D_NR_AM_TH1_LOW,u2ReadDspShmWORD (W_NR_AM_TH1_LOW));
        vWriteDspWORD (ADDR_RC2D_NR_AM_TH2_HIGH,u2ReadDspShmWORD (W_NR_AM_TH2_HIGH));
        vWriteDspWORD (ADDR_RC2D_NR_AM_TH2_LOW,u2ReadDspShmWORD (W_NR_AM_TH2_LOW));
        break;
    case DETECTOR_STREAM:
/* Move to aud_cfg.c _AudDemodSetting         
      #ifdef CC_MT5391_AUD_SUPPORT 
	  //reset FEIF for SIF sigal
        IO_WRITE32(IO_VIRT+0x2B000, 0x4b4, 0x000f0000);
        IO_WRITE32(IO_VIRT+0x2B000, 0x4a4, 0x09c87800);	
        IO_WRITE32(IO_VIRT+0x2B000, 0x110, 0x00000001);
        IO_WRITE32(IO_VIRT+0x2B000, 0x110, 0x00000003);	
        ///////////////////////////////////////
       #endif
*/       
        vWriteDspWORD (ADDR_DETECTOR_DEFAULT_MODE, uReadDspShmBYTE(B_DETECT_DEFAULT)); 
        vWriteDspWORD (ADDR_DETECTOR_START_FLAG, 1); // default turn on detection
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
        vWriteDspWORD (ADDR_DEM_MTS_FINE_TUNE_VOLUME,u4ReadDspShmDWRD  (D_MTS_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (ADDR_DEM_USER_MODE,u2ReadDspShmWORD (W_USER_MODE));
        break;
#endif

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
        vWriteDspWORD(ADDR_RC2D_LPCM_DECODING_TYPE,  uReadDspShmBYTE(W_LPCM_DECODING_TYPE));

        if (uReadDspShmBYTE(W_ADPCM_DECODING_TYPE)!=0) //ADPCM
        {
            vWriteDspWORD(ADDR_RC2D_LPCM_ADPCM_DEC_TYPE, uReadDspShmBYTE(W_ADPCM_DECODING_TYPE));
            vWriteDspWORD(ADDR_RC2D_LPCM_ADPCM_BITS_PER_SAMPLE, uReadDspShmBYTE(B_ADPCM_BIT_PERSAMPLE));
            vWriteDspWORD(ADDR_RC2D_ADPCM_BLOCK_ALIGN, u2ReadDspShmWORD(W_ADPCM_BLOCK_ALIGN));
            vWriteDspWORD(ADDR_RC2D_ADPCM_CHANNEL_NUM, uReadDspShmBYTE(B_ADPCM_CHANNEL_NUM));
        }
        #endif        
        break;
        
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

//CTRL3 NUM_1
// ... should be in switch below
    vWriteDspWORD (ADDR_RC2D_CTRL_5_NUM_1, (UINT16) 12);
//Pink Noise Setting
    vWriteDspWORD (ADDR_RC2D_CHANNEL_USE_DEC2, u2ReadDspShmWORD (W_PINKNOISE));

    vWriteDspWORD(ADDR_RC2D_BYPASS_ASRC_DEC2,0);
//Soft Mute (Max. 7)
    vWriteDspWORD (ADDR_RC2D_VOLUME_SOFT_MUTE_ORDER_DEC2,
                   u2ReadDspShmWORD (B_SOFTMUTEORDER_DEC2));
    HalFlushInvalidateDCache(); //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro

    switch (_bDspStrTypDec2)
    {
    #ifdef ADSP_JPEG_DEC2
    case JPEG_STREAM:
        /// Assign by Driver 
        vWriteDspWORD (ADDR_RC2D_CTRL_5_NUM_1,              0x00);
        vWriteDspWORD (ADDR_RC2D_JPG_CMD_FLAG_DEC2,         uReadDspShmBYTE(W_JPG_CMD_FLAG)&0xBE);  //reset bit 0,bit 6 while initialize
        vWriteDspWORD (ADDR_RC2D_JPG_MAX_SCAN_NUM_DEC2,     uReadDspShmBYTE(B_JPG_MAX_SCAN_NUM));
        vWriteDspWORD (ADDR_RC2D_JPG_MAX_COMPACT_ADDR_DEC2, u4ReadDspShmDWRD(D_JPG_MAX_COMPACT_ADDR));
        vWriteDspWORD (ADDR_RC2D_JPG_LENGTH,                u4ReadDspShmDWRD(D_JPG_LENGTH));  //Set file length from Mp3.c
        vWriteDspWORD (ADDR_RC2D_JPG_SUPPORT_MAX_WIDTH,     u2ReadDspShmWORD(W_JPG_MAX_WIDTH));
        vWriteDspWORD (ADDR_RC2D_JPG_FIFO_FULL,              0x00);

	    // not related to jpeg decoder.. only to protect mp3 do asrc
	    vWriteDspWORD(ADDR_RC2D_BYPASS_ASRC_DEC2,0); // asrc enable
        ///LogSD("Dsp_dec2", u4ReadDspShmDWRD(D_JPG_LENGTH));

        break;
    #endif
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
        vWriteDspWORD (ADDR_RC2D_PCM_PREBUF_DELAY_BANK_DEC2, 
                       u2ReadDspShmWORD (W_PCM_PREBUF_DELAY_BANK_DEC2));
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
        break;
    case MPEG12_STREAM:
        /* 20041220: For testing, hard coded here, remove in the Future */
        // MPEG sync no error checking
//      vWriteDspWORD (ADDR_RC2D_MPEG_SYNC_MODE_DEC2, 0x1);
        //MPEG Error Detection
        vWriteDspWORD (ADDR_RC2D_MPEG_CRC_FLAG_DEC2,
                       u2ReadDspShmWORD (W_MPEGERRDET_DEC2));
        vWriteDspWORD(ADDR_RC2D_MPEG_SYNC_MODE_DEC2, 0x1);
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
        // for NR in BTSC decoder
        vWriteDspWORD (ADDR_RC2D_NR_ENABLE,u2ReadDspShmWORD (W_NR_ENABLE));
        vWriteDspWORD (ADDR_RC2D_NR_MTS_TH1_HIGH,u2ReadDspShmWORD (W_NR_MTS_TH1_HIGH));
        vWriteDspWORD (ADDR_RC2D_NR_MTS_TH1_LOW,u2ReadDspShmWORD (W_NR_MTS_TH1_LOW));
        vWriteDspWORD (ADDR_RC2D_NR_MTS_TH2_HIGH,u2ReadDspShmWORD (W_NR_MTS_TH2_HIGH));
        vWriteDspWORD (ADDR_RC2D_NR_MTS_TH2_LOW,u2ReadDspShmWORD (W_NR_MTS_TH2_LOW));
        vWriteDspWORD (ADDR_RC2D_NR_SAP_TH1_HIGH,u2ReadDspShmWORD (W_NR_SAP_TH1_HIGH));
        vWriteDspWORD (ADDR_RC2D_NR_SAP_TH1_LOW,u2ReadDspShmWORD (W_NR_SAP_TH1_LOW));
        vWriteDspWORD (ADDR_RC2D_NR_SAP_TH2_HIGH,u2ReadDspShmWORD (W_NR_SAP_TH2_HIGH));
        vWriteDspWORD (ADDR_RC2D_NR_SAP_TH2_LOW,u2ReadDspShmWORD (W_NR_SAP_TH2_LOW));
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
        // for NR in A2 decoder
        vWriteDspWORD (ADDR_RC2D_NR_ENABLE,u2ReadDspShmWORD (W_NR_ENABLE));
        vWriteDspWORD (ADDR_RC2D_NR_A2_TH1_HIGH,u2ReadDspShmWORD (W_NR_A2_TH1_HIGH));
        vWriteDspWORD (ADDR_RC2D_NR_A2_TH1_LOW,u2ReadDspShmWORD (W_NR_A2_TH1_LOW));
        vWriteDspWORD (ADDR_RC2D_NR_A2_TH2_HIGH,u2ReadDspShmWORD (W_NR_A2_TH2_HIGH));
        vWriteDspWORD (ADDR_RC2D_NR_A2_TH2_LOW,u2ReadDspShmWORD (W_NR_A2_TH2_LOW));
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
         //for PAL FM BAD SIGNAL CHECK 
	  vWriteDspWORD (ADDR_PAL_HDEV_BAD_SIGNAL_CHECK,uReadDspShmBYTE (B_FM_BAD_SIGNAL));
        vWriteDspWORD (ADDR_DEM_PAL_FINE_TUNE_VOLUME,u4ReadDspShmDWRD  (D_PAL_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (ADDR_DEM_NICAM_FINE_TUNE_VOLUME,u4ReadDspShmDWRD (D_NICAM_FINE_TUNE_VOLUME)>>8);       
        vWriteDspWORD (ADDR_DEM_AM_FINE_TUNE_VOLUME,u4ReadDspShmDWRD  (D_AM_FINE_TUNE_VOLUME)>>8);
        // for NR in PAL decoder
        vWriteDspWORD (ADDR_RC2D_NR_ENABLE,u2ReadDspShmWORD (W_NR_ENABLE));
        vWriteDspWORD (ADDR_RC2D_NR_PAL_TH1_HIGH,u2ReadDspShmWORD (W_NR_PAL_TH1_HIGH));
        vWriteDspWORD (ADDR_RC2D_NR_PAL_TH1_LOW,u2ReadDspShmWORD (W_NR_PAL_TH1_LOW));
        vWriteDspWORD (ADDR_RC2D_NR_PAL_TH2_HIGH,u2ReadDspShmWORD (W_NR_PAL_TH2_HIGH));
        vWriteDspWORD (ADDR_RC2D_NR_PAL_TH2_LOW,u2ReadDspShmWORD (W_NR_PAL_TH2_LOW));
        vWriteDspWORD (ADDR_RC2D_NR_PAL_HDEV_TH1_HIGH,u2ReadDspShmWORD (W_NR_PAL_HDEV_TH1_HIGH));
        vWriteDspWORD (ADDR_RC2D_NR_PAL_HDEV_TH1_LOW,u2ReadDspShmWORD (W_NR_PAL_HDEV_TH1_LOW));
        vWriteDspWORD (ADDR_RC2D_NR_PAL_HDEV_TH2_HIGH,u2ReadDspShmWORD (W_NR_PAL_HDEV_TH2_HIGH));
        vWriteDspWORD (ADDR_RC2D_NR_PAL_HDEV_TH2_LOW,u2ReadDspShmWORD (W_NR_PAL_HDEV_TH2_LOW));
        vWriteDspWORD (ADDR_RC2D_NR_AM_TH1_HIGH,u2ReadDspShmWORD (W_NR_AM_TH1_HIGH));
        vWriteDspWORD (ADDR_RC2D_NR_AM_TH1_LOW,u2ReadDspShmWORD (W_NR_AM_TH1_LOW));
        vWriteDspWORD (ADDR_RC2D_NR_AM_TH2_HIGH,u2ReadDspShmWORD (W_NR_AM_TH2_HIGH));
        vWriteDspWORD (ADDR_RC2D_NR_AM_TH2_LOW,u2ReadDspShmWORD (W_NR_AM_TH2_LOW));

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
        break;
#endif
    default:        
     ;
    }
}

#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
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
    UINT32 u4DspData;

//CTRL3 NUM_1
// ... should be in modified according to MT539x ADSP  
#ifdef CC_MT5391_AUD_SUPPORT
    vWriteDspWORD (ADDR_RC2D_CTRL_8_NUM_1, (UINT16) 12);
    // gallen, 20080714
#endif

//Soft Mute (Max. 7)
    vWriteDspWORD (ADDR_RC2D_VOLUME_SOFT_MUTE_ORDER_DEC3,
                   u2ReadDspShmWORD (B_SOFTMUTEORDER_DEC3));
    HalFlushInvalidateDCache(); //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro

    switch (_bDspStrTypDec3)
    {
    case PCM_STREAM:
        u4DspData = wReadDspWORD (ADDR_D2RC_INPUT_CH_CFG_DEC3);
        u4DspData = u4DspData & ~0xf;
        u4DspData = u4DspData | 0x2;  // Hard set cdda channel config
        vWriteDspWORD (ADDR_D2RC_INPUT_CH_CFG_DEC3, u4DspData);
        vWriteDspWORD (ADDR_RC2D_PCM_INPUT_CHANNEL_NUM_DEC3, 
                       u2ReadDspShmWORD (W_PCM_INPUT_CHANNEL_NUM_DEC3));
        vWriteDspWORD (ADDR_RC2D_PCM_INPUT_CH_ASSIGNMENT_DEC3, 
                       u2ReadDspShmWORD (W_PCM_INPUT_CH_ASSIGNMENT_DEC3));
        vWriteDspWORD (ADDR_RC2D_PCM_INPUT_TYPE_DEC3, 
                       u2ReadDspShmWORD (W_PCM_INPUT_TYPE_DEC3));  //0:pcm24 1:IMA 3:MS 2:pcm16 //msb=1: little endian
        vWriteDspWORD (ADDR_RC2D_PCM_INPUT_SAMPLING_RATE_DEC3, 
                       u2ReadDspShmWORD (W_PCM_INPUT_SAMPLING_RATE_DEC3));
        break;
    case MPEG12_STREAM:
        /* 20041220: For testing, hard coded here, remove in the Future */
        // MPEG sync no error checking
//      vWriteDspWORD (ADDR_RC2D_MPEG_SYNC_MODE_DEC2, 0x1);
        //MPEG Error Detection
        vWriteDspWORD (ADDR_RC2D_MPEG_CRC_FLAG_DEC3,
                       u2ReadDspShmWORD (W_MPEGERRDET_DEC3));
        break;
    default:        
     ;
    }
}
#endif

#if 0
/***************************************************************************
     Function : vIECCommonInit
  Description : None
    Parameter : None
    Return    : None
***************************************************************************/
void vIECCommonInit (BOOL fgFirstDecoder)
{

    AUDIO_CFG_T tAudCfg;
    UINT16 wIecFlag;

    if (fgFirstDecoder)
    {
        _fgIECRAWOff = fgGetIecSetting (&wIecFlag, fgFirstDecoder);
        vWriteDspWORD (ADDR_RC2D_IEC_FLAG, wIecFlag);
        // get setting for previous frequency
        // If frequency is not the same, it will be set again in vDspSetFreq
        vGetAudCfgSetting (_dwDacFreq, &tAudCfg, fgFirstDecoder);

    }
    else
    {
        _fgIECRAWOffDec2 = fgGetIecSetting (&wIecFlag, fgFirstDecoder);
        vWriteDspWORD (ADDR_RC2D_IEC_FLAG_DEC2, wIecFlag);
        // get setting for previous frequency
        // If frequency is not the same, it will be set again in vDspSetFreq
        vGetAudCfgSetting (_dwDacFreqDec2, &tAudCfg, fgFirstDecoder);
    }
    vSetAudCfg (&tAudCfg, fgFirstDecoder);
}
#endif

// *********************************************************************

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

    _dwStcDiff = 0;


    if (_fgDspPlay)
    {
        vDspResetDec1 ();
    }
    if (_fgDspPlayDec2)
    {
        vDspResetDec2 ();
    }
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
    if (_fgDspPlayDec3)
    {
        vDspResetDec3 ();
    }
#endif
    _fgDspStop = TRUE;          // stop ok
    _fgDspStopIssue = TRUE;
    _fgDspPlay = FALSE;
    _fgDspRealPlay = FALSE;
    _fgDspFlush = FALSE;
    _fgSendSample = FALSE;
    _fgDspAoutMuted = FALSE;
    _bDspStrTyp = (UINT8)0xFF;  //Set default to 0xff, since type 0 is AC3.
    /* secondary decoder */
    _fgDspStopDec2 = TRUE;      // stop ok
    _fgDspStopIssueDec2 = TRUE;
    _fgDspPlayDec2 = FALSE;
    _fgDspRealPlayDec2 = FALSE;
    _fgDspFlushDec2 = FALSE;
    _fgDspAoutMutedDec2 = FALSE;
    _bDspStrTypDec2 = (UINT8)0xFF;  //Set default to 0xff, since type 0 is AC3.
    /* secondary decoder */
    _fgSendSample = FALSE;
    _fgDspReParsing = FALSE;
    _dwDspReInitPts = 0;
    _fgDspGetMpgTyp = FALSE;
    _fgDACOn = TRUE;

    _dwDspForceResetCntDec = 0;
    _dwDspForceResetCntDec2 = 0;
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
     /* third decoder */
    _fgDspStopDec3 = TRUE;      // stop ok
    _fgDspStopIssueDec3 = TRUE;
    _fgDspPlayDec3 = FALSE;
    _fgDspRealPlayDec3 = FALSE;
    _fgDspFlushDec3 = FALSE;
    _fgDspAoutMutedDec3 = FALSE;
    _dwDspForceResetCntDec3 = 0;
    _bDspStrTypDec3 = (UINT8)0xFF;  //Set default to 0xff, since type 0 is AC3.    
#endif


// DSP interrupt flag initial
    _dwDspSIntFail = 0;
    _fgDspRInt = FALSE;
    _fgDspSInt = FALSE;
    _fgDspForceReset = FALSE;
    _dwDspFreq = 0xffff;
    _dwDspRamCodeType = (UINT8)-1;

// DSP user operation initial
    _fgDspUop = FALSE;
    _dwDspUop = 0;

    WriteDspCommDram (ADDR_RC2D_DOWNLOAD_TABLE_FLAG, 0);
    _fgDspPtsSet = FALSE;
    if (!_fgDspWakeUp)
    {
        // We can only load code when not wakeup
        // Because at this time dsp is not in cache
        //lint -e{506}
        //lint -e{774}
        if (_fgCommRAMFromFlash)
        {
            vDspLoadRamCode (AUD_COMMRAM);
            vDspLoadRamCode (AUD_PCM_DEC1);// KK_PFGA
        }
        
        /*lint -e{506}*/
        /*lint -e{774}*/
        if (_fgPostRamFromFlash)
        {
            vDspLoadRamCode (AUD_POSTRAM);
        }
        
        _uDspInitState = DSP_INIT_ON;
        _uDspState = (UINT8)DSP_RC_INIT;
    }
}

/***************************************************************************
     Function :
  Description :
    Parameter :
    Return    :
***************************************************************************/
void vDspSuperReset (void)
{

}

/***************************************************************************
     Function : vDspRestDec1
  Description : Reset DSP ,affect DSP status
    Parameter :
    Return    :
****************************************************************************/
void vDspResetDec1 (void)
{
    UINT32 dwTryCnt;

    _dwDspForceResetCntDec++;

    if (!_fgDspWakeUp)
    {
        return;
    }
    if (!_fgDspForceReset)
    {
        dwTryCnt = 0;
        _fgDspStop = TRUE;
        _fgDspStopIssue = TRUE;
        _fgDspPlay = FALSE;
        _fgDspRealPlay = FALSE;
        _fgDspFlush = FALSE;
        _fgDspForceResetDec = TRUE;
        _fgSendSample = FALSE;
        _fgDspReParsing = FALSE;
        _dwDspReInitPts = 0;
        _dwDspRamCodeType = (UINT32)-1;
        _fgDspPtsSet = FALSE;

        dwTryCnt = 0;

        while (TRUE)
        {
            CRIT_STATE_T rCritState;
            dwTryCnt++;

            rCritState = x_crit_start();
            if (!fgDspBusy ())
            {
                SendDSPInt (INT_RC2D_DSP_JOB_RESET, (UINT32) JOB_RESET_DECODER);  // short data & interrupt
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
        vDspDecDes ();
    }
}

/***************************************************************************
     Function : vDspRestDec2
  Description : Reset DSP ,affect DSP status
    Parameter :
    Return    :
****************************************************************************/
void vDspResetDec2 (void)
{
    UINT32 dwTryCnt;

    _dwDspForceResetCntDec2++;

    if (!_fgDspWakeUp)
    {
        return;
    }
    if (!_fgDspForceResetDec2)
    {
        dwTryCnt = 0;
        _fgDspStopDec2 = TRUE;
        _fgDspStopIssueDec2 = TRUE;
        _fgDspPlayDec2 = FALSE;
        _fgDspRealPlayDec2 = FALSE;
        _fgDspFlushDec2 = FALSE;
        _fgDspForceResetDec2 = TRUE;
        _fgSendSampleDec2 = FALSE;
        _fgDspReParsingDec2 = FALSE;
        _dwDspReInitPtsDec2 = 0;
        _dwDspRamCodeTypeDec2 = (UINT32)-1;
        _fgDspPtsSetDec2 = FALSE;

        dwTryCnt = 0;

        while (TRUE)
        {
            CRIT_STATE_T rCritState;
            dwTryCnt++;

            rCritState = x_crit_start();
            if (!fgDspBusy ())
            {
                SendDSPInt (INT_RC2D_DSP_JOB_RESET, (UINT32) JOB_RESET_DECODER2); // short data & interrupt
                vDspBlockPtrReset (FALSE);
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
        //vDspDecDesDec2();
    }
}

#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
/***************************************************************************
     Function : vDspRestDec3
  Description : Reset DSP ,affect DSP status
    Parameter :
    Return    :
****************************************************************************/
void vDspResetDec3 (void)
{
    UINT32 dwTryCnt;

    _dwDspForceResetCntDec3++;

    if (!_fgDspWakeUp)
    {
        return;
    }
    if (!_fgDspForceResetDec3)
    {
        dwTryCnt = 0;
        _fgDspStopDec3 = TRUE;
        _fgDspStopIssueDec3 = TRUE;
        _fgDspPlayDec3 = FALSE;
        _fgDspRealPlayDec3 = FALSE;
        _fgDspFlushDec3 = FALSE;
        _fgDspForceResetDec3 = TRUE;
        _fgSendSampleDec3 = FALSE;
        _fgDspReParsingDec3 = FALSE;
        _dwDspReInitPtsDec3 = 0;
        _dwDspRamCodeTypeDec3 = (UINT32)-1;
        _fgDspPtsSetDec3 = FALSE;

        dwTryCnt = 0;

        while (TRUE)
        {
            CRIT_STATE_T rCritState;
            dwTryCnt++;

            rCritState = x_crit_start();
            if (!fgDspBusy ())
            {
                SendDSPInt (INT_RC2D_DSP_JOB_RESET, (UINT32) JOB_RESET_JOB3); // short data & interrupt
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
        //vDspDecDesDec3();
    }
}

#endif


/***************************************************************************
     Function : vDspRestSystem
  Description : Reset DSP ,affect DSP status
    Parameter :
    Return    :
****************************************************************************/
/*lint -e{957}*/
void vDspResetSystem (void)
{

}

/***************************************************************************
     Function : vDspForceReset
  Description : Reset DSP ,affect DSP status
    Parameter :
    Return    :
***************************************************************************/
/*lint -e{957}*/
void vDspForceReset (void)
{
    /*
       UINT32 dwTryCnt;

       #ifdef NO_AUDIO
       return;
       #endif

       // 20040405 sammy added for exception handling
       if(_fgDspExcptn)
       return;

       #ifdef RESET_DEBUG_ON
       DBGLogS("Force Reset!!");
       #endif


       if (!_fgDspWakeUp)
       {
       return;
       }
       if (!_fgDspForceReset)
       {
       dwTryCnt = 0;
       _fgDspStop = TRUE;
       _fgDspStopIssue = TRUE;
       _fgDspPlay = FALSE;
       _fgDspRealPlay = FALSE;
       _fgDspFlush = FALSE;
       _fgDspForceReset = TRUE;
       _fgSendSample = FALSE;
       _fgDspReParsing = FALSE;
       _fgDspGetMpgTyp = FALSE;    //
       _dwDspReInitPts = 0;
       _dwDspRamCodeType = -1;

       //    _fgDspRamCodeLoad = FALSE;

       _fgDspPtsSet = FALSE;
       dwTryCnt = 0;

       while (TRUE)
       {
       dwTryCnt++;
       if (!fgDspBusy ())
       {
       SendDSPInt (INT_RC2D_FLOW_CONTROL, (UINT32) FLOW_CONTROL_RESET);  // short data & interrupt
       vDspBlockPtrReset ();
       break;
       }

       if (dwTryCnt == DSP_TIMEOUT)
       {
       SendDSPInt (INT_RC2D_FLOW_CONTROL, (UINT32) FLOW_CONTROL_RESET);  // short data & interrupt
       vDspBlockPtrReset ();
       break;
       }

       }
       vDspDecDes();
       }
     */
}

#ifdef ADSP_JPEG_DEC2
void vSetJPEGFileSize(UINT32 dwFileSize)
{
    if(dwFileSize > 0x7FFFFF)
        dwFileSize = 0x7FFFFF;
    vWriteDspShmDWRD(D_JPG_LENGTH, dwFileSize/256);
}
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
    UINT32  u4Temp=0;
#ifdef ADSP_JPEG_DEC2
    extern UINT32 dwGetDJpgAreaSa(void);
    extern UINT32 dwGetDJpgAreaEa(void);
#endif

#ifdef DSP_SUPPORT_NPTV
    AUD_MTS_FAC_T*     prMtsFactory;
    AUD_A2_FAC_T*     prA2Factory;
    AUD_PAL_FAC_T*     prPalFactory;
    UNUSED(DRVCUST_OptQuery(eAudioMtsFactory, (UINT32 *)(void *) &prMtsFactory));
    UNUSED(DRVCUST_OptQuery(eAudioA2Factory, (UINT32 *)(void *) &prA2Factory));
    UNUSED(DRVCUST_OptQuery(eAudioPalFactory, (UINT32 *)(void *) &prPalFactory));
#endif //DSP_SUPPORT_NPTV
    UNUSED(u4Temp);

    // for debug use
    vWriteDspShmDWRD (D_SPKCFG, _u4DspGetSpkConfig()); // full channel
    
    vWriteDspShmDWRD (D_SPKCFG_DEC2,     0x0); // LtRt

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

#ifdef ADSP_JPEG_DEC2
    vWriteDspShmWORD(W_JPG_CMD_FLAG,            0x00);  //reset bit 0 while initialize
    ///All decode thumbnail.
    //vWriteDspShmWORD(W_JPG_CMD_FLAG, (u2ReadShmUINT16(W_JPG_CMD_FLAG)|0x80));   ///Set bit7 for decoder thumbnail in JPEG.
    vWriteDspShmBYTE(B_JPG_MAX_SCAN_NUM,        0x64);  //SRAM = 0x6400

    u4Temp = (dwGetDJpgAreaEa() - dwGetDJpgAreaSa());
    u4Temp -= 0x880*4;                                  ///[Joel] 0x880 for DSP variable & table use.
    u4Temp /= 2;                                        /// /2 for pip-pop buffer.
    vWriteDspShmDWRD(D_JPG_MAX_COMPACT_ADDR,(u4Temp/4)>>8); /// DW and 100h align, Set Max Compact DRAM size for DSP use.
    u4Temp = u4Temp/2/16;                               /// max width = DSP Buffer/2/16 (4:2:0 type)
    vWriteDspShmWORD(W_JPG_MAX_WIDTH, (UINT16)u4Temp);   ///Set bit7 for decoder thumbnail in JPEG.
#endif
    
    // Set IEC output channel selection to SPDIF
    // For temporary use, need to move to driver custermization
//#ifdef CC_AUD_IEC_OUT_SEL_SPDIF
    vWriteDspShmBYTE (B_IEC_PCMCH,          0x3);
//#endif

    vWriteDspShmWORD (W_PROCMOD,    IS_BYPASS_PROC() ? 0xfc00 : (0xc00 | (IS_SPKCFG_SSLN()<<7)) ); // ch9/10 downmix  
    vWriteDspShmBYTE (B_KARAFLAG,          0x0);
    vWriteDspShmDWRD (D_LRMIXRATIO,        0x400000);  // 0.5
    vWriteDspShmBYTE (B_KARAFLAG_DEC2,     0x0);
    vWriteDspShmDWRD (D_LRMIXRATIO_DEC2,   0x400000);  // 0.5

    //vWriteDspShmWORD (W_PROCMOD_DEC2, 0x9000);
    vWriteDspShmWORD (W_PROCMOD_DEC2, 0x9400);		//downmix to LR
#ifdef CC_MT5391_AUD_3_DECODER
    //vWriteDspShmDWRD (W_PROCMOD_DEC3,    0xb000);
    vWriteDspShmDWRD (W_PROCMOD_DEC3,    0x9000);
#endif    

#ifndef DSP_POST_PROCESS_ENABLE
    vWriteDspShmBYTE (B_EQFLAG,          0); // Initial Equalizer to off
    vWriteDspShmBYTE (B_REVERBFLAG,      0); // reverb off
    vWriteDspShmWORD (W_PLIICONFIG,      0);
#ifndef CC_NEW_POST_MULTI_VS_SUPPORT
    vWriteDspShmBYTE (B_VSURRFLAG,       VSS_NONE);
#else
    vWriteDspShmWORD (W_VSURRFLAG,       VSS_NONE);
#endif
    vWriteDspShmBYTE (B_EQFLAG_DEC2,     0); // Initial Equalizer to off
    vWriteDspShmBYTE (B_REVERBFLAG_DEC2, 0); // reverb off
#ifndef CC_NEW_POST_MULTI_VS_SUPPORT
    vWriteDspShmBYTE (B_VSURRFLAG_DEC2,  VSS_NONE);
#else
    vWriteDspShmWORD (W_VSURRFLAG_DEC2,  VSS_NONE);
#endif
#endif //DSP_POST_PROCESS_ENABLE

#ifdef DSP_EQ2_SBASS_SUPPORT
    vWriteDspShmDWRD(D_SBASSBOOSTGAIN,  (BASS_TREBLE_GAIN_MAX-1)/2);
    vWriteDspShmDWRD(D_SBASSCLEARGAIN,  (BASS_TREBLE_GAIN_MAX-1)/2);
#endif   

    vWriteDspShmBYTE (B_IECFLAG,         0);
    //vWriteDspShmBYTE (B_IECFLAG_DEC2,    0);
    vWriteDspShmBYTE (B_IEC_MAX_FREQ, SV_48K);   
    vWriteDspShmBYTE (B_IEC_COPYRIGHT, IEC_COPY_FREELY);
    if  (AUD_GetTargetCountry() == COUNTRY_EU) 
    {
        vWriteDspShmBYTE (B_IEC_CATEGORY_CODE, IEC_ORIGINAL_BROAD_CAST_EU);
    }
    else
    {
        vWriteDspShmBYTE (B_IEC_CATEGORY_CODE, IEC_ORIGINAL_BROAD_CAST);
    }

    // DSP Group 0 Initial
    vWriteDspShmWORD (W_INTCTRL,           INTERRUPT_CTRL);
    vWriteDspShmWORD (W_SYSTEM_CTRL,        0); // bit0: enable internal buffer, bit1: internal data is from dec1(0) or dec2(1)
    vWriteDspShmWORD (W_INTERNAL_BUF_IN_FLAG, 0x2);  // bit 1: fill internal buffer after volume_dec2
#ifdef CC_MT5391_AUD_3_DECODER
    vWriteDspShmWORD (W_INTERNAL_BUF2_IN_FLAG, 0x100);
#endif
    vWriteDspShmWORD (W_SYSTEM_SETUP,       /*0x0*/0x8000 | USE_STC_A1 | AIN_BUF_RD_NOT_HOLD_WR  | (((UINT8)DRVCUST_OptGet(eAudioDownmixLR))<<6)| (((UINT8)DRVCUST_OptGet(eAudioDownmixPosition))<<4)| (((UINT8)DRVCUST_OptGet(eAudioMultiPairOutput))<<3)| (IS_SPKCFG_SSLN()<<2)); //FIXME    
    //Audio Description    
    vWriteDspShmWORD (W_SYSTEM_SETUP_DEC2,  /*0x0*/0x8000 | USE_STC_A1 | AIN_BUF_RD_NOT_HOLD_WR); //FIXME
#ifdef CC_MT5391_AUD_3_DECODER
    vWriteDspShmWORD (W_SYSTEM_SETUP_DEC3,  /*0x0*/0x8000 | USE_STC_A1 | AIN_BUF_RD_NOT_HOLD_WR); //FIXME
#endif  

    vWriteDspShmWORD (W_SPEED,              0);
    vWriteDspShmBYTE (B_BIT,               24); // Bit Resolution

    #if 1 //this is for 250ms channel delay //MT8223_NOTES_Zhaoyuan decrease dec1 data size //change to 5363 config by xiaoyi100319
    vWriteDspShmBYTE (B_BANK576NUM,         9+13); // for mp3 modification
    vWriteDspShmBYTE (B_BANK384NUM,        12+20);
    vWriteDspShmBYTE (B_BANK320NUM,        14+24);
    vWriteDspShmBYTE (B_BANK256NUM,        18+30);//sunman for 250ms (48kHz) buffer

    #else
    vWriteDspShmBYTE (B_BANK576NUM,         9); // for mp3 modification
    vWriteDspShmBYTE (B_BANK384NUM,        12);
    vWriteDspShmBYTE (B_BANK320NUM,        14);
    vWriteDspShmBYTE (B_BANK256NUM,        18);//sunman for 250ms (48kHz) buffer
    #endif
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
    vWriteDspShmBYTE (B_IECLATENCY,         0);
    vWriteDspShmBYTE (B_VOLUPORDER_DEC2,    3); /* volume display up order */
    vWriteDspShmBYTE (B_VOLDOWNORDER_DEC2,  6); /* volume display down order */
#ifdef __MODEL_slt__    
    vWriteDspShmBYTE (B_SOFTMUTEORDER_DEC2,      0); 
#else
    vWriteDspShmBYTE (B_SOFTMUTEORDER_DEC2, 4); /* pause key fade in/out */
#endif //__MODEL_slt__

#ifdef CC_MT5391_AUD_3_DECODER
#ifdef __MODEL_slt__    
    vWriteDspShmBYTE (B_SOFTMUTEORDER_DEC3,      0); 
#else
    vWriteDspShmBYTE (B_SOFTMUTEORDER_DEC3, 4); /* pause key fade in/out */
#endif
#endif //CC_MT5391_AUD_3_DECODER    
     vWriteDspShmWORD (W_ERRORMUTEBANK_DEC2, 0);
    //vWriteDspShmBYTE (B_IECLATENCY_DEC2,    0);

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
#endif

    //DRC
    vWriteDspShmBYTE (B_DRC_AVEORDER,           12);
    vWriteDspShmBYTE (B_DRC_ATTACKORDER,         4);
    vWriteDspShmWORD (W_DRC_THRESHOLD,      0x7FF0);
    vWriteDspShmWORD (W_DRC_MARGIN,          0x400);
    vWriteDspShmDWRD (D_DRC_RE_GAIN,       0x20000);

    // Operation mode 
    if  (AUD_GetTargetCountry() == COUNTRY_EU) 
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

#ifdef FRAC_SPEED_SUPPORT
    vWriteDspShmDWRD(D_PLAY_SPEED, 0x00000000); //sunman for play speed : initial set not on
#endif

    // Light added for AVC
    vWriteDspShmWORD (W_AVC_TARGET_LEV, -20);
    vWriteDspShmWORD (W_AVC_SILENCE_LEV,              (-98));
    vWriteDspShmWORD (W_AVC_MAX_GAIN_UP,        6);
    vWriteDspShmWORD (W_AVC_MAX_GAIN_DOWN,               40);
    vWriteDspShmWORD (W_AVC_FLAG,                         3); // 3: On, 0: off
    vWriteDspShmWORD (W_AVC_ATTACK_THRES,  AVC_ATTACK_THRES);
    vWriteDspShmWORD (W_AVC_ADJUST_RATE,               0x80); // (8.8) format, 0x80 = 0.5 dB
    vWriteDspShmWORD (W_DSP_AVC_UI_ADJUST_RATE,        0x80); // (16.8) format, 4dB / sec @ 32k Hz

    vWriteDspShmWORD (W_AVC_TARGET_LEV_DEC2, -20);
    vWriteDspShmWORD (W_AVC_SILENCE_LEV_DEC2,              (-98));
    vWriteDspShmWORD (W_AVC_MAX_GAIN_UP_DEC2,        6);
    vWriteDspShmWORD (W_AVC_MAX_GAIN_DOWN_DEC2,               40);
    vWriteDspShmWORD (W_AVC_FLAG_DEC2,                         3); // 3: On, 0: off
    vWriteDspShmWORD (W_AVC_ATTACK_THRES_DEC2,  AVC_ATTACK_THRES);
    vWriteDspShmWORD (W_AVC_ADJUST_RATE_DEC2,               0x80); // (8.8) format, 0x80 = 0.5 dB
    vWriteDspShmWORD (W_DSP_AVC_UI_ADJUST_RATE_DEC2,        0x80); // (16.8) format, 4dB / sec @ 32k Hz
    
//-----------------
    // Mixsound
    vWriteDspShmWORD (W_MIXSOUND_FLAG,         0);
#ifndef DSP_TEST_MIXSOUND_ADPCM // PCM    
    vWriteDspShmWORD (W_MIXSOUND_CFG,     0x1f02); // LR/C/LsRs/Ch78/Ch910 mix, Mono, LPCM, 4X
#else   // ADPCM
    vWriteDspShmWORD (W_MIXSOUND_CFG,     0x1f06); // LR/C/LsRs/Ch78/Ch910 mix, Mono, ADPCM, 4X
#endif //DSP_TEST_MIXSOUND_ADPCM
    vWriteDspShmDWRD (D_MIXSOUND_PADDR,   u4GetMixSoundStartByteAddr());
    vWriteDspShmDWRD (D_MIXSOUND_PLEN,      1536); // 24*64 12kHz
    vWriteDspShmDWRD (D_MIXSOUND_GAIN,   0x20000); // 0x20000 --> 0dB

    // System master
    vWriteDspShmDWRD (D_STC_DIFF_LO,       (STC_DIFF_BOUND_032MS * 2 *(1+u4GetStcDiffFactor(AUD_STC_LO))) );      // 32ms (1 frame of AC-3 @ 48kHz)
    vWriteDspShmDWRD (D_STC_DIFF_HI,       (STC_DIFF_BOUND_064MS * 2 * (1+u4GetStcDiffFactor(AUD_STC_HI))) );  // 128ms (4 frame of AC-3 @ 48kHz)
    vWriteDspShmDWRD (D_STC_DIFF_WS,       (STC_DIFF_BOUND_500MS * (1+u4GetStcDiffFactor(AUD_STC_WS))) );      // 0.5 second
    vWriteDspShmDWRD (D_STC_THRESHOLD,      STC_THR_42DB);                // -42dB
    vWriteDspShmDWRD (D_STC_DIFF_LO_DEC2,   STC_DIFF_BOUND_032MS * 2);        // 32ms (1 frame of AC-3 @ 48kHz)
    vWriteDspShmDWRD (D_STC_DIFF_HI_DEC2,  (STC_DIFF_BOUND_064MS * 2));   // 128ms (4 frame of AC-3 @ 48kHz)   
    vWriteDspShmDWRD (D_STC_DIFF_WS_DEC2,   STC_DIFF_BOUND_500MS);        // 0.5 second
    
    vWriteDspShmDWRD (D_STC_THRESHOLD_DEC2, STC_THR_42DB);                // -36dB

    vWriteDspShmDWRD (D_1ST_PTS_PRS_PNT,        0);
    vWriteDspShmWORD (W_1ST_PTS_STCH,           0);
    vWriteDspShmWORD (W_1ST_PTS_STCL,           0);  
    vWriteDspShmDWRD (D_1ST_PTS_PRS_PNT_DEC2,   0);
    vWriteDspShmWORD (W_1ST_PTS_STCH_DEC2,      0);
    vWriteDspShmWORD (W_1ST_PTS_STCL_DEC2,      0);  
        
    vWriteDspShmDWRD (D_STC_THRESHOLD_DEC2, STC_THR_36DB); // -36dB

    // Decoding start point
    vWriteDspShmDWRD (D_DECODING_STR_PNT,       _u4AFIFO[0]);
    vWriteDspShmDWRD (D_DECODING_STR_PNT_DEC2,  _u4AFIFODec2[0]);

    // DSP Group6 Initial
    //Individual Speaker Config Setting
    vWriteDspShmWORD (W_AC3_SPKCFG,         0xFFFF);
    vWriteDspShmWORD (W_MP2_SPKCFG,         0xFFFF);

    //Address Setting
    vWriteDspShmDWRD (D_MEMBACKUPADDR,      CMPT_POST_TEMP_BUFFER);
    vWriteDspShmDWRD (D_REVERBMEMADDR,      CMPT_REVERB_BUFFER);
    vWriteDspShmDWRD (D_REVERBMEMADDR_DEC2, CMPT_REVERB_BUFFER_DEC2);
    vWriteDspShmDWRD (D_3D_BUF_ADDR,        CMPT_BUFFER_ADDRESS_UPMIX); // 0x0d000
    vWriteDspShmDWRD (D_DNMX_BUF_ADR,       CMPT_BUFFER_ADDRESS_DONWMIX);
    vWriteDspShmDWRD (D_MEMBACKUPADDR_DEC2, CMPT_POST_TEMP_BUFFER_DEC2);

    // DSP Group7 Initial
    vWriteDspShmBYTE (B_SPECTRUMUP,         SPECTRUM_HIGH_ORDER); //Spectrum Order
    vWriteDspShmBYTE (B_SPECTRUMDOWN,       SPECTRUM_LOW_ORDER);

    //Reverb
    vWriteDspShmBYTE (B_REVERBBANK0,        79);
    vWriteDspShmBYTE (B_REVERBBANK1,        77);
    vWriteDspShmBYTE (B_REVERBBANK2,        75);
    vWriteDspShmBYTE (B_REVERBBANK3,        73);
    vWriteDspShmDWRD(D_REVERBGAIN,       0x02800);
    vWriteDspShmBYTE (B_REVERBBANK0_DEC2,   79);
    vWriteDspShmBYTE (B_REVERBBANK1_DEC2,   67);
    vWriteDspShmBYTE (B_REVERBBANK2_DEC2,   59);
    vWriteDspShmBYTE (B_REVERBBANK3_DEC2,   47);

    vWriteDspShmWORD (W_WMAPACKET,           0);
    vWriteDspShmWORD (W_VORBISPACKET,        0);
    // Bass filter cutoff freq
    vWriteDspShmDWRD (D_CUTOFF_FREQ,            CUTOFF_FREQ);
    vWriteDspShmDWRD (D_CUTOFF_FREQ_DEC2,  CUTOFF_FREQ);

    // Wait 8032 to add the feature and remove the following line
    vWriteDspShmBYTE (B_AC3AUTODNMIX,        0);

    // initialization of B_EQBANDNUM or B_EQBANDNUM_DEC2
    vWriteDspShmBYTE (B_EQBANDNUM,           4);
    vWriteDspShmBYTE (B_EQBANDNUM_DEC2,      4);

    //Virtual Surround
#ifndef CC_NEW_POST_MULTI_VS_SUPPORT
    vWriteDspShmBYTE (B_VSURRFLAG,    VSS_NONE);
#else
    vWriteDspShmWORD (W_VSURRFLAG,    VSS_NONE);
#endif
    vWriteDspShmBYTE (B_VSURRDELAYNUM,       5);
    vWriteDspShmDWRD (D_VSURRGAIN,     0x20000);
    vWriteDspShmDWRD (D_VSURRWIDEGAIN, 0x20000);

#ifdef CC_TOSHIBA_AUD_TEMP
    vWriteDspShmDWRD (D_VSURR_CLARITY,   0xb0000);   //center gain, 
    vWriteDspShmDWRD (D_VSURR_WIDTH,   0x50000);   //surround gain,
    vWriteDspShmDWRD (D_VSURR_LR_GAIN,   0x200000); //L/R gain,
    vWriteDspShmDWRD (D_VSURR_CROS_TALK,   0xc0000); //crosstalk,
    vWriteDspShmDWRD (D_VSURR_OUTPUT_GAIN,   0x80000); //output gain,   
    vWriteDspShmDWRD (D_VSURR_BASS_GAIN,   0x00000); //bass enhancement,
    vWriteDspShmDWRD (D_VSURR_FO,   0x07f3C00); //FO,
#else
    vWriteDspShmDWRD (D_VSURR_CLARITY,   0xa0000);   //center gain, 
    vWriteDspShmDWRD (D_VSURR_WIDTH,   0xa0000);   //surround gain,
    vWriteDspShmDWRD (D_VSURR_LR_GAIN,   0x200000); //L/R gain,
    vWriteDspShmDWRD (D_VSURR_CROS_TALK,   0xa0000); //crosstalk,
    vWriteDspShmDWRD (D_VSURR_OUTPUT_GAIN,   0xc0000); //output gain,   
    vWriteDspShmDWRD (D_VSURR_BASS_GAIN,   0x150000); //bass enhancement,
    vWriteDspShmDWRD (D_VSURR_FO,   0x07f4300); //FO,
#endif    

/*    
    *(pbDspInfo + B_VSURRFLAG)      = 0;
    *(pbDspInfo + B_VSURRDELAYNUM)  = 5;
    WRITE_DWRDL(pbDspInfo + D_VSURRGAIN,     0x20000);
    WRITE_DWRDL(pbDspInfo + D_VSURRWIDEGAIN, 0x20000);
*/  
#if defined(DSP_SUPPORT_SRSWOW) || defined(DSP_SUPPORT_SRSTSXT)
    _b3DSurrMode = SV_OFF;
#endif

#ifdef DSP_SUPPORT_SRSWOW
    /* SRS WOW */
    //Rice: SRS WOW initial values for user-adjustable parameters.  
    DSP_SRSWOWSetSpkSize(1);   //default: 1: 60Hz
    //vWriteDspShmD(D_SRS_WOW_SPKSIZE,    0x01);       
    
    vWriteDspShmDWRD(D_SRS_WOW_ENABLE,    0x0100);     //mEnable
    vWriteDspShmDWRD(D_SRS_WOW_LMENB,     0x00000000); //mLimiterEnable
    vWriteDspShmDWRD(D_SRS_WOW_BRIGHT,    0x00333333); //mBrightness
    vWriteDspShmDWRD(D_SRS_WOW_WOWCTRL,   0x00666667); //mWowControl
    vWriteDspShmDWRD(D_SRS_WOW_TBCTRL,    0x004ccccd); //mTruBassControl
    vWriteDspShmDWRD(D_SRS_WOW_LMCTRL,    0x00600000); //mLimiterControl
#endif //DSP_SUPPORT_SRSWOW

    /* SRS TSXT2CH */
#ifdef DSP_SUPPORT_SRSTSXT
    /*
        Rice: Define all flags by nibble for easy maintaining.

        bit  0~ 3:  Bypass (0: No, 1: Yes)
        bit  4~ 7:  TS Disable (0: No, 1: Yes)
        bit  8~11:  TS Headphone Mode (0: No, 1: Yes)
        bit 12~15:  Input Mode  (0~12)
        bit 16~19:  FOCUS Enable (0: No, 1: Yes)
        bit 20~23:  TruBass Enable (0: No, 1: Yes)
        bit 24~27:  TruBass Speaker Size (0~7)
        bit 28~31:  reserved
    */
    vWriteDspShmDWRD(D_SRS_TSXT_FLAGS,         0x02111000); //default speaker size 2 
    vWriteDspShmDWRD(D_SRS_TSXT_FC_Elevation,  0x00400000); 
    vWriteDspShmDWRD(D_SRS_TSXT_TB_TBCtrl,     0x004ccccd);
    vWriteDspShmDWRD(D_SRS_TSXT_Input_gain,    0x005ae140);
#endif //DSP_SUPPORT_SRSTSXT

    /* SRS TSHD2CH */
#ifdef DSP_SUPPORT_SRSTSHD //sunman_tshd
    /*
        ;ts_mode_flags
        ;bit-0	enable Bypass, make sure C out disable
        ;bit-1	enable TruBass
        ;bit-2	enable Definition
        ;bit-3	enable Focus
        ;bit-4	enable limiter
        ;bit-5  enable C out
        ;bit-6  enable LFE out
        ;bit-7	enable Headphone mode	
        ;bit-8  enable SubTruBass
        ;bit-9  ts_trusurround
        ;bit-10 ts_passive
        ;bit-11 set space and center
        ;bit-12 LtRt
        ;bit-13 PL2_music
        ;bit-15	move lfe to left	
    */
	vWriteDspShmDWRD(D_SRS_TSHD_FLAGS, 0x0000101e);

	vWriteDspShmDWRD(D_SRS_TSHD_INPUT_GAIN, DRVCUST_OptGet(eAudioTSHDInputGain)); //-8dB
        vWriteDspShmDWRD(D_SRS_TSHD_OUTPUT_GAIN, DRVCUST_OptGet(eAudioTSHDOutputGain)); //0dB
	vWriteDspShmDWRD(D_SRS_TSHD_Surr_Level, DRVCUST_OptGet(eAudioTSHDSurrLvl) ); //60%
	vWriteDspShmBYTE(B_SRS_TSHD_SPKSIZE, (UINT8)DRVCUST_OptGet(eAudioTSHDSpkSize)); //150Hz
	vWriteDspShmDWRD(D_SRS_TSHD_TB_TBCtrl, DRVCUST_OptGet(eAudioTSHDTbCtrl));      //50%
	vWriteDspShmDWRD(D_SRS_TSHD_DefCtrl, DRVCUST_OptGet(eAudioTSHDDefCtrl));          //30%	
	vWriteDspShmDWRD(D_SRS_TSHD_FC_Elevation,  DRVCUST_OptGet(eAudioTSHDElevCtrl)); //50%

        vWriteDspShmBYTE(B_SRS_TSHD_SUBSPKSIZE, 0);
	vWriteDspShmBYTE(B_SRS_TSHD_COFREQ, 0);
	vWriteDspShmBYTE(B_SRS_TSHD_INP_TYPE, 1 );
	vWriteDspShmBYTE(B_SRS_TSHD_CH_DEST, 7);
	vWriteDspShmDWRD(	D_SRS_TSHD_WOW_FC_ctrl, 0x00333333);
	vWriteDspShmDWRD(D_SRS_TSHD_LimiterCtrl, 0x00600000);    
#endif //DSP_SUPPORT_SRSTSHD

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

   vWriteDspShmWORD (W_PCM_PREBUF_DELAY_BANK, 0); // 256 sample/unit, 
                                                  //ie. 5.33 ms/uinit with 48 KHz 
   vWriteDspShmWORD (W_PCM_PREBUF_DELAY_BANK_DEC2, 0);    

#ifdef DSP_SUPPORT_NPTV
   //all ATV audio shared shm
    vWriteDspShmWORD (W_USER_MODE,          0);
#ifdef __MODEL_slt__ 
    vWriteDspShmWORD (W_SOURCE_MODE,      0);
#else
    vWriteDspShmWORD (W_SOURCE_MODE,      1);
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
      ((UINT8)prPalFactory->fgCarrierShift << 0) ));  
#endif //__MODEL_slt__
    vWriteDspShmBYTE (B_FM_SATU_MUTE_MODE,   
    (((UINT8)prMtsFactory->fgSatuMuteMode<< 2) |
     ((UINT8)prA2Factory->fgSatuMuteMode << 1) |
      ((UINT8)prPalFactory->fgSatuMuteMode << 0) ));  
    vWriteDspShmBYTE (B_FM_MUTE_MODE,   
    (((UINT8)prMtsFactory->fgCarrierMuteMode<< 2) |
     ((UINT8)prA2Factory->fgCarrierMuteMode << 1) |
      ((UINT8)prPalFactory->fgCarrierMuteMode << 0) ));  
    vWriteDspShmBYTE (B_NON_EU_FM_MODE,   
    (((UINT8)prA2Factory->fgNonEuMode) |
      ((UINT8)prPalFactory->fgNonEuMode))); 
    
    // MTS Parameters    
    vWriteDspShmWORD(W_MTS_NUM_CHECK, prMtsFactory->u2NumCheck);
    vWriteDspShmWORD(W_MTS_STEREO_MID, prMtsFactory->u2PilotLowerThd);
    vWriteDspShmWORD(W_MTS_STEREO_CON_MID, prMtsFactory->u2PilotHigherThd);
    vWriteDspShmWORD(W_MTS_NUM_PILOT, prMtsFactory->u2NumPilot);
    vWriteDspShmWORD(W_MTS_NUM_SAP, prMtsFactory->u2NumSap);
    vWriteDspShmWORD(W_MTS_SAP_MID, prMtsFactory->u2SapLowerThd);
    vWriteDspShmWORD(W_MTS_SAP_CON_MID, prMtsFactory->u2SapHigherThd);
    vWriteDspShmDWRD( D_SAP_FINE_TUNE_VOLUME, 
                          (VOLUME_FINE_TUNE[vFineTuneVolTblIdxChk(prMtsFactory->u1SapPrescale)]) << 8);
    vWriteDspShmDWRD( D_MTS_FINE_TUNE_VOLUME, 
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
    vWriteDspShmBYTE (B_SAP_FILTER_SEL,   0);
    vWriteDspShmBYTE (B_SAP_RATIO,   0x16); //Original :0x1C  //US FT issue ... SAP sensitivity
#ifdef CC_AUD_MTS_OUTPUT_LEVEL_GAIN_SUPPORT
    vWriteDspShmDWRD(D_MTS_OUTPUT_LEVEL_GAIN, prMtsFactory->u2OutputLevelGain);
#endif
    // Noise reduction for MTS
    vWriteDspShmWORD(W_NR_ENABLE, 0x0);
    vWriteDspShmWORD(W_NR_MTS_TH1_HIGH, 0x70);
    vWriteDspShmWORD(W_NR_MTS_TH1_LOW, 0x50);
    vWriteDspShmWORD(W_NR_MTS_TH2_HIGH, 0x18);
    vWriteDspShmWORD(W_NR_MTS_TH2_LOW, 0x10);     
    vWriteDspShmWORD(W_NR_SAP_TH1_HIGH, 0x70);
    vWriteDspShmWORD(W_NR_SAP_TH1_LOW, 0x30);
    vWriteDspShmWORD(W_NR_SAP_TH2_HIGH, 0x1b);
    vWriteDspShmWORD(W_NR_SAP_TH2_LOW, 0x15);      

    // FMFM Parameters
    vWriteDspShmWORD(W_FMFM_NUM_CHECK, 4);
    vWriteDspShmWORD(W_FMFM_NUM_DOUBLE_CHECK, 9);
    vWriteDspShmWORD(W_FMFM_MONO_WEIGHT, 1);
    vWriteDspShmWORD(W_FMFM_STEREO_WEIGHT, 1);
    vWriteDspShmWORD(W_FMFM_DUAL_WEIGHT, 1);
    vWriteDspShmDWRD( D_FMFM_MONO_FINE_TUNE_VOLUME, (0x2000) << 8);
    vWriteDspShmDWRD( D_FMFM_DUAL_FINE_TUNE_VOLUME, (0x2000) << 8);
    vWriteDspShmWORD(W_FMFM_DETECT_CONFID, 7);
    vWriteDspShmBYTE (B_FM_MUTE_THRESHOLD_HIGHT_EIAJ,   0x22); 
    vWriteDspShmBYTE (B_FM_MUTE_THRESHOLD_LOW_EIAJ,   0x20);   
    
    // A2 Parameters
    vWriteDspShmWORD(W_A2_NUM_CHECK, prA2Factory->u2NumCheck);
    vWriteDspShmWORD(W_A2_NUM_DOUBLE_CHECK,  prA2Factory->u2NumDoubleCheck);
    vWriteDspShmWORD(W_A2_MONO_WEIGHT, prA2Factory->u2MonoWeight);
    vWriteDspShmWORD(W_A2_STEREO_WEIGHT,prA2Factory->u2StereoWeight);
    vWriteDspShmWORD(W_A2_DUAL_WEIGHT, prA2Factory->u2DualWeight);
    vWriteDspShmDWRD( D_A2_FINE_TUNE_VOLUME, 
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
    // Noise reduction parameters for A2
    vWriteDspShmWORD(W_NR_ENABLE, 0x0);
    vWriteDspShmWORD(W_NR_A2_TH1_HIGH, 0x780);
    vWriteDspShmWORD(W_NR_A2_TH1_LOW, 0x700);
    vWriteDspShmWORD(W_NR_A2_TH2_HIGH, 0x2c0);
    vWriteDspShmWORD(W_NR_A2_TH2_LOW, 0x210);  
    
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
    vWriteDspShmDWRD( D_PAL_FINE_TUNE_VOLUME, 
         (VOLUME_FINE_TUNE[vFineTuneVolTblIdxChk(prPalFactory->u1PalPrescale)]) << 8);
    vWriteDspShmDWRD( D_AM_FINE_TUNE_VOLUME, 
        (VOLUME_FINE_TUNE[vFineTuneVolTblIdxChk(prPalFactory->u1AmPrescale)]) << 8);
    vWriteDspShmDWRD( D_NICAM_FINE_TUNE_VOLUME, 
        (VOLUME_FINE_TUNE[vFineTuneVolTblIdxChk(prPalFactory->u1NicamPrescale)]) << 8);
    
    // Noise reduction parameters for PAL
    vWriteDspShmWORD(W_NR_ENABLE, 0x0);
    vWriteDspShmWORD(W_NR_PAL_TH1_HIGH, 0x180);
    vWriteDspShmWORD(W_NR_PAL_TH1_LOW, 0x120);
    vWriteDspShmWORD(W_NR_PAL_TH2_HIGH, 0x51);
    vWriteDspShmWORD(W_NR_PAL_TH2_LOW, 0x3c);
    vWriteDspShmWORD(W_NR_PAL_HDEV_TH1_HIGH, 0x500);
    vWriteDspShmWORD(W_NR_PAL_HDEV_TH1_LOW, 0x400);
    vWriteDspShmWORD(W_NR_PAL_HDEV_TH2_HIGH, 0x3f);
    vWriteDspShmWORD(W_NR_PAL_HDEV_TH2_LOW, 0x38);
    vWriteDspShmWORD(W_NR_AM_TH1_HIGH, 0x400);
    vWriteDspShmWORD(W_NR_AM_TH1_LOW, 0x300);
    vWriteDspShmWORD(W_NR_AM_TH2_HIGH, 0x50);
    vWriteDspShmWORD(W_NR_AM_TH2_LOW, 0x40);
    //for PAL, check FM BAD SIGNAL 
    vWriteDspShmBYTE (B_FM_BAD_SIGNAL, 0x0);  
    vWriteDspShmBYTE (B_CANON_PLUS,   0);
       
    // Detection Parameters
    vWriteDspShmBYTE(B_DETECT_DEFAULT, 0);  // 0: PAL_DK, 1: SECAM-L
 
    //FM RADIO Parameters
    vWriteDspShmDWRD(D_FM_RDO_INBAND_TH  , 0x3000);
    vWriteDspShmDWRD(D_FM_RDO_NOISE_TH  , 0x15000);
 
    // BBE default settings
    vWriteDspShmBYTE (B_BBE_FLAG, 0x0);
    vWriteDspShmBYTE (B_BBE_LEVEL, (UINT8)DRVCUST_OptGet(eAudioBbeLevel));
    vWriteDspShmBYTE (B_BBE_PROCESS, (UINT8)DRVCUST_OptGet(eAudioBbeProcess));
    vWriteDspShmBYTE (B_BBE_LO_CONTOUR, (UINT8)DRVCUST_OptGet(eAudioBbeLoContour));
    vWriteDspShmBYTE (B_BBE_3D_GAIN, (UINT8)DRVCUST_OptGet(eAudioBbe3dGain));    
   //Audio Description setting
    vWriteDspShmBYTE (B_ADFLAG, (UINT8)DRVCUST_OptGet(eAudioADFlag));

    // Limiter mode
    vWriteDspShmWORD (W_LIMITER_MODE, (UINT8)DRVCUST_OptGet(eAudioLimiterMode));
    vWriteDspShmWORD (W_LIMITER_FLAG, 0xffff);
    vWriteDspShmDWRD (D_LIMITER_FIX_THRESHOLD, DRVCUST_OptGet(eAudioLimiterFixThreshold)); // default is 0x100000
#ifdef CC_RATIO_LIMITER
    vWriteDspShmDWRD (D_LIMITER_GAIN_RATIO, 0x07fffff); // default is 0x7fffff
#endif    

    // Audio post-processing initialization
    vWriteDspShmBYTE(B_HPF_ENABLE, 0);
    vWriteDspShmWORD(W_HPF_FC, 100);  // Fc = 100Hz

    vWriteDspShmBYTE(B_LIMITER_ENABLE, 1);
    vWriteDspShmDWRD(D_LIMITER_FLAG, 0xffffffff);  // Defualt on

    // PEQ relative setting
#ifdef CC_AUD_PEQ_INIT_ON    
    vWriteDspShmBYTE(B_PEQ_ENABLE, 1);
#else
    vWriteDspShmBYTE(B_PEQ_ENABLE, 0);
#endif
#ifdef CC_AUD_SPEAKER_HEIGHT
#ifndef CC_AUD_SPEAKER_HEIGHT_MODE
    vWriteDspShmBYTE(B_SPH_ENABLE, 0);
#else
    vWriteDspShmBYTE(B_SPH_MODE, 0);
#endif
#endif

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

    // Virtual Bass 
    vWriteDspShmBYTE(B_VIR_BASS_ENABLE, 0);
    vWriteDspShmDWRD(D_VIR_BASS_GAIN, 0x800);
    vWriteDspShmDWRD(D_VIR_BASS_GAIN_2, 0x1000);

    // Clipper enable
    vWriteDspShmBYTE(B_CLIPPER_ENABLE, 0);
    
#define CLIPPER_DEFAULT_VALUE   0x7c5e4d //0x1f1234

#if 1
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
#else
    vWriteDspShmDWRD(D_CLIPPER_C_VALUE,   CLIPPER_DEFAULT_VALUE);
    vWriteDspShmDWRD(D_CLIPPER_L_VALUE,   CLIPPER_DEFAULT_VALUE + 0x100000);
    vWriteDspShmDWRD(D_CLIPPER_R_VALUE,   CLIPPER_DEFAULT_VALUE + 0x200000);
    vWriteDspShmDWRD(D_CLIPPER_LS_VALUE,  CLIPPER_DEFAULT_VALUE + 0x300000);
    vWriteDspShmDWRD(D_CLIPPER_RS_VALUE,  CLIPPER_DEFAULT_VALUE + 0x400000);
    vWriteDspShmDWRD(D_CLIPPER_SW_VALUE,  CLIPPER_DEFAULT_VALUE + 0x500000);
    vWriteDspShmDWRD(D_CLIPPER_CH7_VALUE, CLIPPER_DEFAULT_VALUE + 0x600000);
    vWriteDspShmDWRD(D_CLIPPER_CH8_VALUE, CLIPPER_DEFAULT_VALUE + 0x000001);
    vWriteDspShmDWRD(D_CLIPPER_CH9_VALUE, CLIPPER_DEFAULT_VALUE + 0x000002);
    vWriteDspShmDWRD(D_CLIPPER_CH10_VALUE,CLIPPER_DEFAULT_VALUE + 0x000003);
#endif
    
#endif //DSP_SUPPORT_NPTV

#ifdef CC_AUD_USE_NVM
    AUD_NVM_Init_From_EEPROM();
#endif  //CC_AUD_USE_NVM  

//DDCO flag
vWriteDspShmBYTE (B_DDCO_FLAG,0x0);
vWriteDspShmBYTE (B_DDCO_AGC_FLAG,0x0);
vWriteDspShmBYTE (B_DDCO_LFE_LPF_FLAG,0x0);



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
    u2DspData = u2DspData | RAM_EXT_RISC_INT | TRAP_PRESET_ON;
    vWriteDspWORD (ADDR_RC2D_INTERRUPT_CTRL, u2DspData);
    // Light add for internal buffer control
    vWriteDspWORD (ADDR_RC2D_SYSTEM_CTRL,    u2ReadDspShmWORD (W_SYSTEM_CTRL));
    vWriteDspWORD (ADDR_RD2D_INTERNAL_BUF_IN_FLAG, u2ReadDspShmWORD (W_INTERNAL_BUF_IN_FLAG));
    vWriteDspWORD (ADDR_RC2D_SYSTEM_SETUP,   u2ReadDspShmWORD (W_SYSTEM_SETUP));
    vWriteDspWORD (ADDR_RC2D_SYSTEM_SETUP_DEC2, 
                   u2ReadDspShmWORD (W_SYSTEM_SETUP_DEC2));
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
        vWriteDspWORD (ADDR_RC2D_SYSTEM_SETUP_DEC3, 
                   u2ReadDspShmWORD (W_SYSTEM_SETUP_DEC3));
    vWriteDspWORD (ADDR_RD2D_INTERNAL_BUF2_IN_FLAG, u2ReadDspShmWORD (W_INTERNAL_BUF2_IN_FLAG));               
#endif

    //Volume
    WriteDspCommDram (ADDR_RC2D_MASTER_VOLUME, u4ReadDspShmDWRD (D_VOL));
    WriteDspCommDram (ADDR_RC2D_MASTER_VOLUME_BACKUP, u4ReadDspShmDWRD (D_VOL));
    WriteDspCommDram (ADDR_RC2D_MASTER_VOLUME_DEC2,
                      u4ReadDspShmDWRD (D_VOL_DEC2));
    WriteDspCommDram (ADDR_RC2D_MASTER_VOLUME_BACKUP_DEC2,
                      u4ReadDspShmDWRD (D_VOL_DEC2));
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
    WriteDspCommDram (ADDR_RC2D_MASTER_VOLUME_DEC3,
                      u4ReadDspShmDWRD (D_VOL_DEC3));
    WriteDspCommDram (ADDR_RC2D_MASTER_VOLUME_BACKUP_DEC3,
                      u4ReadDspShmDWRD (D_VOL_DEC3));
#endif                      

    //Input Src Gain
    WriteDspCommDram (ADDR_RC2D_INPUT_SRC_GAIN, u4ReadDspShmDWRD (D_INSRC_GAIN));
    WriteDspCommDram (ADDR_RC2D_INPUT_SRC_GAIN_DEC2,
                      u4ReadDspShmDWRD (D_INSRC_GAIN_DEC2));
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
        vWriteDspWORD (ADDR_RC2D_INPUT_SRC_GAIN_DEC3, 
                   u2ReadDspShmWORD (D_INSRC_GAIN_DEC3));
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
    WriteDspCommDram (ADDR_RC2D_DIALOGUE_GAIN_DEC2,
                      u4ReadDspShmDWRD (D_DIALOGUE_DEC2));

    WriteDspCommDram (ADDR_RC2D_CH_TRIM_DEC2_L,
                      u4ReadDspShmDWRD (D_TRIM_L_DEC2));
    WriteDspCommDram (ADDR_RC2D_CH_TRIM_DEC2_R,
                      u4ReadDspShmDWRD (D_TRIM_R_DEC2));
    WriteDspCommDram (ADDR_RC2D_CH_TRIM_DEC2_LFE,
                      u4ReadDspShmDWRD (D_TRIM_LFE_DEC2));

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
        


    /* ------- Setup IEC ------- */
    IecInfoInit ();

    /* ------- Setup Customer Option ------ */
    vCustomOptInit ();

//Memory backup address
    WriteDspCommDram (ADDR_RC2D_MEMORY_BUFFER_ADDRESS,
                      u4ReadDspShmDWRD (D_MEMBACKUPADDR));
    WriteDspCommDram (ADDR_RC2D_MEMORY_BUFFER_ADDRESS_DEC2,
                      u4ReadDspShmDWRD (D_MEMBACKUPADDR_DEC2));
//#ifdef CC_MT5391_AUD_3_DECODER
#if 0
        vWriteDspWORD (ADDR_RC2D_MEMORY_BUFFER_ADDRESS_DEC3, 
                   u2ReadDspShmWORD (D_MEMBACKUPADDR_DEC3));
#endif                      
                      
//DRC
    vSetDRCInfo ();

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

#ifdef CC_AUD_NEW_POST_PROC

    WriteDspCommDram (ADDR_RC2D_AVC_TARGET_LEVEL_DEC2,
                     (UINT16)(u2ReadDspShmWORD (W_AVC_TARGET_LEV_DEC2))<<8);
    WriteDspCommDram (ADDR_RC2D_AVC_SILENCE_LEVEL_DEC2,
                      u2ReadDspShmWORD (W_AVC_SILENCE_LEV_DEC2)<<8);
    WriteDspCommDram (ADDR_RC2D_AVC_MAX_GAIN_UP_DEC2,
                      u2ReadDspShmWORD (W_AVC_MAX_GAIN_UP_DEC2)<<8);
    WriteDspCommDram (ADDR_RC2D_AVC_MAX_GAIN_DOWN_DEC2,
                      u2ReadDspShmWORD (W_AVC_MAX_GAIN_DOWN_DEC2)<<8);
    WriteDspCommDram (ADDR_RC2D_AVC_FLAG_DEC2,
                      u2ReadDspShmWORD (W_AVC_FLAG_DEC2)<<8);
    WriteDspCommDram (ADDR_RC2D_AVC_ATTACK_THRES_DEC2,
                      u2ReadDspShmWORD (W_AVC_ATTACK_THRES_DEC2)<<8);
    WriteDspCommDram (ADDR_RC2D_AVC_ADJUST_RATE_DEC2,
                      u2ReadDspShmWORD (W_AVC_ADJUST_RATE_DEC2));
    WriteDspCommDram (ADDR_RC2D_AVC_UI_ADJUST_RATE_DEC2,
                      u2ReadDspShmWORD (W_DSP_AVC_UI_ADJUST_RATE_DEC2));
    
#endif
//-----------------

//Reverb memory address
    WriteDspCommDram (ADDR_RC2D_REVERB_BANK_ADDR,
                      u4ReadDspShmDWRD (D_REVERBMEMADDR));
    WriteDspCommDram (ADDR_RC2D_REVERB_BANK_ADDR_DEC2,
                      u4ReadDspShmDWRD (D_REVERBMEMADDR_DEC2));

//Audio Output Bank Number
    //Audio Output Bank 576 Number
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_576_NUM,
                   uReadDspShmBYTE (B_BANK576NUM));
    //Audio Output Bank 384 Number
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_384_NUM,
                   uReadDspShmBYTE (B_BANK384NUM));
    //Audio Output Bank 320 Number
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_320_NUM,
                   uReadDspShmBYTE (B_BANK320NUM));
    //Audio Output Bank 256 Number
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_256_NUM,
                   uReadDspShmBYTE (B_BANK256NUM));
//Audio Output Bank Number for second decoder
    //Audio Output Bank 576 Number
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_576_NUM_DEC2,
                   uReadDspShmBYTE (B_BANK576NUM_DEC2));
    //Audio Output Bank 384 Number
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_384_NUM_DEC2,
                   uReadDspShmBYTE (B_BANK384NUM_DEC2));
    //Audio Output Bank 320 Number
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_320_NUM_DEC2,
                   uReadDspShmBYTE (B_BANK320NUM_DEC2));
    //Audio Output Bank 256 Number
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_256_NUM_DEC2,
                   uReadDspShmBYTE (B_BANK256NUM_DEC2));
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
//Audio Output Bank Number for 3rd decoder
    //Audio Output Bank 576 Number
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_576_NUM_DEC3,
                   uReadDspShmBYTE (B_BANK576NUM_DEC3));
    //Audio Output Bank 384 Number
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_384_NUM_DEC3,
                   uReadDspShmBYTE (B_BANK384NUM_DEC3));
    //Audio Output Bank 320 Number
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_320_NUM_DEC3,
                   uReadDspShmBYTE (B_BANK320NUM_DEC3));
    //Audio Output Bank 256 Number
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
//Volume Smooth (Max. 15)
    vWriteDspWORD (ADDR_RC2D_VOLUME_SMOOTH_ORDER, (UINT16) 8); // Light modified for AVC, order should be <= 8
//Volume LEVEL UP/DOWN ORDER for second decoder
    vWriteDspWORD (ADDR_RC2D_VOLUME_LEVEL_UP_ORDER_DEC2,
                   uReadDspShmBYTE (B_VOLUPORDER_DEC2));
    vWriteDspWORD (ADDR_RC2D_VOLUME_LEVEL_DOWN_ORDER_DEC2,
                   uReadDspShmBYTE (B_VOLDOWNORDER_DEC2));
//Soft Mute (Max. 7)
    vWriteDspWORD (ADDR_RC2D_VOLUME_SOFT_MUTE_ORDER_DEC2,
                   uReadDspShmBYTE (B_SOFTMUTEORDER_DEC2));
//Volume Smooth (Max. 15)
    vWriteDspWORD (ADDR_RC2D_VOLUME_SMOOTH_ORDER_DEC2, (UINT16) 8);
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
vWriteDspWORD (ADDR_RC2D_VOLUME_SMOOTH_ORDER_DEC3, (UINT16) 8);
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

    _tSrcSpkCfg.wDspSpkCfg = 0xFFFF;
    _tSrcSpkCfgDec2.wDspSpkCfg = 0xFFFF;

    // set stream status initial value
    // dsp polling for MPEG4
    vWriteDspWORD (ADDR_RC2D_POLL_DRAM_TIMER_CNT, 0);
    vWriteDspWORD (ADDR_RC2D_POLL_DRAM_READ_UINT32_CNT_1, 0);
}

/***************************************************************************
     Function : vDspPtsReset(void)
  Description :
    Parameter :
    Return    :
***************************************************************************/
void vDspPtsReset (void)
{


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

//sunman for play speed
#ifdef FRAC_SPEED_SUPPORT
    WriteDspCommDram(ADDR_RC2D_PLAY_SPEED, 	u4ReadDspShmDWRD(D_PLAY_SPEED));    
#endif

    // Light add for internal buffer control
    vWriteDspWORD (ADDR_RC2D_SYSTEM_CTRL,    u2ReadDspShmWORD (W_SYSTEM_CTRL));
    vWriteDspWORD (ADDR_RD2D_INTERNAL_BUF_IN_FLAG, u2ReadDspShmWORD (W_INTERNAL_BUF_IN_FLAG));
    vWriteDspWORD (ADDR_MANUAL_OUTPUT_MATRIX, uReadDspShmBYTE (B_MANUAL_OUTPUT_MATRIX));  
#ifdef CC_MT5391_AUD_3_DECODER    
    vWriteDspWORD (ADDR_RD2D_INTERNAL_BUF2_IN_FLAG, u2ReadDspShmWORD (W_INTERNAL_BUF2_IN_FLAG));
#endif

// New virtual bass
#if 0
    vWriteDspWORD (ADDR_VBASS_CROS_PARA4, 0x0800);
    vWriteDspWORD (ADDR_VBASS_CROS_PARA7, 0x1000);
#else
    vWriteDspWORD (ADDR_VBASS_CROS_PARA4, u4ReadDspShmDWRD(D_VIR_BASS_GAIN));
    vWriteDspWORD (ADDR_VBASS_CROS_PARA7, u4ReadDspShmDWRD(D_VIR_BASS_GAIN_2));
#endif
    vWriteDspWORD (ADDR_VBASS_CROS_PARA5, 0x3);
    vWriteDspWORD (ADDR_VBASS_CROS_PARA6, 0x1000);//0x1000,0db
    vWriteDspWORD (ADDR_EXPEND_GAIN, 0x3);
    vWriteDspWORD (ADDR_dram_attck_count, 0x1000);
    vWriteDspWORD (ADDR_VBASS_CROS_PARA8, 0x1000); 
    vWriteDspWORD (ADDR_VBASS_CROS_PARA9, 0x1000);    


    
#ifndef DSP_SUPPORT_SRSWOW
#ifndef DSP_SUPPORT_SRSTSXT
#ifndef DSP_SUPPORT_SRSTSHD //sunman_tshd
#ifdef CC_AUD_OLD_MTK_VSURR
    // New MTK Vsurr
    vWriteDspWORD (ADDR_VSURR_CROS_PARA1, 0x3);
    vWriteDspWORD (ADDR_VSURR_CROS_PARA2, 0x1);
    vWriteDspWORD (ADDR_VSURR_CROS_PARA3, 0x0);
    WriteDspCommDram (ADDR_VSURR_GAIN_MPY, 0x250000);
    vWriteDspWORD (ADDR_VSURR_GAIN_SL, 0x4);
    vWriteDspWORD (ADDR_VSURR_OUT_OF_PHASE, 0x0);
    vWriteDspWORD (ADDR_VSURR_DELAY_ON, 0x0);
    vWriteDspWORD (ADDR_VSURR_DELAY_SAMPLE_NUM_L, 0x300);
    vWriteDspWORD (ADDR_VSURR_DELAY_SAMPLE_NUM_R, 0x300);
#else
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
#endif
#endif  //DSP_SUPPORT_SRSTSHD
#endif  // DSP_SUPPORT_SRSTSXT
#endif  // DSP_SUPPORT_SRSWOW

#ifdef CC_AUD_BBE_SUPPORT
    // BBE VIVA 4
    WriteDspCommDram (ADDR_BBEV4_LEVEL_COEF, BBEV4_LEVEL_DB[uReadDspShmBYTE (B_BBE_LEVEL)]);
    WriteDspCommDram (ADDR_BBEV4_3D_GAIN, BBEV4_3D_GAIN[uReadDspShmBYTE (B_BBE_3D_GAIN)]);
    WriteDspCommDram (ADDR_BBEV4_LOW_COEF, BBEV4_DB_8[uReadDspShmBYTE (B_BBE_LO_CONTOUR)]); // 7dB
    WriteDspCommDram (ADDR_BBEV4_HIGH_COEF, BBEV4_DB_8[uReadDspShmBYTE (B_BBE_PROCESS)]);  // 7dB 

    vWriteDspWORD (ADDR_BBEV4_BBE_ON, 0x1);
    vWriteDspWORD (ADDR_BBEV4_LOW_ON, 0x1);
    vWriteDspWORD (ADDR_BBEV4_MID_ON, 0x1);
    vWriteDspWORD (ADDR_BBEV4_HIGH_ON, 0x1);     
#endif // CC_AUD_BBE_SUPPORT
    
    // Light: AVC need different initial mode for channel changing and UI Off/On setting (aout reinitial).
    WriteDspCommDram (ADDR_RC2D_AVC_FLAG, u2ReadDspShmWORD (W_AVC_FLAG)<<8);
    
    //System Setup
    vWriteDspWORD (ADDR_RC2D_SYSTEM_SETUP, u2ReadDspShmWORD (W_SYSTEM_SETUP));
    
    vWriteDspWORD (ADDR_RC2D_CTRL_2_NUM_1, (UINT16) CTRL_2_NUM_1);

    //Input Src Gain
    WriteDspCommDram (ADDR_RC2D_INPUT_SRC_GAIN, u4ReadDspShmDWRD (D_INSRC_GAIN));

    //WriteDspCommDram (ADDR_RC2D_STC_DIFF, _dwStcDiff);
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
    // Audio Output Bank 576 Number
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_576_NUM,
                   uReadDspShmBYTE (B_BANK576NUM));

    //Audio Output Bank 384 Number
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_384_NUM,
                   uReadDspShmBYTE (B_BANK384NUM));

    //Audio Output Bank 320 Number
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_320_NUM,
                   uReadDspShmBYTE (B_BANK320NUM));

    //Audio Output Bank 256 Number
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_256_NUM,
                   uReadDspShmBYTE (B_BANK256NUM));


    //Processing Mode
    u2DspData = u2ReadDspShmWORD (W_PROCMOD);
    if (_bDspStrTyp != AC3_STREAM)
    {
        u2DspData &= 0xffdf;
    }
    
    vWriteDspWORD (ADDR_RC2D_PROCESSING_MODE, u2DspData);

    // Strange <- sammy
    //Bit Resolution (24 bit)
    vWriteDspWORD (ADDR_RC2D_BIT_RESOLUTION, uReadDspShmBYTE (B_BIT));

    //IEC Switch
    //vIECCommonInit (TRUE);

    //Decoding Relation Information
    vDecCommonInit ();
    //IEC Latency
    vWriteDspWORD (ADDR_RC2D_IEC_LATENCY, uReadDspShmBYTE (B_IECLATENCY));

    //Equalizer Switch
    vWriteDspWORD (ADDR_RC2D_EQUALIZER_FLAG, uReadDspShmBYTE (B_EQFLAG));
#ifdef CC_NEW_POST_MULTI_VS_SUPPORT
	vWriteDspWORD (ADDR_RC2D_NEWPOST_FLAG, 
				  ((wReadDspWORD(ADDR_RC2D_NEWPOST_FLAG)) | NEWPOST_FLAG_M2S_BIT));
#endif    

#if 0
    //Equalizer Band Number - 1 (Max. 9)
    vWriteDspWORD (ADDR_RC2D_EQUALIZER_BAND_NUM_1, (AUD_EQ_IIR_BAND_NUM-1));

    //Equalizer Dry Gain
    WriteDspCommDram (ADDR_RC2D_EQUALIZER_DRY_LEVEL,
        (u4ReadDspShmDWRD (D_EQ_DRY)));
            
    //Equalizer Shift Bit
    vWriteDspWORD (ADDR_RC2D_EQUALIZER_BAND1_SHIFT_BIT, 0x1); // 2.22 format
    vWriteDspWORD (ADDR_RC2D_EQUALIZER_BAND2_SHIFT_BIT, 0x4); // 5.19 format
#ifdef CC_AUD_7BAND_EQUALIZER
    vWriteDspWORD (ADDR_RC2D_EQUALIZER_BAND2_SHIFT_BIT, 0x3); // 4.20 format
#elif defined (CC_AUD_EQ_SETA)
    vWriteDspWORD (ADDR_RC2D_EQUALIZER_BAND2_SHIFT_BIT, 0x2); // 3.21 format
#endif    

    // Band Gain
    vEQBandGainFlush(AUD_DEC_MAIN);
#endif

#ifdef DSP_EQ2_SBASS_SUPPORT
    vEQ2SbassBandGainFlush(AUD_DEC_MAIN);        
#else
    //Super Bass Delay Number (Max. 7, Min. 1)
    vWriteDspWORD (ADDR_RC2D_SUPER_BASS_DELAY_NUMBER,
                   uReadDspShmBYTE (B_SBASSDELAYNUM));
    //Super Bass Boost Gain
    WriteDspCommDram (ADDR_RC2D_SUPER_BASS_BOOST_GAIN,
                      u4ReadDspShmDWRD (D_SBASSBOOSTGAIN));
    WriteDspCommDram (ADDR_RC2D_SUPER_BASS_BOOST_GAIN_ORG,
                      u4ReadDspShmDWRD (D_SBASSBOOSTGAIN));                      

    //Super Bass Clear Gain
    WriteDspCommDram (ADDR_RC2D_SUPER_BASS_CLEAR_GAIN,
                      u4ReadDspShmDWRD (D_SBASSCLEARGAIN));
    WriteDspCommDram (ADDR_RC2D_SUPER_BASS_CLEAR_GAIN_ORG,
                      u4ReadDspShmDWRD (D_SBASSCLEARGAIN));  
#endif                      

    //Speaker Config
    vSetSpeakerConfig (TRUE);

    //Prologic II
    vWriteDspWORD (ADDR_RC2D_PRO_LOGIC_II_MODE, u2ReadDspShmWORD (W_PLIIMODE));
    vWriteDspWORD (ADDR_RC2D_PRO_LOGIC_II_CONFIG,
                   u2ReadDspShmWORD (W_PLIICONFIG));

    // Virtual surround flag
#ifndef CC_NEW_POST_MULTI_VS_SUPPORT
    vWriteDspWORD (ADDR_RC2D_VIRTUAL_SURROUND, uReadDspShmBYTE (B_VSURRFLAG));
#else
    vWriteDspWORD (ADDR_RC2D_VIRTUAL_SURROUND, u2ReadDspShmWORD(W_VSURRFLAG));
#endif

#ifdef CC_AUD_SPEAKER_HEIGHT
    WriteDspCommDram (ADDR_SPKHIGH_PARA4, 0x0f0000);
    WriteDspCommDram (ADDR_SPKHIGH_PARA5, 0x300);
#endif

    //Reverb Flag
    vWriteDspWORD (ADDR_RC2D_REVERB_FLAG, uReadDspShmBYTE (B_REVERBFLAG));

    // SPDIF Line/in
    vWriteDspWORD(ADDR_RC2D_SPDIF_FLAG, AUD_IsSpdifInSrc(AUD_DEC_MAIN));

	// DDCO flag
//    vWriteDspWORD (ADDR_RC2D_DDCO_FLAG, 0x0);	//only initial at power on
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

    // Setup HPF enable bit
    WriteDspCommDram (ADDR_RC2D_POST_PROC_ENABLE, 
                ((dReadDspCommDram(ADDR_RC2D_POST_PROC_ENABLE)) & ~ADDR_RC2D_POST_PROC_HPF_BIT) |
                ((uReadDspShmBYTE(B_HPF_ENABLE) > 0) ? ADDR_RC2D_POST_PROC_HPF_BIT : 0));
    
    // Setup clipper enable bit
    WriteDspCommDram (ADDR_RC2D_POST_PROC_ENABLE, 
                ((dReadDspCommDram(ADDR_RC2D_POST_PROC_ENABLE)) & ~ADDR_RC2D_POST_PROC_CLIPPER_BIT) |
                ((uReadDspShmBYTE(B_CLIPPER_ENABLE) > 0) ? ADDR_RC2D_POST_PROC_CLIPPER_BIT : 0));

    // Setup limiter enable bit
    WriteDspCommDram (ADDR_RC2D_POST_PROC_ENABLE, 
                ((dReadDspCommDram(ADDR_RC2D_POST_PROC_ENABLE)) & ~ADDR_RC2D_POST_PROC_LIMITER_BIT) |
                ((uReadDspShmBYTE(B_LIMITER_ENABLE) > 0) ? ADDR_RC2D_POST_PROC_LIMITER_BIT : 0));

#ifdef CC_AUD_NEW_POST_PROC

    // Setup clipper value
    WriteDspCommDram(ADDR_RC2D_POST_PROC_CLIPPER_C_VAL, u4ReadDspShmDWRD(D_CLIPPER_C_VALUE));
    WriteDspCommDram(ADDR_RC2D_POST_PROC_CLIPPER_L_VAL, u4ReadDspShmDWRD(D_CLIPPER_L_VALUE));
    WriteDspCommDram(ADDR_RC2D_POST_PROC_CLIPPER_R_VAL, u4ReadDspShmDWRD(D_CLIPPER_R_VALUE));
    WriteDspCommDram(ADDR_RC2D_POST_PROC_CLIPPER_LS_VAL, u4ReadDspShmDWRD(D_CLIPPER_LS_VALUE));
    WriteDspCommDram(ADDR_RC2D_POST_PROC_CLIPPER_RS_VAL, u4ReadDspShmDWRD(D_CLIPPER_RS_VALUE));
    WriteDspCommDram(ADDR_RC2D_POST_PROC_CLIPPER_SW_VAL, u4ReadDspShmDWRD(D_CLIPPER_SW_VALUE));
    WriteDspCommDram(ADDR_RC2D_POST_PROC_CLIPPER_CH7_VAL, u4ReadDspShmDWRD(D_CLIPPER_CH7_VALUE));
    WriteDspCommDram(ADDR_RC2D_POST_PROC_CLIPPER_CH8_VAL, u4ReadDspShmDWRD(D_CLIPPER_CH8_VALUE));
    WriteDspCommDram(ADDR_RC2D_POST_PROC_CLIPPER_CH9_VAL, u4ReadDspShmDWRD(D_CLIPPER_CH9_VALUE));
    WriteDspCommDram(ADDR_RC2D_POST_PROC_CLIPPER_CH10_VAL, u4ReadDspShmDWRD(D_CLIPPER_CH10_VALUE));

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
    // Setup PEQ relative 
    vWriteDspWORD (ADDR_RC2D_NEWPOST_FLAG, 
                ((wReadDspWORD(ADDR_RC2D_NEWPOST_FLAG)) & ~NEWPOST_FLAG_PEQ_BIT) |
                ((uReadDspShmBYTE(B_PEQ_ENABLE) > 0) ? NEWPOST_FLAG_PEQ_BIT : 0));
   
#ifndef CC_NEW_POST_SUPPORT
    // Load Fc, Gain     
    for (i=0; i<8; i++)
    {   // FIXME
        WriteDspCommDram (ADDR_PEQ1_FC + i*3, u2ReadDspShmWORD(W_PEQ_SET0_FC + i*8));
   	    WriteDspCommDram (ADDR_PEQ1_GAIN + i*3, u4ReadDspShmDWRD(D_PEQ_SET0_GAIN + i*8));
   	    WriteDspCommDram (ADDR_PEQ1_BW + i*3, MTK_TAN_TABLE[dwGetPEQFsIdx(i)] );    
    }
#else	
        INT16 j;
        UINT32 temp;
        for (i=0; i<9; i++)
        {   
            WriteDspCommDram (ADDR_PEQ1_FC + i*3, u2ReadDspShmWORD(W_PEQ_SET0_FC + i*8));
            WriteDspCommDram (ADDR_PEQ1_GAIN + i*3, u4ReadDspShmDWRD(D_PEQ_SET0_GAIN + i*8));
            j=dwGetPEQFsIdx(i) ;
            VERIFY(j < sizeof(MTK_TAN_TABLE)/4);
            temp=MTK_TAN_TABLE[j];
                 
           if (j > sizeof(MTK_TAN_TABLE)/4)
           {
                      j = (sizeof(MTK_TAN_TABLE)/2) - j;
                      temp=-(MTK_TAN_TABLE[j]);
           }
                    
            WriteDspCommDram (ADDR_PEQ1_BW + i*3, temp); 
        }
#endif	

    vWriteDspWORD (ADDR_RC2D_NEWPOST_FLAG, 
                ((wReadDspWORD(ADDR_RC2D_NEWPOST_FLAG)) & ~NEWPOST_FLAG_VIR_BASS_BIT) |
                ((uReadDspShmBYTE(B_VIR_BASS_ENABLE) > 0) ? NEWPOST_FLAG_VIR_BASS_BIT : 0));

#ifdef CC_AUD_USE_FLASH_AQ
    vDspFlashAQOperation(AQ_TYPE_LIMITER);
    vDspFlashAQOperation(AQ_TYPE_SPKHEIGHT);
#endif //CC_AUD_USE_FLASH_AQ


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
    vWriteDspWORD (ADDR_RD2D_INTERNAL_BUF_IN_FLAG, u2ReadDspShmWORD (W_INTERNAL_BUF_IN_FLAG));
    vWriteDspWORD (ADDR_MANUAL_OUTPUT_MATRIX_DEC2, uReadDspShmBYTE (B_MANUAL_OUTPUT_MATRIX_DEC2));  
    
    //System Setup
    vWriteDspWORD (ADDR_RC2D_SYSTEM_SETUP_DEC2, u2ReadDspShmWORD (W_SYSTEM_SETUP_DEC2));
    
    //Input Src Gain
    WriteDspCommDram (ADDR_RC2D_INPUT_SRC_GAIN_DEC2, u4ReadDspShmDWRD (D_INSRC_GAIN_DEC2));
    
    /* Initial ctrl_4 */
    vWriteDspWORD (ADDR_RC2D_CTRL_4_NUM_1, (UINT16) CTRL_4_NUM_1);
//  WriteDspCommDram(ADDR_RC2D_STC_DIFF_DEC2, _dwStcDiffDec2);
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
    // Audio Output Bank 576 Number
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_576_NUM_DEC2,
                   uReadDspShmBYTE (B_BANK576NUM_DEC2));
    //Audio Output Bank 384 Number
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_384_NUM_DEC2,
                   uReadDspShmBYTE (B_BANK384NUM_DEC2));
    //Audio Output Bank 320 Number
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_320_NUM_DEC2,
                   uReadDspShmBYTE (B_BANK320NUM_DEC2));
    //Audio Output Bank 256 Number
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_256_NUM_DEC2,
                   uReadDspShmBYTE (B_BANK256NUM_DEC2));

//Processing Mode
    u2DspData = u2ReadDspShmWORD (W_PROCMOD_DEC2);
    if (_bDspStrTypDec2 != AC3_STREAM)
    {
        u2DspData &= 0xffdf;
    }
    
    vWriteDspWORD (ADDR_RC2D_PROCESSING_MODE_DEC2, u2DspData);
//Bit Resolution (24 bit)
    vWriteDspWORD (ADDR_RC2D_BIT_RESOLUTION_DEC2,
                   uReadDspShmBYTE (B_BIT_DEC2));

//IEC Switch
    //vIECCommonInit (FALSE);
//Decoding Relation Information
    vDecCommonInitDec2 ();
//IEC Latency
    vWriteDspWORD (ADDR_RC2D_IEC_LATENCY_DEC2,
                   uReadDspShmBYTE (B_IECLATENCY));
#if 0
//Equalizer Switch
    vWriteDspWORD (ADDR_RC2D_EQUALIZER_FLAG_DEC2, uReadDspShmBYTE (B_EQFLAG_DEC2));

    //Equalizer Band Number - 1 (Max. 9)
    vWriteDspWORD (ADDR_RC2D_EQUALIZER_BAND_NUM_1_DEC2, (AUD_EQ_IIR_BAND_NUM-1));

    //Equalizer Dry Gain
    WriteDspCommDram (ADDR_RC2D_EQUALIZER_DRY_LEVEL_DEC2,
                          (u4ReadDspShmDWRD (D_EQ_DRY_DEC2)));

    //Equalizer Shift Bit
    vWriteDspWORD (ADDR_RC2D_EQUALIZER_BAND1_SHIFT_BIT_DEC2, 0x1); // 2.22 format
#ifdef CC_AUD_EQ_SETA
    vWriteDspWORD (ADDR_RC2D_EQUALIZER_BAND2_SHIFT_BIT_DEC2, 0x2); // 3.21 format
#else
    vWriteDspWORD (ADDR_RC2D_EQUALIZER_BAND2_SHIFT_BIT_DEC2, 0x3); // 4.20 format
#endif    

    // Band Gain
    vEQBandGainFlush(AUD_DEC_AUX);
#endif    

#ifdef DSP_EQ2_SBASS_SUPPORT
    vEQ2SbassBandGainFlush(AUD_DEC_AUX);
#else    
    //Super Bass Delay Number (Max. 7, Min. 1)
    vWriteDspWORD (ADDR_RC2D_SUPER_BASS_DELAY_NUMBER_DEC2,
                   uReadDspShmBYTE (B_SBASSDELAYNUM_DEC2));
    //Super Bass Boost Gain
    WriteDspCommDram (ADDR_RC2D_SUPER_BASS_BOOST_GAIN_DEC2,
                      u4ReadDspShmDWRD (D_SBASSBOOSTGAIN_DEC2));

    //Super Bass Clear Gain
    WriteDspCommDram (ADDR_RC2D_SUPER_BASS_CLEAR_GAIN_DEC2,
                      u4ReadDspShmDWRD (D_SBASSCLEARGAIN_DEC2));
#endif                      

    //Speaker Config
    vSetSpeakerConfig (FALSE);
    // Virtual surround flag
#ifndef CC_NEW_POST_MULTI_VS_SUPPORT
    vWriteDspWORD (ADDR_RC2D_VIRTUAL_SURROUND_DEC2, uReadDspShmBYTE (B_VSURRFLAG_DEC2));
#else
    vWriteDspWORD (ADDR_RC2D_VIRTUAL_SURROUND_DEC2, u2ReadDspShmWORD (W_VSURRFLAG_DEC2));
#endif
    //Reverb Flag
    //vWriteDspWORD (ADDR_RC2D_REVERB_FLAG_DEC2,uReadDspShmBYTE (B_REVERBFLAG_DEC2));

    // SPDIF Line/in
    vWriteDspWORD(ADDR_RC2D_SPDIF_FLAG_DEC2, AUD_IsSpdifInSrc(AUD_DEC_AUX));
    vWriteDspWORD(ADDR_RC2D_MPEG_SYNC_MODE_DEC2, 0x1);
    
    // Audio mute threshold
    WriteDspCommDram (ADDR_RC2D_AUDIO_MUTE_THRESHOLD_DEC2, 0x0);
}

#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
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

      // Light add for internal buffer control
    vWriteDspWORD (ADDR_RC2D_SYSTEM_CTRL,    u2ReadDspShmWORD (W_SYSTEM_CTRL));
    vWriteDspWORD (ADDR_RD2D_INTERNAL_BUF_IN_FLAG, u2ReadDspShmWORD (W_INTERNAL_BUF_IN_FLAG));
    vWriteDspWORD (ADDR_RD2D_INTERNAL_BUF2_IN_FLAG, u2ReadDspShmWORD (W_INTERNAL_BUF2_IN_FLAG));    
    
    //System Setup
    vWriteDspWORD (ADDR_RC2D_SYSTEM_SETUP_DEC3, u2ReadDspShmWORD (W_SYSTEM_SETUP_DEC3));
    
    //Input Src Gain
    WriteDspCommDram (ADDR_RC2D_INPUT_SRC_GAIN_DEC3, u4ReadDspShmDWRD (D_INSRC_GAIN_DEC3));
    
    /* Initial ctrl_4 */ //gallen, needs to modify accroding to new MT539x DSP
    vWriteDspWORD (ADDR_RC2D_CTRL_7_NUM_1, (UINT16) CTRL_7_NUM_1);
// Reset aout bank number to default value for some decoding will change it
    // Audio Output Bank 576 Number
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_576_NUM_DEC3,
                   uReadDspShmBYTE (B_BANK576NUM_DEC3));
    //Audio Output Bank 384 Number
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_384_NUM_DEC3,
                   uReadDspShmBYTE (B_BANK384NUM_DEC3));
    //Audio Output Bank 320 Number
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_320_NUM_DEC3,
                   uReadDspShmBYTE (B_BANK320NUM_DEC3));
    //Audio Output Bank 256 Number
    vWriteDspWORD (ADDR_RC2D_AOUT_BANK_256_NUM_DEC3,
                   uReadDspShmBYTE (B_BANK256NUM_DEC3));

//Processing Mode
    u2DspData = u2ReadDspShmWORD (W_PROCMOD_DEC3);    
    vWriteDspWORD (ADDR_RC2D_PROCESSING_MODE_DEC3, u2DspData);
//Bit Resolution (24 bit)
    vWriteDspWORD (ADDR_RC2D_BIT_RESOLUTION_DEC3,
                   uReadDspShmBYTE (B_BIT_DEC3));

//IEC Switch
    //vIECCommonInit (FALSE);
//Decoding Relation Information
    vDecCommonInitDec3 ();

    //Speaker Config
    //vSetSpeakerConfig (FALSE);
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



